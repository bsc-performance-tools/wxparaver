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

#include "exitdialog.h"

////@begin XPM images
#include "../icons/logout.xpm"
////@end XPM images


/*!
 * ExitDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ExitDialog, wxDialog )


/*!
 * ExitDialog event table definition
 */

BEGIN_EVENT_TABLE( ExitDialog, wxDialog )

////@begin ExitDialog event table entries
  EVT_BUTTON( ID_BUTTON_SAVE_EXIT, ExitDialog::OnButtonSaveExitClick )
  EVT_BUTTON( ID_BUTTON_CANCEL, ExitDialog::OnButtonCancelClick )
  EVT_BUTTON( ID_BUTTON_CLOSE_NO_SAVE, ExitDialog::OnButtonCloseNoSaveClick )
////@end ExitDialog event table entries

END_EVENT_TABLE()


/*!
 * ExitDialog constructors
 */

ExitDialog::ExitDialog()
{
  Init();
}

ExitDialog::ExitDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}


/*!
 * ExitDialog creator
 */

bool ExitDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin ExitDialog creation
  SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Centre();
////@end ExitDialog creation
  return true;
}


/*!
 * ExitDialog destructor
 */

ExitDialog::~ExitDialog()
{
////@begin ExitDialog destruction
////@end ExitDialog destruction
}


/*!
 * Member initialisation
 */

void ExitDialog::Init()
{
////@begin ExitDialog member initialisation
  saveExitButton = nullptr;
  cancelButton = nullptr;
  noSaveExitButton = nullptr;
////@end ExitDialog member initialisation
}


/*!
 * Control creation for ExitDialog
 */

void ExitDialog::CreateControls()
{    
////@begin ExitDialog content construction
  ExitDialog* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer1, 0, wxGROW|wxALL, 5);

  wxStaticBitmap* itemStaticBitmap1 = new wxStaticBitmap( itemDialog1, wxID_STATIC, itemDialog1->GetBitmapResource(wxT("icons/logout.xpm")), wxDefaultPosition, wxSize(48, 48), 0 );
  itemBoxSizer1->Add(itemStaticBitmap1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticText* itemStaticText2 = new wxStaticText( itemDialog1, wxID_STATIC, _("Some windows are already open... Do you want to save this session before closing?"), wxDefaultPosition, wxSize(270, -1), 0 );
  itemBoxSizer1->Add(itemStaticText2, 1, wxGROW|wxALL, 5);

  itemBoxSizer2->Add(5, 0, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

  saveExitButton = new wxButton( itemDialog1, ID_BUTTON_SAVE_EXIT, _("Save and exit"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer3->Add(saveExitButton, 1, wxGROW|wxALL, 5);

  cancelButton = new wxButton( itemDialog1, ID_BUTTON_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer3->Add(cancelButton, 1, wxGROW|wxALL, 5);

  noSaveExitButton = new wxButton( itemDialog1, ID_BUTTON_CLOSE_NO_SAVE, _("Close without saving"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer3->Add(noSaveExitButton, 1, wxGROW|wxALL, 5);

////@end ExitDialog content construction
}



/*!
 * Should we show tooltips?
 */

bool ExitDialog::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ExitDialog::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin ExitDialog bitmap retrieval
  wxUnusedVar(name);
  if (name == wxT("icons/logout.xpm"))
  {
    wxBitmap bitmap(logout_xpm);
    return bitmap;
  }
  return wxNullBitmap;
////@end ExitDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon ExitDialog::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin ExitDialog icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end ExitDialog icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SAVE_EXIT
 */
void ExitDialog::OnButtonSaveExitClick( wxCommandEvent& event )
{
  EndModal( wxID_NO );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CLOSE_NO_SAVE
 */
void ExitDialog::OnButtonCloseNoSaveClick( wxCommandEvent& event )
{
  EndModal( wxID_YES );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CANCEL
 */
void ExitDialog::OnButtonCancelClick( wxCommandEvent& event )
{
  EndModal( wxID_CANCEL );
}
