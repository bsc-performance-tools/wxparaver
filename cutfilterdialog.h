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

#ifndef _CUTFILTERDIALOG_H_
#define _CUTFILTERDIALOG_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/filepicker.h"
#include "wx/notebook.h"
#include "wx/spinctrl.h"
#include "wx/statline.h"
////@end includes

using namespace std;
#include <string>
#include <vector>

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxFilePickerCtrl;
class wxSpinCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_CUTFILTERDIALOG 10103
#define ID_FILECTRL_CUTFILTER_TRACE_SELECTION 10106
#define ID_CHECKLISTBOX 10107
#define ID_BITMAPBUTTON_PUSH_UP 10109
#define ID_BITMAPBUTTON_PUSH_DOWN 10110
#define ID_NOTEBOOK_CUT_FILTER_OPTIONS 10108
#define ID_PANEL_CUTTER 10111
#define ID_RADIOBUTTON 10116
#define ID_RADIOBUTTON1 10117
#define ID_TEXTCTRL_BEGIN_CUT 10118
#define ID_TEXTCTRL_END_CUT 10119
#define ID_BUTTON_SELECT_REGION 10114
#define ID_BUTTON_ALL_TRACE 10115
#define ID_CHECKBOX_CHECK_ORIGINAL_TIME 10120
#define ID_CHECKBOX_REMOVE_FIRST_STATE 10121
#define ID_CHECKBOX_BREAK_STATES 10122
#define ID_CHECKBOX_REMOVE_LAST_STATE 10123
#define ID_SPINCTRL_MAXIMUM_SIZE 10147
#define ID_PANEL_FILTER 10112
#define ID_RADIOBUTTON2 10124
#define ID_RADIOBUTTON3 10125
#define ID_RADIOBUTTON4 10126
#define ID_CHECKLISTBOX1 10128
#define ID_BUTTON2 10129
#define ID_BUTTON3 10130
#define ID_BUTTON10 10151
#define ID_LISTBOX 10141
#define ID_BUTTON4 10142
#define ID_BUTTON5 10143
#define ID_SPINCTRL 10127
#define ID_PANEL_SOFTWARE_COUNTERS 10113
#define ID_RADIOBUTTON5 10131
#define ID_RADIOBUTTON6 10132
#define ID_TEXTCTRL5 10133
#define ID_TEXTCTRL6 10134
#define ID_CHECKLISTBOX2 10148
#define ID_BUTTON8 10149
#define ID_BUTTON9 10150
#define ID_RADIOBUTTON7 10135
#define ID_RADIOBUTTON8 10136
#define ID_CHECKBOX9 10137
#define ID_CHECKBOX10 10138
#define ID_CHECKBOX11 10139
#define ID_CHECKBOX12 10140
#define ID_LISTBOX1 10144
#define ID_BUTTON6 10145
#define ID_BUTTON7 10146
#define SYMBOL_CUTFILTERDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_CUTFILTERDIALOG_TITLE _("Cut & Filter")
#define SYMBOL_CUTFILTERDIALOG_IDNAME ID_CUTFILTERDIALOG
#define SYMBOL_CUTFILTERDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_CUTFILTERDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * CutFilterDialog class declaration
 */

class CutFilterDialog: public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( CutFilterDialog )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  CutFilterDialog();
  CutFilterDialog( wxWindow* parent, wxWindowID id = SYMBOL_CUTFILTERDIALOG_IDNAME, const wxString& caption = SYMBOL_CUTFILTERDIALOG_TITLE, const wxPoint& pos = SYMBOL_CUTFILTERDIALOG_POSITION, const wxSize& size = SYMBOL_CUTFILTERDIALOG_SIZE, long style = SYMBOL_CUTFILTERDIALOG_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_CUTFILTERDIALOG_IDNAME, const wxString& caption = SYMBOL_CUTFILTERDIALOG_TITLE, const wxPoint& pos = SYMBOL_CUTFILTERDIALOG_POSITION, const wxSize& size = SYMBOL_CUTFILTERDIALOG_SIZE, long style = SYMBOL_CUTFILTERDIALOG_STYLE );

  /// Destructor
  ~CutFilterDialog();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin CutFilterDialog event handler declarations

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON_PUSH_UP
  void OnBitmapbuttonPushUpClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON_PUSH_DOWN
  void OnBitmapbuttonPushDownClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_CHECK_ORIGINAL_TIME
  void OnCheckOriginalTimeClick( wxCommandEvent& event );

////@end CutFilterDialog event handler declarations

////@begin CutFilterDialog member function declarations

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end CutFilterDialog member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  void UpdateToolList();

////@begin CutFilterDialog member variables
  wxFilePickerCtrl* filePickerTrace;
  wxCheckListBox* checkListToolOrder;
  wxBitmapButton* buttonUp;
  wxBitmapButton* buttonDown;
  wxRadioButton* radioButtonCutByTime;
  wxRadioButton* radioButtonCutByPercent;
  wxTextCtrl* textBeginCut;
  wxTextCtrl* textEndCut;
  wxButton* buttonSelectRegion;
  wxButton* buttonAllTrace;
  wxCheckBox* checkUseOriginalTime;
  wxCheckBox* checkRemoveFirstState;
  wxCheckBox* checkBreakStates;
  wxCheckBox* checkRemoveLastState;
  wxSpinCtrl* textMaximumTraceSize;
////@end CutFilterDialog member variables

  vector< string > listToolOrder; // Names of the tools
};

#endif
  // _CUTFILTERDIALOG_H_
