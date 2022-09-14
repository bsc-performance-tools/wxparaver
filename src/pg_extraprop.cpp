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

#include <wx/choicdlg.h>
#include <wx/event.h>
#include <wx/utils.h>
#include "wxparaverapp.h"
#include "pg_extraprop.h"
#include "window.h"
#include "selectionrowsutils.h"
#include "labelconstructor.h"
#include "filter.h"
#include "eventsselectiondialog.h"
#include "rowsselectiondialog.h"
#include "timelinetreeselector.h"
#include "paraverlabels.h"

using namespace std;

/**********************************************************
 **       prvEventTypeProperty
 **********************************************************/
#if wxMAJOR_VERSION >= 3 && wxMINOR_VERSION >= 1
wxPG_IMPLEMENT_PROPERTY_CLASS( prvEventTypeProperty, wxPGProperty,
                               TextCtrlAndButton )
#else
WX_PG_IMPLEMENT_PROPERTY_CLASS( prvEventTypeProperty, wxPGProperty,
                                wxArrayInt, const wxArrayInt&, TextCtrlAndButton )
#endif

prvEventTypeProperty::prvEventTypeProperty( const wxString& label,
                                            const wxString& name,
                                            const wxPGChoices& choices,
                                            const wxArrayInt& value)
                                              : wxPGProperty(label,name)
{
    m_choices.Assign(choices);
    wxArrayString tmpArray;

    unsigned int i;
    for ( i=0; i<value.GetCount(); i++ )
      tmpArray.Add( wxString().Format(_( "%d" ), value[i] ) );
    SetValue(tmpArray);
}

prvEventTypeProperty::prvEventTypeProperty( const wxString& label,
                                            const wxString& name,
                                            const wxArrayString& strings,
                                            const wxArrayInt& value)
                                              : wxPGProperty(label,name)
{
    m_choices.Set(strings);
    wxArrayString tmpArray;
    unsigned int i;
    for ( i=0; i<value.GetCount(); i++ )
      tmpArray.Add( wxString().Format( _( "%d" ), value[i] ) );
    SetValue(tmpArray);
}

prvEventTypeProperty::prvEventTypeProperty( const wxString& label,
                                            const wxString& name,
                                            const wxArrayInt& value)
                                              : wxPGProperty(label,name)
{
    wxArrayString strings;
    m_choices.Set(strings);
    wxArrayString tmpArray;
    unsigned int i;
    for ( i=0; i<value.GetCount(); i++ )
      tmpArray.Add( wxString().Format( _( "%d" ), value[i] ) );
    SetValue(tmpArray);
}

prvEventTypeProperty::~prvEventTypeProperty()
{
}

void prvEventTypeProperty::OnSetValue()
{
    GenerateValueAsString();
}


wxString prvEventTypeProperty::ValueToString( wxVariant & value, int argFlags ) const
{
  return value.GetString();
}

void prvEventTypeProperty::GenerateValueAsString()
{
    wxString &tempStr = m_display;
    tempStr = GetValue().GetString();
}

wxArrayInt prvEventTypeProperty::GetValueAsIndices() const
{
    const wxArrayInt& valueArr = GetValueAsArrayInt();
    unsigned int i;

    // Translate values to string indices.
    wxArrayInt selections;

    if ( !m_choices.IsOk() || !m_choices.GetCount() )
    {
        for ( i=0; i<valueArr.GetCount(); i++ )
            selections.Add(-1);
    }
    else
    {
        for ( i=0; i<valueArr.GetCount(); i++ )
        {
            int sIndex = m_choices.Index(valueArr[i]);
            if ( sIndex >= 0 )
                selections.Add(sIndex);
        }
    }

    return selections;
}

