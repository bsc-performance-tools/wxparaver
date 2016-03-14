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

#include <string>
#include <algorithm>

#include <wx/filename.h>

#include "sequencedriver.h"
#include "kernelconnection.h"
#include "gtimeline.h"
#include "traceeditsequence.h"
#include "traceeditstates.h"
#include "traceoptions.h"
#include "runscript.h"
#include "wxparaverapp.h"
#include "cutfilterdialog.h"

/****************************************************************************
 ********             RunAppClusteringAction                         ********
 ****************************************************************************/
vector<TraceEditSequence::TSequenceStates> RunAppClusteringAction::getStateDependencies() const
{
  vector<TraceEditSequence::TSequenceStates> tmpStates;
  return tmpStates;
}

bool RunAppClusteringAction::execute( std::string whichTrace )
{
  bool errorFound = false;

  TraceEditSequence *tmpSequence = (TraceEditSequence *)mySequence;
  std::string tmpFileName = ( (CSVFileNameState *)tmpSequence->getState( TraceEditSequence::csvFileNameState ) )->getData();
  RunScript runAppDialog( wxparaverApp::mainWindow, wxString::FromAscii( whichTrace.c_str() ) );
  runAppDialog.setClustering( wxString::FromAscii( tmpFileName.c_str() ) );

  if( runAppDialog.ShowModal() == wxID_OK )
  {}
  
 return errorFound;
}


/****************************************************************************
 ********              RunAppFoldingAction                           ********
 ****************************************************************************/
vector<TraceEditSequence::TSequenceStates> RunAppFoldingAction::getStateDependencies() const
{
  vector<TraceEditSequence::TSequenceStates> tmpStates;
  return tmpStates;
}

bool RunAppFoldingAction::execute( std::string whichTrace )
{
  bool errorFound = false;

  TraceEditSequence *tmpSequence = (TraceEditSequence *)mySequence;
  std::string tmpFileName = ( (CSVFileNameState *)tmpSequence->getState( TraceEditSequence::csvFileNameState ) )->getData();
  RunScript runAppDialog( wxparaverApp::mainWindow, wxString::FromAscii( whichTrace.c_str() ) );
  runAppDialog.setFolding( wxString::FromAscii( tmpFileName.c_str() ) );

  if( runAppDialog.ShowModal() == wxID_OK )
  {}

  return errorFound;
}


/****************************************************************************
 ********              RunAppDimemasAction                           ********
 ****************************************************************************/
vector<TraceEditSequence::TSequenceStates> RunAppDimemasAction::getStateDependencies() const
{
  vector<TraceEditSequence::TSequenceStates> tmpStates;
  return tmpStates;
}

bool RunAppDimemasAction::execute( std::string whichTrace )
{
  bool errorFound = false;

  //TraceEditSequence *tmpSequence = (TraceEditSequence *)mySequence;
  RunScript runAppDialog( wxparaverApp::mainWindow, wxString::FromAscii( whichTrace.c_str() ) );
  runAppDialog.setDimemas();

  if( runAppDialog.ShowModal() == wxID_OK )
  {}
  
  return errorFound;
}


/****************************************************************************
 ********              RunAppCutterAction                            ********
 ****************************************************************************/
vector<TraceEditSequence::TSequenceStates> RunAppCutterAction::getStateDependencies() const
{
  vector<TraceEditSequence::TSequenceStates> tmpStates;
  return tmpStates;
}


bool RunAppCutterAction::execute( std::string whichTrace )
{
  bool errorFound = false;

  CutFilterDialog *cutFilterDialog = new CutFilterDialog( wxparaverApp::mainWindow );
  

  wxparaverApp::mainWindow->MainSettingsCutFilterDialog( cutFilterDialog, whichTrace, true );
  
  TraceEditSequence *tmpSequence = (TraceEditSequence *)mySequence;
  TraceOptions *traceOptions = ( (TraceOptionsState *)tmpSequence->getState( TraceEditSequence::traceOptionsState ) )->getData();
  string dummyXmlName = "";
  vector< string > toolOrder;
  toolOrder.push_back( TraceCutter::getID() );
  wxparaverApp::mainWindow->OptionsSettingCutFilterDialog( cutFilterDialog, traceOptions, dummyXmlName, toolOrder );

  //cutFilterDialog->TransferDataToWindow( toolOrder, traceOptions );
  //cutFilterDialog->EnableAllTabsFromToolsList(); // TODO: Probably shouldn't be here, it should be in CutFilterDialog
  
  if( cutFilterDialog->ShowModal() == wxID_OK )
  {  
    wxparaverApp::mainWindow->OnOKCutFilterDialog( cutFilterDialog, toolOrder );
  }

#if wxMAJOR_VERSION<3
  cutFilterDialog->MakeModal( false );
#endif
  
  delete traceOptions;
  delete cutFilterDialog;
  
  return errorFound;
}


