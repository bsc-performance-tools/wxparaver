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
#include "wx/imaglist.h"
////@end includes

#include <sstream>
#include <iostream>
#include <algorithm>
#include <wx/filedlg.h>
#include <wx/tokenzr.h>
#include <wx/filename.h>

#include "cutfilterdialog.h"
#include "paraverconfig.h"
#include "kernelconnection.h"
#include "wxparaverapp.h"

////@begin XPM images
#include "arrow_up.xpm"
#include "arrow_down.xpm"
////@end XPM images




/*!
 * CutFilterDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( CutFilterDialog, wxDialog )


/*!
 * CutFilterDialog event table definition
 */

BEGIN_EVENT_TABLE( CutFilterDialog, wxDialog )

////@begin CutFilterDialog event table entries
  EVT_INIT_DIALOG( CutFilterDialog::OnInitDialog )
  EVT_IDLE( CutFilterDialog::OnIdle )

  EVT_BUTTON( ID_BUTTON_LOAD_XML, CutFilterDialog::OnButtonLoadXMLClick )

  EVT_LISTBOX( ID_CHECKLISTBOX, CutFilterDialog::OnCheckListToolOrderSelected )
  EVT_UPDATE_UI( ID_CHECKLISTBOX, CutFilterDialog::OnCheckListToolOrderUpdate )

  EVT_BUTTON( ID_BITMAPBUTTON_PUSH_UP, CutFilterDialog::OnBitmapbuttonPushUpClick )

  EVT_BUTTON( ID_BITMAPBUTTON_PUSH_DOWN, CutFilterDialog::OnBitmapbuttonPushDownClick )

  EVT_NOTEBOOK_PAGE_CHANGED( ID_NOTEBOOK_CUT_FILTER_OPTIONS, CutFilterDialog::OnNotebookCutFilterOptionsPageChanged )
  EVT_UPDATE_UI( ID_NOTEBOOK_CUT_FILTER_OPTIONS, CutFilterDialog::OnNotebookCutFilterOptionsUpdate )

  EVT_BUTTON( ID_BUTTON_CUTTER_SELECT_REGION, CutFilterDialog::OnButtonCutterSelectRegionClick )

  EVT_BUTTON( ID_BUTTON_CUTTER_ALL_TRACE, CutFilterDialog::OnButtonCutterAllTraceClick )

  EVT_CHECKBOX( ID_CHECKBOX_CHECK_CUTTER_ORIGINAL_TIME, CutFilterDialog::OnCheckOriginalTimeClick )

  EVT_UPDATE_UI( ID_PANEL_FILTER, CutFilterDialog::OnPanelFilterUpdate )

  EVT_BUTTON( ID_BUTTON_FILTER_SELECT_ALL, CutFilterDialog::OnButtonFilterSelectAllClick )

  EVT_BUTTON( ID_BUTTON_FILTER_UNSELECT_ALL, CutFilterDialog::OnButtonFilterUnselectAllClick )

  EVT_BUTTON( ID_BUTTON_FILTER_ADD, CutFilterDialog::OnButtonFilterAddClick )

  EVT_BUTTON( ID_BUTTON_FILTER_DELETE, CutFilterDialog::OnButtonFilterDeleteClick )

  EVT_UPDATE_UI( ID_PANEL_SOFTWARE_COUNTERS, CutFilterDialog::OnPanelSoftwareCountersUpdate )

  EVT_BUTTON( ID_BUTTON_SC_SELECTED_EVENTS_ADD, CutFilterDialog::OnButtonScSelectedEventsAddClick )

  EVT_BUTTON( ID_BUTTON_SC_SELECTED_EVENTS_DELETE, CutFilterDialog::OnButtonScSelectedEventsDeleteClick )

  EVT_BUTTON( ID_BUTTON_SC_KEEP_EVENTS_ADD, CutFilterDialog::OnButtonScKeepEventsAddClick )

  EVT_BUTTON( ID_BUTTON_SC_KEEP_EVENTS_DELETE, CutFilterDialog::OnButtonScKeepEventsDeleteClick )

  EVT_BUTTON( wxID_OK, CutFilterDialog::OnOkClick )

////@end CutFilterDialog event table entries

END_EVENT_TABLE()


/*!
 * CutFilterDialog constructors
 */

CutFilterDialog::CutFilterDialog()
{
  Init();
}

CutFilterDialog::CutFilterDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}


/*!
 * CutFilterDialog creator
 */

bool CutFilterDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin CutFilterDialog creation
  SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Centre();
////@end CutFilterDialog creation
  return true;
}


/*!
 * CutFilterDialog destructor
 */

CutFilterDialog::~CutFilterDialog()
{
////@begin CutFilterDialog destruction
////@end CutFilterDialog destruction
}


/*!
 * Member initialisation
 */

void CutFilterDialog::Init()
{
////@begin CutFilterDialog member initialisation
  loadResultingTrace = false;
  nameSourceTrace = "";
  waitingGlobalTiming = false;
  localKernel = NULL;
  filePickerTrace = NULL;
  checkLoadResultingTrace = NULL;
  checkListToolOrder = NULL;
  buttonUp = NULL;
  buttonDown = NULL;
  notebookTools = NULL;
  radioCutterCutByTime = NULL;
  radioCutterCutByTimePercent = NULL;
  textCutterBeginCut = NULL;
  textCutterEndCut = NULL;
  textCutterTasks = NULL;
  buttonCutterSelectRegion = NULL;
  buttonCutterAllTrace = NULL;
  checkCutterUseOriginalTime = NULL;
  checkCutterRemoveFirstState = NULL;
  checkCutterDontBreakStates = NULL;
  checkCutterRemoveLastState = NULL;
  textCutterMaximumTraceSize = NULL;
  checkFilterDiscardStateRecords = NULL;
  checkFilterDiscardEventRecords = NULL;
  checkFilterDiscardCommunicationRecords = NULL;
  staticBoxSizerFilterStates = NULL;
  checkListFilterStates = NULL;
  buttonFilterSelectAll = NULL;
  buttonFilterUnselectAll = NULL;
  labelFilterMinBurstTime = NULL;
  textFilterMinBurstTime = NULL;
  staticBoxSizerFilterEvents = NULL;
  listboxFilterEvents = NULL;
  buttonFilterAdd = NULL;
  buttonFilterDelete = NULL;
  checkFilterDiscardListedEvents = NULL;
  staticBoxSizerFilterCommunications = NULL;
  staticTextFilterSize = NULL;
  textFilterSize = NULL;
  staticTextFilterSizeUnit = NULL;
  radioSCOnIntervals = NULL;
  radioSCOnStates = NULL;
  staticTextSCSamplingInterval = NULL;
  textSCSamplingInterval = NULL;
  staticTextSCMinimumBurstTime = NULL;
  textSCMinimumBurstTime = NULL;
  listSCSelectedEvents = NULL;
  buttonSCSelectedEventsAdd = NULL;
  buttonSCSelectedEventsDelete = NULL;
  radioSCCountEvents = NULL;
  radioSCAccumulateValues = NULL;
  checkSCRemoveStates = NULL;
  checkSCSummarizeUseful = NULL;
  checkSCGlobalCounters = NULL;
  checkSCOnlyInBurstsCounting = NULL;
  listSCKeepEvents = NULL;
  buttonSCKeepEventsAdd = NULL;
  buttonSCKeepEventsDelete = NULL;
////@end CutFilterDialog member initialisation
}


/*!
 * Control creation for CutFilterDialog
 */

