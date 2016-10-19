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

#ifndef _EVENTSSELECTIONDIALOG_H_
#define _EVENTSSELECTIONDIALOG_H_


/*!
 * Includes
 */

#include <vector>
using namespace std;

#include "paraverkerneltypes.h"
#include "gtimeline.h"

////@begin includes
#include "wx/statline.h"
#include "wx/tglbtn.h"
////@end includes



/*!
 * Forward declarations
 */

////@begin forward declarations
class wxBoxSizer;
class wxToggleButton;
////@end forward declarations

class gTimeline;
class Filter;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_EVENTSSELECTIONDIALOG 10053
#define ID_STATIC_TEXT_FUNCTION_TYPES 10170
#define ID_CHOICE_OPERATOR_FUNCTION_TYPES 10054
#define ID_CHECKLISTBOX_TYPES 10161
#define ID_BUTTON_SET_ALL_TYPES 10163
#define ID_BUTTON_UNSET_ALL_TYPES 10164
#define ID_CHOICE_OPERATOR_TYPE_VALUE 10055
#define ID_CHOICE_OPERATOR_FUNCTION_VALUES 10056
#define ID_CHECKLISTBOX_VALUES 10162
#define ID_TEXTCTRL_ADD_VALUES 10168
#define ID_BUTTON_ADD_VALUES 10169
#define ID_TOGGLEBUTTON_SHORT_LABELS 10000
#define ID_BUTTON_SET_ALL_VALUES 10165
#define ID_BUTTON_UNSET_ALL_VALUES 10166
#define SYMBOL_EVENTSSELECTIONDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_EVENTSSELECTIONDIALOG_TITLE _("Events Selection")
#define SYMBOL_EVENTSSELECTIONDIALOG_IDNAME ID_EVENTSSELECTIONDIALOG
#define SYMBOL_EVENTSSELECTIONDIALOG_SIZE wxSize(800, 400)
#define SYMBOL_EVENTSSELECTIONDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * EventsSelectionDialog class declaration
 */

