#include "popupmenu.h"
#include "copypaste.h"
#include <wx/event.h>

void gPopUpMenu::buildItem( wxMenu *popUp,
                            const wxString &title,
                            wxObjectEventFunction handler,
                            ItemType itemType,
                            bool checked )
{
  wxMenuItem *tmp;

  switch ( itemType )
  {
    case ITEMNORMAL:
    {
      tmp = new wxMenuItem( popUp, wxID_ANY, title, "", wxITEM_NORMAL );
      break;
    }
    case ITEMRADIO:
    {
      tmp = new wxMenuItem( popUp, wxID_ANY, title, "", wxITEM_RADIO );
      break;
    }
    case ITEMCHECK:
    {
      tmp = new wxMenuItem( popUp, wxID_ANY, title, "", wxITEM_CHECK );
      break;
    }
    default:
    {
      tmp = new wxMenuItem( popUp, wxID_ANY, title, "", wxITEM_NORMAL );
      break;
    }
  }

  int tmpid = tmp->GetId();
  popUp->Append( tmp );
  if ( tmp->IsCheckable() )
    tmp->Check( checked );

//  popUp->Connect(tmpid, wxEVT_COMMAND_MENU_SELECTED, handler, tmp, this );

  if ( timeline == NULL )
    popUp->Connect(tmpid, wxEVT_COMMAND_MENU_SELECTED, handler, NULL, histogram );
  else
    popUp->Connect(tmpid, wxEVT_COMMAND_MENU_SELECTED, handler, NULL, timeline );
}


void gPopUpMenu::enableMenu( gTimeline *whichTimeline )
{
  gPasteWindowProperties* sharedProperties = gPasteWindowProperties::pasteWindowProperties->getInstance();

  popUpMenu->Enable( popUpMenu->FindItem( ST_COPY ), true );

  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( ST_TIME ), sharedProperties->isAllowed( timeline, ST_TIME) );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( ST_SIZE ), sharedProperties->isAllowed( timeline, ST_SIZE)  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( ST_OBJECTS ), sharedProperties->isAllowed( timeline, ST_OBJECTS)  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( ST_FILTER ), sharedProperties->isAllowed( timeline, ST_FILTER) );
  
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( ST_FILTER_ALL ), sharedProperties->isAllowed( timeline, ST_FILTER_ALL) );
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( ST_FILTER_COMMS ), sharedProperties->isAllowed( timeline, ST_FILTER_COMMS) );
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( ST_FILTER_EVENTS ), sharedProperties->isAllowed( timeline, ST_FILTER_EVENTS) );

  popUpMenu->Enable( popUpMenu->FindItem( ST_PASTE ), sharedProperties->isAllowed( timeline, ST_PASTE) );
  popUpMenu->Enable( popUpMenu->FindItem( ST_PASTE_SPECIAL ), sharedProperties->isAllowed( timeline, ST_PASTE_SPECIAL) );

  popUpMenu->Enable( popUpMenu->FindItem( ST_CLONE ), false ); // when implemented, set to true

  popUpMenu->Enable( popUpMenu->FindItem( ST_FIT_TIME ), true );
  popUpMenu->Enable( popUpMenu->FindItem( ST_FIT_SEMANTIC ), true );
}


void gPopUpMenu::enableMenu( gHistogram *whichHistogram  )
{
  gPasteWindowProperties* sharedProperties = gPasteWindowProperties::pasteWindowProperties->getInstance();

  popUpMenu->Enable( popUpMenu->FindItem( ST_COPY ), true );

  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( ST_TIME ), sharedProperties->isAllowed( histogram, ST_TIME) );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( ST_SIZE ), sharedProperties->isAllowed( histogram, ST_SIZE)  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( ST_OBJECTS ), sharedProperties->isAllowed( histogram, ST_OBJECTS)  );

  popUpMenu->Enable( popUpMenu->FindItem( ST_PASTE ), sharedProperties->isAllowed( histogram, ST_PASTE) );
  popUpMenu->Enable( popUpMenu->FindItem( ST_PASTE_SPECIAL ), sharedProperties->isAllowed( histogram, ST_PASTE_SPECIAL) );
  
  popUpMenu->Enable( popUpMenu->FindItem( ST_CLONE ), false ); // when implemented, set to true
  popUpMenu->Enable( popUpMenu->FindItem( ST_FIT_TIME ), true );
  popUpMenu->Enable( popUpMenu->FindItem( ST_FIT_SEMANTIC ), true );
}


