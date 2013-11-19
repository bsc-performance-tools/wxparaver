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

#include "filebrowserbutton.h"

BEGIN_EVENT_TABLE( FileBrowserButton, wxButton )
  EVT_LEFT_DOWN( FileBrowserButton::OnButton )
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS( FileBrowserButton, wxButton )


void FileBrowserButton::Init()
{
  myTextCtrl = NULL;
    
  fileDialogMessage = wxT( "Choose a file" );
  fileDialogDefaultDir = wxT( "" );
  fileDialogDefaultFile = wxT( "" );
  fileDialogWildcard = wxT( "" );
  fileDialogStyle = 0;
    
  fileDialogMode = FILE_MODE;
}


FileBrowserButton::FileBrowserButton( wxWindow* parent,
                       wxWindowID id,
                       wxTextCtrl *whichTextCtrl,
                       const wxString& whichFileDialogMessage,
                       const wxString& whichFileDialogDefaultDir,
                       const wxString& whichFileDialogDefaultFile,
                       const wxString& whichFileDialogWildcard,
                       long whichFileDialogStyle,
                       TFileDialogMode whichFileDialogMode,
                       const wxString& label,
                       const wxPoint& pos,
                       const wxSize& size,
                       long style,
                       const wxValidator& validator,
                       const wxString& name )
        : wxButton( parent, id, label, pos, size, style, validator, name ), myTextCtrl( whichTextCtrl ) 
{
  // Idea: button enabled only if textCtrl is associated
  Enable( whichTextCtrl != NULL );
}


wxString FileBrowserButton::GetPath() const
{
  // First approach: dummy GetPath
  wxString path;
  
  if ( myTextCtrl != NULL )
  {
    path = myTextCtrl->GetValue();
  }
  
  return path;
}


void FileBrowserButton::OnButton( wxMouseEvent& event )
{
  wxFileDialog myDialog( this,
                         fileDialogMessage,
                         fileDialogDefaultDir,
                         fileDialogDefaultFile,
                         fileDialogWildcard, 
                         fileDialogStyle );
  
  if ( myDialog.ShowModal() == wxID_OK )
  {
    switch ( fileDialogMode )
    {
      case DIRECTORY_MODE:
        myTextCtrl->SetValue( myDialog.GetDirectory() );
        break;
        
      case FILE_MODE:
      default:
        myTextCtrl->SetValue( myDialog.GetFilename() );
        break;
    }
  }
}


