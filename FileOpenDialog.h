/* ----------------------------------------------------------------------------
	FileOpenDialog.h


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



#ifndef _FILEOPENDIALOG_H_
#define _FILEOPENDIALOG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "FileOpenDialog.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/statline.h"
#include "wx/spinctrl.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxBoxSizer;
class wxStaticLine;
class wxSpinCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_MYDIALOG 13000
#define ID_FILENAME 13001
#define ID_BTN_GETFILENAME 13002
#define ID_RADIO_OPENSINGLEFILE 13006
#define ID_RADIO_OPENALLFILESWITH 13004
#define ID_RADIO_OPENALL 13003
#define ID_RADIO_FILETYPE_AUTO 13007
#define ID_RADIO_FILETYPE_DICOM 13008
#define ID_RADIO_FILETYPE_IMAGE 13009
#define ID_RADIO_FILETYPE_ANALYZE 13010
#define ID_IS_MOSAIC 13015
#define ID_MOSAIC_XSIZE 13005
#define MOSAIC_YSIZE 13012
#define ID_MOSAIC_SLICES 13013
#define ID_BUTTON_HELP 13011
#define SYMBOL_FILEOPENDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_FILEOPENDIALOG_TITLE _("Open File(s)...")
#define SYMBOL_FILEOPENDIALOG_IDNAME ID_MYDIALOG
#define SYMBOL_FILEOPENDIALOG_SIZE wxSize(550, 250)
#define SYMBOL_FILEOPENDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * FileOpenDialog class declaration
 */

class FileOpenDialog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( FileOpenDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    FileOpenDialog();
    FileOpenDialog( wxWindow* parent, wxWindowID id = SYMBOL_FILEOPENDIALOG_IDNAME, const wxString& caption = SYMBOL_FILEOPENDIALOG_TITLE, const wxPoint& pos = SYMBOL_FILEOPENDIALOG_POSITION, const wxSize& size = SYMBOL_FILEOPENDIALOG_SIZE, long style = SYMBOL_FILEOPENDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_FILEOPENDIALOG_IDNAME, const wxString& caption = SYMBOL_FILEOPENDIALOG_TITLE, const wxPoint& pos = SYMBOL_FILEOPENDIALOG_POSITION, const wxSize& size = SYMBOL_FILEOPENDIALOG_SIZE, long style = SYMBOL_FILEOPENDIALOG_STYLE );

    /// Destructor
    ~FileOpenDialog();
	
	/* public access functions */
	wxArrayString GetFilePaths(void);
	wxString GetFilePrefix(void);
	int GetFileType(void);
	int GetLoadType(void);
	bool GetIsMosaic(void);
	int GetMosaicXSize();
	int GetMosaicYSize();
	int GetMosaicNumSlices();

	/* local variables */
	wxString filename;
	wxString filePrefix;
	wxArrayString filenames;

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin FileOpenDialog event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BTN_GETFILENAME
    void OnOpenFile( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_IS_MOSAIC
    void OnIsMosaicClick( wxCommandEvent& event );

////@end FileOpenDialog event handler declarations

////@begin FileOpenDialog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end FileOpenDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin FileOpenDialog member variables
    wxBoxSizer* sizerMain;
    wxBoxSizer* sizerFilename;
    wxStaticText* labelFilename;
    wxTextCtrl* txtFilename;
    wxButton* btnGetFilename;
    wxRadioButton* radioOpenSingleFile;
    wxRadioButton* radioOpenAllFilesWith;
    wxRadioButton* radioOpenAll;
    wxStaticLine* staticLine1;
    wxBoxSizer* sizerOptions;
    wxStaticBoxSizer* sizerFileFormat;
    wxRadioButton* radioFileTypeAuto;
    wxRadioButton* radioFileTypeDicom;
    wxRadioButton* radioFileTypeImage;
    wxRadioButton* radioFileTypeAnalyze;
    wxCheckBox* chkIsMosaic;
    wxBoxSizer* sizerXYSize;
    wxStaticText* staticXSizeLabel;
    wxSpinCtrl* spinXSize;
    wxStaticText* staticYSizeLabel;
    wxSpinCtrl* spinYSize;
    wxBoxSizer* sizerNumberSlices;
    wxStaticText* staticNumberSlicesLabel;
    wxSpinCtrl* spinNumberOfSlices;
    wxBoxSizer* sizerBottomButtons;
    wxButton* btnHelp;
    wxButton* btnCancel;
    wxButton* btnOpen;
////@end FileOpenDialog member variables
};

#endif
    // _FILEOPENDIALOG_H_
