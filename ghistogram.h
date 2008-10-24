/////////////////////////////////////////////////////////////////////////////
// Name:        ghistogram.h
// Purpose:     
// Author:      Eloy Martinez
// Modified by: 
// Created:     Mon 23 Jun 2008 11:40:03 CEST
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _GHISTOGRAM_H_
#define _GHISTOGRAM_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/grid.h"
////@end includes
#include "paraverkerneltypes.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxGrid;
////@end forward declarations
class Histogram;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_GHISTOGRAM 10004
#define ID_GRIDHISTO 10005
#define SYMBOL_GHISTOGRAM_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMAXIMIZE_BOX|wxCLOSE_BOX|wxFRAME_NO_TASKBAR
#define SYMBOL_GHISTOGRAM_TITLE _("gHistogram")
#define SYMBOL_GHISTOGRAM_IDNAME ID_GHISTOGRAM
#define SYMBOL_GHISTOGRAM_SIZE wxSize(400, 300)
#define SYMBOL_GHISTOGRAM_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * gHistogram class declaration
 */

class gHistogram: public wxFrame
{    
  DECLARE_CLASS( gHistogram )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  gHistogram();
  gHistogram( wxWindow* parent, wxWindowID id = SYMBOL_GHISTOGRAM_IDNAME, const wxString& caption = SYMBOL_GHISTOGRAM_TITLE, const wxPoint& pos = SYMBOL_GHISTOGRAM_POSITION, const wxSize& size = SYMBOL_GHISTOGRAM_SIZE, long style = SYMBOL_GHISTOGRAM_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_GHISTOGRAM_IDNAME, const wxString& caption = SYMBOL_GHISTOGRAM_TITLE, const wxPoint& pos = SYMBOL_GHISTOGRAM_POSITION, const wxSize& size = SYMBOL_GHISTOGRAM_SIZE, long style = SYMBOL_GHISTOGRAM_STYLE );

  /// Destructor
  ~gHistogram();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin gHistogram event handler declarations

  /// wxEVT_CLOSE_WINDOW event handler for ID_GHISTOGRAM
  void OnCloseWindow( wxCloseEvent& event );

  /// wxEVT_IDLE event handler for ID_GHISTOGRAM
  void OnIdle( wxIdleEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_GRIDHISTO
  void OnGridhistoUpdate( wxUpdateUIEvent& event );

////@end gHistogram event handler declarations

////@begin gHistogram member function declarations

  Histogram* GetHistogram() const { return myHistogram ; }
  void SetHistogram(Histogram* value) { myHistogram = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end gHistogram member function declarations

  void execute();
  void fillGrid();
  void fillTotals( int& rowLabelWidth, TObjectOrder beginRow, THistogramColumn curPlane, UINT16 idStat );
  
  /// Should we show tooltips?
  static bool ShowToolTips();

////@begin gHistogram member variables
  wxGrid* gridHisto;
private:
  Histogram* myHistogram;
////@end gHistogram member variables
};

#endif
  // _GHISTOGRAM_H_
