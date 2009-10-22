/////////////////////////////////////////////////////////////////////////////
// Name:        ghistogram.cpp
// Purpose:     
// Author:      Eloy Martinez
// Modified by: 
// Created:     Mon 23 Jun 2008 11:40:03 CEST
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

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
#include "ghistogram.h"
#include "histogram.h"
#include "labelconstructor.h"
#include "histogramtotals.h"
#include "loadedwindows.h"
#include "windows_tree.h"
#include "paravermain.h"

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
  openControlDragging = false;
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

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
  itemFrame1->SetSizer(itemBoxSizer2);

  mainSizer = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer2->Add(mainSizer, 1, wxGROW|wxALL, 0);

  zoomHisto = new wxScrolledWindow( itemFrame1, ID_ZOOMHISTO, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxFULL_REPAINT_ON_RESIZE|wxHSCROLL|wxVSCROLL );
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

  controlWarning = new wxStaticBitmap( itemFrame1, wxID_CONTROLWARNING, itemFrame1->GetBitmapResource(wxT("caution.xpm")), wxDefaultPosition, itemFrame1->ConvertDialogToPixels(wxSize(9, 8)), 0 );
  if (gHistogram::ShowToolTips())
    controlWarning->SetToolTip(_("Control limits not fitted"));
  warningSizer->Add(controlWarning, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxFIXED_MINSIZE, 5);

  xtraWarning = new wxStaticBitmap( itemFrame1, wxID_3DWARNING, itemFrame1->GetBitmapResource(wxT("caution.xpm")), wxDefaultPosition, itemFrame1->ConvertDialogToPixels(wxSize(9, 8)), 0 );
  if (gHistogram::ShowToolTips())
    xtraWarning->SetToolTip(_("3D limits not fitted"));
  warningSizer->Add(xtraWarning, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxFIXED_MINSIZE, 5);

  wxStaticBitmap* itemStaticBitmap9 = new wxStaticBitmap( itemFrame1, wxID_STATIC, itemFrame1->GetBitmapResource(wxT("caution.xpm")), wxDefaultPosition, itemFrame1->ConvertDialogToPixels(wxSize(9, 9)), 0 );
  itemStaticBitmap9->Show(false);
  warningSizer->Add(itemStaticBitmap9, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxFIXED_MINSIZE, 5);

  warningSizer->Add(17, 20, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxToolBar* itemToolBar11 = CreateToolBar( wxTB_FLAT|wxTB_HORIZONTAL, ID_AUITOOLBAR1 );
  wxBitmap itemtool12Bitmap(itemFrame1->GetBitmapResource(wxT("opencontrol.xpm")));
  wxBitmap itemtool12BitmapDisabled;
  itemToolBar11->AddTool(ID_TOOL_OPEN_CONTROL_WINDOW, _("Open Control Window"), itemtool12Bitmap, itemtool12BitmapDisabled, wxITEM_NORMAL, _("Open Control Window"), wxEmptyString);
  wxBitmap itemtool13Bitmap(itemFrame1->GetBitmapResource(wxT("opendata.xpm")));
  wxBitmap itemtool13BitmapDisabled;
  itemToolBar11->AddTool(ID_TOOL_OPEN_DATA_WINDOW, _("Open Data Window"), itemtool13Bitmap, itemtool13BitmapDisabled, wxITEM_NORMAL, _("Open Data Window"), wxEmptyString);
  wxBitmap itemtool14Bitmap(itemFrame1->GetBitmapResource(wxT("open3d.xpm")));
  wxBitmap itemtool14BitmapDisabled;
  itemToolBar11->AddTool(ID_TOOL_OPEN_EXTRA_WINDOW, _("Open 3D Window"), itemtool14Bitmap, itemtool14BitmapDisabled, wxITEM_NORMAL, _("Open 3D Window"), wxEmptyString);
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
  itemToolBar11->Connect(ID_AUITOOLBAR1, wxEVT_LEFT_DOWN, wxMouseEventHandler(gHistogram::OnLeftDown), NULL, this);
  itemToolBar11->Connect(ID_AUITOOLBAR1, wxEVT_RIGHT_DOWN, wxMouseEventHandler(gHistogram::OnRightDown), NULL, this);
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
  Update();
  
  selectedRows.clear();
  TObjectOrder beginRow = myHistogram->getControlWindow()->getZoomSecondDimension().first;
  TObjectOrder endRow =  myHistogram->getControlWindow()->getZoomSecondDimension().second;
  myHistogram->getControlWindow()->getSelectedRows( myHistogram->getControlWindow()->getLevel(),
                                                    selectedRows, beginRow, endRow );

  myHistogram->execute( myHistogram->getBeginTime(), myHistogram->getEndTime(), selectedRows );

  if( myHistogram->getZoom() )
    fillZoom();
  else
    fillGrid();

  ready = true;

  this->Refresh();

  SetTitle( winTitle );
}


