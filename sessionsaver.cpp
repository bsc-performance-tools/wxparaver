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

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "sessionsaver.h"
#include <string>
#include <fstream>
#include <wx/filename.h>
#include "loadedwindows.h"
#include "cfg.h"
#include "wxparaverapp.h"
#include "window.h"


void SessionSaver::SaveSession( wxString onFile, const vector<Trace *>& traces )
{
  ofstream file( onFile.mb_str() );
  
  for( vector<Trace *>::const_iterator it = traces.begin(); it != traces.end(); ++it )
  {
    file << (*it)->getFileName() << endl;
    wxFileName path( onFile.c_str() );
    wxFileName traceFileName( wxString::FromAscii( (*it)->getFileName().c_str() ) );
    wxFileName cfgFileName( path.GetPathWithSep() + traceFileName.GetFullName() + wxT( ".cfg" ) );
    
    vector<Window *> vTimelines, tmpVTimelines;
    vector<Histogram *> vHistograms;
    LoadedWindows::getInstance()->getAll( *it, tmpVTimelines );
    LoadedWindows::getInstance()->getAll( *it, vHistograms );

    for( vector<Window *>::iterator it = tmpVTimelines.begin(); it != tmpVTimelines.end(); ++it )
    {
      if( !(*it)->getUsedByHistogram() && (*it)->getChild() == NULL )
        vTimelines.push_back( *it );
    }
    CFGLoader::saveCFG( string( cfgFileName.GetFullPath().mb_str() ), SaveOptions(), vTimelines, vHistograms );
  }
  
  file.close();
}


void SessionSaver::LoadSession( wxString whichFile )
{
  ifstream file( whichFile.mb_str() );
  string traceFile;
  bool opened;
  
  while( !file.eof() )
  {
    getline( file, traceFile );
    if( traceFile != "" && traceFile[ 0 ] != '#' ) 
    {
      opened = wxparaverApp::mainWindow->DoLoadTrace( traceFile );

      if ( opened )
      {
        wxFileName path( whichFile.c_str() );
        wxFileName traceFileName( wxString::FromAscii( traceFile.c_str() ) );
        wxFileName cfgFileName( path.GetPathWithSep() + traceFileName.GetFullName() + wxT( ".cfg" ) );
        
        wxparaverApp::mainWindow->DoLoadCFG( string( cfgFileName.GetFullPath().mb_str() ) );
      }
    }
  }
  
  file.close();
}
