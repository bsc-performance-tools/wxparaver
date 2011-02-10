/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                                  wxparaver                                *
 *              Paraver Trace Visualization and Analysis Tool                *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

// Signal handling
#include <signal.h>
#include <stdio.h>

#include "wxparaverapp.h"
#include "connection.h"

////@begin XPM images
////@end XPM images

#ifndef WIN32
struct sigaction act;
#endif

/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( wxparaverApp )
////@end implement app


/*!
 * wxparaverApp type definition
 */

IMPLEMENT_CLASS( wxparaverApp, wxApp )


/*!
 * wxparaverApp event table definition
 */

BEGIN_EVENT_TABLE( wxparaverApp, wxApp )

////@begin wxparaverApp event table entries
////@end wxparaverApp event table entries

END_EVENT_TABLE()


/*!
 * Constructor for wxparaverApp
 */

wxparaverApp::wxparaverApp()
{
  Init();
}


/*!
 * Member initialisation
 */

void wxparaverApp::Init()
{
////@begin wxparaverApp member initialisation
	globalTiming = false;
	globalTimingBegin = 0;
	globalTimingEnd = 0;
	globalTimingCallDialog = NULL;
	globalTimingBeginIsSet = false;
	eventTypeForCode = 60000119;
////@end wxparaverApp member initialisation
  m_locale.Init();
}

/*!
 * Initialisation for wxparaverApp
 */

paraverMain* wxparaverApp::mainWindow = NULL;


#ifndef WIN32
volatile bool sig1 = false;
volatile bool sig2 = false;

void wxparaverApp::handler( int signalNumber )
{
  sigdelset( &act.sa_mask, SIGUSR1 );
  sigdelset( &act.sa_mask, SIGUSR2 );

//  wxMutexGuiEnter();
//  wxparaverApp::mainWindow->OnSignal( signalNumber );
//  wxMutexGuiLeave();
  if ( signalNumber == SIGUSR1 )
  {
    sig1 = true;
  }
  else
  {
    sig1 = false;
  }

  sig2 = !sig1;

//  wxIdleEvent event;
//  event.RequestMore();
//  wxTheApp->SendIdleEvents( mainWindow, event );
  mainWindow->Raise();

  sigaddset( &act.sa_mask, SIGUSR1 );
  sigaddset( &act.sa_mask, SIGUSR2 );
  sigaction( SIGUSR1, &act, NULL );
  sigaction( SIGUSR2, &act, NULL );
}


void wxparaverApp::presetUserSignals()
{

  act.sa_handler = &handler;
  act.sa_flags = 0;

  if ( sigemptyset( &act.sa_mask ) != 0 )
  {
    /* Handle error */
  }

  if ( sigaddset( &act.sa_mask, SIGUSR1 ))
  {
    /* Handle error */
  }

  if ( sigaddset( &act.sa_mask, SIGUSR2 ))
  {
    /* Handle error */
  }

  if ( sigaction( SIGUSR1, &act, NULL ) != 0 )
  {
    /* Handle error */
  }

  if ( sigaction( SIGUSR2, &act, NULL ) != 0 )
  {
    /* Handle error */
  }
}
#endif


