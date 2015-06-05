/* ----------------------------------------------------------------------------
	SaveScreenshot.h


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

#ifndef _SAVESCREENSHOT_H_
#define _SAVESCREENSHOT_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "SaveScreenshot.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/statline.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_SAVESCREENSHOT 21000
#define ID_FILENAME_SAVE 21001
#define ID_SELECT_FILENAME 21002
#define ID_FILETYPE_BMP 21003
#define ID_FILETYPE_JPEG 21004
#define ID_FILETYPE_PNG 21005
#define ID_FILETYPE_TIFF 21006
#define SYMBOL_SAVESCREENSHOT_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_MODAL|wxTAB_TRAVERSAL
#define SYMBOL_SAVESCREENSHOT_TITLE _("Save Screenshot")
#define SYMBOL_SAVESCREENSHOT_IDNAME ID_SAVESCREENSHOT
#define SYMBOL_SAVESCREENSHOT_SIZE wxSize(400, 300)
#define SYMBOL_SAVESCREENSHOT_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * SaveScreenshot class declaration
 */

class SaveScreenshot: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( SaveScreenshot )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    SaveScreenshot();
    SaveScreenshot( wxWindow* parent, wxWindowID id = SYMBOL_SAVESCREENSHOT_IDNAME, const wxString& caption = SYMBOL_SAVESCREENSHOT_TITLE, const wxPoint& pos = SYMBOL_SAVESCREENSHOT_POSITION, const wxSize& size = SYMBOL_SAVESCREENSHOT_SIZE, long style = SYMBOL_SAVESCREENSHOT_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SAVESCREENSHOT_IDNAME, const wxString& caption = SYMBOL_SAVESCREENSHOT_TITLE, const wxPoint& pos = SYMBOL_SAVESCREENSHOT_POSITION, const wxSize& size = SYMBOL_SAVESCREENSHOT_SIZE, long style = SYMBOL_SAVESCREENSHOT_STYLE );

    /// Destructor
    ~SaveScreenshot();

	wxString GetFilePath();
	int GetFiletype();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin SaveScreenshot event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SELECT_FILENAME
    void OnSelectFilenameClick( wxCommandEvent& event );

////@end SaveScreenshot event handler declarations

////@begin SaveScreenshot member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end SaveScreenshot member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin SaveScreenshot member variables
    wxTextCtrl* txtFilename;
    wxButton* btnSelectFilename;
    wxRadioButton* radBMP;
    wxRadioButton* radJPEG;
    wxRadioButton* radPNG;
    wxRadioButton* radTIFF;
////@end SaveScreenshot member variables
};

#endif
    // _SAVESCREENSHOT_H_
