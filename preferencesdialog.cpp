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
  whatWherePrecision = 2;
  histogramThousandSeparator = true;
  histogramShowUnits = true;
  histogramMaxNumColumns = 100000;
  histogramMaxPrecision = 10;
  whatWhereMaxPrecision = 10;
  globalFillStateGaps = false;
  tracesPath = "";
  cfgsPath = "";
  tmpPath = "";
  checkGlobalFillStateGaps = NULL;
  dirPickerTrace = NULL;
  dirPickerCFG = NULL;
  dirPickerTmp = NULL;
  checkTimelineEventLines = NULL;
  checkTimelineCommunicationLines = NULL;
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
  txtWhatWherePrecision = NULL;
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

  itemBoxSizer7->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  dirPickerTrace = new wxDirPickerCtrl( itemPanel2, ID_DIRPICKERCTRL_TRACES, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_USE_TEXTCTRL );
  if (PreferencesDialog::ShowToolTips())
    dirPickerTrace->SetToolTip(_("Base path to traces files (.prv, .prv.gz, .pcf and .row)."));
  itemBoxSizer7->Add(dirPickerTrace, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer6->Add(itemBoxSizer11, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText12 = new wxStaticText( itemPanel2, wxID_STATIC, _("CFGs"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer11->Add(itemStaticText12, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  itemBoxSizer11->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

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

  itemBoxSizer15->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  dirPickerTmp = new wxDirPickerCtrl( itemPanel2, ID_DIRPICKERCTRL_TMP, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_USE_TEXTCTRL );
  if (PreferencesDialog::ShowToolTips())
    dirPickerTmp->SetToolTip(_("Working directory for results and logs."));
  dirPickerTmp->Enable(false);
  itemBoxSizer15->Add(dirPickerTmp, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  GetBookCtrl()->AddPage(itemPanel2, _("Global"));

  wxPanel* itemPanel19 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_TIMELINE, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  if (PreferencesDialog::ShowToolTips())
    itemPanel19->SetToolTip(_("Timeline What/Where information preferences."));
  wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
  itemPanel19->SetSizer(itemBoxSizer20);

  wxStaticBox* itemStaticBoxSizer21Static = new wxStaticBox(itemPanel19, wxID_ANY, _("  Rendering Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer21 = new wxStaticBoxSizer(itemStaticBoxSizer21Static, wxVERTICAL);
  itemStaticBoxSizer21Static->Enable(false);
  itemBoxSizer20->Add(itemStaticBoxSizer21, 1, wxGROW|wxALL, 5);
  checkTimelineEventLines = new wxCheckBox( itemPanel19, ID_CHECKBOX5, _("Event Lines"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineEventLines->SetValue(true);
  checkTimelineEventLines->Enable(false);
  itemStaticBoxSizer21->Add(checkTimelineEventLines, 0, wxGROW|wxTOP, 5);

  checkTimelineCommunicationLines = new wxCheckBox( itemPanel19, ID_CHECKBOX6, _("Communication Lines"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineCommunicationLines->SetValue(true);
  checkTimelineCommunicationLines->Enable(false);
  itemStaticBoxSizer21->Add(checkTimelineCommunicationLines, 0, wxGROW, 5);

  checkTimelineFunctionAsColor = new wxCheckBox( itemPanel19, ID_CHECKBOX7, _("Semantic Values as Color"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineFunctionAsColor->SetValue(true);
  checkTimelineFunctionAsColor->Enable(false);
  itemStaticBoxSizer21->Add(checkTimelineFunctionAsColor, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer21->Add(itemBoxSizer25, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText26 = new wxStaticText( itemPanel19, wxID_STATIC, _("Color"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticText26->Enable(false);
  itemBoxSizer25->Add(itemStaticText26, 5, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  itemBoxSizer25->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString choiceTimelineColorStrings;
  choiceTimelineColor = new wxChoice( itemPanel19, ID_CHOICE, wxDefaultPosition, wxDefaultSize, choiceTimelineColorStrings, 0 );
  choiceTimelineColor->Enable(false);
  itemBoxSizer25->Add(choiceTimelineColor, 5, wxGROW|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer29 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer21->Add(itemBoxSizer29, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText30 = new wxStaticText( itemPanel19, wxID_STATIC, _("Gradient is"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticText30->Enable(false);
  itemBoxSizer29->Add(itemStaticText30, 5, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  itemBoxSizer29->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString choiceTimelineGradientFunctionStrings;
  choiceTimelineGradientFunction = new wxChoice( itemPanel19, ID_CHOICE4, wxDefaultPosition, wxDefaultSize, choiceTimelineGradientFunctionStrings, 0 );
  choiceTimelineGradientFunction->Enable(false);
  itemBoxSizer29->Add(choiceTimelineGradientFunction, 5, wxGROW|wxRIGHT, 5);

  wxStaticBox* itemStaticBoxSizer33Static = new wxStaticBox(itemPanel19, wxID_ANY, _("  Drawmode  "));
  wxStaticBoxSizer* itemStaticBoxSizer33 = new wxStaticBoxSizer(itemStaticBoxSizer33Static, wxVERTICAL);
  itemStaticBoxSizer33Static->Enable(false);
  itemStaticBoxSizer21->Add(itemStaticBoxSizer33, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer34 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer33->Add(itemBoxSizer34, 0, wxGROW, 5);
  wxStaticText* itemStaticText35 = new wxStaticText( itemPanel19, wxID_STATIC, _("Time"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticText35->Enable(false);
  itemBoxSizer34->Add(itemStaticText35, 5, wxALIGN_CENTER_VERTICAL, 5);

  itemBoxSizer34->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString choiceTimelineDrawmodeTimeStrings;
  choiceTimelineDrawmodeTime = new wxChoice( itemPanel19, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, choiceTimelineDrawmodeTimeStrings, 0 );
  choiceTimelineDrawmodeTime->Enable(false);
  itemBoxSizer34->Add(choiceTimelineDrawmodeTime, 5, wxGROW|wxTOP, 5);

  wxBoxSizer* itemBoxSizer38 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer33->Add(itemBoxSizer38, 0, wxGROW, 5);
  wxStaticText* itemStaticText39 = new wxStaticText( itemPanel19, wxID_STATIC, _("Objects"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticText39->Enable(false);
  itemBoxSizer38->Add(itemStaticText39, 5, wxALIGN_CENTER_VERTICAL, 5);

  itemBoxSizer38->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString choiceTimelineDrawmodeObjectsStrings;
  choiceTimelineDrawmodeObjects = new wxChoice( itemPanel19, ID_CHOICE2, wxDefaultPosition, wxDefaultSize, choiceTimelineDrawmodeObjectsStrings, 0 );
  choiceTimelineDrawmodeObjects->Enable(false);
  itemBoxSizer38->Add(choiceTimelineDrawmodeObjects, 5, wxGROW|wxTOP, 5);

  wxBoxSizer* itemBoxSizer42 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer21->Add(itemBoxSizer42, 0, wxGROW|wxTOP|wxBOTTOM, 5);
  wxStaticText* itemStaticText43 = new wxStaticText( itemPanel19, wxID_STATIC, _("Pixel Size"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticText43->Enable(false);
  itemBoxSizer42->Add(itemStaticText43, 5, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  itemBoxSizer42->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString choiceTimelinePixelSizeStrings;
  choiceTimelinePixelSize = new wxChoice( itemPanel19, ID_CHOICE5, wxDefaultPosition, wxDefaultSize, choiceTimelinePixelSizeStrings, 0 );
  choiceTimelinePixelSize->Enable(false);
  itemBoxSizer42->Add(choiceTimelinePixelSize, 5, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP, 5);

  wxBoxSizer* itemBoxSizer46 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer20->Add(itemBoxSizer46, 1, wxGROW, 5);
  wxStaticBox* itemStaticBoxSizer47Static = new wxStaticBox(itemPanel19, wxID_ANY, _("  What / Where  "));
  wxStaticBoxSizer* itemStaticBoxSizer47 = new wxStaticBoxSizer(itemStaticBoxSizer47Static, wxVERTICAL);
  itemBoxSizer46->Add(itemStaticBoxSizer47, 2, wxGROW|wxALL, 5);
  checkTimelineWWSemantic = new wxCheckBox( itemPanel19, ID_CHECKBOX_TIMELINE_WW_SEMANTIC, _("Semantic"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWSemantic->SetValue(true);
  checkTimelineWWSemantic->Enable(false);
  itemStaticBoxSizer47->Add(checkTimelineWWSemantic, 0, wxGROW|wxTOP, 5);

  checkTimelineWWEvents = new wxCheckBox( itemPanel19, ID_CHECKBOX_TIMELINE_WW_EVENTS, _("Events"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWEvents->SetValue(true);
  checkTimelineWWEvents->Enable(false);
  itemStaticBoxSizer47->Add(checkTimelineWWEvents, 0, wxGROW, 5);

  checkTimelineWWCommunications = new wxCheckBox( itemPanel19, ID_CHECKBOX_TIMELINE_WW_COMMUNICATIONS, _("Communications"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWCommunications->SetValue(true);
  checkTimelineWWCommunications->Enable(false);
  itemStaticBoxSizer47->Add(checkTimelineWWCommunications, 0, wxGROW, 5);

  checkTimelineWWPreviousNext = new wxCheckBox( itemPanel19, ID_CHECKBOX_TIMELINE_WW_PREVIOUS_NEXT, _("Previous / Next"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWPreviousNext->SetValue(false);
  checkTimelineWWPreviousNext->Enable(false);
  itemStaticBoxSizer47->Add(checkTimelineWWPreviousNext, 0, wxGROW, 5);

  checkTimelineWWText = new wxCheckBox( itemPanel19, ID_CHECKBOX_TIMELINE_WW_TEXT, _("Text"), wxDefaultPosition, wxDefaultSize, 0 );
  checkTimelineWWText->SetValue(true);
  checkTimelineWWText->Enable(false);
  itemStaticBoxSizer47->Add(checkTimelineWWText, 0, wxGROW|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer53 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer47->Add(itemBoxSizer53, 0, wxGROW|wxTOP|wxBOTTOM, 5);
  wxStaticText* itemStaticText54 = new wxStaticText( itemPanel19, wxID_STATIC, _("Precision"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer53->Add(itemStaticText54, 5, wxALIGN_CENTER_VERTICAL, 5);

  itemBoxSizer53->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txtWhatWherePrecision = new wxTextCtrl( itemPanel19, ID_TEXTBOX_TIMELINE_WW_PRECISION, _("2"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer53->Add(txtWhatWherePrecision, 5, wxGROW, 5);

  wxStaticBox* itemStaticBoxSizer57Static = new wxStaticBox(itemPanel19, wxID_ANY, _("  Save Options  "));
  wxStaticBoxSizer* itemStaticBoxSizer57 = new wxStaticBoxSizer(itemStaticBoxSizer57Static, wxVERTICAL);
  itemStaticBoxSizer57Static->Enable(false);
  itemBoxSizer46->Add(itemStaticBoxSizer57, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer58 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer57->Add(itemBoxSizer58, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText59 = new wxStaticText( itemPanel19, wxID_STATIC, _("Image as"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticText59->Enable(false);
  itemBoxSizer58->Add(itemStaticText59, 5, wxALIGN_CENTER_VERTICAL, 5);

  itemBoxSizer58->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString choiceTimelineSaveImageFormatStrings;
  choiceTimelineSaveImageFormat = new wxChoice( itemPanel19, ID_CHOICE_TIMELINE_SAVE_AS_IMAGE, wxDefaultPosition, wxDefaultSize, choiceTimelineSaveImageFormatStrings, 0 );
  choiceTimelineSaveImageFormat->Enable(false);
  itemBoxSizer58->Add(choiceTimelineSaveImageFormat, 5, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer62 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer57->Add(itemBoxSizer62, 0, wxGROW|wxTOP, 5);
  wxStaticText* itemStaticText63 = new wxStaticText( itemPanel19, wxID_STATIC, _("Text as"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticText63->Enable(false);
  itemBoxSizer62->Add(itemStaticText63, 5, wxALIGN_CENTER_VERTICAL, 5);

  itemBoxSizer62->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString choiceTimelineSaveTextFormatStrings;
  choiceTimelineSaveTextFormat = new wxChoice( itemPanel19, ID_CHOICE_TIMELINE_SAVE_AS_TEXT, wxDefaultPosition, wxDefaultSize, choiceTimelineSaveTextFormatStrings, 0 );
  choiceTimelineSaveTextFormat->Enable(false);
  itemBoxSizer62->Add(choiceTimelineSaveTextFormat, 5, wxALIGN_CENTER_VERTICAL, 5);

  GetBookCtrl()->AddPage(itemPanel19, _("Timeline"));

  wxPanel* itemPanel66 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_HISTOGRAM, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  if (PreferencesDialog::ShowToolTips())
    itemPanel66->SetToolTip(_("Histogram preferences."));
  wxBoxSizer* itemBoxSizer67 = new wxBoxSizer(wxVERTICAL);
  itemPanel66->SetSizer(itemBoxSizer67);

  wxBoxSizer* itemBoxSizer68 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer67->Add(itemBoxSizer68, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText69 = new wxStaticText( itemPanel66, wxID_STATIC, _("Columns"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText69->SetToolTip(_("Initial number of columns for new histograms."));
  itemBoxSizer68->Add(itemStaticText69, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txt2DNumColumns = new wxTextCtrl( itemPanel66, ID_PREFERENCES_2D_NUMCOLUMNS, _("20"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    txt2DNumColumns->SetToolTip(_("Initial number of columns for new histograms."));
  itemBoxSizer68->Add(txt2DNumColumns, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer71 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer67->Add(itemBoxSizer71, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText72 = new wxStaticText( itemPanel66, wxID_STATIC, _("Precision"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText72->SetToolTip(_("Decimal precision applied to the presentation of histogram metrics."));
  itemBoxSizer71->Add(itemStaticText72, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txt2DPrecision = new wxTextCtrl( itemPanel66, ID_PREFERENCES_2D_PRECISION, _("4"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    txt2DPrecision->SetToolTip(_("Decimal precision applied to the presentation of histogram metrics."));
  itemBoxSizer71->Add(txt2DPrecision, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString check2DOptionsStrings;
  check2DOptionsStrings.Add(_("Show Time Units"));
  check2DOptionsStrings.Add(_("Thousands Separator"));
  check2DOptions = new wxCheckListBox( itemPanel66, ID_PREFERENCES_2D_CHECKOPTIONS, wxDefaultPosition, wxDefaultSize, check2DOptionsStrings, wxLB_SINGLE );
  if (PreferencesDialog::ShowToolTips())
    check2DOptions->SetToolTip(_("Properties applied to the presentation of histogram metrics."));
  itemBoxSizer67->Add(check2DOptions, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  GetBookCtrl()->AddPage(itemPanel66, _("Histogram"));

  wxPanel* itemPanel75 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_COLOR, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer76 = new wxBoxSizer(wxVERTICAL);
  itemPanel75->SetSizer(itemBoxSizer76);

  wxStaticBox* itemStaticBoxSizer77Static = new wxStaticBox(itemPanel75, wxID_ANY, _("  Timeline  "));
  wxStaticBoxSizer* itemStaticBoxSizer77 = new wxStaticBoxSizer(itemStaticBoxSizer77Static, wxVERTICAL);
  itemBoxSizer76->Add(itemStaticBoxSizer77, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer78 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer77->Add(itemBoxSizer78, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText79 = new wxStaticText( itemPanel75, wxID_STATIC, _("Background"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText79->SetToolTip(_("When background timeline color is changed, new foreground complementary color is suggested."));
  itemBoxSizer78->Add(itemStaticText79, 5, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  itemBoxSizer78->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  colourPickerBackground = new wxColourPickerCtrl( itemPanel75, ID_COLOURPICKER_BACKGROUND, wxColour(0, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerBackground->SetToolTip(_("When background timeline color is changed, new foreground complementary color is suggested."));
  itemBoxSizer78->Add(colourPickerBackground, 5, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer82 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer77->Add(itemBoxSizer82, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText83 = new wxStaticText( itemPanel75, wxID_STATIC, _("Foreground"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText83->SetToolTip(_("Color used to draw scale axis and object names in timelines."));
  itemBoxSizer82->Add(itemStaticText83, 5, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  itemBoxSizer82->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  colourPickerAxis = new wxColourPickerCtrl( itemPanel75, ID_COLOURPICKER_AXIS, wxColour(255, 255, 255), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerAxis->SetToolTip(_("Color used to draw scale axis and object names in timelines."));
  itemBoxSizer82->Add(colourPickerAxis, 5, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer86 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer77->Add(itemBoxSizer86, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText87 = new wxStaticText( itemPanel75, wxID_STATIC, _("Logical comms."), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText87->SetToolTip(_("Color used to draw logical communications in timelines."));
  itemBoxSizer86->Add(itemStaticText87, 5, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  itemBoxSizer86->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  colourPickerLogical = new wxColourPickerCtrl( itemPanel75, ID_COLOURPICKER_LOGICAL, wxColour(255, 255, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerLogical->SetToolTip(_("Color used to draw logical communications in timelines."));
  itemBoxSizer86->Add(colourPickerLogical, 5, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer90 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer77->Add(itemBoxSizer90, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText91 = new wxStaticText( itemPanel75, wxID_STATIC, _("Physical comms."), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText91->SetToolTip(_("Color used to draw physical communications in timelines."));
  itemBoxSizer90->Add(itemStaticText91, 5, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  itemBoxSizer90->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  colourPickerPhysical = new wxColourPickerCtrl( itemPanel75, ID_COLOURPICKER_PHYSICAL, wxColour(255, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerPhysical->SetToolTip(_("Color used to draw physical communications in timelines."));
  itemBoxSizer90->Add(colourPickerPhysical, 5, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

  wxButton* itemButton94 = new wxButton( itemPanel75, ID_BUTTON_DEFAULT_TIMELINE, _("Default"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemButton94->SetToolTip(_("Restores predefined colors."));
  itemStaticBoxSizer77->Add(itemButton94, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT, 5);

  wxStaticBox* itemStaticBoxSizer95Static = new wxStaticBox(itemPanel75, wxID_ANY, _("  Gradient  "));
  wxStaticBoxSizer* itemStaticBoxSizer95 = new wxStaticBoxSizer(itemStaticBoxSizer95Static, wxVERTICAL);
  itemBoxSizer76->Add(itemStaticBoxSizer95, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer96 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer95->Add(itemBoxSizer96, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText97 = new wxStaticText( itemPanel75, wxID_STATIC, _("Begin"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText97->SetToolTip(_("Base color used to represent semantic values when gradient is selected."));
  itemBoxSizer96->Add(itemStaticText97, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  itemBoxSizer96->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientBegin = new wxColourPickerCtrl( itemPanel75, ID_COLOURPICKER_GRADBEGIN, wxColour(0, 255, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientBegin->SetToolTip(_("Base color used to represent semantic values when gradient is selected."));
  itemBoxSizer96->Add(colourPickerGradientBegin, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer100 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer95->Add(itemBoxSizer100, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText101 = new wxStaticText( itemPanel75, wxID_STATIC, _("End"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText101->SetToolTip(_("Top color used to represent semantic values when gradient is selected."));
  itemBoxSizer100->Add(itemStaticText101, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  itemBoxSizer100->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientEnd = new wxColourPickerCtrl( itemPanel75, ID_COLOURPICKER_GRADEND, wxColour(0, 0, 255), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientEnd->SetToolTip(_("Top color used to represent semantic values when gradient is selected."));
  itemBoxSizer100->Add(colourPickerGradientEnd, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer104 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer95->Add(itemBoxSizer104, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText105 = new wxStaticText( itemPanel75, wxID_STATIC, _("Low outliers"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText105->SetToolTip(_("Any semantic outlier smaller than <Begin> will use this color."));
  itemBoxSizer104->Add(itemStaticText105, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  itemBoxSizer104->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientLow = new wxColourPickerCtrl( itemPanel75, ID_COLOURPICKER_GRADLOW, wxColour(207, 207, 68), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientLow->SetToolTip(_("Any semantic outlier smaller than <Begin> will use this color."));
  itemBoxSizer104->Add(colourPickerGradientLow, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer108 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer95->Add(itemBoxSizer108, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText109 = new wxStaticText( itemPanel75, wxID_STATIC, _("High outliers"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText109->SetToolTip(_("Any semantic outlier greater than <End> will use this color."));
  itemBoxSizer108->Add(itemStaticText109, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  itemBoxSizer108->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientTop = new wxColourPickerCtrl( itemPanel75, ID_COLOURPICKER_GRADTOP, wxColour(255, 146, 24), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientTop->SetToolTip(_("Any semantic outlier greater than <End> will use this color."));
  itemBoxSizer108->Add(colourPickerGradientTop, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxButton* itemButton112 = new wxButton( itemPanel75, ID_BUTTON_DEFAULT_GRADIENT, _("Default"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemButton112->SetToolTip(_("Restores predefined colors."));
  itemStaticBoxSizer95->Add(itemButton112, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT, 5);

  GetBookCtrl()->AddPage(itemPanel75, _("Color"));

  wxPanel* itemPanel113 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_FILTERS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  itemPanel113->Show(false);
  itemPanel113->Enable(false);

  GetBookCtrl()->AddPage(itemPanel113, _("Filters"));

////@end PreferencesDialog content construction

  // Filter forbidden chars
  wxArrayString forbidden;
  forbidden.Add("-");
  forbidden.Add(".");
  forbidden.Add(",");

  wxTextValidator validator( wxFILTER_NUMERIC | wxFILTER_EXCLUDE_CHAR_LIST );
  validator.SetExcludes( forbidden );

  txt2DNumColumns->SetValidator( validator );
  txt2DPrecision->SetValidator( validator );
  txtWhatWherePrecision->SetValidator( validator );
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


void PreferencesDialog::presetStringChoiceBox( vector< string > list,
                                               wxChoice *choiceBox )
{
  wxString aux;

  for( vector< string >::iterator it = list.begin(); it != list.end(); ++it )
  {
    aux << *it;
    choiceBox->Append( aux );
    aux.clear();
  }

  choiceBox->Select( 0 );
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
/*  checkTimelineEventLines->SetValue( timelineEventLines );
  checkTimelineCommunicationLines->SetValue( timelineCommunicationLines );
  checkTimelineFunctionAsColor->SetValue( timelineFunctionAsColor );

  options.push_back( "Code Color" );
  options.push_back( "Gradient Color" );
  options.push_back( "Not Null Gradient" );
  presetStringChoiceBox( options, choiceTimelineColor );
  presetStringChoiceBox( options, choiceTimelineGradientFunction );
  presetStringChoiceBox( options, choiceTimelineDrawmodeTime );
  presetStringChoiceBox( options, choiceTimelineDrawmodeObjects );
  presetStringChoiceBox( options, choiceTimelinePixelSize );

  checkTimelineWWSemantic->SetValue( timelineWWSemantic );
  checkTimelineWWEvents->SetValue( timelineWWEvents );
  checkTimelineWWCommunications->SetValue( timelineWWCommunications );
  checkTimelineWWPreviousNext->SetValue( timelineWWPreviousNext );
  checkTimelineWWText->SetValue( timelineWWText );
*/
  txtWhatWherePrecision->SetValue( formatNumber( whatWherePrecision ));
/*
  presetStringChoiceBox( options, choiceTimelineSaveImageFormat );
  presetStringChoiceBox( options, choiceTimelineSaveTextFormat );
*/
  // HISTOGRAM
  txt2DPrecision->SetValue( formatNumber( histogramPrecision ));
  txt2DNumColumns->SetValue( formatNumber( histogramNumColumns ));

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
  long aux;

  // GLOBAL
  globalFillStateGaps = checkGlobalFillStateGaps->IsChecked();
  tracesPath = dirPickerTrace->GetPath();
  cfgsPath = dirPickerCFG->GetPath();
  tmpPath = dirPickerTmp->GetPath();

  // HISTOGRAM
  if ( txt2DPrecision->GetValue().ToLong( &aux ) )
    histogramPrecision = ( UINT32 )aux;
  if ( txt2DNumColumns->GetValue().ToLong( &aux ) )
    histogramNumColumns = ( UINT32 )aux;

  histogramShowUnits = check2DOptions->IsChecked( ITEM_SHOW_UNITS );
  histogramThousandSeparator = check2DOptions->IsChecked( ITEM_THOUSAND_SEPARATOR );

  // TIMELINE
  if ( txtWhatWherePrecision->GetValue().ToLong( &aux ) )
    whatWherePrecision = ( UINT32 )aux;

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

