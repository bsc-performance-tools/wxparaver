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

#ifndef _CUTFILTERDIALOG_H_
#define _CUTFILTERDIALOG_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/filepicker.h"
#include "wx/notebook.h"
#include "wx/statline.h"
#include "wx/spinctrl.h"
////@end includes

using namespace std;
#include <string>
#include <vector>

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxFilePickerCtrl;
class wxNotebook;
class wxSpinCtrl;
////@end forward declarations

#include "traceoptions.h"

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_CUTFILTERDIALOG 10103
#define ID_FILECTRL_CUTFILTER_TRACE_SELECTION 10106
#define ID_CHECKBOX_LOAD_RESULTING_TRACE 10152
#define ID_BUTTON_LOAD_XML 10153
#define ID_BUTTON_SAVE_XML 10154
#define ID_CHECKLISTBOX 10107
#define ID_BITMAPBUTTON_PUSH_UP 10109
#define ID_BITMAPBUTTON_PUSH_DOWN 10110
#define ID_NOTEBOOK_CUT_FILTER_OPTIONS 10108
#define ID_PANEL_CUTTER 10111
#define ID_RADIOBUTTON_CUTTER_CUT_BY_TIME 10116
#define ID_RADIOBUTTON_CUTTER_CUT_BY_PERCENT 10117
#define ID_TEXTCTRL_CUTTER_BEGIN_CUT 10118
#define ID_TEXTCTRL_CUTTER_END_CUT 10119
#define ID_TEXTCTRL_CUTTER_TASKS 10157
#define ID_BUTTON_CUTTER_SELECT_REGION 10114
#define ID_BUTTON_CUTTER_ALL_TRACE 10115
#define ID_CHECKBOX_CHECK_CUTTER_ORIGINAL_TIME 10120
#define ID_CHECKBOX_CUTTER_REMOVE_FIRST_STATE 10121
#define ID_CHECKBOX_CUTTER_BREAK_STATES 10122
#define ID_CHECKBOX_CUTTER_REMOVE_LAST_STATE 10123
#define ID_SPINCTRL_CUTTER_MAXIMUM_SIZE 10147
#define ID_PANEL_FILTER 10112
#define ID_CHECKBOX_FILTER_DISCARD_STATE 10124
#define ID_CHECKBOX_FILTER_DISCARD_EVENT 10125
#define ID_CHECKBOX_FILTER_DISCARD_COMMUNICATION 10126
#define ID_CHECKLISTBOX_FILTER_STATES 10128
#define ID_BUTTON_FILTER_SELECT_ALL 10129
#define ID_BUTTON_FILTER_UNSELECT_ALL 10130
#define ID_TEXTCTRL_FILTER_MIN_BURST_TIME 10151
#define ID_LISTBOX_FILTER_EVENTS 10141
#define ID_BUTTON_FILTER_ADD 10142
#define ID_BUTTON_FILTER_DELETE 10143
#define ID_CHECKBOX_FILTER_DISCARD_LISTED_EVENTS 10155
#define ID_SPINCTRL_FILTER_SIZE 10127
#define ID_PANEL_SOFTWARE_COUNTERS 10113
#define ID_RADIOBUTTON_SC_ON_INTERVALS 10131
#define ID_RADIOBUTTON_SC_ON_STATES 10132
#define ID_TEXTCTRL_SC_SAMPLING_INTERVAL 10133
#define ID_TEXTCTRL_SC_MINIMUM_BURST_TIME 10134
#define ID_CHECKLISTBOX_SC_SELECTED_EVENTS 10148
#define ID_BUTTON_SC_SELECTED_EVENTS_ADD 10149
#define ID_BUTTON_SC_SELECTED_EVENTS_DELETE 10150
#define ID_RADIOBUTTON_SC_COUNT_EVENTS 10135
#define ID_RADIOBUTTON8 10136
#define ID_CHECKBOX_SC_REMOVE_STATES 10137
#define ID_CHECKBOX_SC_SUMMARIZE_USEFUL 10138
#define ID_CHECKBOX_SC_GLOBAL_COUNTERS 10139
#define ID_CHECKBOX_SC_ONLY_IN_BURSTS_COUNTING 10140
#define ID_LISTBOX_SC_KEEP_EVENTS 10144
#define ID_BUTTON_SC_KEEP_EVENTS_ADD 10145
#define ID_BUTTON_SC_KEEP_EVENTS_DELETE 10146
#define SYMBOL_CUTFILTERDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_CUTFILTERDIALOG_TITLE _("Cut & Filter")
#define SYMBOL_CUTFILTERDIALOG_IDNAME ID_CUTFILTERDIALOG
#define SYMBOL_CUTFILTERDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_CUTFILTERDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * CutFilterDialog class declaration
 */

