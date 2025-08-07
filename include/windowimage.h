#pragma once

#include "gtimeline.h"
#include "paraverconfig.h"
#include "paravertypes.h"
#include "wx/sizer.h"
#include "wx/string.h"

#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/grid.h>

enum class TPosition
{
  FIRST = 0,
  MIDDLE,
  LAST,
  ANY
};

enum class TBackgroundMode
{
  BACKGROUND_SOLID_DARK,
  BACKGROUND_SOLID_LIGHT,
  BACKGROUND_TRANSPARENT,
  BACKGROUND_GRADIENT
};

typedef struct TImageScaleData
{
    int titleMargin;
    int outlierMargin;

    int rectMargin;

    int widthRect;
    int heightRect;

    int rectangleStepY;
    int rectangleStepX;

    int imageWidth;
    int imageHeight;

    bool symbolicDesc;
    bool drawOutliers;

    int precision;

    wxFont textFont;
    TBackgroundMode backgroundMode;

} TImageScaleData;

enum class TScaleImageType
{
  TScaleImageCodeColor,
  TScaleImageGradientColor,
  TScaleImageGradientColorNotNull,
  TScaleImageAlternativeGradientColor,
  TScaleImageGradientFusedLines
};

enum class TScaleImageOrientation
{
  TScaleImageOrientationVertical,
  TScaleImageOrientationHorizontal
};


class ScaleImageGenerator
{
  public:
    ScaleImageGenerator( Histogram* whichMyWindow,
                         Timeline* whichMyDataWindow,
                         const std::map< TSemanticValue, rgb >& whichSemanticValues,
                         wxColour whichBackground,
                         wxColour whichForeground,
                         wxFont whichTextFont,
                         TImageFormat& whichImageType,
                         TScaleImageType scaleType,
                         TScaleImageOrientation orientation,
                         bool whichMergedImages = false );

    ScaleImageGenerator( Timeline* whichMyWindow,
                         const std::map< TSemanticValue, rgb >& whichSemanticValues,
                         wxColour whichBackground,
                         wxColour whichForeground,
                         wxFont whichTextFont,
                         TImageFormat& whichImageType,
                         TScaleImageType scaleType,
                         TScaleImageOrientation orientation,
                         bool whichMergedImages = false );
    ~ScaleImageGenerator();

    wxBitmap* getBitmap()
    {
      return bitmapImageContent;
    }

    wxString getLegendSuffix();

    void updateLegendSuffix();

    void initializeConfiguration();

    void computeGeneratedImageSize( int whichWantedWidth = 0 );
    void computeMaxSemanticLabelsSize();

    void sortSemanticValuesGradientColors();

    void createImageDC();
    void drawImage();

    void drawImageCodeColor();
    void drawImageGradientColor();


    void saveImage( wxString whichFileName, bool appendLegendSufix );
    void processImage( std::optional< int > wantedWidth = std::nullopt );


  private:
    void drawLabelGradient( wxString semanticValueLabel, TPosition alignmentLabel, int posX, int posY );
    void drawRectangle( rgb semanticColour, int posX, int posY );

    wxImage bitmapToImage();


  private:
    WindowGenericItem myWindow;

    Timeline* myDataWindow;
    TImageFormat filterIndex;

    std::map< TSemanticValue, rgb > semanticValuesColors;
    std::map< TSemanticValue, wxString > semanticValuesLabels;

    wxBitmap* bitmapImageContent = nullptr;
    wxMemoryDC* memoryImageContent;

    wxBitmap* bitmapImageMask;
    wxMemoryDC* memoryImageMask;

    TSemanticValue currentMin;
    TSemanticValue currentMax;

    TImageScaleData dataImage;

    TScaleImageType scaleType;
    TScaleImageOrientation orientation;

    wxSize maxLabelSize;

    bool mergedImages;
    int totalSquaresGradientColor;

    wxString legendSuffix;
};


class WindowImageGenerator
{
  public:
    WindowImageGenerator( wxBitmap* wichBitmapTimeline,
                          // WindowGenericItem genericItem,
                          wxColour whichBackground,
                          wxColour whichForeground,
                          wxFont whichTextFont,
                          TImageFormat wichFilterIndex,
                          bool whichMergedImages = false );
    ~WindowImageGenerator();

  public:
    wxBitmap* getWindowsImage();
    wxBitmap* getTimelineScaleImage();
    wxBitmap* getScaleImage();
    wxBitmap* getScaleDataImage();
    wxString getLegendSuffix();

    TImageFormat getImageFormat();
    void setImageFormat( TImageFormat filterIndex );

    void generatescaleImageDataWindows( std::map< TSemanticValue, rgb > semanticValuesToColor, Timeline* myWindow );
    void generatescaleImageDataWindows( std::map< TSemanticValue, rgb > semanticValuesToColor, Timeline* myWindow, Histogram* myHistogram );

    void generateWindowsImage( const std::string& windowsName, const std::string& windowTraceName );
    void generateWindowsImageGrid( wxGrid* gridImage, const std::string& windowsName, const std::string& windowTraceName );
    void generateScaleWindowsImage( std::map< TSemanticValue, rgb > semanticValuesToColor, Timeline* myWindow );

    void saveImageWindows( wxString whichFileName );
    void saveImageWindowScale( wxString whichFileName );
    void saveImageScale( wxString whichFileName, bool appendLegendSufix );
    void saveImageControlScale( wxString imageName, bool appendLegendSufix );

  private:
    wxBitmap* bitmapTimeline;

    wxBitmap* bitmapImageTimeline;
    wxMemoryDC* memoryImageTimeline;

    wxBitmap* bitmapImageTimelineScale;
    wxMemoryDC* memoryImageTimelineScale;

    ScaleImageGenerator* scaleImage     = nullptr;
    ScaleImageGenerator* scaleImageData = nullptr;

    TImageFormat filterIndex;

    wxColour imageBackground;
    wxColour imageForeground;

    wxFont imageTextFont;

    TScaleImageType typeScale;
    TScaleImageOrientation orientationScale;
};


void imageFormatTypeToBitmapType( const TImageFormat& whichFilterIndex, wxBitmapType& imageType );