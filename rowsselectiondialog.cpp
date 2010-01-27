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

#include "rowsselectiondialog.h"
#include "labelconstructor.h"
#include "gtimeline.h"

////@begin XPM images
////@end XPM images

/*!
 * RowsSelectionDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( RowsSelectionDialog, wxPropertySheetDialog )


/*!
 * RowsSelectionDialog event table definition
 */

BEGIN_EVENT_TABLE( RowsSelectionDialog, wxPropertySheetDialog )
EVT_BUTTON( wxID_OK, RowsSelectionDialog::OnOkClick )
END_EVENT_TABLE()

//class gTimeline;

/*!
 * RowsSelectionDialog constructors
 */

RowsSelectionDialog::RowsSelectionDialog()
{
  Init();
}

void RowsSelectionDialog::OnSelectAllButtonClicked( wxCommandEvent& event )
{
  wxCheckListBox * myLevelCheckList = levelCheckList[ GetBookCtrl()->GetSelection() ];

  for ( unsigned int i = 0; i < myLevelCheckList->GetCount(); ++i )
    myLevelCheckList->Check( i );
}


void RowsSelectionDialog::OnUnselectAllButtonClicked( wxCommandEvent& event )
{
  wxCheckListBox * myLevelCheckList = levelCheckList[ GetBookCtrl()->GetSelection() ];

  for ( unsigned int i = 0; i < myLevelCheckList->GetCount(); ++i )
    myLevelCheckList->Check( i, false );
}


void RowsSelectionDialog::OnInvertButtonClicked( wxCommandEvent& event )
{
  wxCheckListBox * myLevelCheckList = levelCheckList[ GetBookCtrl()->GetSelection() ];

  for ( unsigned int i = 0; i < myLevelCheckList->GetCount(); ++i )
    myLevelCheckList->Check( i, !myLevelCheckList->IsChecked( i ) );
}


void RowsSelectionDialog::buildPanel( wxWindow* parent, const wxString& title, TWindowLevel whichLevel )
{
  wxPanel *myPanel;
  gTimeline *myTimeline = (gTimeline *)parent;

  myPanel = new wxPanel( GetBookCtrl(),
                         wxID_ANY,
                         wxDefaultPosition,
                         wxDefaultSize,
                         wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

  GetBookCtrl()->AddPage( myPanel, title);

  wxBoxSizer *panelSizer = new wxBoxSizer( wxVERTICAL );
  wxBoxSizer *buttonsSizer = new wxBoxSizer( wxHORIZONTAL );

  myPanel->SetSizer( panelSizer );

  wxArrayString choices;

  vector< bool > selectedRow;
//  myTimeline->GetMyWindow()->getSelectedRows( whichLevel, selectedRow, true );
  myTimeline->GetMyWindow()->getSelectedRows( whichLevel, selectedRow, false );
  for ( size_t row = (size_t)0; row < selectedRow.size(); ++row )
    choices.Add( wxString::FromAscii( LabelConstructor::objectLabel( (TObjectOrder)row,
                                                                     whichLevel,
                                                                     myTimeline->GetMyWindow()->getTrace() ).c_str() ) );

  vector< TObjectOrder > selectedIndex;
  myTimeline->GetMyWindow()->getSelectedRows( whichLevel, selectedIndex, false );
  wxCheckListBox * auxCheckList = new wxCheckListBox( myPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices );
  levelCheckList.push_back( auxCheckList );

  for ( unsigned int i = 0; i < (unsigned int)selectedIndex.size(); ++i )
    auxCheckList->Check( selectedIndex[ i ] );

  panelSizer->Add( auxCheckList, 3, wxALL | wxALIGN_CENTER | wxGROW, 5 );

  wxButton *auxButton = new wxButton( myPanel, wxID_ANY, _("Select All") );
  selectionButtons.push_back( auxButton );
  auxButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
                      wxCommandEventHandler( RowsSelectionDialog:: OnSelectAllButtonClicked ),
                      NULL,
                      this ); 
  buttonsSizer->Add( auxButton, 1, wxGROW | wxALIGN_BOTTOM | wxALL, 5 );

  auxButton = new wxButton( myPanel, wxID_ANY, _("Unselect All") );
  selectionButtons.push_back( auxButton );
  auxButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
                      wxCommandEventHandler( RowsSelectionDialog::OnUnselectAllButtonClicked ),
                      NULL,
                      this ); 
  buttonsSizer->Add( auxButton, 1, wxGROW | wxALIGN_BOTTOM | wxALL, 5 );

  auxButton = new wxButton( myPanel, wxID_ANY, _("Invert") );
  selectionButtons.push_back( auxButton );
  auxButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
                      wxCommandEventHandler( RowsSelectionDialog::OnInvertButtonClicked ),
                      NULL,
                      this ); 
  buttonsSizer->Add( auxButton, 1, wxGROW | wxALIGN_BOTTOM | wxALL, 5 );

  panelSizer->Add( buttonsSizer, 0, wxALL | wxALIGN_BOTTOM, 5 );
}


