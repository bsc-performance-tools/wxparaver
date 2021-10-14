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


#include <wx/window.h>
#include <wx/filedlg.h>

#include <vector>

// Addings to FileDialog:
// - If missing, file extension is automatically added
// - Checks if directory is writable and file exists

// DON'T include dots in the extensions vector
// ( i.e. good:["png","jpg"], bad [".png",".jpg"])

class FileDialogExtension : public wxFileDialog
{
  public:
    FileDialogExtension( wxWindow* parent,
                         const wxString& message = wxT("Choose a file"),
                         const wxString& defaultDir = wxT(""),
                         const wxString& defaultFile = wxT(""),
                         const wxString& wildcard = wxT( "*.*"),
                         long style = wxFD_DEFAULT_STYLE,
                         const wxPoint& pos = wxDefaultPosition,
                         const wxSize& sz = wxDefaultSize,
                         const wxString& name = wxT("filedlg"),
                         // parameters added to wxFileDialog
                         const std::vector< wxString >& whichExtensions = std::vector< wxString >() );

    ~FileDialogExtension() {};

    int ShowModal();
    wxString GetPath() const { return path; }

  private:
    wxString path;
    std::vector< wxString > extensions;

    bool canWriteDir( wxString whichFile );
    bool canWriteFile( wxString whichFile );
};