bool prvEventTypeProperty::OnEvent( wxPropertyGrid* propgrid,
                                     wxWindow* WXUNUSED(primary),
                                     wxEvent& event )
{
    if ( propgrid->IsMainButtonEvent(event) )
    {
        // Update the value
        propgrid->GetUncommittedPropertyValue();

        wxArrayString labels = m_choices.GetLabels();
        unsigned int choiceCount;

        if ( m_choices.IsOk() )
            choiceCount = m_choices.GetCount();
        else
            choiceCount = 0;

        // launch editor dialog
        wxMultiChoiceDialog dlg( propgrid,
                                 _("Make a selection:"),
                                 m_label,
                                 choiceCount,
                                 choiceCount?&labels[0]:nullptr,
                                 wxCHOICEDLG_STYLE );

        dlg.Move( propgrid->GetGoodEditorDialogPosition(this,dlg.GetSize()) );

        wxArrayString strings = m_value.GetArrayString();
        wxArrayString extraStrings;
        wxArrayInt values;
        for( unsigned int idx = 0; idx < strings.GetCount(); idx++ )
        {
          long tmpLong;
          strings[ idx ].ToLong( &tmpLong );
          int tmpValue = m_choices.Index( tmpLong );
          if( tmpValue != -1 )
            values.Add( tmpValue );
          else
            extraStrings.Add( strings[ idx ] );
        }

        dlg.SetSelections(values);
        
        if ( dlg.ShowModal() == wxID_OK && choiceCount )
        {
            int userStringMode = GetAttributeAsLong(wxT("UserStringMode"), 0);

            wxArrayInt arrInt = dlg.GetSelections();

            wxVariant variant;

            // Strings that were not in list of choices
            wxArrayString value;

            // Translate string indices to strings

            unsigned int n;
            if ( userStringMode == 1 )
            {
                for ( n=0; n<extraStrings.size(); n++ )
                    value.push_back( extraStrings[ n ] );
            }

            unsigned int i;
            for ( i=0; i<arrInt.GetCount(); i++ )
                value.Add( wxString() << m_choices.GetValue( arrInt.Item( i ) ) );

            if ( userStringMode == 2 )
            {
                for (n=0;n<extraStrings.size();n++)
                    value.push_back( extraStrings[ n ] );
            }

            value.Sort();
            
            variant = WXVARIANT(value);

            SetValueInEvent(variant);

            return true;
        }
    }
    return false;
}

bool prvEventTypeProperty::StringToValue( wxVariant& variant, const wxString& text, int ) const
{
    wxArrayString arr;

    int userStringMode = GetAttributeAsLong(wxT("UserStringMode"), 0);

    WX_PG_TOKENIZER1_BEGIN(text,wxT(';'))
        if ( userStringMode > 0 || (m_choices.IsOk() && m_choices.Index( token ) != wxNOT_FOUND) )
            arr.Add(token);
    WX_PG_TOKENIZER1_END()

    wxVariant v( WXVARIANT(arr) );
    variant = v;

    return true;
}

wxArrayInt prvEventTypeProperty::GetValueAsArrayInt() const
{
  wxArrayInt retValue;
  wxArrayString strValues = m_value.GetArrayString();
  
  for( unsigned int idx = 0; idx < strValues.GetCount(); idx++ )
  {
    long tmpInt;
    strValues[ idx ].ToLong( &tmpInt );
    retValue.Add( tmpInt );
  }
    
  return retValue;
}

/**********************************************************
 **       prvEventInfoProperty
 **********************************************************/

#if wxMAJOR_VERSION >= 3 && wxMINOR_VERSION >= 1
wxPG_IMPLEMENT_PROPERTY_CLASS( prvEventInfoProperty, wxPGProperty,
                               TextCtrlAndButton )
#else
WX_PG_IMPLEMENT_PROPERTY_CLASS( prvEventInfoProperty, wxPGProperty,
                                wxArrayInt, const wxArrayInt&, TextCtrlAndButton )
#endif

prvEventInfoProperty::prvEventInfoProperty( const wxString& label,
                                            const wxString& name,
                                            const wxArrayString& strings,
                                            const wxArrayInt& value)
                                              : wxPGProperty( label, name )
{
  m_choices.Set( strings );

  wxArrayString tmpArray;
  for ( unsigned int i = 0; i < value.GetCount(); ++i )
    tmpArray.Add( wxString().Format( _( "%d" ), value[ i ] ) );

  SetValue( tmpArray );
}

#ifndef SWIG

