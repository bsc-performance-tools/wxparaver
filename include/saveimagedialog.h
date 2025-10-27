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

#pragma once


#include "windowimage.h"
/*!
 * Includes
 */

#include <wx/dir.h>
#include <wx/filectrl.h>
#include <wx/filename.h>
#include <wx/textfile.h>
// clang-format off
////@begin includes
#include "wx/notebook.h"
////@end includes
// clang-format on

/*!
 * Forward declarations
 */
// clang-format off
////@begin forward declarations
class wxBoxSizer;
class wxFileCtrl;
////@end forward declarations
// clang-format on

/*!
 * Control identifiers
 */
// clang-format off
////@begin control identifiers
#define ID_SAVEIMAGEDIALOG 10000
#define ID_TEXTPATH 10501
#define ID_FILENAVIGATOR 10002
#define ID_SAVEIMAGECHECKBOX 10504
#define ID_SAVEIMAGETEXTCTRL 10505
#define ID_SAVELEGENDCHECKBOX 10506
#define ID_SAVELEGENDTEXTCTRL 10507
#define ID_SAVELEGENDCONTROLCHECKBOX 10012
#define ID_SAVELEGENDCONTROLTEXTCTRL 10013
#define ID_CHECKBOX1 10011
#define ID_TEXTCTRL 10015
#define ID_NOTEBOOK 10004
#define ID_SCROLLEDWINDOW1 10003
#define ID_SCROLLEDWINDOW_IMAGE 10005
#define ID_SCROLLEDWINDOW 10001
#define ID_SCROLLEDWINDOW_SCALE 10006
#define ID_SCROLLEDWINDOW2 10009
#define ID_SCROLLEDWINDOW_SCALE_CONTROL 10010
#define ID_SCROLLEDWINDOW3 10007
#define ID_SCROLLEDWINDOWANDCALE 10008
#define ID_CHECKBOX 10014
#define ID_ORIENTATION_LEGEND_CONTROL 10002
#define ID_CHECKBOX_SHRINK_LABEL 10016
#define SYMBOL_SAVEIMAGEDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_SAVEIMAGEDIALOG_TITLE _("Save Image Dialog")
#define SYMBOL_SAVEIMAGEDIALOG_IDNAME ID_SAVEIMAGEDIALOG
#define SYMBOL_SAVEIMAGEDIALOG_SIZE wxDefaultSize
#define SYMBOL_SAVEIMAGEDIALOG_POSITION wxDefaultPosition
////@end control identifiers
// clang-format on


/*!
 * SaveImageDialog class declaration
 */

class SaveImageDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS( SaveImageDialog )
    DECLARE_EVENT_TABLE()

