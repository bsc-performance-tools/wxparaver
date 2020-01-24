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

#include "sessionselectiondialog.h"

////@begin XPM images
////@end XPM images


/*!
 * SessionSelectionDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( SessionSelectionDialog, wxDialog )


/*!
 * SessionSelectionDialog event table definition
 */

BEGIN_EVENT_TABLE( SessionSelectionDialog, wxDialog )

////@begin SessionSelectionDialog event table entries
  EVT_LISTBOX( ID_SESSIONBOX, SessionSelectionDialog::OnSessionboxSelected )
  EVT_LISTBOX_DCLICK( ID_SESSIONBOX, SessionSelectionDialog::OnSessionboxDoubleClicked )
  EVT_BUTTON( wxID_CANCEL, SessionSelectionDialog::OnCancelClick )
  EVT_BUTTON( wxID_OK, SessionSelectionDialog::OnOkClick )
  EVT_UPDATE_UI( wxID_OK, SessionSelectionDialog::OnOkUpdate )
////@end SessionSelectionDialog event table entries

END_EVENT_TABLE()


/*!
 * SessionSelectionDialog constructors
 */

SessionSelectionDialog::SessionSelectionDialog()
{
  Init();
  OnCreate();
}

SessionSelectionDialog::SessionSelectionDialog( wxWindow* parent, wxString folderPath, bool isInitialized, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, folderPath, isInitialized, id, caption, pos, size, style);
  OnCreate();
}


/*!
 * SessionSelectionDialog creator
 */

bool SessionSelectionDialog::Create( wxWindow* parent, wxString folderPath, bool isInitialized, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin SessionSelectionDialog creation
  SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Centre();
////@end SessionSelectionDialog creation
  this->folderPath = folderPath;
  this->isInitialized = isInitialized;
  if ( isInitialized )
    buttonCancel->SetLabel( _( "&Cancel" ) );
  return true;
}


/*!
 * SessionSelectionDialog destructor
 */

SessionSelectionDialog::~SessionSelectionDialog()
{
////@begin SessionSelectionDialog destruction
////@end SessionSelectionDialog destruction
}


/*!
 * Member initialisation
 */

void SessionSelectionDialog::Init()
{
////@begin SessionSelectionDialog member initialisation
  textDialogDescription = NULL;
  listSessions = NULL;
  buttonCancel = NULL;
  buttonLoad = NULL;
////@end SessionSelectionDialog member initialisation
}


/*!
 * Control creation for SessionSelectionDialog
 */

