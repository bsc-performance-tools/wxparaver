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

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "finddialog.h"
#include "labelconstructor.h"
#include "paraverconfig.h"

////@begin XPM images
////@end XPM images


/*!
 * FindDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( FindDialog, wxDialog )


/*!
 * FindDialog event table definition
 */

BEGIN_EVENT_TABLE( FindDialog, wxDialog )

////@begin FindDialog event table entries
  EVT_UPDATE_UI( ID_CHECKNEXTOBJECT, FindDialog::OnChecknextobjectUpdate )

  EVT_UPDATE_UI( ID_STATICTYPE, FindDialog::OnStatictypeUpdate )

  EVT_UPDATE_UI( ID_CHOICEEVENTS, FindDialog::OnChoiceeventsUpdate )

  EVT_UPDATE_UI( ID_STATICSEMANTICVALUE, FindDialog::OnStaticsemanticvalueUpdate )

  EVT_UPDATE_UI( ID_COMBOSEMANTICVALUE, FindDialog::OnCombosemanticvalueUpdate )

  EVT_UPDATE_UI( ID_STATICSEMANTICDURATION, FindDialog::OnStaticsemanticdurationUpdate )

  EVT_UPDATE_UI( ID_CHOICEDURATIONFUNCTION, FindDialog::OnChoicedurationfunctionUpdate )

  EVT_UPDATE_UI( ID_TEXTSEMANTICDURATION, FindDialog::OnTextsemanticdurationUpdate )

////@end FindDialog event table entries

END_EVENT_TABLE()


/*!
 * FindDialog constructors
 */

FindDialog::FindDialog()
{
  Init();
}

FindDialog::FindDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}


/*!
 * FindDialog creator
 */

bool FindDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin FindDialog creation
  SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Centre();
////@end FindDialog creation
  return true;
}


/*!
 * FindDialog destructor
 */

FindDialog::~FindDialog()
{
////@begin FindDialog destruction
////@end FindDialog destruction
}


/*!
 * Member initialisation
 */

void FindDialog::Init()
{
////@begin FindDialog member initialisation
  myWindow = NULL;
  radioObjects = NULL;
  choiceObjects = NULL;
  choicePosition = NULL;
  checkNextObject = NULL;
  radioEvents = NULL;
  choiceEventType = NULL;
  radioSemantic = NULL;
  comboSemanticValue = NULL;
  choiceDurationFunction = NULL;
  spinSemanticDuration = NULL;
////@end FindDialog member initialisation
}


/*!
 * Control creation for FindDialog
 */

