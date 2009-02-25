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

    void enablePaste( const string tag, bool checkPaste );
    void enable( const string tag, bool enable );
    void enable( const string tag );
    void disable( const string tag );

    void enableMenu( gTimeline *whichTimeline );
    void enableMenu( gHistogram *whichHistogram );

    wxMenu *getPopUpMenu() { return popUpMenu; };
//    void Check( const string tag, bool checked );
    string getOption( int position );
    bool okPressed( );
    wxArrayInt getSelections();

    void createPasteSpecialDialog( gTimeline * whichTimeline );
    void deletePasteSpecialDialog( );


  private:
    gTimeline *timeline;
    gHistogram *histogram;
  
    wxMenu * popUpMenu;
    wxMenu * popUpMenuColor;
    wxMenu * popUpMenuPaste;
    wxMenu * popUpMenuPasteFilter;

    wxArrayString choices;
    wxMultiChoiceDialog * popUpMenuPasteDialog;

    void buildItem( wxMenu *popUp,
                    const wxString &title,
                    wxObjectEventFunction handler,
                    ItemType itemType,
                    bool checked = false );
};

#endif // _POPUPMENU_H_
