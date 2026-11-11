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
 * @file windowimage.cpp
 * @brief Implementation of WindowImageGenerator, the image model behind the Save Image dialog.
 *
 * The file is organized after the three responsibilities of the class:
 *
 * - Image generation: the window image (from a bitmap or from a grid), the
 *   legends (delegated to ScaleImageGenerator) and their composition.
 * - Legend options: every option changed in the Save Image dialog is applied
 *   to all the legends and the composed image is rebuilt, so previews and
 *   saved files always agree.
 * - Export: saving any of the images to disk or copying it to the clipboard.
 *
 * Device contexts are only created while drawing; the bitmaps kept as members
 * are never left selected in one.
 */

#include "windowimage.h"

#include "histogram.h"
#include "window.h"

#include <algorithm>
#include <cstdint>
#include <wx/brush.h>
#include <wx/dcmemory.h>
#include <wx/grid.h>
#include <wx/image.h>
#include <wx/pen.h>

#ifdef __WXMAC__
#  include <wx/dc.h>
#  include <wx/dcgraph.h>
#endif

namespace
{
// This limit happens to be related with contiguous block of memory for the bitmap buffer.
// If exceeded, aborts after CAIRO assert
constexpr std::uint64_t MAX_CAIRO_SIZE = 4294967296ULL;
constexpr int IMG_DEPTH                = 4 * 8; // ( RGB + alpha ) * 8 bits/color

// Space left around the text of the title strip.
constexpr int TITLE_MARGIN = 5;


/**
 * @brief Checks whether an image would exceed the size the graphics backend can handle.
 * @param width Width of the image in pixels.
 * @param height Height of the image in pixels.
 * @return true if the image is too big to be created.
 */
bool exceedsMaxImageSize( int width, int height )
{
  return static_cast< std::uint64_t >( width ) * static_cast< std::uint64_t >( height ) * IMG_DEPTH > MAX_CAIRO_SIZE;
}


/**
 * @brief Deduces the kind of legend that describes a timeline.
 * @param whichTimeline Timeline to describe.
 * @return The legend type for its colour mode; code colour for the modes without a specific legend.
 */
TScaleImageType getScaleType( const Timeline* whichTimeline )
{
  if( whichTimeline->isGradientColorSet() )
    return TScaleImageType::TScaleImageGradientColor;
  if( whichTimeline->isNotNullGradientColorSet() )
    return TScaleImageType::TScaleImageGradientColorNotNull;
  if( whichTimeline->isAlternativeGradientColorSet() )
    return TScaleImageType::TScaleImageAlternativeGradientColor;
  if( whichTimeline->isFusedLinesColorSet() )
    return TScaleImageType::TScaleImageGradientFusedLines;

  return TScaleImageType::TScaleImageCodeColor;
}


/**
 * @brief Deduces the kind of legend that describes the cells of a histogram.
 * @param whichHistogram Histogram to describe.
 * @return The legend type for its colour mode; code colour for the modes without a specific legend.
 */
TScaleImageType getScaleType( const Histogram* whichHistogram )
{
  switch( whichHistogram->getColorMode() )
  {
    case TColorFunction::GRADIENT:
      return TScaleImageType::TScaleImageGradientColor;
    case TColorFunction::NOT_NULL_GRADIENT:
      return TScaleImageType::TScaleImageGradientColorNotNull;
    case TColorFunction::ALTERNATIVE_GRADIENT:
      return TScaleImageType::TScaleImageAlternativeGradientColor;
    default:
      return TScaleImageType::TScaleImageCodeColor;
  }
}
} // namespace


WindowImageGenerator::WindowImageGenerator( const std::vector< wxBitmap* >& whichBitmaps,
                                            wxColour whichBackground,
                                            wxColour whichForeground,
                                            wxFont whichTextFont,
                                            TImageFormat whichFilterIndex,
                                            bool /* whichMergedImages */ )
  : sourceBitmaps( whichBitmaps )
  , sourceBitmap( whichBitmaps.empty() ? nullptr : whichBitmaps.front() )
  , imageFormat( whichFilterIndex )
  , imageBackground( whichBackground )
  , imageForeground( whichForeground )
  , imageTextFont( whichTextFont )
{
}


