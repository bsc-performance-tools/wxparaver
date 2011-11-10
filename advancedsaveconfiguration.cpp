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

#include "advancedsaveconfiguration.h"

////@begin XPM images
////@end XPM images


/*!
 * AdvancedSaveConfiguration type definition
 */

IMPLEMENT_DYNAMIC_CLASS( AdvancedSaveConfiguration, wxDialog )


/*!
 * AdvancedSaveConfiguration event table definition
 */

BEGIN_EVENT_TABLE( AdvancedSaveConfiguration, wxDialog )

////@begin AdvancedSaveConfiguration event table entries
  EVT_CHOICE( ID_CHOICE_WINDOW, AdvancedSaveConfiguration::OnChoiceWindowSelected )

  EVT_BUTTON( wxID_SAVE, AdvancedSaveConfiguration::OnSaveClick )

////@end AdvancedSaveConfiguration event table entries

END_EVENT_TABLE()

const wxString AdvancedSaveConfiguration::KSuffixSeparator = _( "_" );
const wxString AdvancedSaveConfiguration::KTextCtrlSuffix = AdvancedSaveConfiguration::KSuffixSeparator +
                                                            _( "TXTCTRL" );
const wxString AdvancedSaveConfiguration::KCheckBoxSuffix = AdvancedSaveConfiguration::KSuffixSeparator +
                                                            _( "CHECKBOX" );
const wxString AdvancedSaveConfiguration::KButtonSuffix   = AdvancedSaveConfiguration::KSuffixSeparator +
                                                            _( "BUTTON" );

/*!
 * AdvancedSaveConfiguration constructors
 */

AdvancedSaveConfiguration::AdvancedSaveConfiguration()
{
  Init();
}

AdvancedSaveConfiguration::AdvancedSaveConfiguration( wxWindow* parent,
                                                      wxWindowID id,
                                                      const wxString& caption,
                                                      const wxPoint& pos,
                                                      const wxSize& size,
                                                      long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}

// PRECOND: whichTimelines.size() > 0 || whichHistograms.size() > 0
AdvancedSaveConfiguration::AdvancedSaveConfiguration( wxWindow* parent,
                                                      const vector< Window * > &whichTimelines,
                                                      const vector< Histogram * > &whichHistograms,
                                                      TEditorMode whichMode,
                                                      wxWindowID id,
                                                      const wxString& caption,
                                                      const wxPoint& pos,
                                                      const wxSize& size,
                                                      long style )
{
  Init();

  timelines  = whichTimelines;
  histograms = whichHistograms;
  editionMode = whichMode;

  Create( parent, id, caption, pos, size, style );
}


/*!
 * AdvancedSaveConfiguration creator
 */

bool AdvancedSaveConfiguration::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin AdvancedSaveConfiguration creation
  SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Centre();
////@end AdvancedSaveConfiguration creation
  return true;
}


/*!
 * AdvancedSaveConfiguration destructor
 */

AdvancedSaveConfiguration::~AdvancedSaveConfiguration()
{
  DisconnectWidgetsTagsPanel();
}


/*!
 * Member initialisation
 */

void AdvancedSaveConfiguration::Init()
{
////@begin AdvancedSaveConfiguration member initialisation
  choiceWindow = NULL;
  scrolledWindow = NULL;
  buttonSave = NULL;
////@end AdvancedSaveConfiguration member initialisation
  isTimeline = true;
  currentItem = 0;
  editionMode = PROPERTIES_TAGS;
}


/*!
 * Control creation for AdvancedSaveConfiguration
 */

