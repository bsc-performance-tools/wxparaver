/////////////////////////////////////////////////////////////////////////////
// Name:        paravermain.cpp
// Purpose:     
// Author:      Eloy Martinez
// Modified by: 
// Created:     Wed 25 Jun 2008 15:58:35 CEST
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "wx/imaglist.h"
#include "paravermain.h"
#include "paraverkernelexception.h"
#include "cfg.h"
#include "window.h"
#include "histogram.h"
#include "gtimeline.h"
#include "ghistogram.h"
//#include <wx/propgrid/advprops.h>
#include "pg_extraprop.h"
#include "progresscontroller.h"
#include "loadedwindows.h"
#include "filter.h"

////@begin XPM images
////@end XPM images

#include "table.xpm"
#include "timeline.xpm"
/*!
 * paraverMain type definition
 */

IMPLEMENT_CLASS( paraverMain, wxFrame )


/*!
 * paraverMain event table definition
 */

BEGIN_EVENT_TABLE( paraverMain, wxFrame )

////@begin paraverMain event table entries
  EVT_IDLE( paraverMain::OnIdle )

  EVT_MENU( wxID_OPEN, paraverMain::OnOpenClick )

  EVT_UPDATE_UI( ID_RECENTTRACES, paraverMain::OnRecenttracesUpdate )

  EVT_MENU( ID_MENULOADCFG, paraverMain::OnMenuloadcfgClick )
  EVT_UPDATE_UI( ID_MENULOADCFG, paraverMain::OnMenuloadcfgUpdate )

  EVT_UPDATE_UI( ID_RECENTCFGS, paraverMain::OnMenuloadcfgUpdate )

  EVT_MENU( wxID_EXIT, paraverMain::OnExitClick )

  EVT_UPDATE_UI( ID_CHOICEWINBROWSER, paraverMain::OnChoicewinbrowserUpdate )

  EVT_UPDATE_UI( ID_FOREIGN, paraverMain::OnForeignUpdate )

////@end paraverMain event table entries

  EVT_TREE_SEL_CHANGED( wxID_ANY, paraverMain::OnTreeSelChanged )
  EVT_TREE_ITEM_ACTIVATED( wxID_ANY, paraverMain::OnTreeItemActivated )
  
  EVT_PG_CHANGED( ID_FOREIGN, paraverMain::OnPropertyGridChange )
END_EVENT_TABLE()


wxProgressDialog *paraverMain::dialogProgress = NULL;

/*!
 * paraverMain constructors
 */

paraverMain::paraverMain()
{
  Init();
}

paraverMain::paraverMain( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( parent, id, caption, pos, size, style );
}


/*!
 * paraverMain creator
 */

bool paraverMain::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  imageList = new wxImageList( 16, 16 );
  imageList->Add( wxIcon( table_xpm ) );
  imageList->Add( wxIcon( timeline_xpm ) );

  
////@begin paraverMain creation
  wxFrame::Create( parent, id, caption, pos, size, style );

  CreateControls();
////@end paraverMain creation

  LocalKernel::init();
  localKernel = new LocalKernel();
  paraverConfig = ParaverConfig::getInstance();
  paraverConfig->readParaverConfigFile();

  return true;
}


/*!
 * paraverMain destructor
 */

paraverMain::~paraverMain()
{
////@begin paraverMain destruction
  GetAuiManager().UnInit();
////@end paraverMain destruction

// DISCONNECTS
  wxMenuItem *tmpItem2 = menuFile->FindItem( ID_RECENTTRACES );
  wxMenu *menuTraces2 = tmpItem2->GetSubMenu();
  wxMenuItemList& menuItems2 = menuTraces2->GetMenuItems();
  for (wxMenuItemList::iterator menuIt = menuItems2.begin(); menuIt != menuItems2.end() ; ++menuIt )
  {
    wxMenuItem *tmp = *menuIt;
    int id = tmp->GetId();
    Disconnect( id, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&paraverMain::OnPreviousTracesClick );
  }

  wxMenuItem *tmpItem = menuFile->FindItem( ID_RECENTCFGS );
  wxMenu *menuCFGs = tmpItem->GetSubMenu();
  wxMenuItemList& menuItems = menuCFGs->GetMenuItems();
  for (wxMenuItemList::iterator menuIt = menuItems.begin(); menuIt != menuItems.end() ; ++menuIt )
  {
    wxMenuItem *tmp = *menuIt;
    int id = tmp->GetId();
    Disconnect( id, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&paraverMain::OnPreviousCFGsClick );
  }

  for( vector<Trace *>::iterator it = loadedTraces.begin(); it != loadedTraces.end(); ++it )
    delete *it;
  delete localKernel;
  
  delete imageList;
}


/*!
 * Member initialisation
 */

void paraverMain::Init()
{
////@begin paraverMain member initialisation
  currentTrace = -1;
  currentTimeline = NULL;
  currentHisto = NULL;
  previousTraces = PreviousFiles::createPreviousTraces();
  previousCFGs = PreviousFiles::createPreviousCFGs();
  lastTimeline = NULL;
  lastHisto = NULL;
  currentWindow = NULL;
  menuFile = NULL;
  menuHelp = NULL;
  tbarMain = NULL;
  choiceWindowBrowser = NULL;
  windowProperties = NULL;
////@end paraverMain member initialisation
}


