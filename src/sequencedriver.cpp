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
#include <wx/numdlg.h>

#include "sequencedriver.h"
#include "kernelconnection.h"
#include "gtimeline.h"
#include "traceeditsequence.h"
#include "traceeditstates.h"
#include "traceoptions.h"
#include "runscript.h"
#include "wxparaverapp.h"
#include "cutfilterdialog.h"

using namespace std;

/****************************************************************************
 ********             RunAppClusteringAction                         ********
 ****************************************************************************/
vector<TSequenceStates> RunAppClusteringAction::getStateDependencies() const
{
  vector<TSequenceStates> tmpStates;
  return tmpStates;
}

bool RunAppClusteringAction::execute( std::string whichTrace )
{
  bool errorFound = false;

  TraceEditSequence *tmpSequence = (TraceEditSequence *)mySequence;
  std::string tmpFileName = ( (CSVFileNameState *)tmpSequence->getState( TSequenceStates::csvFileNameState ) )->getData();
  RunScript *runAppDialog = wxparaverApp::mainWindow->GetRunApplication();
  if( runAppDialog == nullptr )
  {
    std::vector<bool> tmpAcceptableApps = ( ( AcceptableAppsState * )tmpSequence->getState( TSequenceStates::acceptableAppsState ) )->getData();
    runAppDialog = new RunScript( wxparaverApp::mainWindow, tmpAcceptableApps );
    wxparaverApp::mainWindow->SetRunApplication( runAppDialog );
  }
  runAppDialog->setTrace( wxString::FromUTF8( whichTrace.c_str() ) );
  runAppDialog->setClustering( wxString::FromUTF8( tmpFileName.c_str() ) );
  
  runAppDialog->Show();
  runAppDialog->Raise();
  
  return errorFound;
}


/****************************************************************************
 ********              RunAppFoldingAction                           ********
 ****************************************************************************/
vector<TSequenceStates> RunAppFoldingAction::getStateDependencies() const
{
  vector<TSequenceStates> tmpStates;
  return tmpStates;
}

bool RunAppFoldingAction::execute( std::string whichTrace )
{
  bool errorFound = false;

  TraceEditSequence *tmpSequence = (TraceEditSequence *)mySequence;
  std::string tmpFileName = ( (CSVFileNameState *)tmpSequence->getState( TSequenceStates::csvFileNameState ) )->getData();
  RunScript *runAppDialog = wxparaverApp::mainWindow->GetRunApplication();
  if( runAppDialog == nullptr )
  {
    std::vector<bool> tmpAcceptableApps = ( ( AcceptableAppsState * )tmpSequence->getState( TSequenceStates::acceptableAppsState ) )->getData();
    runAppDialog = new RunScript( wxparaverApp::mainWindow, tmpAcceptableApps );
    wxparaverApp::mainWindow->SetRunApplication( runAppDialog );
  }
  runAppDialog->setTrace( wxString::FromUTF8( whichTrace.c_str() ) );
  runAppDialog->setFolding( wxString::FromUTF8( tmpFileName.c_str() ) );
  
  runAppDialog->Show();
  runAppDialog->Raise();

  return errorFound;
}


/****************************************************************************
 ********              RunAppDimemasAction                           ********
 ****************************************************************************/
vector<TSequenceStates> RunAppDimemasAction::getStateDependencies() const
{
  vector<TSequenceStates> tmpStates;
  return tmpStates;
}

bool RunAppDimemasAction::execute( std::string whichTrace )
{
  bool errorFound = false;

  TraceEditSequence *tmpSequence = (TraceEditSequence *)mySequence;
  RunScript *runAppDialog = wxparaverApp::mainWindow->GetRunApplication();
  if( runAppDialog == nullptr )
  {
    std::vector<bool> tmpAcceptableApps = ( ( AcceptableAppsState * )tmpSequence->getState( TSequenceStates::acceptableAppsState ) )->getData();
    runAppDialog = new RunScript( wxparaverApp::mainWindow, tmpAcceptableApps );
    wxparaverApp::mainWindow->SetRunApplication( runAppDialog );
  }
  runAppDialog->setTrace( wxString::FromUTF8( whichTrace.c_str() ) );
  runAppDialog->setDimemas();
  
  runAppDialog->Show();
  runAppDialog->Raise();
  
  return errorFound;
}


