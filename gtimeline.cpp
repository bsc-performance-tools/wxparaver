/////////////////////////////////////////////////////////////////////////////
// Name:        gtimeline.cpp
// Purpose:     
// Author:      Eloy Martinez
// Modified by: 
// Created:     Wed 27 Aug 2008 11:26:03 CEST
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstopclo
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes
#include <wx/dcbuffer.h>

#include "gtimeline.h"
#include "window.h"
#include "labelconstructor.h"
#include "drawmode.h"
#include "zoomhistory.cpp"
#include "loadedwindows.h"

#define wxTEST_GRAPHICS 1

#if wxTEST_GRAPHICS
#include "wx/graphics.h"
#if wxUSE_GRAPHICS_CONTEXT == 0
#undef wxTEST_GRAPHICS
#define wxTEST_GRAPHICS 0
#endif
#else
#undef wxUSE_GRAPHICS_CONTEXT
#define wxUSE_GRAPHICS_CONTEXT 0
#endif

////@begin XPM images
////@end XPM images


/*!
 * gTimeline type definition
 */

IMPLEMENT_CLASS( gTimeline, wxFrame )


/*!
 * gTimeline event table definition
 */

BEGIN_EVENT_TABLE( gTimeline, wxFrame )

////@begin gTimeline event table entries
  EVT_CLOSE( gTimeline::OnCloseWindow )
  EVT_SIZE( gTimeline::OnSize )
  EVT_IDLE( gTimeline::OnIdle )
  EVT_RIGHT_DOWN( gTimeline::OnRightDown )

  EVT_UPDATE_UI( ID_SCROLLEDWINDOW, gTimeline::OnScrolledwindowUpdate )

////@end gTimeline event table entries

END_EVENT_TABLE()


/*!
 * gTimeline constructors
 */
//class paraverMain;
#include "paravermain.h"
gTimeline::gTimeline()
{
  Init();
  parent = NULL;
}

gTimeline::gTimeline( wxWindow* whichParent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( whichParent, id, caption, pos, size, style );
  parent = whichParent;
}


/*!
 * gTimeline creator
 */

bool gTimeline::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin gTimeline creation
  wxFrame::Create( parent, id, caption, pos, size, style );

  CreateControls();
////@end gTimeline creation
  return true;
}


/*!
 * gTimeline destructor
 */

gTimeline::~gTimeline()
{
////@begin gTimeline destruction
////@end gTimeline destruction
  delete zoomHistory;
}


/*!
 * Member initialisation
 */

void gTimeline::Init()
{
////@begin gTimeline member initialisation
  ready = false;
  myWindow = NULL;
  objectHeight = 1;
  zooming = false;
  drawZone = NULL;
////@end gTimeline member initialisation

  zoomXY = false;
  bufferImage.Create( 1, 1 );
  objectFont = wxFont( 7, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL );
  timeFont = wxFont( 6, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL );

//  popUpMenu = NULL;  // initialized on Right Click Down.
  zoomHistory = new ZoomHistory<TTime,TObjectOrder>();
}


/*!
 * Control creation for gTimeline
 */

void gTimeline::CreateControls()
{    
////@begin gTimeline content construction
  gTimeline* itemFrame1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemFrame1->SetSizer(itemBoxSizer2);

  drawZone = new wxScrolledWindow( itemFrame1, ID_SCROLLEDWINDOW, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxFULL_REPAINT_ON_RESIZE|wxHSCROLL|wxVSCROLL );
  itemBoxSizer2->Add(drawZone, 1, wxGROW|wxALL, 1);
  drawZone->SetScrollbars(1, 1, 0, 0);

  // Connect events and objects
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_PAINT, wxPaintEventHandler(gTimeline::OnPaint), NULL, this);
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(gTimeline::OnEraseBackground), NULL, this);
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_LEFT_DOWN, wxMouseEventHandler(gTimeline::OnLeftDown), NULL, this);
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_LEFT_UP, wxMouseEventHandler(gTimeline::OnLeftUp), NULL, this);
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_RIGHT_DOWN, wxMouseEventHandler(gTimeline::OnRightDown), NULL, this);
  drawZone->Connect(ID_SCROLLEDWINDOW, wxEVT_MOTION, wxMouseEventHandler(gTimeline::OnMotion), NULL, this);
