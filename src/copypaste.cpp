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


#include "copypaste.h"
#include "trace.h"
#include "filter.h"

using namespace std;

constexpr size_t SAME_TRACE = 0;
constexpr size_t DIFF_TRACE = 1;
constexpr size_t TIMELINE = 0;
constexpr size_t HISTOGRAM = 1;

gPasteWindowProperties* gPasteWindowProperties::pasteWindowProperties = nullptr;


void gPasteWindowProperties::commonMenuSettings( )
{
  for ( int trace = SAME_TRACE; trace <= DIFF_TRACE; trace++ )
    for ( int origin = TIMELINE; origin <= HISTOGRAM; origin++ )
      for ( int destiny = TIMELINE; destiny <= HISTOGRAM; destiny++ )
      {
        allowed[STR_PASTE][trace][origin][destiny] = true;
        allowed[STR_PASTE_DEFAULT_SPECIAL][trace][origin][destiny] = true;
        allowed[STR_PASTE_SPECIAL][trace][origin][destiny] = true;
      }

  // Timeline/sourceHistogram different Menu properties
  if ( sourceTimeline != nullptr )
  {
  }
  else
  {
  }
}


void gPasteWindowProperties::commonTimeSettings( TRecordTime destinyEndTime )
{
  TRecordTime sourceBeginTime;
  int source;

  if ( sourceTimeline != nullptr )
  {
    sourceBeginTime = sourceTimeline->GetMyWindow()->getWindowBeginTime();
    source          = TIMELINE;
  }
  else
  {
    sourceBeginTime = sourceHistogram->GetHistogram()->getBeginTime();
    source          = HISTOGRAM;
  }

  if ( sourceBeginTime > destinyEndTime )
  {
    for( int trace = SAME_TRACE; trace <= DIFF_TRACE; trace++ )
      for( int destiny = TIMELINE; destiny <= HISTOGRAM; destiny++ )
        allowed[STR_TIME][trace][source][destiny] = false;
  }
  else
  {
    for( int trace = SAME_TRACE; trace <= DIFF_TRACE; trace++ )
      for( int destiny = TIMELINE; destiny <= HISTOGRAM; destiny++ )
        allowed[STR_TIME][trace][source][destiny] = true;
  }
}

void gPasteWindowProperties::commonFilterSettings( gTimeline *destinyTimeline )
{
  if ( sourceTimeline != nullptr )
  {
    if ( sourceTimeline->GetMyWindow()->isDerivedWindow() ||
         destinyTimeline->GetMyWindow()->isDerivedWindow() )
    {
      for( int trace = SAME_TRACE; trace <= DIFF_TRACE; trace++ )
      {
        allowed[STR_FILTER][trace][TIMELINE][TIMELINE] = false;
        allowed[STR_FILTER_ALL][trace][TIMELINE][TIMELINE] = false;
        allowed[STR_FILTER_COMMS][trace][TIMELINE][TIMELINE] = false;
        allowed[STR_FILTER_EVENTS][trace][TIMELINE][TIMELINE] = false;
      }
    }
    else
    {
      for( int trace = SAME_TRACE; trace <= DIFF_TRACE; trace++ )
      {
        allowed[STR_FILTER][trace][TIMELINE][TIMELINE] = true;
        allowed[STR_FILTER_ALL][trace][TIMELINE][TIMELINE] = true;
        allowed[STR_FILTER_COMMS][trace][TIMELINE][TIMELINE] = true;
        allowed[STR_FILTER_EVENTS][trace][TIMELINE][TIMELINE] = true;
      }
    }
  }
}

