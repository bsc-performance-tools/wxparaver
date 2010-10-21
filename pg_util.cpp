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
//#include "wx/propgrid/propgrid.h"
#include "pg_extraprop.h"
#include "trace.h"
#include "window.h"
#include "histogram.h"
#include "filter.h"
#include "loadedwindows.h"
#include "labelconstructor.h"
#include "paraverlabels.h"

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

static bool statCatCollapsed      = true;
static bool ctrlCatCollapsed      = true;
static bool dataCatCollapsed      = true;
static bool thirdWinCatCollapsed = true;

inline void updateStateOf( wxPropertyGrid *windowProperties, bool& categoryStat, const wxString& catName )
{
  wxPGProperty *tmpProp = windowProperties->GetPropertyByLabel( catName );
  if( tmpProp != NULL )
    categoryStat = tmpProp->GetFlagsAsString( wxPG_PROP_COLLAPSED ) == _( "COLLAPSED" );
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
    wxArrayStringProperty *parameterProp = new wxArrayStringProperty( 
                                              wxString( _("   ") ) + wxString::FromAscii( whichWindow->getFunctionParamName( functionLevel, paramIdx ).c_str() ), 
                                              propName, 
                                              valuesStr );
    windowProperties->AppendIn( category, parameterProp );
  }
}
                                
