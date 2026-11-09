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
 * @file windowimage.h
 * @brief Declaration of WindowImageGenerator, the image model behind the Save Image dialog.
 *
 * Saving the image of a timeline or histogram involves three collaborators:
 *
 * - WindowImageGenerator (this file) builds and owns every image that can be
 *   saved: the window with its title, the standalone legends, and the window
 *   composed with its legend. It also writes them to disk or to the clipboard.
 * - ScaleImageGenerator (scaleimagegenerator.h) renders one legend. This class
 *   creates the legends it needs and keeps them consistent with the legend
 *   options (orientation, short labels, selected values).
 * - SaveImageDialog (saveimagedialog.h) previews the bitmaps exposed here, lets
 *   the user change the legend options, and tells the caller what to save.
 *
 * gTimeline and gHistogram create a WindowImageGenerator, ask it to generate
 * the images that apply to them, show the dialog, and finally call the save
 * methods chosen by the user.
 */

#pragma once

#include "scaleimagegenerator.h"

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <wx/bitmap.h>
#include <wx/colour.h>
#include <wx/font.h>
#include <wx/string.h>

class wxDC;
class wxGrid;


/**
 * @brief Builds, keeps and saves the images offered by the Save Image dialog.
 *
 * Each image has its own member with a single meaning, whatever the kind of
 * window being saved:
 *
 * - The window image: title plus window contents.
 * - The data legend: legend of the values painted in the window. For a timeline
 *   it describes its semantic values; for a histogram, the values of its cells.
 * - The control legend: legend of the control window of a histogram. It only
 *   exists for histograms that colour their first row.
 * - The window and legend image: the window image with a copy of the data legend
 *   attached below (horizontal legend) or at its right (vertical legend).
 *
 * Any of them may be missing, either because it does not apply or because its
 * generation failed. Getters then return nullptr and save methods return false.
 *
 * @note The source bitmaps and the windows passed to the generation methods are
 *       not owned and must outlive this object.
 */
class WindowImageGenerator
{
  public:
    /**
     * @brief Prepares the generation of the images of a window.
     * @param whichBitmaps Layers with the contents of the window, bottom first.
     *        Only macOS stacks several layers; elsewhere the first one is used.
     * @param whichBackground Background colour of the title and of the composed image.
     * @param whichForeground Colour of the title text.
     * @param whichTextFont Font of the title and of the legends.
     * @param whichFilterIndex Format used when saving to disk.
     * @param whichMergedImages Unused; kept for compatibility with existing callers.
     */
    WindowImageGenerator( const std::vector< wxBitmap* >& whichBitmaps,
                          wxColour whichBackground,
                          wxColour whichForeground,
                          wxFont whichTextFont,
                          TImageFormat whichFilterIndex,
                          bool whichMergedImages = false );

    /**
     * @brief Releases the generated images and legends.
     */
    ~WindowImageGenerator();

#ifdef __WXMAC__
    /**
     * @brief Draws a stack of layers, blending every layer but the first one.
     * @param dc Device context to draw on.
     * @param whichBitmaps Layers to draw, bottom first.
     */
    static void drawStackedBitmaps( wxDC& dc, const std::vector< wxBitmap* >& whichBitmaps );
#endif

    /**
     * @brief Gives access to the window image.
     * @return Bitmap owned by this object, or nullptr if it has not been generated.
     */
    wxBitmap* getWindowsImageBitmap();

    /**
     * @brief Gives access to the window and legend image.
     * @return Bitmap owned by this object, or nullptr if it has not been generated.
     */
    wxBitmap* getTimelineScaleImage();

    /**
     * @brief Gives access to the image of the data legend.
     * @return Bitmap owned by this object, or nullptr if there is no data legend.
     */
    wxBitmap* getDataLegendBitmap();

    /**
     * @brief Gives access to the image of the control legend.
     * @return Bitmap owned by this object, or nullptr if there is no control legend.
     */
    wxBitmap* getControlLegendBitmap();

    /**
     * @brief Gives read access to the data legend.
     * @return Legend owned by this object, or nullptr if it has not been generated.
     */
    const ScaleImageGenerator* getDataLegend() const;

    /**
     * @brief Gives read access to the control legend.
     * @return Legend owned by this object, or nullptr if it has not been generated.
     */
    const ScaleImageGenerator* getControlLegend() const;

    /**
     * @brief Returns the file name suffix that identifies the kind of data legend.
     * @return The suffix, or an empty string if there is no data legend.
     */
    wxString getLegendSuffix() const;

    /**
     * @brief Returns the format used when saving to disk.
     * @return The current image format.
     */
    TImageFormat getImageFormat() const;

