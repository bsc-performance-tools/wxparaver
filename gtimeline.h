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
#include <wx/progdlg.h>
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
class ProgressController;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_GTIMELINE 10001
#define ID_SPLITTER_TIMELINE 10048
#define ID_SCROLLED_DRAW 10007
#define ID_NOTEBOOK_INFO 10042
#define ID_SCROLLED_WHATWHERE 10076
#define ID_CHECKBOX 10077
#define ID_CHECKBOX1 10079
#define ID_CHECKBOX2 10080
#define ID_CHECKBOX3 10083
#define ID_CHECKBOX4 10084
#define ID_RICHTEXTCTRL 10043
#define ID_SCROLLED_TIMING 10044
#define ID_TEXTCTRL 10045
#define ID_TEXTCTRL1 10046
#define ID_TEXTCTRL2 10047
#define ID_SCROLLED_COLORS 10049
#define SYMBOL_GTIMELINE_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMAXIMIZE_BOX|wxCLOSE_BOX|wxFRAME_NO_TASKBAR|wxWANTS_CHARS|wxFULL_REPAINT_ON_RESIZE
#define SYMBOL_GTIMELINE_TITLE _("gTimeline")
#define SYMBOL_GTIMELINE_IDNAME ID_GTIMELINE
#define SYMBOL_GTIMELINE_SIZE wxSize(400, 300)
#define SYMBOL_GTIMELINE_POSITION wxDefaultPosition
////@end control identifiers

