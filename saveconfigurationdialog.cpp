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

#include "saveconfigurationdialog.h"

////@begin XPM images
////@end XPM images


/*!
 * SaveConfigurationDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( SaveConfigurationDialog, wxDialog )


/*!
 * SaveConfigurationDialog event table definition
 */

BEGIN_EVENT_TABLE( SaveConfigurationDialog, wxDialog )

////@begin SaveConfigurationDialog event table entries
  EVT_BUTTON( wxID_SAVE, SaveConfigurationDialog::OnSaveClick )

////@end SaveConfigurationDialog event table entries

END_EVENT_TABLE()


/*!
 * SaveConfigurationDialog constructors
 */

SaveConfigurationDialog::SaveConfigurationDialog()
{
  Init();
}

SaveConfigurationDialog::SaveConfigurationDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}


/*!
 * SaveConfigurationDialog creator
 */

bool SaveConfigurationDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin SaveConfigurationDialog creation
  SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Centre();
////@end SaveConfigurationDialog creation
  return true;
}


/*!
 * SaveConfigurationDialog destructor
 */

SaveConfigurationDialog::~SaveConfigurationDialog()
{
////@begin SaveConfigurationDialog destruction
////@end SaveConfigurationDialog destruction
}


/*!
 * Member initialisation
 */

void SaveConfigurationDialog::Init()
{
////@begin SaveConfigurationDialog member initialisation
  listTimelines = NULL;
  listHistograms = NULL;
  optRelativeBegin = NULL;
  optRelativeEnd = NULL;
  optComputeSemantic = NULL;
  radioAllTrace = NULL;
  radioAllWindow = NULL;
  optComputeLimits = NULL;
  optComputeGradient = NULL;
  textDescription = NULL;
////@end SaveConfigurationDialog member initialisation
}


/*!
 * Control creation for SaveConfigurationDialog
 */

