#include "windowimage.h"

#include "labelconstructor.h"

#include <iterator>
#include <vector>
#include <wx/filename.h>

#define SIZE_MARK_LABEL 4

void imageFormatTypeToBitmapType( const TImageFormat& wichFilterIndex, wxBitmapType& imageType )
{
  switch( wichFilterIndex )
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
}

WindowImageGenerator::WindowImageGenerator( wxBitmap* wichBitmapTimeline,
                                            // WindowGenericItem wichGenericItem,
                                            wxColour whichBackground,
                                            wxColour whichForeground,
                                            wxFont whichTextFont,
                                            TImageFormat wichFilterIndex,
                                            bool whichMergedImages )

{
  // genericItem     = wichGenericItem;
  bitmapTimeline  = wichBitmapTimeline;
  imageBackground = whichBackground;
  imageForeground = whichForeground;
  imageTextFont   = whichTextFont;
  filterIndex     = wichFilterIndex;
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

wxBitmap* WindowImageGenerator::getWindowsImage()
{
  return bitmapImageTimeline;
}
wxBitmap* WindowImageGenerator::getTimelineScaleImage()
{
  return bitmapImageTimelineScale;
}
wxBitmap* WindowImageGenerator::getScaleImage()
{
  if( scaleImage == nullptr )
    return nullptr;
  return scaleImage->getBitmap();
}
wxBitmap* WindowImageGenerator::getScaleDataImage()
{
  if( scaleImageData == nullptr )
    return nullptr;
  return scaleImageData->getBitmap();
}

wxString WindowImageGenerator::getLegendSuffix()
{
  return scaleImage->getLegendSuffix();
}

TImageFormat WindowImageGenerator::getImageFormat()
{
  return filterIndex;
}
void WindowImageGenerator::setImageFormat( TImageFormat wichFilterIndex )
{
  filterIndex = wichFilterIndex;
}

void WindowImageGenerator::generatescaleImageDataWindows( std::map< TSemanticValue, rgb > semanticValuesToColor,
                                                          Timeline* myWindow,
                                                          Histogram* myHistogram )
{
  std::map< TSemanticValue, rgb > scaleValues = semanticValuesToColor;

  if( myHistogram->getColorMode() == TColorFunction::GRADIENT )
  {
    typeScale        = TScaleImageType::TScaleImageGradientColor;
    orientationScale = TScaleImageOrientation::TScaleImageOrientationHorizontal;
  }
  else if( myHistogram->getColorMode() == TColorFunction::NOT_NULL_GRADIENT )
  {
    typeScale        = TScaleImageType::TScaleImageGradientColorNotNull;
    orientationScale = TScaleImageOrientation::TScaleImageOrientationHorizontal;
  }
  else if( myHistogram->getColorMode() == TColorFunction::ALTERNATIVE_GRADIENT )
  {
    typeScale        = TScaleImageType::TScaleImageAlternativeGradientColor;
    orientationScale = TScaleImageOrientation::TScaleImageOrientationHorizontal;
  }
  else if( myHistogram->getColorMode() == TColorFunction::CODE_COLOR )
  {
    typeScale        = TScaleImageType::TScaleImageCodeColor;
    orientationScale = TScaleImageOrientation::TScaleImageOrientationVertical;
  }

  scaleImageData = new ScaleImageGenerator( myHistogram,
                                            myWindow,
                                            scaleValues,
                                            imageBackground,
                                            imageForeground,
                                            imageTextFont,
                                            filterIndex,
                                            typeScale,
                                            orientationScale );

  scaleImageData->initializeConfiguration();
  scaleImageData->processImage();
}

void WindowImageGenerator::generatescaleImageDataWindows( std::map< TSemanticValue, rgb > semanticValuesToColor, Timeline* myWindow )
{
  std::map< TSemanticValue, rgb > scaleValues = semanticValuesToColor;

  if( myWindow->isGradientColorSet() )
  {
    typeScale        = TScaleImageType::TScaleImageGradientColor;
    orientationScale = TScaleImageOrientation::TScaleImageOrientationHorizontal;
  }
  else if( myWindow->isNotNullGradientColorSet() )
  {
    typeScale        = TScaleImageType::TScaleImageGradientColorNotNull;
    orientationScale = TScaleImageOrientation::TScaleImageOrientationHorizontal;
  }
  else if( myWindow->isAlternativeGradientColorSet() )
  {
    typeScale        = TScaleImageType::TScaleImageAlternativeGradientColor;
    orientationScale = TScaleImageOrientation::TScaleImageOrientationHorizontal;
  }
  else if( myWindow->isCodeColorSet() )
  {
    typeScale        = TScaleImageType::TScaleImageCodeColor;
    orientationScale = TScaleImageOrientation::TScaleImageOrientationHorizontal;
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

    typeScale        = TScaleImageType::TScaleImageGradientFusedLines;
    orientationScale = TScaleImageOrientation::TScaleImageOrientationVertical;

    std::map< TSemanticValue, rgb > scaleValues = tmpObjects;
  }

  scaleImage =
    new ScaleImageGenerator( myWindow, scaleValues, imageBackground, imageForeground, imageTextFont, filterIndex, typeScale, orientationScale );

  scaleImage->initializeConfiguration();
  scaleImage->processImage();
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
  drawStackedImages( timelineDC );
#else
  wxMemoryDC timelineDC( *bitmapTimeline );
#endif
  memoryImageTimeline->Blit( 0, titleHeigth, bitmapTimeline->GetWidth(), bitmapTimeline->GetHeight(), &timelineDC, 0, 0 );
}

void WindowImageGenerator::generateWindowsImageGrid( wxGrid* gridImage, const std::string& windowsName, const std::string& windowTraceName )
{
  int imageWidth  = 0;
  int imageHeight = 0;

  auto sumSize = []( const auto& sizes, auto numElems, auto& totalSize )
  {
    for( unsigned int c = 0; c < numElems; ++c )
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

  memoryImageTimeline->Blit( 0, 0, titleWidth, titleHeigth, &titleDC, 0, 0 );

  gridImage->Render( *memoryImageTimeline, wxPoint( 0, titleHeigth ) );
}


void WindowImageGenerator::generateScaleWindowsImage( std::map< TSemanticValue, rgb > semanticValuesToColor, Timeline* myWindow )
{
  ScaleImageGenerator
    tmpScaleImage( myWindow, semanticValuesToColor, imageBackground, imageForeground, imageTextFont, filterIndex, typeScale, orientationScale, true );

  tmpScaleImage.initializeConfiguration();

  std::optional< int > wantedWidth = bitmapTimeline->GetWidth();

  tmpScaleImage.processImage( wantedWidth );

  int imageMargin = 5;
  int imageHeigth = bitmapTimeline->GetHeight() + imageTextFont.GetPointSize() + ( 2 * imageMargin ); // up + down margins + text + image
  int imageWidth  = bitmapTimeline->GetWidth();

  int totalHeigth = imageHeigth + tmpScaleImage.getBitmap()->GetHeight();
  int totalWidth  = imageWidth > tmpScaleImage.getBitmap()->GetWidth() ? imageWidth : tmpScaleImage.getBitmap()->GetWidth();

  bitmapImageTimelineScale = new wxBitmap( totalWidth, totalHeigth );
  memoryImageTimelineScale = new wxMemoryDC( *bitmapImageTimelineScale );

  memoryImageTimelineScale->SetBackground( imageBackground );
  memoryImageTimelineScale->Clear();

  int posX = 0;
  int posY = 0;

  memoryImageTimelineScale->Blit( posX, posY, imageWidth, imageHeigth, memoryImageTimeline, 0, 0 );

  posX = totalWidth - tmpScaleImage.getBitmap()->GetWidth();
  posY = imageHeigth;

  wxMemoryDC tmpScaleMemory( *tmpScaleImage.getBitmap() );

  memoryImageTimelineScale->Blit( posX, posY, tmpScaleImage.getBitmap()->GetWidth(), tmpScaleImage.getBitmap()->GetHeight(), &tmpScaleMemory, 0, 0 );
}

void WindowImageGenerator::saveImageWindows( wxString whichFileName )
{
  wxBitmapType imageType;
  imageFormatTypeToBitmapType( filterIndex, imageType );

  wxImage timelineImage( bitmapImageTimeline->ConvertToImage() );

  // Save timeline image without scale
  timelineImage.SaveFile( whichFileName, imageType );
}
void WindowImageGenerator::saveImageWindowScale( wxString whichFileName )
{
  wxImage timelineImageScale( bitmapImageTimelineScale->ConvertToImage() );


  wxString currentFormat = wxString::FromUTF8( LabelConstructor::getImageFileSuffix( TImageFormat( filterIndex ) ).c_str() );
  wxString tmpScaledTimelinePath =
    wxFileName( whichFileName ).GetPathWithSep() + wxFileName( whichFileName ).GetName() + wxString( _( ".w_legend." ) ) + currentFormat;

  wxBitmapType imageType;
  imageFormatTypeToBitmapType( filterIndex, imageType );

  // Save timeline image without scale
  timelineImageScale.SaveFile( tmpScaledTimelinePath, imageType );
}

void WindowImageGenerator::saveImageControlScale( wxString imageName, bool appendLegendSufix )
{
  scaleImageData->saveImage( imageName, appendLegendSufix );
}

void WindowImageGenerator::saveImageScale( wxString imageName, bool appendLegendSufix )
{
  scaleImage->saveImage( imageName, appendLegendSufix );
}


ScaleImageGenerator::ScaleImageGenerator( Histogram* whichMyWindow,
                                          Timeline* whichmyDataWindow,
                                          const std::map< TSemanticValue, rgb >& whichSemanticValues,
                                          wxColour whichBackground,
                                          wxColour whichForeground,
                                          wxFont whichTextFont,
                                          TImageFormat& whichImageType,
                                          TScaleImageType wichScaleType,
                                          TScaleImageOrientation wichOrientation,
                                          bool whichMergedImages )
  : myWindow( whichMyWindow )
  , semanticValuesColors( whichSemanticValues )
  , myDataWindow( whichmyDataWindow )
  , filterIndex( whichImageType )
  , dataImage()
  , scaleType( wichScaleType )
  , orientation( wichOrientation )
  , maxLabelSize( 0, 0 )
  , totalSquaresGradientColor( 21 )
  , mergedImages( whichMergedImages )
{
  // Inicializar dataImage con algunos valores lógicos o por defecto
  dataImage.precision = 2;

  // Inicializar punteros a nullptr
  bitmapImageContent = nullptr;
  memoryImageContent = nullptr;

  bitmapImageMask = nullptr;
  memoryImageMask = nullptr;

  dataImage.textFont = whichTextFont;

  semanticValuesLabels.clear();
  updateLegendSuffix();
}

ScaleImageGenerator::ScaleImageGenerator( Timeline* whichmyWindow,
                                          const std::map< TSemanticValue, rgb >& whichSemanticValues,
                                          wxColour whichBackground,
                                          wxColour whichForeground,
                                          wxFont whichTextFont,
                                          TImageFormat& whichImageType,
                                          TScaleImageType wichScaleType,
                                          TScaleImageOrientation wichOrientation,
                                          bool whichMergedImages )
  : myWindow( whichmyWindow )
  , myDataWindow( whichmyWindow )
  , semanticValuesColors( whichSemanticValues )
  , filterIndex( whichImageType )
  , dataImage()
  , scaleType( wichScaleType )
  , orientation( wichOrientation )
  , maxLabelSize( 0, 0 )
  , totalSquaresGradientColor( 21 )
  , mergedImages( whichMergedImages )
{
  // Inicializar dataImage con algunos valores lógicos o por defecto
  dataImage.precision = 2;

  // Inicializar punteros a nullptr
  bitmapImageContent = nullptr;
  memoryImageContent = nullptr;

  bitmapImageMask = nullptr;
  memoryImageMask = nullptr;

  dataImage.textFont = whichTextFont;

  // Inicializa los mapas de etiquetas semánticas( vacíos por ahora ) semanticValuesLabels.clear();
  updateLegendSuffix();
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
  auto visitorMaxiumY = overloads{ []( Timeline* myWindow )
                                   {
                                     return myWindow->getMaximumY();
                                   },
                                   []( Histogram* myWindow )
                                   {
                                     return myWindow->getMaxGradient();
                                   } };
  auto visitorMiniumY = overloads{ []( Timeline* myWindow )
                                   {
                                     return myWindow->getMinimumY();
                                   },
                                   []( Histogram* myWindow )
                                   {
                                     return myWindow->getMinGradient();
                                   } };
  //
  currentMin = std::visit( visitorMiniumY, myWindow );
  currentMax = std::visit( visitorMaxiumY, myWindow );
  //
  dataImage.symbolicDesc = false;
  dataImage.titleMargin  = 5;
  dataImage.rectMargin   = 2 * dataImage.titleMargin;
  //
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
  if( filterIndex == TImageFormat::BMP || TImageFormat::JPG == filterIndex || mergedImages )
  {
    dataImage.backgroundMode = TBackgroundMode::BACKGROUND_SOLID_DARK;
  }
  else
  {
    dataImage.backgroundMode = TBackgroundMode::BACKGROUND_TRANSPARENT;
  }
}
//
//
void ScaleImageGenerator::computeMaxSemanticLabelsSize()
{
  // Dimensions: loop for every value to build labels and measure the maximum
  wxString curLabel;
  wxString maxLabel;
  //
  TSemanticValue semanticValueMaxLengthLabel;
  //
  size_t maxLengthLabel = 0;
  size_t curLengthLabel;
  //
  for( auto it = semanticValuesColors.begin(); it != semanticValuesColors.end(); ++it )
  {
    // Get Labels
    if( TScaleImageType::TScaleImageGradientFusedLines != scaleType )
    {
      semanticValuesLabels[ it->first ] =
        wxString::FromUTF8( LabelConstructor::semanticLabel( myDataWindow, it->first, dataImage.symbolicDesc, dataImage.precision, false ).c_str() );
    }
    else
    {
      if( auto timeline = std::get_if< Timeline* >( &myWindow ) )
      {
        semanticValuesLabels[ it->first ] =
          wxString::FromUTF8( LabelConstructor::objectLabel( it->first, ( *timeline )->getLevel(), ( *timeline )->getTrace() ).c_str() );
      }
    }
    //
    // Get Longest label
    curLengthLabel = semanticValuesLabels[ it->first ].Len();
    //
    if( maxLengthLabel < curLengthLabel )
    {
      maxLengthLabel              = curLengthLabel;
      semanticValueMaxLengthLabel = it->first;
    }
  }
  //
  wxBitmap maxLabelBitmap( 1920, 64 );
  wxMemoryDC maxLabelDC( maxLabelBitmap );
  maxLabelDC.SetFont( dataImage.textFont );
  maxLabelSize = maxLabelDC.GetTextExtent( semanticValuesLabels[ semanticValueMaxLengthLabel ] );
}
//
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
//
//
void ScaleImageGenerator::computeGeneratedImageSize( int whichWantedWidth )
{
  if( orientation == TScaleImageOrientation::TScaleImageOrientationVertical )
  {
    dataImage.imageHeight =
      ( semanticValuesLabels.size() - 1 ) * ( dataImage.heightRect + dataImage.rectMargin ) + dataImage.heightRect + 2 * dataImage.titleMargin;
    //
    if( dataImage.drawOutliers )
      dataImage.imageHeight += ( dataImage.heightRect + dataImage.titleMargin ) * 2;
    //
    dataImage.imageWidth = maxLabelSize.GetWidth() + ( 2 * dataImage.titleMargin ) + dataImage.widthRect + 2 * dataImage.rectMargin;
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

      for( auto it = semanticValuesColors.begin(); it != semanticValuesColors.end(); ++it )
      {
        wxSize titleSize = maxLabelDC.GetTextExtent( semanticValuesLabels[ it->first ] );
        dataImage.imageWidth += 3 * dataImage.rectMargin + dataImage.heightRect + titleSize.GetWidth();
      }
      dataImage.imageWidth += 2 * dataImage.rectMargin;
    }
  }
}
//
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
//
void ScaleImageGenerator::drawImage()
{
  if( scaleType == TScaleImageType::TScaleImageGradientColor || scaleType == TScaleImageType::TScaleImageGradientColorNotNull ||
      scaleType == TScaleImageType::TScaleImageAlternativeGradientColor )
    drawImageGradientColor();
  else
    drawImageCodeColor();
}
//
void ScaleImageGenerator::drawImageCodeColor()
{
  int posX = dataImage.rectMargin + dataImage.titleMargin;
  int posY = dataImage.titleMargin;
  //
  for( auto it = semanticValuesColors.begin(); it != semanticValuesColors.end(); ++it )
  {
    drawRectangle( it->second, posX, posY );
    //
    posX += dataImage.heightRect + dataImage.rectMargin;
    //
    memoryImageContent->DrawText( semanticValuesLabels[ it->first ], posX, posY );
    memoryImageMask->DrawText( semanticValuesLabels[ it->first ], posX, posY );
    //
    if( orientation == TScaleImageOrientation::TScaleImageOrientationVertical )
    {
      posX = dataImage.rectMargin + dataImage.titleMargin;
      posY += dataImage.rectMargin + dataImage.widthRect;
    }
    else
    {
      wxSize titleSize = memoryImageContent->GetTextExtent( semanticValuesLabels[ it->first ] );
      posX += dataImage.rectMargin + dataImage.heightRect + titleSize.GetWidth();
    }
  }
}
//
//
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
  memoryImageContent->DrawText( tmpSemanticValueLabel, posX, posY + dataImage.heightRect + SIZE_MARK_LABEL );
  memoryImageMask->DrawText( tmpSemanticValueLabel, posX, posY + dataImage.heightRect + SIZE_MARK_LABEL );
  //
  posX = posX + dataImage.widthRect + dataImage.rectMargin;
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
    posX += dataImage.widthRect;
  }

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
//
//
void ScaleImageGenerator::drawRectangle( rgb semanticColour, int posX, int posY )
{
  memoryImageContent->SetBrush( wxColour( semanticColour.red, semanticColour.green, semanticColour.blue ) );
  memoryImageContent->DrawRectangle( posX, posY, dataImage.widthRect, dataImage.heightRect );
  //
  memoryImageMask->DrawRectangle( posX, posY, dataImage.widthRect, dataImage.heightRect );
}
//
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
  memoryImageContent->DrawLine( posX + shiftLeftLine,
                                posY + dataImage.heightRect,
                                posX + shiftLeftLine,
                                posY + dataImage.heightRect + SIZE_MARK_LABEL );
  memoryImageMask->DrawLine( posX + shiftLeftLine, posY + dataImage.heightRect, posX + shiftLeftLine, posY + dataImage.heightRect + SIZE_MARK_LABEL );
  //
  memoryImageContent->DrawText( semanticValueLabel, posX + shiftLeftLabel, posY + dataImage.heightRect + SIZE_MARK_LABEL + 1 );
  memoryImageMask->DrawText( semanticValueLabel, posX + shiftLeftLabel, posY + dataImage.heightRect + SIZE_MARK_LABEL + 1 );
}
//
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

