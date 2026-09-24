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

#include "windowstreeselector.h"

#include "histogram.h"
#include "trace.h"
#include "window.h"
#include "windows_tree.h"

#include <wx/sizer.h>

#include <map>
#include <variant>

using std::map;
using std::vector;

constexpr wxWindowID ID_TIMELINETREE = 10001;


BEGIN_EVENT_TABLE( WindowsTreeSelector, wxDialog )
  EVT_TREE_ITEM_ACTIVATED( ID_TIMELINETREE, WindowsTreeSelector::OnTreeItemActivated )
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS( WindowsTreeSelector, wxDialog )


class WindowSelectorItemData: public wxTreeItemData
{
  public:
    WindowSelectorItemData( const wxString& whichDesc, TWindowType whichWin ) :
      desc( whichDesc ), myWindow( whichWin )
    {}
    
    const wxString& GetDesc() const
    {
      return desc;
    }
    
    TWindowType getWindow() const
    {
      return myWindow;
    }
    
  private:
    wxString desc;
    TWindowType myWindow {};
};


WindowsTreeSelector::WindowsTreeSelector( wxWindow* parent,
                                          wxWindowID id,
                                          const wxString& title,
                                          const std::vector< TWindowID >& windows,
                                          const TWindowType& currentWindow,
                                          const Trace *currentTrace,
                                          bool needNoneElement,
                                          const wxPoint& pos,
                                          const wxSize& size,
                                          long style,
                                          const wxString& name ) :
  wxDialog( parent, id, title, pos, size, style, name )
{
  CreateControls();
  fillTree( windows, currentWindow, currentTrace, needNoneElement );
}


TWindowType WindowsTreeSelector::getSelection() const
{
  WindowSelectorItemData *currentItem = ( WindowSelectorItemData* ) timelineTree->GetItemData( timelineTree->GetSelection() );
  return currentItem->getWindow();
}

// Timeline *WindowsTreeSelector::getSelection() const
// {
//   TWindowType *currentWindow = getSelection();
//   if( std::holds_alternative< Histogram * >( currentWindow ) )
//     return nullptr;

//   return std::get< Timeline * >( currentWindow );
// }

// Histogram *WindowsTreeSelector::getSelection() const
// {
//   TWindowType *currentWindow = getSelection();
//   if( std::holds_alternative< Timeline * >( currentWindow ) )
//     return nullptr;

//   return std::get< Histogram * >( currentWindow );
// }

void WindowsTreeSelector::CreateControls()
{
  wxBoxSizer* itemBoxSizer = new wxBoxSizer( wxVERTICAL );
  this->SetSizer( itemBoxSizer );

  timelineTree = new wxTreeCtrl( this, ID_TIMELINETREE, wxDefaultPosition, wxDefaultSize, wxTR_HIDE_ROOT|wxTR_SINGLE|wxTR_HAS_BUTTONS|wxTR_FULL_ROW_HIGHLIGHT|wxTR_DEFAULT_STYLE );
  itemBoxSizer->Add( timelineTree, 1, wxGROW );
}


void WindowsTreeSelector::OnTreeItemActivated( wxTreeEvent& event )
{
  if( timelineTree->GetItemParent( event.GetItem() ) != timelineTree->GetRootItem() || 
      timelineTree->GetItemText( event.GetItem() ) == wxT( "None" ) )
    EndModal( wxID_OK );
}


void WindowsTreeSelector::fillTree( const std::vector< TWindowID >& windows, const TWindowType& currentWindow, const Trace *currentTrace, bool needNoneElement )
{
  TWindowID currentId{};

  timelineTree->SetImageList( paraverMain::myParaverMain->GetImageList() );
  
  wxTreeItemId root = timelineTree->AddRoot( wxT( "root" ) );
  if( needNoneElement )
  {
    wxTreeItemId noneId = timelineTree->AppendItem( root, wxT( "None" ), -1, -1, new WindowSelectorItemData( wxT( "None" ), std::monostate{} ) );
    if( std::holds_alternative< std::monostate >( currentWindow ) )
      timelineTree->SelectItem( noneId );
  }
  wxTreeItemId currentTraceId = timelineTree->AppendItem( root, wxString( currentTrace->getFileNameNumbered().c_str(), wxConvUTF8 ) );

  map< Trace *, wxTreeItemId > traceRoot;
  for( vector< TWindowID >::const_iterator it = windows.begin(); it != windows.end(); ++it )
  {
    currentId = *it;

    Trace *tmpCurrentTrace;
    TWindowType currentWin;
    if( std::holds_alternative< Timeline * >( currentWindow ) )
    {
      Timeline *tmpWin = LoadedWindows::getInstance()->getWindow( currentId );
      if( tmpWin->getChild() != nullptr )
        continue;
      tmpCurrentTrace = tmpWin->getTrace();
      currentWin = tmpWin;
    }
    else if( std::holds_alternative< Histogram * >( currentWindow ) )
    {
      Histogram *tmpWin = LoadedWindows::getInstance()->getHisto( currentId );
      if( tmpWin->haveChildren() )
        continue;
      tmpCurrentTrace = tmpWin->getTrace();
      currentWin = tmpWin;
    }

    if( tmpCurrentTrace == currentTrace )
      addTreeItem( currentWin, currentWindow, currentTraceId );
    else
    {
      map< Trace *, wxTreeItemId >::iterator itMap = traceRoot.find( tmpCurrentTrace );
      wxTreeItemId insertId;
      if( itMap != traceRoot.end() )
        insertId = itMap->second;
      else
      {
        insertId = timelineTree->AppendItem( root, wxString( tmpCurrentTrace->getFileNameNumbered().c_str(), wxConvUTF8 ) );
        traceRoot[ tmpCurrentTrace ] = insertId;
      }
      addTreeItem( currentWin, currentWindow, insertId );
    }
  }
  
  timelineTree->Expand( currentTraceId );
}


void WindowsTreeSelector::addTreeItem( TWindowType& whichWindow, const TWindowType& currentWindow, wxTreeItemId whichParent )
{
  auto visitorGetName = []( const auto& window )
                        {
                          using T = std::decay_t< decltype( window ) >;
                          if constexpr ( std::is_same_v< T, std::monostate > )
                            return std::string{};
                          else
                            return window->getName();
                        };
  // auto visitorGetIcon = []( auto&& window ) { return getIconNumber( window ); };

  wxTreeItemId tmpId = timelineTree->AppendItem( whichParent, wxString( std::visit( visitorGetName, whichWindow ).c_str(), wxConvUTF8 ), getIconNumber( whichWindow ), -1,
                                                 new WindowSelectorItemData( wxString( std::visit( visitorGetName, whichWindow ).c_str(), wxConvUTF8 ), whichWindow ) );
  if( whichWindow == currentWindow ) // TODO: correct?
    timelineTree->SelectItem( tmpId );

  if( std::holds_alternative< Timeline * >( whichWindow ) ) 
  {
    Timeline *tmpTimeline = std::get< Timeline * >( whichWindow );

    if( tmpTimeline->isDerivedWindow() )
    {
      TWindowType parent1 = TWindowType( tmpTimeline->getParent( 0 ) );
      addTreeItem( parent1, currentWindow, tmpId );
      TWindowType parent2 = TWindowType( tmpTimeline->getParent( 1 ) );
      addTreeItem( parent2, currentWindow, tmpId );
    }
  }
  else if( std::holds_alternative< Histogram * >( whichWindow ) )
  {
    // TODO: Add parents?
    //&& whichWindow->isDerivedHistogram() 
  }
}
