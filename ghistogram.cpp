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
#include "histo_zoom.xpm"
#include "timeline.xpm"
#include "histo_color.xpm"
#include "histo_horvert.xpm"
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

  EVT_GRID_CELL_RIGHT_CLICK( gHistogram::OnCellRightClick )
  EVT_GRID_LABEL_RIGHT_CLICK( gHistogram::OnLabelRightClick )
  EVT_GRID_RANGE_SELECT( gHistogram::OnRangeSelect )
  EVT_UPDATE_UI( ID_GRIDHISTO, gHistogram::OnGridhistoUpdate )

  EVT_MENU( ID_TOOLZOOM, gHistogram::OnToolzoomClick )
  EVT_UPDATE_UI( ID_TOOLZOOM, gHistogram::OnToolzoomUpdate )

  EVT_MENU( ID_TOOL_OPEN_CONTROL_WINDOW, gHistogram::OnToolOpenControlWindowClick )
  EVT_UPDATE_UI( ID_TOOL_OPEN_CONTROL_WINDOW, gHistogram::OnToolOpenControlWindowUpdate )

  EVT_MENU( ID_TOOLGRADIENT, gHistogram::OnToolgradientClick )
  EVT_UPDATE_UI( ID_TOOLGRADIENT, gHistogram::OnToolgradientUpdate )

  EVT_MENU( ID_TOOLHORIZVERT, gHistogram::OnToolhorizvertClick )
  EVT_UPDATE_UI( ID_TOOLHORIZVERT, gHistogram::OnToolhorizvertUpdate )

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

  mainSizer = new wxBoxSizer(wxVERTICAL);
  itemFrame1->SetSizer(mainSizer);

  zoomHisto = new wxScrolledWindow( itemFrame1, ID_ZOOMHISTO, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxFULL_REPAINT_ON_RESIZE|wxHSCROLL|wxVSCROLL );
  mainSizer->Add(zoomHisto, 1, wxGROW|wxALL, 1);
  zoomHisto->SetScrollbars(1, 1, 0, 0);

  gridHisto = new wxGrid( itemFrame1, ID_GRIDHISTO, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL|wxALWAYS_SHOW_SB );
  gridHisto->SetDefaultColSize(50);
  gridHisto->SetDefaultRowSize(25);
  gridHisto->SetColLabelSize(25);
  gridHisto->SetRowLabelSize(50);
  mainSizer->Add(gridHisto, 1, wxGROW|wxALL, 1);

  histoStatus = new wxStatusBar( itemFrame1, ID_HISTOSTATUS, wxST_SIZEGRIP|wxNO_BORDER );
  histoStatus->SetFieldsCount(1);
  mainSizer->Add(histoStatus, 0, wxGROW, 5);

  wxToolBar* itemToolBar6 = CreateToolBar( wxTB_FLAT|wxTB_HORIZONTAL, ID_TOOLBAR1 );
  wxBitmap itemtool7Bitmap(itemFrame1->GetBitmapResource(wxT("histo_zoom.xpm")));
  wxBitmap itemtool7BitmapDisabled;
  itemToolBar6->AddTool(ID_TOOLZOOM, _("Zoom"), itemtool7Bitmap, itemtool7BitmapDisabled, wxITEM_CHECK, _("Histogram zoom"), wxEmptyString);
  wxBitmap itemtool8Bitmap(itemFrame1->GetBitmapResource(wxT("timeline.xpm")));
  wxBitmap itemtool8BitmapDisabled;
  itemToolBar6->AddTool(ID_TOOL_OPEN_CONTROL_WINDOW, _("Open Control Window"), itemtool8Bitmap, itemtool8BitmapDisabled, wxITEM_NORMAL, _("Open Control Window"), wxEmptyString);
  itemToolBar6->AddSeparator();
  wxBitmap itemtool10Bitmap(itemFrame1->GetBitmapResource(wxT("histo_color.xpm")));
  wxBitmap itemtool10BitmapDisabled;
  itemToolBar6->AddTool(ID_TOOLGRADIENT, _("Gradient"), itemtool10Bitmap, itemtool10BitmapDisabled, wxITEM_CHECK, _("View gradient colors"), wxEmptyString);
  wxBitmap itemtool11Bitmap(itemFrame1->GetBitmapResource(wxT("histo_horvert.xpm")));
  wxBitmap itemtool11BitmapDisabled;
  itemToolBar6->AddTool(ID_TOOLHORIZVERT, _("Horizontal/Vertical"), itemtool11Bitmap, itemtool11BitmapDisabled, wxITEM_CHECK, _("Horizontal/Vertical"), wxEmptyString);
  itemToolBar6->Realize();
  itemFrame1->SetToolBar(itemToolBar6);

  // Connect events and objects
  zoomHisto->Connect(ID_ZOOMHISTO, wxEVT_PAINT, wxPaintEventHandler(gHistogram::OnPaint), NULL, this);
  zoomHisto->Connect(ID_ZOOMHISTO, wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(gHistogram::OnEraseBackground), NULL, this);
  zoomHisto->Connect(ID_ZOOMHISTO, wxEVT_LEFT_DOWN, wxMouseEventHandler(gHistogram::OnLeftDown), NULL, this);
  zoomHisto->Connect(ID_ZOOMHISTO, wxEVT_LEFT_UP, wxMouseEventHandler(gHistogram::OnLeftUp), NULL, this);
  zoomHisto->Connect(ID_ZOOMHISTO, wxEVT_MOTION, wxMouseEventHandler(gHistogram::OnMotion), NULL, this);
  zoomHisto->Connect(ID_ZOOMHISTO, wxEVT_CONTEXT_MENU, wxContextMenuEventHandler(gHistogram::OnZoomContextMenu), NULL, this);
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
  SetTitle( winTitle + _(" (Working...)") );
  Update();
  
  selectedRows.clear();
  TObjectOrder beginRow = myHistogram->getControlWindow()->getZoomSecondDimension().first;
  TObjectOrder endRow =  myHistogram->getControlWindow()->getZoomSecondDimension().second;
  myHistogram->getControlWindow()->getSelectedRows( selectedRows, beginRow, endRow );
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
    throw( exception() );

  if( myHistogram->getComputeGradient() )
    myHistogram->recalcGradientLimits();
    
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
    throw( exception() );

  if( myHistogram->getComputeGradient() )
    myHistogram->recalcGradientLimits();
    
  if( commStat )
    curPlane = myHistogram->getCommSelectedPlane();
  else
    curPlane = myHistogram->getSelectedPlane();

  numCols = myHistogram->getNumColumns( myHistogram->getCurrentStat() );
  if( horizontal )
  {
    numDrawCols = myHistogram->getNumColumns( myHistogram->getCurrentStat() );
    numDrawRows = myHistogram->getNumRows();
  }
  else
  {
    numDrawCols = myHistogram->getNumRows();
    numDrawRows = myHistogram->getNumColumns( myHistogram->getCurrentStat() );
  }

  cellWidth = (double)( zoomHisto->GetSize().GetWidth() ) / (double)( numDrawCols + 1 );
  cellHeight = (double)( zoomHisto->GetSize().GetHeight() ) / (double)( numDrawRows + 1 );

  for( THistogramColumn iCol = 0; iCol < numCols; iCol++ )
  {
    if( commStat )
      myHistogram->setCommFirstCell( iCol, curPlane );
    else
      myHistogram->setFirstCell( iCol, curPlane );

    bufferDraw.SetBrush( *wxGREY_BRUSH );
    bufferDraw.SetPen( *wxTRANSPARENT_PEN );
    bufferDraw.DrawRectangle( 0, 0, bufferDraw.GetSize().GetWidth(), cellHeight );
    bufferDraw.DrawRectangle( 0, 0, cellWidth, bufferDraw.GetSize().GetHeight() );

    THistogramColumn beginCol = iCol;
    THistogramColumn endCol = beginCol;
    if( horizontal )
    {
      while( ( endCol + 1 ) <= numCols && ( endCol + 2 ) * cellWidth == ( beginCol + 1 ) * cellWidth )
        ++endCol;
    }
    else
    {
      while( ( endCol + 1 ) <= numCols && ( endCol + 2 ) * cellHeight == ( beginCol + 1 ) * cellHeight )
        ++endCol;
    }
    
    drawColumn( beginCol, endCol, bufferDraw );
  }
  
  bufferDraw.SetPen( *wxBLACK_PEN );
  bufferDraw.SetBrush( *wxTRANSPARENT_BRUSH );
  bufferDraw.DrawRectangle( 0, 0, bufferDraw.GetSize().GetWidth(), bufferDraw.GetSize().GetHeight() );
  if( cellHeight > 3.0 )
  {
    for( TObjectOrder iRow = 0; iRow < numDrawRows; ++iRow )
     bufferDraw.DrawLine( 0, ( iRow + 1 ) * cellHeight, bufferDraw.GetSize().GetWidth(), ( iRow + 1 ) * cellHeight );
  }
  if( cellWidth > 3.0 )
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
                             wxMemoryDC& bufferDraw )
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
    throw( exception() );

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

    for( THistogramColumn iCol = beginColumn; iCol <= endColumn; ++iCol )
    {
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
  HistogramTotals *histoTotals = myHistogram->getTotals( myHistogram->getCurrentStat() );
  
  if( myHistogram->getHorizontal() )
    numDrawCols = myHistogram->getNumColumns( myHistogram->getCurrentStat() );
  else
    numDrawCols = myHistogram->getNumRows();

  for( THistogramColumn iCol = 0; iCol < numDrawCols; iCol++ )
  {
    vector<TSemanticValue> totals;
    histoTotals->getAll( totals, idStat, iCol, curPlane );

    for( int i = 0; i < NUMTOTALS; i++ )
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
  if (name == _T("histo_zoom.xpm"))
  {
    wxBitmap bitmap( histo_zoom_xpm);
    return bitmap;
  }
  else if (name == _T("timeline.xpm"))
  {
    wxBitmap bitmap( timeline_xpm);
    return bitmap;
  }
  else if (name == _T("histo_color.xpm"))
  {
    wxBitmap bitmap( color_xpm);
    return bitmap;
  }
  else if (name == _T("histo_horvert.xpm"))
  {
    wxBitmap bitmap( horvert_xpm);
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
  this->SetTitle( myHistogram->getName() );

  if( myHistogram->getShowWindow() )
    this->Show();
  else
    this->Show( false );
    
  myHistogram->setPosX( this->GetPosition().x );
  myHistogram->setPosY( this->GetPosition().y );
  myHistogram->setWidth( this->GetSize().GetWidth() );
  myHistogram->setHeight( this->GetSize().GetHeight() );
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
      SetTitle( winTitle + _(" (Working...)") );
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
  gHistogram *clonedGHistogram = new gHistogram( parent, wxID_ANY, clonedName, position, size );
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
                             getSelectedTraceTree()->GetRootItem() );
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
                            getSelectedTraceTree()->GetRootItem() );
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
                                    getSelectedTraceTree()->GetRootItem() );
    else
