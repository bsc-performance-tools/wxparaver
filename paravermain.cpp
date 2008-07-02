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
#include "histogram.h"

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

////@end paraverMain event table entries

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
  menuFile = NULL;
  menuHelp = NULL;
  tbarMain = NULL;
  choiceWindowBrowser = NULL;
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
  menuFile->AppendSeparator();
  menuFile->Append(ID_MENULOADCFG, _("Load &Configuration"), _T(""), wxITEM_NORMAL);
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
    .Name(_T("auiWindowBrowser")).Caption(_T("Window browser")).Centre().DestroyOnClose(false).Resizable(true).MaximizeButton(true));

  GetAuiManager().Update();

////@end paraverMain content construction
  wxTreeCtrl* tmpTree = new wxTreeCtrl( choiceWindowBrowser, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS |wxTR_HIDE_ROOT|wxTR_SINGLE );
  tmpTree->SetImageList( imageList );
  tmpTree->AddRoot( wxT( "Root" ), 0, -1, new TreeBrowserItemData( "Root" ) );
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
      newTree->AddRoot( wxT( "Root" ), 0, -1, new TreeBrowserItemData( "Root" ) );
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
        for( vector<Histogram *>::iterator it = newHistograms.begin(); it != newHistograms.end(); it++ )
        {
          wxTreeCtrl *allTracesPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( 0 );
          allTracesPage->AppendItem( allTracesPage->GetRootItem(), (*it)->getName(), 0, -1,
            new TreeBrowserItemData( (*it)->getName() ) );
          wxTreeCtrl *currentPage = (wxTreeCtrl *) choiceWindowBrowser->GetPage( currentTrace + 1 );
          currentPage->AppendItem( currentPage->GetRootItem(), (*it)->getName(), 0, -1,
            new TreeBrowserItemData( (*it)->getName() ) );
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



