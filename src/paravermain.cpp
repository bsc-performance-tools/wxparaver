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

////@begin includesMPI
#include "wx/imaglist.h"
////@end includes

#include "wx/tipdlg.h"
#include "wx/aboutdlg.h"
#include "wx/html/htmlwin.h"

#include "wx/fs_mem.h"
#include "wx/filesys.h"
#include "wx/wfstream.h"
#include "logoBSC.xpm"

#include "config.h"
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
//#include "preferencesdialog.h"
#include "windows_tree.h"
#include "derivedtimelinedialog.h"
#include "histogramdialog.h"
#include "cutfilterdialog.h"
#include "labelconstructor.h"
#include "sessionsaver.h"
#include "helpcontents.h"
#include "filedialogext.h"
#include "exitdialog.h"
#include "runscript.h"
#include <wx/display.h>
#include <wx/process.h>

#include <signal.h>
#include <iostream>
#include <sstream>
#include <algorithm>
//#include "connection.h"

#ifdef WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <shlobj.h>
#include <Shlwapi.h>

#define MAX_LEN_PATH 2048
#endif

#ifdef __WXMAC__
#include <CoreFoundation/CoreFoundation.h>
#endif

////@begin XPM images
#include "../icons/new_window.xpm"
#include "../icons/new_derived_window.xpm"
#include "../icons/new_histogram.xpm"
#include "../icons/delete.xpm"
#include "../icons/cut_trace.xpm"
#include "../icons/run_script.xpm"
#include "../icons/file_browser.xpm"
#include "../icons/window_properties.xpm"
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
#include "derived_controlled_enumerate.xpm"

#include <algorithm>

class MenuHintFile : public wxObjectRefData
{
  public:
    string fileName;
};


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
  EVT_MENU( ID_MENULOADSESSION, paraverMain::OnMenuloadsessionClick )
  EVT_UPDATE_UI( ID_RECENTSESSIONS, paraverMain::OnRecentsessionsUpdate )
  EVT_MENU( ID_MENUSAVESESSION, paraverMain::OnMenusavesessionClick )
  EVT_MENU( wxID_PREFERENCES, paraverMain::OnPreferencesClick )
  EVT_UPDATE_UI( wxID_PREFERENCES, paraverMain::OnPreferencesUpdate )
  EVT_MENU( wxID_EXIT, paraverMain::OnExitClick )
  EVT_MENU( wxID_HELPCONTENTS, paraverMain::OnHelpcontentsClick )
  EVT_MENU( wxID_TUTORIALS, paraverMain::OnTutorialsClick )
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
  EVT_MENU( ID_TOOL_RUN_APPLICATION, paraverMain::OnToolRunApplicationClick )
  EVT_CHOICEBOOK_PAGE_CHANGED( ID_CHOICEWINBROWSER, paraverMain::OnChoicewinbrowserPageChanged )
  EVT_UPDATE_UI( ID_CHOICEWINBROWSER, paraverMain::OnChoicewinbrowserUpdate )
  EVT_UPDATE_UI( ID_FOREIGN, paraverMain::OnForeignUpdate )
  EVT_UPDATE_UI( wxID_STATIC_AUTO_REDRAW, paraverMain::OnStaticAutoRedrawUpdate )
  EVT_UPDATE_UI( ID_CHECKBOX_AUTO_REDRAW, paraverMain::OnCheckboxAutoRedrawUpdate )
  EVT_BUTTON( ID_BUTTON_FORCE_REDRAW, paraverMain::OnButtonForceRedrawClick )
  EVT_UPDATE_UI( ID_BUTTON_FORCE_REDRAW, paraverMain::OnButtonForceRedrawUpdate )
  EVT_BUTTON( ID_BUTTON_ACTIVE_WORKSPACES, paraverMain::OnButtonActiveWorkspacesClick )
////@end paraverMain event table entries

  EVT_TREE_SEL_CHANGED( wxID_ANY, paraverMain::OnTreeSelChanged )
  EVT_TREE_ITEM_ACTIVATED( wxID_ANY, paraverMain::OnTreeItemActivated )
  EVT_TREE_ITEM_RIGHT_CLICK(wxID_ANY, paraverMain::OnTreeRightClick)
  EVT_TREE_END_LABEL_EDIT( wxID_ANY, paraverMain::OnTreeEndLabelRename )
  //EVT_KEY_DOWN( wxID_ANY, paraverMain::OnTreeKeyPress )
  

  EVT_TREE_BEGIN_DRAG(wxID_ANY, paraverMain::OnTreeBeginDrag)
  EVT_TREE_END_DRAG( wxID_ANY, paraverMain::OnTreeEndDrag)

  EVT_PG_CHANGED( ID_FOREIGN, paraverMain::OnPropertyGridChange )
  EVT_PG_CHANGING( ID_FOREIGN, paraverMain::OnPropertyGridChanging )
  EVT_ACTIVATE( paraverMain::OnActivate )
  
  EVT_TIMER( ID_TIMER_MAIN, paraverMain::OnSessionTimer )

END_EVENT_TABLE()


paraverMain* paraverMain::myParaverMain = NULL;

wxProgressDialog *paraverMain::dialogProgress = NULL;

wxSize paraverMain::defaultTitleBarSize = wxSize(0,0);

Window *paraverMain::beginDragWindow = NULL;
Window *paraverMain::endDragWindow = NULL;

bool paraverMain::disableUserMessages = false;
bool paraverMain::validSessions = true;

extern volatile bool sig1;
extern volatile bool sig2;
extern struct sigaction act;

static bool userMessage( UserMessageID message )
{
  if( paraverMain::disableUserMessages )
    return true;
  wxMessageDialog tmpDialog( NULL, wxString::FromAscii( userMessages[ message ].c_str() )  + 
        _( " Continue loading CFG file?" ), _( "Paraver question" ), wxYES_NO | wxICON_QUESTION );
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
  images->Add( wxIcon( derived_controlled_enumerate_xpm ) );

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
  myParaverMain = this;

  Init();
  ShowToolTips();
}

paraverMain::paraverMain( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  imageList = getImageList();

  LocalKernel::init();
  localKernel = new LocalKernel( userMessage );
  myParaverMain = this;

  Init();
  Create( parent, id, caption, pos, size, style );

  defaultTitleBarSize = GetSize() - GetClientSize();
  ShowToolTips();
}


bool paraverMain::isSessionFile( const string& filename )
{
  string suffix( "" );

  if ( filename.length() > string( SESSION_FILE_SUFFIX ).length() )
    suffix = filename.substr( filename.length() - string( SESSION_FILE_SUFFIX ).length() );

  return ( suffix.compare( string( SESSION_FILE_SUFFIX ) ) == 0 );
}