/****************************************************************************
 ********              RunSpectralAction                             ********
 ****************************************************************************/
vector<TraceEditSequence::TSequenceStates> RunSpectralAction::getStateDependencies() const
{
  vector<TraceEditSequence::TSequenceStates> tmpStates;
  return tmpStates;
}


// Change timeline to level APPLICATION in NS
// Save CSV
// Throw command "$SPECTRAL_HOME/bin/csv-analysis trace.prv saved.csv X" with X = 0
// Load resulting trace.iterations.prv + iterations.cfg
bool RunSpectralAction::execute( std::string whichTrace )
{
  bool errorFound = true;
  wxString errorMsg;
  
  TraceEditSequence *tmpSequence = (TraceEditSequence *)mySequence;
  std::string tmpFileName = ( (CSVFileNameState *)tmpSequence->getState( TraceEditSequence::csvFileNameState ) )->getData();

  // Exist csv-analysis (Spectral installation)?
  wxString spectralEnvVar = wxString( wxT("SPECTRAL_HOME") );
  wxString spectralPath;
  if ( wxGetEnv( spectralEnvVar, &spectralPath ) )
  {
    wxString tmpSep = wxFileName::GetPathSeparator();
    wxString spectralBin = spectralPath + tmpSep + _("bin") + tmpSep + _("spectral-csv-analysis");
    if ( wxFileName::IsFileExecutable( spectralBin ) )
    {
      // Throw command '$SPECTRAL_HOME/bin/csv-analysis "trace.prv" "saved.csv" X' with X = 0
      wxString traceFileName( _("\"") + wxString::FromAscii( whichTrace.c_str() ) + _("\"") );
      wxString csvFileName( _("\"") + wxString::FromAscii( tmpFileName.c_str() ) + _("\"") );
      wxString numericParameter( _("0") );
      wxString command = _( "/bin/sh -c '") + 
                         spectralBin + _(" ") +
                         traceFileName + _(" ") +
                         csvFileName + _(" ") +
                         numericParameter + _(" ");
                         _(" 1>&- 2>&-'");

      wxExecute( command, wxEXEC_SYNC );
      
      // Load resulting trace + cfg
      std::string tmpIterTrace = whichTrace;
      size_t lastDot = tmpIterTrace.find_last_of(".");
      tmpIterTrace = tmpIterTrace.substr( 0, lastDot ) + std::string( ".iterations.prv" );
      wxString tmpIterTrace_wx = wxString::FromAscii( tmpIterTrace.c_str() );

      std::string tmpCFG = wxparaverApp::mainWindow->GetLocalKernel()->getDistributedCFGsPath() + PATH_SEP +
                  std::string("spectral") + PATH_SEP +
                  std::string("iterations.cfg");
      wxString tmpCFG_wx = wxString::FromAscii( tmpCFG.c_str() );
      
      if ( wxFileName::FileExists( tmpIterTrace_wx ) )
      {
        if ( wxFileName::FileExists( tmpCFG_wx ) )
        {
          wxparaverApp::mainWindow->DoLoadTrace( tmpIterTrace );
          wxparaverApp::mainWindow->DoLoadCFG( tmpCFG );
          errorFound = false;
        }
        else
          errorMsg = wxString( _("Missing file:\n\n") ) + tmpCFG_wx;
      }
      else
        errorMsg = wxString( _("Missing file:\n\n") ) + tmpIterTrace_wx;
    }
    else
      errorMsg = wxString( _("Unable to find/execute file:\n\n") ) + spectralBin;
  }
  else
    errorMsg =  wxString( _("Undeclared environment variable:\n\n$") ) + spectralEnvVar;
  
  if ( errorFound )
  {
    errorMsg += wxString( _("\n\nSpectral sequence aborted.") );
    wxMessageBox( errorMsg, _( "Warning" ), wxOK | wxICON_WARNING );
  }
  
  return errorFound;
}


/****************************************************************************
 ********              ExternalSortAction                            ********
 ****************************************************************************/
vector<TraceEditSequence::TSequenceStates> ExternalSortAction::getStateDependencies() const
{
  vector<TraceEditSequence::TSequenceStates> tmpStates;
  return tmpStates;
}

bool ExternalSortAction::execute( std::string whichTrace )
{
  bool errorFound = false;

 // TraceEditSequence *tmpSequence = (TraceEditSequence *)mySequence;

  return errorFound;
}


