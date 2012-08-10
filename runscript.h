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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#ifndef _RUNSCRIPT_H_
#define _RUNSCRIPT_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/filepicker.h"
#include "wx/statline.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxFilePickerCtrl;
class wxBoxSizer;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_RUN_APPLICATION 10110
#define ID_FILEPICKERCTRL_SCRIPT 10200
#define ID_BUTTON_EDIT_APPLICATION 10204
#define ID_FILEPICKERCTRL_TRACE 10201
#define ID_TEXTCTRL_DEFAULT_PARAMETERS 10205
#define ID_BUTTON_HELP_SCRIPT 10207
#define ID_BUTTON_RUN 10203
#define ID_BUTTON_CLEAR_LOG 10202
#define ID_LISTBOX_RUN_LOG 10199
#define ID_BUTTON_EXIT 10206
#define SYMBOL_RUNSCRIPT_STYLE wxCAPTION|wxRESIZE_BORDER|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_RUNSCRIPT_TITLE _("Run Application")
#define SYMBOL_RUNSCRIPT_IDNAME ID_RUN_APPLICATION
#define SYMBOL_RUNSCRIPT_SIZE wxSize(400, 300)
#define SYMBOL_RUNSCRIPT_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * RunScript class declaration
 */

class RunScript: public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( RunScript )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  RunScript();
  RunScript( wxWindow* parent, wxWindowID id = SYMBOL_RUNSCRIPT_IDNAME, const wxString& caption = SYMBOL_RUNSCRIPT_TITLE, const wxPoint& pos = SYMBOL_RUNSCRIPT_POSITION, const wxSize& size = SYMBOL_RUNSCRIPT_SIZE, long style = SYMBOL_RUNSCRIPT_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_RUNSCRIPT_IDNAME, const wxString& caption = SYMBOL_RUNSCRIPT_TITLE, const wxPoint& pos = SYMBOL_RUNSCRIPT_POSITION, const wxSize& size = SYMBOL_RUNSCRIPT_SIZE, long style = SYMBOL_RUNSCRIPT_STYLE );

  /// Destructor
  ~RunScript();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin RunScript event handler declarations

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_EXIT
  void OnButtonExitClick( wxCommandEvent& event );

////@end RunScript event handler declarations

////@begin RunScript member function declarations

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end RunScript member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

////@begin RunScript member variables
  wxComboBox* filePickerScript;
  wxButton* buttonEditApplication;
  wxFilePickerCtrl* filePickerCtrl_trace;
  wxBoxSizer* boxSizerParameters;
  wxTextCtrl* textCtrlDefaultParameters;
  wxButton* buttonHelpScript;
  wxButton* buttonRun;
  wxButton* buttonClearLog;
  wxListBox* listboxRunLog;
  wxButton* buttonExit;
////@end RunScript member variables
};

#endif
  // _RUNSCRIPT_H_
