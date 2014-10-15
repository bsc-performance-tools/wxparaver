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

#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <wx/filedlg.h>
#include <wx/tokenzr.h>
#include <wx/filename.h>

#include "cutfilterdialog.h"
#include "paraverconfig.h"
#include "kernelconnection.h"
//#include "wxparaverapp.h"
#include "loadedwindows.h"
#include "runscript.h"

////@begin XPM images
#include "arrow_up.xpm"
#include "arrow_down.xpm"
////@end XPM images

using namespace std;

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

  EVT_TEXT( ID_TEXTCTRL_CUT_FILTER_INPUT_TRACE, CutFilterDialog::OnTextctrlCutFilterInputTraceTextUpdated )

  EVT_TEXT( ID_TEXTCTRL_CUT_FILTER_XML, CutFilterDialog::OnTextctrlCutFilterXmlTextUpdated )

  EVT_LISTBOX_DCLICK( ID_CHECKLISTBOX_EXECUTION_CHAIN, CutFilterDialog::OnChecklistboxExecutionChainDoubleClicked )
  EVT_LISTBOX( ID_CHECKLISTBOX_EXECUTION_CHAIN, CutFilterDialog::OnCheckListExecutionChainSelected )
  EVT_CHECKLISTBOX( ID_CHECKLISTBOX_EXECUTION_CHAIN, CutFilterDialog::OnChecklistboxExecutionChainToggled )

  EVT_BUTTON( ID_BITMAPBUTTON_PUSH_UP_FILTER, CutFilterDialog::OnBitmapbuttonPushUpFilterClick )

  EVT_BUTTON( ID_BITMAPBUTTON_PUSH_DOWN_FILTER, CutFilterDialog::OnBitmapbuttonPushDownFilterClick )

  EVT_UPDATE_UI( ID_BUTTON_EDIT_XML, CutFilterDialog::OnButtonViewEditXmlUpdate )

  EVT_BUTTON( ID_BUTTON_SAVE_XML, CutFilterDialog::OnButtonSaveXmlClick )
  EVT_UPDATE_UI( ID_BUTTON_SAVE_XML, CutFilterDialog::OnButtonSaveXmlUpdate )

  EVT_NOTEBOOK_PAGE_CHANGED( ID_NOTEBOOK_CUT_FILTER_OPTIONS, CutFilterDialog::OnNotebookCutFilterOptionsPageChanged )

  EVT_BUTTON( ID_BUTTON_CUTTER_SELECT_REGION, CutFilterDialog::OnButtonCutterSelectRegionClick )
  EVT_UPDATE_UI( ID_BUTTON_CUTTER_SELECT_REGION, CutFilterDialog::OnButtonCutterSelectRegionUpdate )

  EVT_BUTTON( ID_BUTTON_CUTTER_ALL_TRACE, CutFilterDialog::OnButtonCutterAllTraceClick )

  EVT_UPDATE_UI( ID_CHECKBOX_CHECK_CUTTER_ORIGINAL_TIME, CutFilterDialog::OnCheckboxCheckCutterOriginalTimeUpdate )

  EVT_UPDATE_UI( ID_CHECKBOX_FILTER_DISCARD_STATE, CutFilterDialog::OnCheckboxFilterDiscardStateUpdate )

  EVT_UPDATE_UI( ID_CHECKBOX_FILTER_DISCARD_EVENT, CutFilterDialog::OnCheckboxFilterDiscardEventUpdate )

  EVT_UPDATE_UI( ID_CHECKBOX_FILTER_DISCARD_COMMUNICATION, CutFilterDialog::OnCheckboxFilterDiscardCommunicationUpdate )

  EVT_BUTTON( ID_BUTTON_FILTER_SELECT_ALL, CutFilterDialog::OnButtonFilterSelectAllClick )

  EVT_BUTTON( ID_BUTTON_FILTER_UNSELECT_ALL, CutFilterDialog::OnButtonFilterUnselectAllClick )

  EVT_BUTTON( ID_BUTTON_FILTER_ADD, CutFilterDialog::OnButtonFilterAddClick )

  EVT_BUTTON( ID_BUTTON_FILTER_DELETE, CutFilterDialog::OnButtonFilterDeleteClick )

  EVT_UPDATE_UI( ID_PANEL_SOFTWARE_COUNTERS, CutFilterDialog::OnPanelSoftwareCountersUpdate )

  EVT_BUTTON( ID_BUTTON_SC_SELECTED_EVENTS_ADD, CutFilterDialog::OnButtonScSelectedEventsAddClick )

  EVT_BUTTON( ID_BUTTON_SC_SELECTED_EVENTS_DELETE, CutFilterDialog::OnButtonScSelectedEventsDeleteClick )

  EVT_BUTTON( ID_BUTTON_SC_KEEP_EVENTS_ADD, CutFilterDialog::OnButtonScKeepEventsAddClick )

  EVT_BUTTON( ID_BUTTON_SC_KEEP_EVENTS_DELETE, CutFilterDialog::OnButtonScKeepEventsDeleteClick )

  EVT_BUTTON( wxID_APPLY, CutFilterDialog::OnApplyClick )
  EVT_UPDATE_UI( wxID_APPLY, CutFilterDialog::OnApplyUpdate )

////@end CutFilterDialog event table entries

END_EVENT_TABLE()


/*!
 * CutFilterDialog constructors
 */

CutFilterDialog::CutFilterDialog()
{
  Init();
}

