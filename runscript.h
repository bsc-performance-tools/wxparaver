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

#ifndef _RUNSCRIPT_H_
#define _RUNSCRIPT_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/filepicker.h"
#include "wx/statline.h"
#include "wx/html/htmlwin.h"
////@end includes
#include <string>

#include <wx/process.h>

class RunScript;

class RunningProcess : public wxProcess
{
  public:
    RunningProcess( RunScript *whichParent, const wxString& whichCommand )
      : wxProcess( (wxDialog *)whichParent ), command( whichCommand )
    {    
      parent = whichParent;
      Redirect();
    }    

    virtual void OnTerminate( int piwxChoiced, int status );
    virtual bool HasInput();

  protected:
    RunScript *parent;
    wxString command;
};


/*!
 * Forward declarations
 */

////@begin forward declarations
class wxFilePickerCtrl;
class wxBoxSizer;
class wxHtmlWindow;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_RUN_APPLICATION 10110
#define ID_CHOICE_APPLICATION 10200
#define ID_BUTTON_EDIT_APPLICATION 10204
#define ID_FILEPICKER_TRACE 10201
#define ID_TEXTCTRL_DEFAULT_PARAMETERS 10205
#define ID_FILEPICKER_DIMEMAS_CFG 10000
#define ID_BUTTON_DIMEMAS_GUI 10210
#define ID_TEXTCTRL_OUTPUT_TRACE 10001
#define ID_CHECKBOX_DIMEMAS_REUSE 10209
#define ID_TEXTCTRL_STATS_OUTPUT_NAME 10211
#define ID_CHECKBOX_STATS_SHOW_BURSTS 10212
#define ID_CHECKBOX_STATS_SHOW_COMMS_HISTOGRAM 10213
#define ID_CHECKBOX_STATS_ONLYGENERATEDATFILE 10214
#define ID_CHECKBOX_STATS_EXCLUSIVE_TIMES 10215
#define ID_FILECTRL_CLUSTERING_XML 10220
#define ID_CHECKBOX_CLUSTERING_SEMVAL_AS_CLUSTDIMENSION 10219
#define wxID_LABELCOMMANDPREVIEW 10091
#define ID_BUTTON_HELP_SCRIPT 10207
#define ID_BUTTON_RUN 10203
#define ID_BUTTON_CLEAR_LOG 10202
#define ID_LISTBOX_RUN_LOG 10199
#define ID_BUTTON_EXIT 10206
#define SYMBOL_RUNSCRIPT_STYLE wxCAPTION|wxRESIZE_BORDER|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_RUNSCRIPT_TITLE _("Run Application")
#define SYMBOL_RUNSCRIPT_IDNAME ID_RUN_APPLICATION
#define SYMBOL_RUNSCRIPT_SIZE wxSize(600, 600)
#define SYMBOL_RUNSCRIPT_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * RunScript class declaration
 */

