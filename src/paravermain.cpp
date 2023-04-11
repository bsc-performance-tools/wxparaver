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
#include "cfgs4d.h"
#include "window.h"
#include "histogram.h"
#include "gtimeline.h"
#include "ghistogram.h"
#include "pg_extraprop.h"
#include "progresscontroller.h"
#include "loadedwindows.h"
#include "filter.h"
#include "pg_util.h"
#include "saveconfigurationdialog.h"
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
#include "sessionselectiondialog.h"
#include "traceinformationdialog.h"

#include <wx/display.h>
#include <wx/process.h>
#include <wx/mimetype.h>

#include <signal.h>
#include <iostream>
#include <sstream>
#include <algorithm>
//#include "connection.h"

#ifdef _WIN32
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
#include "../icons/information.xpm"
#include "../icons/new_window.xpm"
#include "../icons/new_derived_window.xpm"
#include "../icons/new_histogram.xpm"
#include "../icons/delete.xpm"
#include "../icons/cut_trace.xpm"
#include "../icons/run_script.xpm"
#include "../icons/file_browser.xpm"
#include "../icons/window_properties.xpm"
#include "../icons/autoredraw_refresh.xpm"
#include "../icons/three_dots.xpm"
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
#include "derived_controlled_average.xpm"
#include "derived_controlled_maximum.xpm"
#include "derived_controlled_add.xpm"
#include "derived_controlled_enumerate.xpm"

#include <algorithm>

using namespace std;

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
  EVT_MENU( ID_MENUTRACEINFORMATION, paraverMain::OnTraceInformationClick )
  EVT_UPDATE_UI( ID_MENUTRACEINFORMATION, paraverMain::OnTraceInformationUpdate )
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
  EVT_MENU( ID_TOOL_TRACE_INFORMATION, paraverMain::OnTraceInformationClick )
  EVT_UPDATE_UI( ID_TOOL_TRACE_INFORMATION, paraverMain::OnTraceInformationUpdate )
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


paraverMain* paraverMain::myParaverMain = nullptr;

wxProgressDialog *paraverMain::dialogProgress = nullptr;

wxSize paraverMain::defaultTitleBarSize = wxSize(0,0);

Timeline *paraverMain::beginDragWindow = nullptr;
Timeline *paraverMain::endDragWindow = nullptr;

bool paraverMain::disableUserMessages = false;
bool paraverMain::validSessions = true;
bool paraverMain::stopOnIdle = false;


extern volatile bool sig1;
extern volatile bool sig2;
extern struct sigaction act;

