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

#include <sstream>
#include <iostream>


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
  EVT_BUTTON( ID_BITMAPBUTTON_PUSH_UP, CutFilterDialog::OnBitmapbuttonPushUpClick )

  EVT_BUTTON( ID_BITMAPBUTTON_PUSH_DOWN, CutFilterDialog::OnBitmapbuttonPushDownClick )

  EVT_CHECKBOX( ID_CHECKBOX_CHECK_ORIGINAL_TIME, CutFilterDialog::OnCheckOriginalTimeClick )

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
  radioButtonCutByTime = NULL;
  radioButtonCutByPercent = NULL;
  textBeginCut = NULL;
  textEndCut = NULL;
  buttonSelectRegion = NULL;
  buttonAllTrace = NULL;
  checkUseOriginalTime = NULL;
  checkRemoveFirstState = NULL;
  checkBreakStates = NULL;
  checkRemoveLastState = NULL;
  textMaximumTraceSize = NULL;
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

  wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Trace"), wxDefaultPosition, wxDefaultSize, 0 );
  if (CutFilterDialog::ShowToolTips())
    itemStaticText4->SetToolTip(_("Trace that will be used by the Cut/Filter toolkit."));
  itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  filePickerTrace = new wxFilePickerCtrl( itemDialog1, ID_FILECTRL_CUTFILTER_TRACE_SELECTION, wxEmptyString, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
  if (CutFilterDialog::ShowToolTips())
    filePickerTrace->SetToolTip(_("Trace that will be used by the Cut/Filter toolkit."));
  itemBoxSizer3->Add(filePickerTrace, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer3->Add(itemBoxSizer6, 1, wxGROW, 5);

  wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("  Ordered execution"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer6->Add(itemStaticText7, 0, wxALIGN_LEFT|wxLEFT|wxTOP, 5);

  wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer6->Add(itemBoxSizer8, 1, wxGROW|wxALL, 5);

  wxArrayString checkListToolOrderStrings;
  checkListToolOrder = new wxCheckListBox( itemDialog1, ID_CHECKLISTBOX, wxDefaultPosition, wxDefaultSize, checkListToolOrderStrings, wxLB_SINGLE|wxLB_NEEDED_SB );
  if (CutFilterDialog::ShowToolTips())
    checkListToolOrder->SetToolTip(_("Select the order of the Cut/Filter tools."));
  itemBoxSizer8->Add(checkListToolOrder, 3, wxGROW|wxLEFT|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer8->Add(itemBoxSizer10, 1, wxALIGN_CENTER_VERTICAL, 5);

  buttonUp = new wxBitmapButton( itemDialog1, ID_BITMAPBUTTON_PUSH_UP, itemDialog1->GetBitmapResource(wxT("arrow_up.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (CutFilterDialog::ShowToolTips())
    buttonUp->SetToolTip(_("Select the order of the Cut/Filter tools."));
  itemBoxSizer10->Add(buttonUp, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonDown = new wxBitmapButton( itemDialog1, ID_BITMAPBUTTON_PUSH_DOWN, itemDialog1->GetBitmapResource(wxT("arrow_down.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (CutFilterDialog::ShowToolTips())
    buttonDown->SetToolTip(_("Select the order of the Cut/Filter tools."));
  itemBoxSizer10->Add(buttonDown, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxNotebook* itemNotebook13 = new wxNotebook( itemDialog1, ID_NOTEBOOK_CUT_FILTER_OPTIONS, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

  wxPanel* itemPanel14 = new wxPanel( itemNotebook13, ID_PANEL_CUTTER, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxVERTICAL);
  itemPanel14->SetSizer(itemBoxSizer15);

  wxStaticBox* itemStaticBoxSizer16Static = new wxStaticBox(itemPanel14, wxID_STATIC, _(" Trace Limits "));
  wxStaticBoxSizer* itemStaticBoxSizer16 = new wxStaticBoxSizer(itemStaticBoxSizer16Static, wxVERTICAL);
  itemBoxSizer15->Add(itemStaticBoxSizer16, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer16->Add(itemBoxSizer17, 0, wxGROW|wxLEFT|wxTOP, 5);
  wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer17->Add(itemBoxSizer18, 1, wxGROW|wxLEFT|wxTOP, 5);
  radioButtonCutByTime = new wxRadioButton( itemPanel14, ID_RADIOBUTTON, _("Cut by time"), wxDefaultPosition, wxDefaultSize, 0 );
  radioButtonCutByTime->SetValue(true);
  itemBoxSizer18->Add(radioButtonCutByTime, 1, wxALIGN_LEFT|wxLEFT|wxTOP, 5);

  radioButtonCutByPercent = new wxRadioButton( itemPanel14, ID_RADIOBUTTON1, _("Cut by time %"), wxDefaultPosition, wxDefaultSize, 0 );
  radioButtonCutByPercent->SetValue(false);
  itemBoxSizer18->Add(radioButtonCutByPercent, 1, wxALIGN_LEFT|wxLEFT|wxTOP, 5);

  wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer17->Add(itemBoxSizer21, 2, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer21->Add(itemBoxSizer22, 0, wxGROW|wxLEFT|wxTOP, 5);
  wxStaticText* itemStaticText23 = new wxStaticText( itemPanel14, wxID_STATIC, _("Begin"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer22->Add(itemStaticText23, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  textBeginCut = new wxTextCtrl( itemPanel14, ID_TEXTCTRL_BEGIN_CUT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer22->Add(textBeginCut, 3, wxGROW|wxLEFT|wxTOP|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer21->Add(itemBoxSizer25, 0, wxGROW|wxLEFT|wxTOP, 5);
  wxStaticText* itemStaticText26 = new wxStaticText( itemPanel14, wxID_STATIC, _("End"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer25->Add(itemStaticText26, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  textEndCut = new wxTextCtrl( itemPanel14, ID_TEXTCTRL_END_CUT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer25->Add(textEndCut, 3, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer28 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer16->Add(itemBoxSizer28, 0, wxGROW|wxALL, 5);
  buttonSelectRegion = new wxButton( itemPanel14, ID_BUTTON_SELECT_REGION, _("Select Region..."), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer28->Add(buttonSelectRegion, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  buttonAllTrace = new wxButton( itemPanel14, ID_BUTTON_ALL_TRACE, _("All Trace"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer28->Add(buttonAllTrace, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  wxStaticBox* itemStaticBoxSizer31Static = new wxStaticBox(itemPanel14, wxID_STATIC, _(" Trace Options "));
  wxStaticBoxSizer* itemStaticBoxSizer31 = new wxStaticBoxSizer(itemStaticBoxSizer31Static, wxVERTICAL);
  itemBoxSizer15->Add(itemStaticBoxSizer31, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer32 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer31->Add(itemBoxSizer32, 0, wxGROW|wxLEFT|wxTOP, 5);
  checkUseOriginalTime = new wxCheckBox( itemPanel14, ID_CHECKBOX_CHECK_ORIGINAL_TIME, _("Use original time"), wxDefaultPosition, wxDefaultSize, 0 );
  checkUseOriginalTime->SetValue(false);
  itemBoxSizer32->Add(checkUseOriginalTime, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  checkRemoveFirstState = new wxCheckBox( itemPanel14, ID_CHECKBOX_REMOVE_FIRST_STATE, _("Remove first state"), wxDefaultPosition, wxDefaultSize, 0 );
  checkRemoveFirstState->SetValue(false);
  itemBoxSizer32->Add(checkRemoveFirstState, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  wxBoxSizer* itemBoxSizer35 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer31->Add(itemBoxSizer35, 0, wxGROW|wxLEFT|wxTOP|wxBOTTOM, 5);
  checkBreakStates = new wxCheckBox( itemPanel14, ID_CHECKBOX_BREAK_STATES, _("Break states"), wxDefaultPosition, wxDefaultSize, 0 );
  checkBreakStates->SetValue(false);
  itemBoxSizer35->Add(checkBreakStates, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  checkRemoveLastState = new wxCheckBox( itemPanel14, ID_CHECKBOX_REMOVE_LAST_STATE, _("Remove last state"), wxDefaultPosition, wxDefaultSize, 0 );
  checkRemoveLastState->SetValue(false);
  itemBoxSizer35->Add(checkRemoveLastState, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  wxStaticBox* itemStaticBoxSizer38Static = new wxStaticBox(itemPanel14, wxID_STATIC, _(" Output Trace "));
  wxStaticBoxSizer* itemStaticBoxSizer38 = new wxStaticBoxSizer(itemStaticBoxSizer38Static, wxHORIZONTAL);
  itemBoxSizer15->Add(itemStaticBoxSizer38, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText39 = new wxStaticText( itemPanel14, wxID_STATIC, _("Maximum trace size"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer38->Add(itemStaticText39, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  textMaximumTraceSize = new wxSpinCtrl( itemPanel14, ID_SPINCTRL_MAXIMUM_SIZE, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
  itemStaticBoxSizer38->Add(textMaximumTraceSize, 3, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticText* itemStaticText41 = new wxStaticText( itemPanel14, wxID_STATIC, _("MB"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer38->Add(itemStaticText41, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  itemNotebook13->AddPage(itemPanel14, _("Cutter"));

  wxPanel* itemPanel42 = new wxPanel( itemNotebook13, ID_PANEL_FILTER, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer43 = new wxBoxSizer(wxVERTICAL);
  itemPanel42->SetSizer(itemBoxSizer43);

  wxStaticBox* itemStaticBoxSizer44Static = new wxStaticBox(itemPanel42, wxID_STATIC, _(" Discard Records "));
  wxStaticBoxSizer* itemStaticBoxSizer44 = new wxStaticBoxSizer(itemStaticBoxSizer44Static, wxVERTICAL);
  itemBoxSizer43->Add(itemStaticBoxSizer44, 0, wxGROW|wxALL, 5);
  wxCheckBox* itemCheckBox45 = new wxCheckBox( itemPanel42, ID_RADIOBUTTON2, _("State"), wxDefaultPosition, wxDefaultSize, 0 );
  itemCheckBox45->SetValue(false);
  itemStaticBoxSizer44->Add(itemCheckBox45, 0, wxGROW|wxALL, 5);

  wxCheckBox* itemCheckBox46 = new wxCheckBox( itemPanel42, ID_RADIOBUTTON3, _("Event"), wxDefaultPosition, wxDefaultSize, 0 );
  itemCheckBox46->SetValue(false);
  itemStaticBoxSizer44->Add(itemCheckBox46, 0, wxGROW|wxALL, 5);

  wxCheckBox* itemCheckBox47 = new wxCheckBox( itemPanel42, ID_RADIOBUTTON4, _("Communication"), wxDefaultPosition, wxDefaultSize, 0 );
  itemCheckBox47->SetValue(false);
  itemStaticBoxSizer44->Add(itemCheckBox47, 0, wxGROW|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer48Static = new wxStaticBox(itemPanel42, wxID_STATIC, _(" States "));
  wxStaticBoxSizer* itemStaticBoxSizer48 = new wxStaticBoxSizer(itemStaticBoxSizer48Static, wxHORIZONTAL);
  itemBoxSizer43->Add(itemStaticBoxSizer48, 1, wxGROW|wxALL, 5);
  wxArrayString itemCheckListBox49Strings;
  wxCheckListBox* itemCheckListBox49 = new wxCheckListBox( itemPanel42, ID_CHECKLISTBOX1, wxDefaultPosition, wxDefaultSize, itemCheckListBox49Strings, wxLB_SINGLE );
  itemStaticBoxSizer48->Add(itemCheckListBox49, 2, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer50 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer48->Add(itemBoxSizer50, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
  wxButton* itemButton51 = new wxButton( itemPanel42, ID_BUTTON2, _("Select all"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer50->Add(itemButton51, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxButton* itemButton52 = new wxButton( itemPanel42, ID_BUTTON3, _("Unselect all"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer50->Add(itemButton52, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxButton* itemButton53 = new wxButton( itemPanel42, ID_BUTTON10, _("Set min time"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer50->Add(itemButton53, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer54Static = new wxStaticBox(itemPanel42, wxID_STATIC, _(" Events "));
  wxStaticBoxSizer* itemStaticBoxSizer54 = new wxStaticBoxSizer(itemStaticBoxSizer54Static, wxHORIZONTAL);
  itemBoxSizer43->Add(itemStaticBoxSizer54, 1, wxGROW|wxALL, 5);
  wxArrayString itemListBox55Strings;
  wxListBox* itemListBox55 = new wxListBox( itemPanel42, ID_LISTBOX, wxDefaultPosition, wxDefaultSize, itemListBox55Strings, wxLB_SINGLE );
  itemStaticBoxSizer54->Add(itemListBox55, 2, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer56 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer54->Add(itemBoxSizer56, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
  wxButton* itemButton57 = new wxButton( itemPanel42, ID_BUTTON4, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer56->Add(itemButton57, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxButton* itemButton58 = new wxButton( itemPanel42, ID_BUTTON5, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer56->Add(itemButton58, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer59Static = new wxStaticBox(itemPanel42, wxID_STATIC, _(" Communications "));
  wxStaticBoxSizer* itemStaticBoxSizer59 = new wxStaticBoxSizer(itemStaticBoxSizer59Static, wxHORIZONTAL);
  itemBoxSizer43->Add(itemStaticBoxSizer59, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText60 = new wxStaticText( itemPanel42, wxID_STATIC, _("Size"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer59->Add(itemStaticText60, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxSpinCtrl* itemSpinCtrl61 = new wxSpinCtrl( itemPanel42, ID_SPINCTRL, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
  itemStaticBoxSizer59->Add(itemSpinCtrl61, 3, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticText* itemStaticText62 = new wxStaticText( itemPanel42, wxID_STATIC, _("MB"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer59->Add(itemStaticText62, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  itemNotebook13->AddPage(itemPanel42, _("Filter"));

  wxPanel* itemPanel63 = new wxPanel( itemNotebook13, ID_PANEL_SOFTWARE_COUNTERS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer64 = new wxBoxSizer(wxVERTICAL);
  itemPanel63->SetSizer(itemBoxSizer64);

  wxStaticBox* itemStaticBoxSizer65Static = new wxStaticBox(itemPanel63, wxID_STATIC, _(" Region "));
  wxStaticBoxSizer* itemStaticBoxSizer65 = new wxStaticBoxSizer(itemStaticBoxSizer65Static, wxHORIZONTAL);
  itemBoxSizer64->Add(itemStaticBoxSizer65, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer66 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer65->Add(itemBoxSizer66, 0, wxGROW|wxALL, 5);
  wxRadioButton* itemRadioButton67 = new wxRadioButton( itemPanel63, ID_RADIOBUTTON5, _("On intervals"), wxDefaultPosition, wxDefaultSize, 0 );
  itemRadioButton67->SetValue(false);
  itemBoxSizer66->Add(itemRadioButton67, 1, wxGROW|wxALL, 5);

  wxRadioButton* itemRadioButton68 = new wxRadioButton( itemPanel63, ID_RADIOBUTTON6, _("On states"), wxDefaultPosition, wxDefaultSize, 0 );
  itemRadioButton68->SetValue(false);
  itemBoxSizer66->Add(itemRadioButton68, 1, wxGROW|wxALL, 5);

  wxStaticLine* itemStaticLine69 = new wxStaticLine( itemPanel63, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
  itemStaticBoxSizer65->Add(itemStaticLine69, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer70 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer65->Add(itemBoxSizer70, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
  wxBoxSizer* itemBoxSizer71 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer70->Add(itemBoxSizer71, 1, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText72 = new wxStaticText( itemPanel63, wxID_STATIC, _("Sampling Interval (ns)"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer71->Add(itemStaticText72, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxTextCtrl* itemTextCtrl73 = new wxTextCtrl( itemPanel63, ID_TEXTCTRL5, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer71->Add(itemTextCtrl73, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer74 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer70->Add(itemBoxSizer74, 1, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText75 = new wxStaticText( itemPanel63, wxID_STATIC, _("Min Burst Time (ns)"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer74->Add(itemStaticText75, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxTextCtrl* itemTextCtrl76 = new wxTextCtrl( itemPanel63, ID_TEXTCTRL6, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer74->Add(itemTextCtrl76, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer77Static = new wxStaticBox(itemPanel63, wxID_STATIC, _(" Selected events "));
  wxStaticBoxSizer* itemStaticBoxSizer77 = new wxStaticBoxSizer(itemStaticBoxSizer77Static, wxHORIZONTAL);
  itemBoxSizer64->Add(itemStaticBoxSizer77, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer78 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer77->Add(itemBoxSizer78, 1, wxGROW|wxALL, 5);
  wxArrayString itemCheckListBox79Strings;
  wxCheckListBox* itemCheckListBox79 = new wxCheckListBox( itemPanel63, ID_CHECKLISTBOX2, wxDefaultPosition, wxDefaultSize, itemCheckListBox79Strings, wxLB_SINGLE );
  itemBoxSizer78->Add(itemCheckListBox79, 2, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer80 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer78->Add(itemBoxSizer80, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
  wxButton* itemButton81 = new wxButton( itemPanel63, ID_BUTTON8, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer80->Add(itemButton81, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

  wxButton* itemButton82 = new wxButton( itemPanel63, ID_BUTTON9, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer80->Add(itemButton82, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

  wxStaticBox* itemStaticBoxSizer83Static = new wxStaticBox(itemPanel63, wxID_STATIC, _(" Options "));
  wxStaticBoxSizer* itemStaticBoxSizer83 = new wxStaticBoxSizer(itemStaticBoxSizer83Static, wxHORIZONTAL);
  itemBoxSizer64->Add(itemStaticBoxSizer83, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer84 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer83->Add(itemBoxSizer84, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer85 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer84->Add(itemBoxSizer85, 1, wxGROW|wxALL, 5);
  wxRadioButton* itemRadioButton86 = new wxRadioButton( itemPanel63, ID_RADIOBUTTON7, _("Count events"), wxDefaultPosition, wxDefaultSize, 0 );
  itemRadioButton86->SetValue(false);
  itemBoxSizer85->Add(itemRadioButton86, 1, wxGROW|wxALL, 5);

  wxRadioButton* itemRadioButton87 = new wxRadioButton( itemPanel63, ID_RADIOBUTTON8, _("Accumulate values"), wxDefaultPosition, wxDefaultSize, 0 );
  itemRadioButton87->SetValue(false);
  itemBoxSizer85->Add(itemRadioButton87, 1, wxGROW|wxALL, 5);

  wxStaticLine* itemStaticLine88 = new wxStaticLine( itemPanel63, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
  itemBoxSizer84->Add(itemStaticLine88, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer89 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer84->Add(itemBoxSizer89, 1, wxGROW|wxALL, 5);
  wxCheckBox* itemCheckBox90 = new wxCheckBox( itemPanel63, ID_CHECKBOX9, _("Remove states"), wxDefaultPosition, wxDefaultSize, 0 );
  itemCheckBox90->SetValue(false);
  itemBoxSizer89->Add(itemCheckBox90, 1, wxGROW|wxALL, 5);

  wxCheckBox* itemCheckBox91 = new wxCheckBox( itemPanel63, ID_CHECKBOX10, _("Summarize useful"), wxDefaultPosition, wxDefaultSize, 0 );
  itemCheckBox91->SetValue(false);
  itemBoxSizer89->Add(itemCheckBox91, 1, wxGROW|wxALL, 5);

  wxCheckBox* itemCheckBox92 = new wxCheckBox( itemPanel63, ID_CHECKBOX11, _("Global counters"), wxDefaultPosition, wxDefaultSize, 0 );
  itemCheckBox92->SetValue(false);
  itemBoxSizer89->Add(itemCheckBox92, 1, wxGROW|wxALL, 5);

  wxCheckBox* itemCheckBox93 = new wxCheckBox( itemPanel63, ID_CHECKBOX12, _("Only in bursts counting"), wxDefaultPosition, wxDefaultSize, 0 );
  itemCheckBox93->SetValue(false);
  itemBoxSizer89->Add(itemCheckBox93, 1, wxGROW|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer94Static = new wxStaticBox(itemPanel63, wxID_STATIC, _(" Keep events "));
  wxStaticBoxSizer* itemStaticBoxSizer94 = new wxStaticBoxSizer(itemStaticBoxSizer94Static, wxHORIZONTAL);
  itemBoxSizer64->Add(itemStaticBoxSizer94, 1, wxGROW|wxALL, 5);
  wxArrayString itemListBox95Strings;
  wxListBox* itemListBox95 = new wxListBox( itemPanel63, ID_LISTBOX1, wxDefaultPosition, wxDefaultSize, itemListBox95Strings, wxLB_SINGLE );
  itemStaticBoxSizer94->Add(itemListBox95, 2, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer96 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer94->Add(itemBoxSizer96, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
  wxButton* itemButton97 = new wxButton( itemPanel63, ID_BUTTON6, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer96->Add(itemButton97, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxButton* itemButton98 = new wxButton( itemPanel63, ID_BUTTON7, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer96->Add(itemButton98, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  itemNotebook13->AddPage(itemPanel63, _("Software Counters"));

  itemBoxSizer2->Add(itemNotebook13, 4, wxGROW|wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer99 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer99, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
  wxButton* itemButton100 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer99->AddButton(itemButton100);

  wxButton* itemButton101 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer99->AddButton(itemButton101);

  itemStdDialogButtonSizer99->Realize();

////@end CutFilterDialog content construction

  // Other initializations
  listToolOrder.push_back( string("Cutter") );
  listToolOrder.push_back( string("Filter") );
  listToolOrder.push_back( string("Software Counters") );

  UpdateToolList();
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
  if ( checkUseOriginalTime->IsChecked() )
  {
    checkBreakStates->SetValue( false );
    checkBreakStates->Disable();
  }
  else
  {
    checkBreakStates->Enable();
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