bool gPasteWindowProperties::seekAllowed( const string property, int destiny, gTimeline *destinyTimeline )
{
  Trace *destinyTrace = destinyTimeline->GetMyWindow()->getTrace();

  if ( sourceTimeline != nullptr )
  {
    bool isProcessModel = sourceTimeline->GetMyWindow()->isLevelProcessModel();
    if ( sourceTimeline->GetMyWindow()->getTrace() == destinyTrace ||
         ( property == STR_OBJECTS &&
           Timeline::compatibleLevels( sourceTimeline->GetMyWindow(), destinyTimeline->GetMyWindow() ) &&
           sourceTimeline->GetMyWindow()->getTrace()->isSubsetObjectStruct( destinyTrace, isProcessModel ) )
       )
      return allowed[property][SAME_TRACE][TIMELINE][destiny];
    else
      return allowed[property][DIFF_TRACE][TIMELINE][destiny];
  }
  else
  {
    bool isProcessModel = sourceHistogram->GetHistogram()->getControlWindow()->isLevelProcessModel();
    if ( sourceHistogram->GetHistogram()->getControlWindow()->getTrace() == destinyTrace ||
         ( property == STR_OBJECTS &&
//           Timeline::compatibleLevels( sourceHistogram->GetHistogram()->getControlWindow(), destinyTimeline->GetMyWindow() ) &&
           sourceHistogram->GetHistogram()->getControlWindow()->getLevel() == destinyTimeline->GetMyWindow()->getLevel() &&
           sourceHistogram->GetHistogram()->getControlWindow()->getTrace()->isSubsetObjectStruct( destinyTrace, isProcessModel ) )
       )
      return allowed[property][SAME_TRACE][HISTOGRAM][destiny];
    else
      return allowed[property][DIFF_TRACE][HISTOGRAM][destiny];
  }
}


bool gPasteWindowProperties::seekAllowed( const string property, int destiny, gHistogram *destinyHistogram )
{
  Trace *destinyTrace = destinyHistogram->GetHistogram()->getControlWindow()->getTrace();

  if ( sourceTimeline != nullptr )
  {
    bool isProcessModel = sourceTimeline->GetMyWindow()->isLevelProcessModel();
    if ( sourceTimeline->GetMyWindow()->getTrace() == destinyTrace ||
         ( property == STR_OBJECTS &&
           //Timeline::compatibleLevels( sourceTimeline->GetMyWindow(), destinyHistogram->GetHistogram()->getControlWindow() ) &&
           sourceTimeline->GetMyWindow()->getLevel() == destinyHistogram->GetHistogram()->getControlWindow()->getLevel() &&
           sourceTimeline->GetMyWindow()->getTrace()->isSubsetObjectStruct( destinyTrace, isProcessModel ) )
       )
      return allowed[property][SAME_TRACE][TIMELINE][destiny];
    else
      return allowed[property][DIFF_TRACE][TIMELINE][destiny];
  }
  else
  {
    bool isProcessModel = sourceHistogram->GetHistogram()->getControlWindow()->isLevelProcessModel();
    if ( sourceHistogram->GetHistogram()->getControlWindow()->getTrace() == destinyTrace ||
         ( property == STR_OBJECTS &&
           //Timeline::compatibleLevels( sourceHistogram->GetHistogram()->getControlWindow(), destinyHistogram->GetHistogram()->getControlWindow() ) &&
           sourceHistogram->GetHistogram()->getControlWindow()->getLevel() == destinyHistogram->GetHistogram()->getControlWindow()->getLevel() &&
           sourceHistogram->GetHistogram()->getControlWindow()->getTrace()->isSubsetObjectStruct( destinyTrace, isProcessModel ) )
       )
      return allowed[property][SAME_TRACE][HISTOGRAM][destiny];
    else
      return allowed[property][DIFF_TRACE][HISTOGRAM][destiny];
  }
}


