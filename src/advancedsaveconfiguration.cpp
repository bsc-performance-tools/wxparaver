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
//#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes
#include "advancedsaveconfiguration.h"
#include <wx/statline.h>

////@begin XPM images
////@end XPM images

using namespace std;

class OriginalNameData : public wxObject
{
  public:
    std::string myOriginalName;
};

class CheckboxLinkData : public wxObject
{
  public:
    CheckboxLinkData()
    {}

    ~CheckboxLinkData()
    {}
  
    void setPropertyName( const string& whichName )
    {
      propertyName = whichName;
    }

    void setData( Window *whichWindow )
    {
      myWindow = whichWindow;
      myHistogram = NULL;
    }

    void setData( Histogram *whichHistogram )
    {
      myWindow = NULL;
      myHistogram = whichHistogram;
    }

    string& getPropertyName()
    {
      return propertyName;
    }

    void getData( Window *&onWindow )
    {
      onWindow = myWindow;
    }
    
    void getData( Histogram *&onHistogram )
    {
      onHistogram = myHistogram;
    }

  private:
    string propertyName;
    Window *myWindow;
    Histogram *myHistogram;
};

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
  EVT_TOGGLEBUTTON( ID_TOGGLEBUTTON_LIST_SELECTED, AdvancedSaveConfiguration::OnToggleOnlySelectedClick )
  EVT_BUTTON( wxID_CANCEL, AdvancedSaveConfiguration::OnCancelClick )
  EVT_BUTTON( wxID_SAVE, AdvancedSaveConfiguration::OnSaveClick )
////@end AdvancedSaveConfiguration event table entries

END_EVENT_TABLE()

#define PARAM_SEPARATOR "|"
const wxString AdvancedSaveConfiguration::KParamSeparator = _( PARAM_SEPARATOR );
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

  // Backup 
  switch ( editionMode )
  {
    case HISTOGRAM_STATISTIC_TAGS:
      // Recover previous tags for that histogram
      for( vector< Histogram * >::iterator it = histograms.begin(); it != histograms.end(); ++it )
      {
        backupHistogramsCFG4DStatisticsAliasList[ *it ] = (*it)->getCFG4DStatisticsAliasList();
      }
      break;

    case PROPERTIES_TAGS:
      // Recover previous state for all windows and histograms
      for( vector< Window * >::iterator it = timelines.begin(); it != timelines.end(); ++it )
      {
        backupTimelinesCFG4DEnabled[ *it ] = (*it)->getCFG4DEnabled();
        backupTimelinesCFG4DMode[ *it ] = (*it)->getCFG4DMode();
        backupTimelinesCFG4DAliasList[ *it ] = (*it)->getCFG4DAliasList();
        backupTimelinesCFG4DParamAlias[ *it ] = (*it)->getCFG4DParamAliasList();
      }

      for( vector< Histogram * >::iterator it = histograms.begin(); it != histograms.end(); ++it )
      {
        backupHistogramsCFG4DEnabled[ *it ] = (*it)->getCFG4DEnabled();
        backupHistogramsCFG4DMode[ *it ] = (*it)->getCFG4DMode();
        backupHistogramsCFG4DAliasList[ *it ] = (*it)->getCFG4DAliasList();
        backupHistogramsCFG4DStatisticsAliasList[ *it ] = (*it)->getCFG4DStatisticsAliasList();
      }
      break;

    default:
      break;
  }



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
  DisconnectWidgetsTagsPanel( !toggleOnlySelected->GetValue() );
}


/*!
 * Member initialisation
 */

