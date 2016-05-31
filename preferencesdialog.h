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

#ifndef _PREFERENCESDIALOG_H_
#define _PREFERENCESDIALOG_H_


/*!
 * Includes
 */

#include <map>
#include <string>

////@begin includes
#include "wx/propdlg.h"
#include "wx/spinctrl.h"
#include "filebrowserbutton.h"
#include "wx/clrpicker.h"
#include "wx/statline.h"
////@end includes

#include "wx/bmpbuttn.h"
#include "wx/filedlg.h"

#include "paraverconfig.h"
#include "workspace.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxSpinCtrl;
class DirBrowserButton;
class wxColourPickerCtrl;
class FileBrowserButton;
////@end forward declarations

/*!
 * Control identifiers
 */
////@begin control identifiers
#define ID_PREFERENCESDIALOG 10069
#define ID_PREFERENCES_GLOBAL 10073
#define ID_PREFERENCES_GLOBAL_FILLGAPS 10085
#define ID_PREFERENCES_GLOBAL_FULLTRACEPATH 10010
#define ID_TEXTCTRL_MAXIMUM_LOADABLE_TRACE_SIZE 10156
#define ID_TEXTCTRL_DEFAULT_TRACE 10220
#define ID_BUTTON_DIR_BROWSER_TRACE 10238
#define ID_TEXTCTRL_DEFAULT_CFGS 10089
#define ID_DIRBROWSERBUTTON_DEFAULT_CFGS 10243
#define ID_TEXTCTRL_DEFAULT_XMLS 10239
#define ID_BUTTON_DEFAULT_XMLS 10242
#define ID_TEXTCTRL_DEFAULT_TUTORIALS 10240
#define ID_DIRBROWSERBUTTON_DEFAULT_TUTORIALS 10244
#define ID_TEXTCTRL_DEFAULT_TMP 10241
#define ID_DIRBROWSERBUTTON_DEFAULT_TMP 10245
#define ID_PREFERENCES_GLOBAL_SINGLE_INSTANCE 10158
#define ID_PREFERENCES_GLOBAL_TIME_SESSION 10168
#define ID_PREFERENCES_TIMELINE 10072
#define ID_PREFERENCES_TIMELINE_NAME_PREFIX 10098
#define ID_PREFERENCES_TIMELINE_NAME_FULL 10099
#define ID_PREFERENCES_TIMELINE_COMMUNICATION_LINES 10090
#define ID_PREFERENCES_TIMELINE_EVENT_LINES 10088
#define ID_PREFERENCES_TIMELINE_COLOR 10086
#define ID_PREFERENCES_TIMELINE_GRADIENT 10015
#define ID_PREFERENCES_TIMELINE_DRAWMODE_TIME 10012
#define ID_PREFERENCES_TIMELINE_DRAWMODE_OBJECTS 10013
#define ID_PREFERENCES_TIMELINE_PIXEL_SIZE 10016
#define ID_PREFERENCES_TIMELINE_LABELS 10208
#define ID_PREFERENCES_TIMELINE_OBJECT_AXIS 10254
#define ID_CHECKBOX_TIMELINE_WW_SEMANTIC 10093
#define ID_CHECKBOX_TIMELINE_WW_EVENTS 10094
#define ID_CHECKBOX_TIMELINE_WW_COMMUNICATIONS 10095
#define ID_CHECKBOX_TIMELINE_WW_PREVIOUS_NEXT 10096
#define ID_CHECKBOX_TIMELINE_WW_TEXT 10097
#define ID_PREFERENCES_TIMELINE_WW_PRECISION 10000
#define ID_PREFERENCES_TIMELINE_WW_EVENT_PIXELS 10167
#define ID_PREFERENCES_TIMELINE_SAVE_AS_IMAGE 10014
#define ID_PREFERENCES_TIMELINE_SAVE_AS_TEXT 10017
#define ID_PREFERENCES_HISTOGRAM 10071
#define ID_PREFERENCES_HISTOGRAM_SKIP_CREATE_DIALOG 10033
#define ID_PREFERENCES_HISTOGRAM_NAME_PREFIX 10018
#define ID_PREFERENCES_HISTOGRAM_NAME_FULL 10019
#define ID_PREFERENCES_HISTOGRAM_MATRIX_ZOOM 10092
#define ID_PREFERENCES_HISTOGRAM_MATRIX_HORIZONTAL 10023
#define ID_PREFERENCES_HISTOGRAM_MATRIX_HIDE_EMPTY 10024
#define ID_PREFERENCES_HISTOGRAM_MATRIX_GRADIENT 10022
#define ID_PREFERENCES_HISTOGRAM_MATRIX_LABELS_COLOR 10102
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
#define ID_COLOURPICKER_ZERO 10104
#define ID_COLOURPICKER_PUNCTUAL 10034
#define ID_COLOURPICKER_LOGICAL 10007
#define ID_COLOURPICKER_PHYSICAL 10008
#define ID_BUTTON_DEFAULT_TIMELINE 10087
#define ID_COLOURPICKER_GRADBEGIN 10003
#define ID_COLOURPICKER_GRADEND 10004
#define ID_COLOURPICKER_GRADLOW 10005
#define ID_COLOURPICKER_GRADTOP 10006
#define ID_BUTTON_DEFAULT_GRADIENT 10009
#define ID_PREFERENCES_WORKSPACES 10269
#define ID_LISTBOX_WORKSPACES 10270
#define ID_BUTTON_WORKSPACES_ADD 10271
#define ID_BUTTON_WORKSPACES_DELETE 10272
#define ID_BUTTON_WORKSPACES_UP 10273
#define ID_BUTTON_WORKSPACES_DOWN 10274
#define ID_TEXT_WORKSPACE_NAME 10275
#define ID_STATIC_WORKSPACE_AUTOTYPES 10285
#define ID_TEXT_WORKSPACE_AUTOTYPES 10011
#define ID_LISTBOX_HINTS_WORKSPACE 10276
#define ID_BUTTON_HINT_ADD 10277
#define ID_BUTTON_HINT_DELETE 10278
#define ID_BITMAP_HINT_UP 10279
#define ID_BUTTON_HINT_DOWN 10280
#define ID_TEXTCTRL_WORKSPACE_HINT_PATH 10283
#define ID_FILE_BUTTON_WORKSPACE_HINT_PATH 10282
#define ID_TEXTCTRL_WORKSPACE_HINT_DESCRIPTION 10281
#define ID_PREFERENCES_FILTERS 10070
#define SYMBOL_PREFERENCESDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PREFERENCESDIALOG_TITLE _("Preferences")
#define SYMBOL_PREFERENCESDIALOG_IDNAME ID_PREFERENCESDIALOG
#define SYMBOL_PREFERENCESDIALOG_SIZE wxSize(400, 300)
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

  /// wxEVT_UPDATE_UI event handler for ID_COLOURPICKER_ZERO
  void OnColourpickerZeroUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DEFAULT_TIMELINE
  void OnButtonDefaultTimelineClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DEFAULT_GRADIENT
  void OnButtonDefaultGradientClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX_WORKSPACES
  void OnListboxWorkspacesSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_WORKSPACES_ADD
  void OnButtonWorkspacesAddClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_WORKSPACES_DELETE
  void OnButtonWorkspacesDeleteClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_BUTTON_WORKSPACES_DELETE
  void OnButtonWorkspacesDeleteUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_WORKSPACES_UP
  void OnButtonWorkspacesUpClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_BUTTON_WORKSPACES_UP
  void OnButtonWorkspacesUpUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_WORKSPACES_DOWN
  void OnButtonWorkspacesDownClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_BUTTON_WORKSPACES_DOWN
  void OnButtonWorkspacesDownUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXT_WORKSPACE_NAME
  void OnTextWorkspaceNameTextUpdated( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TEXT_WORKSPACE_NAME
  void OnTextWorkspaceNameUpdate( wxUpdateUIEvent& event );

  /// wxEVT_KILL_FOCUS event handler for ID_TEXT_WORKSPACE_NAME
  void OnTextWorkspaceNameKillFocus( wxFocusEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_STATIC_WORKSPACE_AUTOTYPES
  void OnStaticWorkspaceAutotypesUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXT_WORKSPACE_AUTOTYPES
  void OnTextWorkspaceAutotypesTextUpdated( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TEXT_WORKSPACE_AUTOTYPES
  void OnTextWorkspaceAutotypesUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX_HINTS_WORKSPACE
  void OnListboxHintsWorkspaceSelected( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_LISTBOX_HINTS_WORKSPACE
  void OnListboxHintsWorkspaceUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_HINT_ADD
  void OnButtonHintAddClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_BUTTON_HINT_ADD
  void OnButtonHintAddUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_HINT_DELETE
  void OnButtonHintDeleteClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_BUTTON_HINT_DELETE
  void OnButtonHintDeleteUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAP_HINT_UP
  void OnBitmapHintUpClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_BITMAP_HINT_UP
  void OnBitmapHintUpUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_HINT_DOWN
  void OnButtonHintDownClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_BUTTON_HINT_DOWN
  void OnButtonHintDownUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_WORKSPACE_HINT_PATH
  void OnTextctrlWorkspaceHintPathTextUpdated( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TEXTCTRL_WORKSPACE_HINT_PATH
  void OnTextctrlWorkspaceHintPathUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_FILE_BUTTON_WORKSPACE_HINT_PATH
  void OnFileButtonWorkspaceHintPathUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_WORKSPACE_HINT_DESCRIPTION
  void OnTextctrlWorkspaceHintDescriptionTextUpdated( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TEXTCTRL_WORKSPACE_HINT_DESCRIPTION
  void OnTextctrlWorkspaceHintDescriptionUpdate( wxUpdateUIEvent& event );

////@end PreferencesDialog event handler declarations

////@begin PreferencesDialog member function declarations

  std::string GetCfgsPath() const { return cfgsPath ; }
  void SetCfgsPath(std::string value) { cfgsPath = value ; }

  bool GetColorUseZero() const { return colorUseZero ; }
  void SetColorUseZero(bool value) { colorUseZero = value ; }

  std::string GetFiltersXMLPath() const { return filtersXMLPath ; }
  void SetFiltersXMLPath(std::string value) { filtersXMLPath = value ; }

  bool GetGlobalFillStateGaps() const { return globalFillStateGaps ; }
  void SetGlobalFillStateGaps(bool value) { globalFillStateGaps = value ; }

  bool GetGlobalFullTracePath() const { return globalFullTracePath ; }
  void SetGlobalFullTracePath(bool value) { globalFullTracePath = value ; }

  rgb GetGradientColourBegin() const { return gradientColourBegin ; }
  void SetGradientColourBegin(rgb value) { gradientColourBegin = value ; }

  rgb GetGradientColourEnd() const { return gradientColourEnd ; }
  void SetGradientColourEnd(rgb value) { gradientColourEnd = value ; }

  rgb GetGradientColourLow() const { return gradientColourLow ; }
  void SetGradientColourLow(rgb value) { gradientColourLow = value ; }

  rgb GetGradientColourTop() const { return gradientColourTop ; }
  void SetGradientColourTop(rgb value) { gradientColourTop = value ; }

  bool GetHistogramAutofit3DScale() const { return histogramAutofit3DScale ; }
  void SetHistogramAutofit3DScale(bool value) { histogramAutofit3DScale = value ; }

  bool GetHistogramAutofitControlScale() const { return histogramAutofitControlScale ; }
  void SetHistogramAutofitControlScale(bool value) { histogramAutofitControlScale = value ; }

  bool GetHistogramAutofitDataGradient() const { return histogramAutofitDataGradient ; }
  void SetHistogramAutofitDataGradient(bool value) { histogramAutofitDataGradient = value ; }

  PRV_UINT32 GetHistogramDrawmodeObjects() const { return histogramDrawmodeObjects ; }
  void SetHistogramDrawmodeObjects(PRV_UINT32 value) { histogramDrawmodeObjects = value ; }

  PRV_UINT32 GetHistogramDrawmodeSemantic() const { return histogramDrawmodeSemantic ; }
  void SetHistogramDrawmodeSemantic(PRV_UINT32 value) { histogramDrawmodeSemantic = value ; }

  PRV_UINT32 GetHistogramGradientFunction() const { return histogramGradientFunction ; }
  void SetHistogramGradientFunction(PRV_UINT32 value) { histogramGradientFunction = value ; }

  bool GetHistogramHideEmpty() const { return histogramHideEmpty ; }
  void SetHistogramHideEmpty(bool value) { histogramHideEmpty = value ; }

  bool GetHistogramHorizontal() const { return histogramHorizontal ; }
  void SetHistogramHorizontal(bool value) { histogramHorizontal = value ; }

  bool GetHistogramLabelsColor() const { return histogramLabelsColor ; }
  void SetHistogramLabelsColor(bool value) { histogramLabelsColor = value ; }

  THistogramColumn GetHistogramMaxNumColumns() const { return histogramMaxNumColumns ; }
  void SetHistogramMaxNumColumns(THistogramColumn value) { histogramMaxNumColumns = value ; }

  PRV_UINT32 GetHistogramMaxPrecision() const { return histogramMaxPrecision ; }
  void SetHistogramMaxPrecision(PRV_UINT32 value) { histogramMaxPrecision = value ; }

  std::string GetHistogramNameFormatFull() const { return histogramNameFormatFull ; }
  void SetHistogramNameFormatFull(std::string value) { histogramNameFormatFull = value ; }

  std::string GetHistogramNameFormatPrefix() const { return histogramNameFormatPrefix ; }
  void SetHistogramNameFormatPrefix(std::string value) { histogramNameFormatPrefix = value ; }

  THistogramColumn GetHistogramNumColumns() const { return histogramNumColumns ; }
  void SetHistogramNumColumns(THistogramColumn value) { histogramNumColumns = value ; }

  PRV_UINT32 GetHistogramPrecision() const { return histogramPrecision ; }
  void SetHistogramPrecision(PRV_UINT32 value) { histogramPrecision = value ; }

  PRV_UINT32 GetHistogramSaveImageFormat() const { return histogramSaveImageFormat ; }
  void SetHistogramSaveImageFormat(PRV_UINT32 value) { histogramSaveImageFormat = value ; }

  PRV_UINT32 GetHistogramSaveTextFormat() const { return histogramSaveTextFormat ; }
  void SetHistogramSaveTextFormat(PRV_UINT32 value) { histogramSaveTextFormat = value ; }

  bool GetHistogramScientificNotation() const { return histogramScientificNotation ; }
  void SetHistogramScientificNotation(bool value) { histogramScientificNotation = value ; }

  bool GetHistogramShowGradient() const { return histogramShowGradient ; }
  void SetHistogramShowGradient(bool value) { histogramShowGradient = value ; }

  bool GetHistogramShowUnits() const { return histogramShowUnits ; }
  void SetHistogramShowUnits(bool value) { histogramShowUnits = value ; }

  bool GetHistogramSkipCreateDialog() const { return histogramSkipCreateDialog ; }
  void SetHistogramSkipCreateDialog(bool value) { histogramSkipCreateDialog = value ; }

  bool GetHistogramThousandSeparator() const { return histogramThousandSeparator ; }
  void SetHistogramThousandSeparator(bool value) { histogramThousandSeparator = value ; }

  bool GetHistogramZoom() const { return histogramZoom ; }
  void SetHistogramZoom(bool value) { histogramZoom = value ; }

  float GetMaximumTraceSize() const { return maximumTraceSize ; }
  void SetMaximumTraceSize(float value) { maximumTraceSize = value ; }

  PRV_UINT16 GetSessionSaveTime() const { return sessionSaveTime ; }
  void SetSessionSaveTime(PRV_UINT16 value) { sessionSaveTime = value ; }

  bool GetSingleInstance() const { return singleInstance ; }
  void SetSingleInstance(bool value) { singleInstance = value ; }

  PRV_UINT32 GetTimelineColor() const { return timelineColor ; }
  void SetTimelineColor(PRV_UINT32 value) { timelineColor = value ; }

  rgb GetTimelineColourAxis() const { return timelineColourAxis ; }
  void SetTimelineColourAxis(rgb value) { timelineColourAxis = value ; }

  rgb GetTimelineColourBackground() const { return timelineColourBackground ; }
  void SetTimelineColourBackground(rgb value) { timelineColourBackground = value ; }

  rgb GetTimelineColourLogical() const { return timelineColourLogical ; }
  void SetTimelineColourLogical(rgb value) { timelineColourLogical = value ; }

  rgb GetTimelineColourPhysical() const { return timelineColourPhysical ; }
  void SetTimelineColourPhysical(rgb value) { timelineColourPhysical = value ; }

  rgb GetTimelineColourZero() const { return timelineColourZero ; }
  void SetTimelineColourZero(rgb value) { timelineColourZero = value ; }

  bool GetTimelineCommunicationLines() const { return timelineCommunicationLines ; }
  void SetTimelineCommunicationLines(bool value) { timelineCommunicationLines = value ; }

  PRV_UINT32 GetTimelineDrawmodeObjects() const { return timelineDrawmodeObjects ; }
  void SetTimelineDrawmodeObjects(PRV_UINT32 value) { timelineDrawmodeObjects = value ; }

  PRV_UINT32 GetTimelineDrawmodeTime() const { return timelineDrawmodeTime ; }
  void SetTimelineDrawmodeTime(PRV_UINT32 value) { timelineDrawmodeTime = value ; }

  bool GetTimelineEventLines() const { return timelineEventLines ; }
  void SetTimelineEventLines(bool value) { timelineEventLines = value ; }

  PRV_UINT32 GetTimelineGradientFunction() const { return timelineGradientFunction ; }
  void SetTimelineGradientFunction(PRV_UINT32 value) { timelineGradientFunction = value ; }

  std::string GetTimelineNameFormatFull() const { return timelineNameFormatFull ; }
  void SetTimelineNameFormatFull(std::string value) { timelineNameFormatFull = value ; }

  std::string GetTimelineNameFormatPrefix() const { return timelineNameFormatPrefix ; }
  void SetTimelineNameFormatPrefix(std::string value) { timelineNameFormatPrefix = value ; }

  PRV_UINT32 GetTimelineObjectAxis() const { return timelineObjectAxis ; }
  void SetTimelineObjectAxis(PRV_UINT32 value) { timelineObjectAxis = value ; }

  PRV_UINT32 GetTimelineObjectLabels() const { return timelineObjectLabels ; }
  void SetTimelineObjectLabels(PRV_UINT32 value) { timelineObjectLabels = value ; }

  PRV_UINT32 GetTimelinePixelSize() const { return timelinePixelSize ; }
  void SetTimelinePixelSize(PRV_UINT32 value) { timelinePixelSize = value ; }

  PRV_UINT32 GetTimelineSaveImageFormat() const { return timelineSaveImageFormat ; }
  void SetTimelineSaveImageFormat(PRV_UINT32 value) { timelineSaveImageFormat = value ; }

  PRV_UINT32 GetTimelineSaveTextFormat() const { return timelineSaveTextFormat ; }
  void SetTimelineSaveTextFormat(PRV_UINT32 value) { timelineSaveTextFormat = value ; }

  bool GetTimelineWWCommunications() const { return timelineWWCommunications ; }
  void SetTimelineWWCommunications(bool value) { timelineWWCommunications = value ; }

  PRV_INT16 GetTimelineWWEventPixels() const { return timelineWWEventPixels ; }
  void SetTimelineWWEventPixels(PRV_INT16 value) { timelineWWEventPixels = value ; }

  bool GetTimelineWWEvents() const { return timelineWWEvents ; }
  void SetTimelineWWEvents(bool value) { timelineWWEvents = value ; }

  PRV_UINT32 GetTimelineWWPrecision() const { return timelineWWPrecision ; }
  void SetTimelineWWPrecision(PRV_UINT32 value) { timelineWWPrecision = value ; }

  bool GetTimelineWWPreviousNext() const { return timelineWWPreviousNext ; }
  void SetTimelineWWPreviousNext(bool value) { timelineWWPreviousNext = value ; }

  bool GetTimelineWWSemantic() const { return timelineWWSemantic ; }
  void SetTimelineWWSemantic(bool value) { timelineWWSemantic = value ; }

  bool GetTimelineWWText() const { return timelineWWText ; }
  void SetTimelineWWText(bool value) { timelineWWText = value ; }

  std::string GetTmpPath() const { return tmpPath ; }
  void SetTmpPath(std::string value) { tmpPath = value ; }

  std::string GetTracesPath() const { return tracesPath ; }
  void SetTracesPath(std::string value) { tracesPath = value ; }

  std::string GetTutorialsPath() const { return tutorialsPath ; }
  void SetTutorialsPath(std::string value) { tutorialsPath = value ; }

  PRV_UINT32 GetWhatWhereMaxPrecision() const { return whatWhereMaxPrecision ; }
  void SetWhatWhereMaxPrecision(PRV_UINT32 value) { whatWhereMaxPrecision = value ; }

  std::map<wxString,Workspace> GetWorkspaceContainer() const { return workspaceContainer ; }
  void SetWorkspaceContainer(std::map<wxString,Workspace> value) { workspaceContainer = value ; }

  rgb GetTimelineColourPunctual() const { return timelineColourPunctual ; }
  void SetTimelineColourPunctual(rgb value) { timelineColourPunctual = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end PreferencesDialog member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  bool TransferDataToWindow();
  bool TransferDataFromWindow();

  /* SetPanel allowed values (tab IDs):
      ID_PREFERENCES_GLOBAL, ID_PREFERENCES_TIMELINE, ID_PREFERENCES_HISTOGRAM,
      ID_PREFERENCES_COLOR, ID_PREFERENCES_WORKSPACES
  */
  bool SetPanel( wxWindowID whichPanelID );

////@begin PreferencesDialog member variables
  wxPanel* panelGlobal;
  wxCheckBox* checkGlobalFillStateGaps;
  wxCheckBox* checkGlobalFullTracePath;
  wxSpinCtrl* txtMaximumTraceSize;
  wxTextCtrl* textCtrlTrace;
  DirBrowserButton* dirBrowserButtonTrace;
  wxTextCtrl* textCtrlCFG;
  DirBrowserButton* dirBrowserButtonCFG;
  wxTextCtrl* textCtrlXML;
  DirBrowserButton* dirBrowserButtonXML;
  wxTextCtrl* textCtrlTutorials;
  DirBrowserButton* dirBrowserButtonTutorials;
  wxTextCtrl* textCtrlTmp;
  DirBrowserButton* dirBrowserButtonTmp;
  wxCheckBox* checkGlobalSingleInstance;
  wxSpinCtrl* spinSessionTime;
  wxPanel* panelTimeline;
  wxTextCtrl* txtTimelineNameFormatPrefix;
  wxTextCtrl* txtTimelineNameFormatFull;
  wxCheckBox* checkTimelineCommunicationLines;
  wxCheckBox* checkTimelineEventLines;
  wxChoice* choiceTimelineColor;
  wxChoice* choiceTimelineGradientFunction;
  wxChoice* choiceTimelineDrawmodeTime;
  wxChoice* choiceTimelineDrawmodeObjects;
  wxChoice* choiceTimelinePixelSize;
  wxChoice* choiceTimelineLabels;
  wxChoice* choiceTimelineObjectAxis;
  wxCheckBox* checkTimelineWWSemantic;
  wxCheckBox* checkTimelineWWEvents;
  wxCheckBox* checkTimelineWWCommunications;
  wxCheckBox* checkTimelineWWPreviousNext;
  wxCheckBox* checkTimelineWWText;
  wxSpinCtrl* txtTimelineWWPrecision;
  wxSpinCtrl* txtTimelineWWEventPixels;
  wxChoice* choiceTimelineSaveImageFormat;
  wxChoice* choiceTimelineSaveTextFormat;
  wxPanel* panelHistogram;
  wxCheckBox* checkHistogramSkipCreateDialog;
  wxTextCtrl* txtHistogramNameFormatPrefix;
  wxTextCtrl* txtHistogramNameFormatFull;
  wxCheckBox* checkHistogramZoom;
  wxCheckBox* checkHistogramHorizontal;
  wxCheckBox* checkHistogramHideEmpty;
  wxCheckBox* checkHistogramShowGradient;
  wxCheckBox* checkHistogramLabelsColor;
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
  wxPanel* panelColor;
  wxColourPickerCtrl* colourPickerBackground;
  wxColourPickerCtrl* colourPickerAxis;
  wxCheckBox* checkZero;
  wxColourPickerCtrl* colourPickerZero;
  wxColourPickerCtrl* colourPickerPunctual;
  wxColourPickerCtrl* colourPickerLogical;
  wxColourPickerCtrl* colourPickerPhysical;
  wxColourPickerCtrl* colourPickerGradientBegin;
  wxColourPickerCtrl* colourPickerGradientEnd;
  wxColourPickerCtrl* colourPickerGradientLow;
  wxColourPickerCtrl* colourPickerGradientTop;
  wxPanel* panelWorkspaces;
  wxListBox* listWorkspaces;
  wxBitmapButton* buttonAddWorkspace;
  wxBitmapButton* buttonDeleteWorkspace;
  wxBitmapButton* buttonUpWorkspace;
  wxBitmapButton* buttonDownWorkspace;
  wxTextCtrl* txtWorkspaceName;
  wxTextCtrl* txtAutoTypes;
  wxListBox* listHintsWorkspace;
  wxBitmapButton* buttonAddHint;
  wxBitmapButton* buttonDeleteHint;
  wxBitmapButton* buttonUpHint;
  wxBitmapButton* buttonDownHint;
  wxTextCtrl* txtHintPath;
  FileBrowserButton* fileBrowserHintPath;
  wxTextCtrl* txtHintDescription;
  wxPanel* panelFilters;
private:
  std::string cfgsPath;
  bool colorUseZero;
  std::string filtersXMLPath;
  bool globalFillStateGaps;
  bool globalFullTracePath;
  rgb gradientColourBegin;
  rgb gradientColourEnd;
  rgb gradientColourLow;
  rgb gradientColourTop;
  bool histogramAutofit3DScale;
  bool histogramAutofitControlScale;
  bool histogramAutofitDataGradient;
  PRV_UINT32 histogramDrawmodeObjects;
  PRV_UINT32 histogramDrawmodeSemantic;
  PRV_UINT32 histogramGradientFunction;
  bool histogramHideEmpty;
  bool histogramHorizontal;
  bool histogramLabelsColor;
  THistogramColumn histogramMaxNumColumns;
  PRV_UINT32 histogramMaxPrecision;
  std::string histogramNameFormatFull;
  std::string histogramNameFormatPrefix;
  THistogramColumn histogramNumColumns;
  PRV_UINT32 histogramPrecision;
  PRV_UINT32 histogramSaveImageFormat;
  PRV_UINT32 histogramSaveTextFormat;
  bool histogramScientificNotation;
  bool histogramShowGradient;
  bool histogramShowUnits;
  bool histogramSkipCreateDialog;
  bool histogramThousandSeparator;
  bool histogramZoom;
  float maximumTraceSize;
  PRV_UINT16 sessionSaveTime;
  bool singleInstance;
  PRV_UINT32 timelineColor;
  rgb timelineColourAxis;
  rgb timelineColourBackground;
  rgb timelineColourLogical;
  rgb timelineColourPhysical;
  rgb timelineColourZero;
  bool timelineCommunicationLines;
  PRV_UINT32 timelineDrawmodeObjects;
  PRV_UINT32 timelineDrawmodeTime;
  bool timelineEventLines;
  PRV_UINT32 timelineGradientFunction;
  std::string timelineNameFormatFull;
  std::string timelineNameFormatPrefix;
  PRV_UINT32 timelineObjectAxis;
  PRV_UINT32 timelineObjectLabels;
  PRV_UINT32 timelinePixelSize;
  PRV_UINT32 timelineSaveImageFormat;
  PRV_UINT32 timelineSaveTextFormat;
  bool timelineWWCommunications;
  PRV_INT16 timelineWWEventPixels;
  bool timelineWWEvents;
  PRV_UINT32 timelineWWPrecision;
  bool timelineWWPreviousNext;
  bool timelineWWSemantic;
  bool timelineWWText;
  std::string tmpPath;
  std::string tracesPath;
  std::string tutorialsPath;
  PRV_UINT32 whatWhereMaxPrecision;
  std::map<wxString,Workspace> workspaceContainer;
  rgb timelineColourPunctual;
////@end PreferencesDialog member variables

  // To keep the original name of the selected workspace
  wxString originalWorkspaceName;
  
  std::map< wxWindowID, size_t > panelID;
  
  wxString formatNumber( long value );
  void setLabelsChoiceBox( const std::vector< std::string > &list,
                           const PRV_UINT32 &selected,
                           wxChoice *choiceBox );
  rgb wxColourToRGB( wxColour colour ) ;
  wxColour RGBTowxColour( rgb colour );
};

#endif
  // _PREFERENCESDIALOG_H_
