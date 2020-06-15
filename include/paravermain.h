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

#ifndef _PARAVERMAIN_H_
#define _PARAVERMAIN_H_


/*!
 * Includes
 */

#include <vector>
#include <queue>
#include <wx/progdlg.h>
#include <wx/treectrl.h>
#include <wx/cmdline.h>
#include <wx/imaglist.h>
#include <wx/snglinst.h>

////@begin includes
#include "wx/aui/framemanager.h"
#include "wx/frame.h"
#include "wx/toolbar.h"
#include "wx/choicebk.h"
#include "wx/toolbook.h"
#include "wx/dirctrl.h"
////@end includes
#include "trace.h"
#include "localkernel.h"
#include "paraverconfig.h"
#include "previousfiles.h"
#include "cfg.h" // SaveOptions
#include "workspacemanager.h"
#include "preferencesdialog.h"
#include "loadcfgdialog.h"
#include "sessionselectiondialog.h"
#include "connection.h"

// DATE TIME INCLUDES
//#include <boost/date_time/gregorian/gregorian.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"

#ifdef WIN32
#undef VERSION
#define VERSION "4.8.2"
#endif
/*!
 * Forward declarations
 */

////@begin forward declarations
class wxMenu;
class wxChoicebook;
class wxToolbook;
class wxGenericDirCtrl;
class wxPropertyGrid;
////@end forward declarations
class wxCheckBox;
class ProgressController;
class wxPropertyGridEvent;
class CutFilterDialog;
class TraceOptions;
class RunScript;

#ifdef WIN32
inline double rint( double nr )
{
  double f = floor( nr );
  double c = ceil( nr );
  return ( ( ( c - nr ) >= ( nr - f ) ) ? f : c );
}
#endif


/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_PARAVERMAIN 10000
#define ID_RECENTTRACES 10008
#define ID_UNLOADTRACE 10082
#define ID_MENULOADCFG 10006
#define ID_RECENTCFGS 10009
#define ID_MENUSAVECFG 10011
#define ID_MENULOADSESSION 10170
#define ID_RECENTSESSIONS 10292
#define ID_MENUSAVESESSION 10169
#define wxID_HELPCONTENTS 10005
#define wxID_TUTORIALS 10196
#define ID_TOOLBAR 10003
#define ID_NEW_WINDOW 10030
#define ID_NEW_DERIVED_WINDOW 10031
#define ID_NEW_HISTOGRAM 10060
#define ID_TOOLDELETE 10081
#define ID_TOOL_CUT_TRACE 10100
#define ID_TOOL_RUN_APPLICATION 10004
#define ID_CHOICEWINBROWSER 10002
#define ID_TOOLBOOKFILESANDPROPERTIES 10159
#define ID_DIRCTRLFILES 10160
#define ID_FOREIGN 10010
#define ID_PANEL_AUTOREDRAW 10001
#define wxID_STATIC_AUTO_REDRAW 10291
#define ID_CHECKBOX_AUTO_REDRAW 10007
#define ID_BUTTON_FORCE_REDRAW 10012
#define ID_PANEL_WORKSPACES 10266
#define ID_TEXT_ACTIVE_WORKSPACE 10267
#define ID_BUTTON_ACTIVE_WORKSPACES 10268
#define SYMBOL_PARAVERMAIN_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMINIMIZE_BOX|wxCLOSE_BOX|wxWANTS_CHARS
#define SYMBOL_PARAVERMAIN_TITLE _("Paraver")
#define SYMBOL_PARAVERMAIN_IDNAME ID_PARAVERMAIN
#define SYMBOL_PARAVERMAIN_SIZE wxSize(300, 600)
#define SYMBOL_PARAVERMAIN_POSITION wxPoint(0, -1)
////@end control identifiers
#define ID_TIMER_MAIN 40010



class gTimeline;
class gHistogram;
class HelpContents;

class TreeBrowserItemData: public wxTreeItemData
{
  public:
    TreeBrowserItemData( const wxString& whichDesc, gTimeline* whichWin ) :
      desc( whichDesc ), myTimeline( whichWin ), myHisto( NULL )
    {}
    TreeBrowserItemData( const wxString& whichDesc, gHistogram* whichHisto ) :
      desc( whichDesc ), myTimeline( NULL ), myHisto( whichHisto )
    {}
    
