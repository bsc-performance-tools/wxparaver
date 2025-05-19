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
 ********                 RunProfetAction                            ********
 ****************************************************************************/
vector<TSequenceStates> RunProfetAction::getStateDependencies() const
{
  vector<TSequenceStates> tmpStates;
  return tmpStates;
}

bool RunProfetAction::execute( std::string whichTrace )
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
  runAppDialog->setProfet();
  
  runAppDialog->Show();
  runAppDialog->Raise();
  
  return errorFound;
}


/****************************************************************************
 ********                 SetCSVFileNameRunCarmDialogAction          ********
 ****************************************************************************/
vector<TSequenceStates> SetCSVFileNameRunCarmDialogAction::getStateDependencies() const
{
  return {};
}

bool SetCSVFileNameRunCarmDialogAction::execute( std::string whichTrace )
{
  TraceEditSequence *tmpSequence = (TraceEditSequence *)mySequence;
  
  // Cut has been done => build new basename using new chop.
  Timeline *sourceWindow = ( ( SourceTimelineState * )tmpSequence->getState( TSequenceStates::sourceTimelineState ) )->getData();

  std::string tmpOutputTraceFileName = ( ( OutputTraceFileNameState * )tmpSequence->getState( TSequenceStates::outputTraceFileNameState ) )->getData();
  wxFileName tmpTraceName( wxString::FromUTF8( tmpOutputTraceFileName.c_str() ) );
  tmpTraceName.ClearExt();
  tmpTraceName.AppendDir( wxString::FromUTF8( TraceEditSequence::dirNameCarm.c_str() ) );

  std::string auxName = sourceWindow->getName() + "_";
  std::replace( auxName.begin(), auxName.end(), ',', '-' );
  std::replace( auxName.begin(), auxName.end(), ' ', '_' );
  std::string tmpFileBaseName = std::string( tmpTraceName.GetPath( wxPATH_GET_SEPARATOR ).mb_str() ) + auxName.c_str() + std::string( tmpTraceName.GetFullName().mb_str() );

  // Rewrite .csv name in state
  std::string tmpFileName = tmpFileBaseName + std::string( ".csv" );
  CSVFileNameState *tmpCSVFilenameState = (CSVFileNameState *)tmpSequence->getState( TSequenceStates::csvFileNameState );
  tmpCSVFilenameState->setData( tmpFileName );

  // Rewrite new .legend.csv name
  tmpFileName = tmpFileBaseName + std::string( ".legend.csv" );
  CSVLegendFileNameState *tmpCSVLegendFilenameState = (CSVLegendFileNameState *)tmpSequence->getState( TSequenceStates::csvLegendFileNameState );
  tmpCSVLegendFilenameState->setData( tmpFileName );


  tmpSequence->executeNextAction( whichTrace );

  return false;
}


/****************************************************************************
 ********                 CreateTimelineCarmAction                   ********
 ****************************************************************************/
vector<TSequenceStates> CreateTimelineCarmAction::getStateDependencies() const
{
  return {};
}

bool CreateTimelineCarmAction::execute( std::string whichTrace )
{
  bool errorFound = true;

  TraceEditSequence *tmpSequence = (TraceEditSequence *)mySequence;

  bool usingWholeTrace = ( ( UsingWholeTraceState * )tmpSequence->getState( TSequenceStates::usingWholeTraceState ) )->getData();
  if ( !usingWholeTrace )
  {
    if ( !wxparaverApp::mainWindow->DoLoadTrace( whichTrace ) )
      return errorFound;
  
    // Create copy of source window but using whichTrace.
    Timeline *sourceWindow = ( ( SourceTimelineState * )tmpSequence->getState( TSequenceStates::sourceTimelineState ) )->getData();
    std::vector<Trace *> loadedTraces = wxparaverApp::mainWindow->GetLoadedTraces();
    SaveOptions saveOptions = SaveOptions();
    saveOptions.windowBeginTimeRelative = false;
    saveOptions.windowScaleRelative = false;
    std::vector<Timeline *> windows = { sourceWindow };
    std::vector<Histogram *> dummyHistograms;
    std::vector<CFGS4DLinkedPropertiesManager> linkedProperties;

    std::stringstream tmpCFG;
    if ( !CFGLoader::saveCFG( tmpCFG, saveOptions, windows, dummyHistograms, linkedProperties ) )
      return errorFound;

    windows.clear();
    bool overrideChecks = true;
    if ( !CFGLoader::loadCFG( sourceWindow->getKernel(),
                              loadedTraces[ wxparaverApp::mainWindow->GetCurrentTrace() ],
                              tmpCFG,
                              windows,
                              dummyHistograms, 
                              saveOptions,
                              overrideChecks ) )
      return errorFound;

    // Rewrite source timeline with currently generated timeline.
    for( auto& it : windows )
    {
      if ( it->getChild() == nullptr )
      {
        SourceTimelineState *sourceTimelineState = ( SourceTimelineState * )tmpSequence->getState( TSequenceStates::sourceTimelineState );
        sourceTimelineState->setData( it );
      }
    }
  }

  tmpSequence->executeNextAction( whichTrace );

  return false;
}