RowsSelectionDialog::RowsSelectionDialog( TWindowLevel whichLevel, wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);

  level = whichLevel;

  if (( level >= SYSTEM ) && ( level <= CPU ))
  {
    minLevel = NODE;
    buildPanel( parent, _("Node"), NODE );
    buildPanel( parent, _("CPU"), CPU );
  }
  else if (( level >= WORKLOAD ) && ( level <= THREAD ))
  {
    minLevel = APPLICATION;
    buildPanel( parent, _("Application"), APPLICATION );
    buildPanel( parent, _("Task"), TASK );
    buildPanel( parent, _("Thread"), THREAD );
  }

  LayoutDialog();
  Centre();
}


/*!
 * RowsSelectionDialog creator
 */

bool RowsSelectionDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
  SetSheetStyle(wxPROPSHEET_DEFAULT);
  wxPropertySheetDialog::Create( parent, id, caption, pos, size, style );

  CreateButtons(wxOK|wxCANCEL);
  CreateControls();
  LayoutDialog();
  Centre();

  return true;
}


/*!
 * RowsSelectionDialog destructor
 */

RowsSelectionDialog::~RowsSelectionDialog()
{
  for ( vector< wxButton * >::iterator it = selectionButtons.begin(); it != selectionButtons.end(); ++it )
  {
    (*it)->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, 
                       wxCommandEventHandler( RowsSelectionDialog::OnSelectAllButtonClicked )); 
    ++it; 
    (*it)->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                       wxCommandEventHandler( RowsSelectionDialog::OnUnselectAllButtonClicked )); 
    ++it; 
    (*it)->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                       wxCommandEventHandler( RowsSelectionDialog::OnInvertButtonClicked )); 
  }
}


/*!
 * Member initialisation
 */

void RowsSelectionDialog::Init()
{
}


/*!
 * Control creation for RowsSelectionDialog
 */

void RowsSelectionDialog::CreateControls()
{
}


/*!
 * Should we show tooltips?
 */

bool RowsSelectionDialog::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap RowsSelectionDialog::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
}

/*!
 * Get icon resources
 */

wxIcon RowsSelectionDialog::GetIconResource( const wxString& name )
{
  // Icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
}


int RowsSelectionDialog::GetSelections( TWindowLevel whichLevel, wxArrayInt &selections )
{
  int selected = 0;

  for ( unsigned int i = 0; i < levelCheckList[ whichLevel - minLevel ]->GetCount(); ++i )
  {
    if ( levelCheckList[ whichLevel - minLevel ]->IsChecked( i ) )
    {
      ++selected;
      selections.Add( i );
    }
  }

  return selected;
}


void RowsSelectionDialog::OnOkClick( wxCommandEvent& event )
{
  if ( TransferDataFromWindow() )
    EndModal( wxID_OK );
}
