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

#ifndef _EVENTSSELECTIONDIALOG_H_
#define _EVENTSSELECTIONDIALOG_H_


/*!
 * Includes
 */

#include <vector>
using namespace std;

#include "paraverkerneltypes.h"

////@begin includes
#include "wx/statline.h"
////@end includes



/*!
 * Forward declarations
 */

////@begin forward declarations
class wxBoxSizer;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_EVENTSSELECTIONDIALOG 10053
#define ID_CHOICE_OPERATOR_SELECT_TYPES 10054
#define ID_CHOICE_COMBINED_SELECTION 10055
#define ID_CHOICE_OPERATOR_SELECT_VALUES 10056
#define ID_CHECKLISTBOX_TYPES 10161
#define ID_BUTTON_SET_ALL_TYPES 10163
#define ID_BUTTON_UNSET_ALL_TYPES 10164
#define ID_CHECKLISTBOX_VALUES 10162
#define ID_BUTTON_SET_ALL_VALUES 10165
#define ID_BUTTON_UNSET_ALL_VALUES 10166
#define SYMBOL_EVENTSSELECTIONDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_EVENTSSELECTIONDIALOG_TITLE _("Events Selection")
#define SYMBOL_EVENTSSELECTIONDIALOG_IDNAME ID_EVENTSSELECTIONDIALOG
#define SYMBOL_EVENTSSELECTIONDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_EVENTSSELECTIONDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * EventsSelectionDialog class declaration
 */

class EventsSelectionDialog: public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( EventsSelectionDialog )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  EventsSelectionDialog();
  EventsSelectionDialog( wxWindow* parent,
                         vector<TEventType> types,
                         vector<TEventValue> values,
                         bool hideOperatorsList = false,
                         wxWindowID id = SYMBOL_EVENTSSELECTIONDIALOG_IDNAME,
                         const wxString& caption = SYMBOL_EVENTSSELECTIONDIALOG_TITLE,
                         const wxPoint& pos = SYMBOL_EVENTSSELECTIONDIALOG_POSITION,
                         const wxSize& size = SYMBOL_EVENTSSELECTIONDIALOG_SIZE,
                         long style = SYMBOL_EVENTSSELECTIONDIALOG_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_EVENTSSELECTIONDIALOG_IDNAME, const wxString& caption = SYMBOL_EVENTSSELECTIONDIALOG_TITLE, const wxPoint& pos = SYMBOL_EVENTSSELECTIONDIALOG_POSITION, const wxSize& size = SYMBOL_EVENTSSELECTIONDIALOG_SIZE, long style = SYMBOL_EVENTSSELECTIONDIALOG_STYLE );

  /// Destructor
  ~EventsSelectionDialog();

  /// Initialises member variables
  void Init( bool whichHideOperatorsList );

  /// Creates the controls and sizers
  void CreateControls();

////@begin EventsSelectionDialog event handler declarations

////@end EventsSelectionDialog event handler declarations

////@begin EventsSelectionDialog member function declarations

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end EventsSelectionDialog member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

////@begin EventsSelectionDialog member variables
  wxStaticText* textWindowName;
  wxBoxSizer* boxSizerOperatorsChoice;
  wxChoice* choiceOperatorSelectTypes;
  wxChoice* combinedSelection;
  wxChoice* choiceOperatorSelectValues;
  wxCheckListBox* checkboxSelectTypes;
  wxButton* buttonSetAllTypes;
  wxButton* buttonUnsetAllTypes;
  wxCheckListBox* checkboxSelectValues;
  wxButton* buttonSetAllValues;
  wxButton* buttonUnsetAllValues;
////@end EventsSelectionDialog member variables



  bool hideOperatorsList; // let the dialog show or not the type/values/and-or selectors
                          // Useful when called from filters
  vector<TEventType>  types;
  vector<TEventValue> values;
};

#endif
  // _EVENTSSELECTIONDIALOG_H_
