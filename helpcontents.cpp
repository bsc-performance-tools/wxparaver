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

#include "wx/fs_mem.h"
#include "wx/filesys.h"
#include "wx/wfstream.h"

////@begin includes
////@end includes
#include <string>

#include "helpcontents.h"
#include "paravermain.h"

////@begin XPM images
////@end XPM images
#include "logoBSC.xpm"

using namespace std;

/*!
 * HelpContents type definition
 */

IMPLEMENT_DYNAMIC_CLASS( HelpContents, wxDialog )


/*!
 * HelpContents event table definition
 */

BEGIN_EVENT_TABLE( HelpContents, wxDialog )

////@begin HelpContents event table entries
  EVT_HTML_LINK_CLICKED( ID_HTMLWINDOW, HelpContents::OnHtmlwindowLinkClicked )

  EVT_BUTTON( ID_BUTTON_INDEX, HelpContents::OnButtonIndexClick )

  EVT_BUTTON( ID_BUTTON_CLOSE, HelpContents::OnButtonCloseClick )

////@end HelpContents event table entries

END_EVENT_TABLE()


/*!
 * HelpContents constructors
 */

HelpContents::HelpContents()
{
  Init();
}

HelpContents::HelpContents( wxWindow* parent,
                            wxWindowID id,
                            const wxString& caption,
                            const wxPoint& pos,
                            const wxSize& size,
                            long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}


/*!
 * HelpContents creator
 */

bool HelpContents::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin HelpContents creation
  SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Centre();
////@end HelpContents creation
  return true;
}


/*!
 * HelpContents destructor
 */

HelpContents::~HelpContents()
{
}


/*!
 * Member initialisation
 */
void HelpContents::Init()
{
  currentTutorialDir = _("");
}


const wxString HelpContents::getHtmlIndex( const wxString& path )
{
  wxString htmlIndex;
  wxFileName index( path + wxFileName::GetPathSeparator() + _("index.html") );

  if ( index.FileExists() )
  {
    htmlIndex = index.GetFullPath();
  }

  return htmlIndex;
}


const wxString HelpContents::getTitle( int numTutorial, const wxString& path )
{
  wxString tutorialTitle;

  wxHtmlWindow auxHtml( this );
  auxHtml.LoadPage( path  + wxFileName::GetPathSeparator() + _("index.html") );
  tutorialTitle = auxHtml.GetOpenedPageTitle();
  if ( tutorialTitle.empty() || tutorialTitle == _("index.html") ) // never is empty !?!
  {
    string auxStrTitleFileName( ( path +
                                wxFileName::GetPathSeparator() +
                                _("tutorial_title") ).mb_str() );
    string auxLine;

    ifstream titleFile;
    titleFile.open( auxStrTitleFileName.c_str() );
    if ( titleFile.good() )
    {
      std::getline( titleFile, auxLine );
    
      if ( auxLine.size() > 0 )
      {
        tutorialTitle = wxString::FromAscii( auxLine.c_str() );
      }
      else
      {
        tutorialTitle = _("Tutorial ");
        tutorialTitle << numTutorial;
      }
    }
    else
    {
      tutorialTitle = _("Tutorial ");
      tutorialTitle << numTutorial;
    }

    titleFile.close();
  }

  return tutorialTitle;
}


void HelpContents::appendTutorial( const wxString& title,
                                   const wxString& path,
                                   wxString& htmlDoc )
{
  htmlDoc += _("<LI><P><A HREF=\"") + path + _("\">") + title + _("</A></P></LI>");
}


