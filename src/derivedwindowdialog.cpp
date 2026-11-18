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

#include <wx/gdicmn.h>
#include <wx/numformatter.h>

////@begin includes
////@end includes

#include "derivedwindowdialog.h"
#include "windowstreeselector.h"

////@begin XPM images
#include "../icons/add.xpm"
////@end XPM images

#include "../icons/three_dots.xpm"
#include "../icons/delete.xpm"


wxDEFINE_EVENT(EVT_POPUP_TRANSIENT_CLOSED, wxCommandEvent);

wxIMPLEMENT_CLASS(ButtonArrayPopup,wxPopupTransientWindow);

wxBEGIN_EVENT_TABLE(ButtonArrayPopup,wxPopupTransientWindow)
    EVT_MOUSE_EVENTS( ButtonArrayPopup::OnMouse )
    EVT_SIZE( ButtonArrayPopup::OnSize )
    EVT_SET_FOCUS( ButtonArrayPopup::OnSetFocus )
    EVT_KILL_FOCUS( ButtonArrayPopup::OnKillFocus )
    EVT_KILL_FOCUS( ButtonArrayPopup::OnKillFocus )
    // EVT_KEY_DOWN( ButtonArrayPopup::OnKeyDown )
    EVT_BUTTON( ID_BITMAPBUTTON_OPERATION, ButtonArrayPopup::OnButton )
wxEND_EVENT_TABLE()

#ifdef _WIN32
ButtonArrayPopup::ButtonArrayPopup( wxWindow *parent, wxBitmapButton *whichCallerButton, wxImageList *icons, const wxString& whichSelectedButton )
                     :wxPopupTransientWindow( parent, wxBORDER_NONE | wxWANTS_CHARS | wxPU_CONTAINS_CONTROLS )
#else
ButtonArrayPopup::ButtonArrayPopup( wxWindow *parent, wxBitmapButton *whichCallerButton, wxImageList *icons, const wxString& whichSelectedButton )
                     :wxPopupTransientWindow( parent, wxBORDER_NONE | wxWANTS_CHARS )
#endif
{
  operationName = whichSelectedButton;
  callerButton = whichCallerButton;

  mainPanel = new wxScrolledWindow( this, wxID_ANY );

  auto buildRowSizer = [this, &icons]( const std::vector< std::string >& operations )
  {
    // Returns a sizer with one button for every operation passed.
    wxBoxSizer *rowSizer = new wxBoxSizer( wxHORIZONTAL );
    for( auto op: operations )
    {
      auto pos = paraverMain::iconPosByName[ op ] - paraverMain::iconPosByName[ "add" ];
      wxButton *currentButton = new wxBitmapButton( mainPanel,
                                                    ID_BITMAPBUTTON_OPERATION,
                                                    icons->GetBitmap( pos ),
                                                    wxDefaultPosition,
                                                    wxDefaultSize,
                                                    wxBU_AUTODRAW | wxWANTS_CHARS,
                                                    wxDefaultValidator,
                                                    wxString( op ) );
      rowSizer->Add( currentButton, 0, wxALL, 3 );
    }

    return rowSizer;
  };


  std::vector< std::vector< std::string > > rowOperations = {
                                                              { "add", "substract", "product", "divide" },
                                                              { "maximum", "minimum", "different" }
                                                            };
  
  wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
  for( auto row: rowOperations )
    topSizer->Add( buildRowSizer( row ), 0, wxALL, 3 );

  mainPanel->SetSizer( topSizer );
  topSizer->Fit( mainPanel );
  SetClientSize( mainPanel->GetSize() );
}

ButtonArrayPopup::~ButtonArrayPopup()
{
  ( ( DerivedHistogramDialog * )GetParent() )->SetDestroyButtonArray( true );
}

void ButtonArrayPopup::Popup( wxWindow* WXUNUSED( focus ) )
{
  wxPopupTransientWindow::Popup();
}