CutFilterDialog::CutFilterDialog(
        wxWindow* parent,
        const wxString& whichXMLConfigurationFile, // useful when built from tutorial
        wxWindowID id,
        const wxString& caption,
        const wxPoint& pos,
        const wxSize& size,
        long style )
{
  Init();
  xmlConfigurationFile = whichXMLConfigurationFile;
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
  changedXMLParameters = false;
  globalXMLsPath = "";
  loadResultingTrace = true;
  localKernel = NULL;
  nameSourceTrace = "";
  newXMLsPath = false;
  runAppWithResultingTrace = false;
  waitingGlobalTiming = false;
  textCtrlInputTrace = NULL;
  fileBrowserButtonInputTrace = NULL;
  txtOutputTrace = NULL;
  textCtrlOutputTrace = NULL;
  fileBrowserButtonOutputTrace = NULL;
  checkLoadResultingTrace = NULL;
  checkRunAppWithResultingTrace = NULL;
  textCtrlXML = NULL;
  fileBrowserButtonXML = NULL;
  boxSizerExecutionChain = NULL;
  txtExecutionChain = NULL;
  checkListExecutionChain = NULL;
  buttonUp = NULL;
  buttonDown = NULL;
  buttonViewEditXml = NULL;
  buttonSaveXml = NULL;
  notebookTools = NULL;
  radioCutterCutByTime = NULL;
  radioCutterCutByTimePercent = NULL;
  textCutterBeginCut = NULL;
  textCutterEndCut = NULL;
  textCutterTasks = NULL;
  buttonCutterSelectRegion = NULL;
  buttonCutterAllWindow = NULL;
  buttonCutterAllTrace = NULL;
  checkCutterUseOriginalTime = NULL;
  checkCutterRemoveFirstState = NULL;
  checkCutterDontBreakStates = NULL;
  checkCutterRemoveLastState = NULL;
  checkCutterKeepEvents = NULL;
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
  buttonApply = NULL;
////@end CutFilterDialog member initialisation
  outputPath = "";
  xmlConfigurationFile.Clear(); // paranoic
  
  localKernel = paraverMain::myParaverMain->GetLocalKernel();
  traceOptions = TraceOptions::create( GetLocalKernel() );
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

  wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _(" Traces "));
  wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
  itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxGROW|wxALL, 2);

  wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer3->Add(itemBoxSizer4, 0, wxGROW|wxALL, 3);

  wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer4->Add(itemBoxSizer5, 0, wxGROW|wxALL, 2);

  wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Input"), wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    itemStaticText6->SetToolTip(_("Trace that will be used by the Cut/Filter toolkit."));
  itemBoxSizer5->Add(itemStaticText6, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  textCtrlInputTrace = new wxTextCtrl( itemDialog1, ID_TEXTCTRL_CUT_FILTER_INPUT_TRACE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    textCtrlInputTrace->SetToolTip(_("Trace that will be used by the Cut/Filter toolkit."));
  itemBoxSizer5->Add(textCtrlInputTrace, 3, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  fileBrowserButtonInputTrace = new FileBrowserButton( itemDialog1, ID_BUTTON_FILE_BROWSER_INPUT_TRACE, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    fileBrowserButtonInputTrace->SetToolTip(_("Trace that will be used by the Cut/Filter toolkit."));
  itemBoxSizer5->Add(fileBrowserButtonInputTrace, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer4->Add(itemBoxSizer9, 0, wxGROW|wxALL, 0);

  wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer9->Add(itemBoxSizer10, 0, wxGROW|wxALL, 2);

  txtOutputTrace = new wxStaticText( itemDialog1, wxID_STATIC, _("Output"), wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    txtOutputTrace->SetToolTip(_("Trace that will be saved by the Cut/Filter toolkit."));
  itemBoxSizer10->Add(txtOutputTrace, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  textCtrlOutputTrace = new wxTextCtrl( itemDialog1, ID_TEXTCTRL_CUT_FILTER_OUTPUT_TRACE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    textCtrlOutputTrace->SetToolTip(_("Trace generated by the Cut/Filter toolkit."));
  itemBoxSizer10->Add(textCtrlOutputTrace, 3, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  fileBrowserButtonOutputTrace = new FileBrowserButton( itemDialog1, ID_BUTTON_FILE_BROWSER_OUTPUT_TRACE, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    fileBrowserButtonOutputTrace->SetToolTip(_("Trace generated by the Cut/Filter toolkit."));
  itemBoxSizer10->Add(fileBrowserButtonOutputTrace, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer9->Add(itemBoxSizer14, 0, wxGROW, 2);

  itemBoxSizer14->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL, 2);

  checkLoadResultingTrace = new wxCheckBox( itemDialog1, ID_CHECKBOX_LOAD_RESULTING_TRACE, _("Load the processed trace"), wxDefaultPosition, wxDefaultSize, 0 );
  checkLoadResultingTrace->SetValue(true);
  if (CutFilterDialog::ShowToolTips())
    checkLoadResultingTrace->SetToolTip(_("After the selected tools are applied, the resulting trace is loaded."));
  itemBoxSizer14->Add(checkLoadResultingTrace, 4, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 2);

  wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer9->Add(itemBoxSizer17, 0, wxGROW, 2);

  itemBoxSizer17->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL, 2);

  checkRunAppWithResultingTrace = new wxCheckBox( itemDialog1, ID_CHECKBOX_RUN_APP_WITH_RESULTING_TRACE, _("Run application with the processed trace"), wxDefaultPosition, wxDefaultSize, 0 );
  checkRunAppWithResultingTrace->SetValue(false);
  if (CutFilterDialog::ShowToolTips())
    checkRunAppWithResultingTrace->SetToolTip(_("After the selected tools are applied, the resulting trace is passed to the external application selection window."));
  itemBoxSizer17->Add(checkRunAppWithResultingTrace, 4, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 2);

  wxStaticBox* itemStaticBoxSizer20Static = new wxStaticBox(itemDialog1, wxID_ANY, _(" Cut/Filter Parameters "));
  wxStaticBoxSizer* itemStaticBoxSizer20 = new wxStaticBoxSizer(itemStaticBoxSizer20Static, wxVERTICAL);
  itemBoxSizer2->Add(itemStaticBoxSizer20, 1, wxGROW|wxALL, 2);

  wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer20->Add(itemBoxSizer21, 0, wxGROW|wxALL, 2);

  wxStaticText* itemStaticText22 = new wxStaticText( itemDialog1, wxID_STATIC, _("Configuration file"), wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    itemStaticText22->SetToolTip(_("XML filter configuration that will be used by the Cut/Filter toolkit."));
  itemBoxSizer21->Add(itemStaticText22, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  textCtrlXML = new wxTextCtrl( itemDialog1, ID_TEXTCTRL_CUT_FILTER_XML, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    textCtrlXML->SetToolTip(_("Load new settings from an xml file. Not used tools won't be changed. "));
  itemBoxSizer21->Add(textCtrlXML, 3, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  fileBrowserButtonXML = new FileBrowserButton( itemDialog1, ID_FILEBROWSERBUTTON_CUT_FILTER_XML, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    fileBrowserButtonXML->SetToolTip(_("Load new settings from an xml file. Not used tools won't be changed. "));
  itemBoxSizer21->Add(fileBrowserButtonXML, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  boxSizerExecutionChain = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer20->Add(boxSizerExecutionChain, 0, wxGROW|wxALL, 2);

  boxSizerExecutionChain->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer27 = new wxBoxSizer(wxVERTICAL);
  boxSizerExecutionChain->Add(itemBoxSizer27, 2, wxGROW|wxALL, 2);

  txtExecutionChain = new wxStaticText( itemDialog1, wxID_STATIC, _("Execution chain"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer27->Add(txtExecutionChain, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxBOTTOM, 2);

  wxArrayString checkListExecutionChainStrings;
  checkListExecutionChain = new wxCheckListBox( itemDialog1, ID_CHECKLISTBOX_EXECUTION_CHAIN, wxDefaultPosition, wxDefaultSize, checkListExecutionChainStrings, wxLB_SINGLE|wxLB_NEEDED_SB );
  if (CutFilterDialog::ShowToolTips())
    checkListExecutionChain->SetToolTip(_("Select the order of the Cut/Filter tools."));
  itemBoxSizer27->Add(checkListExecutionChain, 1, wxGROW, 2);

  wxBoxSizer* itemBoxSizer30 = new wxBoxSizer(wxVERTICAL);
  boxSizerExecutionChain->Add(itemBoxSizer30, 0, wxALIGN_BOTTOM|wxALL, 2);

  buttonUp = new wxBitmapButton( itemDialog1, ID_BITMAPBUTTON_PUSH_UP_FILTER, itemDialog1->GetBitmapResource(wxT("arrow_up.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (CutFilterDialog::ShowToolTips())
    buttonUp->SetToolTip(_("Select the order of the Cut/Filter tools."));
  itemBoxSizer30->Add(buttonUp, 1, wxGROW|wxTOP, 2);

  buttonDown = new wxBitmapButton( itemDialog1, ID_BITMAPBUTTON_PUSH_DOWN_FILTER, itemDialog1->GetBitmapResource(wxT("arrow_down.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (CutFilterDialog::ShowToolTips())
    buttonDown->SetToolTip(_("Select the order of the Cut/Filter tools."));
  itemBoxSizer30->Add(buttonDown, 1, wxGROW|wxTOP, 2);

  boxSizerExecutionChain->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer34 = new wxBoxSizer(wxVERTICAL);
  boxSizerExecutionChain->Add(itemBoxSizer34, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  buttonViewEditXml = new wxToggleButton( itemDialog1, ID_BUTTON_EDIT_XML, _("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
  buttonViewEditXml->SetValue(false);
  buttonViewEditXml->Show(false);
  itemBoxSizer34->Add(buttonViewEditXml, 1, wxGROW|wxALL, 5);

  buttonSaveXml = new wxButton( itemDialog1, ID_BUTTON_SAVE_XML, _("Save..."), wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    buttonSaveXml->SetToolTip(_("Save current settings to an XML file."));
  itemBoxSizer34->Add(buttonSaveXml, 1, wxGROW|wxALL, 5);

  boxSizerExecutionChain->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  notebookTools = new wxNotebook( itemDialog1, ID_NOTEBOOK_CUT_FILTER_OPTIONS, wxDefaultPosition, wxSize(-1, 300), wxBK_DEFAULT );

  wxScrolledWindow* itemScrolledWindow39 = new wxScrolledWindow( notebookTools, ID_PANEL_CUTTER, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  itemScrolledWindow39->SetScrollbars(1, 10, 0, 0);
  wxBoxSizer* itemBoxSizer40 = new wxBoxSizer(wxVERTICAL);
  itemScrolledWindow39->SetSizer(itemBoxSizer40);

  wxStaticBox* itemStaticBoxSizer41Static = new wxStaticBox(itemScrolledWindow39, wxID_STATIC, _(" Trace Limits "));
  wxStaticBoxSizer* itemStaticBoxSizer41 = new wxStaticBoxSizer(itemStaticBoxSizer41Static, wxVERTICAL);
  itemBoxSizer40->Add(itemStaticBoxSizer41, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer42 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer41->Add(itemBoxSizer42, 0, wxGROW|wxLEFT|wxTOP, 5);
  wxBoxSizer* itemBoxSizer43 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer42->Add(itemBoxSizer43, 1, wxGROW|wxLEFT|wxTOP, 2);
  radioCutterCutByTime = new wxRadioButton( itemScrolledWindow39, ID_RADIOBUTTON_CUTTER_CUT_BY_TIME, _("Cut by time"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
  radioCutterCutByTime->SetValue(true);
  if (CutFilterDialog::ShowToolTips())
    radioCutterCutByTime->SetToolTip(_("Select this to cut [begin time, end time] region of the trace."));
  itemBoxSizer43->Add(radioCutterCutByTime, 1, wxALIGN_LEFT|wxLEFT|wxTOP, 2);

  radioCutterCutByTimePercent = new wxRadioButton( itemScrolledWindow39, ID_RADIOBUTTON_CUTTER_CUT_BY_PERCENT, _("Cut by time %"), wxDefaultPosition, wxDefaultSize, 0 );
  radioCutterCutByTimePercent->SetValue(false);
  if (CutFilterDialog::ShowToolTips())
    radioCutterCutByTimePercent->SetToolTip(_("Select this to cut [begin % time, end % time] region of the trace."));
  itemBoxSizer43->Add(radioCutterCutByTimePercent, 1, wxALIGN_LEFT|wxLEFT, 2);

  wxBoxSizer* itemBoxSizer46 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer42->Add(itemBoxSizer46, 2, wxGROW|wxRIGHT, 5);
  wxBoxSizer* itemBoxSizer47 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer46->Add(itemBoxSizer47, 0, wxGROW|wxLEFT|wxTOP, 2);
  wxStaticText* itemStaticText48 = new wxStaticText( itemScrolledWindow39, wxID_STATIC, _("Begin"), wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    itemStaticText48->SetToolTip(_("Initial timestamp or percent for the cut."));
  itemBoxSizer47->Add(itemStaticText48, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 2);

  textCutterBeginCut = new wxTextCtrl( itemScrolledWindow39, ID_TEXTCTRL_CUTTER_BEGIN_CUT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    textCutterBeginCut->SetToolTip(_("Initial timestamp or percent for the cut."));
  itemBoxSizer47->Add(textCutterBeginCut, 3, wxGROW|wxLEFT|wxTOP|wxBOTTOM, 2);

  wxBoxSizer* itemBoxSizer50 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer46->Add(itemBoxSizer50, 0, wxGROW|wxLEFT|wxTOP, 5);
  wxStaticText* itemStaticText51 = new wxStaticText( itemScrolledWindow39, wxID_STATIC, _("End"), wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    itemStaticText51->SetToolTip(_("Final timestamp or percent for the cut."));
  itemBoxSizer50->Add(itemStaticText51, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 2);

  textCutterEndCut = new wxTextCtrl( itemScrolledWindow39, ID_TEXTCTRL_CUTTER_END_CUT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    textCutterEndCut->SetToolTip(_("Final timestamp or percent for the cut."));
  itemBoxSizer50->Add(textCutterEndCut, 3, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 2);

  wxBoxSizer* itemBoxSizer53 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer41->Add(itemBoxSizer53, 0, wxGROW|wxALL, 2);
  wxStaticText* itemStaticText54 = new wxStaticText( itemScrolledWindow39, wxID_STATIC, _("Tasks"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer53->Add(itemStaticText54, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  textCutterTasks = new wxTextCtrl( itemScrolledWindow39, ID_TEXTCTRL_CUTTER_TASKS, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    textCutterTasks->SetToolTip(_("Keep only information of tasks specified, separated by commas. Ranges marked with \"-\" are allowed. I.e. \"1-32,33,64-128\". Leave it empty to select all the tasks."));
  itemBoxSizer53->Add(textCutterTasks, 1, wxGROW|wxALL, 2);

  wxStaticLine* itemStaticLine56 = new wxStaticLine( itemScrolledWindow39, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemStaticBoxSizer41->Add(itemStaticLine56, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer57 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer41->Add(itemBoxSizer57, 0, wxGROW|wxALL, 2);
  buttonCutterSelectRegion = new wxButton( itemScrolledWindow39, ID_BUTTON_CUTTER_SELECT_REGION, _("Select Region..."), wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    buttonCutterSelectRegion->SetToolTip(_("Fill times range directly clicking or dragging from timelines. You can click on different timelines."));
  itemBoxSizer57->Add(buttonCutterSelectRegion, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 2);

  buttonCutterAllWindow = new wxButton( itemScrolledWindow39, ID_BUTTON_CUTTER_ALL_WINDOW, _("All Window"), wxDefaultPosition, wxDefaultSize, 0 );
  buttonCutterAllWindow->Enable(false);
  itemBoxSizer57->Add(buttonCutterAllWindow, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 2);

  buttonCutterAllTrace = new wxButton( itemScrolledWindow39, ID_BUTTON_CUTTER_ALL_TRACE, _("All Trace"), wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    buttonCutterAllTrace->SetToolTip(_("Set range [0%, 100%]."));
  itemBoxSizer57->Add(buttonCutterAllTrace, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 2);

  wxStaticBox* itemStaticBoxSizer61Static = new wxStaticBox(itemScrolledWindow39, wxID_STATIC, _(" Trace Options "));
  wxStaticBoxSizer* itemStaticBoxSizer61 = new wxStaticBoxSizer(itemStaticBoxSizer61Static, wxVERTICAL);
  itemBoxSizer40->Add(itemStaticBoxSizer61, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer62 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer61->Add(itemBoxSizer62, 0, wxGROW|wxLEFT|wxTOP, 2);
  checkCutterUseOriginalTime = new wxCheckBox( itemScrolledWindow39, ID_CHECKBOX_CHECK_CUTTER_ORIGINAL_TIME, _("Use original time"), wxDefaultPosition, wxDefaultSize, 0 );
  checkCutterUseOriginalTime->SetValue(false);
  if (CutFilterDialog::ShowToolTips())
    checkCutterUseOriginalTime->SetToolTip(_("If not set, after the cut the first timestamp will be set to 0, and the difference with the original time will be substracted to all the times. If set, original time is kept."));
  itemBoxSizer62->Add(checkCutterUseOriginalTime, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 2);

  checkCutterRemoveFirstState = new wxCheckBox( itemScrolledWindow39, ID_CHECKBOX_CUTTER_REMOVE_FIRST_STATE, _("Remove first state"), wxDefaultPosition, wxDefaultSize, 0 );
  checkCutterRemoveFirstState->SetValue(false);
  if (CutFilterDialog::ShowToolTips())
    checkCutterRemoveFirstState->SetToolTip(_("If the begin limit is inside a burst, don't keep it."));
  itemBoxSizer62->Add(checkCutterRemoveFirstState, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 2);

  wxBoxSizer* itemBoxSizer65 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer61->Add(itemBoxSizer65, 0, wxGROW|wxLEFT|wxTOP, 2);
  checkCutterDontBreakStates = new wxCheckBox( itemScrolledWindow39, ID_CHECKBOX_CUTTER_BREAK_STATES, _("Don't break states"), wxDefaultPosition, wxDefaultSize, 0 );
  checkCutterDontBreakStates->SetValue(false);
  if (CutFilterDialog::ShowToolTips())
    checkCutterDontBreakStates->SetToolTip(_("If set, no matter the given limits, the states in the middle of the cut are maintained. If not set the limits will split them. This options is overriden if \"Use original time\" is set."));
  itemBoxSizer65->Add(checkCutterDontBreakStates, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 2);

  checkCutterRemoveLastState = new wxCheckBox( itemScrolledWindow39, ID_CHECKBOX_CUTTER_REMOVE_LAST_STATE, _("Remove last state"), wxDefaultPosition, wxDefaultSize, 0 );
  checkCutterRemoveLastState->SetValue(false);
  if (CutFilterDialog::ShowToolTips())
    checkCutterRemoveLastState->SetToolTip(_("If the end limit is inside a burst, don't keep it."));
  itemBoxSizer65->Add(checkCutterRemoveLastState, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 2);

  wxBoxSizer* itemBoxSizer68 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer61->Add(itemBoxSizer68, 0, wxGROW|wxLEFT|wxTOP|wxBOTTOM, 2);
  checkCutterKeepEvents = new wxCheckBox( itemScrolledWindow39, ID_CHECKBOX_CUTTER_KEEP_EVENTS, _("Keep boundary events"), wxDefaultPosition, wxDefaultSize, 0 );
  checkCutterKeepEvents->SetValue(false);
  itemBoxSizer68->Add(checkCutterKeepEvents, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 2);

  wxStaticBox* itemStaticBoxSizer70Static = new wxStaticBox(itemScrolledWindow39, wxID_STATIC, _(" Output Trace "));
  wxStaticBoxSizer* itemStaticBoxSizer70 = new wxStaticBoxSizer(itemStaticBoxSizer70Static, wxHORIZONTAL);
  itemBoxSizer40->Add(itemStaticBoxSizer70, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText71 = new wxStaticText( itemScrolledWindow39, wxID_STATIC, _("Maximum trace size"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
  itemStaticBoxSizer70->Add(itemStaticText71, 1, wxGROW|wxALL, 2);

  textCutterMaximumTraceSize = new wxSpinCtrl( itemScrolledWindow39, ID_SPINCTRL_CUTTER_MAXIMUM_SIZE, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000000, 0 );
  if (CutFilterDialog::ShowToolTips())
    textCutterMaximumTraceSize->SetToolTip(_("Set upper limit for the size of the cutted trace  in MB. Once this limit is reached, no more records will be written to the resulting trace."));
  itemStaticBoxSizer70->Add(textCutterMaximumTraceSize, 3, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  wxStaticText* itemStaticText73 = new wxStaticText( itemScrolledWindow39, wxID_STATIC, _("MB"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer70->Add(itemStaticText73, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  itemScrolledWindow39->FitInside();
  notebookTools->AddPage(itemScrolledWindow39, _("Cutter"));

  wxScrolledWindow* itemScrolledWindow74 = new wxScrolledWindow( notebookTools, ID_PANEL_FILTER, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  itemScrolledWindow74->SetScrollbars(1, 10, 0, 0);
  wxBoxSizer* itemBoxSizer75 = new wxBoxSizer(wxVERTICAL);
  itemScrolledWindow74->SetSizer(itemBoxSizer75);

  wxStaticBox* itemStaticBoxSizer76Static = new wxStaticBox(itemScrolledWindow74, wxID_STATIC, _(" Discard Records "));
  wxStaticBoxSizer* itemStaticBoxSizer76 = new wxStaticBoxSizer(itemStaticBoxSizer76Static, wxHORIZONTAL);
  itemBoxSizer75->Add(itemStaticBoxSizer76, 0, wxGROW|wxALL, 3);
  checkFilterDiscardStateRecords = new wxCheckBox( itemScrolledWindow74, ID_CHECKBOX_FILTER_DISCARD_STATE, _("State"), wxDefaultPosition, wxDefaultSize, 0 );
  checkFilterDiscardStateRecords->SetValue(false);
  if (CutFilterDialog::ShowToolTips())
    checkFilterDiscardStateRecords->SetToolTip(_("Discard all the state records from the source trace."));
  itemStaticBoxSizer76->Add(checkFilterDiscardStateRecords, 0, wxGROW|wxALL, 2);

  checkFilterDiscardEventRecords = new wxCheckBox( itemScrolledWindow74, ID_CHECKBOX_FILTER_DISCARD_EVENT, _("Event"), wxDefaultPosition, wxDefaultSize, 0 );
  checkFilterDiscardEventRecords->SetValue(false);
  if (CutFilterDialog::ShowToolTips())
    checkFilterDiscardEventRecords->SetToolTip(_("Discard all the event records from the source trace."));
  itemStaticBoxSizer76->Add(checkFilterDiscardEventRecords, 0, wxGROW|wxALL, 2);

  checkFilterDiscardCommunicationRecords = new wxCheckBox( itemScrolledWindow74, ID_CHECKBOX_FILTER_DISCARD_COMMUNICATION, _("Communication"), wxDefaultPosition, wxDefaultSize, 0 );
  checkFilterDiscardCommunicationRecords->SetValue(false);
  if (CutFilterDialog::ShowToolTips())
    checkFilterDiscardCommunicationRecords->SetToolTip(_("Discard all the communication records from the source trace."));
  itemStaticBoxSizer76->Add(checkFilterDiscardCommunicationRecords, 0, wxGROW|wxALL, 2);

  staticBoxSizerFilterStates = new wxStaticBox(itemScrolledWindow74, wxID_STATIC, _("Keep states "));
  wxStaticBoxSizer* itemStaticBoxSizer80 = new wxStaticBoxSizer(staticBoxSizerFilterStates, wxHORIZONTAL);
  itemBoxSizer75->Add(itemStaticBoxSizer80, 1, wxGROW|wxALL, 3);
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
  checkListFilterStates = new wxCheckListBox( itemScrolledWindow74, ID_CHECKLISTBOX_FILTER_STATES, wxDefaultPosition, wxSize(-1, 100), checkListFilterStatesStrings, wxLB_SINGLE );
  if (CutFilterDialog::ShowToolTips())
    checkListFilterStates->SetToolTip(_("Check the states that you want to keep in the filtered trace."));
  itemStaticBoxSizer80->Add(checkListFilterStates, 3, wxGROW|wxALL, 2);

  wxBoxSizer* itemBoxSizer82 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer80->Add(itemBoxSizer82, 2, wxGROW|wxALL, 5);
  buttonFilterSelectAll = new wxButton( itemScrolledWindow74, ID_BUTTON_FILTER_SELECT_ALL, _("Select all"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer82->Add(buttonFilterSelectAll, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 5);

  buttonFilterUnselectAll = new wxButton( itemScrolledWindow74, ID_BUTTON_FILTER_UNSELECT_ALL, _("Unselect all"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer82->Add(buttonFilterUnselectAll, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer85 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer82->Add(itemBoxSizer85, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
  labelFilterMinBurstTime = new wxStaticText( itemScrolledWindow74, wxID_STATIC, _("Min. burst time"), wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    labelFilterMinBurstTime->SetToolTip(_("Specify the minimum burst time for the state records."));
  itemBoxSizer85->Add(labelFilterMinBurstTime, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  textFilterMinBurstTime = new wxTextCtrl( itemScrolledWindow74, ID_TEXTCTRL_FILTER_MIN_BURST_TIME, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    textFilterMinBurstTime->SetToolTip(_("Specify the minimum burst time for the state records."));
  itemBoxSizer85->Add(textFilterMinBurstTime, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  staticBoxSizerFilterEvents = new wxStaticBox(itemScrolledWindow74, wxID_STATIC, _(" Events "));
  wxStaticBoxSizer* itemStaticBoxSizer88 = new wxStaticBoxSizer(staticBoxSizerFilterEvents, wxHORIZONTAL);
  itemBoxSizer75->Add(itemStaticBoxSizer88, 1, wxGROW|wxALL, 3);
  wxArrayString listboxFilterEventsStrings;
  listboxFilterEvents = new wxListBox( itemScrolledWindow74, ID_LISTBOX_FILTER_EVENTS, wxDefaultPosition, wxDefaultSize, listboxFilterEventsStrings, wxLB_SINGLE );
  if (CutFilterDialog::ShowToolTips())
    listboxFilterEvents->SetToolTip(_("List of the allowed events."));
  itemStaticBoxSizer88->Add(listboxFilterEvents, 3, wxGROW|wxALL, 2);

  wxBoxSizer* itemBoxSizer90 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer88->Add(itemBoxSizer90, 2, wxGROW|wxALL, 5);
  buttonFilterAdd = new wxButton( itemScrolledWindow74, ID_BUTTON_FILTER_ADD, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer90->Add(buttonFilterAdd, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 5);

  buttonFilterDelete = new wxButton( itemScrolledWindow74, ID_BUTTON_FILTER_DELETE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer90->Add(buttonFilterDelete, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 5);

  checkFilterDiscardListedEvents = new wxCheckBox( itemScrolledWindow74, ID_CHECKBOX_FILTER_DISCARD_LISTED_EVENTS, _("Discard"), wxDefaultPosition, wxDefaultSize, 0 );
  checkFilterDiscardListedEvents->SetValue(false);
  if (CutFilterDialog::ShowToolTips())
    checkFilterDiscardListedEvents->SetToolTip(_("If set, all the listed events will be discarded instead of being kept."));
  itemBoxSizer90->Add(checkFilterDiscardListedEvents, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  staticBoxSizerFilterCommunications = new wxStaticBox(itemScrolledWindow74, wxID_STATIC, _("Keep communications "));
  wxStaticBoxSizer* itemStaticBoxSizer94 = new wxStaticBoxSizer(staticBoxSizerFilterCommunications, wxHORIZONTAL);
  itemBoxSizer75->Add(itemStaticBoxSizer94, 0, wxGROW|wxALL, 3);
  staticTextFilterSize = new wxStaticText( itemScrolledWindow74, wxID_STATIC, _("Minimum size "), wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    staticTextFilterSize->SetToolTip(_("Allow only communications with a minimum size."));
  itemStaticBoxSizer94->Add(staticTextFilterSize, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  textFilterSize = new wxSpinCtrl( itemScrolledWindow74, ID_SPINCTRL_FILTER_SIZE, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100000000, 0 );
  if (CutFilterDialog::ShowToolTips())
    textFilterSize->SetToolTip(_("Allow only communications with a minimum size."));
  itemStaticBoxSizer94->Add(textFilterSize, 3, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  staticTextFilterSizeUnit = new wxStaticText( itemScrolledWindow74, wxID_STATIC, _("Bytes"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer94->Add(staticTextFilterSizeUnit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  itemScrolledWindow74->FitInside();
  notebookTools->AddPage(itemScrolledWindow74, _("Filter"));

  wxScrolledWindow* itemScrolledWindow98 = new wxScrolledWindow( notebookTools, ID_PANEL_SOFTWARE_COUNTERS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  itemScrolledWindow98->SetScrollbars(1, 10, 0, 0);
  wxBoxSizer* itemBoxSizer99 = new wxBoxSizer(wxVERTICAL);
  itemScrolledWindow98->SetSizer(itemBoxSizer99);

  wxStaticBox* itemStaticBoxSizer100Static = new wxStaticBox(itemScrolledWindow98, wxID_STATIC, _(" Region "));
  wxStaticBoxSizer* itemStaticBoxSizer100 = new wxStaticBoxSizer(itemStaticBoxSizer100Static, wxHORIZONTAL);
  itemBoxSizer99->Add(itemStaticBoxSizer100, 0, wxGROW|wxALL, 3);
  wxBoxSizer* itemBoxSizer101 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer100->Add(itemBoxSizer101, 0, wxGROW|wxALL, 0);
  radioSCOnIntervals = new wxRadioButton( itemScrolledWindow98, ID_RADIOBUTTON_SC_ON_INTERVALS, _("On intervals"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
  radioSCOnIntervals->SetValue(true);
  if (CutFilterDialog::ShowToolTips())
    radioSCOnIntervals->SetToolTip(_("The software counters will be written periodically after every time interval"));
  itemBoxSizer101->Add(radioSCOnIntervals, 1, wxGROW|wxALL, 2);

  radioSCOnStates = new wxRadioButton( itemScrolledWindow98, ID_RADIOBUTTON_SC_ON_STATES, _("On states"), wxDefaultPosition, wxDefaultSize, 0 );
  radioSCOnStates->SetValue(false);
  if (CutFilterDialog::ShowToolTips())
    radioSCOnStates->SetToolTip(_("The software counters will be written after every context switch of a running burst of at least the declared duration."));
  itemBoxSizer101->Add(radioSCOnStates, 1, wxGROW|wxALL, 2);

  wxStaticLine* itemStaticLine104 = new wxStaticLine( itemScrolledWindow98, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
  itemStaticBoxSizer100->Add(itemStaticLine104, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer105 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer100->Add(itemBoxSizer105, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);
  wxBoxSizer* itemBoxSizer106 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer105->Add(itemBoxSizer106, 1, wxGROW|wxALL, 2);
  staticTextSCSamplingInterval = new wxStaticText( itemScrolledWindow98, wxID_STATIC, _("Sampling Interval (ns)"), wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    staticTextSCSamplingInterval->SetToolTip(_("The software counters will be written periodically after every time interval"));
  itemBoxSizer106->Add(staticTextSCSamplingInterval, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  textSCSamplingInterval = new wxTextCtrl( itemScrolledWindow98, ID_TEXTCTRL_SC_SAMPLING_INTERVAL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    textSCSamplingInterval->SetToolTip(_("The software counters will be written periodically after every time interval."));
  itemBoxSizer106->Add(textSCSamplingInterval, 2, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  wxBoxSizer* itemBoxSizer109 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer105->Add(itemBoxSizer109, 1, wxGROW|wxALL, 2);
  staticTextSCMinimumBurstTime = new wxStaticText( itemScrolledWindow98, wxID_STATIC, _("Min Burst Time (ns)"), wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    staticTextSCMinimumBurstTime->SetToolTip(_("The software counters will be written after every context switch of a running burst of at least the declared duration."));
  itemBoxSizer109->Add(staticTextSCMinimumBurstTime, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  textSCMinimumBurstTime = new wxTextCtrl( itemScrolledWindow98, ID_TEXTCTRL_SC_MINIMUM_BURST_TIME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    textSCMinimumBurstTime->SetToolTip(_("The software counters will be written after every context switch of a running burst of at least the declared duration."));
  itemBoxSizer109->Add(textSCMinimumBurstTime, 2, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  wxStaticBox* itemStaticBoxSizer112Static = new wxStaticBox(itemScrolledWindow98, wxID_STATIC, _(" Selected events "));
  wxStaticBoxSizer* itemStaticBoxSizer112 = new wxStaticBoxSizer(itemStaticBoxSizer112Static, wxHORIZONTAL);
  itemBoxSizer99->Add(itemStaticBoxSizer112, 1, wxGROW|wxALL, 3);
  wxBoxSizer* itemBoxSizer113 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer112->Add(itemBoxSizer113, 1, wxGROW|wxALL, 0);
  wxArrayString listSCSelectedEventsStrings;
  listSCSelectedEvents = new wxListBox( itemScrolledWindow98, ID_CHECKLISTBOX_SC_SELECTED_EVENTS, wxDefaultPosition, wxDefaultSize, listSCSelectedEventsStrings, wxLB_SINGLE );
  if (CutFilterDialog::ShowToolTips())
    listSCSelectedEvents->SetToolTip(_("The counters will express the number of calls for every type-value specified in this list."));
  itemBoxSizer113->Add(listSCSelectedEvents, 3, wxGROW|wxALL, 2);

  wxBoxSizer* itemBoxSizer115 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer113->Add(itemBoxSizer115, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);
  buttonSCSelectedEventsAdd = new wxButton( itemScrolledWindow98, ID_BUTTON_SC_SELECTED_EVENTS_ADD, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer115->Add(buttonSCSelectedEventsAdd, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonSCSelectedEventsDelete = new wxButton( itemScrolledWindow98, ID_BUTTON_SC_SELECTED_EVENTS_DELETE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer115->Add(buttonSCSelectedEventsDelete, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer118Static = new wxStaticBox(itemScrolledWindow98, wxID_STATIC, _(" Options "));
  wxStaticBoxSizer* itemStaticBoxSizer118 = new wxStaticBoxSizer(itemStaticBoxSizer118Static, wxHORIZONTAL);
  itemBoxSizer99->Add(itemStaticBoxSizer118, 0, wxGROW|wxALL, 3);
  wxBoxSizer* itemBoxSizer119 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer118->Add(itemBoxSizer119, 1, wxGROW|wxALL, 0);
  wxBoxSizer* itemBoxSizer120 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer119->Add(itemBoxSizer120, 1, wxGROW|wxALL, 2);
  radioSCCountEvents = new wxRadioButton( itemScrolledWindow98, ID_RADIOBUTTON_SC_COUNT_EVENTS, _("Count events"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
  radioSCCountEvents->SetValue(false);
  if (CutFilterDialog::ShowToolTips())
    radioSCCountEvents->SetToolTip(_("Count how many times the type-event pairs appear in the source trace."));
  itemBoxSizer120->Add(radioSCCountEvents, 1, wxGROW|wxALL, 2);

  radioSCAccumulateValues = new wxRadioButton( itemScrolledWindow98, ID_RADIOBUTTON8, _("Accumulate values"), wxDefaultPosition, wxDefaultSize, 0 );
  radioSCAccumulateValues->SetValue(false);
  if (CutFilterDialog::ShowToolTips())
    radioSCAccumulateValues->SetToolTip(_("Add the values instead of counting how many times the type-event pairs appear in the source trace."));
  itemBoxSizer120->Add(radioSCAccumulateValues, 1, wxGROW|wxALL, 2);

  wxStaticLine* itemStaticLine123 = new wxStaticLine( itemScrolledWindow98, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
  itemBoxSizer119->Add(itemStaticLine123, 0, wxGROW|wxALL, 5);

  wxGridSizer* itemGridSizer124 = new wxGridSizer(2, 2, 0, 0);
  itemBoxSizer119->Add(itemGridSizer124, 2, wxGROW|wxALL, 0);
  checkSCRemoveStates = new wxCheckBox( itemScrolledWindow98, ID_CHECKBOX_SC_REMOVE_STATES, _("Remove states"), wxDefaultPosition, wxDefaultSize, 0 );
  checkSCRemoveStates->SetValue(false);
  itemGridSizer124->Add(checkSCRemoveStates, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 2);

  checkSCSummarizeUseful = new wxCheckBox( itemScrolledWindow98, ID_CHECKBOX_SC_SUMMARIZE_USEFUL, _("Summarize useful"), wxDefaultPosition, wxDefaultSize, 0 );
  checkSCSummarizeUseful->SetValue(false);
  itemGridSizer124->Add(checkSCSummarizeUseful, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 2);

  checkSCGlobalCounters = new wxCheckBox( itemScrolledWindow98, ID_CHECKBOX_SC_GLOBAL_COUNTERS, _("Global counters"), wxDefaultPosition, wxDefaultSize, 0 );
  checkSCGlobalCounters->SetValue(false);
  itemGridSizer124->Add(checkSCGlobalCounters, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 2);

  checkSCOnlyInBurstsCounting = new wxCheckBox( itemScrolledWindow98, ID_CHECKBOX_SC_ONLY_IN_BURSTS_COUNTING, _("Only in bursts counting"), wxDefaultPosition, wxDefaultSize, 0 );
  checkSCOnlyInBurstsCounting->SetValue(false);
  itemGridSizer124->Add(checkSCOnlyInBurstsCounting, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 2);

  wxStaticBox* itemStaticBoxSizer129Static = new wxStaticBox(itemScrolledWindow98, wxID_STATIC, _(" Keep events "));
  wxStaticBoxSizer* itemStaticBoxSizer129 = new wxStaticBoxSizer(itemStaticBoxSizer129Static, wxHORIZONTAL);
  itemBoxSizer99->Add(itemStaticBoxSizer129, 1, wxGROW|wxALL, 3);
  wxArrayString listSCKeepEventsStrings;
  listSCKeepEvents = new wxListBox( itemScrolledWindow98, ID_LISTBOX_SC_KEEP_EVENTS, wxDefaultPosition, wxDefaultSize, listSCKeepEventsStrings, wxLB_SINGLE );
  itemStaticBoxSizer129->Add(listSCKeepEvents, 2, wxGROW|wxALL, 2);

  wxBoxSizer* itemBoxSizer131 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer129->Add(itemBoxSizer131, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
  buttonSCKeepEventsAdd = new wxButton( itemScrolledWindow98, ID_BUTTON_SC_KEEP_EVENTS_ADD, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer131->Add(buttonSCKeepEventsAdd, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonSCKeepEventsDelete = new wxButton( itemScrolledWindow98, ID_BUTTON_SC_KEEP_EVENTS_DELETE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer131->Add(buttonSCKeepEventsDelete, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  itemScrolledWindow98->FitInside();
  notebookTools->AddPage(itemScrolledWindow98, _("Software Counters"));

  itemStaticBoxSizer20->Add(notebookTools, 1, wxGROW|wxALL, 2);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer134 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer134, 0, wxALIGN_RIGHT|wxALL, 2);
  wxButton* itemButton135 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer134->AddButton(itemButton135);

  buttonApply = new wxButton( itemDialog1, wxID_APPLY, _("&Apply"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer134->AddButton(buttonApply);

  itemStdDialogButtonSizer134->Realize();

////@end CutFilterDialog content construction


  // Other initializations

  // INDEX OF TABS
  // Current implementation uses static tabs for filters, so TABINDEX is constant
  TABINDEX[ TraceCutter::getID() ] = 0;
  TABINDEX[ TraceFilter::getID() ] = 1;
  TABINDEX[ TraceSoftwareCounters::getID() ] = 2;

  // TOOL NAMES
  // Sort here as you want them to appear in the tools list widget.
  listToolOrder.push_back( TraceCutter::getName() );
  listToolOrder.push_back( TraceFilter::getName() );
  listToolOrder.push_back( TraceSoftwareCounters::getName() );

  UpdateExecutionChain();
  EnableAllTabsFromToolsList();

  //fileBrowserButtonInputTrace->SetPath( wxString::FromAscii( paraverConfig->getGlobalTracesPath().c_str() ) );

  wxString tmpWildCard = wxT( "Paraver trace (*.prv;*.prv.gz)|*.prv;*.prv.gz|All files (*.*)|*.*" );
  fileBrowserButtonInputTrace->SetDialogMessage( _( "Load Trace" ) );
  fileBrowserButtonInputTrace->SetFileDialogWildcard( tmpWildCard );
  fileBrowserButtonInputTrace->SetTextBox( textCtrlInputTrace );
  fileBrowserButtonInputTrace->Enable();

  fileBrowserButtonOutputTrace->SetDialogMessage( _( "Save Trace" ) );
  fileBrowserButtonOutputTrace->SetFileDialogWildcard( tmpWildCard ); // also trace, so same than input
  fileBrowserButtonOutputTrace->SetTextBox( textCtrlOutputTrace );
  fileBrowserButtonOutputTrace->SetDialogStyle( wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR ); // place before Enable
  fileBrowserButtonOutputTrace->Enable();

  tmpWildCard = wxT( "XML configuration files (*.xml)|*.xml|All files (*.*)|*" );
  fileBrowserButtonXML->SetDialogMessage( wxT( "Load XML Cut/Filter configuration file" ) );
  fileBrowserButtonXML->SetFileDialogWildcard( tmpWildCard ); // also trace, so same than input
  fileBrowserButtonXML->SetTextBox( textCtrlXML );
  fileBrowserButtonXML->Enable();

  enableOutputTraceWidgets( false ); // depends on checked tools

  wxString directory;

  if ( !xmlConfigurationFile.empty() )
  {
    directory = xmlConfigurationFile;
  }
  else if ( !globalXMLsPath.empty() )
  {
    wxFileName auxDirectory( wxString( globalXMLsPath.c_str(), wxConvUTF8 )  );
    if( !auxDirectory.IsDir() )
      auxDirectory = auxDirectory.GetPathWithSep();
    directory = auxDirectory.GetFullPath();
    fileBrowserButtonXML->SetPath( directory );
  }


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


bool CutFilterDialog::isFileSelected( FileBrowserButton *fpc )
{
  wxString path = fpc->GetPath();
  wxFileName tmpName( path );

  return !( path == _("") || tmpName.IsDir() );
}


bool CutFilterDialog::isFileSelected( const string& fpc )
{
  wxString path = wxString( fpc.c_str(), wxConvUTF8 );
  wxFileName tmpName( path );

  return !( path == _("") || tmpName.IsDir() );
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

    // Avoid [ max, min ] times
    double auxBeginTime, auxEndTime;
    textCutterBeginCut->GetValue().ToDouble( &auxBeginTime );
    textCutterEndCut->GetValue().ToDouble( &auxEndTime );
    
    if ( auxBeginTime > auxEndTime )
    {
      // Swap
      wxString auxwxStrBegin = textCutterBeginCut->GetValue();
      textCutterBeginCut->SetValue( textCutterEndCut->GetValue() );
      textCutterEndCut->SetValue( auxwxStrBegin );
    }
    
    if( !wxGetApp().GetGlobalTiming() )
    {
    //  radioCutterCutByTime->SetValue( true ); // why?
      waitingGlobalTiming = false;
    }
    
   // wxGetApp().DeactivateGlobalTiming();
  }
}


/*!
 * wxEVT_INIT_DIALOG event handler for ID_CUTFILTERDIALOG
 */

void CutFilterDialog::OnInitDialog( wxInitDialogEvent& event )
{
  fileBrowserButtonInputTrace->SetPath( wxString( nameSourceTrace.c_str(), wxConvUTF8 ) );
  checkLoadResultingTrace->SetValue( loadResultingTrace );
  checkRunAppWithResultingTrace->SetValue( runAppWithResultingTrace );
  if ( globalXMLsPath.compare( "" ) == 0 )
    globalXMLsPath = nameSourceTrace;
}


// **********************************************************************************
// **********************************************************************************
//   ____      _   _            
//  / ___|   _| |_| |_ ___ _ __ 
// | |  | | | | __| __/ _ \ '__|
// | |__| |_| | |_| ||  __/ |   
//  \____\__,_|\__|\__\___|_|   
//                             
// **********************************************************************************
// **********************************************************************************

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
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CUTTER_SELECT_REGION
 */

void CutFilterDialog::OnButtonCutterSelectRegionClick( wxCommandEvent& event )
{
  radioCutterCutByTime->SetValue( true );
  wxGetApp().ActivateGlobalTiming( this );
  waitingGlobalTiming = true;
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
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_CUTTER_SELECT_REGION
 */
void CutFilterDialog::OnButtonCutterSelectRegionUpdate( wxUpdateUIEvent& event )
{
  buttonCutterSelectRegion->Enable( !LoadedWindows::getInstance()->emptyWindows() );
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


void CutFilterDialog::CheckCutterOptions( bool &previousWarning )
{
  if ( !previousWarning &&
       textCutterBeginCut->GetValue() == _("") &&
       textCutterEndCut->GetValue() == _("") &&
       textCutterTasks->GetValue() != _("") &&
       CheckStringTasks( textCutterTasks->GetValue() ))
  {
    wxMessageDialog message( this,
                             _("Cutter:\nEmpty times.\n\nDo you want to cut these tasks"
                               " all along the 100% of the trace?"),
                             _("Warning"), wxYES_NO|wxYES_DEFAULT );
    if ( message.ShowModal() == wxID_YES )
    {
      radioCutterCutByTimePercent->SetValue( true );
      textCutterBeginCut->SetValue( formatNumber( 0 ));
      textCutterEndCut->SetValue( formatNumber( 100 ));
    }
    else
    {
      textCutterBeginCut->SetFocus();
      previousWarning = true;
    }
  }

  // tasks string
  if( !previousWarning && !CheckStringTasks( textCutterTasks->GetValue() ) )
  {
    wxMessageDialog message( this, _("Cutter:\nNot allowed format in tasks text.\n\nPlease set it properly."), _("Warning"), wxOK );
    message.ShowModal();
    textCutterTasks->SetFocus();
    previousWarning = true;
  }

  // Time region selected?
  if ( !previousWarning && textCutterBeginCut->GetValue() == _("") )
  {
    wxMessageDialog message( this, _("Cutter:\nPlease set the initial time."), _( "Warning" ), wxOK );
    message.ShowModal();
    textCutterBeginCut->SetFocus();
    previousWarning = true;
  }

  if ( !previousWarning && textCutterEndCut->GetValue() == _("") )
  {
    wxMessageDialog message( this, _("Cutter:\nPlease set the final time."), _("Warning"), wxOK );
    message.ShowModal();
    textCutterEndCut->SetFocus();
    previousWarning = true;
  }

  // Are times set properly?
  double cutterBeginTime, cutterEndTime;
  textCutterBeginCut->GetValue().ToDouble( &cutterBeginTime );
  textCutterEndCut->GetValue().ToDouble( &cutterEndTime );

  // negative times?
  if ( !previousWarning && cutterBeginTime < 0.0 )
  {
    wxMessageDialog message( this, _("Cutter:\nTimes must be positive numbers.\n\nPlease set begin time properly."), _("Warning"), wxOK );
    message.ShowModal();
    textCutterBeginCut->SetFocus();
    previousWarning = true;
  }

  if ( !previousWarning && cutterEndTime < 0.0 )
  {
    wxMessageDialog message( this, _("Cutter:\nTimes must be positive numbers.\n\nPlease set end time properly."), _("Warning"), wxOK );
    message.ShowModal();
    textCutterEndCut->SetFocus();
    previousWarning = true;
  }

  // percent out of range?
  if ( !previousWarning && radioCutterCutByTimePercent->GetValue() && cutterBeginTime > 100.0 )
  {
    wxMessageDialog message( this, _("Cutter:\nBegin time percent greater than 100 %.\n\nPlease set time percent properly."), _("Warning"), wxOK );
    message.ShowModal();
    radioCutterCutByTimePercent->SetFocus();
    previousWarning = true;
  }

  if ( !previousWarning && radioCutterCutByTimePercent->GetValue() && cutterEndTime > 100.0 )
  {
    wxMessageDialog message( this, _("Cutter:\nEnd time percent greater than 100 %.\n\nPlease set time percent properly."), _("Warning"), wxOK );
    message.ShowModal();
    radioCutterCutByTimePercent->SetFocus();
    previousWarning = true;
  }

  // begin == end?
  if ( !previousWarning && cutterBeginTime == cutterEndTime )
  {
    wxMessageDialog message( this, _("Cutter:\nSame time for both limits.\n\nPlease set time range properly."), _("Warning"), wxOK );
    message.ShowModal();
    textCutterBeginCut->SetFocus();
    previousWarning = true;
  }

  // begin > end?
  if ( !previousWarning && cutterBeginTime > cutterEndTime ) // Idea: Maybe it could swap times in text boxes.
  {
    wxMessageDialog message( this, _("Cutter:\nBegin time greater than end time.\n\nPlease set time range properly."), _("Warning"), wxOK );
    message.ShowModal();
    textCutterBeginCut->SetFocus();
    previousWarning = true;
  }
}


void CutFilterDialog::TransferCutterDataToWindow( TraceOptions *traceOptions )
{
  stringstream aux;

  aux.str("");
  aux << traceOptions->get_max_trace_size();
  textCutterMaximumTraceSize->SetValue( wxString::FromAscii( aux.str().c_str() ) );

  if ( traceOptions->get_by_time() )
    radioCutterCutByTime->SetValue( true );
  else
    radioCutterCutByTimePercent->SetValue( true );

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

  checkCutterUseOriginalTime->SetValue( traceOptions->get_original_time() );
  if ( traceOptions->get_original_time() )
  {
    checkCutterDontBreakStates->SetValue( false );
    checkCutterDontBreakStates->Disable();
  }
  else
  {
    checkCutterDontBreakStates->SetValue( !traceOptions->get_break_states() );
    checkCutterDontBreakStates->Enable();
  }

  checkCutterRemoveFirstState->SetValue( traceOptions->get_remFirstStates() );
  checkCutterRemoveLastState->SetValue( traceOptions->get_remLastStates() );
  checkCutterKeepEvents->SetValue( traceOptions->get_keep_events() );

  TraceOptions::TTasksList auxList;
  traceOptions->get_tasks_list( auxList );
  textCutterTasks->SetValue( wxString::FromAscii( auxList ).Trim( true ).Trim( false ) );
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
    if( radioCutterCutByTime->GetValue() )
      auxULong = 0;
    traceOptions->set_minimum_time_percentage( (unsigned long long)auxULong );

    textCutterEndCut->GetValue().ToULongLong( &auxULong );
    traceOptions->set_max_cutting_time( (unsigned long long)auxULong );
    if( radioCutterCutByTime->GetValue() )
      auxULong = 100;
    traceOptions->set_maximum_time_percentage( (unsigned long long)auxULong );

    traceOptions->set_original_time( checkCutterUseOriginalTime->IsChecked() );
    traceOptions->set_break_states( !checkCutterDontBreakStates->IsChecked() );
    traceOptions->set_remFirstStates( checkCutterRemoveFirstState->IsChecked() );
    traceOptions->set_remLastStates( checkCutterRemoveLastState->IsChecked() );
    traceOptions->set_keep_events( checkCutterKeepEvents->IsChecked() );

#ifdef UNICODE
    traceOptions->set_tasks_list( (char *)textCutterTasks->GetValue().mb_str().data() );
#else
    traceOptions->set_tasks_list( (char *)textCutterTasks->GetValue().mb_str() );
#endif
  }
}


// **********************************************************************************
// **********************************************************************************
//  _____ _ _ _            
// |  ___(_) | |_ ___ _ __ 
// | |_  | | | __/ _ \ '__|
// |  _| | | | ||  __/ |   
// |_|   |_|_|\__\___|_|   
//                                                    
// **********************************************************************************
// **********************************************************************************


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
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_FILTER_DELETE
 */

void CutFilterDialog::OnButtonFilterDeleteClick( wxCommandEvent& event )
{
  wxArrayInt selec;

  if( listboxFilterEvents->GetSelections( selec ) == 0 )
    return;

  listboxFilterEvents->Delete( selec[ 0 ] );
}



void CutFilterDialog::CheckStatesList( size_t begin, bool value )
{
  for( size_t i = begin; i < checkListFilterStates->GetCount(); ++i )
    checkListFilterStates->Check( (int)i, value );
}

void CutFilterDialog::CheckStatesList( TraceOptions::TStateNames statesList )
{
  size_t s = 0;
  wxArrayString newStates;
  size_t oldMaxStates = checkListFilterStates->GetCount();

  // Look for all the given states in the checkboxlist, and check them.
  while( s < 20 && statesList[ s ] != NULL )
  { 
    bool found = false;
    wxString stateNameToCheck( statesList[ s ], wxConvUTF8 );
    stateNameToCheck = stateNameToCheck.Trim( true ).Trim( false );
    for( size_t i = 0; i < checkListFilterStates->GetCount(); ++i )
    {
      wxString stateName = checkListFilterStates->GetString( i ); // Assume name in list always trimmed both ways.
      if( stateNameToCheck == stateName )
      {
        checkListFilterStates->Check( i );
        found = true;
      }
    }

    if( !found )
    {
      newStates.Add( stateNameToCheck );
    }

    ++s;
  }

  // Was an empty list? Check "Running".
  if( s == 0 )
  {
    for( size_t i = 0; i < checkListFilterStates->GetCount(); ++i )
    {
      wxString stateName = checkListFilterStates->GetString( i );
      if( wxString("Running", wxConvUTF8) == stateName )
        checkListFilterStates->Check( i );
    }
  }

  // Have we found new states? Add them to the botton and check them.
  checkListFilterStates->InsertItems( newStates, checkListFilterStates->GetCount() );
  CheckStatesList( oldMaxStates, !( checkListFilterStates->GetCount() == 0 ) );
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

  listboxFilterEvents->Append( wxString( auxLine.str().c_str(), wxConvUTF8 ) );
}


void CutFilterDialog::SetEventsList( TraceOptions::TFilterTypes eventTypes, int lastType )
{
  listboxFilterEvents->Clear();

  for( int i = 0; i < lastType; ++i )
  {
    SetEventLine( eventTypes, i );
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
      wxMessageDialog message( this, _("Filter:\nNo state selected.\n\nPlease select at least one or\ndiscard all the state records."), _( "Warning" ), wxOK );
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
    wxMessageDialog message( this, _("Filter:\nTimes must be positive numbers.\n\nPlease set minimum burst time properly."), _("Warning"), wxOK );
    message.ShowModal();
    textFilterMinBurstTime->SetFocus();
    previousWarning = true;
  }

  // No events selected
  if ( !previousWarning && !checkFilterDiscardEventRecords->IsChecked() && listboxFilterEvents->GetCount() == 0 )
  {
    wxMessageDialog message( this, _("Filter:\nThe list of events is empty.\n\nPlease add at least one event or\ndiscard all the event records."), _( "Warning" ), wxOK );
    message.ShowModal();
    buttonFilterAdd->SetFocus();
    previousWarning = true;
  }

  // Too much event lines
  if ( !previousWarning && !checkFilterDiscardEventRecords->IsChecked() && listboxFilterEvents->GetCount() > 20 )
  {
    wxMessageDialog message( this, _("Filter:\nToo much event lines.\n\nPlease delete at least one line."), _( "Warning" ), wxOK );
    message.ShowModal();
    listboxFilterEvents->SetFocus();
    previousWarning = true;
  }
}


void CutFilterDialog::TransferFilterDataToWindow( TraceOptions *traceOptions )
{
  stringstream aux;

  // Discard Records
  checkFilterDiscardStateRecords->SetValue( !traceOptions->get_filter_states() );
  checkFilterDiscardEventRecords->SetValue( !traceOptions->get_filter_events() );
  checkFilterDiscardCommunicationRecords->SetValue( !traceOptions->get_filter_comms() );

  // States
  CheckStatesList( 0, traceOptions->get_all_states() ); // Check or uncheck all
  if( !traceOptions->get_all_states() )
  {
    TraceOptions::TStateNames auxNames;
    for( int i = 0; i < 20; ++i )
      auxNames[ i ] = NULL;
    traceOptions->get_state_names( auxNames );
    CheckStatesList( auxNames );
  }

  aux.str("");
  aux << traceOptions->get_min_state_time();
  textFilterMinBurstTime->SetValue( wxString::FromAscii( aux.str().c_str() ));

  // Events
  checkFilterDiscardListedEvents->SetValue( traceOptions->get_discard_given_types() );

  TraceOptions::TFilterTypes auxEvents;
  traceOptions->get_filter_types( auxEvents );
  SetEventsList( auxEvents, traceOptions->get_filter_last_type() );

  // Communications
  aux.str("");
  aux << traceOptions->get_min_comm_size();
  textFilterSize->SetValue( wxString::FromAscii( aux.str().c_str() ) );
}


void CutFilterDialog::TransferWindowToFilterData( bool previousWarning )
{
  if ( !previousWarning )
  {
    // Discard Records
    traceOptions->set_filter_states( !checkFilterDiscardStateRecords->IsChecked() );
    traceOptions->set_filter_events( !checkFilterDiscardEventRecords->IsChecked() );
    traceOptions->set_filter_comms( !checkFilterDiscardCommunicationRecords->IsChecked() );

    // filter_by_call_time is for all the record types, BUT:
    // 1) it uses a buffer (danger!)
    // 2) it's undocumented 
    // 3) if false => normal behaviour
    // Was it an experimental feature?
    traceOptions->set_filter_by_call_time( false );

    //if ( !checkFilterDiscardStateRecords->IsChecked() )
    //{
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
        
      traceOptions->set_all_states( allStatesSelected );
      if ( allStatesSelected )
      {
#ifdef WIN32
        auxNames[ 0 ] = _strdup( "All" );
#else
        auxNames[ 0 ] = strdup( "All" );
#endif
      }
      else
      {
        // Read selected states and fill vector
        int pos = 0;

        for( size_t i = 0; i < checkListFilterStates->GetCount(); ++i )
        {
          if ( checkListFilterStates->IsChecked( i ) )
          {
#ifdef WIN32
            auxNames[ pos++ ] = _strdup( (char *)checkListFilterStates->GetString( i ).c_str());
#elif defined UNICODE
            auxNames[ pos++ ] = strdup( (char *)(checkListFilterStates->GetString( i ).mb_str().data() ));
#else
            auxNames[ pos++ ] = strdup( (char *)(checkListFilterStates->GetString( i ).mb_str() ));
#endif
          }
        }

        if( pos == 0 )
        {
#ifdef WIN32
          auxNames[ 0 ] = _strdup( "Running" );
#else
          auxNames[ 0 ] = strdup( "Running" );
#endif
        }

        traceOptions->set_state_names( auxNames );

        unsigned long auxULong;
        textFilterMinBurstTime->GetValue().ToULong( &auxULong );
        traceOptions->set_min_state_time( (unsigned long long)auxULong );
      }
    //}

    //if ( !checkFilterDiscardEventRecords->IsChecked() )
    //{
      traceOptions->set_discard_given_types( checkFilterDiscardListedEvents->IsChecked() );
      TraceOptions::TFilterTypes auxEvents;
      int lastType = 0;
      GetEventsList( auxEvents, lastType );

      // To fix: always do set_filter_last_type first than set_filter_types
      traceOptions->set_filter_last_type( lastType );
      traceOptions->set_filter_types( auxEvents );
    //}

    //if ( !checkFilterDiscardCommunicationRecords->IsChecked() )
    //{
      traceOptions->set_min_comm_size( textFilterSize->GetValue() );
    //}
  }
}


// **********************************************************************************
// **********************************************************************************
//  ____         __ _                           ____                  _                
// / ___|  ___  / _| |___      ____ _ _ __ ___ / ___|___  _   _ _ __ | |_ ___ _ __ ___ 
// \___ \ / _ \| |_| __\ \ /\ / / _` | '__/ _ \ |   / _ \| | | | '_ \| __/ _ \ '__/ __|
//  ___) | (_) |  _| |_ \ V  V / (_| | | |  __/ |__| (_) | |_| | | | | ||  __/ |  \__ \
// |____/ \___/|_|  \__| \_/\_/ \__,_|_|  \___|\____\___/ \__,_|_| |_|\__\___|_|  |___/
//                                                                                    
// **********************************************************************************
// **********************************************************************************


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


bool CutFilterDialog::SetSoftwareCountersEventsListToString( string listEvents, wxListBox *selectedEvents )
{
  selectedEvents->Clear();
  stringstream auxList( listEvents );
  while( !auxList.eof() )
  {
    string tmpStr;

    std::getline( auxList, tmpStr, ';' );
    selectedEvents->Append( wxString( tmpStr.c_str(), wxConvUTF8 ).Trim(true).Trim(false) );
  }

  return true;
}



char *CutFilterDialog::GetSoftwareCountersEventsListToString( wxListBox *selectedEvents )
{
  string listStr;

  for( size_t i = 0; i < selectedEvents->GetCount(); ++i )
  {
#ifdef UNICODE
    string auxLineStr = string( selectedEvents->GetString( i ).mb_str().data() );
#else
    string auxLineStr = string( selectedEvents->GetString( i ).mb_str() );
#endif
    listStr += auxLineStr;
    if ( i != selectedEvents->GetCount() - 1 )
      listStr += string(";");
  }

#ifdef WIN32
  return _strdup( listStr.c_str() );
#else
  return strdup( listStr.c_str() );
#endif
}


void CutFilterDialog::CheckSoftwareCountersOptions( bool &previousWarning )
{
  // Time region selected?
  if ( !previousWarning && radioSCOnIntervals->GetValue() && textSCSamplingInterval->GetValue() == _("") )
  {
    wxMessageDialog message( this, _("Software Counters:\nPlease set the sampling interval time."), _( "Warning" ), wxOK );
    message.ShowModal();
    textSCSamplingInterval->SetFocus();
    previousWarning = true;
  }

  if ( !previousWarning && radioSCOnStates->GetValue() && textSCMinimumBurstTime->GetValue() == _("")  )
  {
    wxMessageDialog message( this, _("Software Counters:\nPlease set the minimum burst time."), _("Warning"), wxOK );
    message.ShowModal();
    textSCMinimumBurstTime->SetFocus();
    previousWarning = true;
  }

  // Are times set properly?
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
      wxMessageDialog message( this, _("Software Counters:\nTimes must be positive numbers.\n\nPlease set sampling interval burst time properly."), _("Warning"), wxOK );
      message.ShowModal();
      textSCSamplingInterval->SetFocus();
    }
    else
    {
      wxMessageDialog message( this, _("Software Counters:\nTimes must be positive numbers.\n\nPlease set minimum burst time properly."), _("Warning"), wxOK );
      message.ShowModal();
      textSCMinimumBurstTime->SetFocus();
    }
    previousWarning = true;
  }
  // Empty list of events?
  if ( !previousWarning && listSCSelectedEvents->GetCount() == 0 )
  {
    wxMessageDialog message( this, _("Software Counters:\nThe list of event types is empty.\n\nPlease add at least one event type."), _("Warning"), wxOK );
    message.ShowModal();
    buttonSCSelectedEventsAdd->SetFocus();
    previousWarning = true;
  }
}


void CutFilterDialog::TransferWindowToSoftwareCountersData( bool previousWarning )
{
  unsigned long auxULong;

  if ( !previousWarning )
  {
    // Region
    traceOptions->set_sc_onInterval( radioSCOnIntervals->GetValue() );
    
    textSCSamplingInterval->GetValue().ToULong( &auxULong );
    traceOptions->set_sc_sampling_interval( (unsigned long long)auxULong );
    textSCMinimumBurstTime->GetValue().ToULong( &auxULong );
    traceOptions->set_sc_minimum_burst_time( (unsigned long long)auxULong );

    // Selected events
    traceOptions->set_sc_types( GetSoftwareCountersEventsListToString( listSCSelectedEvents ) );

    // Options
    traceOptions->set_sc_acumm_counters( radioSCAccumulateValues->GetValue() );

    traceOptions->set_sc_remove_states( checkSCRemoveStates->IsChecked() );
    traceOptions->set_sc_summarize_states( checkSCSummarizeUseful->IsChecked() );
    traceOptions->set_sc_global_counters( checkSCGlobalCounters->IsChecked() );
    traceOptions->set_sc_only_in_bursts( checkSCOnlyInBurstsCounting->IsChecked() );

    // Keep events
    traceOptions->set_sc_types_kept( GetSoftwareCountersEventsListToString( listSCKeepEvents ));

    // Experimental feature?
    // traceOptions->set_sc_frequency( (int) scFrequency );
  }
}


void CutFilterDialog::TransferSoftwareCountersDataToWindow( TraceOptions *traceOptions )
{
  stringstream aux;

  // Region
  if ( traceOptions->get_sc_onInterval() )
    radioSCOnIntervals->SetValue( true );
  else
    radioSCOnStates->SetValue( true );

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
  radioSCAccumulateValues->SetValue( traceOptions->get_sc_acumm_counters() );
  checkSCRemoveStates->SetValue( traceOptions->get_sc_remove_states() );
  checkSCSummarizeUseful->SetValue( traceOptions->get_sc_summarize_states() );
  checkSCGlobalCounters->SetValue( traceOptions->get_sc_global_counters() );
  checkSCOnlyInBurstsCounting->SetValue( traceOptions->get_sc_only_in_bursts() );

  // Keep events
  done = SetSoftwareCountersEventsListToString( string( traceOptions->get_sc_types_kept() ),
                                                listSCKeepEvents );

  // Experimental feature?
  // traceOptions->set_sc_frequency( (int) scFrequency );
}


// **********************************************************************************
// **********************************************************************************
//   ____                                      
//  / ___|___  _ __ ___  _ __ ___   ___  _ __  
// | |   / _ \| '_ ` _ \| '_ ` _ \ / _ \| '_ \ 
// | |__| (_) | | | | | | | | | | | (_) | | | |
//  \____\___/|_| |_| |_|_| |_| |_|\___/|_| |_|
//                                             
// **********************************************************************************
// **********************************************************************************
// order only contains the identifiers of the selected tools
void CutFilterDialog::TransferCommonDataToWindow( vector< string > order )
{

  if( order.size() > 0 )
  {
    vector< string > auxListToolOrder; // To build the new list, in new order

    // Fill new list, in order, and keeping the non-selected in listToolOrder
    for( size_t i = 0; i < order.size(); ++i )
    {
      auxListToolOrder.push_back( order[i] );
      listToolOrder.erase( find( listToolOrder.begin(), listToolOrder.end(), order[i]));
    }

    // Add the non-selected to the end of the vector and remember if they were checked
    //vector<bool> checkedNotUsedTools;
    for( size_t i = 0; i < listToolOrder.size(); ++i )
    {
      auxListToolOrder.push_back( listToolOrder[i] );
    /*
      for( size_t j = 0; j < checkListExecutionChain->GetCount(); ++j )
      {
        wxString currentCheckName = checkListExecutionChain->GetString( j );
        wxString currentCheckNameClean = currentCheckName.Mid( currentCheckName.Find( wxChar(' '))).Trim(false);
        if( currentCheckNameClean == wxString( listToolOrder[i].c_str(),  wxConvUTF8 ) )
          checkedNotUsedTools.push_back( checkListExecutionChain->IsChecked( j ) );
      }
    */
    }

    // Set the new vector
    listToolOrder.swap( auxListToolOrder );
    UpdateExecutionChain();

    // Check the selected, that fit in the firsts elems of the vector order
    for( size_t i = 0; i < order.size(); ++i )
    {
      checkListExecutionChain->Check( i, true );
    }
    
    // But keep the remembered check state of the unused tools.
    //int j = 0;
    //for( size_t i = order.size(); i < order.size() + checkedNotUsedTools.size(); ++i )
    for( size_t i = order.size(); i < order.size() + checkListExecutionChain->GetCount(); ++i )
    {
    //  checkListExecutionChain->Check( i, checkedNotUsedTools[ j++ ] );
      checkListExecutionChain->Check( i, false );
    }

    // Finally, select first one to allow UpdateUi raise it properly.
    checkListExecutionChain->SetSelection( 0 );
  }
}




/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SAVE_XML
 */

void CutFilterDialog::OnButtonSaveXmlClick( wxCommandEvent& event )
{
  // first, test if all parameters are ready
  bool previousWarning = false;

  //CheckCommonOptions( previousWarning, true );
  //TransferWindowToCommonData( previousWarning );
  TransferToolOrderToCommonData();

  if ( !previousWarning )
  {
    // Which tools are selected?
    for ( vector< string >::const_iterator it = filterToolOrder.begin(); it != filterToolOrder.end(); ++it )
    {
      if ( *it == TraceCutter::getID() )
      {
        CheckCutterOptions( previousWarning );
        TransferWindowToCutterData( previousWarning );
      }
      else if ( *it == TraceFilter::getID() )
      {
        CheckFilterOptions( previousWarning );
        TransferWindowToFilterData( previousWarning );
      }
      else if ( *it == TraceSoftwareCounters::getID() )
      {
        CheckSoftwareCountersOptions( previousWarning );
        TransferWindowToSoftwareCountersData( previousWarning );
      }
      else
      {
      }
    }
  }

  if ( !previousWarning )
  {
    wxFileName auxDirectory( wxString( globalXMLsPath.c_str(), wxConvUTF8 )); 

    if( !auxDirectory.IsDir() )
      auxDirectory = auxDirectory.GetPathWithSep();

    wxString directory( auxDirectory.GetFullPath() );

    wxFileDialog xmlSelectionDialog( this,
                          _( "Save XML Cut/Filter configuration file" ),
                          directory,
                          _( "" ), 
                          _( "XML configuration file (*.xml)|*.xml|All files (*.*)|*.*" ),
                          wxFD_SAVE|wxFD_CHANGE_DIR|wxFD_OVERWRITE_PROMPT );

    if( xmlSelectionDialog.ShowModal() == wxID_OK )
    {
      wxString path = xmlSelectionDialog.GetPath();
      wxString xmlSuffix = _(".xml");
      wxString pathWithExtension;
      
      if ( path.EndsWith( xmlSuffix )) 
        pathWithExtension = path;
      else
        pathWithExtension = path + xmlSuffix;

      wxString selectedDir = wxFileName( pathWithExtension ).GetPathWithSep();
      if ( wxFileName::IsDirWritable( selectedDir ) )
      {
        traceOptions->saveXML( filterToolOrder, string( pathWithExtension.mb_str()) );

        // we must add the proper slash to enter the directory next time
        globalXMLsPath = string( xmlSelectionDialog.GetDirectory().mb_str() ) + PATH_SEP;
        newXMLsPath = true;

        fileBrowserButtonXML->SetPath( pathWithExtension );
      }
      else
      {
         wxMessageDialog message(
                this,
                _("Unable to write xml file in directory ") + selectedDir + _(".\nPlease check permissions."),
                _("Warning"),
                wxOK );
         message.ShowModal();
      }
    }
  }
}


bool CutFilterDialog::GetLoadedXMLPath( string &XMLPath )
{
  if ( newXMLsPath )
  {
    XMLPath = globalXMLsPath;
  }

  return newXMLsPath;
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON
 */

void CutFilterDialog::OnBitmapbuttonPushDownFilterClick( wxCommandEvent& event )
{
  int lastItemSelected = checkListExecutionChain->GetSelection();

  if ( lastItemSelected != wxNOT_FOUND && lastItemSelected < 2  && lastItemSelected > -1 )
  {
    // Save current check state, because UpdateToolList clears it
    vector< bool > checked;
    for( unsigned int i = 0; i < listToolOrder.size(); ++i )
      checked.push_back( checkListExecutionChain->IsChecked( i ) );

    // Get check state of items to swap
    bool auxFirst  = checkListExecutionChain->IsChecked( lastItemSelected );
    bool auxSecond = checkListExecutionChain->IsChecked( lastItemSelected + 1 );

    // Swap the names in the string
    string auxNameFirst = listToolOrder[ lastItemSelected ];
    listToolOrder[ lastItemSelected ] =  listToolOrder[ lastItemSelected + 1 ];
    listToolOrder[ lastItemSelected + 1 ] = auxNameFirst;

    // Rebuild list
    UpdateExecutionChain();

    // Recover and change check state
    for( unsigned int i = 0; i < listToolOrder.size(); ++i )
      checkListExecutionChain->Check( i, checked[ i ] );

    checkListExecutionChain->Check( lastItemSelected, auxSecond );
    checkListExecutionChain->Check( lastItemSelected + 1, auxFirst );

    // Keep the selection
    checkListExecutionChain->SetSelection( ++lastItemSelected );

    setOutputName( globalEnable(), false, std::string( fileBrowserButtonInputTrace->GetPath().mb_str() ) );
  }
}


const vector< string > CutFilterDialog::changeToolsNameToID( const vector< string >& listToolWithNames )
{
  vector< string > listToolWithIDs;
  for ( vector< string >::const_iterator it = listToolWithNames.begin(); it != listToolWithNames.end(); ++it )
  {
    listToolWithIDs.push_back( GetLocalKernel()->getToolID( *it ) );
  }

  return listToolWithIDs;
}


const vector< string > CutFilterDialog::changeToolsIDsToNames( const vector< string >& listToolIDs )
{
  vector< string > listToolWithNames;
  for ( vector< string >::const_iterator it = listToolIDs.begin(); it != listToolIDs.end(); ++it )
  {
    listToolWithNames.push_back( GetLocalKernel()->getToolName( *it ) );
  }

  return listToolWithNames;
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_EDIT_XML
 */

void CutFilterDialog::OnButtonViewEditXmlUpdate( wxUpdateUIEvent& event )
{
  bool enableEdition = buttonViewEditXml->GetValue();

  notebookTools->Enable( enableEdition );
  buttonSaveXml->Enable( changedXMLParameters && enableEdition );

  txtExecutionChain->Enable( enableEdition );
  checkListExecutionChain->Enable( enableEdition );
  buttonUp->Enable( enableEdition );
  buttonDown->Enable( enableEdition );
}


bool CutFilterDialog::isExecutionChainEmpty()
{
  // Any tool selected?
  bool emptyChain = true;

  for ( size_t i = 0; i < checkListExecutionChain->GetCount(); ++i )
  {
    if ( checkListExecutionChain->IsChecked( (int)i ) )
    {
      emptyChain = false;
      break;
    }
  }

  return emptyChain;
}


void CutFilterDialog::ChangePageSelectionFromTabsToToolsOrderList()
{
  int pos = 0;

  for( vector< string >::iterator it = listToolOrder.begin(); it != listToolOrder.end(); ++it )
  {
    if ( *it == string( notebookTools->GetPageText( notebookTools->GetSelection() ).mb_str()) )
    {
      checkListExecutionChain->SetSelection( pos );
      break; // you can't have two of them at the same time
    }
    pos++;
  }
}


/*!
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_CHECKLISTBOX
 */

void CutFilterDialog::OnCheckListExecutionChainSelected( wxCommandEvent& event )
{
  int iSel = event.GetSelection();
  
  if ( iSel > -1 )
    ChangePageSelectionFromToolsOrderListToTabs( iSel );
}


/*!
 * wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED event handler for ID_NOTEBOOK_CUT_FILTER_OPTIONS
 */

void CutFilterDialog::OnNotebookCutFilterOptionsPageChanged( wxNotebookEvent& event )
{
  ChangePageSelectionFromTabsToToolsOrderList();
}


bool CutFilterDialog::globalEnable( const string& auxInputTrace )
{
  return ( isFileSelected( auxInputTrace ) && !isExecutionChainEmpty() );
}


bool CutFilterDialog::globalEnable()
{
  return ( isFileSelected( fileBrowserButtonInputTrace ) && !isExecutionChainEmpty() );
}


// saveGeneratedName keeps it internally ( see getNewTraceName )
// saveGeneratedName = false => useful to compute new ouput trace name,
//   (i.e. when filter execution chain modified ).
// saveGeneratedName = true => just before real trace save
void CutFilterDialog::setOutputName( bool enable,
                                     bool saveGeneratedName,
                                     const string& sourceTrace )
{
  if ( enable )
  {
    TransferToolOrderToCommonData();

    string currentDstTrace =
            GetLocalKernel()->getNewTraceName(
                    sourceTrace, outputPath, filterToolOrder, saveGeneratedName );
    wxString outputName = wxString( currentDstTrace.c_str(), wxConvUTF8 );
    fileBrowserButtonOutputTrace->SetPath( outputName );

    outputPath = std::string( wxFileName( wxString( currentDstTrace.c_str(), wxConvUTF8 ) ).GetPathWithSep().mb_str() );
  }
}

/*!
 * wxEVT_COMMAND_LISTBOX_DOUBLECLICKED event handler for ID_CHECKLISTBOX_EXECUTION_CHAIN
 */

void CutFilterDialog::OnChecklistboxExecutionChainDoubleClicked( wxCommandEvent& event )
{
  int iSel = event.GetSelection();
  if ( iSel > -1 )
  {
    checkListExecutionChain->Check( iSel, !checkListExecutionChain->IsChecked( iSel ) );
    enableOutputTraceWidgets( globalEnable() );
    setOutputName( globalEnable(), false, std::string( fileBrowserButtonInputTrace->GetPath().mb_str() ) );

    EnableSingleTab( iSel );
    ChangePageSelectionFromToolsOrderListToTabs( iSel );
  }
}


void CutFilterDialog::enableOutputTraceWidgets( bool enable )
{
  txtOutputTrace->Enable( enable );
  fileBrowserButtonOutputTrace->Enable( enable );
  checkLoadResultingTrace->Enable( enable );
  checkRunAppWithResultingTrace->Enable( enable );
}


/*!
 * wxEVT_COMMAND_CHECKLISTBOX_TOGGLED event handler for ID_CHECKLISTBOX_EXECUTION_CHAIN
 */

void CutFilterDialog::OnChecklistboxExecutionChainToggled( wxCommandEvent& event )
{
  int iSel = event.GetSelection();

  if ( iSel > -1 )
  {
    UpdateOutputTraceName();
    EnableToolTab( iSel );
  }
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON_PUSH_UP
 */

void CutFilterDialog::OnBitmapbuttonPushUpFilterClick( wxCommandEvent& event )
{
  int lastItemSelected = checkListExecutionChain->GetSelection();

  if ( lastItemSelected != wxNOT_FOUND && lastItemSelected > 0 )
  {
    // Save current check state, because UpdateExecutionChain clears it
    vector< bool > checked;
    for( unsigned int i = 0; i < listToolOrder.size(); ++i )
      checked.push_back( checkListExecutionChain->IsChecked( i ) );

    // Get check state of items to swap
    bool auxFirst  = checkListExecutionChain->IsChecked( lastItemSelected - 1 );
    bool auxSecond = checkListExecutionChain->IsChecked( lastItemSelected );

    // Swap the names in the string
    string auxNameFirst = listToolOrder[ lastItemSelected - 1 ];
    listToolOrder[ lastItemSelected - 1 ] =  listToolOrder[ lastItemSelected ];
    listToolOrder[ lastItemSelected ] = auxNameFirst;

    // Rebuild list
    UpdateExecutionChain();

    // Recover and change check state
    for( unsigned int i = 0; i < listToolOrder.size(); ++i )
      checkListExecutionChain->Check( i, checked[ i ] );

    checkListExecutionChain->Check( lastItemSelected - 1, auxSecond );
    checkListExecutionChain->Check( lastItemSelected, auxFirst );

    // Keep the selection
    checkListExecutionChain->SetSelection( --lastItemSelected );

    setOutputName( globalEnable(), false, std::string( fileBrowserButtonInputTrace->GetPath().mb_str() ) );
  }
}


void CutFilterDialog::UpdateExecutionChain()
{
  wxArrayString items;
  int order = 1;
  for( vector< string >::iterator it = listToolOrder.begin(); it != listToolOrder.end(); ++it )
  {
    stringstream aux;
    aux << order++;
    items.Add(  wxString::FromAscii( aux.str().c_str() ) + _( ".- " ) + wxString::FromAscii( (*it).c_str() ) );
  }

  checkListExecutionChain->Clear();
  checkListExecutionChain->InsertItems( items, 0 );
}


void CutFilterDialog::ChangePageSelectionFromToolsOrderListToTabs( int selected )
{
  for( size_t i = 0; i < notebookTools->GetPageCount(); ++i )
  {
    if ( listToolOrder[ selected ] == string( notebookTools->GetPageText( i ).mb_str()) )
    {
      notebookTools->ChangeSelection( i );
    }
  }
}


void CutFilterDialog::EnableSingleTab( int selected )
{
  string id = GetLocalKernel()->getToolID( listToolOrder[ selected ] );
  int iTab = TABINDEX[ id ];
  bool isChecked = checkListExecutionChain->IsChecked( selected );
  (notebookTools->GetPage( iTab ))->Enable( isChecked );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHECKLISTBOX
 */
// PRECOND: selected is an index to listToolOrder/checkListExecutionChain
void CutFilterDialog::EnableAllTabsFromToolsList()
{
  for( size_t i = 0; i < listToolOrder.size(); ++i )
  {
    EnableSingleTab( (int)i );
  }
}


void CutFilterDialog::CheckCommonOptions( bool &previousWarning, bool showWarning )
{
  // Any trace selected?
  if ( !previousWarning && !isFileSelected( fileBrowserButtonInputTrace ) )
  {
    if ( showWarning )
    {
      wxMessageDialog message( this, _("Missing trace name.\nPlease choose the source trace."), _("Warning"), wxOK );
      message.ShowModal();
    }

    fileBrowserButtonInputTrace->SetFocus();
    previousWarning = true;
  }

  if ( !previousWarning && !isFileSelected( fileBrowserButtonOutputTrace ) )
  {
    if ( showWarning )
    {
      wxMessageDialog message( this, _("Missing trace name.\nPlease choose name for final trace."), _("Warning"), wxOK );
      message.ShowModal();
    }

    fileBrowserButtonOutputTrace->SetFocus();
    previousWarning = true;
  }

  if ( !previousWarning && isExecutionChainEmpty() )
  {
    if ( showWarning )
    {
      wxMessageDialog message( this, _("No utility selected.\nPlease choose the utilities to apply."), _( "Warning" ), wxOK );
      message.ShowModal();
    }

    checkListExecutionChain->SetFocus();
    previousWarning = true;
  }
}


void CutFilterDialog::TransferToolOrderToCommonData()
{
  filterToolOrder.clear();

  for ( size_t i = 0; i < listToolOrder.size(); ++i )
  {
    if ( checkListExecutionChain->IsChecked( i ) )
    {
      filterToolOrder.push_back(
              GetLocalKernel()->getToolID( listToolOrder[ i ] ));
    }
  }
}


void CutFilterDialog::TransferWindowToCommonData( bool previousWarning )
{
  if ( !previousWarning )
  {
    nameSourceTrace = std::string( fileBrowserButtonInputTrace->GetPath().mb_str() );
    nameDestinyTrace = std::string( fileBrowserButtonOutputTrace->GetPath().mb_str() );
    loadResultingTrace = checkLoadResultingTrace->IsChecked();
    runAppWithResultingTrace = checkRunAppWithResultingTrace->IsChecked();

    TransferToolOrderToCommonData();
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for wxID_APPLY
 */

void CutFilterDialog::OnApplyUpdate( wxUpdateUIEvent& event )
{
  buttonApply->Enable( globalEnable() );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_APPLY
 */

void CutFilterDialog::OnApplyClick( wxCommandEvent& event )
{
  // To avoid annoying multiple warning windows at the same time, and also final filter creation
  bool previousWarning = false;
  
  CheckCommonOptions( previousWarning, true );
  TransferWindowToCommonData( previousWarning );

  if ( !previousWarning )
  {
    // Which tools are selected?
    for ( vector< string >::const_iterator it = filterToolOrder.begin(); it != filterToolOrder.end(); ++it )
    {
      if ( *it == TraceCutter::getID() )
      {
        CheckCutterOptions( previousWarning );
        TransferWindowToCutterData( previousWarning );
      }
      else if ( *it == TraceFilter::getID() )
      {
        CheckFilterOptions( previousWarning );
        TransferWindowToFilterData( previousWarning );
      }
      else if ( *it == TraceSoftwareCounters::getID() )
      {
        CheckSoftwareCountersOptions( previousWarning );
        TransferWindowToSoftwareCountersData( previousWarning );
      }
      else
      {
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


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_SAVE_XML
 */

void CutFilterDialog::OnButtonSaveXmlUpdate( wxUpdateUIEvent& event )
{
  buttonSaveXml->Enable( !isExecutionChainEmpty() );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHECKBOX_FILTER_DISCARD_STATE
 */

void CutFilterDialog::OnCheckboxFilterDiscardStateUpdate( wxUpdateUIEvent& event )
{
  staticBoxSizerFilterStates->Enable( !checkFilterDiscardStateRecords->IsChecked() );
  checkListFilterStates->Enable( !checkFilterDiscardStateRecords->IsChecked() );
  buttonFilterSelectAll->Enable( !checkFilterDiscardStateRecords->IsChecked() );
  buttonFilterUnselectAll->Enable( !checkFilterDiscardStateRecords->IsChecked() );
  labelFilterMinBurstTime->Enable( !checkFilterDiscardStateRecords->IsChecked() );
  textFilterMinBurstTime->Enable( !checkFilterDiscardStateRecords->IsChecked() );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHECKBOX_FILTER_DISCARD_EVENT
 */

void CutFilterDialog::OnCheckboxFilterDiscardEventUpdate( wxUpdateUIEvent& event )
{
  staticBoxSizerFilterEvents->Enable( !checkFilterDiscardEventRecords->IsChecked() );
  listboxFilterEvents->Enable( !checkFilterDiscardEventRecords->IsChecked() );
  buttonFilterAdd->Enable( !checkFilterDiscardEventRecords->IsChecked() );
  buttonFilterDelete->Enable( !checkFilterDiscardEventRecords->IsChecked() );
  checkFilterDiscardListedEvents->Enable( !checkFilterDiscardEventRecords->IsChecked() );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHECKBOX_FILTER_DISCARD_COMMUNICATION
 */

void CutFilterDialog::OnCheckboxFilterDiscardCommunicationUpdate( wxUpdateUIEvent& event )
{
  staticBoxSizerFilterCommunications->Enable( !checkFilterDiscardCommunicationRecords->IsChecked() );
  staticTextFilterSize->Enable( !checkFilterDiscardCommunicationRecords->IsChecked() );
  textFilterSize->Enable( !checkFilterDiscardCommunicationRecords->IsChecked() );
  staticTextFilterSizeUnit->Enable( !checkFilterDiscardCommunicationRecords->IsChecked() );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHECKBOX_CHECK_CUTTER_ORIGINAL_TIME
 */

void CutFilterDialog::OnCheckboxCheckCutterOriginalTimeUpdate( wxUpdateUIEvent& event )
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


void CutFilterDialog::SetXMLFile( const wxString& whichXMLFile, bool refresh )
{
  wxString xmlSuffix = _(".xml");
  wxString pathWithExtension;
      
  if ( whichXMLFile.EndsWith( xmlSuffix )) 
  {
    wxString tmpFile = wxFileName( whichXMLFile ).GetFullPath();
    if ( wxFileName::IsFileReadable( tmpFile ) )
    {
      fileBrowserButtonXML->SetPath( tmpFile );
      
      if ( refresh )
        TransferXMLFileToWindow( tmpFile );
    }
  }
}


void CutFilterDialog::TransferDataToWindow( vector< string > order, TraceOptions* traceOptions )
{
  Freeze();

  vector< string > toolsName = changeToolsIDsToNames( order );
  TransferCommonDataToWindow( toolsName );

  for( size_t i = 0; i < order.size(); ++i )
  {
    if ( order[ i ] == TraceCutter::getID() )
    {
      TransferCutterDataToWindow( traceOptions );
    }
    else if ( order[ i ] == TraceFilter::getID() )
    {
      TransferFilterDataToWindow( traceOptions );
    }
    else if ( order[ i ] == TraceSoftwareCounters::getID() )
    {
      TransferSoftwareCountersDataToWindow( traceOptions );
    }
    else
    {
    }
  }
  
  Thaw();  
}


void CutFilterDialog::UpdateGuiXMLSectionFromFile( TraceOptions *traceOptions,
                                                    vector< string > &toolIDsOrder )
{
  TransferDataToWindow( toolIDsOrder, traceOptions );
  EnableAllTabsFromToolsList();
  ChangePageSelectionFromToolsOrderListToTabs( 0 );
}


void CutFilterDialog::UpdateGlobalXMLPath( const wxString& whichPath )
{
 // we must add the proper slash to enter the directory next time
  //globalXMLsPath = string( xmlSelectionDialog.GetDirectory().mb_str() ) + PATH_SEP;
  wxFileName auxDirectory( whichPath );
  if( !auxDirectory.IsDir() )
    auxDirectory = auxDirectory.GetPathWithSep();
  wxString directory( auxDirectory.GetFullPath() );

  globalXMLsPath = string( directory.mb_str() ) + PATH_SEP;
  newXMLsPath = true;
}


void CutFilterDialog::EnableToolTab( int i )
{
  EnableSingleTab( i );
  ChangePageSelectionFromToolsOrderListToTabs( i );
}


void CutFilterDialog::UpdateOutputTraceName()
{
  bool allowChangeOutputTrace = globalEnable();
  if ( allowChangeOutputTrace )
  {
    enableOutputTraceWidgets( allowChangeOutputTrace );
    setOutputName( allowChangeOutputTrace,
                   false,
                   std::string( fileBrowserButtonInputTrace->GetPath().mb_str() ) );
  }
}


// Needed by sequence!
void CutFilterDialog::TransferTraceOptionsToWindow( TraceOptions *traceOptions, 
                                                     vector< string > &whichToolIDsOrder )
{
  UpdateGuiXMLSectionFromFile( traceOptions, whichToolIDsOrder );
}


void CutFilterDialog::TransferXMLFileToWindow( const wxString& whichXMLFile )
{
  if ( traceOptions != NULL )
  {
    delete traceOptions;
    traceOptions = TraceOptions::create( GetLocalKernel() );
  }
  
  vector< string > toolIDsOrder = traceOptions->parseDoc( (char *)whichXMLFile.c_str() );

  UpdateGuiXMLSectionFromFile( traceOptions, toolIDsOrder );
  UpdateGlobalXMLPath( whichXMLFile );

  // Needs filterToolOrder updated
  UpdateOutputTraceName();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_CUT_FILTER_XML
 */
void CutFilterDialog::OnTextctrlCutFilterXmlTextUpdated( wxCommandEvent& event )
{
  TransferXMLFileToWindow( fileBrowserButtonXML->GetPath() );
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_CUT_FILTER_INPUT_TRACE
 */

void CutFilterDialog::OnTextctrlCutFilterInputTraceTextUpdated( wxCommandEvent& event )
{
  wxString tmpPath( textCtrlInputTrace->GetValue() );

  //wxFileName auxDirectory( event.GetPath() );
  wxFileName auxDirectory( tmpPath );
  if( !auxDirectory.IsDir() )
    auxDirectory = auxDirectory.GetPathWithSep();
  wxString directory( auxDirectory.GetFullPath() );
  outputPath = string( directory.mb_str() );

  //string auxSourceTrace = string( event.GetPath().mb_str() ); // why? Widget still empty!!
  string auxSourceTrace = string( tmpPath.mb_str() ); // why? Widget still empty!!

  bool localEnable = globalEnable( auxSourceTrace );
  enableOutputTraceWidgets( localEnable );
  setOutputName( localEnable, false, auxSourceTrace );
}

