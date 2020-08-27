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

#include "saveimagedialog.h"

////@begin XPM images
////@end XPM images


/*!
 * SaveImageDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( SaveImageDialog, wxDialog )


/*!
 * SaveImageDialog event table definition
 */

BEGIN_EVENT_TABLE( SaveImageDialog, wxDialog )

////@begin SaveImageDialog event table entries
  EVT_TEXT( ID_SAVESEARCHTEXTCTRL, SaveImageDialog::OnSavesearchtextctrlTextUpdated )
  EVT_TEXT_ENTER( ID_SAVESEARCHTEXTCTRL, SaveImageDialog::OnSavesearchtextctrlEnter )
  EVT_TEXT_ENTER( ID_FILEPATHSAVEIMGCTRL, SaveImageDialog::OnFilepathsaveimgctrlEnter )
  EVT_TREE_SEL_CHANGED( wxID_TREECTRL, SaveImageDialog::OnSavedirctrlSelChanged )
  EVT_LISTBOX( ID_SAVELISTBOX, SaveImageDialog::OnSavelistboxSelected )
  EVT_LISTBOX_DCLICK( ID_SAVELISTBOX, SaveImageDialog::OnSavelistboxDoubleClicked )
  EVT_CHOICE( ID_FILETYPECHOICE, SaveImageDialog::OnFiletypechoiceSelected )
  EVT_CHECKBOX( ID_SAVEIMAGECHECKBOX, SaveImageDialog::OnSaveimagecheckboxClick )
  EVT_CHECKBOX( ID_SAVELEGENDCHECKBOX, SaveImageDialog::OnSavelegendcheckboxClick )
  EVT_BUTTON( wxID_OK, SaveImageDialog::OnOkClick )
  EVT_UPDATE_UI( wxID_OK, SaveImageDialog::OnOkUpdate )
  EVT_BUTTON( wxID_CANCEL, SaveImageDialog::OnCancelClick )
////@end SaveImageDialog event table entries

END_EVENT_TABLE()


/*!
 * SaveImageDialog constructors
 */

SaveImageDialog::SaveImageDialog()
{
  Init();
}

SaveImageDialog::SaveImageDialog( wxWindow* parent, wxString& directoryStartingPath, wxString defaultFileName, bool isHistogram, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
  : directoryStartingPath( directoryStartingPath ), defaultFileName( defaultFileName ), isHistogram( isHistogram )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}


/*!
 * SaveImageDialog creator
 */

bool SaveImageDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin SaveImageDialog creation
  SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Centre();
////@end SaveImageDialog creation
  return true;
}


/*!
 * SaveImageDialog destructor
 */

SaveImageDialog::~SaveImageDialog()
{
////@begin SaveImageDialog destruction
////@end SaveImageDialog destruction
}


/*!
 * Member initialisation
 */

void SaveImageDialog::Init()
{
////@begin SaveImageDialog member initialisation
  fileNameBar = NULL;
  searchBar = NULL;
  treeDirs = NULL;
  listDirs = NULL;
  fileTypeChoice = NULL;
  imageCheckbox = NULL;
  imageFileName = NULL;
  legendCheckbox = NULL;
  legendFileName = NULL;
  buttonSave = NULL;
  buttonCancel = NULL;
////@end SaveImageDialog member initialisation
}


/*!
 * Control creation for SaveImageDialog
 */