void ButtonArrayPopup::OnDismiss()
{
  ( ( DerivedHistogramDialog * )GetParent() )->SetDestroyButtonArray( true );
  wxPopupTransientWindow::OnDismiss();
}

bool ButtonArrayPopup::ProcessLeftDown( wxMouseEvent& event )
{
  return wxPopupTransientWindow::ProcessLeftDown( event );
}

bool ButtonArrayPopup::Show( bool show )
{
  return wxPopupTransientWindow::Show( show );
}

void ButtonArrayPopup::OnSize( wxSizeEvent &event )
{
  event.Skip();
}

void ButtonArrayPopup::OnSetFocus( wxFocusEvent &event )
{
  event.Skip();
}

void ButtonArrayPopup::OnKillFocus( wxFocusEvent &event )
{
  event.Skip();
}

void ButtonArrayPopup::OnMouse( wxMouseEvent &event )
{
  event.Skip();
}

void ButtonArrayPopup::OnButton( wxCommandEvent& event )
{
  wxBitmapButton *button = static_cast< wxBitmapButton * >( event.GetEventObject() );

  operationName = button->GetName();
  static_cast< DerivedHistogramDialog * >( GetParent() )->SetPopupOperation( operationName.ToStdString() );

  wxCommandEvent sendOperation( EVT_POPUP_TRANSIENT_CLOSED );
  wxPostEvent( GetParent(), sendOperation );

  Show( false );
  OnDismiss();
}


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
  EVT_BUTTON( wxID_OK, DerivedHistogramDialog::OnOkClick )
  EVT_UPDATE_UI( wxID_OK, DerivedHistogramDialog::OnOkUpdate )
////@end DerivedHistogramDialog event table entries

  EVT_COMMAND( wxID_ANY, EVT_POPUP_TRANSIENT_CLOSED, DerivedHistogramDialog::OnPopupOperationModified )
END_EVENT_TABLE()


const wxString DerivedHistogramDialog::KOperation  = _( "_OP_" );
const wxString DerivedHistogramDialog::KSelect     = _( "_SEL_" );
const wxString DerivedHistogramDialog::KDelete     = _( "_DEL_" );
const wxString DerivedHistogramDialog::KTextCtrl   = _( "_TXTCTRL_" );
const wxString DerivedHistogramDialog::KCheckBox   = _( "_CHECKBOX_" );
const wxString DerivedHistogramDialog::KButton     = _( "_BUTTON_" );
const wxString DerivedHistogramDialog::KNotCreated = _( "_NOT_CREATED_" );

ButtonArrayPopup *DerivedHistogramDialog::currentButtonArrayPopup = nullptr;


/*!
 * DerivedHistogramDialog constructors
 */

DerivedHistogramDialog::DerivedHistogramDialog()
{
  Init();
}

DerivedHistogramDialog::DerivedHistogramDialog( wxWindow* parent,
                                                const wxString& whichName,
                                                const std::vector< TWindowType >& whichParents,
                                                const std::vector< std::string >& whichOperations,
                                                wxWindowID id,
                                                const wxString& caption,
                                                const wxPoint& pos,
                                                const wxSize& size,
                                                long style )
{
  Init();

  SetDerivedWindowName( whichName );
  SetParentWindows( whichParents );
  SetOperations( whichOperations );

  InitParentLines();

  Create(parent, id, caption, pos, size, style);
  SetMinSize( wxSize( 400, 300 ) );
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
  if ( currentButtonArrayPopup != nullptr && !destroyButtonArray )
    currentButtonArrayPopup->OnDismiss();

  DisconnectWidgets();
  delete operationsImagesList;
}


/*!
 * Member initialisation
 */

