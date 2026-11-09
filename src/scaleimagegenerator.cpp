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

/**
 * @file scaleimagegenerator.cpp
 * @brief Implementation of ScaleImageGenerator, the renderer of colour legends.
 *
 * Rendering a legend is a pipeline that runs entirely inside generateImage():
 *
 *   1. Reset the layout metrics and read the value range from the source window.
 *   2. For gradients, sample the value range and rebuild the labels.
 *   3. Pick the labels to draw and compute the image size.
 *   4. Draw on a new bitmap (and on its mask) and publish it.
 *
 * The device contexts only exist while drawing, so the published bitmap is never
 * left selected in a device context and can be safely previewed by the Save
 * Image dialog, composed by WindowImageGenerator, saved or copied.
 */

#include "scaleimagegenerator.h"

#include "histogram.h"
#include "labelconstructor.h"
#include "trace.h"
#include "window.h"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>
#include <string>
#include <wx/brush.h>
#include <wx/clipbrd.h>
#include <wx/colour.h>
#include <wx/dataobj.h>
#include <wx/dcmemory.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/pen.h>

namespace
{
  // Length of the tick mark that joins a gradient square with its label,
  // and gap between an outlier square and its mark.
  constexpr int LABEL_TICK_LENGTH = 4;

  // Squares of a gradient legend that is not fitted to a target length.
  constexpr int DEFAULT_GRADIENT_SQUARES = 21;

  // Decimals of the numeric labels.
  constexpr int SEMANTIC_LABEL_PRECISION = 2;

  constexpr int LEGEND_MARGIN        = 5;
  constexpr int DEFAULT_SQUARE_WIDTH = 10;

  // Minimum free space between the middle and the last label of a horizontal gradient.
  constexpr int MINIMUM_LABELS_GAP = 5;

  // Size of the scratch bitmap used to measure text before the legend bitmap exists.
  constexpr int TEXT_MEASURE_BITMAP_WIDTH  = 1920;
  constexpr int TEXT_MEASURE_BITMAP_HEIGHT = 64;


  /**
   * @brief Builds the file name suffix that identifies a kind of legend.
   * @param whichScaleType Legend type.
   * @return The suffix, without separators.
   */
  wxString buildLegendSuffix( TScaleImageType whichScaleType )
  {
    switch( whichScaleType )
    {
      case TScaleImageType::TScaleImageGradientColor:
        return _( "gradient_legend" );
      case TScaleImageType::TScaleImageGradientColorNotNull:
        return _( "nn_gradient_legend" );
      case TScaleImageType::TScaleImageAlternativeGradientColor:
        return _( "alt_gradient_legend" );
      default:
        return _( "code_legend" );
    }
  }
} // namespace


bool isGradientScaleType( TScaleImageType whichScaleType )
{
  return whichScaleType == TScaleImageType::TScaleImageGradientColor || whichScaleType == TScaleImageType::TScaleImageGradientColorNotNull ||
         whichScaleType == TScaleImageType::TScaleImageAlternativeGradientColor;
}


wxBitmapType imageFormatTypeToBitmapType( const TImageFormat& whichFilterIndex )
{
  switch( whichFilterIndex )
  {
    case TImageFormat::BMP:
      return wxBITMAP_TYPE_BMP;
    case TImageFormat::JPG:
      return wxBITMAP_TYPE_JPEG;
    case TImageFormat::PNG:
      return wxBITMAP_TYPE_PNG;
    case TImageFormat::XPM:
      return wxBITMAP_TYPE_XPM;
    default:
      return wxBITMAP_TYPE_PNG;
  }
}


bool copyBitmapToClipboard( const wxBitmap& whichBitmap )
{
  if( !whichBitmap.IsOk() || !wxTheClipboard->Open() )
    return false;

  // The clipboard takes ownership of the data object.
  const bool copyDone = wxTheClipboard->SetData( new wxBitmapDataObject( whichBitmap ) );
  wxTheClipboard->Close();

  return copyDone;
}


/**
 * @brief Pair of device contexts (visible legend and its mask) drawn in lockstep.
 *
 * Everything drawn on the legend is replicated in black on the mask, so the
 * mask ends up holding the silhouette needed for transparent backgrounds.
 */
struct ScaleImageGenerator::TCanvas
{
    wxMemoryDC& content; // Visible legend
    wxMemoryDC& mask;    // Silhouette of everything drawn on the legend

