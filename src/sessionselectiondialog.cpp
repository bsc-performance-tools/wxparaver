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


// Version that gets 
SessionSelectionDialog::SessionSelectionDialog( wxString folderPath, bool isInitialized )
{
  Init();
  this->folderPath = folderPath;
  this->isInitialized = isInitialized;
  OnCreateNoDialog();
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

////@end SessionSelectionDialog content construction


  textDialogDescription->Show( !isInitialized );
  textDialogDescription->GetFont().SetWeight( wxFONTWEIGHT_BOLD );
  if ( isInitialized )
  {
    textDialogDescription->SetLabel( _("Select one of your last auto-saved Paraver sessions") );
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
    
    map< boost::posix_time::ptime, wxString, std::greater< boost::posix_time::ptime > > dtToFile;
    for ( int i = 0 ; i < filesInDir.size() ; ++i )
    {
      #ifdef WIN32
      wxString datetime = filesInDir[ i ].AfterLast( '\\' ).AfterFirst( '_' ).Left( 15 );
      #else
      wxString datetime = filesInDir[ i ].AfterLast( '/' ).AfterFirst( '_' ).Left( 15 );
      #endif
      datetime[ 8 ] = 'T';

      boost::posix_time::ptime dt;
      dt = boost::posix_time::from_iso_string( std::string( datetime.mb_str() ) );
      
      dtToFile.insert( std::pair< boost::posix_time::ptime, wxString >( dt , filesInDir[ i ] ) );
    }

    map< boost::posix_time::ptime, wxString, std::greater< boost::posix_time::ptime > >::iterator it;
    for ( it = dtToFile.begin(); it != dtToFile.end(); ++it )
    {
      wxString fileName = FormatFileName( (* it ).second.AfterLast( '/' ) );
      listSessions->Append( fileName );
      linksPerFileName[ fileName ] = (* it ).second;
    }
  }
  return true;
}


bool SessionSelectionDialog::OnCreateNoDialog()
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
    
    linksPerFileName.clear();
    
    map< boost::posix_time::ptime, wxString, std::greater< boost::posix_time::ptime > > dtToFile;
    for ( int i = 0 ; i < filesInDir.size() ; ++i )
    {
      #ifdef WIN32
      wxString datetime = filesInDir[ i ].AfterLast( '\\' ).AfterFirst( '_' ).Left( 15 );
      #else
      wxString datetime = filesInDir[ i ].AfterLast( '/' ).AfterFirst( '_' ).Left( 15 );
      #endif
      datetime[ 8 ] = 'T';

      boost::posix_time::ptime dt;
      dt = boost::posix_time::from_iso_string( std::string( datetime.mb_str() ) );

      dtToFile.insert( std::pair< boost::posix_time::ptime, wxString >( dt , filesInDir[ i ] ) );
    }

    map< boost::posix_time::ptime, wxString, std::greater< boost::posix_time::ptime > >::iterator it;
    for ( it = dtToFile.begin(); it != dtToFile.end(); ++it )
    {
      allFilesInDir.push_back( (* it ).second );
    }
  }
  return true;
}



wxString SessionSelectionDialog::FormatFileName( wxString fileName )
{
  //wxArrayString parts = wxSplit( fileName, '_' );
  std::string fileStringStd = std::string( fileName.mb_str() ) ;
  wxArrayString parts;  

  std::size_t end, begin = 0;
  char delim = '_';
  end = fileStringStd.find( delim );


  wxString subPart;
  while ( end != std::string::npos ) 
  {
    subPart = wxString( fileStringStd.substr( begin, end - begin ).c_str(), wxConvUTF8 );
    parts.push_back( subPart );
    begin = end + 1;
    end = fileStringStd.find( delim, begin );
  }
  subPart = wxString( fileStringStd.substr( begin, end - begin ).c_str(), wxConvUTF8 );
  parts.push_back( subPart );

  wxString dmy = parts[ 1 ];
  wxString hms = parts[ 2 ]; 

  dmy = dmy.Mid( 6, 2 ) +  // YYYYMMDD (iso compliant)
        wxT( "/" ) +
        dmy.Mid( 4, 2 ) +
        wxT( "/" ) +
        dmy.Mid( 0, 4 );


  hms = hms.Mid( 0, 2 ) +
        wxT( ":" ) +
        hms.Mid( 2, 2 ) +
        wxT( ":" ) +
        hms.Mid( 4, 2 );
  
  wxString crash = ( parts[3] == wxT( "0.session" ) ? wxT( " [Crashed]" ) : _( "" ) );

  return dmy +
         wxT( " " ) +
         hms + 
         crash;
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



wxString SessionSelectionDialog::GetSessionPath()
{
  return myPath;
}



wxArrayString SessionSelectionDialog::GetSessionPaths()
{
  return allFilesInDir;
}

