/////////////////////////////////////////////////////////////////////////////
// Name:        gtimeline.cpp
// Purpose:
// Author:      Eloy Martinez
// Modified by:
// Created:     Wed 27 Aug 2008 11:26:03 CEST
// RCS-ID:
// Copyright:
// Licence:
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#ifdef __BORLANDC__
#pragma hdrstopclo
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
#include "wx/imaglist.h"
////@end includes
#include <wx/dcbuffer.h>
#include <wx/statline.h>
#include <algorithm>

#include "gtimeline.h"
#include "window.h"
#include "labelconstructor.h"
#include "drawmode.h"
#include "loadedwindows.h"
#include "windows_tree.h"
#include "caution.xpm"

#define wxTEST_GRAPHICS 1

#if wxTEST_GRAPHICS
#include "wx/graphics.h"
#if wxUSE_GRAPHICS_CONTEXT == 0
#undef wxTEST_GRAPHICS
#define wxTEST_GRAPHICS 0
#endif
#else
#undef wxUSE_GRAPHICS_CONTEXT
#define wxUSE_GRAPHICS_CONTEXT 0
#endif

////@begin XPM images
////@end XPM images


/*!
 * gTimeline type definition
 */

IMPLEMENT_CLASS( gTimeline, wxFrame )


/*!
 * gTimeline event table definition
 */

BEGIN_EVENT_TABLE( gTimeline, wxFrame )

////@begin gTimeline event table entries
  EVT_CLOSE( gTimeline::OnCloseWindow )
  EVT_IDLE( gTimeline::OnIdle )
  EVT_RIGHT_DOWN( gTimeline::OnRightDown )

  EVT_SPLITTER_DCLICK( ID_SPLITTERWINDOW, gTimeline::OnSplitterwindowSashDClick )
  EVT_SPLITTER_UNSPLIT( ID_SPLITTERWINDOW, gTimeline::OnSplitterwindowSashUnsplit )

  EVT_UPDATE_UI( ID_SCROLLEDWINDOW, gTimeline::OnScrolledWindowUpdate )

  EVT_NOTEBOOK_PAGE_CHANGING( ID_NOTEBOOK, gTimeline::OnNotebookPageChanging )

  EVT_UPDATE_UI( ID_PANEL1, gTimeline::OnColorsPanelUpdate )

  EVT_CHECKBOX( ID_CHECK_DRAWLINES, gTimeline::OnCheckDrawlinesClick )
  EVT_UPDATE_UI( ID_CHECK_DRAWLINES, gTimeline::OnCheckDrawlinesUpdate )

  EVT_CHECKBOX( ID_CHECK_DRAWFLAGS, gTimeline::OnCheckDrawflagsClick )
  EVT_UPDATE_UI( ID_CHECK_DRAWFLAGS, gTimeline::OnCheckDrawflagsUpdate )

  EVT_CHECKBOX( ID_CHECK_FUNCTIONLINECOLOR, gTimeline::OnCheckFunctionlinecolorClick )
  EVT_UPDATE_UI( ID_CHECK_FUNCTIONLINECOLOR, gTimeline::OnCheckFunctionlinecolorUpdate )

////@end gTimeline event table entries

END_EVENT_TABLE()

static char flag[20] = { 0xc7, 0x01, 0x7d, 0x03, 0xab, 0x02, 0x55, 0x03, 0xab, 0x02, 0xd7, 0x03,
                         0x79, 0x02, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00 };

/*!
 * gTimeline constructors
 */
//class paraverMain;
#include "paravermain.h"
gTimeline::gTimeline()
{
  Init();
  parent = NULL;
}

gTimeline::gTimeline( wxWindow* whichParent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( whichParent, id, caption, pos, size, style );
  parent = whichParent;
}


/*!
 * gTimeline creator
 */

bool gTimeline::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin gTimeline creation
  wxFrame::Create( parent, id, caption, pos, size, style );

  CreateControls();
////@end gTimeline creation
#ifndef WIN32
  splitter->Unsplit();
#endif
  return true;
}


/*!
 * gTimeline destructor
 */

gTimeline::~gTimeline()
{
////@begin gTimeline destruction
////@end gTimeline destruction
}


/*!
 * Member initialisation
 */

void gTimeline::Init()
{
////@begin gTimeline member initialisation
  ready = false;
  myWindow = NULL;
  objectHeight = 1;
  zooming = false;
  canRedraw = false;
  firstUnsplit = false;
  redoColors = false;
  drawCaution = false;
  splitter = NULL;
  drawZone = NULL;
  infoZone = NULL;
  whatWhereText = NULL;
  timingZone = NULL;
  initialTimeText = NULL;
  finalTimeText = NULL;
  durationText = NULL;
  colorsPanel = NULL;
  colorsSizer = NULL;
  viewPropPanel = NULL;
  checkDrawLines = NULL;
  checkDrawFlags = NULL;
  checkFunctionLineColor = NULL;
////@end gTimeline member initialisation

  zoomXY = false;
  bufferImage.Create( 1, 1 );
  objectFont = wxFont( 7, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL );
  timeFont = wxFont( 6, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL );
}


/*!
 * Control creation for gTimeline
 */

