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


#include "windowimage.h"

#include "labelconstructor.h"

#include <iterator>
#include <vector>
#include <wx/clipbrd.h>
#include <wx/filename.h>

#ifdef __WXMAC__
#  include <wx/dc.h>
#  include <wx/dcgraph.h>
#endif

#define SIZE_MARK_LABEL 4


wxBitmapType imageFormatTypeToBitmapType( const TImageFormat& whichFilterIndex )
{
  wxBitmapType imageType;

  switch( whichFilterIndex )
  {
    case TImageFormat::BMP:
      imageType = wxBITMAP_TYPE_BMP;
      break;
    case TImageFormat::JPG:
      imageType = wxBITMAP_TYPE_JPEG;
      break;
    case TImageFormat::PNG:
      imageType = wxBITMAP_TYPE_PNG;
      break;
    case TImageFormat::XPM:
      imageType = wxBITMAP_TYPE_XPM;
      break;
    default:
      imageType = wxBITMAP_TYPE_PNG;
      break;
  }

  return imageType;
}


WindowImageGenerator::WindowImageGenerator( const std::vector< wxBitmap* >& whichBitmaps,
                                            // WindowGenericItem whichGenericItem,
                                            wxColour whichBackground,
                                            wxColour whichForeground,
                                            wxFont whichTextFont,
                                            TImageFormat whichFilterIndex,
                                            bool whichMergedImages )

{
  // genericItem     = whichGenericItem;
  bitmaps         = whichBitmaps;
  bitmapTimeline  = whichBitmaps.front();
  imageBackground = whichBackground;
  imageForeground = whichForeground;
  imageTextFont   = whichTextFont;
  filterIndex     = whichFilterIndex;
}


WindowImageGenerator::~WindowImageGenerator()
{
  if( scaleImage != scaleImageData )
    delete scaleImage;

  delete scaleImageData;

  delete bitmapImageTimeline;
  delete memoryImageTimeline;

  delete bitmapImageTimelineScale;
  delete memoryImageTimelineScale;
}


wxBitmap* WindowImageGenerator::getWindowsImageBitmap()
{
  return bitmapImageTimeline;
}


wxBitmap* WindowImageGenerator::getTimelineScaleImage()
{
  return bitmapImageTimelineScale;
}


wxBitmap* WindowImageGenerator::getScaleImageBitmap()
{
  if( scaleImage == nullptr )
    return nullptr;
  return scaleImage->getBitmap();
}


ScaleImageGenerator* WindowImageGenerator::getScaleImage()
{
  if( scaleImage == nullptr )
    return nullptr;
  return scaleImage;
}


ScaleImageGenerator* WindowImageGenerator::getScaleWindowsImage()
{
  if( scaleWindowsImage == nullptr )
    return nullptr;
  return scaleWindowsImage;
}


wxBitmap* WindowImageGenerator::getScaleDataImageBitmap()
{
  if( scaleImageData == nullptr )
    return nullptr;
  return scaleImageData->getBitmap();
}


ScaleImageGenerator* WindowImageGenerator::getScaleDataImage()
{
  if( scaleImageData == nullptr )
    return nullptr;
  return scaleImageData;
}


wxString WindowImageGenerator::getLegendSuffix()
{
  return scaleImage->getLegendSuffix();
}


TImageFormat WindowImageGenerator::getImageFormat()
{
  return filterIndex;
}


void WindowImageGenerator::setImageFormat( TImageFormat whichFilterIndex )
{
  filterIndex = whichFilterIndex;
}


void WindowImageGenerator::setImageOrientation( TScaleImageOrientation a_orientation )
{
  if( orientation != a_orientation )
  {
    orientation = a_orientation;

    if( scaleImageData )
    {
      scaleImageData->setImageOrientation( a_orientation );
      scaleImageData->initializeConfiguration();
      scaleImageData->processImage();
    }

    if( scaleImage )
    {
      scaleImage->setImageOrientation( a_orientation );
      scaleImage->initializeConfiguration();
      scaleImage->processImage();
    }

    generateScaleWindowsImage();
  }
}


void WindowImageGenerator::transformToShort( bool isShort )
{
  if( scaleImage )
  {
    scaleImage->setTrasformToShort( isShort );
    scaleImage->initializeConfiguration();
    scaleImage->processImage();
  }
  if( scaleImageData )
  {
    scaleImageData->setTrasformToShort( isShort );
    scaleImageData->initializeConfiguration();
    scaleImageData->processImage();
  }

  scaleWindowsImage->setTrasformToShort( isShort );
  scaleWindowsImage->initializeConfiguration();
  scaleWindowsImage->processImage();
}


void WindowImageGenerator::generatescaleImageDataWindows( std::map< TSemanticValue, rgb > semanticValuesToColor,
                                                          Timeline* myWindow,
                                                          Histogram* myHistogram )
{
  std::map< TSemanticValue, rgb > scaleValues = semanticValuesToColor;

  if( myHistogram->getColorMode() == TColorFunction::GRADIENT )
  {
    typeScale = TScaleImageType::TScaleImageGradientColor;
  }
  else if( myHistogram->getColorMode() == TColorFunction::NOT_NULL_GRADIENT )
  {
    typeScale = TScaleImageType::TScaleImageGradientColorNotNull;
  }
  else if( myHistogram->getColorMode() == TColorFunction::ALTERNATIVE_GRADIENT )
  {
    typeScale = TScaleImageType::TScaleImageAlternativeGradientColor;
  }
  else if( myHistogram->getColorMode() == TColorFunction::CODE_COLOR )
  {
    typeScale = TScaleImageType::TScaleImageCodeColor;
  }

  orientationScale = TScaleImageOrientation::TScaleImageOrientationHorizontal;

  scaleImageData = new ScaleImageGenerator( myHistogram,
                                            myWindow,
                                            scaleValues,
                                            imageBackground,
                                            imageForeground,
                                            imageTextFont,
                                            filterIndex,
                                            typeScale,
                                            orientationScale,
                                            TBackgroundMode::BACKGROUND_SOLID_LIGHT );

  scaleImageData->initializeConfiguration();
  scaleImageData->computeSemanticLabels();
  scaleImageData->processImage();
}


