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

#include "autosessionmanager.h"

// WARNING: CODE WILL ONLY WORK ON UNIX + WX 3.0 >=
// TO DO AFTER FUNCTIONALITY IS DONE: COMPATIBILITY ifdefs


map < unsigned int, AutoSessionManager::SessionItems > AutoSessionManager::sessions;


AutoSessionManager::AutoSessionManager( ) { }

AutoSessionManager::AutoSessionManager( std::vector < unsigned int >& pids )
{   
  SetSessions( pids );
}


void AutoSessionManager::SetSession( unsigned int& pid )
{
  //if ( sessions.find( pid ) == sessions.end() )
  //{
    SessionItems si = SessionItems();
    sessions.insert( { pid, si } );
  //}
}


void AutoSessionManager::SetSessions( std::vector < unsigned int >& pids )
{
  for ( auto pid : pids )
    SetSession( pid );
}


void AutoSessionManager::SaveAutoSession( wxSingleInstanceChecker* &instChecker )//, const vector< unsigned int >& pids )
{
  const wxString name = wxString::Format( _( "wxparaver-%s" ), wxGetUserId().c_str());
  wxLogNull *tmpLogNull = new wxLogNull();
  if (instChecker == NULL)
    instChecker = new wxSingleInstanceChecker(name);
  delete tmpLogNull;
  
  //Only 1 session
  if ( !instChecker->IsAnotherRunning() ) 
  {
    std::cout << " [one session found] \n";
    string pid = "";
    paraverMain::SessionSaveWrapper( pid );
  }
  else
  {
      std::cout << " [multi session found] ";
    wxLogNull logNull;
  
    stClient *client = new stClient;
    wxString hostName = wxT( "localhost" );
    const wxString service_name = wxString::Format( _( "/tmp/wxparaver_service-%s" ), wxGetUserId().c_str());
    wxConnectionBase *connection = client->MakeConnection( hostName, service_name, wxT( "wxparaver" ) );
      std::cout << connection << std::endl;
      std::cout << hostName << " + " << service_name << "  " << std::endl;
    if( connection )
    {
      std::cout << "--> Sending PID to main process... \n " << std::endl;

      connection->Poke( "pid", _( std::to_string( getpid() ) ) );

      string pid = std::to_string( getpid() );
      paraverMain::SessionSaveWrapper( pid );

      connection->Disconnect();
      delete connection;
    }
    delete client;
  }
}

void AutoSessionManager::LoadAutoSession( wxString whichFile )
{

	//SessionSaver::LoadSession( sessionName );
	/*wxLogNull logNull;
    
	stClient *client = new stClient;
	wxString hostName = wxT( "localhost" );
#ifdef WIN32
	  wxConnectionBase *connection = client->MakeConnection( hostName, 
	                                                         wxT( "wxparaver_service" ),
	                                                         wxT( "wxparaver" ) );
#else
	const wxString service_name = wxString::Format( _( "/tmp/wxparaver_service-%s" ), wxGetUserId().c_str());
	wxConnectionBase *connection = client->MakeConnection( hostName, 
                                                             service_name,
                                                             wxT( "wxparaver" ) );
#endif

    if( connection )
    {
#if wxMAJOR_VERSION >= 3
        connection->Execute( "BEGIN" );
        connection->Execute( "END" );
#else
        connection->Execute( wxT( "BEGIN" ) );
        connection->Execute( wxT( "END" ) );
#endif
        connection->Disconnect();
        delete connection;
        delete client;

        return true;
	}
	else
	{
		wxMessageBox( wxT( "Sorry, the existing instance may be too busy to respond." ),
		              wxT( "wxparaver" ), wxICON_INFORMATION|wxOK );
		delete client;
		return false;
	}*/
}