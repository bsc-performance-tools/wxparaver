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
////@end includes

#include <sstream>
#include "histogramdialog.h"
#include "paraverconfig.h"
#include "labelconstructor.h"
#include "wxparaverapp.h"
// #include "histogram.h"

// pREFERENCES
//#include <wx/notebook.h>
#include "preferencesdialog.h"
////@begin XPM images
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
  extraControlTimelineAutofit = true;
  waitingGlobalTiming = false;
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
  // Generated by DialogBlocks, 04/07/2012 16:30:14 (unregistered)

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
  wxString errorMessage = _( "" );
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
      errorMessage = _( "\tControl Timeline Minimum : " ) + formatNumber( controlTimelineMin ) + _( "\n" );
    }

    if ( txtControlTimelineMax->GetValue().ToDouble( &tmp ) )
      controlTimelineMax = tmp;
    else
    {
      auxID = controlTimelines[ listControlTimelines->GetCurrentSelection() ];
      controlTimelineMax = LoadedWindows::getInstance()->getWindow( auxID )->getMaximumY();
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
        errorMessage += _( "\t3D Timeline Min : " ) + formatNumber( extraControlTimelineMin ) + _( "\n" );
      }

      if ( txt3DTimelineMax->GetValue().ToDouble( &tmp ) )
        extraControlTimelineMax = tmp;
      else
      {
        auxID = extraControlTimelines[ list3DTimelines->GetCurrentSelection() - 1 ];
        extraControlTimelineMax = LoadedWindows::getInstance()->getWindow( auxID )->getMaximumY();
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

  auxID = controlTimelines[ listControlTimelines->GetCurrentSelection() ];
  bool done = LabelConstructor::getTimeValue( std::string( txtBeginTime->GetValue().mb_str() ),
                                              LoadedWindows::getInstance()->getWindow( auxID )->getTimeUnit(),
                                              ParaverConfig::getInstance()->getTimelinePrecision(),
                                              auxBegin );
  if ( !done )
  {
    if( radioAllTrace->GetValue() )
      auxBegin = 0.0;
    else
      auxBegin = LoadedWindows::getInstance()->getWindow( auxID )->getWindowBeginTime();
    errorMessage += _( "\tBegin Time : " ) + formatNumber( auxBegin ) + _( "\n" );
  }
  else
    auxBegin = LoadedWindows::getInstance()->getWindow( auxID )->windowUnitsToTraceUnits( auxBegin );

  done = LabelConstructor::getTimeValue( std::string( txtEndTime->GetValue().mb_str() ),
                                         LoadedWindows::getInstance()->getWindow( auxID )->getTimeUnit(),
                                         ParaverConfig::getInstance()->getTimelinePrecision(),
                                         auxEnd );
  if ( !done )
  {
    if( radioAllTrace->GetValue() )
      auxEnd = LoadedWindows::getInstance()->getWindow( auxID )->getTrace()->getEndTime();
    else
      auxEnd = LoadedWindows::getInstance()->getWindow( auxID )->getWindowEndTime();
    errorMessage += _( "\tEnd Time : " ) + formatNumber( auxEnd ) + _( "\n" );
  }
  else
    auxEnd = LoadedWindows::getInstance()->getWindow( auxID )->windowUnitsToTraceUnits( auxEnd );

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

  getSelectedWindowID( listControlTimelines, controlTimelines, false );

  getSelectedWindowID( listDataTimelines, dataTimelines, false );

  getSelectedWindowID( list3DTimelines, extraControlTimelines, true );


  return true;
}



