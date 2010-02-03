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
//#include "paraverconfig.h"
//#include "paraverkerneltypes.h"

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

  EVT_CHECKBOX( ID_CHECKBOX, gTimeline::OnCheckWhatWhere )

  EVT_CHECKBOX( ID_CHECKBOX1, gTimeline::OnCheckWhatWhere )

  EVT_CHECKBOX( ID_CHECKBOX2, gTimeline::OnCheckWhatWhere )

  EVT_CHECKBOX( ID_CHECKBOX3, gTimeline::OnCheckWhatWhere )

  EVT_CHECKBOX( ID_CHECKBOX4, gTimeline::OnCheckWhatWhereText )

  EVT_UPDATE_UI( ID_PANEL1, gTimeline::OnColorsPanelUpdate )

  EVT_CHECKBOX( ID_CHECK_DRAWLINES, gTimeline::OnCheckDrawlinesClick )
  EVT_UPDATE_UI( ID_CHECK_DRAWLINES, gTimeline::OnCheckDrawlinesUpdate )

  EVT_CHECKBOX( ID_CHECK_DRAWFLAGS, gTimeline::OnCheckDrawflagsClick )
  EVT_UPDATE_UI( ID_CHECK_DRAWFLAGS, gTimeline::OnCheckDrawflagsUpdate )

  EVT_CHECKBOX( ID_CHECK_FUNCTIONLINECOLOR, gTimeline::OnCheckFunctionlinecolorClick )
  EVT_UPDATE_UI( ID_CHECK_FUNCTIONLINECOLOR, gTimeline::OnCheckFunctionlinecolorUpdate )

////@end gTimeline event table entries

  EVT_TIMER( wxID_ANY, gTimeline::OnTimerSize )
  
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
  splitter->Unsplit();
  return true;
}


/*!
 * gTimeline destructor
 */

gTimeline::~gTimeline()
{
////@begin gTimeline destruction
////@end gTimeline destruction
  delete timerSize;
  delete myWindow;
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
  splitChanged = false;
  timerSize = new wxTimer( this );
  pixelSize = 1;
  infoZoneLastSize = 200;
  splitter = NULL;
  drawZone = NULL;
  infoZone = NULL;
  whatWherePanel = NULL;
  checkWWSemantic = NULL;
  checkWWEvents = NULL;
  checkWWCommunications = NULL;
  checkWWPreviousNext = NULL;
  checkWWText = NULL;
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
  whatWhereTime = 0.0;
  whatWhereRow = 0;

  // Avoid manipulation
  UINT32 pixelSizeIndexSelector = ParaverConfig::getInstance()->getTimelinePixelSize();
  if ( pixelSizeIndexSelector >= 0 && pixelSizeIndexSelector <= 3 )
    pixelSize = UINT32( floor ( pow( 2, (double)pixelSizeIndexSelector )));
}


/*!
 * Control creation for gTimeline
 */

