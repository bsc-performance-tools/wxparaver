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
#  pragma hdrstop
#endif

#ifndef WX_PRECOMP
#  include "wx/wx.h"
#endif

// clang-format off
////@begin includes
#include "wx/imaglist.h"
////@end includes
// clang-format on

#include "labelconstructor.h"
#include "saveimagedialog.h"
#include "wxparaverapp.h"

// clang-format off
////@begin XPM images
////@end XPM images
// clang-format on

constexpr char SCALE_IMAGE_TAG[] = "scale_image";

/*!
 * SaveImageDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( SaveImageDialog, wxDialog )


/*!
 * SaveImageDialog event table definition
 */

BEGIN_EVENT_TABLE( SaveImageDialog, wxDialog )

// clang-format off
////@begin SaveImageDialog event table entries
  EVT_TEXT_ENTER( ID_TEXTPATH, SaveImageDialog::OnTextpathEnter )
  EVT_UPDATE_UI( ID_FILENAVIGATOR, SaveImageDialog::OnFilenavigatorUpdate )
  EVT_CHECKBOX( ID_SAVEIMAGECHECKBOX, SaveImageDialog::OnSaveimagecheckboxClick )
  EVT_CHECKBOX( ID_SAVELEGENDCHECKBOX, SaveImageDialog::OnSavelegendcheckboxClick )
  EVT_CHECKBOX( ID_SAVELEGENDCONTROLCHECKBOX, SaveImageDialog::OnSavelegendcontrolcheckboxClick )
  EVT_CHECKBOX( ID_IMAGE_PLUS_LEGEND_CHECKBOX, SaveImageDialog::OnImagePlusLegendCheckboxClick )
  EVT_CHECKBOX( ID_PREVIEW_REAL_SIZE_CHECKBOX, SaveImageDialog::OnPreviewRealSizeCheckboxClick )
  EVT_BUTTON( ID_SAVEIMAGE_COPY_CLIPBOARD_BUTTON, SaveImageDialog::OnSaveimageCopyClipboardButtonClick )
  EVT_CHOICE( ID_ORIENTATION_LEGEND_CONTROL, SaveImageDialog::OnOrientationLegendControlSelected )
  EVT_CHECKBOX( ID_CHECKBOX_SHRINK_LABEL, SaveImageDialog::OnCheckboxShrinkLabelClick )
  EVT_BUTTON( ID_SELECT_LEGEND, SaveImageDialog::OnShowLegend )
  EVT_BUTTON( wxID_OK, SaveImageDialog::OnOkClick )
  EVT_UPDATE_UI( wxID_OK, SaveImageDialog::OnOkUpdate )
  EVT_BUTTON( wxID_CANCEL, SaveImageDialog::OnCancelClick )
////@end SaveImageDialog event table entries
// clang-format on

EVT_FILECTRL_SELECTIONCHANGED( ID_FILENAVIGATOR, SaveImageDialog::OnFileNavigatorChanged )
EVT_FILECTRL_FOLDERCHANGED( ID_FILENAVIGATOR, SaveImageDialog::OnFileNavigatorChanged )
EVT_FILECTRL_FILTERCHANGED( ID_FILENAVIGATOR, SaveImageDialog::OnFileNavigatorChanged )

END_EVENT_TABLE()

wxString SaveImageDialog::directoryStartingPath = _( "" );

/*!
 * SaveImageDialog constructors
 */

SaveImageDialog::SaveImageDialog()
{
  Init();
}

SaveImageDialog::SaveImageDialog( wxWindow* parent,
                                  wxString& whichStartingPath,
                                  wxString whichFileName,
                                  WindowImageGenerator* whichImageGeneration,
                                  Histogram* whichHistogram,
                                  wxString whichLegendSuffix,
                                  wxString whichLegendControlSuffix,
                                  wxWindowID id,
                                  const wxString& caption,
                                  const wxPoint& pos,
                                  const wxSize& size,
                                  long style )
  : defaultFileName( whichFileName )
  , myHistogram( whichHistogram )
  , legendSuffix( whichLegendSuffix )
  , legendControlSuffix( whichLegendControlSuffix )
{
  if( directoryStartingPath.IsEmpty() )
  {
    directoryStartingPath = whichStartingPath;
  }
  imageGeneration = whichImageGeneration;

  Init();
  Create( parent, id, caption, pos, size, style );
}

/*!
 * SaveImageDialog creator
 */

bool SaveImageDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  // clang-format off
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
  // clang-format on

  return true;
}


/*!
 * SaveImageDialog destructor
 */

SaveImageDialog::~SaveImageDialog()
{
  // clang-format off
////@begin SaveImageDialog destruction
////@end SaveImageDialog destruction
  // clang-format on
}


/*!
 * Member initialisation
 */

void SaveImageDialog::Init()
{
  // clang-format off
////@begin SaveImageDialog member initialisation
  sizerMain = NULL;
  sizerPath = NULL;
  textPath = NULL;
  fileNavigator = NULL;
  imageToSaveSizer = NULL;
  imageSizer = NULL;
  imageCheckbox = NULL;
  imageFileName = NULL;
  legendSizer = NULL;
  legendCheckbox = NULL;
  legendFileName = NULL;
  legendControlSizer = NULL;
  legendControlCheckbox = NULL;
  legendControlFileName = NULL;
  imageScaleControlSizer = NULL;
  imageAndScaleCheckbox = NULL;
  imageScaleFileName = NULL;
  drawWindowSingle = NULL;
  drawScaleSingle = NULL;
  drawScaleControlSingle = NULL;
  drawWindowAndScale = NULL;
  previewRealSizeCheckBox = NULL;
  copyToClipboardButton = NULL;
  widgetTopCompose1 = NULL;
  shrinkLabelCheckBox = NULL;
  buttonSelectLegendValues = NULL;
  buttonSave = NULL;
  buttonCancel = NULL;
////@end SaveImageDialog member initialisation
  // clang-format on
}


/*!
 * Control creation for SaveImageDialog
 */