WindowImageGenerator::~WindowImageGenerator() = default;


#ifdef __WXMAC__
void WindowImageGenerator::drawStackedBitmaps( wxDC& dc, const std::vector< wxBitmap* >& whichBitmaps )
{
  bool alpha = false;
  for( auto b : whichBitmaps )
  {
    dc.DrawBitmap( *b, 0, 0, alpha );
    if( !alpha )
      alpha = true; // Apply alpha to others than first
  }
}
#endif


wxBitmap* WindowImageGenerator::getWindowsImageBitmap()
{
  return windowImage.IsOk() ? &windowImage : nullptr;
}


wxBitmap* WindowImageGenerator::getTimelineScaleImage()
{
  return windowAndLegendImage.IsOk() ? &windowAndLegendImage : nullptr;
}


wxBitmap* WindowImageGenerator::getDataLegendBitmap()
{
  return dataLegend != nullptr ? dataLegend->getBitmap() : nullptr;
}


wxBitmap* WindowImageGenerator::getControlLegendBitmap()
{
  return controlLegend != nullptr ? controlLegend->getBitmap() : nullptr;
}


const ScaleImageGenerator* WindowImageGenerator::getDataLegend() const
{
  return dataLegend.get();
}


const ScaleImageGenerator* WindowImageGenerator::getControlLegend() const
{
  return controlLegend.get();
}


wxString WindowImageGenerator::getLegendSuffix() const
{
  return dataLegend != nullptr ? dataLegend->getLegendSuffix() : wxString();
}


TImageFormat WindowImageGenerator::getImageFormat() const
{
  return imageFormat;
}


void WindowImageGenerator::setImageFormat( TImageFormat whichFilterIndex )
{
  imageFormat = whichFilterIndex;
}


void WindowImageGenerator::setImageOrientation( TScaleImageOrientation whichOrientation )
{
  if( orientation == whichOrientation )
    return;

  orientation = whichOrientation;

  for( ScaleImageGenerator* standaloneLegend : { dataLegend.get(), controlLegend.get() } )
  {
    if( standaloneLegend != nullptr )
    {
      standaloneLegend->setImageOrientation( orientation );
      standaloneLegend->generateImage();
    }
  }

  // The attached legend takes the orientation when the image is composed.
  composeWindowAndLegendImage( true );
}


void WindowImageGenerator::transformToShort( bool isShort )
{
  for( ScaleImageGenerator* standaloneLegend : { dataLegend.get(), controlLegend.get() } )
  {
    if( standaloneLegend != nullptr )
    {
      standaloneLegend->setShortLabels( isShort );
      standaloneLegend->generateImage();
    }
  }

  if( attachedLegend != nullptr )
    attachedLegend->setShortLabels( isShort );

  composeWindowAndLegendImage( true );
}


void WindowImageGenerator::setSelectedSemanticValues( const std::vector< int >& whichSelectedPositions )
{
  if( dataLegend != nullptr )
  {
    dataLegend->setSelectedSemanticValues( whichSelectedPositions );
    dataLegend->generateImage();
  }

  if( attachedLegend != nullptr )
    attachedLegend->setSelectedSemanticValues( whichSelectedPositions );

  composeWindowAndLegendImage( true );
}


