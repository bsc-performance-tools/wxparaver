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


/*!
 * Includes
 */
// clang-format off
////@begin includes
#include "wx/statline.h"
////@end includes
// clang-format on
#include "cfg.h"
#include "cfgs4d.h"
#include "histogram.h"
#include "window.h"

/*!
 * Forward declarations
 */
// clang-format off
////@begin forward declarations
////@end forward declarations
// clang-format on

/*!
 * Control identifiers
 */
// clang-format off
////@begin control identifiers
#define ID_SAVECONFIGURATIONDIALOG 10012
#define ID_CHOICE_TRACE_SELECTOR 10191
#define ID_LISTTIMELINES 10013
#define ID_BUTTON_SET_ALL_TIMELINES 10188
#define ID_BUTTON_UNSET_ALL_TIMELINES 10189
#define ID_LISTHISTOGRAMS 10014
#define ID_BUTTON_SET_ALL_HISTOGRAMS 10000
#define ID_BUTTON_UNSET_ALL_HISTOGRAMS 10001
#define ID_CHECKBEGIN 10016
#define ID_CHECKEND 10017
#define ID_CHECKSEMANTIC 10018
#define ID_RADIOALLTRACE 10019
#define ID_RADIOALLWINDOW 10020
#define ID_CHECKGRADIENT 10022
#define ID_TEXTDESCRIPTION 10015
#define ID_CHECKBOX_SAVE_BASIC_MODE 10190
#define SYMBOL_SAVECONFIGURATIONDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxSTAY_ON_TOP|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_SAVECONFIGURATIONDIALOG_TITLE _("Save configuration")
#define SYMBOL_SAVECONFIGURATIONDIALOG_IDNAME ID_SAVECONFIGURATIONDIALOG
#define SYMBOL_SAVECONFIGURATIONDIALOG_SIZE wxDefaultSize
#define SYMBOL_SAVECONFIGURATIONDIALOG_POSITION wxDefaultPosition
////@end control identifiers
// clang-format on

/*!
 * SaveConfigurationDialog class declaration
 */

class SaveConfigurationDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS( SaveConfigurationDialog )
    DECLARE_EVENT_TABLE()

  public:
    /// Constructors
    SaveConfigurationDialog();
    SaveConfigurationDialog( wxWindow* parent,
                             wxWindowID id           = SYMBOL_SAVECONFIGURATIONDIALOG_IDNAME,
                             const wxString& caption = SYMBOL_SAVECONFIGURATIONDIALOG_TITLE,
                             const wxPoint& pos      = SYMBOL_SAVECONFIGURATIONDIALOG_POSITION,
                             const wxSize& size      = SYMBOL_SAVECONFIGURATIONDIALOG_SIZE,
                             long style              = SYMBOL_SAVECONFIGURATIONDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent,
                 wxWindowID id           = SYMBOL_SAVECONFIGURATIONDIALOG_IDNAME,
                 const wxString& caption = SYMBOL_SAVECONFIGURATIONDIALOG_TITLE,
                 const wxPoint& pos      = SYMBOL_SAVECONFIGURATIONDIALOG_POSITION,
                 const wxSize& size      = SYMBOL_SAVECONFIGURATIONDIALOG_SIZE,
                 long style              = SYMBOL_SAVECONFIGURATIONDIALOG_STYLE );

    /// Destructor
    ~SaveConfigurationDialog();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    // clang-format off
  ////@begin SaveConfigurationDialog event handler declarations

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_TRACE_SELECTOR
  void OnChoiceTraceSelectorSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SET_ALL_TIMELINES
  void OnButtonSetAllTimelinesClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_UNSET_ALL_TIMELINES
  void OnButtonUnsetAllTimelinesClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SET_ALL_HISTOGRAMS
  void OnButtonSetAllHistogramsClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_UNSET_ALL_HISTOGRAMS
  void OnButtonUnsetAllHistogramsClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_SAVE
  void OnSaveClick( wxCommandEvent& event );

  ////@end SaveConfigurationDialog event handler declarations
    // clang-format on

    // clang-format off
  ////@begin SaveConfigurationDialog member function declarations

  std::vector<Histogram *> GetHistograms() const { return histograms ; }
  void SetHistograms(std::vector<Histogram *> value) { histograms = value ; }

  Trace * GetInitialTrace() const { return initialTrace ; }
  void SetInitialTrace(Trace * value) { initialTrace = value ; }

  SaveOptions GetOptions() const { return options ; }
  void SetOptions(SaveOptions value) { options = value ; }

  std::vector< Histogram * > GetSelectedHistograms() const { return selectedHistograms ; }
  void SetSelectedHistograms(std::vector< Histogram * > value) { selectedHistograms = value ; }

  std::vector< Timeline * > GetSelectedTimelines() const { return selectedTimelines ; }
  void SetSelectedTimelines(std::vector< Timeline * > value) { selectedTimelines = value ; }

  std::vector<Timeline *> GetTimelines() const { return timelines ; }
  void SetTimelines(std::vector<Timeline *> value) { timelines = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
  ////@end SaveConfigurationDialog member function declarations
    // clang-format on

    const CFGS4DLinkedPropertiesManager& getLinkedPropertiesManager() const;

    /// Should we show tooltips?
    static bool ShowToolTips();

    bool TransferDataToWindow();
    bool TransferDataFromWindow();

    // clang-format off
  ////@begin SaveConfigurationDialog member variables
  wxChoice* choiceTraceSelector;
  wxCheckListBox* listTimelines;
  wxButton* buttonSetAllTimelines;
  wxButton* buttonUnsetAllTimelines;
  wxCheckListBox* listHistograms;
  wxButton* buttonSetAllHistograms;
  wxButton* buttonUnsetAllHistograms;
  wxCheckBox* optRelativeBegin;
  wxCheckBox* optRelativeEnd;
  wxCheckBox* optComputeSemantic;
  wxRadioButton* radioAllTrace;
  wxRadioButton* radioAllWindow;
  wxCheckBox* optComputeGradient;
  wxTextCtrl* textDescription;
  wxCheckBox* checkboxSaveCFGBasicMode;
private:
  std::vector<Histogram *> histograms;
  Trace * initialTrace;
  SaveOptions options;
  std::vector< Histogram * > selectedHistograms;
  std::vector< Timeline * > selectedTimelines;
  std::vector<Timeline *> timelines;
  ////@end SaveConfigurationDialog member variables
    // clang-format on
    std::vector< std::string > traces;
    std::vector< Timeline* > displayedTimelines;
    std::vector< Histogram* > displayedHistograms;
    CFGS4DLinkedPropertiesManager linkedProperties;
};
