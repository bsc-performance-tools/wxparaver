/////////////////////////////////////////////////////////////////////////////
// Name:        preferencesdialog.h
// Purpose:     
// Author:      Eloy Martinez
// Modified by: 
// Created:     Mon 28 Sep 2009 14:37:45 CEST
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _PREFERENCESDIALOG_H_
#define _PREFERENCESDIALOG_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/propdlg.h"
#include "wx/filepicker.h"
#include "wx/clrpicker.h"
////@end includes

#include "paraverconfig.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxDirPickerCtrl;
class wxColourPickerCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_PREFERENCESDIALOG 10069
#define ID_PREFERENCES_GLOBAL 10073
#define ID_PREFERENCES_GLOBAL_FILLGAPS 10085
#define ID_DIRPICKERCTRL_TRACES 10089
#define ID_DIRPICKERCTRL_CFG 10010
#define ID_DIRPICKERCTRL_TMP 10011
#define ID_PREFERENCES_TIMELINE 10072
#define ID_PREFERENCES_WW_PRECISION 10000
#define ID_PREFERENCES_HISTOGRAM 10071
#define ID_PREFERENCES_2D_NUMCOLUMNS 10075
#define ID_PREFERENCES_2D_PRECISION 10074
#define ID_PREFERENCES_2D_CHECKOPTIONS 10078
#define ID_PREFERENCES_COLOR 10086
#define ID_COLOURPICKER_BACKGROUND 10002
#define ID_COLOURPICKER_AXIS 10001
#define ID_COLOURPICKER_LOGICAL 10007
#define ID_COLOURPICKER_PHYSICAL 10008
#define ID_BUTTON_DEFAULT_TIMELINE 10087
#define ID_COLOURPICKER_GRADBEGIN 10003
#define ID_COLOURPICKER_GRADEND 10004
#define ID_COLOURPICKER_GRADLOW 10005
#define ID_COLOURPICKER_GRADTOP 10006
#define ID_BUTTON_DEFAULT_GRADIENT 10009
#define ID_PREFERENCES_FILTERS 10070
#define SYMBOL_PREFERENCESDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PREFERENCESDIALOG_TITLE _("Preferences")
#define SYMBOL_PREFERENCESDIALOG_IDNAME ID_PREFERENCESDIALOG
#define SYMBOL_PREFERENCESDIALOG_SIZE wxSize(399, 300)
#define SYMBOL_PREFERENCESDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * PreferencesDialog class declaration
 */

class PreferencesDialog: public wxPropertySheetDialog
{    
  DECLARE_DYNAMIC_CLASS( PreferencesDialog )
  DECLARE_EVENT_TABLE()

public:
  enum ItemCheck { ITEM_SHOW_UNITS, ITEM_THOUSAND_SEPARATOR };

  /// Constructors
  PreferencesDialog();
  PreferencesDialog( wxWindow* parent, wxWindowID id = SYMBOL_PREFERENCESDIALOG_IDNAME, const wxString& caption = SYMBOL_PREFERENCESDIALOG_TITLE, const wxPoint& pos = SYMBOL_PREFERENCESDIALOG_POSITION, const wxSize& size = SYMBOL_PREFERENCESDIALOG_SIZE, long style = SYMBOL_PREFERENCESDIALOG_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_PREFERENCESDIALOG_IDNAME, const wxString& caption = SYMBOL_PREFERENCESDIALOG_TITLE, const wxPoint& pos = SYMBOL_PREFERENCESDIALOG_POSITION, const wxSize& size = SYMBOL_PREFERENCESDIALOG_SIZE, long style = SYMBOL_PREFERENCESDIALOG_STYLE );

  /// Destructor
  ~PreferencesDialog();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin PreferencesDialog event handler declarations

