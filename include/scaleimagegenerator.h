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
 * @file scaleimagegenerator.h
 * @brief Declaration of ScaleImageGenerator, the renderer of colour legends.
 *
 * A legend (also called "scale") is the image that explains how the colours of
 * a timeline or histogram map to semantic values. This file declares:
 *
 * - The enumerations that configure a legend (type, orientation, background).
 * - ScaleImageGenerator, which turns a set of semantic values and colours into
 *   a bitmap, and saves it to disk or copies it to the clipboard.
 * - Two small image helpers shared with WindowImageGenerator.
 *
 * ScaleImageGenerator knows nothing about the Save Image dialog nor about the
 * image of the window itself. WindowImageGenerator (windowimage.h) owns the
 * legends needed by a window, composes them with the window image, and is the
 * object that SaveImageDialog (saveimagedialog.h) previews and saves.
 */

#pragma once

// paraverconfig.h provides TImageFormat and, through the kernel window API,
// Timeline, Histogram and WindowGenericItem.
#include "paraverconfig.h"
#include "paravertypes.h"

#include <map>
#include <optional>
#include <vector>
#include <wx/bitmap.h>
#include <wx/font.h>
#include <wx/gdicmn.h>
#include <wx/string.h>


/** @brief Background used to draw a legend. */
enum class TBackgroundMode
{
  BACKGROUND_SOLID_DARK,
  BACKGROUND_SOLID_LIGHT,
  BACKGROUND_TRANSPARENT,
  BACKGROUND_GRADIENT
};

/** @brief Kind of legend, derived from the colour mode of the window it describes. */
enum class TScaleImageType
{
  TScaleImageCodeColor,
  TScaleImageGradientColor,
  TScaleImageGradientColorNotNull,
  TScaleImageAlternativeGradientColor,
  TScaleImageGradientFusedLines
};

/** @brief Direction along which the legend entries are laid out. */
enum class TScaleImageOrientation
{
  TScaleImageOrientationVertical,
  TScaleImageOrientationHorizontal
};


/**
 * @brief Tells whether a legend type is drawn as a continuous gradient.
 * @param whichScaleType Legend type to check.
 * @return true for the three gradient types; false for code colour and fused lines,
 *         which are drawn as a list of coloured squares with their labels.
 */
bool isGradientScaleType( TScaleImageType whichScaleType );

/**
 * @brief Translates a Paraver image format into the equivalent wxWidgets bitmap type.
 * @param whichFilterIndex Image format selected by the user.
 * @return The matching wxBitmapType; PNG for unknown formats.
 */
wxBitmapType imageFormatTypeToBitmapType( const TImageFormat& whichFilterIndex );

/**
 * @brief Copies a bitmap to the system clipboard.
 * @param whichBitmap Bitmap to copy.
 * @return true if the bitmap was placed in the clipboard; false if the bitmap is
 *         invalid or the clipboard could not be opened.
 */
bool copyBitmapToClipboard( const wxBitmap& whichBitmap );


/**
 * @brief Renders the legend of a timeline or histogram into a bitmap.
 *
 * Depending on its type the legend is drawn either as a list of coloured squares
 * with a label each (code colour, fused lines) or as a gradient bar with outlier
 * squares at both ends (gradient types).
 *
 * The options that affect the result (orientation, long/short labels, selected
 * values) are plain state: changing them never draws anything, and
 * generateImage() always renders from the current state only. The resulting
 * image therefore does not depend on the order in which options were changed.
 *
 * @note The windows passed to the constructors are not owned and must outlive
 *       the generator.
 */
class ScaleImageGenerator
{
  public:
    /**
     * @brief Creates the legend of a histogram.
     * @param whichHistogram Histogram that provides the value range and the gradient colours.
     * @param whichDataWindow Data window of the histogram, used to build the value labels.
     * @param whichSemanticValueColors Semantic values to describe and their colours.
     *        Gradient legends only use them until the first generateImage().
     * @param whichTextFont Font of the labels.
     * @param whichScaleType Kind of legend to draw.
     * @param whichOrientation Initial layout direction.
     * @param whichBackgroundMode Background of the legend.
     */
    ScaleImageGenerator( Histogram* whichHistogram,
                         Timeline* whichDataWindow,
                         const std::map< TSemanticValue, rgb >& whichSemanticValueColors,
                         const wxFont& whichTextFont,
                         TScaleImageType whichScaleType,
                         TScaleImageOrientation whichOrientation,
                         TBackgroundMode whichBackgroundMode );

    /**
     * @brief Creates the legend of a timeline.
     * @param whichTimeline Timeline that provides the value range, the colours and the labels.
     * @param whichSemanticValueColors Semantic values to describe and their colours.
     *        For fused lines the keys are object indexes instead of semantic values.
     *        Gradient legends only use them until the first generateImage().
     * @param whichTextFont Font of the labels.
     * @param whichScaleType Kind of legend to draw.
     * @param whichOrientation Initial layout direction.
     * @param whichBackgroundMode Background of the legend.
     */
    ScaleImageGenerator( Timeline* whichTimeline,
                         const std::map< TSemanticValue, rgb >& whichSemanticValueColors,
                         const wxFont& whichTextFont,
                         TScaleImageType whichScaleType,
                         TScaleImageOrientation whichOrientation,
                         TBackgroundMode whichBackgroundMode );