/*!
 * Control creation for paraverMain
 */

void paraverMain::CreateControls()
{    
////@begin paraverMain content construction
  paraverMain* itemFrame1 = this;

  GetAuiManager().SetManagedWindow(this);

  wxMenuBar* menuBar = new wxMenuBar;
  menuFile = new wxMenu;
  menuFile->Append(wxID_OPEN, _("Load &Trace"), _T(""), wxITEM_NORMAL);
  wxMenu* itemMenu5 = new wxMenu;
  menuFile->Append(ID_RECENTTRACES, _("Previous Traces"), itemMenu5);
  menuFile->AppendSeparator();
  menuFile->Append(ID_MENULOADCFG, _("Load &Configuration"), _T(""), wxITEM_NORMAL);
  wxMenu* itemMenu8 = new wxMenu;
  menuFile->Append(ID_RECENTCFGS, _("Previous Configurations"), itemMenu8);
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT, _("&Quit"), _T(""), wxITEM_NORMAL);
  menuBar->Append(menuFile, _("&File"));
  menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT, _("&About..."), _T(""), wxITEM_NORMAL);
  menuBar->Append(menuHelp, _("&Help"));
  itemFrame1->SetMenuBar(menuBar);

  tbarMain = new wxToolBar( itemFrame1, ID_TOOLBAR, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL|wxTB_NODIVIDER );
  tbarMain->Realize();
  itemFrame1->GetAuiManager().AddPane(tbarMain, wxAuiPaneInfo()
    .ToolbarPane().Name(_T("auiTBarMain")).Top().Layer(10).CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(false).Floatable(false).Gripper(true));

  choiceWindowBrowser = new wxChoicebook( itemFrame1, ID_CHOICEWINBROWSER, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

  itemFrame1->GetAuiManager().AddPane(choiceWindowBrowser, wxAuiPaneInfo()
    .Name(_T("auiWindowBrowser")).Caption(_T("Window browser")).Centre().CloseButton(false).DestroyOnClose(false).Resizable(true).MaximizeButton(true));

  windowProperties = new wxPropertyGrid( itemFrame1, ID_FOREIGN, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER );
  itemFrame1->GetAuiManager().AddPane(windowProperties, wxAuiPaneInfo()
    .Name(_T("auiWindowProperties")).Caption(_T("Window properties")).Centre().Position(1).CloseButton(false).DestroyOnClose(false).Resizable(true).MaximizeButton(true));

  GetAuiManager().Update();

////@end paraverMain content construction
  wxTreeCtrl* tmpTree = new wxTreeCtrl( choiceWindowBrowser, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                        wxTR_HAS_BUTTONS |wxTR_HIDE_ROOT|wxTR_LINES_AT_ROOT );
  tmpTree->SetImageList( imageList );
  tmpTree->AddRoot( wxT( "Root" ), 0, -1, new TreeBrowserItemData( "Root", (gTimeline *)NULL ) );
  choiceWindowBrowser->AddPage( tmpTree, "All Traces" );
}

bool paraverMain::DoLoadTrace( const string &path )
{
  Trace *tr = NULL;
  bool loaded = true;
  
  for( vector<Trace *>::iterator it = loadedTraces.begin(); it != loadedTraces.end(); ++it )
  {
    if( (*it)->getFileName().compare( path ) == 0 )
      return true;
  }

  ProgressController *progress = ProgressController::create( localKernel );
  progress->setHandler( progressFunction );

  try
  {
    if( paraverMain::dialogProgress == NULL )
      paraverMain::dialogProgress = new wxProgressDialog( wxT("Loading trace..."), wxT(""),numeric_limits<int>::max(),
                                         this,
                                         wxPD_AUTO_HIDE|wxPD_APP_MODAL|wxPD_ELAPSED_TIME|wxPD_ESTIMATED_TIME|wxPD_REMAINING_TIME );
    paraverMain::dialogProgress->Pulse( wxT( path.c_str() ) );
    paraverMain::dialogProgress->Fit();
    paraverMain::dialogProgress->Show();
    tr = Trace::create( localKernel, path, progress );
    
    loadedTraces.push_back( tr );
    currentTrace = loadedTraces.size() - 1;
    wxTreeCtrl *newTree =  new wxTreeCtrl( choiceWindowBrowser, wxID_ANY, 
      wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS |wxTR_HIDE_ROOT|wxTR_SINGLE );
    newTree->SetImageList( imageList );
    newTree->AddRoot( wxT( "Root" ), 0, -1, new TreeBrowserItemData( "Root", (gTimeline *)NULL ) );
    choiceWindowBrowser->AddPage( newTree, path );
    previousTraces->add( path );
  }
  catch( ParaverKernelException& ex )
  {
    loaded = false;
    wxMessageDialog message( this, ex.what(), "Error loading trace", wxOK );
    message.ShowModal();
  }
  paraverMain::dialogProgress->Show( false );
  delete paraverMain::dialogProgress;
  paraverMain::dialogProgress = NULL;
  delete progress;
  
  return loaded;
}