void updateTimelineProperties( wxPropertyGrid* windowProperties, Window *whichWindow )
{
  UINT32 precision = ParaverConfig::getInstance()->getTimelinePrecision();

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
  
  windowProperties->Append( new wxStringProperty( wxT("Name"), wxPG_LABEL, wxString::FromAscii( whichWindow->getName().c_str() ) ) );
  windowProperties->Append( new wxStringProperty( wxT("Begin time"), wxPG_LABEL, 
                            wxString::FromAscii( LabelConstructor::timeLabel( whichWindow->traceUnitsToWindowUnits( whichWindow->getWindowBeginTime() ),
                                                                              whichWindow->getTimeUnit(),
                                                                              precision ).c_str() ) ) );
  windowProperties->Append( new wxStringProperty( wxT("End time"), wxPG_LABEL, 
                            wxString::FromAscii( LabelConstructor::timeLabel( whichWindow->traceUnitsToWindowUnits( whichWindow->getWindowEndTime() ),
                                                                              whichWindow->getTimeUnit(),
                                                                              precision ).c_str() ) ) );

  windowProperties->Append( new wxFloatProperty( wxT("Semantic Minimum"), wxPG_LABEL,
                                                 whichWindow->getMinimumY() ) );
  windowProperties->Append( new wxFloatProperty( wxT("Semantic Maximum"), wxPG_LABEL,
                                                 whichWindow->getMaximumY() ) );
                            
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
    
    windowProperties->Append( new wxEnumProperty( wxT("Level"), wxPG_LABEL,
                                                  arrayLevels, arrayLevelsPos, whichWindow->getLevel() ) );
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
    
    windowProperties->Append( new wxEnumProperty( wxT("Level"), wxPG_LABEL,
                                                  arrayLevels, arrayLevelsPos, whichWindow->getLevel() ) );
  }
  
  arrayStr.Clear();
  arrayInt.Clear();
  for( int iUnit = 0; iUnit <= DAY; ++iUnit )
  {
    arrayStr.Add( wxString::FromAscii( FULL_LABEL_TIMEUNIT[ iUnit ].c_str() ) );
    arrayInt.Add( iUnit );
  }
  windowProperties->Append( new wxEnumProperty( wxT( "Time Unit" ), wxPG_LABEL, arrayStr, arrayInt,
                                                whichWindow->getTimeUnit() ) );

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

    wxPGId filterCat = windowProperties->Append( new wxPropertyCategory( wxT("Filter") ) );
    if( filterCatCollapsed )
      filterCat->SetFlagsFromString( _( "COLLAPSED" ) );
    
    // ---------------------------- COMMUNICATION FILTER ---------------------------
    wxPGId commFilterCat = windowProperties->AppendIn( filterCat, new wxPropertyCategory( wxT("Communications") ) );
    if( commFilterCatCollapsed )
      commFilterCat->SetFlagsFromString( _( "COLLAPSED" ) );
    
    windowProperties->AppendIn( commFilterCat, 
        new wxBoolProperty( wxT("Logical"), wxPG_LABEL, filter->getLogical() ) );
    windowProperties->AppendIn( commFilterCat, 
        new wxBoolProperty( wxT("Physical"), wxPG_LABEL, filter->getPhysical() ) );

    // Comm From
    wxPGId commFilterFrom = windowProperties->AppendIn( commFilterCat, 
                                                        new wxStringProperty( wxT("Comm from"),
                                                                              wxPG_LABEL,
                                                                              wxT("<composed>") ) );
    if( commFilterFromCollapsed )
      commFilterFrom->SetFlagsFromString( _( "DISABLED|COLLAPSED" ) );
    else
      commFilterFrom->SetFlagsFromString( _( "DISABLED" ) );

    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      if( (*it) == filter->getCommFromFunction() )
        selected = pos;
      pos++;
    }
    wxEnumProperty *fromFunction = new wxEnumProperty( wxT("Function"), wxT("FromFunction"),
                                    arrayFilterFunctions, arrayFilterFunctionsPos, selected );
    windowProperties->AppendIn( commFilterFrom, fromFunction );

    arrayStr.Clear();
    vector<TObjectOrder> fromSel;
    filter->getCommFrom( fromSel );
    for( vector<TObjectOrder>::iterator it = fromSel.begin(); it != fromSel.end(); it++ )
      arrayStr.Add( wxString() << ( (*it) + 1 ) );
    wxArrayStringProperty *fromProperty = new wxArrayStringProperty( wxT("From"), wxPG_LABEL, arrayStr );
    windowProperties->AppendIn( commFilterFrom, fromProperty );

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
    wxEnumProperty *fromToOp = new wxEnumProperty( wxT("From/To Op"), wxT("FromToOp"), 
                               arrayStr, arrayInt, selected );
    windowProperties->AppendIn( commFilterCat, fromToOp );
        
    // Comm To
    wxPGId commFilterTo = windowProperties->AppendIn( commFilterCat, 
                                                      new wxStringProperty( wxT("Comm to"),
                                                                            wxPG_LABEL,
                                                                            wxT("<composed>") ) );
    if( commFilterToCollapsed )
      commFilterTo->SetFlagsFromString( _( "DISABLED|COLLAPSED" ) );
    else
      commFilterTo->SetFlagsFromString( _( "DISABLED" ) );

    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      if( (*it) == filter->getCommToFunction() )
        selected = pos;
      pos++;
    }
    wxEnumProperty *toFunction = new wxEnumProperty( wxT("Function"), wxT("ToFunction"),
                                    arrayFilterFunctions, arrayFilterFunctionsPos, selected );
    windowProperties->AppendIn( commFilterTo, toFunction );

    arrayStr.Clear();
    vector<TObjectOrder> toSel;
    filter->getCommTo( toSel );
    for( vector<TObjectOrder>::iterator it = toSel.begin(); it != toSel.end(); it++ )
      arrayStr.Add( wxString() << ( (*it) + 1 ) );
    wxArrayStringProperty *toProperty = new wxArrayStringProperty( wxT("To"), wxPG_LABEL, arrayStr );
    windowProperties->AppendIn( commFilterTo, toProperty );

    // Comm Tag
    wxPGId commFilterTag = windowProperties->AppendIn( commFilterCat, 
                                                       new wxStringProperty( wxT("Comm tag"),
                                                                             wxPG_LABEL,
                                                                             wxT("<composed>") ) );
    if( commFilterTagCollapsed )
      commFilterTag->SetFlagsFromString( _( "DISABLED|COLLAPSED" ) );
    else
      commFilterTag->SetFlagsFromString( _( "DISABLED" ) );

    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      if( (*it) == filter->getCommTagFunction() )
        selected = pos;
      pos++;
    }
    wxEnumProperty *tagFunction = new wxEnumProperty( wxT("Function"), wxT("TagFunction"),
                                    arrayFilterFunctions, arrayFilterFunctionsPos, selected );
    windowProperties->AppendIn( commFilterTag, tagFunction );

    arrayStr.Clear();
    vector<TCommTag> tagSel;
    filter->getCommTag( tagSel );
    for( vector<TCommTag>::iterator it = tagSel.begin(); it != tagSel.end(); it++ )
      arrayStr.Add( wxString() << (*it) );
    wxArrayStringProperty *tagProperty = new wxArrayStringProperty( wxT("Tag"), wxPG_LABEL, arrayStr );
    windowProperties->AppendIn( commFilterTag, tagProperty );

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
    wxEnumProperty *tagSizeOp = new wxEnumProperty( wxT("Tag/Size Op"), wxT("TagSizeOp"), 
                                arrayStr, arrayInt, selected );
    windowProperties->AppendIn( commFilterCat, tagSizeOp );

    // Comm Size
    wxPGId commFilterSize = windowProperties->AppendIn( commFilterCat, 
                                                        new wxStringProperty( wxT("Comm size"), 
                                                                               wxPG_LABEL,
                                                                               wxT("<composed>") ) );
    if( commFilterSizeCollapsed )
      commFilterSize->SetFlagsFromString( _( "DISABLED|COLLAPSED" ) );
    else
      commFilterSize->SetFlagsFromString( _( "DISABLED" ) );
      
    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      if( (*it) == filter->getCommSizeFunction() )
        selected = pos;
      pos++;
    }
    wxEnumProperty *sizeFunction = new wxEnumProperty( wxT("Function"), wxT("SizeFunction"),
                                    arrayFilterFunctions, arrayFilterFunctionsPos, selected );
    windowProperties->AppendIn( commFilterSize, sizeFunction );

    arrayStr.Clear();
    vector<TCommSize> sizeSel;
    filter->getCommSize( sizeSel );
    for( vector<TCommSize>::iterator it = sizeSel.begin(); it != sizeSel.end(); it++ )
      arrayStr.Add( wxString() << (*it) );
    wxArrayStringProperty *sizeProperty = new wxArrayStringProperty( wxT("Size"), wxPG_LABEL, arrayStr );
    windowProperties->AppendIn( commFilterSize, sizeProperty );
    
    // Comm BandWidth
    wxPGId commFilterBW = windowProperties->AppendIn( commFilterCat, 
                                                      new wxStringProperty( wxT("Comm bandwidth"), 
                                                                            wxPG_LABEL,
                                                                            wxT("<composed>") ) );
    if( commFilterBWCollapsed )
      commFilterBW->SetFlagsFromString( _( "DISABLED|COLLAPSED" ) );
    else
      commFilterBW->SetFlagsFromString( _( "DISABLED" ) );

    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      if( (*it) == filter->getBandWidthFunction() )
        selected = pos;
      pos++;
    }
    wxEnumProperty *bwFunction = new wxEnumProperty( wxT("Function"), wxT("BWFunction"),
                                    arrayFilterFunctions, arrayFilterFunctionsPos, selected );
    windowProperties->AppendIn( commFilterBW, bwFunction );

    arrayStr.Clear();
    vector<TSemanticValue> bwSel;
    filter->getBandWidth( bwSel );
    for( vector<TSemanticValue>::iterator it = bwSel.begin(); it != bwSel.end(); it++ )
      arrayStr.Add( wxString() << (*it) );
    wxArrayStringProperty *bwProperty = new wxArrayStringProperty( wxT("Bandwidth"), wxPG_LABEL, arrayStr );
    windowProperties->AppendIn( commFilterBW, bwProperty );

    // -------------------------------- EVENT FILTER -------------------------------
    wxPGId eventFilterCat = windowProperties->AppendIn( filterCat, new wxPropertyCategory( wxT("Events") ) );
    if( eventFilterCatCollapsed )
      eventFilterCat->SetFlagsFromString( _( "COLLAPSED" ) );
    // Event Type
    wxPGId eventFilterType = windowProperties->AppendIn( eventFilterCat, 
                                                         new wxStringProperty( wxT("Event type"), 
                                                                               wxPG_LABEL,
                                                                               wxT("<composed>") ) );
    if( eventFilterTypeCollapsed )
      eventFilterType->SetFlagsFromString( _( "DISABLED|COLLAPSED" ) );
    else
      eventFilterType->SetFlagsFromString( _( "DISABLED" ) );

    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      if( (*it) == filter->getEventTypeFunction() )
        selected = pos;
      pos++;
    }
    wxEnumProperty *typeFunction = new wxEnumProperty( wxT("Function"), wxT("TypeFunction"),
                                    arrayFilterFunctions, arrayFilterFunctionsPos, selected );
    windowProperties->AppendIn( eventFilterType, typeFunction );

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
    wxArrayInt values;
    vector<TEventType> typesSel;
    filter->getEventType( typesSel );
    for( vector<TEventType>::iterator it = typesSel.begin(); it != typesSel.end(); ++it )
      values.Add( (*it ) );
    prvEventTypeProperty *tmpEventProperty = new prvEventTypeProperty( wxT("Types"), wxPG_LABEL, typeChoices, values );
    windowProperties->AppendIn( eventFilterType, tmpEventProperty );
    windowProperties->SetPropertyAttribute( tmpEventProperty->GetId(), wxPG_ATTR_MULTICHOICE_USERSTRINGMODE, 1 );
    
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
    wxEnumProperty *typeValueOp = new wxEnumProperty( wxT("Type/Value Op"), wxT("TypeValueOp"), 
                                  arrayStr, arrayInt, selected );
    windowProperties->AppendIn( eventFilterCat, typeValueOp );

    // Event Value
    wxPGId eventFilterValue = windowProperties->AppendIn( eventFilterCat, 
                                                          new wxStringProperty( wxT("Event value"), 
                                                                                wxPG_LABEL,
                                                                                wxT("<composed>") ) );
    if( eventFilterValueCollapsed )
      eventFilterValue->SetFlagsFromString( _( "DISABLED|COLLAPSED" ) );
    else
      eventFilterValue->SetFlagsFromString( _( "DISABLED" ) );

    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      if( (*it) == filter->getEventValueFunction() )
        selected = pos;
      pos++;
    }
    wxEnumProperty *valueFunction = new wxEnumProperty( wxT("Function"), wxT("ValueFunction"),
                                    arrayFilterFunctions, arrayFilterFunctionsPos, selected );
    windowProperties->AppendIn( eventFilterValue, valueFunction );

    arrayStr.Clear();
    vector<TEventValue> valuesSel;
    filter->getEventValue( valuesSel );
    for( vector<TEventValue>::iterator it = valuesSel.begin(); it != valuesSel.end(); it++ )
      arrayStr.Add( wxString() << (*it) );
    wxArrayStringProperty *valueProperty = new wxArrayStringProperty( wxT("Values"), wxPG_LABEL, arrayStr );
    windowProperties->AppendIn( eventFilterValue, valueProperty );
  }
  // END of Filter related properties

  //-------------------------------------------------------------------------
  // Semantic related properties
  //-------------------------------------------------------------------------
  TParamValue pValues;
  wxPGId semanticCat = windowProperties->Append( new wxPropertyCategory( wxT("Semantic") ) );
  if( semanticCatCollapsed )
    semanticCat->SetFlagsFromString( _( "COLLAPSED" ) );
  
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
  windowProperties->AppendIn( semanticCat,
                              new wxEnumProperty( wxT("Top Compose 1"),
                              wxPG_LABEL,
                              arrayComposeFunctions, arrayComposeFunctionsPos, selected ) );
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
  windowProperties->AppendIn( semanticCat,
                              new wxEnumProperty( wxT("Top Compose 2"),
                              wxPG_LABEL,
                              arrayComposeFunctions, arrayComposeFunctionsPos, selected ) );
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
      windowProperties->AppendIn( semanticCat,
                                  new wxEnumProperty( wxT("Compose Workload"),
                                  wxPG_LABEL,
                                  arrayComposeFunctions, arrayComposeFunctionsPos, selected ) );
      semanticFunctionParameter( windowProperties, whichWindow, semanticCat, COMPOSEWORKLOAD );

      pos = 0;
      selected = -1;
      for( vector<string>::iterator it = notThreadFunctions.begin();
           it != notThreadFunctions.end(); ++it )
      {
        if( (*it) == whichWindow->getLevelFunction( WORKLOAD ) )
          selected = pos;
        ++pos;
      }
      windowProperties->AppendIn( semanticCat,
                                  new wxEnumProperty( wxT("Workload"),
                                  wxPG_LABEL,
                                  arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected ) );
      semanticFunctionParameter( windowProperties, whichWindow, semanticCat, WORKLOAD );
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
      windowProperties->AppendIn( semanticCat,
                                  new wxEnumProperty( wxT("Compose Appl"),
                                  wxPG_LABEL,
                                  arrayComposeFunctions, arrayComposeFunctionsPos, selected ) );
      semanticFunctionParameter( windowProperties, whichWindow, semanticCat, COMPOSEAPPLICATION );

      pos = 0;
      selected = -1;
      for( vector<string>::iterator it = notThreadFunctions.begin();
           it != notThreadFunctions.end(); ++it )
      {
        if( (*it) == whichWindow->getLevelFunction( APPLICATION ) )
          selected = pos;
        ++pos;
      }
      windowProperties->AppendIn( semanticCat,
                                  new wxEnumProperty( wxT("Application"),
                                  wxPG_LABEL,
                                  arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected ) );
      semanticFunctionParameter( windowProperties, whichWindow, semanticCat, APPLICATION );
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
      windowProperties->AppendIn( semanticCat,
                                  new wxEnumProperty( wxT("Compose Task"),
                                  wxPG_LABEL,
                                  arrayComposeFunctions, arrayComposeFunctionsPos, selected ) );
      semanticFunctionParameter( windowProperties, whichWindow, semanticCat, COMPOSETASK );

      pos = 0;
      selected = -1;
      for( vector<string>::iterator it = notThreadFunctions.begin();
           it != notThreadFunctions.end(); ++it )
      {
        if( (*it) == whichWindow->getLevelFunction( TASK ) )
          selected = pos;
        ++pos;
      }
      windowProperties->AppendIn( semanticCat,
                                  new wxEnumProperty( wxT("Task"),
                                  wxPG_LABEL,
                                  arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected ) );
      semanticFunctionParameter( windowProperties, whichWindow, semanticCat, TASK );
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
      windowProperties->AppendIn( semanticCat,
                                  new wxEnumProperty( wxT("Compose Thread"),
                                  wxPG_LABEL,
                                  arrayComposeFunctions, arrayComposeFunctionsPos, selected ) );
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
      windowProperties->AppendIn( semanticCat,
                                  new wxEnumProperty( wxT("Compose System"),
                                  wxPG_LABEL,
                                  arrayComposeFunctions, arrayComposeFunctionsPos, selected ) );
      semanticFunctionParameter( windowProperties, whichWindow, semanticCat, COMPOSESYSTEM );

      pos = 0;
      selected = -1;
      for( vector<string>::iterator it = notThreadFunctions.begin();
           it != notThreadFunctions.end(); ++it )
      {
        if( (*it) == whichWindow->getLevelFunction( SYSTEM ) )
          selected = pos;
        ++pos;
      }
      windowProperties->AppendIn( semanticCat,
                                  new wxEnumProperty( wxT("System"),
                                  wxPG_LABEL,
                                  arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected ) );
      semanticFunctionParameter( windowProperties, whichWindow, semanticCat, SYSTEM );
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
      windowProperties->AppendIn( semanticCat,
                                  new wxEnumProperty( wxT("Compose Node"),
                                  wxPG_LABEL,
                                  arrayComposeFunctions, arrayComposeFunctionsPos, selected ) );
      semanticFunctionParameter( windowProperties, whichWindow, semanticCat, COMPOSENODE );

      pos = 0;
      selected = -1;
      for( vector<string>::iterator it = notThreadFunctions.begin();
           it != notThreadFunctions.end(); ++it )
      {
        if( (*it) == whichWindow->getLevelFunction( NODE ) )
          selected = pos;
        ++pos;
      }
      windowProperties->AppendIn( semanticCat,
                                  new wxEnumProperty( wxT("Node"),
                                  wxPG_LABEL,
                                  arrayNotThreadFunctions, arrayNotThreadFunctionsPos, selected ) );
      semanticFunctionParameter( windowProperties, whichWindow, semanticCat, NODE );
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
      windowProperties->AppendIn( semanticCat,
                                  new wxEnumProperty( wxT("Compose CPU"),
                                  wxPG_LABEL,
                                  arrayComposeFunctions, arrayComposeFunctionsPos, selected ) );
                                  
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
        windowProperties->AppendIn( semanticCat,
                                    new wxEnumProperty( wxT("CPU"),
                                    wxPG_LABEL,
                                    arrayStr, arrayInt, selected ) );
        semanticFunctionParameter( windowProperties, whichWindow, semanticCat, CPU );
      }
    }
  }
  
  if( ! whichWindow->isDerivedWindow() )
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
    windowProperties->AppendIn( semanticCat,
                                new wxEnumProperty( wxT("Compose Thread"),
                                wxPG_LABEL,
                                arrayComposeFunctions, arrayComposeFunctionsPos, selected ) );
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
    
    windowProperties->AppendIn( semanticCat,
                                new prvSemanticThreadProperty( wxT("Thread"),
                                wxPG_LABEL,
                                levels,
                                threadFunctions,
                                wxString::FromAscii( whichWindow->getLevelFunction( THREAD ).c_str() ) ) );
    semanticFunctionParameter( windowProperties, whichWindow, semanticCat, THREAD );
  }
  
  if( whichWindow->isDerivedWindow() )
  {
    windowProperties->Append( new wxFloatProperty( wxT("Factor #1"), wxPG_LABEL,
                                                 whichWindow->getFactor( 0 ) ) );

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
    windowProperties->AppendIn( semanticCat,
                                new wxEnumProperty( wxT("Derived"),
                                wxPG_LABEL,
                                arrayStr, arrayInt, selected ) );
    semanticFunctionParameter( windowProperties, whichWindow, semanticCat, DERIVED );

    windowProperties->Append( new wxFloatProperty( wxT("Factor #2"), wxPG_LABEL,
                                                 whichWindow->getFactor( 1 ) ) );
  }
  // END of Semantic related properties
  
  windowProperties->SetPropertyAttributeAll( wxPG_BOOL_USE_CHECKBOX, true );

  windowProperties->Refresh();
  windowProperties->Thaw();
}


