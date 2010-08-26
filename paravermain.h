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

////@begin includes
#include "wx/aui/framemanager.h"
#include "wx/frame.h"
#include "wx/toolbar.h"
#include "wx/choicebk.h"
////@end includes
#include "trace.h"
#include "localkernel.h"
#include "paraverconfig.h"
#include "previousfiles.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxMenu;
class wxChoicebook;
class wxPropertyGrid;
////@end forward declarations
class ProgressController;
class wxPropertyGridEvent;

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
#define ID_PREFERENCES 10001
#define ID_TOOLBAR 10003
#define ID_NEW_WINDOW 10030
#define ID_NEW_DERIVED_WINDOW 10031
#define ID_NEW_HISTOGRAM 10060
#define ID_TOOLDELETE 10081
#define ID_TOOL_CUT_TRACE 10100
#define ID_CHOICEWINBROWSER 10002
#define ID_FOREIGN 10010
#define SYMBOL_PARAVERMAIN_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxWANTS_CHARS
#define SYMBOL_PARAVERMAIN_TITLE _("Paraver")
#define SYMBOL_PARAVERMAIN_IDNAME ID_PARAVERMAIN
#define SYMBOL_PARAVERMAIN_SIZE wxSize(300, 600)
#define SYMBOL_PARAVERMAIN_POSITION wxPoint(0, 0)
////@end control identifiers