    /**
     * @brief Renders the legend bitmap from the current configuration.
     * @param targetLength Length in pixels, along the layout direction, of the image
     *        the legend will be attached to. When given, gradient legends adapt
     *        their number of squares (and their width, if horizontal) to it.
     *        Legends made of labelled squares always take the size their labels need.
     * @note Replaces the previous bitmap. Pointers returned by getBitmap() stay
     *       valid, but refer to the new contents.
     */
    void generateImage( std::optional< int > targetLength = std::nullopt );

    /**
     * @brief Gives access to the rendered legend.
     * @return The legend bitmap, owned by this object; nullptr if generateImage()
     *         has not produced a valid image yet.
     */
    wxBitmap* getBitmap();

    /**
     * @brief Returns the kind of legend being drawn.
     * @return The legend type given at construction.
     */
    TScaleImageType getLegendType() const;

    /**
     * @brief Returns the file name suffix that identifies this kind of legend.
     * @return "gradient_legend", "nn_gradient_legend", "alt_gradient_legend" or "code_legend".
     */
    const wxString& getLegendSuffix() const;

    /**
     * @brief Changes the layout direction used by the next generateImage().
     * @param whichOrientation New layout direction.
     */
    void setImageOrientation( TScaleImageOrientation whichOrientation );

    /**
     * @brief Selects long or short labels for the next generateImage().
     * @param whichShortLabels true to use the short version of the labels.
     */
    void setShortLabels( bool whichShortLabels );

    /**
     * @brief Returns the labels of all the values the legend can show.
     * @return Label of every semantic value, long or short as currently configured,
     *         regardless of which ones are selected. The position of a value in
     *         this map is the index used by the selection methods.
     */
    const std::map< TSemanticValue, wxString >& getSemanticValues() const;

    /**
     * @brief Chooses which values are drawn by the next generateImage().
     * @param whichSelectedPositions Positions, within getSemanticValues(), of the values to draw.
     * @note Gradient legends always draw their whole range and reset the selection
     *       on every generateImage().
     */
    void setSelectedSemanticValues( const std::vector< int >& whichSelectedPositions );

    /**
     * @brief Returns which values are currently selected to be drawn.
     * @return Positions, within getSemanticValues(), of the selected values.
     */
    const std::vector< int >& getSelectedSemanticValues() const;

    /**
     * @brief Saves the rendered legend to a file.
     * @param whichFilePath Full path of the file to write.
     * @param whichImageFormat Format of the file.
     * @return true if the file was written; false if there is no image or the write failed.
     */
    bool saveImage( const wxString& whichFilePath, TImageFormat whichImageFormat );

    /**
     * @brief Copies the rendered legend to the system clipboard.
     * @return true if the legend was copied; false if there is no image or the
     *         clipboard is not available.
     */
    bool copyImageToClipboard();

  private:
    /** @brief Labelled positions of a gradient bar. */
    enum class TGradientLabelPosition
    {
      FIRST,
      MIDDLE,
      LAST
    };

    /** @brief Where the tick mark and the text of a gradient label are drawn. */
    struct TGradientLabelPlacement
    {
        int squarePosition = 0; // Position, along the bar, used to check overlaps between labels
        int tickOffset     = 0; // Offset of the tick mark from the origin of its square
        int textOffset     = 0; // Offset of the text from the origin of its square
    };

    using TGradientLabelPlacements = std::map< TGradientLabelPosition, TGradientLabelPlacement >;

    /** @brief Metrics and drawing options of the legend, in pixels unless stated otherwise. */
    struct TLegendLayout
    {
        int margin        = 0; // Space left around the legend contents
        int squareSpacing = 0; // Distance between consecutive squares and between squares and labels

        int squareWidth  = 0;
        int squareHeight = 0;

        int imageWidth  = 0;
        int imageHeight = 0;

        bool symbolicLabels = false; // Labels show names (true) or numeric values (false)
        bool drawOutliers   = false; // The legend includes the squares of the outlier colours

        int labelPrecision = 0; // Decimals of numeric labels

        wxFont textFont;
        TBackgroundMode backgroundMode = TBackgroundMode::BACKGROUND_SOLID_LIGHT;
    };

    /** @brief Pair of device contexts (visible legend and its mask) drawn in lockstep. */
    struct TCanvas;

    /**
     * @brief Common construction for both kinds of source window.
     * @param whichSourceWindow Timeline or histogram that provides the value range and the colours.
     * @param whichLabelWindow Timeline used to build the value labels.
     * @param whichSemanticValueColors Semantic values to describe and their colours.
     * @param whichTextFont Font of the labels.
     * @param whichScaleType Kind of legend to draw.
     * @param whichOrientation Initial layout direction.
     * @param whichBackgroundMode Background of the legend.
     */
    ScaleImageGenerator( WindowGenericItem whichSourceWindow,
                         const Timeline* whichLabelWindow,
                         const std::map< TSemanticValue, rgb >& whichSemanticValueColors,
                         const wxFont& whichTextFont,
                         TScaleImageType whichScaleType,
                         TScaleImageOrientation whichOrientation,
                         TBackgroundMode whichBackgroundMode );

