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


#pragma once



/*!
 * Includes
 */

#include <vector>

#include "paraverkerneltypes.h"
#include "gtimeline.h"

////@begin includes
#include "wx/statline.h"
////@end includes

#include <wx/regex.h>
#include <wx/checkbox.h>
#include <wx/valtext.h> // DELETE ME?
#include <wx/stattext.h>


/*!
 * Forward declarations
 */

////@begin forward declarations
class wxBoxSizer;
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
#define ID_CHECKBOX_SET_ALL_TYPES 10007
#define ID_TEXTCTRL_TYPES_REGEX_SEARCH 10006
#define ID_CHECKLISTBOX_TYPES 10161
#define ID_CHOICE_OPERATOR_TYPE_VALUE 10055
#define ID_CHOICE_OPERATOR_FUNCTION_VALUES 10056
#define ID_CHECKBOX_SET_ALL_VALUES 10008
#define ID_TEXTCTRL_VALUES_REGEX_SEARCH 10009
#define ID_CHECKLISTBOX_VALUES 10162
#define ID_TEXTCTRL_ADD_VALUES 10168
#define ID_BUTTON_ADD_VALUES 10169
#define SYMBOL_EVENTSSELECTIONDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_EVENTSSELECTIONDIALOG_TITLE _("Events Selection")
#define SYMBOL_EVENTSSELECTIONDIALOG_IDNAME ID_EVENTSSELECTIONDIALOG
#define SYMBOL_EVENTSSELECTIONDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_EVENTSSELECTIONDIALOG_POSITION wxDefaultPosition
////@end control identifiers


class EventInfoManager
{
  public:
    EventInfoManager( Timeline *whichWindow, Filter *whichFilter );
    virtual ~EventInfoManager();

    virtual void transferFrom( wxCheckListBox *whichList ) = 0;

    virtual bool isEmpty() const = 0;
    
    // Visible
    virtual void setAllVisible() = 0;
    int getFirstPosSelectedVisible() { return firstPosSelectedVisible; };

    // Selected
    virtual void getSelectedFromVisible( wxArrayString& whichVisible,
                                         wxArrayInt &whichPosVisible,
                                         wxArrayInt &whichGlobalSelection,
                                         wxArrayInt &whichGUISelection,
                                         int &whichFirstPosSelectedVisible,
                                         bool updateFirstPosSelectedVisible = true ) = 0;

//    bool changedVisible();
    bool getChangedSelected() { return changedSelection ; };

    // regex
    bool add( wxString whichRegEx );
    void clearAllRegEx();

  protected:
    Timeline *currentWindow;
    Filter *currentFilter;

    int firstPosSelectedVisible;
    bool changedSelection;

    std::vector< wxRegEx * > filterRegEx;

    virtual void setChangedSelection() = 0;
    bool matchesAllRegex( std::string whichName, std::string whichValue );
    //bool matchesAllRegex( string whichName );
};


class EventTypesInfoManager : public EventInfoManager
{
  public:
    EventTypesInfoManager( Timeline *whichWindow, Filter *whichFilter );
    virtual ~EventTypesInfoManager() {};

    void init();
    virtual void transferFrom( wxCheckListBox *whichList ) {}

    bool isEmpty() const { return fullList.empty(); }
    TEventType getCurrent() { return currentType ; }
    void setCurrent( TEventType whichType ) { currentType = whichType ; }

    // Visible
    virtual void setAllVisible();
    void setVisible( wxArrayInt whichVisible ) { visible = whichVisible ; }

    //wxArrayString getVisible();
    TEventType getVisible( int pos ) { return fullList[ visible[ pos ] ] ; }
    unsigned int countVisible() { return visible.GetCount() ; }
    TEventType getFirstTypeVisible() { return fullList[ visible[ 0 ] ] ; }
    void updateVisible();

    // Selected
    void setSelected( int pos, bool isChecked );
    void setSelected( TEventType whichSelected, bool isChecked );
    void setAllSelected();
    void setAllUnselected();

    wxArrayInt getSelected();
    virtual void getSelectedFromVisible( wxArrayString& whichVisible,
                                         wxArrayInt &whichPosVisible,
                                         wxArrayInt &whichGlobalSelection,
                                         wxArrayInt &whichGUISelection,
                                         int &whichFirstPosSelectedVisible,
                                         bool updateFirstPosSelectedVisible = true );
  protected:
    virtual void setChangedSelection();

