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

#ifndef _AUTOSESSIONMANAGER_H_
#define _AUTOSESSIONMANAGER_H_

#include <map>
#include <vector>
#include <string>
#include "wx/wx.h"
#include "wx/string.h"
#include "connection.h"
#include "paraverconfig.h"
#include "sessionsaver.h"
#include "wx/snglinst.h"

#include "wxparaverapp.h"
#include "paravermain.h"

// SERIALIZATION INCLUDES
#include <fstream>
#include <iostream>
#include <boost/serialization/string.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

// DATE TIME INCLUDES
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;


//#define SESSION_FILE_SUFFIX ".multisession"

//todo versions
class AutoSessionManager
{
  private:
    friend class boost::serialization::access;
    
    static void SetSessions( std::vector < unsigned int >& sessions );

    class SessionItems
    {
      public:
        template< class Archive >
        void serialize( Archive & ar, const unsigned int version )
        {
          if( version >= 1 )
          {
            ar & boost::serialization::make_nvp( "pid", pid );
            ar & boost::serialization::make_nvp( "status", status );
            ar & boost::serialization::make_nvp( "session_date", sessionDate );
          }
        }
        
        enum StatusID
        {
          OPEN   = 0,
          CLOSED = 1
        };
        
        int pid;
        StatusID status;
        boost::gregorian::date sessionDate;    
    } sessionItems ;
    
    map < unsigned int, SessionItems > sessions;

    template< class Archive >
    void serialize( Archive & ar, const unsigned int version )
    {
      ar & boost::serialization::make_nvp( "session_items", sessions);
    }


  public:
    AutoSessionManager();
    AutoSessionManager(vector < unsigned int >& pids) ;
    static void SaveAutoSession( wxSingleInstanceChecker* &instChecker );
    static void LoadAutoSession( wxString whichFile );
};


// Second version: introducing some structure
BOOST_CLASS_VERSION( SessionItems, 1)

#endif // _AUTOSESSIONMANAGER_H_