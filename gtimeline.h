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
#include "paraverkerneltypes.h"

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
#define SYMBOL_GTIMELINE_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMAXIMIZE_BOX|wxCLOSE_BOX|wxFRAME_NO_TASKBAR
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

  /// wxEVT_SIZE event handler for ID_GTIMELINE
  void OnSize( wxSizeEvent& event );

  /// wxEVT_IDLE event handler for ID_GTIMELINE
  void OnIdle( wxIdleEvent& event );

  /// wxEVT_PAINT event handler for ID_SCROLLEDWINDOW
  void OnPaint( wxPaintEvent& event );

  /// wxEVT_ERASE_BACKGROUND event handler for ID_SCROLLEDWINDOW
  void OnEraseBackground( wxEraseEvent& event );

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

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end gTimeline member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  void redraw();
  void drawAxis( wxDC& dc );
  void drawRow( wxDC& dc, TObjectOrder row );

//  void OnRightClick(wxMouseEvent& event);
////@begin gTimeline member variables
  wxScrolledWindow* drawZone;
  wxBitmap bufferImage;
private:
  bool ready;
  Window* myWindow;
  wxCoord objectAxisPos;
  wxCoord timeAxisPos;
  wxFont objectFont;
  wxFont timeFont;
  vector<wxCoord> objectPosList;
////@end gTimeline member variables

  static const wxCoord drawBorder = 5;
};

#endif
  // _GTIMELINE_H_
