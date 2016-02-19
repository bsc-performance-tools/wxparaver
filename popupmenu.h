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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#ifndef _POPUPMENU_H_ 
#define _POPUPMENU_H_

#include <wx/menu.h>
#include <wx/choicdlg.h>
#include <wx/propdlg.h>
#include <wx/generic/propdlg.h>
#include "rowsselectiondialog.h"

#include <map>

using namespace std;

enum ItemType {ITEMNORMAL, ITEMRADIO, ITEMCHECK};

class gTimeline;
class gHistogram;

#define ID_MENU_COPY                              30000
#define ID_MENU_TIME                              30001
#define ID_MENU_OBJECTS                           30002
#define ID_MENU_SIZE                              30003
#define ID_MENU_DURATION                          30004
#define ID_MENU_SEMANTIC_SCALE                    30005
#define ID_MENU_FILTER_ALL                        30006
#define ID_MENU_FILTER_COMMS                      30007
#define ID_MENU_FILTER_EVENTS                     30008
#define ID_MENU_PASTE_SPECIAL                     30009
#define ID_MENU_CLONE                             30010
#define ID_MENU_UNDO_ZOOM                         30011
#define ID_MENU_REDO_ZOOM                         30012
#define ID_MENU_FIT_TIME                          30013
#define ID_MENU_FIT_SEMANTIC_MIN                  30014
#define ID_MENU_FIT_SEMANTIC_MAX                  30015
#define ID_MENU_FIT_SEMANTIC_BOTH                 30016
#define ID_MENU_FIT_OBJECTS                       30017
#define ID_MENU_VIEW_COMMUNICATION_LINES          30018
#define ID_MENU_VIEW_EVENT_FLAGS                  30019
#define ID_MENU_CODE_COLOR                        30020
#define ID_MENU_GRADIENT_COLOR                    30021
#define ID_MENU_NOT_NULL_GRADIENT_COLOR           30022
#define ID_MENU_DRAWMODE_TIME_LAST                30023
#define ID_MENU_DRAWMODE_TIME_MAXIMUM             30024
#define ID_MENU_DRAWMODE_TIME_MINIMUM_NOT_ZERO    30025
#define ID_MENU_DRAWMODE_TIME_RANDOM              30026
#define ID_MENU_DRAWMODE_TIME_RANDOM_NOT_ZERO     30027
#define ID_MENU_DRAWMODE_TIME_AVERAGE             30028
#define ID_MENU_DRAWMODE_OBJECTS_LAST             30029
#define ID_MENU_DRAWMODE_OBJECTS_MAXIMUM          30030
#define ID_MENU_DRAWMODE_OBJECTS_MINIMUM_NOT_ZERO 30031
#define ID_MENU_DRAWMODE_OBJECTS_RANDOM           30032
#define ID_MENU_DRAWMODE_OBJECTS_RANDOM_NOT_ZERO  30033
#define ID_MENU_DRAWMODE_OBJECTS_AVERAGE          30034
#define ID_MENU_DRAWMODE_BOTH_LAST                30035
#define ID_MENU_DRAWMODE_BOTH_MAXIMUM             30036
#define ID_MENU_DRAWMODE_BOTH_MINIMUM_NOT_ZERO    30037
#define ID_MENU_DRAWMODE_BOTH_RANDOM              30038
#define ID_MENU_DRAWMODE_BOTH_RANDOM_NOT_ZERO     30039
#define ID_MENU_DRAWMODE_BOTH_AVERAGE             30040
#define ID_MENU_PIXEL_SIZE_x1                     30041
#define ID_MENU_PIXEL_SIZE_x2                     30042
#define ID_MENU_PIXEL_SIZE_x4                     30043
#define ID_MENU_PIXEL_SIZE_x8                     30044
#define ID_MENU_ROW_SELECTION                     30045
#define ID_MENU_SAVE_IMAGE                        30046
#define ID_MENU_INFO_PANEL                        30047
#define ID_MENU_AUTO_CONTROL_SCALE                30048
#define ID_MENU_AUTO_3D_SCALE                     30049
#define ID_MENU_AUTO_DATA_GRADIENT                30050
#define ID_MENU_GRADIENT_FUNCTION_LINEAR          30051
#define ID_MENU_GRADIENT_FUNCTION_STEPS           30052
#define ID_MENU_GRADIENT_FUNCTION_LOGARITHMIC     30053
#define ID_MENU_GRADIENT_FUNCTION_EXPONENTIAL     30054
#define ID_MENU_PASTE_CONTROL_SCALE               30055
#define ID_MENU_PASTE_3D_SCALE                    30056
#define ID_MENU_SAVE_TIMELINE_AS_TEXT             30057
#define ID_MENU_SAVE_CURRENT_PLANE_AS_TEXT        30058
#define ID_MENU_SAVE_ALL_PLANES_AS_TEXT           30059
#define ID_MENU_SYNCHRONIZE                       30060
#define ID_MENU_REMOVE_ALL_SYNC                   30061
#define ID_MENU_VIEW_FUNCTION_LINE                30062
#define ID_MENU_PASTE_DEFAULT_SPECIAL             30063
#define ID_MENU_CODE_COLOR_2D                     30064
#define ID_MENU_GRADIENT_COLOR_2D                 30065
#define ID_MENU_LABELS_ALL                        30066
#define ID_MENU_LABELS_SPACED                     30067
#define ID_MENU_LABELS_POWER2                     30068
#define ID_MENU_DRAWMODE_TIME_AVERAGE_NOT_ZERO    30069
#define ID_MENU_DRAWMODE_OBJECTS_AVERAGE_NOT_ZERO 30070
#define ID_MENU_DRAWMODE_BOTH_AVERAGE_NOT_ZERO    30071
#define ID_MENU_CLUSTERING                        30072
#define ID_MENU_CUTTER                            30073
#define ID_MENU_DIMEMAS                           30074
#define ID_MENU_FOLDING                           30075
#define ID_MENU_OBJECT_AXIS_CURRENT               30076
#define ID_MENU_OBJECT_AXIS_ALL                   30077
#define ID_MENU_OBJECT_AXIS_ZERO                  30078
#define ID_MENU_OBJECT_AXIS_FIVE                  30079
#define ID_MENU_OBJECT_AXIS_TEN                   30080
#define ID_MENU_OBJECT_AXIS_TWENTYFIVE            30081
#define ID_MENU_SAVE_TIMELINE_AS_CFG              30082
#define ID_MENU_SAVE_HISTOGRAM_AS_CFG             30083
#define ID_MENU_SAVE_IMAGE_LEGEND                 30084
#define ID_MENU_DRAWMODE_TIME_MODE                30085
#define ID_MENU_DRAWMODE_OBJECTS_MODE             30086
#define ID_MENU_DRAWMODE_BOTH_MODE                30087
#define ID_MENU_SPECTRAL                          30088


