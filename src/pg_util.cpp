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

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "pg_util.h"
#include "paravermain.h"
#include "pg_extraprop.h"
#include "trace.h"
#include "window.h"
#include "histogram.h"
#include "filter.h"
#include "loadedwindows.h"
#include "labelconstructor.h"
#include "paraverlabels.h"
#include "rowsselectiondialog.h"
#include <wx/object.h> 
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/editors.h>

// CFG4D
#include "cfg.h"

#if wxMAJOR_VERSION>=3
typedef  wxPGProperty* wxPGId;
#endif

static bool filterCatCollapsed        = true;
static bool commFilterCatCollapsed    = true;
static bool commFilterFromCollapsed   = true;
static bool commFilterToCollapsed     = true;
static bool commFilterTagCollapsed    = true;
static bool commFilterSizeCollapsed   = true;
static bool commFilterBWCollapsed     = true;
static bool eventFilterCatCollapsed   = true;
static bool eventFilterTypeCollapsed  = true;
static bool eventFilterValueCollapsed = true;
static bool semanticCatCollapsed      = true;

static bool statCatCollapsed          = true;
static bool ctrlCatCollapsed          = true;
static bool dataCatCollapsed          = true;
static bool thirdWinCatCollapsed      = true;


class wxSpinButtonsEditor : public wxPGTextCtrlEditor
{
#if wxMAJOR_VERSION>=3
  wxDECLARE_DYNAMIC_CLASS( wxSpinButtonsEditor );
#else
  WX_PG_DECLARE_EDITOR_CLASS( wxSpinButtonsEditor );
#endif
    
  public:
    wxSpinButtonsEditor() {}
    virtual ~wxSpinButtonsEditor() {}
#if wxMAJOR_VERSION>=3
    virtual wxString GetName() const { return wxT( "SpinButtonsEditor" ); }
    virtual wxPGWindowList CreateControls( wxPropertyGrid* propGrid,
                                           wxPGProperty* property,
                                           const wxPoint& pos,
                                           const wxSize& sz ) const;
#else
    wxPG_DECLARE_CREATECONTROLS
#endif
    virtual bool OnEvent( wxPropertyGrid* propGrid,
                          wxPGProperty* property,
                          wxWindow* ctrl,
                          wxEvent& event ) const;
};

#if wxMAJOR_VERSION>=3
wxIMPLEMENT_DYNAMIC_CLASS( wxSpinButtonsEditor, wxPGTextCtrlEditor );
#else
WX_PG_IMPLEMENT_EDITOR_CLASS( SpinButtonsEditor,wxSpinButtonsEditor, wxPGTextCtrlEditor )
#endif

wxPGWindowList wxSpinButtonsEditor::CreateControls( wxPropertyGrid* propGrid,
                                                    wxPGProperty* property,
                                                    const wxPoint& pos,
                                                    const wxSize& sz ) const
{
  wxPGMultiButton* buttons = new wxPGMultiButton( propGrid, sz );

  buttons->Add( wxT( "+" ) );
  buttons->Add( wxT( "-" ) );

  wxPGWindowList wndList = wxPGTextCtrlEditor::CreateControls( propGrid, property, pos,
                                                               buttons->GetPrimarySize() );
#if wxMAJOR_VERSION>=3
  buttons->Finalize( propGrid, pos );
#else
  buttons->FinalizePosition( pos );
#endif
  wndList.SetSecondary( buttons );

  return wndList;
}

bool wxSpinButtonsEditor::OnEvent( wxPropertyGrid* propGrid,
                                   wxPGProperty* property,
                                   wxWindow* ctrl,
                                   wxEvent& event ) const
{
#if wxMAJOR_VERSION>=3
  if ( event.GetEventType() == wxEVT_BUTTON )
#else
  if ( event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED )
#endif
  {
    wxPGMultiButton *buttons = ( wxPGMultiButton* ) propGrid->GetEditorControlSecondary();

    double tmpValue = property->GetValue().GetDouble();
    if( property->GetName() == wxT( "Semantic Minimum" ) || property->GetName() == wxT( "Semantic Maximum" ) )
    {
      Window *currentWin = paraverMain::myParaverMain->GetCurrentTimeline();
      tmpValue = currentWin->getMaximumY() - currentWin->getMinimumY();
    }

    if ( event.GetId() == buttons->GetButtonId( 0 ) )
    {
      // + button
      if( property->GetName() == wxT( "Semantic Minimum" ) || property->GetName() == wxT( "Semantic Maximum" ) )
        property->SetValueInEvent( wxVariant( property->GetValue().GetDouble() + tmpValue * 0.1 ) );
      else
        property->SetValueInEvent( wxVariant( tmpValue * 1.1 ) );
      return true;
    }
    if ( event.GetId() == buttons->GetButtonId( 1 ) )
    {
      // - button
      if( property->GetName() == wxT( "Semantic Minimum" ) || property->GetName() == wxT( "Semantic Maximum" ) )
        property->SetValueInEvent( wxVariant( property->GetValue().GetDouble() - tmpValue * 0.1 ) );
      else
        property->SetValueInEvent( wxVariant( tmpValue * 0.9 ) );
      return true;
    }
  }
  
  return wxPGTextCtrlEditor::OnEvent(propGrid, property, ctrl, event);
}

#if wxMAJOR_VERSION>=3
static wxSpinButtonsEditor *spinButtonsEditor = NULL;
#endif

void initPG()
{
#if wxMAJOR_VERSION>=3
  spinButtonsEditor = new wxSpinButtonsEditor();
  wxPropertyGrid::RegisterEditorClass( spinButtonsEditor );
#else
  wxPGRegisterEditorClass( SpinButtonsEditor );
#endif
}


void AppendCFG4DBoolPropertyWindow( wxPropertyGrid* windowProperties,
                                    Window* whichWindow,
                                    wxPGId fatherWidget,
                                    const wxString &widgetLabel,
                                    const wxString &widgetName,
                                    TSingleTimelineProperties propertyIndex,
                                    const bool &propertyValue )
{
  wxBoolProperty *auxProperty = NULL;

  if ( !whichWindow->getCFG4DEnabled() || !whichWindow->getCFG4DMode() )
  {
    // NORMAL mode
    auxProperty = new wxBoolProperty( widgetLabel, widgetName, propertyValue );

    if ( fatherWidget )
    {
      windowProperties->AppendIn( fatherWidget, auxProperty );
    }
    else
    {
      windowProperties->Append( auxProperty );
    }
  }
  else if ( !whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ) )
  {
    // CFG4D mode (single timeline)
    wxString auxTag = wxString::FromAscii( 
            whichWindow->getCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ).c_str() );

    auxProperty = new wxBoolProperty( auxTag, widgetName, propertyValue );

    windowProperties->Append( auxProperty );
  }
  else if ( whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias(
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ) )
  {
    // CFG4D mode (derived timeline)
    wxString auxTag = wxString::FromAscii( 
            whichWindow->getCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ).c_str() );

    auxProperty = new wxBoolProperty( auxTag, widgetName, propertyValue );

    windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property
  }
}


void AppendCFG4DStringPropertyWindow( wxPropertyGrid* windowProperties,
                                      Window* whichWindow,
                                      wxPGId fatherWidget,
                                      const wxString &widgetLabel,
                                      const wxString &widgetName,
                                      TSingleTimelineProperties propertyIndex,
                                      const string &propertyValue )
{
  wxStringProperty *auxProperty = NULL;

  if ( !whichWindow->getCFG4DEnabled() || !whichWindow->getCFG4DMode() )
  {
    // NORMAL mode
    auxProperty = new wxStringProperty(
            widgetLabel, widgetName, wxString::FromAscii( propertyValue.c_str() ) );

    if ( fatherWidget )
    {
      windowProperties->AppendIn( fatherWidget, auxProperty );
    }
    else
    {
      windowProperties->Append( auxProperty );
    }
  }
  else if ( !whichWindow->isDerivedWindow() && 
            whichWindow->existsCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ) )
  {
    // CFG4D mode (single timeline)
    wxString auxTag = wxString::FromAscii( 
            whichWindow->getCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ).c_str() );

    auxProperty = new wxStringProperty( 
            auxTag, widgetName, wxString::FromAscii( propertyValue.c_str() ) );

    windowProperties->Append( auxProperty );
  }
  else if ( whichWindow->isDerivedWindow() && 
            whichWindow->existsCFG4DAlias(
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ) )
  {
    // CFG4D mode (derived timeline)
    wxString auxTag = wxString::FromAscii( 
            whichWindow->getCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ).c_str() );

    auxProperty = new wxStringProperty( 
            auxTag, widgetName, wxString::FromAscii( propertyValue.c_str() ) );

    windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property
  }
}

void AppendCFG4DEnumPropertyWindow( wxPropertyGrid* windowProperties,
                                    Window* whichWindow,
                                    wxPGId fatherWidget,
                                    const wxString &widgetLabel,
                                    const wxString &widgetPrefix,
                                    const wxString &widgetName,
                                    TSingleTimelineProperties propertyIndex,
                                    const wxArrayString &arrayStr,
                                    const wxArrayInt &arrayInt,
                                    const int selected )
{
  wxEnumProperty *auxProperty = NULL;

  if ( !whichWindow->getCFG4DEnabled() || !whichWindow->getCFG4DMode() )
  {
    // NORMAL mode
    auxProperty = new wxEnumProperty( widgetLabel, widgetName, arrayStr, arrayInt, selected );

    if ( fatherWidget )
    {
      windowProperties->AppendIn( fatherWidget, auxProperty );
    }
    else
    {
      windowProperties->Append( auxProperty );
    }
  }
  else if ( !whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ) )
  {
    // CFG4D mode (single timeline)
    wxString auxTag = wxString::FromAscii( 
            whichWindow->getCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ).c_str() );
    wxString fullName = widgetPrefix + widgetName;
    auxProperty = new wxEnumProperty( auxTag, fullName, arrayStr, arrayInt, selected );

    windowProperties->Append( auxProperty );
  }
  else if ( whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ) )
  {
    // CFG4D mode (derived timeline)
    wxString auxTag = wxString::FromAscii( 
            whichWindow->getCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ).c_str() );
    wxString fullName = widgetPrefix + widgetName;
    auxProperty = new wxEnumProperty( auxTag, fullName, arrayStr, arrayInt, selected );

    windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property
  }
}


