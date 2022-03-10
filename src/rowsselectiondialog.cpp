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
#include "wx/bookctrl.h"
////@end includes
#include "wx/filename.h"

#include "rowsselectiondialog.h"
#include "labelconstructor.h"
#include "gtimeline.h"
#include "histogram.h"
#include "window.h"
//#include "pg_extraprop.h"
//#include "windows_tree.h"
#include "paravermain.h" // for help

using namespace std;

////@begin XPM images
////@end XPM images
#include "../icons/help.xpm"

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

// Constructor for Timelines
RowsSelectionDialog::RowsSelectionDialog( wxWindow* parent,
                                          Timeline *whichTimeline,
                                          SelectionManagement< TObjectOrder, TTraceLevel > *whichSelectedRows,
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

  myHistogram = nullptr;
  myLevel = myTimeline->getLevel();
  myTrace = myTimeline->getTrace();
  lockedByUpdate = false;

  if ( ( myLevel >= TTraceLevel::SYSTEM ) && ( myLevel <= TTraceLevel::CPU ) )
  {
    minLevel = TTraceLevel::NODE;
    buildPanel( wxT("Node"), TTraceLevel::NODE );
    buildPanel( wxT("CPU"), TTraceLevel::CPU );
  }
  else if ( ( myLevel >= TTraceLevel::WORKLOAD ) && ( myLevel <= TTraceLevel::THREAD ) )
  {
    minLevel = TTraceLevel::APPLICATION;
    buildPanel( _("Application"), TTraceLevel::APPLICATION );
    buildPanel( _("Task"), TTraceLevel::TASK );
    buildPanel( _("Thread"), TTraceLevel::THREAD );
  }
  
  LayoutDialog();
  Centre();
}

