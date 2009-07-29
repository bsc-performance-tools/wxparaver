/////////////////////////////////////////////////////////////////////////////
// Name:        derivedtimelinedialog.cpp
// Purpose:     
// Author:      Eloy Martinez
// Modified by: 
// Created:     Tue 07 Jul 2009 17:07:35 CEST
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

#include "derivedtimelinedialog.h"

////@begin XPM images
////@end XPM images


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
  factorTimeline1 = 1.0;
  factorTimeline2 = 1.0;
  currentWindow1 = NULL;
  currentWindow2 = NULL;
  widgetName = NULL;
  widgetTopCompose1 = NULL;
  widgetLabelMinCompose1 = NULL;
  widgetMinCompose1 = NULL;
  widgetLabelMaxCompose1 = NULL;
  widgetMaxCompose1 = NULL;
  widgetTopCompose2 = NULL;
  widgetLabelMinCompose2 = NULL;
  widgetMinCompose2 = NULL;
  widgetLabelMaxCompose2 = NULL;
  widgetMaxCompose2 = NULL;
  widgetFactorTimeline1 = NULL;
  widgetTimelines1 = NULL;
  widgetOperations = NULL;
  widgetTimelines2 = NULL;
  widgetFactorTimeline2 = NULL;
  swapWindowsButton = NULL;
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
  itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL, 5);

  widgetName = new wxTextCtrl( itemDialog1, ID_DERIVED_NAME, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0 );
  itemBoxSizer3->Add(widgetName, 1, wxGROW|wxALL, 5);

  wxStaticLine* itemStaticLine6 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemBoxSizer2->Add(itemStaticLine6, 0, wxGROW|wxLEFT|wxRIGHT, 5);

  wxStaticBox* itemStaticBoxSizer7Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Top Composes"));
  wxStaticBoxSizer* itemStaticBoxSizer7 = new wxStaticBoxSizer(itemStaticBoxSizer7Static, wxVERTICAL);
  itemBoxSizer2->Add(itemStaticBoxSizer7, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer7->Add(itemBoxSizer8, 1, wxGROW|wxLEFT, 5);

  wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer8->Add(itemBoxSizer9, 1, wxALIGN_CENTER_VERTICAL, 5);

  wxStaticText* itemStaticText10 = new wxStaticText( itemDialog1, wxID_STATIC, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer9->Add(itemStaticText10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString widgetTopCompose1Strings;
  widgetTopCompose1 = new wxChoice( itemDialog1, ID_TOPCOMPOSE1, wxDefaultPosition, wxDefaultSize, widgetTopCompose1Strings, wxFULL_REPAINT_ON_RESIZE );
  itemBoxSizer9->Add(widgetTopCompose1, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer8->Add(itemBoxSizer12, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer12->Add(itemBoxSizer13, 1, wxGROW|wxLEFT|wxBOTTOM, 5);

  widgetLabelMinCompose1 = new wxStaticText( itemDialog1, wxID_STATIC, _("Min"), wxDefaultPosition, wxDefaultSize, 0 );
  widgetLabelMinCompose1->Enable(false);
  itemBoxSizer13->Add(widgetLabelMinCompose1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  widgetMinCompose1 = new wxTextCtrl( itemDialog1, ID_MINCOMPOSE1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  widgetMinCompose1->Enable(false);
  itemBoxSizer13->Add(widgetMinCompose1, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer12->Add(itemBoxSizer16, 1, wxGROW|wxLEFT|wxBOTTOM, 5);

  widgetLabelMaxCompose1 = new wxStaticText( itemDialog1, wxID_STATIC, _("Max"), wxDefaultPosition, wxDefaultSize, 0 );
  widgetLabelMaxCompose1->Enable(false);
  itemBoxSizer16->Add(widgetLabelMaxCompose1, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

  widgetMaxCompose1 = new wxTextCtrl( itemDialog1, ID_MAXCOMPOSE1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  widgetMaxCompose1->Enable(false);
  itemBoxSizer16->Add(widgetMaxCompose1, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxStaticLine* itemStaticLine19 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemStaticBoxSizer7->Add(itemStaticLine19, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer7->Add(itemBoxSizer20, 1, wxGROW|wxLEFT, 5);

  wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer20->Add(itemBoxSizer21, 1, wxALIGN_CENTER_VERTICAL, 5);

  wxStaticText* itemStaticText22 = new wxStaticText( itemDialog1, wxID_STATIC, _("2"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer21->Add(itemStaticText22, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString widgetTopCompose2Strings;
  widgetTopCompose2 = new wxChoice( itemDialog1, ID_TOPCOMPOSE2, wxDefaultPosition, wxDefaultSize, widgetTopCompose2Strings, wxFULL_REPAINT_ON_RESIZE );
  itemBoxSizer21->Add(widgetTopCompose2, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxBoxSizer* itemBoxSizer24 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer20->Add(itemBoxSizer24, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer24->Add(itemBoxSizer25, 1, wxGROW|wxLEFT|wxBOTTOM, 5);

  widgetLabelMinCompose2 = new wxStaticText( itemDialog1, wxID_STATIC, _("Min"), wxDefaultPosition, wxDefaultSize, 0 );
  widgetLabelMinCompose2->Enable(false);
  itemBoxSizer25->Add(widgetLabelMinCompose2, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

  widgetMinCompose2 = new wxTextCtrl( itemDialog1, ID_MINCOMPOSE2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  widgetMinCompose2->Enable(false);
  itemBoxSizer25->Add(widgetMinCompose2, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxBoxSizer* itemBoxSizer28 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer24->Add(itemBoxSizer28, 1, wxGROW|wxLEFT|wxBOTTOM, 5);

  widgetLabelMaxCompose2 = new wxStaticText( itemDialog1, wxID_STATIC, _("Max"), wxDefaultPosition, wxDefaultSize, 0 );
  widgetLabelMaxCompose2->Enable(false);
  itemBoxSizer28->Add(widgetLabelMaxCompose2, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

  widgetMaxCompose2 = new wxTextCtrl( itemDialog1, ID_MAXCOMPOSE2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  widgetMaxCompose2->Enable(false);
  itemBoxSizer28->Add(widgetMaxCompose2, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxFlexGridSizer* itemFlexGridSizer31 = new wxFlexGridSizer(0, 2, 0, 0);
  itemBoxSizer2->Add(itemFlexGridSizer31, 1, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText32 = new wxStaticText( itemDialog1, wxID_STATIC, _("Factor"), wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer31->Add(itemStaticText32, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxTOP, 5);

  widgetFactorTimeline1 = new wxTextCtrl( itemDialog1, ID_FACTOR_TIMELINE_1, _("1.0"), wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer31->Add(widgetFactorTimeline1, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  wxStaticText* itemStaticText34 = new wxStaticText( itemDialog1, wxID_STATIC, _("Timeline"), wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer31->Add(itemStaticText34, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxTOP, 5);

  wxArrayString widgetTimelines1Strings;
  widgetTimelines1 = new wxComboBox( itemDialog1, ID_TIMELINES_LIST_1, wxEmptyString, wxDefaultPosition, wxDefaultSize, widgetTimelines1Strings, wxCB_READONLY );
  itemFlexGridSizer31->Add(widgetTimelines1, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  wxStaticText* itemStaticText36 = new wxStaticText( itemDialog1, wxID_STATIC, _("Operation"), wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer31->Add(itemStaticText36, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxTOP, 5);

  wxArrayString widgetOperationsStrings;
  widgetOperations = new wxChoice( itemDialog1, ID_OPERATIONS, wxDefaultPosition, wxDefaultSize, widgetOperationsStrings, wxFULL_REPAINT_ON_RESIZE );
  itemFlexGridSizer31->Add(widgetOperations, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  wxStaticText* itemStaticText38 = new wxStaticText( itemDialog1, wxID_STATIC, _("Timeline"), wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer31->Add(itemStaticText38, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxTOP, 5);

  wxArrayString widgetTimelines2Strings;
  widgetTimelines2 = new wxComboBox( itemDialog1, ID_TIMELINES_LIST_2, wxEmptyString, wxDefaultPosition, wxDefaultSize, widgetTimelines2Strings, wxCB_READONLY );
  itemFlexGridSizer31->Add(widgetTimelines2, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  wxStaticText* itemStaticText40 = new wxStaticText( itemDialog1, wxID_STATIC, _("Factor"), wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer31->Add(itemStaticText40, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxTOP, 5);

  widgetFactorTimeline2 = new wxTextCtrl( itemDialog1, ID_FACTOR_TIMELINE_2, _("1.0"), wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer31->Add(widgetFactorTimeline2, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  wxStaticLine* itemStaticLine42 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemBoxSizer2->Add(itemStaticLine42, 0, wxGROW|wxALL, 5);

  swapWindowsButton = new wxButton( itemDialog1, ID_SWAP_WINDOWS, _("S&wap Windows"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer2->Add(swapWindowsButton, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxStaticLine* itemStaticLine44 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemBoxSizer2->Add(itemStaticLine44, 0, wxGROW|wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer45 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer45, 0, wxALIGN_RIGHT|wxALL, 5);
  wxButton* itemButton46 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer45->AddButton(itemButton46);

  wxButton* itemButton47 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer45->AddButton(itemButton47);

  itemStdDialogButtonSizer45->Realize();

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
  if ( currentWindow1 == NULL )
    currentWindow1 = timelines1[ 0 ];
  if ( currentWindow2 == NULL )
    currentWindow2 = timelines2[ 0 ];

  presetNameField( timelineName, widgetName );

  // Retrieve list of top compose operations, build and select first operation
  currentWindow1->getGroupLabels( 0, topCompose1 );
  topCompose2 = topCompose1;
  presetStringChoiceBox( topCompose1, widgetTopCompose1 );
  presetStringChoiceBox( topCompose2, widgetTopCompose2 );

  // Build lists of timelines and set the selected ones
  presetTimelineComboBox( timelines1, currentWindow1, widgetTimelines1 );
  presetTimelineComboBox( timelines2, currentWindow2, widgetTimelines2 );

  // Retrieve list of operations, build and select first operation
  currentWindow1->getGroupLabels( 1, operations );
  presetStringChoiceBox( operations, widgetOperations );

  // Fill factor fields
  presetFactorField( factorTimeline1, widgetFactorTimeline1 );
  presetFactorField( factorTimeline2, widgetFactorTimeline2 );

  return true;
}


bool DerivedTimelineDialog::TransferDataFromWindow()
{
  bool result;

  if ( ( result = getFactorFields( factorTimeline1, factorTimeline2 ) ) )
  {
      // If factors are ok, then retrieve all the other data
    getName( widgetName, timelineName );

    getSelectedString( widgetTopCompose1, topCompose1 );
    getSelectedString( widgetTopCompose2, topCompose2 );
    getSelectedString( widgetOperations, operations );
  
    getSelectedWindow( widgetTimelines1, timelines1 );
    getSelectedWindow( widgetTimelines2, timelines2 );
  }

  return result;
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
  else
  {
    EndModal( wxID_OK ); // never used!
  }
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
    vector< Window * > aux = timelines1;
    timelines1 = timelines2;
    timelines2 = aux;

    // Get current selected timelines
    int pos1 = widgetTimelines1->GetCurrentSelection();
    int pos2 = widgetTimelines2->GetCurrentSelection();

    // Clean them
    widgetTimelines1->Clear();
    widgetTimelines2->Clear();

    // Rebuild and select timeline properly
    for( vector<Window *>::iterator it = timelines1.begin(); it != timelines1.end(); ++it )
      widgetTimelines1->Append( wxString( (*it)->getName().c_str() ) );

    for( vector<Window *>::iterator it = timelines2.begin(); it != timelines2.end(); ++it )
      widgetTimelines2->Append( wxString( (*it)->getName().c_str() ) );

    widgetTimelines1->Select( pos2 );
    widgetTimelines2->Select( pos1 );
  }
}


// Build first list of timelines and set the selected one
void DerivedTimelineDialog::presetTimelineComboBox( vector< Window * > timelines,
                                                    Window *currentWindow,
                                                    wxComboBox *comboBox )
{
  int pos;

  for( vector<Window *>::iterator it = timelines.begin(); it != timelines.end(); ++it )
    comboBox->Append( wxString( (*it)->getName().c_str() ) );

  pos = 0;
  for( vector<Window *>::iterator it = timelines.begin(); it != timelines.end(); ++it )
  {
    if ( *it != currentWindow )
      pos++;
    else
      break;
  }

  comboBox->Select( pos );
}


void DerivedTimelineDialog::presetStringChoiceBox( vector< string > list,
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

void DerivedTimelineDialog::presetFactorField( double value, wxTextCtrl *field )
{
  wxString auxFactor;

  auxFactor << (float)value;
  field->SetValue( auxFactor );
}

void DerivedTimelineDialog::presetNameField( string whichName, wxTextCtrl *field )
{
  wxString auxName;

  auxName << whichName;
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
  whichName = field->GetValue();
}


bool DerivedTimelineDialog::getFactorFields( double &whichFactor1,
                                             double &whichFactor2 )
{
  double auxFactor1, auxFactor2;
  
  // Checks both factors for mismatch
  if ( !widgetFactorTimeline1->GetValue().ToDouble( &auxFactor1 ))
  {
    wxMessageDialog message( this, 
                             "Factor 1 void or type mismatch.\nPlease use decimal format.",
                             "Error in Field", wxOK );
    message.ShowModal();
    return false;
  }
  else if ( !widgetFactorTimeline2->GetValue().ToDouble( &auxFactor2 ))
  {
    wxMessageDialog message( this, 
                             "Factor 2 void or type mismatch.\nPlease use decimal format.",
                             "Error in Field", wxOK );
    message.ShowModal();
    return false;
  }

  // Only if both of them are ok then retrieve data
  whichFactor1 = auxFactor1;
  whichFactor2 = auxFactor2;

  return true;
}


void DerivedTimelineDialog::setParametersCompose( UINT32 compose,
                                                   string nameFunction,
                                                   UINT32 numParameters,
                                                   vector< string > namesParameters,
                                                   vector< vector< double > > defaultValues )
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

      aux1 << namesParameters[ 0 ];
      widgetLabelMinCompose1->SetLabel( aux1 );

      setParameterComposeField( 0, defaultValues, widgetMinCompose1 );

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

      aux1 << namesParameters[ 0 ];
      aux2 << namesParameters[ 1 ];
      widgetLabelMinCompose1->SetLabel( aux1 );
      widgetLabelMaxCompose1->SetLabel( aux2 );

      setParameterComposeField( 0, defaultValues, widgetMinCompose1 );
      setParameterComposeField( 1, defaultValues, widgetMaxCompose1 );

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

      aux1 << namesParameters[ 0 ];
      widgetLabelMinCompose2->SetLabel( aux1 );

      setParameterComposeField( 0, defaultValues, widgetMinCompose2 );

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

      aux1 << namesParameters[ 0 ];
      aux2 << namesParameters[ 1 ];
      widgetLabelMinCompose2->SetLabel( aux1 );
      widgetLabelMaxCompose2->SetLabel( aux2 );

      setParameterComposeField( 0, defaultValues, widgetMinCompose2 );
      setParameterComposeField( 1, defaultValues, widgetMaxCompose2 );

      widgetLabelMinCompose2->Enable( true );
      widgetMinCompose2->Enable( true );
      widgetLabelMaxCompose2->Enable( true );
      widgetMaxCompose2->Enable( true );
    }
  }

  Layout(); // prueba
  Fit();
}

void DerivedTimelineDialog::setParameterComposeField( UINT32 parameter,
                                                       vector< vector< double > > defaultValues,
                                                       wxTextCtrl *field )
{
  wxString aux;

  UINT32 maxValues = UINT32( defaultValues[ parameter ].size() );
  aux << defaultValues[ parameter ][ 0 ];
  for ( UINT32 i = 1; i < maxValues; ++i  )
  {
    aux << ", ";
    aux << defaultValues[ parameter ][ i ];
  }
  
  field->SetValue( aux );
}

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_TOPCOMPOSE1
 */

void DerivedTimelineDialog::OnTopcompose1Selected( wxCommandEvent& event )
{
  UINT32 numParameters;
  vector< string > namesParameters;
  vector< vector < double > > defaultParameters;

  // Get the Compose selected
  string nameFunction = topCompose1[ widgetTopCompose1->GetCurrentSelection() ];
  
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
  UINT32 numParameters;
  vector< string > namesParameters;
  vector< vector < double > > defaultParameters;

  // Get the Compose selected
  string nameFunction = topCompose2[ widgetTopCompose2->GetCurrentSelection() ];
  
  if ( currentWindow2->getParametersOfFunction( nameFunction, 
                                                 numParameters,
                                                 namesParameters,
                                                 defaultParameters ) )
    setParametersCompose( 1, nameFunction, numParameters, namesParameters, defaultParameters );
}