void AppendCFG4DFloatPropertyWindow( wxPropertyGrid* windowProperties,
                                     Window* whichWindow,
                                     wxPGId fatherWidget,
                                     const wxString &widgetLabel,
                                     const wxString &widgetName,
                                     TSingleTimelineProperties propertyIndex,
                                     const double propertyValue )
{
  wxFloatProperty *auxProperty = NULL;

  if ( !whichWindow->getCFG4DEnabled() || !whichWindow->getCFG4DMode() )
  {
    // NORMAL mode
    auxProperty = new wxFloatProperty( widgetLabel, widgetName, propertyValue );

    if ( fatherWidget )
    {
      windowProperties->AppendIn( fatherWidget, auxProperty );
    }
    else
    {
      windowProperties->Append( auxProperty );
    }
  }
  else if ( !whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ) )
  {
    // CFG4D mode (single timeline)
    wxString auxTag = wxString::FromAscii( 
            whichWindow->getCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ).c_str() );
    auxProperty = new wxFloatProperty( auxTag, widgetName, propertyValue );

    windowProperties->Append( auxProperty );
  }
  else if ( whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ) )
  {
    // CFG4D mode (derived timeline)
    wxString auxTag = wxString::FromAscii( 
            whichWindow->getCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ).c_str() );
    auxProperty = new wxFloatProperty( auxTag, widgetName, propertyValue );

    windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property
  }
  
/* NOT WORKING PROPERLY
#if wxMAJOR_VERSION>=3
  windowProperties->SetPropertyEditor( auxProperty, spinButtonsEditor );
#else
  windowProperties->SetPropertyEditor( auxProperty, wxPG_EDITOR( SpinButtonsEditor ) );
#endif
*/
}


wxPGId AppendCFG4DprvRowsSelectionPropertyWindow( wxPropertyGrid* windowProperties,
                                                  Window* whichWindow,
                                                  wxPGId fatherWidget,
                                                  const wxString &widgetLabel,
                                                  const wxString &widgetPrefix,
                                                  const wxString &widgetName,
                                                  TSingleTimelineProperties propertyIndex,
                                                  const wxString &widgetTitle,
                                                  vector< TObjectOrder > &objects )
{
  wxPGId retId = (wxPGId)NULL;
  prvRowsSelectionProperty *auxProperty = NULL;

  if ( !whichWindow->getCFG4DEnabled() || !whichWindow->getCFG4DMode() )
  {
    // NORMAL mode
    auxProperty = new prvRowsSelectionProperty( windowProperties, whichWindow,
                                                widgetTitle, objects, widgetLabel, widgetName );

    if ( fatherWidget )
    {
      retId = windowProperties->AppendIn( fatherWidget, auxProperty );
    }
    else
    {
      retId = windowProperties->Append( auxProperty );
    }
  }
  else if ( !whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ) )
  {
    // CFG4D mode (single timeline)
    wxString auxTag = wxString::FromAscii( 
            whichWindow->getCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ).c_str() );
    wxString fullName = widgetPrefix + widgetName;
    auxProperty = new prvRowsSelectionProperty( windowProperties, whichWindow,
                                                widgetTitle, objects, auxTag, fullName );

    retId = windowProperties->Append( auxProperty );
  }
  else if ( whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ) )
  {
    // CFG4D mode (derived timeline)
    wxString auxTag = wxString::FromAscii( 
            whichWindow->getCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ).c_str() );
    wxString fullName = widgetPrefix + widgetName;
    auxProperty = new prvRowsSelectionProperty( windowProperties, whichWindow,
                                                widgetTitle, objects, auxTag, fullName );

    retId = windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property and NULL returned.
  }

  return retId;
}


wxPGId AppendCFG4DprvNumbersListPropertyWindow( wxPropertyGrid* windowProperties,
                                                Window* whichWindow,
                                                wxPGId fatherWidget,
                                                const wxString &widgetLabel,
                                                const wxString &widgetPrefix,
                                                const wxString &widgetName,
                                                TSingleTimelineProperties propertyIndex,
                                                const wxArrayString &arrayStr )
{
  wxPGId retId = (wxPGId)NULL;
  prvNumbersListProperty *auxProperty = NULL;
  wxString auxTag;

  if ( !whichWindow->getCFG4DEnabled() || !whichWindow->getCFG4DMode() )
  {
    // NORMAL mode
    auxProperty = new prvNumbersListProperty( widgetLabel, widgetName, arrayStr );

    if ( fatherWidget )
    {
      retId = windowProperties->AppendIn( fatherWidget, auxProperty );
    }
    else
    {
      retId = windowProperties->Append( auxProperty );
    }
  }
  else if ( !whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ) )
  {
    // CFG4D mode (single timeline)
    auxTag = wxString::FromAscii( whichWindow->getCFG4DAlias(
                    SingleTimelinePropertyLabels[ propertyIndex ] ).c_str() );
    wxString fullName = widgetPrefix + widgetName;

    auxProperty = new prvNumbersListProperty( auxTag, fullName, arrayStr );

    retId = windowProperties->Append( auxProperty );
  }
  else if ( whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ) )
  {
    // CFG4D mode (derived timeline)
    auxTag = wxString::FromAscii( whichWindow->getCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ).c_str() );
    wxString fullName = widgetPrefix + widgetName;
    auxProperty = new prvNumbersListProperty( auxTag, fullName, arrayStr );

    retId = windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property and NULL returned.
  }

  return retId;
}


wxPGId AppendCFG4DprvEventInfoPropertyWindow( wxPropertyGrid* windowProperties,
                                              Window* whichWindow,
                                              wxPGId fatherWidget,
                                              const wxString &widgetLabel,
                                              const wxString &widgetPrefix,
                                              const wxString &widgetName,
                                              TSingleTimelineProperties propertyIndex,
                                              const wxPGChoices &choices//,
                                              /*prvEventInfoProperty *&valueProperty*/ )
{
  wxPGId retId = (wxPGId)NULL;
  prvEventInfoProperty *auxProperty = NULL;

  if ( !whichWindow->getCFG4DEnabled() || !whichWindow->getCFG4DMode() )
  {
    // NORMAL mode
    auxProperty = new prvEventInfoProperty( widgetLabel, widgetName, choices, whichWindow );

    if ( fatherWidget )
    {
      retId = windowProperties->AppendIn( fatherWidget, auxProperty );
    }
    else
    {
      retId = windowProperties->Append( auxProperty );
    }
  }
  else if ( !whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ) )
  {
    // CFG4D mode (single timeline)
    wxString auxTag = wxString::FromAscii( 
            whichWindow->getCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ).c_str() );
    wxString fullName = widgetPrefix + widgetName;
    auxProperty = new prvEventInfoProperty( auxTag, fullName, choices, whichWindow );

    retId = windowProperties->Append( auxProperty );
  }
  else if ( whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ) )
  {
    // CFG4D mode (derived timeline)
    wxString auxTag = wxString::FromAscii( 
            whichWindow->getCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ).c_str() );
    wxString fullName = widgetPrefix + widgetName;
    auxProperty = new prvEventInfoProperty( auxTag, fullName, choices, whichWindow );

    retId = windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property and NULL returned.
  }

 // valueProperty = auxProperty;

  return retId;
}


wxPGId AppendCFG4DprvSemanticThreadPropertyWindow( wxPropertyGrid* windowProperties,
                                              Window* whichWindow,
                                              wxPGId fatherWidget,
                                              const wxString &widgetLabel,
                                              const wxString &widgetPrefix,
                                              const wxString &widgetName,
                                              TSingleTimelineProperties propertyIndex,
                                              vector<string> levels,
                                              vector<vector<string> > threadFunctions,
                                              const wxString &defaultValue )
{
  wxPGId retId = (wxPGId)NULL;
  prvSemanticThreadProperty *auxProperty = NULL;

  if ( !whichWindow->getCFG4DEnabled() || !whichWindow->getCFG4DMode() )
  {
    // NORMAL mode
    auxProperty = new prvSemanticThreadProperty( widgetLabel, widgetName, 
                                                 levels, threadFunctions, defaultValue );

    if ( fatherWidget )
    {
      retId = windowProperties->AppendIn( fatherWidget, auxProperty );
    }
    else
    {
      retId = windowProperties->Append( auxProperty );
    }
  }
  else if ( !whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ) )
  {
    // CFG4D mode (single timeline)
    wxString auxTag = wxString::FromAscii( 
            whichWindow->getCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ).c_str() );
    wxString fullName = widgetPrefix + widgetName;
    auxProperty = new prvSemanticThreadProperty( auxTag, fullName, 
                                                 levels, threadFunctions, defaultValue );

    retId = windowProperties->Append( auxProperty );

  }
  else if ( whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ) )
  {
    // CFG4D mode (derived timeline)
    wxString auxTag = wxString::FromAscii( 
            whichWindow->getCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ).c_str() );
    wxString fullName = widgetPrefix + widgetName;
    auxProperty = new prvSemanticThreadProperty( auxTag, fullName,
                                                 levels, threadFunctions, defaultValue );

    retId = windowProperties->Append( auxProperty );

  }
  else
  {
    // CFG4D mode but no tag found => don't show property and NULL returned.
  }

  return retId;
}


void AppendCFG4DStringPropertyHistogram(  wxPropertyGrid* windowProperties,
                                          Histogram* whichHisto,
                                          wxPGId fatherWidget,
                                          const wxString &widgetLabel,
                                          const wxString &widgetName,
                                          THistogramProperties propertyIndex,
                                          const string &propertyValue )
{
  wxStringProperty *auxProperty = NULL;

  if ( !whichHisto->getCFG4DEnabled() || !whichHisto->getCFG4DMode() )
  {
    // NORMAL mode
    auxProperty = new wxStringProperty(
            widgetLabel, widgetName, wxString::FromAscii( propertyValue.c_str() ) );

    if ( fatherWidget )
    {
      windowProperties->AppendIn( fatherWidget, auxProperty );
    }
    else
    {
      windowProperties->Append( auxProperty );
    }
  }
  else if ( whichHisto->existsCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ) )
  {
    // CFG4D mode
    wxString auxTag = wxString::FromAscii( 
            whichHisto->getCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ).c_str() );

    auxProperty = new wxStringProperty( 
            auxTag, widgetName, wxString::FromAscii( propertyValue.c_str() ) );

    windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property
  }
}


