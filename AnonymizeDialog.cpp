/* ----------------------------------------------------------------------------
	AnonymizeDialog.cpp


	MIView - Medical Image Viewer
    Copyright (C) 2009-2011  Gregory Book

    MIView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	--------------------------------------------------------------------------- */


#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "AnonymizeDialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
#include "wx/imaglist.h"
////@end includes

#include "MainFrame.h"
#include "gdcmAnonymizer.h"
#include "gdcmReader.h"
#include "gdcmWriter.h"
#include "wx/dir.h"
#include "wx/File.h"
#include <wx/progdlg.h>
#include <wx/filename.h>
#include "AnonymizeDialog.h"

////@begin XPM images
////@end XPM images


/*!
 * AnonymizeDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( AnonymizeDialog, wxDialog )


/*!
 * AnonymizeDialog event table definition
 */

BEGIN_EVENT_TABLE( AnonymizeDialog, wxDialog )

////@begin AnonymizeDialog event table entries
    EVT_BUTTON( ID_CLEAR_LIST, AnonymizeDialog::OnClearListClick )

    EVT_BUTTON( ID_REMOVE_SELECTED, AnonymizeDialog::OnRemoveSelectedClick )

    EVT_BUTTON( ID_ADD_FILES, AnonymizeDialog::OnAddFilesClick )

    EVT_BUTTON( ID_ADD_DIRECTORY, AnonymizeDialog::OnAddDirectoryClick )

    EVT_LIST_DELETE_ITEM( ID_LIST_FILES, AnonymizeDialog::UpdateListCount )
    EVT_LIST_DELETE_ALL_ITEMS( ID_LIST_FILES, AnonymizeDialog::UpdateListCount )
    EVT_LIST_INSERT_ITEM( ID_LIST_FILES, AnonymizeDialog::UpdateListCount )

    EVT_BUTTON( ID_BUTTON_CHECK_ALL, AnonymizeDialog::OnButtonCheckAllClick )

    EVT_BUTTON( ID_BUTTON_UNCHECK_ALL, AnonymizeDialog::OnButtonUncheckAllClick )

    EVT_BUTTON( ID_BTN_ANONYMIZE, AnonymizeDialog::OnAnonymizeClick )

////@end AnonymizeDialog event table entries

END_EVENT_TABLE()


/*!
 * AnonymizeDialog constructors
 */

AnonymizeDialog::AnonymizeDialog()
{
    Init();
}

AnonymizeDialog::AnonymizeDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * AnonymizeDialog creator
 */

bool AnonymizeDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin AnonymizeDialog creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end AnonymizeDialog creation
    return true;
}


/*!
 * AnonymizeDialog destructor
 */

AnonymizeDialog::~AnonymizeDialog()
{
////@begin AnonymizeDialog destruction
////@end AnonymizeDialog destruction
}


// --------------------------------------------------------
// ---------- Init ----------------------------------------
// --------------------------------------------------------
void AnonymizeDialog::Init()
{
////@begin AnonymizeDialog member initialisation
    btnClearFiles = NULL;
    btnRemoveFiles = NULL;
    btnAddFiles = NULL;
    btnAddDirectory = NULL;
    chkAddRecursive = NULL;
    lstFiles = NULL;
    lblListCount = NULL;
    chkPatientName = NULL;
    txtPatientName = NULL;
    chkPatientBirthdate = NULL;
    txtPatientBirthdate = NULL;
    chkStudyDate = NULL;
    txtStudyDate = NULL;
    chkSeriesDate = NULL;
    txtSeriesDate = NULL;
    chkAcqDate = NULL;
    txtAcqDate = NULL;
    chkContentDate = NULL;
    txtContentDate = NULL;
    chkStudyTime = NULL;
    txtStudyTime = NULL;
    chkSeriesTime = NULL;
    txtSeriesTime = NULL;
    chkAcqTime = NULL;
    txtAcqTime = NULL;
    chkContentTime = NULL;
    txtContentTime = NULL;
    chkModelName = NULL;
    txtModelName = NULL;
    chkSeriesNum = NULL;
    txtSeriesNum = NULL;
    chkInstitName = NULL;
    txtInstitName = NULL;
    chkInstitAddress = NULL;
    txtInstitAddress = NULL;
    chkReferPhysName = NULL;
    txtReferPhysName = NULL;
    chkStationName = NULL;
    txtStationName = NULL;
    chkStudyDesc = NULL;
    txtStudyDesc = NULL;
    chkSeriesDesc = NULL;
    txtSeriesDesc = NULL;
    chkPerformPhysName = NULL;
    txtPerformPhysName = NULL;
    chkOperator = NULL;
    txtOperator = NULL;
    chkPatientID = NULL;
    txtPatientID = NULL;
    chkPatientSex = NULL;
    txtPatientSex = NULL;
    chkPatientWeight = NULL;
    txtPatientWeight = NULL;
    chkSliceThickness = NULL;
    txtSliceThickness = NULL;
    chkPixelSpacing = NULL;
    txtPixelSpacing = NULL;
    btnCheckAll = NULL;
    btnUncheckAll = NULL;
    btnAnonymize = NULL;
////@end AnonymizeDialog member initialisation
}


