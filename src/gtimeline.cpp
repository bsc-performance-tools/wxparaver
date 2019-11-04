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

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#ifdef __BORLANDC__
#pragma hdrstopclo
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/version.h>
#if wxMAJOR_VERSION>=3
#include <wx/dcgraph.h>
#endif

////@begin includes
#include "wx/imaglist.h"
////@end includes
#include <wx/dcbuffer.h>
#include <wx/statline.h>
#include <algorithm>
#include <wx/filename.h>
#include <wx/display.h>

#include "config_traits.h"
#include "wxparaverapp.h"
#include "gtimeline.h"
#include "finddialog.h"
#include "window.h"
#include "labelconstructor.h"
#include "drawmode.h"
#include "loadedwindows.h"
#include "windows_tree.h"
#include "caution.xpm"
#include "caution_yellow.xpm"
#include "output.h"
#include "filedialogext.h"
#include "progresscontroller.h"
#include "paravermain.h"

#ifdef __WXMAC__
  #include <wx/rawbmp.h>
#endif

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
  EVT_SPLITTER_DCLICK( ID_SPLITTER_TIMELINE, gTimeline::OnSplitterTimelineSashDClick )
  EVT_SPLITTER_UNSPLIT( ID_SPLITTER_TIMELINE, gTimeline::OnSplitterTimelineSashUnsplit )
  EVT_UPDATE_UI( ID_SCROLLED_DRAW, gTimeline::OnScrolledWindowUpdate )
  EVT_NOTEBOOK_PAGE_CHANGING( ID_NOTEBOOK_INFO, gTimeline::OnNotebookInfoPageChanging )
  EVT_CHECKBOX( ID_CHECKBOX, gTimeline::OnCheckWhatWhere )
  EVT_CHECKBOX( ID_CHECKBOX1, gTimeline::OnCheckWhatWhere )
  EVT_CHECKBOX( ID_CHECKBOX2, gTimeline::OnCheckWhatWhere )
  EVT_CHECKBOX( ID_CHECKBOX3, gTimeline::OnCheckWhatWhere )
  EVT_CHECKBOX( ID_CHECKBOX4, gTimeline::OnCheckWhatWhereText )
  EVT_CHECKBOX( ID_CHECKBOX5, gTimeline::OnCheckWhatWhereText )
  EVT_UPDATE_UI( wxID_STATIC_SLOPE, gTimeline::OnStaticSlopeUpdate )
  EVT_UPDATE_UI( ID_SCROLLED_COLORS, gTimeline::OnColorsPanelUpdate )
////@end gTimeline event table entries

  EVT_TIMER( ID_TIMER_SIZE, gTimeline::OnTimerSize )
  EVT_TIMER( ID_TIMER_MOTION, gTimeline::OnTimerMotion )
  EVT_TIMER( ID_TIMER_WHEEL, gTimeline::OnTimerWheel )
  
END_EVENT_TABLE()

wxProgressDialog *gTimeline::dialogProgress = NULL;

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
  redrawStopWatch->Pause();
  return true;
}


/*!
 * gTimeline destructor
 */

gTimeline::~gTimeline()
{
////@begin gTimeline destruction
////@end gTimeline destruction
  gPasteWindowProperties::getInstance()->verifyRemove( this );

  delete timerSize;
  delete timerMotion;
  delete myWindow;
}


/*!
 * Member initialisation
 */

void gTimeline::Init()
{
////@begin gTimeline member initialisation
  canRedraw = false;
  drawCaution = false;
  drawCautionNegatives = false;
  escapePressed = false;
  findBeginTime = 0;
  findEndTime = 0;
  findFirstObject = 0;
  findLastObject = 0;
  firstUnsplit = false;
  infoZoneLastSize = 200;
  lastEventFoundTime = 0;
  lastFoundObject = 0;
  lastSemanticFoundTime = 0;
  myWindow = NULL;
  objectHeight = 1;
  redoColors = false;
  redrawStopWatch = new wxStopWatch();
  splitChanged = false;
  timerMotion = new wxTimer( this, ID_TIMER_MOTION );
  timerSize = new wxTimer( this, ID_TIMER_SIZE );
  timerWheel = new wxTimer( this, ID_TIMER_WHEEL );
  timing = false;
  wheelZoomBeginObject = 0;
  wheelZoomBeginTime = 0;
  wheelZoomEndObject = 0;
  wheelZoomEndTime = 0;
  wheelZoomFactor = 1;
  zooming = false;
  splitter = NULL;
  drawZone = NULL;
  infoZone = NULL;
  whatWherePanel = NULL;
  checkWWSemantic = NULL;
  checkWWEvents = NULL;
  checkWWCommunications = NULL;
  checkWWPreviousNext = NULL;
  checkWWText = NULL;
  checkWWShowDate = NULL;
  whatWhereText = NULL;
  timingZone = NULL;
  initialTimeText = NULL;
  initialSemanticLabel = NULL;
  initialSemanticText = NULL;
  finalTimeText = NULL;
  finalSemanticLabel = NULL;
  finalSemanticText = NULL;
  durationText = NULL;
  slopeLabel = NULL;
  slopeText = NULL;
  colorsPanel = NULL;
  colorsSizer = NULL;
////@end gTimeline member initialisation

  zoomXY = false;
  bufferImage.Create( 1, 1 );
#ifdef __WXMAC__
  objectFont = wxFont( 8, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD );
  timeFont = wxFont( 8, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL );
  semanticFont = wxFont( 10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD );
#else
  objectFont = wxFont( 7, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL );
  timeFont = wxFont( 6, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL );
  semanticFont = wxFont( 8, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD );
#endif
  whatWhereTime = 0.0;
  whatWhereRow = 0;
  whatWhereSemantic = 0.0;

  lastType = NO_TYPE;
  lastMin = 0;
  lastMax = 15;
  lastValuesSize = semanticValuesToColor.size();
  codeColorSet = true;
  gradientFunc = GradientColor::LINEAR;
  
#ifdef WIN32
  wheelZoomObjects = false;
#endif
}


/*!
 * Control creation for gTimeline
 */

