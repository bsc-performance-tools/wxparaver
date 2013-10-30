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

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include <wx/utils.h> // wxGetEnv
#include <wx/txtstrm.h>
#include <wx/filefn.h> // wxPathList

#include <vector>
#include <algorithm>

#include "wxparaverapp.h" // paraverMain
#include "runscript.h"


////@begin XPM images
#include "app_edit.xpm"
////@end XPM images


void RunningProcess::OnTerminate( int pid, int status )
{
  while ( HasInput() )
    ;

  parent->OnProcessTerminated();
}


bool RunningProcess::HasInput()
{
  bool hasInput = false;

  if ( IsInputAvailable() )
  {
      wxTextInputStream tis( *GetInputStream() );

      wxString msg;
      // msg << command << _T(" (stdout): ") << tis.ReadLine();
      msg << tis.ReadLine();

/*
      // Try to eliminate multiple lines when '\r' is used
      unsigned int numLines = parent->listboxRunLog->GetCount();
      if ( msg[0] == wxT( '\r' ) && numLines > 0 )
      {
        parent->listboxRunLog->Delete( numLines - 1 );
      }
*/
      parent->AppendToLog( msg );

      hasInput = true;
  }

  if ( IsErrorAvailable() )
  {
      wxTextInputStream tis( *GetErrorStream() );

      wxString msg;
      // msg << command << _T(" (stderr): ") << tis.ReadLine();
      msg << tis.ReadLine();

      parent->AppendToLog( msg );

      hasInput = true;
  }

  return hasInput;
}


/*!
 * RunScript type definition
 */

IMPLEMENT_DYNAMIC_CLASS( RunScript, wxDialog )


/*!
 * RunScript event table definition
 */

BEGIN_EVENT_TABLE( RunScript, wxDialog )

////@begin RunScript event table entries
  EVT_IDLE( RunScript::OnIdle )

  EVT_CHOICE( ID_CHOICE_APPLICATION, RunScript::OnChoiceApplicationSelected )

  EVT_FILEPICKER_CHANGED( ID_FILEPICKER_TRACE, RunScript::OnFilepickerTraceFilePickerChanged )

  EVT_BUTTON( ID_BUTTON_DIMEMAS_GUI, RunScript::OnButtonDimemasGuiClick )
  EVT_UPDATE_UI( ID_BUTTON_DIMEMAS_GUI, RunScript::OnButtonDimemasGuiUpdate )

  EVT_UPDATE_UI( ID_CHECKBOX_CLUSTERING_SEMVAL_AS_CLUSTDIMENSION, RunScript::OnCheckboxClusteringSemvalAsClustdimensionUpdate )

  EVT_UPDATE_UI( wxID_LABELCOMMANDPREVIEW, RunScript::OnLabelcommandpreviewUpdate )

  EVT_BUTTON( ID_BUTTON_RUN, RunScript::OnButtonRunClick )
  EVT_UPDATE_UI( ID_BUTTON_RUN, RunScript::OnButtonRunUpdate )

  EVT_BUTTON( ID_BUTTON_CLEAR_LOG, RunScript::OnButtonClearLogClick )

  EVT_HTML_LINK_CLICKED( ID_LISTBOX_RUN_LOG, RunScript::OnListboxRunLogLinkClicked )

  EVT_BUTTON( ID_BUTTON_EXIT, RunScript::OnButtonExitClick )

////@end RunScript event table entries

END_EVENT_TABLE()


/*!
 * RunScript constructors
 */

RunScript::RunScript()
{
  Init();
}


RunScript::RunScript( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}


RunScript::RunScript( wxWindow* parent,
                      wxString whichTrace,
                      wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
  
  if ( !whichTrace.IsEmpty() )
  {
     filePickerTrace->SetPath( whichTrace );
  }
  
/*
  if ( !whichCommand.IsEmpty() )
  {
    wxString auxCommand = whichCommand;

    int nextPos = auxCommand.Find( wxT( "%20" ) ) ;
    if ( nextPos != wxNOT_FOUND )
    {
      auxCommand.Replace( wxT( "%20" ), wxT( " " ) );
    }
    
    textCtrlDefaultParameters->SetValue( auxCommand );
  }
*/
  helpOption = false; 
}


/*!
 * RunScript creator
 */

bool RunScript::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin RunScript creation
  SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  Centre();
////@end RunScript creation
  return true;
}


/*!
 * RunScript destructor
 */

RunScript::~RunScript()
{
////@begin RunScript destruction
////@end RunScript destruction
}


/*!
 * Member initialisation
 */

void RunScript::Init()
{
////@begin RunScript member initialisation
  myProcess = NULL;
  choiceApplication = NULL;
  buttonEditApplication = NULL;
  filePickerTrace = NULL;
  boxSizerParameters = NULL;
  labelTextCtrlDefaultParameters = NULL;
  textCtrlDefaultParameters = NULL;
  dimemasSection = NULL;
  labelFilePickerDimemasCFG = NULL;
  filePickerDimemasCFG = NULL;
  buttonDimemasGUI = NULL;
  labelTextCtrlOutputTrace = NULL;
  textCtrlOutputTrace = NULL;
  checkBoxReuseDimemasTrace = NULL;
  statsSection = NULL;
  statsLabelTextCtrlOutputName = NULL;
  statsTextCtrlOutputName = NULL;
  statsCheckBoxShowBurstsHistogram = NULL;
  statsCheckBoxShowCommsHistogram = NULL;
  statsCheckBoxOnlyDatFile = NULL;
  statsCheckBoxExclusiveTimes = NULL;
  clusteringSection = NULL;
  filePickerClusteringXML = NULL;
  checkBoxClusteringCSVValueAsDimension = NULL;
  checkBoxClusteringNormalize = NULL;
  foldingSection = NULL;
  labelCommandPreview = NULL;
  buttonHelpScript = NULL;
  buttonRun = NULL;
  buttonClearLog = NULL;
  listboxRunLog = NULL;
  buttonExit = NULL;
////@end RunScript member initialisation

  wxString extensionsAllowed[] = { _(".prv"), _(".prv.gz"), _(".cfg"),
                                   _(".dat"), _(".gnuplot"), _(".xml") };
  extensions = wxArrayString( (size_t)6, extensionsAllowed );

  // Names of environment variables
  environmentVariable[ PATH ]         = wxString( wxT("PATH") );
  environmentVariable[ PARAVER_HOME ] = wxString( wxT("PARAVER_HOME") );
  environmentVariable[ DIMEMAS_HOME ] = wxString( wxT("DIMEMAS_HOME") );
  
  // Labels to construct selector & warning dialogs
  applicationLabel[ DIMEMAS_WRAPPER ]= wxString( wxT("Dimemas") );                          
  applicationLabel[ STATS_WRAPPER ]  = wxString( wxT("Stats") );                             
  applicationLabel[ CLUSTERING ]     = wxString( wxT("Clustering") );                             
  applicationLabel[ FOLDING ]        = wxString( wxT("Folding") );                             
  applicationLabel[ USER_DEFINED ]   = wxString( wxT("User defined") );
  // Following only for warning dialogs
  applicationLabel[ DIMEMAS_GUI ]    = wxString( wxT("DimemasGUI") );
  applicationLabel[ STATS ]          = wxString( wxT("Stats") );

  // application names
  application[ DIMEMAS_WRAPPER ]     = wxString( wxT("dimemas-wrapper.sh") );                             
  application[ STATS_WRAPPER ]       = wxString( wxT("stats-wrapper.sh") );                             
  application[ CLUSTERING ]          = wxString( wxT("BurstClustering") );                             
  application[ FOLDING ]             = wxString( wxT("folding") );                             
  application[ USER_DEFINED ]        = wxString( wxT("") ); // NOT USED                           
  application[ DIMEMAS_GUI ]         = wxString( wxT("DimemasGUI") );
  application[ STATS ]               = wxString( wxT("stats") );
}


