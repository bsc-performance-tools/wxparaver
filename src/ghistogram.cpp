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
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include <wx/version.h>
#if wxMAJOR_VERSION>=3
#include <wx/dcgraph.h>
#endif

#include <wx/progdlg.h>
#include <wx/clipbrd.h>
#include <wx/display.h>
#include <sstream>
#include <iostream>
#include "config_traits.h"
#include "wxparaverapp.h"
#include "ghistogram.h"
#include "histogram.h"
#include "labelconstructor.h"
#include "histogramtotals.h"
#include "loadedwindows.h"
#include "windows_tree.h"
#include "paravermain.h"
#include "textoutput.h"
#include "paraverkernelexception.h"
#include "histotablebase.h"
#include "filedialogext.h"
//#include "progresscontroller.h"

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
#include "../icons/opencontrol.xpm"
#include "../icons/opendata.xpm"
#include "../icons/open3d.xpm"
#include "../icons/histo_zoom.xpm"
#include "../icons/openfiltered.xpm"
#include "../icons/histo_color.xpm"
#include "../icons/histo_horvert.xpm"
#include "../icons/hide_cols.xpm"
#include "../icons/semantic_color.xpm"
#include "../icons/short_labels.xpm"
#include "../icons/histo_sum.xpm"
#include "../icons/inclusive.xpm"
#include "../icons/histo_sort.xpm"
#include "../icons/arrow_reverse.xpm"
#include "../icons/caution.xpm"
////@end XPM images

/*!
 * gHistogram type definition
 */

IMPLEMENT_CLASS( gHistogram, wxFrame )


/*!
 * gHistogram event table definition
 */

BEGIN_EVENT_TABLE( gHistogram, wxFrame )

////@begin gHistogram event table entries
  EVT_CLOSE( gHistogram::OnCloseWindow )
  EVT_SIZE( gHistogram::OnSize )
  EVT_IDLE( gHistogram::OnIdle )
  EVT_MENU( ID_TOOL_OPEN_CONTROL_WINDOW, gHistogram::OnToolOpenControlWindowClick )
  EVT_MENU( ID_TOOL_OPEN_DATA_WINDOW, gHistogram::OnToolOpenDataWindowClick )
  EVT_MENU( ID_TOOL_OPEN_EXTRA_WINDOW, gHistogram::OnToolOpenExtraWindowClick )
  EVT_UPDATE_UI( ID_TOOL_OPEN_EXTRA_WINDOW, gHistogram::OnToolOpenExtraWindowUpdate )
  EVT_MENU( ID_TOOLZOOM, gHistogram::OnToolzoomClick )
  EVT_UPDATE_UI( ID_TOOLZOOM, gHistogram::OnToolzoomUpdate )
  EVT_MENU( ID_TOOL_OPEN_FILTERED_CONTROL_WINDOW, gHistogram::OnToolOpenFilteredControlWindowClick )
  EVT_UPDATE_UI( ID_TOOL_OPEN_FILTERED_CONTROL_WINDOW, gHistogram::OnToolOpenFilteredControlWindowUpdate )
  EVT_MENU( ID_TOOLGRADIENT, gHistogram::OnToolgradientClick )
  EVT_UPDATE_UI( ID_TOOLGRADIENT, gHistogram::OnToolgradientUpdate )
  EVT_MENU( ID_TOOLHORIZVERT, gHistogram::OnToolhorizvertClick )
  EVT_UPDATE_UI( ID_TOOLHORIZVERT, gHistogram::OnToolhorizvertUpdate )
  EVT_MENU( ID_TOOL_HIDE_COLUMNS, gHistogram::OnToolHideColumnsClick )
  EVT_UPDATE_UI( ID_TOOL_HIDE_COLUMNS, gHistogram::OnToolHideColumnsUpdate )
  EVT_MENU( ID_TOOL_LABEL_COLORS, gHistogram::OnToolLabelColorsClick )
  EVT_UPDATE_UI( ID_TOOL_LABEL_COLORS, gHistogram::OnToolLabelColorsUpdate )
  EVT_MENU( ID_TOOL_SHORT_LABELS, gHistogram::OnToolShortLabelsClick )
  EVT_UPDATE_UI( ID_TOOL_SHORT_LABELS, gHistogram::OnToolShortLabelsUpdate )
  EVT_MENU( ID_TOOL_ONLY_TOTALS, gHistogram::OnToolOnlyTotalsClick )
  EVT_UPDATE_UI( ID_TOOL_ONLY_TOTALS, gHistogram::OnToolOnlyTotalsUpdate )
  EVT_MENU( ID_TOOL_INCLUSIVE, gHistogram::OnToolInclusiveClick )
  EVT_UPDATE_UI( ID_TOOL_INCLUSIVE, gHistogram::OnToolInclusiveUpdate )
  EVT_UPDATE_UI( ID_ZOOMHISTO, gHistogram::OnZoomhistoUpdate )
  EVT_GRID_CELL_LEFT_CLICK( gHistogram::OnCellLeftClick )
  EVT_GRID_CELL_RIGHT_CLICK( gHistogram::OnCellRightClick )
  EVT_GRID_LABEL_LEFT_CLICK( gHistogram::OnLabelLeftClick )
  EVT_GRID_LABEL_RIGHT_CLICK( gHistogram::OnLabelRightClick )
  EVT_GRID_RANGE_SELECT( gHistogram::OnRangeSelect )
  EVT_UPDATE_UI( ID_GRIDHISTO, gHistogram::OnGridhistoUpdate )
  EVT_UPDATE_UI( wxID_CONTROLWARNING, gHistogram::OnControlWarningUpdate )
  EVT_UPDATE_UI( wxID_3DWARNING, gHistogram::On3dWarningUpdate )
////@end gHistogram event table entries
  
  EVT_TIMER( wxID_ANY, gHistogram::OnTimerZoom )
  
END_EVENT_TABLE()

wxProgressDialog *gHistogram::dialogProgress = NULL;

/*!
 * gHistogram constructors
 */

gHistogram::gHistogram()
{
  Init();
}

gHistogram::gHistogram( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( parent, id, caption, pos, size, style );
  this->parent = parent;
}


/*!
 * gHistogram creator
 */

bool gHistogram::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin gHistogram creation
  wxFrame::Create( parent, id, caption, pos, size, style );

  CreateControls();
////@end gHistogram creation

  return true;
}


/*!
 * gHistogram destructor
 */

gHistogram::~gHistogram()
{
////@begin gHistogram destruction
////@end gHistogram destruction
  gPasteWindowProperties::getInstance()->verifyRemove( this );
  
  if( tableBase != NULL )
  {
    gridHisto->SetTable( NULL );
    delete tableBase;
    tableBase = NULL;
  }

  if( myHistogram != NULL )
    delete myHistogram;
  myHistogram = NULL;
}


/*!
 * Member initialisation
 */

void gHistogram::Init()
{
////@begin gHistogram member initialisation
  escapePressed = false;
  lastPosZoomX = 0;
  lastPosZoomY = 0;
  myHistogram = NULL;
  openControlActivated = false;
  ready = false;
  redrawStopWatch = new wxStopWatch();
  tableBase = NULL;
  timerZoom = new wxTimer( this );
  zoomDragging = false;
  panelToolbar = NULL;
  tbarHisto = NULL;
  panelData = NULL;
  mainSizer = NULL;
  zoomHisto = NULL;
  gridHisto = NULL;
  warningSizer = NULL;
  controlWarning = NULL;
  xtraWarning = NULL;
  histoStatus = NULL;
////@end gHistogram member initialisation
  parent = NULL;

}


/*!
 * Control creation for gHistogram
 */

