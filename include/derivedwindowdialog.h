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
#define ID_DERIVEDHISTOGRAMDIALOG 10000
#define ID_TEXTCTRL_DERIVED_HISTOGRAM_NAME 10001
#define ID_SCROLLEDWINDOW_HISTOGRAM_LIST -1
#define ID_TEXTCTRL1 10004
#define ID_BITMAPBUTTON 10002
#define ID_BITMAPBUTTON3 10007
#define ID_TEXTCTRL2 10005
#define ID_BITMAPBUTTON2 10006
#define ID_BITMAPBUTTON_ADD_PARENT_HISTOGRAM 10008
#define ID_BITMAPBUTTON_DELETE_PARENT_HISTOGRAM 10009
#define SYMBOL_DERIVEDHISTOGRAMDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_DERIVEDHISTOGRAMDIALOG_TITLE _("Derived Histogram Dialog")
#define SYMBOL_DERIVEDHISTOGRAMDIALOG_IDNAME ID_DERIVEDHISTOGRAMDIALOG
#define SYMBOL_DERIVEDHISTOGRAMDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_DERIVEDHISTOGRAMDIALOG_POSITION wxDefaultPosition
////@end control identifiers


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
  DerivedHistogramDialog( wxWindow* parent, wxWindowID id = SYMBOL_DERIVEDHISTOGRAMDIALOG_IDNAME, const wxString& caption = SYMBOL_DERIVEDHISTOGRAMDIALOG_TITLE, const wxPoint& pos = SYMBOL_DERIVEDHISTOGRAMDIALOG_POSITION, const wxSize& size = SYMBOL_DERIVEDHISTOGRAMDIALOG_SIZE, long style = SYMBOL_DERIVEDHISTOGRAMDIALOG_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_DERIVEDHISTOGRAMDIALOG_IDNAME, const wxString& caption = SYMBOL_DERIVEDHISTOGRAMDIALOG_TITLE, const wxPoint& pos = SYMBOL_DERIVEDHISTOGRAMDIALOG_POSITION, const wxSize& size = SYMBOL_DERIVEDHISTOGRAMDIALOG_SIZE, long style = SYMBOL_DERIVEDHISTOGRAMDIALOG_STYLE );

  /// Destructor
  ~DerivedHistogramDialog();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin DerivedHistogramDialog event handler declarations

  /// wxEVT_COMMAND_TEXT_ENTER event handler for ID_TEXTCTRL_DERIVED_HISTOGRAM_NAME
  void OnTextctrlDerivedHistogramNameEnter( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON_ADD_PARENT_HISTOGRAM
  void OnBitmapbuttonAddParentHistogramClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON_DELETE_PARENT_HISTOGRAM
  void OnBitmapbuttonDeleteParentHistogramClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_BITMAPBUTTON_DELETE_PARENT_HISTOGRAM
  void OnBitmapbuttonDeleteParentHistogramUpdate( wxUpdateUIEvent& event );

////@end DerivedHistogramDialog event handler declarations

////@begin DerivedHistogramDialog member function declarations

  wxString GetDerivedWindowName() const { return derivedWindowName ; }
  void SetDerivedWindowName(wxString value) { derivedWindowName = value ; }

  TWindowType GetParentWindows() const { return parentWindows ; }
  void SetParentWindows(TWindowType value) { parentWindows = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end DerivedHistogramDialog member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

////@begin DerivedHistogramDialog member variables
  wxTextCtrl* derivedHistogramName;
  wxBitmapButton* buttonAddParentHistogram;
  wxBitmapButton* buttonDeleteParentHistogram;
private:
  wxString derivedWindowName;
  TWindowType parentWindows;
////@end DerivedHistogramDialog member variables

  // First version, added directly
  std::vector< Histogram * > parentHistograms;
  

};

#endif
  // _DERIVEDHISTOGRAMDIALOG_H_
