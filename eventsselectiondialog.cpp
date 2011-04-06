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

#include "eventsselectiondialog.h"

////@begin XPM images
////@end XPM images


/*!
 * EventsSelectionDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( EventsSelectionDialog, wxDialog )


/*!
 * EventsSelectionDialog event table definition
 */

BEGIN_EVENT_TABLE( EventsSelectionDialog, wxDialog )

////@begin EventsSelectionDialog event table entries
////@end EventsSelectionDialog event table entries

END_EVENT_TABLE()


/*!
 * EventsSelectionDialog constructors
 */

EventsSelectionDialog::EventsSelectionDialog()
{
  Init( false );
}

EventsSelectionDialog::EventsSelectionDialog( wxWindow* parent,
                                              vector<TEventType> types,
                                              vector<TEventValue> values,
                                              bool hideOperatorsList,
                                              wxWindowID id,
                                              const wxString& caption,
                                              const wxPoint& pos,
                                              const wxSize& size,
                                              long style )
{
  // window name, operators (selection and combined), types, values, selected types, selected values
  // Init assigns to attributes
  Init( hideOperatorsList );
  
  // and create reads them to set the window
  Create(parent, id, caption, pos, size, style );
}


/*!
 * EventsSelectionDialog creator
 */

bool EventsSelectionDialog::Create( wxWindow* parent,
                                    wxWindowID id,
                                    const wxString& caption,
                                    const wxPoint& pos,
                                    const wxSize& size,
                                    long style )
{
////@begin EventsSelectionDialog creation
  SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Centre();
////@end EventsSelectionDialog creation
  return true;
}


/*!
 * EventsSelectionDialog destructor
 */

EventsSelectionDialog::~EventsSelectionDialog()
{
////@begin EventsSelectionDialog destruction
////@end EventsSelectionDialog destruction
}


/*!
 * Member initialisation
 */

void EventsSelectionDialog::Init( bool whichHideOperatorsList )
{
////@begin EventsSelectionDialog member initialisation
  textWindowName = NULL;
  boxSizerOperatorsChoice = NULL;
  choiceOperatorSelectTypes = NULL;
  combinedSelection = NULL;
  choiceOperatorSelectValues = NULL;
  checkboxSelectTypes = NULL;
  buttonSetAllTypes = NULL;
  buttonUnsetAllTypes = NULL;
  checkboxSelectValues = NULL;
  buttonSetAllValues = NULL;
  buttonUnsetAllValues = NULL;
////@end EventsSelectionDialog member initialisation
  hideOperatorsList = whichHideOperatorsList;
}


/*!
 * Control creation for EventsSelectionDialog
 */

void EventsSelectionDialog::CreateControls()
{    
////@begin EventsSelectionDialog content construction
  EventsSelectionDialog* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

  textWindowName = new wxStaticText( itemDialog1, wxID_STATIC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer3->Add(textWindowName, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer2->Add(itemBoxSizer5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer5->Add(itemBoxSizer6, 0, wxGROW|wxALL, 5);

  itemBoxSizer6->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("Types"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
  itemBoxSizer6->Add(itemStaticText8, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  itemBoxSizer6->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticLine* itemStaticLine10 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
  itemBoxSizer6->Add(itemStaticLine10, 0, wxGROW|wxALL, 5);

  itemBoxSizer6->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticText* itemStaticText12 = new wxStaticText( itemDialog1, wxID_STATIC, _("Values"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
  itemBoxSizer6->Add(itemStaticText12, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  itemBoxSizer6->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  boxSizerOperatorsChoice = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer5->Add(boxSizerOperatorsChoice, 0, wxGROW|wxALL, 5);

  wxArrayString choiceOperatorSelectTypesStrings;
  choiceOperatorSelectTypes = new wxChoice( itemDialog1, ID_CHOICE_OPERATOR_SELECT_TYPES, wxDefaultPosition, wxDefaultSize, choiceOperatorSelectTypesStrings, 0 );
  boxSizerOperatorsChoice->Add(choiceOperatorSelectTypes, 2, wxGROW|wxALL, 5);

  wxArrayString combinedSelectionStrings;
  combinedSelectionStrings.Add(_("And"));
  combinedSelectionStrings.Add(_("Or"));
  combinedSelection = new wxChoice( itemDialog1, ID_CHOICE_COMBINED_SELECTION, wxDefaultPosition, wxDefaultSize, combinedSelectionStrings, 0 );
  combinedSelection->SetStringSelection(_("And"));
  boxSizerOperatorsChoice->Add(combinedSelection, 1, wxGROW|wxALL, 5);

  wxArrayString choiceOperatorSelectValuesStrings;
  choiceOperatorSelectValues = new wxChoice( itemDialog1, ID_CHOICE_OPERATOR_SELECT_VALUES, wxDefaultPosition, wxDefaultSize, choiceOperatorSelectValuesStrings, 0 );
  boxSizerOperatorsChoice->Add(choiceOperatorSelectValues, 2, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer5->Add(itemBoxSizer18, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer18->Add(itemBoxSizer19, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString checkboxSelectTypesStrings;
  checkboxSelectTypes = new wxCheckListBox( itemDialog1, ID_CHECKLISTBOX_TYPES, wxDefaultPosition, wxDefaultSize, checkboxSelectTypesStrings, wxLB_EXTENDED|wxLB_HSCROLL );
  itemBoxSizer19->Add(checkboxSelectTypes, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer19->Add(itemBoxSizer21, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonSetAllTypes = new wxButton( itemDialog1, ID_BUTTON_SET_ALL_TYPES, _("Set all"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer21->Add(buttonSetAllTypes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  buttonUnsetAllTypes = new wxButton( itemDialog1, ID_BUTTON_UNSET_ALL_TYPES, _("Unset all"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer21->Add(buttonUnsetAllTypes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticLine* itemStaticLine24 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
  itemBoxSizer18->Add(itemStaticLine24, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer18->Add(itemBoxSizer25, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString checkboxSelectValuesStrings;
  checkboxSelectValues = new wxCheckListBox( itemDialog1, ID_CHECKLISTBOX_VALUES, wxDefaultPosition, wxDefaultSize, checkboxSelectValuesStrings, wxLB_EXTENDED|wxLB_HSCROLL );
  itemBoxSizer25->Add(checkboxSelectValues, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer27 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer25->Add(itemBoxSizer27, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonSetAllValues = new wxButton( itemDialog1, ID_BUTTON_SET_ALL_VALUES, _("Set all"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer27->Add(buttonSetAllValues, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  buttonUnsetAllValues = new wxButton( itemDialog1, ID_BUTTON_UNSET_ALL_VALUES, _("Unset all"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer27->Add(buttonUnsetAllValues, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer30 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer30, 0, wxALIGN_RIGHT|wxALL, 5);
  wxButton* itemButton31 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer30->AddButton(itemButton31);

  wxButton* itemButton32 = new wxButton( itemDialog1, wxID_APPLY, _("&Apply"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer30->AddButton(itemButton32);

  itemStdDialogButtonSizer30->Realize();

////@end EventsSelectionDialog content construction
}


/*!
 * Should we show tooltips?
 */

bool EventsSelectionDialog::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap EventsSelectionDialog::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin EventsSelectionDialog bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
////@end EventsSelectionDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon EventsSelectionDialog::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin EventsSelectionDialog icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end EventsSelectionDialog icon retrieval
}