void AppendCFG4DEnumPropertyHistogram(  wxPropertyGrid* windowProperties,
                                        Histogram* whichHisto,
                                        wxPGId fatherWidget,
                                        const wxString &widgetLabel,
                                        const wxString &widgetName,
                                        THistogramProperties propertyIndex,
                                        const wxArrayString &arrayStr,
                                        const wxArrayInt &arrayInt,
                                        const int selected )
{
  wxEnumProperty *auxProperty = NULL;

  if ( !whichHisto->getCFG4DEnabled() || !whichHisto->getCFG4DMode() )
  {
    // NORMAL mode
    auxProperty = new wxEnumProperty( widgetLabel, widgetName, arrayStr, arrayInt, selected );

    if ( fatherWidget )
    {
      windowProperties->AppendIn( fatherWidget, auxProperty );
    }
    else
    {
      windowProperties->Append( auxProperty );
    }
  }
  else if ( whichHisto->existsCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ) )
  {
    // CFG4D mode
    wxString auxTag = wxString::FromAscii( 
            whichHisto->getCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ).c_str() );
    auxProperty = new wxEnumProperty( auxTag, widgetName, arrayStr, arrayInt, selected );

    windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property
  }
}


void AppendCFG4DFloatPropertyHistogram(  wxPropertyGrid* windowProperties,
                                         Histogram* whichHisto,
                                         wxPGId fatherWidget,
                                         const wxString &widgetLabel,
                                         const wxString &widgetName,
                                         THistogramProperties propertyIndex,
                                         const double propertyValue )
{
  wxFloatProperty *auxProperty = NULL;

  if ( !whichHisto->getCFG4DEnabled() || !whichHisto->getCFG4DMode() )
  {
    // NORMAL mode
    auxProperty = new wxFloatProperty( widgetLabel, widgetName, propertyValue );

    if ( fatherWidget )
    {
      windowProperties->AppendIn( fatherWidget, auxProperty );
    }
    else
    {
      windowProperties->Append( auxProperty );
    }
  }
  else if ( whichHisto->existsCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ) )
  {
    // CFG4D mode
    wxString auxTag = wxString::FromAscii( 
            whichHisto->getCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ).c_str() );
    auxProperty = new wxFloatProperty( auxTag, widgetName, propertyValue );

    windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property
  }
}


wxPGId AppendCFG4DParamPrvNumbersListPropertyWindow( wxPropertyGrid* windowProperties,
                                                     Window* whichWindow,
                                                     wxPGId fatherWidget,
                                                     TWindowLevel currentSemanticLevel,
                                                     PRV_UINT32 currentNumParameter,
                                                     const wxString &widgetLabel,
                                                     const wxString &widgetPrefix,
                                                     const wxString &widgetName,
                                                     const wxArrayString &arrayStr )
{
  wxPGId retId = (wxPGId)NULL;
  prvNumbersListProperty *auxProperty = NULL;
  wxString auxTag;
  
  // Splitted Parameter alias key
  string kSemanticLevel;
  string kFunction;
  string currentFunction;
  PRV_UINT32 kNumParameter;

  if ( !whichWindow->getCFG4DEnabled() || !whichWindow->getCFG4DMode() )
  {
    // NORMAL mode
    auxProperty = new prvNumbersListProperty( widgetLabel, widgetName, arrayStr );

    if ( fatherWidget )
    {
      retId = windowProperties->AppendIn( fatherWidget, auxProperty );
    }
    else
    {
      retId = windowProperties->Append( auxProperty );
    }
  }
  else
  {
    string currentSemanticLevelStr = TimelineLevelLabels[ currentSemanticLevel ];
    vector< Window::TParamAliasKey > paramAliasKey =
            whichWindow->getCFG4DParamKeysBySemanticLevel( currentSemanticLevelStr );
    for( vector< Window::TParamAliasKey >::const_iterator it = paramAliasKey.begin(); 
         it != paramAliasKey.end(); ++it )
    {
      // CFG4D mode
      whichWindow->splitCFG4DParamAliasKey( *it, kSemanticLevel, kFunction, kNumParameter );
      string currentFunction = whichWindow->getLevelFunction( currentSemanticLevel );
      if ( kFunction      == currentFunction &&
           kNumParameter  == currentNumParameter )
      {
        auxTag = wxString::FromAscii( whichWindow->getCFG4DParamAlias( *it ).c_str() );
        wxString fullName = widgetPrefix + widgetName;
        auxProperty = new prvNumbersListProperty( auxTag, fullName, arrayStr );

        retId = windowProperties->Append( auxProperty );
      }
      else
      {
        // CFG4D mode but no tag found => don't show property and NULL returned.
      }
    }
  }

  return retId;
}


inline void updateStateOf( wxPropertyGrid *windowProperties, bool& categoryStat, const wxString& catName )
{
#if wxMAJOR_VERSION<3
  wxPGProperty *tmpProp = windowProperties->GetPropertyByLabel( catName );
#else
  wxPGProperty *tmpProp = windowProperties->GetProperty( catName );
#endif

  if( tmpProp != NULL )
#if wxMAJOR_VERSION<3
    categoryStat = tmpProp->GetFlagsAsString( wxPG_PROP_COLLAPSED ) == _( "COLLAPSED" );
#else
    categoryStat = !windowProperties->IsPropertyExpanded( tmpProp );
#endif

 
}


inline void updateCategoriesState( wxPropertyGrid *windowProperties )
{
  updateStateOf( windowProperties, filterCatCollapsed,        wxT( "Filter" ) );
  updateStateOf( windowProperties, commFilterCatCollapsed,    wxT( "Communications" ) );
  updateStateOf( windowProperties, commFilterFromCollapsed,   wxT( "Comm from" ) );
  updateStateOf( windowProperties, commFilterToCollapsed,     wxT( "Comm to" ) );
  updateStateOf( windowProperties, commFilterTagCollapsed,    wxT( "Comm tag" ) );
  updateStateOf( windowProperties, commFilterSizeCollapsed,   wxT( "Comm size" ) );
  updateStateOf( windowProperties, commFilterBWCollapsed,     wxT( "Comm bandwidth" ) );
  updateStateOf( windowProperties, eventFilterCatCollapsed,   wxT( "Events" ) );
  updateStateOf( windowProperties, eventFilterTypeCollapsed,  wxT( "Event type" ) );
  updateStateOf( windowProperties, eventFilterValueCollapsed, wxT( "Event value" ) );
  updateStateOf( windowProperties, semanticCatCollapsed,      wxT( "Semantic" ) );

  updateStateOf( windowProperties, statCatCollapsed,      wxT( "Statistics" ) );
  updateStateOf( windowProperties, ctrlCatCollapsed,      wxT( "Control" ) );
  updateStateOf( windowProperties, dataCatCollapsed,      wxT( "Data" ) );
  updateStateOf( windowProperties, thirdWinCatCollapsed,  wxT( "3D" ) );
}


void semanticFunctionParameter( wxPropertyGrid* windowProperties,
                                Window *whichWindow,
                                wxPGId category,
                                TWindowLevel functionLevel )
{
  for( TParamIndex paramIdx = 0; paramIdx < whichWindow->getFunctionNumParam( functionLevel ); ++paramIdx )
  {
    wxArrayString valuesStr;
    wxString propName( _( "Param" ) );
    propName << _( " " ) << paramIdx << _( " " ) << functionLevel;
    TParamValue values = whichWindow->getFunctionParam( functionLevel, paramIdx );
    for( TParamValue::iterator it = values.begin(); it != values.end(); ++it )
      valuesStr.Add( wxString() << (*it) );

    wxString propLabel = wxString( _("   ") ) +
            wxString::FromAscii( whichWindow->getFunctionParamName( functionLevel, paramIdx ).c_str() );
    wxPGId semanticFunctionParameterValue = (wxPGId)NULL;
    semanticFunctionParameterValue = AppendCFG4DParamPrvNumbersListPropertyWindow(
            windowProperties, whichWindow, category,
            functionLevel, (PRV_UINT32)paramIdx,
            propLabel, wxT(""), propName,
            valuesStr );
  }
}


