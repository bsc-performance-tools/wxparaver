#include "copypaste.h"
#include "trace.h"
//IDEA undo copy, undo paste

gPasteWindowProperties* gPasteWindowProperties::pasteWindowProperties = NULL;

// PRIVATE 

void gPasteWindowProperties::commonMenuSettings( )
{
  // Common Menu properties

  for ( int trace = SAME_TRACE; trace <= DIFF_TRACE; trace++ )
    for ( int origin = TIMELINE; origin <= HISTOGRAM; origin++ )
      for ( int destiny = TIMELINE; destiny <= HISTOGRAM; destiny++ )
        allowed["Paste"][trace][origin][destiny] = true;

  // Timeline/histogram different Menu properties
  if ( timeline != NULL )
  {
  //  if ()
  
  
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
        allowed["Time"][trace][source][destiny] = allowed["Time"][trace][source][destiny] && false;
  }
  else
  {
    for( int trace = SAME_TRACE; trace <= DIFF_TRACE; trace++ )
      for( int destiny = TIMELINE; destiny <= HISTOGRAM; destiny++ )
        allowed["Time"][trace][source][destiny] = allowed["Time"][trace][source][destiny] || true;
  }
}

bool gPasteWindowProperties::seekAllowedPaste( const string property, int destiny, Trace *destinyTrace )
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
  
  // Something more ingenous?
  vector< bool > destiny( 2 , false );
  vector < vector< bool > > source( 2, destiny );
  vector< vector < vector< bool > > > option( 2, source );

  // Policy : Allow paste times between different kind of windows and different traces
  option[SAME_TRACE][TIMELINE][TIMELINE] = true;
  option[SAME_TRACE][TIMELINE][TIMELINE] = true;
  option[SAME_TRACE][TIMELINE][HISTOGRAM] = true;
  option[SAME_TRACE][HISTOGRAM][TIMELINE] = true;
  option[SAME_TRACE][HISTOGRAM][HISTOGRAM] = true;
  option[DIFF_TRACE][TIMELINE][TIMELINE] = true;
  option[DIFF_TRACE][TIMELINE][HISTOGRAM] = true;
  option[DIFF_TRACE][HISTOGRAM][TIMELINE] = true;
  option[DIFF_TRACE][HISTOGRAM][HISTOGRAM] = true;
  allowed["Time"] = option;
  
  // Policy : Allow paste size between different type of windows and different traces
/*
  option[SAME_TRACE][TIMELINE][TIMELINE] = true;
  option[SAME_TRACE][TIMELINE][TIMELINE] = true;
  option[SAME_TRACE][TIMELINE][HISTOGRAM] = true;
  option[SAME_TRACE][HISTOGRAM][TIMELINE] = true;
  option[SAME_TRACE][HISTOGRAM][HISTOGRAM] = true;
  option[DIFF_TRACE][TIMELINE][TIMELINE] = true;
  option[DIFF_TRACE][TIMELINE][HISTOGRAM] = true;
  option[DIFF_TRACE][HISTOGRAM][TIMELINE] = true;
  option[DIFF_TRACE][HISTOGRAM][HISTOGRAM] = true;
*/
  for ( int trace = SAME_TRACE; trace <= DIFF_TRACE; trace++ )
    for ( int copy = TIMELINE; copy <= HISTOGRAM; copy++ )
      for ( int paste = TIMELINE; paste <= HISTOGRAM; paste++ )
        option[trace][copy][paste] = true;
  allowed["Size"] = option;

  for ( int trace = SAME_TRACE; trace <= DIFF_TRACE; trace++ )
    for ( int copy = TIMELINE; copy <= HISTOGRAM; copy++ )
      for ( int paste = TIMELINE; paste <= HISTOGRAM; paste++ )
        option[trace][copy][paste] = false;

  // Policy : Allow paste times between different windows and differente traces.
