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
#include <set>
#include <sstream>
#include <wx/object.h> 
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/editors.h>
#include <wx/valnum.h>
#include <wx/artprov.h>

// CFG4D
#include "cfg.h"

using namespace std;

typedef  wxPGProperty* wxPGId;

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
  wxDECLARE_DYNAMIC_CLASS( wxSpinButtonsEditor );
#else
  WX_PG_DECLARE_EDITOR_CLASS( wxSpinButtonsEditor );
#endif
    
  public:
    wxSpinButtonsEditor() {}
    virtual ~wxSpinButtonsEditor() {}
    virtual wxString GetName() const { return wxT( "SpinButtonsEditor" ); }
    virtual wxPGWindowList CreateControls( wxPropertyGrid* propGrid,
                                           wxPGProperty* property,
                                           const wxPoint& pos,
                                           const wxSize& sz ) const;
    virtual bool OnEvent( wxPropertyGrid* propGrid,
                          wxPGProperty* property,
                          wxWindow* ctrl,
                          wxEvent& event ) const;
};

wxIMPLEMENT_DYNAMIC_CLASS( wxSpinButtonsEditor, wxPGTextCtrlEditor );

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
  buttons->Finalize( propGrid, pos );
  wndList.SetSecondary( buttons );

  return wndList;
}

