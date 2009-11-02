/////////////////////////////////////////////////////////////////////////////
// Name:        saveconfigurationdialog.h
// Purpose:     
// Author:      Eloy Martinez
// Modified by: 
// Created:     Mon 23 Feb 2009 15:24:31 CET
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SAVECONFIGURATIONDIALOG_H_
#define _SAVECONFIGURATIONDIALOG_H_

/*!
 * Includes
 */

////@begin includes
#include "wx/statline.h"
////@end includes
#include "cfg.h"
#include "window.h"
#include "histogram.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_SAVECONFIGURATIONDIALOG 10012
#define ID_LISTTIMELINES 10013
#define ID_LISTHISTOGRAMS 10014
#define ID_CHECKBEGIN 10016
#define ID_CHECKEND 10017
#define ID_CHECKSEMANTIC 10018
#define ID_RADIOALLTRACE 10019
#define ID_RADIOALLWINDOW 10020
#define ID_CHECKLIMITS 10021
#define ID_CHECKGRADIENT 10022
#define ID_TEXTDESCRIPTION 10015
#define SYMBOL_SAVECONFIGURATIONDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxSTAY_ON_TOP|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_SAVECONFIGURATIONDIALOG_TITLE _("Save configuration")
#define SYMBOL_SAVECONFIGURATIONDIALOG_IDNAME ID_SAVECONFIGURATIONDIALOG
#define SYMBOL_SAVECONFIGURATIONDIALOG_SIZE wxDefaultSize
#define SYMBOL_SAVECONFIGURATIONDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * SaveConfigurationDialog class declaration
 */

class SaveConfigurationDialog: public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( SaveConfigurationDialog )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  SaveConfigurationDialog();
  SaveConfigurationDialog( wxWindow* parent, wxWindowID id = SYMBOL_SAVECONFIGURATIONDIALOG_IDNAME, const wxString& caption = SYMBOL_SAVECONFIGURATIONDIALOG_TITLE, const wxPoint& pos = SYMBOL_SAVECONFIGURATIONDIALOG_POSITION, const wxSize& size = SYMBOL_SAVECONFIGURATIONDIALOG_SIZE, long style = SYMBOL_SAVECONFIGURATIONDIALOG_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SAVECONFIGURATIONDIALOG_IDNAME, const wxString& caption = SYMBOL_SAVECONFIGURATIONDIALOG_TITLE, const wxPoint& pos = SYMBOL_SAVECONFIGURATIONDIALOG_POSITION, const wxSize& size = SYMBOL_SAVECONFIGURATIONDIALOG_SIZE, long style = SYMBOL_SAVECONFIGURATIONDIALOG_STYLE );

  /// Destructor
  ~SaveConfigurationDialog();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin SaveConfigurationDialog event handler declarations

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_SAVE
  void OnSaveClick( wxCommandEvent& event );

////@end SaveConfigurationDialog event handler declarations

////@begin SaveConfigurationDialog member function declarations

  SaveOptions GetOptions() const { return options ; }
  void SetOptions(SaveOptions value) { options = value ; }

  vector<Window *> GetTimelines() const { return timelines ; }
  void SetTimelines(vector<Window *> value) { timelines = value ; }

  vector<Histogram *> GetHistograms() const { return histograms ; }
  void SetHistograms(vector<Histogram *> value) { histograms = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end SaveConfigurationDialog member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  bool TransferDataToWindow();
  bool TransferDataFromWindow();
  
////@begin SaveConfigurationDialog member variables
  wxListBox* listTimelines;
  wxListBox* listHistograms;
  wxCheckBox* optRelativeBegin;
  wxCheckBox* optRelativeEnd;
  wxCheckBox* optComputeSemantic;
  wxRadioButton* radioAllTrace;
  wxRadioButton* radioAllWindow;
  wxCheckBox* optComputeLimits;
  wxCheckBox* optComputeGradient;
  wxTextCtrl* textDescription;
private:
  SaveOptions options;
  vector<Window *> timelines;
  vector<Histogram *> histograms;
////@end SaveConfigurationDialog member variables
};

#endif
  // _SAVECONFIGURATIONDIALOG_H_
