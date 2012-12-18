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
#include "semanticcolor.h"
#include "labelconstructor.h"

// Validators
#include <wx/arrstr.h>
#include <wx/validate.h>

////@begin XPM images
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
  timelineFunctionAsColor = true;
  timelineGradientFunction = 0;
  timelineNameFormatFull = "%W @ %T";
  timelineNameFormatPrefix = "New Window # %N";
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
  txtMaximumTraceSize = NULL;
  dirPickerTrace = NULL;
  dirPickerCFG = NULL;
  dirPickerXML = NULL;
  dirPickerTutorials = NULL;
  dirPickerTmp = NULL;
  checkGlobalSingleInstance = NULL;
  spinSessionTime = NULL;
  txtTimelineNameFormatPrefix = NULL;
  txtTimelineNameFormatFull = NULL;
  checkTimelineCommunicationLines = NULL;
  checkTimelineEventLines = NULL;
  checkTimelineFunctionAsColor = NULL;
  choiceTimelineColor = NULL;
  choiceTimelineGradientFunction = NULL;
  choiceTimelineDrawmodeTime = NULL;
  choiceTimelineDrawmodeObjects = NULL;
  choiceTimelinePixelSize = NULL;
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
  itemStaticBoxSizer4->Add(checkGlobalFillStateGaps, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer4->Add(itemBoxSizer6, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText7 = new wxStaticText( itemPanel2, wxID_STATIC, _("Maximum loadable trace size (MB)"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer6->Add(itemStaticText7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txtMaximumTraceSize = new wxSpinCtrl( itemPanel2, ID_TEXTCTRL_MAXIMUM_LOADABLE_TRACE_SIZE, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 5000, 0 );
  itemBoxSizer6->Add(txtMaximumTraceSize, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer9Static = new wxStaticBox(itemPanel2, wxID_ANY, _("  Default directories  "));
  wxStaticBoxSizer* itemStaticBoxSizer9 = new wxStaticBoxSizer(itemStaticBoxSizer9Static, wxVERTICAL);
  itemBoxSizer3->Add(itemStaticBoxSizer9, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer9->Add(itemBoxSizer10, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText11 = new wxStaticText( itemPanel2, wxID_STATIC, _("Traces"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer10->Add(itemStaticText11, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  dirPickerTrace = new wxDirPickerCtrl( itemPanel2, ID_DIRPICKERCTRL_TRACES, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_USE_TEXTCTRL );
  if (PreferencesDialog::ShowToolTips())
    dirPickerTrace->SetToolTip(_("Base path to traces files (.prv, .prv.gz, .pcf and .row)."));
  itemBoxSizer10->Add(dirPickerTrace, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer9->Add(itemBoxSizer13, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText14 = new wxStaticText( itemPanel2, wxID_STATIC, _("CFGs"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer13->Add(itemStaticText14, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  dirPickerCFG = new wxDirPickerCtrl( itemPanel2, ID_DIRPICKERCTRL_CFG, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_USE_TEXTCTRL );
  dirPickerCFG->SetHelpText(_("Base path to configuration files (.cfg). "));
  if (PreferencesDialog::ShowToolTips())
    dirPickerCFG->SetToolTip(_("Base path to configuration files (.cfg)."));
  itemBoxSizer13->Add(dirPickerCFG, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer9->Add(itemBoxSizer16, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText17 = new wxStaticText( itemPanel2, wxID_STATIC, _("Filters XML"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer16->Add(itemStaticText17, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  dirPickerXML = new wxDirPickerCtrl( itemPanel2, ID_DIRPICKERCTRL_XML, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_USE_TEXTCTRL );
  if (PreferencesDialog::ShowToolTips())
    dirPickerXML->SetToolTip(_("Base path to Cut/Filters XML."));
  itemBoxSizer16->Add(dirPickerXML, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer9->Add(itemBoxSizer19, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText20 = new wxStaticText( itemPanel2, wxID_STATIC, _("Tutorials root"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer19->Add(itemStaticText20, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  dirPickerTutorials = new wxDirPickerCtrl( itemPanel2, ID_DIRPICKERCTRL_TUTORIALS, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_USE_TEXTCTRL );
  if (PreferencesDialog::ShowToolTips())
    dirPickerTutorials->SetToolTip(_("Base path to wxparaver tutorials."));
  itemBoxSizer19->Add(dirPickerTutorials, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer9->Add(itemBoxSizer22, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText23 = new wxStaticText( itemPanel2, wxID_STATIC, _("Tmp dir"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer22->Add(itemStaticText23, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  dirPickerTmp = new wxDirPickerCtrl( itemPanel2, ID_DIRPICKERCTRL_TMP, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_USE_TEXTCTRL );
  if (PreferencesDialog::ShowToolTips())
    dirPickerTmp->SetToolTip(_("Working directory for results and logs."));
  itemBoxSizer22->Add(dirPickerTmp, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer25Static = new wxStaticBox(itemPanel2, wxID_ANY, _("  Behaviour  "));
  wxStaticBoxSizer* itemStaticBoxSizer25 = new wxStaticBoxSizer(itemStaticBoxSizer25Static, wxVERTICAL);
  itemBoxSizer3->Add(itemStaticBoxSizer25, 0, wxGROW|wxALL, 5);
  checkGlobalSingleInstance = new wxCheckBox( itemPanel2, ID_PREFERENCES_GLOBAL_SINGLE_INSTANCE, _("Allow only one running instance"), wxDefaultPosition, wxDefaultSize, 0 );
  checkGlobalSingleInstance->SetValue(false);
  itemStaticBoxSizer25->Add(checkGlobalSingleInstance, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer27 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer25->Add(itemBoxSizer27, 1, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText28 = new wxStaticText( itemPanel2, wxID_STATIC, _("Automatically save session every"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer27->Add(itemStaticText28, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  spinSessionTime = new wxSpinCtrl( itemPanel2, ID_PREFERENCES_GLOBAL_TIME_SESSION, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
  itemBoxSizer27->Add(spinSessionTime, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticText* itemStaticText30 = new wxStaticText( itemPanel2, wxID_STATIC, _("minutes"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer27->Add(itemStaticText30, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  GetBookCtrl()->AddPage(itemPanel2, _("Global"));

  wxPanel* itemPanel31 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_TIMELINE, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer32 = new wxBoxSizer(wxHORIZONTAL);
  itemPanel31->SetSizer(itemBoxSizer32);

  wxBoxSizer* itemBoxSizer33 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer32->Add(itemBoxSizer33, 3, wxGROW|wxTOP, 5);
  wxStaticBox* itemStaticBoxSizer34Static = new wxStaticBox(itemPanel31, wxID_STATIC, _("  Name Format  "));
  wxStaticBoxSizer* itemStaticBoxSizer34 = new wxStaticBoxSizer(itemStaticBoxSizer34Static, wxVERTICAL);
  itemStaticBoxSizer34Static->Enable(false);
  itemBoxSizer33->Add(itemStaticBoxSizer34, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer35 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer34->Add(itemBoxSizer35, 0, wxGROW|wxTOP|wxBOTTOM, 5);
  wxStaticText* itemStaticText36 = new wxStaticText( itemPanel31, wxID_STATIC, _("Prefix"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText36->SetToolTip(_("Default name for new timelines, applied in window browser."));
  itemStaticText36->Enable(false);
  itemBoxSizer35->Add(itemStaticText36, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  txtTimelineNameFormatPrefix = new wxTextCtrl( itemPanel31, ID_PREFERENCES_TIMELINE_NAME_PREFIX, _("New Window # %N"), wxDefaultPosition, wxDefaultSize, 0 );
  txtTimelineNameFormatPrefix->SetMaxLength(256);
  if (PreferencesDialog::ShowToolTips())
    txtTimelineNameFormatPrefix->SetToolTip(_("Default name for new timelines, applied in window browser."));
  txtTimelineNameFormatPrefix->Enable(false);
  itemBoxSizer35->Add(txtTimelineNameFormatPrefix, 2, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer38 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer34->Add(itemBoxSizer38, 0, wxGROW|wxBOTTOM, 5);
  wxStaticText* itemStaticText39 = new wxStaticText( itemPanel31, wxID_STATIC, _("Full"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText39->SetToolTip(_("Default full name for new timelines, applied in window title."));
  itemStaticText39->Enable(false);
  itemBoxSizer38->Add(itemStaticText39, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  txtTimelineNameFormatFull = new wxTextCtrl( itemPanel31, ID_PREFERENCES_TIMELINE_NAME_FULL, _("%P @ %T"), wxDefaultPosition, wxDefaultSize, 0 );
  txtTimelineNameFormatFull->SetMaxLength(256);
  if (PreferencesDialog::ShowToolTips())
    txtTimelineNameFormatFull->SetToolTip(_("Default full name for new timelines, applied in window title."));
  txtTimelineNameFormatFull->Enable(false);
  itemBoxSizer38->Add(txtTimelineNameFormatFull, 2, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxStaticBox* itemStaticBoxSizer41Static = new wxStaticBox(itemPanel31, wxID_ANY, _("  Rendering Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer41 = new wxStaticBoxSizer(itemStaticBoxSizer41Static, wxVERTICAL);
  itemBoxSizer33->Add(itemStaticBoxSizer41, 1, wxGROW|wxALL, 5);
  checkTimelineCommunicationLines = new wxCheckBox( itemPanel31, ID_PREFERENCES_TIMELINE_COMMUNICATION_LINES, _("Communication Lines"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineCommunicationLines->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineCommunicationLines->SetToolTip(_("View communications lines in new timelines."));
  itemStaticBoxSizer41->Add(checkTimelineCommunicationLines, 0, wxGROW|wxTOP, 5);

  checkTimelineEventLines = new wxCheckBox( itemPanel31, ID_PREFERENCES_TIMELINE_EVENT_LINES, _("Event Flags"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineEventLines->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineEventLines->SetToolTip(_("View event flags in new timelines."));
  itemStaticBoxSizer41->Add(checkTimelineEventLines, 0, wxGROW, 5);

  checkTimelineFunctionAsColor = new wxCheckBox( itemPanel31, ID_PREFERENCES_TIMELINE_SEMANTIC_AS_COLOR, _("Semantic Values as Color"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineFunctionAsColor->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineFunctionAsColor->SetToolTip(_("Represent semantic values as function in new timelines."));
  itemStaticBoxSizer41->Add(checkTimelineFunctionAsColor, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer45 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer41->Add(itemBoxSizer45, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText46 = new wxStaticText( itemPanel31, wxID_STATIC, _("Color"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText46->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer45->Add(itemStaticText46, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceTimelineColorStrings;
  choiceTimelineColor = new wxChoice( itemPanel31, ID_PREFERENCES_TIMELINE_COLOR, wxDefaultPosition, wxDefaultSize, choiceTimelineColorStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineColor->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer45->Add(choiceTimelineColor, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer48 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer41->Add(itemBoxSizer48, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText49 = new wxStaticText( itemPanel31, wxID_STATIC, _("Gradient Function"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText49->SetToolTip(_("Gradient function selected in new timeline."));
  itemBoxSizer48->Add(itemStaticText49, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxArrayString choiceTimelineGradientFunctionStrings;
  choiceTimelineGradientFunction = new wxChoice( itemPanel31, ID_PREFERENCES_TIMELINE_GRADIENT, wxDefaultPosition, wxDefaultSize, choiceTimelineGradientFunctionStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineGradientFunction->SetToolTip(_("Gradient function selected in new timeline."));
  itemBoxSizer48->Add(choiceTimelineGradientFunction, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer51 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer41->Add(itemBoxSizer51, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText52 = new wxStaticText( itemPanel31, wxID_STATIC, _("Drawmode Time"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText52->SetToolTip(_("Drawmode for time axis selected in new timeline."));
  itemBoxSizer51->Add(itemStaticText52, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceTimelineDrawmodeTimeStrings;
  choiceTimelineDrawmodeTime = new wxChoice( itemPanel31, ID_PREFERENCES_TIMELINE_DRAWMODE_TIME, wxDefaultPosition, wxDefaultSize, choiceTimelineDrawmodeTimeStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineDrawmodeTime->SetToolTip(_("Drawmode for time axis selected in new timeline."));
  itemBoxSizer51->Add(choiceTimelineDrawmodeTime, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer54 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer41->Add(itemBoxSizer54, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText55 = new wxStaticText( itemPanel31, wxID_STATIC, _("Drawmode Objects"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText55->SetToolTip(_("Drawmode for objects axis selected in new timeline."));
  itemBoxSizer54->Add(itemStaticText55, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceTimelineDrawmodeObjectsStrings;
  choiceTimelineDrawmodeObjects = new wxChoice( itemPanel31, ID_PREFERENCES_TIMELINE_DRAWMODE_OBJECTS, wxDefaultPosition, wxDefaultSize, choiceTimelineDrawmodeObjectsStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineDrawmodeObjects->SetToolTip(_("Drawmode for objects axis selected in new timeline."));
  itemBoxSizer54->Add(choiceTimelineDrawmodeObjects, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer57 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer41->Add(itemBoxSizer57, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText58 = new wxStaticText( itemPanel31, wxID_STATIC, _("Pixel Size"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText58->SetToolTip(_("Pixel size selected in new timeline."));
  itemBoxSizer57->Add(itemStaticText58, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString choiceTimelinePixelSizeStrings;
  choiceTimelinePixelSize = new wxChoice( itemPanel31, ID_PREFERENCES_TIMELINE_PIXEL_SIZE, wxDefaultPosition, wxDefaultSize, choiceTimelinePixelSizeStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelinePixelSize->SetToolTip(_("Pixel size selected in new timeline."));
  itemBoxSizer57->Add(choiceTimelinePixelSize, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer60 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer32->Add(itemBoxSizer60, 2, wxGROW|wxTOP, 5);
  wxStaticBox* itemStaticBoxSizer61Static = new wxStaticBox(itemPanel31, wxID_ANY, _("  What / Where  "));
  wxStaticBoxSizer* itemStaticBoxSizer61 = new wxStaticBoxSizer(itemStaticBoxSizer61Static, wxVERTICAL);
  itemBoxSizer60->Add(itemStaticBoxSizer61, 2, wxGROW|wxALL, 5);
  checkTimelineWWSemantic = new wxCheckBox( itemPanel31, ID_CHECKBOX_TIMELINE_WW_SEMANTIC, _("Semantic"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWSemantic->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWSemantic->SetToolTip(_("Show semantic information when left click in timeline."));
  itemStaticBoxSizer61->Add(checkTimelineWWSemantic, 0, wxGROW|wxTOP, 5);

  checkTimelineWWEvents = new wxCheckBox( itemPanel31, ID_CHECKBOX_TIMELINE_WW_EVENTS, _("Events"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWEvents->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWEvents->SetToolTip(_("Show events information when left click in timeline."));
  itemStaticBoxSizer61->Add(checkTimelineWWEvents, 0, wxGROW, 5);

  checkTimelineWWCommunications = new wxCheckBox( itemPanel31, ID_CHECKBOX_TIMELINE_WW_COMMUNICATIONS, _("Communications"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWCommunications->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWCommunications->SetToolTip(_("Show communications information when left click in timeline."));
  itemStaticBoxSizer61->Add(checkTimelineWWCommunications, 0, wxGROW, 5);

  checkTimelineWWPreviousNext = new wxCheckBox( itemPanel31, ID_CHECKBOX_TIMELINE_WW_PREVIOUS_NEXT, _("Previous / Next"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWPreviousNext->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWPreviousNext->SetToolTip(_("In addition to current semantic value, show information for previous and next ones when left click in timeline."));
  itemStaticBoxSizer61->Add(checkTimelineWWPreviousNext, 0, wxGROW, 5);

  checkTimelineWWText = new wxCheckBox( itemPanel31, ID_CHECKBOX_TIMELINE_WW_TEXT, _("Text"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWText->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWText->SetToolTip(_("Show semantic value as text when left click in timeline."));
  itemStaticBoxSizer61->Add(checkTimelineWWText, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer67 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer61->Add(itemBoxSizer67, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText68 = new wxStaticText( itemPanel31, wxID_STATIC, _("Precision"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer67->Add(itemStaticText68, 2, wxALIGN_CENTER_VERTICAL, 5);

  txtTimelineWWPrecision = new wxSpinCtrl( itemPanel31, ID_PREFERENCES_TIMELINE_WW_PRECISION, _T("2"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 2 );
  if (PreferencesDialog::ShowToolTips())
    txtTimelineWWPrecision->SetToolTip(_("Decimal precision for semantic values."));
  itemBoxSizer67->Add(txtTimelineWWPrecision, 3, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer70 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer61->Add(itemBoxSizer70, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText71 = new wxStaticText( itemPanel31, wxID_STATIC, _("Event pixels"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticText71->SetHelpText(_("Pixels to look around for events"));
  if (PreferencesDialog::ShowToolTips())
    itemStaticText71->SetToolTip(_("Pixels to look around for events"));
  itemBoxSizer70->Add(itemStaticText71, 2, wxALIGN_CENTER_VERTICAL, 5);

  txtTimelineWWEventPixels = new wxSpinCtrl( itemPanel31, ID_PREFERENCES_TIMELINE_WW_EVENT_PIXELS, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
  txtTimelineWWEventPixels->SetHelpText(_("Pixels to look around for events"));
  if (PreferencesDialog::ShowToolTips())
    txtTimelineWWEventPixels->SetToolTip(_("Pixels to look around for events"));
  itemBoxSizer70->Add(txtTimelineWWEventPixels, 3, wxALIGN_CENTER_VERTICAL, 5);

  wxStaticBox* itemStaticBoxSizer73Static = new wxStaticBox(itemPanel31, wxID_ANY, _("  Default Save Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer73 = new wxStaticBoxSizer(itemStaticBoxSizer73Static, wxVERTICAL);
  itemBoxSizer60->Add(itemStaticBoxSizer73, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer74 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer73->Add(itemBoxSizer74, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxStaticText* itemStaticText75 = new wxStaticText( itemPanel31, wxID_STATIC, _("Image as"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText75->SetToolTip(_("Default image format selected."));
  itemBoxSizer74->Add(itemStaticText75, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceTimelineSaveImageFormatStrings;
  choiceTimelineSaveImageFormat = new wxChoice( itemPanel31, ID_PREFERENCES_TIMELINE_SAVE_AS_IMAGE, wxDefaultPosition, wxDefaultSize, choiceTimelineSaveImageFormatStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineSaveImageFormat->SetToolTip(_("Default image format selected."));
  itemBoxSizer74->Add(choiceTimelineSaveImageFormat, 3, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer77 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer73->Add(itemBoxSizer77, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxStaticText* itemStaticText78 = new wxStaticText( itemPanel31, wxID_STATIC, _("Text as"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText78->SetToolTip(_("Default text format selected."));
  itemBoxSizer77->Add(itemStaticText78, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceTimelineSaveTextFormatStrings;
  choiceTimelineSaveTextFormat = new wxChoice( itemPanel31, ID_PREFERENCES_TIMELINE_SAVE_AS_TEXT, wxDefaultPosition, wxDefaultSize, choiceTimelineSaveTextFormatStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineSaveTextFormat->SetToolTip(_("Default text format selected."));
  itemBoxSizer77->Add(choiceTimelineSaveTextFormat, 3, wxALIGN_CENTER_VERTICAL, 5);

  GetBookCtrl()->AddPage(itemPanel31, _("Timeline"));

  wxPanel* itemPanel80 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_HISTOGRAM, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  if (PreferencesDialog::ShowToolTips())
    itemPanel80->SetToolTip(_("Histogram preferences."));
  wxBoxSizer* itemBoxSizer81 = new wxBoxSizer(wxHORIZONTAL);
  itemPanel80->SetSizer(itemBoxSizer81);

  wxBoxSizer* itemBoxSizer82 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer81->Add(itemBoxSizer82, 3, wxGROW|wxTOP, 5);
  wxStaticBox* itemStaticBoxSizer83Static = new wxStaticBox(itemPanel80, wxID_STATIC, _("  Name Format  "));
  wxStaticBoxSizer* itemStaticBoxSizer83 = new wxStaticBoxSizer(itemStaticBoxSizer83Static, wxVERTICAL);
  itemStaticBoxSizer83Static->Enable(false);
  itemBoxSizer82->Add(itemStaticBoxSizer83, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer84 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer83->Add(itemBoxSizer84, 0, wxGROW|wxTOP|wxBOTTOM, 5);
  wxStaticText* itemStaticText85 = new wxStaticText( itemPanel80, wxID_STATIC, _("Prefix"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText85->SetToolTip(_("Default name for new histograms, applied in window browser."));
  itemStaticText85->Enable(false);
  itemBoxSizer84->Add(itemStaticText85, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  txtHistogramNameFormatPrefix = new wxTextCtrl( itemPanel80, ID_PREFERENCES_HISTOGRAM_NAME_PREFIX, _("New Histogram # %N"), wxDefaultPosition, wxDefaultSize, 0 );
  txtHistogramNameFormatPrefix->SetMaxLength(256);
  if (PreferencesDialog::ShowToolTips())
    txtHistogramNameFormatPrefix->SetToolTip(_("Default name for new histograms, applied in window browser."));
  txtHistogramNameFormatPrefix->Enable(false);
  itemBoxSizer84->Add(txtHistogramNameFormatPrefix, 2, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer87 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer83->Add(itemBoxSizer87, 0, wxGROW|wxBOTTOM, 5);
  wxStaticText* itemStaticText88 = new wxStaticText( itemPanel80, wxID_STATIC, _("Full"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText88->SetToolTip(_("Default full name for new histograms, applied in window title."));
  itemStaticText88->Enable(false);
  itemBoxSizer87->Add(itemStaticText88, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  txtHistogramNameFormatFull = new wxTextCtrl( itemPanel80, ID_PREFERENCES_HISTOGRAM_NAME_FULL, _("%P @ %T"), wxDefaultPosition, wxDefaultSize, 0 );
  txtHistogramNameFormatFull->SetMaxLength(256);
  if (PreferencesDialog::ShowToolTips())
    txtHistogramNameFormatFull->SetToolTip(_("Default full name for new histograms, applied in window title."));
  txtHistogramNameFormatFull->Enable(false);
  itemBoxSizer87->Add(txtHistogramNameFormatFull, 2, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxStaticBox* itemStaticBoxSizer90Static = new wxStaticBox(itemPanel80, wxID_ANY, _("  Matrix Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer90 = new wxStaticBoxSizer(itemStaticBoxSizer90Static, wxVERTICAL);
  itemBoxSizer82->Add(itemStaticBoxSizer90, 1, wxGROW|wxALL, 5);
  checkHistogramZoom = new wxCheckBox( itemPanel80, ID_PREFERENCES_HISTOGRAM_MATRIX_ZOOM, _("Zoom View as default"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramZoom->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramZoom->SetToolTip(_("Select between numeric and zoom view for the histogram matrix."));
  itemStaticBoxSizer90->Add(checkHistogramZoom, 0, wxGROW|wxTOP, 5);

  checkHistogramHorizontal = new wxCheckBox( itemPanel80, ID_PREFERENCES_HISTOGRAM_MATRIX_HORIZONTAL, _("Horizontal disposition"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramHorizontal->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramHorizontal->SetToolTip(_("Select between horizontal / vertical disposition for the histogram matrix."));
  itemStaticBoxSizer90->Add(checkHistogramHorizontal, 0, wxGROW, 5);

  checkHistogramHideEmpty = new wxCheckBox( itemPanel80, ID_PREFERENCES_HISTOGRAM_MATRIX_HIDE_EMPTY, _("Hide empty columns"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramHideEmpty->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramHideEmpty->SetToolTip(_("Hide columns when all its cells are empty."));
  itemStaticBoxSizer90->Add(checkHistogramHideEmpty, 0, wxGROW, 5);

  checkHistogramShowGradient = new wxCheckBox( itemPanel80, ID_PREFERENCES_HISTOGRAM_MATRIX_GRADIENT, _("Show Gradient Colors"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramShowGradient->SetValue(true);
  itemStaticBoxSizer90->Add(checkHistogramShowGradient, 0, wxGROW, 5);

  checkHistogramLabelsColor = new wxCheckBox( itemPanel80, ID_PREFERENCES_HISTOGRAM_MATRIX_LABELS_COLOR, _("Labels color"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramLabelsColor->SetValue(false);
  itemStaticBoxSizer90->Add(checkHistogramLabelsColor, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer96 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer90->Add(itemBoxSizer96, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText97 = new wxStaticText( itemPanel80, wxID_STATIC, _("Gradient Function"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText97->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer96->Add(itemStaticText97, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceHistogramGradientFunctionStrings;
  choiceHistogramGradientFunction = new wxChoice( itemPanel80, ID_PREFERENCES_HISTOGRAM_MATRIX_GRADIENT_FUNCTION, wxDefaultPosition, wxDefaultSize, choiceHistogramGradientFunctionStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceHistogramGradientFunction->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer96->Add(choiceHistogramGradientFunction, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer99 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer90->Add(itemBoxSizer99, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText100 = new wxStaticText( itemPanel80, wxID_STATIC, _("Drawmode Semantic"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText100->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer99->Add(itemStaticText100, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceHistogramDrawmodeSemanticStrings;
  choiceHistogramDrawmodeSemantic = new wxChoice( itemPanel80, ID_PREFERENCES_HISTOGRAM_MATRIX_DRAWMODE_SEMANTIC, wxDefaultPosition, wxDefaultSize, choiceHistogramDrawmodeSemanticStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceHistogramDrawmodeSemantic->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer99->Add(choiceHistogramDrawmodeSemantic, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer102 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer90->Add(itemBoxSizer102, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText103 = new wxStaticText( itemPanel80, wxID_STATIC, _("Drawmode Objects"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText103->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer102->Add(itemStaticText103, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceHistogramDrawmodeObjectsStrings;
  choiceHistogramDrawmodeObjects = new wxChoice( itemPanel80, ID_PREFERENCES_HISTOGRAM_MATRIX_DRAWMODE_OBJECTS, wxDefaultPosition, wxDefaultSize, choiceHistogramDrawmodeObjectsStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceHistogramDrawmodeObjects->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer102->Add(choiceHistogramDrawmodeObjects, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer105 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer81->Add(itemBoxSizer105, 2, wxGROW|wxTOP, 5);
  wxStaticBox* itemStaticBoxSizer106Static = new wxStaticBox(itemPanel80, wxID_ANY, _("  Cell Format  "));
  wxStaticBoxSizer* itemStaticBoxSizer106 = new wxStaticBoxSizer(itemStaticBoxSizer106Static, wxVERTICAL);
  itemBoxSizer105->Add(itemStaticBoxSizer106, 0, wxGROW|wxALL, 5);
  checkHistogramScientificNotation = new wxCheckBox( itemPanel80, ID_PREFERENCES_HISTOGRAM_SCIENTIFIC_NOTATION, _("Scientific Notation"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramScientificNotation->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramScientificNotation->SetToolTip(_("Write cell content using scientific notation."));
  itemStaticBoxSizer106->Add(checkHistogramScientificNotation, 0, wxGROW|wxTOP, 5);

  checkHistogramThousandsSeparator = new wxCheckBox( itemPanel80, ID_PREFERENCES_HISTOGRAM_THOUSANDS_SEPARATOR, _("Thousands Separator"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramThousandsSeparator->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramThousandsSeparator->SetToolTip(_("Write cell content using thousands separator."));
  itemStaticBoxSizer106->Add(checkHistogramThousandsSeparator, 0, wxGROW, 5);

  checkHistogramShowUnits = new wxCheckBox( itemPanel80, ID_PREFERENCES_HISTOGRAM_SHOW_UNITS, _("Show Units"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramShowUnits->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramShowUnits->SetToolTip(_("End numeric information with its units."));
  itemStaticBoxSizer106->Add(checkHistogramShowUnits, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer110 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer106->Add(itemBoxSizer110, 0, wxGROW|wxBOTTOM, 5);
  wxStaticText* itemStaticText111 = new wxStaticText( itemPanel80, wxID_STATIC, _("Precision"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText111->SetToolTip(_("Decimal precision applied to the presentation of histogram metrics."));
  itemBoxSizer110->Add(itemStaticText111, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txtHistogramPrecision = new wxSpinCtrl( itemPanel80, ID_PREFERENCES_HISTOGRAM_PRECISION, _T("2"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 2 );
  if (PreferencesDialog::ShowToolTips())
    txtHistogramPrecision->SetToolTip(_("Decimal precision applied to the presentation of histogram metrics."));
  itemBoxSizer110->Add(txtHistogramPrecision, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer113Static = new wxStaticBox(itemPanel80, wxID_ANY, _("  Compute Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer113 = new wxStaticBoxSizer(itemStaticBoxSizer113Static, wxVERTICAL);
  itemBoxSizer105->Add(itemStaticBoxSizer113, 0, wxGROW|wxALL, 5);
  checkHistogramAutofitControlScale = new wxCheckBox( itemPanel80, ID_PREFERENCES_HISTOGRAM_AUTOFIT_CONTROL, _("Autofit Control Scale"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramAutofitControlScale->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramAutofitControlScale->SetToolTip(_("Fit control scale when histogram is created."));
  itemStaticBoxSizer113->Add(checkHistogramAutofitControlScale, 0, wxGROW|wxTOP, 5);

  checkHistogramAutofit3DScale = new wxCheckBox( itemPanel80, ID_PREFERENCES_HISTOGRAM_AUTOFIT_3D, _("Autofit 3D Scale"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramAutofit3DScale->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramAutofit3DScale->SetToolTip(_("Fit 3D scale when histogram is created."));
  itemStaticBoxSizer113->Add(checkHistogramAutofit3DScale, 0, wxGROW, 5);

  checkHistogramAutofitDataGradient = new wxCheckBox( itemPanel80, ID_PREFERENCES_HISTOGRAM_AUTOFIT_DATA_GRADIENT, _("Autofit Data Gradient"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramAutofitDataGradient->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramAutofitDataGradient->SetToolTip(_("Fit data gradient when histogram is created."));
  itemStaticBoxSizer113->Add(checkHistogramAutofitDataGradient, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer117 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer113->Add(itemBoxSizer117, 0, wxGROW|wxBOTTOM, 5);
  wxStaticText* itemStaticText118 = new wxStaticText( itemPanel80, wxID_STATIC, _("Columns"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText118->SetToolTip(_("Initial number of columns for new histograms."));
  itemBoxSizer117->Add(itemStaticText118, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txtHistogramNumColumns = new wxSpinCtrl( itemPanel80, ID_PREFERENCES_HISTOGRAM_NUMCOLUMNS, _T("20"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 256, 20 );
  if (PreferencesDialog::ShowToolTips())
    txtHistogramNumColumns->SetToolTip(_("Initial number of columns for new histograms."));
  itemBoxSizer117->Add(txtHistogramNumColumns, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer120Static = new wxStaticBox(itemPanel80, wxID_ANY, _("  Default Save Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer120 = new wxStaticBoxSizer(itemStaticBoxSizer120Static, wxVERTICAL);
  itemBoxSizer105->Add(itemStaticBoxSizer120, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer121 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer120->Add(itemBoxSizer121, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxStaticText* itemStaticText122 = new wxStaticText( itemPanel80, wxID_STATIC, _("Image as"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText122->SetToolTip(_("Default image format selected."));
  itemBoxSizer121->Add(itemStaticText122, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceHistogramSaveImageFormatStrings;
  choiceHistogramSaveImageFormat = new wxChoice( itemPanel80, ID_PREFERENCES_HISTOGRAM_SAVE_IMAGE_FORMAT, wxDefaultPosition, wxDefaultSize, choiceHistogramSaveImageFormatStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceHistogramSaveImageFormat->SetToolTip(_("Default image format selected."));
  itemBoxSizer121->Add(choiceHistogramSaveImageFormat, 3, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer124 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer120->Add(itemBoxSizer124, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxStaticText* itemStaticText125 = new wxStaticText( itemPanel80, wxID_STATIC, _("Text as"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText125->SetToolTip(_("Default text format selected."));
  itemBoxSizer124->Add(itemStaticText125, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceHistogramSaveTextFormatStrings;
  choiceHistogramSaveTextFormat = new wxChoice( itemPanel80, ID_PREFERENCES_HISTOGRAM_SAVE_TXT_FORMAT, wxDefaultPosition, wxDefaultSize, choiceHistogramSaveTextFormatStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceHistogramSaveTextFormat->SetToolTip(_("Default text format selected."));
  itemBoxSizer124->Add(choiceHistogramSaveTextFormat, 3, wxALIGN_CENTER_VERTICAL, 5);

  GetBookCtrl()->AddPage(itemPanel80, _("Histogram"));

  wxPanel* itemPanel127 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_COLOR, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer128 = new wxBoxSizer(wxVERTICAL);
  itemPanel127->SetSizer(itemBoxSizer128);

  wxStaticBox* itemStaticBoxSizer129Static = new wxStaticBox(itemPanel127, wxID_ANY, _("  Timeline  "));
  wxStaticBoxSizer* itemStaticBoxSizer129 = new wxStaticBoxSizer(itemStaticBoxSizer129Static, wxVERTICAL);
  itemBoxSizer128->Add(itemStaticBoxSizer129, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer130 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer129->Add(itemBoxSizer130, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText131 = new wxStaticText( itemPanel127, wxID_STATIC, _("Background"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText131->SetToolTip(_("When background timeline color is changed, new foreground complementary color is suggested."));
  itemBoxSizer130->Add(itemStaticText131, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerBackground = new wxColourPickerCtrl( itemPanel127, ID_COLOURPICKER_BACKGROUND, wxColour(0, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerBackground->SetToolTip(_("When background timeline color is changed, new foreground complementary color is suggested."));
  itemBoxSizer130->Add(colourPickerBackground, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer133 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer129->Add(itemBoxSizer133, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText134 = new wxStaticText( itemPanel127, wxID_STATIC, _("Foreground"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText134->SetToolTip(_("Color used to draw scale axis and object names in timelines."));
  itemBoxSizer133->Add(itemStaticText134, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerAxis = new wxColourPickerCtrl( itemPanel127, ID_COLOURPICKER_AXIS, wxColour(255, 255, 255), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerAxis->SetToolTip(_("Color used to draw scale axis and object names in timelines."));
  itemBoxSizer133->Add(colourPickerAxis, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer136 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer129->Add(itemBoxSizer136, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  checkZero = new wxCheckBox( itemPanel127, wxID_STATIC, _("Semantic zero"), wxDefaultPosition, wxDefaultSize, 0 );
  checkZero->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkZero->SetToolTip(_("Color used for semantic zero values"));
  itemBoxSizer136->Add(checkZero, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerZero = new wxColourPickerCtrl( itemPanel127, ID_COLOURPICKER_ZERO, wxColour(0, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerZero->SetToolTip(_("Color used for semantic zero values"));
  itemBoxSizer136->Add(colourPickerZero, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer139 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer129->Add(itemBoxSizer139, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText140 = new wxStaticText( itemPanel127, wxID_STATIC, _("Logical comms."), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText140->SetToolTip(_("Color used to draw logical communications in timelines."));
  itemBoxSizer139->Add(itemStaticText140, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerLogical = new wxColourPickerCtrl( itemPanel127, ID_COLOURPICKER_LOGICAL, wxColour(255, 255, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerLogical->SetToolTip(_("Color used to draw logical communications in timelines."));
  itemBoxSizer139->Add(colourPickerLogical, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer142 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer129->Add(itemBoxSizer142, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText143 = new wxStaticText( itemPanel127, wxID_STATIC, _("Physical comms."), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText143->SetToolTip(_("Color used to draw physical communications in timelines."));
  itemBoxSizer142->Add(itemStaticText143, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerPhysical = new wxColourPickerCtrl( itemPanel127, ID_COLOURPICKER_PHYSICAL, wxColour(255, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerPhysical->SetToolTip(_("Color used to draw physical communications in timelines."));
  itemBoxSizer142->Add(colourPickerPhysical, 2, wxALIGN_CENTER_VERTICAL|wxTOP, 5);

  wxButton* itemButton145 = new wxButton( itemPanel127, ID_BUTTON_DEFAULT_TIMELINE, _("Default"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemButton145->SetToolTip(_("Restores predefined colors."));
  itemStaticBoxSizer129->Add(itemButton145, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT, 5);

  wxStaticBox* itemStaticBoxSizer146Static = new wxStaticBox(itemPanel127, wxID_ANY, _("  Gradient  "));
  wxStaticBoxSizer* itemStaticBoxSizer146 = new wxStaticBoxSizer(itemStaticBoxSizer146Static, wxVERTICAL);
  itemBoxSizer128->Add(itemStaticBoxSizer146, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer147 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer146->Add(itemBoxSizer147, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText148 = new wxStaticText( itemPanel127, wxID_STATIC, _("Begin"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText148->SetToolTip(_("Base color used to represent semantic values when gradient is selected."));
  itemBoxSizer147->Add(itemStaticText148, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientBegin = new wxColourPickerCtrl( itemPanel127, ID_COLOURPICKER_GRADBEGIN, wxColour(0, 255, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientBegin->SetToolTip(_("Base color used to represent semantic values when gradient is selected."));
  itemBoxSizer147->Add(colourPickerGradientBegin, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer150 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer146->Add(itemBoxSizer150, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText151 = new wxStaticText( itemPanel127, wxID_STATIC, _("End"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText151->SetToolTip(_("Top color used to represent semantic values when gradient is selected."));
  itemBoxSizer150->Add(itemStaticText151, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientEnd = new wxColourPickerCtrl( itemPanel127, ID_COLOURPICKER_GRADEND, wxColour(0, 0, 255), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientEnd->SetToolTip(_("Top color used to represent semantic values when gradient is selected."));
  itemBoxSizer150->Add(colourPickerGradientEnd, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer153 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer146->Add(itemBoxSizer153, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText154 = new wxStaticText( itemPanel127, wxID_STATIC, _("Low outliers"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText154->SetToolTip(_("Any semantic outlier smaller than <Begin> will use this color."));
  itemBoxSizer153->Add(itemStaticText154, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientLow = new wxColourPickerCtrl( itemPanel127, ID_COLOURPICKER_GRADLOW, wxColour(207, 207, 68), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientLow->SetToolTip(_("Any semantic outlier smaller than <Begin> will use this color."));
  itemBoxSizer153->Add(colourPickerGradientLow, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer156 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer146->Add(itemBoxSizer156, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText157 = new wxStaticText( itemPanel127, wxID_STATIC, _("High outliers"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText157->SetToolTip(_("Any semantic outlier greater than <End> will use this color."));
  itemBoxSizer156->Add(itemStaticText157, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientTop = new wxColourPickerCtrl( itemPanel127, ID_COLOURPICKER_GRADTOP, wxColour(255, 146, 24), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientTop->SetToolTip(_("Any semantic outlier greater than <End> will use this color."));
  itemBoxSizer156->Add(colourPickerGradientTop, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxButton* itemButton159 = new wxButton( itemPanel127, ID_BUTTON_DEFAULT_GRADIENT, _("Default"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemButton159->SetToolTip(_("Restores predefined colors."));
  itemStaticBoxSizer146->Add(itemButton159, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT, 5);

  GetBookCtrl()->AddPage(itemPanel127, _("Color"));

  wxPanel* itemPanel160 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_FILTERS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  itemPanel160->Show(false);
  itemPanel160->Enable(false);

  GetBookCtrl()->AddPage(itemPanel160, _("Filters"));

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
  dirPickerTrace->SetPath( wxString::FromAscii( tracesPath.c_str() ) );
  dirPickerCFG->SetPath( wxString::FromAscii( cfgsPath.c_str() ) );
  dirPickerXML->SetPath( wxString::FromAscii( filtersXMLPath.c_str() ) );
  dirPickerTutorials->SetPath( wxString::FromAscii( tutorialsPath.c_str() ) );
  dirPickerTmp->SetPath( wxString::FromAscii( tmpPath.c_str() ) );
  txtMaximumTraceSize->SetValue( maximumTraceSize );
  checkGlobalSingleInstance->SetValue( singleInstance );
  spinSessionTime->SetValue( sessionSaveTime );

  // TIMELINE
  txtTimelineNameFormatPrefix->SetValue( wxString::FromAscii( timelineNameFormatPrefix.c_str() ) );
  txtTimelineNameFormatFull->SetValue( wxString::FromAscii( timelineNameFormatFull.c_str() ) );

  checkTimelineEventLines->SetValue( timelineEventLines );
  checkTimelineCommunicationLines->SetValue( timelineCommunicationLines );
  checkTimelineFunctionAsColor->SetValue( timelineFunctionAsColor );

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

  return true;
}

// Transfers data from all panels
bool PreferencesDialog::TransferDataFromWindow()
{
  vector< string > options;

  // GLOBAL
  globalFillStateGaps = checkGlobalFillStateGaps->IsChecked();
  tracesPath = std::string( dirPickerTrace->GetPath().mb_str() );
  cfgsPath = std::string( dirPickerCFG->GetPath().mb_str() );
  filtersXMLPath = std::string( dirPickerXML->GetPath().mb_str() );
  tutorialsPath = std::string( dirPickerTutorials->GetPath().mb_str() );
  tmpPath = std::string( dirPickerTmp->GetPath().mb_str() );
  maximumTraceSize = (float)txtMaximumTraceSize->GetValue();
  singleInstance = checkGlobalSingleInstance->GetValue();
  sessionSaveTime = spinSessionTime->GetValue();

  // TIMELINE
  timelineNameFormatPrefix = std::string( txtTimelineNameFormatPrefix->GetValue().mb_str() );
  timelineNameFormatFull = std::string( txtTimelineNameFormatFull->GetValue().mb_str() );
  // Parse format looking for modifiers %N -> number, %P -> prefix, %T -> trace

  timelineEventLines = checkTimelineEventLines->IsChecked();
  timelineCommunicationLines = checkTimelineCommunicationLines->IsChecked();
  timelineFunctionAsColor = checkTimelineFunctionAsColor->IsChecked();

  timelineColor = ( PRV_UINT32 )choiceTimelineColor->GetCurrentSelection();
  timelineGradientFunction = ( PRV_UINT32 )choiceTimelineGradientFunction->GetCurrentSelection();
  timelineDrawmodeTime = ( PRV_UINT32 )choiceTimelineDrawmodeTime->GetCurrentSelection();
  timelineDrawmodeObjects = ( PRV_UINT32 )choiceTimelineDrawmodeObjects->GetCurrentSelection();
  timelinePixelSize = ( PRV_UINT32 )choiceTimelinePixelSize->GetCurrentSelection();

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

