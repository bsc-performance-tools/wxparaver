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


/*!
 * Includes
 */
#include "cfg.h"

#include <map>
#include <wx/dir.h>
#include <wx/filectrl.h>
#include <wx/filename.h>
#include <wx/textfile.h>
// clang-format off
////@begin includes
////@end includes
// clang-format on

/*!
 * Forward declarations
 */
// clang-format off
////@begin forward declarations
class wxFileCtrl;
////@end forward declarations
// clang-format on

/*!
 * Control identifiers
 */
// clang-format off
////@begin control identifiers
#define ID_LOADCFGDIALOG 10000
#define ID_SEARCHCTRL 10004
#define ID_FILE_NAVIGATOR 10001
#define ID_TEXTLOADDESCRIPTION 10003
#define SYMBOL_LOADCFGDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_LOADCFGDIALOG_TITLE _("Load CFG Dialog")
#define SYMBOL_LOADCFGDIALOG_IDNAME ID_LOADCFGDIALOG
#define SYMBOL_LOADCFGDIALOG_SIZE wxSize(800, 600)
#define SYMBOL_LOADCFGDIALOG_POSITION wxDefaultPosition
////@end control identifiers
// clang-format on

/*!
 * LoadCFGDialog class declaration
 */

class LoadCFGDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS( LoadCFGDialog )
    DECLARE_EVENT_TABLE()

  public:
    /// Constructors
    LoadCFGDialog();
    LoadCFGDialog( wxWindow* parent,
                   wxString directoryPath  = _( "" ),
                   wxWindowID id           = SYMBOL_LOADCFGDIALOG_IDNAME,
                   const wxString& caption = SYMBOL_LOADCFGDIALOG_TITLE,
                   const wxPoint& pos      = SYMBOL_LOADCFGDIALOG_POSITION,
                   const wxSize& size      = SYMBOL_LOADCFGDIALOG_SIZE,
                   long style              = SYMBOL_LOADCFGDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent,
                 wxWindowID id           = SYMBOL_LOADCFGDIALOG_IDNAME,
                 const wxString& caption = SYMBOL_LOADCFGDIALOG_TITLE,
                 const wxPoint& pos      = SYMBOL_LOADCFGDIALOG_POSITION,
                 const wxSize& size      = SYMBOL_LOADCFGDIALOG_SIZE,
                 long style              = SYMBOL_LOADCFGDIALOG_STYLE );


    /// Destructor
    ~LoadCFGDialog();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    // clang-format off
  ////@begin LoadCFGDialog event handler declarations

  /// wxEVT_COMMAND_TEXT_ENTER event handler for ID_SEARCHCTRL
  void OnSearchctrlEnter( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TEXTLOADDESCRIPTION
  void OnTextloaddescriptionUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
  void OnCancelClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
  void OnOkClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for wxID_OK
  void OnOkUpdate( wxUpdateUIEvent& event );

  ////@end LoadCFGDialog event handler declarations
    // clang-format on

    void OnFileNavigatorDoubleClick( wxFileCtrlEvent& event );

    wxString GetFilePath();

    // clang-format off
  ////@begin LoadCFGDialog member function declarations

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
  ////@end LoadCFGDialog member function declarations
    // clang-format on

    /// Should we show tooltips?
    static bool ShowToolTips();

    // clang-format off
  ////@begin LoadCFGDialog member variables
  wxTextCtrl* searchBar;
  wxFileCtrl* fileNavigator;
  wxTextCtrl* textDescription;
  wxButton* buttonCancel;
  wxButton* buttonLoad;
  ////@end LoadCFGDialog member variables
    // clang-format on
    wxString directoryStartingPath;
    wxString selectedCfgFilePath;
    std::map< wxString, wxString > linksPerFileName;
};