void SaveImageDialog::CreateControls()
{
  // clang-format off
////@begin SaveImageDialog content construction
  SaveImageDialog* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer1);

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer1->Add(itemBoxSizer2, 1, wxGROW|wxALL, 5);

  sizerMain = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer2->Add(sizerMain, 1, wxGROW|wxALL, 5);

  sizerPath = new wxBoxSizer(wxHORIZONTAL);
  sizerMain->Add(sizerPath, 0, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Path:"), wxDefaultPosition, wxDefaultSize, 0 );
  sizerPath->Add(itemStaticText5, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  textPath = new wxTextCtrl( itemDialog1, ID_TEXTPATH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
  sizerPath->Add(textPath, 5, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  fileNavigator = new wxFileCtrl( itemDialog1,ID_FILENAVIGATOR,wxEmptyString,wxEmptyString,"BMP (*.bmp)|*.bmp|JPG (*.jpg;*.jpeg)|*.jpg;*.jpeg|PNG (*.png)|*.png|XPM (*.xpm)|*.xpm",wxSIMPLE_BORDER|wxFC_SAVE,wxDefaultPosition,wxSize(600, 400) );
  sizerMain->Add(fileNavigator, 1, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

  wxStaticBox* itemStaticBoxSizer8Static = new wxStaticBox(itemDialog1, wxID_ANY, _(" Image to save "));
  imageToSaveSizer = new wxStaticBoxSizer(itemStaticBoxSizer8Static, wxVERTICAL);
  sizerMain->Add(imageToSaveSizer, 0, wxGROW|wxLEFT|wxRIGHT, 5);

  imageSizer = new wxBoxSizer(wxHORIZONTAL);
  imageToSaveSizer->Add(imageSizer, 0, wxGROW, 5);

  imageCheckbox = new wxCheckBox( itemDialog1, ID_SAVEIMAGECHECKBOX, _("Timeline"), wxDefaultPosition, wxDefaultSize, 0 );
  imageCheckbox->SetValue(true);
  imageSizer->Add(imageCheckbox, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  imageFileName = new wxTextCtrl( itemDialog1, ID_SAVEIMAGETEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
  imageSizer->Add(imageFileName, 4, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP, 5);

  legendSizer = new wxBoxSizer(wxHORIZONTAL);
  imageToSaveSizer->Add(legendSizer, 0, wxGROW, 5);

  legendCheckbox = new wxCheckBox( itemDialog1, ID_SAVELEGENDCHECKBOX, _("Legend Data"), wxDefaultPosition, wxDefaultSize, 0 );
  legendCheckbox->SetValue(true);
  legendSizer->Add(legendCheckbox, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  legendFileName = new wxTextCtrl( itemDialog1, ID_SAVELEGENDTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
  legendSizer->Add(legendFileName, 4, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

  legendControlSizer = new wxBoxSizer(wxHORIZONTAL);
  imageToSaveSizer->Add(legendControlSizer, 0, wxGROW, 5);

  legendControlCheckbox = new wxCheckBox( itemDialog1, ID_SAVELEGENDCONTROLCHECKBOX, _("Legend Control"), wxDefaultPosition, wxDefaultSize, 0 );
  legendControlCheckbox->SetValue(true);
  legendControlSizer->Add(legendControlCheckbox, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  legendControlFileName = new wxTextCtrl( itemDialog1, ID_SAVELEGENDCONTROLTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
  legendControlSizer->Add(legendControlFileName, 4, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

  imageScaleControlSizer = new wxBoxSizer(wxHORIZONTAL);
  imageToSaveSizer->Add(imageScaleControlSizer, 0, wxGROW, 5);

  imageAndScaleCheckbox = new wxCheckBox( itemDialog1, ID_IMAGE_PLUS_LEGEND_CHECKBOX, _("Image + Legend"), wxDefaultPosition, wxDefaultSize, 0 );
  imageAndScaleCheckbox->SetValue(true);
  imageScaleControlSizer->Add(imageAndScaleCheckbox, 1, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  imageScaleFileName = new wxTextCtrl( itemDialog1, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
  imageScaleControlSizer->Add(imageScaleFileName, 4, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer2->Add(itemBoxSizer21, 1, wxGROW|wxALL, 5);

  wxNotebook* itemNotebook22 = new wxNotebook( itemDialog1, ID_NOTEBOOK, wxDefaultPosition, wxSize(600, 300), wxBK_DEFAULT );

  drawWindowSingle = new wxScrolledWindow( itemNotebook22, ID_SCROLLEDWINDOW_IMAGE, wxDefaultPosition, wxSize(-1, 250), wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
  drawWindowSingle->SetScrollbars(1, 1, 0, 0);
  itemNotebook22->AddPage(drawWindowSingle, _("Window Preview"));

  drawScaleSingle = new wxScrolledWindow( itemNotebook22, ID_SCROLLEDWINDOW_SCALE, wxDefaultPosition, wxSize(-1, 250), wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
  drawScaleSingle->SetScrollbars(1, 100, 0, 0);
  itemNotebook22->AddPage(drawScaleSingle, _("Legend Data Preview"));

  drawScaleControlSingle = new wxScrolledWindow( itemNotebook22, ID_SCROLLEDWINDOW_SCALE_CONTROL, wxDefaultPosition, wxSize(-1, 250), wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
  drawScaleControlSingle->SetScrollbars(1, 1, 0, 0);
  itemNotebook22->AddPage(drawScaleControlSingle, _("Legend Control Preview"));

  drawWindowAndScale = new wxScrolledWindow( itemNotebook22, ID_SCROLLEDWINDOWANDCALE, wxDefaultPosition, wxSize(-1, 250), wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
  drawWindowAndScale->SetScrollbars(1, 1, 0, 0);
  itemNotebook22->AddPage(drawWindowAndScale, _("Window + Legend Preview"));

  itemBoxSizer21->Add(itemNotebook22, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer35 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer21->Add(itemBoxSizer35, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer35->Add(itemBoxSizer7, 0, wxGROW, 2);

  previewRealSizeCheckBox = new wxCheckBox( itemDialog1, ID_PREVIEW_REAL_SIZE_CHECKBOX, _("Preview 1:1"), wxDefaultPosition, wxDefaultSize, 0 );
  previewRealSizeCheckBox->SetValue(false);
  itemBoxSizer7->Add(previewRealSizeCheckBox, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 2);

  itemBoxSizer7->Add(5, 5, 1, wxGROW|wxLEFT|wxRIGHT, 2);

  copyToClipboardButton = new wxButton( itemDialog1, ID_SAVEIMAGE_COPY_CLIPBOARD_BUTTON, _("Copy to Clipboard"), wxDefaultPosition, wxDefaultSize, 0 );
  copyToClipboardButton->SetHelpText(_("Copy previewed image to clipboard."));
  if (SaveImageDialog::ShowToolTips())
    copyToClipboardButton->SetToolTip(_("Copy previewed image to clipboard."));
  itemBoxSizer7->Add(copyToClipboardButton, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 2);

  wxStaticBox* itemStaticBoxSizer2Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Legend"));
  wxStaticBoxSizer* itemStaticBoxSizer2 = new wxStaticBoxSizer(itemStaticBoxSizer2Static, wxVERTICAL);
  itemBoxSizer35->Add(itemStaticBoxSizer2, 0, wxALIGN_LEFT|wxALL, 5);

  wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer2->Add(itemBoxSizer6, 0, wxALIGN_LEFT|wxALL, 5);

  wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("Orientation"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer6->Add(itemStaticText7, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

  wxArrayString widgetTopCompose1Strings;
  widgetTopCompose1Strings.Add(_("Horizontal"));
  widgetTopCompose1Strings.Add(_("Vertical"));
  widgetTopCompose1 = new wxChoice( itemDialog1, ID_ORIENTATION_LEGEND_CONTROL, wxDefaultPosition, wxDefaultSize, widgetTopCompose1Strings, wxFULL_REPAINT_ON_RESIZE );
  widgetTopCompose1->SetStringSelection(_("Horizontal"));
  itemBoxSizer6->Add(widgetTopCompose1, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  shrinkLabelCheckBox = new wxCheckBox( itemDialog1, ID_CHECKBOX_SHRINK_LABEL, _("Shrink Labels"), wxDefaultPosition, wxDefaultSize, 0 );
  shrinkLabelCheckBox->SetValue(false);
  itemStaticBoxSizer2->Add(shrinkLabelCheckBox, 0, wxALIGN_LEFT|wxALL, 5);

  buttonSelectLegendValues = new wxButton( itemDialog1, ID_SELECT_LEGEND, _("Select Values"), wxDefaultPosition, wxDefaultSize, 0 );
  buttonSelectLegendValues->SetName(wxT("buttonSelectLegendValues"));
  itemStaticBoxSizer2->Add(buttonSelectLegendValues, 0, wxALIGN_LEFT|wxALL, 5);

  wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer1->Add(itemBoxSizer5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer6 = new wxStdDialogButtonSizer;

  itemBoxSizer5->Add(itemStdDialogButtonSizer6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
  buttonSave = new wxButton( itemDialog1, wxID_OK, _("&Save"), wxDefaultPosition, wxDefaultSize, 0 );
  buttonSave->Enable(false);
  itemStdDialogButtonSizer6->AddButton(buttonSave);

  buttonCancel = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer6->AddButton(buttonCancel);

  itemStdDialogButtonSizer6->Realize();

  // Connect events and objects
  drawWindowSingle->Connect(ID_SCROLLEDWINDOW_IMAGE, wxEVT_PAINT, wxPaintEventHandler(SaveImageDialog::OnPaintWindowSingle), NULL, this);
  drawScaleSingle->Connect(ID_SCROLLEDWINDOW_SCALE, wxEVT_PAINT, wxPaintEventHandler(SaveImageDialog::onPaintScale), NULL, this);
  drawScaleControlSingle->Connect(ID_SCROLLEDWINDOW_SCALE_CONTROL, wxEVT_PAINT, wxPaintEventHandler(SaveImageDialog::onPaintScaleControl), NULL, this);
  drawWindowAndScale->Connect(ID_SCROLLEDWINDOWANDCALE, wxEVT_PAINT, wxPaintEventHandler(SaveImageDialog::OnPaintWindowScale), NULL, this);
////@end SaveImageDialog content construction
  // clang-format on
  fileTypeText = _( ".png" );

#if !defined( __WXGTK3__ ) && !defined( __WXOSX__ ) && !defined( __WXMSW__ )
  sizerMain->Hide( sizerPath, true );
#endif

  ScaleImageGenerator* scaleMaster = nullptr;
  if( myHistogram == nullptr )
  {
    scaleMaster = imageGeneration->getScaleImage();

    drawScaleControlSingle->Hide();
    imageToSaveSizer->Hide( (wxSizer*)legendControlSizer, true );
  }
  else
  {
    imageCheckbox->SetLabel( _( "Histogram" ) );
    scaleMaster = imageGeneration->getScaleDataImage();

    if( imageGeneration->getScaleDataImageBitmap() == nullptr && imageGeneration->getScaleImageBitmap() == nullptr )
    {
      imageCheckbox->Hide();

      imageToSaveSizer->Hide( (wxSizer*)legendControlSizer, true );
      imageToSaveSizer->Hide( (wxSizer*)legendSizer, true );
    }
    if( imageGeneration->getScaleImageBitmap() == nullptr )
    {
      drawScaleControlSingle->Hide();
      imageToSaveSizer->Hide( (wxSizer*)legendControlSizer, true );
    }
  }
  if( scaleMaster != nullptr )
  {
    if( scaleMaster->getLegendType() == TScaleImageType::TScaleImageGradientColor ||
        scaleMaster->getLegendType() == TScaleImageType::TScaleImageGradientColorNotNull ||
        scaleMaster->getLegendType() == TScaleImageType::TScaleImageAlternativeGradientColor )
    {
      shrinkLabelCheckBox->Hide();
      buttonSelectLegendValues->Hide();
    }
  }
  else
  {
    drawScaleSingle->Hide();
    drawWindowAndScale->Hide();
    imageToSaveSizer->Hide( (wxSizer*)legendSizer, true );
    imageToSaveSizer->Hide( (wxSizer*)imageScaleControlSizer, true );
  }

  Layout();

  textPath->ChangeValue( directoryStartingPath );
  fileNavigator->SetDirectory( directoryStartingPath );
  fileNavigator->SetFilterIndex( 2 );
  fileNavigator->SetFilename( defaultFileName );


  updateFileNamesAndPaths();
}


void SaveImageDialog::updateFileNamesAndPaths()
{
  wxFileName myPath = fileNavigator->GetPath();

  if( wxDir::Exists( fileNavigator->GetDirectory() ) )
  {
    textPath->ChangeValue( fileNavigator->GetDirectory() );
    directoryStartingPath = fileNavigator->GetDirectory();
  }

  fileTypeText = "." + LabelConstructor::getImageFileSuffix( static_cast< TImageFormat >( fileNavigator->GetFilterIndex() ) );

  setImageFileName();
}


/*!
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_SEARCHCTRL
 */

void SaveImageDialog::OnTextpathEnter( wxCommandEvent& event )
{
  wxString myPath = textPath->GetValue();
  if( wxDirExists( myPath ) )
  {
    fileNavigator->SetDirectory( myPath );
    directoryStartingPath = myPath;
  }
}


void SaveImageDialog::setImageFileName()
{
  wxString fileName = fileNavigator->GetFilename();
  if( fileName.Find( "." ) != wxNOT_FOUND )
  {
    // Change file if matched current selected image extension
    wxString selectedFileSuffix = wxString( LabelConstructor::getImageFileSuffix( static_cast< TImageFormat >( fileNavigator->GetFilterIndex() ) ) );
    if( fileNavigator->GetFilename().AfterLast( '.' ) == selectedFileSuffix )
      fileName = fileNavigator->GetFilename().BeforeLast( '.' );
    else
    {
      // Change filename and selector if matched some available image extensions
      for( int tif = (int)TImageFormat::BMP; tif <= (int)TImageFormat::XPM; ++tif )
      {
        wxString ext = wxString( LabelConstructor::getImageFileSuffix( static_cast< TImageFormat >( tif ) ) );
        if( fileNavigator->GetFilename().AfterLast( '.' ) == ext )
        {
          fileName = fileNavigator->GetFilename().BeforeLast( '.' );
          fileNavigator->SetFilterIndex( tif );
          break;
        }
      }
    }
  }

  selectedImageFilePath = fileName + fileTypeText;
  if( imageCheckbox->IsChecked() && imageFileName->GetValue() != selectedImageFilePath )
    imageFileName->ChangeValue( selectedImageFilePath );
  else if( !imageCheckbox->IsChecked() && !imageFileName->IsEmpty() )
    imageFileName->Clear();

  selectedLegendFilePath = fileName + "." + legendSuffix + fileTypeText;
  if( legendCheckbox->IsChecked() && legendFileName->GetValue() != selectedLegendFilePath )
    legendFileName->ChangeValue( selectedLegendFilePath );
  else if( !legendCheckbox->IsChecked() && !legendFileName->IsEmpty() )
    legendFileName->Clear();

  selectedLegendControlFilePath = fileName + "." + legendControlSuffix + fileTypeText;
  if( legendControlCheckbox->IsChecked() && legendControlFileName->GetValue() != selectedLegendControlFilePath )
    legendControlFileName->ChangeValue( selectedLegendControlFilePath );
  else if( !legendControlCheckbox->IsChecked() && !legendControlFileName->IsEmpty() )
    legendControlFileName->Clear();

  selectedImageScaleFilePath = fileName + "." + wxString( SCALE_IMAGE_TAG ) + fileTypeText;
  if( imageAndScaleCheckbox->IsChecked() && imageScaleFileName->GetValue() != selectedImageScaleFilePath )
    imageScaleFileName->ChangeValue( selectedImageScaleFilePath );
  else if( !imageAndScaleCheckbox->IsChecked() && !imageScaleFileName->IsEmpty() )
    imageScaleFileName->Clear();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_IMAGECHECKBOX
 */

void SaveImageDialog::OnSaveimagecheckboxClick( wxCommandEvent& event )
{
  setImageFileName();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_LEGENDCHECKBOX
 */

void SaveImageDialog::OnSavelegendcheckboxClick( wxCommandEvent& event )
{
  setImageFileName();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void SaveImageDialog::OnOkClick( wxCommandEvent& event )
{
  if( imageCheckbox->IsChecked() )
  {
    wxString tmpImageFile = GetImageFilePath();
    if( wxFileExists( tmpImageFile ) )
    {
      wxMessageDialog tmpDialog( this, tmpImageFile + " exists.\n\nDo yo want to overwrite it?", "File exists", wxYES_NO | wxICON_QUESTION );
      if( tmpDialog.ShowModal() == wxID_NO )
        return;
    }
  }

  if( legendCheckbox->IsChecked() )
  {
    wxString tmpLegendFile = GetLegendFilePath();
    if( wxFileExists( tmpLegendFile ) )
    {
      wxMessageDialog tmpDialog( this, tmpLegendFile + " exists.\n\nDo yo want to overwrite it?", "File exists", wxYES_NO | wxICON_QUESTION );
      if( tmpDialog.ShowModal() == wxID_NO )
        return;
    }
  }
  if( legendControlCheckbox->IsChecked() )
  {
    wxString tmpLegendFile = GetLegendControlFilePath();
    if( wxFileExists( tmpLegendFile ) )
    {
      wxMessageDialog tmpDialog( this, tmpLegendFile + " exists.\n\nDo yo want to overwrite it?", "File exists", wxYES_NO | wxICON_QUESTION );
      if( tmpDialog.ShowModal() == wxID_NO )
        return;
    }
  }

  if( ( imageCheckbox->IsChecked() || legendCheckbox->IsChecked() || legendControlCheckbox->IsChecked() || imageAndScaleCheckbox->IsChecked() ) &&
      !textPath->IsEmpty() )
  {
    directoryStartingPath = textPath->GetValue();
    EndModal( wxID_OK );
  }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void SaveImageDialog::OnCancelClick( wxCommandEvent& event )
{
  // clang-format off
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL in SaveImageDialog.
  // Before editing this code, remove the block markers.
  event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL in SaveImageDialog.
  // clang-format on
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
  // clang-format off
  // Bitmap retrieval
  ////@begin SaveImageDialog bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
////@end SaveImageDialog bitmap retrieval
  // clang-format on
}

/*!
 * Get icon resources
 */

wxIcon SaveImageDialog::GetIconResource( const wxString& name )
{
  // clang-format off
  // Icon retrieval
  ////@begin SaveImageDialog icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end SaveImageDialog icon retrieval
  // clang-format on
}

wxString SaveImageDialog::GetImageFilePath()
{
  return directoryStartingPath + PATH_SEP + selectedImageFilePath;
}
wxString SaveImageDialog::GetImageScaleFilePath()
{
  return directoryStartingPath + PATH_SEP + selectedImageScaleFilePath;
}


wxString SaveImageDialog::GetLegendFilePath()
{
  return directoryStartingPath + PATH_SEP + selectedLegendFilePath;
}

wxString SaveImageDialog::GetLegendControlFilePath()
{
  return directoryStartingPath + PATH_SEP + selectedLegendControlFilePath;
}

bool SaveImageDialog::DialogSavesImage()
{
  return ( imageCheckbox->IsChecked() && imageCheckbox->IsShown() );
}

bool SaveImageDialog::DialogSavesLegend()
{
  return ( legendCheckbox->IsChecked() && legendCheckbox->IsShown() );
}

bool SaveImageDialog::DialogSavesControlLegend()
{
  return ( legendControlCheckbox->IsChecked() && legendControlCheckbox->IsShown() );
}

bool SaveImageDialog::DialogSavesImageScale()
{
  return ( imageAndScaleCheckbox->IsChecked() && imageAndScaleCheckbox->IsShown() );
}


int SaveImageDialog::GetFilterIndex()
{
  return fileNavigator->GetFilterIndex();
}


/*!
 * wxEVT_UPDATE_UI event handler for wxID_OK
 */

void SaveImageDialog::OnOkUpdate( wxUpdateUIEvent& event )
{
  event.Enable( ( imageCheckbox->IsChecked() || legendCheckbox->IsChecked() || imageAndScaleCheckbox->IsChecked() ) &&
                wxDir::Exists( textPath->GetValue() ) && !fileNavigator->GetFilename().IsEmpty() );
}


void SaveImageDialog::OnFileNavigatorChanged( wxFileCtrlEvent& event )
{
  // With wxGenericFileCtrl the constructor emits events before the creation is done, so fileNavigator pointer is still NULL
  if( fileNavigator == nullptr )
    return;

  updateFileNamesAndPaths();
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_FILENAVIGATOR
 */

void SaveImageDialog::OnFilenavigatorUpdate( wxUpdateUIEvent& event )
{
  setImageFileName();
}


void SaveImageDialog::onPaintScale( wxPaintEvent& event )
{
  ////@begin wxEVT_PAINT event handler for ID_SCROLLEDWINDOW_IMAGE in SaveImageDialog.
  // Before editing this code, remove the block markers.
  wxPaintDC dc( wxDynamicCast( event.GetEventObject(), wxWindow ) );
  ////@end wxEVT_PAINT event handler for ID_SCROLLEDWINDOW_IMAGE in SaveImageDialog.
  wxPaintDC dcScale( drawScaleSingle );
  drawScaleSingle->PrepareDC( dcScale );

  EnableLegendControls();

  // wxPaintDC dcscale( drawScaleSingle );
  wxBitmap* originalBmp = nullptr;

  if( myHistogram == nullptr )
    originalBmp = imageGeneration->getScaleImageBitmap();
  else
    originalBmp = imageGeneration->getScaleDataImageBitmap();

  if( originalBmp && originalBmp->IsOk() )
  {
    wxImage img = originalBmp->ConvertToImage();

    int originalWidth  = originalBmp->GetWidth();
    int originalHeight = originalBmp->GetHeight();

    int scrollContainerHeight = drawScaleSingle->GetClientSize().GetHeight();
    int scrollContainerWidth  = drawScaleSingle->GetClientSize().GetWidth();

    int posY = 0;
    int posX = 0;

    if( resize_scale )
    {
      int newWidth;
      int newHeight;

      calculateScaledSize( originalWidth, originalHeight, scrollContainerWidth, scrollContainerHeight, newWidth, newHeight );

      // Resize the image
      wxImage scaledImg = img.Scale( newWidth, newHeight, wxIMAGE_QUALITY_HIGH );

      // Convert back to wxBitmap
      wxBitmap scaledBmp( scaledImg );

      if( ( scrollContainerHeight - newHeight ) > 0 )
        posY = ( scrollContainerHeight - newHeight ) / 2;
      if( ( scrollContainerWidth - newWidth ) > 0 )
        posX = ( scrollContainerWidth - newWidth ) / 2;


      dcScale.DrawBitmap( scaledBmp, posX, posY, true );

      drawScaleSingle->Refresh();
      drawScaleSingle->Update();
    }

    else
    {
      if( ( scrollContainerHeight - originalHeight ) > 0 )
        posY = ( scrollContainerHeight - originalHeight ) / 2;
      if( ( scrollContainerWidth - originalWidth ) > 0 )
        posX = ( scrollContainerWidth - originalWidth ) / 2;
      dcScale.DrawBitmap( *originalBmp, posX, posY, true );

      drawScaleSingle->Refresh();
      drawScaleSingle->Update();
    }
  }
}


void SaveImageDialog::onPaintScaleControl( wxPaintEvent& event )
{
  ////@begin wxEVT_PAINT event handler for ID_SCROLLEDWINDOW_IMAGE in SaveImageDialog.
  // Before editing this code, remove the block markers.
  wxPaintDC dc( wxDynamicCast( event.GetEventObject(), wxWindow ) );
  ////@end wxEVT_PAINT event handler for ID_SCROLLEDWINDOW_IMAGE in SaveImageDialog.
  wxPaintDC dcScale( drawScaleControlSingle );
  drawScaleControlSingle->PrepareDC( dcScale );
  // wxPaintDC dcscale( drawScaleSingle );

  EnableLegendControls();

  wxBitmap* originalBmp = imageGeneration->getScaleImageBitmap();
  if( originalBmp && originalBmp->IsOk() )
  {
    wxImage img = originalBmp->ConvertToImage();

    int originalWidth  = originalBmp->GetWidth();
    int originalHeight = originalBmp->GetHeight();

    int scrollContainerHeight = drawScaleControlSingle->GetClientSize().GetHeight();
    int scrollContainerWidth  = drawScaleControlSingle->GetClientSize().GetWidth();

    int posY = 0;
    int posX = 0;

    if( resize_scale_control )
    {
      int newWidth;
      int newHeight;

      calculateScaledSize( originalWidth, originalHeight, scrollContainerWidth, scrollContainerHeight, newWidth, newHeight );

      // Resize the image
      wxImage scaledImg = img.Scale( newWidth, newHeight, wxIMAGE_QUALITY_HIGH );

      // Convert back to wxBitmap
      wxBitmap scaledBmp( scaledImg );

      if( ( scrollContainerHeight - newHeight ) > 0 )
        posY = ( scrollContainerHeight - newHeight ) / 2;
      if( ( scrollContainerWidth - newWidth ) > 0 )
        posX = ( scrollContainerWidth - newWidth ) / 2;


      dcScale.DrawBitmap( scaledBmp, posX, posY, true );

      drawScaleControlSingle->Refresh();
      drawScaleControlSingle->Update();
    }

    else
    {
      if( ( scrollContainerHeight - originalHeight ) > 0 )
        posY = ( scrollContainerHeight - originalHeight ) / 2;
      if( ( scrollContainerWidth - originalWidth ) > 0 )
        posX = ( scrollContainerWidth - originalWidth ) / 2;
      dcScale.DrawBitmap( *originalBmp, posX, posY, true );

      drawScaleControlSingle->Refresh();
      drawScaleControlSingle->Update();
    }
  }
}

/*!
 * wxEVT_PAINT event handler for ID_SCROLLEDWINDOW_IMAGE
 */

void SaveImageDialog::OnPaintWindowScale( wxPaintEvent& event )
{
  ////@begin wxEVT_PAINT event handler for ID_SCROLLEDWINDOW_IMAGE in SaveImageDialog.
  // Before editing this code, remove the block markers.
  // wxPaintDC dc( wxDynamicCast( event.GetEventObject(), wxWindow ) );
  ////@end wxEVT_PAINT event handler for ID_SCROLLEDWINDOW_IMAGE in SaveImageDialog.
  wxPaintDC dcScale( drawWindowAndScale );
  drawWindowAndScale->PrepareDC( dcScale ); // <--- This line is critical!

  EnableLegendControls();

  wxBitmap* originalBmp = imageGeneration->getTimelineScaleImage();
  if( originalBmp && originalBmp->IsOk() )
  {
    // Convert to wxImage
    wxImage img = originalBmp->ConvertToImage();

    int originalWidth  = originalBmp->GetWidth();
    int originalHeight = originalBmp->GetHeight();

    int scrollContainerHeight = drawWindowAndScale->GetClientSize().GetHeight();
    int scrollContainerWidth  = drawWindowAndScale->GetClientSize().GetWidth();

    int posY = 0;
    int posX = 0;

    if( resize_window )
    {
      int newWidth;
      int newHeight;

      calculateScaledSize( originalWidth, originalHeight, scrollContainerWidth, scrollContainerHeight, newWidth, newHeight );

      // Resize the image
      wxImage scaledImg = img.Scale( newWidth, newHeight, wxIMAGE_QUALITY_HIGH );

      // Convert back to wxBitmap
      wxBitmap scaledBmp( scaledImg );

      if( ( scrollContainerHeight - newHeight ) > 0 )
        posY = ( scrollContainerHeight - newHeight ) / 2;
      if( ( scrollContainerWidth - newWidth ) > 0 )
        posX = ( scrollContainerWidth - newWidth ) / 2;


      dcScale.DrawBitmap( scaledBmp, posX, posY, true );

      drawWindowAndScale->Refresh();
      drawWindowAndScale->Update();
    }

    else
    {
      if( ( scrollContainerHeight - originalHeight ) > 0 )
        posY = ( scrollContainerHeight - originalHeight ) / 2;
      if( ( scrollContainerWidth - originalWidth ) > 0 )
        posX = ( scrollContainerWidth - originalWidth ) / 2;
      dcScale.DrawBitmap( *originalBmp, posX, posY, true );

      drawWindowAndScale->Refresh();
      drawWindowAndScale->Update();
    }
  }
}


void SaveImageDialog::calculateScaledSize( int originalWidth,
                                           int originalHeight,
                                           int scrollContainerWidth,
                                           int scrollContainerHeight,
                                           int& newWidth,
                                           int& newHeight )
{
  if( scrollContainerWidth < originalWidth && scrollContainerHeight < originalHeight )
  {
    double differenceWidth  = (double)originalWidth / (double)scrollContainerWidth;
    double differenceHeight = (double)originalHeight / (double)scrollContainerHeight;
    if( differenceWidth > differenceHeight )
    {
      newWidth  = scrollContainerWidth;
      newHeight = ( originalHeight * newWidth ) / originalWidth;
    }
    else
    {
      newHeight = scrollContainerHeight;
      newWidth  = ( originalWidth * newHeight ) / originalHeight;
    }
  }
  else if( scrollContainerWidth < originalWidth )
  {
    newWidth  = scrollContainerWidth;
    newHeight = ( originalHeight * newWidth ) / originalWidth;
    if( newHeight == 0 )
      newHeight = 1;
  }
  else if( scrollContainerHeight < originalHeight )
  {
    // Desired new width (anchura)
    newHeight = scrollContainerHeight;
    newWidth  = ( originalWidth * newHeight ) / originalHeight;
  }
  else
  {
    double differenceWidth  = (double)scrollContainerWidth / (double)originalWidth;
    double differenceHeight = (double)scrollContainerHeight / (double)originalHeight;
    if( differenceWidth < differenceHeight )
    {
      newWidth  = scrollContainerWidth;
      newHeight = ( originalHeight * newWidth ) / originalWidth;
    }
    else
    {
      newHeight = scrollContainerHeight;
      newWidth  = ( originalWidth * newHeight ) / originalHeight;
    }
  }
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX
 */

// void SaveImageDialog::OnCheckboxScaledsizeClick( wxCommandEvent& event )
// {
//   wxCheckBox* checkbox = wxDynamicCast( FindWindow( ID_CHECKBOX_SCALEDSIZE ), wxCheckBox );
//   if( checkbox )
//   {
//     bool isChecked = checkbox->IsChecked();
//     if( isChecked )
//     {
//       wxSize clientSize = drawWindowSingle->GetClientSize();
//       drawWindowSingle->SetVirtualSize( clientSize ); // virtual size = client size → no scroll
//       drawWindowSingle->SetScrollRate( 0, 0 );        // disable scroll increments
//       drawWindowSingle->ShowScrollbars( wxSHOW_SB_NEVER, wxSHOW_SB_NEVER );
//       resize_window = true;
//       // Checkbox is checked
//     }
//     else
//     {
//       int originalWidth  = imageGeneration->getWindowsImageBitmap()->GetWidth();
//       int originalHeight = imageGeneration->getWindowsImageBitmap()->GetHeight();

//       drawWindowSingle->SetVirtualSize( originalWidth, originalHeight );
//       drawWindowSingle->SetScrollRate( 10, 10 );
//       drawWindowSingle->ShowScrollbars( wxSHOW_SB_DEFAULT, wxSHOW_SB_DEFAULT );
//       resize_window = false;
//       // Checkbox is unchecked
//     }
//   }

//   ////@begin wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX in SaveImageDialog.
//   // Before editing this code, remove the block markers.
//   event.Skip();
//   ////@end wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX in SaveImageDialog.
// }


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX
 */

// void SaveImageDialog::OnCheckboxLegendClick( wxCommandEvent& event )
// {
//   wxCheckBox* checkbox = wxDynamicCast( FindWindow( ID_CHECKBOX_LEGEND ), wxCheckBox );
//   if( checkbox )
//   {
//     bool isChecked = checkbox->IsChecked();
//     if( isChecked )
//     {
//       wxSize clientSize = drawScaleSingle->GetClientSize();
//       drawScaleSingle->SetVirtualSize( clientSize ); // virtual size = client size → no scroll
//       drawScaleSingle->SetScrollRate( 0, 0 );        // disable scroll increments
//       drawScaleSingle->ShowScrollbars( wxSHOW_SB_NEVER, wxSHOW_SB_NEVER );
//       resize_scale = true;
//       // Checkbox is checked
//     }
//     else
//     {
//       int originalWidth;
//       int originalHeight;

//       if( isHistogram )
//       {
//         originalWidth  = imageGeneration->getScaleDataImageBitmap()->GetWidth();
//         originalHeight = imageGeneration->getScaleDataImageBitmap()->GetHeight();
//       }
//       else
//       {
//         originalWidth  = imageGeneration->getScaleImageBitmap()->GetWidth();
//         originalHeight = imageGeneration->getScaleImageBitmap()->GetHeight();
//       }


//       drawScaleSingle->SetVirtualSize( originalWidth, originalHeight );
//       drawScaleSingle->SetScrollRate( 10, 10 );
//       drawScaleSingle->ShowScrollbars( wxSHOW_SB_DEFAULT, wxSHOW_SB_DEFAULT );
//       resize_scale = false;
//       // Checkbox is unchecked
//     }
//   }
//   ////@begin wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX in SaveImageDialog.
//   // Before editing this code, remove the block markers.
//   event.Skip();
//   ////@end wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX in SaveImageDialog.
// }


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_LEGEND_CONTROL
 */

// void SaveImageDialog::OnCheckboxLegendControlClick( wxCommandEvent& event )
// {
//   wxCheckBox* checkbox = wxDynamicCast( FindWindow( ID_CHECKBOX_LEGEND_CONTROL ), wxCheckBox );
//   if( checkbox )
//   {
//     bool isChecked = checkbox->IsChecked();
//     if( isChecked )
//     {
//       wxSize clientSize = drawScaleControlSingle->GetClientSize();
//       drawScaleControlSingle->SetVirtualSize( clientSize ); // virtual size = client size → no scroll
//       drawScaleControlSingle->SetScrollRate( 0, 0 );        // disable scroll increments
//       drawScaleControlSingle->ShowScrollbars( wxSHOW_SB_NEVER, wxSHOW_SB_NEVER );
//       resize_scale_control = true;
//       // Checkbox is checked
//     }
//     else
//     {
//       int originalWidth;
//       int originalHeight;

//       if( isHistogram )
//       {
//         originalWidth  = imageGeneration->getScaleImageBitmap()->GetWidth();
//         originalHeight = imageGeneration->getScaleImageBitmap()->GetHeight();
//       }
//       else
//       {
//         originalWidth  = imageGeneration->getScaleImageBitmap()->GetWidth();
//         originalHeight = imageGeneration->getScaleImageBitmap()->GetHeight();
//       }


//       drawScaleControlSingle->SetVirtualSize( originalWidth, originalHeight );
//       drawScaleControlSingle->SetScrollRate( 10, 10 );
//       drawScaleControlSingle->ShowScrollbars( wxSHOW_SB_DEFAULT, wxSHOW_SB_DEFAULT );
//       resize_scale_control = false;
//       // Checkbox is unchecked
//     }
//   }
//   ////@begin wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_LEGEND_CONTROL in SaveImageDialog.
//   // Before editing this code, remove the block markers.
//   event.Skip();
//   ////@end wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_LEGEND_CONTROL in SaveImageDialog.
// }


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SAVELEGENDCONTROLCHECKBOX
 */

void SaveImageDialog::OnSavelegendcontrolcheckboxClick( wxCommandEvent& event )
{
  ////@begin wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SAVELEGENDCONTROLCHECKBOX in SaveImageDialog.
  // Before editing this code, remove the block markers.
  event.Skip();
  ////@end wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SAVELEGENDCONTROLCHECKBOX in SaveImageDialog.
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX
 */

void SaveImageDialog::OnPreviewRealSizeCheckboxClick( wxCommandEvent& event )
{
  // wxCheckBox* checkbox = wxDynamicCast( FindWindow( ID_CHECKBOX ), wxCheckBox );
  // if( !checkbox )
  //   return;

  // bool isChecked = checkbox->IsChecked();

  // Lambda helper para evitar código repetido
  adjustPanelSize( event.IsChecked() );
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_ORIENTATION_LEGEND_CONTROL
 */

void SaveImageDialog::OnOrientationLegendControlSelected( wxCommandEvent& event )
{
  // int selectionIndex     = event.GetSelection(); // index of selected item
  wxString selectionText = event.GetString(); // text of selected item

  if( selectionText == "Horizontal" )
  {
    imageGeneration->setImageOrientation( TScaleImageOrientation::TScaleImageOrientationHorizontal );

    drawScaleSingle->Refresh();
    drawScaleSingle->Update();

    drawScaleControlSingle->Refresh();
    drawScaleControlSingle->Update();

    drawWindowAndScale->Refresh();
    drawWindowAndScale->Update();
  }
  else if( selectionText == "Vertical" )
  {
    imageGeneration->setImageOrientation( TScaleImageOrientation::TScaleImageOrientationVertical );

    drawScaleSingle->Refresh();
    drawScaleSingle->Update();

    drawScaleControlSingle->Refresh();
    drawScaleControlSingle->Update();

    drawWindowAndScale->Refresh();
    drawWindowAndScale->Update();

    //   drawScaleSingle;
    // wxScrolledWindow* drawScaleControlSingle;
  }

  adjustPanelSize( previewRealSizeCheckBox->IsChecked() );
}


/*!
 * wxEVT_PAINT event handler for ID_SCROLLEDWINDOW_IMAGE
 */

void SaveImageDialog::OnPaintWindowSingle( wxPaintEvent& event )
{
  ////@begin wxEVT_PAINT event handler for ID_SCROLLEDWINDOW_IMAGE in SaveImageDialog.
  // Before editing this code, remove the block markers.
  wxPaintDC dc( wxDynamicCast( event.GetEventObject(), wxWindow ) );
  ////@end wxEVT_PAINT event handler for ID_SCROLLEDWINDOW_IMAGE in SaveImageDialog.
  wxPaintDC dcScale( drawWindowSingle );
  drawWindowSingle->PrepareDC( dcScale ); // <--- This line is critical!

  EnableLegendControls( false );

  wxBitmap* originalBmp = imageGeneration->getWindowsImageBitmap();
  if( originalBmp && originalBmp->IsOk() )
  {
    // Convert to wxImage
    wxImage img = originalBmp->ConvertToImage();

    int originalWidth  = originalBmp->GetWidth();
    int originalHeight = originalBmp->GetHeight();

    int scrollContainerHeight = drawWindowSingle->GetClientSize().GetHeight();
    int scrollContainerWidth  = drawWindowSingle->GetClientSize().GetWidth();

    int posY = 0;
    int posX = 0;

    if( resize_window )
    {
      int newWidth;
      int newHeight;

      calculateScaledSize( originalWidth, originalHeight, scrollContainerWidth, scrollContainerHeight, newWidth, newHeight );

      // Resize the image
      wxImage scaledImg = img.Scale( newWidth, newHeight, wxIMAGE_QUALITY_HIGH );

      // Convert back to wxBitmap
      wxBitmap scaledBmp( scaledImg );

      if( ( scrollContainerHeight - newHeight ) > 0 )
        posY = ( scrollContainerHeight - newHeight ) / 2;
      if( ( scrollContainerWidth - newWidth ) > 0 )
        posX = ( scrollContainerWidth - newWidth ) / 2;


      dcScale.DrawBitmap( scaledBmp, posX, posY, true );

      drawWindowSingle->Refresh();
      drawWindowSingle->Update();
    }

    else
    {
      if( ( scrollContainerHeight - originalHeight ) > 0 )
        posY = ( scrollContainerHeight - originalHeight ) / 2;
      if( ( scrollContainerWidth - originalWidth ) > 0 )
        posX = ( scrollContainerWidth - originalWidth ) / 2;
      dcScale.DrawBitmap( *originalBmp, posX, posY, true );

      drawWindowSingle->Refresh();
      drawWindowSingle->Update();
    }
  }
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_IMAGESCALE_SAVE1
 */


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX1
 */

void SaveImageDialog::OnImagePlusLegendCheckboxClick( wxCommandEvent& event )
{
  ////@begin wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX1 in SaveImageDialog.
  // Before editing this code, remove the block markers.
  event.Skip();
  ////@end wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX1 in SaveImageDialog.
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SHRINK_LABEL
 */

void SaveImageDialog::OnCheckboxShrinkLabelClick( wxCommandEvent& event )
{
  imageGeneration->transformToShort( event.IsChecked() );

  drawScaleSingle->Refresh();
  drawScaleSingle->Update();

  drawScaleControlSingle->Refresh();
  drawScaleControlSingle->Update();

  imageGeneration->generateScaleWindowsImage();

  drawWindowAndScale->Refresh();
  drawWindowAndScale->Update();

  adjustPanelSize( previewRealSizeCheckBox->IsChecked() );
}


void SaveImageDialog::adjustPanelSize( bool resizePanel )
{
  auto adjustPanel = [ & ]( wxScrolledWindow* panel, wxBitmap* image, bool fitToWindow, bool& resizeFlag )
  {
    if( fitToWindow )
    {
      wxSize clientSize = panel->GetClientSize();
      panel->SetVirtualSize( clientSize ); // virtual size = client size → no scroll
      panel->SetScrollRate( 0, 0 );        // disable scroll increments
      panel->ShowScrollbars( wxSHOW_SB_NEVER, wxSHOW_SB_NEVER );
      resizeFlag = true;
    }
    else
    {
      if( image )
      {
        panel->SetVirtualSize( image->GetWidth(), image->GetHeight() );
        panel->SetScrollRate( 10, 10 );
        panel->ShowScrollbars( wxSHOW_SB_DEFAULT, wxSHOW_SB_DEFAULT );
      }
      resizeFlag = false;
    }
  };


  wxBitmap* windowImage = imageGeneration->getWindowsImageBitmap();
  adjustPanel( drawWindowSingle, windowImage, !resizePanel, resize_window );

  wxBitmap* scaleImage = imageGeneration->getScaleImageBitmap();

  if( myHistogram != nullptr )
  {
    wxBitmap* scaleControlImage = imageGeneration->getScaleDataImageBitmap();

    if( scaleControlImage != nullptr )
    {
      adjustPanel( drawScaleSingle, scaleControlImage, !resizePanel, resize_scale );
    }
    if( scaleImage != nullptr )
    {
      adjustPanel( drawScaleControlSingle, scaleImage, !resizePanel, resize_scale_control );
    }
  }
  else
  {
    wxBitmap* scaleImage = imageGeneration->getScaleImageBitmap();
    adjustPanel( drawScaleSingle, scaleImage, !resizePanel, resize_scale );
  }


  // Ventana + Escala
  wxBitmap* timelineScaleImage = imageGeneration->getTimelineScaleImage();
  adjustPanel( drawWindowAndScale, timelineScaleImage, !resizePanel, resize_scale_control );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SELECT_LEGEND
 */

void SaveImageDialog::OnShowLegend( wxCommandEvent& event )
{
  wxArrayString choices;
  wxArrayInt preSelections;

  std::map< TSemanticValue, wxString > semanticValues;

  if( myHistogram != nullptr )
  {
    semanticValues = imageGeneration->getScaleDataImage()->getSemanticValues();
  }
  else
  {
    semanticValues = imageGeneration->getScaleImage()->getSemanticValues();
  }


  // SyncWindows::getInstance()->getGroupAvailableProperties( groupId, properties );

  auto tmpIt = 0;
  for( auto& values : semanticValues )
  {
    choices.Add( values.second );


    // if( SyncWindows::getInstance()->isPropertySelected( groupId, prop ) )
    // {
    //   preSelections.Add( tmpIt );
    // }

    tmpIt += 1;
  }

  wxMultiChoiceDialog* dialog = new wxMultiChoiceDialog( nullptr, "Select the element of the legend you want:", "Legend Group Selector", choices );

  std::vector< int > stdSelections = imageGeneration->getScaleImage()->getSelectedSemanticValues();

  wxArrayInt wxSelections;
  wxSelections.reserve( stdSelections.size() );
  for( int val : stdSelections )
    wxSelections.Add( val );

  dialog->SetSelections( wxSelections );

  if( dialog->ShowModal() == wxID_OK )
  {
    wxArrayInt selections = dialog->GetSelections();

    std::vector< int > selectedIndices;
    selectedIndices.reserve( selections.size() );
    for( int i = 0; i < (int)selections.size(); ++i )
      selectedIndices.push_back( selections[ i ] );


    if( myHistogram == nullptr )
    {
      imageGeneration->getScaleImage()->setSelectedSemanticValues( selectedIndices );

      imageGeneration->getScaleImage()->initializeConfiguration();
      imageGeneration->getScaleImage()->processImage();

      drawScaleSingle->Refresh();
      drawScaleSingle->Update();
    }
    else
    {
      imageGeneration->getScaleDataImage()->setSelectedSemanticValues( selectedIndices );

      imageGeneration->getScaleDataImage()->initializeConfiguration();
      imageGeneration->getScaleDataImage()->processImage();

      drawScaleControlSingle->Refresh();
      drawScaleControlSingle->Update();
    }


    imageGeneration->getScaleWindowsImage()->setSelectedSemanticValues( selectedIndices );

    imageGeneration->generateScaleWindowsImage();

    drawWindowAndScale->Refresh();
    drawWindowAndScale->Update();


    adjustPanelSize( previewRealSizeCheckBox->IsChecked() );
  }

  delete dialog;
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SAVEIMAGE_COPY_CLIPBOARD_BUTTON
 */

void SaveImageDialog::OnSaveimageCopyClipboardButtonClick( wxCommandEvent& event )
{
  bool tmpCopyToClipboard = true;

  // Common to timeline and histograms
  if( drawWindowSingle->IsShown() )
    imageGeneration->saveImageWindows( _( "" ), tmpCopyToClipboard );
  else if( drawWindowAndScale->IsShown() )
    imageGeneration->saveImageWindowScale( _( "" ), tmpCopyToClipboard );
  else if( myHistogram == nullptr && drawScaleSingle->IsShown() )
  {
    // Only for timelines
    imageGeneration->saveImageScale( _( "" ), false, tmpCopyToClipboard );
  }
  else
  {
    // Only for histograms
    if( drawScaleSingle->IsShown() )
      imageGeneration->saveImageControlScale( _( "" ), false, tmpCopyToClipboard );
    else if( drawScaleControlSingle->IsShown() && myHistogram->getFirstRowColored() )
      imageGeneration->saveImageScale( _( "" ), false, tmpCopyToClipboard );
  }
}

void SaveImageDialog::EnableLegendControls( bool enable )
{
  widgetTopCompose1->Enable( enable );
  shrinkLabelCheckBox->Enable( enable );
  buttonSelectLegendValues->Enable( enable );
}
