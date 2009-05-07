#include "popupmenu.h"
#include "copypaste.h"
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
  EVT_MENU( ID_MENU_FIT_SEMANTIC, gPopUpMenu::OnMenuFitSemantic )
  EVT_MENU( ID_MENU_CODE_COLOR, gPopUpMenu::OnMenuCodeColor )
  EVT_MENU( ID_MENU_GRADIENT_COLOR, gPopUpMenu::OnMenuGradientColor )
  EVT_MENU( ID_MENU_NOT_NULL_GRADIENT_COLOR, gPopUpMenu::OnMenuNotNullGradientColor )
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
  Enable( FindItem( STR_FIT_SEMANTIC ), true );
}


wxMultiChoiceDialog *gPopUpMenu::createPasteSpecialDialog( wxArrayString& choices, gHistogram *whichHistogram )
{
  choices.Empty();
  
  choices.Add(wxT(STR_TIME));
  choices.Add(wxT(STR_OBJECTS));
  choices.Add(wxT(STR_SIZE));
  return new wxMultiChoiceDialog( whichHistogram,
                                  wxT( "Select properties to paste:" ),
                                  wxT("Paste Special"),
                                  choices);
}

wxMultiChoiceDialog *gPopUpMenu::createPasteSpecialDialog( wxArrayString& choices, gTimeline *whichTimeline )
{
  choices.Empty();
  gPasteWindowProperties *pasteActions = gPasteWindowProperties::pasteWindowProperties->getInstance();
  
  if ( pasteActions->isAllowed( whichTimeline, string(STR_TIME) ) )
    choices.Add(wxT(STR_TIME));

  if ( pasteActions->isAllowed( whichTimeline, string(STR_OBJECTS) ) )
    choices.Add(wxT(STR_OBJECTS));

  if ( pasteActions->isAllowed( whichTimeline, string(STR_SIZE) ) )
    choices.Add(wxT(STR_SIZE));

  if ( pasteActions->isAllowed( whichTimeline, string(STR_FILTER_COMMS) ) )
    choices.Add(wxT(STR_FILTER_COMMS_XT));

  if ( pasteActions->isAllowed( whichTimeline, string(STR_FILTER_EVENTS) ) )
    choices.Add(wxT(STR_FILTER_EVENTS_XT));

  return new wxMultiChoiceDialog( whichTimeline,
                                  wxT( "Select properties to paste:" ),
                                  wxT("Paste Special"),
                                  choices );
}


gPopUpMenu::gPopUpMenu( gTimeline *whichTimeline )
{
  timeline = whichTimeline;
  histogram = NULL;

  popUpMenuColor = new wxMenu;
  popUpMenuPaste = new wxMenu;
  popUpMenuPasteFilter = new wxMenu;
  
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
  buildItem( this, wxString( STR_FIT_SEMANTIC ), ITEMNORMAL, NULL, ID_MENU_FIT_SEMANTIC);

  AppendSeparator();

  buildItem( popUpMenuColor, wxString( "Code Color" ), ITEMRADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuCodeColor, ID_MENU_CODE_COLOR, timeline->GetMyWindow()->IsCodeColorSet() );
  buildItem( popUpMenuColor, wxString( "Gradient Color" ), ITEMRADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuGradientColor,ID_MENU_GRADIENT_COLOR, timeline->GetMyWindow()->IsGradientColorSet() );
  buildItem( popUpMenuColor, wxString( "Not Null Gradient Color" ), ITEMRADIO, (wxObjectEventFunction)&gPopUpMenu::OnMenuNotNullGradientColor,ID_MENU_NOT_NULL_GRADIENT_COLOR, timeline->GetMyWindow()->IsNotNullGradientColorSet() );
  AppendSubMenu( popUpMenuColor, wxString( "Color" ));

  enableMenu( timeline );
}


gPopUpMenu::gPopUpMenu( gHistogram *whichHistogram )
{
  timeline = NULL;
  histogram = whichHistogram;

  popUpMenuColor = new wxMenu;
  popUpMenuPaste = new wxMenu;
  popUpMenuPasteFilter = new wxMenu;

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
  buildItem( this, wxString( STR_FIT_SEMANTIC ), ITEMNORMAL, NULL, ID_MENU_FIT_SEMANTIC );

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

void gPopUpMenu::OnMenuFitSemantic( wxCommandEvent& event)
{
  if ( timeline != NULL )
    timeline->OnPopUpFitSemanticScale();
  else
    histogram->OnPopUpFitSemanticScale();
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

