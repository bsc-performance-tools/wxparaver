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

#ifndef _SESSIONSELECTIONDIALOG_H_
#define _SESSIONSELECTIONDIALOG_H_


/*!
 * Includes
 */

////@begin includes
////@end includes
#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/dir.h>
#include <vector>
#include <map>
#include <algorithm>
#include "boost/date_time/posix_time/posix_time.hpp"
using namespace std;

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_SESSIONSELECTIONDIALOG 10000
#define ID_SESSIONBOX 10001
#define SYMBOL_SESSIONSELECTIONDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_SESSIONSELECTIONDIALOG_TITLE _("Session Selection Dialog")
#define SYMBOL_SESSIONSELECTIONDIALOG_IDNAME ID_SESSIONSELECTIONDIALOG
#define SYMBOL_SESSIONSELECTIONDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_SESSIONSELECTIONDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * SessionSelectionDialog class declaration
 */

class SessionSelectionDialog: public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( SessionSelectionDialog )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  SessionSelectionDialog();
  SessionSelectionDialog( wxWindow* parent, wxString folderPath, wxWindowID id = SYMBOL_SESSIONSELECTIONDIALOG_IDNAME, const wxString& caption = SYMBOL_SESSIONSELECTIONDIALOG_TITLE, const wxPoint& pos = SYMBOL_SESSIONSELECTIONDIALOG_POSITION, const wxSize& size = SYMBOL_SESSIONSELECTIONDIALOG_SIZE, long style = SYMBOL_SESSIONSELECTIONDIALOG_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxString folderPath, wxWindowID id = SYMBOL_SESSIONSELECTIONDIALOG_IDNAME, const wxString& caption = SYMBOL_SESSIONSELECTIONDIALOG_TITLE, const wxPoint& pos = SYMBOL_SESSIONSELECTIONDIALOG_POSITION, const wxSize& size = SYMBOL_SESSIONSELECTIONDIALOG_SIZE, long style = SYMBOL_SESSIONSELECTIONDIALOG_STYLE );

  /// Destructor
  ~SessionSelectionDialog();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();
  
////@begin SessionSelectionDialog event handler declarations

  /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_SESSIONBOX
  void OnSessionboxSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_LISTBOX_DOUBLECLICKED event handler for ID_SESSIONBOX
  void OnSessionboxDoubleClicked( wxCommandEvent& event );

  /// wxEVT_CREATE event handler for ID_SESSIONBOX
  void OnCreate( wxWindowCreateEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
  void OnCancelClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
  void OnOkClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for wxID_OK
  void OnOkUpdate( wxUpdateUIEvent& event );

////@end SessionSelectionDialog event handler declarations
  bool OnCreate();

////@begin SessionSelectionDialog member function declarations

  std::map< wxString, wxString > GetLinksPerFileName() const { return linksPerFileName ; }
  void SetLinksPerFileName(std::map< wxString, wxString > value) { linksPerFileName = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end SessionSelectionDialog member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();
  std::string GetSessionPath();

////@begin SessionSelectionDialog member variables
  wxListBox* listSessions;
  wxButton* buttonCancel;
  wxButton* buttonLoad;
private:
  std::map< wxString, wxString > linksPerFileName;
////@end SessionSelectionDialog member variables

  wxString myPath;
  wxString folderPath;
  wxString FormatFileName( wxString fileName );
};

#endif
  // _SESSIONSELECTIONDIALOG_H_
