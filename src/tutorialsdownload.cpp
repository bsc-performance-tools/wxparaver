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

#include <boost/asio.hpp>

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <sstream>
#include <boost/bind.hpp>
#include <boost/asio/ssl.hpp>

#include <wx/uri.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/filefn.h>
#include <wx/progdlg.h>

#include "paraverkernelexception.h"
#include "paraverconfig.h"
#include "tutorialsdownload.h"
#include "wxparaverapp.h"

using boost::asio::ip::tcp;
using std::stringstream;

std::string TutorialsDownload::tutorialsListFile = "tutorials_list.xml";


TutorialsProgress::TutorialsProgress( wxString& title,
                                      wxString& message,
                                      unsigned int whichNumTutorials ) :
        numTutorials( whichNumTutorials ),
        currentTutorial( 0 ),
        currentDownloadSize( 0 ),
        currentInstallSize( 0 )
{
  dialog = new wxProgressDialog( title, message, numTutorials * 100, NULL, wxPD_APP_MODAL|wxPD_AUTO_HIDE|wxPD_ELAPSED_TIME );
}

TutorialsProgress::~TutorialsProgress()
{
  if( dialog != NULL )
    delete dialog;
}

void TutorialsProgress::nextTutorial()
{
  ++currentTutorial;
}

void TutorialsProgress::setMessage( wxString& whichMessage )
{
  dialog->Update( 100 * currentTutorial, whichMessage );
  dialog->Layout();
}

void TutorialsProgress::setCurrentDownloadSize( unsigned int whichSize )
{
  currentDownloadSize = whichSize;
}

void TutorialsProgress::setCurrentInstallSize( unsigned int whichSize )
{
  currentInstallSize = whichSize;
}

void TutorialsProgress::updateDownload( int whichValue )
{
  int newValue = 100 * currentTutorial;

  newValue += ( (double)whichValue / (double)currentDownloadSize ) * 90.0;

  dialog->Update( newValue );
}

void TutorialsProgress::updateInstall( int whichValue )
{
  int newValue = ( 100 * currentTutorial ) + 90;

  newValue += ( (double)whichValue / (double)currentInstallSize ) * 10.0;

  dialog->Update( newValue );
}


// Download using client based on https://github.com/alexandruc/SimpleHttpsClient ( boost::ASIO )
class client
{
  public:
    client( boost::asio::io_service& io_service,
            boost::asio::ssl::context& context,
            const std::string& server, const std::string& path,
            ofstream& storeFile,
            TutorialsProgress *progress )
            : resolver_( io_service ),
              socket_( io_service, context ),
              store_( storeFile ),
              progress_( progress )
    {

        // Form the request. We specify the "Connection: close" header so that the
        // server will close the socket after transmitting the response. This will
        // allow us to treat all data up until the EOF as the content.
        std::ostream request_stream( &request_ );
        request_stream << "GET " << path << " HTTP/1.0\r\n";
        request_stream << "Host: " << server << "\r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Connection: close\r\n\r\n";

        // Start an asynchronous resolve to translate the server and service names
        // into a list of endpoints.
        tcp::resolver::query query( server, "https" );
        resolver_.async_resolve( query,
                                 boost::bind( &client::handle_resolve,
                                              this,
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::iterator ) );
    }

  private:

    void handle_resolve( const boost::system::error_code& err,
                         tcp::resolver::iterator endpoint_iterator )
    {
      if ( !err )
      {
        socket_.set_verify_mode( boost::asio::ssl::verify_none );
        socket_.set_verify_callback( boost::bind( &client::verify_certificate, this, _1, _2 ) );

        boost::asio::async_connect( socket_.lowest_layer(), 
                                    endpoint_iterator,
                                    boost::bind( &client::handle_connect, 
                                                 this,
                                                 boost::asio::placeholders::error ) );
      }
      else
      {
        throw ParaverKernelException( ParaverKernelException::downloadFailed, err.message().c_str() );
      }
    }