void HelpContents::htmlMessage( wxString& htmlDoc )
{
  htmlDoc += _("<P><H3>No tutorial found!?</H3></P>");
/*
  htmlDoc += _("<P>Before going on, please have in mind that:</P>");
  htmlDoc += _("<UL>");
  htmlDoc += _("<LI>A single <B>tutorials root directory</B> must contain all ");
  htmlDoc += _("the <B>tutorials directories</B> that you want <B>visible</B> for wxparaver.</LI>");
  htmlDoc += _("<LI>In their own directories, every tutorial has a <B>first page</B> called <TT>index.html</TT>, and also related content (like traces, cfgs, etc.).</LI>");
  htmlDoc += _("<LI>You <B>don't</B> need to <B>write</B> any global tutorials page; we walk through the included tutorials ");
  htmlDoc += _("in the given root directory and build it for you.</LI>");
  htmlDoc += _("<LI>The tutorial title showed in this automatically built main index is read from:</LI>");
  htmlDoc += _("<OL type=\"1\">");
  htmlDoc += _("<LI>The <TT>TITLE</TT> tag in the<TT>index.html</TT> file.</LI>");
  htmlDoc += _("<LI>If this tag is missing or empty, from a single line file named <TT>tutorial_title</TT>");
  htmlDoc += _(", also local to this tutorial.</LI>");
  htmlDoc += _("<LI>If no <TT>tutorial_title</TT> file is found, we give a numbered 'Tutorial #'.</LI>");
  htmlDoc += _("</OL>");
  htmlDoc += _("</UL>");
*/
  htmlDoc += _("<P>Please check that <B>root directory</B> to tutorials is properly defined:</P>");
  htmlDoc += _("<OL type=\"1\">");
  htmlDoc += _("<LI>Open <A HREF=\"init_preferences\"><I>Preferences Window</I></A>.</LI>");
  htmlDoc += _("<LI>Select <I>Global</I> tab.</LI>");
  htmlDoc += _("<LI>In the <I>Default directories</I> box, change the <I>Tutorials root</I> directory");
  htmlDoc += _("<LI>Save your new settings clicking the <I>Ok</I> button in the <I>Preferences Window</I>.</LI>");
  htmlDoc += _("<LI>After that, we automatically refresh the tutorials list.</LI>");
  htmlDoc += _("<LI>If nothing happens, come back here and press the button <I>Index</I> to rebuild the tutorials list.");
  htmlDoc += _("</OL>");
  htmlDoc += _("<P>If the button <I>Index</I> doesn't seem to work (you're still reading this help!), please verify that:</P>");
  htmlDoc += _("<UL>");
  htmlDoc += _("<LI>Every tutorial is <B>uncompressed</B>.</LI>");
  htmlDoc += _("<LI>Every tutorial is inside its own <B>subdirectory</B>.</LI>");
  htmlDoc += _("<LI>These subdirectories (or tutorials) are copied/linked into the root directory that ");
  htmlDoc += _("you selected before (i.e: /home/myuser/mytutorials/tut1/, /home/myuser/mytutorials/tut2/, etc).</LI>");
  htmlDoc += _("<LI>Every tutorial has a main <B>index.html</B> (i.e: /home/myuser/mytutorials/tut1/index.html ).</LI>");
  htmlDoc += _("</UL>");
  htmlDoc += _("<P>If you still get this help after checking these steps again, please contact us at paraver@bsc.es.</P>");
}


