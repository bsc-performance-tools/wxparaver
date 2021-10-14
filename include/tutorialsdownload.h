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


#include <string>
#include <vector>
#include <fstream>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include "paraverkerneltypes.h"

using std::string;
using std::vector;
using std::ofstream;


class TutorialsProgress
{
  public:
    TutorialsProgress( wxString& title,
                       wxString& message,
                       unsigned int whichNumTutorials );

    ~TutorialsProgress();

    void nextTutorial();
    void setMessage( wxString& whichMessage );
    void setCurrentDownloadSize( unsigned int whichSize );
    void setCurrentInstallSize( unsigned int whichSize );
    void updateDownload( int whichValue );
    void updateInstall( int whichValue );

  private:
    wxProgressDialog *dialog;
    unsigned int numTutorials;
    unsigned int currentTutorial;
    unsigned int currentDownloadSize;
    unsigned int currentInstallSize;
};

class TutorialData
{
  public:
    TutorialData()
    {}
    
    TutorialData( PRV_UINT16 whichId,
                  string     whichUrl,
                  string     whichName,
                  PRV_UINT16 whichVersion
                ) :
                  id( whichId ),
                  url( whichUrl ),
                  name( whichName ),
                  version( whichVersion )
    {}
    
    ~TutorialData()
    {}
    
    PRV_UINT16 getId() const { return id; }
    string     getUrl() const { return url; }
    string     getName() const { return name; }
    PRV_UINT16 getVersion() const { return version; }
    
    void setName( string whichName ) { name = whichName; }
    void setVersion( PRV_UINT16 whichVersion ) { version = whichVersion; }

    template< class Archive >
    void serialize( Archive & ar, const unsigned int whichVersion )
    {
      ar & boost::serialization::make_nvp( "id", id );
      ar & boost::serialization::make_nvp( "url", url );
      ar & boost::serialization::make_nvp( "name", name );
      ar & boost::serialization::make_nvp( "version", version );
    }
    
  private:
    PRV_UINT16 id;
    string     url;
    string     name;
    PRV_UINT16 version;

};


BOOST_CLASS_VERSION( TutorialData, 1 )


class TutorialsDownload
{
  public:
    static std::string tutorialsListFile;

    static TutorialsDownload *getInstance();
    
    ~TutorialsDownload();
    
    void writeList( std::string& fullPath );

    vector<TutorialData> getTutorialsList();
    void downloadInstall( const vector<PRV_UINT16>& whichTutorials );
    const TutorialData& findTutorial( PRV_UINT16 whichId ) const;

    template< class Archive >
    void serialize( Archive & ar, const unsigned int whichVersion )
    {
      ar & boost::serialization::make_nvp( "tutorialsList", tutorialsList );
    }

  protected:
    TutorialsDownload();

  private:
    static TutorialsDownload *instance;
    
    bool tutorialsListUpdated;
    vector<TutorialData> tutorialsList;

    void loadXML( const std::string& whichFilename, TutorialsDownload *whichTutorials, const std::string& whichTag );
    void loadXML( const std::string& whichFilename, vector<TutorialData>& whichTutorials, const std::string& whichTag );
    bool downloadTutorialsList() const;
    bool download( const TutorialData& whichTutorial, string& tutorialFile, TutorialsProgress& progress ) const;
    bool install( const string& tutorialFile, TutorialsProgress& progress ) const;
};

BOOST_CLASS_VERSION( TutorialsDownload, 1 )

