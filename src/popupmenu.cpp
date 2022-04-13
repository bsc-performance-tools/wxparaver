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
#include "copypaste.h"
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


wxMenuItem *gPopUpMenu::buildItem( wxMenu *popUp,
                                   const wxString &title,
                                   wxItemKind itemType,
                                   wxObjectEventFunction handler,
                                   wxWindowID id,
                                   bool checked )
{
  wxMenuItem *tmp;

  tmp = new wxMenuItem( popUp, id, title, _( "" ), itemType );

  popUp->Append( tmp );
  if ( tmp->IsCheckable() )
    tmp->Check( checked );
    
  if( handler != nullptr )
#ifdef _WIN32
    Connect( id,
#else
    popUp->Connect( id,
#endif
                    wxEVT_COMMAND_MENU_SELECTED, 
                    handler,
                    nullptr,
                    this );
                    
  return tmp;
}


void gPopUpMenu::enableMenu( gTimeline *whichTimeline )
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


void gPopUpMenu::enableMenu( gHistogram *whichHistogram )
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
  Enable( FindItem( _( STR_CONTROL_DIMENSIONS ) ), sharedProperties->isAllowed( whichHistogram, STR_CONTROL_DIMENSIONS ) );
  if( whichHistogram->GetHistogram()->getThreeDimensions() &&
      histogram != nullptr && histogram->GetHistogram()->getThreeDimensions() )
    Enable( FindItem( _( STR_3D_SCALE ) ), sharedProperties->isAllowed( whichHistogram, STR_3D_SCALE) );
  else
    Enable( FindItem( _( STR_3D_SCALE ) ), false );

  Enable( FindItem( _( STR_AUTOFIT_CONTROL_ZERO ) ), whichHistogram->GetHistogram()->getCompute2DScale() );

  Enable( FindItem( _( "Image..." ) ), whichHistogram->GetHistogram()->getZoom() );
}


wxMultiChoiceDialog *gPopUpMenu::createPasteSpecialDialog( wxArrayString& choices, gHistogram *whichHistogram )
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


wxMultiChoiceDialog *gPopUpMenu::createPasteSpecialDialog( wxArrayString& choices, gTimeline *whichTimeline )
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


RowsSelectionDialog *gPopUpMenu::createRowSelectionDialog( gTimeline *whichTimeline )
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


RowsSelectionDialog *gPopUpMenu::createRowSelectionDialog( gHistogram *histogram )
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


