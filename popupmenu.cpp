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

#include "popupmenu.h"
#include "copypaste.h"
#include "labelconstructor.h"
#include "window.h"
#include <wx/event.h>
#include <iostream>
#include <cmath>

using namespace std;

BEGIN_EVENT_TABLE( gPopUpMenu, wxMenu )
  EVT_MENU( ID_MENU_COPY, gPopUpMenu::OnMenuCopy )
  EVT_MENU( ID_MENU_TIME, gPopUpMenu::OnMenuTime )
  EVT_MENU( ID_MENU_OBJECTS, gPopUpMenu::OnMenuObjects )
  EVT_MENU( ID_MENU_SIZE, gPopUpMenu::OnMenuSize )
  EVT_MENU( ID_MENU_DURATION, gPopUpMenu::OnMenuDuration )
  EVT_MENU( ID_MENU_SEMANTIC_SCALE, gPopUpMenu::OnMenuSemanticScale )
  EVT_MENU( ID_MENU_FILTER_ALL, gPopUpMenu::OnMenuFilterAll )
  EVT_MENU( ID_MENU_FILTER_COMMS, gPopUpMenu::OnMenuFilterComms )
  EVT_MENU( ID_MENU_FILTER_EVENTS, gPopUpMenu::OnMenuFilterEvents )
  EVT_MENU( ID_MENU_PASTE_SPECIAL, gPopUpMenu::OnMenuPasteSpecial )
  EVT_MENU( ID_MENU_CLONE, gPopUpMenu::OnMenuClone )
  EVT_MENU( ID_MENU_UNDO_ZOOM, gPopUpMenu::OnMenuUndoZoom )
  EVT_MENU( ID_MENU_REDO_ZOOM, gPopUpMenu::OnMenuRedoZoom )
  EVT_MENU( ID_MENU_FIT_TIME, gPopUpMenu::OnMenuFitTime )
  EVT_MENU( ID_MENU_FIT_SEMANTIC_MIN, gPopUpMenu::OnMenuFitSemanticMin )
  EVT_MENU( ID_MENU_FIT_SEMANTIC_MAX, gPopUpMenu::OnMenuFitSemanticMax )
  EVT_MENU( ID_MENU_FIT_SEMANTIC_BOTH, gPopUpMenu::OnMenuFitSemanticBoth )
  EVT_MENU( ID_MENU_FIT_OBJECTS, gPopUpMenu::OnMenuFitObjects )
  EVT_MENU( ID_MENU_VIEW_COMMUNICATION_LINES, gPopUpMenu::OnMenuViewCommunicationLines )
  EVT_MENU( ID_MENU_VIEW_EVENT_FLAGS, gPopUpMenu::OnMenuViewEventFlags )
  EVT_MENU( ID_MENU_CODE_COLOR, gPopUpMenu::OnMenuCodeColor )
  EVT_MENU( ID_MENU_GRADIENT_COLOR, gPopUpMenu::OnMenuGradientColor )
  EVT_MENU( ID_MENU_NOT_NULL_GRADIENT_COLOR, gPopUpMenu::OnMenuNotNullGradientColor )
  EVT_MENU( ID_MENU_DRAWMODE_TIME_LAST, gPopUpMenu::OnMenuDrawModeTimeLast )
  EVT_MENU( ID_MENU_DRAWMODE_TIME_MAXIMUM, gPopUpMenu::OnMenuDrawModeTimeMaximum )
  EVT_MENU( ID_MENU_DRAWMODE_TIME_MINIMUM_NOT_ZERO, gPopUpMenu::OnMenuDrawModeTimeMinimumNotZero )
  EVT_MENU( ID_MENU_DRAWMODE_TIME_RANDOM, gPopUpMenu::OnMenuDrawModeTimeRandom )
  EVT_MENU( ID_MENU_DRAWMODE_TIME_RANDOM_NOT_ZERO, gPopUpMenu::OnMenuDrawModeTimeRandomNotZero )
  EVT_MENU( ID_MENU_DRAWMODE_TIME_AVERAGE, gPopUpMenu::OnMenuDrawModeTimeAverage )
  EVT_MENU( ID_MENU_DRAWMODE_OBJECTS_LAST, gPopUpMenu::OnMenuDrawModeObjectsLast )
  EVT_MENU( ID_MENU_DRAWMODE_OBJECTS_MAXIMUM, gPopUpMenu::OnMenuDrawModeObjectsMaximum )
  EVT_MENU( ID_MENU_DRAWMODE_OBJECTS_MINIMUM_NOT_ZERO, gPopUpMenu::OnMenuDrawModeObjectsMinimumNotZero )
  EVT_MENU( ID_MENU_DRAWMODE_OBJECTS_RANDOM, gPopUpMenu::OnMenuDrawModeObjectsRandom )
  EVT_MENU( ID_MENU_DRAWMODE_OBJECTS_RANDOM_NOT_ZERO, gPopUpMenu::OnMenuDrawModeObjectsRandomNotZero )
  EVT_MENU( ID_MENU_DRAWMODE_OBJECTS_AVERAGE, gPopUpMenu::OnMenuDrawModeObjectsAverage )
  EVT_MENU( ID_MENU_DRAWMODE_BOTH_LAST, gPopUpMenu::OnMenuDrawModeBothLast )
  EVT_MENU( ID_MENU_DRAWMODE_BOTH_MAXIMUM, gPopUpMenu::OnMenuDrawModeBothMaximum )
  EVT_MENU( ID_MENU_DRAWMODE_BOTH_MINIMUM_NOT_ZERO, gPopUpMenu::OnMenuDrawModeBothMinimumNotZero )
  EVT_MENU( ID_MENU_DRAWMODE_BOTH_RANDOM, gPopUpMenu::OnMenuDrawModeBothRandom )
  EVT_MENU( ID_MENU_DRAWMODE_BOTH_RANDOM_NOT_ZERO, gPopUpMenu::OnMenuDrawModeBothRandomNotZero )
  EVT_MENU( ID_MENU_DRAWMODE_BOTH_AVERAGE, gPopUpMenu::OnMenuDrawModeBothAverage )
  EVT_MENU( ID_MENU_PIXEL_SIZE_x1, gPopUpMenu::OnMenuPixelSize )
  EVT_MENU( ID_MENU_PIXEL_SIZE_x2, gPopUpMenu::OnMenuPixelSize )
  EVT_MENU( ID_MENU_PIXEL_SIZE_x4, gPopUpMenu::OnMenuPixelSize )
  EVT_MENU( ID_MENU_PIXEL_SIZE_x8, gPopUpMenu::OnMenuPixelSize )
  EVT_MENU( ID_MENU_ROW_SELECTION, gPopUpMenu::OnMenuRowSelection )
  EVT_MENU( ID_MENU_SAVE_IMAGE, gPopUpMenu::OnMenuSaveImage )
  EVT_MENU( ID_MENU_SAVE_TEXT, gPopUpMenu::OnMenuSaveText )
  EVT_MENU( ID_MENU_AUTO_CONTROL_SCALE, gPopUpMenu::OnMenuAutoControlScale )
  EVT_MENU( ID_MENU_AUTO_3D_SCALE, gPopUpMenu::OnMenuAuto3DScale )
  EVT_MENU( ID_MENU_AUTO_DATA_GRADIENT, gPopUpMenu::OnMenuAutoDataGradient )
  EVT_MENU( ID_MENU_GRADIENT_FUNCTION_LINEAR, gPopUpMenu::OnMenuGradientFunction )
  EVT_MENU( ID_MENU_GRADIENT_FUNCTION_STEPS, gPopUpMenu::OnMenuGradientFunction )
  EVT_MENU( ID_MENU_GRADIENT_FUNCTION_LOGARITHMIC, gPopUpMenu::OnMenuGradientFunction )
  EVT_MENU( ID_MENU_GRADIENT_FUNCTION_EXPONENTIAL, gPopUpMenu::OnMenuGradientFunction )
  EVT_MENU( ID_MENU_PASTE_CONTROL_SCALE, gPopUpMenu::OnMenuPasteControlScale )
  EVT_MENU( ID_MENU_PASTE_3D_SCALE, gPopUpMenu::OnMenuPaste3DScale )
  EVT_MENU( ID_MENU_SYNCHRONIZE, gPopUpMenu::OnMenuSynchronize )

