/////////////////////////////////////////////////////////////////////////////
// Name:        histogramdialog.cpp
// Purpose:     
// Author:      Eloy Martinez
// Modified by: 
// Created:     Wed 02 Sep 2009 12:48:45 CEST
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
////@end includes

#include <sstream>
#include "histogramdialog.h"
#include "paraverconfig.h"
// #include "histogram.h"

////@begin XPM images
////@end XPM images


/*!
 * HistogramDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( HistogramDialog, wxDialog )


/*!
 * HistogramDialog event table definition
 */

BEGIN_EVENT_TABLE( HistogramDialog, wxDialog )

////@begin HistogramDialog event table entries
  EVT_CHOICE( ID_HISTOGRAM_CONTROLTIMELINELIST, HistogramDialog::OnHistogramControltimelinelistSelected )

  EVT_TOGGLEBUTTON( ID_HISTOGRAM_CONTROLTIMELINEAUTOFIT, HistogramDialog::OnHistogramControltimelineautofitClick )
  EVT_UPDATE_UI( ID_HISTOGRAM_CONTROLTIMELINEAUTOFIT, HistogramDialog::OnHistogramControltimelineautofitUpdate )

  EVT_CHOICE( ID_HISTOGRAM_DATATIMELINELIST, HistogramDialog::OnHistogramDatatimelinelistSelected )

  EVT_CHOICE( ID_HISTOGRAM_3DTIMELINELIST, HistogramDialog::OnHistogram3dtimelinelistSelected )

  EVT_TOGGLEBUTTON( ID_HISTOGRAM_3DTIMELINEAUTOFIT, HistogramDialog::OnHistogram3dtimelineautofitClick )
  EVT_UPDATE_UI( ID_HISTOGRAM_3DTIMELINEAUTOFIT, HistogramDialog::OnHistogram3dtimelineautofitUpdate )

  EVT_RADIOBUTTON( ID_RADIOBUTTON_ALLWINDOW, HistogramDialog::OnRadiobuttonAllwindowSelected )
  EVT_UPDATE_UI( ID_RADIOBUTTON_ALLWINDOW, HistogramDialog::OnRadiobuttonAllwindowUpdate )

  EVT_RADIOBUTTON( ID_RADIOBUTTON_ALLTRACE, HistogramDialog::OnRadiobuttonAlltraceSelected )
  EVT_UPDATE_UI( ID_RADIOBUTTON_ALLTRACE, HistogramDialog::OnRadiobuttonAlltraceUpdate )

  EVT_RADIOBUTTON( ID_RADIOBUTTON_MANUAL, HistogramDialog::OnRadiobuttonManualSelected )
  EVT_UPDATE_UI( ID_RADIOBUTTON_MANUAL, HistogramDialog::OnRadiobuttonManualUpdate )

  EVT_TOGGLEBUTTON( ID_HISTOGRAM_BUTTONSELECT, HistogramDialog::OnHistogramButtonselectClick )
  EVT_UPDATE_UI( ID_HISTOGRAM_BUTTONSELECT, HistogramDialog::OnHistogramButtonselectUpdate )

  EVT_BUTTON( wxID_OK, HistogramDialog::OnOkClick )

////@end HistogramDialog event table entries

END_EVENT_TABLE()


/*!
 * HistogramDialog constructors
 */

HistogramDialog::HistogramDialog()
{
  Init();
}

HistogramDialog::HistogramDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}


/*!
 * HistogramDialog creator
 */

bool HistogramDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin HistogramDialog creation
  SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Centre();
////@end HistogramDialog creation
  return true;
}


/*!
 * HistogramDialog destructor
 */

HistogramDialog::~HistogramDialog()
{
////@begin HistogramDialog destruction
////@end HistogramDialog destruction
}


/*!
 * Member initialisation
 */

