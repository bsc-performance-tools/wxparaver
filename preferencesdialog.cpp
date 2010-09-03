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
  histogramNumColumns = 20;
  histogramPrecision = 2;
  timelineWWPrecision = 2;
  histogramThousandSeparator = true;
  histogramShowUnits = true;
  histogramMaxNumColumns = 100000;
  histogramMaxPrecision = 10;
  whatWhereMaxPrecision = 10;
  globalFillStateGaps = false;
  tracesPath = "";
  cfgsPath = "";
  tmpPath = "";
  timelineNameFormatPrefix = "New Window # %N";
  timelineNameFormatFull = "%W @ %T";
  timelineEventLines = false;
  timelineCommunicationLines = true;
  timelineFunctionAsColor = true;
  timelineWWSemantic = true;
  timelineWWEvents = true;
  timelineWWCommunications = true;
  timelineWWPreviousNext = false;
  timelineWWText = true;
  timelineColor = 0;
  timelineGradientFunction = 0;
  timelineDrawmodeTime = 0;
  timelineDrawmodeObjects = 0;
  timelinePixelSize = 0;
  timelineSaveImageFormat = 0;
  timelineSaveTextFormat = 0;
  histogramNameFormatPrefix = "";
  histogramNameFormatFull = "";
  histogramZoom = false;
  histogramHideEmpty = false;
  histogramHorizontal = true;
  histogramShowGradient = true;
  histogramGradientFunction = 0;
  histogramDrawmodeObjects = 0;
  histogramScientificNotation = false;
  histogramAutofitControlScale = true;
  histogramAutofitDataGradient = true;
  histogramSaveImageFormat = 0;
  histogramSaveTextFormat = 0;
  histogramDrawmodeSemantic = true;
  histogramAutofit3DScale = true;
  histogramLabelsColor = false;
  colorUseZero = false;
  checkGlobalFillStateGaps = NULL;
  dirPickerTrace = NULL;
  dirPickerCFG = NULL;
  dirPickerTmp = NULL;
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
  itemStaticBoxSizer4->Add(itemBoxSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
  wxStaticText* itemStaticText7 = new wxStaticText( itemPanel2, wxID_STATIC, _("Maximum loadable trace size"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer6->Add(itemStaticText7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxTextCtrl* itemTextCtrl8 = new wxTextCtrl( itemPanel2, ID_TEXTCTRL_MAXIMUM_LOADABLE_TRACE_SIZE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer6->Add(itemTextCtrl8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

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
    dirPickerCFG->SetToolTip(_("Base path to configuration files (.cfg). "));
  itemBoxSizer13->Add(dirPickerCFG, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer9->Add(itemBoxSizer16, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText17 = new wxStaticText( itemPanel2, wxID_STATIC, _("Tmp"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticText17->Enable(false);
  itemBoxSizer16->Add(itemStaticText17, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  dirPickerTmp = new wxDirPickerCtrl( itemPanel2, ID_DIRPICKERCTRL_TMP, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_USE_TEXTCTRL );
  if (PreferencesDialog::ShowToolTips())
    dirPickerTmp->SetToolTip(_("Working directory for results and logs."));
  dirPickerTmp->Enable(false);
  itemBoxSizer16->Add(dirPickerTmp, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  GetBookCtrl()->AddPage(itemPanel2, _("Global"));

  wxPanel* itemPanel19 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_TIMELINE, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
  itemPanel19->SetSizer(itemBoxSizer20);

  wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer20->Add(itemBoxSizer21, 3, wxGROW|wxTOP, 5);
  wxStaticBox* itemStaticBoxSizer22Static = new wxStaticBox(itemPanel19, wxID_STATIC, _("  Name Format  "));
  wxStaticBoxSizer* itemStaticBoxSizer22 = new wxStaticBoxSizer(itemStaticBoxSizer22Static, wxVERTICAL);
  itemStaticBoxSizer22Static->Enable(false);
  itemBoxSizer21->Add(itemStaticBoxSizer22, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer22->Add(itemBoxSizer23, 0, wxGROW|wxTOP|wxBOTTOM, 5);
  wxStaticText* itemStaticText24 = new wxStaticText( itemPanel19, wxID_STATIC, _("Prefix"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText24->SetToolTip(_("Default name for new timelines, applied in window browser."));
  itemStaticText24->Enable(false);
  itemBoxSizer23->Add(itemStaticText24, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  txtTimelineNameFormatPrefix = new wxTextCtrl( itemPanel19, ID_PREFERENCES_TIMELINE_NAME_PREFIX, _("New Window # %N"), wxDefaultPosition, wxDefaultSize, 0 );
  txtTimelineNameFormatPrefix->SetMaxLength(256);
  if (PreferencesDialog::ShowToolTips())
    txtTimelineNameFormatPrefix->SetToolTip(_("Default name for new timelines, applied in window browser."));
  txtTimelineNameFormatPrefix->Enable(false);
  itemBoxSizer23->Add(txtTimelineNameFormatPrefix, 2, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer22->Add(itemBoxSizer26, 0, wxGROW|wxBOTTOM, 5);
  wxStaticText* itemStaticText27 = new wxStaticText( itemPanel19, wxID_STATIC, _("Full"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText27->SetToolTip(_("Default full name for new timelines, applied in window title."));
  itemStaticText27->Enable(false);
  itemBoxSizer26->Add(itemStaticText27, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  txtTimelineNameFormatFull = new wxTextCtrl( itemPanel19, ID_PREFERENCES_TIMELINE_NAME_FULL, _("%P @ %T"), wxDefaultPosition, wxDefaultSize, 0 );
  txtTimelineNameFormatFull->SetMaxLength(256);
  if (PreferencesDialog::ShowToolTips())
    txtTimelineNameFormatFull->SetToolTip(_("Default full name for new timelines, applied in window title."));
  txtTimelineNameFormatFull->Enable(false);
  itemBoxSizer26->Add(txtTimelineNameFormatFull, 2, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxStaticBox* itemStaticBoxSizer29Static = new wxStaticBox(itemPanel19, wxID_ANY, _("  Rendering Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer29 = new wxStaticBoxSizer(itemStaticBoxSizer29Static, wxVERTICAL);
  itemBoxSizer21->Add(itemStaticBoxSizer29, 1, wxGROW|wxALL, 5);
  checkTimelineCommunicationLines = new wxCheckBox( itemPanel19, ID_PREFERENCES_TIMELINE_COMMUNICATION_LINES, _("Communication Lines"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineCommunicationLines->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineCommunicationLines->SetToolTip(_("View communications lines in new timelines."));
  itemStaticBoxSizer29->Add(checkTimelineCommunicationLines, 0, wxGROW|wxTOP, 5);

  checkTimelineEventLines = new wxCheckBox( itemPanel19, ID_PREFERENCES_TIMELINE_EVENT_LINES, _("Event Flags"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineEventLines->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineEventLines->SetToolTip(_("View event flags in new timelines."));
  itemStaticBoxSizer29->Add(checkTimelineEventLines, 0, wxGROW, 5);

  checkTimelineFunctionAsColor = new wxCheckBox( itemPanel19, ID_PREFERENCES_TIMELINE_SEMANTIC_AS_COLOR, _("Semantic Values as Color"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineFunctionAsColor->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineFunctionAsColor->SetToolTip(_("Represent semantic values as function in new timelines."));
  itemStaticBoxSizer29->Add(checkTimelineFunctionAsColor, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer33 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer29->Add(itemBoxSizer33, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText34 = new wxStaticText( itemPanel19, wxID_STATIC, _("Color"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText34->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer33->Add(itemStaticText34, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceTimelineColorStrings;
  choiceTimelineColor = new wxChoice( itemPanel19, ID_PREFERENCES_TIMELINE_COLOR, wxDefaultPosition, wxDefaultSize, choiceTimelineColorStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineColor->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer33->Add(choiceTimelineColor, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer36 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer29->Add(itemBoxSizer36, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText37 = new wxStaticText( itemPanel19, wxID_STATIC, _("Gradient Function"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText37->SetToolTip(_("Gradient function selected in new timeline."));
  itemBoxSizer36->Add(itemStaticText37, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxArrayString choiceTimelineGradientFunctionStrings;
  choiceTimelineGradientFunction = new wxChoice( itemPanel19, ID_PREFERENCES_TIMELINE_GRADIENT, wxDefaultPosition, wxDefaultSize, choiceTimelineGradientFunctionStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineGradientFunction->SetToolTip(_("Gradient function selected in new timeline."));
  itemBoxSizer36->Add(choiceTimelineGradientFunction, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer39 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer29->Add(itemBoxSizer39, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText40 = new wxStaticText( itemPanel19, wxID_STATIC, _("Drawmode Time"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText40->SetToolTip(_("Drawmode for time axis selected in new timeline."));
  itemBoxSizer39->Add(itemStaticText40, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceTimelineDrawmodeTimeStrings;
  choiceTimelineDrawmodeTime = new wxChoice( itemPanel19, ID_PREFERENCES_TIMELINE_DRAWMODE_TIME, wxDefaultPosition, wxDefaultSize, choiceTimelineDrawmodeTimeStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineDrawmodeTime->SetToolTip(_("Drawmode for time axis selected in new timeline."));
  itemBoxSizer39->Add(choiceTimelineDrawmodeTime, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer42 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer29->Add(itemBoxSizer42, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText43 = new wxStaticText( itemPanel19, wxID_STATIC, _("Drawmode Objects"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText43->SetToolTip(_("Drawmode for objects axis selected in new timeline."));
  itemBoxSizer42->Add(itemStaticText43, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceTimelineDrawmodeObjectsStrings;
  choiceTimelineDrawmodeObjects = new wxChoice( itemPanel19, ID_PREFERENCES_TIMELINE_DRAWMODE_OBJECTS, wxDefaultPosition, wxDefaultSize, choiceTimelineDrawmodeObjectsStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineDrawmodeObjects->SetToolTip(_("Drawmode for objects axis selected in new timeline."));
  itemBoxSizer42->Add(choiceTimelineDrawmodeObjects, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer45 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer29->Add(itemBoxSizer45, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText46 = new wxStaticText( itemPanel19, wxID_STATIC, _("Pixel Size"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText46->SetToolTip(_("Pixel size selected in new timeline."));
  itemBoxSizer45->Add(itemStaticText46, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString choiceTimelinePixelSizeStrings;
  choiceTimelinePixelSize = new wxChoice( itemPanel19, ID_PREFERENCES_TIMELINE_PIXEL_SIZE, wxDefaultPosition, wxDefaultSize, choiceTimelinePixelSizeStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelinePixelSize->SetToolTip(_("Pixel size selected in new timeline."));
  itemBoxSizer45->Add(choiceTimelinePixelSize, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer48 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer20->Add(itemBoxSizer48, 2, wxGROW|wxTOP, 5);
  wxStaticBox* itemStaticBoxSizer49Static = new wxStaticBox(itemPanel19, wxID_ANY, _("  What / Where  "));
  wxStaticBoxSizer* itemStaticBoxSizer49 = new wxStaticBoxSizer(itemStaticBoxSizer49Static, wxVERTICAL);
  itemBoxSizer48->Add(itemStaticBoxSizer49, 2, wxGROW|wxALL, 5);
  checkTimelineWWSemantic = new wxCheckBox( itemPanel19, ID_CHECKBOX_TIMELINE_WW_SEMANTIC, _("Semantic"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWSemantic->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWSemantic->SetToolTip(_("Show semantic information when left click in timeline."));
  itemStaticBoxSizer49->Add(checkTimelineWWSemantic, 0, wxGROW|wxTOP, 5);

  checkTimelineWWEvents = new wxCheckBox( itemPanel19, ID_CHECKBOX_TIMELINE_WW_EVENTS, _("Events"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWEvents->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWEvents->SetToolTip(_("Show events information when left click in timeline."));
  itemStaticBoxSizer49->Add(checkTimelineWWEvents, 0, wxGROW, 5);

  checkTimelineWWCommunications = new wxCheckBox( itemPanel19, ID_CHECKBOX_TIMELINE_WW_COMMUNICATIONS, _("Communications"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWCommunications->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWCommunications->SetToolTip(_("Show communications information when left click in timeline."));
  itemStaticBoxSizer49->Add(checkTimelineWWCommunications, 0, wxGROW, 5);

  checkTimelineWWPreviousNext = new wxCheckBox( itemPanel19, ID_CHECKBOX_TIMELINE_WW_PREVIOUS_NEXT, _("Previous / Next"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWPreviousNext->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWPreviousNext->SetToolTip(_("In addition to current semantic value, show information for previous and next ones when left click in timeline."));
  itemStaticBoxSizer49->Add(checkTimelineWWPreviousNext, 0, wxGROW, 5);

  checkTimelineWWText = new wxCheckBox( itemPanel19, ID_CHECKBOX_TIMELINE_WW_TEXT, _("Text"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWText->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWText->SetToolTip(_("Show semantic value as text when left click in timeline."));
  itemStaticBoxSizer49->Add(checkTimelineWWText, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer55 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer49->Add(itemBoxSizer55, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText56 = new wxStaticText( itemPanel19, wxID_STATIC, _("Precision"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer55->Add(itemStaticText56, 2, wxALIGN_CENTER_VERTICAL, 5);

  txtTimelineWWPrecision = new wxSpinCtrl( itemPanel19, ID_PREFERENCES_TIMELINE_WW_PRECISION, _T("2"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 2 );
  if (PreferencesDialog::ShowToolTips())
    txtTimelineWWPrecision->SetToolTip(_("Decimal precision for semantic values."));
  itemBoxSizer55->Add(txtTimelineWWPrecision, 3, wxALIGN_CENTER_VERTICAL, 5);

  wxStaticBox* itemStaticBoxSizer58Static = new wxStaticBox(itemPanel19, wxID_ANY, _("  Save Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer58 = new wxStaticBoxSizer(itemStaticBoxSizer58Static, wxVERTICAL);
  itemBoxSizer48->Add(itemStaticBoxSizer58, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer59 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer58->Add(itemBoxSizer59, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxStaticText* itemStaticText60 = new wxStaticText( itemPanel19, wxID_STATIC, _("Image as"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText60->SetToolTip(_("Default image format selected."));
  itemBoxSizer59->Add(itemStaticText60, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceTimelineSaveImageFormatStrings;
  choiceTimelineSaveImageFormat = new wxChoice( itemPanel19, ID_PREFERENCES_TIMELINE_SAVE_AS_IMAGE, wxDefaultPosition, wxDefaultSize, choiceTimelineSaveImageFormatStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineSaveImageFormat->SetToolTip(_("Default image format selected."));
  itemBoxSizer59->Add(choiceTimelineSaveImageFormat, 3, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer62 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer58->Add(itemBoxSizer62, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxStaticText* itemStaticText63 = new wxStaticText( itemPanel19, wxID_STATIC, _("Text as"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText63->SetToolTip(_("Default text format selected."));
  itemStaticText63->Enable(false);
  itemBoxSizer62->Add(itemStaticText63, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceTimelineSaveTextFormatStrings;
  choiceTimelineSaveTextFormat = new wxChoice( itemPanel19, ID_PREFERENCES_TIMELINE_SAVE_AS_TEXT, wxDefaultPosition, wxDefaultSize, choiceTimelineSaveTextFormatStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineSaveTextFormat->SetToolTip(_("Default text format selected."));
  choiceTimelineSaveTextFormat->Enable(false);
  itemBoxSizer62->Add(choiceTimelineSaveTextFormat, 3, wxALIGN_CENTER_VERTICAL, 5);

  GetBookCtrl()->AddPage(itemPanel19, _("Timeline"));

  wxPanel* itemPanel65 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_HISTOGRAM, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  if (PreferencesDialog::ShowToolTips())
    itemPanel65->SetToolTip(_("Histogram preferences."));
  wxBoxSizer* itemBoxSizer66 = new wxBoxSizer(wxHORIZONTAL);
  itemPanel65->SetSizer(itemBoxSizer66);

  wxBoxSizer* itemBoxSizer67 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer66->Add(itemBoxSizer67, 3, wxGROW|wxTOP, 5);
  wxStaticBox* itemStaticBoxSizer68Static = new wxStaticBox(itemPanel65, wxID_STATIC, _("  Name Format  "));
  wxStaticBoxSizer* itemStaticBoxSizer68 = new wxStaticBoxSizer(itemStaticBoxSizer68Static, wxVERTICAL);
  itemStaticBoxSizer68Static->Enable(false);
  itemBoxSizer67->Add(itemStaticBoxSizer68, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer69 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer68->Add(itemBoxSizer69, 0, wxGROW|wxTOP|wxBOTTOM, 5);
  wxStaticText* itemStaticText70 = new wxStaticText( itemPanel65, wxID_STATIC, _("Prefix"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText70->SetToolTip(_("Default name for new histograms, applied in window browser."));
  itemStaticText70->Enable(false);
  itemBoxSizer69->Add(itemStaticText70, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  txtHistogramNameFormatPrefix = new wxTextCtrl( itemPanel65, ID_PREFERENCES_HISTOGRAM_NAME_PREFIX, _("New Histogram # %N"), wxDefaultPosition, wxDefaultSize, 0 );
  txtHistogramNameFormatPrefix->SetMaxLength(256);
  if (PreferencesDialog::ShowToolTips())
    txtHistogramNameFormatPrefix->SetToolTip(_("Default name for new histograms, applied in window browser."));
  txtHistogramNameFormatPrefix->Enable(false);
  itemBoxSizer69->Add(txtHistogramNameFormatPrefix, 2, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer72 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer68->Add(itemBoxSizer72, 0, wxGROW|wxBOTTOM, 5);
  wxStaticText* itemStaticText73 = new wxStaticText( itemPanel65, wxID_STATIC, _("Full"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText73->SetToolTip(_("Default full name for new histograms, applied in window title."));
  itemStaticText73->Enable(false);
  itemBoxSizer72->Add(itemStaticText73, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  txtHistogramNameFormatFull = new wxTextCtrl( itemPanel65, ID_PREFERENCES_HISTOGRAM_NAME_FULL, _("%P @ %T"), wxDefaultPosition, wxDefaultSize, 0 );
  txtHistogramNameFormatFull->SetMaxLength(256);
  if (PreferencesDialog::ShowToolTips())
    txtHistogramNameFormatFull->SetToolTip(_("Default full name for new histograms, applied in window title."));
  txtHistogramNameFormatFull->Enable(false);
  itemBoxSizer72->Add(txtHistogramNameFormatFull, 2, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxStaticBox* itemStaticBoxSizer75Static = new wxStaticBox(itemPanel65, wxID_ANY, _("  Matrix Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer75 = new wxStaticBoxSizer(itemStaticBoxSizer75Static, wxVERTICAL);
  itemBoxSizer67->Add(itemStaticBoxSizer75, 1, wxGROW|wxALL, 5);
  checkHistogramZoom = new wxCheckBox( itemPanel65, ID_PREFERENCES_HISTOGRAM_MATRIX_ZOOM, _("Zoom View as default"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramZoom->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramZoom->SetToolTip(_("Select between numeric and zoom view for the histogram matrix."));
  itemStaticBoxSizer75->Add(checkHistogramZoom, 0, wxGROW|wxTOP, 5);

  checkHistogramHorizontal = new wxCheckBox( itemPanel65, ID_PREFERENCES_HISTOGRAM_MATRIX_HORIZONTAL, _("Horizontal disposition"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramHorizontal->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramHorizontal->SetToolTip(_("Select between horizontal / vertical disposition for the histogram matrix."));
  itemStaticBoxSizer75->Add(checkHistogramHorizontal, 0, wxGROW, 5);

  checkHistogramHideEmpty = new wxCheckBox( itemPanel65, ID_PREFERENCES_HISTOGRAM_MATRIX_HIDE_EMPTY, _("Hide empty columns"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramHideEmpty->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramHideEmpty->SetToolTip(_("Hide columns when all its cells are empty."));
  itemStaticBoxSizer75->Add(checkHistogramHideEmpty, 0, wxGROW, 5);

  checkHistogramShowGradient = new wxCheckBox( itemPanel65, ID_PREFERENCES_HISTOGRAM_MATRIX_GRADIENT, _("Show Gradient Colors"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramShowGradient->SetValue(true);
  itemStaticBoxSizer75->Add(checkHistogramShowGradient, 0, wxGROW, 5);

  checkHistogramLabelsColor = new wxCheckBox( itemPanel65, ID_PREFERENCES_HISTOGRAM_MATRIX_LABELS_COLOR, _("Labels color"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramLabelsColor->SetValue(false);
  itemStaticBoxSizer75->Add(checkHistogramLabelsColor, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer81 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer75->Add(itemBoxSizer81, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText82 = new wxStaticText( itemPanel65, wxID_STATIC, _("Gradient Function"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText82->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer81->Add(itemStaticText82, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceHistogramGradientFunctionStrings;
  choiceHistogramGradientFunction = new wxChoice( itemPanel65, ID_PREFERENCES_HISTOGRAM_MATRIX_GRADIENT_FUNCTION, wxDefaultPosition, wxDefaultSize, choiceHistogramGradientFunctionStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceHistogramGradientFunction->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer81->Add(choiceHistogramGradientFunction, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer84 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer75->Add(itemBoxSizer84, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText85 = new wxStaticText( itemPanel65, wxID_STATIC, _("Drawmode Semantic"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText85->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer84->Add(itemStaticText85, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceHistogramDrawmodeSemanticStrings;
  choiceHistogramDrawmodeSemantic = new wxChoice( itemPanel65, ID_PREFERENCES_HISTOGRAM_MATRIX_DRAWMODE_SEMANTIC, wxDefaultPosition, wxDefaultSize, choiceHistogramDrawmodeSemanticStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceHistogramDrawmodeSemantic->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer84->Add(choiceHistogramDrawmodeSemantic, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer87 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer75->Add(itemBoxSizer87, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText88 = new wxStaticText( itemPanel65, wxID_STATIC, _("Drawmode Objects"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText88->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer87->Add(itemStaticText88, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxArrayString choiceHistogramDrawmodeObjectsStrings;
  choiceHistogramDrawmodeObjects = new wxChoice( itemPanel65, ID_PREFERENCES_HISTOGRAM_MATRIX_DRAWMODE_OBJECTS, wxDefaultPosition, wxDefaultSize, choiceHistogramDrawmodeObjectsStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceHistogramDrawmodeObjects->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer87->Add(choiceHistogramDrawmodeObjects, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer90 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer66->Add(itemBoxSizer90, 2, wxGROW|wxTOP, 5);
  wxStaticBox* itemStaticBoxSizer91Static = new wxStaticBox(itemPanel65, wxID_ANY, _("  Cell Format  "));
  wxStaticBoxSizer* itemStaticBoxSizer91 = new wxStaticBoxSizer(itemStaticBoxSizer91Static, wxVERTICAL);
  itemBoxSizer90->Add(itemStaticBoxSizer91, 0, wxGROW|wxALL, 5);
  checkHistogramScientificNotation = new wxCheckBox( itemPanel65, ID_PREFERENCES_HISTOGRAM_SCIENTIFIC_NOTATION, _("Scientific Notation"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramScientificNotation->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramScientificNotation->SetToolTip(_("Write cell content using scientific notation."));
  itemStaticBoxSizer91->Add(checkHistogramScientificNotation, 0, wxGROW|wxTOP, 5);

  checkHistogramThousandsSeparator = new wxCheckBox( itemPanel65, ID_PREFERENCES_HISTOGRAM_THOUSANDS_SEPARATOR, _("Thousands Separator"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramThousandsSeparator->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramThousandsSeparator->SetToolTip(_("Write cell content using thousands separator."));
  itemStaticBoxSizer91->Add(checkHistogramThousandsSeparator, 0, wxGROW, 5);

  checkHistogramShowUnits = new wxCheckBox( itemPanel65, ID_PREFERENCES_HISTOGRAM_SHOW_UNITS, _("Show Units"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramShowUnits->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramShowUnits->SetToolTip(_("End numeric information with its units."));
  itemStaticBoxSizer91->Add(checkHistogramShowUnits, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer95 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer91->Add(itemBoxSizer95, 0, wxGROW|wxBOTTOM, 5);
  wxStaticText* itemStaticText96 = new wxStaticText( itemPanel65, wxID_STATIC, _("Precision"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText96->SetToolTip(_("Decimal precision applied to the presentation of histogram metrics."));
  itemBoxSizer95->Add(itemStaticText96, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txtHistogramPrecision = new wxSpinCtrl( itemPanel65, ID_PREFERENCES_HISTOGRAM_PRECISION, _T("2"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 2 );
  if (PreferencesDialog::ShowToolTips())
    txtHistogramPrecision->SetToolTip(_("Decimal precision applied to the presentation of histogram metrics."));
  itemBoxSizer95->Add(txtHistogramPrecision, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer98Static = new wxStaticBox(itemPanel65, wxID_ANY, _("  Compute Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer98 = new wxStaticBoxSizer(itemStaticBoxSizer98Static, wxVERTICAL);
  itemBoxSizer90->Add(itemStaticBoxSizer98, 0, wxGROW|wxALL, 5);
  checkHistogramAutofitControlScale = new wxCheckBox( itemPanel65, ID_PREFERENCES_HISTOGRAM_AUTOFIT_CONTROL, _("Autofit Control Scale"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramAutofitControlScale->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramAutofitControlScale->SetToolTip(_("Fit control scale when histogram is created."));
  itemStaticBoxSizer98->Add(checkHistogramAutofitControlScale, 0, wxGROW|wxTOP, 5);

  checkHistogramAutofit3DScale = new wxCheckBox( itemPanel65, ID_PREFERENCES_HISTOGRAM_AUTOFIT_3D, _("Autofit 3D Scale"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramAutofit3DScale->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramAutofit3DScale->SetToolTip(_("Fit 3D scale when histogram is created."));
  itemStaticBoxSizer98->Add(checkHistogramAutofit3DScale, 0, wxGROW, 5);

  checkHistogramAutofitDataGradient = new wxCheckBox( itemPanel65, ID_PREFERENCES_HISTOGRAM_AUTOFIT_DATA_GRADIENT, _("Autofit Data Gradient"), wxDefaultPosition, wxDefaultSize, 0 );
  checkHistogramAutofitDataGradient->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkHistogramAutofitDataGradient->SetToolTip(_("Fit data gradient when histogram is created."));
  itemStaticBoxSizer98->Add(checkHistogramAutofitDataGradient, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer102 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer98->Add(itemBoxSizer102, 0, wxGROW|wxBOTTOM, 5);
  wxStaticText* itemStaticText103 = new wxStaticText( itemPanel65, wxID_STATIC, _("Columns"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText103->SetToolTip(_("Initial number of columns for new histograms."));
  itemBoxSizer102->Add(itemStaticText103, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txtHistogramNumColumns = new wxSpinCtrl( itemPanel65, ID_PREFERENCES_HISTOGRAM_NUMCOLUMNS, _T("20"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 256, 20 );
  if (PreferencesDialog::ShowToolTips())
    txtHistogramNumColumns->SetToolTip(_("Initial number of columns for new histograms."));
  itemBoxSizer102->Add(txtHistogramNumColumns, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer105Static = new wxStaticBox(itemPanel65, wxID_ANY, _("  Save Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer105 = new wxStaticBoxSizer(itemStaticBoxSizer105Static, wxVERTICAL);
  itemStaticBoxSizer105Static->Enable(false);
  itemBoxSizer90->Add(itemStaticBoxSizer105, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer106 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer105->Add(itemBoxSizer106, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxStaticText* itemStaticText107 = new wxStaticText( itemPanel65, wxID_STATIC, _("Image as"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText107->SetToolTip(_("Default image format selected."));
  itemStaticText107->Enable(false);
  itemBoxSizer106->Add(itemStaticText107, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceHistogramSaveImageFormatStrings;
  choiceHistogramSaveImageFormat = new wxChoice( itemPanel65, ID_PREFERENCES_HISTOGRAM_SAVE_IMAGE_FORMAT, wxDefaultPosition, wxDefaultSize, choiceHistogramSaveImageFormatStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceHistogramSaveImageFormat->SetToolTip(_("Default image format selected."));
  choiceHistogramSaveImageFormat->Enable(false);
  itemBoxSizer106->Add(choiceHistogramSaveImageFormat, 3, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer109 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer105->Add(itemBoxSizer109, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxStaticText* itemStaticText110 = new wxStaticText( itemPanel65, wxID_STATIC, _("Text as"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText110->SetToolTip(_("Default text format selected."));
  itemStaticText110->Enable(false);
  itemBoxSizer109->Add(itemStaticText110, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceHistogramSaveTextFormatStrings;
  choiceHistogramSaveTextFormat = new wxChoice( itemPanel65, ID_PREFERENCES_HISTOGRAM_SAVE_TXT_FORMAT, wxDefaultPosition, wxDefaultSize, choiceHistogramSaveTextFormatStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceHistogramSaveTextFormat->SetToolTip(_("Default text format selected."));
  choiceHistogramSaveTextFormat->Enable(false);
  itemBoxSizer109->Add(choiceHistogramSaveTextFormat, 3, wxALIGN_CENTER_VERTICAL, 5);

  GetBookCtrl()->AddPage(itemPanel65, _("Histogram"));

  wxPanel* itemPanel112 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_COLOR, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer113 = new wxBoxSizer(wxVERTICAL);
  itemPanel112->SetSizer(itemBoxSizer113);

  wxStaticBox* itemStaticBoxSizer114Static = new wxStaticBox(itemPanel112, wxID_ANY, _("  Timeline  "));
  wxStaticBoxSizer* itemStaticBoxSizer114 = new wxStaticBoxSizer(itemStaticBoxSizer114Static, wxVERTICAL);
  itemBoxSizer113->Add(itemStaticBoxSizer114, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer115 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer114->Add(itemBoxSizer115, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText116 = new wxStaticText( itemPanel112, wxID_STATIC, _("Background"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText116->SetToolTip(_("When background timeline color is changed, new foreground complementary color is suggested."));
  itemBoxSizer115->Add(itemStaticText116, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerBackground = new wxColourPickerCtrl( itemPanel112, ID_COLOURPICKER_BACKGROUND, wxColour(0, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerBackground->SetToolTip(_("When background timeline color is changed, new foreground complementary color is suggested."));
  itemBoxSizer115->Add(colourPickerBackground, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer118 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer114->Add(itemBoxSizer118, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText119 = new wxStaticText( itemPanel112, wxID_STATIC, _("Foreground"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText119->SetToolTip(_("Color used to draw scale axis and object names in timelines."));
  itemBoxSizer118->Add(itemStaticText119, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerAxis = new wxColourPickerCtrl( itemPanel112, ID_COLOURPICKER_AXIS, wxColour(255, 255, 255), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerAxis->SetToolTip(_("Color used to draw scale axis and object names in timelines."));
  itemBoxSizer118->Add(colourPickerAxis, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer121 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer114->Add(itemBoxSizer121, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  checkZero = new wxCheckBox( itemPanel112, wxID_STATIC, _("Semantic zero"), wxDefaultPosition, wxDefaultSize, 0 );
  checkZero->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkZero->SetToolTip(_("Color used for semantic zero values"));
  itemBoxSizer121->Add(checkZero, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerZero = new wxColourPickerCtrl( itemPanel112, ID_COLOURPICKER_ZERO, wxColour(0, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerZero->SetToolTip(_("Color used for semantic zero values"));
  itemBoxSizer121->Add(colourPickerZero, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer124 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer114->Add(itemBoxSizer124, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText125 = new wxStaticText( itemPanel112, wxID_STATIC, _("Logical comms."), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText125->SetToolTip(_("Color used to draw logical communications in timelines."));
  itemBoxSizer124->Add(itemStaticText125, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerLogical = new wxColourPickerCtrl( itemPanel112, ID_COLOURPICKER_LOGICAL, wxColour(255, 255, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerLogical->SetToolTip(_("Color used to draw logical communications in timelines."));
  itemBoxSizer124->Add(colourPickerLogical, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer127 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer114->Add(itemBoxSizer127, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText128 = new wxStaticText( itemPanel112, wxID_STATIC, _("Physical comms."), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText128->SetToolTip(_("Color used to draw physical communications in timelines."));
  itemBoxSizer127->Add(itemStaticText128, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerPhysical = new wxColourPickerCtrl( itemPanel112, ID_COLOURPICKER_PHYSICAL, wxColour(255, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerPhysical->SetToolTip(_("Color used to draw physical communications in timelines."));
  itemBoxSizer127->Add(colourPickerPhysical, 2, wxALIGN_CENTER_VERTICAL|wxTOP, 5);

  wxButton* itemButton130 = new wxButton( itemPanel112, ID_BUTTON_DEFAULT_TIMELINE, _("Default"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemButton130->SetToolTip(_("Restores predefined colors."));
  itemStaticBoxSizer114->Add(itemButton130, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT, 5);

  wxStaticBox* itemStaticBoxSizer131Static = new wxStaticBox(itemPanel112, wxID_ANY, _("  Gradient  "));
  wxStaticBoxSizer* itemStaticBoxSizer131 = new wxStaticBoxSizer(itemStaticBoxSizer131Static, wxVERTICAL);
  itemBoxSizer113->Add(itemStaticBoxSizer131, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer132 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer131->Add(itemBoxSizer132, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText133 = new wxStaticText( itemPanel112, wxID_STATIC, _("Begin"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText133->SetToolTip(_("Base color used to represent semantic values when gradient is selected."));
  itemBoxSizer132->Add(itemStaticText133, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientBegin = new wxColourPickerCtrl( itemPanel112, ID_COLOURPICKER_GRADBEGIN, wxColour(0, 255, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientBegin->SetToolTip(_("Base color used to represent semantic values when gradient is selected."));
  itemBoxSizer132->Add(colourPickerGradientBegin, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer135 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer131->Add(itemBoxSizer135, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText136 = new wxStaticText( itemPanel112, wxID_STATIC, _("End"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText136->SetToolTip(_("Top color used to represent semantic values when gradient is selected."));
  itemBoxSizer135->Add(itemStaticText136, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientEnd = new wxColourPickerCtrl( itemPanel112, ID_COLOURPICKER_GRADEND, wxColour(0, 0, 255), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientEnd->SetToolTip(_("Top color used to represent semantic values when gradient is selected."));
  itemBoxSizer135->Add(colourPickerGradientEnd, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer138 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer131->Add(itemBoxSizer138, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText139 = new wxStaticText( itemPanel112, wxID_STATIC, _("Low outliers"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText139->SetToolTip(_("Any semantic outlier smaller than <Begin> will use this color."));
  itemBoxSizer138->Add(itemStaticText139, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientLow = new wxColourPickerCtrl( itemPanel112, ID_COLOURPICKER_GRADLOW, wxColour(207, 207, 68), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientLow->SetToolTip(_("Any semantic outlier smaller than <Begin> will use this color."));
  itemBoxSizer138->Add(colourPickerGradientLow, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer141 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer131->Add(itemBoxSizer141, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText142 = new wxStaticText( itemPanel112, wxID_STATIC, _("High outliers"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText142->SetToolTip(_("Any semantic outlier greater than <End> will use this color."));
  itemBoxSizer141->Add(itemStaticText142, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientTop = new wxColourPickerCtrl( itemPanel112, ID_COLOURPICKER_GRADTOP, wxColour(255, 146, 24), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientTop->SetToolTip(_("Any semantic outlier greater than <End> will use this color."));
  itemBoxSizer141->Add(colourPickerGradientTop, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxButton* itemButton144 = new wxButton( itemPanel112, ID_BUTTON_DEFAULT_GRADIENT, _("Default"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemButton144->SetToolTip(_("Restores predefined colors."));
  itemStaticBoxSizer131->Add(itemButton144, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT, 5);

  GetBookCtrl()->AddPage(itemPanel112, _("Color"));

  wxPanel* itemPanel145 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_FILTERS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  itemPanel145->Show(false);
  itemPanel145->Enable(false);

  GetBookCtrl()->AddPage(itemPanel145, _("Filters"));

////@end PreferencesDialog content construction

  // Filter forbidden chars
  wxArrayString forbidden;
  forbidden.Add( _( "-" ) );
  forbidden.Add( _( "." ) );
  forbidden.Add( _( "," ) );

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
                                               const UINT32 &selected,
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
  dirPickerTmp->SetPath( wxString::FromAscii( tmpPath.c_str() ) );

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
  tmpPath = std::string( dirPickerTmp->GetPath().mb_str() );

  // TIMELINE
  timelineNameFormatPrefix = std::string( txtTimelineNameFormatPrefix->GetValue().mb_str() );
  timelineNameFormatFull = std::string( txtTimelineNameFormatFull->GetValue().mb_str() );
  // Parse format looking for modifiers %N -> number, %P -> prefix, %T -> trace

  timelineEventLines = checkTimelineEventLines->IsChecked();
  timelineCommunicationLines = checkTimelineCommunicationLines->IsChecked();
  timelineFunctionAsColor = checkTimelineFunctionAsColor->IsChecked();

  timelineColor = ( UINT32 )choiceTimelineColor->GetCurrentSelection();
  timelineGradientFunction = ( UINT32 )choiceTimelineGradientFunction->GetCurrentSelection();
  timelineDrawmodeTime = ( UINT32 )choiceTimelineDrawmodeTime->GetCurrentSelection();
  timelineDrawmodeObjects = ( UINT32 )choiceTimelineDrawmodeObjects->GetCurrentSelection();
  timelinePixelSize = ( UINT32 )choiceTimelinePixelSize->GetCurrentSelection();

  timelineWWSemantic = checkTimelineWWSemantic->IsChecked();
  timelineWWEvents = checkTimelineWWEvents->IsChecked();
  timelineWWCommunications = checkTimelineWWCommunications->IsChecked();
  timelineWWPreviousNext = checkTimelineWWPreviousNext->IsChecked();
  timelineWWText = checkTimelineWWText->IsChecked();
  timelineWWPrecision = ( UINT32 )txtTimelineWWPrecision->GetValue();

  timelineSaveImageFormat = ( UINT32 )choiceTimelineSaveImageFormat->GetCurrentSelection();
  timelineSaveTextFormat = ( UINT32 )choiceTimelineSaveTextFormat->GetCurrentSelection();

  // HISTOGRAM
  histogramNameFormatPrefix = std::string( txtHistogramNameFormatPrefix->GetValue().mb_str() );
  histogramNameFormatFull = std::string( txtHistogramNameFormatFull->GetValue().mb_str() );

  histogramZoom = checkHistogramZoom->GetValue();
  histogramHorizontal = checkHistogramHorizontal->GetValue();
  histogramHideEmpty = checkHistogramHideEmpty->GetValue();
  histogramShowGradient = checkHistogramShowGradient->GetValue();
  histogramLabelsColor = checkHistogramLabelsColor->GetValue();

  histogramGradientFunction = ( UINT32 )choiceHistogramGradientFunction->GetCurrentSelection();
  histogramDrawmodeSemantic = ( UINT32 )choiceHistogramDrawmodeSemantic->GetCurrentSelection();
  histogramDrawmodeObjects = ( UINT32 )choiceHistogramDrawmodeObjects->GetCurrentSelection();

  histogramScientificNotation = checkHistogramScientificNotation->GetValue();
  histogramThousandSeparator = checkHistogramThousandsSeparator->GetValue();
  histogramShowUnits = checkHistogramShowUnits->IsChecked();
  histogramPrecision = ( UINT32 )txtHistogramPrecision->GetValue();

  histogramAutofitControlScale = checkHistogramAutofitControlScale->GetValue();
  histogramAutofit3DScale = checkHistogramAutofit3DScale->GetValue();
  histogramAutofitDataGradient = checkHistogramAutofitDataGradient->GetValue();
  histogramNumColumns = (UINT32)txtHistogramNumColumns->GetValue();
  histogramNumColumns = ( UINT32 )txtHistogramNumColumns->GetValue();

  histogramSaveImageFormat = ( UINT32 )choiceHistogramSaveImageFormat->GetCurrentSelection();
  histogramSaveTextFormat = ( UINT32 )choiceHistogramSaveTextFormat->GetCurrentSelection();

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

  // FILTERS

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

