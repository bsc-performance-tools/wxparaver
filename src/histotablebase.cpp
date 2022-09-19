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

#include "histotablebase.h"
#include "ghistogram.h"
#include "labelconstructor.h"
#include "histogramtotals.h"
#include "window.h"


HistoTableBase::HistoTableBase()
{}


HistoTableBase::HistoTableBase( gHistogram* whichHisto ):
    myHisto( whichHisto )
{}


HistoTableBase::~HistoTableBase()
{}


int HistoTableBase::GetNumberRows()
{
  int extra = 0;
  if( myHisto->GetHistogram()->getFirstRowColored() ) ++extra;

  if( myHisto->GetHistogram()->getOnlyTotals() )
    return myHisto->GetHistogram()->getHorizontal() ? NUMTOTALS + extra : NUMTOTALS;
  
  if( myHisto->GetHistogram()->getHorizontal() )
    return myHisto->GetHistogram()->getNumRows() + NUMTOTALS + 1 + extra;

  if( myHisto->GetHistogram()->getHideColumns() )
    return noVoidSemRanges->size() + NUMTOTALS + 1;

  return myHisto->GetHistogram()->getNumColumns( myHisto->GetHistogram()->getCurrentStat() ) + NUMTOTALS + 1;
}


int HistoTableBase::GetNumberCols()
{
  int extra = 0;
  if( myHisto->GetHistogram()->getFirstRowColored() && !myHisto->GetHistogram()->getOnlyTotals() ) ++extra;

  if( myHisto->GetHistogram()->getHorizontal() )
  {
    if( myHisto->GetHistogram()->getHideColumns() )
      return noVoidSemRanges->size();
    else
      return myHisto->GetHistogram()->getNumColumns( myHisto->GetHistogram()->getCurrentStat() );
  }

  return myHisto->GetHistogram()->getNumRows() + extra;
}


int HistoTableBase::getNumSemanticColumns() const
{
  if ( myHisto->GetHistogram()->getHideColumns() )
    return noVoidSemRanges->size();

  return (int)myHisto->GetHistogram()->getNumColumns( myHisto->GetHistogram()->getCurrentStat() );
}


wxString HistoTableBase::GetRowLabelValue( int row )
{
  label.Clear();

  if( myHisto->GetHistogram()->getHorizontal() && myHisto->GetHistogram()->getFirstRowColored() )
  {
    if( row == 0 ) return wxT( "" );
    --row;
  }

  int tmpNumColumns = getNumSemanticColumns();

  if( myHisto->GetHistogram()->getOnlyTotals() )
  {
    label = wxString::FromUTF8( LabelConstructor::histoTotalLabel( (THistoTotals)( row ) ).c_str() );
  }
  else if( myHisto->GetHistogram()->getHorizontal() && row >= myHisto->GetHistogram()->getNumRows() )
  {
    int iTotal = row - myHisto->GetHistogram()->getNumRows();
    if( iTotal == 0 )
      label = wxT( "" );
    else
      label = wxString::FromUTF8( LabelConstructor::histoTotalLabel( (THistoTotals)( iTotal - 1 ) ).c_str() );
  }
  else if( !myHisto->GetHistogram()->getHorizontal() && row >= tmpNumColumns )
  {
    int iTotal = row - tmpNumColumns;
    if( iTotal == 0 )
      label = wxT( "" );
    else
      label = wxString::FromUTF8( LabelConstructor::histoTotalLabel( (THistoTotals)( iTotal - 1 ) ).c_str() );
  }
  else if( myHisto->GetHistogram()->getHorizontal() )
    label = wxString::FromUTF8( myHisto->GetHistogram()->getRowLabel( (*selectedRows)[ row ] ).c_str() );
  else
  {
    row = myHisto->GetHistogram()->getSemanticRealColumn( row, *noVoidSemRanges );
    label = wxString::FromUTF8( myHisto->GetHistogram()->getColumnLabel( row ).c_str() );
  }

  int w, h;
  wxFont tmpFont( GetView()->GetLabelFont() );
  GetView()->GetTextExtent( label, &w, &h, nullptr, nullptr, &tmpFont );
  if( !myHisto->GetHistogram()->getHorizontal() && myHisto->GetHistogram()->getFirstRowColored() && !myHisto->GetHistogram()->getOnlyTotals() )
    GetView()->SetRowLabelSize( 0 );
  else if( GetView()->GetRowLabelSize() == 0 || GetView()->GetRowLabelSize() - 5 < w )
    GetView()->SetRowLabelSize( w + 5 );

  return label;
}