void WindowImageGenerator::generatescaleImageDataWindows( std::map< TSemanticValue, rgb > semanticValuesToColor,
                                                          Timeline* myWindow,
                                                          bool derivedFromHisto )
{
  std::map< TSemanticValue, rgb > scaleValues = semanticValuesToColor;

  if( myWindow->isGradientColorSet() )
  {
    typeScale = TScaleImageType::TScaleImageGradientColor;
  }
  else if( myWindow->isNotNullGradientColorSet() )
  {
    typeScale = TScaleImageType::TScaleImageGradientColorNotNull;
  }
  else if( myWindow->isAlternativeGradientColorSet() )
  {
    typeScale = TScaleImageType::TScaleImageAlternativeGradientColor;
  }
  else if( myWindow->isCodeColorSet() )
  {
    typeScale = TScaleImageType::TScaleImageCodeColor;
  }
  else if( myWindow->isFusedLinesColorSet() )
  {
    std::map< TSemanticValue, rgb > tmpObjects;

    TObjectOrder beginRow = myWindow->getZoomSecondDimension().first;
    TObjectOrder endRow   = myWindow->getZoomSecondDimension().second;
    std::vector< TObjectOrder > selected;
    myWindow->getSelectedRows( myWindow->getLevel(), selected, beginRow, endRow, true );

    for( std::vector< TObjectOrder >::iterator it = selected.begin(); it != selected.end(); ++it )
    {
      rgb tmprgb = myWindow->getSemanticColor().calcColor( ( *it ) + 1, 0, myWindow->getTrace()->getLevelObjects( myWindow->getLevel() ) );
      tmpObjects[ (TSemanticValue)( *it ) ] = tmprgb;
    }

    typeScale = TScaleImageType::TScaleImageGradientFusedLines;

    std::map< TSemanticValue, rgb > scaleValues = tmpObjects;
  }
  orientationScale = TScaleImageOrientation::TScaleImageOrientationHorizontal;

  TBackgroundMode backgroundColor;
  if( derivedFromHisto )
  {
    backgroundColor = TBackgroundMode::BACKGROUND_SOLID_LIGHT;
  }
  else
  {
    backgroundColor = TBackgroundMode::BACKGROUND_SOLID_DARK;
  }
  scaleImage = new ScaleImageGenerator( myWindow,
                                        scaleValues,
                                        imageBackground,
                                        imageForeground,
                                        imageTextFont,
                                        filterIndex,
                                        typeScale,
                                        orientationScale,
                                        backgroundColor );

  scaleImage->initializeConfiguration();
  scaleImage->computeSemanticLabels();
  scaleImage->processImage();
  // scaleImage->setImageOrientation(orientation);
}


void WindowImageGenerator::generateWindowsImage( const std::string& windowsName, const std::string& windowTraceName )
{
  int marginSpace = 5;
  // Get title
  wxString longTitle    = wxString::FromUTF8( ( windowsName + " @ " + windowTraceName ).c_str() );
  wxString shortTitle   = wxString::FromUTF8( ( windowsName ).c_str() );
  wxString writtenTitle = longTitle;


  int titleHeigth = imageTextFont.GetPointSize() + ( 2 * marginSpace ); // up + down margins + text
  int titleWidth  = bitmapTimeline->GetWidth();

  int titleWritableWidth = titleWidth - ( 2 * marginSpace );

  int imageMargin         = 5;
  int imageTimelineHeigth = bitmapTimeline->GetHeight() + imageTextFont.GetPointSize() + ( 2 * imageMargin ); // up + down margins + text + image
  int imageTimelineWidth  = bitmapTimeline->GetWidth();

  // Build DC for title
  wxBitmap titleBitmap( titleWidth, titleHeigth );
  wxMemoryDC titleDC( titleBitmap );

  // Set font and check if using it the title will fit
  titleDC.SetFont( imageTextFont );
  wxSize titleSize = titleDC.GetTextExtent( writtenTitle );

  if( titleSize.GetWidth() > titleWritableWidth )
  {
    titleSize    = titleDC.GetTextExtent( shortTitle );
    writtenTitle = shortTitle;
  }

  // Set colors
  titleDC.SetBackground( wxBrush( imageBackground ) );
  titleDC.Clear();

  titleDC.SetPen( wxPen( imageBackground, 1 ) );
  titleDC.SetTextBackground( imageBackground );
  titleDC.SetTextForeground( imageForeground );
  titleDC.DrawText( writtenTitle, marginSpace, marginSpace );


  bitmapImageTimeline = new wxBitmap( imageTimelineWidth, imageTimelineHeigth );
  memoryImageTimeline = new wxMemoryDC( *bitmapImageTimeline );

  // Compute title image size

  memoryImageTimeline->Blit( 0, 0, titleWidth, titleHeigth, &titleDC, 0, 0 );

#ifdef __WXMAC__
  wxBitmap tmpDrawImage( bitmapTimeline->GetWidth(), bitmapTimeline->GetHeight() );
  wxMemoryDC timelineDC( tmpDrawImage );
  WindowImageGenerator::drawStackedBitmaps( timelineDC, bitmaps );
#else
  wxMemoryDC timelineDC( *bitmapTimeline );
#endif
  memoryImageTimeline->Blit( 0, titleHeigth, bitmapTimeline->GetWidth(), bitmapTimeline->GetHeight(), &timelineDC, 0, 0 );
}


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

