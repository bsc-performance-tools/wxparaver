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

#include "popupmenu.h"

#include "externalapps.h"
#include "labelconstructor.h"
#include "loadedwindows.h"
#include "paraverlabels.h"
#include "paravermain.h"
#include "sequencedriver.h"
#include "syncwindows.h"
#include "window.h"

#include <cmath>
#include <iostream>
#include <optional>
#include <wx/event.h>
#include <wx/msgdlg.h>

using namespace std;

#define INIT_WXMENU( name ) name = new wxMenu

/* Constructor of gPopUpMenu initialize menu variables and build them. There are three different
types of menus that will change the available options */

gPopUpMenu::gPopUpMenu( std::vector< gWindowGenericItem > argWindowGenericList )
{
  windowGenericList = argWindowGenericList;


  for( auto &elementItemList : argWindowGenericList )
  {
    if( std::holds_alternative< gHistogram * >( elementItemList ) )
    {
      if( typeDataPopup == PopUpMenuType::POPUP_MENU_INI )
        typeDataPopup = PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE;
      else if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE )
        typeDataPopup = PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_MULTIPLE;
      else
        typeDataPopup = PopUpMenuType::POPUP_MENU_TYPE_MIXED;
    }
    if( std::holds_alternative< gTimeline * >( elementItemList ) )
    {
      if( typeDataPopup == PopUpMenuType::POPUP_MENU_INI )
        typeDataPopup = PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE;
      else if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE )
        typeDataPopup = PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE;
      else
        typeDataPopup = PopUpMenuType::POPUP_MENU_TYPE_MIXED;
    }
  }
}

gPopUpMenu::gPopUpMenu( gHistogram *wichHistogramDerivedList )
{
  typeDataPopup = PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE;
  windowGenericList.emplace_back( wichHistogramDerivedList ); // Implicitly creates a variant with gTimeline*
}

gPopUpMenu::gPopUpMenu( gTimeline *wichTimelineDerivedList )
{
  typeDataPopup = PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE;
  windowGenericList.emplace_back( wichTimelineDerivedList ); // Implicitly creates a variant with gTimeline*
}

