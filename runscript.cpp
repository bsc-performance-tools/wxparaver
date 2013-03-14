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

#include "wxparaverapp.h" // PATH_SEP

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

  EVT_BUTTON( ID_BUTTON_DIMEMAS_GUI, RunScript::OnButtonDimemasGuiClick )
  EVT_UPDATE_UI( ID_BUTTON_DIMEMAS_GUI, RunScript::OnButtonDimemasGuiUpdate )

  EVT_BUTTON( ID_BUTTON_STATS_RUN_GNUPLOT, RunScript::OnButtonStatsRunGnuplotClick )
  EVT_UPDATE_UI( ID_BUTTON_STATS_RUN_GNUPLOT, RunScript::OnButtonStatsRunGnuplotUpdate )

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
                      wxString whichApp,
                      wxString whichTrace,
                      wxString whichCommand,
                      bool runNow,
                      wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
  
  if ( !whichTrace.IsEmpty() )
  {
     filePickerTrace->SetPath( whichTrace );
  }
  
  if ( !whichCommand.IsEmpty() )
  {
    // Find if whichApp is in registered applications list
    // If whichApp isn't registered -> message? turn into command?
    // widget ---> filePickerScript
    wxString auxCommand = whichCommand;

    int nextPos = auxCommand.Find( wxT( "%20" ) ) ;
    if ( nextPos != wxNOT_FOUND )
    {
      auxCommand.Replace( wxT( "%20" ), wxT( " " ) );
    }
    
    textCtrlDefaultParameters->SetValue( auxCommand );
  }
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
  paraverBin = wxT( "" );
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
  statsButtonRunGnuplot = NULL;
  buttonHelpScript = NULL;
  buttonRun = NULL;
  buttonClearLog = NULL;
  listboxRunLog = NULL;
  buttonExit = NULL;