// precond window has no parents
void paraverMain::BuildTree( wxTreeCtrl *root1, wxTreeItemId idRoot1,
                             wxTreeCtrl *root2, wxTreeItemId idRoot2,
                             Window *window )
{
  wxTreeItemId currentWindowId1, currentWindowId2;
  TreeBrowserItemData *currentData;

  gTimeline* tmpTimeline = new gTimeline( this, wxID_ANY, window->getName() );
  LoadedWindows::getInstance()->add( window );
  tmpTimeline->SetMyWindow( window );
  tmpTimeline->SetSize( window->getPosX(), window->getPosY(), window->getWidth(), window->getHeight() );
  if( window->getShowWindow() )
  {
    tmpTimeline->Show();
    tmpTimeline->redraw();
  }

  currentData =  new TreeBrowserItemData( window->getName(), tmpTimeline );
  currentWindowId1 = root1->AppendItem( idRoot1, window->getName(), 1, -1, currentData );
  currentWindowId2 = root2->AppendItem( idRoot2, window->getName(), 1, -1, new TreeBrowserItemData( *currentData ) );

  if ( window->getParent( 0 ) != NULL )
  {
    BuildTree( root1, currentWindowId1, root2, currentWindowId2, window->getParent( 0 ) );
    BuildTree( root1, currentWindowId1, root2, currentWindowId2, window->getParent( 1 ) );
  }
}



bool paraverMain::DoLoadCFG( const string &path )
{
   if( !CFGLoader::isCFGFile( path ))
    {
      wxString errMessage = path + " isn't a valid cfg.";
      wxMessageDialog message( this, errMessage.c_str(), "Invalid file", wxOK );
      message.ShowModal();
    }
    else
    {
      vector<Window *> newWindows;
      vector<Histogram *> newHistograms;
      
      if( !CFGLoader::loadCFG( localKernel, path, loadedTraces[ currentTrace ], newWindows, newHistograms ) )
      {
        wxString errMessage = path + " failed to load.";
        wxMessageDialog message( this, errMessage.c_str(), "Loading error", wxOK );
        message.ShowModal();
      }
      else
      {
        wxTreeItemId child;
        for( vector<Window *>::iterator it = newWindows.begin(); it != newWindows.end(); ++it )
        {
          wxTreeCtrl *allTracesPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 );
          wxTreeCtrl *currentPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( currentTrace + 1 );

          if ( (*it)->getChild() == NULL )
            BuildTree( allTracesPage, allTracesPage->GetRootItem(), currentPage, currentPage->GetRootItem(), *it );
        }

        for( vector<Histogram *>::iterator it = newHistograms.begin(); it != newHistograms.end(); ++it )
        {
          gHistogram* tmpHisto = new gHistogram( this, wxID_ANY, (*it)->getName() );

          wxTreeCtrl *allTracesPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 );
          allTracesPage->AppendItem( allTracesPage->GetRootItem(), (*it)->getName(), 0, -1,
            new TreeBrowserItemData( (*it)->getName(), tmpHisto ) );

          wxTreeCtrl *currentPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( currentTrace + 1 );
          currentPage->AppendItem( currentPage->GetRootItem(), (*it)->getName(), 0, -1,
            new TreeBrowserItemData( (*it)->getName(), tmpHisto ) );

          tmpHisto->SetHistogram( *it );
          tmpHisto->SetSize( (*it)->getPosX(), (*it)->getPosY(), (*it)->getWidth(), (*it)->getHeight() );
          if( (*it)->getShowWindow() )
          {
            tmpHisto->Show();
          }
          tmpHisto->execute();
        }
        previousCFGs->add( path );
      }
    }
    return true;
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_OPEN
 */
void paraverMain::OnOpenClick( wxCommandEvent& event )
{
  wxFileDialog dialog( this, "Load Trace", "", "", 
    "Paraver trace (*.prv;*.prv.gz)|*.prv;*.prv.gz|All files (*.*)|*.*", 
    wxOPEN|wxFILE_MUST_EXIST );
  if( dialog.ShowModal() == wxID_OK )
  {
    wxString path = dialog.GetPath();
    DoLoadTrace( path.c_str() );
  }
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENULOADCFG
 */

void paraverMain::OnMenuloadcfgClick( wxCommandEvent& event )
{
  wxFileDialog dialog( this, "Load Configuration", "", "",
    "Paraver configuration file (*.cfg)|*.cfg|All files (*.*)|*.*",
    wxOPEN|wxFILE_MUST_EXIST );
  if( dialog.ShowModal() == wxID_OK )
  {
    wxString path = dialog.GetPath();
    DoLoadCFG( path.c_str());
  }
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
 */

void paraverMain::OnExitClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT in paraverMain.
  // Before editing this code, remove the block markers.
  Destroy();
////@end wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT in paraverMain. 
}


/*!
 * Should we show tooltips?
 */

bool paraverMain::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap paraverMain::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin paraverMain bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
////@end paraverMain bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon paraverMain::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin paraverMain icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end paraverMain icon retrieval
}

/*!
 * wxEVT_PG_CHANGED event handler for ID_FOREIGN
 */
