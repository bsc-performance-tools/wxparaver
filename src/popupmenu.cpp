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
#include "labelconstructor.h"
#include "window.h"
#include "syncwindows.h"
#include <wx/event.h>
#include <iostream>
#include <cmath>
#include "sequencedriver.h"
#include "paravermain.h"
#include "paraverlabels.h"

using namespace std;

template<>
void gPopUpMenu<gTimeline>::enableMenu( gTimeline *whichTimeline )
{
  gPasteWindowProperties* sharedProperties = gPasteWindowProperties::getInstance();

  Enable( FindItem( _( STR_COPY ) ), true );

  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_TIME ) ), sharedProperties->isAllowed( whichTimeline, STR_TIME) );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_SIZE ) ), sharedProperties->isAllowed( whichTimeline, STR_SIZE) );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_OBJECTS ) ), sharedProperties->isAllowed( whichTimeline, STR_OBJECTS) );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_DURATION ) ), sharedProperties->isAllowed( whichTimeline, STR_DURATION ) );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_SEMANTIC_SCALE ) ), sharedProperties->isAllowed( whichTimeline, STR_SEMANTIC_SCALE ) );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_CUSTOM_PALETTE ) ), sharedProperties->isAllowed( whichTimeline, STR_CUSTOM_PALETTE ) );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_FILTER ) ), sharedProperties->isAllowed( whichTimeline, STR_FILTER) );
  
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( _( STR_FILTER_ALL ) ), sharedProperties->isAllowed( whichTimeline, STR_FILTER_ALL) );
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( _( STR_FILTER_COMMS ) ), sharedProperties->isAllowed( whichTimeline, STR_FILTER_COMMS) );
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( _( STR_FILTER_EVENTS ) ), sharedProperties->isAllowed( whichTimeline, STR_FILTER_EVENTS) );

  popUpMenuSync->Enable( popUpMenuSync->FindItem( _( STR_SYNC_REMOVE_GROUP ) ), SyncWindows::getInstance()->getNumGroups() > 1 );
  
  Enable( FindItem( _( STR_PASTE ) ), sharedProperties->isAllowed( whichTimeline, STR_PASTE ) );
  Enable( FindItem( _( STR_PASTE_DEFAULT_SPECIAL ) ), sharedProperties->isAllowed( whichTimeline, STR_PASTE_DEFAULT_SPECIAL ) );
  Enable( FindItem( _( STR_PASTE_SPECIAL ) ), sharedProperties->isAllowed( whichTimeline, STR_PASTE_SPECIAL ) );

  Enable( FindItem( _( STR_CLONE ) ), true );
  Enable( FindItem( _( STR_RENAME ) ), true );

  Enable( FindItem( _( STR_FIT_TIME ) ), true );
  Enable( FindItem( _( STR_FIT_SEMANTIC ) ), true );
  Enable( FindItem( _( STR_FIT_OBJECTS ) ), true );
}


template<>
void gPopUpMenu<gHistogram>::enableMenu( gHistogram *whichHistogram )
{
  gPasteWindowProperties* sharedProperties = gPasteWindowProperties::getInstance();

  Enable( FindItem( _( STR_COPY ) ), true );

  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_TIME ) ), sharedProperties->isAllowed( whichHistogram, STR_TIME) );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_SIZE ) ), sharedProperties->isAllowed( whichHistogram, STR_SIZE)  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_OBJECTS ) ), sharedProperties->isAllowed( whichHistogram, STR_OBJECTS)  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_DURATION ) ), sharedProperties->isAllowed( whichHistogram, STR_DURATION )  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_SEMANTIC_SCALE ) ), sharedProperties->isAllowed( whichHistogram, STR_SEMANTIC_SCALE)  );

  popUpMenuSync->Enable( popUpMenuSync->FindItem( _( STR_SYNC_REMOVE_GROUP ) ), SyncWindows::getInstance()->getNumGroups() > 1 );

  Enable( FindItem( _( STR_PASTE ) ), sharedProperties->isAllowed( whichHistogram, STR_PASTE ) );
  Enable( FindItem( _( STR_PASTE_DEFAULT_SPECIAL ) ), sharedProperties->isAllowed( whichHistogram, STR_PASTE_DEFAULT_SPECIAL ) );
  Enable( FindItem( _( STR_PASTE_SPECIAL ) ), sharedProperties->isAllowed( whichHistogram, STR_PASTE_SPECIAL ) );

  Enable( FindItem( _( STR_CLONE ) ), true );
  Enable( FindItem( _( STR_RENAME ) ), true );
  Enable( FindItem( _( STR_FIT_TIME ) ), true );
  Enable( FindItem( _( STR_FIT_OBJECTS ) ), true );
  
  Enable( FindItem( _( STR_CONTROL_SCALE ) ), sharedProperties->isAllowed( whichHistogram, STR_CONTROL_SCALE ) );
  Enable( FindItem( _( STR_PASTE_SEMANTIC_SORT ) ), sharedProperties->isAllowed( whichHistogram, STR_PASTE_SEMANTIC_SORT ) );
  Enable( FindItem( _( STR_CONTROL_DIMENSIONS ) ), sharedProperties->isAllowed( whichHistogram, STR_CONTROL_DIMENSIONS ) );
  if( whichHistogram->GetHistogram()->getThreeDimensions() )
    Enable( FindItem( _( STR_3D_SCALE ) ), sharedProperties->isAllowed( whichHistogram, STR_3D_SCALE) );
  else
    Enable( FindItem( _( STR_3D_SCALE ) ), false );

  Enable( FindItem( _( STR_AUTOFIT_CONTROL_ZERO ) ), whichHistogram->GetHistogram()->getCompute2DScale() );

  Enable( FindItem( _( "Image..." ) ), whichHistogram->GetHistogram()->getZoom() );
}


template<>
wxMultiChoiceDialog *gPopUpMenu<gHistogram>::createPasteSpecialDialog( wxArrayString& choices, gHistogram *whichHistogram )
{
  wxArrayInt sel;
  int i = 0;

  choices.Empty();
  
  gPasteWindowProperties *pasteActions = gPasteWindowProperties::getInstance();
  
  if ( pasteActions->isAllowed( whichHistogram, STR_TIME ) )
  {
    choices.Add( _( STR_TIME ) );
    sel.Add( i );
    ++i;
  }

  if ( pasteActions->isAllowed( whichHistogram, STR_OBJECTS ) )
  {
    choices.Add( _( STR_OBJECTS ) );
    sel.Add( i );
    ++i;
  }

  if ( pasteActions->isAllowed( whichHistogram, STR_SIZE ) )
  {
    choices.Add( _( STR_SIZE ) );
    sel.Add( i );
    ++i;
  }

  if ( pasteActions->isAllowed( whichHistogram, STR_DURATION ) )
    choices.Add( _( STR_DURATION ) );

  if ( pasteActions->isAllowed( whichHistogram, STR_SEMANTIC_SCALE ) )
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
  
  wxMultiChoiceDialog *tmpDialog = new wxMultiChoiceDialog( whichHistogram,
                                                            _( "Select properties to paste:" ),
                                                            _("Paste Special"),
                                                            choices);
  tmpDialog->SetSelections( sel );
  
  return tmpDialog;
}


