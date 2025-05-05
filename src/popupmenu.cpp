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

#include <cmath>
#include <iostream>
#include <optional>
#include <wx/event.h>
#include <wx/msgdlg.h>

#include "popupmenu.h"

#include "externalapps.h"
#include "labelconstructor.h"
#include "paraverlabels.h"
#include "paravermain.h"
#include "sequencedriver.h"
#include "syncwindows.h"
#include "window.h"

using namespace std;

#define INIT_WXMENU(name) name = new wxMenu

/* Constructor of gPopUpMenu initialize menu variables and build them. There are three different
types of menus that will change the available options */
gPopUpMenu::gPopUpMenu (std::vector<gHistogram *> wichHistogramDerivedList, std::vector<gTimeline *> wichTimelineDerivedList)
{

  histogramDerivedList = wichHistogramDerivedList;
  timelineDerivedList = wichTimelineDerivedList;

  typeDataPopup = PopUpMenuType::POPUP_MENU_INI;

  if (!histogramDerivedList.empty ())
  {
    typeDataPopup = (histogramDerivedList.size () > 1) ? PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_MULTIPLE : PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE;
  }

  if (!timelineDerivedList.empty ())
  {
    if (typeDataPopup != PopUpMenuType::POPUP_MENU_INI)
    {

      typeDataPopup = PopUpMenuType::POPUP_MENU_TYPE_MIXED;
    }
    else
    {
      typeDataPopup = (timelineDerivedList.size () > 1) ? PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE : PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE;
    }
  }
}
gPopUpMenu::gPopUpMenu (std::vector<gHistogram *> wichHistogramDerivedList)
{

  histogramDerivedList = wichHistogramDerivedList;

  typeDataPopup = PopUpMenuType::POPUP_MENU_INI;

  if (!histogramDerivedList.empty ())
  {
    typeDataPopup = (histogramDerivedList.size () > 1) ? PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_MULTIPLE : PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE;
  }
}

gPopUpMenu::gPopUpMenu (std::vector<gTimeline *> wichTimelineDerivedList)
{

  timelineDerivedList = wichTimelineDerivedList;

  typeDataPopup = PopUpMenuType::POPUP_MENU_INI;

  if (!timelineDerivedList.empty ())
  {
    typeDataPopup = (timelineDerivedList.size () > 1) ? PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE : PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE;
  }
}

void gPopUpMenu::initializePopUpMenu ()
{

  INIT_WXMENU (popUpMenuView);
  INIT_WXMENU (popUpMenuColor);
  INIT_WXMENU (popUpMenuPaste);
  INIT_WXMENU (popUpMenuPasteFilter);
  INIT_WXMENU (popUpMenuFitSemantic);
  INIT_WXMENU (popUpMenuDrawMode);
  INIT_WXMENU (popUpMenuDrawModeTime);
  INIT_WXMENU (popUpMenuDrawModeObjects);
  INIT_WXMENU (popUpMenuDrawModeBoth);
  INIT_WXMENU (popUpMenuPixelSize);
  INIT_WXMENU (popUpMenuGradientFunction);
  INIT_WXMENU (popUpMenuLabels);
  INIT_WXMENU (popUpMenuObjectAxis);
  INIT_WXMENU (popUpMenuSave);
  INIT_WXMENU (popUpMenuRun);
  INIT_WXMENU (popUpMenuSync);
  INIT_WXMENU (popUpMenuColor2D);
  INIT_WXMENU (popUpMenuSyncRemove);

  if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE)
  {
#ifdef __WXMAC__
    this->buildItem (this, _ (STR_COPY), wxITEM_NORMAL, &gPopUpMenu::OnPopUpCopy,
                     ID_MENU_COPY);
#else
    this->buildItem (this, _ (STR_COPY), wxITEM_NORMAL, &gPopUpMenu::OnPopUpCopy,
                     wxID_COPY);
#endif
    this->buildItem (this, _ (STR_RENAME), wxITEM_NORMAL, &gPopUpMenu::OnPopUpRename,
                     ID_MENU_RENAME);
  }

  // Build all the components of the windows. Each function will check if is available in the selected windows

  this->buildPopUpMenuPaste ();

  this->buildItem (this, _ (STR_CLONE), wxITEM_NORMAL, &gPopUpMenu::OnPopUpClone,
                   ID_MENU_CLONE);

  this->AppendSeparator ();

  this->buildPopUpMenuZoom ();

  this->buildPopUpMenuFitSemantic ();

  this->buildPopUpMenuFitObjects ();

  this->buildPopUpMenuDimensionsConfiguration ();

  this->buildPopUpMenuColor ();

  this->buildPopUpMenuGradientFunction ();

  this->buildPopUpMenuView ();

  this->buildPopUpMenuDrawMode ();

  this->buildPopUpMenuPixelSize ();

  this->buildPopUpMenuLabels ();

  this->buildPopUpMenuObjectAxis ();

  this->AppendSeparator ();

  this->buildPopUpMenuRun ();

  this->AppendSeparator ();

  this->buildPopUpMenuSync ();

  this->AppendSeparator ();

  this->buildPopUpMenuSave ();

  this->AppendSeparator ();

  this->buildPopUpExtraPanel ();

  this->buildPopUpRunApp ();
}

/*************************************************************/
/****BUILD FUNCTIONS FOR DIFFERENT ITEMS OF THE MAIN MENU*****/
/*************************************************************/

void gPopUpMenu ::buildPopUpMenuZoom ()
{
  if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE)
  {
    std::vector<BuildMenuItem> zoomItems = {
        {this, _ (STR_UNDO_ZOOM), wxITEM_NORMAL, &gPopUpMenu::OnPopUpUndoZoom, wxID_UNDO},
        {this, _ (STR_REDO_ZOOM), wxITEM_NORMAL, &gPopUpMenu::OnPopUpRedoZoom, wxID_REDO}};

    buildListOfItems (zoomItems);
  }
}
void gPopUpMenu ::buildListOfItems (std::vector<BuildMenuItem> &itemList)
{

  for (const auto &item : itemList)
  {
    this->buildItem (item.menuBaseItem, item.labelStringItem, item.kindItem,
                     item.functionHandler, item.idItem, item.customConditionItem);

    if (item.itemHasSeparator)
    {
      item.menuBaseItem->AppendSeparator ();
      continue;
    }
  }
}

void gPopUpMenu ::buildPopUpMenuView ()
{

  if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE)
  {
    std::vector<BuildMenuItem> viewItems = {
        {popUpMenuView, _ ("Communication Lines"), wxITEM_CHECK, &gPopUpMenu::OnPopUpViewCommunicationLines, ID_MENU_VIEW_COMMUNICATION_LINES,
         allWindowsHas (timelineDerivedList, [] (gTimeline *win)
                        { return win->GetMyWindow ()->getDrawCommLines (); })},

        {popUpMenuView, _ ("Event Flags"), wxITEM_CHECK, &gPopUpMenu::OnPopUpViewEventFlags, ID_MENU_VIEW_EVENT_FLAGS,
         allWindowsHas (timelineDerivedList, [] (gTimeline *win)
                        { return win->GetMyWindow ()->getDrawFlags (); })}};

    buildListOfItems (viewItems);
  }
}

void gPopUpMenu ::buildPopUpMenuPaste ()
{
  std::vector<BuildMenuItem> pasteItemsBasicCommon = {
      {popUpMenuPaste, _ (STR_PASTE_DEFAULT_SPECIAL), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteDefaultSpecial, ID_MENU_PASTE_DEFAULT_SPECIAL, true, true},

      {popUpMenuPaste, _ (STR_TIME), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteTime, ID_MENU_PASTE_TIME},

      {popUpMenuPaste, _ (STR_OBJECTS), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteObjects, ID_MENU_PASTE_OBJECTS},

      {popUpMenuPaste, _ (STR_SIZE), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteSize, ID_MENU_PASTE_SIZE},

      {popUpMenuPaste, _ (STR_DURATION), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteDuration, ID_MENU_PASTE_DURATION},

      {popUpMenuPaste, _ (STR_SEMANTIC_SCALE), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteSemanticScale, ID_MENU_PASTE_SEMANTIC_SCALE},
  };

  std::vector<BuildMenuItem> pasteItemsBasicHistogram = {
      {popUpMenuPaste, _ (STR_CONTROL_SCALE), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteControlScale, ID_MENU_PASTE_CONTROL_SCALE},

      {popUpMenuPaste, _ (STR_PASTE_SEMANTIC_SORT), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteSemanticSort, ID_MENU_PASTE_SEMANTIC_SORT},

      {popUpMenuPaste, _ (STR_CONTROL_DIMENSIONS), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteControlDimensions, ID_MENU_PASTE_CONTROL_DIMENSIONS},

      {popUpMenuPaste, _ (STR_3D_SCALE), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPaste3DScale, ID_MENU_PASTE_3D_SCALE},
  };

  std::vector<BuildMenuItem> pasteItemsExtraCommon = {
      {popUpMenuPaste, _ (STR_PASTE_SPECIAL), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteSpecial, ID_MENU_PASTE_SPECIAL},

      {popUpMenuPaste, _ (STR_CUSTOM_PALETTE), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteCustomPalette, ID_MENU_PASTE_CUSTOM_PALETTE},
  };

  std::vector<BuildMenuItem>
      pasteItemsFilterSubmenu = {
          {popUpMenuPasteFilter, _ (STR_FILTER_ALL), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteFilterAll, ID_MENU_PASTE_FILTER_ALL},

          {popUpMenuPasteFilter, _ (STR_FILTER_COMMS), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteFilterCommunications, ID_MENU_PASTE_FILTER_COMMS},

          {popUpMenuPasteFilter, _ (STR_FILTER_EVENTS), wxITEM_NORMAL, &gPopUpMenu::OnPopUpPasteFilterEvents, ID_MENU_PASTE_FILTER_EVENTS},
      };

  buildListOfItems (pasteItemsBasicCommon);

  if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_MULTIPLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE)
  {
    buildListOfItems (pasteItemsBasicHistogram);
  }

  buildListOfItems (pasteItemsExtraCommon);

  if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE)
  {
    buildListOfItems (pasteItemsFilterSubmenu);
    popUpMenuPaste->AppendSubMenu (popUpMenuPasteFilter, _ (STR_FILTER));

  }

  this->AppendSubMenu (popUpMenuPaste, _ (STR_PASTE));
}