/****************************************************************************
 ********                 SequenceDriver                             ********
 ****************************************************************************/
void SequenceDriver::sequenceClustering( gTimeline *whichTimeline )
{
  KernelConnection *myKernel =  whichTimeline->GetMyWindow()->getKernel();
  TraceEditSequence *mySequence = TraceEditSequence::create( myKernel );

  mySequence->pushbackAction( TraceEditSequence::csvOutputAction );
  mySequence->pushbackAction( TraceEditSequence::traceCutterAction );
  mySequence->pushbackAction( new RunAppClusteringAction( mySequence ) );
  
  TraceOptions *tmpOptions = TraceOptions::create( myKernel );
  tmpOptions->set_by_time( true );
  tmpOptions->set_min_cutting_time( whichTimeline->GetMyWindow()->getWindowBeginTime() );
  tmpOptions->set_max_cutting_time( whichTimeline->GetMyWindow()->getWindowEndTime() );
  tmpOptions->set_original_time( false );
  tmpOptions->set_break_states( false );

  TraceOptionsState *tmpOptionsState = new TraceOptionsState( mySequence );
  tmpOptionsState->setData( tmpOptions );
  mySequence->addState( TraceEditSequence::traceOptionsState, tmpOptionsState );
  
  TextOutput output;
  output.setObjectHierarchy( true );
  output.setWindowTimeUnits( false );
  CSVOutputState *tmpOutputState = new CSVOutputState( mySequence );
  tmpOutputState->setData( output );
  mySequence->addState( TraceEditSequence::csvOutputState, tmpOutputState );
  
  CSVWindowState *tmpWindowState = new CSVWindowState( mySequence );
  tmpWindowState->setData( whichTimeline->GetMyWindow() );
  mySequence->addState( TraceEditSequence::csvWindowState, tmpWindowState );

  CSVFileNameState *tmpCSVFilenameState = new CSVFileNameState( mySequence );
  std::string tmpFileName;
  wxFileName tmpTraceName( wxString::FromAscii( whichTimeline->GetMyWindow()->getTrace()->getFileName().c_str() ) );
  tmpTraceName.ClearExt();
  tmpTraceName.AppendDir( wxString::FromAscii( TraceEditSequence::dirNameClustering.c_str() ) );
  
  if( !tmpTraceName.DirExists() )
    tmpTraceName.Mkdir();
  std::string auxName = whichTimeline->GetMyWindow()->getName() + "_";
  std::replace( auxName.begin(), auxName.end(), ',', '-' );
  tmpFileName = std::string( tmpTraceName.GetPath( wxPATH_GET_SEPARATOR ).mb_str() ) + auxName.c_str() + std::string( tmpTraceName.GetFullName().mb_str() ) + std::string( ".csv" );

  tmpCSVFilenameState->setData( tmpFileName );
  mySequence->addState( TraceEditSequence::csvFileNameState, tmpCSVFilenameState );

  OutputDirSuffixState *tmpOutputDirSuffixState = new OutputDirSuffixState( mySequence );
  tmpOutputDirSuffixState->setData( TraceEditSequence::dirNameClustering );
  mySequence->addState( TraceEditSequence::outputDirSuffixState, tmpOutputDirSuffixState );
  
  vector<std::string> traces;
  traces.push_back( whichTimeline->GetMyWindow()->getTrace()->getFileName() );
  mySequence->execute( traces );
  
  delete mySequence;
}


void SequenceDriver::sequenceCutter( gTimeline *whichTimeline )
{
  KernelConnection *myKernel =  whichTimeline->GetMyWindow()->getKernel();
  TraceEditSequence *mySequence = TraceEditSequence::create( myKernel );

  mySequence->pushbackAction( new RunAppCutterAction( mySequence ) );
  
  TraceOptions *tmpOptions = TraceOptions::create( myKernel );
  tmpOptions->set_by_time( true );
  tmpOptions->set_min_cutting_time( whichTimeline->GetMyWindow()->getWindowBeginTime() );
  tmpOptions->set_max_cutting_time( whichTimeline->GetMyWindow()->getWindowEndTime() );
  tmpOptions->set_original_time( false );
  tmpOptions->set_break_states( false );
  tmpOptions->set_remLastStates( true );
  tmpOptions->set_keep_events( true );

  TraceOptionsState *tmpOptionsState = new TraceOptionsState( mySequence );
  tmpOptionsState->setData( tmpOptions );
  mySequence->addState( TraceEditSequence::traceOptionsState, tmpOptionsState );

  vector<std::string> traces;
  traces.push_back( whichTimeline->GetMyWindow()->getTrace()->getFileName() );
  mySequence->execute( traces );
  
  delete mySequence;
}


