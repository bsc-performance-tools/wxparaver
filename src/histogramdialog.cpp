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
////@end includes

#include <sstream>
#include "histogramdialog.h"
#include "paraverconfig.h"
#include "labelconstructor.h"
#include "wxparaverapp.h"
#include "timelinetreeselector.h"
// #include "histogram.h"

// PREFERENCES
//#include <wx/notebook.h>
#include "preferencesdialog.h"
////@begin XPM images
#include "../icons/three_dots.xpm"
////@end XPM images

using namespace std;

/*!
 * HistogramDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( HistogramDialog, wxDialog )


/*!
 * HistogramDialog event table definition
 */

BEGIN_EVENT_TABLE( HistogramDialog, wxDialog )

////@begin HistogramDialog event table entries
  EVT_IDLE( HistogramDialog::OnIdle )
  EVT_BUTTON( ID_HISTOGRAM_CONTROLTIMELINEBUTTON, HistogramDialog::OnHistogramControltimelinebuttonClick )
  EVT_TOGGLEBUTTON( ID_HISTOGRAM_CONTROLTIMELINEAUTOFIT, HistogramDialog::OnHistogramControltimelineautofitClick )
  EVT_UPDATE_UI( ID_HISTOGRAM_CONTROLTIMELINEAUTOFIT, HistogramDialog::OnHistogramControltimelineautofitUpdate )
  EVT_BUTTON( ID_HISTOGRAM_DATATIMELINEBUTTON, HistogramDialog::OnHistogramDatatimelinebuttonClick )
  EVT_BUTTON( ID_HISTOGRAM_3DTIMELINEBUTTON, HistogramDialog::OnHistogram3dtimelinebuttonClick )
  EVT_TOGGLEBUTTON( ID_HISTOGRAM_3DTIMELINEAUTOFIT, HistogramDialog::OnHistogram3dtimelineautofitClick )
  EVT_UPDATE_UI( ID_HISTOGRAM_3DTIMELINEAUTOFIT, HistogramDialog::OnHistogram3dtimelineautofitUpdate )
  EVT_RADIOBUTTON( ID_RADIOBUTTON_ALLWINDOW, HistogramDialog::OnRadiobuttonAllwindowSelected )
  EVT_UPDATE_UI( ID_RADIOBUTTON_ALLWINDOW, HistogramDialog::OnRadiobuttonAllwindowUpdate )
  EVT_RADIOBUTTON( ID_RADIOBUTTON_ALLTRACE, HistogramDialog::OnRadiobuttonAlltraceSelected )
  EVT_UPDATE_UI( ID_RADIOBUTTON_ALLTRACE, HistogramDialog::OnRadiobuttonAlltraceUpdate )
  EVT_RADIOBUTTON( ID_RADIOBUTTON_MANUAL, HistogramDialog::OnRadiobuttonManualSelected )
  EVT_UPDATE_UI( ID_RADIOBUTTON_MANUAL, HistogramDialog::OnRadiobuttonManualUpdate )
  EVT_BUTTON( ID_HISTOGRAM_BUTTONSELECT, HistogramDialog::OnHistogramButtonselectClick )
  EVT_UPDATE_UI( ID_HISTOGRAM_BUTTONSELECT, HistogramDialog::OnHistogramButtonselectUpdate )
  EVT_BUTTON( wxID_CANCEL, HistogramDialog::OnCancelClick )
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
  controlTimelineSelected = nullptr;
  currentWindow = nullptr;
  dataTimelineSelected = nullptr;
  extraControlTimelineAutofit = true;
  extraControlTimelineSelected = nullptr;
  waitingGlobalTiming = false;
  txtControlTimelines = NULL;
  buttonControlTimelines = NULL;
  buttonControlTimelineAutoFit = NULL;
  labelControlTimelineMin = NULL;
  txtControlTimelineMin = NULL;
  labelControlTimelineMax = NULL;
  txtControlTimelineMax = NULL;
  labelControlTimelineDelta = NULL;
  txtControlTimelineDelta = NULL;
  txtDataTimelines = NULL;
  buttonDataTimelines = NULL;
  txt3DTimelines = NULL;
  button3DTimelines = NULL;
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

  txtControlTimelines = new wxTextCtrl( itemDialog1, ID_HISTOGRAM_CONTROLTIMELINETEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
  itemBoxSizer4->Add(txtControlTimelines, 2, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  buttonControlTimelines = new wxBitmapButton( itemDialog1, ID_HISTOGRAM_CONTROLTIMELINEBUTTON, itemDialog1->GetBitmapResource(wxT("icons/three_dots.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer4->Add(buttonControlTimelines, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

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

  txtDataTimelines = new wxTextCtrl( itemDialog1, ID_HISTOGRAM_DATATIMELINETEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
  itemBoxSizer16->Add(txtDataTimelines, 2, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  buttonDataTimelines = new wxBitmapButton( itemDialog1, ID_HISTOGRAM_DATATIMELINEBUTTON, itemDialog1->GetBitmapResource(wxT("icons/three_dots.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer16->Add(buttonDataTimelines, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  itemBoxSizer16->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticLine* itemStaticLine20 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemStaticBoxSizer3->Add(itemStaticLine20, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer3->Add(itemBoxSizer21, 1, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText22 = new wxStaticText( itemDialog1, wxID_STATIC, _("3D Timeline"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer21->Add(itemStaticText22, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  txt3DTimelines = new wxTextCtrl( itemDialog1, ID_HISTOGRAM_3DTIMELINETEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
  itemBoxSizer21->Add(txt3DTimelines, 2, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  button3DTimelines = new wxBitmapButton( itemDialog1, ID_HISTOGRAM_3DTIMELINEBUTTON, itemDialog1->GetBitmapResource(wxT("icons/three_dots.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer21->Add(button3DTimelines, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

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

  buttonSelect = new wxButton( itemDialog1, ID_HISTOGRAM_BUTTONSELECT, _("Select..."), wxDefaultPosition, wxDefaultSize, 0 );
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

  controlTimelineAutofit = ParaverConfig::getInstance()->getHistogramAutofitControlScale();
  buttonControlTimelineAutoFit->SetValue( controlTimelineAutofit );
  extraControlTimelineAutofit = ParaverConfig::getInstance()->getHistogramAutofitThirdDimensionScale();
  button3DTimelineAutoFit->SetValue( extraControlTimelineAutofit ); 

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
  if (name == wxT("icons/three_dots.xpm"))
  {
    wxBitmap bitmap(three_dots_xpm);
    return bitmap;
  }
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


bool HistogramDialog::TransferDataFromWindow()
{
  // Copy Selected window
  wxString errorMessage = _( "" );
  double tmp;

  if ( !GetControlTimelineAutofit() )
  {
    if ( txtControlTimelineMin->GetValue().ToDouble( &tmp ) )
      controlTimelineMin = tmp;
    else
    {
      controlTimelineMin = controlTimelineSelected->getMinimumY();
      errorMessage = _( "\tControl Timeline Minimum : " ) + formatNumber( controlTimelineMin ) + _( "\n" );
    }

    if ( txtControlTimelineMax->GetValue().ToDouble( &tmp ) )
      controlTimelineMax = tmp;
    else
    {
      controlTimelineMax = controlTimelineSelected->getMaximumY();
      errorMessage += _( "\tControl Timeline Maximum : " ) + formatNumber( controlTimelineMax ) + _( "\n" );
    }

    if ( txtControlTimelineDelta->GetValue().ToDouble( &tmp ) )
      controlTimelineDelta = tmp;
    else
    {
      controlTimelineDelta = computeDelta( controlTimelineMin, controlTimelineMax );
      errorMessage += _( "\tControl Timeline Delta : " ) + formatNumber( controlTimelineDelta ) + _( "\n" );
    }
  }


  if ( extraControlTimelineSelected != nullptr )
  {
    if ( !GetExtraControlTimelineAutofit() )
    {
      if ( txt3DTimelineMin->GetValue().ToDouble( &tmp ) )
        extraControlTimelineMin = tmp;
      else
      {
        extraControlTimelineMin = extraControlTimelineSelected->getMinimumY();
        errorMessage += _( "\t3D Timeline Min : " ) + formatNumber( extraControlTimelineMin ) + _( "\n" );
      }

      if ( txt3DTimelineMax->GetValue().ToDouble( &tmp ) )
        extraControlTimelineMax = tmp;
      else
      {
        extraControlTimelineMax = extraControlTimelineSelected->getMaximumY();
        errorMessage += _( "\t3D Timeline Max : " ) + formatNumber( extraControlTimelineMax ) + _( "\n" );
      }

      if ( txt3DTimelineDelta->GetValue().ToDouble( &tmp ) )
        extraControlTimelineDelta = tmp;
      else
      {
        extraControlTimelineDelta = computeDelta( extraControlTimelineMin, extraControlTimelineMax );
        errorMessage += _( "\t3D Timeline Delta : " ) +  formatNumber( extraControlTimelineDelta ) + _( "\n" );
      }
    }
  }
  else
    extraControlTimelines.clear();

  timeRange.clear();

  TRecordTime auxBegin, auxEnd;

  bool done = LabelConstructor::getTimeValue( std::string( txtBeginTime->GetValue().mb_str() ),
                                              controlTimelineSelected->getTimeUnit(),
                                              ParaverConfig::getInstance()->getTimelinePrecision(),
                                              auxBegin );
  if ( !done )
  {
    if( radioAllTrace->GetValue() )
      auxBegin = 0.0;
    else
      auxBegin = controlTimelineSelected->getWindowBeginTime();
    errorMessage += _( "\tBegin Time : " ) + formatNumber( auxBegin ) + _( "\n" );
  }
  else
    auxBegin = controlTimelineSelected->windowUnitsToTraceUnits( auxBegin );

  done = LabelConstructor::getTimeValue( std::string( txtEndTime->GetValue().mb_str() ),
                                         controlTimelineSelected->getTimeUnit(),
                                         ParaverConfig::getInstance()->getTimelinePrecision(),
                                         auxEnd );
  if ( !done )
  {
    if( radioAllTrace->GetValue() )
      auxEnd = controlTimelineSelected->getTrace()->getEndTime();
    else
      auxEnd = controlTimelineSelected->getWindowEndTime();
    errorMessage += _( "\tEnd Time : " ) + formatNumber( auxEnd ) + _( "\n" );
  }
  else
    auxEnd = controlTimelineSelected->windowUnitsToTraceUnits( auxEnd );

  timeRange.push_back( make_pair( auxBegin, auxEnd ) );

  if ( errorMessage != _( "" ) )
  {
    wxString prefix = _( "Following substitutions will be applied:\n\n" );
    wxMessageDialog message( this,
                             prefix + errorMessage,
                             _( "Conversion problem" ),
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
  {
    EndModal( wxID_OK );
  }
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

  computeColumns( controlTimelineSelected, min, max, delta );

  txtControlTimelineMin->SetValue( formatNumber( min ) );
  txtControlTimelineMax->SetValue( formatNumber( max ) );
  txtControlTimelineDelta->SetValue( formatNumber( delta ) );
}

void HistogramDialog::updateExtraControlTimelineAutofit()
{
  TSemanticValue min, max, delta;

  if ( extraControlTimelineSelected == nullptr )
  {
    txt3DTimelineMin->Clear();
    txt3DTimelineMax->Clear();
    txt3DTimelineDelta->Clear();

    button3DTimelineAutoFit->Enable( false );
    button3DTimelineAutoFit->SetValue( extraControlTimelineAutofit );
    enable3DFields( false );
  }
  else
  {
    computeColumns( extraControlTimelineSelected, min, max, delta );

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
  wxGetApp().ActivateGlobalTiming( this );
  waitingGlobalTiming = true;
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

  try
  {
    auxSStr.imbue( std::locale( "" ) );
  }
  catch(...)
  {
    auxSStr.imbue( std::locale::classic() );
  }
  auxSStr.precision( ParaverConfig::getInstance()->getHistogramPrecision() );
  auxSStr << fixed;
  auxSStr << value;
  auxNumber << wxString::FromUTF8( auxSStr.str().c_str() );

  return auxNumber;
}


TSemanticValue HistogramDialog::computeDelta( TSemanticValue min, TSemanticValue max )
{
  TSemanticValue delta;

  if ( max - min < ( TSemanticValue )1.0 )
    delta = ( max - min ) / ParaverConfig::getInstance()->getHistogramNumColumns();
  else if ( max - min <= ParaverConfig::getInstance()->getHistogramNumColumns() )
    delta = (double )1.0;
  else
    delta = ( max - min ) / ParaverConfig::getInstance()->getHistogramNumColumns();

  return delta;
}


void HistogramDialog::computeColumns( Timeline *current, TSemanticValue &min, TSemanticValue &max, TSemanticValue &delta )
{
  min = current->getMinimumY();
  max = current->getMaximumY();

  delta = computeDelta( min, max );
}

// if current = nullptr, PUT none
PRV_UINT32 HistogramDialog::fillList( Timeline *current, vector< TWindowID > listTimelines, wxChoice *listWidget )
{
  Timeline *aux;
  int pos = 0;
  int posSelected = 0;

  if ( current == nullptr )
    listWidget->Append( _( "- - - None - - -" ) );

  for( vector< TWindowID >::iterator it = listTimelines.begin(); it != listTimelines.end(); ++it )
  {
    // Fill control timeline list widget
    aux = LoadedWindows::getInstance()->getWindow( *it );
    listWidget->Append( wxString::FromUTF8( aux->getName().c_str() ) );

    // Find its position in that list
    if ( aux == current )
      posSelected = pos;
    pos++;
  }

  if ( current == nullptr )
    posSelected = 0;

  return posSelected;
}

bool HistogramDialog::TransferDataToWindow( Timeline *current )
{
  PRV_UINT32 pos;
  currentWindow = current;
  controlTimelineSelected = current;
  dataTimelineSelected = current;

  txtControlTimelines->SetValue( wxString( current->getName().c_str(), wxConvUTF8 ) );
  // Set Control Timeline Min, Max and Delta
  TSemanticValue min, max, delta;

  computeColumns( current, min, max, delta );

  txtControlTimelineMin->SetValue( formatNumber( min ) );
  txtControlTimelineMax->SetValue( formatNumber(  max ) );
  txtControlTimelineDelta->SetValue( formatNumber( delta ) );

  buttonControlTimelineAutoFit->SetValue( controlTimelineAutofit );

  LoadedWindows::getInstance()->getValidDataWindow( current, nullptr, dataTimelines );
  txtDataTimelines->SetValue( wxString( current->getName().c_str(), wxConvUTF8 ) );
  
  LoadedWindows::getInstance()->getValidControlWindow( current, current, extraControlTimelines );
  txt3DTimelines->SetValue( wxT( "None" ) );

  button3DTimelineAutoFit->SetValue( extraControlTimelineAutofit );
  button3DTimelineAutoFit->Enable( false );

  txtBeginTime->SetValue(
    wxString::FromUTF8( LabelConstructor::timeLabel( current->traceUnitsToWindowUnits( timeRange[ static_cast< int >( TTimeRangeSource::WINDOW_RANGE ) ].first ),
                                                      current->getTimeUnit(),
                                                      ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() )
  );

  txtEndTime->SetValue(
    wxString::FromUTF8( LabelConstructor::timeLabel( current->traceUnitsToWindowUnits( timeRange[ static_cast< int >( TTimeRangeSource::WINDOW_RANGE ) ].second ),
                                                      current->getTimeUnit(),
                                                      ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() )
  );

  return true;
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_ALLWINDOW
 */

void HistogramDialog::OnRadiobuttonAllwindowSelected( wxCommandEvent& event )
{
  Timeline *current = controlTimelineSelected;

  txtBeginTime->SetValue(
    wxString::FromUTF8( LabelConstructor::timeLabel( current->traceUnitsToWindowUnits( timeRange[ static_cast< int >( TTimeRangeSource::WINDOW_RANGE ) ].first ),
                                                     current->getTimeUnit(),
                                                     ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() )
  );

  txtEndTime->SetValue(
    wxString::FromUTF8( LabelConstructor::timeLabel( current->traceUnitsToWindowUnits( timeRange[ static_cast< int >( TTimeRangeSource::WINDOW_RANGE ) ].second ),
                                                     current->getTimeUnit(),
                                                     ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() )
  );
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_ALLTRACE
 */

void HistogramDialog::OnRadiobuttonAlltraceSelected( wxCommandEvent& event )
{
  Timeline *current = controlTimelineSelected;

  txtBeginTime->SetValue(
    wxString::FromUTF8( LabelConstructor::timeLabel( current->traceUnitsToWindowUnits( timeRange[ static_cast< int >( TTimeRangeSource::TRACE_RANGE ) ].first ),
                                                     current->getTimeUnit(),
                                                     ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() )
  );

  txtEndTime->SetValue(
    wxString::FromUTF8( LabelConstructor::timeLabel( current->traceUnitsToWindowUnits( timeRange[ static_cast< int >( TTimeRangeSource::TRACE_RANGE ) ].second ),
                                                     current->getTimeUnit(),
                                                     ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() )
  );
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_MANUAL
 */

void HistogramDialog::OnRadiobuttonManualSelected( wxCommandEvent& event )
{
  event.Skip();
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
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void HistogramDialog::OnCancelClick( wxCommandEvent& event )
{
  EndModal( wxID_CANCEL );
}


/*!
 * wxEVT_IDLE event handler for ID_HISTOGRAMDIALOG
 */

void HistogramDialog::OnIdle( wxIdleEvent& event )
{
  if( waitingGlobalTiming )
  {
    Timeline *current = controlTimelineSelected;
    txtBeginTime->SetValue(
      wxString::FromUTF8( LabelConstructor::timeLabel( current->traceUnitsToWindowUnits( wxGetApp().GetGlobalTimingBegin() ),
                                                        current->getTimeUnit(),
                                                        ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() )
    );

    txtEndTime->SetValue(
      wxString::FromUTF8( LabelConstructor::timeLabel( current->traceUnitsToWindowUnits( wxGetApp().GetGlobalTimingEnd() ),
                                                        current->getTimeUnit(),
                                                        ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() )
    );
    
    if( !wxGetApp().GetGlobalTiming() )
      waitingGlobalTiming = false;
  }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_HISTOGRAM_CONTROLTIMELINEBUTTON
 */

void HistogramDialog::OnHistogramControltimelinebuttonClick( wxCommandEvent& event )
{
  TimelineTreeSelector timelineSelector( this,
                                         wxID_ANY,
                                         wxT( "Control Timeline" ),
                                         controlTimelines,
                                         controlTimelineSelected,
                                         controlTimelineSelected->getTrace(),
                                         false );
  timelineSelector.Move( wxGetMousePosition() );
  
  int retCode = timelineSelector.ShowModal();
  if( retCode == wxID_OK )
  {
    if( controlTimelineSelected == timelineSelector.getSelection() )
      return;
    controlTimelineSelected = timelineSelector.getSelection();
    txtControlTimelines->SetValue( wxString( controlTimelineSelected->getName().c_str(), wxConvUTF8 ) );
    updateControlTimelineAutofit();
  }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_HISTOGRAM_DATATIMELINEBUTTON
 */

void HistogramDialog::OnHistogramDatatimelinebuttonClick( wxCommandEvent& event )
{
  TimelineTreeSelector timelineSelector( this,
                                         wxID_ANY,
                                         wxT( "Data Timeline" ),
                                         dataTimelines,
                                         dataTimelineSelected,
                                         dataTimelineSelected->getTrace(),
                                         false );
  timelineSelector.Move( wxGetMousePosition() );
  
  int retCode = timelineSelector.ShowModal();
  if( retCode == wxID_OK )
  {
    if( dataTimelineSelected == timelineSelector.getSelection() )
      return;
    dataTimelineSelected = timelineSelector.getSelection();
    txtDataTimelines->SetValue( wxString( dataTimelineSelected->getName().c_str(), wxConvUTF8 ) );
  }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_HISTOGRAM_3DTIMELINEBUTTON
 */

void HistogramDialog::OnHistogram3dtimelinebuttonClick( wxCommandEvent& event )
{
  TimelineTreeSelector timelineSelector( this,
                                         wxID_ANY,
                                         wxT( "3D Timeline" ),
                                         extraControlTimelines,
                                         extraControlTimelineSelected,
                                         extraControlTimelineSelected == nullptr ? controlTimelineSelected->getTrace() : extraControlTimelineSelected->getTrace(),
                                         true );
  timelineSelector.Move( wxGetMousePosition() );
  
  int retCode = timelineSelector.ShowModal();
  if( retCode == wxID_OK )
  {
    if( extraControlTimelineSelected == timelineSelector.getSelection() )
      return;
    extraControlTimelineSelected = timelineSelector.getSelection();
    if( extraControlTimelineSelected == nullptr )
      txt3DTimelines->SetValue( wxT( "None" ) );
    else
    {
      txt3DTimelines->SetValue( wxString( extraControlTimelineSelected->getName().c_str(), wxConvUTF8 ) );
      updateExtraControlTimelineAutofit();
    }
  }
}

