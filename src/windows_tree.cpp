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


#include "windows_tree.h"
#include "loadedwindows.h"
#include "gtimeline.h"
#include "ghistogram.h"
#include "paravermain.h"
#include <wx/display.h>


wxTreeCtrl * createTree( wxImageList *imageList )
{
  wxChoicebook *choiceWindowBrowser = paraverMain::myParaverMain->choiceWindowBrowser;
  wxTreeCtrl *newTree =  new wxTreeCtrl( choiceWindowBrowser, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                         wxTR_HIDE_ROOT|wxTR_DEFAULT_STYLE );
#ifndef WIN32
  newTree->SetWindowStyle( wxTR_HAS_BUTTONS|wxTR_HIDE_ROOT|wxTR_SINGLE );
#endif
  newTree->SetImageList( imageList );
  newTree->AddRoot( wxT( "Root" ), -1, -1, new TreeBrowserItemData( _( "Root" ), (gTimeline *)NULL ) );
  
  return newTree;
}


wxTreeCtrl *getAllTracesTree()
{
  wxChoicebook *choiceWindowBrowser = paraverMain::myParaverMain->choiceWindowBrowser;
  
  return (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 );
}


wxTreeCtrl *getSelectedTraceTree( Trace *trace )
{
  wxChoicebook *choiceWindowBrowser = paraverMain::myParaverMain->choiceWindowBrowser;

  PRV_INT16 currentTrace = paraverMain::myParaverMain->getTracePosition( trace );

  return (wxTreeCtrl *) choiceWindowBrowser->GetPage( currentTrace + 1 );
}


