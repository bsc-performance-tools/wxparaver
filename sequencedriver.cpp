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
  tmpFileName = std::string( tmpTraceName.GetPath( wxPATH_GET_SEPARATOR ).mb_str() ) + auxName.c_str() + std::string( tmpTraceName.GetFullName().mb_str() ) + std::string( ".csv" );

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