    /**
     * @brief Draws a text on the legend and on its mask.
     * @param text Text to draw.
     * @param posX Horizontal position of the text.
     * @param posY Vertical position of the text.
     */
    void drawText( const wxString& text, int posX, int posY )
    {
      content.DrawText( text, posX, posY );
      mask.DrawText( text, posX, posY );
    }

    /**
     * @brief Draws a line on the legend and on its mask.
     * @param beginX Horizontal position of the first point.
     * @param beginY Vertical position of the first point.
     * @param endX Horizontal position of the last point.
     * @param endY Vertical position of the last point.
     */
    void drawLine( int beginX, int beginY, int endX, int endY )
    {
      content.DrawLine( beginX, beginY, endX, endY );
      mask.DrawLine( beginX, beginY, endX, endY );
    }

    /**
     * @brief Draws a filled rectangle on the legend and its silhouette on the mask.
     * @param fillColour Colour of the rectangle on the legend.
     * @param posX Horizontal origin of the rectangle.
     * @param posY Vertical origin of the rectangle.
     * @param width Width of the rectangle.
     * @param height Height of the rectangle.
     */
    void drawRectangle( const wxColour& fillColour, int posX, int posY, int width, int height )
    {
      content.SetBrush( wxBrush( fillColour ) );
      content.DrawRectangle( posX, posY, width, height );
      mask.DrawRectangle( posX, posY, width, height );
    }

    /**
     * @brief Measures a text with the font of the legend.
     * @param text Text to measure.
     * @return Extent of the text in pixels.
     */
    wxSize getTextExtent( const wxString& text ) const
    {
      return content.GetTextExtent( text );
    }
};


ScaleImageGenerator::ScaleImageGenerator( Histogram* whichHistogram,
                                          Timeline* whichDataWindow,
                                          const std::map< TSemanticValue, rgb >& whichSemanticValueColors,
                                          const wxFont& whichTextFont,
                                          TScaleImageType whichScaleType,
                                          TScaleImageOrientation whichOrientation,
                                          TBackgroundMode whichBackgroundMode )
  : ScaleImageGenerator( WindowGenericItem( whichHistogram ),
                         whichDataWindow,
                         whichSemanticValueColors,
                         whichTextFont,
                         whichScaleType,
                         whichOrientation,
                         whichBackgroundMode )
{}


ScaleImageGenerator::ScaleImageGenerator( Timeline* whichTimeline,
                                          const std::map< TSemanticValue, rgb >& whichSemanticValueColors,
                                          const wxFont& whichTextFont,
                                          TScaleImageType whichScaleType,
                                          TScaleImageOrientation whichOrientation,
                                          TBackgroundMode whichBackgroundMode )
  : ScaleImageGenerator( WindowGenericItem( whichTimeline ),
                         whichTimeline,
                         whichSemanticValueColors,
                         whichTextFont,
                         whichScaleType,
                         whichOrientation,
                         whichBackgroundMode )
{}


ScaleImageGenerator::ScaleImageGenerator( WindowGenericItem whichSourceWindow,
                                          const Timeline* whichLabelWindow,
                                          const std::map< TSemanticValue, rgb >& whichSemanticValueColors,
                                          const wxFont& whichTextFont,
                                          TScaleImageType whichScaleType,
                                          TScaleImageOrientation whichOrientation,
                                          TBackgroundMode whichBackgroundMode )
  : sourceWindow( whichSourceWindow )
  , labelWindow( whichLabelWindow )
  , semanticValuesColors( whichSemanticValueColors )
  , scaleType( whichScaleType )
  , orientation( whichOrientation )
  , gradientSquareCount( DEFAULT_GRADIENT_SQUARES )
  , legendSuffix( buildLegendSuffix( whichScaleType ) )
{
  layout.textFont       = whichTextFont;
  layout.backgroundMode = whichBackgroundMode;
  layout.labelPrecision = SEMANTIC_LABEL_PRECISION;

  // The layout decides whether labels are symbolic, so it goes before the labels.
  initializeLayout();
  computeSemanticLabels();
  selectAllSemanticValues();
}