////@end gTimeline content construction
}


/*!
 * Should we show tooltips?
 */

bool gTimeline::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap gTimeline::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin gTimeline bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
////@end gTimeline bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon gTimeline::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin gTimeline icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end gTimeline icon retrieval
}


void gTimeline::redraw()
{
  wxString winTitle = GetTitle();
  SetTitle( winTitle + _(" (Working...)") );
  if ( zoomHistory->isEmpty() )
    zoomHistory->addZoom( myWindow->getWindowBeginTime(), myWindow->getWindowEndTime(),
                          0, myWindow->getWindowLevelObjects() - 1 );
  ready = false;
  bufferImage.Create( drawZone->GetSize().GetWidth(), drawZone->GetSize().GetHeight() );
  drawImage.Create( drawZone->GetSize().GetWidth(), drawZone->GetSize().GetHeight() );
  commImage.Create( drawZone->GetSize().GetWidth(), drawZone->GetSize().GetHeight() );
  wxMemoryDC bufferDraw( bufferImage );
  wxMemoryDC commdc( commImage );
  commdc.SetBackgroundMode( wxTRANSPARENT );
  commdc.SetBackground( *wxTRANSPARENT_BRUSH );
  commdc.Clear();
  wxBitmap commMask;
  commMask.Create( drawZone->GetSize().GetWidth(), drawZone->GetSize().GetHeight(), 1 );
  wxMemoryDC maskdc( commMask );
  maskdc.SetBackground( *wxBLACK_BRUSH );
  maskdc.SetPen( wxPen( wxColour( 255, 255, 255 ), 1 ) );
  maskdc.Clear();
  
  bufferDraw.SetBackground( wxBrush( *wxBLACK_BRUSH ) );
  bufferDraw.Clear();
  drawAxis( bufferDraw );
  myWindow->init( myWindow->getWindowBeginTime(), CREATECOMMS );
  
  TObjectOrder minObj = zoomHistory->getSecondDimension().first;
  TObjectOrder maxObj = zoomHistory->getSecondDimension().second;
  
  for( TObjectOrder obj = minObj; obj <= maxObj; ++obj )
  {
    TObjectOrder firstObj = obj;
    TObjectOrder lastObj = firstObj;
    while( ( lastObj + 1 ) <= maxObj && objectPosList[ lastObj + 1 ] == objectPosList[ firstObj ] )
      ++lastObj;
    obj = lastObj;
    drawRow( bufferDraw, commdc, maskdc, firstObj, lastObj );
  }
  bufferDraw.SelectObject(wxNullBitmap);
  bufferDraw.SelectObject( drawImage );
  bufferDraw.DrawBitmap( bufferImage, 0, 0, false );
  maskdc.SetPen( *wxBLACK_PEN );
  maskdc.DrawRectangle( 0, 0, objectAxisPos + 1, drawZone->GetSize().GetHeight() );
  maskdc.DrawRectangle( drawZone->GetSize().GetWidth() - drawBorder, 0, drawBorder, drawZone->GetSize().GetHeight() );
  wxMask *mask = new wxMask( commMask );
  commImage.SetMask( mask );
  if( myWindow->getDrawCommLines() )
    bufferDraw.DrawBitmap( commImage, 0, 0, true );
  
  ready = true;
  SetTitle( winTitle );
}


