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

#pragma once



/*!
 * Includes
 */

////@begin includes
#include "filebrowserbutton.h"
#include "wx/notebook.h"
#include "wx/spinctrl.h"
#include "wx/statline.h"
#include "wx/html/htmlwin.h"
////@end includes
#include <wx/filename.h>

#include <string>
#include <map>

#include <wx/process.h>

class RunScript;

#define ID_TIMER_MESSAGE 40001

class RunningProcess : public wxProcess
{
  DECLARE_EVENT_TABLE()

  public:
    RunningProcess( RunScript *whichParent, const wxString& whichCommand )
      : wxProcess( (wxDialog *)whichParent ), command( whichCommand )
    {
      parent = whichParent;
      Redirect();
      msgTimer.SetOwner( this, ID_TIMER_MESSAGE );
      msgTimer.Start( 500 );
    }

    virtual void OnTerminate( int pid, int status );
    virtual bool HasInput();

    virtual void OnTimerMessage( wxTimerEvent& event );

  protected:
    RunScript *parent;
    wxString command;

    wxString outMsg;
    wxString errMsg;

    wxTimer msgTimer;

    static int pidDimemasGUI;
};


/*!
 * Forward declarations
 */

////@begin forward declarations
class FileBrowserButton;
class wxBoxSizer;
class wxSpinCtrl;
class wxStaticLine;
class wxHtmlWindow;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_RUN_APPLICATION 10110
#define ID_CHOICE_APPLICATION 10200
#define ID_BUTTON_EDIT_APPLICATION 10204
#define ID_TEXTCTRL_TRACE 10233
#define ID_BUTTON_TRACE_BROWSER 10234
#define ID_TEXTCTRL_DEFAULT_PARAMETERS 10205
#define ID_TEXTCTRL_DIMEMAS_CFG 10201
#define ID_BUTTON_DIMEMAS_CFG_BROWSER 10235
#define ID_BUTTON_DIMEMAS_GUI 10210
#define ID_TEXTCTRL_OUTPUT_TRACE 10001
#define ID_NOTEBOOK_DIMEMAS 10254
#define ID_SCROLLEDWINDOW_DIMEMAS_MAIN 10256
#define ID_CHECKBOX_DIMEMAS_REUSE 10209
#define ID_CHECKBOX_PRV2DIM_N 10253
#define ID_CHECKBOX_VERBOSE 10252
#define ID_SCROLLEDWINDOW_DIMEMAS_ADVANCED 10255
#define ID_TEXTCTRL_DIMEMAS_BANDWIDTH 10257
#define ID_TEXTCTRL_DIMEMAS_LATENCY 10004
#define ID_PANEL_DUMMY 10264
#define ID_RADIOBUTTON_DIMEMAS_DEFAULT_TASKS_MAPPING 10258
#define ID_RADIOBUTTON_DIMEMAS_FILL_NODES 10263
#define ID_RADIOBUTTON_DIMEMAS_INTERLEAVED 10262
#define ID_RADIOBUTTON_DIMEMAS_TASKS_PER_NODE 10260
#define ID_TEXTCTRL_DIMEMAS_TASKS_PER_NODE 10259
#define ID_TEXTCTRL_STATS_OUTPUT_NAME 10211
#define ID_CHECKBOX_STATS_SHOW_BURSTS 10212
#define ID_CHECKBOX_STATS_SHOW_COMMS_HISTOGRAM 10213
#define ID_CHECKBOX_STATS_ONLYGENERATEDATFILE 10214
#define ID_CHECKBOX_STATS_EXCLUSIVE_TIMES 10215
#define ID_TEXTCTRL_CLUSTERING_XML 10236
#define ID_BUTTON_CLUSTERING_XML 10237
#define ID_BITMAPBUTTON_CLUSTERING_XML 10106
#define ID_TEXTCTRL 10009
#define ID_CHECKBOX_CLUSTERING_USE_SEMANTIC_WINDOW 10003
#define ID_CHECKBOX_CLUSTERING_SEMVAL_AS_CLUSTDIMENSION 10219
#define ID_CHECKBOX_CLUSTERING_NORMALIZE 10002
#define ID_CHECKBOX_CLUSTERING_NUMBER_OF_SAMPLES 10007
#define ID_TEXTCTRL_CLUSTERING_NUMBER_OF_SAMPLES 10008
#define ID_CHECKBOX_CLUSTERING_GENERATE_SEQUENCES 10000
#define ID_RADIOBUTTON_CLUSTERING_GEN_SEQ_NUMBERED 10261
#define ID_RADIOBUTTON_CLUSTERING_GEN_SEQ_FASTA 10265
#define ID_RADIOBUTTON_CLUSTERING_XMLDEFINED 10221
#define ID_RADIOBUTTON_CLUSTERING_DBSCAN 10222
#define ID_RADIOBUTTON_CLUSTERING_REFINEMENT 10223
#define ID_TEXTCTRL_CLUSTERING_DBSCAN_EPSILON 10224
#define ID_TEXTCTRL_DBSCAN_MIN_POINTS 10225
#define ID_CHECKBOX_CLUSTERING_REFINEMENT_PRINT_DATA 10226
#define ID_CHECKBOX_CLUSTERING_REFINEMENT_TUNE 10227
#define ID_TEXTCTRL_CLUSTERING_REFINEMENT_EPSILON_MIN 10228
#define ID_TEXTCTRL_CLUSTERING_REFINEMENT_EPSILON_MAX 10229
#define ID_TEXTCTRL_CLUSTERING_REFINEMENT_STEPS 10230
#define ID_TEXTCTRL_CLUSTERING_REFINEMENT_MIN_POINTS 10231
#define ID_CHECKBOX_FOLDING_ONLY 10005
#define ID_CHECKBOX_FOLDING_REUSE_FILES 10006
#define ID_CHECKBOX_FOLDING_USE_SEMANTIC_VALUE 10153
#define ID_COMBOBOX_FOLDING_MODEL 10284
#define ID_TEXTCTRL_PROFET_OUTPUT_TRACE 10010
#define ID_TEXTCTRL_PROFET_CONFIG_FILE 10011
#define ID_BUTTON_PROFET_CONFIG_FILE 10012
#define ID_RADIOBUTTON_PROFET_BY_MEMORY_CONTROLLER 10013
#define ID_RADIOBUTTON_PROFET_BY_SOCKET 10014
#define wxID_LABELCOMMANDPREVIEW 10091
#define ID_BUTTON_HELP_SCRIPT 10207
#define ID_BUTTON_RUN 10203
#define ID_BUTTON_KILL 10232
#define ID_BUTTON_CLEAR_LOG 10202
#define ID_LISTBOX_RUN_LOG 10199
#define ID_BUTTON_EXIT 10206
#define SYMBOL_RUNSCRIPT_STYLE wxCAPTION|wxRESIZE_BORDER|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_RUNSCRIPT_TITLE _("Run Application")
#define SYMBOL_RUNSCRIPT_IDNAME ID_RUN_APPLICATION
#define SYMBOL_RUNSCRIPT_SIZE wxSize(600, -1)
#define SYMBOL_RUNSCRIPT_POSITION wxDefaultPosition
////@end control identifiers