bool wxSpinButtonsEditor::OnEvent( wxPropertyGrid* propGrid,
                                   wxPGProperty* property,
                                   wxWindow* ctrl,
                                   wxEvent& event ) const
{
  if ( event.GetEventType() == wxEVT_BUTTON )
  {
    wxPGMultiButton *buttons = ( wxPGMultiButton* ) propGrid->GetEditorControlSecondary();

    double tmpValue = property->GetValue().GetDouble();
    if( property->GetName() == wxT( "Semantic Minimum" ) || property->GetName() == wxT( "Semantic Maximum" ) )
    {
      Timeline *currentWin = paraverMain::myParaverMain->GetCurrentTimeline();
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
  wxDECLARE_DYNAMIC_CLASS( wxChoiceAndMinusButtonEditor );
    
  public:
    wxChoiceAndMinusButtonEditor() {}
    virtual ~wxChoiceAndMinusButtonEditor() {}
    virtual wxString GetName() const { return wxT( "ChoiceAndMinusButtonEditor" ); }
    virtual wxPGWindowList CreateControls( wxPropertyGrid* propGrid,
                                           wxPGProperty* property,
                                           const wxPoint& pos,
                                           const wxSize& sz ) const;
    virtual bool OnEvent( wxPropertyGrid* propGrid,
                          wxPGProperty* property,
                          wxWindow* ctrl,
                          wxEvent& event ) const;
};

wxIMPLEMENT_DYNAMIC_CLASS( wxChoiceAndMinusButtonEditor, wxPGChoiceEditor );

wxPGWindowList wxChoiceAndMinusButtonEditor::CreateControls( wxPropertyGrid* propGrid,
                                                             wxPGProperty* property,
                                                             const wxPoint& pos,
                                                             const wxSize& sz ) const
{
  wxPGMultiButton* buttons = new wxPGMultiButton( propGrid, sz );

  buttons->Add( wxT( "-" ) );

  wxPGWindowList wndList = wxPGChoiceEditor::CreateControls( propGrid, property, pos,
                                                               buttons->GetPrimarySize() );
  buttons->Finalize( propGrid, pos );
  wndList.SetSecondary( buttons );

  return wndList;
}

bool wxChoiceAndMinusButtonEditor::OnEvent( wxPropertyGrid* propGrid,
                                            wxPGProperty* property,
                                            wxWindow* ctrl,
                                            wxEvent& event ) const
{
  if ( event.GetEventType() == wxEVT_BUTTON )
  {
  }
  
  return wxPGChoiceEditor::OnEvent( propGrid, property, ctrl, event );
}


class wxChoiceAndPlusButtonEditor : public wxPGChoiceEditor
{
  wxDECLARE_DYNAMIC_CLASS( wxChoiceAndPlusButtonEditor );
    
  public:
    wxChoiceAndPlusButtonEditor() {}
    virtual ~wxChoiceAndPlusButtonEditor() {}
    virtual wxString GetName() const { return wxT( "ChoiceAndPlusButtonEditor" ); }
    virtual wxPGWindowList CreateControls( wxPropertyGrid* propGrid,
                                           wxPGProperty* property,
                                           const wxPoint& pos,
                                           const wxSize& sz ) const;
    virtual bool OnEvent( wxPropertyGrid* propGrid,
                          wxPGProperty* property,
                          wxWindow* ctrl,
                          wxEvent& event ) const;
};

wxIMPLEMENT_DYNAMIC_CLASS( wxChoiceAndPlusButtonEditor, wxPGChoiceEditor );

wxPGWindowList wxChoiceAndPlusButtonEditor::CreateControls( wxPropertyGrid* propGrid,
                                                            wxPGProperty* property,
                                                            const wxPoint& pos,
                                                            const wxSize& sz ) const
{
  wxPGMultiButton* buttons = new wxPGMultiButton( propGrid, sz );

  buttons->Add( wxT( "+" ) );

  wxPGWindowList wndList = wxPGChoiceEditor::CreateControls( propGrid, property, pos,
                                                             buttons->GetPrimarySize() );
  buttons->Finalize( propGrid, pos );
  wndList.SetSecondary( buttons );

  return wndList;
}

bool wxChoiceAndPlusButtonEditor::OnEvent( wxPropertyGrid* propGrid,
                                           wxPGProperty* property,
                                           wxWindow* ctrl,
                                           wxEvent& event ) const
{
  if ( event.GetEventType() == wxEVT_BUTTON )
  {
  }
  
  return wxPGChoiceEditor::OnEvent(propGrid, property, ctrl, event);
}


class wxChoiceAndBothButtonsEditor : public wxPGChoiceEditor
{
  wxDECLARE_DYNAMIC_CLASS( wxChoiceAndBothButtonsEditor );
    
  public:
    wxChoiceAndBothButtonsEditor() {}
    virtual ~wxChoiceAndBothButtonsEditor() {}
    virtual wxString GetName() const { return wxT( "ChoiceAndBothButtonsEditor" ); }
    virtual wxPGWindowList CreateControls( wxPropertyGrid* propGrid,
                                           wxPGProperty* property,
                                           const wxPoint& pos,
                                           const wxSize& sz ) const;
    virtual bool OnEvent( wxPropertyGrid* propGrid,
                          wxPGProperty* property,
                          wxWindow* ctrl,
                          wxEvent& event ) const;
};

wxIMPLEMENT_DYNAMIC_CLASS( wxChoiceAndBothButtonsEditor, wxPGChoiceEditor );

wxPGWindowList wxChoiceAndBothButtonsEditor::CreateControls( wxPropertyGrid* propGrid,
                                                             wxPGProperty* property,
                                                             const wxPoint& pos,
                                                             const wxSize& sz ) const
{
  wxPGMultiButton* buttons = new wxPGMultiButton( propGrid, sz );


  buttons->Add( wxArtProvider::GetBitmap( wxART_PLUS ) );
  buttons->Add( wxArtProvider::GetBitmap( wxART_MINUS ) );

  wxPGWindowList wndList = wxPGChoiceEditor::CreateControls( propGrid, property, pos,
                                                             buttons->GetPrimarySize() );
  buttons->Finalize( propGrid, pos );
  wndList.SetSecondary( buttons );

  return wndList;
}


bool wxChoiceAndBothButtonsEditor::OnEvent( wxPropertyGrid* propGrid,
                                            wxPGProperty* property,
                                            wxWindow* ctrl,
                                            wxEvent& event ) const
{
  if ( event.GetEventType() == wxEVT_BUTTON )
  {
    Timeline *tmpTimeline = ( ( PropertyClientData * )property->GetClientData() )->ownerTimeline;
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


//static wxSpinButtonsEditor *spinButtonsEditor = nullptr;
static wxChoiceAndMinusButtonEditor *choiceAndMinusButtonEditor = nullptr;
static wxChoiceAndPlusButtonEditor  *choiceAndPlusButtonEditor  = nullptr;
static wxChoiceAndBothButtonsEditor *choiceAndBothButtonsEditor  = nullptr;


void initPG()
{
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
}


void fillPropertyClientData( Timeline * whichWindow,
                             Histogram* whichHistogram,
                             wxPGProperty* whichProperty,
                             const std::string& widgetName,
                             std::vector< PropertyClientData * >& whichPropertiesClientData,
                             PropertyClientData *whichClientData = nullptr )
{
  if ( whichProperty != nullptr )
  {
    PropertyClientData *tmpClientData = whichClientData;
    if( tmpClientData == nullptr )
      tmpClientData = new PropertyClientData();
    tmpClientData->ownerTimeline = whichWindow;
    tmpClientData->ownerHistogram = whichHistogram;
    tmpClientData->propName = widgetName;
    whichPropertiesClientData.push_back( tmpClientData );
    whichProperty->SetClientData( tmpClientData );
  }
}

class CFGS4DLinkedPropertyShown
{
  public:
    TCFGS4DGroup groupLink;
    string propertyName;

    bool operator<( const CFGS4DLinkedPropertyShown& other ) const
    {
      if( groupLink < other.groupLink )
        return true;
      
      return propertyName < other.propertyName;
    }
};


bool insertLinkedPropertyShown( Timeline* whichWindow,
                                Histogram* whichHistogram,
                                PRV_UINT32 propertyIndex,
                                set< CFGS4DLinkedPropertyShown >& linkedPropertiesShown,
                                std::string propertyName = "" )
{
  CFGS4DLinkedPropertyShown tmpLinked;

  if( whichWindow != nullptr )
  {
    if ( propertyName != "" )
       tmpLinked.propertyName = propertyName;
    else if ( whichWindow->isDerivedWindow() )
      tmpLinked.propertyName = DerivedTimelinePropertyLabels[ propertyIndex ];
    else
      tmpLinked.propertyName = SingleTimelinePropertyLabels[ propertyIndex ];
    
    tmpLinked.groupLink = whichWindow->getCFGS4DGroupLink( tmpLinked.propertyName );
  }
  else if( whichHistogram != nullptr )
  {
    if ( propertyName != "" )
      tmpLinked.propertyName = propertyName;
    else
      tmpLinked.propertyName = HistogramPropertyLabels[ propertyIndex ];

    tmpLinked.groupLink = whichHistogram->getCFGS4DGroupLink( tmpLinked.propertyName );
  }

  if( tmpLinked.groupLink == NO_GROUP_LINK )
    return true;

  return linkedPropertiesShown.insert( tmpLinked ).second;
}


wxPGId AppendCFG4DBoolPropertyWindow( wxPropertyGrid* windowProperties,
                                      Timeline * whichWindow,
                                      std::vector< PropertyClientData * >& whichPropertiesClientData,
                                      set< CFGS4DLinkedPropertyShown >& linkedPropertiesShown,
                                      wxPGId fatherWidget,
                                      const wxString &widgetLabel,
                                      TSingleTimelineProperties propertyIndex,
                                      const bool &propertyValue )
{
  wxPGId retId = (wxPGId)nullptr;
  wxBoolProperty *auxProperty = nullptr;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;
  
  std::string widgetName;
  if ( whichWindow->isDerivedWindow() ) 
    widgetName = DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ];
  else
    widgetName = SingleTimelinePropertyLabels[ propertyIndex ];

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
  else
  {
    if ( !whichWindow->isDerivedWindow() &&
          whichWindow->existsCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ) &&
          insertLinkedPropertyShown( whichWindow, nullptr, propertyIndex, linkedPropertiesShown ) )
    {
      // CFG4D mode (single timeline)
      wxString auxTag = wxString::FromUTF8( 
              whichWindow->getCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ).c_str() );

      auxProperty = new wxBoolProperty( auxTag, tmpWidgetName, propertyValue );

      retId = windowProperties->Append( auxProperty );
    }
    else if ( whichWindow->isDerivedWindow() &&
              whichWindow->existsCFG4DAlias(
                      DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ) &&
              insertLinkedPropertyShown( whichWindow, nullptr, propertyIndex, linkedPropertiesShown ) )
    {
      // CFG4D mode (derived timeline)
      wxString auxTag = wxString::FromUTF8( 
              whichWindow->getCFG4DAlias( 
                      DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ).c_str() );

      auxProperty = new wxBoolProperty( auxTag, tmpWidgetName, propertyValue );

      retId = windowProperties->Append( auxProperty );
    }
  }

  fillPropertyClientData( whichWindow, nullptr, auxProperty, widgetName, whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DStringPropertyWindow( wxPropertyGrid* windowProperties,
                                        Timeline * whichWindow,
                                        std::vector< PropertyClientData * >& whichPropertiesClientData,
                                        set< CFGS4DLinkedPropertyShown >& linkedPropertiesShown,
                                        wxPGId fatherWidget,
                                        const wxString &widgetLabel,
                                        TSingleTimelineProperties propertyIndex,
                                        const string &propertyValue )
{
  wxPGId retId = (wxPGId)nullptr;
  wxStringProperty *auxProperty = nullptr;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;
  
  std::string widgetName;
  if ( whichWindow->isDerivedWindow() ) 
    widgetName = DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ];
  else
    widgetName = SingleTimelinePropertyLabels[ propertyIndex ];

  if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
  {
    // NORMAL mode
    auxProperty = new wxStringProperty(
            widgetLabel, tmpWidgetName, wxString::FromUTF8( propertyValue.c_str() ) );

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
            whichWindow->existsCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ) &&
            insertLinkedPropertyShown( whichWindow, nullptr, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode (single timeline)
    wxString auxTag = wxString::FromUTF8( 
            whichWindow->getCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ).c_str() );

    auxProperty = new wxStringProperty( 
            auxTag, tmpWidgetName, wxString::FromUTF8( propertyValue.c_str() ) );

    retId = windowProperties->Append( auxProperty );
  }
  else if ( whichWindow->isDerivedWindow() && 
            whichWindow->existsCFG4DAlias( DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ) &&
            insertLinkedPropertyShown( whichWindow, nullptr, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode (derived timeline)
    wxString auxTag = wxString::FromUTF8( 
            whichWindow->getCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ).c_str() );

    auxProperty = new wxStringProperty( 
            auxTag, tmpWidgetName, wxString::FromUTF8( propertyValue.c_str() ) );

    retId = windowProperties->Append( auxProperty );
  }
  
  fillPropertyClientData( whichWindow, nullptr, auxProperty, widgetName, whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DEnumPropertyWindow( wxPropertyGrid* windowProperties,
                                      Timeline * whichWindow,
                                      std::vector< PropertyClientData * >& whichPropertiesClientData,
                                      set< CFGS4DLinkedPropertyShown >& linkedPropertiesShown,
                                      wxPGId fatherWidget,
                                      const wxString &widgetLabel,
                                      TSingleTimelineProperties propertyIndex,
                                      const wxArrayString &arrayStr,
                                      const wxArrayInt &arrayInt,
                                      const int selected,
                                      const ButtonType addButton = NO_BUTTON,
                                      PropertyClientData *whichClientData = nullptr )
{
  wxPGId retId = (wxPGId)nullptr;
  wxEnumProperty *auxProperty = nullptr;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;

  std::string widgetName;
  if ( whichWindow->isDerivedWindow() ) 
    widgetName = DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ];
  else
    widgetName = SingleTimelinePropertyLabels[ propertyIndex ];

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
          windowProperties->SetPropertyEditor( auxProperty, choiceAndPlusButtonEditor );
        break;

      case MINUS_BUTTON:
          windowProperties->SetPropertyEditor( auxProperty, choiceAndMinusButtonEditor );

        break;
      
      case BOTH_BUTTONS:
          windowProperties->SetPropertyEditor( auxProperty, choiceAndBothButtonsEditor );
        break;

      default:
        break;
    }
  }
  else if ( !whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ) &&
            insertLinkedPropertyShown( whichWindow, nullptr, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode (single timeline)
    wxString auxTag = wxString::FromUTF8( 
            whichWindow->getCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ).c_str() );
    auxProperty = new wxEnumProperty( auxTag, tmpWidgetName, arrayStr, arrayInt, selected );
    retId = windowProperties->Append( auxProperty );
  }
  else if ( whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ) &&
            insertLinkedPropertyShown( whichWindow, nullptr, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode (derived timeline)
    wxString auxTag = wxString::FromUTF8( 
            whichWindow->getCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ).c_str() );
    auxProperty = new wxEnumProperty( auxTag, tmpWidgetName, arrayStr, arrayInt, selected );

    retId = windowProperties->Append( auxProperty );
  }

  fillPropertyClientData( whichWindow, nullptr, auxProperty, widgetName, whichPropertiesClientData, whichClientData );

  return retId;
}