void gTimeline::drawAxis( wxDC& dc )
{
  dc.SetPen( wxPen( *wxWHITE, 1 ) );
  dc.SetTextForeground( *wxWHITE );
  
  // Get the text extent for time label
  dc.SetFont( timeFont );
  wxSize timeExt = dc.GetTextExtent( LabelConstructor::timeLabel( myWindow->getWindowBeginTime(), myWindow->getTimeUnit() ) );
  timeAxisPos = dc.GetSize().GetHeight() - ( drawBorder + timeExt.GetHeight() + drawBorder );

  // Compute number of objects
  TObjectOrder minObj = zoomHistory->getSecondDimension().first;
  TObjectOrder maxObj = zoomHistory->getSecondDimension().second;
  
  // Get the text extent for the last object (probably the larger one)
  dc.SetFont( objectFont );
  // +1!
  wxSize objectExt = dc.GetTextExtent( LabelConstructor::objectLabel( maxObj - minObj, myWindow->getLevel(), 
                                                                     myWindow->getTrace() ) );
  objectAxisPos = drawBorder + objectExt.GetWidth() + drawBorder;
  
  // Draw axis lines
  dc.DrawLine( objectAxisPos, drawBorder, 
               objectAxisPos, timeAxisPos );
  dc.DrawLine( objectAxisPos, timeAxisPos,
               dc.GetSize().GetWidth() - drawBorder, timeAxisPos );

  // Draw axis labels
  wxCoord y;
  double inc = (double)( timeAxisPos - drawBorder ) / (double)( maxObj - minObj + 1 );
  
  objectPosList.clear();
  objectHeight = 1;
  for( TObjectOrder obj = minObj; obj <= maxObj; obj++ )
  {
    y = ( (wxCoord) ( inc * ( obj - minObj ) ) ) + drawBorder;
    if( ( inc * 0.25 ) >= 1.0 )
    {
      if( obj > minObj ) objectHeight < ( y - objectPosList[ obj - minObj - 1 ] ) * 0.75 ? 
                          objectHeight = ( y - objectPosList[ obj - minObj - 1 ] ) * 0.75 :
                          objectHeight = objectHeight;
      y += (wxCoord)( inc * 0.25 );
    }
    else
    {
      if( obj > minObj ) objectHeight < ( y - objectPosList[ obj - minObj - 1 ] ) ? 
                          objectHeight = ( y - objectPosList[ obj - minObj - 1 ] ) :
                          objectHeight = objectHeight;
    }
    objectPosList.push_back( y );
    dc.DrawText( LabelConstructor::objectLabel( obj, myWindow->getLevel(), myWindow->getTrace() ),
                 drawBorder, y );
  }

  dc.SetFont( timeFont );
  dc.DrawText( LabelConstructor::timeLabel( myWindow->getWindowBeginTime(), myWindow->getTimeUnit() ),
               objectAxisPos, timeAxisPos + drawBorder );
  dc.DrawText( LabelConstructor::timeLabel( myWindow->getWindowEndTime(), myWindow->getTimeUnit() ),
               dc.GetSize().GetWidth() -
               ( dc.GetTextExtent( LabelConstructor::timeLabel( myWindow->getWindowEndTime(), myWindow->getTimeUnit() ) )
               .GetWidth() + drawBorder ),
               timeAxisPos + drawBorder );
}


void gTimeline::drawRow( wxDC& dc, wxMemoryDC& commdc, wxDC& maskdc, TObjectOrder firstRow, TObjectOrder lastRow )
{
  TTime timeStep = ( myWindow->getWindowEndTime() - myWindow->getWindowBeginTime() ) /
                   ( dc.GetSize().GetWidth() - objectAxisPos - drawBorder );
  vector<TSemanticValue> timeValues;
  vector<TSemanticValue> rowValues;
  wxCoord timePos = objectAxisPos + 1;
  TObjectOrder minObj = zoomHistory->getSecondDimension().first;
  wxCoord objectPos = objectPosList[firstRow - minObj];
  
  for( TTime currentTime = myWindow->getWindowBeginTime() + timeStep; 
       currentTime <= myWindow->getWindowEndTime(); 
       currentTime += timeStep )
  {
    rowValues.clear();

    for( TObjectOrder row = firstRow; row <= lastRow; ++row )
    {
      timeValues.clear();
      
      while( myWindow->getEndTime( row ) <= currentTime - timeStep )
        myWindow->calcNext( row );
      
      timeValues.push_back( myWindow->getValue( row ) );
      RecordList *rl = myWindow->getRecordList( row );
      if( rl != NULL )
        drawComm( commdc, maskdc, rl, currentTime - timeStep, currentTime, timeStep, timePos );
      while( myWindow->getEndTime( row ) < currentTime )
      {
        myWindow->calcNext( row );
        timeValues.push_back( myWindow->getValue( row ) );
        rl = myWindow->getRecordList( row );
        if( rl != NULL )
          drawComm( commdc, maskdc, rl, currentTime - timeStep, currentTime, timeStep, timePos );
      }
    
      rowValues.push_back( DrawMode::selectValue( timeValues, myWindow->getDrawModeTime() ) );
    }
    TSemanticValue valueToDraw = DrawMode::selectValue( rowValues, myWindow->getDrawModeObject() );
    rgb colorToDraw = myWindow->calcColor( valueToDraw, *myWindow );
    dc.SetPen( wxPen( wxColour( colorToDraw.red, colorToDraw.green, colorToDraw.blue ) ) );
    if( objectPos + objectHeight < timeAxisPos )
      dc.DrawLine( timePos, objectPos, timePos, objectPos + objectHeight );
    else
      dc.DrawLine( timePos, objectPos, timePos, timeAxisPos - 1 );
    timePos++;
  }
}


