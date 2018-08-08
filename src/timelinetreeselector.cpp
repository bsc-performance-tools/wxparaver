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

#include "timelinetreeselector.h"
#include <wx/treectrl.h>
#include <wx/sizer.h>

#define ID_TIMELINETREE 10001

BEGIN_EVENT_TABLE( TimelineTreeSelector, wxMiniFrame )
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS( TimelineTreeSelector, wxMiniFrame )

TimelineTreeSelector::TimelineTreeSelector( wxWindow* parent,
                                            wxWindowID id,
                                            const wxString& title,
                                            const wxPoint& pos,
                                            const wxSize& size,
                                            long style,
                                            const wxString& name ) :
  wxMiniFrame( parent, id, title, pos, size, style, name )
{

}

void TimelineTreeSelector::CreateControls()
{
  wxBoxSizer* itemBoxSizer = new wxBoxSizer( wxVERTICAL );
  this->SetSizer( itemBoxSizer );

  timelineTree = new wxTreeCtrl( this, ID_TIMELINETREE );
  itemBoxSizer->Add( timelineTree, 1, wxGROW );
}
