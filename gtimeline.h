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

#ifndef _GTIMELINE_H_
#define _GTIMELINE_H_


/*!
 * Includes
 */
#include "prvtypes.h"

#ifdef WIN32
#include <hash_set>
#else
#include <ext/hash_set>
#endif

#ifdef WIN32
using namespace stdext;
#else
using namespace __gnu_cxx;
#endif

////@begin includes
#include "wx/frame.h"
#include "wx/splitter.h"
#include "wx/notebook.h"
#include "wx/richtext/richtextctrl.h"
////@end includes
#include <wx/treebase.h>

#include "wx/checkbox.h"
#include "wx/choicdlg.h"
#include "wx/dcmemory.h"
#include "wx/scrolwin.h"
#include "wx/timer.h"
#include "wx/icon.h"

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
class wxBoxSizer;
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
#define ID_SCROLLEDWINDOW2 10076
#define ID_CHECKBOX 10077
#define ID_CHECKBOX1 10079
#define ID_CHECKBOX2 10080
#define ID_CHECKBOX3 10083
#define ID_CHECKBOX4 10084
#define ID_RICHTEXTCTRL 10043
#define ID_PANEL 10044
#define ID_TEXTCTRL 10045
#define ID_TEXTCTRL1 10046
#define ID_TEXTCTRL2 10047
#define ID_PANEL1 10049
#define SYMBOL_GTIMELINE_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMAXIMIZE_BOX|wxCLOSE_BOX|wxFRAME_NO_TASKBAR|wxWANTS_CHARS|wxFULL_REPAINT_ON_RESIZE
#define SYMBOL_GTIMELINE_TITLE _("gTimeline")
#define SYMBOL_GTIMELINE_IDNAME ID_GTIMELINE
#define SYMBOL_GTIMELINE_SIZE wxSize(400, 300)
#define SYMBOL_GTIMELINE_POSITION wxDefaultPosition
////@end control identifiers

#define ID_TIMER_SIZE 40000
#define ID_TIMER_MOTION 40001