void gHistogram::CreateControls()
{    
////@begin gHistogram content construction
  gHistogram* itemFrame1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemFrame1->SetSizer(itemBoxSizer2);

  panelToolbar = new wxPanel( itemFrame1, HISTO_PANEL_TOOLBAR, wxDefaultPosition, wxDLG_UNIT(itemFrame1, wxSize(200, -1)), wxTAB_TRAVERSAL );
  panelToolbar->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer2->Add(panelToolbar, 0, wxGROW, wxDLG_UNIT(itemFrame1, wxSize(5, -1)).x);

  tbarHisto = new wxToolBar( panelToolbar, ID_TOOLBAR_HISTOGRAM, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL );
  wxBitmap itemtool5Bitmap(itemFrame1->GetBitmapResource(wxT("icons/opencontrol.xpm")));
  wxBitmap itemtool5BitmapDisabled;
  tbarHisto->AddTool(ID_TOOL_OPEN_CONTROL_WINDOW, _("Open Control Window"), itemtool5Bitmap, itemtool5BitmapDisabled, wxITEM_NORMAL, _("Open Control Window"), wxEmptyString);
  wxBitmap itemtool6Bitmap(itemFrame1->GetBitmapResource(wxT("icons/opendata.xpm")));
  wxBitmap itemtool6BitmapDisabled;
  tbarHisto->AddTool(ID_TOOL_OPEN_DATA_WINDOW, _("Open Data Window"), itemtool6Bitmap, itemtool6BitmapDisabled, wxITEM_NORMAL, _("Open Data Window"), wxEmptyString);
  wxBitmap itemtool7Bitmap(itemFrame1->GetBitmapResource(wxT("icons/open3d.xpm")));
  wxBitmap itemtool7BitmapDisabled;
  tbarHisto->AddTool(ID_TOOL_OPEN_EXTRA_WINDOW, _("Open 3rd Window"), itemtool7Bitmap, itemtool7BitmapDisabled, wxITEM_NORMAL, _("Open 3rd Window"), wxEmptyString);
  tbarHisto->AddSeparator();
  wxBitmap itemtool9Bitmap(itemFrame1->GetBitmapResource(wxT("icons/histo_zoom.xpm")));
  wxBitmap itemtool9BitmapDisabled;
  tbarHisto->AddTool(ID_TOOLZOOM, _("Zoom"), itemtool9Bitmap, itemtool9BitmapDisabled, wxITEM_CHECK, _("Histogram zoom"), wxEmptyString);
  wxBitmap itemtool10Bitmap(itemFrame1->GetBitmapResource(wxT("icons/openfiltered.xpm")));
  wxBitmap itemtool10BitmapDisabled;
  tbarHisto->AddTool(ID_TOOL_OPEN_FILTERED_CONTROL_WINDOW, _("Open Filtered Control Window"), itemtool10Bitmap, itemtool10BitmapDisabled, wxITEM_NORMAL, _("Open Filtered Control Window"), wxEmptyString);
  tbarHisto->AddSeparator();
  wxBitmap itemtool12Bitmap(itemFrame1->GetBitmapResource(wxT("icons/histo_color.xpm")));
  wxBitmap itemtool12BitmapDisabled;
  tbarHisto->AddTool(ID_TOOLGRADIENT, _("View Data Gradient Colors"), itemtool12Bitmap, itemtool12BitmapDisabled, wxITEM_CHECK, _("View Data Gradient Colors"), wxEmptyString);
  wxBitmap itemtool13Bitmap(itemFrame1->GetBitmapResource(wxT("icons/histo_horvert.xpm")));
  wxBitmap itemtool13BitmapDisabled;
  tbarHisto->AddTool(ID_TOOLHORIZVERT, _("Horizontal/Vertical"), itemtool13Bitmap, itemtool13BitmapDisabled, wxITEM_CHECK, _("Horizontal/Vertical"), wxEmptyString);
  wxBitmap itemtool14Bitmap(itemFrame1->GetBitmapResource(wxT("icons/hide_cols.xpm")));
  wxBitmap itemtool14BitmapDisabled;
  tbarHisto->AddTool(ID_TOOL_HIDE_COLUMNS, _("Hide Empty Columns"), itemtool14Bitmap, itemtool14BitmapDisabled, wxITEM_CHECK, _("Hide Empty Columns"), wxEmptyString);
  wxBitmap itemtool15Bitmap(itemFrame1->GetBitmapResource(wxT("icons/semantic_color.xpm")));
  wxBitmap itemtool15BitmapDisabled;
  tbarHisto->AddTool(ID_TOOL_LABEL_COLORS, _("Enable Header Colors"), itemtool15Bitmap, itemtool15BitmapDisabled, wxITEM_CHECK, _("Enable Header Colors"), wxEmptyString);
  wxBitmap itemtool16Bitmap(itemFrame1->GetBitmapResource(wxT("icons/short_labels.xpm")));
  wxBitmap itemtool16BitmapDisabled;
  tbarHisto->AddTool(ID_TOOL_SHORT_LABELS, _("Shrink Header Labels"), itemtool16Bitmap, itemtool16BitmapDisabled, wxITEM_CHECK, _("Shrink Header Labels"), wxEmptyString);
  wxBitmap itemtool17Bitmap(itemFrame1->GetBitmapResource(wxT("icons/histo_sum.xpm")));
  wxBitmap itemtool17BitmapDisabled;
  tbarHisto->AddTool(ID_TOOL_ONLY_TOTALS, _("Show Totals Only"), itemtool17Bitmap, itemtool17BitmapDisabled, wxITEM_CHECK, _("Show Totals Only"), wxEmptyString);
  wxBitmap itemtool18Bitmap(itemFrame1->GetBitmapResource(wxT("icons/inclusive.xpm")));
  wxBitmap itemtool18BitmapDisabled;
  tbarHisto->AddTool(ID_TOOL_INCLUSIVE, _("Inclusive/Exclusive"), itemtool18Bitmap, itemtool18BitmapDisabled, wxITEM_CHECK, _("Inclusive/Exclusive"), wxEmptyString);
  tbarHisto->AddSeparator();
  wxStaticBitmap* itemStaticBitmap1 = new wxStaticBitmap( tbarHisto, wxID_STATIC, itemFrame1->GetBitmapResource(wxT("icons/histo_sort.xpm")), wxDefaultPosition, wxDLG_UNIT(tbarHisto, wxSize(6, 4)), 0 );
  if (gHistogram::ShowToolTips())
    itemStaticBitmap1->SetToolTip(_("Sort columns by"));
  tbarHisto->AddControl(itemStaticBitmap1);
  wxArrayString itemChoice2Strings;
  itemChoice2Strings.Add(_("Default"));
  itemChoice2Strings.Add(_("Total"));
  itemChoice2Strings.Add(_("Average"));
  itemChoice2Strings.Add(_("Maximum"));
  itemChoice2Strings.Add(_("Minimum"));
  itemChoice2Strings.Add(_("StDev"));
  itemChoice2Strings.Add(_("Avg/Max"));
  wxChoice* itemChoice2 = new wxChoice( tbarHisto, ID_TOOL_CHOICE_SORTBY, wxDefaultPosition, wxDefaultSize, itemChoice2Strings, 0 );
  itemChoice2->SetStringSelection(_("Default"));
  if (gHistogram::ShowToolTips())
    itemChoice2->SetToolTip(_("Sort columns by"));
  tbarHisto->AddControl(itemChoice2);
  wxBitmap itemtool1Bitmap(itemFrame1->GetBitmapResource(wxT("icons/arrow_reverse.xpm")));
  wxBitmap itemtool1BitmapDisabled;
  tbarHisto->AddTool(ID_TOOL_REVERSE, wxEmptyString, itemtool1Bitmap, itemtool1BitmapDisabled, wxITEM_CHECK, _("Reverse order"), wxEmptyString);
  tbarHisto->Realize();

  panelData = new wxPanel( itemFrame1, HISTO_PANEL_DATA, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
  panelData->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer2->Add(panelData, 1, wxGROW, wxDLG_UNIT(itemFrame1, wxSize(5, -1)).x);

  wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
  panelData->SetSizer(itemBoxSizer20);

  mainSizer = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer20->Add(mainSizer, 1, wxGROW|wxALL, 0);

  zoomHisto = new wxScrolledWindow( panelData, ID_ZOOMHISTO, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxFULL_REPAINT_ON_RESIZE );
  mainSizer->Add(zoomHisto, 1, wxGROW|wxALL, wxDLG_UNIT(panelData, wxSize(1, -1)).x);
  zoomHisto->SetScrollbars(1, 1, 0, 0);

  gridHisto = new wxGrid( panelData, ID_GRIDHISTO, wxDefaultPosition, wxDefaultSize, 0 );
  gridHisto->SetDefaultColSize(wxDLG_UNIT(panelData, wxSize(50, -1)).x);
  gridHisto->SetDefaultRowSize(wxDLG_UNIT(panelData, wxSize(-1, 25)).y);
  gridHisto->SetColLabelSize(wxDLG_UNIT(panelData, wxSize(-1, 25)).y);
  gridHisto->SetRowLabelSize(wxDLG_UNIT(panelData, wxSize(50, -1)).x);
  mainSizer->Add(gridHisto, 1, wxGROW|wxALL, wxDLG_UNIT(panelData, wxSize(1, -1)).x);

  warningSizer = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer20->Add(warningSizer, 0, wxGROW|wxALL, 0);

  controlWarning = new wxStaticBitmap( panelData, wxID_CONTROLWARNING, itemFrame1->GetBitmapResource(wxT("icons/caution.xpm")), wxDefaultPosition, wxDLG_UNIT(panelData, wxSize(8, 7)), 0 );
  if (gHistogram::ShowToolTips())
    controlWarning->SetToolTip(_("Control limits not fitted"));
  warningSizer->Add(controlWarning, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxFIXED_MINSIZE, wxDLG_UNIT(panelData, wxSize(5, -1)).x);

  xtraWarning = new wxStaticBitmap( panelData, wxID_3DWARNING, itemFrame1->GetBitmapResource(wxT("icons/caution.xpm")), wxDefaultPosition, wxDLG_UNIT(panelData, wxSize(8, 7)), 0 );
  if (gHistogram::ShowToolTips())
    xtraWarning->SetToolTip(_("3D limits not fitted"));
  warningSizer->Add(xtraWarning, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxFIXED_MINSIZE, wxDLG_UNIT(panelData, wxSize(5, -1)).x);

  wxStaticBitmap* itemStaticBitmap27 = new wxStaticBitmap( panelData, wxID_STATIC, itemFrame1->GetBitmapResource(wxT("caution.xpm")), wxDefaultPosition, wxDLG_UNIT(panelData, wxSize(9, 9)), 0 );
  itemStaticBitmap27->Show(false);
  warningSizer->Add(itemStaticBitmap27, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxFIXED_MINSIZE, wxDLG_UNIT(panelData, wxSize(5, -1)).x);

  warningSizer->Add(wxDLG_UNIT(panelData, wxSize(10, -1)).x, wxDLG_UNIT(panelData, wxSize(-1, 10)).y, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, wxDLG_UNIT(panelData, wxSize(5, -1)).x);

  histoStatus = new wxStatusBar( itemFrame1, ID_HISTOSTATUS, wxST_SIZEGRIP|wxNO_BORDER );
  histoStatus->SetFieldsCount(1);
  itemFrame1->SetStatusBar(histoStatus);

  // Connect events and objects
  zoomHisto->Connect(ID_ZOOMHISTO, wxEVT_PAINT, wxPaintEventHandler(gHistogram::OnPaint), NULL, this);
  zoomHisto->Connect(ID_ZOOMHISTO, wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(gHistogram::OnEraseBackground), NULL, this);
  zoomHisto->Connect(ID_ZOOMHISTO, wxEVT_LEFT_DOWN, wxMouseEventHandler(gHistogram::OnLeftDown), NULL, this);
  zoomHisto->Connect(ID_ZOOMHISTO, wxEVT_LEFT_UP, wxMouseEventHandler(gHistogram::OnLeftUp), NULL, this);
  zoomHisto->Connect(ID_ZOOMHISTO, wxEVT_MOTION, wxMouseEventHandler(gHistogram::OnMotion), NULL, this);
  zoomHisto->Connect(ID_ZOOMHISTO, wxEVT_CONTEXT_MENU, wxContextMenuEventHandler(gHistogram::OnZoomContextMenu), NULL, this);
  zoomHisto->Connect(ID_ZOOMHISTO, wxEVT_KEY_DOWN, wxKeyEventHandler(gHistogram::OnZoomHistoKeyDown), NULL, this);
////@end gHistogram content construction

#ifdef __WXGTK__
  SetToolBar( tbarHisto );
  panelToolbar->Hide();
#endif

  gridHisto->CreateGrid( 0, 0 );
  gridHisto->EnableEditing( false );
  gridHisto->SetDefaultCellAlignment( wxALIGN_RIGHT, wxALIGN_CENTRE );
#if wxMAJOR_VERSION<3
  gridHisto->SetWindowStyleFlag( wxALWAYS_SHOW_SB|wxHSCROLL|wxVSCROLL );
#else
  gridHisto->ShowScrollbars( wxSHOW_SB_ALWAYS, wxSHOW_SB_ALWAYS );
#endif
}


void gHistogram::execute()
{
  if( myHistogram == NULL )
    return;
  wxString winTitle = GetTitle();
  SetTitle( _("(Working...) ") + winTitle );
  zoomHisto->Show( false );
  gridHisto->Show( false );
  Update();

  ProgressController *progress = NULL;

  if ( myHistogram->getShowProgressBar() )
  {
    // Disabled because some window managers can't show the dialog later
    //redrawStopWatch->Start();
    progress = ProgressController::create( myHistogram->getControlWindow()->getKernel() );
    progress->setHandler( progressFunctionHistogram, this );

#ifndef WIN32
    if( gHistogram::dialogProgress == NULL )
      gHistogram::dialogProgress = new wxProgressDialog( wxT("Computing window..."),
                                                         wxT(""),
                                                         numeric_limits<int>::max(),
                                                         this,
                                                         wxPD_CAN_ABORT|wxPD_AUTO_HIDE|\
                                                         wxPD_APP_MODAL|wxPD_ELAPSED_TIME|\
                                                         wxPD_ESTIMATED_TIME|wxPD_REMAINING_TIME );

  // Disabled because some window managers can't show the dialog later
    //gHistogram::dialogProgress->Show( false );
    gHistogram::dialogProgress->Pulse( winTitle + _( "\t" ) );
    gHistogram::dialogProgress->Fit();
    progress->setMessage( std::string( winTitle.mb_str() ) );
#endif // WIN32
  }


  TObjectOrder beginRow, endRow;
  if( myHistogram->isZoomEmpty() )
  {
    beginRow = myHistogram->getControlWindow()->getZoomSecondDimension().first;
    endRow =  myHistogram->getControlWindow()->getZoomSecondDimension().second;
  }
  else
  {
    beginRow = myHistogram->getZoomSecondDimension().first;
    endRow =  myHistogram->getZoomSecondDimension().second;
  }

  selectedRows.clear();
  selectedRows = myHistogram->getSelectedRows();
  if ( selectedRows.size() == 0 )
    myHistogram->getControlWindow()->getSelectedRows( myHistogram->getControlWindow()->getLevel(),
                                                      selectedRows, beginRow, endRow, true );

  myHistogram->execute( myHistogram->getBeginTime(), myHistogram->getEndTime(), selectedRows, progress );

  if( myHistogram->getZoom() )
    fillZoom();
  else
    fillGrid();

  ready = true;
  
  if( gHistogram::dialogProgress != NULL )
  {
    gHistogram::dialogProgress->Show( false );
    delete gHistogram::dialogProgress;
    gHistogram::dialogProgress = NULL;
  }
  
  if ( progress != NULL )
    delete progress;

  redrawStopWatch->Pause();

  this->Refresh();

  SetTitle( winTitle );

  // If none, we set the current view as the first zoom of the list
  if( myHistogram->isZoomEmpty() )
  {
    HistogramProxy::TZoomInfo currentZoom1, currentZoom2;
    currentZoom1.begin = myHistogram->getControlMin();
    currentZoom1.end = myHistogram->getControlMax();
    currentZoom2.begin = myHistogram->getControlDelta();
    myHistogram->addZoom( currentZoom1, currentZoom2 , beginRow, endRow );
  }

  SetFocus();
}

void gHistogram::fillGrid()
{
  wxFont labelFont = gridHisto->GetLabelFont();
  wxFont cellFontBold = gridHisto->GetDefaultCellFont();
  bool commStat = myHistogram->itsCommunicationStat( myHistogram->getCurrentStat() );
  bool horizontal = myHistogram->getHorizontal();
  bool firstRowColored = myHistogram->getFirstRowColored();
  
  zoomHisto->Show( false );
  gridHisto->Show( true );
  mainSizer->Layout();
  
  if( tableBase == NULL )
    tableBase = new HistoTableBase( myHistogram );
  tableBase->setSelectedRows( &selectedRows );
  cellFontBold.SetWeight( wxFONTWEIGHT_BOLD );
  tableBase->setDefaultFontBold( cellFontBold );

  gridHisto->SetColMinimalAcceptableWidth( 0 );
  gridHisto->SetRowMinimalAcceptableHeight( 0 );
  if( myHistogram->getComputeGradient() )
  {
    myHistogram->recalcGradientLimits();
    myHistogram->setChanged( true );
  }
  if( firstRowColored && !commStat )
  {
    if( horizontal )
    {
      gridHisto->SetRowLabelSize( gridHisto->GetDefaultRowLabelSize() );
      gridHisto->SetColLabelSize( 0 );
    }
    else
    {
      gridHisto->SetColLabelSize( gridHisto->GetDefaultColLabelSize() );
      gridHisto->SetRowLabelSize( 0 );
    }
  }
  else
  {
    gridHisto->SetRowLabelSize( gridHisto->GetDefaultRowLabelSize() );
    gridHisto->SetColLabelSize( gridHisto->GetDefaultColLabelSize() );
  }

  if( myHistogram->getComputeGradient() )
  {
    myHistogram->recalcGradientLimits();
    myHistogram->setChanged( true );
  }

  gridHisto->SetTable( tableBase );

  gridHisto->AutoSizeColumns();
  gridHisto->AutoSizeRows();
  gridHisto->EndBatch();
  gridHisto->ForceRefresh();
}

void gHistogram::fillZoom()
{
  bool commStat = myHistogram->itsCommunicationStat( myHistogram->getCurrentStat() );
  PRV_UINT16 idStat;
  THistogramColumn curPlane;
  THistogramColumn numCols, numDrawCols;
  TObjectOrder numDrawRows;
  bool horizontal = myHistogram->getHorizontal();
  double cellWidth;
  double cellHeight;
  vector<THistogramColumn> noVoidColumns;
  vector<bool> selectedColumns;
  PRV_UINT16 pixelSize = myHistogram->getPixelSize();
  
  gridHisto->Show( false );
  zoomHisto->Show( true );
  mainSizer->Layout();
  
  ready = false;
  zoomImage.Create( zoomHisto->GetSize().GetWidth(), zoomHisto->GetSize().GetHeight() );
  drawImage.Create( zoomHisto->GetSize().GetWidth(), zoomHisto->GetSize().GetHeight() );
  wxMemoryDC bufferDraw( zoomImage );
#ifdef __WXMAC__
  wxGraphicsContext *gc = wxGraphicsContext::Create( bufferDraw );
  gc->SetAntialiasMode( wxANTIALIAS_NONE );
#endif
  bufferDraw.SetBackground( wxBrush( *wxLIGHT_GREY_BRUSH ) );
  bufferDraw.Clear();

  if( !myHistogram->getIdStat( myHistogram->getCurrentStat(), idStat ) )
    throw( std::exception() );

  if( myHistogram->getComputeGradient() )
  {
    myHistogram->recalcGradientLimits();
    myHistogram->setChanged( true );
  }
    
  if( commStat )
    curPlane = myHistogram->getCommSelectedPlane();
  else
    curPlane = myHistogram->getSelectedPlane();

  numCols = myHistogram->getNumColumns( myHistogram->getCurrentStat() );

  if( myHistogram->getHideColumns() )
  {
    if( commStat )
      columnSelection.init( myHistogram->getCommColumnTotals(), idStat, numCols, curPlane );
    else
      columnSelection.init( myHistogram->getColumnTotals(), idStat, numCols, curPlane );
    columnSelection.getSelected( noVoidColumns );
    if( horizontal )
    {
      numDrawCols = noVoidColumns.size();
      numDrawRows = myHistogram->getNumRows();
    }
    else
    {
      numDrawCols = myHistogram->getNumRows();
      numDrawRows = noVoidColumns.size();
    }
    columnSelection.getSelected( selectedColumns );
  }
  else
  {
    if( horizontal )
    {
      numDrawCols = numCols;
      numDrawRows = myHistogram->getNumRows();
    }
    else
    {
      numDrawCols = myHistogram->getNumColumns();
      numDrawRows = numCols;
    }
    selectedColumns.insert( selectedColumns.begin(), numCols, true );
  }
  
  zoomCellWidth = (double)( zoomHisto->GetSize().GetWidth() ) / (double)( numDrawCols + 1 );
  cellWidth = zoomCellWidth  / (double)pixelSize;
  zoomCellHeight = (double)( zoomHisto->GetSize().GetHeight() ) / (double)( numDrawRows + 1 );
  cellHeight = zoomCellHeight  / (double)pixelSize;

  THistogramColumn tmpNumCols = numCols;
  if( myHistogram->getHideColumns() )
    tmpNumCols = noVoidColumns.size();

  bufferDraw.SetBrush( *wxGREY_BRUSH );
  bufferDraw.SetPen( *wxTRANSPARENT_PEN );
  bufferDraw.DrawRectangle( 0, 0, bufferDraw.GetSize().GetWidth(), cellHeight * pixelSize);
  bufferDraw.DrawRectangle( 0, 0, cellWidth * pixelSize, bufferDraw.GetSize().GetHeight() );

  for( THistogramColumn iCol = 0; iCol < tmpNumCols; ++iCol )
  {
    THistogramColumn realCol = iCol;
    if( myHistogram->getHideColumns() )
      realCol = noVoidColumns[ iCol ];
      
    if( commStat )
      myHistogram->setCommFirstCell( realCol, curPlane );
    else
      myHistogram->setFirstCell( realCol, curPlane );


    THistogramColumn beginCol = iCol;
    THistogramColumn endCol = beginCol;
    if( horizontal )
    {
      while( ( endCol + 1 ) < tmpNumCols 
             && rint( ( endCol + 2 ) * cellWidth ) == rint( ( beginCol + 1 ) * cellWidth ) )
      {
        ++endCol;
        THistogramColumn tmpEndCol = endCol;
        if( myHistogram->getHideColumns() )
          tmpEndCol = noVoidColumns[ endCol ];
        if( commStat )
          myHistogram->setCommFirstCell( tmpEndCol, curPlane );
        else
          myHistogram->setFirstCell( tmpEndCol, curPlane );
      }
    }
    else
    {
      while( ( endCol + 1 ) < tmpNumCols 
             && rint( ( endCol + 2 ) * cellHeight ) == rint( ( beginCol + 1 ) * cellHeight ) )
      {
        ++endCol;
        THistogramColumn tmpEndCol = endCol;
        if( myHistogram->getHideColumns() )
          tmpEndCol = noVoidColumns[ endCol ];

        if( commStat )
          myHistogram->setCommFirstCell( tmpEndCol, curPlane );
        else
          myHistogram->setFirstCell( tmpEndCol, curPlane );
      }
    }

    drawColumn( beginCol, endCol, noVoidColumns, bufferDraw );
    if( endCol > iCol )
      iCol = endCol;
  }
  
  bufferDraw.SetPen( *wxBLACK_PEN );
  bufferDraw.SetBrush( *wxTRANSPARENT_BRUSH );
  bufferDraw.DrawRectangle( 0, 0, bufferDraw.GetSize().GetWidth(), bufferDraw.GetSize().GetHeight() );
  if( cellHeight > 5.0 )
  {
    for( TObjectOrder iRow = 0; iRow < numDrawRows; ++iRow )
     bufferDraw.DrawLine( 0, ( iRow + 1 ) * cellHeight * pixelSize, bufferDraw.GetSize().GetWidth(), ( iRow + 1 ) * cellHeight * pixelSize);
  }
  if( cellWidth > 5.0 )
  {
    for( TObjectOrder iCol = 0; iCol < numDrawCols; ++iCol )
     bufferDraw.DrawLine( ( iCol + 1 ) * cellWidth * pixelSize, 0, ( iCol + 1 ) * cellWidth * pixelSize, bufferDraw.GetSize().GetHeight() );
  }
  
  bufferDraw.SelectObject( wxNullBitmap );
  bufferDraw.SelectObject( drawImage );
  bufferDraw.DrawBitmap( zoomImage, 0, 0, false );
#ifdef __WXMAC__
  delete gc;
#endif
  
  zoomHisto->Refresh();
  ready = true;
}

void gHistogram::drawColumn( THistogramColumn beginColumn, THistogramColumn endColumn,
                             vector<THistogramColumn>& noVoidColumns, wxMemoryDC& bufferDraw )
{
  TObjectOrder numRows = myHistogram->getNumRows();
  
  bool commStat = myHistogram->itsCommunicationStat( myHistogram->getCurrentStat() );
  bool horizontal = myHistogram->getHorizontal();
  bool firstRowColored = myHistogram->getFirstRowColored();
  PRV_UINT16 idStat;
  THistogramColumn curPlane;
  vector<TSemanticValue> valuesColumns;
  vector<TSemanticValue> valuesObjects;
  PRV_UINT16 pixelSize = myHistogram->getPixelSize();
  double cellWidth = zoomCellWidth / (double)pixelSize;
  double cellHeight = zoomCellHeight / (double)pixelSize;
  
  if( !myHistogram->getIdStat( myHistogram->getCurrentStat(), idStat ) )
    throw( std::exception() );

  if( commStat )
    curPlane = myHistogram->getCommSelectedPlane();
  else
    curPlane = myHistogram->getSelectedPlane();

  valuesObjects.clear();
  for( TObjectOrder iRow = 0; iRow < numRows; ++iRow )
  {
    valuesColumns.clear();
    for( THistogramColumn drawCol = beginColumn; drawCol <= endColumn; ++drawCol )
    {
      THistogramColumn iCol = drawCol;
      if( myHistogram->getHideColumns() )
        iCol = noVoidColumns[ drawCol ];
        
      if( !( ( commStat && myHistogram->endCommCell( iCol, curPlane ) ) ||
            ( !commStat && myHistogram->endCell( iCol, curPlane ) ) ) )
      {
        if( commStat )
        {
          if( myHistogram->getCommCurrentRow( iCol, curPlane ) == iRow )
          {
            valuesColumns.push_back( myHistogram->getCommCurrentValue( iCol, idStat, curPlane ) );
            myHistogram->setCommNextCell( iCol, curPlane );
          }
        }
        else
        {
          if( myHistogram->getCurrentRow( iCol, curPlane ) == iRow )
          {
            valuesColumns.push_back( myHistogram->getCurrentValue( iCol, idStat, curPlane ) );
            myHistogram->setNextCell( iCol, curPlane );
          }
        }
      }
    }
    if( valuesColumns.begin() != valuesColumns.end() )
    {
      if( commStat )
        valuesObjects.push_back( DrawMode::selectValue( valuesColumns, myHistogram->getDrawModeObjects() ) );
      else
        valuesObjects.push_back( DrawMode::selectValue( valuesColumns, myHistogram->getDrawModeColumns() ) );
    }

    THistogramColumn iDrawCol;
    TObjectOrder iDrawRow;
    if( horizontal )
    {
      iDrawCol = beginColumn;
      iDrawRow = iRow;
    }
    else
    {
      iDrawCol = iRow;
      iDrawRow = beginColumn;
    }

    if( firstRowColored && !commStat )
    {
      THistogramColumn firstDrawCol = iDrawCol;
      TObjectOrder firstDrawRow = iDrawRow;
      if( horizontal )
        ++firstDrawCol;
      else
        ++firstDrawRow;
          
      rgb tmpCol;
      Window *controlWindow = myHistogram->getControlWindow();
      THistogramColumn tmpBeginCol = beginColumn;
      if( myHistogram->getHideColumns() )
        tmpBeginCol = noVoidColumns[ beginColumn ];

      TSemanticValue tmpValue = ( tmpBeginCol * myHistogram->getControlDelta() ) +
                                myHistogram->getControlMin();

      if( myHistogram->getControlWindow()->isCodeColorSet() )
        tmpCol = controlWindow->getCodeColor().calcColor( tmpValue,
                                                          myHistogram->getControlMin(),
                                                          myHistogram->getControlMax() );
      else
        tmpCol = controlWindow->getGradientColor().calcColor( tmpValue,
                                                              controlWindow->getMinimumY(),
                                                              controlWindow->getMaximumY() );
      bufferDraw.SetBrush( wxBrush( wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) ) );
      bufferDraw.DrawRectangle( rint( ( firstDrawCol ) * cellWidth * pixelSize ), rint( ( firstDrawRow ) * cellHeight *pixelSize ),
                                cellWidth < 1.0 ? pixelSize * 2 : ceil( cellWidth * pixelSize ),
                                cellHeight < 1.0 ? pixelSize * 2 : ceil( cellHeight * pixelSize ) );
      firstRowColored = false;
    }
      
    if( valuesObjects.begin() != valuesObjects.end() &&
        ( iRow == numRows || 
          ( horizontal && ceil( ( iRow + 1 ) * cellHeight ) != ceil( ( iRow + 2 ) * cellHeight ) ) ||
          ( !horizontal && ceil( ( iRow + 1 ) * cellWidth ) != ceil( ( iRow + 2 ) * cellWidth ) )
        )
      )
    {
      bool tmpDrawColor = false;
      rgb tmpColorToDraw;
      TSemanticValue tmpValueToDraw = DrawMode::selectValue( valuesObjects, myHistogram->getDrawModeObjects() );
      if( myHistogram->getColorMode() == SemanticColor::COLOR )
      {
        tmpColorToDraw = myHistogram->getDataWindow()->getCodeColor().calcColor( tmpValueToDraw,
                                                                                 myHistogram->getMinGradient(), myHistogram->getMaxGradient() );
        tmpDrawColor = true;
      }
      else
      {
        if( myHistogram->getColorMode() == SemanticColor::GRADIENT ||
            ( myHistogram->getColorMode() == SemanticColor::NOT_NULL_GRADIENT && tmpValueToDraw != 0.0 ) )
        {
          tmpColorToDraw = myHistogram->calcGradientColor( tmpValueToDraw );
          tmpDrawColor = true;
        }
      }
      if( tmpDrawColor )
      {
        bufferDraw.SetBrush( wxBrush( wxColour( tmpColorToDraw.red, tmpColorToDraw.green, tmpColorToDraw.blue ) ) );
        bufferDraw.DrawRectangle( rint( ( iDrawCol + 1 ) * cellWidth * pixelSize ), rint( ( iDrawRow + 1 ) * cellHeight * pixelSize ),
                                  cellWidth < 1.0 ? pixelSize * 2 : ceil( cellWidth * pixelSize ),
                                  cellHeight < 1.0 ? pixelSize * 2 : ceil( cellHeight * pixelSize ) );
      }
      
      valuesObjects.clear();
    }
  }
}


