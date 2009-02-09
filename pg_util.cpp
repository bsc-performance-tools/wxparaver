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

void updateTimelineProperties( wxPropertyGrid* windowProperties, Window *whichWindow )
{
  whichWindow->setChanged( false );
  windowProperties->Freeze();
  windowProperties->Clear();
  wxArrayString arrayStr;
  wxArrayInt arrayInt;
  int selected, pos;
  
  windowProperties->Append( new wxStringProperty( wxT("Name"), wxPG_LABEL, wxT( whichWindow->getName() ) ) );
  windowProperties->Append( new wxFloatProperty( wxT("Begin time"), wxPG_LABEL, 
                            wxT( whichWindow->getWindowBeginTime() ) ) );
  windowProperties->Append( new wxFloatProperty( wxT("End time"), wxPG_LABEL, 
                            wxT( whichWindow->getWindowEndTime() ) ) );

  // Filter related properties
  if( whichWindow->getFilter() != NULL )
  {
    vector<string> filterFunctions;
    whichWindow->getFilter()->getAllFilterFunctions( filterFunctions );

    wxPGId filterCat = windowProperties->Append( new wxPropertyCategory( wxT("Filter") ) );
    
    // ---------------------------- COMMUNICATION FILTER ---------------------------
    wxPGId commFilterCat = windowProperties->AppendIn( filterCat, new wxPropertyCategory( wxT("Communications") ) );

    windowProperties->AppendIn( commFilterCat, 
        new wxBoolProperty( wxT("Logical"), wxPG_LABEL, whichWindow->getFilter()->getLogical() ) );
    windowProperties->AppendIn( commFilterCat, 
        new wxBoolProperty( wxT("Physical"), wxPG_LABEL, whichWindow->getFilter()->getPhysical() ) );

    // Comm From
    wxPGId commFilterFrom = windowProperties->AppendIn( commFilterCat, 
                                                        new wxStringProperty( wxT("Comm from"),
                                                                              wxPG_LABEL,
                                                                              wxT("<composed>") ) );
    commFilterFrom->SetFlagsFromString( "DISABLED|COLLAPSED" );

    arrayStr.Clear();
    arrayInt.Clear();
    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      arrayStr.Add( wxT( (*it).c_str() ) );
      arrayInt.Add( pos );
      if( (*it) == whichWindow->getFilter()->getCommFromFunction() )
        selected = pos;
      pos++;
    }
    wxEnumProperty *fromFunction = new wxEnumProperty( wxT("Function"), wxT("FromFunction"), arrayStr, arrayInt, selected );
    windowProperties->AppendIn( commFilterFrom, fromFunction );

    arrayStr.Clear();
    vector<TObjectOrder> fromSel;
    whichWindow->getFilter()->getCommFrom( fromSel );
    for( vector<TObjectOrder>::iterator it = fromSel.begin(); it != fromSel.end(); it++ )
      arrayStr.Add( wxString() << ( (*it) + 1 ) );
    wxArrayStringProperty *fromProperty = new wxArrayStringProperty( wxT("From"), wxPG_LABEL, arrayStr );
    windowProperties->AppendIn( commFilterFrom, fromProperty );

    arrayStr.Clear();
    arrayInt.Clear();
    arrayStr.Add( "And" );
    arrayStr.Add( "Or" );
    arrayInt.Add( 0 );
    arrayInt.Add( 1 );
    if( whichWindow->getFilter()->getOpFromTo() )
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
    commFilterTo->SetFlagsFromString( "DISABLED|COLLAPSED" );

    arrayStr.Clear();
    arrayInt.Clear();
    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      arrayStr.Add( wxT( (*it).c_str() ) );
      arrayInt.Add( pos );
      if( (*it) == whichWindow->getFilter()->getCommToFunction() )
        selected = pos;
      pos++;
    }
    wxEnumProperty *toFunction = new wxEnumProperty( wxT("Function"), wxT("ToFunction"), arrayStr, arrayInt, selected );
    windowProperties->AppendIn( commFilterTo, toFunction );

    arrayStr.Clear();
    vector<TObjectOrder> toSel;
    whichWindow->getFilter()->getCommTo( toSel );
    for( vector<TObjectOrder>::iterator it = toSel.begin(); it != toSel.end(); it++ )
      arrayStr.Add( wxString() << ( (*it) + 1 ) );
    wxArrayStringProperty *toProperty = new wxArrayStringProperty( wxT("To"), wxPG_LABEL, arrayStr );
    windowProperties->AppendIn( commFilterTo, toProperty );

    // Comm Tag
    wxPGId commFilterTag = windowProperties->AppendIn( commFilterCat, 
                                                       new wxStringProperty( wxT("Comm tag"),
                                                                             wxPG_LABEL,
                                                                             wxT("<composed>") ) );
    commFilterTag->SetFlagsFromString( "DISABLED|COLLAPSED" );

    arrayStr.Clear();
    arrayInt.Clear();
    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      arrayStr.Add( wxT( (*it).c_str() ) );
      arrayInt.Add( pos );
      if( (*it) == whichWindow->getFilter()->getCommTagFunction() )
        selected = pos;
      pos++;
    }
    wxEnumProperty *tagFunction = new wxEnumProperty( wxT("Function"), wxT("TagFunction"), arrayStr, arrayInt, selected );
    windowProperties->AppendIn( commFilterTag, tagFunction );

    arrayStr.Clear();
    vector<TCommTag> tagSel;
    whichWindow->getFilter()->getCommTag( tagSel );
    for( vector<TCommTag>::iterator it = tagSel.begin(); it != tagSel.end(); it++ )
      arrayStr.Add( wxString() << (*it) );
    wxArrayStringProperty *tagProperty = new wxArrayStringProperty( wxT("Tag"), wxPG_LABEL, arrayStr );
    windowProperties->AppendIn( commFilterTag, tagProperty );

    arrayStr.Clear();
    arrayInt.Clear();
    arrayStr.Add( "And" );
    arrayStr.Add( "Or" );
    arrayInt.Add( 0 );
    arrayInt.Add( 1 );
    if( whichWindow->getFilter()->getOpTagSize() )
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
    commFilterSize->SetFlagsFromString( "DISABLED|COLLAPSED" );

    arrayStr.Clear();
    arrayInt.Clear();
    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      arrayStr.Add( wxT( (*it).c_str() ) );
      arrayInt.Add( pos );
      if( (*it) == whichWindow->getFilter()->getCommSizeFunction() )
        selected = pos;
      pos++;
    }
    wxEnumProperty *sizeFunction = new wxEnumProperty( wxT("Function"), wxT("SizeFunction"), arrayStr, arrayInt, selected );
    windowProperties->AppendIn( commFilterSize, sizeFunction );

    arrayStr.Clear();
    vector<TCommSize> sizeSel;
    whichWindow->getFilter()->getCommSize( sizeSel );
    for( vector<TCommSize>::iterator it = sizeSel.begin(); it != sizeSel.end(); it++ )
      arrayStr.Add( wxString() << (*it) );
    wxArrayStringProperty *sizeProperty = new wxArrayStringProperty( wxT("Size"), wxPG_LABEL, arrayStr );
    windowProperties->AppendIn( commFilterSize, sizeProperty );
    
    // Comm BandWidth
    wxPGId commFilterBW = windowProperties->AppendIn( commFilterCat, 
                                                      new wxStringProperty( wxT("Comm bandwidth"), 
                                                                            wxPG_LABEL,
                                                                            wxT("<composed>") ) );
    commFilterBW->SetFlagsFromString( "DISABLED|COLLAPSED" );

    arrayStr.Clear();
    arrayInt.Clear();
    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      arrayStr.Add( wxT( (*it).c_str() ) );
      arrayInt.Add( pos );
      if( (*it) == whichWindow->getFilter()->getBandWidthFunction() )
        selected = pos;
      pos++;
    }
    wxEnumProperty *bwFunction = new wxEnumProperty( wxT("Function"), wxT("BWFunction"), arrayStr, arrayInt, selected );
    windowProperties->AppendIn( commFilterBW, bwFunction );

    arrayStr.Clear();
    vector<TSemanticValue> bwSel;
    whichWindow->getFilter()->getBandWidth( bwSel );
    for( vector<TSemanticValue>::iterator it = bwSel.begin(); it != bwSel.end(); it++ )
      arrayStr.Add( wxString() << (*it) );
    wxArrayStringProperty *bwProperty = new wxArrayStringProperty( wxT("Bandwidth"), wxPG_LABEL, arrayStr );
    windowProperties->AppendIn( commFilterBW, bwProperty );

    // -------------------------------- EVENT FILTER -------------------------------
    wxPGId eventFilterCat = windowProperties->AppendIn( filterCat, new wxPropertyCategory( wxT("Events") ) );
    // Event Type
    wxPGId eventFilterType = windowProperties->AppendIn( eventFilterCat, 
                                                         new wxStringProperty( wxT("Event type"), 
                                                                               wxPG_LABEL,
                                                                               wxT("<composed>") ) );
    eventFilterType->SetFlagsFromString( "DISABLED|COLLAPSED" );
    
    arrayStr.Clear();
    arrayInt.Clear();
    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      arrayStr.Add( wxT( (*it).c_str() ) );
      arrayInt.Add( pos );
      if( (*it) == whichWindow->getFilter()->getEventTypeFunction() )
        selected = pos;
      pos++;
    }
    wxEnumProperty *typeFunction = new wxEnumProperty( wxT("Function"), wxT("TypeFunction"), arrayStr, arrayInt, selected );
    windowProperties->AppendIn( eventFilterType, typeFunction );

    vector<TEventType> types;
    whichWindow->getTrace()->getEventLabels().getTypes( types );
    arrayStr.Clear();
    arrayInt.Clear();
    for( vector<TEventType>::iterator it = types.begin(); it != types.end(); ++it )
    {
      arrayInt.Add( (*it) );
      string tmpstr;
      whichWindow->getTrace()->getEventLabels().getEventTypeLabel( (*it), tmpstr );
      arrayStr.Add( wxString() << (*it) << " " << wxT( tmpstr.c_str() ) );
    }
    wxPGChoices typeChoices( arrayStr, arrayInt );
    wxArrayInt values;
    vector<TEventType> typesSel;
    whichWindow->getFilter()->getEventType( typesSel );
    for( vector<TEventType>::iterator it = typesSel.begin(); it != typesSel.end(); ++it )
      values.Add( (*it ) );
    prvEventTypeProperty *tmpEventProperty = new prvEventTypeProperty( wxT("Types"), wxPG_LABEL, typeChoices, values );
    windowProperties->AppendIn( eventFilterType, tmpEventProperty );
    windowProperties->SetPropertyAttribute( tmpEventProperty->GetId(), wxPG_ATTR_MULTICHOICE_USERSTRINGMODE, 1 );
    
    arrayStr.Clear();
    arrayInt.Clear();
    arrayStr.Add( "And" );
    arrayStr.Add( "Or" );
    arrayInt.Add( 0 );
    arrayInt.Add( 1 );
    if( whichWindow->getFilter()->getOpTypeValue() )
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
    eventFilterValue->SetFlagsFromString( "DISABLED|COLLAPSED" );
    
    arrayStr.Clear();
    arrayInt.Clear();
    pos = 0;
    selected = -1;
    for( vector<string>::iterator it = filterFunctions.begin();
         it != filterFunctions.end(); ++it )
    {
      arrayStr.Add( wxT( (*it).c_str() ) );
      arrayInt.Add( pos );
      if( (*it) == whichWindow->getFilter()->getEventValueFunction() )
        selected = pos;
      pos++;
    }
    wxEnumProperty *valueFunction = new wxEnumProperty( wxT("Function"), wxT("ValueFunction"), arrayStr, arrayInt, selected );
    windowProperties->AppendIn( eventFilterValue, valueFunction );

    arrayStr.Clear();
    vector<TEventValue> valuesSel;
    whichWindow->getFilter()->getEventValue( valuesSel );
    for( vector<TEventValue>::iterator it = valuesSel.begin(); it != valuesSel.end(); it++ )
      arrayStr.Add( wxString() << (*it) );
    wxArrayStringProperty *valueProperty = new wxArrayStringProperty( wxT("Values"), wxPG_LABEL, arrayStr );
    windowProperties->AppendIn( eventFilterValue, valueProperty );
  }
  // END of Filter related properties
  
  windowProperties->SetPropertyAttributeAll( wxPG_BOOL_USE_CHECKBOX, true );

  windowProperties->Refresh();
  windowProperties->Thaw();
}