void AdvancedSaveConfiguration::CreateControls()
{    
////@begin AdvancedSaveConfiguration content construction
  AdvancedSaveConfiguration* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL|wxFIXED_MINSIZE, 5);

  wxArrayString choiceWindowStrings;
  choiceWindow = new wxChoice( itemDialog1, ID_CHOICE_WINDOW, wxDefaultPosition, wxDefaultSize, choiceWindowStrings, 0 );
  itemBoxSizer3->Add(choiceWindow, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  scrolledWindow = new wxScrolledWindow( itemDialog1, ID_SCROLLEDWINDOW1, wxDefaultPosition, wxSize(450, 400), wxSUNKEN_BORDER|wxVSCROLL|wxTAB_TRAVERSAL );
  itemBoxSizer2->Add(scrolledWindow, 1, wxGROW|wxALL|wxFIXED_MINSIZE, 5);
  scrolledWindow->SetScrollbars(15, 15, 0, 0);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer6 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer6, 0, wxALIGN_RIGHT|wxALL, 5);
  wxButton* itemButton7 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer6->AddButton(itemButton7);

  buttonSave = new wxButton( itemDialog1, wxID_SAVE, _("&Save"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer6->AddButton(buttonSave);

  itemStdDialogButtonSizer6->Realize();

////@end AdvancedSaveConfiguration content construction


  // Build choice list
  for ( vector< Window * >::iterator it = timelines.begin(); it != timelines.end(); ++it )
  {
    choiceWindow->Append( BuildName( *it ) );
  }

  for ( vector< Histogram * >::iterator it = histograms.begin(); it != histograms.end(); ++it )
  {
    choiceWindow->Append( BuildName( *it ) );
  }

  currentItem = 0;
  if ( timelines.size() > 0 )
  {
    isTimeline = true;
    BuildTagsPanel( timelines[ currentItem ] );
  }
  else
  {
    isTimeline = false;
    BuildTagsPanel( histograms[ currentItem ] );
  }

  choiceWindow->SetSelection( currentItem );

  if ( editionMode == HISTOGRAM_STATISTIC_TAGS )
  {
    buttonSave->SetLabel( _("Ok") );
    choiceWindow->Enable( false );

    // doesn't work
    //SetPosition( wxPoint( GetParent()->GetPosition().x + 20 ,
    //                      GetParent()->GetPosition().y + 20 ));
  }
}


wxString AdvancedSaveConfiguration::BuildName( Window *current )
{
  return  ( wxString::FromAscii( current->getName().c_str() ) + _( " @ " ) +
            wxString::FromAscii( current->getTrace()->getTraceNameNumbered().c_str() ) );
}


wxString AdvancedSaveConfiguration::BuildName( Histogram *current )
{
  return  ( wxString::FromAscii( current->getName().c_str() ) + _( " @ " ) +
            wxString::FromAscii( current->getTrace()->getTraceNameNumbered().c_str() ) );
}


void AdvancedSaveConfiguration::CleanTagsPanel()
{
  DisconnectWidgetsTagsPanel();
  scrolledWindow->DestroyChildren();
}


void AdvancedSaveConfiguration::DisconnectWidgetsTagsPanel()
{
  for( map< string, string >::iterator it = renamedTag.begin(); it != renamedTag.end(); ++it )
  {
    if ( allowedLevel( it->first ) )
    {
      wxString currentCheckBoxName = wxString::FromAscii( it->first.c_str() );
      GetCheckBoxByName( currentCheckBoxName )->Disconnect(
              wxEVT_COMMAND_CHECKBOX_CLICKED,
              wxCommandEventHandler( AdvancedSaveConfiguration::OnCheckBoxClicked ),
              NULL,
              this );
    }
  }
}


void AdvancedSaveConfiguration::BuildTagMaps( const vector< string > &fullTagList,
                                              const map< string, string > &renamedTagMap,
                                              map< string, bool > &whichEnabledFullTagList,
                                              map< string, string > &whichRenamedFullTagList )
{
  map< string, bool > auxEnabledFullTagList;
  map< string, string > auxRenamedFullTagsList;

  for( vector< string >::const_iterator it = fullTagList.begin(); it != fullTagList.end(); ++it )
  {
    // check only the present in renamedTagMap
    bool exists = renamedTagMap.find( *it ) != renamedTagMap.end();
    auxEnabledFullTagList[ *it ] = exists;
    
    // and fill the whole list
    if ( exists )
    {
      auxRenamedFullTagsList[ *it ] =  renamedTagMap.find( *it )->second; // found!
    }
    else
    {
      auxRenamedFullTagsList[ *it ] = *it; // doesn't exist => copy the tag/key
    }
  }

  whichEnabledFullTagList = auxEnabledFullTagList;
  whichRenamedFullTagList = auxRenamedFullTagsList;
}

bool AdvancedSaveConfiguration::allowedLevel( const string &tag )
{
  bool allowed = false;
 

  if ( isTimeline )
  {
    Window *currentWindow = timelines[ currentItem ];
    if ( tag == SingleTimelinePropertyLabels[ SINGLE_COMPOSEWORKLOAD ] ||
         tag == SingleTimelinePropertyLabels[ SINGLE_WORKLOAD ] )
    {
      if ( currentWindow->getLevel() == WORKLOAD )
        allowed = true;
    }
    else if ( tag == SingleTimelinePropertyLabels[ SINGLE_COMPOSEAPPL ] ||
              tag == SingleTimelinePropertyLabels[ SINGLE_APPLICATION ] )
    {
      if ( currentWindow->getLevel() >= WORKLOAD && currentWindow->getLevel() <= APPLICATION )
        allowed = true;
    }
    else if ( tag == SingleTimelinePropertyLabels[ SINGLE_COMPOSETASK ] ||
              tag == SingleTimelinePropertyLabels[ SINGLE_TASK ] )
    {
      if ( currentWindow->getLevel() >= WORKLOAD && currentWindow->getLevel() <= TASK )
        allowed = true;
    }
    else if ( tag == SingleTimelinePropertyLabels[ SINGLE_COMPOSETHREAD ] ||
              tag == SingleTimelinePropertyLabels[ SINGLE_THREAD ] )
    {
      allowed = true;
    }
    else if ( tag == SingleTimelinePropertyLabels[ SINGLE_COMPOSESYSTEM ] ||
              tag == SingleTimelinePropertyLabels[ SINGLE_SYSTEM ] )
    {
      if ( currentWindow->getLevel() == SYSTEM )
        allowed = true;
    }
    else if ( tag == SingleTimelinePropertyLabels[ SINGLE_COMPOSENODE ] ||
              tag == SingleTimelinePropertyLabels[ SINGLE_NODE ] )
    {
      if ( currentWindow->getLevel() >= SYSTEM && currentWindow->getLevel() <= NODE )
        allowed = true;
    }
    else if ( tag == SingleTimelinePropertyLabels[ SINGLE_COMPOSECPU ] ||
              tag == SingleTimelinePropertyLabels[ SINGLE_CPU ] )
    {
      if ( currentWindow->getLevel() >= SYSTEM && currentWindow->getLevel() <= CPU )
        allowed = true;
    }
    else
      allowed = true;
  }
  else
    allowed = true;

  return allowed;
}


void AdvancedSaveConfiguration::BuildTagWidgets( const vector< string > &fullTagList )
{
  // Create widgets (checkboxes and textboxes ) and connect them
  wxBoxSizer *auxBoxSizer;
  wxCheckBox *auxCheckBox;
  wxTextCtrl *auxTextCtrl;
  wxButton   *auxButton;

  wxBoxSizer *boxSizerCurrentItem = new wxBoxSizer( wxVERTICAL );

  map< string, string >::iterator it;

  for( vector< string >::const_iterator itOrd = fullTagList.begin(); itOrd != fullTagList.end(); ++itOrd )
  {
    if ( allowedLevel( *itOrd ) )
    {
      it = renamedTag.find( *itOrd );
      auxBoxSizer = new wxBoxSizer( wxHORIZONTAL );

      auxCheckBox = new wxCheckBox( scrolledWindow,
                                    wxID_ANY,
                                    wxString::FromAscii( it->first.c_str() ),
                                    wxDefaultPosition,
                                    wxDefaultSize,
                                    0,
                                    wxDefaultValidator,
                                    wxString::FromAscii( it->first.c_str() ) + KCheckBoxSuffix );
      /*if ( editionMode == HISTOGRAM_STATISTIC_TAGS ) // && complete
      {
        auxCheckBox->SetValue( true );
      }
      else*/
      {
        auxCheckBox->SetValue( enabledTag[ it->first ] );
      }

      auxBoxSizer->Add( auxCheckBox, 2, wxALIGN_LEFT | wxGROW | wxALL, 2 );

      wxTextValidator excludeVerticalBar( wxFILTER_EXCLUDE_LIST );
      wxArrayString forbiddenChars;
      forbiddenChars.Add( wxT('|') );
      excludeVerticalBar.SetExcludes( forbiddenChars );
      auxTextCtrl = new wxTextCtrl( scrolledWindow,
                                    wxID_ANY,
                                    wxString::FromAscii( it->second.c_str() ),
                                    wxDefaultPosition,
                                    wxDefaultSize,
                                    0,
                                    wxDefaultValidator,
                                    // wxTextValidator( wxFILTER_ALPHANUMERIC ),
                                    //excludeVerticalBar,
                                    wxString::FromAscii( it->first.c_str() ) + KTextCtrlSuffix ); 
      //if ( editionMode == HISTOGRAM_STATISTIC_TAGS ) // && complete
      //{
      //  auxCheckBox->SetValue( true );
      //}
      //else
      {
        auxTextCtrl->Enable( enabledTag[ it->first ] );
      }

      auxTextCtrl->SetValidator( excludeVerticalBar );

      auxBoxSizer->Add( auxTextCtrl, 2, wxEXPAND | wxGROW | wxALL, 2 );

      if ( editionMode == PROPERTIES_TAGS )
      {
        if( wxString::FromAscii( it->first.c_str() ) == _( "Statistic" ) )
        {
          auxButton = new wxButton( scrolledWindow,
                                    wxID_ANY, _("..."),
                                    wxDefaultPosition,
                                    wxDefaultSize,
                                    wxBU_EXACTFIT,
                                    wxDefaultValidator,
                                    wxString::FromAscii( it->first.c_str() ) + KButtonSuffix );
          // auxBoxSizer->Add( auxButton, 0, wxEXPAND | wxGROW | wxALL, 2 );
          auxButton->Enable( enabledTag[ it->first ] );
          auxBoxSizer->Add( auxButton, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2 );

          auxButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
                          wxCommandEventHandler( AdvancedSaveConfiguration::OnStatisticsButtonClick ),
                          NULL,
                          this ); 

        }
        else
        {
          auxBoxSizer->Add(2, 2, 1, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
        }
      }

      auxCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED,
                            wxCommandEventHandler( AdvancedSaveConfiguration::OnCheckBoxClicked ),
                            NULL,
                            this ); 

      boxSizerCurrentItem->Add( auxBoxSizer, 0, wxGROW|wxALL, 2 );
    }
  }

  scrolledWindow->SetSizer( boxSizerCurrentItem );
  scrolledWindow->FitInside();
}


