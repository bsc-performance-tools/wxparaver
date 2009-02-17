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

  popUpMenu->Enable( popUpMenu->FindItem( "Copy" ), true );

  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( "Time" ), sharedProperties->isAllowed( timeline, "Time") );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( "Size" ), sharedProperties->isAllowed( timeline, "Size")  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( "Objects" ), sharedProperties->isAllowed( timeline, "Objects")  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( "Filter" ), sharedProperties->isAllowed( timeline, "Filter") );
  
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( "All" ), sharedProperties->isAllowed( timeline, "Filter All") );
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( "Communications" ), sharedProperties->isAllowed( timeline, "Communications") );
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( "Events" ), sharedProperties->isAllowed( timeline, "Events") );

  popUpMenu->Enable( popUpMenu->FindItem( "Paste" ), sharedProperties->isAllowed( timeline, "Paste") );
  popUpMenu->Enable( popUpMenu->FindItem( "Paste Special..." ), sharedProperties->isAllowed( timeline, "Paste Special...") );

  popUpMenu->Enable( popUpMenu->FindItem( "Clone" ), false ); // when implemented, set to true

  popUpMenu->Enable( popUpMenu->FindItem( "Fit Time Scale" ), true );
  popUpMenu->Enable( popUpMenu->FindItem( "Fit Semantic Scale" ), true );
}


void gPopUpMenu::enableMenu( gHistogram *whichHistogram  )
{
  gPasteWindowProperties* sharedProperties = gPasteWindowProperties::pasteWindowProperties->getInstance();

  popUpMenu->Enable( popUpMenu->FindItem( "Copy" ), true );

  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( "Time" ), sharedProperties->isAllowed( histogram, "Time") );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( "Size" ), sharedProperties->isAllowed( histogram, "Size")  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( "Objects" ), sharedProperties->isAllowed( histogram, "Objects")  );

  popUpMenu->Enable( popUpMenu->FindItem( "Paste" ), sharedProperties->isAllowed( histogram, "Paste") );
  popUpMenu->Enable( popUpMenu->FindItem( "Paste Special..." ), sharedProperties->isAllowed( histogram, "Paste Special...") );
  
  popUpMenu->Enable( popUpMenu->FindItem( "Clone" ), false ); // when implemented, set to true
  popUpMenu->Enable( popUpMenu->FindItem( "Fit Time Scale" ), true );
  popUpMenu->Enable( popUpMenu->FindItem( "Fit Semantic Scale" ), true );
}