void WindowImageGenerator::generateWindowsImageGrid( wxGrid* gridImage, const std::string& windowsName, const std::string& windowTraceName )
{
  int imageWidth  = 0;
  int imageHeight = 0;

  auto sumSize = []( const auto& sizes, auto numElems, auto& totalSize )
  {
    for( auto c = 0; c < numElems; ++c )
      totalSize += sizes.GetSize( c );
  };

  sumSize( gridImage->GetColSizes(), gridImage->GetNumberCols(), imageWidth );
  sumSize( gridImage->GetRowSizes(), gridImage->GetNumberRows(), imageHeight );

  imageWidth += gridImage->GetRowLabelSize();
  imageHeight += gridImage->GetColLabelSize();

  int marginSpace = 5;
  // Get title
  wxString longTitle    = wxString::FromUTF8( ( windowsName + " @ " + windowTraceName ).c_str() );
  wxString shortTitle   = wxString::FromUTF8( ( windowsName ).c_str() );
  wxString writtenTitle = longTitle;


  int titleHeigth = imageTextFont.GetPointSize() + ( 2 * marginSpace ); // up + down margins + text
  int titleWidth  = imageWidth;

  int titleWritableWidth = titleWidth - ( 2 * marginSpace );

  int imageMargin         = 5;
  int imageTimelineHeigth = imageHeight + imageTextFont.GetPointSize() + ( 2 * imageMargin ); // up + down margins + text + image
  int imageTimelineWidth  = imageWidth;

  // Build DC for title
  wxBitmap titleBitmap( titleWidth, titleHeigth );
  wxMemoryDC titleDC( titleBitmap );

  // Set font and check if using it the title will fit
  titleDC.SetFont( imageTextFont );
  wxSize titleSize = titleDC.GetTextExtent( writtenTitle );

  if( titleSize.GetWidth() > titleWritableWidth )
  {
    titleSize    = titleDC.GetTextExtent( shortTitle );
    writtenTitle = shortTitle;
  }

  // Set colors
  titleDC.SetBackground( wxBrush( imageBackground ) );
  titleDC.Clear();


  titleDC.SetPen( wxPen( imageBackground, 1 ) );
  titleDC.SetTextBackground( imageBackground );
  titleDC.SetTextForeground( imageForeground );
  titleDC.DrawText( writtenTitle, marginSpace, marginSpace );


  bitmapImageTimeline = new wxBitmap( imageTimelineWidth, imageTimelineHeigth );
  memoryImageTimeline = new wxMemoryDC( *bitmapImageTimeline );

  // Compute title image size


  auto c = gridImage->GetLabelBackgroundColour();
  memoryImageTimeline->SetBackground( wxBrush( c ) );
  memoryImageTimeline->Clear();

  memoryImageTimeline->Blit( 0, 0, titleWidth, titleHeigth, &titleDC, 0, 0 );

  gridImage->Render( *memoryImageTimeline, wxPoint( 0, titleHeigth ) );
}


void WindowImageGenerator::generateScaleWindowsImage( std::map< TSemanticValue, rgb > semanticValuesToColor, Timeline* myWindow, Histogram* myHisto )
{
  if( myHisto == nullptr )
    scaleWindowsImage = new ScaleImageGenerator( myWindow,
                                                 semanticValuesToColor,
                                                 imageBackground,
                                                 imageForeground,
                                                 imageTextFont,
                                                 filterIndex,
                                                 typeScale,
                                                 orientationScale,
                                                 TBackgroundMode::BACKGROUND_SOLID_DARK,
                                                 true );
  else
    scaleWindowsImage = new ScaleImageGenerator( myHisto,
                                                 myWindow,
                                                 semanticValuesToColor,
                                                 imageBackground,
                                                 imageForeground,
                                                 imageTextFont,
                                                 filterIndex,
                                                 typeScale,
                                                 orientationScale,
                                                 TBackgroundMode::BACKGROUND_SOLID_LIGHT,
                                                 true );
  scaleWindowsImage->initializeConfiguration();
  scaleWindowsImage->computeSemanticLabels();

  if( orientation == TScaleImageOrientation::TScaleImageOrientationHorizontal )
  {
    std::optional< int > wantedWidth = bitmapTimeline->GetWidth();

    scaleWindowsImage->processImage( wantedWidth );

    int imageMargin = 5;
    int imageHeigth = bitmapTimeline->GetHeight() + imageTextFont.GetPointSize() + ( 2 * imageMargin ); // up + down margins + text + image
    int imageWidth  = bitmapTimeline->GetWidth();

    int totalHeigth = imageHeigth + scaleWindowsImage->getBitmap()->GetHeight();
    // int totalWidth  = imageWidth > scaleWindowsImage->getBitmap()->GetWidth() ? imageWidth : scaleWindowsImage->getBitmap()->GetWidth();
    int totalWidth = imageWidth;

    bitmapImageTimelineScale = new wxBitmap( totalWidth, totalHeigth );
    memoryImageTimelineScale = new wxMemoryDC( *bitmapImageTimelineScale );

    memoryImageTimelineScale->SetBackground( imageBackground );
    memoryImageTimelineScale->Clear();

    int posX = 0;
    int posY = 0;

    memoryImageTimelineScale->Blit( posX, posY, imageWidth, imageHeigth, memoryImageTimeline, 0, 0 );

    posX = 0;
    posY = imageHeigth;

    wxMemoryDC tmpScaleMemory( *scaleWindowsImage->getBitmap() );

    memoryImageTimelineScale
      ->Blit( posX, posY, scaleWindowsImage->getBitmap()->GetWidth(), scaleWindowsImage->getBitmap()->GetHeight(), &tmpScaleMemory, 0, 0 );
  }
  else
  {
    std::optional< int > wantedHeight = bitmapTimeline->GetHeight();

    scaleWindowsImage->processImage( wantedHeight );

    int imageMargin = 5;
    int imageHeigth = bitmapTimeline->GetHeight() + imageTextFont.GetPointSize() + ( 2 * imageMargin ); // up + down margins + text + image
    int imageWidth  = bitmapTimeline->GetWidth();

    int totalWidth  = imageWidth + scaleWindowsImage->getBitmap()->GetWidth();
    int totalHeigth = imageHeigth > scaleWindowsImage->getBitmap()->GetHeight() ? imageHeigth : scaleWindowsImage->getBitmap()->GetHeight();


    bitmapImageTimelineScale = new wxBitmap( totalWidth, totalHeigth );
    memoryImageTimelineScale = new wxMemoryDC( *bitmapImageTimelineScale );

    memoryImageTimelineScale->SetBackground( imageBackground );
    memoryImageTimelineScale->Clear();

    int posX = 0;
    int posY = 0;

    memoryImageTimelineScale->Blit( posX, posY, imageWidth, imageHeigth, memoryImageTimeline, 0, 0 );

    posY = ( totalHeigth - scaleWindowsImage->getBitmap()->GetHeight() ) / 2;
    posX = imageWidth;

    wxMemoryDC tmpScaleMemory( *scaleWindowsImage->getBitmap() );

    memoryImageTimelineScale
      ->Blit( posX, posY, scaleWindowsImage->getBitmap()->GetWidth(), scaleWindowsImage->getBitmap()->GetHeight(), &tmpScaleMemory, 0, 0 );
  }
}