gPasteWindowProperties::gPasteWindowProperties()
{
  sourceTimeline = nullptr;
  sourceHistogram = nullptr;

  vector< bool > destiny( 2 , false );
  vector < vector< bool > > source( 2, destiny );
  vector< vector < vector< bool > > > option( 2, source );

  // Policy : Allow paste option between different kind of windows and different traces
  for ( int trace = SAME_TRACE; trace <= DIFF_TRACE; trace++ )
    for ( int copy = TIMELINE; copy <= HISTOGRAM; copy++ )
      for ( int paste = TIMELINE; paste <= HISTOGRAM; paste++ )
        option[trace][copy][paste] = true;

  allowed[STR_TIME] = option;
  allowed[STR_COPY] = option;
  allowed[STR_SIZE] = option;
  allowed[STR_DURATION] = option;
  allowed[STR_SEMANTIC_SCALE] = option;

  // Policy : Only same trace
  for ( int trace = SAME_TRACE; trace <= DIFF_TRACE; trace++ )
    for ( int copy = TIMELINE; copy <= HISTOGRAM; copy++ )
      for ( int paste = TIMELINE; paste <= HISTOGRAM; paste++ )
        if ( trace == SAME_TRACE )
          option[trace][copy][paste] = true;
        else
          option[trace][copy][paste] = false;
  allowed[STR_OBJECTS] = option;

  // Policy : Don't allow initial paste
  for ( int trace = SAME_TRACE; trace <= DIFF_TRACE; trace++ )
    for ( int copy = TIMELINE; copy <= HISTOGRAM; copy++ )
      for ( int paste = TIMELINE; paste <= HISTOGRAM; paste++ )
        option[trace][copy][paste] = false;
  allowed[STR_PASTE] = option;
  allowed[STR_PASTE_DEFAULT_SPECIAL] = option;
  allowed[STR_PASTE_SPECIAL] = option;

  // Policy : Selective paste for FILTER options
  option[SAME_TRACE][TIMELINE][TIMELINE] = true;
  option[SAME_TRACE][TIMELINE][HISTOGRAM] = false;
  option[SAME_TRACE][HISTOGRAM][TIMELINE] = false;
  option[SAME_TRACE][HISTOGRAM][HISTOGRAM] = false;
  option[DIFF_TRACE][TIMELINE][TIMELINE] = true;
  option[DIFF_TRACE][TIMELINE][HISTOGRAM] = false;
  option[DIFF_TRACE][HISTOGRAM][TIMELINE] = false;
  option[DIFF_TRACE][HISTOGRAM][HISTOGRAM] = false;

  allowed[STR_FILTER] = option;
  allowed[STR_FILTER_ALL] = option;
  allowed[STR_FILTER_COMMS] = option;
  allowed[STR_FILTER_EVENTS] = option;
  allowed[STR_CUSTOM_PALETTE] = option;
  
  option[SAME_TRACE][TIMELINE][TIMELINE] = false;
  option[SAME_TRACE][TIMELINE][HISTOGRAM] = false;
  option[SAME_TRACE][HISTOGRAM][TIMELINE] = false;
  option[SAME_TRACE][HISTOGRAM][HISTOGRAM] = true;
  option[DIFF_TRACE][TIMELINE][TIMELINE] = false;
  option[DIFF_TRACE][TIMELINE][HISTOGRAM] = false;
  option[DIFF_TRACE][HISTOGRAM][TIMELINE] = false;
  option[DIFF_TRACE][HISTOGRAM][HISTOGRAM] = true;
  
  allowed[STR_CONTROL_SCALE] = option;
  allowed[STR_CONTROL_DIMENSIONS] = option;
  allowed[STR_3D_SCALE] = option;
}


gPasteWindowProperties::~gPasteWindowProperties()
{
}


gPasteWindowProperties *gPasteWindowProperties::getInstance()
{
  if ( gPasteWindowProperties::pasteWindowProperties == nullptr )
    gPasteWindowProperties::pasteWindowProperties = new gPasteWindowProperties();
  return gPasteWindowProperties::pasteWindowProperties;
}


void gPasteWindowProperties::copy( gTimeline *whichTimeline )
{
  sourceTimeline = whichTimeline;
  sourceHistogram = nullptr;
}


void gPasteWindowProperties::copy( gHistogram *whichHistogram )
{
  sourceTimeline = nullptr;
  sourceHistogram = whichHistogram;
}

