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

#ifndef _ROWSSELECTIONDIALOG_H_
#define _ROWSSELECTIONDIALOG_H_

/*!
 * Includes
 */
#include <wx/panel.h>
#include <wx/propdlg.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/checklst.h>

#include <wx/regex.h>
#include <wx/checkbox.h>
#include <wx/valtext.h> // DELETE ME?
#include <wx/stattext.h>

#include <map>

#include "paraverkerneltypes.h"
#include "selectionmanagement.h"

/*!
 * Forward declarations
 */
class Window;

/*!
 * Control identifiers
 */

#define ID_ROWSSELECTIONDIALOG 10078
#define SYMBOL_ROWSSELECTIONDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_ROWSSELECTIONDIALOG_TITLE _("Objects Selection")
#define SYMBOL_ROWSSELECTIONDIALOG_IDNAME ID_ROWSSELECTIONDIALOG
#define SYMBOL_ROWSSELECTIONDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_ROWSSELECTIONDIALOG_POSITION wxDefaultPosition

/*!
 * RowsSelectionDialog class declaration
 */
class RowsSelectionDialog: public wxPropertySheetDialog
{    
  DECLARE_DYNAMIC_CLASS( RowsSelectionDialog )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  RowsSelectionDialog();
  RowsSelectionDialog( wxWindow* parent,
                       Window *whichWindow,
                       SelectionManagement< TObjectOrder, TWindowLevel > *whichSelectedRows,
                       wxWindowID id = SYMBOL_ROWSSELECTIONDIALOG_IDNAME,
                       const wxString& caption = SYMBOL_ROWSSELECTIONDIALOG_TITLE,
                       const wxPoint& pos = SYMBOL_ROWSSELECTIONDIALOG_POSITION,
                       const wxSize& size = SYMBOL_ROWSSELECTIONDIALOG_SIZE,
                       long style = SYMBOL_ROWSSELECTIONDIALOG_STYLE );

  /// Creation
  bool Create( wxWindow* parent,
               wxWindowID id = SYMBOL_ROWSSELECTIONDIALOG_IDNAME,
               const wxString& caption = SYMBOL_ROWSSELECTIONDIALOG_TITLE,
               const wxPoint& pos = SYMBOL_ROWSSELECTIONDIALOG_POSITION,
               const wxSize& size = SYMBOL_ROWSSELECTIONDIALOG_SIZE,
               long style = SYMBOL_ROWSSELECTIONDIALOG_STYLE );

  /// Destructor
  ~RowsSelectionDialog();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );

  /// Should we show tooltips?
  static bool ShowToolTips();

  int GetSelections( TWindowLevel whichLevel, wxArrayInt &selections );

  virtual bool TransferDataFromWindow();
  
  // If it's visible, timeline zoom should change
  bool ShouldChangeTimelineZoom() const { return shouldChangeTimelineZoom; }
   // Next two gets only have sense if ShouldChangeTimelineZoom rets true or
   // you'll get zeros
  TObjectOrder GetNewBeginZoom() const { return beginZoom; }
  TObjectOrder GetNewEndZoom() const { return endZoom; }

private:
  Window *myTimeline;
  SelectionManagement< TObjectOrder, TWindowLevel > *mySelectedRows;
  SelectionManagement< TObjectOrder, TWindowLevel > originalSelectedRows;
  TWindowLevel minLevel; 
  std::vector< wxButton * > selectionButtons;
  std::vector< wxCheckListBox* > levelCheckList;
  
  bool shouldChangeTimelineZoom;
  TObjectOrder beginZoom;
  TObjectOrder endZoom;
  
  std::map< TWindowLevel , std::vector< TObjectOrder > >selectedIndex;

  // RE
  bool lockedByUpdate;
  std::vector< wxStaticText *> messageMatchesFound;
  std::vector< wxCheckBox *> checkBoxPosixBasicRegExp;
  std::vector< wxTextCtrl *> textCtrlRegularExpr;
  std::vector< wxButton * > applyButtons;
  std::vector< wxRegEx * > validRE;
  wxString getMyToolTip( const bool posixBasicRegExpTip );
  void OnCheckBoxMatchPosixRegExpClicked( wxCommandEvent& event );
  wxTextValidator *getValidator( bool basicPosixRegExprMode ); // DELETE ME?
  // void CheckRegularExpression( wxCommandEvent& event );
  wxString buildRegularExpressionString( const wxString& enteredRE );
  int countMatches( int iTab, wxRegEx *&levelRE );
  void checkMatches( const int &iTab, wxRegEx *&levelRE );
  void OnRegularExpressionApply( wxCommandEvent& event );
  void OnCheckListBoxSelected( wxCommandEvent& event );

  void OnSelectAllButtonClicked( wxCommandEvent& event );
  void OnUnselectAllButtonClicked( wxCommandEvent& event );
  void OnInvertButtonClicked( wxCommandEvent& event );
  void buildPanel( const wxString& title, TWindowLevel level );
  void OnOkClick( wxCommandEvent& event );
};

#endif // _ROWSSELECTIONDIALOG_H_
