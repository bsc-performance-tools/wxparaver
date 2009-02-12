#include "popupmenu.h"
#include "copypaste.h"
#include <wx/event.h>

//#include <iostream>
//using namespace std;


void gPopUpMenu::BuildItem( wxMenu *popUp,
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


gPopUpMenu::gPopUpMenu( gTimeline *whichTimeline )
{
  timeline = whichTimeline;

  popUpMenu = new wxMenu;
  popUpMenuColor = new wxMenu;
  popUpMenuPaste = new wxMenu;
  popUpMenuPasteFilter = new wxMenu;

  BuildItem( popUpMenu, wxString("Copy"), (wxObjectEventFunction)&gTimeline::OnPopUpCopy, ITEMNORMAL );

  BuildItem( popUpMenuPaste, wxString("Time"), (wxObjectEventFunction)&gTimeline::OnPopUpPasteTime, ITEMNORMAL );
  BuildItem( popUpMenuPaste, wxString("Objects"), (wxObjectEventFunction)&gTimeline::OnPopUpPasteObjects, ITEMNORMAL );
  BuildItem( popUpMenuPaste, wxString("Size"), (wxObjectEventFunction)&gTimeline::OnPopUpPasteSize, ITEMNORMAL );

  BuildItem( popUpMenuPasteFilter, wxString("All"), (wxObjectEventFunction)&gTimeline::OnPopUpPasteFilterAll, ITEMNORMAL );
  BuildItem( popUpMenuPasteFilter, wxString("Communications"), (wxObjectEventFunction)&gTimeline::OnPopUpPasteFilterCommunications, ITEMNORMAL );
  BuildItem( popUpMenuPasteFilter, wxString("Events"), (wxObjectEventFunction)&gTimeline::OnPopUpPasteFilterEvents, ITEMNORMAL );
  popUpMenuPaste->AppendSubMenu( popUpMenuPasteFilter, wxString( "Filter" ));
  popUpMenu->AppendSubMenu( popUpMenuPaste, wxString( "Paste" ));

  BuildItem( popUpMenu, wxString("Paste Special..."), (wxObjectEventFunction)&gTimeline::OnPopUpPasteSpecial, ITEMNORMAL );
  BuildItem( popUpMenu, wxString("Clone"), (wxObjectEventFunction)&gTimeline::OnPopUpClone, ITEMNORMAL );

  popUpMenu->AppendSeparator();

  BuildItem( popUpMenu, wxString( "Undo Zoom" ), ( wxObjectEventFunction )&gTimeline::OnPopUpUndoZoom, ITEMNORMAL );
  BuildItem( popUpMenu, wxString( "Redo Zoom" ), ( wxObjectEventFunction )&gTimeline::OnPopUpRedoZoom, ITEMNORMAL );

  popUpMenu->AppendSeparator();

  BuildItem( popUpMenu, wxString( "Fit &Time Scale" ), ( wxObjectEventFunction )&gTimeline::OnPopUpFitTimeScale, ITEMNORMAL );
  BuildItem( popUpMenu, wxString( "Fit &Semantic Scale" ), ( wxObjectEventFunction )&gTimeline::OnPopUpFitSemanticScale, ITEMNORMAL );

  popUpMenu->AppendSeparator();

  BuildItem( popUpMenuColor, wxString( "Code Color" ), ( wxObjectEventFunction )&gTimeline::OnPopUpCodeColor, ITEMRADIO, timeline->GetMyWindow()->IsCodeColorSet() );
  BuildItem( popUpMenuColor, wxString( "Gradient Color" ), ( wxObjectEventFunction )&gTimeline::OnPopUpGradientColor, ITEMRADIO, timeline->GetMyWindow()->IsGradientColorSet() );
  popUpMenu->AppendSubMenu( popUpMenuColor, wxString( "Color" ));

//  wxArrayString choices;
  choices.Add(wxT("Time"));
  choices.Add(wxT("Objects"));
  choices.Add(wxT("Size"));
  choices.Add(wxT("Filter:Communications"));
  choices.Add(wxT("Filter:Events"));
  popUpMenuPasteDialog = new wxMultiChoiceDialog( timeline,
                                                  wxT( "Select pasted properties:" ),
                                                  wxT("Paste Special"),
                                                  choices);
  // Remove these when operative
  popUpMenu->Enable( popUpMenu->FindItem( "Copy" ), true );

  gPasteWindowProperties* sharedProperties = gPasteWindowProperties::pasteWindowProperties->getInstance();

  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( "Time" ), sharedProperties->allowPaste( timeline, "Time") );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( "Size" ), sharedProperties->allowPaste( timeline, "Size")  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( "Objects" ), sharedProperties->allowPaste( timeline, "Objects")  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( "Filter" ), sharedProperties->allowPaste( timeline, "Filter") );
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( "All" ), sharedProperties->allowPaste( timeline, "All") );
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( "Communications" ), sharedProperties->allowPaste( timeline, "Communications") );
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( "Events" ), sharedProperties->allowPaste( timeline, "Events") );

  popUpMenu->Enable( popUpMenu->FindItem( "Paste" ), sharedProperties->allowPaste( timeline, "Paste") );
  popUpMenu->Enable( popUpMenu->FindItem( "Paste Special..." ), sharedProperties->allowPaste( timeline, "Paste") );

  popUpMenu->Enable( popUpMenu->FindItem( "Clone" ), false );

  popUpMenu->Enable( popUpMenu->FindItem( "Fit &Semantic Scale" ), true );
}

