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
class wxBoxSizer;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_DERIVEDWINDOWDIALOG 10000
#define ID_TEXTCTRL_DERIVED_HISTOGRAM_NAME 10001
#define ID_SCROLLEDWINDOW_HISTOGRAM_LIST -1
#define ID_PANEL_1 10012
#define ID_BITMAPBUTTON1 10011
#define ID_TEXTCTRL1 10004
#define ID_BITMAPBUTTON 10002
#define ID_CHECKBOX1 10010
#define ID_PANEL_2 10013
#define ID_BITMAPBUTTON3 10007
#define ID_TEXTCTRL2 10005
#define ID_BITMAP_HISTO_BUTTON2 10006
#define ID_CHECKBOX 10003
#define ID_BITMAPBUTTON_ADD_PARENT_WINDOW 10008
#define ID_BITMAPBUTTON_DELETE_PARENT_WINDOW 10009
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

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON
  void OnBitmapbuttonClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_BITMAPBUTTON
  void OnBitmapbuttonUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON_ADD_PARENT_WINDOW
  void OnBitmapbuttonAddParentWindowClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON_DELETE_PARENT_WINDOW
  void OnBitmapbuttonDeleteParentWindowClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_BITMAPBUTTON_DELETE_PARENT_WINDOW
  void OnBitmapbuttonDeleteParentWindowUpdate( wxUpdateUIEvent& event );

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
  wxBoxSizer* verticalBoxSizerHistogramLines;
  wxPanel* panelHistogramLine1;
  wxPanel* panelHistogramLine2;
  wxBitmapButton* buttonOperation;
  wxBitmapButton* buttonSelectWindow;
  wxBitmapButton* buttonAddParentWindow;
  wxBitmapButton* buttonDeleteParentWindow;
private:
  wxString derivedWindowName;
  std::vector< TWindowType > parentWindows;
////@end DerivedHistogramDialog member variables

  wxImageList *imagesList;

  void CreateControlsForParentsList();
  void CreateControlsForParent( const TWindowType& whichParents,
                                bool showOperation,
                                const wxBitmap& derivedOperationIcon );
};

#endif
  // _DERIVEDHISTOGRAMDIALOG_H_