void gPopUpMenu::initializePopUpMenu()
{
  INIT_WXMENU( popUpMenuView );
  INIT_WXMENU( popUpMenuColor );
  INIT_WXMENU( popUpMenuPaste );
  INIT_WXMENU( popUpMenuPasteFilter );
  INIT_WXMENU( popUpMenuFitSemantic );
  INIT_WXMENU( popUpMenuShowWindows );
  INIT_WXMENU( popUpMenuDrawMode );
  INIT_WXMENU( popUpMenuDrawModeTime );
  INIT_WXMENU( popUpMenuDrawModeObjects );
  INIT_WXMENU( popUpMenuDrawModeBoth );
  INIT_WXMENU( popUpMenuPixelSize );
  INIT_WXMENU( popUpMenuGradientFunction );
  INIT_WXMENU( popUpMenuLabels );
  INIT_WXMENU( popUpMenuObjectAxis );
  INIT_WXMENU( popUpMenuSave );
  INIT_WXMENU( popUpMenuRun );
  INIT_WXMENU( popUpMenuSync );
  INIT_WXMENU( popUpMenuColor2D );
  INIT_WXMENU( popUpMenuSyncRemove );
  INIT_WXMENU( popUpMenuSyncPropertiesType );
  INIT_WXMENU( popUpMenuSaveAsText );

  if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE )
  {
#ifdef __WXMAC__
    this->buildItem( this, _( STR_COPY ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpCopy, ID_MENU_COPY );
#else
    this->buildItem( this, _( STR_COPY ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpCopy, wxID_COPY );
#endif
    this->buildItem( this, _( STR_RENAME ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpRename, ID_MENU_RENAME );
  }

  // Build all the components of the windows. Each function will check if is available in the selected windows

  this->buildPopUpMenuPaste();

  this->buildItem( this, _( STR_CLONE ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpClone, ID_MENU_CLONE );

  this->AppendSeparator();

  this->buildPopUpMenuZoom();

  this->buildPopUpMenuFitSemantic();

  this->buildPopUpMenuFitObjects();

  this->buildPopUpMenuDimensionsConfiguration();

  this->buildPopUpMenuColor();

  this->buildPopUpMenuGradientFunction();

  this->buildPopUpMenuView();

  this->buildPopUpMenuDrawMode();

  this->buildPopUpMenuPixelSize();

  this->buildPopUpMenuLabels();

  this->buildPopUpMenuObjectAxis();

  this->AppendSeparator();

  this->buildPopUpMenuRun();

  this->AppendSeparator();

  this->buildPopUpMenuSync();

  this->buildPopUpMenuShowWindows();

  this->AppendSeparator();

  this->buildPopUpMenuSave();

  this->AppendSeparator();

  this->buildPopUpExtraPanel();

  this->buildPopUpRunApp();
}

/*************************************************************/
/****BUILD FUNCTIONS FOR DIFFERENT ITEMS OF THE MAIN MENU*****/
/*************************************************************/

void gPopUpMenu ::buildPopUpMenuZoom()
{
  if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE )
  {
    std::vector< BuildMenuItem > zoomItems = { { this, _( STR_UNDO_ZOOM ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpUndoZoom, wxID_UNDO },
                                               { this, _( STR_REDO_ZOOM ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpRedoZoom, wxID_REDO } };

    buildListOfItems( zoomItems );
  }
}
void gPopUpMenu ::buildListOfItems( std::vector< BuildMenuItem > &itemList )
{
  for( const auto &item : itemList )
  {
    this->buildItem( item.menuBaseItem, item.labelStringItem, item.kindItem, item.functionHandler, item.idItem, item.customConditionItem );

    if( item.itemHasSeparator )
    {
      item.menuBaseItem->AppendSeparator();
      continue;
    }
  }
}

void gPopUpMenu ::buildPopUpMenuView()
{
  if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE )
  {
    std::vector< BuildMenuItem > viewItems = { { popUpMenuView,
                                                 _( "Communication Lines" ),
                                                 wxITEM_CHECK,
                                                 &gPopUpMenu::OnPopUpViewCommunicationLines,
                                                 ID_MENU_VIEW_COMMUNICATION_LINES,
                                                 allWindowHas(
                                                   []( gTimeline *win )
                                                   {
                                                     return win->GetMyWindow()->getDrawCommLines();
                                                   },
                                                   []( gHistogram *win )
                                                   {
                                                     return true;
                                                   } ) },

                                               { popUpMenuView,
                                                 _( "Event Flags" ),
                                                 wxITEM_CHECK,
                                                 &gPopUpMenu::OnPopUpViewEventFlags,
                                                 ID_MENU_VIEW_EVENT_FLAGS,
                                                 allWindowHas(
                                                   []( gTimeline *win )
                                                   {
                                                     return win->GetMyWindow()->getDrawFlags();
                                                   },
                                                   []( gHistogram *win )
                                                   {
                                                     return true;
                                                   } ) } };

    buildListOfItems( viewItems );
    AppendSubMenu( popUpMenuView, _( "View" ) );
  }
}

void gPopUpMenu ::buildPopUpMenuPaste()
{
  std::vector< BuildMenuItem > pasteItemsBasicCommon = {
    { popUpMenuPaste,
      _( STR_PASTE_DEFAULT_SPECIAL ),
      wxITEM_NORMAL,
      &gPopUpMenu::OnPopUpPasteDefaultSpecial,
      ID_MENU_PASTE_DEFAULT_SPECIAL,
      true,
      true },

    { popUpMenuPaste, _( STR_TIME ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteTime, ID_MENU_PASTE_TIME },

    { popUpMenuPaste, _( STR_OBJECTS ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteObjects, ID_MENU_PASTE_OBJECTS },

    { popUpMenuPaste, _( STR_SIZE ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteSize, ID_MENU_PASTE_SIZE },

    { popUpMenuPaste, _( STR_DURATION ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteDuration, ID_MENU_PASTE_DURATION },

    { popUpMenuPaste, _( STR_SEMANTIC_SCALE ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteSemanticScale, ID_MENU_PASTE_SEMANTIC_SCALE },
  };

  std::vector< BuildMenuItem > pasteItemsBasicHistogram = {
    { popUpMenuPaste, _( STR_CONTROL_SCALE ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteControlScale, ID_MENU_PASTE_CONTROL_SCALE },

    { popUpMenuPaste, _( STR_PASTE_SEMANTIC_SORT ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteSemanticSort, ID_MENU_PASTE_SEMANTIC_SORT },

    { popUpMenuPaste, _( STR_CONTROL_DIMENSIONS ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteControlDimensions, ID_MENU_PASTE_CONTROL_DIMENSIONS },

    { popUpMenuPaste, _( STR_3D_SCALE ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPaste3DScale, ID_MENU_PASTE_3D_SCALE },
  };

  std::vector< BuildMenuItem > pasteItemsExtraCommon = {
    { popUpMenuPaste, _( STR_PASTE_SPECIAL ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteSpecial, ID_MENU_PASTE_SPECIAL },

    { popUpMenuPaste, _( STR_CUSTOM_PALETTE ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteCustomPalette, ID_MENU_PASTE_CUSTOM_PALETTE },
  };

  std::vector< BuildMenuItem > pasteItemsFilterSubmenu = {
    { popUpMenuPasteFilter, _( STR_FILTER_ALL ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteFilterAll, ID_MENU_PASTE_FILTER_ALL },

    { popUpMenuPasteFilter, _( STR_FILTER_COMMS ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteFilterCommunications, ID_MENU_PASTE_FILTER_COMMS },

    { popUpMenuPasteFilter, _( STR_FILTER_EVENTS ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteFilterEvents, ID_MENU_PASTE_FILTER_EVENTS },
  };

  buildListOfItems( pasteItemsBasicCommon );

  if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_MULTIPLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE )
  {
    buildListOfItems( pasteItemsBasicHistogram );
  }

  buildListOfItems( pasteItemsExtraCommon );

  if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE )
  {
    buildListOfItems( pasteItemsFilterSubmenu );
    popUpMenuPaste->AppendSubMenu( popUpMenuPasteFilter, _( STR_FILTER ) );
  }

  this->AppendSubMenu( popUpMenuPaste, _( STR_PASTE ) );
}

void gPopUpMenu ::buildPopUpMenuColor()
{
  std::vector< BuildMenuItem > popUpColorTimelineItems = {
    { popUpMenuColor,
      _( "Function Line" ),
      wxITEM_CHECK,
      &gPopUpMenu::OnPopUpFunctionLineColor,
      ID_MENU_VIEW_FUNCTION_LINE,
      allWindowHas(
        []( gTimeline *win )
        {
          return win->GetMyWindow()->isFunctionLineColorSet();
        },
        []( gHistogram *win )
        {
          return true;
        } ) },

    { popUpMenuColor,
      _( "Fused Lines" ),
      wxITEM_CHECK,
      &gPopUpMenu::OnPopUpFusedLinesColor,
      ID_MENU_VIEW_FUSED_LINES,
      allWindowHas(
        []( gTimeline *win )
        {
          return win->GetMyWindow()->isFusedLinesColorSet();
        },
        []( gHistogram *win )
        {
          return true;
        } ) },

    { popUpMenuColor,
      _( "Punctual" ),
      wxITEM_CHECK,
      &gPopUpMenu::OnPopUpPunctualColor,
      ID_MENU_PUNCTUAL,
      allWindowHas(
        []( gTimeline *win )
        {
          return win->GetMyWindow()->isPunctualColorSet();
        },
        []( gHistogram *win )
        {
          return true;
        } ) },
  };

  std::vector< BuildMenuItem > popUpColorCommonItems = { { popUpMenuColor,
                                                           _( "Code Color" ),
                                                           wxITEM_CHECK,
                                                           &gPopUpMenu::OnPopUpCodeColor,
                                                           ID_MENU_CODE_COLOR,
                                                           allWindowHas(
                                                             []( gTimeline *win )
                                                             {
                                                               return win->GetMyWindow()->isCodeColorSet();
                                                             },
                                                             []( gHistogram *win )
                                                             {
                                                               return true;
                                                             } ) },

                                                         { popUpMenuColor,
                                                           _( "Gradient Color" ),
                                                           wxITEM_CHECK,
                                                           &gPopUpMenu::OnPopUpGradientColor,
                                                           ID_MENU_GRADIENT_COLOR,
                                                           allWindowHas(
                                                             []( gTimeline *win )
                                                             {
                                                               return win->GetMyWindow()->isGradientColorSet();
                                                             },
                                                             []( gHistogram *win )
                                                             {
                                                               return true;
                                                             } ) },

                                                         { popUpMenuColor,
                                                           _( "Not Null Gradient Color" ),
                                                           wxITEM_CHECK,
                                                           &gPopUpMenu::OnPopUpNotNullGradientColor,
                                                           ID_MENU_NOT_NULL_GRADIENT_COLOR,
                                                           allWindowHas(
                                                             []( gTimeline *win )
                                                             {
                                                               return win->GetMyWindow()->isNotNullGradientColorSet();
                                                             },
                                                             []( gHistogram *win )
                                                             {
                                                               return true;
                                                             } ) },

                                                         { popUpMenuColor,
                                                           _( "Alternative Gradient Color" ),
                                                           wxITEM_CHECK,
                                                           &gPopUpMenu::OnPopUpAlternativeGradientColor,
                                                           ID_MENU_ALTERNATIVE_GRADIENT_COLOR,
                                                           allWindowHas(
                                                             []( gTimeline *win )
                                                             {
                                                               return win->GetMyWindow()->isAlternativeGradientColorSet();
                                                             },
                                                             []( gHistogram *win )
                                                             {
                                                               return true;
                                                             } ),
                                                           true } };

  if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE )
  {
    buildListOfItems( popUpColorTimelineItems );
  }

  buildListOfItems( popUpColorCommonItems );
}

void gPopUpMenu ::buildPopUpMenuDimensionsConfiguration()
{
  if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_MULTIPLE )
  {
    auto generalCodeColor = std::get< gHistogram * >( *windowGenericList.begin() )->GetHistogram()->getColorMode();
    auto colorSyncronized = true;

    for( auto &histogram : windowGenericList )
    {
      if( !( generalCodeColor == std::get< gHistogram * >( histogram )->GetHistogram()->getColorMode() ) )
      {
        colorSyncronized = false;
        break;
      }
    }
    std::vector< BuildMenuItem > color2DItems = { { popUpMenuColor2D,
                                                    _( "Code Color" ),
                                                    wxITEM_CHECK,
                                                    &gPopUpMenu::OnPopUpColor2D,
                                                    ID_MENU_CODE_COLOR_2D,
                                                    ( generalCodeColor == TColorFunction::CODE_COLOR ) && colorSyncronized },

                                                  { popUpMenuColor2D,
                                                    _( "Gradient Color" ),
                                                    wxITEM_CHECK,
                                                    &gPopUpMenu::OnPopUpColor2D,
                                                    ID_MENU_GRADIENT_COLOR_2D,
                                                    ( generalCodeColor == TColorFunction::GRADIENT ) && colorSyncronized },

                                                  { popUpMenuColor2D,
                                                    _( "Not Null Gradient Color" ),
                                                    wxITEM_CHECK,
                                                    &gPopUpMenu::OnPopUpColor2D,
                                                    ID_MENU_NOT_NULL_GRADIENT_COLOR_2D,
                                                    ( generalCodeColor == TColorFunction::NOT_NULL_GRADIENT ) && colorSyncronized },

                                                  { popUpMenuColor2D,
                                                    _( "Alternative Gradient Color" ),
                                                    wxITEM_CHECK,
                                                    &gPopUpMenu::OnPopUpColor2D,
                                                    ID_MENU_ALTERNATIVE_GRADIENT_COLOR_2D,
                                                    ( generalCodeColor == TColorFunction::ALTERNATIVE_GRADIENT ) && colorSyncronized } };

    buildListOfItems( color2DItems );

    AppendSubMenu( popUpMenuColor2D, _( "Color Mode" ) );
  }
}

void gPopUpMenu ::buildPopUpMenuShowWindows()
{
  std::vector< BuildMenuItem > showWindowsOptions = {
    { popUpMenuShowWindows, _( STR_SHOW_SELECTED_WINDOWS ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpShowSelectedWindows, ID_SHOW_SELECTED_WINDOWS },
    { popUpMenuShowWindows,
      _( STR_SHOW_ONLY_SELECTED_WINDOWS ),
      wxITEM_NORMAL,
      &gPopUpMenu::OnPopUpShowOnlySelectedWindows,
      ID_SHOW_ONLY_SELECTED_WINDOWS },
    { popUpMenuShowWindows, _( STR_HIDE_SELECTED_WINDOWS ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpHideSelectedWindows, ID_HIDE_SELECTED_WINDOWS },
    { popUpMenuShowWindows,
      _( STR_SHOW_SELECTED_WINDOWS_TRACE ),
      wxITEM_NORMAL,
      &gPopUpMenu::OnPopUpShowSelectedWindowsTrace,
      ID_SHOW_SELECTED_WINDOWS_TRACE },
    { popUpMenuShowWindows,
      _( STR_HIDE_SELECTED_WINDOWS_TRACE ),
      wxITEM_NORMAL,
      &gPopUpMenu::OnPopUpHideSelectedWindowsTrace,
      ID_HIDE_SELECTED_WINDOWS_TRACE }
  };

  buildListOfItems( showWindowsOptions );

  AppendSubMenu( popUpMenuShowWindows, _( STR_SHOW_WINDOWS_STATE ) );
}

void gPopUpMenu ::buildPopUpMenuFitSemantic()
{
  if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE )
  {
    std::vector< BuildMenuItem > fitSemanticItems = {
      { popUpMenuFitSemantic, _( "Fit Minimum" ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpFitSemanticScaleMin, ID_MENU_FIT_SEMANTIC_MIN },

      { popUpMenuFitSemantic, _( "Fit Maximum" ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpFitSemanticScaleMax, ID_MENU_FIT_SEMANTIC_MAX },

      { popUpMenuFitSemantic, _( "Fit Both" ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpFitSemanticScale, ID_MENU_FIT_SEMANTIC_BOTH }
    };

    buildListOfItems( fitSemanticItems );

    AppendSubMenu( popUpMenuFitSemantic, _( STR_FIT_SEMANTIC ) );
  }
}

void gPopUpMenu ::buildPopUpMenuFitObjects()
{
  std::vector< BuildMenuItem > fitItemsCommon = {
    { this, _( STR_FIT_TIME ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpFitTimeScale, wxID_ZOOM_100 },

    { this, _( STR_FIT_OBJECTS ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpFitObjects, ID_MENU_FIT_OBJECTS }
  };

  buildListOfItems( fitItemsCommon );

  this->buildItem( this, _( "Select Objects..." ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpRowSelection, ID_MENU_ROW_SELECTION );

  if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_MULTIPLE )
  {
    std::vector< BuildMenuItem > fitItemsHistogram = { { this,
                                                         _( "Auto Fit Control Scale" ),
                                                         wxITEM_CHECK,
                                                         &gPopUpMenu::OnPopUpAutoControlScale,
                                                         ID_MENU_AUTO_CONTROL_SCALE,
                                                         allWindowHas(
                                                           []( gHistogram *win )
                                                           {
                                                             return win->GetHistogram()->getCompute2DScale();
                                                           },
                                                           []( gTimeline *win )
                                                           {
                                                             return true;
                                                           } ) },

                                                       { this,
                                                         _( STR_AUTOFIT_CONTROL_ZERO ),
                                                         wxITEM_CHECK,
                                                         &gPopUpMenu::OnPopUpAutoControlScaleZero,
                                                         ID_MENU_AUTO_CONTROL_SCALE_ZERO,
                                                         allWindowHas(
                                                           []( gHistogram *win )
                                                           {
                                                             return win->GetHistogram()->getCompute2DScaleZero();
                                                           },
                                                           []( gTimeline *win )
                                                           {
                                                             return true;
                                                           } ) },

                                                       { this,
                                                         _( "Auto Fit Data Gradient" ),
                                                         wxITEM_CHECK,
                                                         &gPopUpMenu::OnPopUpAutoDataGradient,
                                                         ID_MENU_AUTO_DATA_GRADIENT,
                                                         allWindowHas(
                                                           []( gHistogram *win )
                                                           {
                                                             return win->GetHistogram()->getComputeGradient();
                                                           },
                                                           []( gTimeline *win )
                                                           {
                                                             return true;
                                                           } ) } };

    buildListOfItems( fitItemsHistogram );

    if( allWindowHas(
          []( gHistogram *win )
          {
            return win->GetHistogram()->getThreeDimensions();
          },
          []( gTimeline *win )
          {
            return true;
          } ) )
    {
      this->buildItem( this,
                       _( "Auto Fit 3D Scale" ),
                       wxITEM_CHECK,
                       &gPopUpMenu::OnPopUpAuto3DScale,
                       ID_MENU_AUTO_3D_SCALE,
                       allWindowHas(
                         []( gHistogram *win )
                         {
                           return win->GetHistogram()->getCompute3DScale();
                         },
                         []( gTimeline *win )
                         {
                           return true;
                         } ) );
    }
  }

  this->AppendSeparator();
}

void gPopUpMenu ::buildPopUpMenuDrawMode()
{
  if( typeDataPopup != PopUpMenuType::POPUP_MENU_TYPE_MIXED )
  {
    DrawModeMethod drawModeTime;
    DrawModeMethod drawModeObject;
    auto drawModeTimeSync   = true;
    auto drawModeObjectSync = true;

    auto visitorDrawModeTime = overloads{ []( gHistogram *histogram )
                                          {
                                            return histogram->GetHistogram()->getDrawModeColumns();
                                          },
                                          []( gTimeline *timeline )
                                          {
                                            return timeline->GetMyWindow()->getDrawModeTime();
                                          }

    };
    auto visitorDrawModeObjects = overloads{ []( gHistogram *histogram )
                                             {
                                               return histogram->GetHistogram()->getDrawModeObjects();
                                             },
                                             []( gTimeline *timeline )
                                             {
                                               return timeline->GetMyWindow()->getDrawModeObject();
                                             } };

    drawModeTime   = std::visit( visitorDrawModeTime, *windowGenericList.begin() );
    drawModeObject = std::visit( visitorDrawModeObjects, *windowGenericList.begin() );

    for( auto &window : windowGenericList )
    {
      if( drawModeTime != std::visit( visitorDrawModeTime, window ) )
      {
        drawModeTimeSync = false;
      }
      if( drawModeObject != std::visit( visitorDrawModeObjects, window ) )
      {
        drawModeObjectSync = false;
      }
    }
    std::vector< BuildMenuItem > drawModeTimeItems = { { popUpMenuDrawModeTime,
                                                         _( GUI_DRAWMODE_LAST ),
                                                         wxITEM_CHECK,
                                                         &gPopUpMenu::OnPopUpDrawModeTimeLast,
                                                         ID_MENU_DRAWMODE_TIME_LAST,
                                                         ( drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_LAST ) },

                                                       { popUpMenuDrawModeTime,
                                                         _( GUI_DRAWMODE_MAXIMUM ),
                                                         wxITEM_CHECK,
                                                         &gPopUpMenu::OnPopUpDrawModeTimeMaximum,
                                                         ID_MENU_DRAWMODE_TIME_MAXIMUM,
                                                         ( drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_MAXIMUM ) },

                                                       { popUpMenuDrawModeTime,
                                                         _( GUI_DRAWMODE_ABSOLUTE_MAXIMUM ),
                                                         wxITEM_CHECK,
                                                         &gPopUpMenu::OnPopUpDrawModeTimeAbsoluteMaximum,
                                                         ID_MENU_DRAWMODE_TIME_ABSOLUTE_MAXIMUM,
                                                         ( drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM ) },

                                                       { popUpMenuDrawModeTime,
                                                         _( GUI_DRAWMODE_MINIMUM_NOT_ZERO ),
                                                         wxITEM_CHECK,
                                                         &gPopUpMenu::OnPopUpDrawModeTimeMinimumNotZero,
                                                         ID_MENU_DRAWMODE_TIME_MINIMUM_NOT_ZERO,
                                                         ( drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_MINNOTZERO ) },

                                                       { popUpMenuDrawModeTime,
                                                         _( GUI_DRAWMODE_ABSOLUTE_MINIMUM_NOT_ZERO ),
                                                         wxITEM_CHECK,
                                                         &gPopUpMenu::OnPopUpDrawModeTimeAbsoluteMinimumNotZero,
                                                         ID_MENU_DRAWMODE_TIME_ABSOLUTE_MINIMUM_NOT_ZERO,
                                                         ( drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO ) },

                                                       { popUpMenuDrawModeTime,
                                                         _( GUI_DRAWMODE_RANDOM ),
                                                         wxITEM_CHECK,
                                                         &gPopUpMenu::OnPopUpDrawModeTimeRandom,
                                                         ID_MENU_DRAWMODE_TIME_RANDOM,
                                                         drawModeTime == DrawModeMethod::DRAW_RANDOM },

                                                       { popUpMenuDrawModeTime,
                                                         _( GUI_DRAWMODE_RANDOM_NOT_ZERO ),
                                                         wxITEM_CHECK,
                                                         &gPopUpMenu::OnPopUpDrawModeTimeRandomNotZero,
                                                         ID_MENU_DRAWMODE_TIME_RANDOM_NOT_ZERO,
                                                         ( drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_RANDNOTZERO ) },

                                                       { popUpMenuDrawModeTime,
                                                         _( GUI_DRAWMODE_AVERAGE ),
                                                         wxITEM_CHECK,
                                                         &gPopUpMenu::OnPopUpDrawModeTimeAverage,
                                                         ID_MENU_DRAWMODE_TIME_AVERAGE,
                                                         ( drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_AVERAGE ) },

                                                       { popUpMenuDrawModeTime,
                                                         _( GUI_DRAWMODE_AVERAGE_NOT_ZERO ),
                                                         wxITEM_CHECK,
                                                         &gPopUpMenu::OnPopUpDrawModeTimeAverageNotZero,
                                                         ID_MENU_DRAWMODE_TIME_AVERAGE_NOT_ZERO,
                                                         ( drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_AVERAGENOTZERO ) },

                                                       { popUpMenuDrawModeTime,
                                                         _( GUI_DRAWMODE_MODE ),
                                                         wxITEM_CHECK,
                                                         &gPopUpMenu::OnPopUpDrawModeTimeMode,
                                                         ID_MENU_DRAWMODE_TIME_MODE,
                                                         ( drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_MODE ) } };

    std::vector< BuildMenuItem > drawModeObjectsItems = { { popUpMenuDrawModeObjects,
                                                            _( GUI_DRAWMODE_LAST ),
                                                            wxITEM_CHECK,
                                                            &gPopUpMenu::OnPopUpDrawModeObjectsLast,
                                                            ID_MENU_DRAWMODE_OBJECTS_LAST,
                                                            ( drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_LAST ) },

                                                          { popUpMenuDrawModeObjects,
                                                            _( GUI_DRAWMODE_MAXIMUM ),
                                                            wxITEM_CHECK,
                                                            &gPopUpMenu::OnPopUpDrawModeObjectsMaximum,
                                                            ID_MENU_DRAWMODE_OBJECTS_MAXIMUM,
                                                            ( drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_MAXIMUM ) },

                                                          { popUpMenuDrawModeObjects,
                                                            _( GUI_DRAWMODE_ABSOLUTE_MAXIMUM ),
                                                            wxITEM_CHECK,
                                                            &gPopUpMenu::OnPopUpDrawModeObjectsAbsoluteMaximum,
                                                            ID_MENU_DRAWMODE_OBJECTS_ABSOLUTE_MAXIMUM,
                                                            ( drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM ) },

                                                          { popUpMenuDrawModeObjects,
                                                            _( GUI_DRAWMODE_MINIMUM_NOT_ZERO ),
                                                            wxITEM_CHECK,
                                                            &gPopUpMenu::OnPopUpDrawModeObjectsMinimumNotZero,
                                                            ID_MENU_DRAWMODE_OBJECTS_MINIMUM_NOT_ZERO,
                                                            ( drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_MINNOTZERO ) },

                                                          { popUpMenuDrawModeObjects,
                                                            _( GUI_DRAWMODE_ABSOLUTE_MINIMUM_NOT_ZERO ),
                                                            wxITEM_CHECK,
                                                            &gPopUpMenu::OnPopUpDrawModeObjectsAbsoluteMinimumNotZero,
                                                            ID_MENU_DRAWMODE_OBJECTS_ABSOLUTE_MINIMUM_NOT_ZERO,
                                                            ( drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO ) },

                                                          { popUpMenuDrawModeObjects,
                                                            _( GUI_DRAWMODE_RANDOM ),
                                                            wxITEM_CHECK,
                                                            &gPopUpMenu::OnPopUpDrawModeObjectsRandom,
                                                            ID_MENU_DRAWMODE_OBJECTS_RANDOM,
                                                            ( drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_RANDOM ) },

                                                          { popUpMenuDrawModeObjects,
                                                            _( GUI_DRAWMODE_RANDOM_NOT_ZERO ),
                                                            wxITEM_CHECK,
                                                            &gPopUpMenu::OnPopUpDrawModeObjectsRandomNotZero,
                                                            ID_MENU_DRAWMODE_OBJECTS_RANDOM_NOT_ZERO,
                                                            ( drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_RANDNOTZERO ) },

                                                          { popUpMenuDrawModeObjects,
                                                            _( GUI_DRAWMODE_AVERAGE ),
                                                            wxITEM_CHECK,
                                                            &gPopUpMenu::OnPopUpDrawModeObjectsAverage,
                                                            ID_MENU_DRAWMODE_OBJECTS_AVERAGE,
                                                            ( drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_AVERAGE ) },

                                                          { popUpMenuDrawModeObjects,
                                                            _( GUI_DRAWMODE_AVERAGE_NOT_ZERO ),
                                                            wxITEM_CHECK,
                                                            &gPopUpMenu::OnPopUpDrawModeObjectsAverageNotZero,
                                                            ID_MENU_DRAWMODE_OBJECTS_AVERAGE_NOT_ZERO,
                                                            ( drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_AVERAGENOTZERO ) },

                                                          { popUpMenuDrawModeObjects,
                                                            _( GUI_DRAWMODE_MODE ),
                                                            wxITEM_CHECK,
                                                            &gPopUpMenu::OnPopUpDrawModeObjectsMode,
                                                            ID_MENU_DRAWMODE_OBJECTS_MODE,
                                                            ( drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_MODE ) } };

    std::vector< BuildMenuItem > drawModeBothItems = {
      { popUpMenuDrawModeBoth,
        _( GUI_DRAWMODE_LAST ),
        wxITEM_CHECK,
        &gPopUpMenu::OnPopUpDrawModeBothLast,
        ID_MENU_DRAWMODE_BOTH_LAST,
        ( drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_LAST ) && ( drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_LAST ) },

      { popUpMenuDrawModeBoth,
        _( GUI_DRAWMODE_MAXIMUM ),
        wxITEM_CHECK,
        &gPopUpMenu::OnPopUpDrawModeBothMaximum,
        ID_MENU_DRAWMODE_BOTH_MAXIMUM,
        ( drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_MAXIMUM ) &&
          ( drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_MAXIMUM ) },

      { popUpMenuDrawModeBoth,
        _( GUI_DRAWMODE_ABSOLUTE_MAXIMUM ),
        wxITEM_CHECK,
        &gPopUpMenu::OnPopUpDrawModeBothAbsoluteMaximum,
        ID_MENU_DRAWMODE_BOTH_ABSOLUTE_MAXIMUM,
        ( drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM ) &&
          ( drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM ) },

      { popUpMenuDrawModeBoth,
        _( GUI_DRAWMODE_MINIMUM_NOT_ZERO ),
        wxITEM_CHECK,
        &gPopUpMenu::OnPopUpDrawModeBothMinimumNotZero,
        ID_MENU_DRAWMODE_BOTH_MINIMUM_NOT_ZERO,
        ( drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_MINNOTZERO ) &&
          ( drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_MINNOTZERO ) },

      { popUpMenuDrawModeBoth,
        _( GUI_DRAWMODE_ABSOLUTE_MINIMUM_NOT_ZERO ),
        wxITEM_CHECK,
        &gPopUpMenu::OnPopUpDrawModeBothAbsoluteMinimumNotZero,
        ID_MENU_DRAWMODE_BOTH_ABSOLUTE_MINIMUM_NOT_ZERO,
        ( drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO ) &&
          ( drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO ) },

      { popUpMenuDrawModeBoth,
        _( GUI_DRAWMODE_RANDOM ),
        wxITEM_CHECK,
        &gPopUpMenu::OnPopUpDrawModeBothRandom,
        ID_MENU_DRAWMODE_BOTH_RANDOM,
        ( drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_RANDOM ) &&
          ( drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_RANDOM ) },

      { popUpMenuDrawModeBoth,
        _( GUI_DRAWMODE_RANDOM_NOT_ZERO ),
        wxITEM_CHECK,
        &gPopUpMenu::OnPopUpDrawModeBothRandomNotZero,
        ID_MENU_DRAWMODE_BOTH_RANDOM_NOT_ZERO,
        ( drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_RANDNOTZERO ) &&
          ( drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_RANDNOTZERO ) },

      { popUpMenuDrawModeBoth,
        _( GUI_DRAWMODE_AVERAGE ),
        wxITEM_CHECK,
        &gPopUpMenu::OnPopUpDrawModeBothAverage,
        ID_MENU_DRAWMODE_BOTH_AVERAGE,
        ( drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_AVERAGE ) &&
          ( drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_AVERAGE ) },

      { popUpMenuDrawModeBoth,
        _( GUI_DRAWMODE_AVERAGE_NOT_ZERO ),
        wxITEM_CHECK,
        &gPopUpMenu::OnPopUpDrawModeBothAverageNotZero,
        ID_MENU_DRAWMODE_BOTH_AVERAGE_NOT_ZERO,
        ( drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_AVERAGENOTZERO ) &&
          ( drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_AVERAGENOTZERO ) },

      { popUpMenuDrawModeBoth,
        _( GUI_DRAWMODE_MODE ),
        wxITEM_CHECK,
        &gPopUpMenu::OnPopUpDrawModeBothMode,
        ID_MENU_DRAWMODE_BOTH_MODE,
        ( drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_MODE ) && ( drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_MODE ) }
    };

    buildListOfItems( drawModeTimeItems );

    buildListOfItems( drawModeObjectsItems );

    buildListOfItems( drawModeBothItems );

    if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_MULTIPLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE )
    {
      popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeTime, _( "Semantic" ) );
      popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeObjects, _( "Objects" ) );
      popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeBoth, _( "Both" ) );
    }
    if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE )
    {
      popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeTime, _( "Time" ) );
      popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeObjects, _( "Objects" ) );
      popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeBoth, _( "Both" ) );
    }

    wxMenuItem *tmpDrawModeSubMenu = AppendSubMenu( popUpMenuDrawMode, _( "Drawmode" ) );

    if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE )
    {
      Enable( tmpDrawModeSubMenu->GetId(),
              ( allWindowHas(
                []( gTimeline *window )
                {
                  return ( ( !window->GetMyWindow()->isPunctualColorSet() ) || ( window->GetMyWindow()->getPunctualColorWindow() != nullptr ) );
                },
                []( gHistogram *win )
                {
                  return true;
                } ) ) );
    }
  }
}

void gPopUpMenu::buildPopUpMenuPixelSize()
{
  bool isSync   = true;
  int pixelSize = 0;

  auto visitor = overloads{ []( gHistogram *histogram )
                            {
                              return histogram->GetHistogram()->getPixelSize();
                            },
                            []( gTimeline *timeline )
                            {
                              return timeline->GetMyWindow()->getPixelSize();
                            }

  };

  for( auto &window : windowGenericList )
  {
    if( pixelSize == 0 )
    {
      pixelSize = std::visit( visitor, window );
    }
    else if( std::visit( visitor, window ) != pixelSize )
    {
      isSync = false;
      break;
    }
  }

  std::vector< BuildMenuItem > pixelSizeItems = {
    { popUpMenuPixelSize, _( "x1" ), wxITEM_CHECK, &gPopUpMenu::OnPopUpPixelSize, ID_MENU_PIXEL_SIZE_x1, ( isSync && pixelSize == 1 ) },

    { popUpMenuPixelSize, _( "x2" ), wxITEM_CHECK, &gPopUpMenu::OnPopUpPixelSize, ID_MENU_PIXEL_SIZE_x2, ( isSync && pixelSize == 2 ) },

    { popUpMenuPixelSize, _( "x4" ), wxITEM_CHECK, &gPopUpMenu::OnPopUpPixelSize, ID_MENU_PIXEL_SIZE_x4, ( isSync && pixelSize == 4 ) },

    { popUpMenuPixelSize, _( "x8" ), wxITEM_CHECK, &gPopUpMenu::OnPopUpPixelSize, ID_MENU_PIXEL_SIZE_x8, ( isSync && pixelSize == 8 ) }
  };

  buildListOfItems( pixelSizeItems );

  AppendSubMenu( popUpMenuPixelSize, _( "Pixel Size" ) );
}

void gPopUpMenu ::buildPopUpMenuGradientFunction()
{
  if( typeDataPopup != PopUpMenuType::POPUP_MENU_TYPE_MIXED )
  {
    TGradientFunction gradientFunctionSelected = TGradientFunction::LINEAR;

    bool valueSynchronyzed = true;
    bool initialValue      = true;

    auto checkGradientFunction = [ & ]( TGradientFunction timelineFunction ) -> bool
    {
      if( initialValue )
      {
        initialValue             = false;
        gradientFunctionSelected = timelineFunction;
        return true;
      }
      return gradientFunctionSelected == timelineFunction;
    };

    auto visitor = overloads{ []( gHistogram *histogram )
                              {
                                return histogram->GetHistogram()->getSemanticColor().getGradientFunction();
                              },
                              []( gTimeline *timeline )
                              {
                                return timeline->GetMyWindow()->getSemanticColor().getGradientFunction();
                              }

    };

    for( auto &window : windowGenericList )
    {
      if( !checkGradientFunction( std::visit( visitor, window ) ) )
      {
        valueSynchronyzed = false;
        break;
      }
    }

    std::vector< BuildMenuItem > gradientFunctionItems = {
      { popUpMenuGradientFunction,
        _( "Linear" ),
        wxITEM_CHECK,
        &gPopUpMenu::OnPopUpGradientFunction,
        ID_MENU_GRADIENT_FUNCTION_LINEAR,
        gradientFunctionSelected == TGradientFunction::LINEAR && ( !initialValue && valueSynchronyzed ) },

      { popUpMenuGradientFunction,
        _( "Steps" ),
        wxITEM_CHECK,
        &gPopUpMenu::OnPopUpGradientFunction,
        ID_MENU_GRADIENT_FUNCTION_STEPS,
        gradientFunctionSelected == TGradientFunction::STEPS && ( !initialValue && valueSynchronyzed ) },

      { popUpMenuGradientFunction,
        _( "Logarithmic" ),
        wxITEM_CHECK,
        &gPopUpMenu::OnPopUpGradientFunction,
        ID_MENU_GRADIENT_FUNCTION_LOGARITHMIC,
        gradientFunctionSelected == TGradientFunction::LOGARITHMIC && ( !initialValue && valueSynchronyzed ) },

      { popUpMenuGradientFunction,
        _( "Exponential" ),
        wxITEM_CHECK,
        &gPopUpMenu::OnPopUpGradientFunction,
        ID_MENU_GRADIENT_FUNCTION_EXPONENTIAL,
        gradientFunctionSelected == TGradientFunction::EXPONENTIAL && ( !initialValue && valueSynchronyzed ) }
    };

    buildListOfItems( gradientFunctionItems );

    if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_MULTIPLE )
    {
      this->AppendSubMenu( popUpMenuGradientFunction, _( "Gradient Function " ) );
    }
    else if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE )
    {
      wxMenuItem *tmpPuncWin =
        buildItem( popUpMenuColor, _( "Punctual Window..." ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPunctualColorWindow, ID_MENU_PUNCTUAL_WINDOW );

      wxMenuItem *tmpGradFunc = popUpMenuColor->AppendSubMenu( popUpMenuGradientFunction, _( "Gradient Function" ) );

      wxMenuItem *tmpSemScaleMinZero = buildItem( popUpMenuColor,
                                                  _( "Semantic scale min at 0" ),
                                                  wxITEM_CHECK,
                                                  &gPopUpMenu::OnPopUpSemanticScaleMinAtZero,
                                                  ID_MENU_SEMANTIC_SCALE_MIN_AT_ZERO,
                                                  allWindowHas(
                                                    []( gTimeline *window )
                                                    {
                                                      return window->GetMyWindow()->getSemanticScaleMinAtZero();
                                                    },
                                                    []( gHistogram *win )
                                                    {
                                                      return true;
                                                    } ) );

      this->AppendSubMenu( popUpMenuColor, _( "Paint As" ) );
    }
  }
}

void gPopUpMenu ::buildPopUpMenuObjectAxis()
{
  if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE )
  {
    bool isSync                = true;
    TObjectAxisSize objectAxis = ( std::get< gTimeline * >( *windowGenericList.begin() ) )->GetMyWindow()->getObjectAxisSize();

    auto visitor = overloads{ []( gHistogram *histogram )
                              {
                                return histogram->GetHistogram()->getDataWindow();
                              },
                              []( gTimeline *timeline )
                              {
                                return timeline->GetMyWindow();
                              }

    };

    for( auto &window : windowGenericList )
    {
      if( std::visit( visitor, window )->getObjectAxisSize() != objectAxis )
      {
        isSync = false;
        break;
      }
    }

    std::vector< BuildMenuItem > objectAxisItems = {
      { popUpMenuObjectAxis,
        _( "Fit Current Level" ),
        wxITEM_CHECK,
        &gPopUpMenu::OnPopUpObjectAxis,
        ID_MENU_OBJECT_AXIS_CURRENT,
        ( objectAxis == TObjectAxisSize::CURRENT_LEVEL && isSync ) },

      { popUpMenuObjectAxis,
        _( "Fit All Levels" ),
        wxITEM_CHECK,
        &gPopUpMenu::OnPopUpObjectAxis,
        ID_MENU_OBJECT_AXIS_ALL,
        ( objectAxis == TObjectAxisSize::ALL_LEVELS && isSync ) },

      { popUpMenuObjectAxis,
        _( "0%" ),
        wxITEM_CHECK,
        &gPopUpMenu::OnPopUpObjectAxis,
        ID_MENU_OBJECT_AXIS_ZERO,
        ( objectAxis == TObjectAxisSize::ZERO_PERC && isSync ) },

      { popUpMenuObjectAxis,
        _( "5%" ),
        wxITEM_CHECK,
        &gPopUpMenu::OnPopUpObjectAxis,
        ID_MENU_OBJECT_AXIS_FIVE,
        ( objectAxis == TObjectAxisSize::FIVE_PERC && isSync ) },

      { popUpMenuObjectAxis,
        _( "10%" ),
        wxITEM_CHECK,
        &gPopUpMenu::OnPopUpObjectAxis,
        ID_MENU_OBJECT_AXIS_TEN,
        ( objectAxis == TObjectAxisSize::TEN_PERC && isSync ) },

      { popUpMenuObjectAxis,
        _( "25%" ),
        wxITEM_CHECK,
        &gPopUpMenu::OnPopUpObjectAxis,
        ID_MENU_OBJECT_AXIS_TWENTYFIVE,
        ( objectAxis == TObjectAxisSize::TWENTYFIVE_PERC && isSync ) },
    };

    buildListOfItems( objectAxisItems );

    AppendSubMenu( popUpMenuObjectAxis, _( "Object Axis" ) );
  }
}

void gPopUpMenu ::buildPopUpMenuSave()
{
  std::vector< BuildMenuItem > saveMenuItems = {
    { popUpMenuSave, _( "Configuration..." ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpSaveCFG, ID_MENU_SAVE_TIMELINE_AS_CFG },
  };
  if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE )
  {
    saveMenuItems.push_back( { popUpMenuSave, _( "Image..." ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpSaveImageDialog, ID_MENU_SAVE_IMAGE } );

    if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE )
      saveMenuItems.push_back( { popUpMenuSave, _( "Text..." ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpSaveText, ID_MENU_SAVE_TIMELINE_AS_TEXT } );

    else if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE )
    {
      if( std::get< gHistogram * >( *windowGenericList.begin() )->GetHistogram()->getThreeDimensions() )
      {
        saveMenuItems.push_back(
          { popUpMenuSaveAsText, _( "Current Plane..." ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpSaveText, ID_MENU_SAVE_CURRENT_PLANE_AS_TEXT } );

        saveMenuItems.push_back(
          { popUpMenuSaveAsText, _( "All Planes..." ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpSaveText, ID_MENU_SAVE_ALL_PLANES_AS_TEXT } );

        popUpMenuSave->AppendSubMenu( popUpMenuSaveAsText, _( "Text..." ) );
      }
      else
        saveMenuItems.push_back( { popUpMenuSave, _( "Text..." ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpSaveText, ID_MENU_SAVE_ALL_PLANES_AS_TEXT } );
    }
  };
  buildListOfItems( saveMenuItems );

  AppendSubMenu( popUpMenuSave, _( "Save" ) );
}

void gPopUpMenu ::buildPopUpMenuRun()
{
}

void gPopUpMenu ::buildPopUpMenuLabels()
{
  if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE )
  {
    std::vector< BuildMenuItem > objectLabelItems = { { popUpMenuLabels,
                                                        _( "All" ),
                                                        wxITEM_CHECK,
                                                        &gPopUpMenu::OnPopUpLabels,
                                                        ID_MENU_LABELS_ALL,
                                                        allWindowHas(
                                                          []( gTimeline *win )
                                                          {
                                                            return win->GetMyWindow()->getObjectLabels() == TObjectLabels::ALL_LABELS;
                                                          },
                                                          []( gHistogram *win )
                                                          {
                                                            return true;
                                                          } ) },

                                                      { popUpMenuLabels,
                                                        _( "Spaced" ),
                                                        wxITEM_CHECK,
                                                        &gPopUpMenu::OnPopUpLabels,
                                                        ID_MENU_LABELS_SPACED,
                                                        allWindowHas(
                                                          []( gTimeline *win )
                                                          {
                                                            return win->GetMyWindow()->getObjectLabels() == TObjectLabels::SPACED_LABELS;
                                                          },
                                                          []( gHistogram *win )
                                                          {
                                                            return true;
                                                          } ) },

                                                      { popUpMenuLabels,
                                                        _( "2^n" ),
                                                        wxITEM_CHECK,
                                                        &gPopUpMenu::OnPopUpLabels,
                                                        ID_MENU_LABELS_POWER2,
                                                        allWindowHas(
                                                          []( gTimeline *win )
                                                          {
                                                            return win->GetMyWindow()->getObjectLabels() == TObjectLabels::POWER2_LABELS;
                                                          },
                                                          []( gHistogram *win )
                                                          {
                                                            return true;
                                                          } ) } };

    buildListOfItems( objectLabelItems );

    this->AppendSubMenu( popUpMenuLabels, _( "Object Labels" ) );
  }
}

void gPopUpMenu ::buildPopUpMenuSync()
{
  std::vector< TGroupId > tmpGroups;
  SyncWindows::getInstance()->getGroups( tmpGroups );
  TGroupId i              = 0;
  int syncValue           = -1;
  bool windowsSyncronized = true;

  auto checkSyncronizedGroup = [ & ]( int group ) -> bool
  {
    if( syncValue == -1 )
    {
      syncValue = group;
      return true;
    }
    return syncValue == group;
  };
  auto visitor_isSync = overloads{ []( gHistogram *histogram )
                                   {
                                     return histogram->GetHistogram()->isSync();
                                   },
                                   []( gTimeline *timeline )
                                   {
                                     return timeline->GetMyWindow()->isSync();
                                   }

  };

  auto visitor = overloads{ []( gHistogram *histogram )
                            {
                              return histogram->GetHistogram()->getSyncGroup();
                            },
                            []( gTimeline *timeline )
                            {
                              return timeline->GetMyWindow()->getSyncGroup();
                            }

  };

  for( auto &window : windowGenericList )
  {
    if( std::visit( visitor_isSync, window ) )
    {
      if( !checkSyncronizedGroup( std::visit( visitor, window ) ) )
      {
        windowsSyncronized = false;
        break;
      }
    }
    else
    {
      windowsSyncronized = false;
      break;
    }
  }

  bool checked;

  for( vector< TGroupId >::const_iterator itGroup = tmpGroups.begin(); itGroup != tmpGroups.end(); ++itGroup )
  {
    std::string strShortcut = "\tCtrl+" + std::to_string( i + 1 );
    wxString label          = wxString::Format( _( "%u" ), *itGroup + 1 ) + wxString( strShortcut );

    bool checked = ( windowsSyncronized && syncValue == *itGroup );
    this->buildItem( popUpMenuSync, label, wxITEM_CHECK, &gPopUpMenu::OnPopUpSynchronize, ID_MENU_SYNC_GROUP_BASE + i, checked );

    ++i;
  }
  popUpMenuSync->AppendSeparator();
  this->buildItem( popUpMenuSync, _( STR_SYNC_NEW_GROUP ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpSynchronize, ID_MENU_NEWGROUP );
  i = 0;
  for( std::vector< TGroupId >::const_iterator itGroup = tmpGroups.begin(); itGroup != tmpGroups.end(); ++itGroup )
  {
    this->buildItem( popUpMenuSyncPropertiesType,
                     wxString::Format( _( "%u" ), *itGroup + 1 ),
                     wxITEM_NORMAL,
                     &gPopUpMenu::OnPopUpSynchronizeProperties,
                     ID_MENU_SYNC_PROPERTIES_GROUP_BASE + i );
    ++i;
  }

  popUpMenuSync->AppendSubMenu( popUpMenuSyncPropertiesType, _( STR_SYNC_GROUP_PROP ) );

  i = 0;
  for( std::vector< TGroupId >::const_iterator itGroup = tmpGroups.begin(); itGroup != tmpGroups.end(); ++itGroup )
  {
    this->buildItem( popUpMenuSyncRemove,
                     wxString::Format( _( "%u" ), *itGroup + 1 ),
                     wxITEM_NORMAL,
                     &gPopUpMenu::OnPopUpRemoveGroup,
                     ID_MENU_SYNC_REMOVE_GROUP_BASE + i );
    ++i;
  }
  popUpMenuSyncRemove->AppendSeparator();
  this->buildItem( popUpMenuSyncRemove,
                   _( STR_SYNC_REMOVE_ALL_GROUPS ),
                   wxITEM_NORMAL,
                   &gPopUpMenu::OnPopUpRemoveAllGroups,
                   ID_MENU_SYNC_REMOVE_ALL_GROUPS );
  popUpMenuSync->AppendSubMenu( popUpMenuSyncRemove, _( STR_SYNC_REMOVE_GROUP ) );

  AppendSubMenu( popUpMenuSync, _( STR_SYNCHRONIZE ) );
}

void gPopUpMenu::buildPopUpRunApp()
{
  if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE )
  {
    auto suitableApps = ( std::get< gTimeline * >( *windowGenericList.begin() ) )->GetMyWindow()->getTrace()->getSuitableApps();

    this->buildItem( popUpMenuRun, _( "Cutter" ), wxITEM_NORMAL, &gPopUpMenu::OnPopUpRunApp, ID_MENU_CUTTER );

    if( suitableApps[ (int)TExternalAppID::DIMEMAS ] )
      this->buildItem( popUpMenuRun,
                       ExternalApps::getApplicationLabel( TExternalAppID::DIMEMAS ),
                       wxITEM_NORMAL,
                       &gPopUpMenu::OnPopUpRunApp,
                       ID_MENU_DIMEMAS );

    if( suitableApps[ (int)TExternalAppID::CLUSTERING ] )
      this->buildItem( popUpMenuRun,
                       ExternalApps::getApplicationLabel( TExternalAppID::CLUSTERING ),
                       wxITEM_NORMAL,
                       &gPopUpMenu::OnPopUpRunApp,
                       ID_MENU_CLUSTERING );

    if( suitableApps[ (int)TExternalAppID::FOLDING ] )
      this->buildItem( popUpMenuRun,
                       ExternalApps::getApplicationLabel( TExternalAppID::FOLDING ),
                       wxITEM_NORMAL,
                       &gPopUpMenu::OnPopUpRunApp,
                       ID_MENU_FOLDING );

    if( suitableApps[ (int)TExternalAppID::MESS ] )
      this->buildItem( popUpMenuRun,
                       ExternalApps::getApplicationLabel( TExternalAppID::MESS ),
                       wxITEM_NORMAL,
                       &gPopUpMenu::OnPopUpRunApp,
                       ID_MENU_MESS );

    this->buildItem( popUpMenuRun,
                     ExternalApps::getApplicationLabel( TExternalAppID::USER_COMMAND ),
                     wxITEM_NORMAL,
                     &gPopUpMenu::OnPopUpRunApp,
                     ID_MENU_USER_COMMAND );
    AppendSubMenu( popUpMenuRun, _( "Run" ) );
  }
}

void gPopUpMenu::buildPopUpExtraPanel()
{
  if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE )
  {
    std::vector< BuildMenuItem > viewMenuItems = { { this,
                                                     _( "Timing\tCTRL+T" ),
                                                     wxITEM_CHECK,
                                                     &gPopUpMenu::OnPopUpTiming,
                                                     ID_MENU_TIMING,
                                                     allWindowHas(
                                                       []( gTimeline *win )
                                                       {
                                                         return win->GetTiming();
                                                       },
                                                       []( gHistogram *win )
                                                       {
                                                         return true;
                                                       } ) },

                                                   { this,
                                                     _( "Info Panel" ),
                                                     wxITEM_CHECK,
                                                     &gPopUpMenu::OnPopUpInfoPanel,
                                                     ID_MENU_INFO_PANEL,
                                                     allWindowHas(
                                                       []( gTimeline *win )
                                                       {
                                                         return win->IsSplit();
                                                       },
                                                       []( gHistogram *win )
                                                       {
                                                         return true;
                                                       } ) } };
    buildListOfItems( viewMenuItems );
  }
}

wxMenuItem *gPopUpMenu::buildItem( wxMenu *baseMenuContainer,
                                   const wxString &titleMenuItem,
                                   const wxItemKind &typeMenuItem,
                                   void ( gPopUpMenu::*callbackFunctionMenuItem )( wxCommandEvent & ),
                                   const wxWindowID &windowId,
                                   bool isChecked )
{
  wxMenuItem *newMenuItem;

  newMenuItem = new wxMenuItem( baseMenuContainer, windowId, titleMenuItem, _( "" ), typeMenuItem );

  baseMenuContainer->Append( newMenuItem );

  if( newMenuItem->IsCheckable() )
  {
    newMenuItem->Check( isChecked );
  }

  // #ifdef _WIN32
  //   Bind( wxEVT_COMMAND_MENU_SELECTED, function, window, id );
  // #else
  baseMenuContainer->Bind( wxEVT_COMMAND_MENU_SELECTED, callbackFunctionMenuItem, this, windowId );

  // #endif

  return newMenuItem;
}

template< typename... Funcs >
bool gPopUpMenu::allWindowHas( Funcs &&...funcs )
{
  auto visitor = overloads{ std::forward< Funcs >( funcs )... };

  return std::all_of( windowGenericList.begin(),
                      windowGenericList.end(),
                      [ & ]( const gWindowGenericItem &item )
                      {
                        return std::visit( visitor, item );
                      } );
}

bool gPopUpMenu::allWindowsHas( vector< gTimeline * > timelineWindows, std::function< bool( gTimeline * ) > function )
{
  for( auto *timeline : timelineWindows )
  {
    if( function( timeline ) == false )
    {
      return false;
    }
  }
  return true;
}
bool gPopUpMenu::allWindowsHas( vector< gHistogram * > histogramWindows, std::function< bool( gHistogram * ) > function )
{
  for( auto *histogram : histogramWindows )
  {
    if( function( histogram ) == false )
    {
      return false;
    }
  }
  return true;
}

/*************************************************************/
/*************ENABLE POP UP MENU FUNCTIONALLITIES*************/
/*************************************************************/

bool gPopUpMenu::checkAllowedProperties( const char *property )
{
  bool rv                                  = true;
  gPasteWindowProperties *sharedProperties = gPasteWindowProperties::getInstance();

  auto visitor = [ & ]( auto *win )
  {
    return sharedProperties->isAllowed( win, property );
  };

  for( auto &window : windowGenericList )
  {
    if( !std::visit( visitor, window ) )
    {
      return false;
    }
  }

  return rv;
}

void gPopUpMenu::enablePopUpMenu()
{
  auto tryToEnableItem = [ & ]( wxMenu *menuComponent, char *itemName ) -> void
  {
    int itemId = FindItem( _( itemName ) );
    if( itemId != wxNOT_FOUND )
    {
      menuComponent->Enable( itemId, true );
    }
  };

  gPasteWindowProperties *sharedProperties = gPasteWindowProperties::getInstance();

  this->Enable( FindItem( _( STR_PASTE ) ), checkAllowedProperties( STR_PASTE ) );
  this->Enable( FindItem( _( STR_PASTE_DEFAULT_SPECIAL ) ), checkAllowedProperties( STR_PASTE_DEFAULT_SPECIAL ) );
  this->Enable( FindItem( _( STR_PASTE_SPECIAL ) ), checkAllowedProperties( STR_PASTE_SPECIAL ) );

  if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_MULTIPLE )
  {
    bool isDerivedHistogram = std::get< gHistogram * >( *windowGenericList.begin() )->GetHistogram()->isDerivedHistogram();

    this->Enable( FindItem( _( STR_CONTROL_SCALE ) ), !isDerivedHistogram && checkAllowedProperties( STR_CONTROL_SCALE ) );
    this->Enable( FindItem( _( STR_PASTE_SEMANTIC_SORT ) ), checkAllowedProperties( STR_PASTE_SEMANTIC_SORT ) );
    this->Enable( FindItem( _( STR_CONTROL_DIMENSIONS ) ), checkAllowedProperties( STR_CONTROL_DIMENSIONS ) );

    this->Enable( FindItem( _( STR_FIT_TIME ) ), !isDerivedHistogram );

    this->Enable( FindItem( _( STR_AUTOFIT_CONTROL_ZERO ) ),
                  !isDerivedHistogram &&
                  allWindowHas(
                    []( gHistogram *win )
                    {
                      return win->GetHistogram()->getCompute2DScale();
                    },
                    []( gTimeline *win )
                    {
                      return true;
                    } ) );

    if( allWindowHas(
          []( gHistogram *win )
          {
            return win->GetHistogram()->getThreeDimensions();
          },
          []( gTimeline *win )
          {
            return true;
          } ) )
    {
      this->Enable( FindItem( _( STR_3D_SCALE ) ), checkAllowedProperties( STR_3D_SCALE ) );
    }
    else
    {
      this->Enable( FindItem( _( STR_3D_SCALE ) ), false );
    }

    // TODO: may need fine tuning
    if ( isDerivedHistogram )
    {
      auto mainParentSynchGroup = std::get< gHistogram * >( *windowGenericList.begin() )->GetHistogram()->getParent( 0 )->getSyncGroup();
      this->Enable( FindItem( _( STR_SYNCHRONIZE ) ),
                              allWindowHas(
                                [mainParentSynchGroup]( gHistogram *win )
                                {
                                  auto parents = win->GetHistogram()->getParents();
                                  return std::all_of( parents.cbegin(),
                                                      parents.cend(),
                                                      [mainParentSynchGroup] ( auto p )
                                                      { 
                                                        return p->isSync() && p->getSyncGroup() == mainParentSynchGroup;
                                                      } );
                                },
                                []( gTimeline *win )
                                {
                                  return true;
                                }  ) );
    }
  }
  if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE )
  {
    this->Enable( FindItem( _( STR_REDO_ZOOM ) ), !( std::get< gHistogram * >( *windowGenericList.begin() ) )->GetHistogram()->emptyPrevZoom() );
    this->Enable( FindItem( _( STR_UNDO_ZOOM ) ), !( std::get< gHistogram * >( *windowGenericList.begin() ) )->GetHistogram()->emptyNextZoom() );
  }
  if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE )
  {
    this->Enable( FindItem( _( STR_REDO_ZOOM ) ), !( std::get< gTimeline * >( *windowGenericList.begin() ) )->GetMyWindow()->emptyNextZoom() );
    this->Enable( FindItem( _( STR_UNDO_ZOOM ) ), !( std::get< gTimeline * >( *windowGenericList.begin() ) )->GetMyWindow()->emptyPrevZoom() );
  }

  this->Enable( FindItem( _( "Select Objects..." ) ), isSelectObjectsAvailable() );

  if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE )
  {
    popUpMenuColor->Enable( FindItem( _( "Punctual Window..." ) ),
                            allWindowHas(
                              []( gTimeline *window )
                              {
                                return window->GetMyWindow()->isPunctualColorSet();
                              },
                              []( gHistogram *win )
                              {
                                return true;
                              } ) );

    popUpMenuColor->Enable( FindItem( _( "Gradient Function" ) ),
                            ( allWindowHas(
                                []( gTimeline *window )
                                {
                                  return window->GetMyWindow()->isGradientColorSet();
                                },
                                []( gHistogram *win )
                                {
                                  return true;
                                } ) ||
                              allWindowHas(
                                []( gTimeline *window )
                                {
                                  return window->GetMyWindow()->isNotNullGradientColorSet();
                                },
                                []( gHistogram *win )
                                {
                                  return true;
                                } ) ||
                              allWindowHas(
                                []( gTimeline *window )
                                {
                                  return window->GetMyWindow()->isAlternativeGradientColorSet();
                                },
                                []( gHistogram *win )
                                {
                                  return true;
                                } ) ||
                              allWindowHas(
                                []( gTimeline *window )
                                {
                                  return window->GetMyWindow()->isFunctionLineColorSet();
                                },
                                []( gHistogram *win )
                                {
                                  return true;
                                } ) ||
                              allWindowHas(
                                []( gTimeline *window )
                                {
                                  return window->GetMyWindow()->isPunctualColorSet();
                                },
                                []( gHistogram *win )
                                {
                                  return true;
                                } ) ) );

    popUpMenuColor->Enable( FindItem( _( "Semantic scale min at 0" ) ),
                            ( allWindowHas(
                                []( gTimeline *window )
                                {
                                  return window->GetMyWindow()->isFunctionLineColorSet();
                                },
                                []( gHistogram *win )
                                {
                                  return true;
                                } ) ||
                              allWindowHas(
                                []( gTimeline *window )
                                {
                                  return window->GetMyWindow()->isFusedLinesColorSet();
                                },
                                []( gHistogram *win )
                                {
                                  return true;
                                } ) ||
                              allWindowHas(
                                []( gTimeline *window )
                                {
                                  return window->GetMyWindow()->isPunctualColorSet();
                                },
                                []( gHistogram *win )
                                {
                                  return true;
                                } ) ) );
  }

  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_TIME ) ), checkAllowedProperties( STR_TIME ) );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_SIZE ) ), checkAllowedProperties( STR_SIZE ) );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_OBJECTS ) ), checkAllowedProperties( STR_OBJECTS ) );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_DURATION ) ), checkAllowedProperties( STR_DURATION ) );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_SEMANTIC_SCALE ) ), checkAllowedProperties( STR_SEMANTIC_SCALE ) );

  if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE )
  {
    popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_CUSTOM_PALETTE ) ), checkAllowedProperties( STR_CUSTOM_PALETTE ) );
    popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_FILTER ) ), checkAllowedProperties( STR_FILTER ) );
    popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( _( STR_FILTER_ALL ) ), checkAllowedProperties( STR_FILTER_ALL ) );
    popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( _( STR_FILTER_COMMS ) ), checkAllowedProperties( STR_FILTER_COMMS ) );
    popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( _( STR_FILTER_EVENTS ) ), checkAllowedProperties( STR_FILTER_EVENTS ) );
  }

  auto sameTrace = isShowTraceWindowsAvailable();
  popUpMenuShowWindows->Enable( popUpMenuShowWindows->FindItem( _( STR_SHOW_SELECTED_WINDOWS_TRACE ) ), sameTrace );
  popUpMenuShowWindows->Enable( popUpMenuShowWindows->FindItem( _( STR_HIDE_SELECTED_WINDOWS_TRACE ) ), sameTrace );

  bool tmpEnableRemoveGroup = SyncWindows::getInstance()->getNumGroups() > 1 || SyncWindows::getInstance()->getNumWindows( 0 ) > 0;
  popUpMenuSync->Enable( popUpMenuSync->FindItem( _( STR_SYNC_REMOVE_GROUP ) ), tmpEnableRemoveGroup );
}