prvEventInfoProperty::prvEventInfoProperty( const wxString& label,
                                            const wxString& name,
                                            Timeline *whichWindow,
                                            prvEventInfoType whichInfoType,
                                            const wxPGChoices& choices )
                                              : wxPGProperty( label, name )
{
  m_choices.Assign( choices );

  currentWindow = whichWindow;

  wxArrayString tmpArray;

  vector<TEventType> typesSel;
  vector<TSemanticValue> valuesSel;

  switch( whichInfoType )
  {
    case prvEventInfoType::TYPES:
      currentWindow->getFilter()->getEventType( typesSel );
      for( vector<TEventType>::iterator it = typesSel.begin(); it != typesSel.end(); ++it )
      {
        tmpArray.Add( wxString().Format( _( "%d" ), (*it) ) );
      }
      
      break;

    case prvEventInfoType::VALUES:
      currentWindow->getFilter()->getEventValue( valuesSel );
      for( vector<TSemanticValue>::iterator it = valuesSel.begin(); it != valuesSel.end(); ++it )
      {
        double tmpIntpart;
        if( std::modf( *it, &tmpIntpart ) == 0.0 )
        {
          TEventValue tmpEventValue = tmpIntpart;
          tmpArray.Add( wxString().Format( _( "%lld" ), tmpEventValue ) );
        }
        else
          tmpArray.Add( wxString().Format( _( "%f" ), (*it) ) );
      }
      
      break;
      
    default:
      break;
  }

  SetValue( tmpArray );
}


prvEventInfoProperty::prvEventInfoProperty( const wxString& label,
                                            const wxString& name,
                                            const wxArrayInt& value )
                                              : wxPGProperty(label,name)
{
  wxArrayString strings;
  m_choices.Set( strings );

  wxArrayString tmpArray;
  for( unsigned int i = 0; i < value.GetCount(); ++i )
    tmpArray.Add( wxString().Format( _( "%d" ), value[ i ] ) );

  SetValue( tmpArray );
}

#endif


prvEventInfoProperty::~prvEventInfoProperty()
{
}


void prvEventInfoProperty::OnSetValue()
{
  GenerateValueAsString();
}


wxString prvEventInfoProperty::ValueToString( wxVariant & value, int argFlags ) const
{
  return value.GetString();
}


void prvEventInfoProperty::GenerateValueAsString()
{
  wxString &tempStr = m_display;
  tempStr = GetValue().GetString();
}


wxArrayInt prvEventInfoProperty::GetValueAsIndices() const
{
  const wxArrayInt& valueArr = GetValueAsArrayInt();
  unsigned int i;

  // Translate values to string indices.
  wxArrayInt selections;

  if ( !m_choices.IsOk() || !m_choices.GetCount() )
  {
    for ( i = 0; i < valueArr.GetCount(); ++i )
      selections.Add( -1 );
  }
  else
  {
    for ( i = 0; i < valueArr.GetCount(); ++i )
    {
      int sIndex = m_choices.Index( valueArr[ i ] );
      if ( sIndex >= 0 )
        selections.Add( sIndex );
    }
  }

  return selections;
}