    /**
     * @brief Changes the format used when saving to disk.
     * @param whichFilterIndex New image format.
     */
    void setImageFormat( TImageFormat whichFilterIndex );

    /**
     * @brief Changes the layout direction of every legend.
     * @param whichOrientation New layout direction.
     * @note Regenerates the legends and the window and legend image.
     */
    void setImageOrientation( TScaleImageOrientation whichOrientation );

    /**
     * @brief Switches every legend between long and short labels.
     * @param isShort true to use short labels.
     * @note Regenerates the legends and the window and legend image.
     */
    void transformToShort( bool isShort );

    /**
     * @brief Chooses which values appear in the data legend.
     * @param whichSelectedPositions Positions, within getDataLegend()->getSemanticValues(),
     *        of the values to show.
     * @note Regenerates the data legend and the window and legend image. The
     *       control legend is not affected.
     */
    void setSelectedSemanticValues( const std::vector< int >& whichSelectedPositions );

    /**
     * @brief Generates the window image from the source bitmap.
     * @param windowsName Name of the window, shown in the title.
     * @param windowTraceName Name of the trace, shown in the title if it fits.
     * @return true if the image was generated; false if the source bitmap is
     *         missing or the image would be too big.
     */
    bool generateWindowsImage( const std::string& windowsName, const std::string& windowTraceName );

    /**
     * @brief Generates the window image by rendering a grid (histogram in table mode).
     * @param gridImage Grid to render. Not owned.
     * @param windowsName Name of the window, shown in the title.
     * @param windowTraceName Name of the trace, shown in the title if it fits.
     * @return true if the image was generated; false if the grid is missing or
     *         the image would be too big.
     */
    bool generateWindowsImageGrid( wxGrid* gridImage, const std::string& windowsName, const std::string& windowTraceName );

    /**
     * @brief Generates the data legend of a timeline.
     * @param semanticValuesToColor Values to describe and their colours. For
     *        timelines in fused lines mode, objects and their colours.
     * @param whichTimeline Timeline being saved. Not owned.
     * @note The legend type follows the colour mode of the timeline.
     */
    void generateTimelineLegend( const std::map< TSemanticValue, rgb >& semanticValuesToColor, Timeline* whichTimeline );

    /**
     * @brief Generates the data legend of a histogram.
     * @param semanticValuesToColor Cell values to describe and their colours.
     * @param whichDataWindow Data window of the histogram, used for the labels. Not owned.
     * @param whichHistogram Histogram being saved. Not owned.
     * @note The legend type follows the colour mode of the histogram.
     */
    void generateHistogramDataLegend( const std::map< TSemanticValue, rgb >& semanticValuesToColor,
                                      Timeline* whichDataWindow,
                                      Histogram* whichHistogram );

    /**
     * @brief Generates the control legend of a histogram.
     * @param semanticValuesToColor Control values to describe and their colours.
     * @param whichControlWindow Control window of the histogram. Not owned.
     * @note The legend type follows the colour mode of the control window.
     */
    void generateHistogramControlLegend( const std::map< TSemanticValue, rgb >& semanticValuesToColor, Timeline* whichControlWindow );

    /**
     * @brief Generates the window and legend image.
     * @param semanticValuesToColor Values to describe and their colours.
     * @param myWindow Timeline being saved or, for histograms, their data window. Not owned.
     * @param myHistogram Histogram being saved; nullptr for timelines. Not owned.
     * @note Requires the window image. The legend attached to the window is a
     *       separate copy fitted to the window size, so the data legend keeps its own size.
     */
    void generateScaleWindowsImage( const std::map< TSemanticValue, rgb >& semanticValuesToColor,
                                    Timeline* myWindow,
                                    Histogram* myHistogram = nullptr );

    /**
     * @brief Copies an image to the system clipboard.
     * @param whichBitmapImage Bitmap to copy; nullptr to copy the window image.
     * @return true if the image was copied.
     */
    bool copyImageToClipboard( wxBitmap* whichBitmapImage = nullptr );

    /**
     * @brief Saves the window image.
     * @param whichFileName Full path of the file to write.
     * @param whichCopyToClipboard true to copy to the clipboard instead of writing a file.
     * @return true if the image was saved or copied.
     */
    bool saveImageWindows( wxString whichFileName, bool whichCopyToClipboard = false );

    /**
     * @brief Saves the window and legend image.
     * @param whichFileName Full path of the file to write.
     * @param whichCopyToClipboard true to copy to the clipboard instead of writing a file.
     * @return true if the image was saved or copied.
     */
    bool saveImageWindowScale( wxString whichFileName, bool whichCopyToClipboard = false );

