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
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include <wx/clipbrd.h>
#include <sstream>
#include <iostream>
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
#include "caution.xpm"
#include "opencontrol.xpm"
#include "opendata.xpm"
#include "open3d.xpm"
#include "histo_zoom.xpm"
#include "openfiltered.xpm"
#include "histo_color.xpm"
#include "histo_horvert.xpm"
#include "hide_cols.xpm"
#include "semantic_color.xpm"
#include "inclusive.xpm"
////@end XPM images

#ifdef WIN32
inline double rint(double nr)
{
  double f = floor(nr);
  double c = ceil(nr);
  return (((c-nr) >= (nr-f)) ? f :c);
}
#endif

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

  EVT_UPDATE_UI( ID_ZOOMHISTO, gHistogram::OnZoomhistoUpdate )

  EVT_GRID_CELL_LEFT_CLICK( gHistogram::OnCellLeftClick )
  EVT_GRID_CELL_RIGHT_CLICK( gHistogram::OnCellRightClick )
  EVT_GRID_LABEL_LEFT_CLICK( gHistogram::OnLabelLeftClick )
  EVT_GRID_LABEL_RIGHT_CLICK( gHistogram::OnLabelRightClick )
  EVT_GRID_RANGE_SELECT( gHistogram::OnRangeSelect )
  EVT_UPDATE_UI( ID_GRIDHISTO, gHistogram::OnGridhistoUpdate )

  EVT_UPDATE_UI( wxID_CONTROLWARNING, gHistogram::OnControlWarningUpdate )

  EVT_UPDATE_UI( wxID_3DWARNING, gHistogram::On3dWarningUpdate )

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

  EVT_MENU( ID_TOOL_INCLUSIVE, gHistogram::OnToolInclusiveClick )
  EVT_UPDATE_UI( ID_TOOL_INCLUSIVE, gHistogram::OnToolInclusiveUpdate )

////@end gHistogram event table entries
  
  EVT_TIMER( wxID_ANY, gHistogram::OnTimerZoom )
  
END_EVENT_TABLE()


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
  this->parent = parent;

  return true;
}


/*!
 * gHistogram destructor
 */

gHistogram::~gHistogram()
{
////@begin gHistogram destruction
////@end gHistogram destruction

  delete myHistogram;
}


/*!
 * Member initialisation
 */

void gHistogram::Init()
{
////@begin gHistogram member initialisation
  myHistogram = NULL;
  ready = false;
  timerZoom = new wxTimer( this );
  lastPosZoomX = 0;
  lastPosZoomY = 0;
  openControlActivated = false;
  zoomDragging = false;
  escapePressed = false;
  mainSizer = NULL;
  zoomHisto = NULL;
  gridHisto = NULL;
  warningSizer = NULL;
  controlWarning = NULL;
  xtraWarning = NULL;
  histoStatus = NULL;
////@end gHistogram member initialisation
  parent = NULL;
  zoomRow.begin = 0;
  zoomRow.end = 0;
}


/*!
 * Control creation for gHistogram
 */