/****************************************************************************
 ********              RunAppCutterAction                            ********
 ****************************************************************************/
vector<TSequenceStates> RunAppCutterAction::getStateDependencies() const
{
  vector<TSequenceStates> tmpStates;
  return tmpStates;
}


bool RunAppCutterAction::execute( std::string whichTrace )
{
  bool errorFound = false;

  CutFilterDialog *cutFilterDialog = new CutFilterDialog( wxparaverApp::mainWindow );

  wxparaverApp::mainWindow->MainSettingsCutFilterDialog( cutFilterDialog, whichTrace, true );
  
  TraceEditSequence *tmpSequence = (TraceEditSequence *)mySequence;
  TraceOptions *traceOptions = ( (TraceOptionsState *)tmpSequence->getState( TSequenceStates::traceOptionsState ) )->getData();
  string dummyXmlName = "";
  vector< string > toolOrder;
  toolOrder.push_back( TraceCutter::getID() );
  wxparaverApp::mainWindow->OptionsSettingCutFilterDialog( cutFilterDialog, traceOptions, dummyXmlName, toolOrder );

  cutFilterDialog->Show();
  
  return errorFound;
}


/****************************************************************************
 ********                 RunMessAction                            ********
 ****************************************************************************/
vector<TSequenceStates> RunMessAction::getStateDependencies() const
{
  vector<TSequenceStates> tmpStates;
  return tmpStates;
}

bool RunMessAction::execute( std::string whichTrace )
{
  bool errorFound = false;

  TraceEditSequence *tmpSequence = (TraceEditSequence *)mySequence;
  RunScript *runAppDialog = wxparaverApp::mainWindow->GetRunApplication();
  if( runAppDialog == nullptr )
  {
    std::vector<bool> tmpAcceptableApps = ( ( AcceptableAppsState * )tmpSequence->getState( TSequenceStates::acceptableAppsState ) )->getData();
    runAppDialog = new RunScript( wxparaverApp::mainWindow, tmpAcceptableApps );
    wxparaverApp::mainWindow->SetRunApplication( runAppDialog );
  }
  runAppDialog->setTrace( wxString::FromUTF8( whichTrace.c_str() ) );
  runAppDialog->setMess();
  
  runAppDialog->Show();
  runAppDialog->Raise();
  
  return errorFound;
}



/****************************************************************************
 ********              ExternalSortAction                            ********
 ****************************************************************************/
vector<TSequenceStates> ExternalSortAction::getStateDependencies() const
{
  vector<TSequenceStates> tmpStates;
  return tmpStates;
}

bool ExternalSortAction::execute( std::string whichTrace )
{
  bool errorFound = false;

 // TraceEditSequence *tmpSequence = (TraceEditSequence *)mySequence;

  return errorFound;
}


/****************************************************************************
 ********              RunAppUserCommandAction                       ********
 ****************************************************************************/
vector<TSequenceStates> RunAppUserCommandAction::getStateDependencies() const
{
  vector<TSequenceStates> tmpStates;
  return tmpStates;
}

bool RunAppUserCommandAction::execute( std::string whichTrace )
{
  bool errorFound = false;

  TraceEditSequence *tmpSequence = (TraceEditSequence *)mySequence;
  RunScript *runAppDialog = wxparaverApp::mainWindow->GetRunApplication();
  if( runAppDialog == nullptr )
  {
    std::vector<bool> tmpAcceptableApps = ( ( AcceptableAppsState * )tmpSequence->getState( TSequenceStates::acceptableAppsState ) )->getData();
    runAppDialog = new RunScript( wxparaverApp::mainWindow, tmpAcceptableApps );
    wxparaverApp::mainWindow->SetRunApplication( runAppDialog );
  }
  runAppDialog->setTrace( wxString::FromUTF8( whichTrace.c_str() ) );
  runAppDialog->setUserCommand();
  
  runAppDialog->Show();
  runAppDialog->Raise();
  
  return errorFound;
}


