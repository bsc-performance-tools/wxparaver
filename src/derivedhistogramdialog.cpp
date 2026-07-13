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

#include "derivedhistogramdialog.h"

////@begin XPM images
#include "../icons/three_dots.xpm"
#include "../icons/derived_add.xpm"
#include "../icons/add.xpm"
#include "../icons/delete.xpm"
////@end XPM images


/*!
 * DerivedHistogramDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( DerivedHistogramDialog, wxDialog )


/*!
 * DerivedHistogramDialog event table definition
 */

BEGIN_EVENT_TABLE( DerivedHistogramDialog, wxDialog )

////@begin DerivedHistogramDialog event table entries
////@end DerivedHistogramDialog event table entries

END_EVENT_TABLE()


/*!
 * DerivedHistogramDialog constructors
 */

DerivedHistogramDialog::DerivedHistogramDialog()
{
  Init();
}

DerivedHistogramDialog::DerivedHistogramDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}

/*!
 * DerivedHistogramDialog creator
 */

bool DerivedHistogramDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin DerivedHistogramDialog creation
  SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Centre();
////@end DerivedHistogramDialog creation
  return true;
}


/*!
 * DerivedHistogramDialog destructor
 */

DerivedHistogramDialog::~DerivedHistogramDialog()
{
////@begin DerivedHistogramDialog destruction
////@end DerivedHistogramDialog destruction
}


/*!
 * Member initialisation
 */

void DerivedHistogramDialog::Init()
{
////@begin DerivedHistogramDialog member initialisation
  derivedHistogramName = NULL;
////@end DerivedHistogramDialog member initialisation
}


/*!
 * Control creation for DerivedHistogramDialog
 */

void DerivedHistogramDialog::CreateControls()
{    
////@begin DerivedHistogramDialog content construction
  DerivedHistogramDialog* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer1, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxStaticText* itemStaticText2 = new wxStaticText( itemDialog1, wxID_STATIC, _("Name"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
  itemBoxSizer1->Add(itemStaticText2, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  derivedHistogramName = new wxTextCtrl( itemDialog1, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0 );
  itemBoxSizer1->Add(derivedHistogramName, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer1Static = new wxStaticBox(itemDialog1, wxID_ANY, _(" Histograms "));
  wxStaticBoxSizer* itemStaticBoxSizer1 = new wxStaticBoxSizer(itemStaticBoxSizer1Static, wxHORIZONTAL);
  itemBoxSizer3->Add(itemStaticBoxSizer1, 1, wxGROW|wxALL, 5);

  wxScrolledWindow* itemScrolledWindow2 = new wxScrolledWindow( itemDialog1, ID_SCROLLEDWINDOW, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxVSCROLL|wxALWAYS_SHOW_SB );
  itemStaticBoxSizer1->Add(itemScrolledWindow2, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
  itemScrolledWindow2->SetScrollbars(1, 1, 0, 0);
  wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
  itemScrolledWindow2->SetSizer(itemBoxSizer4);

  wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer4->Add(itemBoxSizer5, 1, wxGROW|wxALL, 5);
  wxTextCtrl* itemTextCtrl6 = new wxTextCtrl( itemScrolledWindow2, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer5->Add(itemTextCtrl6, 0, wxGROW|wxALL, 5);

  wxBitmapButton* itemBitmapButton7 = new wxBitmapButton( itemScrolledWindow2, ID_BITMAPBUTTON, itemDialog1->GetBitmapResource(wxT("icons/three_dots.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer5->Add(itemBitmapButton7, 0, wxGROW|wxALL, 5);

  wxBitmapButton* itemBitmapButton8 = new wxBitmapButton( itemScrolledWindow2, ID_BITMAPBUTTON1, itemDialog1->GetBitmapResource(wxT("icons/derived_add.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer5->Add(itemBitmapButton8, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer4->Add(itemBoxSizer9, 1, wxGROW|wxALL, 5);
  wxTextCtrl* itemTextCtrl10 = new wxTextCtrl( itemScrolledWindow2, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer9->Add(itemTextCtrl10, 0, wxGROW|wxALL, 5);

  wxBitmapButton* itemBitmapButton11 = new wxBitmapButton( itemScrolledWindow2, ID_BITMAPBUTTON2, itemDialog1->GetBitmapResource(wxT("icons/three_dots.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer9->Add(itemBitmapButton11, 0, wxGROW|wxALL, 5);

  wxBitmapButton* itemBitmapButton12 = new wxBitmapButton( itemScrolledWindow2, ID_BITMAPBUTTON3, itemDialog1->GetBitmapResource(wxT("icons/derived_add.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer9->Add(itemBitmapButton12, 0, wxGROW|wxALL, 5);

  itemScrolledWindow2->SetMinSize(wxSize(200, 100));

  wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer1->Add(itemBoxSizer6, 0, wxALIGN_TOP|wxLEFT|wxRIGHT|wxBOTTOM, 5);

  wxBitmapButton* itemBitmapButton2 = new wxBitmapButton( itemDialog1, ID_BITMAPBUTTON4, itemDialog1->GetBitmapResource(wxT("icons/add.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer6->Add(itemBitmapButton2, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxBitmapButton* itemBitmapButton3 = new wxBitmapButton( itemDialog1, ID_BITMAPBUTTON5, itemDialog1->GetBitmapResource(wxT("icons/delete.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer6->Add(itemBitmapButton3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer1 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer1, 0, wxALIGN_RIGHT|wxALL, 5);
  wxButton* itemButton2 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer1->AddButton(itemButton2);

  wxButton* itemButton3 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer1->AddButton(itemButton3);

  itemStdDialogButtonSizer1->Realize();

////@end DerivedHistogramDialog content construction
}


/*!
 * Should we show tooltips?
 */

bool DerivedHistogramDialog::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap DerivedHistogramDialog::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin DerivedHistogramDialog bitmap retrieval
  wxUnusedVar(name);
  if (name == wxT("icons/three_dots.xpm"))
  {
    wxBitmap bitmap(three_dots_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/derived_add.xpm"))
  {
    wxBitmap bitmap(derived_add_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/add.xpm"))
  {
    wxBitmap bitmap(add);
    return bitmap;
  }
  else if (name == wxT("icons/delete.xpm"))
  {
    wxBitmap bitmap(delete_xpm);
    return bitmap;
  }
  return wxNullBitmap;
////@end DerivedHistogramDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon DerivedHistogramDialog::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin DerivedHistogramDialog icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end DerivedHistogramDialog icon retrieval
}
