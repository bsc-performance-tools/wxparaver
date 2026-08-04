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
#include "../icons/add.xpm"
////@end XPM images


// #include "../icons/derived_add.xpm"
// #include "../icons/derived_substract.xpm"
// #include "../icons/derived_product.xpm"
// #include "../icons/derived_divide.xpm"
// #include "../icons/derived_minimum.xpm"
// #include "../icons/derived_maximum.xpm"
// #include "../icons/derived_different.xpm"

#include "../icons/three_dots.xpm"

#include "../icons/delete.xpm"

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
  EVT_BUTTON( ID_BITMAPBUTTON_ADD_PARENT_WINDOW, DerivedHistogramDialog::OnButtonAddParentClick )
////@end DerivedHistogramDialog event table entries

END_EVENT_TABLE()

const wxString DerivedHistogramDialog::KTextCtrlSuffix  = _( "_TXTCTRL" );
const wxString DerivedHistogramDialog::KCheckBoxSuffix  = _( "_CHECKBOX" );
const wxString DerivedHistogramDialog::KButtonSuffix    = _( "_BUTTON" );


/*!
 * DerivedHistogramDialog constructors
 */

DerivedHistogramDialog::DerivedHistogramDialog()
{
  Init();
}

DerivedHistogramDialog::DerivedHistogramDialog( wxWindow* parent, const std::vector< TWindowType >& whichParents, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init( whichParents );
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
  DisconnectWidgets();
}


/*!
 * Member initialisation
 */

