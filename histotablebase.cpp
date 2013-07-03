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

#include "histotablebase.h"
#include "histogram.h"
#include "labelconstructor.h"
#include "histogramtotals.h"
#include "window.h"

HistoTableBase::HistoTableBase()
{}

HistoTableBase::HistoTableBase( Histogram* whichHisto ):
    myHisto( whichHisto )
{}

HistoTableBase::~HistoTableBase()
{}
  
int HistoTableBase::GetNumberRows()
{
  int extra = 0;
  if( myHisto->getFirstRowColored() ) ++extra;
  
  if( myHisto->getHorizontal() )
    return myHisto->getNumRows() + NUMTOTALS + 1 + extra;
    
  return myHisto->getNumColumns( myHisto->getCurrentStat() ) + NUMTOTALS + 1;
}

int HistoTableBase::GetNumberCols()
{
  int extra = 0;
  if( myHisto->getFirstRowColored() ) ++extra;

  if( myHisto->getHorizontal() )
    return myHisto->getNumColumns( myHisto->getCurrentStat() );

  return myHisto->getNumRows() + extra;
}

wxString HistoTableBase::GetRowLabelValue( int row )
{
  label.Clear();
  
  if( myHisto->getHorizontal() && myHisto->getFirstRowColored() )
  {
    if( row == 0 ) return wxT( "" );
    --row;
  }

  if( myHisto->getHorizontal() && row >= myHisto->getNumRows() )
  {
    int iTotal = row - myHisto->getNumRows();
    if( iTotal == 0 )
      label = wxT( "" );
    else
      label = wxString::FromAscii( LabelConstructor::histoTotalLabel( (THistoTotals)( iTotal - 1 ) ).c_str() );
  }
  else if( !myHisto->getHorizontal() && row >= (int)myHisto->getNumColumns( myHisto->getCurrentStat() ) )
  {
    int iTotal = row - (int)myHisto->getNumColumns( myHisto->getCurrentStat() );
    if( iTotal == 0 )
      label = wxT( "" );
    else
      label = wxString::FromAscii( LabelConstructor::histoTotalLabel( (THistoTotals)( iTotal - 1 ) ).c_str() );
  }
  else if( myHisto->itsCommunicationStat( myHisto->getCurrentStat() ) )
    label = wxString::FromAscii( myHisto->getRowLabel( (*selectedRows)[ row ] ).c_str() );
  else if( myHisto->getHorizontal() )
    label = wxString::FromAscii( myHisto->getRowLabel( (*selectedRows)[ row ] ).c_str() );
  else
    label = wxString::FromAscii( myHisto->getColumnLabel( row ).c_str() );
  
  int w, h;
  wxFont tmpFont( GetView()->GetLabelFont() );
  GetView()->GetTextExtent( label, &w, &h, NULL, NULL, &tmpFont );
  if( !myHisto->getHorizontal() && myHisto->getFirstRowColored() )
    GetView()->SetRowLabelSize( 0 );
  else if( GetView()->GetRowLabelSize() == 0 || GetView()->GetRowLabelSize() - 5 < w )
    GetView()->SetRowLabelSize( w + 5 );

  return label;
}

wxString HistoTableBase::GetColLabelValue( int col )
{
  if( !myHisto->getHorizontal() && myHisto->getFirstRowColored() )
  {
    if( col == 0 ) return wxT( "" );
    --col;
  }

  if( myHisto->itsCommunicationStat( myHisto->getCurrentStat() ) )
    return wxString::FromAscii( myHisto->getRowLabel( col ).c_str() );
    
  if( myHisto->getHorizontal() )
    return wxString::FromAscii( myHisto->getColumnLabel( col ).c_str() );
  else
    return wxString::FromAscii( myHisto->getRowLabel( (*selectedRows)[ col ] ).c_str() );
}

