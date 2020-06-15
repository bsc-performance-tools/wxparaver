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

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "pg_util.h"
#include "wxparaverapp.h"
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
#if wxMAJOR_VERSION>=3
#include <wx/artprov.h>
#endif

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
static unsigned int propNameCounter   = 0;

/*
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
*/

enum ButtonType { NO_BUTTON = 0, PLUS_BUTTON, MINUS_BUTTON, BOTH_BUTTONS };

class wxChoiceAndMinusButtonEditor : public wxPGChoiceEditor
{
#if wxMAJOR_VERSION>=3
  wxDECLARE_DYNAMIC_CLASS( wxChoiceAndMinusButtonEditor );
#else
  WX_PG_DECLARE_EDITOR_CLASS( wxChoiceAndMinusButtonEditor );
#endif
    
  public:
    wxChoiceAndMinusButtonEditor() {}
    virtual ~wxChoiceAndMinusButtonEditor() {}
#if wxMAJOR_VERSION>=3
    virtual wxString GetName() const { return wxT( "ChoiceAndMinusButtonEditor" ); }
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
wxIMPLEMENT_DYNAMIC_CLASS( wxChoiceAndMinusButtonEditor, wxPGChoiceEditor );
#else
WX_PG_IMPLEMENT_EDITOR_CLASS( ChoiceAndMinusButtonEditor,
                              wxChoiceAndMinusButtonEditor,
                              wxPGChoiceEditor )
#endif

wxPGWindowList wxChoiceAndMinusButtonEditor::CreateControls( wxPropertyGrid* propGrid,
                                                             wxPGProperty* property,
                                                             const wxPoint& pos,
                                                             const wxSize& sz ) const
{
  wxPGMultiButton* buttons = new wxPGMultiButton( propGrid, sz );

  buttons->Add( wxT( "-" ) );

  wxPGWindowList wndList = wxPGChoiceEditor::CreateControls( propGrid, property, pos,
                                                               buttons->GetPrimarySize() );
#if wxMAJOR_VERSION>=3
  buttons->Finalize( propGrid, pos );
#else
  buttons->FinalizePosition( pos );
#endif
  wndList.SetSecondary( buttons );

  return wndList;
}

bool wxChoiceAndMinusButtonEditor::OnEvent( wxPropertyGrid* propGrid,
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
  }
  
  return wxPGChoiceEditor::OnEvent( propGrid, property, ctrl, event );
}


class wxChoiceAndPlusButtonEditor : public wxPGChoiceEditor
{
#if wxMAJOR_VERSION>=3
  wxDECLARE_DYNAMIC_CLASS( wxChoiceAndPlusButtonEditor );
#else
  WX_PG_DECLARE_EDITOR_CLASS( wxChoiceAndPlusButtonEditor );
#endif
    
  public:
    wxChoiceAndPlusButtonEditor() {}
    virtual ~wxChoiceAndPlusButtonEditor() {}
#if wxMAJOR_VERSION>=3
    virtual wxString GetName() const { return wxT( "ChoiceAndPlusButtonEditor" ); }
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
wxIMPLEMENT_DYNAMIC_CLASS( wxChoiceAndPlusButtonEditor, wxPGChoiceEditor );
#else
WX_PG_IMPLEMENT_EDITOR_CLASS( ChoiceAndPlusButtonEditor,
                              wxChoiceAndPlusButtonEditor,
                              wxPGChoiceEditor )
#endif

wxPGWindowList wxChoiceAndPlusButtonEditor::CreateControls( wxPropertyGrid* propGrid,
                                                            wxPGProperty* property,
                                                            const wxPoint& pos,
                                                            const wxSize& sz ) const
{
  wxPGMultiButton* buttons = new wxPGMultiButton( propGrid, sz );

  buttons->Add( wxT( "+" ) );

  wxPGWindowList wndList = wxPGChoiceEditor::CreateControls( propGrid, property, pos,
                                                             buttons->GetPrimarySize() );
#if wxMAJOR_VERSION>=3
  buttons->Finalize( propGrid, pos );
#else
  buttons->FinalizePosition( pos );
#endif
  wndList.SetSecondary( buttons );

  return wndList;
}

bool wxChoiceAndPlusButtonEditor::OnEvent( wxPropertyGrid* propGrid,
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
  }
  
  return wxPGChoiceEditor::OnEvent(propGrid, property, ctrl, event);
}


class wxChoiceAndBothButtonsEditor : public wxPGChoiceEditor
{
#if wxMAJOR_VERSION>=3
  wxDECLARE_DYNAMIC_CLASS( wxChoiceAndBothButtonsEditor );
#else
  WX_PG_DECLARE_EDITOR_CLASS( wxChoiceAndBothButtonsEditor );
#endif
    
  public:
    wxChoiceAndBothButtonsEditor() {}
    virtual ~wxChoiceAndBothButtonsEditor() {}
#if wxMAJOR_VERSION>=3
    virtual wxString GetName() const { return wxT( "ChoiceAndBothButtonsEditor" ); }
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
wxIMPLEMENT_DYNAMIC_CLASS( wxChoiceAndBothButtonsEditor, wxPGChoiceEditor );
#else
WX_PG_IMPLEMENT_EDITOR_CLASS( ChoiceAndBothButtonsEditor,
                              wxChoiceAndBothButtonsEditor,
                              wxPGChoiceEditor )
#endif

wxPGWindowList wxChoiceAndBothButtonsEditor::CreateControls( wxPropertyGrid* propGrid,
                                                             wxPGProperty* property,
                                                             const wxPoint& pos,
                                                             const wxSize& sz ) const
{
  wxPGMultiButton* buttons = new wxPGMultiButton( propGrid, sz );


#if wxMAJOR_VERSION>=3
  buttons->Add( wxArtProvider::GetBitmap( wxART_PLUS ) );
  buttons->Add( wxArtProvider::GetBitmap( wxART_MINUS ) );
#else
  buttons->Add( wxT( "+" ) );
  buttons->Add( wxT( "-" ) );
#endif

  wxPGWindowList wndList = wxPGChoiceEditor::CreateControls( propGrid, property, pos,
                                                             buttons->GetPrimarySize() );
#if wxMAJOR_VERSION>=3
  buttons->Finalize( propGrid, pos );
#else
  buttons->FinalizePosition( pos );
#endif
  wndList.SetSecondary( buttons );

  return wndList;
}


bool wxChoiceAndBothButtonsEditor::OnEvent( wxPropertyGrid* propGrid,
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
    Window *tmpTimeline = ( ( PropertyClientData * )property->GetClientData() )->ownerTimeline;
    wxPGMultiButton *buttons = ( wxPGMultiButton* ) propGrid->GetEditorControlSecondary();
    if ( event.GetId() == buttons->GetButtonId( 0 ) )
    {
      // + button
      tmpTimeline->addExtraCompose( TOPCOMPOSE1 );
    }
    if ( event.GetId() == buttons->GetButtonId( 1 ) )
    {
      // - button
      tmpTimeline->removeExtraCompose( TOPCOMPOSE1 );
    }
    
    paraverMain::myParaverMain->spreadSetChanged( tmpTimeline );
    paraverMain::myParaverMain->spreadSetRedraw( tmpTimeline );
  }
  
