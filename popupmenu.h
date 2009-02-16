#ifndef _POPUPMENU_H_ 
#define _POPUPMENU_H_

#include <wx/menu.h>
#include <wx/choicdlg.h>
#include <map>
using namespace std;

enum ItemType {ITEMNORMAL, ITEMRADIO, ITEMCHECK};

class gTimeline;
class gHistogram;

class gPopUpMenu
{
  public:
    gPopUpMenu( gTimeline *whichTimeline );
    gPopUpMenu( gHistogram *whichHistogram );
    ~gPopUpMenu();

    void Enable( const string tag, bool checkPaste );
    void Enable( const string tag, bool checkPaste, bool enable );

    void enableMenu( gTimeline *whichTimeline );
    void enableMenu( gHistogram *whichHistogram );

    wxMenu *GetPopUpMenu() { return popUpMenu; };
//    void Check( const string tag, bool checked );
    string GetOption( int position );
    bool OkPressed( );
    wxArrayInt GetSelections();


  private:
    gTimeline *timeline;
    gHistogram *histogram;
  
    wxMenu * popUpMenu;
    wxMenu * popUpMenuColor;
    wxMenu * popUpMenuPaste;
    wxMenu * popUpMenuPasteFilter;

    wxArrayString choices;
    wxMultiChoiceDialog * popUpMenuPasteDialog;


    void BuildItem( wxMenu *popUp,
                    const wxString &title,
                    wxObjectEventFunction handler,
                    ItemType itemType,
                    bool checked = false );
};

#endif // _POPUPMENU_H_
