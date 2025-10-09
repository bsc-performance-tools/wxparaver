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

#include <iostream>

////@begin includes
////@end includes

#include "filter.h"

#include "eventsselectiondialog.h"
#include "labelconstructor.h"

using namespace std;

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
  EVT_CHECKBOX( ID_CHECKBOX_SET_ALL_TYPES, EventsSelectionDialog::OnCheckboxSetAllTypesClick )
  EVT_UPDATE_UI( ID_CHECKBOX_SET_ALL_TYPES, EventsSelectionDialog::OnCheckboxSetAllTypesUpdate )
  EVT_TEXT( ID_TEXTCTRL_TYPES_REGEX_SEARCH, EventsSelectionDialog::OnTextctrlTypesRegexSearchTextUpdated )
  EVT_LISTBOX_DCLICK( ID_CHECKLISTBOX_TYPES, EventsSelectionDialog::OnChecklistboxTypesDoubleClicked )
  EVT_LISTBOX( ID_CHECKLISTBOX_TYPES, EventsSelectionDialog::OnChecklistboxTypesSelected )
  EVT_CHECKLISTBOX( ID_CHECKLISTBOX_TYPES, EventsSelectionDialog::OnChecklistboxTypesToggled )
  EVT_CHOICE( ID_CHOICE_OPERATOR_TYPE_VALUE, EventsSelectionDialog::OnChoiceOperatorTypeValueSelected )
  EVT_CHOICE( ID_CHOICE_OPERATOR_FUNCTION_VALUES, EventsSelectionDialog::OnChoiceOperatorFunctionValuesSelected )
  EVT_CHECKBOX( ID_CHECKBOX_SET_ALL_VALUES, EventsSelectionDialog::OnCheckboxSetAllValuesClick )
  EVT_UPDATE_UI( ID_CHECKBOX_SET_ALL_VALUES, EventsSelectionDialog::OnCheckboxSetAllValuesUpdate )
  EVT_TEXT( ID_TEXTCTRL_VALUES_REGEX_SEARCH, EventsSelectionDialog::OnTextctrlValuesRegexSearchTextUpdated )
  EVT_LISTBOX_DCLICK( ID_CHECKLISTBOX_VALUES, EventsSelectionDialog::OnChecklistboxValuesDoubleClicked )
  EVT_CHECKLISTBOX( ID_CHECKLISTBOX_VALUES, EventsSelectionDialog::OnChecklistboxValuesToggled )
  EVT_BUTTON( ID_BUTTON_ADD_VALUES, EventsSelectionDialog::OnButtonAddValuesClick )
  EVT_BUTTON( wxID_APPLY, EventsSelectionDialog::OnApplyClick )
  EVT_UPDATE_UI( wxID_APPLY, EventsSelectionDialog::OnApplyUpdate )
////@end EventsSelectionDialog event table entries

END_EVENT_TABLE()


static int wxCMPFUNC_CONV compare_int(int *a, int *b)
{
  if ( *a > *b )
    return 1;
  else if ( *a < *b )
    return -1;
  else
    return 0;
}


static int wxCMPFUNC_CONV compare_double(double *a, double *b)
{
  if ( *a > *b )
    return 1;
  else if ( *a < *b )
    return -1;
  else
    return 0;
}


/*!
 * EventsSelectionDialog constructors
 */
EventsSelectionDialog::EventsSelectionDialog()
{
  Init();
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
  checkboxSetAllTypes = NULL;
  typesRegexSearch = NULL;
  checkListSelectTypes = NULL;
  choiceOperatorTypeValue = NULL;
  boxSizerFunctionValues = NULL;
  staticTextFunctionValues = NULL;
  choiceOperatorFunctionValues = NULL;
  checkboxSetAllValues = NULL;
  valuesRegexSearch = NULL;
  checkListSelectValues = NULL;
  textCtrlAddValues = NULL;
  buttonAddValues = NULL;
  applyButton = NULL;
////@end EventsSelectionDialog member initialisation

  hideOperatorsList = false;

  currentWindow = nullptr;
  currentFilter = nullptr;

  changedEventTypesFunction = false;
  previousEventTypesFunction = 0;

  changedEventTypesSelection = false;
  currentType = TEventType( 0 );

  changedOperatorTypeValue = false;
  previousOperatorTypeValue = 0;

  changedEventValuesFunction = false;
  previousEventValuesFunction = 0;

  changedEventValues = false;
}


void EventsSelectionDialog::TransferDataToWindowPreCreateControls( Timeline *whichWindow,
                                                                   bool whichHideOperatorsList )
{
  currentWindow = whichWindow;
  currentFilter = currentWindow->getFilter();
  hideOperatorsList = whichHideOperatorsList;

  // First time all types are shown, also related values
  typesHandler = new EventTypesInfoManager( currentWindow, currentFilter );
  if( !typesHandler->isEmpty() )
  {
    currentType = typesHandler->getFirstTypeVisible();
    typesHandler->setCurrent( currentType );
  }
  valuesHandler = new EventValuesInfoManager( currentWindow, currentFilter, currentType );
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

  vector<string> filterFunctions;
  currentFilter->getAllFilterFunctions( filterFunctions );

  for( vector<string>::iterator it = filterFunctions.begin(); it != filterFunctions.end(); ++it )
  {
    // Fill wxChoice and select the current
    int i = choiceOperatorFunctionTypes->Append( wxString::FromUTF8( (*it).c_str() ) );
    if( (*it) == currentFilter->getEventTypeFunction() )
    {
      choiceOperatorFunctionTypes->SetSelection( i );
      previousEventTypesFunction = i;
    }
  }

  // fill and/or 
  choiceOperatorTypeValue->Append( _( "And" ) );
  choiceOperatorTypeValue->Append( _( "Or" ) );
  if( currentFilter->getOpTypeValue() == Filter::AND )
  {
    choiceOperatorTypeValue->SetSelection( 0 );
    previousOperatorTypeValue = 0;
  }
  else
  {
    choiceOperatorTypeValue->SetSelection( 1 );
    previousOperatorTypeValue = 1;
  }

  for( vector<string>::iterator it = filterFunctions.begin(); it != filterFunctions.end(); ++it )
  {
    int i = choiceOperatorFunctionValues->Append( wxString::FromUTF8( (*it).c_str() ) );
    if( (*it) == currentFilter->getEventValueFunction() )
    {
      choiceOperatorFunctionValues->SetSelection( i );
      previousEventValuesFunction = i;
    }
  }

  // Update both widgets
  UpdateWidgetChecklistboxTypes();
  if( typesHandler->countVisible() > 0 )
  {
    currentType = typesHandler->getCurrent();
    bool keepSelected = false; // Initially, read also from window filter
    UpdateChecklistboxValues( currentType, keepSelected );
  }

  EnableApplyButton();
}


