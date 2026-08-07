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

#include <wx/listctrl.h>
#include <wx/popupwin.h>

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

#define ID_BITMAPBUTTON_OPERATION 22000


class ButtonArrayPopup: public wxPopupTransientWindow
{
public:
    ButtonArrayPopup( wxWindow *parent,
                      wxBitmapButton *whichCallerButton,
                      wxImageList *icons,
                      const wxString& operationName );
    virtual ~ButtonArrayPopup();

    // wxPopupTransientWindow virtual methods are all overridden to log them
    virtual void Popup( wxWindow *focus = NULL ) wxOVERRIDE;
    virtual void OnDismiss() wxOVERRIDE;
    virtual bool ProcessLeftDown( wxMouseEvent& event ) wxOVERRIDE;
    virtual bool Show( bool show = true ) wxOVERRIDE;

    wxString GetOperationName() { return operationName; }

private:
    wxString operationName;
    wxBitmapButton *callerButton;
  
    wxScrolledWindow *m_panel;

    // TODO: something smarter
    wxButton *m_button1;
    wxButton *m_button2;
    wxButton *m_button3;
    wxButton *m_button4;
    wxButton *m_button5;
    wxButton *m_button6;
    wxButton *m_button7;

private:
    void OnMouse( wxMouseEvent &event );
    void OnSize( wxSizeEvent &event );
    void OnSetFocus( wxFocusEvent &event );
    void OnKillFocus( wxFocusEvent &event );
    void OnButton( wxCommandEvent& event );
    // void OnKeyDown( wxKeyEvent& event );

private:
    wxDECLARE_ABSTRACT_CLASS(ButtonArrayPopup);
    wxDECLARE_EVENT_TABLE();
};



using TOperationName = std::string;
using TButtonName = wxString;
using TParentIndex = int;


struct TIntWrapper : public wxObject
{
  TParentIndex pos;
};


struct TLineData
{
  TWindowType parent = std::monostate{};
  TOperationName operationName = "none";
  std::vector< TButtonName > button {};

  TLineData( TWindowType whichParent,
             TOperationName whichOperationName,
             std::vector< TButtonName > whichButton )
    : parent( whichParent ),
      operationName( whichOperationName ),
      button( whichButton )
  {} 
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
  DerivedHistogramDialog( wxWindow* parent,
                          const std::vector< TWindowType >& whichParents,
                          const std::vector< std::string >& whichOperations,
                          wxWindowID id = SYMBOL_DERIVEDHISTOGRAMDIALOG_IDNAME,
                          const wxString& caption = SYMBOL_DERIVEDHISTOGRAMDIALOG_TITLE,
                          const wxPoint& pos = SYMBOL_DERIVEDHISTOGRAMDIALOG_POSITION,
                          const wxSize& size = SYMBOL_DERIVEDHISTOGRAMDIALOG_SIZE,
                          long style = SYMBOL_DERIVEDHISTOGRAMDIALOG_STYLE );

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

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON_ADD_PARENT_WINDOW
  void OnButtonAddParentClick( wxCommandEvent& event );

////@end DerivedHistogramDialog event handler declarations

////@begin DerivedHistogramDialog member function declarations

  wxString GetDerivedWindowName() const { return derivedWindowName ; }
  void SetDerivedWindowName(wxString value) { derivedWindowName = value ; }

  std::vector< std::string > GetOperations() const { return operations ; }
  void SetOperations(std::vector< std::string > value) { operations = value ; }

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

  void SetPopupOperation( const std::string& whichOperation )
  {
    selectedPopupOperation = whichOperation;
  }

private:
  wxString derivedWindowName;
  std::vector< std::string > operations;
  std::vector< TWindowType > parentWindows;
////@end DerivedHistogramDialog member variables

  wxImageList *imagesList;
  wxImageList *operationsImagesList;
  static ButtonArrayPopup *currentButtonArrayPopup;
  wxBitmapButton *operationButtonCaller;
  std::string selectedPopupOperation {};
    
  std::vector< TLineData > parentLines;
  void deleteParentLine( int whichPos);
  bool changeParentLine( const wxString& buttonName, const std::string& newOperation );

  std::string GetWindowName( const TWindowType& whichParent );
  wxString BuildName( const TWindowType& whichParent );

  void CreateControlsForParentsList();
  void CreateControlsForParentLine( wxBoxSizer *whichBoxSizer,
                                    const TWindowType& whichParents,
                                    int pos,
                                    const wxBitmap& derivedOperationIcon );
  void TransferDataFromPanel();
  void TransferWindowsToData();
  void DisconnectWidgets();
  void CleanParentsList();
  void RebuildParentsList();

  // Button callbacks per line
  void OnButtonOperationSelectionClick( wxCommandEvent& event );
  void OnButtonOperationSelectionUpdate( wxUpdateUIEvent& event );
  void OnPopupOperationModified( wxCommandEvent& event );

  void OnButtonParentSelectionClick( wxCommandEvent& event );
  void OnButtonParentSelectionUpdate( wxUpdateUIEvent& event );

  void OnButtonDeleteParentClick( wxCommandEvent& event );
  void OnButtonDeleteParentUpdate( wxUpdateUIEvent& event );

protected:
    const static wxString KOperation;
    const static wxString KSelect; 
    const static wxString KDelete;
    const static wxString KCheckBox;
    const static wxString KTextCtrl;
    const static wxString KButton;
    const static wxString KNotCreated;
};

#endif
  // _DERIVEDHISTOGRAMDIALOG_H_