void gPasteWindowProperties::paste( gTimeline* destinyTimeline, const string property )
{
  if ( sourceTimeline != nullptr )
  {
    // paste sourceTimeline -> destinyTimeline
    if ( property == STR_TIME )
    {
      TRecordTime sourceBeginTime = sourceTimeline->GetMyWindow()->getWindowBeginTime();
      TRecordTime sourceEndTime   = sourceTimeline->GetMyWindow()->getWindowEndTime();

      sourceBeginTime = destinyTimeline->GetMyWindow()->customUnitsToTraceUnits( sourceBeginTime, sourceTimeline->GetMyWindow()->getTrace()->getTimeUnit() );
      sourceEndTime = destinyTimeline->GetMyWindow()->customUnitsToTraceUnits( sourceEndTime, sourceTimeline->GetMyWindow()->getTrace()->getTimeUnit() );
      
      destinyTimeline->GetMyWindow()->setWindowBeginTime( sourceBeginTime );
      destinyTimeline->GetMyWindow()->setWindowEndTime( sourceEndTime );
    }
    else if ( property == STR_SIZE )
    {
      int width, height;
      width = sourceTimeline->GetMyWindow()->getWidth();
      height = sourceTimeline->GetMyWindow()->getHeight();
      destinyTimeline->resizeDrawZone( width, height );
    }
    else if ( property == STR_OBJECTS )
    {
      vector< TObjectOrder > auxRows;
      TTraceLevel firstLevel;
      TTraceLevel lastLevel;

      if( sourceTimeline->GetMyWindow()->getLevel() >= TTraceLevel::WORKLOAD && sourceTimeline->GetMyWindow()->getLevel() <= TTraceLevel::THREAD )
      {
        firstLevel = TTraceLevel::WORKLOAD;
        lastLevel = TTraceLevel::THREAD;
      }
      else
      {
        firstLevel = TTraceLevel::NODE;
        lastLevel = TTraceLevel::CPU;
      }

      for( TTraceLevel iLevel = firstLevel; iLevel <= lastLevel; ++iLevel )
      {
        sourceTimeline->GetMyWindow()->getSelectedRows( iLevel, auxRows, true );
        destinyTimeline->GetMyWindow()->setSelectedRows( iLevel, auxRows );
      }
      destinyTimeline->GetMyWindow()->addZoom( destinyTimeline->GetMyWindow()->getWindowBeginTime(),
                                               destinyTimeline->GetMyWindow()->getWindowEndTime(),
                                               sourceTimeline->GetMyWindow()->getZoomSecondDimension().first,
                                               sourceTimeline->GetMyWindow()->getZoomSecondDimension().second );
    }
    else if ( property == STR_DURATION )
    {
      TRecordTime sourceBeginTime = sourceTimeline->GetMyWindow()->getWindowBeginTime();
      TRecordTime sourceEndTime   = sourceTimeline->GetMyWindow()->getWindowEndTime();

      sourceBeginTime = destinyTimeline->GetMyWindow()->customUnitsToTraceUnits( sourceBeginTime, sourceTimeline->GetMyWindow()->getTrace()->getTimeUnit() );
      sourceEndTime = destinyTimeline->GetMyWindow()->customUnitsToTraceUnits( sourceEndTime, sourceTimeline->GetMyWindow()->getTrace()->getTimeUnit() );

      TRecordTime sourceDuration  = sourceEndTime - sourceBeginTime;
      TRecordTime newEndTime      = destinyTimeline->GetMyWindow()->getWindowBeginTime() + sourceDuration;

      destinyTimeline->GetMyWindow()->setWindowEndTime( newEndTime );
    }
    else if ( property == STR_FILTER_COMMS )
    {
      destinyTimeline->GetMyWindow()->getFilter()->copyCommunicationsSection( sourceTimeline->GetMyWindow()->getFilter() );
    }
    else if ( property == STR_FILTER_EVENTS )
    {
      destinyTimeline->GetMyWindow()->getFilter()->copyEventsSection( sourceTimeline->GetMyWindow()->getFilter() );
    }
    else if ( property == STR_FILTER_ALL )
    {
      destinyTimeline->GetMyWindow()->getFilter()->copyEventsSection( sourceTimeline->GetMyWindow()->getFilter() );
      destinyTimeline->GetMyWindow()->getFilter()->copyCommunicationsSection( sourceTimeline->GetMyWindow()->getFilter() );
    }
    else if ( property == STR_SEMANTIC_SCALE )
    {
      destinyTimeline->GetMyWindow()->setMinimumY( sourceTimeline->GetMyWindow()->getMinimumY() );
      destinyTimeline->GetMyWindow()->setMaximumY( sourceTimeline->GetMyWindow()->getMaximumY() );
    }
    else if ( property == STR_CUSTOM_PALETTE )
    {
      destinyTimeline->GetMyWindow()->getCodeColor().setCustomPalette( sourceTimeline->GetMyWindow()->getCodeColor().getCustomPalette() );
      destinyTimeline->GetMyWindow()->setUseCustomPalette( true );
    }
    else
    {
    }
  }
  else
  {
    // paste sourceHistogram -> destinyTimeline
    if ( property == STR_TIME )
    {
      TRecordTime sourceBeginTime = sourceHistogram->GetHistogram()->getBeginTime();
      TRecordTime sourceEndTime   = sourceHistogram->GetHistogram()->getEndTime();

      sourceBeginTime = destinyTimeline->GetMyWindow()->customUnitsToTraceUnits( sourceBeginTime, sourceHistogram->GetHistogram()->getTrace()->getTimeUnit() );
      sourceEndTime = destinyTimeline->GetMyWindow()->customUnitsToTraceUnits( sourceEndTime, sourceHistogram->GetHistogram()->getTrace()->getTimeUnit() );

      destinyTimeline->GetMyWindow()->setWindowBeginTime( sourceBeginTime );
      destinyTimeline->GetMyWindow()->setWindowEndTime( sourceEndTime );
    }
    else if ( property == STR_SIZE )
    {
      int width, height;
      sourceHistogram->GetSize( &width, &height);
      destinyTimeline->SetSize( width, height );
    }
    else if ( property == STR_OBJECTS )
    {
      vector< TObjectOrder > auxRows = sourceHistogram->GetHistogram()->getSelectedRows();
      destinyTimeline->GetMyWindow()->setSelectedRows( destinyTimeline->GetMyWindow()->getLevel(), auxRows );

    }
    else if ( property == STR_DURATION )
    {
      TRecordTime sourceBeginTime = sourceHistogram->GetHistogram()->getBeginTime();
      TRecordTime sourceEndTime   = sourceHistogram->GetHistogram()->getEndTime();

      sourceBeginTime = destinyTimeline->GetMyWindow()->customUnitsToTraceUnits( sourceBeginTime, sourceHistogram->GetHistogram()->getTrace()->getTimeUnit() );
      sourceEndTime = destinyTimeline->GetMyWindow()->customUnitsToTraceUnits( sourceEndTime, sourceHistogram->GetHistogram()->getTrace()->getTimeUnit() );

      TRecordTime sourceDuration  = sourceEndTime - sourceBeginTime;
      TRecordTime newEndTime      = destinyTimeline->GetMyWindow()->getWindowBeginTime() + sourceDuration;

      destinyTimeline->GetMyWindow()->setWindowEndTime( newEndTime );
    }
    else if ( property == STR_SEMANTIC_SCALE )
    {
      Timeline *controlWin = sourceHistogram->GetHistogram()->getControlWindow();
      destinyTimeline->GetMyWindow()->setMinimumY( controlWin->getMinimumY() );
      destinyTimeline->GetMyWindow()->setMaximumY( controlWin->getMaximumY() );
    }
    else
    {
      cout << "*** UNKNOWN PROPERTY!!! ***" << endl;
    }
  }
}


