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
#include "paraverconfig.h"
#include "sessionsaver.h"
#include "wx/snglinst.h"



// DATE TIME INCLUDES
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;


//#define SESSION_FILE_SUFFIX ".multisession"

//todo versions
class AutoSessionManager
{
  /*public:
    AutoSessionManager();
    AutoSessionManager(vector < unsigned int >& pids) ;
    static void SaveAutoSession();
    static void LoadAutoSession();

    static void UpdateSession( wxString &pid );

    unsigned int getPid()  
    {
      return pid;
    }
    wxString getFileName();


  private:
    static void SetSession( unsigned int& pid );
    static void SetSessions( std::vector < unsigned int >& pids );
    
    unsigned int pid;*/
};


#endif // _AUTOSESSIONMANAGER_H_