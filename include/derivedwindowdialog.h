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

#ifndef _DERIVEDHISTOGRAMDIALOG_H_
#define _DERIVEDHISTOGRAMDIALOG_H_


/*!
 * Includes
 */

////@begin includes
////@end includes

#include "histogram.h"
#include "paravermain.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_DERIVEDWINDOWDIALOG 10000
#define ID_TEXTCTRL_DERIVED_HISTOGRAM_NAME 10001
#define ID_SCROLLEDWINDOW_HISTOGRAM_LIST -1
#define ID_BITMAPBUTTON_ADD_PARENT_WINDOW 10008
#define SYMBOL_DERIVEDHISTOGRAMDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_DERIVEDHISTOGRAMDIALOG_TITLE _("Derived Histogram Dialog")
#define SYMBOL_DERIVEDHISTOGRAMDIALOG_IDNAME ID_DERIVEDWINDOWDIALOG
#define SYMBOL_DERIVEDHISTOGRAMDIALOG_SIZE wxSize(600, 400)
#define SYMBOL_DERIVEDHISTOGRAMDIALOG_POSITION wxDefaultPosition
////@end control identifiers

struct TWindowTypeWrapper : public wxObject
{
  TWindowType window;
};

/*!
 * DerivedHistogramDialog class declaration
 */

class DerivedHistogramDialog: public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( DerivedHistogramDialog )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  DerivedHistogramDialog();
  DerivedHistogramDialog( wxWindow* parent, const std::vector< TWindowType >& whichParents, wxWindowID id = SYMBOL_DERIVEDHISTOGRAMDIALOG_IDNAME, const wxString& caption = SYMBOL_DERIVEDHISTOGRAMDIALOG_TITLE, const wxPoint& pos = SYMBOL_DERIVEDHISTOGRAMDIALOG_POSITION, const wxSize& size = SYMBOL_DERIVEDHISTOGRAMDIALOG_SIZE, long style = SYMBOL_DERIVEDHISTOGRAMDIALOG_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_DERIVEDHISTOGRAMDIALOG_IDNAME, const wxString& caption = SYMBOL_DERIVEDHISTOGRAMDIALOG_TITLE, const wxPoint& pos = SYMBOL_DERIVEDHISTOGRAMDIALOG_POSITION, const wxSize& size = SYMBOL_DERIVEDHISTOGRAMDIALOG_SIZE, long style = SYMBOL_DERIVEDHISTOGRAMDIALOG_STYLE );

  /// Destructor
  ~DerivedHistogramDialog();

  /// Initialises member variables
  void Init( const std::vector< TWindowType >& whichHistograms = {} );

  /// Creates the controls and sizers
  void CreateControls();

////@begin DerivedHistogramDialog event handler declarations

  /// wxEVT_COMMAND_TEXT_ENTER event handler for ID_TEXTCTRL_DERIVED_HISTOGRAM_NAME
  void OnTextctrlDerivedHistogramNameEnter( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON_ADD_PARENT_WINDOW
  void OnButtonAddParentClick( wxCommandEvent& event );

////@end DerivedHistogramDialog event handler declarations

////@begin DerivedHistogramDialog member function declarations

  wxString GetDerivedWindowName() const { return derivedWindowName ; }
  void SetDerivedWindowName(wxString value) { derivedWindowName = value ; }

  std::vector< TWindowType > GetParentWindows() const { return parentWindows ; }
  void SetParentWindows(std::vector< TWindowType > value) { parentWindows = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end DerivedHistogramDialog member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

////@begin DerivedHistogramDialog member variables
  wxTextCtrl* derivedHistogramName;
  wxScrolledWindow* histogramsScrolledWindow;
  wxBitmapButton* buttonAddParentWindow;
private:
  wxString derivedWindowName;
  std::vector< TWindowType > parentWindows;
////@end DerivedHistogramDialog member variables

  wxImageList *imagesList;
  // std::vector< std::pair< wxString, std::function< void( wxCommandEvent& ) > > > managedWidgets {}; // saved here after Connect, read in Disconnect
  // std::vector< std::pair< wxString, std::function< void( wxCommandEvent& ) > > > managedWidgets {}; // saved here after Connect, read in Disconnect
  // std::vector< std::pair< wxString, wxObjectEventFunction > > managedWidgets {}; // saved here after Connect, read in Disconnect
  std::vector< std::pair< wxString, int > > managedWidgets {}; // saved here after Connect, read in Disconnect

  wxString BuildName( const TWindowType& whichParent );
  void CreateControlsForParentsList();
  void CreateControlsForParentRow( wxBoxSizer *whichBoxSizer,
                                   const TWindowType& whichParents,
                                   bool showOperation,
                                   const wxBitmap& derivedOperationIcon,
                                   int pos );
  void TransferDataFromPanel();
  void DisconnectWidgets();
  void CleanParentsList();

  // Button callbacks per line
  void OnButtonOperationSelectionClick( wxCommandEvent& event );
  void OnButtonOperationSelectionUpdate( wxUpdateUIEvent& event );

  void OnButtonParentSelectionClick( wxCommandEvent& event );
  void OnButtonParentSelectionUpdate( wxUpdateUIEvent& event );

  void OnButtonDeleteParentClick( wxCommandEvent& event );
  void OnButtonDeleteParentUpdate( wxUpdateUIEvent& event );

protected:
    const static wxString KCheckBoxSuffix;
    const static wxString KTextCtrlSuffix;
    const static wxString KButtonSuffix;
};

#endif
  // _DERIVEDHISTOGRAMDIALOG_H_