END_EVENT_TABLE()


void gPopUpMenu::buildItem( wxMenu *popUp,
                            const wxString &title,
                            ItemType itemType,
                            wxObjectEventFunction handler,
                            wxWindowID id,
                            bool checked )
{
  wxMenuItem *tmp;

  switch ( itemType )
  {
    case ITEMNORMAL:
    {
      tmp = new wxMenuItem( popUp, id, title, _( "" ), wxITEM_NORMAL );
      break;
    }
    case ITEMRADIO:
    {
      tmp = new wxMenuItem( popUp, id, title, _( "" ), wxITEM_RADIO );
      break;
    }
    case ITEMCHECK:
    {
      tmp = new wxMenuItem( popUp, id, title, _( "" ), wxITEM_CHECK );
      break;
    }
    default:
    {
      tmp = new wxMenuItem( popUp, id, title, _( "" ), wxITEM_NORMAL );
      break;
    }
  }

  popUp->Append( tmp );
  if ( tmp->IsCheckable() )
    tmp->Check( checked );
    
  if( handler != NULL )
    popUp->Connect( id,
                    wxEVT_COMMAND_MENU_SELECTED, 
                    handler,
                    NULL,
                    this );
}


void gPopUpMenu::enableMenu( gTimeline *whichTimeline )
{
  gPasteWindowProperties* sharedProperties = gPasteWindowProperties::pasteWindowProperties->getInstance();

  Enable( FindItem( _( STR_COPY ) ), true );

  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_TIME ) ), sharedProperties->isAllowed( whichTimeline, STR_TIME) );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_SIZE ) ), sharedProperties->isAllowed( whichTimeline, STR_SIZE)  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_OBJECTS ) ), sharedProperties->isAllowed( whichTimeline, STR_OBJECTS)  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_DURATION ) ), sharedProperties->isAllowed( whichTimeline, STR_DURATION )  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_SEMANTIC_SCALE ) ), sharedProperties->isAllowed( whichTimeline, STR_SEMANTIC_SCALE )  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_FILTER ) ), sharedProperties->isAllowed( whichTimeline, STR_FILTER) );
  
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( _( STR_FILTER_ALL ) ), sharedProperties->isAllowed( whichTimeline, STR_FILTER_ALL) );
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( _( STR_FILTER_COMMS ) ), sharedProperties->isAllowed( whichTimeline, STR_FILTER_COMMS) );
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( _( STR_FILTER_EVENTS ) ), sharedProperties->isAllowed( whichTimeline, STR_FILTER_EVENTS) );

  Enable( FindItem( _( STR_PASTE ) ), sharedProperties->isAllowed( whichTimeline, STR_PASTE) );
  Enable( FindItem( _( STR_PASTE_SPECIAL ) ), sharedProperties->isAllowed( whichTimeline, STR_PASTE_SPECIAL) );

  Enable( FindItem( _( STR_CLONE ) ), true );

  Enable( FindItem( _( STR_FIT_TIME ) ), true );
  Enable( FindItem( _( STR_FIT_SEMANTIC ) ), true );
  Enable( FindItem( _( STR_FIT_OBJECTS ) ), true );
}