bool WindowImageGenerator::generateWindowsImage( const std::string& windowsName, const std::string& windowTraceName )
{
  // Whatever was generated before no longer matches the window.
  windowImage          = wxBitmap();
  windowAndLegendImage = wxBitmap();

  if( sourceBitmap == nullptr || !sourceBitmap->IsOk() )
    return false;

  const int contentsWidth  = sourceBitmap->GetWidth();
  const int contentsHeight = sourceBitmap->GetHeight();
  const int titleHeight    = getTitleHeight();

  // NOTE: only the title strip is checked here; the contents already exist as a bitmap.
  if( exceedsMaxImageSize( contentsWidth, titleHeight ) )
    return false;

  wxBitmap titleBitmap = renderTitle( windowsName, windowTraceName, contentsWidth );
  wxBitmap newWindowImage( contentsWidth, contentsHeight + titleHeight );
  if( !titleBitmap.IsOk() || !newWindowImage.IsOk() )
    return false;

  {
    wxMemoryDC windowDC( newWindowImage );

    wxMemoryDC titleDC( titleBitmap );
    windowDC.Blit( 0, 0, contentsWidth, titleHeight, &titleDC, 0, 0 );

#ifdef __WXMAC__
    wxBitmap stackedContents( contentsWidth, contentsHeight );
    wxMemoryDC contentsDC( stackedContents );
    WindowImageGenerator::drawStackedBitmaps( contentsDC, sourceBitmaps );
#else
    wxMemoryDC contentsDC( *sourceBitmap );
#endif
    windowDC.Blit( 0, titleHeight, contentsWidth, contentsHeight, &contentsDC, 0, 0 );
  }

  windowImage = newWindowImage;

  return true;
}


bool WindowImageGenerator::generateWindowsImageGrid( wxGrid* gridImage, const std::string& windowsName, const std::string& windowTraceName )
{
  // Whatever was generated before no longer matches the window.
  windowImage          = wxBitmap();
  windowAndLegendImage = wxBitmap();

  if( gridImage == nullptr )
    return false;

  int gridWidth  = 0;
  int gridHeight = 0;

  auto sumSize = []( const auto& sizes, auto numElems, auto& totalSize )
  {
    for( auto c = 0; c < numElems; ++c )
      totalSize += sizes.GetSize( c );
  };

  sumSize( gridImage->GetColSizes(), gridImage->GetNumberCols(), gridWidth );
  sumSize( gridImage->GetRowSizes(), gridImage->GetNumberRows(), gridHeight );

  gridWidth += gridImage->GetRowLabelSize();
  gridHeight += gridImage->GetColLabelSize();

  if( gridWidth <= 0 || gridHeight <= 0 || exceedsMaxImageSize( gridWidth, gridHeight ) )
    return false;

  const int titleHeight = getTitleHeight();

  wxBitmap titleBitmap = renderTitle( windowsName, windowTraceName, gridWidth );
  wxBitmap newWindowImage( gridWidth, gridHeight + titleHeight );
  if( !titleBitmap.IsOk() || !newWindowImage.IsOk() )
    return false;

  {
    wxMemoryDC windowDC( newWindowImage );
    windowDC.SetBackground( wxBrush( gridImage->GetLabelBackgroundColour() ) );
    windowDC.Clear();

    wxMemoryDC titleDC( titleBitmap );
    windowDC.Blit( 0, 0, gridWidth, titleHeight, &titleDC, 0, 0 );

    gridImage->Render( windowDC, wxPoint( 0, titleHeight ) );
  }

  windowImage = newWindowImage;

  return true;
}


void WindowImageGenerator::generateTimelineLegend( const std::map< TSemanticValue, rgb >& semanticValuesToColor, Timeline* whichTimeline )
{
  dataLegend = createLegend( semanticValuesToColor, whichTimeline, nullptr, imageBackground, imageForeground );

  if( dataLegend != nullptr )
    dataLegend->generateImage();
}


void WindowImageGenerator::generateHistogramDataLegend( const std::map< TSemanticValue, rgb >& semanticValuesToColor,
                                                        Timeline* whichDataWindow,
                                                        Histogram* whichHistogram )
{
  dataLegend.reset();

  if( whichHistogram == nullptr )
    return;

  dataLegend = createLegend( semanticValuesToColor, whichDataWindow, whichHistogram, *wxWHITE, *wxBLACK );

  if( dataLegend != nullptr )
    dataLegend->generateImage();
}


