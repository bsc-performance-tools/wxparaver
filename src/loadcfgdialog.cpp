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

#include "loadcfgdialog.h"

////@begin XPM images
////@end XPM images
#include "paravermain.h"


/*!
 * LoadCFGDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( LoadCFGDialog, wxDialog )


/*!
 * LoadCFGDialog event table definition
 */

BEGIN_EVENT_TABLE( LoadCFGDialog, wxDialog )

////@begin LoadCFGDialog event table entries
  EVT_TEXT_ENTER( ID_SEARCHCTRL, LoadCFGDialog::OnSearchctrlEnter )
  EVT_UPDATE_UI( ID_TEXTDESCRIPTION, LoadCFGDialog::OnTextdescriptionUpdate )
  EVT_BUTTON( wxID_CANCEL, LoadCFGDialog::OnCancelClick )
  EVT_BUTTON( wxID_OK, LoadCFGDialog::OnOkClick )
  EVT_UPDATE_UI( wxID_OK, LoadCFGDialog::OnOkUpdate )
////@end LoadCFGDialog event table entries

END_EVENT_TABLE()


/*!
 * LoadCFGDialog constructors
 */

LoadCFGDialog::LoadCFGDialog()
{
  Init();
}


LoadCFGDialog::LoadCFGDialog( wxWindow* parent, wxString directoryStartingPath, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ):
   directoryStartingPath( directoryStartingPath )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}


/*!
 * LoadCFGDialog creator
 */

bool LoadCFGDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin LoadCFGDialog creation
  SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  Centre();
////@end LoadCFGDialog creation

  searchBar->ChangeValue( directoryStartingPath );
  fileNavigator->SetDirectory( directoryStartingPath );

  return true;
}


/*!
 * LoadCFGDialog destructor
 */

LoadCFGDialog::~LoadCFGDialog()
{
////@begin LoadCFGDialog destruction
////@end LoadCFGDialog destruction
}


/*!
 * Member initialisation
 */

void LoadCFGDialog::Init()
{
////@begin LoadCFGDialog member initialisation
  searchBar = NULL;
  fileNavigator = NULL;
  textDescription = NULL;
  buttonCancel = NULL;
  buttonLoad = NULL;
////@end LoadCFGDialog member initialisation
}


/*!
 * Control creation for LoadCFGDialog
 */

void LoadCFGDialog::CreateControls()
{    
////@begin LoadCFGDialog content construction
  LoadCFGDialog* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  searchBar = new wxTextCtrl( itemDialog1, ID_SEARCHCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
  itemBoxSizer2->Add(searchBar, 0, wxGROW|wxALL, 5);

  fileNavigator = new wxFileCtrl( itemDialog1,ID_FILE_NAVIGATOR,wxEmptyString,wxEmptyString,"CFG (*.cfg)|*.cfg",wxSIMPLE_BORDER|wxFC_OPEN,wxDefaultPosition,wxDefaultSize );
  itemBoxSizer2->Add(fileNavigator, 3, wxGROW|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer6Static = new wxStaticBox(itemDialog1, wxID_STATIC, _("Description"));
  wxStaticBoxSizer* itemStaticBoxSizer6 = new wxStaticBoxSizer(itemStaticBoxSizer6Static, wxHORIZONTAL);
  itemBoxSizer2->Add(itemStaticBoxSizer6, 1, wxGROW|wxALL, 5);

  textDescription = new wxTextCtrl( itemDialog1, ID_TEXTDESCRIPTION, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxNO_BORDER );
  if (LoadCFGDialog::ShowToolTips())
    textDescription->SetToolTip(_("Shows the description of a configuration (.cfg) file."));
  itemStaticBoxSizer6->Add(textDescription, 1, wxGROW|wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer8 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer8, 0, wxGROW|wxALL, 5);
  buttonCancel = new wxButton( itemDialog1, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer8->AddButton(buttonCancel);

  buttonLoad = new wxButton( itemDialog1, wxID_OK, _("&Load"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer8->AddButton(buttonLoad);

  itemStdDialogButtonSizer8->Realize();

////@end LoadCFGDialog content construction

  searchBar->ChangeValue( directoryStartingPath );
  fileNavigator->SetDirectory( directoryStartingPath );
}

bool LoadCFGDialog::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap LoadCFGDialog::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin LoadCFGDialog bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
////@end LoadCFGDialog bitmap retrieval
}

/*!DoLoad
 * Get icon resources
 */

wxIcon LoadCFGDialog::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin LoadCFGDialog icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end LoadCFGDialog icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void LoadCFGDialog::OnCancelClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL in LoadCFGDialog.
  // Before editing this code, remove the block markers.
  event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL in LoadCFGDialog. 
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED evencdirctrlt handler for wxID_OK
 */

void LoadCFGDialog::OnOkClick( wxCommandEvent& event )
{
#if wxMAJOR_VERSION<3
  MakeModal( false );
#endif
  EndModal( wxID_OK );
}


/*!
 * wxEVT_UPDATE_UI event handler for wxID_OK
 */

void LoadCFGDialog::OnOkUpdate( wxUpdateUIEvent& event )
{
  buttonLoad->Enable( !fileNavigator->GetFilename().IsEmpty() &&
                      CFGLoader::isCFGFile( std::string( fileNavigator->GetPath().mb_str() ) ) );
}


wxString LoadCFGDialog::GetFilePath()
{
  return selectedCfgFilePath;
}


/*!
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_SEARCHCTRL
 */

void LoadCFGDialog::OnSearchctrlEnter( wxCommandEvent& event )
{
  wxString myPath = searchBar->GetValue();
  if ( wxDirExists( myPath ) ) 
  {
    fileNavigator->SetDirectory( myPath );
    textDescription->Clear();
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TEXTDESCRIPTION
 */

void LoadCFGDialog::OnTextdescriptionUpdate( wxUpdateUIEvent& event )
{
  wxString myPath = fileNavigator->GetPath();
  selectedCfgFilePath = myPath;
  
  std::string description = "";
  if ( !fileNavigator->GetFilename().IsEmpty() )
  {
    if ( !CFGLoader::isCFGFile( std::string( myPath.mb_str() ) ) )
      description = "*Not a Paraver CFG file!*";
    else if( !CFGLoader::loadDescription( std::string( myPath.mb_str() ), description ) )
      description = "*No description available*";
  }

  textDescription->ChangeValue( description );
}