void SequenceDriver::sequenceDimemas( gTimeline *whichTimeline )
{
  KernelConnection *myKernel =  whichTimeline->GetMyWindow()->getKernel();
  TraceEditSequence *mySequence = TraceEditSequence::create( myKernel );

  mySequence->pushbackAction( TraceEditSequence::traceCutterAction );
  mySequence->pushbackAction( new RunAppDimemasAction( mySequence ) );
  
  TraceOptions *tmpOptions = TraceOptions::create( myKernel );
  tmpOptions->set_by_time( true );
  tmpOptions->set_min_cutting_time( whichTimeline->GetMyWindow()->getWindowBeginTime() );
  tmpOptions->set_max_cutting_time( whichTimeline->GetMyWindow()->getWindowEndTime() );
  tmpOptions->set_original_time( false );
  tmpOptions->set_break_states( false );

  TraceOptionsState *tmpOptionsState = new TraceOptionsState( mySequence );
  tmpOptionsState->setData( tmpOptions );
  mySequence->addState( TraceEditSequence::traceOptionsState, tmpOptionsState );

  CSVWindowState *tmpWindowState = new CSVWindowState( mySequence );
  tmpWindowState->setData( whichTimeline->GetMyWindow() );
  mySequence->addState( TraceEditSequence::csvWindowState, tmpWindowState );

  std::string tmpFileName;
  wxFileName tmpTraceName( wxString::FromAscii( whichTimeline->GetMyWindow()->getTrace()->getFileName().c_str() ) );
  tmpTraceName.ClearExt();
  tmpTraceName.AppendDir( wxString::FromAscii( TraceEditSequence::dirNameDimemas.c_str() ) );
  
  if( !tmpTraceName.DirExists() )
    tmpTraceName.Mkdir();
  
  OutputDirSuffixState *tmpOutputDirSuffixState = new OutputDirSuffixState( mySequence );
  tmpOutputDirSuffixState->setData( TraceEditSequence::dirNameDimemas );
  mySequence->addState( TraceEditSequence::outputDirSuffixState, tmpOutputDirSuffixState );
  
  vector<std::string> traces;
  traces.push_back( whichTimeline->GetMyWindow()->getTrace()->getFileName() );
  mySequence->execute( traces );
  
  delete mySequence;
}


void SequenceDriver::sequenceFolding( gTimeline *whichTimeline )
{
  KernelConnection *myKernel =  whichTimeline->GetMyWindow()->getKernel();
  TraceEditSequence *mySequence = TraceEditSequence::create( myKernel );

  mySequence->pushbackAction( TraceEditSequence::csvOutputAction );
  mySequence->pushbackAction( TraceEditSequence::traceCutterAction );
  mySequence->pushbackAction( new RunAppFoldingAction( mySequence ) );
  
  TraceOptions *tmpOptions = TraceOptions::create( myKernel );
  tmpOptions->set_by_time( true );
  tmpOptions->set_min_cutting_time( whichTimeline->GetMyWindow()->getWindowBeginTime() );
  tmpOptions->set_max_cutting_time( whichTimeline->GetMyWindow()->getWindowEndTime() );
  tmpOptions->set_original_time( false );
  tmpOptions->set_break_states( false );

  TraceOptionsState *tmpOptionsState = new TraceOptionsState( mySequence );
  tmpOptionsState->setData( tmpOptions );
  mySequence->addState( TraceEditSequence::traceOptionsState, tmpOptionsState );
  
  TextOutput output;
  output.setObjectHierarchy( true );
  output.setWindowTimeUnits( false );
  output.setTextualSemantic( true );
  CSVOutputState *tmpOutputState = new CSVOutputState( mySequence );
  tmpOutputState->setData( output );
  mySequence->addState( TraceEditSequence::csvOutputState, tmpOutputState );
  
  CSVWindowState *tmpWindowState = new CSVWindowState( mySequence );
  tmpWindowState->setData( whichTimeline->GetMyWindow() );
  mySequence->addState( TraceEditSequence::csvWindowState, tmpWindowState );

  CSVFileNameState *tmpCSVFilenameState = new CSVFileNameState( mySequence );
  std::string tmpFileName;
  wxFileName tmpTraceName( wxString::FromAscii( whichTimeline->GetMyWindow()->getTrace()->getFileName().c_str() ) );
  tmpTraceName.ClearExt();
  tmpTraceName.AppendDir( wxString::FromAscii( TraceEditSequence::dirNameFolding.c_str() ) );
  
  if( !tmpTraceName.DirExists() )
    tmpTraceName.Mkdir();
  std::string auxName = whichTimeline->GetMyWindow()->getName() + "_";
  tmpFileName = std::string( tmpTraceName.GetPath( wxPATH_GET_SEPARATOR ).mb_str() ) +
                auxName.c_str() + std::string( tmpTraceName.GetFullName().mb_str() ) +
                std::string( ".csv" );

  tmpCSVFilenameState->setData( tmpFileName );
  mySequence->addState( TraceEditSequence::csvFileNameState, tmpCSVFilenameState );

  OutputDirSuffixState *tmpOutputDirSuffixState = new OutputDirSuffixState( mySequence );
  tmpOutputDirSuffixState->setData( TraceEditSequence::dirNameFolding );
  mySequence->addState( TraceEditSequence::outputDirSuffixState, tmpOutputDirSuffixState );
  
  vector<std::string> traces;
  traces.push_back( whichTimeline->GetMyWindow()->getTrace()->getFileName() );
  mySequence->execute( traces );
  
  delete mySequence;
}


