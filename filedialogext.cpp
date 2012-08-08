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

#include "filedialogext.h"

#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/datetime.h>

FileDialogExtension::FileDialogExtension( wxWindow* parent,
                                          const wxString& message,
                                          const wxString& defaultDir,
                                          const wxString& defaultFile,
                                          const wxString& wildcard,
                                          long style,
                                          const wxPoint& pos,
                                          const wxSize& sz,
                                          const wxString& name,
                                          const std::vector< wxString >& whichExtensions ) :
        wxFileDialog( parent, message, defaultDir, defaultFile, wildcard, style, pos, sz, name )
                         
{
  extensions = whichExtensions;
  path = defaultDir;
}


int FileDialogExtension::ShowModal()
{
  wxString auxPath( wxFileDialog::GetPath() );
  wxFileName auxName;
  wxString suffix;
  
  bool cancelDialog = false;
  bool validName = false;
  while ( !cancelDialog && !validName )
  {
    if( wxFileDialog::ShowModal() == wxID_OK )
    {
      auxPath = wxFileDialog::GetPath();

      // Know which kind of type are we working with --> extension?
      if ( wxFileDialog::GetFilterIndex() > (int)extensions.size() )
      {
        // Error!? Put first one
        suffix = extensions[ 0 ]; 
      }
      else
      {
        suffix = extensions[ wxFileDialog::GetFilterIndex() ];      
      }
      
      // Guarantee that finishes with proper extension
      if( !auxPath.EndsWith( suffix ) )
      {
        auxPath += suffix;
      
        // Does file exists?
        auxName = wxFileName( auxPath );

        if ( !auxName.FileExists() )
        {
          // After adding extension, we see that it doesn't exist; got it!
          validName = true;
          path = auxPath;
        }
        else
        {
          wxString msg;

          msg.Printf( _("File '%s' already exists, do you really want to overwrite it?"),
                      auxPath.c_str());

          wxMessageDialog dlg( this, msg, _("Confirm"), wxYES_NO | wxICON_QUESTION );
          if ( dlg.ShowModal() != wxID_YES )
          {
            // Don't exit and show the dialog again
            SetPath( auxPath );
          }
          else
          {
            // Save the path and exit
            validName = true;
            path = auxPath;
          }
        }
      }
      else
      {
        // It ends with proper suffix and 
        // it was accepted by previous overwrite dialog opened by wxFileDialog 
        validName = true;
        path = auxPath;
      }
    }
    else
    {
      cancelDialog = true;
    }
  }
  
  return ( validName? wxID_OK : wxID_CANCEL );
}
