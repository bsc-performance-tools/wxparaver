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

#include "ghistogram.h"
#include "gtimeline.h"
#include "loadedwindows.h"
#include "paravermain.h"

#include <wx/display.h>

using namespace std;

wxTreeCtrl *createTree( wxImageList *imageList )
{
  wxChoicebook *choiceWindowBrowser = paraverMain::myParaverMain->choiceWindowBrowser;
  wxTreeCtrl *newTree =
    new wxTreeCtrl( choiceWindowBrowser, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_HIDE_ROOT | wxTR_DEFAULT_STYLE | wxTR_MULTIPLE );
#ifndef _WIN32
  newTree->SetWindowStyle( wxTR_HAS_BUTTONS | wxTR_HIDE_ROOT | wxTR_MULTIPLE );
#endif
  newTree->SetImageList( imageList );
  newTree->AddRoot( wxT( "Root" ), -1, -1, new TreeBrowserItemData( _( "Root" ), (gTimeline *)nullptr ) );

  return newTree;
}


wxTreeCtrl *getAllTracesTree()
{
  wxChoicebook *choiceWindowBrowser = paraverMain::myParaverMain->choiceWindowBrowser;

  return (wxTreeCtrl *)choiceWindowBrowser->GetPage( 0 );
}


wxTreeCtrl *getSelectedTraceTree( Trace *trace )
{
  wxChoicebook *choiceWindowBrowser = paraverMain::myParaverMain->choiceWindowBrowser;

  PRV_INT16 currentTrace = paraverMain::myParaverMain->getTracePosition( trace );

  return (wxTreeCtrl *)choiceWindowBrowser->GetPage( currentTrace + 1 );
}


void appendHistogram2Tree( gHistogram *ghistogram,
                           wxTreeCtrl *rootAllTraces,
                           wxTreeItemId idRootAllTraces,
                           wxTreeCtrl *rootCurrentPage,
                           wxTreeItemId idRootCurrentPage )
{
  // Refresh tree in current page and always in global page
  if ( rootAllTraces == nullptr && rootCurrentPage == nullptr )
  {
    rootAllTraces = getAllTracesTree();
    idRootAllTraces = rootAllTraces->GetRootItem();

    rootCurrentPage = getSelectedTraceTree( ghistogram->GetHistogram()->getControlWindow()->getTrace() );
    idRootCurrentPage = rootCurrentPage->GetRootItem();
  }

  rootCurrentPage->UnselectAll();

  TreeBrowserItemData *currentData = new TreeBrowserItemData( wxString::FromUTF8( ghistogram->GetHistogram()->getName().c_str() ), ghistogram );

  int iconNumber = getIconNumber( ghistogram->GetHistogram() );
  wxTreeItemId tmpCurrentWindowId1 = rootAllTraces->AppendItem( idRootAllTraces,
                                                                wxString::FromUTF8( ghistogram->GetHistogram()->getName().c_str() ),
                                                                iconNumber,
                                                                -1,
                                                                currentData );
  wxTreeItemId tmpCurrentWindowId2 = rootCurrentPage->AppendItem( idRootCurrentPage,
                                                                  wxString::FromUTF8( ghistogram->GetHistogram()->getName().c_str() ),
                                                                  iconNumber,
                                                                  -1,
                                                                  new TreeBrowserItemData( *currentData ) );
  if (ghistogram->GetHistogram()->isDerivedHistogram())
      std::cout << "Derived " << std::endl;
  
  else
      std::cout << "Single " << std::endl;
  
  for( auto parent : ghistogram->GetHistogram()->getParents() )
  {
    gHistogram *tmpHistogram = getGHistogramFromWindow( getAllTracesTree()->GetRootItem(), parent );
    if( tmpHistogram != nullptr )
    {
      appendHistogram2Tree( tmpHistogram, rootAllTraces, tmpCurrentWindowId1, rootCurrentPage, tmpCurrentWindowId2 );
      std::cout << parent->getName() << std::endl;
    }
    else
    {
      std::cout << "nullptr!" << std::endl;
    }
  }
}