void updateHistogramProperties( wxPropertyGrid* windowProperties, Histogram *whichHisto )
{
  UINT32 precision = ParaverConfig::getInstance()->getTimelinePrecision();

  updateCategoriesState( windowProperties );
  
  wxArrayString arrayStr;
  wxArrayInt arrayInt;
  int selected, pos;
  
  whichHisto->setChanged( false );
  windowProperties->Freeze();
  windowProperties->Clear();
  
  windowProperties->Append( new wxStringProperty( wxT("Name"), wxPG_LABEL, wxString::FromAscii( whichHisto->getName().c_str() ) ) );
  windowProperties->Append( new wxStringProperty( wxT("Begin time"), wxPG_LABEL, 
                            wxString::FromAscii( LabelConstructor::timeLabel( whichHisto->getControlWindow()->traceUnitsToWindowUnits( whichHisto->getBeginTime() ),
                                                                              whichHisto->getControlWindow()->getTimeUnit(),
                                                                              precision ).c_str() ) ) );
  windowProperties->Append( new wxStringProperty( wxT("End time"), wxPG_LABEL, 
                            wxString::FromAscii( LabelConstructor::timeLabel( whichHisto->getControlWindow()->traceUnitsToWindowUnits( whichHisto->getEndTime() ),
                                                                              whichHisto->getControlWindow()->getTimeUnit(),
                                                                              precision ).c_str() ) ) );

  // Control Window related properties
  wxPGId ctrlCat = windowProperties->Append( new wxPropertyCategory( wxT("Control") ) );
  if( ctrlCatCollapsed )
    ctrlCat->SetFlagsFromString( _( "COLLAPSED" ) );
  
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
  wxEnumProperty *tmpCtrlWin = new wxEnumProperty( wxT("Window"), wxT("ControlWindow"), arrayStr, arrayInt, selected );
  windowProperties->AppendIn( ctrlCat, tmpCtrlWin );
  windowProperties->AppendIn( ctrlCat, new wxFloatProperty( wxT("Minimum"), wxT("ControlMinimum"), whichHisto->getControlMin() ) );
  windowProperties->AppendIn( ctrlCat, new wxFloatProperty( wxT("Maximum"), wxT("ControlMaximum"), whichHisto->getControlMax() ) );
  windowProperties->AppendIn( ctrlCat, new wxFloatProperty( wxT("Delta"), wxT("ControlDelta"), whichHisto->getControlDelta() ) );

  // Statistic related properties
  wxPGId statCat = windowProperties->Append( new wxPropertyCategory( wxT("Statistics") ) );
  if( statCatCollapsed )
    statCat->SetFlagsFromString( _( "COLLAPSED" ) );
  
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
  windowProperties->AppendIn( statCat, new wxEnumProperty( wxT("Type"), wxPG_LABEL, arrayStr, arrayInt, selected ) );
  
  tmpV.clear();
  arrayStr.Clear();
  arrayInt.Clear();
  whichHisto->getStatisticsLabels( tmpV, selected );
  pos = 0;
  selected = -1;
  for( vector<string>::iterator it = tmpV.begin(); it != tmpV.end(); ++it )
  {
    arrayStr.Add( wxString::FromAscii( (*it).c_str() ) );
    arrayInt.Add( pos );
    if( (*it) == whichHisto->getCurrentStat() )
      selected = pos;
    pos++;
  }
  if( selected == -1 ) selected = 0;
  windowProperties->AppendIn( statCat, new wxEnumProperty( wxT("Statistic"), wxPG_LABEL, arrayStr, arrayInt, selected ) );
  windowProperties->AppendIn( statCat, new wxFloatProperty( wxT("Minimum Gradient"), wxT("DataMinimum"), whichHisto->getMinGradient() ) );
  windowProperties->AppendIn( statCat, new wxFloatProperty( wxT("Maximum Gradient"), wxT("DataMaximum"), whichHisto->getMaxGradient() ) );

  // Data Window related properties
  wxPGId dataCat = windowProperties->Append( new wxPropertyCategory( wxT("Data") ) );
  if( dataCatCollapsed )
    dataCat->SetFlagsFromString( _( "COLLAPSED" ) );
  
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
  wxEnumProperty *tmpDataWin = new wxEnumProperty( wxT("Window"), wxT("DataWindow"), arrayStr, arrayInt, selected );
  windowProperties->AppendIn( dataCat, tmpDataWin );

  // 3rd window related properties
  wxPGId thirdWinCat = windowProperties->Append( new wxPropertyCategory( wxT("3D") ) );
  if( thirdWinCatCollapsed )
    thirdWinCat->SetFlagsFromString( _( "COLLAPSED" ) );
  
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
  wxEnumProperty *tmp3rdWin = new wxEnumProperty( wxT("3rd Window"), wxPG_LABEL, arrayStr, arrayInt, selected );
  windowProperties->AppendIn( thirdWinCat, tmp3rdWin );
  wxFloatProperty *tmp3dMin = new wxFloatProperty( wxT("Minimum"), wxT("3DMinimum"), whichHisto->getExtraControlMin() );
  windowProperties->AppendIn( thirdWinCat, tmp3dMin );
  wxFloatProperty *tmp3dMax = new wxFloatProperty( wxT("Maximum"), wxT("3DMaximum"), whichHisto->getExtraControlMax() );
  windowProperties->AppendIn( thirdWinCat, tmp3dMax );
  wxFloatProperty *tmp3dDelta = new wxFloatProperty( wxT("Delta"), wxT("3DDelta"), whichHisto->getExtraControlDelta() );
  windowProperties->AppendIn( thirdWinCat, tmp3dDelta );
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
  wxEnumProperty *tmp3dPlane = new wxEnumProperty( wxT("Plane"), wxT("3DPlane"), arrayStr, arrayInt, selected );
  windowProperties->AppendIn( thirdWinCat, tmp3dPlane );

  if( !whichHisto->getThreeDimensions() )
  {
    tmp3dMin->SetFlagsFromString( _( "DISABLED" ) );
    tmp3dMax->SetFlagsFromString( _( "DISABLED" ) );
    tmp3dDelta->SetFlagsFromString( _( "DISABLED" ) );
    tmp3dPlane->SetFlagsFromString( _( "DISABLED" ) );
  }

  windowProperties->SetPropertyAttributeAll( wxPG_BOOL_USE_CHECKBOX, true );
  windowProperties->Refresh();
  windowProperties->Thaw();
}