template<>
wxMultiChoiceDialog *gPopUpMenu<gTimeline>::createPasteSpecialDialog( wxArrayString& choices, gTimeline *whichTimeline )
{
  wxArrayInt sel;
  int i = 0;

  choices.Empty();
  gPasteWindowProperties *pasteActions = gPasteWindowProperties::getInstance();
  
  if ( pasteActions->isAllowed( whichTimeline, STR_TIME ) )
  {
    choices.Add( _( STR_TIME ) );
    sel.Add( i );
    ++i;
  }

  if ( pasteActions->isAllowed( whichTimeline, STR_OBJECTS ) )
  {
    choices.Add( _( STR_OBJECTS ) );
    sel.Add( i );
    ++i;
  }

  if ( pasteActions->isAllowed( whichTimeline, STR_SIZE ) )
  {
    choices.Add( _( STR_SIZE ) );
    sel.Add( i );
    ++i;
  }

  if ( pasteActions->isAllowed( whichTimeline, STR_DURATION ) )
    choices.Add( _( STR_DURATION ) );

  if ( pasteActions->isAllowed( whichTimeline, STR_SEMANTIC_SCALE ) )
    choices.Add( _( STR_SEMANTIC_SCALE ) );

  if ( pasteActions->isAllowed( whichTimeline, STR_FILTER_COMMS ) )
    choices.Add( _( STR_FILTER_COMMS_XT ) );

  if ( pasteActions->isAllowed( whichTimeline, STR_FILTER_EVENTS ) )
    choices.Add( _( STR_FILTER_EVENTS_XT ) );

  wxMultiChoiceDialog *tmpDialog = new wxMultiChoiceDialog( whichTimeline,
                                                            _( "Select properties to paste:" ),
                                                            _("Paste Special"),
                                                            choices );
  tmpDialog->SetSelections( sel );
  
  return tmpDialog;
}


template<>
RowsSelectionDialog *gPopUpMenu<gTimeline>::createRowSelectionDialog( gTimeline *whichTimeline )
{
  bool parentIsGtimeline = true;

  RowsSelectionDialog *myDialog = new RowsSelectionDialog( (wxWindow * )whichTimeline,
                                                           whichTimeline->GetMyWindow(),
                                                           whichTimeline->GetMyWindow()->getSelectedRows(),
                                                           ID_ROWSSELECTIONDIALOG,
                                                           _( "Timeline Row Selection" ), 
                                                           parentIsGtimeline );

  return myDialog;
}


template<>
RowsSelectionDialog *gPopUpMenu<gHistogram>::createRowSelectionDialog( gHistogram *histogram )
{
  bool parentIsGtimeline = true;

  RowsSelectionDialog *myDialog = new RowsSelectionDialog( (wxWindow * )histogram,
                                                           histogram->GetHistogram(),
                                                           histogram->GetHistogram()->getRowSelectionManagement(),
                                                           ID_ROWSSELECTIONDIALOG,
                                                           _( "Histogram Row Selection" ), 
                                                           parentIsGtimeline );

  return myDialog;
}