void gTimeline::drawComm( wxMemoryDC& commdc, wxDC& maskdc, RecordList *comms, TTime from, TTime to, TTime step, wxCoord pos )
{
  TObjectOrder minObj = zoomHistory->getSecondDimension().first;
  TObjectOrder maxObj = zoomHistory->getSecondDimension().second;
  RecordList::iterator it = comms->begin();
  step = ( 1 / step );
  while( it != comms->end() && it->getTime() < from )
    ++it;
  while( it != comms->end() && it->getTime() <= to )
  {
    if( it->getCommPartnerObject() >= minObj && it->getCommPartnerObject() <= maxObj &&
        ( it->getType() & RECV ||
          ( it->getType() & SEND && it->getCommPartnerTime() > myWindow->getWindowEndTime() ) )
      )
    {
      if( it->getType() & LOG )
        commdc.SetPen( wxPen( wxColour( 255, 255, 0 ) ) );
      else if( it->getType() & PHY )
        commdc.SetPen( *wxRED_PEN );
      wxCoord posPartner = (wxCoord)( ( it->getCommPartnerTime() - myWindow->getWindowBeginTime() ) * step );
      posPartner += objectAxisPos;
      commdc.DrawLine( posPartner, objectPosList[it->getCommPartnerObject() - minObj],
                   pos, objectPosList[it->getOrder() - minObj] );
      maskdc.DrawLine( posPartner, objectPosList[it->getCommPartnerObject() - minObj],
                       pos, objectPosList[it->getOrder() - minObj] );
    }
    ++it;
  }
  comms->erase( comms->begin(), it );
}


/*!
 * wxEVT_ERASE_BACKGROUND event handler for ID_SCROLLEDWINDOW
 */
void gTimeline::OnEraseBackground( wxEraseEvent& event )
{
  //event.Skip();
}


/*!
 * wxEVT_PAINT event handler for ID_SCROLLEDWINDOW
 */
void gTimeline::OnPaint( wxPaintEvent& event )
{
  wxPaintDC dc( drawZone );

  if( ready )
    dc.DrawBitmap( drawImage, 0, 0, false );
}


/*!
 * wxEVT_SIZE event handler for ID_GTIMELINE
 */
void gTimeline::OnSize( wxSizeEvent& event )
{
  if( ready )
    redraw();
  event.Skip();
}


/*!
 * wxEVT_IDLE event handler for ID_GTIMELINE
 */
void gTimeline::OnIdle( wxIdleEvent& event )
{
  this->SetTitle( myWindow->getName() );
  
  if( myWindow->getShowWindow() )
  {
    this->Show();
    if( !ready )
      redraw();
  }
  else
    this->Show( false );

  myWindow->setPosX( this->GetPosition().x );
  myWindow->setPosY( this->GetPosition().y );
  myWindow->setWidth( this->GetSize().GetWidth() );
  myWindow->setHeight( this->GetSize().GetHeight() );
}


/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_GTIMELINE
 */
void gTimeline::OnCloseWindow( wxCloseEvent& event )
{
  myWindow->setShowWindow( false );
}


/*!
 * wxEVT_LEFT_DOWN event handler for ID_SCROLLEDWINDOW
 */
