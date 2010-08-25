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

#include "cutfilterdialog.h"
#include "paraverconfig.h"

#include <sstream>
#include <iostream>
#include <algorithm>
#include <wx/filedlg.h>

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
  EVT_FILEPICKER_CHANGED( ID_FILECTRL_CUTFILTER_TRACE_SELECTION, CutFilterDialog::OnFilectrlTracePickerChanged )

  EVT_UPDATE_UI( ID_CHECKLISTBOX, CutFilterDialog::OnCheckListToolOrderUpdate )

  EVT_BUTTON( ID_BITMAPBUTTON_PUSH_UP, CutFilterDialog::OnBitmapbuttonPushUpClick )

  EVT_BUTTON( ID_BITMAPBUTTON_PUSH_DOWN, CutFilterDialog::OnBitmapbuttonPushDownClick )

  EVT_BUTTON( ID_BUTTON_CUTTER_ALL_TRACE, CutFilterDialog::OnButtonCutterAllTraceClick )

  EVT_CHECKBOX( ID_CHECKBOX_CHECK_CUTTER_ORIGINAL_TIME, CutFilterDialog::OnCheckOriginalTimeClick )

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
  filePickerTrace = NULL;
  checkListToolOrder = NULL;
  buttonUp = NULL;
  buttonDown = NULL;
  notebookTools = NULL;
  radioCutterCutByTime = NULL;
  radioCutterCutByPercent = NULL;
  textCutterBeginCut = NULL;
  textCutterEndCut = NULL;
  buttonCutterSelectRegion = NULL;
  buttonCutterAllTrace = NULL;
  checkCutterUseOriginalTime = NULL;
  checkCutterRemoveFirstState = NULL;
  checkCutterBreakStates = NULL;
  checkCutterRemoveLastState = NULL;
  textCutterMaximumTraceSize = NULL;
  checkFilterDiscardState = NULL;
  checkFilterDiscardEvent = NULL;
  checkFilterDiscardCommunication = NULL;
  checkListFilterStates = NULL;
  buttonFilterSelectAll = NULL;
  buttonFilterUnselectAll = NULL;
  buttonFilterSetMinimumTime = NULL;
  listboxFilterEvents = NULL;
  buttonFilterAdd = NULL;
  buttonFilterDelete = NULL;
  textFilterSize = NULL;
  radioSCOnIntervals = NULL;
  radioSCOnStates = NULL;
  textSCSamplingInterval = NULL;
  textSCMinimumBurstTime = NULL;
  checkListSCSelectedEvents = NULL;
  buttonSCSelectedEventsAdd = NULL;
  buttonSCSelectedEventsDelete = NULL;
  buttonSCCountEvents = NULL;
  buttonSCAccumulateValues = NULL;
  checkSCRemoveStates = NULL;
  checkSCSummarizeUseful = NULL;
  checkSCGlobalCounters = NULL;
  checkSCOnlyInBurstCounting = NULL;
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
  itemBoxSizer3->Add(itemBoxSizer4, 1, wxALIGN_TOP|wxALL, 5);

  wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Trace"), wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    itemStaticText5->SetToolTip(_("Trace that will be used by the Cut/Filter toolkit."));
  itemBoxSizer4->Add(itemStaticText5, 0, wxALIGN_LEFT|wxALL, 5);

  filePickerTrace = new wxFilePickerCtrl( itemDialog1, ID_FILECTRL_CUTFILTER_TRACE_SELECTION, _T("\"\""), _("Trace to Cut/Filter"), _T("Paraver trace (*.prv;*.prv.gz)|*.prv;*.prv.gz|All files (*.*)|*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_OPEN|wxFLP_FILE_MUST_EXIST|wxFLP_CHANGE_DIR );
  if (CutFilterDialog::ShowToolTips())
    filePickerTrace->SetToolTip(_("Trace that will be used by the Cut/Filter toolkit."));
  itemBoxSizer4->Add(filePickerTrace, 2, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer3->Add(itemBoxSizer7, 1, wxGROW, 5);

  wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("  Ordered execution"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer7->Add(itemStaticText8, 0, wxALIGN_LEFT|wxALL, 5);

  wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer7->Add(itemBoxSizer9, 1, wxGROW|wxALL, 5);

  wxArrayString checkListToolOrderStrings;
  checkListToolOrder = new wxCheckListBox( itemDialog1, ID_CHECKLISTBOX, wxDefaultPosition, wxDefaultSize, checkListToolOrderStrings, wxLB_SINGLE|wxLB_NEEDED_SB );
  if (CutFilterDialog::ShowToolTips())
    checkListToolOrder->SetToolTip(_("Select the order of the Cut/Filter tools."));
  itemBoxSizer9->Add(checkListToolOrder, 3, wxGROW|wxLEFT|wxTOP, 5);

  wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer9->Add(itemBoxSizer11, 1, wxALIGN_CENTER_VERTICAL, 2);

  buttonUp = new wxBitmapButton( itemDialog1, ID_BITMAPBUTTON_PUSH_UP, itemDialog1->GetBitmapResource(wxT("arrow_up.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (CutFilterDialog::ShowToolTips())
    buttonUp->SetToolTip(_("Select the order of the Cut/Filter tools."));
  itemBoxSizer11->Add(buttonUp, 1, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 5);

  buttonDown = new wxBitmapButton( itemDialog1, ID_BITMAPBUTTON_PUSH_DOWN, itemDialog1->GetBitmapResource(wxT("arrow_down.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (CutFilterDialog::ShowToolTips())
    buttonDown->SetToolTip(_("Select the order of the Cut/Filter tools."));
  itemBoxSizer11->Add(buttonDown, 1, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 5);

  notebookTools = new wxNotebook( itemDialog1, ID_NOTEBOOK_CUT_FILTER_OPTIONS, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

  wxPanel* itemPanel15 = new wxPanel( notebookTools, ID_PANEL_CUTTER, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxVERTICAL);
  itemPanel15->SetSizer(itemBoxSizer16);

  wxStaticBox* itemStaticBoxSizer17Static = new wxStaticBox(itemPanel15, wxID_STATIC, _(" Trace Limits "));
  wxStaticBoxSizer* itemStaticBoxSizer17 = new wxStaticBoxSizer(itemStaticBoxSizer17Static, wxVERTICAL);
  itemBoxSizer16->Add(itemStaticBoxSizer17, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer17->Add(itemBoxSizer18, 0, wxGROW|wxLEFT|wxTOP, 5);
  wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer18->Add(itemBoxSizer19, 1, wxGROW|wxLEFT|wxTOP, 5);
  radioCutterCutByTime = new wxRadioButton( itemPanel15, ID_RADIOBUTTON_CUTTER_CUT_BY_TIME, _("Cut by time"), wxDefaultPosition, wxDefaultSize, 0 );
  radioCutterCutByTime->SetValue(true);
  itemBoxSizer19->Add(radioCutterCutByTime, 1, wxALIGN_LEFT|wxLEFT|wxTOP, 5);

  radioCutterCutByPercent = new wxRadioButton( itemPanel15, ID_RADIOBUTTON_CUTTER_CUT_BY_PERCENT, _("Cut by time %"), wxDefaultPosition, wxDefaultSize, 0 );
  radioCutterCutByPercent->SetValue(false);
  itemBoxSizer19->Add(radioCutterCutByPercent, 1, wxALIGN_LEFT|wxLEFT|wxTOP, 5);

  wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer18->Add(itemBoxSizer22, 2, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer22->Add(itemBoxSizer23, 0, wxGROW|wxLEFT|wxTOP, 5);
  wxStaticText* itemStaticText24 = new wxStaticText( itemPanel15, wxID_STATIC, _("Begin"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer23->Add(itemStaticText24, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  textCutterBeginCut = new wxTextCtrl( itemPanel15, ID_TEXTCTRL_CUTTER_BEGIN_CUT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer23->Add(textCutterBeginCut, 3, wxGROW|wxLEFT|wxTOP|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer22->Add(itemBoxSizer26, 0, wxGROW|wxLEFT|wxTOP, 5);
  wxStaticText* itemStaticText27 = new wxStaticText( itemPanel15, wxID_STATIC, _("End"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer26->Add(itemStaticText27, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  textCutterEndCut = new wxTextCtrl( itemPanel15, ID_TEXTCTRL_CUTTER_END_CUT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer26->Add(textCutterEndCut, 3, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer29 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer17->Add(itemBoxSizer29, 0, wxGROW|wxALL, 5);
  buttonCutterSelectRegion = new wxButton( itemPanel15, ID_BUTTON_CUTTER_SELECT_REGION, _("Select Region..."), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer29->Add(buttonCutterSelectRegion, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  buttonCutterAllTrace = new wxButton( itemPanel15, ID_BUTTON_CUTTER_ALL_TRACE, _("All Trace"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer29->Add(buttonCutterAllTrace, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  wxStaticBox* itemStaticBoxSizer32Static = new wxStaticBox(itemPanel15, wxID_STATIC, _(" Trace Options "));
  wxStaticBoxSizer* itemStaticBoxSizer32 = new wxStaticBoxSizer(itemStaticBoxSizer32Static, wxVERTICAL);
  itemBoxSizer16->Add(itemStaticBoxSizer32, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer33 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer32->Add(itemBoxSizer33, 0, wxGROW|wxLEFT|wxTOP, 5);
  checkCutterUseOriginalTime = new wxCheckBox( itemPanel15, ID_CHECKBOX_CHECK_CUTTER_ORIGINAL_TIME, _("Use original time"), wxDefaultPosition, wxDefaultSize, 0 );
  checkCutterUseOriginalTime->SetValue(false);
  itemBoxSizer33->Add(checkCutterUseOriginalTime, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  checkCutterRemoveFirstState = new wxCheckBox( itemPanel15, ID_CHECKBOX_CUTTER_REMOVE_FIRST_STATE, _("Remove first state"), wxDefaultPosition, wxDefaultSize, 0 );
  checkCutterRemoveFirstState->SetValue(false);
  itemBoxSizer33->Add(checkCutterRemoveFirstState, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  wxBoxSizer* itemBoxSizer36 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer32->Add(itemBoxSizer36, 0, wxGROW|wxLEFT|wxTOP|wxBOTTOM, 5);
  checkCutterBreakStates = new wxCheckBox( itemPanel15, ID_CHECKBOX_CUTTER_BREAK_STATES, _("Break states"), wxDefaultPosition, wxDefaultSize, 0 );
  checkCutterBreakStates->SetValue(false);
  itemBoxSizer36->Add(checkCutterBreakStates, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  checkCutterRemoveLastState = new wxCheckBox( itemPanel15, ID_CHECKBOX_CUTTER_REMOVE_LAST_STATE, _("Remove last state"), wxDefaultPosition, wxDefaultSize, 0 );
  checkCutterRemoveLastState->SetValue(false);
  itemBoxSizer36->Add(checkCutterRemoveLastState, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  wxStaticBox* itemStaticBoxSizer39Static = new wxStaticBox(itemPanel15, wxID_STATIC, _(" Output Trace "));
  wxStaticBoxSizer* itemStaticBoxSizer39 = new wxStaticBoxSizer(itemStaticBoxSizer39Static, wxHORIZONTAL);
  itemBoxSizer16->Add(itemStaticBoxSizer39, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText40 = new wxStaticText( itemPanel15, wxID_STATIC, _("Maximum trace size"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer39->Add(itemStaticText40, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  textCutterMaximumTraceSize = new wxSpinCtrl( itemPanel15, ID_SPINCTRL_CUTTER_MAXIMUM_SIZE, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
  itemStaticBoxSizer39->Add(textCutterMaximumTraceSize, 3, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticText* itemStaticText42 = new wxStaticText( itemPanel15, wxID_STATIC, _("MB"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer39->Add(itemStaticText42, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  notebookTools->AddPage(itemPanel15, _("Cutter"));

  wxPanel* itemPanel43 = new wxPanel( notebookTools, ID_PANEL_FILTER, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer44 = new wxBoxSizer(wxVERTICAL);
  itemPanel43->SetSizer(itemBoxSizer44);

  wxStaticBox* itemStaticBoxSizer45Static = new wxStaticBox(itemPanel43, wxID_STATIC, _(" Discard Records "));
  wxStaticBoxSizer* itemStaticBoxSizer45 = new wxStaticBoxSizer(itemStaticBoxSizer45Static, wxVERTICAL);
  itemBoxSizer44->Add(itemStaticBoxSizer45, 0, wxGROW|wxALL, 5);
  checkFilterDiscardState = new wxCheckBox( itemPanel43, ID_CHECKBOX_FILTER_DISCARD_STATE, _("State"), wxDefaultPosition, wxDefaultSize, 0 );
  checkFilterDiscardState->SetValue(false);
  itemStaticBoxSizer45->Add(checkFilterDiscardState, 0, wxGROW|wxALL, 5);

  checkFilterDiscardEvent = new wxCheckBox( itemPanel43, ID_CHECKBOX_FILTER_DISCARD_EVENT, _("Event"), wxDefaultPosition, wxDefaultSize, 0 );
  checkFilterDiscardEvent->SetValue(false);
  itemStaticBoxSizer45->Add(checkFilterDiscardEvent, 0, wxGROW|wxALL, 5);

  checkFilterDiscardCommunication = new wxCheckBox( itemPanel43, ID_CHECKBOX_FILTER_DISCARD_COMMUNICATION, _("Communication"), wxDefaultPosition, wxDefaultSize, 0 );
  checkFilterDiscardCommunication->SetValue(false);
  itemStaticBoxSizer45->Add(checkFilterDiscardCommunication, 0, wxGROW|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer49Static = new wxStaticBox(itemPanel43, wxID_STATIC, _(" States "));
  wxStaticBoxSizer* itemStaticBoxSizer49 = new wxStaticBoxSizer(itemStaticBoxSizer49Static, wxHORIZONTAL);
  itemBoxSizer44->Add(itemStaticBoxSizer49, 1, wxGROW|wxALL, 5);
  wxArrayString checkListFilterStatesStrings;
  checkListFilterStates = new wxCheckListBox( itemPanel43, ID_CHECKLISTBOX_FILTER_STATES, wxDefaultPosition, wxDefaultSize, checkListFilterStatesStrings, wxLB_SINGLE );
  itemStaticBoxSizer49->Add(checkListFilterStates, 2, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer51 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer49->Add(itemBoxSizer51, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
  buttonFilterSelectAll = new wxButton( itemPanel43, ID_BUTTON_FILTER_SELECT_ALL, _("Select all"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer51->Add(buttonFilterSelectAll, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 5);

  buttonFilterUnselectAll = new wxButton( itemPanel43, ID_BUTTON_FILTER_UNSELECT_ALL, _("Unselect all"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer51->Add(buttonFilterUnselectAll, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 5);

  buttonFilterSetMinimumTime = new wxButton( itemPanel43, ID_BUTTON_FILTER_SET_MINIMUM_TIME, _("Set min time"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer51->Add(buttonFilterSetMinimumTime, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 5);

  wxStaticBox* itemStaticBoxSizer55Static = new wxStaticBox(itemPanel43, wxID_STATIC, _(" Events "));
  wxStaticBoxSizer* itemStaticBoxSizer55 = new wxStaticBoxSizer(itemStaticBoxSizer55Static, wxHORIZONTAL);
  itemBoxSizer44->Add(itemStaticBoxSizer55, 1, wxGROW|wxALL, 5);
  wxArrayString listboxFilterEventsStrings;
  listboxFilterEvents = new wxListBox( itemPanel43, ID_LISTBOX_FILTER_EVENTS, wxDefaultPosition, wxDefaultSize, listboxFilterEventsStrings, wxLB_SINGLE );
  itemStaticBoxSizer55->Add(listboxFilterEvents, 2, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer57 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer55->Add(itemBoxSizer57, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
  buttonFilterAdd = new wxButton( itemPanel43, ID_BUTTON_FILTER_ADD, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer57->Add(buttonFilterAdd, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 5);

  buttonFilterDelete = new wxButton( itemPanel43, ID_BUTTON_FILTER_DELETE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer57->Add(buttonFilterDelete, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 5);

  wxStaticBox* itemStaticBoxSizer60Static = new wxStaticBox(itemPanel43, wxID_STATIC, _(" Communications "));
  wxStaticBoxSizer* itemStaticBoxSizer60 = new wxStaticBoxSizer(itemStaticBoxSizer60Static, wxHORIZONTAL);
  itemBoxSizer44->Add(itemStaticBoxSizer60, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText61 = new wxStaticText( itemPanel43, wxID_STATIC, _("Size"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer60->Add(itemStaticText61, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  textFilterSize = new wxSpinCtrl( itemPanel43, ID_SPINCTRL_FILTER_SIZE, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
  itemStaticBoxSizer60->Add(textFilterSize, 3, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticText* itemStaticText63 = new wxStaticText( itemPanel43, wxID_STATIC, _("MB"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer60->Add(itemStaticText63, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  notebookTools->AddPage(itemPanel43, _("Filter"));

  wxPanel* itemPanel64 = new wxPanel( notebookTools, ID_PANEL_SOFTWARE_COUNTERS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer65 = new wxBoxSizer(wxVERTICAL);
  itemPanel64->SetSizer(itemBoxSizer65);

  wxStaticBox* itemStaticBoxSizer66Static = new wxStaticBox(itemPanel64, wxID_STATIC, _(" Region "));
  wxStaticBoxSizer* itemStaticBoxSizer66 = new wxStaticBoxSizer(itemStaticBoxSizer66Static, wxHORIZONTAL);
  itemBoxSizer65->Add(itemStaticBoxSizer66, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer67 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer66->Add(itemBoxSizer67, 0, wxGROW|wxALL, 5);
  radioSCOnIntervals = new wxRadioButton( itemPanel64, ID_RADIOBUTTON_SC_ON_INTERVALS, _("On intervals"), wxDefaultPosition, wxDefaultSize, 0 );
  radioSCOnIntervals->SetValue(false);
  itemBoxSizer67->Add(radioSCOnIntervals, 1, wxGROW|wxALL, 5);

  radioSCOnStates = new wxRadioButton( itemPanel64, ID_RADIOBUTTON_SC_ON_STATES, _("On states"), wxDefaultPosition, wxDefaultSize, 0 );
  radioSCOnStates->SetValue(false);
  itemBoxSizer67->Add(radioSCOnStates, 1, wxGROW|wxALL, 5);

  wxStaticLine* itemStaticLine70 = new wxStaticLine( itemPanel64, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
  itemStaticBoxSizer66->Add(itemStaticLine70, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer71 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer66->Add(itemBoxSizer71, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
  wxBoxSizer* itemBoxSizer72 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer71->Add(itemBoxSizer72, 1, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText73 = new wxStaticText( itemPanel64, wxID_STATIC, _("Sampling Interval (ns)"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer72->Add(itemStaticText73, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  textSCSamplingInterval = new wxTextCtrl( itemPanel64, ID_TEXTCTRL_SC_SAMPLING_INTERVAL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer72->Add(textSCSamplingInterval, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer75 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer71->Add(itemBoxSizer75, 1, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText76 = new wxStaticText( itemPanel64, wxID_STATIC, _("Min Burst Time (ns)"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer75->Add(itemStaticText76, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  textSCMinimumBurstTime = new wxTextCtrl( itemPanel64, ID_TEXTCTRL_SC_MINIMUM_BURST_TIME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer75->Add(textSCMinimumBurstTime, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer78Static = new wxStaticBox(itemPanel64, wxID_STATIC, _(" Selected events "));
  wxStaticBoxSizer* itemStaticBoxSizer78 = new wxStaticBoxSizer(itemStaticBoxSizer78Static, wxHORIZONTAL);
  itemBoxSizer65->Add(itemStaticBoxSizer78, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer79 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer78->Add(itemBoxSizer79, 1, wxGROW|wxALL, 5);
  wxArrayString checkListSCSelectedEventsStrings;
  checkListSCSelectedEvents = new wxCheckListBox( itemPanel64, ID_CHECKLISTBOX_SC_SELECTED_EVENTS, wxDefaultPosition, wxDefaultSize, checkListSCSelectedEventsStrings, wxLB_SINGLE );
  itemBoxSizer79->Add(checkListSCSelectedEvents, 2, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer81 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer79->Add(itemBoxSizer81, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
  buttonSCSelectedEventsAdd = new wxButton( itemPanel64, ID_BUTTON_SC_SELECTED_EVENTS_ADD, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer81->Add(buttonSCSelectedEventsAdd, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

  buttonSCSelectedEventsDelete = new wxButton( itemPanel64, ID_BUTTON_SC_SELECTED_EVENTS_DELETE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer81->Add(buttonSCSelectedEventsDelete, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

  wxStaticBox* itemStaticBoxSizer84Static = new wxStaticBox(itemPanel64, wxID_STATIC, _(" Options "));
  wxStaticBoxSizer* itemStaticBoxSizer84 = new wxStaticBoxSizer(itemStaticBoxSizer84Static, wxHORIZONTAL);
  itemBoxSizer65->Add(itemStaticBoxSizer84, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer85 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer84->Add(itemBoxSizer85, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer86 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer85->Add(itemBoxSizer86, 1, wxGROW|wxALL, 5);
  buttonSCCountEvents = new wxRadioButton( itemPanel64, ID_RADIOBUTTON_SC_COUNT_EVENTS, _("Count events"), wxDefaultPosition, wxDefaultSize, 0 );
  buttonSCCountEvents->SetValue(false);
  itemBoxSizer86->Add(buttonSCCountEvents, 1, wxGROW|wxALL, 5);

  buttonSCAccumulateValues = new wxRadioButton( itemPanel64, ID_RADIOBUTTON8, _("Accumulate values"), wxDefaultPosition, wxDefaultSize, 0 );
  buttonSCAccumulateValues->SetValue(false);
  itemBoxSizer86->Add(buttonSCAccumulateValues, 1, wxGROW|wxALL, 5);

  wxStaticLine* itemStaticLine89 = new wxStaticLine( itemPanel64, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
  itemBoxSizer85->Add(itemStaticLine89, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer90 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer85->Add(itemBoxSizer90, 1, wxGROW|wxALL, 5);
  checkSCRemoveStates = new wxCheckBox( itemPanel64, ID_CHECKBOX_SC_REMOVE_STATES, _("Remove states"), wxDefaultPosition, wxDefaultSize, 0 );
  checkSCRemoveStates->SetValue(false);
  itemBoxSizer90->Add(checkSCRemoveStates, 1, wxGROW|wxALL, 5);

  checkSCSummarizeUseful = new wxCheckBox( itemPanel64, ID_CHECKBOX_SC_SUMMARIZE_USEFUL, _("Summarize useful"), wxDefaultPosition, wxDefaultSize, 0 );
  checkSCSummarizeUseful->SetValue(false);
  itemBoxSizer90->Add(checkSCSummarizeUseful, 1, wxGROW|wxALL, 5);

  checkSCGlobalCounters = new wxCheckBox( itemPanel64, ID_CHECKBOX_SC_GLOBAL_COUNTERS, _("Global counters"), wxDefaultPosition, wxDefaultSize, 0 );
  checkSCGlobalCounters->SetValue(false);
  itemBoxSizer90->Add(checkSCGlobalCounters, 1, wxGROW|wxALL, 5);

  checkSCOnlyInBurstCounting = new wxCheckBox( itemPanel64, ID_CHECKBOX_SC_ONLY_IN_BURSTS_COUNTING, _("Only in bursts counting"), wxDefaultPosition, wxDefaultSize, 0 );
  checkSCOnlyInBurstCounting->SetValue(false);
  itemBoxSizer90->Add(checkSCOnlyInBurstCounting, 1, wxGROW|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer95Static = new wxStaticBox(itemPanel64, wxID_STATIC, _(" Keep events "));
  wxStaticBoxSizer* itemStaticBoxSizer95 = new wxStaticBoxSizer(itemStaticBoxSizer95Static, wxHORIZONTAL);
  itemBoxSizer65->Add(itemStaticBoxSizer95, 1, wxGROW|wxALL, 5);
  wxArrayString listSCKeepEventsStrings;
  listSCKeepEvents = new wxListBox( itemPanel64, ID_LISTBOX_SC_KEEP_EVENTS, wxDefaultPosition, wxDefaultSize, listSCKeepEventsStrings, wxLB_SINGLE );
  itemStaticBoxSizer95->Add(listSCKeepEvents, 2, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer97 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer95->Add(itemBoxSizer97, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
  buttonSCKeepEventsAdd = new wxButton( itemPanel64, ID_BUTTON_SC_KEEP_EVENTS_ADD, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer97->Add(buttonSCKeepEventsAdd, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonSCKeepEventsDelete = new wxButton( itemPanel64, ID_BUTTON_SC_KEEP_EVENTS_DELETE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer97->Add(buttonSCKeepEventsDelete, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  notebookTools->AddPage(itemPanel64, _("Software Counters"));

  itemBoxSizer2->Add(notebookTools, 4, wxGROW|wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer100 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer100, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
  wxButton* itemButton101 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer100->AddButton(itemButton101);

  wxButton* itemButton102 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer100->AddButton(itemButton102);

  itemStdDialogButtonSizer100->Realize();

////@end CutFilterDialog content construction

  // Other initializations
  listToolOrder.push_back( string("Cutter") );
  listToolOrder.push_back( string("Filter") );
  listToolOrder.push_back( string("Software Counters") );

  UpdateToolList();

  //filePickerTrace->SetMessage( _( "Load Trace" ) );
  filePickerTrace->SetName( _( "Load Trace" ) );
  //filePickerTrace->SetPath( wxString::FromAscii( paraverConfig->getGlobalTracesPath().c_str() ) );
  //filePickerTrace->SetStyle(  wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_CHANGE_DIR );
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
    checkCutterBreakStates->SetValue( false );
    checkCutterBreakStates->Disable();
  }
  else
  {
    checkCutterBreakStates->Enable();
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
      if ( *it == string( notebookTools->GetPageText( i ).c_str()))
        (notebookTools->GetPage( i ))->Enable( checkListToolOrder->IsChecked( pos ) );
    }

    pos++;
  }
}


/*!
 * wxEVT_FILEPICKER_CHANGED event handler for ID_FILECTRL_CUTFILTER_TRACE_SELECTION
 */

void CutFilterDialog::OnFilectrlTracePickerChanged( wxFileDirPickerEvent& event )
{
  wxString path = filePickerTrace->GetPath();
cout << path << endl;
//  tracePath = wxFileName( path ).GetPath();
//  DoLoadTrace( std::string( path.mb_str() ) );
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
  radioCutterCutByPercent->SetValue( true );
  textCutterBeginCut->SetValue( formatNumber( 0 ));
  textCutterEndCut->SetValue( formatNumber( 100 ));
}

