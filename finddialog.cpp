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

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "finddialog.h"

////@begin XPM images

////@end XPM images


/*!
 * FindDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( FindDialog, wxDialog )


/*!
 * FindDialog event table definition
 */

BEGIN_EVENT_TABLE( FindDialog, wxDialog )

////@begin FindDialog event table entries
////@end FindDialog event table entries

END_EVENT_TABLE()


/*!
 * FindDialog constructors
 */

FindDialog::FindDialog()
{
  Init();
}

FindDialog::FindDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}


/*!
 * FindDialog creator
 */

bool FindDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin FindDialog creation
  SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Centre();
////@end FindDialog creation
  return true;
}


/*!
 * FindDialog destructor
 */

FindDialog::~FindDialog()
{
////@begin FindDialog destruction
////@end FindDialog destruction
}


/*!
 * Member initialisation
 */

void FindDialog::Init()
{
////@begin FindDialog member initialisation
////@end FindDialog member initialisation
}


/*!
 * Control creation for FindDialog
 */

void FindDialog::CreateControls()
{    
////@begin FindDialog content construction
  FindDialog* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

////@end FindDialog content construction
}


/*!
 * Should we show tooltips?
 */

bool FindDialog::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap FindDialog::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin FindDialog bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
////@end FindDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon FindDialog::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin FindDialog icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end FindDialog icon retrieval
}