wxString RunScript::getEnvironmentPath( TEnvironmentVar envVar, wxString command )
{
  wxString pathToBin;
  wxString tmpEnv;
  wxPathList currentPathEnv;
  wxFileName auxName;
  
  switch( envVar )
  {
    case PATH:
      currentPathEnv.AddEnvList( "PATH" );
      pathToBin = currentPathEnv.FindAbsoluteValidPath( command );
      auxName  = wxFileName( pathToBin );
      pathToBin = auxName.GetPath( wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR );
      
      break;
      
    case PARAVER_HOME:
    case DIMEMAS_HOME:
      
      if ( wxGetEnv( environmentVariable[ envVar ], &tmpEnv ) )
      {
        pathToBin = tmpEnv + wxFileName::GetPathSeparator() + wxString( wxT( "bin" ) ) + wxFileName::GetPathSeparator();
      }
      
      break;
      
    default:
      break;
  }
    
  return pathToBin;
}


/*!
 * Control creation for RunScript
 */

void RunScript::CreateControls()
{    
////@begin RunScript content construction
  RunScript* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer3->Add(itemBoxSizer4, 0, wxGROW|wxTOP|wxBOTTOM, 5);

  wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Application"), wxDefaultPosition, wxDefaultSize, 0 );
  if (RunScript::ShowToolTips())
    itemStaticText5->SetToolTip(_("Select registered application"));
  itemBoxSizer4->Add(itemStaticText5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  wxArrayString choiceApplicationStrings;
  choiceApplication = new wxChoice( itemDialog1, ID_CHOICE_APPLICATION, wxDefaultPosition, wxDefaultSize, choiceApplicationStrings, 0 );
  choiceApplication->SetHelpText(_("Select external application"));
  if (RunScript::ShowToolTips())
    choiceApplication->SetToolTip(_("Select external application"));
  itemBoxSizer4->Add(choiceApplication, 4, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 2);

  buttonEditApplication = new wxButton( itemDialog1, ID_BUTTON_EDIT_APPLICATION, _("Edit..."), wxDefaultPosition, wxDefaultSize, 0 );
  buttonEditApplication->SetHelpText(_("Edit application preferences"));
  if (RunScript::ShowToolTips())
    buttonEditApplication->SetToolTip(_("Edit application preferences"));
  buttonEditApplication->Show(false);
  itemBoxSizer4->Add(buttonEditApplication, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer3->Add(itemBoxSizer8, 0, wxGROW|wxTOP|wxBOTTOM, 5);

  wxStaticText* itemStaticText9 = new wxStaticText( itemDialog1, wxID_STATIC, _("Trace"), wxDefaultPosition, wxDefaultSize, 0 );
  if (RunScript::ShowToolTips())
    itemStaticText9->SetToolTip(_("Select the input trace read by the application"));
  itemBoxSizer8->Add(itemStaticText9, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  filePickerTrace = new wxFilePickerCtrl( itemDialog1, ID_FILEPICKER_TRACE, wxEmptyString, wxEmptyString, _T("Paraver trace (*.prv;*.prv.gz)|*.prv;*.prv.gz|All files (*.*)|*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
  if (RunScript::ShowToolTips())
    filePickerTrace->SetToolTip(_("Select the input trace read by the application"));
  itemBoxSizer8->Add(filePickerTrace, 4, wxGROW|wxLEFT|wxRIGHT, 2);

  boxSizerParameters = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(boxSizerParameters, 0, wxGROW|wxALL, 5);

  labelTextCtrlDefaultParameters = new wxStaticText( itemDialog1, wxID_STATIC, _("Command"), wxDefaultPosition, wxDefaultSize, 0 );
  boxSizerParameters->Add(labelTextCtrlDefaultParameters, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  textCtrlDefaultParameters = new wxTextCtrl( itemDialog1, ID_TEXTCTRL_DEFAULT_PARAMETERS, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (RunScript::ShowToolTips())
    textCtrlDefaultParameters->SetToolTip(_("Parameters passed to the script"));
  boxSizerParameters->Add(textCtrlDefaultParameters, 4, wxGROW|wxLEFT|wxRIGHT, 2);

  dimemasSection = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer2->Add(dimemasSection, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
  dimemasSection->Add(itemBoxSizer15, 0, wxGROW|wxTOP|wxBOTTOM, 2);

  labelFilePickerDimemasCFG = new wxStaticText( itemDialog1, wxID_STATIC, _("Dimemas Cfg"), wxDefaultPosition, wxDefaultSize, 0 );
  if (RunScript::ShowToolTips())
    labelFilePickerDimemasCFG->SetToolTip(_("Select the Dimemas configuration file to apply"));
  itemBoxSizer15->Add(labelFilePickerDimemasCFG, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer15->Add(itemBoxSizer17, 4, wxALIGN_CENTER_VERTICAL, 2);

  filePickerDimemasCFG = new wxFilePickerCtrl( itemDialog1, ID_FILEPICKER_DIMEMAS_CFG, wxEmptyString, wxEmptyString, _T("Dimemas configuration file (*.cfg)|*.cfg|All files (*.*)|*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
  if (RunScript::ShowToolTips())
    filePickerDimemasCFG->SetToolTip(_("Select the Dimemas configuration file to apply"));
  itemBoxSizer17->Add(filePickerDimemasCFG, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 0);

  buttonDimemasGUI = new wxBitmapButton( itemDialog1, ID_BUTTON_DIMEMAS_GUI, itemDialog1->GetBitmapResource(wxT("app_edit.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  if (RunScript::ShowToolTips())
    buttonDimemasGUI->SetToolTip(_("Edit Dimemas Configuration File using DimemasGUI."));
  itemBoxSizer17->Add(buttonDimemasGUI, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 2);

  wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
  dimemasSection->Add(itemBoxSizer20, 0, wxGROW|wxTOP|wxBOTTOM, 2);

  labelTextCtrlOutputTrace = new wxStaticText( itemDialog1, wxID_STATIC, _("Output Trace"), wxDefaultPosition, wxDefaultSize, 0 );
  if (RunScript::ShowToolTips())
    labelTextCtrlOutputTrace->SetToolTip(_("Write the name given to the output trace; if missing, suffix '.prv' will be appended"));
  itemBoxSizer20->Add(labelTextCtrlOutputTrace, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  textCtrlOutputTrace = new wxTextCtrl( itemDialog1, ID_TEXTCTRL_OUTPUT_TRACE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (RunScript::ShowToolTips())
    textCtrlOutputTrace->SetToolTip(_("Write the name given to the output trace; if missing, suffix '.prv' will be appended"));
  itemBoxSizer20->Add(textCtrlOutputTrace, 4, wxGROW|wxLEFT|wxRIGHT, 2);

  wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
  dimemasSection->Add(itemBoxSizer23, 0, wxGROW|wxTOP|wxBOTTOM, 2);

  itemBoxSizer23->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  checkBoxReuseDimemasTrace = new wxCheckBox( itemDialog1, ID_CHECKBOX_DIMEMAS_REUSE, _("Reuse Dimemas trace if previously generated"), wxDefaultPosition, wxDefaultSize, 0 );
  checkBoxReuseDimemasTrace->SetValue(true);
  if (RunScript::ShowToolTips())
    checkBoxReuseDimemasTrace->SetToolTip(_("Check this if you want to run many simulations varying only the Dimemas parametrization but the translated trace (obtained from given .prv trace)."));
  itemBoxSizer23->Add(checkBoxReuseDimemasTrace, 4, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 2);

  statsSection = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer2->Add(statsSection, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer27 = new wxBoxSizer(wxHORIZONTAL);
  statsSection->Add(itemBoxSizer27, 0, wxGROW|wxALL, 2);

  statsLabelTextCtrlOutputName = new wxStaticText( itemDialog1, wxID_STATIC, _("Output Prefix"), wxDefaultPosition, wxDefaultSize, 0 );
  if (RunScript::ShowToolTips())
    statsLabelTextCtrlOutputName->SetToolTip(_("Name given to resulting .dat and .gnuplot files."));
  statsLabelTextCtrlOutputName->SetName(_T("O"));
  itemBoxSizer27->Add(statsLabelTextCtrlOutputName, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  statsTextCtrlOutputName = new wxTextCtrl( itemDialog1, ID_TEXTCTRL_STATS_OUTPUT_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  if (RunScript::ShowToolTips())
    statsTextCtrlOutputName->SetToolTip(_("Name given to resulting .dat and .gnuplot files."));
  itemBoxSizer27->Add(statsTextCtrlOutputName, 4, wxGROW|wxALL, 2);

  wxBoxSizer* itemBoxSizer30 = new wxBoxSizer(wxHORIZONTAL);
  statsSection->Add(itemBoxSizer30, 0, wxGROW|wxALL, 2);

  statsCheckBoxShowBurstsHistogram = new wxCheckBox( itemDialog1, ID_CHECKBOX_STATS_SHOW_BURSTS, _("Generate bursts histogram"), wxDefaultPosition, wxDefaultSize, 0 );
  statsCheckBoxShowBurstsHistogram->SetValue(false);
  if (RunScript::ShowToolTips())
    statsCheckBoxShowBurstsHistogram->SetToolTip(_("Generat _bursts histogram files."));
  itemBoxSizer30->Add(statsCheckBoxShowBurstsHistogram, 1, wxGROW|wxALL, 2);

  statsCheckBoxShowCommsHistogram = new wxCheckBox( itemDialog1, ID_CHECKBOX_STATS_SHOW_COMMS_HISTOGRAM, _("Generate communications histogram"), wxDefaultPosition, wxDefaultSize, 0 );
  statsCheckBoxShowCommsHistogram->SetValue(false);
  if (RunScript::ShowToolTips())
    statsCheckBoxShowCommsHistogram->SetToolTip(_("Generate communications histogram"));
  itemBoxSizer30->Add(statsCheckBoxShowCommsHistogram, 1, wxGROW|wxALL, 2);

  wxBoxSizer* itemBoxSizer33 = new wxBoxSizer(wxHORIZONTAL);
  statsSection->Add(itemBoxSizer33, 0, wxGROW|wxALL, 2);

  statsCheckBoxOnlyDatFile = new wxCheckBox( itemDialog1, ID_CHECKBOX_STATS_ONLYGENERATEDATFILE, _("Only generate .dat file"), wxDefaultPosition, wxDefaultSize, 0 );
  statsCheckBoxOnlyDatFile->SetValue(false);
  statsCheckBoxOnlyDatFile->SetHelpText(_("If checked if won't generate .gnuplot, only .dat. If unchecked it generates both."));
  if (RunScript::ShowToolTips())
    statsCheckBoxOnlyDatFile->SetToolTip(_("If checked if won't generate .gnuplot, only .dat. If unchecked it generates both."));
  itemBoxSizer33->Add(statsCheckBoxOnlyDatFile, 1, wxGROW|wxALL, 2);

  statsCheckBoxExclusiveTimes = new wxCheckBox( itemDialog1, ID_CHECKBOX_STATS_EXCLUSIVE_TIMES, _("Exclusive instead of inclusive times"), wxDefaultPosition, wxDefaultSize, 0 );
  statsCheckBoxExclusiveTimes->SetValue(false);
  if (RunScript::ShowToolTips())
    statsCheckBoxExclusiveTimes->SetToolTip(_("Changes how the times of the routine calls are calculated; if unchecked, inclusive times are calculated; if checked, exclusive times are calculated."));
  itemBoxSizer33->Add(statsCheckBoxExclusiveTimes, 1, wxGROW|wxALL, 2);

  clusteringSection = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer2->Add(clusteringSection, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer37 = new wxBoxSizer(wxHORIZONTAL);
  clusteringSection->Add(itemBoxSizer37, 0, wxGROW|wxALL, 2);

  wxStaticText* itemStaticText38 = new wxStaticText( itemDialog1, wxID_STATIC, _("XML File"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer37->Add(itemStaticText38, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  filePickerClusteringXML = new wxFilePickerCtrl( itemDialog1, ID_FILECTRL_CLUSTERING_XML, wxEmptyString, wxEmptyString, _T("Clustering configuration file (*.xml)|*.xml|All files (*.*)|*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
  itemBoxSizer37->Add(filePickerClusteringXML, 4, wxGROW|wxALL, 2);

  wxBoxSizer* itemBoxSizer40 = new wxBoxSizer(wxHORIZONTAL);
  clusteringSection->Add(itemBoxSizer40, 0, wxGROW|wxALL, 2);

  itemBoxSizer40->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  checkBoxClusteringCSVValueAsDimension = new wxCheckBox( itemDialog1, ID_CHECKBOX_CLUSTERING_SEMVAL_AS_CLUSTDIMENSION, _("CSV Semantic values as clustering dimensions"), wxDefaultPosition, wxDefaultSize, 0 );
  checkBoxClusteringCSVValueAsDimension->SetValue(false);
  itemBoxSizer40->Add(checkBoxClusteringCSVValueAsDimension, 4, wxGROW|wxRIGHT|wxTOP|wxBOTTOM, 5);

  wxBoxSizer* itemBoxSizer43 = new wxBoxSizer(wxHORIZONTAL);
  clusteringSection->Add(itemBoxSizer43, 0, wxGROW|wxALL, 2);

  itemBoxSizer43->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  checkBoxClusteringNormalize = new wxCheckBox( itemDialog1, ID_CHECKBOX_CLUSTERING_NORMALIZE, _("Apply logarithmic normalization to semantic value"), wxDefaultPosition, wxDefaultSize, 0 );
  checkBoxClusteringNormalize->SetValue(false);
  itemBoxSizer43->Add(checkBoxClusteringNormalize, 4, wxGROW|wxRIGHT|wxTOP|wxBOTTOM, 5);

  foldingSection = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer2->Add(foldingSection, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

  wxStaticLine* itemStaticLine47 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemBoxSizer2->Add(itemStaticLine47, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer48 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer48, 1, wxGROW, 5);

  wxStaticText* itemStaticText49 = new wxStaticText( itemDialog1, wxID_STATIC, _("Preview:"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer48->Add(itemStaticText49, 1, wxALIGN_TOP|wxALL, 5);

  labelCommandPreview = new wxTextCtrl( itemDialog1, wxID_LABELCOMMANDPREVIEW, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
  itemBoxSizer48->Add(labelCommandPreview, 4, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer51 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer51, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT, 5);

  buttonHelpScript = new wxButton( itemDialog1, ID_BUTTON_HELP_SCRIPT, _("Help"), wxDefaultPosition, wxDefaultSize, 0 );
  if (RunScript::ShowToolTips())
    buttonHelpScript->SetToolTip(_("Shows the application '--help' message if available"));
  buttonHelpScript->Show(false);
  itemBoxSizer51->Add(buttonHelpScript, 0, wxGROW|wxALL, 5);

  buttonRun = new wxButton( itemDialog1, ID_BUTTON_RUN, _("Run"), wxDefaultPosition, wxDefaultSize, 0 );
  if (RunScript::ShowToolTips())
    buttonRun->SetToolTip(_("Runs the application"));
  itemBoxSizer51->Add(buttonRun, 0, wxGROW|wxALL, 5);

  buttonClearLog = new wxButton( itemDialog1, ID_BUTTON_CLEAR_LOG, _("Clear Log"), wxDefaultPosition, wxDefaultSize, 0 );
  if (RunScript::ShowToolTips())
    buttonClearLog->SetToolTip(_("Clears accumulated messages"));
  itemBoxSizer51->Add(buttonClearLog, 0, wxGROW|wxALL, 5);

  listboxRunLog = new wxHtmlWindow( itemDialog1, ID_LISTBOX_RUN_LOG, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO|wxHSCROLL|wxVSCROLL|wxALWAYS_SHOW_SB );
  if (RunScript::ShowToolTips())
    listboxRunLog->SetToolTip(_("Execution messages"));
  itemBoxSizer2->Add(listboxRunLog, 3, wxGROW|wxALL, 7);

  wxBoxSizer* itemBoxSizer56 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer56, 0, wxALIGN_RIGHT|wxALL, 5);

  wxBoxSizer* itemBoxSizer57 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer56->Add(itemBoxSizer57, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  buttonExit = new wxButton( itemDialog1, ID_BUTTON_EXIT, _("Exit"), wxDefaultPosition, wxDefaultSize, 0 );
  if (RunScript::ShowToolTips())
    buttonExit->SetToolTip(_("Close window but don't run the selected application."));
  itemBoxSizer57->Add(buttonExit, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

////@end RunScript content construction

  for ( int i = DIMEMAS_WRAPPER; i <= USER_DEFINED; i++ )
  {
    choiceApplication->Append( applicationLabel[ TExternalApp(i) ] );
  }
  
  int appNumber = DIMEMAS_WRAPPER; // Default is 0
  choiceApplication->Select( appNumber ); 
  adaptWindowToApplicationSelection();
}


/*!
 * Should we show tooltips?
 */

bool RunScript::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap RunScript::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin RunScript bitmap retrieval
  wxUnusedVar(name);
  if (name == _T("app_edit.xpm"))
  {
    wxBitmap bitmap(app_edit_xpm);
    return bitmap;
  }
  return wxNullBitmap;
////@end RunScript bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon RunScript::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin RunScript icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end RunScript icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_EXIT
 */
void RunScript::OnButtonExitClick( wxCommandEvent& event )
{
  EndModal( wxID_OK );
}


wxString RunScript::GetCommand( wxString &command, wxString &parameters, TExternalApp selectedApp )
{
  wxFileName tmpFilename;
  wxString tmpPath;
  wxString tmpNameWOExtension;
  wxString tmpParams;
  wxString fullCommand;
  
  command.Clear();
  parameters.Clear();
 
  if ( selectedApp == DEFAULT )
  {
    selectedApp = (TExternalApp)choiceApplication->GetSelection();
  }
 
  switch ( selectedApp )
  {
    case DIMEMAS_WRAPPER:
      command = application[ DIMEMAS_WRAPPER ];

      parameters = doubleQuote( filePickerTrace->GetPath() );      // Source trace
      parameters += wxT( " " ) + doubleQuote( filePickerDimemasCFG->GetPath() ); // Dimemas cfg
      parameters += wxT( " " ) + doubleQuote( textCtrlOutputTrace->GetValue() ); // Final trace
      if ( checkBoxReuseDimemasTrace->IsChecked() )
      {
        parameters += wxT( " 1" );
      }
      else
      {
        parameters += wxT( " 0" );
      }
      parameters += wxT( " " ) + expandVariables( textCtrlDefaultParameters->GetValue() ); // Extra params
      
      if ( textCtrlDefaultParameters->GetValue() == wxString( wxT( "--help" ) ))
      {
        helpOption = true;
      }

      break;
    
    case DIMEMAS_GUI:
      command = application[ DIMEMAS_GUI ];
    
      break;
      
    case STATS_WRAPPER:
    case STATS:
    
      if ( textCtrlDefaultParameters->GetValue() == wxString( wxT( "--help" ) ))
      {
        command  = application[ STATS ];
        parameters = textCtrlDefaultParameters->GetValue();
        helpOption = true;
      }
      else
      {
        // TODO: DEFAULT VALUES?
        command  = application[ STATS_WRAPPER ];

        parameters = doubleQuote( filePickerTrace->GetPath() );      // Source trace
        parameters += wxString( wxT( " -o " ) ) + doubleQuote( statsTextCtrlOutputName->GetValue() ); // Final name
        if ( statsCheckBoxShowBurstsHistogram->IsChecked() )
        {
          parameters += wxString( wxT( " -bursts_histo" ) );
        }
        if ( statsCheckBoxShowCommsHistogram->IsChecked() )
        {
          parameters += wxString( wxT( " -comms_histo" ) );
        }
        if ( statsCheckBoxOnlyDatFile->IsChecked() )
        {
          parameters += wxString( wxT( " -only_dat_file" ) );
        }
        if ( statsCheckBoxExclusiveTimes->IsChecked() )
        {
          parameters += wxString( wxT( " -exclusive_times" ) );
        }
        parameters += wxString( wxT( " " ) ) +
                expandVariables( textCtrlDefaultParameters->GetValue() ); // Extra params
      }
      
      break;
      
    case CLUSTERING:
      command = application[ CLUSTERING ];
      
      parameters = wxT( " -s" );
      
      if ( checkBoxClusteringCSVValueAsDimension->IsChecked() )
      {
        parameters += wxT( " -c" );
        if ( checkBoxClusteringNormalize->IsChecked() )
        {
          parameters += wxT( "l" );
        }
      }
      
      parameters += wxT( " -d " ) + doubleQuote( filePickerClusteringXML->GetPath() );
      
      parameters += wxT( " -i " );
      if ( !clusteringCSV.IsEmpty() )
      {
        parameters += doubleQuote( clusteringCSV + wxString( wxT( "," ) ) + filePickerTrace->GetPath() ) ;
      }
      else
      {
        parameters += doubleQuote( filePickerTrace->GetPath() );
      }
      
      parameters += wxT(" -o ");
      tmpFilename = wxFileName( filePickerTrace->GetPath() );
      tmpPath = tmpFilename.GetPath( wxPATH_GET_SEPARATOR );
      tmpNameWOExtension = tmpFilename.GetName();
      parameters += doubleQuote( tmpPath + tmpNameWOExtension + wxString( wxT( ".clustered.prv" )));

      break;
      
    case FOLDING:
      command = application[ FOLDING ];

      parameters = doubleQuote( filePickerTrace->GetPath() );
      parameters += wxT(" ");
      parameters += doubleQuote( foldingCSV );
      
      break;
      
    case USER_DEFINED:
      
      tmpParams = expandVariables( textCtrlDefaultParameters->GetValue() );
      command = tmpParams.BeforeFirst( ' ' );
      parameters = tmpParams.AfterFirst( ' ' );
      
      break;
      
    default:
      // Third kind: registered application doesn't need a wrapper; app has the same name
      command = choiceApplication->GetString( choiceApplication->GetSelection() );
      
      break;
  }
  
  fullCommand = command;
  if ( !command.IsEmpty() )
  { 
    // extend with parameters
    fullCommand += wxString( wxT( " " ) ) + parameters;
  }
  
  return ( fullCommand );
}


void RunScript::ShowWarning( wxString whichMessage )
{
  wxMessageDialog message( this, whichMessage, _( "Warning" ), wxOK | wxICON_EXCLAMATION );
  message.ShowModal();
}


void RunScript::ShowWarningUnreachableProgram( wxString program, TEnvironmentVar envVar, bool alsoPrintPath )
{
  wxString auxMessage;
  
  if ( envVar == PATH && alsoPrintPath )
  {
    alsoPrintPath = false;
  }
  
  auxMessage = wxString( wxT( "Unable to find:" ) );
  auxMessage += wxString( wxT( "\n\n\t" ) ) + program + wxString( wxT( "\n\n" ) );
  auxMessage += wxString( wxT("Please check that the program is reachable through the environment variable " ) );
  if ( alsoPrintPath )
  {
    auxMessage += wxString( wxT( "$PATH or " ) )  + environmentVariable[ envVar ] + wxString( wxT( "." ) );
  }
  auxMessage += wxString( wxT( "$" ) )  + environmentVariable[ envVar ] + wxString( wxT( "." ) );
  
  ShowWarning( auxMessage );
}
  

wxString RunScript::GetReachableCommand( TExternalApp selectedApp )
{
  wxString program, parameters;
  wxString readyCommand;
  wxString pathToProgram;

  //readyCommand.Clear();
  
  wxString candidateCommand = GetCommand( program, parameters, selectedApp );
  if ( candidateCommand.IsEmpty() )
  {
    ShowWarning( wxString( wxT( "Empty command." )) );
  }
  else
  {
    if ( selectedApp == DEFAULT )
    {
      selectedApp = (TExternalApp)choiceApplication->GetSelection();
    } 
   
    switch ( selectedApp )
    {
        
      case DIMEMAS_GUI:
        pathToProgram = getEnvironmentPath( DIMEMAS_HOME );
        if ( !pathToProgram.IsEmpty() )
        {
          readyCommand = doubleQuote( pathToProgram + candidateCommand );
        }
        else
        {
          ShowWarningUnreachableProgram( program, DIMEMAS_HOME );
        }

        break;
        
      case DIMEMAS_WRAPPER:
        pathToProgram = getEnvironmentPath( PATH, program );
        if ( !pathToProgram.IsEmpty() )
        {
          readyCommand =  pathToProgram + candidateCommand;
        }
        else
        {
          pathToProgram = getEnvironmentPath( PARAVER_HOME );
          if ( !pathToProgram.IsEmpty() )
          {
            readyCommand =  pathToProgram + candidateCommand;
          }
          else
          {
            ShowWarningUnreachableProgram( program, PARAVER_HOME, true );
          }
        }
        
        break;
        
      case STATS_WRAPPER:
        pathToProgram = getEnvironmentPath( PATH, program );
        if ( !pathToProgram.IsEmpty() )
        {
          readyCommand =  pathToProgram + candidateCommand;
        }
        else
        {
          pathToProgram = getEnvironmentPath( PARAVER_HOME );
          if ( !pathToProgram.IsEmpty() )
          {
            readyCommand =  pathToProgram + candidateCommand;
          }
          else
          {
            ShowWarningUnreachableProgram( program, PARAVER_HOME, true );
          }
        }
        
        break;
        
      case CLUSTERING:
      case FOLDING:
      case USER_DEFINED:
      default:
        pathToProgram = getEnvironmentPath( PATH, program );
        if ( !pathToProgram.IsEmpty() )
        {
          readyCommand =  pathToProgram + candidateCommand;
        }
        else
        {
          ShowWarningUnreachableProgram( program, PATH );
        }      
        
        break;
    }
  }
  
  return ( readyCommand );
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_RUN
 */
void RunScript::OnButtonRunClick( wxCommandEvent& event )
{
  buttonRun->Enable( false );
  helpOption = false;  

  wxString readyCommand = GetReachableCommand();
  if ( !readyCommand.IsEmpty() )
  {
    myProcess = new RunningProcess( this, readyCommand );
    if( !wxExecute( readyCommand, wxEXEC_ASYNC, myProcess ) )
    {
      OnProcessTerminated();
    }
  }

  buttonRun->Enable( true );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_RUN
 */
void RunScript::OnButtonRunUpdate( wxUpdateUIEvent& event )
{
  // Check parameters
  bool active = ( myProcess == NULL );
          
  buttonRun->Enable( active );
}


void RunScript::OnProcessTerminated()
{
  delete myProcess;
  myProcess = NULL;
}


void RunScript::AppendToLog( wxString msg )
{
  if ( !helpOption )
  {
    msg = insertLinks( msg, extensions );
  }
  
  listboxRunLog->AppendToPage( wxT("<TT>") + msg + wxT("</TT><BR>") );
  int x, y;
  listboxRunLog->GetVirtualSize( &x, &y );
  listboxRunLog->Scroll( -1, y );
  listboxRunLog->Update();
}


/*!
 * wxEVT_IDLE event handler for ID_RUN_APPLICATION
 */

void RunScript::OnIdle( wxIdleEvent& event )
{
  if ( myProcess != NULL && myProcess->HasInput() )
  {
    event.RequestMore();
  }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CLEAR_LOG
 */

void RunScript::OnButtonClearLogClick( wxCommandEvent& event )
{
  listboxRunLog->SetPage( wxT( "" ) );
}


wxString RunScript::doubleQuote( const wxString& path )
{
  return wxString( wxT( "\"" ) ) + path + wxString( wxT( "\"" ) );
}


void RunScript::adaptWindowToApplicationSelection()
{
  int currentChoice = choiceApplication->GetSelection();
  
  switch ( currentChoice )
  {
    case DIMEMAS_WRAPPER:
      labelTextCtrlDefaultParameters->SetLabel( wxT( "Parameters" ) ); 
      textCtrlDefaultParameters->SetToolTip( wxT( "Extra parameters passed to the script\n"
                                                  "%TRACE refers to input trace" ) );
      labelTextCtrlDefaultParameters->Show();
      textCtrlDefaultParameters->Show();
      break;
      
    case STATS_WRAPPER:
      labelTextCtrlDefaultParameters->SetLabel( wxT( "Parameters" ) ); 
      textCtrlDefaultParameters->SetToolTip( wxT( "Extra parameters passed to 'stats'\n"
                                                  "-events_histo[:type1[-type2],...]\n"
                                                  "-thread_calls[:type1[-type2],...]\n" ) );
      labelTextCtrlDefaultParameters->Show();
      textCtrlDefaultParameters->Show();
      break;
      
    case CLUSTERING:
      labelTextCtrlDefaultParameters->Hide();
      textCtrlDefaultParameters->Hide();
      if ( clusteringCSV.IsEmpty() )
      {
        checkBoxClusteringCSVValueAsDimension->Enable( false );
      }
      break;
      
    case FOLDING:
      labelTextCtrlDefaultParameters->Hide();
      textCtrlDefaultParameters->Hide();
      break;
      
    case USER_DEFINED:
      labelTextCtrlDefaultParameters->SetLabel( wxT( "Command" ) );
      textCtrlDefaultParameters->SetToolTip( wxT( "Command and parameters to execute\n"
                                                  "%TRACE refers to input trace" ) );    
      labelTextCtrlDefaultParameters->Show();
      textCtrlDefaultParameters->Show();
      break;
      
    default:
      break;
  }

  dimemasSection->Show( currentChoice == DIMEMAS_WRAPPER );
  statsSection->Show( currentChoice == STATS_WRAPPER );
  clusteringSection->Show( currentChoice == CLUSTERING );
  foldingSection->Show( currentChoice == FOLDING );

  Layout();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_APPLICATION
 */
void RunScript::OnChoiceApplicationSelected( wxCommandEvent& event )
{
  adaptWindowToApplicationSelection();
}


wxString RunScript::expandVariables( wxString command )
{
  command.Replace( wxT( "%TRACE" ), doubleQuote( filePickerTrace->GetPath() ) );

  return command;
}


struct gthan
{
  bool operator()( const std::pair< int, wxString > &a, const std::pair< int, wxString > &b )
  {   
    return ( a.first > b.first ) ||
           ( a.first == b.first && a.second.Len() > b.second.Len() );
  }   
} greaterThan;


wxString RunScript::insertLinks( wxString rawLine,
                                 wxArrayString extensions )
{
  std::vector< std::pair< int, wxString > > extensionsPositions; // {(4,.cfg),(6,.prv),(6.prv.gz)}
  
  // Detect all the ocurrences of every given extension
  for ( size_t i = 0; i < extensions.Count(); ++i )
  {
    int endLine = rawLine.Len();
    int initSubStr = 0; // Start from the begin
    while ( initSubStr < endLine )
    {
      wxString subStr = rawLine.Mid( initSubStr, endLine - initSubStr );
      int initSuffixPos = subStr.Find( extensions[i] );

      if ( initSuffixPos != wxNOT_FOUND )
      {
        // Remember (position, extension)
        int globalPos = initSubStr + initSuffixPos;

        extensionsPositions.push_back( std::make_pair( globalPos, extensions[i] ) );
        
        // Advance: compute new end of the substring 
        initSubStr = globalPos + extensions[i].Len();
      }
      else
      {
        // No more current extensions; exit loop to try next extension
        initSubStr = endLine;
      }
    }
  }
  
  if ( extensionsPositions.size() == 0 )
  {
    // No match found for any extension!! -> turn spaces to &nsbp for all the line
    rawLine.Replace( wxT( " " ), wxT( "&nbsp;" ) );
  }
  else 
  {  
    // Sort extensionsPosition by
    //   1. position, descending
    //   2. longest suffix first
    // Ex:
    //   Before :{ (4,.cfg), (6,.prv), (25,.prv), (6,.prv.gz) }
    //   After  :{ (25,.prv), (6,.prv.gz), (6,.prv), (4,.cfg) }
   
    std::sort( extensionsPositions.begin(), extensionsPositions.end(), greaterThan );
    
    // Clean extensionsPosition
    //   1. with unique positions, only longest suffix is kept (.prv.gz vs .prv)
    // Ex:
    //   Before :{ (25,.prv), (6,.prv.gz), (6,.prv), (4,.cfg) }
    //   After  :{ (25,.prv), (6,.prv.gz), (4,.cfg) }
    std::vector< std::pair< int, wxString > > auxExtensionsPositions;
    for ( std::vector< std::pair< int, wxString > >::iterator it = extensionsPositions.begin();
            it != extensionsPositions.end(); ++it )
    {
      if ( auxExtensionsPositions.empty() )
      {
        auxExtensionsPositions.push_back( *it );
      }
      else if ( it->first != auxExtensionsPositions.back().first )
      {
        auxExtensionsPositions.push_back( *it );
      }
    }
    
    std::swap( extensionsPositions, auxExtensionsPositions );
    

    // If available, get trace path from wxFilePickerCtrl
    wxString selectedTracePath =
        wxFileName( filePickerTrace->GetPath() ).GetPath( wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);

    // Now, traverse rawLine using extensionPositions.
    wxString auxLine;  // where output line is being built, starting from the tail.
    int endLine = rawLine.Len();
    int endSubStr = endLine;
    int oldEndSubStr = endSubStr;
    size_t i = 0;
    while ( i < extensionsPositions.size() )
    {
      // New end of the substring
      int currentEndSubStr = extensionsPositions[i].first + 
                             extensionsPositions[i].second.Len();
      
      // Anything after it must be nbspaced; for sure it doesn't belong to a link
      wxString trashTail = rawLine.Mid( currentEndSubStr, endSubStr - currentEndSubStr );
      trashTail.Replace( wxT( " " ), wxT( "&nbsp;" ) );
  
      // Advance endSubStr, keeping the old one
      oldEndSubStr = endSubStr;
      endSubStr = currentEndSubStr;
      
      // Find trace candidate
      wxFileName candidateFile;
      int currentPos = 0; // always start from the beginning
      bool candidateFound = false;
      int initSuffixPos = extensionsPositions[i].first;
      wxString candidateName;
      while ( currentPos < initSuffixPos && !candidateFound )
      {
        // Normalize
        candidateName = rawLine.Mid( currentPos, endSubStr - currentPos );
        candidateFile = wxFileName( candidateName );
        candidateFound = ( candidateFile.Normalize() && candidateFile.FileExists() );
    
        if ( !candidateFound )
        {
          candidateFile = wxFileName( candidateName );
          candidateFound =
              ( candidateFile.Normalize( wxPATH_NORM_ALL, selectedTracePath ) &&
                candidateFile.FileExists() );
        }
    
        ++currentPos;
      }

      if ( candidateFound )
      {
        --currentPos;

        wxString linkName = candidateName;
        linkName.Replace( wxT( " " ), wxT( "&nbsp;" ) );
        wxString linkFullPath = candidateFile.GetFullPath();
        wxString currentLink = wxT("<A HREF=\"") + linkFullPath + wxT("\">") +
                              linkName +
                              wxT("</A>");

        auxLine = currentLink + trashTail + auxLine;        
        endSubStr = currentPos;
        
        // Advance vector of positions-extensions to next useful (position, ext)
        // Ex: After succesful detection of link, any extension inside must be ignored.
        //   Good link:                    vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
        //   Dangerous line: (...) bla bla /home/user/traces/current.cfgs.chop.prv.gz
        //   Extension to avoid!!:                                  ^^^^^
        while ( i < extensionsPositions.size() && extensionsPositions[i].first > currentPos )
        {
          ++i;
        }
      }
      else
      {
        // Advance vector positions-extensions to next useful (position, ext)
        // It's the next one, because we didn't consume any rawLine character (no links!)
        ++i;
        endSubStr = oldEndSubStr;
      }
    }

    if ( endSubStr > 0 )
    {
      wxString trashHead = rawLine.Mid( 0, endSubStr );
      trashHead.Replace( wxT( " " ), wxT( "&nbsp;" ) );
      auxLine = trashHead + auxLine;
    }

    // Finally we copy it.
    rawLine = auxLine;
  }

  return rawLine;
}


std::string RunScript::getHrefFullPath( wxHtmlLinkEvent &event )
{
  std::string hrefFullPath = std::string( event.GetLinkInfo().GetHref().mb_str() );

  return hrefFullPath;
}


bool RunScript::matchHrefExtension( wxHtmlLinkEvent &event, const wxString extension )
{
  return ( event.GetLinkInfo().GetHref().Right( extension.Len() ).Cmp( extension ) == 0 );
}


/*!
 * wxEVT_COMMAND_HTML_LINK_CLICKED event handler for ID_LISTBOX_RUN_LOG
 */

void RunScript::OnListboxRunLogLinkClicked( wxHtmlLinkEvent& event )
{
  wxString auxCommand;

  if ( matchHrefExtension( event, wxT(".prv") ) || matchHrefExtension( event, wxT(".prv.gz")))
  {
    paraverMain::myParaverMain->DoLoadTrace( getHrefFullPath( event ) );
  }
  else if ( matchHrefExtension( event, wxT(".dat" )))
  {
    wxString command = wxString( wxT( "libreoffice --calc " ) ) +
                       wxString( wxT( "\"" ) ) +
                       wxString( getHrefFullPath( event ).c_str(), wxConvUTF8 ) +
                       wxString( wxT( "\"" ) );
    runDetachedProcess( command );    
  }
  else if ( matchHrefExtension( event, wxT(".gnuplot" )))
  {
    wxString command = wxString( wxT( "gnuplot -p " ) ) +
                       wxString( wxT( "\"" ) ) +
                       wxString( getHrefFullPath( event ).c_str(), wxConvUTF8 ) +
                       wxString( wxT( "\"" ) );
    runDetachedProcess( command );    
  }
  else if ( matchHrefExtension( event, _(".cfg")))
  {
    if ( paraverMain::myParaverMain->GetLoadedTraces().size() > 0 )
    {
      paraverMain::myParaverMain->DoLoadCFG( getHrefFullPath( event )  );
    }
    else
    {
      ShowWarning( wxString( wxT( "No trace loaded." ) ) );
    }
  }
  else if ( matchHrefExtension( event, _(".xml")))
  {
    std::string traceName = std::string( wxFileName( filePickerTrace->GetPath() ).GetFullPath().mb_str() );

    bool loadTrace = true;
    std::string strXmlFile = getHrefFullPath( event );

    paraverMain::myParaverMain->ShowCutTraceWindow( traceName, loadTrace, strXmlFile );
  }
  else
  {
    event.Skip();
  }
}


void RunScript::runDetachedProcess( wxString command )
{
  RunningProcess *localProcess = new RunningProcess( this, command );
  if( !wxExecute( command, wxEXEC_ASYNC, myProcess ) )
  {
    ShowWarning( wxT( "Unable to execute command. Please check it and rerun" ) );
  }
  else
  {
    localProcess->Detach();
  }
  
  delete localProcess;
  localProcess = NULL;
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DIMEMAS_GUI
 */
void RunScript::OnButtonDimemasGuiClick( wxCommandEvent& event )
{
/*
  wxString dimemasHome;
  if ( wxGetEnv( wxT( "DIMEMAS_HOME" ), &dimemasHome ) )
  {
    wxString dimemasBinPath = dimemasHome +  wxFileName::GetPathSeparator() + wxString( wxT( "bin" ) ) +  wxFileName::GetPathSeparator();
    wxString command  = wxString( wxT( "\"" ) ) + dimemasBinPath + wxString( wxT( "DimemasGUI" ) ) + wxString( wxT( "\"" ) );

*/
  
  wxString command = GetReachableCommand( DIMEMAS_GUI );
  if( !command.IsEmpty() )
  {  
    
    //wxString command  = wxString( wxT( "\"" ) ) + dimemasBinPath + wxString( wxT( "DimemasGUI" ) ) + wxString( wxT( "\"" ) );
    // wxString command  = doubleQuote( dimemasBinPath + application[ DIMEMAS_GUI ] );
    runDetachedProcess( command );    
  }
  else
  {
    wxMessageDialog message( this,
                             _("Environment variable $DIMEMAS_HOME not found.\n"
                               "Please set to DIMEMAS installation."),
                             _( "Warning" ), wxOK );
    message.ShowModal();
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_DIMEMAS_GUI
 */

void RunScript::OnButtonDimemasGuiUpdate( wxUpdateUIEvent& event )
{
  bool active = ( myProcess == NULL );
          
  buttonDimemasGUI->Enable( active );
}


/*!
 * wxEVT_FILEPICKER_CHANGED event handler for ID_FILEPICKER_TRACE
 */

void RunScript::OnFilepickerTraceFilePickerChanged( wxFileDirPickerEvent& event )
{
  if ( choiceApplication->GetSelection() == STATS )
  {
    statsTextCtrlOutputName->SetValue( filePickerTrace->GetPath() );
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for wxID_LABELCOMMANDPREVIEW
 */

void RunScript::OnLabelcommandpreviewUpdate( wxUpdateUIEvent& event )
{
  wxString dummyCommand, dummyParameter;
  event.SetText( GetCommand( dummyCommand, dummyParameter ) );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHECKBOX_CLUSTERING_SEMVAL_AS_CLUSTDIMENSION
 */

void RunScript::OnCheckboxClusteringSemvalAsClustdimensionUpdate( wxUpdateUIEvent& event )
{
  checkBoxClusteringNormalize->Enable( checkBoxClusteringCSVValueAsDimension->GetValue() );
}


void RunScript::setApp( TExternalApp whichApp )
{
  choiceApplication->Select( whichApp ); 
  adaptWindowToApplicationSelection();
}


void RunScript::setDimemas()
{
  setApp( DIMEMAS_WRAPPER );
}


void RunScript::setStats()
{
  setApp( STATS_WRAPPER );
}


void RunScript::setClustering( wxString whichClusteringCSV )
{
  clusteringCSV = whichClusteringCSV;
  setApp( CLUSTERING );
}


void RunScript::setFolding( wxString whichFoldingCSV )
{
  foldingCSV = whichFoldingCSV;
  setApp( FOLDING );
}


void RunScript::setUserDefined()
{
  setApp( USER_DEFINED );
}
