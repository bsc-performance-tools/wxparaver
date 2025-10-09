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

#include <wx/string.h>
#include <array>
#include <vector>

#include "paraverkerneltypes.h"
#include "trace.h"

class trace;

enum class TExternalAppID
{
  // --- Called through RunScript choice selector widget --- 
  DIMEMAS = 0,  // Dimemas
  PRVSTATS,     // prvstats
  CLUSTERING,   // Clustering
  FOLDING,      // Folding
  MESS,       // Mess
    // <-- add new apps here at most
  USER_COMMAND, // User command

  // --- Called by different widget ---
  DIMEMAS_GUI,  // DimemasGui   invoked through button

  NUMBER_APPS
};

class ExternalApps
{
  public:
    ExternalApps() = delete;

    static wxString getApplicationLabel( TExternalAppID whichApp );
    static wxString getApplicationBin( TExternalAppID whichApp );
    static wxString getApplicationCheckBin( TExternalAppID whichApp );

    static bool existCommand( const wxString& program );
    static bool existCommand( TExternalAppID programID );

    static bool isSuitableAppForTrace( TExternalAppID programID, const Trace& whichTrace );

    static std::vector< bool > suitableAppsForTrace( const Trace& whichTrace );

  private:
    // Labels to construct selector & warning dialogs
    static const std::array< wxString, (int)TExternalAppID::NUMBER_APPS > applicationLabel;

    // Application binary names
    static const std::array< wxString, (int)TExternalAppID::NUMBER_APPS > applicationBin;

    // Application binary check names
    static const std::array< wxString, (int)TExternalAppID::NUMBER_APPS > applicationCheckBin;

    // Functions to verify if app is suitable for trace
    static const std::array< std::function< bool( const Trace& ) >, (int)TExternalAppID::NUMBER_APPS > applicationVerifyFunctions;

    static bool verifySuitableEvents( TExternalAppID programID, const Trace& whichTrace );
};