void gHistogram::fillGrid()
{
  int rowLabelWidth = 0;
  wxFont labelFont = gridHisto->GetLabelFont();
  bool commStat = myHistogram->itsCommunicationStat( myHistogram->getCurrentStat() );
  UINT16 idStat;
  THistogramColumn curPlane;
  THistogramColumn numCols, numDrawCols;
  TObjectOrder numRows, numDrawRows;
  bool horizontal = myHistogram->getHorizontal();
  
  zoomHisto->Show( false );
  gridHisto->Show( true );
  mainSizer->Layout();
  
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
    numDrawRows = numRows;
  }
  else
  {
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

  for( THistogramColumn iCol = 0; iCol < numCols; iCol++ )
  {
    if( commStat )
    {
      gridHisto->SetColLabelValue( iCol, myHistogram->getRowLabel( iCol ) );
      myHistogram->setCommFirstCell( iCol, curPlane );
    }
    else
    {
      if( horizontal )
        gridHisto->SetColLabelValue( iCol, myHistogram->getColumnLabel( iCol ) );
      else
      {
        int w, h;
      
        gridHisto->GetTextExtent( myHistogram->getColumnLabel( iCol ), &w, &h, NULL, NULL, &labelFont );
        if( rowLabelWidth == 0 || rowLabelWidth < w )
          rowLabelWidth = w;
        gridHisto->SetRowLabelValue( iCol, myHistogram->getColumnLabel( iCol ) );
      }
      myHistogram->setFirstCell( iCol, curPlane );
    }

    for( TObjectOrder iRow = 0; iRow < numRows; iRow++ )
    {
      if( horizontal )
      {
        if( iCol == 0 )
        {
          int w, h;
      
          gridHisto->GetTextExtent( myHistogram->getRowLabel( selectedRows[ iRow ] ),
                                    &w, &h, NULL, NULL, &labelFont );
          if( rowLabelWidth == 0 || rowLabelWidth < w )
            rowLabelWidth = w;
          gridHisto->SetRowLabelValue( iRow, myHistogram->getRowLabel( selectedRows[ iRow ] ) );
        }
      }
      else
      {
        gridHisto->SetColLabelValue( iRow, myHistogram->getRowLabel( selectedRows[ iRow ] ) );
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
        gridHisto->SetCellValue( iDrawRow, iDrawCol, wxString( "-" ) );
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
            gridHisto->SetCellValue( iDrawRow, iDrawCol, wxString( tmpStr ) );
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
            gridHisto->SetCellValue( iDrawRow, iDrawCol, wxString( "-" ) );
            gridHisto->SetCellBackgroundColour( iDrawRow, iDrawCol, *wxWHITE );
          }
        }
        else
        {
          if( myHistogram->getCurrentRow( iCol, curPlane ) == iRow )
          {
            string tmpStr;
            tmpStr = LabelConstructor::histoCellLabel( myHistogram,
              myHistogram->getCurrentValue( iCol, idStat, curPlane ), true );
            gridHisto->SetCellValue( iDrawRow, iDrawCol, wxString( tmpStr ) );
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
            else gridHisto->SetCellBackgroundColour( iDrawRow, iDrawCol, *wxWHITE );
            myHistogram->setNextCell( iCol, curPlane );
          }
          else
          {
            gridHisto->SetCellValue( iDrawRow, iDrawCol, wxString( "-" ) );
            gridHisto->SetCellBackgroundColour( iDrawRow, iDrawCol, *wxWHITE );
          }
        }
      }
    }

    gridHisto->SetRowLabelValue( numDrawRows, "" );
  }

  fillTotals( rowLabelWidth, numDrawRows + 1, curPlane, idStat );

  gridHisto->SetRowLabelSize( rowLabelWidth + 5 );
  gridHisto->AutoSizeColumns();
  gridHisto->AutoSizeRows();
  gridHisto->EndBatch();
}

