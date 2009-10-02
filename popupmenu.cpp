#include "popupmenu.h"
#include "copypaste.h"
#include "labelconstructor.h"
#include "window.h"
#include <wx/event.h>
#include <iostream>

using namespace std;

BEGIN_EVENT_TABLE( gPopUpMenu, wxMenu )
  EVT_MENU( ID_MENU_COPY, gPopUpMenu::OnMenuCopy )
  EVT_MENU( ID_MENU_TIME, gPopUpMenu::OnMenuTime )
  EVT_MENU( ID_MENU_OBJECTS, gPopUpMenu::OnMenuObjects )
  EVT_MENU( ID_MENU_SIZE, gPopUpMenu::OnMenuSize )
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
  EVT_MENU( ID_MENU_PAINT_COMMUNICATION_LINES, gPopUpMenu::OnMenuPaintCommunicationLines )
  EVT_MENU( ID_MENU_PAINT_EVENT_FLAGS, gPopUpMenu::OnMenuPaintEventFlags )
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
  EVT_MENU( ID_MENU_ROW_SELECTION, gPopUpMenu::OnMenuRowSelection )
  EVT_MENU( ID_MENU_AUTO_CONTROL_SCALE, gPopUpMenu::OnMenuAutoControlScale )
  EVT_MENU( ID_MENU_AUTO_3D_SCALE, gPopUpMenu::OnMenuAuto3DScale )
  EVT_MENU( ID_MENU_AUTO_DATA_GRADIENT, gPopUpMenu::OnMenuAutoDataGradient )

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
      tmp = new wxMenuItem( popUp, id, title, "", wxITEM_NORMAL );
      break;
    }
    case ITEMRADIO:
    {
      tmp = new wxMenuItem( popUp, id, title, "", wxITEM_RADIO );
      break;
    }
    case ITEMCHECK:
    {
      tmp = new wxMenuItem( popUp, id, title, "", wxITEM_CHECK );
      break;
    }
    default:
    {
      tmp = new wxMenuItem( popUp, id, title, "", wxITEM_NORMAL );
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

  Enable( FindItem( STR_COPY ), true );

  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( STR_TIME ), sharedProperties->isAllowed( whichTimeline, STR_TIME) );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( STR_SIZE ), sharedProperties->isAllowed( whichTimeline, STR_SIZE)  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( STR_OBJECTS ), sharedProperties->isAllowed( whichTimeline, STR_OBJECTS)  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( STR_FILTER ), sharedProperties->isAllowed( whichTimeline, STR_FILTER) );
  
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( STR_FILTER_ALL ), sharedProperties->isAllowed( whichTimeline, STR_FILTER_ALL) );
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( STR_FILTER_COMMS ), sharedProperties->isAllowed( whichTimeline, STR_FILTER_COMMS) );
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( STR_FILTER_EVENTS ), sharedProperties->isAllowed( whichTimeline, STR_FILTER_EVENTS) );

  Enable( FindItem( STR_PASTE ), sharedProperties->isAllowed( whichTimeline, STR_PASTE) );
  Enable( FindItem( STR_PASTE_SPECIAL ), sharedProperties->isAllowed( whichTimeline, STR_PASTE_SPECIAL) );

  Enable( FindItem( STR_CLONE ), true );

  Enable( FindItem( STR_FIT_TIME ), true );
  Enable( FindItem( STR_FIT_SEMANTIC ), true );
}


void gPopUpMenu::enableMenu( gHistogram *whichHistogram  )
{
  gPasteWindowProperties* sharedProperties = gPasteWindowProperties::pasteWindowProperties->getInstance();

  Enable( FindItem( STR_COPY ), true );

  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( STR_TIME ), sharedProperties->isAllowed( whichHistogram, STR_TIME) );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( STR_SIZE ), sharedProperties->isAllowed( whichHistogram, STR_SIZE)  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( STR_OBJECTS ), sharedProperties->isAllowed( whichHistogram, STR_OBJECTS)  );

  Enable( FindItem( STR_PASTE ), sharedProperties->isAllowed( whichHistogram, STR_PASTE) );
  Enable( FindItem( STR_PASTE_SPECIAL ), sharedProperties->isAllowed( whichHistogram, STR_PASTE_SPECIAL) );

  Enable( FindItem( STR_CLONE ), true );
  Enable( FindItem( STR_FIT_TIME ), true );
}