void gPopUpMenu ::buildPopUpMenuColor ()
{
  std::vector<BuildMenuItem> popUpColorTimelineItems = {
      {popUpMenuColor, _ ("Function Line"), wxITEM_CHECK, &gPopUpMenu::OnPopUpFunctionLineColor, ID_MENU_VIEW_FUNCTION_LINE,
       allWindowsHas (timelineDerivedList, [] (gTimeline *win)
                      { return win->GetMyWindow ()->isFunctionLineColorSet (); })},

      {popUpMenuColor, _ ("Fused Lines"), wxITEM_CHECK, &gPopUpMenu::OnPopUpFusedLinesColor, ID_MENU_VIEW_FUSED_LINES,
       allWindowsHas (timelineDerivedList, [] (gTimeline *win)
                      { return win->GetMyWindow ()->isFusedLinesColorSet (); })},

      {popUpMenuColor, _ ("Punctual"), wxITEM_CHECK, &gPopUpMenu::OnPopUpPunctualColor, ID_MENU_PUNCTUAL,
       allWindowsHas (timelineDerivedList, [] (gTimeline *win)
                      { return win->GetMyWindow ()->isPunctualColorSet (); })},
  };

  std::vector<BuildMenuItem> popUpColorCommonItems = {
      {popUpMenuColor, _ ("Code Color"), wxITEM_CHECK, &gPopUpMenu::OnPopUpCodeColor, ID_MENU_CODE_COLOR,
       allWindowsHas (timelineDerivedList, [] (gTimeline *win)
                      { return win->GetMyWindow ()->isCodeColorSet (); })},

      {popUpMenuColor, _ ("Gradient Color"), wxITEM_CHECK, &gPopUpMenu::OnPopUpGradientColor, ID_MENU_GRADIENT_COLOR,
       allWindowsHas (timelineDerivedList, [] (gTimeline *win)
                      { return win->GetMyWindow ()->isGradientColorSet (); })},

      {popUpMenuColor, _ ("Not Null Gradient Color"), wxITEM_CHECK, &gPopUpMenu::OnPopUpNotNullGradientColor, ID_MENU_NOT_NULL_GRADIENT_COLOR,
       allWindowsHas (timelineDerivedList, [] (gTimeline *win)
                      { return win->GetMyWindow ()->isNotNullGradientColorSet (); })},

      {popUpMenuColor, _ ("Alternative Gradient Color"), wxITEM_CHECK, &gPopUpMenu::OnPopUpAlternativeGradientColor, ID_MENU_ALTERNATIVE_GRADIENT_COLOR,
       allWindowsHas (timelineDerivedList, [] (gTimeline *win)
                      { return win->GetMyWindow ()->isAlternativeGradientColorSet (); }),
       true},
  };

  if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE)
  {
    buildListOfItems (popUpColorTimelineItems);
  }

  buildListOfItems (popUpColorCommonItems);
}

void gPopUpMenu ::buildPopUpMenuDimensionsConfiguration ()
{
  if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_MULTIPLE)
  {

    auto generalCodeColor = (*histogramDerivedList.begin ())->GetHistogram ()->getColorMode ();
    auto colorSyncronized = true;

    for (gHistogram *histogram : histogramDerivedList)
    {
      if (!(generalCodeColor == histogram->GetHistogram ()->getColorMode ()))
      {
        colorSyncronized = false;
        break;
      }
    }
    std::vector<BuildMenuItem> color2DItems = {
        {popUpMenuColor2D, _ ("Code Color"), wxITEM_CHECK, &gPopUpMenu::OnPopUpColor2D, ID_MENU_CODE_COLOR_2D,
         (generalCodeColor == TColorFunction::CODE_COLOR) && colorSyncronized},

        {popUpMenuColor2D, _ ("Gradient Color"), wxITEM_CHECK, &gPopUpMenu::OnPopUpColor2D, ID_MENU_GRADIENT_COLOR_2D,
         (generalCodeColor == TColorFunction::GRADIENT) && colorSyncronized},

        {popUpMenuColor2D, _ ("Not Null Gradient Color"), wxITEM_CHECK, &gPopUpMenu::OnPopUpColor2D, ID_MENU_NOT_NULL_GRADIENT_COLOR_2D,
         (generalCodeColor == TColorFunction::NOT_NULL_GRADIENT) && colorSyncronized},

        {popUpMenuColor2D, _ ("Alternative Gradient Color"), wxITEM_CHECK, &gPopUpMenu::OnPopUpColor2D, ID_MENU_ALTERNATIVE_GRADIENT_COLOR_2D,
         (generalCodeColor == TColorFunction::ALTERNATIVE_GRADIENT) && colorSyncronized}};

    buildListOfItems (color2DItems);

    AppendSubMenu (popUpMenuColor2D, _ ("Color Mode"));
  }
}

void gPopUpMenu ::buildPopUpMenuFitSemantic ()
{
  if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE)
  {
    std::vector<BuildMenuItem> fitSemanticItems = {
        {popUpMenuFitSemantic, _ ("Fit Minimum"), wxITEM_NORMAL, &gPopUpMenu::OnPopUpFitSemanticScaleMin, ID_MENU_FIT_SEMANTIC_MIN},

        {popUpMenuFitSemantic, _ ("Fit Maximum"), wxITEM_NORMAL, &gPopUpMenu::OnPopUpFitSemanticScaleMax, ID_MENU_FIT_SEMANTIC_MAX},

        {popUpMenuFitSemantic, _ ("Fit Both"), wxITEM_NORMAL, &gPopUpMenu::OnPopUpFitSemanticScale, ID_MENU_FIT_SEMANTIC_BOTH}};

    buildListOfItems (fitSemanticItems);

    AppendSubMenu (popUpMenuFitSemantic, _ (STR_FIT_SEMANTIC));
  }
}

void gPopUpMenu ::buildPopUpMenuFitObjects ()
{

  std::vector<BuildMenuItem> fitItemsCommon = {
      {this, _ (STR_FIT_TIME), wxITEM_NORMAL, &gPopUpMenu::OnPopUpFitTimeScale, wxID_ZOOM_100},

      {this, _ (STR_FIT_OBJECTS), wxITEM_NORMAL, &gPopUpMenu::OnPopUpFitObjects, ID_MENU_FIT_OBJECTS}};

  buildListOfItems (fitItemsCommon);

  this->buildItem (this, _ ("Select Objects..."), wxITEM_NORMAL,
                   &gPopUpMenu::OnPopUpRowSelection, ID_MENU_ROW_SELECTION);

  if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_MULTIPLE)
  {
    std::vector<BuildMenuItem> fitItemsHistogram = {
        {this, _ ("Auto Fit Control Scale"), wxITEM_CHECK, &gPopUpMenu::OnPopUpAutoControlScale, ID_MENU_AUTO_CONTROL_SCALE,
         allWindowsHas (histogramDerivedList, static_cast<std::function<bool (gHistogram *)>> ([] (gHistogram *win)
                                                                                               { return win->GetHistogram ()->getCompute2DScale (); }))},

        {this, _ (STR_AUTOFIT_CONTROL_ZERO), wxITEM_CHECK, &gPopUpMenu::OnPopUpAutoControlScaleZero, ID_MENU_AUTO_CONTROL_SCALE_ZERO,
         allWindowsHas (histogramDerivedList, static_cast<std::function<bool (gHistogram *)>> ([] (gHistogram *win)
                                                                                               { return win->GetHistogram ()->getCompute2DScaleZero (); }))},

        {this, _ ("Auto Fit Data Gradient"), wxITEM_CHECK, &gPopUpMenu::OnPopUpAutoDataGradient, ID_MENU_AUTO_DATA_GRADIENT,
         allWindowsHas (histogramDerivedList, static_cast<std::function<bool (gHistogram *)>> ([] (gHistogram *win)
                                                                                               { return win->GetHistogram ()->getComputeGradient (); }))}};

    buildListOfItems (fitItemsHistogram);

    if (allWindowsHas (histogramDerivedList, [] (gHistogram *win)
                       { return win->GetHistogram ()->getThreeDimensions (); }))
    {
      this->buildItem (this, _ ("Auto Fit 3D Scale"), wxITEM_CHECK, &gPopUpMenu::OnPopUpAuto3DScale, ID_MENU_AUTO_3D_SCALE,
                       allWindowsHas (histogramDerivedList, static_cast<std::function<bool (gHistogram *)>> ([] (gHistogram *win)
                                                                                                             { return win->GetHistogram ()->getCompute3DScale (); })));
    }
  }

  this->AppendSeparator ();
}