void gHistogram::CreateControls()
{    
////@begin gHistogram content construction
  gHistogram* itemFrame1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
  itemFrame1->SetSizer(itemBoxSizer2);

  mainSizer = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer2->Add(mainSizer, 1, wxGROW|wxALL, 0);

  zoomHisto = new wxScrolledWindow( itemFrame1, ID_ZOOMHISTO, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxFULL_REPAINT_ON_RESIZE );
  mainSizer->Add(zoomHisto, 1, wxGROW|wxALL, 1);
  zoomHisto->SetScrollbars(1, 1, 0, 0);

  gridHisto = new wxGrid( itemFrame1, ID_GRIDHISTO, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL|wxALWAYS_SHOW_SB );
  gridHisto->SetDefaultColSize(50);
  gridHisto->SetDefaultRowSize(25);
  gridHisto->SetColLabelSize(25);
  gridHisto->SetRowLabelSize(50);
  mainSizer->Add(gridHisto, 1, wxGROW|wxALL, 1);

  warningSizer = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer2->Add(warningSizer, 0, wxGROW|wxALL, 0);

  controlWarning = new wxStaticBitmap( itemFrame1, wxID_CONTROLWARNING, itemFrame1->GetBitmapResource(wxT("caution.xpm")), wxDefaultPosition, itemFrame1->ConvertDialogToPixels(wxSize(8, 7)), 0 );
  if (gHistogram::ShowToolTips())
    controlWarning->SetToolTip(_("Control limits not fitted"));
  warningSizer->Add(controlWarning, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxFIXED_MINSIZE, 5);

  xtraWarning = new wxStaticBitmap( itemFrame1, wxID_3DWARNING, itemFrame1->GetBitmapResource(wxT("caution.xpm")), wxDefaultPosition, itemFrame1->ConvertDialogToPixels(wxSize(8, 7)), 0 );
  if (gHistogram::ShowToolTips())
    xtraWarning->SetToolTip(_("3D limits not fitted"));
  warningSizer->Add(xtraWarning, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxFIXED_MINSIZE, 5);

  wxStaticBitmap* itemStaticBitmap9 = new wxStaticBitmap( itemFrame1, wxID_STATIC, itemFrame1->GetBitmapResource(wxT("caution.xpm")), wxDefaultPosition, itemFrame1->ConvertDialogToPixels(wxSize(9, 9)), 0 );
  itemStaticBitmap9->Show(false);
  warningSizer->Add(itemStaticBitmap9, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxFIXED_MINSIZE, 5);

  warningSizer->Add(20, 21, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxToolBar* itemToolBar11 = CreateToolBar( wxTB_FLAT|wxTB_HORIZONTAL, ID_AUITOOLBAR1 );
  wxBitmap itemtool12Bitmap(itemFrame1->GetBitmapResource(wxT("opencontrol.xpm")));
  wxBitmap itemtool12BitmapDisabled;
  itemToolBar11->AddTool(ID_TOOL_OPEN_CONTROL_WINDOW, _("Open Control Window"), itemtool12Bitmap, itemtool12BitmapDisabled, wxITEM_NORMAL, _("Open Control Window"), wxEmptyString);
  wxBitmap itemtool13Bitmap(itemFrame1->GetBitmapResource(wxT("opendata.xpm")));
  wxBitmap itemtool13BitmapDisabled;
  itemToolBar11->AddTool(ID_TOOL_OPEN_DATA_WINDOW, _("Open Data Window"), itemtool13Bitmap, itemtool13BitmapDisabled, wxITEM_NORMAL, _("Open Data Window"), wxEmptyString);
  wxBitmap itemtool14Bitmap(itemFrame1->GetBitmapResource(wxT("open3d.xpm")));
  wxBitmap itemtool14BitmapDisabled;
  itemToolBar11->AddTool(ID_TOOL_OPEN_EXTRA_WINDOW, _("Open 3rd Window"), itemtool14Bitmap, itemtool14BitmapDisabled, wxITEM_NORMAL, _("Open 3rd Window"), wxEmptyString);
  itemToolBar11->AddSeparator();
  wxBitmap itemtool16Bitmap(itemFrame1->GetBitmapResource(wxT("histo_zoom.xpm")));
  wxBitmap itemtool16BitmapDisabled;
  itemToolBar11->AddTool(ID_TOOLZOOM, _("Zoom"), itemtool16Bitmap, itemtool16BitmapDisabled, wxITEM_CHECK, _("Histogram zoom"), wxEmptyString);
  wxBitmap itemtool17Bitmap(itemFrame1->GetBitmapResource(wxT("openfiltered.xpm")));
  wxBitmap itemtool17BitmapDisabled;
  itemToolBar11->AddTool(ID_TOOL_OPEN_FILTERED_CONTROL_WINDOW, _("Open Filtered Control Window"), itemtool17Bitmap, itemtool17BitmapDisabled, wxITEM_NORMAL, _("Open Filtered Control Window"), wxEmptyString);
  itemToolBar11->AddSeparator();
  wxBitmap itemtool19Bitmap(itemFrame1->GetBitmapResource(wxT("histo_color.xpm")));
  wxBitmap itemtool19BitmapDisabled;
  itemToolBar11->AddTool(ID_TOOLGRADIENT, _("Gradient"), itemtool19Bitmap, itemtool19BitmapDisabled, wxITEM_CHECK, _("View gradient colors"), wxEmptyString);
  wxBitmap itemtool20Bitmap(itemFrame1->GetBitmapResource(wxT("histo_horvert.xpm")));
  wxBitmap itemtool20BitmapDisabled;
  itemToolBar11->AddTool(ID_TOOLHORIZVERT, _("Horizontal/Vertical"), itemtool20Bitmap, itemtool20BitmapDisabled, wxITEM_CHECK, _("Horizontal/Vertical"), wxEmptyString);
  wxBitmap itemtool21Bitmap(itemFrame1->GetBitmapResource(wxT("hide_cols.xpm")));
  wxBitmap itemtool21BitmapDisabled;
  itemToolBar11->AddTool(ID_TOOL_HIDE_COLUMNS, _("Hide empty columns"), itemtool21Bitmap, itemtool21BitmapDisabled, wxITEM_CHECK, _("Hide empty columns"), wxEmptyString);
  wxBitmap itemtool22Bitmap(itemFrame1->GetBitmapResource(wxT("semantic_color.xpm")));
  wxBitmap itemtool22BitmapDisabled;
  itemToolBar11->AddTool(ID_TOOL_LABEL_COLORS, _("Label colors"), itemtool22Bitmap, itemtool22BitmapDisabled, wxITEM_CHECK, _("Label colors"), wxEmptyString);
  wxBitmap itemtool23Bitmap(itemFrame1->GetBitmapResource(wxT("inclusive.xpm")));
  wxBitmap itemtool23BitmapDisabled;
  itemToolBar11->AddTool(ID_TOOL_INCLUSIVE, _("Inclusive/Exclusive"), itemtool23Bitmap, itemtool23BitmapDisabled, wxITEM_CHECK, _("Inclusive/Exclusive"), wxEmptyString);
  itemToolBar11->Realize();
  itemFrame1->SetToolBar(itemToolBar11);

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
  gridHisto->CreateGrid( 0, 0 );
  gridHisto->EnableEditing( false );
  gridHisto->SetDefaultCellAlignment( wxALIGN_RIGHT, wxALIGN_CENTRE );
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

  TObjectOrder beginRow, endRow;
  selectedRows.clear();
  if( myHistogram->isZoomEmpty() )
  {
    beginRow = myHistogram->getControlWindow()->getZoomSecondDimension().first;
    endRow =  myHistogram->getControlWindow()->getZoomSecondDimension().second;
  }
  else
  {
    //beginRow = myHistogram->getZoomSecondDimension().first;
    beginRow = zoomRow.begin;
    //endRow =  myHistogram->getZoomSecondDimension().second;
    endRow = zoomRow.end;
  }
  myHistogram->getControlWindow()->getSelectedRows( myHistogram->getControlWindow()->getLevel(),
                                                    selectedRows, beginRow, endRow, true );

  myHistogram->execute( myHistogram->getBeginTime(), myHistogram->getEndTime(), selectedRows );

  if( myHistogram->getZoom() )
    fillZoom();
  else
    fillGrid();

  ready = true;

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
    zoomRow.begin = beginRow;
    zoomRow.end = endRow;
  }
}