void gTimeline::OnLeftDown( wxMouseEvent& event )
{
  zooming = true;
  zoomBeginX = event.GetX();
  zoomBeginY = event.GetY();
}


/*!
 * wxEVT_LEFT_UP event handler for ID_SCROLLEDWINDOW
 */
void gTimeline::OnLeftUp( wxMouseEvent& event )
{
  wxMemoryDC dc( bufferImage );
  zoomEndX = event.GetX();
  zoomEndY = event.GetY();
  zoomXY = event.ControlDown();

  if( ready && ( zoomBeginX != zoomEndX || zoomBeginY != zoomEndY ))
  {
    // TIME zoom limits
    if( zoomEndX < zoomBeginX )
    {
      long tmp = zoomEndX; zoomEndX = zoomBeginX; zoomBeginX = tmp;
    }
    if( zoomBeginX < objectAxisPos )
      zoomBeginX = 0;
    else
      zoomBeginX -= objectAxisPos;
      
    if( zoomEndX > dc.GetSize().GetWidth() - drawBorder )
      zoomEndX = dc.GetSize().GetWidth() - drawBorder;
    zoomEndX -= objectAxisPos;

    // Detect begin and end TIME
    TTime timeStep = ( myWindow->getWindowEndTime() - myWindow->getWindowBeginTime() ) /
                     ( dc.GetSize().GetWidth() - objectAxisPos - drawBorder );

    TTime endTime = ( timeStep * zoomEndX ) + myWindow->getWindowBeginTime();
    TTime beginTime = ( timeStep * zoomBeginX ) + myWindow->getWindowBeginTime();

    // ROW zoom limits
    TObjectOrder beginRow = zoomHistory->getSecondDimension().first;
    TObjectOrder endRow = zoomHistory->getSecondDimension().second;
    TObjectOrder minObj = 0;

    if( zoomXY )
    {
      if( zoomEndY < zoomBeginY )
      {
        long tmp = zoomEndY; zoomEndY = zoomBeginY; zoomBeginY = tmp;
      }
      if( zoomBeginY > timeAxisPos )
        zoomBeginY = timeAxisPos - 1;
      if( zoomBeginY < drawBorder )
        zoomBeginY = drawBorder;

      if( zoomEndY > timeAxisPos )
        zoomEndY = timeAxisPos - 1;
      if( zoomEndY < drawBorder )
        zoomEndY = drawBorder;

      TObjectOrder numObjects = zoomHistory->getSecondDimension().second - zoomHistory->getSecondDimension().first + 1;

      double heightPerRow = (double)( timeAxisPos - drawBorder - 1 ) / (double)numObjects;
      beginRow = TObjectOrder( floor( (zoomBeginY - drawBorder - 1) / heightPerRow ) );
      endRow = TObjectOrder( floor( (zoomEndY - drawBorder - 1) / heightPerRow ) );
  
      if( endRow > numObjects )
        endRow = numObjects - 1;
      minObj = zoomHistory->getSecondDimension().first;
    }

    zoomHistory->addZoom( beginTime, endTime, beginRow + minObj, endRow + minObj );

    // Update window properties
    myWindow->setWindowBeginTime( beginTime );
    myWindow->setWindowEndTime( endTime );
    myWindow->setRedraw( true );
    myWindow->setChanged( true );
  }

  zooming = false;
  zoomXY = false;
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_SCROLLEDWINDOW
 */
void gTimeline::OnScrolledwindowUpdate( wxUpdateUIEvent& event )
{
  if( this->IsShown() )
  {
    if( myWindow->getRedraw() )
    {
      myWindow->setRedraw( false );
      redraw();
      drawZone->Refresh();
    }
  }
}


void gTimeline::OnPopUpCopy()
{
  gPasteWindowProperties::pasteWindowProperties->getInstance()->copy( this );
}


// simple windows can let this method do the entire work passing a NULL clonedWindow
// derived windows must pass existing clonedWindow, because Window::clone is recursive
gTimeline *gTimeline::clone( Window *clonedWindow,
                             wxWindow *parent,
                             wxTreeItemId idRoot1, 
                             wxTreeItemId idRoot2 )
{
  if ( clonedWindow == NULL )
    clonedWindow = myWindow->clone(); // recursive clone

  // Create empty gTimeline and assign window with same dimensions.
  // Shifts position right and down.
  wxSize titleBarSize = GetSize() - GetClientSize();
  if ( titleBarSize.GetHeight() == 0 )
    titleBarSize = paraverMain::defaultTitleBarSize;
  wxPoint position =  wxPoint( GetPosition().x + titleBarSize.GetHeight(),
                               GetPosition().y + titleBarSize.GetHeight() );
  wxSize size = wxSize( clonedWindow->getWidth(), clonedWindow->getHeight() );

  gTimeline *clonedTimeline = new gTimeline( parent, wxID_ANY, wxT( myWindow->getName().c_str() ), position, size );
  clonedTimeline->SetMyWindow( clonedWindow );

  // clone zoom history
/*  delete clonedTimeline->zoomHistory;
  clonedTimeline->zoomHistory = zoomHistory->clone();*/

  // add to loaded windows list
  LoadedWindows::getInstance()->add( clonedWindow );

  wxChoicebook *choiceWindowBrowser = paraverMain::myParaverMain->choiceWindowBrowser;
  INT16 currentTrace = paraverMain::myParaverMain->GetCurrentTrace();
  wxTreeCtrl *allTracesPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 ); // Global page
  wxTreeCtrl *currentPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( currentTrace + 1 ); // Current page

  TreeBrowserItemData *currentData =  new TreeBrowserItemData( clonedWindow->getName(), clonedTimeline );
  wxTreeItemId currentWindowId1 = allTracesPage->AppendItem( idRoot1, clonedWindow->getName(), 1, -1, currentData );
  wxTreeItemId currentWindowId2 = currentPage->AppendItem( idRoot2, clonedWindow->getName(), 1, -1, new TreeBrowserItemData( *currentData ) );

  if( myWindow->getShowWindow() )
  {
    clonedTimeline->Show();
    clonedTimeline->redraw();
  }
  else
    clonedTimeline->Show(false);

  // if derived, clone parents
  if ( clonedWindow->isDerivedWindow() )
  {
//    vector< gTimeline * > gParents;
//    getParentGTimeline( this, gParents );

    clone( clonedWindow->getParent( 0 ), parent, currentWindowId1, currentWindowId2 );
    clone( clonedWindow->getParent( 1 ), parent, currentWindowId1, currentWindowId2 );
  }

  return clonedTimeline;
}