void paraverMain::OnPropertyGridChange( wxPropertyGridEvent& event )
{
  wxPGProperty *property = event.GetProperty();
  
  if( !property )
    return;
    
  const wxString& propName = property->GetName();
  
  if( propName == "Name" )
  {
    wxString tmpName = property->GetValue().GetString();
    if( currentTimeline != NULL )
      currentTimeline->setName( string( tmpName.c_str() ) );
    else if( currentHisto != NULL )
      currentHisto->setName( string( tmpName.c_str() ) );
  }
  else if( propName == "Begin time" )
  {
    double tmpValue = property->GetValue().GetDouble();
    if( currentTimeline != NULL )
    {
      currentTimeline->setWindowBeginTime( tmpValue );
      currentTimeline->setRedraw( true );
    }
    else if( currentHisto != NULL )
    {
      currentHisto->setWindowBeginTime( tmpValue );
      currentHisto->setRecalc( true );
    }
  }
  else if( propName == "End time" )
  {
    double tmpValue = property->GetValue().GetDouble();
    if( currentTimeline != NULL )
    {
      currentTimeline->setWindowEndTime( tmpValue );
      currentTimeline->setRedraw( true );
    }
    else if( currentHisto != NULL )
    {
      currentHisto->setWindowEndTime( tmpValue );
      currentHisto->setRecalc( true );
    }
  }
  // Control Window related properties
  else if( propName == "ControlWindow" )
  {
    currentHisto->setControlWindow( LoadedWindows::getInstance()->getWindow( property->GetValue().GetLong() ) );
    currentHisto->setRecalc( true );
  }
  else if( propName == "ControlMinimum" )
  {
    currentHisto->setControlMin( property->GetValue().GetDouble() );
    currentHisto->setRecalc( true );
  }
  else if( propName == "ControlMaximum" )
  {
    currentHisto->setControlMax( property->GetValue().GetDouble() );
    currentHisto->setRecalc( true );
  }
  else if( propName == "ControlDelta" )
  {
    currentHisto->setControlDelta( property->GetValue().GetDouble() );
    currentHisto->setRecalc( true );
  }
// Data Window related properties
  else if( propName == "DataWindow" )
  {
    currentHisto->setDataWindow( LoadedWindows::getInstance()->getWindow( property->GetValue().GetLong() ) );
    currentHisto->setRecalc( true );
  }
  // Histogram related properties
  else if( propName == "Calculate all" )
    currentHisto->setCalculateAll( property->GetValue().GetBool() );
  else if( propName == "Type" )
  {
    if( property->GetValue().GetLong() == 0 )
      currentHisto->setCurrentStat( currentHisto->getFirstCommStatistic() );
    else
      currentHisto->setCurrentStat( currentHisto->getFirstStatistic() );
    currentHisto->setChanged( true );
    currentHisto->setRedraw( true );
  }
  else if( propName == "Statistic" )
  {
    currentHisto->setCurrentStat( string( property->GetDisplayedString().c_str() ) );
    currentHisto->setRedraw( true );
  }
  else if( propName == "Activate 3D" )
  {
    if( property->GetValueAsString() == wxT( "True" ) )
      currentHisto->setExtraControlWindow( currentHisto->getControlWindow() );
    else
      currentHisto->clearExtraControlWindow();
    currentHisto->setRecalc( true );
  }
  else if( propName == "3rd Window" )
  {
    currentHisto->setExtraControlWindow( LoadedWindows::getInstance()->getWindow( property->GetValue().GetLong() ) );
    currentHisto->setRecalc( true );
  }
  else if( propName == "3DMinimum" )
  {
    currentHisto->setExtraControlMin( property->GetValue().GetDouble() );
    currentHisto->setRecalc( true );
  }
  else if( propName == "3DMaximum" )
  {
    currentHisto->setExtraControlMax( property->GetValue().GetDouble() );
    currentHisto->setRecalc( true );
  }
  else if( propName == "3DDelta" )
  {
    currentHisto->setExtraControlDelta( property->GetValue().GetDouble() );
    currentHisto->setRecalc( true );
  }
  else if( propName == "3DPlane" )
  {
    currentHisto->setSelectedPlane( property->GetValue().GetLong() );
    currentHisto->setRedraw( true );
  }
  
  // Timeline related properties
  
}


/*!
 * wxEVT_TREE_SEL_CHANGED event handler for wxID_ANY
 */
void paraverMain::OnTreeSelChanged( wxTreeEvent& event )
{
  wxTreeCtrl *tmpTree = static_cast<wxTreeCtrl *>( event.GetEventObject() );
  TreeBrowserItemData *itemData = static_cast<TreeBrowserItemData *>( tmpTree->GetItemData( event.GetItem() ) );
  
  if( gHistogram *histo = itemData->getHistogram() )
  {
    currentHisto = histo->GetHistogram();
    currentTimeline = NULL;
    if( histo->IsShown() )
      histo->Raise();
  }
  else if( gTimeline *timeline = itemData->getTimeline() )
  {
    currentHisto = NULL;
    currentTimeline = timeline->GetMyWindow();
    if( timeline->IsShown() )
      timeline->Raise();
  }
}

