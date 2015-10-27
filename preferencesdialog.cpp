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
#include "wx/bookctrl.h"
////@end includes

#include "preferencesdialog.h"
#include "paravermain.h"
#include "semanticcolor.h"
#include "labelconstructor.h"

// Validators
#include <wx/arrstr.h>
#include <wx/validate.h>

////@begin XPM images
#include "derived_add.xpm"
#include "delete.xpm"
#include "arrow_up.xpm"
#include "arrow_down.xpm"
////@end XPM images

using namespace std;

/*!
 * PreferencesDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( PreferencesDialog, wxPropertySheetDialog )


/*!
 * PreferencesDialog event table definition
 */

BEGIN_EVENT_TABLE( PreferencesDialog, wxPropertySheetDialog )

////@begin PreferencesDialog event table entries
  EVT_COLOURPICKER_CHANGED( ID_COLOURPICKER_BACKGROUND, PreferencesDialog::OnColourpickerBackgroundColourPickerChanged )
  EVT_UPDATE_UI( ID_COLOURPICKER_ZERO, PreferencesDialog::OnColourpickerZeroUpdate )
  EVT_BUTTON( ID_BUTTON_DEFAULT_TIMELINE, PreferencesDialog::OnButtonDefaultTimelineClick )
  EVT_BUTTON( ID_BUTTON_DEFAULT_GRADIENT, PreferencesDialog::OnButtonDefaultGradientClick )
  EVT_LISTBOX( ID_LISTBOX_WORKSPACES, PreferencesDialog::OnListboxWorkspacesSelected )
  EVT_BUTTON( ID_BUTTON_WORKSPACES_ADD, PreferencesDialog::OnButtonWorkspacesAddClick )
  EVT_BUTTON( ID_BUTTON_WORKSPACES_DELETE, PreferencesDialog::OnButtonWorkspacesDeleteClick )
  EVT_UPDATE_UI( ID_BUTTON_WORKSPACES_DELETE, PreferencesDialog::OnButtonWorkspacesDeleteUpdate )
  EVT_BUTTON( ID_BUTTON_WORKSPACES_UP, PreferencesDialog::OnButtonWorkspacesUpClick )
  EVT_UPDATE_UI( ID_BUTTON_WORKSPACES_UP, PreferencesDialog::OnButtonWorkspacesUpUpdate )
  EVT_BUTTON( ID_BUTTON_WORKSPACES_DOWN, PreferencesDialog::OnButtonWorkspacesDownClick )
  EVT_UPDATE_UI( ID_BUTTON_WORKSPACES_DOWN, PreferencesDialog::OnButtonWorkspacesDownUpdate )
  EVT_TEXT( ID_TEXT_WORKSPACE_NAME, PreferencesDialog::OnTextWorkspaceNameTextUpdated )
  EVT_UPDATE_UI( ID_TEXT_WORKSPACE_NAME, PreferencesDialog::OnTextWorkspaceNameUpdate )
  EVT_UPDATE_UI( ID_STATIC_WORKSPACE_AUTOTYPES, PreferencesDialog::OnStaticWorkspaceAutotypesUpdate )
  EVT_TEXT( ID_TEXT_WORKSPACE_AUTOTYPES, PreferencesDialog::OnTextWorkspaceAutotypesTextUpdated )
  EVT_UPDATE_UI( ID_TEXT_WORKSPACE_AUTOTYPES, PreferencesDialog::OnTextWorkspaceAutotypesUpdate )
  EVT_LISTBOX( ID_LISTBOX_HINTS_WORKSPACE, PreferencesDialog::OnListboxHintsWorkspaceSelected )
  EVT_UPDATE_UI( ID_LISTBOX_HINTS_WORKSPACE, PreferencesDialog::OnListboxHintsWorkspaceUpdate )
  EVT_BUTTON( ID_BUTTON_HINT_ADD, PreferencesDialog::OnButtonHintAddClick )
  EVT_UPDATE_UI( ID_BUTTON_HINT_ADD, PreferencesDialog::OnButtonHintAddUpdate )
  EVT_BUTTON( ID_BUTTON_HINT_DELETE, PreferencesDialog::OnButtonHintDeleteClick )
  EVT_UPDATE_UI( ID_BUTTON_HINT_DELETE, PreferencesDialog::OnButtonHintDeleteUpdate )
  EVT_BUTTON( ID_BITMAP_HINT_UP, PreferencesDialog::OnBitmapHintUpClick )
  EVT_UPDATE_UI( ID_BITMAP_HINT_UP, PreferencesDialog::OnBitmapHintUpUpdate )
  EVT_BUTTON( ID_BUTTON_HINT_DOWN, PreferencesDialog::OnButtonHintDownClick )
  EVT_UPDATE_UI( ID_BUTTON_HINT_DOWN, PreferencesDialog::OnButtonHintDownUpdate )
  EVT_TEXT( ID_TEXTCTRL_WORKSPACE_HINT_PATH, PreferencesDialog::OnTextctrlWorkspaceHintPathTextUpdated )
  EVT_UPDATE_UI( ID_TEXTCTRL_WORKSPACE_HINT_PATH, PreferencesDialog::OnTextctrlWorkspaceHintPathUpdate )
  EVT_UPDATE_UI( ID_FILE_BUTTON_WORKSPACE_HINT_PATH, PreferencesDialog::OnFileButtonWorkspaceHintPathUpdate )
  EVT_TEXT( ID_TEXTCTRL_WORKSPACE_HINT_DESCRIPTION, PreferencesDialog::OnTextctrlWorkspaceHintDescriptionTextUpdated )
  EVT_UPDATE_UI( ID_TEXTCTRL_WORKSPACE_HINT_DESCRIPTION, PreferencesDialog::OnTextctrlWorkspaceHintDescriptionUpdate )
////@end PreferencesDialog event table entries

END_EVENT_TABLE()


/*!
 * PreferencesDialog constructors
 */

PreferencesDialog::PreferencesDialog()
{
  Init();
}

PreferencesDialog::PreferencesDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}


/*!
 * PreferencesDialog creator
 */

bool PreferencesDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin PreferencesDialog creation
  SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
  SetSheetStyle(wxPROPSHEET_DEFAULT);
  wxPropertySheetDialog::Create( parent, id, caption, pos, size, style );

  CreateButtons(wxOK|wxCANCEL);
  CreateControls();
  LayoutDialog();
  Centre();
////@end PreferencesDialog creation
  return true;
}


/*!
 * PreferencesDialog destructor
 */

PreferencesDialog::~PreferencesDialog()
{
////@begin PreferencesDialog destruction
////@end PreferencesDialog destruction
}


/*!
 * Member initialisation
 */

void PreferencesDialog::Init()
{
////@begin PreferencesDialog member initialisation
  cfgsPath = "";
  colorUseZero = false;
  filtersXMLPath = "";
  globalFillStateGaps = false;
  globalFullTracePath = false;
  histogramAutofit3DScale = true;
  histogramAutofitControlScale = true;
  histogramAutofitDataGradient = true;
  histogramDrawmodeObjects = 0;
  histogramDrawmodeSemantic = 0;
  histogramGradientFunction = 0;
  histogramHideEmpty = false;
  histogramHorizontal = true;
  histogramLabelsColor = false;
  histogramMaxNumColumns = 100000;
  histogramMaxPrecision = 10;
  histogramNameFormatFull = "";
  histogramNameFormatPrefix = "";
  histogramNumColumns = 20;
  histogramPrecision = 2;
  histogramSaveImageFormat = 0;
  histogramSaveTextFormat = 0;
  histogramScientificNotation = false;
  histogramShowGradient = true;
  histogramShowUnits = true;
  histogramThousandSeparator = true;
  histogramZoom = false;
  maximumTraceSize = 256;
  timelineColor = 0;
  timelineCommunicationLines = true;
  timelineDrawmodeObjects = 0;
  timelineDrawmodeTime = 0;
  timelineEventLines = false;
  timelineGradientFunction = 0;
  timelineNameFormatFull = "%W @ %T";
  timelineNameFormatPrefix = "New Window # %N";
  timelineObjectAxis = 0;
  timelineObjectLabels = 0;
  timelinePixelSize = 0;
  timelineSaveImageFormat = 0;
  timelineSaveTextFormat = 0;
  timelineWWCommunications = true;
  timelineWWEvents = true;
  timelineWWPrecision = 2;
  timelineWWPreviousNext = false;
  timelineWWSemantic = true;
  timelineWWText = true;
  tmpPath = "";
  tracesPath = "";
  tutorialsPath = "";
  whatWhereMaxPrecision = 10;
  checkGlobalFillStateGaps = NULL;
  checkGlobalFullTracePath = NULL;
  txtMaximumTraceSize = NULL;
  textCtrlTrace = NULL;
  dirBrowserButtonTrace = NULL;
  textCtrlCFG = NULL;
  dirBrowserButtonCFG = NULL;
  textCtrlXML = NULL;
  dirBrowserButtonXML = NULL;
  textCtrlTutorials = NULL;
  dirBrowserButtonTutorials = NULL;
  textCtrlTmp = NULL;
  dirBrowserButtonTmp = NULL;
  checkGlobalSingleInstance = NULL;
  spinSessionTime = NULL;
  txtTimelineNameFormatPrefix = NULL;
  txtTimelineNameFormatFull = NULL;
  checkTimelineCommunicationLines = NULL;
  checkTimelineEventLines = NULL;
  choiceTimelineColor = NULL;
  choiceTimelineGradientFunction = NULL;
  choiceTimelineDrawmodeTime = NULL;
  choiceTimelineDrawmodeObjects = NULL;
  choiceTimelinePixelSize = NULL;
  choiceTimelineLabels = NULL;
  choiceTimelineObjectAxis = NULL;
  checkTimelineWWSemantic = NULL;
  checkTimelineWWEvents = NULL;
  checkTimelineWWCommunications = NULL;
  checkTimelineWWPreviousNext = NULL;
  checkTimelineWWText = NULL;
  txtTimelineWWPrecision = NULL;
  txtTimelineWWEventPixels = NULL;
  choiceTimelineSaveImageFormat = NULL;
  choiceTimelineSaveTextFormat = NULL;
  txtHistogramNameFormatPrefix = NULL;
  txtHistogramNameFormatFull = NULL;
  checkHistogramZoom = NULL;
  checkHistogramHorizontal = NULL;
  checkHistogramHideEmpty = NULL;
  checkHistogramShowGradient = NULL;
  checkHistogramLabelsColor = NULL;
  choiceHistogramGradientFunction = NULL;
  choiceHistogramDrawmodeSemantic = NULL;
  choiceHistogramDrawmodeObjects = NULL;
  checkHistogramScientificNotation = NULL;
  checkHistogramThousandsSeparator = NULL;
  checkHistogramShowUnits = NULL;
  txtHistogramPrecision = NULL;
  checkHistogramAutofitControlScale = NULL;
  checkHistogramAutofit3DScale = NULL;
  checkHistogramAutofitDataGradient = NULL;
  txtHistogramNumColumns = NULL;
  choiceHistogramSaveImageFormat = NULL;
  choiceHistogramSaveTextFormat = NULL;
  colourPickerBackground = NULL;
  colourPickerAxis = NULL;
  checkZero = NULL;
  colourPickerZero = NULL;
  colourPickerLogical = NULL;
  colourPickerPhysical = NULL;
  colourPickerGradientBegin = NULL;
  colourPickerGradientEnd = NULL;
  colourPickerGradientLow = NULL;
  colourPickerGradientTop = NULL;
  listWorkspaces = NULL;
  buttonAddWorkspace = NULL;
  buttonDeleteWorkspace = NULL;
  buttonUpWorkspace = NULL;
  buttonDownWorkspace = NULL;
  txtWorkspaceName = NULL;
  txtAutoTypes = NULL;
  listHintsWorkspace = NULL;
  buttonAddHint = NULL;
  buttonDeleteHint = NULL;
  buttonUpHint = NULL;
  buttonDownHint = NULL;
  txtHintPath = NULL;
  fileBrowserHintPath = NULL;
  txtHintDescription = NULL;
////@end PreferencesDialog member initialisation
}


/*!
 * Control creation for PreferencesDialog
 */