wxPGId AppendCFG4DFloatPropertyWindow( wxPropertyGrid* windowProperties,
                                       Timeline * whichWindow,
                                       std::vector< PropertyClientData * >& whichPropertiesClientData,
                                       set< CFGS4DLinkedPropertyShown >& linkedPropertiesShown,
                                       wxPGId fatherWidget,
                                       const wxString &widgetLabel,
                                       TSingleTimelineProperties propertyIndex,
                                       const double propertyValue )
{
  wxPGId retId = (wxPGId)nullptr;
  wxFloatProperty *auxProperty = nullptr;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;

  std::string widgetName;
  if ( whichWindow->isDerivedWindow() ) 
    widgetName = DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ];
  else
    widgetName = SingleTimelinePropertyLabels[ propertyIndex ];

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
            whichWindow->existsCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ) &&
            insertLinkedPropertyShown( whichWindow, nullptr, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode (single timeline)
    wxString auxTag = wxString::FromUTF8( 
            whichWindow->getCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ).c_str() );
    auxProperty = new wxFloatProperty( auxTag, tmpWidgetName, propertyValue );

    retId = windowProperties->Append( auxProperty );
  }
  else if ( whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ) &&
            insertLinkedPropertyShown( whichWindow, nullptr, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode (derived timeline)
    wxString auxTag = wxString::FromUTF8( 
            whichWindow->getCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ).c_str() );
    auxProperty = new wxFloatProperty( auxTag, tmpWidgetName, propertyValue );

    retId = windowProperties->Append( auxProperty );
  }
    
// NOT WORKING PROPERLY
/*
  windowProperties->SetPropertyEditor( auxProperty, spinButtonsEditor );
*/

  fillPropertyClientData( whichWindow, nullptr, auxProperty, widgetName, whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DIntegerPropertyWindow( wxPropertyGrid* windowProperties,
                                         Timeline * whichWindow,
                                         std::vector< PropertyClientData * >& whichPropertiesClientData,
                                         set< CFGS4DLinkedPropertyShown >& linkedPropertiesShown,
                                         wxPGId fatherWidget,
                                         const wxString &widgetLabel,
                                         TSingleTimelineProperties propertyIndex,
                                         const long int propertyValue )
{
  wxPGId retId = (wxPGId)nullptr;
  wxIntProperty *auxProperty = nullptr;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;

  std::string widgetName;
  if ( whichWindow->isDerivedWindow() ) 
    widgetName = DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ];
  else
    widgetName = SingleTimelinePropertyLabels[ propertyIndex ];

  if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
  {
    // NORMAL mode
    auxProperty = new wxIntProperty( widgetLabel, tmpWidgetName, propertyValue );

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
            whichWindow->existsCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ) &&
            insertLinkedPropertyShown( whichWindow, nullptr, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode (single timeline)
    wxString auxTag = wxString::FromUTF8( 
            whichWindow->getCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ).c_str() );
    auxProperty = new wxIntProperty( auxTag, tmpWidgetName, propertyValue );

    retId = windowProperties->Append( auxProperty );
  }
  else if ( whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ) &&
            insertLinkedPropertyShown( whichWindow, nullptr, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode (derived timeline)
    wxString auxTag = wxString::FromUTF8( 
            whichWindow->getCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ).c_str() );
    auxProperty = new wxIntProperty( auxTag, tmpWidgetName, propertyValue );

    retId = windowProperties->Append( auxProperty );
  }
  
  