gPopUpMenu::gPopUpMenu( gTimeline *whichTimeline )
{
  timeline = whichTimeline;
  histogram = NULL;

  popUpMenu = new wxMenu;
  popUpMenuColor = new wxMenu;
  popUpMenuPaste = new wxMenu;
  popUpMenuPasteFilter = new wxMenu;

  buildItem( popUpMenu, wxString("Copy"), (wxObjectEventFunction)&gTimeline::OnPopUpCopy, ITEMNORMAL );

  buildItem( popUpMenuPaste, wxString("Time"), (wxObjectEventFunction)&gTimeline::OnPopUpPasteTime, ITEMNORMAL );
  buildItem( popUpMenuPaste, wxString("Objects"), (wxObjectEventFunction)&gTimeline::OnPopUpPasteObjects, ITEMNORMAL );
  buildItem( popUpMenuPaste, wxString("Size"), (wxObjectEventFunction)&gTimeline::OnPopUpPasteSize, ITEMNORMAL );

  buildItem( popUpMenuPasteFilter, wxString("All"), (wxObjectEventFunction)&gTimeline::OnPopUpPasteFilterAll, ITEMNORMAL );
  buildItem( popUpMenuPasteFilter, wxString("Communications"), (wxObjectEventFunction)&gTimeline::OnPopUpPasteFilterCommunications, ITEMNORMAL );
  buildItem( popUpMenuPasteFilter, wxString("Events"), (wxObjectEventFunction)&gTimeline::OnPopUpPasteFilterEvents, ITEMNORMAL );
  popUpMenuPaste->AppendSubMenu( popUpMenuPasteFilter, wxString( "Filter" ));
  popUpMenu->AppendSubMenu( popUpMenuPaste, wxString( "Paste" ));

  buildItem( popUpMenu, wxString("Paste Special..."), (wxObjectEventFunction)&gTimeline::OnPopUpPasteSpecial, ITEMNORMAL );
  buildItem( popUpMenu, wxString("Clone"), (wxObjectEventFunction)&gTimeline::OnPopUpClone, ITEMNORMAL );

  popUpMenu->AppendSeparator();

  buildItem( popUpMenu, wxString( "Undo Zoom" ), ( wxObjectEventFunction )&gTimeline::OnPopUpUndoZoom, ITEMNORMAL );
  buildItem( popUpMenu, wxString( "Redo Zoom" ), ( wxObjectEventFunction )&gTimeline::OnPopUpRedoZoom, ITEMNORMAL );

  popUpMenu->AppendSeparator();

  buildItem( popUpMenu, wxString( "Fit Time Scale" ), ( wxObjectEventFunction )&gTimeline::OnPopUpFitTimeScale, ITEMNORMAL );
  buildItem( popUpMenu, wxString( "Fit Semantic Scale" ), ( wxObjectEventFunction )&gTimeline::OnPopUpFitSemanticScale, ITEMNORMAL );

  popUpMenu->AppendSeparator();

  buildItem( popUpMenuColor, wxString( "Code Color" ), ( wxObjectEventFunction )&gTimeline::OnPopUpCodeColor, ITEMRADIO, timeline->GetMyWindow()->IsCodeColorSet() );
  buildItem( popUpMenuColor, wxString( "Gradient Color" ), ( wxObjectEventFunction )&gTimeline::OnPopUpGradientColor, ITEMRADIO, timeline->GetMyWindow()->IsGradientColorSet() );
  popUpMenu->AppendSubMenu( popUpMenuColor, wxString( "Color" ));

  choices.Add(wxT("Time"));
  choices.Add(wxT("Objects"));
  choices.Add(wxT("Size"));
  choices.Add(wxT("Filter:Communications"));
  choices.Add(wxT("Filter:Events"));
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

  buildItem( popUpMenu, wxString("Copy"), (wxObjectEventFunction)&gHistogram::OnPopUpCopy, ITEMNORMAL );

  buildItem( popUpMenuPaste, wxString("Time"), (wxObjectEventFunction)&gHistogram::OnPopUpPasteTime, ITEMNORMAL );
  buildItem( popUpMenuPaste, wxString("Objects"), (wxObjectEventFunction)&gHistogram::OnPopUpPasteObjects, ITEMNORMAL );
  buildItem( popUpMenuPaste, wxString("Size"), (wxObjectEventFunction)&gHistogram::OnPopUpPasteSize, ITEMNORMAL );

  popUpMenu->AppendSubMenu( popUpMenuPaste, wxString( "Paste" ));

  buildItem( popUpMenu, wxString("Paste Special..."), (wxObjectEventFunction)&gHistogram::OnPopUpPasteSpecial, ITEMNORMAL );
  buildItem( popUpMenu, wxString("Clone"), (wxObjectEventFunction)&gHistogram::OnPopUpClone, ITEMNORMAL );

  popUpMenu->AppendSeparator();

  buildItem( popUpMenu, wxString( "Undo Zoom" ), ( wxObjectEventFunction )&gHistogram::OnPopUpUndoZoom, ITEMNORMAL );
  buildItem( popUpMenu, wxString( "Redo Zoom" ), ( wxObjectEventFunction )&gHistogram::OnPopUpRedoZoom, ITEMNORMAL );

  popUpMenu->AppendSeparator();

  buildItem( popUpMenu, wxString( "Fit Time Scale" ), ( wxObjectEventFunction )&gHistogram::OnPopUpFitTimeScale, ITEMNORMAL );
  buildItem( popUpMenu, wxString( "Fit Semantic Scale" ), ( wxObjectEventFunction )&gHistogram::OnPopUpFitSemanticScale, ITEMNORMAL );

  choices.Add(wxT("Time"));
  choices.Add(wxT("Objects"));
  choices.Add(wxT("Size"));
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
      popUpMenu->Enable( popUpMenu->FindItem( "Paste" ),
                         gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( histogram, "Paste" ));
      popUpMenu->Enable( popUpMenu->FindItem( "Paste Special..." ),
                         gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( histogram, "Paste Special..." ));
    }
    popUpMenu->Enable( popUpMenu->FindItem( tag ), gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( histogram, tag ));
  }
  else
  {
    if ( checkPaste )
    {
      popUpMenu->Enable( popUpMenu->FindItem( "Paste" ),
                         gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( timeline, "Paste" ));
      popUpMenu->Enable( popUpMenu->FindItem( "Paste Special..." ),
                         gPasteWindowProperties::pasteWindowProperties->getInstance()->isAllowed( timeline, "Paste Special..." ));

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
  if ( choices[ position ].compare( "Filter:Communications" ))
    return string( "Communications" );
  else if ( choices[ position ].compare( "Filter:Events" ))
    return string( "Events" );
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
