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

  TraceGeneralInfo->WriteText( "Name: " );
  TraceGeneralInfo->BeginBold(); 
  TraceGeneralInfo->WriteText( myTrace->getTraceName() + "\n" );
  TraceGeneralInfo->EndBold();

  ptime headerTime = myTrace->getTraceTime();
  ptime clickTime = headerTime ;// + boost::posix_time::nanoseconds( (long) myTrace->traceUnitsToCustomUnits( whichTime, myTrace->getTimeUnit() ) );
  wxString formattedTime = wxString::FromUTF8( LabelConstructor::timeLabel( clickTime, 0 ).c_str() );

  TraceGeneralInfo->WriteText( "Date of creation: " );
  TraceGeneralInfo->BeginBold(); 
  TraceGeneralInfo->WriteText( formattedTime );
  TraceGeneralInfo->EndBold();



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
  

  if ( myTrace->existResourceInfo() )
  { 
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


/*!
 * TraceInformationDialog creator
 */

bool TraceInformationDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
  TraceGeneralInfo = NULL;
  ProcessModelInfo = NULL;
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

  wxStaticBox* itemStaticBoxSizer2Static = new wxStaticBox(itemDialog1, wxID_ANY, _("General Information"));
  wxStaticBoxSizer* itemStaticBoxSizer2 = new wxStaticBoxSizer(itemStaticBoxSizer2Static, wxVERTICAL);
  itemBoxSizer1->Add(itemStaticBoxSizer2, 0, wxGROW|wxALL, 5);

  TraceGeneralInfo = new wxRichTextCtrl( itemDialog1, ID_GENERAL_RICHTEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(300, 100), wxTE_READONLY|wxWANTS_CHARS );
  itemStaticBoxSizer2->Add(TraceGeneralInfo, 0, wxGROW|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer4Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Process Model Information"));
  wxStaticBoxSizer* itemStaticBoxSizer4 = new wxStaticBoxSizer(itemStaticBoxSizer4Static, wxVERTICAL);
  itemBoxSizer1->Add(itemStaticBoxSizer4, 0, wxGROW|wxALL, 5);

  ProcessModelInfo = new wxRichTextCtrl( itemDialog1, ID_PROCESS_RICHTEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(300, 100), wxTE_READONLY|wxWANTS_CHARS );
  itemStaticBoxSizer4->Add(ProcessModelInfo, 0, wxGROW|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer6Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Resource Model Information"));
  wxStaticBoxSizer* itemStaticBoxSizer6 = new wxStaticBoxSizer(itemStaticBoxSizer6Static, wxVERTICAL);
  itemBoxSizer1->Add(itemStaticBoxSizer6, 0, wxGROW|wxALL, 5);

  ResourceModelInfo = new wxRichTextCtrl( itemDialog1, ID_RESOURCE_RICHTEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(300, 50), wxTE_READONLY|wxWANTS_CHARS );
  itemStaticBoxSizer6->Add(ResourceModelInfo, 0, wxGROW|wxALL, 5);

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
