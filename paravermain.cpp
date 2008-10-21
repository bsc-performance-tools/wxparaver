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

////@begin XPM images
////@end XPM images

#include "table.xpm"

/*!
 * paraverMain type definition
 */

IMPLEMENT_CLASS( paraverMain, wxFrame )


/*!
 * paraverMain event table definition
 */

BEGIN_EVENT_TABLE( paraverMain, wxFrame )

////@begin paraverMain event table entries
  EVT_MENU( wxID_OPEN, paraverMain::OnOpenClick )

  EVT_MENU( ID_MENULOADCFG, paraverMain::OnMenuloadcfgClick )
  EVT_UPDATE_UI( ID_MENULOADCFG, paraverMain::OnMenuloadcfgUpdate )

  EVT_MENU( wxID_EXIT, paraverMain::OnExitClick )

  EVT_UPDATE_UI( ID_CHOICEWINBROWSER, paraverMain::OnChoicewinbrowserUpdate )

////@end paraverMain event table entries

  EVT_TREE_SEL_CHANGED( wxID_ANY, paraverMain::OnTreeSelChanged )
  EVT_TREE_ITEM_ACTIVATED( wxID_ANY, paraverMain::OnTreeItemActivated )
  
  EVT_PG_CHANGED( ID_FOREIGN, paraverMain::OnPropertyGridChange )
END_EVENT_TABLE()


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

  for( vector<Trace *>::iterator it = loadedTraces.begin(); it != loadedTraces.end(); it++ )
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
  currentWindow = NULL;
  currentHisto = NULL;
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
  menuFile->Append(wxID_RECENTTRACES, _("Previous Traces"), itemMenu5);
  menuFile->AppendSeparator();
  menuFile->Append(ID_MENULOADCFG, _("Load &Configuration"), _T(""), wxITEM_NORMAL);
  wxMenu* itemMenu8 = new wxMenu;
  menuFile->Append(wxID_RECENTCFGS, _("Previous Configurations"), itemMenu8);
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
  wxTreeCtrl* tmpTree = new wxTreeCtrl( choiceWindowBrowser, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS |wxTR_HIDE_ROOT|wxTR_SINGLE );
  tmpTree->SetImageList( imageList );
  tmpTree->AddRoot( wxT( "Root" ), 0, -1, new TreeBrowserItemData( "Root", (gTimeline *)NULL ) );
  choiceWindowBrowser->AddPage( tmpTree, "All Traces" );
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
    Trace *tr = NULL;
    wxString path = dialog.GetPath();
    for( vector<Trace *>::iterator it = loadedTraces.begin(); it != loadedTraces.end(); it++ )
    {
      if( (*it)->getFileName().compare( path.c_str() ) == 0 )
        return;
    }
    
    try
    {
      tr = Trace::create( localKernel, path.c_str() );
      loadedTraces.push_back( tr );
      currentTrace = loadedTraces.size() - 1;
      wxTreeCtrl *newTree =  new wxTreeCtrl( choiceWindowBrowser, wxID_ANY, 
        wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS |wxTR_HIDE_ROOT|wxTR_SINGLE );
      newTree->SetImageList( imageList );
      newTree->AddRoot( wxT( "Root" ), 0, -1, new TreeBrowserItemData( "Root", (gTimeline *)NULL ) );
      choiceWindowBrowser->AddPage( newTree, path );
    }
    catch( ParaverKernelException& ex )
    {
      wxMessageDialog message( this, ex.what(), "Error loading trace", wxOK );
      message.ShowModal();
    }
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
    if( !CFGLoader::isCFGFile( path.c_str() ) )
    {
      wxString errMessage = path + " isn't a valid cfg.";
      wxMessageDialog message( this, errMessage.c_str(), "Invalid file", wxOK );
      message.ShowModal();
    }
    else
    {
      vector<Window *> newWindows;
      vector<Histogram *> newHistograms;
      
      if( !CFGLoader::loadCFG( localKernel, path.c_str(), loadedTraces[ currentTrace ], newWindows, newHistograms ) )
      {
        wxString errMessage = path + " failed to load.";
        wxMessageDialog message( this, errMessage.c_str(), "Loading error", wxOK );
        message.ShowModal();
      }
      else
      {
        for( vector<Window *>::iterator it = newWindows.begin(); it != newWindows.end(); it++ )
        {
          gTimeline* tmpTimeline = new gTimeline( this, wxID_ANY, (*it)->getName() );

          wxTreeCtrl *allTracesPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 );
          allTracesPage->AppendItem( allTracesPage->GetRootItem(), (*it)->getName(), -1, -1,
            new TreeBrowserItemData( (*it)->getName(), tmpTimeline ) );
            
          wxTreeCtrl *currentPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( currentTrace + 1 );
          currentPage->AppendItem( currentPage->GetRootItem(), (*it)->getName(), -1, -1,
            new TreeBrowserItemData( (*it)->getName(), tmpTimeline ) );
        
          tmpTimeline->SetMyWindow( *it );
          tmpTimeline->SetSize( (*it)->getPosX(), (*it)->getPosY(), (*it)->getWidth(), (*it)->getHeight() );
          if( (*it)->getShowWindow() )
          {
            tmpTimeline->Show();
            tmpTimeline->redraw();
          }
        }
        
        for( vector<Histogram *>::iterator it = newHistograms.begin(); it != newHistograms.end(); it++ )
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
          tmpHisto->Show();
          tmpHisto->execute();
        }
      }
    }
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
 * wxEVT_UPDATE_UI event handler for ID_MENULOADCFG
 */

