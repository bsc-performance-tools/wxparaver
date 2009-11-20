/////////////////////////////////////////////////////////////////////////////
// Name:        preferencesdialog.cpp
// Purpose:     
// Author:      Eloy Martinez
// Modified by: 
// Created:     Mon 28 Sep 2009 14:37:45 CEST
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

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
  txt2DNumColumns = NULL;
  txt2DPrecision = NULL;
  check2DOptions = NULL;
  colourPickerBackground = NULL;
  colourPickerAxis = NULL;
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

  wxStaticBox* itemStaticBoxSizer6Static = new wxStaticBox(itemPanel2, wxID_ANY, _("  Default directories  "));
  wxStaticBoxSizer* itemStaticBoxSizer6 = new wxStaticBoxSizer(itemStaticBoxSizer6Static, wxVERTICAL);
  itemBoxSizer3->Add(itemStaticBoxSizer6, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer6->Add(itemBoxSizer7, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText8 = new wxStaticText( itemPanel2, wxID_STATIC, _("Traces"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer7->Add(itemStaticText8, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  itemBoxSizer7->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  dirPickerTrace = new wxDirPickerCtrl( itemPanel2, ID_DIRPICKERCTRL_TRACES, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_USE_TEXTCTRL );
  if (PreferencesDialog::ShowToolTips())
    dirPickerTrace->SetToolTip(_("Base path to traces files (.prv, .prv.gz, .pcf and .row)."));
  itemBoxSizer7->Add(dirPickerTrace, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer6->Add(itemBoxSizer11, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText12 = new wxStaticText( itemPanel2, wxID_STATIC, _("CFGs"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer11->Add(itemStaticText12, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  itemBoxSizer11->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  dirPickerCFG = new wxDirPickerCtrl( itemPanel2, ID_DIRPICKERCTRL_CFG, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_USE_TEXTCTRL );
  dirPickerCFG->SetHelpText(_("Base path to configuration files (.cfg). "));
  if (PreferencesDialog::ShowToolTips())
    dirPickerCFG->SetToolTip(_("Base path to configuration files (.cfg). "));
  itemBoxSizer11->Add(dirPickerCFG, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer6->Add(itemBoxSizer15, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText16 = new wxStaticText( itemPanel2, wxID_STATIC, _("Tmp"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticText16->Enable(false);
  itemBoxSizer15->Add(itemStaticText16, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  itemBoxSizer15->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  dirPickerTmp = new wxDirPickerCtrl( itemPanel2, ID_DIRPICKERCTRL_TMP, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_USE_TEXTCTRL );
  if (PreferencesDialog::ShowToolTips())
    dirPickerTmp->SetToolTip(_("Working directory for results and logs."));
  dirPickerTmp->Enable(false);
  itemBoxSizer15->Add(dirPickerTmp, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  GetBookCtrl()->AddPage(itemPanel2, _("Global"));

  wxPanel* itemPanel19 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_TIMELINE, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
  itemPanel19->SetSizer(itemBoxSizer20);

  wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer20->Add(itemBoxSizer21, 3, wxGROW|wxALL, 5);
  wxStaticBox* itemStaticBoxSizer22Static = new wxStaticBox(itemPanel19, wxID_STATIC, _("  Name Format  "));
  wxStaticBoxSizer* itemStaticBoxSizer22 = new wxStaticBoxSizer(itemStaticBoxSizer22Static, wxVERTICAL);
  itemBoxSizer21->Add(itemStaticBoxSizer22, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer22->Add(itemBoxSizer23, 0, wxGROW|wxTOP|wxBOTTOM, 5);
  wxStaticText* itemStaticText24 = new wxStaticText( itemPanel19, wxID_STATIC, _("Prefix"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText24->SetToolTip(_("Default name for new timelines, applied in window browser."));
  itemStaticText24->Enable(false);
  itemBoxSizer23->Add(itemStaticText24, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  itemBoxSizer23->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  txtTimelineNameFormatPrefix = new wxTextCtrl( itemPanel19, ID_PREFERENCES_TIMELINE_NAME_PREFIX, _("New Window # %N"), wxDefaultPosition, wxDefaultSize, 0 );
  txtTimelineNameFormatPrefix->SetMaxLength(256);
  if (PreferencesDialog::ShowToolTips())
    txtTimelineNameFormatPrefix->SetToolTip(_("Default name for new timelines, applied in window browser."));
  txtTimelineNameFormatPrefix->Enable(false);
  itemBoxSizer23->Add(txtTimelineNameFormatPrefix, 2, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer27 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer22->Add(itemBoxSizer27, 0, wxGROW|wxBOTTOM, 5);
  wxStaticText* itemStaticText28 = new wxStaticText( itemPanel19, wxID_STATIC, _("Full"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText28->SetToolTip(_("Default full name for new timelines, applied in window title."));
  itemStaticText28->Enable(false);
  itemBoxSizer27->Add(itemStaticText28, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  itemBoxSizer27->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  txtTimelineNameFormatFull = new wxTextCtrl( itemPanel19, ID_PREFERENCES_TIMELINE_NAME_FULL, _("%P @ %T"), wxDefaultPosition, wxDefaultSize, 0 );
  txtTimelineNameFormatFull->SetMaxLength(256);
  if (PreferencesDialog::ShowToolTips())
    txtTimelineNameFormatFull->SetToolTip(_("Default full name for new timelines, applied in window title."));
  txtTimelineNameFormatFull->Enable(false);
  itemBoxSizer27->Add(txtTimelineNameFormatFull, 2, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxStaticBox* itemStaticBoxSizer31Static = new wxStaticBox(itemPanel19, wxID_ANY, _("  Rendering Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer31 = new wxStaticBoxSizer(itemStaticBoxSizer31Static, wxVERTICAL);
  itemBoxSizer21->Add(itemStaticBoxSizer31, 1, wxGROW|wxALL, 5);
  checkTimelineCommunicationLines = new wxCheckBox( itemPanel19, ID_PREFERENCES_TIMELINE_COMMUNICATION_LINES, _("Communication Lines"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineCommunicationLines->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineCommunicationLines->SetToolTip(_("View communications lines in new timelines."));
  itemStaticBoxSizer31->Add(checkTimelineCommunicationLines, 0, wxGROW|wxTOP, 5);

  checkTimelineEventLines = new wxCheckBox( itemPanel19, ID_PREFERENCES_TIMELINE_EVENT_LINES, _("Event Flags"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineEventLines->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineEventLines->SetToolTip(_("View event flags in new timelines."));
  itemStaticBoxSizer31->Add(checkTimelineEventLines, 0, wxGROW, 5);

  checkTimelineFunctionAsColor = new wxCheckBox( itemPanel19, ID_PREFERENCES_TIMELINE_SEMANTIC_AS_COLOR, _("Semantic Values as Color"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineFunctionAsColor->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineFunctionAsColor->SetToolTip(_("Represent semantic values as function in new timelines."));
  itemStaticBoxSizer31->Add(checkTimelineFunctionAsColor, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer35 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer31->Add(itemBoxSizer35, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText36 = new wxStaticText( itemPanel19, wxID_STATIC, _("Color"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText36->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer35->Add(itemStaticText36, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  itemBoxSizer35->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceTimelineColorStrings;
  choiceTimelineColor = new wxChoice( itemPanel19, ID_PREFERENCES_TIMELINE_COLOR, wxDefaultPosition, wxDefaultSize, choiceTimelineColorStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineColor->SetToolTip(_("Color mode selected in new timeline."));
  itemBoxSizer35->Add(choiceTimelineColor, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer39 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer31->Add(itemBoxSizer39, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText40 = new wxStaticText( itemPanel19, wxID_STATIC, _("Gradient is"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText40->SetToolTip(_("Gradient function selected in new timeline."));
  itemBoxSizer39->Add(itemStaticText40, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  itemBoxSizer39->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceTimelineGradientFunctionStrings;
  choiceTimelineGradientFunction = new wxChoice( itemPanel19, ID_PREFERENCES_TIMELINE_GRADIENT, wxDefaultPosition, wxDefaultSize, choiceTimelineGradientFunctionStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineGradientFunction->SetToolTip(_("Gradient function selected in new timeline."));
  itemBoxSizer39->Add(choiceTimelineGradientFunction, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer43 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer31->Add(itemBoxSizer43, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText44 = new wxStaticText( itemPanel19, wxID_STATIC, _("Drawmode Time"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText44->SetToolTip(_("Drawmode for time axis selected in new timeline."));
  itemBoxSizer43->Add(itemStaticText44, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  itemBoxSizer43->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceTimelineDrawmodeTimeStrings;
  choiceTimelineDrawmodeTime = new wxChoice( itemPanel19, ID_PREFERENCES_TIMELINE_DRAWMODE_TIME, wxDefaultPosition, wxDefaultSize, choiceTimelineDrawmodeTimeStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineDrawmodeTime->SetToolTip(_("Drawmode for time axis selected in new timeline."));
  itemBoxSizer43->Add(choiceTimelineDrawmodeTime, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer47 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer31->Add(itemBoxSizer47, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText48 = new wxStaticText( itemPanel19, wxID_STATIC, _("Drawmode Objects"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText48->SetToolTip(_("Drawmode for objects axis selected in new timeline."));
  itemBoxSizer47->Add(itemStaticText48, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  itemBoxSizer47->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceTimelineDrawmodeObjectsStrings;
  choiceTimelineDrawmodeObjects = new wxChoice( itemPanel19, ID_PREFERENCES_TIMELINE_DRAWMODE_OBJECTS, wxDefaultPosition, wxDefaultSize, choiceTimelineDrawmodeObjectsStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineDrawmodeObjects->SetToolTip(_("Drawmode for objects axis selected in new timeline."));
  itemBoxSizer47->Add(choiceTimelineDrawmodeObjects, 1, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer51 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer31->Add(itemBoxSizer51, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText52 = new wxStaticText( itemPanel19, wxID_STATIC, _("Pixel Size"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText52->SetToolTip(_("Pixel size selected in new timeline."));
  itemBoxSizer51->Add(itemStaticText52, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  itemBoxSizer51->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceTimelinePixelSizeStrings;
  choiceTimelinePixelSize = new wxChoice( itemPanel19, ID_PREFERENCES_TIMELINE_PIXEL_SIZE, wxDefaultPosition, wxDefaultSize, choiceTimelinePixelSizeStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelinePixelSize->SetToolTip(_("Pixel size selected in new timeline."));
  itemBoxSizer51->Add(choiceTimelinePixelSize, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer55 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer20->Add(itemBoxSizer55, 2, wxGROW, 5);
  wxStaticBox* itemStaticBoxSizer56Static = new wxStaticBox(itemPanel19, wxID_ANY, _("  What / Where  "));
  wxStaticBoxSizer* itemStaticBoxSizer56 = new wxStaticBoxSizer(itemStaticBoxSizer56Static, wxVERTICAL);
  itemBoxSizer55->Add(itemStaticBoxSizer56, 2, wxGROW|wxALL, 5);
  checkTimelineWWSemantic = new wxCheckBox( itemPanel19, ID_CHECKBOX_TIMELINE_WW_SEMANTIC, _("Semantic"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWSemantic->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWSemantic->SetToolTip(_("Show semantic information when left click in timeline."));
  itemStaticBoxSizer56->Add(checkTimelineWWSemantic, 0, wxGROW|wxTOP, 5);

  checkTimelineWWEvents = new wxCheckBox( itemPanel19, ID_CHECKBOX_TIMELINE_WW_EVENTS, _("Events"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWEvents->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWEvents->SetToolTip(_("Show events information when left click in timeline."));
  itemStaticBoxSizer56->Add(checkTimelineWWEvents, 0, wxGROW, 5);

  checkTimelineWWCommunications = new wxCheckBox( itemPanel19, ID_CHECKBOX_TIMELINE_WW_COMMUNICATIONS, _("Communications"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWCommunications->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWCommunications->SetToolTip(_("Show communications information when left click in timeline."));
  itemStaticBoxSizer56->Add(checkTimelineWWCommunications, 0, wxGROW, 5);

  checkTimelineWWPreviousNext = new wxCheckBox( itemPanel19, ID_CHECKBOX_TIMELINE_WW_PREVIOUS_NEXT, _("Previous / Next"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWPreviousNext->SetValue(false);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWPreviousNext->SetToolTip(_("In addition to current semantic value, show information for previous and next ones when left click in timeline."));
  itemStaticBoxSizer56->Add(checkTimelineWWPreviousNext, 0, wxGROW, 5);

  checkTimelineWWText = new wxCheckBox( itemPanel19, ID_CHECKBOX_TIMELINE_WW_TEXT, _("Text"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWText->SetValue(true);
  if (PreferencesDialog::ShowToolTips())
    checkTimelineWWText->SetToolTip(_("Show semantic value as text when left click in timeline."));
  itemStaticBoxSizer56->Add(checkTimelineWWText, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer62 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer56->Add(itemBoxSizer62, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText63 = new wxStaticText( itemPanel19, wxID_STATIC, _("Precision"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer62->Add(itemStaticText63, 2, wxALIGN_CENTER_VERTICAL, 5);

  itemBoxSizer62->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxSHAPED, 5);

  txtTimelineWWPrecision = new wxSpinCtrl( itemPanel19, ID_PREFERENCES_TIMELINE_WW_PRECISION, _T("2"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 2 );
  if (PreferencesDialog::ShowToolTips())
    txtTimelineWWPrecision->SetToolTip(_("Decimal precision for semantic values."));
  itemBoxSizer62->Add(txtTimelineWWPrecision, 3, wxALIGN_CENTER_VERTICAL, 5);

  wxStaticBox* itemStaticBoxSizer66Static = new wxStaticBox(itemPanel19, wxID_ANY, _("  Save Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer66 = new wxStaticBoxSizer(itemStaticBoxSizer66Static, wxVERTICAL);
  itemBoxSizer55->Add(itemStaticBoxSizer66, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer67 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer66->Add(itemBoxSizer67, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxStaticText* itemStaticText68 = new wxStaticText( itemPanel19, wxID_STATIC, _("Image as"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText68->SetToolTip(_("Default image format selected."));
  itemBoxSizer67->Add(itemStaticText68, 2, wxALIGN_CENTER_VERTICAL, 5);

  itemBoxSizer67->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceTimelineSaveImageFormatStrings;
  choiceTimelineSaveImageFormat = new wxChoice( itemPanel19, ID_PREFERENCES_TIMELINE_SAVE_AS_IMAGE, wxDefaultPosition, wxDefaultSize, choiceTimelineSaveImageFormatStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineSaveImageFormat->SetToolTip(_("Default image format selected."));
  itemBoxSizer67->Add(choiceTimelineSaveImageFormat, 3, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer71 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer66->Add(itemBoxSizer71, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
  wxStaticText* itemStaticText72 = new wxStaticText( itemPanel19, wxID_STATIC, _("Text as"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText72->SetToolTip(_("Default text format selected."));
  itemStaticText72->Enable(false);
  itemBoxSizer71->Add(itemStaticText72, 2, wxALIGN_CENTER_VERTICAL, 5);

  itemBoxSizer71->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  wxArrayString choiceTimelineSaveTextFormatStrings;
  choiceTimelineSaveTextFormat = new wxChoice( itemPanel19, ID_PREFERENCES_TIMELINE_SAVE_AS_TEXT, wxDefaultPosition, wxDefaultSize, choiceTimelineSaveTextFormatStrings, 0 );
  if (PreferencesDialog::ShowToolTips())
    choiceTimelineSaveTextFormat->SetToolTip(_("Default text format selected."));
  choiceTimelineSaveTextFormat->Enable(false);
  itemBoxSizer71->Add(choiceTimelineSaveTextFormat, 3, wxALIGN_CENTER_VERTICAL, 5);

  GetBookCtrl()->AddPage(itemPanel19, _("Timeline"));

  wxPanel* itemPanel75 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_HISTOGRAM, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  if (PreferencesDialog::ShowToolTips())
    itemPanel75->SetToolTip(_("Histogram preferences."));
  wxBoxSizer* itemBoxSizer76 = new wxBoxSizer(wxVERTICAL);
  itemPanel75->SetSizer(itemBoxSizer76);

  wxBoxSizer* itemBoxSizer77 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer76->Add(itemBoxSizer77, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText78 = new wxStaticText( itemPanel75, wxID_STATIC, _("Columns"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText78->SetToolTip(_("Initial number of columns for new histograms."));
  itemBoxSizer77->Add(itemStaticText78, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  itemBoxSizer77->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  txt2DNumColumns = new wxSpinCtrl( itemPanel75, ID_PREFERENCES_2D_NUMCOLUMNS, _T("20"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 256, 20 );
  if (PreferencesDialog::ShowToolTips())
    txt2DNumColumns->SetToolTip(_("Initial number of columns for new histograms."));
  itemBoxSizer77->Add(txt2DNumColumns, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer81 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer76->Add(itemBoxSizer81, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText82 = new wxStaticText( itemPanel75, wxID_STATIC, _("Precision"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText82->SetToolTip(_("Decimal precision applied to the presentation of histogram metrics."));
  itemBoxSizer81->Add(itemStaticText82, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  itemBoxSizer81->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  txt2DPrecision = new wxSpinCtrl( itemPanel75, ID_PREFERENCES_2D_PRECISION, _T("2"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 2 );
  if (PreferencesDialog::ShowToolTips())
    txt2DPrecision->SetToolTip(_("Decimal precision applied to the presentation of histogram metrics."));
  itemBoxSizer81->Add(txt2DPrecision, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString check2DOptionsStrings;
  check2DOptionsStrings.Add(_("Show Time Units"));
  check2DOptionsStrings.Add(_("Thousands Separator"));
  check2DOptions = new wxCheckListBox( itemPanel75, ID_PREFERENCES_2D_CHECKOPTIONS, wxDefaultPosition, wxDefaultSize, check2DOptionsStrings, wxLB_SINGLE );
  if (PreferencesDialog::ShowToolTips())
    check2DOptions->SetToolTip(_("Properties applied to the presentation of histogram metrics."));
  itemBoxSizer76->Add(check2DOptions, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  GetBookCtrl()->AddPage(itemPanel75, _("Histogram"));

  wxPanel* itemPanel86 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_COLOR, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer87 = new wxBoxSizer(wxVERTICAL);
  itemPanel86->SetSizer(itemBoxSizer87);

  wxStaticBox* itemStaticBoxSizer88Static = new wxStaticBox(itemPanel86, wxID_ANY, _("  Timeline  "));
  wxStaticBoxSizer* itemStaticBoxSizer88 = new wxStaticBoxSizer(itemStaticBoxSizer88Static, wxVERTICAL);
  itemBoxSizer87->Add(itemStaticBoxSizer88, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer89 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer88->Add(itemBoxSizer89, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText90 = new wxStaticText( itemPanel86, wxID_STATIC, _("Background"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText90->SetToolTip(_("When background timeline color is changed, new foreground complementary color is suggested."));
  itemBoxSizer89->Add(itemStaticText90, 1, wxALIGN_CENTER_VERTICAL, 5);

  itemBoxSizer89->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerBackground = new wxColourPickerCtrl( itemPanel86, ID_COLOURPICKER_BACKGROUND, wxColour(0, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerBackground->SetToolTip(_("When background timeline color is changed, new foreground complementary color is suggested."));
  itemBoxSizer89->Add(colourPickerBackground, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer93 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer88->Add(itemBoxSizer93, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText94 = new wxStaticText( itemPanel86, wxID_STATIC, _("Foreground"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText94->SetToolTip(_("Color used to draw scale axis and object names in timelines."));
  itemBoxSizer93->Add(itemStaticText94, 1, wxALIGN_CENTER_VERTICAL, 5);

  itemBoxSizer93->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerAxis = new wxColourPickerCtrl( itemPanel86, ID_COLOURPICKER_AXIS, wxColour(255, 255, 255), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerAxis->SetToolTip(_("Color used to draw scale axis and object names in timelines."));
  itemBoxSizer93->Add(colourPickerAxis, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer97 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer88->Add(itemBoxSizer97, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText98 = new wxStaticText( itemPanel86, wxID_STATIC, _("Logical comms."), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText98->SetToolTip(_("Color used to draw logical communications in timelines."));
  itemBoxSizer97->Add(itemStaticText98, 1, wxALIGN_CENTER_VERTICAL, 5);

  itemBoxSizer97->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerLogical = new wxColourPickerCtrl( itemPanel86, ID_COLOURPICKER_LOGICAL, wxColour(255, 255, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerLogical->SetToolTip(_("Color used to draw logical communications in timelines."));
  itemBoxSizer97->Add(colourPickerLogical, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer101 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer88->Add(itemBoxSizer101, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText102 = new wxStaticText( itemPanel86, wxID_STATIC, _("Physical comms."), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText102->SetToolTip(_("Color used to draw physical communications in timelines."));
  itemBoxSizer101->Add(itemStaticText102, 1, wxALIGN_CENTER_VERTICAL, 5);

  itemBoxSizer101->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerPhysical = new wxColourPickerCtrl( itemPanel86, ID_COLOURPICKER_PHYSICAL, wxColour(255, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerPhysical->SetToolTip(_("Color used to draw physical communications in timelines."));
  itemBoxSizer101->Add(colourPickerPhysical, 2, wxALIGN_CENTER_VERTICAL|wxTOP, 5);

  wxButton* itemButton105 = new wxButton( itemPanel86, ID_BUTTON_DEFAULT_TIMELINE, _("Default"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemButton105->SetToolTip(_("Restores predefined colors."));
  itemStaticBoxSizer88->Add(itemButton105, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT, 5);

  wxStaticBox* itemStaticBoxSizer106Static = new wxStaticBox(itemPanel86, wxID_ANY, _("  Gradient  "));
  wxStaticBoxSizer* itemStaticBoxSizer106 = new wxStaticBoxSizer(itemStaticBoxSizer106Static, wxVERTICAL);
  itemBoxSizer87->Add(itemStaticBoxSizer106, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer107 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer106->Add(itemBoxSizer107, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText108 = new wxStaticText( itemPanel86, wxID_STATIC, _("Begin"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText108->SetToolTip(_("Base color used to represent semantic values when gradient is selected."));
  itemBoxSizer107->Add(itemStaticText108, 1, wxALIGN_CENTER_VERTICAL, 5);

  itemBoxSizer107->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientBegin = new wxColourPickerCtrl( itemPanel86, ID_COLOURPICKER_GRADBEGIN, wxColour(0, 255, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientBegin->SetToolTip(_("Base color used to represent semantic values when gradient is selected."));
  itemBoxSizer107->Add(colourPickerGradientBegin, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer111 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer106->Add(itemBoxSizer111, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText112 = new wxStaticText( itemPanel86, wxID_STATIC, _("End"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText112->SetToolTip(_("Top color used to represent semantic values when gradient is selected."));
  itemBoxSizer111->Add(itemStaticText112, 1, wxALIGN_CENTER_VERTICAL, 5);

  itemBoxSizer111->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientEnd = new wxColourPickerCtrl( itemPanel86, ID_COLOURPICKER_GRADEND, wxColour(0, 0, 255), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientEnd->SetToolTip(_("Top color used to represent semantic values when gradient is selected."));
  itemBoxSizer111->Add(colourPickerGradientEnd, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer115 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer106->Add(itemBoxSizer115, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText116 = new wxStaticText( itemPanel86, wxID_STATIC, _("Low outliers"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText116->SetToolTip(_("Any semantic outlier smaller than <Begin> will use this color."));
  itemBoxSizer115->Add(itemStaticText116, 1, wxALIGN_CENTER_VERTICAL, 5);

  itemBoxSizer115->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientLow = new wxColourPickerCtrl( itemPanel86, ID_COLOURPICKER_GRADLOW, wxColour(207, 207, 68), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientLow->SetToolTip(_("Any semantic outlier smaller than <Begin> will use this color."));
  itemBoxSizer115->Add(colourPickerGradientLow, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer119 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer106->Add(itemBoxSizer119, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText120 = new wxStaticText( itemPanel86, wxID_STATIC, _("High outliers"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText120->SetToolTip(_("Any semantic outlier greater than <End> will use this color."));
  itemBoxSizer119->Add(itemStaticText120, 1, wxALIGN_CENTER_VERTICAL, 5);

  itemBoxSizer119->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientTop = new wxColourPickerCtrl( itemPanel86, ID_COLOURPICKER_GRADTOP, wxColour(255, 146, 24), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientTop->SetToolTip(_("Any semantic outlier greater than <End> will use this color."));
  itemBoxSizer119->Add(colourPickerGradientTop, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxButton* itemButton123 = new wxButton( itemPanel86, ID_BUTTON_DEFAULT_GRADIENT, _("Default"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemButton123->SetToolTip(_("Restores predefined colors."));
  itemStaticBoxSizer106->Add(itemButton123, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT, 5);

  GetBookCtrl()->AddPage(itemPanel86, _("Color"));

  wxPanel* itemPanel124 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_FILTERS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  itemPanel124->Show(false);
  itemPanel124->Enable(false);

  GetBookCtrl()->AddPage(itemPanel124, _("Filters"));

////@end PreferencesDialog content construction

  // Filter forbidden chars
  wxArrayString forbidden;
  forbidden.Add("-");
  forbidden.Add(".");
  forbidden.Add(",");

/*
// wxTextCtrl validators

  wxTextValidator validator( wxFILTER_NUMERIC | wxFILTER_EXCLUDE_CHAR_LIST );
  validator.SetExcludes( forbidden );

  txt2DNumColumns->SetValidator( validator );
  txt2DPrecision->SetValidator( validator );
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
  auxNumber << auxSStr.str();

  return auxNumber;
}


void PreferencesDialog::setLabelsChoiceBox( const vector< string > &list,
                                               const UINT32 &selected,
                                               wxChoice *choiceBox )
{
  wxString aux;
  choiceBox->Clear(); // entra dos veces!!!

  for( vector< string >::const_iterator it = list.begin(); it != list.end(); ++it )
  {
    // add every string of the list to the choice box
    aux << *it;
    choiceBox->Append( aux );
    aux.clear();
  }

  choiceBox->Select( selected );
}

// Transfers data to all panels
bool PreferencesDialog::TransferDataToWindow()
{
  vector< string > options;

  // GLOBAL
  checkGlobalFillStateGaps->SetValue( globalFillStateGaps );
  wxString tmpStr;
  tmpStr << tracesPath;
  dirPickerTrace->SetPath( tmpStr );
  tmpStr.Clear();
  tmpStr << cfgsPath;
  dirPickerCFG->SetPath( tmpStr );
  tmpStr.Clear();
  tmpStr << tmpPath;
  dirPickerTmp->SetPath( tmpStr );

  // TIMELINE
  txtTimelineNameFormatPrefix->SetValue( timelineNameFormatPrefix );
  txtTimelineNameFormatFull->SetValue( timelineNameFormatFull );

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
  txt2DPrecision->SetValue( ( int )histogramPrecision );
  txt2DNumColumns->SetValue( ( int )histogramNumColumns );

  check2DOptions->Check( ITEM_SHOW_UNITS, histogramShowUnits );
  check2DOptions->Check( ITEM_THOUSAND_SEPARATOR, histogramThousandSeparator );

  // COLORS
  colourPickerBackground->SetColour( RGBTowxColour( timelineColourBackground ) );
  colourPickerAxis->SetColour( RGBTowxColour( timelineColourAxis ) );
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
  tracesPath = dirPickerTrace->GetPath();
  cfgsPath = dirPickerCFG->GetPath();
  tmpPath = dirPickerTmp->GetPath();

  // TIMELINE
  timelineNameFormatPrefix = txtTimelineNameFormatPrefix->GetValue();
  timelineNameFormatFull = txtTimelineNameFormatFull->GetValue();
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
  histogramPrecision = ( UINT32 )txt2DPrecision->GetValue();
  histogramNumColumns = ( UINT32 )txt2DNumColumns->GetValue();

  histogramShowUnits = check2DOptions->IsChecked( ITEM_SHOW_UNITS );
  histogramThousandSeparator = check2DOptions->IsChecked( ITEM_THOUSAND_SEPARATOR );

  txtTimelineNameFormatFull->SetValue( timelineNameFormatFull );
  timelineWWPrecision = ( UINT32 )txtTimelineWWPrecision->GetValue();

  // COLORS
  timelineColourBackground = wxColourToRGB( colourPickerBackground->GetColour() );
  timelineColourAxis       = wxColourToRGB( colourPickerAxis->GetColour() );
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
  colourPickerGradientLow->SetColour( RGBTowxColour( SemanticColor::DEFAULT_ABOVE_OUTLIER_COLOR ) );
  colourPickerGradientTop->SetColour( RGBTowxColour( SemanticColor::DEFAULT_BELOW_OUTLIER_COLOR ) );
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

