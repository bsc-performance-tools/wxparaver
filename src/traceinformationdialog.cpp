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
////@end includes

#include "../include/traceinformationdialog.h"

////@begin XPM images
////@end XPM images

#include "labelconstructor.h"

/*!
 * TraceInformationDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( TraceInformationDialog, wxDialog )


/*!
 * TraceInformationDialog event table definition
 */

BEGIN_EVENT_TABLE( TraceInformationDialog, wxDialog )

////@begin TraceInformationDialog event table entries
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
  Create(parent, id, caption, pos, size, style);


  // Writing text at constructor
  DisplayTraceInformation();
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

  TraceGeneralInfo->WriteText( "Name: " );
  TraceGeneralInfo->BeginBold(); 
  TraceGeneralInfo->WriteText( myTrace->getTraceName() + "\n" );
  TraceGeneralInfo->EndBold();

  TraceGeneralInfo->WriteText( "File: " );
  TraceGeneralInfo->BeginBold(); 
  TraceGeneralInfo->WriteText( myTrace->getFileName() + "\n" );
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
    ResourceModelInfo->Show( false );
  }
}

int TraceInformationDialog::getRackInformation()
{

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

  return 0;
}
/*!
 * TraceInformationDialog creator
 */

    bool
    TraceInformationDialog::Create(wxWindow *parent, wxWindowID id, const wxString &caption, const wxPoint &pos, const wxSize &size, long style)
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
  GeneralInfoSizer = NULL;
  TraceGeneralInfo = NULL;
  ProcessModelSizer = NULL;
  ProcessModelInfo = NULL;
  ResourceModelSizer = NULL;
  ResourceModelInfo = NULL;
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

  wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer1);

  GeneralInfoSizer = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer1->Add(GeneralInfoSizer, 3, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText1 = new wxStaticText( itemDialog1, wxID_GTI_STATIC, _("General Information"), wxDefaultPosition, wxDefaultSize, 0 );
  GeneralInfoSizer->Add(itemStaticText1, 0, wxGROW|wxALL, 5);

  TraceGeneralInfo = new wxRichTextCtrl( itemDialog1, ID_GENERAL_RICHTEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(400, 140), wxTE_READONLY|wxWANTS_CHARS );
  GeneralInfoSizer->Add(TraceGeneralInfo, 1, wxGROW|wxALL|wxADJUST_MINSIZE, 5);

  ProcessModelSizer = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer1->Add(ProcessModelSizer, 2, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText2 = new wxStaticText( itemDialog1, wxID_PMI_STATIC, _("Process Model Information"), wxDefaultPosition, wxDefaultSize, 0 );
  ProcessModelSizer->Add(itemStaticText2, 0, wxGROW|wxALL, 5);

  ProcessModelInfo = new wxRichTextCtrl( itemDialog1, ID_PROCESS_RICHTEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(-1, 75), wxTE_READONLY|wxWANTS_CHARS );
  ProcessModelSizer->Add(ProcessModelInfo, 1, wxGROW|wxALL|wxADJUST_MINSIZE, 5);

  ResourceModelSizer = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer1->Add(ResourceModelSizer, 2, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_RMI_STATIC, _("Resource Model Information"), wxDefaultPosition, wxDefaultSize, 0 );
  ResourceModelSizer->Add(itemStaticText3, 0, wxGROW|wxALL, 5);

  ResourceModelInfo = new wxRichTextCtrl( itemDialog1, ID_RESOURCE_RICHTEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(-1, 40), wxTE_READONLY|wxWANTS_CHARS );
  ResourceModelSizer->Add(ResourceModelInfo, 1, wxGROW|wxALL|wxADJUST_MINSIZE, 5);

////@end TraceInformationDialog content construction
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
