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
#  pragma hdrstop
#endif

#ifndef WX_PRECOMP
#  include "wx/wx.h"
#endif

// clang-format off
////@begin includes
////@end includes
// clang-format on

#include <algorithm>
#include <cmath>

#include <wx/activityindicator.h>
#include <wx/datetime.h>
#include <wx/evtloop.h>
#include <wx/gauge.h>
#include <wx/statline.h>

#include "progresslistdialog.h"
#include "paravermain.h"
#include "progresscontroller.h"

// clang-format off
////@begin XPM images
////@end XPM images
// clang-format on


/*!
 * ProgressListDialog type definition
 */
IMPLEMENT_DYNAMIC_CLASS( ProgressListDialog, wxDialog )


/*!
 * ProgressListDialog event table definition
 */
BEGIN_EVENT_TABLE( ProgressListDialog, wxDialog )

// clang-format off
////@begin ProgressListDialog event table entries
  EVT_CLOSE( ProgressListDialog::OnCloseWindow )
  EVT_BUTTON( ID_BUTTON_CANCEL_ALL, ProgressListDialog::OnButtonCancelAllClick )
////@end ProgressListDialog event table entries
// clang-format on

END_EVENT_TABLE()


ProgressListDialog* ProgressListDialog::instance = nullptr;


namespace
{
  wxString formatTime( wxLongLong milliseconds )
  {
    return wxTimeSpan::Milliseconds( milliseconds ).Format( wxT( "%H:%M:%S" ) );
  }

  // Fixed width, so the labels after it do not move while the time changes.
  wxStaticText* createTimeLabel( wxWindow* parent, const wxString& prefix )
  {
    wxStaticText* label = new wxStaticText( parent, wxID_ANY, wxEmptyString );

    int maxWidth = label->GetTextExtent( prefix + _( "unknown" ) ).GetWidth();
    for( wxChar digit = wxT( '0' ); digit <= wxT( '9' ); ++digit )
    {
      wxString time = wxString( digit, 2 ) + wxT( ":" ) + wxString( digit, 2 ) + wxT( ":" ) + wxString( digit, 2 );
      maxWidth      = std::max( maxWidth, label->GetTextExtent( prefix + time ).GetWidth() );
    }

    label->SetMinSize( wxSize( maxWidth, -1 ) );
    return label;
  }
}


/*!
 * ProgressListDialog constructors
 */
ProgressListDialog::ProgressListDialog()
{
  Init();
}

ProgressListDialog::ProgressListDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( parent, id, caption, pos, size, style );
}


/*!
 * ProgressListDialog creator
 */
bool ProgressListDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  // clang-format off
////@begin ProgressListDialog creation
  SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  Centre();
////@end ProgressListDialog creation
  // clang-format on
  return true;
}


/*!
 * ProgressListDialog destructor
 */
ProgressListDialog::~ProgressListDialog()
{
  // clang-format off
////@begin ProgressListDialog destruction
////@end ProgressListDialog destruction
  // clang-format on
  if( instance == this )
    instance = nullptr;
}


/*!
 * Member initialisation
 */
void ProgressListDialog::Init()
{
  // clang-format off
////@begin ProgressListDialog member initialisation
  rowsPanel = NULL;
  rowsSizer = NULL;
  cancelAllButton = NULL;
////@end ProgressListDialog member initialisation
  // clang-format on
  cancelAll    = false;
  fittedHeight = 0;
}


/*!
 * Control creation for ProgressListDialog
 */