void gPasteWindowProperties::paste( gHistogram* destinyHistogram, const string property )
{
  if ( sourceTimeline != nullptr )
  {
    // paste sourceTimeline -> destinyHistogram
    if ( property == STR_TIME )
    {
      destinyHistogram->GetHistogram()->setWindowBeginTime( sourceTimeline->GetMyWindow()->getWindowBeginTime() );
      destinyHistogram->GetHistogram()->setWindowEndTime( sourceTimeline->GetMyWindow()->getWindowEndTime() );
    }
    else if ( property == STR_SIZE )
    {
      int width, height;
      sourceTimeline->GetSize( &width, &height);
      destinyHistogram->SetSize( width, height );
    }
    else if ( property == STR_OBJECTS )
    {
      destinyHistogram->GetHistogram()->addZoom( sourceTimeline->GetMyWindow()->getZoomSecondDimension().first,
                                                 sourceTimeline->GetMyWindow()->getZoomSecondDimension().second );
      
      SelectionManagement< TObjectOrder, TTraceLevel >* tSel = sourceTimeline->GetMyWindow()->getSelectedRows();
      vector< TObjectOrder > auxRows;
      tSel->getSelected( auxRows, sourceTimeline->GetMyWindow()->getLevel() );
      destinyHistogram->GetHistogram()->setSelectedRows( auxRows );
    }
    else if ( property == STR_DURATION )
    {
      TRecordTime sourceBeginTime = sourceTimeline->GetMyWindow()->getWindowBeginTime();
      TRecordTime sourceEndTime   = sourceTimeline->GetMyWindow()->getWindowEndTime();
      TRecordTime sourceDuration  = sourceEndTime - sourceBeginTime;
      TRecordTime newEndTime      = destinyHistogram->GetHistogram()->getBeginTime() + sourceDuration;

      destinyHistogram->GetHistogram()->setWindowEndTime( newEndTime );
    }
    else if ( property == STR_SEMANTIC_SCALE )
    {
      destinyHistogram->GetHistogram()->setMinGradient( sourceTimeline->GetMyWindow()->getMinimumY() );
      destinyHistogram->GetHistogram()->setMaxGradient( sourceTimeline->GetMyWindow()->getMaximumY() );
    }
    else
    {
    }
  }
  else
  {
    // paste histogram -> histogram
    if ( property == STR_TIME )
    {
      destinyHistogram->GetHistogram()->setWindowBeginTime( sourceHistogram->GetHistogram()->getBeginTime() );
      destinyHistogram->GetHistogram()->setWindowEndTime( sourceHistogram->GetHistogram()->getEndTime() );
    }
    else if ( property == STR_SIZE )
    {
      int width, height;
      sourceHistogram->GetSize( &width, &height);
      destinyHistogram->SetSize( width, height );
    }
    else if ( property == STR_OBJECTS )
    {
      destinyHistogram->GetHistogram()->addZoom( sourceHistogram->GetHistogram()->getZoomSecondDimension().first,
                                                 sourceHistogram->GetHistogram()->getZoomSecondDimension().second );
      
      vector< TObjectOrder > auxRows = sourceHistogram->GetHistogram()->getSelectedRows();
      destinyHistogram->GetHistogram()->setSelectedRows( auxRows );
    }
    else if ( property == STR_DURATION )
    {
      TRecordTime sourceBeginTime = sourceHistogram->GetHistogram()->getBeginTime();
      TRecordTime sourceEndTime   = sourceHistogram->GetHistogram()->getEndTime();
      TRecordTime sourceDuration  = sourceEndTime - sourceBeginTime;
      TRecordTime newEndTime      = destinyHistogram->GetHistogram()->getBeginTime() + sourceDuration;

      destinyHistogram->GetHistogram()->setWindowEndTime( newEndTime );
    }
    else if ( property == STR_SEMANTIC_SCALE )
    {
      destinyHistogram->GetHistogram()->setMinGradient( sourceHistogram->GetHistogram()->getMinGradient() );
      destinyHistogram->GetHistogram()->setMaxGradient( sourceHistogram->GetHistogram()->getMaxGradient() );
    }
    else if( property == STR_CONTROL_SCALE )
    {
      Histogram *srcHisto = sourceHistogram->GetHistogram();
      Histogram *dstHisto = destinyHistogram->GetHistogram();
      dstHisto->setControlMin( srcHisto->getControlMin() );
      dstHisto->setControlMax( srcHisto->getControlMax() );
      dstHisto->setControlDelta( srcHisto->getControlDelta() );
    }
    else if( property == STR_3D_SCALE )
    {
      Histogram *srcHisto = sourceHistogram->GetHistogram();
      Histogram *dstHisto = destinyHistogram->GetHistogram();
      if( dstHisto->getExtraControlDelta() == srcHisto->getExtraControlDelta() )
      {
        double planeMin = dstHisto->getPlaneMinValue();
        dstHisto->setExtraControlDelta( srcHisto->getExtraControlDelta() );
        dstHisto->setPlaneMinValue( planeMin );
      }
      else
        dstHisto->setExtraControlDelta( srcHisto->getExtraControlDelta() );
      dstHisto->setExtraControlMin( srcHisto->getExtraControlMin() );
      dstHisto->setExtraControlMax( srcHisto->getExtraControlMax() );
    }
    else if( property == STR_CONTROL_DIMENSIONS )
    {
      Histogram *srcHisto = sourceHistogram->GetHistogram();
      Histogram *dstHisto = destinyHistogram->GetHistogram();
      dstHisto->setControlDelta( srcHisto->getControlDelta() );
      THistogramLimit newMax =
              dstHisto->getControlMin() + ( srcHisto->getControlMax() - srcHisto->getControlMin() );
      dstHisto->setControlMax( newMax );
    }
    else
    {
    }
  }
}