void gPopUpMenu::enableItemByTag( const std::string tag, bool enable )
{
  Enable( FindItem( wxString::FromUTF8( tag.c_str() ) ), enable );
}

void gPopUpMenu::enableItemByTag( const std::string tag )
{
  Enable( FindItem( wxString::FromUTF8( tag.c_str() ) ), true );
}

void gPopUpMenu::disableItemByTag( const std::string tag )
{
  Enable( FindItem( wxString::FromUTF8( tag.c_str() ) ), false );
}

/*************************************************************/
/********************EXTRA FUNCTIONALITIES********************/
/*************************************************************/

std::string gPopUpMenu::getOption( wxArrayString &choices, int position )
{
  if( choices[ position ].Cmp( _( STR_FILTER_COMMS_XT ) ) == 0 )
    return std::string( STR_FILTER_COMMS );
  else if( choices[ position ].Cmp( _( STR_FILTER_EVENTS_XT ) ) == 0 )
    return std::string( STR_FILTER_EVENTS );
  else
    return std::string( choices[ position ].mb_str() );
}

wxMultiChoiceDialog *gPopUpMenu::createPasteSpecialDialog( wxArrayString &choices, gHistogram *whichHistogram )
{
  wxArrayInt sel;
  int i = 0;

  choices.Empty();

  gPasteWindowProperties *pasteActions = gPasteWindowProperties::getInstance();

  if( pasteActions->isAllowed( whichHistogram, STR_TIME ) )
  {
    choices.Add( _( STR_TIME ) );
    sel.Add( i );
    ++i;
  }

  if( pasteActions->isAllowed( whichHistogram, STR_OBJECTS ) )
  {
    choices.Add( _( STR_OBJECTS ) );
    sel.Add( i );
    ++i;
  }

  if( pasteActions->isAllowed( whichHistogram, STR_SIZE ) )
  {
    choices.Add( _( STR_SIZE ) );
    sel.Add( i );
    ++i;
  }

  if( pasteActions->isAllowed( whichHistogram, STR_DURATION ) )
    choices.Add( _( STR_DURATION ) );

  if( pasteActions->isAllowed( whichHistogram, STR_SEMANTIC_SCALE ) )
    choices.Add( _( STR_SEMANTIC_SCALE ) );

  if( pasteActions->isAllowed( whichHistogram, STR_CONTROL_SCALE ) )
    choices.Add( _( STR_CONTROL_SCALE ) );

  if( pasteActions->isAllowed( whichHistogram, STR_CONTROL_DIMENSIONS ) )
    choices.Add( _( STR_CONTROL_DIMENSIONS ) );

  if( pasteActions->isAllowed( whichHistogram, STR_CONTROL_SCALE ) )
  {
    if( whichHistogram->GetHistogram()->getThreeDimensions() )
      choices.Add( _( STR_3D_SCALE ) );
  }

  wxMultiChoiceDialog *tmpDialog = new wxMultiChoiceDialog( whichHistogram, _( "Select properties to paste:" ), _( "Paste Special" ), choices );
  tmpDialog->SetSelections( sel );

  return tmpDialog;
}