void AdvancedSaveConfiguration::BuildTagsPanel( Window *currentWindow )
{
  // Build renamedTag and enabledTag maps
  BuildTagMaps( currentWindow->getCFG4DFullTagList(),
                currentWindow->getCFG4DAliasList(),
                enabledTag,
                renamedTag );

  BuildTagWidgets( currentWindow->getCFG4DFullTagList() );
}


void AdvancedSaveConfiguration::BuildTagsPanel( Histogram *currentHistogram )
{
  vector< string > fullTagList;
  int selected;

  // Build renamedTag and enabledTag maps
  switch ( editionMode )
  {
    case HISTOGRAM_STATISTIC_TAGS:
      selected = ( currentHistogram->itsCommunicationStat( currentHistogram->getCurrentStat() ) )? 0 : 1;
      currentHistogram->getStatisticsLabels( fullTagList, selected );
      BuildTagMaps( fullTagList,
                    currentHistogram->getCFG4DStatisticsAliasList(),
                    enabledTag,
                    renamedTag );
      break;
    case PROPERTIES_TAGS:
      fullTagList = currentHistogram->getCFG4DFullTagList();
      BuildTagMaps( fullTagList,
                    currentHistogram->getCFG4DAliasList(),
                    enabledTag,
                    renamedTag );
      break;
    default:
      break;
  }

  BuildTagWidgets( fullTagList );
}

