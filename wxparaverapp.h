/////////////////////////////////////////////////////////////////////////////
// Name:        wxparaverapp.h
// Purpose:     
// Author:      Eloy Martinez
// Modified by: 
// Created:     Mon 16 Jun 2008 12:38:16 CEST
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

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

struct sigaction act;

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