void HelpContents::buildIndex()
{
  // get tutorials directory
  string auxStrPath = paraverMain::myParaverMain->GetParaverConfig()->getGlobalTutorialsPath();
  wxString auxPath = wxString::FromAscii( auxStrPath.c_str() );
  wxFileName tutorialsGlobalPath( auxPath );

  // open html index
  wxString tutorialsHtmlIndex, tutorialsList;
  tutorialsHtmlIndex += _("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">");
  tutorialsHtmlIndex += _("<HTML>");
  tutorialsHtmlIndex += _("<HEAD>");
  tutorialsHtmlIndex += _("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />");
  tutorialsHtmlIndex += _("<TITLE>Tutorials</TITLE>");
  tutorialsHtmlIndex += _("</HEAD>");
  tutorialsHtmlIndex += _("<BODY>");

  
  // Place logoBSC
  wxFileSystem::AddHandler( new wxMemoryFSHandler() );
  wxMemoryFSHandler::AddFile( wxT("logoBSC.xpm"),
                              wxBITMAP( logoBSC ),
                              wxBITMAP_TYPE_XPM );
  tutorialsHtmlIndex += _("<P ALIGN=LEFT><IMG SRC=\"memory:logoBSC.xpm\" NAME=\"logoBSC\" ALIGN=BOTTOM BORDER=0></P>" );

  // look for tutorials directories, and for index.html inside them
  if ( wxDirExists( auxPath ) )
  {
    tutorialsList += _("<UL>");
    int numTutorials = 0;

    wxString currentDir = wxFindFirstFile(
            tutorialsGlobalPath.GetLongPath() + wxFileName::GetPathSeparator() + _("*"),
            wxDIR );
    wxArrayString tutorials;
    while( !currentDir.empty() )
    {
      if ( getHtmlIndex( currentDir ) != _("")  )
      {
        // index.html found! => we consider this is a tutorial
        numTutorials++;
        tutorials.Add( currentDir );
      }

      currentDir = wxFindNextFile();
    }
    
    // Order them: every tutorial dir must have a numbered prefix
    //   tutorialsPath/ #_title_bla_bla /index.html
    tutorials.Sort();
    
    for( int i = 0; i < numTutorials; ++i )
    {
      appendTutorial( getTitle( numTutorials, tutorials[ i ] ),
                      getHtmlIndex( tutorials[ i ] ),
                      tutorialsList );
    }

    tutorialsList += _("</UL>");

    if ( numTutorials == 0 )
    {
      htmlMessage( tutorialsHtmlIndex );
    }
    else
    {
      tutorialsHtmlIndex += _("<P><H3><B>Index</B></H3></P>");
      tutorialsHtmlIndex += tutorialsList;
    }
  }
  else
  {
    htmlMessage( tutorialsHtmlIndex );
  }

  // close html index
  tutorialsHtmlIndex += _("</BODY></HTML>");

  htmlWindow->SetPage( tutorialsHtmlIndex );

  wxMemoryFSHandler::RemoveFile( wxT("logoBSC.xpm") );
}


/*!
 * Control creation for HelpContents
 */
void HelpContents::CreateControls()
{    
////@begin HelpContents content construction
  HelpContents* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  htmlWindow = new wxHtmlWindow( itemDialog1, ID_HTMLWINDOW, wxDefaultPosition, wxSize(600, 400), wxHW_SCROLLBAR_AUTO|wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
  itemBoxSizer2->Add(htmlWindow, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer4, 0, wxGROW|wxALL, 5);

  wxButton* itemButton5 = new wxButton( itemDialog1, ID_BUTTON_INDEX, _("Index"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer4->Add(itemButton5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  itemBoxSizer4->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxButton* itemButton7 = new wxButton( itemDialog1, ID_BUTTON_CLOSE, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer4->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end HelpContents content construction

  buildIndex();
}


/*!
 * Should we show tooltips?
 */

bool HelpContents::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap HelpContents::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin HelpContents bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
////@end HelpContents bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon HelpContents::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin HelpContents icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end HelpContents icon retrieval
}


std::string HelpContents::getCurrentTutorialFullPath()
{
  string fullPath = paraverMain::myParaverMain->GetParaverConfig()->getGlobalTutorialsPath();
  fullPath += wxString( wxFileName::GetPathSeparator() ).mb_str();
  fullPath += currentTutorialDir.mb_str();
  fullPath += wxString( wxFileName::GetPathSeparator() ).mb_str();

  return fullPath;
}


std::string HelpContents::getHrefFullPath( wxHtmlLinkEvent &event )
{
  string hrefFullPath = getCurrentTutorialFullPath();
  hrefFullPath += std::string( event.GetLinkInfo().GetHref().mb_str() );

  return hrefFullPath;
}


bool HelpContents::matchHrefExtension( wxHtmlLinkEvent &event, const wxString extension )
{
  //return ( event.GetLinkInfo().GetHref().Right( extension.Len() ).Cmp( extension ) == 0 );
  return ( event.GetLinkInfo().GetHref().Right( extension.Len() ).Cmp( extension ) == 0 );
}


/*!
 * wxEVT_COMMAND_HTML_LINK_CLICKED event handler for ID_HTMLWINDOW
 */

void HelpContents::OnHtmlwindowLinkClicked( wxHtmlLinkEvent& event )
{
  wxString auxCommand;

  if ( event.GetLinkInfo().GetHref().StartsWith( _("init_command:"), &auxCommand ) )
  {
    string app("");
    string trace("");
    string command( auxCommand.mb_str() );
    bool runNow = true;
    //paraverMain::myParaverMain->ShowRunCommand( app, trace, command, runNow );
  }
  else if ( event.GetLinkInfo().GetHref().Cmp( _("init_preferences") ) == 0 )
  {
    paraverMain::myParaverMain->ShowPreferences();

    // we rebuild it anyway
    buildIndex();
  }
  else if ( matchHrefExtension( event, _(".prv") ) ||
            matchHrefExtension( event, _(".prv.gz")))
  {
    paraverMain::myParaverMain->DoLoadTrace( getHrefFullPath( event ) );
  }
  else if ( matchHrefExtension( event, _(".cfg")))
  {
    if ( paraverMain::myParaverMain->GetLoadedTraces().size() > 0 )
    {
      paraverMain::myParaverMain->DoLoadCFG( getHrefFullPath( event )  );
    }
    else
    {
      wxMessageDialog message( this, _("No trace loaded."), _( "Warning" ), wxOK );
      message.ShowModal();
    }
  }
  else if ( matchHrefExtension( event, _(".xml")))
  {
    string traceName = getCurrentTutorialFullPath();
    bool loadTrace = true;
    string strXmlFile = getHrefFullPath( event );

    paraverMain::myParaverMain->ShowCutTraceWindow( traceName, loadTrace, strXmlFile );
  }
  else
  {
    // If current clicked link points to a tutorial index.html file, keep its
    //   tutorial directory name to allow relative references.
    // Idea to detect tutorial:
    //   /home/user/root-tutorials/ => dir depth tutorials = 3
    //   vs.
    //   /home/user/root-tutorials/tutorial1/index.html => dir depth current = 4
    //   /home/user/root-tutorials/tutorial1/html/.../anyotherpage.html => dir depth current > 4
    wxString anyTutorialPath =
            wxString::FromAscii( paraverMain::myParaverMain->GetParaverConfig()->getGlobalTutorialsPath().c_str() );
    if ( anyTutorialPath[ anyTutorialPath.Len() - 1 ] != wxString( wxFileName::GetPathSeparator() ))
    {
      // last separator needed to count properly
      anyTutorialPath += wxString( wxFileName::GetPathSeparator() );
    }

    wxFileName anyTutorialDir( anyTutorialPath );

    size_t dirsDepthTutorials = anyTutorialDir.GetDirCount();

    wxFileName currentLink( event.GetLinkInfo().GetHref() );
    size_t dirsDepthCurrentLink = currentLink.GetDirCount();

    if (( dirsDepthCurrentLink == dirsDepthTutorials + 1 ) &&
        ( currentLink.GetFullName().Cmp( _("index.html") ) == 0 ))
    {
      wxArrayString dirs = currentLink.GetDirs();
      currentTutorialDir = dirs[ dirsDepthCurrentLink - 1 ];
    }

    // and let the html window browse it.
    event.Skip();
  }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CLOSE
 */

void HelpContents::OnButtonCloseClick( wxCommandEvent& event )
{
  Close();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_INDEX
 */

void HelpContents::OnButtonIndexClick( wxCommandEvent& event )
{
  buildIndex();
}