void gTimeline::OnPopUpClone()
{
  clone( NULL, parent, getAllTracesTree()->GetRootItem(), getSelectedTraceTree()->GetRootItem());
}


void gTimeline::OnPopUpFitTimeScale()
{
  myWindow->setWindowBeginTime( 0 );
  myWindow->setWindowEndTime( myWindow->getTrace()->getEndTime() );
  zoomHistory->addZoom( 0, myWindow->getTrace()->getEndTime() );
  myWindow->setRedraw( true );
  myWindow->setChanged( true );
}


void gTimeline::OnPopUpFitSemanticScale()
{
  // problem! computes for all the rows!!
  myWindow->computeYScale();
  myWindow->setRedraw( true );
  myWindow->setChanged( true );
}


void gTimeline::OnPopUpCodeColor()
{
  myWindow->setCodeColorMode();
  myWindow->setRedraw( true );
}


void gTimeline::OnPopUpPasteTime()
{
  gPasteWindowProperties* pasteActions = gPasteWindowProperties::pasteWindowProperties->getInstance();
  pasteActions->paste( this, "Time" );
  zoomHistory->addZoom( pasteActions->getBeginTime(), pasteActions->getEndTime() );
  myWindow->setRedraw( true );
  myWindow->setChanged( true );
}


void gTimeline::OnPopUpPasteObjects()
{
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Objects" );
  myWindow->setRedraw( true );
}


void gTimeline::OnPopUpPasteSize()
{
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Size" );
  myWindow->setRedraw( true );
}


void gTimeline::OnPopUpPasteFilterAll()
{
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Events" );
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Communications" );
  myWindow->setRedraw( true );
  myWindow->setChanged( true );
}


void gTimeline::OnPopUpPasteFilterCommunications()
{
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Communications" );
  myWindow->setRedraw( true );
  myWindow->setChanged( true );
}


