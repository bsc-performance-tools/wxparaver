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
#include "wx/imaglist.h"
////@end includes

#include "wx/tipdlg.h"
#include "wx/aboutdlg.h"

#include "paravermain.h"
#include "paraverkernelexception.h"
#include "cfg.h"
#include "window.h"
#include "histogram.h"
#include "gtimeline.h"
#include "ghistogram.h"
//#include <wx/propgrid/advprops.h>
#include "pg_extraprop.h"
#include "progresscontroller.h"
#include "loadedwindows.h"
#include "filter.h"
#include "pg_util.h"
#include "saveconfigurationdialog.h"
#include "windows_tree.h"
#include "derivedtimelinedialog.h"
#include "histogramdialog.h"
#include "preferencesdialog.h"
#include "cutfilterdialog.h"
#include "labelconstructor.h"


#include <signal.h>
#include <iostream>

////@begin XPM images
#include "new_window.xpm"
#include "new_derived_window.xpm"
#include "new_histogram.xpm"
#include "delete.xpm"
#include "cut_trace.xpm"
#include "file_browser.xpm"
#include "window_properties.xpm"
////@end XPM images

#include "table.xpm"
#include "timeline.xpm"

#include "derived_add.xpm"
#include "derived_product.xpm"
#include "derived_substract.xpm"
#include "derived_divide.xpm"
#include "derived_maximum.xpm"
#include "derived_minimum.xpm"
#include "derived_different.xpm"
#include "derived_controlled_clear_by.xpm"
#include "derived_controlled_maximum.xpm"
#include "derived_controlled_add.xpm"

#include <algorithm>

/*!
 * paraverMain type definition
 */

IMPLEMENT_CLASS( paraverMain, wxFrame )


/*!
 * paraverMain event table definition
 */

BEGIN_EVENT_TABLE( paraverMain, wxFrame )

////@begin paraverMain event table entries
  EVT_CLOSE( paraverMain::OnCloseWindow )
  EVT_ICONIZE( paraverMain::OnIconize )
  EVT_SIZE( paraverMain::OnSize )
  EVT_IDLE( paraverMain::OnIdle )

  EVT_MENU( wxID_OPEN, paraverMain::OnOpenClick )

  EVT_UPDATE_UI( ID_RECENTTRACES, paraverMain::OnRecenttracesUpdate )

  EVT_MENU( ID_UNLOADTRACE, paraverMain::OnUnloadtraceClick )
  EVT_UPDATE_UI( ID_UNLOADTRACE, paraverMain::OnUnloadtraceUpdate )

  EVT_MENU( ID_MENULOADCFG, paraverMain::OnMenuloadcfgClick )
  EVT_UPDATE_UI( ID_MENULOADCFG, paraverMain::OnMenuloadcfgUpdate )

  EVT_UPDATE_UI( ID_RECENTCFGS, paraverMain::OnMenuloadcfgUpdate )

  EVT_MENU( ID_MENUSAVECFG, paraverMain::OnMenusavecfgClick )
  EVT_UPDATE_UI( ID_MENUSAVECFG, paraverMain::OnMenusavecfgUpdate )

  EVT_MENU( ID_PREFERENCES, paraverMain::OnPreferencesClick )
  EVT_UPDATE_UI( ID_PREFERENCES, paraverMain::OnPreferencesUpdate )

  EVT_MENU( wxID_EXIT, paraverMain::OnExitClick )

  EVT_MENU( wxID_ABOUT, paraverMain::OnAboutClick )

  EVT_MENU( ID_NEW_WINDOW, paraverMain::OnToolNewWindowClick )
  EVT_UPDATE_UI( ID_NEW_WINDOW, paraverMain::OnToolNewWindowUpdate )

  EVT_MENU( ID_NEW_DERIVED_WINDOW, paraverMain::OnNewDerivedWindowClick )
  EVT_UPDATE_UI( ID_NEW_DERIVED_WINDOW, paraverMain::OnNewDerivedWindowUpdate )

  EVT_MENU( ID_NEW_HISTOGRAM, paraverMain::OnNewHistogramClick )
  EVT_UPDATE_UI( ID_NEW_HISTOGRAM, paraverMain::OnNewHistogramUpdate )

  EVT_MENU( ID_TOOLDELETE, paraverMain::OnTooldeleteClick )
  EVT_UPDATE_UI( ID_TOOLDELETE, paraverMain::OnTooldeleteUpdate )

  EVT_MENU( ID_TOOL_CUT_TRACE, paraverMain::OnToolCutTraceClick )
  EVT_UPDATE_UI( ID_TOOL_CUT_TRACE, paraverMain::OnToolCutTraceUpdate )

  EVT_CHOICEBOOK_PAGE_CHANGED( ID_CHOICEWINBROWSER, paraverMain::OnChoicewinbrowserPageChanged )
  EVT_UPDATE_UI( ID_CHOICEWINBROWSER, paraverMain::OnChoicewinbrowserUpdate )

  EVT_UPDATE_UI( ID_FOREIGN, paraverMain::OnForeignUpdate )

////@end paraverMain event table entries

  EVT_TREE_SEL_CHANGED( wxID_ANY, paraverMain::OnTreeSelChanged )
  EVT_TREE_ITEM_ACTIVATED( wxID_ANY, paraverMain::OnTreeItemActivated )
  EVT_TREE_ITEM_RIGHT_CLICK(wxID_ANY, paraverMain::OnTreeRightClick)
  
  EVT_TREE_BEGIN_DRAG(wxID_ANY, paraverMain::OnTreeBeginDrag)
  EVT_TREE_END_DRAG( wxID_ANY, paraverMain::OnTreeEndDrag)

  EVT_PG_CHANGED( ID_FOREIGN, paraverMain::OnPropertyGridChange )
  
  EVT_ACTIVATE(paraverMain::OnActivate) 
END_EVENT_TABLE()


paraverMain* paraverMain::myParaverMain = NULL;

wxProgressDialog *paraverMain::dialogProgress = NULL;

wxSize paraverMain::defaultTitleBarSize = wxSize(0,0);

Window *paraverMain::beginDragWindow = NULL;
Window *paraverMain::endDragWindow = NULL;

extern volatile bool sig1;
extern volatile bool sig2;
extern struct sigaction act;

static bool userMessage( string message )
{
  wxMessageDialog tmpDialog( NULL, wxString::FromAscii( message.c_str() ), _( "Paraver question" ), wxYES_NO | wxICON_QUESTION );
  paraverMain::myParaverMain->SetRaiseCurrentWindow( false );
  int tmpResult = tmpDialog.ShowModal();
  paraverMain::myParaverMain->SetRaiseCurrentWindow( true );
  return tmpResult == wxID_YES;
}

wxImageList *paraverMain::getImageList()
{
  wxImageList *images;

  images = new wxImageList( 16, 16 );
  images->Add( wxIcon( table_xpm ) );
  images->Add( wxIcon( timeline_xpm ) );

  // Derived icons added in same order than kernel returns
  images->Add( wxIcon( derived_add_xpm ) );
  images->Add( wxIcon( derived_product_xpm ) );
  images->Add( wxIcon( derived_substract_xpm ) );
  images->Add( wxIcon( derived_divide_xpm ) );
  images->Add( wxIcon( derived_maximum_xpm ) );
  images->Add( wxIcon( derived_minimum_xpm ) );
  images->Add( wxIcon( derived_different_xpm ) );
  images->Add( wxIcon( derived_controlled_clear_by_xpm ) );
  images->Add( wxIcon( derived_controlled_maximum_xpm ) );
  images->Add( wxIcon( derived_controlled_add_xpm ) );

  return images;
}


/*!
 * paraverMain constructors
 */

paraverMain::paraverMain()
{
  imageList = getImageList();

  LocalKernel::init();
  localKernel = new LocalKernel( userMessage );
  paraverConfig = ParaverConfig::getInstance();
  myParaverMain = this;

  Init();
  ShowToolTips();
}

paraverMain::paraverMain( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  imageList = getImageList();

  LocalKernel::init();
  localKernel = new LocalKernel( userMessage );
  paraverConfig = ParaverConfig::getInstance();
  paraverConfig->readParaverConfigFile();
  myParaverMain = this;
  
  Init();
  Create( parent, id, caption, pos, size, style );
  
  defaultTitleBarSize = GetSize() - GetClientSize();
  ShowToolTips();
}


void paraverMain::commandLineLoadings( wxCmdLineParser &parser )
{
  string fileName;
  for ( unsigned int i = 0; i < parser.GetParamCount(); ++i )
  {
    fileName = parser.GetParam( i ).mb_str();

    if (( fileName.substr( fileName.length() - 6 ) == "prv.gz" ) || 
        ( fileName.substr( fileName.length() - 3 ) == "prv" ))
    {
      DoLoadTrace( fileName );
    }
    else if (( fileName.substr( fileName.length() - 3 ) == "cfg" ) && !loadedTraces.empty() )
    {
      DoLoadCFG( fileName );
    }

    fileName.erase();
  }
}

/*!
 * paraverMain creator
 */

bool paraverMain::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin paraverMain creation
  wxFrame::Create( parent, id, caption, pos, size, style );

  CreateControls();
////@end paraverMain creation

  return true;
}


/*!
 * paraverMain destructor
 */

paraverMain::~paraverMain()
{
////@begin paraverMain destruction
  GetAuiManager().UnInit();
////@end paraverMain destruction

// DISCONNECTS
  wxMenuItem *tmpItem2 = menuFile->FindItem( ID_RECENTTRACES );
  wxMenu *menuTraces2 = tmpItem2->GetSubMenu();
  wxMenuItemList& menuItems2 = menuTraces2->GetMenuItems();
  for (wxMenuItemList::iterator menuIt = menuItems2.begin(); menuIt != menuItems2.end() ; ++menuIt )
  {
    wxMenuItem *tmp = *menuIt;
    int id = tmp->GetId();
    Disconnect( id, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&paraverMain::OnPreviousTracesClick );
  }

  wxMenuItem *tmpItem = menuFile->FindItem( ID_RECENTCFGS );
  wxMenu *menuCFGs = tmpItem->GetSubMenu();
  wxMenuItemList& menuItems = menuCFGs->GetMenuItems();
  for (wxMenuItemList::iterator menuIt = menuItems.begin(); menuIt != menuItems.end() ; ++menuIt )
  {
    wxMenuItem *tmp = *menuIt;
    int id = tmp->GetId();
    Disconnect( id, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&paraverMain::OnPreviousCFGsClick );
  }

  for( vector<Trace *>::iterator it = loadedTraces.begin(); it != loadedTraces.end(); ++it )
    delete *it;
  delete localKernel;
  
  delete imageList;
}


/*!
 * Member initialisation
 */

void paraverMain::Init()
{
////@begin paraverMain member initialisation
  currentTrace = -1;
  currentTimeline = NULL;
  currentHisto = NULL;
  previousTraces = PreviousFiles::createPreviousTraces();
  previousCFGs = PreviousFiles::createPreviousCFGs();
  lastTimeline = NULL;
  lastHisto = NULL;
  currentWindow = NULL;
  numNewWindows = 0;
  numNewHistograms = 0;
  numNewDerived = 0;
  raiseCurrentWindow = true;
  canServeSignal = true;
  menuFile = NULL;
  menuHelp = NULL;
  tbarMain = NULL;
  choiceWindowBrowser = NULL;
  toolBookFilesProperties = NULL;
  dirctrlFiles = NULL;
  windowProperties = NULL;
////@end paraverMain member initialisation

  traceLoadedBefore = false;
  CFGLoadedBefore = false;
}

/*!
 * Control creation for paraverMain
 */