wxMultiChoiceDialog *gPopUpMenu::createPasteSpecialDialog( wxArrayString &choices, gTimeline *whichTimeline )
{
  wxArrayInt sel;
  int i = 0;

  choices.Empty();
  gPasteWindowProperties *pasteActions = gPasteWindowProperties::getInstance();

  if( pasteActions->isAllowed( whichTimeline, STR_TIME ) )
  {
    choices.Add( _( STR_TIME ) );
    sel.Add( i );
    ++i;
  }

  if( pasteActions->isAllowed( whichTimeline, STR_OBJECTS ) )
  {
    choices.Add( _( STR_OBJECTS ) );
    sel.Add( i );
    ++i;
  }

  if( pasteActions->isAllowed( whichTimeline, STR_SIZE ) )
  {
    choices.Add( _( STR_SIZE ) );
    sel.Add( i );
    ++i;
  }

  if( pasteActions->isAllowed( whichTimeline, STR_DURATION ) )
    choices.Add( _( STR_DURATION ) );

  if( pasteActions->isAllowed( whichTimeline, STR_SEMANTIC_SCALE ) )
    choices.Add( _( STR_SEMANTIC_SCALE ) );

  if( pasteActions->isAllowed( whichTimeline, STR_FILTER_COMMS ) )
    choices.Add( _( STR_FILTER_COMMS_XT ) );

  if( pasteActions->isAllowed( whichTimeline, STR_FILTER_EVENTS ) )
    choices.Add( _( STR_FILTER_EVENTS_XT ) );

  wxMultiChoiceDialog *tmpDialog = new wxMultiChoiceDialog( whichTimeline, _( "Select properties to paste:" ), _( "Paste Special" ), choices );
  tmpDialog->SetSelections( sel );

  return tmpDialog;
}

