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


template< class W >
constexpr void fillPropertyClientDataOwner( W *whichWindow, PropertyClientData *whichClientData ) = delete;

template<>
constexpr void fillPropertyClientDataOwner<Timeline>( Timeline *whichWindow, PropertyClientData *whichClientData )
{
  whichClientData->ownerTimeline = whichWindow;
  whichClientData->ownerHistogram = nullptr;
}

template<>
constexpr void fillPropertyClientDataOwner<Histogram>( Histogram *whichWindow, PropertyClientData *whichClientData )
{
  whichClientData->ownerTimeline = nullptr;
  whichClientData->ownerHistogram = whichWindow;
}

template< class W >
constexpr void fillPropertyClientData( W *whichWindow,
                                       wxPGProperty *whichProperty,
                                       const std::string& widgetName,
                                       std::vector< PropertyClientData * >& whichPropertiesClientData,
                                       PropertyClientData *whichClientData = nullptr )
{
  if ( whichProperty != nullptr )
  {
    PropertyClientData *tmpClientData = whichClientData;
    if( tmpClientData == nullptr )
      tmpClientData = new PropertyClientData();
    fillPropertyClientDataOwner( whichWindow, tmpClientData );
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


template<class T, typename U>
inline std::string getWidgetNameFromPropertyIndex( T *window, U propertyIndex ) = delete;

template<>
inline std::string getWidgetNameFromPropertyIndex<Timeline, TSingleTimelineProperties>( Timeline *window, TSingleTimelineProperties propertyIndex )
{
  if ( window->isDerivedWindow() ) 
    return DerivedTimelinePropertyLabels[ (TDerivedTimelineProperties)propertyIndex ];

  return SingleTimelinePropertyLabels[ propertyIndex ];
}

template<>
inline std::string getWidgetNameFromPropertyIndex<Histogram, THistogramProperties>( Histogram *window, THistogramProperties propertyIndex )
{
  return HistogramPropertyLabels[ propertyIndex ];
}


template< class W, typename P >
inline bool insertLinkedPropertyShown( W* whichWindow,
                                       P propertyIndex,
                                       set< CFGS4DLinkedPropertyShown >& linkedPropertiesShown,
                                       std::string propertyName = "" )
{
  CFGS4DLinkedPropertyShown tmpLinked;

  if ( propertyName != "" )
    tmpLinked.propertyName = propertyName;
  else
    tmpLinked.propertyName = getWidgetNameFromPropertyIndex( whichWindow, propertyIndex );
  
  tmpLinked.groupLink = whichWindow->getCFGS4DGroupLink( tmpLinked.propertyName );

  if( tmpLinked.groupLink == NO_GROUP_LINK )
    return true;

  return linkedPropertiesShown.insert( tmpLinked ).second;
}


template<class PropertyType, typename P >
struct buildProperty
{
  template<class W, typename... TArgs >
  PropertyType *operator()( wxString widgetLabel, wxString widgetName, W *window, P propertyIndex, TArgs&&... argsValues )
  {
    return new PropertyType( widgetLabel, widgetName, std::forward<TArgs>( argsValues )... );
  }
};

template<typename P>
struct buildProperty<prvRowsSelectionProperty, P>
{
  template<class W, typename... TArgs >
  prvRowsSelectionProperty *operator()( wxString widgetLabel, wxString widgetName, W *window, P propertyIndex, TArgs&&... argsValues )
  {
    return new prvRowsSelectionProperty( widgetLabel, widgetName, window, std::forward<TArgs>( argsValues )... );
  }
};

template<typename P>
struct buildProperty<prvEventInfoProperty, P>
{
  template<class W, typename... TArgs >
  prvEventInfoProperty *operator()( wxString widgetLabel, wxString widgetName, W *window, P propertyIndex, TArgs&&... argsValues )
  {
    prvEventInfoType infoType;
    if ( propertyIndex == SINGLE_EVENTTYPEVALUES )
      infoType = prvEventInfoType::TYPES;
    else if ( propertyIndex == SINGLE_EVENTVALUEVALUES )
      infoType = prvEventInfoType::VALUES;

    return new prvEventInfoProperty( widgetLabel, widgetName, window, infoType, std::forward<TArgs>( argsValues )... );
  }
};


struct CFG4DPropertyCustomOptions
{
  ButtonType addButton = NO_BUTTON;
  PropertyClientData *clientData = nullptr;
};

template< class PropertyType, class W, typename P, typename... TArgs >
wxPGId AppendCFG4DProperty( PropertyType *dummyObject, // dummyObject only for type deduction purposes
                            wxPropertyGrid* windowProperties,
                            W* whichWindow,
                            std::vector< PropertyClientData * >& whichPropertiesClientData,
                            set< CFGS4DLinkedPropertyShown >& linkedPropertiesShown,
                            wxPGId fatherWidget,
                            const wxString &widgetLabel,
                            P propertyIndex,
                            const CFG4DPropertyCustomOptions& propertyOptions,
                            TArgs&&... argsValues )
{
  wxPGId retId = (wxPGId)nullptr;
  PropertyType *auxProperty = nullptr;

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;
  std::string widgetName = getWidgetNameFromPropertyIndex( whichWindow, propertyIndex );

  if ( paraverMain::myParaverMain->isCFG4DModeDisabled() )
  {
    // NORMAL mode
    auxProperty = buildProperty<PropertyType, P>()( widgetLabel, tmpWidgetName, whichWindow, propertyIndex, std::forward<TArgs>( argsValues )... );

    if ( fatherWidget )
    {
      retId = windowProperties->AppendIn( fatherWidget, auxProperty );
    }
    else
    {
      retId = windowProperties->Append( auxProperty );
    }

    switch( propertyOptions.addButton )
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
  else if ( whichWindow->existsCFG4DAlias( widgetName ) &&
            insertLinkedPropertyShown( whichWindow, propertyIndex, linkedPropertiesShown ) )
  {
    wxString auxTag = wxString::FromUTF8( whichWindow->getCFG4DAlias( widgetName ).c_str() );

    auxProperty = buildProperty<PropertyType, P>()( auxTag, tmpWidgetName, whichWindow, propertyIndex, std::forward<TArgs>( argsValues )... );

    retId = windowProperties->Append( auxProperty );
  }
  
  fillPropertyClientData( whichWindow, auxProperty, widgetName, whichPropertiesClientData, propertyOptions.clientData );

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
  std::string widgetName = getWidgetNameFromPropertyIndex( whichHisto, propertyIndex );  

  wxString valueStr;
  if( currentWindow == nullptr )
    valueStr = wxT( "None" );
  else
    valueStr = wxString( currentWindow->getName().c_str(), wxConvUTF8 );

  Trace *currentTrace;
  if( currentWindow == nullptr )
    currentTrace = whichHisto->getTrace();
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
  else if ( whichHisto->existsCFG4DAlias( widgetName ) &&
            insertLinkedPropertyShown( whichHisto, propertyIndex, linkedPropertiesShown ) )
  {
    // CFG4D mode
    wxString auxTag = wxString::FromUTF8( whichHisto->getCFG4DAlias( widgetName ).c_str() );

    auxProperty = new prvTimelineTreeProperty( auxTag,
                                               tmpWidgetName,
                                               valueStr,
                                               windowsList,
                                               currentWindow,
                                               currentTrace,
                                               needNoneElement );
    retId = windowProperties->Append( auxProperty );
  }

  fillPropertyClientData( whichHisto, auxProperty, widgetName, whichPropertiesClientData );

  return retId;
}


// NOT EASY-----------------------------------------------------------------------------------
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

  wxString tmpWidgetName;
  tmpWidgetName << propNameCounter++;
  std::string widgetName = getWidgetNameFromPropertyIndex( whichWindow, propertyIndex );  

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
      // Splitted Parameter alias key
      TWindowLevel kSemanticLevel;
      string kFunction;
      TParamIndex kNumParameter;

      whichWindow->splitCFG4DParamAliasKey( *it, kSemanticLevel, kFunction, kNumParameter );

      if ( !insertLinkedPropertyShown( whichWindow,
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
        wxString auxTag = wxString::FromUTF8( whichWindow->getCFG4DParamAlias( *it ).c_str() );

        auxProperty = new prvNumbersListProperty( auxTag, tmpWidgetName, arrayStr );
        retId = windowProperties->Append( auxProperty );
      }
    }
  }

  PropertyClientData *tmpData = new PropertyClientData();
  tmpData->semanticLevel = currentSemanticLevel;
  tmpData->numParameter = currentNumParameter;
  tmpData->extraTopComposeLevel = extraComposePos;

  fillPropertyClientData( whichWindow, auxProperty, widgetName, whichPropertiesClientData, tmpData );

  return retId;
}
// NOT EASY-----------------------------------------------------------------------------------


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

  AppendCFG4DProperty( (wxStringProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown,
                       dummyPGId, wxT("Name"), SINGLE_NAME, CFG4DPropertyCustomOptions(), whichWindow->getName() );

  AppendCFG4DProperty( (wxStringProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown,
                       dummyPGId, wxT("Begin time"), SINGLE_BEGINTIME, CFG4DPropertyCustomOptions(),
                       LabelConstructor::timeLabel( whichWindow->traceUnitsToWindowUnits( whichWindow->getWindowBeginTime() ),
                                                    whichWindow->getTimeUnit(),
                                                    precision ) );

  AppendCFG4DProperty( (wxStringProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown,
                       dummyPGId, wxT("End time"), SINGLE_ENDTIME, CFG4DPropertyCustomOptions(),
                       LabelConstructor::timeLabel( whichWindow->traceUnitsToWindowUnits( whichWindow->getWindowEndTime() ),
                                                    whichWindow->getTimeUnit(),
                                                    precision ) );

  AppendCFG4DProperty( (wxFloatProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
                       wxT("Semantic Minimum"), SINGLE_SEMANTICMINIMUM, CFG4DPropertyCustomOptions(), whichWindow->getMinimumY() );

  AppendCFG4DProperty( (wxFloatProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
                       wxT("Semantic Maximum"), SINGLE_SEMANTICMAXIMUM, CFG4DPropertyCustomOptions(), whichWindow->getMaximumY() );

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

    AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
                         wxT("Level"), SINGLE_LEVEL, CFG4DPropertyCustomOptions(), arrayLevels, arrayLevelsPos, static_cast<int>( whichWindow->getLevel() ) );
  }
  else
  {
    wxArrayString arrayLevels;
    wxArrayInt arrayLevelsPos;
    
    if( whichWindow->getMinAcceptableLevel() >= TTraceLevel::SYSTEM &&
        whichWindow->getMinAcceptableLevel() <= TTraceLevel::CPU )
    {
      if( whichWindow->getMinAcceptableLevel() >= TTraceLevel::SYSTEM )
      {
        arrayLevels.Add( _( "System" ) );
        arrayLevelsPos.Add( static_cast<int>( SYSTEM ) );
      }
      if( whichWindow->getMinAcceptableLevel() >= TTraceLevel::NODE )
      {
        arrayLevels.Add( _( "Node" ) );
        arrayLevelsPos.Add( static_cast<int>( NODE ) );
      }
      if( whichWindow->getMinAcceptableLevel() >= TTraceLevel::CPU )
      {
        arrayLevels.Add( _( "CPU" ) );
        arrayLevelsPos.Add( static_cast<int>( CPU ) );
      }
    }
    else
    {
      if( whichWindow->getMinAcceptableLevel() >= TTraceLevel::WORKLOAD )
      {
        arrayLevels.Add( _( "Workload" ) );
        arrayLevelsPos.Add( static_cast<int>( WORKLOAD ) );
      }
      if( whichWindow->getMinAcceptableLevel() >= TTraceLevel::APPLICATION )
      {
        arrayLevels.Add( _( "Application" ) );
        arrayLevelsPos.Add( static_cast< int >( APPLICATION ) );
      }
      if( whichWindow->getMinAcceptableLevel() >= TTraceLevel::TASK )
      {
        arrayLevels.Add( _( "Task" ) );
        arrayLevelsPos.Add( static_cast< int >( TASK ) );
      }
      if( whichWindow->getMinAcceptableLevel() >= TTraceLevel::THREAD )
      {
        arrayLevels.Add( _( "Thread" ) );
        arrayLevelsPos.Add( static_cast< int >( THREAD ) );
      }
    }

    AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
                         wxT("Level"), (TSingleTimelineProperties)DERIVED_LEVEL, CFG4DPropertyCustomOptions(), arrayLevels, arrayLevelsPos, static_cast<int>( whichWindow->getLevel() ) );
  }

  arrayStr.Clear();
  arrayInt.Clear();
  for( int iUnit = 0; iUnit <= DAY; ++iUnit )
  {
    arrayStr.Add( wxString::FromUTF8( FULL_LABEL_TIMEUNIT[ iUnit ].c_str() ) );
    arrayInt.Add( iUnit );
  }

  AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
                       wxT("Time unit"), SINGLE_TIMEUNIT, CFG4DPropertyCustomOptions(), arrayStr, arrayInt, whichWindow->getTimeUnit() );

  
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

    AppendCFG4DProperty( (wxBoolProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterCat,
                         wxT("Logical"), SINGLE_COMMLOGICAL, CFG4DPropertyCustomOptions(), filter->getLogical() );

    AppendCFG4DProperty( (wxBoolProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown,commFilterCat,
                         wxT("Physical"), SINGLE_COMMPHYSICAL, CFG4DPropertyCustomOptions(), filter->getPhysical() );

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

    AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterFrom,
                         wxT( "Function" ), SINGLE_COMMFROMFUNCTION, CFG4DPropertyCustomOptions(), arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    // add communication senders to arrayStr
    arrayStr.Clear();
    vector<TObjectOrder> fromSel;
    filter->getCommFrom( fromSel );
    wxPGId commFilterFromValues = (wxPGId)nullptr;

    commFilterFromValues = AppendCFG4DProperty( (prvRowsSelectionProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData,
                                                linkedPropertiesShown, commFilterFrom, wxT( "From" ), SINGLE_COMMFROMVALUES, CFG4DPropertyCustomOptions(), _("From - Rows Selection"), fromSel );

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

    AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterCat,
                         wxT("From/To Op"), SINGLE_COMMFROMTOOP, CFG4DPropertyCustomOptions(), arrayStr, arrayInt, selected );

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

    AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterTo,
                         wxT( "Function" ), SINGLE_COMMTOFUNCTION, CFG4DPropertyCustomOptions(), arrayFilterFunctions, arrayFilterFunctionsPos, selected );


    arrayStr.Clear();
    vector<TObjectOrder> toSel;
    filter->getCommTo( toSel );

    wxPGId commFilterToValues = (wxPGId)nullptr;
    commFilterToValues = AppendCFG4DProperty( (prvRowsSelectionProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData,
                                              linkedPropertiesShown, commFilterTo, wxT( "To" ), SINGLE_COMMTOVALUES, CFG4DPropertyCustomOptions(), _("To - Rows Selection"), toSel );

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

    AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterTag,
                         wxT( "Function" ), SINGLE_COMMTAGFUNCTION, CFG4DPropertyCustomOptions(), arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    arrayStr.Clear();
    vector<TCommTag> tagSel;
    filter->getCommTag( tagSel );
    for( vector<TCommTag>::iterator it = tagSel.begin(); it != tagSel.end(); it++ )
      arrayStr.Add( wxString() << (*it) );

    wxPGId commFilterTagValues = (wxPGId)nullptr;
    commFilterTagValues = AppendCFG4DProperty( (prvNumbersListProperty *)nullptr, windowProperties, whichWindow,
                                               whichPropertiesClientData, linkedPropertiesShown, commFilterTag,
                                               wxT( "Tag" ), SINGLE_COMMTAGVALUES, CFG4DPropertyCustomOptions(), arrayStr );

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

    AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterCat,
                         wxT("Tag/Size Op"), SINGLE_COMMTAGSIZEOP, CFG4DPropertyCustomOptions(), arrayStr, arrayInt, selected );

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

    AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterSize,
                         wxT("Function"), SINGLE_COMMSIZEFUNCTION, CFG4DPropertyCustomOptions(), arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    arrayStr.Clear();
    vector<TCommSize> sizeSel;
    filter->getCommSize( sizeSel );
    for( vector<TCommSize>::iterator it = sizeSel.begin(); it != sizeSel.end(); it++ )
      arrayStr.Add( wxString() << (*it) );

    wxPGId commFilterSizeValues = (wxPGId)nullptr;
    commFilterSizeValues = AppendCFG4DProperty( (prvNumbersListProperty *)nullptr, windowProperties, whichWindow,
                                                whichPropertiesClientData, linkedPropertiesShown, commFilterSize,
                                                wxT("Size"), SINGLE_COMMSIZEVALUES, CFG4DPropertyCustomOptions(), arrayStr );

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

    AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, commFilterBW,
                         wxT("Function"), SINGLE_COMMBANDWIDTHFUNCTION, CFG4DPropertyCustomOptions(), arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    arrayStr.Clear();
    vector<TSemanticValue> bwSel;
    filter->getBandWidth( bwSel );
    for( vector<TSemanticValue>::iterator it = bwSel.begin(); it != bwSel.end(); it++ )
      arrayStr.Add( wxString() << (*it) );

    wxPGId commFilterBandWidthValues = (wxPGId)nullptr;
    commFilterBandWidthValues = AppendCFG4DProperty( (prvNumbersListProperty *)nullptr, windowProperties, whichWindow,
                                                     whichPropertiesClientData, linkedPropertiesShown, commFilterBW,
                                                     wxT("Bandwidth"), SINGLE_COMMBANDWIDTHVALUES, CFG4DPropertyCustomOptions(), arrayStr );

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

    AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, eventFilterType,
                         wxT("Function"), SINGLE_EVENTTYPEFUNCTION, CFG4DPropertyCustomOptions(), arrayFilterFunctions, arrayFilterFunctionsPos, selected );

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
    eventFilterTypeValues = AppendCFG4DProperty( (prvEventInfoProperty *)nullptr,
            windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, eventFilterType,
            wxT("Types"), SINGLE_EVENTTYPEVALUES, CFG4DPropertyCustomOptions(),
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

    AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, eventFilterCat,
                         wxT("Type/Value Op"), SINGLE_EVENTTYPEVALUESOP, CFG4DPropertyCustomOptions(), arrayStr, arrayInt, selected );

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

    AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, eventFilterValue,
                         wxT("Function"), SINGLE_EVENTVALUEFUNCTION, CFG4DPropertyCustomOptions(), arrayFilterFunctions, arrayFilterFunctionsPos, selected );

    wxPGId eventFilterValueValues = (wxPGId)nullptr;
    eventFilterValueValues = AppendCFG4DProperty( (prvEventInfoProperty *)nullptr,
            windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, eventFilterValue,
            wxT("Values"), SINGLE_EVENTVALUEVALUES, CFG4DPropertyCustomOptions(),
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
    
    CFG4DPropertyCustomOptions tmpOptions { tmpButtonType, tmpData };
    AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                         tmpName, whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_EXTRATOPCOMPOSE1 : SINGLE_EXTRATOPCOMPOSE1,
                         tmpOptions, arrayComposeFunctions, arrayComposeFunctionsPos, selected );

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
  CFG4DPropertyCustomOptions tmpOptions { tmpButtonType };
  AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                       wxT("Top Compose 1"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_TOPCOMPOSE1 : SINGLE_TOPCOMPOSE1,
                       tmpOptions, arrayComposeFunctions, arrayComposeFunctionsPos, selected );

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

  AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                       wxT("Top Compose 2"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_TOPCOMPOSE2 : SINGLE_TOPCOMPOSE2,
                       CFG4DPropertyCustomOptions(), arrayComposeFunctions, arrayComposeFunctionsPos, selected );

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

  TTraceLevel level = whichWindow->getLevel();
  TTraceLevel minAcceptLevel = whichWindow->getMinAcceptableLevel();
  
  if( level >= TTraceLevel::WORKLOAD && level <= TTraceLevel::THREAD )
  {
    if( level == TTraceLevel::WORKLOAD )
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

      AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                           wxT("Compose Workload"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_COMPOSEWORKLOAD : SINGLE_COMPOSEWORKLOAD,
                           CFG4DPropertyCustomOptions(), arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, COMPOSEWORKLOAD );

      if ( !whichWindow->isDerivedWindow() || minAcceptLevel > TTraceLevel::WORKLOAD )
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

        AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                             wxT("Workload"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_WORKLOAD : SINGLE_WORKLOAD,
                             CFG4DPropertyCustomOptions(), arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected );
        semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, WORKLOAD );
      }
    }
      
    if( level <= TTraceLevel::APPLICATION && minAcceptLevel >= TTraceLevel::APPLICATION )
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

      AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                           wxT("Compose Appl"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_COMPOSEAPPL : SINGLE_COMPOSEAPPL,
                           CFG4DPropertyCustomOptions(), arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, COMPOSEAPPLICATION );

      if ( !whichWindow->isDerivedWindow() || minAcceptLevel > TTraceLevel::APPLICATION )
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

        AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                             wxT("Application"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_APPLICATION : SINGLE_APPLICATION,
                             CFG4DPropertyCustomOptions(), arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected );
        semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, APPLICATION );
      }
    }

    if( level <= TTraceLevel::TASK && minAcceptLevel >= TTraceLevel::TASK )
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

      AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                           wxT("Compose Task"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_COMPOSETASK : SINGLE_COMPOSETASK,
                           CFG4DPropertyCustomOptions(), arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, COMPOSETASK );

      if ( !whichWindow->isDerivedWindow() || minAcceptLevel > TTraceLevel::TASK )
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

        AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                             wxT("Task"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_TASK : SINGLE_TASK,
                             CFG4DPropertyCustomOptions(), arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected );
        semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, TASK );
      }
    }
    
    if( whichWindow->isDerivedWindow() && minAcceptLevel >= TTraceLevel::THREAD )
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

      AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                           wxT("Compose Thread"), (TSingleTimelineProperties)DERIVED_COMPOSETHREAD,
                           CFG4DPropertyCustomOptions(), arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, COMPOSETHREAD );
    }
  }
  else if( level >= TTraceLevel::SYSTEM && level <= TTraceLevel::CPU )
  {
    if( level == TTraceLevel::SYSTEM && minAcceptLevel >= TTraceLevel::SYSTEM )
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

      AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                           wxT("Compose System"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_COMPOSESYSTEM : SINGLE_COMPOSESYSTEM,
                           CFG4DPropertyCustomOptions(), arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, COMPOSESYSTEM );

      if ( !whichWindow->isDerivedWindow() || minAcceptLevel > TTraceLevel::SYSTEM )
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

        AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                             wxT("System"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_SYSTEM : SINGLE_SYSTEM,
                             CFG4DPropertyCustomOptions(), arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected );
        semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, SYSTEM );
      }
    }
      
    if( level <= TTraceLevel::NODE && minAcceptLevel >= TTraceLevel::NODE )
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

      AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                           wxT("Compose Node"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_COMPOSENODE : SINGLE_COMPOSENODE,
                           CFG4DPropertyCustomOptions(), arrayComposeFunctions, arrayComposeFunctionsPos, selected );
      semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, COMPOSENODE );

      if ( !whichWindow->isDerivedWindow() || minAcceptLevel > TTraceLevel::NODE )
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

        AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                             wxT("Node"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_NODE : SINGLE_NODE,
                             CFG4DPropertyCustomOptions(), arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected );
        semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, NODE );
      }
    }
      
    if( level <= TTraceLevel::CPU && minAcceptLevel >= TTraceLevel::CPU )
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

      AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                           wxT("Compose CPU"), whichWindow->isDerivedWindow() ? (TSingleTimelineProperties)DERIVED_COMPOSECPU : SINGLE_COMPOSECPU,
                           CFG4DPropertyCustomOptions(), arrayComposeFunctions, arrayComposeFunctionsPos, selected );

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

        AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                             wxT("CPU"), SINGLE_CPU, CFG4DPropertyCustomOptions(), arrayStr, arrayInt, selected );

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

    AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                         wxT("Compose Thread"), SINGLE_COMPOSETHREAD, CFG4DPropertyCustomOptions(), arrayComposeFunctions, arrayComposeFunctionsPos, selected );
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
    AppendCFG4DProperty( (prvSemanticThreadProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown,
                         semanticCat, wxT("Thread"), SINGLE_THREAD, CFG4DPropertyCustomOptions(), levels, threadFunctions, auxDefaultValue );
    semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, THREAD );
  }
  
  if( whichWindow->isDerivedWindow() )
  {
    wxPGProperty *tmpShiftProperty = nullptr;
    wxIntegerValidator<PRV_INT16> shiftValidator;
    shiftValidator.SetRange( -10, 10 );

    tmpShiftProperty = AppendCFG4DProperty( (wxIntProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
                                            DerivedTimelinePropertyLabels[ DERIVED_SHIFT1 ], (TSingleTimelineProperties)DERIVED_SHIFT1,
                                            CFG4DPropertyCustomOptions(),
                                            whichWindow->getShift( 0 ) );
    if( tmpShiftProperty != nullptr )
      tmpShiftProperty->SetValidator( shiftValidator );

    AppendCFG4DProperty( (wxFloatProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
                         wxT("Factor #1"), (TSingleTimelineProperties)DERIVED_FACTOR1, CFG4DPropertyCustomOptions(), whichWindow->getFactor( 0 ) );

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

    AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat,
                         wxT("Derived"), (TSingleTimelineProperties)DERIVED_DERIVED, CFG4DPropertyCustomOptions(), arrayStr, arrayInt, selected );
    semanticFunctionParameter( windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, semanticCat, DERIVED );

    AppendCFG4DProperty( (wxFloatProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
                         wxT("Factor #2"), (TSingleTimelineProperties)DERIVED_FACTOR2, CFG4DPropertyCustomOptions(), whichWindow->getFactor( 1 ) );

    tmpShiftProperty = AppendCFG4DProperty( (wxIntProperty *)nullptr, windowProperties, whichWindow, whichPropertiesClientData, linkedPropertiesShown, dummyPGId,
                                            DerivedTimelinePropertyLabels[ DERIVED_SHIFT2 ], (TSingleTimelineProperties)DERIVED_SHIFT2,
                                            CFG4DPropertyCustomOptions(),
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
    fillPropertyClientData( whichWindow, tmpSelector, "Mode", whichPropertiesClientData );
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
    fillPropertyClientData( whichHisto, tmpSelector, "Mode", whichPropertiesClientData );
  }

  wxPGId dummyPGId = (wxPGId)nullptr; // used to append always to windowProperties

  AppendCFG4DProperty( (wxStringProperty *)nullptr, windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown,
                       dummyPGId, wxT("Name"), HISTOGRAM_NAME, CFG4DPropertyCustomOptions(), whichHisto->getName() );

  AppendCFG4DProperty( (wxStringProperty *)nullptr, windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown,
                       dummyPGId, wxT("Begin time"), HISTOGRAM_BEGINTIME,
                       CFG4DPropertyCustomOptions(),
                       LabelConstructor::timeLabel( whichHisto->getControlWindow()->traceUnitsToWindowUnits( whichHisto->getBeginTime() ),
                                                    whichHisto->getControlWindow()->getTimeUnit(),
                                                    precision ) );

  AppendCFG4DProperty( (wxStringProperty *)nullptr, windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown,
                       dummyPGId, wxT("End time"), HISTOGRAM_ENDTIME,
                       CFG4DPropertyCustomOptions(),
                       LabelConstructor::timeLabel( whichHisto->getControlWindow()->traceUnitsToWindowUnits( whichHisto->getEndTime() ),
                                                    whichHisto->getControlWindow()->getTimeUnit(),
                                                    precision ) );

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

  AppendCFG4DProperty( (wxFloatProperty *)nullptr, windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, ctrlCat,
                       wxT("Minimum"), HISTOGRAM_CONTROLMINIMUM, CFG4DPropertyCustomOptions(), whichHisto->getControlMin() );
  AppendCFG4DProperty( (wxFloatProperty *)nullptr, windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, ctrlCat,
                       wxT("Maximum"), HISTOGRAM_CONTROLMAXIMUM, CFG4DPropertyCustomOptions(), whichHisto->getControlMax() );
  AppendCFG4DProperty( (wxFloatProperty *)nullptr, windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, ctrlCat,
                       wxT("Delta"), HISTOGRAM_CONTROLDELTA, CFG4DPropertyCustomOptions(), whichHisto->getControlDelta() );

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

  AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, statCat,
                       wxT("Type"), HISTOGRAM_TYPE, CFG4DPropertyCustomOptions(), arrayStr, arrayInt, selected );

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

  AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, statCat,
                       wxT("Statistic"), HISTOGRAM_STATISTIC, CFG4DPropertyCustomOptions(), arrayStr, arrayInt, selected );

  AppendCFG4DProperty( (wxFloatProperty *)nullptr, windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, statCat,
                       wxT("Minimum Gradient"), HISTOGRAM_MINIMUMGRADIENT, CFG4DPropertyCustomOptions(), whichHisto->getMinGradient() );

  AppendCFG4DProperty( (wxFloatProperty *)nullptr, windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, statCat,
                       wxT("Maximum Gradient"), HISTOGRAM_MAXIMUMGRADIENT, CFG4DPropertyCustomOptions(), whichHisto->getMaxGradient() );

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

  wxPGId thirdWinMinimum = AppendCFG4DProperty( (wxFloatProperty *)nullptr, windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, thirdWinCat,
                                                wxT("Minimum"), HISTOGRAM_3DMINIMUM, CFG4DPropertyCustomOptions(), whichHisto->getExtraControlMin() );

  wxPGId thirdWinMaximum = AppendCFG4DProperty( (wxFloatProperty *)nullptr, windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, thirdWinCat,
                                                wxT("Maximum"), HISTOGRAM_3DMAXIMUM, CFG4DPropertyCustomOptions(), whichHisto->getExtraControlMax() );

  wxPGId thirdWinDelta   = AppendCFG4DProperty( (wxFloatProperty *)nullptr, windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown, thirdWinCat,
                                                wxT("Delta"), HISTOGRAM_3DDELTA, CFG4DPropertyCustomOptions(), whichHisto->getExtraControlDelta() );

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

  wxPGId thirdWinPlane = AppendCFG4DProperty( (wxEnumProperty *)nullptr, windowProperties, whichHisto, whichPropertiesClientData, linkedPropertiesShown,
                                              thirdWinCat, wxT("Plane"), HISTOGRAM_3DPLANE, CFG4DPropertyCustomOptions(), arrayStr, arrayInt, selected );

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