enum class TExternalApp
{
  DEFAULT = -1,

  // --- Called through choice selector widget --- 
  DIMEMAS_WRAPPER,     // Dimemas      selected in choice widget
  STATS_WRAPPER,       // Stats        selected in choice widget
  CLUSTERING,          // Clustering   selected in choice widget
  FOLDING,             // Folding      selected in choice widget
  PROFET,
                        // <-- add new apps here at most
  USER_COMMAND,        // User command selected in choice widget

  // --- Called by different widget ---
  DIMEMAS_GUI,         // DimemasGui   invoked through button
  STATS                // stats binary invoked by help
};

/*!
 * RunScript class declaration
 */

enum class TEnvironmentVar
{
  PATH = 0,
  PARAVER_HOME,
  DIMEMAS_HOME
};

enum class TTagPosition
{
  PREFIX,
  SUFFIX
};

struct TOutputLink
{
  std::string tag;
  TTagPosition position;
  std::function<bool(const wxString&, wxString&, wxString& )> makeLink;
};


class RunScript: public wxDialog
{
  DECLARE_DYNAMIC_CLASS( RunScript )
  DECLARE_EVENT_TABLE()

public:

  /// Constructors
  RunScript();
  RunScript( wxWindow* parent,
             wxWindowID id = SYMBOL_RUNSCRIPT_IDNAME, const wxString& caption = SYMBOL_RUNSCRIPT_TITLE, const wxPoint& pos = SYMBOL_RUNSCRIPT_POSITION, const wxSize& size = SYMBOL_RUNSCRIPT_SIZE, long style = SYMBOL_RUNSCRIPT_STYLE );
  RunScript( wxWindow* parent,
             wxString whichTrace,
             wxWindowID id = SYMBOL_RUNSCRIPT_IDNAME, const wxString& caption = SYMBOL_RUNSCRIPT_TITLE, const wxPoint& pos = SYMBOL_RUNSCRIPT_POSITION, const wxSize& size = SYMBOL_RUNSCRIPT_SIZE, long style = SYMBOL_RUNSCRIPT_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_RUNSCRIPT_IDNAME, const wxString& caption = SYMBOL_RUNSCRIPT_TITLE, const wxPoint& pos = SYMBOL_RUNSCRIPT_POSITION, const wxSize& size = SYMBOL_RUNSCRIPT_SIZE, long style = SYMBOL_RUNSCRIPT_STYLE );