void WindowImageGenerator::generateScaleWindowsImage()
{
  scaleWindowsImage->setImageOrientation( orientation );
  scaleWindowsImage->initializeConfiguration();

  if( orientation == TScaleImageOrientation::TScaleImageOrientationHorizontal )
  {
    std::optional< int > wantedWidth = bitmapTimeline->GetWidth();

    scaleWindowsImage->processImage( wantedWidth );

    int imageMargin = 5;
    int imageHeigth = bitmapTimeline->GetHeight() + imageTextFont.GetPointSize() + ( 2 * imageMargin ); // up + down margins + text + image
    int imageWidth  = bitmapTimeline->GetWidth();

    int totalHeigth = imageHeigth + scaleWindowsImage->getBitmap()->GetHeight();
    // int totalWidth  = imageWidth > scaleWindowsImage->getBitmap()->GetWidth() ? imageWidth : scaleWindowsImage->getBitmap()->GetWidth();
    int totalWidth = imageWidth;

    bitmapImageTimelineScale = new wxBitmap( totalWidth, totalHeigth );
    memoryImageTimelineScale = new wxMemoryDC( *bitmapImageTimelineScale );

    memoryImageTimelineScale->SetBackground( imageBackground );
    memoryImageTimelineScale->Clear();

    int posX = 0;
    int posY = 0;

    memoryImageTimelineScale->Blit( posX, posY, imageWidth, imageHeigth, memoryImageTimeline, 0, 0 );

    posX = ( totalWidth - scaleWindowsImage->getBitmap()->GetWidth() ) / 2;
    posY = imageHeigth;

    wxMemoryDC tmpScaleMemory( *scaleWindowsImage->getBitmap() );

    memoryImageTimelineScale
      ->Blit( posX, posY, scaleWindowsImage->getBitmap()->GetWidth(), scaleWindowsImage->getBitmap()->GetHeight(), &tmpScaleMemory, 0, 0 );
  }
  else
  {
    std::optional< int > wantedHeight = bitmapTimeline->GetHeight();

    scaleWindowsImage->processImage( wantedHeight );

    int imageMargin = 5;
    int imageHeigth = bitmapTimeline->GetHeight() + imageTextFont.GetPointSize() + ( 2 * imageMargin ); // up + down margins + text + image
    int imageWidth  = bitmapTimeline->GetWidth();

    int totalWidth  = imageWidth + scaleWindowsImage->getBitmap()->GetWidth();
    int totalHeigth = imageHeigth > scaleWindowsImage->getBitmap()->GetHeight() ? imageHeigth : scaleWindowsImage->getBitmap()->GetHeight();

    bitmapImageTimelineScale = new wxBitmap( totalWidth, totalHeigth );
    memoryImageTimelineScale = new wxMemoryDC( *bitmapImageTimelineScale );

    memoryImageTimelineScale->SetBackground( imageBackground );
    memoryImageTimelineScale->Clear();

    int posX = 0;
    int posY = 0;

    memoryImageTimelineScale->Blit( posX, posY, imageWidth, imageHeigth, memoryImageTimeline, 0, 0 );

    posY = ( totalHeigth - scaleWindowsImage->getBitmap()->GetHeight() ) / 2;
    posX = imageWidth;

    wxMemoryDC tmpScaleMemory( *scaleWindowsImage->getBitmap() );

    memoryImageTimelineScale
      ->Blit( posX, posY, scaleWindowsImage->getBitmap()->GetWidth(), scaleWindowsImage->getBitmap()->GetHeight(), &tmpScaleMemory, 0, 0 );
  }
}

// TODO: refactor using CRPT
bool WindowImageGenerator::copyImageToClipboard( wxBitmap* whichBitmapImage )
{
  bool copyDone = false;
  if( whichBitmapImage == nullptr )
    whichBitmapImage = bitmapImageTimeline;

  if( wxTheClipboard->Open() )
  {
    wxImage timelineImage( whichBitmapImage->ConvertToImage() );
    copyDone = wxTheClipboard->SetData( new wxBitmapDataObject( *whichBitmapImage ) );

    wxTheClipboard->Close();
  }

  return copyDone;
}


void WindowImageGenerator::saveImageWindows( wxString whichFileName, bool whichCopyToClipboard )
{
  // Save timeline image without scale
  if( whichCopyToClipboard )
    copyImageToClipboard();
  else
  {
    wxImage timelineImage( bitmapImageTimeline->ConvertToImage() );
    timelineImage.SaveFile( whichFileName, imageFormatTypeToBitmapType( filterIndex ) );
  }
}


void WindowImageGenerator::saveImageWindowScale( wxString whichFileName, bool whichCopyToClipboard )
{
  // Save timeline image without scale
  if( whichCopyToClipboard )
    copyImageToClipboard( bitmapImageTimelineScale );
  else
  {
    wxString currentFormat         = wxString::FromUTF8( LabelConstructor::getImageFileSuffix( TImageFormat( filterIndex ) ).c_str() );
    wxString tmpScaledTimelinePath = wxFileName( whichFileName ).GetPathWithSep() + wxFileName( whichFileName ).GetName() + currentFormat;

    wxImage timelineImageScale( bitmapImageTimelineScale->ConvertToImage() );
    timelineImageScale.SaveFile( tmpScaledTimelinePath, imageFormatTypeToBitmapType( filterIndex ) );
  }
}


void WindowImageGenerator::saveImageControlScale( wxString imageName, bool appendLegendSuffix, bool whichCopyToClipboard )
{
  scaleImageData->saveImage( imageName, appendLegendSuffix, whichCopyToClipboard );
}


void WindowImageGenerator::saveImageScale( wxString imageName, bool appendLegendSuffix, bool whichCopyToClipboard )
{
  scaleImage->saveImage( imageName, appendLegendSuffix, whichCopyToClipboard );
}


ScaleImageGenerator::ScaleImageGenerator( Histogram* whichMyWindow,
                                          Timeline* whichmyDataWindow,
                                          const std::map< TSemanticValue, rgb >& whichSemanticValues,
                                          wxColour whichBackground,
                                          wxColour whichForeground,
                                          wxFont whichTextFont,
                                          TImageFormat& whichImageType,
                                          TScaleImageType whichScaleType,
                                          TScaleImageOrientation whichOrientation,
                                          TBackgroundMode backgroundColor,
                                          bool whichMergedImages )
  : myWindow( whichMyWindow )
  , myDataWindow( whichmyDataWindow )
  , filterIndex( whichImageType )
  , semanticValuesColors( whichSemanticValues )
  , dataImage()
  , scaleType( whichScaleType )
  , orientation( whichOrientation )
  , mergedImages( whichMergedImages )
{
  dataImage.backgroundMode = backgroundColor;
  dataImage.precision      = 2;

  dataImage.textFont = whichTextFont;

  updateLegendSuffix();
  computeSemanticLabels();
}


