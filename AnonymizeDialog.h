/* ----------------------------------------------------------------------------
	AnonymizeDialog.h


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



#ifndef _ANONYMIZEDIALOG_H_
#define _ANONYMIZEDIALOG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "AnonymizeDialog.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/listctrl.h"
#include "wx/statline.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxListCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_ANONYMIZEDIALOG 17000
#define ID_CLEAR_LIST 17001
#define ID_REMOVE_SELECTED 17002
#define ID_ADD_FILES 17003
#define ID_ADD_DIRECTORY 17052
#define ID_ADD_RECURSIVE 17053
#define ID_LIST_FILES 17004
#define ID_LIST_COUNT 17054
#define ID_CHECKBOX4 17005
#define ID_TEXTCTRL1 17006
#define ID_CHECKBOX 17007
#define ID_TEXTCTRL 17008
#define ID_CHECKBOX1 17009
#define ID_TEXTCTRL2 17010
#define ID_CHECKBOX2 17011
#define ID_TEXTCTRL3 17012
#define ID_CHECKBOX3 17013
#define ID_TEXTCTRL4 17014
#define ID_CHECKBOX5 17015
#define ID_TEXTCTRL5 17016
#define ID_CHECKBOX6 17017
#define ID_TEXTCTRL6 17018
#define ID_CHECKBOX7 17019
#define ID_TEXTCTRL7 17020
#define ID_CHECKBOX8 17021
#define ID_TEXTCTRL8 17022
#define ID_CHECKBOX9 17023
#define ID_TEXTCTRL9 17024
#define ID_CHECKBOX20 17025
#define ID_TEXTCTRL20 17026
#define ID_CHECKBOX24 17059
#define ID_TEXTCTRL24 17060
#define ID_CHECKBOX10 17027
#define ID_TEXTCTRL10 17028
#define ID_CHECKBOX11 17029
#define ID_TEXTCTRL11 17030
#define ID_CHECKBOX12 17031
#define ID_TEXTCTRL12 17032
#define ID_CHECKBOX13 17033
#define ID_TEXTCTRL13 17034
#define ID_CHECKBOX14 17035
#define ID_TEXTCTRL14 17036
#define ID_CHECKBOX15 17037
#define ID_TEXTCTRL15 17038
#define ID_CHECKBOX16 17039
#define ID_TEXTCTRL16 17040
#define ID_CHECKBOX17 17041
#define ID_TEXTCTRL17 17042
#define ID_CHECKBOX18 17043
#define ID_TEXTCTRL18 17044
#define ID_CHECKBOX19 17045
#define ID_TEXTCTRL19 17046
#define ID_CHECKBOX21 17047
#define ID_TEXTCTRL21 17048
#define ID_CHECKBOX22 17055
#define ID_TEXTCTRL22 17056
#define ID_CHECKBOX23 17057
#define ID_TEXTCTRL23 17058
#define ID_BUTTON_CHECK_ALL 17050
#define ID_BUTTON_UNCHECK_ALL 17051
#define ID_BTN_ANONYMIZE 17049
#define SYMBOL_ANONYMIZEDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_ANONYMIZEDIALOG_TITLE _("Anonymize")
#define SYMBOL_ANONYMIZEDIALOG_IDNAME ID_ANONYMIZEDIALOG
#define SYMBOL_ANONYMIZEDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_ANONYMIZEDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * AnonymizeDialog class declaration
 */

