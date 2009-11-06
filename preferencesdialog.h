/////////////////////////////////////////////////////////////////////////////
// Name:        preferencesdialog.h
// Purpose:     
// Author:      Eloy Martinez
// Modified by: 
// Created:     Mon 28 Sep 2009 14:37:45 CEST
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _PREFERENCESDIALOG_H_
#define _PREFERENCESDIALOG_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/propdlg.h"
////@end includes

#include "paraverconfig.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_PREFERENCESDIALOG 10069
#define ID_PREFERENCES_2D 10071
#define ID_PREFERENCES_2D_NUMCOLUMNS 10075
#define ID_PREFERENCES_2D_PRECISION 10074
#define ID_PREFERENCES_2D_CHECKOPTIONS 10078
#define ID_PREFERENCES_WHATWHERE 10072
#define ID_PREFERENCES_WW_PRECISION 10000
#define ID_PREFERENCES_GLOBAL 10073
#define ID_PREFERENCES_GLOBAL_FILLGAPS 10085
#define ID_PREFERENCES_FILTERS 10070
#define SYMBOL_PREFERENCESDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PREFERENCESDIALOG_TITLE _("Preferences")
#define SYMBOL_PREFERENCESDIALOG_IDNAME ID_PREFERENCESDIALOG
#define SYMBOL_PREFERENCESDIALOG_SIZE wxSize(399, 300)
#define SYMBOL_PREFERENCESDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * PreferencesDialog class declaration
 */

class PreferencesDialog: public wxPropertySheetDialog
{    
  DECLARE_DYNAMIC_CLASS( PreferencesDialog )
  DECLARE_EVENT_TABLE()

public:
  enum ItemCheck { ITEM_SHOW_UNITS, ITEM_THOUSAND_SEPARATOR };

  /// Constructors
  PreferencesDialog();
  PreferencesDialog( wxWindow* parent, wxWindowID id = SYMBOL_PREFERENCESDIALOG_IDNAME, const wxString& caption = SYMBOL_PREFERENCESDIALOG_TITLE, const wxPoint& pos = SYMBOL_PREFERENCESDIALOG_POSITION, const wxSize& size = SYMBOL_PREFERENCESDIALOG_SIZE, long style = SYMBOL_PREFERENCESDIALOG_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_PREFERENCESDIALOG_IDNAME, const wxString& caption = SYMBOL_PREFERENCESDIALOG_TITLE, const wxPoint& pos = SYMBOL_PREFERENCESDIALOG_POSITION, const wxSize& size = SYMBOL_PREFERENCESDIALOG_SIZE, long style = SYMBOL_PREFERENCESDIALOG_STYLE );

  /// Destructor
  ~PreferencesDialog();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin PreferencesDialog event handler declarations

////@end PreferencesDialog event handler declarations

////@begin PreferencesDialog member function declarations

  UINT32 GetHistogramNumColumns() const { return histogramNumColumns ; }
  void SetHistogramNumColumns(UINT32 value) { histogramNumColumns = value ; }

  UINT32 GetHistogramPrecision() const { return histogramPrecision ; }
  void SetHistogramPrecision(UINT32 value) { histogramPrecision = value ; }

  UINT32 GetWhatWherePrecision() const { return whatWherePrecision ; }
  void SetWhatWherePrecision(UINT32 value) { whatWherePrecision = value ; }

  bool GetHistogramThousandSeparator() const { return histogramThousandSeparator ; }
  void SetHistogramThousandSeparator(bool value) { histogramThousandSeparator = value ; }

  bool GetHistogramShowUnits() const { return histogramShowUnits ; }
  void SetHistogramShowUnits(bool value) { histogramShowUnits = value ; }

  UINT32 GetHistogramMaxNumColumns() const { return histogramMaxNumColumns ; }
  void SetHistogramMaxNumColumns(UINT32 value) { histogramMaxNumColumns = value ; }

  UINT32 GetHistogramMaxPrecision() const { return histogramMaxPrecision ; }
  void SetHistogramMaxPrecision(UINT32 value) { histogramMaxPrecision = value ; }

  UINT32 GetWhatWhereMaxPrecision() const { return whatWhereMaxPrecision ; }
  void SetWhatWhereMaxPrecision(UINT32 value) { whatWhereMaxPrecision = value ; }

  bool GetGlobalFillStateGaps() const { return globalFillStateGaps ; }
  void SetGlobalFillStateGaps(bool value) { globalFillStateGaps = value ; }

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
////@end PreferencesDialog member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  bool TransferDataToWindow();
  bool TransferDataFromWindow();

////@begin PreferencesDialog member variables
  wxTextCtrl* txt2DNumColumns;
  wxTextCtrl* txt2DPrecision;
  wxCheckListBox* check2DOptions;
  wxTextCtrl* txtWhatWherePrecision;
  wxCheckBox* checkGlobalFillStateGaps;
private:
  UINT32 histogramNumColumns;
  UINT32 histogramPrecision;
  UINT32 whatWherePrecision;
  bool histogramThousandSeparator;
  bool histogramShowUnits;
  UINT32 histogramMaxNumColumns;
  UINT32 histogramMaxPrecision;
  UINT32 whatWhereMaxPrecision;
  bool globalFillStateGaps;
////@end PreferencesDialog member variables

  wxString formatNumber( long value );
};

#endif
  // _PREFERENCESDIALOG_H_