  return wxPGChoiceEditor::OnEvent( propGrid, property, ctrl, event );
}


#if wxMAJOR_VERSION>=3
//static wxSpinButtonsEditor *spinButtonsEditor = NULL;
static wxChoiceAndMinusButtonEditor *choiceAndMinusButtonEditor = NULL;
static wxChoiceAndPlusButtonEditor  *choiceAndPlusButtonEditor  = NULL;
static wxChoiceAndBothButtonsEditor *choiceAndBothButtonsEditor  = NULL;
#endif


void initPG()
{
#if wxMAJOR_VERSION>=3
/*
  spinButtonsEditor = new wxSpinButtonsEditor();
  wxPropertyGrid::RegisterEditorClass( spinButtonsEditor );
*/
  choiceAndMinusButtonEditor = new wxChoiceAndMinusButtonEditor();
  choiceAndPlusButtonEditor = new wxChoiceAndPlusButtonEditor();
  choiceAndBothButtonsEditor = new wxChoiceAndBothButtonsEditor();
  wxPropertyGrid::RegisterEditorClass( choiceAndMinusButtonEditor );
  wxPropertyGrid::RegisterEditorClass( choiceAndPlusButtonEditor );
  wxPropertyGrid::RegisterEditorClass( choiceAndBothButtonsEditor );
#else
//  wxPGRegisterEditorClass( SpinButtonsEditor );
  wxPGRegisterEditorClass( ChoiceAndMinusButtonEditor );
  wxPGRegisterEditorClass( ChoiceAndPlusButtonEditor );
  wxPGRegisterEditorClass( ChoiceAndBothButtonsEditor );
#endif
}


void fillPropertyClientData( Window* whichWindow,
                             Histogram* whichHistogram,
                             wxPGProperty* whichProperty,
                             wxString widgetName,
                             std::vector< PropertyClientData * >& whichPropertiesClientData )
{
  if ( whichProperty != NULL )
  {
    PropertyClientData *tmpClientData = new PropertyClientData();
    tmpClientData->ownerTimeline = whichWindow;
    tmpClientData->ownerHistogram = whichHistogram;
    tmpClientData->propName = widgetName;
    whichPropertiesClientData.push_back( tmpClientData );
    whichProperty->SetClientData( tmpClientData );
  }
}