void ScaleImageGenerator::generateImage( std::optional< int > targetLength )
{
  initializeLayout();

  const bool gradientLegend = isGradientScaleType( scaleType );
  if( gradientLegend )
  {
    // A gradient is redrawn from the value range of the window, sampled with as
    // many squares as fit in the target length.
    gradientSquareCount = DEFAULT_GRADIENT_SQUARES;
    if( targetLength.has_value() )
      gradientSquareCount = std::max( 1, targetLength.value() / std::max( 1, layout.squareHeight ) );

    computeGradientColors();
    computeSemanticLabels();
    selectAllSemanticValues();
  }

  selectVisibleLabels();
  computeImageSize( targetLength );

  wxBitmap newLegendBitmap( layout.imageWidth, layout.imageHeight );
  wxBitmap newMaskBitmap( layout.imageWidth, layout.imageHeight );
  if( !newLegendBitmap.IsOk() || !newMaskBitmap.IsOk() )
  {
    legendBitmap     = wxBitmap();
    legendMaskBitmap = wxBitmap();
    return;
  }

  {
    // The device contexts are released before the bitmaps are published.
    wxMemoryDC contentDC( newLegendBitmap );
    wxMemoryDC maskDC( newMaskBitmap );
    TCanvas canvas{ contentDC, maskDC };

    prepareCanvas( canvas );

    if( gradientLegend )
      drawImageGradientColor( canvas );
    else
      drawImageCodeColor( canvas );
  }

  legendBitmap     = newLegendBitmap;
  legendMaskBitmap = newMaskBitmap;
}


wxBitmap* ScaleImageGenerator::getBitmap()
{
  return legendBitmap.IsOk() ? &legendBitmap : nullptr;
}


TScaleImageType ScaleImageGenerator::getLegendType() const
{
  return scaleType;
}


const wxString& ScaleImageGenerator::getLegendSuffix() const
{
  return legendSuffix;
}


void ScaleImageGenerator::setImageOrientation( TScaleImageOrientation whichOrientation )
{
  orientation = whichOrientation;
}


void ScaleImageGenerator::setShortLabels( bool whichShortLabels )
{
  shortLabels = whichShortLabels;
}


const std::map< TSemanticValue, wxString >& ScaleImageGenerator::getSemanticValues() const
{
  return shortLabels ? semanticValuesLabelsShort : semanticValuesLabelsLong;
}


void ScaleImageGenerator::setSelectedSemanticValues( const std::vector< int >& whichSelectedPositions )
{
  selectedLabelPositions = whichSelectedPositions;
}


const std::vector< int >& ScaleImageGenerator::getSelectedSemanticValues() const
{
  return selectedLabelPositions;
}


bool ScaleImageGenerator::saveImage( const wxString& whichFilePath, TImageFormat whichImageFormat )
{
  if( !legendBitmap.IsOk() )
    return false;

  applyTransparencyMask();

  return legendBitmap.ConvertToImage().SaveFile( whichFilePath, imageFormatTypeToBitmapType( whichImageFormat ) );
}


bool ScaleImageGenerator::copyImageToClipboard()
{
  if( !legendBitmap.IsOk() )
    return false;

  applyTransparencyMask();

  return copyBitmapToClipboard( legendBitmap );
}


void ScaleImageGenerator::initializeLayout()
{
  minimumValue = std::visit( overloads{ []( Timeline* timeline )
                                        {
                                          return timeline->getMinimumY();
                                        },
                                        []( Histogram* histogram )
                                        {
                                          return histogram->getMinGradient();
                                        } },
                             sourceWindow );
  maximumValue = std::visit( overloads{ []( Timeline* timeline )
                                        {
                                          return timeline->getMaximumY();
                                        },
                                        []( Histogram* histogram )
                                        {
                                          return histogram->getMaxGradient();
                                        } },
                             sourceWindow );

  if( scaleType == TScaleImageType::TScaleImageGradientFusedLines )
  {
    // Fused lines map objects, not semantic values, to colours.
    if( auto timeline = std::get_if< Timeline* >( &sourceWindow ) )
    {
      minimumValue = 0;
      maximumValue = ( *timeline )->getTrace()->getLevelObjects( ( *timeline )->getLevel() );
    }
  }

  // Gradients show numeric values and outliers; the other legends show names.
  const bool gradientLegend = isGradientScaleType( scaleType );
  layout.drawOutliers       = gradientLegend;
  layout.symbolicLabels     = !gradientLegend;

  layout.margin        = LEGEND_MARGIN;
  layout.squareSpacing = 2 * layout.margin;

  layout.squareWidth  = DEFAULT_SQUARE_WIDTH;
  layout.squareHeight = layout.textFont.GetPointSize() + 3;
  layout.imageHeight  = layout.textFont.GetPointSize() + 2 * layout.margin;
  layout.imageWidth   = layout.squareWidth;
}