class EventsSelectionDialog: public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( EventsSelectionDialog )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  EventsSelectionDialog();

  EventsSelectionDialog( wxWindow* parent,
                         Window *whichWindow,
//                         wxArrayString &whichSelectedEventTypes,
//                         wxArrayString &whichSelectedEventValues,
                         bool hideOperatorsList = false,
                         const wxString& caption = SYMBOL_EVENTSSELECTIONDIALOG_TITLE,
                         wxWindowID id = SYMBOL_EVENTSSELECTIONDIALOG_IDNAME,
                         const wxPoint& pos = SYMBOL_EVENTSSELECTIONDIALOG_POSITION,
                         const wxSize& size = SYMBOL_EVENTSSELECTIONDIALOG_SIZE,
                         long style = SYMBOL_EVENTSSELECTIONDIALOG_STYLE );

  EventsSelectionDialog( wxWindow* parent,
                         vector<TEventType> types,
                         vector<TEventValue> values,
                         bool hideOperatorsList = false,
                         wxWindowID id = SYMBOL_EVENTSSELECTIONDIALOG_IDNAME,
                         const wxString& caption = SYMBOL_EVENTSSELECTIONDIALOG_TITLE,
                         const wxPoint& pos = SYMBOL_EVENTSSELECTIONDIALOG_POSITION,
                         const wxSize& size = SYMBOL_EVENTSSELECTIONDIALOG_SIZE,
                         long style = SYMBOL_EVENTSSELECTIONDIALOG_STYLE );
  EventsSelectionDialog( wxWindow* parent,
                         wxArrayString &whichTypes,
                         wxArrayInt &whichSelectedEventTypes,
                         //bool whichSelectedOpTypeValue,
                         //int &whichSelectedFunctionValue,
                         //vector<TEventValue> &whichValues,
                         //vector<TEventValue> &whichSelectedValues,
                         bool whichHideOperatorsList,
                         const wxString& caption,
                         wxWindowID id,
                         const wxPoint& pos,
                         const wxSize& size,
                         long style);

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_EVENTSSELECTIONDIALOG_IDNAME, const wxString& caption = SYMBOL_EVENTSSELECTIONDIALOG_TITLE, const wxPoint& pos = SYMBOL_EVENTSSELECTIONDIALOG_POSITION, const wxSize& size = SYMBOL_EVENTSSELECTIONDIALOG_SIZE, long style = SYMBOL_EVENTSSELECTIONDIALOG_STYLE );

  /// Destructor
  ~EventsSelectionDialog();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin EventsSelectionDialog event handler declarations

  /// wxEVT_IDLE event handler for ID_EVENTSSELECTIONDIALOG
  void OnIdle( wxIdleEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_OPERATOR_FUNCTION_TYPES
  void OnChoiceOperatorFunctionTypesSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_LISTBOX_DOUBLECLICKED event handler for ID_CHECKLISTBOX_TYPES
  void OnChecklistboxTypesDoubleClicked( wxCommandEvent& event );

  /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_CHECKLISTBOX_TYPES
  void OnChecklistboxTypesSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKLISTBOX_TOGGLED event handler for ID_CHECKLISTBOX_TYPES
  void OnChecklistboxTypesToggled( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SET_ALL_TYPES
  void OnButtonSetAllTypesClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_UNSET_ALL_TYPES
  void OnButtonUnsetAllTypesClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_OPERATOR_TYPE_VALUE
  void OnChoiceOperatorTypeValueSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_OPERATOR_FUNCTION_VALUES
  void OnChoiceOperatorFunctionValuesSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_LISTBOX_DOUBLECLICKED event handler for ID_CHECKLISTBOX_VALUES
  void OnChecklistboxValuesDoubleClicked( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKLISTBOX_TOGGLED event handler for ID_CHECKLISTBOX_VALUES
  void OnChecklistboxValuesToggled( wxCommandEvent& event );

  /// wxEVT_KEY_DOWN event handler for ID_TEXTCTRL_ADD_VALUES
  void OnTextCtrlKeyDown( wxKeyEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ADD_VALUES
  void OnButtonAddValuesClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_TOGGLEBUTTON_SHORT_LABELS
  void OnTogglebuttonShortLabelsClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SET_ALL_VALUES
  void OnButtonSetAllValuesClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_UNSET_ALL_VALUES
  void OnButtonUnsetAllValuesClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_APPLY
  void OnApplyClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for wxID_APPLY
  void OnApplyUpdate( wxUpdateUIEvent& event );

////@end EventsSelectionDialog event handler declarations

////@begin EventsSelectionDialog member function declarations

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end EventsSelectionDialog member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  int GetIndexEventTypesFunction() const;
  std::string GetNameEventTypesFunction() const;
  wxArrayInt GetEventTypesSelection() const;

  int GetIndexOperatorTypeValue() const;
  std::string  GetNameOperatorTypeValue() const;

  int GetIndexEventValuesFunction() const;
  std::string  GetNameEventValuesFunction() const;
  wxArrayInt GetEventValues() const;

  bool ChangedEventTypesFunction() const;
  bool ChangedEventTypesSelection() const;
  bool ChangedOperatorTypeValue() const ;
  bool ChangedEventValuesFunction() const;
  bool ChangedEventValues() const;

  private:

////@begin EventsSelectionDialog member variables
  wxBoxSizer* boxSizerFunctionTypes;
  wxStaticText* staticTextFunctionTypes;
  wxChoice* choiceOperatorFunctionTypes;
  wxCheckListBox* checkListSelectTypes;
  wxButton* buttonSetAllTypes;
  wxButton* buttonUnsetAllTypes;
  wxChoice* choiceOperatorTypeValue;
  wxBoxSizer* boxSizerFunctionValues;
  wxStaticText* staticTextFunctionValues;
  wxChoice* choiceOperatorFunctionValues;
  wxCheckListBox* checkListSelectValues;
  wxTextCtrl* textCtrlAddValues;
  wxButton* buttonAddValues;
  wxToggleButton* buttonShortLabels;
  wxButton* buttonSetAllValues;
  wxButton* buttonUnsetAllValues;
  wxButton* applyButton;
////@end EventsSelectionDialog member variables

  bool hideOperatorsList; // let the dialog show or not the type/values/and-or selectors
                          // Useful when called from filters
                          // ( TODO: please consider the alternative of inheritance design)

  // *** To ease access to the data of the filter ***
  Window               *currentWindow;
  Filter               *currentFilter;     // To set/get events info, and operator or/and

  // *** Related to selector of event types function ***
  int                  previousEventTypesFunction;
  bool                 changedEventTypesFunction;

  // *** Related to event types check list ***
  TEventType           currentType;
  vector< TEventType > eventTypes;         // FULL LIST of event types
  wxArrayString        labeledEventTypes;  // Labeled names of eventTypes
  wxArrayInt           selectedEventTypes; // INDEX to eventTypes (Selected/checked events)
  bool                 changedEventTypesSelection;

  // *** Related to selector of and/or operator ***
  int                  previousOperatorTypeValue;
  bool                 changedOperatorTypeValue;

  // *** Related to selector of event values function ***
  int                  previousEventValuesFunction;
  bool                 changedEventValuesFunction;

  // *** Related to event values check list ***
  unsigned int        firstEventTypePos;
  wxArrayInt           eventValues;         // related to the current selected type
  wxArrayInt           selectedEventValues; // global selected event type
  wxArrayInt           originalSelectedEventValues; // global selected event type
  bool                 changedEventValues;

  void checkAll( wxCheckListBox *boxlist, bool value );
  void GetEventValueLabels( wxArrayString & whichEventValues );
  void BackupCheckListboxValues();
  void UpdateCheckListboxValues( TEventType type );

  bool HasChanged( wxChoice *choice, int selectedFunction ) const;
  bool HasChanged( wxCheckListBox *checkList, wxArrayInt &index ) const;
  bool HasChanged( wxArrayInt &arr1, wxArrayInt &arr2 ) const;

  bool CopyChanges( wxChoice *choice, int &selectedFunction );
  bool CopyChanges( wxCheckListBox *checkList,
                    wxArrayInt &index,
                    wxArrayString &selected,
                    bool copyStrings = false );
  void InsertValueFromTextCtrl();

  //int compare_int(int *a, int *b);

  void TransferDataToWindowPreCreateControls( Window *whichWindow,
                                              bool whichHideOperatorsList);
  void EnableApplyButton();
  void TransferDataToWindowPostCreateControls();

  void TransferWindowToData();
  unsigned int GetSelections( wxCheckListBox *checkList, wxArrayInt &index ) const;

};

#endif
  // _EVENTSSELECTIONDIALOG_H_