wxString HistoTableBase::GetColLabelValue( int col )
{
  if( !myHisto->GetHistogram()->getHorizontal() && myHisto->GetHistogram()->getFirstRowColored() && !myHisto->GetHistogram()->getOnlyTotals() )
  {
    if( col == 0 ) return wxT( "" );
    --col;
  }

  if( myHisto->GetHistogram()->getHorizontal() )
  {
    col = myHisto->GetHistogram()->getSemanticRealColumn( col, *noVoidSemRanges );
 
    return wxString::FromUTF8( myHisto->GetHistogram()->getColumnLabel( col ).c_str() );
  }
  else
    return wxString::FromUTF8( myHisto->GetHistogram()->getRowLabel( (*selectedRows)[ col ] ).c_str() );
}


wxString HistoTableBase::GetValue( int row, int col )
{
  int drawCol = col;
  int drawRow = row;

  if( myHisto->GetHistogram()->getHorizontal() && myHisto->GetHistogram()->getFirstRowColored() )
  {
    if( row == 0 ) return GetColLabelValue( col );
    --row;
  }
  else if( !myHisto->GetHistogram()->getHorizontal() && myHisto->GetHistogram()->getFirstRowColored() && !myHisto->GetHistogram()->getOnlyTotals() )
  {
    if( col == 0 ) return GetRowLabelValue( row );
    --col;
  }

  if( !myHisto->GetHistogram()->getHorizontal() )
  {
    int tmp = row;
    row = col;
    col = tmp;
  }

  int tmpNumColumns = getNumSemanticColumns();

  if( myHisto->GetHistogram()->getHorizontal() && col >= tmpNumColumns )
    return "";
  
  PRV_UINT16 idStat;
  label.Clear();
  tmpStr.clear();
  if( !myHisto->GetHistogram()->getIdStat( myHisto->GetHistogram()->getCurrentStat(), idStat ) )
    throw( std::exception() );

  TSemanticValue semValue;
  if( ( myHisto->GetHistogram()->getHorizontal() && row >= myHisto->GetHistogram()->getNumRows() ) ||
      ( !myHisto->GetHistogram()->getHorizontal() && col >= tmpNumColumns ) ||
      myHisto->GetHistogram()->getOnlyTotals() )
  {
    int iTotal;
    if( myHisto->GetHistogram()->getOnlyTotals() )
      iTotal = myHisto->GetHistogram()->getHorizontal() ? row : col;
    else if( myHisto->GetHistogram()->getHorizontal() && row >= myHisto->GetHistogram()->getNumRows() )
      iTotal = row - myHisto->GetHistogram()->getNumRows() - 1;
    else
      iTotal = col - tmpNumColumns - 1;

    if( iTotal == -1 )
      label = wxString::FromUTF8( "" );
    else
    {
      HistogramTotals *totals = myHisto->GetHistogram()->getTotals( myHisto->GetHistogram()->getCurrentStat() );
      vector<TSemanticValue> vTotals;
      if( !myHisto->GetHistogram()->getHorizontal() )
        col = row;
      else
        col = myHisto->GetHistogram()->getSemanticSortedColumn( myHisto->GetHistogram()->getSemanticRealColumn( col, *noVoidSemRanges ) );

      if( myHisto->GetHistogram()->isCommunicationStat( myHisto->GetHistogram()->getCurrentStat() ) )
        totals->getAll( vTotals, idStat, col, myHisto->GetHistogram()->getCommSelectedPlane() );
      else
        totals->getAll( vTotals, idStat, col, myHisto->GetHistogram()->getSelectedPlane() );

      delete totals;

      // Paint total cells
      if( iTotal >= vTotals.size() )
        label = wxString::FromUTF8( "" );
      else if( vTotals[ 0 ] != 0.0 )
      {
        if( iTotal == AVGDIVMAX )
          label = wxString::FromUTF8( LabelConstructor::histoCellLabel( myHisto->GetHistogram(), vTotals[ iTotal ], false ).c_str());
        else
          label = wxString::FromUTF8( LabelConstructor::histoCellLabel( myHisto->GetHistogram(), vTotals[ iTotal ], true ).c_str());
      }
      else
      {
        label = wxString::FromUTF8( "-" );
      }
    }
  }
  else if( myHisto->GetHistogram()->isCommunicationStat( myHisto->GetHistogram()->getCurrentStat() ) )
  {
    col = myHisto->GetHistogram()->getSemanticRealColumn( col, *noVoidSemRanges );

    if( myHisto->GetHistogram()->getCommCellValue( semValue, row, col, idStat, myHisto->GetHistogram()->getCommSelectedPlane() ) )
    {
      tmpStr = LabelConstructor::histoCellLabel( myHisto->GetHistogram(), semValue, true );
      label = wxString::FromUTF8( tmpStr.c_str() );
    }
    else
      label = wxString::FromUTF8( "-" );
  }
  else if ( row < myHisto->GetHistogram()->getNumRows() && col < tmpNumColumns ) // the if is CKC's bug fix
  {
    col = myHisto->GetHistogram()->getSemanticRealColumn( col, *noVoidSemRanges );

    if( myHisto->GetHistogram()->getCellValue( semValue, row, col, idStat, myHisto->GetHistogram()->getSelectedPlane() ) )
    {
      if( !myHisto->GetHistogram()->isNotZeroStat( myHisto->GetHistogram()->getCurrentStat() ) ||
          myHisto->GetHistogram()->getNotZeroValue( row, col, idStat, myHisto->GetHistogram()->getSelectedPlane() ) )
      {
        tmpStr = LabelConstructor::histoCellLabel( myHisto->GetHistogram(), semValue, true );
        label = wxString::FromUTF8( tmpStr.c_str() );
      }
      else
        label = wxString::FromUTF8( "-" );
    }
    else
      label = wxString::FromUTF8( "-" );
  }
  else
    label = wxString::FromUTF8( "-" );

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
  Timeline *controlWindow = myHisto->GetHistogram()->getControlWindow();

  int tmpNumColumns = getNumSemanticColumns();

  if( myHisto->GetHistogram()->getHorizontal() && myHisto->GetHistogram()->getFirstRowColored() )
  {
    if( row == 0 )
    {
      tmpAttr->SetAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
      tmpAttr->SetFont( cellFontBold );

      col = myHisto->GetHistogram()->getSemanticRealColumn( col, *noVoidSemRanges );
      TSemanticValue tmpValue = ( myHisto->GetHistogram()->getSemanticSortedColumn( col ) * myHisto->GetHistogram()->getControlDelta() ) +
                                myHisto->GetHistogram()->getControlMin();
      rgb tmpCol;
      if( controlWindow->isCodeColorSet() )
        tmpCol = controlWindow->getCodeColor().calcColor( tmpValue,
                                                          myHisto->GetHistogram()->getControlMin(),
                                                          myHisto->GetHistogram()->getControlMax(),
                                                          controlWindow->getUseCustomPalette() );
      else
        tmpCol = controlWindow->getGradientColor().calcColor( tmpValue,
                                                              controlWindow->getMinimumY(),
                                                              controlWindow->getMaximumY() );
      tmpAttr->SetBackgroundColour( wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) );
      tmpAttr->SetTextColour( *getLuminance( wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) ) );
        
      return tmpAttr;
    }
    else if( myHisto->GetHistogram()->getOnlyTotals() )
      return tmpAttr;

    --row;
  }
  else if( myHisto->GetHistogram()->getOnlyTotals() )
  {
    return tmpAttr;
  }
  else if( !myHisto->GetHistogram()->getHorizontal() && myHisto->GetHistogram()->getFirstRowColored() )
  {
    if( col == 0 )
    {
      tmpAttr->SetAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
      tmpAttr->SetFont( cellFontBold );

      if( row < tmpNumColumns )
      {
        row = myHisto->GetHistogram()->getSemanticRealColumn( row, *noVoidSemRanges );
        TSemanticValue tmpValue = ( myHisto->GetHistogram()->getSemanticSortedColumn( row ) * myHisto->GetHistogram()->getControlDelta() ) +
                                  myHisto->GetHistogram()->getControlMin();
        rgb tmpCol;
        if( controlWindow->isCodeColorSet() )
          tmpCol = controlWindow->getCodeColor().calcColor( tmpValue,
                                                            myHisto->GetHistogram()->getControlMin(),
                                                            myHisto->GetHistogram()->getControlMax(),
                                                            controlWindow->getUseCustomPalette() );
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

  if( !myHisto->GetHistogram()->getHorizontal() )
  {
    int tmp = row;
    row = col;
    col = tmp;
  }

  PRV_UINT16 idStat;
  if( !myHisto->GetHistogram()->getIdStat( myHisto->GetHistogram()->getCurrentStat(), idStat ) )
    throw( std::exception() );

  TSemanticValue semValue;

  if( ( myHisto->GetHistogram()->getHorizontal() && row < myHisto->GetHistogram()->getNumRows() ) ||
      ( !myHisto->GetHistogram()->getHorizontal() && col < tmpNumColumns ) )
  {
    if ( myHisto->GetHistogram()->isCommunicationStat( myHisto->GetHistogram()->getCurrentStat() ) )
    {
      col = myHisto->GetHistogram()->getSemanticRealColumn( col, *noVoidSemRanges );

      if( myHisto->GetHistogram()->getCommCellValue( semValue, row, col, idStat, myHisto->GetHistogram()->getCommSelectedPlane() ) &&
          myHisto->GetHistogram()->getShowColor() )
      {
        rgb tmpCol = myHisto->GetHistogram()->calcGradientColor( semValue );
        tmpAttr->SetBackgroundColour( wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) );
        tmpAttr->SetTextColour( *getLuminance( wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) ) );
      }
    }
    else if ( row < myHisto->GetHistogram()->getNumRows() && col < tmpNumColumns )
    {
      col = myHisto->GetHistogram()->getSemanticRealColumn( col, *noVoidSemRanges );

      if( myHisto->GetHistogram()->getCellValue( semValue, row, col, idStat, myHisto->GetHistogram()->getSelectedPlane() ) && myHisto->GetHistogram()->getShowColor() )
      {
        rgb tmpCol;
        if( myHisto->GetHistogram()->getColorMode() == TColorFunction::COLOR )
        {
          tmpCol = myHisto->GetHistogram()->getDataWindow()->getCodeColor().calcColor( semValue,
                                                                       myHisto->GetHistogram()->getMinGradient(),
                                                                       myHisto->GetHistogram()->getMaxGradient(),
                                                                       myHisto->GetHistogram()->getDataWindow()->getUseCustomPalette() );
          tmpAttr->SetBackgroundColour( wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) );
          tmpAttr->SetTextColour( *getLuminance( wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) ) );
        }
        else
        {
          if( myHisto->GetHistogram()->getColorMode() == TColorFunction::GRADIENT || 
              ( myHisto->GetHistogram()->getColorMode() == TColorFunction::NOT_NULL_GRADIENT && semValue != 0.0 ) )
          {
            tmpCol = myHisto->GetHistogram()->calcGradientColor( semValue );
            tmpAttr->SetBackgroundColour( wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) );
            tmpAttr->SetTextColour( *getLuminance( wxColour( tmpCol.red, tmpCol.green, tmpCol.blue ) ) );
          }
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


void HistoTableBase::setNoVoidSemRanges( vector< THistogramColumn > *whichCols )
{
  noVoidSemRanges = whichCols;
}


void HistoTableBase::setDefaultFontBold( wxFont& whichFont )
{
  cellFontBold = whichFont;
}


const wxColour *HistoTableBase::getLuminance( wxColour fromColour ) const
{
  unsigned int BackColour_luminance = ( fromColour.Red()   * 30 ) / 100 +
                                      ( fromColour.Green() * 59 ) / 100 +
                                      ( fromColour.Blue()  * 11 ) / 100;
  if ( BackColour_luminance >= 128 )
    return wxBLACK;
  return wxWHITE;
}