void ScaleImageGenerator::computeSemanticLabels()
{
  semanticValuesLabelsLong.clear();
  semanticValuesLabelsShort.clear();

  for( const auto& valueColour : semanticValuesColors )
  {
    const TSemanticValue semanticValue = valueColour.first;

    std::string longLabel;
    if( scaleType != TScaleImageType::TScaleImageGradientFusedLines )
    {
      longLabel = LabelConstructor::semanticLabel( labelWindow, semanticValue, layout.symbolicLabels, layout.labelPrecision, false );
    }
    else if( auto timeline = std::get_if< Timeline* >( &sourceWindow ) )
    {
      longLabel = LabelConstructor::objectLabel( static_cast< TObjectOrder >( semanticValue ), ( *timeline )->getLevel(), ( *timeline )->getTrace() );
    }

    semanticValuesLabelsLong[ semanticValue ]  = wxString::FromUTF8( longLabel.c_str() );
    semanticValuesLabelsShort[ semanticValue ] = wxString::FromUTF8( LabelConstructor::transformToShort( longLabel ).c_str() );
  }
}


void ScaleImageGenerator::selectAllSemanticValues()
{
  selectedLabelPositions.resize( semanticValuesColors.size() );
  std::iota( selectedLabelPositions.begin(), selectedLabelPositions.end(), 0 );
}


void ScaleImageGenerator::computeGradientColors()
{
  semanticValuesColors.clear();

  const TSemanticValue rangeMinimum = minimumValue;
  const TSemanticValue rangeMaximum = maximumValue;
  const TSemanticValue sampleStep   = gradientSquareCount > 1 ? ( rangeMaximum - rangeMinimum ) / double( gradientSquareCount - 1 ) : 0.0;

  for( int sample = 0; sample < gradientSquareCount; ++sample )
  {
    TSemanticValue sampleValue = ( sample * sampleStep ) + rangeMinimum;
    if( sampleValue > rangeMaximum )
      sampleValue = rangeMaximum;

    semanticValuesColors[ sampleValue ] = std::visit( overloads{ [ sampleValue ]( Histogram* histogram )
                                                                 {
                                                                   return histogram->calcGradientColor( sampleValue );
                                                                 },
                                                                 [ sampleValue, rangeMinimum, rangeMaximum ]( Timeline* timeline )
                                                                 {
                                                                   return timeline->getSemanticColor().calcColor( sampleValue,
                                                                                                                  rangeMinimum,
                                                                                                                  rangeMaximum );
                                                                 } },
                                                      sourceWindow );
  }
}


void ScaleImageGenerator::selectVisibleLabels()
{
  const std::map< TSemanticValue, wxString >& allLabels = getSemanticValues();

  visibleLabels.clear();

  const wxString* longestLabel = nullptr;
  int labelPosition            = 0;
  for( const auto& valueLabel : allLabels )
  {
    if( std::find( selectedLabelPositions.begin(), selectedLabelPositions.end(), labelPosition ) != selectedLabelPositions.end() )
    {
      const auto visibleLabel = visibleLabels.insert( valueLabel ).first;
      if( longestLabel == nullptr || visibleLabel->second.Len() > longestLabel->Len() )
        longestLabel = &visibleLabel->second;
    }
    ++labelPosition;
  }

  if( longestLabel == nullptr )
  {
    maxLabelSize = wxSize( 0, 0 );
    return;
  }

  wxBitmap measureBitmap( TEXT_MEASURE_BITMAP_WIDTH, TEXT_MEASURE_BITMAP_HEIGHT );
  wxMemoryDC measureDC( measureBitmap );
  measureDC.SetFont( layout.textFont );
  maxLabelSize = measureDC.GetTextExtent( *longestLabel );
}