void PreferencesDialog::CreateControls()
{    
////@begin PreferencesDialog content construction
  PreferencesDialog* itemPropertySheetDialog1 = this;

  wxPanel* itemPanel2 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_GLOBAL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
  itemPanel2->SetSizer(itemBoxSizer3);

  wxStaticBox* itemStaticBoxSizer4Static = new wxStaticBox(itemPanel2, wxID_ANY, _("  Trace  "));
  wxStaticBoxSizer* itemStaticBoxSizer4 = new wxStaticBoxSizer(itemStaticBoxSizer4Static, wxVERTICAL);
  itemBoxSizer3->Add(itemStaticBoxSizer4, 0, wxGROW|wxALL, 5);
  checkGlobalFillStateGaps = new wxCheckBox( itemPanel2, ID_PREFERENCES_GLOBAL_FILLGAPS, _("Fill State gaps with IDLE State"), wxDefaultPosition, wxDefaultSize, 0 );
  checkGlobalFillStateGaps->SetValue(false);
  itemStaticBoxSizer4->Add(checkGlobalFillStateGaps, 1, wxGROW|wxALL, 2);

  checkGlobalFullTracePath = new wxCheckBox( itemPanel2, ID_PREFERENCES_GLOBAL_FULLTRACEPATH, _("View full path in trace selector"), wxDefaultPosition, wxDefaultSize, 0 );
  checkGlobalFullTracePath->SetValue(false);
  itemStaticBoxSizer4->Add(checkGlobalFullTracePath, 1, wxGROW|wxALL, 2);

  wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer4->Add(itemBoxSizer7, 0, wxGROW|wxALL, 2);
  wxStaticText* itemStaticText8 = new wxStaticText( itemPanel2, wxID_STATIC, _("Maximum loadable trace size (MB)"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer7->Add(itemStaticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txtMaximumTraceSize = new wxSpinCtrl( itemPanel2, ID_TEXTCTRL_MAXIMUM_LOADABLE_TRACE_SIZE, wxT("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 5000, 0 );
  itemBoxSizer7->Add(txtMaximumTraceSize, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer10Static = new wxStaticBox(itemPanel2, wxID_ANY, _("  Default directories  "));
  wxStaticBoxSizer* itemStaticBoxSizer10 = new wxStaticBoxSizer(itemStaticBoxSizer10Static, wxVERTICAL);
  itemBoxSizer3->Add(itemStaticBoxSizer10, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer10->Add(itemBoxSizer11, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText12 = new wxStaticText( itemPanel2, wxID_STATIC, _("Traces"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText12->SetToolTip(_("Base path to traces files (.prv, .prv.gz, .pcf and .row)."));
  itemBoxSizer11->Add(itemStaticText12, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  textCtrlTrace = new wxTextCtrl( itemPanel2, ID_TEXTCTRL_DEFAULT_TRACE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    textCtrlTrace->SetToolTip(_("Base path to traces files (.prv, .prv.gz, .pcf and .row)."));
  itemBoxSizer11->Add(textCtrlTrace, 5, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  dirBrowserButtonTrace = new DirBrowserButton( itemPanel2, ID_BUTTON_DIR_BROWSER_TRACE, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    dirBrowserButtonTrace->SetToolTip(_("Base path to traces files (.prv, .prv.gz, .pcf and .row)."));
  itemBoxSizer11->Add(dirBrowserButtonTrace, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer10->Add(itemBoxSizer15, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText16 = new wxStaticText( itemPanel2, wxID_STATIC, _("CFGs"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText16->SetToolTip(_("Base path to configuration files (.cfg)."));
  itemBoxSizer15->Add(itemStaticText16, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  textCtrlCFG = new wxTextCtrl( itemPanel2, ID_TEXTCTRL_DEFAULT_CFGS, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    textCtrlCFG->SetToolTip(_("Base path to configuration files (.cfg)."));
  itemBoxSizer15->Add(textCtrlCFG, 5, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  dirBrowserButtonCFG = new DirBrowserButton( itemPanel2, ID_DIRBROWSERBUTTON_DEFAULT_CFGS, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    dirBrowserButtonCFG->SetToolTip(_("Base path to configuration files (.cfg)."));
  itemBoxSizer15->Add(dirBrowserButtonCFG, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer10->Add(itemBoxSizer19, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText20 = new wxStaticText( itemPanel2, wxID_STATIC, _("Filters XML"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText20->SetToolTip(_("Base path to Cut/Filters XML."));
  itemBoxSizer19->Add(itemStaticText20, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  textCtrlXML = new wxTextCtrl( itemPanel2, ID_TEXTCTRL_DEFAULT_XMLS, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    textCtrlXML->SetToolTip(_("Base path to Cut/Filters XML."));
  itemBoxSizer19->Add(textCtrlXML, 5, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  dirBrowserButtonXML = new DirBrowserButton( itemPanel2, ID_BUTTON_DEFAULT_XMLS, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    dirBrowserButtonXML->SetToolTip(_("Base path to Cut/Filters XML."));
  itemBoxSizer19->Add(dirBrowserButtonXML, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer10->Add(itemBoxSizer23, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText24 = new wxStaticText( itemPanel2, wxID_STATIC, _("Tutorials root"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText24->SetToolTip(_("Base path to wxparaver tutorials."));
  itemBoxSizer23->Add(itemStaticText24, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  textCtrlTutorials = new wxTextCtrl( itemPanel2, ID_TEXTCTRL_DEFAULT_TUTORIALS, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    textCtrlTutorials->SetToolTip(_("Base path to wxparaver tutorials."));
  itemBoxSizer23->Add(textCtrlTutorials, 5, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  dirBrowserButtonTutorials = new DirBrowserButton( itemPanel2, ID_DIRBROWSERBUTTON_DEFAULT_TUTORIALS, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    dirBrowserButtonTutorials->SetToolTip(_("Base path to wxparaver tutorials."));
  itemBoxSizer23->Add(dirBrowserButtonTutorials, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer27 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer10->Add(itemBoxSizer27, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText28 = new wxStaticText( itemPanel2, wxID_STATIC, _("Tmp dir"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText28->SetToolTip(_("Working directory for results and logs."));
  itemBoxSizer27->Add(itemStaticText28, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  textCtrlTmp = new wxTextCtrl( itemPanel2, ID_TEXTCTRL_DEFAULT_TMP, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    textCtrlTmp->SetToolTip(_("Working directory for results and logs."));
  itemBoxSizer27->Add(textCtrlTmp, 5, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  dirBrowserButtonTmp = new DirBrowserButton( itemPanel2, ID_DIRBROWSERBUTTON_DEFAULT_TMP, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    dirBrowserButtonTmp->SetToolTip(_("Working directory for results and logs."));
  itemBoxSizer27->Add(dirBrowserButtonTmp, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer31Static = new wxStaticBox(itemPanel2, wxID_ANY, _("  Behaviour  "));
  wxStaticBoxSizer* itemStaticBoxSizer31 = new wxStaticBoxSizer(itemStaticBoxSizer31Static, wxVERTICAL);
  itemBoxSizer3->Add(itemStaticBoxSizer31, 0, wxGROW|wxALL, 5);
  checkGlobalSingleInstance = new wxCheckBox( itemPanel2, ID_PREFERENCES_GLOBAL_SINGLE_INSTANCE, _("Allow only one running instance"), wxDefaultPosition, wxDefaultSize, 0 );
  checkGlobalSingleInstance->SetValue(false);
  itemStaticBoxSizer31->Add(checkGlobalSingleInstance, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer33 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer31->Add(itemBoxSizer33, 1, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText34 = new wxStaticText( itemPanel2, wxID_STATIC, _("Automatically save session every"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer33->Add(itemStaticText34, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  spinSessionTime = new wxSpinCtrl( itemPanel2, ID_PREFERENCES_GLOBAL_TIME_SESSION, wxT("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
  itemBoxSizer33->Add(spinSessionTime, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticText* itemStaticText36 = new wxStaticText( itemPanel2, wxID_STATIC, _("minutes"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer33->Add(itemStaticText36, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  GetBookCtrl()->AddPage(itemPanel2, _("Global"));

  wxPanel* itemPanel37 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_TIMELINE, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer38 = new wxBoxSizer(wxHORIZONTAL);
  itemPanel37->SetSizer(itemBoxSizer38);

  wxBoxSizer* itemBoxSizer39 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer38->Add(itemBoxSizer39, 3, wxGROW|wxTOP, 5);
  wxStaticBox* itemStaticBoxSizer40Static = new wxStaticBox(itemPanel37, wxID_STATIC, _("  Name Format  "));
  wxStaticBoxSizer* itemStaticBoxSizer40 = new wxStaticBoxSizer(itemStaticBoxSizer40Static, wxVERTICAL);
  itemStaticBoxSizer40Static->Enable(false);
  itemBoxSizer39->Add(itemStaticBoxSizer40, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer41 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer40->Add(itemBoxSizer41, 0, wxGROW|wxTOP|wxBOTTOM, 5);
  wxStaticText* itemStaticText42 = new wxStaticText( itemPanel37, wxID_STATIC, _("Prefix"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText42->SetToolTip(_("Default name for new timelines, applied in window browser."));
  itemStaticText42->Enable(false);
  itemBoxSizer41->Add(itemStaticText42, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  txtTimelineNameFormatPrefix = new wxTextCtrl( itemPanel37, ID_PREFERENCES_TIMELINE_NAME_PREFIX, _("New Window # %N"), wxDefaultPosition, wxDefaultSize, 0 );
  txtTimelineNameFormatPrefix->SetMaxLength(256);
  if (PreferencesDialog::ShowToolTips())
    txtTimelineNameFormatPrefix->SetToolTip(_("Default name for new timelines, applied in window browser."));
  txtTimelineNameFormatPrefix->Enable(false);
  itemBoxSizer41->Add(txtTimelineNameFormatPrefix, 2, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer44 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer40->Add(itemBoxSizer44, 0, wxGROW|wxBOTTOM, 5);
  wxStaticText* itemStaticText45 = new wxStaticText( itemPanel37, wxID_STATIC, _("Full"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText45->SetToolTip(_("Default full name for new timelines, applied in window title."));
  itemStaticText45->Enable(false);
  itemBoxSizer44->Add(itemStaticText45, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  txtTimelineNameFormatFull = new wxTextCtrl( itemPanel37, ID_PREFERENCES_TIMELINE_NAME_FULL, _("%P @ %T"), wxDefaultPosition, wxDefaultSize, 0 );
  txtTimelineNameFormatFull->SetMaxLength(256);
  if (PreferencesDialog::ShowToolTips())
    txtTimelineNameFormatFull->SetToolTip(_("Default full name for new timelines, applied in window title."));
  txtTimelineNameFormatFull->Enable(false);
  itemBoxSizer44->Add(txtTimelineNameFormatFull, 2, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxStaticBox* itemStaticBoxSizer47Static = new wxStaticBox(itemPanel37, wxID_ANY, _("  Rendering Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer47 = new wxStaticBoxSizer(itemStaticBoxSizer47Static, wxVERTICAL);
  itemBoxSizer39->Add(itemStaticBoxSizer47, 1, wxGROW|wxALL, 5);
  checkTimelineCommunicationLines = new wxCheckBox( itemPanel37, ID_PREFERENCES_TIMELINE_COMMUNICATION_LINES, _("Communication Lines"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineCommunicationLines->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineCommunicationLines->SetToolTip(_("View communications lines in new timelines."));
  itemStaticBoxSizer47->Add(checkTimelineCommunicationLines, 0, wxGROW|wxTOP, 5);

  checkTimelineEventLines = new wxCheckBox( itemPanel37, ID_PREFERENCES_TIMELINE_EVENT_LINES, _("Event Flags"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineEventLines->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineEventLines->SetToolTip(_("View event flags in new timelines."));
  itemStaticBoxSizer47->Add(checkTimelineEventLines, 0, wxGROW, 5);

  wxBoxSizer* itemBoxSizer50 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer47->Add(itemBoxSizer50, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText51 = new wxStaticText( itemPanel37, wxID_STATIC, _("Color"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText51->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer50->Add(itemStaticText51, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceTimelineColorStrings;
  choiceTimelineColorStrings.Add(_("Dummy Text"));
  choiceTimelineColor = new wxChoice( itemPanel37, ID_PREFERENCES_TIMELINE_COLOR, wxDefaultPosition, wxDefaultSize, choiceTimelineColorStrings, 0 );
  choiceTimelineColor->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineColor->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer50->Add(choiceTimelineColor, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer53 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer47->Add(itemBoxSizer53, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText54 = new wxStaticText( itemPanel37, wxID_STATIC, _("Gradient Function"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText54->SetToolTip(_("Gradient function selected in new timeline."));
  itemBoxSizer53->Add(itemStaticText54, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxArrayString choiceTimelineGradientFunctionStrings;
  choiceTimelineGradientFunctionStrings.Add(_("Dummy Text"));
  choiceTimelineGradientFunction = new wxChoice( itemPanel37, ID_PREFERENCES_TIMELINE_GRADIENT, wxDefaultPosition, wxDefaultSize, choiceTimelineGradientFunctionStrings, 0 );
  choiceTimelineGradientFunction->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineGradientFunction->SetToolTip(_("Gradient function selected in new timeline."));
  itemBoxSizer53->Add(choiceTimelineGradientFunction, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer56 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer47->Add(itemBoxSizer56, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText57 = new wxStaticText( itemPanel37, wxID_STATIC, _("Drawmode Time"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText57->SetToolTip(_("Drawmode for time axis selected in new timeline."));
  itemBoxSizer56->Add(itemStaticText57, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceTimelineDrawmodeTimeStrings;
  choiceTimelineDrawmodeTimeStrings.Add(_("Dummy Text"));
  choiceTimelineDrawmodeTime = new wxChoice( itemPanel37, ID_PREFERENCES_TIMELINE_DRAWMODE_TIME, wxDefaultPosition, wxDefaultSize, choiceTimelineDrawmodeTimeStrings, 0 );
  choiceTimelineDrawmodeTime->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineDrawmodeTime->SetToolTip(_("Drawmode for time axis selected in new timeline."));
  itemBoxSizer56->Add(choiceTimelineDrawmodeTime, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer59 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer47->Add(itemBoxSizer59, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText60 = new wxStaticText( itemPanel37, wxID_STATIC, _("Drawmode Objects"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText60->SetToolTip(_("Drawmode for objects axis selected in new timeline."));
  itemBoxSizer59->Add(itemStaticText60, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceTimelineDrawmodeObjectsStrings;
  choiceTimelineDrawmodeObjectsStrings.Add(_("Dummy Text"));
  choiceTimelineDrawmodeObjects = new wxChoice( itemPanel37, ID_PREFERENCES_TIMELINE_DRAWMODE_OBJECTS, wxDefaultPosition, wxDefaultSize, choiceTimelineDrawmodeObjectsStrings, 0 );
  choiceTimelineDrawmodeObjects->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineDrawmodeObjects->SetToolTip(_("Drawmode for objects axis selected in new timeline."));
  itemBoxSizer59->Add(choiceTimelineDrawmodeObjects, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer62 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer47->Add(itemBoxSizer62, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText63 = new wxStaticText( itemPanel37, wxID_STATIC, _("Pixel Size"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText63->SetToolTip(_("Pixel size selected in new timeline."));
  itemBoxSizer62->Add(itemStaticText63, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString choiceTimelinePixelSizeStrings;
  choiceTimelinePixelSizeStrings.Add(_("Dummy Text"));
  choiceTimelinePixelSize = new wxChoice( itemPanel37, ID_PREFERENCES_TIMELINE_PIXEL_SIZE, wxDefaultPosition, wxDefaultSize, choiceTimelinePixelSizeStrings, 0 );
  choiceTimelinePixelSize->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceTimelinePixelSize->SetToolTip(_("Pixel size selected in new timeline."));
  itemBoxSizer62->Add(choiceTimelinePixelSize, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer65 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer47->Add(itemBoxSizer65, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText66 = new wxStaticText( itemPanel37, wxID_STATIC, _("Labels to draw"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText66->SetToolTip(_("Labels to draw by default in a new timeline."));
  itemBoxSizer65->Add(itemStaticText66, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString choiceTimelineLabelsStrings;
  choiceTimelineLabelsStrings.Add(_("Dummy Text"));
  choiceTimelineLabels = new wxChoice( itemPanel37, ID_PREFERENCES_TIMELINE_LABELS, wxDefaultPosition, wxDefaultSize, choiceTimelineLabelsStrings, 0 );
  choiceTimelineLabels->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineLabels->SetToolTip(_("Labels to draw by default in a new timeline."));
  itemBoxSizer65->Add(choiceTimelineLabels, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer68 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer47->Add(itemBoxSizer68, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText69 = new wxStaticText( itemPanel37, wxID_STATIC, _("Object Axis Position"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer68->Add(itemStaticText69, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString choiceTimelineObjectAxisStrings;
  choiceTimelineObjectAxisStrings.Add(_("Dummy Text"));
  choiceTimelineObjectAxis = new wxChoice( itemPanel37, ID_PREFERENCES_TIMELINE_OBJECT_AXIS, wxDefaultPosition, wxDefaultSize, choiceTimelineObjectAxisStrings, 0 );
  choiceTimelineObjectAxis->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineObjectAxis->SetToolTip(_("Object axis position by default in a new timeline."));
  itemBoxSizer68->Add(choiceTimelineObjectAxis, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer71 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer38->Add(itemBoxSizer71, 2, wxGROW|wxTOP, 5);
  wxStaticBox* itemStaticBoxSizer72Static = new wxStaticBox(itemPanel37, wxID_ANY, _("  What / Where  "));
  wxStaticBoxSizer* itemStaticBoxSizer72 = new wxStaticBoxSizer(itemStaticBoxSizer72Static, wxVERTICAL);
  itemBoxSizer71->Add(itemStaticBoxSizer72, 2, wxGROW|wxALL, 5);
  checkTimelineWWSemantic = new wxCheckBox( itemPanel37, ID_CHECKBOX_TIMELINE_WW_SEMANTIC, _("Semantic"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWSemantic->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWSemantic->SetToolTip(_("Show semantic information when left click in timeline."));
  itemStaticBoxSizer72->Add(checkTimelineWWSemantic, 0, wxGROW|wxTOP, 5);

  checkTimelineWWEvents = new wxCheckBox( itemPanel37, ID_CHECKBOX_TIMELINE_WW_EVENTS, _("Events"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWEvents->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWEvents->SetToolTip(_("Show events information when left click in timeline."));
  itemStaticBoxSizer72->Add(checkTimelineWWEvents, 0, wxGROW, 5);

  checkTimelineWWCommunications = new wxCheckBox( itemPanel37, ID_CHECKBOX_TIMELINE_WW_COMMUNICATIONS, _("Communications"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWCommunications->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWCommunications->SetToolTip(_("Show communications information when left click in timeline."));
  itemStaticBoxSizer72->Add(checkTimelineWWCommunications, 0, wxGROW, 5);

  checkTimelineWWPreviousNext = new wxCheckBox( itemPanel37, ID_CHECKBOX_TIMELINE_WW_PREVIOUS_NEXT, _("Previous / Next"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWPreviousNext->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWPreviousNext->SetToolTip(_("In addition to current semantic value, show information for previous and next ones when left click in timeline."));
  itemStaticBoxSizer72->Add(checkTimelineWWPreviousNext, 0, wxGROW, 5);

  checkTimelineWWText = new wxCheckBox( itemPanel37, ID_CHECKBOX_TIMELINE_WW_TEXT, _("Text"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWText->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWText->SetToolTip(_("Show semantic value as text when left click in timeline."));
  itemStaticBoxSizer72->Add(checkTimelineWWText, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer78 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer72->Add(itemBoxSizer78, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText79 = new wxStaticText( itemPanel37, wxID_STATIC, _("Precision"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer78->Add(itemStaticText79, 2, wxALIGN_CENTER_VERTICAL, 5);

  txtTimelineWWPrecision = new wxSpinCtrl( itemPanel37, ID_PREFERENCES_TIMELINE_WW_PRECISION, wxT("2"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 2 );
  if (PreferencesDialog::ShowToolTips())
    txtTimelineWWPrecision->SetToolTip(_("Decimal precision for semantic values."));
  itemBoxSizer78->Add(txtTimelineWWPrecision, 3, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer81 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer72->Add(itemBoxSizer81, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText82 = new wxStaticText( itemPanel37, wxID_STATIC, _("Event pixels"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticText82->SetHelpText(_("Pixels to look around for events"));
  if (PreferencesDialog::ShowToolTips())
    itemStaticText82->SetToolTip(_("Pixels to look around for events"));
  itemBoxSizer81->Add(itemStaticText82, 2, wxALIGN_CENTER_VERTICAL, 5);

  txtTimelineWWEventPixels = new wxSpinCtrl( itemPanel37, ID_PREFERENCES_TIMELINE_WW_EVENT_PIXELS, wxT("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
  txtTimelineWWEventPixels->SetHelpText(_("Pixels to look around for events"));
  if (PreferencesDialog::ShowToolTips())
    txtTimelineWWEventPixels->SetToolTip(_("Pixels to look around for events"));
  itemBoxSizer81->Add(txtTimelineWWEventPixels, 3, wxALIGN_CENTER_VERTICAL, 5);

  wxStaticBox* itemStaticBoxSizer84Static = new wxStaticBox(itemPanel37, wxID_ANY, _("  Default Save Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer84 = new wxStaticBoxSizer(itemStaticBoxSizer84Static, wxVERTICAL);
  itemBoxSizer71->Add(itemStaticBoxSizer84, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer85 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer84->Add(itemBoxSizer85, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxStaticText* itemStaticText86 = new wxStaticText( itemPanel37, wxID_STATIC, _("Image as"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText86->SetToolTip(_("Default image format selected."));
  itemBoxSizer85->Add(itemStaticText86, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceTimelineSaveImageFormatStrings;
  choiceTimelineSaveImageFormatStrings.Add(_("Dummy Text"));
  choiceTimelineSaveImageFormat = new wxChoice( itemPanel37, ID_PREFERENCES_TIMELINE_SAVE_AS_IMAGE, wxDefaultPosition, wxDefaultSize, choiceTimelineSaveImageFormatStrings, 0 );
  choiceTimelineSaveImageFormat->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineSaveImageFormat->SetToolTip(_("Default image format selected."));
  itemBoxSizer85->Add(choiceTimelineSaveImageFormat, 3, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer88 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer84->Add(itemBoxSizer88, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxStaticText* itemStaticText89 = new wxStaticText( itemPanel37, wxID_STATIC, _("Text as"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText89->SetToolTip(_("Default text format selected."));
  itemBoxSizer88->Add(itemStaticText89, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceTimelineSaveTextFormatStrings;
  choiceTimelineSaveTextFormatStrings.Add(_("Dummy Text"));
  choiceTimelineSaveTextFormat = new wxChoice( itemPanel37, ID_PREFERENCES_TIMELINE_SAVE_AS_TEXT, wxDefaultPosition, wxDefaultSize, choiceTimelineSaveTextFormatStrings, 0 );
  choiceTimelineSaveTextFormat->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineSaveTextFormat->SetToolTip(_("Default text format selected."));
  itemBoxSizer88->Add(choiceTimelineSaveTextFormat, 3, wxALIGN_CENTER_VERTICAL, 5);

  GetBookCtrl()->AddPage(itemPanel37, _("Timeline"));

  wxPanel* itemPanel91 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_HISTOGRAM, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer92 = new wxBoxSizer(wxHORIZONTAL);
  itemPanel91->SetSizer(itemBoxSizer92);

  wxBoxSizer* itemBoxSizer93 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer92->Add(itemBoxSizer93, 3, wxGROW|wxTOP, 5);
  wxStaticBox* itemStaticBoxSizer94Static = new wxStaticBox(itemPanel91, wxID_STATIC, _("  Name Format  "));
  wxStaticBoxSizer* itemStaticBoxSizer94 = new wxStaticBoxSizer(itemStaticBoxSizer94Static, wxVERTICAL);
  itemStaticBoxSizer94Static->Enable(false);
  itemBoxSizer93->Add(itemStaticBoxSizer94, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer95 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer94->Add(itemBoxSizer95, 0, wxGROW|wxTOP|wxBOTTOM, 5);
  wxStaticText* itemStaticText96 = new wxStaticText( itemPanel91, wxID_STATIC, _("Prefix"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText96->SetToolTip(_("Default name for new histograms, applied in window browser."));
  itemStaticText96->Enable(false);
  itemBoxSizer95->Add(itemStaticText96, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  txtHistogramNameFormatPrefix = new wxTextCtrl( itemPanel91, ID_PREFERENCES_HISTOGRAM_NAME_PREFIX, _("New Histogram # %N"), wxDefaultPosition, wxDefaultSize, 0 );
  txtHistogramNameFormatPrefix->SetMaxLength(256);
  if (PreferencesDialog::ShowToolTips())
    txtHistogramNameFormatPrefix->SetToolTip(_("Default name for new histograms, applied in window browser."));
  txtHistogramNameFormatPrefix->Enable(false);
  itemBoxSizer95->Add(txtHistogramNameFormatPrefix, 2, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer98 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer94->Add(itemBoxSizer98, 0, wxGROW|wxBOTTOM, 5);
  wxStaticText* itemStaticText99 = new wxStaticText( itemPanel91, wxID_STATIC, _("Full"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText99->SetToolTip(_("Default full name for new histograms, applied in window title."));
  itemStaticText99->Enable(false);
  itemBoxSizer98->Add(itemStaticText99, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  txtHistogramNameFormatFull = new wxTextCtrl( itemPanel91, ID_PREFERENCES_HISTOGRAM_NAME_FULL, _("%P @ %T"), wxDefaultPosition, wxDefaultSize, 0 );
  txtHistogramNameFormatFull->SetMaxLength(256);
  if (PreferencesDialog::ShowToolTips())
    txtHistogramNameFormatFull->SetToolTip(_("Default full name for new histograms, applied in window title."));
  txtHistogramNameFormatFull->Enable(false);
  itemBoxSizer98->Add(txtHistogramNameFormatFull, 2, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxStaticBox* itemStaticBoxSizer101Static = new wxStaticBox(itemPanel91, wxID_ANY, _("  Matrix Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer101 = new wxStaticBoxSizer(itemStaticBoxSizer101Static, wxVERTICAL);
  itemBoxSizer93->Add(itemStaticBoxSizer101, 1, wxGROW|wxALL, 5);
  checkHistogramZoom = new wxCheckBox( itemPanel91, ID_PREFERENCES_HISTOGRAM_MATRIX_ZOOM, _("Zoom View as default"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramZoom->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramZoom->SetToolTip(_("Select between numeric and zoom view for the histogram matrix."));
  itemStaticBoxSizer101->Add(checkHistogramZoom, 0, wxGROW|wxTOP, 5);

  checkHistogramHorizontal = new wxCheckBox( itemPanel91, ID_PREFERENCES_HISTOGRAM_MATRIX_HORIZONTAL, _("Horizontal disposition"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramHorizontal->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramHorizontal->SetToolTip(_("Select between horizontal / vertical disposition for the histogram matrix."));
  itemStaticBoxSizer101->Add(checkHistogramHorizontal, 0, wxGROW, 5);

  checkHistogramHideEmpty = new wxCheckBox( itemPanel91, ID_PREFERENCES_HISTOGRAM_MATRIX_HIDE_EMPTY, _("Hide empty columns"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramHideEmpty->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramHideEmpty->SetToolTip(_("Hide columns when all its cells are empty."));
  itemStaticBoxSizer101->Add(checkHistogramHideEmpty, 0, wxGROW, 5);

  checkHistogramShowGradient = new wxCheckBox( itemPanel91, ID_PREFERENCES_HISTOGRAM_MATRIX_GRADIENT, _("Show Gradient Colors"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramShowGradient->SetValue(true);
  itemStaticBoxSizer101->Add(checkHistogramShowGradient, 0, wxGROW, 5);

  checkHistogramLabelsColor = new wxCheckBox( itemPanel91, ID_PREFERENCES_HISTOGRAM_MATRIX_LABELS_COLOR, _("Labels color"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramLabelsColor->SetValue(false);
  itemStaticBoxSizer101->Add(checkHistogramLabelsColor, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer107 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer101->Add(itemBoxSizer107, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText108 = new wxStaticText( itemPanel91, wxID_STATIC, _("Gradient Function"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText108->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer107->Add(itemStaticText108, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceHistogramGradientFunctionStrings;
  choiceHistogramGradientFunctionStrings.Add(_("Dummy Text"));
  choiceHistogramGradientFunction = new wxChoice( itemPanel91, ID_PREFERENCES_HISTOGRAM_MATRIX_GRADIENT_FUNCTION, wxDefaultPosition, wxDefaultSize, choiceHistogramGradientFunctionStrings, 0 );
  choiceHistogramGradientFunction->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceHistogramGradientFunction->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer107->Add(choiceHistogramGradientFunction, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer110 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer101->Add(itemBoxSizer110, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText111 = new wxStaticText( itemPanel91, wxID_STATIC, _("Drawmode Semantic"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText111->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer110->Add(itemStaticText111, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceHistogramDrawmodeSemanticStrings;
  choiceHistogramDrawmodeSemanticStrings.Add(_("Dummy Text"));
  choiceHistogramDrawmodeSemantic = new wxChoice( itemPanel91, ID_PREFERENCES_HISTOGRAM_MATRIX_DRAWMODE_SEMANTIC, wxDefaultPosition, wxDefaultSize, choiceHistogramDrawmodeSemanticStrings, 0 );
  choiceHistogramDrawmodeSemantic->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceHistogramDrawmodeSemantic->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer110->Add(choiceHistogramDrawmodeSemantic, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer113 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer101->Add(itemBoxSizer113, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText114 = new wxStaticText( itemPanel91, wxID_STATIC, _("Drawmode Objects"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText114->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer113->Add(itemStaticText114, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceHistogramDrawmodeObjectsStrings;
  choiceHistogramDrawmodeObjectsStrings.Add(_("Dummy Text"));
  choiceHistogramDrawmodeObjects = new wxChoice( itemPanel91, ID_PREFERENCES_HISTOGRAM_MATRIX_DRAWMODE_OBJECTS, wxDefaultPosition, wxDefaultSize, choiceHistogramDrawmodeObjectsStrings, 0 );
  choiceHistogramDrawmodeObjects->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceHistogramDrawmodeObjects->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer113->Add(choiceHistogramDrawmodeObjects, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer116 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer92->Add(itemBoxSizer116, 2, wxGROW|wxTOP, 5);
  wxStaticBox* itemStaticBoxSizer117Static = new wxStaticBox(itemPanel91, wxID_ANY, _("  Cell Format  "));
  wxStaticBoxSizer* itemStaticBoxSizer117 = new wxStaticBoxSizer(itemStaticBoxSizer117Static, wxVERTICAL);
  itemBoxSizer116->Add(itemStaticBoxSizer117, 0, wxGROW|wxALL, 5);
  checkHistogramScientificNotation = new wxCheckBox( itemPanel91, ID_PREFERENCES_HISTOGRAM_SCIENTIFIC_NOTATION, _("Scientific Notation"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramScientificNotation->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramScientificNotation->SetToolTip(_("Write cell content using scientific notation."));
  itemStaticBoxSizer117->Add(checkHistogramScientificNotation, 0, wxGROW|wxTOP, 5);

  checkHistogramThousandsSeparator = new wxCheckBox( itemPanel91, ID_PREFERENCES_HISTOGRAM_THOUSANDS_SEPARATOR, _("Thousands Separator"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramThousandsSeparator->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramThousandsSeparator->SetToolTip(_("Write cell content using thousands separator."));
  itemStaticBoxSizer117->Add(checkHistogramThousandsSeparator, 0, wxGROW, 5);

  checkHistogramShowUnits = new wxCheckBox( itemPanel91, ID_PREFERENCES_HISTOGRAM_SHOW_UNITS, _("Show Units"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramShowUnits->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramShowUnits->SetToolTip(_("End numeric information with its units."));
  itemStaticBoxSizer117->Add(checkHistogramShowUnits, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer121 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer117->Add(itemBoxSizer121, 0, wxGROW|wxBOTTOM, 5);
  wxStaticText* itemStaticText122 = new wxStaticText( itemPanel91, wxID_STATIC, _("Precision"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText122->SetToolTip(_("Decimal precision applied to the presentation of histogram metrics."));
  itemBoxSizer121->Add(itemStaticText122, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txtHistogramPrecision = new wxSpinCtrl( itemPanel91, ID_PREFERENCES_HISTOGRAM_PRECISION, wxT("2"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 2 );
  if (PreferencesDialog::ShowToolTips())
    txtHistogramPrecision->SetToolTip(_("Decimal precision applied to the presentation of histogram metrics."));
  itemBoxSizer121->Add(txtHistogramPrecision, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer124Static = new wxStaticBox(itemPanel91, wxID_ANY, _("  Compute Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer124 = new wxStaticBoxSizer(itemStaticBoxSizer124Static, wxVERTICAL);
  itemBoxSizer116->Add(itemStaticBoxSizer124, 0, wxGROW|wxALL, 5);
  checkHistogramAutofitControlScale = new wxCheckBox( itemPanel91, ID_PREFERENCES_HISTOGRAM_AUTOFIT_CONTROL, _("Autofit Control Scale"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramAutofitControlScale->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramAutofitControlScale->SetToolTip(_("Fit control scale when histogram is created."));
  itemStaticBoxSizer124->Add(checkHistogramAutofitControlScale, 0, wxGROW|wxTOP, 5);

  checkHistogramAutofit3DScale = new wxCheckBox( itemPanel91, ID_PREFERENCES_HISTOGRAM_AUTOFIT_3D, _("Autofit 3D Scale"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramAutofit3DScale->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramAutofit3DScale->SetToolTip(_("Fit 3D scale when histogram is created."));
  itemStaticBoxSizer124->Add(checkHistogramAutofit3DScale, 0, wxGROW, 5);

  checkHistogramAutofitDataGradient = new wxCheckBox( itemPanel91, ID_PREFERENCES_HISTOGRAM_AUTOFIT_DATA_GRADIENT, _("Autofit Data Gradient"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramAutofitDataGradient->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramAutofitDataGradient->SetToolTip(_("Fit data gradient when histogram is created."));
  itemStaticBoxSizer124->Add(checkHistogramAutofitDataGradient, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer128 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer124->Add(itemBoxSizer128, 0, wxGROW|wxBOTTOM, 5);
  wxStaticText* itemStaticText129 = new wxStaticText( itemPanel91, wxID_STATIC, _("Columns"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText129->SetToolTip(_("Initial number of columns for new histograms."));
  itemBoxSizer128->Add(itemStaticText129, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txtHistogramNumColumns = new wxSpinCtrl( itemPanel91, ID_PREFERENCES_HISTOGRAM_NUMCOLUMNS, wxT("20"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 256, 20 );
  if (PreferencesDialog::ShowToolTips())
    txtHistogramNumColumns->SetToolTip(_("Initial number of columns for new histograms."));
  itemBoxSizer128->Add(txtHistogramNumColumns, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer131Static = new wxStaticBox(itemPanel91, wxID_ANY, _("  Default Save Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer131 = new wxStaticBoxSizer(itemStaticBoxSizer131Static, wxVERTICAL);
  itemBoxSizer116->Add(itemStaticBoxSizer131, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer132 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer131->Add(itemBoxSizer132, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxStaticText* itemStaticText133 = new wxStaticText( itemPanel91, wxID_STATIC, _("Image as"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText133->SetToolTip(_("Default image format selected."));
  itemBoxSizer132->Add(itemStaticText133, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceHistogramSaveImageFormatStrings;
  choiceHistogramSaveImageFormatStrings.Add(_("Dummy Text"));
  choiceHistogramSaveImageFormat = new wxChoice( itemPanel91, ID_PREFERENCES_HISTOGRAM_SAVE_IMAGE_FORMAT, wxDefaultPosition, wxDefaultSize, choiceHistogramSaveImageFormatStrings, 0 );
  choiceHistogramSaveImageFormat->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceHistogramSaveImageFormat->SetToolTip(_("Default image format selected."));
  itemBoxSizer132->Add(choiceHistogramSaveImageFormat, 3, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer135 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer131->Add(itemBoxSizer135, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxStaticText* itemStaticText136 = new wxStaticText( itemPanel91, wxID_STATIC, _("Text as"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText136->SetToolTip(_("Default text format selected."));
  itemBoxSizer135->Add(itemStaticText136, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceHistogramSaveTextFormatStrings;
  choiceHistogramSaveTextFormatStrings.Add(_("Dummy Text"));
  choiceHistogramSaveTextFormat = new wxChoice( itemPanel91, ID_PREFERENCES_HISTOGRAM_SAVE_TXT_FORMAT, wxDefaultPosition, wxDefaultSize, choiceHistogramSaveTextFormatStrings, 0 );
  choiceHistogramSaveTextFormat->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceHistogramSaveTextFormat->SetToolTip(_("Default text format selected."));
  itemBoxSizer135->Add(choiceHistogramSaveTextFormat, 3, wxALIGN_CENTER_VERTICAL, 5);

  GetBookCtrl()->AddPage(itemPanel91, _("Histogram"));

  wxPanel* itemPanel138 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_COLOR, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer139 = new wxBoxSizer(wxVERTICAL);
  itemPanel138->SetSizer(itemBoxSizer139);

  wxStaticBox* itemStaticBoxSizer140Static = new wxStaticBox(itemPanel138, wxID_ANY, _("  Timeline  "));
  wxStaticBoxSizer* itemStaticBoxSizer140 = new wxStaticBoxSizer(itemStaticBoxSizer140Static, wxVERTICAL);
  itemBoxSizer139->Add(itemStaticBoxSizer140, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer141 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer140->Add(itemBoxSizer141, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText142 = new wxStaticText( itemPanel138, wxID_STATIC, _("Background"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText142->SetToolTip(_("When background timeline color is changed, new foreground complementary color is suggested."));
  itemBoxSizer141->Add(itemStaticText142, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerBackground = new wxColourPickerCtrl( itemPanel138, ID_COLOURPICKER_BACKGROUND, wxColour(0, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerBackground->SetToolTip(_("When background timeline color is changed, new foreground complementary color is suggested."));
  itemBoxSizer141->Add(colourPickerBackground, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer144 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer140->Add(itemBoxSizer144, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText145 = new wxStaticText( itemPanel138, wxID_STATIC, _("Foreground"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText145->SetToolTip(_("Color used to draw scale axis and object names in timelines."));
  itemBoxSizer144->Add(itemStaticText145, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerAxis = new wxColourPickerCtrl( itemPanel138, ID_COLOURPICKER_AXIS, wxColour(255, 255, 255), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerAxis->SetToolTip(_("Color used to draw scale axis and object names in timelines."));
  itemBoxSizer144->Add(colourPickerAxis, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer147 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer140->Add(itemBoxSizer147, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  checkZero = new wxCheckBox( itemPanel138, wxID_STATIC, _("Semantic zero"), wxDefaultPosition, wxDefaultSize, 0 );
  checkZero->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkZero->SetToolTip(_("Color used for semantic zero values"));
  itemBoxSizer147->Add(checkZero, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerZero = new wxColourPickerCtrl( itemPanel138, ID_COLOURPICKER_ZERO, wxColour(0, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerZero->SetToolTip(_("Color used for semantic zero values"));
  itemBoxSizer147->Add(colourPickerZero, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer150 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer140->Add(itemBoxSizer150, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText151 = new wxStaticText( itemPanel138, wxID_STATIC, _("Logical comms."), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText151->SetToolTip(_("Color used to draw logical communications in timelines."));
  itemBoxSizer150->Add(itemStaticText151, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerLogical = new wxColourPickerCtrl( itemPanel138, ID_COLOURPICKER_LOGICAL, wxColour(255, 255, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerLogical->SetToolTip(_("Color used to draw logical communications in timelines."));
  itemBoxSizer150->Add(colourPickerLogical, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer153 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer140->Add(itemBoxSizer153, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText154 = new wxStaticText( itemPanel138, wxID_STATIC, _("Physical comms."), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText154->SetToolTip(_("Color used to draw physical communications in timelines."));
  itemBoxSizer153->Add(itemStaticText154, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerPhysical = new wxColourPickerCtrl( itemPanel138, ID_COLOURPICKER_PHYSICAL, wxColour(255, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerPhysical->SetToolTip(_("Color used to draw physical communications in timelines."));
  itemBoxSizer153->Add(colourPickerPhysical, 2, wxALIGN_CENTER_VERTICAL|wxTOP, 5);

  wxButton* itemButton156 = new wxButton( itemPanel138, ID_BUTTON_DEFAULT_TIMELINE, _("Default"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemButton156->SetToolTip(_("Restores predefined colors."));
  itemStaticBoxSizer140->Add(itemButton156, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT, 5);

  wxStaticBox* itemStaticBoxSizer157Static = new wxStaticBox(itemPanel138, wxID_ANY, _("  Gradient  "));
  wxStaticBoxSizer* itemStaticBoxSizer157 = new wxStaticBoxSizer(itemStaticBoxSizer157Static, wxVERTICAL);
  itemBoxSizer139->Add(itemStaticBoxSizer157, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer158 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer157->Add(itemBoxSizer158, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText159 = new wxStaticText( itemPanel138, wxID_STATIC, _("Begin"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText159->SetToolTip(_("Base color used to represent semantic values when gradient is selected."));
  itemBoxSizer158->Add(itemStaticText159, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientBegin = new wxColourPickerCtrl( itemPanel138, ID_COLOURPICKER_GRADBEGIN, wxColour(0, 255, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientBegin->SetToolTip(_("Base color used to represent semantic values when gradient is selected."));
  itemBoxSizer158->Add(colourPickerGradientBegin, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer161 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer157->Add(itemBoxSizer161, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText162 = new wxStaticText( itemPanel138, wxID_STATIC, _("End"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText162->SetToolTip(_("Top color used to represent semantic values when gradient is selected."));
  itemBoxSizer161->Add(itemStaticText162, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientEnd = new wxColourPickerCtrl( itemPanel138, ID_COLOURPICKER_GRADEND, wxColour(0, 0, 255), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientEnd->SetToolTip(_("Top color used to represent semantic values when gradient is selected."));
  itemBoxSizer161->Add(colourPickerGradientEnd, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer164 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer157->Add(itemBoxSizer164, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText165 = new wxStaticText( itemPanel138, wxID_STATIC, _("Low outliers"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText165->SetToolTip(_("Any semantic outlier smaller than <Begin> will use this color."));
  itemBoxSizer164->Add(itemStaticText165, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientLow = new wxColourPickerCtrl( itemPanel138, ID_COLOURPICKER_GRADLOW, wxColour(207, 207, 68), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientLow->SetToolTip(_("Any semantic outlier smaller than <Begin> will use this color."));
  itemBoxSizer164->Add(colourPickerGradientLow, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer167 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer157->Add(itemBoxSizer167, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText168 = new wxStaticText( itemPanel138, wxID_STATIC, _("High outliers"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText168->SetToolTip(_("Any semantic outlier greater than <End> will use this color."));
  itemBoxSizer167->Add(itemStaticText168, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientTop = new wxColourPickerCtrl( itemPanel138, ID_COLOURPICKER_GRADTOP, wxColour(255, 146, 24), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientTop->SetToolTip(_("Any semantic outlier greater than <End> will use this color."));
  itemBoxSizer167->Add(colourPickerGradientTop, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxButton* itemButton170 = new wxButton( itemPanel138, ID_BUTTON_DEFAULT_GRADIENT, _("Default"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemButton170->SetToolTip(_("Restores predefined colors."));
  itemStaticBoxSizer157->Add(itemButton170, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT, 5);

  GetBookCtrl()->AddPage(itemPanel138, _("Color"));

  wxPanel* itemPanel171 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_WORKSPACES, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer172 = new wxBoxSizer(wxHORIZONTAL);
  itemPanel171->SetSizer(itemBoxSizer172);

  wxBoxSizer* itemBoxSizer173 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer172->Add(itemBoxSizer173, 2, wxGROW|wxALL, 5);
  wxArrayString listWorkspacesStrings;
  listWorkspaces = new wxListBox( itemPanel171, ID_LISTBOX_WORKSPACES, wxDefaultPosition, wxDefaultSize, listWorkspacesStrings, wxLB_SINGLE|wxLB_HSCROLL );
  itemBoxSizer173->Add(listWorkspaces, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer175 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer173->Add(itemBoxSizer175, 0, wxALIGN_TOP|wxALL, 0);
  buttonAddWorkspace = new wxBitmapButton( itemPanel171, ID_BUTTON_WORKSPACES_ADD, itemPropertySheetDialog1->GetBitmapResource(wxT("derived_add.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer175->Add(buttonAddWorkspace, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonDeleteWorkspace = new wxBitmapButton( itemPanel171, ID_BUTTON_WORKSPACES_DELETE, itemPropertySheetDialog1->GetBitmapResource(wxT("delete.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer175->Add(buttonDeleteWorkspace, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonUpWorkspace = new wxBitmapButton( itemPanel171, ID_BUTTON_WORKSPACES_UP, itemPropertySheetDialog1->GetBitmapResource(wxT("arrow_up.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer175->Add(buttonUpWorkspace, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonDownWorkspace = new wxBitmapButton( itemPanel171, ID_BUTTON_WORKSPACES_DOWN, itemPropertySheetDialog1->GetBitmapResource(wxT("arrow_down.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer175->Add(buttonDownWorkspace, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxStaticLine* itemStaticLine180 = new wxStaticLine( itemPanel171, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
  itemBoxSizer172->Add(itemStaticLine180, 0, wxGROW|wxTOP|wxBOTTOM, 10);

  wxBoxSizer* itemBoxSizer181 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer172->Add(itemBoxSizer181, 4, wxGROW|wxALL, 0);
  wxBoxSizer* itemBoxSizer182 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer181->Add(itemBoxSizer182, 0, wxGROW|wxLEFT|wxTOP, 5);
  txtWorkspaceName = new wxTextCtrl( itemPanel171, ID_TEXT_WORKSPACE_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer182->Add(txtWorkspaceName, 1, wxGROW|wxRIGHT|wxTOP|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer184 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer181->Add(itemBoxSizer184, 0, wxGROW|wxLEFT, 5);
  wxStaticText* itemStaticText185 = new wxStaticText( itemPanel171, ID_STATIC_WORKSPACE_AUTOTYPES, _("Event Types"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer184->Add(itemStaticText185, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txtAutoTypes = new wxTextCtrl( itemPanel171, ID_TEXT_WORKSPACE_AUTOTYPES, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer184->Add(txtAutoTypes, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

  wxStaticBox* itemStaticBoxSizer187Static = new wxStaticBox(itemPanel171, wxID_ANY, _(" Hints "));
  wxStaticBoxSizer* itemStaticBoxSizer187 = new wxStaticBoxSizer(itemStaticBoxSizer187Static, wxVERTICAL);
  itemBoxSizer181->Add(itemStaticBoxSizer187, 1, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
  wxBoxSizer* itemBoxSizer188 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer187->Add(itemBoxSizer188, 4, wxGROW, 0);
  wxArrayString listHintsWorkspaceStrings;
  listHintsWorkspace = new wxListBox( itemPanel171, ID_LISTBOX_HINTS_WORKSPACE, wxDefaultPosition, wxDefaultSize, listHintsWorkspaceStrings, wxLB_SINGLE );
  itemBoxSizer188->Add(listHintsWorkspace, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer190 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer188->Add(itemBoxSizer190, 0, wxALIGN_TOP|wxALL, 0);
  buttonAddHint = new wxBitmapButton( itemPanel171, ID_BUTTON_HINT_ADD, itemPropertySheetDialog1->GetBitmapResource(wxT("derived_add.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer190->Add(buttonAddHint, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonDeleteHint = new wxBitmapButton( itemPanel171, ID_BUTTON_HINT_DELETE, itemPropertySheetDialog1->GetBitmapResource(wxT("delete.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer190->Add(buttonDeleteHint, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonUpHint = new wxBitmapButton( itemPanel171, ID_BITMAP_HINT_UP, itemPropertySheetDialog1->GetBitmapResource(wxT("arrow_up.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer190->Add(buttonUpHint, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonDownHint = new wxBitmapButton( itemPanel171, ID_BUTTON_HINT_DOWN, itemPropertySheetDialog1->GetBitmapResource(wxT("arrow_down.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer190->Add(buttonDownHint, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer195 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer187->Add(itemBoxSizer195, 0, wxGROW, 5);
  wxBoxSizer* itemBoxSizer196 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer195->Add(itemBoxSizer196, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);
  wxStaticText* itemStaticText197 = new wxStaticText( itemPanel171, wxID_STATIC, _("Path"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer196->Add(itemStaticText197, 0, wxALIGN_LEFT|wxALL, 5);

  itemBoxSizer196->Add(5, 5, 0, wxGROW|wxTOP|wxBOTTOM, 2);

  wxStaticText* itemStaticText199 = new wxStaticText( itemPanel171, wxID_STATIC, _("Description"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer196->Add(itemStaticText199, 0, wxALIGN_LEFT|wxALL, 5);

  wxBoxSizer* itemBoxSizer200 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer195->Add(itemBoxSizer200, 4, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);
  wxBoxSizer* itemBoxSizer201 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer200->Add(itemBoxSizer201, 0, wxGROW|wxALL, 0);
  txtHintPath = new wxTextCtrl( itemPanel171, ID_TEXTCTRL_WORKSPACE_HINT_PATH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
  itemBoxSizer201->Add(txtHintPath, 1, wxGROW|wxTOP|wxBOTTOM, 5);

  fileBrowserHintPath = new FileBrowserButton( itemPanel171, ID_FILE_BUTTON_WORKSPACE_HINT_PATH, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer201->Add(fileBrowserHintPath, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer204 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer200->Add(itemBoxSizer204, 0, wxGROW|wxALL, 0);
  txtHintDescription = new wxTextCtrl( itemPanel171, ID_TEXTCTRL_WORKSPACE_HINT_DESCRIPTION, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer204->Add(txtHintDescription, 1, wxGROW|wxRIGHT|wxTOP|wxBOTTOM, 5);

  GetBookCtrl()->AddPage(itemPanel171, _("Workspaces"));

  wxPanel* itemPanel206 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_FILTERS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  itemPanel206->Show(false);
  itemPanel206->Enable(false);

  GetBookCtrl()->AddPage(itemPanel206, _("Filters"));

////@end PreferencesDialog content construction

  // Filter forbidden chars
  wxArrayString forbidden;
  forbidden.Add( _( "-" ) );
  forbidden.Add( _( "." ) );
  forbidden.Add( _( "," ) );
  
  // DUMMY CODE: the purpose is only to avoid warning at compile
  itemPropertySheetDialog1 = NULL;

/*
// wxTextCtrl validators

  wxTextValidator validator( wxFILTER_NUMERIC | wxFILTER_EXCLUDE_CHAR_LIST );
  validator.SetExcludes( forbidden );

  txtHistogramNumColumns->SetValidator( validator );
  txtHistogramPrecision->SetValidator( validator );
  txtTimelineWWPrecision->SetValidator( validator );
*/
  
  dirBrowserButtonTrace->SetTextBox( textCtrlTrace );
  dirBrowserButtonTrace->SetDialogMessage( _( "Select Traces Default Directory" ) );
  dirBrowserButtonTrace->Enable();
  
  dirBrowserButtonCFG->SetTextBox( textCtrlCFG );
  dirBrowserButtonCFG->SetDialogMessage( _( "Select Paraver CFGs Default Directory" ) );
  dirBrowserButtonCFG->Enable();
  
  dirBrowserButtonXML->SetTextBox( textCtrlXML );
  dirBrowserButtonXML->SetDialogMessage( _( "Select Cut/Filter XMLs Default Directory" ) );
  dirBrowserButtonXML->Enable();
  
  dirBrowserButtonTutorials->SetTextBox( textCtrlTutorials );
  dirBrowserButtonTutorials->SetDialogMessage( _( "Select Tutorials Root Directory" ) );
  dirBrowserButtonTutorials->Enable();
  
  dirBrowserButtonTmp->SetTextBox( textCtrlTmp );
  dirBrowserButtonTmp->SetDialogMessage( _( "Select TMP Default Directory" ) );
  dirBrowserButtonTmp->Enable();
  
  fileBrowserHintPath->SetTextBox( txtHintPath );
  fileBrowserHintPath->SetFileDialogWildcard( _( "Paraver configuration file (*.cfg)|*.cfg|All files (*.*)|*.*" ) );
  fileBrowserHintPath->SetDialogMessage( _( "Select Configuration File" ) );
  
  panelID[ ID_PREFERENCES_GLOBAL ] = 0;
  panelID[ ID_PREFERENCES_TIMELINE ] = 1;
  panelID[ ID_PREFERENCES_HISTOGRAM ] = 2;
  panelID[ ID_PREFERENCES_COLOR ] = 3;
  panelID[ ID_PREFERENCES_WORKSPACES ] = 4;
}


bool PreferencesDialog::SetPanel( wxWindowID whichPanelID )
{
  bool found = false;
  
  if ( panelID.find( whichPanelID ) != panelID.end() )
  {
    GetBookCtrl()->SetSelection( panelID[ whichPanelID ] );
    found = true;
  }

  return found;
}


/*!
 * Should we show tooltips?
 */

bool PreferencesDialog::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap PreferencesDialog::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin PreferencesDialog bitmap retrieval
  wxUnusedVar(name);
  if (name == wxT("derived_add.xpm"))
  {
    wxBitmap bitmap(derived_add_xpm);
    return bitmap;
  }
  else if (name == wxT("delete.xpm"))
  {
    wxBitmap bitmap(delete_xpm);
    return bitmap;
  }
  else if (name == wxT("arrow_up.xpm"))
  {
    wxBitmap bitmap(arrow_up_xpm);
    return bitmap;
  }
  else if (name == wxT("arrow_down.xpm"))
  {
    wxBitmap bitmap(arrow_down_xpm);
    return bitmap;
  }
  return wxNullBitmap;
////@end PreferencesDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon PreferencesDialog::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin PreferencesDialog icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end PreferencesDialog icon retrieval
}


wxString PreferencesDialog::formatNumber( long value )
{
  stringstream auxSStr;
  wxString auxNumber;

  locale mylocale("");
  auxSStr.imbue( mylocale );
//  auxSStr.precision( ParaverConfig::getInstance()->getPrecision() );
//  auxSStr << fixed;
  auxSStr << value;
  auxNumber << wxString::FromAscii( auxSStr.str().c_str() );

  return auxNumber;
}


void PreferencesDialog::setLabelsChoiceBox( const vector< string > &list,
                                               const PRV_UINT32 &selected,
                                               wxChoice *choiceBox )
{
  choiceBox->Clear(); // entra dos veces!!!

  for( vector< string >::const_iterator it = list.begin(); it != list.end(); ++it )
  {
    // add every string of the list to the choice box
    choiceBox->Append( wxString::FromAscii( (*it).c_str() ) );
  }

  choiceBox->Select( selected );
}

// Transfers data to all panels
bool PreferencesDialog::TransferDataToWindow()
{
  vector< string > options;

  // GLOBAL
  checkGlobalFillStateGaps->SetValue( globalFillStateGaps );
  checkGlobalFullTracePath->SetValue( globalFullTracePath );
  dirBrowserButtonTrace->SetPath( wxString::FromAscii( tracesPath.c_str() ) );
  dirBrowserButtonCFG->SetPath( wxString::FromAscii( cfgsPath.c_str() ) );
  dirBrowserButtonXML->SetPath( wxString::FromAscii( filtersXMLPath.c_str() ) );
  dirBrowserButtonTutorials->SetPath( wxString::FromAscii( tutorialsPath.c_str() ) );
  dirBrowserButtonTmp->SetPath( wxString::FromAscii( tmpPath.c_str() ) );
  txtMaximumTraceSize->SetValue( maximumTraceSize );
  checkGlobalSingleInstance->SetValue( singleInstance );
  spinSessionTime->SetValue( sessionSaveTime );

  // TIMELINE
  txtTimelineNameFormatPrefix->SetValue( wxString::FromAscii( timelineNameFormatPrefix.c_str() ) );
  txtTimelineNameFormatFull->SetValue( wxString::FromAscii( timelineNameFormatFull.c_str() ) );

  checkTimelineEventLines->SetValue( timelineEventLines );
  checkTimelineCommunicationLines->SetValue( timelineCommunicationLines );

  options.clear();
  LabelConstructor::getGUIGroupLabels( LabelConstructor::COLOR, options );
  setLabelsChoiceBox( options, timelineColor, choiceTimelineColor );

  options.clear();
  LabelConstructor::getGUIGroupLabels( LabelConstructor::GRADIENT_FUNCTION, options );
  setLabelsChoiceBox( options, timelineGradientFunction, choiceTimelineGradientFunction );

  options.clear();
  LabelConstructor::getGUIGroupLabels( LabelConstructor::DRAWMODE, options );
  setLabelsChoiceBox( options, timelineDrawmodeTime, choiceTimelineDrawmodeTime );
  setLabelsChoiceBox( options, timelineDrawmodeObjects, choiceTimelineDrawmodeObjects );

  options.clear();
  LabelConstructor::getGUIGroupLabels( LabelConstructor::PIXEL_SIZE, options );
  setLabelsChoiceBox( options, timelinePixelSize, choiceTimelinePixelSize );

  options.clear();
  LabelConstructor::getGUIGroupLabels( LabelConstructor::OBJECT_LABELS, options );
  setLabelsChoiceBox( options, timelineObjectLabels, choiceTimelineLabels );

  options.clear();
  LabelConstructor::getGUIGroupLabels( LabelConstructor::OBJECT_AXIS, options );
  setLabelsChoiceBox( options, timelineObjectAxis, choiceTimelineObjectAxis );
  
  checkTimelineWWSemantic->SetValue( timelineWWSemantic );
  checkTimelineWWEvents->SetValue( timelineWWEvents );
  checkTimelineWWCommunications->SetValue( timelineWWCommunications );
  checkTimelineWWPreviousNext->SetValue( timelineWWPreviousNext );
  checkTimelineWWText->SetValue( timelineWWText );
  txtTimelineWWPrecision->SetValue( ( int )timelineWWPrecision );
  txtTimelineWWEventPixels->SetValue( ( int ) timelineWWEventPixels );

  options.clear();
  LabelConstructor::getGUIGroupLabels( LabelConstructor::IMAGE_FORMAT, options );
  setLabelsChoiceBox( options, timelineSaveImageFormat, choiceTimelineSaveImageFormat );

  options.clear();
  LabelConstructor::getGUIGroupLabels( LabelConstructor::TEXT_FORMAT, options );
  setLabelsChoiceBox( options, timelineSaveTextFormat, choiceTimelineSaveTextFormat );

  // HISTOGRAM
  txtHistogramNameFormatPrefix->SetValue( wxString::FromAscii( histogramNameFormatPrefix.c_str() ) );
  txtHistogramNameFormatFull->SetValue( wxString::FromAscii( histogramNameFormatFull.c_str() ) );

  checkHistogramZoom->SetValue( histogramZoom );
  checkHistogramHorizontal->SetValue( histogramHorizontal );
  checkHistogramHideEmpty->SetValue( histogramHideEmpty );
  checkHistogramShowGradient->SetValue( histogramShowGradient );
  checkHistogramLabelsColor->SetValue( histogramLabelsColor );

  options.clear();
  LabelConstructor::getGUIGroupLabels( LabelConstructor::GRADIENT_FUNCTION, options );
  setLabelsChoiceBox( options, histogramGradientFunction, choiceHistogramGradientFunction );

  options.clear();
  LabelConstructor::getGUIGroupLabels( LabelConstructor::DRAWMODE, options );
  setLabelsChoiceBox( options, histogramDrawmodeSemantic, choiceHistogramDrawmodeSemantic );
  setLabelsChoiceBox( options, histogramDrawmodeObjects, choiceHistogramDrawmodeObjects );

  checkHistogramScientificNotation->SetValue( histogramScientificNotation );
  checkHistogramThousandsSeparator->SetValue( histogramThousandSeparator );
  checkHistogramShowUnits->SetValue( histogramShowUnits );
  txtHistogramPrecision->SetValue( ( int )histogramPrecision );

  checkHistogramAutofitControlScale->SetValue( histogramAutofitControlScale );
  checkHistogramAutofit3DScale->SetValue( histogramAutofit3DScale );
  checkHistogramAutofitDataGradient->SetValue( histogramAutofitDataGradient );
  txtHistogramNumColumns->SetValue( ( int )histogramNumColumns );

  options.clear();
  LabelConstructor::getGUIGroupLabels( LabelConstructor::IMAGE_FORMAT, options );
  setLabelsChoiceBox( options, histogramSaveImageFormat, choiceHistogramSaveImageFormat );

  options.clear();
  LabelConstructor::getGUIGroupLabels( LabelConstructor::TEXT_FORMAT, options );
  setLabelsChoiceBox( options, histogramSaveTextFormat, choiceHistogramSaveTextFormat );

  // COLORS
  colourPickerBackground->SetColour( RGBTowxColour( timelineColourBackground ) );
  colourPickerAxis->SetColour( RGBTowxColour( timelineColourAxis ) );
  checkZero->SetValue( colorUseZero );
  colourPickerZero->SetColour( RGBTowxColour( timelineColourZero ) );
  colourPickerLogical->SetColour( RGBTowxColour( timelineColourLogical ) );
  colourPickerPhysical->SetColour( RGBTowxColour( timelineColourPhysical ) );

  colourPickerGradientBegin->SetColour( RGBTowxColour( gradientColourBegin ) );
  colourPickerGradientEnd->SetColour( RGBTowxColour( gradientColourEnd ) );
  colourPickerGradientLow->SetColour( RGBTowxColour( gradientColourLow ) );
  colourPickerGradientTop->SetColour( RGBTowxColour( gradientColourTop ) );

  // FILTERS


  // WORKSPACES
  std::vector<std::string> tmpWorkspaceList = WorkspaceManager::getInstance()->getWorkspaces();
  for( std::vector<std::string>::iterator it = tmpWorkspaceList.begin(); it != tmpWorkspaceList.end(); ++it )
  {
    listWorkspaces->Append( wxString::FromAscii( it->c_str() ) );
    workspaceContainer.insert( std::pair<wxString,Workspace>( wxString::FromAscii( it->c_str() ),
                                                              WorkspaceManager::getInstance()->getWorkspace( *it ) ) );
  }

  return true;
}

// Transfers data from all panels
bool PreferencesDialog::TransferDataFromWindow()
{
  vector< string > options;

  // GLOBAL
  globalFillStateGaps = checkGlobalFillStateGaps->IsChecked();
  globalFullTracePath = checkGlobalFullTracePath->IsChecked();
  tracesPath = std::string( dirBrowserButtonTrace->GetPath().mb_str() );
  cfgsPath = std::string( dirBrowserButtonCFG->GetPath().mb_str() );
  filtersXMLPath = std::string( dirBrowserButtonXML->GetPath().mb_str() );
  tutorialsPath = std::string( dirBrowserButtonTutorials->GetPath().mb_str() );
  tmpPath = std::string( dirBrowserButtonTmp->GetPath().mb_str() );
  maximumTraceSize = (float)txtMaximumTraceSize->GetValue();
  singleInstance = checkGlobalSingleInstance->GetValue();
  sessionSaveTime = spinSessionTime->GetValue();

  // TIMELINE
  timelineNameFormatPrefix = std::string( txtTimelineNameFormatPrefix->GetValue().mb_str() );
  timelineNameFormatFull = std::string( txtTimelineNameFormatFull->GetValue().mb_str() );
  // Parse format looking for modifiers %N -> number, %P -> prefix, %T -> trace

  timelineEventLines = checkTimelineEventLines->IsChecked();
  timelineCommunicationLines = checkTimelineCommunicationLines->IsChecked();

  timelineColor = ( PRV_UINT32 )choiceTimelineColor->GetCurrentSelection();
  timelineGradientFunction = ( PRV_UINT32 )choiceTimelineGradientFunction->GetCurrentSelection();
  timelineDrawmodeTime = ( PRV_UINT32 )choiceTimelineDrawmodeTime->GetCurrentSelection();
  timelineDrawmodeObjects = ( PRV_UINT32 )choiceTimelineDrawmodeObjects->GetCurrentSelection();
  timelinePixelSize = ( PRV_UINT32 )choiceTimelinePixelSize->GetCurrentSelection();
  timelineObjectLabels = ( Window::TObjectLabels ) choiceTimelineLabels->GetCurrentSelection();
  timelineObjectAxis = ( Window::TObjectAxisSize ) choiceTimelineObjectAxis->GetCurrentSelection();

  timelineWWSemantic = checkTimelineWWSemantic->IsChecked();
  timelineWWEvents = checkTimelineWWEvents->IsChecked();
  timelineWWCommunications = checkTimelineWWCommunications->IsChecked();
  timelineWWPreviousNext = checkTimelineWWPreviousNext->IsChecked();
  timelineWWText = checkTimelineWWText->IsChecked();
  timelineWWPrecision = ( PRV_UINT32 )txtTimelineWWPrecision->GetValue();
  timelineWWEventPixels = ( PRV_INT16 )txtTimelineWWEventPixels->GetValue();

  timelineSaveImageFormat = ( PRV_UINT32 )choiceTimelineSaveImageFormat->GetCurrentSelection();
  timelineSaveTextFormat = ( PRV_UINT32 )choiceTimelineSaveTextFormat->GetCurrentSelection();

  // HISTOGRAM
  histogramNameFormatPrefix = std::string( txtHistogramNameFormatPrefix->GetValue().mb_str() );
  histogramNameFormatFull = std::string( txtHistogramNameFormatFull->GetValue().mb_str() );

  histogramZoom = checkHistogramZoom->GetValue();
  histogramHorizontal = checkHistogramHorizontal->GetValue();
  histogramHideEmpty = checkHistogramHideEmpty->GetValue();
  histogramShowGradient = checkHistogramShowGradient->GetValue();
  histogramLabelsColor = checkHistogramLabelsColor->GetValue();

  histogramGradientFunction = ( PRV_UINT32 )choiceHistogramGradientFunction->GetCurrentSelection();
  histogramDrawmodeSemantic = ( PRV_UINT32 )choiceHistogramDrawmodeSemantic->GetCurrentSelection();
  histogramDrawmodeObjects = ( PRV_UINT32 )choiceHistogramDrawmodeObjects->GetCurrentSelection();

  histogramScientificNotation = checkHistogramScientificNotation->GetValue();
  histogramThousandSeparator = checkHistogramThousandsSeparator->GetValue();
  histogramShowUnits = checkHistogramShowUnits->IsChecked();
  histogramPrecision = ( PRV_UINT32 )txtHistogramPrecision->GetValue();

  histogramAutofitControlScale = checkHistogramAutofitControlScale->GetValue();
  histogramAutofit3DScale = checkHistogramAutofit3DScale->GetValue();
  histogramAutofitDataGradient = checkHistogramAutofitDataGradient->GetValue();
  histogramNumColumns = ( THistogramColumn )txtHistogramNumColumns->GetValue();

  histogramSaveImageFormat = ( PRV_UINT32 )choiceHistogramSaveImageFormat->GetCurrentSelection();
  histogramSaveTextFormat = ( PRV_UINT32 )choiceHistogramSaveTextFormat->GetCurrentSelection();

  // COLORS
  timelineColourBackground = wxColourToRGB( colourPickerBackground->GetColour() );
  timelineColourAxis       = wxColourToRGB( colourPickerAxis->GetColour() );
  colorUseZero             = checkZero->IsChecked();
  timelineColourZero       = wxColourToRGB( colourPickerZero->GetColour() );
  timelineColourLogical    = wxColourToRGB( colourPickerLogical->GetColour() );
  timelineColourPhysical   = wxColourToRGB( colourPickerPhysical->GetColour() );

  gradientColourBegin      = wxColourToRGB( colourPickerGradientBegin->GetColour() );
  gradientColourEnd        = wxColourToRGB( colourPickerGradientEnd->GetColour() );
  gradientColourLow        = wxColourToRGB( colourPickerGradientLow->GetColour() );
  gradientColourTop        = wxColourToRGB( colourPickerGradientTop->GetColour() );

  // WORKSPACES
  WorkspaceManager::getInstance()->clear();
  for( int i = 0; i < listWorkspaces->GetCount(); ++i )
    WorkspaceManager::getInstance()->addWorkspace( workspaceContainer[ listWorkspaces->GetString( i ) ] );
  
  return true;
}

rgb PreferencesDialog::wxColourToRGB( wxColour colour )
{
  rgb current;

  current.red   = colour.Red();
  current.green = colour.Green();
  current.blue  = colour.Blue();

  return current;
}

wxColour PreferencesDialog::RGBTowxColour( rgb colour )
{
  return wxColour( colour.red, colour.green, colour.blue );
}



/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DEFAULT_TIMELINE
 */

void PreferencesDialog::OnButtonDefaultTimelineClick( wxCommandEvent& event )
{
  colourPickerBackground->SetColour( RGBTowxColour( SemanticColor::BACKGROUND ) );
  colourPickerAxis->SetColour( RGBTowxColour( SemanticColor::FOREGROUND ) );
  colourPickerZero->SetColour( RGBTowxColour( SemanticColor::BACKGROUND ) );
  colourPickerLogical->SetColour( RGBTowxColour( SemanticColor::DEFAULT_LOGICAL_COMMUNICATIONS ) );
  colourPickerPhysical->SetColour( RGBTowxColour( SemanticColor::DEFAULT_PHYSICAL_COMMUNICATIONS ) );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DEFAULT_GRADIENT
 */

void PreferencesDialog::OnButtonDefaultGradientClick( wxCommandEvent& event )
{
  colourPickerGradientBegin->SetColour( RGBTowxColour( SemanticColor::DEFAULT_BEGIN_GRADIENT_COLOR ) );
  colourPickerGradientEnd->SetColour( RGBTowxColour( SemanticColor::DEFAULT_END_GRADIENT_COLOR ) );
  colourPickerGradientLow->SetColour( RGBTowxColour( SemanticColor::DEFAULT_BELOW_OUTLIER_COLOR ) );
  colourPickerGradientTop->SetColour( RGBTowxColour( SemanticColor::DEFAULT_ABOVE_OUTLIER_COLOR ) );
}


/*!
 * wxEVT_COLOURPICKER_CHANGED event handler for ID_COLOURPICKER_BACKGROUND
 */

void PreferencesDialog::OnColourpickerBackgroundColourPickerChanged( wxColourPickerEvent& event )
{
  // RGB Complementary
  rgb tmpBackground = wxColourToRGB( colourPickerBackground->GetColour() );

  tmpBackground.red   = 255 - tmpBackground.red;
  tmpBackground.green = 255 - tmpBackground.green;
  tmpBackground.blue  = 255 - tmpBackground.blue;

  colourPickerAxis->SetColour( RGBTowxColour( tmpBackground ) );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_COLOURPICKER_ZERO
 */

void PreferencesDialog::OnColourpickerZeroUpdate( wxUpdateUIEvent& event )
{
  event.Enable( checkZero->IsChecked() );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_WORKSPACES_DELETE
 */

void PreferencesDialog::OnButtonWorkspacesDeleteUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listWorkspaces->GetSelection() != wxNOT_FOUND );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_WORKSPACES_UP
 */

void PreferencesDialog::OnButtonWorkspacesUpUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listWorkspaces->GetSelection() != wxNOT_FOUND &&
                listWorkspaces->GetSelection() > 0 );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_WORKSPACES_DOWN
 */

void PreferencesDialog::OnButtonWorkspacesDownUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listWorkspaces->GetSelection() != wxNOT_FOUND &&
                listWorkspaces->GetSelection() < listWorkspaces->GetCount() - 1 );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TEXT_WORKSPACE_NAME
 */

void PreferencesDialog::OnTextWorkspaceNameUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listWorkspaces->GetSelection() != wxNOT_FOUND );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_LISTBOX_HINTS_WORKSPACE
 */

void PreferencesDialog::OnListboxHintsWorkspaceUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listWorkspaces->GetSelection() != wxNOT_FOUND );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_HINT_ADD
 */

void PreferencesDialog::OnButtonHintAddUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listWorkspaces->GetSelection() != wxNOT_FOUND );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_HINT_DELETE
 */

void PreferencesDialog::OnButtonHintDeleteUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listWorkspaces->GetSelection() != wxNOT_FOUND &&
                listHintsWorkspace->GetSelection() != wxNOT_FOUND );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BITMAP_HINT_UP
 */

void PreferencesDialog::OnBitmapHintUpUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listWorkspaces->GetSelection() != wxNOT_FOUND &&
                listHintsWorkspace->GetSelection() != wxNOT_FOUND &&
                listHintsWorkspace->GetSelection() > 0 );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_HINT_DOWN
 */

void PreferencesDialog::OnButtonHintDownUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listWorkspaces->GetSelection() != wxNOT_FOUND &&
                listHintsWorkspace->GetSelection() != wxNOT_FOUND &&
                listHintsWorkspace->GetSelection() < listHintsWorkspace->GetCount() - 1 );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TEXTCTRL_WORKSPACE_HINT_PATH
 */

void PreferencesDialog::OnTextctrlWorkspaceHintPathUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listWorkspaces->GetSelection() != wxNOT_FOUND &&
                listHintsWorkspace->GetSelection() != wxNOT_FOUND );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_FILE_BUTTON_WORKSPACE_HINT_PATH
 */

void PreferencesDialog::OnFileButtonWorkspaceHintPathUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listWorkspaces->GetSelection() != wxNOT_FOUND &&
                listHintsWorkspace->GetSelection() != wxNOT_FOUND );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TEXTCTRL_WORKSPACE_HINT_DESCRIPTION
 */

void PreferencesDialog::OnTextctrlWorkspaceHintDescriptionUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listWorkspaces->GetSelection() != wxNOT_FOUND &&
                listHintsWorkspace->GetSelection() != wxNOT_FOUND );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_WORKSPACES_ADD
 */

void PreferencesDialog::OnButtonWorkspacesAddClick( wxCommandEvent& event )
{
  int n = 1;
  wxString workspaceName = wxString( _( "New Workspace " ) ) + wxString::Format( _( "%d" ), n );
  while( listWorkspaces->FindString( workspaceName ) != wxNOT_FOUND )
    workspaceName = wxString( _( "New Workspace " ) ) + wxString::Format( _( "%d" ), ++n );
  listWorkspaces->Append( workspaceName );
  workspaceContainer.insert( std::pair<wxString,Workspace>( workspaceName, Workspace( std::string( workspaceName.mb_str() ) ) ) );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_WORKSPACES_DELETE
 */

void PreferencesDialog::OnButtonWorkspacesDeleteClick( wxCommandEvent& event )
{
  workspaceContainer.erase( listWorkspaces->GetStringSelection() );
  listWorkspaces->Delete( listWorkspaces->GetSelection() );
}


/*!
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX_WORKSPACES
 */

void PreferencesDialog::OnListboxWorkspacesSelected( wxCommandEvent& event )
{
  if( listWorkspaces->GetSelection() == wxNOT_FOUND )
  {
    return;
  }
  
  txtWorkspaceName->ChangeValue( listWorkspaces->GetStringSelection() );
  Workspace& currentWrk = workspaceContainer[ listWorkspaces->GetStringSelection() ];
  
  std::vector<TEventType> tmpAutoTypes = currentWrk.getAutoTypes();
  wxString formatAutoTypes;
  for( std::vector<TEventType>::iterator it = tmpAutoTypes.begin(); it != tmpAutoTypes.end(); ++it )
    formatAutoTypes<<*it<<wxT( ";" );
  formatAutoTypes.RemoveLast();
  txtAutoTypes->ChangeValue( formatAutoTypes );
  
  listHintsWorkspace->Clear();
  std::vector<std::pair<std::string,std::string> > hints = currentWrk.getHintCFGs();
  for( std::vector<std::pair<std::string,std::string> >::iterator it = hints.begin(); 
       it != hints.end(); ++it )
    listHintsWorkspace->Append( paraverMain::getHintComposed( *it ) );
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXT_WORKSPACE_NAME
 */

void PreferencesDialog::OnTextWorkspaceNameTextUpdated( wxCommandEvent& event )
{
  Workspace tmpWrk = workspaceContainer[ listWorkspaces->GetStringSelection() ];
  workspaceContainer.erase( listWorkspaces->GetStringSelection() );
  std::string tmpName = std::string( event.GetString().mb_str() );
  tmpWrk.setName( tmpName );
  workspaceContainer.insert( std::pair<wxString,Workspace>( event.GetString(), tmpWrk ) );
  listWorkspaces->SetString( listWorkspaces->GetSelection(), event.GetString() );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_WORKSPACES_UP
 */

void PreferencesDialog::OnButtonWorkspacesUpClick( wxCommandEvent& event )
{
  int tmpSel = listWorkspaces->GetSelection();
  wxArrayString items = listWorkspaces->GetStrings();
  wxString tmpStr = items[ tmpSel ];
  items[ tmpSel ] = items[ tmpSel - 1 ];
  items[ tmpSel - 1 ] = tmpStr;
  listWorkspaces->Set( items );
  listWorkspaces->SetSelection( tmpSel - 1 );
  txtWorkspaceName->ChangeValue( listWorkspaces->GetStringSelection() );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_WORKSPACES_DOWN
 */

void PreferencesDialog::OnButtonWorkspacesDownClick( wxCommandEvent& event )
{
  int tmpSel = listWorkspaces->GetSelection();
  wxArrayString items = listWorkspaces->GetStrings();
  wxString tmpStr = items[ tmpSel ];
  items[ tmpSel ] = items[ tmpSel + 1 ];
  items[ tmpSel + 1 ] = tmpStr;
  listWorkspaces->Set( items );
  listWorkspaces->SetSelection( tmpSel + 1 );
  txtWorkspaceName->ChangeValue( listWorkspaces->GetStringSelection() );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_HINT_ADD
 */

void PreferencesDialog::OnButtonHintAddClick( wxCommandEvent& event )
{
  wxString tmpPath( _( "/some/path/new_hint.cfg" ) );
  wxString tmpDesc( _( "New hint description..." ) );
  std::pair< std::string, std::string > tmpHint = std::pair< std::string, std::string >( std::string( tmpPath.mb_str() ), std::string( tmpDesc.mb_str() ) );
  workspaceContainer[ listWorkspaces->GetStringSelection() ].addHintCFG( tmpHint );
  listHintsWorkspace->Append( paraverMain::getHintComposed( tmpHint ) );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_HINT_DELETE
 */

void PreferencesDialog::OnButtonHintDeleteClick( wxCommandEvent& event )
{
  workspaceContainer[ listWorkspaces->GetStringSelection() ].removeHintCFG( listHintsWorkspace->GetSelection() );
  listHintsWorkspace->Delete( listHintsWorkspace->GetSelection() );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAP_HINT_UP
 */

void PreferencesDialog::OnBitmapHintUpClick( wxCommandEvent& event )
{
  int tmpSel = listHintsWorkspace->GetSelection();
  Workspace& tmpWrk = workspaceContainer[ listWorkspaces->GetStringSelection() ];
  wxArrayString items = listHintsWorkspace->GetStrings();
  wxString tmpStr = items[ tmpSel ];
  items[ tmpSel ] = items[ tmpSel - 1 ];
  items[ tmpSel - 1 ] = tmpStr;
  listHintsWorkspace->Set( items );
  listHintsWorkspace->SetSelection( tmpSel - 1 );
  
  std::pair< std::string, std::string > tmpHint = tmpWrk.getHintCFG( tmpSel );
  std::pair< std::string, std::string > tmpHintPrev = std::pair< std::string, std::string >( tmpWrk.getHintCFG( tmpSel - 1 ) );
  tmpWrk.modifyHintCFG( tmpSel, tmpHintPrev );
  tmpWrk.modifyHintCFG( tmpSel - 1, tmpHint );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_HINT_DOWN
 */

void PreferencesDialog::OnButtonHintDownClick( wxCommandEvent& event )
{
  int tmpSel = listHintsWorkspace->GetSelection();
  Workspace& tmpWrk = workspaceContainer[ listWorkspaces->GetStringSelection() ];
  wxArrayString items = listHintsWorkspace->GetStrings();
  wxString tmpStr = items[ tmpSel ];
  items[ tmpSel ] = items[ tmpSel + 1 ];
  items[ tmpSel + 1 ] = tmpStr;
  listHintsWorkspace->Set( items );
  listHintsWorkspace->SetSelection( tmpSel + 1 );
  
  std::pair< std::string, std::string > tmpHint = tmpWrk.getHintCFG( tmpSel );
  std::pair< std::string, std::string > tmpHintNext = std::pair< std::string, std::string >( tmpWrk.getHintCFG( tmpSel + 1 ) );
  tmpWrk.modifyHintCFG( tmpSel, tmpHintNext );
  tmpWrk.modifyHintCFG( tmpSel + 1, tmpHint );
}


/*!
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX_HINTS_WORKSPACE
 */

void PreferencesDialog::OnListboxHintsWorkspaceSelected( wxCommandEvent& event )
{
  if( listHintsWorkspace->GetSelection() == wxNOT_FOUND )
  {
    return;
  }
  std::pair< std::string, std::string > tmpHint = workspaceContainer[ listWorkspaces->GetStringSelection() ]
                                                  .getHintCFG( listHintsWorkspace->GetSelection() );
  txtHintDescription->ChangeValue( wxString::FromAscii( tmpHint.second.c_str() ) );
  fileBrowserHintPath->ChangePath( wxString::FromAscii( tmpHint.first.c_str() ) );
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_WORKSPACE_HINT_PATH
 */

void PreferencesDialog::OnTextctrlWorkspaceHintPathTextUpdated( wxCommandEvent& event )
{
  if( listHintsWorkspace->GetSelection() == wxNOT_FOUND )
    return;

  std::pair< std::string, std::string > tmpHint = std::pair< std::string, std::string >( 
                                                    std::string( fileBrowserHintPath->GetPath().mb_str() ),
                                                    std::string( txtHintDescription->GetValue().mb_str() ) );
  workspaceContainer[ listWorkspaces->GetStringSelection() ].modifyHintCFG( listHintsWorkspace->GetSelection(), tmpHint );
  listHintsWorkspace->SetString( listHintsWorkspace->GetSelection(), paraverMain::getHintComposed( tmpHint ) );
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_WORKSPACE_HINT_DESCRIPTION
 */

void PreferencesDialog::OnTextctrlWorkspaceHintDescriptionTextUpdated( wxCommandEvent& event )
{
  if( listHintsWorkspace->GetSelection() == wxNOT_FOUND )
    return;

  std::pair< std::string, std::string > tmpHint = std::pair< std::string, std::string >( 
                                                    std::string( fileBrowserHintPath->GetPath().mb_str() ),
                                                    std::string( txtHintDescription->GetValue().mb_str() ) );
  workspaceContainer[ listWorkspaces->GetStringSelection() ].modifyHintCFG( listHintsWorkspace->GetSelection(), tmpHint );
  listHintsWorkspace->SetString( listHintsWorkspace->GetSelection(), paraverMain::getHintComposed( tmpHint ) );
}




/*!
 * wxEVT_UPDATE_UI event handler for ID_STATIC_WORKSPACE_AUTOTYPES
 */

void PreferencesDialog::OnStaticWorkspaceAutotypesUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listWorkspaces->GetSelection() != wxNOT_FOUND );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TEXT_WORKSPACE_AUTOTYPES
 */

void PreferencesDialog::OnTextWorkspaceAutotypesUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listWorkspaces->GetSelection() != wxNOT_FOUND );
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXT_WORKSPACE_AUTOTYPES
 */

void PreferencesDialog::OnTextWorkspaceAutotypesTextUpdated( wxCommandEvent& event )
{
  Workspace tmpWrk = workspaceContainer[ listWorkspaces->GetStringSelection() ];
  std::string tmpTxt = std::string( event.GetString().mb_str() );
  vector<TEventType> tmpAutoTypes;
  tmpWrk.setAutoTypes( tmpAutoTypes );
}

