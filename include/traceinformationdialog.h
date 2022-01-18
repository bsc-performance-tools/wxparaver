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

#ifndef _TRACEINFORMATIONDIALOG_H_
#define _TRACEINFORMATIONDIALOG_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/richtext/richtextctrl.h"
////@end includes
#include "trace.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxBoxSizer;
class wxRichTextCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_TRACEINFORMATIONDIALOG 10000
#define wxID_GTI_STATIC 10298
#define ID_GENERAL_RICHTEXTCTRL 10001
#define wxID_PMI_STATIC 10296
#define ID_PROCESS_RICHTEXTCTRL 10002
#define wxID_RMI_STATIC 10297
#define ID_RESOURCE_RICHTEXTCTRL 10003
#define SYMBOL_TRACEINFORMATIONDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_TRACEINFORMATIONDIALOG_TITLE _("Trace Information Dialog")
#define SYMBOL_TRACEINFORMATIONDIALOG_IDNAME ID_TRACEINFORMATIONDIALOG
#define SYMBOL_TRACEINFORMATIONDIALOG_SIZE wxDefaultSize
#define SYMBOL_TRACEINFORMATIONDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * TraceInformationDialog class declaration
 */

class TraceInformationDialog: public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( TraceInformationDialog )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  TraceInformationDialog( );
  TraceInformationDialog( wxWindow* parent, Trace* whichTrace, wxWindowID id = SYMBOL_TRACEINFORMATIONDIALOG_IDNAME, const wxString& caption = SYMBOL_TRACEINFORMATIONDIALOG_TITLE, const wxPoint& pos = SYMBOL_TRACEINFORMATIONDIALOG_POSITION, const wxSize& size = SYMBOL_TRACEINFORMATIONDIALOG_SIZE, long style = SYMBOL_TRACEINFORMATIONDIALOG_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_TRACEINFORMATIONDIALOG_IDNAME, const wxString& caption = SYMBOL_TRACEINFORMATIONDIALOG_TITLE, const wxPoint& pos = SYMBOL_TRACEINFORMATIONDIALOG_POSITION, const wxSize& size = SYMBOL_TRACEINFORMATIONDIALOG_SIZE, long style = SYMBOL_TRACEINFORMATIONDIALOG_STYLE );

  /// Destructor
  ~TraceInformationDialog();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin TraceInformationDialog event handler declarations

////@end TraceInformationDialog event handler declarations

////@begin TraceInformationDialog member function declarations

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end TraceInformationDialog member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

////@begin TraceInformationDialog member variables
  wxBoxSizer* GeneralInfoSizer;
  wxRichTextCtrl* TraceGeneralInfo;
  wxBoxSizer* ProcessModelSizer;
  wxRichTextCtrl* ProcessModelInfo;
  wxBoxSizer* ResourceModelSizer;
  wxRichTextCtrl* ResourceModelInfo;
////@end TraceInformationDialog member variables

  Trace* myTrace;

private:
  void DisplayTraceInformation();
};

#endif
  // _TRACEINFORMATIONDIALOG_H_
