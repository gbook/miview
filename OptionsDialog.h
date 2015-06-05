/* ----------------------------------------------------------------------------
	OptionsDialog.h


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



#ifndef _OPTIONSDIALOG_H_
#define _OPTIONSDIALOG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "OptionsDialog.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/notebook.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxNotebook;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define OPTIONS_DIALOG 12001
#define ID_NOTEBOOK_OPTIONS 12002
#define ID_PANEL_GENERAL 12003
#define ID_CHECK_USE_SI 12004
#define ID_RADIO_OPENSINGLE 12005
#define ID_RADIO_OPENALLIMAGES 12006
#define ID_RADIO_OPENPREFIX 12007
#define ID_PANEL_FILE_ASSOC 12008
#define ID_CHECK_DICOM 12009
#define ID_CHECK_ANALYZE 12010
#define ID_CHECK_NIFTI 12011
#define SYMBOL_OPTIONSDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_OPTIONSDIALOG_TITLE _("MIView Options")
#define SYMBOL_OPTIONSDIALOG_IDNAME OPTIONS_DIALOG
#define SYMBOL_OPTIONSDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_OPTIONSDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * OptionsDialog class declaration
 */

class OptionsDialog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( OptionsDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    OptionsDialog();
    OptionsDialog( wxWindow* parent, wxWindowID id = SYMBOL_OPTIONSDIALOG_IDNAME, const wxString& caption = SYMBOL_OPTIONSDIALOG_TITLE, const wxPoint& pos = SYMBOL_OPTIONSDIALOG_POSITION, const wxSize& size = SYMBOL_OPTIONSDIALOG_SIZE, long style = SYMBOL_OPTIONSDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_OPTIONSDIALOG_IDNAME, const wxString& caption = SYMBOL_OPTIONSDIALOG_TITLE, const wxPoint& pos = SYMBOL_OPTIONSDIALOG_POSITION, const wxSize& size = SYMBOL_OPTIONSDIALOG_SIZE, long style = SYMBOL_OPTIONSDIALOG_STYLE );

    /// Destructor
    ~OptionsDialog();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin OptionsDialog event handler declarations

////@end OptionsDialog event handler declarations

////@begin OptionsDialog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end OptionsDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin OptionsDialog member variables
    wxNotebook* nbkOptions;
    wxPanel* pnlGeneral;
    wxCheckBox* chkUseSI;
    wxRadioButton* radOpenSingle;
    wxRadioButton* radOpenAll;
    wxRadioButton* radOpenPrefix;
    wxPanel* pnlFileAssoc;
    wxCheckBox* chkDicom;
    wxCheckBox* chkAnalyze;
    wxCheckBox* chkNifti;
////@end OptionsDialog member variables
};

#endif
    // _OPTIONSDIALOG_H_