#warning TODO: Gui Exception class inherited from ParaverKernelException
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

void gHistogram::OnPopUpFitSemanticScale(){}

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
    SetTitle( winTitle + _(" (Working...)") );
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
  
  THistogramColumn column = myHistogram->getHorizontal() ? floor( lastPosZoomX / zoomCellWidth ) :
                                                           floor( lastPosZoomY / zoomCellHeight );
  TObjectOrder row = myHistogram->getHorizontal() ? floor( lastPosZoomY / zoomCellHeight ) :
                                                    floor( lastPosZoomX / zoomCellWidth );
  if( row > 0 )
    text << _( myHistogram->getRowLabel( selectedRows[ row - 1 ] ).c_str() )
         << _( "  " );

  if( column > 0 )
    text << _( myHistogram->getColumnLabel( column - 1 ).c_str() )
         << _( "  " );
  
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

void gHistogram::OnToolOpenControlWindowClick( wxCommandEvent& event )
{
  openControlActivated = true;
  zoomHisto->SetCursor( *wxCROSS_CURSOR );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TOOL_OPEN_CONTROL_WINDOW
 */

void gHistogram::OnToolOpenControlWindowUpdate( wxUpdateUIEvent& event )
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
  columnEnd = myHistogram->getHorizontal() ? floor( xEnd / zoomCellWidth ) :
                                             floor( yEnd / zoomCellHeight );
  if( columnEnd > 0 ) --columnEnd;
  objectBegin = myHistogram->getHorizontal() ? floor( yBegin / zoomCellHeight ) :
                                               floor( xBegin / zoomCellWidth );
  if( objectBegin > 0 ) --objectBegin;
  objectBegin = selectedRows[ objectBegin ];
  objectEnd = myHistogram->getHorizontal() ? floor( yEnd / zoomCellHeight ) :
                                             floor( xEnd / zoomCellWidth );
  if( objectEnd > 0 ) --objectEnd;
  objectEnd = selectedRows[ objectEnd ];
}

void gHistogram::openControlWindow( THistogramColumn columnBegin, THistogramColumn columnEnd,
                                    TObjectOrder objectBegin, TObjectOrder objectEnd )
{
  bool found;
  gTimeline *tmpControlWindow = getGTimelineFromWindow( getAllTracesTree()->GetRootItem(), 
                                                        myHistogram->getControlWindow(),
                                                        found );
  if( !found )
    throw exception();
    
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
                                          getSelectedTraceTree()->GetRootItem(),
                                          false );
  }
  else
  {
    openWindow = tmpControlWindow->clone( controlCloned, 
                                          parent,
                                          getAllTracesTree()->GetRootItem(),
                                          getSelectedTraceTree()->GetRootItem(),
                                          false );
  }
  
  if( openWindow != NULL )
    openWindow->GetMyWindow()->setShowWindow( true );
  else
    throw exception();
}