bool prvEventInfoProperty::OnEvent( wxPropertyGrid* propgrid,
                                    wxWindow* WXUNUSED( primary ),
                                    wxEvent& event )
{
  if ( propgrid->IsMainButtonEvent( event ))
  {
    wxArrayString labels;
    unsigned int numLabels = 0;

    // Update the value
    propgrid->GetUncommittedPropertyValue();

    if ( m_choices.IsOk() )
    {
      labels    = m_choices.GetLabels();
      numLabels = m_choices.GetCount();
    }

    bool hideChoiceOperators = false;
    const wxString windowTitle( _("Events Selection") );

    wxArrayString labelsSelected = m_value.GetArrayString();

    EventsSelectionDialog eventsDialog( propgrid,
                                        currentWindow,
                                        hideChoiceOperators,
                                        windowTitle );

    wxparaverApp::mainWindow->SetOpenedPropertyDialog( &eventsDialog );

    if ( eventsDialog.ShowModal() == wxID_OK && numLabels )
    {
      TWindowsSet timelines;
      TWindowsSet allTimelines;

      if ( eventsDialog.ChangedEventTypesFunction() )
      {
        CFGS4DGlobalManager::getInstance()->getLinks( currentWindow->getCFGS4DGroupLink( SingleTimelinePropertyLabels[ SINGLE_EVENTTYPEFUNCTION ] ),
                                                      SingleTimelinePropertyLabels[ SINGLE_EVENTTYPEFUNCTION ],
                                                      timelines );
        if( timelines.size() == 0 )
          timelines.insert( currentWindow );

        for( TWindowsSet::iterator it = timelines.begin(); it != timelines.end(); ++it )
        {
          allTimelines.insert( *it );
          (*it)->getFilter()->setEventTypeFunction( eventsDialog.GetNameEventTypesFunction() );
        }
      }

      if ( eventsDialog.ChangedEventTypesSelection() )
      {
        wxArrayInt tmpEventTypes = eventsDialog.GetEventTypesSelection();

        timelines.clear();
        CFGS4DGlobalManager::getInstance()->getLinks( currentWindow->getCFGS4DGroupLink( SingleTimelinePropertyLabels[ SINGLE_EVENTTYPEVALUES ] ),
                                                      SingleTimelinePropertyLabels[ SINGLE_EVENTTYPEVALUES ],
                                                      timelines );
        if( timelines.size() == 0 )
          timelines.insert( currentWindow );

        for( TWindowsSet::iterator it = timelines.begin(); it != timelines.end(); ++it )
        {
          allTimelines.insert( *it );

          Filter *filter = (*it)->getFilter();
          filter->clearEventTypes();

          for( unsigned int i = 0; i < tmpEventTypes.GetCount(); ++i )
          {
            filter->insertEventType( (TEventType)tmpEventTypes[ i ] );
          }
        }
      }

      if ( eventsDialog.ChangedOperatorTypeValue() )
      {
        int func = eventsDialog.GetIndexOperatorTypeValue();

        timelines.clear();
        CFGS4DGlobalManager::getInstance()->getLinks( currentWindow->getCFGS4DGroupLink( SingleTimelinePropertyLabels[ SINGLE_EVENTTYPEVALUESOP ] ),
                                                      SingleTimelinePropertyLabels[ SINGLE_EVENTTYPEVALUESOP ],
                                                      timelines );
        if( timelines.size() == 0 )
          timelines.insert( currentWindow );

        for( TWindowsSet::iterator it = timelines.begin(); it != timelines.end(); ++it )
        {
          allTimelines.insert( *it );

          if ( func == 0 )
            (*it)->getFilter()->setOpTypeValueAnd();
          else 
            (*it)->getFilter()->setOpTypeValueOr();
        }
      }

      if ( eventsDialog.ChangedEventValuesFunction() )
      {
        timelines.clear();
        CFGS4DGlobalManager::getInstance()->getLinks( currentWindow->getCFGS4DGroupLink( SingleTimelinePropertyLabels[ SINGLE_EVENTVALUEFUNCTION ] ),
                                                      SingleTimelinePropertyLabels[ SINGLE_EVENTVALUEFUNCTION ],
                                                      timelines );
        if( timelines.size() == 0 )
          timelines.insert( currentWindow );

        for( TWindowsSet::iterator it = timelines.begin(); it != timelines.end(); ++it )
        {
          allTimelines.insert( *it );
          (*it)->getFilter()->setEventValueFunction( eventsDialog.GetNameEventValuesFunction() );
        }
      }

      if ( eventsDialog.ChangedEventValuesSelection() )
      {
        wxArrayDouble tmpEventValues = eventsDialog.GetEventValues();

        timelines.clear();
        CFGS4DGlobalManager::getInstance()->getLinks( currentWindow->getCFGS4DGroupLink( SingleTimelinePropertyLabels[ SINGLE_EVENTVALUEVALUES ] ),
                                                      SingleTimelinePropertyLabels[ SINGLE_EVENTVALUEVALUES ],
                                                      timelines );
        if( timelines.size() == 0 )
          timelines.insert( currentWindow );

        for( TWindowsSet::iterator it = timelines.begin(); it != timelines.end(); ++it )
        {
          allTimelines.insert( *it );

          Filter *filter = (*it)->getFilter();
          filter->clearEventValues();
          for( unsigned int i = 0; i < tmpEventValues.GetCount(); ++i )
          {
            filter->insertEventValue( (TSemanticValue)tmpEventValues[ i ] );
          }
        }
      }

      wxparaverApp::mainWindow->SetSomeWinIsRedraw( true );
      for( TWindowsSet::iterator it = allTimelines.begin(); it != allTimelines.end(); ++it )
      {
        paraverMain::myParaverMain->spreadSetChanged( *it );
        paraverMain::myParaverMain->spreadSetRedraw( *it );
      }
      wxparaverApp::mainWindow->SetSomeWinIsRedraw( false );

      wxparaverApp::mainWindow->SetOpenedPropertyDialog( nullptr );

      return true;
    }

    wxparaverApp::mainWindow->SetOpenedPropertyDialog( nullptr );
  }

  return false;
}

