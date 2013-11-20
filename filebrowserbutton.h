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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#ifndef _FILEBROWSERBUTTON_H_ 
#define _FILEBROWSERBUTTON_H_

#include <wx/button.h>
#include <wx/textctrl.h>
//#include <wx/string.h>

class FileBrowserButton : public wxButton
{
  DECLARE_DYNAMIC_CLASS( FileBrowserButton )
  DECLARE_EVENT_TABLE()
  
  public:
  
    enum TFileDialogMode
    {
      FILE_MODE,
      DIRECTORY_MODE
    };
  
    FileBrowserButton() { Init(); }
    FileBrowserButton( wxWindow* parent,
                       wxWindowID id,
                       const wxString& label = wxEmptyString,
                       const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxDefaultSize,
                       long style = 0,

                       wxTextCtrl *whichTextCtrl = NULL, // associated text control
                       const wxString& whichFileDialogMessage = wxT( "Choose a file" ),
                       const wxString& whichFileDialogDefaultDir = wxT( "" ),
                       const wxString& whichFileDialogDefaultFile = wxT( "" ),
                       const wxString& whichFileDialogWildcard = wxT( "*.*" ),
                       long whichFileDialogStyle = wxFD_DEFAULT_STYLE, // set wxFileDialog style
                       TFileDialogMode whichFileDialogMode = FILE_MODE,   // to recover files or dirs

                       const wxValidator& validator = wxDefaultValidator,
                       const wxString& name = wxT( "button" ) );
    ~FileBrowserButton() {}
    

    void setTextBox( wxTextCtrl *whichTextCtrl )
    { associatedTextCtrl = whichTextCtrl; Enable( whichTextCtrl != NULL ); }
    
    void setFileDialogDefaultDir( const wxString& whichFileDialogDefaultDir )
    { fileDialogDefaultDir = whichFileDialogDefaultDir; }
    
    void setFileDialogDefaultFile( const wxString& whichFileDialogDefaultFile )
    { fileDialogDefaultFile = whichFileDialogDefaultFile; }
    
    void setFileDialogWildcard( const wxString& whichFileDialogWildcard )
    { fileDialogWildcard = whichFileDialogWildcard; }
    
    void setFileDialogStyle( long whichFileDialogStyle )
    { fileDialogStyle = whichFileDialogStyle; }
    
    void setFileDialogMode( TFileDialogMode whichFileDialogMode )
    { fileDialogMode = whichFileDialogMode; }
 

    void OnButton( wxMouseEvent& event );
    
    // Remembers passed path and informs textCtrl
    void SetPath( const wxString& whichPath );
    
    // Gathers from last set path
    wxString GetPath() const;

    
  private:
    wxString fullPath; // Contains file name
    
    wxTextCtrl *associatedTextCtrl; // Will show only file name, path through tooltip
    
    // wxFileDialog properties
    wxString fileDialogMessage;
    wxString fileDialogDefaultDir;
    wxString fileDialogDefaultFile;
    wxString fileDialogWildcard;
    long fileDialogStyle;
    
    // Changes the way OnButton recovers the information from wxFileDialog
    TFileDialogMode fileDialogMode;
    
    void Init();
};


#endif // _FILEBROWSERBUTTON_H_