  private:
    std::vector< TEventType > fullList; // eventTypes;         // FULL LIST of event types
    wxArrayString             labels;   // labeledEventTypes;  // Labeled names of eventTypes
    wxArrayInt                selected; // selectedEventTypes; // INDEX to eventTypes (Selected/checked events)
    wxArrayInt                visible;  // visibleEventTypes;  // INDEX to all visible event types (matching reg. exps)
    
    TEventType currentType;
    wxArrayInt initialSelected;
};


class EventValuesInfoManager : public EventInfoManager
{
  public:
    EventValuesInfoManager( Timeline *whichWindow, Filter *whichFilter, TEventType whichType );
    virtual ~EventValuesInfoManager() {};
    
    void init( TEventType whichType, bool keepSelected = false );
    virtual void transferFrom( wxCheckListBox *whichList );
    bool isEmpty() const { return fullList.IsEmpty(); }
    bool insert( double whichValue, wxString whichLabel ); // Only values can be added

    // Visible
    virtual void setAllVisible();
    unsigned int countVisible() { return visible.GetCount() ; }
    void updateVisible();
    void setVisible( wxArrayInt whichVisible );

    // Selected
    void setAllSelected();
    void setAllUnselected();

    wxArrayDouble getSelected();
    virtual void getSelectedFromVisible( wxArrayString& whichVisible,
                                         wxArrayInt &whichPosVisible,
                                         wxArrayInt &whichGlobalSelection,
                                         wxArrayInt &whichGUISelection,
                                         int &whichFirstPosSelectedVisible,
                                         bool updateFirstPosSelectedVisible = true );
  protected:
    virtual void setChangedSelection();

  private:
    TEventType currentType;

    wxArrayDouble fullList;    // eventValues // related to the current selected value
    wxArrayString labels; // labeledEventValues;          // Labeled names of event values
    wxArrayDouble selected; //selectedEventValues;         // FULL LIST of selected event value
    wxArrayDouble visible; //visibleEventValues;          // FULL LIST to visible event values (matching reg. exps)

    wxArrayDouble addedFullList;
    wxArrayDouble initialSelected;
};


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
                         Timeline *whichWindow,