ScaleImageGenerator::ScaleImageGenerator( Timeline* whichmyWindow,
                                          const std::map< TSemanticValue, rgb >& whichSemanticValues,
                                          wxColour whichBackground,
                                          wxColour whichForeground,
                                          wxFont whichTextFont,
                                          TImageFormat& whichImageType,
                                          TScaleImageType whichScaleType,
                                          TScaleImageOrientation whichOrientation,
                                          TBackgroundMode backgroundColor,
                                          bool whichMergedImages )
  : myWindow( whichmyWindow )
  , myDataWindow( whichmyWindow )
  , filterIndex( whichImageType )
  , semanticValuesColors( whichSemanticValues )
  , dataImage()
  , scaleType( whichScaleType )
  , orientation( whichOrientation )
  , mergedImages( whichMergedImages )
{
  dataImage.backgroundMode = backgroundColor;
  dataImage.precision      = 2;
  dataImage.textFont       = whichTextFont;

  updateLegendSuffix();
  computeSemanticLabels();
}


ScaleImageGenerator::~ScaleImageGenerator()
{
  delete bitmapImageContent;
  delete memoryImageContent;

  delete bitmapImageMask;
  delete memoryImageMask;
}


void ScaleImageGenerator::initializeConfiguration()
{
  auto visitorMaximumY = overloads{ []( Timeline* myWindow )
                                    {
                                      return myWindow->getMaximumY();
                                    },
                                    []( Histogram* myWindow )
                                    {
                                      return myWindow->getMaxGradient();
                                    } };
  auto visitorMinimumY = overloads{ []( Timeline* myWindow )
                                    {
                                      return myWindow->getMinimumY();
                                    },
                                    []( Histogram* myWindow )
                                    {
                                      return myWindow->getMinGradient();
                                    } };

  currentMin = std::visit( visitorMinimumY, myWindow );
  currentMax = std::visit( visitorMaximumY, myWindow );

  dataImage.symbolicDesc = false;
  dataImage.titleMargin  = 5;
  dataImage.rectMargin   = 2 * dataImage.titleMargin;

  if( scaleType == TScaleImageType::TScaleImageGradientColor || scaleType == TScaleImageType::TScaleImageGradientColorNotNull ||
      scaleType == TScaleImageType::TScaleImageAlternativeGradientColor )
  {
    dataImage.drawOutliers = true;
    dataImage.symbolicDesc = false;
    // totalSquaresGradientColor = 10;
  }
  else
  {
    dataImage.drawOutliers = false;
    dataImage.symbolicDesc = true;
  }
  if( scaleType == TScaleImageType::TScaleImageGradientFusedLines )
  {
    if( auto timeline = std::get_if< Timeline* >( &myWindow ) )
    {
      currentMin = 0;
      currentMax = ( *timeline )->getTrace()->getLevelObjects( ( *timeline )->getLevel() );
    }
  }
  dataImage.widthRect   = 10;
  dataImage.heightRect  = dataImage.textFont.GetPointSize() + 3;
  dataImage.imageHeight = dataImage.textFont.GetPointSize() + 2 * dataImage.titleMargin;
  dataImage.imageWidth  = dataImage.widthRect;
  //
  //
  // if( filterIndex != TImageFormat::BMP || TImageFormat::JPG != filterIndex || !mergedImages )
  // {
  //   dataImage.backgroundMode = TBackgroundMode::BACKGROUND_TRANSPARENT;
  // }
}


void ScaleImageGenerator::setImageOrientation( TScaleImageOrientation a_orientation )
{
  orientation = a_orientation;
}


void ScaleImageGenerator::computeSemanticLabels()
{
  // Dimensions: loop for every value to build labels and measure the maximum
  wxString curLabel;
  wxString maxLabel;
  //

  //
  for( auto it = semanticValuesColors.begin(); it != semanticValuesColors.end(); ++it )
  {
    // Get Labels
    if( TScaleImageType::TScaleImageGradientFusedLines != scaleType )
    {
      semanticValuesLabelsLong[ it->first ] =
        wxString::FromUTF8( LabelConstructor::semanticLabel( myDataWindow, it->first, dataImage.symbolicDesc, dataImage.precision, false ).c_str() );
    }
    else
    {
      if( auto timeline = std::get_if< Timeline* >( &myWindow ) )
      {
        semanticValuesLabelsLong[ it->first ] =
          wxString::FromUTF8( LabelConstructor::objectLabel( it->first, ( *timeline )->getLevel(), ( *timeline )->getTrace() ).c_str() );
      }
    }
    // Convert wxString → std::string (UTF-8)
    std::string narrow = std::string( semanticValuesLabelsLong[ it->first ].mb_str() );

    // Call your transform function
    LabelConstructor::transformToShort( narrow );

    // Convert back to wxString and assign
    semanticValuesLabelsShort[ it->first ] = wxString::FromUTF8( narrow.c_str() );
  }
  //

  for( auto i = 0; i < (int)semanticValuesLabelsLong.size(); ++i )
  {
    semanticValuesSelectedPositions.push_back( i );
  }
}


void ScaleImageGenerator::computeMaxSemanticLabelsSize()
{
  size_t maxLengthLabel = 0;
  size_t curLengthLabel;

  TSemanticValue semanticValueMaxLengthLabel;


  int i = 0;
  semanticValuesLabels.clear();

  if( !shortLabel )
  {
    for( auto it = semanticValuesLabelsLong.begin(); it != semanticValuesLabelsLong.end(); ++it )
    {
      if( std::find( semanticValuesSelectedPositions.begin(), semanticValuesSelectedPositions.end(), i ) != semanticValuesSelectedPositions.end() )
      {
        semanticValuesLabels.insert( *it );
        curLengthLabel = it->second.Len();
        //
        if( maxLengthLabel < curLengthLabel )
        {
          maxLengthLabel              = curLengthLabel;
          semanticValueMaxLengthLabel = it->first;
        }
      }
      ++i;
    }
  }
  else
  {
    for( auto it = semanticValuesLabelsShort.begin(); it != semanticValuesLabelsShort.end(); ++it )
    {
      if( std::find( semanticValuesSelectedPositions.begin(), semanticValuesSelectedPositions.end(), i ) != semanticValuesSelectedPositions.end() )
      {
        semanticValuesLabels.insert( *it );

        curLengthLabel = it->second.Len();
        //
        if( maxLengthLabel < curLengthLabel )
        {
          maxLengthLabel              = curLengthLabel;
          semanticValueMaxLengthLabel = it->first;
        }
      }
      ++i;
    }
  }


  wxBitmap maxLabelBitmap( 1920, 64 );
  wxMemoryDC maxLabelDC( maxLabelBitmap );
  maxLabelDC.SetFont( dataImage.textFont );
  maxLabelSize = maxLabelDC.GetTextExtent( semanticValuesLabels[ semanticValueMaxLengthLabel ] );
}


