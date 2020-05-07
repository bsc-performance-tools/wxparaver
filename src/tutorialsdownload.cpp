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


#include "tutorialsdownload.h"
#include <iostream>
#include <wx/wfstream.h>
#include <wx/protocol/http.h>
#include <wx/datstrm.h>
#include <wx/uri.h>
#include <wx/filename.h>

TutorialsDownload *TutorialsDownload::instance = NULL;

TutorialsDownload *TutorialsDownload::getInstance()
{
  if( instance == NULL )
    instance = new TutorialsDownload();
    return instance;
}

TutorialsDownload::TutorialsDownload()
{
  tutorialsListUpdated = false;
}

TutorialsDownload::~TutorialsDownload()
{}

const vector<TutorialData>& TutorialsDownload::getTutorialsList()
{
  if( tutorialsListUpdated )
    return tutorialsList;
    
  // TODO: Download and parse tutorials data from server
  
  // Tutorial data example. Debug purposes.
  TutorialData tmp( 1, "https://tools.bsc.es/sites/default/files/documentation/1.introduction_to_paraver_mpi.tar.gz", "Paraver introduction (MPI)", 1 );
  tutorialsList.push_back( tmp );
  
  tutorialsListUpdated = true;
  
  return tutorialsList;
}


void TutorialsDownload::downloadInstall( const vector<PRV_UINT16>& whichTutorials )
{
  for( vector<PRV_UINT16>::const_iterator it = whichTutorials.begin(); it != whichTutorials.end(); ++it )
  {
    download( findTutorial( *it ) );
  }
}


const TutorialData& TutorialsDownload::findTutorial( PRV_UINT16 whichId ) const
{
  for( vector<TutorialData>::const_iterator it = tutorialsList.begin(); it != tutorialsList.end(); ++it )
  {
    if( it->getId() == whichId )
      return *it;
  }
  
  throw std::exception();
}


void TutorialsDownload::download( const TutorialData& whichTutorial ) const
{
  wxURI tutorialURI( wxString::FromUTF8( whichTutorial.getUrl().c_str() ) );
  wxString path   = tutorialURI.GetPath();
  wxString server = tutorialURI.GetServer();

  wxHTTP http; 
  //http.SetHeader(_T("Content-type"), contentType); 
  http.SetTimeout( 10 );

  wxFileName outputFilePath( path );
  wxFileOutputStream output( wxT( "~/" ) + outputFilePath.GetFullName() );
  wxDataOutputStream store( output );

  if( http.Connect( server, 80 ) ) 
  {
    wxInputStream *stream;
    stream = http.GetInputStream( path );

    if( stream == NULL ) 
    {
      output.Close();
    } 
    else 
    {
      unsigned char buffer[ 1024 ];
      int byteRead;

      // receive stream
      while( !stream->Eof() )
      {
        stream->Read( buffer, sizeof( buffer ) );
        store.Write8( buffer, sizeof( buffer ) );
        byteRead = stream->LastRead();
        if( byteRead <= 0 ) 
        {
          break;
        }
      }

      output.Close();
    }
  } 
  else 
  {
    output.Close();
  }
}
