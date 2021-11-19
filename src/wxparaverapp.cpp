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

#include <wx/filefn.h>
#include <wx/filename.h>

#ifdef __WXMAC__
#include <wx/sysopt.h>
#endif

#include "labelconstructor.h"

#include "config.h"
#include "wxparaverapp.h"
//#include "connection.h"
#include "sessionsaver.h"
#include "helpcontents.h"
#include "gtimeline.h"
#include "ghistogram.h"
#include "tutorialsdownload.h"

using namespace std;

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

wxCmdLineEntryDesc wxparaverApp::argumentsParseSyntax[] =
{
  { wxCMD_LINE_SWITCH, 
    "v",
    "version",
    "Show wxparaver version." },

  { wxCMD_LINE_SWITCH, 
    "h",
    "help",
    "Show this help.",
    wxCMD_LINE_VAL_NONE,
    wxCMD_LINE_OPTION_HELP },

  { wxCMD_LINE_SWITCH, 
    "i",
    "image",
    "Save cfg last window as an image; if it's a timeline, also saves its legend",
    wxCMD_LINE_VAL_NONE,
    wxCMD_LINE_PARAM_OPTIONAL },

  { wxCMD_LINE_OPTION, 
    "e",
    "event",
    "Event type to code linking.",
    wxCMD_LINE_VAL_NUMBER,
    wxCMD_LINE_PARAM_OPTIONAL },

  { wxCMD_LINE_OPTION, 
    "t",
    "tutorial",
    "Load tutorial. <str> can be the path to the tutorial "
      "containing the index.html file, or the whole url, like "
      "path/file.html (then, other names than 'index' are allowed).",
    wxCMD_LINE_VAL_STRING,
    wxCMD_LINE_PARAM_OPTIONAL },

  { wxCMD_LINE_SWITCH, 
    "gtf",
    "generate_tutorials_file",
    "",
    wxCMD_LINE_VAL_NONE,
    wxCMD_LINE_PARAM_OPTIONAL },

   { wxCMD_LINE_PARAM, 
    nullptr,
    nullptr,
    "(trace.prv | trace.prv.gz) (configuration.cfg) | saved_session.session",
    wxCMD_LINE_VAL_STRING,
    wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_PARAM_MULTIPLE },

  { wxCMD_LINE_NONE }
};

/*!
 * Constructor for wxparaverApp
 */

wxparaverApp::wxparaverApp()
{
  wxSetEnv( wxT( "UBUNTU_MENUPROXY" ), wxT( "0" ) );
  Init();
}


/*!
 * Member initialisation
 */

void wxparaverApp::Init()
{
////@begin wxparaverApp member initialisation
	eventTypeForCode = 60000119;
	globalTiming = false;
	globalTimingBegin = 0;
	globalTimingBeginIsSet = false;
	globalTimingCallDialog = nullptr;
	globalTimingEnd = 0;
////@end wxparaverApp member initialisation
  m_locale.Init();

#ifdef __WXMAC__
  wxSystemOptions::SetOption( "mac.toolbar.no-native", 1 );
#endif
}

/*!
 * Initialisation for wxparaverApp
 */

paraverMain* wxparaverApp::mainWindow = nullptr;


#if !defined _MSC_VER && !defined __MINGW32__
volatile bool sig1 = false;
volatile bool sig2 = false;

void wxparaverApp::handler( int signalNumber )
{
  sigdelset( &act.sa_mask, SIGUSR1 );
  sigdelset( &act.sa_mask, SIGUSR2 );

  /*if ( signalNumber == SIGUSR1 )
    sig1 = true;
  else
    sig1 = false;
  */
  sig1 = ( signalNumber == SIGUSR1 );
  sig2 = !sig1;

  mainWindow->insertSignalItem( sig1 );
  sig1 = false;
  sig2 = false;
  mainWindow->Raise();

  sigaddset( &act.sa_mask, SIGUSR1 );
  sigaddset( &act.sa_mask, SIGUSR2 );
  sigaction( SIGUSR1, &act, nullptr );
  sigaction( SIGUSR2, &act, nullptr );
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

  if ( sigaction( SIGUSR1, &act, nullptr ) != 0 )
  {
    /* Handle error */
  }

  if ( sigaction( SIGUSR2, &act, nullptr ) != 0 )
  {
    /* Handle error */
  }
}
#endif