bool wxparaverApp::OnInit()
{
  ParaverConfig::getInstance()->readParaverConfigFile();
  
  if( ParaverConfig::getInstance()->getGlobalSingleInstance() )
  {
    const wxString name = wxString::Format( _( "wxparaver-%s" ), wxGetUserId().c_str());
    m_checker = new wxSingleInstanceChecker(name);
    if ( !m_checker->IsAnotherRunning() )
    {
      m_server = new stServer;
    
#ifdef WIN32
      if( !m_server->Create( wxT( "wxparaver_service" ) ) )
#else
      const wxString service_name = wxString::Format( _( "/tmp/wxparaver_service-%s" ), wxGetUserId().c_str());
      if( !m_server->Create( service_name ) )
#endif
        wxLogDebug( wxT( "Failed to create an IPC service." ) );
    }
    else
    {
      wxLogNull logNull;
    
      stClient *client = new stClient;
      wxString hostName = wxT( "localhost" );
#ifdef WIN32
      wxConnectionBase *connection = client->MakeConnection( hostName, 
                                                             wxT( "wxparaver_service" ),
                                                             wxT( "wxparaver" ) );
#else
      const wxString service_name = wxString::Format( _( "/tmp/wxparaver_service-%s" ), wxGetUserId().c_str());
      wxConnectionBase *connection = client->MakeConnection( hostName, 
                                                             service_name,
                                                             wxT( "wxparaver" ) );
#endif
      if( connection )
      {
        connection->Execute( wxT( "BEGIN" ) );
        for( int i = 1; i < argc; ++i )
          connection->Execute( argv[ i ] );
        connection->Execute( wxT( "END" ) );
        connection->Disconnect();
        delete connection;
      }
      else
      {
        wxMessageBox( wxT( "Sorry, the existing instance may be too busy to respond." ),
                      wxT( "wxparaver" ), wxICON_INFORMATION|wxOK );
      }
    
      delete client;
      return false;
    }
  }
  
  wxCmdLineEntryDesc argumentsParseSyntax[] =
  {
    { wxCMD_LINE_SWITCH, 
      wxT("h"),
      wxT("help"),
      wxT("Show this help."),
      wxCMD_LINE_VAL_STRING,
      wxCMD_LINE_OPTION_HELP },

    { wxCMD_LINE_OPTION, 
      wxT("t"),
      wxT("type"),
      wxT("Event type to code linking."),
      wxCMD_LINE_VAL_NUMBER,
      wxCMD_LINE_PARAM_OPTIONAL },

    { wxCMD_LINE_PARAM, 
      NULL,
      NULL,
      wxT( "(trace.prv | trace.prv.gz) (configuration.cfg)" ),
      wxCMD_LINE_VAL_STRING,
      wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_PARAM_MULTIPLE },

    { wxCMD_LINE_NONE }
  };

#if wxUSE_XPM
  wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
  wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
  wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
  wxImage::AddHandler(new wxGIFHandler);
#endif

  wxCmdLineParser paraverCommandLineParser( argumentsParseSyntax, argc, argv );
  paraverCommandLineParser.Parse();

  wxSize mainWindowSize( ParaverConfig::getInstance()->getMainWindowWidth(),
                         ParaverConfig::getInstance()->getMainWindowHeight() );
  mainWindow = new paraverMain( NULL, SYMBOL_PARAVERMAIN_IDNAME, SYMBOL_PARAVERMAIN_TITLE, SYMBOL_PARAVERMAIN_POSITION, mainWindowSize );
  //mainWindow = new paraverMain( NULL );

  mainWindow->Show(true);

  long int tmpType;
  if( paraverCommandLineParser.Found( wxT("t"), &tmpType ) )
    eventTypeForCode = tmpType;
  
  mainWindow->commandLineLoadings( paraverCommandLineParser );

/*
  paraverMain *mainWindow;
  mainWindow = new paraverMain( NULL );

  mainWindow->Show(true);

  mainWindow->commandLineLoadings( paraverCommandLineParser );
*/
#ifndef WIN32
  presetUserSignals();
#endif

  return true;
}


/*!
 * Cleanup for wxparaverApp
 */

int wxparaverApp::OnExit()
{
//  double w, h;
//  wxparaverApp::mainWindow->GetAuiManager().GetDockSizeConstraint( &w, &h );
//  cout<<w<<" "<<h<<endl;
//  cout << wxparaverApp::mainWindow->GetAuiManager().SavePaneInfo(
//            wxparaverApp::mainWindow->GetAuiManager().GetPane( wxparaverApp::mainWindow->choiceWindowBrowser ) ).mb_str()<<endl;
  ParaverConfig::getInstance()->writeParaverConfigFile();
  
  if( m_checker != NULL )
    delete m_checker;
    
  if( m_server != NULL )
    delete m_server;

  // Every time you write code without goto(s), God kills a kitten. Or was it the other way around?
  goto the_kitten_goto;
  the_kitten_goto:

////@begin wxparaverApp cleanup
	return wxApp::OnExit();
////@end wxparaverApp cleanup
}

#ifdef WIN32
int wxparaverApp::FilterEvent(wxEvent& event)
{
  if ( event.GetEventType()==wxEVT_KEY_DOWN && 
       ((wxKeyEvent&)event).ControlDown() &&
       ((wxKeyEvent&)event).GetKeyCode() == (long) 'C'
     )
  {
    mainWindow->OnKeyCopy();
    return true;
  }
  else if ( event.GetEventType()==wxEVT_KEY_DOWN && 
            ((wxKeyEvent&)event).ControlDown() &&
            ((wxKeyEvent&)event).GetKeyCode() == (long) 'V'
          )
  {
    mainWindow->OnKeyPaste();
    return true;
  }
  
  return -1;
}
#endif

void wxparaverApp::ActivateGlobalTiming( wxDialog* whichDialog )
{
  globalTimingCallDialog = whichDialog;
  wxSetCursor( *wxCROSS_CURSOR );
  globalTiming = true;
  globalTimingBeginIsSet = false;
  globalTimingCallDialog->Enable( false );
  globalTimingCallDialog->MakeModal( false );
#ifdef WIN32
  globalTimingCallDialog->Iconize( true );
#endif
  mainWindow->Raise();
}

void wxparaverApp::DeactivateGlobalTiming()
{
  wxSetCursor( wxNullCursor );
  globalTiming = false;
  globalTimingBeginIsSet = false;
  globalTimingCallDialog->Enable( true );
  globalTimingCallDialog->MakeModal( true );
#ifdef WIN32
  globalTimingCallDialog->Iconize( false );
#endif
  globalTimingCallDialog->Raise();
}