/*
  option[SAME_TRACE][TIMELINE][TIMELINE] = true;
  option[SAME_TRACE][TIMELINE][TIMELINE] = true;
  option[SAME_TRACE][TIMELINE][HISTOGRAM] = true;
  option[SAME_TRACE][HISTOGRAM][TIMELINE] = true;
  option[SAME_TRACE][HISTOGRAM][HISTOGRAM] = true;
  option[DIFF_TRACE][TIMELINE][TIMELINE] = true;
  option[DIFF_TRACE][TIMELINE][HISTOGRAM] = true;
  option[DIFF_TRACE][HISTOGRAM][TIMELINE] = true;
  option[DIFF_TRACE][HISTOGRAM][HISTOGRAM] = true;
*/
  allowed["Objects"] = option;

  // Policy : Allow paste times between different windows and differente traces.
/*
  option[SAME_TRACE][TIMELINE][TIMELINE] = true;
  option[SAME_TRACE][TIMELINE][TIMELINE] = true;
  option[SAME_TRACE][TIMELINE][HISTOGRAM] = true;
  option[SAME_TRACE][HISTOGRAM][TIMELINE] = true;
  option[SAME_TRACE][HISTOGRAM][HISTOGRAM] = true;
  option[DIFF_TRACE][TIMELINE][TIMELINE] = true;
  option[DIFF_TRACE][TIMELINE][HISTOGRAM] = true;
  option[DIFF_TRACE][HISTOGRAM][TIMELINE] = true;
  option[DIFF_TRACE][HISTOGRAM][HISTOGRAM] = true;
*/
  allowed["Filter"] = option;

  // Policy : Allow paste times between different windows and differente traces.
/*
  option[SAME_TRACE][TIMELINE][TIMELINE] = true;
  option[SAME_TRACE][TIMELINE][TIMELINE] = true;
  option[SAME_TRACE][TIMELINE][HISTOGRAM] = true;
  option[SAME_TRACE][HISTOGRAM][TIMELINE] = true;
  option[SAME_TRACE][HISTOGRAM][HISTOGRAM] = true;
  option[DIFF_TRACE][TIMELINE][TIMELINE] = true;
  option[DIFF_TRACE][TIMELINE][HISTOGRAM] = true;
  option[DIFF_TRACE][HISTOGRAM][TIMELINE] = true;
  option[DIFF_TRACE][HISTOGRAM][HISTOGRAM] = true;
*/
  allowed["Filter All"] = option;

  // Policy : Allow paste times between different windows and differente traces.
/*
  option[SAME_TRACE][TIMELINE][TIMELINE] = true;
  option[SAME_TRACE][TIMELINE][TIMELINE] = true;
  option[SAME_TRACE][TIMELINE][HISTOGRAM] = true;
  option[SAME_TRACE][HISTOGRAM][TIMELINE] = true;
  option[SAME_TRACE][HISTOGRAM][HISTOGRAM] = true;
  option[DIFF_TRACE][TIMELINE][TIMELINE] = true;
  option[DIFF_TRACE][TIMELINE][HISTOGRAM] = true;
  option[DIFF_TRACE][HISTOGRAM][TIMELINE] = true;
  option[DIFF_TRACE][HISTOGRAM][HISTOGRAM] = true;
*/
  allowed["Communications"] = option;

  // Policy : Allow paste times between different windows and differente traces.
/*
  option[SAME_TRACE][TIMELINE][TIMELINE] = true;
  option[SAME_TRACE][TIMELINE][TIMELINE] = true;
  option[SAME_TRACE][TIMELINE][HISTOGRAM] = true;
  option[SAME_TRACE][HISTOGRAM][TIMELINE] = true;
  option[SAME_TRACE][HISTOGRAM][HISTOGRAM] = true;
  option[DIFF_TRACE][TIMELINE][TIMELINE] = true;
  option[DIFF_TRACE][TIMELINE][HISTOGRAM] = true;
  option[DIFF_TRACE][HISTOGRAM][TIMELINE] = true;
  option[DIFF_TRACE][HISTOGRAM][HISTOGRAM] = true;
*/
  allowed["Events"] = option;
/*
  for ( int trace = SAME_TRACE; trace <= DIFF_TRACE; trace++ )
    for ( int copy = TIMELINE; copy <= HISTOGRAM; copy++ )
      for ( int paste = TIMELINE; paste <= HISTOGRAM; paste++ )
      {
        option[trace][copy][paste] = false;
      }
*/
  allowed["Paste"] = option;
}

