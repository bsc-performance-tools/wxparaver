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
////@begin includes
#include "wx/aui/framemanager.h"
#include "wx/frame.h"
#include "wx/toolbar.h"
#include "wx/choicebk.h"
////@end includes
#include "wx/treectrl.h"
#include "trace.h"
#include "localkernel.h"
#include "paraverconfig.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxMenu;
class wxChoicebook;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_PARAVERMAIN 10000
#define ID_MENULOADCFG 10006
#define ID_TOOLBAR 10003
#define ID_CHOICEWINBROWSER 10002
#define SYMBOL_PARAVERMAIN_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PARAVERMAIN_TITLE _("Paraver")
#define SYMBOL_PARAVERMAIN_IDNAME ID_PARAVERMAIN
#define SYMBOL_PARAVERMAIN_SIZE wxDefaultSize
#define SYMBOL_PARAVERMAIN_POSITION wxPoint(0, 0)
////@end control identifiers


class TreeBrowserItemData: public wxTreeItemData
{
  public:
    TreeBrowserItemData( const wxString& whichDesc, wxWindow* whichWin ) :
      desc( whichDesc ), myWindow( whichWin )
    {}
    
    const wxString& GetDesc() const
    {
      return desc;
    }
    
  private:
    wxString desc;
    wxWindow* myWindow;
    Histogram* myHisto;
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

  /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_OPEN
  void OnOpenClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENULOADCFG
  void OnMenuloadcfgClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_MENULOADCFG
  void OnMenuloadcfgUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
  void OnExitClick( wxCommandEvent& event );

////@end paraverMain event handler declarations

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

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end paraverMain member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

////@begin paraverMain member variables
  wxAuiManager m_auiManager;
  wxMenu* menuFile;
  wxMenu* menuHelp;
  wxToolBar* tbarMain;
  wxChoicebook* choiceWindowBrowser;
private:
  vector<Trace *> loadedTraces;
  KernelConnection* localKernel;
  ParaverConfig* paraverConfig;
  INT16 currentTrace;
  wxImageList* imageList;
////@end paraverMain member variables
};

#endif
  // _PARAVERMAIN_H_
