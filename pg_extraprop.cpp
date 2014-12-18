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

#include <wx/choicdlg.h>
#include "pg_extraprop.h"
#include "window.h"
#include "selectionrowsutils.h"
#include "labelconstructor.h"

#include "filter.h"
#include "eventsselectiondialog.h"
#include <wx/event.h>

/**********************************************************
 **       prvEventTypeProperty
 **********************************************************/

WX_PG_IMPLEMENT_PROPERTY_CLASS( prvEventTypeProperty, wxPGProperty,
                                wxArrayInt, const wxArrayInt&, TextCtrlAndButton )

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

wxString prvEventTypeProperty::GetValueAsString( int ) const
{
  return m_display;
}

void prvEventTypeProperty::GenerateValueAsString()
{
    wxString &tempStr = m_display;
    tempStr = GetValue().GetString();
}

wxArrayInt prvEventTypeProperty::GetValueAsIndices() const
{
    const wxArrayInt& valueArr = wxArrayIntFromVariant(GetValue());
    unsigned int i;

    // Translate values to string indices.
    wxArrayInt selections;

    if ( !m_choices.IsOk() || !m_choices.GetCount() || !(&valueArr) )
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
        PrepareValueForDialogEditing(propgrid);

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
                                 choiceCount?&labels[0]:NULL,
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

#if wxMAJOR_VERSION<3
int prvEventTypeProperty::GetChoiceInfo( wxPGChoiceInfo* choiceinfo )
{
    if ( choiceinfo )
        choiceinfo->m_choices = &m_choices;
    return -1;
}
#endif

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

WX_PG_IMPLEMENT_PROPERTY_CLASS( prvEventInfoProperty, wxPGProperty,
                                wxArrayInt, const wxArrayInt&, TextCtrlAndButton )

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
                                            const wxPGChoices& choices,
                                            Window *whichWindow )
                                              : wxPGProperty( label, name )
{
  m_choices.Assign( choices );

  currentWindow = whichWindow;

  wxArrayString tmpArray;

  if( GetName().Cmp( _("Event type.Types") ) == 0 || GetName().Cmp( _("Types") ) == 0 )
  {
    vector<TEventType> typesSel;
    currentWindow->getFilter()->getEventType( typesSel );
    for( vector<TEventType>::iterator it = typesSel.begin(); it != typesSel.end(); ++it )
    {
      tmpArray.Add( wxString().Format( _( "%d" ), (*it ) ) );
    }
  }
  else if( GetName().Cmp( _("Event value.Values") ) == 0 || GetName().Cmp( _("Values") ) == 0 )
  {
    vector<TEventValue> valuesSel;
    currentWindow->getFilter()->getEventValue( valuesSel );
    for( vector<TEventValue>::iterator it = valuesSel.begin(); it != valuesSel.end(); ++it )
    {
      tmpArray.Add( wxString().Format( _( "%d" ), (*it ) ) );
    }
  }

  SetValue( tmpArray );
}