void ProgressListDialog::CreateControls()
{
  // clang-format off
////@begin ProgressListDialog content construction
  ProgressListDialog* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  rowsPanel = new wxScrolledWindow( itemDialog1, ID_SCROLLEDWINDOW_PROGRESS_ROWS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxVSCROLL );
  itemBoxSizer2->Add(rowsPanel, 1, wxGROW|wxALL, 5);
  rowsPanel->SetScrollbars(10, 10, 0, 0);
  rowsSizer = new wxBoxSizer(wxVERTICAL);
  rowsPanel->SetSizer(rowsSizer);

  rowsPanel->FitInside();

  wxStaticLine* itemStaticLine6 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemBoxSizer2->Add(itemStaticLine6, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

  cancelAllButton = new wxButton( itemDialog1, ID_BUTTON_CANCEL_ALL, _("Cancel all"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer2->Add(cancelAllButton, 0, wxALIGN_RIGHT|wxALL, 5);

////@end ProgressListDialog content construction
  // clang-format on
}


/*!
 * Should we show tooltips?
 */
bool ProgressListDialog::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */
wxBitmap ProgressListDialog::GetBitmapResource( const wxString& name )
{
  // clang-format off
  // Bitmap retrieval
////@begin ProgressListDialog bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
////@end ProgressListDialog bitmap retrieval
  // clang-format on
}

/*!
 * Get icon resources
 */
wxIcon ProgressListDialog::GetIconResource( const wxString& name )
{
  // clang-format off
  // Icon retrieval
////@begin ProgressListDialog icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end ProgressListDialog icon retrieval
  // clang-format on
}


/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_PROGRESSLISTDIALOG
 */
void ProgressListDialog::OnCloseWindow( wxCloseEvent& event )
{
  if( hasActiveRows() && event.CanVeto() )
  {
    cancelEverything();
    event.Veto();
    return;
  }

  Hide();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CANCEL_ALL
 */
void ProgressListDialog::OnButtonCancelAllClick( wxCommandEvent& event )
{
  cancelEverything();
}


void ProgressListDialog::cancelEverything()
{
  cancelAll = true;
  for( auto& it : rows )
    cancelRow( it.second );
  cancelAllButton->Enable( false );
}


ProgressListDialog* ProgressListDialog::getInstance( wxWindow* parent )
{
  if( instance == nullptr )
    instance = new ProgressListDialog( parent );
  return instance;
}


bool ProgressListDialog::isSomeViewRunning()
{
  if( instance == nullptr )
    return false;

  for( const auto& it : instance->rows )
    if( it.second.state == RowState::RUNNING )
      return true;
  return false;
}


bool ProgressListDialog::hasActiveRows() const
{
  for( const auto& it : rows )
    if( it.second.state == RowState::PENDING || it.second.state == RowState::RUNNING )
      return true;
  return false;
}


// Rows left when nothing is pending or drawing belong to the previous batch.
void ProgressListDialog::startBatchIfNeeded()
{
  if( hasActiveRows() )
    return;

  while( !rows.empty() )
    removeRow( rows.begin()->first );

  cancelAll = false;
  cancelAllButton->Enable( true );
}


void ProgressListDialog::addRow( void* key, const wxString& title )
{
  Row newRow;
  newRow.state           = RowState::PENDING;
  newRow.cancelRequested = false;

  //            view @ trace
  //   [icon]   [gauge........................]   [Cancel]
  //            message
  //            elapsed   estimated   remaining
  const int margin = 5;
  const int indent = margin + ICON_WIDTH + margin;

  newRow.panel           = new wxPanel( rowsPanel, wxID_ANY );
  wxBoxSizer* panelSizer = new wxBoxSizer( wxVERTICAL );
  wxBoxSizer* rowSizer   = new wxBoxSizer( wxHORIZONTAL );
  wxBoxSizer* infoSizer  = new wxBoxSizer( wxVERTICAL );
  newRow.panel->SetSizer( panelSizer );
  panelSizer->Add( rowSizer, 0, wxGROW | wxTOP | wxBOTTOM, margin );
  panelSizer->Add( new wxStaticLine( newRow.panel ), 0, wxGROW | wxLEFT | wxRIGHT, margin );

  newRow.label = new wxStaticText( newRow.panel, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxST_ELLIPSIZE_MIDDLE );
  wxFont labelFont = newRow.label->GetFont();
  labelFont.MakeBold();
  newRow.label->SetFont( labelFont );
  newRow.label->SetToolTip( title );
  infoSizer->Add( newRow.label, 0, wxGROW | wxLEFT, indent );
  infoSizer->AddSpacer( 2 * margin );

  newRow.spinner = new wxActivityIndicator( newRow.panel, wxID_ANY, wxDefaultPosition, wxSize( ICON_WIDTH, ICON_WIDTH ) );
  newRow.icon    = new wxStaticText( newRow.panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( ICON_WIDTH, -1 ), wxALIGN_CENTRE_HORIZONTAL );
  wxFont iconFont = newRow.icon->GetFont();
  iconFont.MakeBold().MakeLarger();
  newRow.icon->SetFont( iconFont );
  newRow.gauge = new wxGauge( newRow.panel, wxID_ANY, MAX_PROGRESS_BAR_VALUE, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL | wxGA_SMOOTH );
  wxBoxSizer* gaugeSizer = new wxBoxSizer( wxHORIZONTAL );
  gaugeSizer->Add( newRow.spinner, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, margin );
  gaugeSizer->Add( newRow.icon, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, margin );
  gaugeSizer->Add( newRow.gauge, 1, wxALIGN_CENTER_VERTICAL );
  infoSizer->Add( gaugeSizer, 0, wxGROW );

  newRow.message   = new wxStaticText( newRow.panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxST_ELLIPSIZE_END );
  newRow.elapsed   = createTimeLabel( newRow.panel, _( "Elapsed: " ) );
  newRow.estimated = createTimeLabel( newRow.panel, _( "Estimated: " ) );
  newRow.remaining = createTimeLabel( newRow.panel, _( "Remaining: " ) );
  wxBoxSizer* timesSizer = new wxBoxSizer( wxHORIZONTAL );
  timesSizer->Add( newRow.elapsed, 0, wxRIGHT, 15 );
  timesSizer->Add( newRow.estimated, 0, wxRIGHT, 15 );
  timesSizer->Add( newRow.remaining, 0 );
  infoSizer->Add( newRow.message, 0, wxGROW | wxLEFT, indent );
  infoSizer->Add( timesSizer, 0, wxGROW | wxLEFT, indent );

  // Keeps its space when hidden, so all gauges have the same length.
  newRow.cancelButton = new wxButton( newRow.panel, wxID_ANY, _( "Cancel" ) );
  newRow.cancelButton->Bind( wxEVT_BUTTON,
                             [ this, key ]( wxCommandEvent& )
                             {
                               auto it = rows.find( key );
                               if( it != rows.end() )
                                 cancelRow( it->second );
                             } );

  rowSizer->Add( infoSizer, 1, wxGROW );
  rowSizer->Add( newRow.cancelButton, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT | wxRESERVE_SPACE_EVEN_IF_HIDDEN, 2 * margin );

  rowsSizer->Add( newRow.panel, 0, wxGROW | wxALL, 2 );

  auto inserted = rows.insert( std::make_pair( key, newRow ) );

  setRowState( inserted.first->second, RowState::PENDING );
  if( cancelAll )
    cancelRow( inserted.first->second );
}


void ProgressListDialog::removeRow( void* key )
{
  auto it = rows.find( key );
  if( it == rows.end() )
    return;

  rowsSizer->Detach( it->second.panel );
  it->second.panel->Destroy();
  rows.erase( it );
}


void ProgressListDialog::cancelRow( Row& whichRow )
{
  if( whichRow.cancelRequested || whichRow.state == RowState::DONE || whichRow.state == RowState::CANCELLED )
    return;

  whichRow.cancelRequested = true;
  whichRow.cancelButton->Enable( false );
  whichRow.message->SetLabel( _( "Cancelling..." ) );
  showCancelledSymbol( whichRow );
  whichRow.panel->Layout();
}


void ProgressListDialog::showSpinner( Row& whichRow )
{
  whichRow.icon->Hide();
  whichRow.spinner->Show();
  whichRow.spinner->Start();
}


void ProgressListDialog::showSymbol( Row& whichRow, const wxString& symbol, const wxColour& colour )
{
  whichRow.spinner->Stop();
  whichRow.spinner->Hide();
  whichRow.icon->SetLabel( symbol );
  whichRow.icon->SetForegroundColour( colour );
  whichRow.icon->Show();
}


void ProgressListDialog::showCancelledSymbol( Row& whichRow )
{
  showSymbol( whichRow, L"\u2716", wxColour( 210, 0, 0 ) );  // cross
}


void ProgressListDialog::setRowState( Row& whichRow, RowState newState )
{
  whichRow.state = newState;

  switch( newState )
  {
    case RowState::PENDING:
      showSymbol( whichRow, L"\u2026", wxSystemSettings::GetColour( wxSYS_COLOUR_GRAYTEXT ) );  // ellipsis
      whichRow.message->SetLabel( _( "Waiting..." ) );
      break;
    case RowState::RUNNING:
      whichRow.watch.Start();
      if( whichRow.cancelRequested )
        showCancelledSymbol( whichRow );
      else
      {
        showSpinner( whichRow );
        whichRow.message->SetLabel( _( "Drawing..." ) );
      }
      break;
    case RowState::DONE:
      showSymbol( whichRow, L"\u2714", wxColour( 0, 170, 0 ) );  // check mark
      whichRow.message->SetLabel( _( "Done" ) );
      whichRow.gauge->SetValue( MAX_PROGRESS_BAR_VALUE );
      break;
    case RowState::CANCELLED:
      showCancelledSymbol( whichRow );
      whichRow.message->SetLabel( _( "Cancelled" ) );
      break;
  }

  whichRow.cancelButton->Show( newState == RowState::PENDING || newState == RowState::RUNNING );

  if( newState == RowState::DONE || newState == RowState::CANCELLED )
  {
    whichRow.elapsed->SetLabel( _( "Elapsed: " ) + formatTime( whichRow.watch.Time() ) );
    whichRow.estimated->SetLabel( wxEmptyString );
    whichRow.remaining->SetLabel( wxEmptyString );
  }

  whichRow.panel->Layout();
}


void ProgressListDialog::updateTimes( Row& whichRow, double fraction )
{
  long elapsed = whichRow.watch.Time();
  whichRow.elapsed->SetLabel( _( "Elapsed: " ) + formatTime( elapsed ) );

  if( fraction > 0.0 )
  {
    wxLongLong estimated( (wxLongLong_t)( elapsed / fraction ) );
    wxLongLong remaining = std::max( estimated - elapsed, wxLongLong( 0 ) );
    whichRow.estimated->SetLabel( _( "Estimated: " ) + formatTime( estimated ) );
    whichRow.remaining->SetLabel( _( "Remaining: " ) + formatTime( remaining ) );
  }
  else
  {
    whichRow.estimated->SetLabel( _( "Estimated: " ) + _( "unknown" ) );
    whichRow.remaining->SetLabel( _( "Remaining: " ) + _( "unknown" ) );
  }
}


void ProgressListDialog::scrollToRow( const Row& whichRow )
{
  int ppuX, ppuY;
  rowsPanel->GetScrollPixelsPerUnit( &ppuX, &ppuY );
  if( ppuY <= 0 )
    return;

  int viewStartX, viewStartY;
  rowsPanel->GetViewStart( &viewStartX, &viewStartY );
  int rowTop    = whichRow.panel->GetPosition().y + viewStartY * ppuY;
  int rowBottom = rowTop + whichRow.panel->GetSize().GetHeight();
  int visibleH  = rowsPanel->GetClientSize().GetHeight();

  if( rowTop < viewStartY * ppuY || rowBottom > viewStartY * ppuY + visibleH )
    rowsPanel->Scroll( -1, rowTop / ppuY );
}


void ProgressListDialog::updateTitle()
{
  size_t finished = 0;
  for( const auto& it : rows )
    if( it.second.state == RowState::DONE || it.second.state == RowState::CANCELLED )
      ++finished;

  size_t current = hasActiveRows() ? finished + 1 : finished;
  SetTitle( wxString::Format( wxT( "%s (%zu/%zu)" ), SYMBOL_PROGRESSLISTDIALOG_TITLE, current, rows.size() ) );
}


void ProgressListDialog::relayout()
{
  updateTitle();
  rowsPanel->FitInside();
  rowsPanel->Layout();
  fitToRows();
  Layout();
}


// Only when the needed height changes (rows added or taller), so a manual resize is kept.
void ProgressListDialog::fitToRows()
{
  if( rows.empty() )
    return;

  int allRowsHeight = rowsSizer->GetMinSize().GetHeight();
  int oneRowHeight  = allRowsHeight / rows.size();
  int visibleHeight = std::min( allRowsHeight, oneRowHeight * MAX_VISIBLE_ROWS );
  if( visibleHeight == fittedHeight )
    return;
  fittedHeight = visibleHeight;

  rowsPanel->SetMinSize( wxSize( -1, visibleHeight + 2 * rowsPanel->GetWindowBorderSize().GetHeight() ) );
  SetSize( wxSize( GetSize().GetWidth(), GetSizer()->ComputeFittingWindowSize( this ).GetHeight() ) );
}


void ProgressListDialog::syncPending( const std::vector< PendingView >& pending )
{
  if( !pending.empty() )
    startBatchIfNeeded();

  std::vector< void* > toRemove;
  for( const auto& it : rows )
  {
    if( it.second.state != RowState::PENDING )
      continue;

    auto isSameView = [ &it ]( const PendingView& view ) { return view.key == it.first; };
    if( std::none_of( pending.begin(), pending.end(), isSameView ) )
      toRemove.push_back( it.first );
  }
  for( auto key : toRemove )
    removeRow( key );

  for( const auto& view : pending )
    if( rows.find( view.key ) == rows.end() )
      addRow( view.key, view.title );

  relayout();
}


void ProgressListDialog::setRunning( void* key, const wxString& title )
{
  startBatchIfNeeded();

  if( rows.find( key ) == rows.end() )
    addRow( key, title );

  Row& row = rows.find( key )->second;
  setRowState( row, RowState::RUNNING );
  updateTimes( row, 0.0 );

  relayout();
  scrollToRow( row );

  if( !IsShown() )
    Show();
  Raise();
}


void ProgressListDialog::finish( void* key, bool wasStopped )
{
  auto it = rows.find( key );
  if( it != rows.end() )
    setRowState( it->second, wasStopped ? RowState::CANCELLED : RowState::DONE );

  relayout();

  if( !hasActiveRows() )
    Hide();
}


void ProgressListDialog::setProgress( void* key, int value, const wxString& message )
{
  auto it = rows.find( key );
  if( it == rows.end() )
    return;

  Row& row = it->second;
  row.gauge->SetValue( value );
  if( !row.cancelRequested && !message.IsEmpty() )
    row.message->SetLabel( message );
  updateTimes( row, (double)value / MAX_PROGRESS_BAR_VALUE );
}


bool ProgressListDialog::isCancelled( void* key ) const
{
  auto it = rows.find( key );
  return it != rows.end() && it->second.cancelRequested;
}


void progressListFunction( void* whichKey, ProgressController* progress )
{
  ProgressListDialog* dialog = ProgressListDialog::getInstance( nullptr );

  if( progress->getCurrentProgress() == 0.0 ||
      ( progress->getCurrentProgress() - progress->getLastUpdate() ) / progress->getEndLimit() >= 0.01 )
  {
    int p;
    if( progress->getCurrentProgress() > progress->getEndLimit() )
      p = MAX_PROGRESS_BAR_VALUE;
    else
      p = (int)std::floor( ( progress->getCurrentProgress() * MAX_PROGRESS_BAR_VALUE ) / progress->getEndLimit() );

    wxString newMessage;
    if( progress->getMessageChanged() )
    {
      newMessage = wxString::FromUTF8( progress->getMessage().c_str() );
      progress->clearMessageChanged();
    }

    dialog->setProgress( whichKey, p, newMessage );
    progress->setLastUpdate( progress->getCurrentProgress() );

    // Repaint and receive Cancel clicks.
    wxEventLoopBase* loop = wxEventLoopBase::GetActive();
    if( loop != nullptr )
      loop->YieldFor( wxEVT_CATEGORY_UI | wxEVT_CATEGORY_USER_INPUT );
  }

  if( dialog->isCancelled( whichKey ) )
    progress->setStop( true );
}


ProgressListScope::ProgressListScope( wxWindow* parent,
                                      void* whichKey,
                                      const wxString& title,
                                      ProgressController* whichProgress,
                                      std::function< std::vector< ProgressListDialog::PendingView >() > whichPendingViews ) :
        dialog( ProgressListDialog::getInstance( parent ) ),
        key( whichKey ),
        progress( whichProgress ),
        pendingViews( whichPendingViews )
{
  dialog->syncPending( pendingViews() );
  dialog->setRunning( key, title );

  progress->setHandler( key, progressListFunction );
  if( dialog->isCancelled( key ) )
    progress->setStop( true );

  disabler.reset( new wxWindowDisabler( dialog ) );

  wxEventLoopBase* loop = wxEventLoopBase::GetActive();
  if( loop != nullptr )
    loop->YieldFor( wxEVT_CATEGORY_UI );
}


ProgressListScope::~ProgressListScope()
{
  disabler.reset();
  dialog->syncPending( pendingViews() );
  dialog->finish( key, progress->getStop() );
}
