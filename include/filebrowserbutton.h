/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                                  wxparaver                                *
 *              Paraver Trace Visualization and Analysis Tool                *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/


#pragma once


#include <wx/button.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/textctrl.h>

class BrowserButton : public wxButton
{
  DECLARE_DYNAMIC_CLASS( BrowserButton )
  DECLARE_EVENT_TABLE()

  public:
    static const wxString DEFAULT_WILDCARD;

    BrowserButton() {}
    BrowserButton( wxWindow* parent,
                   wxWindowID id,
                   const wxString& label = wxEmptyString,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = 0,

                   wxTextCtrl *whichTextCtrl = nullptr, // associated text control
                   const wxString& whichDialogMessage = wxT( "Choose a file" ),
                   const wxString& whichDialogDefaultDir = wxT( "" ),
                   long whichDialogStyle = wxFD_DEFAULT_STYLE, // set wxFileDialog style

                   const wxValidator& validator = wxDefaultValidator,
                   const wxString& name = wxT( "button" ) );
    ~BrowserButton() {}
  
    void SetTextBox( wxTextCtrl *whichTextCtrl, bool readOnly = true );
    void SetDialogMessage( const wxString& whichDialogMessage ) { dialogMessage = whichDialogMessage; }
    void SetDialogDefaultDir( const wxString& defaultDir ) { dialogDefaultDir = defaultDir; }
    void SetDialogStyle( long whichDialogStyle ) { dialogStyle = whichDialogStyle; }
    void SetPath( const wxString& whichFullPath ) { fullPath = wxFileName( whichFullPath ); }

    // Mimics FilePicker
    wxString GetPath() const
    { 
      return ( fullPath.GetPath( wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR ) + fullPath.GetFullName() ); 
    }

    bool Enable( bool enable = true );

    void OnButton( wxMouseEvent& event ) {} // Can't be pure virtual

  protected:  
    wxTextCtrl *associatedTextCtrl; // Will show only file name, path through tooltip

    // FileDialog common properties
    wxString dialogMessage;
    wxString dialogDefaultDir;
    long dialogStyle;

    void Init();

  private:
    wxFileName fullPath;
};


class FileBrowserButton : public BrowserButton
{
  DECLARE_DYNAMIC_CLASS( FileBrowserButton )
  DECLARE_EVENT_TABLE()
  
  public:
    FileBrowserButton() { Init(); }
    FileBrowserButton( wxWindow* parent,
                       wxWindowID id,
                       const wxString& label = wxEmptyString,
                       const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxDefaultSize,
                       long style = 0,

                       wxTextCtrl *whichTextCtrl = nullptr, // associated text control
                       const wxString& whichDialogMessage = wxT( "Choose a file" ),
                       const wxString& whichDialogDefaultDir = wxT( "" ),
                       const wxString& whichFileDialogDefaultFile = wxT( "" ),
                       const wxString& whichFileDialogWildcard = DEFAULT_WILDCARD,
                       long whichDialogStyle = wxFD_DEFAULT_STYLE, // set wxFileDialog style

                       const wxValidator& validator = wxDefaultValidator,
                       const wxString& name = wxT( "button" ) );
    ~FileBrowserButton() {}    

    void SetFileDialogDefaultFile( const wxString& defaultFile )
    { fileDialogDefaultFile = defaultFile; }
    
    void SetFileDialogWildcard( const wxString& whichFileDialogWildcard )
    { fileDialogWildcard = whichFileDialogWildcard; }
    
    // Remember this path and modify associated wxTextCtrl
    void SetPath( const wxString& whichPath );
    void ChangePath( const wxString& whichPath );

    void OnButton( wxMouseEvent& event );
    
    
  private:
    // wxFileDialog properties
    wxString fileDialogDefaultFile;
    wxString fileDialogWildcard;
    
    void Init();
};


class DirBrowserButton : public BrowserButton
{
  DECLARE_DYNAMIC_CLASS( FileBrowserButton )
  DECLARE_EVENT_TABLE()
  
  public:
    DirBrowserButton() { Init(); }
    DirBrowserButton( wxWindow* parent,
                       wxWindowID id,
                       const wxString& label = wxEmptyString,
                       const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxDefaultSize,
                       long style = 0,

                       wxTextCtrl *whichTextCtrl = nullptr, // associated text control
                       const wxString& whichDialogMessage = wxT( "Choose a file" ),
                       const wxString& whichDialogDefaultDir = wxT( "" ),
                       long whichDialogStyle = wxFD_DEFAULT_STYLE, // set wxFileDialog style

                       const wxValidator& validator = wxDefaultValidator,
                       const wxString& name = wxT( "button" ) );
    ~DirBrowserButton() {}    

    // Remember this path and modify associated wxTextCtrl
    void SetPath( const wxString& whichPath );
    
    void OnButton( wxMouseEvent& event );
};


