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


#include <vector>
#include <string>

#include <wx/propgrid/propgrid.h>
#include <wx/version.h>
#if wxMAJOR_VERSION>=3
#include <wx/propgrid/property.h>
#else
#include <wx/propgrid/propdev.h>
#endif
#include "loadedwindows.h"

class Timeline;

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
#if wxMAJOR_VERSION<3
    virtual wxString GetValueAsString( int flags = 0 ) const;
#else
    virtual wxString ValueToString( wxVariant & value, int argFlags = 0 ) const;
#endif
    virtual bool StringToValue( wxVariant& variant, const wxString& text, int argFlags = 0 ) const;
#if wxMAJOR_VERSION<3
    WX_PG_DECLARE_EVENT_METHODS()

    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
#else
    bool OnEvent( wxPropertyGrid* propgrid,
                  wxWindow* WXUNUSED(primary),
                  wxEvent& event );
#endif

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
 **       prvEventInfoProperty
 **********************************************************/
class Filter;

enum class prvEventInfoType{ TYPES = 0, VALUES };

class prvEventInfoProperty: public wxPGProperty
{
  WX_PG_DECLARE_PROPERTY_CLASS( prvEventInfoProperty )
  public:

    prvEventInfoProperty( const wxString& label,
                          const wxString& name,
                          const wxPGChoices& choices,
                          Timeline *whichWindow,
                          prvEventInfoType whichInfoType  );

    prvEventInfoProperty( const wxString& label,
                          const wxString& name,
                          const wxArrayString& strings,
                          const wxArrayInt& value = wxArrayInt() );
#ifndef SWIG
    prvEventInfoProperty( const wxString& label,
                          const wxString& name,
                          const wxPGChoices& choices,
                          const Filter *whichFilter );

    prvEventInfoProperty( const wxString& label = wxPG_LABEL,
                          const wxString& name = wxPG_LABEL,
                          const wxArrayInt& value = wxArrayInt() );
#endif
    virtual ~prvEventInfoProperty();

    virtual void OnSetValue();
#if wxMAJOR_VERSION<3
    virtual wxString GetValueAsString( int flags = 0 ) const;
#else
    virtual wxString ValueToString( wxVariant & value, int argFlags = 0 ) const;
#endif
    virtual bool StringToValue( wxVariant& variant, const wxString& text, int argFlags = 0 ) const;
#if wxMAJOR_VERSION<3
    WX_PG_DECLARE_EVENT_METHODS()

    virtual int GetChoiceInfo( wxPGChoiceInfo* choiceinfo );
#else
    bool OnEvent( wxPropertyGrid* propgrid,
                  wxWindow* WXUNUSED(primary),
                  wxEvent& event );
#endif
    wxArrayInt GetValueAsArrayInt() const;

  protected:

    void GenerateValueAsString();

    // Returns translation of values into string indices.
    wxArrayInt GetValueAsIndices() const;

    wxArrayString       m_valueAsStrings;  // Value as array of strings

    wxPGChoices         m_choices;

    wxString            m_display; // Cache displayed text since generating it is relatively complicated.
    
  private:
    Timeline *currentWindow;
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
    SemanticMenu( const std::vector<std::string>& levels,
                  const std::vector<std::vector<std::string> >& functions,
                  const wxString& value,
                  prvSemanticThreadProperty *prop );
    virtual ~SemanticMenu();
    
    void OnMenu( wxCommandEvent& event );
    void PopupMenu( wxWindow *onWindow );
    
  private:
    prvSemanticThreadProperty *myProperty;
    std::vector<wxMenu *> subMenus;
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
                               const std::vector<std::string>& levels,
                               const std::vector<std::vector<std::string> >& choices,
                               const wxString& value );
    virtual ~prvSemanticThreadProperty();

#if wxMAJOR_VERSION<3
    virtual wxString GetValueAsString( int ) const;
#else
    virtual wxString ValueToString( wxVariant & value, int argFlags = 0 ) const;
#endif