    bool verify_certificate( bool preverified,
                             boost::asio::ssl::verify_context& ctx )
    {
      // The verify callback can be used to check whether the certificate that is
      // being presented is valid for the peer. For example, RFC 2818 describes
      // the steps involved in doing this for HTTPS. Consult the OpenSSL
      // documentation for more details. Note that the callback is called once
      // for each certificate in the certificate chain, starting from the root
      // certificate authority.

      // Get subject name.
      char subject_name[ 256 ];
      X509* cert = X509_STORE_CTX_get_current_cert( ctx.native_handle() );
      X509_NAME_oneline( X509_get_subject_name( cert ), subject_name, 256 ) ;

      return preverified;
    }

    void handle_connect( const boost::system::error_code& err )
    {
      if ( !err )
      {
        socket_.async_handshake( boost::asio::ssl::stream_base::client,
                                 boost::bind( &client::handle_handshake,
                                              this,
                                              boost::asio::placeholders::error ) );
      }
      else
      {
        throw ParaverKernelException( ParaverKernelException::downloadFailed, err.message().c_str() );
      }
    }

    void handle_handshake( const boost::system::error_code& err )
    {
      if ( !err )
      {
        // The handshake was successful. Send the request.
        boost::asio::async_write( socket_,
                                  request_,
                                  boost::bind( &client::handle_write_request, 
                                               this,
                                               boost::asio::placeholders::error ) );
      }
      else
      {
        throw ParaverKernelException( ParaverKernelException::downloadFailed, err.message().c_str() );
      }
    }

    void handle_write_request( const boost::system::error_code& err )
    {
      if ( !err )
      {
        // Read the response status line. The response_ streambuf will
        // automatically grow to accommodate the entire line. The growth may be
        // limited by passing a maximum size to the streambuf constructor.
        boost::asio::async_read_until( socket_, 
                                       response_, 
                                       "\r\n",
                                       boost::bind( &client::handle_read_status_line,
                                                    this,
                                                    boost::asio::placeholders::error ) );
      }
      else
      {
        throw ParaverKernelException( ParaverKernelException::downloadFailed, err.message().c_str() );
      }
    }

    void handle_read_status_line( const boost::system::error_code& err )
    {
      if ( !err )
      {
        // Check that response is OK.
        std::istream response_stream( &response_ );
        std::string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        std::string status_message;
        std::getline( response_stream, status_message );
        if ( !response_stream || http_version.substr( 0, 5 ) != "HTTP/" )
        {
          throw ParaverKernelException( ParaverKernelException::downloadFailed, "Invalid response" );
        }
        if ( status_code != 200 )
        {
          stringstream tmpStr;
          tmpStr << status_code;
          throw ParaverKernelException( ParaverKernelException::downloadFailed, string( "Response returned with status code " + tmpStr.str() ).c_str() );
        }

        // Read the response headers, which are terminated by a blank line.
        boost::asio::async_read_until( socket_,
                                       response_,
                                       "\r\n\r\n",
                                       boost::bind( &client::handle_read_headers,
                                                    this,
                                                    boost::asio::placeholders::error ) );
      }
      else
      {
        throw ParaverKernelException( ParaverKernelException::downloadFailed, err.message().c_str() );
      }
    }

    void handle_read_headers( const boost::system::error_code& err )
    {
      if ( !err )
      {
        // Process the response headers.
        std::istream response_stream( &response_ );
        std::string header;
        while ( std::getline( response_stream, header ) && header != "\r" )
        {
          if( !header.compare( 0, 15, "Content-Length:" ) )
          {
            stringstream tmpSstr( header.substr( 16 ) );
            tmpSstr >> totalBytes_;
            if( progress_ != NULL )
              progress_->setCurrentDownloadSize( totalBytes_ );
          }
        } 
        // Write whatever content we already have to output.
        if ( response_.size() > 0 )
          store_ << &response_;

        // Start reading remaining data until EOF.
        boost::asio::async_read( socket_,
                                 response_,
                                 boost::asio::transfer_at_least(1),
                                 boost::bind( &client::handle_read_content,
                                              this,
                                              boost::asio::placeholders::error ) );
      }
      else
      {
        throw ParaverKernelException( ParaverKernelException::downloadFailed, err.message().c_str() );
      }
    }