void paraverMain::OnMenuloadcfgUpdate( wxUpdateUIEvent& event )
{
  if( currentTrace == -1 )
    menuFile->Enable( ID_MENULOADCFG, false );
  else
    menuFile->Enable( ID_MENULOADCFG, true );
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
    if( currentWindow != NULL )
      currentWindow->setName( string( tmpName.c_str() ) );
    else if( currentHisto != NULL )
      currentHisto->setName( string( tmpName.c_str() ) );
  }
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
    currentWindow = NULL;
    updateHistogramProperties( currentHisto );
  }
  else if( gTimeline *timeline = itemData->getTimeline() )
  {
    currentHisto = NULL;
    currentWindow = timeline->GetMyWindow();
    updateTimelineProperties( currentWindow );
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
    //Histogram *tmpHisto = histo->GetHistogram();
    
/*    currentHisto = histo->GetHistogram();
    currentWindow = NULL;
    updateHistogramProperties( currentHisto );*/
  }
  else if( gTimeline *timeline = itemData->getTimeline() )
  {
    Window *tmpWin = timeline->GetMyWindow();
    tmpWin->setShowWindow( !tmpWin->getShowWindow() );
/*    currentHisto = NULL;
    currentWindow = timeline->GetMyWindow();
    updateTimelineProperties( currentWindow );*/
  }
}

void paraverMain::updateTimelineProperties( Window *whichWindow )
{
  windowProperties->Freeze();
  windowProperties->Clear();
  
  windowProperties->Append( new wxStringProperty( wxT("Name"), wxPG_LABEL, wxT( whichWindow->getName() ) ) );
  
  windowProperties->Refresh();
  windowProperties->Thaw();
}

void paraverMain::updateHistogramProperties( Histogram *whichHisto )
{
  windowProperties->Freeze();
  windowProperties->Clear();
  
  windowProperties->Append( new wxStringProperty( wxT("Name"), wxPG_LABEL, wxT( whichHisto->getName() ) ) );
  
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
    tmpName = tmpTimeline->GetMyWindow()->getName();
  }
  else if( gHistogram *tmpHistogram = itemData->getHistogram() )
  {
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