class gPopUpMenu : public wxMenu
{
  DECLARE_EVENT_TABLE()

  public:
    gPopUpMenu() 
    {
    }
    
    gPopUpMenu( gTimeline *whichTimeline );
    gPopUpMenu( gHistogram *whichHistogram );
    virtual ~gPopUpMenu();

    void enablePaste( const string tag, bool checkPaste );
    void enable( const string tag, bool enable );
    void enable( const string tag );
    void disable( const string tag );

    void enableMenu( gTimeline *whichTimeline );
    void enableMenu( gHistogram *whichHistogram );

    static wxMultiChoiceDialog *createPasteSpecialDialog( wxArrayString& choices, gHistogram *whichHistogram );
    static wxMultiChoiceDialog *createPasteSpecialDialog( wxArrayString& choices, gTimeline * whichTimeline );
    // static wxMultiChoiceDialog *createRowSelectionDialog( wxArrayString& choices, gTimeline *whichTimeline );
    static RowsSelectionDialog *createRowSelectionDialog( gTimeline *whichTimeline );
    static string getOption( wxArrayString& choices, int position );

    void OnMenuCopy( wxCommandEvent& event);
    void OnMenuTime( wxCommandEvent& event);
    void OnMenuObjects( wxCommandEvent& event);
    void OnMenuSize( wxCommandEvent& event);
    void OnMenuDuration( wxCommandEvent& event);
    void OnMenuSemanticScale( wxCommandEvent& event);
    void OnMenuFilterAll( wxCommandEvent& event);
    void OnMenuFilterComms( wxCommandEvent& event);
    void OnMenuFilterEvents( wxCommandEvent& event);
    void OnMenuPasteControlScale( wxCommandEvent& event );
    void OnMenuPaste3DScale( wxCommandEvent& event );
    void OnMenuPasteDefaultSpecial( wxCommandEvent& event);
    void OnMenuPasteSpecial( wxCommandEvent& event);
    void OnMenuClone( wxCommandEvent& event);
    void OnMenuUndoZoom( wxCommandEvent& event);
    void OnMenuRedoZoom( wxCommandEvent& event);
    void OnMenuFitTime( wxCommandEvent& event);
    void OnMenuFitSemanticMin( wxCommandEvent& event);
    void OnMenuFitSemanticMax( wxCommandEvent& event);
    void OnMenuFitSemanticBoth( wxCommandEvent& event);
    void OnMenuFitObjects( wxCommandEvent& event);

    void OnMenuViewCommunicationLines( wxCommandEvent& event);
    void OnMenuViewEventFlags( wxCommandEvent& event);
    void OnMenuViewFunctionLine( wxCommandEvent& event );

    void OnMenuCodeColor( wxCommandEvent& event);
    void OnMenuGradientColor( wxCommandEvent& event);
    void OnMenuNotNullGradientColor( wxCommandEvent& event);