gPopUpMenu::gPopUpMenu( gTimeline *whichTimeline )
{
  timeline = whichTimeline;
  histogram = NULL;

  popUpMenu = new wxMenu;
  popUpMenuColor = new wxMenu;
  popUpMenuPaste = new wxMenu;
  popUpMenuPasteFilter = new wxMenu;

  buildItem( popUpMenu, wxString(ST_COPY), (wxObjectEventFunction)&gTimeline::OnPopUpCopy, ITEMNORMAL );

  buildItem( popUpMenuPaste, wxString(ST_TIME), (wxObjectEventFunction)&gTimeline::OnPopUpPasteTime, ITEMNORMAL );
  buildItem( popUpMenuPaste, wxString(ST_OBJECTS), (wxObjectEventFunction)&gTimeline::OnPopUpPasteObjects, ITEMNORMAL );
  buildItem( popUpMenuPaste, wxString(ST_SIZE), (wxObjectEventFunction)&gTimeline::OnPopUpPasteSize, ITEMNORMAL );

  buildItem( popUpMenuPasteFilter, wxString(ST_FILTER_ALL), (wxObjectEventFunction)&gTimeline::OnPopUpPasteFilterAll, ITEMNORMAL );
  buildItem( popUpMenuPasteFilter, wxString(ST_FILTER_COMMS), (wxObjectEventFunction)&gTimeline::OnPopUpPasteFilterCommunications, ITEMNORMAL );
  buildItem( popUpMenuPasteFilter, wxString(ST_FILTER_EVENTS), (wxObjectEventFunction)&gTimeline::OnPopUpPasteFilterEvents, ITEMNORMAL );
  popUpMenuPaste->AppendSubMenu( popUpMenuPasteFilter, wxString( ST_FILTER ));
  popUpMenu->AppendSubMenu( popUpMenuPaste, wxString( ST_PASTE ));

  buildItem( popUpMenu, wxString(ST_PASTE_SPECIAL), (wxObjectEventFunction)&gTimeline::OnPopUpPasteSpecial, ITEMNORMAL );
  buildItem( popUpMenu, wxString(ST_CLONE), (wxObjectEventFunction)&gTimeline::OnPopUpClone, ITEMNORMAL );

  popUpMenu->AppendSeparator();

  buildItem( popUpMenu, wxString( "Undo Zoom" ), ( wxObjectEventFunction )&gTimeline::OnPopUpUndoZoom, ITEMNORMAL );
  buildItem( popUpMenu, wxString( "Redo Zoom" ), ( wxObjectEventFunction )&gTimeline::OnPopUpRedoZoom, ITEMNORMAL );

  popUpMenu->AppendSeparator();

  buildItem( popUpMenu, wxString( ST_FIT_TIME ), ( wxObjectEventFunction )&gTimeline::OnPopUpFitTimeScale, ITEMNORMAL );
  buildItem( popUpMenu, wxString( ST_FIT_SEMANTIC ), ( wxObjectEventFunction )&gTimeline::OnPopUpFitSemanticScale, ITEMNORMAL );

  popUpMenu->AppendSeparator();

  buildItem( popUpMenuColor, wxString( "Code Color" ), ( wxObjectEventFunction )&gTimeline::OnPopUpCodeColor, ITEMRADIO, timeline->GetMyWindow()->IsCodeColorSet() );
  buildItem( popUpMenuColor, wxString( "Gradient Color" ), ( wxObjectEventFunction )&gTimeline::OnPopUpGradientColor, ITEMRADIO, timeline->GetMyWindow()->IsGradientColorSet() );
  popUpMenu->AppendSubMenu( popUpMenuColor, wxString( "Color" ));

  choices.Add(wxT(ST_TIME));
  choices.Add(wxT(ST_OBJECTS));
  choices.Add(wxT(ST_SIZE));
  choices.Add(wxT(ST_FILTER_COMMS_XT));
  choices.Add(wxT(ST_FILTER_EVENTS_XT));
  popUpMenuPasteDialog = new wxMultiChoiceDialog( timeline,
                                                  wxT( "Select pasted properties:" ),
                                                  wxT("Paste Special"),
                                                  choices);
  enableMenu( timeline );
}