void ScaleImageGenerator::sortSemanticValuesGradientColors()
{
  semanticValuesColors.clear();
  TSemanticValue step = ( currentMax - currentMin ) / double( totalSquaresGradientColor - 1 ); //?
  for( int i = 0; i < totalSquaresGradientColor; ++i )
  {
    TSemanticValue current = ( i * step ) + currentMin;
    if( current > currentMax )
      current = currentMax;
    //
    auto min = currentMin;
    auto max = currentMax;
    //
    semanticValuesColors[ current ] = std::visit( overloads{ [ current ]( Histogram* myWindow )
                                                             {
                                                               return myWindow->calcGradientColor( current );
                                                             },
                                                             [ current, min, max ]( Timeline* myWindow )
                                                             {
                                                               return myWindow->getSemanticColor().calcColor( current, min, max );
                                                             } },
                                                  myWindow );
  }
}


void ScaleImageGenerator::computeGeneratedImageSize( int whichWantedWidth )
{
  if( orientation == TScaleImageOrientation::TScaleImageOrientationVertical )
  {
    if( scaleType == TScaleImageType::TScaleImageGradientColor || scaleType == TScaleImageType::TScaleImageGradientColorNotNull ||
        scaleType == TScaleImageType::TScaleImageAlternativeGradientColor )
    {
      dataImage.imageHeight = ( dataImage.heightRect ) * ( totalSquaresGradientColor ) + 2 * dataImage.titleMargin;
      //
      if( dataImage.drawOutliers )
        dataImage.imageHeight += ( dataImage.heightRect + dataImage.titleMargin ) * 2 + 2 * dataImage.titleMargin;
      // }

      dataImage.imageWidth = maxLabelSize.GetWidth() + ( 2 * dataImage.titleMargin ) + dataImage.widthRect + 2 * dataImage.rectMargin;
    }
    else
    {
      dataImage.imageHeight =
        ( semanticValuesLabels.size() - 1 ) * ( dataImage.heightRect + dataImage.rectMargin ) + dataImage.heightRect + 2 * dataImage.titleMargin;
      //
      if( dataImage.drawOutliers )
        dataImage.imageHeight += ( dataImage.heightRect + dataImage.titleMargin ) * 2;
      // }

      dataImage.imageWidth = maxLabelSize.GetWidth() + ( 2 * dataImage.titleMargin ) + dataImage.widthRect + 2 * dataImage.rectMargin;
    }
  }
  else if( orientation == TScaleImageOrientation::TScaleImageOrientationHorizontal )
  {
    if( scaleType == TScaleImageType::TScaleImageGradientColor || scaleType == TScaleImageType::TScaleImageGradientColorNotNull ||
        scaleType == TScaleImageType::TScaleImageAlternativeGradientColor )
    {
      dataImage.imageHeight = dataImage.heightRect + maxLabelSize.GetHeight() + 3 * dataImage.titleMargin;

      if( whichWantedWidth != 0 )
      {
        dataImage.widthRect  = ( whichWantedWidth - 2 * dataImage.titleMargin - 2 * dataImage.rectMargin ) / ( ( totalSquaresGradientColor + 2 ) );
        dataImage.imageWidth = whichWantedWidth;
      }
      else
      {
        dataImage.imageWidth = ( dataImage.widthRect * totalSquaresGradientColor + 2 * dataImage.rectMargin + 2 * dataImage.titleMargin ) +
                               2 * dataImage.rectMargin + 2 * dataImage.widthRect;
      }
    }
    else
    {
      dataImage.imageHeight = dataImage.heightRect + 2 * dataImage.titleMargin;

      dataImage.imageWidth = 0;

      wxBitmap maxLabelBitmap( 1920, 64 );
      wxMemoryDC maxLabelDC( maxLabelBitmap );
      maxLabelDC.SetFont( dataImage.textFont );

      for( auto it = semanticValuesLabels.begin(); it != semanticValuesLabels.end(); ++it )
      {
        wxSize titleSize = maxLabelDC.GetTextExtent( it->second );
        dataImage.imageWidth += 3 * dataImage.rectMargin + dataImage.heightRect + titleSize.GetWidth();
      }
      dataImage.imageWidth += 2 * dataImage.rectMargin;
    }
  }
}


void ScaleImageGenerator::createImageDC()
{
  bitmapImageContent = new wxBitmap( dataImage.imageWidth, dataImage.imageHeight );
  memoryImageContent = new wxMemoryDC( *bitmapImageContent );
  //
  memoryImageContent->SetFont( dataImage.textFont );
  //
  if( dataImage.backgroundMode == TBackgroundMode::BACKGROUND_SOLID_DARK )
  {
    memoryImageContent->SetBackground( wxBrush( *wxBLACK ) );
    memoryImageContent->SetTextBackground( *wxWHITE );
    memoryImageContent->SetTextForeground( *wxWHITE );
    memoryImageContent->SetPen( wxPen( *wxWHITE ) );
  }
  else
  {
    memoryImageContent->SetBackground( *wxWHITE_BRUSH );
    memoryImageContent->SetTextForeground( *wxBLACK );
  }
  //
  memoryImageContent->Clear();
  //
  // Mask for legend
  bitmapImageMask = new wxBitmap( dataImage.imageWidth, dataImage.imageHeight );
  memoryImageMask = new wxMemoryDC( *bitmapImageMask );
  memoryImageMask->SetBackground( *wxWHITE_BRUSH );
  memoryImageMask->SetBrush( *wxBLACK_BRUSH );
  memoryImageMask->SetPen( *wxBLACK_PEN );
  memoryImageMask->SetTextForeground( *wxBLACK );
  memoryImageMask->SetFont( dataImage.textFont );
  memoryImageMask->Clear();
}