class RunScript: public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( RunScript )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  RunScript();
  RunScript( wxWindow* parent, wxWindowID id = SYMBOL_RUNSCRIPT_IDNAME, const wxString& caption = SYMBOL_RUNSCRIPT_TITLE, const wxPoint& pos = SYMBOL_RUNSCRIPT_POSITION, const wxSize& size = SYMBOL_RUNSCRIPT_SIZE, long style = SYMBOL_RUNSCRIPT_STYLE );

  RunScript( wxWindow* parent,
             wxString whichTrace,
             wxString whichClusteringCSV,
             wxWindowID id = SYMBOL_RUNSCRIPT_IDNAME, const wxString& caption = SYMBOL_RUNSCRIPT_TITLE, const wxPoint& pos = SYMBOL_RUNSCRIPT_POSITION, const wxSize& size = SYMBOL_RUNSCRIPT_SIZE, long style = SYMBOL_RUNSCRIPT_STYLE );


  RunScript( wxWindow* parent,
             //wxString whichApp,
             wxString whichTrace,
             //wxString whichCommand,
             //bool runNow,
             wxWindowID id = SYMBOL_RUNSCRIPT_IDNAME, const wxString& caption = SYMBOL_RUNSCRIPT_TITLE, const wxPoint& pos = SYMBOL_RUNSCRIPT_POSITION, const wxSize& size = SYMBOL_RUNSCRIPT_SIZE, long style = SYMBOL_RUNSCRIPT_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_RUNSCRIPT_IDNAME, const wxString& caption = SYMBOL_RUNSCRIPT_TITLE, const wxPoint& pos = SYMBOL_RUNSCRIPT_POSITION, const wxSize& size = SYMBOL_RUNSCRIPT_SIZE, long style = SYMBOL_RUNSCRIPT_STYLE );

  /// Destructor
  ~RunScript();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin RunScript event handler declarations

  /// wxEVT_IDLE event handler for ID_RUN_APPLICATION
  void OnIdle( wxIdleEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_APPLICATION
  void OnChoiceApplicationSelected( wxCommandEvent& event );

  /// wxEVT_FILEPICKER_CHANGED event handler for ID_FILEPICKER_TRACE
  void OnFilepickerTraceFilePickerChanged( wxFileDirPickerEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DIMEMAS_GUI
  void OnButtonDimemasGuiClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_BUTTON_DIMEMAS_GUI
  void OnButtonDimemasGuiUpdate( wxUpdateUIEvent& event );

  /// wxEVT_UPDATE_UI event handler for wxID_LABELCOMMANDPREVIEW
  void OnLabelcommandpreviewUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_RUN
  void OnButtonRunClick( wxCommandEvent& event );

  /// wxEVT_UPDATE_UI event handler for ID_BUTTON_RUN
  void OnButtonRunUpdate( wxUpdateUIEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CLEAR_LOG
  void OnButtonClearLogClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_HTML_LINK_CLICKED event handler for ID_LISTBOX_RUN_LOG
  void OnListboxRunLogLinkClicked( wxHtmlLinkEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_EXIT
  void OnButtonExitClick( wxCommandEvent& event );

////@end RunScript event handler declarations

////@begin RunScript member function declarations

  RunningProcess * GetMyProcess() const { return myProcess ; }
  void SetMyProcess(RunningProcess * value) { myProcess = value ; }

  wxString GetParaverBin() const { return paraverBin ; }
  void SetParaverBin(wxString value) { paraverBin = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end RunScript member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  void OnProcessTerminated();

  void AppendToLog( wxString msg );
  
  wxString GetCommandString();
  
////@begin RunScript member variables
  wxChoice* choiceApplication;
  wxButton* buttonEditApplication;
  wxFilePickerCtrl* filePickerTrace;
  wxBoxSizer* boxSizerParameters;
  wxStaticText* labelTextCtrlDefaultParameters;
  wxTextCtrl* textCtrlDefaultParameters;
  wxBoxSizer* dimemasSection;
  wxStaticText* labelFilePickerDimemasCFG;
  wxFilePickerCtrl* filePickerDimemasCFG;
  wxBitmapButton* buttonDimemasGUI;
  wxStaticText* labelTextCtrlOutputTrace;
  wxTextCtrl* textCtrlOutputTrace;
  wxCheckBox* checkBoxReuseDimemasTrace;
  wxBoxSizer* statsSection;
  wxStaticText* statsLabelTextCtrlOutputName;
  wxTextCtrl* statsTextCtrlOutputName;
  wxCheckBox* statsCheckBoxShowBurstsHistogram;
  wxCheckBox* statsCheckBoxShowCommsHistogram;
  wxCheckBox* statsCheckBoxOnlyDatFile;
  wxCheckBox* statsCheckBoxExclusiveTimes;
  wxBoxSizer* clusteringSection;
  wxFilePickerCtrl* filePickerClusteringXML;
  wxCheckBox* checkBoxClusteringCSVValueAsDimension;
  wxBoxSizer* foldingSection;
  wxTextCtrl* labelCommandPreview;
  wxButton* buttonHelpScript;
  wxButton* buttonRun;
  wxButton* buttonClearLog;
  wxHtmlWindow* listboxRunLog;
  wxButton* buttonExit;
private:
  RunningProcess * myProcess;
  wxString paraverBin;
////@end RunScript member variables

  wxString clusteringCSV;
  bool helpOption;
  wxArrayString extensions;

  wxString doubleQuote( const wxString& path );
  
  void adaptWindowToApplicationSelection();
  wxString expandVariables( wxString command );
  wxString insertLinks( wxString rawLine, wxArrayString extensions );
  void runDetachedProcess( wxString command );

  // TODO: This method's been copied from HelpContents; consider write new class
  std::string getHrefFullPath( wxHtmlLinkEvent &event );
  bool matchHrefExtension( wxHtmlLinkEvent &event, const wxString extension );
};

#endif // _RUNSCRIPT_H_