bool gPopUpMenu ::isSelectObjectsAvailable()
{
  bool sameTraceParameters = true;
  bool sameTraceLevelMode  = true;
  bool isProcessModel;

  Trace *trace = nullptr;
  TTraceLevel dataLevel;

  auto isProcessModelCheck = [ & ]( TTraceLevel traceLevel ) -> bool
  {
    return ( ( traceLevel >= TTraceLevel::WORKLOAD ) && ( traceLevel <= TTraceLevel::THREAD ) );
  };

  auto visitor = overloads{ []( gHistogram *histogram )
                            {
                              return histogram->GetHistogram()->getDataWindow();
                            },
                            []( gTimeline *timeline )
                            {
                              return timeline->GetMyWindow();
                            }

  };

  auto dataWindow = std::visit( visitor, ( windowGenericList[ 0 ] ) );
  trace           = dataWindow->getTrace();
  isProcessModel  = isProcessModelCheck( dataWindow->getLevel() );

  for( auto &window : windowGenericList )
  {
    dataWindow = std::visit( visitor, window );
    if( !trace->isSameObjectStruct( dataWindow->getTrace(), true ) )
    {
      sameTraceParameters = false;
      break;
    }
    if( !( isProcessModel == isProcessModelCheck( dataWindow->getLevel() ) ) )
    {
      sameTraceLevelMode = false;
      break;
    }
  }

  return ( sameTraceParameters && sameTraceLevelMode );
}