bool prvEventInfoProperty::StringToValue( wxVariant& variant, const wxString& text, int ) const
{
    wxArrayString arr;

    int userStringMode = GetAttributeAsLong( wxT("UserStringMode"), 0 );

    WX_PG_TOKENIZER1_BEGIN(text,wxT(';'))
        if ( userStringMode > 0 || (m_choices.IsOk() && m_choices.Index( token ) != wxNOT_FOUND) )
            arr.Add(token);
    WX_PG_TOKENIZER1_END()

    wxVariant v( WXVARIANT(arr) );
    variant = v;

    return true;
}

wxArrayInt prvEventInfoProperty::GetValueAsArrayInt() const
{
  wxArrayInt retValue;
  wxArrayString strValues = m_value.GetArrayString();
  
  for( unsigned int idx = 0; idx < strValues.GetCount(); idx++ )
  {
    long tmpInt;
    strValues[ idx ].ToLong( &tmpInt );
    retValue.Add( tmpInt );
  }

  return retValue;
}

/**********************************************************
 **       prvSemanticThreadProperty
 **********************************************************/

BEGIN_EVENT_TABLE( SemanticMenu, wxMenu )
  EVT_MENU_RANGE( 1, 200, SemanticMenu::OnMenu )
END_EVENT_TABLE()

SemanticMenu::SemanticMenu( const vector<string>& levels,
                            const vector<vector<string> >& functions,
                            const wxString& value,
                            prvSemanticThreadProperty *prop )
                            : myProperty( prop )
{
  int idMenu = 1;
  
  for( unsigned int i = 0; i < levels.size(); ++i )
  {
    subMenus.push_back( new wxMenu() );
    for( vector<string>::const_iterator it = functions[ i ].begin(); 
         it != functions[ i ].end(); ++it )
    {
      wxString tmpStr( wxString::FromUTF8( (*it).c_str() ) );
      subMenus[ i ]->AppendCheckItem( idMenu, tmpStr );
      if( tmpStr == value )
      {
        subMenus[ i ]->Check( idMenu, true );
        currentItemID = idMenu;
      }
      else
        subMenus[ i ]->Check( idMenu, false );
      ++idMenu;
    }
    subMenus[ i ]->Connect( wxEVT_COMMAND_MENU_SELECTED, 
                            wxCommandEventHandler( SemanticMenu::OnMenu ),
                            nullptr,
                            this );
    Append( i, wxString::FromUTF8( levels[ i ].c_str() ), subMenus[ i ] );
  }
}

SemanticMenu::~SemanticMenu()
{}

void SemanticMenu::OnMenu( wxCommandEvent& event )
{
  wxMenuItem *lastItem;
  int itemMenu = 1;
  bool stop = false;
  
  for( vector<wxMenu *>::iterator it = subMenus.begin();
       it != subMenus.end(); ++it )
  {
    if( ( lastItem = (*it)->FindItem( currentItemID ) ) != nullptr )
    {
      if( !lastItem->IsChecked() )
        lastItem->Check( true );
      else
        lastItem->Check( false );
      break;
    }
  }
  for( vector<wxMenu *>::iterator it = subMenus.begin();
       it != subMenus.end(); ++it )
  {
    for( unsigned int i = 0; i < (*it)->GetMenuItemCount(); ++i )
    {
      if( (*it)->IsChecked( itemMenu ) )
      {
        currentItemID = itemMenu;
        stop = true;
        myProperty->SetValueInEvent( wxString( (*it)->GetLabelText( itemMenu ) ) );
        break;
      }
      ++itemMenu;
    }
    if( stop ) break;
  }
}

void SemanticMenu::PopupMenu( wxWindow *onWindow )
{
  onWindow->PopupMenu( this );
}


#if wxMAJOR_VERSION >= 3 && wxMINOR_VERSION >= 1
wxPG_IMPLEMENT_PROPERTY_CLASS( prvSemanticThreadProperty, wxPGProperty,
                               TextCtrlAndButton )