// --------------------------------------------------------
// ---------- CreateControls ------------------------------
// --------------------------------------------------------
void AnonymizeDialog::CreateControls()
{    
////@begin AnonymizeDialog content construction
    AnonymizeDialog* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer3, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer4, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Original Files..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText5->SetFont(wxFont(10, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer4->Add(itemStaticText5, 0, wxALIGN_BOTTOM|wxALL|wxADJUST_MINSIZE, 1);

    itemBoxSizer4->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    btnClearFiles = new wxButton( itemDialog1, ID_CLEAR_LIST, _("Clear List"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(btnClearFiles, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    btnRemoveFiles = new wxButton( itemDialog1, ID_REMOVE_SELECTED, _("Remove Selected"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(btnRemoveFiles, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer4->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    btnAddFiles = new wxButton( itemDialog1, ID_ADD_FILES, _("Add Files..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(btnAddFiles, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer4->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer4->Add(itemBoxSizer12, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    btnAddDirectory = new wxButton( itemDialog1, ID_ADD_DIRECTORY, _("Add Directory..."), wxDefaultPosition, wxSize(-1, 20), 0 );
    itemBoxSizer12->Add(btnAddDirectory, 0, wxALIGN_LEFT|wxALL, 1);

    chkAddRecursive = new wxCheckBox( itemDialog1, ID_ADD_RECURSIVE, _("Add recursively"), wxDefaultPosition, wxDefaultSize, 0 );
    chkAddRecursive->SetValue(true);
    itemBoxSizer12->Add(chkAddRecursive, 0, wxALIGN_LEFT|wxALL, 1);

    lstFiles = new wxListCtrl( itemDialog1, ID_LIST_FILES, wxDefaultPosition, wxSize(100, 100), wxLC_REPORT );
    itemBoxSizer3->Add(lstFiles, 1, wxGROW|wxALL, 2);

    lblListCount = new wxStaticText( itemDialog1, ID_LIST_COUNT, _("List contains n items"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(lblListCount, 0, wxALIGN_LEFT|wxALL, 2);

    wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer17, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer17->Add(itemBoxSizer18, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer18->Add(itemBoxSizer19, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer20, 0, wxALIGN_LEFT|wxALL, 2);

    chkPatientName = new wxCheckBox( itemDialog1, ID_CHECKBOX4, _("Patient Name (0010,0010)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkPatientName->SetValue(true);
    itemBoxSizer20->Add(chkPatientName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer20->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtPatientName = new wxTextCtrl( itemDialog1, ID_TEXTCTRL1, _("Name^Anonymous"), wxDefaultPosition, wxSize(150, -1), 0 );
    itemBoxSizer20->Add(txtPatientName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer24, 0, wxALIGN_LEFT|wxALL, 2);

    chkPatientBirthdate = new wxCheckBox( itemDialog1, ID_CHECKBOX, _("Patient Birthdate (0010,0030)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkPatientBirthdate->SetValue(true);
    itemBoxSizer24->Add(chkPatientBirthdate, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer24->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtPatientBirthdate = new wxTextCtrl( itemDialog1, ID_TEXTCTRL, _("19000101"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer24->Add(txtPatientBirthdate, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer28 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer28, 0, wxALIGN_LEFT|wxALL, 2);

    chkStudyDate = new wxCheckBox( itemDialog1, ID_CHECKBOX1, _("Study Date (0008,0020)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkStudyDate->SetValue(true);
    itemBoxSizer28->Add(chkStudyDate, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer28->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtStudyDate = new wxTextCtrl( itemDialog1, ID_TEXTCTRL2, _("19000101"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer28->Add(txtStudyDate, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer32 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer32, 0, wxALIGN_LEFT|wxALL, 2);

    chkSeriesDate = new wxCheckBox( itemDialog1, ID_CHECKBOX2, _("Series Date (0008,0021)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkSeriesDate->SetValue(true);
    itemBoxSizer32->Add(chkSeriesDate, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer32->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtSeriesDate = new wxTextCtrl( itemDialog1, ID_TEXTCTRL3, _("19000101"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer32->Add(txtSeriesDate, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer36 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer36, 0, wxALIGN_LEFT|wxALL, 2);

    chkAcqDate = new wxCheckBox( itemDialog1, ID_CHECKBOX3, _("Acquisition Date (0008,0022)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkAcqDate->SetValue(true);
    itemBoxSizer36->Add(chkAcqDate, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer36->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtAcqDate = new wxTextCtrl( itemDialog1, ID_TEXTCTRL4, _("19000101"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer36->Add(txtAcqDate, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer40 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer40, 0, wxALIGN_LEFT|wxALL, 2);

    chkContentDate = new wxCheckBox( itemDialog1, ID_CHECKBOX5, _("Content Date (0008,0023)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkContentDate->SetValue(true);
    itemBoxSizer40->Add(chkContentDate, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer40->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtContentDate = new wxTextCtrl( itemDialog1, ID_TEXTCTRL5, _("19000101"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer40->Add(txtContentDate, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer44 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer44, 0, wxALIGN_LEFT|wxALL, 2);

    chkStudyTime = new wxCheckBox( itemDialog1, ID_CHECKBOX6, _("Study Time (0008,0030)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkStudyTime->SetValue(true);
    itemBoxSizer44->Add(chkStudyTime, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer44->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtStudyTime = new wxTextCtrl( itemDialog1, ID_TEXTCTRL6, _("120000"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer44->Add(txtStudyTime, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer48 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer48, 0, wxALIGN_LEFT|wxALL, 2);

    chkSeriesTime = new wxCheckBox( itemDialog1, ID_CHECKBOX7, _("Series Time (0008,0031)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkSeriesTime->SetValue(true);
    itemBoxSizer48->Add(chkSeriesTime, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer48->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtSeriesTime = new wxTextCtrl( itemDialog1, ID_TEXTCTRL7, _("120000"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer48->Add(txtSeriesTime, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer52 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer52, 0, wxALIGN_LEFT|wxALL, 2);

    chkAcqTime = new wxCheckBox( itemDialog1, ID_CHECKBOX8, _("Acquisition Time (0008,0032)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkAcqTime->SetValue(true);
    itemBoxSizer52->Add(chkAcqTime, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer52->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtAcqTime = new wxTextCtrl( itemDialog1, ID_TEXTCTRL8, _("120000"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer52->Add(txtAcqTime, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer56 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer56, 0, wxALIGN_LEFT|wxALL, 2);

    chkContentTime = new wxCheckBox( itemDialog1, ID_CHECKBOX9, _("Content Time (0008,0033)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkContentTime->SetValue(true);
    itemBoxSizer56->Add(chkContentTime, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer56->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtContentTime = new wxTextCtrl( itemDialog1, ID_TEXTCTRL9, _("120000"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer56->Add(txtContentTime, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer60 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer60, 0, wxALIGN_LEFT|wxALL, 2);

    chkModelName = new wxCheckBox( itemDialog1, ID_CHECKBOX20, _("Manufacturer's Model Name (0008,1090)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkModelName->SetValue(true);
    itemBoxSizer60->Add(chkModelName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer60->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtModelName = new wxTextCtrl( itemDialog1, ID_TEXTCTRL20, _("Anonymous"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer60->Add(txtModelName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer64 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer64, 0, wxALIGN_LEFT|wxALL, 2);

    chkSeriesNum = new wxCheckBox( itemDialog1, ID_CHECKBOX24, _("Series Number (0020,0011)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkSeriesNum->SetValue(true);
    itemBoxSizer64->Add(chkSeriesNum, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer64->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtSeriesNum = new wxTextCtrl( itemDialog1, ID_TEXTCTRL24, _("5000"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer64->Add(txtSeriesNum, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxStaticLine* itemStaticLine68 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
    itemBoxSizer18->Add(itemStaticLine68, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer69 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer18->Add(itemBoxSizer69, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer70 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer69->Add(itemBoxSizer70, 0, wxALIGN_LEFT|wxALL, 2);

    chkInstitName = new wxCheckBox( itemDialog1, ID_CHECKBOX10, _("Institution Name (0008,0080)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkInstitName->SetValue(true);
    itemBoxSizer70->Add(chkInstitName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer70->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtInstitName = new wxTextCtrl( itemDialog1, ID_TEXTCTRL10, _("Name^Anonymous"), wxDefaultPosition, wxSize(150, -1), 0 );
    itemBoxSizer70->Add(txtInstitName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer74 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer69->Add(itemBoxSizer74, 0, wxALIGN_LEFT|wxALL, 2);

    chkInstitAddress = new wxCheckBox( itemDialog1, ID_CHECKBOX11, _("Institution Address (0008,0081)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkInstitAddress->SetValue(true);
    itemBoxSizer74->Add(chkInstitAddress, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer74->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtInstitAddress = new wxTextCtrl( itemDialog1, ID_TEXTCTRL11, _("Name^Anonymous"), wxDefaultPosition, wxSize(150, -1), 0 );
    itemBoxSizer74->Add(txtInstitAddress, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer78 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer69->Add(itemBoxSizer78, 0, wxALIGN_LEFT|wxALL, 2);

    chkReferPhysName = new wxCheckBox( itemDialog1, ID_CHECKBOX12, _("Referring Physician's Name (0008,0090)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkReferPhysName->SetValue(true);
    itemBoxSizer78->Add(chkReferPhysName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer78->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtReferPhysName = new wxTextCtrl( itemDialog1, ID_TEXTCTRL12, _("Name^Anonymous"), wxDefaultPosition, wxSize(150, -1), 0 );
    itemBoxSizer78->Add(txtReferPhysName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer82 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer69->Add(itemBoxSizer82, 0, wxALIGN_LEFT|wxALL, 2);

    chkStationName = new wxCheckBox( itemDialog1, ID_CHECKBOX13, _("Station Name (0008,1010)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkStationName->SetValue(true);
    itemBoxSizer82->Add(chkStationName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer82->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtStationName = new wxTextCtrl( itemDialog1, ID_TEXTCTRL13, _("Anonymous"), wxDefaultPosition, wxSize(150, -1), 0 );
    itemBoxSizer82->Add(txtStationName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer86 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer69->Add(itemBoxSizer86, 0, wxALIGN_LEFT|wxALL, 2);

    chkStudyDesc = new wxCheckBox( itemDialog1, ID_CHECKBOX14, _("Study Description (0008,1030)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkStudyDesc->SetValue(false);
    itemBoxSizer86->Add(chkStudyDesc, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer86->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtStudyDesc = new wxTextCtrl( itemDialog1, ID_TEXTCTRL14, _("Anonymous"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer86->Add(txtStudyDesc, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer90 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer69->Add(itemBoxSizer90, 0, wxALIGN_LEFT|wxALL, 2);

    chkSeriesDesc = new wxCheckBox( itemDialog1, ID_CHECKBOX15, _("Series Description (0008,103E)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkSeriesDesc->SetValue(false);
    itemBoxSizer90->Add(chkSeriesDesc, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer90->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtSeriesDesc = new wxTextCtrl( itemDialog1, ID_TEXTCTRL15, _("Anonymous"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer90->Add(txtSeriesDesc, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer94 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer69->Add(itemBoxSizer94, 0, wxALIGN_LEFT|wxALL, 2);

    chkPerformPhysName = new wxCheckBox( itemDialog1, ID_CHECKBOX16, _("Performing Physician's Name (0008,1050)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkPerformPhysName->SetValue(true);
    itemBoxSizer94->Add(chkPerformPhysName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer94->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtPerformPhysName = new wxTextCtrl( itemDialog1, ID_TEXTCTRL16, _("Name^Anonymous"), wxDefaultPosition, wxSize(150, -1), 0 );
    itemBoxSizer94->Add(txtPerformPhysName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer98 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer69->Add(itemBoxSizer98, 0, wxALIGN_LEFT|wxALL, 2);

    chkOperator = new wxCheckBox( itemDialog1, ID_CHECKBOX17, _("Operator's Name (0008,1070)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkOperator->SetValue(true);
    itemBoxSizer98->Add(chkOperator, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer98->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtOperator = new wxTextCtrl( itemDialog1, ID_TEXTCTRL17, _("Name^Anonymous"), wxDefaultPosition, wxSize(150, -1), 0 );
    itemBoxSizer98->Add(txtOperator, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer102 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer69->Add(itemBoxSizer102, 0, wxALIGN_LEFT|wxALL, 2);

    chkPatientID = new wxCheckBox( itemDialog1, ID_CHECKBOX18, _("Patient's ID (0010,0020)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkPatientID->SetValue(true);
    itemBoxSizer102->Add(chkPatientID, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer102->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtPatientID = new wxTextCtrl( itemDialog1, ID_TEXTCTRL18, _("000000"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer102->Add(txtPatientID, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer106 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer69->Add(itemBoxSizer106, 0, wxALIGN_LEFT|wxALL, 2);

    chkPatientSex = new wxCheckBox( itemDialog1, ID_CHECKBOX19, _("Patient's Sex (0010,0040)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkPatientSex->SetValue(true);
    itemBoxSizer106->Add(chkPatientSex, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer106->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtPatientSex = new wxTextCtrl( itemDialog1, ID_TEXTCTRL19, _("U"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer106->Add(txtPatientSex, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer110 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer69->Add(itemBoxSizer110, 0, wxALIGN_LEFT|wxALL, 2);

    chkPatientWeight = new wxCheckBox( itemDialog1, ID_CHECKBOX21, _("Patient's Weight (0010,1030)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkPatientWeight->SetValue(true);
    itemBoxSizer110->Add(chkPatientWeight, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer110->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtPatientWeight = new wxTextCtrl( itemDialog1, ID_TEXTCTRL21, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer110->Add(txtPatientWeight, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer114 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer69->Add(itemBoxSizer114, 0, wxALIGN_LEFT|wxALL, 2);

    chkSliceThickness = new wxCheckBox( itemDialog1, ID_CHECKBOX22, _("Slice Thickness (0018,0050) &\nSpacing Between Slices (0018,0088)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkSliceThickness->SetValue(true);
    itemBoxSizer114->Add(chkSliceThickness, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer114->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtSliceThickness = new wxTextCtrl( itemDialog1, ID_TEXTCTRL22, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer114->Add(txtSliceThickness, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer118 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer69->Add(itemBoxSizer118, 0, wxALIGN_LEFT|wxALL, 2);

    chkPixelSpacing = new wxCheckBox( itemDialog1, ID_CHECKBOX23, _("Pixel Spacing (0028,0030)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkPixelSpacing->SetValue(true);
    itemBoxSizer118->Add(chkPixelSpacing, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer118->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtPixelSpacing = new wxTextCtrl( itemDialog1, ID_TEXTCTRL23, _("1\\1"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer118->Add(txtPixelSpacing, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxStaticLine* itemStaticLine122 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer17->Add(itemStaticLine122, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer123 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer17->Add(itemBoxSizer123, 1, wxGROW|wxALL, 0);

    btnCheckAll = new wxButton( itemDialog1, ID_BUTTON_CHECK_ALL, _("Check All"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    btnCheckAll->SetFont(wxFont(7, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Tahoma")));
    itemBoxSizer123->Add(btnCheckAll, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    btnUncheckAll = new wxButton( itemDialog1, ID_BUTTON_UNCHECK_ALL, _("Uncheck All"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    btnUncheckAll->SetFont(wxFont(7, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Tahoma")));
    itemBoxSizer123->Add(btnUncheckAll, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    itemBoxSizer123->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton127 = new wxButton( itemDialog1, wxID_CANCEL, _("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer123->Add(itemButton127, 0, wxALIGN_BOTTOM|wxALL, 5);

    itemBoxSizer123->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    btnAnonymize = new wxButton( itemDialog1, ID_BTN_ANONYMIZE, _("Anonymize"), wxDefaultPosition, wxDefaultSize, 0 );
    btnAnonymize->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer123->Add(btnAnonymize, 0, wxALIGN_BOTTOM|wxALL, 5);

////@end AnonymizeDialog content construction

	wxListItem colItem;
	colItem.SetText("Filepath");
	lstFiles->InsertColumn(0,colItem);
	lstFiles->SetColumnWidth(0,400);
	colItem.SetText("File Size");
	lstFiles->InsertColumn(1,colItem);
	lstFiles->SetColumnWidth(1,100);
	colItem.SetText("Result");
	lstFiles->InsertColumn(2,colItem);
	lstFiles->SetColumnWidth(2,100);
}


// --------------------------------------------------------
// ---------- OnClearListClick ----------------------------
// --------------------------------------------------------
void AnonymizeDialog::OnClearListClick( wxCommandEvent& event )
{
	lstFiles->DeleteAllItems();
}


// --------------------------------------------------------
// ---------- OnRemoveSelectedClick -----------------------
// --------------------------------------------------------
void AnonymizeDialog::OnRemoveSelectedClick( wxCommandEvent& event )
{
    long item;

	item = lstFiles->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	while ( item != -1 ) {
		lstFiles->DeleteItem(item);
		// -1 because the indices were shifted by DeleteItem()
		item = lstFiles->GetNextItem(item - 1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	}
}


// --------------------------------------------------------
// ---------- OnAddFilesClick -----------------------------
// --------------------------------------------------------
void AnonymizeDialog::OnAddFilesClick( wxCommandEvent& event )
{
	int i;
	wxString fileSize;
	wxFileName fName;
	static const wxChar	*FILETYPES = _T( "Dicom/ACR-NEMA/Papyrus (*.dcm, *.acr, *.pap)|*.dcm;*.acr;*.pap|Analyze 7.5 (*.img,*.hdr)|*.img;*.hdr|GIF (*.gif)|*.gif|TIFF (*.tif, *.tiff)|*.tif;*.tiff|PNG (*.png)|*.png|BMP (*.bmp)|*.bmp|JPEG (*.jpg,*.jpeg,*.jpe,*.jff,*.jfif,*.jif,*.jtf)|*.jpg;*.jpeg;*.jpe;*.jff;*.jfif;*.jif;*.jtf|All files(*.*)|*.*");

	wxFileDialog *openFileDialog = new wxFileDialog (this, "Select files...", "", "", FILETYPES, wxOPEN|wxFD_MULTIPLE, wxDefaultPosition);

	if (openFileDialog->ShowModal() == wxID_OK) {
		wxArrayString filenames;
		openFileDialog->GetPaths(filenames);
		int numItems = (int)filenames.Count();
		int index = lstFiles->GetItemCount();
		for (i=0;i<numItems;i++) {
			fName.Assign(filenames[i]);
			fileSize = fName.GetHumanReadableSize();
			filenames[i].Replace("\\","/");
			lstFiles->InsertItem(index,filenames[i]);
//			lstFiles->SetItem(index,1,"DICOM");
			lstFiles->SetItem(index,1,fileSize);
		}
	}
}


// --------------------------------------------------------
// ---------- OnAddDirectoryClick -------------------------
// --------------------------------------------------------
void AnonymizeDialog::OnAddDirectoryClick( wxCommandEvent& event )
{
	wxString dirPath;
	wxString msg;

	wxDirDialog *dirDialog = new wxDirDialog(this, "Select Browse Directory", "", wxDD_DEFAULT_STYLE|wxDD_DIR_MUST_EXIST, wxDefaultPosition, wxDefaultSize,"wxDirCtrl");
	if (dirDialog->ShowModal() == wxID_OK) {
		dirPath = dirDialog->GetPath();

		wxArrayString files;
		int fileCount;
		int i;
		wxString fileSize;

		/* go and get the entire list of files */
		if (chkAddRecursive->GetValue())
			wxDir::GetAllFiles(dirPath,&files,"",wxDIR_DEFAULT);
		else
			wxDir::GetAllFiles(dirPath,&files,"",wxDIR_FILES);
		fileCount = (int)files.Count();
		int index = lstFiles->GetItemCount();

		/* setup the progress dialog */
		wxProgressDialog progDlg(wxT("Searching for DICOM files..."), wxT(" "), fileCount, this, wxPD_SMOOTH | wxPD_ELAPSED_TIME | wxPD_CAN_ABORT | wxPD_REMAINING_TIME);

		for (i=0;i<fileCount;i++) {
			if (!progDlg.Update(i,msg.Format("Checking file %d of %d",i,fileCount)))
				break;
			wxFileName fn(files[i]);
			if ( (fn.GetExt() == "dcm") || (fn.GetExt() == "") ) {
				fileSize = fn.GetHumanReadableSize();
				files[i].Replace("\\","/");
				lstFiles->InsertItem(index,files[i]);
				lstFiles->SetItem(index,1,fileSize);
				lstFiles->EnsureVisible(index);
			}
		}
	}
}


// --------------------------------------------------------
// ---------- UpdateListCount -----------------------------
// --------------------------------------------------------
void AnonymizeDialog::UpdateListCount( wxListEvent& event )
{
	lblListCount->SetLabel(wxString::Format("List contains %d items",lstFiles->GetItemCount()+1));
}


// --------------------------------------------------------
// ---------- OnButtonCheckAllClick -----------------------
// --------------------------------------------------------
void AnonymizeDialog::OnButtonCheckAllClick( wxCommandEvent& event )
{
    chkPatientName->SetValue(true);
    chkPatientBirthdate->SetValue(true);
    chkStudyDate->SetValue(true);
    chkSeriesDate->SetValue(true);
    chkAcqDate->SetValue(true);
    chkContentDate->SetValue(true);
    chkStudyTime->SetValue(true);
    chkSeriesTime->SetValue(true);
    chkAcqTime->SetValue(true);
    chkContentTime->SetValue(true);
    chkModelName->SetValue(true);
    chkInstitName->SetValue(true);
    chkInstitAddress->SetValue(true);
    chkReferPhysName->SetValue(true);
    chkStationName->SetValue(true);
    chkStudyDesc->SetValue(true);
    chkSeriesDesc->SetValue(true);
    chkPerformPhysName->SetValue(true);
    chkOperator->SetValue(true);
    chkPatientID->SetValue(true);
    chkPatientSex->SetValue(true);
    chkPatientWeight->SetValue(true);
    chkSliceThickness->SetValue(true);
    chkPixelSpacing->SetValue(true);
    chkSeriesNum->SetValue(true);
}


// --------------------------------------------------------
// ---------- OnButtonUncheckAllClick ---------------------
// --------------------------------------------------------
void AnonymizeDialog::OnButtonUncheckAllClick( wxCommandEvent& event )
{
    chkPatientName->SetValue(false);
    chkPatientBirthdate->SetValue(false);
    chkStudyDate->SetValue(false);
    chkSeriesDate->SetValue(false);
    chkAcqDate->SetValue(false);
    chkContentDate->SetValue(false);
    chkStudyTime->SetValue(false);
    chkSeriesTime->SetValue(false);
    chkAcqTime->SetValue(false);
    chkContentTime->SetValue(false);
    chkModelName->SetValue(false);
    chkInstitName->SetValue(false);
    chkInstitAddress->SetValue(false);
    chkReferPhysName->SetValue(false);
    chkStationName->SetValue(false);
    chkStudyDesc->SetValue(false);
    chkSeriesDesc->SetValue(false);
    chkPerformPhysName->SetValue(false);
    chkOperator->SetValue(false);
    chkPatientID->SetValue(false);
    chkPatientSex->SetValue(false);
    chkPatientWeight->SetValue(false);
    chkSliceThickness->SetValue(false);
    chkPixelSpacing->SetValue(false);
    chkSeriesNum->SetValue(false);
}


// --------------------------------------------------------
// ---------- OnAnonymizeClick ----------------------------
// --------------------------------------------------------
void AnonymizeDialog::OnAnonymizeClick( wxCommandEvent& event )
{
	int i;
	wxString fullfilepath;
	wxString dirname;
	wxString filename;
	wxArrayString filelist;
	wxDir directory;
	wxString msg;

	long item = -1;

	/* get list of files */
    for (;;) {
        item = lstFiles->GetNextItem(item, wxLIST_NEXT_ALL);
        if ( item == -1 ) break;

		/* get the full filepath */
		filelist.Add(lstFiles->GetItemText(item));
    }

	int fileCount = lstFiles->GetItemCount();

	/* setup the progress dialog */
	wxProgressDialog progDlg(wxT("Searching for DICOM files..."), wxT(" "), fileCount, this, wxPD_SMOOTH | wxPD_ELAPSED_TIME | wxPD_CAN_ABORT | wxPD_REMAINING_TIME);
    
    /* loop through the file(s) and open/anonymize/close */
	for (i=0;i<filelist.Count();i++) {
		if (!progDlg.Update(i,msg.Format("Anonymizing file %d of %d",i,fileCount)))
			break;

		/* open dicom file */
		gdcm::Reader reader;
		reader.SetFileName( filelist.Item(i).c_str() );
		if ( !reader.Read() ) {
			/* note it as an error */
			lstFiles->SetItem(i,2,"Failed to read file");
			lstFiles->SetItemBackgroundColour(i,wxColour(210,255,210));
		}
		else {
			gdcm::Anonymizer anonymizer;
			anonymizer.SetFile( reader.GetFile() );

			/* determine which fields to anonymize */
			if (chkPatientName->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0010, 0x0010), txtPatientName->GetValue().c_str());
			if (chkPatientBirthdate->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0010, 0x0030), txtPatientBirthdate->GetValue().c_str());
			if (chkStudyDate->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0008, 0x0020), txtStudyDate->GetValue().c_str());
			if (chkSeriesDate->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0008, 0x0021), txtSeriesDate->GetValue().c_str());
			if (chkAcqDate->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0008, 0x0022), txtAcqDate->GetValue().c_str());
			if (chkContentDate->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0008, 0x0023), txtContentDate->GetValue().c_str());
			if (chkStudyTime->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0008, 0x0030), txtStudyTime->GetValue().c_str());
			if (chkSeriesTime->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0008, 0x0031), txtSeriesTime->GetValue().c_str());
			if (chkAcqTime->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0008, 0x0032), txtAcqTime->GetValue().c_str());
			if (chkContentTime->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0008, 0x0033), txtContentTime->GetValue().c_str());
			if (chkModelName->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0008, 0x1090), txtModelName->GetValue().c_str());
			if (chkInstitName->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0008, 0x0080), txtInstitName->GetValue().c_str());
			if (chkInstitAddress->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0008, 0x0081), txtInstitAddress->GetValue().c_str());
			if (chkReferPhysName->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0008, 0x0090), txtReferPhysName->GetValue().c_str());
			if (chkStationName->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0008, 0x1010), txtStationName->GetValue().c_str());
			if (chkStudyDesc->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0008, 0x1030), txtStudyDesc->GetValue().c_str());
			if (chkSeriesDesc->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0008, 0x103E), txtSeriesDesc->GetValue().c_str());
			if (chkPerformPhysName->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0008, 0x1050), txtPerformPhysName->GetValue().c_str());
			if (chkOperator->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0008, 0x1070), txtOperator->GetValue().c_str());
			if (chkPatientID->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0010, 0x0020), txtPatientID->GetValue().c_str());
			if (chkPatientSex->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0010, 0x0040), txtPatientSex->GetValue().c_str());
			if (chkPatientWeight->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0010, 0x1030), txtPatientWeight->GetValue().c_str());
			if (chkSliceThickness->GetValue()) {
				anonymizer.Replace( gdcm::Tag(0x0018, 0x0050), txtSliceThickness->GetValue().c_str());
				anonymizer.Replace( gdcm::Tag(0x0018, 0x0088), txtSliceThickness->GetValue().c_str());
			}
			if (chkPixelSpacing->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0028, 0x0030), txtPixelSpacing->GetValue().c_str());
			if (chkSeriesNum->GetValue())
				anonymizer.Replace( gdcm::Tag(0x0020, 0x0011), txtSeriesNum->GetValue().c_str());

			gdcm::Writer writer;
			writer.SetFileName( filelist.Item(i).c_str() );
			writer.SetFile( reader.GetFile() );
			writer.SetCheckFileMetaInformation( false );
			if( !writer.Write() ) {
				/* note it as an error */
				lstFiles->SetItem(i,2,"Failed to write file");
				lstFiles->SetItemBackgroundColour(i,wxColour(210,255,210));
			}
			else {
				/* add the file to the list of completed ones */
				lstFiles->SetItem(i,2,"Complete");
				lstFiles->SetItemBackgroundColour(i,wxColour(210,255,210));
			}
			lstFiles->EnsureVisible(i);
		}
	}
}


/*!
 * Should we show tooltips?
 */

bool AnonymizeDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap AnonymizeDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin AnonymizeDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end AnonymizeDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon AnonymizeDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin AnonymizeDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end AnonymizeDialog icon retrieval
}