bool gPopUpMenu ::isShowTraceWindowsAvailable()
{
  bool sameTrace = true;

  Trace *trace = nullptr;

  auto visitor = overloads{ []( gHistogram *histogram )
                            {
                              return histogram->GetHistogram()->getDataWindow();
                            },
                            []( gTimeline *timeline )
                            {
                              return timeline->GetMyWindow();
                            }

  };

  auto dataWindow = std::visit( visitor, ( windowGenericList[ 0 ] ) );
  trace           = dataWindow->getTrace();

  for( auto &window : windowGenericList )
  {
    dataWindow = std::visit( visitor, window );
    if( trace != dataWindow->getTrace() )
    {
      sameTrace = false;
      break;
    }
  }

  return sameTrace;
}

void gPopUpMenu ::createRowSelectionDialog()
{
  bool isInitialized  = false;
  bool isProcessModel = false;
  Trace *trace        = nullptr;

  SelectionManagement< TObjectOrder, TTraceLevel > intersectionSelectedRows;
  std::vector< std::vector< TObjectOrder > > tmpWindowSelection;
  std::vector< std::vector< TObjectOrder > > intersectedSelection;

  auto visitor = overloads{ []( gHistogram *histogram )
                            {
                              return histogram->GetHistogram()->getDataWindow();
                            },
                            []( gTimeline *timeline )
                            {
                              return timeline->GetMyWindow();
                            }

  };

  auto visitorSelectedRows = overloads{ []( gHistogram *histogram )
                                        {
                                          return histogram->GetHistogram()->getRowSelectionManager();
                                        },
                                        []( gTimeline *timeline )
                                        {
                                          return timeline->GetMyWindow()->getRowSelectionManager();
                                        }

  };

  auto dataWindow   = std::visit( visitor, *windowGenericList.begin() );
  auto selectedRows = std::visit( visitorSelectedRows, *windowGenericList.begin() );
  selectedRows.getAllSelected( intersectedSelection );

  isProcessModel = ( ( dataWindow->getLevel() >= TTraceLevel::WORKLOAD ) && ( dataWindow->getLevel() <= TTraceLevel::THREAD ) );
  trace          = dataWindow->getTrace();

  for( auto &window : windowGenericList )
  {
    dataWindow = std::visit( visitor, window );
    dataWindow->getRowSelectionManager().getAllSelected( tmpWindowSelection );

    intersectedSelection = matrix_intersection( tmpWindowSelection, intersectedSelection );
  }

  TTraceLevel beginLevel;
  TTraceLevel endLevel;

  intersectionSelectedRows.init( trace );

  intersectionSelectedRows.setAllSelected( intersectedSelection );

  RowsSelectionDialog *myDialog = new RowsSelectionDialog( trace, isProcessModel, intersectionSelectedRows );

  if( myDialog->ShowModal() == wxID_OK )
  {
    bool applyZoom = ZoomAwareTransferData( myDialog );

    transferDataSelectionToObjects( myDialog, isProcessModel, &intersectionSelectedRows, applyZoom );
  }

  delete myDialog;
}