void ScaleImageGenerator::drawImage()
{
  if( scaleType == TScaleImageType::TScaleImageGradientColor || scaleType == TScaleImageType::TScaleImageGradientColorNotNull ||
      scaleType == TScaleImageType::TScaleImageAlternativeGradientColor )
    drawImageGradientColor();
  else
    drawImageCodeColor();
}


void ScaleImageGenerator::drawImageCodeColor()
{
  int posX = dataImage.rectMargin + dataImage.titleMargin;
  int posY = dataImage.titleMargin;
  //
  for( auto it = semanticValuesLabels.begin(); it != semanticValuesLabels.end(); ++it )
  {
    drawRectangle( semanticValuesColors[ it->first ], posX, posY );
    //
    posX += dataImage.widthRect + dataImage.rectMargin;
    //
    wxSize titleSize = memoryImageContent->GetTextExtent( semanticValuesLabels[ it->first ] );

    int textX = posX; // 2 píxeles de margen
    int textY = posY + ( dataImage.heightRect - titleSize.GetHeight() ) / 2;

    memoryImageContent->DrawText( semanticValuesLabels[ it->first ], textX, textY );
    memoryImageMask->DrawText( semanticValuesLabels[ it->first ], textX, textY );
    //
    if( orientation == TScaleImageOrientation::TScaleImageOrientationVertical )
    {
      posX = dataImage.rectMargin + dataImage.titleMargin;
      posY += dataImage.rectMargin + dataImage.widthRect;
    }
    else
    {
      titleSize = memoryImageContent->GetTextExtent( semanticValuesLabels[ it->first ] );
      posX += dataImage.rectMargin + dataImage.heightRect + titleSize.GetWidth();
    }
  }
}


void ScaleImageGenerator::drawImageGradientColor()
{
  // *** 1. Draw color scale ****************************************************************
  int currentElement      = 0;
  int posX                = dataImage.rectMargin + dataImage.titleMargin;
  int posY                = dataImage.titleMargin;
  const int middleElement = ( totalSquaresGradientColor / 2 ) + 1;
  //
  auto visitorBelowOutlier = overloads{ []( auto* myWindow )
                                        {
                                          return myWindow->getSemanticColor().getBelowOutlierColor();
                                        } };
  auto visitorAboveOutlier = overloads{ []( auto* myWindow )
                                        {
                                          return myWindow->getSemanticColor().getAboveOutlierColor();
                                        } };
  //
  wxString tmpSemanticValueLabel = wxT( "<" );
  //
  drawRectangle( std::visit( visitorBelowOutlier, myWindow ), posX, posY );
  //
  if( orientation == TScaleImageOrientation::TScaleImageOrientationVertical )
  {
    memoryImageContent->DrawText( tmpSemanticValueLabel, posX + dataImage.heightRect + SIZE_MARK_LABEL, posY );
    memoryImageMask->DrawText( tmpSemanticValueLabel, posX + dataImage.heightRect + SIZE_MARK_LABEL, posY );
    posY += dataImage.heightRect + dataImage.rectMargin;
  }
  else
  {
    memoryImageContent->DrawText( tmpSemanticValueLabel, posX, posY + dataImage.heightRect + SIZE_MARK_LABEL );
    memoryImageMask->DrawText( tmpSemanticValueLabel, posX, posY + dataImage.heightRect + SIZE_MARK_LABEL );
    posX = posX + dataImage.widthRect + dataImage.rectMargin;
  }
  //

  //
  for( auto it = semanticValuesColors.begin(); it != semanticValuesColors.end(); ++it )
  {
    ++currentElement;
    //
    if( it == semanticValuesColors.begin() )
    {
      drawRectangle( it->second, posX, posY );
      drawLabelGradient( semanticValuesLabels[ it->first ], TPosition::FIRST, posX, posY );
    }
    else if( std::next( it ) == semanticValuesColors.end() )
    {
      drawRectangle( it->second, posX, posY );
      drawLabelGradient( semanticValuesLabels[ it->first ], TPosition::LAST, posX, posY );
    }
    else if( currentElement == middleElement )
    {
      drawRectangle( it->second, posX, posY );
      drawLabelGradient( semanticValuesLabels[ it->first ], TPosition::MIDDLE, posX, posY );
    }
    else
    {
      drawRectangle( it->second, posX, posY );
    }
    //
    if( orientation == TScaleImageOrientation::TScaleImageOrientationVertical )
    {
      posX = dataImage.rectMargin + dataImage.titleMargin;
      posY += dataImage.heightRect;
    }
    else if( orientation == TScaleImageOrientation::TScaleImageOrientationHorizontal )
    {
      posX += dataImage.widthRect;
    }
  }


  if( orientation == TScaleImageOrientation::TScaleImageOrientationVertical )
  {
    posX = dataImage.rectMargin + dataImage.titleMargin;
    posY += dataImage.rectMargin;

    tmpSemanticValueLabel = wxT( "<" );

    wxSize labelSize = memoryImageContent->GetTextExtent( tmpSemanticValueLabel );

    drawRectangle( std::visit( visitorAboveOutlier, myWindow ), posX, posY );

    memoryImageContent->DrawText( tmpSemanticValueLabel,
                                  posX + dataImage.widthRect + SIZE_MARK_LABEL,
                                  posY + dataImage.heightRect - labelSize.GetHeight() );
    memoryImageMask->DrawText( tmpSemanticValueLabel,
                               posX + dataImage.widthRect + SIZE_MARK_LABEL,
                               posY + dataImage.heightRect - labelSize.GetWidth() );
  }
  else if( orientation == TScaleImageOrientation::TScaleImageOrientationHorizontal )
  {
    posX += dataImage.rectMargin;

    tmpSemanticValueLabel = wxT( "<" );

    wxSize labelSize = memoryImageContent->GetTextExtent( tmpSemanticValueLabel );

    drawRectangle( std::visit( visitorAboveOutlier, myWindow ), posX, posY );
    memoryImageContent->DrawText( tmpSemanticValueLabel,
                                  posX + dataImage.widthRect - labelSize.GetWidth(),
                                  posY + dataImage.heightRect + SIZE_MARK_LABEL );
    memoryImageMask->DrawText( tmpSemanticValueLabel,
                               posX + dataImage.widthRect - labelSize.GetWidth(),
                               posY + dataImage.heightRect + SIZE_MARK_LABEL );
  }
}