  /// wxEVT_COLOURPICKER_CHANGED event handler for ID_COLOURPICKER_BACKGROUND
  void OnColourpickerBackgroundColourPickerChanged( wxColourPickerEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DEFAULT_TIMELINE
  void OnButtonDefaultTimelineClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DEFAULT_GRADIENT
  void OnButtonDefaultGradientClick( wxCommandEvent& event );

////@end PreferencesDialog event handler declarations

////@begin PreferencesDialog member function declarations

  UINT32 GetHistogramNumColumns() const { return histogramNumColumns ; }
  void SetHistogramNumColumns(UINT32 value) { histogramNumColumns = value ; }

  UINT32 GetHistogramPrecision() const { return histogramPrecision ; }
  void SetHistogramPrecision(UINT32 value) { histogramPrecision = value ; }

  UINT32 GetWhatWherePrecision() const { return whatWherePrecision ; }
  void SetWhatWherePrecision(UINT32 value) { whatWherePrecision = value ; }

  bool GetHistogramThousandSeparator() const { return histogramThousandSeparator ; }
  void SetHistogramThousandSeparator(bool value) { histogramThousandSeparator = value ; }

  bool GetHistogramShowUnits() const { return histogramShowUnits ; }
  void SetHistogramShowUnits(bool value) { histogramShowUnits = value ; }

  UINT32 GetHistogramMaxNumColumns() const { return histogramMaxNumColumns ; }
  void SetHistogramMaxNumColumns(UINT32 value) { histogramMaxNumColumns = value ; }

  UINT32 GetHistogramMaxPrecision() const { return histogramMaxPrecision ; }
  void SetHistogramMaxPrecision(UINT32 value) { histogramMaxPrecision = value ; }

  UINT32 GetWhatWhereMaxPrecision() const { return whatWhereMaxPrecision ; }
  void SetWhatWhereMaxPrecision(UINT32 value) { whatWhereMaxPrecision = value ; }

  bool GetGlobalFillStateGaps() const { return globalFillStateGaps ; }
  void SetGlobalFillStateGaps(bool value) { globalFillStateGaps = value ; }

  rgb GetTimelineColourBackground() const { return timelineColourBackground ; }
  void SetTimelineColourBackground(rgb value) { timelineColourBackground = value ; }

  rgb GetTimelineColourAxis() const { return timelineColourAxis ; }
  void SetTimelineColourAxis(rgb value) { timelineColourAxis = value ; }

  rgb GetTimelineColourLogical() const { return timelineColourLogical ; }
  void SetTimelineColourLogical(rgb value) { timelineColourLogical = value ; }

  rgb GetTimelineColourPhysical() const { return timelineColourPhysical ; }
  void SetTimelineColourPhysical(rgb value) { timelineColourPhysical = value ; }

  rgb GetGradientColourBegin() const { return gradientColourBegin ; }
  void SetGradientColourBegin(rgb value) { gradientColourBegin = value ; }

  rgb GetGradientColourEnd() const { return gradientColourEnd ; }
  void SetGradientColourEnd(rgb value) { gradientColourEnd = value ; }

  rgb GetGradientColourLow() const { return gradientColourLow ; }
  void SetGradientColourLow(rgb value) { gradientColourLow = value ; }

  rgb GetGradientColourTop() const { return gradientColourTop ; }
  void SetGradientColourTop(rgb value) { gradientColourTop = value ; }

  string GetTracesPath() const { return tracesPath ; }
  void SetTracesPath(string value) { tracesPath = value ; }

  string GetCfgsPath() const { return cfgsPath ; }
  void SetCfgsPath(string value) { cfgsPath = value ; }

  string GetTmpPath() const { return tmpPath ; }
  void SetTmpPath(string value) { tmpPath = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end PreferencesDialog member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  bool TransferDataToWindow();
  bool TransferDataFromWindow();

////@begin PreferencesDialog member variables
  wxCheckBox* checkGlobalFillStateGaps;
  wxDirPickerCtrl* dirPickerTrace;
  wxDirPickerCtrl* dirPickerCFG;
  wxDirPickerCtrl* dirPickerTmp;
  wxTextCtrl* txtWhatWherePrecision;
  wxTextCtrl* txt2DNumColumns;
  wxTextCtrl* txt2DPrecision;
  wxCheckListBox* check2DOptions;
  wxColourPickerCtrl* colourPickerBackground;
  wxColourPickerCtrl* colourPickerAxis;
  wxColourPickerCtrl* colourPickerLogical;
  wxColourPickerCtrl* colourPickerPhysical;
  wxColourPickerCtrl* colourPickerGradientBegin;
  wxColourPickerCtrl* colourPickerGradientEnd;
  wxColourPickerCtrl* colourPickerGradientLow;
  wxColourPickerCtrl* colourPickerGradientTop;
private:
  UINT32 histogramNumColumns;
  UINT32 histogramPrecision;
  UINT32 whatWherePrecision;
  bool histogramThousandSeparator;
  bool histogramShowUnits;
  UINT32 histogramMaxNumColumns;
  UINT32 histogramMaxPrecision;
  UINT32 whatWhereMaxPrecision;
  bool globalFillStateGaps;
  rgb timelineColourBackground;
  rgb timelineColourAxis;
  rgb timelineColourLogical;
  rgb timelineColourPhysical;
  rgb gradientColourBegin;
  rgb gradientColourEnd;
  rgb gradientColourLow;
  rgb gradientColourTop;
  string tracesPath;
  string cfgsPath;
  string tmpPath;
////@end PreferencesDialog member variables

  wxString formatNumber( long value );
  rgb wxColourToRGB( wxColour colour ) ;
  wxColour RGBTowxColour( rgb colour );

};

#endif
  // _PREFERENCESDIALOG_H_