class CutFilterDialog: public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( CutFilterDialog )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  CutFilterDialog();
  CutFilterDialog( wxWindow* parent, wxWindowID id = SYMBOL_CUTFILTERDIALOG_IDNAME, const wxString& caption = SYMBOL_CUTFILTERDIALOG_TITLE, const wxPoint& pos = SYMBOL_CUTFILTERDIALOG_POSITION, const wxSize& size = SYMBOL_CUTFILTERDIALOG_SIZE, long style = SYMBOL_CUTFILTERDIALOG_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_CUTFILTERDIALOG_IDNAME, const wxString& caption = SYMBOL_CUTFILTERDIALOG_TITLE, const wxPoint& pos = SYMBOL_CUTFILTERDIALOG_POSITION, const wxSize& size = SYMBOL_CUTFILTERDIALOG_SIZE, long style = SYMBOL_CUTFILTERDIALOG_STYLE );

  /// Destructor
  ~CutFilterDialog();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin CutFilterDialog event handler declarations

  /// wxEVT_INIT_DIALOG event handler for ID_CUTFILTERDIALOG
  void OnInitDialog( wxInitDialogEvent& event );

  /// wxEVT_IDLE event handler for ID_CUTFILTERDIALOG
  void OnIdle( wxIdleEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_LOAD_XML
  void OnButtonLoadXMLClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_CHECKLISTBOX
  void OnCheckListToolOrderSelected( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_CHECKLISTBOX
  void OnCheckListToolOrderUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON_PUSH_UP
  void OnBitmapbuttonPushUpClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON_PUSH_DOWN
  void OnBitmapbuttonPushDownClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED event handler for ID_NOTEBOOK_CUT_FILTER_OPTIONS
  void OnNotebookCutFilterOptionsPageChanged( wxNotebookEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_NOTEBOOK_CUT_FILTER_OPTIONS
  void OnNotebookCutFilterOptionsUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CUTTER_SELECT_REGION
  void OnButtonCutterSelectRegionClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CUTTER_ALL_TRACE
  void OnButtonCutterAllTraceClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_CHECK_CUTTER_ORIGINAL_TIME
  void OnCheckOriginalTimeClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_PANEL_FILTER
  void OnPanelFilterUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_FILTER_SELECT_ALL
  void OnButtonFilterSelectAllClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_FILTER_UNSELECT_ALL
  void OnButtonFilterUnselectAllClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_FILTER_ADD
  void OnButtonFilterAddClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_FILTER_DELETE
  void OnButtonFilterDeleteClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_PANEL_SOFTWARE_COUNTERS
  void OnPanelSoftwareCountersUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SC_SELECTED_EVENTS_ADD
  void OnButtonScSelectedEventsAddClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SC_SELECTED_EVENTS_DELETE
  void OnButtonScSelectedEventsDeleteClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SC_KEEP_EVENTS_ADD
  void OnButtonScKeepEventsAddClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SC_KEEP_EVENTS_DELETE
  void OnButtonScKeepEventsDeleteClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
  void OnOkClick( wxCommandEvent& event );

////@end CutFilterDialog event handler declarations

////@begin CutFilterDialog member function declarations

  TraceOptions * GetTraceOptions() const { return traceOptions ; }
  void SetTraceOptions(TraceOptions * value) { traceOptions = value ; }

  vector< int > GetFilterToolOrder() const { return filterToolOrder ; }
  void SetFilterToolOrder(vector< int > value) { filterToolOrder = value ; }

  bool GetLoadResultingTrace() const { return loadResultingTrace ; }
  void SetLoadResultingTrace(bool value) { loadResultingTrace = value ; }

  string GetNameSourceTrace() const { return nameSourceTrace ; }
  void SetNameSourceTrace(string value) { nameSourceTrace = value ; }

  string GetPathOutputTrace() const { return pathOutputTrace ; }
  void SetPathOutputTrace(string value) { pathOutputTrace = value ; }

  bool GetWaitingGlobalTiming() const { return waitingGlobalTiming ; }
  void SetWaitingGlobalTiming(bool value) { waitingGlobalTiming = value ; }

  KernelConnection * GetLocalKernel() const { return localKernel ; }
  void SetLocalKernel(KernelConnection * value) { localKernel = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end CutFilterDialog member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  void UpdateToolList();
  wxString formatNumber( double value );

  string GetTraceFileName();
  vector< int > GetToolsOrder();
  bool LoadResultingTrace();

  void ChangePageSelectionFromToolsOrderListToTabs();
  void ChangePageSelectionFromTabsToToolsOrderList();

  void CheckCommonOptions( bool &previousWarning );
  void CheckCutterOptions( bool &previousWarning );
  void CheckFilterOptions( bool &previousWarning );
  void CheckSoftwareCountersOptions( bool &previousWarning );

  bool CheckStringTasks( wxString taskStr );
  bool CheckEventsLine( wxString eventsStr );
  void GetEventsFromLine( wxString eventsStr,
                          TraceOptions::TFilterTypes &eventTypes,
                          int &lastType );
  void SetEventLine( TraceOptions::TFilterTypes eventTypes, int current );
  void SetEventsList( TraceOptions::TFilterTypes types, int lastType ); 
  void GetEventsList( TraceOptions::TFilterTypes &types, int &lastType );

  bool SetSoftwareCountersEventsListToString( string listEvents, wxListBox *selectedEvents );
  char *GetSoftwareCountersEventsListToString( wxListBox *selectedEvents );

  void TransferWindowToCommonData( bool previousWarning );
  void TransferWindowToCutterData( bool previousWarning );
  void TransferWindowToFilterData( bool previousWarning );
  void TransferWindowToSoftwareCountersData( bool previousWarning );

  void TransferCommonDataToWindow( vector<int> order );
  void TransferCutterDataToWindow( TraceOptions *traceOptions );
  void TransferFilterDataToWindow( TraceOptions *traceOptions );
  void TransferSoftwareCountersDataToWindow( TraceOptions *traceOptions );
  void TransferDataToWindow( vector<int> order, TraceOptions *traceOptions );

////@begin CutFilterDialog member variables
  wxFilePickerCtrl* filePickerTrace;
  wxCheckBox* checkLoadResultingTrace;
  wxCheckListBox* checkListToolOrder;
  wxBitmapButton* buttonUp;
  wxBitmapButton* buttonDown;
  wxNotebook* notebookTools;
  wxRadioButton* radioCutterCutByTime;
  wxRadioButton* radioCutterCutByTimePercent;
  wxTextCtrl* textCutterBeginCut;
  wxTextCtrl* textCutterEndCut;
  wxTextCtrl* textCutterTasks;
  wxButton* buttonCutterSelectRegion;
  wxButton* buttonCutterAllTrace;
  wxCheckBox* checkCutterUseOriginalTime;
  wxCheckBox* checkCutterRemoveFirstState;
  wxCheckBox* checkCutterDontBreakStates;
  wxCheckBox* checkCutterRemoveLastState;
  wxSpinCtrl* textCutterMaximumTraceSize;
  wxCheckBox* checkFilterDiscardStateRecords;
  wxCheckBox* checkFilterDiscardEventRecords;
  wxCheckBox* checkFilterDiscardCommunicationRecords;
  wxStaticBox* staticBoxSizerFilterStates;
  wxCheckListBox* checkListFilterStates;
  wxButton* buttonFilterSelectAll;
  wxButton* buttonFilterUnselectAll;
  wxStaticText* labelFilterMinBurstTime;
  wxTextCtrl* textFilterMinBurstTime;
  wxStaticBox* staticBoxSizerFilterEvents;
  wxListBox* listboxFilterEvents;
  wxButton* buttonFilterAdd;
  wxButton* buttonFilterDelete;
  wxCheckBox* checkFilterDiscardListedEvents;
  wxStaticBox* staticBoxSizerFilterCommunications;
  wxStaticText* staticTextFilterSize;
  wxSpinCtrl* textFilterSize;
  wxStaticText* staticTextFilterSizeUnit;
  wxRadioButton* radioSCOnIntervals;
  wxRadioButton* radioSCOnStates;
  wxStaticText* staticTextSCSamplingInterval;
  wxTextCtrl* textSCSamplingInterval;
  wxStaticText* staticTextSCMinimumBurstTime;
  wxTextCtrl* textSCMinimumBurstTime;
  wxListBox* listSCSelectedEvents;
  wxButton* buttonSCSelectedEventsAdd;
  wxButton* buttonSCSelectedEventsDelete;
  wxRadioButton* radioSCCountEvents;
  wxRadioButton* radioSCAccumulateValues;
  wxCheckBox* checkSCRemoveStates;
  wxCheckBox* checkSCSummarizeUseful;
  wxCheckBox* checkSCGlobalCounters;
  wxCheckBox* checkSCOnlyInBurstsCounting;
  wxListBox* listSCKeepEvents;
  wxButton* buttonSCKeepEventsAdd;
  wxButton* buttonSCKeepEventsDelete;
private:
  TraceOptions * traceOptions;
  vector< int > filterToolOrder;
  bool loadResultingTrace;
  string nameSourceTrace;
  string pathOutputTrace;
  bool waitingGlobalTiming;
  KernelConnection * localKernel;
////@end CutFilterDialog member variables

  vector< string > listToolOrder; // Names of the tools
};

#endif
  // _CUTFILTERDIALOG_H_