  /// Destructor
  ~RunScript();

  /// Initialises member variables
  void InitOutputLinks();
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin RunScript event handler declarations

  /// wxEVT_CLOSE_WINDOW event handler for ID_RUN_APPLICATION
  void OnCloseWindow( wxCloseEvent& event );

  /// wxEVT_IDLE event handler for ID_RUN_APPLICATION
  void OnIdle( wxIdleEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_APPLICATION
  void OnChoiceApplicationSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_TRACE
  void OnTextctrlTraceTextUpdated( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DIMEMAS_GUI
  void OnButtonDimemasGuiClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_BUTTON_DIMEMAS_GUI
  void OnButtonDimemasGuiUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON_CLUSTERING_XML
  void OnBitmapbuttonClusteringXmlClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_BITMAPBUTTON_CLUSTERING_XML
  void OnBitmapbuttonClusteringXmlUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_CHECKBOX_CLUSTERING_SEMVAL_AS_CLUSTDIMENSION
  void OnCheckboxClusteringSemvalAsClustdimensionUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_CHECKBOX_CLUSTERING_NORMALIZE
  void OnCheckboxClusteringNormalizeUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_TEXTCTRL_CLUSTERING_NUMBER_OF_SAMPLES
  void OnTextctrlClusteringNumberOfSamplesUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_CHECKBOX_CLUSTERING_GENERATE_SEQUENCES
  void OnCheckboxClusteringGenerateSequencesUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_CLUSTERING_XMLDEFINED
  void OnRadiobuttonClusteringXmldefinedSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_CLUSTERING_DBSCAN
  void OnRadiobuttonClusteringDbscanSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_CLUSTERING_REFINEMENT
  void OnRadiobuttonClusteringRefinementSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_CLUSTERING_REFINEMENT_TUNE
  void OnCheckboxClusteringRefinementTuneClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_CHECKBOX_FOLDING_USE_SEMANTIC_VALUE
  void OnCheckboxFoldingUseSemanticValueUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_PROFET_BY_MEMORY_CONTROLLER
  void OnRadiobuttonProfetByMemoryControllerSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_PROFET_BY_SOCKET
  void OnRadiobuttonProfetBySocketSelected( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for wxID_LABELCOMMANDPREVIEW
  void OnLabelcommandpreviewUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_RUN
  void OnButtonRunClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_BUTTON_RUN
  void OnButtonRunUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_KILL
  void OnButtonKillClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_BUTTON_KILL
  void OnButtonKillUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CLEAR_LOG
  void OnButtonClearLogClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_HTML_LINK_CLICKED event handler for ID_LISTBOX_RUN_LOG
  void OnListboxRunLogLinkClicked( wxHtmlLinkEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_EXIT
  void OnButtonExitClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_BUTTON_EXIT
  void OnButtonExitUpdate( wxUpdateUIEvent& event );

////@end RunScript event handler declarations

////@begin RunScript member function declarations

  RunningProcess * GetMyProcess() const { return myProcess ; }
  void SetMyProcess(RunningProcess * value) { myProcess = value ; }

  long GetMyProcessPid() const { return myProcessPid ; }
  void SetMyProcessPid(long value) { myProcessPid = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end RunScript member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  void OnProcessTerminated( int pid );

  void AppendToLog( wxString msg, bool formatOutput = true );

  void setTrace( wxString whichTrace );

  void setDimemas();
  void setStats();
  void setClustering( wxString whichClusteringCSV );
  void setFolding( wxString whichFoldingCSV );
  void setProfet();
  void setUserCommand();

  void closeWindow();
  void killRunningProcess( std::function<void(const wxString&)> messageLog );

  
////@begin RunScript member variables
  wxChoice* choiceApplication;
  wxButton* buttonEditApplication;
  wxTextCtrl* textCtrlTrace;
  FileBrowserButton* fileBrowserButtonTrace;
  wxBoxSizer* boxSizerParameters;
  wxStaticText* labelTextCtrlDefaultParameters;
  wxTextCtrl* textCtrlDefaultParameters;
  wxBoxSizer* dimemasSection;
  wxStaticText* labelFilePickerDimemasCFG;
  wxTextCtrl* textCtrlDimemasCFG;
  FileBrowserButton* fileBrowserButtonDimemasCFG;
  wxBitmapButton* buttonDimemasGUI;
  wxStaticText* labelTextCtrlOutputTrace;
  wxTextCtrl* textCtrlOutputTrace;
  wxCheckBox* checkBoxReuseDimemasTrace;
  wxCheckBox* checkBoxDontTranslateIdleStates;
  wxCheckBox* checkBoxDimemasVerbose;
  wxTextCtrl* textCtrlDimemasBandwidth;
  wxTextCtrl* textCtrlDimemasLatency;
  wxRadioButton* radioButtonDimemasDefaultTasksMapping;
  wxRadioButton* radioButtonDimemasFillNodes;
  wxRadioButton* radioButtonDimemasInterleaved;
  wxRadioButton* radioButtonDimemasTasksPerNode;
  wxSpinCtrl* spinCtrlDimemasTasksPerNode;
  wxBoxSizer* statsSection;
  wxStaticText* statsLabelTextCtrlOutputName;
  wxTextCtrl* statsTextCtrlOutputName;
  wxCheckBox* statsCheckBoxShowBurstsHistogram;
  wxCheckBox* statsCheckBoxShowCommsHistogram;
  wxCheckBox* statsCheckBoxOnlyDatFile;
  wxCheckBox* statsCheckBoxExclusiveTimes;
  wxBoxSizer* clusteringSection;
  wxTextCtrl* textCtrlClusteringXML;
  FileBrowserButton* fileBrowserButtonClusteringXML;
  wxBitmapButton* buttonClusteringXML;
  wxTextCtrl* textCtrlClusteringOutputTrace;
  wxCheckBox* checkBoxClusteringUseSemanticWindow;
  wxCheckBox* checkBoxClusteringCSVValueAsDimension;
  wxCheckBox* checkBoxClusteringNormalize;
  wxCheckBox* checkBoxClusteringNumberOfSamples;
  wxTextCtrl* clusteringTextBoxNumberOfSamples;
  wxCheckBox* checkBoxClusteringGenerateSeq;
  wxRadioButton* clusteringRadioGenerateSeqNumbered;
  wxRadioButton* clusteringRadioGenerateSeqFASTA;
  wxStaticBox* clusteringSizerAlgorithm;
  wxRadioButton* clusteringRadioXMLDefined;
  wxRadioButton* clusteringRadioDBScan;
  wxRadioButton* clusteringRadioRefinement;
  wxStaticLine* clusteringAlgorithmLineSeparator;
  wxBoxSizer* clusteringSizerDBScan;
  wxTextCtrl* clusteringTextBoxDBScanEpsilon;
  wxSpinCtrl* clusteringTextBoxDBScanMinPoints;
  wxBoxSizer* clusteringSizerRefinement;
  wxCheckBox* clusteringCheckBoxRefinementPrintData;
  wxCheckBox* clusteringCheckBoxRefinementTune;
  wxStaticText* clusteringLabelRefinementEpsilon;
  wxStaticText* clusteringLabelRefinementEpsilonMin;
  wxTextCtrl* clusteringTextBoxRefinementEpsilonMin;
  wxStaticText* clusteringLabelRefinementEpsilonMax;
  wxTextCtrl* clusteringTextBoxRefinementEpsilonMax;
  wxStaticText* clusteringLabelRefinementSteps;
  wxSpinCtrl* clusteringTextBoxRefinementSteps;
  wxStaticText* clusteringLabelRefinementMinPoints;
  wxSpinCtrl* clusteringTextBoxRefinementMinPoints;
  wxBoxSizer* foldingSection;
  wxCheckBox* checkboxFoldingOnly;
  wxCheckBox* checkboxFoldingReuseFiles;
  wxCheckBox* checkboxFoldingUseSemanticValues;
  wxComboBox* comboboxFoldingModel;
  wxBoxSizer* profetSection;
  wxTextCtrl* textCtrlProfetOutputTrace;
  wxTextCtrl* textCtrlProfetCFG;
  FileBrowserButton* fileBrowserButtonProfetCFG;
  wxRadioButton* radioButtonProfetByMemoryController;
  wxRadioButton* radioButtonProfetBySocket;
  wxTextCtrl* labelCommandPreview;
  wxButton* buttonHelpScript;
  wxButton* buttonRun;
  wxButton* buttonKill;
  wxButton* buttonClearLog;
  wxHtmlWindow* listboxRunLog;
  wxButton* buttonExit;
private:
  RunningProcess * myProcess;
  long myProcessPid;
////@end RunScript member variables
  int pidDimemasGUI;
/*
public:
  int executionStatus;

private:
*/

  static wxString clusteringXML;

  TExternalApp currentApp;

  // Application list: labels and names
  std::map< TExternalApp, wxString > applicationLabel;
  std::map< TExternalApp, wxString > application;

  using TMakeLinkFunction = std::function< bool( const wxString&, const wxString&, wxString&, wxString& ) >;
  std::map< TExternalApp, TMakeLinkFunction > applicationLinkMaker;
  TMakeLinkFunction defaultLinkMaker;
  
  std::map< int, bool > appIsFound;

  // extensions to detect in log
  wxArrayString extensions;

  std::vector< TOutputLink > outputLinks;

  // Tags to detect times
  wxString iterationTag;
  wxString punctualTimeTag;
  wxString rangeTimeTag;
  wxArrayString timeMarkTags;

  // Hidden app parameters
  wxString clusteringCSV;
  wxString foldingCSV;

  bool helpOption; // delete?

  wxString tagFoldingOutputDirectory;
  wxString foldingOutputDirectory;

  std::map< TEnvironmentVar, wxString > environmentVariable;

  wxProgressDialog *progressBar;

  // Selection
  void setApp( TExternalApp whichApp );
  void adaptClusteringAlgorithmParameters();
  void adaptWindowToApplicationSelection();

  // Command check and build
  void ShowWarning( wxString message );
  void ShowWarningUnreachableProgram( wxString program, TEnvironmentVar envVar, bool alsoPrintPath = false );
  wxString getEnvironmentPath( TEnvironmentVar envVar, wxString command = wxString( wxT("") ) );
  wxString doubleQuote( const wxString& path );
  wxString expandVariables( wxString command );

  wxString GetCommand( wxString &command, wxString &parameters, TExternalApp selectedApp = TExternalApp::DEFAULT );
  wxString GetReachableCommand( TExternalApp selectedApp = TExternalApp::DEFAULT ); // adds path to the binary

  // Log related
  bool readFoldingTag( wxString rawLine );
  wxString rawFormat( wxString rawLine );
  bool timeMarkTagFound( wxString rawLine, std::pair< int, wxString >  &tagPosition );
  wxString insertTimeMarkLink( wxString rawLine, std::pair< int, wxString > tagPosition );
  //wxString insertLinks( wxString rawLine, wxArrayString extensions );
  wxString insertLinks( wxString rawLine );

  wxString insertLog( wxString rawLine, wxArrayString extensions );

  // TODO: This method's been copied from HelpContents; consider write new class
  std::string getHrefFullPath( wxHtmlLinkEvent &event, wxString whichSuffixToErase = wxT("") );
  bool matchHrefExtension( wxHtmlLinkEvent &event, const wxString extension ) const;
  bool matchHrefPrefix( wxHtmlLinkEvent &event, const wxString extension ) const;

  // Execution
  //void OnTerminateShellCommand( int pid, int status );

  //bool shellCommand( const wxString& program, const wxString& whichFile );
  bool existCommand( const wxString& program );
  void runCommandAsync( const wxString& program, const wxString& parameter );
  void runDetachedProcess( wxString command, bool checkPidDimemasGUI = false );
};





