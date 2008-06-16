/////////////////////////////////////////////////////////////////////////////
// Name:        wxparaverapp.cpp
// Purpose:     
// Author:      Eloy Martinez
// Modified by: 
// Created:     Mon 16 Jun 2008 12:38:16 CEST
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "wxparaverapp.h"

////@begin XPM images

////@end XPM images


/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( wxparaverApp )
////@end implement app


/*!
 * wxparaverApp type definition
 */

IMPLEMENT_CLASS( wxparaverApp, wxApp )


/*!
 * wxparaverApp event table definition
 */

BEGIN_EVENT_TABLE( wxparaverApp, wxApp )

////@begin wxparaverApp event table entries
////@end wxparaverApp event table entries

END_EVENT_TABLE()


/*!
 * Constructor for wxparaverApp
 */

wxparaverApp::wxparaverApp()
{
    Init();
}


/*!
 * Member initialisation
 */

void wxparaverApp::Init()
{
////@begin wxparaverApp member initialisation
////@end wxparaverApp member initialisation
}

/*!
 * Initialisation for wxparaverApp
 */

bool wxparaverApp::OnInit()
{    
////@begin wxparaverApp initialisation
    // Remove the comment markers above and below this block
    // to make permanent changes to the code.

#if wxUSE_XPM
    wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
    wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
    wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
    wxImage::AddHandler(new wxGIFHandler);
#endif
////@end wxparaverApp initialisation

    return true;
}


/*!
 * Cleanup for wxparaverApp
 */

int wxparaverApp::OnExit()
{    
////@begin wxparaverApp cleanup
    return wxApp::OnExit();
////@end wxparaverApp cleanup
}