/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void HistogramDialog::OnOkClick( wxCommandEvent& event )
{
  if ( TransferDataFromWindow() )
  {
    MakeModal( false );
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
    //button3DTimelineAutoFit->SetValue( true );
    button3DTimelineAutoFit->SetValue( extraControlTimelineAutofit );
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

/*
PreferencesDialog::PreferencesDialog( wxWindow* parent,
                 wxWindowID id,
                 const wxString& title)
{
  SetSheetStyle(wxPROPSHEET_NOTEBOOK);
  Create(parent, id, title);
}

PreferencesDialog::~PreferencesDialog()
{}

bool PreferencesDialog::Create( wxWindow* parent,
                 wxWindowID id,
                 const wxString& title,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style,
                 const wxString& name )
{


  if (!wxPropertySheetDialog::Create( parent, id, title ))
    return false;

  cout << "PREFERENCES DIALOG CREATED!" << endl;
  CreateButtons( wxOK | wxCANCEL | wxHELP );


//  wxNotebookPage* mypage = new wxNotebookPage();
//  wxNotebook* mypanel = new wxNotebook();
//  mypanel->AddPage( mypage, wxT("General"), true);
//  GetBookCtrl()->AddPage( mypanel, wxT("General"));


  wxBookCtrlBase* notebook = GetBookCtrl();
  wxPanel *panel1 = new wxPanel( notebook );

  notebook->AddPage( panel1, _("General"), true);

  LayoutDialog();
  ShowModal();

  return true;
}

*/


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

  locale mylocale( "" );
  auxSStr.imbue( mylocale );
  auxSStr.precision( ParaverConfig::getInstance()->getHistogramPrecision() );
  auxSStr << fixed;
  auxSStr << value;
  auxNumber << wxString::FromAscii( auxSStr.str().c_str() );

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


void HistogramDialog::computeColumns( Window *current, TSemanticValue &min, TSemanticValue &max, TSemanticValue &delta )
{
  min = current->getMinimumY();
  max = current->getMaximumY();

  delta = computeDelta( min, max );
}

// if current = NULL, PUT none
PRV_UINT32 HistogramDialog::fillList( Window *current, vector< TWindowID > listTimelines, wxChoice *listWidget )
{
  Window *aux;
  int pos = 0;
  int posSelected = 0;

  if ( current == NULL )
    listWidget->Append( _( "- - - None - - -" ) );

  for( vector< TWindowID >::iterator it = listTimelines.begin(); it != listTimelines.end(); ++it )
  {
    // Fill control timeline list widget
    aux = LoadedWindows::getInstance()->getWindow( *it );
    listWidget->Append( wxString::FromAscii( aux->getName().c_str() ) );

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
  PRV_UINT32 pos;

  // Fill Control Timelines choice list
  pos = fillList( current, controlTimelines, listControlTimelines );
  listControlTimelines->Select( pos );

  // Set Control Timeline Min, Max and Delta
  TSemanticValue min, max, delta;

  computeColumns( current, min, max, delta );

  txtControlTimelineMin->SetValue( formatNumber( min ) );
  txtControlTimelineMax->SetValue( formatNumber(  max ) );
  txtControlTimelineDelta->SetValue( formatNumber( delta ) );

  buttonControlTimelineAutoFit->SetValue( controlTimelineAutofit );

  // Fill Data Timelines choice list
  LoadedWindows::getInstance()->getValidDataWindow( current, NULL, dataTimelines );
  pos = fillList( current, dataTimelines, listDataTimelines ); // here selected is also the current one
  listDataTimelines->Select( pos );

  // Fill 3D Control Timelines choice list
  Window *auxDataWindow = LoadedWindows::getInstance()->getWindow( dataTimelines[ pos ] );
  LoadedWindows::getInstance()->getValidControlWindow( auxDataWindow, current, extraControlTimelines );
  pos = fillList( NULL, extraControlTimelines, list3DTimelines );
  list3DTimelines->Select( pos );

  button3DTimelineAutoFit->SetValue( extraControlTimelineAutofit );
  button3DTimelineAutoFit->Enable( false );

  txtBeginTime->SetValue(
    wxString::FromAscii( LabelConstructor::timeLabel( current->traceUnitsToWindowUnits( timeRange[ WINDOW_RANGE ].first ),
                                                      current->getTimeUnit(),
                                                      ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() )
  );

  txtEndTime->SetValue(
    wxString::FromAscii( LabelConstructor::timeLabel( current->traceUnitsToWindowUnits( timeRange[ WINDOW_RANGE ].second ),
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
  Window *current = LoadedWindows::getInstance()->getWindow( controlTimelines[ listControlTimelines->GetCurrentSelection() ] );

  txtBeginTime->SetValue(
    wxString::FromAscii( LabelConstructor::timeLabel( current->traceUnitsToWindowUnits( timeRange[ WINDOW_RANGE ].first ),
                                                      current->getTimeUnit(),
                                                      ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() )
  );

  txtEndTime->SetValue(
    wxString::FromAscii( LabelConstructor::timeLabel( current->traceUnitsToWindowUnits( timeRange[ WINDOW_RANGE ].second ),
                                                      current->getTimeUnit(),
                                                      ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() )
  );
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_ALLTRACE
 */

void HistogramDialog::OnRadiobuttonAlltraceSelected( wxCommandEvent& event )
{
  Window *current = LoadedWindows::getInstance()->getWindow( controlTimelines[ listControlTimelines->GetCurrentSelection() ] );

  txtBeginTime->SetValue(
    wxString::FromAscii( LabelConstructor::timeLabel( current->traceUnitsToWindowUnits( timeRange[ TRACE_RANGE ].first ),
                                                      current->getTimeUnit(),
                                                      ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() )
  );

  txtEndTime->SetValue(
    wxString::FromAscii( LabelConstructor::timeLabel( current->traceUnitsToWindowUnits( timeRange[ TRACE_RANGE ].second ),
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




/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void HistogramDialog::OnCancelClick( wxCommandEvent& event )
{
  MakeModal( false );
  EndModal( wxID_CANCEL );
}


/*!
 * wxEVT_IDLE event handler for ID_HISTOGRAMDIALOG
 */

void HistogramDialog::OnIdle( wxIdleEvent& event )
{
  if( waitingGlobalTiming )
  {
    Window *current = LoadedWindows::getInstance()->getWindow( controlTimelines[ listControlTimelines->GetCurrentSelection() ] );
    txtBeginTime->SetValue(
      wxString::FromAscii( LabelConstructor::timeLabel( current->traceUnitsToWindowUnits( wxGetApp().GetGlobalTimingBegin() ),
                                                        current->getTimeUnit(),
                                                        ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() )
    );

    txtEndTime->SetValue(
      wxString::FromAscii( LabelConstructor::timeLabel( current->traceUnitsToWindowUnits( wxGetApp().GetGlobalTimingEnd() ),
                                                        current->getTimeUnit(),
                                                        ParaverConfig::getInstance()->getTimelinePrecision() ).c_str() )
    );
    
    if( !wxGetApp().GetGlobalTiming() )
      waitingGlobalTiming = false;
  }
}