prvEventInfoProperty::prvEventInfoProperty( const wxString& label,
                                            const wxString& name,
                                            const wxArrayInt& value)
                                              : wxPGProperty(label,name)
{
  wxArrayString strings;
  m_choices.Set( strings );

  wxArrayString tmpArray;
  for ( unsigned int i = 0; i < value.GetCount(); ++i )
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


wxString prvEventInfoProperty::GetValueAsString( int ) const
{
  return m_display;
}


void prvEventInfoProperty::GenerateValueAsString()
{
  wxString &tempStr = m_display;
  tempStr = GetValue().GetString();
}


wxArrayInt prvEventInfoProperty::GetValueAsIndices() const
{
  const wxArrayInt& valueArr = wxArrayIntFromVariant( GetValue() );
  unsigned int i;

  // Translate values to string indices.
  wxArrayInt selections;

  if ( !m_choices.IsOk() || !m_choices.GetCount() || !( &valueArr ) )
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
    PrepareValueForDialogEditing( propgrid );

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

    eventsDialog.Move( propgrid->GetGoodEditorDialogPosition( this, eventsDialog.GetSize() ) );

    if ( eventsDialog.ShowModal() == wxID_OK && numLabels )
    {
//      int userStringMode = GetAttributeAsLong( wxT( "UserStringMode" ), 0 );

//      wxArrayInt arrInt = eventsDialog.GetEventTypesSelections();

//      wxVariant variant;

      // Strings that were not in list of choices
//      wxArrayString value;

      // Translate string indices to strings
//      if ( userStringMode == 1 )
//      {
//        for ( unsigned int n = 0; n < unknownEventTypes.size(); ++n )
//          value.push_back( unknownEventTypes[ n ] );
//      }

//      for ( unsigned int i = 0; i < arrInt.GetCount(); ++i )
//        value.Add( wxString() << m_choices.GetValue( arrInt.Item( i ) ) );

//      if ( userStringMode == 2 )
//      {
//        for ( unsigned int n = 0; n < unknownEventTypes.size(); ++n )
//          value.push_back( unknownEventTypes[ n ] );
//      }

//      value.Sort();

//      variant = WXVARIANT( value );

      if ( eventsDialog.ChangedEventTypesFunction() )
      {
        wxVariant tmpVal =  WXVARIANT( eventsDialog.GetIndexEventTypesFunction() ); 
        propgrid->GetProperty( _("Event type.TypeFunction") )->SetValue( tmpVal );
        currentWindow->getFilter()->setEventTypeFunction( eventsDialog.GetNameEventTypesFunction() );
      }

      if ( eventsDialog.ChangedEventValuesFunction() )
      {
        wxVariant tmpVal =  WXVARIANT( eventsDialog.GetIndexEventValuesFunction() ); 
        propgrid->GetProperty( _("Event value.ValueFunction") )->SetValue( tmpVal );
        currentWindow->getFilter()->setEventValueFunction( eventsDialog.GetNameEventValuesFunction() );

/*        bool disable = ( auxname.compare( "All" ) == 0 || auxname.compare( "None" ) == 0 );
        propgrid->GetProperty( _("Event value.Values") )->ChangeFlag( wxPG_PROP_DISABLED, disable );*/
      }

      if ( eventsDialog.ChangedOperatorTypeValue() )
      {
        int func = eventsDialog.GetIndexOperatorTypeValue();
        wxVariant tmpVal =  WXVARIANT( func ); 
        propgrid->GetProperty( _("TypeValueOp") )->SetValue( tmpVal );

        if ( func == 0 )
          currentWindow->getFilter()->setOpTypeValueAnd();
        else 
          currentWindow->getFilter()->setOpTypeValueOr();
      }

      if ( eventsDialog.ChangedEventValues() )
      {
        wxArrayString tmpStrEventValues;
        wxArrayInt tmpEventValues = eventsDialog.GetEventValues();

        Filter *filter = currentWindow->getFilter();
        filter->clearEventValues();

        for( unsigned int i = 0; i < tmpEventValues.GetCount(); ++i )
        {
          wxString tmpVal;
          tmpVal << tmpEventValues[ i ];
          tmpStrEventValues.Add( tmpVal );

          filter->insertEventValue( (TEventValue)tmpEventValues[ i ] );
        }

        wxVariant tmpVar( WXVARIANT( tmpStrEventValues ));

        if ( GetName().Cmp( _("Event value.Values") ) != 0 )
        {
          propgrid->GetProperty( _("Event value.Values") )->SetValue( tmpVar );
          // Try leave the control enabled
          //string auxname;
          //int func = eventsDialog.GetEventValuesFunction( auxname );
/*          bool disable = ( auxname.compare( "All" ) == 0 || auxname.compare( "None" ) == 0 );
          propgrid->GetProperty( _("Event value.Values") )->ChangeFlag( wxPG_PROP_DISABLED, disable );*/
        }
        else
          SetValueInEvent( tmpVar );
      }

      if ( eventsDialog.ChangedEventTypesSelection() )
      {
        wxArrayString tmpStrEventTypes;
        wxArrayInt tmpEventTypes = eventsDialog.GetEventTypesSelection();

        Filter *filter = currentWindow->getFilter();
        filter->clearEventTypes();

        for( unsigned int i = 0; i < tmpEventTypes.GetCount(); ++i )
        {
          wxString tmpVal;
          tmpVal << tmpEventTypes[ i ];
          tmpStrEventTypes.Add( tmpVal );
          filter->insertEventType( (TEventType)tmpEventTypes[ i ] );
        }

        wxVariant tmpVar( WXVARIANT( tmpStrEventTypes ));

        if ( GetName().Cmp( _("Event type.Types") ) != 0 )
          propgrid->GetProperty( _("Event type.Types") )->SetValue( tmpVar );
        else
          SetValueInEvent( tmpVar );
      }
/*
      SetValueInEvent( variant );
*/
      currentWindow->setRedraw( true );
      currentWindow->setChanged( true );
      return true;
    }
  }

  return false;
}

#if wxMAJOR_VERSION<3
int prvEventInfoProperty::GetChoiceInfo( wxPGChoiceInfo* choiceinfo )
{
    if ( choiceinfo )
        choiceinfo->m_choices = &m_choices;
    return -1;
}
#endif

bool prvEventInfoProperty::StringToValue( wxVariant& variant, const wxString& text, int ) const
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
      wxString tmpStr( wxString::FromAscii( (*it).c_str() ) );
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
                            NULL,
                            this );
    Append( i, wxString::FromAscii( levels[ i ].c_str() ), subMenus[ i ] );
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
    if( ( lastItem = (*it)->FindItem( currentItemID ) ) != NULL )
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


