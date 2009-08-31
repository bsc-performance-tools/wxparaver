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
#include "wx/toolbar.h"
#include "wx/statusbr.h"
////@end includes
#include <wx/statbmp.h>
#include "wx/timer.h"
#include "paraverkerneltypes.h"
#include "popupmenu.h"
#include "copypaste.h"
// TEMPLATES
#include "zoomhistory.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxBoxSizer;
class wxGrid;
class wxStatusBar;
////@end forward declarations
class Histogram;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_GHISTOGRAM 10004
#define ID_ZOOMHISTO 10023
#define ID_GRIDHISTO 10005
#define wxID_CONTROLWARNING 10024
#define wxID_3DWARNING 10057
#define ID_AUITOOLBAR1 10059
#define ID_TOOL_OPEN_CONTROL_WINDOW 10050
#define ID_TOOL_OPEN_DATA_WINDOW 10051
#define ID_TOOL_OPEN_EXTRA_WINDOW 10052
#define ID_TOOLZOOM 10025
#define ID_TOOL_OPEN_FILTERED_CONTROL_WINDOW 10029
#define ID_TOOLGRADIENT 10026
#define ID_TOOLHORIZVERT 10027
#define ID_TOOL_HIDE_COLUMNS 10058
#define ID_HISTOSTATUS 10028
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

  /// wxEVT_SIZE event handler for ID_GHISTOGRAM
  void OnSize( wxSizeEvent& event );

  /// wxEVT_IDLE event handler for ID_GHISTOGRAM
  void OnIdle( wxIdleEvent& event );

  /// wxEVT_PAINT event handler for ID_ZOOMHISTO
  void OnPaint( wxPaintEvent& event );

  /// wxEVT_ERASE_BACKGROUND event handler for ID_ZOOMHISTO
  void OnEraseBackground( wxEraseEvent& event );

  /// wxEVT_LEFT_DOWN event handler for ID_ZOOMHISTO
  void OnLeftDown( wxMouseEvent& event );

  /// wxEVT_LEFT_UP event handler for ID_ZOOMHISTO
  void OnLeftUp( wxMouseEvent& event );

  /// wxEVT_MOTION event handler for ID_ZOOMHISTO
  void OnMotion( wxMouseEvent& event );

  /// wxEVT_CONTEXT_MENU event handler for ID_ZOOMHISTO
  void OnZoomContextMenu( wxContextMenuEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_ZOOMHISTO
  void OnZoomhistoUpdate( wxUpdateUIEvent& event );

  /// wxEVT_GRID_CELL_RIGHT_CLICK event handler for ID_GRIDHISTO
  void OnCellRightClick( wxGridEvent& event );

  /// wxEVT_GRID_LABEL_RIGHT_CLICK event handler for ID_GRIDHISTO
  void OnLabelRightClick( wxGridEvent& event );

  /// wxEVT_GRID_RANGE_SELECT event handler for ID_GRIDHISTO
  void OnRangeSelect( wxGridRangeSelectEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_GRIDHISTO
  void OnGridhistoUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for wxID_CONTROLWARNING
  void OnControlWarningUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for wxID_3DWARNING
  void On3dWarningUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_OPEN_CONTROL_WINDOW
  void OnToolOpenControlWindowClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_OPEN_DATA_WINDOW
  void OnToolOpenDataWindowClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_OPEN_EXTRA_WINDOW
  void OnToolOpenExtraWindowClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TOOL_OPEN_EXTRA_WINDOW
  void OnToolOpenExtraWindowUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOLZOOM
  void OnToolzoomClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TOOLZOOM
  void OnToolzoomUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_OPEN_FILTERED_CONTROL_WINDOW
  void OnToolOpenFilteredControlWindowClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TOOL_OPEN_FILTERED_CONTROL_WINDOW
  void OnToolOpenFilteredControlWindowUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOLGRADIENT
  void OnToolgradientClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TOOLGRADIENT
  void OnToolgradientUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOLHORIZVERT
  void OnToolhorizvertClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TOOLHORIZVERT
  void OnToolhorizvertUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_HIDE_COLUMNS
  void OnToolHideColumnsClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TOOL_HIDE_COLUMNS
  void OnToolHideColumnsUpdate( wxUpdateUIEvent& event );

////@end gHistogram event handler declarations

////@begin gHistogram member function declarations

  Histogram* GetHistogram() const { return myHistogram ; }
  void SetHistogram(Histogram* value) { myHistogram = value ; }

  bool GetReady() const { return ready ; }
  void SetReady(bool value) { ready = value ; }

  wxBitmap GetZoomImage() const { return zoomImage ; }
  void SetZoomImage(wxBitmap value) { zoomImage = value ; }

  wxTimer * GetTimerZoom() const { return timerZoom ; }
  void SetTimerZoom(wxTimer * value) { timerZoom = value ; }

  double GetLastPosZoomX() const { return lastPosZoomX ; }
  void SetLastPosZoomX(double value) { lastPosZoomX = value ; }

  double GetLastPosZoomY() const { return lastPosZoomY ; }
  void SetLastPosZoomY(double value) { lastPosZoomY = value ; }

  double GetZoomCellWidth() const { return zoomCellWidth ; }
  void SetZoomCellWidth(double value) { zoomCellWidth = value ; }

  double GetZommCellHeight() const { return zoomCellHeight ; }
  void SetZommCellHeight(double value) { zoomCellHeight = value ; }

  bool GetOpenControlActivated() const { return openControlActivated ; }
  void SetOpenControlActivated(bool value) { openControlActivated = value ; }

  wxPoint GetZoomPointBegin() const { return zoomPointBegin ; }
  void SetZoomPointBegin(wxPoint value) { zoomPointBegin = value ; }

  wxPoint GetZoomPointEnd() const { return zoomPointEnd ; }
  void SetZoomPointEnd(wxPoint value) { zoomPointEnd = value ; }

  wxBitmap GetDrawImage() const { return drawImage ; }
  void SetDrawImage(wxBitmap value) { drawImage = value ; }

  bool GetOpenControlDragging() const { return openControlDragging ; }
  void SetOpenControlDragging(bool value) { openControlDragging = value ; }

  vector<TObjectOrder> GetSelectedRows() const { return selectedRows ; }
  void SetSelectedRows(vector<TObjectOrder> value) { selectedRows = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end gHistogram member function declarations

  void execute();
  void fillGrid();
  void fillZoom();
  
  /// Should we show tooltips?
  static bool ShowToolTips();


  // Pop Up Menu Methods
  void OnPopUpCopy();
  void OnPopUpPaste();
  void OnPopUpPasteSpecial();
  void OnPopUpPasteTime();
  void OnPopUpPasteObjects();
  void OnPopUpPasteSize();
  void OnPopUpClone();
  void OnPopUpFitTimeScale();
  void OnPopUpAutoControlScale( bool state );
  void OnPopUpAutoDataGradient( bool state );
  void OnPopUpUndoZoom();
  void OnPopUpRedoZoom();

  void OnPopUpDrawModeSemanticLast();
  void OnPopUpDrawModeSemanticRandom();
  void OnPopUpDrawModeSemanticRandomNotZero();
  void OnPopUpDrawModeSemanticMaximum();
  void OnPopUpDrawModeSemanticMinimumNotZero();
  void OnPopUpDrawModeSemanticAverage();

  void OnPopUpDrawModeObjectsLast();
  void OnPopUpDrawModeObjectsRandom();
  void OnPopUpDrawModeObjectsRandomNotZero();
  void OnPopUpDrawModeObjectsMaximum();
  void OnPopUpDrawModeObjectsMinimumNotZero();
  void OnPopUpDrawModeObjectsAverage();

  void OnPopUpDrawModeBothLast();
  void OnPopUpDrawModeBothRandom();
  void OnPopUpDrawModeBothRandomNotZero();
  void OnPopUpDrawModeBothMaximum();
  void OnPopUpDrawModeBothMinimumNotZero();
  void OnPopUpDrawModeBothAverage();

  void rightDownManager();

////@begin gHistogram member variables
  wxBoxSizer* mainSizer;
  wxScrolledWindow* zoomHisto;
  wxGrid* gridHisto;
  wxBoxSizer* warningSizer;
  wxStaticBitmap* controlWarning;
  wxStaticBitmap* xtraWarning;
  wxStatusBar* histoStatus;
private:
  Histogram* myHistogram;
  bool ready;
  wxBitmap zoomImage;
  wxTimer * timerZoom;
  double lastPosZoomX;
  double lastPosZoomY;
  double zoomCellWidth;
  double zoomCellHeight;
  bool openControlActivated;
  wxPoint zoomPointBegin;
  wxPoint zoomPointEnd;
  wxBitmap drawImage;
  bool openControlDragging;
  vector<TObjectOrder> selectedRows;
////@end gHistogram member variables
  wxWindow *parent; // for clone

  void updateHistogram();
  void OnTimerZoom( wxTimerEvent& event );
  TSemanticValue getZoomSemanticValue( THistogramColumn column, TObjectOrder row ) const;
//  ZoomHistory<TTime, TObjectOrder> *zoomHistory;
  void drawColumn( THistogramColumn beginColumn, THistogramColumn endColumn, wxMemoryDC& bufferDraw );
  void fillTotals( int& rowLabelWidth, TObjectOrder beginRow, THistogramColumn curPlane, UINT16 idStat );
  void openControlGetParameters( int xBegin, int xEnd, int yBegin, int yEnd,
                                 THistogramColumn& columnBegin, THistogramColumn& columnEnd,
                                 TObjectOrder& objectBegin, TObjectOrder& objectEnd );
  void openControlWindow( THistogramColumn columnBegin, THistogramColumn columnEnd,
                          TObjectOrder objectBegin, TObjectOrder objectEnd );
};

#endif
  // _GHISTOGRAM_H_