// PUBLIC 

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
    if ( property == "Time" )
    {
      TRecordTime sourceBeginTime = timeline->GetMyWindow()->getWindowBeginTime();
      TRecordTime sourceEndTime   = timeline->GetMyWindow()->getWindowEndTime();

      // Time paste
      if ( sourceBeginTime < destinyTraceBeginTime )
        whichTimeline->GetMyWindow()->setWindowBeginTime( destinyTraceBeginTime );
      else
        whichTimeline->GetMyWindow()->setWindowBeginTime( sourceBeginTime );

      if ( sourceEndTime > destinyTraceEndTime )
        whichTimeline->GetMyWindow()->setWindowEndTime( destinyTraceEndTime );
      else
        whichTimeline->GetMyWindow()->setWindowEndTime( sourceEndTime );
    }
    else if ( property == "Size" )
    {
      int width, height;
      timeline->GetSize( &width, &height);
      whichTimeline->SetSize( width, height );
    }
    else if ( property == "Objects" )
    {
    }
    else if ( property == "Communications" )
    {
    }
    else if ( property == "Events" )
    {
    }
    else if ( property == "Filter All" )
    {
    }
    else
    {
    }
  }
  else
  {
    // paste histogram -> timeline
    if ( property == "Time" )
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
    else if ( property == "Size" )
    {
      int width, height;
      histogram->GetSize( &width, &height);
      whichTimeline->SetSize( width, height );
    }
    else if ( property == "Objects" )
    {
    }
    else if ( property == "Communications" )
    {
    }
    else if ( property == "Events" )
    {
    }
    else if ( property == "Filter All" )
    {
    }
    else
    {
    }
  }

}

void gPasteWindowProperties::paste( gHistogram* whichHistogram, const string property )
{
  if ( timeline != NULL )
  {
    // paste timeline -> histogram
    if ( property == "Time" )
    {
      whichHistogram->GetHistogram()->setWindowBeginTime( timeline->GetMyWindow()->getWindowBeginTime() );
      whichHistogram->GetHistogram()->setWindowEndTime( timeline->GetMyWindow()->getWindowEndTime() );
    }
    else if ( property == "Size" )
    {
      int width, height;
      timeline->GetSize( &width, &height);
      whichHistogram->SetSize( width, height );
    }
    else if ( property == "Objects" )
    {
    }
    else if ( property == "Communications" )
    {
    }
    else if ( property == "Events" )
    {
    }
    else if ( property == "Filter All" )
    {
    }
    else
    {
    }
  }
  else
  {
    // paste histogram -> histogram
    if ( property == "Time" )
    {
      whichHistogram->GetHistogram()->setWindowBeginTime( histogram->GetHistogram()->getBeginTime() );
      whichHistogram->GetHistogram()->setWindowEndTime( histogram->GetHistogram()->getBeginTime() );
    }
    else if ( property == "Size" )
    {
      int width, height;
      histogram->GetSize( &width, &height);
      whichHistogram->SetSize( width, height );
    }
    else if ( property == "Objects" )
    {
    }
    else if ( property == "Communications" )
    {
    }
    else if ( property == "Events" )
    {
    }
    else if ( property == "Filter All" )
    {
    }
    else
    {
    }
  }
}

bool gPasteWindowProperties::allowPaste( gTimeline *whichTimeline, const string property )
{
  if ( timeline == NULL && histogram == NULL )
    return false;
  else if ( allowed.count( property ) == 0 )
    return false;

  commonTimeSettings( whichTimeline->GetMyWindow()->getTrace()->getEndTime() );
  commonMenuSettings();

  return seekAllowedPaste( property, TIMELINE, whichTimeline->GetMyWindow()->getTrace() );
}


bool gPasteWindowProperties::allowPaste( gHistogram *whichHistogram, const string property )
{
  if ( timeline == NULL && histogram == NULL )
    return false;
  else if ( allowed.count( property ) == 0 )
    return false;

  commonTimeSettings( whichHistogram->GetHistogram()->getControlWindow()->getTrace()->getEndTime() );
  commonMenuSettings();

  return seekAllowedPaste( property, HISTOGRAM, whichHistogram->GetHistogram()->getControlWindow()->getTrace() );
}