void WindowImageGenerator::generateHistogramControlLegend( const std::map< TSemanticValue, rgb >& semanticValuesToColor,
                                                           Timeline* whichControlWindow )
{
  controlLegend = createLegend( semanticValuesToColor, whichControlWindow, nullptr, *wxWHITE, *wxBLACK );

  if( controlLegend != nullptr )
    controlLegend->generateImage();
}


void WindowImageGenerator::generateScaleWindowsImage( const std::map< TSemanticValue, rgb >& semanticValuesToColor,
                                                      Timeline* myWindow,
                                                      Histogram* myHistogram )
{
  if( myHistogram != nullptr )
    attachedLegend = createLegend( semanticValuesToColor, myWindow, myHistogram, *wxWHITE, *wxBLACK );
  else
    attachedLegend = createLegend( semanticValuesToColor, myWindow, myHistogram, imageBackground, imageForeground );


  // The first composition keeps a horizontal legend aligned to the left side of
  // the window; the ones triggered by the legend options center it.
  composeWindowAndLegendImage( false );
}


bool WindowImageGenerator::copyImageToClipboard( wxBitmap* whichBitmapImage )
{
  if( whichBitmapImage == nullptr )
    whichBitmapImage = getWindowsImageBitmap();

  return whichBitmapImage != nullptr && copyBitmapToClipboard( *whichBitmapImage );
}


bool WindowImageGenerator::saveImageWindows( wxString whichFileName, bool whichCopyToClipboard )
{
  return exportBitmap( windowImage, whichFileName, whichCopyToClipboard );
}


bool WindowImageGenerator::saveImageWindowScale( wxString whichFileName, bool whichCopyToClipboard )
{
  return exportBitmap( windowAndLegendImage, whichFileName, whichCopyToClipboard );
}


bool WindowImageGenerator::saveImageScale( wxString whichFileName, bool /* appendLegendSuffix */, bool whichCopyToClipboard )
{
  return exportLegend( dataLegend.get(), whichFileName, whichCopyToClipboard );
}


bool WindowImageGenerator::saveImageControlScale( wxString whichFileName, bool /* appendLegendSuffix */, bool whichCopyToClipboard )
{
  return exportLegend( controlLegend.get(), whichFileName, whichCopyToClipboard );
}


int WindowImageGenerator::getTitleHeight() const
{
  return imageTextFont.GetPointSize() + ( 2 * TITLE_MARGIN ); // up + down margins + text
}


wxBitmap WindowImageGenerator::renderTitle( const std::string& windowsName, const std::string& windowTraceName, int titleWidth ) const
{
  wxBitmap titleBitmap( titleWidth, getTitleHeight() );
  if( !titleBitmap.IsOk() )
    return titleBitmap;

  {
    wxMemoryDC titleDC( titleBitmap );

    // Set font and check if using it the title will fit
    titleDC.SetFont( imageTextFont );

    wxString writtenTitle          = wxString::FromUTF8( ( windowsName + " @ " + windowTraceName ).c_str() );
    const int titleWritableWidth   = titleWidth - ( 2 * TITLE_MARGIN );
    const bool fullTitleDoesNotFit = titleDC.GetTextExtent( writtenTitle ).GetWidth() > titleWritableWidth;
    if( fullTitleDoesNotFit )
      writtenTitle = wxString::FromUTF8( windowsName.c_str() );

    // Set colors
    titleDC.SetBackground( wxBrush( imageBackground ) );
    titleDC.Clear();

    titleDC.SetPen( wxPen( imageBackground, 1 ) );
    titleDC.SetTextBackground( imageBackground );
    titleDC.SetTextForeground( imageForeground );
    titleDC.DrawText( writtenTitle, TITLE_MARGIN, TITLE_MARGIN );
  }

  return titleBitmap;
}


