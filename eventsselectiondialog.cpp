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

#include <iostream>

////@begin includes
////@end includes

#include "filter.h"

#include "eventsselectiondialog.h"
#include "labelconstructor.h"
//#include "semanticcolor.h"

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
  EVT_IDLE( EventsSelectionDialog::OnIdle )

  EVT_CHOICE( ID_CHOICE_OPERATOR_FUNCTION_TYPES, EventsSelectionDialog::OnChoiceOperatorFunctionTypesSelected )

  EVT_LISTBOX_DCLICK( ID_CHECKLISTBOX_TYPES, EventsSelectionDialog::OnChecklistboxTypesDoubleClicked )
  EVT_LISTBOX( ID_CHECKLISTBOX_TYPES, EventsSelectionDialog::OnChecklistboxTypesSelected )
  EVT_CHECKLISTBOX( ID_CHECKLISTBOX_TYPES, EventsSelectionDialog::OnChecklistboxTypesToggled )

  EVT_BUTTON( ID_BUTTON_SET_ALL_TYPES, EventsSelectionDialog::OnButtonSetAllTypesClick )

  EVT_BUTTON( ID_BUTTON_UNSET_ALL_TYPES, EventsSelectionDialog::OnButtonUnsetAllTypesClick )

  EVT_CHOICE( ID_CHOICE_OPERATOR_TYPE_VALUE, EventsSelectionDialog::OnChoiceOperatorTypeValueSelected )

  EVT_CHOICE( ID_CHOICE_OPERATOR_FUNCTION_VALUES, EventsSelectionDialog::OnChoiceOperatorFunctionValuesSelected )

  EVT_LISTBOX_DCLICK( ID_CHECKLISTBOX_VALUES, EventsSelectionDialog::OnChecklistboxValuesDoubleClicked )
  EVT_CHECKLISTBOX( ID_CHECKLISTBOX_VALUES, EventsSelectionDialog::OnChecklistboxValuesToggled )

  EVT_BUTTON( ID_BUTTON_ADD_VALUES, EventsSelectionDialog::OnButtonAddValuesClick )

  EVT_BUTTON( ID_BUTTON_SET_ALL_VALUES, EventsSelectionDialog::OnButtonSetAllValuesClick )

  EVT_BUTTON( ID_BUTTON_UNSET_ALL_VALUES, EventsSelectionDialog::OnButtonUnsetAllValuesClick )

  EVT_BUTTON( wxID_APPLY, EventsSelectionDialog::OnApplyClick )

////@end EventsSelectionDialog event table entries

END_EVENT_TABLE()


/*!
 * EventsSelectionDialog constructors
 */
EventsSelectionDialog::EventsSelectionDialog()
{
  Init();
}


