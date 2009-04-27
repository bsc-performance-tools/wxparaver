#include "popupmenu.h"
#include "copypaste.h"
#include <wx/event.h>
#include <iostream>
using namespace std;
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
    popUpMenu->Connect(tmpid, wxEVT_COMMAND_MENU_SELECTED, handler, NULL, histogram );
  else
    popUpMenu->Connect(tmpid, wxEVT_COMMAND_MENU_SELECTED, handler, NULL, timeline );
}


void gPopUpMenu::enableMenu( gTimeline *whichTimeline )
{
  gPasteWindowProperties* sharedProperties = gPasteWindowProperties::pasteWindowProperties->getInstance();

  popUpMenu->Enable( popUpMenu->FindItem( STR_COPY ), true );

  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( STR_TIME ), sharedProperties->isAllowed( whichTimeline, STR_TIME) );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( STR_SIZE ), sharedProperties->isAllowed( whichTimeline, STR_SIZE)  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( STR_OBJECTS ), sharedProperties->isAllowed( whichTimeline, STR_OBJECTS)  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( STR_FILTER ), sharedProperties->isAllowed( whichTimeline, STR_FILTER) );
  
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( STR_FILTER_ALL ), sharedProperties->isAllowed( whichTimeline, STR_FILTER_ALL) );
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( STR_FILTER_COMMS ), sharedProperties->isAllowed( whichTimeline, STR_FILTER_COMMS) );
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( STR_FILTER_EVENTS ), sharedProperties->isAllowed( whichTimeline, STR_FILTER_EVENTS) );

  popUpMenu->Enable( popUpMenu->FindItem( STR_PASTE ), sharedProperties->isAllowed( whichTimeline, STR_PASTE) );
  popUpMenu->Enable( popUpMenu->FindItem( STR_PASTE_SPECIAL ), sharedProperties->isAllowed( whichTimeline, STR_PASTE_SPECIAL) );

  popUpMenu->Enable( popUpMenu->FindItem( STR_CLONE ), true );

  popUpMenu->Enable( popUpMenu->FindItem( STR_FIT_TIME ), true );
  popUpMenu->Enable( popUpMenu->FindItem( STR_FIT_SEMANTIC ), true );
}