void gHistogram::fillGrid()
{
  int rowLabelWidth = 0;
  wxFont labelFont = gridHisto->GetLabelFont();
  bool commStat = myHistogram->itsCommunicationStat( myHistogram->getCurrentStat() );
  PRV_UINT16 idStat;
  THistogramColumn curPlane;
  THistogramColumn numCols, numDrawCols;
  TObjectOrder numRows, numDrawRows;
  bool horizontal = myHistogram->getHorizontal();
  bool firstRowColored = myHistogram->getFirstRowColored();
  wxFont cellFontBold;
  Window *controlWindow = myHistogram->getControlWindow();
  
  zoomHisto->Show( false );
  gridHisto->Show( true );
  mainSizer->Layout();
  
  if( firstRowColored )
  {
    cellFontBold = gridHisto->GetDefaultCellFont();
    cellFontBold.SetWeight( wxFONTWEIGHT_BOLD );
  }
  
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
  numRows = myHistogram->getNumRows();

  if( horizontal )
  {
    numDrawCols = numCols;
    if( firstRowColored && !commStat )
      numDrawRows = numRows + 1;
    else
      numDrawRows = numRows;
  }
  else
  {
    if( firstRowColored && !commStat )
      numDrawCols = numRows + 1;
    else
      numDrawCols = numRows;
    numDrawRows = numCols;
  }

  gridHisto->SetColMinimalAcceptableWidth( 0 );
  gridHisto->BeginBatch();
  if( (THistogramColumn)gridHisto->GetNumberCols() != numDrawCols )
  {
    if( gridHisto->GetNumberCols() > 0 )
      gridHisto->DeleteCols( 0, gridHisto->GetNumberCols() );
    gridHisto->AppendCols( numDrawCols );
  }
  if( gridHisto->GetNumberRows() != numDrawRows + NUMTOTALS + 1 )
  {
    if( gridHisto->GetNumberRows() > 0 )
      gridHisto->DeleteRows( 0, gridHisto->GetNumberRows() );
    gridHisto->AppendRows( numDrawRows + NUMTOTALS + 1 );
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

  for( THistogramColumn iCol = 0; iCol < numCols; ++iCol )
  {
    if( commStat )
    {
      gridHisto->SetColLabelValue( iCol, wxString::FromAscii( myHistogram->getRowLabel( iCol ).c_str() ) );
      myHistogram->setCommFirstCell( iCol, curPlane );
    }
    else
    {
      if( horizontal )
      {
        if( firstRowColored )
        {
          gridHisto->SetCellAlignment( 0, iCol, wxALIGN_CENTRE, wxALIGN_CENTRE );
          gridHisto->SetCellFont( 0, iCol, cellFontBold );
          gridHisto->SetCellValue( 0, iCol, wxString::FromAscii( myHistogram->getColumnLabel( iCol ).c_str() ) );
          gridHisto->SetRowLabelValue( 0, wxT( "" ) );

          TSemanticValue tmpValue = ( iCol / myHistogram->getControlDelta() ) +
                                    myHistogram->getControlMin();
          rgb tmpCol;
          if( controlWindow->IsCodeColorSet() )
            tmpCol = controlWindow->getCodeColor().calcColor( tmpValue,
                                                              controlWindow->getMinimumY(),
                                                              controlWindow->getMaximumY() );
          else
            tmpCol = controlWindow->getGradientColor().calcColor( tmpValue,
                                                                  controlWindow->getMinimumY(),
                                                                  controlWindow->getMaximumY() );
          gridHisto->SetCellBackgroundColour( 0, iCol, 
                                              wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) );
          wxColour BackColour = wxColour( tmpCol.red, tmpCol.green, tmpCol.blue );
          unsigned int BackColour_luminance = (BackColour.Red() * 30)/100 +
                                              (BackColour.Green() * 59)/100 +
                                              (BackColour.Blue() * 11) / 100;
          if (BackColour_luminance >= 128)
            gridHisto->SetCellTextColour( 0, iCol, *wxBLACK );
          else
            gridHisto->SetCellTextColour( 0, iCol, *wxWHITE );
        }
        else
          gridHisto->SetColLabelValue( iCol, wxString::FromAscii( myHistogram->getColumnLabel( iCol ).c_str() ) );
      }
      else
      {
        int w, h;
      
        gridHisto->GetTextExtent( wxString::FromAscii( myHistogram->getColumnLabel( iCol ).c_str() ), &w, &h, NULL, NULL, &labelFont );
        if( rowLabelWidth == 0 || rowLabelWidth < w )
          rowLabelWidth = w;
        if( firstRowColored )
        {
          gridHisto->SetCellAlignment( iCol, 0, wxALIGN_CENTRE, wxALIGN_CENTRE );
          gridHisto->SetCellFont( iCol, 0, cellFontBold );
          gridHisto->SetCellValue( iCol, 0, wxString::FromAscii( myHistogram->getColumnLabel( iCol ).c_str() ) );
          gridHisto->SetColLabelValue( 0, wxT( "" ) );

          TSemanticValue tmpValue = ( iCol / myHistogram->getControlDelta() ) +
                                    myHistogram->getControlMin();
          rgb tmpCol;
          if( controlWindow->IsCodeColorSet() )
            tmpCol = controlWindow->getCodeColor().calcColor( tmpValue,
                                                              controlWindow->getMinimumY(),
                                                              controlWindow->getMaximumY() );
          else
            tmpCol = controlWindow->getGradientColor().calcColor( tmpValue,
                                                                  controlWindow->getMinimumY(),
                                                                  controlWindow->getMaximumY() );
          gridHisto->SetCellBackgroundColour( iCol, 0,
                                              wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) );
          wxColour BackColour = wxColour( tmpCol.red, tmpCol.green, tmpCol.blue );
          unsigned int BackColour_luminance = (BackColour.Red() * 30)/100 +
                                              (BackColour.Green() * 59)/100 +
                                              (BackColour.Blue() * 11) / 100;
          if (BackColour_luminance >= 128)
            gridHisto->SetCellTextColour( iCol, 0, *wxBLACK );
          else
            gridHisto->SetCellTextColour( iCol, 0, *wxWHITE );
        }
        else
          gridHisto->SetRowLabelValue( iCol, wxString::FromAscii( myHistogram->getColumnLabel( iCol ).c_str() ) );
      }
      myHistogram->setFirstCell( iCol, curPlane );
    }

    for( TObjectOrder iLoopRow = 0; iLoopRow < numRows; iLoopRow++ )
    {
      TObjectOrder iRow = iLoopRow;
      if( firstRowColored && !commStat )
        ++iRow;

      if( horizontal )
      {
        if( iCol == 0 )
        {
          int w, h;
      
          gridHisto->GetTextExtent( wxString::FromAscii( myHistogram->getRowLabel( selectedRows[ iLoopRow ] ).c_str() ),
                                    &w, &h, NULL, NULL, &labelFont );
          if( rowLabelWidth == 0 || rowLabelWidth < w )
            rowLabelWidth = w;
          gridHisto->SetRowLabelValue( iRow, wxString::FromAscii( myHistogram->getRowLabel( selectedRows[ iLoopRow ] ).c_str() ) );
        }
      }
      else
      {
        gridHisto->SetColLabelValue( iRow, wxString::FromAscii( myHistogram->getRowLabel( selectedRows[ iLoopRow ] ).c_str() ) );
      }
      
      THistogramColumn iDrawCol;
      TObjectOrder iDrawRow;
      if( horizontal )
      {
        iDrawCol = iCol;
        iDrawRow = iRow;
      }
      else
      {
        iDrawCol = iRow;
        iDrawRow = iCol;
      }
      
      if( ( commStat && myHistogram->endCommCell( iCol, curPlane ) ) ||
          ( !commStat && myHistogram->endCell( iCol, curPlane ) ) )
      {
        gridHisto->SetCellValue( iDrawRow, iDrawCol, wxString::FromAscii( "-" ) );
        gridHisto->SetCellBackgroundColour( iDrawRow, iDrawCol, *wxWHITE );
      }
      else
      {
        if( commStat )
        {
          if( myHistogram->getCommCurrentRow( iCol, curPlane ) == iRow )
          {
            string tmpStr;
            tmpStr = LabelConstructor::histoCellLabel( myHistogram,
              myHistogram->getCommCurrentValue( iCol, idStat, curPlane ), true );
            gridHisto->SetCellValue( iDrawRow, iDrawCol, wxString::FromAscii( tmpStr.c_str() ) );
            if( myHistogram->getShowColor() )
            {
              rgb tmpCol = myHistogram->calcGradientColor( 
                myHistogram->getCommCurrentValue( iCol, idStat, curPlane ) );
              gridHisto->SetCellBackgroundColour( iDrawRow, iDrawCol, 
                                                  wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) );
            }
            else gridHisto->SetCellBackgroundColour( iDrawRow, iDrawCol, *wxWHITE );
            myHistogram->setCommNextCell( iCol, curPlane );
          }
          else
          {
            gridHisto->SetCellValue( iDrawRow, iDrawCol, wxString::FromAscii( "-" ) );
            gridHisto->SetCellBackgroundColour( iDrawRow, iDrawCol, *wxWHITE );
          }
        }
        else
        {
          if( myHistogram->getCurrentRow( iCol, curPlane ) == iLoopRow )
          {
            string tmpStr;
            tmpStr = LabelConstructor::histoCellLabel( myHistogram,
              myHistogram->getCurrentValue( iCol, idStat, curPlane ), true );
            gridHisto->SetCellValue( iDrawRow, iDrawCol, wxString::FromAscii( tmpStr.c_str() ) );
            if( myHistogram->getShowColor() )
            {
              rgb tmpCol = myHistogram->calcGradientColor( 
                myHistogram->getCurrentValue( iCol, idStat, curPlane ) );
              gridHisto->SetCellBackgroundColour( iDrawRow, iDrawCol, 
                                                  wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) );
              wxColour BackColour = wxColour( tmpCol.red, tmpCol.green, tmpCol.blue );
              unsigned int BackColour_luminance = (BackColour.Red() * 30)/100 +
                                                  (BackColour.Green() * 59)/100 +
                                                  (BackColour.Blue() * 11) / 100;
              if (BackColour_luminance >= 128)
                gridHisto->SetCellTextColour( iDrawRow, iDrawCol, *wxBLACK );
              else
                gridHisto->SetCellTextColour( iDrawRow, iDrawCol, *wxWHITE );
            }
            else
            {
              gridHisto->SetCellBackgroundColour( iDrawRow, iDrawCol, *wxWHITE );
              gridHisto->SetCellTextColour( iDrawRow, iDrawCol, *wxBLACK );
            }
            myHistogram->setNextCell( iCol, curPlane );
          }
          else
          {
            gridHisto->SetCellValue( iDrawRow, iDrawCol, wxString::FromAscii( "-" ) );
            gridHisto->SetCellBackgroundColour( iDrawRow, iDrawCol, *wxWHITE );
          }
        }
      }
    }

    gridHisto->SetRowLabelValue( numDrawRows, _( "" ) );
  }

  fillTotals( rowLabelWidth, numDrawRows + 1, curPlane, idStat );

  if( !firstRowColored || horizontal || commStat )
    gridHisto->SetRowLabelSize( rowLabelWidth + 5 );
  gridHisto->AutoSizeColumns();
  gridHisto->AutoSizeRows();
  gridHisto->EndBatch();
}