void DerivedHistogramDialog::Init( const std::vector< TWindowType >& whichParents )
{
////@begin DerivedHistogramDialog member initialisation
  derivedWindowName = wxT("");
  derivedHistogramName = NULL;
  histogramsScrolledWindow = NULL;
  buttonAddParentWindow = NULL;
////@end DerivedHistogramDialog member initialisation

  SetParentWindows( whichParents );
  imagesList = paraverMain::myParaverMain->GetImageList();
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

  histogramsScrolledWindow = new wxScrolledWindow( itemDialog1, ID_SCROLLEDWINDOW_HISTOGRAM_LIST, wxDefaultPosition, wxSize(200, 100), wxSUNKEN_BORDER|wxVSCROLL|wxALWAYS_SHOW_SB );
  itemStaticBoxSizer1->Add(histogramsScrolledWindow, 1, wxGROW|wxALL, 5);
  histogramsScrolledWindow->SetScrollbars(1, 1, 0, 0);

  wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxVERTICAL);
  itemStaticBoxSizer1->Add(itemBoxSizer6, 0, wxALIGN_TOP|wxLEFT|wxRIGHT|wxBOTTOM, 5);

  buttonAddParentWindow = new wxBitmapButton( itemDialog1, ID_BITMAPBUTTON_ADD_PARENT_WINDOW, itemDialog1->GetBitmapResource(wxT("icons/add.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
  itemBoxSizer6->Add(buttonAddParentWindow, 0, wxALIGN_CENTER_HORIZONTAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer1 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer1, 0, wxALIGN_RIGHT|wxALL, 5);
  wxButton* itemButton2 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer1->AddButton(itemButton2);

  wxButton* itemButton3 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer1->AddButton(itemButton3);

  itemStdDialogButtonSizer1->Realize();

////@end DerivedHistogramDialog content construction

  CreateControlsForParentsList();
}


wxString DerivedHistogramDialog::BuildName( const TWindowType& whichParent )
{
  return std::visit( [](const auto& current)
                     {
                       return wxString::FromUTF8( current->getName().c_str() ) + _( "_" ) +
                              wxString::FromUTF8( current->getTrace()->getTraceNameNumbered().c_str() );
                     }, whichParent );
}


void DerivedHistogramDialog::CreateControlsForParentRow( wxBoxSizer *whichBoxSizer,
                                                         const TWindowType& whichParent,
                                                         bool showOperation,
                                                         const wxBitmap& derivedOperationIcon,
                                                         int pos )
{
  // Create panel
  wxPanel* panelHistogramLine = new wxPanel( histogramsScrolledWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
  panelHistogramLine->SetExtraStyle( wxWS_EX_VALIDATE_RECURSIVELY );
  whichBoxSizer->Add( panelHistogramLine, 0, wxGROW|wxALL, 5 );


  // Create boxsizer
  wxBoxSizer* lineBoxSizer = new wxBoxSizer( wxHORIZONTAL );
  panelHistogramLine->SetSizer( lineBoxSizer );


  // Create operation button
  wxString tmpName {};
  if( showOperation )
  {
    managedWidgets.emplace_back( std::make_pair( BuildName( whichParent ) + wxT( "_operation_" ) + KButtonSuffix,
                                                pos )); //std::mem_fn( &DerivedHistogramDialog::OnButtonOperationSelectionClick ) ) );
    tmpName = managedWidgets.back().first;
  }
  wxBitmapButton* bitmapButtonOperation = new wxBitmapButton( panelHistogramLine,
                                                              // ID_BITMAPBUTTON1,
                                                              wxID_ANY,
                                                              derivedOperationIcon,
                                                              wxDefaultPosition,
                                                              wxDefaultSize,
                                                              wxBU_AUTODRAW,
                                                              wxDefaultValidator,
                                                              tmpName );
  bitmapButtonOperation->Show( showOperation );
  if( showOperation )
  {
    int *tmpPos = new int( pos );
    bitmapButtonOperation->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
                                    // wxCommandEventHandler( managedWidgets.back().second ),
                                    wxCommandEventHandler( DerivedHistogramDialog::OnButtonOperationSelectionClick ),
                                    (wxObject *)tmpPos,
                                    this );
  }
  lineBoxSizer->Add(bitmapButtonOperation, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRESERVE_SPACE_EVEN_IF_HIDDEN, 5);


  // Insert spacer
  lineBoxSizer->Add(0, 0, 1, wxALIGN_CENTER_VERTICAL, 5);


  // Create textctrl for parent name
  std::string name = std::visit( [](const auto& w)
                                 {
                                   return w->getName();
                                 }, whichParent );
  wxTextCtrl* textCtrlHistogramName = new wxTextCtrl( panelHistogramLine, wxID_ANY, name, wxDefaultPosition, wxSize(150, -1), 0 );
  lineBoxSizer->Add(textCtrlHistogramName, 6, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxFIXED_MINSIZE, 5);


  // Create button for parent selection
  managedWidgets.emplace_back( std::make_pair( BuildName( whichParent ) + wxT( "_select_" ) + KButtonSuffix,
                                               pos)); //std::bind( &DerivedHistogramDialog::OnButtonParentSelectionClick, this, std::placeholders::_1 ) ) );
  wxBitmapButton* bitmapButtonHistogramSelection = new wxBitmapButton( panelHistogramLine,
                                                                       wxID_ANY,
                                                                       GetBitmapResource(wxT("icons/three_dots.xpm")),
                                                                       wxDefaultPosition,
                                                                       wxDefaultSize,
                                                                       wxBU_AUTODRAW,
                                                                       wxDefaultValidator,
                                                                       managedWidgets.back().first );
  TWindowTypeWrapper *tmpParent = new TWindowTypeWrapper();
  tmpParent->window = whichParent;
  int *tmpPos12 = new int( pos );
  bitmapButtonHistogramSelection->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
                                          //  wxCommandEventHandler( managedWidgets.back().second ),
                                           wxCommandEventHandler( DerivedHistogramDialog::OnButtonParentSelectionClick ),
                                           //(wxObject *)tmpParent,
                                           (wxObject *)tmpPos12,
                                           this );
  lineBoxSizer->Add(bitmapButtonHistogramSelection, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  // Create button for delete
  managedWidgets.emplace_back( std::make_pair( BuildName( whichParent ) + wxT( "_delete_" ) + KButtonSuffix,
                                               pos )); //std::bind( &DerivedHistogramDialog::OnButtonDeleteParentClick, this, std::placeholders::_1  ) ) );
  wxBitmapButton* deleteLine = new wxBitmapButton( panelHistogramLine,
                                                   wxID_ANY,
                                                   GetBitmapResource(wxT("icons/delete.xpm")),
                                                   wxDefaultPosition,
                                                   wxDefaultSize,
                                                   wxBU_AUTODRAW,
                                                   wxDefaultValidator,
                                                   managedWidgets.back().first );
  lineBoxSizer->Add( deleteLine, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5 );
  int *tmpPos2 = new int( pos );
  deleteLine->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
                       wxCommandEventHandler( DerivedHistogramDialog::OnButtonDeleteParentClick ),
                      //  wxCommandEventHandler(  managedWidgets.back().second ),
                       (wxObject *)tmpPos2, // nullptr, //whichParent, // maybe unused here
                       this );

  panelHistogramLine->FitInside();
}


void DerivedHistogramDialog::CreateControlsForParentsList()
{
  if( parentWindows.empty() )
    return;

  wxBoxSizer *boxSizerHistogramsList = new wxBoxSizer( wxVERTICAL );

  if( std::holds_alternative< Histogram * >( parentWindows.front() ) )
  {
    int i = 0;
    for( auto pw: parentWindows )
    {
      // Histogram *tmpParent = std::get< Histogram * >( pw );
      wxBitmap derivedOperationIcon = imagesList->GetBitmap( paraverMain::iconPosByName[ "add" ] ); 
      CreateControlsForParentRow( boxSizerHistogramsList, pw, pw != parentWindows.front(), derivedOperationIcon, i );
      ++i;
      // TODO: remove this line
      //CreateControlsForParentRow( boxSizerHistogramsList, pw, true, derivedOperationIcon );
    }
  }

  histogramsScrolledWindow->SetSizer( boxSizerHistogramsList );
  histogramsScrolledWindow->FitInside();
}


void DerivedHistogramDialog::TransferDataFromPanel()
{

}


void DerivedHistogramDialog::DisconnectWidgets()
{
  // for( auto& parent: parentWindows )
  for( auto w: managedWidgets )
  {
    std::cout << "name of widget to DISCONNECT -> " << w.first << std::endl;

    // wxWindow *tmpWidget = histogramsScrolledWindow->FindWindowByName( w.first );
    // if( tmpWidget != nullptr )
    // {
    //   wxBitmapButton *tmpButton = static_cast< wxBitmapButton * >( tmpWidget );

    //   // TODO: dangerous: the histogram can be named _operation_trac.prv_operation....
    //   if( w.first.find( wxT( "_operation_" ) ) != wxString::npos )
    //   {
    //     std::cout << "DISCONNECT -> " << w.first << std::endl;
    //     tmpButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
    //                            wxCommandEventHandler( DerivedHistogramDialog::OnButtonOperationSelectionClick ),
    //                            nullptr,
    //                            this );
    //   }
    //   else if( w.first.find( wxT( "_select_" ) ) != wxString::npos )
    //   {
    //     std::cout << "DISCONNECT -> " << w.first << std::endl;
    //     tmpButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
    //                            wxCommandEventHandler( DerivedHistogramDialog::OnButtonParentSelectionClick ),
    //                            nullptr,
    //                            this );
    //   }
    //   else if( w.first.find( wxT( "_delete_" ) ) != wxString::npos )
    //   {    
    //     std::cout << "DISCONNECT -> " << w.first << std::endl;
    //     tmpButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
    //                            wxCommandEventHandler(DerivedHistogramDialog::OnButtonDeleteParentClick  ),
    //                            nullptr,
    //                            this );
    //  }
    //}
  }
}

void DerivedHistogramDialog::CleanParentsList()
{
  TransferDataFromPanel();
  DisconnectWidgets();
  managedWidgets.clear();
  histogramsScrolledWindow->DestroyChildren();
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

  // Added extra bitmaps here due to final return wxNullBitmap
  wxUnusedVar( name );
  if ( name == wxT( "icons/delete.xpm" ) )
  {
    wxBitmap bitmap( delete_xpm );
    return bitmap;
  }
  else if (name == wxT("icons/three_dots.xpm"))
  {
    wxBitmap bitmap(three_dots_xpm);
    return bitmap;
  }

  // wxUnusedVar(name);
  // else if (name == wxT("icons/add.xpm"))
  // {
  //   wxBitmap bitmap(add);
  //   return bitmap;
  // }
  // return wxNullBitmap;

////@begin DerivedHistogramDialog bitmap retrieval
  wxUnusedVar(name);
  if (name == wxT("icons/add.xpm"))
  {
    wxBitmap bitmap(add);
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


void DerivedHistogramDialog::OnButtonAddParentClick( wxCommandEvent& event )
{
  if( std::holds_alternative< Histogram * >( parentWindows.front() ) )
  {
    std::vector< Histogram * > derivableHistograms;
    
    paraverMain::myParaverMain->findCompatibleHistogramsForDerivation( std::get< Histogram * >( parentWindows.front() ), derivableHistograms );
    
    // TODO: add menu selection
    for( auto& h: derivableHistograms )
    {
    
      std::cout << "add parent: "  << h->getName() << std::endl;
    }  

    Histogram *parentToAdd = nullptr;
    if( !derivableHistograms.empty() )
      parentToAdd = derivableHistograms.front();

    if( parentToAdd )
    {
      // Add selected to parents
      parentWindows.emplace_back( parentToAdd );

      // Rebuild list
      CleanParentsList();
      CreateControlsForParentsList();
    }
  }  
}  


void DerivedHistogramDialog::OnButtonOperationSelectionClick( wxCommandEvent& event )
{
  wxBitmapButton *tmpDeleteButton = static_cast< wxBitmapButton * >( event.GetEventObject() );
  // TODO: check if first line
  std::cout << "Button operation selection #" <<  *(int *)event.GetEventUserData() << std::endl;
  tmpDeleteButton->Enable( *(int *)event.GetEventUserData() != 0 );
}


void DerivedHistogramDialog::OnButtonOperationSelectionUpdate( wxUpdateUIEvent& event )
{
}  


void DerivedHistogramDialog::OnButtonParentSelectionClick( wxCommandEvent& event )
{
  // wxObject *tmpRawData = event.GetEventUserData();
  // TWindowTypeWrapper *tmpWrapper = static_cast< TWindowTypeWrapper * >( tmpRawData );

  // std::string name = std::visit( [](const auto& w)
  //                                {
  //                                  return w->getName();
  //                                }, tmpWrapper->window );
  // std::cout << "Test callback for window: " << name << std::endl;

  wxBitmapButton *tmpSelectionButton = static_cast< wxBitmapButton * >( event.GetEventObject() );
  // TODO: check if first line
  std::cout << "Button selection #" <<  *(int *)event.GetEventUserData() << std::endl;
}

void DerivedHistogramDialog::OnButtonParentSelectionUpdate( wxUpdateUIEvent& event )
{
}


void DerivedHistogramDialog::OnButtonDeleteParentClick( wxCommandEvent& event )
{
  wxBitmapButton *tmpDeleteButton = static_cast< wxBitmapButton * >( event.GetEventObject() );
  // TODO: check if first line
  std::cout << "Button delete #" <<  *(int *)event.GetEventUserData() << std::endl;
}


void DerivedHistogramDialog::OnButtonDeleteParentUpdate( wxUpdateUIEvent& event )
{
  wxBitmapButton *tmpDeleteButton = static_cast< wxBitmapButton * >( event.GetEventObject() );
  // TODO: check if first line
  // std::cout << "Button delete update #" <<  *(int *)event.GetEventUserData() << std::endl;
  tmpDeleteButton->Enable( *(int *)event.GetEventUserData() != 0 );
}


