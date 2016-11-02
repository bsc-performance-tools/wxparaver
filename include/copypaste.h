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

#ifndef _COPYPASTE_H_ 
#define _COPYPASTE_H_

#include <map>
using namespace std;

#include "window.h"
#include "histogram.h"
#include "gtimeline.h"
#include "ghistogram.h"

#define STR_COPY                "Copy\tCTRL+C"
#define STR_PASTE               "Paste"
#define STR_PASTE_DEFAULT_SPECIAL    "Default\tCTRL+V"
#define STR_PASTE_SPECIAL       "Paste Special..."
#define STR_TIME                "Time"
#define STR_SIZE                "Size"
#define STR_OBJECTS             "Objects"
#define STR_DURATION            "Duration"
#define STR_SEMANTIC_SCALE      "Semantic Scale"
#define STR_FILTER              "Filter"
#define STR_FILTER_COMMS        "Communications"
#define STR_FILTER_COMMS_XT     "Filter:Communications"
#define STR_FILTER_EVENTS       "Events"
#define STR_FILTER_EVENTS_XT    "Filter:Events"
#define STR_FILTER_ALL          "All"
#define STR_CLONE               "Clone"
#define STR_FIT_TIME            "Fit Time Scale"
#define STR_FIT_SEMANTIC        "Fit Semantic Scale"
#define STR_FIT_OBJECTS         "Fit Objects"
#define STR_CONTROL_SCALE       "Control Scale"
#define STR_CONTROL_DIMENSIONS  "Control Dimensions"
#define STR_3D_SCALE            "3D scale"
#define STR_SYNCHRONIZE         "Synchronize"
#define STR_SYNC_REMOVEALL      "Remove all sync"
#define STR_SAVE_IMAGE          "Save Image..."
#define STR_UNDO_ZOOM           "Undo Zoom\tCTRL+U"
#define STR_REDO_ZOOM           "Redo Zoom\tCTRL+R"

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

    TRecordTime getBeginTime();
    TRecordTime getEndTime();

    void verifyRemove( gTimeline *whichTimeline );
    void verifyRemove( gHistogram *whichHistogram );
    
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