void FindDialog::CreateControls()
{    
////@begin FindDialog content construction
  FindDialog* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer3, 1, wxGROW|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer4Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Objects"));
  wxStaticBoxSizer* itemStaticBoxSizer4 = new wxStaticBoxSizer(itemStaticBoxSizer4Static, wxVERTICAL);
  itemBoxSizer3->Add(itemStaticBoxSizer4, 3, wxGROW|wxALL, 5);

  radioObjects = new wxRadioButton( itemDialog1, ID_RADIOOBJECTS, _("Zoom on"), wxDefaultPosition, wxDefaultSize, 0 );
  radioObjects->SetValue(true);
  itemStaticBoxSizer4->Add(radioObjects, 0, wxGROW|wxALL, 5);

  wxArrayString choiceObjectsStrings;
  choiceObjects = new wxListBox( itemDialog1, ID_CHOICEOBJECT, wxDefaultPosition, wxSize(-1, 100), choiceObjectsStrings, wxLB_SINGLE|wxLB_ALWAYS_SB );
  itemStaticBoxSizer4->Add(choiceObjects, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer4->Add(itemBoxSizer7, 0, wxGROW|wxALL, 5);

  wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("position"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer7->Add(itemStaticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString choicePositionStrings;
  choicePositionStrings.Add(_("begin"));
  choicePositionStrings.Add(_("middle"));
  choicePositionStrings.Add(_("end"));
  choicePosition = new wxChoice( itemDialog1, ID_CHOICEPOSITION, wxDefaultPosition, wxDefaultSize, choicePositionStrings, 0 );
  choicePosition->SetStringSelection(_("middle"));
  itemBoxSizer7->Add(choicePosition, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer10Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Search by time"));
  wxStaticBoxSizer* itemStaticBoxSizer10 = new wxStaticBoxSizer(itemStaticBoxSizer10Static, wxVERTICAL);
  itemBoxSizer3->Add(itemStaticBoxSizer10, 4, wxGROW|wxALL, 5);

  checkNextObject = new wxCheckBox( itemDialog1, ID_CHECKNEXTOBJECT, _("Continue on next object"), wxDefaultPosition, wxDefaultSize, 0 );
  checkNextObject->SetValue(false);
  itemStaticBoxSizer10->Add(checkNextObject, 0, wxALIGN_LEFT|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer12Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Events"));
  wxStaticBoxSizer* itemStaticBoxSizer12 = new wxStaticBoxSizer(itemStaticBoxSizer12Static, wxHORIZONTAL);
  itemStaticBoxSizer10->Add(itemStaticBoxSizer12, 0, wxGROW|wxALL, 5);

  radioEvents = new wxRadioButton( itemDialog1, ID_RADIOEVENTS, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  radioEvents->SetValue(false);
  itemStaticBoxSizer12->Add(radioEvents, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  wxStaticText* itemStaticText14 = new wxStaticText( itemDialog1, ID_STATICTYPE, _("Type"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer12->Add(itemStaticText14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString choiceEventTypeStrings;
  choiceEventType = new wxChoice( itemDialog1, ID_CHOICEEVENTS, wxDefaultPosition, wxDefaultSize, choiceEventTypeStrings, 0 );
  itemStaticBoxSizer12->Add(choiceEventType, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticBox* itemStaticBoxSizer16Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Semantic"));
  wxStaticBoxSizer* itemStaticBoxSizer16 = new wxStaticBoxSizer(itemStaticBoxSizer16Static, wxVERTICAL);
  itemStaticBoxSizer10->Add(itemStaticBoxSizer16, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer16->Add(itemBoxSizer17, 0, wxGROW|wxRIGHT|wxTOP|wxBOTTOM, 5);

  radioSemantic = new wxRadioButton( itemDialog1, ID_RADIOSEMANTIC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  radioSemantic->SetValue(false);
  itemBoxSizer17->Add(radioSemantic, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  wxStaticText* itemStaticText19 = new wxStaticText( itemDialog1, ID_STATICSEMANTICVALUE, _("Value"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer17->Add(itemStaticText19, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString comboSemanticValueStrings;
  comboSemanticValue = new wxComboBox( itemDialog1, ID_COMBOSEMANTICVALUE, wxEmptyString, wxDefaultPosition, wxDefaultSize, comboSemanticValueStrings, wxCB_DROPDOWN );
  itemBoxSizer17->Add(comboSemanticValue, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer16->Add(itemBoxSizer21, 0, wxGROW|wxRIGHT|wxTOP|wxBOTTOM, 5);

  wxStaticText* itemStaticText22 = new wxStaticText( itemDialog1, ID_STATICSEMANTICDURATION, _("Duration"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer21->Add(itemStaticText22, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxArrayString choiceDurationFunctionStrings;
  choiceDurationFunctionStrings.Add(_(">"));
  choiceDurationFunctionStrings.Add(_("<"));
  choiceDurationFunction = new wxChoice( itemDialog1, ID_CHOICEDURATIONFUNCTION, wxDefaultPosition, wxDefaultSize, choiceDurationFunctionStrings, 0 );
  choiceDurationFunction->SetStringSelection(_(">"));
  itemBoxSizer21->Add(choiceDurationFunction, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  spinSemanticDuration = new wxSpinCtrl( itemDialog1, ID_TEXTSEMANTICDURATION, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
  itemBoxSizer21->Add(spinSemanticDuration, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxStaticLine* itemStaticLine25 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
  itemBoxSizer2->Add(itemStaticLine25, 0, wxGROW|wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer26 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer26, 0, wxALIGN_RIGHT|wxALL, 5);
  wxButton* itemButton27 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer26->AddButton(itemButton27);

  wxButton* itemButton28 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStdDialogButtonSizer26->AddButton(itemButton28);

  itemStdDialogButtonSizer26->Realize();

////@end FindDialog content construction
}


/*!
 * Should we show tooltips?
 */

bool FindDialog::ShowToolTips()
{
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap FindDialog::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin FindDialog bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
////@end FindDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon FindDialog::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin FindDialog icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end FindDialog icon retrieval
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHOICE
 */

void FindDialog::OnChoiceeventsUpdate( wxUpdateUIEvent& event )
{
  event.Enable( radioEvents->GetValue() );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TEXTSEMANTIC
 */

void FindDialog::OnCombosemanticvalueUpdate( wxUpdateUIEvent& event )
{
  event.Enable( radioSemantic->GetValue() );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_STATICTYPE
 */

void FindDialog::OnStatictypeUpdate( wxUpdateUIEvent& event )
{
  event.Enable( radioEvents->GetValue() );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_STATICSEMANTIC
 */

void FindDialog::OnStaticsemanticvalueUpdate( wxUpdateUIEvent& event )
{
  event.Enable( radioSemantic->GetValue() );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHECKNEXTOBJECT
 */

void FindDialog::OnChecknextobjectUpdate( wxUpdateUIEvent& event )
{
  event.Enable( radioEvents->GetValue() || radioSemantic->GetValue() );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_STATICSEMANTICDURATION
 */

void FindDialog::OnStaticsemanticdurationUpdate( wxUpdateUIEvent& event )
{
  event.Enable( radioSemantic->GetValue() );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHOICEDURATIONFUNCTION
 */

void FindDialog::OnChoicedurationfunctionUpdate( wxUpdateUIEvent& event )
{
  event.Enable( radioSemantic->GetValue() );
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_COMBOSEMANTICDURATION
 */

void FindDialog::OnTextsemanticdurationUpdate( wxUpdateUIEvent& event )
{
  event.Enable( radioSemantic->GetValue() );
}


void FindDialog::InitControlsBeforeShow()
{
  vector<TObjectOrder> selectedObjects;
  myWindow->getSelectedRows( myWindow->getLevel(), selectedObjects, true );
  
  for( vector<TObjectOrder>::iterator it = selectedObjects.begin();
       it != selectedObjects.end(); ++it )
  {
    string strObject = LabelConstructor::objectLabel( *it,
                                                      myWindow->getLevel(),
                                                      myWindow->getTrace() );
    choiceObjects->Append( wxT( strObject.c_str() ) );
  }
  choiceObjects->SetSelection( 0 );

  set<TEventType> events = myWindow->getTrace()->getLoadedEvents();
  
  for( set<TEventType>::iterator it = events.begin(); it != events.end(); ++it )
  {
    string strEvent = LabelConstructor::eventTypeLabel( myWindow, *it, true );
    choiceEventType->Append( wxT( strEvent.c_str() ) );
  }
  choiceEventType->SetSelection( 0 );
  
  TSemanticValue max = myWindow->getMaximumY() - myWindow->getMinimumY() > 200 ? 
                       myWindow->getMinimumY() + 200 : myWindow->getMaximumY();
  for( TSemanticValue val = myWindow->getMinimumY(); val <= max; ++val )
  {
    string strSemantic = LabelConstructor::semanticLabel( myWindow, val, true, 
                                                          ParaverConfig::getInstance()->getTimelinePrecision() );
    comboSemanticValue->Append( wxT( strSemantic.c_str() ) );
  }
  comboSemanticValue->SetSelection( 0 );
  
  spinSemanticDuration->SetRange( 0, numeric_limits<int>::max() );
  spinSemanticDuration->SetValue( 0 );
}