/*!
 * Should we show tooltips?
 */

bool AdvancedSaveConfiguration::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap AdvancedSaveConfiguration::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin AdvancedSaveConfiguration bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
////@end AdvancedSaveConfiguration bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon AdvancedSaveConfiguration::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin AdvancedSaveConfiguration icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end AdvancedSaveConfiguration icon retrieval
}


wxCheckBox *AdvancedSaveConfiguration::GetCheckBoxByName( const wxString& widgetName ) const
{
  wxString currentCheckBoxName = widgetName + KCheckBoxSuffix;
  wxWindow *relatedwxWidget = scrolledWindow->FindWindowByName( currentCheckBoxName );
  return static_cast<wxCheckBox *>( relatedwxWidget );
}


wxTextCtrl *AdvancedSaveConfiguration::GetTextCtrlByName( const wxString& widgetName ) const
{
  wxString currentTextCtrlName = widgetName + KTextCtrlSuffix;
  wxWindow *relatedwxWidget = scrolledWindow->FindWindowByName( currentTextCtrlName );
  return static_cast<wxTextCtrl *>( relatedwxWidget );
}


wxButton *AdvancedSaveConfiguration::GetButtonByName( const wxString& widgetName ) const
{
  wxString currentButtonName = widgetName + KButtonSuffix;
  wxWindow *relatedwxWidget = scrolledWindow->FindWindowByName( currentButtonName );
  return static_cast<wxButton *>( relatedwxWidget );
}

void AdvancedSaveConfiguration::OnCheckBoxClicked( wxCommandEvent& event )
{
  wxCheckBox *currentCheckBox = static_cast<wxCheckBox *>( event.GetEventObject() );
  wxString currentTextCtrlName = currentCheckBox->GetName().BeforeLast( KSuffixSeparator[0] );
  GetTextCtrlByName( currentTextCtrlName )->Enable( currentCheckBox->GetValue() );

  wxButton *relatedButton = GetButtonByName( currentTextCtrlName );
  if ( relatedButton != NULL )
    relatedButton->Enable( currentCheckBox->GetValue() );
}