wxString HistoTableBase::GetValue( int row, int col )
{
  int drawCol = col;
  
  if( myHisto->getHorizontal() && myHisto->getFirstRowColored() )
  {
    if( row == 0 ) return GetColLabelValue( col );
    --row;
  }
  else if( !myHisto->getHorizontal() && myHisto->getFirstRowColored() )
  {
    if( col == 0 ) return GetRowLabelValue( row );
    --col;
  }

  if( !myHisto->getHorizontal() )
  {
    int tmp = row;
    row = col;
    col = tmp;
  }

  PRV_UINT16 idStat;
  label.Clear();
  tmpStr.clear();
  if( !myHisto->getIdStat( myHisto->getCurrentStat(), idStat ) )
    throw( std::exception() );

  TSemanticValue semValue;
  if( ( myHisto->getHorizontal() && row >= myHisto->getNumRows() ) ||
      ( !myHisto->getHorizontal() && col >= (int)myHisto->getNumColumns( myHisto->getCurrentStat() ) ) )
  {
    int iTotal;
    if( myHisto->getHorizontal() && row >= myHisto->getNumRows() )
      iTotal = row - myHisto->getNumRows() - 1;
    else
      iTotal = col - (int)myHisto->getNumColumns( myHisto->getCurrentStat() ) - 1;
    
    if( iTotal == -1 )
      label = wxString::FromAscii( "" );
    else
    {
      HistogramTotals *totals = myHisto->getTotals( myHisto->getCurrentStat() );
      vector<TSemanticValue> vTotals;
      if( !myHisto->getHorizontal() )
        col = row;
      if( myHisto->itsCommunicationStat( myHisto->getCurrentStat() ) )
        totals->getAll( vTotals, idStat, col, myHisto->getCommSelectedPlane() );
      else
        totals->getAll( vTotals, idStat, col, myHisto->getSelectedPlane() );

      if( vTotals[ 0 ] > 0.0 )
      {
        if( iTotal == AVGDIVMAX )
          label = wxString::FromAscii( LabelConstructor::histoCellLabel( myHisto, vTotals[ iTotal ], false ).c_str());
        else
          label = wxString::FromAscii( LabelConstructor::histoCellLabel( myHisto, vTotals[ iTotal ], true ).c_str());
      }
      else
      {
        if( myHisto->getHideColumns() )
        {
          if( myHisto->getHorizontal() )
            GetView()->SetColSize( drawCol, 0 );
        }
        label = wxString::FromAscii( "-" );
      }
    }
  }
  else if( myHisto->itsCommunicationStat( myHisto->getCurrentStat() ) )
  {
    if( myHisto->getCommCellValue( semValue, row, col, idStat, myHisto->getCommSelectedPlane() ) )
    {
      tmpStr = LabelConstructor::histoCellLabel( myHisto, semValue, true );
      label = wxString::FromAscii( tmpStr.c_str() );
    }
    else
      label = wxString::FromAscii( "-" );
  }
  else
  {
    if( myHisto->getCellValue( semValue, row, col, idStat, myHisto->getSelectedPlane() ) )
    {
      tmpStr = LabelConstructor::histoCellLabel( myHisto, semValue, true );
      label = wxString::FromAscii( tmpStr.c_str() );
    }
    else
      label = wxString::FromAscii( "-" );
  }

  return label;
}

void HistoTableBase::SetValue( int row, int col, const wxString &value )
{}

bool HistoTableBase::IsEmptyCell( int row, int col )
{
  return false;
}