void DerivedHistogramDialog::Init()
{
////@begin DerivedHistogramDialog member initialisation
  derivedWindowName = wxT("");
  operations = {};
  parentWindows = {};
  numInstance = {};
  derivedHistogramName = NULL;
  histogramsScrolledWindow = NULL;
  buttonAddParentWindow = NULL;
////@end DerivedHistogramDialog member initialisation

  imagesList = paraverMain::myParaverMain->GetImageList();

  operationsImagesList = new wxImageList( 16, 16 );
  for( auto i = paraverMain::iconPosByName[ "add" ]; i <= paraverMain::iconPosByName[ "different" ]; ++i )
  {  
    operationsImagesList->Add( imagesList->GetBitmap( i ) );
  }
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

  // Charset filter for derivedHistogramName
  wxString forbiddenCharsList[] = { _( "|" ), _( "/" ), _( "\\" ) };
  wxArrayString forbiddenChars( (size_t)3, forbiddenCharsList );
  wxTextValidator excludeVerticalBar( wxFILTER_EXCLUDE_CHAR_LIST );
  excludeVerticalBar.SetExcludes( forbiddenChars );
  derivedHistogramName->SetValidator( excludeVerticalBar );

  if( !derivedWindowName.IsEmpty() )
    derivedHistogramName->SetValue( derivedWindowName );

  CreateControlsForParentsList();
}


void DerivedHistogramDialog::deleteParentLine( int whichPos  )
{
  parentLines.erase( parentLines.begin() + whichPos );
}


bool DerivedHistogramDialog::changeParentLine( const wxString& buttonName, const std::string& newOperation )
{
  bool changed = false;

  auto currentLine = parentLines.begin();
  for( currentLine = parentLines.begin(); currentLine != parentLines.end(); ++currentLine )
    {
    if( std::find( currentLine->button.cbegin(), currentLine->button.cend(), buttonName ) != currentLine->button.cend() )
      break;
  }

  if( currentLine->operationName != newOperation )
  {
    currentLine->operationName = newOperation;
    changed = true;
  }

  return changed;
}


std::string DerivedHistogramDialog::GetWindowName( const TWindowType& whichParent )
{
  return std::visit( [](const auto& current)
                     {
                       using T = std::decay_t< decltype( current ) >;

                       if constexpr ( std::is_same_v< T, std::monostate > )
                         return std::string( "" );
                       else 
                         return current->getName();
                      }, whichParent );
}


wxString DerivedHistogramDialog::BuildParentName( const TWindowType& whichParent )
{
  return std::visit( [](const auto& current)
                     {
                       using T = std::decay_t< decltype( current ) >;

                       if constexpr ( std::is_same_v< T, std::monostate > )
                         return KNotCreated;
                       else 
                         return wxString::FromUTF8( current->getName().c_str() ) + _( "_" ) +
                                wxString::FromUTF8( current->getTrace()->getTraceNameNumbered().c_str() );
                     }, whichParent );
}