    void OnMenuDrawModeTimeLast( wxCommandEvent& event);
    void OnMenuDrawModeTimeRandom( wxCommandEvent& event);
    void OnMenuDrawModeTimeRandomNotZero( wxCommandEvent& event);
    void OnMenuDrawModeTimeMaximum( wxCommandEvent& event);
    void OnMenuDrawModeTimeMinimumNotZero( wxCommandEvent& event);
    void OnMenuDrawModeTimeAverage( wxCommandEvent& event);
    void OnMenuDrawModeTimeAverageNotZero( wxCommandEvent& event);
    void OnMenuDrawModeTimeMode( wxCommandEvent& event);

    void OnMenuDrawModeObjectsLast( wxCommandEvent& event);
    void OnMenuDrawModeObjectsRandom( wxCommandEvent& event);
    void OnMenuDrawModeObjectsRandomNotZero( wxCommandEvent& event);
    void OnMenuDrawModeObjectsMaximum( wxCommandEvent& event);
    void OnMenuDrawModeObjectsMinimumNotZero( wxCommandEvent& event);
    void OnMenuDrawModeObjectsAverage( wxCommandEvent& event);
    void OnMenuDrawModeObjectsAverageNotZero( wxCommandEvent& event);
    void OnMenuDrawModeObjectsMode( wxCommandEvent& event);

    void OnMenuDrawModeBothLast( wxCommandEvent& event);
    void OnMenuDrawModeBothRandom( wxCommandEvent& event);
    void OnMenuDrawModeBothRandomNotZero( wxCommandEvent& event);
    void OnMenuDrawModeBothMaximum( wxCommandEvent& event);
    void OnMenuDrawModeBothMinimumNotZero( wxCommandEvent& event);
    void OnMenuDrawModeBothAverage( wxCommandEvent& event);
    void OnMenuDrawModeBothAverageNotZero( wxCommandEvent& event);
    void OnMenuDrawModeBothMode( wxCommandEvent& event);

    void OnMenuPixelSize( wxCommandEvent& event );

    void OnMenuRowSelection( wxCommandEvent& event);

    // Save 
    void OnMenuSaveImage( wxCommandEvent& event ); // both
    void OnMenuSaveImageLegend( wxCommandEvent& event );

    void OnMenuSaveTimelineAsCFG( wxCommandEvent& event );
    void OnMenuSaveTimelineAsText( wxCommandEvent& event );

    void OnMenuSaveHistogramAsCFG( wxCommandEvent& event );
    void OnMenuSaveAllPlanesAsText( wxCommandEvent& event );
    void OnMenuSaveCurrentPlaneAsText( wxCommandEvent& event );

    void OnMenuInfoPanel( wxCommandEvent& event );
    
    void OnMenuAutoControlScale( wxCommandEvent& event );
    void OnMenuAuto3DScale( wxCommandEvent& event );
    void OnMenuAutoDataGradient( wxCommandEvent& event );
    
    void OnMenuGradientFunction( wxCommandEvent& event );

    void OnMenuSynchronize( wxCommandEvent& event );
    void OnMenuRemoveAllSync( wxCommandEvent& event );
    
    void OnMenuCodeColor2D( wxCommandEvent& event );
    void OnMenuGradientColor2D( wxCommandEvent& event );
    
    void OnMenuLabelsAll( wxCommandEvent& event );
    void OnMenuLabelsSpaced( wxCommandEvent& event );
    void OnMenuLabelsPower2( wxCommandEvent& event );
    
    void OnMenuObjectAxisCurrent( wxCommandEvent& event );
    void OnMenuObjectAxisAll( wxCommandEvent& event );
    void OnMenuObjectAxisZero( wxCommandEvent& event );
    void OnMenuObjectAxisFive( wxCommandEvent& event );
    void OnMenuObjectAxisTen( wxCommandEvent& event );
    void OnMenuObjectAxisTwentyFive( wxCommandEvent& event );
    
    void OnMenuClustering( wxCommandEvent& event );
    void OnMenuCutter( wxCommandEvent& event );
    void OnMenuDimemas( wxCommandEvent& event );
    void OnMenuFolding( wxCommandEvent& event );
    void OnMenuSpectral( wxCommandEvent& event );
    
  private:
    gTimeline *timeline;
    gHistogram *histogram;

    wxMenu *popUpMenuView;
    wxMenu *popUpMenuColor;
    wxMenu *popUpMenuPaste;
    wxMenu *popUpMenuPasteFilter;
    wxMenu *popUpMenuFitSemantic;
    wxMenu *popUpMenuDrawMode;
    wxMenu *popUpMenuDrawModeTime;
    wxMenu *popUpMenuDrawModeObjects;
    wxMenu *popUpMenuDrawModeBoth;
    wxMenu *popUpMenuPixelSize;
    wxMenu *popUpMenuGradientFunction;
    wxMenu *popUpMenuSaveAsText;
    wxMenu *popUpMenuColor2D;
    wxMenu *popUpMenuLabels;
    wxMenu *popUpMenuObjectAxis;
    wxMenu *popUpMenuSave;
    wxMenu *popUpMenuRun;

    void buildItem( wxMenu *popUp,
                    const wxString &title,
                    ItemType itemType,
                    wxObjectEventFunction handler,
                    wxWindowID id,
                    bool checked = false );
};

#endif // _POPUPMENU_H_
