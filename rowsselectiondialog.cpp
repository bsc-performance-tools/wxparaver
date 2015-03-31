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
#include "wx/bookctrl.h"
////@end includes

#include "rowsselectiondialog.h"
#include "labelconstructor.h"
#include "gtimeline.h"
#include "window.h"
//#include "pg_extraprop.h"
//#include "windows_tree.h"


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


/*
 * RowsSelectionDialog constructors
 */
RowsSelectionDialog::RowsSelectionDialog()
{
  Init();
}


RowsSelectionDialog::RowsSelectionDialog( wxWindow* parent,
                                          Window *whichTimeline,
                                          SelectionManagement< TObjectOrder, TWindowLevel > *whichSelectedRows,
                                          wxWindowID id,
                                          const wxString& caption,
                                          bool whichParentIsGtimeline,
                                          const wxPoint& pos,
                                          const wxSize& size,
                                          long style ) :
        myTimeline( whichTimeline ), mySelectedRows( whichSelectedRows ), parentIsGtimeline( whichParentIsGtimeline )
                                          
{
  Init();
  Create( parent, id, caption, pos, size, style );

  TWindowLevel level = myTimeline->getLevel();
  lockedByUpdate = false;

  if (( level >= SYSTEM ) && ( level <= CPU ))
  {
    minLevel = NODE;
    buildPanel( _("Node"), NODE );
    buildPanel( _("CPU"), CPU );
  }
  else if (( level >= WORKLOAD ) && ( level <= THREAD ))
  {
    minLevel = APPLICATION;
    buildPanel( _("Application"), APPLICATION );
    buildPanel( _("Task"), TASK );
    buildPanel( _("Thread"), THREAD );
  }
  
  LayoutDialog();
  Centre();
}


/*!
 * RowsSelectionDialog creator
 */
bool RowsSelectionDialog::Create( wxWindow* parent,
                                  wxWindowID id,
                                  const wxString& caption,
                                  const wxPoint& pos,
                                  const wxSize& size,
                                  long style )
{
  SetExtraStyle( wxWS_EX_VALIDATE_RECURSIVELY | wxWS_EX_BLOCK_EVENTS );
  SetSheetStyle( wxPROPSHEET_DEFAULT );
  wxPropertySheetDialog::Create( parent, id, caption, pos, size, style );

  CreateButtons( wxOK | wxCANCEL );
  CreateControls();
  LayoutDialog();
  Centre();

  return true;
}

/*
 * Dynamic panel building
 */
