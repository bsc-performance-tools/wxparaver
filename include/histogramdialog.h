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

// wxPropertyDialogSheet
//#include <wx/propdlg.h>
//#include <wx/generic/propdlg.h>

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
#define ID_HISTOGRAM_CONTROLTIMELINETEXT 10065
#define ID_HISTOGRAM_CONTROLTIMELINEBUTTON 10001
#define ID_HISTOGRAM_CONTROLTIMELINEAUTOFIT 10067
#define ID_HISTOGRAM_CONTROLTIMELINEMIN 10068
#define ID_HISTOGRAM_CONTROLTIMELINEMAX 10005
#define ID_HISTOGRAM_CONTROLTIMELINEDELTA 10006
#define ID_HISTOGRAM_DATATIMELINETEXT 10003
#define ID_HISTOGRAM_DATATIMELINEBUTTON 10002
#define ID_HISTOGRAM_3DTIMELINETEXT 10004
#define ID_HISTOGRAM_3DTIMELINEBUTTON 10007
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


// Radio Button
enum class TTimeRangeSource
{
  WINDOW_RANGE = 0,
  TRACE_RANGE,
  SELECTION_RANGE
};

/*!
 * HistogramDialog class declaration
 */

class HistogramDialog: public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( HistogramDialog )
  DECLARE_EVENT_TABLE()

public:
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

  /// wxEVT_IDLE event handler for ID_HISTOGRAMDIALOG
  void OnIdle( wxIdleEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_HISTOGRAM_CONTROLTIMELINEBUTTON
  void OnHistogramControltimelinebuttonClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_HISTOGRAM_CONTROLTIMELINEAUTOFIT
  void OnHistogramControltimelineautofitClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_HISTOGRAM_CONTROLTIMELINEAUTOFIT
  void OnHistogramControltimelineautofitUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_HISTOGRAM_DATATIMELINEBUTTON
  void OnHistogramDatatimelinebuttonClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_HISTOGRAM_3DTIMELINEBUTTON
  void OnHistogram3dtimelinebuttonClick( wxCommandEvent& event );

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

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_HISTOGRAM_BUTTONSELECT
  void OnHistogramButtonselectClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_HISTOGRAM_BUTTONSELECT
  void OnHistogramButtonselectUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
  void OnCancelClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
  void OnOkClick( wxCommandEvent& event );

////@end HistogramDialog event handler declarations

////@begin HistogramDialog member function declarations

  bool GetControlTimelineAutofit() const { return controlTimelineAutofit ; }
  void SetControlTimelineAutofit(bool value) { controlTimelineAutofit = value ; }

  double GetControlTimelineDelta() const { return controlTimelineDelta ; }
  void SetControlTimelineDelta(double value) { controlTimelineDelta = value ; }

  double GetControlTimelineMax() const { return controlTimelineMax ; }
  void SetControlTimelineMax(double value) { controlTimelineMax = value ; }

  double GetControlTimelineMin() const { return controlTimelineMin ; }
  void SetControlTimelineMin(double value) { controlTimelineMin = value ; }

  Window * GetControlTimelineSelected() const { return controlTimelineSelected ; }
  void SetControlTimelineSelected(Window * value) { controlTimelineSelected = value ; }

  std::vector<TWindowID> GetControlTimelines() const { return controlTimelines ; }
  void SetControlTimelines(std::vector<TWindowID> value) { controlTimelines = value ; }

  Window * GetCurrentWindow() const { return currentWindow ; }
  void SetCurrentWindow(Window * value) { currentWindow = value ; }

  Window * GetDataTimelineSelected() const { return dataTimelineSelected ; }
  void SetDataTimelineSelected(Window * value) { dataTimelineSelected = value ; }

  std::vector<TWindowID> GetDataTimelines() const { return dataTimelines ; }
  void SetDataTimelines(std::vector<TWindowID> value) { dataTimelines = value ; }

  bool GetExtraControlTimelineAutofit() const { return extraControlTimelineAutofit ; }
  void SetExtraControlTimelineAutofit(bool value) { extraControlTimelineAutofit = value ; }

  double GetExtraControlTimelineDelta() const { return extraControlTimelineDelta ; }
  void SetExtraControlTimelineDelta(double value) { extraControlTimelineDelta = value ; }

  double GetExtraControlTimelineMax() const { return extraControlTimelineMax ; }
  void SetExtraControlTimelineMax(double value) { extraControlTimelineMax = value ; }

  double GetExtraControlTimelineMin() const { return extraControlTimelineMin ; }
  void SetExtraControlTimelineMin(double value) { extraControlTimelineMin = value ; }

  Window * GetExtraControlTimelineSelected() const { return extraControlTimelineSelected ; }
  void SetExtraControlTimelineSelected(Window * value) { extraControlTimelineSelected = value ; }

  std::vector<TWindowID> GetExtraControlTimelines() const { return extraControlTimelines ; }
  void SetExtraControlTimelines(std::vector<TWindowID> value) { extraControlTimelines = value ; }

  std::vector<std::pair<TRecordTime,TRecordTime> > GetTimeRange() const { return timeRange ; }
  void SetTimeRange(std::vector<std::pair<TRecordTime,TRecordTime> > value) { timeRange = value ; }

  bool GetWaitingGlobalTiming() const { return waitingGlobalTiming ; }
  void SetWaitingGlobalTiming(bool value) { waitingGlobalTiming = value ; }

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
  wxTextCtrl* txtControlTimelines;
  wxButton* buttonControlTimelines;
  wxToggleButton* buttonControlTimelineAutoFit;
  wxStaticText* labelControlTimelineMin;
  wxTextCtrl* txtControlTimelineMin;
  wxStaticText* labelControlTimelineMax;
  wxTextCtrl* txtControlTimelineMax;
  wxStaticText* labelControlTimelineDelta;
  wxTextCtrl* txtControlTimelineDelta;
  wxTextCtrl* txtDataTimelines;
  wxButton* buttonDataTimelines;
  wxTextCtrl* txt3DTimelines;
  wxButton* button3DTimelines;
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
  wxButton* buttonSelect;
private:
  bool controlTimelineAutofit;
  double controlTimelineDelta;
  double controlTimelineMax;
  double controlTimelineMin;
  Window * controlTimelineSelected;
  std::vector<TWindowID> controlTimelines;
  Window * currentWindow;
  Window * dataTimelineSelected;
  std::vector<TWindowID> dataTimelines;
  bool extraControlTimelineAutofit;
  double extraControlTimelineDelta;
  double extraControlTimelineMax;
  double extraControlTimelineMin;
  Window * extraControlTimelineSelected;
  std::vector<TWindowID> extraControlTimelines;
  std::vector<std::pair<TRecordTime,TRecordTime> > timeRange;
  bool waitingGlobalTiming;
////@end HistogramDialog member variables

  wxString formatNumber( double value );

  TSemanticValue computeDelta( TSemanticValue min, TSemanticValue max );
  void computeColumns( Window *timeline, TSemanticValue &min, TSemanticValue &max, TSemanticValue &delta );
  void updateControlTimelineAutofit();
  void updateExtraControlTimelineAutofit();
  PRV_UINT32 fillList( Window *current, std::vector< TWindowID > listTimelines, wxChoice *listWidget );
  void enable3DFields( bool autofit );
};

#endif // _HISTOGRAMDIALOG_H_
