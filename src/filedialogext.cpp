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


bool FileDialogExtension::canWriteDir( wxString whichFile )
{
  wxFileName auxDir( whichFile );
  bool canWrite = wxFileName::IsDirWritable( auxDir.GetPathWithSep() );
  if ( !canWrite )
  {
    wxString msg;
    msg.Printf( _("Unable to write '%s'.\n\nPlease check directory permissions."), whichFile.c_str());
    wxMessageDialog message( this, msg, _("Warning"), wxOK | wxICON_WARNING );
    message.ShowModal();
  }

  return canWrite;
}


bool FileDialogExtension::canWriteFile( wxString whichFile )
{
  bool rewrite = false;
  bool exists = wxFileName::FileExists( whichFile );

  if ( exists )
  { 
    wxString msg;
    msg.Printf( _("File '%s' already exists.\n\nProceed to rewrite it?"), whichFile.c_str());
    wxMessageDialog dlg( this, msg, _("Confirm"), wxYES_NO | wxICON_QUESTION );

    rewrite = dlg.ShowModal() == wxID_YES;
  }

  return ( !exists || rewrite );
}


int FileDialogExtension::ShowModal()
{
  bool isValidName = false;
  wxString suffix;
  wxString auxPath;

  // /aa/bb/ != /aa/bb/kk.txt ; Is a path or a file?
  if ( wxFileName( path ).GetPath() != path )
  {
    //path = wxFileName( path ).GetPath();
    SetDirectory( wxFileName( path ).GetPath() );
  }

  while ( !isValidName && wxFileDialog::ShowModal() == wxID_OK )
  {
    // Get suffix
    suffix = extensions[ 0 ]; // by default
    if ( wxFileDialog::GetFilterIndex() < (int)extensions.size() )
    {
      suffix = extensions[ wxFileDialog::GetFilterIndex() ];
    }

    // Suffix not written by the user? Append it.
    auxPath = wxFileDialog::GetPath();
    if( !auxPath.EndsWith( _(".") + suffix ) )
    {
      auxPath += _(".") + suffix;
    }

    isValidName = canWriteDir( auxPath ) && canWriteFile( auxPath );
  }

  if ( isValidName )
    path = auxPath;

  return ( isValidName? wxID_OK : wxID_CANCEL );
}