    void handle_read_content( const boost::system::error_code& err )
    {
      if ( !err )
      {
        // Write all of the data that has been read so far.
        store_ << &response_;

        if( progress_ != NULL )
          progress_->updateDownload( store_.tellp() );
          
        // Continue reading remaining data until EOF.
        boost::asio::async_read( socket_, 
                                 response_,
                                 boost::asio::transfer_at_least(1),
                                 boost::bind( &client::handle_read_content, 
                                              this,
                                              boost::asio::placeholders::error ) );
      }
      else if ( err != boost::asio::error::eof )
      {
        throw ParaverKernelException( ParaverKernelException::downloadFailed, err.message().c_str() );
      }
    }

    tcp::resolver resolver_;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
    boost::asio::streambuf request_;
    boost::asio::streambuf response_;
    ofstream& store_;
    int totalBytes_;
    TutorialsProgress *progress_;
};


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

void TutorialsDownload::writeList( std::string& fullPath )
{
  TutorialData tmp( 1, "https://tools.bsc.es/sites/default/files/documentation/1.introduction_to_paraver_mpi.tar.gz", "Paraver introduction (MPI)", 1 );
  tutorialsList.push_back( tmp );
  TutorialData tmp2( 2, "https://tools.bsc.es/sites/default/files/documentation/2.introduction_to_dimemas.tar.gz", "Dimemas introduction", 1 );
  tutorialsList.push_back(tmp2);
  TutorialData tmp3( 3, "https://tools.bsc.es/sites/default/files/documentation/3.introduction_to_paraver_and_dimemas_methodology.tar.gz", "Introduction to Paraver and Dimemas methodology", 1 );
  tutorialsList.push_back(tmp3);
  TutorialData tmp4( 4, "https://tools.bsc.es/sites/default/files/documentation/4.methodology_of_analysis.tar.gz", "Methodology", 1 );
  tutorialsList.push_back(tmp4);
  TutorialData tmp5( 5, "https://tools.bsc.es/sites/default/files/documentation/5.tutorial_on_hydroc.tar.gz", "Tutorial on HydroC analysis (MPI, Dimemas, CUDA)", 1 );
  tutorialsList.push_back(tmp5);
  TutorialData tmp6( 6, "https://tools.bsc.es/sites/default/files/documentation/6.paraver_trace_preparation.tar.gz", "Trace preparation", 1 );
  tutorialsList.push_back(tmp6);
  //TutorialData tmp7( 7, "https://tools.bsc.es/sites/default/files/documentation/trace_alignment.tar.gz", "Trace alignment tutorial", 1 );
  //tutorialsList.push_back(tmp7);

  std::ofstream ofs( fullPath.c_str() );
  if( ofs.good() )
  {
    boost::archive::xml_oarchive oa( ofs );
    oa << boost::serialization::make_nvp( "tutorials", *this );
  }
  ofs.close();
}


vector<TutorialData> TutorialsDownload::getTutorialsList()
{
  if( !tutorialsListUpdated )
  {
    downloadTutorialsList();
    loadXML( ParaverConfig::getInstance()->getParaverConfigDir() + TutorialsDownload::tutorialsListFile, this, "tutorials" );
    tutorialsListUpdated = true;
  }

  vector<TutorialData> tutorialsListState = tutorialsList;
  vector<TutorialData> installedTutorials;
  loadXML( ParaverConfig::getInstance()->getGlobalTutorialsPath() + PATH_SEP + TutorialsDownload::tutorialsListFile,
           installedTutorials,
           "installed_tutorials" );

  for( vector<TutorialData>::iterator it = tutorialsListState.begin(); it != tutorialsListState.end(); ++it )
  {
    for( vector<TutorialData>::const_iterator itInstalled = installedTutorials.begin(); itInstalled != installedTutorials.end(); ++itInstalled )
    {
      if( itInstalled->getId() == it->getId() )
      {
        if( itInstalled->getVersion() < it->getVersion() )
          it->setName( "[NEW VERSION] " + it->getName() );
        else
          it->setName( "[INSTALLED] " + it->getName() );

        break;
      }
    }
  }

  return tutorialsListState;
}