gPopUpMenu::gPopUpMenu( gTimeline *whichTimeline )
{
  timeline = whichTimeline;
  histogram = nullptr;

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
  buildItem( this, _( STR_COPY ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuCopy, ID_MENU_COPY );
#else
  buildItem( this, _( STR_COPY ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuCopy, wxID_COPY );
#endif
  buildItem( popUpMenuPaste, _( STR_PASTE_DEFAULT_SPECIAL ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuPasteDefaultSpecial, ID_MENU_PASTE_DEFAULT_SPECIAL );
  popUpMenuPaste->AppendSeparator();
  buildItem( popUpMenuPaste, _( STR_TIME ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuTime, ID_MENU_PASTE_TIME );
  buildItem( popUpMenuPaste, _( STR_OBJECTS ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuObjects, ID_MENU_PASTE_OBJECTS );
  buildItem( popUpMenuPaste, _( STR_SIZE ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuSize, ID_MENU_PASTE_SIZE );
  buildItem( popUpMenuPaste, _( STR_DURATION ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuDuration, ID_MENU_PASTE_DURATION );

  buildItem( popUpMenuPaste, _( STR_SEMANTIC_SCALE ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuSemanticScale, ID_MENU_PASTE_SEMANTIC_SCALE );

  buildItem( popUpMenuPaste, _( STR_CUSTOM_PALETTE ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuCustomPalette, ID_MENU_PASTE_CUSTOM_PALETTE );

  buildItem( popUpMenuPasteFilter, _( STR_FILTER_ALL ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFilterAll, ID_MENU_PASTE_FILTER_ALL );
  buildItem( popUpMenuPasteFilter, _( STR_FILTER_COMMS ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFilterComms, ID_MENU_PASTE_FILTER_COMMS );
  buildItem( popUpMenuPasteFilter, _( STR_FILTER_EVENTS ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFilterEvents, ID_MENU_PASTE_FILTER_EVENTS );
  popUpMenuPaste->AppendSubMenu( popUpMenuPasteFilter, _( STR_FILTER ) );

  buildItem( popUpMenuPaste, _( STR_PASTE_SPECIAL ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuPasteSpecial, ID_MENU_PASTE_SPECIAL );

  AppendSubMenu( popUpMenuPaste, _( STR_PASTE ) );

  buildItem( this, _( STR_CLONE ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuClone, ID_MENU_CLONE );
  buildItem( this, _( STR_RENAME ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuRename, ID_MENU_RENAME );

  AppendSeparator();

  buildItem( this, _( STR_UNDO_ZOOM ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuUndoZoom, wxID_UNDO );
  buildItem( this, _( STR_REDO_ZOOM ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuRedoZoom, wxID_REDO );

  buildItem( this, _( STR_FIT_TIME ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFitTime, wxID_ZOOM_100 );
  buildItem( popUpMenuFitSemantic, _( "Fit Minimum" ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFitSemanticMin, ID_MENU_FIT_SEMANTIC_MIN);
  buildItem( popUpMenuFitSemantic, _( "Fit Maximum" ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFitSemanticMax, ID_MENU_FIT_SEMANTIC_MAX);
  buildItem( popUpMenuFitSemantic, _( "Fit Both" ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFitSemanticBoth, ID_MENU_FIT_SEMANTIC_BOTH);
  AppendSubMenu( popUpMenuFitSemantic, _( STR_FIT_SEMANTIC ) );

  buildItem( this, _( STR_FIT_OBJECTS ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFitObjects, ID_MENU_FIT_OBJECTS );
  buildItem( this, _( "Select Objects..." ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuRowSelection, ID_MENU_ROW_SELECTION );

  AppendSeparator();

  buildItem( popUpMenuView, _( "Communication Lines" ), wxITEM_CHECK, (wxObjectEventFunction)&gPopUpMenu::OnMenuViewCommunicationLines, ID_MENU_VIEW_COMMUNICATION_LINES, timeline->GetMyWindow()->getDrawCommLines() );
  buildItem( popUpMenuView, _( "Event Flags" ), wxITEM_CHECK, (wxObjectEventFunction)&gPopUpMenu::OnMenuViewEventFlags, ID_MENU_VIEW_EVENT_FLAGS, timeline->GetMyWindow()->getDrawFlags() );
  AppendSubMenu( popUpMenuView, _( "View" ));

  buildItem( popUpMenuColor, _( "Function Line" ), wxITEM_RADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuViewFunctionLine, ID_MENU_VIEW_FUNCTION_LINE, timeline->GetMyWindow()->isFunctionLineColorSet() );
  buildItem( popUpMenuColor, _( "Fused Lines" ), wxITEM_RADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuViewFusedLines, ID_MENU_VIEW_FUSED_LINES, timeline->GetMyWindow()->isFusedLinesColorSet() );
  buildItem( popUpMenuColor, _( "Punctual" ), wxITEM_RADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuPunctual, ID_MENU_PUNCTUAL, timeline->GetMyWindow()->isPunctualColorSet() );
  buildItem( popUpMenuColor, _( "Code Color" ), wxITEM_RADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuCodeColor, ID_MENU_CODE_COLOR, timeline->GetMyWindow()->isCodeColorSet() );
  buildItem( popUpMenuColor, _( "Gradient Color" ), wxITEM_RADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuGradientColor,ID_MENU_GRADIENT_COLOR, timeline->GetMyWindow()->isGradientColorSet() );
  buildItem( popUpMenuColor, _( "Not Null Gradient Color" ), wxITEM_RADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuNotNullGradientColor,ID_MENU_NOT_NULL_GRADIENT_COLOR, timeline->GetMyWindow()->isNotNullGradientColorSet() );

  popUpMenuColor->AppendSeparator();

  wxMenuItem *tmpPuncWin = buildItem( popUpMenuColor, _( "Punctual Window..." ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuPunctualWindow, ID_MENU_PUNCTUAL_WINDOW );
  popUpMenuColor->Enable( tmpPuncWin->GetId(), timeline->GetMyWindow()->isPunctualColorSet() );

  buildItem( popUpMenuGradientFunction, _( "Linear" ), wxITEM_RADIO,(wxObjectEventFunction)&gPopUpMenu::OnMenuGradientFunction, ID_MENU_GRADIENT_FUNCTION_LINEAR, timeline->GetMyWindow()->getGradientColor().getGradientFunction() == TGradientFunction::LINEAR );
  buildItem( popUpMenuGradientFunction, _( "Steps" ), wxITEM_RADIO,(wxObjectEventFunction)&gPopUpMenu::OnMenuGradientFunction, ID_MENU_GRADIENT_FUNCTION_STEPS, timeline->GetMyWindow()->getGradientColor().getGradientFunction() == TGradientFunction::STEPS );
  buildItem( popUpMenuGradientFunction, _( "Logarithmic" ), wxITEM_RADIO,(wxObjectEventFunction)&gPopUpMenu::OnMenuGradientFunction, ID_MENU_GRADIENT_FUNCTION_LOGARITHMIC, timeline->GetMyWindow()->getGradientColor().getGradientFunction() == TGradientFunction::LOGARITHMIC );
  buildItem( popUpMenuGradientFunction, _( "Exponential" ), wxITEM_RADIO,(wxObjectEventFunction)&gPopUpMenu::OnMenuGradientFunction, ID_MENU_GRADIENT_FUNCTION_EXPONENTIAL, timeline->GetMyWindow()->getGradientColor().getGradientFunction() == TGradientFunction::EXPONENTIAL );
  wxMenuItem *tmpGradFunc = popUpMenuColor->AppendSubMenu( popUpMenuGradientFunction, _( "Gradient Function" ) );
  popUpMenuColor->Enable( tmpGradFunc->GetId(), timeline->GetMyWindow()->isGradientColorSet() ||
                                                timeline->GetMyWindow()->isNotNullGradientColorSet() ||
                                                timeline->GetMyWindow()->isFunctionLineColorSet() ||
                                                timeline->GetMyWindow()->isPunctualColorSet() );

  wxMenuItem *tmpSemScaleMinZero = buildItem( popUpMenuColor, _( "Semantic scale min at 0" ), wxITEM_CHECK, (wxObjectEventFunction)&gPopUpMenu::OnMenuSemanticScaleMinAtZero, ID_MENU_SEMANTIC_SCALE_MIN_AT_ZERO, timeline->GetMyWindow()->getSemanticScaleMinAtZero() );
  popUpMenuColor->Enable( tmpSemScaleMinZero->GetId(), timeline->GetMyWindow()->isFunctionLineColorSet() ||
                                                       timeline->GetMyWindow()->isFusedLinesColorSet() ||
                                                       timeline->GetMyWindow()->isPunctualColorSet() );

  AppendSubMenu( popUpMenuColor, _( "Paint As" ));


  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_LAST ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeLast,
             ID_MENU_DRAWMODE_TIME_LAST,
             timeline->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_LAST );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_MAXIMUM ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeMaximum,
             ID_MENU_DRAWMODE_TIME_MAXIMUM,
             timeline->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_ABSOLUTE_MAXIMUM ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeAbsoluteMaximum,
             ID_MENU_DRAWMODE_TIME_ABSOLUTE_MAXIMUM,
             timeline->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeMinimumNotZero,
             ID_MENU_DRAWMODE_TIME_MINIMUM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_ABSOLUTE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeAbsoluteMinimumNotZero,
             ID_MENU_DRAWMODE_TIME_ABSOLUTE_MINIMUM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO);
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_RANDOM ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeRandom,
             ID_MENU_DRAWMODE_TIME_RANDOM,
             timeline->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_RANDOM );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_RANDOM_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeRandomNotZero,
             ID_MENU_DRAWMODE_TIME_RANDOM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_AVERAGE ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeAverage,
             ID_MENU_DRAWMODE_TIME_AVERAGE,
             timeline->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_AVERAGE );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_AVERAGE_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeAverageNotZero,
             ID_MENU_DRAWMODE_TIME_AVERAGE_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_AVERAGENOTZERO );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_MODE ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeMode,
             ID_MENU_DRAWMODE_TIME_MODE,
             timeline->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_MODE );

  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_LAST ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsLast,
             ID_MENU_DRAWMODE_OBJECTS_LAST,
             timeline->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_LAST );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_MAXIMUM ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsMaximum,
             ID_MENU_DRAWMODE_OBJECTS_MAXIMUM,
             timeline->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_ABSOLUTE_MAXIMUM ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsAbsoluteMaximum,
             ID_MENU_DRAWMODE_OBJECTS_ABSOLUTE_MAXIMUM,
             timeline->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsMinimumNotZero,
             ID_MENU_DRAWMODE_OBJECTS_MINIMUM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_ABSOLUTE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsAbsoluteMinimumNotZero,
             ID_MENU_DRAWMODE_OBJECTS_ABSOLUTE_MINIMUM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_RANDOM ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsRandom,
             ID_MENU_DRAWMODE_OBJECTS_RANDOM,
             timeline->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_RANDOM );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_RANDOM_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsRandomNotZero,
             ID_MENU_DRAWMODE_OBJECTS_RANDOM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_AVERAGE ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsAverage,
             ID_MENU_DRAWMODE_OBJECTS_AVERAGE,
             timeline->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_AVERAGE );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_AVERAGE_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsAverageNotZero,
             ID_MENU_DRAWMODE_OBJECTS_AVERAGE_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_AVERAGENOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_MODE ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsMode,
             ID_MENU_DRAWMODE_OBJECTS_MODE,
             timeline->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_MODE );

  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_LAST ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothLast,
             ID_MENU_DRAWMODE_BOTH_LAST,
             timeline->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_LAST &&
             timeline->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_LAST );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_MAXIMUM ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothMaximum,
             ID_MENU_DRAWMODE_BOTH_MAXIMUM,
             timeline->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_MAXIMUM && 
             timeline->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_ABSOLUTE_MAXIMUM ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothAbsoluteMaximum,
             ID_MENU_DRAWMODE_BOTH_ABSOLUTE_MAXIMUM,
             timeline->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM &&
             timeline->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothMinimumNotZero,
             ID_MENU_DRAWMODE_BOTH_MINIMUM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_MINNOTZERO &&
             timeline->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_ABSOLUTE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothAbsoluteMinimumNotZero,
             ID_MENU_DRAWMODE_BOTH_ABSOLUTE_MINIMUM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO &&
             timeline->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_RANDOM ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothRandom,
             ID_MENU_DRAWMODE_BOTH_RANDOM,
             timeline->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_RANDOM &&
             timeline->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_RANDOM );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_RANDOM_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothRandomNotZero,
             ID_MENU_DRAWMODE_BOTH_RANDOM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_RANDNOTZERO &&
             timeline->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_AVERAGE ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothAverage,
             ID_MENU_DRAWMODE_BOTH_AVERAGE,
             timeline->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_AVERAGE &&
             timeline->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_AVERAGE );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_AVERAGE_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothAverageNotZero,
             ID_MENU_DRAWMODE_BOTH_AVERAGE_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_AVERAGENOTZERO &&
             timeline->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_AVERAGENOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_MODE ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothMode,
             ID_MENU_DRAWMODE_BOTH_MODE,
             timeline->GetMyWindow()->getDrawModeObject() == DrawModeMethod::DRAW_MODE &&
             timeline->GetMyWindow()->getDrawModeTime() == DrawModeMethod::DRAW_MODE );

  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeTime, _( "Time" ));
  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeObjects, _( "Objects" ));
  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeBoth, _( "Both" ));
  wxMenuItem *tmpDrawModeSubMenu = AppendSubMenu( popUpMenuDrawMode, _( "Drawmode" ));
  Enable( tmpDrawModeSubMenu->GetId(), !timeline->GetMyWindow()->isPunctualColorSet() || 
                                       timeline->GetMyWindow()->getPunctualColorWindow() != nullptr );

  buildItem( popUpMenuPixelSize,
             _( "x1" ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuPixelSize,
             ID_MENU_PIXEL_SIZE_x1,
             timeline->GetMyWindow()->getPixelSize() == 1 );
  buildItem( popUpMenuPixelSize,
             _( "x2" ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuPixelSize,
             ID_MENU_PIXEL_SIZE_x2,
             timeline->GetMyWindow()->getPixelSize() == 2 );
  buildItem( popUpMenuPixelSize,
             _( "x4" ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuPixelSize,
             ID_MENU_PIXEL_SIZE_x4,
             timeline->GetMyWindow()->getPixelSize() == 4 );
  buildItem( popUpMenuPixelSize,
             _( "x8" ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuPixelSize,
             ID_MENU_PIXEL_SIZE_x8,
             timeline->GetMyWindow()->getPixelSize() == 8 );

  AppendSubMenu( popUpMenuPixelSize, _( "Pixel Size" ));

  buildItem( popUpMenuLabels, _( "All" ), wxITEM_RADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuLabelsAll, ID_MENU_LABELS_ALL, 
             timeline->GetMyWindow()->getObjectLabels() == TObjectLabels::ALL_LABELS );
  buildItem( popUpMenuLabels, _( "Spaced" ), wxITEM_RADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuLabelsSpaced, ID_MENU_LABELS_SPACED, 
             timeline->GetMyWindow()->getObjectLabels() ==  TObjectLabels::SPACED_LABELS );
  buildItem( popUpMenuLabels, _( "2^n" ), wxITEM_RADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuLabelsPower2, ID_MENU_LABELS_POWER2, 
             timeline->GetMyWindow()->getObjectLabels() == TObjectLabels::POWER2_LABELS );
  AppendSubMenu( popUpMenuLabels, _( "Object Labels" ) );

  buildItem( popUpMenuObjectAxis, _( "Fit Current Level" ), wxITEM_RADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuObjectAxisCurrent, ID_MENU_OBJECT_AXIS_CURRENT, 
             timeline->GetMyWindow()->getObjectAxisSize() == TObjectAxisSize::CURRENT_LEVEL );
  buildItem( popUpMenuObjectAxis, _( "Fit All Levels" ), wxITEM_RADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuObjectAxisAll, ID_MENU_OBJECT_AXIS_ALL, 
             timeline->GetMyWindow()->getObjectAxisSize() == TObjectAxisSize::ALL_LEVELS );
  buildItem( popUpMenuObjectAxis, _( "0%" ), wxITEM_RADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuObjectAxisZero, ID_MENU_OBJECT_AXIS_ZERO, 
             timeline->GetMyWindow()->getObjectAxisSize() == TObjectAxisSize::ZERO_PERC );
  buildItem( popUpMenuObjectAxis, _( "5%" ), wxITEM_RADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuObjectAxisFive, ID_MENU_OBJECT_AXIS_FIVE, 
             timeline->GetMyWindow()->getObjectAxisSize() == TObjectAxisSize::FIVE_PERC );
  buildItem( popUpMenuObjectAxis, _( "10%" ), wxITEM_RADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuObjectAxisTen, ID_MENU_OBJECT_AXIS_TEN, 
             timeline->GetMyWindow()->getObjectAxisSize() == TObjectAxisSize::TEN_PERC );
  buildItem( popUpMenuObjectAxis, _( "25%" ), wxITEM_RADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuObjectAxisTwentyFive, ID_MENU_OBJECT_AXIS_TWENTYFIVE, 
             timeline->GetMyWindow()->getObjectAxisSize() == TObjectAxisSize::TWENTYFIVE_PERC );
  AppendSubMenu( popUpMenuObjectAxis, _( "Object Axis" ) );

  AppendSeparator();
  
  buildItem( popUpMenuRun, _( "Cutter" ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuCutter, ID_MENU_CUTTER );
  buildItem( popUpMenuRun, _( "Dimemas" ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuDimemas, ID_MENU_DIMEMAS );
  buildItem( popUpMenuRun, _( "Clustering" ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuClustering, ID_MENU_CLUSTERING );
  buildItem( popUpMenuRun, _( "Folding" ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFolding, ID_MENU_FOLDING );
  //buildItem( popUpMenuRun, _( "Spectral" ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuSpectral, ID_MENU_SPECTRAL );
  buildItem( popUpMenuRun, _( "User Command" ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuUserCommand, ID_MENU_USER_COMMAND );
  AppendSubMenu( popUpMenuRun, _( "Run" ) );
  
  AppendSeparator();

  vector< TGroupId > tmpGroups;
  SyncWindows::getInstance()->getGroups( tmpGroups );
  TGroupId i = 0;
  for( vector< TGroupId >::const_iterator itGroup = tmpGroups.begin(); itGroup != tmpGroups.end(); ++itGroup )
  {
    buildItem( popUpMenuSync, wxString::Format( _( "%u" ), *itGroup + 1 ), wxITEM_CHECK, (wxObjectEventFunction)&gPopUpMenu::OnMenuSynchronize,
               ID_MENU_SYNC_GROUP_BASE + i, timeline->GetMyWindow()->isSync() && timeline->GetMyWindow()->getSyncGroup() == *itGroup );
    ++i;
  }
  popUpMenuSync->AppendSeparator();
  buildItem( popUpMenuSync, _( STR_SYNC_NEW_GROUP ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuSynchronize,
             ID_MENU_NEWGROUP );

  i = 1;
  for( vector< TGroupId >::const_iterator itGroup = ++tmpGroups.begin(); itGroup != tmpGroups.end(); ++itGroup )
  {
    buildItem( popUpMenuSyncRemove, wxString::Format( _( "%u" ), *itGroup + 1 ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuRemoveGroup, ID_MENU_SYNC_REMOVE_GROUP_BASE + i );
    ++i;
  }
  popUpMenuSyncRemove->AppendSeparator();
  buildItem( popUpMenuSyncRemove, _( STR_SYNC_REMOVE_ALL_GROUPS ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuRemoveAllGroups,
             ID_MENU_SYNC_REMOVE_ALL_GROUPS );
  popUpMenuSync->AppendSubMenu( popUpMenuSyncRemove, _( STR_SYNC_REMOVE_GROUP ) );

  AppendSubMenu( popUpMenuSync, _( STR_SYNCHRONIZE ) );

  AppendSeparator();

  buildItem( popUpMenuSave, _( "Configuration..." ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuSaveTimelineAsCFG, ID_MENU_SAVE_TIMELINE_AS_CFG );
  buildItem( popUpMenuSave, _( "Image..." ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuSaveImage, ID_MENU_SAVE_IMAGE );
  buildItem( popUpMenuSave, _( "Text..." ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuSaveTimelineAsText, ID_MENU_SAVE_TIMELINE_AS_TEXT );
  AppendSubMenu( popUpMenuSave, _( "Save" ) );

  AppendSeparator();

  buildItem( this,
             _( "Timing\tCTRL+T" ),
             wxITEM_CHECK,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuTiming,
             ID_MENU_TIMING,
             timeline->GetTiming() );
  buildItem( this,
             _( "Info Panel" ),
             wxITEM_CHECK,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuInfoPanel,
             ID_MENU_INFO_PANEL,
             timeline->IsSplit() );

  enableMenu( timeline );
}


gPopUpMenu::gPopUpMenu( gHistogram *whichHistogram )
{
  timeline = nullptr;
  histogram = whichHistogram;

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
  buildItem( this, _( STR_COPY ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuCopy, ID_MENU_COPY );
#else
  buildItem( this, _( STR_COPY ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuCopy, wxID_COPY );
#endif
  buildItem( popUpMenuPaste, _( STR_PASTE_DEFAULT_SPECIAL ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuPasteDefaultSpecial, ID_MENU_PASTE_DEFAULT_SPECIAL );
  popUpMenuPaste->AppendSeparator();
  buildItem( popUpMenuPaste, _( STR_TIME ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuTime, ID_MENU_PASTE_TIME );
  buildItem( popUpMenuPaste, _( STR_OBJECTS ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuObjects, ID_MENU_PASTE_OBJECTS );
  buildItem( popUpMenuPaste, _( STR_SIZE ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuSize, ID_MENU_PASTE_SIZE );
  buildItem( popUpMenuPaste, _( STR_DURATION ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuDuration, ID_MENU_PASTE_DURATION );
  buildItem( popUpMenuPaste, _( STR_SEMANTIC_SCALE ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuSemanticScale, ID_MENU_PASTE_SEMANTIC_SCALE );
  buildItem( popUpMenuPaste, _( STR_CONTROL_SCALE ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuPasteControlScale, ID_MENU_PASTE_CONTROL_SCALE );
  buildItem( popUpMenuPaste, _( STR_CONTROL_DIMENSIONS ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuPasteControlDimensions, ID_MENU_PASTE_CONTROL_DIMENSIONS );
  buildItem( popUpMenuPaste, _( STR_3D_SCALE ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuPaste3DScale, ID_MENU_PASTE_3D_SCALE );
  buildItem( popUpMenuPaste, _( STR_PASTE_SPECIAL ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuPasteSpecial, ID_MENU_PASTE_SPECIAL );

  AppendSubMenu( popUpMenuPaste, _( STR_PASTE ) );

  buildItem( this, _( STR_CLONE ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuClone, ID_MENU_CLONE );
  buildItem( this, _( STR_RENAME ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuRename, ID_MENU_RENAME );

  AppendSeparator();

  buildItem( this, _( STR_UNDO_ZOOM), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuUndoZoom, wxID_UNDO );
  buildItem( this, _( STR_REDO_ZOOM ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuRedoZoom, wxID_REDO );

  
  buildItem( this, _( STR_FIT_TIME ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFitTime, wxID_ZOOM_100 );
  buildItem( this, _( STR_FIT_OBJECTS ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFitObjects, ID_MENU_FIT_OBJECTS );
  buildItem( this, _( "Select Objects..." ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuRowSelection, ID_MENU_ROW_SELECTION );

  buildItem( this, 
             _( "Auto Fit Control Scale" ),
             wxITEM_CHECK,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuAutoControlScale,
             ID_MENU_AUTO_CONTROL_SCALE,
             histogram->GetHistogram()->getCompute2DScale() );
  buildItem( this, 
             _( STR_AUTOFIT_CONTROL_ZERO ),
             wxITEM_CHECK,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuAutoControlScaleZero,
             ID_MENU_AUTO_CONTROL_SCALE_ZERO,
             histogram->GetHistogram()->getCompute2DScaleZero() );
  if( histogram->GetHistogram()->getThreeDimensions() )
    buildItem( this, 
               _( "Auto Fit 3D Scale" ),
               wxITEM_CHECK,
               (wxObjectEventFunction)&gPopUpMenu::OnMenuAuto3DScale,
               ID_MENU_AUTO_3D_SCALE,
               histogram->GetHistogram()->getCompute3DScale() );
  buildItem( this, 
             _( "Auto Fit Data Gradient" ),
             wxITEM_CHECK,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuAutoDataGradient,
             ID_MENU_AUTO_DATA_GRADIENT,
             histogram->GetHistogram()->getComputeGradient() );

  AppendSeparator();

  buildItem( popUpMenuColor2D, _( "Code Color" ), wxITEM_RADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuCodeColor2D, ID_MENU_CODE_COLOR_2D, histogram->GetHistogram()->getColorMode() == TColorFunction::COLOR );
  buildItem( popUpMenuColor2D, _( "Gradient Color" ), wxITEM_RADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuGradientColor2D, ID_MENU_GRADIENT_COLOR_2D, histogram->GetHistogram()->getColorMode() == TColorFunction::GRADIENT );
  buildItem( popUpMenuColor2D, _( "Not Null Gradient Color" ), wxITEM_RADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuNotNullGradientColor2D, ID_MENU_NOT_NULL_GRADIENT_COLOR_2D, histogram->GetHistogram()->getColorMode() == TColorFunction::NOT_NULL_GRADIENT );
  AppendSubMenu( popUpMenuColor2D, _( "Color Mode" ) );
  
  buildItem( popUpMenuGradientFunction, _( "Linear" ), wxITEM_RADIO,(wxObjectEventFunction)&gPopUpMenu::OnMenuGradientFunction, ID_MENU_GRADIENT_FUNCTION_LINEAR, histogram->GetHistogram()->getGradientColor().getGradientFunction() == TGradientFunction::LINEAR );
  buildItem( popUpMenuGradientFunction, _( "Steps" ), wxITEM_RADIO,(wxObjectEventFunction)&gPopUpMenu::OnMenuGradientFunction, ID_MENU_GRADIENT_FUNCTION_STEPS, histogram->GetHistogram()->getGradientColor().getGradientFunction() == TGradientFunction::STEPS );
  buildItem( popUpMenuGradientFunction, _( "Logarithmic" ), wxITEM_RADIO,(wxObjectEventFunction)&gPopUpMenu::OnMenuGradientFunction, ID_MENU_GRADIENT_FUNCTION_LOGARITHMIC, histogram->GetHistogram()->getGradientColor().getGradientFunction() == TGradientFunction::LOGARITHMIC );
  buildItem( popUpMenuGradientFunction, _( "Exponential" ), wxITEM_RADIO,(wxObjectEventFunction)&gPopUpMenu::OnMenuGradientFunction, ID_MENU_GRADIENT_FUNCTION_EXPONENTIAL, histogram->GetHistogram()->getGradientColor().getGradientFunction() == TGradientFunction::EXPONENTIAL );
  AppendSubMenu( popUpMenuGradientFunction, _( "Gradient Function " ) );
  
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_LAST ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeLast,
             ID_MENU_DRAWMODE_TIME_LAST,
             histogram->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_LAST );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_MAXIMUM ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeMaximum,
             ID_MENU_DRAWMODE_TIME_MAXIMUM,
             histogram->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_ABSOLUTE_MAXIMUM ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeAbsoluteMaximum,
             ID_MENU_DRAWMODE_TIME_ABSOLUTE_MAXIMUM,
             histogram->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeMinimumNotZero,
             ID_MENU_DRAWMODE_TIME_MINIMUM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_ABSOLUTE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeAbsoluteMinimumNotZero,
             ID_MENU_DRAWMODE_TIME_ABSOLUTE_MINIMUM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO );
//----------------------------
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_RANDOM ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeRandom,
             ID_MENU_DRAWMODE_TIME_RANDOM,
             histogram->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_RANDOM );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_RANDOM_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeRandomNotZero,
             ID_MENU_DRAWMODE_TIME_RANDOM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_AVERAGE ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeAverage,
             ID_MENU_DRAWMODE_TIME_AVERAGE,
             histogram->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_AVERAGE );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_AVERAGE_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeAverageNotZero,
             ID_MENU_DRAWMODE_TIME_AVERAGE_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_AVERAGENOTZERO );
  buildItem( popUpMenuDrawModeTime,
             _( GUI_DRAWMODE_MODE ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeMode,
             ID_MENU_DRAWMODE_TIME_MODE,
             histogram->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_MODE );

  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_LAST ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsLast,
             ID_MENU_DRAWMODE_OBJECTS_LAST,
             histogram->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_LAST );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_MAXIMUM ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsMaximum,
             ID_MENU_DRAWMODE_OBJECTS_MAXIMUM,
             histogram->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_ABSOLUTE_MAXIMUM ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsAbsoluteMaximum,
             ID_MENU_DRAWMODE_OBJECTS_ABSOLUTE_MAXIMUM,
             histogram->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsMinimumNotZero,
             ID_MENU_DRAWMODE_OBJECTS_MINIMUM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_ABSOLUTE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsAbsoluteMinimumNotZero,
             ID_MENU_DRAWMODE_OBJECTS_ABSOLUTE_MINIMUM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_RANDOM ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsRandom,
             ID_MENU_DRAWMODE_OBJECTS_RANDOM,
             histogram->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_RANDOM );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_RANDOM_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsRandomNotZero,
             ID_MENU_DRAWMODE_OBJECTS_RANDOM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_AVERAGE ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsAverage,
             ID_MENU_DRAWMODE_OBJECTS_AVERAGE,
             histogram->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_AVERAGE );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_AVERAGE_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsAverageNotZero,
             ID_MENU_DRAWMODE_OBJECTS_AVERAGE_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_AVERAGENOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             _( GUI_DRAWMODE_MODE ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsMode,
             ID_MENU_DRAWMODE_OBJECTS_MODE,
             histogram->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_MODE );

  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_LAST ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothLast,
             ID_MENU_DRAWMODE_BOTH_LAST,
             histogram->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_LAST &&
             histogram->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_LAST );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_MAXIMUM ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothMaximum,
             ID_MENU_DRAWMODE_BOTH_MAXIMUM,
             histogram->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_MAXIMUM && 
             histogram->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_ABSOLUTE_MAXIMUM ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothAbsoluteMaximum,
             ID_MENU_DRAWMODE_BOTH_ABSOLUTE_MAXIMUM,
             histogram->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM &&
             histogram->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_ABSOLUTE_MAXIMUM );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothMinimumNotZero,
             ID_MENU_DRAWMODE_BOTH_MINIMUM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_MINNOTZERO &&
             histogram->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_ABSOLUTE_MINIMUM_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothAbsoluteMinimumNotZero,
             ID_MENU_DRAWMODE_BOTH_ABSOLUTE_MINIMUM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO &&
             histogram->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_ABSOLUTE_MINNOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_RANDOM ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothRandom,
             ID_MENU_DRAWMODE_BOTH_RANDOM,
             histogram->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_RANDOM &&
             histogram->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_RANDOM );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_RANDOM_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothRandomNotZero,
             ID_MENU_DRAWMODE_BOTH_RANDOM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_RANDNOTZERO &&
             histogram->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_AVERAGE ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothAverage,
             ID_MENU_DRAWMODE_BOTH_AVERAGE,
             histogram->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_AVERAGE &&
             histogram->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_AVERAGE );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_AVERAGE_NOT_ZERO ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothAverageNotZero,
             ID_MENU_DRAWMODE_BOTH_AVERAGE_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_AVERAGENOTZERO &&
             histogram->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_AVERAGENOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             _( GUI_DRAWMODE_MODE ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothMode,
             ID_MENU_DRAWMODE_BOTH_MODE,
             histogram->GetHistogram()->getDrawModeObjects() == DrawModeMethod::DRAW_MODE &&
             histogram->GetHistogram()->getDrawModeColumns() == DrawModeMethod::DRAW_MODE );
             
  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeTime, _( "Semantic" ));
  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeObjects, _( "Objects" ));
  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeBoth, _( "Both" ));
  AppendSubMenu( popUpMenuDrawMode, _( "Drawmode" ) );

  buildItem( popUpMenuPixelSize,
             _( "x1" ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuPixelSize,
             ID_MENU_PIXEL_SIZE_x1,
             histogram->GetHistogram()->getPixelSize() == 1 );
  buildItem( popUpMenuPixelSize,
             _( "x2" ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuPixelSize,
             ID_MENU_PIXEL_SIZE_x2,
             histogram->GetHistogram()->getPixelSize() == 2 );
  buildItem( popUpMenuPixelSize,
             _( "x4" ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuPixelSize,
             ID_MENU_PIXEL_SIZE_x4,
             histogram->GetHistogram()->getPixelSize() == 4 );
  buildItem( popUpMenuPixelSize,
             _( "x8" ),
             wxITEM_RADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuPixelSize,
             ID_MENU_PIXEL_SIZE_x8,
             histogram->GetHistogram()->getPixelSize() == 8 );

  AppendSubMenu( popUpMenuPixelSize, _( "Pixel Size" ));

  AppendSeparator();

  vector< TGroupId > tmpGroups;
  SyncWindows::getInstance()->getGroups( tmpGroups );
  TGroupId i = 0;
  for( vector< TGroupId >::const_iterator itGroup = tmpGroups.begin(); itGroup != tmpGroups.end(); ++itGroup )
  {
    buildItem( popUpMenuSync, wxString::Format( _( "%u" ), *itGroup + 1 ), wxITEM_CHECK, (wxObjectEventFunction)&gPopUpMenu::OnMenuSynchronize,
               ID_MENU_SYNC_GROUP_BASE + i, histogram->GetHistogram()->isSync() && histogram->GetHistogram()->getSyncGroup() == *itGroup );
    ++i;
  }
  popUpMenuSync->AppendSeparator();
  buildItem( popUpMenuSync, _( STR_SYNC_NEW_GROUP ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuSynchronize,
             ID_MENU_NEWGROUP );
  
  i = 1;
  for( vector< TGroupId >::const_iterator itGroup = ++tmpGroups.begin(); itGroup != tmpGroups.end(); ++itGroup )
  {
    buildItem( popUpMenuSyncRemove, wxString::Format( _( "%u" ), *itGroup + 1 ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuRemoveGroup, ID_MENU_SYNC_REMOVE_GROUP_BASE + i );
    ++i;
  }
  popUpMenuSyncRemove->AppendSeparator();
  buildItem( popUpMenuSyncRemove, _( STR_SYNC_REMOVE_ALL_GROUPS ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuRemoveAllGroups,
             ID_MENU_SYNC_REMOVE_ALL_GROUPS );

  popUpMenuSync->AppendSubMenu( popUpMenuSyncRemove, _( STR_SYNC_REMOVE_GROUP ) );
  
  AppendSubMenu( popUpMenuSync, _( STR_SYNCHRONIZE ) );

  AppendSeparator();

  buildItem( popUpMenuSave, _( "Configuration..." ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuSaveHistogramAsCFG, ID_MENU_SAVE_HISTOGRAM_AS_CFG );
  buildItem( popUpMenuSave, _( "Image..." ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuSaveImage, ID_MENU_SAVE_IMAGE );

  if ( histogram->GetHistogram()->getThreeDimensions() )
  {
    buildItem( popUpMenuSaveAsText, 
               _( "Current Plane..." ),
               wxITEM_NORMAL,
               (wxObjectEventFunction)&gPopUpMenu::OnMenuSaveCurrentPlaneAsText,
               ID_MENU_SAVE_CURRENT_PLANE_AS_TEXT );
    buildItem( popUpMenuSaveAsText, _( "All Planes..." ),
               wxITEM_NORMAL,
               (wxObjectEventFunction)&gPopUpMenu::OnMenuSaveAllPlanesAsText,
               ID_MENU_SAVE_ALL_PLANES_AS_TEXT );
    popUpMenuSave->AppendSubMenu( popUpMenuSaveAsText, _( "Text..." ) );

  }
  else
    buildItem( popUpMenuSave, _( "Text..." ), wxITEM_NORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuSaveAllPlanesAsText, ID_MENU_SAVE_ALL_PLANES_AS_TEXT );

  AppendSubMenu( popUpMenuSave, _( "Save" ) );
  
  enableMenu( histogram );
}


gPopUpMenu::~gPopUpMenu()
{
}


void gPopUpMenu::enablePaste( const string tag, bool checkPaste )
{
  if ( timeline == nullptr )
  {
    if ( checkPaste )
    {
      Enable( FindItem( _( STR_PASTE ) ),
              gPasteWindowProperties::getInstance()->isAllowed( histogram, STR_PASTE ) );
      Enable( FindItem( _( STR_PASTE_DEFAULT_SPECIAL ) ),
              gPasteWindowProperties::getInstance()->isAllowed( histogram, STR_PASTE_DEFAULT_SPECIAL ) );
      Enable( FindItem( _( STR_PASTE_SPECIAL ) ),
              gPasteWindowProperties::getInstance()->isAllowed( histogram, STR_PASTE_SPECIAL ) );
    }
    Enable( FindItem( wxString::FromUTF8( tag.c_str() ) ), gPasteWindowProperties::getInstance()->isAllowed( histogram, tag ));
  }
  else
  {
    if ( checkPaste )
    {
      Enable( FindItem( _( STR_PASTE ) ),
              gPasteWindowProperties::getInstance()->isAllowed( timeline, STR_PASTE ));
      Enable( FindItem( _( STR_PASTE_DEFAULT_SPECIAL ) ),
              gPasteWindowProperties::getInstance()->isAllowed( timeline, STR_PASTE_DEFAULT_SPECIAL ));
      Enable( FindItem( _( STR_PASTE_SPECIAL ) ),
              gPasteWindowProperties::getInstance()->isAllowed( timeline, STR_PASTE_SPECIAL ));

    }
    Enable( FindItem( wxString::FromUTF8( tag.c_str() ) ), gPasteWindowProperties::getInstance()->isAllowed( timeline, tag ));
  }
}


void gPopUpMenu::enable( const string tag, bool enable )
{
  Enable( FindItem( wxString::FromUTF8( tag.c_str() ) ), enable );
}


void gPopUpMenu::enable( const string tag )
{
  Enable( FindItem( wxString::FromUTF8( tag.c_str() ) ), true );
}


void gPopUpMenu::disable( const string tag )
{
  Enable( FindItem( wxString::FromUTF8( tag.c_str() ) ), false );
}


string gPopUpMenu::getOption( wxArrayString& choices, int position )
{
  if ( choices[ position ].Cmp( _( STR_FILTER_COMMS_XT ) ) == 0 )
    return string( STR_FILTER_COMMS );
  else if ( choices[ position ].Cmp( _( STR_FILTER_EVENTS_XT ) ) == 0 )
    return string( STR_FILTER_EVENTS );
  else  
    return string( choices[ position ].mb_str() );
}


void gPopUpMenu::OnMenuCopy( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpCopy();
  else
    histogram->OnPopUpCopy();
}


void gPopUpMenu::OnMenuTime( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpPasteTime();
  else
    histogram->OnPopUpPasteTime();
}


void gPopUpMenu::OnMenuObjects( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpPasteObjects();
  else
    histogram->OnPopUpPasteObjects();
}


void gPopUpMenu::OnMenuSize( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpPasteSize();
  else
    histogram->OnPopUpPasteSize();
}


void gPopUpMenu::OnMenuDuration( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpPasteDuration();
  else
    histogram->OnPopUpPasteDuration();
}


void gPopUpMenu::OnMenuSemanticScale( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpPasteSemanticScale();
  else
    histogram->OnPopUpPasteSemanticScale();
}

void gPopUpMenu::OnMenuCustomPalette( wxCommandEvent& event )
{
  if( timeline != nullptr )
  {
    gPasteWindowProperties::getInstance()->paste( timeline, STR_CUSTOM_PALETTE );
    timeline->GetMyWindow()->setRedraw( true );
  }
}

void gPopUpMenu::OnMenuFilterAll( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpPasteFilterAll();
}


void gPopUpMenu::OnMenuFilterComms( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpPasteFilterCommunications();
}


void gPopUpMenu::OnMenuFilterEvents( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpPasteFilterEvents();
}


void gPopUpMenu::OnMenuPasteControlScale( wxCommandEvent& event )
{
  histogram->OnPopUpPasteControlScale();
}


void gPopUpMenu::OnMenuPaste3DScale( wxCommandEvent& event )
{
  histogram->OnPopUpPaste3DScale();
}


void gPopUpMenu::OnMenuPasteControlDimensions( wxCommandEvent& event )
{
  histogram->OnPopUpPasteControlDimensions();
}


void gPopUpMenu::OnMenuPasteDefaultSpecial( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpPasteDefaultSpecial();
  else
    histogram->OnPopUpPasteDefaultSpecial();
}


void gPopUpMenu::OnMenuPasteSpecial( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpPasteSpecial();
  else
    histogram->OnPopUpPasteSpecial();
}


void gPopUpMenu::OnMenuClone( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpClone();
  else
    histogram->OnPopUpClone();
}


void gPopUpMenu::OnMenuRename( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpRename();
  else
    histogram->OnPopUpRename();
}


void gPopUpMenu::OnMenuUndoZoom( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpUndoZoom();
  else
    histogram->OnPopUpUndoZoom();
}


void gPopUpMenu::OnMenuRedoZoom( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpRedoZoom();
  else
    histogram->OnPopUpRedoZoom();
}


void gPopUpMenu::OnMenuFitTime( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpFitTimeScale();
  else
    histogram->OnPopUpFitTimeScale();
}


void gPopUpMenu::OnMenuFitSemanticMin( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpFitSemanticScaleMin();
}


void gPopUpMenu::OnMenuFitSemanticMax( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpFitSemanticScaleMax();
}


void gPopUpMenu::OnMenuFitSemanticBoth( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpFitSemanticScale();
}


void gPopUpMenu::OnMenuFitObjects( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpFitObjects();
  else
    histogram->OnPopUpFitObjects();
}


void gPopUpMenu::OnMenuViewCommunicationLines( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->drawCommunicationLines( event.IsChecked() );
}


void gPopUpMenu::OnMenuViewEventFlags( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->drawEventFlags( event.IsChecked() );
}


void gPopUpMenu::OnMenuViewFunctionLine( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->drawFunctionLineColor();
}


void gPopUpMenu::OnMenuViewFusedLines( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->drawFusedLinesColor();
}


void gPopUpMenu::OnMenuPunctual( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpPunctualColor();
}


void gPopUpMenu::OnMenuPunctualWindow( wxCommandEvent& event )
{
  if( timeline != nullptr )
    timeline->OnPopUpPunctualColorWindow();
}


void gPopUpMenu::OnMenuCodeColor( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpCodeColor();
}


void gPopUpMenu::OnMenuGradientColor( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpGradientColor();
}


void gPopUpMenu::OnMenuNotNullGradientColor( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpNotNullGradientColor();
}


void gPopUpMenu::OnMenuDrawModeTimeLast( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeTimeLast();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeSemanticLast();
}


void gPopUpMenu::OnMenuDrawModeTimeRandom( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeTimeRandom();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeSemanticRandom();
}


void gPopUpMenu::OnMenuDrawModeTimeRandomNotZero( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeTimeRandomNotZero();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeSemanticRandomNotZero();
}


void gPopUpMenu::OnMenuDrawModeTimeMaximum( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeTimeMaximum();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeSemanticMaximum();
}


void gPopUpMenu::OnMenuDrawModeTimeMinimumNotZero( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeTimeMinimumNotZero();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeSemanticMinimumNotZero();
}


void gPopUpMenu::OnMenuDrawModeTimeAbsoluteMaximum( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeTimeAbsoluteMaximum();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeSemanticAbsoluteMaximum();
}


void gPopUpMenu::OnMenuDrawModeTimeAbsoluteMinimumNotZero( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeTimeAbsoluteMinimumNotZero();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeSemanticAbsoluteMinimumNotZero();
}


void gPopUpMenu::OnMenuDrawModeTimeAverage( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeTimeAverage();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeSemanticAverage();
}


void gPopUpMenu::OnMenuDrawModeTimeAverageNotZero( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeTimeAverageNotZero();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeSemanticAverageNotZero();
}


void gPopUpMenu::OnMenuDrawModeTimeMode( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeTimeMode();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeSemanticMode();
}


void gPopUpMenu::OnMenuDrawModeObjectsLast( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeObjectsLast();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeObjectsLast();
}


void gPopUpMenu::OnMenuDrawModeObjectsRandom( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeObjectsRandom();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeObjectsRandom();
}


void gPopUpMenu::OnMenuDrawModeObjectsRandomNotZero( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeObjectsRandomNotZero();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeObjectsRandomNotZero();
}


void gPopUpMenu::OnMenuDrawModeObjectsMaximum( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeObjectsMaximum();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeObjectsMaximum();
}


void gPopUpMenu::OnMenuDrawModeObjectsMinimumNotZero( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeObjectsMinimumNotZero();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeObjectsMinimumNotZero();
}


void gPopUpMenu::OnMenuDrawModeObjectsAbsoluteMaximum( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeObjectsAbsoluteMaximum();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeObjectsAbsoluteMaximum();
}


void gPopUpMenu::OnMenuDrawModeObjectsAbsoluteMinimumNotZero( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeObjectsAbsoluteMinimumNotZero();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeObjectsAbsoluteMinimumNotZero();
}


void gPopUpMenu::OnMenuDrawModeObjectsAverage( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeObjectsAverage();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeObjectsAverage();
}


void gPopUpMenu::OnMenuDrawModeObjectsAverageNotZero( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeObjectsAverageNotZero();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeObjectsAverageNotZero();
}


void gPopUpMenu::OnMenuDrawModeObjectsMode( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeObjectsMode();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeObjectsMode();
}


void gPopUpMenu::OnMenuDrawModeBothLast( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeBothLast();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeBothLast();
}


void gPopUpMenu::OnMenuDrawModeBothRandom( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeBothRandom();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeBothRandom();
}


void gPopUpMenu::OnMenuDrawModeBothRandomNotZero( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeBothRandomNotZero();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeBothRandomNotZero();
}


void gPopUpMenu::OnMenuDrawModeBothMaximum( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeBothMaximum();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeBothMaximum();
}


void gPopUpMenu::OnMenuDrawModeBothMinimumNotZero( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeBothMinimumNotZero();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeBothMinimumNotZero();
}


void gPopUpMenu::OnMenuDrawModeBothAbsoluteMaximum( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeBothAbsoluteMaximum();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeBothAbsoluteMaximum();
}


void gPopUpMenu::OnMenuDrawModeBothAbsoluteMinimumNotZero( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeBothAbsoluteMinimumNotZero();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeBothAbsoluteMinimumNotZero();
}


void gPopUpMenu::OnMenuDrawModeBothAverage( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeBothAverage();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeBothAverage();
}


void gPopUpMenu::OnMenuDrawModeBothAverageNotZero( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeBothAverageNotZero();
  else if( histogram != nullptr )
    histogram->OnPopUpDrawModeBothAverageNotZero();
}


void gPopUpMenu::OnMenuDrawModeBothMode( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpDrawModeBothMode();
  else if ( histogram != nullptr )
    histogram->OnPopUpDrawModeBothMode();
}


void gPopUpMenu::OnMenuPixelSize( wxCommandEvent& event )
{
  if ( timeline != nullptr )
  {
    PRV_UINT32 pixelSize; 

    switch( event.GetId() )
    {
      case ID_MENU_PIXEL_SIZE_x1: pixelSize = 1; break;
      case ID_MENU_PIXEL_SIZE_x2: pixelSize = 2; break;
      case ID_MENU_PIXEL_SIZE_x4: pixelSize = 4; break;
      case ID_MENU_PIXEL_SIZE_x8: pixelSize = 8; break;
      default:                    pixelSize = 1; break;
    }

    timeline->OnPopUpPixelSize( pixelSize );
  }
  else if( histogram != nullptr )
  {
    PRV_UINT16 pixelSize;

    switch( event.GetId() )
    {
      case ID_MENU_PIXEL_SIZE_x1: pixelSize = 1; break;
      case ID_MENU_PIXEL_SIZE_x2: pixelSize = 2; break;
      case ID_MENU_PIXEL_SIZE_x4: pixelSize = 4; break;
      case ID_MENU_PIXEL_SIZE_x8: pixelSize = 8; break;
      default:                    pixelSize = 1; break;
    }

    histogram->OnPopUpPixelSize( pixelSize );
  }
}


void gPopUpMenu::OnMenuRowSelection( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->OnPopUpRowSelection();
  else if ( histogram != nullptr )
    histogram->OnPopUpRowSelection();
}


void gPopUpMenu::OnMenuSaveHistogramAsCFG( wxCommandEvent& event )
{
  histogram->saveCFG();
}


void gPopUpMenu::OnMenuSaveImage( wxCommandEvent& event )
{
  if ( timeline != nullptr )
    timeline->saveImageDialog( _( "" ) );
  else if ( histogram != nullptr )
    histogram->saveImageDialog( _( "" ) );
}


void gPopUpMenu::OnMenuSaveTimelineAsText( wxCommandEvent& event )
{
  timeline->saveText();
}


void gPopUpMenu::OnMenuSaveTimelineAsCFG( wxCommandEvent& event )
{
  timeline->saveCFG();
}


void gPopUpMenu::OnMenuSaveCurrentPlaneAsText( wxCommandEvent& event )
{
  bool onlySelectedPlane = true;
  histogram->saveText( onlySelectedPlane );
}


void gPopUpMenu::OnMenuSaveAllPlanesAsText( wxCommandEvent& event )
{
  bool onlySelectedPlane = false;
  histogram->saveText( onlySelectedPlane );
}


void gPopUpMenu::OnMenuTiming( wxCommandEvent& event )
{
  if( timeline != nullptr )
    timeline->OnPopUpTiming( event.IsChecked() );
}


void gPopUpMenu::OnMenuInfoPanel( wxCommandEvent& event )
{
  if( timeline != nullptr )
    timeline->OnPopUpInfoPanel();
}


void gPopUpMenu::OnMenuAutoControlScale( wxCommandEvent& event )
{
  if( histogram != nullptr )
    histogram->OnPopUpAutoControlScale( event.IsChecked() );
}


void gPopUpMenu::OnMenuAutoControlScaleZero( wxCommandEvent& event )
{
  if( histogram != nullptr )
    histogram->OnPopUpAutoControlScaleZero( event.IsChecked() );
}


void gPopUpMenu::OnMenuAuto3DScale( wxCommandEvent& event )
{
  if( histogram != nullptr )
    histogram->OnPopUpAuto3DScale( event.IsChecked() );
}


void gPopUpMenu::OnMenuAutoDataGradient( wxCommandEvent& event )
{
  if( histogram != nullptr )
    histogram->OnPopUpAutoDataGradient( event.IsChecked() );
}


void gPopUpMenu::OnMenuGradientFunction( wxCommandEvent& event )
{
  TGradientFunction gradFunc;

  switch( event.GetId() )
  {
    case ID_MENU_GRADIENT_FUNCTION_LINEAR: gradFunc = TGradientFunction::LINEAR; break;
    case ID_MENU_GRADIENT_FUNCTION_STEPS: gradFunc = TGradientFunction::STEPS; break;
    case ID_MENU_GRADIENT_FUNCTION_LOGARITHMIC: gradFunc = TGradientFunction::LOGARITHMIC; break;
    case ID_MENU_GRADIENT_FUNCTION_EXPONENTIAL: gradFunc = TGradientFunction::EXPONENTIAL; break;
    default: gradFunc = TGradientFunction::LINEAR;
  }
  
  if( timeline != nullptr )
    timeline->OnMenuGradientFunction( gradFunc );
  else if( histogram != nullptr )
    histogram->OnMenuGradientFunction( gradFunc );
}


void gPopUpMenu::OnMenuSemanticScaleMinAtZero( wxCommandEvent& event )
{
  timeline->GetMyWindow()->setSemanticScaleMinAtZero( event.IsChecked() );
  timeline->GetMyWindow()->setRedraw( true );
}


void gPopUpMenu::OnMenuSynchronize( wxCommandEvent& event )
{
  if( event.GetId() == ID_MENU_NEWGROUP )
  {
    if( timeline != nullptr )
      timeline->GetMyWindow()->addToSyncGroup( SyncWindows::getInstance()->newGroup() );
    else if( histogram != nullptr )
      histogram->GetHistogram()->addToSyncGroup( SyncWindows::getInstance()->newGroup() );
  }
  else
  {
    vector<TGroupId> tmpGroups;
    SyncWindows::getInstance()->getGroups( tmpGroups );
    
    TGroupId group = tmpGroups[ event.GetId() - ID_MENU_SYNC_GROUP_BASE ];
    if( timeline != nullptr )
    {
      if( timeline->GetMyWindow()->isSync() && group == timeline->GetMyWindow()->getSyncGroup() )
        timeline->GetMyWindow()->removeFromSync();
      else
        timeline->GetMyWindow()->addToSyncGroup( group );
    }
    else if( histogram != nullptr )
    {
      if( histogram->GetHistogram()->isSync() && group == histogram->GetHistogram()->getSyncGroup() )
        histogram->GetHistogram()->removeFromSync();
      else
        histogram->GetHistogram()->addToSyncGroup( group );
    }
  }
}


void gPopUpMenu::OnMenuRemoveGroup( wxCommandEvent& event )
{
  vector< TGroupId > tmpGroups;
  SyncWindows::getInstance()->getGroups( tmpGroups );
  SyncWindows::getInstance()->removeAllWindows( tmpGroups[ event.GetId() - ID_MENU_SYNC_REMOVE_GROUP_BASE ] );
}


void gPopUpMenu::OnMenuRemoveAllGroups( wxCommandEvent& event )
{
  vector< TGroupId > tmpGroups;
  SyncWindows::getInstance()->removeAllGroups();
}


void gPopUpMenu::OnMenuCodeColor2D( wxCommandEvent& event )
{
  histogram->GetHistogram()->setColorMode( TColorFunction::COLOR );
  histogram->GetHistogram()->setRedraw( true );
}


void gPopUpMenu::OnMenuGradientColor2D( wxCommandEvent& event )
{
  histogram->GetHistogram()->setColorMode( TColorFunction::GRADIENT );
  histogram->GetHistogram()->setRedraw( true );
}


void gPopUpMenu::OnMenuNotNullGradientColor2D( wxCommandEvent& event )
{
  histogram->GetHistogram()->setColorMode( TColorFunction::NOT_NULL_GRADIENT );
  histogram->GetHistogram()->setRedraw( true );
}


void gPopUpMenu::OnMenuLabelsAll( wxCommandEvent& event )
{
  timeline->GetMyWindow()->setObjectLabels( TObjectLabels::ALL_LABELS );
  timeline->GetMyWindow()->setRedraw( true );
}


void gPopUpMenu::OnMenuLabelsSpaced( wxCommandEvent& event )
{
  timeline->GetMyWindow()->setObjectLabels(  TObjectLabels::SPACED_LABELS );
  timeline->GetMyWindow()->setRedraw( true );
}


void gPopUpMenu::OnMenuLabelsPower2( wxCommandEvent& event )
{
  timeline->GetMyWindow()->setObjectLabels( TObjectLabels::POWER2_LABELS );
  timeline->GetMyWindow()->setRedraw( true );
}


void gPopUpMenu::OnMenuObjectAxisCurrent( wxCommandEvent& event )
{
  timeline->GetMyWindow()->setObjectAxisSize( TObjectAxisSize::CURRENT_LEVEL );
  timeline->GetMyWindow()->setRedraw( true );
}


void gPopUpMenu::OnMenuObjectAxisAll( wxCommandEvent& event )
{
  timeline->GetMyWindow()->setObjectAxisSize( TObjectAxisSize::ALL_LEVELS );
  timeline->GetMyWindow()->setRedraw( true );
}


void gPopUpMenu::OnMenuObjectAxisZero( wxCommandEvent& event )
{
  timeline->GetMyWindow()->setObjectAxisSize( TObjectAxisSize::ZERO_PERC );
  timeline->GetMyWindow()->setRedraw( true );
}


void gPopUpMenu::OnMenuObjectAxisFive( wxCommandEvent& event )
{
  timeline->GetMyWindow()->setObjectAxisSize( TObjectAxisSize::FIVE_PERC );
  timeline->GetMyWindow()->setRedraw( true );
}


void gPopUpMenu::OnMenuObjectAxisTen( wxCommandEvent& event )
{
  timeline->GetMyWindow()->setObjectAxisSize( TObjectAxisSize::TEN_PERC );
  timeline->GetMyWindow()->setRedraw( true );
}


void gPopUpMenu::OnMenuObjectAxisTwentyFive( wxCommandEvent& event )
{
  timeline->GetMyWindow()->setObjectAxisSize( TObjectAxisSize::TWENTYFIVE_PERC );
  timeline->GetMyWindow()->setRedraw( true );
}


void gPopUpMenu::OnMenuClustering( wxCommandEvent& event )
{
  //paraverMain::myParaverMain->SetClusteringGTimeline( timeline );
  paraverMain::myParaverMain->SetClusteringWindow( timeline->GetMyWindow() );
  SequenceDriver::sequenceClustering( timeline );
}


void gPopUpMenu::OnMenuFolding( wxCommandEvent& event )
{
  SequenceDriver::sequenceFolding( timeline );
}


void gPopUpMenu::OnMenuSpectral( wxCommandEvent& event )
{
  SequenceDriver::sequenceSpectral( timeline );
}


void gPopUpMenu::OnMenuDimemas( wxCommandEvent& event )
{
  SequenceDriver::sequenceDimemas( timeline );
}


void gPopUpMenu::OnMenuCutter( wxCommandEvent& event )
{
  SequenceDriver::sequenceCutter( timeline );
}

void gPopUpMenu::OnMenuUserCommand( wxCommandEvent& event )
{
  SequenceDriver::sequenceUserCommand( timeline );
}

