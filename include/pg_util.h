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

#include <vector>

#include "paraverconfig.h"

class Timeline;
class Histogram;
class wxPropertyGrid;
struct PropertyClientData;

class NumColumnsChoices
{
  public:
    enum
    {
      DEFAULT = 1,
      CUSTOM_DELTA = 2
    };

    template< typename F >
    static void createChoices( F addFunction )
    {
      addFunction( "1" );
      wxString tmpStr;
      tmpStr << ParaverConfig::getInstance()->getHistogramNumColumns();
      addFunction( tmpStr + " (default)" );
      addFunction( "Custom delta" );
    }
};

void initPG();

void updateTimelineProperties( wxPropertyGrid* windowProperties,
                               Timeline *whichWindow,
                               std::vector< PropertyClientData * >& whichPropertiesClientData );
void updateHistogramProperties( wxPropertyGrid* windowProperties,
                                Histogram *whichHisto,
                                std::vector< PropertyClientData * >& whichPropertiesClientData );