wxPGId AppendCFG4DBoolPropertyWindow( wxPropertyGrid* windowProperties,
                                      Window* whichWindow,
                                      std::vector< PropertyClientData * >& whichPropertiesClientData,
                                      wxPGId fatherWidget,
                                      const wxString &widgetLabel,
                                      const wxString &widgetName,
                                      TSingleTimelineProperties propertyIndex,
                                      const bool &propertyValue )
{
  wxPGId retId = (wxPGId)NULL;
  wxBoolProperty *auxProperty = NULL;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;
  
  if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
  {
    // NORMAL mode
    auxProperty = new wxBoolProperty( widgetLabel, tmpWidgetName, propertyValue );

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

    auxProperty = new wxBoolProperty( auxTag, tmpWidgetName, propertyValue );

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

    auxProperty = new wxBoolProperty( auxTag, tmpWidgetName, propertyValue );

    retId = windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property
  }

  fillPropertyClientData( whichWindow, NULL, auxProperty, widgetName, whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DStringPropertyWindow( wxPropertyGrid* windowProperties,
                                        Window* whichWindow,
                                        std::vector< PropertyClientData * >& whichPropertiesClientData,
                                        wxPGId fatherWidget,
                                        const wxString &widgetLabel,
                                        const wxString &widgetName,
                                        TSingleTimelineProperties propertyIndex,
                                        const string &propertyValue )
{
  wxPGId retId = (wxPGId)NULL;
  wxStringProperty *auxProperty = NULL;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;
  
  if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
  {
    // NORMAL mode
    auxProperty = new wxStringProperty(
            widgetLabel, tmpWidgetName, wxString::FromAscii( propertyValue.c_str() ) );

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

    auxProperty = new wxStringProperty( 
            auxTag, tmpWidgetName, wxString::FromAscii( propertyValue.c_str() ) );

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

    auxProperty = new wxStringProperty( 
            auxTag, tmpWidgetName, wxString::FromAscii( propertyValue.c_str() ) );

    retId = windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property
  }

  fillPropertyClientData( whichWindow, NULL, auxProperty, widgetName, whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DEnumPropertyWindow( wxPropertyGrid* windowProperties,
                                      Window* whichWindow,
                                      std::vector< PropertyClientData * >& whichPropertiesClientData,
                                      wxPGId fatherWidget,
                                      const wxString &widgetLabel,
                                      const wxString &widgetPrefix,
                                      const wxString &widgetName,
                                      TSingleTimelineProperties propertyIndex,
                                      const wxArrayString &arrayStr,
                                      const wxArrayInt &arrayInt,
                                      const int selected,
                                      const ButtonType addButton = NO_BUTTON )
{
  wxPGId retId = (wxPGId)NULL;
  wxEnumProperty *auxProperty = NULL;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;

  if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
  {
    // NORMAL mode
    auxProperty = new wxEnumProperty( widgetLabel, tmpWidgetName, arrayStr, arrayInt, selected );

    if ( fatherWidget )
    {
      retId = windowProperties->AppendIn( fatherWidget, auxProperty );
    }
    else
    {
      retId = windowProperties->Append( auxProperty );
    }

    switch( addButton )
    {
      case PLUS_BUTTON:
        #if wxMAJOR_VERSION>=3
          windowProperties->SetPropertyEditor( auxProperty, choiceAndPlusButtonEditor );
        #else
          windowProperties->SetPropertyEditor( auxProperty, wxPG_EDITOR( ChoiceAndPlusButtonEditor ));
        #endif
        break;

      case MINUS_BUTTON:
        #if wxMAJOR_VERSION>=3
          windowProperties->SetPropertyEditor( auxProperty, choiceAndMinusButtonEditor );
        #else
          windowProperties->SetPropertyEditor( auxProperty, wxPG_EDITOR( ChoiceAndMinusButtonEditor ));
        #endif

        break;
      
      case BOTH_BUTTONS:
        #if wxMAJOR_VERSION>=3
          windowProperties->SetPropertyEditor( auxProperty, choiceAndBothButtonsEditor );
        #else
          windowProperties->SetPropertyEditor( auxProperty, wxPG_EDITOR( ChoiceAndBothButtonsEditor ));
        #endif
        break;

      default:
        break;
    }
  }
  else if ( !whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ) )
  {
    // CFG4D mode (single timeline)
    wxString auxTag = wxString::FromAscii( 
            whichWindow->getCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ).c_str() );
    auxProperty = new wxEnumProperty( auxTag, tmpWidgetName, arrayStr, arrayInt, selected );
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
    auxProperty = new wxEnumProperty( auxTag, tmpWidgetName, arrayStr, arrayInt, selected );

    retId = windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property
  }

  fillPropertyClientData( whichWindow, NULL, auxProperty, widgetPrefix + widgetName, whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DFloatPropertyWindow( wxPropertyGrid* windowProperties,
                                       Window* whichWindow,
                                       std::vector< PropertyClientData * >& whichPropertiesClientData,
                                       wxPGId fatherWidget,
                                       const wxString &widgetLabel,
                                       const wxString &widgetName,
                                       TSingleTimelineProperties propertyIndex,
                                       const double propertyValue )
{
  wxPGId retId = (wxPGId)NULL;
  wxFloatProperty *auxProperty = NULL;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;

  if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
  {
    // NORMAL mode
    auxProperty = new wxFloatProperty( widgetLabel, tmpWidgetName, propertyValue );

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
    auxProperty = new wxFloatProperty( auxTag, tmpWidgetName, propertyValue );

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
    auxProperty = new wxFloatProperty( auxTag, tmpWidgetName, propertyValue );

    retId = windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property
  }
  
// NOT WORKING PROPERLY
/*
#if wxMAJOR_VERSION>=3
  windowProperties->SetPropertyEditor( auxProperty, spinButtonsEditor );
#else
  windowProperties->SetPropertyEditor( auxProperty, wxPG_EDITOR( SpinButtonsEditor ) );
#endif
*/

  fillPropertyClientData( whichWindow, NULL, auxProperty, widgetName, whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DprvRowsSelectionPropertyWindow( wxPropertyGrid* windowProperties,
                                                  Window* whichWindow,
                                                  std::vector< PropertyClientData * >& whichPropertiesClientData,
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

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;

  if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
  {
    // NORMAL mode
    auxProperty = new prvRowsSelectionProperty( windowProperties, whichWindow,
                                                widgetTitle, objects, widgetLabel, tmpWidgetName );

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
    auxProperty = new prvRowsSelectionProperty( windowProperties, whichWindow,
                                                widgetTitle, objects, auxTag, tmpWidgetName );

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
    auxProperty = new prvRowsSelectionProperty( windowProperties, whichWindow,
                                                widgetTitle, objects, auxTag, tmpWidgetName );

    retId = windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property and NULL returned.
  }

  fillPropertyClientData( whichWindow, NULL, auxProperty, widgetPrefix + widgetName, whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DprvNumbersListPropertyWindow( wxPropertyGrid* windowProperties,
                                                Window* whichWindow,
                                                std::vector< PropertyClientData * >& whichPropertiesClientData,
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

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;

  if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
  {
    // NORMAL mode
    auxProperty = new prvNumbersListProperty( widgetLabel, tmpWidgetName, arrayStr );

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
    auxProperty = new prvNumbersListProperty( auxTag, tmpWidgetName, arrayStr );

    retId = windowProperties->Append( auxProperty );
  }
  else if ( whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ) )
  {
    // CFG4D mode (derived timeline)
    auxTag = wxString::FromAscii( whichWindow->getCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ).c_str() );
    auxProperty = new prvNumbersListProperty( auxTag, tmpWidgetName, arrayStr );

    retId = windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property and NULL returned.
  }

  fillPropertyClientData( whichWindow, NULL, auxProperty, widgetPrefix + widgetName, whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DprvEventInfoPropertyWindow( wxPropertyGrid* windowProperties,
                                              Window* whichWindow,
                                              std::vector< PropertyClientData * >& whichPropertiesClientData,
                                              wxPGId fatherWidget,
                                              const wxString &widgetLabel,
                                              const wxString &widgetPrefix,
                                              const wxString &widgetName,
                                              TSingleTimelineProperties propertyIndex,
                                              const wxPGChoices &choices )
{
  wxPGId retId = (wxPGId)NULL;
  prvEventInfoProperty *auxProperty = NULL;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;
  
  prvEventInfoProperty::InfoType infoType;
  if ( widgetName.Cmp( _("Types") ) == 0 )
    infoType = prvEventInfoProperty::TYPES;
  else if ( widgetName.Cmp( _("Values") ) == 0 )
    infoType = prvEventInfoProperty::VALUES;

  if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
  {
    // NORMAL mode
    auxProperty = new prvEventInfoProperty( widgetLabel, tmpWidgetName, choices, whichWindow, infoType );

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
    auxProperty = new prvEventInfoProperty( auxTag, tmpWidgetName, choices, whichWindow, infoType );

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
    auxProperty = new prvEventInfoProperty( auxTag, tmpWidgetName, choices, whichWindow, infoType );

    retId = windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property and NULL returned.
  }

  fillPropertyClientData( whichWindow, NULL, auxProperty, widgetPrefix + widgetName, whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DprvSemanticThreadPropertyWindow( wxPropertyGrid* windowProperties,
                                                   Window* whichWindow,
                                                   std::vector< PropertyClientData * >& whichPropertiesClientData,
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

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;

  if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
  {
    // NORMAL mode
    auxProperty = new prvSemanticThreadProperty( widgetLabel, tmpWidgetName, 
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
    auxProperty = new prvSemanticThreadProperty( auxTag, tmpWidgetName, 
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
    auxProperty = new prvSemanticThreadProperty( auxTag, tmpWidgetName,
                                                 levels, threadFunctions, defaultValue );

    retId = windowProperties->Append( auxProperty );

  }
  else
  {
    // CFG4D mode but no tag found => don't show property and NULL returned.
  }

  fillPropertyClientData( whichWindow, NULL, auxProperty, widgetPrefix + widgetName, whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DStringPropertyHistogram(  wxPropertyGrid* windowProperties,
                                            Histogram* whichHisto,
                                            std::vector< PropertyClientData * >& whichPropertiesClientData,
                                            wxPGId fatherWidget,
                                            const wxString &widgetLabel,
                                            const wxString &widgetName,
                                            THistogramProperties propertyIndex,
                                            const string &propertyValue )
{
  wxPGId retId = (wxPGId)NULL;
  wxStringProperty *auxProperty = NULL;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;

  if ( !whichHisto->getCFG4DEnabled() || !whichHisto->getCFG4DMode() )
  {
    // NORMAL mode
    auxProperty = new wxStringProperty(
            widgetLabel, tmpWidgetName, wxString::FromAscii( propertyValue.c_str() ) );

    if ( fatherWidget )
    {
      retId = windowProperties->AppendIn( fatherWidget, auxProperty );
    }
    else
    {
      retId = windowProperties->Append( auxProperty );
    }
  }
  else if ( whichHisto->existsCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ) )
  {
    // CFG4D mode
    wxString auxTag = wxString::FromAscii( 
            whichHisto->getCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ).c_str() );

    auxProperty = new wxStringProperty( 
            auxTag, tmpWidgetName, wxString::FromAscii( propertyValue.c_str() ) );

    retId = windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property
  }

  fillPropertyClientData( NULL, whichHisto, auxProperty, widgetName, whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DEnumPropertyHistogram(  wxPropertyGrid* windowProperties,
                                          Histogram* whichHisto,
                                          std::vector< PropertyClientData * >& whichPropertiesClientData,
                                          wxPGId fatherWidget,
                                          const wxString &widgetLabel,
                                          const wxString &widgetName,
                                          THistogramProperties propertyIndex,
                                          const wxArrayString &arrayStr,
                                          const wxArrayInt &arrayInt,
                                          const int selected )
{
  wxPGId retId = (wxPGId)NULL;
  wxEnumProperty *auxProperty = NULL;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;

  if ( !whichHisto->getCFG4DEnabled() || !whichHisto->getCFG4DMode() )
  {
    // NORMAL mode
    auxProperty = new wxEnumProperty( widgetLabel, tmpWidgetName, arrayStr, arrayInt, selected );

    if ( fatherWidget )
    {
      retId = windowProperties->AppendIn( fatherWidget, auxProperty );
    }
    else
    {
      retId = windowProperties->Append( auxProperty );
    }
  }
  else if ( whichHisto->existsCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ) )
  {
    // CFG4D mode
    wxString auxTag = wxString::FromAscii( 
            whichHisto->getCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ).c_str() );
    auxProperty = new wxEnumProperty( auxTag, tmpWidgetName, arrayStr, arrayInt, selected );

    retId = windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property
  }

  fillPropertyClientData( NULL, whichHisto, auxProperty, widgetName, whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DTimelineTreePropertyHistogram( wxPropertyGrid* windowProperties,
                                                 Histogram* whichHisto,
                                                 std::vector< PropertyClientData * >& whichPropertiesClientData,
                                                 wxPGId fatherWidget,
                                                 const wxString &widgetLabel,
                                                 const wxString &widgetName,
                                                 THistogramProperties propertyIndex,
                                                 vector<TWindowID> windowsList,
                                                 Window *currentWindow,
                                                 bool needNoneElement = false )
{
  wxPGId retId = (wxPGId)NULL;
  prvTimelineTreeProperty *auxProperty = NULL;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;

  wxString valueStr;
  if( currentWindow == NULL )
    valueStr = wxT( "None" );
  else
    valueStr = wxString( currentWindow->getName().c_str(), wxConvUTF8 );

  Trace *currentTrace;
  if( currentWindow == NULL )
    currentTrace = whichHisto->getControlWindow()->getTrace();
  else
    currentTrace = currentWindow->getTrace();

  if ( !whichHisto->getCFG4DEnabled() || !whichHisto->getCFG4DMode() )
  {
    // NORMAL mode
    auxProperty = new prvTimelineTreeProperty( widgetLabel,
                                               tmpWidgetName,
                                               valueStr,
                                               windowsList,
                                               currentWindow,
                                               currentTrace,
                                               needNoneElement );

    if ( fatherWidget )
    {
      retId = windowProperties->AppendIn( fatherWidget, auxProperty );
    }
    else
    {
      retId = windowProperties->Append( auxProperty );
    }
  }
  else if ( whichHisto->existsCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ) )
  {
    // CFG4D mode
    wxString auxTag = wxString::FromAscii( 
            whichHisto->getCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ).c_str() );
    auxProperty = new prvTimelineTreeProperty( auxTag,
                                               tmpWidgetName,
                                               valueStr,
                                               windowsList,
                                               currentWindow,
                                               currentTrace,
                                               needNoneElement );

    retId = windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property
  }

  fillPropertyClientData( NULL, whichHisto, auxProperty, widgetName, whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DFloatPropertyHistogram(  wxPropertyGrid* windowProperties,
                                           Histogram* whichHisto,
                                           std::vector< PropertyClientData * >& whichPropertiesClientData,
                                           wxPGId fatherWidget,
                                           const wxString &widgetLabel,
                                           const wxString &widgetName,
                                           THistogramProperties propertyIndex,
                                           const double propertyValue )
{
  wxPGId retId = (wxPGId)NULL;
  wxFloatProperty *auxProperty = NULL;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;

  if ( !whichHisto->getCFG4DEnabled() || !whichHisto->getCFG4DMode() )
  {
    // NORMAL mode
    auxProperty = new wxFloatProperty( widgetLabel, tmpWidgetName, propertyValue );

    if ( fatherWidget )
    {
      retId = windowProperties->AppendIn( fatherWidget, auxProperty );
    }
    else
    {
      retId = windowProperties->Append( auxProperty );
    }
  }
  else if ( whichHisto->existsCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ) )
  {
    // CFG4D mode
    wxString auxTag = wxString::FromAscii( 
            whichHisto->getCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ).c_str() );
    auxProperty = new wxFloatProperty( auxTag, tmpWidgetName, propertyValue );

    retId = windowProperties->Append( auxProperty );
  }
  else
  {
    // CFG4D mode but no tag found => don't show property
  }

  fillPropertyClientData( NULL, whichHisto, auxProperty, widgetName, whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DParamPrvNumbersListPropertyWindow( wxPropertyGrid* windowProperties,
                                                     Window* whichWindow,
                                                     std::vector< PropertyClientData * >& whichPropertiesClientData,
                                                     wxPGId fatherWidget,
                                                     TWindowLevel currentSemanticLevel,
                                                     PRV_UINT32 currentNumParameter,
                                                     const wxString &widgetLabel,
                                                     const wxString &widgetPrefix,
                                                     const wxString &widgetName,
                                                     const wxArrayString &arrayStr,
                                                     bool isExtraCompose = false,
                                                     size_t extraComposePos = 0 )
{
  wxPGId retId = (wxPGId)NULL;
  prvNumbersListProperty *auxProperty = NULL;
  wxString auxTag;
  
  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;

  // Splitted Parameter alias key
  string kSemanticLevel;
  string kFunction;
  string currentFunction;
  PRV_UINT32 kNumParameter;

  if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
  {
    // NORMAL mode
    auxProperty = new prvNumbersListProperty( widgetLabel, tmpWidgetName, arrayStr );

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
    // TODO: CFG4D extra compose parameters
    if( isExtraCompose ) return retId;
    
    string currentSemanticLevelStr = TimelineLevelLabels[ currentSemanticLevel ];
    vector< Window::TParamAliasKey > paramAliasKey =
            whichWindow->getCFG4DParamKeysBySemanticLevel( currentSemanticLevelStr );
    for( vector< Window::TParamAliasKey >::const_iterator it = paramAliasKey.begin(); 
         it != paramAliasKey.end(); ++it )
    {
      // CFG4D mode
      whichWindow->splitCFG4DParamAliasKey( *it, kSemanticLevel, kFunction, kNumParameter );
      string currentFunction;
      if( isExtraCompose )
        currentFunction = whichWindow->getExtraLevelFunction( currentSemanticLevel, extraComposePos );
      else
        currentFunction = whichWindow->getLevelFunction( currentSemanticLevel );

      if ( kFunction      == currentFunction &&
           kNumParameter  == currentNumParameter )
      {
        auxTag = wxString::FromAscii( whichWindow->getCFG4DParamAlias( *it ).c_str() );
        auxProperty = new prvNumbersListProperty( auxTag, tmpWidgetName, arrayStr );

        retId = windowProperties->Append( auxProperty );
      }
      else
      {
        // CFG4D mode but no tag found => don't show property and NULL returned.
      }
    }
  }

  fillPropertyClientData( whichWindow, NULL, auxProperty, widgetPrefix + widgetName, whichPropertiesClientData );

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


void semanticExtraComposeFunctionParameter( wxPropertyGrid* windowProperties,
                                            Window *whichWindow,
                                            std::vector< PropertyClientData * >& whichPropertiesClientData,
                                            wxPGId category,
                                            TWindowLevel functionLevel,
                                            size_t whichPos )
{
  for( TParamIndex paramIdx = 0; paramIdx < whichWindow->getExtraFunctionNumParam( functionLevel, whichPos - 1 ); ++paramIdx )
  {
    wxArrayString valuesStr;
    wxString propName( _( "Extra Param" ) );
    propName << _( " " ) << paramIdx << _( " " ) << functionLevel << _( " " )
             << whichWindow->getExtraNumPositions( functionLevel ) - whichPos + 1;
    TParamValue values = whichWindow->getExtraFunctionParam( functionLevel, whichPos - 1, paramIdx );
    for( TParamValue::iterator it = values.begin(); it != values.end(); ++it )
      valuesStr.Add( wxString() << (*it) );

    wxString propLabel = wxString( _("   ") ) +
            wxString::FromAscii( whichWindow->getExtraFunctionParamName( functionLevel, whichPos - 1, paramIdx ).c_str() );
    wxPGId semanticFunctionParameterValue = (wxPGId)NULL;
    semanticFunctionParameterValue = AppendCFG4DParamPrvNumbersListPropertyWindow(
            windowProperties, whichWindow, whichPropertiesClientData, category,
            functionLevel, (PRV_UINT32)paramIdx,
            propLabel, wxT(""), propName,
            valuesStr, true, whichPos );
  }
}

void semanticFunctionParameter( wxPropertyGrid* windowProperties,
                                Window *whichWindow,
                                std::vector< PropertyClientData * >& whichPropertiesClientData,
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
            windowProperties, whichWindow, whichPropertiesClientData, category,
            functionLevel, (PRV_UINT32)paramIdx,
            propLabel, wxT(""), propName,
            valuesStr );
  }
}


void closeOpenedPropertyDialog()
{
  wxDialog *tmpDialog = wxparaverApp::mainWindow->GetOpenedPropertyDialog();
  if( tmpDialog != NULL )
    tmpDialog->EndModal( wxID_CANCEL );
}

void updateTimelinePropertiesRecursive( wxPropertyGrid* windowProperties, Window *whichWindow,
                                        std::vector< PropertyClientData * >& whichPropertiesClientData )
{
  PRV_UINT32 precision = ParaverConfig::getInstance()->getTimelinePrecision();

  whichWindow->setChanged( false );

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

  wxPGId dummyPGId = (wxPGId)NULL; // used to append always to windowProperties

  AppendCFG4DStringPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, dummyPGId,
          wxT("Name"), wxT("Name"), SINGLE_NAME, whichWindow->getName() );

  AppendCFG4DStringPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, dummyPGId,
          wxT("Begin time"), wxT("Begin time"), SINGLE_BEGINTIME,
          LabelConstructor::timeLabel(
                  whichWindow->traceUnitsToWindowUnits( whichWindow->getWindowBeginTime() ),
                  whichWindow->getTimeUnit(),
                  precision ));

  AppendCFG4DStringPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, dummyPGId,
          wxT("End time"), wxT("End time"), SINGLE_ENDTIME,
          LabelConstructor::timeLabel(
                  whichWindow->traceUnitsToWindowUnits( whichWindow->getWindowEndTime() ),
                  whichWindow->getTimeUnit(),
                  precision ));

  AppendCFG4DFloatPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, dummyPGId,
          wxT("Semantic Minimum"), wxT("Semantic Minimum"), SINGLE_SEMANTICMINIMUM, whichWindow->getMinimumY() );

  AppendCFG4DFloatPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, dummyPGId,
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, dummyPGId,
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, dummyPGId,
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

  AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, dummyPGId,
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
    if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
    {
      filterCat = windowProperties->Append( new wxPropertyCategory( wxT("Filter"), wxT("Filter") ) );
      if( filterCatCollapsed )
        filterCat->SetFlagsFromString( _( "COLLAPSED" ) );
    }
    
    // ---------------------------- COMMUNICATION FILTER ---------------------------
    wxPGId commFilterCat = (wxPGId)NULL;
    if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
    {
      commFilterCat = windowProperties->AppendIn( filterCat, new wxPropertyCategory( wxT("Communications"), wxT("Communications") ) );
      if( commFilterCatCollapsed )
        commFilterCat->SetFlagsFromString( _( "COLLAPSED" ) );
    }

    AppendCFG4DBoolPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, commFilterCat,
            wxT("Logical"), wxT("Logical"), SINGLE_COMMLOGICAL, filter->getLogical() );

    AppendCFG4DBoolPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, commFilterCat,
            wxT("Physical"), wxT("Physical"), SINGLE_COMMPHYSICAL, filter->getPhysical() );

    // Comm From
    wxPGId commFilterFrom = ( wxPGId )NULL;
    if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, commFilterFrom,
            wxT( "Function" ), wxT("Comm from."), wxT("FromFunction"), SINGLE_COMMFROMFUNCTION,
            arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    // add communication senders to arrayStr
    arrayStr.Clear();
    vector<TObjectOrder> fromSel;
    filter->getCommFrom( fromSel );
    wxPGId commFilterFromValues = (wxPGId)NULL;

    commFilterFromValues = AppendCFG4DprvRowsSelectionPropertyWindow(
            windowProperties, whichWindow, whichPropertiesClientData, commFilterFrom,
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, commFilterCat,
             wxT("From/To Op"), wxT(""), wxT("FromToOp"), SINGLE_COMMFROMTOOP,
             arrayStr, arrayInt, selected );

    // Comm To
    wxPGId commFilterTo = (wxPGId) NULL; 
    if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, commFilterTo,
            wxT( "Function" ), wxT("Comm to."), wxT("ToFunction"), SINGLE_COMMTOFUNCTION,
            arrayFilterFunctions, arrayFilterFunctionsPos, selected );


    arrayStr.Clear();
    vector<TObjectOrder> toSel;
    filter->getCommTo( toSel );

    wxPGId commFilterToValues = (wxPGId)NULL;
    commFilterToValues = AppendCFG4DprvRowsSelectionPropertyWindow(
            windowProperties, whichWindow, whichPropertiesClientData, commFilterTo,
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
    if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, commFilterTag,
            wxT( "Function" ), wxT("Comm tag."), wxT("TagFunction"), SINGLE_COMMTAGFUNCTION,
            arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    arrayStr.Clear();
    vector<TCommTag> tagSel;
    filter->getCommTag( tagSel );
    for( vector<TCommTag>::iterator it = tagSel.begin(); it != tagSel.end(); it++ )
      arrayStr.Add( wxString() << (*it) );

    wxPGId commFilterTagValues = (wxPGId)NULL;
    commFilterTagValues = AppendCFG4DprvNumbersListPropertyWindow(
            windowProperties, whichWindow, whichPropertiesClientData, commFilterTag,
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, commFilterCat,
            wxT("Tag/Size Op"), wxT(""), wxT("TagSizeOp"), SINGLE_COMMTAGSIZEOP,
            arrayStr, arrayInt, selected );

    // Comm Size
    wxPGId commFilterSize = (wxPGId)NULL; 
    if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, commFilterSize,
            wxT("Function"), wxT("Comm size."), wxT("SizeFunction"), SINGLE_COMMSIZEFUNCTION,
            arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    arrayStr.Clear();
    vector<TCommSize> sizeSel;
    filter->getCommSize( sizeSel );
    for( vector<TCommSize>::iterator it = sizeSel.begin(); it != sizeSel.end(); it++ )
      arrayStr.Add( wxString() << (*it) );

    wxPGId commFilterSizeValues = (wxPGId)NULL;
    commFilterSizeValues = AppendCFG4DprvNumbersListPropertyWindow(
            windowProperties, whichWindow, whichPropertiesClientData, commFilterSize,
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
    if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, commFilterBW,
            wxT("Function"), wxT("Comm bandwidth."), wxT("BWFunction"), SINGLE_COMMBANDWIDTHFUNCTION,
            arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    arrayStr.Clear();
    vector<TSemanticValue> bwSel;
    filter->getBandWidth( bwSel );
    for( vector<TSemanticValue>::iterator it = bwSel.begin(); it != bwSel.end(); it++ )
      arrayStr.Add( wxString() << (*it) );

    wxPGId commFilterBandWidthValues = (wxPGId)NULL;
    commFilterBandWidthValues = AppendCFG4DprvNumbersListPropertyWindow(
            windowProperties, whichWindow, whichPropertiesClientData, commFilterBW,
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
    if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
    {
      eventFilterCat = windowProperties->AppendIn( filterCat, new wxPropertyCategory( wxT("Events"), wxT("Events") ) );
      if( eventFilterCatCollapsed )
        eventFilterCat->SetFlagsFromString( _( "COLLAPSED" ) );
    }

    // Event Type
    wxPGId eventFilterType = (wxPGId)NULL;
    if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, eventFilterType,
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
            windowProperties, whichWindow, whichPropertiesClientData, eventFilterType,
            wxT("Types"), wxT("Event type."), wxT("Types"), SINGLE_EVENTTYPEVALUES,
            typeChoices );

    if ( eventFilterTypeValues != (wxPGId)NULL )
    {
      windowProperties->SetPropertyAttribute( eventFilterTypeValues, wxPG_ATTR_MULTICHOICE_USERSTRINGMODE, 1 );

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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, eventFilterCat,
            wxT("Type/Value Op"), wxT(""), wxT("TypeValueOp"), SINGLE_EVENTTYPEVALUESOP,
            arrayStr, arrayInt, selected );

    // Event Value
    wxPGId eventFilterValue = (wxPGId)NULL;
    if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, eventFilterValue,
            wxT("Function"),  wxT("Event value."), wxT("ValueFunction"), SINGLE_EVENTVALUEFUNCTION,
            arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    wxPGId eventFilterValueValues = (wxPGId)NULL;
    eventFilterValueValues = AppendCFG4DprvEventInfoPropertyWindow(
            windowProperties, whichWindow, whichPropertiesClientData, eventFilterValue,
            wxT("Values"), wxT("Event value."), wxT("Values"), SINGLE_EVENTVALUEVALUES,
            typeChoices );

    if ( eventFilterValueValues != (wxPGId) NULL )
    {
      windowProperties->SetPropertyAttribute( eventFilterValueValues, wxPG_ATTR_MULTICHOICE_USERSTRINGMODE, 1 );

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
  if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
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

  ButtonType tmpButtonType = NO_BUTTON;
  // Extra composes
  for( size_t nExtraCompose = whichWindow->getExtraNumPositions( TOPCOMPOSE1 ); nExtraCompose > 0 ; --nExtraCompose )
  {
    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = composeFunctions.begin();
         it != composeFunctions.end(); ++it )
    {
      if( (*it) == whichWindow->getExtraLevelFunction( TOPCOMPOSE1, nExtraCompose - 1 ) )
        selected = pos;
      pos++;
    }
    
    // Name "Extra Top Compose n"
    wxString tmpNum;
    tmpNum << whichWindow->getExtraNumPositions( TOPCOMPOSE1 ) - nExtraCompose + 1;
    wxString tmpName = wxT("Extra Top Compose ") + tmpNum;
    
    if( nExtraCompose == whichWindow->getExtraNumPositions( TOPCOMPOSE1 ) )
      tmpButtonType = BOTH_BUTTONS;
    else
      tmpButtonType = NO_BUTTON;
    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, semanticCat,
              tmpName, wxT(""), tmpName, whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_EXTRATOPCOMPOSE1 : SINGLE_EXTRATOPCOMPOSE1,
              arrayComposeFunctions, arrayComposeFunctionsPos, selected, tmpButtonType );

    semanticExtraComposeFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData,
                                           semanticCat, TOPCOMPOSE1, nExtraCompose );
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

  if( whichWindow->getExtraNumPositions( TOPCOMPOSE1 ) == 0 )
    tmpButtonType = BOTH_BUTTONS;
  else
    tmpButtonType = NO_BUTTON;
  AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, semanticCat,
            wxT("Top Compose 1"), wxT(""), wxT("Top Compose 1"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_TOPCOMPOSE1 : SINGLE_TOPCOMPOSE1,
            arrayComposeFunctions, arrayComposeFunctionsPos, selected, tmpButtonType );

  semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, semanticCat, TOPCOMPOSE1 );

  pos = 0;
  selected = -1;
  for( vector<string>::iterator it = composeFunctions.begin();
       it != composeFunctions.end(); ++it )
  {
    if( (*it) == whichWindow->getLevelFunction( TOPCOMPOSE2 ) )
      selected = pos;
    ++pos;
  }

  AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, semanticCat,
            wxT("Top Compose 2"), wxT(""), wxT("Top Compose 2"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_TOPCOMPOSE2 : SINGLE_TOPCOMPOSE2,
            arrayComposeFunctions, arrayComposeFunctionsPos, selected );

  semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, semanticCat, TOPCOMPOSE2 );

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

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, semanticCat,
                wxT("Compose Workload"), wxT(""), wxT("Compose Workload"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_COMPOSEWORKLOAD : SINGLE_COMPOSEWORKLOAD,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, semanticCat, COMPOSEWORKLOAD );

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

        AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, semanticCat,
                  wxT("Workload"), wxT(""), wxT("Workload"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_WORKLOAD : SINGLE_WORKLOAD,
                  arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected );
        semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, semanticCat, WORKLOAD );
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

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, semanticCat,
                wxT("Compose Appl"), wxT(""), wxT("Compose Appl"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_COMPOSEAPPL : SINGLE_COMPOSEAPPL,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, semanticCat, COMPOSEAPPLICATION );

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

        AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, semanticCat,
                  wxT("Application"), wxT(""), wxT("Application"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_APPLICATION : SINGLE_APPLICATION,
                  arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected );
        semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, semanticCat, APPLICATION );
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

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, semanticCat,
                wxT("Compose Task"), wxT(""), wxT("Compose Task"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_COMPOSETASK : SINGLE_COMPOSETASK,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, semanticCat, COMPOSETASK );

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

        AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, semanticCat,
                  wxT("Task"), wxT(""), wxT("Task"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_TASK : SINGLE_TASK,
                  arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected );
        semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, semanticCat, TASK );
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

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, semanticCat,
                wxT("Compose Thread"), wxT(""), wxT("Compose Thread"), (TSingleTimelineProperties)DERIVED_COMPOSETHREAD,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, semanticCat, COMPOSETHREAD );
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

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, semanticCat,
                wxT("Compose System"), wxT(""), wxT("Compose System"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_COMPOSESYSTEM : SINGLE_COMPOSESYSTEM,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, semanticCat, COMPOSESYSTEM );

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

        AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, semanticCat,
                  wxT("System"), wxT(""), wxT("System"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_SYSTEM : SINGLE_SYSTEM,
                  arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected );
        semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, semanticCat, SYSTEM );
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

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, semanticCat,
                wxT("Compose Node"), wxT(""), wxT("Compose Node"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_COMPOSENODE : SINGLE_COMPOSENODE,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, semanticCat, COMPOSENODE );

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

        AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, semanticCat,
                  wxT("Node"), wxT(""), wxT("Node"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_NODE : SINGLE_NODE,
                  arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected );
        semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, semanticCat, NODE );
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

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, semanticCat,
                wxT("Compose CPU"), wxT(""), wxT("Compose CPU"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_COMPOSECPU : SINGLE_COMPOSECPU,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );

      semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, semanticCat, COMPOSECPU );
      
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

        AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, semanticCat,
                  wxT("CPU"), wxT(""), wxT("CPU"), SINGLE_CPU,
                  arrayStr, arrayInt, selected );

        semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, semanticCat, CPU );
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, semanticCat,
              wxT("Compose Thread"), wxT(""), wxT("Compose Thread"), SINGLE_COMPOSETHREAD,
              arrayComposeFunctions, arrayComposeFunctionsPos, selected );
    semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, semanticCat, COMPOSETHREAD );

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
    AppendCFG4DprvSemanticThreadPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, semanticCat,
              wxT("Thread"), wxT(""), wxT("Thread"), SINGLE_THREAD,
              levels, threadFunctions, auxDefaultValue );
    semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, semanticCat, THREAD );
  }
  
  if( whichWindow->isDerivedWindow() )
  {
    AppendCFG4DFloatPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, dummyPGId,
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, semanticCat,
              wxT("Derived"), wxT(""), wxT("Derived"), (TSingleTimelineProperties)DERIVED_DERIVED,
              arrayStr, arrayInt, selected );
    semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, semanticCat, DERIVED );

    AppendCFG4DFloatPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, dummyPGId,
            wxT("Factor #2"), wxT("Factor #2"), (TSingleTimelineProperties)DERIVED_FACTOR2,
            whichWindow->getFactor( 1 ) );
  }
  // END of Semantic related properties
  
  if ( whichWindow->getCFG4DEnabled() && whichWindow->getCFG4DMode() && whichWindow->isDerivedWindow() )
  {
    updateTimelinePropertiesRecursive( windowProperties, whichWindow->getParent( 0 ), whichPropertiesClientData );
    updateTimelinePropertiesRecursive( windowProperties, whichWindow->getParent( 1 ), whichPropertiesClientData );
  }
}