void CutFilterDialog::CreateControls()
{    
////@begin CutFilterDialog content construction
  CutFilterDialog* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer3, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer3->Add(itemBoxSizer4, 5, wxALIGN_TOP|wxALL, 3);

  wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Trace"), wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    itemStaticText5->SetToolTip(_("Trace that will be used by the Cut/Filter toolkit."));
  itemBoxSizer4->Add(itemStaticText5, 0, wxALIGN_LEFT|wxALL, 2);

  filePickerTrace = new wxFilePickerCtrl( itemDialog1, ID_FILECTRL_CUTFILTER_TRACE_SELECTION, wxEmptyString, _("Trace to Cut/Filter"), _T("Paraver trace (*.prv;*.prv.gz)|*.prv;*.prv.gz|All files (*.*)|*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_OPEN|wxFLP_FILE_MUST_EXIST|wxFLP_CHANGE_DIR );
  if (CutFilterDialog::ShowToolTips())
    filePickerTrace->SetToolTip(_("Trace that will be used by the Cut/Filter toolkit."));
  itemBoxSizer4->Add(filePickerTrace, 2, wxGROW|wxALL, 2);

  checkLoadResultingTrace = new wxCheckBox( itemDialog1, ID_CHECKBOX_LOAD_RESULTING_TRACE, _("Load the resulting trace"), wxDefaultPosition, wxDefaultSize, 0 );
  checkLoadResultingTrace->SetValue(false);
  itemBoxSizer4->Add(checkLoadResultingTrace, 0, wxGROW|wxALL, 2);

  wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer4->Add(itemBoxSizer8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

  wxButton* itemButton9 = new wxButton( itemDialog1, ID_BUTTON_LOAD_XML, _("Load XML..."), wxDefaultPosition, wxDefaultSize, 0 );
  itemButton9->Enable(false);
  itemBoxSizer8->Add(itemButton9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  wxButton* itemButton10 = new wxButton( itemDialog1, ID_BUTTON_SAVE_XML, _("Save XML..."), wxDefaultPosition, wxDefaultSize, 0 );
  itemButton10->Enable(false);
  itemBoxSizer8->Add(itemButton10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer3->Add(itemBoxSizer11, 3, wxGROW, 5);

  wxStaticText* itemStaticText12 = new wxStaticText( itemDialog1, wxID_STATIC, _("  Ordered execution"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer11->Add(itemStaticText12, 0, wxALIGN_LEFT|wxALL, 2);

  wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer11->Add(itemBoxSizer13, 3, wxGROW|wxALL, 2);

  wxArrayString checkListToolOrderStrings;
  checkListToolOrder = new wxCheckListBox( itemDialog1, ID_CHECKLISTBOX, wxDefaultPosition, wxDefaultSize, checkListToolOrderStrings, wxLB_SINGLE|wxLB_NEEDED_SB );
  if (CutFilterDialog::ShowToolTips())
    checkListToolOrder->SetToolTip(_("Select the order of the Cut/Filter tools."));
  itemBoxSizer13->Add(checkListToolOrder, 2, wxGROW, 2);

  wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer13->Add(itemBoxSizer15, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 2);

  buttonUp = new wxBitmapButton( itemDialog1, ID_BITMAPBUTTON_PUSH_UP, itemDialog1->GetBitmapResource(wxT("arrow_up.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (CutFilterDialog::ShowToolTips())
    buttonUp->SetToolTip(_("Select the order of the Cut/Filter tools."));
  itemBoxSizer15->Add(buttonUp, 1, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 5);

  buttonDown = new wxBitmapButton( itemDialog1, ID_BITMAPBUTTON_PUSH_DOWN, itemDialog1->GetBitmapResource(wxT("arrow_down.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (CutFilterDialog::ShowToolTips())
    buttonDown->SetToolTip(_("Select the order of the Cut/Filter tools."));
  itemBoxSizer15->Add(buttonDown, 1, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 5);

  notebookTools = new wxNotebook( itemDialog1, ID_NOTEBOOK_CUT_FILTER_OPTIONS, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

  wxPanel* itemPanel19 = new wxPanel( notebookTools, ID_PANEL_CUTTER, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxVERTICAL);
  itemPanel19->SetSizer(itemBoxSizer20);

  wxStaticBox* itemStaticBoxSizer21Static = new wxStaticBox(itemPanel19, wxID_STATIC, _(" Trace Limits "));
  wxStaticBoxSizer* itemStaticBoxSizer21 = new wxStaticBoxSizer(itemStaticBoxSizer21Static, wxVERTICAL);
  itemBoxSizer20->Add(itemStaticBoxSizer21, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer21->Add(itemBoxSizer22, 0, wxGROW|wxLEFT|wxTOP, 5);
  wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer22->Add(itemBoxSizer23, 1, wxGROW|wxLEFT|wxTOP, 5);
  radioCutterCutByTime = new wxRadioButton( itemPanel19, ID_RADIOBUTTON_CUTTER_CUT_BY_TIME, _("Cut by time"), wxDefaultPosition, wxDefaultSize, 0 );
  radioCutterCutByTime->SetValue(true);
  itemBoxSizer23->Add(radioCutterCutByTime, 1, wxALIGN_LEFT|wxLEFT|wxTOP, 5);

  radioCutterCutByTimePercent = new wxRadioButton( itemPanel19, ID_RADIOBUTTON_CUTTER_CUT_BY_PERCENT, _("Cut by time %"), wxDefaultPosition, wxDefaultSize, 0 );
  radioCutterCutByTimePercent->SetValue(false);
  itemBoxSizer23->Add(radioCutterCutByTimePercent, 1, wxALIGN_LEFT|wxLEFT|wxTOP, 5);

  wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer22->Add(itemBoxSizer26, 2, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxBoxSizer* itemBoxSizer27 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer26->Add(itemBoxSizer27, 0, wxGROW|wxLEFT|wxTOP, 5);
  wxStaticText* itemStaticText28 = new wxStaticText( itemPanel19, wxID_STATIC, _("Begin"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer27->Add(itemStaticText28, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  textCutterBeginCut = new wxTextCtrl( itemPanel19, ID_TEXTCTRL_CUTTER_BEGIN_CUT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer27->Add(textCutterBeginCut, 3, wxGROW|wxLEFT|wxTOP|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer30 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer26->Add(itemBoxSizer30, 0, wxGROW|wxLEFT|wxTOP, 5);
  wxStaticText* itemStaticText31 = new wxStaticText( itemPanel19, wxID_STATIC, _("End"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer30->Add(itemStaticText31, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  textCutterEndCut = new wxTextCtrl( itemPanel19, ID_TEXTCTRL_CUTTER_END_CUT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer30->Add(textCutterEndCut, 3, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  wxStaticLine* itemStaticLine33 = new wxStaticLine( itemPanel19, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemStaticBoxSizer21->Add(itemStaticLine33, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer34 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer21->Add(itemBoxSizer34, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText35 = new wxStaticText( itemPanel19, wxID_STATIC, _("Tasks"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer34->Add(itemStaticText35, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  textCutterTasks = new wxTextCtrl( itemPanel19, ID_TEXTCTRL_CUTTER_TASKS, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    textCutterTasks->SetToolTip(_("Select the list of tasks to cut, separated by commas. Ranges marked with \"-\" are allowed. I.e. \"1-32,33,64-128\". Leave it empty to select all the tasks."));
  itemBoxSizer34->Add(textCutterTasks, 1, wxGROW|wxALL, 5);

  wxStaticLine* itemStaticLine37 = new wxStaticLine( itemPanel19, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemStaticBoxSizer21->Add(itemStaticLine37, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer38 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer21->Add(itemBoxSizer38, 0, wxGROW|wxALL, 5);
  buttonCutterSelectRegion = new wxButton( itemPanel19, ID_BUTTON_CUTTER_SELECT_REGION, _("Select Region..."), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer38->Add(buttonCutterSelectRegion, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  buttonCutterAllTrace = new wxButton( itemPanel19, ID_BUTTON_CUTTER_ALL_TRACE, _("All Trace"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer38->Add(buttonCutterAllTrace, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  wxStaticBox* itemStaticBoxSizer41Static = new wxStaticBox(itemPanel19, wxID_STATIC, _(" Trace Options "));
  wxStaticBoxSizer* itemStaticBoxSizer41 = new wxStaticBoxSizer(itemStaticBoxSizer41Static, wxVERTICAL);
  itemBoxSizer20->Add(itemStaticBoxSizer41, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer42 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer41->Add(itemBoxSizer42, 0, wxGROW|wxLEFT|wxTOP, 5);
  checkCutterUseOriginalTime = new wxCheckBox( itemPanel19, ID_CHECKBOX_CHECK_CUTTER_ORIGINAL_TIME, _("Use original time"), wxDefaultPosition, wxDefaultSize, 0 );
  checkCutterUseOriginalTime->SetValue(false);
  itemBoxSizer42->Add(checkCutterUseOriginalTime, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  checkCutterRemoveFirstState = new wxCheckBox( itemPanel19, ID_CHECKBOX_CUTTER_REMOVE_FIRST_STATE, _("Remove first state"), wxDefaultPosition, wxDefaultSize, 0 );
  checkCutterRemoveFirstState->SetValue(false);
  itemBoxSizer42->Add(checkCutterRemoveFirstState, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  wxBoxSizer* itemBoxSizer45 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer41->Add(itemBoxSizer45, 0, wxGROW|wxLEFT|wxTOP|wxBOTTOM, 5);
  checkCutterDontBreakStates = new wxCheckBox( itemPanel19, ID_CHECKBOX_CUTTER_BREAK_STATES, _("Don't break states"), wxDefaultPosition, wxDefaultSize, 0 );
  checkCutterDontBreakStates->SetValue(false);
  itemBoxSizer45->Add(checkCutterDontBreakStates, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  checkCutterRemoveLastState = new wxCheckBox( itemPanel19, ID_CHECKBOX_CUTTER_REMOVE_LAST_STATE, _("Remove last state"), wxDefaultPosition, wxDefaultSize, 0 );
  checkCutterRemoveLastState->SetValue(false);
  itemBoxSizer45->Add(checkCutterRemoveLastState, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  wxStaticBox* itemStaticBoxSizer48Static = new wxStaticBox(itemPanel19, wxID_STATIC, _(" Output Trace "));
  wxStaticBoxSizer* itemStaticBoxSizer48 = new wxStaticBoxSizer(itemStaticBoxSizer48Static, wxHORIZONTAL);
  itemBoxSizer20->Add(itemStaticBoxSizer48, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText49 = new wxStaticText( itemPanel19, wxID_STATIC, _("Maximum trace size"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer48->Add(itemStaticText49, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  textCutterMaximumTraceSize = new wxSpinCtrl( itemPanel19, ID_SPINCTRL_CUTTER_MAXIMUM_SIZE, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000000, 0 );
  itemStaticBoxSizer48->Add(textCutterMaximumTraceSize, 3, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticText* itemStaticText51 = new wxStaticText( itemPanel19, wxID_STATIC, _("MB"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer48->Add(itemStaticText51, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  notebookTools->AddPage(itemPanel19, _("Cutter"));

  wxPanel* itemPanel52 = new wxPanel( notebookTools, ID_PANEL_FILTER, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer53 = new wxBoxSizer(wxVERTICAL);
  itemPanel52->SetSizer(itemBoxSizer53);

  wxStaticBox* itemStaticBoxSizer54Static = new wxStaticBox(itemPanel52, wxID_STATIC, _(" Discard Records "));
  wxStaticBoxSizer* itemStaticBoxSizer54 = new wxStaticBoxSizer(itemStaticBoxSizer54Static, wxHORIZONTAL);
  itemBoxSizer53->Add(itemStaticBoxSizer54, 0, wxGROW|wxALL, 3);
  checkFilterDiscardStateRecords = new wxCheckBox( itemPanel52, ID_CHECKBOX_FILTER_DISCARD_STATE, _("State"), wxDefaultPosition, wxDefaultSize, 0 );
  checkFilterDiscardStateRecords->SetValue(false);
  itemStaticBoxSizer54->Add(checkFilterDiscardStateRecords, 0, wxGROW|wxALL, 2);

  checkFilterDiscardEventRecords = new wxCheckBox( itemPanel52, ID_CHECKBOX_FILTER_DISCARD_EVENT, _("Event"), wxDefaultPosition, wxDefaultSize, 0 );
  checkFilterDiscardEventRecords->SetValue(false);
  itemStaticBoxSizer54->Add(checkFilterDiscardEventRecords, 0, wxGROW|wxALL, 2);

  checkFilterDiscardCommunicationRecords = new wxCheckBox( itemPanel52, ID_CHECKBOX_FILTER_DISCARD_COMMUNICATION, _("Communication"), wxDefaultPosition, wxDefaultSize, 0 );
  checkFilterDiscardCommunicationRecords->SetValue(false);
  itemStaticBoxSizer54->Add(checkFilterDiscardCommunicationRecords, 0, wxGROW|wxALL, 2);

  staticBoxSizerFilterStates = new wxStaticBox(itemPanel52, wxID_STATIC, _(" States "));
  wxStaticBoxSizer* itemStaticBoxSizer58 = new wxStaticBoxSizer(staticBoxSizerFilterStates, wxHORIZONTAL);
  itemBoxSizer53->Add(itemStaticBoxSizer58, 1, wxGROW|wxALL, 3);
  wxArrayString checkListFilterStatesStrings;
  checkListFilterStatesStrings.Add(_("Idle"));
  checkListFilterStatesStrings.Add(_("Running"));
  checkListFilterStatesStrings.Add(_("Not created"));
  checkListFilterStatesStrings.Add(_("Waiting a message"));
  checkListFilterStatesStrings.Add(_("Bloking Send"));
  checkListFilterStatesStrings.Add(_("Thd. Synchr."));
  checkListFilterStatesStrings.Add(_("Test/Probe"));
  checkListFilterStatesStrings.Add(_("Sched. and Fork/Join"));
  checkListFilterStatesStrings.Add(_("Wait/WaitAll"));
  checkListFilterStatesStrings.Add(_("Blocked"));
  checkListFilterStatesStrings.Add(_("Inmediate Send"));
  checkListFilterStatesStrings.Add(_("Inmediate Receive"));
  checkListFilterStatesStrings.Add(_("I/O"));
  checkListFilterStatesStrings.Add(_("Group Communication"));
  checkListFilterStatesStrings.Add(_("Tracing Disabled"));
  checkListFilterStatesStrings.Add(_("Others"));
  checkListFilterStatesStrings.Add(_("Send Receive"));
  checkListFilterStates = new wxCheckListBox( itemPanel52, ID_CHECKLISTBOX_FILTER_STATES, wxDefaultPosition, wxSize(-1, 100), checkListFilterStatesStrings, wxLB_SINGLE );
  itemStaticBoxSizer58->Add(checkListFilterStates, 2, wxGROW|wxALL, 2);

  wxBoxSizer* itemBoxSizer60 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer58->Add(itemBoxSizer60, 1, wxGROW|wxALL, 5);
  buttonFilterSelectAll = new wxButton( itemPanel52, ID_BUTTON_FILTER_SELECT_ALL, _("Select all"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer60->Add(buttonFilterSelectAll, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 5);

  buttonFilterUnselectAll = new wxButton( itemPanel52, ID_BUTTON_FILTER_UNSELECT_ALL, _("Unselect all"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer60->Add(buttonFilterUnselectAll, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer63 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer60->Add(itemBoxSizer63, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
  labelFilterMinBurstTime = new wxStaticText( itemPanel52, wxID_STATIC, _("Min. burst time"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer63->Add(labelFilterMinBurstTime, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  textFilterMinBurstTime = new wxTextCtrl( itemPanel52, ID_TEXTCTRL_FILTER_MIN_BURST_TIME, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer63->Add(textFilterMinBurstTime, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  staticBoxSizerFilterEvents = new wxStaticBox(itemPanel52, wxID_STATIC, _(" Events "));
  wxStaticBoxSizer* itemStaticBoxSizer66 = new wxStaticBoxSizer(staticBoxSizerFilterEvents, wxHORIZONTAL);
  itemBoxSizer53->Add(itemStaticBoxSizer66, 1, wxGROW|wxALL, 3);
  wxArrayString listboxFilterEventsStrings;
  listboxFilterEvents = new wxListBox( itemPanel52, ID_LISTBOX_FILTER_EVENTS, wxDefaultPosition, wxDefaultSize, listboxFilterEventsStrings, wxLB_SINGLE );
  itemStaticBoxSizer66->Add(listboxFilterEvents, 2, wxGROW|wxALL, 2);

  wxBoxSizer* itemBoxSizer68 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer66->Add(itemBoxSizer68, 1, wxGROW|wxALL, 5);
  buttonFilterAdd = new wxButton( itemPanel52, ID_BUTTON_FILTER_ADD, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer68->Add(buttonFilterAdd, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 5);

  buttonFilterDelete = new wxButton( itemPanel52, ID_BUTTON_FILTER_DELETE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer68->Add(buttonFilterDelete, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 5);

  checkFilterDiscardListedEvents = new wxCheckBox( itemPanel52, ID_CHECKBOX_FILTER_DISCARD_LISTED_EVENTS, _("Discard"), wxDefaultPosition, wxDefaultSize, 0 );
  checkFilterDiscardListedEvents->SetValue(false);
  itemBoxSizer68->Add(checkFilterDiscardListedEvents, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  staticBoxSizerFilterCommunications = new wxStaticBox(itemPanel52, wxID_STATIC, _(" Communications "));
  wxStaticBoxSizer* itemStaticBoxSizer72 = new wxStaticBoxSizer(staticBoxSizerFilterCommunications, wxHORIZONTAL);
  itemBoxSizer53->Add(itemStaticBoxSizer72, 0, wxGROW|wxALL, 3);
  staticTextFilterSize = new wxStaticText( itemPanel52, wxID_STATIC, _("Size"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer72->Add(staticTextFilterSize, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  textFilterSize = new wxSpinCtrl( itemPanel52, ID_SPINCTRL_FILTER_SIZE, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
  itemStaticBoxSizer72->Add(textFilterSize, 3, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  staticTextFilterSizeUnit = new wxStaticText( itemPanel52, wxID_STATIC, _("MB"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer72->Add(staticTextFilterSizeUnit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  notebookTools->AddPage(itemPanel52, _("Filter"));

  wxPanel* itemPanel76 = new wxPanel( notebookTools, ID_PANEL_SOFTWARE_COUNTERS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer77 = new wxBoxSizer(wxVERTICAL);
  itemPanel76->SetSizer(itemBoxSizer77);

  wxStaticBox* itemStaticBoxSizer78Static = new wxStaticBox(itemPanel76, wxID_STATIC, _(" Region "));
  wxStaticBoxSizer* itemStaticBoxSizer78 = new wxStaticBoxSizer(itemStaticBoxSizer78Static, wxHORIZONTAL);
  itemBoxSizer77->Add(itemStaticBoxSizer78, 0, wxGROW|wxALL, 3);
  wxBoxSizer* itemBoxSizer79 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer78->Add(itemBoxSizer79, 0, wxGROW|wxALL, 0);
  radioSCOnIntervals = new wxRadioButton( itemPanel76, ID_RADIOBUTTON_SC_ON_INTERVALS, _("On intervals"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
  radioSCOnIntervals->SetValue(true);
  itemBoxSizer79->Add(radioSCOnIntervals, 1, wxGROW|wxALL, 2);

  radioSCOnStates = new wxRadioButton( itemPanel76, ID_RADIOBUTTON_SC_ON_STATES, _("On states"), wxDefaultPosition, wxDefaultSize, 0 );
  radioSCOnStates->SetValue(false);
  itemBoxSizer79->Add(radioSCOnStates, 1, wxGROW|wxALL, 2);

  wxStaticLine* itemStaticLine82 = new wxStaticLine( itemPanel76, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
  itemStaticBoxSizer78->Add(itemStaticLine82, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer83 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer78->Add(itemBoxSizer83, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);
  wxBoxSizer* itemBoxSizer84 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer83->Add(itemBoxSizer84, 1, wxGROW|wxALL, 2);
  staticTextSCSamplingInterval = new wxStaticText( itemPanel76, wxID_STATIC, _("Sampling Interval (ns)"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer84->Add(staticTextSCSamplingInterval, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  textSCSamplingInterval = new wxTextCtrl( itemPanel76, ID_TEXTCTRL_SC_SAMPLING_INTERVAL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer84->Add(textSCSamplingInterval, 2, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  wxBoxSizer* itemBoxSizer87 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer83->Add(itemBoxSizer87, 1, wxGROW|wxALL, 2);
  staticTextSCMinimumBurstTime = new wxStaticText( itemPanel76, wxID_STATIC, _("Min Burst Time (ns)"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer87->Add(staticTextSCMinimumBurstTime, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  textSCMinimumBurstTime = new wxTextCtrl( itemPanel76, ID_TEXTCTRL_SC_MINIMUM_BURST_TIME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer87->Add(textSCMinimumBurstTime, 2, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  wxStaticBox* itemStaticBoxSizer90Static = new wxStaticBox(itemPanel76, wxID_STATIC, _(" Selected events "));
  wxStaticBoxSizer* itemStaticBoxSizer90 = new wxStaticBoxSizer(itemStaticBoxSizer90Static, wxHORIZONTAL);
  itemBoxSizer77->Add(itemStaticBoxSizer90, 1, wxGROW|wxALL, 3);
  wxBoxSizer* itemBoxSizer91 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer90->Add(itemBoxSizer91, 1, wxGROW|wxALL, 0);
  wxArrayString listSCSelectedEventsStrings;
  listSCSelectedEvents = new wxListBox( itemPanel76, ID_CHECKLISTBOX_SC_SELECTED_EVENTS, wxDefaultPosition, wxDefaultSize, listSCSelectedEventsStrings, wxLB_SINGLE );
  itemBoxSizer91->Add(listSCSelectedEvents, 2, wxGROW|wxALL, 2);

  wxBoxSizer* itemBoxSizer93 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer91->Add(itemBoxSizer93, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
  buttonSCSelectedEventsAdd = new wxButton( itemPanel76, ID_BUTTON_SC_SELECTED_EVENTS_ADD, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer93->Add(buttonSCSelectedEventsAdd, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonSCSelectedEventsDelete = new wxButton( itemPanel76, ID_BUTTON_SC_SELECTED_EVENTS_DELETE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer93->Add(buttonSCSelectedEventsDelete, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer96Static = new wxStaticBox(itemPanel76, wxID_STATIC, _(" Options "));
  wxStaticBoxSizer* itemStaticBoxSizer96 = new wxStaticBoxSizer(itemStaticBoxSizer96Static, wxHORIZONTAL);
  itemBoxSizer77->Add(itemStaticBoxSizer96, 0, wxGROW|wxALL, 3);
  wxBoxSizer* itemBoxSizer97 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer96->Add(itemBoxSizer97, 1, wxGROW|wxALL, 0);
  wxBoxSizer* itemBoxSizer98 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer97->Add(itemBoxSizer98, 1, wxGROW|wxALL, 2);
  radioSCCountEvents = new wxRadioButton( itemPanel76, ID_RADIOBUTTON_SC_COUNT_EVENTS, _("Count events"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
  radioSCCountEvents->SetValue(false);
  itemBoxSizer98->Add(radioSCCountEvents, 1, wxGROW|wxALL, 2);

  radioSCAccumulateValues = new wxRadioButton( itemPanel76, ID_RADIOBUTTON8, _("Accumulate values"), wxDefaultPosition, wxDefaultSize, 0 );
  radioSCAccumulateValues->SetValue(false);
  itemBoxSizer98->Add(radioSCAccumulateValues, 1, wxGROW|wxALL, 2);

  wxStaticLine* itemStaticLine101 = new wxStaticLine( itemPanel76, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
  itemBoxSizer97->Add(itemStaticLine101, 0, wxGROW|wxALL, 5);

  wxGridSizer* itemGridSizer102 = new wxGridSizer(2, 2, 0, 0);
  itemBoxSizer97->Add(itemGridSizer102, 2, wxGROW|wxALL, 0);
  checkSCRemoveStates = new wxCheckBox( itemPanel76, ID_CHECKBOX_SC_REMOVE_STATES, _("Remove states"), wxDefaultPosition, wxDefaultSize, 0 );
  checkSCRemoveStates->SetValue(false);
  itemGridSizer102->Add(checkSCRemoveStates, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 2);

  checkSCSummarizeUseful = new wxCheckBox( itemPanel76, ID_CHECKBOX_SC_SUMMARIZE_USEFUL, _("Summarize useful"), wxDefaultPosition, wxDefaultSize, 0 );
  checkSCSummarizeUseful->SetValue(false);
  itemGridSizer102->Add(checkSCSummarizeUseful, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 2);

  checkSCGlobalCounters = new wxCheckBox( itemPanel76, ID_CHECKBOX_SC_GLOBAL_COUNTERS, _("Global counters"), wxDefaultPosition, wxDefaultSize, 0 );
  checkSCGlobalCounters->SetValue(false);
  itemGridSizer102->Add(checkSCGlobalCounters, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 2);

  checkSCOnlyInBurstsCounting = new wxCheckBox( itemPanel76, ID_CHECKBOX_SC_ONLY_IN_BURSTS_COUNTING, _("Only in bursts counting"), wxDefaultPosition, wxDefaultSize, 0 );
  checkSCOnlyInBurstsCounting->SetValue(false);
  itemGridSizer102->Add(checkSCOnlyInBurstsCounting, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 2);

  wxStaticBox* itemStaticBoxSizer107Static = new wxStaticBox(itemPanel76, wxID_STATIC, _(" Keep events "));
  wxStaticBoxSizer* itemStaticBoxSizer107 = new wxStaticBoxSizer(itemStaticBoxSizer107Static, wxHORIZONTAL);
  itemBoxSizer77->Add(itemStaticBoxSizer107, 1, wxGROW|wxALL, 3);
  wxArrayString listSCKeepEventsStrings;
  listSCKeepEvents = new wxListBox( itemPanel76, ID_LISTBOX_SC_KEEP_EVENTS, wxDefaultPosition, wxDefaultSize, listSCKeepEventsStrings, wxLB_SINGLE );
  itemStaticBoxSizer107->Add(listSCKeepEvents, 2, wxGROW|wxALL, 2);

  wxBoxSizer* itemBoxSizer109 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer107->Add(itemBoxSizer109, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
  buttonSCKeepEventsAdd = new wxButton( itemPanel76, ID_BUTTON_SC_KEEP_EVENTS_ADD, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer109->Add(buttonSCKeepEventsAdd, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonSCKeepEventsDelete = new wxButton( itemPanel76, ID_BUTTON_SC_KEEP_EVENTS_DELETE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer109->Add(buttonSCKeepEventsDelete, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  notebookTools->AddPage(itemPanel76, _("Software Counters"));

  itemBoxSizer2->Add(notebookTools, 4, wxGROW|wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer112 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer112, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);
  wxButton* itemButton113 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer112->AddButton(itemButton113);

  wxButton* itemButton114 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer112->AddButton(itemButton114);

  itemStdDialogButtonSizer112->Realize();

////@end CutFilterDialog content construction

  // Other initializations

  // Fill vector with tool names; sort here as you want them to appear in the tools list widget.
  listToolOrder.push_back( string("Cutter") );
  listToolOrder.push_back( string("Filter") );
  listToolOrder.push_back( string("Software Counters") );

  UpdateToolList();

  filePickerTrace->SetName( _( "Load Trace" ) );
  //filePickerTrace->SetPath( wxString::FromAscii( paraverConfig->getGlobalTracesPath().c_str() ) );

  // Allow only numeric character for text boxes.
  textCutterBeginCut->SetValidator( wxTextValidator( wxFILTER_NUMERIC ));
  textCutterEndCut->SetValidator( wxTextValidator( wxFILTER_NUMERIC ));
  textCutterTasks->SetValidator( wxTextValidator( wxFILTER_NUMERIC ));
  textFilterMinBurstTime->SetValidator( wxTextValidator( wxFILTER_NUMERIC ));
  textSCSamplingInterval->SetValidator( wxTextValidator( wxFILTER_NUMERIC ));
  textSCMinimumBurstTime->SetValidator( wxTextValidator( wxFILTER_NUMERIC ));
}


/*!
 * Should we show tooltips?
 */

bool CutFilterDialog::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap CutFilterDialog::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin CutFilterDialog bitmap retrieval
  wxUnusedVar(name);
  if (name == _T("arrow_up.xpm"))
  {
    wxBitmap bitmap(arrow_up_xpm);
    return bitmap;
  }
  else if (name == _T("arrow_down.xpm"))
  {
    wxBitmap bitmap(arrow_down_xpm);
    return bitmap;
  }
  return wxNullBitmap;
////@end CutFilterDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon CutFilterDialog::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin CutFilterDialog icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end CutFilterDialog icon retrieval
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_CHECK_ORIGINAL_TIME
 */

void CutFilterDialog::OnCheckOriginalTimeClick( wxCommandEvent& event )
{
  if ( checkCutterUseOriginalTime->IsChecked() )
  {
    checkCutterDontBreakStates->SetValue( false );
    checkCutterDontBreakStates->Disable();
  }
  else
  {
    checkCutterDontBreakStates->Enable();
  }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON_PUSH_UP
 */

void CutFilterDialog::OnBitmapbuttonPushUpClick( wxCommandEvent& event )
{
  int lastItemSelected = checkListToolOrder->GetSelection();

  if ( lastItemSelected != wxNOT_FOUND && lastItemSelected > 0 )
  {
    // Save current check state, because UpdateToolList clears it
    vector< bool > checked;
    for( unsigned int i = 0; i < listToolOrder.size(); ++i )
      checked.push_back( checkListToolOrder->IsChecked( i ) );

    // Get check state of items to swap
    bool auxFirst  = checkListToolOrder->IsChecked( lastItemSelected - 1 );
    bool auxSecond = checkListToolOrder->IsChecked( lastItemSelected );

    // Swap the names in the string
    string auxNameFirst = listToolOrder[ lastItemSelected - 1 ];
    listToolOrder[ lastItemSelected - 1 ] =  listToolOrder[ lastItemSelected ];
    listToolOrder[ lastItemSelected ] = auxNameFirst;

    // Rebuild list
    UpdateToolList();

    // Recover and change check state
    for( unsigned int i = 0; i < listToolOrder.size(); ++i )
      checkListToolOrder->Check( i, checked[ i ] );

    checkListToolOrder->Check( lastItemSelected - 1, auxSecond );
    checkListToolOrder->Check( lastItemSelected, auxFirst );

    // Keep the selection
    checkListToolOrder->SetSelection( --lastItemSelected );
  }
}


void CutFilterDialog::UpdateToolList()
{
  wxArrayString items;
  int order = 1;
  for( vector< string >::iterator it = listToolOrder.begin(); it != listToolOrder.end(); ++it )
  {
    stringstream aux;
    aux << order++;
    items.Add(  wxString::FromAscii( aux.str().c_str() ) + _( ".- " ) + wxString::FromAscii( (*it).c_str() ) );
  }

  checkListToolOrder->Clear();
  checkListToolOrder->InsertItems( items, 0 );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON_PUSH_DOWN
 */

void CutFilterDialog::OnBitmapbuttonPushDownClick( wxCommandEvent& event )
{
  int lastItemSelected = checkListToolOrder->GetSelection();

  if ( lastItemSelected != wxNOT_FOUND && lastItemSelected < 2 )
  {
    // Save current check state, because UpdateToolList clears it
    vector< bool > checked;
    for( unsigned int i = 0; i < listToolOrder.size(); ++i )
      checked.push_back( checkListToolOrder->IsChecked( i ) );

    // Get check state of items to swap
    bool auxFirst  = checkListToolOrder->IsChecked( lastItemSelected );
    bool auxSecond = checkListToolOrder->IsChecked( lastItemSelected + 1 );

    // Swap the names in the string
    string auxNameFirst = listToolOrder[ lastItemSelected ];
    listToolOrder[ lastItemSelected ] =  listToolOrder[ lastItemSelected + 1 ];
    listToolOrder[ lastItemSelected + 1 ] = auxNameFirst;

    // Rebuild list
    UpdateToolList();

    // Recover and change check state
    for( unsigned int i = 0; i < listToolOrder.size(); ++i )
      checkListToolOrder->Check( i, checked[ i ] );

    checkListToolOrder->Check( lastItemSelected, auxSecond );
    checkListToolOrder->Check( lastItemSelected + 1, auxFirst );

    // Keep the selection
    checkListToolOrder->SetSelection( ++lastItemSelected );
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHECKLISTBOX
 */

void CutFilterDialog::OnCheckListToolOrderUpdate( wxUpdateUIEvent& event )
{
  int pos = 0;

  for( vector< string >::iterator it = listToolOrder.begin(); it != listToolOrder.end(); ++it )
  {
    for( size_t i = 0; i < notebookTools->GetPageCount(); ++i )
    {
      if ( *it == string( notebookTools->GetPageText( i ).mb_str()))
      {
        (notebookTools->GetPage( i ))->Enable( checkListToolOrder->IsChecked( pos ) );
      }
    }

    pos++;
  }
}



// This code is cut/paste from Histogram!!!!
wxString CutFilterDialog::formatNumber( double value )
{
  stringstream auxSStr;
  wxString auxNumber;

  locale mylocale( "" );
  auxSStr.imbue( mylocale );
  auxSStr.precision( ParaverConfig::getInstance()->getHistogramPrecision() );
  auxSStr << fixed;
  auxSStr << value;
  auxNumber << wxString::FromAscii( auxSStr.str().c_str() );

  return auxNumber;
}



/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ALL_TRACE
 */

void CutFilterDialog::OnButtonCutterAllTraceClick( wxCommandEvent& event )
{
  radioCutterCutByTimePercent->SetValue( true );
  textCutterBeginCut->SetValue( formatNumber( 0 ));
  textCutterEndCut->SetValue( formatNumber( 100 ));
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_PANEL_SOFTWARE_COUNTERS
 */

void CutFilterDialog::OnPanelSoftwareCountersUpdate( wxUpdateUIEvent& event )
{
  staticTextSCSamplingInterval->Enable( radioSCOnIntervals->GetValue() );
  textSCSamplingInterval->Enable( radioSCOnIntervals->GetValue() );

  staticTextSCMinimumBurstTime->Enable( !radioSCOnIntervals->GetValue() );
  textSCMinimumBurstTime->Enable( !radioSCOnIntervals->GetValue() );

  checkSCRemoveStates->Enable( radioSCOnIntervals->GetValue() );
  checkSCRemoveStates->SetValue( radioSCOnIntervals->GetValue() && checkSCRemoveStates->IsChecked() );

  checkSCSummarizeUseful->Enable( radioSCOnIntervals->GetValue() );
  checkSCSummarizeUseful->SetValue( radioSCOnIntervals->GetValue() && checkSCSummarizeUseful->IsChecked() );

  checkSCGlobalCounters->Enable( !radioSCAccumulateValues->GetValue() );
  checkSCGlobalCounters->SetValue( !radioSCAccumulateValues->GetValue() && checkSCGlobalCounters->IsChecked() );

  checkSCOnlyInBurstsCounting->Enable( radioSCOnIntervals->GetValue() );
  checkSCOnlyInBurstsCounting->SetValue( radioSCOnIntervals->GetValue() && checkSCOnlyInBurstsCounting->IsChecked() );
}


void CutFilterDialog::ChangePageSelectionFromToolsOrderListToTabs()
{
  int lastItemSelected = checkListToolOrder->GetSelection();

  if ( lastItemSelected != wxNOT_FOUND )
  {
    for( size_t i = 0; i < notebookTools->GetPageCount(); ++i )
    {
      if ( listToolOrder[ lastItemSelected ] == string( notebookTools->GetPageText( i ).mb_str()))
        notebookTools->ChangeSelection( i );
    }
  }
}


void CutFilterDialog::ChangePageSelectionFromTabsToToolsOrderList()
{
  int pos = 0;

  for( vector< string >::iterator it = listToolOrder.begin(); it != listToolOrder.end(); ++it )
  {
    if ( *it == string( notebookTools->GetPageText( notebookTools->GetSelection() ).mb_str()) )
    {
      checkListToolOrder->SetSelection( pos );
    }
    pos++;
  }
}

/*!
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_CHECKLISTBOX
 */

void CutFilterDialog::OnCheckListToolOrderSelected( wxCommandEvent& event )
{
  ChangePageSelectionFromToolsOrderListToTabs();
}


/*!
 * wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED event handler for ID_NOTEBOOK_CUT_FILTER_OPTIONS
 */

void CutFilterDialog::OnNotebookCutFilterOptionsPageChanged( wxNotebookEvent& event )
{
  ChangePageSelectionFromTabsToToolsOrderList();
}


void CutFilterDialog::TransferWindowToCommonData( bool previousWarning )
{
  if ( !previousWarning )
  {
    wxString fullPath = filePickerTrace->GetPath();  // includes path + name
    wxString path = fullPath.BeforeLast('/'); // only path

    nameSourceTrace = std::string( fullPath.mb_str() );
    pathOutputTrace = std::string( path.mb_str() );

    loadResultingTrace = checkLoadResultingTrace->IsChecked();

    for ( size_t i = 0; i < listToolOrder.size(); ++i )
    {
      if ( checkListToolOrder->IsChecked( i ) )
      {
        if ( listToolOrder[ i ] == "Cutter" )
          filterToolOrder.push_back( INC_CHOP_COUNTER );
        if ( listToolOrder[ i ] == "Filter" )
          filterToolOrder.push_back( INC_FILTER_COUNTER );
        if ( listToolOrder[ i ] == "Software Counters" )
          filterToolOrder.push_back( INC_SC_COUNTER );
      }
    }
  }
}


void CutFilterDialog::TransferWindowToCutterData( bool previousWarning )
{
  unsigned long long auxULong;

  if ( !previousWarning )
  {
    traceOptions->set_max_trace_size( textCutterMaximumTraceSize->GetValue() );

    traceOptions->set_by_time( radioCutterCutByTime->GetValue() );

    textCutterBeginCut->GetValue().ToULongLong( &auxULong );
    traceOptions->set_min_cutting_time( (unsigned long long)auxULong );
    traceOptions->set_minimum_time_percentage( (unsigned long long)auxULong );

    textCutterEndCut->GetValue().ToULongLong( &auxULong );
    traceOptions->set_max_cutting_time( (unsigned long long)auxULong );
    traceOptions->set_maximum_time_percentage( (unsigned long long)auxULong );

    traceOptions->set_original_time( (char)checkCutterUseOriginalTime->IsChecked() );
    traceOptions->set_break_states( (int)!checkCutterDontBreakStates->IsChecked() );
    traceOptions->set_remFirstStates( (int)checkCutterRemoveFirstState->IsChecked() );
    traceOptions->set_remLastStates( (int)checkCutterRemoveLastState->IsChecked() );

    traceOptions->set_tasks_list( (char *)textCutterTasks->GetValue().c_str() );
  }
}

bool CutFilterDialog::CheckEventsLine( wxString eventsStr )
{
  if( eventsStr == _( "" ) )
    return false;

  stringstream sstr( string( eventsStr.mb_str() ) );

  while( !sstr.eof() )
  {
    string tmpStr;
    long tmpLong;

    std::getline( sstr, tmpStr, ':' );

    stringstream tmpStream( tmpStr );
    std::getline( tmpStream, tmpStr, '-' );

    if( !( stringstream( tmpStr ) >> tmpLong ) )
      return false;
    else if( tmpLong == 0 )
      return false;

    if( !tmpStream.eof() )
    {
      std::getline( tmpStream, tmpStr );
      if( !( stringstream( tmpStr ) >> tmpLong ) )
        return false;
      else if( tmpLong == 0 )
        return false;
    }

    while( !sstr.eof() )
    {
      std::getline( sstr, tmpStr, ',' );

      if( !( stringstream( tmpStr ) >> tmpLong ) )
        return false;
    }
  }

  return true;
}


void CutFilterDialog::GetEventsFromLine( wxString eventsStr,
                                         TraceOptions::TFilterTypes &eventTypes,
                                         int &lastType )
{
  if( eventsStr == _( "" ) )
    return;

  stringstream sstr( string( eventsStr.mb_str() ) );

  while( !sstr.eof() )
  {
    string tmpStr;
    long tmpLong;

    std::getline( sstr, tmpStr, ':' );

    stringstream tmpStream( tmpStr );
    std::getline( tmpStream, tmpStr, '-' );

    if( !( stringstream( tmpStr ) >> tmpLong ) )
      return;
    else if( tmpLong == 0 )
      return;
    else
    {
      eventTypes[ lastType ].type = tmpLong;
      eventTypes[ lastType ].max_type = 0;
    }

    if( !tmpStream.eof() )
    {
      std::getline( tmpStream, tmpStr );
      if( !( stringstream( tmpStr ) >> tmpLong ) )
        return;
      else if( tmpLong == 0 )
        return;
      else
      {
        eventTypes[ lastType ].max_type = tmpLong;
      }
    }

    int index = 0;
    while( !sstr.eof() )
    {
      std::getline( sstr, tmpStr, ',' );

      if( !( stringstream( tmpStr ) >> tmpLong ) )
        return;
      else
      {
        eventTypes[ lastType ].value[index++] = tmpLong;
      }
    }
    eventTypes[ lastType ].last_value = index;

    ++lastType;
  }
}


void CutFilterDialog::GetEventsList( TraceOptions::TFilterTypes &eventTypes, int &lastType )
{
  for( size_t i = 0; i < listboxFilterEvents->GetCount(); ++i )
  {
    if ( CheckEventsLine( listboxFilterEvents->GetString( i ) ) )
    {
      GetEventsFromLine( listboxFilterEvents->GetString( i ), eventTypes, lastType );
    }
  }
}


void CutFilterDialog::SetEventLine( TraceOptions::TFilterTypes eventTypes, int current )
{
  stringstream auxLine;

  auxLine << eventTypes[ current ].type;
  if ( eventTypes[ current ].max_type != 0 )
  {
    auxLine << "-" << eventTypes[ current ].max_type;
  }
  else
  {
    if ( eventTypes[ current ].last_value != 0 )
    {
      auxLine << ":";
      for( int j = 0; j < eventTypes[ current ].last_value; ++j )
      {
        auxLine << eventTypes[ current ].value[ j ];
        if ( j < eventTypes[ current ].last_value - 1 )
        {
          auxLine << ",";
        }
      }
    }
  }

  listboxFilterEvents->Append( wxString( auxLine.str().c_str()) );
}


void CutFilterDialog::SetEventsList( TraceOptions::TFilterTypes eventTypes, int lastType )
{
  listboxFilterEvents->Clear();

  for( int i = 0; i < lastType; ++i )
  {
    SetEventLine( eventTypes, i );
  }
}


void CutFilterDialog::TransferWindowToFilterData( bool previousWarning )
{
  if ( !previousWarning )
  {
    // Discard Records
    traceOptions->set_filter_states( (char)!checkFilterDiscardStateRecords->IsChecked() );
    traceOptions->set_filter_events( (char)!checkFilterDiscardEventRecords->IsChecked() );
    traceOptions->set_filter_comms( (char)!checkFilterDiscardCommunicationRecords->IsChecked() );

    // filter_by_call_time is for all the record types, BUT:
    // 1) it uses a buffer (danger!)
    // 2) it's undocumented 
    // 3) if false => normal behaviour
    // Was it an experimental feature?
    traceOptions->set_filter_by_call_time( (char)false );

    if ( !checkFilterDiscardStateRecords->IsChecked() )
    {
      bool allStatesSelected = true;
      for( size_t i = 0; i < checkListFilterStates->GetCount(); ++i )
      {
        if ( !checkListFilterStates->IsChecked( i ) )
        {
          allStatesSelected = false;
          break;
        }
      }

      TraceOptions::TStateNames auxNames;
      for( int i = 0; i < 20; ++i )
        auxNames[ i ] = NULL;
        
      traceOptions->set_all_states( (char)allStatesSelected );
      if ( allStatesSelected )
      {
        auxNames[ 0 ] = strdup( "All" );
      }
      else
      {
        // Read selected states and fill vector
        int pos = 0;

        for( size_t i = 0; i < checkListFilterStates->GetCount(); ++i )
        {
          if ( checkListFilterStates->IsChecked( i ) )
          {
            auxNames[ pos++ ] = strdup( (char *)checkListFilterStates->GetString( i ).c_str());
          }
        }

        if( pos == 0 )
        {
          auxNames[ 0 ] = strdup( "Running" );
        }

        traceOptions->set_state_names( auxNames );

        unsigned long auxULong;
        textFilterMinBurstTime->GetValue().ToULong( &auxULong );
        traceOptions->set_min_state_time( (unsigned long long)auxULong );
      }
    }

    if ( !checkFilterDiscardEventRecords->IsChecked() )
    {
      traceOptions->set_discard_given_types( (char)checkFilterDiscardListedEvents->IsChecked() );
      TraceOptions::TFilterTypes auxEvents;
      int lastType = 0;
      GetEventsList( auxEvents, lastType );

      // To fix: always do set_filter_last_type first than set_filter_types
      traceOptions->set_filter_last_type( lastType );
      traceOptions->set_filter_types( auxEvents );
    }

    if ( !checkFilterDiscardCommunicationRecords->IsChecked() )
    {
      traceOptions->set_min_comm_size( textFilterSize->GetValue() );
    }
  }
}


bool CutFilterDialog::SetSoftwareCountersEventsListToString( string listEvents, wxListBox *selectedEvents )
{
  selectedEvents->Clear();
  stringstream auxList( listEvents );
  while( !auxList.eof() )
  {
    string tmpStr;

    std::getline( auxList, tmpStr, ';' );
    selectedEvents->Append( wxString( tmpStr.c_str(), wxConvUTF8 ) );
  }

  return true;
}


char *CutFilterDialog::GetSoftwareCountersEventsListToString( wxListBox *selectedEvents )
{
  string listStr;

  for( size_t i = 0; i < selectedEvents->GetCount(); ++i )
  {
    string lineStr = string( selectedEvents->GetString( i ).mb_str() );
    listStr += lineStr;
    if ( i != selectedEvents->GetCount() - 1 )
      listStr += string(";");
  }

  return strdup( listStr.c_str() );
}


void CutFilterDialog::TransferWindowToSoftwareCountersData( bool previousWarning )
{
  unsigned long auxULong;

  if ( !previousWarning )
  {
    // Region
    traceOptions->set_sc_onInterval( (int) radioSCOnIntervals->GetValue() );

    traceOptions->set_sc_sampling_interval( (unsigned long long)textSCSamplingInterval->GetValue().ToULong( &auxULong ) );
    traceOptions->set_sc_minimum_burst_time( (unsigned long long)textSCMinimumBurstTime->GetValue().ToULong( &auxULong ) );

    // Selected events
    traceOptions->set_sc_types( GetSoftwareCountersEventsListToString( listSCSelectedEvents ) );

    // Options
    traceOptions->set_sc_acumm_counters( (int) radioSCAccumulateValues->GetValue() );

    traceOptions->set_sc_remove_states( (int) checkSCRemoveStates->IsChecked() );
    traceOptions->set_sc_summarize_states( (int) checkSCSummarizeUseful->IsChecked() );
    traceOptions->set_sc_global_counters( (int) checkSCGlobalCounters->IsChecked() );
    traceOptions->set_sc_only_in_bursts( (int) checkSCOnlyInBurstsCounting->IsChecked() );

    // Keep events
    traceOptions->set_sc_types_kept( GetSoftwareCountersEventsListToString( listSCKeepEvents ));

    // Experimental feature?
    // traceOptions->set_sc_frequency( (int) scFrequency );
  }
}


void CutFilterDialog::CheckCommonOptions( bool &previousWarning )
{
  // Any trace selected?
  wxString path = filePickerTrace->GetPath();
  wxFileName tmpName( path );

  if ( !previousWarning && ( path == _("") || tmpName.IsDir() ))
  {
    wxMessageDialog message( this, _("Missing trace name.\nPlease choose one trace."), _("Warning"), wxOK );
    message.ShowModal();
    filePickerTrace->SetFocus();
    previousWarning = true;
  }

  // Any tool selected?
  bool someUtilitySelected = false;
  for (size_t i = 0; i < checkListToolOrder->GetCount(); ++i )
  {
    if ( checkListToolOrder->IsChecked( i ) )
    {
      someUtilitySelected = true;
      break;
    }
  }

  if ( !previousWarning && !someUtilitySelected )
  {
    wxMessageDialog message( this, _("No utility selected.\nPlease choose the utilities to apply."), _( "Warning" ), wxOK );
    message.ShowModal();
    checkListToolOrder->SetFocus();
    previousWarning = true;
  }
}


void CutFilterDialog::CheckCutterOptions( bool &previousWarning )
{
  // Time region selected?
  if ( !previousWarning && textCutterBeginCut->GetValue() == _("") )
  {
    wxMessageDialog message( this, _("Please set the initial time."), _( "Warning" ), wxOK );
    message.ShowModal();
    textCutterBeginCut->SetFocus();
    previousWarning = true;
  }

  if ( !previousWarning && textCutterEndCut->GetValue() == _("") )
  {
    wxMessageDialog message( this, _("Please set the final time."), _("Warning"), wxOK );
    message.ShowModal();
    textCutterEndCut->SetFocus();
    previousWarning = true;
  }

  // Are time set properly?
  double cutterBeginTime, cutterEndTime;
  textCutterBeginCut->GetValue().ToDouble( &cutterBeginTime );
  textCutterEndCut->GetValue().ToDouble( &cutterEndTime );

  // negative times?
  if ( !previousWarning && cutterBeginTime < 0.0 )
  {
    wxMessageDialog message( this, _("Times must be positive numbers.\n\nPlease set begin time properly."), _("Warning"), wxOK );
    message.ShowModal();
    textCutterBeginCut->SetFocus();
    previousWarning = true;
  }

  if ( !previousWarning && cutterEndTime < 0.0 )
  {
    wxMessageDialog message( this, _("Times must be positive numbers.\n\nPlease set end time properly."), _("Warning"), wxOK );
    message.ShowModal();
    textCutterEndCut->SetFocus();
    previousWarning = true;
  }

  // percent out of range?
  if ( !previousWarning && radioCutterCutByTimePercent->GetValue() && cutterBeginTime > 100.0 )
  {
    wxMessageDialog message( this, _("Begin time percent greater than 100 %.\n\nPlease set time percent properly."), _("Warning"), wxOK );
    message.ShowModal();
    radioCutterCutByTimePercent->SetFocus();
    previousWarning = true;
  }

  if ( !previousWarning && radioCutterCutByTimePercent->GetValue() && cutterEndTime > 100.0 )
  {
    wxMessageDialog message( this, _("End time percent greater than 100 %.\n\nPlease set time percent properly."), _("Warning"), wxOK );
    message.ShowModal();
    radioCutterCutByTimePercent->SetFocus();
    previousWarning = true;
  }

  // begin == end?
  if ( !previousWarning && cutterBeginTime == cutterEndTime )
  {
    wxMessageDialog message( this, _("Same time for both limits.\n\nPlease set time range properly."), _("Warning"), wxOK );
    message.ShowModal();
    textCutterBeginCut->SetFocus();
    previousWarning = true;
  }

  // begin > end?
  if ( !previousWarning && cutterBeginTime > cutterEndTime ) // Idea: Maybe it could swap times in text boxes.
  {
    wxMessageDialog message( this, _("Begin time greater than end time.\n\nPlease set time range properly."), _("Warning"), wxOK );
    message.ShowModal();
    textCutterBeginCut->SetFocus();
    previousWarning = true;
  }
  
  // tasks string
  if( !previousWarning && !CheckStringTasks( textCutterTasks->GetValue() ) )
  {
    wxMessageDialog message( this, _("Not allowed format in tasks text.\n\nPlease set it properly."), _("Warning"), wxOK );
    message.ShowModal();
    textCutterTasks->SetFocus();
    previousWarning = true;
  }
}


void CutFilterDialog::CheckFilterOptions( bool &previousWarning )
{
  // No state selected?
  if ( !previousWarning && !checkFilterDiscardStateRecords->IsChecked() )
  {
    bool statesSelected = false;
    for (size_t i = 0; i < checkListFilterStates->GetCount(); ++i )
    {
      if ( checkListFilterStates->IsChecked( i ) )
      {
        statesSelected = true;
        break;
      }
    }

    if ( !statesSelected )
    {
      wxMessageDialog message( this, _("No state selected.\n\nPlease select at least one or\ndiscard all the state records."), _( "Warning" ), wxOK );
      message.ShowModal();
      checkListFilterStates->SetFocus();
      previousWarning = true;
    }
  }

  // Min. burst time < 0
  double filterMinBurstTime;
  textFilterMinBurstTime->GetValue().ToDouble( &filterMinBurstTime );
  if ( !previousWarning && !checkFilterDiscardStateRecords->IsChecked() && filterMinBurstTime < 0.0 )
  {
    wxMessageDialog message( this, _("Times must be positive numbers.\n\nPlease set minimum burst time properly."), _("Warning"), wxOK );
    message.ShowModal();
    textFilterMinBurstTime->SetFocus();
    previousWarning = true;
  }

  // No events selected
  if ( !previousWarning && !checkFilterDiscardEventRecords->IsChecked() && listboxFilterEvents->GetCount() == 0 )
  {
    wxMessageDialog message( this, _("The list of events is empty.\n\nPlease add at least one event or\ndiscard all the event records."), _( "Warning" ), wxOK );
    message.ShowModal();
    buttonFilterAdd->SetFocus();
    previousWarning = true;
  }

  // Too much event lines
  if ( !previousWarning && !checkFilterDiscardEventRecords->IsChecked() && listboxFilterEvents->GetCount() > 20 )
  {
    wxMessageDialog message( this, _("Too much event lines.\n\nPlease delete at least one line."), _( "Warning" ), wxOK );
    message.ShowModal();
    listboxFilterEvents->SetFocus();
    previousWarning = true;
  }
}


void CutFilterDialog::CheckSoftwareCountersOptions( bool &previousWarning )
{
  // Time region selected?
  if ( !previousWarning && radioSCOnIntervals->GetValue() && textSCSamplingInterval->GetValue() == _("") )
  {
    wxMessageDialog message( this, _("Please set the sampling interval time."), _( "Warning" ), wxOK );
    message.ShowModal();
    textSCSamplingInterval->SetFocus();
    previousWarning = true;
  }

  if ( !previousWarning && radioSCOnStates->GetValue() && textSCMinimumBurstTime->GetValue() == _("")  )
  {
    wxMessageDialog message( this, _("Please set the minimum burst time."), _("Warning"), wxOK );
    message.ShowModal();
    textSCMinimumBurstTime->SetFocus();
    previousWarning = true;
  }

  // Are time set properly?
  double regionTime;
  if ( !previousWarning && radioSCOnIntervals->GetValue() )
    textSCSamplingInterval->GetValue().ToDouble( &regionTime );
  else
    textSCMinimumBurstTime->GetValue().ToDouble( &regionTime );


  // sampling interval <0  or Min. burst time < 0
  if ( !previousWarning && regionTime < 0.0 )
  {
    if ( radioSCOnIntervals->GetValue() )
    {
      wxMessageDialog message( this, _("Times must be positive numbers.\n\nPlease set sampling interval burst time properly."), _("Warning"), wxOK );
      message.ShowModal();
      textSCSamplingInterval->SetFocus();
    }
    else
    {
      wxMessageDialog message( this, _("Times must be positive numbers.\n\nPlease set minimum burst time properly."), _("Warning"), wxOK );
      message.ShowModal();
      textSCMinimumBurstTime->SetFocus();
    }
    previousWarning = true;
  }

  // Empty list of events?
  if ( !previousWarning && listSCSelectedEvents->GetCount() == 0 )
  {
    wxMessageDialog message( this, _("The list of event types is empty.\n\nPlease add at least one event type."), _("Warning"), wxOK );
    message.ShowModal();
    buttonSCSelectedEventsAdd->SetFocus();
    previousWarning = true;
  }
}


void CutFilterDialog::OnOkClick( wxCommandEvent& event )
{
  // To avoid annoying multiple warning windows at the same time, and also final filter creation
  bool previousWarning = false;

  CheckCommonOptions( previousWarning );
  TransferWindowToCommonData( previousWarning );

  if ( !previousWarning )
  {
    // Which tools are selected?
    for (size_t i = 0; i < checkListToolOrder->GetCount(); ++i )
    {
      if ( checkListToolOrder->IsChecked( i ) )
      {
        if ( listToolOrder[ i ] == "Cutter" )
        {
          CheckCutterOptions( previousWarning );
          TransferWindowToCutterData( previousWarning );
        }
        if ( listToolOrder[ i ] == "Filter" )
        {
          CheckFilterOptions( previousWarning );
          TransferWindowToFilterData( previousWarning );
        }
        if ( listToolOrder[ i ] == "Software Counters" )
        {
          CheckSoftwareCountersOptions( previousWarning );
          TransferWindowToSoftwareCountersData( previousWarning );
        }
      }
    }

    if( previousWarning )
      return;
    else
      EndModal( wxID_OK );
  }
  else
    return;
}


void CutFilterDialog::OnPanelFilterUpdate( wxUpdateUIEvent& event )
{
  staticBoxSizerFilterStates->Enable( !checkFilterDiscardStateRecords->IsChecked() );
  checkListFilterStates->Enable( !checkFilterDiscardStateRecords->IsChecked() );
  buttonFilterSelectAll->Enable( !checkFilterDiscardStateRecords->IsChecked() );
  buttonFilterUnselectAll->Enable( !checkFilterDiscardStateRecords->IsChecked() );
  labelFilterMinBurstTime->Enable( !checkFilterDiscardStateRecords->IsChecked() );
  textFilterMinBurstTime->Enable( !checkFilterDiscardStateRecords->IsChecked() );

  staticBoxSizerFilterEvents->Enable( !checkFilterDiscardEventRecords->IsChecked() );
  listboxFilterEvents->Enable( !checkFilterDiscardEventRecords->IsChecked() );
  buttonFilterAdd->Enable( !checkFilterDiscardEventRecords->IsChecked() );
  buttonFilterDelete->Enable( !checkFilterDiscardEventRecords->IsChecked() );
  checkFilterDiscardListedEvents->Enable( !checkFilterDiscardEventRecords->IsChecked() );

  staticBoxSizerFilterCommunications->Enable( !checkFilterDiscardCommunicationRecords->IsChecked() );
  staticTextFilterSize->Enable( !checkFilterDiscardCommunicationRecords->IsChecked() );
  textFilterSize->Enable( !checkFilterDiscardCommunicationRecords->IsChecked() );
  staticTextFilterSizeUnit->Enable( !checkFilterDiscardCommunicationRecords->IsChecked() );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_FILTER_ADD
 */

void CutFilterDialog::OnButtonFilterAddClick( wxCommandEvent& event )
{
  wxTextEntryDialog textEntry( this, 
                               wxString() << _("Allowed formats:\n")
                                          << _(" Single event type: \'Type\'\n")
                                          << _(" Range of event types: \'Begin type-End type\'\n")
                                          << _(" Values for a single type: \'Type:Value 1,...,Value n\'"),
                               _("Add events") );
                               
  if( textEntry.ShowModal() == wxID_OK )
  {
    unsigned long tmp;
    wxStringTokenizer tok;
    bool errorString = false;
    wxString tmpStr;
    
    if( textEntry.GetValue() == _("") )
      return;
      
    tok.SetString( textEntry.GetValue(), _("-:,") );

    while( ( tmpStr = tok.GetNextToken() ) != _("") )
    {
      if( !tmpStr.ToULong( &tmp ) )
      {
        errorString = true;
        break;
      }
    }
    
    if( errorString )
    {
      wxMessageBox( _("Text inserted doesn't fit the allowed formats"), _("Not allowed format") );
      return;
    }
    
    listboxFilterEvents->Append( textEntry.GetValue() );
  }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_FILTER_DELETE
 */

void CutFilterDialog::OnButtonFilterDeleteClick( wxCommandEvent& event )
{
  wxArrayInt selec;

  if( listboxFilterEvents->GetSelections( selec ) == 0 )
    return;

  listboxFilterEvents->Delete( selec[ 0 ] );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SC_SELECTED_EVENTS_ADD
 */

void CutFilterDialog::OnButtonScSelectedEventsAddClick( wxCommandEvent& event )
{
  wxTextEntryDialog textEntry( this, 
                               wxString() << _("Allowed formats:\n")
                                          << _(" Single event type: \'Type\'\n")
                                          << _(" Values for a single type: \'Type:Value 1,...,Value n\'"),
                               _("Add events") );
                               
  if( textEntry.ShowModal() == wxID_OK )
  {
    unsigned long tmp;
    wxStringTokenizer tok;
    bool errorString = false;
    wxString tmpStr;
    
    if( textEntry.GetValue() == _("") )
      return;
      
    tok.SetString( textEntry.GetValue(), _(":,") );

    while( ( tmpStr = tok.GetNextToken() ) != _("") )
    {
      if( !tmpStr.ToULong( &tmp ) )
      {
        errorString = true;
        break;
      }
    }
    
    if( errorString )
    {
      wxMessageBox( _("Text inserted doesn't fit the allowed formats"), _("Not allowed format") );
      return;
    }
    
    listSCSelectedEvents->Append( textEntry.GetValue() );
  }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SC_SELECTED_EVENTS_DELETE
 */

void CutFilterDialog::OnButtonScSelectedEventsDeleteClick( wxCommandEvent& event )
{
  wxArrayInt selec;
  
  if( listSCSelectedEvents->GetSelections( selec ) == 0 )
    return;
    
  listSCSelectedEvents->Delete( selec[ 0 ] );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SC_KEEP_EVENTS_ADD
 */

void CutFilterDialog::OnButtonScKeepEventsAddClick( wxCommandEvent& event )
{
  wxTextEntryDialog textEntry( this, 
                               wxString() << _("Allowed formats:\n")
                                          << _(" Single event type: \'Type\'\n")
                                          << _(" Range of event types: \'Begin type-End type\'\n"),
                               _("Add events") );
                               
  if( textEntry.ShowModal() == wxID_OK )
  {
    unsigned long tmp;
    wxStringTokenizer tok;
    bool errorString = false;
    wxString tmpStr;
    
    if( textEntry.GetValue() == _("") )
      return;
      
    tok.SetString( textEntry.GetValue(), _("-") );

    while( ( tmpStr = tok.GetNextToken() ) != _("") )
    {
      if( !tmpStr.ToULong( &tmp ) )
      {
        errorString = true;
        break;
      }
    }
    
    if( errorString )
    {
      wxMessageBox( _("Text inserted doesn't fit the allowed formats"), _("Not allowed format") );
      return;
    }
    
    listSCKeepEvents->Append( textEntry.GetValue() );
  }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SC_KEEP_EVENTS_DELETE
 */

void CutFilterDialog::OnButtonScKeepEventsDeleteClick( wxCommandEvent& event )
{
  wxArrayInt selec;
  
  if( listSCKeepEvents->GetSelections( selec ) == 0 )
    return;
    
  listSCKeepEvents->Delete( selec[ 0 ] );
}

bool CutFilterDialog::CheckStringTasks( wxString taskStr )
{
  if( taskStr == _( "" ) )
    return true;
    
  stringstream sstr( string( taskStr.mb_str() ) );
  
  while( !sstr.eof() )
  {
    string tmpStr;
    long tmpLong;
    
    std::getline( sstr, tmpStr, ',' );
    
    stringstream tmpStream( tmpStr );
    std::getline( tmpStream, tmpStr, '-' );
    if( !( stringstream( tmpStr ) >> tmpLong ) )
      return false;
    else if( tmpLong == 0 )
      return false;
    
    if( !tmpStream.eof() )
    {
      std::getline( tmpStream, tmpStr );
      if( !( stringstream( tmpStr ) >> tmpLong ) )
        return false;
      else if( tmpLong == 0 )
        return false;
    }
  }
  
  return true;
}


void CutFilterDialog::OnButtonFilterSelectAllClick( wxCommandEvent& event )
{
  for( size_t i = 0; i < checkListFilterStates->GetCount(); ++i )
  {
    checkListFilterStates->Check( i );
  }
}


void CutFilterDialog::OnButtonFilterUnselectAllClick( wxCommandEvent& event )
{
  for( size_t i = 0; i < checkListFilterStates->GetCount(); ++i )
  {
    checkListFilterStates->Check( i, false );
  }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CUTTER_SELECT_REGION
 */

void CutFilterDialog::OnButtonCutterSelectRegionClick( wxCommandEvent& event )
{
  wxGetApp().ActivateGlobalTiming( this );
  waitingGlobalTiming = true;
}


/*!
 * wxEVT_IDLE event handler for ID_CUTFILTERDIALOG
 */

void CutFilterDialog::OnIdle( wxIdleEvent& event )
{
  if( waitingGlobalTiming )
  {
    stringstream tmpsstr;
    
    tmpsstr << fixed;
    tmpsstr << wxGetApp().GetGlobalTimingBegin();
    textCutterBeginCut->SetValue( wxString( tmpsstr.str().c_str(), wxConvUTF8 ) );

    tmpsstr.str( "" );
    tmpsstr << wxGetApp().GetGlobalTimingEnd();
    textCutterEndCut->SetValue( wxString( tmpsstr.str().c_str(),  wxConvUTF8 ) );
    
    if( !wxGetApp().GetGlobalTiming() )
    {
      radioCutterCutByTime->SetValue( true );
      waitingGlobalTiming = false;
    }
  }
}


/*!
 * wxEVT_INIT_DIALOG event handler for ID_CUTFILTERDIALOG
 */

void CutFilterDialog::OnInitDialog( wxInitDialogEvent& event )
{
  filePickerTrace->SetPath( wxString( nameSourceTrace.c_str(), wxConvUTF8 ) );
  checkLoadResultingTrace->SetValue( loadResultingTrace );
}


void CutFilterDialog::TransferCommonDataToWindow( vector<int> order )
{
  vector< string > auxListToolOrder; 

  if( order.size() > 0 )
  {
    // Fill new list, in order, and keeping the non-selected in listToolOrder
    for( size_t i = 0; i < order.size(); ++i )
    {
      switch( order[i] )
      {
        case INC_CHOP_COUNTER:
          auxListToolOrder.push_back( "Cutter" );
          listToolOrder.erase( find( listToolOrder.begin(), listToolOrder.end(), string( "Cutter" )));
          break;
        case INC_FILTER_COUNTER:
          auxListToolOrder.push_back( "Filter" );
          listToolOrder.erase( find( listToolOrder.begin(), listToolOrder.end(), string( "Filter" )));
          break;
        case INC_SC_COUNTER:
          auxListToolOrder.push_back( "Software Counters" );
          listToolOrder.erase( find( listToolOrder.begin(), listToolOrder.end(), string( "Software Counters" )));
          break;
      }
    }

    // Add the non-selected to the end of the vector
    for( size_t i = 0; i < listToolOrder.size(); ++i )
    {
      auxListToolOrder.push_back( listToolOrder[i] );
    }

    // Set the new vector
    listToolOrder.swap( auxListToolOrder );
    UpdateToolList();

    // And check the selected, that fit in the firsts elems of the vector order
    for( size_t i = 0; i < order.size(); ++i )
    {
      checkListToolOrder->Check( i, true );
    }

    // Finally, select first one to allow update raise it properly.
    checkListToolOrder->SetSelection( 0 );
  }
}


void CutFilterDialog::TransferCutterDataToWindow( TraceOptions *traceOptions )
{
  stringstream aux;

  aux.str("");
  aux << traceOptions->get_max_trace_size();
  textCutterMaximumTraceSize->SetValue( wxString::FromAscii( aux.str().c_str() ) );

  radioCutterCutByTime->SetValue( (bool)traceOptions->get_by_time() );

  if ( radioCutterCutByTime->GetValue() )
  {
    aux.str("");
    aux << traceOptions->get_min_cutting_time();
    textCutterBeginCut->SetValue(  wxString::FromAscii( aux.str().c_str() ) );

    aux.str("");
    aux << traceOptions->get_max_cutting_time();
    textCutterEndCut->SetValue(  wxString::FromAscii( aux.str().c_str() ) );
  }
  else
  {
    aux.str("");
    aux << traceOptions->get_minimum_time_percentage();
    textCutterBeginCut->SetValue( wxString::FromAscii( aux.str().c_str() ) );

    aux.str("");
    aux << traceOptions->get_maximum_time_percentage();
    textCutterEndCut->SetValue( wxString::FromAscii( aux.str().c_str() ) );
  }

  checkCutterUseOriginalTime->SetValue( (bool)traceOptions->get_original_time() );
  checkCutterDontBreakStates->SetValue( !((bool)traceOptions->get_break_states()) );
  checkCutterRemoveFirstState->SetValue( (bool)traceOptions->get_remFirstStates() );
  checkCutterRemoveLastState->SetValue( (bool)traceOptions->get_remLastStates() );

  TraceOptions::TTasksList auxList;
  traceOptions->get_tasks_list( auxList );
  textCutterTasks->SetValue( wxString::FromAscii( auxList ) );
}


void CutFilterDialog::TransferFilterDataToWindow( TraceOptions *traceOptions )
{
  stringstream aux;

  // Discard Records
  checkFilterDiscardStateRecords->SetValue( !(bool)traceOptions->get_filter_states() );
  checkFilterDiscardEventRecords->SetValue( !(bool)traceOptions->get_filter_events() );
  checkFilterDiscardCommunicationRecords->SetValue( !(bool)traceOptions->get_filter_comms() );

  // States

  // Events
  checkFilterDiscardListedEvents->SetValue( (bool)traceOptions->get_discard_given_types() );

  TraceOptions::TFilterTypes auxEvents;
  traceOptions->get_filter_types( auxEvents );
  SetEventsList( auxEvents, traceOptions->get_filter_last_type() );

  // Communications
  aux.str("");
  aux << traceOptions->get_min_comm_size();
  textFilterSize->SetValue( wxString::FromAscii( aux.str().c_str() ) );

/* copy/paste!!! change!!!
    if ( !checkFilterDiscardStateRecords->IsChecked() )
    {
      bool allStatesSelected = true;
      for( size_t i = 0; i < checkListFilterStates->GetCount(); ++i )
      {
        if ( !checkListFilterStates->IsChecked( i ) )
        {
          allStatesSelected = false;
          break;
        }
      }

      TraceOptions::TStateNames auxNames;
      for( int i = 0; i < 20; ++i )
        auxNames[ i ] = NULL;
        
      traceOptions->set_all_states( (char)allStatesSelected );
      if ( allStatesSelected )
      {
        auxNames[ 0 ] = strdup( "All" );
      }
      else
      {
        // Read selected states and fill vector
        int pos = 0;

        for( size_t i = 0; i < checkListFilterStates->GetCount(); ++i )
        {
          if ( checkListFilterStates->IsChecked( i ) )
          {
            auxNames[ pos++ ] = strdup( (char *)checkListFilterStates->GetString( i ).c_str());
          }
        }

        if( pos == 0 )
        {
          auxNames[ 0 ] = strdup( "Running" );
        }

        traceOptions->set_state_names( auxNames );

        unsigned long auxULong;
        textFilterMinBurstTime->GetValue().ToULong( &auxULong );
        traceOptions->set_min_state_time( (unsigned long long)auxULong );
      }
    }
*/
}


void CutFilterDialog::TransferSoftwareCountersDataToWindow( TraceOptions *traceOptions )
{
  stringstream aux;

  // Region
  radioSCOnIntervals->SetValue( (bool)traceOptions->get_sc_onInterval() );

  aux.str("");
  aux << traceOptions->get_sc_sampling_interval();
  textSCSamplingInterval->SetValue( wxString::FromAscii( aux.str().c_str() ) );
 
  aux.str("");
  aux << traceOptions->get_sc_minimum_burst_time();
  textSCMinimumBurstTime->SetValue( wxString::FromAscii( aux.str().c_str() ) );

  // Selected events
  bool done = SetSoftwareCountersEventsListToString( string( traceOptions->get_sc_types() ),
                                                     listSCSelectedEvents );

  // Options
  radioSCAccumulateValues->SetValue( (bool)traceOptions->get_sc_acumm_counters() );
  checkSCRemoveStates->SetValue( (bool)traceOptions->get_sc_remove_states() );
  checkSCSummarizeUseful->SetValue( (bool)traceOptions->get_sc_summarize_states() );
  checkSCGlobalCounters->SetValue( (bool)traceOptions->get_sc_global_counters() );
  checkSCOnlyInBurstsCounting->SetValue( (bool)traceOptions->get_sc_only_in_bursts() );

  // Keep events
  done = SetSoftwareCountersEventsListToString( string( traceOptions->get_sc_types_kept() ),
                                                listSCKeepEvents );

  // Experimental feature?
  // traceOptions->set_sc_frequency( (int) scFrequency );
}


void CutFilterDialog::TransferDataToWindow( vector<int> order, TraceOptions *traceOptions )
{
  Freeze();

  TransferCommonDataToWindow( order );

  for( size_t i = 0; i < order.size(); ++i )
  {
    switch( order[i] )
    {
      case INC_CHOP_COUNTER:
        TransferCutterDataToWindow( traceOptions );
        break;
      case INC_FILTER_COUNTER:
        TransferFilterDataToWindow( traceOptions );
        break;
      case INC_SC_COUNTER:
        TransferSoftwareCountersDataToWindow( traceOptions );
        break;
    }
  }

  Thaw();
}


void CutFilterDialog::OnButtonLoadXMLClick( wxCommandEvent& event )
{
  wxFileDialog xmlSelectionDialog( this,
                        _( "Load XML Cut/Filter configuration file" ),
                        wxString( nameSourceTrace.c_str(), wxConvUTF8 ),
                        _( "" ), 
                        _( "XML configuration file (*.xml)|*.xml|All files (*.*)|*.*" ),
                        wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_CHANGE_DIR );

  if( xmlSelectionDialog.ShowModal() == wxID_OK )
  {
    TraceOptions *traceOptions = TraceOptions::create( GetLocalKernel() );
    wxString path = xmlSelectionDialog.GetPath();
    vector<int> order = traceOptions->parseDoc( (char *)string( path.mb_str()).c_str() );
    TransferDataToWindow( order, traceOptions );
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_NOTEBOOK_CUT_FILTER_OPTIONS
 */

void CutFilterDialog::OnNotebookCutFilterOptionsUpdate( wxUpdateUIEvent& event )
{
  ChangePageSelectionFromToolsOrderListToTabs();
}