void appendHistogram2Tree( gHistogram *ghistogram )
{
  // Refresh tree in current page and always in global page
  wxTreeCtrl *allTracesPage = getAllTracesTree();
  wxTreeCtrl *currentPage = getSelectedTraceTree( ghistogram->GetHistogram()->getControlWindow()->getTrace() );
  TreeBrowserItemData *currentData =  new TreeBrowserItemData( wxString::FromAscii( ghistogram->GetHistogram()->getName().c_str() ), ghistogram );

  wxTreeItemId tmpCurrentWindowId;
  tmpCurrentWindowId = allTracesPage->AppendItem( allTracesPage->GetRootItem(),
                                                   wxString::FromAscii( ghistogram->GetHistogram()->getName().c_str() ), 0, -1,
                                                   currentData );
  tmpCurrentWindowId = currentPage->AppendItem( currentPage->GetRootItem(),
                                                 wxString::FromAscii( ghistogram->GetHistogram()->getName().c_str() ), 0, -1,
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
    gTimeline *tmpTimeline = ((TreeBrowserItemData *)(getAllTracesTree()->GetItemData( itemCurrent )))->getTimeline();
    if ( tmpTimeline != NULL && tmpTimeline == wanted )
    {
      root = itemCurrent;
      found = true;
    }
    else if ( tmpTimeline != NULL )
    {
      root = getItemIdFromGTimeline( itemCurrent, wanted, found );
    }
    if( !found )
      itemCurrent = getAllTracesTree()->GetNextChild( root, cookie );
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


// TODO: Separate recursion function to remove bool from parameters in main definition
gTimeline *getGTimelineFromWindow( wxTreeItemId root, Window *wanted, bool &found )
{
  gTimeline *retgt = NULL;
  wxTreeItemIdValue cookie;

  found = false;

  wxTreeItemId itemCurrent = getAllTracesTree()->GetFirstChild( root, cookie );
  wxTreeItemId itemLast = getAllTracesTree()->GetLastChild( root );
  
  while ( !found && itemCurrent.IsOk() && itemCurrent != itemLast )
  {
    gTimeline *tmpTimeline = ((TreeBrowserItemData *)(getAllTracesTree()->GetItemData( itemCurrent )))->getTimeline();
    if ( tmpTimeline != NULL && tmpTimeline->GetMyWindow() == wanted )
    {
      retgt = tmpTimeline;
      found = true;
    }
    else if ( tmpTimeline != NULL )
    {
      retgt = getGTimelineFromWindow( itemCurrent, wanted, found );
    }
    if( !found )
      itemCurrent = getAllTracesTree()->GetNextChild( root, cookie );
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


gHistogram *getGHistogramFromWindow( wxTreeItemId root, Histogram *wanted )
{
  gHistogram *retgh = NULL;
  wxTreeItemIdValue cookie;

  bool found = false;

  wxTreeItemId itemCurrent = getAllTracesTree()->GetFirstChild( root, cookie );
  wxTreeItemId itemLast = getAllTracesTree()->GetLastChild( root );
  
  while( !found && itemCurrent.IsOk() && itemCurrent != itemLast )
  {
    gHistogram *tmpHistogram = ( (TreeBrowserItemData *)( getAllTracesTree()->GetItemData( itemCurrent ) ) )->getHistogram();
    if( tmpHistogram != NULL && tmpHistogram->GetHistogram() == wanted )
    {
      retgh = tmpHistogram;
      found = true;
    }
    if( !found )
      itemCurrent = getAllTracesTree()->GetNextChild( root, cookie );
  }
  
  if( !found && itemLast.IsOk() )
  {
    if( ( (TreeBrowserItemData *)( getAllTracesTree()->GetItemData( itemLast ) ) )->getHistogram()->GetHistogram() == wanted )
    {
      retgh = ( (TreeBrowserItemData *)(getAllTracesTree()->GetItemData( itemLast )))->getHistogram();
      found = true;
    }
  }
  
  return retgh;
}


wxTreeItemId getItemIdFromWindow( wxTreeItemId root, Window *wanted, bool &found )
{
  wxTreeItemId retItemId;
  wxTreeItemIdValue cookie;

  found = false;

  wxTreeItemId itemCurrent = getAllTracesTree()->GetFirstChild( root, cookie );
  wxTreeItemId itemLast = getAllTracesTree()->GetLastChild( root );
  
  while ( !found && itemCurrent.IsOk() && itemCurrent != itemLast )
  {
    gTimeline *tmpTimeline = ((TreeBrowserItemData *)(getAllTracesTree()->GetItemData( itemCurrent )))->getTimeline();
    if ( tmpTimeline != NULL && tmpTimeline->GetMyWindow() == wanted )
    {
      retItemId = itemCurrent;
      found = true;
    }
    else if ( tmpTimeline != NULL )
    {
      retItemId = getItemIdFromWindow( itemCurrent, wanted, found );
    }
    if( !found )
      itemCurrent = getAllTracesTree()->GetNextChild( root, cookie );
  }
  
  if( !found && itemLast.IsOk() )
  {
    if (((TreeBrowserItemData *)(getAllTracesTree()->GetItemData( itemLast )))->getTimeline()->GetMyWindow() == wanted )
    {
      retItemId = itemLast;
      found = true;
    }
    else
    {
      retItemId = getItemIdFromWindow( itemLast, wanted, found );
    }
  }
  
  return retItemId;
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


void BuildTree( paraverMain *parent,
                wxTreeCtrl *root1, wxTreeItemId idRoot1,
                wxTreeCtrl *root2, wxTreeItemId idRoot2,
                Window *window,
                string nameSuffix )
{
  wxTreeItemId currentWindowId1, currentWindowId2;
  TreeBrowserItemData *currentData;

  string composedName = window->getName() + " @ " +
                        window->getTrace()->getTraceName();

  wxPoint tmpPos( window->getPosX(), window->getPosY() );
  if( wxDisplay::GetCount() > 1 /*&& ParaverConfig::???*/ )
  {
    int currentDisplay = wxDisplay::GetFromWindow( paraverMain::myParaverMain );
    if ( currentDisplay != wxNOT_FOUND && currentDisplay >= 0 )
    {
      wxDisplay tmpDisplay( currentDisplay );
      tmpPos.x += tmpDisplay.GetGeometry().x;
      tmpPos.y += tmpDisplay.GetGeometry().y;
      if( tmpPos.x != window->getPosX() ) window->setPosX( tmpPos.x );
      if( tmpPos.x != window->getPosY() ) window->setPosX( tmpPos.y );
    }
  }
  
#if wxMAJOR_VERSION<3 || !__WXGTK__
  gTimeline* tmpTimeline = new gTimeline( parent, wxID_ANY, wxString::FromAscii( composedName.c_str() ), tmpPos );
#else
  gTimeline* tmpTimeline = new gTimeline( parent, wxID_ANY, wxString::FromAscii( composedName.c_str() ) );
#endif
  LoadedWindows::getInstance()->add( window );
  tmpTimeline->SetMyWindow( window );
  tmpTimeline->SetClientSize( wxSize( window->getWidth(), window->getHeight() ) );
#if !( wxMAJOR_VERSION<3 || !__WXGTK__ )
  tmpTimeline->Move( tmpPos );
#endif

  currentData =  new TreeBrowserItemData( wxString::FromAscii( window->getName().c_str() ), tmpTimeline );

  int iconNumber = getIconNumber( window );
  currentWindowId1 = root1->AppendItem( idRoot1, wxString::FromAscii( window->getName().c_str() ), iconNumber, -1, currentData );
  currentWindowId2 = root2->AppendItem( idRoot2, wxString::FromAscii( window->getName().c_str() ), iconNumber, -1, new TreeBrowserItemData( *currentData ) );

  if ( window->getParent( 0 ) != NULL )
  {
    BuildTree( parent, root1, currentWindowId1, root2, currentWindowId2, window->getParent( 0 ) );
    BuildTree( parent, root1, currentWindowId1, root2, currentWindowId2, window->getParent( 1 ) );
  }
  parent->SetCurrentWindow( (wxWindow *)tmpTimeline );
}


bool updateTreeItem( wxTreeCtrl *tree,
                     wxTreeItemId& id,
                     vector< Window * > &allWindows,
                     vector< Histogram * > &allHistograms,
                     wxWindow **currentWindow,
                     bool allTracesTree )
{
  bool destroy = false;
  TreeBrowserItemData *itemData = (TreeBrowserItemData *)tree->GetItemData( id );

  // No matter timeline or histogram, get its name and delete from given vector 
  wxString tmpName;
  if( gTimeline *tmpTimeline = itemData->getTimeline() )
  {
    Window *tmpWindow = tmpTimeline->GetMyWindow();
    if( tmpWindow->isSync() )
      tree->SetItemBold( id, true );
    else
      tree->SetItemBold( id, false );
      
    if( tmpTimeline->IsActive() && !tmpWindow->getDestroy() )
    {
      *currentWindow = tmpTimeline;
      tree->SelectItem( id );
    }
    tmpName = wxString::FromAscii( tmpWindow->getName().c_str() );
    
    for ( vector<Window *>::iterator it = allWindows.begin(); it != allWindows.end(); it++ )
    {
      if ( *it == tmpWindow )
      {
        allWindows.erase( it );
        break;
      }
    }
    
    if( tmpWindow->getDestroy() )
    {
      if( paraverMain::myParaverMain->GetCurrentTimeline() == tmpWindow )
      {
        paraverMain::myParaverMain->SetCurrentTimeline( NULL );
        paraverMain::myParaverMain->clearProperties();
      }
      if( !allTracesTree )
        tmpTimeline->Destroy();
      Window *parent1 = tmpWindow->getParent( 0 );
      if( parent1 != NULL )
      {
        parent1->setChild( NULL );
        parent1->setDestroy( true );
      }
      Window *parent2 = tmpWindow->getParent( 1 );
      if( parent2 != NULL )
      {
        parent2->setChild( NULL );
        parent2->setDestroy( true );
      }
      destroy = true;
    }
  }
  else if( gHistogram *tmpHistogram = itemData->getHistogram() )
  {
    Histogram *tmpHisto = tmpHistogram->GetHistogram();
    if( tmpHisto->isSync() )
      tree->SetItemBold( id, true );
    else
      tree->SetItemBold( id, false );

    if( tmpHistogram->IsActive() && !tmpHisto->getDestroy() )
    {
      *currentWindow = tmpHistogram;
      tree->SelectItem( id );
    }
    tmpName = wxString::FromAscii( tmpHisto->getName().c_str() );
    for ( vector<Histogram *>::iterator it = allHistograms.begin(); it != allHistograms.end(); it++ )
    {
      if ( *it == tmpHisto )
      {
        allHistograms.erase( it );
        break;
      }
    }

    if( tmpHisto->getDestroy() )
    {
      if( paraverMain::myParaverMain->GetCurrentHisto() == tmpHisto )
      {
        paraverMain::myParaverMain->SetCurrentHisto( NULL );
        paraverMain::myParaverMain->clearProperties();
      }
      if( !allTracesTree )
        tmpHistogram->Destroy();
      destroy = true;
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
      updateTreeItem( tree, currentChild, allWindows, allHistograms, currentWindow, allTracesTree );
      if( !destroy )
        currentChild = tree->GetNextChild( id, cookie );
      else
        currentChild = tree->GetFirstChild( id, cookie );
    }
  }

  if( destroy )
  {
    // Properly select previous window if no further timeline or histogram in the tree exists
    // The next sibling is properly selected by recursion if it exists
    wxTreeItemId candidate = tree->GetNextSibling( id );
    if ( !candidate.IsOk() )
      candidate = tree->GetPrevSibling( id );

    if ( candidate.IsOk() )
    {
      if ( gTimeline *tmpTimeline = itemData->getTimeline() )
      {
        *currentWindow = tmpTimeline;
      }
      else if( gHistogram *tmpHistogram = itemData->getHistogram() )
      {
        *currentWindow = tmpHistogram;
      }
      tree->SelectItem( candidate );
    }

    tree->Delete( id );
  }
    
  return destroy;
}


void iconizeWindows( wxTreeCtrl *tree,
                     wxTreeItemId& id,
                     bool iconize )
{
  wxTreeItemIdValue cookie;
  wxTreeItemId currentChild = tree->GetFirstChild( id, cookie );
  unsigned int numberChild = tree->GetChildrenCount( id, false );
  unsigned int current = 0;
  while( current < numberChild )
  {
    if ( currentChild.IsOk() )
    {
      TreeBrowserItemData *itemData = (TreeBrowserItemData *)tree->GetItemData( currentChild );

      if( gTimeline *tmpTimeline = itemData->getTimeline() )
      {
        if( tmpTimeline->GetMyWindow()->getShowWindow() )
          tmpTimeline->Show( iconize );
      }
      else if( gHistogram *tmpHistogram = itemData->getHistogram() )
      {
        if( tmpHistogram->GetHistogram()->getShowWindow() )
          tmpHistogram->Show( iconize );
      }
      
      if( tree->ItemHasChildren( currentChild ) )
        iconizeWindows( tree, currentChild, iconize );
    }

    currentChild = tree->GetNextChild( id, cookie );
    ++current;
  }
}

int getIconNumber( Window *whichWindow )
{
  int iconNumber = 1; // number of timeline icon
  if ( whichWindow->isDerivedWindow() )
  {
    string derivedFunctionName = whichWindow->getLevelFunction( DERIVED );

    // GUI should'nt know these tags -> add operation to kernel
    if ( derivedFunctionName == "add" )
      iconNumber = 2;
    else if  ( derivedFunctionName == "product" )
      iconNumber = 3;
    else if  ( derivedFunctionName == "substract" )
      iconNumber = 4;
    else if  ( derivedFunctionName == "divide" )
      iconNumber = 5;
    else if  ( derivedFunctionName == "maximum" )
      iconNumber = 6;
    else if  ( derivedFunctionName == "minimum" )
      iconNumber = 7;
    else if  ( derivedFunctionName == "different" )
      iconNumber = 8;
    else if  ( derivedFunctionName == "controlled: clear by" )
      iconNumber = 9;
    else if  ( derivedFunctionName == "controlled: maximum" )
      iconNumber = 10;
    else if ( derivedFunctionName == "controlled: add" )
      iconNumber = 11;
    else if ( derivedFunctionName == "controlled: enumerate" )
      iconNumber = 12;
  }
  
  return iconNumber;
}