std::unique_ptr< ScaleImageGenerator > WindowImageGenerator::createLegend( const std::map< TSemanticValue, rgb >& semanticValuesToColor,
                                                                           Timeline* whichTimeline,
                                                                           Histogram* whichHistogram,
                                                                           wxColour whichBackground,
                                                                           wxColour whichForeground ) const
{
  if( whichTimeline == nullptr )
    return nullptr;

  if( whichHistogram != nullptr )
    return std::make_unique< ScaleImageGenerator >( whichHistogram,
                                                    whichTimeline,
                                                    semanticValuesToColor,
                                                    imageTextFont,
                                                    getScaleType( whichHistogram ),
                                                    orientation,
                                                    whichBackground,
                                                    whichForeground );

  return std::make_unique< ScaleImageGenerator >( whichTimeline,
                                                  semanticValuesToColor,
                                                  imageTextFont,
                                                  getScaleType( whichTimeline ),
                                                  orientation,
                                                  whichBackground,
                                                  whichForeground );
}


void WindowImageGenerator::composeWindowAndLegendImage( bool centerHorizontalLegend )
{
  windowAndLegendImage = wxBitmap();

  if( attachedLegend == nullptr || !windowImage.IsOk() )
    return;

  const bool horizontalLegend = orientation == TScaleImageOrientation::TScaleImageOrientationHorizontal;
  const int windowWidth       = windowImage.GetWidth();
  const int windowHeight      = windowImage.GetHeight(); // up + down margins + text + image

  // The legend runs along the window contents: their width if it goes below, their height if it goes at the right.
  attachedLegend->setImageOrientation( orientation );
  attachedLegend->generateImage( horizontalLegend ? windowWidth : windowHeight - getTitleHeight() );

  wxBitmap* legendBitmap = attachedLegend->getBitmap();
  if( legendBitmap == nullptr )
    return;

  const int legendWidth  = legendBitmap->GetWidth();
  const int legendHeight = legendBitmap->GetHeight();

  int totalWidth  = 0;
  int totalHeight = 0;
  int legendPosX  = 0;
  int legendPosY  = 0;
  if( horizontalLegend )
  {
    totalWidth  = windowWidth;
    totalHeight = windowHeight + legendHeight;
    legendPosX  = centerHorizontalLegend ? ( totalWidth - legendWidth ) / 2 : 0;
    legendPosY  = windowHeight;
  }
  else
  {
    totalWidth  = windowWidth + legendWidth;
    totalHeight = std::max( windowHeight, legendHeight );
    legendPosX  = windowWidth;
    legendPosY  = ( totalHeight - legendHeight ) / 2;
  }

  wxBitmap composedImage( totalWidth, totalHeight );
  if( !composedImage.IsOk() )
    return;

  {
    wxMemoryDC composedDC( composedImage );
    composedDC.SetBackground( wxBrush( imageBackground ) );
    composedDC.Clear();

    wxMemoryDC windowDC( windowImage );
    composedDC.Blit( 0, 0, windowWidth, windowHeight, &windowDC, 0, 0 );

    wxMemoryDC legendDC( *legendBitmap );
    composedDC.Blit( legendPosX, legendPosY, legendWidth, legendHeight, &legendDC, 0, 0 );
  }

  windowAndLegendImage = composedImage;
}


bool WindowImageGenerator::exportBitmap( const wxBitmap& whichBitmap, const wxString& whichFilePath, bool whichCopyToClipboard ) const
{
  if( !whichBitmap.IsOk() )
    return false;

  if( whichCopyToClipboard )
    return copyBitmapToClipboard( whichBitmap );

  return whichBitmap.ConvertToImage().SaveFile( whichFilePath, imageFormatTypeToBitmapType( imageFormat ) );
}


bool WindowImageGenerator::exportLegend( ScaleImageGenerator* whichLegend, const wxString& whichFilePath, bool whichCopyToClipboard ) const
{
  if( whichLegend == nullptr )
    return false;

  if( whichCopyToClipboard )
    return whichLegend->copyImageToClipboard();

  return whichLegend->saveImage( whichFilePath, imageFormat );
}