/****************************************************************************
 ********                 SequenceDriver                             ********
 ****************************************************************************/
void SequenceDriver::sequenceClustering( gTimeline *whichTimeline )
{
  KernelConnection *myKernel =  whichTimeline->GetMyWindow()->getKernel();
  TraceEditSequence *mySequence = TraceEditSequence::create( myKernel );

  mySequence->pushbackAction( TSequenceActions::csvOutputAction );
  mySequence->pushbackAction( TSequenceActions::traceCutterAction );
  mySequence->pushbackAction( new RunAppClusteringAction( mySequence ) );
  
  AcceptableAppsState *tmpAcceptableAppsState = new AcceptableAppsState( mySequence );
  tmpAcceptableAppsState->setData( whichTimeline->GetMyWindow()->getTrace()->getSuitableApps() );
  mySequence->addState( TSequenceStates::acceptableAppsState, tmpAcceptableAppsState );

  TraceOptions *tmpOptions = TraceOptions::create( myKernel );
  tmpOptions->set_by_time( true );
  tmpOptions->set_min_cutting_time( whichTimeline->GetMyWindow()->getWindowBeginTime() );
  tmpOptions->set_max_cutting_time( whichTimeline->GetMyWindow()->getWindowEndTime() );
  tmpOptions->set_original_time( false );
  tmpOptions->set_break_states( false );

  TraceOptionsState *tmpOptionsState = new TraceOptionsState( mySequence );
  tmpOptionsState->setData( tmpOptions );
  mySequence->addState( TSequenceStates::traceOptionsState, tmpOptionsState );
  
  TextOutput output;
  output.setWindowTimeUnits( false );
  CSVOutputState *tmpOutputState = new CSVOutputState( mySequence );
  tmpOutputState->setData( output );
  mySequence->addState( TSequenceStates::csvOutputState, tmpOutputState );
  
  SourceTimelineState *tmpWindowState = new SourceTimelineState( mySequence );
  tmpWindowState->setData( whichTimeline->GetMyWindow() );
  mySequence->addState( TSequenceStates::sourceTimelineState, tmpWindowState );

  CSVFileNameState *tmpCSVFilenameState = new CSVFileNameState( mySequence );
  std::string tmpFileName;
  wxFileName tmpTraceName( wxString::FromUTF8( whichTimeline->GetMyWindow()->getTrace()->getFileName().c_str() ) );
  tmpTraceName.ClearExt();
  tmpTraceName.AppendDir( wxString::FromUTF8( TraceEditSequence::dirNameClustering.c_str() ) );
  
  if( !tmpTraceName.DirExists() )
    tmpTraceName.Mkdir();
  std::string auxName = whichTimeline->GetMyWindow()->getName() + "_";
  std::replace( auxName.begin(), auxName.end(), ',', '-' );
  tmpFileName = std::string( tmpTraceName.GetPath( wxPATH_GET_SEPARATOR ).mb_str() ) + auxName.c_str() + std::string( tmpTraceName.GetFullName().mb_str() ) + std::string( ".csv" );

  tmpCSVFilenameState->setData( tmpFileName );
  mySequence->addState( TSequenceStates::csvFileNameState, tmpCSVFilenameState );

  OutputDirSuffixState *tmpOutputDirSuffixState = new OutputDirSuffixState( mySequence );
  tmpOutputDirSuffixState->setData( TraceEditSequence::dirNameClustering );
  mySequence->addState( TSequenceStates::outputDirSuffixState, tmpOutputDirSuffixState );
  
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
  tmpOptions->set_keep_boundary_events( true );

  TraceOptionsState *tmpOptionsState = new TraceOptionsState( mySequence );
  tmpOptionsState->setData( tmpOptions );
  mySequence->addState( TSequenceStates::traceOptionsState, tmpOptionsState );

  vector<std::string> traces;
  traces.push_back( whichTimeline->GetMyWindow()->getTrace()->getFileName() );
  mySequence->execute( traces );
  
  delete mySequence;
}


