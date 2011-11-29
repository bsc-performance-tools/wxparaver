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

#ifndef _HELPCONTENTS_H_
#define _HELPCONTENTS_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/html/htmlwin.h"
////@end includes

#include "wx/help.h"    // pruebas wxHelpController
#include "wx/fs_zip.h"  // pruebas wxHelpController

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxHtmlWindow;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_HELPCONTENTS 10192
#define ID_HTMLWINDOW 10193
#define ID_BUTTON_INDEX 10194
#define ID_BUTTON_CLOSE 10195
#define SYMBOL_HELPCONTENTS_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_HELPCONTENTS_TITLE _("Help Contents")
#define SYMBOL_HELPCONTENTS_IDNAME ID_HELPCONTENTS
#define SYMBOL_HELPCONTENTS_SIZE wxSize(400, 300)
#define SYMBOL_HELPCONTENTS_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * HelpContents class declaration
 */

class HelpContents: public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( HelpContents )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  HelpContents();
  HelpContents( wxWindow* parent,
                //bool borrame, wxHelpController *help, 
                wxWindowID id = SYMBOL_HELPCONTENTS_IDNAME, const wxString& caption = SYMBOL_HELPCONTENTS_TITLE, const wxPoint& pos = SYMBOL_HELPCONTENTS_POSITION, const wxSize& size = SYMBOL_HELPCONTENTS_SIZE, long style = SYMBOL_HELPCONTENTS_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_HELPCONTENTS_IDNAME, const wxString& caption = SYMBOL_HELPCONTENTS_TITLE, const wxPoint& pos = SYMBOL_HELPCONTENTS_POSITION, const wxSize& size = SYMBOL_HELPCONTENTS_SIZE, long style = SYMBOL_HELPCONTENTS_STYLE );

  /// Destructor
  ~HelpContents();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin HelpContents event handler declarations

  /// wxEVT_COMMAND_HTML_LINK_CLICKED event handler for ID_HTMLWINDOW
  void OnHtmlwindowLinkClicked( wxHtmlLinkEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_INDEX
  void OnButtonIndexClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CLOSE
  void OnButtonCloseClick( wxCommandEvent& event );

////@end HelpContents event handler declarations

////@begin HelpContents member function declarations

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end HelpContents member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

////@begin HelpContents member variables
  wxHtmlWindow* htmlWindow;
////@end HelpContents member variables

private:
  wxString currentTutorialDir;

  std::string getHrefFullPath( wxHtmlLinkEvent &event );
  bool matchHrefExtension( wxHtmlLinkEvent &event, const wxString extension );

  const wxString getHtmlIndex( const wxString& path );
  const wxString getTitle( int numTutorial, const wxString& path );
  void appendTutorial( const wxString& title, const wxString& path, wxString& htmlDoc );
  void buildIndex();

  void htmlMessage( wxString& htmlDoc );
};

#endif
  // _HELPCONTENTS_H_