WX_PG_IMPLEMENT_PROPERTY_CLASS( prvSemanticThreadProperty, wxPGProperty,
                                wxString, wxString&, TextCtrlAndButton )

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

wxString prvSemanticThreadProperty::GetValueAsString( int ) const
{
  return GetValue().GetString();
}


/**********************************************************
 **       prvRowsSelectionProperty
 **********************************************************/

WX_PG_IMPLEMENT_PROPERTY_CLASS( prvRowsSelectionProperty, wxPGProperty,
                                wxString, wxString&, TextCtrlAndButton )

prvRowsSelectionProperty::prvRowsSelectionProperty( wxPropertyGrid *propgrid,
                                                    Window *whichWindow,
                                                    const wxString &windowName,
                                                    vector<TObjectOrder> &whichSelection,
                                                    const wxString &label,
                                                    const wxString &name,
                                                    const wxString &value ) : wxPGProperty(label, name)
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



wxString 	prvRowsSelectionProperty::GetValueAsString ( int ) const
{
  return GetValue().GetString();
}


void prvRowsSelectionProperty::GetSelectionAsVector( TWindowLevel whichLevel,
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
    RowsSelectionDialog *dialog = new RowsSelectionDialog( (wxWindow *)propgrid,
                                                           myTimeline,
                                                           &mySelectedRows,
                                                           ID_ROWSSELECTIONDIALOG,
                                                           myWindowName );

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

    if( myTimeline->getLevel() == CPU )
      onString += wxString::FromAscii( LabelConstructor::objectLabel( *it + 1,
                                                                      myTimeline->getLevel(),
                                                                      myTimeline->getTrace(),
                                                                      false ).c_str() );
    else
      onString += wxString::FromAscii( LabelConstructor::objectLabel( *it,
                                                                      myTimeline->getLevel(),
                                                                      myTimeline->getTrace(),
                                                                      false ).c_str() );
  }
}


/**********************************************************
 **       prvNumbersListProperty
 **********************************************************/

WX_PG_IMPLEMENT_PROPERTY_CLASS( prvNumbersListProperty, wxPGProperty,
                                wxArrayInt, const wxArrayInt&, TextCtrl )

prvNumbersListProperty::prvNumbersListProperty( const wxString& label,
                                                const wxString& name,
                                                const wxArrayString& value )
                                                  : wxPGProperty(label,name)
{
  // Get local chars for decimal and thousand separators
  // Actually having problems to read numbers with thousands sep like 60.000.001, so not allowed
  
  locale mylocale( "" );
  char decimalChar =  use_facet< numpunct< char > >( mylocale ).decimal_point();
// use_facet< numpunct< char > >( mylocale ).thousands_sep() can return the same char !?!

/*  
  char thousandsSepChar;

  // Some locales are both the same!!
  if ( decimalChar == ',' )
    thousandsSepChar = '.';
  else
    thousandsSepChar = ',';
*/

  wxString allowedChars[] = { _("0"), _("1"), _("2"), _("3"), _("4"),
                              _("5"), _("6"), _("7"), _("8"), _("9"),
//                             wxT( decimalChar ), wxT( thousandsSepChar ),
                              _(","),
                              _(";") };
  if ( decimalChar == '.' )
  {
    allowedChars[ 10 ] = _(".");
  }

  // Set numeric validator
  wxTextValidator myValidator( (long int)wxFILTER_INCLUDE_CHAR_LIST );
//  wxArrayString charIncludes( (size_t)13, allowedChars );
  wxArrayString charIncludes( (size_t)12, allowedChars );
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


wxString prvNumbersListProperty::GetValueAsString( int ) const
{
  return m_display;
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

  WX_PG_TOKENIZER1_BEGIN(text,wxT(';'))
    arr.Add(token);
  WX_PG_TOKENIZER1_END()
  
  // Order values
  double tmpValue;

  map< double, wxString > sortedValues;
  for ( unsigned int i = 0; i < arr.GetCount(); ++i )
  {
    if ( arr[i].ToDouble( &tmpValue ))  // invalid values not used
    {
      sortedValues[ tmpValue ] = arr[i];
    }
  }
  
  wxArrayString tmpArr;
  for ( map< double, wxString >::iterator it = sortedValues.begin(); it != sortedValues.end(); ++it )
  {
    tmpArr.Add( (*it).second );
  }

  arr = tmpArr;
  
  wxVariant v( WXVARIANT(arr) );
  variant = v;

  return true;
}


bool prvNumbersListProperty::OnEvent( wxPropertyGrid* propgrid,
                                      wxWindow* WXUNUSED(primary),
                                      wxEvent& event )
{
  return true;
}
