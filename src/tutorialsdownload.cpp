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


TutorialsDownload *TutorialsDownload::instance = NULL;
bool              TutorialsDownload::tutorialsListUpdated = false;

TutorialsDownload *TutorialsDownload::getInstance()
{
  if( instance == NULL )
    instance = new TutorialsDownload();
    
  return instance;
}

TutorialsDownload::TutorialsDownload()
{}

TutorialsDownload::~TutorialsDownload()
{}

const vector<TutorialData>& TutorialsDownload::getTutorialsList()
{
  if( TutorialsDownload::tutorialsListUpdated )
    return tutorialsList;
    
  // TODO: Download and parse tutorials data from server
  
  // Tutorial data example. Debug purposes.
  TutorialData tmp( 1, "https://tools.bsc.es/sites/default/files/documentation/1.introduction_to_paraver_mpi.tar.gz", "Paraver introduction (MPI)", 1 );
  tutorialsList.push_back( tmp );
  
  TutorialsDownload::tutorialsListUpdated = true;
  
  return tutorialsList;
}
