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
  txtWhatWherePrecision = NULL;
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
  itemBoxSizer15->Add(itemStaticText16, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  itemBoxSizer15->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  dirPickerTmp = new wxDirPickerCtrl( itemPanel2, ID_DIRPICKERCTRL_TMP, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_USE_TEXTCTRL );
  if (PreferencesDialog::ShowToolTips())
    dirPickerTmp->SetToolTip(_("Working directory for results and logs."));
  itemBoxSizer15->Add(dirPickerTmp, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  GetBookCtrl()->AddPage(itemPanel2, _("Global"));

  wxPanel* itemPanel19 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_TIMELINE, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  if (PreferencesDialog::ShowToolTips())
    itemPanel19->SetToolTip(_("Timeline What/Where information preferences."));
  itemPanel19->Enable(false);
  wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
  itemPanel19->SetSizer(itemBoxSizer20);

  wxStaticText* itemStaticText21 = new wxStaticText( itemPanel19, wxID_STATIC, _("Precision"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer20->Add(itemStaticText21, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txtWhatWherePrecision = new wxTextCtrl( itemPanel19, ID_PREFERENCES_WW_PRECISION, _("2"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer20->Add(txtWhatWherePrecision, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  GetBookCtrl()->AddPage(itemPanel19, _("Timeline"));

  wxPanel* itemPanel23 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_HISTOGRAM, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  if (PreferencesDialog::ShowToolTips())
    itemPanel23->SetToolTip(_("Histogram preferences."));
  wxBoxSizer* itemBoxSizer24 = new wxBoxSizer(wxVERTICAL);
  itemPanel23->SetSizer(itemBoxSizer24);

  wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer24->Add(itemBoxSizer25, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText26 = new wxStaticText( itemPanel23, wxID_STATIC, _("Columns"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText26->SetToolTip(_("Initial number of columns for new histograms."));
  itemBoxSizer25->Add(itemStaticText26, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txt2DNumColumns = new wxTextCtrl( itemPanel23, ID_PREFERENCES_2D_NUMCOLUMNS, _("20"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    txt2DNumColumns->SetToolTip(_("Initial number of columns for new histograms."));
  itemBoxSizer25->Add(txt2DNumColumns, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer28 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer24->Add(itemBoxSizer28, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText29 = new wxStaticText( itemPanel23, wxID_STATIC, _("Precision"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText29->SetToolTip(_("Decimal precision applied to the presentation of histogram metrics."));
  itemBoxSizer28->Add(itemStaticText29, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txt2DPrecision = new wxTextCtrl( itemPanel23, ID_PREFERENCES_2D_PRECISION, _("4"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    txt2DPrecision->SetToolTip(_("Decimal precision applied to the presentation of histogram metrics."));
  itemBoxSizer28->Add(txt2DPrecision, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString check2DOptionsStrings;
  check2DOptionsStrings.Add(_("Show Time Units"));
  check2DOptionsStrings.Add(_("Thousands Separator"));
  check2DOptions = new wxCheckListBox( itemPanel23, ID_PREFERENCES_2D_CHECKOPTIONS, wxDefaultPosition, wxDefaultSize, check2DOptionsStrings, wxLB_SINGLE );
  if (PreferencesDialog::ShowToolTips())
    check2DOptions->SetToolTip(_("Properties applied to the presentation of histogram metrics."));
  itemBoxSizer24->Add(check2DOptions, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  GetBookCtrl()->AddPage(itemPanel23, _("Histogram"));

  wxPanel* itemPanel32 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_COLOR, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  wxBoxSizer* itemBoxSizer33 = new wxBoxSizer(wxVERTICAL);
  itemPanel32->SetSizer(itemBoxSizer33);

  wxStaticBox* itemStaticBoxSizer34Static = new wxStaticBox(itemPanel32, wxID_ANY, _("  Timeline  "));
  wxStaticBoxSizer* itemStaticBoxSizer34 = new wxStaticBoxSizer(itemStaticBoxSizer34Static, wxVERTICAL);
  itemBoxSizer33->Add(itemStaticBoxSizer34, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer35 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer34->Add(itemBoxSizer35, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText36 = new wxStaticText( itemPanel32, wxID_STATIC, _("Background"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText36->SetToolTip(_("When background timeline color is changed, new foreground complementary color is suggested."));
  itemBoxSizer35->Add(itemStaticText36, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  itemBoxSizer35->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  colourPickerBackground = new wxColourPickerCtrl( itemPanel32, ID_COLOURPICKER_BACKGROUND, wxColour(0, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerBackground->SetToolTip(_("When background timeline color is changed, new foreground complementary color is suggested."));
  itemBoxSizer35->Add(colourPickerBackground, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer39 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer34->Add(itemBoxSizer39, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText40 = new wxStaticText( itemPanel32, wxID_STATIC, _("Foreground"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText40->SetToolTip(_("Color used to draw scale axis and object names in timelines."));
  itemBoxSizer39->Add(itemStaticText40, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  itemBoxSizer39->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  colourPickerAxis = new wxColourPickerCtrl( itemPanel32, ID_COLOURPICKER_AXIS, wxColour(255, 255, 255), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerAxis->SetToolTip(_("Color used to draw scale axis and object names in timelines."));
  itemBoxSizer39->Add(colourPickerAxis, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer43 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer34->Add(itemBoxSizer43, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText44 = new wxStaticText( itemPanel32, wxID_STATIC, _("Logical comms."), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText44->SetToolTip(_("Color used to draw logical communications in timelines."));
  itemBoxSizer43->Add(itemStaticText44, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  itemBoxSizer43->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  colourPickerLogical = new wxColourPickerCtrl( itemPanel32, ID_COLOURPICKER_LOGICAL, wxColour(255, 255, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerLogical->SetToolTip(_("Color used to draw logical communications in timelines."));
  itemBoxSizer43->Add(colourPickerLogical, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer47 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer34->Add(itemBoxSizer47, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText48 = new wxStaticText( itemPanel32, wxID_STATIC, _("Physical comms."), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText48->SetToolTip(_("Color used to draw physical communications in timelines."));
  itemBoxSizer47->Add(itemStaticText48, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  itemBoxSizer47->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  colourPickerPhysical = new wxColourPickerCtrl( itemPanel32, ID_COLOURPICKER_PHYSICAL, wxColour(255, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerPhysical->SetToolTip(_("Color used to draw physical communications in timelines."));
  itemBoxSizer47->Add(colourPickerPhysical, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

  wxButton* itemButton51 = new wxButton( itemPanel32, ID_BUTTON_DEFAULT_TIMELINE, _("Default"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemButton51->SetToolTip(_("Restores predefined colors."));
  itemStaticBoxSizer34->Add(itemButton51, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

  wxStaticBox* itemStaticBoxSizer52Static = new wxStaticBox(itemPanel32, wxID_ANY, _("  Gradient  "));
  wxStaticBoxSizer* itemStaticBoxSizer52 = new wxStaticBoxSizer(itemStaticBoxSizer52Static, wxVERTICAL);
  itemBoxSizer33->Add(itemStaticBoxSizer52, 1, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer53 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer52->Add(itemBoxSizer53, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText54 = new wxStaticText( itemPanel32, wxID_STATIC, _("Begin"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText54->SetToolTip(_("Base color used to represent semantic values when gradient is selected."));
  itemBoxSizer53->Add(itemStaticText54, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  itemBoxSizer53->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientBegin = new wxColourPickerCtrl( itemPanel32, ID_COLOURPICKER_GRADBEGIN, wxColour(0, 255, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientBegin->SetToolTip(_("Base color used to represent semantic values when gradient is selected."));
  itemBoxSizer53->Add(colourPickerGradientBegin, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer57 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer52->Add(itemBoxSizer57, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText58 = new wxStaticText( itemPanel32, wxID_STATIC, _("End"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText58->SetToolTip(_("Top color used to represent semantic values when gradient is selected."));
  itemBoxSizer57->Add(itemStaticText58, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  itemBoxSizer57->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientEnd = new wxColourPickerCtrl( itemPanel32, ID_COLOURPICKER_GRADEND, wxColour(0, 0, 255), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientEnd->SetToolTip(_("Top color used to represent semantic values when gradient is selected."));
  itemBoxSizer57->Add(colourPickerGradientEnd, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer61 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer52->Add(itemBoxSizer61, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText62 = new wxStaticText( itemPanel32, wxID_STATIC, _("Low outliers"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText62->SetToolTip(_("Any semantic outlier smaller than <Begin> will use this color."));
  itemBoxSizer61->Add(itemStaticText62, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  itemBoxSizer61->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientLow = new wxColourPickerCtrl( itemPanel32, ID_COLOURPICKER_GRADLOW, wxColour(207, 207, 68), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientLow->SetToolTip(_("Any semantic outlier smaller than <Begin> will use this color."));
  itemBoxSizer61->Add(colourPickerGradientLow, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer65 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer52->Add(itemBoxSizer65, 1, wxGROW|wxLEFT|wxRIGHT, 5);
  wxStaticText* itemStaticText66 = new wxStaticText( itemPanel32, wxID_STATIC, _("High outliers"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText66->SetToolTip(_("Any semantic outlier greater than <End> will use this color."));
  itemBoxSizer65->Add(itemStaticText66, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  itemBoxSizer65->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL, 5);

  colourPickerGradientTop = new wxColourPickerCtrl( itemPanel32, ID_COLOURPICKER_GRADTOP, wxColour(255, 146, 24), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL|wxCLRP_SHOW_LABEL );
  if (PreferencesDialog::ShowToolTips())
    colourPickerGradientTop->SetToolTip(_("Any semantic outlier greater than <End> will use this color."));
  itemBoxSizer65->Add(colourPickerGradientTop, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxButton* itemButton69 = new wxButton( itemPanel32, ID_BUTTON_DEFAULT_GRADIENT, _("Default"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemButton69->SetToolTip(_("Restores predefined colors."));
  itemStaticBoxSizer52->Add(itemButton69, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

  GetBookCtrl()->AddPage(itemPanel32, _("Color"));

  wxPanel* itemPanel70 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_FILTERS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  itemPanel70->Enable(false);

  GetBookCtrl()->AddPage(itemPanel70, _("Filters"));

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


// Transfers data to all panels
bool PreferencesDialog::TransferDataToWindow()
{
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
  txtWhatWherePrecision->SetValue( formatNumber( whatWherePrecision ));

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