void paraverMain::commandLineLoadings( wxCmdLineParser &parser )
{
  string fileName;
  for ( unsigned int i = 0; i < parser.GetParamCount(); ++i )
  {
    fileName = parser.GetParam( i ).mb_str();

    if ( isSessionFile( fileName ) )
      DoLoadSession( fileName );
    else if ( localKernel->isTraceFile( fileName ) )
      DoLoadTrace( fileName );
    else if ( CFGLoader::isCFGFile( fileName ) && !loadedTraces.empty() )
      DoLoadCFG( fileName );

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
  if ( tutorialsWindow != NULL )
  {
    delete tutorialsWindow;
  }
  if ( helpContents != NULL )
  {
    delete helpContents;
  }

  wxMemoryFSHandler::RemoveFile( wxT("logoBSC.xpm") );
}


/*!
 * Member initialisation
 */

void paraverMain::Init()
{
////@begin paraverMain member initialisation
  CFGLoadedBefore = false;
  XMLLoadedBefore = false;
  canServeSignal = true;
  clusteringWindow = NULL;
  currentHisto = NULL;
  currentTimeline = NULL;
  currentTrace = -1;
  currentWindow = NULL;
  helpContents = NULL;
  lastHisto = NULL;
  lastTimeline = NULL;
  numNewDerived = 0;
  numNewHistograms = 0;
  numNewWindows = 0;
  paraverConfig = ParaverConfig::getInstance();
  previousCFGs = PreviousFiles::createPreviousCFGs();
  previousCutFilteredTraces = PreviousFiles::createPreviousTreatedTraces();
  previousSessions = PreviousFiles::createPreviousSessions();
  previousTraces = PreviousFiles::createPreviousTraces();
  raiseCurrentWindow = true;
  runApplication = NULL;
  sessionTimer = new wxTimer( this, ID_TIMER_MAIN );
  traceLoadedBefore = false;
  tutorialsWindow = NULL;
  workspacesManager = WorkspaceManager::getInstance();
  openedPropertyDialog = NULL;
  menuFile = NULL;
  menuHints = NULL;
  menuHelp = NULL;
  tbarMain = NULL;
  choiceWindowBrowser = NULL;
  toolBookFilesProperties = NULL;
  dirctrlFiles = NULL;
  windowProperties = NULL;
  panelAutoRedraw = NULL;
  checkAutoRedraw = NULL;
  buttonForceRedraw = NULL;
  txtActiveWorkspaces = NULL;
  btnActiveWorkspaces = NULL;
////@end paraverMain member initialisation

  if ( ParaverConfig::getInstance()->getGlobalSessionSaveTime() > 0 )
  {
    sessionTimer->Start( ParaverConfig::getInstance()->getGlobalSessionSaveTime() * 60e3 );
  }

  traceLoadedBefore = false;
  CFGLoadedBefore = false;
  firstSave = true;
  instChecker = NULL;

  wxFileSystem::AddHandler( new wxMemoryFSHandler() );
#ifdef WIN32
  wxBitmap bmp( logoBSC_xpm );
#endif

  wxMemoryFSHandler::AddFile( wxT( "logoBSC.xpm" ),
#ifdef WIN32
                              bmp,
#else
                              wxBITMAP( logoBSC ),
#endif
                              wxBITMAP_TYPE_XPM );

  workspacesManager->loadXML();

  initPG();
  initSessionInfo();

  if ( !ParaverConfig::getInstance()->getAppsChecked() )
  {
    filterExternalApps();
    ParaverConfig::getInstance()->setAppsChecked();
  }
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
  menuFile->Append(ID_UNLOADTRACE, _("Unload Traces..."), wxEmptyString, wxITEM_NORMAL);
  menuFile->AppendSeparator();
  menuFile->Append(ID_MENULOADCFG, _("Load &Configuration..."), wxEmptyString, wxITEM_NORMAL);
  wxMenu* itemMenu9 = new wxMenu;
  menuFile->Append(ID_RECENTCFGS, _("Previous Configurations"), itemMenu9);
  menuFile->Append(ID_MENUSAVECFG, _("&Save Configuration..."), wxEmptyString, wxITEM_NORMAL);
  menuFile->AppendSeparator();
  menuFile->Append(ID_MENULOADSESSION, _("Load Session...\tCTRL+l"), wxEmptyString, wxITEM_NORMAL);
  wxMenu* itemMenu1 = new wxMenu;
  menuFile->Append(ID_RECENTSESSIONS, _("Previous Sessions"), itemMenu1);
  menuFile->Append(ID_MENUSAVESESSION, _("Save Session...\tCTRL+S"), wxEmptyString, wxITEM_NORMAL);
  menuFile->AppendSeparator();
  menuFile->Append(wxID_PREFERENCES, _("&Preferences..."), wxEmptyString, wxITEM_NORMAL);
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT, _("&Quit"), wxEmptyString, wxITEM_NORMAL);
  menuBar->Append(menuFile, _("&File"));
  menuHints = new wxMenu;
  menuBar->Append(menuHints, _("Hints"));
  menuHelp = new wxMenu;
  menuHelp->Append(wxID_HELPCONTENTS, _("&Help Contents..."), wxEmptyString, wxITEM_NORMAL);
  menuHelp->Append(wxID_TUTORIALS, _("&Tutorials..."), wxEmptyString, wxITEM_NORMAL);
  menuHelp->Append(wxID_ABOUT, _("&About..."), wxEmptyString, wxITEM_NORMAL);
  menuBar->Append(menuHelp, _("&Help"));
  itemFrame1->SetMenuBar(menuBar);

  tbarMain = new wxToolBar( itemFrame1, ID_TOOLBAR, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL|wxTB_NODIVIDER|wxWANTS_CHARS );
  wxBitmap itemtool24Bitmap(itemFrame1->GetBitmapResource(wxT("icons/new_window.xpm")));
  wxBitmap itemtool24BitmapDisabled;
  tbarMain->AddTool(ID_NEW_WINDOW, _("Create new window"), itemtool24Bitmap, itemtool24BitmapDisabled, wxITEM_NORMAL, _("New single timeline window"), wxEmptyString);
  tbarMain->EnableTool(ID_NEW_WINDOW, false);
  wxBitmap itemtool25Bitmap(itemFrame1->GetBitmapResource(wxT("icons/new_derived_window.xpm")));
  wxBitmap itemtool25BitmapDisabled;
  tbarMain->AddTool(ID_NEW_DERIVED_WINDOW, _("Create new derived window"), itemtool25Bitmap, itemtool25BitmapDisabled, wxITEM_NORMAL, _("New derived timeline window"), wxEmptyString);
  tbarMain->EnableTool(ID_NEW_DERIVED_WINDOW, false);
  wxBitmap itemtool26Bitmap(itemFrame1->GetBitmapResource(wxT("icons/new_histogram.xpm")));
  wxBitmap itemtool26BitmapDisabled;
  tbarMain->AddTool(ID_NEW_HISTOGRAM, _("Create new histogram"), itemtool26Bitmap, itemtool26BitmapDisabled, wxITEM_NORMAL, _("New histogram"), wxEmptyString);
  tbarMain->EnableTool(ID_NEW_HISTOGRAM, false);
  tbarMain->AddSeparator();
  wxBitmap itemtool28Bitmap(itemFrame1->GetBitmapResource(wxT("icons/delete.xpm")));
  wxBitmap itemtool28BitmapDisabled;
  tbarMain->AddTool(ID_TOOLDELETE, _("Delete window (Ctrl+DEL)"), itemtool28Bitmap, itemtool28BitmapDisabled, wxITEM_NORMAL, _("Delete selected window (Ctrl+DEL)"), wxEmptyString);
  tbarMain->AddSeparator();
  wxBitmap itemtool30Bitmap(itemFrame1->GetBitmapResource(wxT("icons/cut_trace.xpm")));
  wxBitmap itemtool30BitmapDisabled;
  tbarMain->AddTool(ID_TOOL_CUT_TRACE, _("Filter Trace"), itemtool30Bitmap, itemtool30BitmapDisabled, wxITEM_NORMAL, _("Filter Trace"), wxEmptyString);
  wxBitmap itemtool31Bitmap(itemFrame1->GetBitmapResource(wxT("icons/run_script.xpm")));
  wxBitmap itemtool31BitmapDisabled;
  tbarMain->AddTool(ID_TOOL_RUN_APPLICATION, _("Run Application"), itemtool31Bitmap, itemtool31BitmapDisabled, wxITEM_NORMAL, _("Run Application"), wxEmptyString);
  tbarMain->Realize();
  itemFrame1->GetAuiManager().AddPane(tbarMain, wxAuiPaneInfo()
    .ToolbarPane().Name(wxT("auiTBarMain")).Top().Layer(10).CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(false).Floatable(false).Gripper(true));

  choiceWindowBrowser = new wxChoicebook( itemFrame1, ID_CHOICEWINBROWSER, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT|wxWANTS_CHARS );

  itemFrame1->GetAuiManager().AddPane(choiceWindowBrowser, wxAuiPaneInfo()
    .Name(wxT("auiWindowBrowser")).Caption(_("Window browser")).Centre().Position(1).CloseButton(false).DestroyOnClose(false).Resizable(true));

  toolBookFilesProperties = new wxToolbook( itemFrame1, ID_TOOLBOOKFILESANDPROPERTIES, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );
  wxImageList* toolBookFilesPropertiesImageList = new wxImageList(16, 16, true, 2);
  {
    wxIcon toolBookFilesPropertiesIcon0(itemFrame1->GetIconResource(wxT("icons/file_browser.xpm")));
    toolBookFilesPropertiesImageList->Add(toolBookFilesPropertiesIcon0);
    wxIcon toolBookFilesPropertiesIcon1(itemFrame1->GetIconResource(wxT("icons/window_properties.xpm")));
    toolBookFilesPropertiesImageList->Add(toolBookFilesPropertiesIcon1);
  }
  toolBookFilesProperties->AssignImageList(toolBookFilesPropertiesImageList);

  dirctrlFiles = new wxGenericDirCtrl( toolBookFilesProperties, ID_DIRCTRLFILES, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRCTRL_SELECT_FIRST|wxDIRCTRL_SHOW_FILTERS, wxT("Paraver files|*.prv;*.prv.gz;*.cfg|CFG files (*.cfg)|*.cfg|PRV Files (*.prv, *.prv.gz)|*.prv;*.prv.gz"), 0 );

  toolBookFilesProperties->AddPage(dirctrlFiles, wxEmptyString, false, 0);

  windowProperties = new wxPropertyGrid( toolBookFilesProperties, ID_FOREIGN, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxWANTS_CHARS );

  toolBookFilesProperties->AddPage(windowProperties, wxEmptyString, false, 1);

  itemFrame1->GetAuiManager().AddPane(toolBookFilesProperties, wxAuiPaneInfo()
    .Name(wxT("auiCfgAndProperties")).Caption(_("Files & Window Properties")).Centre().Position(2).CloseButton(false).DestroyOnClose(false).Resizable(true).PaneBorder(false));

  panelAutoRedraw = new wxPanel( itemFrame1, ID_PANEL_AUTOREDRAW, wxDefaultPosition, wxDefaultSize, 0 );
  panelAutoRedraw->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemFrame1->GetAuiManager().AddPane(panelAutoRedraw, wxAuiPaneInfo()
    .Name(wxT("auiAutoRedraw")).Caption(_("Auto Redraw")).Bottom().Dockable(false).CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(false).Floatable(false).Movable(false).PaneBorder(false));

  wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
  panelAutoRedraw->SetSizer(itemBoxSizer4);

  wxStaticBitmap* itemStaticBitmap5 = new wxStaticBitmap( panelAutoRedraw, wxID_STATIC_AUTO_REDRAW, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer4->Add(itemStaticBitmap5, 0, wxALIGN_CENTER_VERTICAL|wxALL, wxDLG_UNIT(panelAutoRedraw, wxSize(2, -1)).x);

  checkAutoRedraw = new wxCheckBox( panelAutoRedraw, ID_CHECKBOX_AUTO_REDRAW, _("Automatic Redraw"), wxDefaultPosition, wxDefaultSize, 0 );
  checkAutoRedraw->SetValue(true);
  itemBoxSizer4->Add(checkAutoRedraw, 1, wxALIGN_CENTER_VERTICAL|wxALL, wxDLG_UNIT(panelAutoRedraw, wxSize(2, -1)).x);

  buttonForceRedraw = new wxButton( panelAutoRedraw, ID_BUTTON_FORCE_REDRAW, _("Force Redraw"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer4->Add(buttonForceRedraw, 0, wxALIGN_CENTER_VERTICAL|wxALL, wxDLG_UNIT(panelAutoRedraw, wxSize(1, -1)).x);

  // Fit to content
  itemFrame1->GetAuiManager().GetPane(wxT("auiAutoRedraw")).BestSize(panelAutoRedraw->GetSizer()->Fit(panelAutoRedraw)).MinSize(panelAutoRedraw->GetSizer()->GetMinSize());

  wxPanel* itemPanel36 = new wxPanel( itemFrame1, ID_PANEL_WORKSPACES, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  itemFrame1->GetAuiManager().AddPane(itemPanel36, wxAuiPaneInfo()
    .Name(wxT("Pane1")).Caption(_("Workspaces")).Top().Dockable(false).CloseButton(false).DestroyOnClose(false).Resizable(false).Floatable(false).Movable(false).PaneBorder(false));

  wxBoxSizer* itemBoxSizer37 = new wxBoxSizer(wxHORIZONTAL);
  itemPanel36->SetSizer(itemBoxSizer37);

  txtActiveWorkspaces = new wxTextCtrl( itemPanel36, ID_TEXT_ACTIVE_WORKSPACE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
  itemBoxSizer37->Add(txtActiveWorkspaces, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

  btnActiveWorkspaces = new wxButton( itemPanel36, ID_BUTTON_ACTIVE_WORKSPACES, _(" ... "), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
  btnActiveWorkspaces->Show(false);
  itemBoxSizer37->Add(btnActiveWorkspaces, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

  // Fit to content
  itemFrame1->GetAuiManager().GetPane(wxT("Pane1")).BestSize(itemPanel36->GetSizer()->Fit(itemPanel36)).MinSize(itemPanel36->GetSizer()->GetMinSize());

  GetAuiManager().Update();

////@end paraverMain content construction
  wxTreeCtrl* tmpTree = createTree( imageList );
  tmpTree->Connect( wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler( paraverMain::OnTreeKeyPress ), NULL, this );
  choiceWindowBrowser->AddPage( tmpTree, _( "All Traces" ) );
#if wxMAJOR_VERSION>=3
  choiceWindowBrowser->AddPage( createTree( imageList ), _( "Dummy Tree" ) );
  choiceWindowBrowser->DeletePage( 1 );
#endif

  toolBookFilesProperties->GetToolBar()->SetToolShortHelp( 1, wxT("Paraver Files") );
  toolBookFilesProperties->GetToolBar()->SetToolShortHelp( 2, wxT("Window Properties") );

  dirctrlFiles->SetPath( wxString( ParaverConfig::getInstance()->getGlobalCFGsPath().c_str(), wxConvUTF8 ));

  setActiveWorkspacesText();
//  refreshMenuHints();

  // These are here because no UpdateUI for MenuBar
  wxUpdateUIEvent tmpEvent;
  OnMenuHintUpdate( tmpEvent );
}


void paraverMain::refreshMenuHints()
{
  bool separator = false;

  // Destroy previous if any
  size_t maxItems = menuHints->GetMenuItemCount();
  for ( size_t i = 0; i < maxItems; ++i )
  {
    wxMenuItem *currentItem = menuHints->FindItemByPosition( 0 );
    //int id = currentItem->GetId();
    //Disconnect( id, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&paraverMain::OnHintClick );
    menuHints->Destroy( currentItem );
  }

  if( currentTrace == -1 )
    return;

  // Create updated one
  size_t currentWorkspace = 0;
  for ( vector< string >::iterator it = traceWorkspaces[ getCurrentTrace() ].begin(); it != traceWorkspaces[ getCurrentTrace()  ].end(); ++it )
  {
    wxString currentWorkspaceName = wxString::FromAscii( it->c_str() );
    wxMenu *currentWorkspaceMenu = new wxMenu();

    std::vector< std::pair< std::string, std::string > > currentHints;
    if( currentWorkspace < firstUserWorkspace[ getCurrentTrace() ] ) // Distributed workspaces
    {
      currentHints = workspacesManager->getWorkspace( *it, WorkspaceManager::DISTRIBUTED ).getHintCFGs();
      if( workspacesManager->existWorkspace( *it, WorkspaceManager::USER_DEFINED ) )
      {
        vector< WorkspaceValue > tmpDistAutoTypes = workspacesManager->getWorkspace( *it, WorkspaceManager::DISTRIBUTED ).getAutoTypes();
        sort( tmpDistAutoTypes.begin(), tmpDistAutoTypes.end() );
        vector< WorkspaceValue > tmpUserAutoTypes = workspacesManager->getWorkspace( *it, WorkspaceManager::USER_DEFINED ).getAutoTypes();
        sort( tmpUserAutoTypes.begin(), tmpUserAutoTypes.end() );
        if( includes( tmpDistAutoTypes.begin(), tmpDistAutoTypes.end(),
                      tmpUserAutoTypes.begin(), tmpUserAutoTypes.end() ) )
        {
          currentHints.push_back( std::pair< std::string, std::string >( "WXSEPARATOR", "WXSEPARATOR" ) );
          std::vector< std::pair< std::string, std::string > > tmpHints = workspacesManager->getWorkspace( *it, WorkspaceManager::USER_DEFINED ).getHintCFGs();
          currentHints.insert( currentHints.end(), tmpHints.begin(), tmpHints.end() );
        }
      }
    }
    else // User defined workspaces
    {
      if( !separator )
      {
        menuHints->AppendSeparator();
        separator = true;
      }
      if( workspacesManager->existWorkspace( *it, WorkspaceManager::DISTRIBUTED ) )
        currentWorkspaceName += wxT( "#2" );
      currentHints = workspacesManager->getWorkspace( *it, WorkspaceManager::USER_DEFINED ).getHintCFGs();
    }

    for ( std::vector<std::pair<std::string,std::string> >::iterator it2 = currentHints.begin(); it2 != currentHints.end(); ++it2 )
    {
      if( (*it2).first == "WXSEPARATOR" && (*it2).second == "WXSEPARATOR" )
      {
        currentWorkspaceMenu->AppendSeparator();
        continue;
      }
      wxString tmpName = getHintComposed( *it2 );
      wxMenuItem *currentHint = new wxMenuItem( currentWorkspaceMenu, wxID_ANY, tmpName );

      MenuHintFile *tmpHintFile = new MenuHintFile();
      tmpHintFile->fileName = (*it2).first;
      currentHint->SetRefData( tmpHintFile );

      currentWorkspaceMenu->Append( currentHint );
      Connect( currentHint->GetId(),
               wxEVT_COMMAND_MENU_SELECTED,
               (wxObjectEventFunction)&paraverMain::OnHintClick );
    }

    menuHints->AppendSubMenu( currentWorkspaceMenu, currentWorkspaceName );

    ++currentWorkspace;
  }
}


// Initial set, to be called after inserting new trace
void paraverMain::setTraceWorkspaces( Trace *whichTrace )
{
  set< TState > tmpLoadedStates = whichTrace->getLoadedStates();
  set< TEventType > tmpLoadedTypes = whichTrace->getLoadedEvents();
  firstUserWorkspace[ whichTrace ] = 0;
  traceWorkspaces[ whichTrace ].clear();
  workspacesManager->getMergedWorkspaces( tmpLoadedStates, tmpLoadedTypes, traceWorkspaces[ whichTrace ], firstUserWorkspace[ whichTrace ] );
}


void paraverMain::DoLoadSession( const string &whichFileName )
{
  wxFileName tmpFileName( wxString( whichFileName.c_str(), wxConvUTF8 ) );

#ifdef WIN32
  tmpFileName.Normalize( wxPATH_NORM_DOTS | wxPATH_NORM_ABSOLUTE |
                         wxPATH_NORM_LONG );
#else
  tmpFileName.Normalize( wxPATH_NORM_DOTS | wxPATH_NORM_ABSOLUTE |
                         wxPATH_NORM_LONG | wxPATH_NORM_TILDE );
#endif
  if ( tmpFileName.IsFileReadable() )
  {
    SessionSaver::LoadSession( tmpFileName.GetFullPath() );
    previousSessions->add( std::string( tmpFileName.GetFullPath().mb_str() ) );
  }
  else
  {
    wxString errMessage = wxString::FromAscii( whichFileName.c_str() ) + _( " isn't a valid session file." );
    wxMessageDialog message( this, errMessage, _( "Invalid file" ), wxOK );
    raiseCurrentWindow = false;
    message.ShowModal();
    raiseCurrentWindow = true;
  }
}


bool paraverMain::DoLoadTrace( const string &path )
{
  Trace *tr = NULL;
  bool loaded = true;

  canServeSignal = false;

  // Append whole path.
  wxFileName tmpFileName( wxString( path.c_str(), wxConvUTF8 ) );

#ifdef WIN32
  tmpFileName.Normalize( wxPATH_NORM_DOTS | wxPATH_NORM_ABSOLUTE |
                         wxPATH_NORM_LONG );
#else
  tmpFileName.Normalize( wxPATH_NORM_DOTS | wxPATH_NORM_ABSOLUTE |
                         wxPATH_NORM_LONG | wxPATH_NORM_TILDE );
#endif

  string tmpPath = std::string( tmpFileName.GetFullPath().mb_str() );

  TTraceSize traceSize = localKernel->getTraceSize( tmpPath );
  if( !localKernel->checkTraceSize( tmpPath, ParaverConfig::getInstance()->getFiltersFilterTraceUpToMB() * 1E6 ) )
  {
    wxString tmpSize;
    tmpSize << rint( traceSize / 1E6 );
    wxMessageDialog maxSizeDialog( this,
            wxString( wxT( "The size (" ) ) +
                      tmpSize +
                      wxString( wxT( " MB) of the trace\n\n  " ) ) +
                      wxString::FromAscii( tmpPath.c_str() ) +
                      wxString( wxT( "\n\nexceeds the maximum loadable defined"
                                     " in Preferences.\n\nWould you like to cut/filter the trace?" ) ),
            wxT( "Reduce trace size" ),
            wxYES_NO|wxCANCEL|wxICON_QUESTION );

    switch( maxSizeDialog.ShowModal() )
    {
      case wxID_YES:
        ShowCutTraceWindow( tmpPath, true );
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

  map< string, PRV_UINT32 >::iterator it = traceInstance.find( std::string( tmpFileName.GetFullName().mb_str() ) );
  if ( it == traceInstance.end() )
    traceInstance[ std::string( tmpFileName.GetFullName().mb_str() ) ] = 0;

  ProgressController *progress = ProgressController::create( localKernel );
  progress->setHandler( progressFunction, this );

  try
  {
    if( paraverMain::dialogProgress == NULL )
      paraverMain::dialogProgress = new wxProgressDialog( wxT("Loading trace..."),
                                                          wxT(""),
                                                          numeric_limits<int>::max(),
                                                          this,
                                                          wxPD_CAN_ABORT|wxPD_AUTO_HIDE|\
                                                          wxPD_APP_MODAL|wxPD_ELAPSED_TIME|\
                                                          wxPD_ESTIMATED_TIME|wxPD_REMAINING_TIME );
    string reducePath;

    if( tmpPath.length() > 40 && tmpPath.find_last_of( PATH_SEP ) != string::npos )
    {
      string file = tmpPath.substr( tmpPath.find_last_of( PATH_SEP ) );
      string tmp = tmpPath.substr( 0, tmpPath.find_last_of( PATH_SEP ) );
      if ( tmp.find_last_of( PATH_SEP ) != string::npos )
      {
        reducePath = "/..." + tmpPath.substr( tmp.find_last_of( PATH_SEP ),
                                              tmp.length() - tmp.find_last_of( PATH_SEP ) )
                     + file;
      }
      else
      {
        reducePath = "/..." + file;
      }
    }
    else
      reducePath = tmpPath;
    reducePath += "\t";

    paraverMain::dialogProgress->Pulse( wxString::FromAscii( reducePath.c_str() ) );
    paraverMain::dialogProgress->Fit();
    paraverMain::dialogProgress->Show();

    tr = Trace::create( localKernel, tmpPath, false, progress );
    tr->setShowProgressBar( traceSize / 1E6 > 10.0 );
    tr->setInstanceNumber( traceInstance[ std::string( tmpFileName.GetFullName().mb_str() ) ]++ );

    loadedTraces.push_back( tr );
    currentTrace = loadedTraces.size() - 1;
    setTraceWorkspaces( tr );

    wxTreeCtrl *newTree = createTree( imageList );
    newTree->Connect( wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler( paraverMain::OnTreeKeyPress ), NULL, this );

    if( paraverConfig->getGlobalFullTracePath() )
      choiceWindowBrowser->AddPage( newTree, wxString::FromAscii( tr->getFileNameNumbered().c_str() ) );
    else
      choiceWindowBrowser->AddPage( newTree, wxFileName( wxString::FromAscii( tr->getFileNameNumbered().c_str() ) ).GetFullName() );
    choiceWindowBrowser->ChangeSelection( choiceWindowBrowser->GetPageCount() - 1 );

    previousTraces->add( tmpPath );

    traceLoadedBefore = true;
    tracePath = tmpFileName.GetPath();
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

  setActiveWorkspacesText();
  refreshMenuHints();

  canServeSignal = true;

  // These are here because no UpdateUI for MenuBar
  wxUpdateUIEvent tmpEvent;
  OnMenuHintUpdate( tmpEvent );

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
    SaveOptions options;

    Trace *tmpTraceToUse = getCurrentTrace();

    if( !CFGLoader::loadCFG( localKernel, path, tmpTraceToUse,
                             newWindows, newHistograms, options ) )
    {
      wxString errMessage = wxString::FromAscii( path.c_str() ) + _( " failed to load in:\n'" ) + wxString::FromAscii( CFGLoader::errorLine.c_str() ) + _( "'" );
      wxMessageDialog message( this, errMessage, _( "Loading error" ), wxOK|wxICON_ERROR );
      raiseCurrentWindow = false;
      message.ShowModal();
      raiseCurrentWindow = true;
      for ( PRV_UINT32 i = 0; i < newHistograms.size(); ++i )
      {
        if ( newHistograms[ i ] != NULL )
          delete newHistograms[ i ];
      }
      newHistograms.clear();

      for ( PRV_UINT32 i = 0; i < newWindows.size(); ++i )
      {
        if ( newWindows[ i ] != NULL )
          delete newWindows[ i ];
      }
      newWindows.clear();
    }
    else
    {
      if( !CFGLoader::errorLine.empty() )
      {
        wxString errMessage = wxString::FromAscii( path.c_str() ) + _( " loaded with error in:\n'" ) + wxString::FromAscii( CFGLoader::errorLine.c_str() ) + _( "'" );
        wxMessageDialog message( this, errMessage, _( "Loading error" ), wxOK|wxICON_EXCLAMATION );
        raiseCurrentWindow = false;
        message.ShowModal();
        raiseCurrentWindow = true;
      }

      wxTreeItemId child;
      for( vector<Window *>::iterator it = newWindows.begin(); it != newWindows.end(); ++it )
      {
        wxTreeCtrl *allTracesPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 );
        wxTreeCtrl *currentPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( getTracePosition( tmpTraceToUse ) + 1 );

        if ( (*it)->getChild() == NULL )
          BuildTree( this, allTracesPage, allTracesPage->GetRootItem(), currentPage, currentPage->GetRootItem(), *it );

        if ( it + 1 == newWindows.end() )
        {
          currentTimeline = *it;
          currentHisto = NULL;
        }
      }

      int currentDisplay = wxDisplay::GetFromWindow( paraverMain::myParaverMain );
      for( vector<Histogram *>::iterator it = newHistograms.begin(); it != newHistograms.end(); ++it )
      {
        wxPoint tmpPos( (*it)->getPosX(), (*it)->getPosY() );
        if( wxDisplay::GetCount() > 1 /*&& ParaverConfig::???*/ )
        {
          if ( currentDisplay != wxNOT_FOUND && currentDisplay >= 0 )
          {
            wxDisplay tmpDisplay( currentDisplay );
            tmpPos.x += tmpDisplay.GetGeometry().x;
            tmpPos.y += tmpDisplay.GetGeometry().y;
            if( tmpPos.x != (*it)->getPosX() ) (*it)->setPosX( tmpPos.x );
            if( tmpPos.x != (*it)->getPosY() ) (*it)->setPosX( tmpPos.y );
          }
        }
#if wxMAJOR_VERSION<3 || !__WXGTK__
        gHistogram* tmpHisto = new gHistogram( this, wxID_ANY, wxString::FromAscii( (*it)->getName().c_str() ), tmpPos );
#else
        gHistogram* tmpHisto = new gHistogram( this, wxID_ANY, wxString::FromAscii( (*it)->getName().c_str() ) );
#endif
        tmpHisto->SetHistogram( *it );

        appendHistogram2Tree( tmpHisto );
        LoadedWindows::getInstance()->add( (*it) );

        tmpHisto->SetClientSize( wxSize( (*it)->getWidth(), (*it)->getHeight() ) );
        if( (*it)->getShowWindow() )
        {
#if !( wxMAJOR_VERSION<3 || !__WXGTK__ )
          tmpHisto->Move( tmpPos );
#endif
          tmpHisto->Show();
        }
        tmpHisto->execute();

        if ( it + 1 == newHistograms.end() )
        {
          currentTimeline = NULL;
          currentHisto = *it;
        }
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
    _( "Paraver trace (*.prv;*.prv.gz;*.csv)|*.prv;*.prv.gz;*.csv|All files (*.*)|*.*" ),
    wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_CHANGE_DIR );

  raiseCurrentWindow = false;
  if( dialog.ShowModal() == wxID_OK )
  {
    wxString path = dialog.GetPath();
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

  //wxFileDialog dialog( this, _( "Load Configuration" ), CFGPath, _( "" ),
  //                      _( "Paraver configuration file (*.cfg)|*.cfg|All files (*.*)|*.*" ),
  //                      wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_CHANGE_DIR );

  LoadCFGDialog dialog( this, CFGPath );

  raiseCurrentWindow = false;
  if( dialog.ShowModal() == wxID_OK )
  {
    wxString path = dialog.GetFilePath();
    CFGLoadedBefore = true;
    CFGPath = wxFileName( path ).GetPath();
    DoLoadCFG( std::string( path.mb_str() ) );
  }
  raiseCurrentWindow = true;
}

void paraverMain::OnMenuLoadAutoSavedSession( wxCommandEvent& event )
{
  #ifdef WIN32
  string file( ParaverConfig::getInstance()->getGlobalSessionPath() + "\\paraver.session" );
  #else
  string file( ParaverConfig::getInstance()->getGlobalSessionPath() + "/paraver.session" );
  #endif
  DoLoadSession( file );
}


void paraverMain::OnMenuLoadAutoSavedSessionSelect( wxCommandEvent& event )
{
  //MultiSessionLoad( true );
  #ifdef WIN32
    wxString folder( wxString( ParaverConfig::getInstance()->getGlobalSessionPath().c_str(), wxConvUTF8 )  + _( "\\AutosavedSessions" ) );
  #else
    wxString folder( wxString( ParaverConfig::getInstance()->getGlobalSessionPath().c_str(), wxConvUTF8 ) + _( "/AutosavedSessions" ) );
  #endif

  SessionSelectionDialog dialog( this, folder, true );
  if( dialog.ShowModal() == wxID_OK )
  {
    wxString path = dialog.GetSessionPath();
    wxString folderPath = path;
    folderPath.Replace( wxT( ".session" ), wxT( "_session" ) );
    SessionSaver::LoadSession( path );
  }
}


void paraverMain::exitManager( wxEvent& event )
{
  if ( !LoadedWindows::getInstance()->emptyWindows() )
  {
    /*
    wxMessageDialog questionDialog( this,
            wxT( "Some windows are already open... Do you want to save this session before closing?" ),
            wxT( "Please confirm" ),
            wxICON_QUESTION | wxYES_NO | wxCANCEL);
    Original layout of buttons:   No(2) | Cancel(3) | Yes(1) --> (1, 2, 3)
    Translated layout of buttons: Close without saving (2) | Cancel (3) | Save and Exit (1) --> (1, 2, 3)
    Current layout of buttons:    Save and Exit (1) | Cancel (3) |  Close without saving (2) --> (2, 1, 3)
    */

    ExitDialog questionDialog( this );
    int question = questionDialog.ShowModal();
    //if ( question == wxID_CANCEL || ( question == wxID_YES && !OnMenusavesession() ) )
    
    // This should be read as: do NOT exit Paraver if question is cancelled, 
    // or clicked save+exit and the session wasn't saved at the following menu
    if ( question == wxID_CANCEL || ( question == wxID_NO && !OnMenusavesession() ) )
    {
      return;
    }
  }
  PrepareToExit();
  Destroy();
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
 */

void paraverMain::OnExitClick( wxCommandEvent& event )
{
  exitManager( event );
}

/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_PARAVERMAIN
 */

void paraverMain::OnCloseWindow( wxCloseEvent& event )
{
  if ( event.CanVeto() )
  {
    exitManager( event );
  }
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
  if (name == wxT("icons/new_window.xpm"))
  {
    wxBitmap bitmap(application_star_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/new_derived_window.xpm"))
  {
    wxBitmap bitmap(application_add_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/new_histogram.xpm"))
  {
    wxBitmap bitmap(new_histogram_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/delete.xpm"))
  {
    wxBitmap bitmap(delete_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/cut_trace.xpm"))
  {
    wxBitmap bitmap(cut_trace_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/run_script.xpm"))
  {
    wxBitmap bitmap(run_script_xpm);
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
  if (name == wxT("icons/file_browser.xpm"))
  {
    wxIcon icon(file_browser_xpm);
    return icon;
  }
  else if (name == wxT("icons/window_properties.xpm"))
  {
    wxIcon icon(window_properties_xpm);
    return icon;
  }
  return wxNullIcon;
////@end paraverMain icon retrieval
}


void paraverMain::spreadSetChangedRecursive( Window *whichWindow )
{
  whichWindow->setChanged( true );

  if ( whichWindow->getChild() != NULL )
    spreadSetChangedRecursive( whichWindow->getChild() );

  set<Histogram *> tmpHistograms = whichWindow->getHistograms();
  for( set<Histogram *>::iterator it = tmpHistograms.begin(); it != tmpHistograms.end(); ++it )
    (*it)->setChanged( true );
}


void paraverMain::spreadSetChanged( Window *whichWindow )
{
  if ( isCFG4DModeDisabled() )
    whichWindow->setChanged( true );
  else
    spreadSetChangedRecursive( whichWindow );
}


void paraverMain::spreadSetRedrawRecursive( Window *whichWindow )
{
  whichWindow->setRedraw( true );

  if ( whichWindow->getChild() != NULL )
    spreadSetRedrawRecursive( whichWindow->getChild() );

  set<Histogram *> tmpHistograms = whichWindow->getHistograms();
  for( set<Histogram *>::iterator it = tmpHistograms.begin(); it != tmpHistograms.end(); ++it )
    (*it)->setRecalc( true );
}


void paraverMain::spreadSetRedraw( Window *whichWindow )
{
  if ( isCFG4DModeDisabled() )
    whichWindow->setRedraw( true );
  else
    spreadSetRedrawRecursive( whichWindow );
}

/*!
 * wxEVT_PG_CHANGING event handler for ID_FOREIGN
 */
void paraverMain::OnPropertyGridChanging( wxPropertyGridEvent& event )
{
  propertyPrevValue = event.GetProperty()->GetValue();
}

/*!
 * wxEVT_PG_CHANGED event handler for ID_FOREIGN
 */
void paraverMain::OnPropertyGridChange( wxPropertyGridEvent& event )
{
  wxPGProperty *property = event.GetProperty();
  if( property == NULL )
    return;

  if( property->GetDisplayedString() == _( "" ) )
  {
    property->SetValue( propertyPrevValue );
    return;
  }

  PropertyClientData *tmpClientData = (PropertyClientData *)property->GetClientData();
  if( tmpClientData == NULL )
    return;

  const wxString& propName = tmpClientData->propName;
  wxString *tmpRest = new wxString(_(""));
  
  if( propName == _( "Mode" ) )
  {
    if( tmpClientData->ownerTimeline != NULL )
    {
      tmpClientData->ownerTimeline->setCFG4DMode( property->GetValue().GetLong() == 1 );
      tmpClientData->ownerTimeline->setChanged( true );
    }
    else if ( tmpClientData->ownerHistogram != NULL )
    {
      tmpClientData->ownerHistogram->setCFG4DMode( property->GetValue().GetLong() == 1 );
      tmpClientData->ownerHistogram->setChanged( true );
    }
  }
  else if( propName == wxString( "Name", wxConvUTF8 ) )
  {
    wxString tmpName = property->GetValue().GetString();
    if( tmpClientData->ownerTimeline != NULL )
    {
      tmpClientData->ownerTimeline->setName( std::string( tmpName.mb_str() ) );
      tmpClientData->ownerTimeline->setChanged( true );
    }
    else if( tmpClientData->ownerHistogram != NULL )
    {
      tmpClientData->ownerHistogram->setName( std::string( tmpName.mb_str() ) );
      tmpClientData->ownerHistogram->setChanged( true );
    }
  }
  else if( propName == _( "Begin time" ) )
  {
    TTime tmpValue;
    if( tmpClientData->ownerTimeline != NULL )
    {
      bool done = LabelConstructor::getTimeValue( std::string( property->GetValue().GetString().mb_str()),
                                                  tmpClientData->ownerTimeline->getTimeUnit(),
                                                  ParaverConfig::getInstance()->getTimelinePrecision(),
                                                  tmpValue );

      if (!done)
        tmpValue = property->GetValue().GetDouble();

      tmpClientData->ownerTimeline->setWindowBeginTime( tmpClientData->ownerTimeline->windowUnitsToTraceUnits( tmpValue ) );

      // modify current zoom directly
      pair< TTime, TTime > zoomInfo = tmpClientData->ownerTimeline->getZoomFirstDimension();
      zoomInfo.first = property->GetValue().GetDouble(); // begin
      tmpClientData->ownerTimeline->setZoomFirstDimension( zoomInfo );

      tmpClientData->ownerTimeline->setChanged( true );
      tmpClientData->ownerTimeline->setRedraw( true );
    }
    else if( tmpClientData->ownerHistogram != NULL )
    {
      bool done = LabelConstructor::getTimeValue( std::string( property->GetValue().GetString().mb_str()),
                                                  tmpClientData->ownerHistogram->getControlWindow()->getTimeUnit(),
                                                  ParaverConfig::getInstance()->getTimelinePrecision(),
                                                  tmpValue );

      if (!done)
        tmpValue = property->GetValue().GetDouble();

      tmpClientData->ownerHistogram->setWindowBeginTime( tmpClientData->ownerHistogram->getControlWindow()->windowUnitsToTraceUnits( tmpValue ) );
      tmpClientData->ownerHistogram->setChanged( true );
      tmpClientData->ownerHistogram->setRecalc( true );
    }
  }
  else if( propName == _( "End time" ) )
  {
    TTime tmpValue;
    if( tmpClientData->ownerTimeline != NULL )
    {
      bool done = LabelConstructor::getTimeValue( std::string( property->GetValue().GetString().mb_str()),
                                                  tmpClientData->ownerTimeline->getTimeUnit(),
                                                  ParaverConfig::getInstance()->getTimelinePrecision(),
                                                  tmpValue );

      if (!done)
        tmpValue = property->GetValue().GetDouble();

      tmpClientData->ownerTimeline->setWindowEndTime( tmpClientData->ownerTimeline->windowUnitsToTraceUnits( tmpValue ) );

      // modify current zoom directly
      pair< TTime, TTime > zoomInfo = tmpClientData->ownerTimeline->getZoomFirstDimension();
      zoomInfo.second = property->GetValue().GetDouble(); // end
      tmpClientData->ownerTimeline->setZoomFirstDimension( zoomInfo );

      tmpClientData->ownerTimeline->setChanged( true );
      tmpClientData->ownerTimeline->setRedraw( true );
    }
    else if( tmpClientData->ownerHistogram != NULL )
    {
      bool done = LabelConstructor::getTimeValue( std::string( property->GetValue().GetString().mb_str()),
                                                  tmpClientData->ownerHistogram->getControlWindow()->getTimeUnit(),
                                                  ParaverConfig::getInstance()->getTimelinePrecision(),
                                                  tmpValue );

      if (!done)
        tmpValue = property->GetValue().GetDouble();

      tmpClientData->ownerHistogram->setWindowEndTime( tmpClientData->ownerHistogram->getControlWindow()->windowUnitsToTraceUnits( tmpValue ) );
      tmpClientData->ownerHistogram->setChanged( true );
      tmpClientData->ownerHistogram->setRecalc( true );
    }
  }
  else if( propName == _( "Semantic Minimum" ) )
  {
    double tmpValue = property->GetValue().GetDouble();
    tmpClientData->ownerTimeline->setMinimumY( tmpValue );
    tmpClientData->ownerTimeline->setRedraw( true );
  }
  else if( propName == _( "Semantic Maximum" ) )
  {
    double tmpValue = property->GetValue().GetDouble();
    tmpClientData->ownerTimeline->setMaximumY( tmpValue );
    tmpClientData->ownerTimeline->setRedraw( true );
  }
  // Control Window related properties
  else if( propName == _( "ControlWindow" ) )
  {
    tmpClientData->ownerHistogram->setControlWindow( ( ( prvTimelineTreeProperty * )property )->getSelectedWindow() );
    tmpClientData->ownerHistogram->setRecalc( true );
  }
  else if( propName == _( "ControlMinimum" ) )
  {
    tmpClientData->ownerHistogram->setControlMin( property->GetValue().GetDouble() );

    // modify current zoom directly
    pair< HistogramProxy::TZoomInfo, HistogramProxy::TZoomInfo > zoomInfo = tmpClientData->ownerHistogram->getZoomFirstDimension();
    zoomInfo.first.begin = property->GetValue().GetDouble(); // minimum
    tmpClientData->ownerHistogram->setZoomFirstDimension( zoomInfo );

    tmpClientData->ownerHistogram->setCompute2DScale( false );
    tmpClientData->ownerHistogram->setRecalc( true );
  }
  else if( propName == _( "ControlMaximum" ) )
  {
    tmpClientData->ownerHistogram->setControlMax( property->GetValue().GetDouble() );

    // modify current zoom directly
    pair< HistogramProxy::TZoomInfo, HistogramProxy::TZoomInfo > zoomInfo = tmpClientData->ownerHistogram->getZoomFirstDimension();
    zoomInfo.first.end = property->GetValue().GetDouble(); // maximum
    tmpClientData->ownerHistogram->setZoomFirstDimension( zoomInfo );

    tmpClientData->ownerHistogram->setCompute2DScale( false );
    tmpClientData->ownerHistogram->setRecalc( true );
  }
  else if( propName == _( "ControlDelta" ) )
  {
    if( property->GetValue().GetDouble() == 0 )
    {
      property->SetValue( tmpClientData->ownerHistogram->getControlDelta() );
      delete tmpRest;
      return;
    }
    tmpClientData->ownerHistogram->setControlDelta( property->GetValue().GetDouble() );

    // modify current zoom directly
    pair< HistogramProxy::TZoomInfo, HistogramProxy::TZoomInfo > zoomInfo = tmpClientData->ownerHistogram->getZoomFirstDimension();
    zoomInfo.second.begin = property->GetValue().GetDouble(); // delta
    tmpClientData->ownerHistogram->setZoomFirstDimension( zoomInfo );

    tmpClientData->ownerHistogram->setCompute2DScale( false );
    tmpClientData->ownerHistogram->setRecalc( true );
  }
  // Data Window related properties
  else if( propName == _( "DataWindow" ) )
  {
    tmpClientData->ownerHistogram->setDataWindow( ( ( prvTimelineTreeProperty * )property )->getSelectedWindow() );
    tmpClientData->ownerHistogram->setRecalc( true );
  }
  else if( propName == _( "DataMinimum" ) )
  {
    tmpClientData->ownerHistogram->setMinGradient( property->GetValue().GetDouble() );
    tmpClientData->ownerHistogram->setComputeGradient( false );
    tmpClientData->ownerHistogram->setRedraw( true );
  }
  else if( propName == _( "DataMaximum" ) )
  {
    tmpClientData->ownerHistogram->setMaxGradient( property->GetValue().GetDouble() );
    tmpClientData->ownerHistogram->setComputeGradient( false );
    tmpClientData->ownerHistogram->setRedraw( true );
  }
  // Histogram related properties
  else if( propName == wxString( "Type", wxConvUTF8 ) )
  {
    if( property->GetValue().GetLong() == 0 )
      tmpClientData->ownerHistogram->setCurrentStat( tmpClientData->ownerHistogram->getFirstCommStatistic() );
    else
      tmpClientData->ownerHistogram->setCurrentStat( tmpClientData->ownerHistogram->getFirstStatistic() );
    tmpClientData->ownerHistogram->setRedraw( true );
    tmpClientData->ownerHistogram->setChanged( true );
  }
  else if( propName == _( "Statistic" ) )
  {
    bool getOriginalList = ( !tmpClientData->ownerHistogram->getCFG4DEnabled() || !tmpClientData->ownerHistogram->getCFG4DMode() );
    if ( getOriginalList )
    {
      tmpClientData->ownerHistogram->setCurrentStat( std::string( property->GetDisplayedString().mb_str() ) );
    }
    else
    {
      map< string, string > statList( tmpClientData->ownerHistogram->getCFG4DStatisticsAliasList() );
      string selected( std::string( property->GetDisplayedString().mb_str() ) );
      for ( map< string, string >::iterator it = statList.begin(); it != statList.end(); ++it )
      {
        if ( it->second == selected )
        {
          selected = it->first;
        }
      }
      tmpClientData->ownerHistogram->setCurrentStat( selected );
    }

    tmpClientData->ownerHistogram->setRedraw( true );
  }
  else if( propName == _( "3D3rdWindow" ) )
  {
    if( ( ( prvTimelineTreeProperty * )property )->getSelectedWindow() == NULL )
      tmpClientData->ownerHistogram->clearExtraControlWindow();
    else
      tmpClientData->ownerHistogram->setExtraControlWindow( ( ( prvTimelineTreeProperty * )property )->getSelectedWindow() );
    tmpClientData->ownerHistogram->setRecalc( true );
  }
  else if( propName == _( "3DMinimum" ) )
  {
    tmpClientData->ownerHistogram->setExtraControlMin( property->GetValue().GetDouble() );
    tmpClientData->ownerHistogram->setCompute3DScale( false );
    tmpClientData->ownerHistogram->setRecalc( true );
  }
  else if( propName == _( "3DMaximum" ) )
  {
    tmpClientData->ownerHistogram->setExtraControlMax( property->GetValue().GetDouble() );
    tmpClientData->ownerHistogram->setCompute3DScale( false );
    tmpClientData->ownerHistogram->setRecalc( true );
  }
  else if( propName == _( "3DDelta" ) )
  {
    if( property->GetValue().GetDouble() == 0 )
    {
      property->SetValue( tmpClientData->ownerHistogram->getExtraControlDelta() );
      delete tmpRest;
      return;
    }

    tmpClientData->ownerHistogram->setExtraControlDelta( property->GetValue().GetDouble() );
    tmpClientData->ownerHistogram->setCompute3DScale( false );
    tmpClientData->ownerHistogram->setRecalc( true );
  }
  else if( propName == _( "3DPlane" ) )
  {
    tmpClientData->ownerHistogram->setSelectedPlane( property->GetValue().GetLong() );
    tmpClientData->ownerHistogram->setRedraw( true );
  }

  // Timeline related properties
  else if( propName == _( "Level" ) )
  {
    tmpClientData->ownerTimeline->setLevel( (TWindowLevel)property->GetValue().GetLong() );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Time Unit" ) )
  {
    tmpClientData->ownerTimeline->setTimeUnit( (TWindowLevel)property->GetValue().GetLong() );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );

  }
  else if( propName == _( "Logical" ) )
  {
    tmpClientData->ownerTimeline->getFilter()->setLogical( property->GetValue().GetBool() );
    tmpClientData->ownerTimeline->setRedraw( true );
  }
  else if( propName == _( "Physical" ) )
  {
    tmpClientData->ownerTimeline->getFilter()->setPhysical( property->GetValue().GetBool() );
    tmpClientData->ownerTimeline->setRedraw( true );
  }
  else if( propName == _( "Comm from.FromFunction" ) )
  {
    tmpClientData->ownerTimeline->getFilter()->setCommFromFunction( std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Comm from.From" ) )
  {
    prvRowsSelectionProperty *myProperty = (prvRowsSelectionProperty *)event.GetProperty();

    Filter *filter = tmpClientData->ownerTimeline->getFilter();
    filter->clearCommFrom();
    vector< TObjectOrder > selection;
    myProperty->GetSelectionAsVector( tmpClientData->ownerTimeline->getLevel(), selection );
    for( vector< TObjectOrder >::iterator it = selection.begin(); it != selection.end(); ++it )
    {
      filter->insertCommFrom( long(*it) );
    }

    spreadSetRedraw( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "FromToOp" ) )
  {
    long op = property->GetValue().GetLong();
    Filter *filter = tmpClientData->ownerTimeline->getFilter();
    if( op == 0 )
      filter->setOpFromToAnd();
    else if( op == 1 )
      filter->setOpFromToOr();

    spreadSetRedraw( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Comm to.ToFunction" ) )
  {
    tmpClientData->ownerTimeline->getFilter()->setCommToFunction( std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Comm to.To" ) )
  {
    prvRowsSelectionProperty *myProperty = (prvRowsSelectionProperty *)event.GetProperty();

    Filter *filter = tmpClientData->ownerTimeline->getFilter();
    filter->clearCommTo();
    vector< TObjectOrder > selection;
    myProperty->GetSelectionAsVector( tmpClientData->ownerTimeline->getLevel(), selection );
    for( vector< TObjectOrder >::iterator it = selection.begin(); it != selection.end(); ++it )
    {
      filter->insertCommTo( long(*it) );
    }

    spreadSetRedraw( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Comm tag.TagFunction" ) )
  {
    tmpClientData->ownerTimeline->getFilter()->setCommTagFunction( std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Comm tag.Tag" ) )
  {
    Filter *filter = tmpClientData->ownerTimeline->getFilter();
    filter->clearCommTags();
    wxArrayString value = property->GetValue().GetArrayString();
    for( unsigned int idx = 0; idx < value.GetCount(); idx++ )
    {
      long long tmpLong;
      value[ idx ].ToLongLong( &tmpLong );
      filter->insertCommTag( tmpLong );
    }

    spreadSetRedraw( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "TagSizeOp" ) )
  {
    long op = property->GetValue().GetLong();
    Filter *filter = tmpClientData->ownerTimeline->getFilter();
    if( op == 0 )
      filter->setOpTagSizeAnd();
    else if( op == 1 )
      filter->setOpTagSizeOr();

    spreadSetRedraw( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Comm size.SizeFunction" ) )
  {
    tmpClientData->ownerTimeline->getFilter()->setCommSizeFunction( std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Comm size.Size" ) )
  {
    Filter *filter = tmpClientData->ownerTimeline->getFilter();
    filter->clearCommSizes();
    wxArrayString value = property->GetValue().GetArrayString();
    for( unsigned int idx = 0; idx < value.GetCount(); idx++ )
    {
      long long tmpLong;
      value[ idx ].ToLongLong( &tmpLong );
      // cout << value[idx] << endl;
      filter->insertCommSize( tmpLong );
    }

    spreadSetRedraw( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Comm bandwidth.BWFunction" ) )
  {
    tmpClientData->ownerTimeline->getFilter()->setBandWidthFunction( std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Comm bandwidth.Bandwidth" ) )
  {
    Filter *filter = tmpClientData->ownerTimeline->getFilter();
    filter->clearBandWidth();
    wxArrayString value = property->GetValue().GetArrayString();
    for( unsigned int idx = 0; idx < value.GetCount(); idx++ )
    {
      double tmpDouble;
      value[ idx ].ToDouble( &tmpDouble );
      filter->insertBandWidth( tmpDouble );
    }

    spreadSetRedraw( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Event type.TypeFunction" ) )
  {
    tmpClientData->ownerTimeline->getFilter()->setEventTypeFunction( std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Event type.Types" ) )
  {
    Filter *filter = tmpClientData->ownerTimeline->getFilter();
    filter->clearEventTypes();
    wxArrayInt value = ( (prvEventTypeProperty *) property )->GetValueAsArrayInt();
    for( unsigned int idx = 0; idx < value.GetCount(); idx++ )
      filter->insertEventType( value[ idx ] );

    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "TypeValueOp" ) )
  {
    long op = property->GetValue().GetLong();
    Filter *filter = tmpClientData->ownerTimeline->getFilter();
    if( op == 0 )
      filter->setOpTypeValueAnd();
    else if( op == 1 )
      filter->setOpTypeValueOr();

    spreadSetRedraw( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Event value.ValueFunction" ) )
  {
    tmpClientData->ownerTimeline->getFilter()->setEventValueFunction( std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Event value.Values" ) )
  {
    Filter *filter = tmpClientData->ownerTimeline->getFilter();
    filter->clearEventValues();

    wxArrayString value = property->GetValue().GetArrayString();
    for( unsigned int idx = 0; idx < value.GetCount(); idx++ )
    {
      double tmpDouble;
      value[ idx ].ToDouble( &tmpDouble );
      filter->insertEventValue( tmpDouble );
    }

    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName.StartsWith( _( "Extra Top Compose " ), tmpRest ) )
  {
    // propName = "Extra Top Compose 1"
    // startwith  "Extra Top Compose "
    // tmpRest  =                   "1"
    //                               |
    //                               L pos
    size_t position;
    unsigned long tmpPos;
    tmpRest->ToULong( &tmpPos );
    position = (size_t)tmpPos;

    int reversedIndex = (int)position;
    int maxPos = (int)tmpClientData->ownerTimeline->getExtraNumPositions( TOPCOMPOSE1 );
    position = (size_t)(maxPos - reversedIndex);

    tmpClientData->ownerTimeline->setExtraLevelFunction( TOPCOMPOSE1, position, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Top Compose 1" ) )
  {
    tmpClientData->ownerTimeline->setLevelFunction( TOPCOMPOSE1, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Top Compose 2" ) )
  {
    tmpClientData->ownerTimeline->setLevelFunction( TOPCOMPOSE2, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Factor #1" ) )
  {
    tmpClientData->ownerTimeline->setFactor( 0, property->GetValue().GetDouble() );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Derived" ) )
  {
    tmpClientData->ownerTimeline->setLevelFunction( DERIVED, std::string( property->GetDisplayedString().mb_str() ) );

    // Change Derived operation Icon: find icon in list
    int iconNumber = getIconNumber( tmpClientData->ownerTimeline );

    // Change Derived operation Icon: set both trees, global and trace tree
    wxTreeCtrl *globalTreePage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 );
    wxTreeCtrl *currentTraceTreePage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( currentTrace + 1 );
    bool found;
    wxTreeItemId idInGlobalTree = getItemIdFromWindow( globalTreePage->GetRootItem(), tmpClientData->ownerTimeline, found );
    globalTreePage->SetItemImage( idInGlobalTree, iconNumber );
    wxTreeItemId idInCurrentTraceTree = getItemIdFromWindow( currentTraceTreePage->GetRootItem(), tmpClientData->ownerTimeline, found );
    currentTraceTreePage->SetItemImage( idInCurrentTraceTree, iconNumber );

    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Factor #2" ) )
  {
    tmpClientData->ownerTimeline->setFactor( 1, property->GetValue().GetDouble() );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Compose Workload" ) )
  {
    tmpClientData->ownerTimeline->setLevelFunction( COMPOSEWORKLOAD, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Workload" ) )
  {
    tmpClientData->ownerTimeline->setLevelFunction( WORKLOAD, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Compose Appl" ) )
  {
    tmpClientData->ownerTimeline->setLevelFunction( COMPOSEAPPLICATION, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Application" ) )
  {
    tmpClientData->ownerTimeline->setLevelFunction( APPLICATION, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Compose Task" ) )
  {
    tmpClientData->ownerTimeline->setLevelFunction( COMPOSETASK, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Task" ) )
  {
    tmpClientData->ownerTimeline->setLevelFunction( TASK, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Compose System" ) )
  {
    tmpClientData->ownerTimeline->setLevelFunction( COMPOSESYSTEM, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "System" ) )
  {
    tmpClientData->ownerTimeline->setLevelFunction( SYSTEM, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Compose Node" ) )
  {
    tmpClientData->ownerTimeline->setLevelFunction( COMPOSENODE, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Node" ) )
  {
    tmpClientData->ownerTimeline->setLevelFunction( NODE, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Compose CPU" ) )
  {
    tmpClientData->ownerTimeline->setLevelFunction( COMPOSECPU, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "CPU" ) )
  {
    tmpClientData->ownerTimeline->setLevelFunction( CPU, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Compose Thread" ) )
  {
    tmpClientData->ownerTimeline->setLevelFunction( COMPOSETHREAD, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName == _( "Thread" ) )
  {
    tmpClientData->ownerTimeline->setLevelFunction( THREAD, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( tmpClientData->ownerTimeline );
    spreadSetChanged( tmpClientData->ownerTimeline );
  }
  else if( propName.StartsWith( _( "Extra Param " ), tmpRest ) )
  {
    // propName = "Extra Param 1 2 3"
    // startwith  "Extra Param "
    // *tmpRest =             "1 2 3"
    //                         | | |
    //                         | | L pos
    //                         | L function level
    //                         L param index
    wxString paramData = *tmpRest;
    TParamIndex paramIdx;
    TWindowLevel functionLevel;
    size_t position;
    unsigned long tmpLong;

    paramData.BeforeFirst( ' ' ).ToULong( &tmpLong );
    paramIdx = tmpLong;

    paramData.AfterFirst( ' ' ).BeforeFirst( ' ' ).ToULong( &tmpLong );
    functionLevel = (TWindowLevel)tmpLong;

    paramData.AfterLast( ' ' ).ToULong( &tmpLong );
    int reversedIndex = (int)tmpLong;
    int maxPos = (int)tmpClientData->ownerTimeline->getExtraNumPositions( TOPCOMPOSE1 );
    position = (size_t)(maxPos - reversedIndex);

    wxArrayString valuesStr = property->GetValue().GetArrayString();
    TParamValue values;
    for( unsigned int idx = 0; idx < valuesStr.GetCount(); idx++ )
    {
      double tmpDouble;
      valuesStr[ idx ].ToDouble( &tmpDouble );
      values.push_back( tmpDouble );
    }

    tmpClientData->ownerTimeline->setExtraFunctionParam( functionLevel, position, paramIdx, values );
    spreadSetRedraw( tmpClientData->ownerTimeline );
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
    tmpClientData->ownerTimeline->setFunctionParam( functionLevel, paramIdx, values );
    spreadSetRedraw( tmpClientData->ownerTimeline );
  }

  delete tmpRest;
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

  if( choiceWindowBrowser->GetSelection() == 0 )
  {
    refreshMenuHints();
    setActiveWorkspacesText();
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
      string auxFile = std::string( fileName.GetFullPath().mb_str() );
      if( localKernel->isTraceFile( auxFile ))
        DoLoadTrace( auxFile );
      else if( CFGLoader::isCFGFile( auxFile ) && loadedTraces.size() > 0 )
        DoLoadCFG( auxFile );
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
    if( tmpHisto->getShowWindow() )
      histo->Raise();
  }
  else if( gTimeline *timeline = itemData->getTimeline() )
  {
    Window *tmpWin = timeline->GetMyWindow();

    beginDragWindow = timeline->GetMyWindow();

    tmpWin->setShowWindow( !tmpWin->getShowWindow() );
    if( tmpWin->getShowWindow() )
      timeline->Raise();
  }
}


/*!
 * wxEVT_TREE_ITEM_ACTIVATED event handler for wxID_ANY
 */
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
 * wxEVT_KEY_DOWN event handler for wxID_ANY
 */
void paraverMain::OnTreeKeyPress( wxKeyEvent& event )
{
  wxTreeCtrl *currentTree = (wxTreeCtrl *) choiceWindowBrowser->GetPage( choiceWindowBrowser->GetSelection() );

  if ( event.GetKeyCode() == WXK_F2 && !currentTree->IsEmpty() )
    currentTree->EditLabel( currentTree->GetSelection() );
}


/*!
 * wxEVT_TREE_END_LABEL_EDIT event handler for wxID_ANY
 */
void paraverMain::OnTreeEndLabelRename( wxTreeEvent& event )
{
  wxTreeCtrl *currentTree = (wxTreeCtrl *) choiceWindowBrowser->GetPage( choiceWindowBrowser->GetSelection() );
  if ( !event.IsEditCancelled() )
  {
    TreeBrowserItemData *itemData = static_cast<TreeBrowserItemData *>( 
        currentTree->GetItemData( currentTree->GetSelection() ) );

    if( gHistogram *histo = itemData->getHistogram() )
    {
      histo->GetHistogram()->setName( std::string( event.GetLabel().mb_str() ) );
      histo->GetHistogram()->setChanged( true );
    }
    else if( gTimeline *timeline = itemData->getTimeline() )
    {
      timeline->GetMyWindow()->setName( std::string( event.GetLabel().mb_str() ) );
      timeline->GetMyWindow()->setChanged( true );
    }
  }
}


gTimeline * paraverMain::GetSelectedTimeline()
{
  wxTreeCtrl *currentTree = (wxTreeCtrl *) choiceWindowBrowser->GetPage( choiceWindowBrowser->GetSelection() );
  TreeBrowserItemData *itemData = static_cast<TreeBrowserItemData *>( currentTree->GetItemData( currentTree->GetSelection() ) );
  return itemData->getTimeline();
}


gHistogram * paraverMain::GetSelectedHistogram()
{
  wxTreeCtrl *currentTree = (wxTreeCtrl *) choiceWindowBrowser->GetPage( choiceWindowBrowser->GetSelection() );
  TreeBrowserItemData *itemData = static_cast<TreeBrowserItemData *>( currentTree->GetItemData( currentTree->GetSelection() ) );
  return itemData->getHistogram();
}


void paraverMain::renameTreeItem( ) 
{
  wxTreeCtrl *currentTree = (wxTreeCtrl *) choiceWindowBrowser->GetPage( choiceWindowBrowser->GetSelection() );
  if( !currentTree->IsEmpty() )
    currentTree->EditLabel( currentTree->GetSelection() );
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
  for( unsigned int iPage = 0; iPage < choiceWindowBrowser->GetPageCount(); ++iPage )
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
    if( currentTrace == -1 )
    {
      LoadedWindows::getInstance()->getAll( allWindows );
      LoadedWindows::getInstance()->getAll( allHistograms );
    }
    else
    {
      LoadedWindows::getInstance()->getAll( loadedTraces[ currentTrace ], allWindows );
      LoadedWindows::getInstance()->getAll( loadedTraces[ currentTrace ], allHistograms );
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

    updateHistogramProperties( windowProperties, currentHisto, propertiesClientData );
  }
  else if( currentTimeline != NULL )
  {
    lastHisto = NULL;
    if( lastTimeline == currentTimeline && !currentTimeline->getChanged() )
      return;
    lastTimeline = currentTimeline;

    updateTimelineProperties( windowProperties, currentTimeline, propertiesClientData );
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


void paraverMain::OnPreviousSessionsClick( wxCommandEvent& event )
{
  int eventId = event.GetId();
  int i = 0;

  wxMenuItem *item = menuFile->FindItem( ID_RECENTSESSIONS );
  wxMenu *menu = item->GetSubMenu();
  wxMenuItemList& menuItems = menu->GetMenuItems();
  wxMenuItemList::iterator menuIt = menuItems.begin(); ++menuIt; ++menuIt; //begin + 2 (load autosave + separator)

  for ( menuIt ; menuIt != menuItems.end(); ++menuIt )
  {
    wxMenuItem *tmp = *menuIt;
    int currentId = tmp->GetId();
     if ( currentId == eventId )
      DoLoadSession( previousSessions->getFiles()[i] );
     
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
 * wxEVT_UPDATE_UI event handler for ID_RECENTSESSIONS
 */

void paraverMain::OnRecentsessionsUpdate( wxUpdateUIEvent& event )
{
  vector<string> v = previousSessions->getFiles();

  wxMenuItem *tmpItem = menuFile->FindItem( ID_RECENTSESSIONS );
  wxMenu *menuSessions = tmpItem->GetSubMenu();

  wxMenuItemList& menuItems = menuSessions->GetMenuItems();
  wxMenuItemList::iterator menuIt = menuItems.begin();

  if ( v.begin() == v.end() && menuItems.size() == 0 )
  {
    if ( ParaverConfig::getInstance()->getGlobalSingleInstance() )
    {
      wxMenuItem *tmp0 = new wxMenuItem( menuSessions, wxID_ANY, _("Load Auto-Saved Session") );
      menuSessions->Append( tmp0 );
      Connect( tmp0->GetId(),
               wxEVT_COMMAND_MENU_SELECTED,
               (wxObjectEventFunction) &paraverMain::OnMenuLoadAutoSavedSession );
      tmp0->Enable( ParaverConfig::getInstance()->getGlobalPrevSessionLoad() && ParaverConfig::getInstance()->getGlobalSessionSaveTime() != 0 );
    }
    else
    {
      wxMenuItem *tmp0 = new wxMenuItem( menuSessions, wxID_ANY, _("Select from Auto-Saved Sessions") );
      menuSessions->Append( tmp0 );
      Connect( tmp0->GetId(),
               wxEVT_COMMAND_MENU_SELECTED,
               (wxObjectEventFunction) &paraverMain::OnMenuLoadAutoSavedSessionSelect );
      tmp0->Enable( ParaverConfig::getInstance()->getGlobalPrevSessionLoad() && ParaverConfig::getInstance()->getGlobalSessionSaveTime() != 0 );
    }
    menuSessions->AppendSeparator();
  } 
  else
  {
    for ( vector<string>::iterator it = v.begin(); it != v.end(); ++it )
    {
      // Handler load auto-session options
      if ( menuItems.size() == 0 && it == v.begin() )
      {
        if ( ParaverConfig::getInstance()->getGlobalSingleInstance() )
        {
          wxMenuItem *tmp1 = new wxMenuItem( menuSessions, wxID_ANY, _("Load Auto-Saved Session") );
          menuSessions->Append( tmp1 );
          Connect( tmp1->GetId(),
                   wxEVT_COMMAND_MENU_SELECTED,
                   (wxObjectEventFunction) &paraverMain::OnMenuLoadAutoSavedSession );
          tmp1->Enable( ParaverConfig::getInstance()->getGlobalPrevSessionLoad() && ParaverConfig::getInstance()->getGlobalSessionSaveTime() != 0 );
        }
        else
        {
          wxMenuItem *tmp1 = new wxMenuItem( menuSessions, wxID_ANY, _("Select from Auto-Saved Sessions") );
          menuSessions->Append( tmp1 );
          Connect( tmp1->GetId(),
                   wxEVT_COMMAND_MENU_SELECTED,
                   (wxObjectEventFunction) &paraverMain::OnMenuLoadAutoSavedSessionSelect );
          tmp1->Enable( ParaverConfig::getInstance()->getGlobalPrevSessionLoad() && ParaverConfig::getInstance()->getGlobalSessionSaveTime() != 0 );
        }

        menuSessions->AppendSeparator();
      }
      else if ( menuItems.size() >= 2 && it == v.begin() )
      {
        wxMenuItem *tmp = *menuIt;
        tmp->Enable( ParaverConfig::getInstance()->getGlobalPrevSessionLoad() && ParaverConfig::getInstance()->getGlobalSessionSaveTime() != 0 );
        ++menuIt; ++menuIt;
      }

      // Item handler
      if ( menuIt == menuItems.end() )
      {
        wxMenuItem *newItem = new wxMenuItem( menuSessions, wxID_ANY, wxString::FromAscii( (*it).c_str() ) );
        menuSessions->Append( newItem );
        Connect( newItem->GetId(),
                 wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&paraverMain::OnPreviousSessionsClick );
      }
      else
      {
        wxMenuItem *tmp = *menuIt;
        tmp->SetItemLabel( wxString::FromAscii( (*it).c_str() ) );
        ++menuIt;
      }
    }
  }
}



/*!
 * wxEVT_UPDATE_UI event handler for ID_MENULOADCFG
 */

void paraverMain::OnMenuloadcfgUpdate( wxUpdateUIEvent& event )
{
  //event.Enable( loadedTraces.size() > 0 ); // why not this one?
  event.Enable( currentTrace != -1 );

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

void progressFunction( ProgressController *progress, void *callerWindow )
{
  int p;
  if ( progress->getCurrentProgress() > progress->getEndLimit() )
    p = numeric_limits<int>::max();
  else
    p = (int)floor( ( progress->getCurrentProgress() * numeric_limits<int>::max() ) / progress->getEndLimit() );

  wxString newMessage;
  if( progress->getMessageChanged() )
  {
    newMessage = wxString::FromAscii( progress->getMessage().c_str() );
    progress->clearMessageChanged();
  }

  if( !paraverMain::dialogProgress->Update( p, newMessage ) )
    progress->setStop( true );
//  app->Yield();
}

/*!
 * wxEVT_IDLE event handler for ID_PARAVERMAIN
 */

void paraverMain::OnIdle( wxIdleEvent& event )
{
#ifdef __WXMAC__
  wxUpdateUIEvent tmpEvent;
  OnRecenttracesUpdate( tmpEvent );
  OnMenuloadcfgUpdate( tmpEvent );
#endif

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
    size_t prevSize = loadedTraces.size();
    vector<Trace *>::iterator it = loadedTraces.begin();
    while( it != loadedTraces.end() )
    {
      if( (*it)->getUnload() )
      {
        vector<Window *> windows;
        vector<Histogram *> histograms;

        LoadedWindows::getInstance()->getAll( *it, windows );
        LoadedWindows::getInstance()->getAll( *it, histograms );

        if( windows.begin() == windows.end() && histograms.begin() == histograms.end() )
        {
          if( currentTrace == iTrace )
            currentTrace = -1;
          else if ( currentTrace > -1 )
            --currentTrace;

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
            ++tmpIt;
            it = tmpIt;
          }
          delete tmpTrace;
          choiceWindowBrowser->DeletePage( iTrace + 1 );

          continue;
        }
        else
          ++iTrace;
      }
      else
        ++iTrace;

      ++it;
    }
    if( currentTrace == -1 && loadedTraces.size() > 0 )
      currentTrace = loadedTraces.size() - 1;

    // These are here because no UpdateUI for MenuBar
    if( loadedTraces.size() != prevSize )
    {
      wxUpdateUIEvent tmpEvent;
      OnMenuHintUpdate( tmpEvent );
    }
  }

#ifndef WIN32
  if( signalQueue.size() > 0 )
    OnSignal();
#endif


  if( ParaverConfig::getInstance()->getGlobalSessionSaveTime() == 0 )
  {
    sessionTimer->Stop();
  }
  else if( ParaverConfig::getInstance()->getGlobalSessionSaveTime() > 0 &&
           sessionTimer->GetInterval() > 
           ParaverConfig::getInstance()->getGlobalSessionSaveTime() * 60e3 )
  {
    sessionTimer->Stop();
    sessionTimer->Start( ParaverConfig::getInstance()->getGlobalSessionSaveTime() * 60e3 );
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
  if( tree->GetSelection().IsOk() )
  {
    TreeBrowserItemData *item = (TreeBrowserItemData *) tree->GetItemData( tree->GetSelection() );
    if( item->getTimeline() != NULL )
    {
      currentWindow = item->getTimeline();
      currentTimeline = item->getTimeline()->GetMyWindow();
      beginDragWindow = currentTimeline;
      currentHisto = NULL;
    }
    else if( item->getHistogram() != NULL )
    {
      currentWindow = item->getHistogram();
      currentHisto = item->getHistogram()->GetHistogram();
      currentTimeline = NULL;
    }
  }

  setActiveWorkspacesText();
  refreshMenuHints();
}


void paraverMain::SaveConfigurationFile( wxWindow *parent,
                                          SaveOptions options,
                                          vector< Window * > timelines,
                                          vector< Histogram * > histograms )
{
  if ( !CFGLoadedBefore )
    CFGPath =  wxString::FromAscii( paraverConfig->getGlobalCFGsPath().c_str() );

  vector< wxString > extensions;
  extensions.push_back( wxT( "cfg" ) );

  FileDialogExtension dialog( parent,
                              _( "Save Configuration" ),
                              CFGPath,
                              _( "" ),
                              _( "Paraver configuration file (*.cfg)|*.cfg" ),
                              wxFD_SAVE | wxFD_CHANGE_DIR,
                              wxDefaultPosition,
                              wxDefaultSize,
                              _( "filedlg" ),
                              extensions );
  if( dialog.ShowModal() == wxID_OK )
  {
    CFGPath = dialog.GetPath();
    CFGLoadedBefore = true;

    CFGLoader::saveCFG( std::string( CFGPath.mb_str() ), options, timelines, histograms );
    previousCFGs->add( std::string( CFGPath.mb_str() ) );
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

  // Find trace to put in the selector
  Trace *selectedTrace = NULL;
  vector<Window *> auxWindows;
  vector<Histogram *> auxHistograms;

  int currentPage = choiceWindowBrowser->GetSelection();
  LoadedWindows::getInstance()->getAll( loadedTraces[ currentTrace ], auxWindows );
  LoadedWindows::getInstance()->getAll( loadedTraces[ currentTrace ], auxHistograms );

  if (( currentPage != 0 ) && ( auxWindows.size() > 0 || auxHistograms.size() > 0 ))
  {
    selectedTrace = loadedTraces[ currentTrace ];
  }
  saveDialog.SetInitialTrace( selectedTrace );


  raiseCurrentWindow = false;
  if( saveDialog.ShowModal() == wxID_OK )
  {
    timelines = saveDialog.GetSelectedTimelines();
    histograms = saveDialog.GetSelectedHistograms();
    options = saveDialog.GetOptions();

    SaveConfigurationFile( (wxWindow *)this, options, timelines, histograms );

    // Disable CFG4D once it is saved
    for( vector< Window * >::iterator it = timelines.begin(); it != timelines.end(); ++it )
    {
      (*it)->setCFG4DEnabled( false );
    }
    for( vector< Histogram * >::iterator it = histograms.begin(); it != histograms.end(); ++it )
    {
      (*it)->setCFG4DEnabled( false );
    }
  }
  raiseCurrentWindow = true;
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_MENUSAVECFG
 */

void paraverMain::OnMenusavecfgUpdate( wxUpdateUIEvent& event )
{
  /*
  if( LoadedWindows::getInstance()->emptyWindows() && 
      LoadedWindows::getInstance()->emptyHistograms() )
    event.Enable( false );
  else
    event.Enable( true );
  */
  bool areWindowsEmpty = LoadedWindows::getInstance()->emptyWindows() && 
      LoadedWindows::getInstance()->emptyHistograms();
  event.Enable( !areWindowsEmpty );
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL
 */
wxSize paraverMain::defaultWindowSize = wxSize( 600, 115 );
int paraverMain::initialPosX = 0;
int paraverMain::initialPosY = 0;


Window *paraverMain::createBaseWindow( wxString whichName )
{
  // Create new window
  Window *newWindow = Window::create( localKernel, getCurrentTrace() );
  ++numNewWindows;

  if ( whichName.IsEmpty() )
  {
    whichName = wxString( wxT( "New window #" ) );
    whichName << numNewWindows;
  }

  newWindow->setName( std::string( whichName.mb_str() ) );
  newWindow->setTimeUnit( loadedTraces[ currentTrace ]->getTimeUnit());
  newWindow->addZoom( 0, loadedTraces[ currentTrace ]->getEndTime(),
                      0, newWindow->getWindowLevelObjects() - 1 );

  // Position window in screen
  newWindow->setWidth( defaultWindowSize.GetWidth() ); // magic numbers!
  newWindow->setHeight( defaultWindowSize.GetHeight() );

  /*
  if ( initialPosX != 0 )
    initialPosX += defaultTitleBarSize.GetHeight();
  else
    initialPosX += GetSize().GetWidth();
  */

#ifdef __WXMAC__
  if( initialPosY < GetPosition().y )
    initialPosY = GetPosition().y;
#endif

  //newWindow->setPosX( initialPosX );
  newWindow->setPosX( GetNextPosX() );
  newWindow->setPosY( initialPosY );
  initialPosY += defaultTitleBarSize.GetHeight();

  // Its default semantic
  for ( PRV_UINT16 windowLevel = TOPCOMPOSE1; windowLevel <= COMPOSECPU; windowLevel++ )
    newWindow->setLevelFunction( (TWindowLevel)windowLevel, "As Is" );

  string semanticFunction = loadedTraces[ currentTrace ]->getDefaultSemanticFunc( THREAD );
  std::string traceFileName = loadedTraces[ currentTrace ]->getFileName() ;
  std::string traceFileExt = traceFileName.substr( traceFileName.length() - 3 ) ;
  if ( semanticFunction != "" )
    newWindow->setLevelFunction( THREAD, semanticFunction );
  else if ( traceFileExt == "csv" ) // csv
    newWindow->setLevelFunction( THREAD, "Last Evt Val" );
  else
    newWindow->setLevelFunction( THREAD, "State As Is" );

  return newWindow;
}


void paraverMain::insertInTree( Window *whichWindow )
{
  // Build gtimeline and append new window to windows tree
  wxTreeCtrl *allTracesPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 );
  wxTreeCtrl *currentPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( currentTrace + 1 );

  BuildTree( this,
             allTracesPage, allTracesPage->GetRootItem(),
             currentPage,   currentPage->GetRootItem(),
             whichWindow );
}


void paraverMain::OnToolNewWindowClick( wxCommandEvent& event )
{
  insertInTree( createBaseWindow() );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TOOL
 */

void paraverMain::OnToolNewWindowUpdate( wxUpdateUIEvent& event )
{
  tbarMain->EnableTool( ID_NEW_WINDOW, loadedTraces.size() > 0 );

  if ( currentTimeline != NULL )
    tbarMain->EnableTool( ID_NEW_WINDOW, true );
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEW_DERIVED_WINDOW
 */

void paraverMain::ShowDerivedDialog()
{
  DerivedTimelineDialog derivedDialog( this );
  vector<TWindowID> timelines;

  LoadedWindows::getInstance()->getDerivedCompatible( beginDragWindow->getTrace(), timelines );

  ++numNewDerived;
  wxString tmpName( _( "New Derived Window #" ) );
  tmpName << numNewDerived;
  derivedDialog.SetTimelineName( std::string( tmpName.mb_str() ) );

  // Set timelines list
  derivedDialog.SetTimelines1( timelines );
  derivedDialog.SetTimelines2( timelines );

  // Set current window
  derivedDialog.SetCurrentWindow1( beginDragWindow );
  if( endDragWindow == NULL )
  {
    if( LoadedWindows::getInstance()->getWindow( timelines[ 0 ] ) == beginDragWindow && timelines.size() > 1 )
      derivedDialog.SetCurrentWindow2( LoadedWindows::getInstance()->getWindow( timelines[ 1 ] ) );
    else
      derivedDialog.SetCurrentWindow2( LoadedWindows::getInstance()->getWindow( timelines[ 0 ] ) );
  }
  else
    derivedDialog.SetCurrentWindow2( endDragWindow );
    
  raiseCurrentWindow = false;
  if( derivedDialog.ShowModal() == wxID_OK )
  {
    beginDragWindow = derivedDialog.GetCurrentWindow1()->clone();
    beginDragWindow->setPosX( GetNextPosX() );
    beginDragWindow->setPosY( GetNextPosY() );

    endDragWindow = derivedDialog.GetCurrentWindow2()->clone();

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
    newWindow->setWidth( beginDragWindow->getWidth() );
    newWindow->setHeight( beginDragWindow->getHeight() );

    newWindow->setMaximumY( beginDragWindow->getMaximumY() );
    newWindow->setMinimumY( beginDragWindow->getMinimumY() );
    newWindow->setDrawCommLines( beginDragWindow->getDrawCommLines() );
    newWindow->setDrawFlags( beginDragWindow->getDrawFlags() );
    if( beginDragWindow->isCodeColorSet() )
      newWindow->setCodeColorMode();
    else if( beginDragWindow->isGradientColorSet() )
      newWindow->setGradientColorMode();
    else if( beginDragWindow->isNotNullGradientColorSet() )
      newWindow->setNotNullGradientColorMode();
    else if( beginDragWindow->isFunctionLineColorSet() )
      newWindow->setFunctionLineColorMode();

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
      currentWindow = last;
  }
  raiseCurrentWindow = true;
}


void paraverMain::ShowHistogramDialog()
{
  HistogramDialog histogramDialog( this );
  //HistogramDialog histogramDialog( NULL );

  vector<TWindowID> timelines;
  LoadedWindows::getInstance()->getAll( timelines );

  histogramDialog.SetControlTimelines( timelines );

  vector< pair< TRecordTime, TRecordTime > > ranges;
  // Window Times
  ranges.push_back( make_pair( currentTimeline->getWindowBeginTime(), currentTimeline->getWindowEndTime() ) );
  // Trace Times
  ranges.push_back( make_pair( 0.0, currentTimeline->getTrace()->getEndTime() ) );

  histogramDialog.SetTimeRange( ranges );

  histogramDialog.TransferDataToWindow( currentTimeline );

  raiseCurrentWindow = false;
  if( ParaverConfig::getInstance()->getHistogramSkipCreateDialog() || histogramDialog.ShowModal() == wxID_OK )
  {
    vector< TWindowID > controlTimeline = histogramDialog.GetControlTimelines();
    vector< TWindowID > dataTimeline = histogramDialog.GetDataTimelines();
    vector< TWindowID > extraControlTimeline;
    if( !ParaverConfig::getInstance()->getHistogramSkipCreateDialog() )
      extraControlTimeline = histogramDialog.GetExtraControlTimelines();

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

    if( ParaverConfig::getInstance()->getHistogramSkipCreateDialog() )
      newHistogram->setControlWindow( currentTimeline );
    else
      newHistogram->setControlWindow( histogramDialog.GetControlTimelineSelected() );

    if ( histogramDialog.GetControlTimelineAutofit() )
      newHistogram->setCompute2DScale( true );
    else
    {
      newHistogram->setCompute2DScale( false );

      newHistogram->setControlMin( histogramDialog.GetControlTimelineMin() );
      newHistogram->setControlMax( histogramDialog.GetControlTimelineMax() );
      newHistogram->setControlDelta( histogramDialog.GetControlTimelineDelta() );
    }

    if( ParaverConfig::getInstance()->getHistogramSkipCreateDialog() )
      newHistogram->setDataWindow( currentTimeline );
    else
      newHistogram->setDataWindow( histogramDialog.GetDataTimelineSelected() );

    if ( !extraControlTimeline.empty() )
    {
      newHistogram->setExtraControlWindow( histogramDialog.GetExtraControlTimelineSelected() );

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

    newHistogram->setPosX( GetNextPosX() );
    newHistogram->setPosY( initialPosY );
    initialPosY += defaultTitleBarSize.GetHeight();
    if( wxDisplay::GetCount() > 1 /*&& ParaverConfig::???*/ )
    {
      int currentDisplay = wxDisplay::GetFromWindow( paraverMain::myParaverMain );
      if ( currentDisplay != wxNOT_FOUND && currentDisplay >= 0 )
      {
        wxDisplay tmpDisplay( currentDisplay );
        newHistogram->setPosX( newHistogram->getPosX() + tmpDisplay.GetGeometry().x );
        newHistogram->setPosY( newHistogram->getPosY() + tmpDisplay.GetGeometry().y );
      }
    }

    tmpHisto->SetClientSize( wxRect( newHistogram->getPosX(), newHistogram->getPosY(),
                                     newHistogram->getWidth(), newHistogram->getHeight() ) );
    tmpHisto->Move( newHistogram->getPosX(), newHistogram->getPosY() );
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


int paraverMain::GetDefaultTitleBarHeight()
{
  return defaultTitleBarSize.GetHeight();
}


int paraverMain::GetNextPosX()
{
  if ( initialPosX != 0 )
    initialPosX += GetDefaultTitleBarHeight();
  else
    initialPosX += GetSize().GetWidth();

  return initialPosX;
}


int paraverMain::GetNextPosY()
{
  initialPosY += GetDefaultTitleBarHeight();

  return initialPosY;
}


void paraverMain::OnTreeEndDrag( wxTreeEvent& event )
{
  if( static_cast<wxTreeCtrl *>( event.GetEventObject() ) == dirctrlFiles->GetTreeCtrl() )
    return;

  if ( event.GetItem().IsOk() )
  {
    wxTreeCtrl *tmpTree = static_cast<wxTreeCtrl *>( event.GetEventObject() );
    TreeBrowserItemData *itemData = static_cast<TreeBrowserItemData *>( tmpTree->GetItemData( event.GetItem() ) );

    endDragWindow = NULL;

    if ( gTimeline *timeline = itemData->getTimeline())
    {
      endDragWindow = timeline->GetMyWindow();
      if( beginDragWindow->getTrace()->isSameObjectStruct( endDragWindow->getTrace(), endDragWindow->isLevelProcessModel() ) &&
          Window::compatibleLevels( beginDragWindow, endDragWindow ) )
      {
        ShowDerivedDialog();
      }
      else
        wxMessageBox( wxT( "Incompatible windows used to derive." ), wxT( "Warning" ), wxOK|wxICON_EXCLAMATION, this );
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
    if( currentTrace == -1 )
      LoadedWindows::getInstance()->getAll( timelines );
    else
      LoadedWindows::getInstance()->getAll( loadedTraces[ currentTrace ], timelines );
    tbarMain->EnableTool( ID_NEW_HISTOGRAM, ( timelines.size() > 0 ) && ( currentTimeline != NULL ) );
  }
  else
    tbarMain->EnableTool( ID_NEW_HISTOGRAM, false );
}

wxArrayString paraverMain::FromVectorStringToWxArray( std::vector< std::string > vec, std::string fileType ) 
{
  wxArrayString arr;
  for ( int i = 0 ; i < vec.size(); ++i )
  {
    wxString myWxStr( vec[ i ].c_str(), wxConvUTF8 );
    arr.Add( myWxStr );
  }
  return arr;
}

std::vector< std::string > paraverMain::FromWxArrayToVectorString( wxArrayString arr ) 
{
  std::vector< std::string > vec(0);
  for ( int i = 0 ; i < arr.size() ; ++i )
    vec.push_back( std::string( arr[ i ].mb_str() ) );

  return vec;
}

void paraverMain::ShowPreferences( wxWindowID whichPanelID )
{
  PreferencesDialog preferences( this );

  // GLOBAL
  preferences.SetGlobalFillStateGaps( paraverConfig->getGlobalFillStateGaps() );
  preferences.SetGlobalFullTracePath( paraverConfig->getGlobalFullTracePath() );
  preferences.SetTracesPath( paraverConfig->getGlobalTracesPath() );
  preferences.SetCfgsPath( paraverConfig->getGlobalCFGsPath() );
  preferences.SetTutorialsPath( paraverConfig->getGlobalTutorialsPath() );
  preferences.SetTmpPath( paraverConfig->getGlobalTmpPath() );
  preferences.SetMaximumTraceSize( paraverConfig->getFiltersFilterTraceUpToMB() );
  preferences.SetSingleInstance( paraverConfig->getGlobalSingleInstance() );
  preferences.SetSessionSaveTime( paraverConfig->getGlobalSessionSaveTime() );
  preferences.SetAskForPrevSessionLoad( paraverConfig->getGlobalPrevSessionLoad() );
  preferences.SetHelpContentsUsesBrowser( paraverConfig->getGlobalHelpContentsUsesBrowser() );
 
  // TIMELINE

  preferences.SetTimelineNameFormatPrefix( paraverConfig->getTimelineDefaultName() );
  preferences.SetTimelineNameFormatFull( paraverConfig->getTimelineNameFormat() );
  //preferences.GetTimelineDefaultCFG( paraverConfig->setTimelineDefaultCFG() );
  preferences.SetTimelineEventLines( paraverConfig->getTimelineViewEventsLines() );
  preferences.SetTimelineCommunicationLines( paraverConfig->getTimelineViewCommunicationsLines() );
  preferences.SetTimelineColor( ( PRV_UINT32 )paraverConfig->getTimelineColor() );
  preferences.SetTimelineGradientFunction( ( PRV_UINT32 )paraverConfig->getTimelineGradientFunction() );
  preferences.SetTimelineDrawmodeTime( ( PRV_UINT32 )paraverConfig->getTimelineDrawmodeTime() );
  preferences.SetTimelineDrawmodeObjects( ( PRV_UINT32 )paraverConfig->getTimelineDrawmodeObjects() );
  preferences.SetTimelinePixelSize( ( PRV_UINT32 )paraverConfig->getTimelinePixelSize() );
  preferences.SetTimelineObjectLabels( ( PRV_UINT32 )paraverConfig->getTimelineLabels() );
  preferences.SetTimelineObjectAxis( ( PRV_UINT32 )paraverConfig->getTimelineObjectAxisSize() );
  preferences.SetTimelineWWPrecision( paraverConfig->getTimelinePrecision() );
  preferences.SetTimelineWWSemantic( paraverConfig->getTimelineWhatWhereSemantic() );
  preferences.SetTimelineWWEvents( paraverConfig->getTimelineWhatWhereEvents() );
  preferences.SetTimelineWWCommunications( paraverConfig->getTimelineWhatWhereCommunications() );
  preferences.SetTimelineWWPreviousNext( paraverConfig->getTimelineWhatWherePreviousNext() );
  preferences.SetTimelineWWText( paraverConfig->getTimelineWhatWhereText() );
  preferences.SetTimelineWWEventPixels( paraverConfig->getTimelineWhatWhereEventPixels() );
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
  preferences.SetHistogramSkipCreateDialog( paraverConfig->getHistogramSkipCreateDialog() );

  // COLORS
  preferences.SetTimelineColourBackground( paraverConfig->getColorsTimelineBackground() );
  preferences.SetTimelineColourAxis( paraverConfig->getColorsTimelineAxis() );
  preferences.SetColorUseZero( paraverConfig->getColorsTimelineUseZero() );
  preferences.SetTimelineColourZero( paraverConfig->getColorsTimelineColorZero() );
  preferences.SetTimelineColourPunctual( paraverConfig->getColorsTimelinePunctual() );
  preferences.SetTimelineColourLogical( paraverConfig->getColorsTimelineLogicalCommunications() );
  preferences.SetTimelineColourPhysical( paraverConfig->getColorsTimelinePhysicalCommunications() );

  preferences.SetGradientColourBegin( paraverConfig->getColorsBeginGradient() );
  preferences.SetGradientColourEnd( paraverConfig->getColorsEndGradient() );
  preferences.SetGradientColourNegativeBegin( paraverConfig->getColorsBeginNegativeGradient() );
  preferences.SetGradientColourNegativeEnd( paraverConfig->getColorsEndNegativeGradient() );
  preferences.SetGradientColourLow( paraverConfig->getColorsLowGradient() );
  preferences.SetGradientColourTop( paraverConfig->getColorsTopGradient() );

  // FILTER
  preferences.SetFiltersXMLPath( paraverConfig->getFiltersXMLPath() );

  // EXTERNAL APPS
  wxArrayString externalTextEditors = paraverMain::FromVectorStringToWxArray( paraverConfig->getGlobalExternalTextEditors(), "txt" );
  wxArrayString externalPDFReaders = paraverMain::FromVectorStringToWxArray( paraverConfig->getGlobalExternalPDFReaders(), "pdf" );
  
  preferences.SetGlobalExternalTextEditors( externalTextEditors );
  preferences.SetGlobalExternalPDFReaders( externalPDFReaders );


  preferences.SetPanel( whichPanelID );

  // ShowModal calls it by default
  //preferences.TransferDataToWindow();

  raiseCurrentWindow = true;
  if ( preferences.ShowModal() == wxID_OK )
  {
    //It is called by default
    //preferences.TransferDataFromWindow();

    // Apply Preferences

    // GLOBAL
    paraverConfig->setGlobalFillStateGaps( preferences.GetGlobalFillStateGaps() );
    paraverConfig->setGlobalFullTracePath( preferences.GetGlobalFullTracePath() );
    paraverConfig->setGlobalTracesPath( preferences.GetTracesPath() );
    paraverConfig->setGlobalCFGsPath( preferences.GetCfgsPath() );
    paraverConfig->setGlobalTutorialsPath( preferences.GetTutorialsPath() );
    paraverConfig->setGlobalTmpPath( preferences.GetTmpPath() );
    paraverConfig->setFiltersFilterTraceUpToMB( (float)preferences.GetMaximumTraceSize() );
    paraverConfig->setGlobalSingleInstance( preferences.GetSingleInstance() );
    paraverConfig->setGlobalSessionSaveTime( preferences.GetSessionSaveTime() );
    paraverConfig->setGlobalPrevSessionLoad( preferences.GetAskForPrevSessionLoad() );
    paraverConfig->setGlobalHelpContentsUsesBrowser( preferences.GetHelpContentsUsesBrowser() );


    // TIMELINE
    paraverConfig->setTimelineDefaultName( preferences.GetTimelineNameFormatPrefix() );
    paraverConfig->setTimelineNameFormat( preferences.GetTimelineNameFormatFull() );
    //paraverConfig->setTimelineDefaultCFG( preferences.GetTimelineDefaultCFG() );
    paraverConfig->setTimelinePrecision( preferences.GetTimelineWWPrecision() );
    paraverConfig->setTimelineViewEventsLines( preferences.GetTimelineEventLines() );
    paraverConfig->setTimelineViewCommunicationsLines( preferences.GetTimelineCommunicationLines() );
    paraverConfig->setTimelineColor( (SemanticColor::TColorFunction)preferences.GetTimelineColor() );
    paraverConfig->setTimelineGradientFunction( (GradientColor::TGradientFunction)preferences.GetTimelineGradientFunction() );
    paraverConfig->setTimelineDrawmodeTime( (DrawModeMethod)preferences.GetTimelineDrawmodeTime() );
    paraverConfig->setTimelineDrawmodeObjects( (DrawModeMethod)preferences.GetTimelineDrawmodeObjects() );
    paraverConfig->setTimelinePixelSize( preferences.GetTimelinePixelSize() );
    paraverConfig->setTimelineLabels( (Window::TObjectLabels)preferences.GetTimelineObjectLabels() );
    paraverConfig->setTimelineObjectAxisSize( (Window::TObjectAxisSize)preferences.GetTimelineObjectAxis() );
    paraverConfig->setTimelineWhatWhereSemantic( preferences.GetTimelineWWSemantic() );
    paraverConfig->setTimelineWhatWhereEvents( preferences.GetTimelineWWEvents() );
    paraverConfig->setTimelineWhatWhereCommunications( preferences.GetTimelineWWCommunications() );
    paraverConfig->setTimelineWhatWherePreviousNext( preferences.GetTimelineWWPreviousNext() );
    paraverConfig->setTimelineWhatWhereText( preferences.GetTimelineWWText() );
    paraverConfig->setTimelineWhatWhereEventPixels( preferences.GetTimelineWWEventPixels() );
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
    paraverConfig->setHistogramSkipCreateDialog( preferences.GetHistogramSkipCreateDialog() );

    // COLORS
    paraverConfig->setColorsTimelineBackground( preferences.GetTimelineColourBackground() );
    paraverConfig->setColorsTimelineAxis( preferences.GetTimelineColourAxis() );
    paraverConfig->setColorsTimelineUseZero( preferences.GetColorUseZero() );
    paraverConfig->setColorsTimelineColorZero( preferences.GetTimelineColourZero() );
    paraverConfig->setColorsTimelinePunctual( preferences.GetTimelineColourPunctual() );
    paraverConfig->setColorsTimelineLogicalCommunications( preferences.GetTimelineColourLogical() );
    paraverConfig->setColorsTimelinePhysicalCommunications( preferences.GetTimelineColourPhysical() );

    paraverConfig->setColorsBeginGradient( preferences.GetGradientColourBegin() );
    paraverConfig->setColorsEndGradient( preferences.GetGradientColourEnd() );
    paraverConfig->setColorsBeginNegativeGradient( preferences.GetGradientColourNegativeBegin() );
    paraverConfig->setColorsEndNegativeGradient( preferences.GetGradientColourNegativeEnd() );
    paraverConfig->setColorsLowGradient( preferences.GetGradientColourLow() );
    paraverConfig->setColorsTopGradient( preferences.GetGradientColourTop() );

    vector<Window *> tmpWins;
    LoadedWindows::getInstance()->getAll( tmpWins );
    for( vector<Window *>::iterator it = tmpWins.begin(); it != tmpWins.end(); ++it )
    {
      (*it)->getGradientColor().setBeginGradientColor( preferences.GetGradientColourBegin() );
      (*it)->getGradientColor().setEndGradientColor( preferences.GetGradientColourEnd() );
      (*it)->getGradientColor().setBelowOutlierColor( preferences.GetGradientColourLow() );
      (*it)->getGradientColor().setAboveOutlierColor( preferences.GetGradientColourTop() );
    }

    // FILTER
    paraverConfig->setFiltersXMLPath( preferences.GetFiltersXMLPath() );

    // EXTERNAL APPS
    std::vector< std::string > vectorTextEditors = paraverMain::FromWxArrayToVectorString( preferences.GetGlobalExternalTextEditors() );
    std::vector< std::string > vectorPDFReaders = paraverMain::FromWxArrayToVectorString( preferences.GetGlobalExternalPDFReaders() );
    paraverConfig->setGlobalExternalTextEditors( vectorTextEditors );
    paraverConfig->setGlobalExternalPDFReaders( vectorPDFReaders );
    
    // Save Preferences to File
    paraverConfig->writeParaverConfigFile();

    // WORKSPACES
    workspacesManager->saveXML();

    for ( vector< Trace * >::iterator it = loadedTraces.begin(); it != loadedTraces.end(); ++it )
    {
      setTraceWorkspaces( *it );
    }

    setActiveWorkspacesText();
    refreshMenuHints();
  }
  raiseCurrentWindow = false;
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_PREFERENCES
 */

void paraverMain::OnPreferencesClick( wxCommandEvent& event )
{
  ShowPreferences();
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
  {
    if ( loadedTraces[ currentTrace ] == trace )
      break;
  }

  int currentPage  = choiceWindowBrowser->GetSelection();

  if ( currentPage != 0 )
    choiceWindowBrowser->SetSelection( currentTrace + 1 );

  SetCurrentTrace( currentTrace );
}


PRV_UINT16 paraverMain::getTracePosition( Trace *trace )
{
  PRV_UINT16 currentTrace;

  for ( currentTrace = 0; currentTrace < loadedTraces.size(); ++currentTrace )
  {
    if ( loadedTraces[ currentTrace ] == trace )
      break;
  }

  return currentTrace;
}


bool getUsedByHistogram( Window *whichWindow )
{
  if ( whichWindow->getUsedByHistogram() )
    return true;

  else if ( whichWindow->isDerivedWindow() )
  {
    if ( getUsedByHistogram( whichWindow->getParent( 0 ) ) )
      return true;

    return getUsedByHistogram( whichWindow->getParent( 1 ) );
  }
  return false;
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
    if( !getUsedByHistogram( currentTimeline ) )
    {
      if( currentTimeline->getChild() != NULL )
        wxMessageBox( _( "Cannot delete parent windows. Delete first derived window" ),
                      _( "Paraver information" ),
                      wxOK | wxICON_INFORMATION );
      else
        currentTimeline->setDestroy( true );
    }
    else
    {
      wxMessageBox( _( "Cannot delete windows used by histograms." ),
                    _( "Paraver information" ),
                    wxOK | wxICON_INFORMATION );
    }
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TOOLDELETE
 */
void paraverMain::OnTooldeleteUpdate( wxUpdateUIEvent& event )
{
  bool tmpEnableButtonDestroy;

  if( currentTimeline != NULL )
  {
    bool dummyFound;
    gTimeline *tmpTimeline = getGTimelineFromWindow( getAllTracesTree()->GetRootItem(), currentTimeline, dummyFound );
    tmpEnableButtonDestroy = tmpTimeline->getEnableDestroyButton();
  }
  else if ( currentHisto != NULL )
  {
    gHistogram *tmpHistogram = getGHistogramFromWindow( getAllTracesTree()->GetRootItem(), currentHisto );
    tmpEnableButtonDestroy = tmpHistogram->getEnableDestroyButton();
  }
  else // Both null
  {
    tmpEnableButtonDestroy = false;
  }

  event.Enable( tmpEnableButtonDestroy );
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_UNLOADTRACE
 */

void paraverMain::OnUnloadtraceClick( wxCommandEvent& event )
{
  wxArrayString choices;

  for( vector<Trace *>::iterator it = loadedTraces.begin(); it != loadedTraces.end(); ++it )
    choices.Add( wxString::FromAscii( (*it)->getTraceNameNumbered().c_str() ) );
  wxMultiChoiceDialog dialog( this, _("Select the traces to unload:"), _("Unload Traces"), choices );

  raiseCurrentWindow = false;

  dialog.ShowModal();
  if ( dialog.GetReturnCode() == wxID_OK )
  {
    wxArrayInt sel = dialog.GetSelections();
    for( size_t i = 0; i < sel.GetCount(); ++i )
      UnloadTrace( sel.Item( i ) );
  }

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

  traceWorkspaces.erase( loadedTraces[ whichTrace ] );
  firstUserWorkspace.erase( loadedTraces[ whichTrace ] );
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



void paraverMain::HandleMaxSessionFiles()
{
#ifdef WIN32
  wxString folder( 
        wxString( ParaverConfig::getInstance()->getGlobalSessionPath().c_str(), wxConvUTF8 ) +
        _( "\\AutosavedSessions" ) ) ;
#else
  wxString folder( 
        wxString( ParaverConfig::getInstance()->getGlobalSessionPath().c_str(), wxConvUTF8 ) +
        _( "/AutosavedSessions" ) ) ;
  wxString sessionFolder( _( "/tmp" ) ) ;
#endif
  
  wxArrayString filesInFolder, sessionFilesToRemove;
  if ( wxDirExists( folder ) )
  {
    wxDir::GetAllFiles( folder, &filesInFolder, wxT( "*.session" ), wxDIR_FILES );
    if ( filesInFolder.size() > CUTOFF )
    {
      map< boost::posix_time::ptime, wxString > dtToFile;
      for ( int i = 0 ; i < filesInFolder.size() ; ++i )
      {
        #ifdef WIN32
        wxString datetime = filesInFolder[ i ].AfterLast( '\\' ).AfterFirst( '_' ).Left( 15 );
        #else
        wxString datetime = filesInFolder[ i ].AfterLast( '/' ).AfterFirst( '_' ).Left( 15 );
        #endif
        datetime[ 8 ] = 'T';
        
        boost::posix_time::ptime dt( boost::posix_time::from_iso_string( std::string( datetime.mb_str() ) ) );
        dtToFile.insert( std::pair< boost::posix_time::ptime, wxString >( dt , filesInFolder[ i ] ) );
      }

      // Remove >=10 oldest auto-saved session files EXCEPT those in execution
      if( !ParaverConfig::getInstance()->getGlobalSingleInstance() )
      {
#ifdef WIN32
        // ST : Service Table
        wxDir wxd( wxT( "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\" ) );
        wxString service, serviceFlag = wxT( "wxparaver_service*" );
        std::map< wxString, wxString > serviceMap;
        bool cont = wxd.GetFirst( &service, serviceFlag );

        while ( cont )
        {
          wxString servicePID = service.AfterLast( '-' );
          wxString serviceName = wxT( "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\" ) + service;
          serviceMap.insert( std::pair< wxString, wxString >( servicePID, serviceName ) );
          cont = wxd.GetNext( &service );
        }
#else
        // ST : Service Table
        wxDir wxd( wxT( "/tmp/" ) );
        wxString service, serviceFlag = wxT( "wxparaver_service*" );
        std::map< wxString, wxString > serviceMap;
        bool cont = wxd.GetFirst( &service, serviceFlag );

        while ( cont )
        {
          wxString servicePID = service.AfterLast( '-' );
          wxString serviceName = wxT( "/tmp/" ) + service;
          
          serviceMap.insert( std::pair< wxString, wxString >( servicePID, serviceName ) );
          cont = wxd.GetNext( &service );
        }
#endif

        wxLogNull logNull;
        stClient *client = new stClient;
        wxString hostName = wxT( "localhost" );

        map< boost::posix_time::ptime, wxString >::iterator it = dtToFile.begin();
        for ( int deleteCtr = 0 ; deleteCtr < filesInFolder.size()-CUTOFF ; ++deleteCtr )
        {
          wxString folderToRemove = (*it).second; 
          folderToRemove.Replace( wxT( ".session" ), wxT( "_session" ) );

          wxString autoSessionPID = folderToRemove.BeforeFirst( '_' ).AfterLast( 's' );
          autoSessionPID.Replace( wxT( "ps" ), wxT( "" ) );

          wxString serviceName = serviceMap[ autoSessionPID ];
          wxConnectionBase *connection = client->MakeConnection( hostName, serviceName, wxT( "wxparaver" ) );

          if ( connection == NULL )
          {
            if ( wxDirExists( folderToRemove ) && wxFileExists( (*it).second ) )
            {
              wxDir::GetAllFiles( folderToRemove, &sessionFilesToRemove, wxT( "" ), wxDIR_FILES );
              for ( int iFile = 0 ; iFile < sessionFilesToRemove.size() ; ++iFile ) 
                wxRemoveFile( sessionFilesToRemove[ iFile ] );
              sessionFilesToRemove.Clear();
            } 
            wxRemoveFile( (*it).second );
            wxRmdir( folderToRemove );
          }
          delete connection;
          ++it;
        }
        delete client;
      }
    }
  }
}

void paraverMain::PrepareToExit()
{
  //Saves session before exit
  if( !ParaverConfig::getInstance()->getGlobalSingleInstance() )
  {
    stringstream strPid;
    stringstream strStatus;
    strPid << sessionInfo.pid;
    strStatus << sessionInfo.status;
  #ifdef WIN32

    wxString file( wxString( std::string( ParaverConfig::getInstance()->getGlobalSessionPath() +
                                          "\\AutosavedSessions" +
                                          "\\ps" +
                                          strPid.str() +
                                          "_" +
                                          sessionInfo.sessionDate +
                                          "_" +
                                          strStatus.str() +
                                          ".session" ).c_str(), wxConvUTF8 ) );

    wxString folder( wxString( std:string( ParaverConfig::getInstance()->getGlobalSessionPath() +
                                           "\\AutosavedSessions" +
                                           "\\ps" +
                                           strPid.str() +
                                           "_" +
                                           sessionInfo.sessionDate +
                                           "_" +
                                           strStatus.str() +
                                           "_session" ).c_str(), wxConvUTF8 ) );
  #else
    wxString file( wxString( std::string( ParaverConfig::getInstance()->getGlobalSessionPath() + 
                                          "/AutosavedSessions" +
                                          "/ps" +
                                          strPid.str() +
                                          "_" +
                                          sessionInfo.sessionDate +
                                          "_" +
                                          strStatus.str() +
                                          ".session" ).c_str(), wxConvUTF8 ) );

    wxString folder( wxString( std::string( ParaverConfig::getInstance()->getGlobalSessionPath() +
                                            "/AutosavedSessions" +
                                            "/ps" +
                                            strPid.str() +
                                            "_" +
                                            sessionInfo.sessionDate +
                                            "_" +
                                            strStatus.str() +
                                            "_session" ).c_str(), wxConvUTF8 ) );
  #endif
    
    sessionInfo.status = SessionInfo::CLOSED;

    if ( wxFileExists( file ) && wxDirExists( folder ) )
    {
      wxRemoveFile( file );

      wxArrayString filesInFolder;
      wxDir::GetAllFiles( folder, &filesInFolder, wxT( "*" ), wxDIR_FILES );
      for ( wxArrayString::iterator it = filesInFolder.begin() ; it != filesInFolder.end() ; ++it )
        wxRemoveFile( *it );
      
      wxRmdir( folder );
    }

    strStatus.str( std::string() ); //clear
    strStatus << sessionInfo.status;
  #ifdef WIN32
    file = wxString( std::string( ParaverConfig::getInstance()->getGlobalSessionPath() +
                                  "\\AutosavedSessions" +
                                  "\\ps" +
                                  strPid.str() +
                                  "_" +
                                  sessionInfo.sessionDate +
                                  "_" +
                                  strStatus.str() +
                                  ".session" ).c_str(), wxConvUTF8 );
  #else
    file = wxString( std::string( ParaverConfig::getInstance()->getGlobalSessionPath() +
                                  "/AutosavedSessions" +
                                  "/ps" +
                                  strPid.str() +
                                  "_" +
                                  sessionInfo.sessionDate +
                                  "_" +
                                  strStatus.str() +
                                  ".session" ).c_str(), wxConvUTF8 );
  #endif
    SessionSaver::SaveSession( file, GetLoadedTraces() );

    HandleMaxSessionFiles();
  }


  vector<Histogram *> histograms;
  LoadedWindows::getInstance()->getAll( histograms );
  
  for( vector<Histogram *>::iterator it = histograms.begin(); it != histograms.end(); ++it )
  {
    ( *it )->clearControlWindow();
    ( *it )->clearDataWindow();
    ( *it )->clearExtraControlWindow();
  }

  if( instChecker != NULL ) delete instChecker;
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


void paraverMain::OnSignal()
{
  if( !canServeSignal )
    return;

  // Is some trace loading now?
  if( paraverMain::dialogProgress != NULL )
    return;

  canServeSignal = false;

  SignalItem currentSignal = signalQueue.front();
  signalQueue.pop();

  if( !currentSignal.goodFile )
  {
    wxMessageDialog message( this,
                             _( "File ./paraload.sig not found" ),
                             _( "Signal Handler Manager" ),
                             wxOK | wxICON_EXCLAMATION );
    raiseCurrentWindow = false;
    message.ShowModal();
    raiseCurrentWindow = true;
    canServeSignal = true;
    return;
  }

  if( currentSignal.badTimes )
  {
    wxMessageDialog message( this, _( "Missing times separator ':' in file paraload.sig" ), _( "Signal Handler Manager" ), wxOK | wxICON_EXCLAMATION );
    raiseCurrentWindow = false;
    message.ShowModal();
    raiseCurrentWindow = true;
    canServeSignal = true;
    return;
  }

  if( !currentSignal.traceFileName.empty() )
    DoLoadTrace( currentSignal.traceFileName );

  // Anyway, Am I able to load any cfg?
  if( loadedTraces.size() == 0 )
  {
    wxMessageDialog message( this,
                             _( "No trace loaded" ),
                             _( "Signal Handler Manager" ),
                             wxOK | wxICON_EXCLAMATION );
    raiseCurrentWindow = false;
    message.ShowModal();
    raiseCurrentWindow = true;
    canServeSignal = true;
    return;
  }

  // Code only for sigusr1: load cfg
  if( currentSignal.isSignal1() && !currentSignal.cfgFileName.empty() )
    DoLoadCFG( currentSignal.cfgFileName );

  // Code for both sigusr1 and sigusr2: zoom & save image
  if( currentTimeline == NULL && currentHisto == NULL )
  {
    wxMessageDialog message( this, _( "No timeline nor histogram created" ), _( "Signal Handler Manager" ), wxOK | wxICON_EXCLAMATION );
    raiseCurrentWindow = false;
    message.ShowModal();
    raiseCurrentWindow = true;
    canServeSignal = true;
    return;
  }
  else
  {
    if( currentTimeline != NULL )
    {
      bool dummyFound;

      // Zoom
      currentTimeline->setWindowBeginTime( currentSignal.beginTime );
      currentTimeline->setWindowEndTime( currentSignal.endTime );
      currentTimeline->addZoom(  currentSignal.beginTime, currentSignal.endTime );

      // Redraw
      currentTimeline->setChanged( true );
      //currentTimeline->setRedraw( true );
      gTimeline *tmpTimeline = getGTimelineFromWindow( getAllTracesTree()->GetRootItem(), currentTimeline, dummyFound );
      tmpTimeline->redraw();

      // Save image if needed
      if( !currentSignal.imageFileName.empty() )
        tmpTimeline->saveImage( false, wxString::FromAscii( currentSignal.imageFileName.c_str() ) );
    }
    else // Histogram
    {
      // Zoom
      currentHisto->setWindowBeginTime( currentSignal.beginTime );
      currentHisto->setWindowEndTime( currentSignal.endTime );

      // Redraw
      currentHisto->setChanged( true );
      gHistogram *tmpHistogram = getGHistogramFromWindow( getAllTracesTree()->GetRootItem(), currentHisto );
      tmpHistogram->execute();

      // Save image if needed
      if( !currentSignal.imageFileName.empty() )
        tmpHistogram->saveImage( false, wxString::FromAscii( currentSignal.imageFileName.c_str() ) );
    }
  }

  // Refresh
  choiceWindowBrowser->UpdateWindowUI();

  canServeSignal = true;
}
#endif


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
    item->getTimeline()->OnPopUpPasteDefaultSpecial();
  else if( item->getHistogram() != NULL )
    item->getHistogram()->OnPopUpPasteDefaultSpecial();
}
#endif

void paraverMain::OnFindDialog()
{
  wxTreeCtrl *tree = (wxTreeCtrl *) choiceWindowBrowser->GetCurrentPage();
  if( !tree->GetSelection().IsOk() )
    return;
  TreeBrowserItemData *item = (TreeBrowserItemData *) tree->GetItemData( tree->GetSelection() );
  if( item->getTimeline() != NULL )
  {
    item->getTimeline()->setEnableDestroyButton( false );
    item->getTimeline()->OnFindDialog();
    item->getTimeline()->setEnableDestroyButton( true );
  }
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_CUT_TRACE
 */
void paraverMain::OnToolCutTraceClick( wxCommandEvent& event )
{
  if ( currentTrace == -1)
    ShowCutTraceWindow();
  else
    ShowCutTraceWindow( loadedTraces[ currentTrace ]->getFileName() );
}


string paraverMain::DoLoadFilteredTrace( string traceSrcFileName,
                                         string traceDstFileName,
                                         TraceOptions *traceOptions,
                                         vector< string > &filterToolIDs )
{
  string tmpTraceIn, tmpTraceOut;
  string tmpNameIn, tmpNameOut, tmpPathOut, tmpPathOutBackup;
  string strOutputFile;
  vector< string > tmpFiles;

#ifdef OLD_PCFPARSER
  ParaverTraceConfig *myConfig;
#else
  UIParaverTraceConfig *myConfig;
#endif

  ProgressController *progress = ProgressController::create( localKernel );
  progress->setHandler( progressFunction, this );

  if( paraverMain::dialogProgress == NULL )
    paraverMain::dialogProgress = new wxProgressDialog( wxT("Processing trace..."),
                                                        wxT(""),
                                                        numeric_limits<int>::max(),
                                                        this,
                                                        wxPD_CAN_ABORT|wxPD_AUTO_HIDE|\
                                                        wxPD_APP_MODAL|wxPD_ELAPSED_TIME|\
                                                        wxPD_ESTIMATED_TIME|wxPD_REMAINING_TIME );


  tmpNameOut = traceSrcFileName;

  for( PRV_UINT16 i = 0; i < filterToolIDs.size(); ++i )
  {
    tmpNameIn = tmpNameOut;

    if ( i == filterToolIDs.size() - 1 )
      tmpNameOut = traceDstFileName;
    else
      tmpNameOut = localKernel->getNewTraceName( tmpNameIn, filterToolIDs[ i ], false );

    paraverMain::dialogProgress->Pulse( wxString::FromAscii( tmpNameOut.c_str() ) );
    paraverMain::dialogProgress->Fit();
    paraverMain::dialogProgress->Show();

    if ( filterToolIDs[ i ] == TraceCutter::getID() )
    {
      TraceCutter *traceCutter = TraceCutter::create( localKernel, tmpNameIn, tmpNameOut, traceOptions, progress );
      traceCutter->execute( tmpNameIn, tmpNameOut, progress );
      localKernel->copyPCF( tmpNameIn, tmpNameOut );
      delete traceCutter;
    }
    else if ( filterToolIDs[ i ] == TraceFilter::getID() )
    {
      map< TTypeValuePair, TTypeValuePair > dummyTranslation; // it it's empty, it's ignored

      TraceFilter *traceFilter = localKernel->newTraceFilter( (char *)tmpNameIn.c_str(),
                                                              (char *)tmpNameOut.c_str(),
                                                              traceOptions,
                                                              dummyTranslation,
                                                              progress );
      localKernel->copyPCF( tmpNameIn, tmpNameOut );
      delete traceFilter;
    }
    else if ( filterToolIDs[ i ] == TraceSoftwareCounters::getID() )
    {
      TraceSoftwareCounters *traceSoftwareCounters =
              localKernel->newTraceSoftwareCounters( (char *)tmpNameIn.c_str(),
                                                     (char *)tmpNameOut.c_str(),
                                                     traceOptions,
                                                     progress );
      // traceSoftwareCounters modifies the pcf, don't copy here!
      delete traceSoftwareCounters;
    }

    localKernel->copyROW( tmpNameIn, tmpNameOut );
    tmpFiles.push_back( tmpNameOut );
  }

  // Delete intermediate files
  string pcfName, rowName;
  for( PRV_UINT16 i = 0; i < tmpFiles.size() - 1; ++i )
  {
    pcfName = LocalKernel::composeName( tmpFiles[ i ], string( "pcf" ) );
    rowName = LocalKernel::composeName( tmpFiles[ i ], string( "row" ) );
    remove( tmpFiles[ i ].c_str() );
    remove( pcfName.c_str() );
    remove( rowName.c_str() );
  }

  localKernel->commitNewTraceName( traceDstFileName );

  paraverMain::dialogProgress->Show( false );
  delete paraverMain::dialogProgress;
  paraverMain::dialogProgress = NULL;
  delete progress;

  return tmpNameOut;
}


void paraverMain::MainSettingsCutFilterDialog( CutFilterDialog *cutFilterDialog,
                                                const string& filename,
                                                bool loadTrace )
{
  cutFilterDialog->SetLocalKernel( localKernel );
  cutFilterDialog->SetLoadResultingTrace( loadTrace );

  // Set trace
  if( !filename.empty() )
  {
    // 1) given by parameter
    cutFilterDialog->SetNameSourceTrace( filename );
  }
  else if ( loadedTraces.size() > 0 && currentTrace == -1 )
  {
    // 2) tracePath if "All Traces" selected
    cutFilterDialog->SetNameSourceTrace( std::string( tracePath.mb_str() ) + PATH_SEP );
  }
  else
  {
    // 3) Default
    cutFilterDialog->SetNameSourceTrace( paraverConfig->getGlobalTracesPath() + PATH_SEP );
  }
}


void paraverMain::OptionsSettingCutFilterDialog( CutFilterDialog *cutFilterDialog,
                                                  TraceOptions *traceOptions,
                                                  const string& xmlFile,
                                                  vector< string > &filterToolOrder )
{
  if ( !xmlFile.empty() )
  {
    filterToolOrder = traceOptions->parseDoc( (char *)xmlFile.c_str() );

    // Keep my XMLPath
    wxString auxName = wxString::FromAscii( xmlFile.c_str() );
    wxString auxPath = wxFileName( auxName ).GetPath( wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR );
    XMLPath = std::string( auxPath.mb_str() );

    bool refresh = false;
    cutFilterDialog->SetXMLFile( auxName, refresh );
    //cutFilterDialog->TransferTraceOptionsToWindow( traceOptions, filterToolOrder );
  }
  else if ( !XMLLoadedBefore )
  {
    // Default Path
    XMLPath =  paraverConfig->getFiltersXMLPath() + PATH_SEP;
  }

  // If xmlFile empty we consider that information is given by parameter
  cutFilterDialog->TransferTraceOptionsToWindow( traceOptions, filterToolOrder );
}


void paraverMain::OnOKCutFilterDialog( CutFilterDialog *cutFilterDialog,
                                        vector< string > filterToolOrder )
{
  filterToolOrder   = cutFilterDialog->GetFilterToolOrder();
  string srcTrace   = cutFilterDialog->GetNameSourceTrace();
  string dstTrace   = cutFilterDialog->GetNameDestinyTrace();

  DoLoadFilteredTrace( srcTrace, dstTrace, cutFilterDialog->GetTraceOptions(), filterToolOrder );

  if ( cutFilterDialog->GetLoadResultingTrace() )
  {
    DoLoadTrace( dstTrace );
  }

  XMLLoadedBefore = cutFilterDialog->GetLoadedXMLPath( XMLPath );

  if ( cutFilterDialog->GetRunAppWithResultingTrace() )
  {
    wxString auxTrace =  wxString::FromAscii( dstTrace.c_str() );
    ShowRunCommand( auxTrace );
  }
}


void paraverMain::ShowCutTraceWindow( const string& filename,
                                       bool loadTrace,
                                       const string& xmlFile )
{
  CutFilterDialog *cutFilterDialog = new CutFilterDialog( this );

  MainSettingsCutFilterDialog( cutFilterDialog, filename, loadTrace );

  TraceOptions *traceOptions = TraceOptions::create( localKernel );
  vector< string > filterToolOrder;
  OptionsSettingCutFilterDialog( cutFilterDialog, traceOptions, xmlFile, filterToolOrder );

  if( cutFilterDialog->ShowModal() == wxID_OK )
  {
    OnOKCutFilterDialog( cutFilterDialog, filterToolOrder );
  }

#if wxMAJOR_VERSION<3
  cutFilterDialog->MakeModal( false );
#endif

  delete traceOptions;
  delete cutFilterDialog;
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
#if wxMAJOR_VERSION<3
    iconizeWindows( currentTree, root, event.Iconized() );
#else
    iconizeWindows( currentTree, root, event.IsIconized() );
#endif
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


void paraverMain::OnSessionTimer( wxTimerEvent& event )
{
  string file;
  if ( ParaverConfig::getInstance()->getGlobalSingleInstance() ) 
  {
    #ifdef WIN32
    file = ParaverConfig::getInstance()->getGlobalSessionPath() + "\\paraver.session";
    #else
    file = ParaverConfig::getInstance()->getGlobalSessionPath() + "/paraver.session";
    #endif
    if ( firstSave )
    {
      ParaverConfig::getInstance()->cleanCompleteSessionFile();
      firstSave = false;
    }
  }
  else /*if ( !ParaverConfig::getInstance()->getGlobalSingleInstance() ) */
  {
    stringstream strPid;
    stringstream strStatus;
    strPid << sessionInfo.pid;
    strStatus << sessionInfo.status;

    #ifdef WIN32
    file = ParaverConfig::getInstance()->getGlobalSessionPath() + "\\AutosavedSessions" + "\\ps" + strPid.str() + "_" + sessionInfo.sessionDate + "_" + strStatus.str() + ".session";
    #else
    file = ParaverConfig::getInstance()->getGlobalSessionPath() + "/AutosavedSessions" +  "/ps" + strPid.str() + "_" + sessionInfo.sessionDate + "_" + strStatus.str() + ".session";
    #endif
  }
  SessionSaver::SaveSession( wxString::FromAscii( file.c_str() ), GetLoadedTraces() );
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENULOADSESSION
 */

void paraverMain::OnMenuloadsessionClick( wxCommandEvent& event )
{
  wxFileDialog dialog( this, wxT( "Load session" ), _(""), _(""), _("Paraver session (*.session)|*.session|All files|*"), wxFD_OPEN|wxFD_FILE_MUST_EXIST );
  if( dialog.ShowModal() == wxID_OK )
  {
    SessionSaver::LoadSession( dialog.GetPath() );
    previousSessions->add( std::string( dialog.GetPath().mb_str() ) );
  }
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUSAVESESSION
 */

void paraverMain::OnMenusavesessionClick( wxCommandEvent& event )
{
  OnMenusavesession();
}

bool paraverMain::OnMenusavesession( )
{
  vector< wxString > extensions;
  extensions.push_back( wxT( "session" ) );

  FileDialogExtension dialog( this,
                              wxT( "Save session" ),
                              _(""),
                              _(""),
                              _("Paraver session (*.session)|*.session"),
                              wxFD_SAVE,
                              wxDefaultPosition,
                              wxDefaultSize,
                              _( "filedlg" ),
                              extensions );
  if( dialog.ShowModal() == wxID_OK )
  {
    wxFileName tmpFile( dialog.GetPath() );
    SessionSaver::SaveSession( tmpFile.GetFullPath(), GetLoadedTraces() );
    previousSessions->add( std::string( dialog.GetPath().mb_str() ) );
    return true;
  }
  return false;
}



/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_ABOUT
 */

void paraverMain::OnAboutClick( wxCommandEvent& event )
{
  wxAboutDialogInfo info;
  wxArrayString developers;
  wxString description;

  info.SetName( _("wxParaver") );
  info.SetVersion( wxString() << _( VERSION ) << _( " (Build " ) + \
                   wxString() << _( __DATE__ ) << _( ")" ) );

  description << _( "Licensed under LGPL-2.1\n\n" );

  description << _( "wxParaver is a trace-based visualization and analysis tool " );
  description << _( "designed to study quantitative detailed metrics and obtain " );
  description << _( "qualitative knowledge of the performance of applications, " );
  description << _( "libraries, processors and whole architectures.\n" );

  description << _( "\nwxParaver, Extrae, and Dimemas belong to the Performance ");
  description << _( "Tools Suite developed at BSC-CNS :\n" );
  description << _( "Barcelona Supercomputing Center - Centro Nacional de Supercomputacion\n\n" );

  description << _( "\nwebsite: https://tools.bsc.es/paraver" );
  description << _( "\ndownloads: https://tools.bsc.es/downloads" );
  description << _( "\nmail: paraver@bsc.es" );
  description << _( "\ntools@bsc.es\n" );
  info.SetDescription( description );
  developers.Add( _("Eloy Martinez Hortelano (eloy.martinez@bsc.es)") );
  developers.Add( _("Pedro Antonio Gonzalez Navarro (pedro.gonzalez@bsc.es)") );
  developers.Add( _("Miguel Moreno Gomez (miguel.moreno@bsc.es)") );
  info.SetDevelopers( developers );
  info.SetWebSite( _("https://tools.bsc.es/paraver") );
  //info.SetCopyright(_T(""));

  wxAboutBox(info);
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_ABOUT
 */

void paraverMain::OnTutorialsClick( wxCommandEvent& event )
{
  wxString tutorialsRoot;

  if ( tutorialsWindow == NULL )
  {
    tutorialsRoot = wxString( paraverMain::myParaverMain->GetParaverConfig()->getGlobalTutorialsPath().c_str(), wxConvUTF8 );
    tutorialsWindow = HelpContents::createObject( HelpContents::TUTORIAL, this, tutorialsRoot, 
                                                  true, wxID_ANY, _("Tutorials") );
  }

  tutorialsWindow->Show( true );
}


void paraverMain::ShowRunCommand( wxString traceFile )
{
  if( runApplication != NULL )
  {
    runApplication->Raise();
    return;
  }

  runApplication = new RunScript( (wxWindow *)this, traceFile );

  runApplication->Show();
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_RUN_SCRIPT
 */
void paraverMain::OnToolRunApplicationClick( wxCommandEvent& event )
{
  if( !loadedTraces.empty() )
  {
    wxString tmpStr( loadedTraces[ currentTrace ]->getFileName().c_str(), wxConvUTF8 );
    ShowRunCommand( tmpStr );
  }
  else
    ShowRunCommand( wxT("") );
}


/*
  Reads path where the current wxparaver.bin binary is installed
*/
bool paraverMain::getParaverHome( wxString &paraverHome )
{
  bool done = false;

  /*#ifdef __WXMAC__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef fileURL = CFBundleCopyResourceURL( mainBundle, CFSTR("aaa"), CFSTR("txt"), NULL );
    CFStringRef filePath = CFURLCopyFileSystemPath( fileURL, kCFURLPOSIXPathStyle );
    CFStringEncoding encodingMethod = CFStringGetSystemEncoding();
    const char *path = CFStringGetCStringPtr( filePath, encodingMethod );
    cout<<"File location: "<<path<<endl;
#endif*/

  //wxString paraverHome;

#ifdef WIN32
  std::string baseDir;

  char myPath[ MAX_LEN_PATH ];
  HMODULE hModule = GetModuleHandle( NULL );
  if ( hModule != NULL )
  {
    GetModuleFileName( NULL, myPath, ( sizeof( myPath ) ));
    PathRemoveFileSpec( myPath );
    /*char tmpMyPath[ MAX_LEN_PATH ];
    size_t tmpSize;
    wcstombs_s( &tmpSize, tmpMyPath, MAX_LEN_PATH, myPath, MAX_LEN_PATH );
    baseDir = tmpMyPath;*/
    baseDir = myPath;
  }
  paraverHome = wxT( baseDir.c_str() );

  done = paraverHome != wxT( "" );

#elif defined( __APPLE__ )

  CFBundleRef mainBundle = CFBundleGetMainBundle();
  CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
  char tmpPath[PATH_MAX];
  if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)tmpPath, PATH_MAX))
  {
    throw ParaverKernelException();
  }
  CFRelease(resourcesURL);

  paraverHome = tmpPath;

  done = paraverHome != wxT( "" );

#else

  done = wxGetEnv( wxString( wxT( "PARAVER_HOME" ) ), &paraverHome );

#endif

  return done;
}


void paraverMain::messageUnknownPath( wxString helpContentsPath, wxString paraverHome )
{
  wxString msg =
          wxString( wxT( "Path to Help Contents doesn't exist:\n\n  " ) ) +
          helpContentsPath +
          wxString( wxT( "\n\nPlease check $PARAVER_HOME:\n\n" ) ) +
          paraverHome;

  wxMessageDialog message( this, msg, _( "Warning" ), wxOK | wxICON_WARNING );
  message.ShowModal();
}


void paraverMain::messageUndefinedParaverHome()
{
  wxString msg =
            wxString( wxT( "Unable to find Help Contents.\n\n$PARAVER_HOME is undefined" ) );

  wxMessageDialog message( this, msg, _( "Warning" ), wxOK | wxICON_WARNING );
  message.ShowModal();
}


// helpContentsBaseRelativePath is relative path to help base dir inside absolute $PARAVER_HOME:
// i.e. Given helpContentsPath = "/share/docs/html"
//      and having $PARAVER_HOME = "/opt/wxparaver"
//      where $PARAVER_HOME + helpContentsPath contains 2 subdirs
//            (/opt/wxparaver/share/docs/html/)1.quick_reference/index.html
//            (/opt/wxparaver/share/docs/html/)2.paraver_toolset/index.html
// a help window indexing both 1 and 2 subhelps is built.
void paraverMain::createHelpContentsWindow(
        const wxString &helpContentsBaseRelativePath,  // relative path"/share/docs/html/"
        const wxString &helpFile, // empty or relative subpath like "1.quick_reference/index.html"
        const wxString &hRef,      // empty or href like "#section_1"
        bool  isModal )
{
  wxString paraverHome;
  if ( !getParaverHome( paraverHome ) )
  {
    messageUndefinedParaverHome();
    return;
  }

  wxString helpContentsAbsolutePath = paraverHome + helpContentsBaseRelativePath;
  if ( !wxFileName( helpContentsAbsolutePath ).DirExists() )
  {
    messageUnknownPath( helpContentsBaseRelativePath, paraverHome );
    return;
  }

  bool lookForContents = helpFile.IsEmpty();

  if ( helpContents == NULL )
  {
    //helpContents = new HelpContents( NULL, helpContentsAbsolutePath, lookForContents, wxID_ANY, _("Help Contents") );
    helpContents = HelpContents::createObject( HelpContents::HELP, NULL, helpContentsAbsolutePath, 
                                               lookForContents, wxID_ANY, _("Help Contents") );
  }

  // If helpFile has no "html" at the end, use Help Content's Index as hCAP (which works)
  if ( helpFile.SubString( helpFile.size() - 4, helpFile.size() - 1) != wxT( "html" ) )
  {
    helpContentsAbsolutePath = wxT( "file://" ) + 
          wxString::FromAscii( paraverConfig->getParaverConfigDir().c_str() ) +
          wxString( wxFileName::GetPathSeparator() ) +
          wxString( wxT( "help_contents" ) ) + wxT( "_index.html" );
  }
  else // Otherwise use previous hCAP
  {
    helpContentsAbsolutePath = wxT( "file://" ) + paraverHome + helpContentsBaseRelativePath + helpFile + hRef;
  }

  if ( lookForContents )
  {
    if ( paraverConfig->getGlobalHelpContentsUsesBrowser() )
    { 
      helpContents->LoadHtml( helpContentsAbsolutePath );
    } //helpContents->buildIndex();
    else if ( helpContents->IsShown() )
      helpContents->Raise();
    else
    {
      if ( isModal )
        helpContents->ShowModal();
      else
        helpContents->Show();
    }
  }
  else
  {
    helpContents->LoadHtml( helpContentsAbsolutePath );
    if ( !paraverConfig->getGlobalHelpContentsUsesBrowser() && isModal )
      helpContents->ShowModal();
    else if ( !paraverConfig->getGlobalHelpContentsUsesBrowser() )
      helpContents->Show();
  }
}


void paraverMain::OnHelpcontentsClick( wxCommandEvent& event )
{
  if ( !paraverConfig->getGlobalHelpContentsQuestionAnswered() )
    helpQuestion();

  wxChar SEP = wxFileName::GetPathSeparator();

  wxString baseRelativePath = SEP +
        wxString( wxT( "share" ) ) + SEP +
        wxString( wxT( "doc" ) ) + SEP +
        wxString( wxT( "wxparaver_help_contents" ) ) + SEP +
        wxString( wxT( "html" ) ) + SEP;

  createHelpContentsWindow( baseRelativePath );
}


wxString paraverMain::getHintComposed( const std::pair< std::string, std::string >& hint )
{
//  wxFileName filename( wxString::FromAscii(  hint.first.c_str() ) );
//  return filename.GetName() + _( " - " ) + wxString::FromAscii(  hint.second.c_str() );
  return wxString::FromAscii( hint.second.c_str() );
}


bool isForbidden( const char& c )
{
  static std::string forbiddenChars =
          std::string( wxFileName::GetForbiddenChars().mb_str() ) + ' ' + PATH_SEP;

  return ( std::find( forbiddenChars.begin(), forbiddenChars.end(), c ) != forbiddenChars.end() );
}


wxString paraverMain::buildFormattedFileName( std::string windowName, const std::string& traceName )
{
  std::replace_if( windowName.begin(), windowName.end(), isForbidden, '_' );
  wxString auxTraceName = wxString::FromAscii( traceName.c_str() );
  auxTraceName.Remove( auxTraceName.Find( wxT( ".prv" ) ) );

  return ( wxString::FromAscii( windowName.c_str() ) + wxString( wxT( '@' ) ) + auxTraceName );
}


void paraverMain::OnHintClick( wxCommandEvent& event )
{
  int hintId = event.GetId();
  wxMenuItem *tmpMenuItem = menuHints->FindItem( hintId );

  wxFileName tmpCFG( wxString::FromAscii( ( (MenuHintFile *) tmpMenuItem->GetRefData() )->fileName.c_str() ) );
  if ( tmpCFG.IsRelative() )
  {
    wxString tmpGlobalCFGs( localKernel->getDistributedCFGsPath().c_str(), wxConvUTF8 );
    tmpCFG.MakeAbsolute( tmpGlobalCFGs );
  }

  DoLoadCFG( std::string( tmpCFG.GetFullPath().mb_str() ) );
}


void paraverMain::OnMenuHintUpdate( wxUpdateUIEvent& event )
{
  GetMenuBar()->EnableTop( 1, 
                           !loadedTraces.empty() && 
                           !traceWorkspaces[ getCurrentTrace() ].empty() );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ACTIVE_WORKSPACES
 */

void paraverMain::OnButtonActiveWorkspacesClick( wxCommandEvent& event )
{
  vector< string > tmpWorkspaces = workspacesManager->getWorkspaces( WorkspaceManager::ALL );

  if( tmpWorkspaces.empty() )
  {
    wxMessageDialog emptyWorkspacesDialog( this,
                                           _( "No Workspaces available.\nDo you want to open Preferences dialog to create any?" ),
                                           _( "No Workspaces" ),
                                           wxYES_NO| wxICON_QUESTION );
    if( emptyWorkspacesDialog.ShowModal() == wxID_YES )
    {
      ShowPreferences( ID_PREFERENCES_WORKSPACES );
    }

    return;
  }

  wxArrayString tmpNames;
  wxArrayInt tmpActive;
  int position = 0;
  for ( vector< string >::iterator it = tmpWorkspaces.begin(); it != tmpWorkspaces.end(); ++it )
  {
    tmpNames.Add( wxString::FromAscii( it->c_str() ) );
    if ( std::find( traceWorkspaces[ loadedTraces[ currentTrace ] ].begin(), traceWorkspaces[ loadedTraces[ currentTrace ] ].end(),*it ) != traceWorkspaces[ loadedTraces[ currentTrace ] ].end() )
      tmpActive.Add( position );
    ++position;
  }

  wxMultiChoiceDialog tmpChoiceDialog( this, _("Select active workspaces"),  _( "Workspaces" ), tmpNames );
  tmpChoiceDialog.SetSelections( tmpActive );

  if ( tmpChoiceDialog.ShowModal() == wxID_OK )
  {
    tmpActive = tmpChoiceDialog.GetSelections();
    traceWorkspaces[ loadedTraces[ currentTrace ] ].clear();
    for( size_t i = 0; i < tmpActive.GetCount(); ++i )
    {
      traceWorkspaces[ loadedTraces[ currentTrace ] ].push_back( tmpWorkspaces[ tmpActive[ i ] ] );
    }

    setActiveWorkspacesText();
    refreshMenuHints();

    // These are here because no UpdateUI for MenuBar
    wxUpdateUIEvent tmpEvent;
    OnMenuHintUpdate( tmpEvent );
  }
}


void paraverMain::setActiveWorkspacesText()
{
  if ( currentTrace == -1 || traceWorkspaces[ getCurrentTrace() ].empty() )
  {
    txtActiveWorkspaces->SetValue( _("None") );
  }
  else
  {
    wxString tmpActive;
    size_t tmpCurrentWorkspace = 0;
    for ( vector< string >::iterator it = traceWorkspaces[ getCurrentTrace() ].begin(); it != traceWorkspaces[ getCurrentTrace() ].end(); ++it )
    {
      if ( !tmpActive.IsEmpty() )
        tmpActive += _( "+" );

      if( tmpCurrentWorkspace < firstUserWorkspace[ getCurrentTrace() ] )
        tmpActive += wxString::FromAscii( it->c_str() );
      else
      {
        if( workspacesManager->existWorkspace( it->c_str(), WorkspaceManager::DISTRIBUTED ) )
          tmpActive += wxString::FromAscii( it->c_str() ) + wxT( "#2" );
        else
          tmpActive += wxString::FromAscii( it->c_str() );
      }
      ++tmpCurrentWorkspace;
    }

    txtActiveWorkspaces->SetValue( tmpActive );
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for wxID_STATIC_AUTO_REDRAW
 */

void paraverMain::OnStaticAutoRedrawUpdate( wxUpdateUIEvent& event )
{
  event.Enable( currentTimeline != NULL || currentHisto != NULL );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHECKBOX_AUTO_REDRAW
 */

void paraverMain::OnCheckboxAutoRedrawUpdate( wxUpdateUIEvent& event )
{
  event.Enable( currentTimeline != NULL || currentHisto != NULL );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_FORCE_REDRAW
 */

void paraverMain::OnButtonForceRedrawUpdate( wxUpdateUIEvent& event )
{
  event.Enable( currentTimeline != NULL || currentHisto != NULL );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_FORCE_REDRAW
 */

void paraverMain::OnButtonForceRedrawClick( wxCommandEvent& event )
{
  if( currentTimeline != NULL )
    currentTimeline->setForceRedraw( true );
  else if( currentHisto != NULL )
    currentHisto->setForceRecalc( true );
}


bool paraverMain::getAutoRedraw() const
{
  return checkAutoRedraw->GetValue();
}


bool paraverMain::isCFG4DModeDisabled() const
{
  return ( ( currentTimeline != NULL &&
              ( !currentTimeline->getCFG4DEnabled() || !currentTimeline->getCFG4DMode() ) )
           ||
           ( currentHisto != NULL &&
              ( !currentHisto->getCFG4DEnabled() || !currentHisto->getCFG4DMode() ) ) );
}


#ifndef WIN32
void paraverMain::insertSignalItem( bool isSig1 )
{
  SignalItem tmpSignalItem;

  // Does paraload.sig exists?
  string path = getenv("HOME");
  string filename = "paraload.sig";
  string fullName = path + "/" + filename;
  ifstream paraloadFile;
  paraloadFile.open( fullName.c_str() );
  if( !paraloadFile  )
  {
    tmpSignalItem.goodFile = false;
    signalQueue.push( tmpSignalItem );
    return;
  }
  tmpSignalItem.goodFile = true;

  string::size_type position;

  tmpSignalItem.signal1 = isSig1;

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
    if( auxLine[0] != '#' )
    {
      lines.push_back( auxLine );
      auxLine.clear();
    }
  }

  // POSTCOND:
  // lines[0] must contain paraver cfg
  // lines[1] must contain time range to set the window
  // if exists, lines[2] must contain a trace to load

  if ( lines.size() > 2 && lines[ 2 ] != "" ) // any trace?
  {
    bool found = false;
    PRV_INT16 current = currentTrace;

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
      for ( current = 0; current < (PRV_INT16)loadedTraces.size(); ++current )
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

    if ( current <= 0 || !found ) // No trace loaded or not found!!
      tmpSignalItem.traceFileName = lines[ 2 ];
  }

  // Read cfg location
  tmpSignalItem.cfgFileName = lines[ 0 ];

  // Image file if exists
  if( lines.size() > 3 )
    tmpSignalItem.imageFileName = lines[ 3 ];

  // Read paraload.sig second line: times
  string times = lines[ 1 ];

  size_t pos = times.find( ":" );
  if( pos == string::npos )
  {
    tmpSignalItem.badTimes = true;
    signalQueue.push( tmpSignalItem );
    return;
  }
  else
  {
    tmpSignalItem.badTimes = false;
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

    tmpSignalItem.beginTime = auxt1;
    tmpSignalItem.endTime = auxt2;
  }

  signalQueue.push( tmpSignalItem );

  wxIdleEvent tmpEvent;
  AddPendingEvent( tmpEvent );
}
#endif // WIN32



void paraverMain::LastSessionLoad( bool isSessionInitialized )
{
  #ifdef WIN32
    wxString folder( wxString( ParaverConfig::getInstance()->getGlobalSessionPath().c_str(), wxConvUTF8 )  +
        _( "\\AutosavedSessions" ) );
  #else
    wxString folder( wxString( ParaverConfig::getInstance()->getGlobalSessionPath().c_str(), wxConvUTF8 ) + 
        _( "/AutosavedSessions" ) );
  #endif

  wxArrayString paths = SessionSelectionDialog( folder, isSessionInitialized ).GetSessionPaths();
  if ( paths.size() > 0 )
  {
    wxLogNull logNull;
    stClient *client = new stClient;
    wxString hostName = wxT( "localhost" );
    wxString pathToLoad = paths[ 0 ];
    bool found = false;

    for ( int idx = 0 ; idx < paths.size() && !found ; ++idx )
    {
      wxString path = paths[ idx ]; 
      wxString folderPath = path;
      folderPath.Replace( wxT( ".session" ), wxT( "_session" ) );

#ifdef WIN32
      wxString folderPathSimple = folderPath.AfterLast( '\\' );
      wxString sessionPID = folderPathSimple.BeforeFirst( '_' ).AfterLast( 's' );
      sessionPID.Replace( wxT( "ps" ), wxT( "" ) );
      wxString serviceName = wxT( "wxparaver_service-" ) + 
          wxGetUserId() + 
          wxT( "-" ) +
          sessionPID;
#else
      wxString folderPathSimple = folderPath.AfterLast( '/' );
      wxString sessionPID = folderPathSimple.BeforeFirst( '_' ).AfterLast( 's' );
      sessionPID.Replace( wxT( "ps" ), wxT( "" ) );
      wxString serviceName = wxT( "/tmp/wxparaver_service-" ) + 
          wxGetUserId() + 
          wxT( "-" ) +
          sessionPID;
#endif
      wxConnectionBase *connection = client->MakeConnection( hostName, serviceName, wxT( "wxparaver" ) );

      if ( connection == NULL && wxDirExists( folderPath ) && wxFileExists( path ) )
      {
        SessionSaver::LoadSession( path );
        found = true;
        // "Replacing" crashed session and prevent
        // repetition in case older sessions fail
        wxRemoveFile( serviceName );
        wxRemoveFile( path );
        wxRmdir( folderPath );
      }
      delete connection;
    }
    delete client;
  }
  else
  {
    wxMessageDialog message( 
      this, 
      _("An error occurred with last session's file.\nNo session will be loaded."), 
      _("Warning"), 
      wxOK );
    message.ShowModal();
  }
}



void paraverMain::checkIfPrevSessionLoad( bool prevSessionWasComplete )
{ 
  //to do : add alert popup
  #ifdef WIN32
    string file( ParaverConfig::getInstance()->getGlobalSessionPath() + "\\paraver.session" );
  #else
    string file( ParaverConfig::getInstance()->getGlobalSessionPath() + "/paraver.session" );
  #endif
  if ( ParaverConfig::getInstance()->getGlobalSingleInstance() )
  {
    if ( !prevSessionWasComplete && isSessionFile( file ) &&
        wxMessageBox( wxT( "Paraver closed unexpectedly. Do you want to load your last auto-saved Paraver session?" ),
                      wxT( "Load auto-saved session" ), wxICON_QUESTION | wxYES_NO, this ) == wxYES )
      DoLoadSession( file );
  }
  else if ( wxMessageBox( wxT( "Paraver closed unexpectedly. Do you want to load your last crashed auto-saved Paraver session?" ),
                      wxT( "Load auto-saved sessions" ), wxICON_QUESTION | wxYES_NO, this ) == wxYES )
  {
    LastSessionLoad( false );
  }
}


inline std::string ZeroTrail( int number )
{
  stringstream trailedNumber;
  if ( number < 10 )
    trailedNumber << "0";
  
  trailedNumber << number;
  return trailedNumber.str();
}

void paraverMain::initSessionInfo()
{
  sessionInfo.pid = getpid();
  sessionInfo.status = SessionInfo::OPEN;

  boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
  std::stringstream ss;
  ss << ZeroTrail( now.date().year() ) 
     << ZeroTrail( static_cast<int>( now.date().month() ) ) 
     << ZeroTrail( now.date().day() ) 
     << "_" 
     << ZeroTrail( now.time_of_day().hours() ) 
     << ZeroTrail( now.time_of_day().minutes() ) 
     << ZeroTrail( now.time_of_day().seconds() ); //iso

  sessionInfo.sessionDate = ss.str();
}


void paraverMain::filterExternalApps()
{
  //Get WX variants and check
  wxArrayString externalTextEditors = paraverMain::FromVectorStringToWxArray( paraverConfig->getGlobalExternalTextEditors(), "txt" );
  wxArrayString newTxt;

  for ( int i = 0 ; i < externalTextEditors.size(); ++i )
  {
    //wxString myWxStr( externalTextEditors[ i ].wc_str(), wxConvUTF8 );
#ifdef WIN32
    wxString command = externalTextEditors[ i ] + wxT( " --version" );
#else
    //wxString command = myWxStr + wxT( " --version 1>/dev/null 2>/dev/null'");
    wxString command = externalTextEditors[ i ] + wxT( " --version 1>&- 2>&-'");
#endif
    int execRes = wxExecute( command, wxEXEC_SYNC );
    if ( execRes == 0 )
      newTxt.Add( externalTextEditors[ i ] );
  }

  if ( newTxt.size() == 0 )
  {
    // default type: "txt"
    wxString errMessage = _( "No text editors installed. Please verify the External Applications tab and add at least a text editor." );
    wxMessageBox( errMessage, _( "No programs found" ), wxOK );
  }



  wxArrayString externalPDFReaders = paraverMain::FromVectorStringToWxArray( paraverConfig->getGlobalExternalPDFReaders(), "pdf" );
  wxArrayString newPDF;

  for ( int i = 0 ; i < externalPDFReaders.size(); ++i )
  {
#ifdef WIN32
    wxString command = externalPDFReaders[ i ] + wxT( " --version" );
#else
    wxString command = externalPDFReaders[ i ] + wxT( " --version 1>&- 2>&-'");
#endif
    int execRes = wxExecute( command, wxEXEC_SYNC );
    if ( execRes == 0 )
      newPDF.Add( externalPDFReaders[ i ] );
  }

  if ( newPDF.size() == 0 )
  {
    // default type: "pdf"
    wxString errMessage = _( "No PDF readers installed. Please verify the External Applications tab and add at least a PDF reader." );
    wxMessageBox( errMessage, _( "No programs found" ), wxOK );
  }

  // Pass them back
  paraverConfig->setGlobalExternalTextEditors( FromWxArrayToVectorString( newTxt ) );
  paraverConfig->setGlobalExternalPDFReaders( FromWxArrayToVectorString( newPDF ) );
}


bool paraverMain::IsSessionValid()
{
  return paraverMain::validSessions;
}


void paraverMain::ValidateSession( bool setValidate )
{
  paraverMain::validSessions = paraverMain::validSessions && setValidate;
}


void paraverMain::helpQuestion()
{
  int question = wxMessageBox( wxT( "Do you want to use your system's browser to open the Help menu? You can change this setting at the Preferences menu." ),
          wxT( "Please confirm" ),
          wxICON_QUESTION|wxYES_NO);
  
  paraverConfig->setGlobalHelpContentsUsesBrowser( question == wxYES );
  paraverConfig->setGlobalHelpContentsQuestionAnswered( true );
}

Trace *paraverMain::getCurrentTrace() const
{
  Trace *tmpTraceToUse = NULL;
  if( choiceWindowBrowser->GetSelection() == 0 )
  {
    if( currentTimeline != NULL )
      tmpTraceToUse = currentTimeline->getTrace();
    else if( currentHisto != NULL )
      tmpTraceToUse = currentHisto->getTrace();
    else
      tmpTraceToUse = loadedTraces[ currentTrace ];
  }
  else
  {
    tmpTraceToUse = loadedTraces[ currentTrace ];
  }

  return tmpTraceToUse;
}