    /**
     * @brief Saves the data legend.
     * @param whichFileName Full path of the file to write.
     * @param appendLegendSuffix Has no effect: the path is used as given. Kept for
     *        compatibility with existing callers.
     * @param whichCopyToClipboard true to copy to the clipboard instead of writing a file.
     * @return true if the legend was saved or copied; false if there is no data legend.
     */
    bool saveImageScale( wxString whichFileName, bool appendLegendSuffix, bool whichCopyToClipboard = false );

    /**
     * @brief Saves the control legend.
     * @param whichFileName Full path of the file to write.
     * @param appendLegendSuffix Has no effect: the path is used as given. Kept for
     *        compatibility with existing callers.
     * @param whichCopyToClipboard true to copy to the clipboard instead of writing a file.
     * @return true if the legend was saved or copied; false if there is no control legend.
     */
    bool saveImageControlScale( wxString whichFileName, bool appendLegendSuffix, bool whichCopyToClipboard = false );

  private:
    /**
     * @brief Returns the height of the title strip drawn above the window contents.
     * @return Height in pixels.
     */
    int getTitleHeight() const;

    /**
     * @brief Draws the title strip of the window image.
     * @param windowsName Name of the window.
     * @param windowTraceName Name of the trace; omitted if the full title does not fit.
     * @param titleWidth Width of the strip in pixels.
     * @return A new bitmap with the title.
     */
    wxBitmap renderTitle( const std::string& windowsName, const std::string& windowTraceName, int titleWidth ) const;

    /**
     * @brief Creates a legend for a window, without rendering it.
     * @param semanticValuesToColor Values to describe and their colours.
     * @param whichTimeline Timeline that provides the labels and, if there is no
     *        histogram, the colours and the legend type.
     * @param whichHistogram Histogram that provides the colours and the legend
     *        type; nullptr for the legend of a timeline.
     * @param whichBackgroundMode Background of the legend.
     * @return The new legend, or nullptr if there is no timeline to take the labels from.
     */
    std::unique_ptr< ScaleImageGenerator > createLegend( const std::map< TSemanticValue, rgb >& semanticValuesToColor,
                                                         Timeline* whichTimeline,
                                                         Histogram* whichHistogram,
                                                         TBackgroundMode whichBackgroundMode ) const;

    /**
     * @brief Renders the attached legend and composes it with the window image.
     * @param centerHorizontalLegend Whether a horizontal legend is centered under
     *        the window (true) or aligned to its left side (false).
     * @note Replaces the window and legend image; leaves it empty if the window
     *       image or the attached legend are missing.
     */
    void composeWindowAndLegendImage( bool centerHorizontalLegend );

    /**
     * @brief Saves a bitmap to a file or copies it to the clipboard.
     * @param whichBitmap Bitmap to export.
     * @param whichFilePath Full path of the file to write.
     * @param whichCopyToClipboard true to copy to the clipboard instead of writing a file.
     * @return true if the bitmap was exported; false if it is not valid or the export failed.
     */
    bool exportBitmap( const wxBitmap& whichBitmap, const wxString& whichFilePath, bool whichCopyToClipboard ) const;

    /**
     * @brief Saves a legend to a file or copies it to the clipboard.
     * @param whichLegend Legend to export; may be nullptr.
     * @param whichFilePath Full path of the file to write.
     * @param whichCopyToClipboard true to copy to the clipboard instead of writing a file.
     * @return true if the legend was exported; false if it is missing or the export failed.
     */
    bool exportLegend( ScaleImageGenerator* whichLegend, const wxString& whichFilePath, bool whichCopyToClipboard ) const;

  private:
    std::vector< wxBitmap* > sourceBitmaps; // Layers with the window contents. Not owned.
    wxBitmap* sourceBitmap = nullptr;       // First layer; defines the size of the contents. Not owned.

    wxBitmap windowImage;          // Title + window contents
    wxBitmap windowAndLegendImage; // Window image + attached legend

    std::unique_ptr< ScaleImageGenerator > dataLegend;     // Legend of the values painted in the window
    std::unique_ptr< ScaleImageGenerator > controlLegend;  // Legend of the control window of a histogram
    std::unique_ptr< ScaleImageGenerator > attachedLegend; // Copy of the data legend fitted to the window image

    TScaleImageOrientation orientation = TScaleImageOrientation::TScaleImageOrientationHorizontal;

    TImageFormat imageFormat;

    wxColour imageBackground;
    wxColour imageForeground;

    wxFont imageTextFont;
};