void ScaleImageGenerator::drawRectangle( rgb semanticColour, int posX, int posY )
{
  memoryImageContent->SetBrush( wxColour( semanticColour.red, semanticColour.green, semanticColour.blue ) );
  memoryImageContent->DrawRectangle( posX, posY, dataImage.widthRect, dataImage.heightRect );
  //
  memoryImageMask->DrawRectangle( posX, posY, dataImage.widthRect, dataImage.heightRect );
}


void ScaleImageGenerator::drawLabelGradient( wxString semanticValueLabel, TPosition alignmentLabel, int posX, int posY )
{
  wxSize labelSize = memoryImageContent->GetTextExtent( semanticValueLabel );
  //
  int shiftLeftLabel = 0, shiftLeftLine = 0;
  //
  switch( alignmentLabel )
  {
    case TPosition::FIRST:
      shiftLeftLine  = 0;
      shiftLeftLabel = 0;
      break;
    case TPosition::MIDDLE:
      shiftLeftLine  = ceil( (double)dataImage.widthRect / 2 );
      shiftLeftLabel = ceil( (double)dataImage.widthRect / 2 - (double)labelSize.GetWidth() / 2 );
      break;
    case TPosition::LAST:
      shiftLeftLabel = dataImage.widthRect - labelSize.GetWidth();
      shiftLeftLine  = dataImage.widthRect - 1;
      break;
    default:
      break;
  }
  //
  if( orientation == TScaleImageOrientation::TScaleImageOrientationHorizontal )
  {
    memoryImageContent->DrawLine( posX + shiftLeftLine,
                                  posY + dataImage.heightRect,
                                  posX + shiftLeftLine,
                                  posY + dataImage.heightRect + SIZE_MARK_LABEL );
    memoryImageMask->DrawLine( posX + shiftLeftLine,
                               posY + dataImage.heightRect,
                               posX + shiftLeftLine,
                               posY + dataImage.heightRect + SIZE_MARK_LABEL );
    //
    memoryImageContent->DrawText( semanticValueLabel, posX + shiftLeftLabel, posY + dataImage.heightRect + SIZE_MARK_LABEL + 1 );
    memoryImageMask->DrawText( semanticValueLabel, posX + shiftLeftLabel, posY + dataImage.heightRect + SIZE_MARK_LABEL + 1 );
  }
  else
  {
    memoryImageContent->DrawLine( posX + dataImage.widthRect,
                                  posY + shiftLeftLine,
                                  posX + dataImage.widthRect + SIZE_MARK_LABEL,
                                  posY + shiftLeftLine );
    memoryImageMask->DrawLine( posX + dataImage.widthRect, posY + shiftLeftLine, posX + dataImage.widthRect + SIZE_MARK_LABEL, posY + shiftLeftLine );

    //
    memoryImageContent->DrawText( semanticValueLabel, posX + dataImage.widthRect + SIZE_MARK_LABEL + 1, posY );
    memoryImageMask->DrawText( semanticValueLabel, posX + dataImage.widthRect + SIZE_MARK_LABEL + 1, posY );
  }
}


wxImage ScaleImageGenerator::bitmapToImage()
{
  memoryImageContent->SelectObject( wxNullBitmap );
  //
#ifndef __WXMAC__
  // TODO: avoid to create/handle scaleMaskDC in all the other methods if wxTRANSPARENT
  if( dataImage.backgroundMode == TBackgroundMode::BACKGROUND_TRANSPARENT )
  {
    memoryImageMask->SelectObject( wxNullBitmap );
    wxMask* mask = new wxMask( *bitmapImageMask, *wxWHITE );
    bitmapImageContent->SetMask( mask );
  }
#endif

  wxImage scaleImage( bitmapImageContent->ConvertToImage() );

  return scaleImage;
}

// TODO: refactor using CRPT
bool ScaleImageGenerator::copyImageToClipboard( wxBitmap* whichBitmapImage )
{
  bool copyDone = false;
  if( whichBitmapImage == nullptr )
    whichBitmapImage = bitmapImageContent;

  if( wxTheClipboard->Open() )
  {
    wxImage timelineImage( whichBitmapImage->ConvertToImage() );
    copyDone = wxTheClipboard->SetData( new wxBitmapDataObject( *whichBitmapImage ) );

    wxTheClipboard->Close();
  }

  return copyDone;
}

void ScaleImageGenerator::saveImage( wxString imageName, bool appendLegendSuffix, bool whichCopyToClipboard )
{
  wxImage baseLayer = bitmapToImage();

  if( whichCopyToClipboard )
    copyImageToClipboard();
  else
  {
    wxString tmpSuffix;
    if( appendLegendSuffix )
    {
      tmpSuffix = legendSuffix + _( "." ) + wxString::FromUTF8( LabelConstructor::getImageFileSuffix( filterIndex ).c_str() );
    }
    else
    {
      tmpSuffix = _( "." ) + wxString::FromUTF8( LabelConstructor::getImageFileSuffix( filterIndex ).c_str() );
    }
    wxString imagePath = imageName + tmpSuffix;
    baseLayer.SaveFile( imagePath, imageFormatTypeToBitmapType( filterIndex ) );
  }
}


void ScaleImageGenerator::processImage( std::optional< int > wantedWidth )
{
  computeMaxSemanticLabelsSize();
  if( scaleType == TScaleImageType::TScaleImageGradientColor || scaleType == TScaleImageType::TScaleImageGradientColorNotNull ||
      scaleType == TScaleImageType::TScaleImageAlternativeGradientColor )
  {
    if( wantedWidth != std::nullopt )
    {
      totalSquaresGradientColor = wantedWidth.value() / dataImage.heightRect;
    }
    this->sortSemanticValuesGradientColors();
  }
  //
  if( wantedWidth == std::nullopt )
    this->computeGeneratedImageSize();
  else
    this->computeGeneratedImageSize( wantedWidth.value() );
  //

  this->createImageDC();

  this->drawImage();
}


wxString ScaleImageGenerator::getLegendSuffix()
{
  return legendSuffix;
}


void ScaleImageGenerator::updateLegendSuffix()
{
  if( scaleType == TScaleImageType::TScaleImageGradientColor )
    legendSuffix = _( ".gradient_legend" );
  else if( scaleType == TScaleImageType::TScaleImageGradientColorNotNull )
    legendSuffix = _( ".nn_gradient_legend" );
  else if( scaleType == TScaleImageType::TScaleImageAlternativeGradientColor )
    legendSuffix = _( ".alt_gradient_legend" );
  else
    legendSuffix += _( ".code_legend" );
}