void ScaleImageGenerator::computeImageSize( std::optional< int > targetLength )
{
  const bool gradientLegend = isGradientScaleType( scaleType );

  if( orientation == TScaleImageOrientation::TScaleImageOrientationVertical )
  {
    if( gradientLegend )
    {
      layout.imageHeight = layout.squareHeight * gradientSquareCount + 2 * layout.margin;

      if( layout.drawOutliers )
        layout.imageHeight += ( layout.squareHeight + layout.margin ) * 2 + 2 * layout.margin;
    }
    else
    {
      const int visibleLabelCount = static_cast< int >( visibleLabels.size() );

      layout.imageHeight = ( visibleLabelCount - 1 ) * ( layout.squareHeight + layout.squareSpacing ) + layout.squareHeight + 2 * layout.margin;

      if( layout.drawOutliers )
        layout.imageHeight += ( layout.squareHeight + layout.margin ) * 2;
    }

    layout.imageWidth = maxLabelSize.GetWidth() + ( 2 * layout.margin ) + layout.squareWidth + 2 * layout.squareSpacing;
  }
  else
  {
    if( gradientLegend )
    {
      layout.imageHeight = layout.squareHeight + maxLabelSize.GetHeight() + 3 * layout.margin;

      const int targetWidth = targetLength.value_or( 0 );
      if( targetWidth != 0 )
      {
        // Fit the bar, including both outlier squares, in the requested width.
        layout.squareWidth = ( targetWidth - 2 * layout.margin - 2 * layout.squareSpacing ) / ( gradientSquareCount + 2 );
        layout.imageWidth  = targetWidth;
      }
      else
      {
        layout.imageWidth = ( layout.squareWidth * gradientSquareCount + 2 * layout.squareSpacing + 2 * layout.margin ) + 2 * layout.squareSpacing +
                            2 * layout.squareWidth;
      }
    }
    else
    {
      layout.imageHeight = layout.squareHeight + 2 * layout.margin;
      layout.imageWidth  = 0;

      wxBitmap measureBitmap( TEXT_MEASURE_BITMAP_WIDTH, TEXT_MEASURE_BITMAP_HEIGHT );
      wxMemoryDC measureDC( measureBitmap );
      measureDC.SetFont( layout.textFont );

      for( const auto& valueLabel : visibleLabels )
      {
        const wxSize labelSize = measureDC.GetTextExtent( valueLabel.second );
        layout.imageWidth += 3 * layout.squareSpacing + layout.squareHeight + labelSize.GetWidth();
      }
      layout.imageWidth += 2 * layout.squareSpacing;
    }
  }

  // A legend without visible values still needs a valid (non-empty) bitmap.
  layout.imageWidth  = std::max( 1, layout.imageWidth );
  layout.imageHeight = std::max( 1, layout.imageHeight );
}


void ScaleImageGenerator::prepareCanvas( TCanvas& canvas ) const
{
  canvas.content.SetFont( layout.textFont );

  if( layout.backgroundMode == TBackgroundMode::BACKGROUND_SOLID_DARK )
  {
    canvas.content.SetBackground( wxBrush( *wxBLACK ) );
    canvas.content.SetTextBackground( *wxWHITE );
    canvas.content.SetTextForeground( *wxWHITE );
    canvas.content.SetPen( wxPen( *wxWHITE ) );
  }
  else
  {
    canvas.content.SetBackground( *wxWHITE_BRUSH );
    canvas.content.SetTextForeground( *wxBLACK );
  }

  canvas.content.Clear();

  canvas.mask.SetBackground( *wxWHITE_BRUSH );
  canvas.mask.SetBrush( *wxBLACK_BRUSH );
  canvas.mask.SetPen( *wxBLACK_PEN );
  canvas.mask.SetTextForeground( *wxBLACK );
  canvas.mask.SetFont( layout.textFont );
  canvas.mask.Clear();
}


void ScaleImageGenerator::drawImageCodeColor( TCanvas& canvas ) const
{
  const bool verticalLegend = orientation == TScaleImageOrientation::TScaleImageOrientationVertical;

  int posX = layout.squareSpacing + layout.margin;
  int posY = layout.margin;

  for( const auto& valueLabel : visibleLabels )
  {
    const auto valueColour = semanticValuesColors.find( valueLabel.first );
    drawSquare( canvas, valueColour != semanticValuesColors.end() ? valueColour->second : rgb(), posX, posY );

    posX += layout.squareWidth + layout.squareSpacing;

    const wxSize labelSize = canvas.getTextExtent( valueLabel.second );
    canvas.drawText( valueLabel.second, posX, posY + ( layout.squareHeight - labelSize.GetHeight() ) / 2 );

    if( verticalLegend )
    {
      posX = layout.squareSpacing + layout.margin;
      posY += layout.squareSpacing + layout.squareWidth;
    }
    else
    {
      posX += layout.squareSpacing + layout.squareHeight + labelSize.GetWidth();
    }
  }
}


