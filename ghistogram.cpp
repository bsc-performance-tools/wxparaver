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

#include <sstream>
#include "ghistogram.h"
#include "histogram.h"
#include "labelconstructor.h"
#include "histogramtotals.h"

////@begin XPM images
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
  EVT_IDLE( gHistogram::OnIdle )

////@end gHistogram event table entries

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
}


/*!
 * Member initialisation
 */

void gHistogram::Init()
{
////@begin gHistogram member initialisation
  myHistogram = NULL;
  gridHisto = NULL;
////@end gHistogram member initialisation
}


/*!
 * Control creation for gHistogram
 */

void gHistogram::CreateControls()
{    
////@begin gHistogram content construction
  gHistogram* itemFrame1 = this;

  gridHisto = new wxGrid( itemFrame1, ID_GRIDHISTO, wxDefaultPosition, itemFrame1->ConvertDialogToPixels(wxSize(200, 150)), wxHSCROLL|wxVSCROLL|wxALWAYS_SHOW_SB );
  gridHisto->SetDefaultColSize(50);
  gridHisto->SetDefaultRowSize(25);
  gridHisto->SetColLabelSize(25);
  gridHisto->SetRowLabelSize(50);
  gridHisto->CreateGrid(5, 5, wxGrid::wxGridSelectCells);

////@end gHistogram content construction
  gridHisto->EnableEditing( false );
  gridHisto->SetDefaultCellAlignment( wxALIGN_RIGHT, wxALIGN_CENTRE );
}


void gHistogram::execute()
{
  if( myHistogram == NULL )
    return;

  myHistogram->execute( myHistogram->getBeginTime(), myHistogram->getEndTime() );

  fillGrid();
  
  this->Refresh();
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
  
  if( !myHistogram->getIdStat( myHistogram->getCurrentStat(), idStat ) )
    throw( exception() );
    
  if( commStat )
    curPlane = myHistogram->getCommSelectedPlane();
  else
    curPlane = myHistogram->getSelectedPlane();

  numCols = myHistogram->getNumColumns( myHistogram->getCurrentStat() );
  numRows = myHistogram->getNumRows();
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
  
  gridHisto->BeginBatch();
  if( (THistogramColumn)gridHisto->GetNumberCols() != numDrawCols )
  {
    gridHisto->DeleteCols( 0, gridHisto->GetNumberCols() );
    gridHisto->AppendCols( numDrawCols );
  }
  if( gridHisto->GetNumberRows() != numDrawRows + NUMTOTALS + 1 )
  {
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
      
          gridHisto->GetTextExtent( myHistogram->getRowLabel( iRow ), &w, &h, NULL, NULL, &labelFont );
          if( rowLabelWidth == 0 || rowLabelWidth < w )
            rowLabelWidth = w;
          gridHisto->SetRowLabelValue( iRow, myHistogram->getRowLabel( iRow ) );
        }
      }
      else
      {
        gridHisto->SetColLabelValue( iRow, myHistogram->getRowLabel( iRow ) );
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
      
      if( commStat && myHistogram->endCommCell( iCol, curPlane ) ||
          !commStat && myHistogram->endCell( iCol, curPlane ) )
        gridHisto->SetCellValue( iDrawRow, iDrawCol, wxString( "-" ) );
      else
      {
        if( commStat )
        {
          if( myHistogram->getCommCurrentRow( iCol, curPlane ) == iRow )
          {
            string tmpStr;
            tmpStr = LabelConstructor::histoCellLabel( myHistogram,
              myHistogram->getCommCurrentValue( iCol, idStat, curPlane ) );
            gridHisto->SetCellValue( iDrawRow, iDrawCol, wxString( tmpStr ) );
            myHistogram->setCommNextCell( iCol, curPlane );
          }
          else gridHisto->SetCellValue( iDrawRow, iDrawCol, wxString( "-" ) );
        }
        else
        {
          if( myHistogram->getCurrentRow( iCol, curPlane ) == iRow )
          {
            string tmpStr;
            tmpStr = LabelConstructor::histoCellLabel( myHistogram,
              myHistogram->getCurrentValue( iCol, idStat, curPlane ) );
            gridHisto->SetCellValue( iDrawRow, iDrawCol, wxString( tmpStr ) );
            myHistogram->setNextCell( iCol, curPlane );
          }
          else gridHisto->SetCellValue( iDrawRow, iDrawCol, wxString( "-" ) );
        }
      }
    }
    
    gridHisto->SetRowLabelValue( numDrawRows, "" );
  }
  
  fillTotals( rowLabelWidth, numDrawRows + 1, curPlane, idStat );
  
  gridHisto->SetRowLabelSize( rowLabelWidth + 5 );
  gridHisto->Fit();
  gridHisto->AutoSize();
  gridHisto->EndBatch();
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
        tmpStr = LabelConstructor::histoCellLabel( myHistogram, totals[ i ] );
        gridHisto->SetCellValue( beginRow + i, iCol, wxString( tmpStr ) );
      }
      else gridHisto->SetCellValue( beginRow + i, iCol, wxString( "-" ) );
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
}