bool gPasteWindowProperties::isAllowed( gTimeline *destinyTimeline, const string property )
{
  if ( sourceTimeline == nullptr && sourceHistogram == nullptr )
    return false;

  /*if ( property == STR_TIME )
    commonTimeSettings( destinyTimeline->GetMyWindow()->getTrace()->getEndTime() );*/
  
  if ( property == STR_FILTER ||
       property == STR_FILTER_ALL ||
       property == STR_FILTER_EVENTS ||
       property == STR_FILTER_COMMS )
    commonFilterSettings( destinyTimeline );

  if( property == STR_CUSTOM_PALETTE && sourceTimeline != nullptr && !sourceTimeline->GetMyWindow()->getCodeColor().existCustomColors() )
    return false;

  commonMenuSettings();

  return seekAllowed( property, TIMELINE, destinyTimeline );
}


bool gPasteWindowProperties::isAllowed( gHistogram *destinyHistogram, const string property )
{

  if ( sourceTimeline == nullptr && sourceHistogram == nullptr )
    return false;

  if ( property == STR_TIME )
    commonTimeSettings( destinyHistogram->GetHistogram()->getControlWindow()->getTrace()->getEndTime() );
    
  commonMenuSettings();

  return seekAllowed( property, HISTOGRAM, destinyHistogram );
}


TRecordTime gPasteWindowProperties::getBeginTime()
{
  if ( sourceTimeline != nullptr )
    return sourceTimeline->GetMyWindow()->getWindowBeginTime();

  return sourceHistogram->GetHistogram()->getBeginTime();
}

TRecordTime gPasteWindowProperties::getEndTime()
{
  if ( sourceTimeline != nullptr )
    return sourceTimeline->GetMyWindow()->getWindowEndTime();

  return sourceHistogram->GetHistogram()->getEndTime();
}

void gPasteWindowProperties::verifyRemove( gTimeline *destinyTimeline )
{
  if( destinyTimeline == sourceTimeline )
    sourceTimeline = nullptr;
}

void gPasteWindowProperties::verifyRemove( gHistogram *destinyHistogram )
{
  if( destinyHistogram == sourceHistogram )
    sourceHistogram = nullptr;
}