/*!
 * wxEVT_TREE_ITEM_ACTIVATED event handler for wxID_ANY
 */
void paraverMain::OnTreeItemActivated( wxTreeEvent& event )
{
  wxTreeCtrl *tmpTree = static_cast<wxTreeCtrl *>( event.GetEventObject() );
  TreeBrowserItemData *itemData = static_cast<TreeBrowserItemData *>( tmpTree->GetItemData( event.GetItem() ) );
  
  if( gHistogram *histo = itemData->getHistogram() )
  {
    Histogram *tmpHisto = histo->GetHistogram();
    tmpHisto->setShowWindow( !tmpHisto->getShowWindow() );
  }
  else if( gTimeline *timeline = itemData->getTimeline() )
  {
    Window *tmpWin = timeline->GetMyWindow();
    tmpWin->setShowWindow( !tmpWin->getShowWindow() );
  }
}

void paraverMain::updateTimelineProperties( Window *whichWindow )
{
  if( lastTimeline == whichWindow && !whichWindow->getChanged() )
    return;
  whichWindow->setChanged( false );
  lastTimeline = whichWindow;
  windowProperties->Freeze();
  windowProperties->Clear();
  wxArrayString arrayStr;
  wxArrayInt arrayInt;
  
  windowProperties->Append( new wxStringProperty( wxT("Name"), wxPG_LABEL, wxT( whichWindow->getName() ) ) );
  windowProperties->Append( new wxFloatProperty( wxT("Begin time"), wxPG_LABEL, 
                            wxT( whichWindow->getWindowBeginTime() ) ) );
  windowProperties->Append( new wxFloatProperty( wxT("End time"), wxPG_LABEL, 
                            wxT( whichWindow->getWindowEndTime() ) ) );

  // Filter related properties
  if( whichWindow->getFilter() != NULL )
  {
    wxPGId filterCat = windowProperties->Append( new wxPropertyCategory( wxT("Filter") ) );
    //wxPGId commFilterCat = windowProperties->AppendIn( filterCat, new wxPropertyCategory( wxT("Communications") ) );
  
    wxPGId eventFilterCat = windowProperties->AppendIn( filterCat, new wxPropertyCategory( wxT("Events") ) );
    wxPGId eventFilterType = windowProperties->AppendIn( eventFilterCat, 
                                                         new wxStringProperty( wxT("Event type"), 
                                                                               wxPG_LABEL,
                                                                               wxT("<composed>") ) );

    arrayStr.Add(wxT("="));
    arrayInt.Add(0);
    windowProperties->AppendIn( eventFilterType, new wxEnumProperty( wxT("Function"), 
                                                                     wxPG_LABEL, arrayStr, arrayInt ) );

    vector<TEventType> types;
    whichWindow->getTrace()->getEventLabels().getTypes( types );
    arrayStr.Clear();
    arrayInt.Clear();
    for( vector<TEventType>::iterator it = types.begin(); it != types.end(); ++it )
    {
      arrayInt.Add( (*it) );
      string tmpstr;
      whichWindow->getTrace()->getEventLabels().getEventTypeLabel( (*it), tmpstr );
      arrayStr.Add( wxString() << (*it) << " " << wxT( tmpstr.c_str() ) );
    }
    wxPGChoices typeChoices( arrayStr, arrayInt );
    wxArrayInt values;
    vector<TEventType> typesSel;
    whichWindow->getFilter()->getEventType( typesSel );
    for( vector<TEventType>::iterator it = typesSel.begin(); it != typesSel.end(); ++it )
      values.Add( (*it ) );
    prvEventTypeProperty *tmpEventProperty = new prvEventTypeProperty( wxT("Types"), wxPG_LABEL, typeChoices, values );
    windowProperties->AppendIn( eventFilterType, tmpEventProperty );
    windowProperties->SetPropertyAttribute( tmpEventProperty->GetId(), wxPG_ATTR_MULTICHOICE_USERSTRINGMODE, 1 );
  }
  // END of Filter related properties
  
  windowProperties->SetPropertyAttributeAll( wxPG_BOOL_USE_CHECKBOX, true );

  windowProperties->Refresh();
  windowProperties->Thaw();
}

