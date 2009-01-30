#include "window.h"
#include <map>
using namespace std;

class gPasteWindowProperties
{
  public:
    static gPasteWindowProperties *pasteWindowProperties;

    ~gPasteWindowProperties();
   
    static gPasteWindowProperties *getInstance();
    
    void setTime( TTime bTime, TTime eTime );
    TTime getBeginTime( ) const { return beginTime; }
    TTime getEndTime( ) const { return endTime; }

    void setRow( TObjectOrder bRow, TObjectOrder eRow );
    TObjectOrder getBeginRow( ) const { return beginRow; }
    TObjectOrder getEndRow( ) const { return endRow; }

    bool allowPaste( const string tag );

  private:
    gPasteWindowProperties();
    
    // ?? static gPasteWindowProperties *pasteWindowProperties;
    
    // Common Window Properties
    map < const string, bool > active;
    TTime beginTime, endTime;
    TObjectOrder beginRow, endRow;
};

