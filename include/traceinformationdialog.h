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
// clang-format off
////@begin includes
#include "wx/notebook.h"
#include "wx/richtext/richtextctrl.h"
////@end includes
// clang-format on
#include "trace.h"

/*!
 * Forward declarations
 */
// clang-format off
////@begin forward declarations
class wxBoxSizer;
class wxRichTextCtrl;
////@end forward declarations
// clang-format on

/*!
 * Control identifiers
 */
// clang-format off
////@begin control identifiers
#define ID_TRACEINFORMATIONDIALOG 10000
#define ID_NOTEBOOK 10006
#define ID_PANEL_GENERAL 10007
#define ID_GENERAL_RICHTEXTCTRL 10001
#define ID_MTI_STATIC 10004
#define ID_METADATA_RICHTEXTCTRL 10005
#define wxID_PMI_STATIC 10296
#define ID_PROCESS_RICHTEXTCTRL 10002
#define wxID_RMI_STATIC 10297
#define ID_RESOURCE_RICHTEXTCTRL 10003
#define ID_PANEL_EVENTS 10008
#define ID_LISTBOX_TYPES 10009
#define ID_LISTBOX_VALUES 10010
#define SYMBOL_TRACEINFORMATIONDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_TRACEINFORMATIONDIALOG_TITLE _("Trace Information Dialog")
#define SYMBOL_TRACEINFORMATIONDIALOG_IDNAME ID_TRACEINFORMATIONDIALOG
#define SYMBOL_TRACEINFORMATIONDIALOG_SIZE wxSize(400, 500)
#define SYMBOL_TRACEINFORMATIONDIALOG_POSITION wxDefaultPosition
////@end control identifiers
// clang-format on


/*!
 * TraceInformationDialog class declaration
 */

class TraceInformationDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS( TraceInformationDialog )
    DECLARE_EVENT_TABLE()

  public:
    /// Constructors
    TraceInformationDialog();
    TraceInformationDialog( wxWindow* parent,
                            Trace* whichTrace,
                            wxWindowID id           = SYMBOL_TRACEINFORMATIONDIALOG_IDNAME,
                            const wxString& caption = SYMBOL_TRACEINFORMATIONDIALOG_TITLE,
                            const wxPoint& pos      = SYMBOL_TRACEINFORMATIONDIALOG_POSITION,
                            const wxSize& size      = SYMBOL_TRACEINFORMATIONDIALOG_SIZE,
                            long style              = SYMBOL_TRACEINFORMATIONDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent,
                 wxWindowID id           = SYMBOL_TRACEINFORMATIONDIALOG_IDNAME,
                 const wxString& caption = SYMBOL_TRACEINFORMATIONDIALOG_TITLE,
                 const wxPoint& pos      = SYMBOL_TRACEINFORMATIONDIALOG_POSITION,
                 const wxSize& size      = SYMBOL_TRACEINFORMATIONDIALOG_SIZE,
                 long style              = SYMBOL_TRACEINFORMATIONDIALOG_STYLE );

    /// Destructor
    ~TraceInformationDialog();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    // clang-format off
  ////@begin TraceInformationDialog event handler declarations

  /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX_TYPES
  void OnListboxTypesSelected( wxCommandEvent& event );

  ////@end TraceInformationDialog event handler declarations
    // clang-format on

    // clang-format off
  ////@begin TraceInformationDialog member function declarations

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
  ////@end TraceInformationDialog member function declarations
    // clang-format on

    /// Should we show tooltips?
    static bool ShowToolTips();

    // clang-format off
  ////@begin TraceInformationDialog member variables
  wxBoxSizer* sizerMain;
  wxBoxSizer* GeneralInfoSizer;
  wxRichTextCtrl* TraceGeneralInfo;
  wxBoxSizer* MetadataInfoSizer;
  wxRichTextCtrl* MetadataGeneralInfo;
  wxBoxSizer* ProcessModelSizer;
  wxRichTextCtrl* ProcessModelInfo;
  wxBoxSizer* ResourceModelSizer;
  wxRichTextCtrl* ResourceModelInfo;
  wxListBox* listTypes;
  wxListBox* listValues;
  ////@end TraceInformationDialog member variables
    // clang-format on

    Trace* myTrace;

  private:
    void DisplayTraceInformation();
    wxString FormatTraceSize( double traceByteSize );
    int getRackInformation();

    std::vector< TEventType > eventTypes;
};