gPopUpMenu::gPopUpMenu( gHistogram *whichHistogram )
{
  timeline = NULL;
  histogram = whichHistogram;

  popUpMenu = new wxMenu;
  popUpMenuColor = new wxMenu;
  popUpMenuPaste = new wxMenu;
  popUpMenuPasteFilter = new wxMenu;

  buildItem( popUpMenu, wxString(ST_COPY), (wxObjectEventFunction)&gHistogram::OnPopUpCopy, ITEMNORMAL );

  buildItem( popUpMenuPaste, wxString(ST_TIME), (wxObjectEventFunction)&gHistogram::OnPopUpPasteTime, ITEMNORMAL );
  buildItem( popUpMenuPaste, wxString(ST_OBJECTS), (wxObjectEventFunction)&gHistogram::OnPopUpPasteObjects, ITEMNORMAL );
  buildItem( popUpMenuPaste, wxString(ST_SIZE), (wxObjectEventFunction)&gHistogram::OnPopUpPasteSize, ITEMNORMAL );

  popUpMenu->AppendSubMenu( popUpMenuPaste, wxString( ST_PASTE ));

  buildItem( popUpMenu, wxString(ST_PASTE_SPECIAL), (wxObjectEventFunction)&gHistogram::OnPopUpPasteSpecial, ITEMNORMAL );
  buildItem( popUpMenu, wxString(ST_CLONE), (wxObjectEventFunction)&gHistogram::OnPopUpClone, ITEMNORMAL );

  popUpMenu->AppendSeparator();

  buildItem( popUpMenu, wxString( "Undo Zoom" ), ( wxObjectEventFunction )&gHistogram::OnPopUpUndoZoom, ITEMNORMAL );
  buildItem( popUpMenu, wxString( "Redo Zoom" ), ( wxObjectEventFunction )&gHistogram::OnPopUpRedoZoom, ITEMNORMAL );

  popUpMenu->AppendSeparator();

  buildItem( popUpMenu, wxString( ST_FIT_TIME ), ( wxObjectEventFunction )&gHistogram::OnPopUpFitTimeScale, ITEMNORMAL );
  buildItem( popUpMenu, wxString( ST_FIT_SEMANTIC ), ( wxObjectEventFunction )&gHistogram::OnPopUpFitSemanticScale, ITEMNORMAL );

  choices.Add(wxT(ST_TIME));
  choices.Add(wxT(ST_OBJECTS));
  choices.Add(wxT(ST_SIZE));
  popUpMenuPasteDialog = new wxMultiChoiceDialog( histogram,
                                                  wxT( "Select pasted properties:" ),
                                                  wxT("Paste Special"),
                                                  choices);
  enableMenu( histogram );
}


void gPopUpMenu::enablePaste( const string tag, bool checkPaste )
{
  if ( timeline == NULL )
  {
    if ( checkPaste )
    {
      popUpMenu->Enable( popUpMenu->FindItem( ST_PASTE ),
                         gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( histogram, ST_PASTE ));
      popUpMenu->Enable( popUpMenu->FindItem( ST_PASTE_SPECIAL ),
                         gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( histogram, ST_PASTE_SPECIAL ));
    }
    popUpMenu->Enable( popUpMenu->FindItem( tag ), gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( histogram, tag ));
  }
  else
  {
    if ( checkPaste )
    {
      popUpMenu->Enable( popUpMenu->FindItem( ST_PASTE ),
                         gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( timeline, ST_PASTE ));
      popUpMenu->Enable( popUpMenu->FindItem( ST_PASTE_SPECIAL ),
                         gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( timeline, ST_PASTE_SPECIAL ));

    }
    popUpMenu->Enable( popUpMenu->FindItem( tag ), gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( timeline, tag ));
  }
}


void gPopUpMenu::enable( const string tag, bool enable )
{
  popUpMenu->Enable( popUpMenu->FindItem( tag ), enable );
}


void gPopUpMenu::enable( const string tag )
{
  popUpMenu->Enable( popUpMenu->FindItem( tag ), true );
}


void gPopUpMenu::disable( const string tag )
{
  popUpMenu->Enable( popUpMenu->FindItem( tag ), false );
}


string gPopUpMenu::getOption( int position )
{
  if ( choices[ position ].compare( ST_FILTER_COMMS_XT ))
    return string( ST_FILTER_COMMS );
  else if ( choices[ position ].compare( ST_FILTER_EVENTS_XT ))
    return string( ST_FILTER_EVENTS );
  else  
    return string( choices[ position ] );
}


bool gPopUpMenu::okPressed( )
{
  return ( popUpMenuPasteDialog->ShowModal() == wxID_OK );
}


wxArrayInt gPopUpMenu::getSelections()
{
  return popUpMenuPasteDialog->GetSelections();
}