void gPopUpMenu ::buildPopUpMenuDrawMode ()
{
  if (typeDataPopup != PopUpMenuType::POPUP_MENU_TYPE_MIXED)
  {
    DrawModeMethod drawModeTime;
    DrawModeMethod drawModeObject;
    auto drawModeTimeSync = true;
    auto drawModeObjectSync = true;

    if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE)
    {

      drawModeTime = (*timelineDerivedList.begin ())->GetMyWindow ()->getDrawModeTime ();
      drawModeObject = (*timelineDerivedList.begin ())->GetMyWindow ()->getDrawModeObject ();

      for (gTimeline *timeline : timelineDerivedList)
      {
        if (!(drawModeTime == timeline->GetMyWindow ()->getDrawModeTime ()))
        {
          drawModeTimeSync = false;
        }
        if (!(drawModeObject == timeline->GetMyWindow ()->getDrawModeObject ()))
        {
          drawModeObjectSync = false;
        }
      }
    }

    if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_MULTIPLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE)
    {

      drawModeTime = (*histogramDerivedList.begin ())->GetHistogram ()->getDrawModeColumns ();
      drawModeObject = (*histogramDerivedList.begin ())->GetHistogram ()->getDrawModeObjects ();

      for (gHistogram *histogram : histogramDerivedList)
      {
        if (!(drawModeTime == histogram->GetHistogram ()->getDrawModeColumns ()))
        {
          drawModeTimeSync = false;
          break;
        }
        if (!(drawModeObject == histogram->GetHistogram ()->getDrawModeObjects ()))
        {
          drawModeObjectSync = false;
          break;
        }
      }
    }
    std::vector<BuildMenuItem> drawModeTimeItems = {
        {popUpMenuDrawModeTime, _ (GUI_DRAWMODE_LAST), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeTimeLast, ID_MENU_DRAWMODE_TIME_LAST, (drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_LAST)},

        {popUpMenuDrawModeTime, _ (GUI_DRAWMODE_MAXIMUM), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeTimeMaximum, ID_MENU_DRAWMODE_TIME_MAXIMUM, (drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_MAXIMUM)},

        {popUpMenuDrawModeTime, _ (GUI_DRAWMODE_ABSOLUTE_MAXIMUM), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeTimeAbsoluteMaximum, ID_MENU_DRAWMODE_TIME_ABSOLUTE_MAXIMUM, (drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM)},

        {popUpMenuDrawModeTime, _ (GUI_DRAWMODE_MINIMUM_NOT_ZERO), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeTimeMinimumNotZero, ID_MENU_DRAWMODE_TIME_MINIMUM_NOT_ZERO, (drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_MINNOTZERO)},

        {popUpMenuDrawModeTime, _ (GUI_DRAWMODE_ABSOLUTE_MINIMUM_NOT_ZERO), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeTimeAbsoluteMinimumNotZero, ID_MENU_DRAWMODE_TIME_ABSOLUTE_MINIMUM_NOT_ZERO, (drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO)},

        {popUpMenuDrawModeTime, _ (GUI_DRAWMODE_RANDOM), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeTimeRandom, ID_MENU_DRAWMODE_TIME_RANDOM, drawModeTime == DrawModeMethod::DRAW_RANDOM},

        {popUpMenuDrawModeTime, _ (GUI_DRAWMODE_RANDOM_NOT_ZERO), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeTimeRandomNotZero, ID_MENU_DRAWMODE_TIME_RANDOM_NOT_ZERO, (drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_RANDNOTZERO)},

        {popUpMenuDrawModeTime, _ (GUI_DRAWMODE_AVERAGE), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeTimeAverage, ID_MENU_DRAWMODE_TIME_AVERAGE, (drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_AVERAGE)},

        {popUpMenuDrawModeTime, _ (GUI_DRAWMODE_AVERAGE_NOT_ZERO), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeTimeAverageNotZero, ID_MENU_DRAWMODE_TIME_AVERAGE_NOT_ZERO, (drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_AVERAGENOTZERO)},

        {popUpMenuDrawModeTime, _ (GUI_DRAWMODE_MODE), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeTimeMode, ID_MENU_DRAWMODE_TIME_MODE, (drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_MODE)}};

    std::vector<BuildMenuItem> drawModeObjectsItems = {
        {popUpMenuDrawModeObjects, _ (GUI_DRAWMODE_LAST), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeObjectsLast, ID_MENU_DRAWMODE_OBJECTS_LAST, (drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_LAST)},

        {popUpMenuDrawModeObjects, _ (GUI_DRAWMODE_MAXIMUM), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeObjectsMaximum, ID_MENU_DRAWMODE_OBJECTS_MAXIMUM, (drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_MAXIMUM)},

        {popUpMenuDrawModeObjects, _ (GUI_DRAWMODE_ABSOLUTE_MAXIMUM), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeObjectsAbsoluteMaximum, ID_MENU_DRAWMODE_OBJECTS_ABSOLUTE_MAXIMUM, (drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM)},

        {popUpMenuDrawModeObjects, _ (GUI_DRAWMODE_MINIMUM_NOT_ZERO), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeObjectsMinimumNotZero, ID_MENU_DRAWMODE_OBJECTS_MINIMUM_NOT_ZERO, (drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_MINNOTZERO)},

        {popUpMenuDrawModeObjects, _ (GUI_DRAWMODE_ABSOLUTE_MINIMUM_NOT_ZERO), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeObjectsAbsoluteMinimumNotZero, ID_MENU_DRAWMODE_OBJECTS_ABSOLUTE_MINIMUM_NOT_ZERO, (drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO)},

        {popUpMenuDrawModeObjects, _ (GUI_DRAWMODE_RANDOM), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeObjectsRandom, ID_MENU_DRAWMODE_OBJECTS_RANDOM, (drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_RANDOM)},

        {popUpMenuDrawModeObjects, _ (GUI_DRAWMODE_RANDOM_NOT_ZERO), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeObjectsRandomNotZero, ID_MENU_DRAWMODE_OBJECTS_RANDOM_NOT_ZERO, (drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_RANDNOTZERO)},

        {popUpMenuDrawModeObjects, _ (GUI_DRAWMODE_AVERAGE), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeObjectsAverage, ID_MENU_DRAWMODE_OBJECTS_AVERAGE, (drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_AVERAGE)},

        {popUpMenuDrawModeObjects, _ (GUI_DRAWMODE_AVERAGE_NOT_ZERO), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeObjectsAverageNotZero, ID_MENU_DRAWMODE_OBJECTS_AVERAGE_NOT_ZERO, (drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_AVERAGENOTZERO)},

        {popUpMenuDrawModeObjects, _ (GUI_DRAWMODE_MODE), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeObjectsMode, ID_MENU_DRAWMODE_OBJECTS_MODE, (drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_MODE)}};

    std::vector<BuildMenuItem> drawModeBothItems = {
        {popUpMenuDrawModeBoth, _ (GUI_DRAWMODE_LAST), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeBothLast, ID_MENU_DRAWMODE_BOTH_LAST,
         (drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_LAST) || (drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_LAST)},

        {popUpMenuDrawModeBoth, _ (GUI_DRAWMODE_MAXIMUM), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeBothMaximum, ID_MENU_DRAWMODE_BOTH_MAXIMUM,
         (drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_LAST) || (drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_MAXIMUM)},

        {popUpMenuDrawModeBoth, _ (GUI_DRAWMODE_ABSOLUTE_MAXIMUM), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeBothAbsoluteMaximum, ID_MENU_DRAWMODE_BOTH_ABSOLUTE_MAXIMUM,
         (drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_LAST) || (drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM)},

        {popUpMenuDrawModeBoth, _ (GUI_DRAWMODE_MINIMUM_NOT_ZERO), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeBothMinimumNotZero, ID_MENU_DRAWMODE_BOTH_MINIMUM_NOT_ZERO,
         (drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_LAST) || (drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_MINNOTZERO)},

        {popUpMenuDrawModeBoth, _ (GUI_DRAWMODE_ABSOLUTE_MINIMUM_NOT_ZERO), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeBothRandomNotZero, ID_MENU_DRAWMODE_BOTH_RANDOM_NOT_ZERO,
         (drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_LAST) || (drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_RANDNOTZERO)},

        {popUpMenuDrawModeBoth, _ (GUI_DRAWMODE_AVERAGE), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeBothAverage, ID_MENU_DRAWMODE_BOTH_AVERAGE,
         (drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_LAST) || (drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_AVERAGE)},

        {popUpMenuDrawModeBoth, _ (GUI_DRAWMODE_AVERAGE_NOT_ZERO), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeBothAverageNotZero, ID_MENU_DRAWMODE_BOTH_AVERAGE_NOT_ZERO,
         (drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_LAST) || (drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_AVERAGENOTZERO)},

        {popUpMenuDrawModeBoth, _ (GUI_DRAWMODE_MODE), wxITEM_CHECK, &gPopUpMenu::OnPopUpDrawModeBothMode, ID_MENU_DRAWMODE_BOTH_MODE,
         (drawModeObjectSync && drawModeObject == DrawModeMethod::DRAW_LAST) || (drawModeTimeSync && drawModeTime == DrawModeMethod::DRAW_MODE)}};

    buildListOfItems (drawModeTimeItems);

    buildListOfItems (drawModeObjectsItems);

    buildListOfItems (drawModeBothItems);

    if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_MULTIPLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE)
    {
      popUpMenuDrawMode->AppendSubMenu (popUpMenuDrawModeTime, _ ("Semantic"));
      popUpMenuDrawMode->AppendSubMenu (popUpMenuDrawModeObjects, _ ("Objects"));
      popUpMenuDrawMode->AppendSubMenu (popUpMenuDrawModeBoth, _ ("Both"));
    }
    if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE)
    {
      popUpMenuDrawMode->AppendSubMenu (popUpMenuDrawModeTime, _ ("Time"));
      popUpMenuDrawMode->AppendSubMenu (popUpMenuDrawModeObjects, _ ("Objects"));
      popUpMenuDrawMode->AppendSubMenu (popUpMenuDrawModeBoth, _ ("Both"));
    }

    wxMenuItem *tmpDrawModeSubMenu =
        AppendSubMenu (popUpMenuDrawMode, _ ("Drawmode"));

    if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE)
    {
      Enable (tmpDrawModeSubMenu->GetId (),
              (allWindowsHas (timelineDerivedList, [] (gTimeline *window)
                              { return ((!window->GetMyWindow ()->isPunctualColorSet ()) || (window->GetMyWindow ()->getPunctualColorWindow () != nullptr)); })));
    }
  }
}