void gTimeline::CreateControls()
{
////@begin gTimeline content construction
  gTimeline* itemFrame1 = this;

  splitter = new wxSplitterWindow( itemFrame1, ID_SPLITTERWINDOW, wxDefaultPosition, wxDefaultSize, wxSP_3DBORDER|wxSP_3DSASH|wxSP_PERMIT_UNSPLIT|wxNO_BORDER );
  splitter->SetMinimumPaneSize(0);

  drawZone = new wxScrolledWindow( splitter, ID_SCROLLEDWINDOW, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxFULL_REPAINT_ON_RESIZE|wxHSCROLL|wxVSCROLL );
  drawZone->SetScrollbars(1, 1, 0, 0);
  infoZone = new wxNotebook( splitter, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

  whatWhereText = new wxRichTextCtrl( infoZone, ID_RICHTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxWANTS_CHARS );

  infoZone->AddPage(whatWhereText, _("What/Where"));

  timingZone = new wxScrolledWindow( infoZone, ID_PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  timingZone->SetScrollbars(1, 5, 0, 0);
  wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
  timingZone->SetSizer(itemBoxSizer7);

  wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer7->Add(itemBoxSizer8, 0, wxGROW, 5);
  wxStaticText* itemStaticText9 = new wxStaticText( timingZone, wxID_STATIC, _("Initial time"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer8->Add(itemStaticText9, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 10);

  wxStaticText* itemStaticText10 = new wxStaticText( timingZone, wxID_STATIC, _("Final time"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer8->Add(itemStaticText10, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 10);

  wxStaticText* itemStaticText11 = new wxStaticText( timingZone, wxID_STATIC, _("Duration"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer8->Add(itemStaticText11, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 10);

  wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer7->Add(itemBoxSizer12, 1, wxGROW, 5);
  initialTimeText = new wxTextCtrl( timingZone, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer12->Add(initialTimeText, 0, wxGROW|wxALL, 5);

  finalTimeText = new wxTextCtrl( timingZone, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer12->Add(finalTimeText, 0, wxGROW|wxALL, 5);

  durationText = new wxTextCtrl( timingZone, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer12->Add(durationText, 0, wxGROW|wxALL, 5);

  timingZone->FitInside();
  infoZone->AddPage(timingZone, _("Timing"));

  colorsPanel = new wxScrolledWindow( infoZone, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxVSCROLL|wxTAB_TRAVERSAL );
  colorsPanel->SetScrollbars(1, 5, 0, 0);
  colorsSizer = new wxBoxSizer(wxVERTICAL);
  colorsPanel->SetSizer(colorsSizer);

  colorsPanel->FitInside();
  infoZone->AddPage(colorsPanel, _("Colors"));

  viewPropPanel = new wxScrolledWindow( infoZone, ID_SCROLLEDWINDOW1, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
  viewPropPanel->SetScrollbars(1, 5, 0, 0);
  wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxVERTICAL);
  viewPropPanel->SetSizer(itemBoxSizer19);

  checkDrawLines = new wxCheckBox( viewPropPanel, ID_CHECK_DRAWLINES, _("Draw Communication Lines"), wxDefaultPosition, wxDefaultSize, 0 );
  checkDrawLines->SetValue(true);
  itemBoxSizer19->Add(checkDrawLines, 0, wxALIGN_LEFT|wxALL, 5);

  checkDrawFlags = new wxCheckBox( viewPropPanel, ID_CHECK_DRAWFLAGS, _("Draw Event Flags"), wxDefaultPosition, wxDefaultSize, 0 );
  checkDrawFlags->SetValue(true);
  itemBoxSizer19->Add(checkDrawFlags, 0, wxALIGN_LEFT|wxALL, 5);

  checkFunctionLineColor = new wxCheckBox( viewPropPanel, ID_CHECK_FUNCTIONLINECOLOR, _("Draw Function Line With Color"), wxDefaultPosition, wxDefaultSize, 0 );
  checkFunctionLineColor->SetValue(true);
  itemBoxSizer19->Add(checkFunctionLineColor, 0, wxALIGN_LEFT|wxALL, 5);

  viewPropPanel->FitInside();
  infoZone->AddPage(viewPropPanel, _("View Properties"));

  splitter->SplitHorizontally(drawZone, infoZone, 0);

  // Connect events and objects
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_SIZE, wxSizeEventHandler(gTimeline::OnScrolledWindowSize), NULL, this);
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_PAINT, wxPaintEventHandler(gTimeline::OnScrolledWindowPaint), NULL, this);
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(gTimeline::OnScrolledWindowEraseBackground), NULL, this);
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_LEFT_DOWN, wxMouseEventHandler(gTimeline::OnScrolledWindowLeftDown), NULL, this);
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_LEFT_UP, wxMouseEventHandler(gTimeline::OnScrolledWindowLeftUp), NULL, this);
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_RIGHT_DOWN, wxMouseEventHandler(gTimeline::OnScrolledWindowRightDown), NULL, this);
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_MOTION, wxMouseEventHandler(gTimeline::OnScrolledWindowMotion), NULL, this);
////@end gTimeline content construction
}


/*!
 * Should we show tooltips?
 */

bool gTimeline::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap gTimeline::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin gTimeline bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
////@end gTimeline bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon gTimeline::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin gTimeline icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end gTimeline icon retrieval
}


void gTimeline::redraw()
{
  redoColors = true;
  
  wxString winTitle = GetTitle();
  SetTitle( winTitle + _(" (Working...)") );

  // Get selected rows
  vector<bool>         selected;
  vector<TObjectOrder> selectedSet;
  TObjectOrder beginRow = myWindow->getZoomSecondDimension().first;
  TObjectOrder endRow =  myWindow->getZoomSecondDimension().second;
  myWindow->getSelectedRows( myWindow->getLevel(), selected );
  myWindow->getSelectedRows( myWindow->getLevel(), selectedSet, beginRow, endRow );
  TObjectOrder maxObj = selectedSet[ selectedSet.size() - 1 ];

  ready = false;
  bufferImage.Create( drawZone->GetSize().GetWidth(), drawZone->GetSize().GetHeight() );
  drawImage.Create( drawZone->GetSize().GetWidth(), drawZone->GetSize().GetHeight() );
  commImage.Create( drawZone->GetSize().GetWidth(), drawZone->GetSize().GetHeight() );
  eventImage.Create( drawZone->GetSize().GetWidth(), drawZone->GetSize().GetHeight() );
  wxMemoryDC bufferDraw( bufferImage );
  wxMemoryDC commdc( commImage );
  wxMemoryDC eventdc( eventImage );
  commdc.SetBackgroundMode( wxTRANSPARENT );
  commdc.SetBackground( *wxTRANSPARENT_BRUSH );
  commdc.Clear();
  eventdc.SetBackgroundMode( wxTRANSPARENT );
  eventdc.SetBackground( *wxTRANSPARENT_BRUSH );
  eventdc.Clear();
  wxBitmap commMask;
  commMask.Create( drawZone->GetSize().GetWidth(), drawZone->GetSize().GetHeight(), 1 );
  wxMemoryDC commmaskdc( commMask );
  commmaskdc.SetBackground( *wxBLACK_BRUSH );
  commmaskdc.SetPen( wxPen( wxColour( 255, 255, 255 ), 1 ) );
  commmaskdc.Clear();
  wxBitmap eventMask;
  eventMask.Create( drawZone->GetSize().GetWidth(), drawZone->GetSize().GetHeight(), 1 );
  wxMemoryDC eventmaskdc( eventMask );
  eventmaskdc.SetBackground( *wxBLACK_BRUSH );
  eventmaskdc.SetPen( wxPen( wxColour( 255, 255, 255 ), 1 ) );
  eventmaskdc.Clear();

  bufferDraw.SetBackground( wxBrush( *wxBLACK_BRUSH ) );
  bufferDraw.Clear();
  drawAxis( bufferDraw, selectedSet );
  myWindow->init( myWindow->getWindowBeginTime(), CREATECOMMS + CREATEEVENTS );

  drawCaution = false;
  
  // Drawmode: Group objects with same wxCoord in objectPosList
  vector<TObjectOrder>::iterator endIt = selectedSet.end();
  for( vector< TObjectOrder >::iterator obj = selectedSet.begin(); obj != endIt; ++obj )
  {
    TObjectOrder firstObj = *obj;
    TObjectOrder lastObj = firstObj;
    while( ( lastObj + 1 ) <= maxObj && objectPosList[ lastObj + 1 ] == objectPosList[ firstObj ] )
    {
      ++obj;
      lastObj = *obj;
    }
    drawRow( bufferDraw, commdc, commmaskdc, eventdc, eventmaskdc,
             firstObj, lastObj, selectedSet, selected );
  }
  bufferDraw.SelectObject(wxNullBitmap);
  bufferDraw.SelectObject( drawImage );
  bufferDraw.DrawBitmap( bufferImage, 0, 0, false );

  if( drawCaution )
  {
    wxBitmap cautionImage( caution_xpm );
    bufferDraw.DrawBitmap( cautionImage,
                           drawBorder,
                           drawZone->GetSize().GetHeight() - cautionImage.GetHeight() - drawBorder,
                           true );
  }
  
  eventmaskdc.SetPen( *wxBLACK_PEN );
  eventmaskdc.SetBrush( *wxBLACK_BRUSH );
  eventmaskdc.DrawRectangle( 0, 0, objectAxisPos + 1, drawZone->GetSize().GetHeight() );
  eventmaskdc.DrawRectangle( drawZone->GetSize().GetWidth() - drawBorder, 0, drawBorder, drawZone->GetSize().GetHeight() );
  eventmaskdc.SelectObject(wxNullBitmap);
  wxMask *mask = new wxMask( eventMask );
  eventImage.SetMask( mask );

  if( myWindow->getDrawFlags() )
    bufferDraw.DrawBitmap( eventImage, 0, 0, true );

  commmaskdc.SetPen( *wxBLACK_PEN );
  commmaskdc.SetBrush( *wxBLACK_BRUSH );
  commmaskdc.DrawRectangle( 0, 0, objectAxisPos + 1, drawZone->GetSize().GetHeight() );
  commmaskdc.DrawRectangle( drawZone->GetSize().GetWidth() - drawBorder, 0, drawBorder, drawZone->GetSize().GetHeight() );
  commmaskdc.SelectObject(wxNullBitmap);
  mask = new wxMask( commMask );
  commImage.SetMask( mask );

  if( myWindow->getDrawCommLines() )
    bufferDraw.DrawBitmap( commImage, 0, 0, true );

  ready = true;
  SetTitle( winTitle );
}


void gTimeline::drawAxis( wxDC& dc, vector<TObjectOrder>& selected )
{
  size_t numObjects = selected.size();

  dc.SetPen( wxPen( *wxWHITE, 1 ) );
  dc.SetTextForeground( *wxWHITE );

  // Get the text extent for time label
  dc.SetFont( timeFont );
  wxSize timeExt = dc.GetTextExtent( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( myWindow->getWindowBeginTime() ),
                                                                  myWindow->getTimeUnit() ) );
  timeAxisPos = dc.GetSize().GetHeight() - ( drawBorder + timeExt.GetHeight() + drawBorder );

  // Get the text extent for the last object (probably the larger one)
  dc.SetFont( objectFont );
  // +1!
  wxSize objectExt = dc.GetTextExtent( LabelConstructor::objectLabel( myWindow->getWindowLevelObjects() - 1,
                                                                      myWindow->getLevel(),
                                                                      myWindow->getTrace() ) );
  objectAxisPos = drawBorder + objectExt.GetWidth() + drawBorder;

  // Draw axis lines
  dc.DrawLine( objectAxisPos, drawBorder,
               objectAxisPos, timeAxisPos );
  dc.DrawLine( objectAxisPos, timeAxisPos,
               dc.GetSize().GetWidth() - drawBorder, timeAxisPos );

  // Draw axis labels
  wxCoord y;
  double inc = (double)( timeAxisPos - drawBorder ) / (double)( numObjects );

  objectPosList.clear();
  objectPosList.insert( objectPosList.begin(), myWindow->getWindowLevelObjects(), 0 );
  objectHeight = 1;
  vector< TObjectOrder >::iterator it = selected.begin();

  // for every object
  for( TObjectOrder obj = (TObjectOrder)0; obj < numObjects; obj++ )
  {
    y = ( (wxCoord) ( inc * ( obj ) ) ) + drawBorder;
    if( ( inc * 0.25 ) >= 1.0 )
    {
      if( obj > (TObjectOrder)0 )
        objectHeight < ( y - objectPosList[ selected[ obj - 1 ] ] ) * 0.75 ?
                       objectHeight = ( y - objectPosList[ selected[ obj - 1 ] ] ) * 0.75 :
                       objectHeight = objectHeight;
      y += (wxCoord)( inc * 0.25 );
    }
    else
    {
      if( obj > (TObjectOrder)0 )
        objectHeight < ( y - objectPosList[ selected[ obj - 1 ] ] ) ?
                       objectHeight = ( y - objectPosList[ selected[ obj - 1 ] ] ) :
                       objectHeight = objectHeight;
    }
    objectPosList[ selected[ obj ] ] = y;
    dc.DrawText( LabelConstructor::objectLabel( *it, myWindow->getLevel(), myWindow->getTrace() ),
                 drawBorder, y );

    // next selected row
    ++it;
  }

  if( numObjects == 1 )
    objectHeight = timeAxisPos - objectPosList[ selected[ 0 ] ];

  dc.SetFont( timeFont );
  dc.DrawText( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( myWindow->getWindowBeginTime() ),
                                            myWindow->getTimeUnit() ),
               objectAxisPos, timeAxisPos + drawBorder );
  dc.DrawText( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( myWindow->getWindowEndTime() ),
                                            myWindow->getTimeUnit() ),
               dc.GetSize().GetWidth() -
               ( dc.GetTextExtent( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( myWindow->getWindowEndTime() ),
                                                                myWindow->getTimeUnit() ) )
               .GetWidth() + drawBorder ),
               timeAxisPos + drawBorder );
}


