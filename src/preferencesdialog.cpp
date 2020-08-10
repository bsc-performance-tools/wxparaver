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

// Tokenizer
#include <wx/tokenzr.h>

#include <wx/filename.h>

////@begin XPM images
#include "../icons/derived_add.xpm"
#include "../icons/delete.xpm"
#include "../icons/arrow_up.xpm"
#include "../icons/arrow_down.xpm"
#include "../icons/report_add.xpm"
#include "../icons/report_disk.xpm"
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
  EVT_SPINCTRL( ID_PREFERENCES_GLOBAL_TIME_SESSION, PreferencesDialog::OnPreferencesGlobalTimeSessionUpdated )
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
  EVT_BUTTON( ID_BUTTON_WORKSPACES_IMPORT, PreferencesDialog::OnButtonWorkspacesImportClick )
  EVT_UPDATE_UI( ID_BUTTON_WORKSPACES_IMPORT, PreferencesDialog::OnButtonWorkspacesImportUpdate )
  EVT_BUTTON( ID_BUTTON_WORKSPACES_EXPORT, PreferencesDialog::OnButtonWorkspacesExportClick )
  EVT_UPDATE_UI( ID_BUTTON_WORKSPACES_EXPORT, PreferencesDialog::OnButtonWorkspacesExportUpdate )
  EVT_TEXT( ID_TEXT_WORKSPACE_NAME, PreferencesDialog::OnTextWorkspaceNameTextUpdated )
  EVT_UPDATE_UI( ID_TEXT_WORKSPACE_NAME, PreferencesDialog::OnTextWorkspaceNameUpdate )
  EVT_RADIOBUTTON( ID_RADIOSTATES, PreferencesDialog::OnRadiostatesSelected )
  EVT_UPDATE_UI( ID_RADIOSTATES, PreferencesDialog::OnRadiostatesUpdate )
  EVT_RADIOBUTTON( ID_RADIOEVENTYPES, PreferencesDialog::OnRadioeventypesSelected )
  EVT_UPDATE_UI( ID_RADIOEVENTYPES, PreferencesDialog::OnRadioeventypesUpdate )
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
  EVT_LISTBOX( ID_LISTBOX_TEXT_EDITORS, PreferencesDialog::OnListboxTextEditorsSelected )
  EVT_BUTTON( ID_BUTTON_TXT_ADD, PreferencesDialog::OnButtonTxtAddClick )
  EVT_BUTTON( ID_BUTTON_TXT_DEL, PreferencesDialog::OnButtonTxtDelClick )
  EVT_UPDATE_UI( ID_BUTTON_TXT_DEL, PreferencesDialog::OnButtonTxtDelUpdate )
  EVT_BUTTON( ID_BUTTON_TXT_UP, PreferencesDialog::OnButtonTxtUpClick )
  EVT_UPDATE_UI( ID_BUTTON_TXT_UP, PreferencesDialog::OnButtonTxtUpUpdate )
  EVT_BUTTON( ID_BUTTON_TXT_DOWN, PreferencesDialog::OnButtonTxtDownClick )
  EVT_UPDATE_UI( ID_BUTTON_TXT_DOWN, PreferencesDialog::OnButtonTxtDownUpdate )
  EVT_LISTBOX( ID_LISTBOX_PDF_READERS, PreferencesDialog::OnListboxPdfReadersSelected )
  EVT_BUTTON( ID_BUTTON_PDF_ADD, PreferencesDialog::OnButtonPdfAddClick )
  EVT_BUTTON( ID_BUTTON_PDF_DEL, PreferencesDialog::OnButtonPdfDelClick )
  EVT_UPDATE_UI( ID_BUTTON_PDF_DEL, PreferencesDialog::OnButtonPdfDelUpdate )
  EVT_BUTTON( ID_BUTTON_PDF_UP, PreferencesDialog::OnButtonPdfUpClick )
  EVT_UPDATE_UI( ID_BUTTON_PDF_UP, PreferencesDialog::OnButtonPdfUpUpdate )
  EVT_BUTTON( ID_BUTTON_PDF_DOWN, PreferencesDialog::OnButtonPdfDownClick )
  EVT_UPDATE_UI( ID_BUTTON_PDF_DOWN, PreferencesDialog::OnButtonPdfDownUpdate )
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
}


/*!
 * Member initialisation
 */

void PreferencesDialog::Init()
{
////@begin PreferencesDialog member initialisation
  cfgsPath = "";
  colorUseZero = false;
  disableTimelineZoomMouseWheel = false;
  filtersXMLPath = "";
  globalFillStateGaps = false;
  globalFullTracePath = false;
  helpContentsUsesBrowser = false;
  histogramAutofit3DScale = true;
  histogramAutofitControlScale = true;
  histogramAutofitControlScaleZero = false;
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
  histogramSkipCreateDialog = false;
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
  timelineSemanticScaleMinAtZero = false;
  panelGlobal = NULL;
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
  checkGlobalAskForPrevSessionLoad = NULL;
  checkGlobalHelpOnBrowser = NULL;
  checkDisableTimelineZoomMouseWheel = NULL;
  panelTimeline = NULL;
  txtTimelineNameFormatPrefix = NULL;
  txtTimelineNameFormatFull = NULL;
  checkTimelineCommunicationLines = NULL;
  checkTimelineEventLines = NULL;
  checkSemanticScaleMinAtZero = NULL;
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
  panelHistogram = NULL;
  checkHistogramSkipCreateDialog = NULL;
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
  checkHistogramAutofitControlScaleZero = NULL;
  checkHistogramAutofit3DScale = NULL;
  checkHistogramAutofitDataGradient = NULL;
  txtHistogramNumColumns = NULL;
  choiceHistogramSaveImageFormat = NULL;
  choiceHistogramSaveTextFormat = NULL;
  panelColor = NULL;
  colourPickerBackground = NULL;
  colourPickerAxis = NULL;
  checkZero = NULL;
  colourPickerZero = NULL;
  colourPickerPunctual = NULL;
  colourPickerLogical = NULL;
  colourPickerPhysical = NULL;
  colourPickerGradientBegin = NULL;
  colourPickerGradientEnd = NULL;
  colourPickerNegativeGradientBegin = NULL;
  colourPickerNegativeGradientEnd = NULL;
  colourPickerGradientLow = NULL;
  colourPickerGradientTop = NULL;
  panelWorkspaces = NULL;
  listWorkspaces = NULL;
  buttonAddWorkspace = NULL;
  buttonDeleteWorkspace = NULL;
  buttonUpWorkspace = NULL;
  buttonDownWorkspace = NULL;
  buttonImportWorkspace = NULL;
  buttonExportWorkspace = NULL;
  txtWorkspaceName = NULL;
  radioStates = NULL;
  radioEventTypes = NULL;
  txtAutoTypes = NULL;
  listHintsWorkspace = NULL;
  buttonAddHint = NULL;
  buttonDeleteHint = NULL;
  buttonUpHint = NULL;
  buttonDownHint = NULL;
  txtHintPath = NULL;
  fileBrowserHintPath = NULL;
  txtHintDescription = NULL;
  panelExternal = NULL;
  listTextEditors = NULL;
  buttonAddTextEditor = NULL;
  buttonDeleteTextEditor = NULL;
  buttonUpTextEditor = NULL;
  buttonDownTextEditor = NULL;
  listPDFReaders = NULL;
  buttonAddPDFReader = NULL;
  buttonDeletePDFReader = NULL;
  buttonUpPDFReader = NULL;
  buttonDownPDFReader = NULL;
  panelFilters = NULL;
////@end PreferencesDialog member initialisation
}


/*!
 * Control creation for PreferencesDialog
 */

