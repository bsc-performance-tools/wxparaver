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

#pragma once

/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/toolbar.h"
#include "wx/grid.h"
#include "wx/statusbr.h"
////@end includes
#include <wx/statbmp.h>
#include <wx/choice.h>
#include "wx/timer.h"
#include "paraverkerneltypes.h"
#include "copypaste.h"
#include "windows_tree.h"

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
class HistoTableBase;
class gWindow;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_GHISTOGRAM 10004
#define HISTO_PANEL_TOOLBAR 10001
#define ID_TOOLBAR_HISTOGRAM 10059
#define ID_TOOL_OPEN_CONTROL_WINDOW 10050
#define ID_TOOL_OPEN_DATA_WINDOW 10051
#define ID_TOOL_OPEN_EXTRA_WINDOW 10052
#define ID_TOOLZOOM 10025
#define ID_TOOL_OPEN_FILTERED_CONTROL_WINDOW 10029
#define ID_TOOLGRADIENT 10026
#define ID_TOOLHORIZVERT 10027
#define ID_TOOL_HIDE_COLUMNS 10058
#define ID_TOOL_LABEL_COLORS 10101
#define ID_TOOL_SHORT_LABELS 10287
#define ID_TOOL_ONLY_TOTALS 10286
#define ID_TOOL_INCLUSIVE 10105
#define ID_TOOL_CHOICE_SORTBY 10002
#define ID_TOOL_REVERSE 10285
#define HISTO_PANEL_DATA 10000
#define ID_ZOOMHISTO 10023
#define ID_GRIDHISTO 10005
#define wxID_CONTROLWARNING 10024
#define wxID_3DWARNING 10057
#define ID_AUTOREDRAW 10299
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

