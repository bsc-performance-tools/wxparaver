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

#include <vector>
#include <string>

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/propdev.h>

#include "rowsselectiondialog.h"

using namespace std;

class Window;

/**********************************************************
 **       prvEventTypeProperty
 **********************************************************/
class prvEventTypeProperty: public wxPGProperty
{
  WX_PG_DECLARE_PROPERTY_CLASS( prvEventTypeProperty )
  public:

    prvEventTypeProperty( const wxString& label,
                          const wxString& name,
                          const wxArrayString& strings,
                          const wxArrayInt& value = wxArrayInt() );
#ifndef SWIG
    prvEventTypeProperty( const wxString& label,
                          const wxString& name,
                          const wxPGChoices& choices,
                          const wxArrayInt& value = wxArrayInt() );

    prvEventTypeProperty( const wxString& label = wxPG_LABEL,
                          const wxString& name = wxPG_LABEL,
                          const wxArrayInt& value = wxArrayInt() );
#endif
    virtual ~prvEventTypeProperty();

    virtual void OnSetValue();
    virtual wxString GetValueAsString( int flags = 0 ) const;
    virtual bool StringToValue( wxVariant& variant, const wxString& text, int argFlags = 0 ) const;
    WX_PG_DECLARE_EVENT_METHODS()

    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );

    wxArrayInt GetValueAsArrayInt() const;

  protected:

    void GenerateValueAsString();

    // Returns translation of values into string indices.
    wxArrayInt GetValueAsIndices() const;

    wxArrayString       m_valueAsStrings;  // Value as array of strings

    wxPGChoices         m_choices;

    wxString            m_display; // Cache displayed text since generating it is relatively complicated.
};


/**********************************************************
 **       prvSemanticThreadProperty
 **********************************************************/
class prvSemanticThreadProperty;

class SemanticMenu: public wxMenu
{
  DECLARE_EVENT_TABLE()

  public:
    SemanticMenu()
    {}
    SemanticMenu( const vector<string>& levels,
                  const vector<vector<string> >& functions,
                  const wxString& value,
                  prvSemanticThreadProperty *prop );
    virtual ~SemanticMenu();
    
    void OnMenu( wxCommandEvent& event );
    void PopupMenu( wxWindow *onWindow );
    
  private:
    prvSemanticThreadProperty *myProperty;
    vector<wxMenu *> subMenus;
    int currentItemID;
};

class prvSemanticThreadProperty: public wxPGProperty
{
  WX_PG_DECLARE_PROPERTY_CLASS( prvSemanticThreadProperty )
  public:
    prvSemanticThreadProperty()
    {}
    
    prvSemanticThreadProperty( const wxString& label,
                               const wxString& name,
                               const vector<string>& levels,
                               const vector<vector<string> >& choices,
                               const wxString& value );
    virtual ~prvSemanticThreadProperty();

    wxString GetValueAsString( int ) const;
    WX_PG_DECLARE_EVENT_METHODS()
  private:
    SemanticMenu *myMenu;
};

/**********************************************************
 **        prvRowsSelectionProperty
 **********************************************************/
class prvRowsSelectionProperty: public wxPGProperty
{
  WX_PG_DECLARE_PROPERTY_CLASS( prvRowsSelectionProperty )
  public:
    prvRowsSelectionProperty()
    {}

    prvRowsSelectionProperty( wxPropertyGrid *propgrid,
                              Window *whichWindow,
                              const wxString &label,
                              const wxString &name,
                              const wxString &value = wxEmptyString );
                               
    virtual ~prvRowsSelectionProperty();

    wxString GetValueAsString ( int ) const;
    WX_PG_DECLARE_EVENT_METHODS()
  private:
    Window *myTimeline;
    SelectionManagement< TObjectOrder, TWindowLevel > mySelectedRows;
};