void paraverMain::CreateControls()
{    
////@begin paraverMain content construction
  paraverMain* itemFrame1 = this;

  GetAuiManager().SetManagedWindow(this);

  wxMenuBar* menuBar = new wxMenuBar;
  menuFile = new wxMenu;
  menuFile->Append(wxID_OPEN, _("Load &Trace..."), wxEmptyString, wxITEM_NORMAL);
  wxMenu* itemMenu5 = new wxMenu;
  menuFile->Append(ID_RECENTTRACES, _("Previous Traces"), itemMenu5);
  menuFile->Append(ID_UNLOADTRACE, _("Unload Trace..."), wxEmptyString, wxITEM_NORMAL);
  menuFile->AppendSeparator();
  menuFile->Append(ID_MENULOADCFG, _("Load &Configuration..."), wxEmptyString, wxITEM_NORMAL);
  wxMenu* itemMenu9 = new wxMenu;
  menuFile->Append(ID_RECENTCFGS, _("Previous Configurations"), itemMenu9);
  menuFile->Append(ID_MENUSAVECFG, _("&Save Configuration..."), wxEmptyString, wxITEM_NORMAL);
  menuFile->AppendSeparator();
  menuFile->Append(ID_PREFERENCES, _("&Preferences..."), wxEmptyString, wxITEM_NORMAL);
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT, _("&Quit"), wxEmptyString, wxITEM_NORMAL);
  menuBar->Append(menuFile, _("&File"));
  menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT, _("&About..."), wxEmptyString, wxITEM_NORMAL);
  menuBar->Append(menuHelp, _("&Help"));
  itemFrame1->SetMenuBar(menuBar);

  tbarMain = new wxToolBar( itemFrame1, ID_TOOLBAR, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL|wxTB_NODIVIDER|wxWANTS_CHARS );
  wxBitmap itemtool18Bitmap(itemFrame1->GetBitmapResource(wxT("new_window.xpm")));
  wxBitmap itemtool18BitmapDisabled;
  tbarMain->AddTool(ID_NEW_WINDOW, _("Create new window"), itemtool18Bitmap, itemtool18BitmapDisabled, wxITEM_NORMAL, _("New single timeline window"), wxEmptyString);
  tbarMain->EnableTool(ID_NEW_WINDOW, false);
  wxBitmap itemtool19Bitmap(itemFrame1->GetBitmapResource(wxT("new_derived_window.xpm")));
  wxBitmap itemtool19BitmapDisabled;
  tbarMain->AddTool(ID_NEW_DERIVED_WINDOW, _("Create new derived window"), itemtool19Bitmap, itemtool19BitmapDisabled, wxITEM_NORMAL, _("New derived timeline window"), wxEmptyString);
  tbarMain->EnableTool(ID_NEW_DERIVED_WINDOW, false);
  wxBitmap itemtool20Bitmap(itemFrame1->GetBitmapResource(wxT("new_histogram.xpm")));
  wxBitmap itemtool20BitmapDisabled;
  tbarMain->AddTool(ID_NEW_HISTOGRAM, _("Create new histogram"), itemtool20Bitmap, itemtool20BitmapDisabled, wxITEM_NORMAL, _("New histogram"), wxEmptyString);
  tbarMain->EnableTool(ID_NEW_HISTOGRAM, false);
  tbarMain->AddSeparator();
  wxBitmap itemtool22Bitmap(itemFrame1->GetBitmapResource(wxT("delete.xpm")));
  wxBitmap itemtool22BitmapDisabled;
  tbarMain->AddTool(ID_TOOLDELETE, _("Delete window"), itemtool22Bitmap, itemtool22BitmapDisabled, wxITEM_NORMAL, _("Delete selected window"), wxEmptyString);
  tbarMain->AddSeparator();
  wxBitmap itemtool24Bitmap(itemFrame1->GetBitmapResource(wxT("cut_trace.xpm")));
  wxBitmap itemtool24BitmapDisabled;
  tbarMain->AddTool(ID_TOOL_CUT_TRACE, _("Filter Trace"), itemtool24Bitmap, itemtool24BitmapDisabled, wxITEM_NORMAL, _("Filter Trace"), wxEmptyString);
  tbarMain->Realize();
  itemFrame1->GetAuiManager().AddPane(tbarMain, wxAuiPaneInfo()
    .ToolbarPane().Name(_T("auiTBarMain")).Top().Layer(10).CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(false).Floatable(false).Gripper(true));

  choiceWindowBrowser = new wxChoicebook( itemFrame1, ID_CHOICEWINBROWSER, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT|wxWANTS_CHARS );

  itemFrame1->GetAuiManager().AddPane(choiceWindowBrowser, wxAuiPaneInfo()
    .Name(_T("auiWindowBrowser")).Caption(_("Window browser")).Centre().CloseButton(false).DestroyOnClose(false).Resizable(true));

  toolBookFilesProperties = new wxToolbook( itemFrame1, ID_TOOLBOOKFILESANDPROPERTIES, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );
  wxImageList* toolBookFilesPropertiesImageList = new wxImageList(16, 16, true, 2);
  {
    wxIcon toolBookFilesPropertiesIcon0(itemFrame1->GetIconResource(wxT("file_browser.xpm")));
    toolBookFilesPropertiesImageList->Add(toolBookFilesPropertiesIcon0);
    wxIcon toolBookFilesPropertiesIcon1(itemFrame1->GetIconResource(wxT("window_properties.xpm")));
    toolBookFilesPropertiesImageList->Add(toolBookFilesPropertiesIcon1);
  }
  toolBookFilesProperties->AssignImageList(toolBookFilesPropertiesImageList);

  dirctrlFiles = new wxGenericDirCtrl( toolBookFilesProperties, ID_DIRCTRLFILES, _T("/home/eloy/etc/cfgs"), wxDefaultPosition, wxDefaultSize, wxDIRCTRL_SHOW_FILTERS, _T("Paraver files|*.prv;*.prv.gz;*.cfg|CFG files (*.cfg)|*.cfg|PRV Files (*.prv, *.prv.gz)|*.prv;*.prv.gz"), 0 );

  toolBookFilesProperties->AddPage(dirctrlFiles, wxEmptyString, false, 0);

  windowProperties = new wxPropertyGrid( toolBookFilesProperties, ID_FOREIGN, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxWANTS_CHARS );

  toolBookFilesProperties->AddPage(windowProperties, wxEmptyString, false, 1);

  itemFrame1->GetAuiManager().AddPane(toolBookFilesProperties, wxAuiPaneInfo()
    .Name(_T("auiCfgAndProperties")).Caption(_("Files & Window Properties")).Centre().Position(1).CloseButton(false).DestroyOnClose(false).Resizable(true).PaneBorder(false));

  GetAuiManager().Update();

////@end paraverMain content construction
  wxTreeCtrl* tmpTree = createTree( imageList );
  choiceWindowBrowser->AddPage( tmpTree, _( "All Traces" ) );
  
  toolBookFilesProperties->GetToolBar()->SetToolShortHelp( 1, wxT("Paraver Files") );
  toolBookFilesProperties->GetToolBar()->SetToolShortHelp( 2, wxT("Window Properties") );
}