#if wxMAJOR_VERSION<3
    WX_PG_DECLARE_EVENT_METHODS()
#else
    bool OnEvent( wxPropertyGrid* propgrid,
                  wxWindow* WXUNUSED(primary),
                  wxEvent& event );
#endif
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
                              Timeline *whichWindow,
                              const wxString &windowName,
                              std::vector<TObjectOrder> &whichSelection,
                              const wxString &label,
                              const wxString &name,
                              const wxString &value = wxEmptyString );
                               
    virtual ~prvRowsSelectionProperty();

#if wxMAJOR_VERSION<3
    virtual wxString GetValueAsString ( int ) const;
#else
    virtual wxString ValueToString( wxVariant & value, int argFlags = 0 ) const;
#endif

    void GetSelectionAsVector( TWindowLevel whichLevel,
                               std::vector<TObjectOrder> &levelSelections );
                               
#if wxMAJOR_VERSION<3
    WX_PG_DECLARE_EVENT_METHODS()
#else
    bool OnEvent( wxPropertyGrid* propgrid,
                  wxWindow* WXUNUSED(primary),
                  wxEvent& event );
#endif
  private:
    Timeline *myTimeline;
    wxString myWindowName;
    SelectionManagement< TObjectOrder, TWindowLevel > mySelectedRows;

    void GetStringValueFromVector( std::vector<TObjectOrder> &whichSelection,
                                   wxString &onString );
};


/**********************************************************
 **        prvNumbersListProperty
 **********************************************************/
class prvNumbersListProperty: public wxPGProperty
{
  WX_PG_DECLARE_PROPERTY_CLASS( prvNumbersListProperty )
  public:
    prvNumbersListProperty()
    {}

    prvNumbersListProperty( const wxString& label,
                            const wxString& name,
                            const wxArrayString& value );

    virtual ~prvNumbersListProperty();

    virtual void OnSetValue();
#if wxMAJOR_VERSION<3
    virtual wxString GetValueAsString( int flags = 0 ) const;
#else
    virtual wxString ValueToString( wxVariant & value, int argFlags = 0 ) const;
#endif
    virtual bool StringToValue( wxVariant& variant, const wxString& text, int argFlags = 0 ) const;

#if wxMAJOR_VERSION<3
    WX_PG_DECLARE_EVENT_METHODS()
#else
    bool OnEvent( wxPropertyGrid* propgrid,
                  wxWindow* WXUNUSED(primary),
                  wxEvent& event );

#endif

  protected:

    void GenerateValueAsString();

    wxArrayString       m_valueAsStrings;  // Value as array of strings

    wxString            m_display; // Cache displayed text since generating it is relatively complicated.
};


/**********************************************************
 **        prvTimelineTreeProperty
 **********************************************************/
class prvTimelineTreeProperty: public wxPGProperty
{
  WX_PG_DECLARE_PROPERTY_CLASS( prvTimelineTreeProperty )
  public:
    prvTimelineTreeProperty()
    {}
    
    prvTimelineTreeProperty( const wxString& label,
                             const wxString& name,
                             const wxString& value,
                             std::vector<TWindowID> windows,
                             Timeline *currentWindow,
                             const Trace *currentTrace,
                             bool needNoneElement = false );
    virtual ~prvTimelineTreeProperty();

#if wxMAJOR_VERSION<3
    virtual wxString GetValueAsString( int ) const;
#else
    virtual wxString ValueToString( wxVariant & value, int argFlags = 0 ) const;
#endif

#if wxMAJOR_VERSION<3
    WX_PG_DECLARE_EVENT_METHODS()
#else
    bool OnEvent( wxPropertyGrid* propgrid,
                  wxWindow* WXUNUSED(primary),
                  wxEvent& event );
#endif
    Timeline *getSelectedWindow() const;

  private:
    std::vector<TWindowID> myWindows;
    const Trace *myCurrentTrace;
    Timeline *selectedWindow;
    bool myNeedNoneElement;
};