void HistogramDialog::Init()
{
////@begin HistogramDialog member initialisation
  controlTimelineAutofit = true;
  extraControlTimelineAutofit = true;
  listControlTimelines = NULL;
  buttonControlTimelineAutoFit = NULL;
  labelControlTimelineMin = NULL;
  txtControlTimelineMin = NULL;
  labelControlTimelineMax = NULL;
  txtControlTimelineMax = NULL;
  labelControlTimelineDelta = NULL;
  txtControlTimelineDelta = NULL;
  listDataTimelines = NULL;
  list3DTimelines = NULL;
  button3DTimelineAutoFit = NULL;
  label3DTimelineMin = NULL;
  txt3DTimelineMin = NULL;
  label3DTimelineMax = NULL;
  txt3DTimelineMax = NULL;
  label3DTimelineDelta = NULL;
  txt3DTimelineDelta = NULL;
  txtBeginTime = NULL;
  txtEndTime = NULL;
  radioAllWindow = NULL;
  radioAllTrace = NULL;
  radioManual = NULL;
  buttonSelect = NULL;
////@end HistogramDialog member initialisation
}


/*!
 * Control creation for HistogramDialog
 */

void HistogramDialog::CreateControls()
{    
////@begin HistogramDialog content construction
  HistogramDialog* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _(" Timeline Selection "));
  wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
  itemBoxSizer2->Add(itemStaticBoxSizer3, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer3->Add(itemBoxSizer4, 1, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Control Timeline"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer4->Add(itemStaticText5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString listControlTimelinesStrings;
  listControlTimelines = new wxChoice( itemDialog1, ID_HISTOGRAM_CONTROLTIMELINELIST, wxDefaultPosition, wxDefaultSize, listControlTimelinesStrings, 0 );
  itemBoxSizer4->Add(listControlTimelines, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  buttonControlTimelineAutoFit = new wxToggleButton( itemDialog1, ID_HISTOGRAM_CONTROLTIMELINEAUTOFIT, _("Auto Fit"), wxDefaultPosition, wxDefaultSize, 0 );
  buttonControlTimelineAutoFit->SetValue(true);
  itemBoxSizer4->Add(buttonControlTimelineAutoFit, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer3->Add(itemBoxSizer8, 1, wxGROW|wxALL, 5);

  labelControlTimelineMin = new wxStaticText( itemDialog1, wxID_STATIC, _("Min"), wxDefaultPosition, wxDefaultSize, 0 );
  labelControlTimelineMin->Enable(false);
  itemBoxSizer8->Add(labelControlTimelineMin, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txtControlTimelineMin = new wxTextCtrl( itemDialog1, ID_HISTOGRAM_CONTROLTIMELINEMIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  txtControlTimelineMin->Enable(false);
  itemBoxSizer8->Add(txtControlTimelineMin, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  labelControlTimelineMax = new wxStaticText( itemDialog1, wxID_STATIC, _("Max"), wxDefaultPosition, wxDefaultSize, 0 );
  labelControlTimelineMax->Enable(false);
  itemBoxSizer8->Add(labelControlTimelineMax, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txtControlTimelineMax = new wxTextCtrl( itemDialog1, ID_HISTOGRAM_CONTROLTIMELINEMAX, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  txtControlTimelineMax->Enable(false);
  itemBoxSizer8->Add(txtControlTimelineMax, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  labelControlTimelineDelta = new wxStaticText( itemDialog1, wxID_STATIC, _("Delta"), wxDefaultPosition, wxDefaultSize, 0 );
  labelControlTimelineDelta->Enable(false);
  itemBoxSizer8->Add(labelControlTimelineDelta, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txtControlTimelineDelta = new wxTextCtrl( itemDialog1, ID_HISTOGRAM_CONTROLTIMELINEDELTA, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  txtControlTimelineDelta->Enable(false);
  itemBoxSizer8->Add(txtControlTimelineDelta, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticLine* itemStaticLine15 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemStaticBoxSizer3->Add(itemStaticLine15, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer3->Add(itemBoxSizer16, 1, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText17 = new wxStaticText( itemDialog1, wxID_STATIC, _("Data Timeline"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer16->Add(itemStaticText17, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString listDataTimelinesStrings;
  listDataTimelines = new wxChoice( itemDialog1, ID_HISTOGRAM_DATATIMELINELIST, wxDefaultPosition, wxDefaultSize, listDataTimelinesStrings, 0 );
  itemBoxSizer16->Add(listDataTimelines, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  itemBoxSizer16->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticLine* itemStaticLine20 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemStaticBoxSizer3->Add(itemStaticLine20, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer3->Add(itemBoxSizer21, 1, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText22 = new wxStaticText( itemDialog1, wxID_STATIC, _("3D Timeline"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer21->Add(itemStaticText22, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString list3DTimelinesStrings;
  list3DTimelines = new wxChoice( itemDialog1, ID_HISTOGRAM_3DTIMELINELIST, wxDefaultPosition, wxDefaultSize, list3DTimelinesStrings, 0 );
  itemBoxSizer21->Add(list3DTimelines, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  button3DTimelineAutoFit = new wxToggleButton( itemDialog1, ID_HISTOGRAM_3DTIMELINEAUTOFIT, _("Auto Fit"), wxDefaultPosition, wxDefaultSize, 0 );
  button3DTimelineAutoFit->SetValue(true);
  itemBoxSizer21->Add(button3DTimelineAutoFit, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer3->Add(itemBoxSizer25, 1, wxGROW|wxALL, 5);

  label3DTimelineMin = new wxStaticText( itemDialog1, wxID_STATIC, _("Min"), wxDefaultPosition, wxDefaultSize, 0 );
  label3DTimelineMin->Enable(false);
  itemBoxSizer25->Add(label3DTimelineMin, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txt3DTimelineMin = new wxTextCtrl( itemDialog1, ID_HISTOGRAM_3DTIMELINEMIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  txt3DTimelineMin->Enable(false);
  itemBoxSizer25->Add(txt3DTimelineMin, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  label3DTimelineMax = new wxStaticText( itemDialog1, wxID_STATIC, _("Max"), wxDefaultPosition, wxDefaultSize, 0 );
  label3DTimelineMax->Enable(false);
  itemBoxSizer25->Add(label3DTimelineMax, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txt3DTimelineMax = new wxTextCtrl( itemDialog1, ID_HISTOGRAM_3DTIMELINEMAX, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  txt3DTimelineMax->Enable(false);
  itemBoxSizer25->Add(txt3DTimelineMax, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  label3DTimelineDelta = new wxStaticText( itemDialog1, wxID_STATIC, _("Delta"), wxDefaultPosition, wxDefaultSize, 0 );
  label3DTimelineDelta->Enable(false);
  itemBoxSizer25->Add(label3DTimelineDelta, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txt3DTimelineDelta = new wxTextCtrl( itemDialog1, ID_HISTOGRAM_3DTIMELINEDELTA, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  txt3DTimelineDelta->Enable(false);
  itemBoxSizer25->Add(txt3DTimelineDelta, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer32Static = new wxStaticBox(itemDialog1, wxID_ANY, _(" Time Range "));
  wxStaticBoxSizer* itemStaticBoxSizer32 = new wxStaticBoxSizer(itemStaticBoxSizer32Static, wxVERTICAL);
  itemBoxSizer2->Add(itemStaticBoxSizer32, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer33 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer32->Add(itemBoxSizer33, 0, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText34 = new wxStaticText( itemDialog1, wxID_STATIC, _("Begin"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer33->Add(itemStaticText34, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txtBeginTime = new wxTextCtrl( itemDialog1, ID_HISTOGRAM_BEGINTIME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  txtBeginTime->Enable(false);
  itemBoxSizer33->Add(txtBeginTime, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticText* itemStaticText36 = new wxStaticText( itemDialog1, wxID_STATIC, _("End"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer33->Add(itemStaticText36, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txtEndTime = new wxTextCtrl( itemDialog1, ID_HISTOGRAM_ENDTIME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  txtEndTime->Enable(false);
  itemBoxSizer33->Add(txtEndTime, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer38 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer32->Add(itemBoxSizer38, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer39 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer38->Add(itemBoxSizer39, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  radioAllWindow = new wxRadioButton( itemDialog1, ID_RADIOBUTTON_ALLWINDOW, _("Current Timeline"), wxDefaultPosition, wxDefaultSize, 0 );
  radioAllWindow->SetValue(true);
  itemBoxSizer39->Add(radioAllWindow, 0, wxALIGN_LEFT|wxALL, 5);

  radioAllTrace = new wxRadioButton( itemDialog1, ID_RADIOBUTTON_ALLTRACE, _("All Trace"), wxDefaultPosition, wxDefaultSize, 0 );
  radioAllTrace->SetValue(false);
  itemBoxSizer39->Add(radioAllTrace, 0, wxALIGN_LEFT|wxALL, 5);

  radioManual = new wxRadioButton( itemDialog1, ID_RADIOBUTTON_MANUAL, _("Manual Selection"), wxDefaultPosition, wxDefaultSize, 0 );
  radioManual->SetValue(false);
  itemBoxSizer39->Add(radioManual, 0, wxALIGN_LEFT|wxALL, 5);

  buttonSelect = new wxToggleButton( itemDialog1, ID_HISTOGRAM_BUTTONSELECT, _("Select..."), wxDefaultPosition, wxDefaultSize, 0 );
  buttonSelect->SetValue(false);
  buttonSelect->Enable(false);
  itemBoxSizer38->Add(buttonSelect, 0, wxALIGN_BOTTOM|wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer44 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer44, 0, wxALIGN_RIGHT|wxALL, 5);
  wxButton* itemButton45 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer44->AddButton(itemButton45);

  wxButton* itemButton46 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer44->AddButton(itemButton46);

  itemStdDialogButtonSizer44->Realize();

////@end HistogramDialog content construction

  // Filter forbidden chars
  txtControlTimelineMin->SetValidator( wxTextValidator( wxFILTER_NUMERIC ));
  txtControlTimelineMax->SetValidator( wxTextValidator( wxFILTER_NUMERIC ));
  txtControlTimelineDelta->SetValidator( wxTextValidator( wxFILTER_NUMERIC ));
  
  txt3DTimelineMin->SetValidator( wxTextValidator( wxFILTER_NUMERIC ));
  txt3DTimelineMax->SetValidator( wxTextValidator( wxFILTER_NUMERIC ));
  txt3DTimelineDelta->SetValidator( wxTextValidator( wxFILTER_NUMERIC ));
}


/*!
 * Should we show tooltips?
 */

bool HistogramDialog::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap HistogramDialog::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin HistogramDialog bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
////@end HistogramDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon HistogramDialog::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin HistogramDialog icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end HistogramDialog icon retrieval
}


void HistogramDialog::getSelectedWindowID( wxChoice * listWidget,
                                           vector< TWindowID > &selection,
                                           bool listWithFirstVoidOption )
{
  if ( listWidget->GetCurrentSelection() != wxNOT_FOUND )
  {
    if ( listWithFirstVoidOption ) // NONE of the Xtra
    {
      if ( listWidget->GetCurrentSelection() != 0)  // NONE selected?
      {
        TWindowID tmp = selection[ listWidget->GetCurrentSelection() - 1 ];
        selection.clear();
        selection.push_back( tmp );
      }
      else
      {
        selection.clear(); // NONE selected
      }
    }
    else
    {
      TWindowID tmp = selection[ listWidget->GetCurrentSelection() ];
      selection.clear();
      selection.push_back( tmp );
    }
  }
  else
    selection.clear();
}

bool HistogramDialog::TransferDataFromWindow()
{
  // Copy Selected window
  string errorMessage = "";
  double tmp;
  TWindowID auxID;

  if ( !GetControlTimelineAutofit() )
  {
    if ( txtControlTimelineMin->GetValue().ToDouble( &tmp ) )
      controlTimelineMin = tmp;
    else
    {
      auxID = controlTimelines[ listControlTimelines->GetCurrentSelection() ];
      controlTimelineMin = LoadedWindows::getInstance()->getWindow( auxID )->getMinimumY();
      errorMessage = "\tControl Timeline Minimum : " + formatNumber( controlTimelineMin ) + "\n";
    }

    if ( txtControlTimelineMax->GetValue().ToDouble( &tmp ) )
      controlTimelineMax = tmp;
    else
    {
      auxID = controlTimelines[ listControlTimelines->GetCurrentSelection() ];
      controlTimelineMax = LoadedWindows::getInstance()->getWindow( auxID )->getMaximumY();
      errorMessage += "\tControl Timeline Maximum : " + formatNumber( controlTimelineMax ) + "\n";
    }

    if ( txtControlTimelineDelta->GetValue().ToDouble( &tmp ) )
      controlTimelineDelta = tmp;
    else
    {
      controlTimelineDelta = computeDelta( controlTimelineMin, controlTimelineMax );
      errorMessage += "\tControl Timeline Delta : " + formatNumber( controlTimelineDelta ) + "\n";
    }
  }

  getSelectedWindowID( listControlTimelines, controlTimelines, false );

  getSelectedWindowID( listDataTimelines, dataTimelines, false );

  getSelectedWindowID( list3DTimelines, extraControlTimelines, true );


  if ( list3DTimelines->GetCurrentSelection() != 0 )
  {
    if ( !GetExtraControlTimelineAutofit() )
    {
      if ( txt3DTimelineMin->GetValue().ToDouble( &tmp ) )
        extraControlTimelineMin = tmp;
      else
      {
        auxID = extraControlTimelines[ list3DTimelines->GetCurrentSelection() - 1 ];
        extraControlTimelineMin = LoadedWindows::getInstance()->getWindow( auxID )->getMinimumY();
        errorMessage += "\t3D Timeline Min : " + formatNumber( extraControlTimelineMin ) + "\n";
      }

      if ( txt3DTimelineMax->GetValue().ToDouble( &tmp ) )
        extraControlTimelineMax = tmp;
      else
      {
        auxID = extraControlTimelines[ list3DTimelines->GetCurrentSelection() - 1 ];
        extraControlTimelineMax = LoadedWindows::getInstance()->getWindow( auxID )->getMaximumY();
        errorMessage += "\t3D Timeline Max : " + formatNumber( extraControlTimelineMax ) + "\n";
      }

      if ( txt3DTimelineDelta->GetValue().ToDouble( &tmp ) )
        extraControlTimelineDelta = tmp;
      else
      {
        extraControlTimelineDelta = computeDelta( extraControlTimelineMin, extraControlTimelineMax );
        errorMessage += "\t3D Timeline Delta : " +  formatNumber( extraControlTimelineDelta ) + "\n";
      }
    }
  }
  else
    extraControlTimelines.clear();

  timeRange.clear();

  TRecordTime auxBegin, auxEnd;
  if ( txtBeginTime->GetValue().ToDouble( &tmp ) )
    auxBegin = tmp;
  else
  {
    auxID = controlTimelines[ listControlTimelines->GetCurrentSelection() ];
    auxBegin = LoadedWindows::getInstance()->getWindow( auxID )->getWindowBeginTime();
    errorMessage += "\tBegin Time : " + formatNumber( auxBegin ) + "\n";
  }

  if ( txtEndTime->GetValue().ToDouble( &tmp ) )
    auxEnd = tmp;
  else
  {
    auxID  = controlTimelines[ listControlTimelines->GetCurrentSelection() ];
    auxEnd = LoadedWindows::getInstance()->getWindow( auxID )->getWindowEndTime();
    errorMessage += "\tEnd Time : " + formatNumber( auxEnd ) + "\n";
  }

  timeRange.push_back( make_pair( auxBegin, auxEnd ) );

  if ( errorMessage != "" )
  {
    string prefix = "Following substitutions will be applied:\n\n";
    wxMessageDialog message( this,
                             wxT( prefix + errorMessage ) ,
                            "Conversion problem",
                             wxOK | wxICON_EXCLAMATION );
    message.ShowModal();
  }

  return true;
}



/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void HistogramDialog::OnOkClick( wxCommandEvent& event )
{
  if ( TransferDataFromWindow() )
    EndModal( wxID_OK );
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_HISTOGRAM_CONTROLTIMELINEAUTOFIT
 */

void HistogramDialog::OnHistogramControltimelineautofitClick( wxCommandEvent& event )
{
  SetControlTimelineAutofit( buttonControlTimelineAutoFit->GetValue() );
}

void HistogramDialog::updateControlTimelineAutofit()
{
  TSemanticValue min, max, delta;

  Window *aux = LoadedWindows::getInstance()->getWindow( controlTimelines[ listControlTimelines->GetCurrentSelection() ] );
  computeColumns( aux, min, max, delta );

  txtControlTimelineMin->SetValue( formatNumber( min ) );
  txtControlTimelineMax->SetValue( formatNumber( max ) );
  txtControlTimelineDelta->SetValue( formatNumber( delta ) );
}

void HistogramDialog::updateExtraControlTimelineAutofit()
{
  TSemanticValue min, max, delta;

  if ( list3DTimelines->GetCurrentSelection() == 0 )
  {
    txt3DTimelineMin->Clear();
    txt3DTimelineMax->Clear();
    txt3DTimelineDelta->Clear();

    button3DTimelineAutoFit->Enable( false );
    button3DTimelineAutoFit->SetValue( true );
    enable3DFields( false );
  }
  else
  {
    TWindowID auxID = extraControlTimelines[ list3DTimelines->GetCurrentSelection() - 1 ];
    Window *auxWindow = LoadedWindows::getInstance()->getWindow( auxID );
    computeColumns( auxWindow, min, max, delta );

    txt3DTimelineMin->SetValue( formatNumber( min ) );
    txt3DTimelineMax->SetValue( formatNumber( max ) );
    txt3DTimelineDelta->SetValue( formatNumber( delta ) );

    button3DTimelineAutoFit->Enable( true );
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_HISTOGRAM_CONTROLTIMELINEAUTOFIT
 */

void HistogramDialog::OnHistogramControltimelineautofitUpdate( wxUpdateUIEvent& event )
{
  bool autofit = !buttonControlTimelineAutoFit->GetValue();

  labelControlTimelineMin->Enable( autofit );
  txtControlTimelineMin->Enable( autofit );

  labelControlTimelineMax->Enable( autofit );
  txtControlTimelineMax->Enable( autofit );

  labelControlTimelineDelta->Enable( autofit );
  txtControlTimelineDelta->Enable( autofit );
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_HISTOGRAM_3DTIMELINEAUTOFIT
 */

void HistogramDialog::OnHistogram3dtimelineautofitClick( wxCommandEvent& event )
{
  SetExtraControlTimelineAutofit( button3DTimelineAutoFit->GetValue() );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_HISTOGRAM_3DTIMELINEAUTOFIT
 */

void HistogramDialog::enable3DFields( bool autofit )
{
  label3DTimelineMin->Enable( autofit );
  txt3DTimelineMin->Enable( autofit );

  label3DTimelineMax->Enable( autofit );
  txt3DTimelineMax->Enable( autofit );

  label3DTimelineDelta->Enable( autofit );
  txt3DTimelineDelta->Enable( autofit );
}


void HistogramDialog::OnHistogram3dtimelineautofitUpdate( wxUpdateUIEvent& event )
{
  enable3DFields( !button3DTimelineAutoFit->GetValue() );
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_HISTOGRAM_BUTTONSELECT
 */

void HistogramDialog::OnHistogramButtonselectClick( wxCommandEvent& event )
{
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_HISTOGRAM_BUTTONSELECT
 */

void HistogramDialog::OnHistogramButtonselectUpdate( wxUpdateUIEvent& event )
{
  buttonSelect->Enable( radioManual->GetValue() );
}


wxString HistogramDialog::formatNumber( double value )
{
  stringstream auxSStr;
  wxString auxNumber;

  locale mylocale("");
  auxSStr.imbue( mylocale );
  auxSStr.precision( ParaverConfig::getInstance()->getPrecision() );
  auxSStr << fixed;
  auxSStr << value;
  auxNumber << auxSStr.str();

  return auxNumber;
}


TSemanticValue HistogramDialog::computeDelta( TSemanticValue min, TSemanticValue max )
{
  TSemanticValue delta;

  if ( max - min < ( TSemanticValue )1.0 )
    delta = ( max - min ) / ParaverConfig::getInstance()->getHistoNumColumns();
  else if ( max - min <= ParaverConfig::getInstance()->getHistoNumColumns() )
    delta = (double )1.0;
  else
    delta = ( max - min ) / ParaverConfig::getInstance()->getHistoNumColumns();

  return delta;
}


void HistogramDialog::computeColumns( Window *current, TSemanticValue &min, TSemanticValue &max, TSemanticValue &delta )
{
  min = current->getMinimumY();
  max = current->getMaximumY();

  delta = computeDelta( min, max );
}

// if current = NULL, PUT none
UINT32 HistogramDialog::fillList( Window *current, vector< TWindowID > listTimelines, wxChoice *listWidget )
{
  Window *aux;
  int pos = 0;
  int posSelected = 0;

  if ( current == NULL )
    listWidget->Append( wxString( "- - - None - - -" ) );

  for( vector< TWindowID >::iterator it = listTimelines.begin(); it != listTimelines.end(); ++it )
  {
    // Fill control timeline list widget
    aux = LoadedWindows::getInstance()->getWindow( *it );
    listWidget->Append( wxString( aux->getName().c_str() ) );

    // Find its position in that list
    if ( aux == current )
      posSelected = pos;
    pos++;
  }

  if ( current == NULL )
    posSelected = 0;

  return posSelected;
}



bool HistogramDialog::TransferDataToWindow( Window *current )
{
  UINT32 pos;

  // Fill Control Timelines choice list
  pos = fillList( current, controlTimelines, listControlTimelines );
  listControlTimelines->Select( pos );

  // Set Control Timeline Min, Max and Delta
  TSemanticValue min, max, delta;

  computeColumns( current, min, max, delta );

  txtControlTimelineMin->SetValue( formatNumber( min ) );
  txtControlTimelineMax->SetValue( formatNumber(  max ) );
  txtControlTimelineDelta->SetValue( formatNumber( delta ) );

  // Fill Data Timelines choice list
  LoadedWindows::getInstance()->getValidDataWindow( current, NULL, dataTimelines );
  pos = fillList( current, dataTimelines, listDataTimelines ); // here selected is also the current one
  listDataTimelines->Select( pos );

  // Fill 3D Control Timelines choice list
  Window *auxDataWindow = LoadedWindows::getInstance()->getWindow( dataTimelines[ pos ] );
  LoadedWindows::getInstance()->getValidControlWindow( auxDataWindow, current, extraControlTimelines );
  pos = fillList( NULL, extraControlTimelines, list3DTimelines );
  list3DTimelines->Select( pos );

  button3DTimelineAutoFit->Enable( false );

  txtBeginTime->SetValue( formatNumber( timeRange[ WINDOW_RANGE ].first ) );
  txtEndTime->SetValue( formatNumber( timeRange[ WINDOW_RANGE ].second ) );

  return true;
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_ALLWINDOW
 */

void HistogramDialog::OnRadiobuttonAllwindowSelected( wxCommandEvent& event )
{
  txtBeginTime->SetValue( formatNumber( timeRange[ WINDOW_RANGE ].first ) );
  txtEndTime->SetValue( formatNumber( timeRange[ WINDOW_RANGE ].second ) );
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_ALLTRACE
 */

void HistogramDialog::OnRadiobuttonAlltraceSelected( wxCommandEvent& event )
{
  txtBeginTime->SetValue( formatNumber( timeRange[ TRACE_RANGE ].first ) );
  txtEndTime->SetValue( formatNumber( timeRange[ TRACE_RANGE ].second ) );
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_MANUAL
 */

void HistogramDialog::OnRadiobuttonManualSelected( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_MANUAL in HistogramDialog.
  // Before editing this code, remove the block markers.
  event.Skip();
////@end wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_MANUAL in HistogramDialog. 
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_RADIOBUTTON_ALLWINDOW
 */

void HistogramDialog::OnRadiobuttonAllwindowUpdate( wxUpdateUIEvent& event )
{
  radioAllWindow->Enable( !timeRange.empty() );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_RADIOBUTTON_ALLTRACE
 */

void HistogramDialog::OnRadiobuttonAlltraceUpdate( wxUpdateUIEvent& event )
{
  radioAllTrace->Enable( !timeRange.empty() );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_RADIOBUTTON_MANUAL
 */

void HistogramDialog::OnRadiobuttonManualUpdate( wxUpdateUIEvent& event )
{
  bool active = radioManual->GetValue();
  txtBeginTime->Enable( active );
  txtEndTime->Enable( active );
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_HISTOGRAM_CONTROLTIMELINELIST
 */

void HistogramDialog::OnHistogramControltimelinelistSelected( wxCommandEvent& event )
{
  updateControlTimelineAutofit();
}

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_HISTOGRAM_DATATIMELINELIST
 */

void HistogramDialog::OnHistogramDatatimelinelistSelected( wxCommandEvent& event )
{
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_HISTOGRAM_3DTIMELINELIST
 */

void HistogramDialog::OnHistogram3dtimelinelistSelected( wxCommandEvent& event )
{
  updateExtraControlTimelineAutofit();
}