void gTimeline::drawRow( wxDC& dc, wxMemoryDC& commdc, wxDC& commmaskdc,
                         wxMemoryDC& eventdc, wxDC& eventmaskdc,
                         TObjectOrder firstRow, TObjectOrder lastRow,
                         vector<TObjectOrder>& selectedSet, vector<bool>& selected )
{
  TTime timeStep = ( myWindow->getWindowEndTime() - myWindow->getWindowBeginTime() ) /
                   ( dc.GetSize().GetWidth() - objectAxisPos - drawBorder );
  vector<TSemanticValue> timeValues;
  vector<TSemanticValue> rowValues;
  wxCoord timePos = objectAxisPos + 1;

  vector<TObjectOrder>::iterator first = find( selectedSet.begin(), selectedSet.end(), firstRow );
  vector<TObjectOrder>::iterator last  = find( selectedSet.begin(), selectedSet.end(), lastRow );

  wxCoord objectPos = objectPosList[ firstRow ];
  int lineLastPos = 0;
  
  for( TTime currentTime = myWindow->getWindowBeginTime() + timeStep;
       currentTime <= myWindow->getWindowEndTime();
       currentTime += timeStep )
  {
    rowValues.clear();
    for( vector<TObjectOrder>::iterator row = first; row <= last; ++row )
    {
      timeValues.clear();

      while( myWindow->getEndTime( *row ) <= currentTime - timeStep )
        myWindow->calcNext( *row );

      timeValues.push_back( myWindow->getValue( *row ) );
      while( myWindow->getEndTime( *row ) < currentTime )
      {
        myWindow->calcNext( *row );
        TSemanticValue currentValue = myWindow->getValue( *row );
        timeValues.push_back( currentValue );
        if( currentValue < myWindow->getMinimumY() 
            || currentValue > myWindow->getMaximumY() )
          drawCaution = true;
      }
      rowValues.push_back( DrawMode::selectValue( timeValues, myWindow->getDrawModeTime() ) );

      RecordList *rl = myWindow->getRecordList( *row );
      if( rl != NULL )
        drawRecords( commdc, commmaskdc, eventdc, eventmaskdc,
                     rl, currentTime - timeStep, currentTime, timeStep, timePos, selected );
    }
    TSemanticValue valueToDraw = DrawMode::selectValue( rowValues, myWindow->getDrawModeObject() );
    if( myWindow->getDrawFunctionLineColor() )
    {
      rgb colorToDraw = myWindow->calcColor( valueToDraw, *myWindow );
      dc.SetPen( wxPen( wxColour( colorToDraw.red, colorToDraw.green, colorToDraw.blue ) ) );
      if( objectPos + objectHeight < timeAxisPos )
        dc.DrawLine( timePos, objectPos, timePos, objectPos + objectHeight );
      else
        dc.DrawLine( timePos, objectPos, timePos, timeAxisPos - 1 );
    }
    else
    {
      if( valueToDraw < myWindow->getMinimumY() )
        valueToDraw = myWindow->getMinimumY();
      else if( valueToDraw > myWindow->getMaximumY() )
        valueToDraw = myWindow->getMaximumY();

      double tmpPos = ( valueToDraw - myWindow->getMinimumY() ) 
                      / ( myWindow->getMaximumY() - myWindow->getMinimumY() );
      int currentPos = objectHeight * tmpPos;
      
      dc.SetPen( *wxWHITE_PEN );
      if( currentPos != lineLastPos )
      {
        int from = ( currentPos > lineLastPos ) ? currentPos : lineLastPos;
        int to   = ( currentPos < lineLastPos ) ? currentPos : lineLastPos;
        dc.DrawLine( timePos, objectPos + objectHeight - from,
                     timePos, objectPos + objectHeight - to + 1 );
      }
      else
        dc.DrawPoint( timePos, objectPos + objectHeight - currentPos );

      lineLastPos = currentPos;
    }
    timePos++;
  }
}


void gTimeline::drawRecords( wxMemoryDC& commdc, wxDC& commmaskdc,
                             wxMemoryDC& eventdc, wxDC& eventmaskdc,
                             RecordList *records,
                             TTime from, TTime to, TTime step, wxCoord pos, vector<bool>& selected )
{
  bool existEvents = false;
  TObjectOrder row = 0;
  
  RecordList::iterator it = records->begin();
  step = ( 1 / step );
  while( it != records->end() && it->getTime() < from )
    ++it;
  while( it != records->end() && it->getTime() <= to )
  {
    if( it->getType() & EVENT )
    {
      existEvents = true;
      row = it->getOrder();
    }
    else
    {
      TObjectOrder partnerObject = it->getCommPartnerObject();
      if( ( it->getType() & COMM ) && selected[ partnerObject ] &&
          ( it->getType() & RECV ||
            ( it->getType() & SEND && it->getCommPartnerTime() > myWindow->getWindowEndTime() ) )
        )
      {
        if( it->getType() & LOG )
          commdc.SetPen( wxPen( wxColour( 255, 255, 0 ) ) );
        else if( it->getType() & PHY )
          commdc.SetPen( *wxRED_PEN );
        wxCoord posPartner = (wxCoord)( ( it->getCommPartnerTime() - myWindow->getWindowBeginTime() ) * step );
        posPartner += objectAxisPos;
        commdc.DrawLine( posPartner, objectPosList[ partnerObject ],
                         pos, objectPosList[ it->getOrder() ] );
        commmaskdc.DrawLine( posPartner, objectPosList[ partnerObject ],
                             pos, objectPosList[ it->getOrder() ] );
      }
    }
    ++it;
  }

  if( existEvents )
  {
    eventdc.SetTextForeground( *wxGREEN );
    eventdc.SetTextBackground( *wxBLACK );
/*    eventdc.SetPen( *wxGREEN_PEN );
    eventdc.SetBrush( *wxGREEN_BRUSH );*/
    eventdc.SetBackgroundMode( wxTRANSPARENT );
/*    eventdc.SetBackground( *wxBLACK_BRUSH );
    eventdc.SetBrush( *wxGREEN_BRUSH );*/
    wxBitmap imgFlag( flag, 10, 10 );
/*    wxMask *newMask = new wxMask( imgFlag, *wxWHITE );
    imgFlag.SetMask( newMask );*/
    eventdc.DrawBitmap( imgFlag, pos, objectPosList[ row ] - 10, true );
    eventmaskdc.SetPen( *wxWHITE_PEN );
    eventmaskdc.SetBrush( *wxWHITE_BRUSH );
//    eventmaskdc.DrawBitmap( imgFlag, pos, objectPosList[ row ] - 10, true );
    eventmaskdc.DrawRectangle( pos, objectPosList[ row ] - 10, 9, 9 );
  }
  
  records->erase( records->begin(), it );
}


