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
#include "sequencedriver.h"
#include "kernelconnection.h"
#include "gtimeline.h"
#include "traceeditsequence.h"
#include "traceeditstates.h"
#include "traceoptions.h"

void SequenceDriver::sequenceCutter( gTimeline *whichTimeline )
{
  KernelConnection *myKernel =  whichTimeline->GetMyWindow()->getKernel();
  TraceEditSequence *mySequence = TraceEditSequence::create( myKernel );

  mySequence->pushbackAction( TraceEditSequence::traceCutterAction );
  TraceOptions *tmpOptions = TraceOptions::create( myKernel );
  
  tmpOptions->set_by_time( true );
  tmpOptions->set_min_cutting_time( whichTimeline->GetMyWindow()->getWindowBeginTime() );
  tmpOptions->set_max_cutting_time( whichTimeline->GetMyWindow()->getWindowEndTime() );
  tmpOptions->set_original_time( true );
  tmpOptions->set_break_states( false );

  TraceOptionsState *tmpOptionsState = new TraceOptionsState( mySequence );
  tmpOptionsState->setData( tmpOptions );
  mySequence->addState( TraceEditSequence::traceOptionsState, tmpOptionsState );
  
  vector<std::string> traces;
  traces.push_back( whichTimeline->GetMyWindow()->getTrace()->getFileName() );
  mySequence->execute( traces );
  
  delete mySequence;
}
