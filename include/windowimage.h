#pragma once

#include "gtimeline.h"
#include "paraverconfig.h"
#include "paravertypes.h"
#include "wx/sizer.h"
#include "wx/string.h"

#include <wx/dc.h>
#include <wx/dcclient.h>

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
    ScaleImageGenerator( Timeline* whichMyWindow,
                         const std::map< TSemanticValue, rgb >& whichSemanticValues,
                         wxColour whichBackground,
                         wxColour whichForeground,
                         wxFont whichTextFont,
                         TImageFormat& whichImageType,
                         TScaleImageType scaleType,
                         TScaleImageOrientation orientation,
                         bool whichMergedImages = false );

    ~ScaleImageGenerator() = default;

    wxBitmap* getBitmap()
    {
      return bitmapImageContent;
    }

    void initializeConfiguration();

    void computeGeneratedImageSize( int whichWantedWidth = 0 );
    void computeMaxSemanticLabelsSize();

    void sortSemanticValuesGradientColors();

    void createImageDC();
    void drawImage();

    void drawImageCodeColor();
    void drawImageGradientColor();


    void saveImage( wxString imagePath );
    void processImage();


  private:
    void drawLabelGradient( wxString semanticValueLabel, TPosition alignmentLabel, int posX, int posY );
    void drawRectangle( rgb semanticColour, int posX, int posY );

    wxImage bitmapToImage();


  private:
    Timeline* myWindow;
    TImageFormat filterIndex;

    std::map< TSemanticValue, rgb > semanticValuesColors;
    std::map< TSemanticValue, wxString > semanticValuesLabels;

    wxBitmap* bitmapImageContent;
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
};


class WindowImageGenerator
{
  public:
    WindowImageGenerator( wxBitmap* wichBitmapTimeline,
                          Timeline* whichMyWindow,
                          wxColour whichBackground,
                          wxColour whichForeground,
                          wxFont whichTextFont,
                          TImageFormat wichFilterIndex,
                          bool whichMergedImages = false );

  public:
    wxBitmap* getTimelineImage();
    wxBitmap* getTimelineScaleImage();
    wxBitmap* getScaleImage();

    void generateScaleImage( std::map< TSemanticValue, rgb > semanticValuesToColor);
    void generateTimelineImage();
    void generateScaleTimelineImage();

    void saveImageTimeline();
    void saveImageTimelineScale();
    void saveImageScale();

  private:
    wxBitmap* bitmapTimeline;

    wxBitmap* bitmapImageTimeline;
    wxMemoryDC* memoryImageTimeline;

    wxBitmap* bitmapImageTimelineScale;
    wxMemoryDC* memoryImageTimelineScale;

    ScaleImageGenerator* scaleImage;

    TImageFormat filterIndex;

    Timeline* myWindow;

    wxColour imageBackground;
    wxColour imageForeground;

    wxFont imageTextFont;

    TScaleImageType typeScale;
    TScaleImageOrientation orientationScale;
};