void gHistogram::fillZoom()
{
  bool commStat = myHistogram->itsCommunicationStat( myHistogram->getCurrentStat() );
  PRV_UINT16 idStat;
  THistogramColumn curPlane;
  THistogramColumn numCols, numDrawCols;
  TObjectOrder numDrawRows;
  bool horizontal = myHistogram->getHorizontal();
  double& cellWidth = zoomCellWidth;
  double& cellHeight = zoomCellHeight;
  vector<THistogramColumn> noVoidColumns;
  vector<bool> selectedColumns;
  
  gridHisto->Show( false );
  zoomHisto->Show( true );
  mainSizer->Layout();
  
  ready = false;
  zoomImage.Create( zoomHisto->GetSize().GetWidth(), zoomHisto->GetSize().GetHeight() );
  drawImage.Create( zoomHisto->GetSize().GetWidth(), zoomHisto->GetSize().GetHeight() );
  wxMemoryDC bufferDraw( zoomImage );
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
      numDrawCols = myHistogram->getNumRows();
      numDrawRows = numCols;
    }
    selectedColumns.insert( selectedColumns.begin(), numCols, true );
  }
  
  cellWidth = (double)( zoomHisto->GetSize().GetWidth() ) / (double)( numDrawCols + 1 );
  cellHeight = (double)( zoomHisto->GetSize().GetHeight() ) / (double)( numDrawRows + 1 );

  THistogramColumn tmpNumCols = numCols;
  if( myHistogram->getHideColumns() )
    tmpNumCols = noVoidColumns.size();

  bufferDraw.SetBrush( *wxGREY_BRUSH );
  bufferDraw.SetPen( *wxTRANSPARENT_PEN );
  bufferDraw.DrawRectangle( 0, 0, bufferDraw.GetSize().GetWidth(), cellHeight );
  bufferDraw.DrawRectangle( 0, 0, cellWidth, bufferDraw.GetSize().GetHeight() );

  for( THistogramColumn iCol = 0; iCol < tmpNumCols; iCol++ )
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
      while( ( endCol + 1 ) <= tmpNumCols 
             && ( endCol + 2 ) * cellWidth == ( beginCol + 1 ) * cellWidth )
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
      while( ( endCol + 1 ) <= tmpNumCols 
             && ( endCol + 2 ) * cellHeight == ( beginCol + 1 ) * cellHeight )
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
     bufferDraw.DrawLine( 0, ( iRow + 1 ) * cellHeight, bufferDraw.GetSize().GetWidth(), ( iRow + 1 ) * cellHeight );
  }
  if( cellWidth > 5.0 )
  {
    for( TObjectOrder iCol = 0; iCol < numDrawCols; ++iCol )
     bufferDraw.DrawLine( ( iCol + 1 ) * cellWidth, 0, ( iCol + 1 ) * cellWidth, bufferDraw.GetSize().GetHeight() );
  }
  
  bufferDraw.SelectObject( wxNullBitmap );
  bufferDraw.SelectObject( drawImage );
  bufferDraw.DrawBitmap( zoomImage, 0, 0, false );
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
  double& cellWidth = zoomCellWidth;
  double& cellHeight = zoomCellHeight;
  PRV_UINT16 idStat;
  THistogramColumn curPlane;
  vector<TSemanticValue> valuesColumns;
  vector<TSemanticValue> valuesObjects;

  if( !myHistogram->getIdStat( myHistogram->getCurrentStat(), idStat ) )
    throw( std::exception() );

  if( commStat )
    curPlane = myHistogram->getCommSelectedPlane();
  else
    curPlane = myHistogram->getSelectedPlane();

  for( TObjectOrder iRow = 0; iRow < numRows; iRow++ )
  {
    if( iRow == 0 || 
        ( horizontal && rint( iRow * cellHeight ) == rint( ( iRow + 1 ) * cellHeight ) ) ||
        ( !horizontal && rint( iRow * cellWidth ) == rint( ( iRow + 1 ) * cellWidth ) )
      )
      valuesObjects.clear();
      
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
      TSemanticValue tmpValue = ( tmpBeginCol / myHistogram->getControlDelta() ) +
                                myHistogram->getControlMin();
      if( myHistogram->getControlWindow()->IsCodeColorSet() )
        tmpCol = controlWindow->getCodeColor().calcColor( tmpValue,
                                                          controlWindow->getMinimumY(),
                                                          controlWindow->getMaximumY() );
      else
        tmpCol = controlWindow->getGradientColor().calcColor( tmpValue,
                                                              controlWindow->getMinimumY(),
                                                              controlWindow->getMaximumY() );
      bufferDraw.SetBrush( wxBrush( wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) ) );
      bufferDraw.DrawRectangle( rint( ( firstDrawCol ) * cellWidth ), rint( ( firstDrawRow ) * cellHeight ),
                                cellWidth < 1.0 ? 1 : rint( cellWidth ),
                                cellHeight < 1.0 ? 1 : rint( cellHeight ) );
      firstRowColored = false;
    }
      
    if( valuesObjects.begin() != valuesObjects.end() &&
        ( iRow == numRows || 
          ( horizontal && rint( ( iRow + 1 ) * cellHeight ) != rint( ( iRow + 2 ) * cellHeight ) ) ||
          ( !horizontal && rint( ( iRow + 1 ) * cellWidth ) != rint( ( iRow + 2 ) * cellWidth ) )
        )
      )
    {
      rgb tmpCol = myHistogram->calcGradientColor( 
                     DrawMode::selectValue( valuesObjects, myHistogram->getDrawModeObjects() ) );
      bufferDraw.SetBrush( wxBrush( wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) ) );
      bufferDraw.DrawRectangle( rint( ( iDrawCol + 1 ) * cellWidth ), rint( ( iDrawRow + 1 ) * cellHeight ),
                                cellWidth < 1.0 ? 1 : rint( cellWidth ),
                                cellHeight < 1.0 ? 1 : rint( cellHeight ) );

      valuesObjects.clear();
    }
  }
}