/*!
 * wxEVT_ERASE_BACKGROUND event handler for ID_SCROLLEDWINDOW
 */
void gTimeline::OnScrolledWindowEraseBackground( wxEraseEvent& event )
{
  //event.Skip();
}


/*!
 * wxEVT_PAINT event handler for ID_SCROLLEDWINDOW
 */
void gTimeline::OnScrolledWindowPaint( wxPaintEvent& event )
{
  wxPaintDC dc( drawZone );
  wxMemoryDC bufferDraw;
  
  if( !ready )
    return;
  
  dc.DrawBitmap( drawImage, 0, 0, false );
}


/*!
 * wxEVT_SIZE event handler for ID_GTIMELINE
 */
void gTimeline::OnScrolledWindowSize( wxSizeEvent& event )
{
  if( canRedraw &&
      ( event.GetSize().GetWidth() != myWindow->getWidth() ||
        event.GetSize().GetHeight() != myWindow->getHeight() ) )
  {
    if( ready )
      redraw();

    myWindow->setWidth( event.GetSize().GetWidth() );
    myWindow->setHeight( event.GetSize().GetHeight() );
  }
  event.Skip();
}


/*!
 * wxEVT_IDLE event handler for ID_GTIMELINE
 */
void gTimeline::OnIdle( wxIdleEvent& event )
{
  this->SetTitle( myWindow->getName() );

#ifdef WIN32
  if( !firstUnsplit )
  {
    firstUnsplit = true;
    splitter->Unsplit();
//    this->SetSize( myWindow->getWidth(), myWindow->getHeight() );
    drawZone->SetSize( myWindow->getWidth(), myWindow->getHeight() );
    canRedraw = true;
    if( !ready )
      redraw();
  }
#else
  canRedraw = true;
#endif

  if( myWindow->getShowWindow() )
  {
    this->Show();
    if( !ready )
      redraw();
  }
  else
    this->Show( false );

  if( myWindow->getRaiseWindow() )
  {
    if( !this->IsShown() )
      myWindow->setShowWindow( true );
    else
      this->Raise();
    
    myWindow->setRaiseWindow( false );
  }
  
  myWindow->setPosX( this->GetPosition().x );
  myWindow->setPosY( this->GetPosition().y );
}


/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_GTIMELINE
 */
void gTimeline::OnCloseWindow( wxCloseEvent& event )
{
  myWindow->setShowWindow( false );
}


/*!
 * wxEVT_LEFT_DOWN event handler for ID_SCROLLEDWINDOW
 */
void gTimeline::OnScrolledWindowLeftDown( wxMouseEvent& event )
{
  wxBitmap tmpImage( caution_xpm );
  if( event.GetX() < tmpImage.GetWidth() + drawBorder
      && event.GetY() > drawZone->GetSize().GetHeight() - tmpImage.GetHeight() - drawBorder )
  {
    wxMessageDialog dialog( this,
                            wxT( "Some semantic values are outside the maximum or minimum boundaries." ),
                            wxT( "Semantic Scale Warning" ),
                            wxOK|wxICON_EXCLAMATION );
    dialog.ShowModal();
    return;
  }
  zooming = true;
  zoomBeginX = event.GetX();
  zoomBeginY = event.GetY();
}


/*!
 * wxEVT_LEFT_UP event handler for ID_SCROLLEDWINDOW
 */
void gTimeline::OnScrolledWindowLeftUp( wxMouseEvent& event )
{
  TObjectOrder beginRow = myWindow->getZoomSecondDimension().first;
  TObjectOrder endRow = myWindow->getZoomSecondDimension().second;
  wxMemoryDC dc( bufferImage );

  if( event.ShiftDown() )
  {
    zooming = false;
    return;
  }
  
  zoomEndX = event.GetX();
  zoomEndY = event.GetY();
  zoomXY = event.ControlDown();

  // TIME zoom limits
  if( zooming )
  {
    if( zoomEndX < zoomBeginX )
    {
      long tmp = zoomEndX; zoomEndX = zoomBeginX; zoomBeginX = tmp;
    }
    if( zoomBeginX < objectAxisPos )
      zoomBeginX = 0;
    else
      zoomBeginX -= objectAxisPos;
  }
  if( zoomEndX < objectAxisPos )
    zoomEndX = 0;
  else if( zoomEndX > dc.GetSize().GetWidth() - drawBorder )
    zoomEndX = dc.GetSize().GetWidth() - drawBorder - objectAxisPos;
  else
    zoomEndX -= objectAxisPos;

  // Detect begin and end TIME
  TTime timeStep = ( myWindow->getWindowEndTime() - myWindow->getWindowBeginTime() ) /
                   ( dc.GetSize().GetWidth() - objectAxisPos - drawBorder );

  TTime endTime = ( timeStep * zoomEndX ) + myWindow->getWindowBeginTime();
  TTime beginTime = ( timeStep * zoomBeginX ) + myWindow->getWindowBeginTime();

  if( zooming )
  {
    if( zoomEndY < zoomBeginY )
    {
      long tmp = zoomEndY; zoomEndY = zoomBeginY; zoomBeginY = tmp;
    }
    if( zoomBeginY > timeAxisPos )
      zoomBeginY = timeAxisPos - 1;
    if( zoomBeginY < drawBorder )
      zoomBeginY = drawBorder + 1;
  }

  if( zoomEndY > timeAxisPos )
    zoomEndY = timeAxisPos - 1;
  if( zoomEndY < drawBorder )
    zoomEndY = drawBorder;

  vector<TObjectOrder> selected;
  myWindow->getSelectedRows( myWindow->getLevel(), selected, beginRow, endRow );
  TObjectOrder numObjects = selected.size();
  double heightPerRow = (double)( timeAxisPos - drawBorder - 1 ) / (double)numObjects;
  beginRow = TObjectOrder( floor( (zoomBeginY - drawBorder) / heightPerRow ) );
  endRow = TObjectOrder( floor( (zoomEndY - drawBorder - 1) / heightPerRow ) );

  if( endRow > numObjects )
    endRow = numObjects - 1;

  beginRow = selected[ beginRow ];
  endRow   = selected[ endRow ];

  if( zooming && ready &&
      ( zoomBeginX != zoomEndX || zoomBeginY != zoomEndY ) )
  {
    if( !zoomXY )
    {
      beginRow = myWindow->getZoomSecondDimension().first;
      endRow =  myWindow->getZoomSecondDimension().second;
    }
    if( ( endTime - beginTime ) < 10 )
      endTime = beginTime + 10;
    myWindow->addZoom( beginTime, endTime, beginRow, endRow );

    // Update window properties
    myWindow->setWindowBeginTime( beginTime );
    myWindow->setWindowEndTime( endTime );

    myWindow->setRedraw( true );
    myWindow->setChanged( true );
  }
  else
  {
    if( !splitter->IsSplit() )
    {
      Split();
    }
    printWhatWhere( endTime, endRow );
  }

  zooming = false;
  zoomXY = false;
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_SCROLLEDWINDOW
 */
void gTimeline::OnScrolledWindowUpdate( wxUpdateUIEvent& event )
{
  if( this->IsShown() )
  {
    if( myWindow->getRedraw() )
    {
      myWindow->setRedraw( false );
      redraw();
      drawZone->Refresh();
    }
  }
}


void gTimeline::OnPopUpCopy()
{
  gPasteWindowProperties::pasteWindowProperties->getInstance()->copy( this );
}


// simple windows can let this method do the entire work passing a NULL clonedWindow
// derived windows must pass existing clonedWindow, because Window::clone is recursive
gTimeline *gTimeline::clone( Window *clonedWindow,
                             wxWindow *parent,
                             wxTreeItemId idRoot1,
                             wxTreeItemId idRoot2,
                             bool mustRedraw )
{
  if ( clonedWindow == NULL )
    clonedWindow = myWindow->clone(); // recursive clone

  // Create empty gTimeline and assign window with same dimensions.
  // Shifts position right and down.
  wxSize titleBarSize = GetSize() - GetClientSize();
  if ( titleBarSize.GetHeight() == 0 )
    titleBarSize = paraverMain::defaultTitleBarSize;
  wxPoint position =  wxPoint( GetPosition().x + titleBarSize.GetHeight(),
                               GetPosition().y + titleBarSize.GetHeight() );
#ifdef WIN32
  wxSize size = wxSize( clonedWindow->getWidth(), clonedWindow->getHeight() + titleBarSize.GetHeight() );
#else
  wxSize size = wxSize( clonedWindow->getWidth(), clonedWindow->getHeight() );
#endif

  gTimeline *clonedTimeline = new gTimeline( parent, wxID_ANY, wxT( myWindow->getName().c_str() ), position, size );
  clonedTimeline->SetMyWindow( clonedWindow );

  // add to loaded windows list
  LoadedWindows::getInstance()->add( clonedWindow );

  wxChoicebook *choiceWindowBrowser = paraverMain::myParaverMain->choiceWindowBrowser;
  INT16 currentTrace = paraverMain::myParaverMain->GetCurrentTrace();
  wxTreeCtrl *allTracesPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 ); // Global page
  wxTreeCtrl *currentPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( currentTrace + 1 ); // Current page

  TreeBrowserItemData *currentData =  new TreeBrowserItemData( clonedWindow->getName(), clonedTimeline );
  wxTreeItemId currentWindowId1 = allTracesPage->AppendItem( idRoot1, clonedWindow->getName(), 1, -1, currentData );
  wxTreeItemId currentWindowId2 = currentPage->AppendItem( idRoot2, clonedWindow->getName(), 1, -1, new TreeBrowserItemData( *currentData ) );

  if( mustRedraw )
  {
    if( myWindow->getShowWindow() )
    {
      clonedTimeline->Show();
      clonedTimeline->redraw();
    }
    else
      clonedTimeline->Show(false);
  }
  else
    clonedWindow->setShowWindow( false );

  // if derived, clone parents
  if ( clonedWindow->isDerivedWindow() )
  {
//    vector< gTimeline * > gParents;
//    getParentGTimeline( this, gParents );

    clone( clonedWindow->getParent( 0 ), parent, currentWindowId1, currentWindowId2, mustRedraw );
    clone( clonedWindow->getParent( 1 ), parent, currentWindowId1, currentWindowId2, mustRedraw );
  }

  return clonedTimeline;
}


