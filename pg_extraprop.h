#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/propdev.h>
#include <vector>

using namespace std;

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

class SemanticMenu: public wxEvtHandler
{
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
    wxMenu myMenu;
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