EventsSelectionDialog::EventsSelectionDialog(  wxWindow* parent,
                                               Timeline *whichWindow,
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
  delete typesHandler;
  delete valuesHandler;
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

  wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer5->Add(itemBoxSizer5, 0, wxGROW|wxLEFT|wxRIGHT, 5);

  checkboxSetAllTypes = new wxCheckBox( itemDialog1, ID_CHECKBOX_SET_ALL_TYPES, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  checkboxSetAllTypes->SetValue(false);
  checkboxSetAllTypes->SetHelpText(_("Check/uncheck visible all types."));
  if (EventsSelectionDialog::ShowToolTips())
    checkboxSetAllTypes->SetToolTip(_("Check/uncheck visible all types."));
  itemBoxSizer5->Add(checkboxSetAllTypes, 0, wxALIGN_CENTER_VERTICAL, 5);

  wxStaticText* itemStaticText2 = new wxStaticText( itemDialog1, wxID_STATIC, _("Search:"), wxDefaultPosition, wxDefaultSize, 0 );
  if (EventsSelectionDialog::ShowToolTips())
    itemStaticText2->SetToolTip(_("Search by type or label."));
  itemBoxSizer5->Add(itemStaticText2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  typesRegexSearch = new wxTextCtrl( itemDialog1, ID_TEXTCTRL_TYPES_REGEX_SEARCH, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (EventsSelectionDialog::ShowToolTips())
    typesRegexSearch->SetToolTip(_("Search by type or label."));
  itemBoxSizer5->Add(typesRegexSearch, 1, wxGROW|wxTOP|wxBOTTOM, 5);

  wxArrayString checkListSelectTypesStrings;
  checkListSelectTypes = new wxCheckListBox( itemDialog1, ID_CHECKLISTBOX_TYPES, wxDefaultPosition, wxSize(500, -1), checkListSelectTypesStrings, wxLB_EXTENDED|wxLB_HSCROLL );
  itemStaticBoxSizer5->Add(checkListSelectTypes, 1, wxGROW|wxALL, 5);

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

  wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer18->Add(itemBoxSizer7, 0, wxGROW|wxLEFT|wxRIGHT, 5);

  checkboxSetAllValues = new wxCheckBox( itemDialog1, ID_CHECKBOX_SET_ALL_VALUES, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  checkboxSetAllValues->SetValue(false);
  if (EventsSelectionDialog::ShowToolTips())
    checkboxSetAllValues->SetToolTip(_("Check/uncheck all visible values."));
  itemBoxSizer7->Add(checkboxSetAllValues, 0, wxALIGN_CENTER_VERTICAL, 5);

  wxStaticText* itemStaticText9 = new wxStaticText( itemDialog1, wxID_STATIC, _("Search:"), wxDefaultPosition, wxDefaultSize, 0 );
  if (EventsSelectionDialog::ShowToolTips())
    itemStaticText9->SetToolTip(_("Search by value or label."));
  itemBoxSizer7->Add(itemStaticText9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  valuesRegexSearch = new wxTextCtrl( itemDialog1, ID_TEXTCTRL_VALUES_REGEX_SEARCH, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (EventsSelectionDialog::ShowToolTips())
    valuesRegexSearch->SetToolTip(_("Search by value or label."));
  itemBoxSizer7->Add(valuesRegexSearch, 1, wxGROW|wxTOP|wxBOTTOM, 5);

  wxArrayString checkListSelectValuesStrings;
  checkListSelectValues = new wxCheckListBox( itemDialog1, ID_CHECKLISTBOX_VALUES, wxDefaultPosition, wxDefaultSize, checkListSelectValuesStrings, wxLB_EXTENDED|wxLB_HSCROLL );
  itemStaticBoxSizer18->Add(checkListSelectValues, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer18->Add(itemBoxSizer23, 0, wxGROW|wxALL, 5);

  textCtrlAddValues = new wxTextCtrl( itemDialog1, ID_TEXTCTRL_ADD_VALUES, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (EventsSelectionDialog::ShowToolTips())
    textCtrlAddValues->SetToolTip(_("Insert new value (allowed  0-9 only)."));
  itemBoxSizer23->Add(textCtrlAddValues, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

  buttonAddValues = new wxButton( itemDialog1, ID_BUTTON_ADD_VALUES, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer23->Add(buttonAddValues, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

  wxStaticLine* itemStaticLine31 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemBoxSizer2->Add(itemStaticLine31, 0, wxGROW|wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer32 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer32, 0, wxALIGN_RIGHT|wxALL, 5);
  wxButton* itemButton33 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer32->AddButton(itemButton33);

  applyButton = new wxButton( itemDialog1, wxID_APPLY, _("&Apply"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer32->AddButton(applyButton);

  itemStdDialogButtonSizer32->Realize();

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


int EventsSelectionDialog::GetIndexEventTypesFunction() const
{
  return choiceOperatorFunctionTypes->GetSelection();
}


std::string EventsSelectionDialog::GetNameEventTypesFunction() const
{
  return std::string( choiceOperatorFunctionTypes->GetString( choiceOperatorFunctionTypes->GetSelection() ).mb_str( wxConvUTF8 ) );
}


wxArrayInt EventsSelectionDialog::GetEventTypesSelection() const
{
  return typesHandler->getSelected();
}


int EventsSelectionDialog::GetIndexOperatorTypeValue() const
{
  return choiceOperatorTypeValue->GetSelection();
}


std::string EventsSelectionDialog::GetNameOperatorTypeValue() const
{
  return std::string( choiceOperatorTypeValue->GetString( choiceOperatorTypeValue->GetSelection() ).mb_str( wxConvUTF8 ) );
}


int EventsSelectionDialog::GetIndexEventValuesFunction() const
{
  return choiceOperatorFunctionValues->GetSelection();
}


std::string EventsSelectionDialog::GetNameEventValuesFunction() const
{
  return std::string( choiceOperatorFunctionValues->GetString( choiceOperatorFunctionValues->GetSelection() ).mb_str( wxConvUTF8 ) );
}


wxArrayDouble EventsSelectionDialog::GetEventValues() const
{
  return valuesHandler->getSelected();
}


bool EventsSelectionDialog::ChangedEventTypesFunction() const
{
  return changedEventTypesFunction;
}


bool EventsSelectionDialog::ChangedEventTypesSelection() const
{
  return typesHandler->getChangedSelected();
}


bool EventsSelectionDialog::ChangedOperatorTypeValue() const
{
  return changedOperatorTypeValue;
}


bool EventsSelectionDialog::ChangedEventValuesFunction() const
{
  return changedEventValuesFunction;
}


bool EventsSelectionDialog::ChangedEventValuesSelection() const
{
  return changedEventValues;
}


/*!
 * wxEVT_IDLE event handler for ID_EVENTSSELECTIONDIALOG
 */
void EventsSelectionDialog::OnIdle( wxIdleEvent& event )
{
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


unsigned int EventsSelectionDialog::GetSelections( wxCheckListBox *checkList, wxArrayInt &index ) const
{
  unsigned int numSelections = 0;
  for( unsigned int i = 0; i < checkList->GetCount(); ++i )
  {
    if ( checkList->IsChecked( i ) )
    {
      numSelections++;
      index.Add( i );
    }
  }

  return numSelections;
}


bool EventsSelectionDialog::HasChanged( wxCheckListBox *checkList, wxArrayInt &index ) const
{
  bool changed = false;

  wxArrayInt tmpIndex;
  unsigned int numSelections = GetSelections( checkList, tmpIndex ); 

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


bool EventsSelectionDialog::HasChanged( wxCheckListBox *checkList, EventInfoManager *manager ) const
{
  bool changed = false;

  wxArrayString dummyVisible;
  wxArrayInt dummyPosVisible;
  wxArrayInt dummyGlobalSelected;
  wxArrayInt tmpGUISelected;
  int dummyFirstPosSelectedVisible;

  manager->getSelectedFromVisible( dummyVisible, dummyPosVisible, dummyGlobalSelected, tmpGUISelected, dummyFirstPosSelectedVisible );

  wxArrayInt tmpIndex;
  unsigned int numSelections = GetSelections( checkList, tmpIndex ); 

  if ( tmpGUISelected.Count() == numSelections )
  {
    for( unsigned int i = 0; i < numSelections; ++i )
    {
      if ( tmpIndex[ i ] != tmpGUISelected[ i ] )
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

  unsigned int numSelections = GetSelections( checkList, index ); 

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
  changedEventTypesFunction = CopyChanges( choiceOperatorFunctionTypes, previousEventTypesFunction );
  changedEventTypesSelection = typesHandler->getChangedSelected();
  changedOperatorTypeValue = CopyChanges( choiceOperatorTypeValue, previousOperatorTypeValue );
  changedEventValuesFunction = CopyChanges( choiceOperatorFunctionValues, previousEventValuesFunction );
  changedEventValues = valuesHandler->getChangedSelected();
}


/*!
 * wxEVT_COMMAND_CHECKLISTBOX_TOGGLED event handler for ID_CHECKLISTBOX_TYPES
 */
void EventsSelectionDialog::OnChecklistboxTypesToggled( wxCommandEvent& event )
{
  int pos = event.GetInt();

  typesHandler->setSelected( pos, checkListSelectTypes->IsChecked( pos ) );
  TEventType tmpNewCurrentType = typesHandler->getVisible( pos );
  if ( currentType != tmpNewCurrentType )
  {
    currentType = tmpNewCurrentType;
    typesHandler->setCurrent( currentType );
    UpdateChecklistboxValues( currentType );
  }
}


/*!
 * wxEVT_COMMAND_CHECKLISTBOX_TOGGLED event handler for ID_CHECKLISTBOX_VALUES
 */
void EventsSelectionDialog::OnChecklistboxValuesToggled( wxCommandEvent& event )
{
  valuesHandler->transferFrom( checkListSelectValues );
  changedEventValues = valuesHandler->getChangedSelected();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_OPERATOR_FUNCTION_TYPES
 */
void EventsSelectionDialog::OnChoiceOperatorFunctionTypesSelected( wxCommandEvent& event )
{
  changedEventTypesFunction = HasChanged( choiceOperatorFunctionTypes, previousEventTypesFunction );
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_OPERATOR_FUNCTION_VALUES
 */
void EventsSelectionDialog::OnChoiceOperatorFunctionValuesSelected( wxCommandEvent& event )
{
  changedEventValuesFunction = HasChanged( choiceOperatorFunctionValues, previousEventValuesFunction );
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_OPERATOR_TYPE_VALUE
 */
void EventsSelectionDialog::OnChoiceOperatorTypeValueSelected( wxCommandEvent& event )
{
  changedOperatorTypeValue = HasChanged( choiceOperatorTypeValue, previousOperatorTypeValue );
  UpdateChecklistboxValues( currentType, true );
}


/*!
 * wxEVT_COMMAND_LISTBOX_DOUBLECLICKED event handler for ID_CHECKLISTBOX_TYPES
 */
void EventsSelectionDialog::OnChecklistboxTypesDoubleClicked( wxCommandEvent& event )
{
  int pos = event.GetInt();
  if ( pos >= 0 )
  {
    checkListSelectTypes->Check( pos, !checkListSelectTypes->IsChecked( pos ) );
    TEventType tmpNewCurrentType = typesHandler->getVisible( pos );
    typesHandler->setSelected( tmpNewCurrentType, checkListSelectTypes->IsChecked( pos ) );

    // Because after checkListSelectTypes->Clear() the "Selected" event triggers this function with pos < 0
    if ( currentType != tmpNewCurrentType )
    {
      currentType = tmpNewCurrentType;
      typesHandler->setCurrent( currentType );
      typesHandler->setSelected( currentType, checkListSelectTypes->IsChecked( pos ) );
      UpdateChecklistboxValues( currentType );
    }
  }
}


void EventsSelectionDialog::UpdateWidgetChecklistboxValues()
{
  wxArrayString tmpVisible;
  wxArrayInt dummyPosVisible;
  wxArrayInt dummyGlobalSelected;
  wxArrayInt tmpGUISelected;
  int firstPos;

  valuesHandler->getSelectedFromVisible( tmpVisible, dummyPosVisible, dummyGlobalSelected, tmpGUISelected, firstPos );

  // Insert strings of visible values
  checkListSelectValues->Clear();
  if( !tmpVisible.IsEmpty() )
  {
    checkListSelectValues->InsertItems( tmpVisible, 0 );

    // Check them
    for ( unsigned int i = 0; i < tmpGUISelected.GetCount(); ++i )
    {
      checkListSelectValues->Check( tmpGUISelected[ i ] );
    }

    // Select first one (previosly found)
    checkListSelectValues->SetSelection( firstPos );
  }
}


void EventsSelectionDialog::UpdateWidgetChecklistboxTypes()
{
  wxArrayString tmpVisible;
  wxArrayInt dummyPosVisible;
  wxArrayInt dummyGlobalSelected;
  wxArrayInt tmpGUISelected;
  int firstPos = 0;
  bool tmpUpdateFirstPos = true;

  typesHandler->getSelectedFromVisible( tmpVisible, dummyPosVisible, dummyGlobalSelected, tmpGUISelected, firstPos, tmpUpdateFirstPos );

  // Insert strings of visible types
  // Following backup of currentType is needed because Clear emits Selected event
  // and OnChecklistboxTypesSelected changes it
  TEventType tmpCurrent = typesHandler->getCurrent();
  currentType = tmpCurrent;
  checkListSelectTypes->Clear(); // ---> throws Selected event with pos -1

  typesHandler->setCurrent( tmpCurrent );

  if( !tmpVisible.IsEmpty() )
  {
    checkListSelectTypes->InsertItems( tmpVisible, 0 );

    // Check them
    for ( unsigned int i = 0; i < tmpGUISelected.GetCount(); ++i )
    {
      checkListSelectTypes->Check( tmpGUISelected[ i ] );
    }

    // Select first one (previosly found)
    checkListSelectTypes->SetSelection( firstPos );
  }
}


// Updates: - variable eventValues to be returned
//          - checklist widget
void EventsSelectionDialog::UpdateChecklistboxValues( TEventType whichType, bool keepSelected )
{
  valuesHandler->init( whichType, !(bool)GetIndexOperatorTypeValue(), keepSelected );
  UpdateWidgetChecklistboxValues();
}


/*!
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_CHECKLISTBOX_TYPES
 */
void EventsSelectionDialog::OnChecklistboxTypesSelected( wxCommandEvent& event )
{
  int pos = event.GetInt();
  if ( pos >= 0 )
  {
    // Because after checkListSelectTypes->Clear() the "Selected" event triggers this function with pos < 0
    TEventType tmpNewCurrentType = typesHandler->getVisible( pos );
    if ( currentType != tmpNewCurrentType )
    {
      currentType = tmpNewCurrentType;
      typesHandler->setCurrent( currentType );
      typesHandler->setSelected( currentType, checkListSelectTypes->IsChecked( pos ) );
      UpdateChecklistboxValues( currentType );
    }
  }
}


/*!
 * wxEVT_COMMAND_LISTBOX_DOUBLECLICKED event handler for ID_CHECKLISTBOX_VALUES
 */
void EventsSelectionDialog::OnChecklistboxValuesDoubleClicked( wxCommandEvent& event )
{
  int pos = event.GetInt();
  checkListSelectValues->Check( pos, !checkListSelectValues->IsChecked( pos ) );
  valuesHandler->transferFrom( checkListSelectValues );
  changedEventValues = valuesHandler->getChangedSelected();
}


void EventsSelectionDialog::InsertValueFromTextCtrl()
{
  // Read from the widget wxtextctrl
  double tmpDouble;
  textCtrlAddValues->GetValue().ToDouble( &tmpDouble );

  changedEventValues = valuesHandler->insert( tmpDouble, _("") );

  // Update changed?
  if ( changedEventValues )
  {
    UpdateChecklistboxValues( currentType );
  }

  // Empty the wxtextctrl
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


void EventsSelectionDialog::EnableApplyButton()
{
  // Save current Filter state
  string tmpCurrentTypeFunc  = currentFilter->getEventTypeFunction();
  string tmpCurrentValueFunc = currentFilter->getEventValueFunction();
  
  currentFilter->setEventTypeFunction( GetNameEventTypesFunction() );
  currentFilter->setEventValueFunction( GetNameEventValuesFunction() );
  
  // Some functions like '<' only admit 1 parameter
  wxArrayInt dummyIndex;
  unsigned int numTypesSelected  = typesHandler->getSelected().Count();
  unsigned int numValuesSelected = valuesHandler->getSelected().Count();
  bool enabledByFunctionTypes  = currentFilter->allowedEventTypeFunctionNumParams( numTypesSelected );
  bool enabledByFunctionValues = currentFilter->allowedEventValueFunctionNumParams( numValuesSelected );
  
  // Restore Filter state
  currentFilter->setEventTypeFunction( tmpCurrentTypeFunc );
  currentFilter->setEventValueFunction( tmpCurrentValueFunc );

  // Detected some change in types or values?
  bool someChange = typesHandler->getChangedSelected() || valuesHandler->getChangedSelected();

  // Be consistent with and/or operation
  bool on;
  if ( choiceOperatorTypeValue->GetSelection() == 0 ) // ==> AND
    on = ( enabledByFunctionTypes && enabledByFunctionValues ) && someChange;
  else
    on = ( enabledByFunctionTypes || enabledByFunctionValues ) && someChange;

  applyButton->Enable( on );
}


/*!
 * wxEVT_UPDATE_UI event handler for wxID_APPLY
 */
void EventsSelectionDialog::OnApplyUpdate( wxUpdateUIEvent& event )
{
  EnableApplyButton();
}


EventInfoManager::EventInfoManager( Timeline *whichWindow, Filter *whichFilter )
{
  currentWindow = whichWindow;
  currentFilter = whichFilter;
  firstPosSelectedVisible = 0;
  filterRegEx = vector< wxRegEx * >();
  changedSelection = false;
}


EventInfoManager::~EventInfoManager()
{
  clearAllRegEx();
}


// regex
// If whichRegEx compiles, add it and return true
bool EventInfoManager::add( wxString whichRegEx )
{
  bool regExAdded = false;
  
  wxRegEx *labelRegEx = new wxRegEx();

  if ( labelRegEx->Compile( whichRegEx ) )
  {
    filterRegEx.push_back( labelRegEx );
    regExAdded = true;
  }
  
  return regExAdded;
}


void EventInfoManager::clearAllRegEx()
{
  for( vector< wxRegEx * >::iterator it = filterRegEx.begin(); it != filterRegEx.end(); ++it )
  {
    delete *it;
  }

  filterRegEx.clear();
}


bool EventInfoManager::matchesAllRegex( string whichName, string whichValue )
{
  bool matchesAll = true;

  for( vector< wxRegEx * >::iterator it = filterRegEx.begin(); it != filterRegEx.end(); ++it )
  {
    if ( !(*it)->Matches( wxString::FromUTF8( whichName.c_str() ) ) &&
         !(*it)->Matches( wxString::FromUTF8( whichValue.c_str() ) ) )
    {
      matchesAll = false;
      break;
    }
  }

  return matchesAll;
}


EventTypesInfoManager::EventTypesInfoManager( Timeline *whichWindow, Filter *whichFilter )
  : EventInfoManager( whichWindow, whichFilter )
{
  init();
}


void EventTypesInfoManager::setAllVisible()
{
  visible.Clear();

  for( size_t i = 0; i < fullList.size(); ++i )
  {
    visible.Add( i );
  }
}


void EventTypesInfoManager::setSelected( int vPos, bool isChecked )
{
  firstPosSelectedVisible = visible[ vPos ];

  int elemPos = selected.Index( visible[ vPos ] );
  if ( isChecked )
  {
    if ( elemPos == wxNOT_FOUND )
      selected.Add( visible[ vPos ] );
  }
  else
  {
    if ( elemPos != wxNOT_FOUND )
      selected.RemoveAt( elemPos );
  }

  selected.Sort( compare_int );

  setChangedSelection();
}


void EventTypesInfoManager::setSelected( TEventType whichSelected, bool isChecked )
{
  int elemPos = wxNOT_FOUND;

  vector< TEventType >::iterator itType = find( fullList.begin(), fullList.end(), whichSelected );
  if ( itType != fullList.end() )
  {
    elemPos = int( itType - fullList.begin() );
  }

  int posInSelected = selected.Index( elemPos );
  if ( isChecked )
  {
    if ( posInSelected == wxNOT_FOUND )
      selected.Add( elemPos );
  }
  else
  {
    if ( posInSelected != wxNOT_FOUND )
      selected.RemoveAt( posInSelected );
  }
  
  selected.Sort( compare_int );

  setChangedSelection();
}


void EventTypesInfoManager::setAllSelected()
{
  for( size_t i = 0; i < visible.GetCount(); ++i )
  {
    if ( selected.Index( visible[ i ] ) == wxNOT_FOUND )
      selected.Add( visible[ i ] );
  }

  selected.Sort( compare_int );

  setChangedSelection();
}


void EventTypesInfoManager::setAllUnselected()
{
  for( size_t i = 0; i < visible.GetCount(); ++i )
  {
    int tmpPos = selected.Index( visible[ i ] );
    if ( tmpPos != wxNOT_FOUND )
      selected.RemoveAt( tmpPos );
  }

  selected.Sort( compare_int );

  setChangedSelection();
}


void EventTypesInfoManager::init()
{
  fullList.clear();
  labels.Clear();
  visible.Clear();
  selected.Clear();
  initialSelected.Clear();

  // 1) Get complete event types list
  set< TEventType > tmpEventTypes = currentWindow->getTrace()->getLoadedEvents();

  vector< TEventType > labeledTypes;
  currentWindow->getTrace()->getEventLabels().getTypes( labeledTypes );

  tmpEventTypes.insert( labeledTypes.begin(), labeledTypes.end() );

  fullList.clear();
  fullList.assign( tmpEventTypes.begin(), tmpEventTypes.end() ); // set to vector
  
  vector< TEventType > tmpSelectedTypes;
  currentFilter->getEventType( tmpSelectedTypes );

  // 3) Build labels for every type and then filter them using given parameter *labelRE (regular expr.)
  for( vector< TEventType >::iterator it = fullList.begin(); it != fullList.end(); ++it )
  {
    // build labeled name
    string tmpstr;
    currentWindow->getTrace()->getEventLabels().getEventTypeLabel( (*it), tmpstr );

    labels.Add( wxString() << ( *it ) << _( " " ) << wxString::FromUTF8( tmpstr.c_str() ) );

    // Check if event type i in current filter
    vector< TEventType >::iterator itType = find( tmpSelectedTypes.begin(), tmpSelectedTypes.end(), ( *it ) );
    if ( itType != tmpSelectedTypes.end() )
    {
      int tmpPos = int( it - fullList.begin() );
      selected.Add( tmpPos );
      initialSelected.Add( tmpPos );
    }

    visible.Add( int( it - fullList.begin() ) );
  }

  // 5) Set class attributes
  if ( selected.GetCount() > 0 )
  {
    firstPosSelectedVisible = selected[ 0 ];
    currentType = firstPosSelectedVisible;
  }

  changedSelection = false;
}


wxArrayInt EventTypesInfoManager::getSelected()
{
  wxArrayInt tmpTypesSelected;

  for( size_t i = 0; i < selected.GetCount(); ++i )
  {
    tmpTypesSelected.Add( fullList[ selected[ i ]] );
  }

  return tmpTypesSelected;
}


void EventTypesInfoManager::getSelectedFromVisible( wxArrayString& whichVisible,
                                                    wxArrayInt &whichPosVisible,
                                                    wxArrayInt &whichGlobalSelection,
                                                    wxArrayInt &whichGUISelection,
                                                    int &whichFirstPosSelectedVisible,
                                                    bool updateFirstPosSelectedVisible )
{
  wxArrayString tmpVisible;
  wxArrayInt tmpPosVisible;
  wxArrayInt tmpGlobalSelection;
  wxArrayInt tmpGUISelection;
  int tmpFirstPosSelectedVisible = 0;

  bool foundFirst = false;

  unsigned int selectedPositionInGUI = 0;

  for ( unsigned int i = 0; i < visible.GetCount(); ++i )
  {
    // Pick type and label
    stringstream tmpValue;
    tmpValue << fullList[ visible[ i ] ];

    string tmpLabel( labels[ visible[ i ] ].mb_str() );

    if ( matchesAllRegex( tmpLabel, tmpValue.str() ) )
    {
      tmpVisible.Add( labels[ visible[ i ] ] );
      tmpPosVisible.Add( visible[ i ] );
      if ( selected.Index( visible[ i ] ) != wxNOT_FOUND )
      {
        tmpGUISelection.Add( selectedPositionInGUI );
        tmpGlobalSelection.Add( visible[ i ] );
      }
      
      
      if (( !foundFirst ) && ( fullList[ visible[ i ] ] == currentType ))
      {
        tmpFirstPosSelectedVisible = selectedPositionInGUI;
        foundFirst = true;
      }

      ++selectedPositionInGUI;
    }
  }
  
  whichVisible = tmpVisible;
  whichPosVisible = tmpPosVisible;
  whichGlobalSelection = tmpGlobalSelection;
  whichGUISelection = tmpGUISelection;
  whichFirstPosSelectedVisible = tmpFirstPosSelectedVisible;

  if ( updateFirstPosSelectedVisible )
  {
    firstPosSelectedVisible = tmpFirstPosSelectedVisible;
  }
}


void EventTypesInfoManager::updateVisible()
{
  wxArrayString dummyVisible;
  wxArrayInt posVisible;
  wxArrayInt dummyGlobalSelected;
  wxArrayInt dummyGUISelected;
  int dummyFirstPosSelectedVisible;

  getSelectedFromVisible( dummyVisible, posVisible, dummyGlobalSelected, dummyGUISelected, dummyFirstPosSelectedVisible );

  setVisible( posVisible );
}


void EventTypesInfoManager::setChangedSelection()
{
  bool changed = false;
  
  if ( selected.GetCount() != initialSelected.GetCount() )
  {
    changed = true;
  }
  else
  {
    for( size_t i = 0; i < selected.GetCount(); ++i )
    {
      if ( selected[ i ] != initialSelected[ i ] )
      {
        changed = true;
        break;
      }
    }
  }

  changedSelection = changed;
}


EventValuesInfoManager::EventValuesInfoManager( Timeline *whichWindow, Filter *whichFilter, TEventType whichType )
  : EventInfoManager( whichWindow, whichFilter )
{
  currentType = whichType;
}


void EventValuesInfoManager::setAllVisible()
{
  visible.Clear();
  for( size_t i = 0; i < fullList.size(); ++i )
  {
    visible.Add( fullList[i] );
  }
}


void EventValuesInfoManager::init( TEventType whichType, bool typeValueOp, bool keepSelected )
{
  currentType = whichType;

  fullList.Clear();
  labels.Clear();
  visible.Clear();
  if ( ! keepSelected )
    selected.Clear();

  // 1) Read values for this event type from the trace of the window
  map< TEventValue, string > auxValues;
  currentWindow->getTrace()->getEventLabels().getValues( whichType, auxValues );
  for( map< TEventValue, string >::iterator it = auxValues.begin(); it != auxValues.end(); ++it )
  {
    fullList.Add( (*it).first );
  }

  // 2) Append the ones in the filter as selected
  if ( ! keepSelected )
  {
    vector< TSemanticValue > tmpValues;
    currentFilter->getEventValue( tmpValues );
    for( vector<TSemanticValue>::iterator it = tmpValues.begin(); it != tmpValues.end(); ++it )
    {
      selected.Add( (*it) );
    }
  }

  // 3) Are values in filter in fullList? Add them if missing
  for( unsigned int i = 0; i < selected.GetCount(); ++i )
  {
    if ( fullList.Index( selected[ i ] ) == wxNOT_FOUND && typeValueOp != Filter::AND )
    {
      fullList.Add( selected[ i ] );
    }
  }

  for( unsigned int i = 0; i < addedFullList.GetCount(); ++i )
  {
    if ( fullList.Index( addedFullList[ i ] ) == wxNOT_FOUND )
    {
      fullList.Add( addedFullList[ i ] );
    }
  }

  // 4)
  fullList.Sort( compare_double );

  // 5) Build labels for every value
  wxArrayString tmpEventValues;
  for( unsigned int i = 0; i < fullList.GetCount(); ++i )
  {
    stringstream tmpValue;
    tmpValue << fullList[ i ];

    string tmpLabel = LabelConstructor::eventValueLabel( currentWindow, whichType, fullList[ i ], true );
    if ( tmpLabel == "" )
    {
      tmpLabel = tmpValue.str();
    }

    tmpEventValues.Add( wxString::FromUTF8( tmpLabel.c_str() ) );

    // and also 6) Filter values using given parameter regular expr.
    if ( matchesAllRegex( tmpLabel, tmpValue.str() ) )
    {
      visible.Add( fullList[ i ] );
    }
  }

  labels = tmpEventValues;
}


wxArrayDouble EventValuesInfoManager::getSelected()
{
  return selected;
}


void EventValuesInfoManager::setAllSelected()
{
  for( size_t i = 0; i < visible.GetCount(); ++i )
  {
    if ( selected.Index( visible[ i ] ) == wxNOT_FOUND )
      selected.Add( visible[ i ] );
  }

  selected.Sort( compare_double );

  setChangedSelection();
}


void EventValuesInfoManager::setAllUnselected()
{
  for( size_t i = 0; i < visible.GetCount(); ++i )
  {
    int tmpPos = selected.Index( visible[ i ] );
    if ( tmpPos != wxNOT_FOUND )
      selected.RemoveAt( tmpPos );
  }

  selected.Sort( compare_double );

  setChangedSelection();
}


void EventValuesInfoManager::getSelectedFromVisible( wxArrayString& whichVisible,
                                                     wxArrayInt &whichPosVisible,
                                                     wxArrayInt &whichGlobalSelection,
                                                     wxArrayInt &whichGUISelection,
                                                     int &whichFirstPosSelectedVisible,
                                                     bool updateFirstPosSelectedVisible )
{
  wxArrayString tmpVisible;
  wxArrayInt tmpPosVisible;
  wxArrayInt tmpGlobalSelection;
  wxArrayInt tmpGUISelection;

  firstPosSelectedVisible = 0;
  int tmpFirstPosSelectedVisible = 0;

  bool foundFirst = false;

  unsigned int selectedPositionInGUI = 0;

  for ( unsigned int i = 0; i < visible.GetCount(); ++i )
  {
    int fullListPos = fullList.Index( visible[ i ] );
    string tmpLabel( labels[ fullListPos ].mb_str() );

    stringstream tmpValue;
    tmpValue << fullList[ fullListPos ];
    if ( matchesAllRegex( tmpLabel, tmpValue.str() ) )
    {
      tmpVisible.Add( labels[ fullListPos ] );
      tmpPosVisible.Add( fullListPos );

      if ( selected.Index( visible[ i ] ) != wxNOT_FOUND )
      {
        tmpGUISelection.Add( selectedPositionInGUI );
        tmpGlobalSelection.Add( visible[ i ] );

        if ( !foundFirst )
        {
          tmpFirstPosSelectedVisible = selectedPositionInGUI;
          foundFirst = true;
        }
      }

      ++selectedPositionInGUI;
    }
  }

  whichVisible = tmpVisible;
  whichPosVisible = tmpPosVisible;
  whichGlobalSelection = tmpGlobalSelection;
  whichGUISelection = tmpGUISelection;
  whichFirstPosSelectedVisible = tmpFirstPosSelectedVisible;

  if ( updateFirstPosSelectedVisible )
  {
    firstPosSelectedVisible = tmpFirstPosSelectedVisible;
  }
}


bool EventValuesInfoManager::insert( double whichValue, wxString whichLabel )
{
  bool changed = false;

  if( fullList.Index( whichValue ) == wxNOT_FOUND )
  {
    // Insert in eventValues
    fullList.Add( whichValue );
    fullList.Sort( compare_double );
    
    // Insert in labeledEventValues
    int insertPos = fullList.Index( whichValue );
    int maxPos = fullList.GetCount() - 1;
    wxArrayString tmpLabeledEventValues;
    for( int i = 0; i < insertPos; ++i )
    {
      tmpLabeledEventValues.Add( labels[ i ] );
    }

    tmpLabeledEventValues.Add( whichLabel );

    for( int i = insertPos; i < maxPos; ++i )
    {
      tmpLabeledEventValues.Add( labels[ i ] );
    }

    labels = tmpLabeledEventValues;

    changed = true;
  }

  if( selected.Index( whichValue ) == wxNOT_FOUND )
  {
    selected.Add( whichValue );
    selected.Sort( compare_double );
    changed = true;
  }

  if ( visible.Index( whichValue ) == wxNOT_FOUND )
  {
    stringstream tmpValue;
    tmpValue << whichValue;
    if ( matchesAllRegex( string( whichLabel.mb_str() ), tmpValue.str() ) )
    {
      visible.Add( whichValue );
      visible.Sort( compare_double );
    }
  }

  addedFullList.Add( whichValue );

  setChangedSelection();

  return changed;
}


void EventValuesInfoManager::transferFrom( wxCheckListBox *whichList )
{
  for( unsigned int i = 0; i < visible.GetCount(); ++i )
  {
    if ( whichList->IsChecked( i ) )
    {
      if( selected.Index( visible[i] ) == wxNOT_FOUND )
      {
        selected.Add( visible[i] );
      }
    }
    else
    {
      int pos = selected.Index( visible[ i ] );
      if( pos != wxNOT_FOUND )
      {
        selected.RemoveAt( pos );
      }
    }
  }

  selected.Sort( compare_double );

  setChangedSelection();
}


void EventValuesInfoManager::setChangedSelection()
{
  bool changed = false;

  if ( selected.GetCount() != initialSelected.GetCount() )
  {
    changed = true;
  }
  else
  {
    for( size_t i = 0; i < selected.GetCount(); ++i )
    {
      if ( selected[ i ] != initialSelected[ i ] )
      {
        changed = true;
        break;
      }
    }
  }

  changedSelection = changed;
}


void EventValuesInfoManager::setVisible( wxArrayInt whichVisible )
{
  visible.Clear();
  for ( size_t i = 0; i < whichVisible.Count(); ++i )
  {
    visible.Add( fullList[ whichVisible[ i ] ] );
  }
}


void EventValuesInfoManager::updateVisible()
{
  wxArrayString dummyVisible;
  wxArrayInt posVisible;
  wxArrayInt dummyGlobalSelected;
  wxArrayInt dummyGUISelected;
  int dummyFirstPosSelectedVisible;

  getSelectedFromVisible( dummyVisible, posVisible, dummyGlobalSelected, dummyGUISelected, dummyFirstPosSelectedVisible );

  setVisible( posVisible );
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_TYPES_REGEX_SEARCH
 */
void EventsSelectionDialog::OnTextctrlTypesRegexSearchTextUpdated( wxCommandEvent& event )
{
  typesHandler->clearAllRegEx();
  typesHandler->setAllVisible();
  checkboxSetAllTypes->SetValue( false );

  if ( typesRegexSearch->GetValue().Len() > 0 )
  {
    if ( typesHandler->add( typesRegexSearch->GetValue() ) )
    {
      typesHandler->updateVisible();
      UpdateWidgetChecklistboxTypes();
      checkListSelectValues->Clear();
    }
  }
  else
  {
    typesHandler->updateVisible();
    UpdateWidgetChecklistboxTypes();
    checkListSelectValues->Clear();
  }
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SET_ALL_TYPES
 */
void EventsSelectionDialog::OnCheckboxSetAllTypesClick( wxCommandEvent& event )
{
  bool checked = checkboxSetAllTypes->GetValue();

  checkAll( checkListSelectTypes, checked );
  if ( checked )
  {
    typesHandler->setAllSelected();
  }
  else
  {
    typesHandler->setAllUnselected();
  }
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SET_ALL_VALUES
 */
void EventsSelectionDialog::OnCheckboxSetAllValuesClick( wxCommandEvent& event )
{
  bool checked = checkboxSetAllValues->GetValue();

  checkAll( checkListSelectValues, checked );
  changedEventValues = HasChanged( checkListSelectValues, valuesHandler );
  if ( checked )
  {
    valuesHandler->setAllSelected();
  }
  else
  {
    valuesHandler->setAllUnselected();
  }
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_VALUES_REGEX_SEARCH
 */
void EventsSelectionDialog::OnTextctrlValuesRegexSearchTextUpdated( wxCommandEvent& event )
{
  valuesHandler->clearAllRegEx();
  valuesHandler->setAllVisible();
  checkboxSetAllValues->SetValue( false );

  if ( valuesRegexSearch->GetValue().Len() > 0 )
  {
    if ( valuesHandler->add( valuesRegexSearch->GetValue() ) )
    {
      valuesHandler->updateVisible();
      UpdateChecklistboxValues( currentType );
    }
  }
  else
  {
    valuesHandler->updateVisible();
    UpdateChecklistboxValues( currentType );
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHECKBOX_SET_ALL_TYPES
 */
void EventsSelectionDialog::OnCheckboxSetAllTypesUpdate( wxUpdateUIEvent& event )
{
  checkboxSetAllTypes->Enable( checkListSelectTypes->GetCount() > 0 );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHECKBOX_SET_ALL_VALUES
 */
void EventsSelectionDialog::OnCheckboxSetAllValuesUpdate( wxUpdateUIEvent& event )
{
  checkboxSetAllValues->Enable( checkListSelectValues->GetCount() > 0 );
}

