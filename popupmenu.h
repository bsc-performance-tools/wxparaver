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

    void Enable( const string tag );
    void Enable( const string tag, bool enable );

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