vector< TObjectOrder > gHistogram::getSelectedRows()
{
  return myHistogram->getSelectedRows();
}

void gHistogram::setSelectedRows( vector< bool > &selected )
{
  myHistogram->setSelectedRows( selected );
}

void gHistogram::setSelectedRows( vector< TObjectOrder > &selected )
{
  myHistogram->setSelectedRows( selected );
}


/*!
 * Should we show tooltips?
 */

bool gHistogram::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap gHistogram::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin gHistogram bitmap retrieval
  wxUnusedVar(name);
  if (name == wxT("icons/opencontrol.xpm"))
  {
    wxBitmap bitmap(opencontrol_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/opendata.xpm"))
  {
    wxBitmap bitmap(opendata_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/open3d.xpm"))
  {
    wxBitmap bitmap(open3d_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/histo_zoom.xpm"))
  {
    wxBitmap bitmap(histo_zoom_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/openfiltered.xpm"))
  {
    wxBitmap bitmap(openfiltered_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/histo_color.xpm"))
  {
    wxBitmap bitmap(color_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/histo_horvert.xpm"))
  {
    wxBitmap bitmap(horvert_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/hide_cols.xpm"))
  {
    wxBitmap bitmap(hide_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/semantic_color.xpm"))
  {
    wxBitmap bitmap(semantic_color_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/short_labels.xpm"))
  {
    wxBitmap bitmap(short_labels_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/histo_sum.xpm"))
  {
    wxBitmap bitmap(histo_sum_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/inclusive.xpm"))
  {
    wxBitmap bitmap(inclusive_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/histo_sort.xpm"))
  {
    wxBitmap bitmap(sortcols_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/arrow_reverse.xpm"))
  {
    wxBitmap bitmap(arrow_inverse_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/caution.xpm"))
  {
    wxBitmap bitmap(caution_xpm);
    return bitmap;
  }
  else if (name == wxT("caution.xpm"))
  {
    wxBitmap bitmap(caution_xpm);
    return bitmap;
  }
  return wxNullBitmap;
////@end gHistogram bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon gHistogram::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin gHistogram icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end gHistogram icon retrieval
}


/*!
 * wxEVT_IDLE event handler for ID_GHISTOGRAM
 */

void gHistogram::OnIdle( wxIdleEvent& event )
{
  if( myHistogram->getDestroy() )
    return;

#ifndef WIN32
//  if( myHistogram->getZoom() )
  if( IsActive() )
    zoomHisto->SetFocus();
//  else
//    gridHisto->SetFocus();
#endif

  string composedName = myHistogram->getName() + " @ " +
                        myHistogram->getTrace()->getTraceNameNumbered();

  this->SetTitle( wxString::FromAscii( composedName.c_str() ) );

  if(  !wxparaverApp::mainWindow->IsIconized() && myHistogram->getShowWindow() )
  {
    this->Show();
//    paraverMain::myParaverMain->addActiveWindow( this );
  }
  else
  {
    this->Show( false );
//    paraverMain::myParaverMain->removeActiveWindow( this );
  }
    

/*  if( ParaverConfig::??? )
  {
    myHistogram->setPosX( this->GetPosition().x );
    myHistogram->setPosY( this->GetPosition().y );
  }
  else
*/
  {
    int currentDisplay = wxDisplay::GetFromWindow( this );
    if ( currentDisplay != wxNOT_FOUND && currentDisplay >= 0 )
    {
      wxDisplay tmpDisplay( currentDisplay );
      myHistogram->setPosX( this->GetPosition().x - tmpDisplay.GetGeometry().x );
      myHistogram->setPosY( this->GetPosition().y - tmpDisplay.GetGeometry().y );
    }
  }

  myHistogram->setWidth( this->GetClientSize().GetWidth() );
  myHistogram->setHeight( this->GetClientSize().GetHeight() );
  
  controlWarning->Show( myHistogram->getControlOutOfLimits() );
  xtraWarning->Show( myHistogram->getExtraOutOfLimits() );
  Layout();
}

bool isSyncedWithGroup( Window *whichWindow, unsigned int whichGroup )
{
  return whichWindow != NULL && whichWindow->isSync() && whichWindow->getSyncGroup() == whichGroup && whichWindow->getShowWindow();
}

void gHistogram::updateHistogram()
{
  //rowSelection.getSelected( selectedRows );

  if( myHistogram->getForceRecalc() || ( wxparaverApp::mainWindow->getAutoRedraw() && myHistogram->getRecalc() ) )
  {
    unsigned int tmpGroup = myHistogram->getSyncGroup();
    if( ( isSyncedWithGroup( myHistogram->getControlWindow(), tmpGroup )      && !myHistogram->getControlWindow()->getReady() ) ||
        ( isSyncedWithGroup( myHistogram->getDataWindow(), tmpGroup )         && !myHistogram->getDataWindow()->getReady() ) ||
        ( isSyncedWithGroup( myHistogram->getExtraControlWindow(), tmpGroup ) && !myHistogram->getExtraControlWindow()->getReady() ) )
      return;
    if( gHistogram::dialogProgress != NULL )
      return;
    myHistogram->setRecalc( false );
    myHistogram->setForceRecalc( false );
    execute();
    myHistogram->setChanged( true );
  }
  else if( this->IsShown() )
  {
    if( myHistogram->getRedraw() )
    {
      wxString winTitle = GetTitle();
      SetTitle( _("(Working...) ") + winTitle );
      Update();
      
      myHistogram->setRedraw( false );
      if( myHistogram->getZoom() )
        fillZoom();
      else
        fillGrid();

      SetTitle( winTitle );
    }
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_GRIDHISTO
 */

void gHistogram::OnGridhistoUpdate( wxUpdateUIEvent& event )
{
  if( myHistogram->getDestroy() )
    return;

  updateHistogram();
}


/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_GHISTOGRAM
 */

void gHistogram::OnCloseWindow( wxCloseEvent& event )
{
  myHistogram->setShowWindow( false );
}


/*!
 * wxEVT_GRID_RANGE_SELECT event handler for ID_GRIDHISTO
 */

void gHistogram::OnRangeSelect( wxGridRangeSelectEvent& event )
{
  // NEW selection: what you see is what you select
  if (wxTheClipboard->Open())
  {
    wxGridCellCoords topLeft = event.GetTopLeftCoords();
    wxGridCellCoords bottomRight = event.GetBottomRightCoords();
    //wxTheClipboard->SetData( new wxTextDataObject( gridHisto->GetCellValue( topLeft ) ) );
    
    wxString clippy = _( "" );
    for ( int i = topLeft.GetRow() ; i <= bottomRight.GetRow() ; ++i )
    {
      for ( int j = topLeft.GetCol() ; j <= bottomRight.GetCol() ; ++j )
      {
        if ( gridHisto->GetColSize(j) > 0)  
          clippy += gridHisto->GetCellValue( i, j ) + _( "\t" );
      }
      clippy += _( "\n" );
    }
    
    wxTheClipboard->SetData( new wxTextDataObject( clippy ) );
    wxTheClipboard->Close();
  }
}

void gHistogram::OnPopUpCopy()
{
  gPasteWindowProperties::pasteWindowProperties->getInstance()->copy( this );
}

void gHistogram::OnPopUpPaste()
{
//  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this );
}

void gHistogram::OnPopUpPasteSpecial()
{
  wxArrayString choices;
  
  wxMultiChoiceDialog *dialog = gPopUpMenu::createPasteSpecialDialog( choices, this );

  if ( dialog->ShowModal() == wxID_OK )
  {
    wxArrayInt selections = dialog->GetSelections();
    if ( selections.GetCount() > 0 )
    {
      bool recalc = false;
      for ( size_t i = 0; i < selections.GetCount(); i++ )
      {
        gPasteWindowProperties* pasteActions = gPasteWindowProperties::pasteWindowProperties->getInstance();
        if ( pasteActions->isAllowed( this, gPopUpMenu::getOption( choices, selections[i] ) ) )
        {
          if ( gPopUpMenu::getOption( choices, selections[i] ) == "Time" )
            recalc = true;
          else if ( gPopUpMenu::getOption( choices, selections[i] ) == STR_CONTROL_SCALE )
          {
            myHistogram->setCompute2DScale( false );
            recalc = true;
          }
          else if ( gPopUpMenu::getOption( choices, selections[i] ) == STR_3D_SCALE )
          {
            myHistogram->setCompute3DScale( false );
            recalc = true;
          }
          pasteActions->paste( this, gPopUpMenu::getOption( choices, selections[i] ) );
        }
      }

      if ( recalc )
        myHistogram->setRecalc( true );
      updateHistogram();
    }
  }

  delete dialog;
}

void gHistogram::OnPopUpPasteDefaultSpecial()
{
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Time" );
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Objects" );
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Size" );
  myHistogram->setRecalc( true );
  updateHistogram();
}

void gHistogram::OnPopUpPasteTime()
{
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Time" );
  myHistogram->setRecalc( true );
  updateHistogram();
}

void gHistogram::OnPopUpPasteObjects()
{
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Objects" );
  myHistogram->setRecalc( true );
  updateHistogram();
}

void gHistogram::OnPopUpPasteSize()
{
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Size" );
  updateHistogram();
}

void gHistogram::OnPopUpPasteDuration()
{
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Duration" );
  myHistogram->setRecalc( true );
  updateHistogram();
}

void gHistogram::OnPopUpPasteSemanticScale()
{
  myHistogram->setComputeGradient( false );
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Semantic Scale" );
  myHistogram->setChanged( true );
  myHistogram->setRedraw( true );
  updateHistogram();
}

void gHistogram::OnPopUpPasteControlScale()
{
  myHistogram->setCompute2DScale( false );
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, STR_CONTROL_SCALE );
  Histogram::TZoomInfo tmpZoom1, tmpZoom2;
  tmpZoom1.begin = myHistogram->getControlMin();
  tmpZoom1.end   = myHistogram->getControlMax();
  tmpZoom2.begin = myHistogram->getControlDelta();
  myHistogram->addZoom( tmpZoom1, tmpZoom2 );
  
  myHistogram->setRecalc( true );
  updateHistogram();
}

void gHistogram::OnPopUpPaste3DScale()
{
  myHistogram->setCompute3DScale( false );
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, STR_3D_SCALE );
  updateHistogram();
  myHistogram->setRecalc( true );
}

void gHistogram::OnPopUpPasteControlDimensions()
{
  myHistogram->setCompute2DScale( false );
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, STR_CONTROL_DIMENSIONS );
  Histogram::TZoomInfo tmpZoom1, tmpZoom2;
  tmpZoom1.begin = myHistogram->getControlMin();
  tmpZoom1.end   = myHistogram->getControlMax();
  tmpZoom2.begin = myHistogram->getControlDelta();
  myHistogram->addZoom( tmpZoom1, tmpZoom2 );

  updateHistogram();
  myHistogram->setRecalc( true );
}


void gHistogram::OnPopUpClone()
{
  Histogram *clonedHistogram = myHistogram->clone();
  string clonedName = clonedHistogram->getName();

  // Create empty gHistogram and assign window with same dimensions.
  // Shifts position right and down.
  wxSize titleBarSize = GetSize() - GetClientSize();
  if ( titleBarSize.GetHeight() == 0 )
    titleBarSize = paraverMain::defaultTitleBarSize;

  wxPoint position =  wxPoint( this->GetPosition().x + titleBarSize.GetHeight(),
                               this->GetPosition().y + titleBarSize.GetHeight() );
  wxSize size = wxSize( myHistogram->getWidth(), myHistogram->getHeight()/* + titleBarSize.GetHeight()*/ );

  string composedName = clonedName + " @ " +
                        clonedHistogram->getTrace()->getTraceNameNumbered();

  gHistogram *clonedGHistogram = new gHistogram( parent, wxID_ANY, wxString::FromAscii( composedName.c_str() ), position );
  clonedGHistogram->myHistogram = clonedHistogram;
  clonedGHistogram->SetClientSize( size );

  clonedGHistogram->ready = false;

  LoadedWindows::getInstance()->add( clonedHistogram );
  appendHistogram2Tree( clonedGHistogram );
  
  // Window clone
  bool found = false;
  gTimeline *controlGTimeline = getGTimelineFromWindow( getAllTracesTree()->GetRootItem(),
                                                        GetHistogram()->getControlWindow(),
                                                        found );
  if ( found )
    controlGTimeline->clone( clonedHistogram->getControlWindow(),
                             parent,
                             getAllTracesTree()->GetRootItem(),
                             getSelectedTraceTree( clonedHistogram->getControlWindow()->getTrace() )->GetRootItem() );
  else
    throw new ParaverKernelException( ParaverKernelException::undefined, "ERROR! NOT FOUND ORIGINAL CONTROL WINDOW OF HISTOGRAM!", __FILE__, __LINE__ );
    
  if ( GetHistogram()->getDataWindow() != GetHistogram()->getControlWindow() )
  {
    found = false;
    gTimeline *dataGTimeline = getGTimelineFromWindow( getAllTracesTree()->GetRootItem(),
                                                       GetHistogram()->getDataWindow(),
                                                       found );
    if ( found )
      dataGTimeline->clone( clonedHistogram->getDataWindow(),
                            parent,
                            getAllTracesTree()->GetRootItem(),
                            getSelectedTraceTree( clonedHistogram->getDataWindow()->getTrace() )->GetRootItem() );
    else
      throw new ParaverKernelException( ParaverKernelException::undefined, "ERROR! NOT FOUND ORIGINAL DATA WINDOW OF HISTOGRAM!", __FILE__, __LINE__ );
  }

  if ( GetHistogram()->getExtraControlWindow() != NULL &&
       GetHistogram()->getExtraControlWindow() != GetHistogram()->getControlWindow() &&
       GetHistogram()->getExtraControlWindow() != GetHistogram()->getDataWindow() )
  {
    found = false;
    gTimeline *extraControlGTimeline = getGTimelineFromWindow( getAllTracesTree()->GetRootItem(),
                                                               GetHistogram()->getExtraControlWindow(),
                                                               found );
    if ( found )
      extraControlGTimeline->clone( clonedHistogram->getExtraControlWindow(),
                                    parent,
                                    getAllTracesTree()->GetRootItem(),
                                    getSelectedTraceTree( clonedHistogram->getExtraControlWindow()->getTrace() )->GetRootItem() );
    else
      throw new ParaverKernelException( ParaverKernelException::undefined, "ERROR! NOT FOUND ORIGINAL EXTRA CONTROL WINDOW OF HISTOGRAM!", __FILE__, __LINE__ );
  }

  // Finally, execute
  clonedGHistogram->myHistogram->setRecalc( true );
}


void gHistogram::OnPopUpRename()
{
  paraverMain::myParaverMain->renameTreeItem( );
}

void gHistogram::OnPopUpFitTimeScale()
{
  myHistogram->setWindowBeginTime( 0 );
  myHistogram->setWindowEndTime( myHistogram->getControlWindow()->getTrace()->getEndTime() );
  myHistogram->setRecalc( true );
  updateHistogram();
}


void gHistogram::OnPopUpFitObjects()
{
  HistogramProxy::TZoomInfo currentZoom1, currentZoom2;
  TObjectOrder beginRow, endRow;

  currentZoom1.begin = myHistogram->getControlMin();
  currentZoom1.end = myHistogram->getControlMax();
  currentZoom2.begin = myHistogram->getControlDelta();
  beginRow = TObjectOrder( 0 );
  endRow = myHistogram->getDataWindow()->getWindowLevelObjects() - 1;

  // zoom?
  myHistogram->addZoom( currentZoom1, currentZoom2 , beginRow, endRow );

  myHistogram->setRecalc( true );
  updateHistogram();
}


void gHistogram::OnPopUpRowSelection()
{
  
  RowsSelectionDialog *dialog = gPopUpMenu::createRowSelectionDialog( this );

  if ( dialog->ShowModal() == wxID_OK )
  {
    if ( dialog->ShouldChangeTimelineZoom() )
    {
      myHistogram->addZoom( dialog->GetNewBeginZoom(), dialog->GetNewEndZoom() );
    }
  
    myHistogram->setRecalc( true );
    //updateHistogram();
  }
  delete dialog;
}


void gHistogram::OnPopUpAutoControlScale( bool state )
{
  myHistogram->setCompute2DScale( state );
  if( state )
    myHistogram->setRecalc( true );
}

void gHistogram::OnPopUpAuto3DScale( bool state )
{
  myHistogram->setCompute3DScale( state );
  if( state )
    myHistogram->setRecalc( true );
}

void gHistogram::OnPopUpAutoDataGradient( bool state )
{
  myHistogram->setComputeGradient( state );
  if( state )
    myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeSemanticLast()
{
  myHistogram->setDrawModeColumns( DRAW_LAST );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeSemanticRandom()
{
  myHistogram->setDrawModeColumns( DRAW_RANDOM );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeSemanticRandomNotZero()
{
  myHistogram->setDrawModeColumns( DRAW_RANDNOTZERO );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeSemanticMaximum()
{
  myHistogram->setDrawModeColumns( DRAW_MAXIMUM );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeSemanticMinimumNotZero()
{
  myHistogram->setDrawModeColumns( DRAW_MINNOTZERO );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeSemanticAbsoluteMaximum()
{
  myHistogram->setDrawModeColumns( DRAW_ABSOLUTE_MAXIMUM );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeSemanticAbsoluteMinimumNotZero()
{
  myHistogram->setDrawModeColumns( DRAW_ABSOLUTE_MINNOTZERO );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeSemanticAverage()
{
  myHistogram->setDrawModeColumns( DRAW_AVERAGE );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeSemanticAverageNotZero()
{
  myHistogram->setDrawModeColumns( DRAW_AVERAGENOTZERO );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeSemanticMode()
{
  myHistogram->setDrawModeColumns( DRAW_MODE );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeObjectsLast()
{
  myHistogram->setDrawModeObjects( DRAW_LAST );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeObjectsRandom()
{
  myHistogram->setDrawModeObjects( DRAW_RANDOM );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeObjectsRandomNotZero()
{
  myHistogram->setDrawModeObjects( DRAW_RANDNOTZERO );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeObjectsMaximum()
{
  myHistogram->setDrawModeObjects( DRAW_MAXIMUM );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeObjectsMinimumNotZero()
{
  myHistogram->setDrawModeObjects( DRAW_MINNOTZERO );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeObjectsAbsoluteMaximum()
{
  myHistogram->setDrawModeObjects( DRAW_ABSOLUTE_MAXIMUM );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeObjectsAbsoluteMinimumNotZero()
{
  myHistogram->setDrawModeObjects( DRAW_ABSOLUTE_MINNOTZERO );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeObjectsAverage()
{
  myHistogram->setDrawModeObjects( DRAW_AVERAGE );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeObjectsAverageNotZero()
{
  myHistogram->setDrawModeObjects( DRAW_AVERAGENOTZERO );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeObjectsMode()
{
  myHistogram->setDrawModeObjects( DRAW_MODE );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeBothLast()
{
  myHistogram->setDrawModeObjects( DRAW_LAST );
  myHistogram->setDrawModeColumns( DRAW_LAST );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeBothRandom()
{
  myHistogram->setDrawModeObjects( DRAW_RANDOM );
  myHistogram->setDrawModeColumns( DRAW_RANDOM );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeBothRandomNotZero()
{
  myHistogram->setDrawModeObjects( DRAW_RANDNOTZERO );
  myHistogram->setDrawModeColumns( DRAW_RANDNOTZERO );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeBothMaximum()
{
  myHistogram->setDrawModeObjects( DRAW_MAXIMUM );
  myHistogram->setDrawModeColumns( DRAW_MAXIMUM );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeBothMinimumNotZero()
{
  myHistogram->setDrawModeObjects( DRAW_MINNOTZERO );
  myHistogram->setDrawModeColumns( DRAW_MINNOTZERO );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeBothAbsoluteMaximum()
{
  myHistogram->setDrawModeObjects( DRAW_ABSOLUTE_MAXIMUM );
  myHistogram->setDrawModeColumns( DRAW_ABSOLUTE_MAXIMUM );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeBothAbsoluteMinimumNotZero()
{
  myHistogram->setDrawModeObjects( DRAW_ABSOLUTE_MINNOTZERO );
  myHistogram->setDrawModeColumns( DRAW_ABSOLUTE_MINNOTZERO );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeBothAverage()
{
  myHistogram->setDrawModeObjects( DRAW_AVERAGE );
  myHistogram->setDrawModeColumns( DRAW_AVERAGE );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeBothAverageNotZero()
{
  myHistogram->setDrawModeObjects( DRAW_AVERAGENOTZERO );
  myHistogram->setDrawModeColumns( DRAW_AVERAGENOTZERO );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpDrawModeBothMode()
{
  myHistogram->setDrawModeObjects( DRAW_MODE );
  myHistogram->setDrawModeColumns( DRAW_MODE );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpPixelSize( PRV_UINT16 whichPixelSize )
{
  myHistogram->setPixelSize( whichPixelSize );
  myHistogram->setRedraw( true );
}

void gHistogram::OnPopUpUndoZoom()
{
  if ( !GetHistogram()->emptyPrevZoom() )
  {
    GetHistogram()->prevZoom();
    THistogramLimit columnBegin = GetHistogram()->getZoomFirstDimension().first.begin;
    THistogramLimit columnEnd = GetHistogram()->getZoomFirstDimension().first.end;
    THistogramLimit delta = GetHistogram()->getZoomFirstDimension().second.begin;
    TObjectOrder objectBegin = GetHistogram()->getZoomSecondDimension().first;
    TObjectOrder objectEnd = GetHistogram()->getZoomSecondDimension().second;

    zoom( columnBegin, columnEnd, objectBegin, objectEnd, delta );
  }
}

void gHistogram::OnPopUpRedoZoom()
{
  if ( !GetHistogram()->emptyNextZoom() )
  {
    GetHistogram()->nextZoom();
    THistogramLimit columnBegin = GetHistogram()->getZoomFirstDimension().first.begin;
    THistogramLimit columnEnd = GetHistogram()->getZoomFirstDimension().first.end;
    THistogramLimit delta = GetHistogram()->getZoomFirstDimension().second.begin;
    TObjectOrder objectBegin = GetHistogram()->getZoomSecondDimension().first;
    TObjectOrder objectEnd = GetHistogram()->getZoomSecondDimension().second;

    zoom( columnBegin, columnEnd, objectBegin, objectEnd, delta );
  }
}

void gHistogram::rightDownManager()
{
  paraverMain::myParaverMain->selectTrace( GetHistogram()->getControlWindow()->getTrace() );

  gPopUpMenu popUpMenu( this );
  popUpMenu.enable( "Undo Zoom", !GetHistogram()->emptyPrevZoom() );
  popUpMenu.enable( "Redo Zoom", !GetHistogram()->emptyNextZoom() );

  popUpMenu.enableMenu( this );
  PopupMenu( &popUpMenu );
}

/*!
 * wxEVT_GRID_CELL_RIGHT_CLICK event handler for ID_GRIDHISTO
 */

void gHistogram::OnCellRightClick( wxGridEvent& event )
{
  rightDownManager();
}


/*!
 * wxEVT_GRID_LABEL_RIGHT_CLICK event handler for ID_GRIDHISTO
 */

void gHistogram::OnLabelRightClick( wxGridEvent& event )
{
  rightDownManager();
}




/*!
 * wxEVT_ERASE_BACKGROUND event handler for ID_ZOOMHISTO
 */

void gHistogram::OnEraseBackground( wxEraseEvent& event )
{
//  event.Skip();
}


/*!
 * wxEVT_PAINT event handler for ID_ZOOMHISTO
 */

void gHistogram::OnPaint( wxPaintEvent& event )
{
  wxPaintDC dc( zoomHisto );
  
  if( ready && drawImage.IsOk() )
    dc.DrawBitmap( drawImage, 0, 0, false );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_ZOOMHISTO
 */

void gHistogram::OnZoomhistoUpdate( wxUpdateUIEvent& event )
{
  updateHistogram();
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOLZOOM
 */

void gHistogram::OnToolzoomClick( wxCommandEvent& event )
{
  myHistogram->setZoom( event.IsChecked() );
  myHistogram->setRedraw( true );
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOLGRADIENT
 */

void gHistogram::OnToolgradientClick( wxCommandEvent& event )
{
  myHistogram->setShowColor( event.IsChecked() );
  myHistogram->setRedraw( true );
  if( !myHistogram->getZoom() )
    gridHisto->Refresh();
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOLHORIZVERT
 */

void gHistogram::OnToolhorizvertClick( wxCommandEvent& event )
{
  myHistogram->setHorizontal( event.IsChecked() );
  myHistogram->setRedraw( true );
  if( !myHistogram->getZoom() )
    gridHisto->Refresh();
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TOOLZOOM
 */

void gHistogram::OnToolzoomUpdate( wxUpdateUIEvent& event )
{
  event.Check( myHistogram->getZoom() );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TOOLGRADIENT
 */

void gHistogram::OnToolgradientUpdate( wxUpdateUIEvent& event )
{
  event.Check( myHistogram->getShowColor() );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TOOLHORIZVERT
 */

void gHistogram::OnToolhorizvertUpdate( wxUpdateUIEvent& event )
{
  event.Check( myHistogram->getHorizontal() );
}


/*!
 * wxEVT_MOTION event handler for ID_ZOOMHISTO
 */

void gHistogram::OnMotion( wxMouseEvent& event )
{
  if( !ready )
    return;

  lastPosZoomX = event.GetX();
  lastPosZoomY = event.GetY();

  timerZoom->Start( 20, true );

  if( zoomDragging )
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
    dc.SetPen( *wxWHITE_PEN );

    long beginX = zoomPointBegin.x > event.GetX() ? event.GetX() : zoomPointBegin.x;
    long endX = zoomPointBegin.x < event.GetX() ? event.GetX() : zoomPointBegin.x;
    long beginY, endY;
    if ( event.ControlDown() )
    {
      beginY = zoomPointBegin.y > event.GetY() ? event.GetY() : zoomPointBegin.y;
      endY = zoomPointBegin.y < event.GetY() ? event.GetY() : zoomPointBegin.y;
    }
    else
    {
      beginY = 0;
      endY = drawImage.GetHeight() - 1;
    }
    if( beginX < 5 ) beginX = 0;
    if( endX > zoomHisto->GetSize().GetWidth() - 5 ) endX = zoomHisto->GetSize().GetWidth() - 1;
    wxCoord width = endX - beginX;
    wxCoord height = endY - beginY;
    
    dc.DrawBitmap( zoomImage, 0, 0, false );
    dc.DrawRectangle( beginX, beginY, width, height );

    zoomHisto->Refresh();
  }
}


/*!
 * wxEVT_SIZE event handler for ID_GHISTOGRAM
 */

void gHistogram::OnSize( wxSizeEvent& event )
{
  if( ready && myHistogram->getZoom() )
  {
    wxString winTitle = GetTitle();
    SetTitle( _("(Working...) ") + winTitle );
    Update();
    
    fillZoom();
    
    SetTitle( winTitle );
  }
  event.Skip();
}


/*!
 * wxEVT_CONTEXT_MENU event handler for ID_ZOOMHISTO
 */

void gHistogram::OnZoomContextMenu( wxContextMenuEvent& event )
{
  rightDownManager();
}

void gHistogram::OnTimerZoom( wxTimerEvent& event )
{
  wxString text;
  vector<THistogramColumn> noVoidColumns;

  THistogramColumn column = myHistogram->getHorizontal() ? floor( lastPosZoomX / zoomCellWidth ) :
                                                           floor( lastPosZoomY / zoomCellHeight );
  TObjectOrder row = myHistogram->getHorizontal() ? floor( lastPosZoomY / zoomCellHeight ) :
                                                    floor( lastPosZoomX / zoomCellWidth  ) ;

  if( myHistogram->getHideColumns() )
  {
    columnSelection.getSelected( noVoidColumns );
    if( column > noVoidColumns.size() )
      column = noVoidColumns.size();
  }
  else
  {
    if( column > myHistogram->getNumColumns() )
      column = myHistogram->getNumColumns();
  }
    
  if( row > myHistogram->getNumRows() )
    row = myHistogram->getNumRows();
    
  if( row > 0 )
    text << wxString::FromAscii( myHistogram->getRowLabel( selectedRows[ row - 1 ] ).c_str() )
         << _( "  " );

  if( column > 0 )
  {
    if( myHistogram->getHideColumns() )
      column = noVoidColumns[ column - 1 ] + 1;
    text << wxString::FromAscii( myHistogram->getColumnLabel( column - 1 ).c_str() )
         << _( "  " );
  }
  
  if( row > 0 && column > 0 )
  {
    TSemanticValue value = getZoomSemanticValue( column - 1, row - 1 );
    string tmpLabel;
    if ( !myHistogram->getCodeColor() )
    {
      tmpLabel =  LabelConstructor::histoCellLabel( myHistogram, value,  myHistogram->getShowUnits() );
    }
    else
    {
      tmpLabel =  LabelConstructor::semanticLabel( myHistogram->getDataWindow(),
                                                   value,
                                                   true,
                                                   ParaverConfig::getInstance()->getHistogramPrecision() );
    }

    text << _( "= " ) << wxString::FromAscii( tmpLabel.c_str() );
  }
  
  histoStatus->SetStatusText( text );
}

TSemanticValue gHistogram::getZoomSemanticValue( THistogramColumn column, TObjectOrder row ) const
{
  THistogramColumn plane;
  TSemanticValue value = 0.0;
  PRV_UINT16 idStat;
  
  myHistogram->getIdStat( myHistogram->getCurrentStat(), idStat );
  
  if( myHistogram->itsCommunicationStat( myHistogram->getCurrentStat() ) )
  {
    plane = myHistogram->getCommSelectedPlane();
    if( myHistogram->planeCommWithValues( plane ) )
    {
      myHistogram->setCommFirstCell( column, plane );
      while( !myHistogram->endCommCell( column, plane ) &&
             myHistogram->getCommCurrentRow( column, plane ) < row )
        myHistogram->setCommNextCell( column, plane );
      
      if( !myHistogram->endCommCell( column, plane ) &&
          myHistogram->getCommCurrentRow( column, plane ) == row )
        value = myHistogram->getCommCurrentValue( column, idStat, plane );
    }
  }
  else
  {
    plane = myHistogram->getSelectedPlane();
    if( myHistogram->planeWithValues( plane ) )
    {
      myHistogram->setFirstCell( column, plane );
      while( !myHistogram->endCell( column, plane ) &&
             myHistogram->getCurrentRow( column, plane ) < row )
        myHistogram->setNextCell( column, plane );
      
      if( !myHistogram->endCell( column, plane ) &&
          myHistogram->getCurrentRow( column, plane ) == row )
        value = myHistogram->getCurrentValue( column, idStat, plane );
    }
  }
  
  return value;
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_OPEN_CONTROL_WINDOW
 */

void gHistogram::OnToolOpenFilteredControlWindowClick( wxCommandEvent& event )
{
  openControlActivated = true;
  zoomHisto->SetFocus();
  zoomHisto->SetCursor( *wxCROSS_CURSOR );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TOOL_OPEN_CONTROL_WINDOW
 */

void gHistogram::OnToolOpenFilteredControlWindowUpdate( wxUpdateUIEvent& event )
{
  event.Enable( myHistogram->getZoom() );
}


/*!
 * wxEVT_LEFT_DOWN event handler for ID_ZOOMHISTO
 */

void gHistogram::OnLeftDown( wxMouseEvent& event )
{
  zoomHisto->SetFocus();
  zoomDragging = true;
  zoomPointBegin = event.GetPosition();
}


// newDelta = -1.0 (default value) --> zoom called from histogram window
// else, zoom called from undo/redo history with its own delta stored, available in newDelta
void gHistogram::zoom( THistogramLimit newColumnBegin,
                       THistogramLimit newColumnEnd,
                       TObjectOrder newObjectBegin,
                       TObjectOrder newObjectEnd,
                       THistogramLimit newDelta )
{
  THistogramLimit min, max, delta;

  if ( newDelta == -1.0 )
  {
    min = GetHistogram()->getControlMin();
    max = GetHistogram()->getControlMax();
    delta = GetHistogram()->getControlDelta();

    max = ( newColumnEnd * delta ) + min;
    min = ( newColumnBegin * delta ) + min;

    if ( ( max - min == 0 ) || ( myHistogram->getControlWindow()->isCodeColorSet() && ( max - min ) <= 10000 ) )
      delta = 1.0;
    else
      delta = ( max - min ) / ParaverConfig::getInstance()->getHistogramNumColumns();
  }
  else
  {
    min = newColumnBegin;
    max = newColumnEnd;
    delta = newDelta;
  }

  GetHistogram()->setControlMin( min );
  GetHistogram()->setControlMax( max );
  GetHistogram()->setControlDelta( delta );
  GetHistogram()->setCompute2DScale( false );
  std::pair<TObjectOrder, TObjectOrder> tmpZoomRow( newObjectBegin, newObjectEnd );
  GetHistogram()->setZoomSecondDimension( tmpZoomRow );

  GetHistogram()->setRecalc( true );
  updateHistogram();
}



/*!
 * wxEVT_LEFT_UP event handler for ID_ZOOMHISTO
 */

void gHistogram::OnLeftUp( wxMouseEvent& event )
{
  if( escapePressed )
  {
    escapePressed = false;
    return;
  }
  
  if( zoomDragging )
  {
    zoomDragging = false;
    zoomPointEnd = event.GetPosition();
    zoomHisto->SetCursor( wxNullCursor );

    if( zoomPointEnd == zoomPointBegin )
      return;

    wxMemoryDC memdc( drawImage );
    memdc.SetBackgroundMode( wxTRANSPARENT );
    memdc.SetBackground( *wxTRANSPARENT_BRUSH );
    memdc.Clear();
    memdc.DrawBitmap( zoomImage, 0, 0, false );
    
    zoomHisto->Refresh();
    
    int xBegin, xEnd, yBegin, yEnd;
    if( zoomPointBegin.x < zoomPointEnd.x )
    {
      xBegin = zoomPointBegin.x;
      xEnd = zoomPointEnd.x;
    }
    else
    {
      xBegin = zoomPointEnd.x;
      xEnd = zoomPointBegin.x;
    }

    if( zoomPointBegin.y < zoomPointEnd.y )
    {
      yBegin = zoomPointBegin.y;
      yEnd = zoomPointEnd.y;
    }
    else
    {
      yBegin = zoomPointEnd.y;
      yEnd = zoomPointBegin.y;
    }
    
    if( xBegin < 5 ) xBegin = 0;
    if( yBegin < 0 ) yBegin = 0;
    if( xEnd > zoomHisto->GetSize().GetWidth() - 5 ) xEnd = zoomHisto->GetSize().GetWidth() - 1;
    if( yEnd > zoomHisto->GetSize().GetHeight() ) yEnd = zoomHisto->GetSize().GetHeight() - 1;
/*
    if ( !event.ControlDown() )
    {
      yBegin = 0;
      yEnd = zoomHisto->GetSize().GetHeight() - 1;
    }
*/
    THistogramColumn columnBegin, columnEnd;
    TObjectOrder objectBegin, objectEnd;
    openControlGetParameters( xBegin, xEnd, yBegin, yEnd,
                              columnBegin, columnEnd, objectBegin, objectEnd, event.ControlDown() );

    if ( openControlActivated )
    {
      openControlWindow( columnBegin, columnEnd, objectBegin, objectEnd );
      openControlActivated = false;
    }
    else
    {
      objectBegin = selectedRows[ objectBegin ];
      objectEnd = selectedRows[ objectEnd ];
      zoom( columnBegin, columnEnd, objectBegin, objectEnd );
      HistogramProxy::TZoomInfo currentZoom1, currentZoom2;
      currentZoom1.begin = myHistogram->getControlMin();
      currentZoom1.end = myHistogram->getControlMax();
      currentZoom2.begin = myHistogram->getControlDelta();
      myHistogram->addZoom( currentZoom1, currentZoom2, objectBegin, objectEnd );
    }
  }
}

void gHistogram::openControlGetParameters( int xBegin, int xEnd, int yBegin, int yEnd,
                                           THistogramColumn& columnBegin, THistogramColumn& columnEnd,
                                           TObjectOrder& objectBegin, TObjectOrder& objectEnd, bool zoomxy )
{
  columnBegin = myHistogram->getHorizontal() ? floor( xBegin / zoomCellWidth ) :
                                               floor( yBegin / zoomCellHeight );
  if( columnBegin > 0 ) --columnBegin;
  else if( columnBegin < 0 ) columnBegin = 0;
  columnEnd = myHistogram->getHorizontal() ? floor( xEnd / zoomCellWidth ) :
                                             floor( yEnd / zoomCellHeight );
  if( myHistogram->getControlDelta() == 1.0 && columnEnd > 0 ) --columnEnd;
  else if( columnEnd < 0 ) columnEnd = 0;

  if( zoomxy )
  {
    objectBegin = myHistogram->getHorizontal() ? floor( yBegin / zoomCellHeight ) :
                                                 floor( xBegin / zoomCellWidth );
    if( objectBegin > 0 ) --objectBegin;
    else if( objectBegin < 0 ) objectBegin = 0;
    objectEnd = myHistogram->getHorizontal() ? floor( yEnd / zoomCellHeight ) :
                                               floor( xEnd / zoomCellWidth );
    if( objectEnd > 0 ) --objectEnd;
    else if( objectEnd < 0 ) objectEnd = 0;
  }
  else
  {
    objectBegin = 0;
    objectEnd = selectedRows.size() - 1;
  }
  
  
  if( myHistogram->getHideColumns() )
  {
    vector<THistogramColumn> noVoidColumns;
    columnSelection.getSelected( noVoidColumns );
    columnBegin = noVoidColumns[ columnBegin ];
    if( columnEnd >= noVoidColumns.size() )
      columnEnd = noVoidColumns[ noVoidColumns.size() - 1 ] + 1;
    else
      columnEnd = noVoidColumns[ columnEnd ];
  }
}

void gHistogram::openControlWindow( THistogramColumn columnBegin, THistogramColumn columnEnd,
                                    TObjectOrder objectBegin, TObjectOrder objectEnd )
{
  bool found;
  gTimeline *tmpControlWindow = getGTimelineFromWindow( getAllTracesTree()->GetRootItem(), 
                                                        myHistogram->getControlWindow(),
                                                        found );
  if( !found )
    throw std::exception();
    
  gTimeline *openWindow = NULL;
  Window *controlCloned = myHistogram->getControlWindow()->clone();
  controlCloned->unsetUsedByHistogram( myHistogram );
  controlCloned->removeFromSync();
  THistogramLimit min = myHistogram->getControlMin();
  THistogramLimit max = myHistogram->getControlMax();
  THistogramLimit delta = myHistogram->getControlDelta();
  TWindowLevel onLevel = TOPCOMPOSE1;
  bool useExtraCompose = false;
  if( controlCloned->getExtraNumPositions( TOPCOMPOSE1 ) > 0 ||
      ( controlCloned->getFirstFreeCompose() == TOPCOMPOSE1 && controlCloned->getLevelFunction( TOPCOMPOSE1 ) != "As Is" ) )
    useExtraCompose = true;
  else
    onLevel = controlCloned->getFirstFreeCompose();
  
  size_t extraLastPos;
  if( useExtraCompose )
  {
    controlCloned->addExtraCompose( TOPCOMPOSE1 );
    extraLastPos = controlCloned->getExtraNumPositions( TOPCOMPOSE1 ) - 1;
  }

  if( columnBegin == columnEnd && delta != 1.0 )
    ++columnEnd;

  if ( ( ( columnEnd * delta ) + min/* + delta*/ ) >= max || delta == 1.0 )
  {
    if( useExtraCompose )
      controlCloned->setExtraLevelFunction( onLevel, extraLastPos, "Select Range" );
    else
      controlCloned->setLevelFunction( onLevel, "Select Range" );
  }
  else
  {
    if( useExtraCompose )
      controlCloned->setExtraLevelFunction( onLevel, extraLastPos, "Select Range [)" );
    else
      controlCloned->setLevelFunction( onLevel, "Select Range [)" );
  }
      
  TParamValue param;
  if( ( ( columnEnd * delta ) + min/* + delta*/ ) >= max )
    param.push_back( max );
  else
    param.push_back( ( columnEnd * delta ) + min/* + delta*/ );
  if( useExtraCompose )
    controlCloned->setExtraFunctionParam( onLevel, extraLastPos, 0, param );
  else
    controlCloned->setFunctionParam( onLevel, 0, param );
    
  param.clear();
  param.push_back( ( columnBegin * delta ) + min );
  if( useExtraCompose )
    controlCloned->setExtraFunctionParam( onLevel, extraLastPos, 1, param );
  else
    controlCloned->setFunctionParam( onLevel, 1, param );
  
  string name = controlCloned->getName();
  name = name.substr( 0, name.find_last_of( '.' ) );
  stringstream tmpStr;
  tmpStr << name << " 2DZoom range [" << ( columnBegin * delta ) + min << ",";
  if ( ( ( columnEnd * delta ) + min/* + delta*/ ) >= max )
    tmpStr << max << "]";
  else if ( delta == 1 )
    tmpStr << ( columnEnd * delta ) + min/* + delta*/ << "]";
  else
    tmpStr << ( columnEnd * delta ) + min/* + delta*/ << ")";
  controlCloned->setName( tmpStr.str() );

  controlCloned->setWindowBeginTime( myHistogram->getBeginTime() );
  controlCloned->setWindowEndTime( myHistogram->getEndTime() );
  controlCloned->addZoom( myHistogram->getBeginTime(), myHistogram->getEndTime(), 
                          selectedRows[ objectBegin ], selectedRows[ objectEnd ] );

  if( myHistogram->getThreeDimensions() )
  {
    Window *extraControlCloned = myHistogram->getExtraControlWindow()->clone();
    extraControlCloned->unsetUsedByHistogram( myHistogram );
    extraControlCloned->removeFromSync();

    THistogramLimit extraMin = myHistogram->getExtraControlMin();
    THistogramLimit extraMax = myHistogram->getExtraControlMax();
    THistogramLimit extraDelta = myHistogram->getExtraControlDelta();

    onLevel = TOPCOMPOSE1;
    useExtraCompose = false;
    if( extraControlCloned->getExtraNumPositions( TOPCOMPOSE1 ) > 0 ||
        ( extraControlCloned->getFirstFreeCompose() == TOPCOMPOSE1 && extraControlCloned->getLevelFunction( TOPCOMPOSE1 ) != "As Is" ) )
      useExtraCompose = true;
    else
      onLevel = extraControlCloned->getFirstFreeCompose();
    
    extraLastPos;
    if( useExtraCompose )
    {
      extraControlCloned->addExtraCompose( TOPCOMPOSE1 );
      extraLastPos = extraControlCloned->getExtraNumPositions( TOPCOMPOSE1 ) - 1;
    }

    PRV_UINT32 plane;
    if( myHistogram->itsCommunicationStat( myHistogram->getCurrentStat() ) )
      plane = myHistogram->getCommSelectedPlane();
    else
      plane = myHistogram->getSelectedPlane();

    if ( ( ( plane * extraDelta ) + extraMin + extraDelta ) >= extraMax || extraDelta == 1 )
    {
      if( useExtraCompose )
        extraControlCloned->setExtraLevelFunction( onLevel, extraLastPos, "Is In Range" );
      else
        extraControlCloned->setLevelFunction( onLevel, "Is In Range" );
    }
    else
    {
      if( useExtraCompose )
        extraControlCloned->setExtraLevelFunction( onLevel, extraLastPos, "Is In Range [)" );
      else
        extraControlCloned->setLevelFunction( onLevel, "Is In Range [)" );
    }
      
    TParamValue param;
    if( extraDelta == 1 )
      param.push_back( ( plane * extraDelta ) + extraMin );
    else if( ( ( plane * extraDelta ) + extraMin + extraDelta ) >= extraMax )
      param.push_back( extraMax );
    else
      param.push_back( ( plane * extraDelta ) + extraMin + extraDelta );
    if( useExtraCompose )
      extraControlCloned->setExtraFunctionParam( onLevel, extraLastPos, 0, param );
    else
      extraControlCloned->setFunctionParam( onLevel, 0, param );

    param.clear();
    param.push_back( ( plane * extraDelta ) + extraMin );
    if( useExtraCompose )
      extraControlCloned->setExtraFunctionParam( onLevel, extraLastPos, 1, param );
    else
      extraControlCloned->setFunctionParam( onLevel, 1, param );

    string name = extraControlCloned->getName();
    name = name.substr( 0, name.find_last_of( '.' ) );
    stringstream tmpStr;
    tmpStr << name << " 3DZoom Mask range [" << ( plane * extraDelta ) + min << ",";
    if ( ( ( plane * extraDelta ) + extraMin + extraDelta ) >= extraMax )
      tmpStr << extraMax << "]";
    else if( extraDelta == 1 )
        tmpStr << ( plane * extraDelta ) + extraMin/* + extraDelta*/ << "]";
    else
        tmpStr << ( plane * extraDelta ) + extraMin + extraDelta << ")";
    extraControlCloned->setName( tmpStr.str() );

    extraControlCloned->setWindowBeginTime( myHistogram->getBeginTime() );
    extraControlCloned->setWindowEndTime( myHistogram->getEndTime() );

    controlCloned->setShowWindow( false );
    extraControlCloned->setShowWindow( false );

    Window *productWin = Window::create( controlCloned->getKernel(), controlCloned, extraControlCloned );

    productWin->setLevelFunction( DERIVED, "product" );
    name = controlCloned->getName();
    tmpStr.str( "" );
    tmpStr << name << " 3DZoom ranges [" << ( columnBegin * delta ) + min << ",";
    if ( ( ( columnEnd * delta ) + min + delta ) >= max )
      tmpStr << max << "]";
    else if( delta == 1 )
      tmpStr << ( columnEnd * delta ) + min/* + delta*/ << "]";
    else
      tmpStr << ( columnEnd * delta ) + min + delta << ")";
    tmpStr << "/[" << ( plane * extraDelta ) + extraMin << ",";
    if ( ( ( plane * extraDelta ) + extraMin + extraDelta ) >= extraMax )
      tmpStr << extraMax << "]";
    else if( extraDelta == 1 )
        tmpStr << ( plane * extraDelta ) + extraMin/* + extraDelta*/ << "]";
    else
        tmpStr << ( plane * extraDelta ) + extraMin + extraDelta << ")";
    productWin->setName( tmpStr.str() );
    
    productWin->setPosX( controlCloned->getPosX() );
    productWin->setPosY( controlCloned->getPosY() );
    productWin->setWidth( controlCloned->getWidth() );
    productWin->setHeight( controlCloned->getHeight() );

    productWin->setWindowBeginTime( myHistogram->getBeginTime() );
    productWin->setWindowEndTime( myHistogram->getEndTime() );
    productWin->addZoom( myHistogram->getBeginTime(), myHistogram->getEndTime(), 
                         selectedRows[ objectBegin ], selectedRows[ objectEnd ] );
    productWin->setMaximumY( controlCloned->getMaximumY() );
    productWin->setMinimumY( controlCloned->getMinimumY() );
    productWin->setTimeUnit( controlCloned->getTimeUnit() );
    productWin->setDrawCommLines( controlCloned->getDrawCommLines() );
    productWin->setDrawFlags( controlCloned->getDrawFlags() );
    if( controlCloned->isCodeColorSet() )
      productWin->setCodeColorMode();
    else if( controlCloned->isGradientColorSet() )
      productWin->setGradientColorMode();
    else if( controlCloned->isNotNullGradientColorSet() )
      productWin->setNotNullGradientColorMode();
    else if( controlCloned->isFunctionLineColorSet() )
      productWin->setFunctionLineColorMode();
      
    productWin->setDrawModeObject( controlCloned->getDrawModeObject() );
    productWin->setDrawModeTime( controlCloned->getDrawModeTime() );
    productWin->getGradientColor().setGradientFunction(
      controlCloned->getGradientColor().getGradientFunction() );
    productWin->setLevel( controlCloned->getLevel() );
    vector<bool> tmpSel;
    for( int level = APPLICATION; level <= THREAD; ++level )
    {
      tmpSel.clear();
      controlCloned->getSelectedRows( (TWindowLevel)level, tmpSel );
      productWin->setSelectedRows( (TWindowLevel)level, tmpSel );
    }
    if( controlCloned->getTrace()->existResourceInfo() )
    {
      for( int level = NODE; level <= CPU; ++level )
      {
        tmpSel.clear();
        controlCloned->getSelectedRows( (TWindowLevel)level, tmpSel );
        productWin->setSelectedRows( (TWindowLevel)level, tmpSel );
      }
    }

    openWindow = tmpControlWindow->clone( productWin, 
                                          parent,
                                          getAllTracesTree()->GetRootItem(),
                                          getSelectedTraceTree( productWin->getTrace() )->GetRootItem(),
                                          false );
  }
  else
  {
    openWindow = tmpControlWindow->clone( controlCloned, 
                                          parent,
                                          getAllTracesTree()->GetRootItem(),
                                          getSelectedTraceTree( controlCloned->getTrace() )->GetRootItem(),
                                          false );
  }
  
  if( openWindow != NULL )
  {
    vector<bool> tmpSelectedRows;
    THistogramColumn iPlane;
    bool commStat = myHistogram->itsCommunicationStat( myHistogram->getCurrentStat() );
    tmpControlWindow->GetMyWindow()->getSelectedRows( tmpControlWindow->GetMyWindow()->getLevel(),
                                                      tmpSelectedRows );

    if( columnBegin == columnEnd || delta == 1.0 )
      ++columnEnd;

    if ( !commStat )
    {
      iPlane = myHistogram->getSelectedPlane();

      for( THistogramColumn iCol = columnBegin; iCol < columnEnd; ++iCol )
      {
        myHistogram->setFirstCell( iCol, iPlane );
        while( !myHistogram->endCell( iCol, iPlane ) && myHistogram->getCurrentRow( iCol, iPlane ) < objectBegin )
          myHistogram->setNextCell( iCol, iPlane );
      }
      
      TObjectOrder maxRow = tmpSelectedRows.size();
      vector< bool > present( maxRow, false );
      for( THistogramColumn iCol = columnBegin; iCol < columnEnd; ++iCol )
      {
        while ( !myHistogram->endCell( iCol, iPlane ) )
        {
          TObjectOrder currentRow = selectedRows[ myHistogram->getCurrentRow( iCol, iPlane ) ];
          present[ currentRow ] = true;
          myHistogram->setNextCell( iCol, iPlane );
        }
      }

      for (TObjectOrder i = 0; i < maxRow; ++i )
        tmpSelectedRows[ i ] = tmpSelectedRows[ i ] && present[ i ];
    }
    else
    {
      iPlane = myHistogram->getCommSelectedPlane();

      for( THistogramColumn iCol = columnBegin; iCol < columnEnd; ++iCol )
      {
        myHistogram->setCommFirstCell( iCol, iPlane );
        while( !myHistogram->endCommCell( iCol, iPlane ) && myHistogram->getCommCurrentRow( iCol, iPlane ) < objectBegin )
          myHistogram->setCommNextCell( iCol, iPlane );
      }

      TObjectOrder maxRow = tmpSelectedRows.size();
      vector< bool > present( maxRow, false );
      for( THistogramColumn iCol = columnBegin; iCol < columnEnd; ++iCol )
      {
        while ( !myHistogram->endCommCell( iCol, iPlane ) )
        {
          TObjectOrder currentRow = selectedRows[ myHistogram->getCommCurrentRow( iCol, iPlane ) ];
          present[ currentRow ] = true;
          myHistogram->setCommNextCell( iCol, iPlane );
        }
      }
      for (TObjectOrder i = 0; i < maxRow; ++i )
        tmpSelectedRows[ i ] = tmpSelectedRows[ i ] && present[ i ];
    }

    openWindow->GetMyWindow()->setSelectedRows( openWindow->GetMyWindow()->getLevel(), tmpSelectedRows );

    openWindow->GetMyWindow()->unsetUsedByHistogram( myHistogram );
    openWindow->GetMyWindow()->setShowWindow( true );
  }
  else
    throw std::exception();
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_OPEN_CONTROL_WINDOW
 */

void gHistogram::OnToolOpenControlWindowClick( wxCommandEvent& event )
{
  myHistogram->getControlWindow()->setRaiseWindow( true );
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_OPEN_DATA_WINDOW
 */

void gHistogram::OnToolOpenDataWindowClick( wxCommandEvent& event )
{
  myHistogram->getDataWindow()->setRaiseWindow( true );
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_OPEN_EXTRA_WINDOW
 */

void gHistogram::OnToolOpenExtraWindowClick( wxCommandEvent& event )
{
  if( myHistogram->getExtraControlWindow() != NULL )
    myHistogram->getExtraControlWindow()->setRaiseWindow( true );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TOOL_OPEN_EXTRA_WINDOW
 */

void gHistogram::OnToolOpenExtraWindowUpdate( wxUpdateUIEvent& event )
{
  event.Enable( myHistogram->getExtraControlWindow() != NULL );
}


/*!
 * wxEVT_UPDATE_UI event handler for wxID_CONTROLWARNING
 */

void gHistogram::OnControlWarningUpdate( wxUpdateUIEvent& event )
{
  event.Show( myHistogram->getControlOutOfLimits() );
}


/*!
 * wxEVT_UPDATE_UI event handler for wxID_3DWARNING
 */

void gHistogram::On3dWarningUpdate( wxUpdateUIEvent& event )
{
  event.Show( myHistogram->getExtraOutOfLimits() );
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL
 */

void gHistogram::OnToolHideColumnsClick( wxCommandEvent& event )
{
  myHistogram->setHideColumns( event.IsChecked() );
  myHistogram->setRedraw( true );
  if( !myHistogram->getZoom() )
    gridHisto->Refresh();
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TOOL
 */

void gHistogram::OnToolHideColumnsUpdate( wxUpdateUIEvent& event )
{
  event.Check( myHistogram->getHideColumns() );
}




/*!
 * wxEVT_GRID_CELL_LEFT_CLICK event handler for ID_GRIDHISTO
 */

void gHistogram::OnCellLeftClick( wxGridEvent& event )
{
  paraverMain::myParaverMain->selectTrace( GetHistogram()->getControlWindow()->getTrace() );
  event.Skip();
}


/*!
 * wxEVT_GRID_LABEL_LEFT_CLICK event handler for ID_GRIDHISTO
 */

void gHistogram::OnLabelLeftClick( wxGridEvent& event )
{
  paraverMain::myParaverMain->selectTrace( GetHistogram()->getControlWindow()->getTrace() );
}


void gHistogram::OnMenuGradientFunction( GradientColor::TGradientFunction function )
{
  myHistogram->getGradientColor().setGradientFunction( function );
  myHistogram->setRedraw( true );
}


// Returns: histogram_plane_with_spaces_underscored@traceName (without extension PRV)
wxString gHistogram::buildFormattedFileName( bool onlySelectedPlane ) const
{
  std::string histoNameNoSpaces = myHistogram->getName();

  if ( onlySelectedPlane )
  {
    histoNameNoSpaces += "_";
    bool isCommStatistic = myHistogram->itsCommunicationStat( myHistogram->getCurrentStat() );
    if ( !isCommStatistic )
      histoNameNoSpaces += myHistogram->getPlaneLabel( myHistogram->getSelectedPlane() );
    else
      histoNameNoSpaces += myHistogram->getPlaneLabel( myHistogram->getCommSelectedPlane() );
  }

  return ( paraverMain::buildFormattedFileName( histoNameNoSpaces, myHistogram->getTrace()->getTraceNameNumbered() ));
}


void gHistogram::saveText( bool onlySelectedPlane )
{
  wxString fileName;
  wxString tmpSuffix;
  wxString defaultDir;
  
  fileName = buildFormattedFileName( onlySelectedPlane );

#ifdef WIN32
  defaultDir = _(".\\");
#else
  defaultDir = _("./");
#endif

  vector< wxString > extensions;
  tmpSuffix = _(".") +
        wxString::FromAscii( LabelConstructor::getDataFileSuffix(
                ParaverConfig::getInstance()->getHistogramSaveTextFormat() ).c_str() );

  // Builds following wildcard: _( "CSV (*.csv)|*.csv|GNUPlot (*.gnuplot)|*.gnuplot" )
  // Also fills extension
  wxString tmpWildcard;
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
                                  wxFD_SAVE|wxFD_CHANGE_DIR,
                                  wxDefaultPosition,
                                  wxDefaultSize,
                                  _( "filedlg" ),
                                  extensions );

  saveDialog.SetFilterIndex( ParaverConfig::getInstance()->getHistogramSaveTextFormat() );

  if ( saveDialog.ShowModal() == wxID_OK )
  {
    Output *output = Output::createOutput( (Output::TOutput)saveDialog.GetFilterIndex() );
    output->setMultipleFiles( false );
    
    // Set up progress controller
    ProgressController *progress = ProgressController::create( paraverMain::myParaverMain->GetLocalKernel() );
    progress->setHandler( progressFunction, this );

    if( paraverMain::dialogProgress == NULL )
      paraverMain::dialogProgress = new wxProgressDialog( wxT("Save Histogram Text"),
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

    output->dumpHistogram( myHistogram, fileName, onlySelectedPlane, myHistogram->getHideColumns(),
                            true, true, false, progress );
                            
    delete output;
    
    // Delete progress controller
    paraverMain::dialogProgress->Show( false );
    delete paraverMain::dialogProgress;
    paraverMain::dialogProgress = NULL;
    delete progress;
  }
}


void gHistogram::saveImage( bool showSaveDialog, wxString whichFileName )
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

    filterIndex = ParaverConfig::getInstance()->getHistogramSaveImageFormat();

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
  
  // Build image to be saved as: title image + timeline image

  // Get title
  wxString longTitle = wxString::FromAscii(
          ( myHistogram->getName() + " @ " +
            myHistogram->getTrace()->getTraceNameNumbered() ).c_str());
  wxString shortTitle = wxString::FromAscii( ( myHistogram->getName() ).c_str() );
  wxString writtenTitle = longTitle;

  // Get colors
  wxColour foregroundColour = *wxWHITE;
  wxColour backgroundColour = *wxBLACK;

  // Get font
  wxFont titleFont = wxFont( 8, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD );

  // Get dimensions
  wxImage img = drawImage.ConvertToImage();
  int histogramWidth = img.GetWidth();
  int histogramHeight = img.GetHeight();

  int titleMargin = 5; // used in 4 sides
  int titleHeigth = titleFont.GetPointSize() + ( 2 * titleMargin ); // up + down margins + text
  int titleWidth = histogramWidth;
  int titleWritableWidth = titleWidth - ( 2 * titleMargin );

  int imageHeigth = titleHeigth + histogramHeight;
  int imageWidth = histogramWidth;

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

  wxMemoryDC histogramDC( drawImage );
  xsrc = 0;
  ysrc = 0;
  xdst = 0;
  ydst = titleHeigth;
  imageDC.Blit( xdst, ydst, histogramWidth, histogramHeight, &histogramDC, xsrc, ysrc );

  // Get extension and save
#if wxMAJOR_VERSION<3
  long imageType;
#else
  wxBitmapType imageType;
#endif
  switch( filterIndex )
  {
    case ParaverConfig::BMP:
      imageType =  wxBITMAP_TYPE_BMP;
      break;
    case ParaverConfig::JPG:
      imageType =  wxBITMAP_TYPE_JPEG;
      break;
    case ParaverConfig::PNG:
      imageType =  wxBITMAP_TYPE_PNG;
      break;
    case ParaverConfig::XPM:
      imageType = wxBITMAP_TYPE_XPM;
      break;
    default:
      imageType =  wxBITMAP_TYPE_PNG;
      break;
  }

  wxImage baseLayer = imageBitmap.ConvertToImage();
  baseLayer.SaveFile( imagePath, imageType );
}


void gHistogram::saveCFG()
{
  vector< Histogram * > histograms;
  vector< Window * > windows;
  
  histograms.push_back( GetHistogram() );

  if ( myHistogram->getControlWindow() != NULL  )
      windows.push_back( myHistogram->getControlWindow() );
  if ( myHistogram->getDataWindow() != NULL  )
      windows.push_back( myHistogram->getDataWindow() );
  


  paraverMain::myParaverMain->SaveConfigurationFile(
          (wxWindow *)this, SaveOptions(), windows, histograms );
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_LABEL_COLORS
 */

void gHistogram::OnToolLabelColorsClick( wxCommandEvent& event )
{
  if ( !myHistogram->itsCommunicationStat( myHistogram->getCurrentStat() ) )
  {
    myHistogram->setFirstRowColored( event.IsChecked() );
    myHistogram->setRedraw( true );
    if( !myHistogram->getZoom() )
      gridHisto->Refresh();
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TOOL_LABEL_COLORS
 */

void gHistogram::OnToolLabelColorsUpdate( wxUpdateUIEvent& event )
{
  event.Check( myHistogram->getFirstRowColored() );
}


/*!
 * wxEVT_KEY_DOWN event handler for ID_ZOOMHISTO
 */

void gHistogram::OnZoomHistoKeyDown( wxKeyEvent& event )
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


  if( zoomDragging && event.GetKeyCode() == WXK_ESCAPE )
  {
    escapePressed = true;
    zoomDragging = false;

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
    dc.DrawBitmap( zoomImage, 0, 0, false );

    zoomHisto->Refresh();
  }
  else if( openControlActivated && event.GetKeyCode() == WXK_ESCAPE )
  {
    escapePressed = true;
    openControlActivated = false;
    zoomDragging = false;
    zoomHisto->SetCursor( wxNullCursor );
  }

  if( !myHistogram->getZoom() )
  {
    int dummyUnitsVirtWidth, unitsVirtHeigth;
    gridHisto->GetVirtualSize( &dummyUnitsVirtWidth, &unitsVirtHeigth );

    int dummyPixelsPerWidthUnit, pixelsPerHeigthUnit;
    gridHisto->GetScrollPixelsPerUnit( &dummyPixelsPerWidthUnit, &pixelsPerHeigthUnit );

    int dummyUnitsWidthStart, unitsHeigthStart;
    gridHisto->GetViewStart( &dummyUnitsWidthStart, &unitsHeigthStart );

    int unitsFirstColoredRow = 0;
    if ( !myHistogram->getFirstRowColored() ) // Count first row
    {
      unitsFirstColoredRow = rint( gridHisto->GetDefaultColLabelSize() * ( 1 / (double)pixelsPerHeigthUnit ) );
    }
    int unitsToScroll = rint( (double)unitsVirtHeigth / (double)pixelsPerHeigthUnit ) - unitsFirstColoredRow;

    int maxScrollUnits = rint( ( gridHisto->GetNumberRows() + 1 ) * gridHisto->GetRowSize(0) * ( 1 / (double)pixelsPerHeigthUnit ) );

    switch( event.GetKeyCode() )
    {
      case WXK_HOME:
        gridHisto->Scroll( 0, -maxScrollUnits );
        gridHisto->Refresh();
        break;

      case WXK_END:
        gridHisto->Scroll( 0, maxScrollUnits );
        gridHisto->Refresh();
        break;

      case WXK_PAGEUP:
        gridHisto->Scroll( 0, unitsHeigthStart - unitsToScroll );
        //gridHisto->MovePageUp();
        gridHisto->Refresh();
        break;

      case WXK_PAGEDOWN:
        gridHisto->Scroll( 0, unitsHeigthStart + unitsToScroll );
        //gridHisto->MovePageDown();
        gridHisto->Refresh();
        break;
    }
  }
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_INCLUSIVE
 */

void gHistogram::OnToolInclusiveClick( wxCommandEvent& event )
{
  myHistogram->setInclusive( event.IsChecked() );
  myHistogram->setRecalc( true );
  if( !myHistogram->getZoom() )
    gridHisto->Refresh();
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TOOL_INCLUSIVE
 */

void gHistogram::OnToolInclusiveUpdate( wxUpdateUIEvent& event )
{
  event.Enable( myHistogram->getInclusiveEnabled() );
  event.Check( myHistogram->getInclusive() );
}

void progressFunctionHistogram( ProgressController *progress, void *callerWindow )
{
  if( gHistogram::dialogProgress != NULL )
  {
    gHistogram::dialogProgress->Refresh();
    gHistogram::dialogProgress->Update();
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
/*  if( ( (gHistogram*)callerWindow )->GetRedrawStopWatch()->Time() >= 750 )
  {
    if( gHistogram::dialogProgress != NULL && !gHistogram::dialogProgress->IsShown() )
    {
      gHistogram::dialogProgress->Show();
      gHistogram::dialogProgress->Raise();
    }
    
    ( (gHistogram*)callerWindow )->GetRedrawStopWatch()->Pause();
  }
  */
  if( gHistogram::dialogProgress != NULL && !gHistogram::dialogProgress->Update( p, newMessage ) )
    progress->setStop( true );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TOOL_ONLY_TOTALS
 */

void gHistogram::OnToolOnlyTotalsUpdate( wxUpdateUIEvent& event )
{
  event.Enable( !myHistogram->getZoom() );
  event.Check( myHistogram->getOnlyTotals() );
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_ONLY_TOTALS
 */

void gHistogram::OnToolOnlyTotalsClick( wxCommandEvent& event )
{
  myHistogram->setOnlyTotals( event.IsChecked() );
  myHistogram->setRedraw( true );
  if( !myHistogram->getZoom() )
    gridHisto->Refresh();
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TOOL_SHORT_LABELS
 */

void gHistogram::OnToolShortLabelsUpdate( wxUpdateUIEvent& event )
{
  event.Enable( !myHistogram->getZoom() );
  event.Check( myHistogram->getShortLabels() );
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_SHORT_LABELS
 */

void gHistogram::OnToolShortLabelsClick( wxCommandEvent& event )
{
  myHistogram->setShortLabels( event.IsChecked() );
  myHistogram->setRedraw( true );
  if( !myHistogram->getZoom() )
    gridHisto->Refresh();
}

