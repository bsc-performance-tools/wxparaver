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

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include <wx/regex.h>

////@begin includes
#include "wx/imaglist.h"
////@end includes

#include "eventlabels.h"
#include "labelconstructor.h"
#include "traceinformationdialog.h"

////@begin XPM images
////@end XPM images


/*!
 * TraceInformationDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( TraceInformationDialog, wxDialog )


/*!
 * TraceInformationDialog event table definition
 */

BEGIN_EVENT_TABLE( TraceInformationDialog, wxDialog )

////@begin TraceInformationDialog event table entries
  EVT_LISTBOX( ID_LISTBOX_TYPES, TraceInformationDialog::OnListboxTypesSelected )
////@end TraceInformationDialog event table entries

END_EVENT_TABLE()


/*!
 * TraceInformationDialog constructors
 */

TraceInformationDialog::TraceInformationDialog()
{
  Init();
}

TraceInformationDialog::TraceInformationDialog( wxWindow* parent, Trace* whichTrace, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  myTrace = whichTrace;
  wxString myCaption = "Trace Information: " + myTrace->getTraceName();
  Create(parent, id, myCaption, pos, size, style);


  // Writing text at constructor
  DisplayTraceInformation();
}

/*!
 * TraceInformationDialog creator
 */

bool TraceInformationDialog::Create(wxWindow *parent, wxWindowID id, const wxString &caption, const wxPoint &pos, const wxSize &size, long style)
{
////@begin TraceInformationDialog creation
  SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Centre();
////@end TraceInformationDialog creation
  return true;
}


/*!
 * TraceInformationDialog destructor
 */

TraceInformationDialog::~TraceInformationDialog()
{
////@begin TraceInformationDialog destruction
////@end TraceInformationDialog destruction
}


/*!
 * Member initialisation
 */

void TraceInformationDialog::Init()
{
////@begin TraceInformationDialog member initialisation
  sizerMain = NULL;
  GeneralInfoSizer = NULL;
  TraceGeneralInfo = NULL;
  MetadataInfoSizer = NULL;
  MetadataGeneralInfo = NULL;
  ProcessModelSizer = NULL;
  ProcessModelInfo = NULL;
  ResourceModelSizer = NULL;
  ResourceModelInfo = NULL;
  listTypes = NULL;
  listValues = NULL;
////@end TraceInformationDialog member initialisation
  myTrace = nullptr;
}


/*!
 * Control creation for TraceInformationDialog
 */