void gTimeline::OnPopUpPasteFilterEvents()
{
  gPasteWindowProperties::pasteWindowProperties->getInstance()->paste( this, "Events" );
  myWindow->setRedraw( true );
  myWindow->setChanged( true );
}


void gTimeline::OnPopUpPasteSpecial()
{
  wxArrayString choices;
  
  wxMultiChoiceDialog *dialog = gPopUpMenu::createPasteSpecialDialog( choices, this );

  if ( dialog->ShowModal() == wxID_OK )
  {
    wxArrayInt selections = dialog->GetSelections();
    if ( selections.GetCount() > 0 )
    {
      for ( size_t i = 0; i < selections.GetCount(); i++ )
      {
        gPasteWindowProperties* pasteActions = gPasteWindowProperties::pasteWindowProperties->getInstance();
        if ( pasteActions->isAllowed( this, gPopUpMenu::getOption( choices, selections[i] ) ) )
          pasteActions->paste( this, gPopUpMenu::getOption( choices, selections[i] ) );
      }

      myWindow->setRedraw( true );
      myWindow->setChanged( true );
    }
  }

  delete dialog;
}


void gTimeline::OnPopUpRowSelection()
{
  wxArrayString choices;
  
  wxMultiChoiceDialog *dialog = gPopUpMenu::createRowSelectionDialog( choices, this );

  if ( dialog->ShowModal() == wxID_OK )
  {
    wxArrayInt selections = dialog->GetSelections();
    if ( selections.GetCount() > 0 )
    {
      vector< bool > selected;
      myWindow->getSelectedRows( selected );
      selected.assign( selected.size(), false );

      vector< TObjectOrder > selectedSet;
      myWindow->getSelectedRowSet( selectedSet );

      for ( vector< TObjectOrder >::iterator row = selectedSet.begin(); row != selectedSet.end(); row++ )
        selected[ *row ] = true;

      myWindow->setSelectedRows( selected );

      myWindow->setRedraw( true );
      myWindow->setChanged( true );
    }
  }

  delete dialog;
}


