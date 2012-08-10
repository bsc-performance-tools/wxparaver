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

#ifndef _FILEDIALOGEXT_H_ 
#define _FILEDIALOGEXT_H_

#include <wx/window.h>
#include <wx/filedlg.h>

#include <vector>

// The purpose of this class is to let the user write the file to be saved with or without
// its defined extension, automatically generating suffixed name and taking care if it exists.

class FileDialogExtension : public wxFileDialog
{
  public:
    FileDialogExtension( // same than wxFileDialog
                         wxWindow* parent,
                         const wxString& message = wxT("Choose a file"),
                         const wxString& defaultDir = wxT(""),
                         const wxString& defaultFile = wxT(""),
                         const wxString& wildcard = wxT( "*.*"),
                         long style = wxFD_DEFAULT_STYLE,
                         const wxPoint& pos = wxDefaultPosition,
                         const wxSize& sz = wxDefaultSize,
                         const wxString& name = wxT("filedlg"),
                         // added extensions
                         const std::vector< wxString >& whichExtensions = std::vector< wxString >() );
                         
    ~FileDialogExtension() {};
    
    int ShowModal();
    wxString GetPath() const { return path; }
    
  private:
    wxString path;
    std::vector< wxString > extensions;
};

#endif // _FILEDIALOGEXT_H_