#else
WX_PG_IMPLEMENT_PROPERTY_CLASS( prvSemanticThreadProperty, wxPGProperty,
                                wxString, wxString&, TextCtrlAndButton )
#endif

prvSemanticThreadProperty::prvSemanticThreadProperty( const wxString& label,
                                                      const wxString& name,
                                                      const vector<string>& levels,
                                                      const vector<vector<string> >& functions,
                                                      const wxString& value )
                                                        : wxPGProperty(label,name)
{
  SetValue( value );
  
  myMenu = new SemanticMenu( levels, functions, value, this );
}

prvSemanticThreadProperty::~prvSemanticThreadProperty()
{
}

bool prvSemanticThreadProperty::OnEvent( wxPropertyGrid* propgrid,
                                         wxWindow* WXUNUSED(primary),
                                         wxEvent& event )
{
  if ( propgrid->IsMainButtonEvent(event) )
  {
    myMenu->PopupMenu( propgrid );
  }
  return true;
}

wxString prvSemanticThreadProperty::ValueToString( wxVariant & value, int argFlags ) const
{
  return value.GetString();
}


/**********************************************************
 **       prvRowsSelectionProperty
 **********************************************************/

#if wxMAJOR_VERSION >= 3 && wxMINOR_VERSION >= 1
wxPG_IMPLEMENT_PROPERTY_CLASS( prvRowsSelectionProperty, wxPGProperty,
                               TextCtrlAndButton )
#else
WX_PG_IMPLEMENT_PROPERTY_CLASS( prvRowsSelectionProperty, wxPGProperty,
                                wxString, wxString&, TextCtrlAndButton )
#endif

prvRowsSelectionProperty::prvRowsSelectionProperty( const wxString &label,
                                                    const wxString &name,
                                                    Timeline *whichWindow,
                                                    const wxString &windowName,
                                                    vector<TObjectOrder> &whichSelection ) : wxPGProperty(label, name)
{
  myTimeline = whichWindow;
  myWindowName = windowName;
  
  mySelectedRows.init( myTimeline->getTrace() );
  mySelectedRows.setSelected( whichSelection,
                              myTimeline->getTrace()->getLevelObjects( myTimeline->getLevel() ),
                              myTimeline->getLevel() );

  vector<TObjectOrder> tmpSelection( whichSelection );
  SelectionRowsUtils::getAllLevelsSelectedRows( myTimeline->getTrace(), 
                                                mySelectedRows,
                                                myTimeline->getLevel(),
                                                tmpSelection );
  wxString tmp;
  GetStringValueFromVector( tmpSelection, tmp );
  SetValue( tmp );
}


prvRowsSelectionProperty::~prvRowsSelectionProperty()
{
}


wxString prvRowsSelectionProperty::ValueToString( wxVariant & value, int argFlags ) const
{
  return value.GetString();
}

void prvRowsSelectionProperty::GetSelectionAsVector( TTraceLevel whichLevel,
                                                     vector<TObjectOrder> &levelSelections )
{
  mySelectedRows.getSelected( levelSelections, whichLevel );
}


bool prvRowsSelectionProperty::OnEvent( wxPropertyGrid* propgrid,
                                        wxWindow* WXUNUSED(primary),
                                        wxEvent& event )
{
  if ( propgrid->IsMainButtonEvent(event) )
  {
    bool parentIsGtimeline = false;
    RowsSelectionDialog *dialog = new RowsSelectionDialog( (wxWindow *)propgrid,
                                                           myTimeline,
                                                           &mySelectedRows,
                                                           ID_ROWSSELECTIONDIALOG,
                                                           myWindowName,
                                                           parentIsGtimeline );
    wxparaverApp::mainWindow->SetOpenedPropertyDialog( dialog );
    
    if ( dialog->ShowModal() == wxID_OK )
    {
      wxArrayInt tmpArray;
      vector<TObjectOrder> tmpSelection;
      
      dialog->GetSelections( myTimeline->getLevel(), tmpArray );
      for( unsigned int idx = 0; idx < tmpArray.GetCount(); idx++ )
        tmpSelection.push_back( tmpArray[ idx ] );

      SelectionRowsUtils::getAllLevelsSelectedRows( myTimeline->getTrace(), 
                                                    mySelectedRows,
                                                    myTimeline->getLevel(),
                                                    tmpSelection );
      wxString tmp;
      GetStringValueFromVector( tmpSelection, tmp );
      SetValueInEvent( tmp );
    }
  
    wxparaverApp::mainWindow->SetOpenedPropertyDialog( nullptr );
    delete dialog;
  }
  
  return true;
}