void SaveConfigurationDialog::CreateControls()
{    
////@begin SaveConfigurationDialog content construction
  SaveConfigurationDialog* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer3, 2, wxGROW|wxALL, 0);

  wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer3->Add(itemBoxSizer4, 1, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Timelines"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer4->Add(itemStaticText5, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

  wxArrayString listTimelinesStrings;
  listTimelines = new wxCheckListBox( itemDialog1, ID_LISTTIMELINES, wxDefaultPosition, wxSize(200, -1), listTimelinesStrings, wxLB_SINGLE );
  itemBoxSizer4->Add(listTimelines, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer3->Add(itemBoxSizer7, 1, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("Histograms"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer7->Add(itemStaticText8, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

  wxArrayString listHistogramsStrings;
  listHistograms = new wxCheckListBox( itemDialog1, ID_LISTHISTOGRAMS, wxDefaultPosition, wxSize(200, -1), listHistogramsStrings, wxLB_SINGLE );
  itemBoxSizer7->Add(listHistograms, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer3->Add(itemBoxSizer10, 0, wxGROW|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer11Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Timeline options"));
  wxStaticBoxSizer* itemStaticBoxSizer11 = new wxStaticBoxSizer(itemStaticBoxSizer11Static, wxVERTICAL);
  itemBoxSizer10->Add(itemStaticBoxSizer11, 3, wxGROW|wxALL, 5);

  optRelativeBegin = new wxCheckBox( itemDialog1, ID_CHECKBEGIN, _("Relative begin time"), wxDefaultPosition, wxDefaultSize, 0 );
  optRelativeBegin->SetValue(false);
  itemStaticBoxSizer11->Add(optRelativeBegin, 1, wxALIGN_LEFT|wxALL, 5);

  optRelativeEnd = new wxCheckBox( itemDialog1, ID_CHECKEND, _("Relative end time"), wxDefaultPosition, wxDefaultSize, 0 );
  optRelativeEnd->SetValue(false);
  itemStaticBoxSizer11->Add(optRelativeEnd, 1, wxALIGN_LEFT|wxALL, 5);

  optComputeSemantic = new wxCheckBox( itemDialog1, ID_CHECKSEMANTIC, _("Compute semantic scale"), wxDefaultPosition, wxDefaultSize, 0 );
  optComputeSemantic->SetValue(false);
  itemStaticBoxSizer11->Add(optComputeSemantic, 1, wxALIGN_LEFT|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer15Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Histogram options"));
  wxStaticBoxSizer* itemStaticBoxSizer15 = new wxStaticBoxSizer(itemStaticBoxSizer15Static, wxVERTICAL);
  itemBoxSizer10->Add(itemStaticBoxSizer15, 5, wxGROW|wxALL, 5);

  radioAllTrace = new wxRadioButton( itemDialog1, ID_RADIOALLTRACE, _("All trace"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
  radioAllTrace->SetValue(false);
  itemStaticBoxSizer15->Add(radioAllTrace, 1, wxALIGN_LEFT|wxALL, 5);

  radioAllWindow = new wxRadioButton( itemDialog1, ID_RADIOALLWINDOW, _("All window"), wxDefaultPosition, wxDefaultSize, 0 );
  radioAllWindow->SetValue(false);
  itemStaticBoxSizer15->Add(radioAllWindow, 1, wxGROW|wxALL, 5);

  wxStaticLine* itemStaticLine18 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemStaticBoxSizer15->Add(itemStaticLine18, 1, wxGROW|wxLEFT|wxRIGHT, 5);

  optComputeLimits = new wxCheckBox( itemDialog1, ID_CHECKLIMITS, _("Compute min, max, delta limits"), wxDefaultPosition, wxDefaultSize, 0 );
  optComputeLimits->SetValue(false);
  itemStaticBoxSizer15->Add(optComputeLimits, 1, wxALIGN_LEFT|wxALL, 5);

  optComputeGradient = new wxCheckBox( itemDialog1, ID_CHECKGRADIENT, _("Compute gradient limits"), wxDefaultPosition, wxDefaultSize, 0 );
  optComputeGradient->SetValue(false);
  itemStaticBoxSizer15->Add(optComputeGradient, 1, wxALIGN_LEFT|wxALL, 5);

  wxStaticText* itemStaticText21 = new wxStaticText( itemDialog1, wxID_STATIC, _("Description"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer2->Add(itemStaticText21, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

  textDescription = new wxTextCtrl( itemDialog1, ID_TEXTDESCRIPTION, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
  itemBoxSizer2->Add(textDescription, 1, wxGROW|wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer23 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer23, 0, wxALIGN_RIGHT|wxALL, 5);
  wxButton* itemButton24 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer23->AddButton(itemButton24);

  wxButton* itemButton25 = new wxButton( itemDialog1, wxID_SAVE, _("&Save"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer23->AddButton(itemButton25);

  itemStdDialogButtonSizer23->Realize();

////@end SaveConfigurationDialog content construction
}


/*!
 * Should we show tooltips?
 */

bool SaveConfigurationDialog::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap SaveConfigurationDialog::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin SaveConfigurationDialog bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
////@end SaveConfigurationDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon SaveConfigurationDialog::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin SaveConfigurationDialog icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end SaveConfigurationDialog icon retrieval
}


bool SaveConfigurationDialog::TransferDataToWindow()
{
  wxArrayString items;
  for( vector<Window *>::iterator it = timelines.begin(); it != timelines.end(); ++it )
    items.Add( wxString::FromAscii( (*it)->getName().c_str() ) + _( " @ " ) + wxString::FromAscii( (*it)->getTrace()->getTraceName().c_str() ) );
  listTimelines->InsertItems( items, 0 );

  items.Clear();
  for( vector<Histogram *>::iterator it = histograms.begin(); it != histograms.end(); ++it )
    items.Add( wxString::FromAscii( (*it)->getName().c_str() ) + _( " @ " ) + wxString::FromAscii( (*it)->getTrace()->getTraceName().c_str() ) );
  listHistograms->InsertItems( items, 0 );

  optRelativeBegin->SetValue( options.windowBeginTimeRelative );
  optRelativeEnd->SetValue( options.windowScaleRelative );
  optComputeSemantic->SetValue( options.windowComputeYMaxOnLoad );
  radioAllTrace->SetValue( options.histoAllTrace );
  optComputeLimits->SetValue( options.histoComputeYScale );
  optComputeGradient->SetValue( options.histoComputeGradient );

  return true;
}


bool SaveConfigurationDialog::TransferDataFromWindow()
{
  vector<Window *> tmpTimelines;
  for( size_t i = 0; i < listTimelines->GetCount(); ++i )
  {
    if( listTimelines->IsChecked( i ) )
      tmpTimelines.push_back( timelines[ i ] );
  }
  timelines.clear();
  timelines = tmpTimelines;
  
  vector<Histogram *> tmpHistograms;
  for( size_t i = 0; i < listHistograms->GetCount(); ++i )
  {
    if( listHistograms->IsChecked( i ) )
      tmpHistograms.push_back( histograms[ i ] );
  }
  histograms.clear();
  histograms = tmpHistograms;
  
  options.windowBeginTimeRelative = optRelativeBegin->GetValue();
  options.windowScaleRelative = optRelativeEnd->GetValue();
  options.windowComputeYMaxOnLoad = optComputeSemantic->GetValue();
  options.histoAllTrace = radioAllTrace->GetValue();
  options.histoComputeYScale = optComputeLimits->GetValue();
  options.histoComputeGradient = optComputeGradient->GetValue();
  options.description = std::string( textDescription->GetValue().mb_str() );

  return true;
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_SAVE
 */

void SaveConfigurationDialog::OnSaveClick( wxCommandEvent& event )
{
  TransferDataFromWindow();
  if( timelines.begin() == timelines.end() &&
      histograms.begin() == histograms.end() )
  {
    wxMessageDialog message( this, _( "No timeline or histogram selected." ), _( "Warning" ), wxOK );
    message.ShowModal();
    //EndModal( wxID_OK );//EndModal( wxID_CANCEL );
  }
  else
    EndModal( wxID_OK );
}