void updateTimelineProperties( wxPropertyGrid* windowProperties, Window *whichWindow )
{
  PRV_UINT32 precision = ParaverConfig::getInstance()->getTimelinePrecision();

  updateCategoriesState( windowProperties );

  whichWindow->setChanged( false );
  windowProperties->Freeze();

  windowProperties->Clear();
  wxArrayString arrayStr;
  wxArrayInt arrayInt;
  wxArrayString arrayFilterFunctions;
  wxArrayInt arrayFilterFunctionsPos;
  wxArrayString arrayComposeFunctions;
  wxArrayInt arrayComposeFunctionsPos;
  wxArrayString arrayNotThreadFunctions;
  wxArrayInt arrayNotThreadFunctionsPos;
  int selected, pos;
  Filter *filter;
  if( whichWindow->isDerivedWindow() )
    filter = NULL;
  else
    filter = whichWindow->getFilter();

  if ( whichWindow->getCFG4DEnabled() )
  {
    arrayStr.Clear();
    arrayInt.Clear();
    selected = whichWindow->getCFG4DMode() ? 1: 0;
    arrayStr.Add( wxT( "Full" ) );
    arrayStr.Add( wxT( "Basic" ) );
    arrayInt.Add( 0 );
    arrayInt.Add( 1 );

    wxEnumProperty *tmpSelector = new wxEnumProperty( wxT("Properties Mode"), wxT("Mode"), arrayStr, arrayInt, selected );
    windowProperties->Append( tmpSelector );
  }

  wxPGId dummyPGId = (wxPGId)NULL; // used to append always to windowProperties

  AppendCFG4DStringPropertyWindow( windowProperties, whichWindow, dummyPGId,
          wxT("Name"), wxT("Name"), SINGLE_NAME, whichWindow->getName() );

  AppendCFG4DStringPropertyWindow( windowProperties, whichWindow, dummyPGId,
          wxT("Begin time"), wxT("Begin time"), SINGLE_BEGINTIME,
          LabelConstructor::timeLabel(
                  whichWindow->traceUnitsToWindowUnits( whichWindow->getWindowBeginTime() ),
                  whichWindow->getTimeUnit(),
                  precision ));

  AppendCFG4DStringPropertyWindow( windowProperties, whichWindow, dummyPGId,
          wxT("End time"), wxT("End time"), SINGLE_ENDTIME,
          LabelConstructor::timeLabel(
                  whichWindow->traceUnitsToWindowUnits( whichWindow->getWindowEndTime() ),
                  whichWindow->getTimeUnit(),
                  precision ));

  AppendCFG4DFloatPropertyWindow( windowProperties, whichWindow, dummyPGId,
          wxT("Semantic Minimum"), wxT("Semantic Minimum"), SINGLE_SEMANTICMINIMUM, whichWindow->getMinimumY() );

  AppendCFG4DFloatPropertyWindow( windowProperties, whichWindow, dummyPGId,
          wxT("Semantic Maximum"), wxT("Semantic Maximum"), SINGLE_SEMANTICMAXIMUM,
          whichWindow->getMaximumY() );

  if( !whichWindow->isDerivedWindow() )
  {
    wxArrayString arrayLevels;
    wxArrayInt arrayLevelsPos;
    
    if( whichWindow->getTrace()->existResourceInfo() )
    {
      arrayLevels.Add( _( "System" ) );
      arrayLevelsPos.Add( SYSTEM );
      arrayLevels.Add( _( "Node" ) );
      arrayLevelsPos.Add( NODE );
      arrayLevels.Add( _( "CPU" ) );
      arrayLevelsPos.Add( CPU );
    }
    arrayLevels.Add( _( "Workload" ) );
    arrayLevelsPos.Add( WORKLOAD );
    arrayLevels.Add( _( "Application" ) );
    arrayLevelsPos.Add( APPLICATION );
    arrayLevels.Add( _( "Task" ) );
    arrayLevelsPos.Add( TASK );
    arrayLevels.Add( _( "Thread" ) );
    arrayLevelsPos.Add( THREAD );

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, dummyPGId,
            wxT("Level"), wxT(""), wxT("Level"), SINGLE_LEVEL,
            arrayLevels, arrayLevelsPos, whichWindow->getLevel() );

  }
  else
  {
    wxArrayString arrayLevels;
    wxArrayInt arrayLevelsPos;
    
    if( whichWindow->getMinAcceptableLevel() >= SYSTEM &&
        whichWindow->getMinAcceptableLevel() <= CPU )
    {
      if( whichWindow->getMinAcceptableLevel() >= SYSTEM )
      {
        arrayLevels.Add( _( "System" ) );
        arrayLevelsPos.Add( SYSTEM );
      }
      if( whichWindow->getMinAcceptableLevel() >= NODE )
      {
        arrayLevels.Add( _( "Node" ) );
        arrayLevelsPos.Add( NODE );
      }
      if( whichWindow->getMinAcceptableLevel() >= CPU )
      {
        arrayLevels.Add( _( "CPU" ) );
        arrayLevelsPos.Add( CPU );
      }
    }
    else
    {
      if( whichWindow->getMinAcceptableLevel() >= WORKLOAD )
      {
        arrayLevels.Add( _( "Workload" ) );
        arrayLevelsPos.Add( WORKLOAD );
      }
      if( whichWindow->getMinAcceptableLevel() >= APPLICATION )
      {
        arrayLevels.Add( _( "Application" ) );
        arrayLevelsPos.Add( APPLICATION );
      }
      if( whichWindow->getMinAcceptableLevel() >= TASK )
      {
        arrayLevels.Add( _( "Task" ) );
        arrayLevelsPos.Add( TASK );
      }
      if( whichWindow->getMinAcceptableLevel() >= THREAD )
      {
        arrayLevels.Add( _( "Thread" ) );
        arrayLevelsPos.Add( THREAD );
      }
    }

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, dummyPGId,
            wxT("Level"), wxT(""), wxT("Level"), (TSingleTimelineProperties)DERIVED_LEVEL,
            arrayLevels, arrayLevelsPos, whichWindow->getLevel() );
  }

  arrayStr.Clear();
  arrayInt.Clear();
  for( int iUnit = 0; iUnit <= DAY; ++iUnit )
  {
    arrayStr.Add( wxString::FromAscii( FULL_LABEL_TIMEUNIT[ iUnit ].c_str() ) );
    arrayInt.Add( iUnit );
  }

  AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, dummyPGId,
          wxT("Time unit"), wxT(""), wxT("Time Unit"), SINGLE_TIMEUNIT,
          arrayStr, arrayInt, whichWindow->getTimeUnit() );

  
  //-------------------------------------------------------------------------
  // Filter related properties
  //-------------------------------------------------------------------------
  if( !whichWindow->isDerivedWindow() )
  {
    vector<string> filterFunctions;
    filter->getAllFilterFunctions( filterFunctions );
    arrayFilterFunctions.Clear();
    pos = 0;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      arrayFilterFunctions.Add( wxString::FromAscii( (*it).c_str() ) );
      arrayFilterFunctionsPos.Add( pos );
      ++pos;
    }

    wxPGId filterCat = (wxPGId)NULL;
    if ( !whichWindow->getCFG4DEnabled() || !whichWindow->getCFG4DMode() )
    {
      filterCat = windowProperties->Append( new wxPropertyCategory( wxT("Filter"), wxT("Filter") ) );
      if( filterCatCollapsed )
        filterCat->SetFlagsFromString( _( "COLLAPSED" ) );
    }
    
    // ---------------------------- COMMUNICATION FILTER ---------------------------
    wxPGId commFilterCat = (wxPGId)NULL;
    if ( !whichWindow->getCFG4DEnabled() || !whichWindow->getCFG4DMode() )
    {
      commFilterCat = windowProperties->AppendIn( filterCat, new wxPropertyCategory( wxT("Communications"), wxT("Communications") ) );
      if( commFilterCatCollapsed )
        commFilterCat->SetFlagsFromString( _( "COLLAPSED" ) );
    }

    AppendCFG4DBoolPropertyWindow( windowProperties, whichWindow, commFilterCat,
            wxT("Logical"), wxT("Logical"), SINGLE_COMMLOGICAL, filter->getLogical() );

    AppendCFG4DBoolPropertyWindow( windowProperties, whichWindow, commFilterCat,
            wxT("Physical"), wxT("Physical"), SINGLE_COMMPHYSICAL, filter->getPhysical() );

    // Comm From
    wxPGId commFilterFrom = ( wxPGId )NULL;
    if ( !whichWindow->getCFG4DEnabled() || !whichWindow->getCFG4DMode() )
    {
      commFilterFrom = windowProperties->AppendIn( commFilterCat, 
                                                   new wxStringProperty( wxT("Comm from"),
                                                                         wxT("Comm from"),
                                                                         wxT("<composed>") ) );
#if wxMAJOR_VERSION<3
      if( commFilterFromCollapsed )
        commFilterFrom->SetFlagsFromString( _( "DISABLED|COLLAPSED" ) );
      else
        commFilterFrom->SetFlagsFromString( _( "DISABLED" ) );
#else
      if( commFilterFromCollapsed )
        commFilterFrom->SetFlagsFromString( _( "COLLAPSED" ) );
      commFilterFrom->Enable( false );
#endif
    }

    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      if( (*it) == filter->getCommFromFunction() )
        selected = pos;
      pos++;
    }

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, commFilterFrom,
            wxT( "Function" ), wxT("Comm from."), wxT("FromFunction"), SINGLE_COMMFROMFUNCTION,
            arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    // add communication senders to arrayStr
    arrayStr.Clear();
    vector<TObjectOrder> fromSel;
    filter->getCommFrom( fromSel );
    wxPGId commFilterFromValues = (wxPGId)NULL;

    commFilterFromValues = AppendCFG4DprvRowsSelectionPropertyWindow(
            windowProperties, whichWindow, commFilterFrom,
            wxT( "From" ), wxT("Comm from."), wxT("From"), SINGLE_COMMFROMVALUES,
            _("From - Rows Selection"), fromSel );

    if ( commFilterFromValues != (wxPGId)NULL )
    {
      if( filter->getCommFromFunction() == "All" || filter->getCommFromFunction() == "None" )
      {
#if wxMAJOR_VERSION<3
        commFilterFromValues->SetFlagsFromString( _( "DISABLED" ) );
#else
        commFilterFromValues->Enable( false );
#endif
      }
    }

    arrayStr.Clear();
    arrayInt.Clear();
    arrayStr.Add( _( "And" ) );
    arrayStr.Add( _( "Or" ) );
    arrayInt.Add( 0 );
    arrayInt.Add( 1 );
    if( filter->getOpFromTo() )
      selected = 0;
    else
      selected = 1;

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, commFilterCat,
             wxT("From/To Op"), wxT(""), wxT("FromToOp"), SINGLE_COMMFROMTOOP,
            arrayStr, arrayInt, selected );

    // Comm To
    wxPGId commFilterTo = (wxPGId) NULL; 
    if ( !whichWindow->getCFG4DEnabled() || !whichWindow->getCFG4DMode() )
    {
      commFilterTo = windowProperties->AppendIn( commFilterCat, 
                                                 new wxStringProperty( wxT("Comm to"),
                                                                       wxT("Comm to"),
                                                                       wxT("<composed>") ) );
#if wxMAJOR_VERSION<3
      if( commFilterToCollapsed )
        commFilterTo->SetFlagsFromString( _( "DISABLED|COLLAPSED" ) );
      else
        commFilterTo->SetFlagsFromString( _( "DISABLED" ) );
#else
      if( commFilterToCollapsed )
        commFilterTo->SetFlagsFromString( _( "COLLAPSED" ) );
      commFilterTo->Enable( false );
#endif
    }
    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      if( (*it) == filter->getCommToFunction() )
        selected = pos;
      pos++;
    }

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, commFilterTo,
            wxT( "Function" ), wxT("Comm to."), wxT("ToFunction"), SINGLE_COMMTOFUNCTION,
            arrayFilterFunctions, arrayFilterFunctionsPos, selected );


    arrayStr.Clear();
    vector<TObjectOrder> toSel;
    filter->getCommTo( toSel );

    wxPGId commFilterToValues = (wxPGId)NULL;
    commFilterToValues = AppendCFG4DprvRowsSelectionPropertyWindow(
            windowProperties, whichWindow, commFilterTo,
            wxT( "To" ), wxT("Comm to."), wxT( "To" ), SINGLE_COMMTOVALUES,
            _("To - Rows Selection"), toSel );

    if ( commFilterToValues != (wxPGId)NULL )
    {
      if( filter->getCommToFunction() == "All" || filter->getCommToFunction() == "None" )
      {
#if wxMAJOR_VERSION<3
        commFilterToValues->SetFlagsFromString( _( "DISABLED" ) );
#else
        commFilterToValues->Enable( false );
#endif
      }
    }

    // Comm Tag
    wxPGId commFilterTag = (wxPGId)NULL;
    if ( !whichWindow->getCFG4DEnabled() || !whichWindow->getCFG4DMode() )
    {
      commFilterTag = windowProperties->AppendIn( commFilterCat, 
                                                  new wxStringProperty( wxT("Comm tag"),
                                                                        wxT("Comm tag"),
                                                                        wxT("<composed>") ) );
#if wxMAJOR_VERSION<3
      if( commFilterTagCollapsed )
        commFilterTag->SetFlagsFromString( _( "DISABLED|COLLAPSED" ) );
      else
        commFilterTag->SetFlagsFromString( _( "DISABLED" ) );
#else
      if( commFilterTagCollapsed )
        commFilterTag->SetFlagsFromString( _( "COLLAPSED" ) );
      commFilterTag->Enable( false );
#endif
    }

    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      if( (*it) == filter->getCommTagFunction() )
        selected = pos;
      pos++;
    }

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, commFilterTag,
            wxT( "Function" ), wxT("Comm tag."), wxT("TagFunction"), SINGLE_COMMTAGFUNCTION,
            arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    arrayStr.Clear();
    vector<TCommTag> tagSel;
    filter->getCommTag( tagSel );
    for( vector<TCommTag>::iterator it = tagSel.begin(); it != tagSel.end(); it++ )
      arrayStr.Add( wxString() << (*it) );

    wxPGId commFilterTagValues = (wxPGId)NULL;
    commFilterTagValues = AppendCFG4DprvNumbersListPropertyWindow(
            windowProperties, whichWindow, commFilterTag,
            wxT( "Tag" ), wxT("Comm tag."), wxT( "Tag" ), SINGLE_COMMTAGVALUES,
            arrayStr );

    if ( commFilterTagValues != (wxPGId)NULL )
    {
      if( filter->getCommTagFunction() == "All" || filter->getCommTagFunction() == "None" )
#if wxMAJOR_VERSION<3
        commFilterTagValues->SetFlagsFromString( _( "DISABLED" ) );
#else
        commFilterTagValues->Enable( false );
#endif
    }

    arrayStr.Clear();
    arrayInt.Clear();
    arrayStr.Add( _( "And" ) );
    arrayStr.Add( _( "Or" ) );
    arrayInt.Add( 0 );
    arrayInt.Add( 1 );
    if( filter->getOpTagSize() )
      selected = 0;
    else
      selected = 1;

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, commFilterCat,
            wxT("Tag/Size Op"), wxT(""), wxT("TagSizeOp"), SINGLE_COMMTAGSIZEOP,
            arrayStr, arrayInt, selected );

    // Comm Size
    wxPGId commFilterSize = (wxPGId)NULL; 
    if ( !whichWindow->getCFG4DEnabled() || !whichWindow->getCFG4DMode() )
    {
      commFilterSize = windowProperties->AppendIn( commFilterCat,
                                                   new wxStringProperty( wxT("Comm size"), wxT("Comm size"),wxT("<composed>")));
#if wxMAJOR_VERSION<3
      if( commFilterSizeCollapsed )
        commFilterSize->SetFlagsFromString( _( "DISABLED|COLLAPSED" ) );
      else
        commFilterSize->SetFlagsFromString( _( "DISABLED" ) );
#else
      if( commFilterSizeCollapsed )
        commFilterSize->SetFlagsFromString( _( "COLLAPSED" ) );
      commFilterSize->Enable( false );
#endif
    }

    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      if( (*it) == filter->getCommSizeFunction() )
        selected = pos;
      pos++;
    }

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, commFilterSize,
            wxT("Function"), wxT("Comm size."), wxT("SizeFunction"), SINGLE_COMMSIZEFUNCTION,
            arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    arrayStr.Clear();
    vector<TCommSize> sizeSel;
    filter->getCommSize( sizeSel );
    for( vector<TCommSize>::iterator it = sizeSel.begin(); it != sizeSel.end(); it++ )
      arrayStr.Add( wxString() << (*it) );

    wxPGId commFilterSizeValues = (wxPGId)NULL;
    commFilterSizeValues = AppendCFG4DprvNumbersListPropertyWindow(
            windowProperties, whichWindow, commFilterSize,
            wxT("Size"), wxT("Comm size."), wxT("Size"), SINGLE_COMMSIZEVALUES,
            arrayStr );

    if ( commFilterSizeValues != (wxPGId)NULL )
    {
      if( filter->getCommSizeFunction() == "All" || filter->getCommSizeFunction() == "None" )
      {
#if wxMAJOR_VERSION<3
        commFilterSizeValues->SetFlagsFromString( _( "DISABLED" ) );
#else
        commFilterSizeValues->Enable( false );
#endif
      }
    }

    // Comm BandWidth
    wxPGId commFilterBW = (wxPGId)NULL;
    if ( !whichWindow->getCFG4DEnabled() || !whichWindow->getCFG4DMode() )
    {
      commFilterBW = windowProperties->AppendIn(  commFilterCat, 
                                                  new wxStringProperty( wxT("Comm bandwidth"), 
                                                                        wxT("Comm bandwidth"),
                                                                        wxT("<composed>") ) );
#if wxMAJOR_VERSION<3
      if( commFilterBWCollapsed )
        commFilterBW->SetFlagsFromString( _( "DISABLED|COLLAPSED" ) );
      else
        commFilterBW->SetFlagsFromString( _( "DISABLED" ) );
#else
      if( commFilterBWCollapsed )
        commFilterBW->SetFlagsFromString( _( "COLLAPSED" ) );
      commFilterBW->Enable( false );
#endif
    }

    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      if( (*it) == filter->getBandWidthFunction() )
        selected = pos;
      pos++;
    }

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, commFilterBW,
            wxT("Function"), wxT("Comm bandwidth."), wxT("BWFunction"), SINGLE_COMMBANDWIDTHFUNCTION,
            arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    arrayStr.Clear();
    vector<TSemanticValue> bwSel;
    filter->getBandWidth( bwSel );
    for( vector<TSemanticValue>::iterator it = bwSel.begin(); it != bwSel.end(); it++ )
      arrayStr.Add( wxString() << (*it) );

    wxPGId commFilterBandWidthValues = (wxPGId)NULL;
    commFilterBandWidthValues = AppendCFG4DprvNumbersListPropertyWindow(
            windowProperties, whichWindow, commFilterBW,
            wxT("Bandwidth"), wxT("Comm bandwidth."), wxT("Bandwidth"), SINGLE_COMMBANDWIDTHVALUES,
            arrayStr );

    if ( commFilterBandWidthValues != (wxPGId)NULL )
    {
      if( filter->getBandWidthFunction() == "All" || filter->getBandWidthFunction() == "None" )
      {
#if wxMAJOR_VERSION<3
        commFilterBandWidthValues->SetFlagsFromString( _( "DISABLED" ) );
#else
        commFilterBandWidthValues->Enable( false );
#endif
      }
    }

    // -------------------------------- EVENT FILTER -------------------------------
    wxPGId eventFilterCat = (wxPGId)NULL;
    if ( !whichWindow->getCFG4DEnabled() || !whichWindow->getCFG4DMode() )
    {
      eventFilterCat = windowProperties->AppendIn( filterCat, new wxPropertyCategory( wxT("Events"), wxT("Events") ) );
      if( eventFilterCatCollapsed )
        eventFilterCat->SetFlagsFromString( _( "COLLAPSED" ) );
    }

    // Event Type
    wxPGId eventFilterType = (wxPGId)NULL;
    if ( !whichWindow->getCFG4DEnabled() || !whichWindow->getCFG4DMode() )
    {
      eventFilterType = windowProperties->AppendIn( eventFilterCat, 
                                                    new wxStringProperty( wxT("Event type"), 
                                                                          wxT("Event type"),
                                                                          wxT("<composed>") ) );
#if wxMAJOR_VERSION<3
      if( eventFilterTypeCollapsed )
        eventFilterType->SetFlagsFromString( _( "DISABLED|COLLAPSED" ) );
      else
        eventFilterType->SetFlagsFromString( _( "DISABLED" ) );
#else
      if( eventFilterTypeCollapsed )
        eventFilterType->SetFlagsFromString( _( "COLLAPSED" ) );
      eventFilterType->Enable( false );
#endif
    }

    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin(); it != filterFunctions.end(); ++it )
    {
      if( (*it) == filter->getEventTypeFunction() )
        selected = pos;
      pos++;
    }

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, eventFilterType,
            wxT("Function"), wxT("Event type."), wxT("TypeFunction"), SINGLE_EVENTTYPEFUNCTION,
            arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    set<TEventType> allTypes;
    vector<TEventType> labeledTypes;
    whichWindow->getTrace()->getEventLabels().getTypes( labeledTypes );
    allTypes = whichWindow->getTrace()->getLoadedEvents();
    allTypes.insert( labeledTypes.begin(), labeledTypes.end() );
    arrayStr.Clear();
    arrayInt.Clear();
    for( set<TEventType>::iterator it = allTypes.begin(); it != allTypes.end(); ++it )
    {
      arrayInt.Add( (*it) );
      string tmpstr;
      whichWindow->getTrace()->getEventLabels().getEventTypeLabel( (*it), tmpstr );
      arrayStr.Add( wxString() << (*it) << _( " " ) << wxString::FromAscii( tmpstr.c_str() ) );
    }

    wxPGChoices typeChoices( arrayStr, arrayInt );

    wxPGId eventFilterTypeValues = (wxPGId)NULL;
    eventFilterTypeValues = AppendCFG4DprvEventInfoPropertyWindow(
            windowProperties, whichWindow, eventFilterType,
            wxT("Types"), wxT("Event type."), wxT("Types"), SINGLE_EVENTTYPEVALUES,
            typeChoices );

    if ( eventFilterTypeValues != (wxPGId)NULL )
    {
#if wxMAJOR_VERSION<3
      wxPGId auxValProp = windowProperties->GetProperty( wxT("Event type.Types") )->GetId();
#else
      wxPGId auxValProp = windowProperties->GetProperty( wxT("Event type.Types") );
#endif
      windowProperties->SetPropertyAttribute( auxValProp, wxPG_ATTR_MULTICHOICE_USERSTRINGMODE, 1 );

      if( filter->getEventTypeFunction() == "All" ||
          filter->getEventTypeFunction() == "None" )
#if wxMAJOR_VERSION<3
        eventFilterTypeValues->SetFlagsFromString( _( "DISABLED" ) );
#else
        eventFilterTypeValues->Enable( false );
#endif
    }

    arrayStr.Clear();
    arrayInt.Clear();
    arrayStr.Add( _( "And" ) );
    arrayStr.Add( _( "Or" ) );
    arrayInt.Add( 0 );
    arrayInt.Add( 1 );
    if( filter->getOpTypeValue() )
      selected = 0;
    else
      selected = 1;

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, eventFilterCat,
            wxT("Type/Value Op"), wxT(""), wxT("TypeValueOp"), SINGLE_EVENTTYPEVALUESOP,
            arrayStr, arrayInt, selected );

    // Event Value
    wxPGId eventFilterValue = (wxPGId)NULL;
    if ( !whichWindow->getCFG4DEnabled() || !whichWindow->getCFG4DMode() )
    {
      eventFilterValue = windowProperties->AppendIn( eventFilterCat, 
                                                     new wxStringProperty( wxT("Event value"), 
                                                                           wxT("Event value"),
                                                                           wxT("<composed>") ) );
#if wxMAJOR_VERSION<3
      if( eventFilterValueCollapsed )
        eventFilterValue->SetFlagsFromString( _( "DISABLED|COLLAPSED" ) );
      else
        eventFilterValue->SetFlagsFromString( _( "DISABLED" ) );
#else
      if( eventFilterValueCollapsed )
        eventFilterValue->SetFlagsFromString( _( "COLLAPSED" ) );
      eventFilterValue->Enable( false );
#endif
    }

    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      if( (*it) == filter->getEventValueFunction() )
        selected = pos;
      pos++;
    }

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, eventFilterValue,
            wxT("Function"),  wxT("Event value."), wxT("ValueFunction"), SINGLE_EVENTVALUEFUNCTION,
            arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    wxPGId eventFilterValueValues = (wxPGId)NULL;
    eventFilterValueValues = AppendCFG4DprvEventInfoPropertyWindow(
            windowProperties, whichWindow, eventFilterValue,
            wxT("Values"), wxT("Event value."), wxT("Values"), SINGLE_EVENTVALUEVALUES,
            typeChoices );

    if ( eventFilterValueValues != (wxPGId) NULL )
    {
#if wxMAJOR_VERSION<3
      wxPGId auxValProp = windowProperties->GetProperty( wxT("Event value.Values") )->GetId();
#else
      wxPGId auxValProp = windowProperties->GetProperty( wxT("Event value.Values") );
#endif
      windowProperties->SetPropertyAttribute( auxValProp, wxPG_ATTR_MULTICHOICE_USERSTRINGMODE, 1 );

      if( filter->getEventValueFunction() == "All" || filter->getEventValueFunction() == "None" )
      {
#if wxMAJOR_VERSION<3
        eventFilterValueValues->SetFlagsFromString( _( "DISABLED" ) );
#else
        eventFilterValueValues->Enable( false );
#endif
      }
    }
  }

  // END of Filter related properties

  //-------------------------------------------------------------------------
  // Semantic related properties
  //-------------------------------------------------------------------------
  TParamValue pValues;

  wxPGId semanticCat = (wxPGId)NULL;
  if ( !whichWindow->getCFG4DEnabled() || !whichWindow->getCFG4DMode() )
  {
    semanticCat = windowProperties->Append( new wxPropertyCategory( wxT("Semantic"), wxT("Semantic") ) );
    if( semanticCatCollapsed )
      semanticCat->SetFlagsFromString( _( "COLLAPSED" ) );
  }

  vector<string> composeFunctions;
  whichWindow->getAllSemanticFunctions( COMPOSE_GROUP, composeFunctions );
  arrayComposeFunctions.Clear();
  pos = 0;
  for( vector<string>::iterator it = composeFunctions.begin();
       it != composeFunctions.end(); ++it )
  {
    arrayComposeFunctions.Add( wxString::FromAscii( (*it).c_str() ) );
    arrayComposeFunctionsPos.Add( pos );
    ++pos;
  }
  
  pos = 0;
  selected = -1;
  for( vector<string>::iterator it = composeFunctions.begin();
       it != composeFunctions.end(); ++it )
  {
    if( (*it) == whichWindow->getLevelFunction( TOPCOMPOSE1 ) )
      selected = pos;
    pos++;
  }

  AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, semanticCat,
            wxT("Top Compose 1"), wxT(""), wxT("Top Compose 1"), SINGLE_TOPCOMPOSE1,
            arrayComposeFunctions, arrayComposeFunctionsPos, selected );

  semanticFunctionParameter( windowProperties, whichWindow, semanticCat, TOPCOMPOSE1 );

  pos = 0;
  selected = -1;
  for( vector<string>::iterator it = composeFunctions.begin();
       it != composeFunctions.end(); ++it )
  {
    if( (*it) == whichWindow->getLevelFunction( TOPCOMPOSE2 ) )
      selected = pos;
    ++pos;
  }

  AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, semanticCat,
            wxT("Top Compose 2"), wxT(""), wxT("Top Compose 2"), SINGLE_TOPCOMPOSE2,
            arrayComposeFunctions, arrayComposeFunctionsPos, selected );

  semanticFunctionParameter( windowProperties, whichWindow, semanticCat, TOPCOMPOSE2 );

  vector<string> notThreadFunctions;
  whichWindow->getAllSemanticFunctions( NOTTHREAD_GROUP, notThreadFunctions );
  arrayNotThreadFunctions.Clear();
  pos = 0;
  for( vector<string>::iterator it = notThreadFunctions.begin();
       it != notThreadFunctions.end(); ++it )
  {
    arrayNotThreadFunctions.Add( wxString::FromAscii( (*it).c_str() ) );
    arrayNotThreadFunctionsPos.Add( pos );
    ++pos;
  }

  TWindowLevel level = whichWindow->getLevel();
  TWindowLevel minAcceptLevel = whichWindow->getMinAcceptableLevel();
  
  if( level >= WORKLOAD && level <= THREAD )
  {
    if( level == WORKLOAD )
    {
      pos = 0;
      selected = -1;
      for( vector<string>::iterator it = composeFunctions.begin();
           it != composeFunctions.end(); ++it )
      {
        if( (*it) == whichWindow->getLevelFunction( COMPOSEWORKLOAD ) )
          selected = pos;
        ++pos;
      }

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, semanticCat,
                wxT("Compose Workload"), wxT(""), wxT("Compose Workload"), SINGLE_COMPOSEWORKLOAD,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, semanticCat, COMPOSEWORKLOAD );

      if ( !whichWindow->isDerivedWindow() || minAcceptLevel > WORKLOAD )
      {
        pos = 0;
        selected = -1;
        for( vector<string>::iterator it = notThreadFunctions.begin();
             it != notThreadFunctions.end(); ++it )
        {
          if( (*it) == whichWindow->getLevelFunction( WORKLOAD ) )
            selected = pos;
          ++pos;
        }

        AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, semanticCat,
                  wxT("Workload"), wxT(""), wxT("Workload"), SINGLE_WORKLOAD,
                  arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected );
        semanticFunctionParameter( windowProperties, whichWindow, semanticCat, WORKLOAD );
      }
    }
      
    if( level <= APPLICATION && minAcceptLevel >= APPLICATION )
    {
      pos = 0;
      selected = -1;
      for( vector<string>::iterator it = composeFunctions.begin();
           it != composeFunctions.end(); ++it )
      {
        if( (*it) == whichWindow->getLevelFunction( COMPOSEAPPLICATION ) )
          selected = pos;
        ++pos;
      }

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, semanticCat,
                wxT("Compose Appl"), wxT(""), wxT("Compose Appl"), SINGLE_COMPOSEAPPL,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, semanticCat, COMPOSEAPPLICATION );

      if ( !whichWindow->isDerivedWindow() || minAcceptLevel > APPLICATION )
      {
        pos = 0;
        selected = -1;
        for( vector<string>::iterator it = notThreadFunctions.begin();
             it != notThreadFunctions.end(); ++it )
        {
          if( (*it) == whichWindow->getLevelFunction( APPLICATION ) )
            selected = pos;
          ++pos;
        }

        AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, semanticCat,
                  wxT("Application"), wxT(""), wxT("Application"), SINGLE_APPLICATION,
                  arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected );
        semanticFunctionParameter( windowProperties, whichWindow, semanticCat, APPLICATION );
      }
    }

    if( level <= TASK && minAcceptLevel >= TASK )
    {
      pos = 0;
      selected = -1;
      for( vector<string>::iterator it = composeFunctions.begin();
           it != composeFunctions.end(); ++it )
      {
        if( (*it) == whichWindow->getLevelFunction( COMPOSETASK ) )
          selected = pos;
        ++pos;
      }

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, semanticCat,
                wxT("Compose Task"), wxT(""), wxT("Compose Task"), SINGLE_COMPOSETASK,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, semanticCat, COMPOSETASK );

      if ( !whichWindow->isDerivedWindow() || minAcceptLevel > TASK )
      {
        pos = 0;
        selected = -1;
        for( vector<string>::iterator it = notThreadFunctions.begin();
             it != notThreadFunctions.end(); ++it )
        {
          if( (*it) == whichWindow->getLevelFunction( TASK ) )
            selected = pos;
          ++pos;
        }

        AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, semanticCat,
                  wxT("Task"), wxT(""), wxT("Task"), SINGLE_TASK,
                  arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected );
        semanticFunctionParameter( windowProperties, whichWindow, semanticCat, TASK );
      }
    }
    
    if( whichWindow->isDerivedWindow() && minAcceptLevel >= THREAD )
    {
      pos = 0;
      selected = -1;
      for( vector<string>::iterator it = composeFunctions.begin();
           it != composeFunctions.end(); ++it )
      {
        if( (*it) == whichWindow->getLevelFunction( COMPOSETHREAD ) )
          selected = pos;
        ++pos;
      }

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, semanticCat,
                wxT("Compose Thread"), wxT(""), wxT("Compose Thread"), SINGLE_COMPOSETHREAD,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, semanticCat, COMPOSETHREAD );
    }
  }
  else if( level >= SYSTEM && level <= CPU )
  {
    if( level == SYSTEM && minAcceptLevel >= SYSTEM )
    {
      pos = 0;
      selected = -1;
      for( vector<string>::iterator it = composeFunctions.begin();
           it != composeFunctions.end(); ++it )
      {
        if( (*it) == whichWindow->getLevelFunction( COMPOSESYSTEM ) )
          selected = pos;
        ++pos;
      }

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, semanticCat,
                wxT("Compose System"), wxT(""), wxT("Compose System"), SINGLE_COMPOSESYSTEM,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, semanticCat, COMPOSESYSTEM );

      if ( !whichWindow->isDerivedWindow() || minAcceptLevel > SYSTEM )
      {
        pos = 0;
        selected = -1;
        for( vector<string>::iterator it = notThreadFunctions.begin();
             it != notThreadFunctions.end(); ++it )
        {
          if( (*it) == whichWindow->getLevelFunction( SYSTEM ) )
            selected = pos;
          ++pos;
        }

        AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, semanticCat,
                  wxT("System"), wxT(""), wxT("System"), SINGLE_SYSTEM,
                  arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected );
        semanticFunctionParameter( windowProperties, whichWindow, semanticCat, SYSTEM );
      }
    }
      
    if( level <= NODE && minAcceptLevel >= NODE )
    {
      pos = 0;
      selected = -1;
      for( vector<string>::iterator it = composeFunctions.begin();
           it != composeFunctions.end(); ++it )
      {
        if( (*it) == whichWindow->getLevelFunction( COMPOSENODE ) )
          selected = pos;
        ++pos;
      }

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, semanticCat,
                wxT("Compose Node"), wxT(""), wxT("Compose Node"), SINGLE_COMPOSENODE,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, semanticCat, COMPOSENODE );

      if ( !whichWindow->isDerivedWindow() || minAcceptLevel > NODE )
      {
        pos = 0;
        selected = -1;
        for( vector<string>::iterator it = notThreadFunctions.begin();
             it != notThreadFunctions.end(); ++it )
        {
          if( (*it) == whichWindow->getLevelFunction( NODE ) )
            selected = pos;
          ++pos;
        }

        AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, semanticCat,
                  wxT("Node"), wxT(""), wxT("Node"), SINGLE_NODE,
                  arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected );
        semanticFunctionParameter( windowProperties, whichWindow, semanticCat, NODE );
      }
    }
      
    if( level <= CPU && minAcceptLevel >= CPU )
    {
      pos = 0;
      selected = -1;
      for( vector<string>::iterator it = composeFunctions.begin();
           it != composeFunctions.end(); ++it )
      {
        if( (*it) == whichWindow->getLevelFunction( COMPOSECPU ) )
          selected = pos;
        ++pos;
      }

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, semanticCat,
                wxT("Compose CPU"), wxT(""), wxT("Compose CPU"), SINGLE_COMPOSECPU,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );

      semanticFunctionParameter( windowProperties, whichWindow, semanticCat, COMPOSECPU );
      
      if( !whichWindow->isDerivedWindow() )
      {
        pos = 0;
        selected = -1;
        arrayStr.Clear();
        arrayInt.Clear();
        vector<string> cpuFunctions;
        whichWindow->getAllSemanticFunctions( CPU_GROUP, cpuFunctions );
        for( vector<string>::iterator it = cpuFunctions.begin();
             it != cpuFunctions.end(); ++it )
        {
          arrayStr.Add( wxString::FromAscii( (*it).c_str() ) );
          arrayInt.Add( pos );
          if( (*it) == whichWindow->getLevelFunction( CPU ) )
            selected = pos;
          ++pos;
        }

        AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, semanticCat,
                  wxT("CPU"), wxT(""), wxT("CPU"), SINGLE_CPU,
                  arrayStr, arrayInt, selected );

        semanticFunctionParameter( windowProperties, whichWindow, semanticCat, CPU );
      }
    }
  }
  
  if( !whichWindow->isDerivedWindow() )
  {
    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = composeFunctions.begin();
         it != composeFunctions.end(); ++it )
    {
      if( (*it) == whichWindow->getLevelFunction( COMPOSETHREAD ) )
        selected = pos;
      ++pos;
    }

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, semanticCat,
              wxT("Compose Thread"), wxT(""), wxT("Compose Thread"), SINGLE_COMPOSETHREAD,
              arrayComposeFunctions, arrayComposeFunctionsPos, selected );
    semanticFunctionParameter( windowProperties, whichWindow, semanticCat, COMPOSETHREAD );

    vector<vector<string> > threadFunctions;
    vector<string> levels;
    levels.push_back( "State" );
    threadFunctions.push_back( vector<string>() );
    whichWindow->getAllSemanticFunctions( STATE_GROUP, threadFunctions[ 0 ] );
    levels.push_back( "Event" );
    threadFunctions.push_back( vector<string>() );
    whichWindow->getAllSemanticFunctions( EVENT_GROUP, threadFunctions[ 1 ] );
    levels.push_back( "Communication" );
    threadFunctions.push_back( vector<string>() );
    whichWindow->getAllSemanticFunctions( COMM_GROUP, threadFunctions[ 2 ] );
    levels.push_back( "Object" );
    threadFunctions.push_back( vector<string>() );
    whichWindow->getAllSemanticFunctions( OBJECT_GROUP, threadFunctions[ 3 ] );

    wxString auxDefaultValue = wxString::FromAscii( whichWindow->getLevelFunction( THREAD ).c_str() );
    AppendCFG4DprvSemanticThreadPropertyWindow( windowProperties, whichWindow, semanticCat,
              wxT("Thread"), wxT(""), wxT("Thread"), SINGLE_THREAD,
              levels, threadFunctions, auxDefaultValue );
    semanticFunctionParameter( windowProperties, whichWindow, semanticCat, THREAD );
  }
  
  if( whichWindow->isDerivedWindow() )
  {
    AppendCFG4DFloatPropertyWindow( windowProperties, whichWindow, dummyPGId,
            wxT("Factor #1"), wxT("Factor #1"), (TSingleTimelineProperties)DERIVED_FACTOR1,
            whichWindow->getFactor( 0 ) );

    vector<string> derivedFunctions;
    whichWindow->getAllSemanticFunctions( DERIVED_GROUP, derivedFunctions );
    arrayStr.Clear();
    arrayInt.Clear();
    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = derivedFunctions.begin();
         it != derivedFunctions.end(); ++it )
    {
      arrayStr.Add( wxString::FromAscii( (*it).c_str() ) );
      arrayInt.Add( pos );
      if( (*it) == whichWindow->getLevelFunction( DERIVED ) )
        selected = pos;
      ++pos;
    }

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, semanticCat,
              wxT("Derived"), wxT(""), wxT("Derived"), (TSingleTimelineProperties)DERIVED_DERIVED,
              arrayStr, arrayInt, selected );
    semanticFunctionParameter( windowProperties, whichWindow, semanticCat, DERIVED );

    AppendCFG4DFloatPropertyWindow( windowProperties, whichWindow, dummyPGId,
            wxT("Factor #2"), wxT("Factor #2"), (TSingleTimelineProperties)DERIVED_FACTOR2,
            whichWindow->getFactor( 1 ) );
  }
  // END of Semantic related properties
  
  windowProperties->SetPropertyAttributeAll( wxPG_BOOL_USE_CHECKBOX, true );

  windowProperties->Refresh();
  windowProperties->Thaw();
}


