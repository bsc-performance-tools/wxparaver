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
#define ID_FINDDIALOG 10171
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
////@end FindDialog event handler declarations

////@begin FindDialog member function declarations
  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end FindDialog member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

////@begin FindDialog member variables
////@end FindDialog member variables
};

#endif
  // _FINDDIALOG_H_
