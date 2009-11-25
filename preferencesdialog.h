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
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"
////@end includes

#include "paraverconfig.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxDirPickerCtrl;
class wxSpinCtrl;
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
#define ID_PREFERENCES_TIMELINE_NAME_PREFIX 10098
#define ID_PREFERENCES_TIMELINE_NAME_FULL 10099
#define ID_PREFERENCES_TIMELINE_COMMUNICATION_LINES 10090
#define ID_PREFERENCES_TIMELINE_EVENT_LINES 10088
#define ID_PREFERENCES_TIMELINE_SEMANTIC_AS_COLOR 10091
#define ID_PREFERENCES_TIMELINE_COLOR 10086
#define ID_PREFERENCES_TIMELINE_GRADIENT 10015
#define ID_PREFERENCES_TIMELINE_DRAWMODE_TIME 10012
#define ID_PREFERENCES_TIMELINE_DRAWMODE_OBJECTS 10013
#define ID_PREFERENCES_TIMELINE_PIXEL_SIZE 10016
#define ID_CHECKBOX_TIMELINE_WW_SEMANTIC 10093
#define ID_CHECKBOX_TIMELINE_WW_EVENTS 10094
#define ID_CHECKBOX_TIMELINE_WW_COMMUNICATIONS 10095
#define ID_CHECKBOX_TIMELINE_WW_PREVIOUS_NEXT 10096
#define ID_CHECKBOX_TIMELINE_WW_TEXT 10097
#define ID_PREFERENCES_TIMELINE_WW_PRECISION 10000
#define ID_PREFERENCES_TIMELINE_SAVE_AS_IMAGE 10014
#define ID_PREFERENCES_TIMELINE_SAVE_AS_TEXT 10017
#define ID_PREFERENCES_HISTOGRAM 10071
#define ID_PREFERENCES_HISTOGRAM_NAME_PREFIX 10018
#define ID_PREFERENCES_HISTOGRAM_NAME_FULL 10019
#define ID_PREFERENCES_HISTOGRAM_MATRIX_ZOOM 10092
#define ID_PREFERENCES_HISTOGRAM_MATRIX_HORIZONTAL 10023
#define ID_PREFERENCES_HISTOGRAM_MATRIX_HIDE_EMPTY 10024
#define ID_PREFERENCES_HISTOGRAM_MATRIX_GRADIENT 10022
#define ID_PREFERENCES_HISTOGRAM_MATRIX_GRADIENT_FUNCTION 10020
#define ID_PREFERENCES_HISTOGRAM_MATRIX_DRAWMODE_SEMANTIC 10021
#define ID_PREFERENCES_HISTOGRAM_MATRIX_DRAWMODE_OBJECTS 10025
#define ID_PREFERENCES_HISTOGRAM_SCIENTIFIC_NOTATION 10026
#define ID_PREFERENCES_HISTOGRAM_THOUSANDS_SEPARATOR 10027
#define ID_PREFERENCES_HISTOGRAM_SHOW_UNITS 10028
#define ID_PREFERENCES_HISTOGRAM_PRECISION 10074
#define ID_PREFERENCES_HISTOGRAM_AUTOFIT_CONTROL 10030
#define ID_PREFERENCES_HISTOGRAM_AUTOFIT_3D 10030
#define ID_PREFERENCES_HISTOGRAM_AUTOFIT_DATA_GRADIENT 10029
#define ID_PREFERENCES_HISTOGRAM_NUMCOLUMNS 10075
#define ID_PREFERENCES_HISTOGRAM_SAVE_IMAGE_FORMAT 10031
#define ID_PREFERENCES_HISTOGRAM_SAVE_TXT_FORMAT 10032
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

  UINT32 GetTimelineWWPrecision() const { return timelineWWPrecision ; }
  void SetTimelineWWPrecision(UINT32 value) { timelineWWPrecision = value ; }

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

  string GetTimelineNameFormatPrefix() const { return timelineNameFormatPrefix ; }
  void SetTimelineNameFormatPrefix(string value) { timelineNameFormatPrefix = value ; }

  string GetTimelineNameFormatFull() const { return timelineNameFormatFull ; }
  void SetTimelineNameFormatFull(string value) { timelineNameFormatFull = value ; }

  bool GetTimelineEventLines() const { return timelineEventLines ; }
  void SetTimelineEventLines(bool value) { timelineEventLines = value ; }

  bool GetTimelineCommunicationLines() const { return timelineCommunicationLines ; }
  void SetTimelineCommunicationLines(bool value) { timelineCommunicationLines = value ; }

  bool GetTimelineFunctionAsColor() const { return timelineFunctionAsColor ; }
  void SetTimelineFunctionAsColor(bool value) { timelineFunctionAsColor = value ; }

  bool GetTimelineWWSemantic() const { return timelineWWSemantic ; }
  void SetTimelineWWSemantic(bool value) { timelineWWSemantic = value ; }

  bool GetTimelineWWEvents() const { return timelineWWEvents ; }
  void SetTimelineWWEvents(bool value) { timelineWWEvents = value ; }

  bool GetTimelineWWCommunications() const { return timelineWWCommunications ; }
  void SetTimelineWWCommunications(bool value) { timelineWWCommunications = value ; }

  bool GetTimelineWWPreviousNext() const { return timelineWWPreviousNext ; }
  void SetTimelineWWPreviousNext(bool value) { timelineWWPreviousNext = value ; }

  bool GetTimelineWWText() const { return timelineWWText ; }
  void SetTimelineWWText(bool value) { timelineWWText = value ; }

  UINT32 GetTimelineColor() const { return timelineColor ; }
  void SetTimelineColor(UINT32 value) { timelineColor = value ; }

  UINT32 GetTimelineGradientFunction() const { return timelineGradientFunction ; }
  void SetTimelineGradientFunction(UINT32 value) { timelineGradientFunction = value ; }

  UINT32 GetTimelineDrawmodeTime() const { return timelineDrawmodeTime ; }
  void SetTimelineDrawmodeTime(UINT32 value) { timelineDrawmodeTime = value ; }

  UINT32 GetTimelineDrawmodeObjects() const { return timelineDrawmodeObjects ; }
  void SetTimelineDrawmodeObjects(UINT32 value) { timelineDrawmodeObjects = value ; }

  UINT32 GetTimelinePixelSize() const { return timelinePixelSize ; }
  void SetTimelinePixelSize(UINT32 value) { timelinePixelSize = value ; }

  UINT32 GetTimelineSaveImageFormat() const { return timelineSaveImageFormat ; }
  void SetTimelineSaveImageFormat(UINT32 value) { timelineSaveImageFormat = value ; }

  UINT32 GetTimelineSaveTextFormat() const { return timelineSaveTextFormat ; }
  void SetTimelineSaveTextFormat(UINT32 value) { timelineSaveTextFormat = value ; }

  string GetHistogramNameFormatPrefix() const { return histogramNameFormatPrefix ; }
  void SetHistogramNameFormatPrefix(string value) { histogramNameFormatPrefix = value ; }

  string GetHistogramNameFormatFull() const { return histogramNameFormatFull ; }
  void SetHistogramNameFormatFull(string value) { histogramNameFormatFull = value ; }

  bool GetHistogramZoom() const { return histogramZoom ; }
  void SetHistogramZoom(bool value) { histogramZoom = value ; }

  bool GetHistogramHideEmpty() const { return histogramHideEmpty ; }
  void SetHistogramHideEmpty(bool value) { histogramHideEmpty = value ; }

  bool GetHistogramHorizontal() const { return histogramHorizontal ; }
  void SetHistogramHorizontal(bool value) { histogramHorizontal = value ; }

  bool GetHistogramShowGradient() const { return histogramShowGradient ; }
  void SetHistogramShowGradient(bool value) { histogramShowGradient = value ; }

  UINT32 GetHistogramGradientFunction() const { return histogramGradientFunction ; }
  void SetHistogramGradientFunction(UINT32 value) { histogramGradientFunction = value ; }

  UINT32 GetHistogramDrawmodeObjects() const { return histogramDrawmodeObjects ; }
  void SetHistogramDrawmodeObjects(UINT32 value) { histogramDrawmodeObjects = value ; }

  bool GetHistogramScientificNotation() const { return histogramScientificNotation ; }
  void SetHistogramScientificNotation(bool value) { histogramScientificNotation = value ; }

  bool GetHistogramAutofitControlScale() const { return histogramAutofitControlScale ; }
  void SetHistogramAutofitControlScale(bool value) { histogramAutofitControlScale = value ; }

  bool GetHistogramAutofitDataGradient() const { return histogramAutofitDataGradient ; }
  void SetHistogramAutofitDataGradient(bool value) { histogramAutofitDataGradient = value ; }

  UINT32 GetHistogramSaveImageFormat() const { return histogramSaveImageFormat ; }
  void SetHistogramSaveImageFormat(UINT32 value) { histogramSaveImageFormat = value ; }

  UINT32 GetHistogramSaveTextFormat() const { return histogramSaveTextFormat ; }
  void SetHistogramSaveTextFormat(UINT32 value) { histogramSaveTextFormat = value ; }

  bool GetHistogramDrawmodeSemantic() const { return histogramDrawmodeSemantic ; }
  void SetHistogramDrawmodeSemantic(bool value) { histogramDrawmodeSemantic = value ; }

  bool GetHistogramAutofit3DScale() const { return histogramAutofit3DScale ; }
  void SetHistogramAutofit3DScale(bool value) { histogramAutofit3DScale = value ; }

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
  wxTextCtrl* txtTimelineNameFormatPrefix;
  wxTextCtrl* txtTimelineNameFormatFull;
  wxCheckBox* checkTimelineCommunicationLines;
  wxCheckBox* checkTimelineEventLines;
  wxCheckBox* checkTimelineFunctionAsColor;
  wxChoice* choiceTimelineColor;
  wxChoice* choiceTimelineGradientFunction;
  wxChoice* choiceTimelineDrawmodeTime;
  wxChoice* choiceTimelineDrawmodeObjects;
  wxChoice* choiceTimelinePixelSize;
  wxCheckBox* checkTimelineWWSemantic;
  wxCheckBox* checkTimelineWWEvents;
  wxCheckBox* checkTimelineWWCommunications;
  wxCheckBox* checkTimelineWWPreviousNext;
  wxCheckBox* checkTimelineWWText;
  wxSpinCtrl* txtTimelineWWPrecision;
  wxChoice* choiceTimelineSaveImageFormat;
  wxChoice* choiceTimelineSaveTextFormat;
  wxTextCtrl* txtHistogramNameFormatPrefix;
  wxTextCtrl* txtHistogramNameFormatFull;
  wxCheckBox* checkHistogramZoom;
  wxCheckBox* checkHistogramHorizontal;
  wxCheckBox* checkHistogramHideEmpty;
  wxCheckBox* checkHistogramShowGradient;
  wxChoice* choiceHistogramGradientFunction;
  wxChoice* choiceHistogramDrawmodeSemantic;
  wxChoice* choiceHistogramDrawmodeObjects;
  wxCheckBox* checkHistogramScientificNotation;
  wxCheckBox* checkHistogramThousandsSeparator;
  wxCheckBox* checkHistogramShowUnits;
  wxSpinCtrl* txtHistogramPrecision;
  wxCheckBox* checkHistogramAutofitControlScale;
  wxCheckBox* checkHistogramAutofit3DScale;
  wxCheckBox* checkHistogramAutofitDataGradient;
  wxSpinCtrl* txtHistogramNumColumns;
  wxChoice* choiceHistogramSaveImageFormat;
  wxChoice* choiceHistogramSaveTextFormat;
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
  UINT32 timelineWWPrecision;
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
  string timelineNameFormatPrefix;
  string timelineNameFormatFull;
  bool timelineEventLines;
  bool timelineCommunicationLines;
  bool timelineFunctionAsColor;
  bool timelineWWSemantic;
  bool timelineWWEvents;
  bool timelineWWCommunications;
  bool timelineWWPreviousNext;
  bool timelineWWText;
  UINT32 timelineColor;
  UINT32 timelineGradientFunction;
  UINT32 timelineDrawmodeTime;
  UINT32 timelineDrawmodeObjects;
  UINT32 timelinePixelSize;
  UINT32 timelineSaveImageFormat;
  UINT32 timelineSaveTextFormat;
  string histogramNameFormatPrefix;
  string histogramNameFormatFull;
  bool histogramZoom;
  bool histogramHideEmpty;
  bool histogramHorizontal;
  bool histogramShowGradient;
  UINT32 histogramGradientFunction;
  UINT32 histogramDrawmodeObjects;
  bool histogramScientificNotation;
  bool histogramAutofitControlScale;
  bool histogramAutofitDataGradient;
  UINT32 histogramSaveImageFormat;
  UINT32 histogramSaveTextFormat;
  bool histogramDrawmodeSemantic;
  bool histogramAutofit3DScale;
////@end PreferencesDialog member variables

  wxString formatNumber( long value );
  void setLabelsChoiceBox( const vector< string > &list,
                           const UINT32 &selected,
                           wxChoice *choiceBox );
  rgb wxColourToRGB( wxColour colour ) ;
  wxColour RGBTowxColour( rgb colour );

};

#endif
  // _PREFERENCESDIALOG_H_