void prvRowsSelectionProperty::GetStringValueFromVector( vector<TObjectOrder> &whichSelection,
                                                         wxString &onString )
{
  for (  vector<TObjectOrder>::iterator it = whichSelection.begin(); it != whichSelection.end(); ++it )
  {
    if ( it != whichSelection.begin() ) 
    {
      onString += _(", ");
    }

    if( myTimeline->getLevel() == TTraceLevel::CPU )
      onString += wxString::FromUTF8( LabelConstructor::objectLabel( *it + 1,
                                                                      myTimeline->getLevel(),
                                                                      myTimeline->getTrace(),
                                                                      false ).c_str() );
    else
      onString += wxString::FromUTF8( LabelConstructor::objectLabel( *it,
                                                                      myTimeline->getLevel(),
                                                                      myTimeline->getTrace(),
                                                                      false ).c_str() );
  }
}


/**********************************************************
 **       prvNumbersListProperty
 **********************************************************/

#if wxMAJOR_VERSION >= 3 && wxMINOR_VERSION >= 1
wxPG_IMPLEMENT_PROPERTY_CLASS( prvNumbersListProperty, wxPGProperty,
                               TextCtrl )
#else
WX_PG_IMPLEMENT_PROPERTY_CLASS( prvNumbersListProperty, wxPGProperty,
                                wxArrayInt, const wxArrayInt&, TextCtrl )
#endif

prvNumbersListProperty::prvNumbersListProperty( const wxString& label,
                                                const wxString& name,
                                                const wxArrayString& value )
                                                  : wxPGProperty(label,name)
{
  // Get local chars for decimal and thousand separators
  // Actually having problems to read numbers with thousands sep like 60.000.001, so not allowed
  locale mylocale( "" );
  char decimalChar =  use_facet< numpunct< char > >( mylocale ).decimal_point();

  wxString allowedChars[] = { _("0"), _("1"), _("2"), _("3"), _("4"),
                              _("5"), _("6"), _("7"), _("8"), _("9"),
                              _(","),
                              _(";"),
                              _("-") };
  if ( decimalChar == '.' )
  {
    allowedChars[ 10 ] = _(".");
  }

  // Set numeric validator
  wxTextValidator myValidator( (long int)wxFILTER_INCLUDE_CHAR_LIST );
  wxArrayString charIncludes( (size_t)13, allowedChars );
  myValidator.SetIncludes( charIncludes );
  SetValidator( myValidator );

  SetValue( value );
}


prvNumbersListProperty::~prvNumbersListProperty()
{
}


void prvNumbersListProperty::OnSetValue()
{
  GenerateValueAsString();
}

wxString prvNumbersListProperty::ValueToString( wxVariant & value, int argFlags ) const
{
  return value.GetString();
}


void prvNumbersListProperty::GenerateValueAsString()
{
  wxString &tempStr = m_display;
  tempStr = GetValue().GetString();
}


bool prvNumbersListProperty::StringToValue( wxVariant& variant, 
                                            const wxString& text,
                                            int ) const
{
  wxArrayString arr;
  bool error = false;

  WX_PG_TOKENIZER1_BEGIN(text,wxT(';'))
    arr.Add(token);
  WX_PG_TOKENIZER1_END()

  if( !( wxString( GetLabel() ).Trim( false ).IsSameAs( wxT( "Translation List" ) ) ) )
  {
    // Order values
    double tmpValue;

    map< double, wxString > sortedValues;
    for( unsigned int i = 0; i < arr.GetCount(); ++i )
    {
      if ( arr[i].ToDouble( &tmpValue ) )
        sortedValues[ tmpValue ] = arr[ i ];
      else
      {
        sortedValues[ 0 ] = arr[ i ];
        error = true;
      }
    }

    wxArrayString tmpArr;
    for( map< double, wxString >::iterator it = sortedValues.begin(); it != sortedValues.end(); ++it )
    {
      tmpArr.Add( (*it).second );
    }

    arr = tmpArr;
  }
  else
  {
    // Order values
    double tmpValue;

    vector< double > values;
    vector< wxString > labels;
    for( unsigned int i = 0; i < arr.GetCount(); ++i )
    {
      if( arr[i].ToDouble( &tmpValue ) )
      {
        values.push_back( tmpValue );
        labels.push_back( arr[i] );
      }
      else
      {
        values.push_back( 0 );
        labels.push_back( arr[i] );
        error = true;
      }
    }

    wxArrayString tmpArr;
    for( vector< wxString >::iterator it = labels.begin(); it != labels.end(); ++it )
    {
      tmpArr.Add( (*it) );
    }

    arr = tmpArr;
  }

  wxVariant v( WXVARIANT(arr) );
  variant = v;

  if( error )
  {
    wxMessageDialog tmpDialog( wxparaverApp::mainWindow, wxT( "Invalid parameters." ), wxT( " Warning " ), wxOK|wxICON_EXCLAMATION );
    tmpDialog.ShowModal();
  }
  
  return !error;
}