#ifdef DEBUG_CONSOLE_ENABLED
static const WORD MAX_CONSOLE_LINES = 500;
void RedirectIOToConsole()
{
  int hConHandle;
  long long int lStdHandle;
  CONSOLE_SCREEN_BUFFER_INFO coninfo;
  FILE *fp;

  // allocate a console for this app
  AllocConsole();

  // set the screen buffer to be big enough to let us scroll text
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);

  coninfo.dwSize.Y = MAX_CONSOLE_LINES;

  SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

  // redirect unbuffered STDOUT to the console
  lStdHandle = (long long int)GetStdHandle(STD_OUTPUT_HANDLE);
  hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
  fp = _fdopen( hConHandle, "w" );
  *stdout = *fp;
  setvbuf( stdout, NULL, _IONBF, 0 );

  // redirect unbuffered STDIN to the console
  lStdHandle = (long long int)GetStdHandle(STD_INPUT_HANDLE);
  hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
  fp = _fdopen( hConHandle, "r" );
  *stdin = *fp;
  setvbuf( stdin, NULL, _IONBF, 0 );

  // redirect unbuffered STDERR to the console
  lStdHandle = (long long int)GetStdHandle(STD_ERROR_HANDLE);
  hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
  fp = _fdopen( hConHandle, "w" );
  *stderr = *fp;
  setvbuf( stderr, NULL, _IONBF, 0 );

  // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
  // point to console as well
  std::ios::sync_with_stdio();
}
#endif