gPopUpMenu::gPopUpMenu( gHistogram *whichHistogram )
{
  histogram = whichHistogram;

  popUpMenu = new wxMenu;
  popUpMenuColor = new wxMenu;
  popUpMenuPaste = new wxMenu;
  popUpMenuPasteFilter = new wxMenu;

  BuildItem( popUpMenu, wxString("Copy"), (wxObjectEventFunction)&gHistogram::OnPopUpCopy, ITEMNORMAL );

  BuildItem( popUpMenuPaste, wxString("Time"), (wxObjectEventFunction)&gHistogram::OnPopUpPasteTime, ITEMNORMAL );
  BuildItem( popUpMenuPaste, wxString("Objects"), (wxObjectEventFunction)&gHistogram::OnPopUpPasteObjects, ITEMNORMAL );
  BuildItem( popUpMenuPaste, wxString("Size"), (wxObjectEventFunction)&gHistogram::OnPopUpPasteSize, ITEMNORMAL );

  BuildItem( popUpMenuPasteFilter, wxString("All"), (wxObjectEventFunction)&gHistogram::OnPopUpPasteFilterAll, ITEMNORMAL );
  BuildItem( popUpMenuPasteFilter, wxString("Communications"), (wxObjectEventFunction)&gHistogram::OnPopUpPasteFilterCommunications, ITEMNORMAL );
  BuildItem( popUpMenuPasteFilter, wxString("Events"), (wxObjectEventFunction)&gHistogram::OnPopUpPasteFilterEvents, ITEMNORMAL );
  popUpMenuPaste->AppendSubMenu( popUpMenuPasteFilter, wxString( "Filter" ));
  popUpMenu->AppendSubMenu( popUpMenuPaste, wxString( "Paste" ));

  BuildItem( popUpMenu, wxString("Paste Special..."), (wxObjectEventFunction)&gHistogram::OnPopUpPasteSpecial, ITEMNORMAL );
  BuildItem( popUpMenu, wxString("Clone"), (wxObjectEventFunction)&gHistogram::OnPopUpClone, ITEMNORMAL );

  popUpMenu->AppendSeparator();

  BuildItem( popUpMenu, wxString( "Undo Zoom" ), ( wxObjectEventFunction )&gHistogram::OnPopUpUndoZoom, ITEMNORMAL );
  BuildItem( popUpMenu, wxString( "Redo Zoom" ), ( wxObjectEventFunction )&gHistogram::OnPopUpRedoZoom, ITEMNORMAL );

  popUpMenu->AppendSeparator();

  BuildItem( popUpMenu, wxString( "Fit &Time Scale" ), ( wxObjectEventFunction )&gHistogram::OnPopUpFitTimeScale, ITEMNORMAL );
  BuildItem( popUpMenu, wxString( "Fit &Semantic Scale" ), ( wxObjectEventFunction )&gHistogram::OnPopUpFitSemanticScale, ITEMNORMAL );

//  popUpMenu->AppendSeparator();

//  BuildItem( popUpMenuColor, wxString( "Code Color" ), ( wxObjectEventFunction )&gHistogram::OnPopUpCodeColor, ITEMRADIO, true );
//  BuildItem( popUpMenuColor, wxString( "Gradient Color" ), ( wxObjectEventFunction )&gHistogram::OnPopUpGradientColor, ITEMRADIO, false );
//  popUpMenu->AppendSubMenu( popUpMenuColor, wxString( "Color" ));

//  wxArrayString choices;
  choices.Add(wxT("Time"));
  choices.Add(wxT("Objects"));
  choices.Add(wxT("Size"));
  choices.Add(wxT("Filter:Communications"));
  choices.Add(wxT("Filter:Events"));
  popUpMenuPasteDialog = new wxMultiChoiceDialog( histogram,
                                                  wxT( "Select pasted properties:" ),
                                                  wxT("Paste Special"),
                                                  choices);
  // Remove these when operative
  popUpMenu->Enable( popUpMenu->FindItem( "Copy" ), true );

  gPasteWindowProperties* sharedProperties = gPasteWindowProperties::pasteWindowProperties->getInstance();

  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( "Time" ), sharedProperties->allowPaste( histogram, "Time") );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( "Size" ), sharedProperties->allowPaste( histogram, "Size")  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( "Objects" ), sharedProperties->allowPaste( histogram, "Objects")  );
  popUpMenuPaste->Enable( popUpMenuPaste->FindItem( "Filter" ), sharedProperties->allowPaste( histogram, "Filter") );
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( "All" ), sharedProperties->allowPaste( histogram, "All") );
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( "Communications" ), sharedProperties->allowPaste( histogram, "Communications") );
  popUpMenuPasteFilter->Enable( popUpMenuPasteFilter->FindItem( "Events" ), sharedProperties->allowPaste( histogram, "Events") );

  popUpMenu->Enable( popUpMenu->FindItem( "Paste" ), sharedProperties->allowPaste( histogram, "Paste") );
  popUpMenu->Enable( popUpMenu->FindItem( "Paste Special..." ), sharedProperties->allowPaste( histogram, "Paste") );

  popUpMenu->Enable( popUpMenu->FindItem( "Clone" ), false );

  popUpMenu->Enable( popUpMenu->FindItem( "Fit &Semantic Scale" ), true );
}