#ifdef WIN32
using namespace stdext;
#else
using namespace __gnu_cxx;
#endif

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

  /// wxEVT_LEFT_DCLICK event handler for ID_SCROLLEDWINDOW
  void OnScrolledWindowLeftDClick( wxMouseEvent& event );

  /// wxEVT_MIDDLE_UP event handler for ID_SCROLLEDWINDOW
  void OnScrolledWindowMiddleUp( wxMouseEvent& event );

  /// wxEVT_RIGHT_DOWN event handler for ID_SCROLLEDWINDOW
  void OnScrolledWindowRightDown( wxMouseEvent& event );

  /// wxEVT_MOTION event handler for ID_SCROLLEDWINDOW
  void OnScrolledWindowMotion( wxMouseEvent& event );

  /// wxEVT_KEY_DOWN event handler for ID_SCROLLEDWINDOW
  void OnScrolledWindowKeyDown( wxKeyEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_SCROLLEDWINDOW
  void OnScrolledWindowUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING event handler for ID_NOTEBOOK
  void OnNotebookPageChanging( wxNotebookEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX
  void OnCheckWhatWhere( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX4
  void OnCheckWhatWhereText( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_PANEL1
  void OnColorsPanelUpdate( wxUpdateUIEvent& event );

////@end gTimeline event handler declarations

////@begin gTimeline member function declarations

  wxBitmap GetBufferImage() const { return bufferImage ; }
  void SetBufferImage(wxBitmap value) { bufferImage = value ; }

  bool GetReady() const { return ready ; }
  void SetReady(bool value) { ready = value ; }

  Window* GetMyWindow() const { return myWindow ; }
  void SetMyWindow(Window* value) { myWindow = value ; }

  PRV_INT32 GetObjectAxisPos() const { return objectAxisPos ; }
  void SetObjectAxisPos(PRV_INT32 value) { objectAxisPos = value ; }

  PRV_INT32 GetTimeAxisPos() const { return timeAxisPos ; }
  void SetTimeAxisPos(PRV_INT32 value) { timeAxisPos = value ; }

  wxFont GetObjectFont() const { return objectFont ; }
  void SetObjectFont(wxFont value) { objectFont = value ; }

  wxFont GetTimeFont() const { return timeFont ; }
  void SetTimeFont(wxFont value) { timeFont = value ; }

  vector<PRV_INT32> GetObjectPosList() const { return objectPosList ; }
  void SetObjectPosList(vector<PRV_INT32> value) { objectPosList = value ; }

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

  bool GetRedoColors() const { return redoColors ; }
  void SetRedoColors(bool value) { redoColors = value ; }

  wxBitmap GetEventImage() const { return eventImage ; }
  void SetEventImage(wxBitmap value) { eventImage = value ; }

  bool GetDrawCaution() const { return drawCaution ; }
  void SetDrawCaution(bool value) { drawCaution = value ; }

  bool GetSplitChanged() const { return splitChanged ; }
  void SetSplitChanged(bool value) { splitChanged = value ; }

  wxTimer * GetTimerSize() const { return timerSize ; }
  void SetTimerSize(wxTimer * value) { timerSize = value ; }

  int GetInfoZoneLastSize() const { return infoZoneLastSize ; }
  void SetInfoZoneLastSize(int value) { infoZoneLastSize = value ; }

  wxColour GetForegroundColour() const { return foregroundColour ; }
  void SetForegroundColour(wxColour value) { foregroundColour = value ; }

  wxColour GetBackgroundColour() const { return backgroundColour ; }
  void SetBackgroundColour(wxColour value) { backgroundColour = value ; }

  wxColour GetLogicalColour() const { return logicalColour ; }
  void SetLogicalColour(wxColour value) { logicalColour = value ; }

  wxColour GetPhysicalColour() const { return physicalColour ; }
  void SetPhysicalColour(wxColour value) { physicalColour = value ; }

  bool GetEscapePressed() const { return escapePressed ; }
  void SetEscapePressed(bool value) { escapePressed = value ; }

  wxPen GetLogicalPen() const { return logicalPen ; }
  void SetLogicalPen(wxPen value) { logicalPen = value ; }

  wxPen GetPhysicalPen() const { return physicalPen ; }
  void SetPhysicalPen(wxPen value) { physicalPen = value ; }

  wxTimer * GetTimerMotion() const { return timerMotion ; }
  void SetTimerMotion(wxTimer * value) { timerMotion = value ; }

  wxFont GetSemanticFont() const { return semanticFont ; }
  void SetSemanticFont(wxFont value) { semanticFont = value ; }

  wxMouseEvent GetMotionEvent() const { return motionEvent ; }
  void SetMotionEvent(wxMouseEvent value) { motionEvent = value ; }

  wxMouseEvent GetFirstMotionEvent() const { return firstMotionEvent ; }
  void SetFirstMotionEvent(wxMouseEvent value) { firstMotionEvent = value ; }

  TRecordTime GetLastEventFoundTime() const { return lastEventFoundTime ; }
  void SetLastEventFoundTime(TRecordTime value) { lastEventFoundTime = value ; }

  TRecordTime GetLastSemanticFoundTime() const { return lastSemanticFoundTime ; }
  void SetLastSemanticFoundTime(TRecordTime value) { lastSemanticFoundTime = value ; }

  TRecordTime GetFindBeginTime() const { return findBeginTime ; }
  void SetFindBeginTime(TRecordTime value) { findBeginTime = value ; }

  TRecordTime GetFindEndTime() const { return findEndTime ; }
  void SetFindEndTime(TRecordTime value) { findEndTime = value ; }

  TObjectOrder GetFindFirstObject() const { return findFirstObject ; }
  void SetFindFirstObject(TObjectOrder value) { findFirstObject = value ; }

  TObjectOrder GetFindLastObject() const { return findLastObject ; }
  void SetFindLastObject(TObjectOrder value) { findLastObject = value ; }

  TObjectOrder GetLastFoundObject() const { return lastFoundObject ; }
  void SetLastFoundObject(TObjectOrder value) { lastFoundObject = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end gTimeline member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  void redraw();
  bool drawAxis( wxDC& dc, vector<TObjectOrder>& selected );
#ifdef WIN32
  void drawRow( wxDC& dc,
                TObjectOrder firstRow, TObjectOrder lastRow,
                vector<TObjectOrder>& selectedSet, vector<bool>& selected,
                vector< TSemanticValue >& valuesToDraw,              // I
                hash_set< PRV_INT32 >& eventsToDraw,                 // I
                hash_set< commCoord >& commsToDraw,                   // I
                wxMemoryDC& eventdc, wxMemoryDC& eventmaskdc,
                wxMemoryDC& commdc, wxMemoryDC& commmaskdc );
#else
  void drawRow( wxDC& dc,
                TObjectOrder firstRow, TObjectOrder lastRow,
                vector<TObjectOrder>& selectedSet, vector<bool>& selected,
                vector< TSemanticValue >& valuesToDraw,              // I
                hash_set< PRV_INT32 >& eventsToDraw,                 // I
                hash_set< commCoord, hashCommCoord >& commsToDraw,    // I
                wxMemoryDC& eventdc, wxMemoryDC& eventmaskdc,
                wxMemoryDC& commdc, wxMemoryDC& commmaskdc );
#endif

  void drawCommunicationLines( bool draw );
  void drawEventFlags( bool draw );
  void drawFunctionLineColor( bool draw );

  void OnPopUpRightDown( void );

  // Pop Up Menu Methods
  void OnPopUpCopy();
  void OnPopUpPasteDefaultSpecial();
  void OnPopUpPasteSpecial();
  void OnPopUpPasteTime();
  void OnPopUpPasteObjects();
  void OnPopUpPasteSize();
  void OnPopUpPasteDuration();
  void OnPopUpPasteSemanticScale();
  void OnPopUpPasteFilterAll();
  void OnPopUpPasteFilterCommunications();
  void OnPopUpPasteFilterEvents();
  void OnPopUpClone();
  void OnPopUpFitTimeScale();
  void OnPopUpFitSemanticScaleMin();
  void OnPopUpFitSemanticScaleMax();
  void OnPopUpFitSemanticScale();
  void OnPopUpFitObjects();
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

  void OnPopUpPixelSize( PRV_UINT16 whichPixelSize );

  void OnPopUpRowSelection();

  void OnPopUpInfoPanel();
  
  void OnMenuGradientFunction( GradientColor::TGradientFunction function );
  
  void OnFindDialog();

  gTimeline *clone( Window *clonedWindow,
                    wxWindow *parent,
                    wxTreeItemId idRoot1,
                    wxTreeItemId idRoot2,
                    bool mustRedraw = true );

  void rightDownManager();

  void resizeDrawZone( int width, int height );
  
  bool IsSplit() const;

  void saveImage();
  void saveText();

  typedef enum {  RAW_LINE = 0,
                  BEGIN_OBJECT_SECTION,
                  END_OBJECT_SECTION,
                  BEGIN_PREVNEXT_SECTION,
                  END_PREVNEXT_SECTION,
                  BEGIN_CURRENT_SECTION,
                  END_CURRENT_SECTION,
                  BEGIN_SEMANTIC_SECTION,
                  SEMANTIC_LINE,
                  END_SEMANTIC_SECTION,
                  BEGIN_RECORDS_SECTION,
                  MARK_LINE,
                  EVENT_LINE,
                  COMMUNICATION_LINE,
                  END_RECORDS_SECTION } TWWLine;

//  void OnRightClick(wxMouseEvent& event);
////@begin gTimeline member variables
  wxSplitterWindow* splitter;
  wxScrolledWindow* drawZone;
  wxNotebook* infoZone;
  wxScrolledWindow* whatWherePanel;
  wxCheckBox* checkWWSemantic;
  wxCheckBox* checkWWEvents;
  wxCheckBox* checkWWCommunications;
  wxCheckBox* checkWWPreviousNext;
  wxCheckBox* checkWWText;
  wxRichTextCtrl* whatWhereText;
  wxScrolledWindow* timingZone;
  wxTextCtrl* initialTimeText;
  wxTextCtrl* finalTimeText;
  wxTextCtrl* durationText;
  wxScrolledWindow* colorsPanel;
  wxBoxSizer* colorsSizer;
  wxBitmap bufferImage;
  wxBitmap drawImage;
  wxBitmap commImage;
  wxBitmap eventImage;
  bool drawCaution;
  wxColour physicalColour;
private:
  bool ready;
  Window* myWindow;
  PRV_INT32 objectAxisPos;
  PRV_INT32 timeAxisPos;
  wxFont objectFont;
  wxFont timeFont;
  vector<PRV_INT32> objectPosList;
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
  bool redoColors;
  bool splitChanged;
  wxTimer * timerSize;
  int infoZoneLastSize;
  wxColour foregroundColour;
  wxColour backgroundColour;
  wxColour logicalColour;
  bool escapePressed;
  wxPen logicalPen;
  wxPen physicalPen;
  wxTimer * timerMotion;
  wxFont semanticFont;
  wxMouseEvent motionEvent;
  wxMouseEvent firstMotionEvent;
  TRecordTime lastEventFoundTime;
  TRecordTime lastSemanticFoundTime;
  TRecordTime findBeginTime;
  TRecordTime findEndTime;
  TObjectOrder findFirstObject;
  TObjectOrder findLastObject;
  TObjectOrder lastFoundObject;
////@end gTimeline member variables

  SemanticInfoType lastType;
  TSemanticValue lastMin;
  TSemanticValue lastMax;
  bool codeColorSet;
  GradientColor::TGradientFunction gradientFunc;

  wxWindow *parent;
  
  static const wxCoord drawBorder = 5;

  vector< pair< TWWLine, wxString > > whatWhereLines;
  int whatWhereSelectedTimeEventLines;
  int whatWhereSelectedTimeCommunicationLines;
  TRecordTime whatWhereTime;
  TObjectOrder whatWhereRow;

  void computeWhatWhere( TRecordTime whichTime, TObjectOrder whichRow, bool textMode );
  void printWhatWhere( );
  void printWWSemantic( TObjectOrder whichRow, bool clickedValue, bool textMode );
  void printWWRecords( TObjectOrder whichRow, bool clickedValue, bool textMode );

  void Unsplit();
  void Split();
  void OnTimerSize( wxTimerEvent& event );
  void OnTimerMotion( wxTimerEvent& event );
};

#endif  // _GTIMELINE_H_
