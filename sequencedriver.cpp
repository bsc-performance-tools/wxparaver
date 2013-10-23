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

#include <string>
#include <wx/filename.h>
#include "sequencedriver.h"
#include "kernelconnection.h"
#include "gtimeline.h"
#include "traceeditsequence.h"
#include "traceeditstates.h"
#include "traceoptions.h"

void SequenceDriver::sequenceClustering( gTimeline *whichTimeline )
{
  KernelConnection *myKernel =  whichTimeline->GetMyWindow()->getKernel();
  TraceEditSequence *mySequence = TraceEditSequence::create( myKernel );

  mySequence->pushbackAction( TraceEditSequence::traceCutterAction );
  mySequence->pushbackAction( TraceEditSequence::csvOutputAction );
  
  TraceOptions *tmpOptions = TraceOptions::create( myKernel );
  tmpOptions->set_by_time( true );
  tmpOptions->set_min_cutting_time( whichTimeline->GetMyWindow()->getWindowBeginTime() );
  tmpOptions->set_max_cutting_time( whichTimeline->GetMyWindow()->getWindowEndTime() );
  tmpOptions->set_original_time( true );
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
  tmpFileName = std::string( tmpTraceName.GetPath( wxPATH_GET_SEPARATOR ).mb_str() ) + auxName.c_str() + std::string( tmpTraceName.GetFullName().mb_str() );
  tmpCSVFilenameState->setData( tmpFileName );
  mySequence->addState( TraceEditSequence::csvFileNameState, tmpCSVFilenameState );
  
  vector<std::string> traces;
  traces.push_back( whichTimeline->GetMyWindow()->getTrace()->getFileName() );
  mySequence->execute( traces );
  
  delete mySequence;
}