wxTreeItemId getItemIdFromGTimeline( wxTreeItemId root, gTimeline *wanted, bool &found )
{
  wxTreeItemIdValue cookie;

  found = false;

  wxTreeItemId itemCurrent = getAllTracesTree()->GetFirstChild( root, cookie );
  wxTreeItemId itemLast    = getAllTracesTree()->GetLastChild( root );

  while( !found && itemCurrent.IsOk() && itemCurrent != itemLast )
  {
    gTimeline *tmpTimeline = ( (TreeBrowserItemData *)( getAllTracesTree()->GetItemData( itemCurrent ) ) )->getTimeline();
    if( tmpTimeline != nullptr && tmpTimeline == wanted )
    {
      root  = itemCurrent;
      found = true;
    }
    else if( tmpTimeline != nullptr )
    {
      root = getItemIdFromGTimeline( itemCurrent, wanted, found );
    }
    if( !found )
      itemCurrent = getAllTracesTree()->GetNextChild( root, cookie );
  }

  if( !found && itemLast.IsOk() )
  {
    if( ( (TreeBrowserItemData *)( getAllTracesTree()->GetItemData( itemLast ) ) )->getTimeline() == wanted )
    {
      root  = itemLast;
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
gTimeline *getGTimelineFromWindow( wxTreeItemId root, Timeline *wanted, bool &found )
{
  gTimeline *retgt = nullptr;
  wxTreeItemIdValue cookie;

  found = false;

  wxTreeItemId itemCurrent = getAllTracesTree()->GetFirstChild( root, cookie );
  wxTreeItemId itemLast    = getAllTracesTree()->GetLastChild( root );

  while( !found && itemCurrent.IsOk() && itemCurrent != itemLast )
  {
    gTimeline *tmpTimeline = ( (TreeBrowserItemData *)( getAllTracesTree()->GetItemData( itemCurrent ) ) )->getTimeline();
    if( tmpTimeline != nullptr && tmpTimeline->GetMyWindow() == wanted )
    {
      retgt = tmpTimeline;
      found = true;
    }
    else if( tmpTimeline != nullptr )
    {
      retgt = getGTimelineFromWindow( itemCurrent, wanted, found );
    }
    if( !found )
      itemCurrent = getAllTracesTree()->GetNextChild( root, cookie );
  }

  if( !found && itemLast.IsOk() )
  {
    if( ( (TreeBrowserItemData *)( getAllTracesTree()->GetItemData( itemLast ) ) )->getTimeline()->GetMyWindow() == wanted )
    {
      retgt = ( (TreeBrowserItemData *)( getAllTracesTree()->GetItemData( itemLast ) ) )->getTimeline();
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
  gHistogram *retgh = nullptr;
  wxTreeItemIdValue cookie;

  bool found = false;

  wxTreeItemId itemCurrent = getAllTracesTree()->GetFirstChild( root, cookie );
  wxTreeItemId itemLast    = getAllTracesTree()->GetLastChild( root );

  while( !found && itemCurrent.IsOk() && itemCurrent != itemLast )
  {
    gHistogram *tmpHistogram = ( (TreeBrowserItemData *)( getAllTracesTree()->GetItemData( itemCurrent ) ) )->getHistogram();
    if( tmpHistogram != nullptr && tmpHistogram->GetHistogram() == wanted )
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
      retgh = ( (TreeBrowserItemData *)( getAllTracesTree()->GetItemData( itemLast ) ) )->getHistogram();
      found = true;
    }
  }

  return retgh;
}


wxTreeItemId getItemIdFromWindow( wxTreeItemId root, Timeline *wanted, bool &found )
{
  wxTreeItemId retItemId;
  wxTreeItemIdValue cookie;

  found = false;

  wxTreeItemId itemCurrent = getAllTracesTree()->GetFirstChild( root, cookie );
  wxTreeItemId itemLast    = getAllTracesTree()->GetLastChild( root );

  while( !found && itemCurrent.IsOk() && itemCurrent != itemLast )
  {
    gTimeline *tmpTimeline = ( (TreeBrowserItemData *)( getAllTracesTree()->GetItemData( itemCurrent ) ) )->getTimeline();
    if( tmpTimeline != nullptr && tmpTimeline->GetMyWindow() == wanted )
    {
      retItemId = itemCurrent;
      found     = true;
    }
    else if( tmpTimeline != nullptr )
    {
      retItemId = getItemIdFromWindow( itemCurrent, wanted, found );
    }
    if( !found )
      itemCurrent = getAllTracesTree()->GetNextChild( root, cookie );
  }

  if( !found && itemLast.IsOk() )
  {
    if( ( (TreeBrowserItemData *)( getAllTracesTree()->GetItemData( itemLast ) ) )->getTimeline()->GetMyWindow() == wanted )
    {
      retItemId = itemLast;
      found     = true;
    }
    else
    {
      retItemId = getItemIdFromWindow( itemLast, wanted, found );
    }
  }

  return retItemId;
}


// precond : current is a derived gTimeline
void getParentGTimeline( gTimeline *current, vector< gTimeline * > &parents )
{
  // find item for given current gTimeline.
  bool found;
  wxTreeItemId item = getItemIdFromGTimeline( getAllTracesTree()->GetRootItem(), current, found );

  // fill vector with parents
  wxTreeItemIdValue cookie;
  parents.push_back(
    ( (TreeBrowserItemData *)( getAllTracesTree()->GetItemData( getAllTracesTree()->GetFirstChild( item, cookie ) ) ) )->getTimeline() );
  parents.push_back(
    ( (TreeBrowserItemData *)( getAllTracesTree()->GetItemData( getAllTracesTree()->GetNextChild( item, cookie ) ) ) )->getTimeline() );
}


void BuildTree( paraverMain *parent,
                wxTreeCtrl *root1,
                wxTreeItemId idRoot1,
                wxTreeCtrl *root2,
                wxTreeItemId idRoot2,
                Timeline *window,
                string nameSuffix )
{
  root1->UnselectAll();
  root2->UnselectAll();

  wxTreeItemId currentWindowId1, currentWindowId2;
  TreeBrowserItemData *currentData;

  string composedName = window->getName() + " @ " + window->getTrace()->getTraceName();

  wxPoint tmpPos( window->getPosX(), window->getPosY() );
  if( wxDisplay::GetCount() > 1 /*&& ParaverConfig::???*/ )
  {
    int currentDisplay = wxDisplay::GetFromWindow( paraverMain::myParaverMain );
    if( currentDisplay != wxNOT_FOUND && currentDisplay >= 0 )
    {
      wxDisplay tmpDisplay( currentDisplay );
      tmpPos.x += tmpDisplay.GetGeometry().x;
      tmpPos.y += tmpDisplay.GetGeometry().y;
      if( tmpPos.x != window->getPosX() )
        window->setPosX( tmpPos.x );
      if( tmpPos.y != window->getPosY() )
        window->setPosY( tmpPos.y );
    }
  }

#if !__WXGTK__
  gTimeline *tmpTimeline = new gTimeline( parent, wxID_ANY, wxString::FromUTF8( composedName.c_str() ), tmpPos );
#else
  gTimeline *tmpTimeline = new gTimeline( parent, wxID_ANY, wxString::FromUTF8( composedName.c_str() ) );
#endif
  LoadedWindows::getInstance()->add( window );
  tmpTimeline->SetMyWindow( window );
  tmpTimeline->InitMyWindowCallbacks();
  tmpTimeline->SetClientSize( window->getWidth(), window->getHeight() );
#if __WXGTK__
  tmpTimeline->Move( tmpPos );
#endif

  currentData = new TreeBrowserItemData( wxString::FromUTF8( window->getName().c_str() ), tmpTimeline );

  int iconNumber   = getIconNumber( window );
  currentWindowId1 = root1->AppendItem( idRoot1, "", iconNumber, -1, currentData );
  currentWindowId2 = root2->AppendItem( idRoot2, "", iconNumber, -1, new TreeBrowserItemData( *currentData ) );

  if( window->getParent( 0 ) != nullptr )
  {
    BuildTree( parent, root1, currentWindowId1, root2, currentWindowId2, window->getParent( 0 ) );
    BuildTree( parent, root1, currentWindowId1, root2, currentWindowId2, window->getParent( 1 ) );
  }
  parent->SetCurrentWindow( (wxWindow *)tmpTimeline );
}


bool updateTreeItem( wxTreeCtrl *tree,
                     wxTreeItemId &id,
                     vector< Timeline * > &allWindows,
                     vector< Histogram * > &allHistograms,
                     wxWindow **currentWindow,
                     bool allTracesTree )
{
  bool destroy                  = false;
  TreeBrowserItemData *itemData = (TreeBrowserItemData *)tree->GetItemData( id );

  // No matter timeline or histogram, get its name and delete from given vector
  wxString tmpName;
  wxString tmpWindowName;

  bool isEditMode;

  if( gTimeline *tmpTimeline = itemData->getTimeline() )
  {
    isEditMode            = tmpTimeline->getEditMode();
    Timeline *tmpWindow   = tmpTimeline->GetMyWindow();
    std::string groupName = "";
    if( tmpWindow->isSync() )
    {
      int windowGroup = tmpWindow->getSyncGroup() + 1;

      groupName = "[#" + std::to_string( windowGroup ) + "] ";
      tmpName = groupName + wxString::FromUTF8( tmpWindow->getName().c_str() );
      if( !isEditMode && tmpName != tree->GetItemText( id ) )
      {
        tree->SetItemBold( id, true );

        int r = 0, g = 0, b = 0;

        getGroupColor( windowGroup, r, g, b );

        tree->SetItemTextColour( id, wxColour( r, g, b ) );
        tree->SetItemText( id, tmpName );
      }
    }
    else
    {
      tmpName = wxString::FromUTF8( tmpWindow->getName().c_str() );
      if( !isEditMode && tmpName != tree->GetItemText( id ) )
      {
        tree->SetItemTextColour( id, wxColour( 0, 0, 0 ) );
        tree->SetItemBold( id, false );
        tree->SetItemText( id, tmpName );
      }
    }
    if( tmpTimeline->IsActive() && !tmpWindow->getDestroy() )
    {
      *currentWindow = tmpTimeline;

      tree->SelectItem( id );
    }
    tmpWindowName = wxString::FromUTF8( tmpWindow->getName().c_str() );

    for( vector< Timeline * >::iterator it = allWindows.begin(); it != allWindows.end(); it++ )
    {
      if( *it == tmpWindow )
      {
        allWindows.erase( it );
        break;
      }
    }

    if( tmpWindow->getDestroy() )
    {
      if( paraverMain::myParaverMain->GetCurrentTimeline() == tmpWindow )
      {
        paraverMain::myParaverMain->SetCurrentTimeline( nullptr );
        paraverMain::myParaverMain->clearProperties();
      }
      if( !allTracesTree )
        tmpTimeline->Destroy();
      Timeline *parent1 = tmpWindow->getParent( 0 );
      if( parent1 != nullptr )
      {
        parent1->setChild( nullptr );
        parent1->setDestroy( true );
      }
      Timeline *parent2 = tmpWindow->getParent( 1 );
      if( parent2 != nullptr )
      {
        parent2->setChild( nullptr );
        parent2->setDestroy( true );
      }
      destroy = true;
    }
  }
  else if( gHistogram *tmpHistogram = itemData->getHistogram() )
  {
    isEditMode            = tmpHistogram->getEditMode();
    Histogram *tmpHisto   = tmpHistogram->GetHistogram();

    std::string groupName = "";

    if( tmpHisto->isSync() )
    {
      int windowGroup = tmpHisto->getSyncGroup() + 1;

      groupName = "[#" + std::to_string( windowGroup ) + "] ";
      tmpName   = groupName + wxString::FromUTF8( tmpHisto->getName().c_str() );

      if( !isEditMode && tmpName != tree->GetItemText( id ) )
      {
        tree->SetItemBold( id, true );

        int r = 0, g = 0, b = 0;

        getGroupColor( windowGroup, r, g, b );

        tree->SetItemTextColour( id, wxColour( r, g, b ) );
        tree->SetItemText( id, tmpName );
      }
    }
    else
    {
      tmpName = wxString::FromUTF8( tmpHisto->getName().c_str() );
      if( !isEditMode && tmpName != tree->GetItemText( id ) )
      {
        tree->SetItemTextColour( id, wxColour( 0, 0, 0 ) );
        tree->SetItemBold( id, false );
        tree->SetItemText( id, tmpName );
      }
    }

    if( tmpHistogram->IsActive() && !tmpHisto->getDestroy() )
    {
      *currentWindow = tmpHistogram;
      tree->SelectItem( id );
    }

    for( vector< Histogram * >::iterator it = allHistograms.begin(); it != allHistograms.end(); it++ )
    {
      if( *it == tmpHisto )
      {
        allHistograms.erase( it );
        break;
      }
    }

    if( tmpHisto->getDestroy() )
    {
      if( paraverMain::myParaverMain->GetCurrentHisto() == tmpHisto )
      {
        paraverMain::myParaverMain->SetCurrentHisto( nullptr );
        paraverMain::myParaverMain->clearProperties();
      }
      if( !allTracesTree )
        tmpHistogram->Destroy();
      destroy = true;
    }
  }


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
    if( !candidate.IsOk() )
      candidate = tree->GetPrevSibling( id );

    if( candidate.IsOk() )
    {
      if( gTimeline *tmpTimeline = itemData->getTimeline() )
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


void iconizeWindows( wxTreeCtrl *tree, wxTreeItemId &id, bool iconize )
{
  wxTreeItemIdValue cookie;
  wxTreeItemId currentChild = tree->GetFirstChild( id, cookie );
  unsigned int numberChild  = tree->GetChildrenCount( id, false );
  unsigned int current      = 0;
  while( current < numberChild )
  {
    if( currentChild.IsOk() )
    {
      TreeBrowserItemData *itemData = (TreeBrowserItemData *)tree->GetItemData( currentChild );

      if( gTimeline *tmpTimeline = itemData->getTimeline() )
      {
        if( tmpTimeline->GetMyWindow()->getShowWindow() )
          tmpTimeline->Show( true );
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

void getGroupColor( const int &windowGroup, int &r, int &g, int &b )
{
  int intensity = 100 + 15 * windowGroup;

  switch( ( windowGroup ) % 3 )
  {
    case 0:
      r = intensity;
      g = intensity / 3;
      b = intensity / 3;
      break;
    case 1:
      r = intensity / 3;
      g = intensity;
      b = intensity / 3;
      break;
    case 2:
      r = intensity / 3;
      g = intensity / 3;
      b = intensity;
      break;
  }
}

template< class TWindow >
int getIconNumber( TWindow *whichWindow )
{
  if constexpr( std::is_same_v< TWindow, Timeline > )
  {
    if ( whichWindow->isDerivedWindow() )
      return paraverMain::iconPosByName[ whichWindow->getLevelFunction( DERIVED ) ];
    else
      return paraverMain::iconPosByName[ "timeline" ];
  }
  else
  {
    if ( whichWindow->isDerivedHistogram() )
      return paraverMain::iconPosByName[ whichWindow->getDerivedOperation() ];
    else
      return paraverMain::iconPosByName[ "histogram" ];
  }

  return paraverMain::myParaverMain->iconPosByName[ "timeline" ];
}
