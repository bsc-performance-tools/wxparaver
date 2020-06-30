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

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/ssl.hpp>

#include <wx/uri.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/filefn.h> 

#include "paraverkernelexception.h"
#include "paraverconfig.h"
#include "tutorialsdownload.h"

using boost::asio::ip::tcp;
using std::stringstream;


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
  //TutorialData tmp( 1, "https://tools.bsc.es/paraver-tutorials", "Paraver introduction (MPI)", 1 );
  tutorialsList.push_back( tmp );
  
  tutorialsListUpdated = true;
  
  return tutorialsList;
}


void TutorialsDownload::downloadInstall( const vector<PRV_UINT16>& whichTutorials )
{
  for( vector<PRV_UINT16>::const_iterator it = whichTutorials.begin(); it != whichTutorials.end(); ++it )
  {
    string tutorialFile;
    if( download( findTutorial( *it ), tutorialFile ) )
    {
      if ( install( tutorialFile ) )
      {
        wxRemoveFile( wxString::FromUTF8( tutorialFile.c_str() ) );
      }
    }
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

bool TutorialsDownload::install( const string& tutorialFile ) const
{
  string tutorialsPath = ParaverConfig::getInstance()->getGlobalTutorialsPath();
  
  if( !wxFileName::Mkdir( wxString::FromUTF8( tutorialsPath.c_str() ), 0777, wxPATH_MKDIR_FULL ) )
  {
    wxMessageBox( wxT( "Failed creating directory " ) + wxString::FromUTF8( tutorialsPath.c_str() ), wxT( "Install failed" ), wxICON_ERROR );
    return false;
  }
  wxString command = wxT( "tar xf " ) + wxString::FromUTF8( tutorialFile.c_str() ) + wxT( " --directory " ) + wxString::FromUTF8( tutorialsPath.c_str() );
  if( wxExecute( command, wxEXEC_SYNC ) != 0 )
  {
    wxMessageBox( wxT( "Failed installing tutorial " ) + wxString::FromUTF8( tutorialFile.c_str() ), wxT( "Install failed" ), wxICON_ERROR );
    return false;
  }

  return true;
}

// Download using client based on https://github.com/alexandruc/SimpleHttpsClient ( boost::ASIO )
class client
{
  public:
    client( boost::asio::io_service& io_service,
            boost::asio::ssl::context& context,
            const std::string& server, const std::string& path,
            ofstream& storeFile )
            : resolver_( io_service ),
              socket_( io_service, context ),
              store_( storeFile )
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
        socket_.set_verify_mode( boost::asio::ssl::verify_peer );
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
            std::cout << "TOTAL BYTES OF FILE: " << header.substr( 16 ) << std::endl;
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
        std::cout << "store tellp: " << store_.tellp() << std::endl;
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
};

bool TutorialsDownload::download( const TutorialData& whichTutorial, string& tutorialFile ) const
{
  wxURI tutorialURI( wxString::FromUTF8( whichTutorial.getUrl().c_str() ) );
  wxString path   = tutorialURI.GetPath();
  wxString server = tutorialURI.GetServer();

  wxFileName outputFilePath( path );
  string downloadPath = ParaverConfig::getInstance()->getParaverConfigDir();
  tutorialFile = downloadPath + string( outputFilePath.GetFullName().mb_str() );
  ofstream storeFile( tutorialFile );

  try
  {
    boost::asio::ssl::context ctx( boost::asio::ssl::context::sslv23 );
    ctx.set_default_verify_paths();

    boost::asio::io_service io_service;
    client c( io_service, ctx, std::string( server.mb_str() ), std::string( path.mb_str() ), storeFile );
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