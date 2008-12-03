/////////////////////////////////////////////////////////////////////////////
// Name:        paravermain.h
// Purpose:     
// Author:      Eloy Martinez
// Modified by: 
// Created:     Wed 25 Jun 2008 15:58:35 CEST
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _PARAVERMAIN_H_
#define _PARAVERMAIN_H_


/*!
 * Includes
 */

#include <vector>
#include <wx/progdlg.h>
////@begin includes
#include "wx/aui/framemanager.h"
#include "wx/frame.h"
#include "wx/toolbar.h"
#include "wx/choicebk.h"
////@end includes
#include "wx/propgrid/propgrid.h"
#include "wx/treectrl.h"
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

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_PARAVERMAIN 10000
#define ID_RECENTTRACES 10008
#define ID_MENULOADCFG 10006
#define ID_RECENTCFGS 10009
#define ID_TOOLBAR 10003
#define ID_CHOICEWINBROWSER 10002
#define ID_FOREIGN 10010
#define SYMBOL_PARAVERMAIN_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PARAVERMAIN_TITLE _("Paraver")
#define SYMBOL_PARAVERMAIN_IDNAME ID_PARAVERMAIN
#define SYMBOL_PARAVERMAIN_SIZE wxSize(-1, 400)
#define SYMBOL_PARAVERMAIN_POSITION wxPoint(0, 0)
////@end control identiparfiers

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

  /// wxEVT_IDLE event handler for ID_PARAVERMAIN
  void OnIdle( wxIdleEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_OPEN
  void OnOpenClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_RECENTTRACES
  void OnRecenttracesUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENULOADCFG
  void OnMenuloadcfgClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_MENULOADCFG
  void OnMenuloadcfgUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
  void OnExitClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_CHOICEWINBROWSER
  void OnChoicewinbrowserUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_FOREIGN
  void OnForeignUpdate( wxUpdateUIEvent& event );

////@end paraverMain event handler declarations

  /// wxEVT_TREE_SEL_CHANGED event handler for wxID_ANY
  void OnTreeSelChanged( wxTreeEvent& event );
  
  /// wxEVT_TREE_ITEM_ACTIVATED event handler for wxID_ANY
  void OnTreeItemActivated( wxTreeEvent& event );
  
  /// wxEVT_PG_CHANGED event handler for ID_FOREIGN
  void OnPropertyGridChange( wxPropertyGridEvent& event );

  void OnPreviousTracesClick( wxCommandEvent& event );
  void OnPreviousCFGsClick( wxCommandEvent& event );

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

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end paraverMain member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  static wxProgressDialog *dialogProgress;
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
////@end paraverMain member variables
  
  void updateTimelineProperties( Window *whichWindow );
  void updateHistogramProperties( Histogram *whichHisto );
  void updateTreeItem( wxTreeCtrl *tree, wxTreeItemId& id );
  bool DoLoadTrace( const string &path );
  void BuildTree( wxTreeCtrl *root1, wxTreeItemId idRoot1,
                  wxTreeCtrl *root2, wxTreeItemId idRoot2,
                  Window *window );
  bool DoLoadCFG( const string &path );
};

void progressFunction( ProgressController *progress );
  

#endif
  // _PARAVERMAIN_H_