void PreferencesDialog::CreateControls()
{    
////@begin PreferencesDialog content construction
  PreferencesDialog* itemPropertySheetDialog1 = this;

  panelGlobal = new wxPanel( GetBookCtrl(), ID_PREFERENCES_GLOBAL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
  panelGlobal->SetSizer(itemBoxSizer3);

  wxStaticBox* itemStaticBoxSizer4Static = new wxStaticBox(panelGlobal, wxID_ANY, _("  Trace  "));
  wxStaticBoxSizer* itemStaticBoxSizer4 = new wxStaticBoxSizer(itemStaticBoxSizer4Static, wxVERTICAL);
  itemBoxSizer3->Add(itemStaticBoxSizer4, 0, wxGROW|wxALL, 5);
  checkGlobalFillStateGaps = new wxCheckBox( panelGlobal, ID_PREFERENCES_GLOBAL_FILLGAPS, _("Fill State gaps with IDLE State"), wxDefaultPosition, wxDefaultSize, 0 );
  checkGlobalFillStateGaps->SetValue(false);
  itemStaticBoxSizer4->Add(checkGlobalFillStateGaps, 1, wxGROW|wxALL, 2);

  checkGlobalFullTracePath = new wxCheckBox( panelGlobal, ID_PREFERENCES_GLOBAL_FULLTRACEPATH, _("View full path in trace selector"), wxDefaultPosition, wxDefaultSize, 0 );
  checkGlobalFullTracePath->SetValue(false);
  itemStaticBoxSizer4->Add(checkGlobalFullTracePath, 1, wxGROW|wxALL, 2);

  wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer4->Add(itemBoxSizer7, 0, wxGROW|wxALL, 2);
  wxStaticText* itemStaticText8 = new wxStaticText( panelGlobal, wxID_STATIC, _("Maximum loadable trace size (MB)"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer7->Add(itemStaticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txtMaximumTraceSize = new wxSpinCtrl( panelGlobal, ID_TEXTCTRL_MAXIMUM_LOADABLE_TRACE_SIZE, wxT("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 5000, 0 );
  itemBoxSizer7->Add(txtMaximumTraceSize, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer10Static = new wxStaticBox(panelGlobal, wxID_ANY, _("  Default directories  "));
  wxStaticBoxSizer* itemStaticBoxSizer10 = new wxStaticBoxSizer(itemStaticBoxSizer10Static, wxVERTICAL);
  itemBoxSizer3->Add(itemStaticBoxSizer10, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer10->Add(itemBoxSizer11, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText12 = new wxStaticText( panelGlobal, wxID_STATIC, _("Traces"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText12->SetToolTip(_("Base path to traces files (.prv, .prv.gz, .pcf and .row)."));
  itemBoxSizer11->Add(itemStaticText12, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  textCtrlTrace = new wxTextCtrl( panelGlobal, ID_TEXTCTRL_DEFAULT_TRACE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    textCtrlTrace->SetToolTip(_("Base path to traces files (.prv, .prv.gz, .pcf and .row)."));
  itemBoxSizer11->Add(textCtrlTrace, 5, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  dirBrowserButtonTrace = new DirBrowserButton( panelGlobal, ID_BUTTON_DIR_BROWSER_TRACE, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    dirBrowserButtonTrace->SetToolTip(_("Base path to traces files (.prv, .prv.gz, .pcf and .row)."));
  itemBoxSizer11->Add(dirBrowserButtonTrace, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer10->Add(itemBoxSizer15, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText16 = new wxStaticText( panelGlobal, wxID_STATIC, _("CFGs"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText16->SetToolTip(_("Base path to configuration files (.cfg)."));
  itemBoxSizer15->Add(itemStaticText16, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  textCtrlCFG = new wxTextCtrl( panelGlobal, ID_TEXTCTRL_DEFAULT_CFGS, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    textCtrlCFG->SetToolTip(_("Base path to configuration files (.cfg)."));
  itemBoxSizer15->Add(textCtrlCFG, 5, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  dirBrowserButtonCFG = new DirBrowserButton( panelGlobal, ID_DIRBROWSERBUTTON_DEFAULT_CFGS, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    dirBrowserButtonCFG->SetToolTip(_("Base path to configuration files (.cfg)."));
  itemBoxSizer15->Add(dirBrowserButtonCFG, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer10->Add(itemBoxSizer19, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText20 = new wxStaticText( panelGlobal, wxID_STATIC, _("Filters XML"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText20->SetToolTip(_("Base path to Cut/Filters XML."));
  itemBoxSizer19->Add(itemStaticText20, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  textCtrlXML = new wxTextCtrl( panelGlobal, ID_TEXTCTRL_DEFAULT_XMLS, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    textCtrlXML->SetToolTip(_("Base path to Cut/Filters XML."));
  itemBoxSizer19->Add(textCtrlXML, 5, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  dirBrowserButtonXML = new DirBrowserButton( panelGlobal, ID_BUTTON_DEFAULT_XMLS, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    dirBrowserButtonXML->SetToolTip(_("Base path to Cut/Filters XML."));
  itemBoxSizer19->Add(dirBrowserButtonXML, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer10->Add(itemBoxSizer23, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText24 = new wxStaticText( panelGlobal, wxID_STATIC, _("Tutorials root"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText24->SetToolTip(_("Base path to wxparaver tutorials."));
  itemBoxSizer23->Add(itemStaticText24, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  textCtrlTutorials = new wxTextCtrl( panelGlobal, ID_TEXTCTRL_DEFAULT_TUTORIALS, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    textCtrlTutorials->SetToolTip(_("Base path to wxparaver tutorials."));
  itemBoxSizer23->Add(textCtrlTutorials, 5, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  dirBrowserButtonTutorials = new DirBrowserButton( panelGlobal, ID_DIRBROWSERBUTTON_DEFAULT_TUTORIALS, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    dirBrowserButtonTutorials->SetToolTip(_("Base path to wxparaver tutorials."));
  itemBoxSizer23->Add(dirBrowserButtonTutorials, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer27 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer10->Add(itemBoxSizer27, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText28 = new wxStaticText( panelGlobal, wxID_STATIC, _("Tmp dir"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText28->SetToolTip(_("Working directory for results and logs."));
  itemBoxSizer27->Add(itemStaticText28, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  textCtrlTmp = new wxTextCtrl( panelGlobal, ID_TEXTCTRL_DEFAULT_TMP, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    textCtrlTmp->SetToolTip(_("Working directory for results and logs."));
  itemBoxSizer27->Add(textCtrlTmp, 5, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  dirBrowserButtonTmp = new DirBrowserButton( panelGlobal, ID_DIRBROWSERBUTTON_DEFAULT_TMP, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    dirBrowserButtonTmp->SetToolTip(_("Working directory for results and logs."));
  itemBoxSizer27->Add(dirBrowserButtonTmp, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer31Static = new wxStaticBox(panelGlobal, wxID_ANY, _("  Behaviour  "));
  wxStaticBoxSizer* itemStaticBoxSizer31 = new wxStaticBoxSizer(itemStaticBoxSizer31Static, wxVERTICAL);
  itemBoxSizer3->Add(itemStaticBoxSizer31, 0, wxGROW|wxALL, 5);
  checkGlobalSingleInstance = new wxCheckBox( panelGlobal, ID_PREFERENCES_GLOBAL_SINGLE_INSTANCE, _("Allow only one running instance"), wxDefaultPosition, wxDefaultSize, 0 );
  checkGlobalSingleInstance->SetValue(false);
  itemStaticBoxSizer31->Add(checkGlobalSingleInstance, 1, wxGROW|wxALL, 2);

  wxBoxSizer* itemBoxSizer33 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer31->Add(itemBoxSizer33, 1, wxGROW|wxALL, 2);
  wxStaticText* itemStaticText34 = new wxStaticText( panelGlobal, wxID_STATIC, _("Automatically save session every"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer33->Add(itemStaticText34, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  spinSessionTime = new wxSpinCtrl( panelGlobal, ID_PREFERENCES_GLOBAL_TIME_SESSION, wxT("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
  itemBoxSizer33->Add(spinSessionTime, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticText* itemStaticText36 = new wxStaticText( panelGlobal, wxID_STATIC, _("minutes"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer33->Add(itemStaticText36, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  checkGlobalAskForPrevSessionLoad = new wxCheckBox( panelGlobal, ID_GLOBAL_ASK_FOR_PREV_SESSION, _("Show dialog for crashed auto-saved sessions on startup"), wxDefaultPosition, wxDefaultSize, 0 );
  checkGlobalAskForPrevSessionLoad->SetValue(true);
  itemStaticBoxSizer31->Add(checkGlobalAskForPrevSessionLoad, 1, wxGROW|wxALL, 2);

  checkGlobalHelpOnBrowser = new wxCheckBox( panelGlobal, ID_HELP_CONTENTS_IN_BROWSER, _("Show help contents on a browser"), wxDefaultPosition, wxDefaultSize, 0 );
  checkGlobalHelpOnBrowser->SetValue(false);
  itemStaticBoxSizer31->Add(checkGlobalHelpOnBrowser, 1, wxGROW|wxALL, 2);

  checkDisableTimelineZoomMouseWheel = new wxCheckBox( panelGlobal, ID_DISABLE_TIMELINE_ZOOM_MOUSE_WHEEL, _("Disable timeline zoom with mouse wheel"), wxDefaultPosition, wxDefaultSize, 0 );
  checkDisableTimelineZoomMouseWheel->SetValue(false);
  itemStaticBoxSizer31->Add(checkDisableTimelineZoomMouseWheel, 1, wxGROW|wxALL, 2);

  GetBookCtrl()->AddPage(panelGlobal, _("Global"));

  panelTimeline = new wxPanel( GetBookCtrl(), ID_PREFERENCES_TIMELINE, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer38 = new wxBoxSizer(wxHORIZONTAL);
  panelTimeline->SetSizer(itemBoxSizer38);

  wxBoxSizer* itemBoxSizer39 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer38->Add(itemBoxSizer39, 3, wxGROW|wxTOP, 5);
  wxStaticBox* itemStaticBoxSizer40Static = new wxStaticBox(panelTimeline, wxID_STATIC, _("  Name Format  "));
  wxStaticBoxSizer* itemStaticBoxSizer40 = new wxStaticBoxSizer(itemStaticBoxSizer40Static, wxVERTICAL);
  itemStaticBoxSizer40Static->Enable(false);
  itemBoxSizer39->Add(itemStaticBoxSizer40, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer41 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer40->Add(itemBoxSizer41, 0, wxGROW|wxTOP|wxBOTTOM, 5);
  wxStaticText* itemStaticText42 = new wxStaticText( panelTimeline, wxID_STATIC, _("Prefix"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText42->SetToolTip(_("Default name for new timelines, applied in window browser."));
  itemStaticText42->Enable(false);
  itemBoxSizer41->Add(itemStaticText42, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  txtTimelineNameFormatPrefix = new wxTextCtrl( panelTimeline, ID_PREFERENCES_TIMELINE_NAME_PREFIX, _("New Window # %N"), wxDefaultPosition, wxDefaultSize, 0 );
  txtTimelineNameFormatPrefix->SetMaxLength(256);
  if (PreferencesDialog::ShowToolTips())
    txtTimelineNameFormatPrefix->SetToolTip(_("Default name for new timelines, applied in window browser."));
  txtTimelineNameFormatPrefix->Enable(false);
  itemBoxSizer41->Add(txtTimelineNameFormatPrefix, 2, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer44 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer40->Add(itemBoxSizer44, 0, wxGROW|wxBOTTOM, 5);
  wxStaticText* itemStaticText45 = new wxStaticText( panelTimeline, wxID_STATIC, _("Full"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText45->SetToolTip(_("Default full name for new timelines, applied in window title."));
  itemStaticText45->Enable(false);
  itemBoxSizer44->Add(itemStaticText45, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  txtTimelineNameFormatFull = new wxTextCtrl( panelTimeline, ID_PREFERENCES_TIMELINE_NAME_FULL, _("%P @ %T"), wxDefaultPosition, wxDefaultSize, 0 );
  txtTimelineNameFormatFull->SetMaxLength(256);
  if (PreferencesDialog::ShowToolTips())
    txtTimelineNameFormatFull->SetToolTip(_("Default full name for new timelines, applied in window title."));
  txtTimelineNameFormatFull->Enable(false);
  itemBoxSizer44->Add(txtTimelineNameFormatFull, 2, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxStaticBox* itemStaticBoxSizer47Static = new wxStaticBox(panelTimeline, wxID_ANY, _("  Rendering Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer47 = new wxStaticBoxSizer(itemStaticBoxSizer47Static, wxVERTICAL);
  itemBoxSizer39->Add(itemStaticBoxSizer47, 1, wxGROW|wxALL, 5);
  checkTimelineCommunicationLines = new wxCheckBox( panelTimeline, ID_PREFERENCES_TIMELINE_COMMUNICATION_LINES, _("Communication Lines"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineCommunicationLines->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineCommunicationLines->SetToolTip(_("View communications lines in new timelines."));
  itemStaticBoxSizer47->Add(checkTimelineCommunicationLines, 0, wxGROW|wxTOP, 5);

  checkTimelineEventLines = new wxCheckBox( panelTimeline, ID_PREFERENCES_TIMELINE_EVENT_LINES, _("Event Flags"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineEventLines->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineEventLines->SetToolTip(_("View event flags in new timelines."));
  itemStaticBoxSizer47->Add(checkTimelineEventLines, 0, wxGROW, 5);

  checkSemanticScaleMinAtZero = new wxCheckBox( panelTimeline, ID_PREFERENCES_SEMANTIC_SCALE_MIN_AT_ZERO, _("Semantic Scale Min at Zero"), wxDefaultPosition, wxDefaultSize, 0 );
  checkSemanticScaleMinAtZero->SetValue(false);
  itemStaticBoxSizer47->Add(checkSemanticScaleMinAtZero, 0, wxGROW, 5);

  wxBoxSizer* itemBoxSizer50 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer47->Add(itemBoxSizer50, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText51 = new wxStaticText( panelTimeline, wxID_STATIC, _("Color"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText51->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer50->Add(itemStaticText51, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceTimelineColorStrings;
  choiceTimelineColorStrings.Add(_("Dummy Text"));
  choiceTimelineColor = new wxChoice( panelTimeline, ID_PREFERENCES_TIMELINE_COLOR, wxDefaultPosition, wxDefaultSize, choiceTimelineColorStrings, 0 );
  choiceTimelineColor->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineColor->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer50->Add(choiceTimelineColor, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer53 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer47->Add(itemBoxSizer53, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText54 = new wxStaticText( panelTimeline, wxID_STATIC, _("Gradient Function"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText54->SetToolTip(_("Gradient function selected in new timeline."));
  itemBoxSizer53->Add(itemStaticText54, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxArrayString choiceTimelineGradientFunctionStrings;
  choiceTimelineGradientFunctionStrings.Add(_("Dummy Text"));
  choiceTimelineGradientFunction = new wxChoice( panelTimeline, ID_PREFERENCES_TIMELINE_GRADIENT, wxDefaultPosition, wxDefaultSize, choiceTimelineGradientFunctionStrings, 0 );
  choiceTimelineGradientFunction->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineGradientFunction->SetToolTip(_("Gradient function selected in new timeline."));
  itemBoxSizer53->Add(choiceTimelineGradientFunction, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer56 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer47->Add(itemBoxSizer56, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText57 = new wxStaticText( panelTimeline, wxID_STATIC, _("Drawmode Time"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText57->SetToolTip(_("Drawmode for time axis selected in new timeline."));
  itemBoxSizer56->Add(itemStaticText57, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceTimelineDrawmodeTimeStrings;
  choiceTimelineDrawmodeTimeStrings.Add(_("Dummy Text"));
  choiceTimelineDrawmodeTime = new wxChoice( panelTimeline, ID_PREFERENCES_TIMELINE_DRAWMODE_TIME, wxDefaultPosition, wxDefaultSize, choiceTimelineDrawmodeTimeStrings, 0 );
  choiceTimelineDrawmodeTime->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineDrawmodeTime->SetToolTip(_("Drawmode for time axis selected in new timeline."));
  itemBoxSizer56->Add(choiceTimelineDrawmodeTime, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer59 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer47->Add(itemBoxSizer59, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText60 = new wxStaticText( panelTimeline, wxID_STATIC, _("Drawmode Objects"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText60->SetToolTip(_("Drawmode for objects axis selected in new timeline."));
  itemBoxSizer59->Add(itemStaticText60, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceTimelineDrawmodeObjectsStrings;
  choiceTimelineDrawmodeObjectsStrings.Add(_("Dummy Text"));
  choiceTimelineDrawmodeObjects = new wxChoice( panelTimeline, ID_PREFERENCES_TIMELINE_DRAWMODE_OBJECTS, wxDefaultPosition, wxDefaultSize, choiceTimelineDrawmodeObjectsStrings, 0 );
  choiceTimelineDrawmodeObjects->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineDrawmodeObjects->SetToolTip(_("Drawmode for objects axis selected in new timeline."));
  itemBoxSizer59->Add(choiceTimelineDrawmodeObjects, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer62 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer47->Add(itemBoxSizer62, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText63 = new wxStaticText( panelTimeline, wxID_STATIC, _("Pixel Size"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText63->SetToolTip(_("Pixel size selected in new timeline."));
  itemBoxSizer62->Add(itemStaticText63, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString choiceTimelinePixelSizeStrings;
  choiceTimelinePixelSizeStrings.Add(_("Dummy Text"));
  choiceTimelinePixelSize = new wxChoice( panelTimeline, ID_PREFERENCES_TIMELINE_PIXEL_SIZE, wxDefaultPosition, wxDefaultSize, choiceTimelinePixelSizeStrings, 0 );
  choiceTimelinePixelSize->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceTimelinePixelSize->SetToolTip(_("Pixel size selected in new timeline."));
  itemBoxSizer62->Add(choiceTimelinePixelSize, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer65 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer47->Add(itemBoxSizer65, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText66 = new wxStaticText( panelTimeline, wxID_STATIC, _("Labels to draw"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText66->SetToolTip(_("Labels to draw by default in a new timeline."));
  itemBoxSizer65->Add(itemStaticText66, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString choiceTimelineLabelsStrings;
  choiceTimelineLabelsStrings.Add(_("Dummy Text"));
  choiceTimelineLabels = new wxChoice( panelTimeline, ID_PREFERENCES_TIMELINE_LABELS, wxDefaultPosition, wxDefaultSize, choiceTimelineLabelsStrings, 0 );
  choiceTimelineLabels->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineLabels->SetToolTip(_("Labels to draw by default in a new timeline."));
  itemBoxSizer65->Add(choiceTimelineLabels, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer68 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer47->Add(itemBoxSizer68, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText69 = new wxStaticText( panelTimeline, wxID_STATIC, _("Object Axis Position"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer68->Add(itemStaticText69, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString choiceTimelineObjectAxisStrings;
  choiceTimelineObjectAxisStrings.Add(_("Dummy Text"));
  choiceTimelineObjectAxis = new wxChoice( panelTimeline, ID_PREFERENCES_TIMELINE_OBJECT_AXIS, wxDefaultPosition, wxDefaultSize, choiceTimelineObjectAxisStrings, 0 );
  choiceTimelineObjectAxis->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineObjectAxis->SetToolTip(_("Object axis position by default in a new timeline."));
  itemBoxSizer68->Add(choiceTimelineObjectAxis, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer71 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer38->Add(itemBoxSizer71, 2, wxGROW|wxTOP, 5);
  wxStaticBox* itemStaticBoxSizer72Static = new wxStaticBox(panelTimeline, wxID_ANY, _("  What / Where  "));
  wxStaticBoxSizer* itemStaticBoxSizer72 = new wxStaticBoxSizer(itemStaticBoxSizer72Static, wxVERTICAL);
  itemBoxSizer71->Add(itemStaticBoxSizer72, 2, wxGROW|wxALL, 5);
  checkTimelineWWSemantic = new wxCheckBox( panelTimeline, ID_CHECKBOX_TIMELINE_WW_SEMANTIC, _("Semantic"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWSemantic->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWSemantic->SetToolTip(_("Show semantic information when left click in timeline."));
  itemStaticBoxSizer72->Add(checkTimelineWWSemantic, 0, wxGROW|wxTOP, 5);

  checkTimelineWWEvents = new wxCheckBox( panelTimeline, ID_CHECKBOX_TIMELINE_WW_EVENTS, _("Events"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWEvents->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWEvents->SetToolTip(_("Show events information when left click in timeline."));
  itemStaticBoxSizer72->Add(checkTimelineWWEvents, 0, wxGROW, 5);

  checkTimelineWWCommunications = new wxCheckBox( panelTimeline, ID_CHECKBOX_TIMELINE_WW_COMMUNICATIONS, _("Communications"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWCommunications->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWCommunications->SetToolTip(_("Show communications information when left click in timeline."));
  itemStaticBoxSizer72->Add(checkTimelineWWCommunications, 0, wxGROW, 5);

  checkTimelineWWPreviousNext = new wxCheckBox( panelTimeline, ID_CHECKBOX_TIMELINE_WW_PREVIOUS_NEXT, _("Previous / Next"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWPreviousNext->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWPreviousNext->SetToolTip(_("In addition to current semantic value, show information for previous and next ones when left click in timeline."));
  itemStaticBoxSizer72->Add(checkTimelineWWPreviousNext, 0, wxGROW, 5);

  checkTimelineWWText = new wxCheckBox( panelTimeline, ID_CHECKBOX_TIMELINE_WW_TEXT, _("Text"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWText->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWText->SetToolTip(_("Show semantic value as text when left click in timeline."));
  itemStaticBoxSizer72->Add(checkTimelineWWText, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer78 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer72->Add(itemBoxSizer78, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText79 = new wxStaticText( panelTimeline, wxID_STATIC, _("Precision"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer78->Add(itemStaticText79, 2, wxALIGN_CENTER_VERTICAL, 5);

  txtTimelineWWPrecision = new wxSpinCtrl( panelTimeline, ID_PREFERENCES_TIMELINE_WW_PRECISION, wxT("2"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 2 );
  if (PreferencesDialog::ShowToolTips())
    txtTimelineWWPrecision->SetToolTip(_("Decimal precision for semantic values."));
  itemBoxSizer78->Add(txtTimelineWWPrecision, 3, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer81 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer72->Add(itemBoxSizer81, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText82 = new wxStaticText( panelTimeline, wxID_STATIC, _("Event pixels"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticText82->SetHelpText(_("Pixels to look around for events"));
  if (PreferencesDialog::ShowToolTips())
    itemStaticText82->SetToolTip(_("Pixels to look around for events"));
  itemBoxSizer81->Add(itemStaticText82, 2, wxALIGN_CENTER_VERTICAL, 5);

  txtTimelineWWEventPixels = new wxSpinCtrl( panelTimeline, ID_PREFERENCES_TIMELINE_WW_EVENT_PIXELS, wxT("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
  txtTimelineWWEventPixels->SetHelpText(_("Pixels to look around for events"));
  if (PreferencesDialog::ShowToolTips())
    txtTimelineWWEventPixels->SetToolTip(_("Pixels to look around for events"));
  itemBoxSizer81->Add(txtTimelineWWEventPixels, 3, wxALIGN_CENTER_VERTICAL, 5);

  wxStaticBox* itemStaticBoxSizer84Static = new wxStaticBox(panelTimeline, wxID_ANY, _("  Default Save Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer84 = new wxStaticBoxSizer(itemStaticBoxSizer84Static, wxVERTICAL);
  itemBoxSizer71->Add(itemStaticBoxSizer84, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer85 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer84->Add(itemBoxSizer85, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxStaticText* itemStaticText86 = new wxStaticText( panelTimeline, wxID_STATIC, _("Image as"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText86->SetToolTip(_("Default image format selected."));
  itemBoxSizer85->Add(itemStaticText86, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceTimelineSaveImageFormatStrings;
  choiceTimelineSaveImageFormatStrings.Add(_("Dummy Text"));
  choiceTimelineSaveImageFormat = new wxChoice( panelTimeline, ID_PREFERENCES_TIMELINE_SAVE_AS_IMAGE, wxDefaultPosition, wxDefaultSize, choiceTimelineSaveImageFormatStrings, 0 );
  choiceTimelineSaveImageFormat->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineSaveImageFormat->SetToolTip(_("Default image format selected."));
  itemBoxSizer85->Add(choiceTimelineSaveImageFormat, 3, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer88 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer84->Add(itemBoxSizer88, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxStaticText* itemStaticText89 = new wxStaticText( panelTimeline, wxID_STATIC, _("Text as"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText89->SetToolTip(_("Default text format selected."));
  itemBoxSizer88->Add(itemStaticText89, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceTimelineSaveTextFormatStrings;
  choiceTimelineSaveTextFormatStrings.Add(_("Dummy Text"));
  choiceTimelineSaveTextFormat = new wxChoice( panelTimeline, ID_PREFERENCES_TIMELINE_SAVE_AS_TEXT, wxDefaultPosition, wxDefaultSize, choiceTimelineSaveTextFormatStrings, 0 );
  choiceTimelineSaveTextFormat->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineSaveTextFormat->SetToolTip(_("Default text format selected."));
  itemBoxSizer88->Add(choiceTimelineSaveTextFormat, 3, wxALIGN_CENTER_VERTICAL, 5);

  GetBookCtrl()->AddPage(panelTimeline, _("Timeline"));

  panelHistogram = new wxPanel( GetBookCtrl(), ID_PREFERENCES_HISTOGRAM, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer92 = new wxBoxSizer(wxHORIZONTAL);
  panelHistogram->SetSizer(itemBoxSizer92);

  wxBoxSizer* itemBoxSizer93 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer92->Add(itemBoxSizer93, 3, wxGROW|wxTOP, 5);
  wxStaticBox* itemStaticBoxSizer94Static = new wxStaticBox(panelHistogram, wxID_ANY, _(" Create Histogram Dialog"));
  wxStaticBoxSizer* itemStaticBoxSizer94 = new wxStaticBoxSizer(itemStaticBoxSizer94Static, wxVERTICAL);
  itemBoxSizer93->Add(itemStaticBoxSizer94, 0, wxGROW|wxALL, 5);
  checkHistogramSkipCreateDialog = new wxCheckBox( panelHistogram, ID_PREFERENCES_HISTOGRAM_SKIP_CREATE_DIALOG, _("Skip dialog"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramSkipCreateDialog->SetValue(false);
  checkHistogramSkipCreateDialog->SetHelpText(_("Skips \"Create Histogram\" dialog after clicking the \"New Histogram\" button in the main window"));
  if (PreferencesDialog::ShowToolTips())
    checkHistogramSkipCreateDialog->SetToolTip(_("Skips \"Create Histogram\" dialog after clicking the \"New Histogram\" button in the main window"));
  itemStaticBoxSizer94->Add(checkHistogramSkipCreateDialog, 0, wxGROW|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer96Static = new wxStaticBox(panelHistogram, wxID_STATIC, _("  Name Format  "));
  wxStaticBoxSizer* itemStaticBoxSizer96 = new wxStaticBoxSizer(itemStaticBoxSizer96Static, wxVERTICAL);
  itemStaticBoxSizer96Static->Enable(false);
  itemBoxSizer93->Add(itemStaticBoxSizer96, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer97 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer96->Add(itemBoxSizer97, 0, wxGROW|wxTOP|wxBOTTOM, 5);
  wxStaticText* itemStaticText98 = new wxStaticText( panelHistogram, wxID_STATIC, _("Prefix"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText98->SetToolTip(_("Default name for new histograms, applied in window browser."));
  itemStaticText98->Enable(false);
  itemBoxSizer97->Add(itemStaticText98, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  txtHistogramNameFormatPrefix = new wxTextCtrl( panelHistogram, ID_PREFERENCES_HISTOGRAM_NAME_PREFIX, _("New Histogram # %N"), wxDefaultPosition, wxDefaultSize, 0 );
  txtHistogramNameFormatPrefix->SetMaxLength(256);
  if (PreferencesDialog::ShowToolTips())
    txtHistogramNameFormatPrefix->SetToolTip(_("Default name for new histograms, applied in window browser."));
  txtHistogramNameFormatPrefix->Enable(false);
  itemBoxSizer97->Add(txtHistogramNameFormatPrefix, 2, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer100 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer96->Add(itemBoxSizer100, 0, wxGROW|wxBOTTOM, 5);
  wxStaticText* itemStaticText101 = new wxStaticText( panelHistogram, wxID_STATIC, _("Full"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText101->SetToolTip(_("Default full name for new histograms, applied in window title."));
  itemStaticText101->Enable(false);
  itemBoxSizer100->Add(itemStaticText101, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  txtHistogramNameFormatFull = new wxTextCtrl( panelHistogram, ID_PREFERENCES_HISTOGRAM_NAME_FULL, _("%P @ %T"), wxDefaultPosition, wxDefaultSize, 0 );
  txtHistogramNameFormatFull->SetMaxLength(256);
  if (PreferencesDialog::ShowToolTips())
    txtHistogramNameFormatFull->SetToolTip(_("Default full name for new histograms, applied in window title."));
  txtHistogramNameFormatFull->Enable(false);
  itemBoxSizer100->Add(txtHistogramNameFormatFull, 2, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxStaticBox* itemStaticBoxSizer103Static = new wxStaticBox(panelHistogram, wxID_ANY, _("  Matrix Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer103 = new wxStaticBoxSizer(itemStaticBoxSizer103Static, wxVERTICAL);
  itemBoxSizer93->Add(itemStaticBoxSizer103, 1, wxGROW|wxALL, 5);
  checkHistogramZoom = new wxCheckBox( panelHistogram, ID_PREFERENCES_HISTOGRAM_MATRIX_ZOOM, _("Zoom View as default"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramZoom->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramZoom->SetToolTip(_("Select between numeric and zoom view for the histogram matrix."));
  itemStaticBoxSizer103->Add(checkHistogramZoom, 0, wxGROW|wxTOP, 5);

  checkHistogramHorizontal = new wxCheckBox( panelHistogram, ID_PREFERENCES_HISTOGRAM_MATRIX_HORIZONTAL, _("Horizontal disposition"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramHorizontal->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramHorizontal->SetToolTip(_("Select between horizontal / vertical disposition for the histogram matrix."));
  itemStaticBoxSizer103->Add(checkHistogramHorizontal, 0, wxGROW, 5);

  checkHistogramHideEmpty = new wxCheckBox( panelHistogram, ID_PREFERENCES_HISTOGRAM_MATRIX_HIDE_EMPTY, _("Hide empty columns"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramHideEmpty->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramHideEmpty->SetToolTip(_("Hide columns when all its cells are empty."));
  itemStaticBoxSizer103->Add(checkHistogramHideEmpty, 0, wxGROW, 5);

  checkHistogramShowGradient = new wxCheckBox( panelHistogram, ID_PREFERENCES_HISTOGRAM_MATRIX_GRADIENT, _("Show Gradient Colors"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramShowGradient->SetValue(true);
  itemStaticBoxSizer103->Add(checkHistogramShowGradient, 0, wxGROW, 5);

  checkHistogramLabelsColor = new wxCheckBox( panelHistogram, ID_PREFERENCES_HISTOGRAM_MATRIX_LABELS_COLOR, _("Labels color"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramLabelsColor->SetValue(false);
  itemStaticBoxSizer103->Add(checkHistogramLabelsColor, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer109 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer103->Add(itemBoxSizer109, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText110 = new wxStaticText( panelHistogram, wxID_STATIC, _("Gradient Function"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText110->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer109->Add(itemStaticText110, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceHistogramGradientFunctionStrings;
  choiceHistogramGradientFunctionStrings.Add(_("Dummy Text"));
  choiceHistogramGradientFunction = new wxChoice( panelHistogram, ID_PREFERENCES_HISTOGRAM_MATRIX_GRADIENT_FUNCTION, wxDefaultPosition, wxDefaultSize, choiceHistogramGradientFunctionStrings, 0 );
  choiceHistogramGradientFunction->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceHistogramGradientFunction->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer109->Add(choiceHistogramGradientFunction, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer112 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer103->Add(itemBoxSizer112, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText113 = new wxStaticText( panelHistogram, wxID_STATIC, _("Drawmode Semantic"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText113->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer112->Add(itemStaticText113, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceHistogramDrawmodeSemanticStrings;
  choiceHistogramDrawmodeSemanticStrings.Add(_("Dummy Text"));
  choiceHistogramDrawmodeSemantic = new wxChoice( panelHistogram, ID_PREFERENCES_HISTOGRAM_MATRIX_DRAWMODE_SEMANTIC, wxDefaultPosition, wxDefaultSize, choiceHistogramDrawmodeSemanticStrings, 0 );
  choiceHistogramDrawmodeSemantic->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceHistogramDrawmodeSemantic->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer112->Add(choiceHistogramDrawmodeSemantic, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer115 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer103->Add(itemBoxSizer115, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText116 = new wxStaticText( panelHistogram, wxID_STATIC, _("Drawmode Objects"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText116->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer115->Add(itemStaticText116, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceHistogramDrawmodeObjectsStrings;
  choiceHistogramDrawmodeObjectsStrings.Add(_("Dummy Text"));
  choiceHistogramDrawmodeObjects = new wxChoice( panelHistogram, ID_PREFERENCES_HISTOGRAM_MATRIX_DRAWMODE_OBJECTS, wxDefaultPosition, wxDefaultSize, choiceHistogramDrawmodeObjectsStrings, 0 );
  choiceHistogramDrawmodeObjects->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceHistogramDrawmodeObjects->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer115->Add(choiceHistogramDrawmodeObjects, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer118 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer92->Add(itemBoxSizer118, 2, wxGROW|wxTOP, 5);
  wxStaticBox* itemStaticBoxSizer119Static = new wxStaticBox(panelHistogram, wxID_ANY, _("  Cell Format  "));
  wxStaticBoxSizer* itemStaticBoxSizer119 = new wxStaticBoxSizer(itemStaticBoxSizer119Static, wxVERTICAL);
  itemBoxSizer118->Add(itemStaticBoxSizer119, 0, wxGROW|wxALL, 5);
  checkHistogramScientificNotation = new wxCheckBox( panelHistogram, ID_PREFERENCES_HISTOGRAM_SCIENTIFIC_NOTATION, _("Scientific Notation"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramScientificNotation->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramScientificNotation->SetToolTip(_("Write cell content using scientific notation."));
  itemStaticBoxSizer119->Add(checkHistogramScientificNotation, 0, wxGROW|wxTOP, 5);

  checkHistogramThousandsSeparator = new wxCheckBox( panelHistogram, ID_PREFERENCES_HISTOGRAM_THOUSANDS_SEPARATOR, _("Thousands Separator"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramThousandsSeparator->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramThousandsSeparator->SetToolTip(_("Write cell content using thousands separator."));
  itemStaticBoxSizer119->Add(checkHistogramThousandsSeparator, 0, wxGROW, 5);

  checkHistogramShowUnits = new wxCheckBox( panelHistogram, ID_PREFERENCES_HISTOGRAM_SHOW_UNITS, _("Show Units"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramShowUnits->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramShowUnits->SetToolTip(_("End numeric information with its units."));
  itemStaticBoxSizer119->Add(checkHistogramShowUnits, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer123 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer119->Add(itemBoxSizer123, 0, wxGROW|wxBOTTOM, 5);
  wxStaticText* itemStaticText124 = new wxStaticText( panelHistogram, wxID_STATIC, _("Precision"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText124->SetToolTip(_("Decimal precision applied to the presentation of histogram metrics."));
  itemBoxSizer123->Add(itemStaticText124, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txtHistogramPrecision = new wxSpinCtrl( panelHistogram, ID_PREFERENCES_HISTOGRAM_PRECISION, wxT("2"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 2 );
  if (PreferencesDialog::ShowToolTips())
    txtHistogramPrecision->SetToolTip(_("Decimal precision applied to the presentation of histogram metrics."));
  itemBoxSizer123->Add(txtHistogramPrecision, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer126Static = new wxStaticBox(panelHistogram, wxID_ANY, _("  Compute Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer126 = new wxStaticBoxSizer(itemStaticBoxSizer126Static, wxVERTICAL);
  itemBoxSizer118->Add(itemStaticBoxSizer126, 0, wxGROW|wxALL, 5);
  checkHistogramAutofitControlScale = new wxCheckBox( panelHistogram, ID_PREFERENCES_HISTOGRAM_AUTOFIT_CONTROL, _("Autofit Control Scale"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramAutofitControlScale->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramAutofitControlScale->SetToolTip(_("Fit control scale when histogram is created."));
  itemStaticBoxSizer126->Add(checkHistogramAutofitControlScale, 0, wxGROW|wxTOP, 5);

  checkHistogramAutofitControlScaleZero = new wxCheckBox( panelHistogram, ID_PREFERENCES_HISTOGRAM_AUTOFIT_CONTROL_ZERO, _("Autofit Control Scale Zeros"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramAutofitControlScaleZero->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramAutofitControlScaleZero->SetToolTip(_("Fit control scale zeros when histogram is created."));
  itemStaticBoxSizer126->Add(checkHistogramAutofitControlScaleZero, 0, wxGROW, 5);

  checkHistogramAutofit3DScale = new wxCheckBox( panelHistogram, ID_PREFERENCES_HISTOGRAM_AUTOFIT_3D, _("Autofit 3D Scale"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramAutofit3DScale->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramAutofit3DScale->SetToolTip(_("Fit 3D scale when histogram is created."));
  itemStaticBoxSizer126->Add(checkHistogramAutofit3DScale, 0, wxGROW, 5);

  checkHistogramAutofitDataGradient = new wxCheckBox( panelHistogram, ID_PREFERENCES_HISTOGRAM_AUTOFIT_DATA_GRADIENT, _("Autofit Data Gradient"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramAutofitDataGradient->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramAutofitDataGradient->SetToolTip(_("Fit data gradient when histogram is created."));
  itemStaticBoxSizer126->Add(checkHistogramAutofitDataGradient, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer130 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer126->Add(itemBoxSizer130, 0, wxGROW|wxBOTTOM, 5);
  wxStaticText* itemStaticText131 = new wxStaticText( panelHistogram, wxID_STATIC, _("Columns"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText131->SetToolTip(_("Initial number of columns for new histograms."));
  itemBoxSizer130->Add(itemStaticText131, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txtHistogramNumColumns = new wxSpinCtrl( panelHistogram, ID_PREFERENCES_HISTOGRAM_NUMCOLUMNS, wxT("20"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 256, 20 );
  if (PreferencesDialog::ShowToolTips())
    txtHistogramNumColumns->SetToolTip(_("Initial number of columns for new histograms."));
  itemBoxSizer130->Add(txtHistogramNumColumns, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer133Static = new wxStaticBox(panelHistogram, wxID_ANY, _("  Default Save Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer133 = new wxStaticBoxSizer(itemStaticBoxSizer133Static, wxVERTICAL);
  itemBoxSizer118->Add(itemStaticBoxSizer133, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer134 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer133->Add(itemBoxSizer134, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxStaticText* itemStaticText135 = new wxStaticText( panelHistogram, wxID_STATIC, _("Image as"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText135->SetToolTip(_("Default image format selected."));
  itemBoxSizer134->Add(itemStaticText135, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceHistogramSaveImageFormatStrings;
  choiceHistogramSaveImageFormatStrings.Add(_("Dummy Text"));
  choiceHistogramSaveImageFormat = new wxChoice( panelHistogram, ID_PREFERENCES_HISTOGRAM_SAVE_IMAGE_FORMAT, wxDefaultPosition, wxDefaultSize, choiceHistogramSaveImageFormatStrings, 0 );
  choiceHistogramSaveImageFormat->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceHistogramSaveImageFormat->SetToolTip(_("Default image format selected."));
  itemBoxSizer134->Add(choiceHistogramSaveImageFormat, 3, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer137 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer133->Add(itemBoxSizer137, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxStaticText* itemStaticText138 = new wxStaticText( panelHistogram, wxID_STATIC, _("Text as"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText138->SetToolTip(_("Default text format selected."));
  itemBoxSizer137->Add(itemStaticText138, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceHistogramSaveTextFormatStrings;
  choiceHistogramSaveTextFormatStrings.Add(_("Dummy Text"));
  choiceHistogramSaveTextFormat = new wxChoice( panelHistogram, ID_PREFERENCES_HISTOGRAM_SAVE_TXT_FORMAT, wxDefaultPosition, wxDefaultSize, choiceHistogramSaveTextFormatStrings, 0 );
  choiceHistogramSaveTextFormat->SetStringSelection(_("Dummy Text"));
  if (PreferencesDialog::ShowToolTips())
    choiceHistogramSaveTextFormat->SetToolTip(_("Default text format selected."));
  itemBoxSizer137->Add(choiceHistogramSaveTextFormat, 3, wxALIGN_CENTER_VERTICAL, 5);

  GetBookCtrl()->AddPage(panelHistogram, _("Histogram"));

  panelColor = new wxPanel( GetBookCtrl(), ID_PREFERENCES_COLOR, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer141 = new wxBoxSizer(wxVERTICAL);
  panelColor->SetSizer(itemBoxSizer141);

  wxStaticBox* itemStaticBoxSizer142Static = new wxStaticBox(panelColor, wxID_ANY, _("  Timeline  "));
  wxStaticBoxSizer* itemStaticBoxSizer142 = new wxStaticBoxSizer(itemStaticBoxSizer142Static, wxVERTICAL);
  itemBoxSizer141->Add(itemStaticBoxSizer142, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer143 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer142->Add(itemBoxSizer143, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText144 = new wxStaticText( panelColor, wxID_STATIC, _("Background"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText144->SetToolTip(_("When background timeline color is changed, new foreground complementary color is suggested."));
  itemBoxSizer143->Add(itemStaticText144, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerBackground = new wxColourPickerCtrl( panelColor, ID_COLOURPICKER_BACKGROUND, wxColour(0, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerBackground->SetToolTip(_("When background timeline color is changed, new foreground complementary color is suggested."));
  itemBoxSizer143->Add(colourPickerBackground, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer146 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer142->Add(itemBoxSizer146, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText147 = new wxStaticText( panelColor, wxID_STATIC, _("Foreground"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText147->SetToolTip(_("Color used to draw scale axis and object names in timelines."));
  itemBoxSizer146->Add(itemStaticText147, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerAxis = new wxColourPickerCtrl( panelColor, ID_COLOURPICKER_AXIS, wxColour(255, 255, 255), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerAxis->SetToolTip(_("Color used to draw scale axis and object names in timelines."));
  itemBoxSizer146->Add(colourPickerAxis, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer149 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer142->Add(itemBoxSizer149, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  checkZero = new wxCheckBox( panelColor, wxID_STATIC, _("Semantic zero"), wxDefaultPosition, wxDefaultSize, 0 );
  checkZero->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkZero->SetToolTip(_("Color used for semantic zero values"));
  itemBoxSizer149->Add(checkZero, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerZero = new wxColourPickerCtrl( panelColor, ID_COLOURPICKER_ZERO, wxColour(0, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerZero->SetToolTip(_("Color used for semantic zero values"));
  itemBoxSizer149->Add(colourPickerZero, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer152 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer142->Add(itemBoxSizer152, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText153 = new wxStaticText( panelColor, wxID_STATIC, _("Punctual "), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText153->SetToolTip(_("Color used to draw punctual information on timelines."));
  itemBoxSizer152->Add(itemStaticText153, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerPunctual = new wxColourPickerCtrl( panelColor, ID_COLOURPICKER_PUNCTUAL, wxColour(0, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerPunctual->SetToolTip(_("Color used to draw punctual information on timelines."));
  itemBoxSizer152->Add(colourPickerPunctual, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer155 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer142->Add(itemBoxSizer155, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText156 = new wxStaticText( panelColor, wxID_STATIC, _("Logical comms."), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText156->SetToolTip(_("Color used to draw logical communications in timelines."));
  itemBoxSizer155->Add(itemStaticText156, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerLogical = new wxColourPickerCtrl( panelColor, ID_COLOURPICKER_LOGICAL, wxColour(255, 255, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerLogical->SetToolTip(_("Color used to draw logical communications in timelines."));
  itemBoxSizer155->Add(colourPickerLogical, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer158 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer142->Add(itemBoxSizer158, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText159 = new wxStaticText( panelColor, wxID_STATIC, _("Physical comms."), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText159->SetToolTip(_("Color used to draw physical communications in timelines."));
  itemBoxSizer158->Add(itemStaticText159, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerPhysical = new wxColourPickerCtrl( panelColor, ID_COLOURPICKER_PHYSICAL, wxColour(255, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerPhysical->SetToolTip(_("Color used to draw physical communications in timelines."));
  itemBoxSizer158->Add(colourPickerPhysical, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxButton* itemButton161 = new wxButton( panelColor, ID_BUTTON_DEFAULT_TIMELINE, _("Default"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemButton161->SetToolTip(_("Restores predefined colors."));
  itemStaticBoxSizer142->Add(itemButton161, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT, 5);

  wxStaticBox* itemStaticBoxSizer162Static = new wxStaticBox(panelColor, wxID_ANY, _("  Gradient  "));
  wxStaticBoxSizer* itemStaticBoxSizer162 = new wxStaticBoxSizer(itemStaticBoxSizer162Static, wxVERTICAL);
  itemBoxSizer141->Add(itemStaticBoxSizer162, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer163 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer162->Add(itemBoxSizer163, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText164 = new wxStaticText( panelColor, wxID_STATIC, _("Positive Begin"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText164->SetToolTip(_("Base color used to represent positive semantic values when gradient is selected."));
  itemBoxSizer163->Add(itemStaticText164, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientBegin = new wxColourPickerCtrl( panelColor, ID_COLOURPICKER_GRADBEGIN, wxColour(0, 255, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientBegin->SetToolTip(_("Base color used to represent positive semantic values when gradient is selected."));
  itemBoxSizer163->Add(colourPickerGradientBegin, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer166 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer162->Add(itemBoxSizer166, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText167 = new wxStaticText( panelColor, wxID_STATIC, _("Positive End"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText167->SetToolTip(_("Top color used to represent positive semantic values when gradient is selected."));
  itemBoxSizer166->Add(itemStaticText167, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientEnd = new wxColourPickerCtrl( panelColor, ID_COLOURPICKER_GRADEND, wxColour(0, 0, 255), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientEnd->SetToolTip(_("Top color used to represent positive semantic values when gradient is selected."));
  itemBoxSizer166->Add(colourPickerGradientEnd, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer162->Add(itemBoxSizer1, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText2 = new wxStaticText( panelColor, wxID_STATIC, _("Negative Begin"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText2->SetToolTip(_("Base color used to represent negative semantic values when gradient is selected."));
  itemBoxSizer1->Add(itemStaticText2, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerNegativeGradientBegin = new wxColourPickerCtrl( panelColor, ID_COLOURPICKER_NEGATIVE_GRADBEGIN, wxColour(255, 255, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerNegativeGradientBegin->SetToolTip(_("Base color used to represent negative semantic values when gradient is selected."));
  itemBoxSizer1->Add(colourPickerNegativeGradientBegin, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer162->Add(itemBoxSizer4, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText5 = new wxStaticText( panelColor, wxID_STATIC, _("Negative End"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText5->SetToolTip(_("Top color used to represent negative semantic values when gradient is selected."));
  itemBoxSizer4->Add(itemStaticText5, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerNegativeGradientEnd = new wxColourPickerCtrl( panelColor, ID_COLOURPICKER_NEGATIVE_GRADEND, wxColour(255, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerNegativeGradientEnd->SetToolTip(_("Top color used to represent negative semantic values when gradient is selected."));
  itemBoxSizer4->Add(colourPickerNegativeGradientEnd, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer169 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer162->Add(itemBoxSizer169, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText170 = new wxStaticText( panelColor, wxID_STATIC, _("Low outliers"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText170->SetToolTip(_("Any semantic outlier smaller than <Begin> will use this color."));
  itemBoxSizer169->Add(itemStaticText170, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientLow = new wxColourPickerCtrl( panelColor, ID_COLOURPICKER_GRADLOW, wxColour(207, 207, 68), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientLow->SetToolTip(_("Any semantic outlier smaller than <Begin> will use this color."));
  itemBoxSizer169->Add(colourPickerGradientLow, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer172 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer162->Add(itemBoxSizer172, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText173 = new wxStaticText( panelColor, wxID_STATIC, _("High outliers"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText173->SetToolTip(_("Any semantic outlier greater than <End> will use this color."));
  itemBoxSizer172->Add(itemStaticText173, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientTop = new wxColourPickerCtrl( panelColor, ID_COLOURPICKER_GRADTOP, wxColour(255, 146, 24), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientTop->SetToolTip(_("Any semantic outlier greater than <End> will use this color."));
  itemBoxSizer172->Add(colourPickerGradientTop, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxButton* itemButton175 = new wxButton( panelColor, ID_BUTTON_DEFAULT_GRADIENT, _("Default"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemButton175->SetToolTip(_("Restores predefined colors."));
  itemStaticBoxSizer162->Add(itemButton175, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT, 5);

  GetBookCtrl()->AddPage(panelColor, _("Color"));

  panelWorkspaces = new wxPanel( GetBookCtrl(), ID_PREFERENCES_WORKSPACES, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer177 = new wxBoxSizer(wxHORIZONTAL);
  panelWorkspaces->SetSizer(itemBoxSizer177);

  wxBoxSizer* itemBoxSizer178 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer177->Add(itemBoxSizer178, 2, wxGROW|wxALL, 5);
  wxArrayString listWorkspacesStrings;
  listWorkspaces = new wxListBox( panelWorkspaces, ID_LISTBOX_WORKSPACES, wxDefaultPosition, wxDefaultSize, listWorkspacesStrings, wxLB_SINGLE|wxLB_HSCROLL );
  itemBoxSizer178->Add(listWorkspaces, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer180 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer178->Add(itemBoxSizer180, 0, wxALIGN_TOP|wxALL, 0);
  buttonAddWorkspace = new wxBitmapButton( panelWorkspaces, ID_BUTTON_WORKSPACES_ADD, itemPropertySheetDialog1->GetBitmapResource(wxT("icons/derived_add.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (PreferencesDialog::ShowToolTips())
    buttonAddWorkspace->SetToolTip(_("Add workspaces..."));
  itemBoxSizer180->Add(buttonAddWorkspace, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonDeleteWorkspace = new wxBitmapButton( panelWorkspaces, ID_BUTTON_WORKSPACES_DELETE, itemPropertySheetDialog1->GetBitmapResource(wxT("icons/delete.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (PreferencesDialog::ShowToolTips())
    buttonDeleteWorkspace->SetToolTip(_("Delete selected workspace"));
  itemBoxSizer180->Add(buttonDeleteWorkspace, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonUpWorkspace = new wxBitmapButton( panelWorkspaces, ID_BUTTON_WORKSPACES_UP, itemPropertySheetDialog1->GetBitmapResource(wxT("icons/arrow_up.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (PreferencesDialog::ShowToolTips())
    buttonUpWorkspace->SetToolTip(_("Move workspace up"));
  itemBoxSizer180->Add(buttonUpWorkspace, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonDownWorkspace = new wxBitmapButton( panelWorkspaces, ID_BUTTON_WORKSPACES_DOWN, itemPropertySheetDialog1->GetBitmapResource(wxT("icons/arrow_down.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (PreferencesDialog::ShowToolTips())
    buttonDownWorkspace->SetToolTip(_("Move workspace down"));
  itemBoxSizer180->Add(buttonDownWorkspace, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonImportWorkspace = new wxBitmapButton( panelWorkspaces, ID_BUTTON_WORKSPACES_IMPORT, itemPropertySheetDialog1->GetBitmapResource(wxT("icons/report_add.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (PreferencesDialog::ShowToolTips())
    buttonImportWorkspace->SetToolTip(_("Import workspaces"));
  itemBoxSizer180->Add(buttonImportWorkspace, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonExportWorkspace = new wxBitmapButton( panelWorkspaces, ID_BUTTON_WORKSPACES_EXPORT, itemPropertySheetDialog1->GetBitmapResource(wxT("icons/report_disk.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (PreferencesDialog::ShowToolTips())
    buttonExportWorkspace->SetToolTip(_("Export workspaces"));
  itemBoxSizer180->Add(buttonExportWorkspace, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxStaticLine* itemStaticLine185 = new wxStaticLine( panelWorkspaces, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
  itemBoxSizer177->Add(itemStaticLine185, 0, wxGROW|wxTOP|wxBOTTOM, 10);

  wxBoxSizer* itemBoxSizer186 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer177->Add(itemBoxSizer186, 4, wxGROW|wxALL, 0);
  wxBoxSizer* itemBoxSizer187 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer186->Add(itemBoxSizer187, 0, wxGROW|wxLEFT|wxTOP, 5);
  txtWorkspaceName = new wxTextCtrl( panelWorkspaces, ID_TEXT_WORKSPACE_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    txtWorkspaceName->SetToolTip(_("Workspace name"));
  itemBoxSizer187->Add(txtWorkspaceName, 1, wxGROW|wxRIGHT|wxTOP|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer186->Add(itemBoxSizer2, 0, wxGROW|wxLEFT, 5);
  radioStates = new wxRadioButton( panelWorkspaces, ID_RADIOSTATES, _("States"), wxDefaultPosition, wxDefaultSize, 0 );
  radioStates->SetValue(false);
  itemBoxSizer2->Add(radioStates, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  radioEventTypes = new wxRadioButton( panelWorkspaces, ID_RADIOEVENTYPES, _("Event Types"), wxDefaultPosition, wxDefaultSize, 0 );
  radioEventTypes->SetValue(true);
  itemBoxSizer2->Add(radioEventTypes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer189 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer186->Add(itemBoxSizer189, 0, wxGROW|wxLEFT, 5);
  txtAutoTypes = new wxTextCtrl( panelWorkspaces, ID_TEXT_WORKSPACE_AUTOTYPES, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    txtAutoTypes->SetToolTip(_("States or event types list for automatic workspace selection\n\nExample: 50000001;50000002[;type] "));
  itemBoxSizer189->Add(txtAutoTypes, 1, wxGROW|wxRIGHT|wxTOP|wxBOTTOM, 5);

  wxStaticBox* itemStaticBoxSizer192Static = new wxStaticBox(panelWorkspaces, wxID_ANY, _(" Hints "));
  wxStaticBoxSizer* itemStaticBoxSizer192 = new wxStaticBoxSizer(itemStaticBoxSizer192Static, wxVERTICAL);
  itemBoxSizer186->Add(itemStaticBoxSizer192, 1, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
  wxBoxSizer* itemBoxSizer193 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer192->Add(itemBoxSizer193, 4, wxGROW, 0);
  wxArrayString listHintsWorkspaceStrings;
  listHintsWorkspace = new wxListBox( panelWorkspaces, ID_LISTBOX_HINTS_WORKSPACE, wxDefaultPosition, wxDefaultSize, listHintsWorkspaceStrings, wxLB_SINGLE );
  itemBoxSizer193->Add(listHintsWorkspace, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer195 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer193->Add(itemBoxSizer195, 0, wxALIGN_TOP|wxALL, 0);
  buttonAddHint = new wxBitmapButton( panelWorkspaces, ID_BUTTON_HINT_ADD, itemPropertySheetDialog1->GetBitmapResource(wxT("icons/derived_add.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (PreferencesDialog::ShowToolTips())
    buttonAddHint->SetToolTip(_("Add hints..."));
  itemBoxSizer195->Add(buttonAddHint, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonDeleteHint = new wxBitmapButton( panelWorkspaces, ID_BUTTON_HINT_DELETE, itemPropertySheetDialog1->GetBitmapResource(wxT("icons/delete.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (PreferencesDialog::ShowToolTips())
    buttonDeleteHint->SetToolTip(_("Delete selected hints"));
  itemBoxSizer195->Add(buttonDeleteHint, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonUpHint = new wxBitmapButton( panelWorkspaces, ID_BITMAP_HINT_UP, itemPropertySheetDialog1->GetBitmapResource(wxT("icons/arrow_up.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (PreferencesDialog::ShowToolTips())
    buttonUpHint->SetToolTip(_("Move hint up"));
  itemBoxSizer195->Add(buttonUpHint, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonDownHint = new wxBitmapButton( panelWorkspaces, ID_BUTTON_HINT_DOWN, itemPropertySheetDialog1->GetBitmapResource(wxT("icons/arrow_down.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (PreferencesDialog::ShowToolTips())
    buttonDownHint->SetToolTip(_("Move hint down"));
  itemBoxSizer195->Add(buttonDownHint, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer200 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer192->Add(itemBoxSizer200, 0, wxGROW, 5);
  wxBoxSizer* itemBoxSizer201 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer200->Add(itemBoxSizer201, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);
  wxStaticText* itemStaticText202 = new wxStaticText( panelWorkspaces, wxID_STATIC, _("Path"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer201->Add(itemStaticText202, 0, wxALIGN_LEFT|wxALL, 5);

  itemBoxSizer201->Add(5, 5, 0, wxGROW|wxTOP|wxBOTTOM, 2);

  wxStaticText* itemStaticText204 = new wxStaticText( panelWorkspaces, wxID_STATIC, _("Description"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer201->Add(itemStaticText204, 0, wxALIGN_LEFT|wxALL, 5);

  wxBoxSizer* itemBoxSizer205 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer200->Add(itemBoxSizer205, 4, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);
  wxBoxSizer* itemBoxSizer206 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer205->Add(itemBoxSizer206, 0, wxGROW|wxALL, 0);
  txtHintPath = new wxTextCtrl( panelWorkspaces, ID_TEXTCTRL_WORKSPACE_HINT_PATH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
  itemBoxSizer206->Add(txtHintPath, 1, wxGROW|wxTOP|wxBOTTOM, 5);

  fileBrowserHintPath = new FileBrowserButton( panelWorkspaces, ID_FILE_BUTTON_WORKSPACE_HINT_PATH, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer206->Add(fileBrowserHintPath, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer209 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer205->Add(itemBoxSizer209, 0, wxGROW|wxALL, 0);
  txtHintDescription = new wxTextCtrl( panelWorkspaces, ID_TEXTCTRL_WORKSPACE_HINT_DESCRIPTION, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer209->Add(txtHintDescription, 1, wxGROW|wxRIGHT|wxTOP|wxBOTTOM, 5);

  GetBookCtrl()->AddPage(panelWorkspaces, _("Workspaces"));

  panelExternal = new wxPanel( GetBookCtrl(), ID_PREFERENCES_EXTERNAL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
  panelExternal->SetSizer(itemBoxSizer5);

  wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer5->Add(itemBoxSizer9, 1, wxGROW|wxALL, 5);
  wxStaticBox* itemStaticBoxSizer2Static = new wxStaticBox(panelExternal, wxID_ANY, _("Text Editors"));
  wxStaticBoxSizer* itemStaticBoxSizer2 = new wxStaticBoxSizer(itemStaticBoxSizer2Static, wxHORIZONTAL);
  itemBoxSizer9->Add(itemStaticBoxSizer2, 2, wxGROW|wxALL, 5);
  wxArrayString listTextEditorsStrings;
  listTextEditors = new wxListBox( panelExternal, ID_LISTBOX_TEXT_EDITORS, wxDefaultPosition, wxDefaultSize, listTextEditorsStrings, wxLB_HSCROLL );
  itemStaticBoxSizer2->Add(listTextEditors, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer2->Add(itemBoxSizer13, 0, wxALIGN_TOP|wxALL, 0);
  buttonAddTextEditor = new wxBitmapButton( panelExternal, ID_BUTTON_TXT_ADD, itemPropertySheetDialog1->GetBitmapResource(wxT("icons/derived_add.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (PreferencesDialog::ShowToolTips())
    buttonAddTextEditor->SetToolTip(_("Add text editors..."));
  itemBoxSizer13->Add(buttonAddTextEditor, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonDeleteTextEditor = new wxBitmapButton( panelExternal, ID_BUTTON_TXT_DEL, itemPropertySheetDialog1->GetBitmapResource(wxT("icons/delete.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (PreferencesDialog::ShowToolTips())
    buttonDeleteTextEditor->SetToolTip(_("Delete selected text editor"));
  itemBoxSizer13->Add(buttonDeleteTextEditor, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonUpTextEditor = new wxBitmapButton( panelExternal, ID_BUTTON_TXT_UP, itemPropertySheetDialog1->GetBitmapResource(wxT("icons/arrow_up.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (PreferencesDialog::ShowToolTips())
    buttonUpTextEditor->SetToolTip(_("Move text editor up"));
  itemBoxSizer13->Add(buttonUpTextEditor, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonDownTextEditor = new wxBitmapButton( panelExternal, ID_BUTTON_TXT_DOWN, itemPropertySheetDialog1->GetBitmapResource(wxT("icons/arrow_down.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (PreferencesDialog::ShowToolTips())
    buttonDownTextEditor->SetToolTip(_("Move text editor down"));
  itemBoxSizer13->Add(buttonDownTextEditor, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxStaticLine* itemStaticLine18 = new wxStaticLine( panelExternal, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
  itemBoxSizer9->Add(itemStaticLine18, 0, wxGROW|wxTOP|wxBOTTOM, 10);

  wxStaticLine* itemStaticLine19 = new wxStaticLine( panelExternal, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
  itemBoxSizer5->Add(itemStaticLine19, 0, wxGROW|wxTOP|wxBOTTOM, 10);

  wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer5->Add(itemBoxSizer10, 1, wxGROW|wxALL, 5);
  wxStaticBox* itemStaticBoxSizer11Static = new wxStaticBox(panelExternal, wxID_ANY, _("PDF Readers"));
  wxStaticBoxSizer* itemStaticBoxSizer11 = new wxStaticBoxSizer(itemStaticBoxSizer11Static, wxHORIZONTAL);
  itemBoxSizer10->Add(itemStaticBoxSizer11, 2, wxGROW|wxALL, 5);
  wxArrayString listPDFReadersStrings;
  listPDFReaders = new wxListBox( panelExternal, ID_LISTBOX_PDF_READERS, wxDefaultPosition, wxDefaultSize, listPDFReadersStrings, wxLB_HSCROLL );
  itemStaticBoxSizer11->Add(listPDFReaders, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer11->Add(itemBoxSizer14, 0, wxALIGN_TOP|wxALL, 0);
  buttonAddPDFReader = new wxBitmapButton( panelExternal, ID_BUTTON_PDF_ADD, itemPropertySheetDialog1->GetBitmapResource(wxT("icons/derived_add.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (PreferencesDialog::ShowToolTips())
    buttonAddPDFReader->SetToolTip(_("Add PDF readers..."));
  itemBoxSizer14->Add(buttonAddPDFReader, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonDeletePDFReader = new wxBitmapButton( panelExternal, ID_BUTTON_PDF_DEL, itemPropertySheetDialog1->GetBitmapResource(wxT("icons/delete.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (PreferencesDialog::ShowToolTips())
    buttonDeletePDFReader->SetToolTip(_("Delete selected PDF reader"));
  itemBoxSizer14->Add(buttonDeletePDFReader, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonUpPDFReader = new wxBitmapButton( panelExternal, ID_BUTTON_PDF_UP, itemPropertySheetDialog1->GetBitmapResource(wxT("icons/arrow_up.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (PreferencesDialog::ShowToolTips())
    buttonUpPDFReader->SetToolTip(_("Move PDF reader up"));
  itemBoxSizer14->Add(buttonUpPDFReader, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  buttonDownPDFReader = new wxBitmapButton( panelExternal, ID_BUTTON_PDF_DOWN, itemPropertySheetDialog1->GetBitmapResource(wxT("icons/arrow_down.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (PreferencesDialog::ShowToolTips())
    buttonDownPDFReader->SetToolTip(_("Move PDF reader down"));
  itemBoxSizer14->Add(buttonDownPDFReader, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxStaticLine* itemStaticLine22 = new wxStaticLine( panelExternal, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
  itemBoxSizer10->Add(itemStaticLine22, 0, wxGROW|wxTOP|wxBOTTOM, 10);

  GetBookCtrl()->AddPage(panelExternal, _("External applications"));

  panelFilters = new wxPanel( GetBookCtrl(), ID_PREFERENCES_FILTERS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  panelFilters->Show(false);
  panelFilters->Enable(false);

  GetBookCtrl()->AddPage(panelFilters, _("Filters"));

  // Connect events and objects
  txtWorkspaceName->Connect(ID_TEXT_WORKSPACE_NAME, wxEVT_KILL_FOCUS, wxFocusEventHandler(PreferencesDialog::OnTextWorkspaceNameKillFocus), NULL, this);
////@end PreferencesDialog content construction
    
  itemPropertySheetDialog1 = NULL;
  
  dirBrowserButtonTrace->SetTextBox( textCtrlTrace );
  dirBrowserButtonTrace->SetDialogMessage( wxT( "Select Traces Default Directory" ) );
  dirBrowserButtonTrace->Enable();
  
  dirBrowserButtonCFG->SetTextBox( textCtrlCFG );
  dirBrowserButtonCFG->SetDialogMessage( wxT( "Select Paraver CFGs Default Directory" ) );
  dirBrowserButtonCFG->Enable();
  
  dirBrowserButtonXML->SetTextBox( textCtrlXML );
  dirBrowserButtonXML->SetDialogMessage( wxT( "Select Cut/Filter XMLs Default Directory" ) );
  dirBrowserButtonXML->Enable();
  
  dirBrowserButtonTutorials->SetTextBox( textCtrlTutorials );
  dirBrowserButtonTutorials->SetDialogMessage( wxT( "Select Tutorials Root Directory" ) );
  dirBrowserButtonTutorials->Enable();
  
  dirBrowserButtonTmp->SetTextBox( textCtrlTmp );
  dirBrowserButtonTmp->SetDialogMessage( wxT( "Select TMP Default Directory" ) );
  dirBrowserButtonTmp->Enable();
  
  fileBrowserHintPath->SetTextBox( txtHintPath );
  fileBrowserHintPath->SetFileDialogWildcard( _( "Paraver configuration file (*.cfg)|*.cfg|All files (*.*)|*.*" ) );
  fileBrowserHintPath->SetDialogMessage( _( "Select Configuration File" ) );
  
  // WORKSPACE PANEL
  wxString allowedChars[] = { _("0"), _("1"), _("2"), _("3"), _("4"),
                              _("5"), _("6"), _("7"), _("8"), _("9"),
                              _(";") };
  wxTextValidator myValidator( (long int)wxFILTER_INCLUDE_CHAR_LIST );
  wxArrayString charIncludes( (size_t)11, allowedChars );
  myValidator.SetIncludes( charIncludes );
  txtAutoTypes->SetValidator( myValidator );

  wxArrayString forbidden;
  forbidden.Add( _( "#" ) );
  wxTextValidator validator( wxFILTER_EXCLUDE_CHAR_LIST );
  validator.SetExcludes( forbidden );
  txtWorkspaceName->SetValidator( validator );
  
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
  if (name == wxT("icons/derived_add.xpm"))
  {
    wxBitmap bitmap(derived_add_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/delete.xpm"))
  {
    wxBitmap bitmap(delete_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/arrow_up.xpm"))
  {
    wxBitmap bitmap(arrow_up_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/arrow_down.xpm"))
  {
    wxBitmap bitmap(arrow_down_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/report_add.xpm"))
  {
    wxBitmap bitmap(report_add);
    return bitmap;
  }
  else if (name == wxT("icons/report_disk.xpm"))
  {
    wxBitmap bitmap(report_disk);
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
  choiceBox->Clear(); // enters twice

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
  checkGlobalAskForPrevSessionLoad->SetValue( askForPrevSessionLoad );
  checkGlobalAskForPrevSessionLoad->Enable( spinSessionTime->GetValue() != 0 );

  checkGlobalHelpOnBrowser->SetValue( helpContentsUsesBrowser );
  checkDisableTimelineZoomMouseWheel->SetValue( disableTimelineZoomMouseWheel );

  // TIMELINE
  txtTimelineNameFormatPrefix->SetValue( wxString::FromAscii( timelineNameFormatPrefix.c_str() ) );
  txtTimelineNameFormatFull->SetValue( wxString::FromAscii( timelineNameFormatFull.c_str() ) );

  checkTimelineEventLines->SetValue( timelineEventLines );
  checkTimelineCommunicationLines->SetValue( timelineCommunicationLines );
  checkSemanticScaleMinAtZero->SetValue( timelineSemanticScaleMinAtZero );
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
  checkHistogramAutofitControlScaleZero->SetValue( histogramAutofitControlScaleZero );
  checkHistogramAutofit3DScale->SetValue( histogramAutofit3DScale );
  checkHistogramAutofitDataGradient->SetValue( histogramAutofitDataGradient );
  txtHistogramNumColumns->SetValue( ( int )histogramNumColumns );

  options.clear();
  LabelConstructor::getGUIGroupLabels( LabelConstructor::IMAGE_FORMAT, options );
  setLabelsChoiceBox( options, histogramSaveImageFormat, choiceHistogramSaveImageFormat );

  options.clear();
  LabelConstructor::getGUIGroupLabels( LabelConstructor::TEXT_FORMAT, options );
  setLabelsChoiceBox( options, histogramSaveTextFormat, choiceHistogramSaveTextFormat );

  checkHistogramSkipCreateDialog->SetValue( histogramSkipCreateDialog );

  // COLORS
  colourPickerBackground->SetColour( RGBTowxColour( timelineColourBackground ) );
  colourPickerAxis->SetColour( RGBTowxColour( timelineColourAxis ) );
  checkZero->SetValue( colorUseZero );
  colourPickerZero->SetColour( RGBTowxColour( timelineColourZero ) );
  colourPickerPunctual->SetColour( RGBTowxColour( timelineColourPunctual ) );
  colourPickerLogical->SetColour( RGBTowxColour( timelineColourLogical ) );
  colourPickerPhysical->SetColour( RGBTowxColour( timelineColourPhysical ) );

  colourPickerGradientBegin->SetColour( RGBTowxColour( gradientColourBegin ) );
  colourPickerGradientEnd->SetColour( RGBTowxColour( gradientColourEnd ) );
  colourPickerNegativeGradientBegin->SetColour( RGBTowxColour( gradientColourNegativeBegin ) );
  colourPickerNegativeGradientEnd->SetColour( RGBTowxColour( gradientColourNegativeEnd ) );
  colourPickerGradientLow->SetColour( RGBTowxColour( gradientColourLow ) );
  colourPickerGradientTop->SetColour( RGBTowxColour( gradientColourTop ) );

  // FILTERS

  // EXTERNAL APPS
  if ( !textEditorOptions.IsEmpty() )
    listTextEditors->InsertItems( textEditorOptions, 0 );
  if ( !pdfReaderOptions.IsEmpty() )
    listPDFReaders->InsertItems( pdfReaderOptions, 0 );

  // WORKSPACES
  std::vector<std::string> tmpWorkspaceList = WorkspaceManager::getInstance()->getWorkspaces( WorkspaceManager::USER_DEFINED );
  for( std::vector<std::string>::iterator it = tmpWorkspaceList.begin(); it != tmpWorkspaceList.end(); ++it )
  {
    listWorkspaces->Append( wxString::FromAscii( it->c_str() ) );
    workspaceContainer.insert( std::pair<wxString,Workspace>( wxString::FromAscii( it->c_str() ),
                                                              WorkspaceManager::getInstance()->getWorkspace( *it, WorkspaceManager::USER_DEFINED ) ) );
  }
  
  fileBrowserHintPath->SetPath( wxString::FromAscii( cfgsPath.c_str() ) );

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
  askForPrevSessionLoad = checkGlobalAskForPrevSessionLoad->GetValue();
  helpContentsUsesBrowser = checkGlobalHelpOnBrowser->GetValue();
  disableTimelineZoomMouseWheel = checkDisableTimelineZoomMouseWheel->GetValue();

  // TIMELINE
  timelineNameFormatPrefix = std::string( txtTimelineNameFormatPrefix->GetValue().mb_str() );
  timelineNameFormatFull = std::string( txtTimelineNameFormatFull->GetValue().mb_str() );
  // Parse format looking for modifiers %N -> number, %P -> prefix, %T -> trace

  timelineEventLines = checkTimelineEventLines->IsChecked();
  timelineCommunicationLines = checkTimelineCommunicationLines->IsChecked();
  timelineSemanticScaleMinAtZero = checkSemanticScaleMinAtZero->IsChecked();

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
  histogramAutofitControlScaleZero = checkHistogramAutofitControlScaleZero->GetValue();
  histogramAutofit3DScale = checkHistogramAutofit3DScale->GetValue();
  histogramAutofitDataGradient = checkHistogramAutofitDataGradient->GetValue();
  histogramNumColumns = ( THistogramColumn )txtHistogramNumColumns->GetValue();

  histogramSaveImageFormat = ( PRV_UINT32 )choiceHistogramSaveImageFormat->GetCurrentSelection();
  histogramSaveTextFormat = ( PRV_UINT32 )choiceHistogramSaveTextFormat->GetCurrentSelection();
  histogramSkipCreateDialog = checkHistogramSkipCreateDialog->GetValue();

  // COLORS
  timelineColourBackground = wxColourToRGB( colourPickerBackground->GetColour() );
  timelineColourAxis       = wxColourToRGB( colourPickerAxis->GetColour() );
  colorUseZero             = checkZero->IsChecked();
  timelineColourZero       = wxColourToRGB( colourPickerZero->GetColour() );
  timelineColourPunctual   = wxColourToRGB( colourPickerPunctual->GetColour() );
  timelineColourLogical    = wxColourToRGB( colourPickerLogical->GetColour() );
  timelineColourPhysical   = wxColourToRGB( colourPickerPhysical->GetColour() );

  gradientColourBegin         = wxColourToRGB( colourPickerGradientBegin->GetColour() );
  gradientColourEnd           = wxColourToRGB( colourPickerGradientEnd->GetColour() );
  gradientColourNegativeBegin = wxColourToRGB( colourPickerNegativeGradientBegin->GetColour() );
  gradientColourNegativeEnd   = wxColourToRGB( colourPickerNegativeGradientEnd->GetColour() );
  gradientColourLow           = wxColourToRGB( colourPickerGradientLow->GetColour() );
  gradientColourTop           = wxColourToRGB( colourPickerGradientTop->GetColour() );


  // EXTERNAL APPS
  textEditorOptions = listTextEditors->GetStrings();
  pdfReaderOptions = listPDFReaders->GetStrings();

  // WORKSPACES
  if( wxWindow::FindFocus() == txtWorkspaceName )
  {
    Disconnect( ID_TEXT_WORKSPACE_NAME, wxEVT_KILL_FOCUS, wxFocusEventHandler(PreferencesDialog::OnTextWorkspaceNameKillFocus), NULL, this );
    wxFocusEvent dummyEvent;
    OnTextWorkspaceNameKillFocus( dummyEvent );
  }

  WorkspaceManager::getInstance()->clear();
  for( size_t i = 0; i < listWorkspaces->GetCount(); ++i )
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
  checkZero->SetValue( true );
  colourPickerPunctual->SetColour( RGBTowxColour( SemanticColor::DEFAULT_PUNCTUAL ) );
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

  colourPickerNegativeGradientBegin->SetColour( RGBTowxColour( SemanticColor::DEFAULT_NEGATIVE_BEGIN_GRADIENT_COLOR ) );
  colourPickerNegativeGradientEnd->SetColour( RGBTowxColour( SemanticColor::DEFAULT_NEGATIVE_END_GRADIENT_COLOR ) );

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
                listWorkspaces->GetSelection() < int(listWorkspaces->GetCount()) - 1 );
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
                listHintsWorkspace->GetSelection() < int( listHintsWorkspace->GetCount() ) - 1 );
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
  WorkspaceValue::WorkspaceType tmpWorkspaceType;
  if( radioStates->GetValue() )
    tmpWorkspaceType = WorkspaceValue::STATE;
  else
    tmpWorkspaceType = WorkspaceValue::EVENT;
  workspaceContainer.insert( std::pair<wxString,Workspace>( workspaceName, Workspace( std::string( workspaceName.mb_str() ), tmpWorkspaceType ) ) );
    
  // Focus in name text control  
  if ( listWorkspaces->GetCount() > 0 )
    listWorkspaces->Select( listWorkspaces->GetCount() - 1 );
  wxCommandEvent tmpCmdEvent;
  OnListboxWorkspacesSelected( tmpCmdEvent );
  UpdateWindowUI( wxUPDATE_UI_RECURSE );
  txtWorkspaceName->SetFocus();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_WORKSPACES_DELETE
 */

void PreferencesDialog::OnButtonWorkspacesDeleteClick( wxCommandEvent& event )
{
  workspaceContainer.erase( listWorkspaces->GetStringSelection() );

  int tmpSel = listWorkspaces->GetSelection();
  tmpSel = std::max( 0, std::min( tmpSel - 1 , listWorkspaces->GetSelection() - 1 ) );
  listWorkspaces->Delete( listWorkspaces->GetSelection() );
  if ( listWorkspaces->GetCount() > 0 )
  {
    listWorkspaces->SetSelection( tmpSel );
    OnListboxWorkspacesSelected( event );
  }
  else 
    listHintsWorkspace->Clear();
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

  originalWorkspaceName = listWorkspaces->GetStringSelection();
  
  txtWorkspaceName->ChangeValue( listWorkspaces->GetStringSelection() );
  Workspace& currentWrk = workspaceContainer[ listWorkspaces->GetStringSelection() ];
  
  if( currentWrk.getType() == WorkspaceValue::STATE )
    radioStates->SetValue( true );
  else
    radioEventTypes->SetValue( true );

  std::vector<WorkspaceValue> tmpAutoTypes = currentWrk.getAutoTypes();
  wxString formatAutoTypes;
  for( std::vector<WorkspaceValue>::iterator it = tmpAutoTypes.begin(); it != tmpAutoTypes.end(); ++it )
    formatAutoTypes<<it->UInfo.eventType<<wxT( ";" );
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

  wxString tmpPath( dirBrowserButtonCFG->GetPath() + wxFileName::GetPathSeparator() + _( "new_hint.cfg" ) );
  wxString tmpDesc( _( "New hint description..." ) );
  std::pair< std::string, std::string > tmpHint = std::pair< std::string, std::string >( std::string( tmpPath.mb_str() ), std::string( tmpDesc.mb_str() ) );
  workspaceContainer[ listWorkspaces->GetStringSelection() ].addHintCFG( tmpHint );
  listHintsWorkspace->Append( paraverMain::getHintComposed( tmpHint ) );
  
  // Focus in description text control  
  if ( listHintsWorkspace->GetCount() > 0 )
    listHintsWorkspace->Select( listHintsWorkspace->GetCount() - 1 );
  wxCommandEvent tmpCmdEvent;
  OnListboxHintsWorkspaceSelected( tmpCmdEvent );
  UpdateWindowUI( wxUPDATE_UI_RECURSE );
  txtHintDescription->SetFocus();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_HINT_DELETE
 */

void PreferencesDialog::OnButtonHintDeleteClick( wxCommandEvent& event )
{
  workspaceContainer[ listWorkspaces->GetStringSelection() ].removeHintCFG( listHintsWorkspace->GetSelection() );

  int tmpSel = listHintsWorkspace->GetSelection();
  tmpSel = std::max( 0, std::min( tmpSel - 1 , listHintsWorkspace->GetSelection() - 1 ) );
  listHintsWorkspace->Delete( listHintsWorkspace->GetSelection() );
  if ( listHintsWorkspace->GetCount() > 0  )
    listHintsWorkspace->SetSelection( tmpSel );
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


  wxString tmpCFGsPath = dirBrowserButtonCFG->GetPath();
  wxString tmpHintPath = fileBrowserHintPath->GetPath();
  if ( tmpHintPath.Len() > tmpCFGsPath.Len() )
  {
    wxString tmpPrefixPath = tmpHintPath.Left( tmpCFGsPath.Len() ) ;
    if ( tmpPrefixPath.Cmp( tmpCFGsPath ) == 0 )
      tmpHintPath = tmpHintPath.Right( tmpHintPath.Len() - tmpCFGsPath.Len() - 1 );
  }

  std::pair< std::string, std::string > tmpHint = std::pair< std::string, std::string >( 
                                                    std::string( tmpHintPath.mb_str() ),
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
  Workspace& tmpWrk = workspaceContainer[ listWorkspaces->GetStringSelection() ];
  //std::string tmpTxt = std::string( event.GetString().mb_str() );
  vector<WorkspaceValue> tmpAutoTypes;
  wxStringTokenizer tmpTokenTypes( event.GetString(), wxT( ";" ) );
  unsigned long tmpEventType;
  while( tmpTokenTypes.HasMoreTokens() )
  {
    wxString token = tmpTokenTypes.GetNextToken();
    if ( token.ToULong( &tmpEventType ) )
    {
      WorkspaceValue tmpWorkSpaceValue;
      if( radioStates->GetValue() )
        tmpWorkSpaceValue.myType = WorkspaceValue::STATE;
      else
        tmpWorkSpaceValue.myType = WorkspaceValue::EVENT;
      tmpWorkSpaceValue.UInfo.eventType = tmpEventType;
      tmpAutoTypes.push_back( tmpWorkSpaceValue );
    }
  }
  tmpWrk.setAutoTypes( tmpAutoTypes );
}

 
/*!
 * wxEVT_KILL_FOCUS event handler for ID_TEXT_WORKSPACE_NAME
 */

void PreferencesDialog::OnTextWorkspaceNameKillFocus( wxFocusEvent& event )
{
  if ( originalWorkspaceName == txtWorkspaceName->GetValue() )
    return;

  bool nameEdited = false;
  wxString tmpName = txtWorkspaceName->GetValue();
  while( workspaceContainer.find( tmpName ) != workspaceContainer.end())
  {
    nameEdited = true;
    tmpName += wxT( "_" );
  }

  Workspace tmpWrk = workspaceContainer[ originalWorkspaceName ];
  workspaceContainer.erase( originalWorkspaceName );
  string tmpStrName = std::string( tmpName.mb_str() );
  tmpWrk.setName( tmpStrName );
  workspaceContainer.insert( std::pair<wxString,Workspace>( tmpName, tmpWrk ) );

  listWorkspaces->SetString( listWorkspaces->GetSelection(), tmpName );
  
  originalWorkspaceName = tmpName;
  txtWorkspaceName->ChangeValue( tmpName );
  
  if( nameEdited )
    ::wxMessageBox( wxT( "Found duplicated workspace name.\nChanged to:\n\n" ) + tmpName,
                    wxT( "Duplicated name" ),
                    wxICON_EXCLAMATION );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_RADIOSTATES
 */

void PreferencesDialog::OnRadiostatesUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listWorkspaces->GetSelection() != wxNOT_FOUND );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_RADIOEVENTYPES
 */

void PreferencesDialog::OnRadioeventypesUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listWorkspaces->GetSelection() != wxNOT_FOUND );
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOSTATES
 */

void PreferencesDialog::OnRadiostatesSelected( wxCommandEvent& event )
{
  Workspace& tmpWrk = workspaceContainer[ listWorkspaces->GetStringSelection() ];

  tmpWrk.setType( WorkspaceValue::STATE );
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOEVENTYPES
 */

void PreferencesDialog::OnRadioeventypesSelected( wxCommandEvent& event )
{
  Workspace& tmpWrk = workspaceContainer[ listWorkspaces->GetStringSelection() ];

  tmpWrk.setType( WorkspaceValue::EVENT );
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_PREFERENCES_GLOBAL_TIME_SESSION
 */

void PreferencesDialog::OnPreferencesGlobalTimeSessionUpdated( wxSpinEvent& event )
{
  checkGlobalAskForPrevSessionLoad->Enable( spinSessionTime->GetValue() != 0 );
}


/*!
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX_TEXT_EDITORS
 */

void PreferencesDialog::OnListboxTextEditorsSelected( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX_TEXT_EDITORS in PreferencesDialog.
  // Before editing this code, remove the block markers.
  event.Skip();
////@end wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX_TEXT_EDITORS in PreferencesDialog. 
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_TEXT_ADD
 */

void PreferencesDialog::OnButtonTxtAddClick( wxCommandEvent& event )
{
#ifdef WIN32
  wxString dialogDefaultDir = wxT( "C:\\Program Files" );  
  wxString fileDialogWildcard = _( "*.exe" );  
#else
  wxString dialogDefaultDir = _( "/usr/bin" );  
  wxString fileDialogWildcard = _( "*" );  
#endif
  long whichDialogStyle = wxFD_DEFAULT_STYLE | wxFD_MULTIPLE;
  wxFileDialog myDialog ( this,
                         wxT( "Select External Text Editors" ),
                         dialogDefaultDir,
                         _( "" ),
                         fileDialogWildcard, 
                         whichDialogStyle );
  if ( myDialog.ShowModal() == wxID_OK )
  {
    wxArrayString paths;
    myDialog.GetPaths( paths );
    for ( int i = 0 ; i < paths.size() ; ++i )
    {
      wxFileName tmpFileName = paths[ i ];
      listTextEditors->Append( tmpFileName.GetFullName() );
    }
  }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_TEXT_DEL
 */

void PreferencesDialog::OnButtonTxtDelClick( wxCommandEvent& event )
{
  int tmpSel = listTextEditors->GetSelection();
  tmpSel = std::max( 0, std::min( tmpSel - 1 , listTextEditors->GetSelection() - 1 ) );
  listTextEditors->Delete( listTextEditors->GetSelection() );

  if ( listTextEditors->GetCount() > 0  )
    listTextEditors->SetSelection( tmpSel );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_TEXT_DEL
 */

void PreferencesDialog::OnButtonTxtDelUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listTextEditors->GetSelection() != wxNOT_FOUND );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_TEXT_UP
 */

void PreferencesDialog::OnButtonTxtUpClick( wxCommandEvent& event )
{
  int tmpSel = listTextEditors->GetSelection();
  wxArrayString items = listTextEditors->GetStrings();
  wxString tmpStr = items[ tmpSel ];
  items[ tmpSel ] = items[ tmpSel - 1 ];
  items[ tmpSel - 1 ] = tmpStr;
  listTextEditors->Set( items );
  listTextEditors->SetSelection( tmpSel - 1 );
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_TEXT_UP
 */

void PreferencesDialog::OnButtonTxtUpUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listTextEditors->GetSelection() != wxNOT_FOUND &&
                listTextEditors->GetSelection() > 0 );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON3
 */

void PreferencesDialog::OnButtonTxtDownClick( wxCommandEvent& event )
{
  int tmpSel = listTextEditors->GetSelection();
  wxArrayString items = listTextEditors->GetStrings();
  wxString tmpStr = items[ tmpSel ];
  items[ tmpSel ] = items[ tmpSel + 1 ];
  items[ tmpSel + 1 ] = tmpStr;
  listTextEditors->Set( items );
  listTextEditors->SetSelection( tmpSel + 1 );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BITMAPBUTTON3
 */

void PreferencesDialog::OnButtonTxtDownUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listTextEditors->GetSelection() != wxNOT_FOUND &&
                listTextEditors->GetSelection() < int( listTextEditors->GetCount() ) - 1 );
}


/*!
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX1
 */

void PreferencesDialog::OnListboxPdfReadersSelected( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX1 in PreferencesDialog.
  // Before editing this code, remove the block markers.
  event.Skip();
////@end wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX1 in PreferencesDialog. 
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON4
 */

void PreferencesDialog::OnButtonPdfAddClick( wxCommandEvent& event )
{
#ifdef WIN32
  wxString dialogDefaultDir = wxT( "C:\\Program Files" );  
  wxString fileDialogWildcard = _( "*.exe" );  
#else
  wxString dialogDefaultDir = _( "/usr/bin" );  
  wxString fileDialogWildcard = _( "*" );  
#endif
  long whichDialogStyle = wxFD_DEFAULT_STYLE | wxFD_MULTIPLE;
  wxFileDialog myDialog ( this,
                         wxT( "Select External PDF Readers" ),
                         dialogDefaultDir,
                         _( "" ),
                         fileDialogWildcard, 
                         whichDialogStyle );
  if ( myDialog.ShowModal() == wxID_OK )
  {
    wxArrayString paths;
    myDialog.GetPaths( paths );
    for ( int i = 0 ; i < paths.size() ; ++i )
    {
      wxFileName tmpFileName = paths[ i ];
      listPDFReaders->Append( tmpFileName.GetFullName() );
    }
  }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON5
 */

void PreferencesDialog::OnButtonPdfDelClick( wxCommandEvent& event )
{
  int tmpSel = listPDFReaders->GetSelection();
  tmpSel = std::max( 0, std::min( tmpSel - 1 , listPDFReaders->GetSelection() - 1 ) );
  listPDFReaders->Delete( listPDFReaders->GetSelection() );

  if ( listPDFReaders->GetCount() > 0  )
    listPDFReaders->SetSelection( tmpSel );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BITMAPBUTTON5
 */

void PreferencesDialog::OnButtonPdfDelUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listPDFReaders->GetSelection() != wxNOT_FOUND );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON6
 */

void PreferencesDialog::OnButtonPdfUpClick( wxCommandEvent& event )
{
  int tmpSel = listPDFReaders->GetSelection();
  wxArrayString items = listPDFReaders->GetStrings();
  wxString tmpStr = items[ tmpSel ];
  items[ tmpSel ] = items[ tmpSel - 1 ];
  items[ tmpSel - 1 ] = tmpStr;
  listPDFReaders->Set( items );
  listPDFReaders->SetSelection( tmpSel - 1 );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BITMAPBUTTON6
 */

void PreferencesDialog::OnButtonPdfUpUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listPDFReaders->GetSelection() != wxNOT_FOUND &&
                listPDFReaders->GetSelection() > 0 );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON7
 */

void PreferencesDialog::OnButtonPdfDownClick( wxCommandEvent& event )
{
  int tmpSel = listPDFReaders->GetSelection();
  wxArrayString items = listPDFReaders->GetStrings();
  wxString tmpStr = items[ tmpSel ];
  items[ tmpSel ] = items[ tmpSel + 1 ];
  items[ tmpSel + 1 ] = tmpStr;
  listPDFReaders->Set( items );
  listPDFReaders->SetSelection( tmpSel + 1 );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BITMAPBUTTON7
 */

void PreferencesDialog::OnButtonPdfDownUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listPDFReaders->GetSelection() != wxNOT_FOUND &&
                listPDFReaders->GetSelection() < int( listPDFReaders->GetCount() ) - 1 );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON
 */

void PreferencesDialog::OnButtonWorkspacesImportClick( wxCommandEvent& event )
{
  wxString dialogDefaultDir = _( "" );
  wxString fileDialogWildcard = _( "Workspace file (*.ws)|*.ws|All files (*.*)|*" );  

  long whichDialogStyle = wxFD_DEFAULT_STYLE | wxFD_MULTIPLE;
  wxFileDialog myDialog ( this,
                         wxT( "Import workspaces" ),
                         dialogDefaultDir,
                         _( "" ),
                         fileDialogWildcard, 
                         whichDialogStyle );
  if ( myDialog.ShowModal() == wxID_OK )
  {
    wxArrayString paths;
    myDialog.GetPaths( paths );
    for ( int i = 0 ; i < paths.size() ; ++i )
    {
      wxFileName tmpFileName = paths[ i ];
      Workspace ws;
      std::string wsPath( tmpFileName.GetPath().mb_str() );
    #ifdef WIN32
      wsPath.append( "\\" );
    #else
      wsPath.append( "/" );
    #endif
      wsPath.append( tmpFileName.GetFullName().mb_str() );
      ws.loadXML( wsPath );

      wxString wsName( ws.getName().c_str(), wxConvUTF8 );
      workspaceContainer.insert( std::pair<wxString,Workspace>( wsName, ws ) );
      
      listWorkspaces->Append( wsName );
      WorkspaceManager::getInstance()->addWorkspace( ws );
    }
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_WORKSPACES_IMPORT
 */

void PreferencesDialog::OnButtonWorkspacesImportUpdate( wxUpdateUIEvent& event )
{
  event.Enable( true );
}



/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_WORKSPACES_EXPORT
 */

void PreferencesDialog::OnButtonWorkspacesExportClick( wxCommandEvent& event )
{
#ifdef WIN32
  wxString dialogDefaultDir = _(".\\");
#else
  wxString dialogDefaultDir = _("./");
#endif

  wxString fileDialogWildcard = _( ".ws" ); 

  wxString defaultFile = listWorkspaces->GetString( listWorkspaces->GetSelection() );  
  long whichDialogStyle = wxFD_SAVE | wxFD_CHANGE_DIR;
  wxFileDialog myDialog ( this,
                         wxT( "Export workspaces" ),
                         dialogDefaultDir,
                         _( "" ),
                         fileDialogWildcard, 
                         whichDialogStyle );
  if ( myDialog.ShowModal() == wxID_OK )
  {
    Workspace ws = workspaceContainer[ defaultFile ];
    std::string chosenPath( myDialog.GetPath().mb_str() );
    if ( myDialog.GetPath().AfterLast( '.' ) != _( "ws" ) )
      chosenPath.append( ".ws" );
    ws.saveXML( chosenPath );
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_WORKSPACES_EXPORT
 */

void PreferencesDialog::OnButtonWorkspacesExportUpdate( wxUpdateUIEvent& event )
{
  event.Enable( listWorkspaces->GetSelection() != wxNOT_FOUND );
}