public:
  /// Constructors
  SaveImageDialog();
  SaveImageDialog( wxWindow* parent,
                   wxString& directoryStartingPath,
                   wxString defaultFileName,
                   WindowImageGenerator* imageGeneration,
                   bool isHistogram             = false,
                   wxString legendSuffix        = _( "_legend" ),
                   wxString legendControlSuffix = _( "_legendControl" ),
                   wxWindowID id                = SYMBOL_SAVEIMAGEDIALOG_IDNAME,
                   const wxString& caption      = SYMBOL_SAVEIMAGEDIALOG_TITLE,
                   const wxPoint& pos           = SYMBOL_SAVEIMAGEDIALOG_POSITION,
                   const wxSize& size           = SYMBOL_SAVEIMAGEDIALOG_SIZE,
                   long style                   = SYMBOL_SAVEIMAGEDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent,
                 wxWindowID id           = SYMBOL_SAVEIMAGEDIALOG_IDNAME,
                 const wxString& caption = SYMBOL_SAVEIMAGEDIALOG_TITLE,
                 const wxPoint& pos      = SYMBOL_SAVEIMAGEDIALOG_POSITION,
                 const wxSize& size      = SYMBOL_SAVEIMAGEDIALOG_SIZE,
                 long style              = SYMBOL_SAVEIMAGEDIALOG_STYLE );

    /// Destructor
    ~SaveImageDialog();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();


  wxString GetImageFilePath();
  wxString GetImageScaleFilePath();
  wxString GetLegendFilePath();
  wxString GetLegendControlFilePath();

  bool DialogSavesImage();
  bool DialogSavesLegend();
  bool DialogSavesControlLegend();
  bool DialogSavesImageScale();

  int GetFilterIndex(); // Should it be TImageFormat ???

    // clang-format off
  ////@begin SaveImageDialog event handler declarations

  /// wxEVT_COMMAND_TEXT_ENTER event handler for ID_TEXTPATH
  void OnTextpathEnter( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_FILENAVIGATOR
  void OnFilenavigatorUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SAVEIMAGECHECKBOX
  void OnSaveimagecheckboxClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SAVELEGENDCHECKBOX
  void OnSavelegendcheckboxClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SAVELEGENDCONTROLCHECKBOX
  void OnSavelegendcontrolcheckboxClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX1
  void OnCheckbox1Click( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
  void OnOkClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for wxID_OK
  void OnOkUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
  void OnCancelClick( wxCommandEvent& event );

  /// wxEVT_PAINT event handler for ID_SCROLLEDWINDOW_IMAGE
  void OnPaintWindowSingle( wxPaintEvent& event );

  /// wxEVT_PAINT event handler for ID_SCROLLEDWINDOW_SCALE
  void onPaintScale( wxPaintEvent& event );

  /// wxEVT_PAINT event handler for ID_SCROLLEDWINDOW_SCALE_CONTROL
  void onPaintScaleControl( wxPaintEvent& event );

  /// wxEVT_PAINT event handler for ID_SCROLLEDWINDOWANDCALE
  void OnPaintWindowScale( wxPaintEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX
  void OnCheckboxClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_ORIENTATION_LEGEND_CONTROL
  void OnOrientationLegendControlSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SHRINK_LABEL
  void OnCheckboxShrinkLabelClick( wxCommandEvent& event );

  ////@end SaveImageDialog event handler declarations

    void OnFileNavigatorChanged( wxFileCtrlEvent& event );

  void updateFileNamesAndPaths();
  void setImageFileName();
  void calculateScaledSize( int originalWidth,
                            int originalHeight,
                            int scrollContainerWidth,
                            int scrollContainerHeight,
                            int& newWidth,
                            int& newHeight );

  void adjustPanelSize(bool resizePanel );

  ////@begin SaveImageDialog member function declarations

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
  ////@end SaveImageDialog member function declarations
    // clang-format on

    /// Should we show tooltips?
    static bool ShowToolTips();

    // clang-format off
  ////@begin SaveImageDialog member variables
  wxBoxSizer* sizerMain;
  wxBoxSizer* sizerPath;
  wxTextCtrl* textPath;
  wxFileCtrl* fileNavigator;
  wxStaticBoxSizer* imageToSaveSizer;
  wxBoxSizer* imageSizer;
  wxCheckBox* imageCheckbox;
  wxTextCtrl* imageFileName;
  wxBoxSizer* legendSizer;
  wxCheckBox* legendCheckbox;
  wxTextCtrl* legendFileName;
  wxBoxSizer* legendControlSizer;
  wxCheckBox* legendControlCheckbox;
  wxTextCtrl* legendControlFileName;
  wxBoxSizer* imageScaleControlSizer;
  wxCheckBox* imageAndScaleCheckbox;
  wxTextCtrl* imageScaleFileName;
  wxButton* buttonSave;
  wxButton* buttonCancel;
  wxScrolledWindow* drawWindowSingle;
  wxScrolledWindow* drawScaleSingle;
  wxScrolledWindow* drawScaleControlSingle;
  wxScrolledWindow* drawWindowAndScale;
  wxChoice* widgetTopCompose1;
  ////@end SaveImageDialog member variables

  WindowImageGenerator* imageGeneration;

  static wxString directoryStartingPath;
  wxString defaultFileName;
  wxString selectedImageFilePath;
  wxString selectedLegendFilePath;
  wxString selectedLegendControlFilePath;
  wxString selectedImageScaleFilePath;

  wxString fileTypeText;
  bool isHistogram;
  wxString legendSuffix;
  wxString legendControlSuffix;
  bool resize_window = true;
  bool resize_scale  = true;
  bool resize_scale_control = true;
};