void gTimeline::CreateControls()
{
////@begin gTimeline content construction
  gTimeline* itemFrame1 = this;

  splitter = new wxSplitterWindow( itemFrame1, ID_SPLITTER_TIMELINE, wxDefaultPosition, wxDefaultSize, wxSP_BORDER|wxSP_3DSASH|wxSP_PERMIT_UNSPLIT );
  splitter->SetMinimumPaneSize(wxDLG_UNIT(itemFrame1, wxSize(0, -1)).x);

  drawZone = new wxScrolledWindow( splitter, ID_SCROLLED_DRAW, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxWANTS_CHARS|wxFULL_REPAINT_ON_RESIZE );
  drawZone->SetScrollbars(1, 1, 0, 0);
  infoZone = new wxNotebook( splitter, ID_NOTEBOOK_INFO, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

  whatWherePanel = new wxScrolledWindow( infoZone, ID_SCROLLED_WHATWHERE, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
  whatWherePanel->SetScrollbars(1, 1, 0, 0);
  wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxVERTICAL);
  whatWherePanel->SetSizer(itemBoxSizer6);

  wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer6->Add(itemBoxSizer7, 0, wxALIGN_LEFT|wxALL, wxDLG_UNIT(whatWherePanel, wxSize(5, -1)).x);
  checkWWSemantic = new wxCheckBox( whatWherePanel, ID_CHECKBOX, _("Semantic"), wxDefaultPosition, wxDefaultSize, 0 );
  checkWWSemantic->SetValue(true);
  itemBoxSizer7->Add(checkWWSemantic, 0, wxALIGN_CENTER_VERTICAL, wxDLG_UNIT(whatWherePanel, wxSize(2, -1)).x);

  checkWWEvents = new wxCheckBox( whatWherePanel, ID_CHECKBOX1, _("Events"), wxDefaultPosition, wxDefaultSize, 0 );
  checkWWEvents->SetValue(true);
  itemBoxSizer7->Add(checkWWEvents, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, wxDLG_UNIT(whatWherePanel, wxSize(2, -1)).x);

  checkWWCommunications = new wxCheckBox( whatWherePanel, ID_CHECKBOX2, _("Communications"), wxDefaultPosition, wxDefaultSize, 0 );
  checkWWCommunications->SetValue(true);
  itemBoxSizer7->Add(checkWWCommunications, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, wxDLG_UNIT(whatWherePanel, wxSize(2, -1)).x);

  checkWWPreviousNext = new wxCheckBox( whatWherePanel, ID_CHECKBOX3, _("Prev./Next"), wxDefaultPosition, wxDefaultSize, 0 );
  checkWWPreviousNext->SetValue(true);
  itemBoxSizer7->Add(checkWWPreviousNext, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, wxDLG_UNIT(whatWherePanel, wxSize(2, -1)).x);

  checkWWText = new wxCheckBox( whatWherePanel, ID_CHECKBOX4, _("Text"), wxDefaultPosition, wxDefaultSize, 0 );
  checkWWText->SetValue(true);
  itemBoxSizer7->Add(checkWWText, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, wxDLG_UNIT(whatWherePanel, wxSize(2, -1)).x);

  checkWWShowDate = new wxCheckBox( whatWherePanel, ID_CHECKBOX5, _("Show Date"), wxDefaultPosition, wxDefaultSize, 0 );
  checkWWShowDate->SetValue(false);
  itemBoxSizer7->Add(checkWWShowDate, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, wxDLG_UNIT(whatWherePanel, wxSize(2, -1)).x);

  whatWhereText = new wxRichTextCtrl( whatWherePanel, ID_RICHTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxWANTS_CHARS );
  itemBoxSizer6->Add(whatWhereText, 1, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, wxDLG_UNIT(whatWherePanel, wxSize(5, -1)).x);

  whatWherePanel->FitInside();
  infoZone->AddPage(whatWherePanel, _("What / Where"));

  timingZone = new wxScrolledWindow( infoZone, ID_SCROLLED_TIMING, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  timingZone->SetScrollbars(1, 5, 0, 0);
  wxFlexGridSizer* itemFlexGridSizer15 = new wxFlexGridSizer(3, 4, 0, 0);
  timingZone->SetSizer(itemFlexGridSizer15);

  wxStaticText* itemStaticText16 = new wxStaticText( timingZone, wxID_STATIC, _("Initial time"), wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer15->Add(itemStaticText16, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, wxDLG_UNIT(timingZone, wxSize(5, -1)).x);

  initialTimeText = new wxTextCtrl( timingZone, ID_TEXTCTRL_INITIALTIME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer15->Add(initialTimeText, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, wxDLG_UNIT(timingZone, wxSize(5, -1)).x);

  initialSemanticLabel = new wxStaticText( timingZone, wxID_STATIC_INITIALSEMANTIC, _("Inital semantic"), wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer15->Add(initialSemanticLabel, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, wxDLG_UNIT(timingZone, wxSize(5, -1)).x);

  initialSemanticText = new wxTextCtrl( timingZone, ID_TEXTCTRL_INITIALSEMANTIC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer15->Add(initialSemanticText, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, wxDLG_UNIT(timingZone, wxSize(5, -1)).x);

  wxStaticText* itemStaticText20 = new wxStaticText( timingZone, wxID_STATIC, _("Final time"), wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer15->Add(itemStaticText20, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, wxDLG_UNIT(timingZone, wxSize(5, -1)).x);

  finalTimeText = new wxTextCtrl( timingZone, ID_TEXTCTRL_FINALTIME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer15->Add(finalTimeText, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, wxDLG_UNIT(timingZone, wxSize(5, -1)).x);

  finalSemanticLabel = new wxStaticText( timingZone, wxID_STATIC_FINALSEMANTIC, _("Final semantic"), wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer15->Add(finalSemanticLabel, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, wxDLG_UNIT(timingZone, wxSize(5, -1)).x);

  finalSemanticText = new wxTextCtrl( timingZone, ID_TEXTCTRL_FINALSEMANTIC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer15->Add(finalSemanticText, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, wxDLG_UNIT(timingZone, wxSize(5, -1)).x);

  wxStaticText* itemStaticText24 = new wxStaticText( timingZone, wxID_STATIC, _("Duration"), wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer15->Add(itemStaticText24, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, wxDLG_UNIT(timingZone, wxSize(5, -1)).x);

  durationText = new wxTextCtrl( timingZone, ID_TEXTCTRL_DURATION, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer15->Add(durationText, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, wxDLG_UNIT(timingZone, wxSize(5, -1)).x);

  slopeLabel = new wxStaticText( timingZone, wxID_STATIC_SLOPE, _("Slope"), wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer15->Add(slopeLabel, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, wxDLG_UNIT(timingZone, wxSize(5, -1)).x);

  slopeText = new wxTextCtrl( timingZone, ID_TEXTCTRL_SLOPE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer15->Add(slopeText, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, wxDLG_UNIT(timingZone, wxSize(5, -1)).x);

  itemFlexGridSizer15->AddGrowableCol(1);
  itemFlexGridSizer15->AddGrowableCol(3);

  timingZone->FitInside();
  infoZone->AddPage(timingZone, _("Timing"));

  colorsPanel = new wxScrolledWindow( infoZone, ID_SCROLLED_COLORS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxVSCROLL|wxTAB_TRAVERSAL );
  colorsPanel->SetScrollbars(1, 5, 0, 0);
  colorsSizer = new wxBoxSizer(wxVERTICAL);
  colorsPanel->SetSizer(colorsSizer);

  colorsPanel->FitInside();
  infoZone->AddPage(colorsPanel, _("Colors"));

  splitter->SplitHorizontally(drawZone, infoZone, wxDLG_UNIT(itemFrame1, wxSize(0, -1)).x);

  // Connect events and objects
  drawZone->Connect(ID_SCROLLED_DRAW, wxEVT_SIZE, wxSizeEventHandler(gTimeline::OnScrolledWindowSize), NULL, this);
  drawZone->Connect(ID_SCROLLED_DRAW, wxEVT_PAINT, wxPaintEventHandler(gTimeline::OnScrolledWindowPaint), NULL, this);
  drawZone->Connect(ID_SCROLLED_DRAW, wxEVT_MIDDLE_UP, wxMouseEventHandler(gTimeline::OnScrolledWindowMiddleUp), NULL, this);
  drawZone->Connect(ID_SCROLLED_DRAW, wxEVT_RIGHT_DOWN, wxMouseEventHandler(gTimeline::OnScrolledWindowRightDown), NULL, this);
  drawZone->Connect(ID_SCROLLED_DRAW, wxEVT_MOTION, wxMouseEventHandler(gTimeline::OnScrolledWindowMotion), NULL, this);
  drawZone->Connect(ID_SCROLLED_DRAW, wxEVT_MOUSEWHEEL, wxMouseEventHandler(gTimeline::OnScrolledWindowMouseWheel), NULL, this);
  drawZone->Connect(ID_SCROLLED_DRAW, wxEVT_KEY_DOWN, wxKeyEventHandler(gTimeline::OnScrolledWindowKeyDown), NULL, this);
  drawZone->Connect(ID_SCROLLED_DRAW, wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(gTimeline::OnScrolledWindowEraseBackground), NULL, this);
  drawZone->Connect(ID_SCROLLED_DRAW, wxEVT_LEFT_DOWN, wxMouseEventHandler(gTimeline::OnScrolledWindowLeftDown), NULL, this);
  drawZone->Connect(ID_SCROLLED_DRAW, wxEVT_LEFT_UP, wxMouseEventHandler(gTimeline::OnScrolledWindowLeftUp), NULL, this);
  drawZone->Connect(ID_SCROLLED_DRAW, wxEVT_LEFT_DCLICK, wxMouseEventHandler(gTimeline::OnScrolledWindowLeftDClick), NULL, this);
////@end gTimeline content construction

  SetMinSize( wxSize( 100, 50 ) );
  
  ParaverConfig *paraverConfig = ParaverConfig::getInstance();

  checkWWSemantic->SetValue( paraverConfig->getTimelineWhatWhereSemantic() );
  checkWWCommunications->SetValue( paraverConfig->getTimelineWhatWhereCommunications() );
  checkWWEvents->SetValue( paraverConfig->getTimelineWhatWhereEvents() );
  checkWWPreviousNext->SetValue( paraverConfig->getTimelineWhatWherePreviousNext() );
  checkWWText->SetValue( paraverConfig->getTimelineWhatWhereText() );
  //checkWWShowDate->SetValue( paraverConfig->getTimelineWhatWhereShowDate() );
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


std::vector< TObjectOrder > gTimeline::getCurrentZoomRange()
{
  vector< TObjectOrder > zoomRange;

  zoomRange.push_back( myWindow->getZoomSecondDimension().first );
  zoomRange.push_back( myWindow->getZoomSecondDimension().second );
  
  return zoomRange;
}


#ifdef __WXMAC__
void gTimeline::drawStackedImages( wxDC& dc )
{
  if( bufferImage.IsOk() )
    dc.DrawBitmap( bufferImage, 0, 0, false );
    
  doDrawCaution( dc );
  
  if( myWindow->getDrawFlags() && eventImage.IsOk() )
    dc.DrawBitmap( eventImage, 0, 0, true );

  if( myWindow->getDrawCommLines() && commImage.IsOk() )
    dc.DrawBitmap( commImage, 0, 0, true );
    
  if( zooming && zoomBMP.IsOk() && !motionEvent.ShiftDown() )
  {
    dc.DrawBitmap( zoomBMP, 0, 0, true );
  }
}
#endif

void gTimeline::redraw()
{
#ifdef __WXGTK__
  if( splitChanged )
  {
    resizeDrawZone( myWindow->getWidth(), myWindow->getHeight() );
    splitChanged = false;
    return;
  }
#endif
  myWindow->setReady( false );
  redoColors = true;

  semanticValuesToColor.clear();
  semanticColorsToValue.clear();

  rgb rgbForegroundColour = ((paraverMain *)parent)->GetParaverConfig()->getColorsTimelineAxis();
  foregroundColour = wxColour( rgbForegroundColour.red,
                               rgbForegroundColour.green,
                               rgbForegroundColour.blue );
  rgb rgbBackgroundColour = ((paraverMain *)parent)->GetParaverConfig()->getColorsTimelineBackground();
  backgroundColour = wxColour( rgbBackgroundColour.red,
                               rgbBackgroundColour.green,
                               rgbBackgroundColour.blue );

  rgb rgbLogicalColour = ((paraverMain *)parent)->GetParaverConfig()->getColorsTimelineLogicalCommunications();
  logicalColour = wxColour( rgbLogicalColour.red, rgbLogicalColour.green ,rgbLogicalColour.blue );

  rgb rgbPhysicalColour = ((paraverMain *)parent)->GetParaverConfig()->getColorsTimelinePhysicalCommunications();
  physicalColour = wxColour( rgbPhysicalColour.red, rgbPhysicalColour.green ,rgbPhysicalColour.blue );

  logicalPen = wxPen( logicalColour );
  physicalPen = wxPen( physicalColour );

// Disabled because some window managers can't show the progress dialog later
//  redrawStopWatch->Start();
  
  wxString winTitle = GetTitle();
  SetTitle( _("(Working...) ") + winTitle );

  ProgressController *progress = NULL;
  if ( myWindow->getShowProgressBar() )
  {
    progress = ProgressController::create( myWindow->getKernel() );
    progress->setHandler( progressFunctionTimeline, this );

  // Disabled progress dialog on windows. Causes blank image for semantic layer randomly (wxwidgets bug???)
  // Waiting for wxwidgets 3 code adaptation to prove that its solved.
#ifndef WIN32
    if( gTimeline::dialogProgress == NULL )
      gTimeline::dialogProgress = new wxProgressDialog( wxT("Drawing window..."),
                                                        wxT(""),
                                                        numeric_limits<int>::max(),
                                                        this,
                                                        wxPD_CAN_ABORT|wxPD_AUTO_HIDE|\
                                                        wxPD_APP_MODAL|wxPD_ELAPSED_TIME|\
                                                        wxPD_ESTIMATED_TIME|wxPD_REMAINING_TIME );

    // Disabled because some window managers can't show the progress dialog later
    //gTimeline::dialogProgress->Show( false );
    gTimeline::dialogProgress->Pulse( winTitle + _( "\t" ) );
    gTimeline::dialogProgress->Fit();
#endif // WIN32
  }

  // Get selected rows
  vector<bool>         selected;
  vector<TObjectOrder> selectedSet;
  TObjectOrder beginRow = myWindow->getZoomSecondDimension().first;
  TObjectOrder endRow =  myWindow->getZoomSecondDimension().second;
  myWindow->getSelectedRows( myWindow->getLevel(), selected, true );
  myWindow->getSelectedRows( myWindow->getLevel(), selectedSet, beginRow, endRow, true );

//  vector<bool>         tmpSel;
//  myWindow->getSelectedRows( THREAD, tmpSel );
//    for (int i=0;i<tmpSel.size();++i)

  // To avoid empty zooms (probably due to other bug)
  TObjectOrder maxObj;
  if ( selectedSet.size() == 0 )
    maxObj = endRow; //something!!
  else
    maxObj = selectedSet[ selectedSet.size() - 1 ];

  bufferImage.Create( drawZone->GetClientSize().GetWidth(), drawZone->GetClientSize().GetHeight() );
  drawImage.Create( drawZone->GetClientSize().GetWidth(), drawZone->GetClientSize().GetHeight() );
  commImage.Create( drawZone->GetClientSize().GetWidth(), drawZone->GetClientSize().GetHeight() );
  eventImage.Create( drawZone->GetClientSize().GetWidth(), drawZone->GetClientSize().GetHeight() );
#ifdef __WXMAC__
  wxMemoryDC tmpDC( bufferImage );
  wxGraphicsContext *gc = wxGraphicsContext::Create( tmpDC );
  gc->SetAntialiasMode( wxANTIALIAS_NONE );
  wxGCDC bufferDraw( tmpDC );
#else
  wxMemoryDC bufferDraw( bufferImage );
  #if wxMAJOR_VERSION>=3
  wxGraphicsContext *gc = wxGraphicsContext::Create( bufferDraw );
  gc->SetAntialiasMode( wxANTIALIAS_NONE );
  #endif
#endif
  wxMemoryDC commdc( commImage );
  wxMemoryDC eventdc( eventImage );
#if wxMAJOR_VERSION>=3
  wxGraphicsContext *gcComm = wxGraphicsContext::Create( commdc );
  gcComm->SetAntialiasMode( wxANTIALIAS_NONE );
  wxGraphicsContext *gcEvent = wxGraphicsContext::Create( eventdc );
  gcEvent->SetAntialiasMode( wxANTIALIAS_NONE );
#endif
  commdc.SetBackgroundMode( wxTRANSPARENT );
  commdc.SetBackground( *wxTRANSPARENT_BRUSH );
  commdc.Clear();
  eventdc.SetBackgroundMode( wxTRANSPARENT );
  eventdc.SetBackground( *wxTRANSPARENT_BRUSH );
  eventdc.Clear();
  wxBitmap commMask;
  commMask.Create( drawZone->GetClientSize().GetWidth(), drawZone->GetClientSize().GetHeight(), 1 );
  wxMemoryDC commmaskdc( commMask );
#if wxMAJOR_VERSION>=3
  wxGraphicsContext *gcCommMask = wxGraphicsContext::Create( commmaskdc );
  gcCommMask->SetAntialiasMode( wxANTIALIAS_NONE );
#endif
  commmaskdc.SetBackground( *wxBLACK_BRUSH );
  commmaskdc.SetPen( wxPen( wxColour( 255, 255, 255 ), 1 ) );
  commmaskdc.Clear();
  wxBitmap eventMask;
  eventMask.Create( drawZone->GetClientSize().GetWidth(), drawZone->GetClientSize().GetHeight(), 1 );
  wxMemoryDC eventmaskdc( eventMask );
#if wxMAJOR_VERSION>=3
  wxGraphicsContext *gcEventMask = wxGraphicsContext::Create( eventmaskdc );
  gcEventMask->SetAntialiasMode( wxANTIALIAS_NONE );
#endif
  eventmaskdc.SetBackground( *wxBLACK_BRUSH );
  eventmaskdc.SetPen( wxPen( wxColour( 255, 255, 255 ), 1 ) );
  eventmaskdc.Clear();

  bufferDraw.SetBackground( wxBrush( backgroundColour ) );
  bufferDraw.Clear();
  
  // Paint blank image while redrawing
  wxClientDC dc( drawZone );
#ifdef __WXGTK__
  dc.DrawBitmap( bufferImage, 0, 0, false );
  drawZone->Update();
#endif

  if( !drawAxis( bufferDraw, selectedSet ) )
  {
    SetTitle( winTitle );
    myWindow->setReady( true );
    return;
  }
  myWindow->init( myWindow->getWindowBeginTime(), CREATECOMMS + CREATEEVENTS );

  drawCaution          = false;
  drawCautionNegatives = false;

  eventdc.SetTextForeground( *wxGREEN );
  eventdc.SetTextBackground( backgroundColour );
  eventdc.SetPen( *wxGREEN_PEN );
  eventdc.SetBrush( *wxGREEN_BRUSH );
  eventdc.SetBackgroundMode( wxTRANSPARENT );
#ifndef __WXMAC__
  eventmaskdc.SetPen( *wxWHITE_PEN );
  eventmaskdc.SetBrush( *wxWHITE_BRUSH );
#endif

  float magnify = float( myWindow->getPixelSize() );
  TTime timeStep = (( myWindow->getWindowEndTime() - myWindow->getWindowBeginTime() )  * magnify) /
                   ( drawZone->GetClientSize().GetWidth() - objectAxisPos - drawBorder - magnify );
  PRV_INT32 timePos = objectAxisPos + 1;

  vector< vector< TSemanticValue > > valuesToDraw;
  vector< hash_set< PRV_INT32 > > eventsToDraw;
#ifdef WIN32
  vector< hash_set< commCoord > > commsToDraw;
#else
  vector< hash_set< commCoord, hashCommCoord > > commsToDraw;
#endif

  vector< vector< vector< pair<TSemanticValue,TSemanticValue> > > > valuesToDrawPunctual;
  if( myWindow->isPunctualColorSet() )
  {
    if( myWindow->getPunctualColorWindow() != NULL )
      myWindow->getPunctualColorWindow()->init( myWindow->getWindowBeginTime(), NOCREATE );

    myWindow->computeSemanticPunctualParallel( selectedSet, selected,
                                               timeStep, timePos, objectAxisPos,
                                               objectPosList,
                                               maxObj,
                                               drawCaution,
                                               valuesToDrawPunctual, eventsToDraw, commsToDraw,
                                               progress );
  }
  else
  {
    myWindow->computeSemanticParallel( selectedSet, selected,
                                       timeStep, timePos, objectAxisPos,
                                       objectPosList,
                                       maxObj,
                                       drawCaution,
                                       valuesToDraw, eventsToDraw, commsToDraw,
                                       progress );
  }

  // Drawmode: Group objects with same wxCoord in objectPosList
  PRV_UINT32 rowToDraw = 0;
  if( myWindow->getWindowBeginTime() != myWindow->getWindowEndTime() )
  {
    for( vector< TObjectOrder >::iterator obj = selectedSet.begin(); obj != selectedSet.end(); ++obj )
    {
      TObjectOrder firstObj = *obj;
      TObjectOrder lastObj = firstObj;
      if( !myWindow->isFusedLinesColorSet() )
      {
        while( ( lastObj + 1 ) <= maxObj && objectPosList[ lastObj + 1 ] == objectPosList[ firstObj ] )
        {
          ++obj;
          lastObj = *obj;
        }
      }

      if( myWindow->isPunctualColorSet() )
      {
        drawRow( bufferDraw, firstObj,
                 valuesToDrawPunctual[ rowToDraw ], eventsToDraw[ rowToDraw ], commsToDraw[ rowToDraw ],
                 eventdc, eventmaskdc, commdc, commmaskdc );
      }
      else
      {
        drawRow( bufferDraw, firstObj,
                 valuesToDraw[ rowToDraw ], eventsToDraw[ rowToDraw ], commsToDraw[ rowToDraw ],
                 eventdc, eventmaskdc, commdc, commmaskdc );
      }
      ++rowToDraw;

      if( myWindow->isPunctualColorSet() )
      {
        if( rowToDraw >= valuesToDrawPunctual.size() )
          break;
      }
      else
      {
        if( rowToDraw >= valuesToDraw.size() )
          break;
      }
    }
  }

  drawZeroAxis( bufferDraw, selectedSet );

#ifdef __WXMAC__
  dc.DrawBitmap( bufferImage, 0, 0, false );
#else
  bufferDraw.SelectObject(wxNullBitmap);
  bufferDraw.SelectObject( drawImage );
  bufferDraw.DrawBitmap( bufferImage, 0, 0, false );
#endif

#ifdef __WXMAC__
  doDrawCaution( dc );
#else
  doDrawCaution( bufferDraw );
#endif

#ifndef __WXMAC__
  eventmaskdc.SetPen( *wxBLACK_PEN );
  eventmaskdc.SetBrush( *wxBLACK_BRUSH );
  eventmaskdc.DrawRectangle( 0, 0, objectAxisPos + 1, drawZone->GetSize().GetHeight() );
//  eventmaskdc.DrawRectangle( drawZone->GetSize().GetWidth() - drawBorder, 0, drawBorder, drawZone->GetSize().GetHeight() );
  eventmaskdc.SelectObject(wxNullBitmap);
  wxMask *mask = new wxMask( eventMask );
  eventImage.SetMask( mask );
#endif

  if( myWindow->getDrawFlags() )
#ifdef __WXMAC__
    dc.DrawBitmap( eventImage, 0, 0, true );
#else
    bufferDraw.DrawBitmap( eventImage, 0, 0, true );
#endif

#ifndef __WXMAC__
  commmaskdc.SetPen( *wxBLACK_PEN );
  commmaskdc.SetBrush( *wxBLACK_BRUSH );
  commmaskdc.DrawRectangle( 0, 0, objectAxisPos + 1, drawZone->GetSize().GetHeight() );
  commmaskdc.DrawRectangle( drawZone->GetSize().GetWidth() - drawBorder, 0, drawBorder, drawZone->GetSize().GetHeight() );
  commmaskdc.DrawRectangle( 0, timeAxisPos, drawZone->GetSize().GetWidth(), drawZone->GetSize().GetHeight() - timeAxisPos );
  commmaskdc.SelectObject(wxNullBitmap);
  mask = new wxMask( commMask );
  commImage.SetMask( mask );
#else
  commdc.SetBrush( *wxBLACK_BRUSH );
  commdc.SetPen( *wxBLACK_PEN );
  commdc.SetLogicalFunction( wxCLEAR );
  commdc.DrawRectangle( 0, 0, objectAxisPos, drawZone->GetSize().GetHeight() );
  commdc.DrawRectangle( drawZone->GetSize().GetWidth() - drawBorder + 1, 0, drawBorder, drawZone->GetSize().GetHeight() );
  commdc.DrawRectangle( 0, timeAxisPos, drawZone->GetSize().GetWidth(), drawZone->GetSize().GetHeight() - timeAxisPos );
  commdc.SetLogicalFunction( wxCOPY );
#endif

  if( myWindow->getDrawCommLines() )
#ifdef __WXMAC__
    dc.DrawBitmap( commImage, 0, 0, true );
#else
    bufferDraw.DrawBitmap( commImage, 0, 0, true );
#endif

  if( gTimeline::dialogProgress != NULL )
  {
    gTimeline::dialogProgress->Show( false );
    delete gTimeline::dialogProgress;
    gTimeline::dialogProgress = NULL;
  }

  if ( progress != NULL )
    delete progress;

#if wxMAJOR_VERSION>=3
  delete gc;
  delete gcEvent;
  delete gcComm;
  #ifndef __WXMAC__
  delete gcCommMask;
  delete gcEventMask;
  #endif
#endif
// Disabled because some window managers can't show the progress dialog later
//  redrawStopWatch->Pause();

  SetTitle( winTitle );

  myWindow->setReady( true );
  
#ifndef __WXMAC__
  drawZone->Refresh();
#endif

  SetFocus();
}


bool gTimeline::drawAxis( wxDC& dc, vector<TObjectOrder>& selected )
{
  // PRV_UINT32 precision = ParaverConfig::getInstance()->getTimelinePrecision();
  PRV_UINT32 precision = 0;

  size_t numObjects = selected.size();
  float magnify = float( myWindow->getPixelSize() );
  
  rgb rgbAxisColour = ((paraverMain *)parent)->GetParaverConfig()->getColorsTimelineAxis();
  wxColour axisColour = wxColour( rgbAxisColour.red, rgbAxisColour.green ,rgbAxisColour.blue );

  dc.SetPen( wxPen( axisColour, 1 ) );
  dc.SetTextForeground( axisColour );

  // Get the text extent for time label
  dc.SetFont( timeFont );
  wxSize timeExt = dc.GetTextExtent( wxString::FromAscii( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( myWindow->getWindowBeginTime() ),
                                                                                       myWindow->getTimeUnit(), precision ).c_str() ) );
  timeAxisPos = dc.GetSize().GetHeight() - ( drawBorder + timeExt.GetHeight() + drawBorder );
  if( timeAxisPos + drawBorder + 1 > dc.GetSize().GetHeight() )
  {
    return false;
  }

  // Get the text extent for the last object (probably the larger one)
  dc.SetFont( objectFont );
  // +1!
  wxSize objectExt, tmpExt;
  int endLevel = THREAD;
  string tmpLongestLabel;
  wxString tmpCurrentLabel;
  size_t tmpMaxLength = 0;
  size_t tmpCurrentMaxLength = 0;

  switch( myWindow->getObjectAxisSize() )
  {
    case Window::CURRENT_LEVEL:
      if( myWindow->isFusedLinesColorSet() )
      {
        objectExt = dc.GetTextExtent( wxString::FromAscii( LabelConstructor::semanticLabel( myWindow,
                                                                                            myWindow->getMaximumY(),
                                                                                            false,
                                                                                            ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() ) ) + wxSize( 5, 0 );
        break;
      }

      tmpCurrentMaxLength = LabelConstructor::objectLabel( myWindow->getTrace()->getLevelObjects( myWindow->getLevel() ) - 1,
                                                           (TWindowLevel) myWindow->getLevel(),
                                                           myWindow->getTrace() ).length();

      tmpMaxLength = myWindow->getTrace()->getMaxLengthRow( myWindow->getLevel() );

      if ( tmpCurrentMaxLength > tmpMaxLength ) // because row label can be empty or shorter
        tmpMaxLength = tmpCurrentMaxLength;
        
      tmpLongestLabel = string( tmpMaxLength, 'Z' );
      objectExt = dc.GetTextExtent( wxString::FromAscii( tmpLongestLabel.c_str() ) );
      break;

    case Window::ALL_LEVELS:
      if( myWindow->isFusedLinesColorSet() )
      {
        objectExt = dc.GetTextExtent( wxString::FromAscii( LabelConstructor::semanticLabel( myWindow,
                                                                                            myWindow->getMaximumY(),
                                                                                            false,
                                                                                            ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() ) ) + wxSize( 5, 0 );
        break;
      }

      if( myWindow->getTrace()->existResourceInfo() )
        endLevel = CPU;
    
      for( int iLevel = WORKLOAD; iLevel <= endLevel; ++iLevel )
      {
        tmpCurrentMaxLength = myWindow->getTrace()->getMaxLengthRow( (TWindowLevel)iLevel );
        tmpCurrentLabel = wxString::FromAscii( LabelConstructor::objectLabel( myWindow->getTrace()->getLevelObjects( (TWindowLevel) iLevel ) - 1,
                                                                                            (TWindowLevel) iLevel,
                                                                                            myWindow->getTrace() ).c_str() );
        if ( tmpCurrentMaxLength < tmpCurrentLabel.Len() )
          tmpCurrentMaxLength = tmpCurrentLabel.Len();
          
        if ( tmpMaxLength < tmpCurrentMaxLength )
          tmpMaxLength = tmpCurrentMaxLength;
      }

      tmpLongestLabel = string( tmpMaxLength, 'Z' );
      objectExt = dc.GetTextExtent( wxString::FromAscii( tmpLongestLabel.c_str() ) );
      break;

    case Window::ZERO_PERC:
      objectExt = wxSize( 0 , 0 );
      break;

    case Window::FIVE_PERC:
      objectExt = wxSize( dc.GetSize().GetWidth() * 0.05 , 0 );
      break;

    case Window::TEN_PERC:
      objectExt = wxSize( dc.GetSize().GetWidth() * 0.10 , 0 );
      break;

    case Window::TWENTYFIVE_PERC:
      objectExt = wxSize( dc.GetSize().GetWidth() * 0.25 , 0 );
      break;

    default:
      break;
  }
  
  objectAxisPos = drawBorder + objectExt.GetWidth() + drawBorder;
  if( objectAxisPos + drawBorder + 1 > dc.GetSize().GetWidth() )
  {
    return false;
  }

  // Draw axis lines
  dc.DrawLine( objectAxisPos, drawBorder,
               objectAxisPos, timeAxisPos );
  dc.DrawLine( objectAxisPos, timeAxisPos,
               dc.GetSize().GetWidth() - drawBorder, timeAxisPos );

  // Draw axis labels
  if( myWindow->isFusedLinesColorSet() )
  {
    dc.DrawText( wxString::FromAscii( LabelConstructor::semanticLabel( myWindow,
                                                                       myWindow->getMaximumY(),
                                                                       false,
                                                                       ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() ),
                 drawBorder, drawBorder );

    dc.DrawText( wxString::FromAscii( LabelConstructor::semanticLabel( myWindow,
                                                                       myWindow->getMinimumY(),
                                                                       false,
                                                                       ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() ),
                 drawBorder, timeAxisPos - objectExt.GetHeight() );

    if( myWindow->getMinimumY() < 0.0 && myWindow->getMaximumY() > 0.0 )
    {
      TSemanticValue relativeZero = myWindow->getMaximumY() / ( myWindow->getMaximumY() - myWindow->getMinimumY() );
      dc.DrawText( wxString::FromAscii( LabelConstructor::semanticLabel( myWindow,
                                                                         0.0,
                                                                         false,
                                                                         ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() ),
                   drawBorder, ( relativeZero * ( timeAxisPos - objectExt.GetHeight() ) ) + drawBorder + 1 );
    }
    else
    {
      dc.DrawText( wxString::FromAscii( LabelConstructor::semanticLabel( myWindow,
                                                                         ( myWindow->getMaximumY() + myWindow->getMinimumY() ) / 2.0,
                                                                         false,
                                                                         ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() ),
                   drawBorder, ( timeAxisPos - objectExt.GetHeight() ) / 2 );
    }
  }
  else
  {
    wxCoord y;
    double inc = (double)( timeAxisPos - drawBorder ) / (double)( numObjects );
    bool drawLabel = true;
    TObjectOrder power2 = 1;

    objectPosList.clear();
    objectPosList.insert( objectPosList.begin(), myWindow->getWindowLevelObjects(), 0 );
    objectHeight = 1;
    vector< TObjectOrder >::iterator it = selected.begin();

    wxCoord accumHeight = 0;
    wxCoord stepHeight = objectExt.GetHeight();
    
    int numpixels = timeAxisPos - 2 * drawBorder; /* Usable num pixels */
    int everyobj = 1; /* Draw every Xth object */
    bool printlast = numpixels > objectFont.GetPointSize(); /* Do we have space for more than 1? */
    bool printall = numpixels > 0 && numpixels > int( numObjects * objectFont.GetPointSize() ); /* Do we have space for all? */

    /* If we have space for some, but not all, check which to print ... */
    if (!printall && printlast)
    {
      /* Locate the maximum 2^n that fit on the pixels */
      int every = 1;
      while( ( numObjects * objectFont.GetPointSize() ) / every > numpixels-objectFont.GetPointSize() )
        every = every << 1;

      every = every << 1;
      /* Now print 2^n equidistant elements */
      if( int( numObjects ) > every )
        everyobj = numObjects / ( numObjects / every );
      else
        everyobj = numObjects;
    }

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

      switch( myWindow->getObjectLabels() )
      {
        case Window::ALL_LABELS:
          drawLabel = true;
          break;
          
        case Window::SPACED_LABELS:
          if( ( printlast && ( obj % everyobj == 0 || obj == numObjects - 1 ) ) ||
              ( !printlast && obj == 0 ) )
            drawLabel = true;
          else
            drawLabel = false;
          //drawLabel = y > accumHeight;
          break;
          
        case Window::POWER2_LABELS:
          if( obj == power2 - 1 )
          {
            drawLabel = true;
            power2 = power2 << 1;
          }
          else
            drawLabel = false;
          break;
        default:
          break;
      }

      if( ( printall || drawLabel ) &&
          !( myWindow->getObjectAxisSize() == Window::ZERO_PERC ) )
      {
        if( myWindow->getLevel() == CPU || myWindow->getLevel() == NODE || myWindow->getLevel() == SYSTEM )
          dc.DrawText( wxString::FromAscii( LabelConstructor::objectLabel( *it + 1, myWindow->getLevel(), myWindow->getTrace() ).c_str() ),
                       drawBorder, y );
        else
          dc.DrawText( wxString::FromAscii( LabelConstructor::objectLabel( *it, myWindow->getLevel(), myWindow->getTrace() ).c_str() ),
                       drawBorder, y );
        accumHeight += stepHeight;
      }

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

  dc.SetPen( wxPen( backgroundColour ) );
  dc.SetBrush( wxBrush( backgroundColour ) );
  dc.DrawRectangle( objectAxisPos + 1, 0, drawZone->GetSize().GetWidth(), timeAxisPos );
               
  return true;
}

void gTimeline::drawZeroAxis( wxDC& dc, vector<TObjectOrder>& selected )
{
  if( myWindow->getMaximumY() > 0.0 && myWindow->getMinimumY() < 0.0 )
  {
    rgb rgbAxisColour = ((paraverMain *)parent)->GetParaverConfig()->getColorsTimelineAxis();
    wxColour axisColour = wxColour( rgbAxisColour.red, rgbAxisColour.green ,rgbAxisColour.blue );
    dc.SetPen( wxPen( axisColour, 1, wxLONG_DASH ) );
    
    TSemanticValue relativeZero = myWindow->getMaximumY() / ( myWindow->getMaximumY() - myWindow->getMinimumY() );

    if( myWindow->isFusedLinesColorSet() )
    {
      dc.DrawLine( objectAxisPos, ( relativeZero * ( timeAxisPos - drawBorder ) ) + drawBorder + 1,
                   dc.GetSize().GetWidth() - drawBorder, ( relativeZero * ( timeAxisPos - drawBorder ) ) + drawBorder + 1 );
    }
    else if( myWindow->isFunctionLineColorSet() || myWindow->isPunctualColorSet() )
    {
      wxCoord lastPos = 0;
      wxCoord relativePos = relativeZero * objectHeight;
      for( vector<TObjectOrder>::iterator it = selected.begin(); it != selected.end(); ++it )
      {
        wxCoord tmpObjPos = objectPosList[ *it ];
        if( tmpObjPos != lastPos )
        {
          lastPos = tmpObjPos;
          dc.DrawLine( objectAxisPos, relativePos + lastPos + 1,
                       dc.GetSize().GetWidth() - drawBorder, relativePos + lastPos + 1 );
          
        }
      }
    }
  }
}

#ifdef WIN32
template<typename ValuesType>
void gTimeline::drawRow( wxDC& dc,
                         TObjectOrder firstRow,
                         vector< ValuesType >& valuesToDraw,
                         hash_set< PRV_INT32 >& eventsToDraw,
                         hash_set< commCoord >& commsToDraw,
                         wxMemoryDC& eventdc, wxMemoryDC& eventmaskdc,
                         wxMemoryDC& commdc, wxMemoryDC& commmaskdc )
#else
template<typename ValuesType>
void gTimeline::drawRow( wxDC& dc,
                         TObjectOrder firstRow,
                         vector< ValuesType >& valuesToDraw,
                         hash_set< PRV_INT32 >& eventsToDraw,
                         hash_set< commCoord, hashCommCoord >& commsToDraw,
                         wxMemoryDC& eventdc, wxMemoryDC& eventmaskdc,
                         wxMemoryDC& commdc, wxMemoryDC& commmaskdc )
#endif
{
  float magnify  = float( myWindow->getPixelSize() );
  if( myWindow->isPunctualColorSet() )
  {
    rgb rgbPunctualColour = ((paraverMain *)parent)->GetParaverConfig()->getColorsTimelinePunctual();
    wxColour punctualColor( rgbPunctualColour.red,
                            rgbPunctualColour.green,
                            rgbPunctualColour.blue );
    dc.SetPen( punctualColor );
    dc.SetBrush( punctualColor );
  }

  wxCoord objectPos;
  if( myWindow->isFusedLinesColorSet() )
    objectPos = 0;
  else
    objectPos = objectPosList[ firstRow ];
  wxCoord timePos   = objectAxisPos + 1;
  int lineLastPos   = 0;

  for( typename vector< ValuesType >::iterator it = valuesToDraw.begin(); it != valuesToDraw.end(); ++it )
  {
    if( myWindow->isPunctualColorSet() )
    {
      drawRowPunctual( dc, *it, objectPos, timePos, magnify );
    }
    else if( myWindow->isCodeColorSet() || myWindow->isGradientColorSet() || myWindow->isNotNullGradientColorSet() )
    {
      drawRowColor( dc, *it, objectPos, timePos, magnify );
    }
    else if( myWindow->isFunctionLineColorSet() )
    {
      drawRowFunction( dc, *it, lineLastPos, objectPos, timePos, magnify );
    }
    else if( myWindow->isFusedLinesColorSet() )
    {
      drawRowFusedLines( dc, *it, lineLastPos, firstRow, timePos, magnify );
    }

    timePos += (int) magnify ;
  }
  
  if( !myWindow->isFusedLinesColorSet() )
  {
    drawRowEvents( eventdc, eventmaskdc, objectPosList[ firstRow ], eventsToDraw );
    drawRowComms( commdc, commmaskdc, objectPosList[ firstRow ], commsToDraw );
  }
}


template<typename ValuesType>
void gTimeline::drawRowColor( wxDC& dc, ValuesType valueToDraw, wxCoord objectPos, wxCoord timePos, float magnify )
{
  // Default implementation should not be called; only intended for compiling
  abort();
}

template<>
void gTimeline::drawRowColor( wxDC& dc, TSemanticValue valueToDraw, wxCoord objectPos, wxCoord timePos, float magnify )
{
  rgb colorToDraw = myWindow->calcColor( valueToDraw, *myWindow );
  
  if( myWindow->isCodeColorSet() && valueToDraw < 0.0 )
    drawCautionNegatives = true;

  // SaveImage & mouse over needed info
  semanticValuesToColor[ valueToDraw ] = colorToDraw;
  semanticColorsToValue[ colorToDraw ].insert( valueToDraw );

  dc.SetPen( wxPen( wxColour( colorToDraw.red, colorToDraw.green, colorToDraw.blue ) ) );

  if ( magnify == 1.0 )
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
    dc.DrawRectangle( timePos , objectPos, magnify , objectHeight );
  }
}


template<typename ValuesType>
void gTimeline::drawRowFunction( wxDC& dc, ValuesType valueToDraw, int& lineLastPos, wxCoord objectPos, wxCoord timePos, float magnify )
{
  // Default implementation should not be called; only intended for compiling
  abort();
}

template<>
void gTimeline::drawRowFunction( wxDC& dc, TSemanticValue valueToDraw, int& lineLastPos, wxCoord objectPos, wxCoord timePos, float magnify )
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
      dc.DrawLine( timePos,           objectPos + objectHeight - currentPos,
                   timePos + magnify, objectPos + objectHeight - currentPos ); 
  }
  else
  {
    if ( magnify == 1.0 )
      dc.DrawPoint( timePos, objectPos + objectHeight - currentPos );
    else
      dc.DrawLine( timePos,           objectPos + objectHeight - currentPos,
                   timePos + magnify, objectPos + objectHeight - currentPos ); 
  }

  lineLastPos = currentPos;
}


template<typename ValuesType>
void gTimeline::drawRowFusedLines( wxDC& dc, ValuesType valueToDraw, int& lineLastPos, TObjectOrder whichObject, wxCoord timePos, float magnify )
{
  // Default implementation should not be called; only intended for compiling
  abort();
}

template<>
void gTimeline::drawRowFusedLines( wxDC& dc, TSemanticValue valueToDraw, int& lineLastPos, TObjectOrder whichObject, wxCoord timePos, float magnify )
{
  if( valueToDraw < myWindow->getMinimumY() )
    valueToDraw = myWindow->getMinimumY();
  else if( valueToDraw > myWindow->getMaximumY() )
    valueToDraw = myWindow->getMaximumY();

  double tmpPos = ( valueToDraw - myWindow->getMinimumY() ) 
                  / ( myWindow->getMaximumY() - myWindow->getMinimumY() );
  int currentPos = ( timeAxisPos - drawBorder ) * tmpPos;

  rgb colorToDraw = myWindow->getCodeColor().calcColor( whichObject + 1, 0, whichObject + 1 );
  dc.SetPen( wxPen( wxColour( colorToDraw.red, colorToDraw.green, colorToDraw.blue ) ) );
  if( currentPos != lineLastPos )
  {
    int from = ( currentPos > lineLastPos ) ? currentPos : lineLastPos;
    int to   = ( currentPos < lineLastPos ) ? currentPos : lineLastPos;
    dc.DrawLine( timePos, timeAxisPos - from,
                 timePos, timeAxisPos - to + 1 );
    if( magnify > 1.0 )
      dc.DrawLine( timePos,           timeAxisPos - currentPos,
                   timePos + magnify, timeAxisPos - currentPos ); 
  }
  else
  {
    if ( magnify == 1.0 )
      dc.DrawPoint( timePos, timeAxisPos - currentPos );
    else
      dc.DrawLine( timePos,           timeAxisPos - currentPos,
                   timePos + magnify, timeAxisPos - currentPos ); 
  }

  lineLastPos = currentPos;
}


template<typename ValuesType>
void gTimeline::drawRowPunctual( wxDC& dc, ValuesType& valuesToDrawList, wxCoord objectPos, wxCoord timePos, float magnify )
{
  // Default implementation should not be called; only intended for compiling
  abort();
}

template<>
void gTimeline::drawRowPunctual( wxDC& dc, vector< pair<TSemanticValue,TSemanticValue> >& valuesToDrawList, wxCoord objectPos, wxCoord timePos, float magnify )
{
  TSemanticValue valueToDraw;
  for( vector< pair<TSemanticValue,TSemanticValue> >::iterator itValues = valuesToDrawList.begin(); itValues != valuesToDrawList.end(); ++itValues )
  {
    valueToDraw = (*itValues).first;

    if( valueToDraw < myWindow->getMinimumY() || valueToDraw > myWindow->getMaximumY() )
      continue;

    double tmpPos = ( valueToDraw - myWindow->getMinimumY() ) 
                    / ( myWindow->getMaximumY() - myWindow->getMinimumY() );
    int currentPos = floor( ( (double)objectHeight / (double)magnify ) * tmpPos ) * magnify;
    
    if( myWindow->getPunctualColorWindow() != NULL )
    {
      TSemanticValue valueToColor = (*itValues).second;
      rgb colorToDraw = myWindow->getPunctualColorWindow()->calcColor( valueToColor, *( myWindow->getPunctualColorWindow() ) );
      
      // SaveImage & mouse over needed info
      semanticValuesToColor[ valueToColor ] = colorToDraw;
      semanticColorsToValue[ colorToDraw ].insert( valueToColor );
    
      dc.SetPen( wxPen( wxColour( colorToDraw.red, colorToDraw.green, colorToDraw.blue ) ) );
      dc.SetBrush( wxBrush( wxColour( colorToDraw.red, colorToDraw.green, colorToDraw.blue ) ) );
    }
    if( magnify == 1.0 )
      dc.DrawPoint( timePos, objectPos + objectHeight - currentPos - 1 );
    else
      dc.DrawRectangle( timePos, objectPos + objectHeight - currentPos - magnify, magnify, magnify );
  }
}


void gTimeline::drawRowEvents( wxDC& eventdc, wxDC& eventmaskdc, TObjectOrder rowPos, hash_set< PRV_INT32 >& eventsToDraw )
{
  for( hash_set< PRV_INT32 >::iterator it = eventsToDraw.begin(); it != eventsToDraw.end(); ++it )
  {
    eventdc.DrawLine( *it, rowPos - 6, *it, rowPos );
    eventdc.DrawLine( *it+1, rowPos - 6, *it+1, rowPos-3 );
    eventdc.DrawLine( *it+2, rowPos - 6, *it+2, rowPos-3 );
    eventdc.DrawLine( *it+3, rowPos - 6, *it+3, rowPos-3 );
    eventdc.DrawLine( *it+4, rowPos - 6, *it+4, rowPos-3 );
#ifndef __WXMAC__
    eventmaskdc.DrawLine( *it, rowPos - 6, *it, rowPos );
    eventmaskdc.DrawLine( *it+1, rowPos - 6, *it+1, rowPos-3 );
    eventmaskdc.DrawLine( *it+2, rowPos - 6, *it+2, rowPos-3 );
    eventmaskdc.DrawLine( *it+3, rowPos - 6, *it+3, rowPos-3 );
    eventmaskdc.DrawLine( *it+4, rowPos - 6, *it+4, rowPos-3 );
#endif
  }

}


#ifdef WIN32
void gTimeline::drawRowComms( wxDC& commdc, wxDC& commmaskdc, TObjectOrder rowPos, hash_set< commCoord >& commsToDraw )
#else
void gTimeline::drawRowComms( wxDC& commdc, wxDC& commmaskdc, TObjectOrder rowPos, hash_set< commCoord, hashCommCoord >& commsToDraw )
#endif
{
#ifdef WIN32
  for( hash_set<commCoord>::iterator it = commsToDraw.begin(); it != commsToDraw.end(); ++it )
#else
  for( hash_set<commCoord,hashCommCoord>::iterator it = commsToDraw.begin(); it != commsToDraw.end(); ++it )
#endif
  {
    if( it->recType & LOG )
      commdc.SetPen( logicalPen );
    else if( it->recType & PHY )
      commdc.SetPen(  physicalPen );

    if( it->toRow != rowPos )
    {
      commdc.DrawLine( it->toTime, it->toRow,
                       it->fromTime, rowPos );
#ifndef __WXMAC__
      commmaskdc.DrawLine( it->toTime, it->toRow,
                           it->fromTime, rowPos );
#endif
    }
    else
    {
      commdc.DrawLine( it->toTime, it->toRow,
                       it->fromTime, rowPos + ( objectHeight / 2 ) );
#ifndef __WXMAC__
      commmaskdc.DrawLine( it->toTime, it->toRow,
                           it->fromTime, rowPos + ( objectHeight / 2 ) );
#endif
    }
  }
}

/*!
 * wxEVT_ERASE_BACKGROUND event handler for ID_SCROLLEDWINDOW
 */
void gTimeline::OnScrolledWindowEraseBackground( wxEraseEvent& event )
{
#ifdef __WXMAC__
  event.Skip();
#endif
}


/*!
 * wxEVT_PAINT event handler for ID_SCROLLEDWINDOW
 */
void gTimeline::OnScrolledWindowPaint( wxPaintEvent& event )
{
  wxPaintDC dc( drawZone );
  
  if( !myWindow->getReady() )
    return;

#ifdef __WXMAC__
  drawStackedImages( dc );
#else
  if( drawImage.IsOk() )
    dc.DrawBitmap( drawImage, 0, 0, false );
#endif
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
#ifndef WIN32
    if( !splitChanged )
    {
#endif
      myWindow->setWidth( event.GetSize().GetWidth() );
      myWindow->setHeight( event.GetSize().GetHeight() );
#ifndef WIN32
    }
#endif
    timerSize->Start( 250, true );
  }
}


/*!
 * wxEVT_IDLE event handler for ID_GTIMELINE
 */
void gTimeline::OnIdle( wxIdleEvent& event )
{
#ifndef WIN32
  if( IsActive() )
    drawZone->SetFocus();
#endif

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
    drawZone->SetClientSize( myWindow->getWidth(), myWindow->getHeight() );
    canRedraw = true;
  }
#else
  canRedraw = true;
#endif

  if( !wxparaverApp::mainWindow->IsIconized() && myWindow->getShowWindow() )
  {
    this->Show();
    if( !myWindow->getReady() )
    {
      if( gTimeline::dialogProgress == NULL )
      {
        myWindow->setReady( true );
        redraw();
      }
    }
  }
  else
  {
    this->Show( false );
  }

  if( myWindow->getRaiseWindow() )
  {
    if( !myWindow->getShowWindow() )
      myWindow->setShowWindow( true );
    else
      this->Raise();
    
    myWindow->setRaiseWindow( false );
  }

// TODO: fix windows placement from cfg with many displays
/*  if( ParaverConfig::??? )
  {
    myWindow->setPosX( this->GetPosition().x );
    myWindow->setPosY( this->GetPosition().y );
  }
  else
*/
  int currentDisplay = wxDisplay::GetFromWindow( this );
  if ( currentDisplay != wxNOT_FOUND && currentDisplay >= 0 )
  {
    wxDisplay tmpDisplay( currentDisplay );
    myWindow->setPosX( this->GetPosition().x - tmpDisplay.GetGeometry().x );
    myWindow->setPosY( this->GetPosition().y - tmpDisplay.GetGeometry().y );
  }

  bool state = false;
  if( myWindow->isFunctionLineColorSet() || myWindow->isPunctualColorSet() || myWindow->isFusedLinesColorSet() )
    state = true;
  initialSemanticLabel->Show( state );
  initialSemanticText->Show( state );
  finalSemanticLabel->Show( state );
  finalSemanticText->Show( state );
  slopeLabel->Show( state );
  slopeText->Show( state );
  timingZone->Layout();
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
    if( drawCautionNegatives )
    {
      wxMessageDialog dialog( this,
                            wxT( "Negative values are drawn as zero in code color view." ),
                            wxT( "Code color negatives" ),
                            wxOK|wxICON_EXCLAMATION );
      dialog.ShowModal();
    }
    else // hidden feature: clicking on botton left corner always fit the semantic scale, even if no caution is shown
      OnPopUpFitSemanticScale();

    return;
  }
  drawZone->SetFocus();
  zooming = true;
  firstMotionEvent = event;
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

  if( escapePressed )
  {
    escapePressed = false;
    return;
  }
  
  if( timing )
  {
    zooming = false;
    return;
  }

  zoomEndX = event.GetX();
  zoomEndY = event.GetY();
  zoomXY = event.ControlDown();

  if( event.ShiftDown() )
  {
    zooming = false;
    zoomXY = false;
    MousePanLeftUp( event );
    return;
  }
  
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

  if( wxGetApp().GetGlobalTiming() )
  {
    if( zooming )
    {
      zooming = false;
      if( beginTime != endTime )
      {
        wxGetApp().SetGlobalTimingBegin( beginTime );
        wxGetApp().SetGlobalTimingEnd( endTime );
        wxGetApp().DeactivateGlobalTiming();
      }
      else
      {
        if( wxGetApp().GetGlobalTimingBeginIsSet() )
        {
          wxGetApp().SetGlobalTimingEnd( endTime );
          wxGetApp().DeactivateGlobalTiming();
        }
        else
        {
          wxGetApp().SetGlobalTimingBegin( endTime );
          wxGetApp().SetGlobalTimingBeginIsSet( true );
        }
      }
    }
    return;
  }
  
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

  if( zooming && myWindow->getReady() &&
      zoomEndX - zoomBeginX > 5.0 &&
      ( ( zoomXY && zoomEndY - zoomBeginY > 5.0 ) || !zoomXY ) )
  {
    if( !zoomXY )
    {
      beginRow = myWindow->getZoomSecondDimension().first;
      endRow =  myWindow->getZoomSecondDimension().second;
    }
    if( ( endTime - beginTime ) < 10 )
      endTime = beginTime + 10;

    // Update window properties
    if( zoomXY && 
        ( selected.size() == 1 && ( myWindow->isFunctionLineColorSet() || myWindow->isPunctualColorSet() ) 
          || myWindow->isFusedLinesColorSet() )
      )
    {
      if( zoomBeginY < drawBorder )
        zoomBeginY = timeAxisPos - drawBorder;
      else
        zoomBeginY = timeAxisPos - zoomBeginY;
      
      if( zoomEndY < drawBorder )
        zoomEndY = timeAxisPos - drawBorder;
      else
        zoomEndY = timeAxisPos - zoomEndY;
      
      TSemanticValue semanticStep = ( myWindow->getMaximumY() - myWindow->getMinimumY() ) /
                                    ( timeAxisPos - drawBorder );
      TSemanticValue beginSemantic = ( semanticStep * zoomEndY ) + myWindow->getMinimumY();
      TSemanticValue endSemantic = ( semanticStep * zoomBeginY ) + myWindow->getMinimumY();
      
      myWindow->setMinimumY( beginSemantic );
      myWindow->setMaximumY( endSemantic );
    }
    else
    {
      myWindow->addZoom( beginTime, endTime, beginRow, endRow );
    }
    myWindow->setWindowBeginTime( beginTime, true );
    myWindow->setWindowEndTime( endTime, true );

    myWindow->setRedraw( true );
    myWindow->setChanged( true );
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
    if( myWindow->getForceRedraw() || ( wxparaverApp::mainWindow->getAutoRedraw() && myWindow->getRedraw() ) )
    {
      if( gTimeline::dialogProgress != NULL )
        return;
      myWindow->setForceRedraw( false );
      myWindow->setRedraw( false );
      splitChanged = false;
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
  wxSize size = wxSize( clonedWindow->getWidth(), clonedWindow->getHeight() );

  string composedName = myWindow->getName() + " @ " +
                        myWindow->getTrace()->getTraceNameNumbered();

//  gTimeline *clonedTimeline = new gTimeline( parent, wxID_ANY, wxT( myWindow->getName().c_str() ), position, size );
  gTimeline *clonedTimeline = new gTimeline( parent, wxID_ANY, wxString::FromAscii( composedName.c_str() ), position );
  clonedTimeline->SetMyWindow( clonedWindow );
  clonedTimeline->SetClientSize( size );
  clonedWindow->setPixelSize( myWindow->getPixelSize() );

  // add to loaded windows list
  LoadedWindows::getInstance()->add( clonedWindow );

  wxChoicebook *choiceWindowBrowser = paraverMain::myParaverMain->choiceWindowBrowser;
  PRV_INT16 currentTrace = paraverMain::myParaverMain->getTracePosition( clonedWindow->getTrace() );
  wxTreeCtrl *allTracesPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 ); // Global page
  wxTreeCtrl *currentPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( currentTrace + 1 ); // Current page

  TreeBrowserItemData *currentData =  new TreeBrowserItemData( wxString::FromAscii( clonedWindow->getName().c_str() ), clonedTimeline );

  int iconNumber = getIconNumber( clonedWindow );

  wxTreeItemId currentWindowId1 = allTracesPage->AppendItem( idRoot1, wxString::FromAscii( clonedWindow->getName().c_str() ), iconNumber, -1, currentData );
  wxTreeItemId currentWindowId2 = currentPage->AppendItem( idRoot2, wxString::FromAscii( clonedWindow->getName().c_str() ), iconNumber, -1, new TreeBrowserItemData( *currentData ) );

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


  clonedTimeline->SetBufferImage( bufferImage );
  clonedTimeline->SetCommImage( commImage );
  clonedTimeline->SetEventImage( eventImage );
  clonedTimeline->SetDrawImage( drawImage );
  clonedTimeline->SetDrawCaution( drawCaution );
  clonedTimeline->SetDrawCautionNegatives( drawCautionNegatives );
  clonedTimeline->SetObjectAxisPos( objectAxisPos );
  clonedTimeline->SetTimeAxisPos( timeAxisPos );
  clonedTimeline->SetObjectPosList( objectPosList );
  clonedTimeline->SetForegroundColour( foregroundColour );
  clonedTimeline->SetBackgroundColour( backgroundColour );
  clonedTimeline->SetSemanticValuesToColor( semanticValuesToColor );
  clonedTimeline->SetSemanticColorsToValue( semanticColorsToValue );
  clonedTimeline->myWindow->setReady( myWindow->getReady() );
  if( mustRedraw )
  {
    if( myWindow->getShowWindow() )
      clonedTimeline->Show();
    else
      clonedTimeline->Show( false );
  }
  else
  {
    clonedWindow->setShowWindow( false );
    clonedTimeline->Show( false );
    clonedTimeline->myWindow->setReady( false );
  }

  return clonedTimeline;
}


void gTimeline::OnPopUpClone()
{
  clone( NULL, parent, getAllTracesTree()->GetRootItem(), getSelectedTraceTree( myWindow->getTrace() )->GetRootItem());
}


void gTimeline::OnPopUpFitTimeScale()
{
  myWindow->setWindowBeginTime( 0, true );
  myWindow->setWindowEndTime( myWindow->getTrace()->getEndTime(), true );
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

void gTimeline::OnPopUpFitObjects()
{
  // Doesn't work!!
  //myWindow->addZoom( TObjectOrder( 0 ), myWindow->getWindowLevelObjects() - 1 );
  myWindow->addZoom( myWindow->getZoomFirstDimension().first, myWindow->getZoomFirstDimension().second,
                     TObjectOrder( 0 ), myWindow->getWindowLevelObjects() - 1 );
  myWindow->setRedraw( true );
  myWindow->setChanged( true );
}

void gTimeline::OnPopUpPunctualColor()
{
  myWindow->setPunctualColorMode();
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpPunctualColorWindow()
{
  vector<Window *> compatWindows;
  int selIndex = 0;
  
  LoadedWindows::getInstance()->getDerivedCompatible( myWindow->getTrace(), compatWindows );
  compatWindows.erase( std::find( compatWindows.begin(), compatWindows.end(), myWindow ) );
  
  wxArrayString choices;
  choices.Add( _( "None" ) );
  int tmpIndex = 1;
  for( vector<Window *>::iterator it = compatWindows.begin(); it != compatWindows.end(); ++it )
  {
    choices.Add( wxString::FromAscii( (*it)->getName().c_str() ) );
    if( (*it) == myWindow->getPunctualColorWindow() )
      selIndex = tmpIndex;
    ++tmpIndex;
  }

  wxSingleChoiceDialog *dialog = new wxSingleChoiceDialog( this, _("Select window to use for coloring points:"), _("Punctual Window"), choices );
  dialog->SetSelection( selIndex );
  if ( dialog->ShowModal() == wxID_OK )
  {
    if( dialog->GetSelection() == 0 )
      myWindow->setPunctualColorWindow( NULL );
    else
      myWindow->setPunctualColorWindow( compatWindows[ dialog->GetSelection() - 1 ] );
    myWindow->setRedraw( true );
  }
  
  delete dialog;
}

void gTimeline::OnPopUpCodeColor()
{
  myWindow->setCodeColorMode();
  myWindow->setRedraw( true );
}


void gTimeline::OnPopUpPasteDefaultSpecial()
{
  gPasteWindowProperties* pasteActions = gPasteWindowProperties::pasteWindowProperties->getInstance();

  pasteActions->paste( this, "Time" );
  myWindow->addZoom( pasteActions->getBeginTime(), pasteActions->getEndTime() );
  pasteActions->paste( this, "Size" );
  pasteActions->paste( this, "Objects" );

  myWindow->setRedraw( true );
  myWindow->setChanged( true );
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

void gTimeline::OnPopUpPasteDuration()
{
  gPasteWindowProperties* pasteActions = gPasteWindowProperties::pasteWindowProperties->getInstance();
  pasteActions->paste( this, "Duration" );
  myWindow->addZoom( pasteActions->getBeginTime(), pasteActions->getEndTime() );
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
    if ( dialog->ShouldChangeTimelineZoom() )
    {
      myWindow->addZoom( dialog->GetNewBeginZoom(), dialog->GetNewEndZoom() );
    }
  
    myWindow->setRedraw( true );
    myWindow->setChanged( true );
  }

  delete dialog;
}


void gTimeline::OnPopUpGradientColor()
{
  myWindow->setGradientColorMode();
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpNotNullGradientColor()
{
  myWindow->setNotNullGradientColorMode();
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

void gTimeline::OnPopUpDrawModeTimeAbsoluteMaximum()
{
  myWindow->setDrawModeTime( DRAW_ABSOLUTE_MAXIMUM );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeTimeAbsoluteMinimumNotZero()
{
  myWindow->setDrawModeTime( DRAW_ABSOLUTE_MINNOTZERO );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeTimeAverage()
{
  myWindow->setDrawModeTime( DRAW_AVERAGE );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeTimeAverageNotZero()
{
  myWindow->setDrawModeTime( DRAW_AVERAGENOTZERO );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeTimeMode()
{
  myWindow->setDrawModeTime( DRAW_MODE );
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

void gTimeline::OnPopUpDrawModeObjectsAbsoluteMaximum()
{
  myWindow->setDrawModeObject( DRAW_ABSOLUTE_MAXIMUM );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeObjectsAbsoluteMinimumNotZero()
{
  myWindow->setDrawModeObject( DRAW_ABSOLUTE_MINNOTZERO );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeObjectsAverage()
{
  myWindow->setDrawModeObject( DRAW_AVERAGE );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeObjectsAverageNotZero()
{
  myWindow->setDrawModeObject( DRAW_AVERAGENOTZERO );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeObjectsMode()
{
  myWindow->setDrawModeObject( DRAW_MODE );
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

void gTimeline::OnPopUpDrawModeBothAbsoluteMaximum()
{
  myWindow->setDrawModeObject( DRAW_ABSOLUTE_MAXIMUM );
  myWindow->setDrawModeTime( DRAW_ABSOLUTE_MAXIMUM );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeBothAbsoluteMinimumNotZero()
{
  myWindow->setDrawModeObject( DRAW_ABSOLUTE_MINNOTZERO );
  myWindow->setDrawModeTime( DRAW_ABSOLUTE_MINNOTZERO );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeBothAverage()
{
  myWindow->setDrawModeObject( DRAW_AVERAGE );
  myWindow->setDrawModeTime( DRAW_AVERAGE );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeBothAverageNotZero()
{
  myWindow->setDrawModeObject( DRAW_AVERAGENOTZERO );
  myWindow->setDrawModeTime( DRAW_AVERAGENOTZERO );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeBothMode()
{
  myWindow->setDrawModeObject( DRAW_MODE );
  myWindow->setDrawModeTime( DRAW_MODE );
  myWindow->setRedraw( true );
}


void gTimeline::OnPopUpPixelSize( PRV_UINT16 whichPixelSize )
{
  myWindow->setPixelSize( whichPixelSize );
  myWindow->setRedraw( true );
}


void gTimeline::OnPopUpUndoZoom()
{
  if ( !myWindow->emptyPrevZoom() )
  {
    TObjectOrder prevZoomBegin = myWindow->getPrevZoomSecondDimension().first;
    TObjectOrder prevZoomEnd = myWindow->getPrevZoomSecondDimension().second;
    vector<TObjectOrder> selectedSet;
    myWindow->getSelectedRows( myWindow->getLevel(), selectedSet, prevZoomBegin, prevZoomEnd, true );
   
    if ( selectedSet.size() > 0 )
    {
      // as before
      myWindow->prevZoom();
      myWindow->setWindowBeginTime( myWindow->getZoomFirstDimension().first, true );
      myWindow->setWindowEndTime( myWindow->getZoomFirstDimension().second, true );
      myWindow->setRedraw( true );
      myWindow->setChanged( true );
    }
    else
    {
      wxString tmpMsg( wxT( "Unable to browse current objects in previous zoom!\n\nPlease select objects." ) );
      wxMessageDialog tmpDialog( NULL, tmpMsg, _( "Warning" ), wxOK | wxICON_EXCLAMATION );
      if ( tmpDialog.ShowModal() == wxID_OK )
      {
        OnPopUpRowSelection();
      }
    }
  }
}


void gTimeline::OnPopUpRedoZoom()
{
  if ( !myWindow->emptyNextZoom() )
  {
    TObjectOrder nextZoomBegin = myWindow->getNextZoomSecondDimension().first;
    TObjectOrder nextZoomEnd = myWindow->getNextZoomSecondDimension().second;
    vector<TObjectOrder> selectedSet;
    myWindow->getSelectedRows( myWindow->getLevel(), selectedSet, nextZoomBegin, nextZoomEnd, true );
   
    if ( selectedSet.size() > 0 )
    {
      // as before
      myWindow->nextZoom();
      myWindow->setWindowBeginTime( myWindow->getZoomFirstDimension().first, true );
      myWindow->setWindowEndTime( myWindow->getZoomFirstDimension().second, true );
      myWindow->setRedraw( true );
      myWindow->setChanged( true );
    }
    else
    {
      wxString tmpMsg( wxT( "Unable to browse current objects in next zoom!\n\nPlease select objects." ) );
      wxMessageDialog tmpDialog( NULL, tmpMsg, _( "Warning" ), wxOK | wxICON_EXCLAMATION );
      if ( tmpDialog.ShowModal() == wxID_OK )
      {
        OnPopUpRowSelection();
      }
    }
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
  if( gTimeline::dialogProgress != NULL )
    return;

  if( wheelZoomFactor != 1.0 )
    return;

  motionEvent = event;
  if( !event.ShiftDown() )
    timerMotion->Start( 20, true );

  wxMemoryDC dc( bufferImage );
  // PRV_UINT32 precision = ParaverConfig::getInstance()->getTimelinePrecision();
  PRV_UINT32 precision = 0;

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
    if( event.ShiftDown() )
    {
      MousePanMotion();
      return;
    }
    
    if( !timing )
      zoomXY = event.ControlDown();
    else
      zoomXY = false;

    long beginX = zoomBeginX > event.GetX() ? event.GetX() : zoomBeginX;
    long beginY = drawBorder;
    long endX = zoomBeginX < event.GetX() ? event.GetX() : zoomBeginX;
    long endY = timeAxisPos;
    if( beginX < objectAxisPos )
      beginX = objectAxisPos;
    if( endX > drawImage.GetWidth() - drawBorder )
      endX = drawImage.GetWidth() - drawBorder;
    wxCoord width = endX - beginX;
    wxCoord height = timeAxisPos - drawBorder + 1;

    if ( zoomXY )
    {
      beginY = zoomBeginY > event.GetY() ? event.GetY() : zoomBeginY;
      endY = zoomBeginY < event.GetY() ? event.GetY() : zoomBeginY;
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
#ifdef __WXMAC__
      if( !zoomBMP.IsOk() ||
          zoomBMP.GetHeight() != drawImage.GetHeight() ||
          zoomBMP.GetWidth() != drawImage.GetWidth() )
        zoomBMP = wxBitmap( drawImage.GetWidth(), drawImage.GetHeight() );
      wxMemoryDC memdc( zoomBMP );
#else
      wxMemoryDC memdc( drawImage );
#endif
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
    vector<TObjectOrder> selectedSet;
    TObjectOrder beginRow = myWindow->getZoomSecondDimension().first;
    TObjectOrder endRow =  myWindow->getZoomSecondDimension().second;
    myWindow->getSelectedRows( myWindow->getLevel(), selectedSet, beginRow, endRow, true );
    if( zoomXY &&
        ( selectedSet.size() == 1 && ( myWindow->isFunctionLineColorSet() || myWindow->isPunctualColorSet() )
          || myWindow->isFusedLinesColorSet() ) )
    {
      if( beginY < drawBorder )
        beginY = timeAxisPos - drawBorder;
      else
        beginY = timeAxisPos - beginY;
      
      if( endY < drawBorder )
        endY = timeAxisPos - drawBorder;
      else
        endY = timeAxisPos - endY;
      
      TSemanticValue semanticStep = ( myWindow->getMaximumY() - myWindow->getMinimumY() ) /
                                    ( timeAxisPos - drawBorder );
      TSemanticValue beginSemantic = ( semanticStep * endY ) + myWindow->getMinimumY();
      TSemanticValue endSemantic = ( semanticStep * beginY ) + myWindow->getMinimumY();
      
      if( myWindow->getMaximumY() - myWindow->getMinimumY() < 1 ) precision = 6;
      initialSemanticText->SetValue( wxString::FromAscii( LabelConstructor::semanticLabel( myWindow, beginSemantic, false, precision ).c_str() ) );
      finalSemanticText->SetValue( wxString::FromAscii( LabelConstructor::semanticLabel( myWindow, endSemantic, false, precision ).c_str() ) );
      slopeText->SetValue( wxString::FromAscii( LabelConstructor::semanticLabel( myWindow, 
                                                                                 ( endSemantic - beginSemantic ) / 
                                                                                 myWindow->traceUnitsToWindowUnits( endTime - beginTime ),
                                                                                 false,
                                                                                 6 ).c_str() ) );
    }
  }
  else if( timing || wxGetApp().GetGlobalTiming() )
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

void gTimeline::OnSplitterTimelineSashUnsplit( wxSplitterEvent& event )
{
  Unsplit();
}


/*!
 * wxEVT_COMMAND_SPLITTER_DOUBLECLICKED event handler for ID_SPLITTERWINDOW
 */

void gTimeline::OnSplitterTimelineSashDClick( wxSplitterEvent& event )
{
  infoZoneLastSize = infoZone->GetSize().GetHeight();
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

void gTimeline::OnNotebookInfoPageChanging( wxNotebookEvent& event )
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


wxString gTimeline::formatTime( TRecordTime whichTime, bool showDate )
{
  wxString formattedTime;
  
  if ( !showDate )
  {
    formattedTime = wxString::FromAscii( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( whichTime ),
                                                                      myWindow->getTimeUnit(), 0 ).c_str() );
  }
  else
  {
    ptime headerTime = myWindow->getTrace()->getTraceTime();
    ptime clickTime = headerTime + boost::posix_time::nanoseconds( (long)myWindow->traceUnitsToCustomUnits( whichTime, NS ) );
    formattedTime = wxString::FromAscii( LabelConstructor::timeLabel( clickTime, 0 ).c_str() );
  }
  
  return formattedTime;
}


// Computes What/Where, filling whatWhereLines vector. Doesn't show it --> printWhatWhere.
void gTimeline::computeWhatWhere( TRecordTime whichTime,
                                  TObjectOrder whichRow,
                                  TSemanticValue whichSemantic,
                                  bool textMode,
                                  bool showDate )
{
  whatWhereTime = whichTime;
  whatWhereRow = whichRow;
  whatWhereSemantic = whichSemantic;
  
  whatWhereLines.clear();
  whatWhereSelectedTimeEventLines = 0;
  whatWhereSelectedTimeCommunicationLines = 0;

  whatWhereLines.push_back( make_pair( BEGIN_OBJECT_SECTION, _( "" )));

  wxString txt;

  if( myWindow->isFusedLinesColorSet() )
  {
    txt << _( "Semantic: " ) << wxString::FromAscii( LabelConstructor::semanticLabel( myWindow,
                                                                                      whichSemantic,
                                                                                      false,
                                                                                      ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() );
  }
  else
  {
    if( myWindow->getLevel() == CPU )
      txt << _( "Object: " ) << wxString::FromAscii( LabelConstructor::objectLabel( whichRow + 1, myWindow->getLevel(), myWindow->getTrace() ).c_str() );
    else
      txt << _( "Object: " ) << wxString::FromAscii( LabelConstructor::objectLabel( whichRow, myWindow->getLevel(), myWindow->getTrace() ).c_str() );
  }
  txt << _( "\t  Click time: " ) << formatTime( whichTime, showDate );
  txt << _( "\n" );
  whatWhereLines.push_back( make_pair( RAW_LINE, txt ) );

  whatWhereLines.push_back( make_pair( END_OBJECT_SECTION, _( "" )));

  if( myWindow->isFusedLinesColorSet() )
    return;

  myWindow->init( whichTime, CREATEEVENTS + CREATECOMMS, false );
  myWindow->initRow( whichRow, whichTime, CREATEEVENTS + CREATECOMMS, false );
  
  TRecordTime tmpBeginTime = myWindow->getBeginTime( whichRow );

  if ( tmpBeginTime > 0.0 )
  {
    myWindow->getRecordList( whichRow )->erase( myWindow->getRecordList( whichRow )->begin(),
                                                myWindow->getRecordList( whichRow )->end() );

    --tmpBeginTime;
    myWindow->init( tmpBeginTime, CREATEEVENTS + CREATECOMMS, false );
    myWindow->initRow( whichRow, tmpBeginTime, CREATEEVENTS + CREATECOMMS, false );
    
    if( myWindow->getEndTime( whichRow ) < myWindow->getTrace()->getEndTime() )
    {
      printWWSemantic( whichRow, false, textMode );
      printWWRecords( whichRow, false, textMode, showDate );
      myWindow->calcNext( whichRow, false );
      while( myWindow->getEndTime( whichRow ) < myWindow->getTrace()->getEndTime() &&
             myWindow->getBeginTime( whichRow ) == myWindow->getEndTime( whichRow ) )
      {
        printWWSemantic( whichRow, false, textMode );
        printWWRecords( whichRow, false, textMode, showDate );
        myWindow->calcNext( whichRow, false );
      }
    }
  }

  printWWSemantic( whichRow, true, textMode );
  printWWRecords( whichRow, true, textMode, showDate );

  if( myWindow->getEndTime( whichRow ) < myWindow->getTrace()->getEndTime() )
  {
    myWindow->calcNext( whichRow, false );
    while( myWindow->getEndTime( whichRow ) < myWindow->getTrace()->getEndTime() &&
           myWindow->getBeginTime( whichRow ) == myWindow->getEndTime( whichRow ) )
    {
      printWWSemantic( whichRow, false, textMode );
      printWWRecords( whichRow, false, textMode, showDate );
      myWindow->calcNext( whichRow, false );
    }
    printWWSemantic( whichRow, false, textMode );
    printWWRecords( whichRow, false, textMode, showDate );
  }
}


void gTimeline::printWhatWhere( )
{
  int fontSize = 10;
  bool allowedLine, allowedSection = false, tooMuchMessage = true;
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
        whatWhereText->BeginItalic();
        break;
      case END_RECORDS_SECTION:
        whatWhereText->EndItalic();
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
              {
                allowedLine = checkWWEvents->IsChecked();
                if( allowedLine ) ++recordsCount;
              }
              break;
            case COMMUNICATION_LINE:
              if( recordsCount >= 100 )
                allowedLine = false;
              else
              {
                allowedLine = checkWWCommunications->IsChecked();
                if( allowedLine ) ++recordsCount;
              }
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
            whatWhereText->WriteText( it->second );
          else if( tooMuchMessage && recordsCount >= 100 )
          {
            whatWhereText->BeginBold();
            whatWhereText->WriteText( _( "Too much records. Reduce time scale.\n" ) );
            whatWhereText->EndBold();
            tooMuchMessage = false;
          }

        break;
      }
    }
  }

  whatWhereText->EndFontSize();
  whatWhereText->EndBold();
  whatWhereText->EndItalic();
  whatWhereText->EndTextColour();

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
void gTimeline::printWWRecords( TObjectOrder whichRow, bool clickedValue, bool textMode, bool showDate )
{
  wxString onString;

  whatWhereLines.push_back( make_pair( BEGIN_RECORDS_SECTION, _( "" )));

  RecordList *rl = myWindow->getRecordList( whichRow );
  RecordList::iterator it = rl->begin();

  while( it != rl->end() &&
         ( (*it).getTime() < myWindow->getWindowBeginTime() ||
           (*it).getTime() < myWindow->getBeginTime( whichRow ) ) )
  {
    ++it;
  }

  if( clickedValue )
    whatWhereLines.push_back( make_pair( BEGIN_CURRENT_SECTION, _( "" )));
  else
    whatWhereLines.push_back( make_pair( BEGIN_PREVNEXT_SECTION, _( "" )));

  TRecordTime timePerPixel = ( myWindow->getWindowEndTime() - myWindow->getWindowBeginTime() ) /
                             ( bufferImage.GetWidth() - objectAxisPos - drawBorder );
  TRecordTime stepTime = timePerPixel * ParaverConfig::getInstance()->getTimelineWhatWhereEventPixels();
  TRecordTime fromTime = whatWhereTime - stepTime;
  if( fromTime < 0.0 ) fromTime = 0.0;
  TRecordTime toTime = whatWhereTime + stepTime;

  while( it != rl->end() &&
         (*it).getTime() < fromTime )
    ++it;

  while( it != rl->end() && 
         (*it).getTime() >= fromTime &&
         (*it).getTime() <= toTime )
  {
    if( (*it).getType() & EVENT )
    {
      onString << wxT( "User Event at " ) << formatTime( (*it).getTime(), showDate );
      onString << wxT( "    " );
      onString << wxString::FromAscii( LabelConstructor::eventLabel( myWindow,
                                                                     (*it).getEventType(),
                                                                     (*it).getEventValue(),
                                                                     textMode ).c_str() );
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
        
      onString << wxT( "at " ) << formatTime( (*it).getTime(), showDate );
      
      if( (*it).getType() & SEND )
        onString << wxT( " to " );
      else if( (*it).getType() & RECV )
        onString << wxT( " from " );
      if ( myWindow->getLevel() >= WORKLOAD && myWindow->getLevel() <= THREAD )
        onString << wxString::FromAscii( LabelConstructor::objectLabel( myWindow->threadObjectToWindowObject( (*it).getCommPartnerObject() ),
                                                                        myWindow->getLevel(),
                                                                        myWindow->getTrace() ).c_str() );
      else
        onString << wxString::FromAscii( LabelConstructor::objectLabel( myWindow->cpuObjectToWindowObject( (*it).getCommPartnerObject() ),
                                                                        myWindow->getLevel(),
                                                                        myWindow->getTrace() ).c_str() );

      onString << wxT( " at " ) << formatTime( (*it).getCommPartnerTime(), showDate );
      
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

  if( !splitter->IsSplit() )
    this->SetClientSize( width, height );
  else
  {
#ifdef WIN32
  this->SetClientSize( width, height + /*infoZone->GetClientSize().GetHeight()*/infoZoneLastSize + 5 );
#endif
    splitter->SetSashPosition( height );
#ifndef WIN32
    drawZone->SetClientSize( width, height );
    this->SetClientSize( width, height + infoZoneLastSize + 5 );
#endif
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
  resizeDrawZone( myWindow->getWidth(), myWindow->getHeight() );
  infoZone->SetClientSize( myWindow->getWidth(), infoZoneLastSize );
  this->Thaw();
  canRedraw = true;
  splitChanged = true;
}


void gTimeline::OnPopUpTiming( bool whichTiming )
{
  SetTiming( whichTiming );
  if( timing )
    drawZone->SetCursor( *wxCROSS_CURSOR );
  else
    drawZone->SetCursor( wxNullCursor  );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_PANEL1
 */

void gTimeline::OnColorsPanelUpdate( wxUpdateUIEvent& event )
{
  if( !myWindow->getReady() )
    return;

  PRV_UINT32 precision = ParaverConfig::getInstance()->getTimelinePrecision();
  
  if( redoColors &&
      ( myWindow->getSemanticInfoType() != lastType ||
        myWindow->getMinimumY() != lastMin ||
        myWindow->getMaximumY() != lastMax ||
        semanticValuesToColor.size() != lastValuesSize ||
        myWindow->isCodeColorSet() != codeColorSet ||
        myWindow->getGradientColor().getGradientFunction() != gradientFunc )
    )
  {
    lastType = myWindow->getSemanticInfoType();
    lastMin = myWindow->getMinimumY();
    lastMax = myWindow->getMaximumY();
    lastValuesSize = semanticValuesToColor.size();
    codeColorSet = myWindow->isCodeColorSet();
    gradientFunc = myWindow->getGradientColor().getGradientFunction();

    colorsSizer->Clear( true );
    wxBoxSizer *itemSizer;
    wxStaticText *itemText;
    wxPanel *itemColor;
    
    if( myWindow->isFusedLinesColorSet() )
    {
      TObjectOrder beginRow = myWindow->getZoomSecondDimension().first;
      TObjectOrder endRow = myWindow->getZoomSecondDimension().second;
      vector<TObjectOrder> selected;
      myWindow->getSelectedRows( myWindow->getLevel(), selected, beginRow, endRow, true );

      size_t i = 0;
      for( vector<TObjectOrder>::iterator it = selected.begin(); it != selected.end(); ++it )
      {
        string tmpstr = LabelConstructor::objectLabel( *it, myWindow->getLevel(), myWindow->getTrace() );

        itemSizer = new wxBoxSizer(wxHORIZONTAL);

        itemText = new wxStaticText( colorsPanel, wxID_ANY, _T("") );
        wxString tmpStr = wxString::FromAscii( tmpstr.c_str() );
        itemText->SetLabel( tmpStr );

        wxSize tmpSize( 20, itemText->GetSize().GetHeight() );
        itemColor = new wxPanel( colorsPanel, wxID_ANY, wxDefaultPosition, tmpSize );
        rgb tmprgb = myWindow->getCodeColor().calcColor( (*it) + 1, 0, myWindow->getTrace()->getLevelObjects( myWindow->getLevel() ) );
        wxColour tmpColor( tmprgb.red, tmprgb.green, tmprgb.blue );
        itemColor->SetBackgroundColour( tmpColor );

        itemSizer->Add( itemColor );
        itemSizer->AddSpacer( 5 );
        itemSizer->Add( itemText );
        colorsSizer->Add( itemSizer, 0, wxGROW|wxALL, 2 );
      
        if( i < selected.size() - 1 )
          colorsSizer->Add( new wxStaticLine( colorsPanel, wxID_ANY ), 0, wxGROW|wxALL, 2 );
        ++i;
      }
    }
    else if( myWindow->isCodeColorSet() )
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
          endLimit = myWindow->getTrace()->totalCPUs();
        else if( lastMax - lastMin > 200 )
          endLimit = 200 + floor( lastMin );
      }
      int typeEndLimit = 0;

      for( int i = floor( lastMin ); i <= endLimit; ++i )
      {
        if( semanticValuesToColor.find( i ) == semanticValuesToColor.end() )
          continue;

        if( lastType == EVENTTYPE_TYPE && !myWindow->getTrace()->eventLoaded( i ) )
          continue;

        string tmpstr;
        if( lastType == STATE_TYPE &&
            !myWindow->getTrace()->getStateLabels().getStateLabel( i, tmpstr ) )
          continue;
        if( lastType == EVENTVALUE_TYPE &&
            !myWindow->getTrace()->getEventLabels().getEventValueLabel( i, tmpstr ) )
          continue;
          
        if( ( lastType == EVENTTYPE_TYPE ||
              lastType == EVENTVALUE_TYPE ||
              lastType == STATE_TYPE )
              && typeEndLimit > 200 )
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
      TSemanticValue lastValueToUse = 0.0;
      for( int i = 0; i <= 20; ++i )
      {
        TSemanticValue valueToUse;
        if( i > 0 && lastValueToUse != 0.0 && ( ( i - 1 ) * step ) + lastMin < 0.0 && ( i * step ) + lastMin > 0.0 )
        {
          lastValueToUse = valueToUse = 0.0;
          --i;
        }
        else
          lastValueToUse = valueToUse = ( i * step ) + lastMin;
        
        itemSizer = new wxBoxSizer(wxHORIZONTAL);

        itemText = new wxStaticText( colorsPanel, wxID_ANY, _T("") );
        tmpStr.Clear();
        tmpStr << wxString::FromAscii( LabelConstructor::semanticLabel( myWindow, valueToUse, false, precision ).c_str() );
        itemText->SetLabel( tmpStr );

        tmpSize = wxSize( 20, itemText->GetSize().GetHeight() );
        itemColor = new wxPanel( colorsPanel, wxID_ANY, wxDefaultPosition, tmpSize );
        tmprgb = myWindow->getGradientColor().calcColor( valueToUse, lastMin, lastMax );
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
  
  if( !myWindow->getReady() )
    return;
  
  bufferDraw.SelectObject(wxNullBitmap);
  bufferDraw.SelectObject( drawImage );
  bufferDraw.DrawBitmap( bufferImage, 0, 0, false );

  doDrawCaution( bufferDraw );

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
  
  if( !myWindow->getReady() )
    return;
  
  bufferDraw.SelectObject(wxNullBitmap);
  bufferDraw.SelectObject( drawImage );
  bufferDraw.DrawBitmap( bufferImage, 0, 0, false );

  doDrawCaution( bufferDraw );

  if( myWindow->getDrawFlags() )
    bufferDraw.DrawBitmap( eventImage, 0, 0, true );

  if( myWindow->getDrawCommLines() )
    bufferDraw.DrawBitmap( commImage, 0, 0, true );

  drawZone->Refresh();
}


void gTimeline::drawFunctionLineColor()
{
  myWindow->setFunctionLineColorMode();
  myWindow->setRedraw( true );
}

void gTimeline::drawFusedLinesColor()
{
  myWindow->setFusedLinesColorMode();
  myWindow->setRedraw( true );
}


// Returns: fileName_with_spaces_underscored@traceName (without extension PRV)
wxString gTimeline::buildFormattedFileName() const
{
  return ( paraverMain::buildFormattedFileName( myWindow->getName(), myWindow->getTrace()->getTraceNameNumbered() ) );
}


void gTimeline::saveImage( bool showSaveDialog, wxString whichFileName )
{
  wxString imagePath;
  ParaverConfig::TImageFormat filterIndex;
  
  if( !whichFileName.IsEmpty() )
  {
    imagePath = whichFileName;
    filterIndex = ParaverConfig::PNG;
  }
  else
  {
    wxString imageName;
    wxString tmpSuffix;
    wxString defaultDir;

    imageName = buildFormattedFileName();
    
  #ifdef WIN32
    defaultDir = _(".\\");
  #else
    defaultDir = _("./");
  #endif

    filterIndex = ParaverConfig::getInstance()->getTimelineSaveImageFormat();
    tmpSuffix = _(".") +
            wxString::FromAscii( LabelConstructor::getImageFileSuffix( filterIndex ).c_str() );
    imagePath = imageName + tmpSuffix;
    
    if( showSaveDialog )
    {
      // Builds following wildcard, but the 'E' in JPEG  
      // _("BMP image|*.bmp|JPEG image|*.jpg|PNG image|*.png|XPM image|*.xpm")
      // Also build extensions vector -> FileDialogExtension
      wxString tmpWildcard;
      std::vector< wxString > extensions;
      for ( PRV_UINT16 i = 0; i <= PRV_UINT16( ParaverConfig::XPM ); ++i )
      {
        wxString currentFormat =
              wxString::FromAscii( LabelConstructor::getImageFileSuffix(
                      ParaverConfig::TImageFormat( i ) ).c_str() );
                      
        extensions.push_back( currentFormat );

        tmpWildcard += currentFormat.Upper() + _(" image|*.") + currentFormat + _("|");
      }
      tmpWildcard = tmpWildcard.BeforeLast( '|' );

      FileDialogExtension saveDialog( this,
                               _("Save Image"),
                               defaultDir,
                               imageName + tmpSuffix,
                               tmpWildcard,
                               wxFD_SAVE | wxFD_CHANGE_DIR,
                               wxDefaultPosition,
                               wxDefaultSize,
                               _( "filedlg" ),
                               extensions );
      saveDialog.SetFilterIndex( filterIndex );
      if ( saveDialog.ShowModal() != wxID_OK )
        return;

      filterIndex = ParaverConfig::TImageFormat( saveDialog.GetFilterIndex() );
      imagePath = saveDialog.GetPath();
    }
  }
  
  // Get title
  wxString longTitle = wxString::FromAscii(
          ( myWindow->getName() + " @ " +
            myWindow->getTrace()->getTraceNameNumbered() ).c_str());
  wxString shortTitle = wxString::FromAscii( ( myWindow->getName() ).c_str() );
  wxString writtenTitle = longTitle;

  // Get colors
  wxColour foregroundColour = GetForegroundColour();
  wxColour backgroundColour = GetBackgroundColour();

  // Get font
  wxFont titleFont = semanticFont;

  // Get dimensions
  wxImage img = drawImage.ConvertToImage();
  int timelineWidth = img.GetWidth();
  int timelineHeight = img.GetHeight();

  int titleMargin = 5; // used in 4 sides
  int titleHeigth = titleFont.GetPointSize() + ( 2 * titleMargin ); // up + down margins + text
  int titleWidth = timelineWidth;
  int titleWritableWidth = titleWidth - ( 2 * titleMargin );

  int imageHeigth = titleHeigth + timelineHeight;
  int imageWidth = timelineWidth;

  // Build DC for title
  wxBitmap titleBitmap( titleWidth, titleHeigth );
  wxMemoryDC titleDC( titleBitmap );

  // Set font and check if using it the title will fit
  titleDC.SetFont( titleFont );
  wxSize titleSize = titleDC.GetTextExtent( writtenTitle );

  if ( titleSize.GetWidth() > titleWritableWidth )
  {
    titleSize = titleDC.GetTextExtent( shortTitle );
    writtenTitle = shortTitle;
  }

  // Set colors
  titleDC.SetBackground( wxBrush( backgroundColour ) );
  titleDC.Clear();

  titleDC.SetPen( wxPen( backgroundColour, 1 ) );
  titleDC.SetTextBackground( backgroundColour );
  titleDC.SetTextForeground( foregroundColour );

  // Compute title image size
  titleDC.DrawText( writtenTitle, titleMargin, titleMargin );

  wxBitmap imageBitmap( imageWidth, imageHeigth );
  wxMemoryDC imageDC( imageBitmap );
  wxCoord xsrc = 0;
  wxCoord ysrc = 0;
  wxCoord xdst = 0;
  wxCoord ydst = 0;
  imageDC.Blit( xdst, ydst, titleWidth, titleHeigth, &titleDC, xsrc, ysrc );

#ifdef __WXMAC__
  wxBitmap tmpDrawImage( drawImage.GetWidth(), drawImage.GetHeight() );
  wxMemoryDC timelineDC( tmpDrawImage );
  drawStackedImages( timelineDC );
#else
  wxMemoryDC timelineDC( drawImage );
#endif
  xsrc = 0;
  ysrc = 0;
  xdst = 0;
  ydst = titleHeigth;
  imageDC.Blit( xdst, ydst, timelineWidth, timelineHeight, &timelineDC, xsrc, ysrc );

  // Get image type and save
#if wxMAJOR_VERSION<3
  long imageType;
#else
  wxBitmapType imageType;
#endif
  switch( filterIndex )
  {
    case ParaverConfig::BMP:
      imageType = wxBITMAP_TYPE_BMP;
      break;
    case ParaverConfig::JPG:
      imageType = wxBITMAP_TYPE_JPEG;
      break;
    case ParaverConfig::PNG:
      imageType = wxBITMAP_TYPE_PNG;
      break;
    case ParaverConfig::XPM:
      imageType = wxBITMAP_TYPE_XPM;
      break;
    default:
      imageType = wxBITMAP_TYPE_PNG;
      break;
  }

  imageDC.SelectObject( wxNullBitmap );
  wxImage baseLayer = imageBitmap.ConvertToImage();

  // Save timeline with gradient scale
  if ( myWindow->isGradientColorSet() || myWindow->isNotNullGradientColorSet() )
  {
    ScaleImageVertical *tmpImage;

    // Create DC for timeline image
    wxMemoryDC tmpTimelineDC( imageBitmap ); // reuse imageDC?

    // Create DC for scale
    int wantedWidth = baseLayer.GetWidth();
    int backgroundMode = wxSOLID;
    tmpImage = new ScaleImageHorizontalGradientColor(
            myWindow,
            semanticValuesToColor,
            backgroundColour, foregroundColour, backgroundMode,
            titleFont,
            imagePath, wxString( _( "horiz.labels" ) ),
            imageType,
            wantedWidth );
    tmpImage->process();
    //tmpImage->save();

    wxBitmap *tmpScale = tmpImage->getBitmap();
    wxMemoryDC tmpScaleDC( *tmpScale );

    // Create DC for destiny image
    int totalHeigth = baseLayer.GetHeight() + tmpScale->GetHeight();
    int totalWidth  = baseLayer.GetWidth() > tmpScale->GetWidth() ? baseLayer.GetWidth(): tmpScale->GetWidth();
    wxBitmap tmpScaledTimelineBitmap( totalWidth, totalHeigth );
    wxMemoryDC tmpScaledTimelineDC( tmpScaledTimelineBitmap );
    tmpScaledTimelineDC.SetBackground( backgroundColour );
    tmpScaledTimelineDC.Clear();

    // Copy to destiny
    int xsrc = 0;
    int ysrc = 0;
    int xdst = 0;
    int ydst = 0;

    tmpScaledTimelineDC.Blit( xdst, ydst,
                             imageBitmap.GetWidth(), imageBitmap.GetHeight(),
                             &tmpTimelineDC,
                             xsrc, ysrc );

    xdst = totalWidth - tmpScale->GetWidth();
    if ( xdst < 0 )
    {
      xdst = 0;
    }
    ydst = imageBitmap.GetHeight();
    tmpScaledTimelineDC.Blit( xdst, ydst,
                              tmpScale->GetWidth(), tmpScale->GetHeight(),
                              &tmpScaleDC,
                              xsrc, ysrc );

    //tmpScaledTimelineDC->SelectObject( wxNullBitmap );

    wxImage tmpScaledTimeline( tmpScaledTimelineBitmap.ConvertToImage() );
    wxString currentFormat =
            wxString::FromAscii( LabelConstructor::getImageFileSuffix(
                    ParaverConfig::TImageFormat( filterIndex ) ).c_str() );
    wxString tmpScaledTimelinePath = wxFileName( imagePath ).GetPathWithSep() +
                                     wxFileName( imagePath ).GetName() +
                                     wxString( _(".w_legend.") ) +
                                     currentFormat;
    tmpScaledTimeline.SaveFile( tmpScaledTimelinePath, imageType );
  }

  // Save timeline image without scale
  baseLayer.SaveFile( imagePath, imageType );  

}


void gTimeline::saveImageLegend( bool showSaveDialog )
{
  wxString imageName;
  wxString tmpSuffix;
  wxString defaultDir;

  imageName = buildFormattedFileName();
  
#ifdef WIN32
  defaultDir = _(".\\");
#else
  defaultDir = _("./");
#endif

  ParaverConfig::TImageFormat filterIndex = ParaverConfig::getInstance()->getTimelineSaveImageFormat();
  tmpSuffix = _(".");
  if ( myWindow->isGradientColorSet() )
     tmpSuffix +=
            wxString( _( "gradient" ) ) +
            _(".") +
            wxString::FromAscii( LabelConstructor::getImageFileSuffix( filterIndex ).c_str() );
  else if ( myWindow->isNotNullGradientColorSet() )
     tmpSuffix +=
            wxString( _( "nn_gradient" ) ) +
            _(".") +
            wxString::FromAscii( LabelConstructor::getImageFileSuffix( filterIndex ).c_str() );
  else
    tmpSuffix +=
            wxString( _( "code" ) ) +
            _(".") +
            wxString::FromAscii( LabelConstructor::getImageFileSuffix( filterIndex ).c_str() );
  
  wxString imagePath = imageName + tmpSuffix;
  
  if( showSaveDialog )
  {
    // Builds following wildcard, but the 'E' in JPEG  
    // _("BMP image|*.bmp|JPEG image|*.jpg|PNG image|*.png|XPM image|*.xpm")
    // Also build extensions vector -> FileDialogExtension
    wxString tmpWildcard;
    std::vector< wxString > extensions;
    for ( PRV_UINT16 i = 0; i <= PRV_UINT16( ParaverConfig::XPM ); ++i )
    {
      wxString currentFormat =
            wxString::FromAscii( LabelConstructor::getImageFileSuffix(
                    ParaverConfig::TImageFormat( i ) ).c_str() );
                    
      extensions.push_back( currentFormat );
      
      tmpWildcard += currentFormat.Upper() + _(" image|*.") + currentFormat + _("|");
    }
    tmpWildcard = tmpWildcard.BeforeLast( '|' );

    FileDialogExtension saveDialog( this,
                             _("Save Image Legend"),
                             defaultDir,
                             imageName + tmpSuffix,
                             tmpWildcard,
                             wxFD_SAVE | wxFD_CHANGE_DIR,
                             wxDefaultPosition,
                             wxDefaultSize,
                             _( "filedlg" ),
                             extensions );
    saveDialog.SetFilterIndex( filterIndex );
    if ( saveDialog.ShowModal() != wxID_OK )
      return;
      
    filterIndex = ParaverConfig::TImageFormat( saveDialog.GetFilterIndex() );
    imagePath = saveDialog.GetPath();
  }
  
  // Get colors
  wxColour foregroundColour = GetForegroundColour();
  wxColour backgroundColour = GetBackgroundColour();

  // Get font
  wxFont titleFont = semanticFont;

  // Get image type and save
#if wxMAJOR_VERSION<3
  long imageType;
#else
  wxBitmapType imageType;
#endif
  int backgroundMode = wxTRANSPARENT; // default
  switch( filterIndex )
  {
    case ParaverConfig::BMP:
      imageType = wxBITMAP_TYPE_BMP;
      backgroundMode = wxSOLID;
      break;
    case ParaverConfig::JPG:
      imageType = wxBITMAP_TYPE_JPEG;
      backgroundMode = wxSOLID;
      break;
    case ParaverConfig::PNG:
      imageType = wxBITMAP_TYPE_PNG;
      break;
    case ParaverConfig::XPM:
      imageType = wxBITMAP_TYPE_XPM;
      break;
    default:
      imageType = wxBITMAP_TYPE_PNG;
      break;
  }

  ScaleImageVertical *tmpImage;
  if ( myWindow->isGradientColorSet() || myWindow->isNotNullGradientColorSet() )
  {
    tmpImage = new ScaleImageHorizontalGradientColor( myWindow, semanticValuesToColor,
                                                       //backgroundColour, foregroundColour, backgroundMode,
                                                       *wxWHITE, *wxBLACK, backgroundMode,
                                                       titleFont,
                                                       //imagePath, wxString( _( "horiz.labels.transp" ) ),
                                                       imagePath, wxString( _( "" ) ),
                                                       imageType );
    tmpImage->process();
    tmpImage->save();
    delete tmpImage;
   }
  else if ( myWindow->isCodeColorSet() )
  {
    tmpImage = new ScaleImageVerticalCodeColor( myWindow, semanticValuesToColor,
                                                 //backgroundColour, foregroundColour, backgroundMode,
                                                 *wxWHITE, *wxBLACK, backgroundMode,
                                                 titleFont,
                                                 // imagePath, wxString( _( "vert.labels.transp" ) ),
                                                 imagePath, wxString( _( "" ) ),
                                                 imageType );
    tmpImage->process();
    tmpImage->save();
    delete tmpImage;
  }
  else if ( myWindow->isFusedLinesColorSet() )
  {
    std::map< TSemanticValue, rgb > tmpObjects;

    TObjectOrder beginRow = myWindow->getZoomSecondDimension().first;
    TObjectOrder endRow = myWindow->getZoomSecondDimension().second;
    vector<TObjectOrder> selected;
    myWindow->getSelectedRows( myWindow->getLevel(), selected, beginRow, endRow, true );

    for( vector<TObjectOrder>::iterator it = selected.begin(); it != selected.end(); ++it )
    {
      rgb tmprgb = myWindow->getCodeColor().calcColor( (*it) + 1, 0, myWindow->getTrace()->getLevelObjects( myWindow->getLevel() ) - 1 );
      tmpObjects[ (TSemanticValue)(*it) ] = tmprgb;
    }
    tmpImage = new ScaleImageVerticalFusedLines( myWindow, tmpObjects,
                                                 //backgroundColour, foregroundColour, backgroundMode,
                                                 *wxWHITE, *wxBLACK, backgroundMode,
                                                 titleFont,
                                                 // imagePath, wxString( _( "vert.labels.transp" ) ),
                                                 imagePath, wxString( _( "" ) ),
                                                 imageType );
    tmpImage->process();
    tmpImage->save();
    delete tmpImage;
  }
}


//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//
// ScaleImage
//
gTimeline::ScaleImageVertical::ScaleImageVertical(
        Window* whichMyWindow,
        const std::map< TSemanticValue, rgb >& whichSemanticValues,
        wxColour whichBackground,
        wxColour whichForeground,
        int whichBackgroundMode,
        wxFont whichTextFont,
        wxString& whichImagePath,
        const wxString& whichImageInfix,
#if wxMAJOR_VERSION<3
        long whichImageType
#else
        wxBitmapType& whichImageType 
#endif
        ) : myWindow( whichMyWindow ),
            semValues( whichSemanticValues ),
            background( whichBackground ),
            foreground( whichForeground ),
            backgroundMode( whichBackgroundMode ),
            textFont( whichTextFont ),
            imagePath( whichImagePath ),
            imageInfix( whichImageInfix ),
            imageType( whichImageType )
{
}


gTimeline::ScaleImageVertical::~ScaleImageVertical()
{
  destroyDC();
}


wxString gTimeline::ScaleImageVertical::buildScaleImagePath()
{
  wxString scaleImagePath;

  if ( imageInfix.IsEmpty() )
    scaleImagePath = imagePath;
  else
    scaleImagePath =
            wxFileName( imagePath ).GetPathWithSep() +
            wxFileName( imagePath ).GetName() +
            _(".") + imageInfix + _(".") + tmpSuffix;

  return scaleImagePath;
}


void gTimeline::ScaleImageVertical::bitmapToImage()
{
  scaleDC->SelectObject( wxNullBitmap );

#ifndef __WXMAC__
  // TODO: avoid to create/handle scaleMaskDC in all the other methods if wxTRANSPARENT
  if ( backgroundMode == wxTRANSPARENT )
  {
    scaleMaskDC->SelectObject( wxNullBitmap );
    wxMask *mask  = new wxMask( *scaleMaskBitmap, *wxWHITE );
    scaleBitmap->SetMask( mask );
  }
#endif

  scaleImage = new wxImage( scaleBitmap->ConvertToImage() );
}


void gTimeline::ScaleImageVertical::process()
{
  init();
  sortSemanticValues();
  computeMaxLabelSize();
  computeImageSize();
  createDC();
  draw();
  bitmapToImage();
}


void gTimeline::ScaleImageVertical::save()
{
  wxString scaleImagePath = buildScaleImagePath();
  scaleImage->SaveFile( scaleImagePath, imageType );

// Test code for transparency
#if 0
  ::wxInitAllImageHandlers();
  wxBitmap bmp( 64, 64, 32 );
  //bmp.UseAlpha();
  wxBitmap maskbmp( 64, 64, 32 );

  wxMemoryDC dc( bmp );
  //dc.SetBackgroundMode( wxTRANSPARENT );
  dc.SetBackground( *wxWHITE_BRUSH/**wxTRANSPARENT_BRUSH*/ );
  dc.SetBrush( *wxWHITE_BRUSH/**wxTRANSPARENT_BRUSH*/ );
  dc.Clear();
  
  //wxGCDC dc( memDC );
//  wxMemoryDC dc( bmp );
  wxMemoryDC maskDC( maskbmp );

  /*dc.SetBackground( *wxTRANSPARENT_BRUSH );
  dc.Clear();*/
  dc.SetFont( textFont );
  maskDC.SetBackground( *wxBLACK_BRUSH );
  maskDC.Clear();
  maskDC.SetBrush( *wxWHITE_BRUSH );
  maskDC.SetPen( *wxWHITE_PEN );
  maskDC.SetTextForeground( *wxWHITE );
  maskDC.SetFont( textFont );

  dc.SetBrush( *wxRED_BRUSH );
  dc.SetPen( *wxBLACK_PEN );
  dc.DrawRectangle( 10, 30, 10, 10 );
  maskDC.DrawRectangle( 10, 30, 10, 10 );
  dc.SetTextForeground( *wxBLACK );
  dc.DrawText( wxT( "HELLO" ), 10, 10 );
  maskDC.DrawText( wxT( "HELLO" ), 10, 10 );
  dc.SelectObject( wxNullBitmap );
  maskDC.SelectObject( wxNullBitmap );
  maskbmp.SaveFile( wxT( "./mask.png" ), wxBITMAP_TYPE_PNG );
  wxMask *tmpMask = new wxMask( maskbmp, *wxBLACK );
  bmp.SetMask( tmpMask );
  bmp.SaveFile( wxT( "./test.png" ), wxBITMAP_TYPE_PNG );
#endif
}


void gTimeline::ScaleImageVertical::init()
{
  filterIndex = ParaverConfig::getInstance()->getTimelineSaveImageFormat();
  //tmpSuffix = _("as_list.");
  tmpSuffix = _("labels.");
  tmpSuffix += wxString::FromAscii( LabelConstructor::getImageFileSuffix( filterIndex ).c_str() );
  
  currentMin = myWindow->getMinimumY();
  currentMax = myWindow->getMaximumY();
  
  precision = ParaverConfig::getInstance()->getTimelinePrecision();
  symbolicDesc = true;
  
  //numSquares = 20;
  extraPrefixOutlier = wxT("");

  // n labels * label height + top + bottom
  titleMargin = 5; // used in 4 sides
  
  // Colored rectangle size
  widthRect = 10;
  heightRect = textFont.GetPointSize() + 3;

  imageStepY = textFont.GetPointSize() + 2 * titleMargin;
  imageStepXRectangle = widthRect;
  
  // Loop through labels
  xdst = titleMargin;
  ydst = titleMargin;
}


void gTimeline::ScaleImageVertical::computeMaxLabelSize()
{
  // Dimensions: loop for every value to build labels and measure the maximum
  wxString curLabel;
  wxString maxLabel;
  size_t maxLengthLabel = 0;
  size_t curLengthLabel;
  for ( std::vector< TSemanticValue >::iterator it = keys.begin(); it != keys.end(); ++it )
  {
    // Get Labels
    curLabel = wxString::FromAscii( LabelConstructor::semanticLabel( myWindow, *it, symbolicDesc, precision ).c_str() );
    semanticValueLabel[ *it ] = curLabel;

    // Get Longest label
    curLengthLabel = curLabel.Len();
    if ( maxLengthLabel < curLengthLabel )
    {
      maxLengthLabel = curLengthLabel;
      semanticValueWithLongestLabel = *it;
    }
  }

  // Guess height and width of longest label
  wxBitmap maxLabelBitmap( 1920, 64 );
  wxMemoryDC maxLabelDC( maxLabelBitmap );
  maxLabelDC.SetFont( textFont );
  maxLabelSize = maxLabelDC.GetTextExtent( extraPrefixOutlier +
                                           semanticValueLabel[ semanticValueWithLongestLabel ] );
}


void gTimeline::ScaleImageVertical::sortSemanticValues()
{
  for ( std::map< TSemanticValue, rgb >::iterator it = semValues.begin(); it != semValues.end(); ++it )
  {
    keys.push_back( it->first );
  }
  std::sort( keys.begin(), keys.end() );
}


void gTimeline::ScaleImageVertical::computeImageSize()
{
  // Guess height
  imageHeight = semanticValueLabel.size() * imageStepY + ( 2 * titleMargin );

  if ( drawOutliers )
    imageHeight += ( imageStepY + titleMargin ) * 2;

  imageWidth = maxLabelSize.GetWidth() + ( 2 * titleMargin ) + ( widthRect + titleMargin );
}


void gTimeline::ScaleImageVertical::createDC()
{
  // Legend
  scaleBitmap = new wxBitmap( imageWidth, imageHeight );
  scaleDC = new wxMemoryDC( *scaleBitmap );

  scaleDC->SetFont( textFont );
  if ( backgroundMode == wxSOLID )
  {
    scaleDC->SetBackground( background );
    scaleDC->SetTextBackground( background );
    scaleDC->SetTextForeground( foreground );
    scaleDC->SetPen( foreground );
  }
  else
  {
    scaleDC->SetBackground( *wxWHITE_BRUSH );
    scaleDC->SetTextForeground( *wxBLACK );
  }

  scaleDC->Clear();
  
  // Mask for legend
  scaleMaskBitmap = new wxBitmap( imageWidth, imageHeight );
  scaleMaskDC = new wxMemoryDC( *scaleMaskBitmap );
  scaleMaskDC->SetBackground( *wxWHITE_BRUSH );
  scaleMaskDC->SetBrush( *wxBLACK_BRUSH );
  scaleMaskDC->SetPen( *wxBLACK_PEN );
  scaleMaskDC->SetTextForeground( *wxBLACK );
  scaleMaskDC->SetFont( textFont );
  scaleMaskDC->Clear();
}


void gTimeline::ScaleImageVertical::draw()
{
  for ( std::vector< TSemanticValue >::iterator it = keys.begin(); it != keys.end(); ++it )
  {
    drawLabeledRectangle( semValues[ *it ], semanticValueLabel[ *it ] );
    ydst += imageStepY;
  }
}


void gTimeline::ScaleImageVertical::drawLabeledRectangle( rgb semanticColour,
                                                           wxString semanticValueLabel,
                                                           bool drawIt )
{
  scaleDC->SetBrush( wxColour( semanticColour.red,
                                semanticColour.green,
                                semanticColour.blue ) );
  scaleDC->DrawRectangle( xdst, ydst, widthRect, heightRect );
  scaleMaskDC->DrawRectangle( xdst, ydst, widthRect, heightRect );

  if ( drawIt )
  {
    // Write text
    scaleDC->DrawText( semanticValueLabel, xdst + widthRect + titleMargin, ydst );
    scaleMaskDC->DrawText( semanticValueLabel, xdst + widthRect + titleMargin, ydst );
  }
}


void gTimeline::ScaleImageVertical::destroyDC()
{
  delete scaleDC;
  delete scaleMaskDC;
  delete scaleBitmap;
  delete scaleMaskBitmap;
  delete scaleImage;
}


//---------------------------------------------------------------------------------------------------
//
// ScaleImageVerticalCodeColor
//
gTimeline::ScaleImageVerticalCodeColor::ScaleImageVerticalCodeColor(
        Window* whichMyWindow, 
        const std::map< TSemanticValue, rgb >& whichSemanticValues,
        wxColour whichBackground,
        wxColour whichForeground,
        int whichBackgroundMode,
        wxFont whichTextFont,
        wxString& whichImagePath,
        const wxString& whichImageInfix,
#if wxMAJOR_VERSION<3
        long whichImageType
#else
        wxBitmapType& whichImageType 
#endif
        ) : ScaleImageVertical( whichMyWindow,
                                whichSemanticValues,
                                whichBackground,
                                whichForeground,
                                whichBackgroundMode,
                                whichTextFont,
                                whichImagePath,
                                whichImageInfix,
                                whichImageType )
{
}


void gTimeline::ScaleImageVerticalCodeColor::init()
{
  gTimeline::ScaleImageVertical::init();

  drawOutliers = false;
  symbolicDesc = true;
}


//---------------------------------------------------------------------------------------------------
//
// ScaleImageVerticalGradientColor
//
gTimeline::ScaleImageVerticalGradientColor::ScaleImageVerticalGradientColor(
        Window* whichMyWindow, 
        const std::map< TSemanticValue, rgb >& whichSemanticValues,
        wxColour whichBackground,
        wxColour whichForeground,
        int whichBackgroundMode,
        wxFont whichTextFont,
        wxString& whichImagePath,
        const wxString& whichImageInfix,
#if wxMAJOR_VERSION<3
        long whichImageType
#else
        wxBitmapType& whichImageType 
#endif
        ) : ScaleImageVertical( whichMyWindow,
                                whichSemanticValues,
                                whichBackground,
                                whichForeground,
                                whichBackgroundMode,
                                whichTextFont,
                                whichImagePath,
                                whichImageInfix,
                                whichImageType )
{
}


void gTimeline::ScaleImageVerticalGradientColor::init()
{
  gTimeline::ScaleImageVertical::init();

  drawOutliers = true;
  symbolicDesc = false;
  
  numSquaresWithoutOutliers = 20;
  if ( myWindow->isNotNullGradientColorSet() )
    numSquaresWithoutOutliers++; // 0 black
  totalSquares = numSquaresWithoutOutliers + 2; // gradient and outliers

  extraPrefixOutlier = wxT( "< " );
}


void gTimeline::ScaleImageVerticalGradientColor::sortSemanticValues()
{
  semValues.clear();
  TSemanticValue step = ( currentMax - currentMin ) / double( numSquaresWithoutOutliers - 1 ); //?
  for( int i = 0; i < numSquaresWithoutOutliers; ++i )
  {
    TSemanticValue current = ( i * step ) + currentMin;
    keys.push_back( current );
    semValues[ current ] = myWindow->getGradientColor().calcColor( current, currentMin, currentMax );
  }
  std::sort( keys.begin(), keys.end() );
}


void gTimeline::ScaleImageVerticalGradientColor::draw()
{
  // Bottom Outlier
  rgb tmprgb = myWindow->getGradientColor().getBelowOutlierColor();
  wxString tmpSemanticValueLabel =
           wxT( "< " ) +
           wxString::FromAscii( LabelConstructor::semanticLabel( myWindow, keys[0], symbolicDesc, precision ).c_str() );
  drawLabeledRectangle( tmprgb, tmpSemanticValueLabel );
  ydst += imageStepY;
  
  // Values
  for ( std::vector< TSemanticValue >::iterator it = keys.begin(); it != keys.end(); ++it )
  {
    drawLabeledRectangle( semValues[ *it ], semanticValueLabel[ *it ] );   
    ydst += imageStepY;
  }

  // Top outlier
  tmprgb = myWindow->getGradientColor().getAboveOutlierColor();
  tmpSemanticValueLabel =
          wxT( "> " ) +
          wxString::FromAscii( LabelConstructor::semanticLabel( myWindow, keys.back(), symbolicDesc, precision ).c_str() );
  drawLabeledRectangle( tmprgb, tmpSemanticValueLabel );
  ydst += imageStepY;
}


//---------------------------------------------------------------------------------------------------
//
// ScaleImageVerticalFusedLines
//
gTimeline::ScaleImageVerticalFusedLines::ScaleImageVerticalFusedLines(
        Window* whichMyWindow, 
        const std::map< TSemanticValue, rgb >& whichSemanticValues,
        wxColour whichBackground,
        wxColour whichForeground,
        int whichBackgroundMode,
        wxFont whichTextFont,
        wxString& whichImagePath,
        const wxString& whichImageInfix,
#if wxMAJOR_VERSION<3
        long whichImageType
#else
        wxBitmapType& whichImageType 
#endif
        ) : ScaleImageVertical( whichMyWindow,
                                whichSemanticValues,
                                whichBackground,
                                whichForeground,
                                whichBackgroundMode,
                                whichTextFont,
                                whichImagePath,
                                whichImageInfix,
                                whichImageType )
{
}


void gTimeline::ScaleImageVerticalFusedLines::init()
{
  gTimeline::ScaleImageVertical::init();

  currentMin = 0;
  currentMax = myWindow->getTrace()->getLevelObjects( myWindow->getLevel() ) - 1;
}

void gTimeline::ScaleImageVerticalFusedLines::computeMaxLabelSize()
{
  // Dimensions: loop for every value to build labels and measure the maximum
  wxString curLabel;
  wxString maxLabel;
  size_t maxLengthLabel = 0;
  size_t curLengthLabel;
  for ( std::vector< TSemanticValue >::iterator it = keys.begin(); it != keys.end(); ++it )
  {
    // Get Labels
    curLabel = wxString::FromAscii( LabelConstructor::objectLabel( *it, myWindow->getLevel(), myWindow->getTrace() ).c_str() );
    semanticValueLabel[ *it ] = curLabel;

    // Get Longest label
    curLengthLabel = curLabel.Len();
    if ( maxLengthLabel < curLengthLabel )
    {
      maxLengthLabel = curLengthLabel;
      semanticValueWithLongestLabel = *it;
    }
  }

  // Guess height and width of longest label
  wxBitmap maxLabelBitmap( 1920, 64 );
  wxMemoryDC maxLabelDC( maxLabelBitmap );
  maxLabelDC.SetFont( textFont );
  maxLabelSize = maxLabelDC.GetTextExtent( extraPrefixOutlier +
                                           semanticValueLabel[ semanticValueWithLongestLabel ] );
}

//---------------------------------------------------------------------------------------------------
//
// ScaleImageHorizontalGradientColor
//
gTimeline::ScaleImageHorizontalGradientColor::ScaleImageHorizontalGradientColor(
        Window* whichMyWindow, 
        const std::map< TSemanticValue, rgb >& whichSemanticValues,
        wxColour whichBackground,
        wxColour whichForeground,
        int whichBackgroundMode,
        wxFont whichTextFont,
        wxString& whichImagePath,
        const wxString& whichImageInfix,
#if wxMAJOR_VERSION<3
        long whichImageType,
#else
        wxBitmapType& whichImageType,
#endif
        int whichWantedWidth
        ) : ScaleImageVerticalGradientColor( whichMyWindow,
                                             whichSemanticValues,
                                             whichBackground,
                                             whichForeground,
                                             whichBackgroundMode,
                                             whichTextFont,
                                             whichImagePath,
                                             whichImageInfix,
                                             whichImageType )
{
  wantedWidth = whichWantedWidth;
}


void gTimeline::ScaleImageHorizontalGradientColor::init()
{
  gTimeline::ScaleImageVerticalGradientColor::init();
  //tmpSuffix = _("as_row.");
  //tmpSuffix += wxString::FromAscii( LabelConstructor::getImageFileSuffix( filterIndex ).c_str() );

  widthRect = 20;
  imageStepXRectangle = widthRect;
  outlierMargin = 2 * titleMargin;
  SIZE_OF_TINY_MARK = 4;
}


void gTimeline::ScaleImageHorizontalGradientColor::computeImageSize()
{
  // Heigth
  imageHeight = heightRect + maxLabelSize.GetHeight() + 3 * titleMargin;

  // Width
  if ( wantedWidth > 0 )
  {
    imageWidth = totalSquares * widthRect + 2 * outlierMargin + 2 * titleMargin;
    if ( imageWidth > wantedWidth )
    {
      float MINIMUM_WIDTH_RECT = 10.0;
      float variableWidthRect = (float)(( wantedWidth - 2 * outlierMargin - 2 * titleMargin ) / totalSquares );
      while (( totalSquares > 0 ) &&
             ( variableWidthRect < MINIMUM_WIDTH_RECT ) &&
             ( variableWidthRect > 0.0 ))
      {
        --totalSquares;
        variableWidthRect = (float)(( wantedWidth - 2 * outlierMargin - 2 * titleMargin ) / totalSquares );
      }

      widthRect = rint( variableWidthRect );
      imageStepXRectangle = widthRect;
    }
    else if ( imageWidth < wantedWidth )
    {
      int extraSquares = rint( floorf( float( ( wantedWidth - imageWidth ) / widthRect )));
      totalSquares += extraSquares;

      if ( myWindow->isNotNullGradientColorSet() )
        numSquaresWithoutOutliers = totalSquares - 3;
      else
        numSquaresWithoutOutliers = totalSquares - 2;
    }

    keys.clear();
    sortSemanticValues();
    computeMaxLabelSize();
  }

  imageWidth = totalSquares * widthRect + 2 * outlierMargin + 2 * titleMargin;
}


// Drawing's made in two rows and for every row from left to right
// First row is for colors, second row is for labels
void gTimeline::ScaleImageHorizontalGradientColor::draw()
{
  // *** 1. Draw color scale ****************************************************************
  
  int initialXdst = xdst;
  
  // Bottom Outlier
  rgb tmprgb = myWindow->getGradientColor().getBelowOutlierColor();
  drawRectangle( tmprgb );
  xdst += imageStepXRectangle + outlierMargin;
  
  // Colors
  int elem = 0;
  int MIDDLE_POS = rint( numSquaresWithoutOutliers / 2 ) + 1; // 11 
  for ( std::vector< TSemanticValue >::iterator it = keys.begin(); it != keys.end(); ++it )
  {
    ++elem;
    if ( it == keys.begin() )
      drawRectangle( semValues[ *it ], FIRST );
    else if ( it == --keys.end() )
      drawRectangle( semValues[ *it ], LAST  );
    else if ( elem == MIDDLE_POS )
      drawRectangle( semValues[ *it ], MIDDLE );
    else
      drawRectangle( semValues[ *it ], ANY );

    xdst += imageStepXRectangle;
  }

  // Top outlier
  xdst += outlierMargin;
  tmprgb = myWindow->getGradientColor().getAboveOutlierColor();
  drawRectangle( tmprgb );
  
  // *** 2. Draw labels **********************************************************************
  
  xdst = initialXdst;
  ydst += imageStepY - titleMargin + 2;
  
  // Bottom Outlier
  wxString tmpSemanticValueLabel = wxT( "<" );
  drawLabel( tmpSemanticValueLabel );
  xdst += imageStepXRectangle + outlierMargin;
  
  // Labels
  bool drawIt;
  elem = 0;
  for ( std::vector< TSemanticValue >::iterator it = keys.begin(); it != keys.end(); ++it )
  {
    ++elem;
    drawIt = it == keys.begin() || elem == MIDDLE_POS || it == --keys.end();

    if ( it == keys.begin() )
      drawLabel( semanticValueLabel[ *it ], drawIt, LEFT );
    else if ( it == --keys.end() )
    {
      drawLabel( semanticValueLabel[ *it ], drawIt, RIGHT );
    }
    else if ( elem == MIDDLE_POS )
      drawLabel( semanticValueLabel[ *it ], drawIt, CENTER );
    else
      drawLabel( semanticValueLabel[ *it ], drawIt, LEFT );

    xdst += imageStepXRectangle;
  }

  // Top outlier
  xdst += outlierMargin;
  tmpSemanticValueLabel = wxT( ">" );
  drawIt = true;
  drawLabel( tmpSemanticValueLabel, drawIt, RIGHT );
}


void gTimeline::ScaleImageHorizontalGradientColor::drawRectangle( rgb semanticColour, TPosition position )
{
  scaleDC->SetBrush( wxColour( semanticColour.red,
                               semanticColour.green,
                               semanticColour.blue ) );
  scaleDC->DrawRectangle( xdst, ydst, widthRect, heightRect );

  scaleMaskDC->DrawRectangle( xdst, ydst, widthRect, heightRect );
  
  switch( position )
  {
    case FIRST:
      scaleDC->DrawLine( xdst, ydst + heightRect,
                         xdst, ydst + heightRect + SIZE_OF_TINY_MARK );
      scaleMaskDC->DrawLine( xdst, ydst + heightRect,
                             xdst, ydst + heightRect + SIZE_OF_TINY_MARK );
      break;
    case MIDDLE:
      scaleDC->DrawLine( xdst + ceil( (double)widthRect / 2 ), ydst + heightRect,
                         xdst + ceil( (double)widthRect / 2 ), ydst + heightRect + SIZE_OF_TINY_MARK );
      scaleMaskDC->DrawLine( xdst + ceil( (double)widthRect / 2 ), ydst + heightRect,
                             xdst + ceil( (double)widthRect / 2 ), ydst + heightRect + SIZE_OF_TINY_MARK );
      break;
    case LAST:
      scaleDC->DrawLine( xdst + widthRect - 1, ydst + heightRect,
                         xdst + widthRect - 1, ydst + heightRect + SIZE_OF_TINY_MARK );
      scaleMaskDC->DrawLine( xdst + widthRect - 1, ydst + heightRect,
                             xdst + widthRect - 1, ydst + heightRect + SIZE_OF_TINY_MARK );
      break;
    default:
      break;
  }  
}


void gTimeline::ScaleImageHorizontalGradientColor::drawLabel( wxString semanticValueLabel, bool writeIt, TAlign align )
{
  if ( writeIt )
  {
    wxSize labelSize = scaleDC->GetTextExtent( semanticValueLabel );

    int shiftLeft;
    switch( align )
    {
      case LEFT:
        shiftLeft = 0;
        break;
      case CENTER:
        shiftLeft = ceil( (double)labelSize.GetWidth() / 2 - (double)widthRect / 2 );
        break;
      case RIGHT:
        shiftLeft = labelSize.GetWidth() - widthRect;
        break;
      default:
        break;
    }

    int tmpXdst = xdst;
    if (( xdst - shiftLeft ) >= 0 )
    {
      tmpXdst = xdst - shiftLeft;
    }

    // Write text
    scaleDC->DrawText( semanticValueLabel, tmpXdst, ydst);
    scaleMaskDC->DrawText( semanticValueLabel, tmpXdst, ydst );
  }
}


//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void gTimeline::saveText()
{
  wxString fileName;
  wxString tmpSuffix;
  wxString defaultDir;

  fileName = buildFormattedFileName();

#ifdef WIN32
  defaultDir = _(".\\");
#else
  defaultDir = _("./");
#endif

  tmpSuffix = _(".") +
          wxString::FromAscii( LabelConstructor::getDataFileSuffix(
                  ParaverConfig::getInstance()->getTimelineSaveTextFormat() ).c_str() );

  // Builds following wildcard: _( "CSV (*.csv)|*.csv|GNUPlot (*.gnuplot)|*.gnuplot" )
  // Also fills extension
  wxString tmpWildcard;
  vector< wxString > extensions;
  for ( PRV_UINT16 i = 0; i < PRV_UINT16( ParaverConfig::PLAIN ); ++i )
  {
    wxString currentFormat =
          wxString::FromAscii( LabelConstructor::getDataFileSuffix(
                  ParaverConfig::TTextFormat( i ) ).c_str() );
    tmpWildcard +=
            currentFormat.Upper() + _(" (*.") + currentFormat + _(")|*.") + currentFormat + _("|");

    extensions.push_back( currentFormat );
  }
  tmpWildcard = tmpWildcard.BeforeLast( '|' );

  FileDialogExtension saveDialog( this,
                                  _("Save as..."),
                                  defaultDir,
                                  fileName + tmpSuffix,
                                  tmpWildcard,
                                  wxFD_SAVE | wxFD_CHANGE_DIR,
                                  wxDefaultPosition,
                                  wxDefaultSize,
                                  _( "filedlg" ),
                                  extensions );
                           
  saveDialog.SetFilterIndex( ParaverConfig::getInstance()->getTimelineSaveTextFormat() );
  
  if ( saveDialog.ShowModal() == wxID_OK )
  {
    // Set up progress controller
    ProgressController *progress = ProgressController::create( paraverMain::myParaverMain->GetLocalKernel() );
    progress->setHandler( progressFunction, this );

    if( paraverMain::dialogProgress == NULL )
      paraverMain::dialogProgress = new wxProgressDialog( wxT("Save Timeline Text"),
                                                          wxT(""),
                                                          numeric_limits<int>::max(),
                                                          this,
                                                          wxPD_CAN_ABORT|wxPD_AUTO_HIDE|\
                                                          wxPD_APP_MODAL|wxPD_ELAPSED_TIME|\
                                                          wxPD_ESTIMATED_TIME|wxPD_REMAINING_TIME );

    string fileName = string( saveDialog.GetPath().mb_str() );
    string reducePath;

    if( fileName.length() > 36 && fileName.find_last_of( PATH_SEP ) != string::npos )
    {
      string file = fileName.substr( fileName.find_last_of( PATH_SEP ) );
      string tmp = fileName.substr( 0, fileName.find_last_of( PATH_SEP ) );
      if ( tmp.find_last_of( PATH_SEP ) != string::npos )
      {
        reducePath = "/..." + fileName.substr( tmp.find_last_of( PATH_SEP ),
                                                tmp.length() - tmp.find_last_of( PATH_SEP ) )
                     + file;
      }
      else
      {
        reducePath = "/..." + file;
      }
    }
    else
      reducePath = fileName;
    reducePath += "\t";
    
    paraverMain::dialogProgress->Pulse( wxString::FromAscii( reducePath.c_str() ) );
    paraverMain::dialogProgress->Fit();
    paraverMain::dialogProgress->Show();
  
    // Save timeline text
    Output *output = Output::createOutput( (Output::TOutput)saveDialog.GetFilterIndex() );
    output->setMultipleFiles( false );
    
    // Clustering default options
    output->setObjectHierarchy( true );
    output->setWindowTimeUnits( false );
    
    
    output->dumpWindow( myWindow, fileName, progress );
    delete output;
    
    // Delete progress controller
    paraverMain::dialogProgress->Show( false );
    delete paraverMain::dialogProgress;
    paraverMain::dialogProgress = NULL;
    delete progress;
  }
}


void gTimeline::saveCFG()
{
  vector< Window * > timelines;
  timelines.push_back( GetMyWindow() );

  paraverMain::myParaverMain->SaveConfigurationFile(
          (wxWindow *)this, SaveOptions(), timelines, vector< Histogram * >() );
}


void gTimeline::OnTimerSize( wxTimerEvent& event )
{
  if( myWindow->getReady() )
  {
#ifdef WIN32
    redraw();
#else
    if( splitChanged )
      redraw();
    else
      myWindow->setRedraw( true );
#endif
  }
  Refresh();
}


void gTimeline::OnTimerMotion( wxTimerEvent& event )
{
  if( motionEvent.GetX() < objectAxisPos + 1 || motionEvent.GetX() > bufferImage.GetWidth() - drawBorder ||
      motionEvent.GetY() < drawBorder || motionEvent.GetY() > timeAxisPos - 1 )
    return;

  wxMemoryDC dc( bufferImage );
  wxColour tmpColor;

  wxString label;
  if( zooming || timing || wxGetApp().GetGlobalTiming() )
  {
    long beginX;
    PRV_UINT32 precision = 0;
    TTime timeStep = ( myWindow->getWindowEndTime() - myWindow->getWindowBeginTime() ) /
                     ( dc.GetSize().GetWidth() - objectAxisPos - drawBorder );;
    TTime time;

    if( zooming )
    {
      beginX = firstMotionEvent.GetX();
      if( beginX < objectAxisPos )
        beginX = 0;
      else
        beginX -= objectAxisPos;
      time = ( timeStep * beginX ) + myWindow->getWindowBeginTime();
      label = wxString::FromAscii( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( time ), myWindow->getTimeUnit(), precision ).c_str() );
      label += wxT( " - " );
    }
    beginX = motionEvent.GetX();
    beginX -= objectAxisPos;
    time = ( timeStep * beginX ) + myWindow->getWindowBeginTime();
    label += wxString::FromAscii( LabelConstructor::timeLabel( myWindow->traceUnitsToWindowUnits( time ), myWindow->getTimeUnit(), precision ).c_str() );
  }
  else
  {
    if( myWindow->isFunctionLineColorSet() )
      return;

#ifdef __WXMAC__
    wxImage tmpImage = bufferImage.ConvertToImage();
    tmpColor = wxColour( tmpImage.GetRed( motionEvent.GetX(), motionEvent.GetY() ),
                         tmpImage.GetGreen( motionEvent.GetX(), motionEvent.GetY() ),
                         tmpImage.GetBlue( motionEvent.GetX(), motionEvent.GetY() ) );
/*
    wxBitmap tmpBmp = bufferImage.GetSubBitmap( wxRect(0, 0, bufferImage.GetWidth(), bufferImage.GetHeight()));
    wxAlphaPixelData tmpPixelData( tmpBmp );
    wxAlphaPixelData::Iterator itImage( tmpPixelData );
    itImage.Offset( tmpPixelData, motionEvent.GetX(), motionEvent.GetY() );
    tmpColor = wxColour( itImage.Red(), itImage.Green(), itImage.Blue() );*/
#else  
    dc.GetPixel( motionEvent.GetX(), motionEvent.GetY(), &tmpColor );
#endif

    if( tmpColor == backgroundColour )
      return;

    rgb color = { (ParaverColor)tmpColor.Red(), (ParaverColor)tmpColor.Green(), (ParaverColor)tmpColor.Blue() };
    TSemanticValue firstValue, secondValue;
    Window *winToUse = myWindow;
    if( myWindow->isPunctualColorSet() && myWindow->getPunctualColorWindow() != NULL )
      winToUse = myWindow->getPunctualColorWindow();

    if( winToUse->isCodeColorSet() )
    {
      string tmpString;
      firstValue = *( semanticColorsToValue[ color ].begin() );
      tmpString = LabelConstructor::semanticLabel( winToUse, firstValue, true, ParaverConfig::getInstance()->getTimelinePrecision() );
      if( winToUse->getSemanticInfoType() == EVENTVALUE_TYPE )
        LabelConstructor::transformToShort( tmpString );
      label = wxString::FromAscii( tmpString.c_str() );
    }
    else if( !winToUse->calcValueFromColor( color, firstValue, secondValue ) )
    {
      if( !winToUse->isCodeColorSet() )
      {
        //GradientColor& grad = myWindow->getGradientColor();
        if( color == winToUse->getGradientColor().getAboveOutlierColor() )
          label = wxT( "> " ) + wxString::FromAscii( LabelConstructor::semanticLabel( winToUse, winToUse->getMaximumY(), false,
                                                                                      ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() );
        else if( color == winToUse->getGradientColor().getBelowOutlierColor() )
          label = wxT( "< " ) + wxString::FromAscii( LabelConstructor::semanticLabel( winToUse, winToUse->getMinimumY(), false,
                                                                                      ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() );
        else
          return;
      }
      else
        return;
    }
    else
    {
      if( winToUse->isFusedLinesColorSet() )
      {
        string tmpString;
        tmpString = LabelConstructor::objectLabel( (TObjectOrder)firstValue - 1, winToUse->getLevel(), winToUse->getTrace() );
        label = wxString::FromAscii( tmpString.c_str() );
      }
      else
      {
        // Gradient
        firstValue = *( semanticColorsToValue[ color ].begin() );
        label = wxString::FromAscii( LabelConstructor::semanticLabel( winToUse, firstValue, false,
                                                                      ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() );
        if( semanticColorsToValue[ color ].size() > 1 )
        {
          secondValue = *( --( semanticColorsToValue[ color ].end() ) );
          label += wxT( " - " ) + wxString::FromAscii( LabelConstructor::semanticLabel( winToUse, secondValue, false,
                                                                                        ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() );
        }
      }
    }
  }

#ifndef __WXGTK__
  wxClientDC paintDC( drawZone );
  #ifdef __WXMAC__
  drawStackedImages( paintDC );
  #else
  paintDC.DrawBitmap( drawImage, 0, 0 );
  #endif
#else
  #if wxMAJOR_VERSION<3
  wxPaintDC paintDC( drawZone );
  #else
  wxClientDC paintDC( drawZone );
  #endif
  paintDC.DrawBitmap( drawImage, 0, 0 );
#endif

  paintDC.SetFont( semanticFont );
  wxSize objectExt = paintDC.GetTextExtent( label );

  paintDC.SetPen( backgroundColour );
  paintDC.SetBrush( backgroundColour );
//  paintDC.DrawRectangle( ( bufferImage.GetWidth() - objectAxisPos ) / 2, timeAxisPos + 1, objectExt.GetWidth() + 30, bufferImage.GetHeight() - timeAxisPos );
  if( !( zooming || timing || wxGetApp().GetGlobalTiming() ) )
  {
    paintDC.SetBrush( tmpColor );
    paintDC.DrawRectangle( ( bufferImage.GetWidth() - objectAxisPos ) / 2, timeAxisPos + 2, 10, bufferImage.GetHeight() - timeAxisPos - 3 );
  }
  paintDC.SetTextForeground( foregroundColour );
  if( zooming )
    paintDC.DrawText( label, ( bufferImage.GetWidth() - objectAxisPos ) / 2 + objectAxisPos - ( objectExt.GetWidth() / 2 ), timeAxisPos + 3 );
  else
    paintDC.DrawText( label, ( bufferImage.GetWidth() - objectAxisPos ) / 2 + 12, timeAxisPos + 3 );
}

void gTimeline::OnTimerWheel( wxTimerEvent& event )
{
#ifdef WIN32
  wheelZoomObjects = false;
#endif
  wheelZoomFactor = 1.0;
  myWindow->addZoom( wheelZoomBeginTime, wheelZoomEndTime, wheelZoomBeginObject, wheelZoomEndObject );
  myWindow->setWindowBeginTime( wheelZoomBeginTime, true );
  myWindow->setWindowEndTime( wheelZoomEndTime, true );
  myWindow->setRedraw( true );
  myWindow->setChanged( true );
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
  checkWWShowDate->Enable( false );

  computeWhatWhere( whatWhereTime, whatWhereRow, whatWhereSemantic,
                    checkWWText->IsChecked(),
                    checkWWShowDate->IsChecked() );
  printWhatWhere();

  checkWWSemantic->Enable( true );
  checkWWEvents->Enable( true );
  checkWWCommunications->Enable( true );
  checkWWPreviousNext->Enable( true );
  checkWWText->Enable( true );
  checkWWShowDate->Enable( true );
}

void gTimeline::OnCheckWhatWhere( wxCommandEvent& event )
{
  checkWWSemantic->Enable( false );
  checkWWEvents->Enable( false );
  checkWWCommunications->Enable( false );
  checkWWPreviousNext->Enable( false );
  checkWWText->Enable( false );
  checkWWShowDate->Enable( false );

  printWhatWhere();

  checkWWSemantic->Enable( true );
  checkWWEvents->Enable( true );
  checkWWCommunications->Enable( true );
  checkWWPreviousNext->Enable( true );
  checkWWText->Enable( true );
  checkWWShowDate->Enable( true );
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
  wxString command;

  wxString alienApp;
  wxString alienType;
  if ( wxGetEnv( wxString( wxT( "PARAVER_ALIEN_APP" ) ), &alienApp ) &&
       wxGetEnv( wxString( wxT( "PARAVER_ALIEN_TYPE" ) ), &alienType ) )
  {
    long tmpType;
    if (!alienType.ToLong(&tmpType))
    {
      wxMessageBox( _( "Unable to interpret $PARAVER_ALIEN_TYPE as an event type\n\nValue: " ) + alienType,
                    _( "Application invocation" ) );
      return;
    }
    else
    {
      events.push_back( TEventType( tmpType ) );
      if( myWindow->getTrace()->findLastEventValue( posRow, posTime, events, type, value ) )
      {
        string valueStr = LabelConstructor::eventValueLabel( myWindow, type, value );
        if ( valueStr.empty() )
        {
          command << alienApp <<  _(" ") << value;
        }
        else
        {
          command << alienApp <<  _(" ") << wxString::FromAscii( valueStr.c_str() );
        }
        
        //wxString currentDir = wxGetCwd();
        //wxString myTraceDir =
        //        wxFileName ( wxString::FromAscii( GetMyWindow()->getTrace()->getFileName().c_str() ) ).GetPath();
        //wxSetWorkingDirectory( myTraceDir );

        std::string absolutePathCurrentTrace = GetMyWindow()->getTrace()->getFileName();
#ifdef WIN32
        _putenv_s( "PARAVER_ALIEN_TRACE_FULL_PATH", (const char *)absolutePathCurrentTrace.c_str() );
        wxExecute( command );
#else
        setenv( "PARAVER_ALIEN_TRACE_FULL_PATH", (const char *)absolutePathCurrentTrace.c_str(), 1 );
        if( wxExecute( command ) == 0 )
        {
        }
#endif
        //wxSetWorkingDirectory( currentDir );
      }
      else
      {
        //wxMessageBox( _( "Unable to find $PARAVER_ALIEN_TYPE as an event type" ), _( "Alien application invocation" ) );
      }
    }
  }
  else
  {
    events.push_back( wxGetApp().GetEventTypeForCode() );
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
        wxString command;
#ifdef WIN32
        command << _( "wordpad.exe " ) /*<< " +" << lineStr << " "*/ << path << _( "\\" ) << wxString::FromAscii( fileStr.c_str() );
        wxExecute( command );
#else
        // As before
        command << _( "gvim " ) << _( " +" ) << wxString::FromAscii( lineStr.c_str() ) << _( " " ) << path << _( "/" ) << wxString::FromAscii( fileStr.c_str() );
        if( wxExecute( command ) == 0 )
        {
          command.Clear();
          command << _( "nedit " ) << _( " +" ) << wxString::FromAscii( lineStr.c_str() ) << _( " " ) << path << _( "/" ) << wxString::FromAscii( fileStr.c_str() );
          if( wxExecute( command ) == 0 )
            wxMessageBox( _( "Install gvim or nedit for view source code files." ), _( "Show source code" ) );
        }
#endif
      }
    }
    else
    {
      wxMessageBox( _( "Event " ) + (wxString() << wxGetApp().GetEventTypeForCode()) + _( " not found." ), _( "Show source code" ) );
    }
  }
}


/*!
 * wxEVT_KEY_DOWN event handler for ID_SCROLLEDWINDOW
 */

void gTimeline::OnScrolledWindowKeyDown( wxKeyEvent& event )
{
  if( event.ControlDown() && event.GetKeyCode() == (long) 'C' )
  {
    OnPopUpCopy();
    return;
  }
  if( event.ControlDown() && event.GetKeyCode() == (long) 'V' )
  {
    if( gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( this, STR_PASTE_SPECIAL ) )
      OnPopUpPasteDefaultSpecial();
    return;
  }
  if( event.ControlDown() && event.GetKeyCode() == (long) 'U' )
  {
    OnPopUpUndoZoom();
    return;
  }
  if( event.ControlDown() && event.GetKeyCode() == (long) 'R' )
  {
    OnPopUpRedoZoom();
    return;
  }
  if( event.ControlDown() && event.GetKeyCode() == (long) 'T' )
  {
    OnPopUpTiming( !timing );
    return;
  }

  if( zooming && event.GetKeyCode() == WXK_ESCAPE )
  {
    escapePressed = true;
    zooming = false;

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
    dc.DrawBitmap( bufferImage, 0, 0, false );
    if( myWindow->getDrawFlags() )
      dc.DrawBitmap( eventImage, 0, 0, true );
    if( myWindow->getDrawCommLines() )
      dc.DrawBitmap( commImage, 0, 0, true );
    drawZone->Refresh();
  }

  event.Skip();
}

bool gTimeline::pixelToTimeObject( long x, long y, TTime& onTime, TObjectOrder& onObject )
{
  if( x < objectAxisPos ||
      x > bufferImage.GetWidth() - drawBorder )
    return false;
  else
    x -= objectAxisPos;

  if( y > timeAxisPos ||
      y < drawBorder )
    return false;

  TTime timeStep = ( myWindow->getWindowEndTime() - myWindow->getWindowBeginTime() ) /
                   ( bufferImage.GetWidth() - objectAxisPos - drawBorder );
  onTime = ( timeStep * x ) + myWindow->getWindowBeginTime();

  TObjectOrder beginRow = myWindow->getZoomSecondDimension().first;
  TObjectOrder endRow = myWindow->getZoomSecondDimension().second;
  vector<TObjectOrder> selected;
  myWindow->getSelectedRows( myWindow->getLevel(), selected, beginRow, endRow, true );
  TObjectOrder numObjects = selected.size();
  double heightPerRow = (double)( timeAxisPos - drawBorder - 1 ) / (double)numObjects;
  endRow = TObjectOrder( floor( ( y - drawBorder - 1 ) / heightPerRow ) );

  if( endRow >= numObjects )
    endRow = numObjects - 1;
  onObject = selected[ endRow ];

  return true;
}


/*!
 * wxEVT_LEFT_DCLICK event handler for ID_SCROLLEDWINDOW
 */

void gTimeline::OnScrolledWindowLeftDClick( wxMouseEvent& event )
{
  TObjectOrder object;
  TTime time;

  if( !pixelToTimeObject( event.GetX(), event.GetY(), time, object ) )
    return;

  TSemanticValue tmpSemantic = 0.0;
  
  if( myWindow->isFusedLinesColorSet() )
  {
    TSemanticValue semanticStep = ( myWindow->getMaximumY() - myWindow->getMinimumY() ) /
                     ( timeAxisPos - drawBorder );
    tmpSemantic = myWindow->getMaximumY() - ( semanticStep * ( event.GetY() - drawBorder - 1 ) );//( semanticStep * y ) + myWindow->getMinimumY();
  }

  if( !splitter->IsSplit() )
  {
    Split();
  }
  whatWhereText->Clear();
  whatWhereText->AppendText( _( "Working..." ) );
  Update();
  computeWhatWhere( time, object, tmpSemantic, checkWWText->IsChecked(), checkWWShowDate->IsChecked() );
  printWhatWhere();
}


void gTimeline::drawTimeMarks( std::vector< TRecordTime > times,
                               vector< TObjectOrder > &selectedObjects,
                               bool drawXCross,
                               bool allObjects,
                               TObjectOrder lastFoundObject )
{
  wxMemoryDC bufferDraw;

  if( !myWindow->getReady() )
    return;
    
  if ( allObjects )
  {
    myWindow->getSelectedRows( myWindow->getLevel(), selectedObjects, true );
    lastFoundObject = selectedObjects.size() -1;
  }

  bufferDraw.SelectObject( wxNullBitmap );
  bufferDraw.SelectObject( drawImage );
  bufferDraw.DrawBitmap( bufferImage, 0, 0, false );

  doDrawCaution( bufferDraw );

  if( myWindow->getDrawFlags() )
    bufferDraw.DrawBitmap( eventImage, 0, 0, true );

  if( myWindow->getDrawCommLines() )
    bufferDraw.DrawBitmap( commImage, 0, 0, true );

  for( std::vector< TRecordTime >::iterator it = times.begin(); it != times.end(); ++it )
  {
    if ( *it >= myWindow->getWindowBeginTime() && *it <= myWindow->getWindowEndTime() )
    {

      wxCoord xTime = ( ( ( *it - myWindow->getWindowBeginTime() ) * ( drawZone->GetSize().GetWidth() - objectAxisPos - drawBorder ) )
                        / ( myWindow->getWindowEndTime() - myWindow->getWindowBeginTime() ) )
                      + objectAxisPos;

      if ( drawXCross )
      {
        // draw found object cross
        bufferDraw.SetPen( wxPen( *wxRED, 2 ) );
        bufferDraw.DrawLine( xTime - 5, objectPosList[ lastFoundObject ] - 5, xTime + 5, objectPosList[ lastFoundObject ] + 5 );
        bufferDraw.DrawLine( xTime - 5, objectPosList[ lastFoundObject ] + 5, xTime + 5, objectPosList[ lastFoundObject ] - 5 );
      }
      
      // draw found time line
      bufferDraw.SetPen( wxPen( *wxRED, 2, wxSHORT_DASH ) );
      bufferDraw.DrawLine( xTime + 1, 0, xTime + 1, drawZone->GetSize().GetHeight() );
    }
  }
  
  drawZone->Refresh();
}


void gTimeline::OnFindDialog()
{
  FindDialog dialog( this );
  dialog.SetMyWindow( myWindow );
  dialog.InitControlsBeforeShow();
  
  if( dialog.ShowModal() == wxID_OK )
  {
    TRecordTime beginTime, endTime;
    unsigned int objectSelection = dialog.choiceObjects->GetSelection();
    vector<TObjectOrder> selectedObjects;
    myWindow->getSelectedRows( myWindow->getLevel(), selectedObjects, true );

    if( dialog.radioObjects->GetValue() )
    {
      beginTime = myWindow->getWindowBeginTime();
      endTime = myWindow->getWindowEndTime();
    }
    else if( dialog.radioEvents->GetValue() )
    {
      bool found = false;
      TRecordTime newFoundTime, timeToSearch;
      set<TEventType> events = myWindow->getTrace()->getLoadedEvents();
      set<TEventType>::iterator itEvt = events.begin();
      int i = 0;
      while( i < dialog.choiceEventType->GetSelection() )
      {
        ++i; ++itEvt;
      }
      TEventType eventType = *itEvt;
      if( lastEventFoundTime >= myWindow->getWindowBeginTime() &&
          lastEventFoundTime <= myWindow->getWindowEndTime() )
        timeToSearch = lastEventFoundTime;
      else
        timeToSearch = myWindow->getWindowBeginTime();
      
      if( dialog.checkNextObject->GetValue() )
      {
        while( !found && objectSelection < selectedObjects.size() )
        {
          if( ( found = myWindow->getTrace()->findNextEvent( selectedObjects[ objectSelection ],
                                                             timeToSearch,
                                                             eventType,
                                                             newFoundTime ) ) )
            break;
          ++objectSelection;
          timeToSearch = 0.0;
        }
      }
      else
      {
        found = myWindow->getTrace()->findNextEvent( selectedObjects[ objectSelection ],
                                                     timeToSearch,
                                                     eventType,
                                                     newFoundTime );
      }

      if( !found )
      {
        wxMessageBox( wxT( "Event '" ) + dialog.choiceEventType->GetStringSelection() + wxT( "' not found." ),
                      wxT( "Not found" ) );
        return;
      }

      beginTime = newFoundTime - myWindow->getTrace()->getEndTime() * 0.001;
      if( beginTime < 0.0 ) beginTime = 0.0;
      endTime = newFoundTime + myWindow->getTrace()->getEndTime() * 0.001;
      
      lastEventFoundTime = newFoundTime;
    }
    else if( dialog.radioSemantic->GetValue() )
    {
      bool found = false;
      TRecordTime timeToSearch, duration;
      TSemanticValue valueToSearch;

      if( lastSemanticFoundTime >= myWindow->getWindowBeginTime() &&
          lastSemanticFoundTime <= myWindow->getWindowEndTime() )
        timeToSearch = lastSemanticFoundTime;
      else
        timeToSearch = myWindow->getWindowBeginTime();

      if( dialog.comboSemanticValue->GetSelection() == wxNOT_FOUND )
      {
        wxString tmpStr = dialog.comboSemanticValue->GetValue();
        tmpStr.ToDouble( &valueToSearch );
      }
      else
        valueToSearch = dialog.comboSemanticValue->GetSelection() + myWindow->getMinimumY();

      duration = dialog.spinSemanticDuration->GetValue();
      
      myWindow->init( timeToSearch, NOCREATE, false );
      myWindow->initRow( selectedObjects[ objectSelection ], timeToSearch, NOCREATE, false );
      while( lastSemanticFoundTime == myWindow->getBeginTime( selectedObjects[ objectSelection ] ) )
        myWindow->calcNext( selectedObjects[ objectSelection ] );

      if( dialog.checkNextObject->GetValue() )
      {
        while( !found && objectSelection < selectedObjects.size() )
        {
          while( !found && myWindow->getBeginTime( selectedObjects[ objectSelection ] ) < myWindow->getWindowEndTime() )
          {
            if( myWindow->getValue( selectedObjects[ objectSelection ] ) == valueToSearch )
            {
              if( dialog.choiceDurationFunction->GetSelection() == 0 &&
                  myWindow->getEndTime( selectedObjects[ objectSelection ] ) - myWindow->getBeginTime( selectedObjects[ objectSelection ] )
                  > duration )
              {
                found = true;
                break;
              }
              else if( dialog.choiceDurationFunction->GetSelection() == 1 &&
                       myWindow->getEndTime( selectedObjects[ objectSelection ] ) - myWindow->getBeginTime( selectedObjects[ objectSelection ] )
                       < duration )
              {
                found = true;
                break;
              }
            }
            myWindow->calcNext( selectedObjects[ objectSelection ] );
          }
          ++objectSelection;
        }
      }
      else
      {
        while( !found && myWindow->getBeginTime( selectedObjects[ objectSelection ] ) < myWindow->getWindowEndTime() )
        {
          if( myWindow->getValue( selectedObjects[ objectSelection ] ) == valueToSearch )
          {
            if( dialog.choiceDurationFunction->GetSelection() == 0 &&
                myWindow->getEndTime( selectedObjects[ objectSelection ] ) - myWindow->getBeginTime( selectedObjects[ objectSelection ] )
                > duration )
            {
              found = true;
              break;
            }
            else if( dialog.choiceDurationFunction->GetSelection() == 1 &&
                     myWindow->getEndTime( selectedObjects[ objectSelection ] ) - myWindow->getBeginTime( selectedObjects[ objectSelection ] )
                     < duration )
            {
              found = true;
              break;
            }
          }
          myWindow->calcNext( selectedObjects[ objectSelection ] );
        }
      }

      if( !found )
      {
        wxMessageBox( wxT( "Semantic '" ) + dialog.comboSemanticValue->GetValue() + wxT( "' not found." ),
                      wxT( "Not found" ) );
        return;
      }

      TRecordTime addTime = ( myWindow->getEndTime( selectedObjects[ objectSelection ] ) 
                              - myWindow->getBeginTime( selectedObjects[ objectSelection ] ) )
                            * 0.1;
      if( myWindow->getBeginTime( selectedObjects[ objectSelection ] ) - myWindow->getEndTime( selectedObjects[ objectSelection ] )
          <
          ( myWindow->getWindowEndTime() - myWindow->getWindowBeginTime() ) * 0.25 )
      {
        beginTime = myWindow->getBeginTime( selectedObjects[ objectSelection ] ) - addTime;
        if( beginTime < 0.0 ) beginTime = 0.0;
        endTime = myWindow->getEndTime( selectedObjects[ objectSelection ] ) + addTime;
      }
      else
      {
        beginTime = myWindow->getBeginTime( selectedObjects[ objectSelection ] ) - addTime;
        if( beginTime < 0.0 ) beginTime = 0.0;
        endTime = beginTime + ( ( myWindow->getWindowEndTime() - myWindow->getWindowBeginTime() ) * 0.25 );
      }

      lastSemanticFoundTime = myWindow->getBeginTime( selectedObjects[ objectSelection ] );
    }
    
    TObjectOrder first, last;
    unsigned int objectsToShow = floor( (double)( timeAxisPos - drawBorder ) / 20 );

    if( objectsToShow >= selectedObjects.size() )
    {
      first = selectedObjects[ 0 ];
      last = selectedObjects[ selectedObjects.size() - 1 ];
    }
    else if( dialog.choicePosition->GetSelection() == 0 ) // position begin
    {
      first = selectedObjects[ objectSelection ];
      if( objectSelection + objectsToShow >= selectedObjects.size() )
        last = selectedObjects[ selectedObjects.size() - 1 ];
      else
        last = selectedObjects[ objectSelection + objectsToShow ];
    }
    else if( dialog.choicePosition->GetSelection() == 2 ) // position end
    {
      last = selectedObjects[ objectSelection ];
      if( objectsToShow > objectSelection )
        first = selectedObjects[ 0 ];
      else
        first = selectedObjects[ objectsToShow - objectSelection ];
    }
    else // position middle
    { 
      objectsToShow /= 2;
      if( objectsToShow > objectSelection )
      {
        first = selectedObjects[ 0 ];
        last = selectedObjects[ objectSelection + objectsToShow ];
      }
      else if( objectSelection + objectsToShow > selectedObjects.size() )
      {
        first = selectedObjects[ objectSelection - objectsToShow ];
        last = selectedObjects[ selectedObjects.size() ];
      }
      else
      {
        first = selectedObjects[ objectSelection - objectsToShow ];
        last = selectedObjects[ objectSelection + objectsToShow ];
      }
    }


    findBeginTime   = beginTime;
    findEndTime     = endTime;
    findFirstObject = first;
    findLastObject  = last;
    lastFoundObject = objectSelection;

    std::vector< TRecordTime > tmpTimes;
    tmpTimes.push_back( lastSemanticFoundTime );
    bool drawXCross = true;
    bool allObjects = false;
    drawTimeMarks( tmpTimes, selectedObjects, drawXCross, allObjects, objectSelection );
  }
}


void progressFunctionTimeline( ProgressController *progress, void *callerWindow )
{
  if( gTimeline::dialogProgress != NULL )
  {
    gTimeline::dialogProgress->Refresh();
    gTimeline::dialogProgress->Update();
  }

  int p;
  if ( progress->getCurrentProgress() > progress->getEndLimit() )
    p = numeric_limits<int>::max();
  else 
    p = (int)floor( ( progress->getCurrentProgress() * numeric_limits<int>::max() ) / progress->getEndLimit() );

  wxString newMessage;
  if( progress->getMessageChanged() )
  {
    newMessage = wxString::FromAscii( progress->getMessage().c_str() );
    progress->clearMessageChanged();
  }

// Disabled because some window managers can't show the dialog later
/*  if( ( (gTimeline*)callerWindow )->GetRedrawStopWatch()->Time() >= 750 )
  {
    if( gTimeline::dialogProgress != NULL && !gTimeline::dialogProgress->IsShown() )
    {
      gTimeline::dialogProgress->Show();
      gTimeline::dialogProgress->Raise();
    }

    ( (gTimeline*)callerWindow )->GetRedrawStopWatch()->Pause();
  }
*/
  
  if( gTimeline::dialogProgress != NULL && !gTimeline::dialogProgress->Update( p, newMessage ) )
    progress->setStop( true );
}


/*!
 * wxEVT_UPDATE_UI event handler for wxID_STATIC_SLOPE
 */

void gTimeline::OnStaticSlopeUpdate( wxUpdateUIEvent& event )
{
  event.SetText( wxString( _( "Slope (by ")  ) + wxString( LABEL_TIMEUNIT[ myWindow->getTimeUnit() ].c_str(), wxConvUTF8 ) + wxString( _( ")" ) ) );
}


/*!
 * wxEVT_MOUSEWHEEL event handler for ID_SCROLLED_DRAW
 */

void gTimeline::OnScrolledWindowMouseWheel( wxMouseEvent& event )
{
  double newWheelFactor = wheelZoomFactor;
  TRecordTime newWheelZoomBeginTime;
  TRecordTime newWheelZoomEndTime;
  TObjectOrder newWheelZoomBeginObject;
  TObjectOrder newWheelZoomEndObject;
  bool zoomOut = event.GetWheelRotation() < 0;
  bool zoomIn = !zoomOut;

  if( !myWindow->getReady() )
    return;

  if( zoomOut )
  {
  //Uncomment if want more zoom out (image get really tiny)
/*    if( newWheelFactor <= 0.2 )
      newWheelFactor -= 0.01;
    else*/
      newWheelFactor -= 0.1;
    if( newWheelFactor <= std::numeric_limits<double>::epsilon() )
      return;
  }
  else
  {
    newWheelFactor += 0.1;
  }

  // Trace time boundary check
  TRecordTime timeWidth = myWindow->getWindowEndTime() - myWindow->getWindowBeginTime();
  if( zoomIn && timeWidth <= 10 )
    return;
  if( zoomOut && myWindow->getWindowBeginTime() == 0 && myWindow->getWindowEndTime() == myWindow->getTrace()->getEndTime() )
    return;

#ifdef WIN32
  if( event.ControlDown() )
    wheelZoomObjects = true;
#endif

  double wheelZoomFactorX = newWheelFactor;
  double wheelZoomFactorY = 1;
#ifdef WIN32
  if( wheelZoomObjects )
#else
  if( event.ControlDown() )
#endif
    wheelZoomFactorY = newWheelFactor;

  wxCoord posX = event.GetX();
  if( posX <= objectAxisPos )
    posX = objectAxisPos + 1;
  else if( posX >= drawZone->GetClientSize().GetWidth() - drawBorder )
    posX = drawZone->GetClientSize().GetWidth() - drawBorder - 1;
  wxCoord pixelsWidth = drawZone->GetClientSize().GetWidth() - objectAxisPos - 1 - drawBorder;
  double ratioLeft = ( (double)( posX - objectAxisPos - 1 ) / (double)pixelsWidth );
  double ratioRight = 1.0 - ratioLeft;
  ratioLeft = ratioLeft * ( 1 - 1 / wheelZoomFactorX );
  ratioRight = ratioRight * ( 1 - 1 / wheelZoomFactorX );
  
  wxCoord pixelsHeight = timeAxisPos;
  double ratioUp = (double)event.GetY() / (double)pixelsHeight;
  double ratioDown = 1.0 - ratioUp;
  ratioUp = ratioUp * ( 1 - 1 / wheelZoomFactorY );
  ratioDown = ratioDown * ( 1 - 1 / wheelZoomFactorY );
  
#ifdef WIN32
  if( wheelZoomObjects )
#else
  if( event.ControlDown() )
#endif
  {
    vector<TObjectOrder> selectedObjects;
    myWindow->getSelectedRows( myWindow->getLevel(),
                               selectedObjects,
                               myWindow->getZoomSecondDimension().first,
                               myWindow->getZoomSecondDimension().second,
                               true );

    TObjectOrder objectHeight = selectedObjects.size();
    PRV_INT64 dummyAppliedAmount;
    newWheelZoomBeginObject = myWindow->shiftFirst( myWindow->getZoomSecondDimension().first, (double)objectHeight * ratioUp, dummyAppliedAmount, myWindow->getLevel() );
    newWheelZoomEndObject = myWindow->shiftLast( myWindow->getZoomSecondDimension().second, - (double)objectHeight * ratioDown, dummyAppliedAmount, myWindow->getLevel() );
  }
  else
  {
    newWheelZoomBeginObject = myWindow->getZoomSecondDimension().first;
    newWheelZoomEndObject = myWindow->getZoomSecondDimension().second;
  }

  newWheelZoomBeginTime = myWindow->getWindowBeginTime() + timeWidth * ratioLeft;
  newWheelZoomEndTime = myWindow->getWindowEndTime() -  timeWidth * ratioRight;

  // Current zoom time boundary check
  if( newWheelZoomEndTime - newWheelZoomBeginTime < 10 || 
      newWheelZoomEndTime < newWheelZoomBeginTime )
    return;

  // Current zoom time correction
  if( newWheelZoomBeginTime < 0 )
  {
    newWheelZoomEndTime -= newWheelZoomBeginTime;
    newWheelZoomBeginTime = 0;
  }
  if( newWheelZoomEndTime > myWindow->getTrace()->getEndTime() )
  {
    newWheelZoomBeginTime -= newWheelZoomEndTime - myWindow->getTrace()->getEndTime();
    newWheelZoomEndTime = myWindow->getTrace()->getEndTime();

    if( newWheelZoomBeginTime < 0 )
      newWheelZoomBeginTime = 0;
  }

  // Setting global parameters
  wheelZoomFactor = newWheelFactor;
  wheelZoomBeginTime = newWheelZoomBeginTime;
  wheelZoomEndTime = newWheelZoomEndTime;
  wheelZoomBeginObject = newWheelZoomBeginObject;
  wheelZoomEndObject = newWheelZoomEndObject;
  
  // Temp draw buffer re-scaled
  wxBitmap tmpBMP;
  tmpBMP.Create( pixelsWidth, pixelsHeight );
  wxMemoryDC tmpDC( tmpBMP );
  tmpDC.SetBrush( wxBrush( backgroundColour ) );
  tmpDC.Clear();
#if __WXMAC__ || WIN32
  tmpDC.DrawRectangle( 0, 0, pixelsWidth, pixelsHeight );
#endif
  tmpDC.SetUserScale( wheelZoomFactorX, wheelZoomFactorY );

  wxCoord pixelBeginX = (double)pixelsWidth * ratioLeft;
  wxCoord pixelBeginY = (double)pixelsHeight * ratioUp;

#if wxMAJOR_VERSION>=3 || !__WXGTK__
  // Source image to temp buffer
  #ifdef __WXMAC__
  wxBitmap tmpDrawImage( drawImage.GetWidth(), drawImage.GetHeight() );
  wxMemoryDC srcDC( tmpDrawImage );
  srcDC.SetBrush( wxBrush( backgroundColour ) );
  srcDC.Clear();
  drawStackedImages( srcDC );
  #else
  wxMemoryDC srcDC( drawImage );
  #endif

  tmpDC.Blit( 0,
              0,
              tmpDC.DeviceToLogicalX( pixelsWidth ),
              tmpDC.DeviceToLogicalY( pixelsHeight ),
              &srcDC,
              objectAxisPos + 1 + pixelBeginX,
              pixelBeginY );

  // Remove axis legend
  if( newWheelFactor < 1.0 )
  {
    tmpDC.SetPen( wxPen( backgroundColour ) );
    tmpDC.SetBrush( wxBrush( backgroundColour ) );
    tmpDC.DrawRectangle( 0, 0, -pixelBeginX, tmpDC.DeviceToLogicalY( timeAxisPos - drawBorder + 1 ) );
  #ifdef WIN32
    if( wheelZoomObjects )
  #else
    if( event.ControlDown() )
  #endif
    {
      tmpDC.DrawRectangle( 0, -pixelBeginY + timeAxisPos, tmpDC.DeviceToLogicalX( tmpBMP.GetWidth() ), tmpBMP.GetHeight() );
    }
  }
#else
  tmpDC.SelectObject( wxNullBitmap );
  wxImage tmpImage = drawImage.ConvertToImage().GetSubImage( wxRect( wxPoint( objectAxisPos + 1, 0 ), wxSize( pixelsWidth, pixelsHeight ) ) );

  if( newWheelFactor >= 1.0 )
  {
    wxCoord pixelEndX = (double)pixelsWidth * ratioRight;
    wxCoord pixelEndY = (double)pixelsHeight * ratioDown;
    wxRect tmpRect( wxPoint( pixelBeginX, pixelBeginY ),
                    wxPoint( pixelsWidth - pixelEndX - 1, pixelsHeight - pixelEndY - 1 ) );
    tmpImage = tmpImage.GetSubImage( tmpRect );
    
    tmpImage.Rescale( pixelsWidth, pixelsHeight );
  }
  else
    tmpImage.Rescale( (double)tmpImage.GetWidth() * wheelZoomFactorX, (double)tmpImage.GetHeight() * wheelZoomFactorY );

  tmpBMP = wxBitmap( tmpImage );
#endif

  tmpDC.SelectObject( wxNullBitmap );

  // Draw zoomed image to timeline window
  wxClientDC dstDC( drawZone );
  dstDC.SetPen( wxPen( backgroundColour ) );
  dstDC.SetBrush( wxBrush( backgroundColour ) );
  dstDC.DrawRectangle( objectAxisPos + 1, 0, drawZone->GetClientSize().GetWidth() - objectAxisPos - 1, timeAxisPos );
#if !( wxMAJOR_VERSION>=3 || !__WXGTK__ )
  if( newWheelFactor >= 1.0 )
  {
#endif  
  dstDC.DrawBitmap( tmpBMP, objectAxisPos + 1, 0 );
#if !( wxMAJOR_VERSION>=3 || !__WXGTK__ )
  }
  else
  {
    dstDC.DrawBitmap( tmpBMP, objectAxisPos + 1 - (double)pixelBeginX * wheelZoomFactorX,
                              -(double)pixelBeginY * wheelZoomFactorY );
  }
#endif

  timerWheel->Start( 750, true );
}


void gTimeline::MousePanMotion()
{
  wxCoord pixelsWidth = drawZone->GetClientSize().GetWidth() - objectAxisPos - drawBorder;
  wxCoord pixelsHeight = timeAxisPos;

  // Temp draw buffer re-scaled
  wxBitmap tmpBMP;
  tmpBMP.Create( pixelsWidth, pixelsHeight );
  wxMemoryDC tmpDC( tmpBMP );
  tmpDC.SetBrush( wxBrush( backgroundColour ) );
  tmpDC.Clear();
  tmpDC.DrawRectangle( 0, 0, pixelsWidth, pixelsHeight );

  // Source image to temp buffer
#ifdef __WXMAC__
  wxBitmap tmpDrawImage( drawImage.GetWidth(), drawImage.GetHeight() );
  wxMemoryDC srcDC( tmpDrawImage );
  srcDC.SetBrush( wxBrush( backgroundColour ) );
  srcDC.Clear();
  drawStackedImages( srcDC );
#else
  wxMemoryDC srcDC( drawImage );
#endif

  wxCoord dstX = zoomBeginX < motionEvent.GetX() ? motionEvent.GetX() - zoomBeginX : 0;
  wxCoord srcX = zoomBeginX < motionEvent.GetX() ? 0 : zoomBeginX - motionEvent.GetX();
  wxCoord dstY;
  wxCoord srcY;
  if( motionEvent.ControlDown() )
  {
    dstY = zoomBeginY < motionEvent.GetY() ? motionEvent.GetY() - zoomBeginY : 0;
    srcY = zoomBeginY < motionEvent.GetY() ? 0 : zoomBeginY - motionEvent.GetY();
  }
  else
  {
    dstY = 0;
    srcY = 0;
  }
  
  wxCoord tmpBlitHeight;
  if( motionEvent.ControlDown() )
    tmpBlitHeight = pixelsHeight - ( zoomBeginY < motionEvent.GetY() ? motionEvent.GetY() - zoomBeginY : zoomBeginY - motionEvent.GetY() );
  else
    tmpBlitHeight = pixelsHeight;

  // Control time limits
  TRecordTime timeWidth = myWindow->getWindowEndTime() - myWindow->getWindowBeginTime();
  TRecordTime panBeginTime = myWindow->getWindowBeginTime() + ( zoomBeginX * timeWidth ) / pixelsWidth;
  TRecordTime panEndTime   = myWindow->getWindowBeginTime() + ( motionEvent.GetX() * timeWidth ) / pixelsWidth;
  TRecordTime deltaTime = panEndTime - panBeginTime;
  panBeginTime = myWindow->getWindowBeginTime() - deltaTime;
  panEndTime   = myWindow->getWindowEndTime()   - deltaTime;
  if( panBeginTime < 0 || panEndTime > myWindow->getTrace()->getEndTime() )
    return;

  // Control object limits
  
  
  tmpDC.Blit( dstX,
              dstY,
              pixelsWidth - ( zoomBeginX < motionEvent.GetX() ? motionEvent.GetX() - zoomBeginX : zoomBeginX - motionEvent.GetX() ),
              tmpBlitHeight,
              &srcDC,
              srcX + objectAxisPos + 1,
              srcY );

  tmpDC.SelectObject( wxNullBitmap );
  srcDC.SelectObject( wxNullBitmap );

  // Draw zoomed image to timeline window
  wxClientDC dstDC( drawZone );
  dstDC.DrawBitmap( tmpBMP, objectAxisPos + 1, 0 );
}


void gTimeline::MousePanLeftUp( wxMouseEvent& event )
{
  TRecordTime  panBeginTime;
  TRecordTime  panEndTime;
  TObjectOrder panBeginObject;
  TObjectOrder panEndObject;
  wxCoord      pixelsWidth = drawZone->GetClientSize().GetWidth() - objectAxisPos - drawBorder;
  wxCoord      pixelsHeight = timeAxisPos;
  
  TRecordTime timeWidth = myWindow->getWindowEndTime() - myWindow->getWindowBeginTime();
  panBeginTime = myWindow->getWindowBeginTime() + ( zoomBeginX * timeWidth ) / pixelsWidth;
  panEndTime   = myWindow->getWindowBeginTime() + ( zoomEndX * timeWidth ) / pixelsWidth;
  TRecordTime deltaTime = panEndTime - panBeginTime;
  
  panBeginTime = myWindow->getWindowBeginTime() - deltaTime;
  panEndTime   = myWindow->getWindowEndTime()   - deltaTime;
  
  if( panBeginTime < 0 )
  {
    panBeginTime = 0;
    panEndTime   = timeWidth;
  }
  if( panEndTime > myWindow->getTrace()->getEndTime() )
  {
    panEndTime   = myWindow->getTrace()->getEndTime();
    panBeginTime = panEndTime - timeWidth;
  }

  if( event.ControlDown() )
  {
    vector<TObjectOrder> selectedObjects;
    myWindow->getSelectedRows( myWindow->getLevel(),
                               selectedObjects,
                               myWindow->getZoomSecondDimension().first,
                               myWindow->getZoomSecondDimension().second,
                               true );

    TObjectOrder objectHeight = selectedObjects.size();
    PRV_INT64 tmpPanBeginObject = (PRV_INT64)myWindow->getZoomSecondDimension().first + (double)( zoomBeginY * objectHeight ) / pixelsHeight;
    PRV_INT64 tmpPanEndObject   = (PRV_INT64)myWindow->getZoomSecondDimension().first + (double)( zoomEndY * objectHeight ) / pixelsHeight;
    PRV_INT64 deltaObject = tmpPanEndObject - tmpPanBeginObject;

    PRV_INT64 appliedDeltaObject;
    if( deltaObject < 0 )
    {
      panEndObject   = myWindow->shiftLast( myWindow->getZoomSecondDimension().second, -deltaObject, appliedDeltaObject, myWindow->getLevel() );
      panBeginObject = myWindow->shiftFirst( myWindow->getZoomSecondDimension().first, appliedDeltaObject, appliedDeltaObject, myWindow->getLevel() );
    }
    else
    {
      panBeginObject = myWindow->shiftFirst( myWindow->getZoomSecondDimension().first, -deltaObject, appliedDeltaObject, myWindow->getLevel() );
      panEndObject   = myWindow->shiftLast( myWindow->getZoomSecondDimension().second, appliedDeltaObject, appliedDeltaObject, myWindow->getLevel() );
    }
  }
  else
  {
    panBeginObject = myWindow->getZoomSecondDimension().first;
    panEndObject = myWindow->getZoomSecondDimension().second;
  }
  
  if( panBeginTime   != myWindow->getWindowBeginTime() ||
      panBeginObject != myWindow->getZoomSecondDimension().first )
  {
    myWindow->addZoom( panBeginTime, panEndTime, panBeginObject, panEndObject );
    myWindow->setWindowBeginTime( panBeginTime, true );
    myWindow->setWindowEndTime( panEndTime, true );
    myWindow->setRedraw( true );
    myWindow->setChanged( true );
  }
  else
  {
    wxClientDC tmpDC( drawZone );
#ifdef __WXMAC__
    drawStackedImages( tmpDC );
#else
    tmpDC.DrawBitmap( drawImage, 0, 0 );
#endif
  }
}

void gTimeline::doDrawCaution( wxDC& whichDC )
{
  if( drawCautionNegatives )
  {
    wxBitmap cautionImage( caution_yellow_xpm );
    whichDC.SetPen( wxPen( backgroundColour ) );
    whichDC.SetBrush( wxBrush( backgroundColour ) );
    whichDC.DrawRectangle( 0, drawZone->GetClientSize().GetHeight() - cautionImage.GetHeight() - drawBorder - 2,
                      drawBorder + cautionImage.GetWidth() + 2, drawZone->GetClientSize().GetHeight() );
    whichDC.DrawBitmap( cautionImage,
                   drawBorder,
                   drawZone->GetClientSize().GetHeight() - cautionImage.GetHeight() - drawBorder,
                   true );
  }
  else if( drawCaution )
  {
    wxBitmap cautionImage( caution_xpm );
    whichDC.SetPen( wxPen( backgroundColour ) );
    whichDC.SetBrush( wxBrush( backgroundColour ) );
    whichDC.DrawRectangle( 0, drawZone->GetClientSize().GetHeight() - cautionImage.GetHeight() - drawBorder - 2,
                            drawBorder + cautionImage.GetWidth() + 2, drawZone->GetClientSize().GetHeight() );
    whichDC.DrawBitmap( cautionImage,
                         drawBorder,
                         drawZone->GetClientSize().GetHeight() - cautionImage.GetHeight() - drawBorder,
                         true );
  }
}

