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

#ifndef _ADVANCEDSAVECONFIGURATION_H_
#define _ADVANCEDSAVECONFIGURATION_H_

/*!
 * Includes
 */


////@begin includes
////@end includes

#include <vector>
#include <map>
#include "paraverkerneltypes.h"
#include "window.h"
#include "histogram.h"



/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_ADVANCEDSAVECONFIGURATION 10186
#define ID_CHOICE_WINDOW 10185
#define ID_SCROLLEDWINDOW1 10187
#define SYMBOL_ADVANCEDSAVECONFIGURATION_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_ADVANCEDSAVECONFIGURATION_TITLE _("Save Basic CFG - Properties Editor")
#define SYMBOL_ADVANCEDSAVECONFIGURATION_IDNAME ID_ADVANCEDSAVECONFIGURATION
#define SYMBOL_ADVANCEDSAVECONFIGURATION_SIZE wxDefaultSize
#define SYMBOL_ADVANCEDSAVECONFIGURATION_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * AdvancedSaveConfiguration class declaration
 */

class AdvancedSaveConfiguration: public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( AdvancedSaveConfiguration )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  AdvancedSaveConfiguration();
  AdvancedSaveConfiguration( wxWindow* parent,
                             wxWindowID id = SYMBOL_ADVANCEDSAVECONFIGURATION_IDNAME,
                             const wxString& caption = SYMBOL_ADVANCEDSAVECONFIGURATION_TITLE,
                             const wxPoint& pos = SYMBOL_ADVANCEDSAVECONFIGURATION_POSITION,
                             const wxSize& size = SYMBOL_ADVANCEDSAVECONFIGURATION_SIZE,
                             long style = SYMBOL_ADVANCEDSAVECONFIGURATION_STYLE );
  AdvancedSaveConfiguration( wxWindow* parent,
                             const vector< Window * > &whichTimelines,
                             const vector< Histogram * > &whichHistograms,
                             wxWindowID id = SYMBOL_ADVANCEDSAVECONFIGURATION_IDNAME,
                             const wxString& caption = SYMBOL_ADVANCEDSAVECONFIGURATION_TITLE,
                             const wxPoint& pos = SYMBOL_ADVANCEDSAVECONFIGURATION_POSITION,
                             const wxSize& size = SYMBOL_ADVANCEDSAVECONFIGURATION_SIZE,
                             long style = SYMBOL_ADVANCEDSAVECONFIGURATION_STYLE );
  AdvancedSaveConfiguration( wxWindow* parent,
                             const wxString &whichWindowName,
                             const bool isTimeline,
                             const unsigned int currentNumber,
                             const unsigned int numberOfWindows,
                             const map< string, string > &whichRenamedTags,
                             const vector< string > &allTags,
                             wxWindowID id = SYMBOL_ADVANCEDSAVECONFIGURATION_IDNAME,
                             const wxString& caption = SYMBOL_ADVANCEDSAVECONFIGURATION_TITLE,
                             const wxPoint& pos = SYMBOL_ADVANCEDSAVECONFIGURATION_POSITION,
                             const wxSize& size = SYMBOL_ADVANCEDSAVECONFIGURATION_SIZE,
                             long style = SYMBOL_ADVANCEDSAVECONFIGURATION_STYLE );
  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ADVANCEDSAVECONFIGURATION_IDNAME, const wxString& caption = SYMBOL_ADVANCEDSAVECONFIGURATION_TITLE, const wxPoint& pos = SYMBOL_ADVANCEDSAVECONFIGURATION_POSITION, const wxSize& size = SYMBOL_ADVANCEDSAVECONFIGURATION_SIZE, long style = SYMBOL_ADVANCEDSAVECONFIGURATION_STYLE );

  /// Destructor
  ~AdvancedSaveConfiguration();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin AdvancedSaveConfiguration event handler declarations

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_WINDOW
  void OnChoiceWindowSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_SAVE
  void OnSaveClick( wxCommandEvent& event );

////@end AdvancedSaveConfiguration event handler declarations

////@begin AdvancedSaveConfiguration member function declarations

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end AdvancedSaveConfiguration member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

////@begin AdvancedSaveConfiguration member variables
  wxChoice* choiceWindow;
  wxScrolledWindow* scrolledWindow;
////@end AdvancedSaveConfiguration member variables

    //bool TransferDataFromWindow();

//    const map< string, string >& GetCFG4DAliasList() const { return renamedTag; };
//    const map< string, bool >&   GetCFG4DEnabledList() const { return enabledTag; };

  protected:
    const static wxString KSuffixSeparator;
    const static wxString KCheckBoxSuffix;
    const static wxString KTextCtrlSuffix;

  private:
    bool isTimeline;
    int currentItem;                   // Index to selected item in choice widget. Used to compute
                                       // position of Window * or Histogram * in next vectors.
    vector< Window * > timelines;
    vector< Histogram * > histograms;
    
    map< string, string > renamedTag;
    map< string, bool > enabledTag;
    bool enabledCFG4DMode;

    int GetSelectionIndexCorrected( int index, bool &isTimeline );

    wxString BuildName( Window *current );
    wxString BuildName( Histogram *current );

    void BuildTagMaps( const vector< string > &fullTagList,
                       const map< string, string > &renamedTagMap,
                       map< string, bool > &whichEnabledFullTagList,
                       map< string, string > &whichRenamedFullTagList );
    void BuildTagWidgets( const vector< string > &fullTagList );
    void BuildTagsPanel( Window *currentWindow );
    void BuildTagsPanel( Histogram *currentHistogram );

    void PreparePanel();
    void TransferDataFromPanel();

    void DisconnectWidgetsTagsPanel();
    void CleanTagsPanel();

    wxCheckBox *GetCheckBoxByName( const wxString& widgetName ) const;
    wxTextCtrl *GetTextCtrlByName( const wxString& widgetName ) const;
    void OnCheckBoxClicked( wxCommandEvent& event );
};

#endif
  // _ADVANCEDSAVECONFIGURATION_H_