template<>
gPopUpMenu<gTimeline>::gPopUpMenu( gTimeline *whichTimeline )
{
  window = whichTimeline;

  popUpMenuView = new wxMenu;
  popUpMenuColor = new wxMenu;
  popUpMenuPaste = new wxMenu;
  popUpMenuPasteFilter = new wxMenu;
  popUpMenuFitSemantic = new wxMenu;
  popUpMenuDrawMode = new wxMenu;
  popUpMenuDrawModeTime = new wxMenu;
  popUpMenuDrawModeObjects = new wxMenu;
  popUpMenuDrawModeBoth = new wxMenu;
  popUpMenuPixelSize = new wxMenu;
  popUpMenuGradientFunction = new wxMenu;
  popUpMenuLabels = new wxMenu;
  popUpMenuObjectAxis = new wxMenu;
  popUpMenuSave = new wxMenu;
  popUpMenuRun = new wxMenu;
  popUpMenuSync = new wxMenu;
  popUpMenuSyncRemove = new wxMenu;

#ifdef __WXMAC__
  buildItem( this, _( STR_COPY ), wxITEM_NORMAL, &gTimeline::OnPopUpCopy, ID_MENU_COPY );
#else
  buildItem( this, _( STR_COPY ), wxITEM_NORMAL, &gTimeline::OnPopUpCopy, wxID_COPY );
#endif
  buildItem( popUpMenuPaste, _( STR_PASTE_DEFAULT_SPECIAL ), wxITEM_NORMAL, &gTimeline::OnPopUpPasteDefaultSpecial, ID_MENU_PASTE_DEFAULT_SPECIAL );
  popUpMenuPaste->AppendSeparator();
  buildItem( popUpMenuPaste, _( STR_TIME ), wxITEM_NORMAL, &gTimeline::OnPopUpPasteTime, ID_MENU_PASTE_TIME );
  buildItem( popUpMenuPaste, _( STR_OBJECTS ), wxITEM_NORMAL, &gTimeline::OnPopUpPasteObjects, ID_MENU_PASTE_OBJECTS );
  buildItem( popUpMenuPaste, _( STR_SIZE ), wxITEM_NORMAL, &gTimeline::OnPopUpPasteSize, ID_MENU_PASTE_SIZE );
  buildItem( popUpMenuPaste, _( STR_DURATION ), wxITEM_NORMAL, &gTimeline::OnPopUpPasteDuration, ID_MENU_PASTE_DURATION );

  buildItem( popUpMenuPaste, _( STR_SEMANTIC_SCALE ), wxITEM_NORMAL, &gTimeline::OnPopUpPasteSemanticScale, ID_MENU_PASTE_SEMANTIC_SCALE );

  buildItem( popUpMenuPaste, _( STR_CUSTOM_PALETTE ), wxITEM_NORMAL, &gTimeline::OnPopUpPasteCustomPalette, ID_MENU_PASTE_CUSTOM_PALETTE );

  buildItem( popUpMenuPasteFilter, _( STR_FILTER_ALL ), wxITEM_NORMAL, &gTimeline::OnPopUpPasteFilterAll, ID_MENU_PASTE_FILTER_ALL );
  buildItem( popUpMenuPasteFilter, _( STR_FILTER_COMMS ), wxITEM_NORMAL, &gTimeline::OnPopUpPasteFilterCommunications, ID_MENU_PASTE_FILTER_COMMS );
  buildItem( popUpMenuPasteFilter, _( STR_FILTER_EVENTS ), wxITEM_NORMAL, &gTimeline::OnPopUpPasteFilterEvents, ID_MENU_PASTE_FILTER_EVENTS );
  popUpMenuPaste->AppendSubMenu( popUpMenuPasteFilter, _( STR_FILTER ) );

  buildItem( popUpMenuPaste, _( STR_PASTE_SPECIAL ), wxITEM_NORMAL, &gTimeline::OnPopUpPasteSpecial, ID_MENU_PASTE_SPECIAL );

  AppendSubMenu( popUpMenuPaste, _( STR_PASTE ) );

  buildItem( this, _( STR_CLONE ), wxITEM_NORMAL, &gTimeline::OnPopUpClone, ID_MENU_CLONE );
  buildItem( this, _( STR_RENAME ), wxITEM_NORMAL, &gTimeline::OnPopUpRename, ID_MENU_RENAME );

  AppendSeparator();

  buildItem( this, _( STR_UNDO_ZOOM ), wxITEM_NORMAL, &gTimeline::OnPopUpUndoZoom, wxID_UNDO );
  buildItem( this, _( STR_REDO_ZOOM ), wxITEM_NORMAL, &gTimeline::OnPopUpRedoZoom, wxID_REDO );

  buildItem( this, _( STR_FIT_TIME ), wxITEM_NORMAL, &gTimeline::OnPopUpFitTimeScale, wxID_ZOOM_100 );
  buildItem( popUpMenuFitSemantic, _( "Fit Minimum" ), wxITEM_NORMAL, &gTimeline::OnPopUpFitSemanticScaleMin, ID_MENU_FIT_SEMANTIC_MIN);
  buildItem( popUpMenuFitSemantic, _( "Fit Maximum" ), wxITEM_NORMAL, &gTimeline::OnPopUpFitSemanticScaleMax, ID_MENU_FIT_SEMANTIC_MAX);
  buildItem( popUpMenuFitSemantic, _( "Fit Both" ), wxITEM_NORMAL, &gTimeline::OnPopUpFitSemanticScale, ID_MENU_FIT_SEMANTIC_BOTH);
  AppendSubMenu( popUpMenuFitSemantic, _( STR_FIT_SEMANTIC ) );

  buildItem( this, _( STR_FIT_OBJECTS ), wxITEM_NORMAL, &gTimeline::OnPopUpFitObjects, ID_MENU_FIT_OBJECTS );
  buildItem( this, _( "Select Objects..." ), wxITEM_NORMAL, &gTimeline::OnPopUpRowSelection, ID_MENU_ROW_SELECTION );

  AppendSeparator();

  buildItem( popUpMenuView, _( "Communication Lines" ), wxITEM_CHECK, &gTimeline::OnPopUpViewCommunicationLines, ID_MENU_VIEW_COMMUNICATION_LINES, window->GetMyWindow()->getDrawCommLines() );
  buildItem( popUpMenuView, _( "Event Flags" ), wxITEM_CHECK, &gTimeline::OnPopUpViewEventFlags, ID_MENU_VIEW_EVENT_FLAGS, window->GetMyWindow()->getDrawFlags() );
  AppendSubMenu( popUpMenuView, _( "View" ));

  buildItem( popUpMenuColor, _( "Function Line" ), wxITEM_RADIO, &gTimeline::OnPopUpFunctionLineColor, ID_MENU_VIEW_FUNCTION_LINE, window->GetMyWindow()->isFunctionLineColorSet() );
  buildItem( popUpMenuColor, _( "Fused Lines" ), wxITEM_RADIO, &gTimeline::OnPopUpFusedLinesColor, ID_MENU_VIEW_FUSED_LINES, window->GetMyWindow()->isFusedLinesColorSet() );
  buildItem( popUpMenuColor, _( "Punctual" ), wxITEM_RADIO, &gTimeline::OnPopUpPunctualColor, ID_MENU_PUNCTUAL, window->GetMyWindow()->isPunctualColorSet() );
  buildItem( popUpMenuColor, _( "Code Color" ), wxITEM_RADIO, &gTimeline::OnPopUpCodeColor, ID_MENU_CODE_COLOR, window->GetMyWindow()->isCodeColorSet() );
  buildItem( popUpMenuColor, _( "Gradient Color" ), wxITEM_RADIO, &gTimeline::OnPopUpGradientColor, ID_MENU_GRADIENT_COLOR, window->GetMyWindow()->isGradientColorSet() );
  buildItem( popUpMenuColor, _( "Not Null Gradient Color" ), wxITEM_RADIO, &gTimeline::OnPopUpNotNullGradientColor, ID_MENU_NOT_NULL_GRADIENT_COLOR, window->GetMyWindow()->isNotNullGradientColorSet() );

  popUpMenuColor->AppendSeparator();

  wxMenuItem *tmpPuncWin = buildItem( popUpMenuColor, _( "Punctual Window..." ), wxITEM_NORMAL, &gTimeline::OnPopUpPunctualColorWindow, ID_MENU_PUNCTUAL_WINDOW );
  popUpMenuColor->Enable( tmpPuncWin->GetId(), window->GetMyWindow()->isPunctualColorSet() );

  buildItem( popUpMenuGradientFunction, _( "Linear" ), wxITEM_RADIO, &gTimeline::OnPopUpGradientFunction, ID_MENU_GRADIENT_FUNCTION_LINEAR, window->GetMyWindow()->getGradientColor().getGradientFunction() == TGradientFunction::LINEAR );
  buildItem( popUpMenuGradientFunction, _( "Steps" ), wxITEM_RADIO, &gTimeline::OnPopUpGradientFunction, ID_MENU_GRADIENT_FUNCTION_STEPS, window->GetMyWindow()->getGradientColor().getGradientFunction() == TGradientFunction::STEPS );
  buildItem( popUpMenuGradientFunction, _( "Logarithmic" ), wxITEM_RADIO, &gTimeline::OnPopUpGradientFunction, ID_MENU_GRADIENT_FUNCTION_LOGARITHMIC, window->GetMyWindow()->getGradientColor().getGradientFunction() == TGradientFunction::LOGARITHMIC );
  buildItem( popUpMenuGradientFunction, _( "Exponential" ), wxITEM_RADIO, &gTimeline::OnPopUpGradientFunction, ID_MENU_GRADIENT_FUNCTION_EXPONENTIAL, window->GetMyWindow()->getGradientColor().getGradientFunction() == TGradientFunction::EXPONENTIAL );
  wxMenuItem *tmpGradFunc = popUpMenuColor->AppendSubMenu( popUpMenuGradientFunction, _( "Gradient Function" ) );
  popUpMenuColor->Enable( tmpGradFunc->GetId(), window->GetMyWindow()->isGradientColorSet() ||
                                                window->GetMyWindow()->isNotNullGradientColorSet() ||
                                                window->GetMyWindow()->isFunctionLineColorSet() ||
                                                window->GetMyWindow()->isPunctualColorSet() );

  wxMenuItem *tmpSemScaleMinZero = buildItem( popUpMenuColor, _( "Semantic scale min at 0" ), wxITEM_CHECK, &gTimeline::OnPopUpSemanticScaleMinAtZero, ID_MENU_SEMANTIC_SCALE_MIN_AT_ZERO, window->GetMyWindow()->getSemanticScaleMinAtZero() );
  popUpMenuColor->Enable( tmpSemScaleMinZero->GetId(), window->GetMyWindow()->isFunctionLineColorSet() ||
                                                       window->GetMyWindow()->isFusedLinesColorSet() ||
                                                       window->GetMyWindow()->isPunctualColorSet() );

  AppendSubMenu( popUpMenuColor, _( "Paint As" ));


  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_LAST ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeTimeLast,
             ID_MENU_DRAWMODE_TIME_LAST,
             window->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_LAST );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_MAXIMUM ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeTimeMaximum,
             ID_MENU_DRAWMODE_TIME_MAXIMUM,
             window->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_ABSOLUTE_MAXIMUM ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeTimeAbsoluteMaximum,
             ID_MENU_DRAWMODE_TIME_ABSOLUTE_MAXIMUM,
             window->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeTimeMinimumNotZero,
             ID_MENU_DRAWMODE_TIME_MINIMUM_NOT_ZERO,
             window->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_ABSOLUTE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeTimeAbsoluteMinimumNotZero,
             ID_MENU_DRAWMODE_TIME_ABSOLUTE_MINIMUM_NOT_ZERO,
             window->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO);
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_RANDOM ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeTimeRandom,
             ID_MENU_DRAWMODE_TIME_RANDOM,
             window->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_RANDOM );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_RANDOM_NOT_ZERO ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeTimeRandomNotZero,
             ID_MENU_DRAWMODE_TIME_RANDOM_NOT_ZERO,
             window->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_AVERAGE ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeTimeAverage,
             ID_MENU_DRAWMODE_TIME_AVERAGE,
             window->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_AVERAGE );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_AVERAGE_NOT_ZERO ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeTimeAverageNotZero,
             ID_MENU_DRAWMODE_TIME_AVERAGE_NOT_ZERO,
             window->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_AVERAGENOTZERO );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_MODE ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeTimeMode,
             ID_MENU_DRAWMODE_TIME_MODE,
             window->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_MODE );

  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_LAST ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeObjectsLast,
             ID_MENU_DRAWMODE_OBJECTS_LAST,
             window->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_LAST );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_MAXIMUM ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeObjectsMaximum,
             ID_MENU_DRAWMODE_OBJECTS_MAXIMUM,
             window->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_ABSOLUTE_MAXIMUM ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeObjectsAbsoluteMaximum,
             ID_MENU_DRAWMODE_OBJECTS_ABSOLUTE_MAXIMUM,
             window->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeObjectsMinimumNotZero,
             ID_MENU_DRAWMODE_OBJECTS_MINIMUM_NOT_ZERO,
             window->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_ABSOLUTE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeObjectsAbsoluteMinimumNotZero,
             ID_MENU_DRAWMODE_OBJECTS_ABSOLUTE_MINIMUM_NOT_ZERO,
             window->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_RANDOM ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeObjectsRandom,
             ID_MENU_DRAWMODE_OBJECTS_RANDOM,
             window->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_RANDOM );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_RANDOM_NOT_ZERO ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeObjectsRandomNotZero,
             ID_MENU_DRAWMODE_OBJECTS_RANDOM_NOT_ZERO,
             window->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_AVERAGE ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeObjectsAverage,
             ID_MENU_DRAWMODE_OBJECTS_AVERAGE,
             window->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_AVERAGE );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_AVERAGE_NOT_ZERO ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeObjectsAverageNotZero,
             ID_MENU_DRAWMODE_OBJECTS_AVERAGE_NOT_ZERO,
             window->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_AVERAGENOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_MODE ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeObjectsMode,
             ID_MENU_DRAWMODE_OBJECTS_MODE,
             window->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_MODE );

  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_LAST ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeBothLast,
             ID_MENU_DRAWMODE_BOTH_LAST,
             window->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_LAST &&
             window->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_LAST );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_MAXIMUM ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeBothMaximum,
             ID_MENU_DRAWMODE_BOTH_MAXIMUM,
             window->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_MAXIMUM && 
             window->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_ABSOLUTE_MAXIMUM ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeBothAbsoluteMaximum,
             ID_MENU_DRAWMODE_BOTH_ABSOLUTE_MAXIMUM,
             window->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM &&
             window->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeBothMinimumNotZero,
             ID_MENU_DRAWMODE_BOTH_MINIMUM_NOT_ZERO,
             window->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_MINNOTZERO &&
             window->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_ABSOLUTE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeBothAbsoluteMinimumNotZero,
             ID_MENU_DRAWMODE_BOTH_ABSOLUTE_MINIMUM_NOT_ZERO,
             window->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO &&
             window->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_RANDOM ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeBothRandom,
             ID_MENU_DRAWMODE_BOTH_RANDOM,
             window->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_RANDOM &&
             window->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_RANDOM );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_RANDOM_NOT_ZERO ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeBothRandomNotZero,
             ID_MENU_DRAWMODE_BOTH_RANDOM_NOT_ZERO,
             window->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_RANDNOTZERO &&
             window->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_AVERAGE ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeBothAverage,
             ID_MENU_DRAWMODE_BOTH_AVERAGE,
             window->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_AVERAGE &&
             window->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_AVERAGE );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_AVERAGE_NOT_ZERO ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeBothAverageNotZero,
             ID_MENU_DRAWMODE_BOTH_AVERAGE_NOT_ZERO,
             window->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_AVERAGENOTZERO &&
             window->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_AVERAGENOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_MODE ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpDrawModeBothMode,
             ID_MENU_DRAWMODE_BOTH_MODE,
             window->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_MODE &&
             window->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_MODE );

  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeTime, _( "Time" ));
  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeObjects, _( "Objects" ));
  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeBoth, _( "Both" ));
  wxMenuItem *tmpDrawModeSubMenu = AppendSubMenu( popUpMenuDrawMode, _( "Drawmode" ));
  Enable( tmpDrawModeSubMenu->GetId(), !window->GetMyWindow()->isPunctualColorSet() || 
                                       window->GetMyWindow()->getPunctualColorWindow() != nullptr );

  buildItem( popUpMenuPixelSize,
             _( "x1" ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpPixelSize,
             ID_MENU_PIXEL_SIZE_x1,
             window->GetMyWindow()->getPixelSize() == 1 );
  buildItem( popUpMenuPixelSize,
             _( "x2" ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpPixelSize,
             ID_MENU_PIXEL_SIZE_x2,
             window->GetMyWindow()->getPixelSize() == 2 );
  buildItem( popUpMenuPixelSize,
             _( "x4" ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpPixelSize,
             ID_MENU_PIXEL_SIZE_x4,
             window->GetMyWindow()->getPixelSize() == 4 );
  buildItem( popUpMenuPixelSize,
             _( "x8" ),
             wxITEM_RADIO,
             &gTimeline::OnPopUpPixelSize,
             ID_MENU_PIXEL_SIZE_x8,
             window->GetMyWindow()->getPixelSize() == 8 );

  AppendSubMenu( popUpMenuPixelSize, _( "Pixel Size" ));

  buildItem( popUpMenuLabels, _( "All" ), wxITEM_RADIO, &gTimeline::OnPopUpLabels, ID_MENU_LABELS_ALL, 
             window->GetMyWindow()->getObjectLabels() == TObjectLabels::ALL_LABELS );
  buildItem( popUpMenuLabels, _( "Spaced" ), wxITEM_RADIO, &gTimeline::OnPopUpLabels, ID_MENU_LABELS_SPACED, 
             window->GetMyWindow()->getObjectLabels() ==  TObjectLabels::SPACED_LABELS );
  buildItem( popUpMenuLabels, _( "2^n" ), wxITEM_RADIO, &gTimeline::OnPopUpLabels, ID_MENU_LABELS_POWER2, 
             window->GetMyWindow()->getObjectLabels() == TObjectLabels::POWER2_LABELS );
  AppendSubMenu( popUpMenuLabels, _( "Object Labels" ) );

  buildItem( popUpMenuObjectAxis, _( "Fit Current Level" ), wxITEM_RADIO, &gTimeline::OnPopUpObjectAxis, ID_MENU_OBJECT_AXIS_CURRENT, 
             window->GetMyWindow()->getObjectAxisSize() == TObjectAxisSize::CURRENT_LEVEL );
  buildItem( popUpMenuObjectAxis, _( "Fit All Levels" ), wxITEM_RADIO, &gTimeline::OnPopUpObjectAxis, ID_MENU_OBJECT_AXIS_ALL, 
             window->GetMyWindow()->getObjectAxisSize() == TObjectAxisSize::ALL_LEVELS );
  buildItem( popUpMenuObjectAxis, _( "0%" ), wxITEM_RADIO, &gTimeline::OnPopUpObjectAxis, ID_MENU_OBJECT_AXIS_ZERO, 
             window->GetMyWindow()->getObjectAxisSize() == TObjectAxisSize::ZERO_PERC );
  buildItem( popUpMenuObjectAxis, _( "5%" ), wxITEM_RADIO, &gTimeline::OnPopUpObjectAxis, ID_MENU_OBJECT_AXIS_FIVE, 
             window->GetMyWindow()->getObjectAxisSize() == TObjectAxisSize::FIVE_PERC );
  buildItem( popUpMenuObjectAxis, _( "10%" ), wxITEM_RADIO, &gTimeline::OnPopUpObjectAxis, ID_MENU_OBJECT_AXIS_TEN, 
             window->GetMyWindow()->getObjectAxisSize() == TObjectAxisSize::TEN_PERC );
  buildItem( popUpMenuObjectAxis, _( "25%" ), wxITEM_RADIO, &gTimeline::OnPopUpObjectAxis, ID_MENU_OBJECT_AXIS_TWENTYFIVE, 
             window->GetMyWindow()->getObjectAxisSize() == TObjectAxisSize::TWENTYFIVE_PERC );
  AppendSubMenu( popUpMenuObjectAxis, _( "Object Axis" ) );

  AppendSeparator();
  
  buildItem( popUpMenuRun, _( "Cutter" ), wxITEM_NORMAL, &gTimeline::OnPopUpRunApp, ID_MENU_CUTTER );
  buildItem( popUpMenuRun, _( "Dimemas" ), wxITEM_NORMAL, &gTimeline::OnPopUpRunApp, ID_MENU_DIMEMAS );
  buildItem( popUpMenuRun, _( "Clustering" ), wxITEM_NORMAL, &gTimeline::OnPopUpRunApp, ID_MENU_CLUSTERING );
  buildItem( popUpMenuRun, _( "Folding" ), wxITEM_NORMAL, &gTimeline::OnPopUpRunApp, ID_MENU_FOLDING );
  //buildItem( popUpMenuRun, _( "Spectral" ), wxITEM_NORMAL, &gTimeline::OnPopUpRunApp, ID_MENU_SPECTRAL );
  buildItem( popUpMenuRun, _( "User Command" ), wxITEM_NORMAL, &gTimeline::OnPopUpRunApp, ID_MENU_USER_COMMAND );
  AppendSubMenu( popUpMenuRun, _( "Run" ) );
  
  AppendSeparator();

  vector< TGroupId > tmpGroups;
  SyncWindows::getInstance()->getGroups( tmpGroups );
  TGroupId i = 0;
  for( vector< TGroupId >::const_iterator itGroup = tmpGroups.begin(); itGroup != tmpGroups.end(); ++itGroup )
  {
    buildItem( popUpMenuSync, wxString::Format( _( "%u" ), *itGroup + 1 ), wxITEM_CHECK, &gTimeline::OnPopUpSynchronize,
               ID_MENU_SYNC_GROUP_BASE + i, window->GetMyWindow()->isSync() && window->GetMyWindow()->getSyncGroup() == *itGroup );
    ++i;
  }
  popUpMenuSync->AppendSeparator();
  buildItem( popUpMenuSync, _( STR_SYNC_NEW_GROUP ), wxITEM_NORMAL, &gTimeline::OnPopUpSynchronize,
             ID_MENU_NEWGROUP );

  i = 1;
  for( vector< TGroupId >::const_iterator itGroup = ++tmpGroups.begin(); itGroup != tmpGroups.end(); ++itGroup )
  {
    buildItem( popUpMenuSyncRemove, wxString::Format( _( "%u" ), *itGroup + 1 ), wxITEM_NORMAL, &gTimeline::OnPopUpRemoveGroup, ID_MENU_SYNC_REMOVE_GROUP_BASE + i );
    ++i;
  }
  popUpMenuSyncRemove->AppendSeparator();
  buildItem( popUpMenuSyncRemove, _( STR_SYNC_REMOVE_ALL_GROUPS ), wxITEM_NORMAL, &gTimeline::OnPopUpRemoveAllGroups, ID_MENU_SYNC_REMOVE_ALL_GROUPS );
  popUpMenuSync->AppendSubMenu( popUpMenuSyncRemove, _( STR_SYNC_REMOVE_GROUP ) );

  AppendSubMenu( popUpMenuSync, _( STR_SYNCHRONIZE ) );

  AppendSeparator();

  buildItem( popUpMenuSave, _( "Configuration..." ), wxITEM_NORMAL, &gTimeline::OnPopUpSaveCFG, ID_MENU_SAVE_TIMELINE_AS_CFG );
  buildItem( popUpMenuSave, _( "Image..." ), wxITEM_NORMAL, &gTimeline::OnPopUpSaveImageDialog, ID_MENU_SAVE_IMAGE );
  buildItem( popUpMenuSave, _( "Text..." ), wxITEM_NORMAL, &gTimeline::OnPopUpSaveText, ID_MENU_SAVE_TIMELINE_AS_TEXT );
  AppendSubMenu( popUpMenuSave, _( "Save" ) );

  AppendSeparator();

  buildItem( this,
             _( "Timing\tCTRL+T" ),
             wxITEM_CHECK,
             &gTimeline::OnPopUpTiming,
             ID_MENU_TIMING,
             window->GetTiming() );
  buildItem( this,
             _( "Info Panel" ),
             wxITEM_CHECK,
             &gTimeline::OnPopUpInfoPanel,
             ID_MENU_INFO_PANEL,
             window->IsSplit() );

  enableMenu( window );
}


template<>
gPopUpMenu<gHistogram>::gPopUpMenu( gHistogram *whichHistogram )
{
  window = whichHistogram;

  popUpMenuView = new wxMenu;
  popUpMenuColor = new wxMenu;
  popUpMenuPaste = new wxMenu;
  popUpMenuPasteFilter = new wxMenu;
  popUpMenuFitSemantic = new wxMenu;
  popUpMenuDrawMode = new wxMenu;
  popUpMenuDrawModeTime = new wxMenu;
  popUpMenuDrawModeObjects = new wxMenu;
  popUpMenuDrawModeBoth = new wxMenu;
  popUpMenuGradientFunction = new wxMenu;
  popUpMenuPixelSize = new wxMenu;
  popUpMenuSaveAsText = new wxMenu;
  popUpMenuColor2D = new wxMenu;
  popUpMenuSave = new wxMenu;
  popUpMenuSync = new wxMenu;
  popUpMenuSyncRemove = new wxMenu;
  
#ifdef __WXMAC__
  buildItem( this, _( STR_COPY ), wxITEM_NORMAL, &gHistogram::OnPopUpCopy, ID_MENU_COPY );
#else
  buildItem( this, _( STR_COPY ), wxITEM_NORMAL, &gHistogram::OnPopUpCopy, wxID_COPY );
#endif
  buildItem( popUpMenuPaste, _( STR_PASTE_DEFAULT_SPECIAL ), wxITEM_NORMAL, &gHistogram::OnPopUpPasteDefaultSpecial, ID_MENU_PASTE_DEFAULT_SPECIAL );
  popUpMenuPaste->AppendSeparator();
  buildItem( popUpMenuPaste, _( STR_TIME ), wxITEM_NORMAL, &gHistogram::OnPopUpPasteTime, ID_MENU_PASTE_TIME );
  buildItem( popUpMenuPaste, _( STR_OBJECTS ), wxITEM_NORMAL, &gHistogram::OnPopUpPasteObjects, ID_MENU_PASTE_OBJECTS );
  buildItem( popUpMenuPaste, _( STR_SIZE ), wxITEM_NORMAL, &gHistogram::OnPopUpPasteSize, ID_MENU_PASTE_SIZE );
  buildItem( popUpMenuPaste, _( STR_DURATION ), wxITEM_NORMAL, &gHistogram::OnPopUpPasteDuration, ID_MENU_PASTE_DURATION );
  buildItem( popUpMenuPaste, _( STR_PASTE_SEMANTIC_SORT ), wxITEM_NORMAL, &gHistogram::OnPopUpPasteSemanticSort, ID_MENU_PASTE_SEMANTIC_SORT );
  buildItem( popUpMenuPaste, _( STR_SEMANTIC_SCALE ), wxITEM_NORMAL, &gHistogram::OnPopUpPasteSemanticScale, ID_MENU_PASTE_SEMANTIC_SCALE );
  buildItem( popUpMenuPaste, _( STR_CONTROL_SCALE ), wxITEM_NORMAL, &gHistogram::OnPopUpPasteControlScale, ID_MENU_PASTE_CONTROL_SCALE );
  buildItem( popUpMenuPaste, _( STR_CONTROL_DIMENSIONS ), wxITEM_NORMAL, &gHistogram::OnPopUpPasteControlDimensions, ID_MENU_PASTE_CONTROL_DIMENSIONS );
  buildItem( popUpMenuPaste, _( STR_3D_SCALE ), wxITEM_NORMAL, &gHistogram::OnPopUpPaste3DScale, ID_MENU_PASTE_3D_SCALE );
  buildItem( popUpMenuPaste, _( STR_PASTE_SPECIAL ), wxITEM_NORMAL, &gHistogram::OnPopUpPasteSpecial, ID_MENU_PASTE_SPECIAL );

  AppendSubMenu( popUpMenuPaste, _( STR_PASTE ) );

  buildItem( this, _( STR_CLONE ), wxITEM_NORMAL, &gHistogram::OnPopUpClone, ID_MENU_CLONE );
  buildItem( this, _( STR_RENAME ), wxITEM_NORMAL, &gHistogram::OnPopUpRename, ID_MENU_RENAME );

  AppendSeparator();

  buildItem( this, _( STR_UNDO_ZOOM), wxITEM_NORMAL, &gHistogram::OnPopUpUndoZoom, wxID_UNDO );
  buildItem( this, _( STR_REDO_ZOOM ), wxITEM_NORMAL, &gHistogram::OnPopUpRedoZoom, wxID_REDO );

  
  buildItem( this, _( STR_FIT_TIME ), wxITEM_NORMAL, &gHistogram::OnPopUpFitTimeScale, wxID_ZOOM_100 );
  buildItem( this, _( STR_FIT_OBJECTS ), wxITEM_NORMAL, &gHistogram::OnPopUpFitObjects, ID_MENU_FIT_OBJECTS );
  buildItem( this, _( "Select Objects..." ), wxITEM_NORMAL, &gHistogram::OnPopUpRowSelection, ID_MENU_ROW_SELECTION );

  buildItem( this, 
             _( "Auto Fit Control Scale" ),
             wxITEM_CHECK,
             &gHistogram::OnPopUpAutoControlScale,
             ID_MENU_AUTO_CONTROL_SCALE,
             window->GetHistogram()->getCompute2DScale() );
  buildItem( this, 
             _( STR_AUTOFIT_CONTROL_ZERO ),
             wxITEM_CHECK,
             &gHistogram::OnPopUpAutoControlScaleZero,
             ID_MENU_AUTO_CONTROL_SCALE_ZERO,
             window->GetHistogram()->getCompute2DScaleZero() );
  if( window->GetHistogram()->getThreeDimensions() )
    buildItem( this, 
               _( "Auto Fit 3D Scale" ),
               wxITEM_CHECK,
               &gHistogram::OnPopUpAuto3DScale,
               ID_MENU_AUTO_3D_SCALE,
               window->GetHistogram()->getCompute3DScale() );
  buildItem( this, 
             _( "Auto Fit Data Gradient" ),
             wxITEM_CHECK,
             &gHistogram::OnPopUpAutoDataGradient,
             ID_MENU_AUTO_DATA_GRADIENT,
             window->GetHistogram()->getComputeGradient() );

  AppendSeparator();

  buildItem( popUpMenuColor2D, _( "Code Color" ), wxITEM_RADIO, &gHistogram::OnPopUpColor2D, ID_MENU_CODE_COLOR_2D, window->GetHistogram()->getColorMode() == TColorFunction::COLOR );
  buildItem( popUpMenuColor2D, _( "Gradient Color" ), wxITEM_RADIO, &gHistogram::OnPopUpColor2D, ID_MENU_GRADIENT_COLOR_2D, window->GetHistogram()->getColorMode() == TColorFunction::GRADIENT );
  buildItem( popUpMenuColor2D, _( "Not Null Gradient Color" ), wxITEM_RADIO, &gHistogram::OnPopUpColor2D, ID_MENU_NOT_NULL_GRADIENT_COLOR_2D, window->GetHistogram()->getColorMode() == TColorFunction::NOT_NULL_GRADIENT );
  AppendSubMenu( popUpMenuColor2D, _( "Color Mode" ) );
  
  buildItem( popUpMenuGradientFunction, _( "Linear" ), wxITEM_RADIO, &gHistogram::OnPopUpGradientFunction, ID_MENU_GRADIENT_FUNCTION_LINEAR, window->GetHistogram()->getGradientColor().getGradientFunction() == TGradientFunction::LINEAR );
  buildItem( popUpMenuGradientFunction, _( "Steps" ), wxITEM_RADIO, &gHistogram::OnPopUpGradientFunction, ID_MENU_GRADIENT_FUNCTION_STEPS, window->GetHistogram()->getGradientColor().getGradientFunction() == TGradientFunction::STEPS );
  buildItem( popUpMenuGradientFunction, _( "Logarithmic" ), wxITEM_RADIO, &gHistogram::OnPopUpGradientFunction, ID_MENU_GRADIENT_FUNCTION_LOGARITHMIC, window->GetHistogram()->getGradientColor().getGradientFunction() == TGradientFunction::LOGARITHMIC );
  buildItem( popUpMenuGradientFunction, _( "Exponential" ), wxITEM_RADIO, &gHistogram::OnPopUpGradientFunction, ID_MENU_GRADIENT_FUNCTION_EXPONENTIAL, window->GetHistogram()->getGradientColor().getGradientFunction() == TGradientFunction::EXPONENTIAL );
  AppendSubMenu( popUpMenuGradientFunction, _( "Gradient Function " ) );
  
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_LAST ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeSemanticLast,
             ID_MENU_DRAWMODE_TIME_LAST,
             window->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_LAST );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_MAXIMUM ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeSemanticMaximum,
             ID_MENU_DRAWMODE_TIME_MAXIMUM,
             window->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_ABSOLUTE_MAXIMUM ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeSemanticAbsoluteMaximum,
             ID_MENU_DRAWMODE_TIME_ABSOLUTE_MAXIMUM,
             window->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeSemanticMinimumNotZero,
             ID_MENU_DRAWMODE_TIME_MINIMUM_NOT_ZERO,
             window->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_ABSOLUTE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeSemanticAbsoluteMinimumNotZero,
             ID_MENU_DRAWMODE_TIME_ABSOLUTE_MINIMUM_NOT_ZERO,
             window->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO );
//----------------------------
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_RANDOM ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeSemanticRandom,
             ID_MENU_DRAWMODE_TIME_RANDOM,
             window->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_RANDOM );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_RANDOM_NOT_ZERO ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeSemanticRandomNotZero,
             ID_MENU_DRAWMODE_TIME_RANDOM_NOT_ZERO,
             window->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_AVERAGE ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeSemanticAverage,
             ID_MENU_DRAWMODE_TIME_AVERAGE,
             window->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_AVERAGE );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_AVERAGE_NOT_ZERO ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeSemanticAverageNotZero,
             ID_MENU_DRAWMODE_TIME_AVERAGE_NOT_ZERO,
             window->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_AVERAGENOTZERO );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_MODE ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeSemanticMode,
             ID_MENU_DRAWMODE_TIME_MODE,
             window->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_MODE );

  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_LAST ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeObjectsLast,
             ID_MENU_DRAWMODE_OBJECTS_LAST,
             window->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_LAST );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_MAXIMUM ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeObjectsMaximum,
             ID_MENU_DRAWMODE_OBJECTS_MAXIMUM,
             window->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_ABSOLUTE_MAXIMUM ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeObjectsAbsoluteMaximum,
             ID_MENU_DRAWMODE_OBJECTS_ABSOLUTE_MAXIMUM,
             window->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeObjectsMinimumNotZero,
             ID_MENU_DRAWMODE_OBJECTS_MINIMUM_NOT_ZERO,
             window->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_ABSOLUTE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeObjectsAbsoluteMinimumNotZero,
             ID_MENU_DRAWMODE_OBJECTS_ABSOLUTE_MINIMUM_NOT_ZERO,
             window->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_RANDOM ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeObjectsRandom,
             ID_MENU_DRAWMODE_OBJECTS_RANDOM,
             window->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_RANDOM );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_RANDOM_NOT_ZERO ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeObjectsRandomNotZero,
             ID_MENU_DRAWMODE_OBJECTS_RANDOM_NOT_ZERO,
             window->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_AVERAGE ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeObjectsAverage,
             ID_MENU_DRAWMODE_OBJECTS_AVERAGE,
             window->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_AVERAGE );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_AVERAGE_NOT_ZERO ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeObjectsAverageNotZero,
             ID_MENU_DRAWMODE_OBJECTS_AVERAGE_NOT_ZERO,
             window->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_AVERAGENOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_MODE ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeObjectsMode,
             ID_MENU_DRAWMODE_OBJECTS_MODE,
             window->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_MODE );

  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_LAST ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeBothLast,
             ID_MENU_DRAWMODE_BOTH_LAST,
             window->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_LAST &&
             window->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_LAST );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_MAXIMUM ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeBothMaximum,
             ID_MENU_DRAWMODE_BOTH_MAXIMUM,
             window->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_MAXIMUM && 
             window->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_ABSOLUTE_MAXIMUM ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeBothAbsoluteMaximum,
             ID_MENU_DRAWMODE_BOTH_ABSOLUTE_MAXIMUM,
             window->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM &&
             window->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeBothMinimumNotZero,
             ID_MENU_DRAWMODE_BOTH_MINIMUM_NOT_ZERO,
             window->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_MINNOTZERO &&
             window->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_ABSOLUTE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeBothAbsoluteMinimumNotZero,
             ID_MENU_DRAWMODE_BOTH_ABSOLUTE_MINIMUM_NOT_ZERO,
             window->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO &&
             window->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_RANDOM ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeBothRandom,
             ID_MENU_DRAWMODE_BOTH_RANDOM,
             window->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_RANDOM &&
             window->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_RANDOM );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_RANDOM_NOT_ZERO ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeBothRandomNotZero,
             ID_MENU_DRAWMODE_BOTH_RANDOM_NOT_ZERO,
             window->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_RANDNOTZERO &&
             window->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_AVERAGE ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeBothAverage,
             ID_MENU_DRAWMODE_BOTH_AVERAGE,
             window->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_AVERAGE &&
             window->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_AVERAGE );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_AVERAGE_NOT_ZERO ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeBothAverageNotZero,
             ID_MENU_DRAWMODE_BOTH_AVERAGE_NOT_ZERO,
             window->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_AVERAGENOTZERO &&
             window->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_AVERAGENOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_MODE ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpDrawModeBothMode,
             ID_MENU_DRAWMODE_BOTH_MODE,
             window->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_MODE &&
             window->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_MODE );
             
  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeTime, _( "Semantic" ));
  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeObjects, _( "Objects" ));
  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeBoth, _( "Both" ));
  AppendSubMenu( popUpMenuDrawMode, _( "Drawmode" ) );

  buildItem( popUpMenuPixelSize,
             _( "x1" ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpPixelSize,
             ID_MENU_PIXEL_SIZE_x1,
             window->GetHistogram()->getPixelSize() == 1 );
  buildItem( popUpMenuPixelSize,
             _( "x2" ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpPixelSize,
             ID_MENU_PIXEL_SIZE_x2,
             window->GetHistogram()->getPixelSize() == 2 );
  buildItem( popUpMenuPixelSize,
             _( "x4" ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpPixelSize,
             ID_MENU_PIXEL_SIZE_x4,
             window->GetHistogram()->getPixelSize() == 4 );
  buildItem( popUpMenuPixelSize,
             _( "x8" ),
             wxITEM_RADIO,
             &gHistogram::OnPopUpPixelSize,
             ID_MENU_PIXEL_SIZE_x8,
             window->GetHistogram()->getPixelSize() == 8 );

  AppendSubMenu( popUpMenuPixelSize, _( "Pixel Size" ));

  AppendSeparator();

  vector< TGroupId > tmpGroups;
  SyncWindows::getInstance()->getGroups( tmpGroups );
  TGroupId i = 0;
  for( vector< TGroupId >::const_iterator itGroup = tmpGroups.begin(); itGroup != tmpGroups.end(); ++itGroup )
  {
    buildItem( popUpMenuSync, wxString::Format( _( "%u" ), *itGroup + 1 ), wxITEM_CHECK, &gHistogram::OnPopUpSynchronize,
               ID_MENU_SYNC_GROUP_BASE + i, window->GetHistogram()->isSync() && window->GetHistogram()->getSyncGroup() == *itGroup );
    ++i;
  }
  popUpMenuSync->AppendSeparator();
  buildItem( popUpMenuSync, _( STR_SYNC_NEW_GROUP ), wxITEM_NORMAL, &gHistogram::OnPopUpSynchronize, ID_MENU_NEWGROUP );
  
  i = 1;
  for( vector< TGroupId >::const_iterator itGroup = ++tmpGroups.begin(); itGroup != tmpGroups.end(); ++itGroup )
  {
    buildItem( popUpMenuSyncRemove, wxString::Format( _( "%u" ), *itGroup + 1 ), wxITEM_NORMAL, &gHistogram::OnPopUpRemoveGroup, ID_MENU_SYNC_REMOVE_GROUP_BASE + i );
    ++i;
  }
  popUpMenuSyncRemove->AppendSeparator();
  buildItem( popUpMenuSyncRemove, _( STR_SYNC_REMOVE_ALL_GROUPS ), wxITEM_NORMAL, &gHistogram::OnPopUpRemoveAllGroups,
             ID_MENU_SYNC_REMOVE_ALL_GROUPS );

  popUpMenuSync->AppendSubMenu( popUpMenuSyncRemove, _( STR_SYNC_REMOVE_GROUP ) );
  
  AppendSubMenu( popUpMenuSync, _( STR_SYNCHRONIZE ) );

  AppendSeparator();

  buildItem( popUpMenuSave, _( "Configuration..." ), wxITEM_NORMAL, &gHistogram::OnPopUpSaveCFG, ID_MENU_SAVE_HISTOGRAM_AS_CFG );
  buildItem( popUpMenuSave, _( "Image..." ), wxITEM_NORMAL, &gHistogram::OnPopUpSaveImageDialog, ID_MENU_SAVE_IMAGE );

  if ( window->GetHistogram()->getThreeDimensions() )
  {
    buildItem( popUpMenuSaveAsText, 
               _( "Current Plane..." ),
               wxITEM_NORMAL,
               &gHistogram::OnPopUpSavePlaneAsText,
               ID_MENU_SAVE_CURRENT_PLANE_AS_TEXT );
    buildItem( popUpMenuSaveAsText, _( "All Planes..." ),
               wxITEM_NORMAL,
               &gHistogram::OnPopUpSavePlaneAsText,
               ID_MENU_SAVE_ALL_PLANES_AS_TEXT );
    popUpMenuSave->AppendSubMenu( popUpMenuSaveAsText, _( "Text..." ) );

  }
  else
    buildItem( popUpMenuSave, _( "Text..." ), wxITEM_NORMAL, &gHistogram::OnPopUpSavePlaneAsText, ID_MENU_SAVE_ALL_PLANES_AS_TEXT );

  AppendSubMenu( popUpMenuSave, _( "Save" ) );
  
  enableMenu( window );
}
