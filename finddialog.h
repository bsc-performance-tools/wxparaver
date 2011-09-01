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

#ifndef _FINDDIALOG_H_
#define _FINDDIALOG_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/spinctrl.h"
#include "wx/statline.h"
////@end includes

#include "window.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxSpinCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_FINDDIALOG 10171
#define ID_RADIOOBJECTS 10178
#define ID_CHOICEOBJECT 10172
#define ID_CHOICEPOSITION 10173
#define ID_CHECKNEXTOBJECT 10181
#define ID_RADIOEVENTS 10174
#define ID_STATICTYPE 10179
#define ID_CHOICEEVENTS 10175
#define ID_RADIOSEMANTIC 10176
#define ID_STATICSEMANTICVALUE 10180
#define ID_COMBOSEMANTICVALUE 10177
#define ID_STATICSEMANTICDURATION 10184
#define ID_CHOICEDURATIONFUNCTION 10182
#define ID_TEXTSEMANTICDURATION 10183
#define SYMBOL_FINDDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_FINDDIALOG_TITLE _("Find")
#define SYMBOL_FINDDIALOG_IDNAME ID_FINDDIALOG
#define SYMBOL_FINDDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_FINDDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * FindDialog class declaration
 */

class FindDialog: public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( FindDialog )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  FindDialog();
  FindDialog( wxWindow* parent, wxWindowID id = SYMBOL_FINDDIALOG_IDNAME, const wxString& caption = SYMBOL_FINDDIALOG_TITLE, const wxPoint& pos = SYMBOL_FINDDIALOG_POSITION, const wxSize& size = SYMBOL_FINDDIALOG_SIZE, long style = SYMBOL_FINDDIALOG_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_FINDDIALOG_IDNAME, const wxString& caption = SYMBOL_FINDDIALOG_TITLE, const wxPoint& pos = SYMBOL_FINDDIALOG_POSITION, const wxSize& size = SYMBOL_FINDDIALOG_SIZE, long style = SYMBOL_FINDDIALOG_STYLE );

  /// Destructor
  ~FindDialog();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin FindDialog event handler declarations

  /// wxEVT_UPDATE_UI event handler for ID_CHECKNEXTOBJECT
  void OnChecknextobjectUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_STATICTYPE
  void OnStatictypeUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_CHOICEEVENTS
  void OnChoiceeventsUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_STATICSEMANTICVALUE
  void OnStaticsemanticvalueUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_COMBOSEMANTICVALUE
  void OnCombosemanticvalueUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_STATICSEMANTICDURATION
  void OnStaticsemanticdurationUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_CHOICEDURATIONFUNCTION
  void OnChoicedurationfunctionUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TEXTSEMANTICDURATION
  void OnTextsemanticdurationUpdate( wxUpdateUIEvent& event );

////@end FindDialog event handler declarations

////@begin FindDialog member function declarations

  Window * GetMyWindow() const { return myWindow ; }
  void SetMyWindow(Window * value) { myWindow = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end FindDialog member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  void InitControlsBeforeShow();
  
////@begin FindDialog member variables
  wxRadioButton* radioObjects;
  wxListBox* choiceObjects;
  wxChoice* choicePosition;
  wxCheckBox* checkNextObject;
  wxRadioButton* radioEvents;
  wxChoice* choiceEventType;
  wxRadioButton* radioSemantic;
  wxComboBox* comboSemanticValue;
  wxChoice* choiceDurationFunction;
  wxSpinCtrl* spinSemanticDuration;
private:
  Window * myWindow;
////@end FindDialog member variables
};

#endif
  // _FINDDIALOG_H_