class gHistogram: public wxFrame, public gWindow
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

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_LABEL_COLORS
  void OnToolLabelColorsClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TOOL_LABEL_COLORS
  void OnToolLabelColorsUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_SHORT_LABELS
  void OnToolShortLabelsClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TOOL_SHORT_LABELS
  void OnToolShortLabelsUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_ONLY_TOTALS
  void OnToolOnlyTotalsClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TOOL_ONLY_TOTALS
  void OnToolOnlyTotalsUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_INCLUSIVE
  void OnToolInclusiveClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TOOL_INCLUSIVE
  void OnToolInclusiveUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_TOOL_CHOICE_SORTBY
  void OnToolChoiceSortbySelected( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TOOL_CHOICE_SORTBY
  void OnToolChoiceSortbyUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_REVERSE
  void OnToolReverseClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TOOL_REVERSE
  void OnToolReverseUpdate( wxUpdateUIEvent& event );

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

  /// wxEVT_KEY_DOWN event handler for ID_ZOOMHISTO
  void OnZoomHistoKeyDown( wxKeyEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_ZOOMHISTO
  void OnZoomhistoUpdate( wxUpdateUIEvent& event );

  /// wxEVT_GRID_CELL_LEFT_CLICK event handler for ID_GRIDHISTO
  void OnCellLeftClick( wxGridEvent& event );

  /// wxEVT_GRID_CELL_RIGHT_CLICK event handler for ID_GRIDHISTO
  void OnCellRightClick( wxGridEvent& event );

  /// wxEVT_GRID_LABEL_LEFT_CLICK event handler for ID_GRIDHISTO
  void OnLabelLeftClick( wxGridEvent& event );

  /// wxEVT_GRID_LABEL_RIGHT_CLICK event handler for ID_GRIDHISTO
  void OnLabelRightClick( wxGridEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_GRIDHISTO
  void OnGridhistoUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for wxID_CONTROLWARNING
  void OnControlWarningUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for wxID_3DWARNING
  void On3dWarningUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_AUTOREDRAW
  void OnAutoredrawUpdate( wxUpdateUIEvent& event );

  /// wxEVT_LEFT_DOWN event handler for ID_AUTOREDRAW
  void OnAutoredrawLeftDown( wxMouseEvent& event );

////@end gHistogram event handler declarations

  void OnRangeSelect( wxGridRangeSelectEvent& event );

////@begin gHistogram member function declarations

  wxBitmap GetDrawImage() const { return drawImage ; }
  void SetDrawImage(wxBitmap value) { drawImage = value ; }

  bool GetEscapePressed() const { return escapePressed ; }
  void SetEscapePressed(bool value) { escapePressed = value ; }

  double GetLastPosZoomX() const { return lastPosZoomX ; }
  void SetLastPosZoomX(double value) { lastPosZoomX = value ; }

  double GetLastPosZoomY() const { return lastPosZoomY ; }
  void SetLastPosZoomY(double value) { lastPosZoomY = value ; }

  Histogram* GetHistogram() const { return myHistogram ; }
  void SetHistogram(Histogram* value) { myHistogram = value ; }

  bool GetOpenControlActivated() const { return openControlActivated ; }
  void SetOpenControlActivated(bool value) { openControlActivated = value ; }

  bool GetReady() const { return ready ; }
  void SetReady(bool value) { ready = value ; }

  wxStopWatch * GetRedrawStopWatch() const { return redrawStopWatch ; }
  void SetRedrawStopWatch(wxStopWatch * value) { redrawStopWatch = value ; }

  std::vector<TObjectOrder> GetSelectedRows() const { return selectedRows ; }
  void SetSelectedRows(std::vector<TObjectOrder> value) { selectedRows = value ; }

  HistoTableBase* GetTableBase() const { return tableBase ; }
  void SetTableBase(HistoTableBase* value) { tableBase = value ; }

  wxTimer * GetTimerZoom() const { return timerZoom ; }
  void SetTimerZoom(wxTimer * value) { timerZoom = value ; }

  double GetZommCellHeight() const { return zoomCellHeight ; }
  void SetZommCellHeight(double value) { zoomCellHeight = value ; }

  double GetZoomCellWidth() const { return zoomCellWidth ; }
  void SetZoomCellWidth(double value) { zoomCellWidth = value ; }

  bool GetZoomDragging() const { return zoomDragging ; }
  void SetZoomDragging(bool value) { zoomDragging = value ; }

  wxBitmap GetZoomImage() const { return zoomImage ; }
  void SetZoomImage(wxBitmap value) { zoomImage = value ; }

  wxPoint GetZoomPointBegin() const { return zoomPointBegin ; }
  void SetZoomPointBegin(wxPoint value) { zoomPointBegin = value ; }

  wxPoint GetZoomPointEnd() const { return zoomPointEnd ; }
  void SetZoomPointEnd(wxPoint value) { zoomPointEnd = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end gHistogram member function declarations

  const SelectionManagement< THistogramColumn, int >& GetColumnSelection() const { return columnSelection; }
  void SetColumnSelection( const SelectionManagement< THistogramColumn, int >& value ) { columnSelection = value; }

  void execute();

  void initColumnSelection();
  void fillGrid();
  void fillZoom();
  
  /// Should we show tooltips?
  static bool ShowToolTips();


  // Pop Up Menu Methods
  void OnPopUpCopy( wxCommandEvent& event );
  void OnPopUpPaste( wxCommandEvent& event );
  void OnPopUpPasteDefaultSpecial( wxCommandEvent& event );
  void OnPopUpPasteSpecial( wxCommandEvent& event );
  void OnPopUpPasteTime( wxCommandEvent& event );
  void OnPopUpPasteObjects( wxCommandEvent& event );
  void OnPopUpPasteSize( wxCommandEvent& event );
  void OnPopUpPasteDuration( wxCommandEvent& event );
  void OnPopUpPasteSemanticScale( wxCommandEvent& event );
  void OnPopUpPasteSemanticSort( wxCommandEvent& event );
  void OnPopUpPasteControlScale( wxCommandEvent& event );
  void OnPopUpPaste3DScale( wxCommandEvent& event );
  void OnPopUpPasteControlDimensions( wxCommandEvent& event );

  void OnPopUpClone( wxCommandEvent& event );
  void OnPopUpRename( wxCommandEvent& event );
  void OnPopUpFitTimeScale( wxCommandEvent& event );
  void OnPopUpFitObjects( wxCommandEvent& event );
  void OnPopUpRowSelection( wxCommandEvent& event );
  void OnPopUpAutoControlScale( wxCommandEvent& event );
  void OnPopUpAutoControlScaleZero( wxCommandEvent& event );
  void OnPopUpAuto3DScale( wxCommandEvent& event );
  void OnPopUpAutoDataGradient( wxCommandEvent& event );

  void OnPopUpColor2D( wxCommandEvent& event );
  void OnPopUpGradientFunction( wxCommandEvent& event );

  void OnPopUpUndoZoom( wxCommandEvent& event );
  void OnPopUpRedoZoom( wxCommandEvent& event );

  void OnPopUpDrawModeSemanticLast( wxCommandEvent& event );
  void OnPopUpDrawModeSemanticRandom( wxCommandEvent& event );
  void OnPopUpDrawModeSemanticRandomNotZero( wxCommandEvent& event );
  void OnPopUpDrawModeSemanticMaximum( wxCommandEvent& event );
  void OnPopUpDrawModeSemanticMinimumNotZero( wxCommandEvent& event );
  void OnPopUpDrawModeSemanticAbsoluteMaximum( wxCommandEvent& event );
  void OnPopUpDrawModeSemanticAbsoluteMinimumNotZero( wxCommandEvent& event );
  void OnPopUpDrawModeSemanticAverage( wxCommandEvent& event );
  void OnPopUpDrawModeSemanticAverageNotZero( wxCommandEvent& event );
  void OnPopUpDrawModeSemanticMode( wxCommandEvent& event );

  void OnPopUpDrawModeObjectsLast( wxCommandEvent& event );
  void OnPopUpDrawModeObjectsRandom( wxCommandEvent& event );
  void OnPopUpDrawModeObjectsRandomNotZero( wxCommandEvent& event );
  void OnPopUpDrawModeObjectsMaximum( wxCommandEvent& event );
  void OnPopUpDrawModeObjectsMinimumNotZero( wxCommandEvent& event );
  void OnPopUpDrawModeObjectsAbsoluteMaximum( wxCommandEvent& event );
  void OnPopUpDrawModeObjectsAbsoluteMinimumNotZero( wxCommandEvent& event );
  void OnPopUpDrawModeObjectsAverage( wxCommandEvent& event );
  void OnPopUpDrawModeObjectsAverageNotZero( wxCommandEvent& event );
  void OnPopUpDrawModeObjectsMode( wxCommandEvent& event );

  void OnPopUpDrawModeBothLast( wxCommandEvent& event );
  void OnPopUpDrawModeBothRandom( wxCommandEvent& event );
  void OnPopUpDrawModeBothRandomNotZero( wxCommandEvent& event );
  void OnPopUpDrawModeBothMaximum( wxCommandEvent& event );
  void OnPopUpDrawModeBothMinimumNotZero( wxCommandEvent& event );
  void OnPopUpDrawModeBothAbsoluteMaximum( wxCommandEvent& event );
  void OnPopUpDrawModeBothAbsoluteMinimumNotZero( wxCommandEvent& event );
  void OnPopUpDrawModeBothAverage( wxCommandEvent& event );
  void OnPopUpDrawModeBothAverageNotZero( wxCommandEvent& event );
  void OnPopUpDrawModeBothMode( wxCommandEvent& event );

  void OnPopUpPixelSize( wxCommandEvent& event );
  
  void OnPopUpSynchronize( wxCommandEvent& event );
  void OnPopUpRemoveGroup( wxCommandEvent& event );
  void OnPopUpRemoveAllGroups( wxCommandEvent& event );

  void OnPopUpSavePlaneAsText( wxCommandEvent& event );
  
  void OnPopUpSaveCFG( wxCommandEvent& event );
  void OnPopUpSaveImageDialog( wxCommandEvent& event );
  
  void OnMenuGradientFunction( TGradientFunction function );
  
  void saveCFG();
  void saveImageDialog( wxString whichFileName = _( "" ) );
  void saveImage( wxString whichFileName = _( "" ), TImageFormat filterIndex =  TImageFormat::PNG );
  void saveText( bool onlySelectedPlane = false );

  void rightDownManager();

  static wxProgressDialog *dialogProgress;

  std::vector< TObjectOrder > getSelectedRows();
  virtual void setSelectedRows( std::vector< bool > &selected );
  virtual void setSelectedRows( std::vector< TObjectOrder > &selected );
    
  void EnableCustomSortOption();

////@begin gHistogram member variables
  wxPanel* panelToolbar;
  wxToolBar* tbarHisto;
  wxChoice* choiceSortBy;
  wxPanel* panelData;
  wxBoxSizer* mainSizer;
  wxScrolledWindow* zoomHisto;
  wxGrid* gridHisto;
  wxBoxSizer* warningSizer;
  wxStaticBitmap* controlWarning;
  wxStaticBitmap* xtraWarning;
  wxStaticBitmap* autoRedrawIcon;
  wxStatusBar* histoStatus;
private:
  wxBitmap drawImage;
  bool escapePressed;
  double lastPosZoomX;
  double lastPosZoomY;
  Histogram* myHistogram;
  bool openControlActivated;
  bool ready;
  wxStopWatch * redrawStopWatch;
  std::vector<TObjectOrder> selectedRows;
  HistoTableBase* tableBase;
  wxTimer * timerZoom;
  double zoomCellHeight;
  double zoomCellWidth;
  bool zoomDragging;
  wxBitmap zoomImage;
  wxPoint zoomPointBegin;
  wxPoint zoomPointEnd;
////@end gHistogram member variables
  wxWindow *parent; // for clone

  SelectionManagement<THistogramColumn,int> columnSelection;

  // Returns: histogram_plane_with_spaces_underscored@traceName (without extension PRV)
  wxString buildFormattedFileName( bool onlySelectedPlane = true ) const;

  void updateHistogram();

  void OnTimerZoom( wxTimerEvent& event );
  TSemanticValue getZoomSemanticValue( THistogramColumn column, TObjectOrder row, const std::vector<THistogramColumn>& noVoidSemRanges ) const;

  THistogramColumn getSemanticSortedRealColumn( THistogramColumn whichCol, const std::vector<THistogramColumn>& noVoidSemRanges  ) const;
  void drawColumn( THistogramColumn beginColumn, THistogramColumn endColumn, 
                   std::vector<THistogramColumn>& selectedColumns, wxMemoryDC& bufferDraw );

  void openControlGetParameters( int xBegin, int xEnd, int yBegin, int yEnd,
                                 THistogramColumn& columnBegin, THistogramColumn& columnEnd,
                                 TObjectOrder& objectBegin, TObjectOrder& objectEnd, bool zoomxy  );
  void openControlMinMaxParam( THistogramColumn& columnBegin, THistogramColumn& columnEnd,
                               TParamValue& minParam, TParamValue& maxParam );
  void openControlWindow( THistogramColumn columnBegin, THistogramColumn columnEnd,
                          TObjectOrder objectBegin, TObjectOrder objectEnd );

  void zoom( THistogramLimit newColumnBegin, THistogramLimit newColumnEnd,
             TObjectOrder newObjectBegin, TObjectOrder newObjectEnd, THistogramLimit newDelta = -1.0 );

};

void progressFunctionHistogram( ProgressController *progress, void *callerWindow );