void AdvancedSaveConfiguration::Init()
{
////@begin AdvancedSaveConfiguration member initialisation
  choiceWindow = NULL;
  scrolledWindow = NULL;
  scrolledLinkProperties = NULL;
  toggleOnlySelected = NULL;
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
  itemBoxSizer2->Add(itemBoxSizer3, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer3->Add(itemBoxSizer4, 1, wxGROW|wxFIXED_MINSIZE, 5);

  wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer4->Add(itemBoxSizer5, 0, wxGROW|wxALL|wxFIXED_MINSIZE, 5);

  wxArrayString choiceWindowStrings;
  choiceWindow = new wxChoice( itemDialog1, ID_CHOICE_WINDOW, wxDefaultPosition, wxDefaultSize, choiceWindowStrings, 0 );
  itemBoxSizer5->Add(choiceWindow, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  scrolledWindow = new wxScrolledWindow( itemDialog1, ID_SCROLLEDWINDOW1, wxDefaultPosition, wxSize(450, 400), wxSUNKEN_BORDER|wxVSCROLL|wxTAB_TRAVERSAL );
  itemBoxSizer4->Add(scrolledWindow, 1, wxGROW|wxALL|wxFIXED_MINSIZE, 5);
  scrolledWindow->SetScrollbars(15, 15, 0, 0);

  wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer3->Add(itemBoxSizer8, 1, wxGROW, 5);

  scrolledLinkProperties = new wxScrolledWindow( itemDialog1, ID_SCROLLED_LINK_PROPERTIES, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
  itemBoxSizer8->Add(scrolledLinkProperties, 1, wxGROW|wxALL, 5);
  scrolledLinkProperties->SetScrollbars(1, 1, 0, 0);

  wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer10, 0, wxGROW|wxALL, 5);

  toggleOnlySelected = new wxToggleButton( itemDialog1, ID_TOGGLEBUTTON_LIST_SELECTED, _("View selected"), wxDefaultPosition, wxDefaultSize, 0 );
  toggleOnlySelected->SetValue(false);
  toggleOnlySelected->SetName(wxT("List Selected"));
  itemBoxSizer10->Add(toggleOnlySelected, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  itemBoxSizer10->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer13 = new wxStdDialogButtonSizer;

  itemBoxSizer10->Add(itemStdDialogButtonSizer13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
  wxButton* itemButton14 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer13->AddButton(itemButton14);

  buttonSave = new wxButton( itemDialog1, wxID_SAVE, _("&Save"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer13->AddButton(buttonSave);

  itemStdDialogButtonSizer13->Realize();

////@end AdvancedSaveConfiguration content construction


  // Build choice selector
  for ( vector< Window * >::iterator it = timelines.begin(); it != timelines.end(); ++it )
  {
    choiceWindow->Append( BuildName( *it ) );
  }

  for ( vector< Histogram * >::iterator it = histograms.begin(); it != histograms.end(); ++it )
  {
    choiceWindow->Append( BuildName( *it ) );
  }

  // Build tags panel
  currentItem = 0;
  bool showFullList = !toggleOnlySelected->GetValue();
  if ( timelines.size() > 0 )
  {
    isTimeline = true;
    BuildTagsPanel( timelines[ currentItem ], showFullList );
  }
  else
  {
    isTimeline = false;
    BuildTagsPanel( histograms[ currentItem ], showFullList );
  }

  choiceWindow->SetSelection( currentItem );

  if ( editionMode == HISTOGRAM_STATISTIC_TAGS )
  {
    buttonSave->SetLabel( _("Ok") );
    choiceWindow->Enable( false );
    scrolledLinkProperties->Hide();
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


void AdvancedSaveConfiguration::CleanTagsPanel( bool showFullList )
{
  DisconnectWidgetsTagsPanel( showFullList );
  scrolledWindow->DestroyChildren();
}


void AdvancedSaveConfiguration::DisconnectWidgetsTagsPanel( bool showFullList )
{
  for( map< string, string >::iterator it = renamedTag.begin(); it != renamedTag.end(); ++it )
  {
    if ( allowedLevel( it->first ) && ( showFullList || enabledTag[ it->first ] ))
    {
      wxString currentCheckBoxName = wxString::FromAscii( it->first.c_str() );
      GetCheckBoxByName( currentCheckBoxName )->Disconnect(
              wxEVT_COMMAND_CHECKBOX_CLICKED,
              wxCommandEventHandler( AdvancedSaveConfiguration::OnCheckBoxPropertyClicked ),
              NULL,
              this );
    }
  }
}


void AdvancedSaveConfiguration::BuildTagMaps( const map< string, string > &renamedTagMap,
                                              const bool showFullList )
{
  map< string, bool > auxEnabledFullTagList;
  map< string, string > auxRenamedFullTagsList;

  for( vector< string >::const_iterator it = fullTagList.begin(); it != fullTagList.end(); ++it )
  {
    // Check only the present in renamedTagMap (used by checkboxes)
    bool aliasExists = renamedTagMap.find( *it ) != renamedTagMap.end();

    auxEnabledFullTagList[ *it ] = aliasExists;

    if ( aliasExists )
    {
      // Insert the relation between the tag and its alias 
      auxRenamedFullTagsList[ *it ] =  renamedTagMap.find( *it )->second;
    }
    else
    {
      // Doesn't exist => insert the tag twice if allowed.
      if ( showFullList )
      {
        auxRenamedFullTagsList[ *it ] = *it;
      }
      else
      {
        auxRenamedFullTagsList[ *it ] = string( "" );
      }
    }
  }

  // Finally, copy both maps.
  enabledTag = auxEnabledFullTagList;
  renamedTag = auxRenamedFullTagsList;
}


void AdvancedSaveConfiguration::InsertParametersToTagMaps( const vector< Window::TParamAliasKey > &fullParamList, // maybe not needed, but window
                                                           const Window::TParamAlias &renamedParamAlias,
                                                           const bool showFullList )
{
  vector< string > auxFullTagList;
  map< string, bool > auxEnabledFullTagsList;
  map< string, string > auxRenamedFullTagsList;

  string semanticLevel, function, paramAlias;
  string innerKey;
  PRV_UINT32 numParameter;
  string nameParameter;
  bool enabled;
  Window *currentWindow = timelines[ currentItem ]; // TRY to set this static
  vector< Window::TParamAliasKey > semanticLevelParamKeys;

  // For every given tag:
  for( vector< string >::const_iterator it = fullTagList.begin(); it != fullTagList.end(); ++it )
  {
    if ( allowedLevel( *it ) )
    {
      // first, we copy its information.
      auxFullTagList.push_back( *it );
      auxRenamedFullTagsList[ *it ] = renamedTag[ *it ];
      auxEnabledFullTagsList[ *it ] = enabledTag[ *it ];

      // And then insert its parameters if they exist.
      semanticLevelParamKeys = currentWindow->getCFG4DParamKeysBySemanticLevel( *it, fullParamList );

      TParamIndex curP = 0;
      for( vector< Window::TParamAliasKey >::const_iterator it2 = semanticLevelParamKeys.begin();
           it2 != semanticLevelParamKeys.end(); ++it2 )
      {
        // Tag with parameters!
        currentWindow->splitCFG4DParamAliasKey( *it2, semanticLevel, function, numParameter );

        stringstream auxStr;
        auxStr << numParameter;

        int iSemLevel;
        for( iSemLevel = 0; iSemLevel < DERIVED; ++iSemLevel )
        {
          if( TimelineLevelLabels[ iSemLevel ] == semanticLevel )
            break;
        }
        nameParameter = currentWindow->getFunctionParamName( TWindowLevel( iSemLevel ), TParamIndex( numParameter ) );
        innerKey = *it + string( PARAM_SEPARATOR ) + auxStr.str() + string( PARAM_SEPARATOR ) + function + string(".") + nameParameter;

        if ( renamedParamAlias.find( *it2 ) != renamedParamAlias.end() )
        {
          // if parameter has an alias, insert it.
          enabled = true;
          paramAlias = currentWindow->getCFG4DParamAlias( *it2 );
        }
        else
        {
          // No alias; insert something, like the original name.
          enabled = false;
          paramAlias = currentWindow->getFunctionParamName( TWindowLevel( iSemLevel ), curP );
        }

        auxFullTagList.push_back( innerKey );
        auxEnabledFullTagsList[ innerKey ] = enabled;
        auxRenamedFullTagsList[ innerKey ] = paramAlias;

        curP++;
      }
    }
  }

  // Finally, copy information to attributes.
  fullTagList = auxFullTagList;
  enabledTag  = auxEnabledFullTagsList;
  renamedTag  = auxRenamedFullTagsList;
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


wxBoxSizer *AdvancedSaveConfiguration::BuildTagRowWidgets( map< string, string >::iterator it,
                                                           bool showFullList )
{
  wxBoxSizer *auxBoxSizer = NULL;
  wxBoxSizer *auxBoxSizerLeft = NULL;
  wxCheckBox *auxCheckBox;
  wxTextCtrl *auxTextCtrl;
  wxButton   *auxButton;

  wxString rowLabel;
  wxString rowBaseName;

  if ( showFullList || enabledTag[ it->first ] )
  {
    auxBoxSizer = new wxBoxSizer( wxHORIZONTAL );
    auxBoxSizerLeft = new wxBoxSizer( wxHORIZONTAL );

    rowLabel = wxString::FromAscii( it->first.c_str() );
    rowBaseName = rowLabel;
    if ( rowLabel.AfterLast( KParamSeparator[0] ) != rowLabel )
    {
       rowLabel = rowLabel.AfterLast( KParamSeparator[0] );
       auxBoxSizerLeft->Add( 0, 0, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    }

    auxCheckBox = new wxCheckBox( scrolledWindow,
                                  wxID_ANY,
                                  rowLabel,
                                  wxDefaultPosition,
                                  wxDefaultSize,
                                  0,
                                  wxDefaultValidator,
                                  rowBaseName + KCheckBoxSuffix );
    auxCheckBox->SetValue( enabledTag[ it->first ] );

    auxBoxSizerLeft->Add( auxCheckBox, 2, wxALIGN_LEFT | wxGROW | wxALL, 2 );
    auxBoxSizer->Add( auxBoxSizerLeft, 2, wxALIGN_LEFT | wxALL, 2 );

    wxArrayString forbiddenChars;
    forbiddenChars.Add( wxT("|") );
    wxTextValidator excludeVerticalBar( wxFILTER_EXCLUDE_CHAR_LIST );
    excludeVerticalBar.SetExcludes( forbiddenChars );

    auxTextCtrl = new wxTextCtrl( scrolledWindow,
                                  wxID_ANY,
                                  wxString::FromAscii( it->second.c_str() ),
                                  wxDefaultPosition,
                                  wxDefaultSize,
                                  0,
                                  excludeVerticalBar,
                                  rowBaseName + KTextCtrlSuffix ); 
    auxTextCtrl->Enable( enabledTag[ it->first ] );

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
                          wxCommandEventHandler( AdvancedSaveConfiguration::OnCheckBoxPropertyClicked ),
                          NULL,
                          this ); 
  }

  return auxBoxSizer;
}


void AdvancedSaveConfiguration::BuildTagWidgets( const bool showFullList )
{
  // Create widgets (checkboxes and textboxes ) and connect them
  wxBoxSizer *auxBoxSizer;
  wxBoxSizer *boxSizerCurrentItem = new wxBoxSizer( wxVERTICAL );

  map< string, string >::iterator it;
  for( vector< string >::const_iterator itOrd = fullTagList.begin(); itOrd != fullTagList.end(); ++itOrd )
  {
    if ( allowedLevel( *itOrd ) )
    {
      it = renamedTag.find( *itOrd );

      auxBoxSizer = BuildTagRowWidgets( it, showFullList );
      if ( auxBoxSizer != NULL )
      {
        boxSizerCurrentItem->Add( auxBoxSizer, 0, wxGROW|wxALL, 2 );
      }
    }
  }

  scrolledWindow->SetSizer( boxSizerCurrentItem );
  scrolledWindow->FitInside();
}


void AdvancedSaveConfiguration::BuildTagsPanel( Window *currentWindow, const bool showFullList )
{
  // Build renamedTag and enabledTag maps
  fullTagList = currentWindow->getCFG4DFullTagList();
  BuildTagMaps( currentWindow->getCFG4DAliasList(), showFullList );
  if ( editionMode == PROPERTIES_TAGS )
  {
    InsertParametersToTagMaps( currentWindow->getCFG4DCurrentSelectedFullParamList(),
                               currentWindow->getCFG4DParamAliasList(),
                               showFullList );
  }

  BuildTagWidgets( showFullList );
}


void AdvancedSaveConfiguration::BuildTagsPanel( Histogram *currentHistogram, const bool showFullList )
{
  int selected;

  // Build renamedTag and enabledTag maps
  switch ( editionMode )
  {
    case HISTOGRAM_STATISTIC_TAGS:
      selected = ( currentHistogram->itsCommunicationStat( currentHistogram->getCurrentStat() ) )? 0 : 1;
      currentHistogram->getStatisticsLabels( fullTagList, selected );
      BuildTagMaps( currentHistogram->getCFG4DStatisticsAliasList(), showFullList );
      break;

    case PROPERTIES_TAGS:
      fullTagList = currentHistogram->getCFG4DFullTagList();
      BuildTagMaps( currentHistogram->getCFG4DAliasList(), showFullList );
      break;

    default:
      break;
  }

  BuildTagWidgets( showFullList );
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


void AdvancedSaveConfiguration::OnCheckBoxPropertyClicked( wxCommandEvent& event )
{
  wxCheckBox *currentCheckBox = static_cast<wxCheckBox *>( event.GetEventObject() );
  wxString currentTextCtrlName = currentCheckBox->GetName().BeforeLast( KSuffixSeparator[0] );
  GetTextCtrlByName( currentTextCtrlName )->Enable( currentCheckBox->GetValue() );

  wxButton *relatedButton = GetButtonByName( currentTextCtrlName );
  if ( relatedButton != NULL )
    relatedButton->Enable( currentCheckBox->GetValue() );

  if( editionMode == PROPERTIES_TAGS )
  {
    if( currentCheckBox->GetValue() )
    {
      if( isTimeline )
        unlinkedManager.insertLink( std::string( currentTextCtrlName.mb_str() ), timelines[ currentItem ] );
      else
        unlinkedManager.insertLink( std::string( currentTextCtrlName.mb_str() ), histograms[ currentItem ] );

      unlinkedManager.setCustomName( std::string( currentTextCtrlName.mb_str() ), std::string( GetTextCtrlByName( currentTextCtrlName )->GetValue().mb_str() ) );
    }
    else
    {
      if( isTimeline )
        unlinkedManager.removeLink( std::string( currentTextCtrlName.mb_str() ), timelines[ currentItem ] );
      else
        unlinkedManager.removeLink( std::string( currentTextCtrlName.mb_str() ), histograms[ currentItem ] );
    }

    updateLinkPropertiesWidgets();
  }
}


void AdvancedSaveConfiguration::PreparePanel( bool showFullList )
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
    if ( !allowedLevel( it->first ) || ( !showFullList && !enabledTag[ it->first ] ))
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


void AdvancedSaveConfiguration::TransferDataFromPanel( bool showFullList )
{
  map< string, string > auxActivePropertyTags;
  Window::TParamAliasKey auxParamKey;
  Window::TParamAlias auxActiveParametersTags;
  string semanticLevel;
  string function;
  PRV_UINT32 numParameter;
  string newAlias;

  for( map< string, string >::iterator it = renamedTag.begin(); it != renamedTag.end(); ++it )
  {
    if ( !allowedLevel( it->first ) || ( !showFullList && !enabledTag[ it->first ] ))
      continue;

    wxString currentTagName = wxString::FromAscii( it->first.c_str() );
    enabledTag[ it->first ] = GetCheckBoxByName( currentTagName )->GetValue();

    if ( enabledTag[ it->first ] )
    {
      if ( currentTagName.AfterLast( KParamSeparator[0] ) != currentTagName )
      {
        if ( isTimeline )  // by construction, this the only possibility
        {
          semanticLevel = currentTagName.BeforeFirst( KParamSeparator[0] ).mb_str();
          function = currentTagName.AfterLast( KParamSeparator[0] ).BeforeFirst( wxChar('.') ).mb_str();
          istringstream tmpValue(
                  string( currentTagName.BeforeLast( KParamSeparator[0] ).AfterFirst( KParamSeparator[0] ).mb_str() ) );
          tmpValue >> numParameter;

          auxParamKey = timelines[ currentItem ]->buildCFG4DParamAliasKey( semanticLevel, function, numParameter );
          newAlias = GetTextCtrlByName( currentTagName )->GetValue().mb_str();

          auxActiveParametersTags[ auxParamKey ] = newAlias;
        }
      }
      else
      {
        auxActivePropertyTags[ it->first ] = GetTextCtrlByName( currentTagName )->GetValue().mb_str();
      }
    }
  }

  renamedTag = auxActivePropertyTags;

  if ( isTimeline )
  {
    timelines[ currentItem ]->setCFG4DEnabled( true );
    timelines[ currentItem ]->setCFG4DMode( true );
    timelines[ currentItem ]->setCFG4DAliasList( renamedTag );
    timelines[ currentItem ]->setCFG4DParamAlias( auxActiveParametersTags );
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
  bool showFullList = !toggleOnlySelected->GetValue();
  PreparePanel( showFullList );
  TransferDataFromPanel( showFullList );
  EndModal( wxID_OK );
}

void AdvancedSaveConfiguration::RefreshList( bool showFullList )
{
  PreparePanel( showFullList );
  TransferDataFromPanel( showFullList );
  CleanTagsPanel( showFullList );

  currentItem = GetSelectionIndexCorrected( choiceWindow->GetSelection(), isTimeline );
  if ( isTimeline )
  {
    BuildTagsPanel( timelines[ currentItem ], showFullList );
  }
  else
  {
    BuildTagsPanel( histograms[ currentItem ], showFullList );
  }
}



/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_WINDOW
 */

void AdvancedSaveConfiguration::OnChoiceWindowSelected( wxCommandEvent& event )
{
  RefreshList( !toggleOnlySelected->GetValue() );
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


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_TOGGLEBUTTON
 */

void AdvancedSaveConfiguration::OnToggleOnlySelectedClick( wxCommandEvent& event )
{
  // First, save current information and destroy widgets
  bool previousState = toggleOnlySelected->GetValue();

  PreparePanel( previousState );
  TransferDataFromPanel( previousState );
  CleanTagsPanel( previousState );

  // Second, rebuild widgets list
  bool currentState = !toggleOnlySelected->GetValue();

  currentItem = GetSelectionIndexCorrected( choiceWindow->GetSelection(), isTimeline );
  if ( isTimeline )
  {
    BuildTagsPanel( timelines[ currentItem ], currentState );
  }
  else
  {
    BuildTagsPanel( histograms[ currentItem ], currentState );
  }
}



/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void AdvancedSaveConfiguration::OnCancelClick( wxCommandEvent& event )
{
  switch ( editionMode )
  {
    case HISTOGRAM_STATISTIC_TAGS:
      // Recover previous tags for that histogram
      for( vector< Histogram * >::iterator it = histograms.begin(); it != histograms.end(); ++it )
      {
        (*it)->setCFG4DStatisticsAliasList( backupHistogramsCFG4DStatisticsAliasList[ *it ] );
      }
      break;

    case PROPERTIES_TAGS:
      // Recover previous state for all windows and histograms
      for( vector< Window * >::iterator it = timelines.begin(); it != timelines.end(); ++it )
      {
        (*it)->setCFG4DEnabled( backupTimelinesCFG4DEnabled[ *it ] );
        (*it)->setCFG4DMode( backupTimelinesCFG4DMode[ *it ] );
        (*it)->setCFG4DAliasList( backupTimelinesCFG4DAliasList[ *it ] );
        (*it)->setCFG4DParamAlias( backupTimelinesCFG4DParamAlias[ *it ] );
      }

      for( vector< Histogram * >::iterator it = histograms.begin(); it != histograms.end(); ++it )
      {
        (*it)->setCFG4DEnabled( backupHistogramsCFG4DEnabled[ *it ] );
        (*it)->setCFG4DMode( backupHistogramsCFG4DMode[ *it ] );
        (*it)->setCFG4DAliasList( backupHistogramsCFG4DAliasList[ *it ] );
        (*it)->setCFG4DStatisticsAliasList( backupHistogramsCFG4DStatisticsAliasList[ *it ] );
      }
      break;

    default:
      break;
  }

  EndModal( wxID_CANCEL );
}

void AdvancedSaveConfiguration::OnCheckBoxLinkClicked( wxCommandEvent& event )
{
  Window *tmpWin;
  Histogram *tmpHisto;
  CheckboxLinkData *tmpData = ( CheckboxLinkData *)event.GetEventUserData();
  tmpData->getData( tmpWin );
  if( tmpWin != NULL )
  {
    if( event.IsChecked() )
    {
      unlinkedManager.removeLink( tmpData->getPropertyName(), tmpWin );
      linkedManager.insertLink( tmpData->getPropertyName(), tmpWin );
    }
    else
    {
      unlinkedManager.insertLink( tmpData->getPropertyName(), tmpWin );
      linkedManager.removeLink( tmpData->getPropertyName(), tmpWin );
    }
  }
  else
  {
    tmpData->getData( tmpHisto );
    if( tmpHisto != NULL )
    {
      if( event.IsChecked() )
      {
        unlinkedManager.removeLink( tmpData->getPropertyName(), tmpHisto );
        linkedManager.insertLink( tmpData->getPropertyName(), tmpHisto );
      }
      else
      {
        unlinkedManager.insertLink( tmpData->getPropertyName(), tmpHisto );
        linkedManager.removeLink( tmpData->getPropertyName(), tmpHisto );
      }
    }
  }

  updateLinkPropertiesWidgets();
}

template <typename WindowType>
void AdvancedSaveConfiguration::buildLinkWindowWidget( wxBoxSizer *boxSizerLinks,
                                                       const string& propertyName,
                                                       WindowType *whichWindow,
                                                       bool checked )
{
  wxBoxSizer *boxSizerLinkWindow;
  boxSizerLinkWindow = new wxBoxSizer( wxHORIZONTAL );
  boxSizerLinkWindow->Add( 0, 0, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 20 );
  
  wxCheckBox *auxCheckBox = new wxCheckBox( scrolledLinkProperties,
                                            wxID_ANY,
                                            BuildName( whichWindow ),
                                            wxDefaultPosition,
                                            wxDefaultSize,
                                            0,
                                            wxDefaultValidator );
  auxCheckBox->SetValue( checked );
  CheckboxLinkData *tmpData = new CheckboxLinkData;
  tmpData->setPropertyName( propertyName );
  tmpData->setData( whichWindow );
  auxCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED,
                        wxCommandEventHandler( AdvancedSaveConfiguration::OnCheckBoxLinkClicked ),
                        tmpData,
                        this ); 

  boxSizerLinkWindow->Add( auxCheckBox, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2 );  
  boxSizerLinks->Add( boxSizerLinkWindow );
}

void AdvancedSaveConfiguration::buildWindowsSetWidgets( const string& propertyName, wxBoxSizer *boxSizerLinks, bool checked )
{
  TWindowsSet tmpWinSet;
  THistogramsSet tmpHistoSet;

  if( checked )
  {
    linkedManager.getLinks( propertyName, tmpWinSet );
    linkedManager.getLinks( propertyName, tmpHistoSet );
  }
  else
  {
    unlinkedManager.getLinks( propertyName, tmpWinSet );
    unlinkedManager.getLinks( propertyName, tmpHistoSet );
  }

  for( TWindowsSet::iterator itWin = tmpWinSet.begin(); itWin != tmpWinSet.end(); ++itWin )
  {
    buildLinkWindowWidget( boxSizerLinks, propertyName, *itWin, checked );
  }

  for( THistogramsSet::iterator itHisto = tmpHistoSet.begin(); itHisto != tmpHistoSet.end(); ++itHisto )
  {
    buildLinkWindowWidget( boxSizerLinks, propertyName, *itHisto, checked );
  }
}

void AdvancedSaveConfiguration::OnCheckBoxLinkPropertyClicked( wxCommandEvent& event )
{
  string tmpName = string( ( (wxCheckBox *)event.GetEventObject() )->GetLabel().mb_str() );

  if( event.IsChecked() )
  {
    TWindowsSet tmpWinSet;
    unlinkedManager.getLinks( tmpName, tmpWinSet );
    for( TWindowsSet::iterator it = tmpWinSet.begin(); it != tmpWinSet.end(); ++it )
    {
      unlinkedManager.removeLink( tmpName, *it );
      linkedManager.insertLink( tmpName, *it );
    }

    THistogramsSet tmpHistoSet;
    unlinkedManager.getLinks( tmpName, tmpHistoSet );
    for( THistogramsSet::iterator it = tmpHistoSet.begin(); it != tmpHistoSet.end(); ++it )
    {
      unlinkedManager.removeLink( tmpName, *it );
      linkedManager.insertLink( tmpName, *it ); 
    }
  }
  else
  {
    TWindowsSet tmpWinSet;
    linkedManager.getLinks( tmpName, tmpWinSet );
    for( TWindowsSet::iterator it = tmpWinSet.begin(); it != tmpWinSet.end(); ++it )
    {
      linkedManager.removeLink( tmpName, *it );
      unlinkedManager.insertLink( tmpName, *it );
    }

    THistogramsSet tmpHistoSet;
    linkedManager.getLinks( tmpName, tmpHistoSet );
    for( THistogramsSet::iterator it = tmpHistoSet.begin(); it != tmpHistoSet.end(); ++it )
    {
      linkedManager.removeLink( tmpName, *it ); 
      unlinkedManager.insertLink( tmpName, *it );
    }
  }

  updateLinkPropertiesWidgets();
}


void AdvancedSaveConfiguration::OnLinkedPropertiesNameChanged( wxCommandEvent &event )
{
  string tmpOriginalName  = ( ( OriginalNameData *)event.GetEventUserData() )->myOriginalName;
  linkedManager.setCustomName( tmpOriginalName, string( event.GetString().mb_str() ) );
}


void AdvancedSaveConfiguration::updateLinkPropertiesWidgets()
{
  set<string> links;
  unlinkedManager.getLinksName( links );
  linkedManager.getLinksName( links );

  scrolledLinkProperties->DestroyChildren();

  wxBoxSizer *boxSizerLinks = new wxBoxSizer( wxVERTICAL );

  for( set<string>::iterator it = links.begin(); it != links.end(); ++it )
  {
    wxBoxSizer *boxSizerOriginalName = new wxBoxSizer( wxHORIZONTAL );

    wxString originalNameLabel = wxString::FromAscii( (*it).c_str() );
    wxString fullOriginalNameLabel = originalNameLabel;
    if ( originalNameLabel.AfterLast( KParamSeparator[0] ) != originalNameLabel )
       originalNameLabel = originalNameLabel.AfterLast( KParamSeparator[0] );

    wxCheckBox *auxCheckBox = new wxCheckBox( scrolledLinkProperties,
                                              wxID_ANY,
                                              originalNameLabel,
                                              wxDefaultPosition,
                                              wxDefaultSize,
                                              0,
                                              wxDefaultValidator,
                                              fullOriginalNameLabel + KCheckBoxSuffix );
    auxCheckBox->SetValue( unlinkedManager.getLinksSize( *it ) == 0 );
    auxCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED,
                          wxCommandEventHandler( AdvancedSaveConfiguration::OnCheckBoxLinkPropertyClicked ),
                          NULL,
                          this ); 

    boxSizerOriginalName->Add( auxCheckBox, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALL, 2 );


    wxString tmpCustomName;
    if( linkedManager.getLinksSize( *it ) > 0 )
      tmpCustomName = linkedManager.getCustomName( *it );
    else
      tmpCustomName = unlinkedManager.getCustomName( *it );

    wxArrayString forbiddenChars;
    forbiddenChars.Add( wxT("|") );
    wxTextValidator excludeVerticalBar( wxFILTER_EXCLUDE_CHAR_LIST );
    excludeVerticalBar.SetExcludes( forbiddenChars );

    wxTextCtrl *customNameText = new wxTextCtrl( scrolledLinkProperties,
                                                 wxID_ANY,
                                                 tmpCustomName,
                                                 wxDefaultPosition,
                                                 wxDefaultSize,
                                                 0,
                                                 excludeVerticalBar );
    OriginalNameData *tmpData = new OriginalNameData();
    tmpData->myOriginalName = *it;
    customNameText->Connect( wxEVT_TEXT,
                             wxCommandEventHandler( AdvancedSaveConfiguration::OnLinkedPropertiesNameChanged ),
                             tmpData,
                             this ); 

    boxSizerOriginalName->Add( customNameText, 2, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALL, 2 );

    boxSizerLinks->Add( boxSizerOriginalName, 0, wxEXPAND );

    buildWindowsSetWidgets( *it, boxSizerLinks, false );
    buildWindowsSetWidgets( *it, boxSizerLinks, true );
    
    boxSizerLinks->Add( new wxStaticLine( scrolledLinkProperties ), 0, wxEXPAND|wxALL, 3 );
  }

  scrolledLinkProperties->SetSizer( boxSizerLinks );
  scrolledLinkProperties->FitInside();
}

/*!
 * CFGS4DPropertyWindowsList Methods
 */

void CFGS4DPropertyWindowsList::setCustomName( string whichName )
{
  customName = whichName;
}

string CFGS4DPropertyWindowsList::getCustomName() const
{
  return customName;
}

void CFGS4DPropertyWindowsList::insertWindow( Window *whichWindow )
{
  windows.insert( whichWindow );
}

void CFGS4DPropertyWindowsList::insertWindow( Histogram *whichHistogram )
{
  histograms.insert( whichHistogram );
}

void CFGS4DPropertyWindowsList::removeWindow( Window *whichWindow )
{
  windows.erase( whichWindow );
}

void CFGS4DPropertyWindowsList::removeWindow( Histogram *whichHistogram )
{
  histograms.erase( whichHistogram );
}

void CFGS4DPropertyWindowsList::getWindowList( TWindowsSet& onSet ) const
{
  onSet = windows;
}

void CFGS4DPropertyWindowsList::getWindowList( THistogramsSet& onSet ) const
{
  onSet = histograms;
}

bool CFGS4DPropertyWindowsList::existsWindow( Window *whichWindow ) const
{
  return windows.find( whichWindow ) != windows.end();
}

bool CFGS4DPropertyWindowsList::existsWindow( Histogram *whichHistogram ) const
{
  return histograms.find( whichHistogram ) != histograms.end();
}

size_t CFGS4DPropertyWindowsList::getListSize() const
{
  return windows.size() + histograms.size();
}

/*
 * CFGS4DLinkedPropertiesManager Methods
 */

void CFGS4DLinkedPropertiesManager::setCustomName( std::string originalName, std::string customName )
{
  enabledProperties[ originalName ].setCustomName( customName );
}

std::string CFGS4DLinkedPropertiesManager::getCustomName( std::string originalName ) const
{
  map<string, CFGS4DPropertyWindowsList>::const_iterator it = enabledProperties.find( originalName );
  if ( it != enabledProperties.end() )
    return it->second.getCustomName();

  return "";
}

void CFGS4DLinkedPropertiesManager::insertLink( std::string originalName, Window *whichWindow )
{
  enabledProperties[ originalName ].insertWindow( whichWindow );
}

void CFGS4DLinkedPropertiesManager::insertLink( std::string originalName, Histogram *whichHistogram )
{
  enabledProperties[ originalName ].insertWindow( whichHistogram );
}

void CFGS4DLinkedPropertiesManager::removeLink( std::string originalName, Window *whichWindow )
{
  enabledProperties[ originalName ].removeWindow( whichWindow );
  if( enabledProperties[ originalName ].getListSize() == 0 )
    enabledProperties.erase( originalName );
}

void CFGS4DLinkedPropertiesManager::removeLink( std::string originalName, Histogram *whichHistogram )
{
  enabledProperties[ originalName ].removeWindow( whichHistogram );
  if( enabledProperties[ originalName ].getListSize() == 0 )
    enabledProperties.erase( originalName );
}

void CFGS4DLinkedPropertiesManager::getLinks( std::string whichName, TWindowsSet& onSet ) const
{
  map<string, CFGS4DPropertyWindowsList>::const_iterator it = enabledProperties.find( whichName );
  if ( it != enabledProperties.end() )
    it->second.getWindowList( onSet );  
}

void CFGS4DLinkedPropertiesManager::getLinks( std::string whichName, THistogramsSet& onSet ) const
{
  map<string, CFGS4DPropertyWindowsList>::const_iterator it = enabledProperties.find( whichName );
  if ( it != enabledProperties.end() )
    it->second.getWindowList( onSet );  
}

void CFGS4DLinkedPropertiesManager::getLinksName( std::set<std::string>& onSet ) const
{
  for( std::map< std::string, CFGS4DPropertyWindowsList >::const_iterator it = enabledProperties.begin();
       it != enabledProperties.end(); ++it )
  {
    onSet.insert( it->first );
  }
}

size_t CFGS4DLinkedPropertiesManager::getLinksSize( const std::string whichName ) const
{
  map<string, CFGS4DPropertyWindowsList>::const_iterator it = enabledProperties.find( whichName );
  if ( it != enabledProperties.end() )
    return  it->second.getListSize();

  return 0;
}