// Constructor for Histograms
RowsSelectionDialog::RowsSelectionDialog( wxWindow* parent,
                                          Histogram* histogram,
                                          SelectionManagement< TObjectOrder, TTraceLevel > *whichSelectedRows,
                                          wxWindowID id,
                                          const wxString& caption,
                                          bool whichParentIsGtimeline,
                                          const wxPoint& pos,
                                          const wxSize& size,
                                          long style ) :
        myHistogram( histogram ), mySelectedRows( whichSelectedRows ), parentIsGtimeline( whichParentIsGtimeline )                       
{
  Init();
  Create( parent, id, caption, pos, size, style );
  
  SelectionManagement< TObjectOrder, TTraceLevel > sm = ( *myHistogram->getRowSelectionManagement() );
  vector< TObjectOrder > rowsel;
  sm.getSelected( rowsel, myHistogram->getControlWindow()->getLevel() );

  myTimeline = nullptr;
  myLevel = myHistogram->getControlWindow()->getLevel();
  myTrace = myHistogram->getTrace();
  lockedByUpdate = false;

  if (( myLevel >= TTraceLevel::SYSTEM ) && ( myLevel <= TTraceLevel::CPU ))
  {
    minLevel = TTraceLevel::NODE;
    buildPanel( wxT("Node"), TTraceLevel::NODE );
    buildPanel( wxT("CPU"), TTraceLevel::CPU );
  }
  else if (( myLevel >= TTraceLevel::WORKLOAD ) && ( myLevel <= TTraceLevel::THREAD ))
  {
    minLevel = TTraceLevel::APPLICATION;
    buildPanel( wxT("Application"), TTraceLevel::APPLICATION );
    buildPanel( wxT("Task"), TTraceLevel::TASK );
    buildPanel( wxT("Thread"), TTraceLevel::THREAD );
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


void RowsSelectionDialog::OnRegularExpressionHelp( wxCommandEvent& event )
{
  if ( !paraverMain::myParaverMain->GetParaverConfig()->getGlobalHelpContentsQuestionAnswered() )
    paraverMain::myParaverMain->helpQuestion();

  wxChar SEP = wxFileName::GetPathSeparator();

  wxString helpContentsDir = SEP +
        wxString( wxT( "share" ) ) + SEP +
        wxString( wxT( "doc" ) ) + SEP +
        wxString( wxT( "wxparaver_help_contents" ) ) + SEP +
        wxString( wxT( "html" ) ) + SEP;

  wxString helpChapter = wxString( wxT( "1.quick_reference" ) ) + SEP +
          wxString( wxT( "index.html"));

  wxString helpRegEx = wxString( wxT( "#objects_regex" ) );

  paraverMain::myParaverMain->createHelpContentsWindow( helpContentsDir, helpChapter, helpRegEx, true );
}


/*
 * Dynamic panel building
 */
void RowsSelectionDialog::buildPanel( const wxString& title, TTraceLevel whichLevel )
{
  wxPanel *myPanel;

  myPanel = new wxPanel( GetBookCtrl(),
                         wxID_ANY,
                         wxDefaultPosition,
                         wxDefaultSize,
                         wxSUNKEN_BORDER | wxTAB_TRAVERSAL );

  GetBookCtrl()->AddPage( myPanel, title, whichLevel == myLevel ); //myTimeline->getLevel() );

  wxBoxSizer *panelSizer = new wxBoxSizer( wxVERTICAL );
  wxBoxSizer *buttonsSizer = new wxBoxSizer( wxHORIZONTAL );

  myPanel->SetSizer( panelSizer );

  // Add Checklist lines
  wxArrayString choices;

  for ( size_t row = (size_t)0; row < myTrace->getLevelObjects( whichLevel ); ++row )
  {
    if( whichLevel == TTraceLevel::CPU || whichLevel == TTraceLevel::NODE )
      choices.Add( wxString::FromUTF8( LabelConstructor::objectLabel( (TObjectOrder)row + 1,
                                                                       whichLevel,
                                                                       myTrace ).c_str() ) );
    else
      choices.Add( wxString::FromUTF8( LabelConstructor::objectLabel( (TObjectOrder)row,
                                                                       whichLevel,
                                                                       myTrace ).c_str() ) );
  }

  //vector< TObjectOrder > selectedIndex;
  mySelectedRows->getSelected( selectedIndex[ whichLevel ], whichLevel );
  
  wxCheckListBox * auxCheckList = new wxCheckListBox( myPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices );
  auxCheckList->Connect(
          wxEVT_COMMAND_LISTBOX_SELECTED,
          wxCommandEventHandler( RowsSelectionDialog::OnCheckListBoxSelected ),
          nullptr,
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

  panelSizer->Add( auxCheckList, 3, wxALL | wxGROW, 5 );
  
  //
  // BUTTONS
  //
  wxButton *auxButton = new wxButton( myPanel, wxID_ANY, _("Select All") );
  selectionButtons.push_back( auxButton );
  auxButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
                      wxCommandEventHandler( RowsSelectionDialog:: OnSelectAllButtonClicked ),
                      nullptr,
                      this ); 
  buttonsSizer->Add( auxButton, 1, wxGROW | wxALL, 5 );

  auxButton = new wxButton( myPanel, wxID_ANY, _("Unselect All") );
  selectionButtons.push_back( auxButton );
  auxButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
                      wxCommandEventHandler( RowsSelectionDialog::OnUnselectAllButtonClicked ),
                      nullptr,
                      this ); 
  buttonsSizer->Add( auxButton, 1, wxGROW | wxALL, 5 );

  auxButton = new wxButton( myPanel, wxID_ANY, _("Invert") );
  selectionButtons.push_back( auxButton );
  auxButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
                      wxCommandEventHandler( RowsSelectionDialog::OnInvertButtonClicked ),
                      nullptr,
                      this ); 
  buttonsSizer->Add( auxButton, 1, wxGROW | wxALL, 5 );

  // Build Panel
  panelSizer->Add( buttonsSizer, 0, wxGROW | wxALL , 5 );


  //
  // REGULAR EXPRESSION BOX
  //

  // RE: Text box
  bool initialCheckState = false;

  wxStaticBox* regularExpressionBox = new wxStaticBox(myPanel, wxID_ANY, wxT(" Add checks by objects matching "));
  wxStaticBoxSizer* regularExpressionBoxSizer = new wxStaticBoxSizer( regularExpressionBox, wxVERTICAL );
  wxBoxSizer *regularExpressionSizerUp = new wxBoxSizer( wxHORIZONTAL );
  wxBoxSizer *regularExpressionSizerDown = new wxBoxSizer( wxVERTICAL );


  wxTextCtrl *auxTextCtrl =
          new wxTextCtrl( myPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
  auxTextCtrl->Connect(
          wxEVT_COMMAND_TEXT_ENTER ,
          wxCommandEventHandler( RowsSelectionDialog::OnRegularExpressionApply ),
          nullptr,
          this );
  auxTextCtrl->SetToolTip( getMyToolTip( initialCheckState ) );
  textCtrlRegularExpr.push_back( auxTextCtrl );
  regularExpressionSizerUp->Add( auxTextCtrl, 1, wxGROW | wxALL, 5 );

  // userRegularExpr->SetValidator( getValidator(  ) );

  wxRegEx *aux = new wxRegEx( wxString( wxT( ".*" ) ) );
  validRE.push_back( aux );

  // RE: APPLY button
  auxButton = new wxButton( myPanel, wxID_ANY, _("Apply") );
  auxButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
                      wxCommandEventHandler( RowsSelectionDialog::OnRegularExpressionApply ),
                      nullptr,
                      this );
  auxButton->Enable( true );
  applyButtons.push_back( auxButton );

  regularExpressionSizerUp->Add( auxButton, 0, wxGROW | wxALL, 5 );

  // RE: Help button
  wxBitmapButton *auxButtonBMP = new wxBitmapButton( myPanel,
                                                     wxID_ANY,
                                                     wxBitmap( help_xpm ),
                                                     wxDefaultPosition, wxDefaultSize,
                                                     wxBU_AUTODRAW );
  auxButtonBMP->SetToolTip(_("Regular expressions help."));
  auxButtonBMP->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
                      wxCommandEventHandler( RowsSelectionDialog::OnRegularExpressionHelp ),
                      nullptr,
                      this );
  auxButtonBMP->Enable( true );
  helpRE.push_back( auxButtonBMP );

  regularExpressionSizerUp->Add( auxButtonBMP, 0, wxGROW | wxALL, 5 );

  // RE: Check
  wxCheckBox *auxCheckBox = new wxCheckBox( myPanel, wxID_ANY, _("Match as Posix Basic Regular Expression"), wxDefaultPosition, wxDefaultSize, 0 );
  auxCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED,
                        wxCommandEventHandler( RowsSelectionDialog::OnCheckBoxMatchPosixRegExpClicked ),
                        nullptr,
                        this );
  auxCheckBox->SetValue( initialCheckState );
  auxCheckBox->SetToolTip( getMyToolTip( initialCheckState ) );
  checkBoxPosixBasicRegExp.push_back( auxCheckBox );
  regularExpressionSizerDown->Add( auxCheckBox, 0, wxGROW | wxALL, 5 );

  // RE: Message
  wxStaticText *auxStaticText = new wxStaticText( myPanel, wxID_ANY, _("" ) );
  messageMatchesFound.push_back( auxStaticText );
  regularExpressionSizerDown->Add( auxStaticText, 0, wxGROW | wxALL, 5 );

  // RE: Final Build
  regularExpressionBoxSizer->Add( regularExpressionSizerUp, 0, wxGROW | wxALL , 0 );
  regularExpressionBoxSizer->Add( regularExpressionSizerDown, 0, wxGROW | wxALL , 0 );

  panelSizer->Add( regularExpressionBoxSizer, 0, wxGROW | wxALL , 5 );
}