void updateHistogramProperties( wxPropertyGrid* windowProperties, Histogram *whichHisto )
{
  PRV_UINT32 precision = ParaverConfig::getInstance()->getTimelinePrecision();

  updateCategoriesState( windowProperties );
  
  wxArrayString arrayStr;
  wxArrayInt arrayInt;
  int selected, pos;

  whichHisto->setChanged( false );
  windowProperties->Freeze();
  windowProperties->Clear();


  if ( whichHisto->getCFG4DEnabled() )
  {
    arrayStr.Clear();
    arrayInt.Clear();
    selected = whichHisto->getCFG4DMode() ? 1: 0;
    arrayStr.Add( wxT( "Full" ) );
    arrayStr.Add( wxT( "Basic" ) );
    arrayInt.Add( 0 );
    arrayInt.Add( 1 );

    wxEnumProperty *tmpSelector = new wxEnumProperty( wxT("Properties Mode"), wxT("Mode"), arrayStr, arrayInt, selected );
    windowProperties->Append( tmpSelector );
  }

  wxPGId dummyPGId = (wxPGId)NULL; // used to append always to windowProperties

  AppendCFG4DStringPropertyHistogram( windowProperties, whichHisto, dummyPGId,
          wxT("Name"), wxT("Name"), HISTOGRAM_NAME, whichHisto->getName() );

  AppendCFG4DStringPropertyHistogram( windowProperties, whichHisto, dummyPGId,
          wxT("Begin time"), wxT("Begin time"), HISTOGRAM_BEGINTIME,
          LabelConstructor::timeLabel(
                  whichHisto->getControlWindow()->traceUnitsToWindowUnits( whichHisto->getBeginTime() ),
                  whichHisto->getControlWindow()->getTimeUnit(),
                  precision ));

  AppendCFG4DStringPropertyHistogram( windowProperties, whichHisto, dummyPGId,
          wxT("End time"), wxT("End time"), HISTOGRAM_ENDTIME,
          LabelConstructor::timeLabel(
                  whichHisto->getControlWindow()->traceUnitsToWindowUnits( whichHisto->getEndTime() ),
                  whichHisto->getControlWindow()->getTimeUnit(),
                  precision ));

  // Control Window related properties
  wxPGId ctrlCat = (wxPGId)NULL;
  if ( !whichHisto->getCFG4DEnabled() || !whichHisto->getCFG4DMode() )
  {
    ctrlCat = windowProperties->Append( new wxPropertyCategory( wxT("Control"), wxT("Control") ) );
    if( ctrlCatCollapsed )
      ctrlCat->SetFlagsFromString( _( "COLLAPSED" ) );
  }

  vector<TWindowID> validWin;
  Window *dataWindow = ( whichHisto->getDataWindow() == NULL ) ? whichHisto->getControlWindow() :
                                                                 whichHisto->getDataWindow();
  LoadedWindows::getInstance()->getValidControlWindow( dataWindow, whichHisto->getExtraControlWindow(), validWin );

  arrayStr.Clear();
  arrayInt.Clear();
  selected = -1;
  for( vector<TWindowID>::iterator it = validWin.begin(); it != validWin.end(); ++it )
  {
    arrayStr.Add( wxString::FromAscii( LoadedWindows::getInstance()->getWindow( (*it) )->getName().c_str() ) );
    arrayInt.Add( (*it) );
    // Do we need this -if- here?
    if( LoadedWindows::getInstance()->getWindow( (*it) ) == whichHisto->getControlWindow() )
      selected = (*it);
  }

  AppendCFG4DEnumPropertyHistogram( windowProperties, whichHisto, ctrlCat,
          wxT("Window"), wxT("ControlWindow"), HISTOGRAM_CONTROLWINDOW, arrayStr, arrayInt, selected );

  AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, ctrlCat,
          wxT("Minimum"), wxT("ControlMinimum"), HISTOGRAM_CONTROLMINIMUM, whichHisto->getControlMin() );
  AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, ctrlCat,
          wxT("Maximum"), wxT("ControlMaximum"), HISTOGRAM_CONTROLMAXIMUM, whichHisto->getControlMax() );
  AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, ctrlCat,
          wxT("Delta"), wxT("ControlDelta"), HISTOGRAM_CONTROLDELTA, whichHisto->getControlDelta() );

  // Statistic related properties
  wxPGId statCat = (wxPGId)NULL;
  if ( !whichHisto->getCFG4DEnabled() || !whichHisto->getCFG4DMode() )
  {
    statCat = windowProperties->Append( new wxPropertyCategory( wxT("Statistics"), wxT("Statistics") ) );
    if( statCatCollapsed )
      statCat->SetFlagsFromString( _( "COLLAPSED" ) );
  }
  
