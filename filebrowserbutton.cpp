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

#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/filename.h>

#include "filebrowserbutton.h"

/*
  ***************************************************************************
  BASE CLASS:  BrowserButton
  ***************************************************************************
*/

BEGIN_EVENT_TABLE( BrowserButton, wxButton )
  EVT_LEFT_DOWN( BrowserButton::OnButton )
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS( BrowserButton, wxButton )

const wxString BrowserButton::DEFAULT_WILDCARD = wxT( "*.*" );

BrowserButton::BrowserButton( wxWindow* parent,
                       wxWindowID id,
                       const wxString& label,
                       const wxPoint& pos,
                       const wxSize& size,
                       long style,

                       wxTextCtrl *whichTextCtrl, // associated text control
                       const wxString& whichDialogMessage,
                       const wxString& whichDialogDefaultDir,
                       long whichDialogStyle, // set wxFileDialog style

                       const wxValidator& validator,
                       const wxString& name )
        : wxButton( parent, id, label, pos, size, style, validator, name ),
          associatedTextCtrl( whichTextCtrl )
{
  // Button enabled only if textCtrl is associated
  Enable( whichTextCtrl != NULL );
}


void BrowserButton::Init()
{
  associatedTextCtrl = NULL;
  fullPath = wxT( "" );
    
  dialogMessage = wxT( "Choose a file" );
  dialogDefaultDir = wxT( "" );
  dialogStyle = 0;
}


void BrowserButton::SetPath( const wxString& whichFullPath )
{
  fullPath = whichFullPath;
}


wxString BrowserButton::GetPath() const
{
  return ( !fullPath.IsEmpty()? fullPath: wxString( wxT("") ) );
}


void BrowserButton::EnableButton( wxTextCtrl* whichTextCtrl )
{
  SetDialogStyle( wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR );
  whichTextCtrl->SetWindowStyle( wxTE_READONLY );
  SetTextBox( whichTextCtrl ); // When association is done, button's enabled 
}


/*
  ***************************************************************************
    FILE BROWSER BUTTON
  ***************************************************************************
*/

BEGIN_EVENT_TABLE( FileBrowserButton, BrowserButton )
  EVT_LEFT_DOWN( FileBrowserButton::OnButton )
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS( FileBrowserButton, BrowserButton )


FileBrowserButton::FileBrowserButton( wxWindow* parent,
                       wxWindowID id,
                       const wxString& label,
                       const wxPoint& pos,
                       const wxSize& size,
                       long style,

                       wxTextCtrl *whichTextCtrl,
                       const wxString& whichDialogMessage,
                       const wxString& whichDialogDefaultDir,
                       const wxString& whichFileDialogDefaultFile,
                       const wxString& whichFileDialogWildcard,
                       long whichFileDialogStyle,

                       const wxValidator& validator,
                       const wxString& name )
        : BrowserButton( parent, id, label, pos, size, style,
                         whichTextCtrl, whichDialogMessage, whichDialogDefaultDir, whichFileDialogStyle,
                         validator, name )
{
}


void FileBrowserButton::Init()
{
  BrowserButton::Init();

  fileDialogDefaultFile = wxT( "" );
  fileDialogWildcard = DEFAULT_WILDCARD;
}


void FileBrowserButton::EnableButton( wxTextCtrl *whichTextCtrl, const wxString& whichWildcard )
{
  SetFileDialogWildcard( whichWildcard );
  BrowserButton::EnableButton( whichTextCtrl ); 
}


void FileBrowserButton::SetPath( const wxString& whichFullPath )
{
  BrowserButton::SetPath( whichFullPath );
  
  wxFileName tmpFileName( whichFullPath );
  wxString path( tmpFileName.GetPath( wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR ) );
  wxString fileName( tmpFileName.GetFullName() );
  
  // Next time OnButton will navigate directly to the dir and file given
  dialogDefaultDir = path;
  fileDialogDefaultFile = fileName;

  associatedTextCtrl->SetToolTip( whichFullPath );
  associatedTextCtrl->SetValue( fileName );
}


void FileBrowserButton::OnButton( wxMouseEvent& event )
{
  wxFileDialog myDialog ( this,
                         dialogMessage,
                         dialogDefaultDir,
                         fileDialogDefaultFile,
                         fileDialogWildcard, 
                         dialogStyle );
  if ( myDialog.ShowModal() == wxID_OK )
  {
    SetPath( myDialog.GetPath() );
  }
}

/*
  ***************************************************************************
    DIR BROWSER BUTTON
  ***************************************************************************
*/

BEGIN_EVENT_TABLE( DirBrowserButton, BrowserButton )
  EVT_LEFT_DOWN( DirBrowserButton::OnButton )
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS( DirBrowserButton, BrowserButton )


DirBrowserButton::DirBrowserButton( wxWindow* parent,
                       wxWindowID id,
                       const wxString& label,
                       const wxPoint& pos,
                       const wxSize& size,
                       long style,

                       wxTextCtrl *whichTextCtrl,
                       const wxString& whichDialogMessage,
                       const wxString& whichDialogDefaultDir,
                       long whichFileDialogStyle,

                       const wxValidator& validator,
                       const wxString& name )
        : BrowserButton( parent, id, label, pos, size, style,
                         whichTextCtrl, whichDialogMessage, whichDialogDefaultDir, whichFileDialogStyle,
                         validator, name )
{
}


void DirBrowserButton::SetPath( const wxString& whichFullPath )
{
  BrowserButton::SetPath( whichFullPath );

  // Next time OnButton will navigate directly to the dir
  dialogDefaultDir = whichFullPath;

  associatedTextCtrl->SetToolTip( whichFullPath );
  associatedTextCtrl->SetValue( whichFullPath );

}


void DirBrowserButton::OnButton( wxMouseEvent& event )
{
  wxDirDialog myDialog ( this,
                         dialogMessage,
                         dialogDefaultDir,
                         dialogStyle );
  if ( myDialog.ShowModal() == wxID_OK )
  {
    SetPath( myDialog.GetPath() );
  }
}