void gPopUpMenu::buildPopUpMenuPixelSize ()
{
  bool isSync = true;
  int pixelSize = 0;
  for (gHistogram *histogram : histogramDerivedList)
  {
    if (pixelSize == 0)
    {
      pixelSize = histogram->GetHistogram ()->getPixelSize ();
    }
    else if (histogram->GetHistogram ()->getPixelSize () != pixelSize)
    {
      isSync = false;
      break;
    }
  }
  if (isSync == true)
  {
    for (gTimeline *timeline : timelineDerivedList)
    {
      if (pixelSize == 0)
      {
        pixelSize = timeline->GetMyWindow ()->getPixelSize ();
      }
      else if (timeline->GetMyWindow ()->getPixelSize () != pixelSize)
      {
        isSync = false;
        break;
      }
    }
  }

  std::vector<BuildMenuItem> pixelSizeItems = {
      {popUpMenuPixelSize, _ ("x1"), wxITEM_CHECK, &gPopUpMenu::OnPopUpPixelSize, ID_MENU_PIXEL_SIZE_x1, (isSync && pixelSize == 1)},

      {popUpMenuPixelSize, _ ("x2"), wxITEM_CHECK, &gPopUpMenu::OnPopUpPixelSize, ID_MENU_PIXEL_SIZE_x2, (isSync && pixelSize == 2)},

      {popUpMenuPixelSize, _ ("x4"), wxITEM_CHECK, &gPopUpMenu::OnPopUpPixelSize, ID_MENU_PIXEL_SIZE_x4, (isSync && pixelSize == 4)},

      {popUpMenuPixelSize, _ ("x8"), wxITEM_CHECK, &gPopUpMenu::OnPopUpPixelSize, ID_MENU_PIXEL_SIZE_x8, (isSync && pixelSize == 8)}};

  buildListOfItems (pixelSizeItems);

  AppendSubMenu (popUpMenuPixelSize, _ ("Pixel Size"));
}

void gPopUpMenu ::buildPopUpMenuGradientFunction ()
{
  if (typeDataPopup != PopUpMenuType::POPUP_MENU_TYPE_MIXED)
  {

    TGradientFunction gradientFunctionSelected = TGradientFunction::LINEAR;

    bool valueSynchronyzed = true;
    bool initialValue = true;

    auto checkGradientFunction = [&] (TGradientFunction timelineFunction) -> bool
    {
      if (initialValue)
      {
        initialValue = false;
        gradientFunctionSelected = timelineFunction;
        return true;
      }
      return gradientFunctionSelected == timelineFunction;
    };

    for (gTimeline *timeline : timelineDerivedList)
    {
      if (!checkGradientFunction (timeline->GetMyWindow ()->getSemanticColor ().getGradientFunction ()))
      {
        valueSynchronyzed = false;
        break;
      }
    }

    if (valueSynchronyzed)
    {
      for (gHistogram *histogram : histogramDerivedList)
      {
        if (!checkGradientFunction (histogram->GetHistogram ()->getSemanticColor ().getGradientFunction ()))
        {
          valueSynchronyzed = false;
          break;
        }
      }
    }

    std::vector<BuildMenuItem> gradientFunctionItems = {
        {popUpMenuGradientFunction, _ ("Linear"), wxITEM_CHECK, &gPopUpMenu::OnPopUpGradientFunction,
         ID_MENU_GRADIENT_FUNCTION_LINEAR, gradientFunctionSelected == TGradientFunction::LINEAR && (!initialValue && valueSynchronyzed)},

        {popUpMenuGradientFunction, _ ("Steps"), wxITEM_CHECK, &gPopUpMenu::OnPopUpGradientFunction,
         ID_MENU_GRADIENT_FUNCTION_STEPS, gradientFunctionSelected == TGradientFunction::STEPS && (!initialValue && valueSynchronyzed)},

        {popUpMenuGradientFunction, _ ("Logarithmic"), wxITEM_CHECK, &gPopUpMenu::OnPopUpGradientFunction,
         ID_MENU_GRADIENT_FUNCTION_LOGARITHMIC, gradientFunctionSelected == TGradientFunction::LOGARITHMIC && (!initialValue && valueSynchronyzed)},

        {popUpMenuGradientFunction, _ ("Exponential"), wxITEM_CHECK, &gPopUpMenu::OnPopUpGradientFunction,
         ID_MENU_GRADIENT_FUNCTION_EXPONENTIAL, gradientFunctionSelected == TGradientFunction::EXPONENTIAL && (!initialValue && valueSynchronyzed)}};

    buildListOfItems (gradientFunctionItems);

    if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_MULTIPLE)
    {
      this->AppendSubMenu (popUpMenuGradientFunction, _ ("Gradient Function "));
    }
    else if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE)
    {

      wxMenuItem *tmpPuncWin = buildItem (popUpMenuColor, _ ("Punctual Window..."), wxITEM_NORMAL,
                                          &gPopUpMenu::OnPopUpPunctualColorWindow, ID_MENU_PUNCTUAL_WINDOW);

      wxMenuItem *tmpGradFunc = popUpMenuColor->AppendSubMenu (
          popUpMenuGradientFunction, _ ("Gradient Function"));

      wxMenuItem *tmpSemScaleMinZero = buildItem (popUpMenuColor, _ ("Semantic scale min at 0"), wxITEM_CHECK,
                                                  &gPopUpMenu::OnPopUpSemanticScaleMinAtZero,
                                                  ID_MENU_SEMANTIC_SCALE_MIN_AT_ZERO,
                                                  allWindowsHas (timelineDerivedList, [] (gTimeline *window)
                                                                 { return window->GetMyWindow ()->getSemanticScaleMinAtZero (); }));

      this->AppendSubMenu (popUpMenuColor, _ ("Paint As"));
    }
  }
}

void gPopUpMenu ::buildPopUpMenuObjectAxis ()
{

  if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE)
  {
    bool isSync = true;
    TObjectAxisSize objectAxis = (*timelineDerivedList.begin ())->GetMyWindow ()->getObjectAxisSize ();

    for (gTimeline *timeline : timelineDerivedList)
    {
      if (timeline->GetMyWindow ()->getObjectAxisSize () != objectAxis)
      {
        isSync = false;
        break;
      }
    }

    std::vector<BuildMenuItem> objectAxisItems = {
        {popUpMenuObjectAxis, _ ("Fit Current Level"), wxITEM_CHECK, &gPopUpMenu::OnPopUpObjectAxis,
         ID_MENU_OBJECT_AXIS_CURRENT,
         (objectAxis == TObjectAxisSize::CURRENT_LEVEL && isSync)},

        {popUpMenuObjectAxis, _ ("Fit All Levels"), wxITEM_CHECK, &gPopUpMenu::OnPopUpObjectAxis,
         ID_MENU_OBJECT_AXIS_ALL,
         (objectAxis == TObjectAxisSize::ALL_LEVELS && isSync)},

        {popUpMenuObjectAxis, _ ("0%"), wxITEM_CHECK, &gPopUpMenu::OnPopUpObjectAxis,
         ID_MENU_OBJECT_AXIS_ZERO,
         (objectAxis == TObjectAxisSize::ZERO_PERC && isSync)},

        {popUpMenuObjectAxis, _ ("5%"), wxITEM_CHECK, &gPopUpMenu::OnPopUpObjectAxis,
         ID_MENU_OBJECT_AXIS_FIVE,
         (objectAxis == TObjectAxisSize::FIVE_PERC && isSync)},

        {popUpMenuObjectAxis, _ ("25%"), wxITEM_CHECK, &gPopUpMenu::OnPopUpObjectAxis,
         ID_MENU_OBJECT_AXIS_TWENTYFIVE,
         (objectAxis == TObjectAxisSize::TWENTYFIVE_PERC && isSync)},
    };

    buildListOfItems (objectAxisItems);

    AppendSubMenu (popUpMenuObjectAxis, _ ("Object Axis"));
  }
}

void gPopUpMenu ::buildPopUpMenuSave ()
{
  if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE)
  {
    std::vector<BuildMenuItem> saveMenuItems = {
        {popUpMenuSave, _ ("Configuration..."), wxITEM_NORMAL, &gPopUpMenu::OnPopUpSaveCFG, ID_MENU_SAVE_TIMELINE_AS_CFG},

        {popUpMenuSave, _ ("Image..."), wxITEM_NORMAL, &gPopUpMenu::OnPopUpSaveImageDialog, ID_MENU_SAVE_IMAGE},

        {popUpMenuSave, _ ("Text..."), wxITEM_NORMAL, &gPopUpMenu::OnPopUpSaveText, ID_MENU_SAVE_TIMELINE_AS_TEXT}};

    buildListOfItems (saveMenuItems);

    AppendSubMenu (popUpMenuSave, _ ("Save"));
  }
}

void gPopUpMenu ::buildPopUpMenuRun ()
{
}

void gPopUpMenu ::buildPopUpMenuLabels ()
{
  if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE)
  {
    std::vector<BuildMenuItem> objectLabelItems = {
        {popUpMenuLabels, _ ("All"), wxITEM_CHECK, &gPopUpMenu::OnPopUpLabels, ID_MENU_LABELS_ALL,
         allWindowsHas (timelineDerivedList, [] (gTimeline *win)
                        { return win->GetMyWindow ()->getObjectLabels () == TObjectLabels::ALL_LABELS; })},

        {popUpMenuLabels, _ ("Spaced"), wxITEM_CHECK, &gPopUpMenu::OnPopUpLabels, ID_MENU_LABELS_SPACED,
         allWindowsHas (timelineDerivedList, [] (gTimeline *win)
                        { return win->GetMyWindow ()->getObjectLabels () == TObjectLabels::SPACED_LABELS; })},

        {popUpMenuLabels, _ ("2^n"), wxITEM_CHECK, &gPopUpMenu::OnPopUpLabels, ID_MENU_LABELS_POWER2,
         allWindowsHas (timelineDerivedList, [] (gTimeline *win)
                        { return win->GetMyWindow ()->getObjectLabels () == TObjectLabels::POWER2_LABELS; })}};

    buildListOfItems (objectLabelItems);

    this->AppendSubMenu (popUpMenuLabels, _ ("Object Labels"));
  }
}