void gPopUpMenu::enableMenu( gHistogram *whichHistogram  )
{
  gPasteWindowProperties* sharedProperties = gPasteWindowProperties::pasteWindowProperties->getInstance();

  popUpMenu->Enable( popUpMenu->FindItem( STR_COPY ), true );

  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( STR_TIME ), sharedProperties->isAllowed( whichHistogram, STR_TIME) );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( STR_SIZE ), sharedProperties->isAllowed( whichHistogram, STR_SIZE)  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( STR_OBJECTS ), sharedProperties->isAllowed( whichHistogram, STR_OBJECTS)  );

  popUpMenu->Enable( popUpMenu->FindItem( STR_PASTE ), sharedProperties->isAllowed( whichHistogram, STR_PASTE) );
  popUpMenu->Enable( popUpMenu->FindItem( STR_PASTE_SPECIAL ), sharedProperties->isAllowed( whichHistogram, STR_PASTE_SPECIAL) );

  popUpMenu->Enable( popUpMenu->FindItem( STR_CLONE ), true );
  popUpMenu->Enable( popUpMenu->FindItem( STR_FIT_TIME ), true );
  popUpMenu->Enable( popUpMenu->FindItem( STR_FIT_SEMANTIC ), true );
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

  popUpMenu = new wxMenu;
  popUpMenuColor = new wxMenu;
  popUpMenuPaste = new wxMenu;
  popUpMenuPasteFilter = new wxMenu;
  
  buildItem( popUpMenu, wxString(STR_COPY), (wxObjectEventFunction)&gTimeline::OnPopUpCopy, ITEMNORMAL );

  buildItem( popUpMenuPaste, wxString(STR_TIME), (wxObjectEventFunction)&gTimeline::OnPopUpPasteTime, ITEMNORMAL );
  buildItem( popUpMenuPaste, wxString(STR_OBJECTS), (wxObjectEventFunction)&gTimeline::OnPopUpPasteObjects, ITEMNORMAL );
  buildItem( popUpMenuPaste, wxString(STR_SIZE), (wxObjectEventFunction)&gTimeline::OnPopUpPasteSize, ITEMNORMAL );

  buildItem( popUpMenuPasteFilter, wxString(STR_FILTER_ALL), (wxObjectEventFunction)&gTimeline::OnPopUpPasteFilterAll, ITEMNORMAL );
  buildItem( popUpMenuPasteFilter, wxString(STR_FILTER_COMMS), (wxObjectEventFunction)&gTimeline::OnPopUpPasteFilterCommunications, ITEMNORMAL );
  buildItem( popUpMenuPasteFilter, wxString(STR_FILTER_EVENTS), (wxObjectEventFunction)&gTimeline::OnPopUpPasteFilterEvents, ITEMNORMAL );
  popUpMenuPaste->AppendSubMenu( popUpMenuPasteFilter, wxString( STR_FILTER ));
  popUpMenu->AppendSubMenu( popUpMenuPaste, wxString( STR_PASTE ));

  buildItem( popUpMenu, wxString(STR_PASTE_SPECIAL), (wxObjectEventFunction)&gTimeline::OnPopUpPasteSpecial, ITEMNORMAL );
  buildItem( popUpMenu, wxString(STR_CLONE), (wxObjectEventFunction)&gTimeline::OnPopUpClone, ITEMNORMAL );

  popUpMenu->AppendSeparator();

  buildItem( popUpMenu, wxString( "Undo Zoom" ), ( wxObjectEventFunction )&gTimeline::OnPopUpUndoZoom, ITEMNORMAL );
  buildItem( popUpMenu, wxString( "Redo Zoom" ), ( wxObjectEventFunction )&gTimeline::OnPopUpRedoZoom, ITEMNORMAL );

  popUpMenu->AppendSeparator();

  buildItem( popUpMenu, wxString( STR_FIT_TIME ), ( wxObjectEventFunction )&gTimeline::OnPopUpFitTimeScale, ITEMNORMAL );
  buildItem( popUpMenu, wxString( STR_FIT_SEMANTIC ), ( wxObjectEventFunction )&gTimeline::OnPopUpFitSemanticScale, ITEMNORMAL );

  popUpMenu->AppendSeparator();

  buildItem( popUpMenuColor, wxString( "Code Color" ), ( wxObjectEventFunction )&gTimeline::OnPopUpCodeColor, ITEMRADIO, timeline->GetMyWindow()->IsCodeColorSet() );
  buildItem( popUpMenuColor, wxString( "Gradient Color" ), ( wxObjectEventFunction )&gTimeline::OnPopUpGradientColor, ITEMRADIO, timeline->GetMyWindow()->IsGradientColorSet() );
  popUpMenu->AppendSubMenu( popUpMenuColor, wxString( "Color" ));

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

  buildItem( popUpMenu, wxString(STR_COPY), (wxObjectEventFunction)&gHistogram::OnPopUpCopy, ITEMNORMAL );

  buildItem( popUpMenuPaste, wxString(STR_TIME), (wxObjectEventFunction)&gHistogram::OnPopUpPasteTime, ITEMNORMAL );
  buildItem( popUpMenuPaste, wxString(STR_OBJECTS), (wxObjectEventFunction)&gHistogram::OnPopUpPasteObjects, ITEMNORMAL );
  buildItem( popUpMenuPaste, wxString(STR_SIZE), (wxObjectEventFunction)&gHistogram::OnPopUpPasteSize, ITEMNORMAL );

  popUpMenu->AppendSubMenu( popUpMenuPaste, wxString( STR_PASTE ));

  buildItem( popUpMenu, wxString(STR_PASTE_SPECIAL), (wxObjectEventFunction)&gHistogram::OnPopUpPasteSpecial, ITEMNORMAL );
  buildItem( popUpMenu, wxString(STR_CLONE), (wxObjectEventFunction)&gHistogram::OnPopUpClone, ITEMNORMAL );

  popUpMenu->AppendSeparator();

  buildItem( popUpMenu, wxString( "Undo Zoom" ), ( wxObjectEventFunction )&gHistogram::OnPopUpUndoZoom, ITEMNORMAL );
  buildItem( popUpMenu, wxString( "Redo Zoom" ), ( wxObjectEventFunction )&gHistogram::OnPopUpRedoZoom, ITEMNORMAL );

  popUpMenu->AppendSeparator();

  buildItem( popUpMenu, wxString( STR_FIT_TIME ), ( wxObjectEventFunction )&gHistogram::OnPopUpFitTimeScale, ITEMNORMAL );
  buildItem( popUpMenu, wxString( STR_FIT_SEMANTIC ), ( wxObjectEventFunction )&gHistogram::OnPopUpFitSemanticScale, ITEMNORMAL );

  enableMenu( histogram );
}

gPopUpMenu::~gPopUpMenu()
{
  delete popUpMenu;
}

void gPopUpMenu::enablePaste( const string tag, bool checkPaste )
{
  if ( timeline == NULL )
  {
    if ( checkPaste )
    {
      popUpMenu->Enable( popUpMenu->FindItem( STR_PASTE ),
                         gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( histogram, STR_PASTE ));
      popUpMenu->Enable( popUpMenu->FindItem( STR_PASTE_SPECIAL ),
                         gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( histogram, STR_PASTE_SPECIAL ));
    }
    popUpMenu->Enable( popUpMenu->FindItem( tag ), gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( histogram, tag ));
  }
  else
  {
    if ( checkPaste )
    {
      popUpMenu->Enable( popUpMenu->FindItem( STR_PASTE ),
                         gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( timeline, STR_PASTE ));
      popUpMenu->Enable( popUpMenu->FindItem( STR_PASTE_SPECIAL ),
                         gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( timeline, STR_PASTE_SPECIAL ));

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


string gPopUpMenu::getOption( wxArrayString& choices, int position )
{
  if ( choices[ position ].compare( STR_FILTER_COMMS_XT ))
    return string( STR_FILTER_COMMS );
  else if ( choices[ position ].compare( STR_FILTER_EVENTS_XT ))
    return string( STR_FILTER_EVENTS );
  else  
    return string( choices[ position ] );
}


