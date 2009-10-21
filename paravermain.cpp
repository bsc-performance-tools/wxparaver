/////////////////////////////////////////////////////////////////////////////
// Name:        paravermain.cpp
// Purpose:     
// Author:      Eloy Martinez
// Modified by: 
// Created:     Wed 25 Jun 2008 15:58:35 CEST
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

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

#include "wx/imaglist.h"
#include "wx/tipdlg.h"

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

////@begin XPM images
#include "new_window.xpm"
#include "new_derived_window.xpm"
#include "new_histogram.xpm"
#include "delete.xpm"
////@end XPM images

#include "table.xpm"
#include "timeline.xpm"
/*!
 * paraverMain type definition
 */

IMPLEMENT_CLASS( paraverMain, wxFrame )


/*!
 * paraverMain event table definition
 */

BEGIN_EVENT_TABLE( paraverMain, wxFrame )

////@begin paraverMain event table entries
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

  EVT_MENU( ID_NEW_WINDOW, paraverMain::OnToolNewWindowClick )
  EVT_UPDATE_UI( ID_NEW_WINDOW, paraverMain::OnToolNewWindowUpdate )

  EVT_MENU( ID_NEW_DERIVED_WINDOW, paraverMain::OnNewDerivedWindowClick )
  EVT_UPDATE_UI( ID_NEW_DERIVED_WINDOW, paraverMain::OnNewDerivedWindowUpdate )

  EVT_MENU( ID_NEW_HISTOGRAM, paraverMain::OnNewHistogramClick )
  EVT_UPDATE_UI( ID_NEW_HISTOGRAM, paraverMain::OnNewHistogramUpdate )

  EVT_MENU( ID_TOOLDELETE, paraverMain::OnTooldeleteClick )
  EVT_UPDATE_UI( ID_TOOLDELETE, paraverMain::OnTooldeleteUpdate )

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
END_EVENT_TABLE()


paraverMain* paraverMain::myParaverMain = NULL;

wxProgressDialog *paraverMain::dialogProgress = NULL;

wxSize paraverMain::defaultTitleBarSize = wxSize(0,0);

Window *paraverMain::beginDragWindow = NULL;
Window *paraverMain::endDragWindow = NULL;

static bool userMessage( string message )
{
  wxMessageDialog tmpDialog( NULL, message, "Paraver question", wxYES_NO | wxICON_QUESTION );
  paraverMain::myParaverMain->SetRaiseCurrentWindow( false );
  int tmpResult = tmpDialog.ShowModal();
  paraverMain::myParaverMain->SetRaiseCurrentWindow( true );
  return tmpResult == wxID_YES;
}

/*!
 * paraverMain constructors
 */

paraverMain::paraverMain()
{
  imageList = new wxImageList( 16, 16 );
  imageList->Add( wxIcon( table_xpm ) );
  imageList->Add( wxIcon( timeline_xpm ) );

  LocalKernel::init();
  localKernel = new LocalKernel( userMessage );
  paraverConfig = ParaverConfig::getInstance();
  paraverConfig->readParaverConfigFile();
  myParaverMain = this;

  Init();
  ShowToolTips();
}