void gPopUpMenu::enableMenu( gHistogram *whichHistogram  )
{
  gPasteWindowProperties* sharedProperties = gPasteWindowProperties::pasteWindowProperties->getInstance();

  Enable( FindItem( _( STR_COPY ) ), true );

  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_TIME ) ), sharedProperties->isAllowed( whichHistogram, STR_TIME) );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_SIZE ) ), sharedProperties->isAllowed( whichHistogram, STR_SIZE)  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_OBJECTS ) ), sharedProperties->isAllowed( whichHistogram, STR_OBJECTS)  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_DURATION ) ), sharedProperties->isAllowed( whichHistogram, STR_DURATION )  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( _( STR_SEMANTIC_SCALE ) ), sharedProperties->isAllowed( whichHistogram, STR_SEMANTIC_SCALE)  );

  Enable( FindItem( _( STR_PASTE ) ), sharedProperties->isAllowed( whichHistogram, STR_PASTE) );
  Enable( FindItem( _( STR_PASTE_SPECIAL ) ), sharedProperties->isAllowed( whichHistogram, STR_PASTE_SPECIAL) );

  Enable( FindItem( _( STR_CLONE ) ), true );
  Enable( FindItem( _( STR_FIT_TIME ) ), true );
  Enable( FindItem( _( STR_FIT_OBJECTS ) ), true );
  
  Enable( FindItem( _( STR_CONTROL_SCALE ) ), sharedProperties->isAllowed( whichHistogram, STR_CONTROL_SCALE) );
  if( whichHistogram->GetHistogram()->getThreeDimensions() &&
      histogram != NULL && histogram->GetHistogram()->getThreeDimensions() )
    Enable( FindItem( _( STR_3D_SCALE ) ), sharedProperties->isAllowed( whichHistogram, STR_3D_SCALE) );
  else
    Enable( FindItem( _( STR_3D_SCALE ) ), false );
}


wxMultiChoiceDialog *gPopUpMenu::createPasteSpecialDialog( wxArrayString& choices, gHistogram *whichHistogram )
{
  wxArrayInt sel;
  int i = 0;

  choices.Empty();
  
  gPasteWindowProperties *pasteActions = gPasteWindowProperties::pasteWindowProperties->getInstance();
  
  if ( pasteActions->isAllowed( whichHistogram, STR_TIME ) )
  {
    choices.Add( wxT( STR_TIME ) );
    sel.Add( i );
    ++i;
  }

  if ( pasteActions->isAllowed( whichHistogram, STR_OBJECTS ) )
  {
    choices.Add( wxT( STR_OBJECTS ) );
    sel.Add( i );
    ++i;
  }

  if ( pasteActions->isAllowed( whichHistogram, STR_SIZE ) )
  {
    choices.Add( wxT( STR_SIZE ) );
    sel.Add( i );
    ++i;
  }

  if ( pasteActions->isAllowed( whichHistogram, STR_DURATION ) )
    choices.Add( wxT( STR_DURATION ) );

  if ( pasteActions->isAllowed( whichHistogram, STR_SEMANTIC_SCALE ) )
    choices.Add( wxT( STR_SEMANTIC_SCALE ) );

  if( pasteActions->isAllowed( whichHistogram, STR_CONTROL_SCALE ) )
    choices.Add( wxT( STR_CONTROL_SCALE ) );

  if( pasteActions->isAllowed( whichHistogram, STR_CONTROL_SCALE ) )
  {
    if( whichHistogram->GetHistogram()->getThreeDimensions() )
      choices.Add( wxT( STR_3D_SCALE ) );
  }
  
  wxMultiChoiceDialog *tmpDialog = new wxMultiChoiceDialog( whichHistogram,
                                                            wxT( "Select properties to paste:" ),
                                                            wxT("Paste Special"),
                                                            choices);
  tmpDialog->SetSelections( sel );
  
  return tmpDialog;
}

wxMultiChoiceDialog *gPopUpMenu::createPasteSpecialDialog( wxArrayString& choices, gTimeline *whichTimeline )
{
  wxArrayInt sel;
  int i = 0;

  choices.Empty();
  gPasteWindowProperties *pasteActions = gPasteWindowProperties::pasteWindowProperties->getInstance();
  
  if ( pasteActions->isAllowed( whichTimeline, STR_TIME ) )
  {
    choices.Add( wxT( STR_TIME ) );
    sel.Add( i );
    ++i;
  }

  if ( pasteActions->isAllowed( whichTimeline, STR_OBJECTS ) )
  {
    choices.Add( wxT( STR_OBJECTS ) );
    sel.Add( i );
    ++i;
  }

  if ( pasteActions->isAllowed( whichTimeline, STR_SIZE ) )
  {
    choices.Add( wxT( STR_SIZE ) );
    sel.Add( i );
    ++i;
  }

  if ( pasteActions->isAllowed( whichTimeline, STR_DURATION ) )
    choices.Add( wxT( STR_DURATION ) );

  if ( pasteActions->isAllowed( whichTimeline, STR_SEMANTIC_SCALE ) )
    choices.Add( wxT( STR_SEMANTIC_SCALE ) );

  if ( pasteActions->isAllowed( whichTimeline, STR_FILTER_COMMS ) )
    choices.Add( wxT( STR_FILTER_COMMS_XT ) );

  if ( pasteActions->isAllowed( whichTimeline, STR_FILTER_EVENTS ) )
    choices.Add( wxT( STR_FILTER_EVENTS_XT ) );

  wxMultiChoiceDialog *tmpDialog = new wxMultiChoiceDialog( whichTimeline,
                                                            wxT( "Select properties to paste:" ),
                                                            wxT("Paste Special"),
                                                            choices );
  tmpDialog->SetSelections( sel );
  
  return tmpDialog;
}


RowsSelectionDialog *gPopUpMenu::createRowSelectionDialog( gTimeline *whichTimeline )
{
  RowsSelectionDialog *myDialog = new RowsSelectionDialog( (wxWindow * )whichTimeline,
                                                             ID_ROWSSELECTIONDIALOG,
                                                             _( "Rows Selection" ));

  return myDialog;
}