    /**
     * @brief Resets the layout metrics and reads the value range from the source window.
     * @note Must run before every render: computeImageSize() modifies the metrics.
     */
    void initializeLayout();

    /**
     * @brief Rebuilds the long and short labels of every value in semanticValuesColors.
     */
    void computeSemanticLabels();

    /**
     * @brief Marks every value in semanticValuesColors as selected.
     */
    void selectAllSemanticValues();

    /**
     * @brief Replaces semanticValuesColors with gradientSquareCount samples of the value range.
     * @note The labels must be rebuilt afterwards.
     */
    void computeGradientColors();

    /**
     * @brief Collects the labels that will be drawn and measures the longest one.
     * @note Updates visibleLabels and maxLabelSize.
     */
    void selectVisibleLabels();

    /**
     * @brief Computes the size of the legend image for the current configuration.
     * @param targetLength Length of the image the legend will be attached to, if any.
     * @note Updates layout.imageWidth and layout.imageHeight (and layout.squareWidth
     *       for horizontal gradients with a target length).
     */
    void computeImageSize( std::optional< int > targetLength );

    /**
     * @brief Sets font, colours and background of both device contexts and clears them.
     * @param canvas Device contexts to prepare.
     */
    void prepareCanvas( TCanvas& canvas ) const;

    /**
     * @brief Draws the legend as a list of coloured squares followed by their labels.
     * @param canvas Device contexts to draw on.
     */
    void drawImageCodeColor( TCanvas& canvas ) const;

    /**
     * @brief Draws the legend as a gradient bar between the two outlier squares.
     * @param canvas Device contexts to draw on.
     * @note Only the first, middle and last values are labelled; the middle label
     *       is omitted in horizontal legends when it would overlap the last one.
     */
    void drawImageGradientColor( TCanvas& canvas ) const;

    /**
     * @brief Computes where the tick mark and the text of a gradient label go.
     * @param canvas Device contexts used to measure the text.
     * @param semanticValueLabel Text of the label.
     * @param labelPosition Which of the labelled positions is being placed.
     * @param squarePosition Position of the label's square along the bar.
     * @return The placement of the label.
     */
    TGradientLabelPlacement computeGradientLabelPlacement( TCanvas& canvas,
                                                           const wxString& semanticValueLabel,
                                                           TGradientLabelPosition labelPosition,
                                                           int squarePosition ) const;

    /**
     * @brief Draws the tick mark and the text of a gradient label.
     * @param canvas Device contexts to draw on.
     * @param semanticValueLabel Text of the label.
     * @param labelPlacement Placement computed by computeGradientLabelPlacement().
     * @param posX Horizontal origin of the label's square.
     * @param posY Vertical origin of the label's square.
     */
    void drawLabelGradient( TCanvas& canvas,
                            const wxString& semanticValueLabel,
                            const TGradientLabelPlacement& labelPlacement,
                            int posX,
                            int posY ) const;

    /**
     * @brief Draws one square of the legend.
     * @param canvas Device contexts to draw on.
     * @param semanticColour Fill colour of the square.
     * @param posX Horizontal origin of the square.
     * @param posY Vertical origin of the square.
     */
    void drawSquare( TCanvas& canvas, const rgb& semanticColour, int posX, int posY ) const;

    /**
     * @brief Returns the short label of a semantic value.
     * @param whichValue Semantic value to look up.
     * @return Its short label, or an empty string if the value has no label.
     */
    wxString getShortLabel( TSemanticValue whichValue ) const;

    /**
     * @brief Makes the legend background transparent when that mode is selected.
     * @note Only has effect with TBackgroundMode::BACKGROUND_TRANSPARENT, and never on macOS.
     */
    void applyTransparencyMask();

  private:
    WindowGenericItem sourceWindow;        // Provides value range and colours. Not owned.
    const Timeline* labelWindow = nullptr; // Provides the labels of the values. Not owned.

    std::map< TSemanticValue, rgb > semanticValuesColors;
    std::map< TSemanticValue, wxString > semanticValuesLabelsLong;
    std::map< TSemanticValue, wxString > semanticValuesLabelsShort;

    std::vector< int > selectedLabelPositions;          // Positions of the values chosen to be drawn
    std::map< TSemanticValue, wxString > visibleLabels; // Labels actually drawn by the last render
    wxSize maxLabelSize = wxSize( 0, 0 );               // Extent of the longest visible label

    wxBitmap legendBitmap;     // Rendered legend
    wxBitmap legendMaskBitmap; // Silhouette of the legend, used for transparent backgrounds

    TSemanticValue minimumValue = 0;
    TSemanticValue maximumValue = 0;

    TLegendLayout layout;

    TScaleImageType scaleType;
    TScaleImageOrientation orientation;

    bool shortLabels = false;
    int gradientSquareCount;

    wxString legendSuffix;
};