void updateHistogramProperties( wxPropertyGrid* windowProperties, Histogram *whichHisto )
{
  whichHisto->setChanged( false );
  windowProperties->Freeze();
  windowProperties->Clear();
  
  windowProperties->Append( new wxStringProperty( wxT("Name"), wxPG_LABEL, wxT( whichHisto->getName() ) ) );
  windowProperties->Append( new wxFloatProperty( wxT("Begin time"), wxPG_LABEL, 
                            wxT( whichHisto->getBeginTime() ) ) );
  windowProperties->Append( new wxFloatProperty( wxT("End time"), wxPG_LABEL, 
                            wxT( whichHisto->getEndTime() ) ) );
  // Statistic related properties
  wxPGId statCat = windowProperties->Append( new wxPropertyCategory( wxT("Statistics") ) );
  windowProperties->AppendIn( statCat, new wxBoolProperty( wxT("Calculate all"), wxPG_LABEL, whichHisto->getCalculateAll() ) );
  vector<string> tmpV;
  wxArrayString arrayStr;
  wxArrayInt arrayInt;
  whichHisto->getGroupsLabels( tmpV );
  for( vector<string>::iterator it = tmpV.begin(); it != tmpV.end(); ++it )
    arrayStr.Add( wxT( (*it).c_str() ) );
  arrayInt.Add( 0 );
  arrayInt.Add( 1 );
  int selected;
  if( whichHisto->itsCommunicationStat( whichHisto->getCurrentStat() ) )
    selected = 0;
  else
    selected = 1;
  windowProperties->AppendIn( statCat, new wxEnumProperty( wxT("Type"), wxPG_LABEL, arrayStr, arrayInt, selected ) );
  
  tmpV.clear();
  arrayStr.Clear();
  arrayInt.Clear();
  whichHisto->getStatisticsLabels( tmpV, selected );
  int pos = 0;
  selected = -1;
  for( vector<string>::iterator it = tmpV.begin(); it != tmpV.end(); ++it )
  {
    arrayStr.Add( wxT( (*it).c_str() ) );
    arrayInt.Add( pos );
    if( (*it) == whichHisto->getCurrentStat() )
      selected = pos;
    pos++;
  }
  if( selected == -1 ) selected = 0;
  windowProperties->AppendIn( statCat, new wxEnumProperty( wxT("Statistic"), wxPG_LABEL, arrayStr, arrayInt, selected ) );

  // Control Window related properties
  wxPGId ctrlCat = windowProperties->Append( new wxPropertyCategory( wxT("Control") ) );
  vector<TWindowID> validWin;
  Window *dataWindow = ( whichHisto->getDataWindow() == NULL ) ? whichHisto->getControlWindow() :
                                                                 whichHisto->getDataWindow();
  LoadedWindows::getInstance()->getValidControlWindow( dataWindow, validWin );
  arrayStr.Clear();
  arrayInt.Clear();
  selected = -1;
  for( vector<TWindowID>::iterator it = validWin.begin(); it != validWin.end(); ++it )
  {
    arrayStr.Add( wxT( LoadedWindows::getInstance()->getWindow( (*it) )->getName().c_str() ) );
    arrayInt.Add( (*it) );
    // Do we need this -if- here?
    if( LoadedWindows::getInstance()->getWindow( (*it) ) == whichHisto->getControlWindow() )
      selected = (*it);
  }
  wxEnumProperty *tmpCtrlWin = new wxEnumProperty( wxT("Window"), wxT("ControlWindow"), arrayStr, arrayInt, selected );
  windowProperties->AppendIn( ctrlCat, tmpCtrlWin );
  windowProperties->AppendIn( ctrlCat, new wxFloatProperty( wxT("Minimum"), wxT("ControlMinimum"), wxT( whichHisto->getControlMin() )));
  windowProperties->AppendIn( ctrlCat, new wxFloatProperty( wxT("Maximum"), wxT("ControlMaximum"), wxT( whichHisto->getControlMax() )));
  windowProperties->AppendIn( ctrlCat, new wxFloatProperty( wxT("Delta"), wxT("ControlDelta"), wxT( whichHisto->getControlDelta() )));

  // Data Window related properties
  wxPGId dataCat = windowProperties->Append( new wxPropertyCategory( wxT("Data") ) );
  validWin.clear();  //  vector<TWindowID> validWin;
  LoadedWindows::getInstance()->getValidDataWindow( whichHisto->getControlWindow(),
                                                    whichHisto->getExtraControlWindow(),
                                                    validWin );
  arrayStr.Clear();
  arrayInt.Clear();
  selected = -1;
  for( vector<TWindowID>::iterator it = validWin.begin(); it != validWin.end(); ++it )
  {
    arrayStr.Add( wxT( LoadedWindows::getInstance()->getWindow( (*it) )->getName().c_str() ) );
    arrayInt.Add( (*it) );
    if( LoadedWindows::getInstance()->getWindow( (*it) ) == whichHisto->getDataWindow() )
      selected = (*it);
  }
  wxEnumProperty *tmpDataWin = new wxEnumProperty( wxT("Window"), wxT("DataWindow"), arrayStr, arrayInt, selected );
  windowProperties->AppendIn( dataCat, tmpDataWin );

  // 3rd window related properties
  wxPGId thirdWinCat = windowProperties->Append( new wxPropertyCategory( wxT("3D") ) );
  windowProperties->AppendIn( thirdWinCat, new wxBoolProperty( wxT("Activate 3D"), wxPG_LABEL, whichHisto->getThreeDimensions() ) );
  validWin.clear();
  dataWindow = ( whichHisto->getDataWindow() == NULL ) ? whichHisto->getControlWindow() :
                                                                 whichHisto->getDataWindow();
  LoadedWindows::getInstance()->getValidControlWindow( dataWindow, validWin );
  arrayStr.Clear();
  arrayInt.Clear();
  selected = -1;
  for( vector<TWindowID>::iterator it = validWin.begin(); it != validWin.end(); ++it )
  {
    arrayStr.Add( wxT( LoadedWindows::getInstance()->getWindow( (*it) )->getName().c_str() ) );
    arrayInt.Add( (*it) );
    if( ( LoadedWindows::getInstance()->getWindow( (*it) ) == whichHisto->getExtraControlWindow() ) ||
        ( whichHisto->getExtraControlWindow() == NULL && 
          LoadedWindows::getInstance()->getWindow( (*it) ) == whichHisto->getControlWindow() ) )
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
      arrayStr.Add( whichHisto->getPlaneLabel( i ) );
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
    tmp3rdWin->SetFlagsFromString( "DISABLED" );
    tmp3dMin->SetFlagsFromString( "DISABLED" );
    tmp3dMax->SetFlagsFromString( "DISABLED" );
    tmp3dDelta->SetFlagsFromString( "DISABLED" );
    tmp3dPlane->SetFlagsFromString( "DISABLED" );
  }

  windowProperties->SetPropertyAttributeAll( wxPG_BOOL_USE_CHECKBOX, true );
  windowProperties->Refresh();
  windowProperties->Thaw();
}