#define ID_TIMER_SIZE 40000
#define ID_TIMER_MOTION 40001

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

  /// wxEVT_COMMAND_SPLITTER_DOUBLECLICKED event handler for ID_SPLITTER_TIMELINE
  void OnSplitterTimelineSashDClick( wxSplitterEvent& event );

  /// wxEVT_COMMAND_SPLITTER_UNSPLIT event handler for ID_SPLITTER_TIMELINE
  void OnSplitterTimelineSashUnsplit( wxSplitterEvent& event );

  /// wxEVT_SIZE event handler for ID_SCROLLED_DRAW
  void OnScrolledWindowSize( wxSizeEvent& event );

  /// wxEVT_PAINT event handler for ID_SCROLLED_DRAW
  void OnScrolledWindowPaint( wxPaintEvent& event );

  /// wxEVT_LEFT_DCLICK event handler for ID_SCROLLED_DRAW
  void OnScrolledWindowLeftDClick( wxMouseEvent& event );

  /// wxEVT_MIDDLE_UP event handler for ID_SCROLLED_DRAW
  void OnScrolledWindowMiddleUp( wxMouseEvent& event );

  /// wxEVT_RIGHT_DOWN event handler for ID_SCROLLED_DRAW
  void OnScrolledWindowRightDown( wxMouseEvent& event );

  /// wxEVT_MOTION event handler for ID_SCROLLED_DRAW
  void OnScrolledWindowMotion( wxMouseEvent& event );

  /// wxEVT_KEY_DOWN event handler for ID_SCROLLED_DRAW
  void OnScrolledWindowKeyDown( wxKeyEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_SCROLLED_DRAW
  void OnScrolledWindowUpdate( wxUpdateUIEvent& event );

  /// wxEVT_ERASE_BACKGROUND event handler for ID_SCROLLED_DRAW
  void OnScrolledWindowEraseBackground( wxEraseEvent& event );

  /// wxEVT_LEFT_DOWN event handler for ID_SCROLLED_DRAW
  void OnScrolledWindowLeftDown( wxMouseEvent& event );

  /// wxEVT_LEFT_UP event handler for ID_SCROLLED_DRAW
  void OnScrolledWindowLeftUp( wxMouseEvent& event );

  /// wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING event handler for ID_NOTEBOOK_INFO
  void OnNotebookInfoPageChanging( wxNotebookEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX
  void OnCheckWhatWhere( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX4
  void OnCheckWhatWhereText( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_SCROLLED_COLORS
  void OnColorsPanelUpdate( wxUpdateUIEvent& event );

////@end gTimeline event handler declarations

////@begin gTimeline member function declarations

  wxColour GetBackgroundColour() const { return backgroundColour ; }
  void SetBackgroundColour(wxColour value) { backgroundColour = value ; }

  long GetBeginRow() const { return beginRow ; }
  void SetBeginRow(long value) { beginRow = value ; }

  wxBitmap GetBufferImage() const { return bufferImage ; }
  void SetBufferImage(wxBitmap value) { bufferImage = value ; }

  bool GetCanRedraw() const { return canRedraw ; }
  void SetCanRedraw(bool value) { canRedraw = value ; }

  wxBitmap GetCommImage() const { return commImage ; }
  void SetCommImage(wxBitmap value) { commImage = value ; }

  bool GetDrawCaution() const { return drawCaution ; }
  void SetDrawCaution(bool value) { drawCaution = value ; }

  wxBitmap GetDrawImage() const { return drawImage ; }
  void SetDrawImage(wxBitmap value) { drawImage = value ; }

  long GetEndRow() const { return endRow ; }
  void SetEndRow(long value) { endRow = value ; }

  bool GetEscapePressed() const { return escapePressed ; }
  void SetEscapePressed(bool value) { escapePressed = value ; }

  wxBitmap GetEventImage() const { return eventImage ; }
  void SetEventImage(wxBitmap value) { eventImage = value ; }

  TRecordTime GetFindBeginTime() const { return findBeginTime ; }
  void SetFindBeginTime(TRecordTime value) { findBeginTime = value ; }

  TRecordTime GetFindEndTime() const { return findEndTime ; }
  void SetFindEndTime(TRecordTime value) { findEndTime = value ; }

  TObjectOrder GetFindFirstObject() const { return findFirstObject ; }
  void SetFindFirstObject(TObjectOrder value) { findFirstObject = value ; }

  TObjectOrder GetFindLastObject() const { return findLastObject ; }
  void SetFindLastObject(TObjectOrder value) { findLastObject = value ; }

  wxMouseEvent GetFirstMotionEvent() const { return firstMotionEvent ; }
  void SetFirstMotionEvent(wxMouseEvent value) { firstMotionEvent = value ; }

  bool GetFirstUnsplit() const { return firstUnsplit ; }
  void SetFirstUnsplit(bool value) { firstUnsplit = value ; }

  wxColour GetForegroundColour() const { return foregroundColour ; }
  void SetForegroundColour(wxColour value) { foregroundColour = value ; }

  int GetInfoZoneLastSize() const { return infoZoneLastSize ; }
  void SetInfoZoneLastSize(int value) { infoZoneLastSize = value ; }

  TRecordTime GetLastEventFoundTime() const { return lastEventFoundTime ; }
  void SetLastEventFoundTime(TRecordTime value) { lastEventFoundTime = value ; }

  TObjectOrder GetLastFoundObject() const { return lastFoundObject ; }
  void SetLastFoundObject(TObjectOrder value) { lastFoundObject = value ; }

  TRecordTime GetLastSemanticFoundTime() const { return lastSemanticFoundTime ; }
  void SetLastSemanticFoundTime(TRecordTime value) { lastSemanticFoundTime = value ; }

  wxColour GetLogicalColour() const { return logicalColour ; }
  void SetLogicalColour(wxColour value) { logicalColour = value ; }

  wxPen GetLogicalPen() const { return logicalPen ; }
  void SetLogicalPen(wxPen value) { logicalPen = value ; }

  wxMouseEvent GetMotionEvent() const { return motionEvent ; }
  void SetMotionEvent(wxMouseEvent value) { motionEvent = value ; }

  Window* GetMyWindow() const { return myWindow ; }
  void SetMyWindow(Window* value) { myWindow = value ; }

  PRV_INT32 GetObjectAxisPos() const { return objectAxisPos ; }
  void SetObjectAxisPos(PRV_INT32 value) { objectAxisPos = value ; }

  wxFont GetObjectFont() const { return objectFont ; }
  void SetObjectFont(wxFont value) { objectFont = value ; }

  int GetObjectHeight() const { return objectHeight ; }
  void SetObjectHeight(int value) { objectHeight = value ; }

  vector<PRV_INT32> GetObjectPosList() const { return objectPosList ; }
  void SetObjectPosList(vector<PRV_INT32> value) { objectPosList = value ; }

  wxColour GetPhysicalColour() const { return physicalColour ; }
  void SetPhysicalColour(wxColour value) { physicalColour = value ; }

  wxPen GetPhysicalPen() const { return physicalPen ; }
  void SetPhysicalPen(wxPen value) { physicalPen = value ; }

  bool GetReady() const { return ready ; }
  void SetReady(bool value) { ready = value ; }

  bool GetRedoColors() const { return redoColors ; }
  void SetRedoColors(bool value) { redoColors = value ; }

  wxStopWatch * GetRedrawStopWatch() const { return redrawStopWatch ; }
  void SetRedrawStopWatch(wxStopWatch * value) { redrawStopWatch = value ; }

  wxFont GetSemanticFont() const { return semanticFont ; }
  void SetSemanticFont(wxFont value) { semanticFont = value ; }

  bool GetSplitChanged() const { return splitChanged ; }
  void SetSplitChanged(bool value) { splitChanged = value ; }

  PRV_INT32 GetTimeAxisPos() const { return timeAxisPos ; }
  void SetTimeAxisPos(PRV_INT32 value) { timeAxisPos = value ; }

  wxFont GetTimeFont() const { return timeFont ; }
  void SetTimeFont(wxFont value) { timeFont = value ; }

  wxTimer * GetTimerMotion() const { return timerMotion ; }
  void SetTimerMotion(wxTimer * value) { timerMotion = value ; }

  wxTimer * GetTimerSize() const { return timerSize ; }
  void SetTimerSize(wxTimer * value) { timerSize = value ; }

  long GetZoomBegin() const { return zoomBeginX ; }
  void SetZoomBegin(long value) { zoomBeginX = value ; }

  long GetZoomBeginY() const { return zoomBeginY ; }
  void SetZoomBeginY(long value) { zoomBeginY = value ; }

  long GetZoomEnd() const { return zoomEndX ; }
  void SetZoomEnd(long value) { zoomEndX = value ; }

  long GetZoomEndY() const { return zoomEndY ; }
  void SetZoomEndY(long value) { zoomEndY = value ; }

  bool GetZoomXY() const { return zoomXY ; }
  void SetZoomXY(bool value) { zoomXY = value ; }

  bool GetZooming() const { return zooming ; }
  void SetZooming(bool value) { zooming = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end gTimeline member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  std::vector< TObjectOrder > getCurrentZoomRange();

#ifdef __WXMAC__
  void drawStackedImages( wxDC& dc );
#endif
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
  void drawFunctionLineColor();

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
  void OnPopUpDrawModeTimeAverageNotZero();
  void OnPopUpDrawModeTimeMode();

  void OnPopUpDrawModeObjectsLast();
  void OnPopUpDrawModeObjectsRandom();
  void OnPopUpDrawModeObjectsRandomNotZero();
  void OnPopUpDrawModeObjectsMaximum();
  void OnPopUpDrawModeObjectsMinimumNotZero();
  void OnPopUpDrawModeObjectsAverage();
  void OnPopUpDrawModeObjectsAverageNotZero();
  void OnPopUpDrawModeObjectsMode();

  void OnPopUpDrawModeBothLast();
  void OnPopUpDrawModeBothRandom();
  void OnPopUpDrawModeBothRandomNotZero();
  void OnPopUpDrawModeBothMaximum();
  void OnPopUpDrawModeBothMinimumNotZero();
  void OnPopUpDrawModeBothAverage();
  void OnPopUpDrawModeBothAverageNotZero();
  void OnPopUpDrawModeBothMode();

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

  void saveImage( bool showSaveDialog = true );
  void saveImageLegend( bool showSaveDialog = true );

  void saveText();
  void saveCFG();

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

  static wxProgressDialog *dialogProgress;
  
  
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
  wxBitmap commImage;
  bool drawCaution;
  wxBitmap drawImage;
  wxBitmap eventImage;
  wxColour physicalColour;
private:
  wxColour backgroundColour;
  long beginRow;
  bool canRedraw;
  long endRow;
  bool escapePressed;
  TRecordTime findBeginTime;
  TRecordTime findEndTime;
  TObjectOrder findFirstObject;
  TObjectOrder findLastObject;
  wxMouseEvent firstMotionEvent;
  bool firstUnsplit;
  wxColour foregroundColour;
  int infoZoneLastSize;
  TRecordTime lastEventFoundTime;
  TObjectOrder lastFoundObject;
  TRecordTime lastSemanticFoundTime;
  wxColour logicalColour;
  wxPen logicalPen;
  wxMouseEvent motionEvent;
  Window* myWindow;
  PRV_INT32 objectAxisPos;
  wxFont objectFont;
  int objectHeight;
  vector<PRV_INT32> objectPosList;
  wxPen physicalPen;
  bool ready;
  bool redoColors;
  wxStopWatch * redrawStopWatch;
  wxFont semanticFont;
  bool splitChanged;
  PRV_INT32 timeAxisPos;
  wxFont timeFont;
  wxTimer * timerMotion;
  wxTimer * timerSize;
  long zoomBeginX;
  long zoomBeginY;
  long zoomEndX;
  long zoomEndY;
  bool zoomXY;
  bool zooming;
////@end gTimeline member variables

  SemanticInfoType lastType;
  TSemanticValue lastMin;
  TSemanticValue lastMax;
  bool codeColorSet;
  GradientColor::TGradientFunction gradientFunc;
#ifdef __WXMAC__
  wxBitmap zoomBMP;
#endif

  wxWindow *parent;
  std::map< TSemanticValue, rgb > semanticValues; // Stored for SaveImage scale
  
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

  // Returns: window_name_with_spaces_underscored@traceName (without extension PRV)
  wxString buildFormattedFileName() const;

  void Unsplit();
  void Split();
  void OnTimerSize( wxTimerEvent& event );
  void OnTimerMotion( wxTimerEvent& event );                    

  // Called by saveLabelsImage
  void drawRectangle( wxMemoryDC& labelDC,
                       wxMemoryDC& scaleDC,
                       wxColour foregroundColour,
                       wxColour backgroundColour,
                       rgb semanticColour,
                       wxString semanticValueLabel,
                       int titleMargin,
                       int widthRect,
                       int heightRect,
                       bool tryHorizontal,
                       int& xdst,
                       int& ydst,
                       int xsrc,
                       int ysrc,
                       int imageWidth,
                       int imageHeight,
                       int imageStepY,
                       int imageStepXRectangle,
                       bool drawLabel );

  class ScaleImageVertical
  {
    public:
      ScaleImageVertical(
                  Window* whichMyWindow, 
                  const std::map< TSemanticValue, rgb >& whichSemanticValues,
                  wxColour whichBackground,
                  wxColour whichForeground,
                  int whichBackgroundMode, // wxSOLID or wxTRANSPARENT; wxTRANSPARENT overrides fore and back
                  wxFont whichTextFont,
                  wxString& whichImagePath,
                  const wxString& whichImageInfix,
#if wxMAJOR_VERSION<3
                  long whichImageType );
#else
                  wxBitmapType& whichImageType ); 
#endif
                  
      ~ScaleImageVertical();
      
      void save();
      
    protected:
      virtual void init();
      virtual void sortSemanticValues();
      virtual void computeMaxLabelSize();
      virtual void computeImageSize();
      virtual void createDC();
      virtual void invertMask( wxDC *dcMask );      
      virtual void draw();
      virtual void drawLabeledRectangle( rgb semanticColour,
                                           wxString semanticValueLabel,
                                           bool drawIt = true );
      void destroyDC();

      Window *myWindow;
      std::map< TSemanticValue, rgb > semValues;
      wxColour background;
      wxColour foreground;
      int backgroundMode;
      wxFont textFont;
      wxString& imagePath;
      wxString imageInfix;
#if wxMAJOR_VERSION<3
      long imageType;
#else
      wxBitmapType& imageType;
#endif

      ParaverConfig::TImageFormat filterIndex;
      wxString tmpSuffix;
      TSemanticValue currentMin;
      TSemanticValue currentMax;
      std::vector< TSemanticValue > keys;
      PRV_UINT32 precision;
      wxString extraPrefixOutlier;
      std::map< TSemanticValue, wxString > semanticValueLabel;
      TSemanticValue semanticValueWithLongestLabel;
      int titleMargin;
      int widthRect;
      int heightRect;
      int imageStepY;
      int imageStepXRectangle;
      bool drawOutliers;
      bool symbolicDesc;
      int imageWidth;
      int imageHeight;
      int xsrc;
      int ysrc;
      int xdst;
      int ydst;
      wxBitmap *scaleBitmap;
      wxMemoryDC *scaleDC;
      wxBitmap *scaleMaskBitmap;
      wxMemoryDC *scaleMaskDC;
      wxSize maxLabelSize;
      
    private:
  };

  class ScaleImageVerticalCodeColor : public ScaleImageVertical
  {
    public:
      ScaleImageVerticalCodeColor(
              Window* whichMyWindow,
              const std::map< TSemanticValue, rgb >& whichSemanticValues,
              wxColour whichBackground,
              wxColour whichForeground,
              int whichBackgroundMode,
              wxFont whichTextFont,
              wxString& whichImagePath,
              const wxString& whichImageInfix,
#if wxMAJOR_VERSION<3
              long whichImageType );
#else
              wxBitmapType& whichImageType );
#endif

      ~ScaleImageVerticalCodeColor()
      {}

    protected:
      virtual void init();
    
    private:    
  };

  class ScaleImageVerticalGradientColor : public ScaleImageVertical
  {
    public:
      ScaleImageVerticalGradientColor(
              Window* whichMyWindow,
              const std::map< TSemanticValue, rgb >& whichSemanticValues,
              wxColour whichBackground,
              wxColour whichForeground,
              int whichBackgroundMode,
              wxFont whichTextFont,
              wxString& whichImagePath,
              const wxString& whichImageInfix,
#if wxMAJOR_VERSION<3
              long whichImageType );
#else
              wxBitmapType& whichImageType );
#endif

      ~ScaleImageVerticalGradientColor()
      {}

    protected:
      int numSquaresWithoutOutliers;
      int totalSquares;

      virtual void init();
      virtual void sortSemanticValues();
      virtual void draw();
    
    private:    
  };

  class ScaleImageHorizontalGradientColor : public ScaleImageVerticalGradientColor
  {
    public:
      ScaleImageHorizontalGradientColor(
              Window* whichMyWindow,
              const std::map< TSemanticValue, rgb >& whichSemanticValues,
              wxColour whichBackground,
              wxColour whichForeground,
              int whichBackgroundMode,
              wxFont whichTextFont,
              wxString& whichImagePath,
              const wxString& whichImageInfix,
#if wxMAJOR_VERSION<3
              long whichImageType );
#else
              wxBitmapType& whichImageType );
#endif

      ~ScaleImageHorizontalGradientColor()
      {}

    protected:
      virtual void init();
      virtual void computeImageSize();
      virtual void draw();
    
    private:
      typedef enum { LEFT = 0, CENTER, RIGHT } TAlign;
      typedef enum { FIRST = 0, MIDDLE, LAST, ANY } TPosition;

      int SIZE_OF_TINY_MARK;
      int outlierMargin; // Inner margin between outlier and whole scale

      void drawRectangle( rgb semanticColour, TPosition position = ANY );
      void drawLabel( wxString semanticValueLabel, bool drawIt = true, TAlign align = LEFT );    
  };

};

void progressFunctionTimeline( ProgressController *progress, void *callerWindow );

#endif  // _GTIMELINE_H_