void paraverMain::updateHistogramProperties( Histogram *whichHisto )
{
  if( lastHisto == whichHisto && !whichHisto->getChanged() )
    return;
  whichHisto->setChanged( false );
  lastHisto = whichHisto;
  windowProperties->Freeze();
  windowProperties->Clear();
  
  windowProperties->Append( new wxStringProperty( wxT("Name"), wxPG_LABEL, wxT( whichHisto->getName() ) ) );
  windowProperties->Append( new wxFloatProperty( wxT("Begin time"), wxPG_LABEL, 
                            wxT( whichHisto->getBeginTime() ) ) );
  windowProperties->Append( new wxFloatProperty( wxT("End time"), wxPG_LABEL, 
                            wxT( whichHisto->getEndTime() ) ) );
  // Statistic related properties
  wxPGId statCat = windowProperties->Append( new wxPropertyCategory( wxT("Statistics") ) );
  windowProperties->AppendIn( statCat, new wxBoolProperty( wxT("Calculate all"), wxPG_LABEL, whichHisto->getCalculateAll() ) );
  vector<string> tmpV;
  wxArrayString arrayStr;
  wxArrayInt arrayInt;
  whichHisto->getGroupsLabels( tmpV );
  for( vector<string>::iterator it = tmpV.begin(); it != tmpV.end(); ++it )
    arrayStr.Add( wxT( (*it).c_str() ) );
  arrayInt.Add( 0 );
  arrayInt.Add( 1 );
  int selected;
  if( whichHisto->itsCommunicationStat( whichHisto->getCurrentStat() ) )
    selected = 0;
  else
    selected = 1;
  windowProperties->AppendIn( statCat, new wxEnumProperty( wxT("Type"), wxPG_LABEL, arrayStr, arrayInt, selected ) );
  
  tmpV.clear();
  arrayStr.Clear();
  arrayInt.Clear();
  whichHisto->getStatisticsLabels( tmpV, selected );
  int pos = 0;
  selected = -1;
  for( vector<string>::iterator it = tmpV.begin(); it != tmpV.end(); ++it )
  {
    arrayStr.Add( wxT( (*it).c_str() ) );
    arrayInt.Add( pos );
    if( (*it) == whichHisto->getCurrentStat() )
      selected = pos;
    pos++;
  }
  if( selected == -1 ) selected = 0;
  windowProperties->AppendIn( statCat, new wxEnumProperty( wxT("Statistic"), wxPG_LABEL, arrayStr, arrayInt, selected ) );

  // Control Window related properties
  wxPGId ctrlCat = windowProperties->Append( new wxPropertyCategory( wxT("Control") ) );
  vector<TWindowID> validWin;
  Window *dataWindow = ( whichHisto->getDataWindow() == NULL ) ? whichHisto->getControlWindow() :
                                                                 whichHisto->getDataWindow();
  LoadedWindows::getInstance()->getValidControlWindow( dataWindow, validWin );
  arrayStr.Clear();
  arrayInt.Clear();
  selected = -1;
  for( vector<TWindowID>::iterator it = validWin.begin(); it != validWin.end(); ++it )
  {
    arrayStr.Add( wxT( LoadedWindows::getInstance()->getWindow( (*it) )->getName().c_str() ) );
    arrayInt.Add( (*it) );
    // Do we need this -if- here?
    if( LoadedWindows::getInstance()->getWindow( (*it) ) == whichHisto->getControlWindow() )
      selected = (*it);
  }
  wxEnumProperty *tmpCtrlWin = new wxEnumProperty( wxT("Window"), wxT("ControlWindow"), arrayStr, arrayInt, selected );
  windowProperties->AppendIn( ctrlCat, tmpCtrlWin );
  windowProperties->AppendIn( ctrlCat, new wxFloatProperty( wxT("Minimum"), wxT("ControlMinimum"), wxT( whichHisto->getControlMin() )));
  windowProperties->AppendIn( ctrlCat, new wxFloatProperty( wxT("Maximum"), wxT("ControlMaximum"), wxT( whichHisto->getControlMax() )));
  windowProperties->AppendIn( ctrlCat, new wxFloatProperty( wxT("Delta"), wxT("ControlDelta"), wxT( whichHisto->getControlDelta() )));

  // Data Window related properties
  wxPGId dataCat = windowProperties->Append( new wxPropertyCategory( wxT("Data") ) );
  validWin.clear();  //  vector<TWindowID> validWin;
  LoadedWindows::getInstance()->getValidDataWindow( whichHisto->getControlWindow(),
                                                    whichHisto->getExtraControlWindow(),
                                                    validWin );
  arrayStr.Clear();
  arrayInt.Clear();
  selected = -1;
  for( vector<TWindowID>::iterator it = validWin.begin(); it != validWin.end(); ++it )
  {
    arrayStr.Add( wxT( LoadedWindows::getInstance()->getWindow( (*it) )->getName().c_str() ) );
    arrayInt.Add( (*it) );
    if( LoadedWindows::getInstance()->getWindow( (*it) ) == whichHisto->getDataWindow() )
      selected = (*it);
  }
  wxEnumProperty *tmpDataWin = new wxEnumProperty( wxT("Window"), wxT("DataWindow"), arrayStr, arrayInt, selected );
  windowProperties->AppendIn( dataCat, tmpDataWin );

  // 3rd window related properties
  wxPGId thirdWinCat = windowProperties->Append( new wxPropertyCategory( wxT("3D") ) );
  windowProperties->AppendIn( thirdWinCat, new wxBoolProperty( wxT("Activate 3D"), wxPG_LABEL, whichHisto->getThreeDimensions() ) );
  validWin.clear();
  dataWindow = ( whichHisto->getDataWindow() == NULL ) ? whichHisto->getControlWindow() :
                                                                 whichHisto->getDataWindow();
  LoadedWindows::getInstance()->getValidControlWindow( dataWindow, validWin );
  arrayStr.Clear();
  arrayInt.Clear();
  selected = -1;
  for( vector<TWindowID>::iterator it = validWin.begin(); it != validWin.end(); ++it )
  {
    arrayStr.Add( wxT( LoadedWindows::getInstance()->getWindow( (*it) )->getName().c_str() ) );
    arrayInt.Add( (*it) );
    if( ( LoadedWindows::getInstance()->getWindow( (*it) ) == whichHisto->getExtraControlWindow() ) ||
        ( whichHisto->getExtraControlWindow() == NULL && 
          LoadedWindows::getInstance()->getWindow( (*it) ) == whichHisto->getControlWindow() ) )
      selected = (*it);
  }
  wxEnumProperty *tmp3rdWin = new wxEnumProperty( wxT("3rd Window"), wxPG_LABEL, arrayStr, arrayInt, selected );
  windowProperties->AppendIn( thirdWinCat, tmp3rdWin );
  wxFloatProperty *tmp3dMin = new wxFloatProperty( wxT("Minimum"), wxT("3DMinimum"), whichHisto->getExtraControlMin() );
  windowProperties->AppendIn( thirdWinCat, tmp3dMin );
  wxFloatProperty *tmp3dMax = new wxFloatProperty( wxT("Maximum"), wxT("3DMaximum"), whichHisto->getExtraControlMax() );
  windowProperties->AppendIn( thirdWinCat, tmp3dMax );
  wxFloatProperty *tmp3dDelta = new wxFloatProperty( wxT("Delta"), wxT("3DDelta"), whichHisto->getExtraControlDelta() );
  windowProperties->AppendIn( thirdWinCat, tmp3dDelta );
  arrayStr.Clear();
  arrayInt.Clear();
  pos = 0;
  selected = -1;
  for( THistogramColumn i = 0; i < whichHisto->getNumPlanes(); i++ )
  {
    if( whichHisto->planeWithValues( i ) )
    {
      arrayStr.Add( whichHisto->getPlaneLabel( i ) );
      arrayInt.Add( pos );
      if( pos == whichHisto->getSelectedPlane() )
        selected = pos;
    }
    pos++;
  }
  wxEnumProperty *tmp3dPlane = new wxEnumProperty( wxT("Plane"), wxT("3DPlane"), arrayStr, arrayInt, selected );
  windowProperties->AppendIn( thirdWinCat, tmp3dPlane );

  if( !whichHisto->getThreeDimensions() )
  {
    tmp3rdWin->SetFlagsFromString( "DISABLED" );
    tmp3dMin->SetFlagsFromString( "DISABLED" );
    tmp3dMax->SetFlagsFromString( "DISABLED" );
    tmp3dDelta->SetFlagsFromString( "DISABLED" );
    tmp3dPlane->SetFlagsFromString( "DISABLED" );
  }

  windowProperties->SetPropertyAttributeAll( wxPG_BOOL_USE_CHECKBOX, true );
  windowProperties->Refresh();
  windowProperties->Thaw();
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHOICEWINBROWSER
 */

void paraverMain::OnChoicewinbrowserUpdate( wxUpdateUIEvent& event )
{
  for( unsigned int iPage = 0; iPage < choiceWindowBrowser->GetPageCount(); iPage++ )
  {
    wxTreeCtrl *currentTree = (wxTreeCtrl *) choiceWindowBrowser->GetPage( iPage );
    wxTreeItemId root = currentTree->GetRootItem();
    if( currentTree->ItemHasChildren( root ) )
    {
      wxTreeItemIdValue cookie;
      wxTreeItemId currentChild = currentTree->GetFirstChild( root, cookie );
      while( currentChild.IsOk() )
      {
        updateTreeItem( currentTree, currentChild );
        currentChild = currentTree->GetNextChild( root, cookie );
      }
    }
  }
}

void paraverMain::updateTreeItem( wxTreeCtrl *tree, wxTreeItemId& id )
{
  TreeBrowserItemData *itemData = (TreeBrowserItemData *)tree->GetItemData( id );
  wxString tmpName;
  if( gTimeline *tmpTimeline = itemData->getTimeline() )
  {
    if( tmpTimeline->IsActive() )
    {
      currentWindow = tmpTimeline;
      tree->SelectItem( id );
    }
    tmpName = tmpTimeline->GetMyWindow()->getName();
  }
  else if( gHistogram *tmpHistogram = itemData->getHistogram() )
  {
    if( tmpHistogram->IsActive() )
    {
      currentWindow = tmpHistogram;
      tree->SelectItem( id );
    }
    tmpName = tmpHistogram->GetHistogram()->getName();
  }
  if( tmpName != tree->GetItemText( id ) )
    tree->SetItemText( id, tmpName );
    
  if( tree->ItemHasChildren( id ) )
  {
    wxTreeItemIdValue cookie;
    wxTreeItemId currentChild = tree->GetFirstChild( id, cookie );
    while( currentChild.IsOk() )
    {
      updateTreeItem( tree, currentChild );
      currentChild = tree->GetNextChild( id, cookie );
    }
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_FOREIGN
 */

void paraverMain::OnForeignUpdate( wxUpdateUIEvent& event )
{
  if( currentHisto != NULL )
  {
    lastTimeline = NULL;
    updateHistogramProperties( currentHisto );
  }
  else if( currentTimeline != NULL )
  {
    lastHisto = NULL;
    updateTimelineProperties( currentTimeline );
  }
}


void paraverMain::OnPreviousTracesClick( wxCommandEvent& event )
{
  int eventId = event.GetId();
  int i = 0;

  wxMenuItem *item = menuFile->FindItem( ID_RECENTTRACES );
  wxMenu *menu = item->GetSubMenu();
  wxMenuItemList& menuItems = menu->GetMenuItems();
  for ( wxMenuItemList::iterator menuIt = menuItems.begin(); menuIt != menuItems.end(); ++menuIt )
  {
    wxMenuItem *tmp = *menuIt;
    int currentId = tmp->GetId();
    if ( currentId == eventId )
      DoLoadTrace( previousTraces->getFiles()[i] );
    i++;
  }
}


void paraverMain::OnPreviousCFGsClick( wxCommandEvent& event )
{
  int eventId = event.GetId();
  int i = 0;
  
  wxMenuItem *item = menuFile->FindItem( ID_RECENTCFGS );
  wxMenu *menu = item->GetSubMenu();
  wxMenuItemList& menuItems = menu->GetMenuItems();
  
  for ( wxMenuItemList::iterator menuIt = menuItems.begin(); menuIt != menuItems.end(); ++menuIt )
  {
    wxMenuItem *tmp = *menuIt;
    int currentId = tmp->GetId();
    if ( currentId == eventId )
      DoLoadCFG( previousCFGs->getFiles()[i] );
    i++;
  }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_RECENTTRACES
 */

void paraverMain::OnRecenttracesUpdate( wxUpdateUIEvent& event )
{
  vector<string> v = previousTraces->getFiles();

  wxMenuItem *tmpItem = menuFile->FindItem(ID_RECENTTRACES );
  wxMenu *menuTraces = tmpItem->GetSubMenu();

  wxMenuItemList& menuItems = menuTraces->GetMenuItems();
  wxMenuItemList::iterator menuIt = menuItems.begin();

  for ( vector<string>::iterator it = v.begin(); it != v.end(); ++it )
  {
    if( menuIt == menuItems.end() )
    {
      wxMenuItem *newItem = new wxMenuItem( menuTraces, wxID_ANY, wxT( (*it).c_str() ) );
      menuTraces->Append( newItem );
      Connect( newItem->GetId(),
               wxEVT_COMMAND_MENU_SELECTED,
               (wxObjectEventFunction)&paraverMain::OnPreviousTracesClick );
    }
    else
    {
      wxMenuItem *tmp = *menuIt;
      tmp->SetItemLabel( wxT( (*it).c_str() ) );
      ++menuIt;
    }
  }
/*
  while( menuIt != menuItems.end() )
  {
    wxMenuItem *tmp = *menuIt;
    int id = tmp->GetId();
    Disconnect( id, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&paraverMain::OnPreviousTracesClick );
    menuTraces->Delete( tmp );
    ++menuIt;
  }
*/
}
/*!
 * wxEVT_UPDATE_UI event handler for ID_MENULOADCFG
 */

void paraverMain::OnMenuloadcfgUpdate( wxUpdateUIEvent& event )
{
  if( currentTrace == -1 )
    event.Enable( false );
  else
    event.Enable( true );
      
  vector<string> v = previousCFGs->getFiles();

  wxMenuItem *tmpItem = menuFile->FindItem( ID_RECENTCFGS );
  wxMenu *menuCFGs = tmpItem->GetSubMenu();

  wxMenuItemList& menuItems = menuCFGs->GetMenuItems();
  wxMenuItemList::iterator menuIt = menuItems.begin();

  for ( vector<string>::iterator it = v.begin(); it != v.end(); ++it )
  {
    if( menuIt == menuItems.end() )
    {
      wxMenuItem *newItem = new wxMenuItem( menuCFGs, wxID_ANY, wxT( (*it).c_str() ) );
      menuCFGs->Append( newItem );
      Connect( newItem->GetId(),
               wxEVT_COMMAND_MENU_SELECTED,
              (wxObjectEventFunction)&paraverMain::OnPreviousCFGsClick );
    }
    else
    {
      wxMenuItem *tmp = *menuIt;
      tmp->SetItemLabel( wxT( (*it).c_str() ) );
      ++menuIt;
    }
  }
/* 
  while( menuIt != menuItems.end() )
  {
    wxMenuItem *tmp = *menuIt;
    int id = tmp->GetId();
    Disconnect( id, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&paraverMain::OnPreviousCFGsClick );
    menuCFGs->Delete( tmp );
    ++menuIt;
  }
*/
}

void progressFunction( ProgressController *progress )
{
  int p = (int)floor( ( progress->getCurrentProgress() * numeric_limits<int>::max() ) / progress->getEndLimit() );
  paraverMain::dialogProgress->Update( p );
//  app->Yield();
}

/*!
 * wxEVT_IDLE event handler for ID_PARAVERMAIN
 */

void paraverMain::OnIdle( wxIdleEvent& event )
{
  static bool raise = true;
  
  if( wxTheApp->IsActive() )
  {
    if( currentWindow != NULL && raise )
    {
      raise = false;
      currentWindow->Raise();
    }
  }
  else
    raise = true;
}

