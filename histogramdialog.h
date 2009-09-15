/////////////////////////////////////////////////////////////////////////////
// Name:        histogramdialog.h
// Purpose:     
// Author:      Eloy Martinez
// Modified by: 
// Created:     Wed 02 Sep 2009 12:48:45 CEST
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _HISTOGRAMDIALOG_H_
#define _HISTOGRAMDIALOG_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/tglbtn.h"
#include "wx/statline.h"
////@end includes

#include <vector>

#include "paraverkerneltypes.h"
#include "window.h"
#include "loadedwindows.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxToggleButton;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_HISTOGRAMDIALOG 10061
#define ID_HISTOGRAM_CONTROLTIMELINELIST 10065
#define ID_HISTOGRAM_CONTROLTIMELINEAUTOFIT 10067
#define ID_HISTOGRAM_CONTROLTIMELINEMIN 10068
#define ID_HISTOGRAM_CONTROLTIMELINEMAX 10005
#define ID_HISTOGRAM_CONTROLTIMELINEDELTA 10006
#define ID_HISTOGRAM_DATATIMELINELIST 10003
#define ID_HISTOGRAM_3DTIMELINELIST 10004
#define ID_HISTOGRAM_3DTIMELINEAUTOFIT 10066
#define ID_HISTOGRAM_3DTIMELINEMIN 10008
#define ID_HISTOGRAM_3DTIMELINEMAX 10009
#define ID_HISTOGRAM_3DTIMELINEDELTA 10010
#define ID_HISTOGRAM_BEGINTIME 10011
#define ID_HISTOGRAM_ENDTIME 10012
#define ID_RADIOBUTTON_ALLWINDOW 10063
#define ID_RADIOBUTTON_ALLTRACE 10000
#define ID_RADIOBUTTON_MANUAL 10064
#define ID_HISTOGRAM_BUTTONSELECT 10062
#define SYMBOL_HISTOGRAMDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_HISTOGRAMDIALOG_TITLE _("Create Histogram")
#define SYMBOL_HISTOGRAMDIALOG_IDNAME ID_HISTOGRAMDIALOG
#define SYMBOL_HISTOGRAMDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_HISTOGRAMDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * HistogramDialog class declaration
 */

class HistogramDialog: public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( HistogramDialog )
  DECLARE_EVENT_TABLE()

