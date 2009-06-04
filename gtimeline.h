/////////////////////////////////////////////////////////////////////////////
// Name:        gtimeline.h
// Purpose:     
// Author:      Eloy Martinez
// Modified by: 
// Created:     Wed 27 Aug 2008 11:26:03 CEST
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _GTIMELINE_H_
#define _GTIMELINE_H_


/*!
 * Includes
 */
////@begin includes
#include "wx/frame.h"
////@end includes
#include <wx/treebase.h>

#include "wx/choicdlg.h"
#include "wx/dcmemory.h"
#include "wx/scrolwin.h"

#include "paraverkerneltypes.h"
#include "recordlist.h"
#include "popupmenu.h"
#include "copypaste.h"

// TEMPLATES
#include "zoomhistory.h"
/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations
class Window;
/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_GTIMELINE 10001
#define ID_SCROLLEDWINDOW 10007
#define SYMBOL_GTIMELINE_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMAXIMIZE_BOX|wxCLOSE_BOX|wxFRAME_NO_TASKBAR|wxFULL_REPAINT_ON_RESIZE
#define SYMBOL_GTIMELINE_TITLE _("gTimeline")
#define SYMBOL_GTIMELINE_IDNAME ID_GTIMELINE
#define SYMBOL_GTIMELINE_SIZE wxSize(400, 300)
#define SYMBOL_GTIMELINE_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * gTimeline class declaration
 */

class gTimeline: public wxFrame
{    
  DECLARE_CLASS( gTimeline )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  gTimeline();
  gTimeline( wxWindow* parent, wxWindowID id = SYMBOL_GTIMELINE_IDNAME, const wxString& caption = SYMBOL_GTIMELINE_TITLE, const wxPoint& pos = SYMBOL_GTIMELINE_POSITION, const wxSize& size = SYMBOL_GTIMELINE_SIZE, long style = SYMBOL_GTIMELINE_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_GTIMELINE_IDNAME, const wxString& caption = SYMBOL_GTIMELINE_TITLE, const wxPoint& pos = SYMBOL_GTIMELINE_POSITION, const wxSize& size = SYMBOL_GTIMELINE_SIZE, long style = SYMBOL_GTIMELINE_STYLE );

  /// Destructor
  ~gTimeline();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin gTimeline event handler declarations

  /// wxEVT_CLOSE_WINDOW event handler for ID_GTIMELINE
  void OnCloseWindow( wxCloseEvent& event );

  /// wxEVT_SIZE event handler for ID_GTIMELINE
  void OnSize( wxSizeEvent& event );

  /// wxEVT_IDLE event handler for ID_GTIMELINE
  void OnIdle( wxIdleEvent& event );

  /// wxEVT_RIGHT_DOWN event handler for ID_GTIMELINE
  void OnRightDown( wxMouseEvent& event );

  /// wxEVT_PAINT event handler for ID_SCROLLEDWINDOW
  void OnPaint( wxPaintEvent& event );

  /// wxEVT_ERASE_BACKGROUND event handler for ID_SCROLLEDWINDOW
  void OnEraseBackground( wxEraseEvent& event );

  /// wxEVT_LEFT_DOWN event handler for ID_SCROLLEDWINDOW
  void OnLeftDown( wxMouseEvent& event );

  /// wxEVT_LEFT_UP event handler for ID_SCROLLEDWINDOW
  void OnLeftUp( wxMouseEvent& event );

