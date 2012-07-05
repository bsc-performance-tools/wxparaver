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
#define SYMBOL_ROWSSELECTIONDIALOG_TITLE _("Rows Selection")
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

private:
  Window *myTimeline;
  SelectionManagement< TObjectOrder, TWindowLevel > *mySelectedRows;
  TWindowLevel minLevel; 
  std::vector< wxButton * > selectionButtons;
  std::vector< wxCheckListBox* > levelCheckList;

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
