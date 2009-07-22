/////////////////////////////////////////////////////////////////////////////
// Name:        derivedtimelinedialog.h
// Purpose:     
// Author:      Eloy Martinez
// Modified by: 
// Created:     Tue 07 Jul 2009 17:07:35 CEST
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _DERIVEDTIMELINEDIALOG_H_
#define _DERIVEDTIMELINEDIALOG_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/statline.h"
////@end includes

#include "window.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_DERIVEDTIMELINEDIALOG 10032
#define ID_DERIVED_NAME 10001
#define ID_TOPCOMPOSE1 10002
#define ID_MINCOMPOSE1 10034
#define ID_MAXCOMPOSE2 10040
#define ID_TOPCOMPOSE2 10000
#define ID_MINCOMPOSE2 10041
#define ID_FACTOR_TIMELINE_1 10035
#define ID_TIMELINES_LIST_1 10036
#define ID_OPERATIONS 10037
#define ID_TIMELINES_LIST_2 10038
#define ID_FACTOR_TIMELINE_2 10039
#define ID_SWAP_WINDOWS 10033
#define SYMBOL_DERIVEDTIMELINEDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_DERIVEDTIMELINEDIALOG_TITLE _("Create Derived Timeline Window")
#define SYMBOL_DERIVEDTIMELINEDIALOG_IDNAME ID_DERIVEDTIMELINEDIALOG
#define SYMBOL_DERIVEDTIMELINEDIALOG_SIZE wxDefaultSize
#define SYMBOL_DERIVEDTIMELINEDIALOG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * DerivedTimelineDialog class declaration
 */

class DerivedTimelineDialog: public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( DerivedTimelineDialog )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  DerivedTimelineDialog();
  DerivedTimelineDialog( wxWindow* parent, wxWindowID id = SYMBOL_DERIVEDTIMELINEDIALOG_IDNAME, const wxString& caption = SYMBOL_DERIVEDTIMELINEDIALOG_TITLE, const wxPoint& pos = SYMBOL_DERIVEDTIMELINEDIALOG_POSITION, const wxSize& size = SYMBOL_DERIVEDTIMELINEDIALOG_SIZE, long style = SYMBOL_DERIVEDTIMELINEDIALOG_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_DERIVEDTIMELINEDIALOG_IDNAME, const wxString& caption = SYMBOL_DERIVEDTIMELINEDIALOG_TITLE, const wxPoint& pos = SYMBOL_DERIVEDTIMELINEDIALOG_POSITION, const wxSize& size = SYMBOL_DERIVEDTIMELINEDIALOG_SIZE, long style = SYMBOL_DERIVEDTIMELINEDIALOG_STYLE );

  /// Destructor
  ~DerivedTimelineDialog();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin DerivedTimelineDialog event handler declarations

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_TOPCOMPOSE1
  void OnTopcompose1Selected( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_TOPCOMPOSE2
  void OnTopcompose2Selected( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SWAP_WINDOWS
  void OnSwapWindowsClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
  void OnOkClick( wxCommandEvent& event );

////@end DerivedTimelineDialog event handler declarations

////@begin DerivedTimelineDialog member function declarations

  double GetFactorTimeline1() const { return factorTimeline1 ; }
  void SetFactorTimeline1(double value) { factorTimeline1 = value ; }

  double GetFactorTimeline2() const { return factorTimeline2 ; }
  void SetFactorTimeline2(double value) { factorTimeline2 = value ; }

  vector< Window * > GetTimelines1() const { return timelines1 ; }
  void SetTimelines1(vector< Window * > value) { timelines1 = value ; }

  vector< Window * > GetTimelines2() const { return timelines2 ; }
  void SetTimelines2(vector< Window * > value) { timelines2 = value ; }

  vector< string > GetOperations() const { return operations ; }
  void SetOperations(vector< string > value) { operations = value ; }

  Window * GetCurrentWindow1() const { return currentWindow1 ; }
  void SetCurrentWindow1(Window * value) { currentWindow1 = value ; }

  Window * GetCurrentWindow2() const { return currentWindow2 ; }
  void SetCurrentWindow2(Window * value) { currentWindow2 = value ; }

  vector< string > GetTopCompose1() const { return topCompose1 ; }
  void SetTopCompose1(vector< string > value) { topCompose1 = value ; }

  vector< string > GetTopCompose2() const { return topCompose2 ; }
  void SetTopCompose2(vector< string > value) { topCompose2 = value ; }

  string GetTimelineName() const { return timelineName ; }
  void SetTimelineName(string value) { timelineName = value ; }

  double GetMinCompose1() const { return minCompose1 ; }
  void SetMinCompose1(double value) { minCompose1 = value ; }

  double GetMaxCompose1() const { return maxCompose1 ; }
  void SetMaxCompose1(double value) { maxCompose1 = value ; }

  double GetMinCompose2() const { return minCompose2 ; }
  void SetMinCompose2(double value) { minCompose2 = value ; }

  double GetMaxCompose2() const { return maxCompose2 ; }
  void SetMaxCompose2(double value) { maxCompose2 = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end DerivedTimelineDialog member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  bool TransferDataToWindow();
  bool TransferDataFromWindow();

////@begin DerivedTimelineDialog member variables
  wxTextCtrl* widgetName;
  wxChoice* widgetTopCompose1;
  wxStaticText* widgetLabelMinCompose1;
  wxTextCtrl* widgetMinCompose1;
  wxStaticText* widgetLabelMaxCompose1;
  wxTextCtrl* widgetMaxCompose1;
  wxChoice* widgetTopCompose2;
  wxStaticText* widgetLabelMinCompose2;
  wxTextCtrl* widgetMinCompose2;
  wxStaticText* widgetLabelMaxCompose2;
  wxTextCtrl* widgetMaxCompose2;
  wxTextCtrl* widgetFactorTimeline1;
  wxComboBox* widgetTimelines1;
  wxChoice* widgetOperations;
  wxComboBox* widgetTimelines2;
  wxTextCtrl* widgetFactorTimeline2;
  wxButton* swapWindowsButton;
private:
  double factorTimeline1;
  double factorTimeline2;
  vector< Window * > timelines1;
  vector< Window * > timelines2;
  vector< string > operations;
  Window * currentWindow1;
  Window * currentWindow2;
  vector< string > topCompose1;
  vector< string > topCompose2;
  string timelineName;
  double minCompose1;
  double maxCompose1;
  double minCompose2;
  double maxCompose2;
////@end DerivedTimelineDialog member variables

  void presetTimelineComboBox( vector< Window * > timelines,
                               Window *currentWindow,
                               wxComboBox *comboBox );
  void presetStringChoiceBox( vector< string > list, wxChoice *choiceBox );
  void presetFactorField( double value, wxTextCtrl *field );
  void presetNameField( string whichName, wxTextCtrl *field );

  void getSelectedString( wxChoice *choiceBox, vector< string > &selection ) const;
  void getSelectedWindow( wxComboBox *comboBox, vector< Window * > &selection ) const;
  void getName( wxTextCtrl *field, string &whichName ) const;
  bool getFactorFields( double &whichFactor1,
                        double &whichFactor2 );
  void setEnabled(  wxStaticText *label, wxTextCtrl *field, bool enabled );
  void allowRangeWidgets();
  void setParametersCompose( UINT32 compose,
                             string nameFunction,
                             UINT32 numParameters,
                             vector< string > namesParameters );


};

#endif
  // _DERIVEDTIMELINEDIALOG_H_