void DerivedHistogramDialog::CreateControlsForParentLine( wxBoxSizer *whichBoxSizer,
                                                          const TWindowType& whichParent,
                                                          int pos,
                                                          const wxBitmap& derivedOperationIcon )
{
  bool isNotFirstLine = ( pos != 0 );
  wxString tmpParentFullName {};
  wxString tmpParentName = std::holds_alternative< std::monostate >( whichParent ) ?
                           KNotCreated :
                           BuildParentName( whichParent );

  // Create panel
  wxPanel* panelHistogramLine = new wxPanel( histogramsScrolledWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
  panelHistogramLine->SetExtraStyle( wxWS_EX_VALIDATE_RECURSIVELY );
  whichBoxSizer->Add( panelHistogramLine, 0, wxGROW|wxALL, 5 );

  // Create boxsizer
  wxBoxSizer* lineBoxSizer = new wxBoxSizer( wxHORIZONTAL );
  panelHistogramLine->SetSizer( lineBoxSizer );

  // Create operation button
  if( isNotFirstLine )
  {
    tmpParentFullName = tmpParentName + KOperation + KButton + wxString::Format( wxT( "%d" ), pos );

    parentLines[ pos ].button.push_back( tmpParentFullName );
  }
  wxBitmapButton* bitmapButtonOperation = new wxBitmapButton( panelHistogramLine,
                                                              wxID_ANY,
                                                              derivedOperationIcon,
                                                              wxDefaultPosition,
                                                              wxDefaultSize,
                                                              wxBU_AUTODRAW,
                                                              wxDefaultValidator,
                                                              tmpParentFullName );
  bitmapButtonOperation->Show( isNotFirstLine );
  if( isNotFirstLine )
  {
    TIntWrapper *tmpPos = new TIntWrapper();
    tmpPos->pos = pos;
    bitmapButtonOperation->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
                                    wxCommandEventHandler( DerivedHistogramDialog::OnButtonOperationSelectionClick ),
                                    tmpPos,
                                    this );
  }
  lineBoxSizer->Add(bitmapButtonOperation, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRESERVE_SPACE_EVEN_IF_HIDDEN, 5);

  // Insert spacer
  lineBoxSizer->Add(0, 0, 1, wxALIGN_CENTER_VERTICAL, 5);

  // Create textctrl for parent name
  std::string name = GetWindowName( whichParent );
  wxTextCtrl* textCtrlHistogramName = new wxTextCtrl( panelHistogramLine, wxID_ANY, name, wxDefaultPosition, wxSize( 150, -1 ), 0 );
  textCtrlHistogramName->SetEditable( false );

  if( std::holds_alternative< Histogram * >( parentLines[ pos ].parent ) )
  {
    // Inside another derived histogram?
    Histogram *tmpHistogram = std::get< Histogram * >( parentLines[ pos ].parent );
    if( tmpHistogram->haveChildren() )
    {
      textCtrlHistogramName->SetForegroundColour( wxTheColourDatabase->Find( wxT( "PURPLE" ) ) );
      panelHistogramLine->SetToolTip( "Histogram is a parent of a derived histogram. A clone will be done." );
    }
     
    if( parentLines[ pos ].numInstance > 1 )
    {
      textCtrlHistogramName->SetForegroundColour( wxTheColourDatabase->Find( wxT( "PURPLE" ) ) );
      panelHistogramLine->SetToolTip( "Histogram selected multiple times. A clone will be done." );
    }
  }


  // Compatible?
  if( isNotFirstLine )
  {
    std::string compatibilityMessage;
    if( !isCompatibleWithMain( parentLines[ pos ].parent, compatibilityMessage ) )
    {
      textCtrlHistogramName->SetForegroundColour( *wxRED );
      panelHistogramLine->SetToolTip( "Histogram not compatible with main (first one):\n\t" + compatibilityMessage );
      bitmapButtonOperation->Enable( false );
    }
  }

  lineBoxSizer->Add( textCtrlHistogramName, 6, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT | wxFIXED_MINSIZE, 5 );


  // Create button for parent selection
  tmpParentFullName = tmpParentName + KSelect + KButton + wxString::Format( wxT( "%d" ), pos );
  parentLines[ pos ].button.push_back( tmpParentFullName );
  wxBitmapButton* bitmapButtonHistogramSelection = new wxBitmapButton( panelHistogramLine,
                                                                       wxID_ANY,
                                                                       GetBitmapResource( wxT( "icons/three_dots.xpm" ) ),
                                                                       wxDefaultPosition,
                                                                       wxDefaultSize,
                                                                       wxBU_AUTODRAW,
                                                                       wxDefaultValidator,
                                                                       tmpParentFullName );
  TIntWrapper *tmpPos2 = new TIntWrapper();
  tmpPos2->pos = pos;
  bitmapButtonHistogramSelection->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
                                           wxCommandEventHandler( DerivedHistogramDialog::OnButtonParentSelectionClick ),
                                           tmpPos2,
                                           this );
  lineBoxSizer->Add(bitmapButtonHistogramSelection, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

  // Create button for delete
  tmpParentFullName = tmpParentName + KDelete + KButton + wxString::Format( wxT( "%d" ), pos );
  parentLines[ pos ].button.push_back( tmpParentFullName );
  wxBitmapButton* deleteLine = new wxBitmapButton( panelHistogramLine,
                                                   wxID_ANY,
                                                   GetBitmapResource( wxT( "icons/delete.xpm" ) ),
                                                   wxDefaultPosition,
                                                   wxDefaultSize,
                                                   wxBU_AUTODRAW,
                                                   wxDefaultValidator,
                                                   tmpParentFullName );
  lineBoxSizer->Add( deleteLine, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5 );

  TIntWrapper *tmpPos3 = new TIntWrapper();
  tmpPos3->pos = pos;
  deleteLine->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
                       wxCommandEventHandler( DerivedHistogramDialog::OnButtonDeleteParentClick ),
                       tmpPos3,
                       this );
}


