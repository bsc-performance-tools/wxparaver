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

using namespace std;

string SessionSaver::v2Label( "#Paraver session file v2" );

void SessionSaver::SaveSession( wxString onFile, const vector<Trace *>& traces )
{
  if ( traces.size() > 0 )
    SessionSaver::SaveSession_v2( onFile, traces );
/* version 1 DEPRECATED
  ofstream file( onFile.mb_str() );
  
  for( vector<Trace *>::const_iterator it = traces.begin(); it != traces.end(); ++it )
  {
    file << (*it)->getFileName() << endl;
    wxFileName path( onFile.c_str() );
    wxFileName traceFileName( wxString::FromUTF8( (*it)->getFileName().c_str() ) );
    wxFileName cfgFileName( path.GetPathWithSep() + traceFileName.GetFullName() + wxT( ".cfg" ) );
    
    vector<Timeline *> vTimelines, tmpVTimelines;
    vector<Histogram *> vHistograms;
    LoadedWindows::getInstance()->getAll( *it, tmpVTimelines );
    LoadedWindows::getInstance()->getAll( *it, vHistograms );

    for( vector<Timeline *>::iterator it = tmpVTimelines.begin(); it != tmpVTimelines.end(); ++it )
    {
      if( !(*it)->getUsedByHistogram() && (*it)->getChild() == nullptr )
        vTimelines.push_back( *it );
    }
    CFGLoader::saveCFG( string( cfgFileName.GetFullPath().mb_str() ), SaveOptions(), vTimelines, vHistograms );
  }
  
  file.close();
*/
}

void SessionSaver::SaveSession_v2( wxString onFile, const vector<Trace *>& traces )
{
  ofstream file( onFile.mb_str() );
  wxFileName dirName( onFile );
  dirName.ClearExt();
  dirName = wxFileName( dirName.GetFullPath() + wxString( wxT( "_session" ) ) );
  wxFileName::Mkdir( dirName.GetFullPath(), 0777, wxPATH_MKDIR_FULL );

  file << SessionSaver::v2Label <<endl;

  for( vector<Trace *>::const_iterator it = traces.begin(); it != traces.end(); ++it )
  {
    wxFileName traceFileName( wxString::FromUTF8( (*it)->getFileName().c_str() ) );
    traceFileName.MakeRelativeTo( wxFileName::GetHomeDir() );
    file << std::string( traceFileName.GetFullPath().mb_str() ) << endl;
    wxFileName cfgFileName( dirName.GetFullPath() + 
                            wxFileName::GetPathSeparator() +
                            traceFileName.GetFullName() + wxT( ".cfg" ) );
    
    vector<Timeline *> vTimelines, tmpVTimelines;
    vector<Histogram *> vHistograms;
    LoadedWindows::getInstance()->getAll( *it, tmpVTimelines );
    LoadedWindows::getInstance()->getAll( *it, vHistograms );

    // Delete timelines belonging to derived windows
    for( vector<Timeline *>::iterator itWin = tmpVTimelines.begin(); itWin != tmpVTimelines.end(); ++itWin )
    {
      if( !(*itWin)->getUsedByHistogram() && (*itWin)->getChild() == nullptr )
        vTimelines.push_back( *itWin );
    }

    // TODO: search for each linked properties manager in the timelines and histograms vectors
    CFGS4DLinkedPropertiesManager dummyManager;
    vector<CFGS4DLinkedPropertiesManager> dummyList;
    dummyList.push_back( dummyManager );
    CFGLoader::saveCFG( string( cfgFileName.GetFullPath().mb_str() ), SaveOptions(), vTimelines, vHistograms, dummyList );
  }

  file.close();
}

void SessionSaver::LoadSession( wxString whichFile )
{
  ifstream file( whichFile.mb_str() );
  string traceFile;
  bool opened;
  
  getline( file, traceFile );
  if( traceFile == SessionSaver::v2Label )
  {
    SessionSaver::LoadSession_v2( file, whichFile );
    return;
  }
  
  file.seekg( 0 );
  while( !file.eof() )
  {
    getline( file, traceFile );
    if( traceFile != "" && traceFile[ 0 ] != '#' ) 
    {
      opened = wxparaverApp::mainWindow->DoLoadTrace( traceFile );

      if ( opened )
      {
        wxFileName path( whichFile.c_str() );
        wxFileName traceFileName( wxString::FromUTF8( traceFile.c_str() ) );
        wxFileName cfgFileName( path.GetPathWithSep() + traceFileName.GetFullName() + wxT( ".cfg" ) );
        
        wxparaverApp::mainWindow->DoLoadCFG( string( cfgFileName.GetFullPath().mb_str() ) );
      }
    }
  }
  
  file.close();
}

void SessionSaver::LoadSession_v2( ifstream& whichFile, wxString filename  )
{
  wxFileName dirName( filename );
  dirName.ClearExt();
  dirName = wxFileName( dirName.GetFullPath() + wxString( wxT( "_session" ) ) );
  string traceFile;
  bool opened;

  while( !whichFile.eof() )
  {
    getline( whichFile, traceFile );
    if( traceFile != "" && traceFile[ 0 ] != '#' ) 
    {
      wxFileName traceFileName( wxString::FromUTF8( traceFile.c_str() ) );
      traceFileName.MakeAbsolute( wxFileName::GetHomeDir() );
      opened = wxparaverApp::mainWindow->DoLoadTrace( std::string( traceFileName.GetFullPath().mb_str() ) );

      if ( opened )
      {
        wxFileName cfgFileName( dirName.GetFullPath() + 
                                wxFileName::GetPathSeparator() +
                                traceFileName.GetFullName() + wxT( ".cfg" ) );
        
        wxparaverApp::mainWindow->DoLoadCFG( string( cfgFileName.GetFullPath().mb_str() ) );
      }
    }
  }
  
  whichFile.close();
}
