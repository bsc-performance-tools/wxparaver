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

#include "derivedtimelinedialog.h"
#include <wx/tokenzr.h>
#include "timelinetreeselector.h"

////@begin XPM images
////@end XPM images

using namespace std;

/*!
 * DerivedTimelineDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( DerivedTimelineDialog, wxDialog )


/*!
 * DerivedTimelineDialog event table definition
 */

BEGIN_EVENT_TABLE( DerivedTimelineDialog, wxDialog )

////@begin DerivedTimelineDialog event table entries
  EVT_CHOICE( ID_TOPCOMPOSE1, DerivedTimelineDialog::OnTopcompose1Selected )
  EVT_CHOICE( ID_TOPCOMPOSE2, DerivedTimelineDialog::OnTopcompose2Selected )
  EVT_BUTTON( ID_TIMELINES_BUTTON1, DerivedTimelineDialog::OnTimelinesButton1Click )
  EVT_CHOICE( ID_OPERATIONS, DerivedTimelineDialog::OnOperationsSelected )
  EVT_BUTTON( ID_TIMELINES_BUTTON2, DerivedTimelineDialog::OnTimelinesButton2Click )
  EVT_BUTTON( ID_SWAP_WINDOWS, DerivedTimelineDialog::OnSwapWindowsClick )
  EVT_BUTTON( wxID_OK, DerivedTimelineDialog::OnOkClick )
////@end DerivedTimelineDialog event table entries

END_EVENT_TABLE()


/*!
 * DerivedTimelineDialog constructors
 */

DerivedTimelineDialog::DerivedTimelineDialog()
{
  Init();
}

DerivedTimelineDialog::DerivedTimelineDialog(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
  Init();
  Create(parent, id, caption, pos, size, style);
  Fit();
}


/*!
 * derivedTimelineDialog creator
 */

bool DerivedTimelineDialog::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
////@begin DerivedTimelineDialog creation
  SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Centre();
////@end DerivedTimelineDialog creation

  Fit();

  return true;
}


/*!
 * DerivedTimelineDialog destructor
 */

DerivedTimelineDialog::~DerivedTimelineDialog()
{
////@begin DerivedTimelineDialog destruction
////@end DerivedTimelineDialog destruction
}


/*!
 * Member initialisation
 */

void DerivedTimelineDialog::Init()
{
////@begin DerivedTimelineDialog member initialisation
  currentWindow1 = nullptr;
  currentWindow2 = nullptr;
  factorTimeline1 = 1.0;
  factorTimeline2 = 1.0;
  lastTimeline1 = 0;
  lastTimeline2 = 0;
  shiftTimeline1 = 0;
  shiftTimeline2 = 0;
  widgetName = nullptr;
  widgetTopCompose1 = nullptr;
  widgetLabelMinCompose1 = nullptr;
  widgetMinCompose1 = nullptr;
  widgetLabelMaxCompose1 = nullptr;
  widgetMaxCompose1 = nullptr;
  widgetTopCompose2 = nullptr;
  widgetLabelMinCompose2 = nullptr;
  widgetMinCompose2 = nullptr;
  widgetLabelMaxCompose2 = nullptr;
  widgetMaxCompose2 = nullptr;
  widgetLabelTimelines1 = nullptr;
  widgetLabelTimelines2 = nullptr;
  widgetFactorTimeline1 = nullptr;
  txtTimelines1 = nullptr;
  buttonTimelines1 = nullptr;
  widgetOperations = nullptr;
  txtTimelines2 = nullptr;
  buttonTimelines2 = nullptr;
  widgetFactorTimeline2 = nullptr;
  swapWindowsButton = nullptr;
////@end DerivedTimelineDialog member initialisation
}


/*!
 * Control creation for derivedTimelineDialog
 */

