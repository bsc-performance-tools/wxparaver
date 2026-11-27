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
#include "wx/statline.h"
////@end includes
// clang-format on

#include <functional>
#include <map>
#include <memory>
#include <vector>

#include <wx/dialog.h>
#include <wx/scrolwin.h>
#include <wx/stopwatch.h>
#include <wx/utils.h>

/*!
 * Forward declarations
 */
// clang-format off
////@begin forward declarations
class wxBoxSizer;
////@end forward declarations
// clang-format on

class wxGauge;
class wxStaticText;
class wxActivityIndicator;
class ProgressController;

/*!
 * Control identifiers
 */
// clang-format off
////@begin control identifiers
#define ID_PROGRESSLISTDIALOG 10607
#define ID_SCROLLEDWINDOW_PROGRESS_ROWS 10608
#define ID_BUTTON_CANCEL_ALL 10609
#define SYMBOL_PROGRESSLISTDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_PROGRESSLISTDIALOG_TITLE _("Drawing windows")
#define SYMBOL_PROGRESSLISTDIALOG_IDNAME ID_PROGRESSLISTDIALOG
#define SYMBOL_PROGRESSLISTDIALOG_SIZE wxSize(600, 350)
#define SYMBOL_PROGRESSLISTDIALOG_POSITION wxDefaultPosition
////@end control identifiers
// clang-format on


/*!
 * ProgressListDialog class declaration
 *
 * Shared dialog with one row per view being drawn, pending or finished.
 * Rows are created at runtime, so DialogBlocks only holds the container.
 */
class ProgressListDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS( ProgressListDialog )
    DECLARE_EVENT_TABLE()

  public:
    struct PendingView
    {
      void* key;
      wxString title;
    };

    /// Constructors
    ProgressListDialog();
    ProgressListDialog( wxWindow* parent,
                        wxWindowID id           = SYMBOL_PROGRESSLISTDIALOG_IDNAME,
                        const wxString& caption = SYMBOL_PROGRESSLISTDIALOG_TITLE,
                        const wxPoint& pos      = SYMBOL_PROGRESSLISTDIALOG_POSITION,
                        const wxSize& size      = SYMBOL_PROGRESSLISTDIALOG_SIZE,
                        long style              = SYMBOL_PROGRESSLISTDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent,
                 wxWindowID id           = SYMBOL_PROGRESSLISTDIALOG_IDNAME,
                 const wxString& caption = SYMBOL_PROGRESSLISTDIALOG_TITLE,
                 const wxPoint& pos      = SYMBOL_PROGRESSLISTDIALOG_POSITION,
                 const wxSize& size      = SYMBOL_PROGRESSLISTDIALOG_SIZE,
                 long style              = SYMBOL_PROGRESSLISTDIALOG_STYLE );

    /// Destructor
    ~ProgressListDialog();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    // clang-format off
  ////@begin ProgressListDialog event handler declarations

  /// wxEVT_CLOSE_WINDOW event handler for ID_PROGRESSLISTDIALOG
  void OnCloseWindow( wxCloseEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CANCEL_ALL
  void OnButtonCancelAllClick( wxCommandEvent& event );

  ////@end ProgressListDialog event handler declarations
    // clang-format on

    // clang-format off
  ////@begin ProgressListDialog member function declarations

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
  ////@end ProgressListDialog member function declarations
    // clang-format on

    /// Should we show tooltips?
    static bool ShowToolTips();

    static ProgressListDialog* getInstance( wxWindow* parent );

    static bool isSomeViewRunning();

    void syncPending( const std::vector< PendingView >& pending );

    void setRunning( void* key, const wxString& title );

    void finish( void* key, bool wasStopped );

    void setProgress( void* key, int value, const wxString& message );

    bool isCancelled( void* key ) const;

    // clang-format off
  ////@begin ProgressListDialog member variables
  wxScrolledWindow* rowsPanel;
  wxBoxSizer* rowsSizer;
  wxButton* cancelAllButton;
  ////@end ProgressListDialog member variables
    // clang-format on

  private:
    enum class RowState
    {
      PENDING,
      RUNNING,
      DONE,
      CANCELLED
    };

    struct Row
    {
      wxPanel* panel;
      wxActivityIndicator* spinner;
      wxStaticText* icon;
      wxStaticText* label;
      wxStaticText* message;
      wxStaticText* elapsed;
      wxStaticText* estimated;
      wxStaticText* remaining;
      wxGauge* gauge;
      wxButton* cancelButton;
      RowState state;
      bool cancelRequested;
      wxStopWatch watch;
    };

    static const int ICON_WIDTH       = 20;
    static const int MAX_VISIBLE_ROWS = 4;

    static ProgressListDialog* instance;

    std::map< void*, Row > rows;
    bool cancelAll;
    int fittedHeight;

    void startBatchIfNeeded();
    bool hasActiveRows() const;
    void addRow( void* key, const wxString& title );
    void removeRow( void* key );
    void cancelRow( Row& whichRow );
    void cancelEverything();
    void showSpinner( Row& whichRow );
    void showSymbol( Row& whichRow, const wxString& symbol, const wxColour& colour );
    void showCancelledSymbol( Row& whichRow );
    void setRowState( Row& whichRow, RowState newState );
    void updateTimes( Row& whichRow, double fraction );
    void scrollToRow( const Row& whichRow );
    void updateTitle();
    void relayout();
    void fitToRows();
};


// Progress handler for ProgressController::setHandler.
void progressListFunction( void* whichKey, ProgressController* progress );


// Shows a view in ProgressListDialog while it is drawn and disables the rest
// of the application meanwhile, like the old app-modal wxProgressDialog.
class ProgressListScope
{
  public:
    ProgressListScope( wxWindow* parent,
                       void* key,
                       const wxString& title,
                       ProgressController* progress,
                       std::function< std::vector< ProgressListDialog::PendingView >() > whichPendingViews );
    ~ProgressListScope();

    ProgressListScope( const ProgressListScope& )            = delete;
    ProgressListScope& operator=( const ProgressListScope& ) = delete;

  private:
    ProgressListDialog* dialog;
    void* key;
    ProgressController* progress;
    std::function< std::vector< ProgressListDialog::PendingView >() > pendingViews;
    std::unique_ptr< wxWindowDisabler > disabler;
};