void SequenceDriver::sequenceDimemas( gTimeline *whichTimeline )
{
  KernelConnection *myKernel =  whichTimeline->GetMyWindow()->getKernel();
  TraceEditSequence *mySequence = TraceEditSequence::create( myKernel );

  mySequence->pushbackAction( TSequenceActions::traceCutterAction );
  mySequence->pushbackAction( new RunAppDimemasAction( mySequence ) );
  
  AcceptableAppsState *tmpAcceptableAppsState = new AcceptableAppsState( mySequence );
  tmpAcceptableAppsState->setData( whichTimeline->GetMyWindow()->getTrace()->getSuitableApps() );
  mySequence->addState( TSequenceStates::acceptableAppsState, tmpAcceptableAppsState );

  TraceOptions *tmpOptions = TraceOptions::create( myKernel );
  tmpOptions->set_by_time( true );
  tmpOptions->set_min_cutting_time( whichTimeline->GetMyWindow()->getWindowBeginTime() );
  tmpOptions->set_max_cutting_time( whichTimeline->GetMyWindow()->getWindowEndTime() );
  tmpOptions->set_original_time( false );
  tmpOptions->set_break_states( false );

  TraceOptionsState *tmpOptionsState = new TraceOptionsState( mySequence );
  tmpOptionsState->setData( tmpOptions );
  mySequence->addState( TSequenceStates::traceOptionsState, tmpOptionsState );

  SourceTimelineState *tmpWindowState = new SourceTimelineState( mySequence );
  tmpWindowState->setData( whichTimeline->GetMyWindow() );
  mySequence->addState( TSequenceStates::sourceTimelineState, tmpWindowState );

  std::string tmpFileName;
  wxFileName tmpTraceName( wxString::FromUTF8( whichTimeline->GetMyWindow()->getTrace()->getFileName().c_str() ) );
  tmpTraceName.ClearExt();
  tmpTraceName.AppendDir( wxString::FromUTF8( TraceEditSequence::dirNameDimemas.c_str() ) );
  
  if( !tmpTraceName.DirExists() )
    tmpTraceName.Mkdir();
  
  OutputDirSuffixState *tmpOutputDirSuffixState = new OutputDirSuffixState( mySequence );
  tmpOutputDirSuffixState->setData( TraceEditSequence::dirNameDimemas );
  mySequence->addState( TSequenceStates::outputDirSuffixState, tmpOutputDirSuffixState );
  
  vector<std::string> traces;
  traces.push_back( whichTimeline->GetMyWindow()->getTrace()->getFileName() );
  mySequence->execute( traces );
  
  delete mySequence;
}


