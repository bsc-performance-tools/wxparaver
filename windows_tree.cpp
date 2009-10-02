#include "windows_tree.h"
#include "loadedwindows.h"
#include "gtimeline.h"
#include "ghistogram.h"
#include "paravermain.h"



wxTreeCtrl * createTree( wxImageList *imageList )
{
  wxChoicebook *choiceWindowBrowser = paraverMain::myParaverMain->choiceWindowBrowser;
  wxTreeCtrl *newTree =  new wxTreeCtrl( choiceWindowBrowser, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                         wxTR_HIDE_ROOT|wxTR_DEFAULT_STYLE );
#ifndef WIN32
  newTree->SetWindowStyle( wxTR_HAS_BUTTONS|wxTR_HIDE_ROOT|wxTR_SINGLE );
#endif
  newTree->SetImageList( imageList );
  newTree->AddRoot( wxT( "Root" ), -1, -1, new TreeBrowserItemData( "Root", (gTimeline *)NULL ) );
  
  return newTree;
}

wxTreeCtrl *getAllTracesTree()
{
  wxChoicebook *choiceWindowBrowser = paraverMain::myParaverMain->choiceWindowBrowser;
  
  return (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 );
}

wxTreeCtrl *getSelectedTraceTree()
{
  wxChoicebook *choiceWindowBrowser = paraverMain::myParaverMain->choiceWindowBrowser;
  INT16 currentTrace = paraverMain::myParaverMain->GetCurrentTrace();

  return (wxTreeCtrl *) choiceWindowBrowser->GetPage( currentTrace + 1 );
}




void appendTimeline2Tree( gTimeline *whichTimeline, Window *window )
{
  // Refresh tree in current page and always in global page
  wxTreeCtrl *allTracesPage = getAllTracesTree();
  wxTreeCtrl *currentPage = getSelectedTraceTree();
  TreeBrowserItemData *currentData =  new TreeBrowserItemData( window->getName(), whichTimeline );

  wxTreeItemId currentWindowId1 = allTracesPage->AppendItem( allTracesPage->GetRootItem(), window->getName(), 1, -1, currentData );
  wxTreeItemId currentWindowId2 = currentPage->AppendItem( currentPage->GetRootItem(), window->getName(), 1, -1, new TreeBrowserItemData( *currentData ) );
}


void appendHistogram2Tree( gHistogram *ghistogram )
{
  // Refresh tree in current page and always in global page
  wxTreeCtrl *allTracesPage = getAllTracesTree();
  wxTreeCtrl *currentPage = getSelectedTraceTree();
  TreeBrowserItemData *currentData =  new TreeBrowserItemData( ghistogram->GetHistogram()->getName(), ghistogram );

  wxTreeItemId currentWindowId1 = allTracesPage->AppendItem( allTracesPage->GetRootItem(),
                                                             ghistogram->GetHistogram()->getName(), 0, -1,
                                                             currentData );
  wxTreeItemId currentWindowId2 = currentPage->AppendItem( currentPage->GetRootItem(),
                                                           ghistogram->GetHistogram()->getName(), 0, -1,
                                                           new TreeBrowserItemData( *currentData ) );
}



wxTreeItemId getItemIdFromGTimeline( wxTreeItemId root, gTimeline *wanted, bool &found )
{
  wxTreeItemIdValue cookie;

  found = false;

  wxTreeItemId itemCurrent = getAllTracesTree()->GetFirstChild( root, cookie );
  wxTreeItemId itemLast = getAllTracesTree()->GetLastChild( root );
  
  while ( !found && itemCurrent.IsOk() && itemCurrent != itemLast )
  {
    if (((TreeBrowserItemData *)(getAllTracesTree()->GetItemData( itemCurrent )))->getTimeline() == wanted )
    {
      root = itemCurrent;
      found = true;
    }
    else
    {
      root = getItemIdFromGTimeline( itemCurrent, wanted, found );
      if( !found )
        itemCurrent = getAllTracesTree()->GetNextChild( root, cookie );
    }
  }
  
  if( !found && itemLast.IsOk() )
  {
    if (((TreeBrowserItemData *)(getAllTracesTree()->GetItemData( itemLast )))->getTimeline() == wanted )
    {
      root = itemLast;
      found = true;
    }
    else
    {
      root = getItemIdFromGTimeline( itemLast, wanted, found );
    }
  }
  
  return root;
}