void TutorialsDownload::downloadInstall( const vector<PRV_UINT16>& whichTutorials )
{
  wxString myTitle( wxT( "Download and install tutorials" ) );
  wxString myMessage( wxT( "" ) );
  TutorialsProgress progress( myTitle, myMessage, whichTutorials.size() );

  vector<TutorialData> installedTutorials;

  loadXML( ParaverConfig::getInstance()->getGlobalTutorialsPath() + PATH_SEP + TutorialsDownload::tutorialsListFile, 
           installedTutorials,
           "installed_tutorials" );

  for( vector<PRV_UINT16>::const_iterator it = whichTutorials.begin(); it != whichTutorials.end(); ++it )
  {
    string tutorialFile;
    TutorialData data = findTutorial( *it );

    myMessage = wxString::FromUTF8( data.getName().c_str() );
    progress.setMessage( myMessage );
    if( download( data, tutorialFile, progress ) )
    {
      if ( install( tutorialFile, progress ) )
      {
        wxRemoveFile( wxString::FromUTF8( tutorialFile.c_str() ) );
        
        vector<TutorialData>::iterator itInstalled;
        for( itInstalled = installedTutorials.begin(); itInstalled != installedTutorials.end(); ++itInstalled )
        {
          if( itInstalled->getId() == data.getId() )
          {
            itInstalled->setVersion( data.getVersion() );
            break;
          }
        }

        if ( itInstalled == installedTutorials.end() )
        {
          installedTutorials.push_back( data );
        }
        
        progress.nextTutorial();
      }
    }
  }

  string tmpStr( ParaverConfig::getInstance()->getGlobalTutorialsPath() + PATH_SEP + TutorialsDownload::tutorialsListFile );
  std::ofstream ofs( tmpStr.c_str() );
  if( ofs.good() )
  {
    boost::archive::xml_oarchive oa( ofs );
    oa << boost::serialization::make_nvp( "installed_tutorials", installedTutorials );
  }
  ofs.close();
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


void TutorialsDownload::loadXML( const std::string& whichFilename, TutorialsDownload *whichTutorials, const std::string& whichTag )
{
  std::ifstream ifs( whichFilename.c_str() );
  if( ifs.good() )
  {
    boost::archive::xml_iarchive ia( ifs );
    ia >> boost::serialization::make_nvp( whichTag.c_str(), *whichTutorials );
  }
  ifs.close();
}


void TutorialsDownload::loadXML( const std::string& whichFilename, vector<TutorialData>& whichTutorials, const std::string& whichTag )
{
  std::ifstream ifs( whichFilename.c_str() );
  if( ifs.good() )
  {
    boost::archive::xml_iarchive ia( ifs );
    ia >> boost::serialization::make_nvp( whichTag.c_str(), whichTutorials );
  }
  ifs.close();
}


bool TutorialsDownload::downloadTutorialsList() const
{
  std::string path   = "/sites/default/files/documentation/" + TutorialsDownload::tutorialsListFile;
  std::string server = "tools.bsc.es";

  std::string tutorialFile = ParaverConfig::getInstance()->getParaverConfigDir() + TutorialsDownload::tutorialsListFile;
  ofstream storeFile( tutorialFile.c_str() );

  try
  {
    boost::asio::ssl::context ctx( boost::asio::ssl::context::sslv23 );
    ctx.set_default_verify_paths();

    boost::asio::io_service io_service;
    client c( io_service, ctx, server, path, storeFile, NULL );
    io_service.run();
  }
  catch ( ParaverKernelException& e )
  {
    wxMessageBox( wxString::FromUTF8( e.what() ), wxT( "Download failed" ), wxICON_ERROR );
    storeFile.close();

    return false;
  }
  
  storeFile.close();

  return true;
}


bool TutorialsDownload::download( const TutorialData& whichTutorial, string& tutorialFile, TutorialsProgress& progress ) const
{
  wxURI tutorialURI( wxString::FromUTF8( whichTutorial.getUrl().c_str() ) );
  wxString path   = tutorialURI.GetPath();
  wxString server = tutorialURI.GetServer();

  wxFileName outputFilePath( path );
  string downloadPath = ParaverConfig::getInstance()->getParaverConfigDir();
  tutorialFile = downloadPath + string( outputFilePath.GetFullName().mb_str() );
#ifdef WIN32
  wxString tmpHome;
  if ( !wxparaverApp::mainWindow->getParaverHome( tmpHome ) )
    return false;
    
  wxString command = tmpHome + wxT( "\\bin\\wget.exe --no-check-certificate -P " ) + wxString::FromUTF8( downloadPath.c_str() ) + wxT( " " ) + wxString::FromUTF8( whichTutorial.getUrl().c_str() );
  if( wxExecute( command, wxEXEC_SYNC ) != 0 )
  {
    wxMessageBox( wxT( "Failed downloading tutorial " ) + wxString::FromUTF8( whichTutorial.getUrl().c_str() ), wxT( "Download failed" ), wxICON_ERROR );
    return false;
  }
#else
  ofstream storeFile( tutorialFile.c_str() );
  try
  {
    boost::asio::ssl::context ctx( boost::asio::ssl::context::sslv23 );
    ctx.set_default_verify_paths();

    boost::asio::io_service io_service;
    client c( io_service, ctx, std::string( server.mb_str() ), std::string( path.mb_str() ), storeFile, &progress );
    io_service.run();
  }
  catch ( ParaverKernelException& e )
  {
    wxMessageBox( wxString::FromUTF8( e.what() ), wxT( "Download failed" ), wxICON_ERROR );
    storeFile.close();

    return false;
  }
  storeFile.close();
#endif

  return true;
}


bool TutorialsDownload::install( const string& tutorialFile, TutorialsProgress& progress ) const
{
  string tutorialsPath = ParaverConfig::getInstance()->getGlobalTutorialsPath();
  
  progress.setCurrentInstallSize( 1 );

  if( !wxFileName::Mkdir( wxString::FromUTF8( tutorialsPath.c_str() ), 0777, wxPATH_MKDIR_FULL ) )
  {
    wxMessageBox( wxT( "Failed creating directory " ) + wxString::FromUTF8( tutorialsPath.c_str() ), wxT( "Install failed" ), wxICON_ERROR );
    return false;
  }
#ifdef WIN32
  wxString tmpHome;
  if ( !wxparaverApp::mainWindow->getParaverHome( tmpHome ) )
    return false;
  wxString command = tmpHome + wxT( "\\bin\\gzip.exe -k -d -f " ) + wxString::FromUTF8( tutorialFile.c_str() );
  if( wxExecute( command, wxEXEC_SYNC ) != 0 )
  {
    wxMessageBox( wxT( "Failed unzipping file " ) + wxString::FromUTF8( tutorialFile.c_str() ), wxT( "Install failed" ), wxICON_ERROR );
    return false;
  }
  command = tmpHome + wxT( "\\bin\\tar.exe --force-local -xf " ) + wxString::FromUTF8( tutorialFile.substr( 0, tutorialFile.size() - 3 ).c_str() ) +
            wxT( " --one-top-level=" ) + wxString::FromUTF8( tutorialsPath.c_str() );
#else
  wxString command = wxT( "tar xf " ) + wxString::FromUTF8( tutorialFile.c_str() ) +
                     wxT( " --directory " ) + wxString::FromUTF8( tutorialsPath.c_str() );
#endif
  if( wxExecute( command, wxEXEC_SYNC ) != 0 )
  {
    wxMessageBox( wxT( "Failed installing tutorial " ) + wxString::FromUTF8( tutorialFile.c_str() ), wxT( "Install failed" ), wxICON_ERROR );
    return false;
  }

  progress.updateInstall( 1 );

  return true;
}