class AnonymizeDialog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( AnonymizeDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    AnonymizeDialog();
    AnonymizeDialog( wxWindow* parent, wxWindowID id = SYMBOL_ANONYMIZEDIALOG_IDNAME, const wxString& caption = SYMBOL_ANONYMIZEDIALOG_TITLE, const wxPoint& pos = SYMBOL_ANONYMIZEDIALOG_POSITION, const wxSize& size = SYMBOL_ANONYMIZEDIALOG_SIZE, long style = SYMBOL_ANONYMIZEDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ANONYMIZEDIALOG_IDNAME, const wxString& caption = SYMBOL_ANONYMIZEDIALOG_TITLE, const wxPoint& pos = SYMBOL_ANONYMIZEDIALOG_POSITION, const wxSize& size = SYMBOL_ANONYMIZEDIALOG_SIZE, long style = SYMBOL_ANONYMIZEDIALOG_STYLE );

    /// Destructor
    ~AnonymizeDialog();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin AnonymizeDialog event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_CLEAR_LIST
    void OnClearListClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_REMOVE_SELECTED
    void OnRemoveSelectedClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ADD_FILES
    void OnAddFilesClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ADD_DIRECTORY
    void OnAddDirectoryClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_LIST_DELETE_ITEM event handler for ID_LIST_FILES
    void UpdateListCount( wxListEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CHECK_ALL
    void OnButtonCheckAllClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_UNCHECK_ALL
    void OnButtonUncheckAllClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BTN_ANONYMIZE
    void OnAnonymizeClick( wxCommandEvent& event );

////@end AnonymizeDialog event handler declarations

////@begin AnonymizeDialog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end AnonymizeDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin AnonymizeDialog member variables
    wxButton* btnClearFiles;
    wxButton* btnRemoveFiles;
    wxButton* btnAddFiles;
    wxButton* btnAddDirectory;
    wxCheckBox* chkAddRecursive;
    wxListCtrl* lstFiles;
    wxStaticText* lblListCount;
    wxCheckBox* chkPatientName;
    wxTextCtrl* txtPatientName;
    wxCheckBox* chkPatientBirthdate;
    wxTextCtrl* txtPatientBirthdate;
    wxCheckBox* chkStudyDate;
    wxTextCtrl* txtStudyDate;
    wxCheckBox* chkSeriesDate;
    wxTextCtrl* txtSeriesDate;
    wxCheckBox* chkAcqDate;
    wxTextCtrl* txtAcqDate;
    wxCheckBox* chkContentDate;
    wxTextCtrl* txtContentDate;
    wxCheckBox* chkStudyTime;
    wxTextCtrl* txtStudyTime;
    wxCheckBox* chkSeriesTime;
    wxTextCtrl* txtSeriesTime;
    wxCheckBox* chkAcqTime;
    wxTextCtrl* txtAcqTime;
    wxCheckBox* chkContentTime;
    wxTextCtrl* txtContentTime;
    wxCheckBox* chkModelName;
    wxTextCtrl* txtModelName;
    wxCheckBox* chkSeriesNum;
    wxTextCtrl* txtSeriesNum;
    wxCheckBox* chkInstitName;
    wxTextCtrl* txtInstitName;
    wxCheckBox* chkInstitAddress;
    wxTextCtrl* txtInstitAddress;
    wxCheckBox* chkReferPhysName;
    wxTextCtrl* txtReferPhysName;
    wxCheckBox* chkStationName;
    wxTextCtrl* txtStationName;
    wxCheckBox* chkStudyDesc;
    wxTextCtrl* txtStudyDesc;
    wxCheckBox* chkSeriesDesc;
    wxTextCtrl* txtSeriesDesc;
    wxCheckBox* chkPerformPhysName;
    wxTextCtrl* txtPerformPhysName;
    wxCheckBox* chkOperator;
    wxTextCtrl* txtOperator;
    wxCheckBox* chkPatientID;
    wxTextCtrl* txtPatientID;
    wxCheckBox* chkPatientSex;
    wxTextCtrl* txtPatientSex;
    wxCheckBox* chkPatientWeight;
    wxTextCtrl* txtPatientWeight;
    wxCheckBox* chkSliceThickness;
    wxTextCtrl* txtSliceThickness;
    wxCheckBox* chkPixelSpacing;
    wxTextCtrl* txtPixelSpacing;
    wxButton* btnCheckAll;
    wxButton* btnUncheckAll;
    wxButton* btnAnonymize;
////@end AnonymizeDialog member variables
};

#endif
    // _ANONYMIZEDIALOG_H_