bool paraverMain::DoLoadTrace( const string &path )
{
  Trace *tr = NULL;
  bool loaded = true;

  canServeSignal = false;

  if( !localKernel->checkTraceSize( path, ParaverConfig::getInstance()->getFiltersFilterTraceUpToMB() * 1E6 ) )
  {
    wxMessageDialog maxSizeDialog( this, 
                                   wxString( "The maximum size for trace ",  wxConvUTF8 ) + wxString( path.c_str(), wxConvUTF8 ) + wxString( " is reached.\nWould you like to cut or filter the trace?",  wxConvUTF8 ),
                                   wxT( "Maximum size reached" ),
                                   wxYES_NO|wxCANCEL|wxICON_QUESTION );

    switch( maxSizeDialog.ShowModal() )
    {
      case wxID_YES:
        ShowCutTraceWindow( path, true );
        canServeSignal = true;
        return true;
        break;
        
      case wxID_NO:
        break;
      
      case wxID_CANCEL:
        canServeSignal = true;
        return true;
        break;
        
    }
  }
  
  map< string, PRV_UINT32 >::iterator it = traceInstance.find( path );
  if ( it == traceInstance.end() )
    traceInstance[ path ] = 0;

/*
  for( vector<Trace *>::iterator it = loadedTraces.begin(); it != loadedTraces.end(); ++it )
  {
    if( (*it)->getFileName().compare( path ) == 0 )
    // Trace is loaded.
      timesLoaded++;
  }
  traceWasLoaded = ( timesLoaded > 0 );
*/

  ProgressController *progress = ProgressController::create( localKernel );
  progress->setHandler( progressFunction );

  try
  {
    if( paraverMain::dialogProgress == NULL )
      paraverMain::dialogProgress = new wxProgressDialog( wxT("Loading trace..."), wxT(""),numeric_limits<int>::max(),
                                         this,
                                         wxPD_CAN_ABORT|wxPD_AUTO_HIDE|wxPD_APP_MODAL|wxPD_ELAPSED_TIME|wxPD_ESTIMATED_TIME|wxPD_REMAINING_TIME );
    string reducePath;
#ifdef WIN32
    if( path.length() > 40 && path.find_last_of( '\\' ) != string::npos )
    {
      string file = path.substr( path.find_last_of( '\\' ) );
      string tmp = path.substr( 0, path.find_last_of( '\\' ) );
      reducePath = "/..." + path.substr( tmp.find_last_of( '\\' ),
                                         tmp.length() - tmp.find_last_of( '\\' ) )
                   + file;
#else
    if( path.length() > 40 && path.find_last_of( '/' ) != string::npos )
    {
      string file = path.substr( path.find_last_of( '/' ) );
      string tmp = path.substr( 0, path.find_last_of( '/' ) );
      reducePath = "/..." + path.substr( tmp.find_last_of( '/' ),
                                         tmp.length() - tmp.find_last_of( '/' ) )
                   + file;
#endif
    }
    else
      reducePath = path;
    reducePath += "\t";
    paraverMain::dialogProgress->Pulse( wxString::FromAscii( reducePath.c_str() ) );
    paraverMain::dialogProgress->Fit();
    paraverMain::dialogProgress->Show();

    tr = Trace::create( localKernel, path, false, progress );
    tr->setInstanceNumber( traceInstance[ path ]++ );

    loadedTraces.push_back( tr );
    currentTrace = loadedTraces.size() - 1;
    wxTreeCtrl *newTree = createTree( imageList );

    choiceWindowBrowser->AddPage( newTree, wxString::FromAscii( tr->getFileNameNumbered().c_str() ) );
    choiceWindowBrowser->ChangeSelection( choiceWindowBrowser->GetPageCount() - 1 );

    previousTraces->add( path );

    currentTimeline = NULL;
    currentHisto = NULL;
  }
  catch( ParaverKernelException& ex )
  {
    loaded = false;
    wxMessageDialog message( this, wxString::FromAscii( ex.what() ), _( "Error loading trace" ), wxOK );
    raiseCurrentWindow = false;
    message.ShowModal();
    raiseCurrentWindow = true;
  }
  paraverMain::dialogProgress->Show( false );
  delete paraverMain::dialogProgress;
  paraverMain::dialogProgress = NULL;
  delete progress;

  canServeSignal = true;
  
#ifndef WIN32
  if ( sig1 || sig2 )
    OnSignal();
#endif

  return loaded;
}

bool paraverMain::DoLoadCFG( const string &path )
{
  if( !CFGLoader::isCFGFile( path ))
  {
    wxString errMessage = wxString::FromAscii( path.c_str() ) + _( " isn't a valid cfg." );
    wxMessageDialog message( this, errMessage, _( "Invalid file" ), wxOK );
    raiseCurrentWindow = false;
    message.ShowModal();
    raiseCurrentWindow = true;
  }
  else
  {
    vector<Window *> newWindows;
    vector<Histogram *> newHistograms;
      
    if( !CFGLoader::loadCFG( localKernel, path, loadedTraces[ currentTrace ], newWindows, newHistograms ) )
    {
      wxString errMessage = wxString::FromAscii( path.c_str() ) + _( " failed to load in:\n'" ) + wxString::FromAscii( CFGLoader::errorLine.c_str() ) + _( "'" );
      wxMessageDialog message( this, errMessage, _( "Loading error" ), wxOK );
      raiseCurrentWindow = false;
      message.ShowModal();
      raiseCurrentWindow = true;
    }
    else
    {
      wxTreeItemId child;
      for( vector<Window *>::iterator it = newWindows.begin(); it != newWindows.end(); ++it )
      {
        wxTreeCtrl *allTracesPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 );
        wxTreeCtrl *currentPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( currentTrace + 1 );

        if ( (*it)->getChild() == NULL )
          BuildTree( this, allTracesPage, allTracesPage->GetRootItem(), currentPage, currentPage->GetRootItem(), *it );

        if ( it + 1 == newWindows.end() )
          currentTimeline = *it;
      }

      for( vector<Histogram *>::iterator it = newHistograms.begin(); it != newHistograms.end(); ++it )
      {
        wxPoint tmpPos( (*it)->getPosX(), (*it)->getPosY() );
        gHistogram* tmpHisto = new gHistogram( this, wxID_ANY, wxString::FromAscii( (*it)->getName().c_str() ), tmpPos );
        tmpHisto->SetHistogram( *it );

        appendHistogram2Tree( tmpHisto );
        LoadedWindows::getInstance()->add( (*it) );

        tmpHisto->SetClientSize( wxSize( (*it)->getWidth(), (*it)->getHeight() ) );
        if( (*it)->getShowWindow() )
        {
          tmpHisto->Show();
        }
        tmpHisto->execute();
      }

      previousCFGs->add( path );
    }
  }
  return true;
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_OPEN
 */
void paraverMain::OnOpenClick( wxCommandEvent& event )
{
  if ( !traceLoadedBefore )
    tracePath = wxString::FromAscii( paraverConfig->getGlobalTracesPath().c_str() );

  wxFileDialog dialog( this, _( "Load Trace" ), tracePath, _( "" ), 
    _( "Paraver trace (*.prv;*.prv.gz)|*.prv;*.prv.gz|All files (*.*)|*.*" ),
    wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_CHANGE_DIR );

  raiseCurrentWindow = false;
  if( dialog.ShowModal() == wxID_OK )
  {
    wxString path = dialog.GetPath();
    traceLoadedBefore = true;
    tracePath = wxFileName( path ).GetPath();
    DoLoadTrace( std::string( path.mb_str() ) );
  }
  raiseCurrentWindow = true;
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENULOADCFG
 */

void paraverMain::OnMenuloadcfgClick( wxCommandEvent& event )
{
  if ( !CFGLoadedBefore )
   CFGPath =  wxString::FromAscii( paraverConfig->getGlobalCFGsPath().c_str() );

  wxFileDialog dialog( this, _( "Load Configuration" ), CFGPath, _( "" ),
    _( "Paraver configuration file (*.cfg)|*.cfg|All files (*.*)|*.*" ),
    wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_CHANGE_DIR );
  raiseCurrentWindow = false;
  if( dialog.ShowModal() == wxID_OK )
  {
    wxString path = dialog.GetPath();
    CFGLoadedBefore = true;
    CFGPath = wxFileName( path ).GetPath();
    DoLoadCFG( std::string( path.mb_str() ) );
  }
  raiseCurrentWindow = true;
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
 */

void paraverMain::OnExitClick( wxCommandEvent& event )
{
  PrepareToExit();
  Destroy();
}


/*!
 * Should we show tooltips?
 */

bool paraverMain::ShowToolTips()
{
  static size_t s_index = (size_t) -1;
  if ( s_index == (size_t)-1 )
  {
    srand( time( NULL ));
    s_index = rand() % 2; // number of tips
  }
  /*
  wxTipProvider *tipProvider = wxCreateFileTipProvider( wxT( "./tips.txt" ), s_index );
  
  // this returns a bool 
  wxShowTip( this, tipProvider, true);
  delete tipProvider;
  */
  
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap paraverMain::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin paraverMain bitmap retrieval
  wxUnusedVar(name);
  if (name == _T("new_window.xpm"))
  {
    wxBitmap bitmap(application_star_xpm);
    return bitmap;
  }
  else if (name == _T("new_derived_window.xpm"))
  {
    wxBitmap bitmap(application_add_xpm);
    return bitmap;
  }
  else if (name == _T("new_histogram.xpm"))
  {
    wxBitmap bitmap(new_histogram_xpm);
    return bitmap;
  }
  else if (name == _T("delete.xpm"))
  {
    wxBitmap bitmap(delete_xpm);
    return bitmap;
  }
  else if (name == _T("cut_trace.xpm"))
  {
    wxBitmap bitmap(cut_trace_xpm);
    return bitmap;
  }
  return wxNullBitmap;
////@end paraverMain bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon paraverMain::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin paraverMain icon retrieval
  wxUnusedVar(name);
  if (name == _T("file_browser.xpm"))
  {
    wxIcon icon(file_browser_xpm);
    return icon;
  }
  else if (name == _T("window_properties.xpm"))
  {
    wxIcon icon(window_properties_xpm);
    return icon;
  }
  return wxNullIcon;
////@end paraverMain icon retrieval
}

/*!
 * wxEVT_PG_CHANGED event handler for ID_FOREIGN
 */
void paraverMain::OnPropertyGridChange( wxPropertyGridEvent& event )
{
  wxPGProperty *property = event.GetProperty();

  if( !property )
    return;
    
  const wxString& propName = property->GetName();

  if( propName == _( "Name" ) )
  {
    wxString tmpName = property->GetValue().GetString();
    if( currentTimeline != NULL )
      currentTimeline->setName( std::string( tmpName.mb_str() ) );
    else if( currentHisto != NULL )
      currentHisto->setName( std::string( tmpName.mb_str() ) );
  }
  else if( propName == _( "Begin time" ) )
  {
    TTime tmpValue;
    if( currentTimeline != NULL )
    {
      bool done = LabelConstructor::getTimeValue( std::string( property->GetValue().GetString().mb_str()),
                                                  currentTimeline->getTimeUnit(),
                                                  ParaverConfig::getInstance()->getTimelinePrecision(),
                                                  tmpValue );

      if (!done)
        tmpValue = property->GetValue().GetDouble();

      currentTimeline->setWindowBeginTime( currentTimeline->windowUnitsToTraceUnits( tmpValue ) );

      // modify current zoom directly
      pair< TTime, TTime > zoomInfo = currentTimeline->getZoomFirstDimension();
      zoomInfo.first = property->GetValue().GetDouble(); // begin
      currentTimeline->setZoomFirstDimension( zoomInfo );

      currentTimeline->setChanged( true );
      currentTimeline->setRedraw( true );
    }
    else if( currentHisto != NULL )
    {
      bool done = LabelConstructor::getTimeValue( std::string( property->GetValue().GetString().mb_str()),
                                                  currentHisto->getControlWindow()->getTimeUnit(),
                                                  ParaverConfig::getInstance()->getTimelinePrecision(),
                                                  tmpValue );

      if (!done)
        tmpValue = property->GetValue().GetDouble();

      currentHisto->setWindowBeginTime( currentHisto->getControlWindow()->windowUnitsToTraceUnits( tmpValue ) );
      currentHisto->setChanged( true );
      currentHisto->setRecalc( true );
    }
  }
  else if( propName == _( "End time" ) )
  {
    TTime tmpValue;
    if( currentTimeline != NULL )
    {
      bool done = LabelConstructor::getTimeValue( std::string( property->GetValue().GetString().mb_str()),
                                                  currentTimeline->getTimeUnit(),
                                                  ParaverConfig::getInstance()->getTimelinePrecision(),
                                                  tmpValue );

      if (!done)
        tmpValue = property->GetValue().GetDouble();

      currentTimeline->setWindowEndTime( currentTimeline->windowUnitsToTraceUnits( tmpValue ) );

      // modify current zoom directly
      pair< TTime, TTime > zoomInfo = currentTimeline->getZoomFirstDimension();
      zoomInfo.second = property->GetValue().GetDouble(); // end
      currentTimeline->setZoomFirstDimension( zoomInfo );

      currentTimeline->setChanged( true );
      currentTimeline->setRedraw( true );
    }
    else if( currentHisto != NULL )
    {
      bool done = LabelConstructor::getTimeValue( std::string( property->GetValue().GetString().mb_str()),
                                                  currentHisto->getControlWindow()->getTimeUnit(),
                                                  ParaverConfig::getInstance()->getTimelinePrecision(),
                                                  tmpValue );

      if (!done)
        tmpValue = property->GetValue().GetDouble();

      currentHisto->setWindowEndTime( currentHisto->getControlWindow()->windowUnitsToTraceUnits( tmpValue ) );
      currentHisto->setChanged( true );
      currentHisto->setRecalc( true );
    }
  }
  else if( propName == _( "Semantic Minimum" ) )
  {
    double tmpValue = property->GetValue().GetDouble();
    currentTimeline->setMinimumY( tmpValue );
    currentTimeline->setRedraw( true );
  }
  else if( propName == _( "Semantic Maximum" ) )
  {
    double tmpValue = property->GetValue().GetDouble();
    currentTimeline->setMaximumY( tmpValue );
    currentTimeline->setRedraw( true );
  }
  // Control Window related properties
  else if( propName == _( "ControlWindow" ) )
  {
    currentHisto->setControlWindow( LoadedWindows::getInstance()->getWindow( property->GetValue().GetLong() ) );
    currentHisto->setRecalc( true );
  }
  else if( propName == _( "ControlMinimum" ) )
  {
    currentHisto->setControlMin( property->GetValue().GetDouble() );

    // modify current zoom directly
    pair< HistogramProxy::TZoomInfo, HistogramProxy::TZoomInfo > zoomInfo = currentHisto->getZoomFirstDimension();
    zoomInfo.first.begin = property->GetValue().GetDouble(); // minimum
    currentHisto->setZoomFirstDimension( zoomInfo );

    currentHisto->setCompute2DScale( false );
    currentHisto->setRecalc( true );
  }
  else if( propName == _( "ControlMaximum" ) )
  {
    currentHisto->setControlMax( property->GetValue().GetDouble() );

    // modify current zoom directly
    pair< HistogramProxy::TZoomInfo, HistogramProxy::TZoomInfo > zoomInfo = currentHisto->getZoomFirstDimension();
    zoomInfo.first.end = property->GetValue().GetDouble(); // maximum
    currentHisto->setZoomFirstDimension( zoomInfo );

    currentHisto->setCompute2DScale( false );
    currentHisto->setRecalc( true );
  }
  else if( propName == _( "ControlDelta" ) )
  {
    if( property->GetValue().GetDouble() == 0 )
    {
      property->SetValue( currentHisto->getControlDelta() );
      return;
    }
    currentHisto->setControlDelta( property->GetValue().GetDouble() );

    // modify current zoom directly
    pair< HistogramProxy::TZoomInfo, HistogramProxy::TZoomInfo > zoomInfo = currentHisto->getZoomFirstDimension();
    zoomInfo.second.begin = property->GetValue().GetDouble(); // delta
    currentHisto->setZoomFirstDimension( zoomInfo );

    currentHisto->setCompute2DScale( false );
    currentHisto->setRecalc( true );
  }
// Data Window related properties
  else if( propName == _( "DataWindow" ) )
  {
    currentHisto->setDataWindow( LoadedWindows::getInstance()->getWindow( property->GetValue().GetLong() ) );
    currentHisto->setRecalc( true );
  }
  else if( propName == _( "DataMinimum" ) )
  {
    currentHisto->setMinGradient( property->GetValue().GetDouble() );
    currentHisto->setComputeGradient( false );
    currentHisto->setRedraw( true );
  }
  else if( propName == _( "DataMaximum" ) )
  {
    currentHisto->setMaxGradient( property->GetValue().GetDouble() );
    currentHisto->setComputeGradient( false );
    currentHisto->setRedraw( true );
  }
  // Histogram related properties
  else if( propName == _( "Type" ) )
  {
    if( property->GetValue().GetLong() == 0 )
      currentHisto->setCurrentStat( currentHisto->getFirstCommStatistic() );
    else
      currentHisto->setCurrentStat( currentHisto->getFirstStatistic() );
    currentHisto->setRedraw( true );
    currentHisto->setChanged( true );
  }
  else if( propName == _( "Statistic" ) )
  {
    currentHisto->setCurrentStat( std::string( property->GetDisplayedString().mb_str() ) );
    currentHisto->setRedraw( true );
  }
  else if( propName == _( "3rd Window" ) )
  {
    if( property->GetValue().GetLong() == -1 )
      currentHisto->clearExtraControlWindow();
    else
      currentHisto->setExtraControlWindow( LoadedWindows::getInstance()->getWindow( property->GetValue().GetLong() ) );
    currentHisto->setRecalc( true );
  }
  else if( propName == _( "3DMinimum" ) )
  {
    currentHisto->setExtraControlMin( property->GetValue().GetDouble() );
    currentHisto->setCompute3DScale( false );
    currentHisto->setRecalc( true );
  }
  else if( propName == _( "3DMaximum" ) )
  {
    currentHisto->setExtraControlMax( property->GetValue().GetDouble() );
    currentHisto->setCompute3DScale( false );
    currentHisto->setRecalc( true );
  }
  else if( propName == _( "3DDelta" ) )
  {
    if( property->GetValue().GetDouble() == 0 )
    {
      property->SetValue( currentHisto->getExtraControlDelta() );
      return;
    }
    
    currentHisto->setExtraControlDelta( property->GetValue().GetDouble() );
    currentHisto->setCompute3DScale( false );
    currentHisto->setRecalc( true );
  }
  else if( propName == _( "3DPlane" ) )
  {
    currentHisto->setSelectedPlane( property->GetValue().GetLong() );
    currentHisto->setRedraw( true );
  }
  
  // Timeline related properties
  else if( propName == _( "Level" ) )
  {
    currentTimeline->setLevel( (TWindowLevel)property->GetValue().GetLong() );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == _( "Time Unit" ) )
  {
    currentTimeline->setTimeUnit( (TWindowLevel)property->GetValue().GetLong() );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == _( "Logical" ) )
  {
    currentTimeline->getFilter()->setLogical( property->GetValue().GetBool() );
    currentTimeline->setRedraw( true );
  }
  else if( propName == _( "Physical" ) )
  {
    currentTimeline->getFilter()->setPhysical( property->GetValue().GetBool() );
    currentTimeline->setRedraw( true );
  }
  else if( propName == _( "Comm from.FromFunction" ) )
  {
    currentTimeline->getFilter()->setCommFromFunction( std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
  }
  else if( propName == _( "Comm from.From" ) )
  {
    Filter *filter = currentTimeline->getFilter();
    filter->clearCommFrom();
    wxArrayString value = property->GetValue().GetArrayString();
    for( unsigned int idx = 0; idx < value.GetCount(); idx++ )
    {
      long tmpLong;
      value[ idx ].ToLong( &tmpLong );
      filter->insertCommFrom( tmpLong - 1 );
    }
    
    currentTimeline->setRedraw( true );
  }
  else if( propName == _( "FromToOp" ) )
  {
    long op = property->GetValue().GetLong();
    Filter *filter = currentTimeline->getFilter();
    if( op == 0 )
      filter->setOpFromToAnd();
    else if( op == 1 )
      filter->setOpFromToOr();
      
    currentTimeline->setRedraw( true );
  }
  else if( propName == _( "Comm to.ToFunction" ) )
  {
    currentTimeline->getFilter()->setCommToFunction( std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
  }
  else if( propName == _( "Comm to.To" ) )
  {
    Filter *filter = currentTimeline->getFilter();
    filter->clearCommTo();
    wxArrayString value = property->GetValue().GetArrayString();
    for( unsigned int idx = 0; idx < value.GetCount(); idx++ )
    {
      long tmpLong;
      value[ idx ].ToLong( &tmpLong );
      filter->insertCommTo( tmpLong - 1 );
    }
    
    currentTimeline->setRedraw( true );
  }
  else if( propName == _( "Comm tag.TagFunction" ) )
  {
    currentTimeline->getFilter()->setCommTagFunction( std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
  }
  else if( propName == _( "Comm tag.Tag" ) )
  {
    Filter *filter = currentTimeline->getFilter();
    filter->clearCommTags();
    wxArrayString value = property->GetValue().GetArrayString();
    for( unsigned int idx = 0; idx < value.GetCount(); idx++ )
    {
      long long tmpLong;
      value[ idx ].ToLongLong( &tmpLong );
      filter->insertCommTag( tmpLong );
    }
    
    currentTimeline->setRedraw( true );
  }
  else if( propName == _( "TagSizeOp" ) )
  {
    long op = property->GetValue().GetLong();
    Filter *filter = currentTimeline->getFilter();
    if( op == 0 )
      filter->setOpTagSizeAnd();
    else if( op == 1 )
      filter->setOpTagSizeOr();
      
    currentTimeline->setRedraw( true );
  }
  else if( propName == _( "Comm size.SizeFunction" ) )
  {
    currentTimeline->getFilter()->setCommSizeFunction( std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
  }
  else if( propName == _( "Comm size.Size" ) )
  {
    Filter *filter = currentTimeline->getFilter();
    filter->clearCommSizes();
    wxArrayString value = property->GetValue().GetArrayString();
    for( unsigned int idx = 0; idx < value.GetCount(); idx++ )
    {
      long long tmpLong;
      value[ idx ].ToLongLong( &tmpLong );
      filter->insertCommSize( tmpLong );
    }
    
    currentTimeline->setRedraw( true );
  }
  else if( propName == _( "Comm bandwidth.BWFunction" ) )
  {
    currentTimeline->getFilter()->setBandWidthFunction( std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
  }
  else if( propName == _( "Comm bandwidth.Bandwidth" ) )
  {
    Filter *filter = currentTimeline->getFilter();
    filter->clearBandWidth();
    wxArrayString value = property->GetValue().GetArrayString();
    for( unsigned int idx = 0; idx < value.GetCount(); idx++ )
    {
      double tmpDouble;
      value[ idx ].ToDouble( &tmpDouble );
      filter->insertBandWidth( tmpDouble );
    }
    
    currentTimeline->setRedraw( true );
  }
  else if( propName == _( "Event type.TypeFunction" ) )
  {
    currentTimeline->getFilter()->setEventTypeFunction( std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
  }
  else if( propName == _( "Event type.Types" ) )
  {
    Filter *filter = currentTimeline->getFilter();
    filter->clearEventTypes();
    wxArrayInt value = ( (prvEventTypeProperty *) property )->GetValueAsArrayInt();
    for( unsigned int idx = 0; idx < value.GetCount(); idx++ )
      filter->insertEventType( value[ idx ] );
    
    currentTimeline->setRedraw( true );
  }
  else if( propName == _( "TypeValueOp" ) )
  {
    long op = property->GetValue().GetLong();
    Filter *filter = currentTimeline->getFilter();
    if( op == 0 )
      filter->setOpTypeValueAnd();
    else if( op == 1 )
      filter->setOpTypeValueOr();
      
    currentTimeline->setRedraw( true );
  }
  else if( propName == _( "Event value.ValueFunction" ) )
  {
    currentTimeline->getFilter()->setEventValueFunction( std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
  }
  else if( propName == _( "Event value.Values" ) )
  {
    Filter *filter = currentTimeline->getFilter();
    filter->clearEventValues();
    wxArrayString value = property->GetValue().GetArrayString();
    for( unsigned int idx = 0; idx < value.GetCount(); idx++ )
    {
      unsigned long long tmpLong;
      value[ idx ].ToULongLong( &tmpLong );
      filter->insertEventValue( tmpLong );
    }
    
    currentTimeline->setRedraw( true );
  }
  else if( propName == _( "Top Compose 1" ) )
  {
    currentTimeline->setLevelFunction( TOPCOMPOSE1, std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == _( "Top Compose 2" ) )
  {
    currentTimeline->setLevelFunction( TOPCOMPOSE2, std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == _( "Factor #1" ) )
  {
    currentTimeline->setFactor( 0, property->GetValue().GetDouble() );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == _( "Derived" ) )
  {
    currentTimeline->setLevelFunction( DERIVED, std::string( property->GetDisplayedString().mb_str() ) );

    // Change Derived operation Icon: find icon in list
    int iconNumber = 1; // number of timeline icon
    string derivedFunctionName;
    if ( currentTimeline->isDerivedWindow() )
    {
      derivedFunctionName = currentTimeline->getLevelFunction( DERIVED );

      // GUI should'nt know these tags -> add operation to kernel
      if ( derivedFunctionName == "add" )
        iconNumber = 2;
      else if  ( derivedFunctionName == "product" )
        iconNumber = 3;
      else if  ( derivedFunctionName == "substract" )
        iconNumber = 4;
      else if  ( derivedFunctionName == "divide" )
        iconNumber = 5;
      else if  ( derivedFunctionName == "maximum" )
        iconNumber = 6;
      else if  ( derivedFunctionName == "minimum" )
        iconNumber = 7;
      else if  ( derivedFunctionName == "different" )
        iconNumber = 8;
      else if  ( derivedFunctionName == "controlled: clear by" )
        iconNumber = 9;
      else if  ( derivedFunctionName == "controlled: maximum" )
        iconNumber = 10;
      else if ( derivedFunctionName == "controlled: add" )
        iconNumber = 11;
    }

    // Change Derived operation Icon: set both trees, global and trace tree
    wxTreeCtrl *globalTreePage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 );
    wxTreeCtrl *currentTraceTreePage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( currentTrace + 1 );
    bool found;
    wxTreeItemId idInGlobalTree = getItemIdFromWindow( globalTreePage->GetRootItem(), currentTimeline, found );
    globalTreePage->SetItemImage( idInGlobalTree, iconNumber );
    wxTreeItemId idInCurrentTraceTree = getItemIdFromWindow( currentTraceTreePage->GetRootItem(), currentTimeline, found );
    currentTraceTreePage->SetItemImage( idInCurrentTraceTree, iconNumber );

    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == _( "Factor #2" ) )
  {
    currentTimeline->setFactor( 1, property->GetValue().GetDouble() );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == _( "Compose Workload" ) )
  {
    currentTimeline->setLevelFunction( COMPOSEWORKLOAD, std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == _( "Workload" ) )
  {
    currentTimeline->setLevelFunction( WORKLOAD, std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == _( "Compose Appl" ) )
  {
    currentTimeline->setLevelFunction( COMPOSEAPPLICATION, std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == _( "Application" ) )
  {
    currentTimeline->setLevelFunction( APPLICATION, std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == _( "Compose Task" ) )
  {
    currentTimeline->setLevelFunction( COMPOSETASK, std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == _( "Task" ) )
  {
    currentTimeline->setLevelFunction( TASK, std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == _( "Compose System" ) )
  {
    currentTimeline->setLevelFunction( COMPOSESYSTEM, std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == _( "System" ) )
  {
    currentTimeline->setLevelFunction( SYSTEM, std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == _( "Compose Node" ) )
  {
    currentTimeline->setLevelFunction( COMPOSENODE, std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == _( "Node" ) )
  {
    currentTimeline->setLevelFunction( NODE, std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == _( "Compose CPU" ) )
  {
    currentTimeline->setLevelFunction( COMPOSECPU, std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == _( "CPU" ) )
  {
    currentTimeline->setLevelFunction( CPU, std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == _( "Compose Thread" ) )
  {
    currentTimeline->setLevelFunction( COMPOSETHREAD, std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == _( "Thread" ) )
  {
    currentTimeline->setLevelFunction( THREAD, std::string( property->GetDisplayedString().mb_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName.BeforeFirst( ' ' ) == _( "Param" ) )
  {
    wxString paramData = propName.AfterFirst( ' ' );
    TParamIndex paramIdx;
    TWindowLevel functionLevel;
    unsigned long tmpLong;
    
    paramData.BeforeFirst( ' ' ).ToULong( &tmpLong );
    paramIdx = tmpLong;
    paramData.AfterFirst( ' ' ).ToULong( &tmpLong );
    functionLevel = (TWindowLevel)tmpLong;
    
    wxArrayString valuesStr = property->GetValue().GetArrayString();
    TParamValue values;
    for( unsigned int idx = 0; idx < valuesStr.GetCount(); idx++ )
    {
      double tmpDouble;
      valuesStr[ idx ].ToDouble( &tmpDouble );
      values.push_back( tmpDouble );
    }
    currentTimeline->setFunctionParam( functionLevel, paramIdx, values );
    currentTimeline->setRedraw( true );
  }
}


/*!
 * wxEVT_TREE_SEL_CHANGED event handler for wxID_ANY
 */
void paraverMain::OnTreeSelChanged( wxTreeEvent& event )
{
  wxTreeCtrl *tmpTree = static_cast<wxTreeCtrl *>( event.GetEventObject() );
  if( tmpTree->GetParent()->GetId() == ID_DIRCTRLFILES )
  {
    event.Skip();
    return;
  }
  TreeBrowserItemData *itemData = static_cast<TreeBrowserItemData *>( tmpTree->GetItemData( event.GetItem() ) );

  endDragWindow = NULL;
  if( gHistogram *histo = itemData->getHistogram() ) // Is a histogram?
  {
    currentHisto = histo->GetHistogram();
    currentWindow = (wxWindow *)histo;

    currentTimeline = NULL;
    beginDragWindow = NULL;

    if( histo->IsShown() )
      histo->Raise();
  }
  else if( gTimeline *timeline = itemData->getTimeline() ) // Is a timeline.
  {
    currentTimeline = timeline->GetMyWindow();
    beginDragWindow = timeline->GetMyWindow();
    currentWindow = (wxWindow *)timeline;

    currentHisto = NULL;

    if( timeline->IsShown() )
      timeline->Raise();
  }
}

/*!
 * wxEVT_TREE_ITEM_ACTIVATED event handler for wxID_ANY
 */
void paraverMain::OnTreeItemActivated( wxTreeEvent& event )
{
  wxTreeCtrl *tmpTree = static_cast<wxTreeCtrl *>( event.GetEventObject() );
  if( tmpTree->GetParent()->GetId() == ID_DIRCTRLFILES )
  {
    wxFileName fileName( dirctrlFiles->GetPath() );
    if( !fileName.IsDir() )
    {
      wxString fileExt( fileName.GetExt() );
      if( fileExt == wxT( "prv" ) || fileExt == wxT( "prv.gz" ) )
        DoLoadTrace( std::string( fileName.GetFullPath().mb_str() ) );
      else if( fileExt == wxT( "cfg" ) && loadedTraces.size() > 0 )
        DoLoadCFG( std::string( fileName.GetFullPath().mb_str() ) );
    }
    event.Skip();
    return;
  }
  TreeBrowserItemData *itemData = static_cast<TreeBrowserItemData *>( tmpTree->GetItemData( event.GetItem() ) );

  endDragWindow = NULL;
  if( gHistogram *histo = itemData->getHistogram() )
  {
    Histogram *tmpHisto = histo->GetHistogram();

    beginDragWindow = NULL;

    tmpHisto->setShowWindow( !tmpHisto->getShowWindow() );
  }
  else if( gTimeline *timeline = itemData->getTimeline() )
  {
    Window *tmpWin = timeline->GetMyWindow();

    beginDragWindow = timeline->GetMyWindow();

    tmpWin->setShowWindow( !tmpWin->getShowWindow() );
  }
}


void paraverMain::OnTreeRightClick( wxTreeEvent& event )
{
  wxTreeCtrl *tmpTree = static_cast<wxTreeCtrl *>( event.GetEventObject() );
  if( tmpTree->GetParent()->GetId() == ID_DIRCTRLFILES )
  {
    event.Skip();
    return;
  }
  TreeBrowserItemData *itemData = static_cast<TreeBrowserItemData *>( tmpTree->GetItemData( event.GetItem() ) );
  
  endDragWindow = NULL;

  if( gHistogram *histo = itemData->getHistogram() )
  {
    beginDragWindow = NULL;

    histo->rightDownManager();
  }
  else if( gTimeline *timeline = itemData->getTimeline() )
  {
    beginDragWindow = timeline->GetMyWindow();

    timeline->rightDownManager();
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHOICEWINBROWSER
 */

void paraverMain::OnChoicewinbrowserUpdate( wxUpdateUIEvent& event )
{
  // Get a copy of all Windows and Histograms from LoadedWindows
  vector< Window * > allWindows;
  LoadedWindows::getInstance()->getAll( allWindows );
  vector< Histogram * > allHistograms;
  bool destroyed = false;

  // Update loop and delete
  for( unsigned int iPage = 0; iPage < choiceWindowBrowser->GetPageCount(); iPage++ )
  {
    if( iPage > 0 && choiceWindowBrowser->GetSelection() > 0 )
      currentWindow = NULL;
    wxTreeCtrl *currentTree = (wxTreeCtrl *) choiceWindowBrowser->GetPage( iPage );
    wxTreeItemId root = currentTree->GetRootItem();
    wxTreeItemIdValue cookie;
    wxTreeItemId currentChild = currentTree->GetFirstChild( root, cookie );
    unsigned int numberChild = currentTree->GetChildrenCount( root, false );
    unsigned int current = 0;
    while( current < numberChild )
    {
      if ( currentChild.IsOk() )
      {
        destroyed = updateTreeItem( currentTree, currentChild, allWindows, allHistograms, &currentWindow, iPage == 0 );
        if( iPage > 0 && choiceWindowBrowser->GetSelection() > 0 && currentWindow != NULL )
          choiceWindowBrowser->SetSelection( iPage );
      }
      else
        destroyed = false;

      if( !destroyed )
      {
        currentChild = currentTree->GetNextChild( root, cookie );
        ++current;
      }
      else
      {
        currentChild = currentTree->GetFirstChild( root, cookie );
        if( current == numberChild - 1 )
          ++current;
        else
          current = 0;
      }
    }
  }

   // add pending window or histogram
  for( vector<Window *>::iterator it = allWindows.begin(); it != allWindows.end(); ++it )
  {
    if( (*it)->getDestroy() )
      continue;

    wxTreeCtrl *allTracesPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 );
    wxTreeCtrl *currentPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( currentTrace + 1 );

    if ( (*it)->getChild() == NULL )
      BuildTree( this, allTracesPage, allTracesPage->GetRootItem(), currentPage, currentPage->GetRootItem(), *it );
  }

  for( vector<Histogram *>::iterator it = allHistograms.begin(); it != allHistograms.end(); ++it )
  {
    if( (*it)->getDestroy() )
      continue;
    gHistogram* tmpHisto = new gHistogram( this, wxID_ANY, wxString::FromAscii( (*it)->getName().c_str() ) );
    tmpHisto->SetHistogram( *it );

    appendHistogram2Tree( tmpHisto );
  }

  // No window or histogram? Disable current selection.
  if ( loadedTraces.size() > 0 )
  {
    allWindows.clear();
    allHistograms.clear();
    LoadedWindows::getInstance()->getAll( loadedTraces[ currentTrace ], allWindows );
    LoadedWindows::getInstance()->getAll( loadedTraces[ currentTrace ], allHistograms );

    if ( allWindows.size() == 0 && allHistograms.size() == 0 )
    {
      currentTimeline = NULL;
      currentHisto = NULL;
    }
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_FOREIGN
 */

void paraverMain::OnForeignUpdate( wxUpdateUIEvent& event )
{
  if( currentHisto != NULL )
  {
    lastTimeline = NULL;
    if( lastHisto == currentHisto && !currentHisto->getChanged() )
      return;
    lastHisto = currentHisto;

    updateHistogramProperties( windowProperties, currentHisto );
  }
  else if( currentTimeline != NULL )
  {
    lastHisto = NULL;
    if( lastTimeline == currentTimeline && !currentTimeline->getChanged() )
      return;
    lastTimeline = currentTimeline;

    updateTimelineProperties( windowProperties, currentTimeline );
  }
}


void paraverMain::OnPreviousTracesClick( wxCommandEvent& event )
{
  int eventId = event.GetId();
  int i = 0;

  wxMenuItem *item = menuFile->FindItem( ID_RECENTTRACES );
  wxMenu *menu = item->GetSubMenu();
  wxMenuItemList& menuItems = menu->GetMenuItems();
  for ( wxMenuItemList::iterator menuIt = menuItems.begin(); menuIt != menuItems.end(); ++menuIt )
  {
    wxMenuItem *tmp = *menuIt;
    int currentId = tmp->GetId();
    if ( currentId == eventId )
    {
      traceLoadedBefore = true;
      tracePath = wxFileName( wxString::FromAscii( previousTraces->getFiles()[i].c_str() ) ).GetPath();
      DoLoadTrace( previousTraces->getFiles()[i] );
    }
    i++;
  }
}


void paraverMain::OnPreviousCFGsClick( wxCommandEvent& event )
{
  int eventId = event.GetId();
  int i = 0;
  
  wxMenuItem *item = menuFile->FindItem( ID_RECENTCFGS );
  wxMenu *menu = item->GetSubMenu();
  wxMenuItemList& menuItems = menu->GetMenuItems();
  
  for ( wxMenuItemList::iterator menuIt = menuItems.begin(); menuIt != menuItems.end(); ++menuIt )
  {
    wxMenuItem *tmp = *menuIt;
    int currentId = tmp->GetId();
    if ( currentId == eventId )
      DoLoadCFG( previousCFGs->getFiles()[i] );
    i++;
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_RECENTTRACES
 */

void paraverMain::OnRecenttracesUpdate( wxUpdateUIEvent& event )
{
  vector<string> v = previousTraces->getFiles();

  wxMenuItem *tmpItem = menuFile->FindItem(ID_RECENTTRACES );
  wxMenu *menuTraces = tmpItem->GetSubMenu();

  wxMenuItemList& menuItems = menuTraces->GetMenuItems();
  wxMenuItemList::iterator menuIt = menuItems.begin();

  for ( vector<string>::iterator it = v.begin(); it != v.end(); ++it )
  {
    if( menuIt == menuItems.end() )
    {
      wxMenuItem *newItem = new wxMenuItem( menuTraces, wxID_ANY, wxString::FromAscii( (*it).c_str() ) );
      menuTraces->Append( newItem );
      Connect( newItem->GetId(),
               wxEVT_COMMAND_MENU_SELECTED,
               (wxObjectEventFunction)&paraverMain::OnPreviousTracesClick );
    }
    else
    {
      wxMenuItem *tmp = *menuIt;
      tmp->SetItemLabel( wxString::FromAscii( (*it).c_str() ) );
      ++menuIt;
    }
  }
}
/*!
 * wxEVT_UPDATE_UI event handler for ID_MENULOADCFG
 */

void paraverMain::OnMenuloadcfgUpdate( wxUpdateUIEvent& event )
{
  if( currentTrace == -1 )
    event.Enable( false );
  else
    event.Enable( true );
      
  vector<string> v = previousCFGs->getFiles();

  wxMenuItem *tmpItem = menuFile->FindItem( ID_RECENTCFGS );
  wxMenu *menuCFGs = tmpItem->GetSubMenu();

  wxMenuItemList& menuItems = menuCFGs->GetMenuItems();
  wxMenuItemList::iterator menuIt = menuItems.begin();

  for ( vector<string>::iterator it = v.begin(); it != v.end(); ++it )
  {
    if( menuIt == menuItems.end() )
    {
      wxMenuItem *newItem = new wxMenuItem( menuCFGs, wxID_ANY, wxString::FromAscii( (*it).c_str() ) );
      menuCFGs->Append( newItem );
      Connect( newItem->GetId(),
               wxEVT_COMMAND_MENU_SELECTED,
              (wxObjectEventFunction)&paraverMain::OnPreviousCFGsClick );
    }
    else
    {
      wxMenuItem *tmp = *menuIt;
      tmp->SetItemLabel( wxString::FromAscii( (*it).c_str() ) );
      ++menuIt;
    }
  }
}

void progressFunction( ProgressController *progress )
{
  int p = (int)floor( ( progress->getCurrentProgress() * numeric_limits<int>::max() ) / progress->getEndLimit() );
  if( !paraverMain::dialogProgress->Update( p ) )
    progress->setStop( true );
//  app->Yield();
}

/*!
 * wxEVT_IDLE event handler for ID_PARAVERMAIN
 */

void paraverMain::OnIdle( wxIdleEvent& event )
{
  if( canServeSignal )
  {
    while( !loadFilesQueue.empty() )
    {
      string fileStr = loadFilesQueue.front();
      loadFilesQueue.pop();
      if( fileStr.substr( fileStr.length() - 3 ) == "cfg" )
      {
        if( loadedTraces.size() > 0 )
          DoLoadCFG( fileStr );
      }
      else
        DoLoadTrace( fileStr );
    }
  }
  
  if( wxTheApp->IsActive() )
  {
    int iTrace = 0;
    for( vector<Trace *>::iterator it = loadedTraces.begin(); it != loadedTraces.end(); ++it )
    {
      if( (*it)->getUnload() )
      {
        vector<Window *> windows;
        vector<Histogram *> histograms;
        
        LoadedWindows::getInstance()->getAll( *it, windows );
        LoadedWindows::getInstance()->getAll( *it, histograms );
        
        if( windows.begin() == windows.end() && histograms.begin() == histograms.end() )
        {
          if( currentTrace == iTrace ) currentTrace = -1;
          else --currentTrace;
          Trace *tmpTrace = *it;
          if( it == loadedTraces.begin() )
          {
            loadedTraces.erase( it );
            it = loadedTraces.begin();
          }
          else
          {
            vector<Trace *>::iterator tmpIt = it;
            --tmpIt;
            loadedTraces.erase( it );
            it = tmpIt;
          }
          delete tmpTrace;
          choiceWindowBrowser->DeletePage( iTrace + 1 );
          if( it == loadedTraces.end() )
            break;
        }
        else
          ++iTrace;
      }
      else
        ++iTrace;
    }
  }

#ifndef WIN32
  if ( sig1 || sig2 )
    OnSignal();
#endif
}


/*!
 * wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED event handler for ID_CHOICEWINBROWSER
 */

void paraverMain::OnChoicewinbrowserPageChanged( wxChoicebookEvent& event )
{
  int selPage = event.GetSelection();
  if( selPage == 0 )
    currentTrace = loadedTraces.size() - 1;
  else
    currentTrace = selPage - 1;
    
  wxTreeCtrl *tree = (wxTreeCtrl *) choiceWindowBrowser->GetCurrentPage();
  if( !tree->GetSelection().IsOk() )
    return;

  TreeBrowserItemData *item = (TreeBrowserItemData *) tree->GetItemData( tree->GetSelection() );
  if( item->getTimeline() != NULL )
  {
    currentWindow = item->getTimeline();
    currentTimeline = item->getTimeline()->GetMyWindow();
    currentHisto = NULL;
  }
  else if( item->getHistogram() != NULL )
  {
    currentWindow = item->getHistogram();
    currentHisto = item->getHistogram()->GetHistogram();
    currentTimeline = NULL;
  }
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUSAVECFG
 */

void paraverMain::OnMenusavecfgClick( wxCommandEvent& event )
{
  SaveOptions options;
  vector<Window *> timelines;
  vector<Histogram *> histograms;
  SaveConfigurationDialog saveDialog( this );
  
  saveDialog.SetOptions( options );
  LoadedWindows::getInstance()->getAll( timelines );
  LoadedWindows::getInstance()->getAll( histograms );
  saveDialog.SetTimelines( timelines );
  saveDialog.SetHistograms( histograms );
  
  raiseCurrentWindow = false;
  if( saveDialog.ShowModal() == wxID_OK )
  {
    if ( !CFGLoadedBefore )
      CFGPath =  wxString::FromAscii( paraverConfig->getGlobalCFGsPath().c_str() );

    timelines = saveDialog.GetTimelines();
    histograms = saveDialog.GetHistograms();
    options = saveDialog.GetOptions();
    wxFileDialog dialog( this, _( "Save Configuration" ), CFGPath, _( "" ),
      _("Paraver configuration file (*.cfg)|*.cfg" ),
      wxFD_SAVE|wxFD_OVERWRITE_PROMPT|wxFD_CHANGE_DIR );
    if( dialog.ShowModal() == wxID_OK )
    {
      wxString path = dialog.GetPath();
      CFGLoadedBefore = true;
      CFGPath = wxFileName( path ).GetPath();
      if( !path.EndsWith( _( ".cfg" ) ) )
        path += _( ".cfg" );
      CFGLoader::saveCFG( std::string( path.mb_str() ), options, timelines, histograms );
      previousCFGs->add( std::string( path.mb_str() ) );
    }
  }
  raiseCurrentWindow = true;
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_MENUSAVECFG
 */

void paraverMain::OnMenusavecfgUpdate( wxUpdateUIEvent& event )
{
  if( currentTimeline != NULL || currentHisto != NULL )
    event.Enable( true );
  else
    event.Enable( false );
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL
 */
wxSize paraverMain::defaultWindowSize = wxSize( 600, 115 );
int paraverMain::initialPosX = 0;
int paraverMain::initialPosY = 0;

void paraverMain::OnToolNewWindowClick( wxCommandEvent& event )
{
  // Create new window
  Window *newWindow = Window::create( localKernel, loadedTraces[ currentTrace ] );

  ++numNewWindows;
  wxString tmpName( _( "New window #" ) );
  tmpName << numNewWindows;
  newWindow->setName( std::string( tmpName.mb_str() ) );
  newWindow->setTimeUnit( loadedTraces[ currentTrace ]->getTimeUnit() );
  newWindow->addZoom( 0, loadedTraces[ currentTrace ]->getEndTime(),
                      0, newWindow->getWindowLevelObjects() - 1 );

  // Position window in screen
  newWindow->setWidth( defaultWindowSize.GetWidth() ); // magic numbers!
  newWindow->setHeight( defaultWindowSize.GetHeight() );
  
  if ( initialPosX != 0 )
    initialPosX += defaultTitleBarSize.GetHeight();
  else
    initialPosX += GetSize().GetWidth();

  newWindow->setPosX( initialPosX );
  newWindow->setPosY( initialPosY );
  initialPosY += defaultTitleBarSize.GetHeight();

  // Its default semantic
  for ( PRV_UINT16 windowLevel = TOPCOMPOSE1; windowLevel <= COMPOSECPU; windowLevel++ )
    newWindow->setLevelFunction( (TWindowLevel)windowLevel, "As Is" );

  string semanticFunction = loadedTraces[ currentTrace ]->getDefaultSemanticFunc( THREAD );
  if ( semanticFunction != "" )
    newWindow->setLevelFunction( THREAD, semanticFunction );
  else
    newWindow->setLevelFunction( THREAD, "State As Is" );

  // Build gtimeline and append new window to windows tree
  wxTreeCtrl *allTracesPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 );
  wxTreeCtrl *currentPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( currentTrace + 1 );

  BuildTree( this,
             allTracesPage, allTracesPage->GetRootItem(), 
             currentPage,   currentPage->GetRootItem(),
             newWindow );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TOOL
 */

void paraverMain::OnToolNewWindowUpdate( wxUpdateUIEvent& event )
{
  if ( loadedTraces.size() > 0 )
    tbarMain->EnableTool( ID_NEW_WINDOW, true );
  else
    tbarMain->EnableTool( ID_NEW_WINDOW, false );

  if ( currentTimeline != NULL )
    tbarMain->EnableTool( ID_NEW_WINDOW, true );
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEW_DERIVED_WINDOW
 */

void paraverMain::ShowDerivedDialog()
{
  DerivedTimelineDialog derivedDialog( this );
  vector<Window *> timelines;
  LoadedWindows::getInstance()->getAll( loadedTraces[ currentTrace ], timelines );

  ++numNewDerived;
  wxString tmpName( _( "New Derived Window #" ) );
  tmpName << numNewDerived;
  derivedDialog.SetTimelineName( std::string( tmpName.mb_str() ) );

  // Set timelines list
  derivedDialog.SetTimelines1( timelines );
  derivedDialog.SetTimelines2( timelines );

  // Set current window
  if ( beginDragWindow == NULL ||
      beginDragWindow->getTrace() == loadedTraces[ currentTrace ] )
    derivedDialog.SetCurrentWindow1( beginDragWindow );
  else 
    derivedDialog.SetCurrentWindow1( NULL );

  if ( endDragWindow == NULL ||
      endDragWindow->getTrace() == loadedTraces[ currentTrace ] )
    derivedDialog.SetCurrentWindow2( endDragWindow );
  else 
    derivedDialog.SetCurrentWindow2( NULL );

  raiseCurrentWindow = false;
  if( derivedDialog.ShowModal() == wxID_OK )
  {
    vector< Window * > selectedTimeline = derivedDialog.GetTimelines1();
    beginDragWindow = selectedTimeline[0]->clone();
    beginDragWindow->setPosX( GetNextPosX() );
    beginDragWindow->setPosY( GetNextPosY() );

    selectedTimeline.clear();
    selectedTimeline = derivedDialog.GetTimelines2();

//  if ( endDragWindow == beginDragWindow )
    endDragWindow = selectedTimeline[0]->clone();

    endDragWindow->setPosX( GetNextPosX() );
    endDragWindow->setPosY( GetNextPosY() );

    // Create new derived window
    Window *newWindow = Window::create( localKernel, beginDragWindow, endDragWindow );
    beginDragWindow->setChild( newWindow );
    endDragWindow->setChild( newWindow );
    newWindow->setPosX( GetNextPosX() );
    newWindow->setPosY( GetNextPosY() );

    newWindow->setName( derivedDialog.GetTimelineName() );
    newWindow->setTimeUnit( beginDragWindow->getTimeUnit() );
    newWindow->setWindowBeginTime( beginDragWindow->getWindowBeginTime() );
    newWindow->setWindowEndTime( beginDragWindow->getWindowEndTime() );
    newWindow->addZoom( beginDragWindow->getWindowBeginTime(),
                        beginDragWindow->getWindowEndTime(),
                        0, newWindow->getWindowLevelObjects() - 1 );
    newWindow->setShowChildrenWindow( false );

    // Size
    newWindow->setWidth( beginDragWindow->getWidth() ); // magic numbers!
    newWindow->setHeight( beginDragWindow->getHeight() );

    newWindow->setMaximumY( beginDragWindow->getMaximumY() );
    newWindow->setMinimumY( beginDragWindow->getMinimumY() );
    newWindow->setDrawCommLines( beginDragWindow->getDrawCommLines() );
    newWindow->setDrawFlags( beginDragWindow->getDrawFlags() );
    newWindow->setDrawFunctionLineColor( beginDragWindow->getDrawFunctionLineColor() );
    if( beginDragWindow->IsCodeColorSet() )
      newWindow->setCodeColorMode();
    else if( beginDragWindow->IsGradientColorSet() )
    {
      newWindow->setGradientColorMode();
      newWindow->allowOutOfScale( true );
      newWindow->allowOutliers( true );
    }
    else
    {
      newWindow->setGradientColorMode();
      newWindow->allowOutOfScale( false );
      newWindow->allowOutliers( true );
    }
    newWindow->setDrawModeObject( beginDragWindow->getDrawModeObject() );
    newWindow->setDrawModeTime( beginDragWindow->getDrawModeTime() );
    newWindow->getGradientColor().setGradientFunction(
      beginDragWindow->getGradientColor().getGradientFunction() );
    newWindow->setLevel( beginDragWindow->getLevel() );
    vector<bool> tmpSel;
    for( int level = APPLICATION; level <= THREAD; ++level )
    {
      tmpSel.clear();
      beginDragWindow->getSelectedRows( (TWindowLevel)level, tmpSel );
      newWindow->setSelectedRows( (TWindowLevel)level, tmpSel );
    }
    if( beginDragWindow->getTrace()->existResourceInfo() )
    {
      for( int level = NODE; level <= CPU; ++level )
      {
        tmpSel.clear();
        beginDragWindow->getSelectedRows( (TWindowLevel)level, tmpSel );
        newWindow->setSelectedRows( (TWindowLevel)level, tmpSel );
      }
    }
    
    // Semantic
    vector< string > auxCompose = derivedDialog.GetTopCompose1();
    newWindow->setLevelFunction( TOPCOMPOSE1, auxCompose[0] );
    auxCompose = derivedDialog.GetTopCompose2();
    newWindow->setLevelFunction( TOPCOMPOSE2, auxCompose[0] );

    newWindow->setFactor( 0, derivedDialog.GetFactorTimeline1() );
    newWindow->setFactor( 1, derivedDialog.GetFactorTimeline2() );

    TParamValue auxParam = derivedDialog.GetMinCompose1();
    if ( auxParam.size() > 0 )
      newWindow->setFunctionParam( TOPCOMPOSE1, 0, auxParam );
    auxParam.clear();

    auxParam =  derivedDialog.GetMaxCompose1();
    if ( auxParam.size() > 0 )
      newWindow->setFunctionParam( TOPCOMPOSE1, 1, auxParam );
    auxParam.clear();

    auxParam =  derivedDialog.GetMinCompose2();
    if ( auxParam.size() > 0 )
      newWindow->setFunctionParam( TOPCOMPOSE2, 0, auxParam );
    auxParam.clear();

    auxParam =  derivedDialog.GetMaxCompose2();
    if ( auxParam.size() > 0 )
      newWindow->setFunctionParam( TOPCOMPOSE2, 1, auxParam );
    auxParam.clear();

    vector< string > semanticDerivedFunction = derivedDialog.GetOperations();
    newWindow->setLevelFunction( DERIVED, semanticDerivedFunction[0] );

    // Build gtimeline and append new window to windows tree
    wxTreeCtrl *allTracesPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 );
    wxTreeCtrl *currentPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( currentTrace + 1 );

    BuildTree( this,
               allTracesPage, allTracesPage->GetRootItem(), 
               currentPage,   currentPage->GetRootItem(),
               newWindow );

    bool found;
    gTimeline *last = getGTimelineFromWindow( currentPage->GetRootItem(), newWindow, found );
    if ( found )
//    last->Raise();
      currentWindow = last;
  }
  raiseCurrentWindow = true;
}


void paraverMain::ShowHistogramDialog()
{
//  HistogramDialog histogramDialog( this );
  HistogramDialog histogramDialog( NULL );

  vector<TWindowID> timelines;
  LoadedWindows::getInstance()->getAll( timelines );

  histogramDialog.SetControlTimelines( timelines );
// esto lo hace internamente
//  histogramDialog.SetDataTimelines( timelines ); 

  vector< pair< TRecordTime, TRecordTime > > ranges;
  // Window Times
  ranges.push_back( make_pair( currentTimeline->getWindowBeginTime(), currentTimeline->getWindowEndTime() ) );
  // Trace Times
  ranges.push_back( make_pair( 0.0, currentTimeline->getTrace()->getEndTime() ) );

  histogramDialog.SetTimeRange( ranges );

  histogramDialog.TransferDataToWindow( currentTimeline );

  raiseCurrentWindow = false;
  if( histogramDialog.ShowModal() == wxID_OK )
  {
    vector< TWindowID > controlTimeline = histogramDialog.GetControlTimelines();
    vector< TWindowID > dataTimeline = histogramDialog.GetDataTimelines();
    vector< TWindowID > extraControlTimeline = histogramDialog.GetExtraControlTimelines();
    ranges.clear();
    ranges = histogramDialog.GetTimeRange();

    Histogram *newHistogram = Histogram::create( localKernel );

    ++numNewHistograms;
    wxString tmpName( _( "New Histogram #" ) );
    tmpName << numNewHistograms;
    newHistogram->setName( std::string( tmpName.mb_str() ) );
/*
   newHistogram->setPosX();
    newHistogram->setPosY();
    newHistogram->setWidth();
    newHistogram->setHeigth();
*/
    newHistogram->setShowWindow( true );
    newHistogram->setCalculateAll( true );
    newHistogram->setCurrentStat( "Time" );

    newHistogram->setWindowBeginTime( ranges[0].first );
    newHistogram->setWindowEndTime( ranges[0].second );

    newHistogram->setControlWindow( LoadedWindows::getInstance()->getWindow( controlTimeline[0] ) );

    if ( histogramDialog.GetControlTimelineAutofit() )
      newHistogram->setCompute2DScale( true );
    else
    {
      newHistogram->setCompute2DScale( false );

      newHistogram->setControlMin( histogramDialog.GetControlTimelineMin() );
      newHistogram->setControlMax( histogramDialog.GetControlTimelineMax() );
      newHistogram->setControlDelta( histogramDialog.GetControlTimelineDelta() );
    }

    newHistogram->setDataWindow( LoadedWindows::getInstance()->getWindow( dataTimeline[0] ) );

    if ( !extraControlTimeline.empty() )
    {
      newHistogram->setExtraControlWindow( LoadedWindows::getInstance()->getWindow( extraControlTimeline[0] ) );

      if ( histogramDialog.GetExtraControlTimelineAutofit() )
        newHistogram->setCompute3DScale( true );
      else
      {
        newHistogram->setCompute3DScale( false );

        newHistogram->setExtraControlMin( histogramDialog.GetExtraControlTimelineMin() );
        newHistogram->setExtraControlMax( histogramDialog.GetExtraControlTimelineMax() );
        newHistogram->setExtraControlDelta( histogramDialog.GetExtraControlTimelineDelta() );
      }
    }

    string composedName = newHistogram->getName() + " @ " +
                          newHistogram->getControlWindow()->getTrace()->getTraceName();
    gHistogram* tmpHisto = new gHistogram( this, wxID_ANY, wxString::FromAscii( composedName.c_str() ) );
    tmpHisto->SetHistogram( newHistogram );

    appendHistogram2Tree( tmpHisto );
    LoadedWindows::getInstance()->add( newHistogram );

    tmpHisto->SetClientSize( wxRect( newHistogram->getPosX(), newHistogram->getPosY(),
                                     newHistogram->getWidth(), newHistogram->getHeight() ) );
    if( newHistogram->getShowWindow() )
    {
      tmpHisto->Show();
    }
    tmpHisto->execute();
    
    currentWindow = (wxWindow *)tmpHisto;
  }
  raiseCurrentWindow = true;
}


void paraverMain::OnNewDerivedWindowClick( wxCommandEvent& event )
{
  ShowDerivedDialog();
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_NEW_DERIVED_WINDOW
 */

void paraverMain::OnNewDerivedWindowUpdate( wxUpdateUIEvent& event )
{

  if ( loadedTraces.size() > 0 && currentTrace > -1 )
  {
    vector<Window *> timelines;
    LoadedWindows::getInstance()->getAll( loadedTraces[ currentTrace ], timelines );

    event.Enable( ( timelines.size() > 0 ) && ( currentTimeline != NULL ) );
  }
  else
    event.Enable( false );

  if ( currentHisto != NULL )
    event.Enable( false );
}


void paraverMain::OnTreeBeginDrag( wxTreeEvent& event )
{
  wxTreeCtrl *tmpTree = static_cast<wxTreeCtrl *>( event.GetEventObject() );
  TreeBrowserItemData *itemData = static_cast<TreeBrowserItemData *>( tmpTree->GetItemData( event.GetItem() ) );

  beginDragWindow = NULL;

  if( gTimeline *timeline = itemData->getTimeline() )
  {
    beginDragWindow = timeline->GetMyWindow();
    event.Allow();
  }
}


int paraverMain::GetNextPosX()
{
  if ( initialPosX != 0 )
    initialPosX += defaultTitleBarSize.GetHeight();
  else
    initialPosX += GetSize().GetWidth();
    
  return initialPosX;
}


int paraverMain::GetNextPosY()
{
  initialPosY += defaultTitleBarSize.GetHeight();
  
  return initialPosY;
}


void paraverMain::OnTreeEndDrag( wxTreeEvent& event )
{
  if ( event.GetItem().IsOk() )
  {
    wxTreeCtrl *tmpTree = static_cast<wxTreeCtrl *>( event.GetEventObject() );
    TreeBrowserItemData *itemData = static_cast<TreeBrowserItemData *>( tmpTree->GetItemData( event.GetItem() ) );

    endDragWindow = NULL;

    if ( gTimeline *timeline = itemData->getTimeline())
    {
      endDragWindow = timeline->GetMyWindow();
      ShowDerivedDialog();
    }
  }
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEW_HISTOGRAM
 */

void paraverMain::OnNewHistogramClick( wxCommandEvent& event )
{
  ShowHistogramDialog();
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_NEW_HISTOGRAM
 */

void paraverMain::OnNewHistogramUpdate( wxUpdateUIEvent& event )
{
  if ( loadedTraces.size() > 0 )
  {
    vector<Window *> timelines;
    LoadedWindows::getInstance()->getAll( loadedTraces[ currentTrace ], timelines );
    tbarMain->EnableTool( ID_NEW_HISTOGRAM, ( timelines.size() > 0 ) && ( currentTimeline != NULL ) );
  }
  else
    tbarMain->EnableTool( ID_NEW_HISTOGRAM, false );
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_PREFERENCES
 */

void paraverMain::OnPreferencesClick( wxCommandEvent& event )
{
  PreferencesDialog preferences( this );

  // GLOBAL
  preferences.SetGlobalFillStateGaps( paraverConfig->getGlobalFillStateGaps() );
  preferences.SetTracesPath( paraverConfig->getGlobalTracesPath() );
  preferences.SetCfgsPath( paraverConfig->getGlobalCFGsPath() );
  preferences.SetTmpPath( paraverConfig->getGlobalTmpPath() );
  preferences.SetMaximumTraceSize( paraverConfig->getFiltersFilterTraceUpToMB() );
  preferences.SetSingleInstance( paraverConfig->getGlobalSingleInstance() );

  // TIMELINE

  preferences.SetTimelineNameFormatPrefix( paraverConfig->getTimelineDefaultName() );
  preferences.SetTimelineNameFormatFull( paraverConfig->getTimelineNameFormat() );
  //preferences.GetTimelineDefaultCFG( paraverConfig->setTimelineDefaultCFG() );
  preferences.SetTimelineEventLines( paraverConfig->getTimelineViewEventsLines() );
  preferences.SetTimelineCommunicationLines( paraverConfig->getTimelineViewCommunicationsLines() );
  preferences.SetTimelineFunctionAsColor( paraverConfig->getTimelineViewFunctionAsColor() );
  preferences.SetTimelineColor( ( PRV_UINT32 )paraverConfig->getTimelineColor() );
  preferences.SetTimelineGradientFunction( ( PRV_UINT32 )paraverConfig->getTimelineGradientFunction() );
  preferences.SetTimelineDrawmodeTime( ( PRV_UINT32 )paraverConfig->getTimelineDrawmodeTime() );
  preferences.SetTimelineDrawmodeObjects( ( PRV_UINT32 )paraverConfig->getTimelineDrawmodeObjects() );
  preferences.SetTimelinePixelSize( ( PRV_UINT32 )paraverConfig->getTimelinePixelSize() );
  preferences.SetTimelineWWPrecision( paraverConfig->getTimelinePrecision() );
  preferences.SetTimelineWWSemantic( paraverConfig->getTimelineWhatWhereSemantic() );
  preferences.SetTimelineWWEvents( paraverConfig->getTimelineWhatWhereEvents() );
  preferences.SetTimelineWWCommunications( paraverConfig->getTimelineWhatWhereCommunications() );
  preferences.SetTimelineWWPreviousNext( paraverConfig->getTimelineWhatWherePreviousNext() );
  preferences.SetTimelineWWText( paraverConfig->getTimelineWhatWhereText() );
  preferences.SetTimelineSaveImageFormat( ( PRV_UINT32 )paraverConfig->getTimelineSaveImageFormat() );
  preferences.SetTimelineSaveTextFormat( ( PRV_UINT32 )paraverConfig->getTimelineSaveTextFormat() );

  // HISTOGRAM
  //preferences.SetHistogramNameFormatPrefix( paraverConfig->getHistogramDefaultName() );
  preferences.SetHistogramNameFormatPrefix( std::string( "New Histogram # %N" ) );
  //preferences.SetHistogramNameFormatFull( paraverConfig->getHistogramNameFormat() );
  preferences.SetHistogramNameFormatFull( std::string( "%P @ %T" ) );
  preferences.SetHistogramZoom( paraverConfig->getHistogramViewZoom() );
  preferences.SetHistogramHorizontal( paraverConfig->getHistogramViewHorizontal() );
  preferences.SetHistogramHideEmpty( !paraverConfig->getHistogramViewEmptyColumns() );
  preferences.SetHistogramShowGradient( paraverConfig->getHistogramViewGradientColors() );
  preferences.SetHistogramLabelsColor( paraverConfig->getHistogramViewFirstRowColored() );
  preferences.SetHistogramGradientFunction( paraverConfig->getHistogramGradientFunction() );
  preferences.SetHistogramDrawmodeSemantic( ( PRV_UINT32 ) paraverConfig->getHistogramDrawmodeSemantic() );
  preferences.SetHistogramDrawmodeObjects( ( PRV_UINT32 ) paraverConfig->getHistogramDrawmodeObjects() );
  preferences.SetHistogramScientificNotation( paraverConfig->getHistogramScientificNotation() );
  preferences.SetHistogramThousandSeparator( paraverConfig->getHistogramThousandSep() );
  preferences.SetHistogramShowUnits( paraverConfig->getHistogramShowUnits() );
  preferences.SetHistogramPrecision( paraverConfig->getHistogramPrecision() );
  preferences.SetHistogramAutofitControlScale( paraverConfig->getHistogramAutofitControlScale() );
  preferences.SetHistogramAutofit3DScale( paraverConfig->getHistogramAutofitThirdDimensionScale() );
  preferences.SetHistogramAutofitDataGradient( paraverConfig->getHistogramAutofitDataGradient() );
  preferences.SetHistogramNumColumns( paraverConfig->getHistogramNumColumns() );
//  preferences.SetHistogramMaxNumColumns( 400 ); // TO IMPLEMENT
//  preferences.SetHistogramMaxPrecision( 20 ); // TO IMPLEMENT
  preferences.SetHistogramSaveImageFormat( ( PRV_UINT32 )paraverConfig->getHistogramSaveImageFormat() );
  preferences.SetHistogramSaveTextFormat( ( PRV_UINT32 )paraverConfig->getHistogramSaveTextFormat() );

  // COLORS
  preferences.SetTimelineColourBackground( paraverConfig->getColorsTimelineBackground() );
  preferences.SetTimelineColourAxis( paraverConfig->getColorsTimelineAxis() );
  preferences.SetColorUseZero( paraverConfig->getColorsTimelineUseZero() );
  preferences.SetTimelineColourZero( paraverConfig->getColorsTimelineColorZero() );
  preferences.SetTimelineColourLogical( paraverConfig->getColorsTimelineLogicalCommunications() );
  preferences.SetTimelineColourPhysical( paraverConfig->getColorsTimelinePhysicalCommunications() );

  preferences.SetGradientColourBegin( paraverConfig->getColorsBeginGradient() );
  preferences.SetGradientColourEnd( paraverConfig->getColorsEndGradient() );
  preferences.SetGradientColourLow( paraverConfig->getColorsLowGradient() );
  preferences.SetGradientColourTop( paraverConfig->getColorsTopGradient() );

  // FILTER

  preferences.TransferDataToWindow();

  raiseCurrentWindow = true;
  if ( preferences.ShowModal() == wxID_OK )
  {
    preferences.TransferDataFromWindow();

    // Apply Preferences

    // GLOBAL
    paraverConfig->setGlobalFillStateGaps( preferences.GetGlobalFillStateGaps() );
    paraverConfig->setGlobalTracesPath( preferences.GetTracesPath() );
    paraverConfig->setGlobalCFGsPath( preferences.GetCfgsPath() );
    paraverConfig->setGlobalTmpPath( preferences.GetTmpPath() );
    paraverConfig->setFiltersFilterTraceUpToMB( (float)preferences.GetMaximumTraceSize() );
    paraverConfig->setGlobalSingleInstance( preferences.GetSingleInstance() );

    // TIMELINE
    paraverConfig->setTimelineDefaultName( preferences.GetTimelineNameFormatPrefix() );
    paraverConfig->setTimelineNameFormat( preferences.GetTimelineNameFormatFull() );
    //paraverConfig->setTimelineDefaultCFG( preferences.GetTimelineDefaultCFG() );
    paraverConfig->setTimelinePrecision( preferences.GetTimelineWWPrecision() );
    paraverConfig->setTimelineViewEventsLines( preferences.GetTimelineEventLines() );
    paraverConfig->setTimelineViewCommunicationsLines( preferences.GetTimelineCommunicationLines() );
    paraverConfig->setTimelineViewFunctionAsColor( preferences.GetTimelineFunctionAsColor() );
    paraverConfig->setTimelineColor( (SemanticColor::TColorFunction)preferences.GetTimelineColor() );
    paraverConfig->setTimelineGradientFunction( (GradientColor::TGradientFunction)preferences.GetTimelineGradientFunction() );
    paraverConfig->setTimelineDrawmodeTime( (DrawModeMethod)preferences.GetTimelineDrawmodeTime() );
    paraverConfig->setTimelineDrawmodeObjects( (DrawModeMethod)preferences.GetTimelineDrawmodeObjects() );
    paraverConfig->setTimelinePixelSize( preferences.GetTimelinePixelSize() );
    paraverConfig->setTimelineWhatWhereSemantic( preferences.GetTimelineWWSemantic() );
    paraverConfig->setTimelineWhatWhereEvents( preferences.GetTimelineWWEvents() );
    paraverConfig->setTimelineWhatWhereCommunications( preferences.GetTimelineWWCommunications() );
    paraverConfig->setTimelineWhatWherePreviousNext( preferences.GetTimelineWWPreviousNext() );
    paraverConfig->setTimelineWhatWhereText( preferences.GetTimelineWWText() );
    paraverConfig->setTimelineSaveImageFormat( (ParaverConfig::TImageFormat)preferences.GetTimelineSaveImageFormat() );
    paraverConfig->setTimelineSaveTextFormat( (ParaverConfig::TTextFormat)preferences.GetTimelineSaveTextFormat() );

    // HISTOGRAM
    //paraverConfig->setHistogramDefaultName( preferences.GetHistogramNameFormatPrefix() );
    //paraverConfig->setHistogramNameFormat( preferences.GetHistogramNameFormatFull() );
    paraverConfig->setHistogramViewZoom( preferences.GetHistogramZoom() );
    paraverConfig->setHistogramViewHorizontal( preferences.GetHistogramHorizontal() );
    paraverConfig->setHistogramViewEmptyColumns( !preferences.GetHistogramHideEmpty() );
    paraverConfig->setHistogramViewGradientColors( preferences.GetHistogramShowGradient() );
    paraverConfig->setHistogramViewFirstRowColored( preferences.GetHistogramLabelsColor() );
    paraverConfig->setHistogramGradientFunction( (GradientColor::TGradientFunction)preferences.GetHistogramGradientFunction() );
    paraverConfig->setHistogramDrawmodeSemantic( ( DrawModeMethod ) preferences.GetHistogramDrawmodeSemantic() );
    paraverConfig->setHistogramDrawmodeObjects( ( DrawModeMethod ) preferences.GetHistogramDrawmodeObjects() );
    paraverConfig->setHistogramScientificNotation( preferences.GetHistogramScientificNotation() );
    paraverConfig->setHistogramThousandSep( preferences.GetHistogramThousandSeparator() );
    paraverConfig->setHistogramShowUnits( preferences.GetHistogramShowUnits() );
    paraverConfig->setHistogramPrecision( preferences.GetHistogramPrecision() );
    paraverConfig->setHistogramAutofitControlScale( preferences.GetHistogramAutofitControlScale() );
    paraverConfig->setHistogramAutofitThirdDimensionScale( preferences.GetHistogramAutofit3DScale() );
    paraverConfig->setHistogramAutofitDataGradient( preferences.GetHistogramAutofitDataGradient() );
    paraverConfig->setHistogramNumColumns( preferences.GetHistogramNumColumns() );
    paraverConfig->setHistogramSaveImageFormat( ( ParaverConfig::TImageFormat ) preferences.GetHistogramSaveImageFormat() );
    paraverConfig->setHistogramSaveTextFormat( ( ParaverConfig::TTextFormat ) preferences.GetHistogramSaveTextFormat() );

    // COLORS 
    paraverConfig->setColorsTimelineBackground( preferences.GetTimelineColourBackground() );
    paraverConfig->setColorsTimelineAxis( preferences.GetTimelineColourAxis() );
    paraverConfig->setColorsTimelineUseZero( preferences.GetColorUseZero() );
    paraverConfig->setColorsTimelineColorZero( preferences.GetTimelineColourZero() );
    paraverConfig->setColorsTimelineLogicalCommunications( preferences.GetTimelineColourLogical() );
    paraverConfig->setColorsTimelinePhysicalCommunications( preferences.GetTimelineColourPhysical() );

    paraverConfig->setColorsBeginGradient( preferences.GetGradientColourBegin() );
    paraverConfig->setColorsEndGradient( preferences.GetGradientColourEnd() );
    paraverConfig->setColorsLowGradient( preferences.GetGradientColourLow() );
    paraverConfig->setColorsTopGradient( preferences.GetGradientColourTop() );

    // FILTER

    // Save Preferences to File
    paraverConfig->writeParaverConfigFile();
  }
  raiseCurrentWindow = false;
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_PREFERENCES
 */

void paraverMain::OnPreferencesUpdate( wxUpdateUIEvent& event )
{
////@begin wxEVT_UPDATE_UI event handler for ID_PREFERENCES in paraverMain.
  // Before editing this code, remove the block markers.
  event.Skip();
////@end wxEVT_UPDATE_UI event handler for ID_PREFERENCES in paraverMain. 
}

void paraverMain::selectTrace( Trace *trace )
{
  size_t currentTrace;
  for ( currentTrace = 0; currentTrace < loadedTraces.size(); ++currentTrace )
    if ( loadedTraces[ currentTrace ] == trace )
      break;

  int currentPage  = choiceWindowBrowser->GetSelection();

  if ( currentPage != 0 )
    choiceWindowBrowser->SetSelection( currentTrace + 1 );

  SetCurrentTrace( currentTrace );
}


PRV_UINT16 paraverMain::getTracePosition( Trace *trace )
{
  PRV_UINT16 currentTrace;

  for ( currentTrace = 0; currentTrace < loadedTraces.size(); ++currentTrace )
    if ( loadedTraces[ currentTrace ] == trace )
      break;

  return currentTrace;
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOLDELETE
 */

void paraverMain::OnTooldeleteClick( wxCommandEvent& event )
{
  if( currentHisto != NULL )
    currentHisto->setDestroy( true );
  if( currentTimeline != NULL )
  {
    if( currentTimeline->getChild() == NULL )
    {
      if( !currentTimeline->getUsedByHistogram() )
        currentTimeline->setDestroy( true );
      else
        wxMessageBox( _( "Cannot delete windows used by histograms." ),
                      _( "Paraver information" ),
                      wxOK | wxICON_INFORMATION );
    }
    else
      wxMessageBox( _( "Cannot delete parent windows. Delete first derived window" ),
                    _( "Paraver information" ),
                    wxOK | wxICON_INFORMATION );
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TOOLDELETE
 */

void paraverMain::OnTooldeleteUpdate( wxUpdateUIEvent& event )
{
  if( currentTimeline != NULL || currentHisto != NULL )
    event.Enable( true );
  else
    event.Enable( false );
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_UNLOADTRACE
 */

void paraverMain::OnUnloadtraceClick( wxCommandEvent& event )
{
  wxArrayString choices;

  for( vector<Trace *>::iterator it = loadedTraces.begin(); it != loadedTraces.end(); ++it )
    choices.Add( wxString::FromAscii( (*it)->getTraceNameNumbered().c_str() ) );
  wxSingleChoiceDialog dialog( this, _("Select the trace to unload:"), _("Unload Trace"), choices );
  
  raiseCurrentWindow = false;
#ifndef WIN32
  if( dialog.ShowModal() == wxID_OK )
#else
  dialog.ShowModal();
#endif
    UnloadTrace( dialog.GetSelection() );

  raiseCurrentWindow = true;
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_UNLOADTRACE
 */

void paraverMain::OnUnloadtraceUpdate( wxUpdateUIEvent& event )
{
  event.Enable( loadedTraces.begin() != loadedTraces.end() );
}


void paraverMain::UnloadTrace( int whichTrace )
{
  vector<Window *> windows;
  vector<Histogram *> histograms;
  
  LoadedWindows::getInstance()->getAll( loadedTraces[ whichTrace ], windows );
  LoadedWindows::getInstance()->getAll( loadedTraces[ whichTrace ], histograms );
  
  for( vector<Window *>::iterator it = windows.begin(); it != windows.end(); ++it )
  {
    (*it)->setShowWindow( false );
    if( (*it)->getChild() == NULL )
      (*it)->setDestroy( true );
  }

  for( vector<Histogram *>::iterator it = histograms.begin(); it != histograms.end(); ++it )
  {
    (*it)->clearControlWindow();
    (*it)->clearDataWindow();
    (*it)->clearExtraControlWindow();
    (*it)->setShowWindow( false );
    (*it)->setDestroy( true );
  }
  
  loadedTraces[ whichTrace ]->setUnload( true );
}

void paraverMain::clearProperties()
{
  windowProperties->Clear();
}

void paraverMain::OnActivate( wxActivateEvent& event )
{
#if 0
  if( event.GetActive() )
  {
    for( set<wxWindow *>::iterator it = activeWindows.begin();
        it != activeWindows.end(); ++it )
      (*it)->Raise();
    Raise();
  }
#endif
  event.Skip();
}

void paraverMain::addActiveWindow( wxWindow *window )
{
  activeWindows.insert( window );
}

void paraverMain::removeActiveWindow( wxWindow *window )
{
  activeWindows.erase( window );
}

void paraverMain::PrepareToExit()
{
  vector<Histogram *> histograms;
  LoadedWindows::getInstance()->getAll( histograms );
  
  for( vector<Histogram *>::iterator it = histograms.begin(); it != histograms.end(); ++it )
  {
    (*it)->clearControlWindow();
    (*it)->clearDataWindow();
    (*it)->clearExtraControlWindow();
  }
}

/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_PARAVERMAIN
 */

void paraverMain::OnCloseWindow( wxCloseEvent& event )
{
  PrepareToExit();
  Destroy();
}


#ifndef WIN32

bool paraverMain::matchTraceNames( const string &fileName1,  // path1/name1 or name1
                                   const string &traceName1, // name1
                                   const string &fileName2 ) // path2/name2 or name2
{
// not working for Windows !!
//  string::size_type position = auxLine.find_last_of( '\\' );
  string::size_type position = fileName2.find_last_of( '/' );

  string traceName2;
  if ( position != string::npos )
    traceName2 = fileName2.substr( position + 1 );
  else
    traceName2 = fileName2;

  return ((( fileName1 != traceName1 ) && ( fileName1 == fileName2 )) ||
          (( fileName1 == traceName1 ) && ( traceName1 == traceName2 )));
}


void paraverMain::OnSignal( )
{
  // PRECOND: sig1 XOR sig2 == true;
  if( !canServeSignal )
    return;

  bool mySig1 = sig1;

  // Clear global variables
  sig1 = false;
  sig2 = false;

  // Is some trace loading now?
  if ( paraverMain::dialogProgress != NULL )
  {
    // Restore global variables
    sig1 = mySig1;
    sig2 = !mySig1;
    return;
  }

  // Does paraload.sig exists?
  string path = getenv("HOME");
  string filename = "paraload.sig";
  string fullName = path + "/" + filename;
  ifstream paraloadFile;
  paraloadFile.open( fullName.c_str() );
  if ( !paraloadFile  )
  {
    wxMessageDialog message( this,
                             _( "File ./paraload.sig not found" ),
                             _( "Signal Handler Manager" ),
                             wxOK | wxICON_EXCLAMATION );
    raiseCurrentWindow = false;
    message.ShowModal();
    raiseCurrentWindow = true;
    return;
  }

  string::size_type position;

  // Read all lines in paraload.sig
  vector<string> lines;
  string auxLine;
  while ( !paraloadFile.eof() )
  {
    // get line and trim its leading or trailing spaces or tabs
    std::getline( paraloadFile, auxLine );
    position = auxLine.find_last_not_of(" \t");
    if ( position != string::npos   )
    {
      auxLine.erase( position + 1 );
      position = auxLine.find_first_not_of(" \t");
      if ( position != string::npos )
      {
        auxLine.erase( 0, position );
      }
    }
    else
    {
      auxLine.clear();
    }
    
    // Is it useful? Save it!
    if ( auxLine.length() > 0 && auxLine[0] != '#' )
    {
      lines.push_back( auxLine );
      auxLine.clear();
    }
  }
  
  // POSTCOND:
  // lines[0] must contain paraver cfg
  // lines[1] must contain time range to set the window
  // if exists, lines[2] must contain a trace to load

  if ( lines.size() > 2 ) // any trace?
  {
    bool found = false;
    size_t current = currentTrace;
  
    // Is that trace loaded? First, try with current!
    if ( loadedTraces.size() > 0 )
    {
      found = matchTraceNames( loadedTraces[ current ]->getFileName(),
                               loadedTraces[ current ]->getTraceName(),
                               lines[ 2 ] );
    }
    
    if ( !found )
    {
      // then continue with all the list
      for ( current = 0; current < loadedTraces.size(); ++current )
      {
        found = matchTraceNames( loadedTraces[ current ]->getFileName(),
                                 loadedTraces[ current ]->getTraceName(),
                                 lines[ 2 ] );
        if ( found )
        {
          currentTrace = current; // select it !!
          break;
        }
      }
    }

    if ( current == -1 || !found ) // No trace loaded or not found!!
      DoLoadTrace( lines[ 2 ] );
  }

  // Anyway, Am I able to load any cfg?
  if ( loadedTraces.size() == 0 )
  {
    wxMessageDialog message( this,
                             _( "No trace loaded" ),
                             _( "Signal Handler Manager" ),
                             wxOK | wxICON_EXCLAMATION );
    raiseCurrentWindow = false;
    message.ShowModal();
    raiseCurrentWindow = true;
    return;
  }
  
  // Read cfg location
  string cfgFullName = lines[ 0 ];

  // Code only for sigusr1: load cfg
  if( mySig1 )
    DoLoadCFG( cfgFullName );

  // Code for both sigusr1 and sigusr2: zoom
  Window* myCurrentTimeline;
  vector<Window *> timelines;
  
  LoadedWindows::getInstance()->getAll( loadedTraces[ currentTrace ], timelines );
  if ( currentTimeline == NULL )
  {
    if ( timelines.size() > 0 )
    {
      myCurrentTimeline = timelines[ timelines.size() - 1  ];
    }
    else
    {
      wxMessageDialog message( this, _( "No timeline created" ), _( "Signal Handler Manager" ), wxOK | wxICON_EXCLAMATION );
      raiseCurrentWindow = false;
      message.ShowModal();
      raiseCurrentWindow = true;
      return;
    }
  }
  else
    myCurrentTimeline = currentTimeline;

  // Read paraload.sig second line: times
  string times = lines[1];

  size_t pos = times.find(":");
  if ( pos == string::npos )
  {
    wxMessageDialog message( this, _( "Missing times separator ':' in file paraload.sig" ), _( "Signal Handler Manager" ), wxOK | wxICON_EXCLAMATION );
    raiseCurrentWindow = false;
    message.ShowModal();
    raiseCurrentWindow = true;
    return;
  }
  else
  {
    // Get begin time
    string time = times.substr( 0, pos );
    stringstream aux( time );
    double auxt1;
    aux >> auxt1;

    if ( auxt1 == -1.0 )
      auxt1 = 0.0;

    // Get end time
    string time2 = times.substr( ++pos,times.size() - 1  );
    stringstream aux2( time2 );
    double auxt2;
    aux2 >> auxt2;

    if ( auxt2 == -1.0 )
      auxt2 = myCurrentTimeline->getTrace()->getEndTime();

    // Zoom
    myCurrentTimeline->setWindowBeginTime( auxt1 );
    myCurrentTimeline->setWindowEndTime( auxt2 );
    myCurrentTimeline->addZoom( auxt1, auxt2 );

    myCurrentTimeline->setRedraw( true );
    myCurrentTimeline->setChanged( true );
  }

  // Refresh
  choiceWindowBrowser->UpdateWindowUI();
}
#endif


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_ABOUT
 */

void paraverMain::OnAboutClick( wxCommandEvent& event )
{
  wxAboutDialogInfo info;
  wxArrayString developers;
  wxString description;
  
  info.SetName( _("wxParaver") );
  info.SetVersion( _( "3.99 (Build " ) + wxString() << _( __DATE__ ) << _( ")" ) );
  description << _( "\nwxParaver is a graphical displaying tool developed at BSC :\n" );
  description << _( "Barcelona Supercomputing Center.\n\n" );
  description << _( "wxParaver allows the programmer to examine graphically a " );
  description << _( "tracefile, with the possibility to choose different filters in " );
  description << _( "order to select what is displayed.\n" );
  description << _( "\n(paraver@bsc.es)\n" );
  info.SetDescription( description );
  developers.Add( _("Eloy Martinez Hortelano (eloy.martinez@bsc.es)") );
  developers.Add( _("Pedro Antonio Gonzalez Navarro (pedro.gonzalez@bsc.es)") );
  info.SetDevelopers( developers );
  info.SetWebSite( _("http://www.bsc.es/paraver") );
  //info.SetCopyright(_T(""));

  wxAboutBox(info);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TOOL_CUT_TRACE
 */

void paraverMain::OnToolCutTraceUpdate( wxUpdateUIEvent& event )
{
  event.Enable( true );
}


void paraverMain::enqueueFile( string whichFile )
{
  loadFilesQueue.push( whichFile );
}


#ifdef WIN32
void paraverMain::OnKeyCopy()
{
  wxTreeCtrl *tree = (wxTreeCtrl *) choiceWindowBrowser->GetCurrentPage();
  if( !tree->GetSelection().IsOk() )
    return;
  TreeBrowserItemData *item = (TreeBrowserItemData *) tree->GetItemData( tree->GetSelection() );
  if( item->getTimeline() != NULL )
    item->getTimeline()->OnPopUpCopy();
  else if( item->getHistogram() != NULL )
    item->getHistogram()->OnPopUpCopy();
}


void paraverMain::OnKeyPaste()
{
  wxTreeCtrl *tree = (wxTreeCtrl *) choiceWindowBrowser->GetCurrentPage();
  if( !tree->GetSelection().IsOk() )
    return;
  TreeBrowserItemData *item = (TreeBrowserItemData *) tree->GetItemData( tree->GetSelection() );
  if( item->getTimeline() != NULL )
    item->getTimeline()->OnPopUpPasteSpecial();
  else if( item->getHistogram() != NULL )
    item->getHistogram()->OnPopUpPasteSpecial();
}
#endif


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_CUT_TRACE
 */
void paraverMain::OnToolCutTraceClick( wxCommandEvent& event )
{
  if( currentTrace == -1 )
    ShowCutTraceWindow( "", false ); // Default XML dir in both branches of the if?
  else
    ShowCutTraceWindow( loadedTraces[ currentTrace ]->getFileName(), false );
}


string paraverMain::DoLoadFilteredTrace( string traceFileName,
                                         string traceFilePath,
                                         TraceOptions *traceOptions,
                                         vector< int > &filterToolOrder )
{
  TraceCutter *traceCutter;
  TraceFilter *traceFilter;
  TraceSoftwareCounters *traceSoftwareCounters;
  //TraceCommunicationsFusion *traceCommunicationsFusion;

  string tmpTraceIn, tmpTraceOut;
  char tmpNameIn[1024], tmpNameOut[1024], tmpPathOut[1024], tmpPathOutBackup[1024];
  string strOutputFile;
  vector< string > tmpFiles;

  ProgressController *progress = ProgressController::create( localKernel );
  progress->setHandler( progressFunction );

  if( paraverMain::dialogProgress == NULL )
    paraverMain::dialogProgress = new wxProgressDialog( wxT("Processing trace..."),
                                                        wxT(""),
                                                        numeric_limits<int>::max(),
                                                        this,
                                                        wxPD_AUTO_HIDE|wxPD_APP_MODAL|wxPD_ELAPSED_TIME|wxPD_ESTIMATED_TIME|wxPD_REMAINING_TIME );

  // Concatenate Filter Utilities
  strcpy( tmpNameOut, (char *)traceFileName.c_str() );
  strcpy( tmpPathOut, (char *)traceFilePath.c_str() );
  strcpy( tmpPathOutBackup, (char *)traceFilePath.c_str() );

  for( PRV_UINT16 i = 0; i < filterToolOrder.size(); ++i )
  {
    strcpy( tmpNameIn, tmpNameOut );

    // Please change this to:
    // tmpNameOut = localKernel->getNewTraceName( tmpNameIn, tmpPathOut, filterToolOrder[ i ] );
    localKernel->getNewTraceName( tmpNameIn, tmpPathOut, filterToolOrder[ i ] );
    strcpy( tmpNameOut, tmpPathOut );
    strcpy( tmpPathOut, tmpPathOutBackup );

    paraverMain::dialogProgress->Pulse( wxString::FromAscii( tmpNameOut ) );
    paraverMain::dialogProgress->Fit();
    paraverMain::dialogProgress->Show();

    switch( filterToolOrder[i] )
    {
      case INC_CHOP_COUNTER:
        traceCutter = localKernel->newTraceCutter( tmpNameIn,
                                                   tmpNameOut,
                                                   traceOptions,
                                                   progress );
        localKernel->copyPCF( tmpNameIn, tmpNameOut );
        break;

      case INC_FILTER_COUNTER:
        traceFilter = localKernel->newTraceFilter( tmpNameIn,
                                                   tmpNameOut,
                                                   traceOptions,
                                                   progress );
        localKernel->copyPCF( tmpNameIn, tmpNameOut );
        break;

      case INC_SC_COUNTER:
        traceSoftwareCounters = localKernel->newTraceSoftwareCounters( tmpNameIn,
                                                                    tmpNameOut,
                                                                    traceOptions,
                                                                    progress );
        // traceSoftwareCounters modifies the pcf, don't copy here!
        break;

      default:
        break;
    }

    localKernel->copyROW( tmpNameIn, tmpNameOut );
    tmpFiles.push_back( tmpNameOut );
  }

  // Delete intermediate files
  char *pcfName, *rowName;
  for( PRV_UINT16 i = 0; i < tmpFiles.size() - 1; ++i )
  {
    pcfName = localKernel->composeName( (char *)tmpFiles[ i ].c_str(), "pcf" );
    rowName = localKernel->composeName( (char *)tmpFiles[ i ].c_str(), "row" );
    remove( tmpFiles[ i ].c_str() );
    remove( pcfName );
    remove( rowName );
  }

  // Delete utilities
  // delete traceOptions;
  for( PRV_UINT16 i = 0; i < filterToolOrder.size(); ++i )
  {
    switch( filterToolOrder[i] )
    {
      case INC_CHOP_COUNTER:
        //delete *traceCutter;
        break;
      case INC_FILTER_COUNTER:
        //delete *traceFilter;
        break;
      case INC_SC_COUNTER:
        //delete *traceSoftwareCounters;
        break;
    }
  }

  paraverMain::dialogProgress->Show( false );
  delete paraverMain::dialogProgress;
  paraverMain::dialogProgress = NULL;
  delete progress;

  return string( tmpNameOut );
}

void paraverMain::ShowCutTraceWindow( const string& filename, bool loadTrace )
{
  TraceOptions *traceOptions = TraceOptions::create( localKernel );

  CutFilterDialog cutFilterDialog( this );  
  cutFilterDialog.SetLocalKernel( localKernel );
  cutFilterDialog.SetTraceOptions( traceOptions->getConcrete() );

  if( filename == "" )
  {
    if ( !traceLoadedBefore )
#ifdef WIN32
      cutFilterDialog.SetNameSourceTrace( paraverConfig->getGlobalTracesPath() + "\\" );
    else
      cutFilterDialog.SetNameSourceTrace( std::string( tracePath.mb_str() ) + "\\" );
#else
      cutFilterDialog.SetNameSourceTrace( paraverConfig->getGlobalTracesPath() + "/" );
    else
      cutFilterDialog.SetNameSourceTrace( std::string( tracePath.mb_str() ) + "/" );
#endif
  }
  else
  {
    cutFilterDialog.SetNameSourceTrace( filename );
    cutFilterDialog.SetLoadResultingTrace( loadTrace );
  }
  
  if( cutFilterDialog.ShowModal() == wxID_OK )
  {
    vector< int > filterToolOrder = cutFilterDialog.GetFilterToolOrder();

    string traceFileName = cutFilterDialog.GetNameSourceTrace();
    string traceFileOutputPath = cutFilterDialog.GetPathOutputTrace();
    string resultingTrace = DoLoadFilteredTrace( traceFileName,
                                                 traceFileOutputPath,
                                                 cutFilterDialog.GetTraceOptions(),
                                                 filterToolOrder );

    if ( cutFilterDialog.GetLoadResultingTrace() )
      DoLoadTrace( resultingTrace );
  }
  
  cutFilterDialog.MakeModal( false );
}


/*!
 * wxEVT_ICONIZE event handler for ID_PARAVERMAIN
 */

void paraverMain::OnIconize( wxIconizeEvent& event )
{
  for( unsigned int iPage = 0; iPage < choiceWindowBrowser->GetPageCount(); iPage++ )
  {
    wxTreeCtrl *currentTree = (wxTreeCtrl *) choiceWindowBrowser->GetPage( iPage );
    wxTreeItemId root = currentTree->GetRootItem();
    iconizeWindows( currentTree, root, event.Iconized() );
  }
}


/*!
 * wxEVT_SIZE event handler for ID_PARAVERMAIN
 */

void paraverMain::OnSize( wxSizeEvent& event )
{
  paraverConfig->setMainWindowWidth( event.GetSize().GetWidth() );
  paraverConfig->setMainWindowHeight( event.GetSize().GetHeight() );
  
  event.Skip();
}