/*!
 * RowsSelectionDialog destructor
 */
RowsSelectionDialog::~RowsSelectionDialog()
{
  // TODO: MEMORY ALLOCATED NOT FREED

  for ( vector< wxCheckListBox* >::iterator it = levelCheckList.begin(); it != levelCheckList.end(); ++it )
  {
    (*it)->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED,
                       wxCommandEventHandler( RowsSelectionDialog::OnCheckListBoxSelected ) );
  }

  for ( vector< wxCheckBox* >::iterator it = checkBoxPosixBasicRegExp.begin(); it != checkBoxPosixBasicRegExp.end(); ++it )
  {
    (*it)->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED,
                       wxCommandEventHandler( RowsSelectionDialog::OnCheckBoxMatchPosixRegExpClicked ));
  }

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

  for ( vector< wxTextCtrl * >::iterator it = textCtrlRegularExpr.begin(); it != textCtrlRegularExpr.end(); ++it )
  {
    (*it)->Disconnect( wxEVT_COMMAND_TEXT_ENTER ,
                       wxCommandEventHandler( RowsSelectionDialog::OnRegularExpressionApply ) );
  }

  for ( vector< wxButton * >::iterator it = applyButtons.begin(); it != applyButtons.end(); ++it )
  {
    (*it)->Disconnect( wxEVT_COMMAND_TEXT_ENTER ,
                       wxCommandEventHandler( RowsSelectionDialog::OnRegularExpressionApply ) );
  }

  for ( vector< wxButton * >::iterator it = helpRE.begin(); it != helpRE.end(); ++it )
  {
    (*it)->Disconnect( wxEVT_COMMAND_TEXT_ENTER ,
                       wxCommandEventHandler( RowsSelectionDialog::OnRegularExpressionHelp ) );
  }

  /*if ( myTimeline != nullptr )
    TWindowLevel myLevel = myTimeline->getLevel();
  else if ( myHistogram != nullptr )
    TWindowLevel myLevel = myHistogram->getControlWindow()->getLevel();*/
  
  TTraceLevel beginLevel, endLevel;
  if (( myLevel >= TTraceLevel::SYSTEM ) && ( myLevel <= TTraceLevel::CPU ))
  {
    beginLevel = TTraceLevel::NODE;
    endLevel = TTraceLevel::CPU;
  }
  else if (( myLevel >= TTraceLevel::WORKLOAD ) && ( myLevel <= TTraceLevel::THREAD ))
  {
    beginLevel = TTraceLevel::APPLICATION;
    endLevel = TTraceLevel::THREAD;
  }

  TTraceLevel levelDiff = static_cast<TTraceLevel>( static_cast<size_t>( endLevel ) - static_cast<size_t>( beginLevel ) );
  for( TTraceLevel l = TTraceLevel::NONE; l <= levelDiff; ++l )
  {
    size_t tmpL = static_cast<size_t>( l );
    delete selectionButtons[ tmpL ];
    delete levelCheckList[ tmpL ];
    delete messageMatchesFound[ tmpL ];
    delete checkBoxPosixBasicRegExp[ tmpL ];
    delete textCtrlRegularExpr[ tmpL ];
    delete applyButtons[ tmpL ];
    delete validRE[ tmpL ];
    delete helpRE[ tmpL ];
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


int RowsSelectionDialog::GetSelections( TTraceLevel whichLevel, wxArrayInt &selections )
{
  int selected = 0;
  size_t levelDiff = static_cast<size_t>( whichLevel ) - static_cast<size_t>( minLevel );
  if ( levelCheckList[ levelDiff ] != nullptr )
  for ( unsigned int i = 0; i < levelCheckList[ levelDiff ]->GetCount(); ++i )
  {
    if ( levelCheckList[ levelDiff ]->IsChecked( i ) )
    {
      ++selected;
      selections.Add( i );
    }
  }
  
  return selected;
}


bool RowsSelectionDialog::TransferDataFromWindow()
{
  TTraceLevel beginLevel;
  TTraceLevel endLevel;

  // Set range of levels for update loop
  if (( myLevel >= TTraceLevel::WORKLOAD ) && ( myLevel <= TTraceLevel::THREAD ))
  {
    beginLevel = TTraceLevel::APPLICATION;
    endLevel = TTraceLevel::THREAD;
  }
  else
  {
    beginLevel = TTraceLevel::NODE;
    endLevel = TTraceLevel::CPU;
  }

  // Loop through levels to update gTimeline
  for ( TTraceLevel whichLevel = beginLevel; whichLevel <= endLevel; ++whichLevel )
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
                                   myTrace->getLevelObjects( whichLevel ),
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
    switch ( (wxChar)enteredRE.GetChar( i ) )
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
      case wxChar('?'):
        parsedRE += wxString( wxT( "[[:alpha:]]" ) );
        break;
//      case wxChar('^'): // need this?
//        parsedRE += wxString( wxT( "^" ) );
//        break;
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
                                _("."),  // any character
                                _("+"),  // zero or one repetition of preceeding item
                                _("*"),  // zero or many repetitions of preceeding item
                                _("#"),  // used?
                                _("^"),  // begin of line
                                _("$")   // end of line
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
           wxT( "Posix basic regular expression form:\n"
                "  . : any character (use [.] for dot)\n"
                "  ? : 0 - 1 repetition of preceeding item\n"
                "  + : 1 - n repetitions of preceeding item\n"
                "  * : 0 - n repetitions of preceeding item\n"
                "  ^ : begin of line\n"
                "  $ : end of line\n"
                "  [1234] : set that matches from 1 to 4\n"
                "  [1-3] : range that matches from 1 to 3\n\n" ) :
           wxT( "Quick form:\n"
                "  . : '.' (dot character)\n"
                "  # : only one number\n"
                "  ? : only one character\n"
                "  + : one or many alfanumeric\n"
                "  * : zero or many alfanumeric\n"
                "  ^ : begin of line\n"
                "  $ : end of line\n"
                "  [1234] : set that matches from 1 to 4\n"
                "  [1-3] : range that matches from 1 to 3\n\n" ) );
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
{  // Are selected into the current zoom?
  wxArrayInt dialogSelections;

  int numberSelected = GetSelections( myLevel, dialogSelections );
  if ( numberSelected == 0 )
  {
    wxString tmpMsg( wxT( "No object selected!" ) );
    wxMessageDialog tmpDialog( this, tmpMsg, _( "Warning" ), wxOK | wxICON_EXCLAMATION );
    if ( tmpDialog.ShowModal() == wxID_OK )
    {
    }
  }
  else if ( parentIsGtimeline && myTimeline != nullptr )
  {
    ZoomAwareTransferData( dialogSelections, ((gTimeline *)GetParent())->getCurrentZoomRange() );
  }
  else if ( parentIsGtimeline && myHistogram != nullptr )
  {
    std::vector< TObjectOrder > selection;
    mySelectedRows->getSelected( selection, myLevel );
    ZoomAwareTransferData( dialogSelections, selection );
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