void gHistogram::fillTotals( int& rowLabelWidth, TObjectOrder beginRow, THistogramColumn curPlane, PRV_UINT16 idStat )
{
  THistogramColumn numDrawCols;
  wxFont labelFont = gridHisto->GetLabelFont();
  bool commStat = myHistogram->itsCommunicationStat( myHistogram->getCurrentStat() );
  bool horizontal = myHistogram->getHorizontal();
  bool firstRowColored = myHistogram->getFirstRowColored();
  HistogramTotals *histoTotals = myHistogram->getTotals( myHistogram->getCurrentStat() );
  wxFont cellFontBold;
  
  if( firstRowColored )
  {
    cellFontBold = gridHisto->GetDefaultCellFont();
    cellFontBold.SetWeight( wxFONTWEIGHT_BOLD );
  }
  
  if( horizontal )
    numDrawCols = myHistogram->getNumColumns( myHistogram->getCurrentStat() );
  else
    numDrawCols = myHistogram->getNumRows();

  for( THistogramColumn iCol = 0; iCol < numDrawCols; ++iCol )
  {
    vector<TSemanticValue> totals;
    histoTotals->getAll( totals, idStat, iCol, curPlane );

    for( int i = 0; i < NUMTOTALS; ++i )
    {
      int w,h;

      gridHisto->GetTextExtent( wxString::FromAscii( LabelConstructor::histoTotalLabel( (THistoTotals) i ).c_str() ), 
                                &w, &h, NULL, NULL, &labelFont );
      if( rowLabelWidth == 0 || rowLabelWidth < w )
        rowLabelWidth = w;
      if( firstRowColored && !horizontal && !commStat )
      {
        gridHisto->SetCellAlignment( beginRow + i, 0, wxALIGN_CENTRE, wxALIGN_CENTRE );
        gridHisto->SetCellFont( beginRow + i, 0, cellFontBold );
        gridHisto->SetCellValue( beginRow + i, 0, 
          wxString::FromAscii( LabelConstructor::histoTotalLabel( (THistoTotals) i ).c_str() ) );
        //gridHisto->SetCellBackgroundColour( beginRow + i, 0, *wxLIGHT_GREY );
      }
      else
        gridHisto->SetRowLabelValue( beginRow + i, 
          wxString::FromAscii( LabelConstructor::histoTotalLabel( (THistoTotals) i ).c_str() ) );

      THistogramColumn realCol = iCol;
      if( firstRowColored && !horizontal && !commStat )
        ++realCol;
      if( totals[ 0 ] > 0.0 )
      {
        string tmpStr;
        if( i == AVGDIVMAX )
          tmpStr = LabelConstructor::histoCellLabel( myHistogram, totals[ i ], false );
        else
          tmpStr = LabelConstructor::histoCellLabel( myHistogram, totals[ i ], true );
        gridHisto->SetCellValue( beginRow + i, realCol, wxString::FromAscii( tmpStr.c_str() ) );
      }
      else
      {
        gridHisto->SetCellValue( beginRow + i, realCol, wxString::FromAscii( "-" ) );
      }
    }
  }
  
  if( myHistogram->getHideColumns() )
  {
    vector<wxString> labels;
    
    if( myHistogram->itsCommunicationStat( myHistogram->getCurrentStat() ) )
      histoTotals = myHistogram->getCommColumnTotals();
    else
      histoTotals = myHistogram->getColumnTotals();

    numDrawCols = myHistogram->getNumColumns( myHistogram->getCurrentStat() );

    for( THistogramColumn iCol = 0; iCol < numDrawCols; ++iCol )
    {
      vector<TSemanticValue> totals;
      histoTotals->getAll( totals, idStat, iCol, curPlane );

      if( horizontal )
      {
        if( totals[ 0 ] != 0 )
          labels.push_back( gridHisto->GetColLabelValue( iCol ) );
      }
      else
      {
        if( totals[ 0 ] != 0 )
          labels.push_back( gridHisto->GetRowLabelValue( iCol ) );
      }
    }
    
    THistogramColumn iDrawCol = 0;
    for( THistogramColumn iCol = 0; iCol < numDrawCols; ++iCol )
    {
      vector<TSemanticValue> totals;
      histoTotals->getAll( totals, idStat, iCol, curPlane );
    
      if( totals[ 0 ] == 0 )
      {
        if( horizontal )
          gridHisto->DeleteCols( iDrawCol );
        else
          gridHisto->DeleteRows( iDrawCol );
      }
      else
        ++iDrawCol;
    }

    for( THistogramColumn iCol = 0; iCol < iDrawCol; ++iCol )
    {
      if( horizontal )
        gridHisto->SetColLabelValue( iCol, labels[ iCol ] );
      else
        gridHisto->SetRowLabelValue( iCol, labels[ iCol ] );
    }

    if( !horizontal || commStat)
    {
      gridHisto->SetRowLabelValue( iDrawCol, wxT( "" ) );
      for( int i = 0; i < NUMTOTALS; ++i )
        gridHisto->SetRowLabelValue( iDrawCol + i + 1, 
          wxString::FromAscii( LabelConstructor::histoTotalLabel( (THistoTotals) i ).c_str() ) );
    }
  }
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
  if (name == _T("caution.xpm"))
  {
    wxBitmap bitmap(caution_xpm);
    return bitmap;
  }
  else if (name == _T("opencontrol.xpm"))
  {
    wxBitmap bitmap(opencontrol_xpm);
    return bitmap;
  }
  else if (name == _T("opendata.xpm"))
  {
    wxBitmap bitmap(opendata_xpm);
    return bitmap;
  }
  else if (name == _T("open3d.xpm"))
  {
    wxBitmap bitmap(open3d_xpm);
    return bitmap;
  }
  else if (name == _T("histo_zoom.xpm"))
  {
    wxBitmap bitmap(histo_zoom_xpm);
    return bitmap;
  }
  else if (name == _T("openfiltered.xpm"))
  {
    wxBitmap bitmap(openfiltered_xpm);
    return bitmap;
  }
  else if (name == _T("histo_color.xpm"))
  {
    wxBitmap bitmap(color_xpm);
    return bitmap;
  }
  else if (name == _T("histo_horvert.xpm"))
  {
    wxBitmap bitmap(horvert_xpm);
    return bitmap;
  }
  else if (name == _T("hide_cols.xpm"))
  {
    wxBitmap bitmap(hide_xpm);
    return bitmap;
  }
  else if (name == _T("semantic_color.xpm"))
  {
    wxBitmap bitmap(semantic_color_xpm);
    return bitmap;
  }
  else if (name == _T("inclusive.xpm"))
  {
    wxBitmap bitmap(inclusive_xpm);
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
#ifndef WIN32
//  if( myHistogram->getZoom() )
  if( IsActive() )
    zoomHisto->SetFocus();
//  else
//    gridHisto->SetFocus();
#endif

  if( myHistogram->getDestroy() )
    return;

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
    
  myHistogram->setPosX( this->GetPosition().x );
  myHistogram->setPosY( this->GetPosition().y );
  myHistogram->setWidth( this->GetClientSize().GetWidth() );
  myHistogram->setHeight( this->GetClientSize().GetHeight() );
  
  controlWarning->Show( myHistogram->getControlOutOfLimits() );
  xtraWarning->Show( myHistogram->getExtraOutOfLimits() );
  Layout();
}

void gHistogram::updateHistogram()
{
  if( myHistogram->getRecalc() )
  {
    myHistogram->setRecalc( false );
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
  if (wxTheClipboard->Open())
  {
    wxGridCellCoords topLeft = event.GetTopLeftCoords();
    wxTheClipboard->SetData( new wxTextDataObject( gridHisto->GetCellValue( topLeft ) ) );
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
  updateHistogram();
  myHistogram->setRecalc( true );
}

void gHistogram::OnPopUpPaste3DScale()
{
  myHistogram->setCompute3DScale( false );
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, STR_3D_SCALE );
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
  wxSize size = wxSize( myHistogram->getWidth(), myHistogram->getHeight() + titleBarSize.GetHeight() );

  string composedName = clonedName + " @ " +
                        clonedHistogram->getTrace()->getTraceNameNumbered();

  gHistogram *clonedGHistogram = new gHistogram( parent, wxID_ANY, wxString::FromAscii( composedName.c_str() ), position, size );
  clonedGHistogram->myHistogram = clonedHistogram;

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
  clonedGHistogram->execute();
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

  zoomRow.begin = beginRow;
  zoomRow.end = endRow;

  myHistogram->setRecalc( true );
  updateHistogram();
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

void gHistogram::OnPopUpDrawModeSemanticAverage()
{
  myHistogram->setDrawModeColumns( DRAW_AVERAGE );
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

void gHistogram::OnPopUpDrawModeObjectsAverage()
{
  myHistogram->setDrawModeObjects( DRAW_AVERAGE );
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

void gHistogram::OnPopUpDrawModeBothAverage()
{
  myHistogram->setDrawModeObjects( DRAW_AVERAGE );
  myHistogram->setDrawModeColumns( DRAW_AVERAGE );
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
  
  if( ready )
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
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOLHORIZVERT
 */

void gHistogram::OnToolhorizvertClick( wxCommandEvent& event )
{
  myHistogram->setHorizontal( event.IsChecked() );
  myHistogram->setRedraw( true );
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

  timerZoom->Start( 100, true );

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
                                                    floor( lastPosZoomX / zoomCellWidth );

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
    text << _( "= " ) << wxString::FromAscii( LabelConstructor::histoCellLabel( myHistogram, 
                                                                                value, 
                                                                                myHistogram->getShowUnits() ).c_str() );
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

    if ( max - min == 0 )
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
  zoomRow.begin = newObjectBegin;
  zoomRow.end = newObjectEnd;

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

    if ( !event.ControlDown() )
    {
      yBegin = 0;
      yEnd = zoomHisto->GetSize().GetHeight() - 1;
    }

    THistogramColumn columnBegin, columnEnd;
    TObjectOrder objectBegin, objectEnd;
    openControlGetParameters( xBegin, xEnd, yBegin, yEnd,
                              columnBegin, columnEnd, objectBegin, objectEnd );

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
                                           TObjectOrder& objectBegin, TObjectOrder& objectEnd )
{
  columnBegin = myHistogram->getHorizontal() ? floor( xBegin / zoomCellWidth ) :
                                               floor( yBegin / zoomCellHeight );
  if( columnBegin > 0 ) --columnBegin;
  else if( columnBegin < 0 ) columnBegin = 0;
  columnEnd = myHistogram->getHorizontal() ? floor( xEnd / zoomCellWidth ) :
                                             floor( yEnd / zoomCellHeight );
  if( myHistogram->getControlDelta() == 1.0 && columnEnd > 0 ) --columnEnd;
  else if( columnEnd < 0 ) columnEnd = 0;
  objectBegin = myHistogram->getHorizontal() ? floor( yBegin / zoomCellHeight ) :
                                               floor( xBegin / zoomCellWidth );
  if( objectBegin > 0 ) --objectBegin;
  else if( objectBegin < 0 ) objectBegin = 0;
  objectEnd = myHistogram->getHorizontal() ? floor( yEnd / zoomCellHeight ) :
                                             floor( xEnd / zoomCellWidth );
  if( objectEnd > 0 ) --objectEnd;
  else if( objectEnd < 0 ) objectEnd = 0;

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
  controlCloned->setUsedByHistogram( false );
  THistogramLimit min = myHistogram->getControlMin();
  THistogramLimit max = myHistogram->getControlMax();
  THistogramLimit delta = myHistogram->getControlDelta();
  TWindowLevel onLevel = controlCloned->getFirstFreeCompose();

  if( columnBegin == columnEnd && delta != 1.0 )
    ++columnEnd;

  if ( ( ( columnEnd * delta ) + min/* + delta*/ ) >= max || delta == 1.0)
    controlCloned->setLevelFunction( onLevel, "Select Range" );
  else
    controlCloned->setLevelFunction( onLevel, "Select Range [)" );
      
  TParamValue param;
  if( ( ( columnEnd * delta ) + min/* + delta*/ ) >= max )
    param.push_back( max );
  else
    param.push_back( ( columnEnd * delta ) + min/* + delta*/ );
  controlCloned->setFunctionParam( onLevel, 0, param );

  param.clear();
  param.push_back( ( columnBegin * delta ) + min );
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
    extraControlCloned->setUsedByHistogram( false );

    THistogramLimit extraMin = myHistogram->getExtraControlMin();
    THistogramLimit extraMax = myHistogram->getExtraControlMax();
    THistogramLimit extraDelta = myHistogram->getExtraControlDelta();
    onLevel = extraControlCloned->getFirstFreeCompose();
    PRV_UINT32 plane;
    if( myHistogram->itsCommunicationStat( myHistogram->getCurrentStat() ) )
      plane = myHistogram->getCommSelectedPlane();
    else
      plane = myHistogram->getSelectedPlane();

    if ( ( ( plane * extraDelta ) + extraMin + extraDelta ) >= extraMax || extraDelta == 1 )
      extraControlCloned->setLevelFunction( onLevel, "Is In Range" );
    else
      extraControlCloned->setLevelFunction( onLevel, "Is In Range [)" );
      
    TParamValue param;
    if( ( ( plane * extraDelta ) + extraMin + extraDelta ) >= extraMax )
      param.push_back( extraMax );
    else if( extraDelta == 1 )
      param.push_back( ( plane * extraDelta ) + extraMin );
    else
      param.push_back( ( plane * extraDelta ) + extraMin + extraDelta );
    extraControlCloned->setFunctionParam( onLevel, 0, param );

    param.clear();
    param.push_back( ( plane * extraDelta ) + extraMin );
    extraControlCloned->setFunctionParam( onLevel, 1, param );

    string name = extraControlCloned->getName();
    name = name.substr( 0, name.find_last_of( '.' ) );
    stringstream tmpStr;
    tmpStr << name << " 3DZoom Mask range [" << ( plane * extraDelta ) + min << ",";
    if ( ( ( plane * extraDelta ) + extraMin + extraDelta ) >= extraMax )
      tmpStr << extraMax << "]";
    else if( extraDelta == 1 )
        tmpStr << ( plane * extraDelta ) + extraMin + extraDelta << "]";
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
      tmpStr << ( columnEnd * delta ) + min + delta << "]";
    else
      tmpStr << ( columnEnd * delta ) + min + delta << ")";
    tmpStr << "/[" << ( plane * extraDelta ) + extraMin << ",";
    if ( ( ( plane * extraDelta ) + extraMin + extraDelta ) >= extraMax )
      tmpStr << extraMax << "]";
    else if( extraDelta == 1 )
        tmpStr << ( plane * extraDelta ) + extraMin + extraDelta << "]";
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
    productWin->setDrawFunctionLineColor( controlCloned->getDrawFunctionLineColor() );
    if( controlCloned->IsCodeColorSet() )
      productWin->setCodeColorMode();
    else if( controlCloned->IsGradientColorSet() )
    {
      productWin->setGradientColorMode();
      productWin->allowOutOfScale( true );
      productWin->allowOutliers( true );
    }
    else
    {
      productWin->setGradientColorMode();
      productWin->allowOutOfScale( false );
      productWin->allowOutliers( true );
    }
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

    if( columnBegin == columnEnd )
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

    openWindow->GetMyWindow()->setUsedByHistogram( false );
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

void gHistogram::saveText()
{
  wxString fileName, defaultDir;
  
  string auxName = myHistogram->getName() + "_" + myHistogram->getTrace()->getTraceNameNumbered();
  fileName = wxString::FromAscii( auxName.c_str() );

#ifdef WIN32
  defaultDir = _(".\\");
#else
  defaultDir = _("./");
#endif

  wxFileDialog saveDialog( this,
                           _("Save as text"),
                           defaultDir,
                           fileName,
                           _( "*.*" ),
                           wxSAVE | wxFD_OVERWRITE_PROMPT );

  if ( saveDialog.ShowModal() == wxID_OK )
  {
    TextOutput output;
    output.setMultipleFiles( false );
    string tmpStr = string( saveDialog.GetPath().mb_str() );
    output.dumpHistogram( myHistogram, tmpStr, false, myHistogram->getHideColumns() );
  }
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_LABEL_COLORS
 */

void gHistogram::OnToolLabelColorsClick( wxCommandEvent& event )
{
  myHistogram->setFirstRowColored( event.IsChecked() );
  myHistogram->setRedraw( true );
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
      OnPopUpPasteSpecial();
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
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_INCLUSIVE
 */

void gHistogram::OnToolInclusiveClick( wxCommandEvent& event )
{
  myHistogram->setInclusive( event.IsChecked() );
  myHistogram->setRecalc( true );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TOOL_INCLUSIVE
 */

void gHistogram::OnToolInclusiveUpdate( wxUpdateUIEvent& event )
{
  event.Enable( myHistogram->getInclusiveEnabled() );
  event.Check( myHistogram->getInclusive() );
}