//                         wxArrayString &whichSelectedEventTypes,
//                         wxArrayString &whichSelectedEventValues,
                         bool hideOperatorsList = false,
                         const wxString& caption = SYMBOL_EVENTSSELECTIONDIALOG_TITLE,
                         wxWindowID id = SYMBOL_EVENTSSELECTIONDIALOG_IDNAME,
                         const wxPoint& pos = SYMBOL_EVENTSSELECTIONDIALOG_POSITION,
                         const wxSize& size = SYMBOL_EVENTSSELECTIONDIALOG_SIZE,
                         long style = SYMBOL_EVENTSSELECTIONDIALOG_STYLE );

  EventsSelectionDialog( wxWindow* parent,
                         std::vector<TEventType> types,
                         std::vector<TEventValue> values,
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

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SET_ALL_TYPES
  void OnCheckboxSetAllTypesClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_CHECKBOX_SET_ALL_TYPES
  void OnCheckboxSetAllTypesUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_TYPES_REGEX_SEARCH
  void OnTextctrlTypesRegexSearchTextUpdated( wxCommandEvent& event );

  /// wxEVT_COMMAND_LISTBOX_DOUBLECLICKED event handler for ID_CHECKLISTBOX_TYPES
  void OnChecklistboxTypesDoubleClicked( wxCommandEvent& event );

  /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_CHECKLISTBOX_TYPES
  void OnChecklistboxTypesSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKLISTBOX_TOGGLED event handler for ID_CHECKLISTBOX_TYPES
  void OnChecklistboxTypesToggled( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_OPERATOR_TYPE_VALUE
  void OnChoiceOperatorTypeValueSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_OPERATOR_FUNCTION_VALUES
  void OnChoiceOperatorFunctionValuesSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SET_ALL_VALUES
  void OnCheckboxSetAllValuesClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_CHECKBOX_SET_ALL_VALUES
  void OnCheckboxSetAllValuesUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_VALUES_REGEX_SEARCH
  void OnTextctrlValuesRegexSearchTextUpdated( wxCommandEvent& event );

  /// wxEVT_COMMAND_LISTBOX_DOUBLECLICKED event handler for ID_CHECKLISTBOX_VALUES
  void OnChecklistboxValuesDoubleClicked( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKLISTBOX_TOGGLED event handler for ID_CHECKLISTBOX_VALUES
  void OnChecklistboxValuesToggled( wxCommandEvent& event );

  /// wxEVT_KEY_DOWN event handler for ID_TEXTCTRL_ADD_VALUES
  void OnTextCtrlKeyDown( wxKeyEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ADD_VALUES
  void OnButtonAddValuesClick( wxCommandEvent& event );

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
  std::string GetNameOperatorTypeValue() const;

  int GetIndexEventValuesFunction() const;
  std::string GetNameEventValuesFunction() const;
  wxArrayDouble GetEventValues() const;

  bool ChangedEventTypesFunction() const;
  bool ChangedEventTypesSelection() const;
  bool ChangedOperatorTypeValue() const ;
  bool ChangedEventValuesFunction() const;
  bool ChangedEventValuesSelection() const;

  private:

////@begin EventsSelectionDialog member variables
  wxBoxSizer* boxSizerFunctionTypes;
  wxStaticText* staticTextFunctionTypes;
  wxChoice* choiceOperatorFunctionTypes;
  wxCheckBox* checkboxSetAllTypes;
  wxTextCtrl* typesRegexSearch;
  wxCheckListBox* checkListSelectTypes;
  wxChoice* choiceOperatorTypeValue;
  wxBoxSizer* boxSizerFunctionValues;
  wxStaticText* staticTextFunctionValues;
  wxChoice* choiceOperatorFunctionValues;
  wxCheckBox* checkboxSetAllValues;
  wxTextCtrl* valuesRegexSearch;
  wxCheckListBox* checkListSelectValues;
  wxTextCtrl* textCtrlAddValues;
  wxButton* buttonAddValues;
  wxButton* applyButton;
////@end EventsSelectionDialog member variables

  bool hideOperatorsList; // let the dialog show or not the type/values/and-or selectors
                          // Useful when called from filters
                          // ( TODO: please consider the alternative of inheritance design)

  // *** To ease access to the data of the filter ***
  Timeline               *currentWindow;
  Filter               *currentFilter;     // To set/get events info, and operator or/and

  EventValuesInfoManager *valuesHandler;
  EventTypesInfoManager  *typesHandler;

  // *** Related to selector of event types function ***
  int                  previousEventTypesFunction;
  bool                 changedEventTypesFunction;

  // *** Related to event types check list ***
  TEventType           currentType;
  bool                 changedEventTypesSelection;

  // *** Related to selector of and/or operator ***
  int                  previousOperatorTypeValue;
  bool                 changedOperatorTypeValue;

  // *** Related to selector of event values function ***
  int                  previousEventValuesFunction;
  bool                 changedEventValuesFunction;

  // *** Related to event values check list ***
  bool                 changedEventValues;

  void UpdateWidgetChecklistboxTypes();

  void checkAll( wxCheckListBox *boxlist, bool value );
  void GetEventValueLabels( wxArrayString & whichEventValues );
  void UpdateWidgetChecklistboxValues();
  void UpdateChecklistboxValues( TEventType type, bool keepSelected = true );

  bool HasChanged( wxChoice *choice, int selectedFunction ) const;
  bool HasChanged( wxCheckListBox *checkList, wxArrayInt &index ) const;

  bool HasChanged( wxCheckListBox *checkList, EventTypesInfoManager *manager ) const;
  bool HasChanged( wxCheckListBox *checkList, EventValuesInfoManager *manager ) const;

  bool HasChanged( wxCheckListBox *checkList, wxArrayDouble &index ) const;
  bool HasChanged( wxArrayInt &arr1, wxArrayInt &arr2 ) const;
  bool HasChanged( wxArrayDouble &arr1, wxArrayDouble &arr2 ) const;

  bool CopyChanges( wxChoice *choice, int &selectedFunction );
  bool CopyChanges( wxCheckListBox *checkList,
                    wxArrayInt &index,
                    wxArrayString &selected,
                    bool copyStrings = false );
  void InsertValueFromTextCtrl();

  void TransferDataToWindowPreCreateControls( Timeline *whichWindow,
                                              bool whichHideOperatorsList);
  void EnableApplyButton();
  void TransferDataToWindowPostCreateControls();

  void TransferWindowToData();
  unsigned int GetSelections( wxCheckListBox *checkList, wxArrayInt &index ) const;
};