void gPopUpMenu ::buildPopUpMenuSync ()
{
  std::vector<TGroupId> tmpGroups;
  SyncWindows::getInstance ()->getGroups (tmpGroups);
  TGroupId i = 0;
  int syncValue = -1;
  bool windowsSyncronized = true;

  auto checkSyncronizedGroup = [&] (int group) -> bool
  {
    if (syncValue == -1)
    {
      syncValue = group;
      return true;
    }
    return syncValue == group;
  };

  for (gTimeline *timeline : timelineDerivedList)
  {
    if (timeline->GetMyWindow ()->isSync ())
    {
      if (!checkSyncronizedGroup (timeline->GetMyWindow ()->getSyncGroup ()))
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

  if (windowsSyncronized == true)
  {
    for (gHistogram *histogram : histogramDerivedList)
    {
      if (histogram->GetHistogram ()->isSync ())
      {

        if (!checkSyncronizedGroup (histogram->GetHistogram ()->getSyncGroup ()))
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
  }

  bool checked;

  for (vector<TGroupId>::const_iterator itGroup = tmpGroups.begin ();
       itGroup != tmpGroups.end (); ++itGroup)
  {
    bool checked = (windowsSyncronized && syncValue == *itGroup);
    this->buildItem (popUpMenuSync, wxString::Format (_ ("%u"), *itGroup + 1),
                     wxITEM_CHECK, &gPopUpMenu::OnPopUpSynchronize,
                     ID_MENU_SYNC_GROUP_BASE + i,
                     checked);

    ++i;
  }
  popUpMenuSync->AppendSeparator ();
  this->buildItem (popUpMenuSync, _ (STR_SYNC_NEW_GROUP), wxITEM_NORMAL,
                   &gPopUpMenu::OnPopUpSynchronize, ID_MENU_NEWGROUP);

  i = 0;
  for (std::vector<TGroupId>::const_iterator itGroup = tmpGroups.begin ();
       itGroup != tmpGroups.end (); ++itGroup)
  {
    this->buildItem (popUpMenuSyncRemove,
                     wxString::Format (_ ("%u"), *itGroup + 1), wxITEM_NORMAL,
                     &gPopUpMenu::OnPopUpRemoveGroup,
                     ID_MENU_SYNC_REMOVE_GROUP_BASE + i);
    ++i;
  }
  popUpMenuSyncRemove->AppendSeparator ();
  this->buildItem (popUpMenuSyncRemove, _ (STR_SYNC_REMOVE_ALL_GROUPS),
                   wxITEM_NORMAL, &gPopUpMenu::OnPopUpRemoveAllGroups,
                   ID_MENU_SYNC_REMOVE_ALL_GROUPS);
  popUpMenuSync->AppendSubMenu (popUpMenuSyncRemove,
                                _ (STR_SYNC_REMOVE_GROUP));

  AppendSubMenu (popUpMenuSync, _ (STR_SYNCHRONIZE));
}

void gPopUpMenu::buildPopUpRunApp ()
{
  if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE)
  {
    auto suitableApps = (*timelineDerivedList.begin ())->GetMyWindow ()->getTrace ()->getSuitableApps ();

    this->buildItem (popUpMenuRun, _ ("Cutter"), wxITEM_NORMAL, &gPopUpMenu::OnPopUpRunApp, ID_MENU_CUTTER);

    if (suitableApps[(int)TExternalAppID::DIMEMAS])
      this->buildItem (popUpMenuRun, ExternalApps::getApplicationLabel (TExternalAppID::DIMEMAS), wxITEM_NORMAL, &gPopUpMenu::OnPopUpRunApp, ID_MENU_DIMEMAS);

    if (suitableApps[(int)TExternalAppID::CLUSTERING])
      this->buildItem (popUpMenuRun, ExternalApps::getApplicationLabel (TExternalAppID::CLUSTERING), wxITEM_NORMAL, &gPopUpMenu::OnPopUpRunApp, ID_MENU_CLUSTERING);

    if (suitableApps[(int)TExternalAppID::FOLDING])
      this->buildItem (popUpMenuRun, ExternalApps::getApplicationLabel (TExternalAppID::FOLDING), wxITEM_NORMAL, &gPopUpMenu::OnPopUpRunApp, ID_MENU_FOLDING);

    if (suitableApps[(int)TExternalAppID::PROFET])
      this->buildItem (popUpMenuRun, ExternalApps::getApplicationLabel (TExternalAppID::PROFET), wxITEM_NORMAL, &gPopUpMenu::OnPopUpRunApp, ID_MENU_PROFET);

    this->buildItem (popUpMenuRun, ExternalApps::getApplicationLabel (TExternalAppID::USER_COMMAND), wxITEM_NORMAL, &gPopUpMenu::OnPopUpRunApp, ID_MENU_USER_COMMAND);
    AppendSubMenu (popUpMenuRun, _ ("Run"));
  }
}

void gPopUpMenu::buildPopUpExtraPanel ()
{

  if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE)
  {

    std::vector<BuildMenuItem> viewMenuItems = {
        {this, _ ("Timing\tCTRL+T"), wxITEM_CHECK, &gPopUpMenu::OnPopUpTiming, ID_MENU_TIMING,
         allWindowsHas (timelineDerivedList, [] (gTimeline *win)
                        { return win->GetTiming (); })},

        {this, _ ("Info Panel"), wxITEM_CHECK, &gPopUpMenu::OnPopUpInfoPanel, ID_MENU_INFO_PANEL,
         allWindowsHas (timelineDerivedList, [] (gTimeline *win)
                        { return win->IsSplit (); })}};
    buildListOfItems (viewMenuItems);
  }
}

wxMenuItem *gPopUpMenu::buildItem (
    wxMenu *baseMenuContainer, const wxString &titleMenuItem,
    const wxItemKind &typeMenuItem,
    void (gPopUpMenu::*callbackFunctionMenuItem) (wxCommandEvent &), const wxWindowID &windowId,
    bool isChecked)
{
  wxMenuItem *newMenuItem;

  newMenuItem = new wxMenuItem (baseMenuContainer, windowId, titleMenuItem,
                                _ (""), typeMenuItem);

  baseMenuContainer->Append (newMenuItem);

  if (newMenuItem->IsCheckable ())
  {
    newMenuItem->Check (isChecked);
  }

#ifdef _WIN32
  Bind (wxEVT_COMMAND_MENU_SELECTED, function, window, id);
#else
  baseMenuContainer->Bind (wxEVT_COMMAND_MENU_SELECTED, callbackFunctionMenuItem, this, windowId);

#endif

  return newMenuItem;
}

bool gPopUpMenu::allWindowsHas (vector<gTimeline *> timelineWindows, std::function<bool (gTimeline *)> function)
{
  for (auto *timeline : timelineWindows)
  {
    if (function (timeline) == false)
    {
      return false;
    }
  }
  return true;
}
bool gPopUpMenu::allWindowsHas (vector<gHistogram *> histogramWindows, std::function<bool (gHistogram *)> function)
{
  for (auto *histogram : histogramWindows)
  {
    if (function (histogram) == false)
    {
      return false;
    }
  }
  return true;
}

/*************************************************************/
/*************ENABLE POP UP MENU FUNCTIONALLITIES*************/
/*************************************************************/

bool gPopUpMenu::checkAllowedProperties (const char *property)
{

  bool rv = true;
  gPasteWindowProperties *sharedProperties =
      gPasteWindowProperties::getInstance ();

  for (gTimeline *timeline : timelineDerivedList)
  {
    if (!sharedProperties->isAllowed (timeline, property))
      return false;
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    if (!sharedProperties->isAllowed (histogram, property))
      return false;
  }

  return rv;
}

void gPopUpMenu::enablePopUpMenu ()
{

  auto tryToEnableItem = [&] (wxMenu *menuComponent, char *itemName) -> void
  {
    int itemId = FindItem (_ (itemName));
    if (itemId != wxNOT_FOUND)
    {
      menuComponent->Enable (itemId, true);
    }
  };

  gPasteWindowProperties *sharedProperties =
      gPasteWindowProperties::getInstance ();

  this->Enable (FindItem (_ (STR_PASTE)),
                checkAllowedProperties (STR_PASTE));
  this->Enable (FindItem (_ (STR_PASTE_DEFAULT_SPECIAL)),
                checkAllowedProperties (STR_PASTE_DEFAULT_SPECIAL));
  this->Enable (FindItem (_ (STR_PASTE_SPECIAL)),
                checkAllowedProperties (STR_PASTE_SPECIAL));

  if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_MULTIPLE)
  {

    this->Enable (FindItem (_ (STR_CONTROL_SCALE)),
                  checkAllowedProperties (STR_CONTROL_SCALE));
    this->Enable (FindItem (_ (STR_PASTE_SEMANTIC_SORT)),
                  checkAllowedProperties (STR_PASTE_SEMANTIC_SORT));
    this->Enable (FindItem (_ (STR_CONTROL_DIMENSIONS)),
                  checkAllowedProperties (STR_CONTROL_DIMENSIONS));

    this->Enable (FindItem (_ (STR_AUTOFIT_CONTROL_ZERO)),
                  allWindowsHas (histogramDerivedList, [] (gHistogram *win)
                                 { return win->GetHistogram ()->getCompute2DScale (); }));

    if (allWindowsHas (histogramDerivedList, [] (gHistogram *win)
                       { return win->GetHistogram ()->getThreeDimensions (); }))
    {
      this->Enable (FindItem (_ (STR_3D_SCALE)),
                    checkAllowedProperties (STR_3D_SCALE));
    }
    else
    {
      this->Enable (FindItem (_ (STR_3D_SCALE)), false);
    }
  }
  if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_HISTOGRAM_SINGLE)
  {
    this->Enable (FindItem (_ (STR_REDO_ZOOM)),
                  !(*histogramDerivedList.begin ())->GetHistogram ()->emptyPrevZoom ());
    this->Enable (FindItem (_ (STR_UNDO_ZOOM)),
                  !(*histogramDerivedList.begin ())->GetHistogram ()->emptyNextZoom ());
  }
  if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE)
  {
    this->Enable (FindItem (_ (STR_REDO_ZOOM)),
                  !(*timelineDerivedList.begin ())->GetMyWindow ()->emptyNextZoom ());
    this->Enable (FindItem (_ (STR_UNDO_ZOOM)),
                  !(*timelineDerivedList.begin ())->GetMyWindow ()->emptyPrevZoom ());
  }

  this->Enable (FindItem (_ ("Select Objects...")),
                isSelectObjectsAvailable ());

  if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE)
  {
    popUpMenuColor->Enable (FindItem (_ ("Punctual Window...")), allWindowsHas (timelineDerivedList, [] (gTimeline *window)
                                                                                { return window->GetMyWindow ()->isPunctualColorSet (); }));

    popUpMenuColor->Enable (FindItem (_ ("Gradient Function")),
                            (allWindowsHas (timelineDerivedList, [] (gTimeline *window)
                                            { return window->GetMyWindow ()->isGradientColorSet (); })
                             || allWindowsHas (timelineDerivedList, [] (gTimeline *window)
                                               { return window->GetMyWindow ()->isNotNullGradientColorSet (); })
                             || allWindowsHas (timelineDerivedList, [] (gTimeline *window)
                                               { return window->GetMyWindow ()->isAlternativeGradientColorSet (); })
                             || allWindowsHas (timelineDerivedList, [] (gTimeline *window)
                                               { return window->GetMyWindow ()->isFunctionLineColorSet (); })
                             || allWindowsHas (timelineDerivedList, [] (gTimeline *window)
                                               { return window->GetMyWindow ()->isPunctualColorSet (); })));

    popUpMenuColor->Enable (FindItem (_ ("Semantic scale min at 0")),
                            (allWindowsHas (timelineDerivedList, [] (gTimeline *window)
                                            { return window->GetMyWindow ()->isFunctionLineColorSet (); })
                             || allWindowsHas (timelineDerivedList, [] (gTimeline *window)
                                               { return window->GetMyWindow ()->isFusedLinesColorSet (); })
                             || allWindowsHas (timelineDerivedList, [] (gTimeline *window)
                                               { return window->GetMyWindow ()->isPunctualColorSet (); })));
  }

  popUpMenuPaste->Enable (
      popUpMenuPaste->FindItem (_ (STR_TIME)),
      checkAllowedProperties (STR_TIME));
  popUpMenuPaste->Enable (
      popUpMenuPaste->FindItem (_ (STR_SIZE)),
      checkAllowedProperties (STR_SIZE));
  popUpMenuPaste->Enable (
      popUpMenuPaste->FindItem (_ (STR_OBJECTS)),
      checkAllowedProperties (STR_OBJECTS));
  popUpMenuPaste->Enable (
      popUpMenuPaste->FindItem (_ (STR_DURATION)),
      checkAllowedProperties (STR_DURATION));
  popUpMenuPaste->Enable (
      popUpMenuPaste->FindItem (_ (STR_SEMANTIC_SCALE)),
      checkAllowedProperties (STR_SEMANTIC_SCALE));

  if (typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_MULTIPLE || typeDataPopup == PopUpMenuType::POPUP_MENU_TYPE_TIMELINE_SINGLE)
  {
    popUpMenuPaste->Enable (
        popUpMenuPaste->FindItem (_ (STR_CUSTOM_PALETTE)),
        checkAllowedProperties (STR_CUSTOM_PALETTE));
    popUpMenuPaste->Enable (
        popUpMenuPaste->FindItem (_ (STR_FILTER)),
        checkAllowedProperties (STR_FILTER));
    popUpMenuPasteFilter->Enable (
        popUpMenuPasteFilter->FindItem (_ (STR_FILTER_ALL)),
        checkAllowedProperties (STR_FILTER_ALL));
    popUpMenuPasteFilter->Enable (
        popUpMenuPasteFilter->FindItem (_ (STR_FILTER_COMMS)),
        checkAllowedProperties (STR_FILTER_COMMS));
    popUpMenuPasteFilter->Enable (
        popUpMenuPasteFilter->FindItem (_ (STR_FILTER_EVENTS)),
        checkAllowedProperties (STR_FILTER_EVENTS));
  }

  bool tmpEnableRemoveGroup =
      SyncWindows::getInstance ()->getNumGroups () > 1 || SyncWindows::getInstance ()->getNumWindows (0) > 0;
  popUpMenuSync->Enable (popUpMenuSync->FindItem (_ (STR_SYNC_REMOVE_GROUP)),
                         tmpEnableRemoveGroup);
}