paraverMain::paraverMain( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  imageList = new wxImageList( 16, 16 );
  imageList->Add( wxIcon( table_xpm ) );
  imageList->Add( wxIcon( timeline_xpm ) );

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
  menuFile = NULL;
  menuHelp = NULL;
  tbarMain = NULL;
  choiceWindowBrowser = NULL;
  windowProperties = NULL;
////@end paraverMain member initialisation
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

  tbarMain = new wxToolBar( itemFrame1, ID_TOOLBAR, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL|wxTB_NODIVIDER );
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
  tbarMain->Realize();
  itemFrame1->GetAuiManager().AddPane(tbarMain, wxAuiPaneInfo()
    .ToolbarPane().Name(_T("auiTBarMain")).Top().Layer(10).CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(false).Floatable(false).Gripper(true));

  choiceWindowBrowser = new wxChoicebook( itemFrame1, ID_CHOICEWINBROWSER, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

  itemFrame1->GetAuiManager().AddPane(choiceWindowBrowser, wxAuiPaneInfo()
    .Name(_T("auiWindowBrowser")).Caption(_T("Window browser")).Centre().CloseButton(false).DestroyOnClose(false).Resizable(true).MaximizeButton(true));

  windowProperties = new wxPropertyGrid( itemFrame1, ID_FOREIGN, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER );
  itemFrame1->GetAuiManager().AddPane(windowProperties, wxAuiPaneInfo()
    .Name(_T("auiWindowProperties")).Caption(_T("Window properties")).Centre().Position(1).CloseButton(false).DestroyOnClose(false).Resizable(true).MaximizeButton(true));

  GetAuiManager().Update();

////@end paraverMain content construction
  wxTreeCtrl* tmpTree = createTree( imageList );
  choiceWindowBrowser->AddPage( tmpTree, "All Traces" );
}


bool paraverMain::DoLoadTrace( const string &path )
{
  Trace *tr = NULL;
  bool loaded = true;
  
  map< string, UINT32 >::iterator it = traceInstance.find( path );
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
    if( path.length() > 40 )
    {
#ifdef WIN32
      string file = path.substr( path.find_last_of( '\\' ) );
      string tmp = path.substr( 0, path.find_last_of( '\\' ) );
      reducePath = "/..." + path.substr( tmp.find_last_of( '\\' ),
                                         tmp.length() - tmp.find_last_of( '\\' ) )
                   + file;
#else
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
    paraverMain::dialogProgress->Pulse( wxT( reducePath.c_str() ) );
    paraverMain::dialogProgress->Fit();
    paraverMain::dialogProgress->Show();

    tr = Trace::create( localKernel, path, progress );
    tr->setInstanceNumber( traceInstance[ path ]++ );

    loadedTraces.push_back( tr );
    currentTrace = loadedTraces.size() - 1;
    wxTreeCtrl *newTree = createTree( imageList );

    choiceWindowBrowser->AddPage( newTree, tr->getFileNameNumbered() );
    choiceWindowBrowser->ChangeSelection( choiceWindowBrowser->GetPageCount() - 1 );

    previousTraces->add( path );
  }
  catch( ParaverKernelException& ex )
  {
    loaded = false;
    wxMessageDialog message( this, ex.what(), "Error loading trace", wxOK );
    raiseCurrentWindow = false;
    message.ShowModal();
    raiseCurrentWindow = true;
  }
  paraverMain::dialogProgress->Show( false );
  delete paraverMain::dialogProgress;
  paraverMain::dialogProgress = NULL;
  delete progress;
  
  return loaded;
}

bool paraverMain::DoLoadCFG( const string &path )
{
    if( !CFGLoader::isCFGFile( path ))
    {
      wxString errMessage = path + " isn't a valid cfg.";
      wxMessageDialog message( this, errMessage.c_str(), "Invalid file", wxOK );
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
        wxString errMessage = path + " failed to load in:\n'" + CFGLoader::errorLine.c_str() + "'";
        wxMessageDialog message( this, errMessage.c_str(), "Loading error", wxOK );
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
        }

        for( vector<Histogram *>::iterator it = newHistograms.begin(); it != newHistograms.end(); ++it )
        {

          gHistogram* tmpHisto = new gHistogram( this, wxID_ANY, (*it)->getName() );
          tmpHisto->SetHistogram( *it );

          appendHistogram2Tree( tmpHisto );
          LoadedWindows::getInstance()->add( (*it) );

          tmpHisto->SetClientSize( wxRect( (*it)->getPosX(), (*it)->getPosY(),
                                           (*it)->getWidth(), (*it)->getHeight() ) );
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
  wxFileDialog dialog( this, "Load Trace", "", "", 
    "Paraver trace (*.prv;*.prv.gz)|*.prv;*.prv.gz|All files (*.*)|*.*", 
    wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_CHANGE_DIR );
  raiseCurrentWindow = false;
  if( dialog.ShowModal() == wxID_OK )
  {
    wxString path = dialog.GetPath();
    DoLoadTrace( path.c_str() );
  }
  raiseCurrentWindow = true;
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENULOADCFG
 */

void paraverMain::OnMenuloadcfgClick( wxCommandEvent& event )
{
  wxFileDialog dialog( this, "Load Configuration", "", "",
    "Paraver configuration file (*.cfg)|*.cfg|All files (*.*)|*.*",
    wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_CHANGE_DIR );
  raiseCurrentWindow = false;
  if( dialog.ShowModal() == wxID_OK )
  {
    wxString path = dialog.GetPath();
    DoLoadCFG( path.c_str());
  }
  raiseCurrentWindow = true;
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
 */

void paraverMain::OnExitClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT in paraverMain.
  // Before editing this code, remove the block markers.
  Destroy();
////@end wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT in paraverMain. 
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

  if( propName == "Name" )
  {
    wxString tmpName = property->GetValue().GetString();
    if( currentTimeline != NULL )
      currentTimeline->setName( string( tmpName.c_str() ) );
    else if( currentHisto != NULL )
      currentHisto->setName( string( tmpName.c_str() ) );
  }
  else if( propName == "Begin time" )
  {
    double tmpValue = property->GetValue().GetDouble();
    if( currentTimeline != NULL )
    {
      currentTimeline->setWindowBeginTime( currentTimeline->windowUnitsToTraceUnits( tmpValue ) );
      currentTimeline->setChanged( true );
      currentTimeline->setRedraw( true );
    }
    else if( currentHisto != NULL )
    {
      currentHisto->setWindowBeginTime( currentHisto->getControlWindow()->windowUnitsToTraceUnits( tmpValue ) );
      currentHisto->setChanged( true );
      currentHisto->setRecalc( true );
    }
  }
  else if( propName == "End time" )
  {
    double tmpValue = property->GetValue().GetDouble();
    if( currentTimeline != NULL )
    {
      currentTimeline->setWindowEndTime( currentTimeline->windowUnitsToTraceUnits( tmpValue ) );
      currentTimeline->setRedraw( true );
    }
    else if( currentHisto != NULL )
    {
      currentHisto->setWindowEndTime( currentHisto->getControlWindow()->windowUnitsToTraceUnits( tmpValue ) );
      currentHisto->setRecalc( true );
    }
  }
  else if( propName == "Semantic Minimum" )
  {
    double tmpValue = property->GetValue().GetDouble();
    currentTimeline->setMinimumY( tmpValue );
    currentTimeline->setRedraw( true );
  }
  else if( propName == "Semantic Maximum" )
  {
    double tmpValue = property->GetValue().GetDouble();
    currentTimeline->setMaximumY( tmpValue );
    currentTimeline->setRedraw( true );
  }
  // Control Window related properties
  else if( propName == "ControlWindow" )
  {
    currentHisto->setControlWindow( LoadedWindows::getInstance()->getWindow( property->GetValue().GetLong() ) );
    currentHisto->setRecalc( true );
  }
  else if( propName == "ControlMinimum" )
  {
    currentHisto->setControlMin( property->GetValue().GetDouble() );
    currentHisto->setCompute2DScale( false );
    currentHisto->setRecalc( true );
  }
  else if( propName == "ControlMaximum" )
  {
    currentHisto->setControlMax( property->GetValue().GetDouble() );
    currentHisto->setCompute2DScale( false );
    currentHisto->setRecalc( true );
  }
  else if( propName == "ControlDelta" )
  {
    currentHisto->setControlDelta( property->GetValue().GetDouble() );
    currentHisto->setCompute2DScale( false );
    currentHisto->setRecalc( true );
  }
// Data Window related properties
  else if( propName == "DataWindow" )
  {
    currentHisto->setDataWindow( LoadedWindows::getInstance()->getWindow( property->GetValue().GetLong() ) );
    currentHisto->setRecalc( true );
  }
  else if( propName == "DataMinimum" )
  {
    currentHisto->setMinGradient( property->GetValue().GetDouble() );
    currentHisto->setComputeGradient( false );
    currentHisto->setRedraw( true );
  }
  else if( propName == "DataMaximum" )
  {
    currentHisto->setMaxGradient( property->GetValue().GetDouble() );
    currentHisto->setComputeGradient( false );
    currentHisto->setRedraw( true );
  }
  // Histogram related properties
  else if( propName == "Type" )
  {
    if( property->GetValue().GetLong() == 0 )
      currentHisto->setCurrentStat( currentHisto->getFirstCommStatistic() );
    else
      currentHisto->setCurrentStat( currentHisto->getFirstStatistic() );
    currentHisto->setRedraw( true );
    currentHisto->setChanged( true );
  }
  else if( propName == "Statistic" )
  {
    currentHisto->setCurrentStat( string( property->GetDisplayedString().c_str() ) );
    currentHisto->setRedraw( true );
  }
  else if( propName == "3rd Window" )
  {
    if( property->GetValue().GetLong() == -1 )
      currentHisto->clearExtraControlWindow();
    else
      currentHisto->setExtraControlWindow( LoadedWindows::getInstance()->getWindow( property->GetValue().GetLong() ) );
    currentHisto->setRecalc( true );
  }
  else if( propName == "3DMinimum" )
  {
    currentHisto->setExtraControlMin( property->GetValue().GetDouble() );
    currentHisto->setCompute3DScale( false );
    currentHisto->setRecalc( true );
  }
  else if( propName == "3DMaximum" )
  {
    currentHisto->setExtraControlMax( property->GetValue().GetDouble() );
    currentHisto->setCompute3DScale( false );
    currentHisto->setRecalc( true );
  }
  else if( propName == "3DDelta" )
  {
    currentHisto->setExtraControlDelta( property->GetValue().GetDouble() );
    currentHisto->setCompute3DScale( false );
    currentHisto->setRecalc( true );
  }
  else if( propName == "3DPlane" )
  {
    currentHisto->setSelectedPlane( property->GetValue().GetLong() );
    currentHisto->setRedraw( true );
  }
  
  // Timeline related properties
  else if( propName == "Level" )
  {
    currentTimeline->setLevel( (TWindowLevel)property->GetValue().GetLong() );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == "Logical" )
  {
    currentTimeline->getFilter()->setLogical( property->GetValue().GetBool() );
    currentTimeline->setRedraw( true );
  }
  else if( propName == "Physical" )
  {
    currentTimeline->getFilter()->setPhysical( property->GetValue().GetBool() );
    currentTimeline->setRedraw( true );
  }
  else if( propName == "Comm from.FromFunction" )
  {
    currentTimeline->getFilter()->setCommFromFunction( string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
  }
  else if( propName == "Comm from.From" )
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
  else if( propName == "FromToOp" )
  {
    long op = property->GetValue().GetLong();
    Filter *filter = currentTimeline->getFilter();
    if( op == 0 )
      filter->setOpFromToAnd();
    else if( op == 1 )
      filter->setOpFromToOr();
      
    currentTimeline->setRedraw( true );
  }
  else if( propName == "Comm to.ToFunction" )
  {
    currentTimeline->getFilter()->setCommToFunction( string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
  }
  else if( propName == "Comm to.To" )
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
  else if( propName == "Comm tag.TagFunction" )
  {
    currentTimeline->getFilter()->setCommTagFunction( string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
  }
  else if( propName == "Comm tag.Tag" )
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
  else if( propName == "TagSizeOp" )
  {
    long op = property->GetValue().GetLong();
    Filter *filter = currentTimeline->getFilter();
    if( op == 0 )
      filter->setOpTagSizeAnd();
    else if( op == 1 )
      filter->setOpTagSizeOr();
      
    currentTimeline->setRedraw( true );
  }
  else if( propName == "Comm size.SizeFunction" )
  {
    currentTimeline->getFilter()->setCommSizeFunction( string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
  }
  else if( propName == "Comm size.Size" )
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
  else if( propName == "Comm bandwidth.BWFunction" )
  {
    currentTimeline->getFilter()->setBandWidthFunction( string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
  }
  else if( propName == "Comm bandwidth.Bandwidth" )
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
  else if( propName == "Event type.TypeFunction" )
  {
    currentTimeline->getFilter()->setEventTypeFunction( string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
  }
  else if( propName == "Event type.Types" )
  {
    Filter *filter = currentTimeline->getFilter();
    filter->clearEventTypes();
    wxArrayInt value = ( (prvEventTypeProperty *) property )->GetValueAsArrayInt();
    for( unsigned int idx = 0; idx < value.GetCount(); idx++ )
      filter->insertEventType( value[ idx ] );
    
    currentTimeline->setRedraw( true );
  }
  else if( propName == "TypeValueOp" )
  {
    long op = property->GetValue().GetLong();
    Filter *filter = currentTimeline->getFilter();
    if( op == 0 )
      filter->setOpTypeValueAnd();
    else if( op == 1 )
      filter->setOpTypeValueOr();
      
    currentTimeline->setRedraw( true );
  }
  else if( propName == "Event value.ValueFunction" )
  {
    currentTimeline->getFilter()->setEventValueFunction( string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
  }
  else if( propName == "Event value.Values" )
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
  else if( propName == "Top Compose 1" )
  {
    currentTimeline->setLevelFunction( TOPCOMPOSE1, string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == "Top Compose 2" )
  {
    currentTimeline->setLevelFunction( TOPCOMPOSE2, string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == "Derived" )
  {
    currentTimeline->setLevelFunction( DERIVED, string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == "Compose Workload" )
  {
    currentTimeline->setLevelFunction( COMPOSEWORKLOAD, string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == "Workload" )
  {
    currentTimeline->setLevelFunction( WORKLOAD, string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == "Compose Appl" )
  {
    currentTimeline->setLevelFunction( COMPOSEAPPLICATION, string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == "Application" )
  {
    currentTimeline->setLevelFunction( APPLICATION, string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == "Compose Task" )
  {
    currentTimeline->setLevelFunction( COMPOSETASK, string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == "Task" )
  {
    currentTimeline->setLevelFunction( TASK, string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == "Compose System" )
  {
    currentTimeline->setLevelFunction( COMPOSESYSTEM, string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == "System" )
  {
    currentTimeline->setLevelFunction( SYSTEM, string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == "Compose Node" )
  {
    currentTimeline->setLevelFunction( COMPOSENODE, string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == "Node" )
  {
    currentTimeline->setLevelFunction( NODE, string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == "Compose CPU" )
  {
    currentTimeline->setLevelFunction( COMPOSECPU, string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == "CPU" )
  {
    currentTimeline->setLevelFunction( CPU, string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == "Compose Thread" )
  {
    currentTimeline->setLevelFunction( COMPOSETHREAD, string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName == "Thread" )
  {
    currentTimeline->setLevelFunction( THREAD, string( property->GetDisplayedString().c_str() ) );
    currentTimeline->setRedraw( true );
    currentTimeline->setChanged( true );
  }
  else if( propName.BeforeFirst( ' ' ) == "Param" )
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
    for ( unsigned int current = 0; current < numberChild; ++current )
    {
      if ( currentChild.IsOk() )
      {
        updateTreeItem( currentTree, currentChild, allWindows, allHistograms, &currentWindow, iPage == 0 );
        if( iPage > 0 && choiceWindowBrowser->GetSelection() > 0 && currentWindow != NULL )
          choiceWindowBrowser->SetSelection( iPage );
      }
      currentChild = currentTree->GetNextChild( root, cookie );
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
    gHistogram* tmpHisto = new gHistogram( this, wxID_ANY, (*it)->getName() );
    tmpHisto->SetHistogram( *it );

    appendHistogram2Tree( tmpHisto );
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
      DoLoadTrace( previousTraces->getFiles()[i] );
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
      wxMenuItem *newItem = new wxMenuItem( menuTraces, wxID_ANY, wxT( (*it).c_str() ) );
      menuTraces->Append( newItem );
      Connect( newItem->GetId(),
               wxEVT_COMMAND_MENU_SELECTED,
               (wxObjectEventFunction)&paraverMain::OnPreviousTracesClick );
    }
    else
    {
      wxMenuItem *tmp = *menuIt;
      tmp->SetItemLabel( wxT( (*it).c_str() ) );
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
      wxMenuItem *newItem = new wxMenuItem( menuCFGs, wxID_ANY, wxT( (*it).c_str() ) );
      menuCFGs->Append( newItem );
      Connect( newItem->GetId(),
               wxEVT_COMMAND_MENU_SELECTED,
              (wxObjectEventFunction)&paraverMain::OnPreviousCFGsClick );
    }
    else
    {
      wxMenuItem *tmp = *menuIt;
      tmp->SetItemLabel( wxT( (*it).c_str() ) );
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
  if( wxTheApp->IsActive() )
  {
#if 0 // Because problems on Windows and some window managers (KDE)
    bool showWindow = false;
    if( currentTimeline != NULL )
      showWindow = currentTimeline->getShowWindow();
    else if( currentHisto != NULL )
      showWindow = currentHisto->getShowWindow();
    if( currentWindow != NULL && showWindow && raiseCurrentWindow )
      currentWindow->Raise();
#endif

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
    timelines = saveDialog.GetTimelines();
    histograms = saveDialog.GetHistograms();
    options = saveDialog.GetOptions();
    wxFileDialog dialog( this, "Save Configuration", "", "",
      "Paraver configuration file (*.cfg)|*.cfg",
      wxFD_SAVE|wxFD_OVERWRITE_PROMPT|wxFD_CHANGE_DIR );
    if( dialog.ShowModal() == wxID_OK )
    {
      wxString path = dialog.GetPath();
      CFGLoader::saveCFG( path.c_str(), options, timelines, histograms );
      previousCFGs->add( path.c_str() );
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
  wxString tmpName( "New window #" );
  tmpName << numNewWindows;
  newWindow->setName( tmpName.c_str() );
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
  for ( UINT16 windowLevel = TOPCOMPOSE1; windowLevel <= COMPOSECPU; windowLevel++ )
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
  wxString tmpName( "New Derived Window #" );
  tmpName << numNewDerived;
  derivedDialog.SetTimelineName( tmpName.c_str() );

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
    newWindow->setTimeUnit( loadedTraces[ currentTrace ]->getTimeUnit() );
    newWindow->setWindowBeginTime( beginDragWindow->getWindowBeginTime() );
    newWindow->setWindowEndTime( beginDragWindow->getWindowEndTime() );
    newWindow->addZoom( beginDragWindow->getWindowBeginTime(),
                        beginDragWindow->getWindowEndTime(),
                        0, newWindow->getWindowLevelObjects() - 1 );
    newWindow->setShowChildrenWindow( false );

    // Size
    newWindow->setWidth( defaultWindowSize.GetWidth() ); // magic numbers!
    newWindow->setHeight( defaultWindowSize.GetHeight() );

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
  HistogramDialog histogramDialog( this );

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
    wxString tmpName( "New Histogram #" );
    tmpName << numNewHistograms;
    newHistogram->setName( tmpName.c_str() );
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
    gHistogram* tmpHisto = new gHistogram( this, wxID_ANY, composedName );
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

  preferences.SetHistogramNumColumns( paraverConfig->getHistoNumColumns() );
  preferences.SetHistogramMaxNumColumns( 400 ); // TO IMPLEMENT
  preferences.SetHistogramPrecision( paraverConfig->getPrecision() );
  preferences.SetHistogramMaxPrecision( 20 ); // TO IMPLEMENT
  preferences.SetHistogramShowUnits( paraverConfig->getShowUnits() );
  preferences.SetHistogramThousandSeparator( paraverConfig->getThousandSep() );
  preferences.SetWhatWherePrecision( 2 ); // TO IMPLEMENT

  preferences.TransferDataToWindow();

  raiseCurrentWindow = true;
  if ( preferences.ShowModal() == wxID_OK )
  {
    preferences.TransferDataFromWindow();

    // Apply Preferences
    paraverConfig->setHistoNumColumns( preferences.GetHistogramNumColumns() );
    paraverConfig->setPrecision( preferences.GetHistogramPrecision() );
    paraverConfig->setShowUnits( preferences.GetHistogramShowUnits() );
    paraverConfig->setThousandSep( preferences.GetHistogramThousandSeparator() );

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


UINT16 paraverMain::getTracePosition( Trace *trace )
{
  UINT16 currentTrace;

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
      currentTimeline->setDestroy( true );
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
    choices.Add( (*it)->getTraceNameNumbered().c_str() );
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
    (*it)->setShowWindow( false );
    (*it)->setDestroy( true );
  }
  
  loadedTraces[ whichTrace ]->setUnload( true );
}

void paraverMain::clearProperties()
{
  windowProperties->Clear();
}
