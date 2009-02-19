#ifndef _COPYPASTE_H_ 
#define _COPYPASTE_H_

#include <map>
using namespace std;

#include "window.h"
#include "histogram.h"
#include "gtimeline.h"
#include "ghistogram.h"

#define ST_COPY             "Copy"
#define ST_PASTE            "Paste"
#define ST_PASTE_SPECIAL    "Paste Special..."
#define ST_TIME             "Time"
#define ST_SIZE             "Size"
#define ST_OBJECTS          "Objects"
#define ST_FILTER           "Filter"
#define ST_FILTER_COMMS     "Communications"
#define ST_FILTER_COMMS_XT  "Filter:Communications"
#define ST_FILTER_EVENTS    "Events"
#define ST_FILTER_EVENTS_XT "Filter:Events"
#define ST_FILTER_ALL       "All"
#define ST_CLONE            "Clone"
#define ST_FIT_TIME         "Fit Time Scale"
#define ST_FIT_SEMANTIC     "Fit Semantic Scale"

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
    void commonFilterSettings( gTimeline *whichTimeline );
    bool seekAllowed( const string property, int destiny, Trace *destinyTrace );
};

#endif // _COPYPASTE_H_