void RowsSelectionDialog::buildPanel( const wxString& title,
                                      TWindowLevel whichLevel )
{
  wxPanel *myPanel;

  myPanel = new wxPanel( GetBookCtrl(),
                         wxID_ANY,
                         wxDefaultPosition,
                         wxDefaultSize,
                         wxSUNKEN_BORDER | wxTAB_TRAVERSAL );

  GetBookCtrl()->AddPage( myPanel, title, whichLevel == myTimeline->getLevel() );

  wxBoxSizer *panelSizer = new wxBoxSizer( wxVERTICAL );
  wxBoxSizer *buttonsSizer = new wxBoxSizer( wxHORIZONTAL );

  myPanel->SetSizer( panelSizer );

  // Add Checklist lines
  wxArrayString choices;
  Trace *myTrace = myTimeline->getTrace();
  for ( size_t row = (size_t)0; row < myTrace->getLevelObjects( whichLevel ); ++row )
  {
    if( whichLevel == CPU )
      choices.Add( wxString::FromAscii( LabelConstructor::objectLabel( (TObjectOrder)row + 1,
                                                                       whichLevel,
                                                                       myTrace ).c_str() ) );
    else
      choices.Add( wxString::FromAscii( LabelConstructor::objectLabel( (TObjectOrder)row,
                                                                       whichLevel,
                                                                       myTrace ).c_str() ) );
  }

  //vector< TObjectOrder > selectedIndex;
  mySelectedRows->getSelected( selectedIndex[ whichLevel ], whichLevel );
  wxCheckListBox * auxCheckList = new wxCheckListBox( myPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices );
  auxCheckList->Connect(
          wxEVT_COMMAND_LISTBOX_SELECTED,
          wxCommandEventHandler( RowsSelectionDialog::OnCheckListBoxSelected ),
          NULL,
          this );
  levelCheckList.push_back( auxCheckList );

  PRV_INT16 firstFound = -1;
  for ( unsigned int i = 0; i < (unsigned int)selectedIndex[ whichLevel ].size(); ++i )
  {
    if ( firstFound == -1 )
      firstFound = selectedIndex[ whichLevel ][ i ];
    
    auxCheckList->Check( selectedIndex[ whichLevel ][ i ] );
  }
  
  if ( firstFound != -1 )
    auxCheckList->SetFirstItem( (int)firstFound );

  panelSizer->Add( auxCheckList, 3, wxALL | wxALIGN_CENTER | wxGROW, 5 );
  
  //
  // BUTTONS
  //
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

  // Build Panel
  panelSizer->Add( buttonsSizer, 0, wxGROW | wxALL | wxALIGN_BOTTOM, 5 );


  //
  // REGULAR EXPRESSION BOX
  //

  // RE: Text box
  bool initialCheckState = false;

  wxStaticBox* regularExpressionBox = new wxStaticBox(myPanel, wxID_ANY, _(" Add checks by objects matching "));
  wxStaticBoxSizer* regularExpressionBoxSizer = new wxStaticBoxSizer( regularExpressionBox, wxVERTICAL );
  wxBoxSizer *regularExpressionSizerUp = new wxBoxSizer( wxHORIZONTAL );
  wxBoxSizer *regularExpressionSizerDown = new wxBoxSizer( wxVERTICAL );


  wxTextCtrl *auxTextCtrl =
          new wxTextCtrl( myPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
  auxTextCtrl->Connect(
          wxEVT_COMMAND_TEXT_ENTER ,
          wxCommandEventHandler( RowsSelectionDialog::OnRegularExpressionApply ),
          NULL,
          this );
  auxTextCtrl->SetToolTip( getMyToolTip( initialCheckState ) );
  textCtrlRegularExpr.push_back( auxTextCtrl );
  regularExpressionSizerUp->Add( auxTextCtrl, 1, wxGROW | wxALIGN_BOTTOM | wxALL, 5 );

  // userRegularExpr->SetValidator( getValidator(  ) );

  //wxRegEx *aux = new wxRegEx( wxString("[:alpha]* ?1[.][12][.][0-9]") );// ejemplo para thread 1.1.1
  //wxRegEx *aux = new wxRegEx( wxString( wxT( "([:alnum:]|[_-. ])+" ) ) );// ejemplo para bastante cosa
  wxRegEx *aux = new wxRegEx( wxString( wxT( ".*" ) ) );// ejemplo para bastante cosa
  validRE.push_back( aux );

  // RE: APPLY button
  auxButton = new wxButton( myPanel, wxID_ANY, _("Apply") );
  auxButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
                      wxCommandEventHandler( RowsSelectionDialog::OnRegularExpressionApply ),
                      NULL,
                      this );
  auxButton->Enable( true );
  applyButtons.push_back( auxButton );

  regularExpressionSizerUp->Add( auxButton, 0, wxGROW | wxALIGN_BOTTOM | wxALL, 5 );

  // RE: Check
  wxCheckBox *auxCheckBox = new wxCheckBox( myPanel, wxID_ANY, _("Match as Posix Basic Regular Expression"), wxDefaultPosition, wxDefaultSize, 0 );
  auxCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED,
                        wxCommandEventHandler( RowsSelectionDialog::OnCheckBoxMatchPosixRegExpClicked ),
                        NULL,
                        this );
  auxCheckBox->SetValue( initialCheckState );
  auxCheckBox->SetToolTip( getMyToolTip( initialCheckState ) );
  checkBoxPosixBasicRegExp.push_back( auxCheckBox );
  regularExpressionSizerDown->Add( auxCheckBox, 0, wxGROW | wxALIGN_BOTTOM | wxALL, 5 );

  // RE: Message
  wxStaticText *auxStaticText = new wxStaticText( myPanel, wxID_ANY, _("" ) );
  messageMatchesFound.push_back( auxStaticText );
  regularExpressionSizerDown->Add( auxStaticText, 0, wxGROW | wxALIGN_BOTTOM | wxALL, 5 );

  // RE: Final Build
  regularExpressionBoxSizer->Add( regularExpressionSizerUp, 0, wxGROW | wxALL | wxALIGN_BOTTOM, 0 );
  regularExpressionBoxSizer->Add( regularExpressionSizerDown, 0, wxGROW | wxALL | wxALIGN_BOTTOM, 0 );

  panelSizer->Add( regularExpressionBoxSizer, 0, wxGROW | wxALL | wxALIGN_BOTTOM, 5 );
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

  TWindowLevel level = myTimeline->getLevel();
  TWindowLevel beginLevel, endLevel;
  if (( level >= SYSTEM ) && ( level <= CPU ))
  {
    beginLevel = NODE;
    endLevel = CPU;
  }
  else if (( level >= WORKLOAD ) && ( level <= THREAD ))
  {
    beginLevel = APPLICATION;
    endLevel = THREAD;
  }

  for( int l = 0; l <= (int)endLevel - (int)beginLevel; ++l )
  {
    delete selectionButtons[ l ];
    delete levelCheckList[ l ];
    delete messageMatchesFound[ l ];
    delete checkBoxPosixBasicRegExp[ l ];
    delete textCtrlRegularExpr[ l ];
    delete applyButtons[ l ];
    delete validRE[ l ];
  }
}