void SequenceDriver::sequenceSpectral( gTimeline *whichTimeline )
{
  // Create sequence
  KernelConnection *myKernel =  whichTimeline->GetMyWindow()->getKernel();
  TraceEditSequence *mySequence = TraceEditSequence::create( myKernel );

  // Define sequence
  mySequence->pushbackAction( TraceEditSequence::csvOutputAction );
  mySequence->pushbackAction( new RunSpectralAction( mySequence ) );
  
  // Clone timeline
  Window *tmpWindow = whichTimeline->GetMyWindow()->clone();
  tmpWindow->setLevel( APPLICATION );
  tmpWindow->setTimeUnit( NS );
  
  // Trace options state
  TraceOptions *tmpOptions = TraceOptions::create( myKernel );
  tmpOptions->set_by_time( true );
  tmpOptions->set_min_cutting_time( tmpWindow->getWindowBeginTime() );
  tmpOptions->set_max_cutting_time( tmpWindow->getWindowEndTime() );
  tmpOptions->set_original_time( false );
  tmpOptions->set_break_states( false );
  

  TraceOptionsState *tmpOptionsState = new TraceOptionsState( mySequence );
  tmpOptionsState->setData( tmpOptions );
  mySequence->addState( TraceEditSequence::traceOptionsState, tmpOptionsState );
  
  // CSV state
  TextOutput output;
  output.setObjectHierarchy( true );
  output.setWindowTimeUnits( false );
  output.setTextualSemantic( true );
  CSVOutputState *tmpOutputState = new CSVOutputState( mySequence );
  tmpOutputState->setData( output );
  mySequence->addState( TraceEditSequence::csvOutputState, tmpOutputState );

  // CSV window state
  CSVWindowState *tmpWindowState = new CSVWindowState( mySequence );
  tmpWindowState->setData( tmpWindow );
  mySequence->addState( TraceEditSequence::csvWindowState, tmpWindowState );

  // CSV file name state
  CSVFileNameState *tmpCSVFilenameState = new CSVFileNameState( mySequence );
  std::string tmpFileName;
  wxFileName tmpTraceName( wxString::FromAscii( tmpWindow->getTrace()->getFileName().c_str() ) );
  tmpTraceName.ClearExt();
  tmpTraceName.AppendDir( wxString::FromAscii( TraceEditSequence::dirNameSpectral.c_str() ) );
  if( !tmpTraceName.DirExists() )
    tmpTraceName.Mkdir();
  std::string auxName = tmpWindow->getName() + "_";
  tmpFileName = std::string( tmpTraceName.GetPath( wxPATH_GET_SEPARATOR ).mb_str() ) + 
                auxName.c_str() + std::string( tmpTraceName.GetFullName().mb_str() ) +
                std::string( ".csv" );

  tmpCSVFilenameState->setData( tmpFileName );
  mySequence->addState( TraceEditSequence::csvFileNameState, tmpCSVFilenameState );
  
  // Spectral suffix
  OutputDirSuffixState *tmpOutputDirSuffixState = new OutputDirSuffixState( mySequence );
  tmpOutputDirSuffixState->setData( TraceEditSequence::dirNameSpectral );
  mySequence->addState( TraceEditSequence::outputDirSuffixState, tmpOutputDirSuffixState );

  // Engage sequence  
  vector<std::string> traces;
  traces.push_back( tmpWindow->getTrace()->getFileName() );
  mySequence->execute( traces );

  delete tmpWindow;
  delete mySequence;
}