public:
  // Radio Button
  typedef enum
  {
    WINDOW_RANGE = 0,
    TRACE_RANGE,
    SELECTION_RANGE
  } TTimeRangeSource;

  /// Constructors
  HistogramDialog();
  HistogramDialog( wxWindow* parent, wxWindowID id = SYMBOL_HISTOGRAMDIALOG_IDNAME, const wxString& caption = SYMBOL_HISTOGRAMDIALOG_TITLE, const wxPoint& pos = SYMBOL_HISTOGRAMDIALOG_POSITION, const wxSize& size = SYMBOL_HISTOGRAMDIALOG_SIZE, long style = SYMBOL_HISTOGRAMDIALOG_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_HISTOGRAMDIALOG_IDNAME, const wxString& caption = SYMBOL_HISTOGRAMDIALOG_TITLE, const wxPoint& pos = SYMBOL_HISTOGRAMDIALOG_POSITION, const wxSize& size = SYMBOL_HISTOGRAMDIALOG_SIZE, long style = SYMBOL_HISTOGRAMDIALOG_STYLE );

  /// Destructor
  ~HistogramDialog();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin HistogramDialog event handler declarations

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_HISTOGRAM_CONTROLTIMELINELIST
  void OnHistogramControltimelinelistSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_HISTOGRAM_CONTROLTIMELINEAUTOFIT
  void OnHistogramControltimelineautofitClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_HISTOGRAM_CONTROLTIMELINEAUTOFIT
  void OnHistogramControltimelineautofitUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_HISTOGRAM_DATATIMELINELIST
  void OnHistogramDatatimelinelistSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_HISTOGRAM_3DTIMELINELIST
  void OnHistogram3dtimelinelistSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_HISTOGRAM_3DTIMELINEAUTOFIT
  void OnHistogram3dtimelineautofitClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_HISTOGRAM_3DTIMELINEAUTOFIT
  void OnHistogram3dtimelineautofitUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_ALLWINDOW
  void OnRadiobuttonAllwindowSelected( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_RADIOBUTTON_ALLWINDOW
  void OnRadiobuttonAllwindowUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_ALLTRACE
  void OnRadiobuttonAlltraceSelected( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_RADIOBUTTON_ALLTRACE
  void OnRadiobuttonAlltraceUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_MANUAL
  void OnRadiobuttonManualSelected( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_RADIOBUTTON_MANUAL
  void OnRadiobuttonManualUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_HISTOGRAM_BUTTONSELECT
  void OnHistogramButtonselectClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_HISTOGRAM_BUTTONSELECT
  void OnHistogramButtonselectUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
  void OnOkClick( wxCommandEvent& event );

////@end HistogramDialog event handler declarations

////@begin HistogramDialog member function declarations

  vector< TWindowID > GetControlTimelines() const { return controlTimelines ; }
  void SetControlTimelines(vector< TWindowID > value) { controlTimelines = value ; }

  double GetControlTimelineMin() const { return controlTimelineMin ; }
  void SetControlTimelineMin(double value) { controlTimelineMin = value ; }

  double GetControlTimelineMax() const { return controlTimelineMax ; }
  void SetControlTimelineMax(double value) { controlTimelineMax = value ; }

  double GetControlTimelineDelta() const { return controlTimelineDelta ; }
  void SetControlTimelineDelta(double value) { controlTimelineDelta = value ; }

  vector< TWindowID > GetDataTimelines() const { return dataTimelines ; }
  void SetDataTimelines(vector< TWindowID > value) { dataTimelines = value ; }

  double GetExtraControlTimelineMin() const { return extraControlTimelineMin ; }
  void SetExtraControlTimelineMin(double value) { extraControlTimelineMin = value ; }

  double GetExtraControlTimelineMax() const { return extraControlTimelineMax ; }
  void SetExtraControlTimelineMax(double value) { extraControlTimelineMax = value ; }

  double GetExtraControlTimelineDelta() const { return extraControlTimelineDelta ; }
  void SetExtraControlTimelineDelta(double value) { extraControlTimelineDelta = value ; }

  vector< pair< TRecordTime, TRecordTime > > GetTimeRange() const { return timeRange ; }
  void SetTimeRange(vector< pair< TRecordTime, TRecordTime > > value) { timeRange = value ; }

  vector< TWindowID > GetExtraControlTimelines() const { return extraControlTimelines ; }
  void SetExtraControlTimelines(vector< TWindowID > value) { extraControlTimelines = value ; }

  bool GetControlTimelineAutofit() const { return controlTimelineAutofit ; }
  void SetControlTimelineAutofit(bool value) { controlTimelineAutofit = value ; }

  bool GetExtraControlTimelineAutofit() const { return extraControlTimelineAutofit ; }
  void SetExtraControlTimelineAutofit(bool value) { extraControlTimelineAutofit = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end HistogramDialog member function declarations

  bool TransferDataToWindow( Window *current );
  bool TransferDataFromWindow();

  /// Should we show tooltips?
  static bool ShowToolTips();

////@begin HistogramDialog member variables
  wxChoice* listControlTimelines;
  wxToggleButton* buttonControlTimelineAutoFit;
  wxStaticText* labelControlTimelineMin;
  wxTextCtrl* txtControlTimelineMin;
  wxStaticText* labelControlTimelineMax;
  wxTextCtrl* txtControlTimelineMax;
  wxStaticText* labelControlTimelineDelta;
  wxTextCtrl* txtControlTimelineDelta;
  wxChoice* listDataTimelines;
  wxChoice* list3DTimelines;
  wxToggleButton* button3DTimelineAutoFit;
  wxStaticText* label3DTimelineMin;
  wxTextCtrl* txt3DTimelineMin;
  wxStaticText* label3DTimelineMax;
  wxTextCtrl* txt3DTimelineMax;
  wxStaticText* label3DTimelineDelta;
  wxTextCtrl* txt3DTimelineDelta;
  wxTextCtrl* txtBeginTime;
  wxTextCtrl* txtEndTime;
  wxRadioButton* radioAllWindow;
  wxRadioButton* radioAllTrace;
  wxRadioButton* radioManual;
  wxToggleButton* buttonSelect;
private:
  vector< TWindowID > controlTimelines;
  double controlTimelineMin;
  double controlTimelineMax;
  double controlTimelineDelta;
  vector< TWindowID > dataTimelines;
  double extraControlTimelineMin;
  double extraControlTimelineMax;
  double extraControlTimelineDelta;
  vector< pair< TRecordTime, TRecordTime > > timeRange;
  vector< TWindowID > extraControlTimelines;
  bool controlTimelineAutofit;
  bool extraControlTimelineAutofit;
////@end HistogramDialog member variables

  wxString formatNumber( double value );

  // getSelectedWindow deletes previous information in vector selection
  void getSelectedWindowID( wxChoice * listWidget,
                            vector< TWindowID > &selection,
                            bool listWithFirstVoidOption );
  TSemanticValue computeDelta( TSemanticValue min, TSemanticValue max );
  void computeColumns( Window *timeline, TSemanticValue &min, TSemanticValue &max, TSemanticValue &delta );
  void updateControlTimelineAutofit();
  void updateExtraControlTimelineAutofit();
  UINT32 fillList( Window *current, vector< TWindowID > listTimelines, wxChoice *listWidget );
  void enable3DFields( bool autofit );
};

#endif
  // _HISTOGRAMDIALOG_H_
