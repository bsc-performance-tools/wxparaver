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
  widgetTopCompose2 = NULL;
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

  wxFlexGridSizer* itemFlexGridSizer3 = new wxFlexGridSizer(2, 2, 0, 0);
  itemBoxSizer2->Add(itemFlexGridSizer3, 0, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Name"), wxDefaultPosition, wxSize(100, -1), wxALIGN_RIGHT );
  itemFlexGridSizer3->Add(itemStaticText4, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

  widgetName = new wxTextCtrl( itemDialog1, ID_DERIVED_NAME, _T(""), wxDefaultPosition, wxSize(250, -1), 0 );
  itemFlexGridSizer3->Add(widgetName, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticLine* itemStaticLine6 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemBoxSizer2->Add(itemStaticLine6, 0, wxGROW|wxALL, 5);

  wxGridSizer* itemGridSizer7 = new wxGridSizer(2, 2, 0, 0);
  itemBoxSizer2->Add(itemGridSizer7, 0, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("Top Compose 1"), wxDefaultPosition, wxDefaultSize, 0 );
  itemGridSizer7->Add(itemStaticText8, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString widgetTopCompose1Strings;
  widgetTopCompose1 = new wxChoice( itemDialog1, ID_TOPCOMPOSE1, wxDefaultPosition, wxDefaultSize, widgetTopCompose1Strings, wxFULL_REPAINT_ON_RESIZE );
  itemGridSizer7->Add(widgetTopCompose1, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticText* itemStaticText10 = new wxStaticText( itemDialog1, wxID_STATIC, _("Top Compose 2"), wxDefaultPosition, wxDefaultSize, 0 );
  itemGridSizer7->Add(itemStaticText10, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString widgetTopCompose2Strings;
  widgetTopCompose2 = new wxChoice( itemDialog1, ID_TOPCOMPOSE2, wxDefaultPosition, wxDefaultSize, widgetTopCompose2Strings, wxFULL_REPAINT_ON_RESIZE );
  itemGridSizer7->Add(widgetTopCompose2, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticLine* itemStaticLine12 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemBoxSizer2->Add(itemStaticLine12, 0, wxGROW|wxALL, 5);

  wxGridSizer* itemGridSizer13 = new wxGridSizer(5, 2, 0, 0);
  itemBoxSizer2->Add(itemGridSizer13, 0, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText14 = new wxStaticText( itemDialog1, wxID_STATIC, _("Factor"), wxDefaultPosition, wxDefaultSize, 0 );
  itemGridSizer13->Add(itemStaticText14, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

  widgetFactorTimeline1 = new wxTextCtrl( itemDialog1, ID_FACTOR_TIMELINE_1, _("1.0"), wxDefaultPosition, wxDefaultSize, 0 );
  itemGridSizer13->Add(widgetFactorTimeline1, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticText* itemStaticText16 = new wxStaticText( itemDialog1, wxID_STATIC, _("Timeline"), wxDefaultPosition, wxDefaultSize, 0 );
  itemGridSizer13->Add(itemStaticText16, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString widgetTimelines1Strings;
  widgetTimelines1 = new wxComboBox( itemDialog1, ID_TIMELINES_LIST_1, _T(""), wxDefaultPosition, wxDefaultSize, widgetTimelines1Strings, wxCB_READONLY );
  itemGridSizer13->Add(widgetTimelines1, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticText* itemStaticText18 = new wxStaticText( itemDialog1, wxID_STATIC, _("Operation"), wxDefaultPosition, wxDefaultSize, 0 );
  itemGridSizer13->Add(itemStaticText18, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString widgetOperationsStrings;
  widgetOperations = new wxChoice( itemDialog1, ID_OPERATIONS, wxDefaultPosition, wxDefaultSize, widgetOperationsStrings, wxFULL_REPAINT_ON_RESIZE );
  itemGridSizer13->Add(widgetOperations, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticText* itemStaticText20 = new wxStaticText( itemDialog1, wxID_STATIC, _("Timeline"), wxDefaultPosition, wxDefaultSize, 0 );
  itemGridSizer13->Add(itemStaticText20, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString widgetTimelines2Strings;
  widgetTimelines2 = new wxComboBox( itemDialog1, ID_TIMELINES_LIST_2, _T(""), wxDefaultPosition, wxDefaultSize, widgetTimelines2Strings, wxCB_READONLY );
  itemGridSizer13->Add(widgetTimelines2, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticText* itemStaticText22 = new wxStaticText( itemDialog1, wxID_STATIC, _("Factor"), wxDefaultPosition, wxDefaultSize, 0 );
  itemGridSizer13->Add(itemStaticText22, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

  widgetFactorTimeline2 = new wxTextCtrl( itemDialog1, ID_FACTOR_TIMELINE_2, _("1.0"), wxDefaultPosition, wxDefaultSize, 0 );
  itemGridSizer13->Add(widgetFactorTimeline2, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticLine* itemStaticLine24 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemBoxSizer2->Add(itemStaticLine24, 0, wxGROW|wxALL, 5);

  swapWindowsButton = new wxButton( itemDialog1, ID_SWAP_WINDOWS, _("S&wap Windows"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer2->Add(swapWindowsButton, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxStaticLine* itemStaticLine26 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemBoxSizer2->Add(itemStaticLine26, 0, wxGROW|wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer27 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer27, 0, wxALIGN_RIGHT|wxALL, 5);
  wxButton* itemButton28 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer27->AddButton(itemButton28);

  wxButton* itemButton29 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer27->AddButton(itemButton29);

  itemStdDialogButtonSizer27->Realize();

////@end DerivedTimelineDialog content construction
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
  currentWindow1->getGroupLabels( topCompose1, 0 );
  topCompose2 = topCompose1;
  presetStringChoiceBox( topCompose1, widgetTopCompose1 );
  presetStringChoiceBox( topCompose2, widgetTopCompose2 );

  // Build lists of timelines and set the selected ones
  presetTimelineComboBox( timelines1, currentWindow1, widgetTimelines1 );
  presetTimelineComboBox( timelines2, currentWindow2, widgetTimelines2 );

  // Retrieve list of operations, build and select first operation
  currentWindow1->getGroupLabels( operations, 1 );
  presetStringChoiceBox( operations, widgetOperations );

  // Fill factor fields
  presetFactorField( factorTimeline1, widgetFactorTimeline1 );
  presetFactorField( factorTimeline2, widgetFactorTimeline2 );

  return true;
}


bool DerivedTimelineDialog::TransferDataFromWindow()
{
  bool result;

  if ( result = getFactorFields( factorTimeline1, factorTimeline2 ) )
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