void DerivedHistogramDialog::CreateControlsForParentsList()
{
  if( parentWindows.empty() )
    return;

  wxBoxSizer *boxSizerHistogramsList = new wxBoxSizer( wxVERTICAL );

  // Repeated histograms? Numerate as they appear. Unique ones equal to 1.
  std::transform( parentLines.begin(), parentLines.end(), parentLines.begin(), []( auto& line )
                                                                               {
                                                                                 line.numInstance = 1;
                                                                                 return line;
                                                                               } );
  if( parentLines.size() > 1 )
  {
    for( auto currentPos = parentLines.begin(); currentPos < parentLines.end(); ++currentPos )
    {
      if( currentPos->numInstance == 1 )
      {
        Histogram *currentHistogram = std::get< Histogram * >( currentPos->parent );
        auto numCopiesFound = 0;

        for( auto nextPos = currentPos + 1; nextPos < parentLines.end(); ++nextPos )
        {
          Histogram *nextHistogram = std::get< Histogram * >( nextPos->parent );

          if( currentHistogram == nextHistogram )
            nextPos->numInstance += ++numCopiesFound;
        }
      }
    }
  }

  int pos = 0;
  for( auto line: parentLines )
  {
    wxBitmap derivedOperationIcon = imagesList->GetBitmap( paraverMain::iconPosByName[ line.operationName ] );

    CreateControlsForParentLine( boxSizerHistogramsList, line.parent, pos, derivedOperationIcon );
    ++pos;
  }

  histogramsScrolledWindow->SetSizer( boxSizerHistogramsList );
  histogramsScrolledWindow->FitInside();
}


// Copy back parentLines info to parentWindows, operations
bool DerivedHistogramDialog::TransferWindowToData()
{
  parentWindows.clear();
  operations.clear();
  numInstance.clear();

  for( auto line: parentLines )
  {
    parentWindows.push_back( line.parent );
    numInstance.push_back( line.numInstance );

    if( parentWindows.size() > 1 )
      operations.push_back( line.operationName );
  }

  derivedWindowName = derivedHistogramName->GetValue().ToStdString();

  return parentWindows.size() > 1;
}


// Initalizes parentLines using parentWindows and operations info
bool DerivedHistogramDialog::InitParentLines()
{
  parentLines.clear();
  numInstance.clear();

  bool uniqueOperation = ( operations.size() == 1 );
  bool enoughNumberOfOperations = ( operations.size() >= ( parentWindows.size() - 1 ) );
  int count = -1;
  for( auto pw: parentWindows )
  {
    if( uniqueOperation )
    {
      parentLines.emplace_back( pw, operations.front(), std::vector< TButtonName >(), 1 );
    }
    else if( enoughNumberOfOperations )
    {
      parentLines.emplace_back( pw,
                                ( count < 0 ) ? "add" : operations[ ++count ],
                                std::vector< TButtonName >(),
                                1 );
    }
    else
    {
      // malformed operations vector! => repeat last operation
      parentLines.emplace_back( pw,
                                ( count >= ( int( parentWindows.size() ) - 1 ) ) ? operations.back() : operations[ ++count ],
                                std::vector< TButtonName >(),
                                1 );
    }
  }

  return true;
}


