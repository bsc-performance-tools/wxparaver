#ifndef _COPYPASTE_H_ 
#define _COPYPASTE_H_

#include <map>
using namespace std;

#include "window.h"
#include "histogram.h"
#include "gtimeline.h"
#include "ghistogram.h"

class gTimeline;
class gHistogram;

class gPasteWindowProperties
{
  public:
    static gPasteWindowProperties *pasteWindowProperties;

    ~gPasteWindowProperties();

    static gPasteWindowProperties *getInstance();
    
    void copy( gTimeline* whichTimeline );
    void copy( gHistogram* whichHistogram );

    void paste( gTimeline* whichTimeline, const string property );
    void paste( gHistogram* whichHistogram, const string property );

    bool isAllowed( gTimeline* whichTimeline, const string tag );
    bool isAllowed( gHistogram* whichHistogram, const string tag );

  private:
    #define SAME_TRACE 0
    #define DIFF_TRACE 1
    #define TIMELINE 0
    #define HISTOGRAM 1

    gTimeline  *timeline;
    gHistogram *histogram;
    map < const string, vector< vector < vector< bool > > > > allowed;

    gPasteWindowProperties();

    void commonMenuSettings( );
    void commonTimeSettings( TRecordTime destinyTraceEndTime );
    bool seekAllowed( const string property, int destiny, Trace *destinyTrace );
};

#endif // _COPYPASTE_H_
