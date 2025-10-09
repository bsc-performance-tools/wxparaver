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

#include <wx/arrstr.h>
#include <wx/log.h>
#include <wx/utils.h>

#include <algorithm>
#include <set>

#include "externalapps.h"

#include "labelconstructor.h"

// Labels to construct selector & warning dialogs
const std::array< wxString, (int)TExternalAppID::NUMBER_APPS > ExternalApps::applicationLabel = {
  wxString( "Dimemas" ),      //DIMEMAS
  wxString( "prvstats" ),     //PRVSTATS
  wxString( "Clustering" ),   //CLUSTERING
  wxString( "Folding" ),      //FOLDING
  wxString( "Mess" ),         //MESS
  wxString( "User command" ), //USER_COMMAND

  // Following only for warning dialogs
  wxString( "DimemasGUI" )    //DIMEMAS_GUI
};

// Application binary names
const std::array< wxString, (int)TExternalAppID::NUMBER_APPS > ExternalApps::applicationBin = {
  wxString( "dimemas-wrapper.sh"),  // DIMEMAS
#ifdef _WIN32
  wxString( "prvstats.exe"),        // PRVSTATS
#else
  wxString( "prvstats-wrapper.sh"), // PRVSTATS
#endif
  wxString( "BurstClustering"),     // CLUSTERING
  wxString( "rri-auto"),            // FOLDING
  wxString( "mess-prv"),          // MESS
  wxString( "" ),                   // USER_COMMAND
  wxString( "DimemasGUI")           // DIMEMAS_GUI
};

// Application binary check names
const std::array< wxString, (int)TExternalAppID::NUMBER_APPS > ExternalApps::applicationCheckBin = {
  wxString( "Dimemas" ),         // DIMEMAS
  wxString( "prvstats" ),        // PRVSTATS
  wxString( "BurstClustering" ), // CLUSTERING
  wxString( "rri-auto" ),        // FOLDING
  wxString( "mess-prv" ),        // MESS
  wxString( "" ),                // USER_COMMAND
  wxString( "DimemasGUI")        // DIMEMAS_GUI
};


template< typename C >
bool verifyEventLabels( const Trace& whichTrace, const std::set<TEventType>& whichTraceEvents, const C& whichEventLabels )
{
  if( whichEventLabels.empty() )
    return true;

  std::string strTraceEvent;
  for( auto itTraceEvent = whichTraceEvents.begin(); itTraceEvent != whichTraceEvents.end(); ++itTraceEvent )
  {
    strTraceEvent.clear();
    whichTrace.getEventLabels().getEventTypeLabel( *itTraceEvent, strTraceEvent );
    if( std::any_of( whichEventLabels.begin(), whichEventLabels.end(), [&]( const auto& el ) { return strTraceEvent.find( el ) != std::string::npos; } ) )
      return true;
  }

  return false;
}


bool verifyDimemas( const Trace& whichTrace )
{
  if( whichTrace.totalApplications() > 1 )
    return false;

  const auto& traceEvents = whichTrace.getLoadedEvents();

  constexpr size_t DIMEMAS_LABELS_SIZE = 6;
  const std::array< std::string, DIMEMAS_LABELS_SIZE > dimemasLabels { "MPI Point-to-point",
                                                                       "MPI Collective Comm",
                                                                       "CUDA library call",
                                                                       "Parallel (OMP)",
                                                                       "Executed OpenMP parallel function",
                                                                       "OpenMP barrier" };

  return verifyEventLabels( whichTrace, traceEvents, dimemasLabels );
}


bool verifyClustering( const Trace& whichTrace )
{
  return whichTrace.totalApplications() == 1;
}


bool verifyFolding( const Trace& whichTrace )
{
  const auto& traceEvents = whichTrace.getLoadedEvents();

  constexpr size_t FOLDING_LABELS_SIZE = 1;
  const std::array< std::string, FOLDING_LABELS_SIZE > foldingLabels { "Sampled functions" };

  return verifyEventLabels( whichTrace, traceEvents, foldingLabels );
}


bool verifyMess( const Trace& whichTrace )
{
  const auto& traceEvents = whichTrace.getLoadedEvents();

  constexpr size_t MESS_LABELS_SIZE = 2;
  const std::array< std::string, MESS_LABELS_SIZE > messReadLabels { "Read", "read" };

  std::string strTraceEvent;
  for( auto itTraceEvent = traceEvents.begin(); itTraceEvent != traceEvents.end(); ++itTraceEvent )
  {
    if( !( *itTraceEvent >= 40000000 && *itTraceEvent < 50000000 ) )
      continue;
    strTraceEvent.clear();
    whichTrace.getEventLabels().getEventTypeLabel( *itTraceEvent, strTraceEvent );
    if( std::any_of( messReadLabels.begin(), messReadLabels.end(), [&]( const auto& el ) { return strTraceEvent.find( el ) != std::string::npos; } ) )
    {
      if( strTraceEvent.find( "DRAM" ) != std::string::npos )
        return true;
    }
  }

  return false;
}


const std::array< std::function< bool( const Trace& ) >, (int)TExternalAppID::NUMBER_APPS > ExternalApps::applicationVerifyFunctions = { {
  { verifyDimemas },    // DIMEMAS
  {},                   // PRVSTATS
  { verifyClustering }, // CLUSTERING
  { verifyFolding },    // FOLDING
  { verifyMess },     // MESS
  {},                   // USER_COMMAND
  {},                   // DIMEMAS_GUI
} };


wxString ExternalApps::getApplicationLabel( TExternalAppID whichApp )
{
  return ExternalApps::applicationLabel[ static_cast< int >( whichApp ) ];
}

wxString ExternalApps::getApplicationBin( TExternalAppID whichApp )
{
  return ExternalApps::applicationBin[ static_cast< int >( whichApp ) ];
}

wxString ExternalApps::getApplicationCheckBin( TExternalAppID whichApp )
{
  return ExternalApps::applicationCheckBin[ static_cast< int >( whichApp ) ];
}

bool ExternalApps::existCommand( const wxString& program )
{
  static wxArrayString tmpOutput, tmpErrors;
  wxLogNull dummyLog;
  return wxExecute( program + " --version", tmpOutput, tmpErrors, wxEXEC_SYNC ) == 0;
}

bool ExternalApps::existCommand( TExternalAppID programID )
{
  return ExternalApps::existCommand( getApplicationCheckBin( programID ) );
}

bool ExternalApps::isSuitableAppForTrace( TExternalAppID programID, const Trace& whichTrace )
{
  return ExternalApps::verifySuitableEvents( programID, whichTrace ) && ExternalApps::existCommand( programID );
}

bool ExternalApps::verifySuitableEvents( TExternalAppID programID, const Trace& whichTrace )
{
  const auto& appFunction = ExternalApps::applicationVerifyFunctions[ static_cast< int >( programID ) ];

  if( appFunction )
    return appFunction( whichTrace );

  return true;
}

std::vector< bool > ExternalApps::suitableAppsForTrace( const Trace& whichTrace )
{
  std::vector< bool > result;

  for( size_t iApp = 0; iApp < (int)TExternalAppID::USER_COMMAND; ++iApp )
    result.push_back( ExternalApps::isSuitableAppForTrace( (TExternalAppID)iApp, whichTrace ) );

  return result;
}