  /// wxEVT_MOTION event handler for ID_SCROLLEDWINDOW
  void OnMotion( wxMouseEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_SCROLLEDWINDOW
  void OnScrolledwindowUpdate( wxUpdateUIEvent& event );

////@end gTimeline event handler declarations

////@begin gTimeline member function declarations

  wxBitmap GetBufferImage() const { return bufferImage ; }
  void SetBufferImage(wxBitmap value) { bufferImage = value ; }

  bool GetReady() const { return ready ; }
  void SetReady(bool value) { ready = value ; }

  Window* GetMyWindow() const { return myWindow ; }
  void SetMyWindow(Window* value) { myWindow = value ; }

  wxCoord GetObjectAxisPos() const { return objectAxisPos ; }
  void SetObjectAxisPos(wxCoord value) { objectAxisPos = value ; }

  wxCoord GetTimeAxisPos() const { return timeAxisPos ; }
  void SetTimeAxisPos(wxCoord value) { timeAxisPos = value ; }

  wxFont GetObjectFont() const { return objectFont ; }
  void SetObjectFont(wxFont value) { objectFont = value ; }

  wxFont GetTimeFont() const { return timeFont ; }
  void SetTimeFont(wxFont value) { timeFont = value ; }

  vector<wxCoord> GetObjectPosList() const { return objectPosList ; }
  void SetObjectPosList(vector<wxCoord> value) { objectPosList = value ; }

  int GetObjectHeight() const { return objectHeight ; }
  void SetObjectHeight(int value) { objectHeight = value ; }

  bool GetZooming() const { return zooming ; }
  void SetZooming(bool value) { zooming = value ; }

  long GetZoomBegin() const { return zoomBeginX ; }
  void SetZoomBegin(long value) { zoomBeginX = value ; }

  long GetZoomEnd() const { return zoomEndX ; }
  void SetZoomEnd(long value) { zoomEndX = value ; }

  wxBitmap GetDrawImage() const { return drawImage ; }
  void SetDrawImage(wxBitmap value) { drawImage = value ; }

  wxBitmap GetCommImage() const { return commImage ; }
  void SetCommImage(wxBitmap value) { commImage = value ; }

  bool GetZoomXY() const { return zoomXY ; }
  void SetZoomXY(bool value) { zoomXY = value ; }

  long GetZoomBeginY() const { return zoomBeginY ; }
  void SetZoomBeginY(long value) { zoomBeginY = value ; }

  long GetBeginRow() const { return beginRow ; }
  void SetBeginRow(long value) { beginRow = value ; }

  long GetEndRow() const { return endRow ; }
  void SetEndRow(long value) { endRow = value ; }

  long GetZoomEndY() const { return zoomEndY ; }
  void SetZoomEndY(long value) { zoomEndY = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end gTimeline member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  void redraw();
  void drawAxis( wxDC& dc );
  void drawRow( wxDC& dc, wxMemoryDC& commdc, wxDC& maskdc, TObjectOrder firstRow, TObjectOrder lastRow );
  void drawComm( wxMemoryDC& commdc, wxDC& maskdc, RecordList *comms,
                 TTime from, TTime to, TTime step, wxCoord pos );

  void OnPopUpRightDown( void );

  // Pop Up Menu Methods
  void OnPopUpCopy();
  void OnPopUpPasteSpecial();
  void OnPopUpPasteTime();
  void OnPopUpPasteObjects();
  void OnPopUpPasteSize();
  void OnPopUpPasteFilterAll();
  void OnPopUpPasteFilterCommunications();
  void OnPopUpPasteFilterEvents();
  void OnPopUpClone();
  void OnPopUpFitTimeScale();
  void OnPopUpFitSemanticScale();
  void OnPopUpCodeColor();
  void OnPopUpGradientColor();
  void OnPopUpNotNullGradientColor();
  void OnPopUpUndoZoom();
  void OnPopUpRedoZoom();
  
  void OnPopUpDrawModeTimeLast();
  void OnPopUpDrawModeTimeRandom();
  void OnPopUpDrawModeTimeRandomNotZero();
  void OnPopUpDrawModeTimeMaximum();
  void OnPopUpDrawModeTimeMinimumNotZero();
  void OnPopUpDrawModeTimeAverage();

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

  void OnPopUpRowSelection();

  gTimeline *clone( Window *clonedWindow,
                    wxWindow *parent,
                    wxTreeItemId idRoot1, 
                    wxTreeItemId idRoot2 );

  void rightDownManager();

//  void OnRightClick(wxMouseEvent& event);
////@begin gTimeline member variables
  wxScrolledWindow* drawZone;
  wxBitmap bufferImage;
  wxBitmap drawImage;
  wxBitmap commImage;
private:
  bool ready;
  Window* myWindow;
  wxCoord objectAxisPos;
  wxCoord timeAxisPos;
  wxFont objectFont;
  wxFont timeFont;
  vector<wxCoord> objectPosList;
  int objectHeight;
  bool zooming;
  long zoomBeginX;
  long zoomEndX;
  bool zoomXY;
  long zoomBeginY;
  long beginRow;
  long endRow;
  long zoomEndY;
////@end gTimeline member variables

  wxWindow *parent;
  ZoomHistory<TTime, TObjectOrder> *zoomHistory;
  
  static const wxCoord drawBorder = 5;
};

#endif  // _GTIMELINE_H_