void SequenceDriver::sequenceFolding( gTimeline *whichTimeline )
{
  KernelConnection *myKernel =  whichTimeline->GetMyWindow()->getKernel();
  TraceEditSequence *mySequence = TraceEditSequence::create( myKernel );

  mySequence->pushbackAction( TSequenceActions::csvOutputAction );
  mySequence->pushbackAction( TSequenceActions::traceCutterAction );
  mySequence->pushbackAction( new RunAppFoldingAction( mySequence ) );
  
  AcceptableAppsState *tmpAcceptableAppsState = new AcceptableAppsState( mySequence );
  tmpAcceptableAppsState->setData( whichTimeline->GetMyWindow()->getTrace()->getSuitableApps() );
  mySequence->addState( TSequenceStates::acceptableAppsState, tmpAcceptableAppsState );

  TraceOptions *tmpOptions = TraceOptions::create( myKernel );
  tmpOptions->set_by_time( true );
  tmpOptions->set_min_cutting_time( whichTimeline->GetMyWindow()->getWindowBeginTime() );
  tmpOptions->set_max_cutting_time( whichTimeline->GetMyWindow()->getWindowEndTime() );
  tmpOptions->set_original_time( false );
  tmpOptions->set_break_states( false );

  TraceOptionsState *tmpOptionsState = new TraceOptionsState( mySequence );
  tmpOptionsState->setData( tmpOptions );
  mySequence->addState( TSequenceStates::traceOptionsState, tmpOptionsState );
  
  TextOutput output;
  output.setWindowTimeUnits( false );
  output.setTextualSemantic( true );
  CSVOutputState *tmpOutputState = new CSVOutputState( mySequence );
  tmpOutputState->setData( output );
  mySequence->addState( TSequenceStates::csvOutputState, tmpOutputState );
  
  SourceTimelineState *tmpWindowState = new SourceTimelineState( mySequence );
  tmpWindowState->setData( whichTimeline->GetMyWindow() );
  mySequence->addState( TSequenceStates::sourceTimelineState, tmpWindowState );

  CSVFileNameState *tmpCSVFilenameState = new CSVFileNameState( mySequence );
  std::string tmpFileName;
  wxFileName tmpTraceName( wxString::FromUTF8( whichTimeline->GetMyWindow()->getTrace()->getFileName().c_str() ) );
  tmpTraceName.ClearExt();
  tmpTraceName.AppendDir( wxString::FromUTF8( TraceEditSequence::dirNameFolding.c_str() ) );
  
  if( !tmpTraceName.DirExists() )
    tmpTraceName.Mkdir();
  std::string auxName = whichTimeline->GetMyWindow()->getName() + "_";
  tmpFileName = std::string( tmpTraceName.GetPath( wxPATH_GET_SEPARATOR ).mb_str() ) +
                auxName.c_str() + std::string( tmpTraceName.GetFullName().mb_str() ) +
                std::string( ".csv" );

  tmpCSVFilenameState->setData( tmpFileName );
  mySequence->addState( TSequenceStates::csvFileNameState, tmpCSVFilenameState );

  OutputDirSuffixState *tmpOutputDirSuffixState = new OutputDirSuffixState( mySequence );
  tmpOutputDirSuffixState->setData( TraceEditSequence::dirNameFolding );
  mySequence->addState( TSequenceStates::outputDirSuffixState, tmpOutputDirSuffixState );
  
  vector<std::string> traces;
  traces.push_back( whichTimeline->GetMyWindow()->getTrace()->getFileName() );
  mySequence->execute( traces );
  
  delete mySequence;
}


void SequenceDriver::sequenceMess( gTimeline *whichTimeline )
{
  // Create sequence
  KernelConnection *myKernel =  whichTimeline->GetMyWindow()->getKernel();
  TraceEditSequence *mySequence = TraceEditSequence::create( myKernel );

  // Define sequence
  mySequence->pushbackAction( TSequenceActions::traceCutterAction );
  mySequence->pushbackAction( new RunMessAction( mySequence ) );

  AcceptableAppsState *tmpAcceptableAppsState = new AcceptableAppsState( mySequence );
  tmpAcceptableAppsState->setData( whichTimeline->GetMyWindow()->getTrace()->getSuitableApps() );
  mySequence->addState( TSequenceStates::acceptableAppsState, tmpAcceptableAppsState );

  // Trace options state
  TraceOptions *tmpOptions = TraceOptions::create( myKernel );
  tmpOptions->set_by_time( true );
  tmpOptions->set_min_cutting_time( whichTimeline->GetMyWindow()->getWindowBeginTime() );
  tmpOptions->set_max_cutting_time( whichTimeline->GetMyWindow()->getWindowEndTime() );
  tmpOptions->set_original_time( false );
  tmpOptions->set_break_states( false );
  tmpOptions->set_remFirstStates( false );
  tmpOptions->set_remLastStates( true );
  tmpOptions->set_keep_all_events( true );
  tmpOptions->set_max_cut_time_to_finish_of_first_appl( true );
  TraceOptionsState *tmpOptionsState = new TraceOptionsState( mySequence );
  tmpOptionsState->setData( tmpOptions );
  mySequence->addState( TSequenceStates::traceOptionsState, tmpOptionsState );
  
  SourceTimelineState *tmpWindowState = new SourceTimelineState( mySequence );
  tmpWindowState->setData( whichTimeline->GetMyWindow() );
  mySequence->addState( TSequenceStates::sourceTimelineState, tmpWindowState );

  // Output dir: subdir mess
  std::string tmpFileName;
  wxFileName tmpTraceName( wxString::FromUTF8( whichTimeline->GetMyWindow()->getTrace()->getFileName().c_str() ) );
  tmpTraceName.ClearExt();
  tmpTraceName.AppendDir( wxString::FromUTF8( TraceEditSequence::dirNameMess.c_str() ) );  
  if( !tmpTraceName.DirExists() )
    tmpTraceName.Mkdir();
  
  // Mess suffix
  OutputDirSuffixState *tmpOutputDirSuffixState = new OutputDirSuffixState( mySequence );
  tmpOutputDirSuffixState->setData( TraceEditSequence::dirNameMess );
  mySequence->addState( TSequenceStates::outputDirSuffixState, tmpOutputDirSuffixState );

  // Engage sequence
  vector<std::string> traces;
  traces.push_back( whichTimeline->GetMyWindow()->getTrace()->getFileName() );
  mySequence->execute( traces );
  
  delete mySequence;
}


