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

////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_EXITDIALOG 10000
#define ID_BUTTON_SAVE_EXIT 10253
#define ID_BUTTON_CANCEL 10252
#define ID_BUTTON_CLOSE_NO_SAVE 10251
#define SYMBOL_EXITDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_EXITDIALOG_TITLE _("Exit Dialog")
#define SYMBOL_EXITDIALOG_IDNAME ID_EXITDIALOG
#define SYMBOL_EXITDIALOG_SIZE wxDefaultSize
#define SYMBOL_EXITDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * ExitDialog class declaration
 */

class ExitDialog: public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( ExitDialog )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  ExitDialog();
  ExitDialog( wxWindow* parent, wxWindowID id = SYMBOL_EXITDIALOG_IDNAME, const wxString& caption = SYMBOL_EXITDIALOG_TITLE, const wxPoint& pos = SYMBOL_EXITDIALOG_POSITION, const wxSize& size = SYMBOL_EXITDIALOG_SIZE, long style = SYMBOL_EXITDIALOG_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_EXITDIALOG_IDNAME, const wxString& caption = SYMBOL_EXITDIALOG_TITLE, const wxPoint& pos = SYMBOL_EXITDIALOG_POSITION, const wxSize& size = SYMBOL_EXITDIALOG_SIZE, long style = SYMBOL_EXITDIALOG_STYLE );

  /// Destructor
  ~ExitDialog();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin ExitDialog event handler declarations

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SAVE_EXIT
  void OnButtonSaveExitClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CANCEL
  void OnButtonCancelClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CLOSE_NO_SAVE
  void OnButtonCloseNoSaveClick( wxCommandEvent& event );

////@end ExitDialog event handler declarations

////@begin ExitDialog member function declarations

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end ExitDialog member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

////@begin ExitDialog member variables
  wxButton* saveExitButton;
  wxButton* cancelButton;
  wxButton* noSaveExitButton;
////@end ExitDialog member variables
};
