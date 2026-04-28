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
                         TBackgroundMode backgroundColor,
                         bool whichMergedImages = false );

    ScaleImageGenerator( Timeline* whichMyWindow,
                         const std::map< TSemanticValue, rgb >& whichSemanticValues,
                         wxColour whichBackground,
                         wxColour whichForeground,
                         wxFont whichTextFont,
                         TImageFormat& whichImageType,
                         TScaleImageType scaleType,
                         TScaleImageOrientation orientation,
                         TBackgroundMode backgroundColor,
                         bool whichMergedImages = false );
    ~ScaleImageGenerator();

    wxBitmap* getBitmap()
    {
      return bitmapImageContent;
    }

    bool setTrasformToShort( bool a_shortLabel )
    {
      if( shortLabel == a_shortLabel )
        return false;


      shortLabel = a_shortLabel;

      return true;
    }

    TScaleImageType getLegendType()
    {
      return scaleType;
    }


    wxString getLegendSuffix();

    void updateLegendSuffix();

    void initializeConfiguration();

    std::map< TSemanticValue, wxString > getSemanticValues()
    {
      if( shortLabel )
        return semanticValuesLabelsShort;
      else
        return semanticValuesLabelsLong;
    }

    void setSelectedSemanticValues( std::vector< int > selectedValues )
    {
      semanticValuesSelectedPositions = selectedValues;
    }

    std::vector< int > getSelectedSemanticValues()
    {
      return semanticValuesSelectedPositions;
    }

    void setImageOrientation( TScaleImageOrientation orientation );

    void computeGeneratedImageSize( int whichWantedWidth = 0 );
    void computeMaxSemanticLabelsSize();
    void computeSemanticLabels();

    void sortSemanticValuesGradientColors();

    void createImageDC();
    void drawImage();

    void drawImageCodeColor();
    void drawImageGradientColor();


    void saveImage( wxString whichFileName, bool appendLegendSuffix );
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
    std::map< TSemanticValue, wxString > semanticValuesLabelsShort;
    std::map< TSemanticValue, wxString > semanticValuesLabelsLong;


    std::vector< int > semanticValuesSelectedPositions;


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
    bool shortLabel = false;
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
    wxBitmap* getWindowsImageBitmap();
    wxBitmap* getTimelineScaleImage();
    wxBitmap* getScaleImageBitmap();

    ScaleImageGenerator* getScaleWindowsImage();
    ScaleImageGenerator* getScaleImage();

    wxBitmap* getScaleDataImageBitmap();
    ScaleImageGenerator* getScaleDataImage();

    wxString getLegendSuffix();

    TImageFormat getImageFormat();

    void setImageFormat( TImageFormat filterIndex );
    void setImageOrientation( TScaleImageOrientation orientation );


    void transformToShort( bool isShort );

    void generatescaleImageDataWindows( std::map< TSemanticValue, rgb > semanticValuesToColor, Timeline* myWindow, bool derivedFromHisto = false );
    void generatescaleImageDataWindows( std::map< TSemanticValue, rgb > semanticValuesToColor, Timeline* myWindow, Histogram* myHistogram );

    void generateWindowsImage( const std::string& windowsName, const std::string& windowTraceName );
    void generateWindowsImageGrid( wxGrid* gridImage, const std::string& windowsName, const std::string& windowTraceName );
    void generateScaleWindowsImage( std::map< TSemanticValue, rgb > semanticValuesToColor, Timeline* myWindow, Histogram* myHistogram = nullptr );
    void generateScaleWindowsImage();

    void saveImageWindows( wxString whichFileName );
    void saveImageWindowScale( wxString whichFileName );
    void saveImageScale( wxString whichFileName, bool appendLegendSuffix );
    void saveImageControlScale( wxString imageName, bool appendLegendSuffix );

  private:
    wxBitmap* bitmapTimeline;

    wxBitmap* bitmapImageTimeline;
    wxMemoryDC* memoryImageTimeline;

    wxBitmap* bitmapImageTimelineScale;
    wxMemoryDC* memoryImageTimelineScale;

    ScaleImageGenerator* scaleImage     = nullptr;
    ScaleImageGenerator* scaleImageData = nullptr;

    ScaleImageGenerator* scaleWindowsImage = nullptr;

    TScaleImageOrientation orientation = TScaleImageOrientation::TScaleImageOrientationHorizontal;

    TImageFormat filterIndex;

    wxColour imageBackground;
    wxColour imageForeground;

    wxFont imageTextFont;

    TScaleImageType typeScale;
    TScaleImageOrientation orientationScale = TScaleImageOrientation::TScaleImageOrientationHorizontal;
};


wxBitmapType imageFormatTypeToBitmapType( const TImageFormat& whichFilterIndex );