void SequenceDriver::sequenceUserCommand( gTimeline *whichTimeline )
{
  KernelConnection *myKernel =  whichTimeline->GetMyWindow()->getKernel();
  TraceEditSequence *mySequence = TraceEditSequence::create( myKernel );

  mySequence->pushbackAction( TSequenceActions::traceCutterAction );
  mySequence->pushbackAction( new RunAppUserCommandAction( mySequence ) );
  
  AcceptableAppsState *tmpAcceptableAppsState = new AcceptableAppsState( mySequence );
  tmpAcceptableAppsState->setData( whichTimeline->GetMyWindow()->getTrace()->getSuitableApps() );
  mySequence->addState( TSequenceStates::acceptableAppsState, tmpAcceptableAppsState );

  TraceOptions *tmpOptions = TraceOptions::create( myKernel );
  tmpOptions->set_by_time( true );
  tmpOptions->set_min_cutting_time( whichTimeline->GetMyWindow()->getWindowBeginTime() );
  tmpOptions->set_max_cutting_time( whichTimeline->GetMyWindow()->getWindowEndTime() );
  tmpOptions->set_original_time( false );
  tmpOptions->set_break_states( false );
//  tmpOptions->set_remLastStates( true );
//  tmpOptions->set_keep_boundary_events( true );

  TraceOptionsState *tmpOptionsState = new TraceOptionsState( mySequence );
  tmpOptionsState->setData( tmpOptions );
  mySequence->addState( TSequenceStates::traceOptionsState, tmpOptionsState );

  SourceTimelineState *tmpWindowState = new SourceTimelineState( mySequence );
  tmpWindowState->setData( whichTimeline->GetMyWindow() );
  mySequence->addState( TSequenceStates::sourceTimelineState, tmpWindowState );

  std::string tmpFileName;
  wxFileName tmpTraceName( wxString::FromUTF8( whichTimeline->GetMyWindow()->getTrace()->getFileName().c_str() ) );
  tmpTraceName.ClearExt();
  tmpTraceName.AppendDir( wxString::FromUTF8( TraceEditSequence::dirNameUserCommand.c_str() ) );
  
  if( !tmpTraceName.DirExists() )
    tmpTraceName.Mkdir();

  OutputDirSuffixState *tmpOutputDirSuffixState = new OutputDirSuffixState( mySequence );
  tmpOutputDirSuffixState->setData( TraceEditSequence::dirNameUserCommand );
  mySequence->addState( TSequenceStates::outputDirSuffixState, tmpOutputDirSuffixState );

  vector<std::string> traces;
  traces.push_back( whichTimeline->GetMyWindow()->getTrace()->getFileName() );
  mySequence->execute( traces );
  
  delete mySequence;
}