void DerivedTimelineDialog::CreateControls()
{    
////@begin DerivedTimelineDialog content construction
  DerivedTimelineDialog* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Name"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
  itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  widgetName = new wxTextCtrl( itemDialog1, ID_DERIVED_NAME, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0 );
  itemBoxSizer3->Add(widgetName, 1, wxGROW|wxALL, 5);

  wxStaticLine* itemStaticLine6 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemBoxSizer2->Add(itemStaticLine6, 0, wxGROW|wxLEFT|wxRIGHT, 5);

  wxStaticBox* itemStaticBoxSizer7Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Top Composes"));
  wxStaticBoxSizer* itemStaticBoxSizer7 = new wxStaticBoxSizer(itemStaticBoxSizer7Static, wxVERTICAL);
  itemBoxSizer2->Add(itemStaticBoxSizer7, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer7->Add(itemBoxSizer8, 1, wxGROW|wxLEFT|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer8->Add(itemBoxSizer9, 1, wxALIGN_CENTER_VERTICAL, 5);

  wxStaticText* itemStaticText10 = new wxStaticText( itemDialog1, wxID_STATIC, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer9->Add(itemStaticText10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString widgetTopCompose1Strings;
  widgetTopCompose1Strings.Add(_("Dummy text"));
  widgetTopCompose1 = new wxChoice( itemDialog1, ID_TOPCOMPOSE1, wxDefaultPosition, wxDefaultSize, widgetTopCompose1Strings, wxFULL_REPAINT_ON_RESIZE );
  widgetTopCompose1->SetStringSelection(_("Dummy text"));
  itemBoxSizer9->Add(widgetTopCompose1, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer8->Add(itemBoxSizer12, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer12->Add(itemBoxSizer13, 1, wxGROW|wxLEFT|wxBOTTOM, 5);

  widgetLabelMinCompose1 = new wxStaticText( itemDialog1, wxID_STATIC, _("Min"), wxDefaultPosition, wxDefaultSize, 0 );
  widgetLabelMinCompose1->Enable(false);
  itemBoxSizer13->Add(widgetLabelMinCompose1, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  widgetMinCompose1 = new wxTextCtrl( itemDialog1, ID_MINCOMPOSE1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  widgetMinCompose1->Enable(false);
  itemBoxSizer13->Add(widgetMinCompose1, 2, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer12->Add(itemBoxSizer16, 1, wxGROW|wxLEFT|wxBOTTOM, 5);

  widgetLabelMaxCompose1 = new wxStaticText( itemDialog1, wxID_STATIC, _("Max"), wxDefaultPosition, wxDefaultSize, 0 );
  widgetLabelMaxCompose1->Enable(false);
  itemBoxSizer16->Add(widgetLabelMaxCompose1, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

  widgetMaxCompose1 = new wxTextCtrl( itemDialog1, ID_MAXCOMPOSE1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  widgetMaxCompose1->Enable(false);
  itemBoxSizer16->Add(widgetMaxCompose1, 2, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxStaticLine* itemStaticLine19 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemStaticBoxSizer7->Add(itemStaticLine19, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer7->Add(itemBoxSizer20, 1, wxGROW|wxLEFT|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer20->Add(itemBoxSizer21, 1, wxALIGN_CENTER_VERTICAL, 5);

  wxStaticText* itemStaticText22 = new wxStaticText( itemDialog1, wxID_STATIC, _("2"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer21->Add(itemStaticText22, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString widgetTopCompose2Strings;
  widgetTopCompose2Strings.Add(_("Dummy text"));
  widgetTopCompose2 = new wxChoice( itemDialog1, ID_TOPCOMPOSE2, wxDefaultPosition, wxDefaultSize, widgetTopCompose2Strings, wxFULL_REPAINT_ON_RESIZE );
  widgetTopCompose2->SetStringSelection(_("Dummy text"));
  itemBoxSizer21->Add(widgetTopCompose2, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer24 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer20->Add(itemBoxSizer24, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer24->Add(itemBoxSizer25, 1, wxGROW|wxLEFT|wxBOTTOM, 5);

  widgetLabelMinCompose2 = new wxStaticText( itemDialog1, wxID_STATIC, _("Min"), wxDefaultPosition, wxDefaultSize, 0 );
  widgetLabelMinCompose2->Enable(false);
  itemBoxSizer25->Add(widgetLabelMinCompose2, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

  widgetMinCompose2 = new wxTextCtrl( itemDialog1, ID_MINCOMPOSE2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  widgetMinCompose2->Enable(false);
  itemBoxSizer25->Add(widgetMinCompose2, 2, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxBoxSizer* itemBoxSizer28 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer24->Add(itemBoxSizer28, 1, wxGROW|wxLEFT|wxBOTTOM, 5);

  widgetLabelMaxCompose2 = new wxStaticText( itemDialog1, wxID_STATIC, _("Max"), wxDefaultPosition, wxDefaultSize, 0 );
  widgetLabelMaxCompose2->Enable(false);
  itemBoxSizer28->Add(widgetLabelMaxCompose2, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

  widgetMaxCompose2 = new wxTextCtrl( itemDialog1, ID_MAXCOMPOSE2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  widgetMaxCompose2->Enable(false);
  itemBoxSizer28->Add(widgetMaxCompose2, 2, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer4, 1, wxGROW, 5);

  wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer4->Add(itemBoxSizer1, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer1->Add(itemBoxSizer5, 1, wxALIGN_LEFT|wxALL, 5);

  wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Factor"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer5->Add(itemStaticText6, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer1->Add(itemBoxSizer6, 1, wxALIGN_LEFT|wxALL, 5);

  wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("Shift"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer6->Add(itemStaticText7, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer1->Add(itemBoxSizer7, 1, wxALIGN_LEFT|wxALL, 5);

  widgetLabelTimelines1 = new wxStaticText( itemDialog1, wxID_STATIC, _("Timeline "), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer7->Add(widgetLabelTimelines1, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer1->Add(itemBoxSizer15, 1, wxALIGN_LEFT|wxALL, 5);

  wxStaticText* itemStaticText16 = new wxStaticText( itemDialog1, wxID_STATIC, _("Operation"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer15->Add(itemStaticText16, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer1->Add(itemBoxSizer17, 1, wxALIGN_LEFT|wxALL, 5);

  widgetLabelTimelines2 = new wxStaticText( itemDialog1, wxID_STATIC, _("Timeline"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer17->Add(widgetLabelTimelines2, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer1->Add(itemBoxSizer23, 1, wxALIGN_LEFT|wxALL, 5);

  wxStaticText* itemStaticText24 = new wxStaticText( itemDialog1, wxID_STATIC, _("Shift"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer23->Add(itemStaticText24, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer1->Add(itemBoxSizer19, 1, wxALIGN_LEFT|wxALL, 5);

  wxStaticText* itemStaticText20 = new wxStaticText( itemDialog1, wxID_STATIC, _("Factor"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer19->Add(itemStaticText20, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer4->Add(itemBoxSizer10, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer10->Add(itemBoxSizer11, 1, wxGROW|wxRIGHT|wxTOP|wxBOTTOM, 5);

  widgetFactorTimeline1 = new wxTextCtrl( itemDialog1, ID_FACTOR_TIMELINE_1, _("1.0"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer11->Add(widgetFactorTimeline1, 5, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer27 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer10->Add(itemBoxSizer27, 1, wxGROW|wxRIGHT|wxTOP|wxBOTTOM, 5);

  spinShiftTimeline1 = new wxSpinCtrl( itemDialog1, ID_SHIFT_TIMELINE1, wxT("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -10, 10, 0 );
  itemBoxSizer27->Add(spinShiftTimeline1, 5, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer10->Add(itemBoxSizer14, 1, wxGROW|wxRIGHT|wxTOP|wxBOTTOM, 5);

  txtTimelines1 = new wxTextCtrl( itemDialog1, ID_TIMELINES_TEXT1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
  itemBoxSizer14->Add(txtTimelines1, 5, wxALIGN_CENTER_VERTICAL, 5);

  buttonTimelines1 = new wxButton( itemDialog1, ID_TIMELINES_BUTTON1, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
  itemBoxSizer14->Add(buttonTimelines1, 0, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer10->Add(itemBoxSizer18, 1, wxGROW|wxRIGHT|wxTOP|wxBOTTOM, 5);

  wxArrayString widgetOperationsStrings;
  widgetOperations = new wxChoice( itemDialog1, ID_OPERATIONS, wxDefaultPosition, wxDefaultSize, widgetOperationsStrings, wxFULL_REPAINT_ON_RESIZE );
  itemBoxSizer18->Add(widgetOperations, 5, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer10->Add(itemBoxSizer22, 1, wxGROW|wxRIGHT|wxTOP|wxBOTTOM, 5);

  txtTimelines2 = new wxTextCtrl( itemDialog1, ID_TIMELINES_TEXT2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
  itemBoxSizer22->Add(txtTimelines2, 5, wxALIGN_CENTER_VERTICAL, 5);

  buttonTimelines2 = new wxButton( itemDialog1, ID_TIMELINES_BUTTON2, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
  itemBoxSizer22->Add(buttonTimelines2, 0, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer29 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer10->Add(itemBoxSizer29, 1, wxGROW|wxRIGHT|wxTOP|wxBOTTOM, 5);

  spinShiftTimeline2 = new wxSpinCtrl( itemDialog1, ID_SHIFT_TIMELINE2, wxT("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -10, 10, 0 );
  itemBoxSizer29->Add(spinShiftTimeline2, 5, wxALIGN_CENTER_VERTICAL, 5);

  wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer10->Add(itemBoxSizer26, 1, wxGROW|wxRIGHT|wxTOP|wxBOTTOM, 5);

  widgetFactorTimeline2 = new wxTextCtrl( itemDialog1, ID_FACTOR_TIMELINE_2, _("1.0"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer26->Add(widgetFactorTimeline2, 5, wxALIGN_CENTER_VERTICAL, 5);

  wxStaticLine* itemStaticLine47 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemBoxSizer2->Add(itemStaticLine47, 0, wxGROW|wxALL, 5);

  swapWindowsButton = new wxButton( itemDialog1, ID_SWAP_WINDOWS, _("S&wap Windows"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer2->Add(swapWindowsButton, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxStaticLine* itemStaticLine49 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemBoxSizer2->Add(itemStaticLine49, 0, wxGROW|wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer50 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer50, 0, wxALIGN_RIGHT|wxALL, 5);
  wxButton* itemButton51 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer50->AddButton(itemButton51);

  wxButton* itemButton52 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer50->AddButton(itemButton52);

  itemStdDialogButtonSizer50->Realize();

////@end DerivedTimelineDialog content construction

  widgetLabelMinCompose1->Show( false );
  widgetMinCompose1->Show( false );
  widgetLabelMaxCompose1->Show( false );
  widgetMaxCompose1->Show( false );


  widgetLabelMinCompose2->Show( false );
  widgetMinCompose2->Show( false );
  widgetLabelMaxCompose2->Show( false );
  widgetMaxCompose2->Show( false );
}


/*!
 * Should we show tooltips?
 */

bool DerivedTimelineDialog::ShowToolTips()
{
  return true;
}


bool DerivedTimelineDialog::TransferDataToWindow()
{
  if ( currentWindow1 == nullptr )
    currentWindow1 = LoadedWindows::getInstance()->getWindow( timelines1[ 0 ] );
  if ( currentWindow2 == nullptr )
  {
    for( vector<TWindowID>::const_iterator it = timelines2.begin(); it != timelines2.end(); ++it )
    {
      if( Window::compatibleLevels( currentWindow1, LoadedWindows::getInstance()->getWindow( *it ) ) )
      {
        currentWindow2 = LoadedWindows::getInstance()->getWindow( *it );
        break;
      }
    }
  }

  presetNameField( timelineName, widgetName );

  // Retrieve list of top compose operations, build and select first operation
  currentWindow1->getGroupLabels( 0, topCompose1 );
  topCompose2 = topCompose1;
  presetStringChoiceBox( topCompose1, widgetTopCompose1 );
  presetStringChoiceBox( topCompose2, widgetTopCompose2 );

  // Build lists of timelines and set the selected ones
  //presetTimelineComboBox( timelines1, currentWindow1, widgetTimelines1, lastTimeline1 );
  //presetTimelineComboBox( timelines2, currentWindow2, widgetTimelines2, lastTimeline2 );
  txtTimelines1->SetValue( wxString( currentWindow1->getName().c_str(), wxConvUTF8 ) );
  txtTimelines2->SetValue( wxString( currentWindow2->getName().c_str(), wxConvUTF8 ) );

  // Retrieve list of operations, build and select first operation
  currentWindow1->getGroupLabels( 1, operations );
  presetStringChoiceBox( operations, widgetOperations );
  widgetOperations->SetStringSelection( _( "product" ) );

  // Fill factor fields
  presetFactorField( factorTimeline1, widgetFactorTimeline1 );
  presetFactorField( factorTimeline2, widgetFactorTimeline2 );

  return true;
}


bool DerivedTimelineDialog::getParameterCompose( wxTextCtrl *field,
                                                 TParamValue &parameter,
                                                 wxString prefixMessage )
{
  if ( field->IsShown() )
    if ( !getParameterComposeField( field, parameter ) )
    {
      wxString fullMessage = prefixMessage +
                             _(" parameter void or type mismatch.\nPlease use decimal format." );
      wxMessageDialog message( this, 
                               fullMessage,
                               _( "Error in Field" ), wxOK );
      message.ShowModal();
      return false;
    }

  return true;
}


bool DerivedTimelineDialog::TransferDataFromWindow()
{
  TParamValue paramsMinCompose1, paramsMaxCompose1;
  TParamValue paramsMinCompose2, paramsMaxCompose2;

  if ( !getParameterCompose( widgetMinCompose1, paramsMinCompose1, _( "Compose 1" )))
    return false;

  if ( !getParameterCompose( widgetMaxCompose1, paramsMaxCompose1, _( "Compose 1" )))
    return false;

  if ( !getParameterCompose( widgetMinCompose2, paramsMinCompose2, _( "Compose 2" )))
    return false;

  if ( !getParameterCompose( widgetMaxCompose2, paramsMaxCompose2, _( "Compose 2" )))
    return false;

  if ( !getFactorFields( factorTimeline1, factorTimeline2 ) )
    return false;
  else 
  {
      // If factors are ok, then retrieve all the other data
    getName( widgetName, timelineName );

    getSelectedString( widgetTopCompose1, topCompose1 );
    getSelectedString( widgetTopCompose2, topCompose2 );
    getSelectedString( widgetOperations, operations );

    //getSelectedWindow( widgetTimelines1, timelines1 );
    //getSelectedWindow( widgetTimelines2, timelines2 );

    // and then assign previous gathered parameters of composes
    if ( widgetMinCompose1->IsShown() )
      minCompose1 = paramsMinCompose1;
    else if ( widgetMaxCompose1->IsShown() )
      maxCompose1 = paramsMaxCompose1;
    else if ( widgetMinCompose2->IsShown() )
      minCompose2 = paramsMinCompose2;
    else if ( widgetMaxCompose2->IsShown() )
      maxCompose2 = paramsMaxCompose2;

    shiftTimeline1 = spinShiftTimeline1->GetValue();
    shiftTimeline2 = spinShiftTimeline2->GetValue();
  }

  return true;
}


/*!
 * Get bitmap resources
 */

wxBitmap DerivedTimelineDialog::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin DerivedTimelineDialog bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
////@end DerivedTimelineDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon DerivedTimelineDialog::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin DerivedTimelineDialog icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end DerivedTimelineDialog icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void DerivedTimelineDialog::OnOkClick( wxCommandEvent& event )
{
  if ( TransferDataFromWindow() )
    EndModal( wxID_OK );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SWAP_WINDOWS
 */

void DerivedTimelineDialog::OnSwapWindowsClick( wxCommandEvent& event )
{
  if ( getFactorFields( factorTimeline2, factorTimeline1 ) ) // Change performed through parameter order 
  {
    // Swap factors
    presetFactorField( factorTimeline1, widgetFactorTimeline1 );
    presetFactorField( factorTimeline2, widgetFactorTimeline2 );

    // Swap window vectors
    vector<TWindowID> auxIDs = timelines1;
    timelines1 = timelines2;
    timelines2 = auxIDs;

    txtTimelines2->SetValue( wxString( currentWindow1->getName().c_str(), wxConvUTF8 ) );
    txtTimelines1->SetValue( wxString( currentWindow2->getName().c_str(), wxConvUTF8 ) );
    
    Window *auxWin = currentWindow1;
    currentWindow1 = currentWindow2;
    currentWindow2 = auxWin;
  }
}


// Build first list of timelines and set the selected one
void DerivedTimelineDialog::presetTimelineComboBox( vector< Window * > timelines,
                                                    Window *currentWindow,
                                                    wxComboBox *comboBox,
                                                    int& currentSelection )
{
  for( vector<Window *>::iterator it = timelines.begin(); it != timelines.end(); ++it )
    comboBox->Append( wxString::FromUTF8( (*it)->getName().c_str() ) );

  currentSelection = 0;
  for( vector<Window *>::iterator it = timelines.begin(); it != timelines.end(); ++it )
  {
    if ( *it != currentWindow )
      ++currentSelection;
    else
      break;
  }

  comboBox->Select( currentSelection );
}


void DerivedTimelineDialog::presetStringChoiceBox( vector< string > list,
                                                   wxChoice *choiceBox )
{
  wxString aux;
  choiceBox->Clear();
  
  for( vector< string >::iterator it = list.begin(); it != list.end(); ++it )
  {
    aux << wxString::FromUTF8( (*it).c_str() );
    choiceBox->Append( aux );
    aux.clear();
  }

  choiceBox->Select( 0 );
}

void DerivedTimelineDialog::presetFactorField( double value, wxTextCtrl *field )
{
  wxString auxFactor;

  auxFactor << (float)value;
  field->SetValue( auxFactor );
}

void DerivedTimelineDialog::presetNameField( string whichName, wxTextCtrl *field )
{
  wxString auxName;

  auxName << wxString::FromUTF8( whichName.c_str() );
  field->SetValue( auxName );
}

void DerivedTimelineDialog::getSelectedString( wxChoice *choiceBox,
                                               vector< string > &selection ) const
{
  string tmp = selection[ choiceBox->GetCurrentSelection() ];
  selection.clear();
  selection.push_back( tmp );
}


void DerivedTimelineDialog::getSelectedWindow( wxComboBox *comboBox,
                                               vector< Window * > &selection ) const
{
  Window* tmp = selection[ comboBox->GetCurrentSelection() ];
  selection.clear();
  selection.push_back( tmp );
}

void DerivedTimelineDialog::getName( wxTextCtrl *field,
                                     string &whichName ) const
{
  whichName = string( field->GetValue().mb_str() );
}


bool DerivedTimelineDialog::getFactorFields( double &whichFactor1,
                                             double &whichFactor2 )
{
  double auxFactor1, auxFactor2;
  
  // Checks both factors for mismatch
  if ( !widgetFactorTimeline1->GetValue().ToDouble( &auxFactor1 ))
  {
    wxMessageDialog message( this, 
                             _( "Factor 1 void or type mismatch.\nPlease use decimal format." ),
                             _( "Error in Field" ), wxOK );
    message.ShowModal();
    return false;
  }
  else if ( !widgetFactorTimeline2->GetValue().ToDouble( &auxFactor2 ))
  {
    wxMessageDialog message( this, 
                             _( "Factor 2 void or type mismatch.\nPlease use decimal format." ),
                             _( "Error in Field" ), wxOK );
    message.ShowModal();
    return false;
  }

  // Only if both of them are ok then retrieve data
  whichFactor1 = auxFactor1;
  whichFactor2 = auxFactor2;

  return true;
}


void DerivedTimelineDialog::setParametersCompose( PRV_UINT32 compose,
                                                  string nameFunction,
                                                  PRV_UINT32 numParameters,
                                                  vector< string > namesParameters,
                                                  vector< TParamValue > defaultValues )
{
  if ( compose == 0 )
  {
    if ( numParameters == 0 )
    {
      widgetLabelMinCompose1->Show( false );
      widgetMinCompose1->Show( false );
      widgetLabelMaxCompose1->Show( false );
      widgetMaxCompose1->Show( false );
      
      widgetLabelMinCompose1->Enable( false );
      widgetMinCompose1->Enable( false );
      widgetLabelMaxCompose1->Enable( false );
      widgetMaxCompose1->Enable( false );
    }
    else if ( numParameters == 1 )
    {
      widgetLabelMinCompose1->Show( true );
      widgetMinCompose1->Show( true );
      widgetLabelMaxCompose1->Show( false );
      widgetMaxCompose1->Show( false );

      wxString aux1;

      aux1 << wxString::FromUTF8( namesParameters[ 0 ].c_str() );
      widgetLabelMinCompose1->SetLabel( aux1 );

      setParameterComposeField( defaultValues[ 0 ], widgetMinCompose1 );

      widgetLabelMinCompose1->Enable( true );
      widgetMinCompose1->Enable( true );
      widgetLabelMaxCompose1->Enable( false );
      widgetMaxCompose1->Enable( false );
    }
    else
    {
      widgetLabelMinCompose1->Show( true );
      widgetMinCompose1->Show( true );
      widgetLabelMaxCompose1->Show( true );
      widgetMaxCompose1->Show( true );

      wxString aux1, aux2;

      aux1 << wxString::FromUTF8( namesParameters[ 0 ].c_str() );
      aux2 << wxString::FromUTF8( namesParameters[ 1 ].c_str() );
      widgetLabelMinCompose1->SetLabel( aux1 );
      widgetLabelMaxCompose1->SetLabel( aux2 );

      setParameterComposeField( defaultValues[ 0 ], widgetMinCompose1 );
      setParameterComposeField( defaultValues[ 1 ], widgetMaxCompose1 );

      widgetLabelMinCompose1->Enable( true );
      widgetMinCompose1->Enable( true );
      widgetLabelMaxCompose1->Enable( true );
      widgetMaxCompose1->Enable( true );
    }
  }
  else
  {
    if ( numParameters == 0 )
    {
      widgetLabelMinCompose2->Show( false );
      widgetMinCompose2->Show( false );
      widgetLabelMaxCompose2->Show( false );
      widgetMaxCompose2->Show( false );

      widgetLabelMinCompose2->Enable( false );
      widgetMinCompose2->Enable( false );
      widgetLabelMaxCompose2->Enable( false );
      widgetMaxCompose2->Enable( false );
    }
    else if ( numParameters == 1 )
    {
      widgetLabelMinCompose2->Show( true );
      widgetMinCompose2->Show( true );
      widgetLabelMaxCompose2->Show( false );
      widgetMaxCompose2->Show( false );

      wxString aux1;

      aux1 << wxString::FromUTF8( namesParameters[ 0 ].c_str() );
      widgetLabelMinCompose2->SetLabel( aux1 );

      setParameterComposeField( defaultValues[ 0 ], widgetMinCompose2 );

      widgetLabelMinCompose2->Enable( true );
      widgetMinCompose2->Enable( true );
      widgetLabelMaxCompose2->Enable( false );
      widgetMaxCompose2->Enable( false );
    }
    else
    {
      widgetLabelMinCompose2->Show( true );
      widgetMinCompose2->Show( true );
      widgetLabelMaxCompose2->Show( true );
      widgetMaxCompose2->Show( true );

      wxString aux1, aux2;

      aux1 << wxString::FromUTF8( namesParameters[ 0 ].c_str() );
      aux2 << wxString::FromUTF8( namesParameters[ 1 ].c_str() );
      widgetLabelMinCompose2->SetLabel( aux1 );
      widgetLabelMaxCompose2->SetLabel( aux2 );

      setParameterComposeField( defaultValues[ 0 ], widgetMinCompose2 );
      setParameterComposeField( defaultValues[ 1 ], widgetMaxCompose2 );

      widgetLabelMinCompose2->Enable( true );
      widgetMinCompose2->Enable( true );
      widgetLabelMaxCompose2->Enable( true );
      widgetMaxCompose2->Enable( true );
    }
  }

  Layout();
  Fit();
}


void DerivedTimelineDialog::setParameterComposeField( TParamValue defaultValues, wxTextCtrl *field )
{
  wxString aux;

  PRV_UINT32 maxValues = PRV_UINT32( defaultValues.size() );

  aux << defaultValues[ 0 ];
  for ( PRV_UINT32 i = 1; i < maxValues; ++i  )
  {
    aux << _( "; " );
    aux << defaultValues[ i ];
  }
  
  field->SetValue( aux );
}


bool DerivedTimelineDialog::getParameterComposeField( wxTextCtrl *field, TParamValue &values )
{
  double tmpDouble;

  bool allTokensAreDouble = true;

  wxStringTokenizer tkz( field->GetValue(), wxT( ";" ) );

  while ( allTokensAreDouble && tkz.HasMoreTokens() )
  {
    wxString token = tkz.GetNextToken();
    if ( token.ToDouble( &tmpDouble ) )
      values.push_back( tmpDouble );
    else
      allTokensAreDouble = false;
  }

  return allTokensAreDouble;
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_TOPCOMPOSE1
 */

void DerivedTimelineDialog::OnTopcompose1Selected( wxCommandEvent& event )
{
  PRV_UINT32 numParameters;
  vector< string > namesParameters;
  vector< vector < double > > defaultParameters;

  // Get the Compose 1 function selected
  string nameFunction = topCompose1[ widgetTopCompose1->GetCurrentSelection() ];

  // Show widgets for its parameters
  if ( currentWindow1->getParametersOfFunction( nameFunction, 
                                                 numParameters,
                                                 namesParameters,
                                                 defaultParameters ) )
    setParametersCompose( 0, nameFunction, numParameters, namesParameters, defaultParameters );
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_TOPCOMPOSE2
 */

void DerivedTimelineDialog::OnTopcompose2Selected( wxCommandEvent& event )
{
  PRV_UINT32 numParameters;
  vector< string > namesParameters;
  vector< vector < double > > defaultParameters;

  // Get the Compose 2 function selected
  string nameFunction = topCompose2[ widgetTopCompose2->GetCurrentSelection() ];

  // Show widgets for its parameters
  if ( currentWindow2->getParametersOfFunction( nameFunction, 
                                                numParameters,
                                                namesParameters,
                                                defaultParameters ) )
    setParametersCompose( 1, nameFunction, numParameters, namesParameters, defaultParameters );
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_OPERATIONS
 */

void DerivedTimelineDialog::OnOperationsSelected( wxCommandEvent& event )
{
  // Get the Operation selected
  string nameOperations = operations[ widgetOperations->GetCurrentSelection() ];

  // Changes label
  if ( nameOperations == "controlled: clear by" ||
       nameOperations == "controlled: maximum"  ||
       nameOperations == "controlled: add" ||
       nameOperations == "controlled: enumerate" ||
       nameOperations == "controlled: average" )
  {
    widgetLabelTimelines1->SetLabel( _( "Data" ) );
    widgetLabelTimelines2->SetLabel( _( "Control" ) );
  }
  else
  {
    widgetLabelTimelines1->SetLabel( _( "Timeline" ) );
    widgetLabelTimelines2->SetLabel( _( "Timeline" ) );
  }

  Layout();
}


/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_TIMELINES_LIST_1
 */

/*void DerivedTimelineDialog::OnTimelinesList1Selected( wxCommandEvent& event )
{
  Window *tmpTimeline1 = timelines1[ event.GetSelection() ];
  Window *tmpTimeline2 = timelines2[ widgetTimelines2->GetSelection() ];
  
  if( Window::compatibleLevels( tmpTimeline1, tmpTimeline2 ) )
  {
    lastTimeline1 = event.GetSelection();
    event.Skip();
  }
  else
  {
    wxMessageBox( wxT( "Incompatible levels between windows." ), wxT( "Warning" ), wxOK|wxICON_EXCLAMATION, this );
    widgetTimelines1->SetSelection( lastTimeline1 );
  }
}
*/

/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_TIMELINES_LIST_2
 */

/*void DerivedTimelineDialog::OnTimelinesList2Selected( wxCommandEvent& event )
{
  Window *tmpTimeline1 = timelines1[ widgetTimelines1->GetSelection() ];
  Window *tmpTimeline2 = timelines2[ event.GetSelection() ];
  
  if( Window::compatibleLevels( tmpTimeline1, tmpTimeline2 ) )
  {
    lastTimeline2 = event.GetSelection();
    event.Skip();
  }
  else
  {
    wxMessageBox( wxT( "Incompatible levels between windows." ), wxT( "Warning" ), wxOK|wxICON_EXCLAMATION, this );
    widgetTimelines2->SetSelection( lastTimeline2 );
  }
}
*/


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_TIMELINES_BUTTON1
 */

void DerivedTimelineDialog::OnTimelinesButton1Click( wxCommandEvent& event )
{
  TimelineTreeSelector timelineSelector( this,
                                         wxID_ANY,
                                         wxT( "Timeline" ),
                                         timelines1,
                                         currentWindow1,
                                         currentWindow1->getTrace(),
                                         false );
  timelineSelector.Move( wxGetMousePosition() );
  
  int retCode = timelineSelector.ShowModal();
  if( retCode == wxID_OK )
  {
    if( currentWindow1 == timelineSelector.getSelection() )
      return;
    currentWindow1 = timelineSelector.getSelection();
    txtTimelines1->SetValue( wxString( currentWindow1->getName().c_str(), wxConvUTF8 ) );
  }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_TIMELINES_BUTTON2
 */

void DerivedTimelineDialog::OnTimelinesButton2Click( wxCommandEvent& event )
{
  TimelineTreeSelector timelineSelector( this,
                                         wxID_ANY,
                                         wxT( "Timeline" ),
                                         timelines2,
                                         currentWindow2,
                                         currentWindow2->getTrace(),
                                         false );
  timelineSelector.Move( wxGetMousePosition() );
  
  int retCode = timelineSelector.ShowModal();
  if( retCode == wxID_OK )
  {
    if( currentWindow2 == timelineSelector.getSelection() )
      return;
    currentWindow2 = timelineSelector.getSelection();
    txtTimelines2->SetValue( wxString( currentWindow2->getName().c_str(), wxConvUTF8 ) );
  }
}