void ScaleImageGenerator::drawImageGradientColor( TCanvas& canvas ) const
{
  const bool verticalLegend  = orientation == TScaleImageOrientation::TScaleImageOrientationVertical;
  const wxString outlierMark = wxT( "<" );

  const rgb belowOutlierColour = std::visit(
    []( auto* window )
    {
      return window->getSemanticColor().getBelowOutlierColor();
    },
    sourceWindow );
  const rgb aboveOutlierColour = std::visit(
    []( auto* window )
    {
      return window->getSemanticColor().getAboveOutlierColor();
    },
    sourceWindow );

  int posX = layout.squareSpacing + layout.margin;
  int posY = layout.margin;

  // *** Square and mark of the outliers below the range **********************
  drawSquare( canvas, belowOutlierColour, posX, posY );

  if( verticalLegend )
  {
    canvas.drawText( outlierMark, posX + layout.squareHeight + LABEL_TICK_LENGTH, posY );
    posY += layout.squareHeight + layout.squareSpacing;
  }
  else
  {
    canvas.drawText( outlierMark, posX, posY + layout.squareHeight + LABEL_TICK_LENGTH );
    posX += layout.squareWidth + layout.squareSpacing;
  }

  // *** Gradient bar *********************************************************
  // Only the first, middle and last squares are labelled.
  const int middleElement    = ( gradientSquareCount / 2 ) + 1;
  const auto labelPositionOf = [ this, middleElement ]( std::map< TSemanticValue, rgb >::const_iterator whichSquare,
                                                        int whichElement ) -> std::optional< TGradientLabelPosition >
  {
    if( whichSquare == semanticValuesColors.begin() )
      return TGradientLabelPosition::FIRST;
    if( std::next( whichSquare ) == semanticValuesColors.end() )
      return TGradientLabelPosition::LAST;
    if( whichElement == middleElement )
      return TGradientLabelPosition::MIDDLE;
    return std::nullopt;
  };

  TGradientLabelPlacements labelPlacements;
  {
    int currentElement = 0;
    int squarePosition = posX;
    for( auto it = semanticValuesColors.begin(); it != semanticValuesColors.end(); ++it )
    {
      ++currentElement;

      const std::optional< TGradientLabelPosition > labelPosition = labelPositionOf( it, currentElement );
      if( labelPosition.has_value() )
        labelPlacements[ labelPosition.value() ] =
          computeGradientLabelPlacement( canvas, getShortLabel( it->first ), labelPosition.value(), squarePosition );

      if( verticalLegend )
        squarePosition = layout.squareSpacing + layout.margin;
      else
        squarePosition += layout.squareSpacing;
    }
  }

  // The middle label of a horizontal bar is dropped if it would run into the last one.
  bool writeMiddleLabel      = verticalLegend;
  const auto middlePlacement = labelPlacements.find( TGradientLabelPosition::MIDDLE );
  const auto lastPlacement   = labelPlacements.find( TGradientLabelPosition::LAST );
  if( !verticalLegend && middlePlacement != labelPlacements.end() && lastPlacement != labelPlacements.end() )
  {
    const int middleLabelFinalPos = middlePlacement->second.squarePosition - middlePlacement->second.textOffset;
    const int lastLabelInitialPos = lastPlacement->second.squarePosition + lastPlacement->second.textOffset;

    writeMiddleLabel = middleLabelFinalPos < ( lastLabelInitialPos - MINIMUM_LABELS_GAP );
  }

  int currentElement = 0;
  for( auto it = semanticValuesColors.begin(); it != semanticValuesColors.end(); ++it )
  {
    drawSquare( canvas, it->second, posX, posY );

    ++currentElement;

    const std::optional< TGradientLabelPosition > labelPosition = labelPositionOf( it, currentElement );
    if( labelPosition.has_value() && ( labelPosition.value() != TGradientLabelPosition::MIDDLE || writeMiddleLabel ) )
      drawLabelGradient( canvas, getShortLabel( it->first ), labelPlacements.at( labelPosition.value() ), posX, posY );

    if( verticalLegend )
    {
      posX = layout.squareSpacing + layout.margin;
      posY += layout.squareSpacing;
    }
    else
    {
      posX += layout.squareSpacing;
    }
  }

  // *** Square and mark of the outliers above the range **********************
  const wxSize outlierMarkSize = canvas.getTextExtent( outlierMark );

  if( verticalLegend )
  {
    posX = layout.squareSpacing + layout.margin;
    posY += layout.squareSpacing;

    drawSquare( canvas, aboveOutlierColour, posX, posY );
    canvas.drawText( outlierMark, posX + layout.squareWidth + LABEL_TICK_LENGTH, posY + layout.squareHeight - outlierMarkSize.GetHeight() );
  }
  else
  {
    posX += layout.squareSpacing;

    drawSquare( canvas, aboveOutlierColour, posX, posY );
    canvas.drawText( outlierMark, posX + layout.squareWidth - outlierMarkSize.GetWidth(), posY + layout.squareHeight + LABEL_TICK_LENGTH );
  }
}