void gHistogram::fillZoom()
{
  bool commStat = myHistogram->itsCommunicationStat( myHistogram->getCurrentStat() );
  UINT16 idStat;
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

  for( THistogramColumn iCol = 0; iCol < tmpNumCols; iCol++ )
  {
    THistogramColumn realCol = iCol;
    if( myHistogram->getHideColumns() )
      realCol = noVoidColumns[ iCol ];
      
    if( commStat )
      myHistogram->setCommFirstCell( realCol, curPlane );
    else
      myHistogram->setFirstCell( realCol, curPlane );

    bufferDraw.SetBrush( *wxGREY_BRUSH );
    bufferDraw.SetPen( *wxTRANSPARENT_PEN );
    bufferDraw.DrawRectangle( 0, 0, bufferDraw.GetSize().GetWidth(), cellHeight );
    bufferDraw.DrawRectangle( 0, 0, cellWidth, bufferDraw.GetSize().GetHeight() );

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
  double& cellWidth = zoomCellWidth;
  double& cellHeight = zoomCellHeight;
  UINT16 idStat;
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
    if( valuesObjects.begin() != valuesObjects.end() &&
        ( iRow == numRows || 
          ( horizontal && rint( ( iRow + 1 ) * cellHeight ) != rint( ( iRow + 2 ) * cellHeight ) ) ||
          ( !horizontal && rint( ( iRow + 1 ) * cellWidth ) != rint( ( iRow + 2 ) * cellWidth ) )
        )
      )
    {
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

void gHistogram::fillTotals( int& rowLabelWidth, TObjectOrder beginRow, THistogramColumn curPlane, UINT16 idStat )
{
  THistogramColumn numDrawCols;
  wxFont labelFont = gridHisto->GetLabelFont();
  bool horizontal = myHistogram->getHorizontal();
  HistogramTotals *histoTotals = myHistogram->getTotals( myHistogram->getCurrentStat() );
  
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

      gridHisto->GetTextExtent( LabelConstructor::histoTotalLabel( (THistoTotals) i ), 
                                &w, &h, NULL, NULL, &labelFont );
      if( rowLabelWidth == 0 || rowLabelWidth < w )
        rowLabelWidth = w;
      gridHisto->SetRowLabelValue( beginRow + i, 
        LabelConstructor::histoTotalLabel( (THistoTotals) i ) );

      if( totals[ 0 ] > 0.0 )
      {
        string tmpStr;
        if( i == AVGDIVMAX )
          tmpStr = LabelConstructor::histoCellLabel( myHistogram, totals[ i ], false );
        else
          tmpStr = LabelConstructor::histoCellLabel( myHistogram, totals[ i ], true );
        gridHisto->SetCellValue( beginRow + i, iCol, wxString( tmpStr ) );
      }
      else
      {
        gridHisto->SetCellValue( beginRow + i, iCol, wxString( "-" ) );
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
      if( horizontal )
        labels.push_back( gridHisto->GetColLabelValue( iCol ) );
      else
        labels.push_back( gridHisto->GetRowLabelValue( iCol ) );
    }
    
    THistogramColumn iDrawCol = 0;
    vector<wxString>::iterator labelIt = labels.begin();
    for( THistogramColumn iCol = 0; iCol < numDrawCols; ++iCol )
    {
      vector<TSemanticValue> totals;
      histoTotals->getAll( totals, idStat, iCol, curPlane );
    
      if( totals[ 0 ] == 0 )
      {
        if( horizontal )
        {
          gridHisto->DeleteCols( iDrawCol );
          labels.erase( labelIt );
        }
        else
        {
          gridHisto->DeleteRows( iDrawCol );
          labels.erase( labelIt );
        }
      }
      else
      {
        ++iDrawCol;
        ++labelIt;
      }
    }
    
    for( THistogramColumn iCol = 0; iCol < iDrawCol; ++iCol )
    {
      if( horizontal )
        gridHisto->SetColLabelValue( iCol, labels[ iCol ] );
      else
        gridHisto->SetRowLabelValue( iCol, labels[ iCol ] );
    }

    if( !horizontal )
    {
      gridHisto->SetRowLabelValue( iDrawCol, wxT( "" ) );
      for( int i = 0; i < NUMTOTALS; ++i )
        gridHisto->SetRowLabelValue( iDrawCol + i + 1, 
          LabelConstructor::histoTotalLabel( (THistoTotals) i ) );
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

  string composedName = myHistogram->getName() + " @ " +
                        myHistogram->getTrace()->getTraceNameNumbered();

  this->SetTitle( composedName );

  if( myHistogram->getShowWindow() )
    this->Show();
  else
    this->Show( false );
    
  myHistogram->setPosX( this->GetPosition().x );
  myHistogram->setPosY( this->GetPosition().y );
  myHistogram->setWidth( this->GetSize().GetWidth() );
  myHistogram->setHeight( this->GetSize().GetHeight() );
  
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

void gHistogram::OnPopUpPasteSemanticScale()
{
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Semantic Scale" );
  updateHistogram();
  myHistogram->setChanged( true );
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
  wxSize size = wxSize( myHistogram->getWidth(), myHistogram->getHeight() );

  string composedName = clonedName + " @ " +
                        clonedHistogram->getTrace()->getTraceNameNumbered();

  gHistogram *clonedGHistogram = new gHistogram( parent, wxID_ANY, composedName, position, size );
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
    cout << "ERROR! NOT FOUND ORIGINAL CONTROL WINDOW OF HISTOGRAM!" << endl;
    
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
      cout << "ERROR! NOT FOUND ORIGINAL DATA WINDOW OF HISTOGRAM!" << endl;
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
#ifndef WIN32
#warning TODO: Gui Exception class inherited from ParaverKernelException
#endif
      cout << "ERROR! NOT FOUND ORIGINAL EXTRA CONTROL WINDOW OF HISTOGRAM!" << endl;
  }

  // Finally, execute
  clonedGHistogram->execute();
}


void gHistogram::OnPopUpFitTimeScale()
{
  myHistogram->setWindowBeginTime( 0 );
  myHistogram->setWindowEndTime( myHistogram->getControlWindow()->getTrace()->getEndTime() );
//  zoomHistory->addZoom( 0, myHistogram->getControlWindow()->getTrace()->getEndTime() );
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

void gHistogram::OnPopUpUndoZoom(){}
void gHistogram::OnPopUpRedoZoom(){}


void gHistogram::rightDownManager()
{
  paraverMain::myParaverMain->selectTrace( GetHistogram()->getControlWindow()->getTrace() );

  gPopUpMenu popUpMenu( this );
//  popUpMenu->Enable( "Undo Zoom", !zoomHistory->emptyPrevZoom() );
//  popUpMenu->Enable( "Redo Zoom", !zoomHistory->emptyNextZoom() );

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
  lastPosZoomX = event.GetX();
  lastPosZoomY = event.GetY();
  if( ready )
    timerZoom->Start( 100, true );

  if( openControlDragging )
  {
    wxMemoryDC memdc( drawImage );
    memdc.SetBackgroundMode( wxTRANSPARENT );
    memdc.SetBackground( *wxTRANSPARENT_BRUSH );
    memdc.Clear();
#ifdef __WXGTK__
    wxGCDC dc( memdc );
    dc.SetBrush( wxBrush( wxColour( 255, 255, 255, 80 ) ) );
#else
    wxDC& dc = memdc;
    dc.SetBrush( *wxTRANSPARENT_BRUSH );
#endif
    dc.SetPen( *wxWHITE_PEN );

    long beginX = zoomPointBegin.x > event.GetX() ? event.GetX() : zoomPointBegin.x;
    long endX = zoomPointBegin.x < event.GetX() ? event.GetX() : zoomPointBegin.x;
    long beginY = zoomPointBegin.y > event.GetY() ? event.GetY() : zoomPointBegin.y;
    long endY = zoomPointBegin.y < event.GetY() ? event.GetY() : zoomPointBegin.y;
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

  if( column >= myHistogram->getNumColumns() )
    column = myHistogram->getNumColumns() - 1;
    
  if( row >= myHistogram->getNumRows() )
    row = myHistogram->getNumRows() - 1;
    
  if( row > 0 )
    text << _( myHistogram->getRowLabel( selectedRows[ row - 1 ] ).c_str() )
         << _( "  " );

  if( column > 0 )
  {
    if( myHistogram->getHideColumns() )
    {
      columnSelection.getSelected( noVoidColumns );
      column = noVoidColumns[ column - 1 ] + 1;
    }
    text << _( myHistogram->getColumnLabel( column - 1 ).c_str() )
         << _( "  " );
  }
  
  if( row > 0 && column > 0 )
  {
    TSemanticValue value = getZoomSemanticValue( column - 1, row - 1 );
    text << _( "= " ) << _( LabelConstructor::histoCellLabel( myHistogram, 
                                                              value, 
                                                              myHistogram->getShowUnits() ).c_str() );
  }
  
  histoStatus->SetStatusText( text );
}

TSemanticValue gHistogram::getZoomSemanticValue( THistogramColumn column, TObjectOrder row ) const
{
  THistogramColumn plane;
  TSemanticValue value = 0.0;
  UINT16 idStat;
  
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
  if( openControlActivated )
  {
    openControlActivated= false;
    openControlDragging = true;
    zoomPointBegin = event.GetPosition();
  }
}


/*!
 * wxEVT_LEFT_UP event handler for ID_ZOOMHISTO
 */

void gHistogram::OnLeftUp( wxMouseEvent& event )
{
  if( openControlDragging )
  {
    openControlDragging = false;
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
    
    if( xBegin < 0 ) xBegin = 0;
    if( yBegin < 0 ) yBegin = 0;
    if( xEnd > zoomHisto->GetSize().GetWidth() ) xEnd = zoomHisto->GetSize().GetWidth();
    if( yEnd > zoomHisto->GetSize().GetHeight() ) yEnd = zoomHisto->GetSize().GetHeight();

    THistogramColumn columnBegin, columnEnd;
    TObjectOrder objectBegin, objectEnd;
    openControlGetParameters( xBegin, xEnd, yBegin, yEnd,
                              columnBegin, columnEnd, objectBegin, objectEnd );
    openControlWindow( columnBegin, columnEnd, objectBegin, objectEnd );
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
  if( columnEnd > 0 ) --columnEnd;
  else if( columnEnd < 0 ) columnEnd = 0;
  objectBegin = myHistogram->getHorizontal() ? floor( yBegin / zoomCellHeight ) :
                                               floor( xBegin / zoomCellWidth );
  if( objectBegin > 0 ) --objectBegin;
  else if( objectBegin < 0 ) objectBegin = 0;
  objectBegin = selectedRows[ objectBegin ];
  objectEnd = myHistogram->getHorizontal() ? floor( yEnd / zoomCellHeight ) :
                                             floor( xEnd / zoomCellWidth );
  if( objectEnd > 0 ) --objectEnd;
  else if( objectEnd < 0 ) objectEnd = 0;
  objectEnd = selectedRows[ objectEnd ];

  if( myHistogram->getHideColumns() )
  {
    vector<THistogramColumn> noVoidColumns;
    columnSelection.getSelected( noVoidColumns );
    columnBegin = noVoidColumns[ columnBegin ];
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
  THistogramLimit min = myHistogram->getControlMin();
  THistogramLimit max = myHistogram->getControlMax();
  THistogramLimit delta = myHistogram->getControlDelta();
  TWindowLevel onLevel = controlCloned->getFirstFreeCompose();

  if ( ( ( columnEnd * delta ) + min + delta ) >= max )
    controlCloned->setLevelFunction( onLevel, "Select Range" );
  else
    controlCloned->setLevelFunction( onLevel, "Select Range [)" );
      
  TParamValue param;
  if( ( ( columnEnd * delta ) + min + delta ) >= max )
    param.push_back( max );
  else
    param.push_back( ( columnEnd * delta ) + min + delta );
  controlCloned->setFunctionParam( onLevel, 0, param );

  param.clear();
  param.push_back( ( columnBegin * delta ) + min );
  controlCloned->setFunctionParam( onLevel, 1, param );

  string name = controlCloned->getName();
  name = name.substr( 0, name.find_last_of( '.' ) );
  stringstream tmpStr;
  tmpStr << name << " 2DZoom range [" << ( columnBegin * delta ) + min << ",";
  if ( ( ( columnEnd * delta ) + min + delta ) >= max )
    tmpStr << max << "]";
  else
    tmpStr << ( columnEnd * delta ) + min + delta << ")";
  controlCloned->setName( tmpStr.str() );

  controlCloned->setWindowBeginTime( myHistogram->getBeginTime() );
  controlCloned->setWindowEndTime( myHistogram->getEndTime() );
  controlCloned->addZoom( myHistogram->getBeginTime(), myHistogram->getEndTime(), 
                          objectBegin, objectEnd );

  if( myHistogram->getThreeDimensions() )
  {
    Window *extraControlCloned = myHistogram->getExtraControlWindow()->clone();

    THistogramLimit extraMin = myHistogram->getExtraControlMin();
    THistogramLimit extraMax = myHistogram->getExtraControlMax();
    THistogramLimit extraDelta = myHistogram->getExtraControlDelta();
    onLevel = extraControlCloned->getFirstFreeCompose();
    UINT32 plane;
    if( myHistogram->itsCommunicationStat( myHistogram->getCurrentStat() ) )
      plane = myHistogram->getCommSelectedPlane();
    else
      plane = myHistogram->getSelectedPlane();

    if ( ( ( plane * extraDelta ) + extraMin + extraDelta ) >= extraMax )
      extraControlCloned->setLevelFunction( onLevel, "Is In Range" );
    else
      extraControlCloned->setLevelFunction( onLevel, "Is In Range [)" );
      
    TParamValue param;
    if( ( ( plane * extraDelta ) + extraMin + extraDelta ) >= extraMax )
      param.push_back( extraMax );
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
    else
        tmpStr << ( plane * extraDelta ) + extraMin + extraDelta << ")";
    extraControlCloned->setName( tmpStr.str() );

    extraControlCloned->setWindowBeginTime( myHistogram->getBeginTime() );
    extraControlCloned->setWindowEndTime( myHistogram->getEndTime() );

    controlCloned->setShowWindow( false );
    extraControlCloned->setShowWindow( false );
    Window *productWin = Window::create( controlCloned->getKernel(), controlCloned, extraControlCloned );

    productWin->setLevelFunction( DERIVED, "product" );
    tmpStr.str( "" );
    tmpStr << name << " 3DZoom ranges [" << ( columnBegin * delta ) + min << ",";
    if ( ( ( columnEnd * delta ) + min + delta ) >= max )
      tmpStr << max << "]";
    else
      tmpStr << ( columnEnd * delta ) + min + delta << ")";
    tmpStr << "/[" << ( plane * extraDelta ) + extraMin << ",";
    if ( ( ( plane * extraDelta ) + extraMin + extraDelta ) >= extraMax )
      tmpStr << extraMax << "]";
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
                         objectBegin, objectEnd );
    
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
    openWindow->GetMyWindow()->setShowWindow( true );
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


/*!
 * wxEVT_RIGHT_DOWN event handler for ID_AUITOOLBAR1
 */

void gHistogram::OnRightDown( wxMouseEvent& event )
{
////@begin wxEVT_RIGHT_DOWN event handler for ID_AUITOOLBAR1 in gHistogram.
  // Before editing this code, remove the block markers.
  event.Skip();
////@end wxEVT_RIGHT_DOWN event handler for ID_AUITOOLBAR1 in gHistogram. 
}