    const wxString& GetDesc() const
    {
      return desc;
    }
    
    gTimeline *getTimeline() const
    {
      return myTimeline;
    }
    
    gHistogram *getHistogram() const
    {
      return myHisto;
    }
    
  private:
    wxString desc;
    gTimeline* myTimeline;
    gHistogram* myHisto;
};


struct SignalItem
{
  bool goodFile;
  bool badTimes;
  bool signal1;
  std::string cfgFileName;
  TRecordTime beginTime;
  TRecordTime endTime;
  std::string traceFileName;
  std::string imageFileName;
  
  bool isSignal1() const
  {
    return signal1;
  }
  
  bool isSignal2() const
  {
    return !signal1;
  }
};


// 
struct PropertyClientData
{
  Window *ownerTimeline;
  Histogram *ownerHistogram;
  wxString propName;
};


struct SessionInfo
{    
    enum StatusID
    {
      OPEN   = 0,
      CLOSED = 1
    };
    
    unsigned int pid;
    StatusID status;
    std::string sessionDate;
};

/*!
 * paraverMain class declaration
 */

class paraverMain: public wxFrame
{    
  DECLARE_CLASS( paraverMain )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  paraverMain();
  paraverMain( wxWindow* parent, 
               wxWindowID id = SYMBOL_PARAVERMAIN_IDNAME, 
               const wxString& caption = SYMBOL_PARAVERMAIN_TITLE, 
               const wxPoint& pos = SYMBOL_PARAVERMAIN_POSITION, 
               const wxSize& size = SYMBOL_PARAVERMAIN_SIZE, 
               long style = SYMBOL_PARAVERMAIN_STYLE );

  bool Create( wxWindow* parent, 
               wxWindowID id = SYMBOL_PARAVERMAIN_IDNAME, 
               const wxString& caption = SYMBOL_PARAVERMAIN_TITLE, 
               const wxPoint& pos = SYMBOL_PARAVERMAIN_POSITION, 
               const wxSize& size = SYMBOL_PARAVERMAIN_SIZE, 
               long style = SYMBOL_PARAVERMAIN_STYLE );

  /// Destructor
  ~paraverMain();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();
  void initSessionInfo();
  void filterExternalApps();

////@begin paraverMain event handler declarations

  /// wxEVT_CLOSE_WINDOW event handler for ID_PARAVERMAIN
  void OnCloseWindow( wxCloseEvent& event );

  /// wxEVT_ICONIZE event handler for ID_PARAVERMAIN
  void OnIconize( wxIconizeEvent& event );

  /// wxEVT_SIZE event handler for ID_PARAVERMAIN
  void OnSize( wxSizeEvent& event );