void gTimeline::CreateControls()
{
////@begin gTimeline content construction
  gTimeline* itemFrame1 = this;

  splitter = new wxSplitterWindow( itemFrame1, ID_SPLITTERWINDOW, wxDefaultPosition, wxDefaultSize, wxSP_BORDER|wxSP_3DSASH|wxSP_PERMIT_UNSPLIT );
  splitter->SetMinimumPaneSize(0);

  drawZone = new wxScrolledWindow( splitter, ID_SCROLLEDWINDOW, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxFULL_REPAINT_ON_RESIZE|wxHSCROLL|wxVSCROLL );
  drawZone->SetScrollbars(1, 1, 0, 0);
  infoZone = new wxNotebook( splitter, ID_NOTEBOOK, wxDefaultPosition, wxSize(-1, splitter->ConvertDialogToPixels(wxSize(-1, 50)).y), wxBK_DEFAULT );

  whatWherePanel = new wxScrolledWindow( infoZone, ID_SCROLLEDWINDOW2, wxDefaultPosition, infoZone->ConvertDialogToPixels(wxSize(100, 100)), wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
  whatWherePanel->SetScrollbars(1, 1, 0, 0);
  wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxVERTICAL);
  whatWherePanel->SetSizer(itemBoxSizer6);

  wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer6->Add(itemBoxSizer7, 0, wxALIGN_LEFT|wxALL, 5);
  checkWWSemantic = new wxCheckBox( whatWherePanel, ID_CHECKBOX, _("Semantic"), wxDefaultPosition, wxDefaultSize, 0 );
  checkWWSemantic->SetValue(true);
  itemBoxSizer7->Add(checkWWSemantic, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

  checkWWEvents = new wxCheckBox( whatWherePanel, ID_CHECKBOX1, _("Events"), wxDefaultPosition, wxDefaultSize, 0 );
  checkWWEvents->SetValue(true);
  itemBoxSizer7->Add(checkWWEvents, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

  checkWWCommunications = new wxCheckBox( whatWherePanel, ID_CHECKBOX2, _("Communications"), wxDefaultPosition, wxDefaultSize, 0 );
  checkWWCommunications->SetValue(true);
  itemBoxSizer7->Add(checkWWCommunications, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

  checkWWPreviousNext = new wxCheckBox( whatWherePanel, ID_CHECKBOX3, _("Previous / Next"), wxDefaultPosition, wxDefaultSize, 0 );
  checkWWPreviousNext->SetValue(true);
  itemBoxSizer7->Add(checkWWPreviousNext, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

  checkWWText = new wxCheckBox( whatWherePanel, ID_CHECKBOX4, _("Text"), wxDefaultPosition, wxDefaultSize, 0 );
  checkWWText->SetValue(true);
  itemBoxSizer7->Add(checkWWText, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

  whatWhereText = new wxRichTextCtrl( whatWherePanel, ID_RICHTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxWANTS_CHARS );
  itemBoxSizer6->Add(whatWhereText, 1, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

  whatWherePanel->FitInside();
  infoZone->AddPage(whatWherePanel, _("What / Where"));

  timingZone = new wxScrolledWindow( infoZone, ID_PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  timingZone->SetScrollbars(1, 5, 0, 0);
  wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
  timingZone->SetSizer(itemBoxSizer15);

  wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer15->Add(itemBoxSizer16, 0, wxGROW, 5);
  wxStaticText* itemStaticText17 = new wxStaticText( timingZone, wxID_STATIC, _("Initial time"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer16->Add(itemStaticText17, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 10);

  wxStaticText* itemStaticText18 = new wxStaticText( timingZone, wxID_STATIC, _("Final time"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer16->Add(itemStaticText18, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 10);

  wxStaticText* itemStaticText19 = new wxStaticText( timingZone, wxID_STATIC, _("Duration"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer16->Add(itemStaticText19, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 10);

  wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer15->Add(itemBoxSizer20, 1, wxGROW, 5);
  initialTimeText = new wxTextCtrl( timingZone, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer20->Add(initialTimeText, 0, wxGROW|wxALL, 5);

  finalTimeText = new wxTextCtrl( timingZone, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer20->Add(finalTimeText, 0, wxGROW|wxALL, 5);

  durationText = new wxTextCtrl( timingZone, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer20->Add(durationText, 0, wxGROW|wxALL, 5);

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
  wxBoxSizer* itemBoxSizer27 = new wxBoxSizer(wxVERTICAL);
  viewPropPanel->SetSizer(itemBoxSizer27);

  checkDrawLines = new wxCheckBox( viewPropPanel, ID_CHECK_DRAWLINES, _("Communication Lines"), wxDefaultPosition, wxDefaultSize, 0 );
  checkDrawLines->SetValue(true);
  itemBoxSizer27->Add(checkDrawLines, 0, wxALIGN_LEFT|wxALL, 5);

  checkDrawFlags = new wxCheckBox( viewPropPanel, ID_CHECK_DRAWFLAGS, _("Event Flags"), wxDefaultPosition, wxDefaultSize, 0 );
  checkDrawFlags->SetValue(true);
  itemBoxSizer27->Add(checkDrawFlags, 0, wxALIGN_LEFT|wxALL, 5);

  checkFunctionLineColor = new wxCheckBox( viewPropPanel, ID_CHECK_FUNCTIONLINECOLOR, _("Function Line With Color"), wxDefaultPosition, wxDefaultSize, 0 );
  checkFunctionLineColor->SetValue(true);
  itemBoxSizer27->Add(checkFunctionLineColor, 0, wxALIGN_LEFT|wxALL, 5);

  viewPropPanel->FitInside();
  infoZone->AddPage(viewPropPanel, _("View"));

  splitter->SplitHorizontally(drawZone, infoZone, 0);

  // Connect events and objects
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_SIZE, wxSizeEventHandler(gTimeline::OnScrolledWindowSize), NULL, this);
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_PAINT, wxPaintEventHandler(gTimeline::OnScrolledWindowPaint), NULL, this);
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(gTimeline::OnScrolledWindowEraseBackground), NULL, this);
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_LEFT_DOWN, wxMouseEventHandler(gTimeline::OnScrolledWindowLeftDown), NULL, this);
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_LEFT_UP, wxMouseEventHandler(gTimeline::OnScrolledWindowLeftUp), NULL, this);
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_MIDDLE_UP, wxMouseEventHandler(gTimeline::OnScrolledWindowMiddleUp), NULL, this);
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_RIGHT_DOWN, wxMouseEventHandler(gTimeline::OnScrolledWindowRightDown), NULL, this);
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_MOTION, wxMouseEventHandler(gTimeline::OnScrolledWindowMotion), NULL, this);
////@end gTimeline content construction

  ParaverConfig *paraverConfig = ParaverConfig::getInstance();

  checkWWSemantic->SetValue( paraverConfig->getTimelineWhatWhereSemantic() );
  checkWWCommunications->SetValue( paraverConfig->getTimelineWhatWhereCommunications() );
  checkWWEvents->SetValue( paraverConfig->getTimelineWhatWhereEvents() );
  checkWWPreviousNext->SetValue( paraverConfig->getTimelineWhatWherePreviousNext() );
  checkWWText->SetValue( paraverConfig->getTimelineWhatWhereText() );
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
#ifndef WIN32
  if( splitChanged )
  {
    resizeDrawZone( myWindow->getWidth(), myWindow->getHeight() );
    splitChanged = false;
    return;
  }
#endif

  redoColors = true;

  rgb rgbForegroundColour = ((paraverMain *)parent)->GetParaverConfig()->getColorsTimelineAxis();
  wxColour foregroundColour = wxColour( rgbForegroundColour.red,
                                       rgbForegroundColour.green,
                                       rgbForegroundColour.blue );
  rgb rgbBackgroundColour = ((paraverMain *)parent)->GetParaverConfig()->getColorsTimelineBackground();
  wxColour backgroundColour = wxColour( rgbBackgroundColour.red,
                                       rgbBackgroundColour.green,
                                       rgbBackgroundColour.blue );

  wxString winTitle = GetTitle();
  SetTitle( _("(Working...) ") + winTitle );

  // Get selected rows
  vector<bool>         selected;
  vector<TObjectOrder> selectedSet;
  TObjectOrder beginRow = myWindow->getZoomSecondDimension().first;
  TObjectOrder endRow =  myWindow->getZoomSecondDimension().second;
  myWindow->getSelectedRows( myWindow->getLevel(), selected, true );
  myWindow->getSelectedRows( myWindow->getLevel(), selectedSet, beginRow, endRow, true );
  TObjectOrder maxObj = selectedSet[ selectedSet.size() - 1 ];

  ready = false;
  bufferImage.Create( drawZone->GetClientSize().GetWidth(), drawZone->GetClientSize().GetHeight() );
  drawImage.Create( drawZone->GetClientSize().GetWidth(), drawZone->GetClientSize().GetHeight() );
  commImage.Create( drawZone->GetClientSize().GetWidth(), drawZone->GetClientSize().GetHeight() );
  eventImage.Create( drawZone->GetClientSize().GetWidth(), drawZone->GetClientSize().GetHeight() );
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
  commMask.Create( drawZone->GetClientSize().GetWidth(), drawZone->GetClientSize().GetHeight(), 1 );
  wxMemoryDC commmaskdc( commMask );
  commmaskdc.SetBackground( *wxBLACK_BRUSH );
  commmaskdc.SetPen( wxPen( wxColour( 255, 255, 255 ), 1 ) );
  commmaskdc.Clear();
  wxBitmap eventMask;
  eventMask.Create( drawZone->GetClientSize().GetWidth(), drawZone->GetClientSize().GetHeight(), 1 );
  wxMemoryDC eventmaskdc( eventMask );
  eventmaskdc.SetBackground( *wxBLACK_BRUSH );
  eventmaskdc.SetPen( wxPen( wxColour( 255, 255, 255 ), 1 ) );
  eventmaskdc.Clear();

  bufferDraw.SetBackground( wxBrush( backgroundColour ) );
  bufferDraw.Clear();
  
#ifndef WIN32
  // Paint blank image while redrawing
  wxClientDC dc( drawZone );
  dc.DrawBitmap( bufferImage, 0, 0, false );
  drawZone->Update();
#endif
  
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
    //bufferDraw.SetPen( *wxBLACK_PEN );
    bufferDraw.SetPen( wxPen( backgroundColour ) );
    //bufferDraw.SetBrush( *wxBLACK_BRUSH );
    bufferDraw.SetBrush( wxBrush( backgroundColour ) );
    bufferDraw.DrawRectangle( 0, drawZone->GetClientSize().GetHeight() - cautionImage.GetHeight() - drawBorder - 2,
                              drawBorder + cautionImage.GetWidth() + 2, drawZone->GetClientSize().GetHeight() );
    bufferDraw.DrawBitmap( cautionImage,
                           drawBorder,
                           drawZone->GetClientSize().GetHeight() - cautionImage.GetHeight() - drawBorder,
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
  // UINT32 precision = ParaverConfig::getInstance()->getTimelinePrecision();
  UINT32 precision = 0;

  size_t numObjects = selected.size();
  float magnify = float( GetPixelSize() );
  
  rgb rgbAxisColour = ((paraverMain *)parent)->GetParaverConfig()->getColorsTimelineAxis();
  wxColour axisColour = wxColour( rgbAxisColour.red, rgbAxisColour.green ,rgbAxisColour.blue );

  dc.SetPen( wxPen( axisColour, 1 ) );
  dc.SetTextForeground( axisColour );

  // Get the text extent for time label
  dc.SetFont( timeFont );
  wxSize timeExt = dc.GetTextExtent( wxString::FromAscii( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( myWindow->getWindowBeginTime() ),
                                                                                       myWindow->getTimeUnit(), precision ).c_str() ) );
  timeAxisPos = dc.GetSize().GetHeight() - ( drawBorder + timeExt.GetHeight() + drawBorder );

  // Get the text extent for the last object (probably the larger one)
  dc.SetFont( objectFont );
  // +1!
  wxSize objectExt = dc.GetTextExtent( wxString::FromAscii( LabelConstructor::objectLabel( myWindow->getWindowLevelObjects() - 1,
                                                                                           myWindow->getLevel(),
                                                                                           myWindow->getTrace() ).c_str() ) );
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
    dc.DrawText( wxString::FromAscii( LabelConstructor::objectLabel( *it, myWindow->getLevel(), myWindow->getTrace() ).c_str() ),
                 drawBorder, y );

    // next selected row
    ++it;
  }

  if( numObjects == 1 )
    objectHeight = ( timeAxisPos - objectPosList[ selected[ 0 ] ] );
  else
  {
    if( ( inc * 0.25 ) < 1.0 && magnify > objectHeight )
    {
      for( vector<wxCoord>::iterator it = objectPosList.begin(); it != objectPosList.end(); ++it )
        *it = ( floor( ( *it - drawBorder ) / magnify ) * magnify ) + drawBorder;
      objectHeight = magnify;
    }
  }

  dc.SetFont( timeFont );
  dc.DrawText( wxString::FromAscii( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( myWindow->getWindowBeginTime() ),
                                                                 myWindow->getTimeUnit(), precision ).c_str() ),
               objectAxisPos, timeAxisPos + drawBorder );
  dc.DrawText( wxString::FromAscii( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( myWindow->getWindowEndTime() ),
                                                                 myWindow->getTimeUnit(), precision ).c_str() ),
               dc.GetSize().GetWidth() -
               ( dc.GetTextExtent( wxString::FromAscii( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( myWindow->getWindowEndTime() ),
                                                                                     myWindow->getTimeUnit(), precision ).c_str() ) )
               .GetWidth() + drawBorder ),
               timeAxisPos + drawBorder );
}


void gTimeline::drawRow( wxDC& dc, wxMemoryDC& commdc, wxDC& commmaskdc,
                         wxMemoryDC& eventdc, wxDC& eventmaskdc,
                         TObjectOrder firstRow, TObjectOrder lastRow,
                         vector<TObjectOrder>& selectedSet, vector<bool>& selected )
{
  float magnify = float( GetPixelSize() );

  rgb rgbForegroundColour = ((paraverMain *)parent)->GetParaverConfig()->getColorsTimelineAxis();
  wxColour foregroundColour = wxColour( rgbForegroundColour.red,
                                        rgbForegroundColour.green,
                                        rgbForegroundColour.blue );

  TTime timeStep = (( myWindow->getWindowEndTime() - myWindow->getWindowBeginTime() )  * magnify) /
                   ( dc.GetSize().GetWidth() - objectAxisPos - drawBorder );

  vector<TSemanticValue> timeValues;
  vector<TSemanticValue> rowValues;
  wxCoord timePos = objectAxisPos + 1;

  vector<TObjectOrder>::iterator first = find( selectedSet.begin(), selectedSet.end(), firstRow );
  vector<TObjectOrder>::iterator last  = find( selectedSet.begin(), selectedSet.end(), lastRow );

  wxCoord objectPos = objectPosList[ firstRow ];
  int lineLastPos = 0;
  
  for( TTime currentTime = myWindow->getWindowBeginTime() + timeStep;
       currentTime <= myWindow->getWindowEndTime() && currentTime <= myWindow->getTrace()->getEndTime();
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
        if( currentValue != 0 && ( currentValue < myWindow->getMinimumY() 
                                   || currentValue > myWindow->getMaximumY() ) )
          drawCaution = true;
      }
      rowValues.push_back( DrawMode::selectValue( timeValues, myWindow->getDrawModeTime() ) );

      RecordList *rl = myWindow->getRecordList( *row );
      if( rl != NULL )
        drawRecords( commdc, commmaskdc, eventdc, eventmaskdc,
                     rl, currentTime - timeStep, currentTime, timeStep / magnify, timePos, selected );
    }
    TSemanticValue valueToDraw = DrawMode::selectValue( rowValues, myWindow->getDrawModeObject() );
    if( myWindow->getDrawFunctionLineColor() )
    {
      rgb colorToDraw = myWindow->calcColor( valueToDraw, *myWindow );
      dc.SetPen( wxPen( wxColour( colorToDraw.red, colorToDraw.green, colorToDraw.blue ) ) );

      if ( magnify == 1.0 ) // same than before
      {
        if( objectPos + objectHeight < timeAxisPos )
          dc.DrawLine( timePos, objectPos, timePos, objectPos + objectHeight );
        else
          dc.DrawLine( timePos, objectPos, timePos, timeAxisPos - 1 );
      }
      else
      {
        // Draw a rectangle and fill with same color
        dc.SetBrush( wxBrush( wxColour( colorToDraw.red, colorToDraw.green, colorToDraw.blue ) ) );
        //if( objectPos + objectHeight < timeAxisPos )
          dc.DrawRectangle( timePos , objectPos, magnify , objectHeight );
        /*else
          dc.DrawRectangle( timePos , objectPos, magnify, objectPos + objectHeight - timeAxisPos + 1 );*/
      }
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
      
      dc.SetPen( foregroundColour );
      if( currentPos != lineLastPos )
      {
        int from = ( currentPos > lineLastPos ) ? currentPos : lineLastPos;
        int to   = ( currentPos < lineLastPos ) ? currentPos : lineLastPos;
        dc.DrawLine( timePos, objectPos + objectHeight - from,
                     timePos, objectPos + objectHeight - to + 1 );

        if( magnify > 1.0 )
          dc.DrawLine( timePos,            objectPos + objectHeight - currentPos,
                       timePos + magnify, objectPos + objectHeight - currentPos ); 
      }
      else
      {
        if ( magnify == 1.0 ) // same than before
          dc.DrawPoint( timePos, objectPos + objectHeight - currentPos );
        else
          dc.DrawLine( timePos,            objectPos + objectHeight - currentPos,
                       timePos + magnify, objectPos + objectHeight - currentPos ); 
      }

      lineLastPos = currentPos;
    }
    timePos += (int) magnify ;
  }
}


void gTimeline::drawRecords( wxMemoryDC& commdc, wxDC& commmaskdc,
                             wxMemoryDC& eventdc, wxDC& eventmaskdc,
                             RecordList *records,
                             TTime from, TTime to, TTime step, wxCoord pos, vector<bool>& selected )
{
  bool existEvents = false;
  TObjectOrder row = 0;

  rgb rgbLogicalColour = ((paraverMain *)parent)->GetParaverConfig()->getColorsTimelineLogicalCommunications();
  wxColour logicalColour = wxColour( rgbLogicalColour.red, rgbLogicalColour.green ,rgbLogicalColour.blue );

  rgb rgbPhysicalColour = ((paraverMain *)parent)->GetParaverConfig()->getColorsTimelinePhysicalCommunications();
  wxColour physicalColour = wxColour( rgbPhysicalColour.red, rgbPhysicalColour.green ,rgbPhysicalColour.blue );

  rgb rgbForegroundColour = ((paraverMain *)parent)->GetParaverConfig()->getColorsTimelineAxis();
  wxColour foregroundColour = wxColour( rgbForegroundColour.red,
                                       rgbForegroundColour.green,
                                       rgbForegroundColour.blue );
  rgb rgbBackgroundColour = ((paraverMain *)parent)->GetParaverConfig()->getColorsTimelineBackground();
  wxColour backgroundColour = wxColour( rgbBackgroundColour.red,
                                       rgbBackgroundColour.green,
                                       rgbBackgroundColour.blue );

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
          ( ( it->getType() & RECV ) ||
            ( ( it->getType() & SEND ) && it->getCommPartnerTime() > myWindow->getWindowEndTime() ) )
        )
      {
        if( it->getType() & LOG )
          commdc.SetPen( wxPen( logicalColour ) );
        else if( it->getType() & PHY )
          commdc.SetPen(  wxPen( physicalColour ) );
        wxCoord posPartner = (wxCoord)( ( it->getCommPartnerTime() - myWindow->getWindowBeginTime() ) * step );
        posPartner += objectAxisPos;
        if( posPartner > 10000 )
          posPartner = 10000;
        if( posPartner < -10000 )
          posPartner = -10000;
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
    eventdc.SetTextBackground( backgroundColour );
    eventdc.SetBackgroundMode( wxTRANSPARENT );
    wxBitmap imgFlag( flag, 10, 10 );
    eventdc.DrawBitmap( imgFlag, pos, objectPosList[ row ] - 10, true );
    eventmaskdc.SetPen( *wxWHITE_PEN );
    eventmaskdc.SetBrush( *wxWHITE_BRUSH );
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
  
  if( !ready )
    return;
  
  dc.DrawBitmap( drawImage, 0, 0, false );
}


/*!
 * wxEVT_SIZE event handler for ID_GTIMELINE
 */
void gTimeline::OnScrolledWindowSize( wxSizeEvent& event )
{
  if( !IsShown() )
    return;
  else if( canRedraw &&
           ( event.GetSize().GetWidth() != myWindow->getWidth() ||
             event.GetSize().GetHeight() != myWindow->getHeight() ) )
  {
    if( !splitChanged )
    {
      myWindow->setWidth( event.GetSize().GetWidth() );
      myWindow->setHeight( event.GetSize().GetHeight() );
    }
    timerSize->Start( 100, true );
  }
//  event.Skip();
}


/*!
 * wxEVT_IDLE event handler for ID_GTIMELINE
 */
void gTimeline::OnIdle( wxIdleEvent& event )
{
  if( myWindow->getDestroy() )
    return;

  string composedName = myWindow->getName() + " @ " +
                        myWindow->getTrace()->getTraceNameNumbered();

  this->SetTitle( wxString::FromAscii( composedName.c_str() ) );

#ifdef WIN32
  if( !firstUnsplit )
  {
    firstUnsplit = true;
    splitter->Unsplit();
//    this->SetSize( myWindow->getWidth(), myWindow->getHeight() );
    drawZone->SetClientSize( myWindow->getWidth(), myWindow->getHeight() );
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
//    paraverMain::myParaverMain->addActiveWindow( this );
    if( !ready )
      redraw();
  }
  else
  {
    this->Show( false );
//    paraverMain::myParaverMain->removeActiveWindow( this );
  }

  if( myWindow->getRaiseWindow() )
  {
    if( !myWindow->getShowWindow() )
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
  bool outOfDraw = false;

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
  {
    zoomEndX = 0;
    outOfDraw = true;
  }
  else if( zoomEndX > dc.GetSize().GetWidth() - drawBorder )
  {
    zoomEndX = dc.GetSize().GetWidth() - drawBorder - objectAxisPos;
    outOfDraw = true;
  }
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
  {
    zoomEndY = timeAxisPos;
    outOfDraw = true;
  }
  if( zoomEndY < drawBorder )
  {
    zoomEndY = drawBorder;
    outOfDraw = true;
  }

  vector<TObjectOrder> selected;
  myWindow->getSelectedRows( myWindow->getLevel(), selected, beginRow, endRow, true );
  TObjectOrder numObjects = selected.size();
  double heightPerRow = (double)( timeAxisPos - drawBorder - 1 ) / (double)numObjects;
  beginRow = TObjectOrder( floor( (zoomBeginY - drawBorder - 1) / heightPerRow ) );
  endRow = TObjectOrder( floor( (zoomEndY - drawBorder - 1) / heightPerRow ) );

  if( endRow >= numObjects )
    endRow = numObjects - 1;
  if( beginRow >= numObjects )
    beginRow = numObjects - 1;
    
  beginRow = selected[ beginRow ];
  endRow   = selected[ endRow ];

  if( zooming && ready &&
      ( zoomEndX - zoomBeginX > 5.0 || zoomBeginY != zoomEndY ) )
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
  else if( !outOfDraw )
  {
    if( !splitter->IsSplit() )
    {
      Split();
    }
    whatWhereText->Clear();
    whatWhereText->AppendText( _( "Working..." ) );
    Update();
    computeWhatWhere( endTime, endRow, checkWWText->IsChecked() );
    printWhatWhere();
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

  string composedName = myWindow->getName() + " @ " +
                        myWindow->getTrace()->getTraceNameNumbered();

//  gTimeline *clonedTimeline = new gTimeline( parent, wxID_ANY, wxT( myWindow->getName().c_str() ), position, size );
  gTimeline *clonedTimeline = new gTimeline( parent, wxID_ANY, wxString::FromAscii( composedName.c_str() ), position, size );
  clonedTimeline->SetMyWindow( clonedWindow );
  clonedTimeline->SetPixelSize( GetPixelSize() );

  // add to loaded windows list
  LoadedWindows::getInstance()->add( clonedWindow );

  wxChoicebook *choiceWindowBrowser = paraverMain::myParaverMain->choiceWindowBrowser;
  INT16 currentTrace = paraverMain::myParaverMain->getTracePosition( clonedWindow->getTrace() );
  wxTreeCtrl *allTracesPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 ); // Global page
  wxTreeCtrl *currentPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( currentTrace + 1 ); // Current page

  TreeBrowserItemData *currentData =  new TreeBrowserItemData( wxString::FromAscii( clonedWindow->getName().c_str() ), clonedTimeline );
  wxTreeItemId currentWindowId1 = allTracesPage->AppendItem( idRoot1, wxString::FromAscii( clonedWindow->getName().c_str() ), 1, -1, currentData );
  wxTreeItemId currentWindowId2 = currentPage->AppendItem( idRoot2, wxString::FromAscii( clonedWindow->getName().c_str() ), 1, -1, new TreeBrowserItemData( *currentData ) );

  // if derived, clone parents
  if ( clonedWindow->isDerivedWindow() )
  {
//    vector< gTimeline * > gParents;
//    getParentGTimeline( this, gParents );

    clone( clonedWindow->getParent( 0 ), parent, currentWindowId1, currentWindowId2, mustRedraw );
    clone( clonedWindow->getParent( 1 ), parent, currentWindowId1, currentWindowId2, mustRedraw );
    
    allTracesPage->CollapseAllChildren( currentWindowId1 );
    currentPage->CollapseAllChildren( currentWindowId2 );
  }

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
  {
    clonedWindow->setShowWindow( false );
    clonedTimeline->Show( false );
  }

  return clonedTimeline;
}


void gTimeline::OnPopUpClone()
{
  clone( NULL, parent, getAllTracesTree()->GetRootItem(), getSelectedTraceTree( myWindow->getTrace() )->GetRootItem());
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

void gTimeline::OnPopUpPasteSemanticScale()
{
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Semantic Scale" );
  myWindow->setRedraw( true );
  myWindow->setChanged( true );
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
  RowsSelectionDialog *dialog = gPopUpMenu::createRowSelectionDialog( this );

  if ( dialog->ShowModal() == wxID_OK )
  {
    TWindowLevel beginLevel;
    TWindowLevel endLevel;

    // Set range of levels for update loop
    if (( myWindow->getLevel() >= WORKLOAD ) && ( myWindow->getLevel() <= THREAD ))
    {
      beginLevel = APPLICATION;
      endLevel = THREAD;
    }
    else
    {
      beginLevel = NODE;
      endLevel = CPU;
    }

    // Loop through levels to update gTimeline
    bool refresh;
    for ( TWindowLevel whichLevel = beginLevel; whichLevel <= endLevel; whichLevel = TWindowLevel(whichLevel + 1) )
    {
      wxArrayInt selections;
      int numberSelected = dialog->GetSelections( whichLevel, selections );
      if ( numberSelected > 0 )
      {
        // Get new selections for that level
        vector< TObjectOrder > newSelection;
        for ( size_t row = (size_t)0; row < (size_t)numberSelected; row++ )
        {
          newSelection.push_back( (TObjectOrder)selections[ row ] );
        }
        // Do we need to update?
        vector< TObjectOrder > previousSelection;
        myWindow->getSelectedRows( whichLevel, previousSelection, true );

        if ( ( previousSelection.size() != newSelection.size() ) ||
             !equal( previousSelection.begin(), previousSelection.end(), newSelection.begin() ) )
        {
          // Update
          myWindow->setSelectedRows( whichLevel, newSelection );

          // and do not refresh for minor levels
          if ( whichLevel <= myWindow->getLevel() )
          {
            refresh = true;
          }
        }
      }
    }

    if ( refresh )
    {
      myWindow->setRedraw( true );
      myWindow->setChanged( true );
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


void gTimeline::OnPopUpPixelSize( UINT32 whichPixelSize )
{
  SetPixelSize( whichPixelSize );
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
  // UINT32 precision = ParaverConfig::getInstance()->getTimelinePrecision();
  UINT32 precision = 0;

  rgb rgbForegroundColour = ((paraverMain *)parent)->GetParaverConfig()->getColorsTimelineAxis();
  wxColour foregroundColour = wxColour( rgbForegroundColour.red,
                                       rgbForegroundColour.green,
                                       rgbForegroundColour.blue );
  rgb rgbBackgroundColour = ((paraverMain *)parent)->GetParaverConfig()->getColorsTimelineBackground();
  wxColour backgroundColour = wxColour( rgbBackgroundColour.red,
                                       rgbBackgroundColour.green,
                                       rgbBackgroundColour.blue );

  TTime timeStep = ( myWindow->getWindowEndTime() - myWindow->getWindowBeginTime() ) /
                   ( dc.GetSize().GetWidth() - objectAxisPos - drawBorder );

  if( zooming )
  {
    if( !event.ShiftDown() )
      zoomXY = event.ControlDown();
    else
      zoomXY = false;

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

    if( endX - beginX > 3.0 )
    {
      wxMemoryDC memdc( drawImage );
      memdc.SetBackgroundMode( wxTRANSPARENT );
      memdc.SetBackground( *wxTRANSPARENT_BRUSH );
      memdc.Clear();
#if wxTEST_GRAPHICS == 1
      wxGCDC dc( memdc );
      dc.SetBrush( wxBrush( wxColour( 255, 255, 255, 80 ) ) );
#else
      wxDC& dc = memdc;
      dc.SetBrush( *wxTRANSPARENT_BRUSH );
#endif
      // dc.SetPen( *wxWHITE_PEN );
      dc.SetPen( wxPen( foregroundColour ) );

      dc.DrawBitmap( bufferImage, 0, 0, false );
      if( myWindow->getDrawFlags() )
        dc.DrawBitmap( eventImage, 0, 0, true );
      if( myWindow->getDrawCommLines() )
        dc.DrawBitmap( commImage, 0, 0, true );
      dc.DrawRectangle( beginX, beginY, width, height );

      drawZone->Refresh();
    }

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

    initialTimeText->SetValue( wxString::FromAscii( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( beginTime ),
                                                                                 myWindow->getTimeUnit(), precision ).c_str() ) );
    finalTimeText->SetValue( wxString::FromAscii( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( endTime ),
                                                                               myWindow->getTimeUnit(), precision ).c_str() ) );
    durationText->SetValue( wxString::FromAscii( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( endTime - beginTime ),
                                                                              myWindow->getTimeUnit(), precision ).c_str() ) );
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

    initialTimeText->SetValue( wxString::FromAscii( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( time ), myWindow->getTimeUnit(), precision ).c_str() ) );
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
    //splitter->SplitHorizontally( drawZone, infoZone, myWindow->getHeight() );
    Split();
  }
//  drawZone->SetClientSize( myWindow->getWidth(), myWindow->getHeight() );
  canRedraw = true;
}


// Computes What/Where, filling whatWhereLines vector. Doesn't show it --> printWhatWhere.
void gTimeline::computeWhatWhere( TRecordTime whichTime, TObjectOrder whichRow, bool textMode )
{
  whatWhereTime = whichTime;
  whatWhereRow = whichRow;

  whatWhereLines.clear();
  whatWhereSelectedTimeEventLines = 0;
  whatWhereSelectedTimeCommunicationLines = 0;

  whatWhereLines.push_back( make_pair( BEGIN_OBJECT_SECTION, _( "" )));

  wxString txt;

  txt << _( "Object: " ) << wxString::FromAscii( LabelConstructor::objectLabel( whichRow, myWindow->getLevel(), myWindow->getTrace() ).c_str() );
  txt << _( "\t  Click time: " ) << wxString::FromAscii( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( whichTime ),
                                                                                      myWindow->getTimeUnit(), 0 ).c_str() );
  txt << _( "\n" );
  whatWhereLines.push_back( make_pair( RAW_LINE, txt ) );

  whatWhereLines.push_back( make_pair( END_OBJECT_SECTION, _( "" )));

  myWindow->init( whichTime, CREATEEVENTS + CREATECOMMS, false );

  TRecordTime tmpBeginTime = myWindow->getBeginTime( whichRow );

  if ( tmpBeginTime > 0.0 )
  {
    myWindow->getRecordList( whichRow )->erase( myWindow->getRecordList( whichRow )->begin(),
                                                myWindow->getRecordList( whichRow )->end() );

    --tmpBeginTime;
    myWindow->init( tmpBeginTime, CREATEEVENTS + CREATECOMMS, false );

    printWWSemantic( whichRow, false, textMode );
    printWWRecords( whichRow, false, textMode );
    myWindow->calcNext( whichRow, false );
  }

  printWWSemantic( whichRow, true, textMode );
  printWWRecords( whichRow, true, textMode );

  myWindow->calcNext( whichRow, false );
  printWWSemantic( whichRow, false, textMode );
  printWWRecords( whichRow, false, textMode );
}


void gTimeline::printWhatWhere( )
{
  int fontSize = 8;
  bool allowedLine, allowedSection = false;
  int recordsCount = 0;

  whatWhereText->Clear();

  whatWhereText->BeginFontSize( fontSize );

  for ( vector< pair< TWWLine, wxString > >::iterator it = whatWhereLines.begin(); 
        it != whatWhereLines.end(); ++it )
  {
    // Is that section allowed?
    switch( it->first )
    {
      case BEGIN_OBJECT_SECTION:
        allowedSection = true;
        break;
      case BEGIN_PREVNEXT_SECTION:
        allowedSection = checkWWPreviousNext->IsChecked();
        whatWhereText->BeginTextColour( wxColour( 0xb0b0b0 ) ); // GREY
        break;
      case END_PREVNEXT_SECTION:
        whatWhereText->EndTextColour();
        break;
      case BEGIN_CURRENT_SECTION:
        allowedSection = true;
        whatWhereText->BeginTextColour( *wxBLACK );
        break;
      case END_CURRENT_SECTION:
        whatWhereText->EndTextColour();
        break;
      case BEGIN_RECORDS_SECTION:
        allowedSection = checkWWEvents->IsChecked() || checkWWCommunications->IsChecked();
        whatWhereText->BeginFontSize( fontSize - 1 );
        break;
      case END_RECORDS_SECTION:
        whatWhereText->EndFontSize();
        break;
      case BEGIN_SEMANTIC_SECTION:
        allowedSection = checkWWSemantic->IsChecked();
        whatWhereText->BeginBold();
        break;
      case END_SEMANTIC_SECTION:
        whatWhereText->EndBold();
        break;

      default:
        if ( allowedSection )
        {
          // Is that line allowed?
          switch( it->first )
          {
            case EVENT_LINE:
              if( recordsCount >= 100 )
                allowedLine = false;
              else
                allowedLine = checkWWEvents->IsChecked();
              ++recordsCount;
              break;
            case COMMUNICATION_LINE:
              if( recordsCount >= 100 )
                allowedLine = false;
              else
                allowedLine = checkWWCommunications->IsChecked();
              ++recordsCount;
              break;

            case SEMANTIC_LINE:
              recordsCount = 0;
              allowedLine = checkWWSemantic->IsChecked();
              break;

            case MARK_LINE:
              allowedLine = (( checkWWEvents->IsChecked() && whatWhereSelectedTimeEventLines > 0 ) ||
                             ( checkWWCommunications->IsChecked() && whatWhereSelectedTimeCommunicationLines > 0 ));
              break;

            default:
              allowedLine = true;
              break;
          }

          if ( allowedLine )
            whatWhereText->AppendText( it->second );
          else if( recordsCount == 200 )
          {
            whatWhereText->BeginBold();
            whatWhereText->AppendText( _( "Too much records. Reduce time scale.\n" ) );
            whatWhereText->EndBold();
          }

        break;
      }
    }
  }

  whatWhereText->EndFontSize();

  infoZone->ChangeSelection( 0 );
}


// If some tags changes here, please read printWhatWhere function.
void gTimeline::printWWSemantic( TObjectOrder whichRow, bool clickedValue, bool textMode )
{
  wxString onString;

  whatWhereLines.push_back( make_pair( BEGIN_SEMANTIC_SECTION, _( "" )));

  if( clickedValue )
    whatWhereLines.push_back( make_pair( BEGIN_CURRENT_SECTION, _( "" )));
  else
    whatWhereLines.push_back( make_pair( BEGIN_PREVNEXT_SECTION, _( "" )));

  if ( !textMode )
    onString << _("Semantic value: ");
  onString << wxString::FromAscii( LabelConstructor::semanticLabel( myWindow, myWindow->getValue( whichRow ), textMode, 
                                                                    ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() );
  onString << wxT( "\t  Duration: " ) << wxString::FromAscii( LabelConstructor::timeLabel(
                                                                myWindow->traceUnitsToWindowUnits( myWindow->getEndTime( whichRow )
                                                                                                   - myWindow->getBeginTime( whichRow ) ),
                                                                myWindow->getTimeUnit(), 
                                                                ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() );
  onString << _( "\n" );
  whatWhereLines.push_back( make_pair( SEMANTIC_LINE, onString ));

  if( clickedValue )
    whatWhereLines.push_back( make_pair( END_CURRENT_SECTION, _( "" )));
  else
    whatWhereLines.push_back( make_pair( END_PREVNEXT_SECTION, _( "" )));

  whatWhereLines.push_back( make_pair( END_SEMANTIC_SECTION, _( "" )));
}


// If some tags changes here, please read printWhatWhere function.
void gTimeline::printWWRecords( TObjectOrder whichRow, bool clickedValue, bool textMode )
{
  wxString onString;

  whatWhereLines.push_back( make_pair( BEGIN_RECORDS_SECTION, _( "" )));

  RecordList *rl = myWindow->getRecordList( whichRow );
  RecordList::iterator it = rl->begin();

  while( it != rl->end() &&
         (*it).getTime() < myWindow->getWindowBeginTime() &&
         (*it).getTime() < myWindow->getBeginTime( whichRow ) )
  {
    ++it;
  }

  if( clickedValue )
    whatWhereLines.push_back( make_pair( BEGIN_CURRENT_SECTION, _( "" )));
  else
    whatWhereLines.push_back( make_pair( BEGIN_PREVNEXT_SECTION, _( "" )));

  while( it != rl->end() && 
         (*it).getTime() <= myWindow->getWindowEndTime() &&
         (*it).getTime() <= myWindow->getEndTime( whichRow ) )
  {
    if( (*it).getType() & EVENT )
    {
      onString << wxT( "User Event at " ) << wxString::FromAscii( LabelConstructor::timeLabel(
                                                                    myWindow->traceUnitsToWindowUnits( (*it).getTime() ),
                                                                    myWindow->getTimeUnit(), 
                                                                    0 ).c_str() );
      onString << wxT( "\t" );
      onString << wxString::FromAscii( LabelConstructor::eventLabel( myWindow, (*it).getEventType(), (*it).getEventValue(), textMode ).c_str() );
      onString << wxT( "\n" );

      whatWhereLines.push_back( make_pair( EVENT_LINE, onString ));
      onString.clear();
      
      if ( clickedValue )
        whatWhereSelectedTimeEventLines++;
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
        
      onString << wxT( "at " ) << wxString::FromAscii( LabelConstructor::timeLabel(
                                                         myWindow->traceUnitsToWindowUnits( (*it).getTime() ),
                                                         myWindow->getTimeUnit(),
                                                         0 ).c_str() );
      if( (*it).getType() & SEND )
        onString << wxT( " to " );
      else if( (*it).getType() & RECV )
        onString << wxT( " from " );
      onString << wxString::FromAscii( LabelConstructor::objectLabel( (*it).getCommPartnerObject(),
                                                                      myWindow->getLevel(),
                                                                      myWindow->getTrace() ).c_str() );
      onString << wxT( " at " ) << wxString::FromAscii( LabelConstructor::timeLabel(
                                                          myWindow->traceUnitsToWindowUnits( (*it).getCommPartnerTime() ),
                                                          myWindow->getTimeUnit(),
                                                          0 ).c_str() );
      if( (*it).getType() & SEND )
        onString << wxT( ", Duration: " ) << wxString::FromAscii( LabelConstructor::timeLabel(
                                                                    myWindow->traceUnitsToWindowUnits( (*it).getCommPartnerTime() 
                                                                                                       - (*it).getTime() ),
                                                                    myWindow->getTimeUnit(),
                                                                    0 ).c_str() );
      else if( (*it).getType() & RECV )
        onString << wxT( ", Duration: " ) << wxString::FromAscii( LabelConstructor::timeLabel(
                                                                    myWindow->traceUnitsToWindowUnits( (*it).getTime()
                                                                                                       - (*it).getCommPartnerTime() ),
                                                                    myWindow->getTimeUnit(), 
                                                                    0 ).c_str() );

      onString << wxT( " (size: " ) << (*it).getCommSize() << 
                  wxT( ", tag: " ) << (*it).getCommTag() << wxT( ")" );
      onString << wxT( "\n" );

      whatWhereLines.push_back( make_pair( COMMUNICATION_LINE, onString ));
      onString.clear();

      if ( clickedValue )
        whatWhereSelectedTimeCommunicationLines++;
    }
    ++it;
  }

  rl->erase( rl->begin(), it );

  if( clickedValue )
    whatWhereLines.push_back( make_pair( END_CURRENT_SECTION, _( "" )));
  else
    whatWhereLines.push_back( make_pair( END_PREVNEXT_SECTION, _( "" )));

  whatWhereLines.push_back( make_pair( END_RECORDS_SECTION, _( "" )));
  whatWhereLines.push_back( make_pair( RAW_LINE, _( "\n" )));
}


void gTimeline::resizeDrawZone( int width, int height )
{
  canRedraw = false;
  drawZone->SetClientSize( width, height );
  if( !splitter->IsSplit() )
    this->SetClientSize( width, height );
  else
  {
    this->SetClientSize( width, height + /*infoZone->GetClientSize().GetHeight()*/infoZoneLastSize + 5 );
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
    infoZoneLastSize = infoZone->GetSize().GetHeight();
    splitter->Unsplit();
    Unsplit();
  }
  else
    Split();
}

void gTimeline::Unsplit()
{
  canRedraw = false;
  this->Freeze();

/*#ifdef WIN32
  this->SetClientSize( this->GetClientSize().GetWidth(), this->GetClientSize().GetHeight() -
                                                         infoZone->GetClientSize().GetHeight() );
#else*/
  this->SetClientSize( this->GetClientSize().GetWidth(), myWindow->getHeight() );
//#endif

  drawZone->SetClientSize( myWindow->getWidth(), myWindow->getHeight() );
  this->Thaw();
  canRedraw = true;
  splitChanged = true;
}

void gTimeline::Split()
{
  canRedraw = false;
  this->Freeze();
  splitter->SplitHorizontally( drawZone, infoZone, myWindow->getHeight() );
/*  drawZone->SetClientSize( myWindow->getWidth(), myWindow->getHeight() );
  splitter->SetSashPosition( myWindow->getHeight() );*/
  resizeDrawZone( myWindow->getWidth(), myWindow->getHeight() );
  infoZone->SetClientSize( myWindow->getWidth(), infoZoneLastSize );
#ifdef WIN32
  this->SetClientSize( this->GetClientSize().GetWidth(), this->GetClientSize().GetHeight() +
                                                         /*infoZone->GetClientSize().GetHeight()*/
                                                         infoZoneLastSize + 5 );
#endif
  this->Thaw();
  canRedraw = true;
  splitChanged = true;
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
  static GradientColor::TGradientFunction gradientFunc = GradientColor::LINEAR;
  UINT32 precision = ParaverConfig::getInstance()->getTimelinePrecision();
  
  if( redoColors &&
      ( myWindow->getSemanticInfoType() != lastType ||
        myWindow->getMinimumY() != lastMin ||
        myWindow->getMaximumY() != lastMax ||
        myWindow->IsCodeColorSet() != codeColorSet ||
        myWindow->getGradientColor().getGradientFunction() != gradientFunc )
    )
  {
    lastType = myWindow->getSemanticInfoType();
    lastMin = myWindow->getMinimumY();
    lastMax = myWindow->getMaximumY();
    codeColorSet = myWindow->IsCodeColorSet();
    gradientFunc = myWindow->getGradientColor().getGradientFunction();

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
        wxString tmpStr = wxString::FromAscii( LabelConstructor::semanticLabel( myWindow, i, true, precision ).c_str() );
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
      tmpStr << wxT("< ") << wxString::FromAscii( LabelConstructor::semanticLabel( myWindow, lastMin, false, precision ).c_str() );
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
        tmpStr << wxString::FromAscii( LabelConstructor::semanticLabel( myWindow, ( i * step ) + lastMin, false, precision ).c_str() );
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
      tmpStr << wxT("> ") << wxString::FromAscii( LabelConstructor::semanticLabel( myWindow, lastMax, false, precision ).c_str() );
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


void gTimeline::drawCommunicationLines( bool draw )
{
  wxMemoryDC bufferDraw;

  myWindow->setDrawCommLines( draw );
  
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


void gTimeline::drawEventFlags( bool draw )
{
  wxMemoryDC bufferDraw;

  myWindow->setDrawFlags( draw );
  
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


void gTimeline::saveImage()
{
  wxString imageName, imageSuffix, defaultDir;
  long imageType;
  
  string auxName = myWindow->getName() + "_" + myWindow->getTrace()->getTraceNameNumbered();
  imageName = wxString::FromAscii( auxName.c_str() );

#ifdef WIN32
  defaultDir = _(".\\");
#else
  defaultDir = _("./");
#endif

  wxFileDialog saveDialog( this,
                           _("Save Image"),
                           defaultDir,
                           imageName, // default name ->window name!
#ifdef WIN32
                           _("BMP image|*.bmp|JPEG image|*.jpg|PNG image|*.png|XPM image|*.xpm"), // file types 
#else
                           _("BMP image|*.bmp|JPEG image|*.jpg|PNG image|*.png|XPM image|*.xpm"), // file types 
#endif
                           wxSAVE );

  saveDialog.SetFilterIndex( ParaverConfig::getInstance()->getTimelineSaveImageFormat() );
  if ( saveDialog.ShowModal() == wxID_OK )
  {
    wxImage baseLayer = drawImage.ConvertToImage();
    switch( saveDialog.GetFilterIndex() )
    {
      case 0:
        imageType =  wxBITMAP_TYPE_BMP;
        imageSuffix = _(".bmp");
        break;
      case 1:
        imageType =  wxBITMAP_TYPE_JPEG;
        imageSuffix = _(".jpg");
        break;
      case 2:
        imageType =  wxBITMAP_TYPE_PNG;
        imageSuffix = _(".png");
        break;
      case 3:
        imageType = wxBITMAP_TYPE_XPM;
        imageSuffix = _(".xpm");
        break;
      default:
        imageType =  wxBITMAP_TYPE_PNG;
        imageSuffix = _(".png");
        break;
    }

    baseLayer.SaveFile( saveDialog.GetPath() + imageSuffix , imageType );
  }
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECK_DRAWLINES
 */

void gTimeline::OnCheckDrawlinesClick( wxCommandEvent& event )
{
  drawCommunicationLines( event.IsChecked() );
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
  drawEventFlags( event.IsChecked() );
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

void gTimeline::OnTimerSize( wxTimerEvent& event )
{
  if( ready )
    redraw();
  Refresh();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX
 */

void gTimeline::OnCheckWhatWhereText( wxCommandEvent& event )
{
  checkWWSemantic->Enable( false );
  checkWWEvents->Enable( false );
  checkWWCommunications->Enable( false );
  checkWWPreviousNext->Enable( false );
  checkWWText->Enable( false );

  computeWhatWhere( whatWhereTime, whatWhereRow, event.IsChecked() );
  printWhatWhere();

  checkWWSemantic->Enable( true );
  checkWWEvents->Enable( true );
  checkWWCommunications->Enable( true );
  checkWWPreviousNext->Enable( true );
  checkWWText->Enable( true );
}

void gTimeline::OnCheckWhatWhere( wxCommandEvent& event )
{
  checkWWSemantic->Enable( false );
  checkWWEvents->Enable( false );
  checkWWCommunications->Enable( false );
  checkWWPreviousNext->Enable( false );
  checkWWText->Enable( false );

  printWhatWhere();

  checkWWSemantic->Enable( true );
  checkWWEvents->Enable( true );
  checkWWCommunications->Enable( true );
  checkWWPreviousNext->Enable( true );
  checkWWText->Enable( true );
}

void gTimeline::OnMenuGradientFunction( GradientColor::TGradientFunction function )
{
  myWindow->getGradientColor().setGradientFunction( function );
  myWindow->setRedraw( true );
}


/*!
 * wxEVT_MIDDLE_UP event handler for ID_SCROLLEDWINDOW
 */

void gTimeline::OnScrolledWindowMiddleUp( wxMouseEvent& event )
{
  if( myWindow->getLevel() != THREAD )
    return;

  wxMemoryDC dc( bufferImage );
  long X = event.GetX();
  
  if( X < objectAxisPos || X > dc.GetSize().GetWidth() - drawBorder
      || event.GetY() < drawBorder || event.GetY() > timeAxisPos )
    return;
  else
    X -= objectAxisPos;

  TTime timeStep = ( myWindow->getWindowEndTime() - myWindow->getWindowBeginTime() ) /
                   ( dc.GetSize().GetWidth() - objectAxisPos - drawBorder );

  TTime posTime = ( timeStep * X ) + myWindow->getWindowBeginTime();

  TObjectOrder beginRow = myWindow->getZoomSecondDimension().first;
  TObjectOrder endRow = myWindow->getZoomSecondDimension().second;
  vector<TObjectOrder> selected;
  myWindow->getSelectedRows( myWindow->getLevel(), selected, beginRow, endRow, true );
  TObjectOrder numObjects = selected.size();
  double heightPerRow = (double)( timeAxisPos - drawBorder - 1 ) / (double)numObjects;
  TObjectOrder posRow = TObjectOrder( floor( (event.GetY() - drawBorder - 1) / heightPerRow ) );

  if( posRow >= numObjects )
    posRow = numObjects - 1;
    
  posRow = selected[ posRow ];

  TEventType type;
  TEventValue value;
  vector<TEventType> events;
  events.push_back( 60000119 );
  if( myWindow->getTrace()->findLastEventValue( posRow, posTime, events, type, value ) )
  {
    string valueStr = LabelConstructor::eventValueLabel( myWindow, type, value );
    string lineStr = valueStr.substr( 0, valueStr.find_first_of( ' ', 0 ) );
    string fileStr = valueStr.substr( valueStr.find_first_of( '(', 0 ) + 1,
                                      valueStr.length() - valueStr.find_first_of( '(', 0 ) - 2 );
    wxDirDialog dirDialog( NULL, _( "Choose the directory to find to source files" ) );
    if( dirDialog.ShowModal() == wxID_OK )
    {
      wxString path = dirDialog.GetPath();
      cout << path.c_str() << endl;
      wxString command;
#ifdef WIN32
      command << _( "wordpad.exe " ) /*<< " +" << lineStr << " "*/ << path << _( "\\" ) << wxString::FromAscii( fileStr.c_str() );
      wxExecute( command );
#else
      command << _( "gvim " ) << _( " +" ) << wxString::FromAscii( lineStr.c_str() ) << _( " " ) << path << _( "/" ) << wxString::FromAscii( fileStr.c_str() );
      cout << command << endl;
      if( wxExecute( command ) == 0 )
      {
        command << _( "nedit " ) << _( " +" ) << wxString::FromAscii( lineStr.c_str() ) << _( " " ) << path << _( "/" ) << wxString::FromAscii( fileStr.c_str() );
        if( wxExecute( command ) == 0 )
          wxMessageBox( _( "Install gvim or nedit for view source code files." ), _( "Show source code" ) );
      }
#endif
    }
  }
  else
  {
    wxMessageBox( _( "Event " ) + (wxString() << 60000119) + _( " not found." ), _( "Show source code" ) );
  }
}