void gPopUpMenu::openSyncSelection( const TGroupId &groupId )
{
  wxArrayString choices;
  wxArrayInt preSelections;

  std::vector< SyncPropertiesType > properties;
  SyncWindows::getInstance()->getGroupAvailableProperties( groupId, properties );

  auto tmpIt = 0;
  for( auto &prop : properties )
  {
    choices.Add( LabelConstructor::propertyToLabel( prop ) );

    if( SyncWindows::getInstance()->isPropertySelected( groupId, prop ) )
    {
      preSelections.Add( tmpIt );
    }

    tmpIt += 1;
  }

  wxMultiChoiceDialog *dialog = new wxMultiChoiceDialog( nullptr, "Select the properties you want:", "Sync Group Properties", choices );

  dialog->SetSelections( preSelections );
  if( dialog->ShowModal() == wxID_OK )
  {
    wxArrayInt selections = dialog->GetSelections();

    std::vector< SyncPropertiesType > selectedProperties;

    for( size_t i = 0; i < selections.GetCount(); i++ )
    {
      auto tmpSelectedProperty = LabelConstructor::labelToProperty( gPopUpMenu::getOption( choices, selections[ i ] ) );
      selectedProperties.push_back( tmpSelectedProperty );
    }
    for( auto &prop : properties )
    {
      if( std::find( selectedProperties.begin(), selectedProperties.end(), prop ) != selectedProperties.end() )
      {
        SyncWindows::getInstance()->addProperty( groupId, prop );
      }
      else
      {
        SyncWindows::getInstance()->removeProperty( groupId, prop );
      }
    }
    SyncWindows::getInstance()->broadcastProperty( groupId );
  }


  delete dialog;
}


bool gPopUpMenu::ZoomAwareTransferData( RowsSelectionDialog *myDialog )
{
  auto isZoomAware = true;
  bool applyZoom   = true;

  isZoomAware = allWindowHas(
    [ myDialog ]( gHistogram *histogram )
    {
      return myDialog->isZoomAwareTransferData( histogram->GetHistogram()->getCurrentZoomRange() );
    },
    [ myDialog ]( gTimeline *timeline )
    {
      return myDialog->isZoomAwareTransferData( timeline->GetMyWindow()->getCurrentZoomRange() );
    } );

  if( !isZoomAware )
  {
    wxString tmpMsg = wxT( "Do you want to extend the zoom to fit selected objects?" );
    int answer      = wxMessageBox( tmpMsg, _( "Paraver question" ), wxYES_NO | wxICON_QUESTION, myDialog );

    applyZoom = ( answer == wxYES );
  }
  return applyZoom;
}
void gPopUpMenu ::transferDataSelectionToObjects( RowsSelectionDialog *myDialog,
                                                  bool isProcessModel,
                                                  SelectionManagement< TObjectOrder, TTraceLevel > *intersectionSelectedRows,
                                                  bool applyZoom )
{
  TTraceLevel beginLevel;
  TTraceLevel endLevel;

  // Set range of levels for update loop
  if( isProcessModel )
  {
    beginLevel = TTraceLevel::APPLICATION;
    endLevel   = TTraceLevel::THREAD;
  }
  else
  {
    beginLevel = TTraceLevel::NODE;
    endLevel   = TTraceLevel::CPU;
  }

  // Loop through levels to update gTimeline
  for( TTraceLevel whichLevel = beginLevel; whichLevel <= endLevel; ++whichLevel )
  {
    std::vector< bool > whichSelected;
    intersectionSelectedRows->getSelected( whichSelected, whichLevel );

    auto visitor = overloads{ []( gHistogram *histogram )
                              {
                                return histogram->GetHistogram();
                              },
                              []( gTimeline *timeline )
                              {
                                return timeline->GetMyWindow();
                              }

    };

    gTimeline *tmpTimeline;
    gHistogram *tmpHistogram;

    for( auto &window : windowGenericList )
    {
      if( std::holds_alternative< gTimeline * >( window ) )
      {
        tmpTimeline    = std::get< gTimeline * >( window );
        auto zoomAware = myDialog->isZoomAwareTransferData( tmpTimeline->GetMyWindow()->getCurrentZoomRange() );
        if( applyZoom && !zoomAware )
        {
          tmpTimeline->GetMyWindow()->addZoom( myDialog->GetNewBeginZoom(), myDialog->GetNewEndZoom() );
        }
        tmpTimeline->GetMyWindow()->setSelectedRows( whichLevel, whichSelected );
        tmpTimeline->GetMyWindow()->setRedraw( true );
        tmpTimeline->GetMyWindow()->setChanged( true );
      }
      if( std::holds_alternative< gHistogram * >( window ) )
      {
        tmpHistogram = std::get< gHistogram * >( window );
        std::vector< TObjectOrder > selection;
        tmpHistogram->GetHistogram()->getRowSelectionManager().getSelected( selection, tmpHistogram->GetHistogram()->getDataWindow()->getLevel() );

        auto zoomAware = myDialog->isZoomAwareTransferData( selection );
        if( applyZoom && !zoomAware )
        {
          tmpHistogram->GetHistogram()->addZoom( myDialog->GetNewBeginZoom(), myDialog->GetNewEndZoom() );
        }
        tmpHistogram->GetHistogram()->setSelectedRows( whichLevel, whichSelected );
        tmpHistogram->GetHistogram()->setRecalc( true );
      }
    }
  }
}

RowsSelectionDialog *gPopUpMenu ::createRowSelectionDialog( gTimeline *whichTimeline )
{
  bool isProcessModel =
    ( ( whichTimeline->GetMyWindow()->getLevel() >= TTraceLevel::WORKLOAD ) && ( whichTimeline->GetMyWindow()->getLevel() <= TTraceLevel::THREAD ) );

  RowsSelectionDialog *myDialog =
    new RowsSelectionDialog( whichTimeline->GetMyWindow()->getTrace(), isProcessModel, whichTimeline->GetMyWindow()->getRowSelectionManager() );

  return myDialog;
}

RowsSelectionDialog *gPopUpMenu::createRowSelectionDialog( gHistogram *histogram )
{
  bool isProcessModel = ( ( histogram->GetHistogram()->getDataWindow()->getLevel() >= TTraceLevel::WORKLOAD ) &&
                          ( histogram->GetHistogram()->getDataWindow()->getLevel() <= TTraceLevel::THREAD ) );

  RowsSelectionDialog *myDialog =
    new RowsSelectionDialog( histogram->GetHistogram()->getTrace(), isProcessModel, histogram->GetHistogram()->getRowSelectionManager() );

  return myDialog;
}

/*************************************************************/
/********************ON EVENTS FUNCTIONS**********************/
/*************************************************************/

void gPopUpMenu::OnPopUpCopy( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpCopy( event );
    } );
}

void gPopUpMenu::OnPopUpPasteDefaultSpecial( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpPasteDefaultSpecial( event );
    } );
}
void gPopUpMenu::OnPopUpPasteSpecial( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpPasteSpecial( event );
    } );
}

void gPopUpMenu::OnPopUpPasteTime( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpPasteTime( event );
    } );
}
void gPopUpMenu::OnPopUpPasteObjects( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpPasteObjects( event );
    } );
}
void gPopUpMenu::OnPopUpPasteSize( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpPasteSize( event );
    } );
}
void gPopUpMenu::OnPopUpPasteDuration( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpPasteDuration( event );
    } );
}
void gPopUpMenu::OnPopUpPasteSemanticScale( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpPasteSemanticScale( event );
    } );
}
void gPopUpMenu::OnPopUpPasteSemanticSort( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gHistogram *item )
    {
      item->OnPopUpPasteSemanticSort( event );
    },
    []( gTimeline *item )
    {
    } );
}

// TIMELINE
void gPopUpMenu::OnPopUpPasteCustomPalette( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpPasteCustomPalette( event );
    },
    []( gHistogram *item )
    {
    } );
}
void gPopUpMenu::OnPopUpPasteFilterAll( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpPasteFilterAll( event );
    },
    []( gHistogram *item )
    {
    } );
}

void gPopUpMenu::OnPopUpPasteFilterCommunications( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpPasteFilterCommunications( event );
    },
    []( gHistogram *item )
    {
    } );
}
void gPopUpMenu::OnPopUpPasteFilterEvents( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpPasteFilterEvents( event );
    },
    []( gHistogram *item )
    {
    } );
}

// HISGORAM
void gPopUpMenu::OnPopUpPasteControlScale( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gHistogram *item )
    {
      item->OnPopUpPasteControlScale( event );
    },
    []( gTimeline *item )
    {
    } );
}

void gPopUpMenu::OnPopUpPaste3DScale( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gHistogram *item )
    {
      item->OnPopUpPaste3DScale( event );
    },
    []( gTimeline *item )
    {
    } );
}
void gPopUpMenu::OnPopUpPasteControlDimensions( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gHistogram *item )
    {
      item->OnPopUpPasteControlDimensions( event );
    },
    []( gTimeline *item )
    {
    } );
}

// MIX
void gPopUpMenu::OnPopUpClone( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto &item )
    {
      item->OnPopUpClone( event );
    } );
}

void gPopUpMenu::OnPopUpRename( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto &item )
    {
      item->OnPopUpRename( event );
    } );
}

void gPopUpMenu::OnPopUpFitTimeScale( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto &item )
    {
      item->OnPopUpFitTimeScale( event );
    } );
}

void gPopUpMenu::OnPopUpFitObjects( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto &item )
    {
      item->OnPopUpFitObjects( event );
    } );
}

void gPopUpMenu::OnPopUpShowSelectedWindows( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->GetMyWindow()->setShowWindow( true );
    },
    [ &event ]( gHistogram *item )
    {
      item->GetHistogram()->setShowWindow( true );
    } );
}

void gPopUpMenu::OnPopUpHideSelectedWindows( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->GetMyWindow()->setShowWindow( false );
    },
    [ &event ]( gHistogram *item )
    {
      item->GetHistogram()->setShowWindow( false );
    } );
}
void gPopUpMenu::OnPopUpShowOnlySelectedWindows( wxCommandEvent &event )
{
  std::vector< Timeline * > timelines;
  std::vector< Histogram * > histograms;

  LoadedWindows::getInstance()->getAll( timelines );
  LoadedWindows::getInstance()->getAll( histograms );

  std::unordered_set< Timeline * > selectedTimelines;
  std::unordered_set< Histogram * > selectedHistograms;

  for( const auto &window : windowGenericList )
  {
    if( auto gt = std::get_if< gTimeline * >( &window ) )
    {
      selectedTimelines.insert( ( *gt )->GetMyWindow() );
    }
    else if( auto gh = std::get_if< gHistogram * >( &window ) )
    {
      selectedHistograms.insert( ( *gh )->GetHistogram() );
    }
  }

  // Mostrar solo los Timeline seleccionados
  for( auto *tl : timelines )
  {
    tl->setShowWindow( selectedTimelines.count( tl ) > 0 );
  }

  // Mostrar solo los Histogram seleccionados
  for( auto *hg : histograms )
  {
    hg->setShowWindow( selectedHistograms.count( hg ) > 0 );
  }
}

void gPopUpMenu::OnPopUpShowSelectedWindowsTrace( wxCommandEvent &event )
{
  std::vector< Timeline * > list1;
  std::vector< Timeline * > list2;

  auto visitor = overloads{ []( gHistogram *histogram )
                            {
                              return histogram->GetHistogram()->getDataWindow();
                            },
                            []( gTimeline *timeline )
                            {
                              return timeline->GetMyWindow();
                            }

  };

  auto dataWindow = std::visit( visitor, *windowGenericList.begin() );
  auto trace      = dataWindow->getTrace();

  LoadedWindows::getInstance()->getAll( list1 );
  LoadedWindows::getInstance()->getAll( trace, list2 );

  // Crea un set de punteros de la segunda lista para búsqueda rápida
  std::unordered_set< Timeline * > set2( list2.begin(), list2.end() );

  // Recorre la primera lista
  for( Timeline *tl : list1 )
  {
    if( !set2.count( tl ) )
    {
      tl->setShowWindow( false );
    }
  }

  std::vector< Histogram * > list3;
  std::vector< Histogram * > list4;

  LoadedWindows::getInstance()->getAll( list3 );
  LoadedWindows::getInstance()->getAll( trace, list4 );

  std::unordered_set< Histogram * > set_histo( list4.begin(), list4.end() );

  for( Histogram *histo : list3 )
  {
    if( !set_histo.count( histo ) )
    {
      histo->setShowWindow( false );
    }
  }
}