static bool userMessage( UserMessageID message )
{
  if( paraverMain::disableUserMessages )
    return true;
  wxMessageDialog tmpDialog( nullptr, wxString::FromUTF8( userMessages[ static_cast<size_t>( message ) ].c_str() ) +
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
  images->Add( wxIcon( derived_controlled_average_xpm ) );

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
  if ( tutorialsWindow != nullptr )
  {
    delete tutorialsWindow;
  }
  if ( helpContents != nullptr )
  {
    delete helpContents;
  }

  delete sessionTimer;
  delete previousCFGs;
  delete previousCutFilteredTraces;
  delete previousSessions;
  delete previousTraces;

  for( std::vector< PropertyClientData * >::iterator it = propertiesClientData.begin();
       it != propertiesClientData.end(); ++it )
    delete *it; 

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
  clusteringWindow = nullptr;
  currentHisto = nullptr;
  currentTimeline = nullptr;
  currentTrace = -1;
  currentWindow = nullptr;
  helpContents = nullptr;
  lastHisto = nullptr;
  lastTimeline = nullptr;
  numNewDerived = 0;
  numNewHistograms = 0;
  numNewWindows = 0;
  openedPropertyDialog = nullptr;
  paraverConfig = ParaverConfig::getInstance();
  previousCFGs = PreviousFiles::createPreviousCFGs();
  previousCutFilteredTraces = PreviousFiles::createPreviousTreatedTraces();
  previousSessions = PreviousFiles::createPreviousSessions();
  previousTraces = PreviousFiles::createPreviousTraces();
  raiseCurrentWindow = true;
  runApplication = nullptr;
  sessionTimer = new wxTimer( this, ID_TIMER_MAIN );
  someWinIsRedraw = false;
  traceLoadedBefore = false;
  tutorialsWindow = nullptr;
  workspacesManager = WorkspaceManager::getInstance( localKernel );
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

  cutFilterFinished = false;

  if ( ParaverConfig::getInstance()->getGlobalSessionSaveTime() > 0 )
  {
    sessionTimer->Start( ParaverConfig::getInstance()->getGlobalSessionSaveTime() * 60e3 );
  }

  traceLoadedBefore = false;
  CFGLoadedBefore = false;
  firstSave = true;
  instChecker = nullptr;

  wxFileSystem::AddHandler( new wxMemoryFSHandler() );
#ifdef _WIN32
  wxBitmap bmp( logoBSC_xpm );
#endif

  wxMemoryFSHandler::AddFile( wxT( "logoBSC.xpm" ),
#ifdef _WIN32
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

    paraverConfig->writeParaverConfigFile();
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
  menuFile->Append(ID_MENUTRACEINFORMATION, _("Trace Information..."), wxEmptyString, wxITEM_NORMAL);
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
  wxBitmap itemtool2Bitmap(itemFrame1->GetBitmapResource(wxT("icons/information.xpm")));
  wxBitmap itemtool2BitmapDisabled;
  tbarMain->AddTool(ID_TOOL_TRACE_INFORMATION, _("Trace Information"), itemtool2Bitmap, itemtool2BitmapDisabled, wxITEM_NORMAL, _("View Trace Information"), wxEmptyString);
  tbarMain->AddSeparator();
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

  wxStaticBitmap* itemStaticBitmap1 = new wxStaticBitmap( panelAutoRedraw, wxID_STATIC, itemFrame1->GetBitmapResource(wxT("icons/autoredraw_refresh.xpm")), wxDefaultPosition, wxDLG_UNIT(panelAutoRedraw, wxSize(8, 6)), 0 );
  itemBoxSizer4->Add(itemStaticBitmap1, 0, wxALIGN_CENTER_VERTICAL|wxALL, wxDLG_UNIT(panelAutoRedraw, wxSize(2, -1)).x);

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

  btnActiveWorkspaces = new wxBitmapButton( itemPanel36, ID_BUTTON_ACTIVE_WORKSPACES, itemFrame1->GetBitmapResource(wxT("icons/three_dots.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  btnActiveWorkspaces->Show(false);
  itemBoxSizer37->Add(btnActiveWorkspaces, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

  // Fit to content
  itemFrame1->GetAuiManager().GetPane(wxT("Pane1")).BestSize(itemPanel36->GetSizer()->Fit(itemPanel36)).MinSize(itemPanel36->GetSizer()->GetMinSize());

  GetAuiManager().Update();

////@end paraverMain content construction
  tbarMain->RemoveTool( ID_TOOL_TRACE_INFORMATION );

  wxTreeCtrl* tmpTree = createTree( imageList );
  tmpTree->Connect( wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler( paraverMain::OnTreeKeyPress ), nullptr, this );
  choiceWindowBrowser->AddPage( tmpTree, _( "All Traces" ) );
  choiceWindowBrowser->AddPage( createTree( imageList ), _( "Dummy Tree" ) );
  choiceWindowBrowser->DeletePage( 1 );

  toolBookFilesProperties->GetToolBar()->SetToolShortHelp( 1, wxT("Paraver Files") );
  toolBookFilesProperties->GetToolBar()->SetToolShortHelp( 2, wxT("Window Properties") );

  dirctrlFiles->SetPath( wxString( ParaverConfig::getInstance()->getGlobalCFGsPath().c_str(), wxConvUTF8 ));

  setActiveWorkspacesText();
//  refreshMenuHints();

  // These are here because no UpdateUI for MenuBar
  wxUpdateUIEvent tmpEvent;
  OnMenuHintUpdate( tmpEvent );
}


std::string paraverMain::buildCfgFullPath( const std::string& cfgStr ) const
{
  wxFileName tmpCFG( wxString::FromUTF8( cfgStr.c_str() ) );
  if ( tmpCFG.IsRelative() )
  {
    wxString tmpGlobalCFGs( localKernel->getDistributedCFGsPath().c_str(), wxConvUTF8 );
    tmpCFG.MakeAbsolute( tmpGlobalCFGs );
  }

  return std::string( tmpCFG.GetFullPath().mb_str() );
}


void paraverMain::refreshMenuHints()
{
  bool separator = false;

  // Destroy previous menu hints if any
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

   // Create updated menu hints
  size_t currentWorkspace = 0;
  for ( vector< string >::iterator it = traceWorkspaces[ getCurrentTrace() ].begin(); it != traceWorkspaces[ getCurrentTrace()  ].end(); ++it )
  {
    wxString currentWorkspaceName = wxString::FromUTF8( it->c_str() );
    wxMenu *currentWorkspaceMenu = new wxMenu();
    wxMenu *discardedWorkspaceMenu = new wxMenu();

    // Build combined hints DISTRIBUTED + USER_DEFINED
    std::vector< std::pair< std::string, std::string > > currentHints;
    if( currentWorkspace < firstUserWorkspace[ getCurrentTrace() ] ) // Distributed workspaces
    {
      currentHints = workspacesManager->getWorkspace( *it, TWorkspaceSet::DISTRIBUTED ).getHintCFGs();

      // Add the hints of a user defined workspace if it has the same name and event types
      if( workspacesManager->existWorkspace( *it, TWorkspaceSet::USER_DEFINED ) )
      {
        vector< WorkspaceValue > tmpDistAutoTypes = workspacesManager->getWorkspace( *it, TWorkspaceSet::DISTRIBUTED ).getAutoTypes();
        sort( tmpDistAutoTypes.begin(), tmpDistAutoTypes.end() );
        vector< WorkspaceValue > tmpUserAutoTypes = workspacesManager->getWorkspace( *it, TWorkspaceSet::USER_DEFINED ).getAutoTypes();
        sort( tmpUserAutoTypes.begin(), tmpUserAutoTypes.end() );
        if( includes( tmpDistAutoTypes.begin(), tmpDistAutoTypes.end(),
                      tmpUserAutoTypes.begin(), tmpUserAutoTypes.end() ) )
        {
          currentHints.push_back( std::pair< std::string, std::string >( "WXSEPARATOR", "WXSEPARATOR" ) );
          std::vector< std::pair< std::string, std::string > > tmpHints = workspacesManager->getWorkspace( *it, TWorkspaceSet::USER_DEFINED ).getHintCFGs();
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
      if( workspacesManager->existWorkspace( *it, TWorkspaceSet::DISTRIBUTED ) )
        currentWorkspaceName += wxT( "#2" );
      currentHints = workspacesManager->getWorkspace( *it, TWorkspaceSet::USER_DEFINED ).getHintCFGs();
    }

    // Iterate through combined hints to build wxwidgets menu detecting trace events + workspace state to discard non-relevant hints
    for ( std::vector<std::pair<std::string,std::string> >::iterator itHints = currentHints.begin(); itHints != currentHints.end(); ++itHints )
    {
      if( (*itHints).first == "WXSEPARATOR" && (*itHints).second == "WXSEPARATOR" )
      {
        currentWorkspaceMenu->AppendSeparator();
        continue;
      }

      wxString tmpName = getHintComposed( *itHints );
      MenuHintFile *tmpHintFile = new MenuHintFile();
      wxMenuItem *currentHint;

      tmpHintFile->fileName = buildCfgFullPath( (*itHints).first );

      // Build submenus
      bool isStatesWorkspace =
              ( workspacesManager->existWorkspace( *it, TWorkspaceSet::USER_DEFINED ) && 
                workspacesManager->getWorkspace( *it, TWorkspaceSet::USER_DEFINED ).getType() == WorkspaceType::STATE ) ||
              ( workspacesManager->existWorkspace( *it, TWorkspaceSet::DISTRIBUTED ) && 
                workspacesManager->getWorkspace( *it, TWorkspaceSet::DISTRIBUTED ).getType() == WorkspaceType::STATE );
      if ( isStatesWorkspace ||
           CFGLoader::detectAnyEventTypeInCFG( tmpHintFile->fileName,
                                               getCurrentTrace()->getLoadedEvents().begin(),
                                               getCurrentTrace()->getLoadedEvents().end(),
                                               getCurrentTrace()->getEventLabels() ) )
      {
        currentHint = new wxMenuItem( currentWorkspaceMenu, wxID_ANY, tmpName );
        currentWorkspaceMenu->Append( currentHint );
      }
      else
      {
        if( paraverConfig->getWorkspacesHintsDiscardedSubmenu() )
        {
          currentHint = new wxMenuItem( discardedWorkspaceMenu, wxID_ANY, tmpName );
          discardedWorkspaceMenu->Append( currentHint );
        }
        else
        {
          currentHint = new wxMenuItem( currentWorkspaceMenu, wxID_ANY, tmpName );
          currentWorkspaceMenu->Append( currentHint );
          currentHint->Enable( false );
        }
      }
      currentHint->SetRefData( tmpHintFile );
      Connect( currentHint->GetId(),
               wxEVT_COMMAND_MENU_SELECTED,
               (wxObjectEventFunction)&paraverMain::OnHintClick );
    }

    if ( discardedWorkspaceMenu->GetMenuItemCount() > 0 )
      currentWorkspaceMenu->AppendSubMenu( discardedWorkspaceMenu, _( "Discarded" ) );
    else
      delete discardedWorkspaceMenu;

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

#ifdef _WIN32
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
    wxString errMessage = wxString::FromUTF8( whichFileName.c_str() ) + _( " isn't a valid session file." );
    wxMessageDialog message( this, errMessage, _( "Invalid file" ), wxOK );
    raiseCurrentWindow = false;
    message.ShowModal();
    raiseCurrentWindow = true;
  }
}


bool paraverMain::DoLoadTrace( const string &path )
{
  Trace *tr = nullptr;
  bool loaded = true;

  canServeSignal = false;

  // Append whole path.
  wxFileName tmpFileName( wxString( path.c_str(), wxConvUTF8 ) );

#ifdef _WIN32
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
                      wxString::FromUTF8( tmpPath.c_str() ) +
                      wxString( wxT( "\n\nexceeds the maximum loadable defined"
                                     " in Preferences.\n\nWould you like to cut/filter the trace?" ) ),
            wxT( "Reduce trace size" ),
            wxYES_NO|wxCANCEL|wxICON_QUESTION );

    bool tmpResult = true;
    switch( maxSizeDialog.ShowModal() )
    {
      case wxID_YES:
        //tmpResult = ShowCutTraceWindow( tmpPath, true );
        ShowCutTraceWindow( tmpPath, true );


        // canServeSignal = true;
        return tmpResult;
        break;

      case wxID_NO:
        break;

      case wxID_CANCEL:
        canServeSignal = true;
        return false;
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
    if( paraverMain::dialogProgress == nullptr )
      paraverMain::dialogProgress = new wxProgressDialog( wxT("Loading trace..."),
                                                          wxT(""),
                                                          numeric_limits<PRV_INT16>::max(),
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

    paraverMain::dialogProgress->Pulse( wxString::FromUTF8( reducePath.c_str() ) );
    paraverMain::dialogProgress->Fit();
    paraverMain::dialogProgress->Show();

    tr = Trace::create( localKernel, tmpPath, false, progress );
    tr->setInstanceNumber( traceInstance[ std::string( tmpFileName.GetFullName().mb_str() ) ]++ );

    loadedTraces.push_back( tr );
    currentTrace = loadedTraces.size() - 1;
    setTraceWorkspaces( tr );

    wxTreeCtrl *newTree = createTree( imageList );
    newTree->Connect( wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler( paraverMain::OnTreeKeyPress ), nullptr, this );

    if( paraverConfig->getGlobalFullTracePath() )
      choiceWindowBrowser->AddPage( newTree, wxString::FromUTF8( tr->getFileNameNumbered().c_str() ) );
    else
      choiceWindowBrowser->AddPage( newTree, wxFileName( wxString::FromUTF8( tr->getFileNameNumbered().c_str() ) ).GetFullName() );
    choiceWindowBrowser->ChangeSelection( choiceWindowBrowser->GetPageCount() - 1 );

    previousTraces->add( tmpPath );

    traceLoadedBefore = true;
    tracePath = tmpFileName.GetPath();
  }
  catch( ParaverKernelException& ex )
  {
    loaded = false;
    wxMessageDialog message( this, wxString::FromUTF8( ex.what() ), _( "Error loading trace" ), wxOK );
    raiseCurrentWindow = false;
    message.ShowModal();
    raiseCurrentWindow = true;
  }
  paraverMain::dialogProgress->Show( false );
  delete paraverMain::dialogProgress;
  paraverMain::dialogProgress = nullptr;
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
    wxString errMessage = wxString::FromUTF8( path.c_str() ) + _( " isn't a valid cfg." );
    wxMessageDialog message( this, errMessage, _( "Invalid file" ), wxOK );
    raiseCurrentWindow = false;
    message.ShowModal();
    raiseCurrentWindow = true;
  }
  else
  {
    vector<Timeline *> newWindows;
    vector<Histogram *> newHistograms;
    SaveOptions options;

    Trace *tmpTraceToUse = getCurrentTrace();

    if( !CFGLoader::loadCFG( localKernel, path, tmpTraceToUse,
                             newWindows, newHistograms, options ) )
    {
      wxString errMessage = wxString::FromUTF8( path.c_str() ) + _( " failed to load in:\n'" ) + wxString::FromUTF8( CFGLoader::errorLine.c_str() ) + _( "'" );
      wxMessageDialog message( this, errMessage, _( "Loading error" ), wxOK|wxICON_ERROR );
      raiseCurrentWindow = false;
      message.ShowModal();
      raiseCurrentWindow = true;
      for ( PRV_UINT32 i = 0; i < newHistograms.size(); ++i )
      {
        if ( newHistograms[ i ] != nullptr )
          delete newHistograms[ i ];
      }
      newHistograms.clear();

      for ( PRV_UINT32 i = 0; i < newWindows.size(); ++i )
      {
        if ( newWindows[ i ] != nullptr )
          delete newWindows[ i ];
      }
      newWindows.clear();
    }
    else
    {
      if( !CFGLoader::errorLine.empty() )
      {
        wxString errMessage = wxString::FromUTF8( path.c_str() ) + _( " loaded with error in:\n'" ) + wxString::FromUTF8( CFGLoader::errorLine.c_str() ) + _( "'" );
        wxMessageDialog message( this, errMessage, _( "Loading error" ), wxOK|wxICON_EXCLAMATION );
        raiseCurrentWindow = false;
        message.ShowModal();
        raiseCurrentWindow = true;
      }

      wxTreeItemId child;
      for( vector<Timeline *>::iterator it = newWindows.begin(); it != newWindows.end(); ++it )
      {
        wxTreeCtrl *allTracesPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 );
        wxTreeCtrl *currentPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( getTracePosition( tmpTraceToUse ) + 1 );

        if ( (*it)->getChild() == nullptr )
          BuildTree( this, allTracesPage, allTracesPage->GetRootItem(), currentPage, currentPage->GetRootItem(), *it );

        if ( it + 1 == newWindows.end() )
        {
          currentTimeline = *it;
          currentHisto = nullptr;
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
#if !__WXGTK__
        gHistogram* tmpHisto = new gHistogram( this, wxID_ANY, wxString::FromUTF8( (*it)->getName().c_str() ), tmpPos );
#else
        gHistogram* tmpHisto = new gHistogram( this, wxID_ANY, wxString::FromUTF8( (*it)->getName().c_str() ) );
#endif
        tmpHisto->SetHistogram( *it );

        appendHistogram2Tree( tmpHisto );
        LoadedWindows::getInstance()->add( (*it) );

        tmpHisto->SetClientSize( wxSize( (*it)->getWidth(), (*it)->getHeight() ) );
        if( (*it)->getShowWindow() )
        {
#if __WXGTK__
          tmpHisto->Move( tmpPos );
#endif
          tmpHisto->Show();
        }
        tmpHisto->GetHistogram()->setRecalc( true );

        if ( it + 1 == newHistograms.end() )
        {
          currentTimeline = nullptr;
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
    tracePath = wxString::FromUTF8( paraverConfig->getGlobalTracesPath().c_str() );

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
   CFGPath =  wxString::FromUTF8( paraverConfig->getGlobalCFGsPath().c_str() );

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
  #ifdef _WIN32
  string file( ParaverConfig::getInstance()->getGlobalSessionPath() + "\\paraver.session" );
  #else
  string file( ParaverConfig::getInstance()->getGlobalSessionPath() + "/paraver.session" );
  #endif
  DoLoadSession( file );
}


void paraverMain::OnMenuLoadAutoSavedSessionSelect( wxCommandEvent& event )
{
  //MultiSessionLoad( true );
  #ifdef _WIN32
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


void paraverMain::exitManager()
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
  exitManager();
}

/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_PARAVERMAIN
 */

void paraverMain::OnCloseWindow( wxCloseEvent& event )
{
  if ( event.CanVeto() )
  {
    exitManager();
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
    srand( time( nullptr ));
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
  if (name == wxT("icons/information.xpm"))
  {
    wxBitmap bitmap(information);
    return bitmap;
  }
  else if (name == wxT("icons/new_window.xpm"))
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
  else if (name == wxT("icons/autoredraw_refresh.xpm"))
  {
    wxBitmap bitmap(autoredraw_refresh_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/three_dots.xpm"))
  {
    wxBitmap bitmap(three_dots_xpm);
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


void paraverMain::spreadSetChangedRecursive( Timeline *whichWindow )
{
  whichWindow->setChanged( true );

  if ( whichWindow->getChild() != nullptr )
    spreadSetChangedRecursive( whichWindow->getChild() );

  set<Histogram *> tmpHistograms = whichWindow->getHistograms();
  for( set<Histogram *>::iterator it = tmpHistograms.begin(); it != tmpHistograms.end(); ++it )
    (*it)->setChanged( true );
}


void paraverMain::spreadSetChanged( Timeline *whichWindow )
{
  if ( isCFG4DModeDisabled() )
    whichWindow->setChanged( true );
  else
    spreadSetChangedRecursive( whichWindow );
}


void paraverMain::spreadSetRedrawRecursive( Timeline *whichWindow )
{
  whichWindow->setRedraw( true );

  if ( whichWindow->getChild() != nullptr )
    spreadSetRedrawRecursive( whichWindow->getChild() );

  set<Histogram *> tmpHistograms = whichWindow->getHistograms();
  for( set<Histogram *>::iterator it = tmpHistograms.begin(); it != tmpHistograms.end(); ++it )
    (*it)->setRecalc( true );
}


void paraverMain::spreadSetRedraw( Timeline *whichWindow )
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

std::string getCFG4DParameterOriginalName( Timeline *whichWindow, TWindowLevel whichLevel, TParamIndex whichParam )
{
  return whichWindow->getCFG4DParameterOriginalName( whichLevel, whichParam );
}

// Dummy function: no sense for histograms because they don't have semantic function parameters,
//                 but needed for templated function linkedSetPropertyValue.
std::string getCFG4DParameterOriginalName( Histogram *whichWindow, TWindowLevel whichLevel, TParamIndex whichParam )
{
  return "";
}

template< typename T >
bool paraverMain::linkedSetPropertyValue( T *whichWindow,
                                          wxPropertyGridEvent& event,
                                          wxPGProperty *property,
                                          const string& propName,
                                          PropertyClientData *whichClientData )
{
  bool isLinkedProperty = false;
  string propOriginalName = propName;

  if( propName == SingleTimelinePropertyLabels[ SINGLE_FUNCTIONPARAMETERS ] ||
      propName == DerivedTimelinePropertyLabels[ DERIVED_FUNCTIONPARAMETERS ] )
  {
    propOriginalName = getCFG4DParameterOriginalName( whichWindow,
                                                      whichClientData->semanticLevel,
                                                      whichClientData->numParameter );
  }
  else if( propName == SingleTimelinePropertyLabels[ SINGLE_EXTRAFUNCTIONPARAMETERS ] ||
           propName == DerivedTimelinePropertyLabels[ DERIVED_EXTRAFUNCTIONPARAMETERS ] )
  {
    // TODO: extra compose parameters for linked properties
  }

  TWindowsSet timelines;
  CFGS4DGlobalManager::getInstance()->getLinks( whichWindow->getCFGS4DIndexLink(),
                                                whichWindow->getCFGS4DGroupLink( propOriginalName ),
                                                timelines );
  if( timelines.size() > 0 )
  {
    isLinkedProperty = true;
    for( TWindowsSet::iterator it = timelines.begin(); it != timelines.end(); ++it )
      SetPropertyValue( event, property, propName, whichClientData, *it, nullptr );
  }

  THistogramsSet histograms;
  CFGS4DGlobalManager::getInstance()->getLinks( whichWindow->getCFGS4DIndexLink(), 
                                                whichWindow->getCFGS4DGroupLink( propName ),
                                                histograms );
  if( histograms.size() > 0 )
  {
    isLinkedProperty = true;
    for( THistogramsSet::iterator it = histograms.begin(); it != histograms.end(); ++it )
      SetPropertyValue( event, property, propName, whichClientData, nullptr, *it );
  }

  return isLinkedProperty;
}

/*!
 * wxEVT_PG_CHANGED event handler for ID_FOREIGN
 */
void paraverMain::OnPropertyGridChange( wxPropertyGridEvent& event )
{
  wxPGProperty *property = event.GetProperty();
  if( property == nullptr )
    return;

  if( property->GetDisplayedString() == _( "" ) )
  {
    property->SetValue( propertyPrevValue );
    return;
  }

  PropertyClientData *tmpClientData = (PropertyClientData *)property->GetClientData();
  if( tmpClientData == nullptr )
    return;

  wxparaverApp::mainWindow->SetSomeWinIsRedraw( true );
  
  const string& propName = tmpClientData->propName;

  if( propName == "Mode" )
  {
    if( tmpClientData->ownerTimeline != nullptr )
    {
      tmpClientData->ownerTimeline->setCFG4DMode( property->GetValue().GetLong() == 1 );
      tmpClientData->ownerTimeline->setChanged( true );
    }
    else if ( tmpClientData->ownerHistogram != nullptr )
    {
      tmpClientData->ownerHistogram->setCFG4DMode( property->GetValue().GetLong() == 1 );
      tmpClientData->ownerHistogram->setChanged( true );
    }
  }
  else
  {
    if( tmpClientData->ownerTimeline != nullptr )
    {
      if ( !linkedSetPropertyValue( tmpClientData->ownerTimeline, event, property, propName, tmpClientData ) )
        SetPropertyValue( event, property, propName, tmpClientData, tmpClientData->ownerTimeline, nullptr );
    }
    else if ( tmpClientData->ownerHistogram != nullptr )
    {
      if ( !linkedSetPropertyValue( tmpClientData->ownerHistogram, event, property, propName, tmpClientData ) )
        SetPropertyValue( event, property, propName, tmpClientData, nullptr, tmpClientData->ownerHistogram );
    }
  }

  wxparaverApp::mainWindow->SetSomeWinIsRedraw( false );
}

string getPropertyName( Timeline *whichWindow,
                        Histogram *whichHistogram,
                        TSingleTimelineProperties singleIndex,
                        TDerivedTimelineProperties derivedIndex,
                        THistogramProperties histogramIndex )
{
  if( whichWindow != nullptr )
  {
    if( whichWindow->isDerivedWindow() && derivedIndex != DERIVED_NULL )
      return DerivedTimelinePropertyLabels[ derivedIndex ];
    else if( singleIndex != SINGLE_NULL )
      return SingleTimelinePropertyLabels[ singleIndex ];
  }
  else if( whichHistogram != nullptr && histogramIndex != HISTOGRAM_NULL )
    return HistogramPropertyLabels[ histogramIndex ];

  return "";
}

void paraverMain::SetPropertyValue( wxPropertyGridEvent& event,
                                    wxPGProperty *property,
                                    const string& propName,
                                    PropertyClientData *whichClientData,
                                    Timeline *whichTimeline,
                                    Histogram *whichHistogram )
{
  if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NAME, DERIVED_NAME, HISTOGRAM_NAME ) )
  {
    wxString tmpName = property->GetValue().GetString();
    if( whichTimeline != nullptr )
    {
      whichTimeline->setName( std::string( tmpName.mb_str() ) );
      whichTimeline->setChanged( true );
    }
    else if( whichHistogram != nullptr )
    {
      whichHistogram->setName( std::string( tmpName.mb_str() ) );
      whichHistogram->setChanged( true );
    }
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_BEGINTIME, DERIVED_BEGINTIME, HISTOGRAM_BEGINTIME ) )
  {
    TTime tmpValue;
    if( whichTimeline != nullptr )
    {
      bool done = LabelConstructor::getTimeValue( std::string( property->GetValue().GetString().mb_str()),
                                                  whichTimeline->getTimeUnit(),
                                                  ParaverConfig::getInstance()->getTimelinePrecision(),
                                                  tmpValue );

      if (!done)
        tmpValue = property->GetValue().GetDouble();

      whichTimeline->setWindowBeginTime( whichTimeline->windowUnitsToTraceUnits( tmpValue ) );

      // modify current zoom directly
      pair< TTime, TTime > zoomInfo = whichTimeline->getZoomFirstDimension();
      zoomInfo.first = property->GetValue().GetDouble(); // begin
      whichTimeline->setZoomFirstDimension( zoomInfo );

      whichTimeline->setChanged( true );
      whichTimeline->setRedraw( true );
    }
    else if( whichHistogram != nullptr )
    {
      bool done = LabelConstructor::getTimeValue( std::string( property->GetValue().GetString().mb_str()),
                                                  whichHistogram->getControlWindow()->getTimeUnit(),
                                                  ParaverConfig::getInstance()->getTimelinePrecision(),
                                                  tmpValue );

      if (!done)
        tmpValue = property->GetValue().GetDouble();

      whichHistogram->setWindowBeginTime( whichHistogram->getControlWindow()->windowUnitsToTraceUnits( tmpValue ) );
      whichHistogram->setChanged( true );
      whichHistogram->setRecalc( true );
    }
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_ENDTIME, DERIVED_ENDTIME, HISTOGRAM_ENDTIME ) )
  {
    TTime tmpValue;
    if( whichTimeline != nullptr )
    {
      bool done = LabelConstructor::getTimeValue( std::string( property->GetValue().GetString().mb_str()),
                                                  whichTimeline->getTimeUnit(),
                                                  ParaverConfig::getInstance()->getTimelinePrecision(),
                                                  tmpValue );

      if (!done)
        tmpValue = property->GetValue().GetDouble();

      whichTimeline->setWindowEndTime( whichTimeline->windowUnitsToTraceUnits( tmpValue ) );

      // modify current zoom directly
      pair< TTime, TTime > zoomInfo = whichTimeline->getZoomFirstDimension();
      zoomInfo.second = property->GetValue().GetDouble(); // end
      whichTimeline->setZoomFirstDimension( zoomInfo );

      whichTimeline->setChanged( true );
      whichTimeline->setRedraw( true );
    }
    else if( whichHistogram != nullptr )
    {
      bool done = LabelConstructor::getTimeValue( std::string( property->GetValue().GetString().mb_str()),
                                                  whichHistogram->getControlWindow()->getTimeUnit(),
                                                  ParaverConfig::getInstance()->getTimelinePrecision(),
                                                  tmpValue );

      if (!done)
        tmpValue = property->GetValue().GetDouble();

      whichHistogram->setWindowEndTime( whichHistogram->getControlWindow()->windowUnitsToTraceUnits( tmpValue ) );
      whichHistogram->setChanged( true );
      whichHistogram->setRecalc( true );
    }
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_SEMANTICMINIMUM, DERIVED_SEMANTICMINIMUM, HISTOGRAM_NULL ) )
  {
    double tmpValue = property->GetValue().GetDouble();
    whichTimeline->setMinimumY( tmpValue );
    whichTimeline->setRedraw( true );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_SEMANTICMAXIMUM, DERIVED_SEMANTICMAXIMUM, HISTOGRAM_NULL ) )
  {
    double tmpValue = property->GetValue().GetDouble();
    whichTimeline->setMaximumY( tmpValue );
    whichTimeline->setRedraw( true );
  }
  // Control Timeline related properties
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NULL, DERIVED_NULL, HISTOGRAM_CONTROLWINDOW ) )
  {
    whichHistogram->setControlWindow( ( ( prvTimelineTreeProperty * )property )->getSelectedWindow() );
    whichHistogram->setRecalc( true );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NULL, DERIVED_NULL, HISTOGRAM_CONTROLMINIMUM ) )
  {
    whichHistogram->setControlMin( property->GetValue().GetDouble() );

    // modify current zoom directly
    pair< HistogramProxy::TZoomInfo, HistogramProxy::TZoomInfo > zoomInfo = whichHistogram->getZoomFirstDimension();
    zoomInfo.first.begin = property->GetValue().GetDouble(); // minimum
    whichHistogram->setZoomFirstDimension( zoomInfo );

    whichHistogram->setCompute2DScale( false );
    whichHistogram->setRecalc( true );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NULL, DERIVED_NULL, HISTOGRAM_CONTROLMAXIMUM ) )
  {
    whichHistogram->setControlMax( property->GetValue().GetDouble() );

    // modify current zoom directly
    pair< HistogramProxy::TZoomInfo, HistogramProxy::TZoomInfo > zoomInfo = whichHistogram->getZoomFirstDimension();
    zoomInfo.first.end = property->GetValue().GetDouble(); // maximum
    whichHistogram->setZoomFirstDimension( zoomInfo );

    whichHistogram->setCompute2DScale( false );
    whichHistogram->setRecalc( true );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NULL, DERIVED_NULL, HISTOGRAM_CONTROLDELTA ) )
  {
    if( property->GetValue().GetDouble() == 0 )
    {
      property->SetValue( whichHistogram->getControlDelta() );
      return;
    }
    whichHistogram->setControlDelta( property->GetValue().GetDouble() );

    whichHistogram->setUseCustomDelta( true );

    // modify current zoom directly
    pair< HistogramProxy::TZoomInfo, HistogramProxy::TZoomInfo > zoomInfo = whichHistogram->getZoomFirstDimension();
    zoomInfo.second.begin = property->GetValue().GetDouble(); // delta
    whichHistogram->setZoomFirstDimension( zoomInfo );

    whichHistogram->setCompute2DScale( false );
    whichHistogram->setRecalc( true );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NULL, DERIVED_NULL, HISTOGRAM_NUMCOLUMNS ) )
  {
    unsigned long newNumColumns;
    wxString numColumnsStr = property->GetValueAsString();

    vector<wxString> listNumColumnsChoices;
    NumColumnsChoices::createChoices( [&]( wxString el ) { listNumColumnsChoices.push_back( el ); } );

    if( numColumnsStr == listNumColumnsChoices[ NumColumnsChoices::CUSTOM_DELTA ] )
    {
      whichHistogram->setUseCustomDelta( true );
      return;
    }
    else if( numColumnsStr == listNumColumnsChoices[ NumColumnsChoices::DEFAULT ] )
    {
      newNumColumns = ParaverConfig::getInstance()->getHistogramNumColumns();
    }
    else if( !numColumnsStr.ToULong( &newNumColumns ) || numColumnsStr[ 0 ] == '-' )
    {
      property->SetValue( propertyPrevValue );
      return;
    }

    whichHistogram->setUseCustomDelta( false );
    whichHistogram->setNumColumns( newNumColumns );
    whichHistogram->setCompute2DScale( false );
    whichHistogram->setRecalc( true );
  }
  // Data Timeline related properties
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NULL, DERIVED_NULL, HISTOGRAM_DATAWINDOW ) )
  {
    whichHistogram->setDataWindow( ( ( prvTimelineTreeProperty * )property )->getSelectedWindow() );
    whichHistogram->setRecalc( true );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NULL, DERIVED_NULL, HISTOGRAM_MINIMUMGRADIENT ) )
  {
    whichHistogram->setMinGradient( property->GetValue().GetDouble() );
    whichHistogram->setComputeGradient( false );
    whichHistogram->setRedraw( true );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NULL, DERIVED_NULL, HISTOGRAM_MAXIMUMGRADIENT ) )
  {
    whichHistogram->setMaxGradient( property->GetValue().GetDouble() );
    whichHistogram->setComputeGradient( false );
    whichHistogram->setRedraw( true );
  }
  // Histogram related properties
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NULL, DERIVED_NULL, HISTOGRAM_TYPE ) )
  {
    if( property->GetValue().GetLong() == 0 )
      whichHistogram->setCurrentStat( whichHistogram->getFirstCommStatistic() );
    else
      whichHistogram->setCurrentStat( whichHistogram->getFirstStatistic() );
    whichHistogram->setRedraw( true );
    whichHistogram->setChanged( true );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NULL, DERIVED_NULL, HISTOGRAM_STATISTIC ) )
  {
    bool getOriginalList = ( !whichHistogram->getCFG4DEnabled() || !whichHistogram->getCFG4DMode() );
    if ( getOriginalList )
    {
      whichHistogram->setCurrentStat( std::string( property->GetDisplayedString().mb_str() ) );
    }
    else
    {
      map< string, string > statList( whichHistogram->getCFG4DStatisticsAliasList() );
      string selected( std::string( property->GetDisplayedString().mb_str() ) );
      for ( map< string, string >::iterator it = statList.begin(); it != statList.end(); ++it )
      {
        if ( it->second == selected )
        {
          selected = it->first;
        }
      }
      whichHistogram->setCurrentStat( selected );
    }

    whichHistogram->setRedraw( true );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NULL, DERIVED_NULL, HISTOGRAM_3D3RDWINDOW ) )
  {
    if( ( ( prvTimelineTreeProperty * )property )->getSelectedWindow() == nullptr )
      whichHistogram->clearExtraControlWindow();
    else
      whichHistogram->setExtraControlWindow( ( ( prvTimelineTreeProperty * )property )->getSelectedWindow() );
    whichHistogram->setRecalc( true );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NULL, DERIVED_NULL, HISTOGRAM_3DMINIMUM ) )
  {
    whichHistogram->setExtraControlMin( property->GetValue().GetDouble() );
    whichHistogram->setCompute3DScale( false );
    whichHistogram->setRecalc( true );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NULL, DERIVED_NULL, HISTOGRAM_3DMAXIMUM ) )
  {
    whichHistogram->setExtraControlMax( property->GetValue().GetDouble() );
    whichHistogram->setCompute3DScale( false );
    whichHistogram->setRecalc( true );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NULL, DERIVED_NULL, HISTOGRAM_3DDELTA ) )
  {
    if( property->GetValue().GetDouble() == 0 )
    {
      property->SetValue( whichHistogram->getExtraControlDelta() );
      return;
    }

    whichHistogram->setExtraControlDelta( property->GetValue().GetDouble() );
    whichHistogram->setCompute3DScale( false );
    whichHistogram->setRecalc( true );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NULL, DERIVED_NULL, HISTOGRAM_3DPLANE ) )
  {
    // Disable fixed sort, because different planes couldn't have same columns
    if( whichHistogram->getFixedSemanticSort() )
      whichHistogram->setFixedSemanticSort( false );
    
    if( whichHistogram->isCommunicationStat( whichHistogram->getCurrentStat() ) )
      whichHistogram->setCommSelectedPlane( property->GetValue().GetLong() );
    else
      whichHistogram->setSelectedPlane( property->GetValue().GetLong() );
    whichHistogram->setRedraw( true );
  }

  // Timeline related properties
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_LEVEL, DERIVED_LEVEL, HISTOGRAM_NULL ) )
  {
    whichTimeline->setLevel( static_cast<TTraceLevel>( property->GetValue().GetLong() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_TIMEUNIT, DERIVED_TIMEUNIT, HISTOGRAM_NULL ) )
  {
    whichTimeline->setTimeUnit( (TTimeUnit)property->GetValue().GetLong() );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMMLOGICAL, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    whichTimeline->getFilter()->setLogical( property->GetValue().GetBool() );
    whichTimeline->setRedraw( true );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMMPHYSICAL, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    whichTimeline->getFilter()->setPhysical( property->GetValue().GetBool() );
    whichTimeline->setRedraw( true );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMMFROMFUNCTION, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    whichTimeline->getFilter()->setCommFromFunction( std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMMFROMVALUES, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    prvRowsSelectionProperty *myProperty = (prvRowsSelectionProperty *)event.GetProperty();

    Filter *filter = whichTimeline->getFilter();
    filter->clearCommFrom();
    vector< TObjectOrder > selection;
    myProperty->GetSelectionAsVector( whichTimeline->getLevel(), selection );
    for( vector< TObjectOrder >::iterator it = selection.begin(); it != selection.end(); ++it )
    {
      filter->insertCommFrom( long(*it) );
    }

    spreadSetRedraw( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMMFROMTOOP, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    long op = property->GetValue().GetLong();
    Filter *filter = whichTimeline->getFilter();
    if( op == 0 )
      filter->setOpFromToAnd();
    else if( op == 1 )
      filter->setOpFromToOr();

    spreadSetRedraw( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMMTOFUNCTION, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    whichTimeline->getFilter()->setCommToFunction( std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMMTOVALUES, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    prvRowsSelectionProperty *myProperty = (prvRowsSelectionProperty *)event.GetProperty();

    Filter *filter = whichTimeline->getFilter();
    filter->clearCommTo();
    vector< TObjectOrder > selection;
    myProperty->GetSelectionAsVector( whichTimeline->getLevel(), selection );
    for( vector< TObjectOrder >::iterator it = selection.begin(); it != selection.end(); ++it )
    {
      filter->insertCommTo( long(*it) );
    }

    spreadSetRedraw( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMMTAGFUNCTION, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    whichTimeline->getFilter()->setCommTagFunction( std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMMTAGVALUES, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    Filter *filter = whichTimeline->getFilter();
    filter->clearCommTags();
    wxArrayString value = property->GetValue().GetArrayString();
    for( unsigned int idx = 0; idx < value.GetCount(); idx++ )
    {
      long long tmpLong;
      value[ idx ].ToLongLong( &tmpLong );
      filter->insertCommTag( tmpLong );
    }

    spreadSetRedraw( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMMTAGSIZEOP, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    long op = property->GetValue().GetLong();
    Filter *filter = whichTimeline->getFilter();
    if( op == 0 )
      filter->setOpTagSizeAnd();
    else if( op == 1 )
      filter->setOpTagSizeOr();

    spreadSetRedraw( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMMSIZEFUNCTION, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    whichTimeline->getFilter()->setCommSizeFunction( std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMMSIZEVALUES, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    Filter *filter = whichTimeline->getFilter();
    filter->clearCommSizes();
    wxArrayString value = property->GetValue().GetArrayString();
    for( unsigned int idx = 0; idx < value.GetCount(); idx++ )
    {
      long long tmpLong;
      value[ idx ].ToLongLong( &tmpLong );
      filter->insertCommSize( tmpLong );
    }

    spreadSetRedraw( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMMBANDWIDTHFUNCTION, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    whichTimeline->getFilter()->setBandWidthFunction( std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMMBANDWIDTHVALUES, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    Filter *filter = whichTimeline->getFilter();
    filter->clearBandWidth();
    wxArrayString value = property->GetValue().GetArrayString();
    for( unsigned int idx = 0; idx < value.GetCount(); idx++ )
    {
      double tmpDouble;
      value[ idx ].ToDouble( &tmpDouble );
      filter->insertBandWidth( tmpDouble );
    }

    spreadSetRedraw( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_EVENTTYPEFUNCTION, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    whichTimeline->getFilter()->setEventTypeFunction( std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_EVENTTYPEVALUES, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    Filter *filter = whichTimeline->getFilter();
    filter->clearEventTypes();
    wxArrayInt value = ( (prvEventTypeProperty *) property )->GetValueAsArrayInt();
    for( unsigned int idx = 0; idx < value.GetCount(); idx++ )
      filter->insertEventType( value[ idx ] );

    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_EVENTTYPEVALUESOP, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    long op = property->GetValue().GetLong();
    Filter *filter = whichTimeline->getFilter();
    if( op == 0 )
      filter->setOpTypeValueAnd();
    else if( op == 1 )
      filter->setOpTypeValueOr();

    spreadSetRedraw( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_EVENTVALUEFUNCTION, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    whichTimeline->getFilter()->setEventValueFunction( std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_EVENTVALUEVALUES, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    Filter *filter = whichTimeline->getFilter();
    filter->clearEventValues();

    wxArrayString value = property->GetValue().GetArrayString();
    for( unsigned int idx = 0; idx < value.GetCount(); idx++ )
    {
      double tmpDouble;
      value[ idx ].ToDouble( &tmpDouble );
      filter->insertEventValue( tmpDouble );
    }

    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_EXTRATOPCOMPOSE1, DERIVED_EXTRATOPCOMPOSE1, HISTOGRAM_NULL ) )
  {
    size_t position = whichClientData->extraTopComposeLevel;

    int reversedIndex = (int)position;
    int maxPos = (int)whichTimeline->getExtraNumPositions( TOPCOMPOSE1 );
    position = (size_t)(maxPos - reversedIndex);

    whichTimeline->setExtraLevelFunction( TOPCOMPOSE1, position, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_TOPCOMPOSE1, DERIVED_TOPCOMPOSE1, HISTOGRAM_NULL ) )
  {
    whichTimeline->setLevelFunction( TOPCOMPOSE1, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_TOPCOMPOSE2, DERIVED_TOPCOMPOSE2, HISTOGRAM_NULL ) )
  {
    whichTimeline->setLevelFunction( TOPCOMPOSE2, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NULL, DERIVED_SHIFT1, HISTOGRAM_NULL ) )
  {
    whichTimeline->setShift( 0, property->GetValue().GetInteger() );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NULL, DERIVED_FACTOR1, HISTOGRAM_NULL ) )
  {
    whichTimeline->setFactor( 0, property->GetValue().GetDouble() );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NULL, DERIVED_DERIVED, HISTOGRAM_NULL ) )
  {
    whichTimeline->setLevelFunction( DERIVED, std::string( property->GetDisplayedString().mb_str() ) );

    // Change Derived operation Icon: find icon in list
    int iconNumber = getIconNumber( whichTimeline );

    // Change Derived operation Icon: set both trees, global and trace tree
    wxTreeCtrl *globalTreePage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 );
    wxTreeCtrl *currentTraceTreePage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( currentTrace + 1 );
    bool found;
    wxTreeItemId idInGlobalTree = getItemIdFromWindow( globalTreePage->GetRootItem(), whichTimeline, found );
    globalTreePage->SetItemImage( idInGlobalTree, iconNumber );
    wxTreeItemId idInCurrentTraceTree = getItemIdFromWindow( currentTraceTreePage->GetRootItem(), whichTimeline, found );
    currentTraceTreePage->SetItemImage( idInCurrentTraceTree, iconNumber );

    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NULL, DERIVED_FACTOR2, HISTOGRAM_NULL ) )
  {
    whichTimeline->setFactor( 1, property->GetValue().GetDouble() );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NULL, DERIVED_SHIFT2, HISTOGRAM_NULL ) )
  {
    whichTimeline->setShift( 1, property->GetValue().GetInteger() );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMPOSEWORKLOAD, DERIVED_COMPOSEWORKLOAD, HISTOGRAM_NULL ) )
  {
    whichTimeline->setLevelFunction( COMPOSEWORKLOAD, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_WORKLOAD, DERIVED_WORKLOAD, HISTOGRAM_NULL ) )
  {
    whichTimeline->setLevelFunction( WORKLOAD, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMPOSEAPPL, DERIVED_COMPOSEAPPL, HISTOGRAM_NULL ) )
  {
    whichTimeline->setLevelFunction( COMPOSEAPPLICATION, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_APPLICATION, DERIVED_APPLICATION, HISTOGRAM_NULL ) )
  {
    whichTimeline->setLevelFunction( APPLICATION, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMPOSETASK, DERIVED_COMPOSETASK, HISTOGRAM_NULL ) )
  {
    whichTimeline->setLevelFunction( COMPOSETASK, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_TASK, DERIVED_TASK, HISTOGRAM_NULL ) )
  {
    whichTimeline->setLevelFunction( TASK, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMPOSESYSTEM, DERIVED_COMPOSESYSTEM, HISTOGRAM_NULL ) )
  {
    whichTimeline->setLevelFunction( COMPOSESYSTEM, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_SYSTEM, DERIVED_SYSTEM, HISTOGRAM_NULL ) )
  {
    whichTimeline->setLevelFunction( SYSTEM, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMPOSENODE, DERIVED_COMPOSENODE, HISTOGRAM_NULL ) )
  {
    whichTimeline->setLevelFunction( COMPOSENODE, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_NODE, DERIVED_NODE, HISTOGRAM_NULL ) )
  {
    whichTimeline->setLevelFunction( NODE, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMPOSECPU, DERIVED_COMPOSECPU, HISTOGRAM_NULL ) )
  {
    whichTimeline->setLevelFunction( COMPOSECPU, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_CPU, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    whichTimeline->setLevelFunction( CPU, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_COMPOSETHREAD, DERIVED_COMPOSETHREAD, HISTOGRAM_NULL ) )
  {
    whichTimeline->setLevelFunction( COMPOSETHREAD, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_THREAD, DERIVED_NULL, HISTOGRAM_NULL ) )
  {
    whichTimeline->setLevelFunction( THREAD, std::string( property->GetDisplayedString().mb_str() ) );
    spreadSetRedraw( whichTimeline );
    spreadSetChanged( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_FUNCTIONPARAMETERS, DERIVED_FUNCTIONPARAMETERS, HISTOGRAM_NULL ) )
  {
    TParamIndex paramIdx = whichClientData->numParameter;
    TWindowLevel functionLevel = whichClientData->semanticLevel;

    wxArrayString valuesStr = property->GetValue().GetArrayString();
    TParamValue values;
    for( unsigned int idx = 0; idx < valuesStr.GetCount(); idx++ )
    {
      double tmpDouble;
      valuesStr[ idx ].ToDouble( &tmpDouble );
      values.push_back( tmpDouble );
    }

    whichTimeline->setFunctionParam( functionLevel, paramIdx, values );
    
    spreadSetRedraw( whichTimeline );
  }
  else if( propName == getPropertyName( whichTimeline, whichHistogram, SINGLE_EXTRAFUNCTIONPARAMETERS, DERIVED_EXTRAFUNCTIONPARAMETERS, HISTOGRAM_NULL ) )
  {
    TParamIndex paramIdx = whichClientData->numParameter;
    TWindowLevel functionLevel = whichClientData->semanticLevel;
    size_t extraTopComposeLevel = whichClientData->extraTopComposeLevel;

    wxArrayString valuesStr = property->GetValue().GetArrayString();
    TParamValue values;
    for( unsigned int idx = 0; idx < valuesStr.GetCount(); idx++ )
    {
      double tmpDouble;
      valuesStr[ idx ].ToDouble( &tmpDouble );
      values.push_back( tmpDouble );
    }

    whichTimeline->setExtraFunctionParam( functionLevel, extraTopComposeLevel - 1, paramIdx, values );
    
    spreadSetRedraw( whichTimeline );
  }

}


/*!
 * wxEVT_TREE_SEL_CHANGED event handler for wxID_ANY
 */
void paraverMain::OnTreeSelChanged( wxTreeEvent& event )
{
  wxTreeCtrl *tmpTree = static_cast<wxTreeCtrl *>( event.GetEventObject() );
  if( tmpTree->GetParent()->GetId() == ID_DIRCTRLFILES ||
      !event.GetItem().IsOk() )
  {
    event.Skip();
    return;
  }

  TreeBrowserItemData *itemData = static_cast<TreeBrowserItemData *>( tmpTree->GetItemData( event.GetItem() ) );

  endDragWindow = nullptr;
  if( gHistogram *histo = itemData->getHistogram() ) // Is a histogram?
  {
    currentHisto = histo->GetHistogram();
    currentWindow = (wxWindow *)histo;

    currentTimeline = nullptr;
    beginDragWindow = nullptr;

    if( histo->IsShown() )
      histo->Raise();
  }
  else if( gTimeline *timeline = itemData->getTimeline() ) // Is a timeline.
  {
    currentTimeline = timeline->GetMyWindow();
    beginDragWindow = timeline->GetMyWindow();
    currentWindow = (wxWindow *)timeline;

    currentHisto = nullptr;

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

  endDragWindow = nullptr;
  if( gHistogram *histo = itemData->getHistogram() )
  {
    Histogram *tmpHisto = histo->GetHistogram();

    beginDragWindow = nullptr;

    tmpHisto->setShowWindow( !tmpHisto->getShowWindow() );
    if( tmpHisto->getShowWindow() )
      histo->Raise();
  }
  else if( gTimeline *timeline = itemData->getTimeline() )
  {
    Timeline *tmpWin = timeline->GetMyWindow();

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
  endDragWindow = nullptr;

  if( gHistogram *histo = itemData->getHistogram() )
  {
    beginDragWindow = nullptr;
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
  vector< Timeline * > allWindows;
  LoadedWindows::getInstance()->getAll( allWindows );
  vector< Histogram * > allHistograms;
  bool destroyed = false;

  // Update loop and delete
  for( unsigned int iPage = 0; iPage < choiceWindowBrowser->GetPageCount(); ++iPage )
  {
    if( iPage > 0 && choiceWindowBrowser->GetSelection() > 0 )
      currentWindow = nullptr;
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
        if( iPage > 0 && choiceWindowBrowser->GetSelection() > 0 && currentWindow != nullptr )
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
  for( vector<Timeline *>::iterator it = allWindows.begin(); it != allWindows.end(); ++it )
  {
    if( (*it)->getDestroy() )
      continue;

    wxTreeCtrl *allTracesPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 );
    wxTreeCtrl *currentPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( currentTrace + 1 );

    if ( (*it)->getChild() == nullptr )
      BuildTree( this, allTracesPage, allTracesPage->GetRootItem(), currentPage, currentPage->GetRootItem(), *it );
  }

  for( vector<Histogram *>::iterator it = allHistograms.begin(); it != allHistograms.end(); ++it )
  {
    if( (*it)->getDestroy() )
      continue;
    gHistogram* tmpHisto = new gHistogram( this, wxID_ANY, wxString::FromUTF8( (*it)->getName().c_str() ) );
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
  if( currentHisto != nullptr )
  {
    lastTimeline = nullptr;
    if( lastHisto == currentHisto && !currentHisto->getChanged() )
      return;
    lastHisto = currentHisto;

    updateHistogramProperties( windowProperties, currentHisto, propertiesClientData );
  }
  else if( currentTimeline != nullptr )
  {
    lastHisto = nullptr;
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

  for ( ; menuIt != menuItems.end(); ++menuIt )
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
      wxMenuItem *newItem = new wxMenuItem( menuTraces, wxID_ANY, wxString::FromUTF8( (*it).c_str() ) );
      menuTraces->Append( newItem );
      Connect( newItem->GetId(),
               wxEVT_COMMAND_MENU_SELECTED,
               (wxObjectEventFunction)&paraverMain::OnPreviousTracesClick );
    }
    else
    {
      wxMenuItem *tmp = *menuIt;
      tmp->SetItemLabel( wxString::FromUTF8( (*it).c_str() ) );
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
        wxMenuItem *newItem = new wxMenuItem( menuSessions, wxID_ANY, wxString::FromUTF8( (*it).c_str() ) );
        menuSessions->Append( newItem );
        Connect( newItem->GetId(),
                 wxEVT_COMMAND_MENU_SELECTED,
                 (wxObjectEventFunction)&paraverMain::OnPreviousSessionsClick );
      }
      else
      {
        wxMenuItem *tmp = *menuIt;
        tmp->SetItemLabel( wxString::FromUTF8( (*it).c_str() ) );
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
      wxMenuItem *newItem = new wxMenuItem( menuCFGs, wxID_ANY, wxString::FromUTF8( (*it).c_str() ) );
      menuCFGs->Append( newItem );
      Connect( newItem->GetId(),
               wxEVT_COMMAND_MENU_SELECTED,
              (wxObjectEventFunction)&paraverMain::OnPreviousCFGsClick );
    }
    else
    {
      wxMenuItem *tmp = *menuIt;
      tmp->SetItemLabel( wxString::FromUTF8( (*it).c_str() ) );
      ++menuIt;
    }
  }
}

void progressFunction( ProgressController *progress, void *callerWindow )
{
  int p;
  if ( progress->getCurrentProgress() > progress->getEndLimit() )
    p = MAX_PROGRESS_BAR_VALUE;
  else
    p = (int)floor( ( progress->getCurrentProgress() * MAX_PROGRESS_BAR_VALUE ) / progress->getEndLimit() );

  wxString newMessage;
  if( progress->getMessageChanged() )
  {
    newMessage = wxString::FromUTF8( progress->getMessage().c_str() );
    progress->clearMessageChanged();
  }

  if( !paraverMain::dialogProgress->Update( p, newMessage ) )
    progress->setStop( true );
//  app->Yield();
}


bool isWindowRelatedToOtherTraces( Timeline *whichWindow, Trace *whichTrace, Timeline *parentWindow, Timeline *childWindow )
{
  bool isRelated = false;

  if ( whichWindow->getTrace() != whichTrace && !whichWindow->getTrace()->getUnload() )
  {
    return true;
  }
  
  if( whichWindow->isDerivedWindow() )
  {
    // Recursive
    bool parent1 = false;
    bool parent2 = false;
    if( parentWindow != whichWindow->getParent( 0 ) )
      parent1 = isWindowRelatedToOtherTraces( whichWindow->getParent( 0 ), whichTrace, nullptr, whichWindow );
    if( parentWindow != whichWindow->getParent( 1 ) )
      parent2 = isWindowRelatedToOtherTraces( whichWindow->getParent( 1 ), whichTrace, nullptr, whichWindow );
    isRelated = parent1 || parent2;
  }

  if( whichWindow->getChild() != nullptr && whichWindow->getChild() != childWindow )
  {
    isRelated = isRelated || isWindowRelatedToOtherTraces( whichWindow->getChild(), whichTrace, whichWindow, nullptr );
  }
    
  return isRelated;
}


bool allWindowsRelatedToOtherTraces( vector<Timeline *> windows )
{
  if ( windows.empty() )
    return false;
    
  for ( vector< Timeline * >::iterator it = windows.begin() ; it != windows.end() ; ++it )
  {
    if( !(*it)->isDerivedWindow() && (*it)->getChild() == nullptr )
      return false;

    if( !isWindowRelatedToOtherTraces( *it, (*it)->getTrace(), nullptr, nullptr ) )
      return false;
  }

  return true;
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

  if ( cutFilterFinished )
  {
    cutFilterFinished = false;
    canServeSignal = true;
  }


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
        vector<Timeline *> windows;
        vector<Histogram *> histograms;

        LoadedWindows::getInstance()->getAll( *it, windows );
        LoadedWindows::getInstance()->getAll( *it, histograms );

        if( windows.empty() && histograms.empty() )
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
        {
          if ( allWindowsRelatedToOtherTraces( windows ) )
          {
            (*it)->setUnload( false );
            wxString traceName = wxString::FromUTF8( (*it)->getTraceNameNumbered().c_str() );
            wxMessageBox( _( "Cannot delete trace " ) + traceName + _( ", which is being used by some windows in other traces." ),
                          _( "Warning" ),
                          wxOK | wxICON_EXCLAMATION );
          }
          ++iTrace;
        }
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

#ifndef _WIN32
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
    if( item->getTimeline() != nullptr )
    {
      currentWindow = item->getTimeline();
      currentTimeline = item->getTimeline()->GetMyWindow();
      beginDragWindow = currentTimeline;
      currentHisto = nullptr;
    }
    else if( item->getHistogram() != nullptr )
    {
      currentWindow = item->getHistogram();
      currentHisto = item->getHistogram()->GetHistogram();
      currentTimeline = nullptr;
    }
  }

  setActiveWorkspacesText();
  refreshMenuHints();
}


void paraverMain::SaveConfigurationFile( wxWindow *parent,
                                         SaveOptions options,
                                         vector< Timeline * > timelines,
                                         vector< Histogram * > histograms,
                                         const vector<CFGS4DLinkedPropertiesManager>& linkedProperties )
{
  if ( !CFGLoadedBefore )
    CFGPath =  wxString::FromUTF8( paraverConfig->getGlobalCFGsPath().c_str() );

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

    CFGLoader::saveCFG( std::string( CFGPath.mb_str() ), options, timelines, histograms, linkedProperties );
    previousCFGs->add( std::string( CFGPath.mb_str() ) );
  }
}



/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUSAVECFG
 */

void paraverMain::OnMenusavecfgClick( wxCommandEvent& event )
{
  SaveOptions options;
  vector<Timeline *> timelines;
  vector<Histogram *> histograms;
  SaveConfigurationDialog saveDialog( this );

  saveDialog.SetOptions( options );
  LoadedWindows::getInstance()->getAll( timelines );
  LoadedWindows::getInstance()->getAll( histograms );
  saveDialog.SetTimelines( timelines );
  saveDialog.SetHistograms( histograms );

  // Find trace to put in the selector
  Trace *selectedTrace = nullptr;
  vector<Timeline *> auxWindows;
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
    const CFGS4DLinkedPropertiesManager& linkedProperties = saveDialog.getLinkedPropertiesManager();
    vector<CFGS4DLinkedPropertiesManager> propertiesList;
    propertiesList.push_back( linkedProperties );

    SaveConfigurationFile( (wxWindow *)this, options, timelines, histograms, propertiesList );

    // Disable CFG4D once it is saved
    for( vector< Timeline * >::iterator it = timelines.begin(); it != timelines.end(); ++it )
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


Timeline *paraverMain::createBaseWindow( wxString whichName )
{
  // Create new window
  Timeline *newWindow = Timeline::create( localKernel, getCurrentTrace() );
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


void paraverMain::insertInTree( Timeline *whichWindow )
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

  if ( currentTimeline != nullptr )
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
  if( endDragWindow == nullptr )
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
    Timeline *newWindow = Timeline::create( localKernel, beginDragWindow, endDragWindow );
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
    else if( beginDragWindow->isAlternativeGradientColorSet() )
      newWindow->setAlternativeGradientColorMode();
    else if( beginDragWindow->isNotNullGradientColorSet() )
      newWindow->setNotNullGradientColorMode();
    else if( beginDragWindow->isFunctionLineColorSet() )
      newWindow->setFunctionLineColorMode();

    newWindow->setDrawModeObject( beginDragWindow->getDrawModeObject() );
    newWindow->setDrawModeTime( beginDragWindow->getDrawModeTime() );
    newWindow->getGradientColor().setGradientFunction( beginDragWindow->getGradientColor().getGradientFunction() );
    newWindow->setLevel( beginDragWindow->getLevel() );
    vector<bool> tmpSel;
    for( TTraceLevel level = TTraceLevel::APPLICATION; level <= TTraceLevel::THREAD; ++level )
    {
      tmpSel.clear();
      beginDragWindow->getSelectedRows( level, tmpSel );
      newWindow->setSelectedRows( level, tmpSel );
    }
    if( beginDragWindow->getTrace()->existResourceInfo() )
    {
      for( TTraceLevel level = TTraceLevel::NODE; level <= TTraceLevel::CPU; ++level )
      {
        tmpSel.clear();
        beginDragWindow->getSelectedRows( level, tmpSel );
        newWindow->setSelectedRows( level, tmpSel );
      }
    }

    // Semantic
    vector< string > auxCompose = derivedDialog.GetTopCompose1();
    newWindow->setLevelFunction( TOPCOMPOSE1, auxCompose[0] );
    auxCompose = derivedDialog.GetTopCompose2();
    newWindow->setLevelFunction( TOPCOMPOSE2, auxCompose[0] );

    newWindow->setFactor( 0, derivedDialog.GetFactorTimeline1() );
    newWindow->setFactor( 1, derivedDialog.GetFactorTimeline2() );

    newWindow->setShift( 0, derivedDialog.GetShiftTimeline1() );
    newWindow->setShift( 1, derivedDialog.GetShiftTimeline2() );

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
  //HistogramDialog histogramDialog( nullptr );

  vector<TWindowID> timelines;
  LoadedWindows::getInstance()->getAll( timelines );

  histogramDialog.SetControlTimelines( timelines );

  vector< pair< TRecordTime, TRecordTime > > ranges;
  // Timeline Times
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
    gHistogram* tmpHisto = new gHistogram( this, wxID_ANY, wxString::FromUTF8( composedName.c_str() ) );
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
    tmpHisto->GetHistogram()->setRecalc( true );

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
    vector<Timeline *> timelines;
    LoadedWindows::getInstance()->getAll( loadedTraces[ currentTrace ], timelines );

    event.Enable( ( timelines.size() > 0 ) && ( currentTimeline != nullptr ) );
  }
  else
    event.Enable( false );

  if ( currentHisto != nullptr )
    event.Enable( false );
}


void paraverMain::OnTreeBeginDrag( wxTreeEvent& event )
{
  wxTreeCtrl *tmpTree = static_cast<wxTreeCtrl *>( event.GetEventObject() );
  TreeBrowserItemData *itemData = static_cast<TreeBrowserItemData *>( tmpTree->GetItemData( event.GetItem() ) );

  beginDragWindow = nullptr;

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

    endDragWindow = nullptr;

    if ( gTimeline *timeline = itemData->getTimeline())
    {
      endDragWindow = timeline->GetMyWindow();
      if( beginDragWindow->getTrace()->isSameObjectStruct( endDragWindow->getTrace(), endDragWindow->isLevelProcessModel() ) &&
          Timeline::compatibleLevels( beginDragWindow, endDragWindow ) )
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
    vector<Timeline *> timelines;
    if( currentTrace == -1 )
      LoadedWindows::getInstance()->getAll( timelines );
    else
      LoadedWindows::getInstance()->getAll( loadedTraces[ currentTrace ], timelines );
    tbarMain->EnableTool( ID_NEW_HISTOGRAM, ( timelines.size() > 0 ) && ( currentTimeline != nullptr ) );
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
  preferences.SetDisableTimelineZoomMouseWheel( paraverConfig->getDisableTimelineZoomMouseWheel() );
 
  // TIMELINE

  preferences.SetTimelineNameFormatPrefix( paraverConfig->getTimelineDefaultName() );
  preferences.SetTimelineNameFormatFull( paraverConfig->getTimelineNameFormat() );
  //preferences.GetTimelineDefaultCFG( paraverConfig->setTimelineDefaultCFG() );
  preferences.SetTimelineEventLines( paraverConfig->getTimelineViewEventsLines() );
  preferences.SetTimelineCommunicationLines( paraverConfig->getTimelineViewCommunicationsLines() );
  preferences.SetTimelineSemanticScaleMinAtZero( paraverConfig->getTimelineSemanticScaleMinAtZero() );
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
  preferences.SetTimelineKeepSyncGroupClone( paraverConfig->getTimelineKeepSyncGroupClone() );

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
  preferences.SetHistogramGradientFunction( static_cast< PRV_UINT32 > ( paraverConfig->getHistogramGradientFunction() ) );
  preferences.SetHistogramDrawmodeSemantic( ( PRV_UINT32 ) paraverConfig->getHistogramDrawmodeSemantic() );
  preferences.SetHistogramDrawmodeObjects( ( PRV_UINT32 ) paraverConfig->getHistogramDrawmodeObjects() );
  preferences.SetHistogramScientificNotation( paraverConfig->getHistogramScientificNotation() );
  preferences.SetHistogramThousandSeparator( paraverConfig->getHistogramThousandSep() );
  preferences.SetHistogramShowUnits( paraverConfig->getHistogramShowUnits() );
  preferences.SetHistogramPrecision( paraverConfig->getHistogramPrecision() );
  preferences.SetHistogramAutofitControlScale( paraverConfig->getHistogramAutofitControlScale() );
  preferences.SetHistogramAutofitControlScaleZero( paraverConfig->getHistogramAutofitControlScaleZero() );
  preferences.SetHistogramAutofit3DScale( paraverConfig->getHistogramAutofitThirdDimensionScale() );
  preferences.SetHistogramAutofitDataGradient( paraverConfig->getHistogramAutofitDataGradient() );
  preferences.SetHistogramNumColumns( paraverConfig->getHistogramNumColumns() );
//  preferences.SetHistogramMaxNumColumns( 400 ); // TO IMPLEMENT
//  preferences.SetHistogramMaxPrecision( 20 ); // TO IMPLEMENT
  preferences.SetHistogramSaveImageFormat( ( PRV_UINT32 )paraverConfig->getHistogramSaveImageFormat() );
  preferences.SetHistogramSaveTextFormat( ( PRV_UINT32 )paraverConfig->getHistogramSaveTextFormat() );
  preferences.SetHistogramSkipCreateDialog( paraverConfig->getHistogramSkipCreateDialog() );
  preferences.SetHistogramKeepSyncGroupClone( paraverConfig->getHistogramKeepSyncGroupClone() );

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

  preferences.SetWorkspaceDiscardedSubmenu( paraverConfig->getWorkspacesHintsDiscardedSubmenu() );

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
    paraverConfig->setDisableTimelineZoomMouseWheel( preferences.GetDisableTimelineZoomMouseWheel() );


    // TIMELINE
    paraverConfig->setTimelineDefaultName( preferences.GetTimelineNameFormatPrefix() );
    paraverConfig->setTimelineNameFormat( preferences.GetTimelineNameFormatFull() );
    //paraverConfig->setTimelineDefaultCFG( preferences.GetTimelineDefaultCFG() );
    paraverConfig->setTimelinePrecision( preferences.GetTimelineWWPrecision() );
    paraverConfig->setTimelineViewEventsLines( preferences.GetTimelineEventLines() );
    paraverConfig->setTimelineViewCommunicationsLines( preferences.GetTimelineCommunicationLines() );
    paraverConfig->setTimelineSemanticScaleMinAtZero( preferences.GetTimelineSemanticScaleMinAtZero() );
    paraverConfig->setTimelineColor( (TColorFunction)preferences.GetTimelineColor() );
    paraverConfig->setTimelineGradientFunction( (TGradientFunction)preferences.GetTimelineGradientFunction() );
    paraverConfig->setTimelineDrawmodeTime( (DrawModeMethod)preferences.GetTimelineDrawmodeTime() );
    paraverConfig->setTimelineDrawmodeObjects( (DrawModeMethod)preferences.GetTimelineDrawmodeObjects() );
    paraverConfig->setTimelinePixelSize( preferences.GetTimelinePixelSize() );
    paraverConfig->setTimelineLabels( (TObjectLabels)preferences.GetTimelineObjectLabels() );
    paraverConfig->setTimelineObjectAxisSize( (TObjectAxisSize)preferences.GetTimelineObjectAxis() );
    paraverConfig->setTimelineWhatWhereSemantic( preferences.GetTimelineWWSemantic() );
    paraverConfig->setTimelineWhatWhereEvents( preferences.GetTimelineWWEvents() );
    paraverConfig->setTimelineWhatWhereCommunications( preferences.GetTimelineWWCommunications() );
    paraverConfig->setTimelineWhatWherePreviousNext( preferences.GetTimelineWWPreviousNext() );
    paraverConfig->setTimelineWhatWhereText( preferences.GetTimelineWWText() );
    paraverConfig->setTimelineWhatWhereEventPixels( preferences.GetTimelineWWEventPixels() );
    paraverConfig->setTimelineSaveImageFormat( (TImageFormat)preferences.GetTimelineSaveImageFormat() );
    paraverConfig->setTimelineSaveTextFormat( (TTextFormat)preferences.GetTimelineSaveTextFormat() );
    paraverConfig->setTimelineKeepSyncGroupClone( preferences.GetTimelineKeepSyncGroupClone() );

    // HISTOGRAM
    //paraverConfig->setHistogramDefaultName( preferences.GetHistogramNameFormatPrefix() );
    //paraverConfig->setHistogramNameFormat( preferences.GetHistogramNameFormatFull() );
    paraverConfig->setHistogramViewZoom( preferences.GetHistogramZoom() );
    paraverConfig->setHistogramViewHorizontal( preferences.GetHistogramHorizontal() );
    paraverConfig->setHistogramViewEmptyColumns( !preferences.GetHistogramHideEmpty() );
    paraverConfig->setHistogramViewGradientColors( preferences.GetHistogramShowGradient() );
    paraverConfig->setHistogramViewFirstRowColored( preferences.GetHistogramLabelsColor() );
    paraverConfig->setHistogramGradientFunction( (TGradientFunction)preferences.GetHistogramGradientFunction() );
    paraverConfig->setHistogramDrawmodeSemantic( ( DrawModeMethod ) preferences.GetHistogramDrawmodeSemantic() );
    paraverConfig->setHistogramDrawmodeObjects( ( DrawModeMethod ) preferences.GetHistogramDrawmodeObjects() );
    paraverConfig->setHistogramScientificNotation( preferences.GetHistogramScientificNotation() );
    paraverConfig->setHistogramThousandSep( preferences.GetHistogramThousandSeparator() );
    paraverConfig->setHistogramShowUnits( preferences.GetHistogramShowUnits() );
    paraverConfig->setHistogramPrecision( preferences.GetHistogramPrecision() );
    paraverConfig->setHistogramAutofitControlScale( preferences.GetHistogramAutofitControlScale() );
    paraverConfig->setHistogramAutofitControlScaleZero( preferences.GetHistogramAutofitControlScaleZero() );
    paraverConfig->setHistogramAutofitThirdDimensionScale( preferences.GetHistogramAutofit3DScale() );
    paraverConfig->setHistogramAutofitDataGradient( preferences.GetHistogramAutofitDataGradient() );
    paraverConfig->setHistogramNumColumns( preferences.GetHistogramNumColumns() );
    paraverConfig->setHistogramSaveImageFormat( ( TImageFormat ) preferences.GetHistogramSaveImageFormat() );
    paraverConfig->setHistogramSaveTextFormat( ( TTextFormat ) preferences.GetHistogramSaveTextFormat() );
    paraverConfig->setHistogramSkipCreateDialog( preferences.GetHistogramSkipCreateDialog() );
    paraverConfig->setHistogramKeepSyncGroupClone( preferences.GetHistogramKeepSyncGroupClone() );

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

    vector<Timeline *> tmpWins;
    LoadedWindows::getInstance()->getAll( tmpWins );
    for( vector<Timeline *>::iterator it = tmpWins.begin(); it != tmpWins.end(); ++it )
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

    paraverConfig->setWorkspacesHintsDiscardedSubmenu( preferences.GetWorkspaceDiscardedSubmenu() );

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


bool paraverMain::getUsedBySomeHistogram( Timeline *whichWindow, bool deleteAllTraceWindows, wxArrayInt tracesToDelete )
{
  if ( whichWindow->getUsedByHistogram() )
  {
    if( !deleteAllTraceWindows )
      return true;

    const std::set<Histogram *> histogramsUsed = whichWindow->getHistograms();
    for( std::set<Histogram *>::const_iterator it = histogramsUsed.begin() ; it != histogramsUsed.end(); ++it )
    {
      bool traceToDeleteFound = false;
      for ( size_t i = 0; i < tracesToDelete.GetCount(); ++i )
      {
        if ( loadedTraces[ tracesToDelete.Item( i ) ] == (*it)->getTrace() )
        {
          traceToDeleteFound = true;
          break;
        }
      }
      if ( !traceToDeleteFound && ( *it )->getTrace() != whichWindow->getTrace() )
        return true;
    }
  }
  else if ( whichWindow->isDerivedWindow() )
  {
    if ( getUsedBySomeHistogram( whichWindow->getParent( 0 ), deleteAllTraceWindows, tracesToDelete ) )
      return true;

    return getUsedBySomeHistogram( whichWindow->getParent( 1 ), deleteAllTraceWindows, tracesToDelete );
  }

  return false;
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOLDELETE
 */

void paraverMain::OnTooldeleteClick( wxCommandEvent& event )
{
  if( currentHisto != nullptr )
    currentHisto->setDestroy( true );

  if( currentTimeline != nullptr )
  {
    wxArrayInt dummyArray;
    if( !getUsedBySomeHistogram( currentTimeline, false, dummyArray ) )
    {
      if( currentTimeline->getChild() != nullptr )
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

  if( currentTimeline != nullptr )
  {
    bool dummyFound;
    gTimeline *tmpTimeline = getGTimelineFromWindow( getAllTracesTree()->GetRootItem(), currentTimeline, dummyFound );
    tmpEnableButtonDestroy = tmpTimeline->getEnableDestroyButton();
  }
  else if ( currentHisto != nullptr )
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
    choices.Add( wxString::FromUTF8( (*it)->getTraceNameNumbered().c_str() ) );
  wxMultiChoiceDialog dialog( this, _("Select the traces to unload:"), _("Unload Traces"), choices );

  raiseCurrentWindow = false;

  dialog.ShowModal();
  if ( dialog.GetReturnCode() == wxID_OK )
  {
    wxArrayInt sel = dialog.GetSelections();
    for( size_t i = 0; i < sel.GetCount(); ++i )
    {
      vector<Timeline *> windows;
      LoadedWindows::getInstance()->getAll( loadedTraces[ sel.Item( i ) ], windows );

      bool isThereHistogramLinkedToWindow = false;
      for( vector<Timeline *>::iterator it = windows.begin(); !isThereHistogramLinkedToWindow && it != windows.end(); ++it )
      {
        isThereHistogramLinkedToWindow = getUsedBySomeHistogram( (*it), true, sel );
        if( isThereHistogramLinkedToWindow )
        {
          wxString traceName = wxString::FromUTF8( loadedTraces[ sel.Item( i ) ]->getTraceNameNumbered().c_str() );
          wxMessageBox( _( "Cannot delete trace " ) + traceName + _( ", which is being used in an histogram." ),
                        _( "Warning" ),
                        wxOK | wxICON_EXCLAMATION ); 
        }
      }


      if ( !isThereHistogramLinkedToWindow )
      {
        UnloadTrace( sel.Item( i ) );
      }

    }
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
  vector<Timeline *> windows;
  vector<Histogram *> histograms;

  LoadedWindows::getInstance()->getAll( loadedTraces[ whichTrace ], windows );
  LoadedWindows::getInstance()->getAll( loadedTraces[ whichTrace ], histograms );

  for( vector<Timeline *>::iterator it = windows.begin(); it != windows.end(); ++it )
  {
    (*it)->setShowWindow( false );
    if( (*it)->getChild() == nullptr )
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
#ifdef _WIN32
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
        #ifdef _WIN32
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
#ifdef _WIN32
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

          if ( connection == nullptr )
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
    strStatus << static_cast<int>( sessionInfo.status );
  #ifdef _WIN32

    wxString file( wxString( std::string( ParaverConfig::getInstance()->getGlobalSessionPath() +
                                          "\\AutosavedSessions" +
                                          "\\ps" +
                                          strPid.str() +
                                          "_" +
                                          sessionInfo.sessionDate +
                                          "_" +
                                          strStatus.str() +
                                          ".session" ).c_str(), wxConvUTF8 ) );

    wxString folder( wxString( std::string( ParaverConfig::getInstance()->getGlobalSessionPath() +
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
    
    sessionInfo.status = StatusID::CLOSED;

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
    strStatus << static_cast<int>( sessionInfo.status );
  #ifdef _WIN32
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

  if( instChecker != nullptr ) delete instChecker;
}




#if !defined _MSC_VER && !defined __MINGW32__

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
  if( paraverMain::dialogProgress != nullptr )
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
  if( currentTimeline == nullptr && currentHisto == nullptr )
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
    if( currentTimeline != nullptr )
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
      tmpTimeline->GetMyWindow()->setRedraw( true );

      // Save image if needed
      if( !currentSignal.imageFileName.empty() )
        tmpTimeline->saveImage( wxString::FromUTF8( currentSignal.imageFileName.c_str() ) );
    }
    else // Histogram
    {
      // Zoom
      currentHisto->setWindowBeginTime( currentSignal.beginTime );
      currentHisto->setWindowEndTime( currentSignal.endTime );

      // Redraw
      currentHisto->setChanged( true );
      gHistogram *tmpHistogram = getGHistogramFromWindow( getAllTracesTree()->GetRootItem(), currentHisto );
      tmpHistogram->GetHistogram()->setRecalc( true );

      // Save image if needed
      if( !currentSignal.imageFileName.empty() )
        tmpHistogram->saveImage( wxString::FromUTF8( currentSignal.imageFileName.c_str() ) );
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


#ifdef _WIN32
void paraverMain::OnKeyCopy()
{
  wxCommandEvent dummyEvent;

  wxTreeCtrl *tree = (wxTreeCtrl *) choiceWindowBrowser->GetCurrentPage();
  if( !tree->GetSelection().IsOk() )
    return;
  TreeBrowserItemData *item = (TreeBrowserItemData *) tree->GetItemData( tree->GetSelection() );
  if( item->getTimeline() != nullptr )
    item->getTimeline()->OnPopUpCopy( dummyEvent );
  else if( item->getHistogram() != nullptr )
    item->getHistogram()->OnPopUpCopy( dummyEvent );
}


void paraverMain::OnKeyPaste()
{
  wxCommandEvent dummyEvent;

  wxTreeCtrl *tree = (wxTreeCtrl *) choiceWindowBrowser->GetCurrentPage();
  if( !tree->GetSelection().IsOk() )
    return;
  TreeBrowserItemData *item = (TreeBrowserItemData *) tree->GetItemData( tree->GetSelection() );
  if( item->getTimeline() != nullptr )
    item->getTimeline()->OnPopUpPasteDefaultSpecial( dummyEvent );
  else if( item->getHistogram() != nullptr )
    item->getHistogram()->OnPopUpPasteDefaultSpecial( dummyEvent );
}
#endif

void paraverMain::OnFindDialog()
{
  wxTreeCtrl *tree = (wxTreeCtrl *) choiceWindowBrowser->GetCurrentPage();
  if( !tree->GetSelection().IsOk() )
    return;
  TreeBrowserItemData *item = (TreeBrowserItemData *) tree->GetItemData( tree->GetSelection() );
  if( item->getTimeline() != nullptr )
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
  if ( currentTrace == -1 )
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

  ProgressController *progress = ProgressController::create( localKernel );
  progress->setHandler( progressFunction, this );

  if( paraverMain::dialogProgress == nullptr )
    paraverMain::dialogProgress = new wxProgressDialog( wxT("Processing trace..."),
                                                        wxT(""),
                                                        MAX_PROGRESS_BAR_VALUE,
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

    paraverMain::dialogProgress->Pulse( wxString::FromUTF8( tmpNameOut.c_str() ) );
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
  paraverMain::dialogProgress = nullptr;
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
    wxString auxName = wxString::FromUTF8( xmlFile.c_str() );
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


void paraverMain::OnOKCutFilterDialog( CutFilterDialog *cutFilterDialog )
{
  vector< string > filterToolOrder = cutFilterDialog->GetFilterToolOrder();
  string srcTrace = cutFilterDialog->GetNameSourceTrace();
  string dstTrace = cutFilterDialog->GetNameDestinyTrace();

  DoLoadFilteredTrace( srcTrace, dstTrace, cutFilterDialog->GetTraceOptions(), filterToolOrder );

  if ( cutFilterDialog->GetLoadResultingTrace() )
  {
    DoLoadTrace( dstTrace );
  }

  XMLLoadedBefore = cutFilterDialog->GetLoadedXMLPath( XMLPath );

  if ( cutFilterDialog->GetRunAppWithResultingTrace() )
  {
    wxString auxTrace =  wxString::FromUTF8( dstTrace.c_str() );
    ShowRunCommand( auxTrace );
  }

  cutFilterFinished = true;
}


void paraverMain::ShowCutTraceWindow( const string& filename,
                                      bool loadTrace,
                                      const string& xmlFile )
{
  cutFilterFinished = false;


  CutFilterDialog *cutFilterDialog = new CutFilterDialog( this );

  MainSettingsCutFilterDialog( cutFilterDialog, filename, loadTrace );

  TraceOptions *traceOptions = TraceOptions::create( localKernel );
  vector< string > filterToolOrder;
  OptionsSettingCutFilterDialog( cutFilterDialog, traceOptions, xmlFile, filterToolOrder );

  cutFilterDialog->Show();
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
    iconizeWindows( currentTree, root, event.IsIconized() );
  }
}


/*!
 * wxEVT_SIZE event handler for ID_PARAVERMAIN
 */

void paraverMain::OnSize( wxSizeEvent& event )
{
  paraverConfig->setMainWindowWidth( event.GetSize().GetWidth() );
  paraverConfig->setMainWindowHeight( event.GetSize().GetHeight() );

  paraverConfig->writeParaverConfigFile( false );
  event.Skip();
}


void paraverMain::OnSessionTimer( wxTimerEvent& event )
{
  string file;
  if ( ParaverConfig::getInstance()->getGlobalSingleInstance() ) 
  {
    #ifdef _WIN32
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
    strStatus << static_cast<int>( sessionInfo.status );

    #ifdef _WIN32
    file = ParaverConfig::getInstance()->getGlobalSessionPath() + "\\AutosavedSessions" + "\\ps" + strPid.str() + "_" + sessionInfo.sessionDate + "_" + strStatus.str() + ".session";
    #else
    file = ParaverConfig::getInstance()->getGlobalSessionPath() + "/AutosavedSessions" +  "/ps" + strPid.str() + "_" + sessionInfo.sessionDate + "_" + strStatus.str() + ".session";
    #endif
  }
  SessionSaver::SaveSession( wxString::FromUTF8( file.c_str() ), GetLoadedTraces() );
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
#ifdef _WIN32
  developers.Add( _("\nPedro Antonio Gonzalez Navarro (pedro.gonzalez@bsc.es)") );
  developers.Add( _("\nMiguel Moreno Gomez (miguel.moreno@bsc.es)") );
#else
  developers.Add( _("Pedro Antonio Gonzalez Navarro (pedro.gonzalez@bsc.es)") );
  developers.Add( _("Miguel Moreno Gomez (miguel.moreno@bsc.es)") );
#endif
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

  if ( tutorialsWindow == nullptr )
  {
    tutorialsRoot = wxString( GetParaverConfig()->getGlobalTutorialsPath().c_str(), wxConvUTF8 );
    tutorialsWindow = HelpContents::createObject( TContents::TUTORIAL, this, tutorialsRoot, 
                                                  true, wxID_ANY, _("Tutorials") );
  }

  tutorialsWindow->Show( true );
}


void paraverMain::ShowRunCommand( wxString traceFile )
{
  if( runApplication != nullptr )
  {
    runApplication->Raise();
    return;
  }

  runApplication = new RunScript( this, traceFile ); 

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
    CFURLRef fileURL = CFBundleCopyResourceURL( mainBundle, CFSTR("aaa"), CFSTR("txt"), nullptr );
    CFStringRef filePath = CFURLCopyFileSystemPath( fileURL, kCFURLPOSIXPathStyle );
    CFStringEncoding encodingMethod = CFStringGetSystemEncoding();
    const char *path = CFStringGetCStringPtr( filePath, encodingMethod );
    cout<<"File location: "<<path<<endl;
#endif*/

  //wxString paraverHome;

#ifdef _WIN32
  std::wstring baseDir;

  TCHAR myPath[ MAX_LEN_PATH ];
  HMODULE hModule = GetModuleHandle( nullptr );
  if ( hModule != nullptr )
  {
    GetModuleFileName( nullptr, myPath, ( sizeof( myPath ) ));
    PathRemoveFileSpec( myPath );
    wstring tmpParaverPath( myPath );
    baseDir = tmpParaverPath.substr( 0, tmpParaverPath.size() - 4 );
  }
  paraverHome = wxString( baseDir.c_str(), wxConvUTF8 );

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

  if ( helpContents == nullptr )
  {
    //helpContents = new HelpContents( nullptr, helpContentsAbsolutePath, lookForContents, wxID_ANY, _("Help Contents") );
    helpContents = HelpContents::createObject( TContents::HELP, nullptr, helpContentsAbsolutePath, 
                                               lookForContents, wxID_ANY, _("Help Contents") );
  }

  // If helpFile has no "html" at the end, use Help Content's Index as hCAP (which works)
  if ( helpFile.SubString( helpFile.size() - 4, helpFile.size() - 1) != wxT( "html" ) )
  {
#ifdef _WIN32
    helpContentsAbsolutePath = wxT( "file:///" ) + 
#else
    helpContentsAbsolutePath = wxT( "file://" ) + 
#endif
          wxString::FromUTF8( paraverConfig->getParaverConfigDir().c_str() ) +
          wxString( wxFileName::GetPathSeparator() ) +
          wxString( wxT( "help_contents" ) ) + wxT( "_index.html" );
  }
  else // Otherwise use previous hCAP
  {
#ifdef _WIN32
    helpContentsAbsolutePath = wxT( "file:///" ) + paraverHome + helpContentsBaseRelativePath + helpFile + hRef;
#else
    helpContentsAbsolutePath = wxT( "file://" ) + paraverHome + helpContentsBaseRelativePath + helpFile + hRef;
#endif
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
//  wxFileName filename( wxString::FromUTF8(  hint.first.c_str() ) );
//  return filename.GetName() + _( " - " ) + wxString::FromUTF8(  hint.second.c_str() );
  return wxString::FromUTF8( hint.second.c_str() );
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
  wxString auxTraceName = wxString::FromUTF8( traceName.c_str() );
  auxTraceName.Remove( auxTraceName.Find( wxT( ".prv" ) ) );

  return ( wxString::FromUTF8( windowName.c_str() ) + wxString( wxT( '@' ) ) + auxTraceName );
}


void paraverMain::OnHintClick( wxCommandEvent& event )
{
  int hintId = event.GetId();
  wxMenuItem *tmpMenuItem = menuHints->FindItem( hintId );

  DoLoadCFG( ( (MenuHintFile *) tmpMenuItem->GetRefData() )->fileName );
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
  vector< string > tmpWorkspaces = workspacesManager->getWorkspaces( TWorkspaceSet::ALL );

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
    tmpNames.Add( wxString::FromUTF8( it->c_str() ) );
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
        tmpActive += wxString::FromUTF8( it->c_str() );
      else
      {
        if( workspacesManager->existWorkspace( it->c_str(), TWorkspaceSet::DISTRIBUTED ) )
          tmpActive += wxString::FromUTF8( it->c_str() ) + wxT( "#2" );
        else
          tmpActive += wxString::FromUTF8( it->c_str() );
      }
      ++tmpCurrentWorkspace;
    }

    txtActiveWorkspaces->SetValue( tmpActive );
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHECKBOX_AUTO_REDRAW
 */

void paraverMain::OnCheckboxAutoRedrawUpdate( wxUpdateUIEvent& event )
{
  event.Enable( currentTimeline != nullptr || currentHisto != nullptr );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_FORCE_REDRAW
 */

void paraverMain::OnButtonForceRedrawUpdate( wxUpdateUIEvent& event )
{
  event.Enable( currentTimeline != nullptr || currentHisto != nullptr );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_FORCE_REDRAW
 */

void paraverMain::OnButtonForceRedrawClick( wxCommandEvent& event )
{
  if( currentTimeline != nullptr )
    currentTimeline->setForceRedraw( true );
  else if( currentHisto != nullptr )
    currentHisto->setForceRecalc( true );
}


bool paraverMain::getAutoRedraw() const
{
  return checkAutoRedraw->GetValue();
}


bool paraverMain::isCFG4DModeDisabled() const
{
  return ( ( currentTimeline != nullptr &&
              ( !currentTimeline->getCFG4DEnabled() || !currentTimeline->getCFG4DMode() ) )
           ||
           ( currentHisto != nullptr &&
              ( !currentHisto->getCFG4DEnabled() || !currentHisto->getCFG4DMode() ) ) );
}


#ifndef _WIN32
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
#endif // _WIN32



void paraverMain::LastSessionLoad( bool isSessionInitialized )
{
  #ifdef _WIN32
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

#ifdef _WIN32
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

      if ( connection == nullptr && wxDirExists( folderPath ) && wxFileExists( path ) )
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
  #ifdef _WIN32
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
  sessionInfo.status = StatusID::OPEN;

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
  wxArrayString newTxt;

#ifdef _WIN32
  wxMimeTypesManager mimeTypeMgr;
  wxFileType* tmpFT = mimeTypeMgr.GetFileTypeFromExtension( wxT( "txt" ) );
  wxString tmpCmd = tmpFT->GetOpenCommand( tmpCmd );

  wxString myCmd = tmpFT->GetOpenCommand( tmpCmd );
  if ( myCmd.size() > 2 && myCmd.substr( myCmd.size() - 2, myCmd.size() - 1 ) == "\"\"" )
    myCmd = myCmd.substr( 0, myCmd.size() - 3 );

  newTxt.Add( myCmd );
#else
  wxArrayString externalTextEditors = paraverMain::FromVectorStringToWxArray( paraverConfig->getGlobalExternalTextEditors(), "txt" );
  
  wxArrayString dummyOutput;
  wxArrayString dummyErrors;

  for ( int i = 0 ; i < externalTextEditors.size(); ++i )
  {
    wxString command = externalTextEditors[ i ] + wxT( " --version 1>&- 2>&-'");
    int execRes = wxExecute( command, dummyOutput, dummyErrors, wxEXEC_SYNC );

    if ( execRes == 0 )
      newTxt.Add( externalTextEditors[ i ] );
  }
#endif

  if ( newTxt.size() == 0 )
  {
    // default type: "txt"
    wxString errMessage = _( "No text editors installed. Please verify the External Applications tab and add at least a text editor." );
    wxMessageBox( errMessage, _( "No programs found" ), wxOK );
  }

  wxArrayString newPDF;

#ifdef _WIN32
  tmpFT = mimeTypeMgr.GetFileTypeFromExtension( wxT( "pdf" ) );
  tmpCmd = wxT( "" );

  myCmd = tmpFT->GetOpenCommand( tmpCmd );
  if ( myCmd.size() > 2 && myCmd.substr( myCmd.size() - 2, myCmd.size() - 1 ) == "\"\"" )
    myCmd = myCmd.substr( 0, myCmd.size() - 3 );

  newPDF.Add( myCmd );
#else
  wxArrayString externalPDFReaders = paraverMain::FromVectorStringToWxArray( paraverConfig->getGlobalExternalPDFReaders(), "pdf" );
  
  for ( int i = 0 ; i < externalPDFReaders.size(); ++i )
  {
    wxString command = externalPDFReaders[ i ] + wxT( " --version 1>&- 2>&-'");
    int execRes = wxExecute( command, dummyOutput, dummyErrors, wxEXEC_SYNC );

    if ( execRes == 0 )
      newPDF.Add( externalPDFReaders[ i ] );
  }
#endif

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

  paraverConfig->writeParaverConfigFile();
}

Trace *paraverMain::getCurrentTrace() const
{
  Trace *tmpTraceToUse = nullptr;
  if( choiceWindowBrowser->GetSelection() == 0 )
  {
    if( currentTimeline != nullptr )
      tmpTraceToUse = currentTimeline->getTrace();
    else if( currentHisto != nullptr )
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


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TRACE_INFORMATION
 */

void paraverMain::OnTraceInformationClick( wxCommandEvent& event )
{
  TraceInformationDialog* TID = new TraceInformationDialog( this, getCurrentTrace() );
  TID->Show();
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TRACE_INFORMATION
 */

void paraverMain::OnTraceInformationUpdate( wxUpdateUIEvent& event )
{
  event.Enable( choiceWindowBrowser->GetSelection() > 0 );
}