void TraceInformationDialog::CreateControls()
{    
////@begin TraceInformationDialog content construction
  TraceInformationDialog* itemDialog1 = this;

  sizerMain = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(sizerMain);

  wxNotebook* itemNotebook2 = new wxNotebook( itemDialog1, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

  wxPanel* itemPanel3 = new wxPanel( itemNotebook2, ID_PANEL_GENERAL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  itemPanel3->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemPanel3->SetSizer(itemBoxSizer2);

  GeneralInfoSizer = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer2->Add(GeneralInfoSizer, 3, wxGROW|wxALL, 5);
  TraceGeneralInfo = new wxRichTextCtrl( itemPanel3, ID_GENERAL_RICHTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxWANTS_CHARS );
  GeneralInfoSizer->Add(TraceGeneralInfo, 1, wxGROW|wxALL, 5);

  MetadataInfoSizer = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer2->Add(MetadataInfoSizer, 3, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText7 = new wxStaticText( itemPanel3, ID_MTI_STATIC, _("Metadata Information"), wxDefaultPosition, wxDefaultSize, 0 );
  MetadataInfoSizer->Add(itemStaticText7, 0, wxALIGN_LEFT|wxALL, 5);

  MetadataGeneralInfo = new wxRichTextCtrl( itemPanel3, ID_METADATA_RICHTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxWANTS_CHARS );
  MetadataInfoSizer->Add(MetadataGeneralInfo, 1, wxGROW|wxALL, 5);

  ProcessModelSizer = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer2->Add(ProcessModelSizer, 2, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText10 = new wxStaticText( itemPanel3, wxID_PMI_STATIC, _("Process Model Information"), wxDefaultPosition, wxDefaultSize, 0 );
  ProcessModelSizer->Add(itemStaticText10, 0, wxALIGN_LEFT|wxALL, 5);

  ProcessModelInfo = new wxRichTextCtrl( itemPanel3, ID_PROCESS_RICHTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxWANTS_CHARS );
  ProcessModelSizer->Add(ProcessModelInfo, 1, wxGROW|wxALL, 5);

  ResourceModelSizer = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer2->Add(ResourceModelSizer, 2, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText13 = new wxStaticText( itemPanel3, wxID_RMI_STATIC, _("Resource Model Information"), wxDefaultPosition, wxDefaultSize, 0 );
  ResourceModelSizer->Add(itemStaticText13, 0, wxALIGN_LEFT|wxALL, 5);

  ResourceModelInfo = new wxRichTextCtrl( itemPanel3, ID_RESOURCE_RICHTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxWANTS_CHARS );
  ResourceModelSizer->Add(ResourceModelInfo, 1, wxGROW|wxALL, 5);

  itemNotebook2->AddPage(itemPanel3, _("General"));

  wxPanel* itemPanel17 = new wxPanel( itemNotebook2, ID_PANEL_EVENTS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  itemPanel17->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxVERTICAL);
  itemPanel17->SetSizer(itemBoxSizer15);

  wxStaticText* itemStaticText1 = new wxStaticText( itemPanel17, wxID_STATIC, _("Types"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer15->Add(itemStaticText1, 0, wxALIGN_LEFT|wxALL, 5);

  wxArrayString listTypesStrings;
  listTypes = new wxListBox( itemPanel17, ID_LISTBOX_TYPES, wxDefaultPosition, wxDefaultSize, listTypesStrings, wxLB_SINGLE );
  itemBoxSizer15->Add(listTypes, 1, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText3 = new wxStaticText( itemPanel17, wxID_STATIC, _("Values"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer15->Add(itemStaticText3, 0, wxALIGN_LEFT|wxALL, 5);

  wxArrayString listValuesStrings;
  listValues = new wxListBox( itemPanel17, ID_LISTBOX_VALUES, wxDefaultPosition, wxDefaultSize, listValuesStrings, wxLB_SINGLE );
  itemBoxSizer15->Add(listValues, 1, wxGROW|wxALL, 5);

  itemNotebook2->AddPage(itemPanel17, _("Events"));

  sizerMain->Add(itemNotebook2, 1, wxGROW|wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer1 = new wxStdDialogButtonSizer;

  sizerMain->Add(itemStdDialogButtonSizer1, 0, wxGROW|wxLEFT|wxBOTTOM, 5);
  wxButton* itemButton2 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
  itemButton2->SetDefault();
  itemStdDialogButtonSizer1->AddButton(itemButton2);

  itemStdDialogButtonSizer1->Realize();

////@end TraceInformationDialog content construction

  wxFont tmpFont = listTypes->GetFont();
  tmpFont.SetFamily( wxFONTFAMILY_TELETYPE );
  listTypes->SetFont( tmpFont );
  listValues->SetFont( tmpFont );

  wxString tmpStr;
  myTrace->getEventLabels().getTypes(
    [&]( TEventType type, const std::string& label )
    {
      tmpStr.Clear();
      tmpStr << type << "   " << label;
      listTypesStrings.Add( tmpStr );

      eventTypes.push_back( type );
    }
  );  
  if ( !listTypesStrings.IsEmpty() )
    listTypes->InsertItems( listTypesStrings, 0 );
}


/*!
 * Should we show tooltips?
 */

bool TraceInformationDialog::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap TraceInformationDialog::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin TraceInformationDialog bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
////@end TraceInformationDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon TraceInformationDialog::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin TraceInformationDialog icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end TraceInformationDialog icon retrieval
}


wxString TraceInformationDialog::FormatTraceSize( double traceByteSize )
{
  if ( traceByteSize > 1E12 )
    return wxString::Format( wxT( "%.2f TB" ), double( traceByteSize ) / 1E12 );
  else if ( traceByteSize > 1E9 )
    return wxString::Format( wxT( "%.2f GB" ), double( traceByteSize ) / 1E9 );
  else if ( traceByteSize > 1E6 )
    return wxString::Format( wxT( "%.2f MB" ), double( traceByteSize ) / 1E6 );
  else if ( traceByteSize > 1E3 )
    return wxString::Format( wxT( "%.2f kB" ), double( traceByteSize ) / 1E3 );
  
  return wxString::Format( wxT( "%.2f Bytes" ), double( traceByteSize ) );
}

void TraceInformationDialog::DisplayTraceInformation()
{
  // General Info
  ptime headerTime = myTrace->getTraceTime();
  ptime clickTime = headerTime ;// + boost::posix_time::nanoseconds( (long) myTrace->traceUnitsToCustomUnits( whichTime, myTrace->getTimeUnit() ) );

  wxString formattedCreationTime = wxString::FromUTF8( LabelConstructor::timeLabel( clickTime, 0 ).c_str() ).BeforeFirst( ',' );
  wxString formattedDurationTime = wxString::FromUTF8( LabelConstructor::timeLabel( myTrace->getEndTime(), myTrace->getTimeUnit(), 0 ).c_str() );

  wxString traceSize = FormatTraceSize( myTrace->getTraceSize() );
/*
  TraceGeneralInfo->WriteText( "Name: " );
  TraceGeneralInfo->BeginBold(); 
  TraceGeneralInfo->WriteText( myTrace->getTraceName() + "\n" );
  TraceGeneralInfo->EndBold();
*/
  TraceGeneralInfo->WriteText( "Path: " );
  TraceGeneralInfo->BeginBold(); 
  TraceGeneralInfo->WriteText( wxString( myTrace->getFileName() ).BeforeLast( '/' ) + "\n" );
  TraceGeneralInfo->EndBold();
  
  TraceGeneralInfo->WriteText( "Size: " );
  TraceGeneralInfo->BeginBold(); 
  TraceGeneralInfo->WriteText( traceSize + "\n" );
  TraceGeneralInfo->EndBold();
  
  TraceGeneralInfo->WriteText( "Date of creation: " );
  TraceGeneralInfo->BeginBold(); 
  TraceGeneralInfo->WriteText( formattedCreationTime + "\n" );
  TraceGeneralInfo->EndBold();

  TraceGeneralInfo->WriteText( "Duration: " );
  TraceGeneralInfo->BeginBold(); 
  TraceGeneralInfo->WriteText( formattedDurationTime /*+ "\n"*/ );
  TraceGeneralInfo->EndBold();
  
/*  
  //TODO
    // ADD: Trace filesize, num records state/evt/comms (not available today)
    // RMV: state/evts?
  std::vector< TState > stateVec;
  std::vector< TEventType > eventVec;
  myTrace->getStateLabels().getStates( stateVec );
  myTrace->getEventLabels().getTypes( eventVec );

  TraceGeneralInfo->WriteText( "States: " );
  TraceGeneralInfo->BeginBold(); 
  TraceGeneralInfo->WriteText( wxString::Format( wxT( "%i\n" ), stateVec.size() ) );
  TraceGeneralInfo->EndBold();

  TraceGeneralInfo->WriteText( "Events: " );
  TraceGeneralInfo->BeginBold(); 
  TraceGeneralInfo->WriteText( wxString::Format( wxT( "%i" ), eventVec.size() ) );
  TraceGeneralInfo->EndBold();
*/

  MetadataInfoSizer->Show( false );

/*
  // Metadata
  MetadataInfo->BeginBold(); 
  MetadataInfo->WriteText( "CUTTER\n" );
  MetadataInfo->EndBold(); 

  //MetadataInfo->BeginListStyle( ... ); 
    MetadataInfo->WriteText( "Begin time: " );
    MetadataInfo->BeginBold(); 
    MetadataInfo->WriteText( wxString::Format( wxT( "%i\n" ), myTrace->getCutterBeginTime() ) );
    MetadataInfo->EndBold(); 

    MetadataInfo->WriteText( "End time: " );
    MetadataInfo->BeginBold(); 
    MetadataInfo->WriteText( wxString::Format( wxT( "%i\n" ), myTrace->getCutterEndTime() ) );
    MetadataInfo->EndBold(); 

    MetadataInfo->WriteText( "Offset: " );
    MetadataInfo->BeginBold(); 
    MetadataInfo->WriteText( wxString::Format( wxT( "%i\n" ), myTrace->getCutterOffset() ) );
    MetadataInfo->EndBold(); 

    MetadataInfo->WriteText( "Last begin time: " );
    MetadataInfo->BeginBold(); 
    MetadataInfo->WriteText( wxString::Format( wxT( "%i\n" ), myTrace->getCutterLastBeginTime() ) );
    MetadataInfo->EndBold(); 

    MetadataInfo->WriteText( "Last end time: " );
    MetadataInfo->BeginBold(); 
    MetadataInfo->WriteText( wxString::Format( wxT( "%i\n" ), myTrace->getCutterLastEndTime() ) );
    MetadataInfo->EndBold(); 

    MetadataInfo->WriteText( "Last offset: " );
    MetadataInfo->BeginBold(); 
    MetadataInfo->WriteText( wxString::Format( wxT( "%i\n" ), myTrace->getCutterLastOffset() ) );
    MetadataInfo->EndBold(); 
  //MetadataInfo->EndListStyle(); 
*/
  // Process Model

  ProcessModelInfo->WriteText( "Applications: " );
  ProcessModelInfo->BeginBold(); 
  ProcessModelInfo->WriteText( wxString::Format( wxT( "%i\n" ), myTrace->totalApplications() ) ); 
  ProcessModelInfo->EndBold();
  
  ProcessModelInfo->WriteText( "Tasks: " );
  ProcessModelInfo->BeginBold(); 
  ProcessModelInfo->WriteText( wxString::Format( wxT( "%i\n" ), myTrace->totalTasks() ) ); 
  ProcessModelInfo->EndBold();
  
  ProcessModelInfo->WriteText( "Threads: " );
  ProcessModelInfo->BeginBold(); 
  ProcessModelInfo->WriteText( wxString::Format( wxT( "%i" ), myTrace->totalThreads() ) ); 
  ProcessModelInfo->EndBold();
  

  // Resource Model
  if ( myTrace->existResourceInfo() )
  {
    int numRacks = getRackInformation();
    if ( numRacks > 0 )
    {
      ResourceModelInfo->WriteText( "Racks: " );
      ResourceModelInfo->BeginBold();
      ResourceModelInfo->WriteText( wxString::Format( wxT( "%i\n" ), getRackInformation() ) );
      ResourceModelInfo->EndBold();
    }

    ResourceModelInfo->WriteText( "Nodes: " );
    ResourceModelInfo->BeginBold(); 
    ResourceModelInfo->WriteText( wxString::Format( wxT( "%i\n" ), myTrace->totalNodes() ) ); 
    ResourceModelInfo->EndBold(); 

    ResourceModelInfo->WriteText( "CPUs: " );
    ResourceModelInfo->BeginBold(); 
    ResourceModelInfo->WriteText( wxString::Format( wxT( "%i" ), myTrace->totalCPUs() ) ); 
    ResourceModelInfo->EndBold(); 
  }
  else
  {
    ResourceModelSizer->Show( false );
  }
}

int TraceInformationDialog::getRackInformation()
{
/*
  int numRack;
  std::vector< std::string > NodeSet;
  for ( TThreadOrder i = 0; i < myTrace->totalCPUs(); i += (myTrace->totalCPUs() / myTrace->totalNodes() ) )
  {
    TNodeOrder NID = myTrace->getNodeFromThread( i );
    NodeSet.push_back( myTrace->getRowLabel( NODE, NID ) );
  }

  // regex time: from the set, sort them by racks
  wxRegEx reMN4( "^s[0-9]+r[0-9]+b[0-9]+" );
  int idx = 0;

  std::set<long> rackSet;
  for  (std::vector< std::string >::iterator it = NodeSet.begin(); it != NodeSet.end(); ++it )
  {
    wxString nodeName( ( *it ).c_str(), wxConvUTF8 );

    long rackID;
    if ( reMN4.Matches( nodeName ) )
    {
      //computerID = "MN4";
      //nodeName.AfterFirst( 's' ).BeforeFirst( 'r' ).ToLong( &switchID );
      nodeName.AfterFirst( 'r' ).BeforeFirst( 'b' ).ToLong( &rackID );
      //nodeName.AfterFirst( 'b' ).ToLong( &nodeID );
      rackSet.insert( rackID );
      //rackID = rackID + ( 1000*switchID );
    }
    //else if ( reMT.Matches( nodeName ) ) {}

  }
*/
  return 0;
}


/*!
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX_TYPES
 */

void TraceInformationDialog::OnListboxTypesSelected( wxCommandEvent& event )
{
  wxString tmpStr;
  wxArrayString listValuesStrings;

  listValues->Clear();

  myTrace->getEventLabels().getValues( eventTypes[ event.GetSelection() ],
    [&]( TEventValue value, const std::string& label )
    {
      tmpStr.Clear();
      tmpStr << value << "   " << label;
      listValuesStrings.Add( tmpStr );
    }
  );
  listValues->Append( listValuesStrings );
}


