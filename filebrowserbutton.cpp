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
#include <wx/filename.h>

#include "filebrowserbutton.h"

BEGIN_EVENT_TABLE( FileBrowserButton, wxButton )
  EVT_LEFT_DOWN( FileBrowserButton::OnButton )
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS( FileBrowserButton, wxButton )


void FileBrowserButton::Init()
{
  associatedTextCtrl = NULL;
  fullPath = wxT( "" );
    
  fileDialogMessage = wxT( "Choose a file" );
  fileDialogDefaultDir = wxT( "" );
  fileDialogDefaultFile = wxT( "" );
  fileDialogWildcard = wxT( "" );
  fileDialogStyle = 0;
    
  fileDialogMode = FILE_MODE;
}


FileBrowserButton::FileBrowserButton( wxWindow* parent,
                       wxWindowID id,
                       const wxString& label,
                       const wxPoint& pos,
                       const wxSize& size,
                       long style,

                       wxTextCtrl *whichTextCtrl,
                       const wxString& whichFileDialogMessage,
                       const wxString& whichFileDialogDefaultDir,
                       const wxString& whichFileDialogDefaultFile,
                       const wxString& whichFileDialogWildcard,
                       long whichFileDialogStyle,
                       TFileDialogMode whichFileDialogMode,

                       const wxValidator& validator,
                       const wxString& name )
        : wxButton( parent, id, label, pos, size, style, validator, name ), associatedTextCtrl( whichTextCtrl ) 
{
  // Idea: button enabled only if textCtrl is associated
  Enable( whichTextCtrl != NULL );
}


void FileBrowserButton::SetPath( const wxString& whichFullPath )
{
  fullPath = whichFullPath;
  
  wxFileName tmpFileName( whichFullPath );
  wxString path( tmpFileName.GetPath( wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR ) );
  wxString fileName( tmpFileName.GetFullName() );
  
  // Next time OnButton will navigate directly to the dir and file given
  fileDialogDefaultDir = path;
  fileDialogDefaultFile = fileName;

  switch ( fileDialogMode )
  {
    case DIRECTORY_MODE:
      // In this case should be the same than whichFullPath
      associatedTextCtrl->SetToolTip( path );
      associatedTextCtrl->SetValue( path );
      break;
      
    case FILE_MODE:
    default:
      associatedTextCtrl->SetToolTip( whichFullPath );
      associatedTextCtrl->SetValue( fileName );
      break;
  }
}


wxString FileBrowserButton::GetPath() const
{
  return ( !fullPath.IsEmpty()? fullPath: wxString( wxT("") ) );
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
  /*
    std::cout << "dir     :" << myDialog.GetDirectory() << std::endl;
    std::cout << "path    :" << myDialog.GetPath() << std::endl;
    std::cout << "filename:" << myDialog.GetFilename() << std::endl;
    */
    SetPath( myDialog.GetPath() );
  }
}