  /// wxEVT_IDLE event handler for ID_PARAVERMAIN
  void OnIdle( wxIdleEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_OPEN
  void OnOpenClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_RECENTTRACES
  void OnRecenttracesUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_UNLOADTRACE
  void OnUnloadtraceClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_UNLOADTRACE
  void OnUnloadtraceUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENULOADCFG
  void OnMenuloadcfgClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_MENULOADCFG
  void OnMenuloadcfgUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUSAVECFG
  void OnMenusavecfgClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_MENUSAVECFG
  void OnMenusavecfgUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENULOADSESSION
  void OnMenuloadsessionClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_RECENTSESSIONS
  void OnRecentsessionsUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUSAVESESSION
  void OnMenusavesessionClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_PREFERENCES
  void OnPreferencesClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for wxID_PREFERENCES
  void OnPreferencesUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
  void OnExitClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_HELPCONTENTS
  void OnHelpcontentsClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_TUTORIALS
  void OnTutorialsClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_ABOUT
  void OnAboutClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEW_WINDOW
  void OnToolNewWindowClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_NEW_WINDOW
  void OnToolNewWindowUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEW_DERIVED_WINDOW
  void OnNewDerivedWindowClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_NEW_DERIVED_WINDOW
  void OnNewDerivedWindowUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEW_HISTOGRAM
  void OnNewHistogramClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_NEW_HISTOGRAM
  void OnNewHistogramUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOLDELETE
  void OnTooldeleteClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TOOLDELETE
  void OnTooldeleteUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_CUT_TRACE
  void OnToolCutTraceClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TOOL_CUT_TRACE
  void OnToolCutTraceUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_RUN_APPLICATION
  void OnToolRunApplicationClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED event handler for ID_CHOICEWINBROWSER
  void OnChoicewinbrowserPageChanged( wxChoicebookEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_CHOICEWINBROWSER
  void OnChoicewinbrowserUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_FOREIGN
  void OnForeignUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for wxID_STATIC_AUTO_REDRAW
  void OnStaticAutoRedrawUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_CHECKBOX_AUTO_REDRAW
  void OnCheckboxAutoRedrawUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_FORCE_REDRAW
  void OnButtonForceRedrawClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_BUTTON_FORCE_REDRAW
  void OnButtonForceRedrawUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ACTIVE_WORKSPACES
  void OnButtonActiveWorkspacesClick( wxCommandEvent& event );

////@end paraverMain event handler declarations

  /// wxEVT_TREE_SEL_CHANGED event handler for wxID_ANY
  void OnTreeSelChanged( wxTreeEvent& event );
  
  void OnMenuLoadAutoSavedSession( wxCommandEvent& event );
  void OnMenuLoadAutoSavedSessionSelect( wxCommandEvent& event );
  
  /// wxEVT_TREE_ITEM_ACTIVATED event handler for wxID_ANY
  void OnTreeItemActivated( wxTreeEvent& event );
  void OnTreeRightClick( wxTreeEvent& event );
  void OnTreeEndLabelRename( wxTreeEvent& event );
  void OnTreeKeyPress( wxKeyEvent& event );
  
  void renameTreeItem( );

  void OnTreeBeginDrag( wxTreeEvent& event );
  void OnTreeEndDrag( wxTreeEvent& event );
  
  void spreadSetChangedRecursive( Window *whichWindow );
  void spreadSetRedrawRecursive( Window *whichWindow );
  void spreadSetChanged( Window *whichWindow );
  void spreadSetRedraw( Window *whichWindow );
  /// wxEVT_PG_CHANGED event handler for ID_FOREIGN
  void OnPropertyGridChange( wxPropertyGridEvent& event );

  void OnPreviousTracesClick( wxCommandEvent& event );
  void OnPreviousCFGsClick( wxCommandEvent& event );
  void OnPreviousSessionsClick( wxCommandEvent& event );

  void OnActivate( wxActivateEvent& event );
////@begin paraverMain member function declarations

  /// Returns the AUI manager object
  wxAuiManager& GetAuiManager() { return m_auiManager; }

  bool GetCFGLoadedBefore() const { return CFGLoadedBefore ; }
  void SetCFGLoadedBefore(bool value) { CFGLoadedBefore = value ; }

  wxString GetCFGPath() const { return CFGPath ; }
  void SetCFGPath(wxString value) { CFGPath = value ; }

  bool GetXMLLoadedBefore() const { return XMLLoadedBefore ; }
  void SetXMLLoadedBefore(bool value) { XMLLoadedBefore = value ; }

  std::string GetXMLPath() const { return XMLPath ; }
  void SetXMLPath(std::string value) { XMLPath = value ; }

  std::set<wxWindow *> GetActiveWindows() const { return activeWindows ; }
  void SetActiveWindows(std::set<wxWindow *> value) { activeWindows = value ; }

  bool GetCanServeSignal() const { return canServeSignal ; }
  void SetCanServeSignal(bool value) { canServeSignal = value ; }

  Window * GetClusteringWindow() const { return clusteringWindow ; }
  void SetClusteringWindow(Window * value) { clusteringWindow = value ; }

  Histogram * GetCurrentHisto() const { return currentHisto ; }
  void SetCurrentHisto(Histogram * value) { currentHisto = value ; }

  Window * GetCurrentTimeline() const { return currentTimeline ; }
  void SetCurrentTimeline(Window * value) { currentTimeline = value ; }

  PRV_INT16 GetCurrentTrace() const { return currentTrace ; }
  void SetCurrentTrace(PRV_INT16 value) { currentTrace = value ; }

  wxWindow * GetCurrentWindow() const { return currentWindow ; }
  void SetCurrentWindow(wxWindow * value) { currentWindow = value ; }

  std::map<Trace *, size_t> GetFirstUserWorkspace() const { return firstUserWorkspace ; }
  void SetFirstUserWorkspace(std::map<Trace *, size_t> value) { firstUserWorkspace = value ; }

  HelpContents * GetHelpContents() const { return helpContents ; }
  void SetHelpContents(HelpContents * value) { helpContents = value ; }

  wxImageList* GetImageList() const { return imageList ; }
  void SetImageList(wxImageList* value) { imageList = value ; }

  Histogram * GetLastHisto() const { return lastHisto ; }
  void SetLastHisto(Histogram * value) { lastHisto = value ; }

  Window * GetLastTimeline() const { return lastTimeline ; }
  void SetLastTimeline(Window * value) { lastTimeline = value ; }

  std::queue<std::string> GetLoadFilesQueue() const { return loadFilesQueue ; }
  void SetLoadFilesQueue(std::queue<std::string> value) { loadFilesQueue = value ; }

  std::vector<Trace *> GetLoadedTraces() const { return loadedTraces ; }
  void SetLoadedTraces(std::vector<Trace *> value) { loadedTraces = value ; }

  KernelConnection* GetLocalKernel() const { return localKernel ; }
  void SetLocalKernel(KernelConnection* value) { localKernel = value ; }

  int GetNumNewDerived() const { return numNewDerived ; }
  void SetNumNewDerived(int value) { numNewDerived = value ; }

  int GetNumNewHistograms() const { return numNewHistograms ; }
  void SetNumNewHistograms(int value) { numNewHistograms = value ; }

  int GetNumNewWindows() const { return numNewWindows ; }
  void SetNumNewWindows(int value) { numNewWindows = value ; }

  ParaverConfig* GetParaverConfig() const { return paraverConfig ; }
  void SetParaverConfig(ParaverConfig* value) { paraverConfig = value ; }

  PreviousFiles * GetPreviousCFGs() const { return previousCFGs ; }
  void SetPreviousCFGs(PreviousFiles * value) { previousCFGs = value ; }

  PreviousFiles * GetPreviousCutFilteredTraces() const { return previousCutFilteredTraces ; }
  void SetPreviousCutFilteredTraces(PreviousFiles * value) { previousCutFilteredTraces = value ; }

  PreviousFiles * GetPreviousSessions() const { return previousSessions ; }
  void SetPreviousSessions(PreviousFiles * value) { previousSessions = value ; }

  PreviousFiles * GetPreviousTraces() const { return previousTraces ; }
  void SetPreviousTraces(PreviousFiles * value) { previousTraces = value ; }

  std::vector< PropertyClientData * > GetPropertiesClientData() const { return propertiesClientData ; }
  void SetPropertiesClientData(std::vector< PropertyClientData * > value) { propertiesClientData = value ; }

  bool GetRaiseCurrentWindow() const { return raiseCurrentWindow ; }
  void SetRaiseCurrentWindow(bool value) { raiseCurrentWindow = value ; }

  RunScript * GetRunApplication() const { return runApplication ; }
  void SetRunApplication(RunScript * value) { runApplication = value ; }

  wxTimer * GetSessionTimer() const { return sessionTimer ; }
  void SetSessionTimer(wxTimer * value) { sessionTimer = value ; }

  std::queue<SignalItem> GetSignalQueue() const { return signalQueue ; }
  void SetSignalQueue(std::queue<SignalItem> value) { signalQueue = value ; }

  bool GetTraceLoadedBefore() const { return traceLoadedBefore ; }
  void SetTraceLoadedBefore(bool value) { traceLoadedBefore = value ; }

  wxString GetTracePath() const { return tracePath ; }
  void SetTracePath(wxString value) { tracePath = value ; }

  std::map< Trace*, std::vector< std::string > > GetTraceWorkspaces() const { return traceWorkspaces ; }
  void SetTraceWorkspaces(std::map< Trace*, std::vector< std::string > > value) { traceWorkspaces = value ; }

  HelpContents * GetTutorialsWindow() const { return tutorialsWindow ; }
  void SetTutorialsWindow(HelpContents * value) { tutorialsWindow = value ; }

  WorkspaceManager * GetWorkspacesManager() const { return workspacesManager ; }
  void SetWorkspacesManager(WorkspaceManager * value) { workspacesManager = value ; }

  wxDialog * GetOpenedPropertyDialog() const { return openedPropertyDialog ; }
  void SetOpenedPropertyDialog(wxDialog * value) { openedPropertyDialog = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end paraverMain member function declarations
  
  gTimeline  * GetSelectedTimeline();
  gHistogram * GetSelectedHistogram();

  /// Should we show tooltips?
  bool ShowToolTips();
  void refreshTree( gTimeline *whichTimeline, Window *window );
  
  int GetNextPosX();
  int GetNextPosY();
  int GetDefaultTitleBarHeight();
  void selectTrace( Trace *trace );
  PRV_UINT16 getTracePosition( Trace *trace );

  void commandLineLoadings( wxCmdLineParser &parser );
  
  void clearProperties();
  
  void enqueueFile( std::string whichFile );
  
#ifndef WIN32
  void OnSignal();
  bool matchTraceNames( const std::string &fileName1, 
                        const std::string &traceName1,
                        const std::string &fileName2 );
  void insertSignalItem( bool isSig1 );
#endif // WIN32

#ifdef WIN32
  void OnKeyCopy();
  void OnKeyPaste();
#endif // WIN32
  void OnFindDialog();
  
  void DoLoadSession( const std::string &whichFileName );
  bool DoLoadTrace( const std::string &path );
  void UnloadTrace( int whichTrace );
  bool DoLoadCFG( const std::string &path );
  
  void SaveConfigurationFile( wxWindow *parent,
                               SaveOptions options,
                               std::vector< Window * > timelines,
                               std::vector< Histogram * > histograms );

  void ShowPreferences( wxWindowID whichPanelID = ID_PREFERENCES_GLOBAL );
  
  void MainSettingsCutFilterDialog( CutFilterDialog *cutFilterDialog,
                                     const std::string& filename,
                                     bool loadTrace );
  void OptionsSettingCutFilterDialog( CutFilterDialog *cutFilterDialog,
                                       TraceOptions *traceOptions,
                                       const std::string& xmlFile,
                                       std::vector< std::string > &filterToolOrder );
  void OnOKCutFilterDialog( CutFilterDialog *cutFilterDialog,
                             std::vector< std::string > filterToolOrder );
  void ShowCutTraceWindow( const std::string& filename = "",
                            bool loadTrace = true,
                            const std::string& xmlFile = "" );
  
  bool getAutoRedraw() const;
  bool isCFG4DModeDisabled() const;

  void checkIfPrevSessionLoad( bool prevSessionWasComplete );
  void LastSessionLoad( bool isSessionInitialized );
  
  // void ShowRunCommand( wxString app, wxString traceFile, wxString command, bool runNow );
  void ShowRunCommand( wxString traceFile );

  Window *createBaseWindow( wxString whichName = wxString( wxT("") ) );
  void insertInTree( Window *whichWindow );

  void createHelpContentsWindow( const wxString &helpContentsBaseRelativePath,
                                 const wxString &helpFile = wxString( wxT("") ),
                                 const wxString &hRef = wxString( wxT("") ),
                                 bool  isModal = false );
  bool getParaverHome( wxString &paraverHome );

  static wxString getHintComposed( const std::pair< std::string, std::string >& hint );
  static wxString buildFormattedFileName( std::string windowName, const std::string& traceName );

  static wxProgressDialog *dialogProgress;
  static paraverMain* myParaverMain;  // for update tree of windows
  static wxSize defaultTitleBarSize; // solves the problem of properly get timeline window height
  static wxSize defaultWindowSize;
  static int initialPosX;
  static int initialPosY;
  static Window *beginDragWindow;
  static Window *endDragWindow;
  static bool disableUserMessages;

  static bool IsSessionValid();
  static void ValidateSession( bool setValidate );

  static wxArrayString FromVectorStringToWxArray( std::vector< std::string > vec, std::string fileType = "txt" );
  static std::vector< std::string > FromWxArrayToVectorString( wxArrayString arr );
  
  bool OnMenusavesession( );
  void helpQuestion();

////@begin paraverMain member variables
  wxAuiManager m_auiManager;
  wxMenu* menuFile;
  wxMenu* menuHints;
  wxMenu* menuHelp;
  wxToolBar* tbarMain;
  wxChoicebook* choiceWindowBrowser;
  wxToolbook* toolBookFilesProperties;
  wxGenericDirCtrl* dirctrlFiles;
  wxPropertyGrid* windowProperties;
  wxPanel* panelAutoRedraw;
  wxCheckBox* checkAutoRedraw;
  wxButton* buttonForceRedraw;
  wxTextCtrl* txtActiveWorkspaces;
  wxButton* btnActiveWorkspaces;
  PreviousFiles * previousSessions;
private:
  bool CFGLoadedBefore;
  wxString CFGPath;
  bool XMLLoadedBefore;
  std::string XMLPath;
  std::set<wxWindow *> activeWindows;
  bool canServeSignal;
  Window * clusteringWindow;
  Histogram * currentHisto;
  Window * currentTimeline;
  PRV_INT16 currentTrace;
  wxWindow * currentWindow;
  std::map<Trace *, size_t> firstUserWorkspace;
  HelpContents * helpContents;
  wxImageList* imageList;
  Histogram * lastHisto;
  Window * lastTimeline;
  std::queue<std::string> loadFilesQueue;
  std::vector<Trace *> loadedTraces;
  KernelConnection* localKernel;
  int numNewDerived;
  int numNewHistograms;
  int numNewWindows;
  ParaverConfig* paraverConfig;
  PreviousFiles * previousCFGs;
  PreviousFiles * previousCutFilteredTraces;
  PreviousFiles * previousTraces;
  std::vector< PropertyClientData * > propertiesClientData;
  bool raiseCurrentWindow;
  RunScript * runApplication;
  wxTimer * sessionTimer;
  std::queue<SignalItem> signalQueue;
  bool traceLoadedBefore;
  wxString tracePath;
  std::map< Trace*, std::vector< std::string > > traceWorkspaces;
  HelpContents * tutorialsWindow;
  WorkspaceManager * workspacesManager;
  wxDialog * openedPropertyDialog;
////@end paraverMain member variables
  SessionInfo sessionInfo;
  bool firstSave;

  wxSingleInstanceChecker *instChecker;
  std::map< std::string, PRV_UINT32 > traceInstance;

  static const int CUTOFF = 10;
  static bool validSessions;

//  void updateTreeItem( wxTreeCtrl *tree, wxTreeItemId& id );

  wxImageList *getImageList();

/*
  void BuildTree( wxTreeCtrl *root1, wxTreeItemId idRoot1,
                  wxTreeCtrl *root2, wxTreeItemId idRoot2,
                  Window *window );
*/
  void ShowDerivedDialog();
  void ShowHistogramDialog();

  std::string DoLoadFilteredTrace( std::string traceFileName,
                                   std::string traceFilePath,
                                   TraceOptions *traceOptions,
                                   std::vector< std::string > &filterToolOrder );

  void HandleMaxSessionFiles();
  void PrepareToExit();
  
  void OnSessionTimer( wxTimerEvent& event );
  
  void refreshMenuHints();
  void setTraceWorkspaces( Trace *whichTrace );

  void updateActiveWorkspaces( Trace *whichTrace );
  void OnHintClick( wxCommandEvent& event );
  void OnMenuHintUpdate( wxUpdateUIEvent& event );
  void setActiveWorkspacesText();

  bool isSessionFile( const std::string& filename );
  void exitManager( wxEvent& event );
  void messageUnknownPath( wxString helpContentsPath, wxString paraverHome );
  void messageUndefinedParaverHome();
  
};

void progressFunction( ProgressController *progress, void *callerWindow );
  

#endif // _PARAVERMAIN_H_