void ScaleImageGenerator::saveImage( wxString imageName, bool appendLegendSufix )
{
  wxBitmapType imageType;
  wxString tmpSuffix;

  imageFormatTypeToBitmapType( filterIndex, imageType );

  if( appendLegendSufix )
  {
    tmpSuffix = legendSuffix + _( "." ) + wxString::FromUTF8( LabelConstructor::getImageFileSuffix( filterIndex ).c_str() );
  }
  else
  {
    tmpSuffix = _( "." ) + wxString::FromUTF8( LabelConstructor::getImageFileSuffix( filterIndex ).c_str() );
  }

  wxString imagePath = imageName + tmpSuffix;

  wxImage baseLayer = bitmapToImage();
  baseLayer.SaveFile( imagePath, imageType );
}

void ScaleImageGenerator::processImage( std::optional< int > wantedWidth )
{
  if( scaleType == TScaleImageType::TScaleImageGradientColor || scaleType == TScaleImageType::TScaleImageGradientColorNotNull ||
      scaleType == TScaleImageType::TScaleImageAlternativeGradientColor )
  {
    this->sortSemanticValuesGradientColors();
  }
  this->computeMaxSemanticLabelsSize();
  //
  if( wantedWidth == std::nullopt )
    this->computeGeneratedImageSize();
  else
    this->computeGeneratedImageSize( wantedWidth.value() );
  //
  this->createImageDC();
  this->drawImage();
}
//
wxString ScaleImageGenerator::getLegendSuffix()
{
  return legendSuffix;
}
//
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