void gPopUpMenu::enableItemByTag (const std::string tag, bool enable)
{
  Enable (FindItem (wxString::FromUTF8 (tag.c_str ())), enable);
}

void gPopUpMenu::enableItemByTag (const std::string tag)
{
  Enable (FindItem (wxString::FromUTF8 (tag.c_str ())), true);
}

void gPopUpMenu::disableItemByTag (const std::string tag)
{
  Enable (FindItem (wxString::FromUTF8 (tag.c_str ())), false);
}

/*************************************************************/
/********************EXTRA FUNCTIONALITIES********************/
/*************************************************************/

std::string gPopUpMenu::getOption (wxArrayString &choices, int position)
{
  if (choices[position].Cmp (_ (STR_FILTER_COMMS_XT)) == 0)
    return std::string (STR_FILTER_COMMS);
  else if (choices[position].Cmp (_ (STR_FILTER_EVENTS_XT)) == 0)
    return std::string (STR_FILTER_EVENTS);
  else
    return std::string (choices[position].mb_str ());
}

wxMultiChoiceDialog *gPopUpMenu::createPasteSpecialDialog (wxArrayString &choices,
                                                           gHistogram *whichHistogram)
{
  wxArrayInt sel;
  int i = 0;

  choices.Empty ();

  gPasteWindowProperties *pasteActions =
      gPasteWindowProperties::getInstance ();

  if (pasteActions->isAllowed (whichHistogram, STR_TIME))
  {
    choices.Add (_ (STR_TIME));
    sel.Add (i);
    ++i;
  }

  if (pasteActions->isAllowed (whichHistogram, STR_OBJECTS))
  {
    choices.Add (_ (STR_OBJECTS));
    sel.Add (i);
    ++i;
  }

  if (pasteActions->isAllowed (whichHistogram, STR_SIZE))
  {
    choices.Add (_ (STR_SIZE));
    sel.Add (i);
    ++i;
  }

  if (pasteActions->isAllowed (whichHistogram, STR_DURATION))
    choices.Add (_ (STR_DURATION));

  if (pasteActions->isAllowed (whichHistogram, STR_SEMANTIC_SCALE))
    choices.Add (_ (STR_SEMANTIC_SCALE));

  if (pasteActions->isAllowed (whichHistogram, STR_CONTROL_SCALE))
    choices.Add (_ (STR_CONTROL_SCALE));

  if (pasteActions->isAllowed (whichHistogram, STR_CONTROL_DIMENSIONS))
    choices.Add (_ (STR_CONTROL_DIMENSIONS));

  if (pasteActions->isAllowed (whichHistogram, STR_CONTROL_SCALE))
  {
    if (whichHistogram->GetHistogram ()->getThreeDimensions ())
      choices.Add (_ (STR_3D_SCALE));
  }

  wxMultiChoiceDialog *tmpDialog = new wxMultiChoiceDialog (
      whichHistogram, _ ("Select properties to paste:"), _ ("Paste Special"),
      choices);
  tmpDialog->SetSelections (sel);

  return tmpDialog;
}

wxMultiChoiceDialog *gPopUpMenu::createPasteSpecialDialog (wxArrayString &choices,
                                                           gTimeline *whichTimeline)
{
  wxArrayInt sel;
  int i = 0;

  choices.Empty ();
  gPasteWindowProperties *pasteActions =
      gPasteWindowProperties::getInstance ();

  if (pasteActions->isAllowed (whichTimeline, STR_TIME))
  {
    choices.Add (_ (STR_TIME));
    sel.Add (i);
    ++i;
  }

  if (pasteActions->isAllowed (whichTimeline, STR_OBJECTS))
  {
    choices.Add (_ (STR_OBJECTS));
    sel.Add (i);
    ++i;
  }

  if (pasteActions->isAllowed (whichTimeline, STR_SIZE))
  {
    choices.Add (_ (STR_SIZE));
    sel.Add (i);
    ++i;
  }

  if (pasteActions->isAllowed (whichTimeline, STR_DURATION))
    choices.Add (_ (STR_DURATION));

  if (pasteActions->isAllowed (whichTimeline, STR_SEMANTIC_SCALE))
    choices.Add (_ (STR_SEMANTIC_SCALE));

  if (pasteActions->isAllowed (whichTimeline, STR_FILTER_COMMS))
    choices.Add (_ (STR_FILTER_COMMS_XT));

  if (pasteActions->isAllowed (whichTimeline, STR_FILTER_EVENTS))
    choices.Add (_ (STR_FILTER_EVENTS_XT));

  wxMultiChoiceDialog *tmpDialog = new wxMultiChoiceDialog (
      whichTimeline, _ ("Select properties to paste:"), _ ("Paste Special"),
      choices);
  tmpDialog->SetSelections (sel);

  return tmpDialog;
}