// NOT WORKING PROPERLY
/*
  windowProperties->SetPropertyEditor( auxProperty, spinButtonsEditor );
*/

  fillPropertyClientData( whichWindow, nullptr, auxProperty, widgetName, whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DprvRowsSelectionPropertyWindow( wxPropertyGrid* windowProperties,
                                                  Timeline * whichWindow,
                                                  std::vector< PropertyClientData * >& whichPropertiesClientData,
                                                  set< CFGS4DLinkedPropertyShown >& linkedPropertiesShown,
                                                  wxPGId fatherWidget,
                                                  const wxString &widgetLabel,
                                                  TSingleTimelineProperties propertyIndex,
                                                  const wxString &widgetTitle,
                                                  vector< TObjectOrder > &objects )
{
  wxPGId retId = (wxPGId)nullptr;
  prvRowsSelectionProperty *auxProperty = nullptr;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;

  std::string widgetName;
  if ( whichWindow->isDerivedWindow() ) 
    widgetName = DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ];
  else
    widgetName = SingleTimelinePropertyLabels[ propertyIndex ];

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
            whichWindow->existsCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ) &&
            insertLinkedPropertyShown( whichWindow, nullptr, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode (single timeline)
    wxString auxTag = wxString::FromUTF8( 
            whichWindow->getCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ).c_str() );
    auxProperty = new prvRowsSelectionProperty( windowProperties, whichWindow,
                                                widgetTitle, objects, auxTag, tmpWidgetName );

    retId = windowProperties->Append( auxProperty );
  }
  else if ( whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ) &&
            insertLinkedPropertyShown( whichWindow, nullptr, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode (derived timeline)
    wxString auxTag = wxString::FromUTF8( 
            whichWindow->getCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ).c_str() );
    auxProperty = new prvRowsSelectionProperty( windowProperties, whichWindow,
                                                widgetTitle, objects, auxTag, tmpWidgetName );

    retId = windowProperties->Append( auxProperty );
  }

  fillPropertyClientData( whichWindow, nullptr, auxProperty, widgetName, whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DprvNumbersListPropertyWindow( wxPropertyGrid* windowProperties,
                                                Timeline * whichWindow,
                                                std::vector< PropertyClientData * >& whichPropertiesClientData,
                                                set< CFGS4DLinkedPropertyShown >& linkedPropertiesShown,
                                                wxPGId fatherWidget,
                                                const wxString &widgetLabel,
                                                TSingleTimelineProperties propertyIndex,
                                                const wxArrayString &arrayStr )
{
  wxPGId retId = (wxPGId)nullptr;
  prvNumbersListProperty *auxProperty = nullptr;
  wxString auxTag;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;

  std::string widgetName;
  if ( whichWindow->isDerivedWindow() ) 
    widgetName = DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ];
  else
    widgetName = SingleTimelinePropertyLabels[ propertyIndex ];

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
            whichWindow->existsCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ) &&
            insertLinkedPropertyShown( whichWindow, nullptr, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode (single timeline)
    auxTag = wxString::FromUTF8( whichWindow->getCFG4DAlias(
                    SingleTimelinePropertyLabels[ propertyIndex ] ).c_str() );
    auxProperty = new prvNumbersListProperty( auxTag, tmpWidgetName, arrayStr );

    retId = windowProperties->Append( auxProperty );
  }
  else if ( whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ) &&
            insertLinkedPropertyShown( whichWindow, nullptr, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode (derived timeline)
    auxTag = wxString::FromUTF8( whichWindow->getCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ).c_str() );
    auxProperty = new prvNumbersListProperty( auxTag, tmpWidgetName, arrayStr );

    retId = windowProperties->Append( auxProperty );
  }

  fillPropertyClientData( whichWindow, nullptr, auxProperty, widgetName, whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DprvEventInfoPropertyWindow( wxPropertyGrid* windowProperties,
                                              Timeline * whichWindow,
                                              std::vector< PropertyClientData * >& whichPropertiesClientData,
                                              set< CFGS4DLinkedPropertyShown >& linkedPropertiesShown,
                                              wxPGId fatherWidget,
                                              const wxString &widgetLabel,
                                              TSingleTimelineProperties propertyIndex,
                                              const wxPGChoices &choices )
{
  wxPGId retId = (wxPGId)nullptr;
  prvEventInfoProperty *auxProperty = nullptr;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;
  
  std::string widgetName;
  if ( whichWindow->isDerivedWindow() ) 
    widgetName = DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ];
  else
    widgetName = SingleTimelinePropertyLabels[ propertyIndex ];

  prvEventInfoType infoType;
  if ( propertyIndex == SINGLE_EVENTTYPEVALUES )
    infoType = prvEventInfoType::TYPES;
  else if ( propertyIndex == SINGLE_EVENTVALUEVALUES )
    infoType = prvEventInfoType::VALUES;

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
            whichWindow->existsCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ) &&
            insertLinkedPropertyShown( whichWindow, nullptr, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode (single timeline)
    wxString auxTag = wxString::FromUTF8( 
            whichWindow->getCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ).c_str() );
    auxProperty = new prvEventInfoProperty( auxTag, tmpWidgetName, choices, whichWindow, infoType );

    retId = windowProperties->Append( auxProperty );
  }
  else if ( whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ) &&
            insertLinkedPropertyShown( whichWindow, nullptr, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode (derived timeline)
    wxString auxTag = wxString::FromUTF8( 
            whichWindow->getCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ).c_str() );
    auxProperty = new prvEventInfoProperty( auxTag, tmpWidgetName, choices, whichWindow, infoType );

    retId = windowProperties->Append( auxProperty );  
  }

  fillPropertyClientData( whichWindow, nullptr, auxProperty, widgetName, whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DprvSemanticThreadPropertyWindow( wxPropertyGrid* windowProperties,
                                                   Timeline * whichWindow,
                                                   std::vector< PropertyClientData * >& whichPropertiesClientData,
                                                   set< CFGS4DLinkedPropertyShown >& linkedPropertiesShown,
                                                   wxPGId fatherWidget,
                                                   const wxString &widgetLabel,
                                                   TSingleTimelineProperties propertyIndex,
                                                   vector<string> levels,
                                                   vector<vector<string> > threadFunctions,
                                                   const wxString &defaultValue )
{
  wxPGId retId = (wxPGId)nullptr;
  prvSemanticThreadProperty *auxProperty = nullptr;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;

  std::string widgetName;
  if ( whichWindow->isDerivedWindow() ) 
    widgetName = DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ];
  else
    widgetName = SingleTimelinePropertyLabels[ propertyIndex ];

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
            whichWindow->existsCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ) &&
            insertLinkedPropertyShown( whichWindow, nullptr, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode (single timeline)
    wxString auxTag = wxString::FromUTF8( 
            whichWindow->getCFG4DAlias( SingleTimelinePropertyLabels[ propertyIndex ] ).c_str() );
    auxProperty = new prvSemanticThreadProperty( auxTag, tmpWidgetName, 
                                                 levels, threadFunctions, defaultValue );

    retId = windowProperties->Append( auxProperty );
  }
  else if ( whichWindow->isDerivedWindow() &&
            whichWindow->existsCFG4DAlias( DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ) &&
            insertLinkedPropertyShown( whichWindow, nullptr, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode (derived timeline)
    wxString auxTag = wxString::FromUTF8( 
            whichWindow->getCFG4DAlias( 
                    DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ] ).c_str() );
    auxProperty = new prvSemanticThreadProperty( auxTag, tmpWidgetName,
                                                 levels, threadFunctions, defaultValue );

    retId = windowProperties->Append( auxProperty );
  }

  fillPropertyClientData( whichWindow, nullptr, auxProperty, widgetName, whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DStringPropertyHistogram(  wxPropertyGrid* windowProperties,
                                            Histogram* whichHisto,
                                            std::vector< PropertyClientData * >& whichPropertiesClientData,
                                            set< CFGS4DLinkedPropertyShown >& linkedPropertiesShown,
                                            wxPGId fatherWidget,
                                            const wxString &widgetLabel,
                                            THistogramProperties propertyIndex,
                                            const string &propertyValue )
{
  wxPGId retId = (wxPGId)nullptr;
  wxStringProperty *auxProperty = nullptr;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;

  if ( !whichHisto->getCFG4DEnabled() || !whichHisto->getCFG4DMode() )
  {
    // NORMAL mode
    auxProperty = new wxStringProperty(
            widgetLabel, tmpWidgetName, wxString::FromUTF8( propertyValue.c_str() ) );

    if ( fatherWidget )
    {
      retId = windowProperties->AppendIn( fatherWidget, auxProperty );
    }
    else
    {
      retId = windowProperties->Append( auxProperty );
    }
  }
  else if ( whichHisto->existsCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ) &&
            insertLinkedPropertyShown( nullptr, whichHisto, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode
    wxString auxTag = wxString::FromUTF8( 
            whichHisto->getCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ).c_str() );

    auxProperty = new wxStringProperty( 
            auxTag, tmpWidgetName, wxString::FromUTF8( propertyValue.c_str() ) );

    retId = windowProperties->Append( auxProperty );
  }

  fillPropertyClientData( nullptr, whichHisto, auxProperty, HistogramPropertyLabels[ propertyIndex ], whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DEnumPropertyHistogram(  wxPropertyGrid* windowProperties,
                                          Histogram* whichHisto,
                                          std::vector< PropertyClientData * >& whichPropertiesClientData,
                                          set< CFGS4DLinkedPropertyShown >& linkedPropertiesShown,
                                          wxPGId fatherWidget,
                                          const wxString &widgetLabel,
                                          THistogramProperties propertyIndex,
                                          const wxArrayString &arrayStr,
                                          const wxArrayInt &arrayInt,
                                          const int selected )
{
  wxPGId retId = (wxPGId)nullptr;
  wxEnumProperty *auxProperty = nullptr;

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
  else if ( whichHisto->existsCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ) &&
            insertLinkedPropertyShown( nullptr, whichHisto, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode
    wxString auxTag = wxString::FromUTF8( 
            whichHisto->getCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ).c_str() );
    auxProperty = new wxEnumProperty( auxTag, tmpWidgetName, arrayStr, arrayInt, selected );

    retId = windowProperties->Append( auxProperty );
  }
  
  fillPropertyClientData( nullptr, whichHisto, auxProperty, HistogramPropertyLabels[ propertyIndex ], whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DTimelineTreePropertyHistogram( wxPropertyGrid* windowProperties,
                                                 Histogram* whichHisto,
                                                 std::vector< PropertyClientData * >& whichPropertiesClientData,
                                                 set< CFGS4DLinkedPropertyShown >& linkedPropertiesShown,
                                                 wxPGId fatherWidget,
                                                 const wxString &widgetLabel,
                                                 THistogramProperties propertyIndex,
                                                 vector<TWindowID> windowsList,
                                                 Timeline *currentWindow,
                                                 bool needNoneElement = false )
{
  wxPGId retId = (wxPGId)nullptr;
  prvTimelineTreeProperty *auxProperty = nullptr;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;

  wxString valueStr;
  if( currentWindow == nullptr )
    valueStr = wxT( "None" );
  else
    valueStr = wxString( currentWindow->getName().c_str(), wxConvUTF8 );

  Trace *currentTrace;
  if( currentWindow == nullptr )
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
  else if ( whichHisto->existsCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ) &&
            insertLinkedPropertyShown( nullptr, whichHisto, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode
    wxString auxTag = wxString::FromUTF8( 
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

  fillPropertyClientData( nullptr, whichHisto, auxProperty, HistogramPropertyLabels[ propertyIndex ], whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DFloatPropertyHistogram(  wxPropertyGrid* windowProperties,
                                           Histogram* whichHisto,
                                           std::vector< PropertyClientData * >& whichPropertiesClientData,
                                           set< CFGS4DLinkedPropertyShown >& linkedPropertiesShown,
                                           wxPGId fatherWidget,
                                           const wxString &widgetLabel,
                                           THistogramProperties propertyIndex,
                                           const double propertyValue )
{
  wxPGId retId = (wxPGId)nullptr;
  wxFloatProperty *auxProperty = nullptr;

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
  else if ( whichHisto->existsCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ) &&
            insertLinkedPropertyShown( nullptr, whichHisto, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode
    wxString auxTag = wxString::FromUTF8( 
            whichHisto->getCFG4DAlias( HistogramPropertyLabels[ propertyIndex ] ).c_str() );
    auxProperty = new wxFloatProperty( auxTag, tmpWidgetName, propertyValue );

    retId = windowProperties->Append( auxProperty );
  }

  fillPropertyClientData( nullptr, whichHisto, auxProperty, HistogramPropertyLabels[ propertyIndex ], whichPropertiesClientData );

  return retId;
}


wxPGId AppendCFG4DParamPrvNumbersListPropertyWindow( wxPropertyGrid* windowProperties,
                                                     Timeline * whichWindow,
                                                     std::vector< PropertyClientData * >& whichPropertiesClientData,
                                                     set< CFGS4DLinkedPropertyShown >& linkedPropertiesShown,
                                                     wxPGId fatherWidget,
                                                     TWindowLevel currentSemanticLevel,
                                                     TParamIndex currentNumParameter,
                                                     const wxString &widgetLabel,
                                                     TSingleTimelineProperties propertyIndex,
                                                     const wxArrayString &arrayStr,
                                                     bool isExtraCompose = false,
                                                     size_t extraComposePos = 0 )
{
  wxPGId retId = (wxPGId)nullptr;
  prvNumbersListProperty *auxProperty = nullptr;
  wxString auxTag;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;

  std::string widgetName;
  if ( whichWindow->isDerivedWindow() ) 
    widgetName = DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ];
  else
    widgetName = SingleTimelinePropertyLabels[ propertyIndex ];

  // Splitted Parameter alias key
  TWindowLevel kSemanticLevel;
  string kFunction;
  string currentFunction;
  TParamIndex kNumParameter;

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
    if( isExtraCompose )
      return retId;
    
    string currentSemanticLevelStr = TimelineLevelLabels[ currentSemanticLevel ];
    vector< Timeline::TParamAliasKey > paramAliasKey =
            whichWindow->getCFG4DParamKeysBySemanticLevel( currentSemanticLevelStr );
    for( vector< Timeline::TParamAliasKey >::const_iterator it = paramAliasKey.begin(); 
         it != paramAliasKey.end(); ++it )
    {
      whichWindow->splitCFG4DParamAliasKey( *it, kSemanticLevel, kFunction, kNumParameter );

      if ( !insertLinkedPropertyShown( whichWindow,
                                       nullptr,
                                       propertyIndex,
                                       linkedPropertiesShown,
                                       whichWindow->getCFG4DParameterOriginalName( kSemanticLevel, kNumParameter ) ) )
        continue;

      // CFG4D mode
      string currentFunction;
      if( isExtraCompose )
        currentFunction = whichWindow->getExtraLevelFunction( currentSemanticLevel, extraComposePos );
      else
        currentFunction = whichWindow->getLevelFunction( currentSemanticLevel );

      if ( kFunction      == currentFunction &&
           kNumParameter  == currentNumParameter )
      {
        auxTag = wxString::FromUTF8( whichWindow->getCFG4DParamAlias( *it ).c_str() );
        auxProperty = new prvNumbersListProperty( auxTag, tmpWidgetName, arrayStr );

        retId = windowProperties->Append( auxProperty );
      }
    }
  }

  PropertyClientData *tmpData = new PropertyClientData();
  tmpData->semanticLevel = currentSemanticLevel;
  tmpData->numParameter = currentNumParameter;
  tmpData->extraTopComposeLevel = extraComposePos;

  fillPropertyClientData( whichWindow, nullptr, auxProperty, widgetName, whichPropertiesClientData, tmpData );

  return retId;
}


inline void updateStateOf( wxPropertyGrid *windowProperties, bool& categoryStat, const wxString& catName )
{
  wxPGProperty *tmpProp = windowProperties->GetProperty( catName );

  if( tmpProp != nullptr )
    categoryStat = !windowProperties->IsPropertyExpanded( tmpProp );
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
                                            Timeline *whichWindow,
                                            std::vector< PropertyClientData * >& whichPropertiesClientData,
                                            set< CFGS4DLinkedPropertyShown >& linkedPropertiesShown,
                                            wxPGId category,
                                            TWindowLevel functionLevel,
                                            size_t whichPos )
{
  int tmpPropertyIndex;
  if ( whichWindow->isDerivedWindow() ) 
    tmpPropertyIndex = DERIVED_EXTRAFUNCTIONPARAMETERS;
  else
    tmpPropertyIndex = SINGLE_EXTRAFUNCTIONPARAMETERS;

  for( TParamIndex paramIdx = 0; paramIdx < whichWindow->getExtraFunctionNumParam( functionLevel, whichPos - 1 ); ++paramIdx )
  {
    wxArrayString valuesStr;
    TParamValue values = whichWindow->getExtraFunctionParam( functionLevel, whichPos - 1, paramIdx );
    for( TParamValue::iterator it = values.begin(); it != values.end(); ++it )
      valuesStr.Add( wxString() << (*it) );

    wxString propLabel = wxString( _("   ") ) +
            wxString::FromUTF8( whichWindow->getExtraFunctionParamName( functionLevel, whichPos - 1, paramIdx ).c_str() );
    wxPGId semanticFunctionParameterValue = AppendCFG4DParamPrvNumbersListPropertyWindow(
            windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown,
            category,
            functionLevel, paramIdx,
            propLabel, (TSingleTimelineProperties) tmpPropertyIndex,
            valuesStr, true, whichPos );
  }
}

void semanticFunctionParameter( wxPropertyGrid* windowProperties,
                                Timeline *whichWindow,
                                std::vector< PropertyClientData * >& whichPropertiesClientData,
                                set< CFGS4DLinkedPropertyShown >& linkedPropertiesShown,
                                wxPGId category,
                                TWindowLevel functionLevel )
{
  int tmpPropertyIndex;
  if ( whichWindow->isDerivedWindow() ) 
    tmpPropertyIndex = DERIVED_FUNCTIONPARAMETERS;
  else
    tmpPropertyIndex = SINGLE_FUNCTIONPARAMETERS;

  for( TParamIndex paramIdx = 0; paramIdx < whichWindow->getFunctionNumParam( functionLevel ); ++paramIdx )
  {
    wxArrayString valuesStr;
    TParamValue values = whichWindow->getFunctionParam( functionLevel, paramIdx );
    for( TParamValue::iterator it = values.begin(); it != values.end(); ++it )
      valuesStr.Add( wxString() << (*it) );

    wxString propLabel = wxString( _("   ") ) +
            wxString::FromUTF8( whichWindow->getFunctionParamName( functionLevel, paramIdx ).c_str() );
    wxPGId semanticFunctionParameterValue = AppendCFG4DParamPrvNumbersListPropertyWindow(
            windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, category,
            functionLevel, paramIdx,
            propLabel, (TSingleTimelineProperties) tmpPropertyIndex,
            valuesStr );
  }
}


void closeOpenedPropertyDialog()
{
  wxDialog *tmpDialog = wxparaverApp::mainWindow->GetOpenedPropertyDialog();
  if( tmpDialog != nullptr )
    tmpDialog->EndModal( wxID_CANCEL );
}

void updateTimelinePropertiesRecursive( wxPropertyGrid* windowProperties, Timeline *whichWindow,
                                        std::vector< PropertyClientData * >& whichPropertiesClientData,
                                        set< CFGS4DLinkedPropertyShown >& linkedPropertiesShown )
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
    filter = nullptr;
  else
    filter = whichWindow->getFilter();

  wxPGId dummyPGId = (wxPGId)nullptr; // used to append always to windowProperties

  AppendCFG4DStringPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
          wxT("Name"), SINGLE_NAME, whichWindow->getName() );

  AppendCFG4DStringPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
          wxT("Begin time"), SINGLE_BEGINTIME,
          LabelConstructor::timeLabel(
                  whichWindow->traceUnitsToWindowUnits( whichWindow->getWindowBeginTime() ),
                  whichWindow->getTimeUnit(),
                  precision ));

  AppendCFG4DStringPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
          wxT("End time"), SINGLE_ENDTIME,
          LabelConstructor::timeLabel(
                  whichWindow->traceUnitsToWindowUnits( whichWindow->getWindowEndTime() ),
                  whichWindow->getTimeUnit(),
                  precision ));

  AppendCFG4DFloatPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
          wxT("Semantic Minimum"), SINGLE_SEMANTICMINIMUM, whichWindow->getMinimumY() );

  AppendCFG4DFloatPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
          wxT("Semantic Maximum"), SINGLE_SEMANTICMAXIMUM,
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
            wxT("Level"), SINGLE_LEVEL,
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
            wxT("Level"), (TSingleTimelineProperties)DERIVED_LEVEL,
            arrayLevels, arrayLevelsPos, whichWindow->getLevel() );
  }

  arrayStr.Clear();
  arrayInt.Clear();
  for( int iUnit = 0; iUnit <= DAY; ++iUnit )
  {
    arrayStr.Add( wxString::FromUTF8( FULL_LABEL_TIMEUNIT[ iUnit ].c_str() ) );
    arrayInt.Add( iUnit );
  }

  AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
          wxT("Time unit"), SINGLE_TIMEUNIT,
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
      arrayFilterFunctions.Add( wxString::FromUTF8( (*it).c_str() ) );
      arrayFilterFunctionsPos.Add( pos );
      ++pos;
    }

    wxPGId filterCat = (wxPGId)nullptr;
    if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
    {
      filterCat = windowProperties->Append( new wxPropertyCategory( wxT("Filter"), wxT("Filter") ) );
      if( filterCatCollapsed )
        filterCat->SetFlagsFromString( _( "COLLAPSED" ) );
    }
    
    // ---------------------------- COMMUNICATION FILTER ---------------------------
    wxPGId commFilterCat = (wxPGId)nullptr;
    if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
    {
      commFilterCat = windowProperties->AppendIn( filterCat, new wxPropertyCategory( wxT("Communications"), wxT("Communications") ) );
      if( commFilterCatCollapsed )
        commFilterCat->SetFlagsFromString( _( "COLLAPSED" ) );
    }

    AppendCFG4DBoolPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterCat,
            wxT("Logical"), SINGLE_COMMLOGICAL, filter->getLogical() );

    AppendCFG4DBoolPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown,commFilterCat,
            wxT("Physical"), SINGLE_COMMPHYSICAL, filter->getPhysical() );

    // Comm From
    wxPGId commFilterFrom = ( wxPGId )nullptr;
    if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
    {
      commFilterFrom = windowProperties->AppendIn( commFilterCat, 
                                                   new wxStringProperty( wxT("Comm from"),
                                                                         wxT("Comm from"),
                                                                         wxT("<composed>") ) );
      if( commFilterFromCollapsed )
        commFilterFrom->SetFlagsFromString( _( "COLLAPSED" ) );
      commFilterFrom->Enable( false );
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterFrom,
            wxT( "Function" ), SINGLE_COMMFROMFUNCTION,
            arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    // add communication senders to arrayStr
    arrayStr.Clear();
    vector<TObjectOrder> fromSel;
    filter->getCommFrom( fromSel );
    wxPGId commFilterFromValues = (wxPGId)nullptr;

    commFilterFromValues = AppendCFG4DprvRowsSelectionPropertyWindow(
            windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterFrom,
            wxT( "From" ), SINGLE_COMMFROMVALUES,
            _("From - Rows Selection"), fromSel );

    if ( commFilterFromValues != (wxPGId)nullptr )
    {
      if( filter->getCommFromFunction() == "All" || filter->getCommFromFunction() == "None" )
      {
        commFilterFromValues->Enable( false );
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterCat,
             wxT("From/To Op"), SINGLE_COMMFROMTOOP,
             arrayStr, arrayInt, selected );

    // Comm To
    wxPGId commFilterTo = (wxPGId) nullptr; 
    if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
    {
      commFilterTo = windowProperties->AppendIn( commFilterCat, 
                                                 new wxStringProperty( wxT("Comm to"),
                                                                       wxT("Comm to"),
                                                                       wxT("<composed>") ) );
      if( commFilterToCollapsed )
        commFilterTo->SetFlagsFromString( _( "COLLAPSED" ) );
      commFilterTo->Enable( false );
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterTo,
            wxT( "Function" ), SINGLE_COMMTOFUNCTION,
            arrayFilterFunctions, arrayFilterFunctionsPos, selected );


    arrayStr.Clear();
    vector<TObjectOrder> toSel;
    filter->getCommTo( toSel );

    wxPGId commFilterToValues = (wxPGId)nullptr;
    commFilterToValues = AppendCFG4DprvRowsSelectionPropertyWindow(
            windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterTo,
            wxT( "To" ), SINGLE_COMMTOVALUES,
            _("To - Rows Selection"), toSel );

    if ( commFilterToValues != (wxPGId)nullptr )
    {
      if( filter->getCommToFunction() == "All" || filter->getCommToFunction() == "None" )
      {
        commFilterToValues->Enable( false );
      }
    }

    // Comm Tag
    wxPGId commFilterTag = (wxPGId)nullptr;
    if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
    {
      commFilterTag = windowProperties->AppendIn( commFilterCat, 
                                                  new wxStringProperty( wxT("Comm tag"),
                                                                        wxT("Comm tag"),
                                                                        wxT("<composed>") ) );
      if( commFilterTagCollapsed )
        commFilterTag->SetFlagsFromString( _( "COLLAPSED" ) );
      commFilterTag->Enable( false );
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterTag,
            wxT( "Function" ), SINGLE_COMMTAGFUNCTION,
            arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    arrayStr.Clear();
    vector<TCommTag> tagSel;
    filter->getCommTag( tagSel );
    for( vector<TCommTag>::iterator it = tagSel.begin(); it != tagSel.end(); it++ )
      arrayStr.Add( wxString() << (*it) );

    wxPGId commFilterTagValues = (wxPGId)nullptr;
    commFilterTagValues = AppendCFG4DprvNumbersListPropertyWindow(
            windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterTag,
            wxT( "Tag" ), SINGLE_COMMTAGVALUES,
            arrayStr );

    if ( commFilterTagValues != (wxPGId)nullptr )
    {
      if( filter->getCommTagFunction() == "All" || filter->getCommTagFunction() == "None" )
        commFilterTagValues->Enable( false );
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterCat,
            wxT("Tag/Size Op"), SINGLE_COMMTAGSIZEOP,
            arrayStr, arrayInt, selected );

    // Comm Size
    wxPGId commFilterSize = (wxPGId)nullptr; 
    if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
    {
      commFilterSize = windowProperties->AppendIn( commFilterCat,
                                                   new wxStringProperty( wxT("Comm size"), wxT("Comm size"),wxT("<composed>")));
      if( commFilterSizeCollapsed )
        commFilterSize->SetFlagsFromString( _( "COLLAPSED" ) );
      commFilterSize->Enable( false );
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterSize,
            wxT("Function"), SINGLE_COMMSIZEFUNCTION,
            arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    arrayStr.Clear();
    vector<TCommSize> sizeSel;
    filter->getCommSize( sizeSel );
    for( vector<TCommSize>::iterator it = sizeSel.begin(); it != sizeSel.end(); it++ )
      arrayStr.Add( wxString() << (*it) );

    wxPGId commFilterSizeValues = (wxPGId)nullptr;
    commFilterSizeValues = AppendCFG4DprvNumbersListPropertyWindow(
            windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterSize,
            wxT("Size"), SINGLE_COMMSIZEVALUES,
            arrayStr );

    if ( commFilterSizeValues != (wxPGId)nullptr )
    {
      if( filter->getCommSizeFunction() == "All" || filter->getCommSizeFunction() == "None" )
      {
        commFilterSizeValues->Enable( false );
      }
    }

    // Comm BandWidth
    wxPGId commFilterBW = (wxPGId)nullptr;
    if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
    {
      commFilterBW = windowProperties->AppendIn(  commFilterCat, 
                                                  new wxStringProperty( wxT("Comm bandwidth"), 
                                                                        wxT("Comm bandwidth"),
                                                                        wxT("<composed>") ) );
      if( commFilterBWCollapsed )
        commFilterBW->SetFlagsFromString( _( "COLLAPSED" ) );
      commFilterBW->Enable( false );
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterBW,
            wxT("Function"), SINGLE_COMMBANDWIDTHFUNCTION,
            arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    arrayStr.Clear();
    vector<TSemanticValue> bwSel;
    filter->getBandWidth( bwSel );
    for( vector<TSemanticValue>::iterator it = bwSel.begin(); it != bwSel.end(); it++ )
      arrayStr.Add( wxString() << (*it) );

    wxPGId commFilterBandWidthValues = (wxPGId)nullptr;
    commFilterBandWidthValues = AppendCFG4DprvNumbersListPropertyWindow(
            windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterBW,
            wxT("Bandwidth"), SINGLE_COMMBANDWIDTHVALUES,
            arrayStr );

    if ( commFilterBandWidthValues != (wxPGId)nullptr )
    {
      if( filter->getBandWidthFunction() == "All" || filter->getBandWidthFunction() == "None" )
      {
        commFilterBandWidthValues->Enable( false );
      }
    }

    // -------------------------------- EVENT FILTER -------------------------------
    wxPGId eventFilterCat = (wxPGId)nullptr;
    if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
    {
      eventFilterCat = windowProperties->AppendIn( filterCat, new wxPropertyCategory( wxT("Events"), wxT("Events") ) );
      if( eventFilterCatCollapsed )
        eventFilterCat->SetFlagsFromString( _( "COLLAPSED" ) );
    }

    // Event Type
    wxPGId eventFilterType = (wxPGId)nullptr;
    if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
    {
      eventFilterType = windowProperties->AppendIn( eventFilterCat, 
                                                    new wxStringProperty( wxT("Event type"), 
                                                                          wxT("Event type"),
                                                                          wxT("<composed>") ) );
      if( eventFilterTypeCollapsed )
        eventFilterType->SetFlagsFromString( _( "COLLAPSED" ) );
      eventFilterType->Enable( false );
    }

    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin(); it != filterFunctions.end(); ++it )
    {
      if( (*it) == filter->getEventTypeFunction() )
        selected = pos;
      pos++;
    }

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, eventFilterType,
            wxT("Function"), SINGLE_EVENTTYPEFUNCTION,
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
      arrayStr.Add( wxString() << (*it) << _( " " ) << wxString::FromUTF8( tmpstr.c_str() ) );
    }

    wxPGChoices typeChoices( arrayStr, arrayInt );

    wxPGId eventFilterTypeValues = (wxPGId)nullptr;
    eventFilterTypeValues = AppendCFG4DprvEventInfoPropertyWindow(
            windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, eventFilterType,
            wxT("Types"), SINGLE_EVENTTYPEVALUES,
            typeChoices );

    if ( eventFilterTypeValues != (wxPGId)nullptr )
    {
      windowProperties->SetPropertyAttribute( eventFilterTypeValues, wxPG_ATTR_MULTICHOICE_USERSTRINGMODE, 1 );

      if( filter->getEventTypeFunction() == "All" ||
          filter->getEventTypeFunction() == "None" )
        eventFilterTypeValues->Enable( false );
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, eventFilterCat,
            wxT("Type/Value Op"), SINGLE_EVENTTYPEVALUESOP,
            arrayStr, arrayInt, selected );

    // Event Value
    wxPGId eventFilterValue = (wxPGId)nullptr;
    if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
    {
      eventFilterValue = windowProperties->AppendIn( eventFilterCat, 
                                                     new wxStringProperty( wxT("Event value"), 
                                                                           wxT("Event value"),
                                                                           wxT("<composed>") ) );
      if( eventFilterValueCollapsed )
        eventFilterValue->SetFlagsFromString( _( "COLLAPSED" ) );
      eventFilterValue->Enable( false );
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, eventFilterValue,
            wxT("Function"),  SINGLE_EVENTVALUEFUNCTION,
            arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    wxPGId eventFilterValueValues = (wxPGId)nullptr;
    eventFilterValueValues = AppendCFG4DprvEventInfoPropertyWindow(
            windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, eventFilterValue,
            wxT("Values"), SINGLE_EVENTVALUEVALUES,
            typeChoices );

    if ( eventFilterValueValues != (wxPGId) nullptr )
    {
      windowProperties->SetPropertyAttribute( eventFilterValueValues, wxPG_ATTR_MULTICHOICE_USERSTRINGMODE, 1 );

      if( filter->getEventValueFunction() == "All" || filter->getEventValueFunction() == "None" )
      {
        eventFilterValueValues->Enable( false );
      }
    }
  }

  // END of Filter related properties

  //-------------------------------------------------------------------------
  // Semantic related properties
  //-------------------------------------------------------------------------
  TParamValue pValues;

  wxPGId semanticCat = (wxPGId)nullptr;
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
    arrayComposeFunctions.Add( wxString::FromUTF8( (*it).c_str() ) );
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
    PropertyClientData *tmpData = new PropertyClientData();
    tmpData->extraTopComposeLevel = whichWindow->getExtraNumPositions( TOPCOMPOSE1 ) - nExtraCompose + 1;
    
    if( nExtraCompose == whichWindow->getExtraNumPositions( TOPCOMPOSE1 ) )
      tmpButtonType = BOTH_BUTTONS;
    else
      tmpButtonType = NO_BUTTON;
    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
              tmpName, whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_EXTRATOPCOMPOSE1 : SINGLE_EXTRATOPCOMPOSE1,
              arrayComposeFunctions, arrayComposeFunctionsPos, selected, tmpButtonType, tmpData );

    semanticExtraComposeFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown,
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
  AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
            wxT("Top Compose 1"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_TOPCOMPOSE1 : SINGLE_TOPCOMPOSE1,
            arrayComposeFunctions, arrayComposeFunctionsPos, selected, tmpButtonType );

  semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, TOPCOMPOSE1 );

  pos = 0;
  selected = -1;
  for( vector<string>::iterator it = composeFunctions.begin();
       it != composeFunctions.end(); ++it )
  {
    if( (*it) == whichWindow->getLevelFunction( TOPCOMPOSE2 ) )
      selected = pos;
    ++pos;
  }

  AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
            wxT("Top Compose 2"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_TOPCOMPOSE2 : SINGLE_TOPCOMPOSE2,
            arrayComposeFunctions, arrayComposeFunctionsPos, selected );

  semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, TOPCOMPOSE2 );

  vector<string> notThreadFunctions;
  whichWindow->getAllSemanticFunctions( NOTTHREAD_GROUP, notThreadFunctions );
  arrayNotThreadFunctions.Clear();
  pos = 0;
  for( vector<string>::iterator it = notThreadFunctions.begin();
       it != notThreadFunctions.end(); ++it )
  {
    arrayNotThreadFunctions.Add( wxString::FromUTF8( (*it).c_str() ) );
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

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                wxT("Compose Workload"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_COMPOSEWORKLOAD : SINGLE_COMPOSEWORKLOAD,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, COMPOSEWORKLOAD );

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

        AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                  wxT("Workload"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_WORKLOAD : SINGLE_WORKLOAD,
                  arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected );
        semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, WORKLOAD );
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

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                wxT("Compose Appl"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_COMPOSEAPPL : SINGLE_COMPOSEAPPL,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, COMPOSEAPPLICATION );

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

        AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                  wxT("Application"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_APPLICATION : SINGLE_APPLICATION,
                  arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected );
        semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, APPLICATION );
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

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                wxT("Compose Task"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_COMPOSETASK : SINGLE_COMPOSETASK,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, COMPOSETASK );

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

        AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                  wxT("Task"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_TASK : SINGLE_TASK,
                  arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected );
        semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, TASK );
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

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                wxT("Compose Thread"), (TSingleTimelineProperties)DERIVED_COMPOSETHREAD,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, COMPOSETHREAD );
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

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                wxT("Compose System"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_COMPOSESYSTEM : SINGLE_COMPOSESYSTEM,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, COMPOSESYSTEM );

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

        AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                  wxT("System"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_SYSTEM : SINGLE_SYSTEM,
                  arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected );
        semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, SYSTEM );
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

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                wxT("Compose Node"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_COMPOSENODE : SINGLE_COMPOSENODE,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, COMPOSENODE );

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

        AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                  wxT("Node"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_NODE : SINGLE_NODE,
                  arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected );
        semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, NODE );
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

      AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                wxT("Compose CPU"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_COMPOSECPU : SINGLE_COMPOSECPU,
                arrayComposeFunctions, arrayComposeFunctionsPos, selected );

      semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, COMPOSECPU );
      
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
          arrayStr.Add( wxString::FromUTF8( (*it).c_str() ) );
          arrayInt.Add( pos );
          if( (*it) == whichWindow->getLevelFunction( CPU ) )
            selected = pos;
          ++pos;
        }

        AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                  wxT("CPU"), SINGLE_CPU,
                  arrayStr, arrayInt, selected );

        semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, CPU );
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

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
              wxT("Compose Thread"), SINGLE_COMPOSETHREAD,
              arrayComposeFunctions, arrayComposeFunctionsPos, selected );
    semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, COMPOSETHREAD );

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

    wxString auxDefaultValue = wxString::FromUTF8( whichWindow->getLevelFunction( THREAD ).c_str() );
    AppendCFG4DprvSemanticThreadPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
              wxT("Thread"), SINGLE_THREAD,
              levels, threadFunctions, auxDefaultValue );
    semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, THREAD );
  }
  
  if( whichWindow->isDerivedWindow() )
  {
    wxPGProperty *tmpShiftProperty = nullptr;
    wxIntegerValidator<PRV_INT16> shiftValidator;
    shiftValidator.SetRange( -10, 10 );

    tmpShiftProperty = AppendCFG4DIntegerPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
                                                         DerivedTimelinePropertyLabels[ DERIVED_SHIFT1 ], (TSingleTimelineProperties)DERIVED_SHIFT1,
                                                         whichWindow->getShift( 0 ) );
    if( tmpShiftProperty != nullptr )
      tmpShiftProperty->SetValidator( shiftValidator );

    AppendCFG4DFloatPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
            wxT("Factor #1"), (TSingleTimelineProperties)DERIVED_FACTOR1,
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
      arrayStr.Add( wxString::FromUTF8( (*it).c_str() ) );
      arrayInt.Add( pos );
      if( (*it) == whichWindow->getLevelFunction( DERIVED ) )
        selected = pos;
      ++pos;
    }

    AppendCFG4DEnumPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
              wxT("Derived"), (TSingleTimelineProperties)DERIVED_DERIVED,
              arrayStr, arrayInt, selected );
    semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, DERIVED );

    AppendCFG4DFloatPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
            wxT("Factor #2"), (TSingleTimelineProperties)DERIVED_FACTOR2,
            whichWindow->getFactor( 1 ) );

    tmpShiftProperty = AppendCFG4DIntegerPropertyWindow( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
                                                         DerivedTimelinePropertyLabels[ DERIVED_SHIFT2 ], (TSingleTimelineProperties)DERIVED_SHIFT2,
                                                         whichWindow->getShift( 1 ) );
    if( tmpShiftProperty != nullptr )
      tmpShiftProperty->SetValidator( shiftValidator );
  }
  // END of Semantic related properties
  
  if ( whichWindow->getCFG4DEnabled() && whichWindow->getCFG4DMode() && whichWindow->isDerivedWindow() )
  {
    updateTimelinePropertiesRecursive( windowProperties,
                                       whichWindow->getParent( 0 ),
                                       whichPropertiesClientData,
                                       linkedPropertiesShown );
    updateTimelinePropertiesRecursive( windowProperties,
                                       whichWindow->getParent( 1 ),
                                       whichPropertiesClientData,
                                       linkedPropertiesShown );
  }
}


