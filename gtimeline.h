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
#include "wx/splitter.h"
#include "wx/notebook.h"
#include "wx/richtext/richtextctrl.h"
////@end includes
#include <wx/treebase.h>

#include "wx/choicdlg.h"
#include "wx/dcmemory.h"
#include "wx/scrolwin.h"

#include "paraverkerneltypes.h"
#include "recordlist.h"
#include "popupmenu.h"
#include "copypaste.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxSplitterWindow;
class wxNotebook;
class wxRichTextCtrl;
////@end forward declarations
class Window;
/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_GTIMELINE 10001
#define ID_SPLITTERWINDOW 10048
#define ID_SCROLLEDWINDOW 10007
#define ID_NOTEBOOK 10042
#define ID_RICHTEXTCTRL 10043
#define ID_PANEL 10044
#define ID_TEXTCTRL 10045
#define ID_TEXTCTRL1 10046
#define ID_TEXTCTRL2 10047
#define ID_PANEL1 10049
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

  /// wxEVT_IDLE event handler for ID_GTIMELINE
  void OnIdle( wxIdleEvent& event );

  /// wxEVT_RIGHT_DOWN event handler for ID_GTIMELINE
  void OnRightDown( wxMouseEvent& event );

  /// wxEVT_COMMAND_SPLITTER_DOUBLECLICKED event handler for ID_SPLITTERWINDOW
  void OnSplitterwindowSashDClick( wxSplitterEvent& event );

  /// wxEVT_COMMAND_SPLITTER_UNSPLIT event handler for ID_SPLITTERWINDOW
  void OnSplitterwindowSashUnsplit( wxSplitterEvent& event );

  /// wxEVT_SIZE event handler for ID_SCROLLEDWINDOW
  void OnScrolledWindowSize( wxSizeEvent& event );

  /// wxEVT_PAINT event handler for ID_SCROLLEDWINDOW
  void OnScrolledWindowPaint( wxPaintEvent& event );

  /// wxEVT_ERASE_BACKGROUND event handler for ID_SCROLLEDWINDOW
  void OnScrolledWindowEraseBackground( wxEraseEvent& event );

  /// wxEVT_LEFT_DOWN event handler for ID_SCROLLEDWINDOW
  void OnScrolledWindowLeftDown( wxMouseEvent& event );

  /// wxEVT_LEFT_UP event handler for ID_SCROLLEDWINDOW
  void OnScrolledWindowLeftUp( wxMouseEvent& event );

  /// wxEVT_RIGHT_DOWN event handler for ID_SCROLLEDWINDOW
  void OnScrolledWindowRightDown( wxMouseEvent& event );

  /// wxEVT_MOTION event handler for ID_SCROLLEDWINDOW
  void OnScrolledWindowMotion( wxMouseEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_SCROLLEDWINDOW
  void OnScrolledWindowUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING event handler for ID_NOTEBOOK
  void OnNotebookPageChanging( wxNotebookEvent& event );

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

  bool GetCanRedraw() const { return canRedraw ; }
  void SetCanRedraw(bool value) { canRedraw = value ; }

  bool GetFirstUnsplit() const { return firstUnsplit ; }
  void SetFirstUnsplit(bool value) { firstUnsplit = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end gTimeline member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  void redraw();
  void drawAxis( wxDC& dc, vector<TObjectOrder>& selected );
  void drawRow( wxDC& dc, wxMemoryDC& commdc, wxDC& maskdc, TObjectOrder firstRow, TObjectOrder lastRow,
                vector<TObjectOrder>& selectedSet, vector<bool>& selected );
  void drawComm( wxMemoryDC& commdc, wxDC& maskdc, RecordList *comms,
                 TTime from, TTime to, TTime step, wxCoord pos, vector<bool>& selected );

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

  void OnPopUpInfoPanel();
  
  gTimeline *clone( Window *clonedWindow,
                    wxWindow *parent,
                    wxTreeItemId idRoot1,
                    wxTreeItemId idRoot2,
                    bool mustRedraw = true );

  void rightDownManager();

  void resizeDrawZone( int width, int height );
  
  bool IsSplit() const;
  
//  void OnRightClick(wxMouseEvent& event);
////@begin gTimeline member variables
  wxSplitterWindow* splitter;
  wxScrolledWindow* drawZone;
  wxNotebook* infoZone;
  wxRichTextCtrl* whatWhereText;
  wxPanel* timingZone;
  wxTextCtrl* initialTimeText;
  wxTextCtrl* finalTimeText;
  wxTextCtrl* durationText;
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
  bool canRedraw;
  bool firstUnsplit;
////@end gTimeline member variables

  wxWindow *parent;
  
  static const wxCoord drawBorder = 5;
  
  void printWhatWhere( TRecordTime whichTime, TObjectOrder whichRow );
  void printWWSemantic( wxString& onString, TObjectOrder whichRow, bool clickedValue );
  void printWWRecords( wxString& onString, TObjectOrder whichRow );
  
  void Unsplit();
  void Split();
};

#endif  // _GTIMELINE_H_