////@end RunScript member initialisation

  wxString paraverHome;
  if ( wxGetEnv( wxT( "PARAVER_HOME" ), &paraverHome ) )
  {
    paraverBin = paraverHome + wxFileName::GetPathSeparator() + wxString( wxT( "bin" ) ) + wxFileName::GetPathSeparator();
  }
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

  statsLabelTextCtrlOutputName = new wxStaticText( itemDialog1, wxID_STATIC, _("Output Name"), wxDefaultPosition, wxDefaultSize, 0 );
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

  statsCheckBoxShowBurstsHistogram = new wxCheckBox( itemDialog1, ID_CHECKBOX_STATS_SHOW_BURSTS, _("Show bursts histogram"), wxDefaultPosition, wxDefaultSize, 0 );
  statsCheckBoxShowBurstsHistogram->SetValue(false);
  if (RunScript::ShowToolTips())
    statsCheckBoxShowBurstsHistogram->SetToolTip(_("Show bursts histogram"));
  itemBoxSizer30->Add(statsCheckBoxShowBurstsHistogram, 1, wxGROW|wxALL, 2);

  statsCheckBoxShowCommsHistogram = new wxCheckBox( itemDialog1, ID_CHECKBOX_STATS_SHOW_COMMS_HISTOGRAM, _("Show communications histogram"), wxDefaultPosition, wxDefaultSize, 0 );
  statsCheckBoxShowCommsHistogram->SetValue(false);
  if (RunScript::ShowToolTips())
    statsCheckBoxShowCommsHistogram->SetToolTip(_("Show communications histogram"));
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

  wxBoxSizer* itemBoxSizer36 = new wxBoxSizer(wxHORIZONTAL);
  statsSection->Add(itemBoxSizer36, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  statsButtonRunGnuplot = new wxButton( itemDialog1, ID_BUTTON_STATS_RUN_GNUPLOT, _("Run gnuplot"), wxDefaultPosition, wxDefaultSize, 0 );
  if (RunScript::ShowToolTips())
    statsButtonRunGnuplot->SetToolTip(_("Browse output .gnuplot file."));
  itemBoxSizer36->Add(statsButtonRunGnuplot, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticLine* itemStaticLine38 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemBoxSizer2->Add(itemStaticLine38, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer39 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer39, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT, 5);

  buttonHelpScript = new wxButton( itemDialog1, ID_BUTTON_HELP_SCRIPT, _("Help"), wxDefaultPosition, wxDefaultSize, 0 );
  if (RunScript::ShowToolTips())
    buttonHelpScript->SetToolTip(_("Shows the application '--help' message if available"));
  buttonHelpScript->Show(false);
  itemBoxSizer39->Add(buttonHelpScript, 0, wxGROW|wxALL, 5);

  buttonRun = new wxButton( itemDialog1, ID_BUTTON_RUN, _("Run"), wxDefaultPosition, wxDefaultSize, 0 );
  if (RunScript::ShowToolTips())
    buttonRun->SetToolTip(_("Runs the application"));
  itemBoxSizer39->Add(buttonRun, 0, wxGROW|wxALL, 5);

  buttonClearLog = new wxButton( itemDialog1, ID_BUTTON_CLEAR_LOG, _("Clear Log"), wxDefaultPosition, wxDefaultSize, 0 );
  if (RunScript::ShowToolTips())
    buttonClearLog->SetToolTip(_("Clears accumulated messages"));
  itemBoxSizer39->Add(buttonClearLog, 0, wxGROW|wxALL, 5);

  listboxRunLog = new wxHtmlWindow( itemDialog1, ID_LISTBOX_RUN_LOG, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO|wxHSCROLL|wxVSCROLL|wxALWAYS_SHOW_SB );
  if (RunScript::ShowToolTips())
    listboxRunLog->SetToolTip(_("Execution messages"));
  itemBoxSizer2->Add(listboxRunLog, 1, wxGROW|wxALL, 7);

  wxBoxSizer* itemBoxSizer44 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer44, 0, wxALIGN_RIGHT|wxALL, 5);

  wxBoxSizer* itemBoxSizer45 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer44->Add(itemBoxSizer45, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  buttonExit = new wxButton( itemDialog1, ID_BUTTON_EXIT, _("Exit"), wxDefaultPosition, wxDefaultSize, 0 );
  if (RunScript::ShowToolTips())
    buttonExit->SetToolTip(_("Close window but don't run the selected application."));
  itemBoxSizer45->Add(buttonExit, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

////@end RunScript content construction

  choiceApplication->Append( wxT( "Dimemas" ) );
  choiceApplication->Append( wxT( "Stats" ) );
  choiceApplication->Append( wxT( "User defined" ) );
  choiceApplication->Select( 0 ); // Dimemas
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


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_RUN
 */

void RunScript::OnButtonRunClick( wxCommandEvent& event )
{
  buttonRun->Enable( false );

  // TODO: Check parameters should be done HERE
  
  // Build command
  wxString command;
  wxString currentChoice = choiceApplication->GetString( choiceApplication->GetSelection() );
 
  if ( currentChoice == wxString( wxT( "Dimemas" ) ) )
  {
    // First kind: Application needs a previous wrapper
    if ( paraverBin.IsEmpty() )
    {
      wxMessageDialog message( this,
                               _("Unable to find 'dimemas-wrapper.h'\n"
                                 "Please quit and rerun wxparaver, after setting $PARAVER_HOME."),
                               _( "Warning" ), wxOK );
      message.ShowModal();
    }
    else
    {
      command  = paraverBin + wxString( wxT( "dimemas-wrapper.sh" ) );
      command += wxT( " " ) + filePickerTrace->GetPath();      // Source trace
      command += wxT( " " ) + filePickerDimemasCFG->GetPath(); // Dimemas cfg
      command += wxT( " " ) + textCtrlOutputTrace->GetValue(); // Final trace
      if ( checkBoxReuseDimemasTrace->IsChecked() )
      {
        command += wxT( " 1" );
      }
      else
      {
        command += wxT( " 0" );
      }
      command += wxT( " " ) + expandVariables( textCtrlDefaultParameters->GetValue() ); // Extra params
    }
  }
  else if ( currentChoice == wxString( wxT( "Stats" ) ) )
  {
    // First kind: Application needs a previous wrapper
    if ( paraverBin.IsEmpty() )
    {
      wxMessageDialog message( this,
                               _("Unable to find 'stats'\n"
                                 "Please quit and rerun wxparaver, after setting $PARAVER_HOME."),
                               _( "Warning" ), wxOK );
      message.ShowModal();
    }
    else
    {
      // TODO: DEFAULT VALUES?
      command  = paraverBin + wxString( wxT( "stats" ) );
      command += wxT( " " ) + filePickerTrace->GetPath();      // Source trace
      command += wxT( " -o " ) + statsTextCtrlOutputName->GetValue(); // Final name
      if ( statsCheckBoxShowBurstsHistogram->IsChecked() )
      {
        command += wxT( " -bursts_histo" );
      }
      if ( statsCheckBoxShowCommsHistogram->IsChecked() )
      {
        command += wxT( " -comms_histo" );
      }
      if ( statsCheckBoxOnlyDatFile->IsChecked() )
      {
        command += wxT( " -only_dat_file" );
      }
      if ( statsCheckBoxExclusiveTimes->IsChecked() )
      {
        command += wxT( " -exclusive_times" );
      }
      command += wxT( " " ) + expandVariables( textCtrlDefaultParameters->GetValue() ); // Extra params
    }
  }
  else if ( currentChoice == wxString( wxT( "User defined" ) ) )
  {
    // Second kind: Default parameter is directly used
    command += expandVariables( textCtrlDefaultParameters->GetValue() );
  }
  else 
  {
    // Third kind: registered application doesn't need a wrapper; app has the same name
    command = choiceApplication->GetString( choiceApplication->GetSelection() );
  }                
                
  // Run command
  // if ( currentChoice != wxString( wxT( "Dimemas" ) ) || !paraverBin.IsEmpty() )
  if ( currentChoice == wxString( wxT( "User defined" ) ) || !paraverBin.IsEmpty() )
  {
std::cout << command << std::endl;
    myProcess = new RunningProcess( this, command );
    if( !wxExecute( command, wxEXEC_ASYNC, myProcess ) )
    {
      OnProcessTerminated();
    }
  }
  else
  {
    buttonRun->Enable( true );
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_RUN
 */

void RunScript::OnButtonRunUpdate( wxUpdateUIEvent& event )
{
  // TODO: Strong parameter check can be used to disable Run button
  
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
  msg = insertAllLinks( msg );
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


void RunScript::adaptWindowToApplicationSelection()
{
  // TODO: from the list of registered applications, +o- using internal table with *boxsizers
  wxString currentChoice = choiceApplication->GetString( choiceApplication->GetSelection() );

  if ( currentChoice == wxString( wxT( "User defined" ) ))
  {
    labelTextCtrlDefaultParameters->SetLabel( wxT( "Command" ) );
    textCtrlDefaultParameters->SetToolTip( wxT( "Command and parameters to execute\n"
                                                "%TRACE refers to input trace" ) );    
  }
  else if ( currentChoice == wxString( wxT( "Stats" ) ))
  {
    labelTextCtrlDefaultParameters->SetLabel( wxT( "Parameters" ) ); 
    textCtrlDefaultParameters->SetToolTip( wxT( "Extra parameters passed to 'stats'\n"
                                                "-events_histo[:type1[-type2],...]\n"
                                                "-thread_calls[:type1[-type2],...]\n" ) );
  }
  else // Dimemas
  {
    labelTextCtrlDefaultParameters->SetLabel( wxT( "Parameters" ) ); 
    textCtrlDefaultParameters->SetToolTip( wxT( "Extra parameters passed to the script\n"
                                                "%TRACE refers to input trace" ) );
  }
  
  dimemasSection->Show( currentChoice == wxString( wxT( "Dimemas" ) ) );
  statsSection->Show( currentChoice == wxString( wxT( "Stats" ) ) );
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
  command.Replace( wxT( "%TRACE" ), filePickerTrace->GetPath() );
  
  return command;
}


// Returns substring [initPos, finalPos] of rawLine
// It include HREF info if file is found
wxString RunScript::expandLink( wxString rawLine, 
                                int initPos,
                                int initSuffixPos,
                                int finalPos )
{
  wxString subStrWithExpandedLink = rawLine;

  // If available, get trace path from wxFilePickerCtrl
  wxString selectedTracePath =
          wxFileName( filePickerTrace->GetPath() ).GetPath( wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
  
// Find trace candidate
  wxFileName candidateFile;
  bool candidateFound = false;
  int currentPos = initPos;
  while ( currentPos < initSuffixPos && !candidateFound )
  {
    // Normalize
    wxString candidateName = rawLine.Mid( currentPos, finalPos - currentPos );
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

    wxString trashPrefix = rawLine.Mid( initPos, currentPos - initPos );
    trashPrefix.Replace( wxT( " " ), wxT( "&nbsp;" ) );
    wxString linkName = rawLine.Mid( currentPos, finalPos - currentPos );
    linkName.Replace( wxT( " " ), wxT( "&nbsp;" ) );
    wxString linkFullPath = candidateFile.GetFullPath();
    wxString currentLink = wxT("<A HREF=\"") + linkFullPath + wxT("\">") + linkName + wxT("</A>");
    
    subStrWithExpandedLink = trashPrefix + currentLink;
  }
  else
    subStrWithExpandedLink.Replace( wxT( " " ), wxT( "&nbsp;" ) );

  return subStrWithExpandedLink;
}


// Parse rawLine and try to substitute every trace candidate by a complete HTML link
wxString RunScript::insertAllLinks( wxString rawLine )
{
  wxString lineWithLinks;

  int endLine = rawLine.Len();
  int initSubStr = 0;
  int endSubStr = 0;
  while ( endSubStr < endLine )
  {
    wxString subStr = rawLine.Mid( initSubStr, endLine - initSubStr );
    int initSuffixPos = subStr.Find( wxT( ".prv" ) );
    if ( initSuffixPos != wxNOT_FOUND )
    {
      // Compute new end of the substring 
      endSubStr = initSuffixPos + wxString( wxT( ".prv" ) ).Len();

      // Does it end with ".prv" or ".prv.gz"?
      if ( initSuffixPos + int( wxString( wxT( ".prv.gz" ) ).Len() ) <= endLine )
      {
        if ( rawLine.Mid( initSuffixPos, wxString( wxT( ".prv.gz" ) ).Len() ) == 
             wxString( wxT( ".prv.gz" ) ) )
        {
          // Shift end to the right
          endSubStr += wxString( wxT( ".gz" ) ).Len();
        }
      }
      
      // Build HTML link and append it
      lineWithLinks += expandLink( rawLine, initSubStr, initSuffixPos, endSubStr );
      
      // Advance
      initSubStr = endSubStr;
    }
    else
    {
      // No more ".prv" found; print rest of the line
      wxString tail = rawLine.Mid( endSubStr, endLine - endSubStr );
      tail.Replace( wxT( " " ), wxT( "&nbsp;" ) );
      lineWithLinks += tail;
      
      // And exit the loop
      endSubStr = endLine;
    }
  } 

  if( lineWithLinks.IsEmpty() )
  {
    rawLine.Replace( wxT( " " ), wxT( "&nbsp;" ) );
    lineWithLinks = rawLine;
  }
  
  return lineWithLinks;
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
/*
  else if ( matchHrefExtension( event, _(".cfg")))
  {
    if ( paraverMain::myParaverMain->GetLoadedTraces().size() > 0 )
    {
      paraverMain::myParaverMain->DoLoadCFG( getHrefFullPath( event )  );
    }
    else
    {
      wxMessageDialog message( this, _("No trace loaded."), _( "Warning" ), wxOK );
      message.ShowModal();
    }
  }
  else if ( matchHrefExtension( event, _(".xml")))
  {
    string traceName = getCurrentTutorialFullPath();
    bool loadTrace = true;
    string strXmlFile = getHrefFullPath( event );

    paraverMain::myParaverMain->ShowCutTraceWindow( traceName, loadTrace, strXmlFile );
  }
*/
  else
  {
    event.Skip();
  }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DIMEMAS_GUI
 */

void RunScript::OnButtonDimemasGuiClick( wxCommandEvent& event )
{
  wxString dimemasHome;
  if ( wxGetEnv( wxT( "DIMEMAS_HOME" ), &dimemasHome ) )
  {
    wxString dimemasBinPath = dimemasHome +  wxFileName::GetPathSeparator() + wxString( wxT( "bin" ) ) +  wxFileName::GetPathSeparator();
    wxString command  = dimemasBinPath + wxString( wxT( "DimemasGUI" ) );
    RunningProcess *localProcess = new RunningProcess( this, command );
    if( !wxExecute( command, wxEXEC_ASYNC, myProcess ) )
    {
      wxMessageDialog message( this,
                               _("Unable to execute command."
                                 "Please check it and rerun."),
                               _( "Warning" ), wxOK );
      message.ShowModal();
    }
    else
    {
      localProcess->Detach();
    }
    
    delete localProcess;
    localProcess = NULL;
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
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_STATS_RUN_GNUPLOT
 */

void RunScript::OnButtonStatsRunGnuplotClick( wxCommandEvent& event )
{

}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_STATS_RUN_GNUPLOT
 */

void RunScript::OnButtonStatsRunGnuplotUpdate( wxUpdateUIEvent& event )
{
  // TODO: Check existence of gnuplot in path, and .gnuplot file generated
  bool active = ( myProcess == NULL );
          
  statsButtonRunGnuplot->Enable( active );
}

