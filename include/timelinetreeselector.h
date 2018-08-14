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


#ifndef _TIMELINETREESELECTOR_H_ 
#define _TIMELINETREESELECTOR_H_

#include <wx/dialog.h>
#include <wx/treectrl.h>
#include "loadedwindows.h"

class TimelineTreeSelector : public wxDialog
{
  DECLARE_DYNAMIC_CLASS( TimelineTreeSelector )
  DECLARE_EVENT_TABLE()

  public:
    TimelineTreeSelector()
    {}

    TimelineTreeSelector( wxWindow* parent,
                          wxWindowID id,
                          const wxString& title,
                          const std::vector<TWindowID>& windows,
                          const Trace *currentTrace,
                          const wxPoint& pos = wxDefaultPosition,
                          const wxSize& size = wxDefaultSize,
                          long style = wxDEFAULT_DIALOG_STYLE,
                          const wxString& name = wxT( "dialogBox" ) );

    ~TimelineTreeSelector()
    {}
    
  private:
    void CreateControls();
    void fillTree( const std::vector<TWindowID>& windows, const Trace *currentTrace );
    void addTreeItem( Window *whichWindow, wxTreeItemId whichParent );
    void OnTreeItemActivated( wxTreeEvent& event );
    
    wxTreeCtrl *timelineTree;
};

#endif //_TIMELINETREESELECTOR_H_
