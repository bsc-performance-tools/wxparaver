#ifndef _POPUPMENU_H_ 
#define _POPUPMENU_H_

#include <wx/menu.h>
#include <wx/choicdlg.h>
#include <map>
using namespace std;

enum ItemType {ITEMNORMAL, ITEMRADIO, ITEMCHECK};

class gTimeline;
class gHistogram;

#define ID_MENU_COPY                              30000
#define ID_MENU_TIME                              30001
#define ID_MENU_OBJECTS                           30002
#define ID_MENU_SIZE                              30003
#define ID_MENU_FILTER_ALL                        30004
#define ID_MENU_FILTER_COMMS                      30005
#define ID_MENU_FILTER_EVENTS                     30006
#define ID_MENU_PASTE_SPECIAL                     30007
#define ID_MENU_CLONE                             30008
#define ID_MENU_UNDO_ZOOM                         30009
#define ID_MENU_REDO_ZOOM                         30010
#define ID_MENU_FIT_TIME                          30011
#define ID_MENU_FIT_SEMANTIC                      30012
#define ID_MENU_CODE_COLOR                        30013
#define ID_MENU_GRADIENT_COLOR                    30014
#define ID_MENU_NOT_NULL_GRADIENT_COLOR           30015
#define ID_MENU_DRAWMODE_TIME_LAST                30016
#define ID_MENU_DRAWMODE_TIME_MAXIMUM             30017
#define ID_MENU_DRAWMODE_TIME_MINIMUM_NOT_ZERO    30018
#define ID_MENU_DRAWMODE_TIME_RANDOM              30019
#define ID_MENU_DRAWMODE_TIME_RANDOM_NOT_ZERO     30020
#define ID_MENU_DRAWMODE_TIME_AVERAGE             30021
#define ID_MENU_DRAWMODE_OBJECTS_LAST             30022
#define ID_MENU_DRAWMODE_OBJECTS_MAXIMUM          30023
#define ID_MENU_DRAWMODE_OBJECTS_MINIMUM_NOT_ZERO 30024
#define ID_MENU_DRAWMODE_OBJECTS_RANDOM           30025
#define ID_MENU_DRAWMODE_OBJECTS_RANDOM_NOT_ZERO  30026
#define ID_MENU_DRAWMODE_OBJECTS_AVERAGE          30027
#define ID_MENU_DRAWMODE_BOTH_LAST                30028
#define ID_MENU_DRAWMODE_BOTH_MAXIMUM             30029
#define ID_MENU_DRAWMODE_BOTH_MINIMUM_NOT_ZERO    30030
#define ID_MENU_DRAWMODE_BOTH_RANDOM              30031
#define ID_MENU_DRAWMODE_BOTH_RANDOM_NOT_ZERO     30032
#define ID_MENU_DRAWMODE_BOTH_AVERAGE             30033
#define ID_MENU_ROW_SELECTION                     30034
#define ID_MENU_INFO_PANEL                        30035

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
    static wxMultiChoiceDialog *createRowSelectionDialog( wxArrayString& choices, gTimeline *whichTimeline );
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

    void OnMenuDrawModeTimeLast( wxCommandEvent& event);
    void OnMenuDrawModeTimeRandom( wxCommandEvent& event);
    void OnMenuDrawModeTimeRandomNotZero( wxCommandEvent& event);
    void OnMenuDrawModeTimeMaximum( wxCommandEvent& event);
    void OnMenuDrawModeTimeMinimumNotZero( wxCommandEvent& event);
    void OnMenuDrawModeTimeAverage( wxCommandEvent& event);

    void OnMenuDrawModeObjectsLast( wxCommandEvent& event);
    void OnMenuDrawModeObjectsRandom( wxCommandEvent& event);
    void OnMenuDrawModeObjectsRandomNotZero( wxCommandEvent& event);
    void OnMenuDrawModeObjectsMaximum( wxCommandEvent& event);
    void OnMenuDrawModeObjectsMinimumNotZero( wxCommandEvent& event);
    void OnMenuDrawModeObjectsAverage( wxCommandEvent& event);

    void OnMenuDrawModeBothLast( wxCommandEvent& event);
    void OnMenuDrawModeBothRandom( wxCommandEvent& event);
    void OnMenuDrawModeBothRandomNotZero( wxCommandEvent& event);
    void OnMenuDrawModeBothMaximum( wxCommandEvent& event);
    void OnMenuDrawModeBothMinimumNotZero( wxCommandEvent& event);
    void OnMenuDrawModeBothAverage( wxCommandEvent& event);

    void OnMenuRowSelection( wxCommandEvent& event);

    void OnMenuInfoPanel( wxCommandEvent& event );
  private:
    gTimeline *timeline;
    gHistogram *histogram;

    wxMenu * popUpMenuColor;
    wxMenu * popUpMenuPaste;
    wxMenu * popUpMenuPasteFilter;
    wxMenu * popUpMenuDrawMode;
    wxMenu * popUpMenuDrawModeTime;
    wxMenu * popUpMenuDrawModeObjects;
    wxMenu * popUpMenuDrawModeBoth;

    void buildItem( wxMenu *popUp,
                    const wxString &title,
                    ItemType itemType,
                    wxObjectEventFunction handler,
                    wxWindowID id,
                    bool checked = false );
};

#endif // _POPUPMENU_H_