//  windowProperties->AppendIn( statCat, new wxBoolProperty( wxT("Calculate all"), wxPG_LABEL, whichHisto->getCalculateAll() ) );
  
  vector<string> tmpV;
  whichHisto->getGroupsLabels( tmpV );
  arrayStr.Clear();
  arrayInt.Clear();
  for( vector<string>::iterator it = tmpV.begin(); it != tmpV.end(); ++it )
    arrayStr.Add( wxString::FromAscii( (*it).c_str() ) );
  arrayInt.Add( 0 );
  arrayInt.Add( 1 );
  if( whichHisto->itsCommunicationStat( whichHisto->getCurrentStat() ) )
    selected = 0;
  else
    selected = 1;

  AppendCFG4DEnumPropertyHistogram( windowProperties, whichHisto, statCat,
          wxT("Type"), wxT("Type"), HISTOGRAM_TYPE, arrayStr, arrayInt, selected );

  tmpV.clear();
  arrayStr.Clear();
  arrayInt.Clear();

  bool getOriginalList = ( !whichHisto->getCFG4DEnabled() || !whichHisto->getCFG4DMode() );
  whichHisto->getStatisticsLabels( tmpV, selected, getOriginalList );

  string selectedStat;
  if ( getOriginalList )
  {
    selectedStat = whichHisto->getCurrentStat();
  }
  else
  {
    map< string, string > statList( whichHisto->getCFG4DStatisticsAliasList() );
    selectedStat = statList[  whichHisto->getCurrentStat() ];
  }

  pos = 0;
  selected = -1;
  for( vector<string>::iterator it = tmpV.begin(); it != tmpV.end(); ++it )
  {
    arrayStr.Add( wxString::FromAscii( (*it).c_str() ) );
    arrayInt.Add( pos );
    if( (*it) == selectedStat )
      selected = pos;
    pos++;
  }

  if( selected == -1 ) selected = 0;

  AppendCFG4DEnumPropertyHistogram( windowProperties, whichHisto, statCat,
          wxT("Statistic"), wxT("Statistic"), HISTOGRAM_STATISTIC, arrayStr, arrayInt, selected );

  AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, statCat,
          wxT("Minimum Gradient"), wxT("DataMinimum"), HISTOGRAM_MINIMUMGRADIENT, whichHisto->getMinGradient() );

  AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, statCat,
          wxT("Maximum Gradient"), wxT("DataMaximum"), HISTOGRAM_MAXIMUMGRADIENT, whichHisto->getMaxGradient() );

  // Data Window related properties
  wxPGId dataCat = (wxPGId)NULL;
  if ( !whichHisto->getCFG4DEnabled() || !whichHisto->getCFG4DMode() )
  {
    dataCat = windowProperties->Append( new wxPropertyCategory( wxT("Data"), wxT("Data") ) );
    if( dataCatCollapsed )
      dataCat->SetFlagsFromString( _( "COLLAPSED" ) );
  }

  validWin.clear();  //  vector<TWindowID> validWin;
  LoadedWindows::getInstance()->getValidDataWindow( whichHisto->getControlWindow(),
                                                    whichHisto->getExtraControlWindow(),
                                                    validWin );
  arrayStr.Clear();
  arrayInt.Clear();
  selected = -1;
  for( vector<TWindowID>::iterator it = validWin.begin(); it != validWin.end(); ++it )
  {
    arrayStr.Add( wxString::FromAscii( LoadedWindows::getInstance()->getWindow( (*it) )->getName().c_str() ) );
    arrayInt.Add( (*it) );
    if( LoadedWindows::getInstance()->getWindow( (*it) ) == whichHisto->getDataWindow() )
      selected = (*it);
  }

  AppendCFG4DEnumPropertyHistogram( windowProperties, whichHisto, dataCat,
           wxT("Window"), wxT("DataWindow"), HISTOGRAM_DATAWINDOW, arrayStr, arrayInt, selected );

  // 3rd window related properties
  wxPGId thirdWinCat = (wxPGId)NULL;
  if ( !whichHisto->getCFG4DEnabled() || !whichHisto->getCFG4DMode() )
  {
    thirdWinCat = windowProperties->Append( new wxPropertyCategory( wxT("3D"), wxT("3D") ) );
    if( thirdWinCatCollapsed )
      thirdWinCat->SetFlagsFromString( _( "COLLAPSED" ) );
  }

  validWin.clear();
  dataWindow = ( whichHisto->getDataWindow() == NULL ) ? whichHisto->getControlWindow() :
                                                         whichHisto->getDataWindow();
  LoadedWindows::getInstance()->getValidControlWindow( dataWindow, whichHisto->getControlWindow(), validWin );
  arrayStr.Clear();
  arrayInt.Clear();
  selected = -1;
  arrayStr.Add( wxT( "none" ) );
  arrayInt.Add( -1 );
  for( vector<TWindowID>::iterator it = validWin.begin(); it != validWin.end(); ++it )
  {
    arrayStr.Add( wxString::FromAscii( LoadedWindows::getInstance()->getWindow( (*it) )->getName().c_str() ) );
    arrayInt.Add( (*it) );
    if( ( LoadedWindows::getInstance()->getWindow( (*it) ) == whichHisto->getExtraControlWindow() ) )
      selected = (*it);
  }

  AppendCFG4DEnumPropertyHistogram( windowProperties, whichHisto, thirdWinCat,
          wxT("3rd Window"), wxT("3D3rdWindow"), HISTOGRAM_3D3RDWINDOW, arrayStr, arrayInt, selected );

  AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, thirdWinCat,
          wxT("Minimum"), wxT("3DMinimum"), HISTOGRAM_3DMINIMUM, whichHisto->getExtraControlMin() );

  AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, thirdWinCat,
          wxT("Maximum"), wxT("3DMaximum"), HISTOGRAM_3DMAXIMUM, whichHisto->getExtraControlMax() );

  AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, thirdWinCat,
          wxT("Delta"), wxT("3DDelta"), HISTOGRAM_3DDELTA, whichHisto->getExtraControlDelta() );

  arrayStr.Clear();
  arrayInt.Clear();
  pos = 0;
  selected = -1;
  for( THistogramColumn i = 0; i < whichHisto->getNumPlanes(); i++ )
  {
    if( whichHisto->planeWithValues( i ) )
    {
      arrayStr.Add( wxString::FromAscii( whichHisto->getPlaneLabel( i ).c_str() ) );
      arrayInt.Add( pos );
      if( pos == whichHisto->getSelectedPlane() )
        selected = pos;
    }
    pos++;
  }

  AppendCFG4DEnumPropertyHistogram( windowProperties, whichHisto, thirdWinCat,
          wxT("Plane"), wxT("3DPlane"), HISTOGRAM_3DPLANE, arrayStr, arrayInt, selected );

  if( !whichHisto->getThreeDimensions() )
  {
    wxFloatProperty *auxFloatProperty;
    auxFloatProperty = (wxFloatProperty *)windowProperties->GetProperty( wxT("3DMinimum") );
    if ( auxFloatProperty != NULL )
#if wxMAJOR_VERSION<3
      auxFloatProperty->SetFlagsFromString( _( "DISABLED" ) );
#else
      auxFloatProperty->Enable( false );
#endif

    auxFloatProperty = (wxFloatProperty *) windowProperties->GetProperty( wxT("3DMaximum") );
    if ( auxFloatProperty != NULL )
#if wxMAJOR_VERSION<3
      auxFloatProperty->SetFlagsFromString( _( "DISABLED" ) );
#else
      auxFloatProperty->Enable( false );
#endif

    auxFloatProperty = (wxFloatProperty *)windowProperties->GetProperty( wxT("3DDelta") );
    if ( auxFloatProperty != NULL )
#if wxMAJOR_VERSION<3
      auxFloatProperty->SetFlagsFromString( _( "DISABLED" ) );
#else
      auxFloatProperty->Enable( false );
#endif

    wxEnumProperty *auxEnumProperty;
    auxEnumProperty = (wxEnumProperty *)windowProperties->GetProperty( wxT("3DPlane") );
    if ( auxEnumProperty != NULL )
#if wxMAJOR_VERSION<3
      auxEnumProperty->SetFlagsFromString( _( "DISABLED" ) );
#else
      auxEnumProperty->Enable( false );
#endif
  }

  // Test timelineTreeSelector
  wxPGId retId = (wxPGId)NULL;
  prvTimelineTreeProperty *auxProperty = NULL;
  auxProperty = new prvTimelineTreeProperty( wxT("test timeline tree"), wxT("testTimelineTree"), wxT("test value") );
  retId = windowProperties->Append( auxProperty );
  // Test timelineTreeSelector end

  windowProperties->SetPropertyAttributeAll( wxPG_BOOL_USE_CHECKBOX, true );
  windowProperties->Refresh();
  windowProperties->Thaw();
}