void DerivedHistogramDialog::DisconnectWidgets()
{
  // Buttons 
  for( auto currentLine: parentLines )
  {
    for( auto buttonName: currentLine.button )
    {
      wxWindow *tmpWidget = histogramsScrolledWindow->FindWindowByName( buttonName );
      if( tmpWidget != nullptr )
      {
        wxBitmapButton *tmpButton = static_cast< wxBitmapButton * >( tmpWidget );

        if( buttonName.find( KOperation ) != wxString::npos )
        {
          tmpButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                                 wxCommandEventHandler( DerivedHistogramDialog::OnButtonOperationSelectionClick ),
                                 nullptr,
                                 this );
        }
        else if( buttonName.find( KSelect ) != wxString::npos )
        {
          tmpButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                                 wxCommandEventHandler( DerivedHistogramDialog::OnButtonParentSelectionClick ),
                                 nullptr,
                                 this );
        }
        else if( buttonName.find( KDelete ) != wxString::npos )
        {    
          tmpButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                                 wxCommandEventHandler( DerivedHistogramDialog::OnButtonDeleteParentClick ),
                                 nullptr,
                                 this );
        }
      }
    }
  }
}


void DerivedHistogramDialog::CleanParentsList()
{
  TransferWindowToData();
  DisconnectWidgets();
  for( auto currentLine: parentLines )
  {
    currentLine.button.clear();
  }
  histogramsScrolledWindow->DestroyChildren();
}


