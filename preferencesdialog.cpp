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
  txt2DNumColumns = NULL;
  txt2DPrecision = NULL;
  check2DOptions = NULL;
  txtWhatWherePrecision = NULL;
  checkGlobalFillStateGaps = NULL;
////@end PreferencesDialog member initialisation
}


/*!
 * Control creation for PreferencesDialog
 */

void PreferencesDialog::CreateControls()
{    
////@begin PreferencesDialog content construction
  PreferencesDialog* itemPropertySheetDialog1 = this;

  wxPanel* itemPanel2 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_2D, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  if (PreferencesDialog::ShowToolTips())
    itemPanel2->SetToolTip(_("Histogram preferences."));
  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
  itemPanel2->SetSizer(itemBoxSizer3);

  wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer3->Add(itemBoxSizer4, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText5 = new wxStaticText( itemPanel2, wxID_STATIC, _("Columns"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText5->SetToolTip(_("Initial number of columns for new histograms."));
  itemBoxSizer4->Add(itemStaticText5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txt2DNumColumns = new wxTextCtrl( itemPanel2, ID_PREFERENCES_2D_NUMCOLUMNS, _("20"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    txt2DNumColumns->SetToolTip(_("Initial number of columns for new histograms."));
  itemBoxSizer4->Add(txt2DNumColumns, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer3->Add(itemBoxSizer7, 0, wxGROW|wxALL, 5);
  wxStaticText* itemStaticText8 = new wxStaticText( itemPanel2, wxID_STATIC, _("Precision"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    itemStaticText8->SetToolTip(_("Decimal precision applied to the presentation of histogram metrics."));
  itemBoxSizer7->Add(itemStaticText8, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txt2DPrecision = new wxTextCtrl( itemPanel2, ID_PREFERENCES_2D_PRECISION, _("4"), wxDefaultPosition, wxDefaultSize, 0 );
  if (PreferencesDialog::ShowToolTips())
    txt2DPrecision->SetToolTip(_("Decimal precision applied to the presentation of histogram metrics."));
  itemBoxSizer7->Add(txt2DPrecision, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString check2DOptionsStrings;
  check2DOptionsStrings.Add(_("Show Time Units"));
  check2DOptionsStrings.Add(_("Thousands Separator"));
  check2DOptions = new wxCheckListBox( itemPanel2, ID_PREFERENCES_2D_CHECKOPTIONS, wxDefaultPosition, wxDefaultSize, check2DOptionsStrings, wxLB_SINGLE );
  if (PreferencesDialog::ShowToolTips())
    check2DOptions->SetToolTip(_("Properties applied to the presentation of histogram metrics."));
  itemBoxSizer3->Add(check2DOptions, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  GetBookCtrl()->AddPage(itemPanel2, _("2D"));

  wxPanel* itemPanel11 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_WHATWHERE, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  if (PreferencesDialog::ShowToolTips())
    itemPanel11->SetToolTip(_("Timeline What/Where information preferences."));
  itemPanel11->Enable(false);
  wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
  itemPanel11->SetSizer(itemBoxSizer12);

  wxStaticText* itemStaticText13 = new wxStaticText( itemPanel11, wxID_STATIC, _("Precision"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer12->Add(itemStaticText13, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txtWhatWherePrecision = new wxTextCtrl( itemPanel11, ID_PREFERENCES_WW_PRECISION, _("2"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer12->Add(txtWhatWherePrecision, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  GetBookCtrl()->AddPage(itemPanel11, _("What/Where"));

  wxPanel* itemPanel15 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_GLOBAL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  checkGlobalFillStateGaps = new wxCheckBox( itemPanel15, ID_PREFERENCES_GLOBAL_FILLGAPS, _("Fill State gaps with IDLE State"), wxDefaultPosition, wxDefaultSize, 0 );
  checkGlobalFillStateGaps->SetValue(false);


  GetBookCtrl()->AddPage(itemPanel15, _("Global"));

  wxPanel* itemPanel18 = new wxPanel( GetBookCtrl(), ID_PREFERENCES_FILTERS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  itemPanel18->Show(false);
  itemPanel18->Enable(false);

  GetBookCtrl()->AddPage(itemPanel18, _("Filters"));

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
  // 2D
  txt2DPrecision->SetValue( formatNumber( histogramPrecision ));
  txt2DNumColumns->SetValue( formatNumber( histogramNumColumns ));

  check2DOptions->Check( ITEM_SHOW_UNITS, histogramShowUnits );
  check2DOptions->Check( ITEM_THOUSAND_SEPARATOR, histogramThousandSeparator );

  // What Where
  txtWhatWherePrecision->SetValue( formatNumber( whatWherePrecision ));

  // Global
  checkGlobalFillStateGaps->SetValue( globalFillStateGaps );

  return true;
}


// Transfers data from all panels
bool PreferencesDialog::TransferDataFromWindow()
{
  long aux;

  // 2D
  if ( txt2DPrecision->GetValue().ToLong( &aux ) )
    histogramPrecision = ( UINT32 )aux;
  if ( txt2DNumColumns->GetValue().ToLong( &aux ) )
    histogramNumColumns = ( UINT32 )aux;

  histogramShowUnits = check2DOptions->IsChecked( ITEM_SHOW_UNITS );
  histogramThousandSeparator = check2DOptions->IsChecked( ITEM_THOUSAND_SEPARATOR );

  // What Where
  if ( txtWhatWherePrecision->GetValue().ToLong( &aux ) )
    whatWherePrecision = ( UINT32 )aux;

  // Global
  globalFillStateGaps = checkGlobalFillStateGaps->IsChecked();

  return true;
}



