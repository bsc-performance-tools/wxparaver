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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#ifndef _WXPARAVERAPP_H_
#define _WXPARAVERAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "paravermain.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

#ifndef WIN32
struct sigaction act;
#endif

/*!
 * wxparaverApp class declaration
 */

class wxparaverApp: public wxApp
{    
    DECLARE_CLASS( wxparaverApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    wxparaverApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

#ifndef WIN32
    // Signal handling
    static void handler( int signum );
    void presetUserSignals();
#endif

////@begin wxparaverApp event handler declarations

////@end wxparaverApp event handler declarations

////@begin wxparaverApp member function declarations

////@end wxparaverApp member function declarations

////@begin wxparaverApp member variables
////@end wxparaverApp member variables

    static paraverMain* mainWindow;
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(wxparaverApp)
////@end declare app

#endif
    // _WXPARAVERAPP_H_