void gPopUpMenu::Enable( const string tag )
{
  if ( timeline == NULL )
    popUpMenu->Enable( popUpMenu->FindItem( "Paste" ),
                       gPasteWindowProperties::pasteWindowProperties->getInstance()->allowPaste( histogram, "Paste" ));
  else
    popUpMenu->Enable( popUpMenu->FindItem( "Paste" ),
                       gPasteWindowProperties::pasteWindowProperties->getInstance()->allowPaste( timeline, "Paste" ));

//  cout << "tag:" << tag << endl;

  popUpMenu->Enable( popUpMenu->FindItem( tag ), gPasteWindowProperties::pasteWindowProperties->getInstance()->allowPaste( timeline, tag ));
}


void gPopUpMenu::Enable( const string tag, bool enable )
{
  if ( timeline == NULL )
    popUpMenu->Enable( popUpMenu->FindItem( "Paste" ),
                       gPasteWindowProperties::pasteWindowProperties->getInstance()->allowPaste( histogram, "Paste" ));
  else
    popUpMenu->Enable( popUpMenu->FindItem( "Paste" ),
                       gPasteWindowProperties::pasteWindowProperties->getInstance()->allowPaste( timeline, "Paste" ));

//  cout << "tag2:" << tag << endl;

  popUpMenu->Enable( popUpMenu->FindItem( tag ), enable );
}
/*
void gPopUpMenu::Check( const string tag, bool checked )
{
  int tmpId = popUpMenu->FindItem( tag );
  wxMenuItem *tmpMenuItem = popUpMenu->FindItem( tmpId );
  if ( tmpMenuItem->IsCheckable() )
    tmpMenuItem->Check( checked );
}
*/
string gPopUpMenu::GetOption( int position )
{
  return choices[ position ].c_str();
}

bool gPopUpMenu::OkPressed( )
{
  return ( popUpMenuPasteDialog->ShowModal() == wxID_OK );
}


wxArrayInt gPopUpMenu::GetSelections()
{
   return popUpMenuPasteDialog->GetSelections();
}


void gPopUpMenu::Show(  ){}