wxGridCellAttr *HistoTableBase::GetAttr( int row, int col, wxGridCellAttr::wxAttrKind kind )
{
  wxGridCellAttr *tmpAttr = new wxGridCellAttr();
  Window *controlWindow = myHisto->getControlWindow();

  if( myHisto->getHorizontal() && myHisto->getFirstRowColored() )
  {
    if( row == 0 )
    {
      tmpAttr->SetAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
      tmpAttr->SetFont( cellFontBold );

      TSemanticValue tmpValue = ( col / myHisto->getControlDelta() ) +
                                myHisto->getControlMin();
      rgb tmpCol;
      if( controlWindow->isCodeColorSet() )
        tmpCol = controlWindow->getCodeColor().calcColor( tmpValue,
                                                          controlWindow->getMinimumY(),
                                                          controlWindow->getMaximumY() );
      else
        tmpCol = controlWindow->getGradientColor().calcColor( tmpValue,
                                                              controlWindow->getMinimumY(),
                                                              controlWindow->getMaximumY() );
      tmpAttr->SetBackgroundColour( wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) );
      tmpAttr->SetTextColour( *getLuminance( wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) ) );
        
      return tmpAttr;
    }
    --row;
  }
  else if( !myHisto->getHorizontal() && myHisto->getFirstRowColored() )
  {
    if( col == 0 )
    {
      tmpAttr->SetAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
      tmpAttr->SetFont( cellFontBold );

      if( row < (int)myHisto->getNumColumns( myHisto->getCurrentStat() ) )
      {
        TSemanticValue tmpValue = ( row / myHisto->getControlDelta() ) +
                                  myHisto->getControlMin();
        rgb tmpCol;
        if( controlWindow->isCodeColorSet() )
          tmpCol = controlWindow->getCodeColor().calcColor( tmpValue,
                                                            controlWindow->getMinimumY(),
                                                            controlWindow->getMaximumY() );
        else
          tmpCol = controlWindow->getGradientColor().calcColor( tmpValue,
                                                                controlWindow->getMinimumY(),
                                                                controlWindow->getMaximumY() );
        tmpAttr->SetBackgroundColour( wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) );
        tmpAttr->SetTextColour( *getLuminance( wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) ) );
      }
      return tmpAttr;
    }
    --col;
  }

  if( !myHisto->getHorizontal() )
  {
    int tmp = row;
    row = col;
    col = tmp;
  }

  PRV_UINT16 idStat;
  if( !myHisto->getIdStat( myHisto->getCurrentStat(), idStat ) )
    throw( std::exception() );
  TSemanticValue semValue;
  if( ( myHisto->getHorizontal() && row < myHisto->getNumRows() ) ||
    ( !myHisto->getHorizontal() && col < (int)myHisto->getNumColumns( myHisto->getCurrentStat() ) ) )
  {
    if( myHisto->itsCommunicationStat( myHisto->getCurrentStat() ) )
    {
      if( myHisto->getCommCellValue( semValue, row, col, idStat, myHisto->getCommSelectedPlane() ) )
      {
        if( myHisto->getShowColor() )
        {
          rgb tmpCol = myHisto->calcGradientColor( semValue );
          tmpAttr->SetBackgroundColour( wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) );
          tmpAttr->SetTextColour( *getLuminance( wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) ) );
        }
      }
    }
    else
    {
      if( myHisto->getCellValue( semValue, row, col, idStat, myHisto->getSelectedPlane() ) )
      {
        if( myHisto->getShowColor() )
        {
          rgb tmpCol;
          if( myHisto->getCodeColor() )
          {
            tmpCol = myHisto->getDataWindow()->getCodeColor().calcColor( semValue,
                                                                         myHisto->getMinGradient(),
                                                                         myHisto->getMaxGradient() );
            tmpAttr->SetBackgroundColour( wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) );
          }
          else
          {
            tmpCol = myHisto->calcGradientColor( semValue );
            tmpAttr->SetBackgroundColour( wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) );
          }
          tmpAttr->SetTextColour( *getLuminance( wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) ) );
        }
      }
    }
  }
  
  return tmpAttr;
}

void HistoTableBase::setSelectedRows( vector<TObjectOrder> *whichRows )
{
  selectedRows = whichRows;
}

void HistoTableBase::setDefaultFontBold( wxFont& whichFont )
{
  cellFontBold = whichFont;
}

const wxColour *HistoTableBase::getLuminance( wxColour fromColour ) const
{
  unsigned int BackColour_luminance = (fromColour.Red() * 30)/100 +
                                      (fromColour.Green() * 59)/100 +
                                      (fromColour.Blue() * 11) / 100;
  if (BackColour_luminance >= 128)
    return wxBLACK;
  return wxWHITE;
}