bool prvNumbersListProperty::OnEvent( wxPropertyGrid* propgrid,
                                      wxWindow* WXUNUSED(primary),
                                      wxEvent& event )
{
  return false;
}


/**********************************************************
 **        prvTimelineTreeProperty
 **********************************************************/
#if wxMAJOR_VERSION >= 3 && wxMINOR_VERSION >= 1
wxPG_IMPLEMENT_PROPERTY_CLASS( prvTimelineTreeProperty, wxPGProperty,
                               TextCtrlAndButton )
#else
WX_PG_IMPLEMENT_PROPERTY_CLASS( prvTimelineTreeProperty, wxPGProperty,
                                wxString, wxString&, TextCtrlAndButton )
#endif

constexpr wxWindowID ID_TIMELINETREE = 10001;

prvTimelineTreeProperty::prvTimelineTreeProperty( const wxString& label,
                                                  const wxString& name,
                                                  Timeline *currentWindow,
                                                  const Trace *currentTrace,
                                                  bool needNoneElement,
                                                  std::vector<TWindowID> windows )
                                                    : wxPGProperty(label,name), 
                                                      myWindows( windows ),
                                                      myCurrentTrace( currentTrace )
{
  wxString valueStr;
  if( currentWindow == nullptr )
    valueStr = wxT( "None" );
  else
    valueStr = wxString( currentWindow->getName().c_str(), wxConvUTF8 );

  SetValue( valueStr );
  selectedWindow = currentWindow;
  myNeedNoneElement = needNoneElement;
}

prvTimelineTreeProperty::~prvTimelineTreeProperty()
{
}

bool prvTimelineTreeProperty::OnEvent( wxPropertyGrid* propgrid,
                                       wxWindow* WXUNUSED(primary),
                                       wxEvent& event )
{
  if( propgrid->IsMainButtonEvent(event) )
  {
    TimelineTreeSelector timelineSelector( wxparaverApp::mainWindow,
                                           ID_TIMELINETREE,
                                           GetLabel(),
                                           myWindows,
                                           selectedWindow,
                                           myCurrentTrace,
                                           myNeedNoneElement );
    
    wxparaverApp::mainWindow->SetOpenedPropertyDialog( &timelineSelector );

    timelineSelector.Move( wxGetMousePosition() );
    
    int retCode = timelineSelector.ShowModal();
    if( retCode == wxID_OK )
    {
      if( selectedWindow == timelineSelector.getSelection() )
      {
        wxparaverApp::mainWindow->SetOpenedPropertyDialog( nullptr );
        return false;
      }
      selectedWindow = timelineSelector.getSelection();
      if( selectedWindow != nullptr )
        SetValueInEvent( wxString( selectedWindow->getName().c_str(), wxConvUTF8 ) );
      else
        SetValueInEvent( wxT( "None" ) );
    }
    else
    {
      wxparaverApp::mainWindow->SetOpenedPropertyDialog( nullptr );
      return false;
    }    
  }

  wxparaverApp::mainWindow->SetOpenedPropertyDialog( nullptr );
  return true;
}

wxString prvTimelineTreeProperty::ValueToString( wxVariant & value, int argFlags ) const
{
  return value.GetString();
}

Timeline *prvTimelineTreeProperty::getSelectedWindow() const
{
  return selectedWindow;
}