void updateTimelineProperties( wxPropertyGrid* windowProperties,
                               Window *whichWindow,
                               std::vector< PropertyClientData * >& whichPropertiesClientData )
{
  updateCategoriesState( windowProperties );
  closeOpenedPropertyDialog();
  windowProperties->Freeze();
  windowProperties->Clear();
  
  wxArrayString arrayStr;
  wxArrayInt arrayInt;
  int selected;

  for( std::vector< PropertyClientData * >::iterator it = whichPropertiesClientData.begin();
       it != whichPropertiesClientData.end(); ++it )
    delete *it;
  whichPropertiesClientData.clear();

  propNameCounter = 0;

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
    fillPropertyClientData( whichWindow, NULL, tmpSelector, wxT("Mode"), whichPropertiesClientData );
  }

  updateTimelinePropertiesRecursive( windowProperties, whichWindow, whichPropertiesClientData );

  windowProperties->SetPropertyAttributeAll( wxPG_BOOL_USE_CHECKBOX, true );
  windowProperties->Refresh();
  windowProperties->Thaw();
}


void updateHistogramProperties( wxPropertyGrid* windowProperties,
                                Histogram *whichHisto,
                                std::vector< PropertyClientData * >& whichPropertiesClientData )
{
  PRV_UINT32 precision = ParaverConfig::getInstance()->getTimelinePrecision();

  updateCategoriesState( windowProperties );
  
  wxArrayString arrayStr;
  wxArrayInt arrayInt;
  int selected, pos;

  whichHisto->setChanged( false );
  closeOpenedPropertyDialog();
  windowProperties->Freeze();
  windowProperties->Clear();

  for( std::vector< PropertyClientData * >::iterator it = whichPropertiesClientData.begin();
       it != whichPropertiesClientData.end(); ++it )
    delete *it;
  whichPropertiesClientData.clear();

  propNameCounter = 0;

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
    fillPropertyClientData( NULL, whichHisto, tmpSelector, wxT("Mode"), whichPropertiesClientData );
  }

  wxPGId dummyPGId = (wxPGId)NULL; // used to append always to windowProperties

  AppendCFG4DStringPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, dummyPGId,
          wxT("Name"), wxT("Name"), HISTOGRAM_NAME, whichHisto->getName() );

  AppendCFG4DStringPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, dummyPGId,
          wxT("Begin time"), wxT("Begin time"), HISTOGRAM_BEGINTIME,
          LabelConstructor::timeLabel(
                  whichHisto->getControlWindow()->traceUnitsToWindowUnits( whichHisto->getBeginTime() ),
                  whichHisto->getControlWindow()->getTimeUnit(),
                  precision ));

  AppendCFG4DStringPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, dummyPGId,
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

  AppendCFG4DTimelineTreePropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, ctrlCat,
          wxT("Window"), wxT("ControlWindow"), HISTOGRAM_CONTROLWINDOW, validWin, whichHisto->getControlWindow() );

  AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, ctrlCat,
          wxT("Minimum"), wxT("ControlMinimum"), HISTOGRAM_CONTROLMINIMUM, whichHisto->getControlMin() );
  AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, ctrlCat,
          wxT("Maximum"), wxT("ControlMaximum"), HISTOGRAM_CONTROLMAXIMUM, whichHisto->getControlMax() );
  AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, ctrlCat,
          wxT("Delta"), wxT("ControlDelta"), HISTOGRAM_CONTROLDELTA, whichHisto->getControlDelta() );

  // Statistic related properties
  wxPGId statCat = (wxPGId)NULL;
  if ( !whichHisto->getCFG4DEnabled() || !whichHisto->getCFG4DMode() )
  {
    statCat = windowProperties->Append( new wxPropertyCategory( wxT("Statistics"), wxT("Statistics") ) );
    if( statCatCollapsed )
      statCat->SetFlagsFromString( _( "COLLAPSED" ) );
  }

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

  AppendCFG4DEnumPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, statCat,
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

  AppendCFG4DEnumPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, statCat,
          wxT("Statistic"), wxT("Statistic"), HISTOGRAM_STATISTIC, arrayStr, arrayInt, selected );

  AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, statCat,
          wxT("Minimum Gradient"), wxT("DataMinimum"), HISTOGRAM_MINIMUMGRADIENT, whichHisto->getMinGradient() );

  AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, statCat,
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

  AppendCFG4DTimelineTreePropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, dataCat,
           wxT("Window"), wxT("DataWindow"), HISTOGRAM_DATAWINDOW, validWin, whichHisto->getDataWindow() );

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

  AppendCFG4DTimelineTreePropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, thirdWinCat,
           wxT("3rd Window"), wxT("3D3rdWindow"), HISTOGRAM_3D3RDWINDOW, validWin, whichHisto->getExtraControlWindow(), true );

  wxPGId thirdWinMinimum = AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, thirdWinCat,
          wxT("Minimum"), wxT("3DMinimum"), HISTOGRAM_3DMINIMUM, whichHisto->getExtraControlMin() );

  wxPGId thirdWinMaximum = AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, thirdWinCat,
          wxT("Maximum"), wxT("3DMaximum"), HISTOGRAM_3DMAXIMUM, whichHisto->getExtraControlMax() );

  wxPGId thirdWinDelta = AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, thirdWinCat,
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

  wxPGId thirdWinPlane = AppendCFG4DEnumPropertyHistogram( windowProperties,
                                                           whichHisto,
                                                           whichPropertiesClientData,
                                                           thirdWinCat,
                                                           wxT("Plane"),
                                                           wxT("3DPlane"),
                                                           HISTOGRAM_3DPLANE,
                                                           arrayStr,
                                                           arrayInt,
                                                           selected );

  if( !whichHisto->getThreeDimensions() )
  {
    if ( thirdWinMinimum != NULL )
#if wxMAJOR_VERSION<3
      thirdWinMinimum->SetFlagsFromString( _( "DISABLED" ) );
#else
      thirdWinMinimum->Enable( false );
#endif

    if ( thirdWinMaximum != NULL )
#if wxMAJOR_VERSION<3
      thirdWinMaximum->SetFlagsFromString( _( "DISABLED" ) );
#else
      thirdWinMaximum->Enable( false );
#endif

    if ( thirdWinDelta != NULL )
#if wxMAJOR_VERSION<3
      thirdWinDelta->SetFlagsFromString( _( "DISABLED" ) );
#else
      thirdWinDelta->Enable( false );
#endif

    if ( thirdWinPlane != NULL )
#if wxMAJOR_VERSION<3
      thirdWinPlane->SetFlagsFromString( _( "DISABLED" ) );
#else
      thirdWinPlane->Enable( false );
#endif
  }

  if ( whichHisto->getCFG4DEnabled() && whichHisto->getCFG4DMode() )
  {
    updateTimelinePropertiesRecursive( windowProperties,
                                       whichHisto->getControlWindow(),
                                       whichPropertiesClientData );
    if( whichHisto->getDataWindow() != NULL &&
        whichHisto->getDataWindow() != whichHisto->getControlWindow() )
    {
      updateTimelinePropertiesRecursive( windowProperties,
                                         whichHisto->getDataWindow(),
                                         whichPropertiesClientData );
    }

    if( whichHisto->getThreeDimensions() &&
        whichHisto->getExtraControlWindow() != whichHisto->getControlWindow() &&
        whichHisto->getExtraControlWindow() != whichHisto->getDataWindow() )
    {
      updateTimelinePropertiesRecursive( windowProperties,
                                         whichHisto->getExtraControlWindow(),
                                         whichPropertiesClientData );
    }
  }

  windowProperties->SetPropertyAttributeAll( wxPG_BOOL_USE_CHECKBOX, true );
  windowProperties->Refresh();
  windowProperties->Thaw();
}
