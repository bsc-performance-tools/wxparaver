#ifndef _POPUPMENU_H_ 
#define _POPUPMENU_H_

#include <wx/menu.h>
#include <wx/choicdlg.h>
#include <map>
using namespace std;

enum ItemType {ITEMNORMAL, ITEMRADIO, ITEMCHECK};

class gTimeline;
class gHistogram;

#define ID_MENU_COPY            30000
#define ID_MENU_TIME            30001
#define ID_MENU_OBJECTS         30002
#define ID_MENU_SIZE            30003
#define ID_MENU_FILTER_ALL      30004
#define ID_MENU_FILTER_COMMS    30005
#define ID_MENU_FILTER_EVENTS   30006
#define ID_MENU_PASTE_SPECIAL   30007
#define ID_MENU_CLONE           30008
#define ID_MENU_UNDO_ZOOM       30009
#define ID_MENU_REDO_ZOOM       30010
#define ID_MENU_FIT_TIME        30011
#define ID_MENU_FIT_SEMANTIC    30012
#define ID_MENU_CODE_COLOR      30013
#define ID_MENU_GRADIENT_COLOR  30014
#define ID_MENU_NOT_NULL_GRADIENT_COLOR  30015

class gPopUpMenu : public wxMenu
{
  DECLARE_EVENT_TABLE()

  public:
    gPopUpMenu() 
    {
    }
    
    gPopUpMenu( gTimeline *whichTimeline );
    gPopUpMenu( gHistogram *whichHistogram );
    virtual ~gPopUpMenu();

    void enablePaste( const string tag, bool checkPaste );
    void enable( const string tag, bool enable );
    void enable( const string tag );
    void disable( const string tag );

    void enableMenu( gTimeline *whichTimeline );
    void enableMenu( gHistogram *whichHistogram );

    static wxMultiChoiceDialog *createPasteSpecialDialog( wxArrayString& choices, gHistogram *whichHistogram );
    static wxMultiChoiceDialog *createPasteSpecialDialog( wxArrayString& choices, gTimeline * whichTimeline );
    static string getOption( wxArrayString& choices, int position );

    void OnMenuCopy( wxCommandEvent& event);
    void OnMenuTime( wxCommandEvent& event);
    void OnMenuObjects( wxCommandEvent& event);
    void OnMenuSize( wxCommandEvent& event);
    void OnMenuFilterAll( wxCommandEvent& event);
    void OnMenuFilterComms( wxCommandEvent& event);
    void OnMenuFilterEvents( wxCommandEvent& event);
    void OnMenuPasteSpecial( wxCommandEvent& event);
    void OnMenuClone( wxCommandEvent& event);
    void OnMenuUndoZoom( wxCommandEvent& event);
    void OnMenuRedoZoom( wxCommandEvent& event);
    void OnMenuFitTime( wxCommandEvent& event);
    void OnMenuFitSemantic( wxCommandEvent& event);
    void OnMenuCodeColor( wxCommandEvent& event);
    void OnMenuGradientColor( wxCommandEvent& event);
    void OnMenuNotNullGradientColor( wxCommandEvent& event);

  private:
    gTimeline *timeline;
    gHistogram *histogram;

    wxMenu * popUpMenuColor;
    wxMenu * popUpMenuPaste;
    wxMenu * popUpMenuPasteFilter;

    void buildItem( wxMenu *popUp,
                    const wxString &title,
                    ItemType itemType,
                    wxObjectEventFunction handler,
                    wxWindowID id,
                    bool checked = false );
};

#endif // _POPUPMENU_H_