class gTimeline;
class gHistogram;

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
  paraverMain( wxWindow* parent, wxWindowID id = SYMBOL_PARAVERMAIN_IDNAME, const wxString& caption = SYMBOL_PARAVERMAIN_TITLE, const wxPoint& pos = SYMBOL_PARAVERMAIN_POSITION, const wxSize& size = SYMBOL_PARAVERMAIN_SIZE, long style = SYMBOL_PARAVERMAIN_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_PARAVERMAIN_IDNAME, const wxString& caption = SYMBOL_PARAVERMAIN_TITLE, const wxPoint& pos = SYMBOL_PARAVERMAIN_POSITION, const wxSize& size = SYMBOL_PARAVERMAIN_SIZE, long style = SYMBOL_PARAVERMAIN_STYLE );

  /// Destructor
  ~paraverMain();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin paraverMain event handler declarations

  /// wxEVT_CLOSE_WINDOW event handler for ID_PARAVERMAIN
  void OnCloseWindow( wxCloseEvent& event );

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

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_PREFERENCES
  void OnPreferencesClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_PREFERENCES
  void OnPreferencesUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
  void OnExitClick( wxCommandEvent& event );

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

  /// wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED event handler for ID_CHOICEWINBROWSER
  void OnChoicewinbrowserPageChanged( wxChoicebookEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_CHOICEWINBROWSER
  void OnChoicewinbrowserUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_FOREIGN
  void OnForeignUpdate( wxUpdateUIEvent& event );

////@end paraverMain event handler declarations

  /// wxEVT_TREE_SEL_CHANGED event handler for wxID_ANY
  void OnTreeSelChanged( wxTreeEvent& event );
  
  /// wxEVT_TREE_ITEM_ACTIVATED event handler for wxID_ANY
  void OnTreeItemActivated( wxTreeEvent& event );
  void OnTreeRightClick( wxTreeEvent& event );
  
  void OnTreeBeginDrag( wxTreeEvent& event );
  void OnTreeEndDrag( wxTreeEvent& event );
  
  /// wxEVT_PG_CHANGED event handler for ID_FOREIGN
  void OnPropertyGridChange( wxPropertyGridEvent& event );

  void OnPreviousTracesClick( wxCommandEvent& event );
  void OnPreviousCFGsClick( wxCommandEvent& event );

  void OnActivate( wxActivateEvent& event );
////@begin paraverMain member function declarations

  /// Returns the AUI manager object
  wxAuiManager& GetAuiManager() { return m_auiManager; }

  vector<Trace *> GetLoadedTraces() const { return loadedTraces ; }
  void SetLoadedTraces(vector<Trace *> value) { loadedTraces = value ; }

  KernelConnection* GetLocalKernel() const { return localKernel ; }
  void SetLocalKernel(KernelConnection* value) { localKernel = value ; }

  ParaverConfig* GetParaverConfig() const { return paraverConfig ; }
  void SetParaverConfig(ParaverConfig* value) { paraverConfig = value ; }

  INT16 GetCurrentTrace() const { return currentTrace ; }
  void SetCurrentTrace(INT16 value) { currentTrace = value ; }

  wxImageList* GetImageList() const { return imageList ; }
  void SetImageList(wxImageList* value) { imageList = value ; }

  Window * GetCurrentTimeline() const { return currentTimeline ; }
  void SetCurrentTimeline(Window * value) { currentTimeline = value ; }

  Histogram * GetCurrentHisto() const { return currentHisto ; }
  void SetCurrentHisto(Histogram * value) { currentHisto = value ; }

  PreviousFiles * GetPreviousTraces() const { return previousTraces ; }
  void SetPreviousTraces(PreviousFiles * value) { previousTraces = value ; }

  PreviousFiles * GetPreviousCFGs() const { return previousCFGs ; }
  void SetPreviousCFGs(PreviousFiles * value) { previousCFGs = value ; }

  Window * GetLastTimeline() const { return lastTimeline ; }
  void SetLastTimeline(Window * value) { lastTimeline = value ; }

  Histogram * GetLastHisto() const { return lastHisto ; }
  void SetLastHisto(Histogram * value) { lastHisto = value ; }

  wxWindow * GetCurrentWindow() const { return currentWindow ; }
  void SetCurrentWindow(wxWindow * value) { currentWindow = value ; }

  int GetNumNewWindows() const { return numNewWindows ; }
  void SetNumNewWindows(int value) { numNewWindows = value ; }

  int GetNumNewHistograms() const { return numNewHistograms ; }
  void SetNumNewHistograms(int value) { numNewHistograms = value ; }

  int GetNumNewDerived() const { return numNewDerived ; }
  void SetNumNewDerived(int value) { numNewDerived = value ; }

  bool GetRaiseCurrentWindow() const { return raiseCurrentWindow ; }
  void SetRaiseCurrentWindow(bool value) { raiseCurrentWindow = value ; }

  set<wxWindow *> GetActiveWindows() const { return activeWindows ; }
  void SetActiveWindows(set<wxWindow *> value) { activeWindows = value ; }

  bool GetTraceLoadedBefore() const { return traceLoadedBefore ; }
  void SetTraceLoadedBefore(bool value) { traceLoadedBefore = value ; }

  bool GetCFGLoadedBefore() const { return CFGLoadedBefore ; }
  void SetCFGLoadedBefore(bool value) { CFGLoadedBefore = value ; }

  wxString GetTracePath() const { return tracePath ; }
  void SetTracePath(wxString value) { tracePath = value ; }

  wxString GetCFGPath() const { return CFGPath ; }
  void SetCFGPath(wxString value) { CFGPath = value ; }

  bool GetCanServeSignal() const { return canServeSignal ; }
  void SetCanServeSignal(bool value) { canServeSignal = value ; }

  queue<string> GetLoadFilesQueue() const { return loadFilesQueue ; }
  void SetLoadFilesQueue(queue<string> value) { loadFilesQueue = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end paraverMain member function declarations

  /// Should we show tooltips?
  bool ShowToolTips();
  void refreshTree( gTimeline *whichTimeline, Window *window );
  
  int GetNextPosX();
  int GetNextPosY();
  void selectTrace( Trace *trace );
  UINT16 getTracePosition( Trace *trace );

  void commandLineLoadings( wxCmdLineParser &parser );
  
  void clearProperties();
  
  void addActiveWindow( wxWindow *window );
  void removeActiveWindow( wxWindow *window );

  void enqueueFile( string whichFile );
  
#ifndef WIN32
//  void OnSignal( int sigusr );
  void OnSignal( );
#endif

#ifdef WIN32
  void OnKeyCopy();
  void OnKeyPaste();
#endif

  static wxProgressDialog *dialogProgress;
  static paraverMain* myParaverMain;  // for update tree of windows
  static wxSize defaultTitleBarSize; // solves the problem of properly get timeline window height
  static wxSize defaultWindowSize;
  static int initialPosX;
  static int initialPosY;
  static Window *beginDragWindow;
  static Window *endDragWindow;

////@begin paraverMain member variables
  wxAuiManager m_auiManager;
  wxMenu* menuFile;
  wxMenu* menuHelp;
  wxToolBar* tbarMain;
  wxChoicebook* choiceWindowBrowser;
  wxPropertyGrid* windowProperties;
private:
  vector<Trace *> loadedTraces;
  KernelConnection* localKernel;
  ParaverConfig* paraverConfig;
  INT16 currentTrace;
  wxImageList* imageList;
  Window * currentTimeline;
  Histogram * currentHisto;
  PreviousFiles * previousTraces;
  PreviousFiles * previousCFGs;
  Window * lastTimeline;
  Histogram * lastHisto;
  wxWindow * currentWindow;
  int numNewWindows;
  int numNewHistograms;
  int numNewDerived;
  bool raiseCurrentWindow;
  set<wxWindow *> activeWindows;
  bool traceLoadedBefore;
  bool CFGLoadedBefore;
  wxString tracePath;
  wxString CFGPath;
  bool canServeSignal;
  queue<string> loadFilesQueue;
////@end paraverMain member variables

  map< string, UINT32 > traceInstance;

//  void updateTreeItem( wxTreeCtrl *tree, wxTreeItemId& id );

  wxImageList *getImageList();

  bool DoLoadTrace( const string &path );
  void UnloadTrace( int whichTrace );
/*
  void BuildTree( wxTreeCtrl *root1, wxTreeItemId idRoot1,
                  wxTreeCtrl *root2, wxTreeItemId idRoot2,
                  Window *window );
*/
  bool DoLoadCFG( const string &path );
  void ShowDerivedDialog();
  void ShowHistogramDialog();

  string DoLoadFilteredTrace( string traceFileName,
                              TraceOptions *traceOptions,
                              vector< int > &filterToolOrder );
  void ShowCutTraceWindow();
  
  void PrepareToExit();
};

void progressFunction( ProgressController *progress );
  

#endif
  // _PARAVERMAIN_H_
