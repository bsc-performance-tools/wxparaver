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

#ifndef _SAVEIMAGEDIALOG_H_
#define _SAVEIMAGEDIALOG_H_


/*!
 * Includes
 */

#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/dir.h>
#include <map>
////@begin includes
#include "wx/dirctrl.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxGenericDirCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_SAVEIMAGEDIALOG 10000
#define ID_SAVESEARCHTEXTCTRL 10510
#define ID_FILEPATHSAVEIMGCTRL 10501
#define ID_SAVEDIRCTRL 10502
#define ID_SAVELISTBOX 10503
#define ID_FILETYPECHOICE 10001
#define ID_SAVEIMAGECHECKBOX 10504
#define ID_SAVEIMAGETEXTCTRL 10505
#define ID_SAVELEGENDCHECKBOX 10506
#define ID_SAVELEGENDTEXTCTRL 10507
#define SYMBOL_SAVEIMAGEDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_SAVEIMAGEDIALOG_TITLE _("Save Image Dialog")
#define SYMBOL_SAVEIMAGEDIALOG_IDNAME ID_SAVEIMAGEDIALOG
#define SYMBOL_SAVEIMAGEDIALOG_SIZE wxSize(800, 600)
#define SYMBOL_SAVEIMAGEDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * SaveImageDialog class declaration
 */

class SaveImageDialog: public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( SaveImageDialog )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  SaveImageDialog();
  SaveImageDialog( wxWindow* parent, wxString& directoryStartingPath, wxString defaultFileName, bool isHistogram = false, wxString legendSuffix = _( "_legend" ), wxWindowID id = SYMBOL_SAVEIMAGEDIALOG_IDNAME, const wxString& caption = SYMBOL_SAVEIMAGEDIALOG_TITLE, const wxPoint& pos = SYMBOL_SAVEIMAGEDIALOG_POSITION, const wxSize& size = SYMBOL_SAVEIMAGEDIALOG_SIZE, long style = SYMBOL_SAVEIMAGEDIALOG_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SAVEIMAGEDIALOG_IDNAME, const wxString& caption = SYMBOL_SAVEIMAGEDIALOG_TITLE, const wxPoint& pos = SYMBOL_SAVEIMAGEDIALOG_POSITION, const wxSize& size = SYMBOL_SAVEIMAGEDIALOG_SIZE, long style = SYMBOL_SAVEIMAGEDIALOG_STYLE );

  /// Destructor
  ~SaveImageDialog();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();


  wxString GetImageFilePath();
  wxString GetLegendFilePath();

  bool DialogSavesImage();
  bool DialogSavesLegend();
  int GetFilterIndex(); // Should it be ParaverConfig::TImageFormat ???

////@begin SaveImageDialog event handler declarations

  /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SAVESEARCHTEXTCTRL
  void OnSavesearchtextctrlTextUpdated( wxCommandEvent& event );

  /// wxEVT_COMMAND_TEXT_ENTER event handler for ID_SAVESEARCHTEXTCTRL
  void OnSavesearchtextctrlEnter( wxCommandEvent& event );

  /// wxEVT_COMMAND_TEXT_ENTER event handler for ID_FILEPATHSAVEIMGCTRL
  void OnFilepathsaveimgctrlEnter( wxCommandEvent& event );

  /// wxEVT_COMMAND_TREE_SEL_CHANGED event handler for ID_SAVEDIRCTRL
  void OnSavedirctrlSelChanged( wxTreeEvent& event );

  /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_SAVELISTBOX
  void OnSavelistboxSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_LISTBOX_DOUBLECLICKED event handler for ID_SAVELISTBOX
  void OnSavelistboxDoubleClicked( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_FILETYPECHOICE
  void OnFiletypechoiceSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SAVEIMAGECHECKBOX
  void OnSaveimagecheckboxClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SAVELEGENDCHECKBOX
  void OnSavelegendcheckboxClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
  void OnOkClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
  void OnCancelClick( wxCommandEvent& event );

////@end SaveImageDialog event handler declarations

  void updateFileNamesAndPaths();
////@begin SaveImageDialog member function declarations

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end SaveImageDialog member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

////@begin SaveImageDialog member variables
  wxTextCtrl* fileNameBar;
  wxTextCtrl* searchBar;
  wxGenericDirCtrl* treeDirs;
  wxListBox* listDirs;
  wxChoice* fileTypeChoice;
  wxCheckBox* imageCheckbox;
  wxTextCtrl* imageFileName;
  wxCheckBox* legendCheckbox;
  wxTextCtrl* legendFileName;
  wxButton* buttonSave;
  wxButton* buttonCancel;
////@end SaveImageDialog member variables
  wxString directoryStartingPath;
  wxString defaultFileName;
  wxString selectedImageFilePath;
  wxString selectedLegendFilePath;

  std::map< wxString, wxString > linksPerFileName;
  wxString fileTypeText;
  bool isHistogram;
  wxString legendSuffix;
};

#endif
  // _SAVEIMAGEDIALOG_H_