/****************************************************************************
 ********                 RunCarmAction                              ********
 ****************************************************************************/
vector<TSequenceStates> RunCarmAction::getStateDependencies() const
{
  return {};
}

bool RunCarmAction::execute( std::string whichTrace )
{
  TraceEditSequence *tmpSequence = (TraceEditSequence *)mySequence;

  RunScript *runAppDialog = wxparaverApp::mainWindow->GetRunApplication();
  if( runAppDialog == nullptr )
  {
    std::vector<bool> tmpAcceptableApps = ( ( AcceptableAppsState * )tmpSequence->getState( TSequenceStates::acceptableAppsState ) )->getData();
    runAppDialog = new RunScript( wxparaverApp::mainWindow, tmpAcceptableApps );
    wxparaverApp::mainWindow->SetRunApplication( runAppDialog );
  }

  runAppDialog->setTrace( wxString::FromUTF8( whichTrace.c_str() ) );

  bool usingWholeTrace = ( ( UsingWholeTraceState * )tmpSequence->getState( TSequenceStates::usingWholeTraceState ) )->getData();
  if ( !usingWholeTrace )
  {
    std::vector< Trace * > loadedTraces = wxparaverApp::mainWindow->GetLoadedTraces();
    Timeline *window = ( ( SourceTimelineState * )tmpSequence->getState( TSequenceStates::sourceTimelineState ) )->getData();

    wxTreeCtrl *allTracesPage = (wxTreeCtrl *)wxparaverApp::mainWindow->choiceWindowBrowser->GetPage( 0 );
    wxTreeCtrl *currentPage = (wxTreeCtrl *)wxparaverApp::mainWindow->choiceWindowBrowser->GetPage( wxparaverApp::mainWindow->getTracePosition( loadedTraces[ wxparaverApp::mainWindow->GetCurrentTrace() ] ) + 1 );
    window->setPosX( window->getPosX() + paraverMain::defaultTitleBarSize.y );
    window->setPosY( window->getPosY() + paraverMain::defaultTitleBarSize.y );
    BuildTree( wxparaverApp::mainWindow, allTracesPage, allTracesPage->GetRootItem(), currentPage, currentPage->GetRootItem(), window );
  }

  std::string tmpCSVFileName       = ( (CSVFileNameState *)tmpSequence->getState( TSequenceStates::csvFileNameState ) )->getData();
  std::string tmpCSVLabelsFileName = ( (CSVLegendFileNameState *)tmpSequence->getState( TSequenceStates::csvLegendFileNameState ) )->getData();
  runAppDialog->setCarm( wxString::FromUTF8( tmpCSVFileName.c_str() ), wxString::FromUTF8( tmpCSVLabelsFileName.c_str() ) );
  
  runAppDialog->Show();
  runAppDialog->Raise();

  return false;
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


void SequenceDriver::sequenceProfet( gTimeline *whichTimeline )
{
  // Create sequence
  KernelConnection *myKernel =  whichTimeline->GetMyWindow()->getKernel();
  TraceEditSequence *mySequence = TraceEditSequence::create( myKernel );

  // Define sequence
  mySequence->pushbackAction( TSequenceActions::traceCutterAction );
  mySequence->pushbackAction( new RunProfetAction( mySequence ) );

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

  // Output dir: subdir profet
  std::string tmpFileName;
  wxFileName tmpTraceName( wxString::FromUTF8( whichTimeline->GetMyWindow()->getTrace()->getFileName().c_str() ) );
  tmpTraceName.ClearExt();
  tmpTraceName.AppendDir( wxString::FromUTF8( TraceEditSequence::dirNameProfet.c_str() ) );  
  if( !tmpTraceName.DirExists() )
    tmpTraceName.Mkdir();
  
  // Profet suffix
  OutputDirSuffixState *tmpOutputDirSuffixState = new OutputDirSuffixState( mySequence );
  tmpOutputDirSuffixState->setData( TraceEditSequence::dirNameProfet );
  mySequence->addState( TSequenceStates::outputDirSuffixState, tmpOutputDirSuffixState );

  // Engage sequence
  vector<std::string> traces;
  traces.push_back( whichTimeline->GetMyWindow()->getTrace()->getFileName() );
  mySequence->execute( traces );
  
  delete mySequence;
}


void SequenceDriver::sequenceCarm( gTimeline *whichTimeline )
{
  // Create sequence
  KernelConnection *myKernel = whichTimeline->GetMyWindow()->getKernel();
  TraceEditSequence *mySequence = TraceEditSequence::create( myKernel );

  // Define sequence
  bool usingWholeTrace = ( whichTimeline->GetMyWindow()->getWindowBeginTime() == 0 ) &&
                         ( whichTimeline->GetMyWindow()->getWindowEndTime() == whichTimeline->GetMyWindow()->getTrace()->getEndTime() );
  if ( !usingWholeTrace )
  {
    mySequence->pushbackAction( TSequenceActions::traceCutterAction );
    mySequence->pushbackAction( new SetCSVFileNameRunCarmDialogAction( mySequence ) );
  }
  mySequence->pushbackAction( new CreateTimelineCarmAction( mySequence ) );
  mySequence->pushbackAction( TSequenceActions::csvOutputAction );
  mySequence->pushbackAction( new RunCarmAction( mySequence ) );

  // States for TSequenceActions::traceCutterAction
  TraceOptions *tmpOptions = TraceOptions::create( myKernel );
  tmpOptions->set_min_cutting_time( whichTimeline->GetMyWindow()->getWindowBeginTime() );
  tmpOptions->set_max_cutting_time( whichTimeline->GetMyWindow()->getWindowEndTime() );
  if ( !usingWholeTrace )
  {
    tmpOptions->set_by_time( true );
    tmpOptions->set_original_time( false );
    tmpOptions->set_break_states( false );
    tmpOptions->set_keep_boundary_events( true );
    tmpOptions->set_remFirstStates( false );
    tmpOptions->set_remLastStates( false );
    tmpOptions->set_keep_all_events( true );
  }
  TraceOptionsState *tmpOptionsState = new TraceOptionsState( mySequence );
  tmpOptionsState->setData( tmpOptions );
  mySequence->addState( TSequenceStates::traceOptionsState, tmpOptionsState );

  OutputDirSuffixState *tmpOutputDirSuffixState = new OutputDirSuffixState( mySequence );
  tmpOutputDirSuffixState->setData( TraceEditSequence::dirNameCarm );
  mySequence->addState( TSequenceStates::outputDirSuffixState, tmpOutputDirSuffixState );

  // States for RunCarmAction
  AcceptableAppsState *tmpAcceptableAppsState = new AcceptableAppsState( mySequence );
  tmpAcceptableAppsState->setData( whichTimeline->GetMyWindow()->getTrace()->getSuitableApps() );
  mySequence->addState( TSequenceStates::acceptableAppsState, tmpAcceptableAppsState );

  UsingWholeTraceState *tmpUsingWholeTraceState = new UsingWholeTraceState( mySequence );
  tmpUsingWholeTraceState->setData( usingWholeTrace );
  mySequence->addState( TSequenceStates::usingWholeTraceState, tmpUsingWholeTraceState );

  OutputTraceFileNameState *tmpOutputTraceFileNameState = new OutputTraceFileNameState( mySequence );
  tmpOutputTraceFileNameState->setData( "" );
  mySequence->addState( TSequenceStates::outputTraceFileNameState, tmpOutputTraceFileNameState );

  // States for RunCarm and TSequenceActions::csvOutputAction
  SourceTimelineState *tmpWindowState = new SourceTimelineState( mySequence );
  tmpWindowState->setData( whichTimeline->GetMyWindow() );
  mySequence->addState( TSequenceStates::sourceTimelineState, tmpWindowState );

  // States for TSequenceActions::csvOutputAction 
  TextOutput output;
  output.setWindowTimeUnits( true );
  CSVOutputState *tmpOutputState = new CSVOutputState( mySequence );
  tmpOutputState->setData( output );
  mySequence->addState( TSequenceStates::csvOutputState, tmpOutputState );

  CSVFileNameState *tmpCSVFilenameState = new CSVFileNameState( mySequence );
  wxFileName tmpTraceName( wxString::FromUTF8( whichTimeline->GetMyWindow()->getTrace()->getFileName().c_str() ) );
  tmpTraceName.ClearExt();
  tmpTraceName.AppendDir( wxString::FromUTF8( TraceEditSequence::dirNameCarm.c_str() ) );
  if( !tmpTraceName.DirExists() )
    tmpTraceName.Mkdir();
  std::string auxName = whichTimeline->GetMyWindow()->getName() + "_";
  std::replace( auxName.begin(), auxName.end(), ',', '-' );
  std::replace( auxName.begin(), auxName.end(), ' ', '_' );
  std::string tmpFileBaseName = std::string( tmpTraceName.GetPath( wxPATH_GET_SEPARATOR ).mb_str() ) + auxName.c_str() + std::string( tmpTraceName.GetFullName().mb_str() );
  std::string tmpFileName = tmpFileBaseName + std::string( ".csv" );
  tmpCSVFilenameState->setData( tmpFileName );
  mySequence->addState( TSequenceStates::csvFileNameState, tmpCSVFilenameState );

  CSVLegendFileNameState *tmpCSVLegendFilenameState = new CSVLegendFileNameState( mySequence );
  tmpFileName = tmpFileBaseName + std::string( ".legend.csv" );
  tmpCSVLegendFilenameState->setData( tmpFileName );
  mySequence->addState( TSequenceStates::csvLegendFileNameState, tmpCSVLegendFilenameState );  

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