bool wxparaverApp::OnInit()
{
#ifdef DEBUG_CONSOLE_ENABLED
  RedirectIOToConsole();
#endif

#ifdef TRACING_ENABLED
  Extrae_init();
#endif

  wxCmdLineParser paraverCommandLineParser( argumentsParseSyntax, argc, argv );
  if ( paraverCommandLineParser.Parse( false ) != 0 )
  {
    std::cout << std::endl;
    paraverCommandLineParser.Usage();
    
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "  Environment variables defined by wxParaver:" << std::endl;
    std::cout << "  -------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "    $PARAVER_LOADED                Once started, it's filled with wxParaver PID" << std::endl;
    std::cout << "    $PARAVER_ALIEN_TRACE_FULL_NAME Contains the absolute path to the current loaded trace." << std::endl;
    std::cout << "                                   Published only:" << std::endl;
    std::cout << "                                     * when middle click is done in a timeline and" << std::endl;
    std::cout << "                                     * if PARAVER_ALIEN* vars are defined (see below)." << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "  Environment variables defined by user (optional):" << std::endl;
    std::cout << "  -------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "    To set alien application invoked from timeline with mouse middle button (both vars needed):"  << std::endl;
    std::cout << "      $PARAVER_ALIEN_APP  Application that will be invoked." << std::endl;
    std::cout << "      $PARAVER_ALIEN_TYPE Event type passed as parameter" << std::endl;
    std::cout << "                          Starting from the click, it's sought backwards" << std::endl;
    std::cout << "                          The value passed is 1) its symbolic translation or 2) its value" << std::endl;
    std::cout << std::endl;
    std::cout << "        Example: Create empty file named with MPI callers at level 1" << std::endl;
    std::cout << "          > PARAVER_ALIEN_APP=touch PARAVER_ALIEN_TYPE=70000001 wxparaver &" << std::endl;
    std::cout << std::endl;
    std::cout << "        Example: Writes in a file the translation of the decimal PAPI_TOT_INST counter value into morse" << std::endl;
    std::cout << "          > PARAVER_ALIEN_APP='> in_morse.txt morse -s' PARAVER_ALIEN_TYPE=42000050 wxparaver &" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    
    /*
    std::cout << "        Example: Search line of a user function label in .pcf file and write it to a file" << std::endl;
    std::cout << "          > export PARAVER_ALIEN_APP='> label.txt xargs -I {} grep -n {} trace.pcf <<<'" << std::endl;
    std::cout << "          > export PARAVER_ALIEN_TYPE=70000001" << std::endl;
    std::cout << "          > wxparaver trace.prv &" << std::endl;
    std::cout << std::endl;
    */
    return true; 
  }

  if( paraverCommandLineParser.Found( wxT("v") ))
  {
    PrintVersion();
    return true;
  }

  try
  {
    ParaverConfig::getInstance()->readParaverConfigFile();
  }
  catch( ... )
  {
    wxMessageBox( wxT( "Error while loading preferences. Using default options." ),
                  wxT( "Preferences error" ),
                  wxOK|wxICON_ERROR );
  }

  const wxString name = wxT( "wxparaver-" ) + wxGetUserId();
  wxLogNull *tmpLogNull = new wxLogNull();
  m_checker = new wxSingleInstanceChecker(name);
  delete tmpLogNull;

  invalidateNoConnect = false;
  if( !ParaverConfig::getInstance()->getGlobalSingleInstance() )
  {
    m_server = new stServer;
  
#ifdef WIN32
    // ToDo: port the unix code to Win32
    const wxString serviceFullName = wxT( "wxparaver_service-" ) + 
        wxGetUserId() + 
        wxT( "-" ) +
        wxString::Format( wxT( "%i" ), getpid() );
#else
    const wxString serviceFullName = wxT( "/tmp/wxparaver_service-" ) + 
        wxGetUserId() + 
        wxT( "-" ) +
        wxString::Format( wxT( "%i" ), getpid() );
#endif

    if ( m_server->Create( serviceFullName ) )
    {
      wxLogNull logNull;
      stClient *client = new stClient;
      wxString hostName = wxT( "localhost" );

  #ifdef WIN32
      const wxString serviceName = wxT( "wxparaver_service-" ) + wxGetUserId();

      // Connectivity check for failing sessions
      // ST : Service Table
      
      // Are services stored here in Windows?
      wxDir wxd( wxT( "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\" ) ); 
      wxString service, serviceFlag = wxT( "wxparaver_service*" );
      std::map< wxString, wxString > serviceMap;
      if ( wxd.IsOpened() )
      {
        bool cont = wxd.GetFirst( &service, serviceFlag );

        while ( cont )
        {
          wxString servicePID = service.AfterLast( '-' );
          wxString serviceFileName = wxT( 
            "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\" ) +
            service;
          serviceMap.insert( std::pair< wxString, wxString >( servicePID, serviceFileName ) );
          cont = wxd.GetNext( &service );
        }
      }
      // AS : Autosaved Sessions
      wxArrayString filesAS;
      wxString folderPath( ( ParaverConfig::getInstance()->getGlobalSessionPath() + 
                             string( "\\AutosavedSessions" ) ).c_str(), wxConvUTF8 );

  #else

      const wxString serviceName = wxT( "/tmp/wxparaver_service-" ) + wxGetUserId(); 

      // Connectivity check for failing sessions
      // ST : Service Table
      wxDir wxd( _( "/tmp/" ) );
      wxString service, serviceFlag = wxT( "wxparaver_service*" );
      std::map< wxString, wxString > serviceMap;
      bool cont = wxd.GetFirst( &service, serviceFlag );

      while ( cont )
      {
        wxString servicePID = service.AfterLast( '-' );
        wxString serviceFileName = wxT( "/tmp/" ) + service;
        serviceMap.insert( std::pair< wxString, wxString >( servicePID, serviceFileName ) );
        cont = wxd.GetNext( &service );
      }

      // AS : Autosaved Sessions
      wxArrayString filesAS;
      wxString folderPath( ( ParaverConfig::getInstance()->getGlobalSessionPath() + 
                             string( "/AutosavedSessions" ) ).c_str(), wxConvUTF8 );

  #endif

      wxString myServicePID = serviceFullName.AfterLast( '-' );
      if ( wxDirExists( folderPath ) ) 
      {
        wxDir::GetAllFiles( folderPath, &filesAS, wxT( "*.session" ), wxDIR_FILES );
      }

      for ( unsigned int i = 0; i < filesAS.size(); ++i )
      {
        wxString autoSessionPID = filesAS[ i ].BeforeFirst( '_' ).AfterLast( 's' );
        autoSessionPID.Replace( wxT( "ps" ), wxT( "" ) );

        if ( serviceMap.find( autoSessionPID ) != serviceMap.end() )
        {
          wxString currentService = serviceMap[ autoSessionPID ];
          if ( autoSessionPID != myServicePID && currentService != serviceFullName )
          {
            wxConnectionBase *connection = client->MakeConnection( 
                hostName, 
                currentService, 
                wxT( "wxparaver" ) );
            if ( connection == nullptr )
            {
              wxRemoveFile( currentService );
              invalidateNoConnect = true;
            }
            delete connection;
          }
          serviceMap.erase( autoSessionPID );
        }
      }

      //Premature check (or clean if no connect) for missing maps 
      std::map< wxString, wxString >::iterator it = serviceMap.begin();
      for ( ; it != serviceMap.end() ; ++it )
      {
        wxString currentService = ( *it ).second;
        if ( currentService != serviceFullName )
        {
          wxConnectionBase *connection = client->MakeConnection( 
            hostName, 
            currentService, 
            wxT( "wxparaver" ) );
          if ( connection == nullptr )
            wxRemoveFile( currentService );
          delete connection;
          
        }
      }
      delete client;
    }
    else
      wxLogDebug( wxT( "Failed to create an IPC service." ) );
  }
  else //if( ParaverConfig::getInstance()->getGlobalSingleInstance() )
  {
    if ( !m_checker->IsAnotherRunning() )
    {
      m_server = new stServer;
    
    #ifdef WIN32
      const wxString serviceName = wxT( "wxparaver_service-" ) + wxGetUserId();
    #else
      const wxString serviceName = wxT( "/tmp/wxparaver_service-" ) + wxGetUserId();
    #endif
      if( !m_server->Create( serviceName ) )
        wxLogDebug( wxT( "Failed to create an IPC service." ) );
    }
    else
    {
      wxLogNull logNull;
    
      stClient *client = new stClient;
      wxString hostName = wxT( "localhost" );
    #ifdef WIN32
      const wxString serviceName = wxT( "wxparaver_service-" ) + wxGetUserId();
    #else
      const wxString serviceName = wxT( "/tmp/wxparaver_service-" ) + wxGetUserId();
    #endif
      wxConnectionBase *connection = client->MakeConnection( hostName, 
                                                             serviceName,
                                                             wxT( "wxparaver" ) );
      if( connection )
      {
        connection->Execute( "BEGIN" );
        connection->Execute( argv[ 0 ] );
        for( int i = 1; i < argc; ++i )
        {
          if ( argv[ i ][ 0 ] == '-' )
          {
            connection->Execute( argv[ i ] );
          }
          else
          {
            if ( wxFileName::FileExists( argv[ i ] ) ) 
            {
              wxFileName tmpFile( argv[ i ] );
              tmpFile.Normalize();
              connection->Execute( tmpFile.GetFullPath().c_str() );
            }
            else
            {
              connection->Execute( argv[ i ] );
            }
          }          
        }
        connection->Execute( "END" );
        connection->Disconnect();
        delete connection;
        delete client;
        return true;
      }
      else
      {
        wxMessageBox( wxT( "Sorry, the existing instance may be too busy to respond." ),
                      wxT( "wxparaver" ), wxICON_INFORMATION|wxOK );
        delete client;
        return false;
      }
    }
  }

#if wxUSE_XPM
  wxImage::AddHandler( new wxXPMHandler );
#endif
#if wxUSE_LIBPNG
  wxImage::AddHandler( new wxPNGHandler );
#endif
#if wxUSE_LIBJPEG
  wxImage::AddHandler( new wxJPEGHandler );
#endif
#if wxUSE_GIF
  wxImage::AddHandler( new wxGIFHandler );
#endif

  wxSize mainWindowSize( ParaverConfig::getInstance()->getMainWindowWidth(),
                         ParaverConfig::getInstance()->getMainWindowHeight() );
                         
  mainWindow = new paraverMain( nullptr, 
                                SYMBOL_PARAVERMAIN_IDNAME, 
                                SYMBOL_PARAVERMAIN_TITLE, 
                                SYMBOL_PARAVERMAIN_POSITION, 
                                mainWindowSize );
  mainWindow->Show(true);

  bool prevSessionWasComplete = ParaverConfig::getInstance()->initCompleteSessionFile();

  if ( ParaverConfig::getInstance()->getGlobalPrevSessionLoad() && 
       ParaverConfig::getInstance()->getGlobalSessionSaveTime() > 0 && 
       invalidateNoConnect )
  {
    mainWindow->checkIfPrevSessionLoad( prevSessionWasComplete );
  }  
  else if ( ParaverConfig::getInstance()->getGlobalPrevSessionLoad() && 
            ParaverConfig::getInstance()->getGlobalSessionSaveTime() > 0 && 
            ParaverConfig::getInstance()->getGlobalSingleInstance() && 
            !prevSessionWasComplete   )
  {
    mainWindow->checkIfPrevSessionLoad( prevSessionWasComplete );
  }  


#if !defined _MSC_VER && !defined __MINGW32__
  presetUserSignals();
#endif

  ParseCommandLine( paraverCommandLineParser );

  return true;
}


int wxparaverApp::OnRun()
{
  if( mainWindow != nullptr )
    return wxApp::OnRun();
  return 0;
}


void wxparaverApp::ParseCommandLine( wxCmdLineParser& paraverCommandLineParser )  
{
  if ( paraverCommandLineParser.Found( wxT( "generate_tutorials_file" ) ) )
  {
    std::string tmpTutorialFile( "./" + TutorialsDownload::tutorialsListFile );
    TutorialsDownload::getInstance()->writeList( tmpTutorialFile );
    mainWindow->Close();
    return;
  }

  if ( paraverCommandLineParser.Found( wxT( "i" ) ) )
  {
    string fileName;
    Trace *currentTrace = nullptr;
    paraverMain::disableUserMessages = true;
    paraverMain::stopOnIdle = true;

    for ( unsigned int i = 0; i < paraverCommandLineParser.GetParamCount(); ++i )
    {
      fileName = paraverCommandLineParser.GetParam( i ).mb_str();

      if ( mainWindow->GetLocalKernel()->isTraceFile( fileName ) )
      {
        if( currentTrace != nullptr )
          delete currentTrace;

        wxFileName tmpFileName( wxString( fileName.c_str(), wxConvUTF8 ) );
          
#ifdef WIN32
        tmpFileName.Normalize( wxPATH_NORM_DOTS | wxPATH_NORM_ABSOLUTE |
                               wxPATH_NORM_LONG );
#else
        tmpFileName.Normalize( wxPATH_NORM_DOTS | wxPATH_NORM_ABSOLUTE |
                               wxPATH_NORM_LONG | wxPATH_NORM_TILDE );
#endif

        string tmpPath = std::string( tmpFileName.GetFullPath().mb_str() );
      
        currentTrace = Trace::create( mainWindow->GetLocalKernel(), tmpPath, false, nullptr );
      }
      else if ( CFGLoader::isCFGFile( fileName ) && currentTrace != nullptr )
      {
        vector<Timeline *> newWindows;
        vector<Histogram *> newHistograms;
        SaveOptions options;


        if( CFGLoader::loadCFG( mainWindow->GetLocalKernel(), fileName, currentTrace,
                                newWindows, newHistograms, options ) )
        {
          if( !newHistograms.empty() )
          {
            // save histogram image
            Histogram *histo = newHistograms.back();
            histo->setRedraw( false );
            string composedName = histo->getName() + " @ " +
                                  histo->getControlWindow()->getTrace()->getTraceName();

            gHistogram* tmpGHisto = new gHistogram( mainWindow, 
                wxID_ANY, 
                wxString::FromUTF8( composedName.c_str() ) );
            tmpGHisto->SetHistogram( histo );

            histo->setZoom( true );
            histo->setRecalc( false );
            tmpGHisto->execute();

            tmpGHisto->saveImage( wxT( "" ) );
            
            delete tmpGHisto;
            newHistograms.pop_back();
          }
          else
          {
            // save timeline image
            Timeline *window = newWindows.back();
            window->setRedraw( false );
            string composedName = window->getName() + " @ " +
                                  window->getTrace()->getTraceName();

            wxPoint tmpPos( window->getPosX(), window->getPosY() );
            gTimeline* tmpTimeline = new gTimeline( mainWindow, 
                    wxID_ANY, 
                    wxString::FromUTF8( composedName.c_str() ), 
                    tmpPos );
            tmpTimeline->SetMyWindow( window );
            tmpTimeline->SetClientSize( wxSize( window->getWidth(), window->getHeight() ) );
            
            tmpTimeline->redraw();
            tmpTimeline->saveImage( wxT( "" ) );
            tmpTimeline->saveImageLegend();
            
            delete tmpTimeline;
            newWindows.pop_back();
          }

          for( vector<Histogram *>::iterator it = newHistograms.begin();
               it != newHistograms.end(); ++it )
            delete (*it);
          newHistograms.clear();

          for( vector<Timeline *>::iterator it = newWindows.begin();
               it != newWindows.end(); ++it )
            delete (*it);
          newWindows.clear();
        }
      }

      fileName.erase();
    }
    mainWindow->Close();
    return;
  }

  long int tmpType;
  if( paraverCommandLineParser.Found( wxT( "e" ), &tmpType ) )
    eventTypeForCode = tmpType;

  wxString tmpTutorial;    
  if ( paraverCommandLineParser.Found( wxT( "t" ), &tmpTutorial ) )
  {
    if ( mainWindow->GetTutorialsWindow() == nullptr )
    {
      mainWindow->SetTutorialsWindow( 
              HelpContents::createObject( 
                      TContents::TUTORIAL,
                      mainWindow, 
                      wxString( paraverMain::myParaverMain->GetParaverConfig()->getGlobalTutorialsPath().c_str(), wxConvUTF8 ),
                      true, wxID_ANY, _("Tutorials") 
              )
      );
    }

    if ( !mainWindow->GetTutorialsWindow()->SetHelpContents( tmpTutorial ) )
      wxMessageBox( wxT( "Unable to find index.html in tutorial dir.\n\n"
                         "Please check whole path or create/place index.html file." ),
                    wxT( "Tutorial" ), wxICON_INFORMATION|wxOK );
    else
      mainWindow->GetTutorialsWindow()->Show( true );
  }
  mainWindow->commandLineLoadings( paraverCommandLineParser );
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
//            wxparaverApp::mainWindow->GetAuiManager().GetPanel( wxparaverApp::mainWindow->choiceWindowBrowser ) ).mb_str()<<endl;

  
  if ( mainWindow != nullptr && paraverMain::IsSessionValid() && !invalidateNoConnect ) 
  {
    ParaverConfig::getInstance()->closeCompleteSessionFile();
  }
  
//// Code that deletes PID from the sessions map starts here ////

  if( !ParaverConfig::getInstance()->getGlobalSingleInstance() )
  {
    const wxString name = wxT( "wxparaver-" ) + wxGetUserId();
    wxLogNull *tmpLogNull = new wxLogNull();
    m_checker = new wxSingleInstanceChecker(name);
    delete tmpLogNull;
  }
  
//// Code that deletes PID from the sessions map ends here  ////

  if( m_checker != nullptr )
    delete m_checker;
    
  if( m_server != nullptr )
    delete m_server;

#ifdef TRACING_ENABLED
  Extrae_fini();
#endif

////@begin wxparaverApp cleanup
	return wxApp::OnExit();
////@end wxparaverApp cleanup
}

int wxparaverApp::FilterEvent(wxEvent& event)
{
  int ret = true;

  if ( event.GetEventType() == wxEVT_KEY_DOWN )
  {
    long keyCode = ( (wxKeyEvent&) event ).GetKeyCode();
    if ( keyCode == WXK_ESCAPE && GetGlobalTiming() )
    {
      DeactivateGlobalTiming();
    }
    else if ( ( (wxKeyEvent&) event ).ControlDown() )
    {
      if ( keyCode == (long) 'S' )
      {
        wxFileDialog dialog( mainWindow, wxT( "Save session" ),
              _(""), _(""), _("*"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT );
        if ( dialog.ShowModal() == wxID_OK )
        {
          SessionSaver::SaveSession( dialog.GetPath(), wxparaverApp::mainWindow->GetLoadedTraces() );
        }
      }
      else if ( keyCode == (long) 'L' )
      {
        wxFileDialog dialog( mainWindow, wxT( "Load session" ), _(""), _(""), _("*"), wxFD_OPEN|wxFD_FILE_MUST_EXIST );
        if ( dialog.ShowModal() == wxID_OK )
        {
          SessionSaver::LoadSession( dialog.GetPath() );
        }
      }
      else if ( keyCode == (long) 'F' )
        mainWindow->OnFindDialog();
      else if ( keyCode == (long) 'Q' )
        mainWindow->exitManager();
      else if ( keyCode == (long) 'O' )
      {
        wxCommandEvent dummyEvent;
        mainWindow->OnOpenClick( dummyEvent );
      }

#ifdef WIN32
      else if ( keyCode == (long) 'C' )
        mainWindow->OnKeyCopy();
      else if ( keyCode == (long) 'V' )
        mainWindow->OnKeyPaste();
#endif
      else if( keyCode == WXK_DELETE )
      {
        wxCommandEvent dummyEvent;
        mainWindow->OnTooldeleteClick( dummyEvent );
      }
      else
        ret = -1;
    }
    else
      ret = -1;
  }
  else
    ret = -1;

  return ret;
}


void wxparaverApp::ActivateGlobalTiming( wxDialog* whichDialog )
{
  globalTimingCallDialog = whichDialog;
  wxSetCursor( *wxCROSS_CURSOR );
  globalTiming = true;
  globalTimingBeginIsSet = false;
#ifndef __WXMAC__
  globalTimingCallDialog->Enable( false );
#endif
#ifdef WIN32
  globalTimingCallDialog->Iconize( true );
#endif
#ifdef __WXMAC__
  globalTimingCallDialog->Lower();
#endif
  mainWindow->Raise();
}

void wxparaverApp::DeactivateGlobalTiming()
{
  wxSetCursor( wxNullCursor );
  globalTiming = false;
  globalTimingBeginIsSet = false;
  globalTimingCallDialog->Enable( true );


#ifdef WIN32
  globalTimingCallDialog->Iconize( false );
#endif
  globalTimingCallDialog->Raise();
}


void wxparaverApp::PrintVersion()
{
  cout << PACKAGE_STRING;

  bool reverseOrder = true;
  string auxDate = LabelConstructor::getDate( reverseOrder );

  if ( auxDate.compare("") != 0 )
    cout << " Build ";

  cout << auxDate << endl;
}



void wxparaverApp::ValidateSession( bool setValidate )
{
  paraverMain::ValidateSession( setValidate );
}