void gTimeline::OnPopUpClone()
{
  clone( NULL, parent, getAllTracesTree()->GetRootItem(), getSelectedTraceTree()->GetRootItem());
}


void gTimeline::OnPopUpFitTimeScale()
{
  myWindow->setWindowBeginTime( 0 );
  myWindow->setWindowEndTime( myWindow->getTrace()->getEndTime() );
  myWindow->addZoom( 0, myWindow->getTrace()->getEndTime() );
  myWindow->setRedraw( true );
  myWindow->setChanged( true );
}


void gTimeline::OnPopUpFitSemanticScaleMin()
{
  myWindow->computeYScaleMin();
  myWindow->setRedraw( true );
  myWindow->setChanged( true );
}

void gTimeline::OnPopUpFitSemanticScaleMax()
{
  myWindow->computeYScaleMax();
  myWindow->setRedraw( true );
  myWindow->setChanged( true );
}

void gTimeline::OnPopUpFitSemanticScale()
{
  myWindow->computeYScale();
  myWindow->setRedraw( true );
  myWindow->setChanged( true );
}


void gTimeline::OnPopUpCodeColor()
{
  myWindow->setCodeColorMode();
  myWindow->setRedraw( true );
}


void gTimeline::OnPopUpPasteTime()
{
  gPasteWindowProperties* pasteActions = gPasteWindowProperties::pasteWindowProperties->getInstance();
  pasteActions->paste( this, "Time" );
  myWindow->addZoom( pasteActions->getBeginTime(), pasteActions->getEndTime() );
  myWindow->setRedraw( true );
  myWindow->setChanged( true );
}


void gTimeline::OnPopUpPasteObjects()
{
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Objects" );
  myWindow->setRedraw( true );
}


void gTimeline::OnPopUpPasteSize()
{
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Size" );
  myWindow->setRedraw( true );
}


void gTimeline::OnPopUpPasteFilterAll()
{
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Events" );
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Communications" );
  myWindow->setRedraw( true );
  myWindow->setChanged( true );
}


void gTimeline::OnPopUpPasteFilterCommunications()
{
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Communications" );
  myWindow->setRedraw( true );
  myWindow->setChanged( true );
}


void gTimeline::OnPopUpPasteFilterEvents()
{
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Events" );
  myWindow->setRedraw( true );
  myWindow->setChanged( true );
}


void gTimeline::OnPopUpPasteSpecial()
{
  wxArrayString choices;

  wxMultiChoiceDialog *dialog = gPopUpMenu::createPasteSpecialDialog( choices, this );

  if ( dialog->ShowModal() == wxID_OK )
  {
    wxArrayInt selections = dialog->GetSelections();
    if ( selections.GetCount() > 0 )
    {
      for ( size_t i = 0; i < selections.GetCount(); i++ )
      {
        gPasteWindowProperties* pasteActions = gPasteWindowProperties::pasteWindowProperties->getInstance();
        if ( pasteActions->isAllowed( this, gPopUpMenu::getOption( choices, selections[i] ) ) )
          pasteActions->paste( this, gPopUpMenu::getOption( choices, selections[i] ) );
      }

      myWindow->setRedraw( true );
      myWindow->setChanged( true );
    }
  }

  delete dialog;
}


void gTimeline::OnPopUpRowSelection()
{
  wxArrayString choices;

  wxMultiChoiceDialog *dialog = gPopUpMenu::createRowSelectionDialog( choices, this );

  if ( dialog->ShowModal() == wxID_OK )
  {
    wxArrayInt selections = dialog->GetSelections();
    if ( selections.GetCount() > 0 )
    {
      vector< TObjectOrder > newSelection;
      for ( size_t row = (size_t)0; row < (size_t)selections.GetCount(); row++ )
        newSelection.push_back( (TObjectOrder)selections[ row ] );

      vector< TObjectOrder > previousSelection;
      myWindow->getSelectedRows( myWindow->getLevel(), previousSelection );

      if ( ( previousSelection.size() != newSelection.size() ) ||
           !equal( previousSelection.begin(), previousSelection.end(), newSelection.begin() ) )
      {
        myWindow->setSelectedRows( myWindow->getLevel(), newSelection );
        myWindow->setRedraw( true );
        myWindow->setChanged( true );
      }
    }
  }

  delete dialog;
}


