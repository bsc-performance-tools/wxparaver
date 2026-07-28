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

#include "derivedwindowdialog.h"

////@begin XPM images
#include "../icons/derived_add.xpm"
#include "../icons/three_dots.xpm"
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
  EVT_TEXT_ENTER( ID_TEXTCTRL_DERIVED_HISTOGRAM_NAME, DerivedHistogramDialog::OnTextctrlDerivedHistogramNameEnter )
  EVT_BUTTON( ID_BITMAPBUTTON, DerivedHistogramDialog::OnBitmapbuttonClick )
  EVT_UPDATE_UI( ID_BITMAPBUTTON, DerivedHistogramDialog::OnBitmapbuttonUpdate )
  EVT_BUTTON( ID_BITMAPBUTTON_ADD_PARENT_WINDOW, DerivedHistogramDialog::OnBitmapbuttonAddParentWindowClick )
  EVT_BUTTON( ID_BITMAPBUTTON_DELETE_PARENT_WINDOW, DerivedHistogramDialog::OnBitmapbuttonDeleteParentWindowClick )
  EVT_UPDATE_UI( ID_BITMAPBUTTON_DELETE_PARENT_WINDOW, DerivedHistogramDialog::OnBitmapbuttonDeleteParentWindowUpdate )
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
  derivedWindowName = wxT("");
  derivedHistogramName = NULL;
  panelHistogramLine1 = NULL;
  panelHistogramLine2 = NULL;
  buttonOperation = NULL;
  buttonSelectWindow = NULL;
  buttonAddParentWindow = NULL;
  buttonDeleteParentWindow = NULL;
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
  itemBoxSizer2->Add(itemBoxSizer1, 0, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText2 = new wxStaticText( itemDialog1, wxID_STATIC, _("Name"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
  itemBoxSizer1->Add(itemStaticText2, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

  derivedHistogramName = new wxTextCtrl( itemDialog1, ID_TEXTCTRL_DERIVED_HISTOGRAM_NAME, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0 );
  itemBoxSizer1->Add(derivedHistogramName, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer2->Add(itemBoxSizer3, 1, wxGROW|wxTOP|wxBOTTOM, 5);

  wxStaticBox* itemStaticBoxSizer1Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Histograms "));
  wxStaticBoxSizer* itemStaticBoxSizer1 = new wxStaticBoxSizer(itemStaticBoxSizer1Static, wxHORIZONTAL);
  itemBoxSizer3->Add(itemStaticBoxSizer1, 1, wxGROW|wxALL, 5);

  wxScrolledWindow* itemScrolledWindow2 = new wxScrolledWindow( itemDialog1, ID_SCROLLEDWINDOW_HISTOGRAM_LIST, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxVSCROLL|wxALWAYS_SHOW_SB );
  itemStaticBoxSizer1->Add(itemScrolledWindow2, 1, wxGROW|wxALL, 5);
  itemScrolledWindow2->SetScrollbars(1, 1, 0, 0);
  wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
  itemScrolledWindow2->SetSizer(itemBoxSizer4);

  panelHistogramLine1 = new wxPanel( itemScrolledWindow2, ID_PANEL_1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
  panelHistogramLine1->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer4->Add(panelHistogramLine1, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
  panelHistogramLine1->SetSizer(itemBoxSizer5);

  wxBitmapButton* itemBitmapButton6 = new wxBitmapButton( panelHistogramLine1, ID_BITMAPBUTTON1, itemDialog1->GetBitmapResource(wxT("icons/derived_add.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBitmapButton6->Show(false);
  itemBoxSizer5->Add(itemBitmapButton6, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRESERVE_SPACE_EVEN_IF_HIDDEN, 5);

  itemBoxSizer5->Add(0, 0, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxTextCtrl* itemTextCtrl8 = new wxTextCtrl( panelHistogramLine1, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0 );
  itemBoxSizer5->Add(itemTextCtrl8, 6, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP|wxFIXED_MINSIZE, 5);

  wxBitmapButton* itemBitmapButton9 = new wxBitmapButton( panelHistogramLine1, ID_BITMAPBUTTON, itemDialog1->GetBitmapResource(wxT("icons/three_dots.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer5->Add(itemBitmapButton9, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxCheckBox* itemCheckBox10 = new wxCheckBox( panelHistogramLine1, ID_CHECKBOX1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemCheckBox10->SetValue(false);
  itemBoxSizer5->Add(itemCheckBox10, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  panelHistogramLine2 = new wxPanel( itemScrolledWindow2, ID_PANEL_2, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
  panelHistogramLine2->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer4->Add(panelHistogramLine2, 0, wxGROW|wxALL, 5);
  wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
  panelHistogramLine2->SetSizer(itemBoxSizer12);

  buttonOperation = new wxBitmapButton( panelHistogramLine2, ID_BITMAPBUTTON3, itemDialog1->GetBitmapResource(wxT("icons/derived_add.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer12->Add(buttonOperation, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

  itemBoxSizer12->Add(5, 5, 2, wxALIGN_CENTER_VERTICAL, 5);

  wxTextCtrl* itemTextCtrl15 = new wxTextCtrl( panelHistogramLine2, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0 );
  itemBoxSizer12->Add(itemTextCtrl15, 6, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP|wxFIXED_MINSIZE, 5);

  buttonSelectWindow = new wxBitmapButton( panelHistogramLine2, ID_BITMAP_HISTO_BUTTON2, itemDialog1->GetBitmapResource(wxT("icons/three_dots.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer12->Add(buttonSelectWindow, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

  wxCheckBox* itemCheckBox17 = new wxCheckBox( panelHistogramLine2, ID_CHECKBOX, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  itemCheckBox17->SetValue(false);
  itemBoxSizer12->Add(itemCheckBox17, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);


  itemScrolledWindow2->SetMinSize(wxSize(200, 100));

  wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer1->Add(itemBoxSizer6, 0, wxALIGN_TOP|wxLEFT|wxRIGHT|wxBOTTOM, 5);

  buttonAddParentWindow = new wxBitmapButton( itemDialog1, ID_BITMAPBUTTON_ADD_PARENT_WINDOW, itemDialog1->GetBitmapResource(wxT("icons/add.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer6->Add(buttonAddParentWindow, 0, wxALIGN_CENTER_HORIZONTAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

  buttonDeleteParentWindow = new wxBitmapButton( itemDialog1, ID_BITMAPBUTTON_DELETE_PARENT_WINDOW, itemDialog1->GetBitmapResource(wxT("icons/delete.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer6->Add(buttonDeleteParentWindow, 0, wxALIGN_CENTER_HORIZONTAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

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
  if (name == wxT("icons/derived_add.xpm"))
  {
    wxBitmap bitmap(derived_add_xpm);
    return bitmap;
  }
  else if (name == wxT("icons/three_dots.xpm"))
  {
    wxBitmap bitmap(three_dots_xpm);
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


/*!
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_TEXTCTRL
 */

void DerivedHistogramDialog::OnTextctrlDerivedHistogramNameEnter( wxCommandEvent& event )
{
  SetDerivedWindowName( event.GetString() );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON_DELETE_PARENT_HISTOGRAM
 */

void DerivedHistogramDialog::OnBitmapbuttonDeleteParentWindowClick( wxCommandEvent& event )
{
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON_ADD_PARENT_HISTOGRAM
 */

void DerivedHistogramDialog::OnBitmapbuttonAddParentWindowClick( wxCommandEvent& event )
{
  if( std::holds_alternative< Histogram * >( parentWindows.front() ) )
  {
    std::vector< Histogram * > derivableHistograms;
    
    paraverMain::myParaverMain->findCompatibleHistogramsForDerivation( std::get< Histogram * >( parentWindows.front() ), derivableHistograms );
    for( auto& h: derivableHistograms )
    {
      std::cout << h->getName() << std::endl;
    }
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BITMAPBUTTON_DELETE_PARENT_HISTOGRAM
 */

void DerivedHistogramDialog::OnBitmapbuttonDeleteParentWindowUpdate( wxUpdateUIEvent& event )
{
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON
 */

void DerivedHistogramDialog::OnBitmapbuttonClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON in DerivedHistogramDialog.
  // Before editing this code, remove the block markers.
  event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON in DerivedHistogramDialog. 
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BITMAPBUTTON
 */

void DerivedHistogramDialog::OnBitmapbuttonUpdate( wxUpdateUIEvent& event )
{
////@begin wxEVT_UPDATE_UI event handler for ID_BITMAPBUTTON in DerivedHistogramDialog.
  // Before editing this code, remove the block markers.
  event.Skip();
////@end wxEVT_UPDATE_UI event handler for ID_BITMAPBUTTON in DerivedHistogramDialog. 
}

