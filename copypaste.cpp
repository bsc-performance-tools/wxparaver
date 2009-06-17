#include "copypaste.h"
#include "trace.h"
#include "filter.h"

//#include <iostream>
//using namespace std;
//IDEA undo copy, undo paste

gPasteWindowProperties* gPasteWindowProperties::pasteWindowProperties = NULL;


void gPasteWindowProperties::commonMenuSettings( )
{
  for ( int trace = SAME_TRACE; trace <= DIFF_TRACE; trace++ )
    for ( int origin = TIMELINE; origin <= HISTOGRAM; origin++ )
      for ( int destiny = TIMELINE; destiny <= HISTOGRAM; destiny++ )
      {
        allowed[STR_PASTE][trace][origin][destiny] = true;
        allowed[STR_PASTE_SPECIAL][trace][origin][destiny] = true;
      }

  // Timeline/histogram different Menu properties
  if ( timeline != NULL )
  {
  }
  else
  {
  }
}


void gPasteWindowProperties::commonTimeSettings( TRecordTime destinyEndTime )
{
  TRecordTime sourceBeginTime, sourceEndTime;
  int source;

  if ( timeline != NULL )
  {
    sourceBeginTime = timeline->GetMyWindow()->getWindowBeginTime();
    sourceEndTime   = timeline->GetMyWindow()->getWindowEndTime();
    source          = TIMELINE;
  }
  else
  {
    sourceBeginTime = histogram->GetHistogram()->getBeginTime();
    sourceEndTime   = histogram->GetHistogram()->getEndTime();
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


void gPasteWindowProperties::commonFilterSettings( gTimeline *whichTimeline )
{
  if ( timeline != NULL )
  {
    if ( timeline->GetMyWindow()->isDerivedWindow() ||
         whichTimeline ->GetMyWindow()->isDerivedWindow() )
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


bool gPasteWindowProperties::seekAllowed( const string property, int destiny, Trace *destinyTrace )
{
  if ( timeline != NULL )
  {
    if ( timeline->GetMyWindow()->getTrace() == destinyTrace )
      return allowed[property][SAME_TRACE][TIMELINE][destiny];
    else
      return allowed[property][DIFF_TRACE][TIMELINE][destiny];
  }
  else
  {
    if ( histogram->GetHistogram()->getControlWindow()->getTrace() == destinyTrace )
      return allowed[property][SAME_TRACE][HISTOGRAM][destiny];
    else
      return allowed[property][DIFF_TRACE][HISTOGRAM][destiny];
  }
}


gPasteWindowProperties::gPasteWindowProperties()
{
  timeline = NULL;
  histogram = NULL;

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
}


gPasteWindowProperties::~gPasteWindowProperties()
{
}


gPasteWindowProperties *gPasteWindowProperties::getInstance()
{
  if ( gPasteWindowProperties::pasteWindowProperties == NULL )
    gPasteWindowProperties::pasteWindowProperties = new gPasteWindowProperties();
  return gPasteWindowProperties::pasteWindowProperties;
}


void gPasteWindowProperties::copy( gTimeline* whichTimeline )
{
  timeline = whichTimeline;
  histogram = NULL;
}


void gPasteWindowProperties::copy( gHistogram* whichHistogram )
{
  timeline = NULL;
  histogram = whichHistogram;
}


void gPasteWindowProperties::paste( gTimeline* whichTimeline,const string property )
{
   TRecordTime destinyTraceBeginTime = 0;
// TRecordTime destinyTraceBeginTime =  whichTimeline->GetMyWindow()->getTrace()->getBeginTime();
   TRecordTime destinyTraceEndTime =  whichTimeline->GetMyWindow()->getTrace()->getEndTime();

  if ( timeline != NULL )
  {
    // paste timeline -> timeline
    if ( property == STR_TIME )
    {
      TRecordTime sourceBeginTime = timeline->GetMyWindow()->getWindowBeginTime();
      TRecordTime sourceEndTime   = timeline->GetMyWindow()->getWindowEndTime();

      if ( sourceBeginTime < destinyTraceBeginTime )
        whichTimeline->GetMyWindow()->setWindowBeginTime( destinyTraceBeginTime );
      else
        whichTimeline->GetMyWindow()->setWindowBeginTime( sourceBeginTime );

      if ( sourceEndTime > destinyTraceEndTime )
        whichTimeline->GetMyWindow()->setWindowEndTime( destinyTraceEndTime );
      else
        whichTimeline->GetMyWindow()->setWindowEndTime( sourceEndTime );
    }
    else if ( property == STR_SIZE )
    {
      int width, height;
      timeline->GetSize( &width, &height);
      whichTimeline->SetSize( width, height );
    }
    else if ( property == STR_OBJECTS )
    {
      vector< bool > auxRows;
      timeline->GetMyWindow()->getSelectedRows( auxRows );
      whichTimeline->GetMyWindow()->setSelectedRows( auxRows );
    }
    else if ( property == STR_FILTER_COMMS )
    {
      whichTimeline->GetMyWindow()->getFilter()->copyCommunicationsSection( timeline->GetMyWindow()->getFilter() );
    }
    else if ( property == STR_FILTER_EVENTS )
    {
      whichTimeline->GetMyWindow()->getFilter()->copyEventsSection( timeline->GetMyWindow()->getFilter() );
    }
    else if ( property == STR_FILTER_ALL )
    {
      whichTimeline->GetMyWindow()->getFilter()->copyEventsSection( timeline->GetMyWindow()->getFilter() );
      whichTimeline->GetMyWindow()->getFilter()->copyCommunicationsSection( timeline->GetMyWindow()->getFilter() );
    }
    else
    {
    }
  }
  else
  {
    // paste histogram -> timeline
    if ( property == STR_TIME )
    {
      TRecordTime sourceBeginTime = histogram->GetHistogram()->getBeginTime();
      TRecordTime sourceEndTime   = histogram->GetHistogram()->getEndTime();

      if ( sourceBeginTime < destinyTraceBeginTime )
        whichTimeline->GetMyWindow()->setWindowBeginTime( destinyTraceBeginTime );
      else
        whichTimeline->GetMyWindow()->setWindowBeginTime( sourceBeginTime );

      if ( sourceEndTime > destinyTraceEndTime )
        whichTimeline->GetMyWindow()->setWindowEndTime( destinyTraceEndTime );
      else
        whichTimeline->GetMyWindow()->setWindowEndTime( sourceEndTime );
    }
    else if ( property == STR_SIZE )
    {
      int width, height;
      histogram->GetSize( &width, &height);
      whichTimeline->SetSize( width, height );
    }
    else if ( property == STR_OBJECTS )
    {
      vector< bool > auxRows;
      histogram->GetHistogram()->getControlWindow()->getSelectedRows( auxRows );
      whichTimeline->GetMyWindow()->setSelectedRows( auxRows );
    }
    else
    {
      cout << "*** UNKNOWN PROPERTY!!! ***" << endl;
    }
  }
}


void gPasteWindowProperties::paste( gHistogram* whichHistogram, const string property )
{
  if ( timeline != NULL )
  {
    // paste timeline -> histogram
    if ( property == STR_TIME )
    {
      whichHistogram->GetHistogram()->setWindowBeginTime( timeline->GetMyWindow()->getWindowBeginTime() );
      whichHistogram->GetHistogram()->setWindowEndTime( timeline->GetMyWindow()->getWindowEndTime() );
    }
    else if ( property == STR_SIZE )
    {
      int width, height;
      timeline->GetSize( &width, &height);
      whichHistogram->SetSize( width, height );
    }
    else if ( property == STR_OBJECTS )
    {
      vector< bool > auxRows;
      timeline->GetMyWindow()->getSelectedRows( auxRows );
      whichHistogram->GetHistogram()->getControlWindow()->setSelectedRows( auxRows );
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
      whichHistogram->GetHistogram()->setWindowBeginTime( histogram->GetHistogram()->getBeginTime() );
      whichHistogram->GetHistogram()->setWindowEndTime( histogram->GetHistogram()->getBeginTime() );
    }
    else if ( property == STR_SIZE )
    {
      int width, height;
      histogram->GetSize( &width, &height);
      whichHistogram->SetSize( width, height );
    }
    else if ( property == STR_OBJECTS )
    {
      vector< bool > auxRows;
      histogram->GetHistogram()->getControlWindow()->getSelectedRows( auxRows );
      whichHistogram->GetHistogram()->getControlWindow()->setSelectedRows( auxRows );
    }
    else
    {
    }
  }
}


bool gPasteWindowProperties::isAllowed( gTimeline *whichTimeline, const string property )
{
  if ( timeline == NULL && histogram == NULL )
    return false;

  if ( property == STR_TIME )
    commonTimeSettings( whichTimeline->GetMyWindow()->getTrace()->getEndTime() );
  
  if ( property == STR_FILTER ||
       property == STR_FILTER_ALL ||
       property == STR_FILTER_EVENTS ||
       property == STR_FILTER_COMMS )
    commonFilterSettings( whichTimeline );

  commonMenuSettings();

  return seekAllowed( property, TIMELINE, whichTimeline->GetMyWindow()->getTrace() );
}


bool gPasteWindowProperties::isAllowed( gHistogram *whichHistogram, const string property )
{

  if ( timeline == NULL && histogram == NULL )
    return false;

  if ( property == STR_TIME )
    commonTimeSettings( whichHistogram->GetHistogram()->getControlWindow()->getTrace()->getEndTime() );
    
  commonMenuSettings();

  return seekAllowed( property, HISTOGRAM, whichHistogram->GetHistogram()->getControlWindow()->getTrace() );
}


TRecordTime gPasteWindowProperties::getBeginTime()
{
  if ( timeline != NULL )
    return timeline->GetMyWindow()->getWindowBeginTime();

  return histogram->GetHistogram()->getBeginTime();
}

TRecordTime gPasteWindowProperties::getEndTime()
{
  if ( timeline != NULL )
    return timeline->GetMyWindow()->getWindowEndTime();

  return histogram->GetHistogram()->getEndTime();
}