void gTimeline::OnPopUpGradientColor()
{
  myWindow->setGradientColorMode();
  myWindow->getGradientColor().allowOutOfScale( true );
  myWindow->getGradientColor().allowOutliers( true );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpNotNullGradientColor()
{
  myWindow->setGradientColorMode();
  myWindow->getGradientColor().allowOutOfScale( false );
  myWindow->getGradientColor().allowOutliers( true );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeTimeLast()
{
  myWindow->setDrawModeTime( DRAW_LAST );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeTimeRandom()
{
  myWindow->setDrawModeTime( DRAW_RANDOM );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeTimeRandomNotZero()
{
  myWindow->setDrawModeTime( DRAW_RANDNOTZERO );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeTimeMaximum()
{
  myWindow->setDrawModeTime( DRAW_MAXIMUM );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeTimeMinimumNotZero()
{
  myWindow->setDrawModeTime( DRAW_MINNOTZERO );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeTimeAverage()
{
  myWindow->setDrawModeTime( DRAW_AVERAGE );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeObjectsLast()
{
  myWindow->setDrawModeObject( DRAW_LAST );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeObjectsRandom()
{
  myWindow->setDrawModeObject( DRAW_RANDOM );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeObjectsRandomNotZero()
{
  myWindow->setDrawModeObject( DRAW_RANDNOTZERO );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeObjectsMaximum()
{
  myWindow->setDrawModeObject( DRAW_MAXIMUM );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeObjectsMinimumNotZero()
{
  myWindow->setDrawModeObject( DRAW_MINNOTZERO );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeObjectsAverage()
{
  myWindow->setDrawModeObject( DRAW_AVERAGE );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeBothLast()
{
  myWindow->setDrawModeObject( DRAW_LAST );
  myWindow->setDrawModeTime( DRAW_LAST );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeBothRandom()
{
  myWindow->setDrawModeObject( DRAW_RANDOM );
  myWindow->setDrawModeTime( DRAW_RANDOM );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeBothRandomNotZero()
{
  myWindow->setDrawModeObject( DRAW_RANDNOTZERO );
  myWindow->setDrawModeTime( DRAW_RANDNOTZERO );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeBothMaximum()
{
  myWindow->setDrawModeObject( DRAW_MAXIMUM );
  myWindow->setDrawModeTime( DRAW_MAXIMUM );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeBothMinimumNotZero()
{
  myWindow->setDrawModeObject( DRAW_MINNOTZERO );
  myWindow->setDrawModeTime( DRAW_MINNOTZERO );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeBothAverage()
{
  myWindow->setDrawModeObject( DRAW_AVERAGE );
  myWindow->setDrawModeTime( DRAW_AVERAGE );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpUndoZoom()
{
  if ( !myWindow->emptyPrevZoom() )
  {
    myWindow->prevZoom();
    myWindow->setWindowBeginTime( myWindow->getZoomFirstDimension().first );
    myWindow->setWindowEndTime( myWindow->getZoomFirstDimension().second );

    myWindow->setRedraw( true );
    myWindow->setChanged( true );
  }
}


void gTimeline::OnPopUpRedoZoom()
{
  if ( !myWindow->emptyNextZoom() )
  {
    myWindow->nextZoom();
    myWindow->setWindowBeginTime( myWindow->getZoomFirstDimension().first );
    myWindow->setWindowEndTime( myWindow->getZoomFirstDimension().second );

    myWindow->setRedraw( true );
    myWindow->setChanged( true );
  }
}


void gTimeline::rightDownManager()
{
  gPopUpMenu popUpMenu( this );

  popUpMenu.enable( "Undo Zoom", !myWindow->emptyPrevZoom() );
  popUpMenu.enable( "Redo Zoom", !myWindow->emptyNextZoom() );

  popUpMenu.enableMenu( this );
  PopupMenu( &popUpMenu );
}


/*!
 * wxEVT_RIGHT_DOWN event handler for ID_SCROLLEDWINDOW
 */
void gTimeline::OnScrolledWindowRightDown( wxMouseEvent& event )
{
  rightDownManager();
}


/*!
 * wxEVT_MOTION event handler for ID_SCROLLEDWINDOW
 */
void gTimeline::OnScrolledWindowMotion( wxMouseEvent& event )
{
  wxMemoryDC dc( bufferImage );

  TTime timeStep = ( myWindow->getWindowEndTime() - myWindow->getWindowBeginTime() ) /
                   ( dc.GetSize().GetWidth() - objectAxisPos - drawBorder );

  if( zooming )
  {
    if( !event.ShiftDown() )
      zoomXY = event.ControlDown();
    else
      zoomXY = false;

    wxMemoryDC memdc( drawImage );
    memdc.SetBackgroundMode( wxTRANSPARENT );
    memdc.SetBackground( *wxTRANSPARENT_BRUSH );
    memdc.Clear();
//#ifdef __WXGTK__
#if wxTEST_GRAPHICS == 1
    wxGCDC dc( memdc );
    dc.SetBrush( wxBrush( wxColour( 255, 255, 255, 80 ) ) );
#else
    wxDC& dc = memdc;
    dc.SetBrush( *wxTRANSPARENT_BRUSH );
#endif
    dc.SetPen( *wxWHITE_PEN );

    long beginX = zoomBeginX > event.GetX() ? event.GetX() : zoomBeginX;
    long beginY = drawBorder;
    long endX = zoomBeginX < event.GetX() ? event.GetX() : zoomBeginX;
    if( beginX < objectAxisPos )
      beginX = objectAxisPos;
    if( endX > drawImage.GetWidth() - drawBorder )
      endX = drawImage.GetWidth() - drawBorder;
    wxCoord width = endX - beginX;
    wxCoord height = timeAxisPos - drawBorder + 1;

    if ( zoomXY )
    {
      beginY = zoomBeginY > event.GetY() ? event.GetY() : zoomBeginY;
      long endY = zoomBeginY < event.GetY() ? event.GetY() : zoomBeginY;
      if( beginY > timeAxisPos )
        beginY = timeAxisPos;
      if( endY > timeAxisPos )
        endY = timeAxisPos;
      if( beginY < drawBorder )
        beginY = drawBorder;
      if( endY < drawBorder )
        endY = drawBorder;
      height = endY - beginY;
    }

    dc.DrawBitmap( bufferImage, 0, 0, false );
    if( myWindow->getDrawFlags() )
      dc.DrawBitmap( eventImage, 0, 0, true );
    if( myWindow->getDrawCommLines() )
      dc.DrawBitmap( commImage, 0, 0, true );
    dc.DrawRectangle( beginX, beginY, width, height );

    drawZone->Refresh();

    if( beginX < objectAxisPos )
      beginX = 0;
    else
      beginX -= objectAxisPos;

    if( endX > dc.GetSize().GetWidth() - drawBorder )
      endX = dc.GetSize().GetWidth() - drawBorder - objectAxisPos;
    else
      endX -= objectAxisPos;

    TTime endTime = ( timeStep * endX ) + myWindow->getWindowBeginTime();
    TTime beginTime = ( timeStep * beginX ) + myWindow->getWindowBeginTime();

    initialTimeText->SetValue( _( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( beginTime ),
                                                               myWindow->getTimeUnit() ).c_str() ) );
    finalTimeText->SetValue( _( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( endTime ),
                                                             myWindow->getTimeUnit() ).c_str() ) );
    durationText->SetValue( _( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( endTime - beginTime ),
                                                            myWindow->getTimeUnit() ).c_str() ) );
  }
  else if( event.ShiftDown() )
  {
    long beginX = event.GetX();
    if( beginX < objectAxisPos )
      beginX = 0;
    else if( beginX > dc.GetSize().GetWidth() - drawBorder )
      beginX = dc.GetSize().GetWidth() - drawBorder - objectAxisPos;
    else
      beginX -= objectAxisPos;
    TTime time = ( timeStep * beginX ) + myWindow->getWindowBeginTime();

    initialTimeText->SetValue( _( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( time ), myWindow->getTimeUnit() ).c_str() ) );
    finalTimeText->SetValue( _( "" ) );
    durationText->SetValue( _( "" ) );
  }
}






/*!
 * wxEVT_COMMAND_SPLITTER_UNSPLIT event handler for ID_SPLITTERWINDOW
 */

void gTimeline::OnSplitterwindowSashUnsplit( wxSplitterEvent& event )
{
  Unsplit();
}


/*!
 * wxEVT_COMMAND_SPLITTER_DOUBLECLICKED event handler for ID_SPLITTERWINDOW
 */

void gTimeline::OnSplitterwindowSashDClick( wxSplitterEvent& event )
{
  canRedraw = false;
  event.Skip();
}


/*!
 * wxEVT_RIGHT_DOWN event handler for ID_GTIMELINE
 */

void gTimeline::OnRightDown( wxMouseEvent& event )
{
////@begin wxEVT_RIGHT_DOWN event handler for ID_GTIMELINE in gTimeline.
  // Before editing this code, remove the block markers.
  event.Skip();
////@end wxEVT_RIGHT_DOWN event handler for ID_GTIMELINE in gTimeline.
}




/*!
 * wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING event handler for ID_NOTEBOOK
 */

void gTimeline::OnNotebookPageChanging( wxNotebookEvent& event )
{
  if( myWindow == NULL )
    return;
  canRedraw = false;
  infoZone->ChangeSelection( event.GetSelection() );
  if( !splitter->IsSplit() )
  {
    splitter->SplitHorizontally( drawZone, infoZone, myWindow->getHeight() );
  }
  drawZone->SetSize( myWindow->getWidth(), myWindow->getHeight() );
  canRedraw = true;
}


void gTimeline::printWhatWhere( TRecordTime whichTime, TObjectOrder whichRow )
{
  wxString txt;
  
  whatWhereText->Clear();
  int fontSize = 8;

  whatWhereText->BeginFontSize( fontSize );
  
  txt << _( "Object: " ) << LabelConstructor::objectLabel( whichRow, myWindow->getLevel(), myWindow->getTrace() );
  txt << _( "\t  Click time: " ) << LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( whichTime ),
                                                                 myWindow->getTimeUnit() );
  txt << _( "\n\n" );
  whatWhereText->AppendText( txt );

  myWindow->init( whichTime, CREATEEVENTS + CREATECOMMS );

  txt.Clear();
  myWindow->calcPrev( whichRow );
  printWWSemantic( txt, whichRow, false );
  whatWhereText->BeginBold();
  whatWhereText->AppendText( txt );
  whatWhereText->EndBold();
  txt.Clear();
  printWWRecords( txt, whichRow );
  whatWhereText->BeginFontSize( fontSize - 1 );
  whatWhereText->BeginItalic();
  whatWhereText->AppendText( txt );
  whatWhereText->EndItalic();
  whatWhereText->EndFontSize();

  txt.Clear();
  myWindow->calcNext( whichRow );
  printWWSemantic( txt, whichRow, true );
  whatWhereText->BeginBold();
  whatWhereText->AppendText( txt );
  whatWhereText->EndBold();
  txt.Clear();
  printWWRecords( txt, whichRow );
  whatWhereText->BeginFontSize( fontSize - 1 );
  whatWhereText->BeginItalic();
  whatWhereText->AppendText( txt );
  whatWhereText->EndItalic();
  whatWhereText->EndFontSize();

  txt.Clear();
  myWindow->calcNext( whichRow );
  printWWSemantic( txt, whichRow, false );
  whatWhereText->BeginBold();
  whatWhereText->AppendText( txt );
  whatWhereText->EndBold();
  txt.Clear();
  printWWRecords( txt, whichRow );
  whatWhereText->BeginFontSize( fontSize - 1 );
  whatWhereText->BeginItalic();
  whatWhereText->AppendText( txt );
  whatWhereText->EndItalic();
  whatWhereText->EndFontSize();

  whatWhereText->EndFontSize();
}

void gTimeline::printWWSemantic( wxString& onString, TObjectOrder whichRow, bool clickedValue )
{
  if( clickedValue )
    onString << wxT( "==> " );
  onString << wxT( LabelConstructor::semanticLabel( myWindow, myWindow->getValue( whichRow ), true ) );
  onString << wxT( "\t  Duration: " ) << LabelConstructor::timeLabel(
                                           myWindow->traceUnitsToWindowUnits( myWindow->getEndTime( whichRow )
                                                                              - myWindow->getBeginTime( whichRow ) ),
                                           myWindow->getTimeUnit() );
  onString << _( "\n" );
}

void gTimeline::printWWRecords( wxString& onString, TObjectOrder whichRow )
{
  RecordList *rl = myWindow->getRecordList( whichRow );
  RecordList::iterator it = rl->begin();

  while( it != rl->end() && (*it).getTime() < myWindow->getBeginTime( whichRow ) )
    ++it;
    
  while( it != rl->end() && (*it).getTime() < myWindow->getEndTime( whichRow ) )
  {
    if( (*it).getType() & EVENT )
    {
      onString << wxT( "User Event at " ) << LabelConstructor::timeLabel(
                                               myWindow->traceUnitsToWindowUnits( (*it).getTime() ),
                                               myWindow->getTimeUnit() );
      onString << wxT( "\t" );
      onString << LabelConstructor::eventLabel( myWindow, (*it).getEventType(), (*it).getEventValue(), true );
      onString << wxT( "\n" );
    }
    else if( (*it).getType() & COMM )
    {
      if( (*it).getType() & LOG )
        onString << wxT( "Logical " );
      else if( (*it).getType() & PHY )
        onString << wxT( "Physical " );
        
      if( (*it).getType() & SEND )
        onString << wxT( "SEND " );
      else if( (*it).getType() & RECV )
        onString << wxT( "RECEIVE " );
        
      onString << wxT( "at " ) << LabelConstructor::timeLabel(
                                    myWindow->traceUnitsToWindowUnits( (*it).getTime() ),
                                    myWindow->getTimeUnit() );
      onString << wxT( " to " ) << LabelConstructor::objectLabel( (*it).getCommPartnerObject(),
                                                                  myWindow->getLevel(),
                                                                  myWindow->getTrace() );
      onString << wxT( " at " ) << LabelConstructor::timeLabel(
                                     myWindow->traceUnitsToWindowUnits( (*it).getCommPartnerTime() ),
                                     myWindow->getTimeUnit() );
      if( (*it).getType() & SEND )
        onString << wxT( ", Duration: " ) << LabelConstructor::timeLabel(
                                               myWindow->traceUnitsToWindowUnits( (*it).getCommPartnerTime() 
                                                                                  - (*it).getTime() ),
                                               myWindow->getTimeUnit() );
      else if( (*it).getType() & RECV )
        onString << wxT( ", Duration: " ) << LabelConstructor::timeLabel(
                                               myWindow->traceUnitsToWindowUnits( (*it).getTime()
                                                                                  - (*it).getCommPartnerTime() ),
                                               myWindow->getTimeUnit() );

      onString << wxT( " (size: " ) << (*it).getCommSize() << 
                  wxT( ", tag: " ) << (*it).getCommTag() << wxT( ")" );
      onString << wxT( "\n" );
    }
    ++it;
  }

  rl->erase( rl->begin(), it );
}

void gTimeline::resizeDrawZone( int width, int height )
{
  canRedraw = false;
  drawZone->SetSize( width, height );
  if( !splitter->IsSplit() )
    this->SetClientSize( width, height );
  else
  {
    this->SetClientSize( width, height + infoZone->GetSize().GetHeight() );
    splitter->SetSashPosition( height );
  }
  myWindow->setWidth( width );
  myWindow->setHeight( height );
  canRedraw = true;
}

bool gTimeline::IsSplit() const
{
  return splitter->IsSplit();
}

void gTimeline::OnPopUpInfoPanel()
{
  if( splitter->IsSplit() )
  {
    canRedraw = false;
    splitter->Unsplit();
    Unsplit();
  }
  else
    Split();
}

void gTimeline::Unsplit()
{
  canRedraw = false;
#ifdef WIN32
  this->SetSize( this->GetSize().GetWidth(), this->GetSize().GetHeight() -
                                             infoZone->GetSize().GetHeight() );
#else
  this->SetSize( this->GetSize().GetWidth(), myWindow->getHeight() );
#endif
  drawZone->SetSize( myWindow->getWidth(), myWindow->getHeight() );
  canRedraw = true;
}

void gTimeline::Split()
{
#ifndef WIN32
      int currentHeight = this->GetSize().GetHeight();
#endif
      canRedraw = false;
#ifdef WIN32
      this->SetSize( this->GetSize().GetWidth(),
                     this->GetSize().GetHeight() + infoZone->GetSize().GetHeight() );
      splitter->SplitHorizontally( drawZone, infoZone, myWindow->getHeight() );
#else
      int addHeight = infoZone->GetSize().GetHeight();
      this->SetSize( this->GetSize().GetWidth(),
                     this->GetSize().GetHeight() + ( addHeight < 200 )?200:addHeight );
      splitter->SplitHorizontally( drawZone, infoZone, currentHeight );
#endif
      drawZone->SetSize( myWindow->getWidth(), myWindow->getHeight() );
      canRedraw = true;
}




/*!
 * wxEVT_UPDATE_UI event handler for ID_PANEL1
 */

void gTimeline::OnColorsPanelUpdate( wxUpdateUIEvent& event )
{
  static SemanticInfoType lastType = NO_TYPE;
  static TSemanticValue lastMin = 0;
  static TSemanticValue lastMax = 15;
  static bool codeColorSet = true;
  
  if( redoColors &&
      ( myWindow->getSemanticInfoType() != lastType ||
        myWindow->getMinimumY() != lastMin ||
        myWindow->getMaximumY() != lastMax ||
        myWindow->IsCodeColorSet() != codeColorSet )
    )
  {
    lastType = myWindow->getSemanticInfoType();
    lastMin = myWindow->getMinimumY();
    lastMax = myWindow->getMaximumY();
    codeColorSet = myWindow->IsCodeColorSet();

    colorsSizer->Clear( true );
    wxBoxSizer *itemSizer;
    wxStaticText *itemText;
    wxPanel *itemColor;
    
    if( myWindow->IsCodeColorSet() )
    {
      int endLimit = ceil( lastMax );
    
      if( lastType != EVENTTYPE_TYPE )
      {
        if( lastType == APPL_TYPE )
          endLimit = myWindow->getTrace()->totalApplications() - 1;
        else if( lastType == TASK_TYPE )
          endLimit = myWindow->getTrace()->totalTasks() - 1;
        else if( lastType == THREAD_TYPE )
          endLimit = myWindow->getTrace()->totalThreads() - 1;
        else if( lastType == NODE_TYPE )
          endLimit = myWindow->getTrace()->totalNodes() - 1;
        else if( lastType == CPU_TYPE )
          endLimit = myWindow->getTrace()->totalCPUs() - 1;
        else if( lastMax - lastMin > 200 )
          endLimit = 200 + floor( lastMin );
      }
      int typeEndLimit = 0;
      
      for( int i = floor( lastMin ); i <= endLimit; ++i )
      {
        if( lastType == EVENTTYPE_TYPE && !myWindow->getTrace()->eventLoaded( i ) )
          continue;
          
        if( lastType == EVENTTYPE_TYPE && typeEndLimit > 200 )
          break;
        else
          ++typeEndLimit;
        
        itemSizer = new wxBoxSizer(wxHORIZONTAL);

        itemText = new wxStaticText( colorsPanel, wxID_ANY, _T("") );
        wxString tmpStr = wxT( LabelConstructor::semanticLabel( myWindow, i, true ).c_str() );
        itemText->SetLabel( tmpStr );

        wxSize tmpSize( 20, itemText->GetSize().GetHeight() );
        itemColor = new wxPanel( colorsPanel, wxID_ANY, wxDefaultPosition, tmpSize );
        rgb tmprgb = myWindow->getCodeColor().calcColor( i, myWindow->getMinimumY(), myWindow->getMaximumY() );
        wxColour tmpColor( tmprgb.red, tmprgb.green, tmprgb.blue );
        itemColor->SetBackgroundColour( tmpColor );

        itemSizer->Add( itemColor );
        itemSizer->AddSpacer( 5 );
        itemSizer->Add( itemText );
        colorsSizer->Add( itemSizer, 0, wxGROW|wxALL, 2 );
      
        if( i < ceil( myWindow->getMaximumY() ) )
          colorsSizer->Add( new wxStaticLine( colorsPanel, wxID_ANY ), 0, wxGROW|wxALL, 2 );
      }
    }
    else
    {
      itemSizer = new wxBoxSizer(wxHORIZONTAL);

      itemText = new wxStaticText( colorsPanel, wxID_ANY, _T("") );
      wxString tmpStr;
      tmpStr << wxT("< ") <<LabelConstructor::semanticLabel( myWindow, lastMin, false );
      itemText->SetLabel( tmpStr );

      wxSize tmpSize( 20, itemText->GetSize().GetHeight() );
      itemColor = new wxPanel( colorsPanel, wxID_ANY, wxDefaultPosition, tmpSize );
      rgb tmprgb = myWindow->getGradientColor().calcColor( lastMin - 1, lastMin, lastMax );
      wxColour tmpColor( tmprgb.red, tmprgb.green, tmprgb.blue );
      itemColor->SetBackgroundColour( tmpColor );

      itemSizer->Add( itemColor );
      itemSizer->AddSpacer( 5 );
      itemSizer->Add( itemText );
      colorsSizer->Add( itemSizer, 0, wxGROW|wxALL, 2 );
      
      colorsSizer->Add( new wxStaticLine( colorsPanel, wxID_ANY ), 0, wxGROW|wxALL, 2 );

      TSemanticValue step = ( lastMax - lastMin ) / 20.0;
      for( int i = 0; i <= 20; ++i )
      {
        itemSizer = new wxBoxSizer(wxHORIZONTAL);

        itemText = new wxStaticText( colorsPanel, wxID_ANY, _T("") );
        tmpStr.Clear();
        tmpStr << LabelConstructor::semanticLabel( myWindow, ( i * step ) + lastMin, false );
        itemText->SetLabel( tmpStr );

        tmpSize = wxSize( 20, itemText->GetSize().GetHeight() );
        itemColor = new wxPanel( colorsPanel, wxID_ANY, wxDefaultPosition, tmpSize );
        tmprgb = myWindow->getGradientColor().calcColor( ( i * step ) + lastMin, lastMin, lastMax );
        tmpColor = wxColour( tmprgb.red, tmprgb.green, tmprgb.blue );
        itemColor->SetBackgroundColour( tmpColor );

        itemSizer->Add( itemColor );
        itemSizer->AddSpacer( 5 );
        itemSizer->Add( itemText );
        colorsSizer->Add( itemSizer, 0, wxGROW|wxALL, 2 );
      
        colorsSizer->Add( new wxStaticLine( colorsPanel, wxID_ANY ), 0, wxGROW|wxALL, 2 );
      }
      itemSizer = new wxBoxSizer(wxHORIZONTAL);

      itemText = new wxStaticText( colorsPanel, wxID_ANY, _T("") );
      tmpStr.Clear();
      tmpStr << wxT("> ") <<LabelConstructor::semanticLabel( myWindow, lastMax, false );
      itemText->SetLabel( tmpStr );

      tmpSize = wxSize( 20, itemText->GetSize().GetHeight() );
      itemColor = new wxPanel( colorsPanel, wxID_ANY, wxDefaultPosition, tmpSize );
      tmprgb = myWindow->getGradientColor().calcColor( lastMax + 1, lastMin, lastMax );
      tmpColor = wxColour( tmprgb.red, tmprgb.green, tmprgb.blue );
      itemColor->SetBackgroundColour( tmpColor );

      itemSizer->Add( itemColor );
      itemSizer->AddSpacer( 5 );
      itemSizer->Add( itemText );
      colorsSizer->Add( itemSizer, 0, wxGROW|wxALL, 2 );
    }
    colorsPanel->Layout();
    colorsPanel->FitInside();
  }
  redoColors = false;
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECK_DRAWLINES
 */

void gTimeline::OnCheckDrawlinesClick( wxCommandEvent& event )
{
  wxMemoryDC bufferDraw;

  myWindow->setDrawCommLines( event.IsChecked() );
  
  if( !ready )
    return;
  
  bufferDraw.SelectObject(wxNullBitmap);
  bufferDraw.SelectObject( drawImage );
  bufferDraw.DrawBitmap( bufferImage, 0, 0, false );

  if( drawCaution )
  {
    wxBitmap cautionImage( caution_xpm );
    bufferDraw.DrawBitmap( cautionImage,
                           drawBorder,
                           drawZone->GetSize().GetHeight() - cautionImage.GetHeight() - drawBorder,
                           true );
  }

  if( myWindow->getDrawFlags() )
    bufferDraw.DrawBitmap( eventImage, 0, 0, true );

  if( myWindow->getDrawCommLines() )
    bufferDraw.DrawBitmap( commImage, 0, 0, true );

  drawZone->Refresh();
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHECK_DRAWLINES
 */

void gTimeline::OnCheckDrawlinesUpdate( wxUpdateUIEvent& event )
{
  event.Check( myWindow->getDrawCommLines() );
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECK_DRAWFLAGS
 */

void gTimeline::OnCheckDrawflagsClick( wxCommandEvent& event )
{
  wxMemoryDC bufferDraw;

  myWindow->setDrawFlags( event.IsChecked() );
  
  if( !ready )
    return;
  
  bufferDraw.SelectObject(wxNullBitmap);
  bufferDraw.SelectObject( drawImage );
  bufferDraw.DrawBitmap( bufferImage, 0, 0, false );

  if( drawCaution )
  {
    wxBitmap cautionImage( caution_xpm );
    bufferDraw.DrawBitmap( cautionImage,
                           drawBorder,
                           drawZone->GetSize().GetHeight() - cautionImage.GetHeight() - drawBorder,
                           true );
  }

  if( myWindow->getDrawFlags() )
    bufferDraw.DrawBitmap( eventImage, 0, 0, true );

  if( myWindow->getDrawCommLines() )
    bufferDraw.DrawBitmap( commImage, 0, 0, true );

  drawZone->Refresh();
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHECK_DRAWFLAGS
 */

void gTimeline::OnCheckDrawflagsUpdate( wxUpdateUIEvent& event )
{
  event.Check( myWindow->getDrawFlags() );
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECK_FUNCTIONLINECOLOR
 */

void gTimeline::OnCheckFunctionlinecolorClick( wxCommandEvent& event )
{
  myWindow->setDrawFunctionLineColor( event.IsChecked() );
  myWindow->setRedraw( true );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHECK_FUNCTIONLINECOLOR
 */

void gTimeline::OnCheckFunctionlinecolorUpdate( wxUpdateUIEvent& event )
{
  event.Check( myWindow->getDrawFunctionLineColor() );
}