void EventsSelectionDialog::TransferDataToWindowPreCreateControls( Window *whichWindow,
                                                                   bool whichHideOperatorsList )
{
  // Set member variables
  hideOperatorsList = whichHideOperatorsList;

  currentWindow = whichWindow;
  currentFilter = currentWindow->getFilter();

  // Get both event types lists, the full one and the selected events one
  set< TEventType > tmpEventTypes = currentWindow->getTrace()->getLoadedEvents();

  vector< TEventType > labeledTypes;
  currentWindow->getTrace()->getEventLabels().getTypes( labeledTypes );

  tmpEventTypes.insert( labeledTypes.begin(), labeledTypes.end() );

  eventTypes.clear();
  for( set< TEventType >::iterator it = tmpEventTypes.begin(); it != tmpEventTypes.end(); ++it )
  {
    eventTypes.push_back( (*it) );
  }

  vector< TEventType > selectedTypes;
  currentFilter->getEventType( selectedTypes );

  for( vector< TEventType >::iterator it = eventTypes.begin(); it != eventTypes.end(); ++it )
  {
    // build labeled name
    string tmpstr;
    currentWindow->getTrace()->getEventLabels().getEventTypeLabel( (*it), tmpstr );
    labeledEventTypes.Add( wxString() << (*it) << _( " " ) << wxString::FromAscii( tmpstr.c_str() ) );

    // to check if selected
    vector< TEventType >::iterator itaux = find( selectedTypes.begin(), selectedTypes.end(), ( *it ) );
    if ( itaux != selectedTypes.end() )
    {
      selectedEventTypes.Add( int( it - eventTypes.begin() ) );
    }
  }

  if ( selectedEventTypes.GetCount() > 0 )
    firstEventTypePos = selectedEventTypes[ 0 ];

/*
    wxArrayInt indexEventTypesSelected;
    wxArrayString unknownEventTypes; 
    for( unsigned int idx = 0; idx < eventTypesSelected.GetCount(); ++idx )
    {
      long tmpLong;
      eventTypesSelected[ idx ].ToLong( &tmpLong );
      int tmpValue = m_choices.Index( tmpLong );

      if( tmpValue != -1 )
        indexEventTypesSelected.Add( tmpValue );
      else
        unknownEventTypes.Add( eventTypesSelected[ idx ] );
    }

    // merge eventTypes + unknownTypes
    for( unsigned int idx = 0; idx < unknownEventTypes.GetCount(); ++idx )
    {
      eventTypes.Add( unknownEventTypes[ idx ] );
      numEventTypes = eventTypes.GetCount();
      indexEventTypesSelected.Add( numEventTypes - 1 );
    }
*/

  // Event values: all of the current type, and only the present in the filter
  map< TEventValue, string > auxValues;
  currentWindow->getTrace()->getEventLabels().getValues( eventTypes[ firstEventTypePos ], auxValues );
  for( map< TEventValue, string >::iterator it = auxValues.begin(); it != auxValues.end(); ++it )
  {
    eventValues.Add( (*it).first );
  }

  vector< TEventValue > tmpValues;
  currentFilter->getEventValue( tmpValues );
  for( vector<TEventValue>::iterator it = tmpValues.begin(); it != tmpValues.end(); ++it )
  {
    selectedEventValues.Add( (*it) );
  }

  originalSelectedEventValues = selectedEventValues;
/*
  vector< TEventValue > tmpValues;
  currentFilter->getEventValue( tmpValues );
  for( vector<TEventValue>::iterator it = tmpValues.begin(); it != tmpValues.end(); ++it )
  {
    originalGlobalSelectedEventValues.Add( (*it) );
    globalSelectedEventValues.Add( (*it) );
  }
*/
/*
  for( vector< TEventType >::iterator it = eventTypes.begin(); it != eventTypes.end(); ++it )
  {
    map< TEventValue, string > auxValues;
    currentWindow->getTrace()->getEventLabels().getValues( (*it), auxValues );

    for( map< TEventValue, string >::iterator it2 = auxValues.begin(); it2 != auxValues.end(); ++it2 )
    {
      eventValues.Add( wxString::FromAscii( LabelConstructor::eventValueLabel( currentWindow, (*it), (*it2).first, true ).c_str()));
    }
  }
*/
}

void EventsSelectionDialog::TransferDataToWindowPostCreateControls()
{
  if ( hideOperatorsList )
  {
    bool recursive = true;
    staticTextFunctionTypes->Hide();
    choiceOperatorFunctionTypes->Hide();
    boxSizerFunctionTypes->Hide( recursive );

    staticTextFunctionValues->Hide();
    choiceOperatorFunctionValues->Hide();
    boxSizerFunctionValues->Hide( recursive );
  }

  // Get functions list for event types and set the selected one
  vector<string> filterFunctions;
  currentFilter->getAllFilterFunctions( filterFunctions );

  for( vector<string>::iterator it = filterFunctions.begin(); it != filterFunctions.end(); ++it )
  {
    // int i = choiceOperatorFunctionTypes->Append( _((*it).c_str()) );
    int i = choiceOperatorFunctionTypes->Append( wxString::FromAscii( (*it).c_str() ) );
    if( (*it) == currentFilter->getEventTypeFunction() )
      choiceOperatorFunctionTypes->SetSelection( i );
  }

  // fill event types, check them, and position the selection in the first one
  checkListSelectTypes->InsertItems( labeledEventTypes, 0 );

  for ( unsigned int i = 0; i < selectedEventTypes.GetCount(); ++i )
  {
    checkListSelectTypes->Check( selectedEventTypes[ i ] );
  }

  checkListSelectTypes->SetSelection( firstEventTypePos );

  // fill and/or 
  choiceOperatorTypeValue->Append( _( "And" ) );
  choiceOperatorTypeValue->Append( _( "Or" ) );
  if( currentFilter->getOpTypeValue() == Filter::AND )
    choiceOperatorTypeValue->SetSelection( 0 );
  else
    choiceOperatorTypeValue->SetSelection( 1 );

  // Get functions list for event values and set the selected one
  for( vector<string>::iterator it = filterFunctions.begin(); it != filterFunctions.end(); ++it )
  {
    //int i = choiceOperatorFunctionValues->Append( _((*it).c_str()) );
    int i = choiceOperatorFunctionValues->Append( wxString::FromAscii( (*it).c_str() ) );
    if( (*it) == currentFilter->getEventValueFunction() )
      choiceOperatorFunctionValues->SetSelection( i );
  }

  // fill and set event values
  currentType = eventTypes[ firstEventTypePos ];
  UpdateCheckListboxValues( currentType );
}