void gTimeline::OnPopUpGradientColor()
{
  myWindow->setGradientColorMode();
  myWindow->getGradientColor().allowOutOfScale( true );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpNotNullGradientColor()
{
  myWindow->setGradientColorMode();
  myWindow->getGradientColor().allowOutOfScale( false );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeTimeLast()
{
  myWindow->setDrawModeTime( DRAW_LAST );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeTimeRandom()
{
  myWindow->setDrawModeTime( DRAW_RANDOM );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeTimeRandomNotZero()
{
  myWindow->setDrawModeTime( DRAW_RANDNOTZERO );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeTimeMaximum()
{
  myWindow->setDrawModeTime( DRAW_MAXIMUM );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeTimeMinimumNotZero()
{
  myWindow->setDrawModeTime( DRAW_MINNOTZERO );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeTimeAverage()
{
  myWindow->setDrawModeTime( DRAW_AVERAGE );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeObjectsLast()
{
  myWindow->setDrawModeObject( DRAW_LAST );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeObjectsRandom()
{
  myWindow->setDrawModeObject( DRAW_RANDOM );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeObjectsRandomNotZero()
{
  myWindow->setDrawModeObject( DRAW_RANDNOTZERO );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeObjectsMaximum()
{
  myWindow->setDrawModeObject( DRAW_MAXIMUM );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeObjectsMinimumNotZero()
{
  myWindow->setDrawModeObject( DRAW_MINNOTZERO );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeObjectsAverage()
{
  myWindow->setDrawModeObject( DRAW_AVERAGE );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeBothLast()
{
  myWindow->setDrawModeObject( DRAW_LAST );
  myWindow->setDrawModeTime( DRAW_LAST );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeBothRandom()
{
  myWindow->setDrawModeObject( DRAW_RANDOM );
  myWindow->setDrawModeTime( DRAW_RANDOM );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeBothRandomNotZero()
{
  myWindow->setDrawModeObject( DRAW_RANDNOTZERO );
  myWindow->setDrawModeTime( DRAW_RANDNOTZERO );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeBothMaximum()
{
  myWindow->setDrawModeObject( DRAW_MAXIMUM );
  myWindow->setDrawModeTime( DRAW_MAXIMUM );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeBothMinimumNotZero()
{
  myWindow->setDrawModeObject( DRAW_MINNOTZERO );
  myWindow->setDrawModeTime( DRAW_MINNOTZERO );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpDrawModeBothAverage()
{
  myWindow->setDrawModeObject( DRAW_AVERAGE );
  myWindow->setDrawModeTime( DRAW_AVERAGE );
  myWindow->setRedraw( true );
}

void gTimeline::OnPopUpUndoZoom()
{
  if ( !zoomHistory->emptyPrevZoom() )
  {
    zoomHistory->prevZoom();
    myWindow->setWindowBeginTime( zoomHistory->getFirstDimension().first );
    myWindow->setWindowEndTime( zoomHistory->getFirstDimension().second );
    myWindow->setRedraw( true );
    myWindow->setChanged( true );
  }
}


void gTimeline::OnPopUpRedoZoom()
{
  if ( !zoomHistory->emptyNextZoom() )
  {
    zoomHistory->nextZoom();
    myWindow->setWindowBeginTime( zoomHistory->getFirstDimension().first );
    myWindow->setWindowEndTime( zoomHistory->getFirstDimension().second );
    myWindow->setRedraw( true );
    myWindow->setChanged( true );
  }
}


void gTimeline::rightDownManager()
{
  gPopUpMenu popUpMenu( this );
  
  popUpMenu.enable( "Undo Zoom", !zoomHistory->emptyPrevZoom() );
  popUpMenu.enable( "Redo Zoom", !zoomHistory->emptyNextZoom() );

  popUpMenu.enableMenu( this );
  PopupMenu( &popUpMenu );
}


/*!
 * wxEVT_RIGHT_DOWN event handler for ID_SCROLLEDWINDOW
 */
void gTimeline::OnRightDown( wxMouseEvent& event )
{
  rightDownManager();
}


/*!
 * wxEVT_MOTION event handler for ID_SCROLLEDWINDOW
 */
void gTimeline::OnMotion( wxMouseEvent& event )
{
  if( !zooming )
    return;

  zoomXY = event.ControlDown();

  wxMemoryDC memdc( drawImage );
  memdc.SetBackgroundMode( wxTRANSPARENT );
  memdc.SetBackground( *wxTRANSPARENT_BRUSH );
  memdc.Clear();
#ifdef __WXGTK__
  wxGCDC dc( memdc );
  dc.SetBrush( wxBrush( wxColour( 255, 255, 255, 80 ) ) );
#else
  wxDC& dc = memdc;
  dc.SetBrush( *wxTRANSPARENT_BRUSH );
#endif
  dc.SetPen( *wxWHITE_PEN );

  long beginX = zoomBeginX > event.GetX() ? event.GetX() : zoomBeginX;
  long beginY = drawBorder;
  long endX = zoomBeginX < event.GetX() ? event.GetX() : zoomBeginX;
  if( beginX < objectAxisPos )
    beginX = objectAxisPos;
  if( endX > drawImage.GetWidth() - drawBorder )
    endX = drawImage.GetWidth() - drawBorder;
  wxCoord width = endX - beginX;
  wxCoord height = timeAxisPos - drawBorder + 1;

  if ( zoomXY )
  {
    beginY = zoomBeginY > event.GetY() ? event.GetY() : zoomBeginY;
    long endY = zoomBeginY < event.GetY() ? event.GetY() : zoomBeginY;
    if( beginY > timeAxisPos )
      beginY = timeAxisPos;
    if( endY > timeAxisPos )
      endY = timeAxisPos;
    if( beginY < drawBorder )
      beginY = drawBorder;
    if( endY < drawBorder )
      endY = drawBorder;
    height = endY - beginY;
  }

  dc.DrawBitmap( bufferImage, 0, 0, false );
  if( myWindow->getDrawCommLines() )
    dc.DrawBitmap( commImage, 0, 0, true );
  dc.DrawRectangle( beginX, beginY, width, height );

  drawZone->Refresh();
}
