#include "copypaste.h"

gPasteWindowProperties* gPasteWindowProperties::pasteWindowProperties = NULL;

gPasteWindowProperties::gPasteWindowProperties()
{
  active["Time"] = false;
}

gPasteWindowProperties::~gPasteWindowProperties()
{
}

gPasteWindowProperties *gPasteWindowProperties::getInstance()
{
  if ( gPasteWindowProperties::pasteWindowProperties == NULL )
    gPasteWindowProperties::pasteWindowProperties = new gPasteWindowProperties();
  return gPasteWindowProperties::pasteWindowProperties;
}

void gPasteWindowProperties::setTime( TTime bTime, TTime eTime )
{
  beginTime = bTime;
  endTime = eTime;
  active["Time"] = true;
}

void gPasteWindowProperties::setRow( TObjectOrder bRow, TObjectOrder eRow )
{
  beginRow = bRow;
  endRow = eRow;
}

bool gPasteWindowProperties::allowPaste( const string tag )
{
  return active[tag];
}