EventsSelectionDialog::EventsSelectionDialog(  wxWindow* parent,
                                               Window *whichWindow,
                                               bool whichHideOperatorsList,
                                               const wxString& caption,
                                               wxWindowID id,
                                               const wxPoint& pos,
                                               const wxSize& size,
                                               long style)
{
  Init();
  TransferDataToWindowPreCreateControls( whichWindow, whichHideOperatorsList );
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
}


/*!
 * Member initialisation
 */
void EventsSelectionDialog::Init()
{
////@begin EventsSelectionDialog member initialisation
  boxSizerFunctionTypes = NULL;
  staticTextFunctionTypes = NULL;
  choiceOperatorFunctionTypes = NULL;
  checkListSelectTypes = NULL;
  buttonSetAllTypes = NULL;
  buttonUnsetAllTypes = NULL;
  choiceOperatorTypeValue = NULL;
  boxSizerFunctionValues = NULL;
  staticTextFunctionValues = NULL;
  choiceOperatorFunctionValues = NULL;
  checkListSelectValues = NULL;
  textCtrlAddValues = NULL;
  buttonAddValues = NULL;
  buttonSetAllValues = NULL;
  buttonUnsetAllValues = NULL;
  boxSizerOperatorsChoice = NULL;
////@end EventsSelectionDialog member initialisation

  hideOperatorsList = false;

  currentWindow = NULL;
  currentFilter = NULL;

  changedEventTypesFunction = false;
  selectedEventTypesFunction = 0;

  changedEventTypesSelection = false;
  eventTypes.clear();
  labeledEventTypes.Clear();
  selectedEventTypes.Clear();
  currentType = TEventType( 0 );

  changedOperatorTypeValue = false;
  selectedOperatorTypeValue = 0;

  changedEventValuesFunction = false;
  selectedEventValuesFunction = 0;

  changedEventValues = false;
  firstEventTypePos = 0;
  eventValues.Clear();
  selectedEventValues.Clear();
  originalSelectedEventValues.Clear();
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

  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer2->Add(itemBoxSizer3, 1, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

  wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer3->Add(itemBoxSizer4, 1, wxGROW|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer5Static = new wxStaticBox(itemDialog1, wxID_ANY, _(" Types "));
  wxStaticBoxSizer* itemStaticBoxSizer5 = new wxStaticBoxSizer(itemStaticBoxSizer5Static, wxVERTICAL);
  itemBoxSizer4->Add(itemStaticBoxSizer5, 1, wxGROW|wxLEFT|wxRIGHT, 5);

  boxSizerFunctionTypes = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer5->Add(boxSizerFunctionTypes, 0, wxALIGN_RIGHT|wxLEFT|wxRIGHT, 5);

  staticTextFunctionTypes = new wxStaticText( itemDialog1, ID_STATIC_TEXT_FUNCTION_TYPES, _("Function"), wxDefaultPosition, wxDefaultSize, 0 );
  boxSizerFunctionTypes->Add(staticTextFunctionTypes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString choiceOperatorFunctionTypesStrings;
  choiceOperatorFunctionTypes = new wxChoice( itemDialog1, ID_CHOICE_OPERATOR_FUNCTION_TYPES, wxDefaultPosition, wxDefaultSize, choiceOperatorFunctionTypesStrings, 0 );
  boxSizerFunctionTypes->Add(choiceOperatorFunctionTypes, 2, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  wxArrayString checkListSelectTypesStrings;
  checkListSelectTypes = new wxCheckListBox( itemDialog1, ID_CHECKLISTBOX_TYPES, wxDefaultPosition, wxDefaultSize, checkListSelectTypesStrings, wxLB_EXTENDED|wxLB_HSCROLL );
  itemStaticBoxSizer5->Add(checkListSelectTypes, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer5->Add(itemBoxSizer10, 0, wxALIGN_RIGHT|wxLEFT, 5);

  buttonSetAllTypes = new wxButton( itemDialog1, ID_BUTTON_SET_ALL_TYPES, _("Set all"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer10->Add(buttonSetAllTypes, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

  buttonUnsetAllTypes = new wxButton( itemDialog1, ID_BUTTON_UNSET_ALL_TYPES, _("Unset all"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer10->Add(buttonUnsetAllTypes, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer4->Add(itemBoxSizer13, 0, wxGROW|wxTOP, 5);

  wxStaticLine* itemStaticLine14 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
  itemBoxSizer13->Add(itemStaticLine14, 1, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer13->Add(itemBoxSizer15, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

  wxArrayString choiceOperatorTypeValueStrings;
  choiceOperatorTypeValue = new wxChoice( itemDialog1, ID_CHOICE_OPERATOR_TYPE_VALUE, wxDefaultPosition, wxDefaultSize, choiceOperatorTypeValueStrings, 0 );
  itemBoxSizer15->Add(choiceOperatorTypeValue, 1, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

  wxStaticLine* itemStaticLine17 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
  itemBoxSizer13->Add(itemStaticLine17, 1, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT, 5);

  wxStaticBox* itemStaticBoxSizer18Static = new wxStaticBox(itemDialog1, wxID_ANY, _(" Values "));
  wxStaticBoxSizer* itemStaticBoxSizer18 = new wxStaticBoxSizer(itemStaticBoxSizer18Static, wxVERTICAL);
  itemBoxSizer4->Add(itemStaticBoxSizer18, 1, wxGROW|wxLEFT|wxRIGHT, 5);

  boxSizerFunctionValues = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer18->Add(boxSizerFunctionValues, 0, wxALIGN_RIGHT|wxLEFT|wxRIGHT, 5);

  staticTextFunctionValues = new wxStaticText( itemDialog1, wxID_STATIC, _("Function"), wxDefaultPosition, wxDefaultSize, 0 );
  boxSizerFunctionValues->Add(staticTextFunctionValues, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString choiceOperatorFunctionValuesStrings;
  choiceOperatorFunctionValues = new wxChoice( itemDialog1, ID_CHOICE_OPERATOR_FUNCTION_VALUES, wxDefaultPosition, wxDefaultSize, choiceOperatorFunctionValuesStrings, 0 );
  boxSizerFunctionValues->Add(choiceOperatorFunctionValues, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  wxArrayString checkListSelectValuesStrings;
  checkListSelectValues = new wxCheckListBox( itemDialog1, ID_CHECKLISTBOX_VALUES, wxDefaultPosition, wxDefaultSize, checkListSelectValuesStrings, wxLB_EXTENDED|wxLB_HSCROLL );
  itemStaticBoxSizer18->Add(checkListSelectValues, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer18->Add(itemBoxSizer23, 0, wxGROW|wxALL, 5);

  textCtrlAddValues = new wxTextCtrl( itemDialog1, ID_TEXTCTRL_ADD_VALUES, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer23->Add(textCtrlAddValues, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

  buttonAddValues = new wxButton( itemDialog1, ID_BUTTON_ADD_VALUES, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer23->Add(buttonAddValues, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer18->Add(itemBoxSizer26, 0, wxALIGN_RIGHT|wxRIGHT, 5);

  buttonSetAllValues = new wxButton( itemDialog1, ID_BUTTON_SET_ALL_VALUES, _("Set all"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer26->Add(buttonSetAllValues, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  buttonUnsetAllValues = new wxButton( itemDialog1, ID_BUTTON_UNSET_ALL_VALUES, _("Unset all"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer26->Add(buttonUnsetAllValues, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

  wxStaticLine* itemStaticLine29 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemBoxSizer2->Add(itemStaticLine29, 0, wxGROW|wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer30 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer30, 0, wxALIGN_RIGHT|wxLEFT|wxRIGHT|wxTOP, 5);
  wxButton* itemButton31 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer30->AddButton(itemButton31);

  wxButton* itemButton32 = new wxButton( itemDialog1, wxID_APPLY, _("&Apply"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer30->AddButton(itemButton32);

  itemStdDialogButtonSizer30->Realize();

  boxSizerOperatorsChoice = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(boxSizerOperatorsChoice, 0, wxGROW|wxALL, 5);

  // Connect events and objects
  textCtrlAddValues->Connect(ID_TEXTCTRL_ADD_VALUES, wxEVT_KEY_DOWN, wxKeyEventHandler(EventsSelectionDialog::OnTextCtrlKeyDown), NULL, this);
////@end EventsSelectionDialog content construction

  textCtrlAddValues->SetValidator( wxTextValidator( wxFILTER_NUMERIC ));

  TransferDataToWindowPostCreateControls();
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


int EventsSelectionDialog::GetEventTypesFunction( string &whichName ) const
{
  whichName = choiceOperatorFunctionTypes->GetString( selectedEventTypesFunction ).mb_str( wxConvUTF8 );

  return selectedEventTypesFunction;
}


static int wxCMPFUNC_CONV compare_int(int *a, int *b)
{
  if ( *a > *b )
    return 1;
  else if ( *a < *b )
    return -1;
  else
    return 0;
}


wxArrayInt EventsSelectionDialog::GetEventTypesSelection() const
{
  wxArrayInt selections;

  for( size_t i = 0; i < selectedEventTypes.GetCount(); ++i )
  {
    selections.Add( eventTypes[ selectedEventTypes[ i ] ] );
  }

  selections.Sort( compare_int );

  return selections;
}


int EventsSelectionDialog::GetOperatorTypeValue( string &whichName ) const
{
  whichName = choiceOperatorTypeValue->GetString( selectedOperatorTypeValue ).mb_str( wxConvUTF8 );

  return choiceOperatorTypeValue->GetSelection();
}


int EventsSelectionDialog::GetEventValuesFunction( string &whichName ) const
{
  whichName = choiceOperatorFunctionValues->GetString( selectedEventValuesFunction ).mb_str( wxConvUTF8 );

  return selectedEventValuesFunction;
}


wxArrayInt EventsSelectionDialog::GetEventValues() const
{
  return selectedEventValues;
}


bool EventsSelectionDialog::ChangedEventTypesFunction() const
{
  return changedEventTypesFunction;
}


bool EventsSelectionDialog::ChangedEventTypesSelection() const
{
  return changedEventTypesSelection;
}


bool EventsSelectionDialog::ChangedOperatorTypeValue() const
{
  return changedOperatorTypeValue;
}


bool EventsSelectionDialog::ChangedEventValuesFunction() const
{
  return changedEventValuesFunction;
}


bool EventsSelectionDialog::ChangedEventValues() const
{
  return changedEventValues;
}


/*!
 * wxEVT_IDLE event handler for ID_EVENTSSELECTIONDIALOG
 */
void EventsSelectionDialog::OnIdle( wxIdleEvent& event )
{
  buttonSetAllTypes->Enable( true );
  buttonUnsetAllTypes->Enable( checkListSelectTypes->GetCount() > 0 );
  buttonSetAllValues->Enable( true );
  buttonUnsetAllValues->Enable( checkListSelectValues->GetCount() > 0 );
  buttonAddValues->Enable( !textCtrlAddValues->IsEmpty() );
}


void EventsSelectionDialog::checkAll( wxCheckListBox *boxlist, bool value )
{
  for ( unsigned int i = 0; i < boxlist->GetCount(); ++i )
  {
    boxlist->Check( i, value );
  }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SET_ALL_TYPES
 */
void EventsSelectionDialog::OnButtonSetAllTypesClick( wxCommandEvent& event )
{
  checkAll( checkListSelectTypes, true );
  changedEventTypesSelection = HasChanged( checkListSelectTypes, selectedEventTypes );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_UNSET_ALL_TYPES
 */
void EventsSelectionDialog::OnButtonUnsetAllTypesClick( wxCommandEvent& event )
{
  checkAll( checkListSelectTypes, false );
  changedEventTypesSelection = HasChanged( checkListSelectTypes, selectedEventTypes );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SET_ALL_VALUES
 */
void EventsSelectionDialog::OnButtonSetAllValuesClick( wxCommandEvent& event )
{
  checkAll( checkListSelectValues, true );
  changedEventValues = HasChanged( checkListSelectValues, selectedEventValues );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_UNSET_ALL_VALUES
 */
void EventsSelectionDialog::OnButtonUnsetAllValuesClick( wxCommandEvent& event )
{
  checkAll( checkListSelectValues, false );
  changedEventValues = HasChanged( checkListSelectValues, selectedEventValues );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_APPLY
 */

void EventsSelectionDialog::OnApplyClick( wxCommandEvent& event )
{
  TransferWindowToData();
  EndModal( wxID_OK );
}


bool EventsSelectionDialog::HasChanged( wxChoice *choice, int selectedFunction ) const
{
  return ( choice->GetCurrentSelection() != selectedFunction );
}


bool EventsSelectionDialog::HasChanged( wxCheckListBox *checkList, wxArrayInt &index ) const
{
  bool changed = false;

  wxArrayInt tmpIndex;

//  unsigned int numSelections = checkList->GetSelections( tmpIndex );  // DOESNT WORK!!!???
  unsigned int numSelections = 0;
  for( unsigned int i = 0; i < checkList->GetCount(); ++i )
  {
    if ( checkList->IsChecked( i ) )
    {
      numSelections++;
      tmpIndex.Add( i );
    }
  }

  if ( index.Count() == numSelections )
  {
    for( unsigned int i = 0; i < numSelections; ++i )
    {
      if ( tmpIndex[ i ] != index[ i ] )
      {
        changed = true;
        break;
      }
    }
  }
  else
    changed = true;

  return changed;
}


bool EventsSelectionDialog::HasChanged( wxArrayInt &arr1, wxArrayInt &arr2 ) const
{
  bool changed = false;

  if ( arr1.Count() == arr2.Count() )
  {
    for( unsigned int i = 0; i < arr1.Count(); ++i )
    {
      if ( arr2.Index( arr1[ i ] ) == wxNOT_FOUND )
      {
        changed = true;
        break;
      }
    }
  }
  else
    changed = true;

  return changed;
}


bool EventsSelectionDialog::CopyChanges( wxChoice *choice, int &selectedFunction )
{
  bool changed = HasChanged( choice, selectedFunction );
  selectedFunction = choice->GetCurrentSelection();

  return changed;
}


bool EventsSelectionDialog::CopyChanges( wxCheckListBox *checkList,
                                         wxArrayInt &index,
                                         wxArrayString &selected,
                                         bool copyStrings )
{
  bool changed = HasChanged( checkList, index );

  index.Clear();
  //unsigned int numSelections = checkList->GetSelections( index );
  unsigned int numSelections = 0;
  for( unsigned int i = 0; i < checkList->GetCount(); ++i )
  {
    if ( checkList->IsChecked( i ) )
    {
      numSelections++;
      index.Add( i );
    }
  }

  if ( copyStrings )
  {
    selected.Clear();

    for( unsigned int i = 0; i < numSelections; ++i )
    {
       selected.Add( checkList->GetString( i ) );
    }
  }

  return changed;
}


void EventsSelectionDialog::TransferWindowToData()
{
  changedEventTypesFunction = CopyChanges( choiceOperatorFunctionTypes, selectedEventTypesFunction );

  wxArrayString unused;
  changedEventTypesSelection = CopyChanges( checkListSelectTypes, selectedEventTypes, unused );

  changedOperatorTypeValue = CopyChanges( choiceOperatorTypeValue, selectedOperatorTypeValue );

  changedEventValuesFunction = CopyChanges( choiceOperatorFunctionValues, selectedEventValuesFunction );

  BackupCheckListboxValues();
  changedEventValues = HasChanged( selectedEventValues, originalSelectedEventValues );
}


/*!
 * wxEVT_COMMAND_CHECKLISTBOX_TOGGLED event handler for ID_CHECKLISTBOX_TYPES
 */

void EventsSelectionDialog::OnChecklistboxTypesToggled( wxCommandEvent& event )
{
  int pos = event.GetInt();
  currentType = eventTypes[ pos ];
  changedEventTypesSelection = HasChanged( checkListSelectTypes, selectedEventTypes );
}


/*!
 * wxEVT_COMMAND_CHECKLISTBOX_TOGGLED event handler for ID_CHECKLISTBOX_VALUES
 */

void EventsSelectionDialog::OnChecklistboxValuesToggled( wxCommandEvent& event )
{
  BackupCheckListboxValues();
  changedEventValues = HasChanged( selectedEventValues, originalSelectedEventValues );
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_OPERATOR_FUNCTION_TYPES
 */

void EventsSelectionDialog::OnChoiceOperatorFunctionTypesSelected( wxCommandEvent& event )
{
  changedEventTypesFunction = HasChanged( choiceOperatorFunctionTypes, selectedEventTypesFunction );
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_OPERATOR_FUNCTION_VALUES
 */

void EventsSelectionDialog::OnChoiceOperatorFunctionValuesSelected( wxCommandEvent& event )
{
  changedEventValuesFunction = HasChanged( choiceOperatorFunctionValues, selectedEventValuesFunction );
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_OPERATOR_TYPE_VALUE
 */

void EventsSelectionDialog::OnChoiceOperatorTypeValueSelected( wxCommandEvent& event )
{
  changedOperatorTypeValue = HasChanged( choiceOperatorTypeValue, selectedOperatorTypeValue );
}


/*!
 * wxEVT_COMMAND_LISTBOX_DOUBLECLICKED event handler for ID_CHECKLISTBOX_TYPES
 */

void EventsSelectionDialog::OnChecklistboxTypesDoubleClicked( wxCommandEvent& event )
{
  int pos = event.GetInt();
  currentType = eventTypes[ pos ];
  checkListSelectTypes->Check( pos, !checkListSelectTypes->IsChecked( pos ) );
  changedEventTypesSelection = HasChanged( checkListSelectTypes, selectedEventTypes );
}


// PRECOND: eventValues always updated and sorted
void EventsSelectionDialog::BackupCheckListboxValues()
{
  for( unsigned int i = 0; i < eventValues.GetCount(); ++i )
  {
    if ( checkListSelectValues->IsChecked( i ) )
    {
      if( selectedEventValues.Index( eventValues[i] ) == wxNOT_FOUND )
        selectedEventValues.Add( eventValues[i] );
    }
    else
    {
      int pos = selectedEventValues.Index( eventValues[i] );
      if( pos != wxNOT_FOUND )
        selectedEventValues.RemoveAt( pos );
    }
  }

  selectedEventValues.Sort( compare_int );
}

void EventsSelectionDialog::UpdateCheckListboxValues( TEventType type )
{
  eventValues.Clear();

  map< TEventValue, string > auxValues;
  currentWindow->getTrace()->getEventLabels().getValues( type, auxValues );
  for( map< TEventValue, string >::iterator it = auxValues.begin(); it != auxValues.end(); ++it )
  {
    eventValues.Add( (*it).first );
  }

  for( unsigned int i = 0; i < selectedEventValues.GetCount(); ++i )
  {
    if ( eventValues.Index( selectedEventValues[ i ] ) == wxNOT_FOUND )
      eventValues.Add( selectedEventValues[ i ] );
  }

  eventValues.Sort( compare_int );

  wxArrayString tmpEventValues;
  for( unsigned int i = 0; i < eventValues.GetCount(); ++i )
  {
    string tmpLabel = LabelConstructor::eventValueLabel( currentWindow, type, eventValues[ i ], true );
    if ( tmpLabel == "" )
    {
      stringstream tmpStrLabel;
      tmpStrLabel << eventValues[ i ];
      tmpLabel = tmpStrLabel.str();
    }
    tmpEventValues.Add( wxString::FromAscii( tmpLabel.c_str() ) );
  }

  checkListSelectValues->Clear();
  checkListSelectValues->InsertItems( tmpEventValues, 0 );

  for( unsigned int i = 0; i < eventValues.GetCount(); ++i )
  {
    if ( selectedEventValues.Index( eventValues[ i ] ) != wxNOT_FOUND )
      checkListSelectValues->Check( i );
  }
}


/*!
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_CHECKLISTBOX_TYPES
 */

void EventsSelectionDialog::OnChecklistboxTypesSelected( wxCommandEvent& event )
{
  BackupCheckListboxValues();
  int pos = event.GetInt();
  currentType = eventTypes[ pos ];
  UpdateCheckListboxValues( currentType );
}


void EventsSelectionDialog::GetEventValueLabels( wxArrayString & whichEventValues )
{
  PRV_UINT32 precision = 0;
  SemanticInfoType lastType = currentWindow->getSemanticInfoType();
  TSemanticValue lastMin = currentWindow->getMinimumY();
  TSemanticValue lastMax = currentWindow->getMaximumY();
  bool codeColorSet = currentWindow->IsCodeColorSet();
//  GradientColor::TGradientFunction gradientFunc = currentWindow->getGradientColor().getGradientFunction();

  if( codeColorSet )
  {
    int endLimit = ceil( lastMax );
  
    if( lastType != EVENTTYPE_TYPE )
    {
      if( lastType == APPL_TYPE )
        endLimit = currentWindow->getTrace()->totalApplications() - 1;
      else if( lastType == TASK_TYPE )
        endLimit = currentWindow->getTrace()->totalTasks() - 1;
      else if( lastType == THREAD_TYPE )
        endLimit = currentWindow->getTrace()->totalThreads() - 1;
      else if( lastType == NODE_TYPE )
        endLimit = currentWindow->getTrace()->totalNodes() - 1;
      else if( lastType == CPU_TYPE )
        endLimit = currentWindow->getTrace()->totalCPUs() - 1;
      else if( lastMax - lastMin > 200 )
        endLimit = 200 + floor( lastMin );
    }
    int typeEndLimit = 0;
    
    for( int i = floor( lastMin ); i <= endLimit; ++i )
    {
      if( lastType == EVENTTYPE_TYPE && !currentWindow->getTrace()->eventLoaded( i ) )
        continue;
        
      string tmpstr;
      if( lastType == STATE_TYPE &&
          !currentWindow->getTrace()->getStateLabels().getStateLabel( i, tmpstr ) )
        continue;
      if( lastType == EVENTVALUE_TYPE &&
          !currentWindow->getTrace()->getEventLabels().getEventValueLabel( i, tmpstr ) )
        continue;
        
      if( ( lastType == EVENTTYPE_TYPE ||
            lastType == EVENTVALUE_TYPE ||
            lastType == STATE_TYPE )
            && typeEndLimit > 200 )
        break;
      else
        ++typeEndLimit;

      wxString tmpStr = wxString::FromAscii( LabelConstructor::semanticLabel( currentWindow, i, true, precision ).c_str() );

      whichEventValues.Add( tmpStr );
    }
  }
  else
  {
    wxString tmpStr;
    tmpStr << wxT("< ") << wxString::FromAscii( LabelConstructor::semanticLabel( currentWindow, lastMin, false, precision ).c_str() );
    whichEventValues.Add( tmpStr );

    TSemanticValue step = ( lastMax - lastMin ) / 20.0;
    for( int i = 0; i <= 20; ++i )
    {
      tmpStr.Clear();
      tmpStr << wxString::FromAscii( LabelConstructor::semanticLabel( currentWindow, ( i * step ) + lastMin, false, precision ).c_str() );
      whichEventValues.Add( tmpStr );
    }

    tmpStr.Clear();
    tmpStr << wxT("> ") << wxString::FromAscii( LabelConstructor::semanticLabel( currentWindow, lastMax, false, precision ).c_str() );
    whichEventValues.Add( tmpStr );
  }
}



/*!
 * wxEVT_COMMAND_LISTBOX_DOUBLECLICKED event handler for ID_CHECKLISTBOX_VALUES
 */

void EventsSelectionDialog::OnChecklistboxValuesDoubleClicked( wxCommandEvent& event )
{
  int pos = event.GetInt();
  checkListSelectValues->Check( pos, !checkListSelectValues->IsChecked( pos ) );
  BackupCheckListboxValues();
  changedEventValues = HasChanged( selectedEventValues, originalSelectedEventValues );
}


void EventsSelectionDialog::InsertValueFromTextCtrl()
{
  // read from the ctrl
  long tmpLong;
  textCtrlAddValues->GetValue().ToLong( &tmpLong );

  // add it to the types
  if( eventValues.Index( tmpLong ) == wxNOT_FOUND )
  {
    eventValues.Add( tmpLong );
    eventValues.Sort( compare_int );
  }

  if( selectedEventValues.Index( tmpLong ) == wxNOT_FOUND )
  {
    selectedEventValues.Add( tmpLong );
    selectedEventValues.Sort( compare_int );
  }

  // update changed?
  UpdateCheckListboxValues( currentType );
  changedEventValues = HasChanged( selectedEventValues, originalSelectedEventValues );

  // empty the ctrl
  textCtrlAddValues->Clear();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ADD_VALUES
 */

void EventsSelectionDialog::OnButtonAddValuesClick( wxCommandEvent& event )
{
  InsertValueFromTextCtrl();
}


/*!
 * wxEVT_KEY_DOWN event handler for ID_TEXTCTRL_ADD_VALUES
 */

void EventsSelectionDialog::OnTextCtrlKeyDown( wxKeyEvent& event )
{
  if ( event.GetKeyCode() == WXK_RETURN )
    InsertValueFromTextCtrl();
  else
    event.Skip();
}