void gPopUpMenu::OnPopUpHideSelectedWindowsTrace( wxCommandEvent &event )
{
  std::vector< Timeline * > list1;
  std::vector< Timeline * > list2;

  auto visitor = overloads{ []( gHistogram *histogram )
                            {
                              return histogram->GetHistogram()->getDataWindow();
                            },
                            []( gTimeline *timeline )
                            {
                              return timeline->GetMyWindow();
                            }

  };

  auto dataWindow = std::visit( visitor, *windowGenericList.begin() );
  auto trace      = dataWindow->getTrace();

  LoadedWindows::getInstance()->getAll( list1 );
  LoadedWindows::getInstance()->getAll( trace, list2 );

  // Crea un set de punteros de la segunda lista para búsqueda rápida
  std::unordered_set< Timeline * > set2( list2.begin(), list2.end() );

  // Recorre la primera lista
  for( Timeline *tl : list1 )
  {
    if( set2.count( tl ) )
    {
      tl->setShowWindow( false );
    }
  }

  std::vector< Histogram * > list3;
  std::vector< Histogram * > list4;

  LoadedWindows::getInstance()->getAll( list3 );
  LoadedWindows::getInstance()->getAll( trace, list4 );

  std::unordered_set< Histogram * > set_histo( list4.begin(), list4.end() );

  for( Histogram *histo : list3 )
  {
    if( set_histo.count( histo ) )
    {
      histo->setShowWindow( false );
    }
  }
}

// TIMELINE
void gPopUpMenu::OnPopUpFitSemanticScaleMin( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpFitSemanticScaleMin( event );
    },
    []( gHistogram *item )
    {
    } );
}

void gPopUpMenu::OnPopUpFitSemanticScaleMax( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpFitSemanticScaleMax( event );
    },
    []( gHistogram *item )
    {
    } );
}
void gPopUpMenu::OnPopUpFitSemanticScale( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpFitSemanticScale( event );
    },
    []( gHistogram *item )
    {
    } );
}

void gPopUpMenu::OnPopUpViewCommunicationLines( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpViewCommunicationLines( event );
    },
    []( gHistogram *item )
    {
    } );
}

void gPopUpMenu::OnPopUpViewEventFlags( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpViewEventFlags( event );
    },
    []( gHistogram *item )
    {
    } );
}

void gPopUpMenu::OnPopUpFunctionLineColor( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpFunctionLineColor( event );
    },
    []( gHistogram *item )
    {
    } );
}

void gPopUpMenu::OnPopUpFusedLinesColor( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpFusedLinesColor( event );
    },
    []( gHistogram *item )
    {
    } );
}

void gPopUpMenu::OnPopUpPunctualColor( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpPunctualColor( event );
    },
    []( gHistogram *item )
    {
    } );
}

void gPopUpMenu::OnPopUpPunctualColorWindow( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpPunctualColorWindow( event );
    },
    []( gHistogram *item )
    {
    } );
}

void gPopUpMenu::OnPopUpCodeColor( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpCodeColor( event );
    },
    []( gHistogram *item )
    {
    } );
}

void gPopUpMenu::OnPopUpGradientColor( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpGradientColor( event );
    },
    []( gHistogram *item )
    {
    } );
}

void gPopUpMenu::OnPopUpNotNullGradientColor( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpNotNullGradientColor( event );
    },
    []( gHistogram *item )
    {
    } );
}
void gPopUpMenu::OnPopUpAlternativeGradientColor( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpAlternativeGradientColor( event );
    },
    []( gHistogram *item )
    {
    } );
}
void gPopUpMenu::OnPopUpSemanticScaleMinAtZero( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpSemanticScaleMinAtZero( event );
    },
    []( gHistogram *item )
    {
    } );
}

// HISTOGRAM
void gPopUpMenu::OnPopUpRowSelection( wxCommandEvent &event )
{
  createRowSelectionDialog();
}
void gPopUpMenu::OnPopUpAutoControlScale( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gHistogram *item )
    {
      item->OnPopUpAutoControlScale( event );
    },
    []( gTimeline *item )
    {
    } );
}
void gPopUpMenu::OnPopUpAutoControlScaleZero( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gHistogram *item )
    {
      item->OnPopUpAutoControlScaleZero( event );
    },
    []( gTimeline *item )
    {
    } );
}
void gPopUpMenu::OnPopUpAuto3DScale( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gHistogram *item )
    {
      item->OnPopUpAuto3DScale( event );
    },
    []( gTimeline *item )
    {
    } );
}
void gPopUpMenu::OnPopUpAutoDataGradient( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gHistogram *item )
    {
      item->OnPopUpAutoDataGradient( event );
    },
    []( gTimeline *item )
    {
    } );
}

void gPopUpMenu::OnPopUpColor2D( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gHistogram *item )
    {
      item->OnPopUpColor2D( event );
    },
    []( gTimeline *item )
    {
    } );
}

// MIX
void gPopUpMenu::OnPopUpGradientFunction( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpGradientFunction( event );
    } );
}
void gPopUpMenu::OnPopUpUndoZoom( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpUndoZoom( event );
    } );
}
void gPopUpMenu::OnPopUpRedoZoom( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpRedoZoom( event );
    } );
}

// MIX
void gPopUpMenu::OnPopUpDrawModeTimeLast( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpDrawModeTimeLast( event );
    },
    [ &event ]( gHistogram *item )
    {
      item->OnPopUpDrawModeSemanticLast( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeTimeRandom( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpDrawModeTimeRandom( event );
    },
    [ &event ]( gHistogram *item )
    {
      item->OnPopUpDrawModeSemanticRandom( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeTimeRandomNotZero( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpDrawModeTimeRandomNotZero( event );
    },
    [ &event ]( gHistogram *item )
    {
      item->OnPopUpDrawModeSemanticRandomNotZero( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeTimeMaximum( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpDrawModeTimeMaximum( event );
    },
    [ &event ]( gHistogram *item )
    {
      item->OnPopUpDrawModeSemanticMaximum( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeTimeMinimumNotZero( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpDrawModeTimeMinimumNotZero( event );
    },
    [ &event ]( gHistogram *item )
    {
      item->OnPopUpDrawModeSemanticMinimumNotZero( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeTimeAbsoluteMaximum( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpDrawModeTimeAbsoluteMaximum( event );
    },
    [ &event ]( gHistogram *item )
    {
      item->OnPopUpDrawModeSemanticAbsoluteMaximum( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeTimeAbsoluteMinimumNotZero( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpDrawModeTimeAbsoluteMinimumNotZero( event );
    },
    [ &event ]( gHistogram *item )
    {
      item->OnPopUpDrawModeSemanticAbsoluteMinimumNotZero( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeTimeAverage( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpDrawModeTimeAverage( event );
    },
    [ &event ]( gHistogram *item )
    {
      item->OnPopUpDrawModeSemanticAverage( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeTimeAverageNotZero( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpDrawModeTimeAverageNotZero( event );
    },
    [ &event ]( gHistogram *item )
    {
      item->OnPopUpDrawModeSemanticAverageNotZero( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeTimeMode( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpDrawModeTimeMode( event );
    },
    [ &event ]( gHistogram *item )
    {
      item->OnPopUpDrawModeSemanticMode( event );
    } );
}

// MIX
void gPopUpMenu::OnPopUpDrawModeObjectsLast( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpDrawModeObjectsLast( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeObjectsRandom( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpDrawModeObjectsRandom( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeObjectsRandomNotZero( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpDrawModeObjectsRandomNotZero( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeObjectsMaximum( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpDrawModeObjectsMaximum( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeObjectsMinimumNotZero( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpDrawModeObjectsMinimumNotZero( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeObjectsAbsoluteMaximum( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpDrawModeObjectsAbsoluteMaximum( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeObjectsAbsoluteMinimumNotZero( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpDrawModeObjectsAbsoluteMinimumNotZero( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeObjectsAverage( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpDrawModeObjectsAverage( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeObjectsAverageNotZero( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpDrawModeObjectsAverageNotZero( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeObjectsMode( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpDrawModeObjectsMode( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeBothLast( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpDrawModeBothLast( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeBothRandom( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpDrawModeBothRandom( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeBothRandomNotZero( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpDrawModeBothRandomNotZero( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeBothMaximum( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpDrawModeBothMaximum( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeBothMinimumNotZero( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpDrawModeBothMinimumNotZero( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeBothAbsoluteMaximum( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpDrawModeBothAbsoluteMaximum( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeBothAbsoluteMinimumNotZero( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpDrawModeBothAbsoluteMinimumNotZero( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeBothAverage( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpDrawModeBothAverage( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeBothAverageNotZero( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpDrawModeBothAverageNotZero( event );
    } );
}
void gPopUpMenu::OnPopUpDrawModeBothMode( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpDrawModeBothMode( event );
    } );
}

void gPopUpMenu::OnPopUpPixelSize( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpPixelSize( event );
    } );
}
void gPopUpMenu::OnPopUpSynchronize( wxCommandEvent &event )
{
  TGroupId group;
  bool isChecked;
  if( event.GetId() == ID_MENU_NEWGROUP )
  {
    group     = SyncWindows::getInstance()->newGroup();
    isChecked = true;
  }
  else
  {
    isChecked = popUpMenuSync->IsChecked( event.GetId() );
    vector< TGroupId > tmpGroups;
    SyncWindows::getInstance()->getGroups( tmpGroups );
    group = tmpGroups[ event.GetId() - ID_MENU_SYNC_GROUP_BASE ];
  }

  onAllWindowsCall(
    [ &group, isChecked ]( auto *item )
    {
      item->OnPopUpSynchronizeById( group, isChecked );
    } );
}

void gPopUpMenu::OnPopUpSynchronizeProperties( wxCommandEvent &event )
{
  TGroupId tmpGroup = event.GetId() - ID_MENU_SYNC_PROPERTIES_GROUP_BASE;
  openSyncSelection( tmpGroup );
}

void gPopUpMenu::OnPopUpRemoveGroup( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpRemoveGroup( event );
    } );
}
void gPopUpMenu::OnPopUpRemoveAllGroups( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpRemoveAllGroups( event );
    } );
}

// Timeline
void gPopUpMenu::OnPopUpLabels( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpLabels( event );
    },
    []( gHistogram *item )
    {
    } );
}
void gPopUpMenu::OnPopUpObjectAxis( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpObjectAxis( event );
    },
    []( gHistogram *item )
    {
    } );
}
void gPopUpMenu::OnPopUpRunApp( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpRunApp( event );
    },
    []( gHistogram *item )
    {
    } );
}

void gPopUpMenu::OnPopUpTiming( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpTiming( event );
    },
    []( gHistogram *item )
    {
    } );
}

void gPopUpMenu::OnPopUpInfoPanel( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpInfoPanel( event );
    },
    []( gHistogram *item )
    {
    } );
}

// Mix

void gPopUpMenu::OnPopUpSaveCFG( wxCommandEvent &event )
{
  if( typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE )
  {
    onAllWindowsCall(
      [ &event ]( auto *item )
      {
        item->OnPopUpSaveCFG( event );
      } );
  }
  else
  {
    vector< Histogram * > histograms;
    vector< Timeline * > windows;

    for( auto &window : windowGenericList )
    {
      if( auto windowItem = std::get_if< gTimeline * >( &( window ) ) )
      {
        windows.push_back( ( *windowItem )->GetMyWindow() );
      }

      if( auto windowItem = std::get_if< gHistogram * >( &( window ) ) )
      {
        histograms.push_back( ( *windowItem )->GetHistogram() );
      }
    }

    // TODO: use the histogram linked properties manager if any
    CFGS4DLinkedPropertiesManager dummyManager;
    vector< CFGS4DLinkedPropertiesManager > dummyList;
    dummyList.push_back( dummyManager );

    SaveOptions options;

    paraverMain::myParaverMain->SaveConfigurationFile( nullptr, options, windows, histograms, dummyList );
  }
}

void gPopUpMenu::OnPopUpSaveImageDialog( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( auto *item )
    {
      item->OnPopUpSaveImageDialog( event );
    } );
}

// Timeline
void gPopUpMenu::OnPopUpSaveText( wxCommandEvent &event )
{
  onAllWindowsCall(
    [ &event ]( gTimeline *item )
    {
      item->OnPopUpSaveText( event );
    },
    [ &event ]( gHistogram *item )
    {
      item->OnPopUpSavePlaneAsText( event );
    } );
}

template< typename... Funcs >
void gPopUpMenu::onAllWindowsCall( Funcs &&...funcs )
{
  auto visitor = overloads{ std::forward< Funcs >( funcs )... };

  for( auto &window : windowGenericList )
  {
    std::visit( visitor, window );
  }
}
