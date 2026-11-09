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
#  pragma hdrstop
#endif

#ifndef WX_PRECOMP
#  include "wx/wx.h"
#endif

#include "cfg.h"
#include "histogram.h"
#include "loadedwindows.h"
#include "sessionsaver.h"
#include "window.h"
#include "wxparaverapp.h"

#include <fstream>
#include <string>
#include <wx/filename.h>

using namespace std;

string SessionSaver::v2Label( "#Paraver session file v2" );


std::string SessionSaver::getTraceNameWithoutNumber( const std::string& traceFileName )
{
  auto numericRightTrim = []( const std::string& fileName, const std::string& numericTagDetector ) -> std::string
  {
    auto suffixStartPos = fileName.rfind( numericTagDetector );
    if( suffixStartPos != fileName.npos )
      return fileName.substr( 0, suffixStartPos + numericTagDetector.size() - 2  ); // " #" -> 2

    return "";
  };


  for( auto tag: { ".prv #", ".prv.gz #" } )
  {
    std::string newTraceFileName( numericRightTrim( traceFileName, tag ) );
    if( !newTraceFileName.empty() )
      return newTraceFileName;
  }


  return traceFileName;
}


void SessionSaver::SaveSession( wxString onFile, const vector< Trace * > &traces )
{
  if( traces.size() > 0 )
  {
    ofstream file( onFile.mb_str() );
    wxFileName dirName( onFile );
    dirName.ClearExt();
    dirName = wxFileName( dirName.GetFullPath() + wxString( wxT( "_session" ) ) );
    wxFileName::Mkdir( dirName.GetFullPath(), 0777, wxPATH_MKDIR_FULL );

    file << SessionSaver::v2Label << endl;

    for( vector< Trace * >::const_iterator it = traces.begin(); it != traces.end(); ++it )
    {
      wxFileName traceFileName( wxString::FromUTF8( ( *it )->getFileNameNumbered().c_str() ) );
      traceFileName.MakeRelativeTo( wxFileName::GetHomeDir() );
      file << std::string( traceFileName.GetFullPath().mb_str() ) << endl;
      wxFileName cfgFileName( dirName.GetFullPath() + wxFileName::GetPathSeparator() + traceFileName.GetFullName() + wxT( ".cfg" ) );

      vector< Timeline * > vTimelines, tmpVTimelines;
      vector< Histogram * > vHistograms;
      LoadedWindows::getInstance()->getAll( *it, tmpVTimelines );
      LoadedWindows::getInstance()->getAll( *it, vHistograms );

      // Delete timelines belonging to derived windows
      for( vector< Timeline * >::iterator itWin = tmpVTimelines.begin(); itWin != tmpVTimelines.end(); ++itWin )
      {
        if( !( *itWin )->getUsedByHistogram() && ( *itWin )->getChild() == nullptr )
          vTimelines.push_back( *itWin );
      }

      // TODO: search for each linked properties manager in the timelines and histograms vectors
      CFGS4DLinkedPropertiesManager dummyManager;
      vector< CFGS4DLinkedPropertiesManager > dummyList;
      dummyList.push_back( dummyManager );
      std::ofstream cfgFile( string( cfgFileName.GetFullPath().mb_str() ).c_str() );
      if( cfgFile.good() )
        CFGLoader::saveCFG( cfgFile, SaveOptions(), vTimelines, vHistograms, dummyList );
      cfgFile.close();
    }

    file.close();
  }
}


void SessionSaver::LoadSession( wxString whichFile )
{
  ifstream file( whichFile.mb_str() );
  string traceFile;

  getline( file, traceFile );
  if( traceFile == SessionSaver::v2Label )
  {
    wxFileName dirName( whichFile );
    dirName.ClearExt();
    dirName = wxFileName( dirName.GetFullPath() + wxString( wxT( "_session" ) ) );
    string traceFile;

    while( !file.eof() )
    {
      getline( file, traceFile );
      if( traceFile != "" && traceFile[ 0 ] != '#' )
      {
        // Load trace: strip instance number if found.
        std::string traceFileWithoutNumber = getTraceNameWithoutNumber( traceFile );
        wxFileName traceFileName( wxString::FromUTF8( traceFileWithoutNumber.c_str() ) );
        traceFileName.MakeAbsolute( wxFileName::GetHomeDir() );
        std::optional< Trace * > tmpTrace = wxparaverApp::mainWindow->DoLoadTrace( std::string( traceFileName.GetFullPath().mb_str() ) );

        if( tmpTrace )
        {
          // Load cfg: here instance number, if present, is needed.
          wxFileName traceFileNameInFile( wxString::FromUTF8( traceFile.c_str() ) );
          traceFileNameInFile.MakeAbsolute( wxFileName::GetHomeDir() );
          wxFileName cfgFileName( dirName.GetFullPath() + wxFileName::GetPathSeparator() + traceFileNameInFile.GetFullName() + wxT( ".cfg" ) );

          wxparaverApp::mainWindow->DoLoadCFG( string( cfgFileName.GetFullPath().mb_str() ), tmpTrace );
        }
      }
    }
  }

  file.close();
}