gTimeline *getGTimelineFromWindow( wxTreeItemId root, Window *wanted, bool &found )
{
  gTimeline *retgt = NULL;
  wxTreeItemIdValue cookie;

  found = false;

  wxTreeItemId itemCurrent = getAllTracesTree()->GetFirstChild( root, cookie );
  wxTreeItemId itemLast = getAllTracesTree()->GetLastChild( root );
  
  while ( !found && itemCurrent.IsOk() && itemCurrent != itemLast )
  {
    if (((TreeBrowserItemData *)(getAllTracesTree()->GetItemData( itemCurrent )))->getTimeline()->GetMyWindow() == wanted )
    {
      retgt = ((TreeBrowserItemData *)(getAllTracesTree()->GetItemData( itemCurrent )))->getTimeline();
      found = true;
    }
    else
    {
      retgt = getGTimelineFromWindow( itemCurrent, wanted, found );
      if( !found )
        itemCurrent = getAllTracesTree()->GetNextChild( root, cookie );
    }
  }
  
  if( !found && itemLast.IsOk() )
  {
    if (((TreeBrowserItemData *)(getAllTracesTree()->GetItemData( itemLast )))->getTimeline()->GetMyWindow() == wanted )
    {
      retgt = ((TreeBrowserItemData *)(getAllTracesTree()->GetItemData( itemLast )))->getTimeline();
      found = true;
    }
    else
    {
      retgt = getGTimelineFromWindow( itemLast, wanted, found );
    }
  }
  
  return retgt;
}

// precond : current is a derived gTimeline
void getParentGTimeline( gTimeline *current, vector< gTimeline * > & parents )
{
  // find item for given current gTimeline.
  bool found;
  wxTreeItemId item = getItemIdFromGTimeline( getAllTracesTree()->GetRootItem(), current, found );

  // fill vector with parents
  wxTreeItemIdValue cookie;
  parents.push_back(((TreeBrowserItemData *)(getAllTracesTree()->GetItemData( getAllTracesTree()->GetFirstChild( item, cookie ))))->getTimeline());
  parents.push_back(((TreeBrowserItemData *)(getAllTracesTree()->GetItemData( getAllTracesTree()->GetNextChild( item, cookie ) )))->getTimeline());
}

void BuildTree( wxWindow *parent,
                wxTreeCtrl *root1, wxTreeItemId idRoot1,
                wxTreeCtrl *root2, wxTreeItemId idRoot2,
                Window *window,
                string nameSuffix )
{
  wxTreeItemId currentWindowId1, currentWindowId2;
  TreeBrowserItemData *currentData;

  string composedName = window->getName() + " @ " +
                        window->getTrace()->getTraceName();

  gTimeline* tmpTimeline = new gTimeline( parent, wxID_ANY, composedName );
  LoadedWindows::getInstance()->add( window );
  tmpTimeline->SetMyWindow( window );
  tmpTimeline->SetClientSize( wxRect( window->getPosX(), window->getPosY(),
                                      window->getWidth(), window->getHeight() ) );

  if( window->getShowWindow() )
  {
    tmpTimeline->Show();
    tmpTimeline->redraw();
  }

  currentData =  new TreeBrowserItemData( window->getName(), tmpTimeline );
  currentWindowId1 = root1->AppendItem( idRoot1, window->getName(), 1, -1, currentData );
  currentWindowId2 = root2->AppendItem( idRoot2, window->getName(), 1, -1, new TreeBrowserItemData( *currentData ) );

  if ( window->getParent( 0 ) != NULL )
  {
    BuildTree( parent, root1, currentWindowId1, root2, currentWindowId2, window->getParent( 0 ) );
    BuildTree( parent, root1, currentWindowId1, root2, currentWindowId2, window->getParent( 1 ) );
  }
}


void updateTreeItem( wxTreeCtrl *tree,
                     wxTreeItemId& id,
                     vector< Window * > &allWindows,
                     vector< Histogram * > &allHistograms,
                     wxWindow *currentWindow )
{
  TreeBrowserItemData *itemData = (TreeBrowserItemData *)tree->GetItemData( id );

  // No matter timeline or histogram, get its name and delete from given vector 
  wxString tmpName;
  if( gTimeline *tmpTimeline = itemData->getTimeline() )
  {
    if( tmpTimeline->IsActive() )
    {
      currentWindow = tmpTimeline;
      tree->SelectItem( id );
    }
    Window *tmpWindow = tmpTimeline->GetMyWindow();
    tmpName = tmpWindow->getName();
    for ( vector<Window *>::iterator it = allWindows.begin(); it != allWindows.end(); it++ )
    {
      if ( *it == tmpWindow )
      {
        allWindows.erase( it );
        break;
      }
    }
  }
  else if( gHistogram *tmpHistogram = itemData->getHistogram() )
  {
    if( tmpHistogram->IsActive() )
    {
      currentWindow = tmpHistogram;
      tree->SelectItem( id );
    }
    Histogram *tmpHisto = tmpHistogram->GetHistogram();
    tmpName = tmpHisto->getName();
    for ( vector<Histogram *>::iterator it = allHistograms.begin(); it != allHistograms.end(); it++ )
    {
      if ( *it == tmpHisto )
      {
        allHistograms.erase( it );
        break;
      }
    }
  }
  
  // Update its name
  if( tmpName != tree->GetItemText( id ) )
    tree->SetItemText( id, tmpName );
    
  // Recursive update
  if( tree->ItemHasChildren( id ) )
  {
    wxTreeItemIdValue cookie;
    wxTreeItemId currentChild = tree->GetFirstChild( id, cookie );
    while( currentChild.IsOk() )
    {
      updateTreeItem( tree, currentChild, allWindows, allHistograms, currentWindow );
      currentChild = tree->GetNextChild( id, cookie );
    }
  }
}

