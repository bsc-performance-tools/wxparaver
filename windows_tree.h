#ifndef _GWINDOWTREE_H_ 
#define _GWINDOWTREE_H_

#include <wx/treectrl.h>
#include <wx/choicebk.h>
#include <vector>

class gTimeline;
class gHistogram;
class paraverMain;
class Window;
class Histogram;

using namespace std;

wxTreeCtrl * createTree( wxImageList *imageList );
wxTreeCtrl *getAllTracesTree();
wxTreeCtrl *getSelectedTraceTree();

void appendTimeline2Tree( gTimeline *whichTimeline, Window *window );
void appendHistogram2Tree( gHistogram *ghistogram );

wxTreeItemId getItemIdFromGTimeline( wxTreeItemId root, gTimeline *wanted, bool &found );
gTimeline *getGTimelineFromWindow( wxTreeItemId root, Window *wanted, bool &found );
void getParentGTimeline( gTimeline *current, vector< gTimeline * > & children );

void BuildTree( wxWindow *parent,
                wxTreeCtrl *root1, wxTreeItemId idRoot1,
                wxTreeCtrl *root2, wxTreeItemId idRoot2,
                Window *window,
                string nameSuffix = "" );
void updateTreeItem( wxTreeCtrl *tree,
                     wxTreeItemId& id,
                     vector< Window * > &allWindows,
                     vector< Histogram * > &allHistograms,
                     wxWindow **currentWindow  );

#endif // _GWINDOWTREE_H_