void SaveImageDialog::CreateControls()
{    
////@begin SaveImageDialog content construction
  SaveImageDialog* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer4, 0, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Filename:"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer4->Add(itemStaticText5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  fileNameBar = new wxTextCtrl( itemDialog1, ID_SAVESEARCHTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
  fileNameBar->SetHelpText(_("Write a filename..."));
  if (SaveImageDialog::ShowToolTips())
    fileNameBar->SetToolTip(_("Write a filename..."));
  itemBoxSizer4->Add(fileNameBar, 5, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer1, 0, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText2 = new wxStaticText( itemDialog1, wxID_STATIC, _("Path:"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer1->Add(itemStaticText2, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  searchBar = new wxTextCtrl( itemDialog1, ID_FILEPATHSAVEIMGCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
  searchBar->SetHelpText(_("Write a filename..."));
  if (SaveImageDialog::ShowToolTips())
    searchBar->SetToolTip(_("Write a filename..."));
  itemBoxSizer1->Add(searchBar, 5, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer3, 3, wxGROW|wxALL, 5);

  treeDirs = new wxGenericDirCtrl( itemDialog1, ID_SAVEDIRCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRCTRL_DIR_ONLY, wxT("All files (*.*)|*.*"), 0 );
  itemBoxSizer3->Add(treeDirs, 1, wxGROW|wxALL, 5);

  wxArrayString listDirsStrings;
  listDirs = new wxListBox( itemDialog1, ID_SAVELISTBOX, wxDefaultPosition, wxDefaultSize, listDirsStrings, wxLB_SINGLE );
  itemBoxSizer3->Add(listDirs, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer6, 0, wxALIGN_RIGHT|wxALL, 5);

  wxStaticText* itemStaticText1 = new wxStaticText( itemDialog1, wxID_STATIC, _("File type:"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer6->Add(itemStaticText1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString fileTypeChoiceStrings;
  fileTypeChoiceStrings.Add(_("BMP Image"));
  fileTypeChoiceStrings.Add(_("JPG Image"));
  fileTypeChoiceStrings.Add(_("PNG Image"));
  fileTypeChoiceStrings.Add(_("XPM Image"));
  fileTypeChoice = new wxChoice( itemDialog1, ID_FILETYPECHOICE, wxDefaultPosition, wxDefaultSize, fileTypeChoiceStrings, 0 );
  fileTypeChoice->SetStringSelection(_("PNG Image"));
  itemBoxSizer6->Add(fileTypeChoice, 0, wxGROW|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer4Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Image to save"));
  wxStaticBoxSizer* itemStaticBoxSizer4 = new wxStaticBoxSizer(itemStaticBoxSizer4Static, wxVERTICAL);
  itemBoxSizer2->Add(itemStaticBoxSizer4, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer4->Add(itemBoxSizer5, 1, wxGROW|wxALL, 5);

  imageCheckbox = new wxCheckBox( itemDialog1, ID_SAVEIMAGECHECKBOX, _("Timeline"), wxDefaultPosition, wxDefaultSize, 0 );
  imageCheckbox->SetValue(true);
  itemBoxSizer5->Add(imageCheckbox, 1, wxGROW|wxALL, 5);

  imageFileName = new wxTextCtrl( itemDialog1, ID_SAVEIMAGETEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
  itemBoxSizer5->Add(imageFileName, 5, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer4->Add(itemBoxSizer8, 0, wxGROW|wxALL, 5);

  legendCheckbox = new wxCheckBox( itemDialog1, ID_SAVELEGENDCHECKBOX, _("Legend"), wxDefaultPosition, wxDefaultSize, 0 );
  legendCheckbox->SetValue(false);
  itemBoxSizer8->Add(legendCheckbox, 1, wxGROW|wxALL, 5);

  legendFileName = new wxTextCtrl( itemDialog1, ID_SAVELEGENDTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
  itemBoxSizer8->Add(legendFileName, 5, wxGROW|wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer1 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer1, 0, wxGROW|wxALL, 5);
  buttonSave = new wxButton( itemDialog1, wxID_OK, _("&Save"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer1->AddButton(buttonSave);

  buttonCancel = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer1->AddButton(buttonCancel);

  itemStdDialogButtonSizer1->Realize();

////@end SaveImageDialog content construction
  fileTypeText = _( ".png" );

  if ( isHistogram )
  {
    wxString histogramLabel = wxT( "Histogram" );
    imageCheckbox->SetLabel( histogramLabel );
  }
}


/*!
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_SEARCHCTRL
 */

void SaveImageDialog::OnFilepathsaveimgctrlEnter( wxCommandEvent& event )
{
  wxString myPath = searchBar->GetValue(); 
  if ( wxDirExists( myPath ) ) 
  {
    wxFileName fName;
    fName.AssignDir( myPath );
    wxArrayString filesInDir;
    wxDir myDir( myPath );

    myDir.GetAllFiles( myPath, &filesInDir, wxT( "*" ), wxDIR_FILES );
    
    listDirs->Clear();
    linksPerFileName.clear();
    filesInDir.Sort();
    for ( wxArrayString::iterator fullFilePath = filesInDir.begin(); fullFilePath != filesInDir.end(); ++fullFilePath ) 
    {
      wxString fileName = ( *fullFilePath ).AfterLast( '/' );
      listDirs->Append( fileName );
      linksPerFileName[ fileName ] = ( *fullFilePath );
    }
    treeDirs->SetPath( myPath ); 
  }
  else if ( wxFileExists( myPath ) )
  {
    selectedImageFilePath = myPath;
    #if wxMAJOR_VERSION<3
    MakeModal( false );
    #endif
    EndModal( wxID_OK );
  }
}


/*!
 * wxEVT_COMMAND_TREE_SEL_CHANGED event handler for ID_DIRCTRL
 */

void SaveImageDialog::OnSavedirctrlSelChanged( wxTreeEvent& event )
{
  if ( treeDirs == NULL ) return; 
  wxString myPath = treeDirs->GetPath();
  wxFileName fName;
  
  if ( wxDirExists( myPath ) ) 
  {
    fName.AssignDir( myPath );
    wxArrayString filesInDir;
    wxDir::GetAllFiles( myPath, &filesInDir, wxT( "*" ), wxDIR_FILES ); // wxT( "*.png|*.bmp|*.xpm|*.jpg" )
    
    listDirs->Clear();
    linksPerFileName.clear();
    filesInDir.Sort();
    for ( wxArrayString::iterator fullFilePath = filesInDir.begin(); fullFilePath != filesInDir.end(); ++fullFilePath ) 
    {
      wxString fileName = ( *fullFilePath ).AfterLast( '/' );
      listDirs->Append( fileName );
      linksPerFileName[ fileName ] = ( *fullFilePath );
    }
  }
  /*else if () 
  {
    // ...
  }*/
  searchBar->Clear();
  searchBar->WriteText( myPath );
  event.Skip();
}




/*!
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX
 */

void SaveImageDialog::OnSavelistboxSelected( wxCommandEvent& event )
{
  wxString myPath = linksPerFileName[ listDirs->GetString( listDirs->GetSelection() ) ];
  selectedImageFilePath = myPath;
  
  wxString fileNameWithoutType = listDirs->GetString( listDirs->GetSelection() ).BeforeLast( '.' );
//  std::cout << "IMG = " << fileNameWithoutType.mb_str() << " [" << fileTypeText.mb_str() << "]\tPATH = " << myPath.mb_str() << std::endl;

  fileNameBar->Clear();
  fileNameBar->WriteText( fileNameWithoutType );
}


/*!
 * wxEVT_COMMAND_LISTBOX_DOUBLECLICKED event handler for ID_LISTBOX
 */

void SaveImageDialog::OnSavelistboxDoubleClicked( wxCommandEvent& event )
{
  if ( ( imageCheckbox->IsChecked() || legendCheckbox->IsChecked() ) && !searchBar->IsEmpty() && !fileNameBar->IsEmpty() )
  {
  #if wxMAJOR_VERSION<3
    MakeModal( false );
  #endif
    EndModal( wxID_OK );
  }
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_IMAGECHECKBOX
 */

void SaveImageDialog::OnSaveimagecheckboxClick( wxCommandEvent& event )
{ 
  imageFileName->Clear();
  if ( imageCheckbox->IsChecked() && !fileNameBar->IsEmpty() )
    imageFileName->WriteText( fileNameBar->GetValue() + fileTypeText ); // is [   + _( "_IMAGE" )   ] necessary?
    
  buttonSave->Enable( ( imageCheckbox->IsChecked() || legendCheckbox->IsChecked() ) && !searchBar->IsEmpty() && !fileNameBar->IsEmpty() );
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_LEGENDCHECKBOX
 */

void SaveImageDialog::OnSavelegendcheckboxClick( wxCommandEvent& event )
{
  legendFileName->Clear();
  if ( legendCheckbox->IsChecked() && !fileNameBar->IsEmpty() ) 
    legendFileName->WriteText( fileNameBar->GetValue() + _( "_LEGEND" ) + fileTypeText );
  
  buttonSave->Enable( ( imageCheckbox->IsChecked() || legendCheckbox->IsChecked() ) && !searchBar->IsEmpty() && !fileNameBar->IsEmpty() );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void SaveImageDialog::OnOkClick( wxCommandEvent& event )
{
  if ( ( imageCheckbox->IsChecked() || legendCheckbox->IsChecked() ) && !searchBar->IsEmpty() ) 
  {
  #if wxMAJOR_VERSION<3
    MakeModal( false );
  #endif
    EndModal( wxID_OK );
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for wxID_OK
 */

void SaveImageDialog::OnOkUpdate( wxUpdateUIEvent& event )
{
  //buttonSave->Enable( !fileNameBar->IsEmpty() );
  //buttonSave->Enable( listDirs->GetSelection() != wxNOT_FOUND );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void SaveImageDialog::OnCancelClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL in SaveImageDialog.
  // Before editing this code, remove the block markers.
  event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL in SaveImageDialog. 
}


/*!
 * Should we show tooltips?
 */

bool SaveImageDialog::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap SaveImageDialog::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin SaveImageDialog bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
////@end SaveImageDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon SaveImageDialog::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin SaveImageDialog icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end SaveImageDialog icon retrieval
}

wxString SaveImageDialog::GetImageFilePath() 
{
  return selectedImageFilePath;
}

wxString SaveImageDialog::GetLegendFilePath() 
{
  return selectedLegendFilePath;
}


/*!
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_SAVESEARCHTEXTCTRL
 */

void SaveImageDialog::OnSavesearchtextctrlEnter( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_TEXT_ENTER event handler for ID_SAVESEARCHTEXTCTRL in SaveImageDialog.
  // Before editing this code, remove the block markers.
  event.Skip();
////@end wxEVT_COMMAND_TEXT_ENTER event handler for ID_SAVESEARCHTEXTCTRL in SaveImageDialog. 
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SAVESEARCHTEXTCTRL
 */

void SaveImageDialog::OnSavesearchtextctrlTextUpdated( wxCommandEvent& event )
{ 
// refresh image/legend names
  OnSaveimagecheckboxClick( event );
  OnSavelegendcheckboxClick( event );
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE
 */

void SaveImageDialog::OnFiletypechoiceSelected( wxCommandEvent& event )
{ 
  switch ( fileTypeChoice->GetSelection() ) 
  {
    case 0 :
      fileTypeText = wxT( ".bmp" );
      break;
    case 1 :
      fileTypeText = wxT( ".jpg" );
      break;
    case 2 :
      fileTypeText = wxT( ".png" );
      break;
    case 3 :
      fileTypeText = wxT( ".xpm" );
      break;
  }
  OnSavelistboxSelected( event ); // refresh image/legend names
}