void DerivedHistogramDialog::RebuildParentsList()
{
  // TODO: simplify if possible
  Freeze();
  CleanParentsList();
  CreateControlsForParentsList();
  Thaw();
  histogramsScrolledWindow->GetSizer()->Layout();
  histogramsScrolledWindow->FitInside();
  histogramsScrolledWindow->Refresh();
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
  else if (name == wxT( "icons/three_dots.xpm" ) )
  {
    wxBitmap bitmap(three_dots_xpm);
    return bitmap;
  }

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


Histogram *DerivedHistogramDialog::selectHistogram( Histogram *mainHistogram )
{
  // Find compatible histograms
  std::vector< Histogram * > derivableHistograms;
  bool searchAllTraces = true;
  std::map< Histogram *, std::string > dummyMessages;
  paraverMain::myParaverMain->findCompatibleHistogramsForDerivation( mainHistogram,
                                                                     derivableHistograms,
                                                                     dummyMessages,
                                                                     searchAllTraces );

  // Get IDs
  std::vector< TWindowID > derivableHistogramsIds;
  for( auto& h: derivableHistograms )
    derivableHistogramsIds.emplace_back( LoadedWindows::getInstance()->getHistogramID( h ) );

  // Show WindowTreeSelector
  WindowsTreeSelector histogramSelector( this, wxID_ANY, wxT( "Compatible histograms" ), derivableHistogramsIds, TWindowType( mainHistogram ), mainHistogram->getTrace() );
  histogramSelector.Move( wxGetMousePosition() );

  if( histogramSelector.ShowModal() == wxID_OK )
    return std::get< Histogram * >( histogramSelector.getSelection() );

  return nullptr;
}


void DerivedHistogramDialog::OnButtonAddParentClick( wxCommandEvent& event )
{
  if( std::holds_alternative< Histogram * >( parentWindows.front() ) )
  {
    Histogram *selectedHistogram = selectHistogram( std::get< Histogram * >( parentWindows.front() ) );
    if( !selectedHistogram )
      return;

    parentLines.emplace_back( selectedHistogram, "add", std::vector< TButtonName >(), 1 );

    RebuildParentsList();
  }  
}  


void DerivedHistogramDialog::OnButtonOperationSelectionClick( wxCommandEvent& event )
{
  // if( !destroyButtonArray )
  //   delete currentButtonArrayPopup; // paranoid: shouldn't happen that still exists

  // Save button for further OnPopupOperationModified callback
  operationButtonCaller = static_cast< wxBitmapButton * >( event.GetEventObject() );
  SetDestroyButtonArray( false );
  currentButtonArrayPopup = new ButtonArrayPopup( this, operationButtonCaller, operationsImagesList, "add" );

  // Get position
  wxWindow *btn = ( wxWindow* )event.GetEventObject();
  wxPoint pos = btn->ClientToScreen( wxPoint( 0, 0 ) );
  wxSize sz = btn->GetSize();
  currentButtonArrayPopup->Position( pos, wxSize( sz.GetWidth(), 0 ) );

  currentButtonArrayPopup->Popup();
}


void DerivedHistogramDialog::OnButtonOperationSelectionUpdate( wxUpdateUIEvent& event )
{  
} 


void DerivedHistogramDialog::OnPopupOperationModified( wxCommandEvent& event )
{
  wxString buttonName = operationButtonCaller->GetName();
  bool changed = changeParentLine( buttonName, selectedPopupOperation );
  if( changed )
  {
    operationButtonCaller->SetBitmap( imagesList->GetBitmap(  paraverMain::iconPosByName[ selectedPopupOperation ] ) );
  }
}


bool DerivedHistogramDialog::isCompatibleWithMain( TWindowType whichWindow, std::string& compatibilityMessage )
{
  if( std::holds_alternative< Histogram * >( whichWindow ) )
  {
    Histogram *mainHistogram = std::get< Histogram * >( parentLines.front().parent );
    Histogram *currentHistogram = std::get< Histogram * >( whichWindow );
    std::vector< Histogram * > derivableHistograms;
    bool searchAllTraces = true;
    std::map< Histogram *, std::string > allCompatibilityMessages;
    paraverMain::myParaverMain->findCompatibleHistogramsForDerivation( mainHistogram, derivableHistograms, allCompatibilityMessages, searchAllTraces );

    bool isCompatible = std::find( derivableHistograms.cbegin(), derivableHistograms.cend(), currentHistogram ) != derivableHistograms.cend();
    if( !isCompatible )
      compatibilityMessage = allCompatibilityMessages[ currentHistogram ];

    return isCompatible;
  }

  return false;
}


void DerivedHistogramDialog::OnButtonParentSelectionClick( wxCommandEvent& event )
{
  if( std::holds_alternative< Histogram * >(  parentLines.front().parent ) )
  {
    Histogram *selectedHistogram = selectHistogram( std::get< Histogram * >(  parentLines.front().parent ) );
    if( !selectedHistogram )
      return;

    auto pos = ( (TIntWrapper *)event.GetEventUserData() )->pos;
    parentLines[ pos ].parent = TWindowType( selectedHistogram );
    
    CreateControlsForParentsList();
  }
}


void DerivedHistogramDialog::OnButtonParentSelectionUpdate( wxUpdateUIEvent& event )
{
}


void DerivedHistogramDialog::OnButtonDeleteParentClick( wxCommandEvent& event )
{
  deleteParentLine( ( (TIntWrapper *)event.GetEventUserData() )->pos );
  RebuildParentsList();
}


void DerivedHistogramDialog::OnButtonDeleteParentUpdate( wxUpdateUIEvent& event )
{
  wxBitmapButton *tmpDeleteButton = static_cast< wxBitmapButton * >( event.GetEventObject() );
  tmpDeleteButton->Enable( ( ( TIntWrapper * )event.GetEventUserData() )->pos > 0 );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void DerivedHistogramDialog::OnOkClick( wxCommandEvent& event )
{
  if( TransferWindowToData() )
  {
    EndModal( wxID_OK );
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for wxID_OK
 */

void DerivedHistogramDialog::OnOkUpdate( wxUpdateUIEvent& event )
{
  auto allHistogramsAreCompatible = [this]()
                                    {
                                      for( const TLineData& h: parentLines )
                                      {
                                        if( h.parent == parentLines.front().parent )
                                          continue;

                                        std::string compatibilityMessage;
                                        if( !isCompatibleWithMain( h.parent, compatibilityMessage ) )
                                          return false;
                                      }
                                      return true;
                                    };

  bool enable = ( parentLines.size() >= 2 ) &&
                !derivedHistogramName->IsEmpty() && 
                allHistogramsAreCompatible();
  static_cast< wxBitmapButton * >( event.GetEventObject() )->Enable( enable );
}