void SessionSelectionDialog::CreateControls()
{    
////@begin SessionSelectionDialog content construction
  SessionSelectionDialog* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  textDialogDescription = new wxStaticText( itemDialog1, wxID_STATIC, _("Paraver closed unexpectedly. Do you want to load any of your last crashed auto-saved Paraver sessions?"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer2->Add(textDialogDescription, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer1Static = new wxStaticBox(itemDialog1, wxID_ANY, _("List of crashed sessions"));
  wxStaticBoxSizer* itemStaticBoxSizer1 = new wxStaticBoxSizer(itemStaticBoxSizer1Static, wxHORIZONTAL);
  itemBoxSizer2->Add(itemStaticBoxSizer1, 3, wxGROW|wxALL, 5);

  wxArrayString listSessionsStrings;
  listSessions = new wxListBox( itemDialog1, ID_SESSIONBOX, wxDefaultPosition, wxSize(500, 270), listSessionsStrings, wxLB_SINGLE );
  itemStaticBoxSizer1->Add(listSessions, 1, wxGROW|wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer2 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer2, 0, wxGROW|wxALL, 5);
  buttonCancel = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer2->AddButton(buttonCancel);

  buttonLoad = new wxButton( itemDialog1, wxID_OK, _("&Load Session"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer2->AddButton(buttonLoad);

  itemStdDialogButtonSizer2->Realize();

  // Connect events and objects
  listSessions->Connect(ID_SESSIONBOX, wxEVT_CREATE, wxWindowCreateEventHandler(SessionSelectionDialog::OnCreate), NULL, this);
////@end SessionSelectionDialog content construction

  textDialogDescription->Show( !isInitialized );
  textDialogDescription->GetFont().SetWeight( wxFONTWEIGHT_BOLD );
  if ( isInitialized )
  {
    itemStaticBoxSizer1Static->SetLabel( _("List of auto-saved sessions") );
  }
}


/*!
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX
 */

void SessionSelectionDialog::OnSessionboxSelected( wxCommandEvent& event )
{
  if ( listSessions->GetSelection() != wxNOT_FOUND )
    myPath = linksPerFileName[ listSessions->GetString( listSessions->GetSelection() ) ];
}


/*!
 * wxEVT_COMMAND_LISTBOX_DOUBLECLICKED event handler for ID_LISTBOX
 */

void SessionSelectionDialog::OnSessionboxDoubleClicked( wxCommandEvent& event )
{
#if wxMAJOR_VERSION<3
  MakeModal( false );
#endif
  EndModal( wxID_OK );
}


/*!
 * wxEVT_UPDATE_UI event handler for wxID_OK
 */

void SessionSelectionDialog::OnOkUpdate( wxUpdateUIEvent& event )
{
  buttonLoad->Enable( listSessions->GetSelection() != wxNOT_FOUND );
}


/*!
 * Should we show tooltips?
 */

bool SessionSelectionDialog::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap SessionSelectionDialog::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin SessionSelectionDialog bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
////@end SessionSelectionDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon SessionSelectionDialog::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin SessionSelectionDialog icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end SessionSelectionDialog icon retrieval
}


void SessionSelectionDialog::OnCreate( wxWindowCreateEvent& event ) { }


bool SessionSelectionDialog::OnCreate()
{
  if ( wxDirExists( folderPath ) ) 
  {
    wxArrayString filesInDir;
    if ( isInitialized )
      wxDir::GetAllFiles( folderPath, &filesInDir, wxT( "*.session" ), wxDIR_FILES );
    else
      wxDir::GetAllFiles( folderPath, &filesInDir, wxT( "*0.session" ), wxDIR_FILES );
    
    if ( filesInDir.size() == 0 )
      return false;
    
    listSessions->Clear();
    linksPerFileName.clear();
    
    /*filesInDir.Sort();
    for ( wxArrayString::iterator fullFilePath = filesInDir.begin(); fullFilePath != filesInDir.end(); ++fullFilePath ) 
    {
      wxString fileName = FormatFileName( ( *fullFilePath ).AfterLast( '/' ) );
      listSessions->Append( fileName );
      linksPerFileName[ fileName ] = ( *fullFilePath );
    }*/

    map< boost::posix_time::ptime, wxString, std::greater< boost::posix_time::ptime > > dtToFile;
    vector< boost::posix_time::ptime > dateTimes( filesInDir.size() );
    for ( int i = 0 ; i < filesInDir.size() ; ++i )
    {
      #ifdef WIN32
      wxString datetime = filesInDir[ i ].AfterLast( '\\' ).AfterFirst( '_' ).Left( 15 );
      #else
      wxString datetime = filesInDir[ i ].AfterLast( '/' ).AfterFirst( '_' ).Left( 15 );
      #endif
      datetime[ 8 ] = 'T';

      boost::posix_time::ptime dt;;
      dt = boost::posix_time::from_iso_string( std::string( datetime.mb_str() ) );
      
      dtToFile.insert( std::pair< boost::posix_time::ptime, wxString >( dt , filesInDir[ i ] ) );
      dateTimes[i] = dt;
    }

    for ( map< boost::posix_time::ptime, wxString, std::greater< boost::posix_time::ptime > >::iterator it = dtToFile.begin(); it != dtToFile.end(); ++it )
    {
      wxString fileName = FormatFileName( (* it ).second.AfterLast( '/' ) );
      listSessions->Append( fileName );
      linksPerFileName[ fileName ] = ( (* it ).second );
    }
  }
  return true;
}

wxString SessionSelectionDialog::FormatFileName( wxString fileName )
{
  //wxArrayString parts = wxSplit( fileName, '_' );
  std::string fileString = std::string( fileName ) ;
  wxArrayString parts;  
  std::size_t end, begin = 0;
  char delim = '_';
  end = fileString.find( delim );
  while ( end != std::string::npos ) 
  {
      parts.push_back( fileString.substr( begin, end - begin ) );
      begin = end + 1;
      end = fileString.find( delim, begin );
  }
  parts.push_back( fileString.substr( begin, end - begin ) );



  parts[ 0 ].Replace( "ps", "PID: " );
  wxString dmy = parts[ 1 ] ;
  wxString hms = parts[ 2 ] ;

  dmy = dmy.SubString( 6, 7 ) + "/" + dmy.SubString( 4, 5 ) + "/" + dmy.SubString( 0, 3 ) + " " ; // YYYYMMDD (iso compliant)
  hms = hms.SubString( 0, 1 ) + ":" + hms.SubString( 2, 3 ) + ":" + hms.SubString( 4, 5 ) ;
  
  return ( parts[ 0 ] + "\t| " + dmy + hms );
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void SessionSelectionDialog::OnOkClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK in SessionSelectionDialog.
  // Before editing this code, remove the block markers.
  event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK in SessionSelectionDialog. 
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void SessionSelectionDialog::OnCancelClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL in SessionSelectionDialog.
  // Before editing this code, remove the block markers.
  event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL in SessionSelectionDialog. 
}



std::string SessionSelectionDialog::GetSessionPath()
{
  return std::string( myPath.mb_str() );
}