gPopUpMenu::gPopUpMenu( gTimeline *whichTimeline )
{
  timeline = whichTimeline;
  histogram = NULL;

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

  buildItem( this, _( STR_COPY ), ITEMNORMAL, NULL, ID_MENU_COPY );

  buildItem( popUpMenuPaste, _( STR_TIME ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuTime, ID_MENU_TIME );
  buildItem( popUpMenuPaste, _( STR_OBJECTS ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuObjects, ID_MENU_OBJECTS );
  buildItem( popUpMenuPaste, _( STR_SIZE ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuSize, ID_MENU_SIZE );
  buildItem( popUpMenuPaste, _( STR_DURATION ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuDuration, ID_MENU_DURATION );

  buildItem( popUpMenuPaste, _( STR_SEMANTIC_SCALE ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuSemanticScale, ID_MENU_SEMANTIC_SCALE );

  buildItem( popUpMenuPasteFilter, _( STR_FILTER_ALL ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFilterAll, ID_MENU_FILTER_ALL );
  buildItem( popUpMenuPasteFilter, _( STR_FILTER_COMMS ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFilterComms, ID_MENU_FILTER_COMMS );
  buildItem( popUpMenuPasteFilter, _( STR_FILTER_EVENTS ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFilterEvents, ID_MENU_FILTER_EVENTS );
  popUpMenuPaste->AppendSubMenu( popUpMenuPasteFilter, _( STR_FILTER ) );
  AppendSubMenu( popUpMenuPaste, _( STR_PASTE ) );

  buildItem( this, _( STR_PASTE_SPECIAL ), ITEMNORMAL, NULL, ID_MENU_PASTE_SPECIAL );
  buildItem( this, _( STR_SYNCHRONIZE ), ITEMCHECK, (wxObjectEventFunction)&gPopUpMenu::OnMenuSynchronize, ID_MENU_SYNCHRONIZE, timeline->GetMyWindow()->isSync() );
  buildItem( this, _( STR_CLONE ), ITEMNORMAL, NULL, ID_MENU_CLONE );

  AppendSeparator();

  buildItem( this, _( "Undo Zoom" ), ITEMNORMAL, NULL, ID_MENU_UNDO_ZOOM );
  buildItem( this, _( "Redo Zoom" ), ITEMNORMAL, NULL, ID_MENU_REDO_ZOOM );

  AppendSeparator();

  buildItem( this, _( STR_FIT_TIME ), ITEMNORMAL, NULL, ID_MENU_FIT_TIME );
  buildItem( this, _( STR_FIT_OBJECTS ), ITEMNORMAL, NULL, ID_MENU_FIT_OBJECTS );
  buildItem( popUpMenuFitSemantic, _( "Fit Minimum" ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFitSemanticMin, ID_MENU_FIT_SEMANTIC_MIN);
  buildItem( popUpMenuFitSemantic, _( "Fit Maximum" ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFitSemanticMax, ID_MENU_FIT_SEMANTIC_MAX);
  buildItem( popUpMenuFitSemantic, _( "Fit Both" ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFitSemanticBoth, ID_MENU_FIT_SEMANTIC_BOTH);
  AppendSubMenu( popUpMenuFitSemantic, _( STR_FIT_SEMANTIC ) );

  AppendSeparator();

  buildItem( popUpMenuView, _( "Communication Lines" ), ITEMCHECK, (wxObjectEventFunction)&gPopUpMenu::OnMenuViewCommunicationLines, ID_MENU_VIEW_COMMUNICATION_LINES, timeline->GetMyWindow()->getDrawCommLines() );
  buildItem( popUpMenuView, _( "Event Flags" ), ITEMCHECK, (wxObjectEventFunction)&gPopUpMenu::OnMenuViewEventFlags, ID_MENU_VIEW_EVENT_FLAGS, timeline->GetMyWindow()->getDrawFlags() );
  AppendSubMenu( popUpMenuView, _( "View" ));

  buildItem( popUpMenuColor, _( "Code Color" ), ITEMRADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuCodeColor, ID_MENU_CODE_COLOR, timeline->GetMyWindow()->IsCodeColorSet() );
  buildItem( popUpMenuColor, _( "Gradient Color" ), ITEMRADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuGradientColor,ID_MENU_GRADIENT_COLOR, timeline->GetMyWindow()->IsGradientColorSet() );
  buildItem( popUpMenuColor, _( "Not Null Gradient Color" ), ITEMRADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuNotNullGradientColor,ID_MENU_NOT_NULL_GRADIENT_COLOR, timeline->GetMyWindow()->IsNotNullGradientColorSet() );
  AppendSubMenu( popUpMenuColor, _( "Color" ));

  buildItem( popUpMenuGradientFunction, _( "Linear" ), ITEMRADIO,(wxObjectEventFunction)&gPopUpMenu::OnMenuGradientFunction, ID_MENU_GRADIENT_FUNCTION_LINEAR, timeline->GetMyWindow()->getGradientColor().getGradientFunction() == GradientColor::LINEAR );
  buildItem( popUpMenuGradientFunction, _( "Steps" ), ITEMRADIO,(wxObjectEventFunction)&gPopUpMenu::OnMenuGradientFunction, ID_MENU_GRADIENT_FUNCTION_STEPS, timeline->GetMyWindow()->getGradientColor().getGradientFunction() == GradientColor::STEPS );
  buildItem( popUpMenuGradientFunction, _( "Logarithmic" ), ITEMRADIO,(wxObjectEventFunction)&gPopUpMenu::OnMenuGradientFunction, ID_MENU_GRADIENT_FUNCTION_LOGARITHMIC, timeline->GetMyWindow()->getGradientColor().getGradientFunction() == GradientColor::LOGARITHMIC );
  buildItem( popUpMenuGradientFunction, _( "Exponential" ), ITEMRADIO,(wxObjectEventFunction)&gPopUpMenu::OnMenuGradientFunction, ID_MENU_GRADIENT_FUNCTION_EXPONENTIAL, timeline->GetMyWindow()->getGradientColor().getGradientFunction() == GradientColor::EXPONENTIAL );
  wxMenuItem *tmpGradFunc = AppendSubMenu( popUpMenuGradientFunction, _( "Gradient Function" ) );
  Enable( tmpGradFunc->GetId(), timeline->GetMyWindow()->IsCodeColorSet() == false );

  buildItem( popUpMenuDrawModeTime,
             _( "Last" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeLast,
             ID_MENU_DRAWMODE_TIME_LAST,
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_LAST );
  buildItem( popUpMenuDrawModeTime,
             _( "Maximum" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeMaximum,
             ID_MENU_DRAWMODE_TIME_MAXIMUM,
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeTime,
             _( "Minimum not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeMinimumNotZero,
             ID_MENU_DRAWMODE_TIME_MINIMUM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeTime,
             _( "Random" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeRandom,
             ID_MENU_DRAWMODE_TIME_RANDOM,
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_RANDOM );
  buildItem( popUpMenuDrawModeTime,
             _( "Random not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeRandomNotZero,
             ID_MENU_DRAWMODE_TIME_RANDOM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeTime,
             _( "Average" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeAverage,
             ID_MENU_DRAWMODE_TIME_AVERAGE,
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_AVERAGE );

  buildItem( popUpMenuDrawModeObjects,
             _( "Last" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsLast,
             ID_MENU_DRAWMODE_OBJECTS_LAST,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_LAST );
  buildItem( popUpMenuDrawModeObjects,
             _( "Maximum" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsMaximum,
             ID_MENU_DRAWMODE_OBJECTS_MAXIMUM,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeObjects,
             _( "Minimum not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsMinimumNotZero,
             ID_MENU_DRAWMODE_OBJECTS_MINIMUM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             _( "Random" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsRandom,
             ID_MENU_DRAWMODE_OBJECTS_RANDOM,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_RANDOM );
  buildItem( popUpMenuDrawModeObjects,
             _( "Random not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsRandomNotZero,
             ID_MENU_DRAWMODE_OBJECTS_RANDOM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             _( "Average" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsAverage,
             ID_MENU_DRAWMODE_OBJECTS_AVERAGE,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_AVERAGE );

  buildItem( popUpMenuDrawModeBoth,
             _( "Last" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothLast,
             ID_MENU_DRAWMODE_BOTH_LAST,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_LAST &&
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_LAST );
  buildItem( popUpMenuDrawModeBoth,
             _( "Maximum" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothMaximum,
             ID_MENU_DRAWMODE_BOTH_MAXIMUM,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_MAXIMUM && 
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeBoth,
             _( "Minimum not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothMinimumNotZero,
             ID_MENU_DRAWMODE_BOTH_MINIMUM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_MINNOTZERO &&
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             _( "Random" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothRandom,
             ID_MENU_DRAWMODE_BOTH_RANDOM,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_RANDOM &&
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_RANDOM );
  buildItem( popUpMenuDrawModeBoth,
             _( "Random not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothRandomNotZero,
             ID_MENU_DRAWMODE_BOTH_RANDOM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_RANDNOTZERO &&
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             _( "Average" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothAverage,
             ID_MENU_DRAWMODE_BOTH_AVERAGE,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_AVERAGE &&
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_AVERAGE );

  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeTime, _( "Time" ));
  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeObjects, _( "Objects" ));
  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeBoth, _( "Both" ));
  AppendSubMenu( popUpMenuDrawMode, _( "Drawmode" ));

  buildItem( popUpMenuPixelSize,
             _( "x1" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuPixelSize,
             ID_MENU_PIXEL_SIZE_x1,
             timeline->GetPixelSize() == 1 );
  buildItem( popUpMenuPixelSize,
             _( "x2" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuPixelSize,
             ID_MENU_PIXEL_SIZE_x2,
             timeline->GetPixelSize() == 2 );
  buildItem( popUpMenuPixelSize,
             _( "x4" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuPixelSize,
             ID_MENU_PIXEL_SIZE_x4,
             timeline->GetPixelSize() == 4 );
  buildItem( popUpMenuPixelSize,
             _( "x8" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuPixelSize,
             ID_MENU_PIXEL_SIZE_x8,
             timeline->GetPixelSize() == 8 );

  AppendSubMenu( popUpMenuPixelSize, _( "Pixel Size" ));


  AppendSeparator();
  buildItem( this, _( "Select Rows..." ), ITEMNORMAL, NULL, ID_MENU_ROW_SELECTION );

  AppendSeparator();
  buildItem( this, _( "Save Image..." ), ITEMNORMAL, NULL, ID_MENU_SAVE_IMAGE );
  buildItem( this, _( "Save as text..." ), ITEMNORMAL, NULL, ID_MENU_SAVE_TEXT );

  AppendSeparator();
  buildItem( this, 
             _( "Info Panel" ),
             ITEMCHECK,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuInfoPanel,
             ID_MENU_INFO_PANEL,
             timeline->IsSplit() );

  enableMenu( timeline );
}

gPopUpMenu::gPopUpMenu( gHistogram *whichHistogram )
{
  timeline = NULL;
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

  buildItem( this, _( STR_COPY ), ITEMNORMAL, NULL, ID_MENU_COPY );

  buildItem( popUpMenuPaste, _( STR_TIME ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuTime, ID_MENU_TIME );
  buildItem( popUpMenuPaste, _( STR_OBJECTS ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuObjects, ID_MENU_OBJECTS );
  buildItem( popUpMenuPaste, _( STR_SIZE ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuSize, ID_MENU_SIZE );
  buildItem( popUpMenuPaste, _( STR_DURATION ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuDuration, ID_MENU_DURATION );
  buildItem( popUpMenuPaste, _( STR_SEMANTIC_SCALE ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuSemanticScale, ID_MENU_SEMANTIC_SCALE );
  buildItem( popUpMenuPaste, _( STR_CONTROL_SCALE ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuPasteControlScale, ID_MENU_PASTE_CONTROL_SCALE );
  buildItem( popUpMenuPaste, _( STR_3D_SCALE ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuPaste3DScale, ID_MENU_PASTE_3D_SCALE );

  AppendSubMenu( popUpMenuPaste, _( STR_PASTE ) );

  buildItem( this, _( STR_PASTE_SPECIAL ), ITEMNORMAL, NULL, ID_MENU_PASTE_SPECIAL );
  buildItem( this, _( STR_CLONE ), ITEMNORMAL, NULL, ID_MENU_CLONE );

  AppendSeparator();

  buildItem( this, _( "Undo Zoom" ), ITEMNORMAL, NULL, ID_MENU_UNDO_ZOOM );
  buildItem( this, _( "Redo Zoom" ), ITEMNORMAL, NULL, ID_MENU_REDO_ZOOM );

  AppendSeparator();

  buildItem( this, _( STR_FIT_TIME ), ITEMNORMAL, NULL, ID_MENU_FIT_TIME );
  buildItem( this, _( STR_FIT_OBJECTS ), ITEMNORMAL, NULL, ID_MENU_FIT_OBJECTS );
  buildItem( this, 
             _( "Auto Fit Control Scale" ),
             ITEMCHECK,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuAutoControlScale,
             ID_MENU_AUTO_CONTROL_SCALE,
             histogram->GetHistogram()->getCompute2DScale() );
  if( histogram->GetHistogram()->getThreeDimensions() )
    buildItem( this, 
               _( "Auto Fit 3D Scale" ),
               ITEMCHECK,
               (wxObjectEventFunction)&gPopUpMenu::OnMenuAuto3DScale,
               ID_MENU_AUTO_3D_SCALE,
               histogram->GetHistogram()->getCompute3DScale() );
  buildItem( this, 
             _( "Auto Fit Data Gradient" ),
             ITEMCHECK,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuAutoDataGradient,
             ID_MENU_AUTO_DATA_GRADIENT,
             histogram->GetHistogram()->getComputeGradient() );

  buildItem( popUpMenuGradientFunction, _( "Linear" ), ITEMRADIO,(wxObjectEventFunction)&gPopUpMenu::OnMenuGradientFunction, ID_MENU_GRADIENT_FUNCTION_LINEAR, histogram->GetHistogram()->getGradientColor().getGradientFunction() == GradientColor::LINEAR );
  buildItem( popUpMenuGradientFunction, _( "Steps" ), ITEMRADIO,(wxObjectEventFunction)&gPopUpMenu::OnMenuGradientFunction, ID_MENU_GRADIENT_FUNCTION_STEPS, histogram->GetHistogram()->getGradientColor().getGradientFunction() == GradientColor::STEPS );
  buildItem( popUpMenuGradientFunction, _( "Logarithmic" ), ITEMRADIO,(wxObjectEventFunction)&gPopUpMenu::OnMenuGradientFunction, ID_MENU_GRADIENT_FUNCTION_LOGARITHMIC, histogram->GetHistogram()->getGradientColor().getGradientFunction() == GradientColor::LOGARITHMIC );
  buildItem( popUpMenuGradientFunction, _( "Exponential" ), ITEMRADIO,(wxObjectEventFunction)&gPopUpMenu::OnMenuGradientFunction, ID_MENU_GRADIENT_FUNCTION_EXPONENTIAL, histogram->GetHistogram()->getGradientColor().getGradientFunction() == GradientColor::EXPONENTIAL );
  AppendSubMenu( popUpMenuGradientFunction, _( "Gradient Function " ) );

  AppendSeparator();
  
  buildItem( popUpMenuDrawModeTime,
             _( "Last" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeLast,
             ID_MENU_DRAWMODE_TIME_LAST,
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_LAST );
  buildItem( popUpMenuDrawModeTime,
             _( "Maximum" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeMaximum,
             ID_MENU_DRAWMODE_TIME_MAXIMUM,
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeTime,
             _( "Minimum not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeMinimumNotZero,
             ID_MENU_DRAWMODE_TIME_MINIMUM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeTime,
             _( "Random" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeRandom,
             ID_MENU_DRAWMODE_TIME_RANDOM,
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_RANDOM );
  buildItem( popUpMenuDrawModeTime,
             _( "Random not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeRandomNotZero,
             ID_MENU_DRAWMODE_TIME_RANDOM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeTime,
             _( "Average" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeAverage,
             ID_MENU_DRAWMODE_TIME_AVERAGE,
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_AVERAGE );

  buildItem( popUpMenuDrawModeObjects,
             _( "Last" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsLast,
             ID_MENU_DRAWMODE_OBJECTS_LAST,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_LAST );
  buildItem( popUpMenuDrawModeObjects,
             _( "Maximum" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsMaximum,
             ID_MENU_DRAWMODE_OBJECTS_MAXIMUM,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeObjects,
             _( "Minimum not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsMinimumNotZero,
             ID_MENU_DRAWMODE_OBJECTS_MINIMUM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             _( "Random" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsRandom,
             ID_MENU_DRAWMODE_OBJECTS_RANDOM,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_RANDOM );
  buildItem( popUpMenuDrawModeObjects,
             _( "Random not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsRandomNotZero,
             ID_MENU_DRAWMODE_OBJECTS_RANDOM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             _( "Average" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsAverage,
             ID_MENU_DRAWMODE_OBJECTS_AVERAGE,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_AVERAGE );

  buildItem( popUpMenuDrawModeBoth,
             _( "Last" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothLast,
             ID_MENU_DRAWMODE_BOTH_LAST,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_LAST &&
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_LAST );
  buildItem( popUpMenuDrawModeBoth,
             _( "Maximum" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothMaximum,
             ID_MENU_DRAWMODE_BOTH_MAXIMUM,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_MAXIMUM && 
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeBoth,
             _( "Minimum not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothMinimumNotZero,
             ID_MENU_DRAWMODE_BOTH_MINIMUM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_MINNOTZERO &&
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             _( "Random" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothRandom,
             ID_MENU_DRAWMODE_BOTH_RANDOM,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_RANDOM &&
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_RANDOM );
  buildItem( popUpMenuDrawModeBoth,
             _( "Random not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothRandomNotZero,
             ID_MENU_DRAWMODE_BOTH_RANDOM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_RANDNOTZERO &&
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             _( "Average" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothAverage,
             ID_MENU_DRAWMODE_BOTH_AVERAGE,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_AVERAGE &&
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_AVERAGE );

  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeTime, _( "Semantic" ));
  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeObjects, _( "Objects" ));
  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeBoth, _( "Both" ));
  AppendSubMenu( popUpMenuDrawMode, _( "Drawmode" ) );

  AppendSeparator();
  buildItem( this, _( "Save as text..." ), ITEMNORMAL, NULL, ID_MENU_SAVE_TEXT );

  enableMenu( histogram );
}

gPopUpMenu::~gPopUpMenu()
{
}

void gPopUpMenu::enablePaste( const string tag, bool checkPaste )
{
  if ( timeline == NULL )
  {
    if ( checkPaste )
    {
      Enable( FindItem( _( STR_PASTE ) ),
              gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( histogram, STR_PASTE ) );
      Enable( FindItem( _( STR_PASTE_SPECIAL ) ),
              gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( histogram, STR_PASTE_SPECIAL ) );
    }
    Enable( FindItem( wxString::FromAscii( tag.c_str() ) ), gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( histogram, tag ));
  }
  else
  {
    if ( checkPaste )
    {
      Enable( FindItem( _( STR_PASTE ) ),
              gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( timeline, STR_PASTE ));
      Enable( FindItem( _( STR_PASTE_SPECIAL ) ),
              gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( timeline, STR_PASTE_SPECIAL ));

    }
    Enable( FindItem( wxString::FromAscii( tag.c_str() ) ), gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( timeline, tag ));
  }
}


void gPopUpMenu::enable( const string tag, bool enable )
{
  Enable( FindItem( wxString::FromAscii( tag.c_str() ) ), enable );
}


void gPopUpMenu::enable( const string tag )
{
  Enable( FindItem( wxString::FromAscii( tag.c_str() ) ), true );
}


void gPopUpMenu::disable( const string tag )
{
  Enable( FindItem( wxString::FromAscii( tag.c_str() ) ), false );
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

void gPopUpMenu::OnMenuCopy( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpCopy();
  else
    histogram->OnPopUpCopy();
}

void gPopUpMenu::OnMenuTime( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpPasteTime();
  else
    histogram->OnPopUpPasteTime();
}

void gPopUpMenu::OnMenuObjects( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpPasteObjects();
  else
    histogram->OnPopUpPasteObjects();
}

void gPopUpMenu::OnMenuSize( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpPasteSize();
  else
    histogram->OnPopUpPasteSize();
}

void gPopUpMenu::OnMenuDuration( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpPasteDuration();
  else
    histogram->OnPopUpPasteDuration();
}


void gPopUpMenu::OnMenuSemanticScale( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpPasteSemanticScale();
  else
    histogram->OnPopUpPasteSemanticScale();
}

void gPopUpMenu::OnMenuFilterAll( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpPasteFilterAll();
}

void gPopUpMenu::OnMenuFilterComms( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpPasteFilterCommunications();
}

void gPopUpMenu::OnMenuFilterEvents( wxCommandEvent& event)
{
  if ( timeline != NULL )
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

void gPopUpMenu::OnMenuPasteSpecial( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpPasteSpecial();
  else
    histogram->OnPopUpPasteSpecial();
}

void gPopUpMenu::OnMenuClone( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpClone();
  else
    histogram->OnPopUpClone();
}

void gPopUpMenu::OnMenuUndoZoom( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpUndoZoom();
  else
    histogram->OnPopUpUndoZoom();
}

void gPopUpMenu::OnMenuRedoZoom( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpRedoZoom();
  else
    histogram->OnPopUpRedoZoom();
}

void gPopUpMenu::OnMenuFitTime( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpFitTimeScale();
  else
    histogram->OnPopUpFitTimeScale();
}

void gPopUpMenu::OnMenuFitSemanticMin( wxCommandEvent& event )
{
  if ( timeline != NULL )
    timeline->OnPopUpFitSemanticScaleMin();
}

void gPopUpMenu::OnMenuFitSemanticMax( wxCommandEvent& event )
{
  if ( timeline != NULL )
    timeline->OnPopUpFitSemanticScaleMax();
}

void gPopUpMenu::OnMenuFitSemanticBoth( wxCommandEvent& event )
{
  if ( timeline != NULL )
    timeline->OnPopUpFitSemanticScale();
}

void gPopUpMenu::OnMenuFitObjects( wxCommandEvent& event )
{
  if ( timeline != NULL )
    timeline->OnPopUpFitObjects();
  else
    histogram->OnPopUpFitObjects();
}

void gPopUpMenu::OnMenuViewCommunicationLines( wxCommandEvent& event )
{
  if ( timeline != NULL )
    timeline->drawCommunicationLines( event.IsChecked() );
}

void gPopUpMenu::OnMenuViewEventFlags( wxCommandEvent& event )
{
  if ( timeline != NULL )
    timeline->drawEventFlags( event.IsChecked() );
}

void gPopUpMenu::OnMenuCodeColor( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpCodeColor();
}

void gPopUpMenu::OnMenuGradientColor( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpGradientColor();
}

void gPopUpMenu::OnMenuNotNullGradientColor( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpNotNullGradientColor();
}

void gPopUpMenu::OnMenuDrawModeTimeLast( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpDrawModeTimeLast();
  else if( histogram != NULL )
    histogram->OnPopUpDrawModeSemanticLast();
}

void gPopUpMenu::OnMenuDrawModeTimeRandom( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpDrawModeTimeRandom();
  else if( histogram != NULL )
    histogram->OnPopUpDrawModeSemanticRandom();
}

void gPopUpMenu::OnMenuDrawModeTimeRandomNotZero( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpDrawModeTimeRandomNotZero();
  else if( histogram != NULL )
    histogram->OnPopUpDrawModeSemanticRandomNotZero();
}

void gPopUpMenu::OnMenuDrawModeTimeMaximum( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpDrawModeTimeMaximum();
  else if( histogram != NULL )
    histogram->OnPopUpDrawModeSemanticMaximum();
}

void gPopUpMenu::OnMenuDrawModeTimeMinimumNotZero( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpDrawModeTimeMinimumNotZero();
  else if( histogram != NULL )
    histogram->OnPopUpDrawModeSemanticMinimumNotZero();
}

void gPopUpMenu::OnMenuDrawModeTimeAverage( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpDrawModeTimeAverage();
  else if( histogram != NULL )
    histogram->OnPopUpDrawModeSemanticAverage();
}

void gPopUpMenu::OnMenuDrawModeObjectsLast( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpDrawModeObjectsLast();
  else if( histogram != NULL )
    histogram->OnPopUpDrawModeObjectsLast();
}

void gPopUpMenu::OnMenuDrawModeObjectsRandom( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpDrawModeObjectsRandom();
  else if( histogram != NULL )
    histogram->OnPopUpDrawModeObjectsRandom();
}

void gPopUpMenu::OnMenuDrawModeObjectsRandomNotZero( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpDrawModeObjectsRandomNotZero();
  else if( histogram != NULL )
    histogram->OnPopUpDrawModeObjectsRandomNotZero();
}

void gPopUpMenu::OnMenuDrawModeObjectsMaximum( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpDrawModeObjectsMaximum();
  else if( histogram != NULL )
    histogram->OnPopUpDrawModeObjectsMaximum();
}

void gPopUpMenu::OnMenuDrawModeObjectsMinimumNotZero( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpDrawModeObjectsMinimumNotZero();
  else if( histogram != NULL )
    histogram->OnPopUpDrawModeObjectsMinimumNotZero();
}

void gPopUpMenu::OnMenuDrawModeObjectsAverage( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpDrawModeObjectsAverage();
  else if( histogram != NULL )
    histogram->OnPopUpDrawModeObjectsAverage();
}

void gPopUpMenu::OnMenuDrawModeBothLast( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpDrawModeBothLast();
  else if( histogram != NULL )
    histogram->OnPopUpDrawModeBothLast();
}

void gPopUpMenu::OnMenuDrawModeBothRandom( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpDrawModeBothRandom();
  else if( histogram != NULL )
    histogram->OnPopUpDrawModeBothRandom();
}

void gPopUpMenu::OnMenuDrawModeBothRandomNotZero( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpDrawModeBothRandomNotZero();
  else if( histogram != NULL )
    histogram->OnPopUpDrawModeBothRandomNotZero();
}

void gPopUpMenu::OnMenuDrawModeBothMaximum( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpDrawModeBothMaximum();
  else if( histogram != NULL )
    histogram->OnPopUpDrawModeBothMaximum();
}

void gPopUpMenu::OnMenuDrawModeBothMinimumNotZero( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpDrawModeBothMinimumNotZero();
  else if( histogram != NULL )
    histogram->OnPopUpDrawModeBothMinimumNotZero();
}

void gPopUpMenu::OnMenuDrawModeBothAverage( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpDrawModeBothAverage();
  else if( histogram != NULL )
    histogram->OnPopUpDrawModeBothAverage();
}

void gPopUpMenu::OnMenuPixelSize( wxCommandEvent& event )
{
  if ( timeline != NULL )
  {
    UINT32 pixelSize; 

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
}

void gPopUpMenu::OnMenuRowSelection( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpRowSelection();
}

void gPopUpMenu::OnMenuSaveImage( wxCommandEvent& event )
{
  if ( timeline != NULL )
    timeline->saveImage();
}

void gPopUpMenu::OnMenuSaveText( wxCommandEvent& event )
{
  if( timeline != NULL )
    timeline->saveText();
  else if( histogram != NULL )
    histogram->saveText();
}

void gPopUpMenu::OnMenuInfoPanel( wxCommandEvent& event )
{
  if( timeline != NULL )
    timeline->OnPopUpInfoPanel();
}

void gPopUpMenu::OnMenuAutoControlScale( wxCommandEvent& event )
{
  if( histogram != NULL )
    histogram->OnPopUpAutoControlScale( event.IsChecked() );
}

void gPopUpMenu::OnMenuAuto3DScale( wxCommandEvent& event )
{
  if( histogram != NULL )
    histogram->OnPopUpAuto3DScale( event.IsChecked() );
}

void gPopUpMenu::OnMenuAutoDataGradient( wxCommandEvent& event )
{
  if( histogram != NULL )
    histogram->OnPopUpAutoDataGradient( event.IsChecked() );
}

void gPopUpMenu::OnMenuGradientFunction( wxCommandEvent& event )
{
  GradientColor::TGradientFunction gradFunc;

  switch( event.GetId() )
  {
    case ID_MENU_GRADIENT_FUNCTION_LINEAR: gradFunc = GradientColor::LINEAR; break;
    case ID_MENU_GRADIENT_FUNCTION_STEPS: gradFunc = GradientColor::STEPS; break;
    case ID_MENU_GRADIENT_FUNCTION_LOGARITHMIC: gradFunc = GradientColor::LOGARITHMIC; break;
    case ID_MENU_GRADIENT_FUNCTION_EXPONENTIAL: gradFunc = GradientColor::EXPONENTIAL; break;
    default: gradFunc = GradientColor::LINEAR;
  }
  
  if( timeline != NULL )
    timeline->OnMenuGradientFunction( gradFunc );
  else if( histogram != NULL )
    histogram->OnMenuGradientFunction( gradFunc );
}

void gPopUpMenu::OnMenuSynchronize( wxCommandEvent& event )
{
  if( timeline->GetMyWindow()->isSync() )
    timeline->GetMyWindow()->removeFromSync();
  else
    timeline->GetMyWindow()->addToSyncGroup( 0 );
}