void AdvancedSaveConfiguration::PreparePanel()
{
  // Check if some selected textctrl is empty => rewritten with value
  wxTextCtrl *currentTextCtrl;
  map< string, string > auxMap;

  if ( isTimeline )
  {
    auxMap = timelines[ currentItem ]->getCFG4DAliasList();
  }
  else
  {
    switch ( editionMode )
    {
      case HISTOGRAM_STATISTIC_TAGS:
        auxMap = histograms[ currentItem ]->getCFG4DStatisticsAliasList();
        break;
      case PROPERTIES_TAGS:
        auxMap = histograms[ currentItem ]->getCFG4DAliasList();
        break;
      default:
        break;
    }
  }


  for( map< string, string >::iterator it = renamedTag.begin(); it != renamedTag.end(); ++it )
  {
    if ( !allowedLevel( it->first ) )
      continue;

    wxString currentTagName = wxString::FromAscii( it->first.c_str() );
    currentTextCtrl = GetTextCtrlByName( currentTagName );

    if ( GetCheckBoxByName( currentTagName )->GetValue() && currentTextCtrl->GetValue().IsEmpty() )
    {
      if ( auxMap.find( it->first ) != auxMap.end() )
      {
        // found! => recover the value in the map
        currentTextCtrl->SetValue( wxString::FromAscii( auxMap.find( it->first )->second.c_str() ) );
      }
      else
      {
        // copy the key
        currentTextCtrl->SetValue( wxString::FromAscii( it->first.c_str() ) );
      }
    }
  }
}


void AdvancedSaveConfiguration::TransferDataFromPanel()
{
  map< string, string > auxActiveTags;

  for( map< string, string >::iterator it = renamedTag.begin(); it != renamedTag.end(); ++it )
  {
    if ( !allowedLevel( it->first ) )
      continue;

    wxString currentTagName = wxString::FromAscii( it->first.c_str() );

    enabledTag[ it->first ] = GetCheckBoxByName( currentTagName )->GetValue();
    if ( enabledTag[ it->first ] )
    {
      auxActiveTags[ it->first ] = GetTextCtrlByName( currentTagName )->GetValue().mb_str();
    }
  }

  renamedTag = auxActiveTags;

  if ( isTimeline )
  {
    timelines[ currentItem ]->setCFG4DEnabled( true );
    timelines[ currentItem ]->setCFG4DMode( true );
    timelines[ currentItem ]->setCFG4DAliasList( renamedTag );
  }
  else
  {
    histograms[ currentItem ]->setCFG4DEnabled( true );
    histograms[ currentItem ]->setCFG4DMode( true );

    switch ( editionMode )
    {
      case HISTOGRAM_STATISTIC_TAGS:
        histograms[ currentItem ]->setCFG4DStatisticsAliasList( renamedTag );
        break;
      case PROPERTIES_TAGS:
        histograms[ currentItem ]->setCFG4DAliasList( renamedTag );
      default:
        break;
    }
  }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_SAVE
 */
void AdvancedSaveConfiguration::OnSaveClick( wxCommandEvent& event )
{
  PreparePanel();
  TransferDataFromPanel();
  EndModal( wxID_OK );
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_WINDOW
 */

void AdvancedSaveConfiguration::OnChoiceWindowSelected( wxCommandEvent& event )
{
  PreparePanel();
  TransferDataFromPanel();
  CleanTagsPanel();

  currentItem = GetSelectionIndexCorrected( choiceWindow->GetSelection(), isTimeline );
  if ( isTimeline )
  {
    BuildTagsPanel( timelines[ currentItem ] );
  }
  else
  {
    BuildTagsPanel( histograms[ currentItem ] );
  }
}


int AdvancedSaveConfiguration::GetSelectionIndexCorrected( int index, bool &isTimeline )
{
  isTimeline = ( index <= (int)timelines.size() - 1 );
  if ( !isTimeline )
  {
    index = index - timelines.size();
  }

  return index;
}


void AdvancedSaveConfiguration::OnStatisticsButtonClick( wxCommandEvent& event )
{
  vector< Window * > dummy;
  vector< Histogram * > onlyCurrentHistogram;

  onlyCurrentHistogram.push_back( histograms[ currentItem ] );

  AdvancedSaveConfiguration statisticsEditorDialog(
          (wxWindow *)this,
          dummy,
          onlyCurrentHistogram,
          AdvancedSaveConfiguration::HISTOGRAM_STATISTIC_TAGS,
          wxID_ANY,
          _("Save Basic CFG - Statistics Editor"),
          wxPoint( GetPosition().x + 20 , GetPosition().y + 20 ) ); // doesn't reposition

  if ( statisticsEditorDialog.ShowModal() == wxID_OK )
  {
  }
}