bool gPopUpMenu ::isSelectObjectsAvailable ()
{

  bool sameTraceParameters = true;
  bool sameTraceLevelMode = true;
  bool isProcessModel;

  Trace *trace = nullptr;
  TTraceLevel dataLevel;
  SelectionManagement<TObjectOrder, TTraceLevel> *whichSelectedRows;

  auto isProcessModelCheck = [&] (TTraceLevel traceLevel) -> bool
  {
    return ((traceLevel >= TTraceLevel::WORKLOAD) && (traceLevel <= TTraceLevel::THREAD));
  };

  if (!timelineDerivedList.empty ())
  {
    auto timeline = (*timelineDerivedList.begin ());
    trace = timeline->GetMyWindow ()->getTrace ();
    isProcessModel = isProcessModelCheck (timeline->GetMyWindow ()->getLevel ());
    whichSelectedRows = timeline->GetMyWindow ()->getSelectedRows ();

    for (gTimeline *timeline : timelineDerivedList)
    {
      if (!trace->isSameObjectStruct (timeline->GetMyWindow ()->getTrace (), true))
      {
        sameTraceParameters = false;
        break;
      }
      if (!(isProcessModel == isProcessModelCheck (timeline->GetMyWindow ()->getLevel ())))
      {
        sameTraceLevelMode = false;
        break;
      }
    }
  }
  if (!histogramDerivedList.empty () && sameTraceParameters && sameTraceLevelMode)
  {
    if (trace == nullptr)
    {
      trace = (*histogramDerivedList.begin ())->GetHistogram ()->getDataWindow ()->getTrace ();
      isProcessModel = isProcessModelCheck ((*histogramDerivedList.begin ())->GetHistogram ()->getDataWindow ()->getLevel ());
    }

    for (gHistogram *histogram : histogramDerivedList)
    {
      if (!trace->isSameObjectStruct (histogram->GetHistogram ()->getDataWindow ()->getTrace (), true))
      {
        sameTraceParameters = false;
        break;
      }
      if (!(isProcessModel == isProcessModelCheck (histogram->GetHistogram ()->getDataWindow ()->getLevel ())))
      {
        sameTraceLevelMode = false;
        break;
      }
    }
  }
  return (sameTraceParameters && sameTraceLevelMode);
}

void gPopUpMenu ::createRowSelectionDialog ()
{
  bool isInitialized = false;
  bool isProcessModel = false;
  Trace *trace = nullptr;

  SelectionManagement<TObjectOrder, TTraceLevel> intersectionSelectedRows;
  std::vector<std::vector<TObjectOrder>> tmpWindowSelection;
  std::vector<std::vector<TObjectOrder>> intersectedSelection;

  for (gTimeline *itTimeline : timelineDerivedList)
  {
    itTimeline->GetMyWindow ()->getSelectedRows ()->getAllSelected (tmpWindowSelection);

    if (intersectedSelection.empty ())
    {
      intersectedSelection = tmpWindowSelection;
      isProcessModel = ((itTimeline->GetMyWindow ()->getLevel () >= TTraceLevel::WORKLOAD) && (itTimeline->GetMyWindow ()->getLevel () <= TTraceLevel::THREAD));
      trace = itTimeline->GetMyWindow ()->getTrace ();
    }
    else
    {
      intersectedSelection = matrix_intersection (tmpWindowSelection, intersectedSelection);
    }
  }

  for (gHistogram *itHistogram : histogramDerivedList)
  {
    itHistogram->GetHistogram ()->getRowSelectionManagement ()->getAllSelected (tmpWindowSelection);

    if (intersectedSelection.empty ())
    {
      intersectedSelection = tmpWindowSelection;
      isProcessModel = ((itHistogram->GetHistogram ()->getDataWindow ()->getLevel () >= TTraceLevel::WORKLOAD)
                        && (itHistogram->GetHistogram ()->getDataWindow ()->getLevel () <= TTraceLevel::THREAD));
      trace = itHistogram->GetHistogram ()->getDataWindow ()->getTrace ();
    }
    else
    {
      intersectedSelection = matrix_intersection (tmpWindowSelection, intersectedSelection);
    }
  }
  TTraceLevel beginLevel;
  TTraceLevel endLevel;

  intersectionSelectedRows.init(trace);
  
  intersectionSelectedRows.setAllSelected (intersectedSelection);

  RowsSelectionDialog *myDialog = new RowsSelectionDialog (trace, isProcessModel, &intersectionSelectedRows);

  if (myDialog->ShowModal () == wxID_OK)
  {
    bool applyZoom = ZoomAwareTransferData (myDialog);

    transferDataSelectionToObjects (myDialog, isProcessModel, &intersectionSelectedRows, applyZoom);
  }

  delete myDialog;
}

bool gPopUpMenu::ZoomAwareTransferData (RowsSelectionDialog *myDialog)
{
  auto isZoomAware = true;
  bool applyZoom = true;

  for (gTimeline *timeline : timelineDerivedList)
  {
    if (!isZoomAware)
      break;
    isZoomAware = myDialog->isZoomAwareTransferData (timeline->GetMyWindow ()->getCurrentZoomRange ());
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    if (!isZoomAware)
      break;
    isZoomAware = myDialog->isZoomAwareTransferData (histogram->GetHistogram ()->getCurrentZoomRange ());
  }
  if (!isZoomAware)
  {
    wxString tmpMsg = wxT ("Do you want to extend the zoom to fit selected objects?");
    int answer = wxMessageBox (tmpMsg, _ ("Paraver question"), wxYES_NO | wxICON_QUESTION, myDialog);

    applyZoom = (answer == wxYES);
  }
  return applyZoom;
}
void gPopUpMenu ::transferDataSelectionToObjects (RowsSelectionDialog *myDialog, bool isProcessModel, SelectionManagement<TObjectOrder, TTraceLevel> *intersectionSelectedRows, bool applyZoom)
{
  TTraceLevel beginLevel;
  TTraceLevel endLevel;

  // Set range of levels for update loop
  if (isProcessModel)
  {
    beginLevel = TTraceLevel::APPLICATION;
    endLevel = TTraceLevel::THREAD;
  }
  else
  {
    beginLevel = TTraceLevel::NODE;
    endLevel = TTraceLevel::CPU;
  }

  // Loop through levels to update gTimeline
  for (TTraceLevel whichLevel = beginLevel; whichLevel <= endLevel; ++whichLevel)
  {
    std::vector<bool> whichSelected;
    intersectionSelectedRows->getSelected (whichSelected, whichLevel);

    for (gTimeline *timeline : timelineDerivedList)
    {
      auto zoomAware = myDialog->isZoomAwareTransferData (timeline->GetMyWindow ()->getCurrentZoomRange ());
      if (applyZoom && !zoomAware)
      {
        timeline->GetMyWindow ()->addZoom (myDialog->GetNewBeginZoom (), myDialog->GetNewEndZoom ());
      }
      timeline->GetMyWindow ()->setSelectedRows (whichLevel, whichSelected);
      timeline->GetMyWindow ()->setRedraw (true);
      timeline->GetMyWindow ()->setChanged (true);
    }
    for (gHistogram *histogram : histogramDerivedList)
    {
      std::vector<TObjectOrder> selection;
      histogram->GetHistogram ()->getRowSelectionManagement ()->getSelected (selection, histogram->GetHistogram ()->getDataWindow ()->getLevel ());

      auto zoomAware = myDialog->isZoomAwareTransferData (selection);
      if (applyZoom && !zoomAware)
      {
        histogram->GetHistogram ()->addZoom (myDialog->GetNewBeginZoom (), myDialog->GetNewEndZoom ());
      }
      histogram->GetHistogram ()->getRowSelectionManagement ()->setSelected (whichSelected, whichLevel);
      histogram->GetHistogram ()->setRecalc (true);
    }
  }
}

RowsSelectionDialog *
gPopUpMenu ::createRowSelectionDialog (gTimeline *whichTimeline)
{
  bool isProcessModel = ((whichTimeline->GetMyWindow ()->getLevel () >= TTraceLevel::WORKLOAD) && (whichTimeline->GetMyWindow ()->getLevel () <= TTraceLevel::THREAD));

  RowsSelectionDialog *myDialog = new RowsSelectionDialog (whichTimeline->GetMyWindow ()->getTrace (), isProcessModel, whichTimeline->GetMyWindow ()->getSelectedRows ());

  return myDialog;
}

RowsSelectionDialog *
gPopUpMenu::createRowSelectionDialog (gHistogram *histogram)
{
  bool isProcessModel = ((histogram->GetHistogram ()->getDataWindow ()->getLevel () >= TTraceLevel::WORKLOAD)
                         && (histogram->GetHistogram ()->getDataWindow ()->getLevel () <= TTraceLevel::THREAD));

  RowsSelectionDialog *myDialog = new RowsSelectionDialog (histogram->GetHistogram ()->getTrace (), isProcessModel, histogram->GetHistogram ()->getRowSelectionManagement ());

  return myDialog;
}

/*************************************************************/
/********************ON EVENTS FUNCTIONS**********************/
/*************************************************************/

