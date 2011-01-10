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

int prvEventTypeProperty::GetChoiceInfo( wxPGChoiceInfo* choiceinfo )
{
    if ( choiceinfo )
        choiceinfo->m_choices = &m_choices;
    return -1;
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
                                                    const wxString &label,
                                                    const wxString &name,
                                                    const wxString &value ) : wxPGProperty(label, name)
{
  vector< TObjectOrder > tmp;
 
  myTimeline = whichWindow;
  mySelectedRows.init( whichWindow->getTrace() );
  for ( PRV_UINT32 level = ( PRV_UINT32 )NONE; level <= ( PRV_UINT32 )CPU; ++level )
  {
    mySelectedRows.setSelected( tmp, ( TWindowLevel )level );
  }
}


prvRowsSelectionProperty::~prvRowsSelectionProperty()
{
}

wxString 	prvRowsSelectionProperty::GetValueAsString ( int ) const
{
  return _("");
}


bool prvRowsSelectionProperty::OnEvent( wxPropertyGrid* propgrid,
                                         wxWindow* WXUNUSED(primary),
                                         wxEvent& event )
{
  RowsSelectionDialog *dialog = new RowsSelectionDialog( (wxWindow *)propgrid,
                                                         myTimeline,
                                                         &mySelectedRows );

  if ( propgrid->IsMainButtonEvent(event) )
  {
    if ( dialog->ShowModal() == wxID_OK )
    {
      cout << "OK!!" << endl;
    }
  }
  
  return true;
}
