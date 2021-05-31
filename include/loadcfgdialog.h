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

#ifndef _LOADCFGDIALOG_H_
#define _LOADCFGDIALOG_H_


/*!
 * Includes
 */

#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/dir.h>
#include <map>
#include "cfg.h"

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
#define ID_LOADCFGDIALOG 10000
#define ID_SEARCHCTRL 10004
#define ID_DIRCTRL 10001
#define ID_LISTBOX 10002
#define ID_TEXTDESCRCFG 10003
#define SYMBOL_LOADCFGDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_LOADCFGDIALOG_TITLE _("Load Configuration")
#define SYMBOL_LOADCFGDIALOG_IDNAME ID_LOADCFGDIALOG
#define SYMBOL_LOADCFGDIALOG_SIZE wxSize(800, 600)
#define SYMBOL_LOADCFGDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!A.
 * LoadCFGDialog class declaration
 */

class LoadCFGDialog: public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( LoadCFGDialog )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  LoadCFGDialog();
  LoadCFGDialog( wxWindow* parent, wxString directoryPath = _( "" ), wxWindowID id = SYMBOL_LOADCFGDIALOG_IDNAME, const wxString& caption = SYMBOL_LOADCFGDIALOG_TITLE, const wxPoint& pos = SYMBOL_LOADCFGDIALOG_POSITION, const wxSize& size = SYMBOL_LOADCFGDIALOG_SIZE, long style = SYMBOL_LOADCFGDIALOG_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_LOADCFGDIALOG_IDNAME, const wxString& caption = SYMBOL_LOADCFGDIALOG_TITLE, const wxPoint& pos = SYMBOL_LOADCFGDIALOG_POSITION, const wxSize& size = SYMBOL_LOADCFGDIALOG_SIZE, long style = SYMBOL_LOADCFGDIALOG_STYLE );


  /// Destructor
  ~LoadCFGDialog();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin LoadCFGDialog event handler declarations

  /// wxEVT_COMMAND_TEXT_ENTER event handler for ID_SEARCHCTRL
  void OnSearchctrlEnter( wxCommandEvent& event );

  /// wxEVT_COMMAND_TREE_SEL_CHANGED event handler for ID_DIRCTRL
  void OnDirctrlSelChanged( wxTreeEvent& event );

  /// wxEVT_COMMAND_TREE_ITEM_ACTIVATED event handler for ID_DIRCTRL
  void OnDirctrlItemActivated( wxTreeEvent& event );

  /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX
  void OnListboxSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_LISTBOX_DOUBLECLICKED event handler for ID_LISTBOX
  void OnListboxDoubleClicked( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
  void OnCancelClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
  void OnOkClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for wxID_OK
  void OnOkUpdate( wxUpdateUIEvent& event );

////@end LoadCFGDialog event handler declarations

  wxString GetFilePath();


////@begin LoadCFGDialog member function declarations
  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end LoadCFGDialog member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

////@begin LoadCFGDialog member variables
  wxTextCtrl* searchBar;
  wxGenericDirCtrl* treeDirs;
  wxListBox* listDirs;
  wxTextCtrl* textDescription;
  wxButton* buttonCancel;
  wxButton* buttonLoad;
////@end LoadCFGDialog member variables
  wxString directoryStartingPath;
  wxString selectedCfgFilePath;
  std::map< wxString, wxString > linksPerFileName;
  
};

#endif
  // _LOADCFGDIALOG_H_