/*!
 * Member initialisation
 */
void RowsSelectionDialog::Init()
{
  shouldChangeTimelineZoom = false;
  beginZoom = TObjectOrder( 0 );
  endZoom = TObjectOrder( 0 );
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


bool RowsSelectionDialog::TransferDataFromWindow()
{
  TWindowLevel beginLevel;
  TWindowLevel endLevel;

  // Set range of levels for update loop
  if (( myTimeline->getLevel() >= WORKLOAD ) &&
      ( myTimeline->getLevel() <= THREAD ))
  {
    beginLevel = APPLICATION;
    endLevel = THREAD;
  }
  else
  {
    beginLevel = NODE;
    endLevel = CPU;
  }

  // Loop through levels to update gTimeline
  for ( TWindowLevel whichLevel = beginLevel; whichLevel <= endLevel; whichLevel = TWindowLevel(whichLevel + 1) )
  {
    wxArrayInt selections;
    int numberSelected = GetSelections( whichLevel, selections );
    if ( numberSelected > 0 )
    {
      // Get new selections for that level
      vector< TObjectOrder > newSelection;
      for ( size_t row = (size_t)0; row < (size_t)numberSelected; row++ )
      {
        newSelection.push_back( (TObjectOrder)selections[ row ] );
      }
      
      mySelectedRows->setSelected( newSelection,
                                   myTimeline->getTrace()->getLevelObjects( whichLevel ),
                                   whichLevel );
    }
  }
  
  return true;
}


/*
 * Regular Expression methods
 */
wxString RowsSelectionDialog::buildRegularExpressionString( const wxString& enteredRE )
{
  wxString parsedRE;

  for( size_t i = 0; i < enteredRE.Len(); ++i )
  {
    switch ( enteredRE.GetChar( i ) )
    {
      case wxChar('.'):
        parsedRE += wxString( wxT( "[.]" ) );
        break;
      case wxChar('+'):
        parsedRE += wxString( wxT( "[[:alnum:]]+" ) );
        break;
      case wxChar('*'):
        parsedRE += wxString( wxT( "[[:alnum:]]*" ) );
        break;
      case wxChar('#'):
        parsedRE += wxString( wxT( "[[:digit:]]" ) );
        break;
      case wxChar('$'):
        parsedRE += wxString( wxT( "[[:alpha:]]" ) );
        break;
      default:
        parsedRE += enteredRE.GetChar( i );
        break;
    }
  }

  return parsedRE;
}


int RowsSelectionDialog::countMatches( int iTab, wxRegEx *&levelRE )
{
  int matches = 0;

  wxCheckListBox * myLevelCheckList = levelCheckList[ iTab ];
  for ( unsigned int i = 0; i < myLevelCheckList->GetCount(); ++i )
  {
    wxString currentRow( myLevelCheckList->GetString( i ) );
    if ( levelRE->Matches( currentRow ) )
    {
      matches++;
    }
  }

  return matches;
}


void RowsSelectionDialog::checkMatches( const int &iTab, wxRegEx *&levelRE )
{
  lockedByUpdate = true;

  wxCheckListBox * myLevelCheckList = levelCheckList[ iTab ];
  for ( unsigned int i = 0; i < myLevelCheckList->GetCount(); ++i )
  {
    wxString currentRow( myLevelCheckList->GetString( i ) );
    if ( levelRE->Matches( currentRow ) )
    {
      myLevelCheckList->Check( i );
    }
  }

  lockedByUpdate = false;
}



// false: simple mode
// true : basicPosixRegExprMode 
wxTextValidator* RowsSelectionDialog::getValidator( bool basicPosixRegExprMode )
{
  wxTextValidator *myValidator;

  if ( !basicPosixRegExprMode )
  {
    wxString allowedChars[] = { _("0"), _("1"), _("2"), _("3"), _("4"),
                                _("5"), _("6"), _("7"), _("8"), _("9"),
                                _("."),  // level separator
                                _("+"),  // any alphanum, but '.'
                                _("*"),  // many alphanum, but '.'
                                _("#"),  // any number
                                _("$")   // any char
                                };
    myValidator = new wxTextValidator( (long int)wxFILTER_INCLUDE_CHAR_LIST );
    wxArrayString charIncludes( (size_t)15, allowedChars );
    myValidator->SetIncludes( charIncludes );
  }
  else
  {
    wxString allowedChars[] = { _("0"), _("1"), _("2"), _("3"), _("4"),
                                _("5"), _("6"), _("7"), _("8"), _("9"),
                                _("."),  // level separator
                                _("+"),  // any alphanum, but '.'
                                _("*"),  // many alphanum, but '.'
                                _("#"),  // any number
                                _("$")   // any char
                                };
    myValidator = new wxTextValidator( (long int)wxFILTER_INCLUDE_CHAR_LIST );
    wxArrayString charIncludes( (size_t)15, allowedChars );
    myValidator->SetIncludes( charIncludes );
  }

  return myValidator;
}


wxString RowsSelectionDialog::getMyToolTip( const bool posixBasicRegExpTip )
{
  return ( posixBasicRegExpTip?
           _( "Posix basic regular expression form:\n"
              "  . : any character (use [.] for dot)\n"
              "  ? : zero or one repetition of preceeding item\n"
              "  + : one or many repetitions of preceeding item\n"
              "  * : zero or many repetitions of preceeding item\n"
              "  [12345] : set that matches from 1 to 5\n"
              "  [1-3] : range that matches from 1 to 3\n\n"
              "Examples:\n"
              "  .* (anything)\n"
              "  THREAD 1[.][1][.][1-9]  (THREAD 1.1.1, ...THREAD 1.1.9)\n"
              "  1[.][13]+[.]1    (1.1.1, 1.11.1, 1.13.1, 1.31.1, ...)\n\n") :
           _( "Quick form:\n"
              "  . : '.' (dot character)\n"
              "  # : only one number\n"
              "  $ : only one character\n"
              "  + : one or many alfanumeric\n"
              "  * : zero or many alfanumeric\n"
              "  [12345] : set that matches from 1 to 5\n"
              "  [1-3] : range that matches from 1 to 3\n\n"
              "Examples:\n"
              "  THREAD 1.2##.1  (3 digits per task)\n"
              "  TASK 1.3[1-3] (1.31, 1.32, 1.33)\n\n" ) );
}


void RowsSelectionDialog::OnCheckBoxMatchPosixRegExpClicked( wxCommandEvent& event )
{
  int iTab = GetBookCtrl()->GetSelection();
  bool posixBasicRegExpTip = checkBoxPosixBasicRegExp[ iTab ]->IsChecked();
  textCtrlRegularExpr[ iTab ]->SetToolTip( getMyToolTip( posixBasicRegExpTip ) );
  checkBoxPosixBasicRegExp[ iTab ]->SetToolTip( getMyToolTip( posixBasicRegExpTip ) );
}


void RowsSelectionDialog::OnRegularExpressionApply( wxCommandEvent& event )
{
  // Get wxString that represents regular expression
  int iTab = GetBookCtrl()->GetSelection();
  // wxString parsedRE = event.GetString();  // DOESN'T WORK WITH BUTTON APPLY
  wxString parsedRE  = textCtrlRegularExpr[ iTab ]->GetValue();

  if ( !checkBoxPosixBasicRegExp[ iTab ]->GetValue() )
  {
    // It is Quick Form; we must adapt it. 
    parsedRE = buildRegularExpressionString( parsedRE );
  }

  // Build regular expression
  wxRegEx *levelRE = new wxRegEx();
  if ( levelRE->Compile( parsedRE ) )
  {
    // Any match?
    int matches = countMatches( iTab, levelRE );
    if( matches > 0 )
    {
      // Save this regular expression as a valid one.
      //    validRE[ iTab ] = levelRE; 

      checkMatches( iTab, levelRE );

      // Message
      wxString msg = wxString::Format( wxT( "%i" ), matches );
      if ( matches > 1 )
      {
        msg += wxString( wxT( " matches " ) );
      }
      else
      {
        msg += wxString( wxT( " match " ) );
      }
      msg += wxString( wxT( " has been checked." ) );

      messageMatchesFound[ iTab ]->SetLabel( msg );
    }
    else
    {
      messageMatchesFound[ iTab ]->SetLabel( wxString( wxT( "No matches found." ) ) );
    //    delete levelRE;
    }
  }
  else
  {
    messageMatchesFound[ iTab ]->SetLabel( wxString( wxT( "Syntax error in regular expression!" ) ) );
  }
  
  delete levelRE;
}


void RowsSelectionDialog::OnCheckListBoxSelected( wxCommandEvent& event )
{
  if ( lockedByUpdate )
  {
    event.Skip();
  }
  else
  {
    // Copy selected to Reg. Exp. Text box.
    int tabNumber = GetBookCtrl()->GetSelection();
    wxString currentRow = event.GetString();
    textCtrlRegularExpr[ tabNumber ]->SetValue( currentRow );
  }
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


void RowsSelectionDialog::ZoomAwareTransferData( const wxArrayInt &dialogSelections,
                                                  const vector< TObjectOrder > &timelineZoomRange )
{
  if ( timelineZoomRange.size() > 0 )
  {
    TObjectOrder newBegin( dialogSelections[0] );
    TObjectOrder newEnd( dialogSelections.Last() );
    TObjectOrder curBegin( timelineZoomRange.front() );
    TObjectOrder curEnd( timelineZoomRange.back() );

    if ( curBegin <= newBegin && newEnd <= curEnd ) // Are new limits inside/visible?
    {      
      if ( TransferDataFromWindow() )
        EndModal( wxID_OK );        
    }
    else
    {
      wxString tmpMsg( wxT( "Do you want to extend the zoom to fit selected objects?" ) );
      wxMessageDialog tmpDialog( this, tmpMsg, _( "Paraver question" ), wxYES_NO | wxICON_QUESTION );
      if ( tmpDialog.ShowModal() == wxID_YES )
      {
        if ( TransferDataFromWindow() )
        {      
          // Extend to the maximum
          shouldChangeTimelineZoom = true;
          beginZoom = curBegin < newBegin ? curBegin : newBegin;
          endZoom = curEnd > newEnd ? curEnd : newEnd;

          EndModal( wxID_OK );        
        }
      }
    }
  }
}


void RowsSelectionDialog::OnOkClick( wxCommandEvent& event )
{
  // Are selected into the current zoom?
  wxArrayInt dialogSelections;

  int numberSelected = GetSelections( myTimeline->getLevel(), dialogSelections );
  if ( numberSelected == 0 )
  {
    wxString tmpMsg( wxT( "No object selected!" ) );
    wxMessageDialog tmpDialog( this, tmpMsg, _( "Warning" ), wxOK | wxICON_EXCLAMATION );
    if ( tmpDialog.ShowModal() == wxID_OK )
    {
    }
  }
  else if ( parentIsGtimeline )
  {
    ZoomAwareTransferData( dialogSelections, ((gTimeline *)GetParent())->getCurrentZoomRange() );
  }
  else
  {
#if 0
    bool foundGTimeline;
    gTimeline *tmpTimeline = getGTimelineFromWindow( getAllTracesTree()->GetRootItem(), myTimeline, foundGTimeline );
    if ( foundGTimeline )
    {
      TransferDataComputingZoom( tmpTimeline->getCurrentZoomRange() );
    }
#else
    if ( TransferDataFromWindow() )
      EndModal( wxID_OK );        
#endif
  }
}