wxMultiChoiceDialog *gPopUpMenu::createPasteSpecialDialog( wxArrayString& choices, gHistogram *whichHistogram )
{
  wxArrayInt sel;
  int i = 0;

  choices.Empty();
  
  gPasteWindowProperties *pasteActions = gPasteWindowProperties::pasteWindowProperties->getInstance();
  
  if ( pasteActions->isAllowed( whichHistogram, string(STR_TIME) ) )
  {
    choices.Add(wxT(STR_TIME));
    sel.Add( i );
    ++i;
  }

  if ( pasteActions->isAllowed( whichHistogram, string(STR_OBJECTS) ) )
  {
    choices.Add(wxT(STR_OBJECTS));
    sel.Add( i );
    ++i;
  }

  if ( pasteActions->isAllowed( whichHistogram, string(STR_SIZE) ) )
  {
    choices.Add(wxT(STR_SIZE));
    sel.Add( i );
    ++i;
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
  
  if ( pasteActions->isAllowed( whichTimeline, string(STR_TIME) ) )
  {
    choices.Add(wxT(STR_TIME));
    sel.Add( i );
    ++i;
  }

  if ( pasteActions->isAllowed( whichTimeline, string(STR_OBJECTS) ) )
  {
    choices.Add(wxT(STR_OBJECTS));
    sel.Add( i );
    ++i;
  }

  if ( pasteActions->isAllowed( whichTimeline, string(STR_SIZE) ) )
  {
    choices.Add(wxT(STR_SIZE));
    sel.Add( i );
    ++i;
  }

  if ( pasteActions->isAllowed( whichTimeline, string(STR_FILTER_COMMS) ) )
    choices.Add(wxT(STR_FILTER_COMMS_XT));

  if ( pasteActions->isAllowed( whichTimeline, string(STR_FILTER_EVENTS) ) )
    choices.Add(wxT(STR_FILTER_EVENTS_XT));

  wxMultiChoiceDialog *tmpDialog = new wxMultiChoiceDialog( whichTimeline,
                                                            wxT( "Select properties to paste:" ),
                                                            wxT("Paste Special"),
                                                            choices );
  tmpDialog->SetSelections( sel );
  
  return tmpDialog;
}


wxMultiChoiceDialog *gPopUpMenu::createRowSelectionDialog( wxArrayString& choices, gTimeline *whichTimeline )
{
  choices.Empty();

  vector< bool > selectedRow;
  whichTimeline->GetMyWindow()->getSelectedRows( whichTimeline->GetMyWindow()->getLevel(), selectedRow );

  // Build full rows list
  for ( size_t row = (size_t)0; row < selectedRow.size(); ++row )
    choices.Add( wxT( LabelConstructor::objectLabel( (TObjectOrder)row,
                                                     whichTimeline->GetMyWindow()->getLevel(),
                                                     whichTimeline->GetMyWindow()->getTrace())));

  wxMultiChoiceDialog *myDialog = new wxMultiChoiceDialog( whichTimeline,
                                                         wxT( "Select visible rows:" ),
                                                         wxT("Rows Selection"),
                                                         choices );

  vector< TObjectOrder > selectedIndex;
  whichTimeline->GetMyWindow()->getSelectedRows( whichTimeline->GetMyWindow()->getLevel(), selectedIndex );

  wxArrayInt arrayIndex;
  for ( vector< TObjectOrder>::iterator index = selectedIndex.begin(); index != selectedIndex.end(); index++ )
    arrayIndex.Add( (int)*index );
  
  myDialog->SetSelections( arrayIndex );

  return myDialog;
}


gPopUpMenu::gPopUpMenu( gTimeline *whichTimeline )
{
  timeline = whichTimeline;
  histogram = NULL;

  popUpMenuPaint = new wxMenu;
  popUpMenuColor = new wxMenu;
  popUpMenuPaste = new wxMenu;
  popUpMenuPasteFilter = new wxMenu;
  popUpMenuFitSemantic = new wxMenu;
  popUpMenuDrawMode = new wxMenu;
  popUpMenuDrawModeTime = new wxMenu;
  popUpMenuDrawModeObjects = new wxMenu;
  popUpMenuDrawModeBoth = new wxMenu;

  buildItem( this, wxString(STR_COPY), ITEMNORMAL, NULL, ID_MENU_COPY );

  buildItem( popUpMenuPaste, wxString(STR_TIME), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuTime, ID_MENU_TIME );
  buildItem( popUpMenuPaste, wxString(STR_OBJECTS), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuObjects, ID_MENU_OBJECTS );
  buildItem( popUpMenuPaste, wxString(STR_SIZE), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuSize, ID_MENU_SIZE );

  buildItem( popUpMenuPasteFilter, wxString(STR_FILTER_ALL), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFilterAll, ID_MENU_FILTER_ALL );
  buildItem( popUpMenuPasteFilter, wxString(STR_FILTER_COMMS), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFilterComms, ID_MENU_FILTER_COMMS );
  buildItem( popUpMenuPasteFilter, wxString(STR_FILTER_EVENTS), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFilterEvents, ID_MENU_FILTER_EVENTS );
  popUpMenuPaste->AppendSubMenu( popUpMenuPasteFilter, wxString( STR_FILTER ));
  AppendSubMenu( popUpMenuPaste, wxString( STR_PASTE ));

  buildItem( this, wxString(STR_PASTE_SPECIAL), ITEMNORMAL, NULL, ID_MENU_PASTE_SPECIAL );
  buildItem( this, wxString(STR_CLONE), ITEMNORMAL, NULL, ID_MENU_CLONE );

  AppendSeparator();

  buildItem( this, wxString( "Undo Zoom" ), ITEMNORMAL, NULL, ID_MENU_UNDO_ZOOM );
  buildItem( this, wxString( "Redo Zoom" ), ITEMNORMAL, NULL, ID_MENU_REDO_ZOOM );

  AppendSeparator();

  buildItem( this, wxString( STR_FIT_TIME ), ITEMNORMAL, NULL, ID_MENU_FIT_TIME );
  buildItem( popUpMenuFitSemantic, wxString( "Fit Minimum" ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFitSemanticMin, ID_MENU_FIT_SEMANTIC_MIN);
  buildItem( popUpMenuFitSemantic, wxString( "Fit Maximum" ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFitSemanticMax, ID_MENU_FIT_SEMANTIC_MAX);
  buildItem( popUpMenuFitSemantic, wxString( "Fit Both" ), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuFitSemanticBoth, ID_MENU_FIT_SEMANTIC_BOTH);
  AppendSubMenu( popUpMenuFitSemantic, STR_FIT_SEMANTIC );

  AppendSeparator();

  buildItem( popUpMenuPaint, wxString( "Communication Lines" ), ITEMCHECK, (wxObjectEventFunction)&gPopUpMenu::OnMenuPaintCommunicationLines, ID_MENU_PAINT_COMMUNICATION_LINES, timeline->GetMyWindow()->getDrawCommLines() );
  buildItem( popUpMenuPaint, wxString( "Event Flags" ), ITEMCHECK, (wxObjectEventFunction)&gPopUpMenu::OnMenuPaintEventFlags, ID_MENU_PAINT_EVENT_FLAGS, timeline->GetMyWindow()->getDrawFlags() );
  AppendSubMenu( popUpMenuPaint, wxString( "Paint" ));

  buildItem( popUpMenuColor, wxString( "Code Color" ), ITEMRADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuCodeColor, ID_MENU_CODE_COLOR, timeline->GetMyWindow()->IsCodeColorSet() );
  buildItem( popUpMenuColor, wxString( "Gradient Color" ), ITEMRADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuGradientColor,ID_MENU_GRADIENT_COLOR, timeline->GetMyWindow()->IsGradientColorSet() );
  buildItem( popUpMenuColor, wxString( "Not Null Gradient Color" ), ITEMRADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuNotNullGradientColor,ID_MENU_NOT_NULL_GRADIENT_COLOR, timeline->GetMyWindow()->IsNotNullGradientColorSet() );
  AppendSubMenu( popUpMenuColor, wxString( "Color" ));

  buildItem( popUpMenuDrawModeTime,
             wxString( "Last" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeLast,
             ID_MENU_DRAWMODE_TIME_LAST,
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_LAST );
  buildItem( popUpMenuDrawModeTime,
             wxString( "Maximum" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeMaximum,
             ID_MENU_DRAWMODE_TIME_MAXIMUM,
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeTime,
             wxString( "Minimum not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeMinimumNotZero,
             ID_MENU_DRAWMODE_TIME_MINIMUM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeTime,
             wxString( "Random" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeRandom,
             ID_MENU_DRAWMODE_TIME_RANDOM,
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_RANDOM );
  buildItem( popUpMenuDrawModeTime,
             wxString( "Random not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeRandomNotZero,
             ID_MENU_DRAWMODE_TIME_RANDOM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeTime,
             wxString( "Average" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeAverage,
             ID_MENU_DRAWMODE_TIME_AVERAGE,
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_AVERAGE );

  buildItem( popUpMenuDrawModeObjects,
             wxString( "Last" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsLast,
             ID_MENU_DRAWMODE_OBJECTS_LAST,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_LAST );
  buildItem( popUpMenuDrawModeObjects,
             wxString( "Maximum" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsMaximum,
             ID_MENU_DRAWMODE_OBJECTS_MAXIMUM,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeObjects,
             wxString( "Minimum not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsMinimumNotZero,
             ID_MENU_DRAWMODE_OBJECTS_MINIMUM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             wxString( "Random" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsRandom,
             ID_MENU_DRAWMODE_OBJECTS_RANDOM,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_RANDOM );
  buildItem( popUpMenuDrawModeObjects,
             wxString( "Random not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsRandomNotZero,
             ID_MENU_DRAWMODE_OBJECTS_RANDOM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             wxString( "Average" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsAverage,
             ID_MENU_DRAWMODE_OBJECTS_AVERAGE,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_AVERAGE );

  buildItem( popUpMenuDrawModeBoth,
             wxString( "Last" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothLast,
             ID_MENU_DRAWMODE_BOTH_LAST,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_LAST &&
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_LAST );
  buildItem( popUpMenuDrawModeBoth,
             wxString( "Maximum" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothMaximum,
             ID_MENU_DRAWMODE_BOTH_MAXIMUM,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_MAXIMUM && 
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeBoth,
             wxString( "Minimum not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothMinimumNotZero,
             ID_MENU_DRAWMODE_BOTH_MINIMUM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_MINNOTZERO &&
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             wxString( "Random" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothRandom,
             ID_MENU_DRAWMODE_BOTH_RANDOM,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_RANDOM &&
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_RANDOM );
  buildItem( popUpMenuDrawModeBoth,
             wxString( "Random not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothRandomNotZero,
             ID_MENU_DRAWMODE_BOTH_RANDOM_NOT_ZERO,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_RANDNOTZERO &&
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             wxString( "Average" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothAverage,
             ID_MENU_DRAWMODE_BOTH_AVERAGE,
             timeline->GetMyWindow()->getDrawModeObject() == DRAW_AVERAGE &&
             timeline->GetMyWindow()->getDrawModeTime() == DRAW_AVERAGE );

  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeTime, wxString( "Time" ));
  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeObjects, wxString( "Objects" ));
  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeBoth, wxString( "Both" ));
  AppendSubMenu( popUpMenuDrawMode, wxString( "Drawmode" ));

  AppendSeparator();
  buildItem( this, wxString( "Select Rows" ), ITEMNORMAL, NULL, ID_MENU_ROW_SELECTION );

  AppendSeparator();
  buildItem( this, 
             wxString( "Info Panel" ),
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

  popUpMenuPaint = new wxMenu;
  popUpMenuColor = new wxMenu;
  popUpMenuPaste = new wxMenu;
  popUpMenuPasteFilter = new wxMenu;
  popUpMenuFitSemantic = new wxMenu;
  popUpMenuDrawMode = new wxMenu;
  popUpMenuDrawModeTime = new wxMenu;
  popUpMenuDrawModeObjects = new wxMenu;
  popUpMenuDrawModeBoth = new wxMenu;

  buildItem( this, wxString(STR_COPY), ITEMNORMAL, NULL, ID_MENU_COPY );

  buildItem( popUpMenuPaste, wxString(STR_TIME), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuTime, ID_MENU_TIME );
  buildItem( popUpMenuPaste, wxString(STR_OBJECTS), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuObjects, ID_MENU_OBJECTS );
  buildItem( popUpMenuPaste, wxString(STR_SIZE), ITEMNORMAL, (wxObjectEventFunction)&gPopUpMenu::OnMenuSize, ID_MENU_SIZE );

  AppendSubMenu( popUpMenuPaste, wxString( STR_PASTE ));

  buildItem( this, wxString(STR_PASTE_SPECIAL), ITEMNORMAL, NULL, ID_MENU_PASTE_SPECIAL );
  buildItem( this, wxString(STR_CLONE), ITEMNORMAL, NULL, ID_MENU_CLONE );

  AppendSeparator();

  buildItem( this, wxString( "Undo Zoom" ), ITEMNORMAL, NULL, ID_MENU_UNDO_ZOOM );
  buildItem( this, wxString( "Redo Zoom" ), ITEMNORMAL, NULL, ID_MENU_REDO_ZOOM );

  AppendSeparator();

  buildItem( this, wxString( STR_FIT_TIME ), ITEMNORMAL, NULL, ID_MENU_FIT_TIME );
  buildItem( this, 
             wxString( "Auto Fit Control Scale" ),
             ITEMCHECK,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuAutoControlScale,
             ID_MENU_AUTO_CONTROL_SCALE,
             histogram->GetHistogram()->getCompute2DScale() );
  if( histogram->GetHistogram()->getThreeDimensions() )
    buildItem( this, 
               wxString( "Auto Fit 3D Scale" ),
               ITEMCHECK,
               (wxObjectEventFunction)&gPopUpMenu::OnMenuAuto3DScale,
               ID_MENU_AUTO_3D_SCALE,
               histogram->GetHistogram()->getCompute3DScale() );
  buildItem( this, 
             wxString( "Auto Fit Data Gradient" ),
             ITEMCHECK,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuAutoDataGradient,
             ID_MENU_AUTO_DATA_GRADIENT,
             histogram->GetHistogram()->getComputeGradient() );

  AppendSeparator();
  
  buildItem( popUpMenuDrawModeTime,
             wxString( "Last" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeLast,
             ID_MENU_DRAWMODE_TIME_LAST,
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_LAST );
  buildItem( popUpMenuDrawModeTime,
             wxString( "Maximum" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeMaximum,
             ID_MENU_DRAWMODE_TIME_MAXIMUM,
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeTime,
             wxString( "Minimum not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeMinimumNotZero,
             ID_MENU_DRAWMODE_TIME_MINIMUM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeTime,
             wxString( "Random" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeRandom,
             ID_MENU_DRAWMODE_TIME_RANDOM,
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_RANDOM );
  buildItem( popUpMenuDrawModeTime,
             wxString( "Random not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeRandomNotZero,
             ID_MENU_DRAWMODE_TIME_RANDOM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeTime,
             wxString( "Average" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeTimeAverage,
             ID_MENU_DRAWMODE_TIME_AVERAGE,
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_AVERAGE );

  buildItem( popUpMenuDrawModeObjects,
             wxString( "Last" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsLast,
             ID_MENU_DRAWMODE_OBJECTS_LAST,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_LAST );
  buildItem( popUpMenuDrawModeObjects,
             wxString( "Maximum" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsMaximum,
             ID_MENU_DRAWMODE_OBJECTS_MAXIMUM,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeObjects,
             wxString( "Minimum not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsMinimumNotZero,
             ID_MENU_DRAWMODE_OBJECTS_MINIMUM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             wxString( "Random" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsRandom,
             ID_MENU_DRAWMODE_OBJECTS_RANDOM,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_RANDOM );
  buildItem( popUpMenuDrawModeObjects,
             wxString( "Random not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsRandomNotZero,
             ID_MENU_DRAWMODE_OBJECTS_RANDOM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeObjects,
             wxString( "Average" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeObjectsAverage,
             ID_MENU_DRAWMODE_OBJECTS_AVERAGE,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_AVERAGE );

  buildItem( popUpMenuDrawModeBoth,
             wxString( "Last" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothLast,
             ID_MENU_DRAWMODE_BOTH_LAST,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_LAST &&
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_LAST );
  buildItem( popUpMenuDrawModeBoth,
             wxString( "Maximum" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothMaximum,
             ID_MENU_DRAWMODE_BOTH_MAXIMUM,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_MAXIMUM && 
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_MAXIMUM );
  buildItem( popUpMenuDrawModeBoth,
             wxString( "Minimum not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothMinimumNotZero,
             ID_MENU_DRAWMODE_BOTH_MINIMUM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_MINNOTZERO &&
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_MINNOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             wxString( "Random" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothRandom,
             ID_MENU_DRAWMODE_BOTH_RANDOM,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_RANDOM &&
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_RANDOM );
  buildItem( popUpMenuDrawModeBoth,
             wxString( "Random not zero" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothRandomNotZero,
             ID_MENU_DRAWMODE_BOTH_RANDOM_NOT_ZERO,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_RANDNOTZERO &&
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_RANDNOTZERO );
  buildItem( popUpMenuDrawModeBoth,
             wxString( "Average" ),
             ITEMRADIO,
             (wxObjectEventFunction)&gPopUpMenu::OnMenuDrawModeBothAverage,
             ID_MENU_DRAWMODE_BOTH_AVERAGE,
             histogram->GetHistogram()->getDrawModeObjects() == DRAW_AVERAGE &&
             histogram->GetHistogram()->getDrawModeColumns() == DRAW_AVERAGE );

  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeTime, wxString( "Semantic" ));
  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeObjects, wxString( "Objects" ));
  popUpMenuDrawMode->AppendSubMenu( popUpMenuDrawModeBoth, wxString( "Both" ));
  AppendSubMenu( popUpMenuDrawMode, wxString( "Drawmode" ));

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
      Enable( FindItem( STR_PASTE ),
              gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( histogram, STR_PASTE ));
      Enable( FindItem( STR_PASTE_SPECIAL ),
              gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( histogram, STR_PASTE_SPECIAL ));
    }
    Enable( FindItem( tag ), gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( histogram, tag ));
  }
  else
  {
    if ( checkPaste )
    {
      Enable( FindItem( STR_PASTE ),
              gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( timeline, STR_PASTE ));
      Enable( FindItem( STR_PASTE_SPECIAL ),
              gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( timeline, STR_PASTE_SPECIAL ));

    }
    Enable( FindItem( tag ), gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( timeline, tag ));
  }
}


void gPopUpMenu::enable( const string tag, bool enable )
{
  Enable( FindItem( tag ), enable );
}


void gPopUpMenu::enable( const string tag )
{
  Enable( FindItem( tag ), true );
}


void gPopUpMenu::disable( const string tag )
{
  Enable( FindItem( tag ), false );
}


string gPopUpMenu::getOption( wxArrayString& choices, int position )
{
  if ( choices[ position ].Cmp( STR_FILTER_COMMS_XT ) == 0 )
    return string( STR_FILTER_COMMS );
  else if ( choices[ position ].Cmp( STR_FILTER_EVENTS_XT ) == 0 )
    return string( STR_FILTER_EVENTS );
  else  
    return string( choices[ position ] );
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
}

void gPopUpMenu::OnMenuRedoZoom( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpRedoZoom();
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


void gPopUpMenu::OnMenuPaintCommunicationLines( wxCommandEvent& event )
{
  if ( timeline != NULL )
    timeline->drawCommunicationLines( event.IsChecked() );
}

void gPopUpMenu::OnMenuPaintEventFlags( wxCommandEvent& event )
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

void gPopUpMenu::OnMenuRowSelection( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpRowSelection();
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