ScaleImageGenerator::TGradientLabelPlacement ScaleImageGenerator::computeGradientLabelPlacement( TCanvas& canvas,
                                                                                                 const wxString& semanticValueLabel,
                                                                                                 TGradientLabelPosition labelPosition,
                                                                                                 int squarePosition ) const
{
  const wxSize labelSize = canvas.getTextExtent( semanticValueLabel );

  TGradientLabelPlacement labelPlacement;
  labelPlacement.squarePosition = squarePosition;

  switch( labelPosition )
  {
    case TGradientLabelPosition::FIRST:
      // Tick and text start at the origin of the square.
      break;
    case TGradientLabelPosition::MIDDLE:
      // Tick and text are centered on the square.
      labelPlacement.tickOffset = static_cast< int >( std::ceil( layout.squareWidth / 2.0 ) );
      labelPlacement.textOffset = static_cast< int >( std::ceil( layout.squareWidth / 2.0 - labelSize.GetWidth() / 2.0 ) );
      break;
    case TGradientLabelPosition::LAST:
      // Tick and text end where the square ends.
      labelPlacement.tickOffset = layout.squareWidth - 1;
      labelPlacement.textOffset = layout.squareWidth - labelSize.GetWidth();
      break;
  }

  return labelPlacement;
}


void ScaleImageGenerator::drawLabelGradient( TCanvas& canvas,
                                             const wxString& semanticValueLabel,
                                             const TGradientLabelPlacement& labelPlacement,
                                             int posX,
                                             int posY ) const
{
  if( orientation == TScaleImageOrientation::TScaleImageOrientationHorizontal )
  {
    canvas.drawLine( posX + labelPlacement.tickOffset,
                     posY + layout.squareHeight,
                     posX + labelPlacement.tickOffset,
                     posY + layout.squareHeight + LABEL_TICK_LENGTH );

    canvas.drawText( semanticValueLabel, posX + labelPlacement.textOffset, posY + layout.squareHeight + LABEL_TICK_LENGTH + 1 );
  }
  else
  {
    canvas.drawLine( posX + layout.squareWidth,
                     posY + labelPlacement.tickOffset,
                     posX + layout.squareWidth + LABEL_TICK_LENGTH,
                     posY + labelPlacement.tickOffset );

    canvas.drawText( semanticValueLabel, posX + layout.squareWidth + LABEL_TICK_LENGTH + 1, posY );
  }
}


void ScaleImageGenerator::drawSquare( TCanvas& canvas, const rgb& semanticColour, int posX, int posY ) const
{
  const wxColour squareColour( semanticColour.red, semanticColour.green, semanticColour.blue );

  canvas.drawRectangle( squareColour, posX, posY, layout.squareWidth, layout.squareHeight );
}


wxString ScaleImageGenerator::getShortLabel( TSemanticValue whichValue ) const
{
  const auto valueLabel = semanticValuesLabelsShort.find( whichValue );

  return valueLabel != semanticValuesLabelsShort.end() ? valueLabel->second : wxString();
}


void ScaleImageGenerator::applyTransparencyMask()
{
#ifndef __WXMAC__
  if( layout.backgroundMode == TBackgroundMode::BACKGROUND_TRANSPARENT && legendMaskBitmap.IsOk() )
  {
    // The bitmap takes ownership of the mask.
    legendBitmap.SetMask( new wxMask( legendMaskBitmap, *wxWHITE ) );
  }
#endif
}