void updateTimelineProperties( wxPropertyGrid* windowProperties,
                               Timeline *whichWindow,
                               std::vector< PropertyClientData * >& whichPropertiesClientData )
{
  set< CFGS4DLinkedPropertyShown > linkedPropertiesShown;
  
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
    fillPropertyClientData( whichWindow, nullptr, tmpSelector, "Mode", whichPropertiesClientData );
  }

  updateTimelinePropertiesRecursive( windowProperties,
                                     whichWindow,
                                     whichPropertiesClientData,
                                     linkedPropertiesShown );

  windowProperties->SetPropertyAttributeAll( wxPG_BOOL_USE_CHECKBOX, true );
  windowProperties->Refresh();
  windowProperties->Thaw();
}


void updateHistogramProperties( wxPropertyGrid* windowProperties,
                                Histogram *whichHisto,
                                std::vector< PropertyClientData * >& whichPropertiesClientData )
{
  PRV_UINT32 precision = ParaverConfig::getInstance()->getTimelinePrecision();
  set< CFGS4DLinkedPropertyShown > linkedPropertiesShown;
  
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
    fillPropertyClientData( nullptr, whichHisto, tmpSelector, "Mode", whichPropertiesClientData );
  }

  wxPGId dummyPGId = (wxPGId)nullptr; // used to append always to windowProperties

  AppendCFG4DStringPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
          wxT("Name"), HISTOGRAM_NAME, whichHisto->getName() );

  AppendCFG4DStringPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
          wxT("Begin time"), HISTOGRAM_BEGINTIME,
          LabelConstructor::timeLabel(
                  whichHisto->getControlWindow()->traceUnitsToWindowUnits( whichHisto->getBeginTime() ),
                  whichHisto->getControlWindow()->getTimeUnit(),
                  precision ));

  AppendCFG4DStringPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
          wxT("End time"), HISTOGRAM_ENDTIME,
          LabelConstructor::timeLabel(
                  whichHisto->getControlWindow()->traceUnitsToWindowUnits( whichHisto->getEndTime() ),
                  whichHisto->getControlWindow()->getTimeUnit(),
                  precision ));

  // Control Window related properties
  wxPGId ctrlCat = (wxPGId)nullptr;
  if ( !whichHisto->getCFG4DEnabled() || !whichHisto->getCFG4DMode() )
  {
    ctrlCat = windowProperties->Append( new wxPropertyCategory( wxT("Control"), wxT("Control") ) );
    if( ctrlCatCollapsed )
      ctrlCat->SetFlagsFromString( _( "COLLAPSED" ) );
  }

  vector<TWindowID> validWin;
  Timeline *dataWindow = ( whichHisto->getDataWindow() == nullptr ) ? whichHisto->getControlWindow() :
                                                                 whichHisto->getDataWindow();
  LoadedWindows::getInstance()->getValidControlWindow( dataWindow, whichHisto->getExtraControlWindow(), validWin );

  AppendCFG4DTimelineTreePropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, ctrlCat,
          wxT("Window"), HISTOGRAM_CONTROLWINDOW, validWin, whichHisto->getControlWindow() );

  AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, ctrlCat,
          wxT("Minimum"), HISTOGRAM_CONTROLMINIMUM, whichHisto->getControlMin() );
  AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, ctrlCat,
          wxT("Maximum"), HISTOGRAM_CONTROLMAXIMUM, whichHisto->getControlMax() );
  AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, ctrlCat,
          wxT("Delta"), HISTOGRAM_CONTROLDELTA, whichHisto->getControlDelta() );

  // Statistic related properties
  wxPGId statCat = (wxPGId)nullptr;
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
    arrayStr.Add( wxString::FromUTF8( (*it).c_str() ) );
  arrayInt.Add( 0 );
  arrayInt.Add( 1 );
  if( whichHisto->isCommunicationStat( whichHisto->getCurrentStat() ) )
    selected = 0;
  else
    selected = 1;

  AppendCFG4DEnumPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, statCat,
          wxT("Type"), HISTOGRAM_TYPE, arrayStr, arrayInt, selected );

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
    arrayStr.Add( wxString::FromUTF8( (*it).c_str() ) );
    arrayInt.Add( pos );
    if( (*it) == selectedStat )
      selected = pos;
    pos++;
  }

  if( selected == -1 ) selected = 0;

  AppendCFG4DEnumPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, statCat,
          wxT("Statistic"), HISTOGRAM_STATISTIC, arrayStr, arrayInt, selected );

  AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, statCat,
          wxT("Minimum Gradient"), HISTOGRAM_MINIMUMGRADIENT, whichHisto->getMinGradient() );

  AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, statCat,
          wxT("Maximum Gradient"), HISTOGRAM_MAXIMUMGRADIENT, whichHisto->getMaxGradient() );

  // Data Window related properties
  wxPGId dataCat = (wxPGId)nullptr;
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

  AppendCFG4DTimelineTreePropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, dataCat,
           wxT("Window"), HISTOGRAM_DATAWINDOW, validWin, whichHisto->getDataWindow() );

  // 3rd window related properties
  wxPGId thirdWinCat = (wxPGId)nullptr;
  if ( !whichHisto->getCFG4DEnabled() || !whichHisto->getCFG4DMode() )
  {
    thirdWinCat = windowProperties->Append( new wxPropertyCategory( wxT("3D"), wxT("3D") ) );
    if( thirdWinCatCollapsed )
      thirdWinCat->SetFlagsFromString( _( "COLLAPSED" ) );
  }

  validWin.clear();
  dataWindow = ( whichHisto->getDataWindow() == nullptr ) ? whichHisto->getControlWindow() :
                                                         whichHisto->getDataWindow();
  LoadedWindows::getInstance()->getValidControlWindow( dataWindow, whichHisto->getControlWindow(), validWin );

  AppendCFG4DTimelineTreePropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, thirdWinCat,
           wxT("3rd Window"), HISTOGRAM_3D3RDWINDOW, validWin, whichHisto->getExtraControlWindow(), true );

  wxPGId thirdWinMinimum = AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, thirdWinCat,
          wxT("Minimum"), HISTOGRAM_3DMINIMUM, whichHisto->getExtraControlMin() );

  wxPGId thirdWinMaximum = AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, thirdWinCat,
          wxT("Maximum"), HISTOGRAM_3DMAXIMUM, whichHisto->getExtraControlMax() );

  wxPGId thirdWinDelta = AppendCFG4DFloatPropertyHistogram( windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, thirdWinCat,
          wxT("Delta"), HISTOGRAM_3DDELTA, whichHisto->getExtraControlDelta() );

  arrayStr.Clear();
  arrayInt.Clear();
  pos = 0;
  selected = -1;
  bool tmpIsCommStat = whichHisto->isCommunicationStat( whichHisto->getCurrentStat() );

  for( THistogramColumn i = 0; i < whichHisto->getNumPlanes(); ++i )
  {
    bool tmpPlaneWithValues;
    if( tmpIsCommStat )
      tmpPlaneWithValues = whichHisto->planeCommWithValues( i );
    else
      tmpPlaneWithValues = whichHisto->planeWithValues( i );

    if( tmpPlaneWithValues )
    {
      arrayStr.Add( wxString::FromUTF8( whichHisto->getPlaneLabel( i ).c_str() ) );
      arrayInt.Add( pos );
      if( (  tmpIsCommStat && pos == whichHisto->getCommSelectedPlane() ) ||
          ( !tmpIsCommStat && pos == whichHisto->getSelectedPlane() ) )
        selected = pos;
    }
    ++pos;
  }

  wxPGId thirdWinPlane = AppendCFG4DEnumPropertyHistogram( windowProperties,
                                                           whichHisto,
                                                           whichPropertiesClientData,
                                                           linkedPropertiesShown,
                                                           thirdWinCat,
                                                           wxT("Plane"),
                                                           HISTOGRAM_3DPLANE,
                                                           arrayStr,
                                                           arrayInt,
                                                           selected );

  if( !whichHisto->getThreeDimensions() )
  {
    if ( thirdWinMinimum != nullptr )
      thirdWinMinimum->Enable( false );

    if ( thirdWinMaximum != nullptr )
      thirdWinMaximum->Enable( false );

    if ( thirdWinDelta != nullptr )
      thirdWinDelta->Enable( false );

    if ( thirdWinPlane != nullptr )
      thirdWinPlane->Enable( false );
  }

  if ( whichHisto->getCFG4DEnabled() && whichHisto->getCFG4DMode() )
  {
    updateTimelinePropertiesRecursive( windowProperties,
                                       whichHisto->getControlWindow(),
                                       whichPropertiesClientData,
                                       linkedPropertiesShown );
    if( whichHisto->getDataWindow() != nullptr &&
        whichHisto->getDataWindow() != whichHisto->getControlWindow() )
    {
      updateTimelinePropertiesRecursive( windowProperties,
                                         whichHisto->getDataWindow(),
                                         whichPropertiesClientData,
                                         linkedPropertiesShown );
    }

    if( whichHisto->getThreeDimensions() &&
        whichHisto->getExtraControlWindow() != whichHisto->getControlWindow() &&
        whichHisto->getExtraControlWindow() != whichHisto->getDataWindow() )
    {
      updateTimelinePropertiesRecursive( windowProperties,
                                         whichHisto->getExtraControlWindow(),
                                         whichPropertiesClientData,
                                         linkedPropertiesShown );
    }
  }

  windowProperties->SetPropertyAttributeAll( wxPG_BOOL_USE_CHECKBOX, true );
  windowProperties->Refresh();
  windowProperties->Thaw();
}