void gPopUpMenu::OnPopUpCopy (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpCopy (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpCopy (event);
  }
}
void gPopUpMenu::OnPopUpPasteDefaultSpecial (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpPasteDefaultSpecial (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpPasteDefaultSpecial (event);
  }
}
void gPopUpMenu::OnPopUpPasteSpecial (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpPasteSpecial (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpPasteSpecial (event);
  }
}
void gPopUpMenu::OnPopUpPasteTime (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpPasteTime (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpPasteTime (event);
  }
}
void gPopUpMenu::OnPopUpPasteObjects (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpPasteObjects (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpPasteObjects (event);
  }
}
void gPopUpMenu::OnPopUpPasteSize (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpPasteSize (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpPasteSize (event);
  }
}
void gPopUpMenu::OnPopUpPasteDuration (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpPasteDuration (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpPasteDuration (event);
  }
}
void gPopUpMenu::OnPopUpPasteSemanticScale (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpPasteSemanticScale (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpPasteSemanticScale (event);
  }
}
void gPopUpMenu::OnPopUpPasteSemanticSort (wxCommandEvent &event)
{

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpPasteSemanticSort (event);
  }
}

// TIMELINE
void gPopUpMenu::OnPopUpPasteCustomPalette (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpPasteCustomPalette (event);
  }
}
void gPopUpMenu::OnPopUpPasteFilterAll (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpPasteFilterAll (event);
  }
}
void gPopUpMenu::OnPopUpPasteFilterCommunications (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpPasteFilterCommunications (event);
  }
}
void gPopUpMenu::OnPopUpPasteFilterEvents (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpPasteFilterEvents (event);
  }
}

// HISGORAM
void gPopUpMenu::OnPopUpPasteControlScale (wxCommandEvent &event)
{
  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpPasteControlScale (event);
  }
}
void gPopUpMenu::OnPopUpPaste3DScale (wxCommandEvent &event)
{
  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpPaste3DScale (event);
  }
}
void gPopUpMenu::OnPopUpPasteControlDimensions (wxCommandEvent &event)
{
  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpPasteControlDimensions (event);
  }
}

// MIX
void gPopUpMenu::OnPopUpClone (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpClone (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpClone (event);
  }
}
void gPopUpMenu::OnPopUpRename (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpRename (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpRename (event);
  }
}
void gPopUpMenu::OnPopUpFitTimeScale (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpFitTimeScale (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpFitTimeScale (event);
  }
}
void gPopUpMenu::OnPopUpFitObjects (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpFitObjects (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpFitObjects (event);
  }
}

// TIMELINE
void gPopUpMenu::OnPopUpFitSemanticScaleMin (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpFitSemanticScaleMin (event);
  }
}
void gPopUpMenu::OnPopUpFitSemanticScaleMax (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpFitSemanticScaleMax (event);
  }
}
void gPopUpMenu::OnPopUpFitSemanticScale (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpFitSemanticScale (event);
  }
}
void gPopUpMenu::OnPopUpViewCommunicationLines (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpViewCommunicationLines (event);
  }
}
void gPopUpMenu::OnPopUpViewEventFlags (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpViewEventFlags (event);
  }
}
void gPopUpMenu::OnPopUpFunctionLineColor (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpFunctionLineColor (event);
  }
}
void gPopUpMenu::OnPopUpFusedLinesColor (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpFusedLinesColor (event);
  }
}
void gPopUpMenu::OnPopUpPunctualColor (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpPunctualColor (event);
  }
}
void gPopUpMenu::OnPopUpPunctualColorWindow (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpPunctualColorWindow (event);
  }
}
void gPopUpMenu::OnPopUpCodeColor (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpCodeColor (event);
  }
}
void gPopUpMenu::OnPopUpGradientColor (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpGradientColor (event);
  }
}
void gPopUpMenu::OnPopUpNotNullGradientColor (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpNotNullGradientColor (event);
  }
}
void gPopUpMenu::OnPopUpAlternativeGradientColor (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpAlternativeGradientColor (event);
  }
}
void gPopUpMenu::OnPopUpSemanticScaleMinAtZero (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpSemanticScaleMinAtZero (event);
  }
}

// HISTOGRAM
void gPopUpMenu::OnPopUpRowSelection (wxCommandEvent &event)
{
  createRowSelectionDialog ();
}
void gPopUpMenu::OnPopUpAutoControlScale (wxCommandEvent &event)
{
  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpAutoControlScale (event);
  }
}
void gPopUpMenu::OnPopUpAutoControlScaleZero (wxCommandEvent &event)
{
  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpAutoControlScaleZero (event);
  }
}
void gPopUpMenu::OnPopUpAuto3DScale (wxCommandEvent &event)
{
  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpAuto3DScale (event);
  }
}
void gPopUpMenu::OnPopUpAutoDataGradient (wxCommandEvent &event)
{
  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpAutoDataGradient (event);
  }
}

void gPopUpMenu::OnPopUpColor2D (wxCommandEvent &event)
{
  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpColor2D (event);
  }
}

// MIX
void gPopUpMenu::OnPopUpGradientFunction (wxCommandEvent &event)
{

  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpGradientFunction (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {

    histogram->OnPopUpGradientFunction (event);
  }
}
void gPopUpMenu::OnPopUpUndoZoom (wxCommandEvent &event)
{

  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpUndoZoom (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpUndoZoom (event);
  }
}
void gPopUpMenu::OnPopUpRedoZoom (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpRedoZoom (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpRedoZoom (event);
  }
}

// MIX
void gPopUpMenu::OnPopUpDrawModeTimeLast (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeTimeLast (event);
  }
  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeSemanticLast (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeTimeRandom (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeTimeRandom (event);
  }
  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeSemanticRandom (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeTimeRandomNotZero (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeTimeRandomNotZero (event);
  }
  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeSemanticRandomNotZero (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeTimeMaximum (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeTimeMaximum (event);
  }
  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeSemanticMaximum (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeTimeMinimumNotZero (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeTimeMinimumNotZero (event);
  }
  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeSemanticMinimumNotZero (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeTimeAbsoluteMaximum (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeTimeAbsoluteMaximum (event);
  }
  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeSemanticAbsoluteMaximum (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeTimeAbsoluteMinimumNotZero (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeTimeAbsoluteMinimumNotZero (event);
  }
  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeSemanticAbsoluteMinimumNotZero (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeTimeAverage (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeTimeAverage (event);
  }
  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeSemanticAverage (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeTimeAverageNotZero (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeTimeAverageNotZero (event);
  }
  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeSemanticAverageNotZero (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeTimeMode (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeTimeMode (event);
  }
  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeSemanticMode (event);
  }
}

// MIX
void gPopUpMenu::OnPopUpDrawModeObjectsLast (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeObjectsLast (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeObjectsLast (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeObjectsRandom (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeObjectsRandom (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeObjectsRandom (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeObjectsRandomNotZero (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeObjectsRandomNotZero (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeObjectsRandomNotZero (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeObjectsMaximum (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeObjectsMaximum (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeObjectsMaximum (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeObjectsMinimumNotZero (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeObjectsMinimumNotZero (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeObjectsMinimumNotZero (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeObjectsAbsoluteMaximum (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeObjectsAbsoluteMaximum (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeObjectsAbsoluteMaximum (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeObjectsAbsoluteMinimumNotZero (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeObjectsAbsoluteMinimumNotZero (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeObjectsAbsoluteMinimumNotZero (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeObjectsAverage (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeObjectsAverage (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeObjectsAverage (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeObjectsAverageNotZero (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeObjectsAverageNotZero (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeObjectsAverageNotZero (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeObjectsMode (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeObjectsMode (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeObjectsMode (event);
  }
}

void gPopUpMenu::OnPopUpDrawModeBothLast (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeBothLast (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeBothLast (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeBothRandom (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeBothRandom (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeBothRandom (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeBothRandomNotZero (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeBothRandomNotZero (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeBothRandomNotZero (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeBothMaximum (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeBothMaximum (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeBothMaximum (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeBothMinimumNotZero (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeBothMinimumNotZero (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeBothMinimumNotZero (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeBothAbsoluteMaximum (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeBothAbsoluteMaximum (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeBothAbsoluteMaximum (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeBothAbsoluteMinimumNotZero (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeBothAbsoluteMinimumNotZero (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeBothAbsoluteMinimumNotZero (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeBothAverage (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeBothAverage (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeBothAverage (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeBothAverageNotZero (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeBothAverageNotZero (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeBothAverageNotZero (event);
  }
}
void gPopUpMenu::OnPopUpDrawModeBothMode (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpDrawModeBothMode (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpDrawModeBothMode (event);
  }
}

void gPopUpMenu::OnPopUpPixelSize (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpPixelSize (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpPixelSize (event);
  }
}
void gPopUpMenu::OnPopUpSynchronize (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpSynchronize (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpSynchronize (event);
  }
}
void gPopUpMenu::OnPopUpRemoveGroup (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpRemoveGroup (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpRemoveGroup (event);
  }
}
void gPopUpMenu::OnPopUpRemoveAllGroups (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpRemoveAllGroups (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpRemoveAllGroups (event);
  }
}

// Timeline
void gPopUpMenu::OnPopUpLabels (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpLabels (event);
  }
}
void gPopUpMenu::OnPopUpObjectAxis (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpObjectAxis (event);
  }
}
void gPopUpMenu::OnPopUpRunApp (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpRunApp (event);
  }
}

void gPopUpMenu::OnPopUpTiming (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpTiming (event);
  }
}

void gPopUpMenu::OnPopUpInfoPanel (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpInfoPanel (event);
  }
}

// Mix

void gPopUpMenu::OnPopUpSaveCFG (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpSaveCFG (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpSaveCFG (event);
  }
}
void gPopUpMenu::OnPopUpSaveImageDialog (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpSaveImageDialog (event);
  }

  for (gHistogram *histogram : histogramDerivedList)
  {
    histogram->OnPopUpRemoveAllGroups (event);
  }
}

// Timeline
void gPopUpMenu::OnPopUpSaveText (wxCommandEvent &event)
{
  for (gTimeline *timeline : timelineDerivedList)
  {
    timeline->OnPopUpSaveText (event);
  }
}
