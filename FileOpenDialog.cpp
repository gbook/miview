/* ----------------------------------------------------------------------------
	FileOpenDialog.cpp


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
#pragma implementation "FileOpenDialog.h"
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
////@end includes

#include "MainFrame.h"
#include "FileOpenDialog.h"

////@begin XPM images
////@end XPM images


/*!
 * FileOpenDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( FileOpenDialog, wxDialog )


/*!
 * FileOpenDialog event table definition
 */

BEGIN_EVENT_TABLE( FileOpenDialog, wxDialog )

////@begin FileOpenDialog event table entries
    EVT_BUTTON( ID_BTN_GETFILENAME, FileOpenDialog::OnOpenFile )

    EVT_CHECKBOX( ID_IS_MOSAIC, FileOpenDialog::OnIsMosaicClick )

////@end FileOpenDialog event table entries

END_EVENT_TABLE()


/*!
 * FileOpenDialog constructors
 */

FileOpenDialog::FileOpenDialog()
{
    Init();
}

FileOpenDialog::FileOpenDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * FileOpenDialog creator
 */

bool FileOpenDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin FileOpenDialog creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end FileOpenDialog creation
    return true;
}


/*!
 * FileOpenDialog destructor
 */

FileOpenDialog::~FileOpenDialog()
{
////@begin FileOpenDialog destruction
////@end FileOpenDialog destruction
}


/*!
 * Member initialisation
 */

void FileOpenDialog::Init()
{
////@begin FileOpenDialog member initialisation
    sizerMain = NULL;
    sizerFilename = NULL;
    labelFilename = NULL;
    txtFilename = NULL;
    btnGetFilename = NULL;
    radioOpenSingleFile = NULL;
    radioOpenAllFilesWith = NULL;
    radioOpenAll = NULL;
    staticLine1 = NULL;
    sizerOptions = NULL;
    sizerFileFormat = NULL;
    radioFileTypeAuto = NULL;
    radioFileTypeDicom = NULL;
    radioFileTypeImage = NULL;
    radioFileTypeAnalyze = NULL;
    chkIsMosaic = NULL;
    sizerXYSize = NULL;
    staticXSizeLabel = NULL;
    spinXSize = NULL;
    staticYSizeLabel = NULL;
    spinYSize = NULL;
    sizerNumberSlices = NULL;
    staticNumberSlicesLabel = NULL;
    spinNumberOfSlices = NULL;
    sizerBottomButtons = NULL;
    btnHelp = NULL;
    btnCancel = NULL;
    btnOpen = NULL;
////@end FileOpenDialog member initialisation
}


/*!
 * Control creation for FileOpenDialog
 */

void FileOpenDialog::CreateControls()
{    
////@begin FileOpenDialog content construction
    FileOpenDialog* itemDialog1 = this;

    sizerMain = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(sizerMain);

    sizerFilename = new wxBoxSizer(wxHORIZONTAL);
    sizerMain->Add(sizerFilename, 0, wxGROW|wxALL, 5);

    labelFilename = new wxStaticText( itemDialog1, wxID_STATIC, _("Filename"), wxDefaultPosition, wxDefaultSize, 0 );
    labelFilename->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    sizerFilename->Add(labelFilename, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    txtFilename = new wxTextCtrl( itemDialog1, ID_FILENAME, wxEmptyString, wxDefaultPosition, wxSize(349, -1), 0 );
    sizerFilename->Add(txtFilename, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    btnGetFilename = new wxButton( itemDialog1, ID_BTN_GETFILENAME, _("..."), wxDefaultPosition, wxSize(24, -1), 0 );
    sizerFilename->Add(btnGetFilename, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    radioOpenSingleFile = new wxRadioButton( itemDialog1, ID_RADIO_OPENSINGLEFILE, _("Open only this file"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
    radioOpenSingleFile->SetValue(false);
    sizerMain->Add(radioOpenSingleFile, 0, wxALIGN_LEFT|wxALL, 5);

    radioOpenAllFilesWith = new wxRadioButton( itemDialog1, ID_RADIO_OPENALLFILESWITH, _("Open only selected files"), wxDefaultPosition, wxDefaultSize, 0 );
    radioOpenAllFilesWith->SetValue(true);
    sizerMain->Add(radioOpenAllFilesWith, 0, wxALIGN_LEFT|wxALL, 5);

    radioOpenAll = new wxRadioButton( itemDialog1, ID_RADIO_OPENALL, _("Open all files in folder with same extension"), wxDefaultPosition, wxDefaultSize, 0 );
    radioOpenAll->SetValue(false);
    sizerMain->Add(radioOpenAll, 0, wxALIGN_LEFT|wxALL, 5);

    staticLine1 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    sizerMain->Add(staticLine1, 0, wxGROW|wxALL, 0);

    sizerOptions = new wxBoxSizer(wxHORIZONTAL);
    sizerMain->Add(sizerOptions, 1, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer12Static = new wxStaticBox(itemDialog1, wxID_ANY, _("File Format"));
    sizerFileFormat = new wxStaticBoxSizer(itemStaticBoxSizer12Static, wxVERTICAL);
    sizerOptions->Add(sizerFileFormat, 0, wxGROW|wxALL, 5);

    radioFileTypeAuto = new wxRadioButton( itemDialog1, ID_RADIO_FILETYPE_AUTO, _("Automatically Detect"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
    radioFileTypeAuto->SetValue(true);
    sizerFileFormat->Add(radioFileTypeAuto, 0, wxALIGN_LEFT|wxALL, 5);

    radioFileTypeDicom = new wxRadioButton( itemDialog1, ID_RADIO_FILETYPE_DICOM, _("Dicom, ACR/NEMA (.dcm)"), wxDefaultPosition, wxDefaultSize, 0 );
    radioFileTypeDicom->SetValue(false);
    sizerFileFormat->Add(radioFileTypeDicom, 0, wxALIGN_LEFT|wxALL, 5);

    radioFileTypeImage = new wxRadioButton( itemDialog1, ID_RADIO_FILETYPE_IMAGE, _("Image (.jpg .png .bmp .tif .gif)"), wxDefaultPosition, wxDefaultSize, 0 );
    radioFileTypeImage->SetValue(false);
    sizerFileFormat->Add(radioFileTypeImage, 0, wxALIGN_LEFT|wxALL, 5);

    radioFileTypeAnalyze = new wxRadioButton( itemDialog1, ID_RADIO_FILETYPE_ANALYZE, _("Analyze 7.5 or Nifti1 (.img .hdr .nii)"), wxDefaultPosition, wxDefaultSize, 0 );
    radioFileTypeAnalyze->SetValue(false);
    sizerFileFormat->Add(radioFileTypeAnalyze, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer17Static = new wxStaticBox(itemDialog1, wxID_ANY, _("DICOM Mosaic Options"));
    wxStaticBoxSizer* itemStaticBoxSizer17 = new wxStaticBoxSizer(itemStaticBoxSizer17Static, wxVERTICAL);
    sizerOptions->Add(itemStaticBoxSizer17, 0, wxGROW|wxALL, 5);

    chkIsMosaic = new wxCheckBox( itemDialog1, ID_IS_MOSAIC, _("File(s) are DICOM mosaic images"), wxDefaultPosition, wxDefaultSize, 0 );
    chkIsMosaic->SetValue(false);
    itemStaticBoxSizer17->Add(chkIsMosaic, 0, wxALIGN_LEFT|wxALL, 5);

    sizerXYSize = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer17->Add(sizerXYSize, 0, wxALIGN_LEFT|wxALL, 0);

    staticXSizeLabel = new wxStaticText( itemDialog1, wxID_STATIC, _("X size"), wxDefaultPosition, wxDefaultSize, 0 );
    sizerXYSize->Add(staticXSizeLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 3);

    spinXSize = new wxSpinCtrl( itemDialog1, ID_MOSAIC_XSIZE, _T("64"), wxDefaultPosition, wxSize(49, -1), wxSP_ARROW_KEYS, 0, 32768, 64 );
    spinXSize->Enable(false);
    sizerXYSize->Add(spinXSize, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    sizerXYSize->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    staticYSizeLabel = new wxStaticText( itemDialog1, wxID_STATIC, _("Y size"), wxDefaultPosition, wxDefaultSize, 0 );
    sizerXYSize->Add(staticYSizeLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 3);

    spinYSize = new wxSpinCtrl( itemDialog1, MOSAIC_YSIZE, _T("64"), wxDefaultPosition, wxSize(49, -1), wxSP_ARROW_KEYS, 0, 32768, 64 );
    spinYSize->Enable(false);
    sizerXYSize->Add(spinYSize, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    sizerNumberSlices = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer17->Add(sizerNumberSlices, 0, wxALIGN_LEFT|wxALL, 0);

    staticNumberSlicesLabel = new wxStaticText( itemDialog1, wxID_STATIC, _("Number of Slices"), wxDefaultPosition, wxDefaultSize, 0 );
    sizerNumberSlices->Add(staticNumberSlicesLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 3);

    spinNumberOfSlices = new wxSpinCtrl( itemDialog1, ID_MOSAIC_SLICES, _T("36"), wxDefaultPosition, wxSize(75, -1), wxSP_ARROW_KEYS, 0, 65565, 36 );
    spinNumberOfSlices->Enable(false);
    sizerNumberSlices->Add(spinNumberOfSlices, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxStaticText* itemStaticText28 = new wxStaticText( itemDialog1, wxID_STATIC, _("Mosaic images MUST describe a 3D volume"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText28->SetForegroundColour(wxColour(170, 0, 0));
    itemStaticBoxSizer17->Add(itemStaticText28, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    sizerOptions->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    sizerBottomButtons = new wxBoxSizer(wxVERTICAL);
    sizerOptions->Add(sizerBottomButtons, 0, wxGROW|wxALL, 5);

    btnHelp = new wxButton( itemDialog1, ID_BUTTON_HELP, _("Help"), wxDefaultPosition, wxDefaultSize, 0 );
    sizerBottomButtons->Add(btnHelp, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    sizerBottomButtons->Add(0, 5, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    btnCancel = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    sizerBottomButtons->Add(btnCancel, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    btnOpen = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    sizerBottomButtons->Add(btnOpen, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

////@end FileOpenDialog content construction

	// disable all the special controls
	spinNumberOfSlices->Disable();
	spinXSize->Disable();
	spinYSize->Disable();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BTN_GETFILENAME
 */

void FileOpenDialog::OnOpenFile( wxCommandEvent& event )
{
	static const wxChar	*FILETYPES = _T( "Dicom/ACR-NEMA/Papyrus (*.dcm, *.acr, *.pap)|*.dcm;*.acr;*.pap|Analyze 7.5 (*.img,*.hdr)|*.img;*.hdr|GIF (*.gif)|*.gif|TIFF (*.tif, *.tiff)|*.tif;*.tiff|PNG (*.png)|*.png|BMP (*.bmp)|*.bmp|JPEG (*.jpg,*.jpeg,*.jpe,*.jff,*.jfif,*.jif,*.jtf)|*.jpg;*.jpeg;*.jpe;*.jff;*.jfif;*.jif;*.jtf|All files(*.*)|*.*");

	wxFileDialog *openFileDialog = new wxFileDialog (this, "Open file", "", "", FILETYPES, wxOPEN|wxFD_MULTIPLE, wxDefaultPosition);

	if (openFileDialog->ShowModal() == wxID_OK) {
		filenames.Clear(); /* clear any old stuff in case they cancelled something earlier */
		openFileDialog->GetPaths(filenames);
//		filename.Replace("\\","/");
		if (filenames.Count() > 1)
			txtFilename->SetValue("{multiple}");
		else
			txtFilename->SetValue(filenames[0]);
	}
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_IS_MOSAIC
 */

void FileOpenDialog::OnIsMosaicClick( wxCommandEvent& event )
{
	if (chkIsMosaic->GetValue()) {
		spinXSize->Enable();
		spinYSize->Enable();
		spinNumberOfSlices->Enable();
	}
	else {
		spinXSize->Disable();
		spinYSize->Disable();
		spinNumberOfSlices->Disable();
	}
}


/*!
 * Should we show tooltips?
 */

bool FileOpenDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap FileOpenDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin FileOpenDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end FileOpenDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon FileOpenDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin FileOpenDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end FileOpenDialog icon retrieval
}


// --------------------------------------------------------
// ---------- GetFilePaths --------------------------------
// --------------------------------------------------------
wxArrayString FileOpenDialog::GetFilePaths(void)
{
	return filenames;
}


// --------------------------------------------------------
// ---------- GetFilePrefix -------------------------------
// --------------------------------------------------------
wxString FileOpenDialog::GetFilePrefix(void)
{
	return filePrefix;
}


int FileOpenDialog::GetLoadType(void)
{
	if (radioOpenAll->GetValue()) return OPEN_ALL;
	if (radioOpenAllFilesWith->GetValue()) return OPEN_SELECTED;
	if (radioOpenSingleFile->GetValue()) return OPEN_SINGLE;

	return 0;
}


bool FileOpenDialog::GetIsMosaic(void)
{
	if (chkIsMosaic->GetValue())
		return true;
	else
		return false;
}


int FileOpenDialog::GetMosaicXSize()
{
	return spinXSize->GetValue();
}


int FileOpenDialog::GetMosaicYSize()
{
	return spinYSize->GetValue();
}


int FileOpenDialog::GetMosaicNumSlices()
{
	return spinNumberOfSlices->GetValue();
}


int FileOpenDialog::GetFileType(void)
{
	filename = filenames[0];
	if (radioFileTypeAnalyze->GetValue())
		return FILETYPE_ANALYZE;
	if (radioFileTypeAuto->GetValue()) {
		if ((filename.Right(4) == ".dcm") || (filename.Right(4) == ".acr"))
			return FILETYPE_DICOM;
		if ((filename.Right(4) == ".img") || (filename.Right(4) == ".hdr") || (filename.Right(4) == ".nii"))
			return FILETYPE_ANALYZE;
		if ((filename.Right(4) == ".jpg") || (filename.Right(5) == ".jpeg") || (filename.Right(4) == ".jpe"))
			return FILETYPE_RASTER;
		if ((filename.Right(4) == ".jff") || (filename.Right(5) == ".jfif") || (filename.Right(4) == ".jif"))
			return FILETYPE_RASTER;
		if ((filename.Right(4) == ".gif") || (filename.Right(4) == ".bmp") || (filename.Right(4) == ".xpm"))
			return FILETYPE_RASTER;
		if ((filename.Right(4) == ".tif") || (filename.Right(5) == ".tiff") || (filename.Right(4) == ".png"))
			return FILETYPE_RASTER;
		if ((filename.Right(4) == ".pcx") || (filename.Right(4) == ".pnm") || (filename.Right(4) == ".ico"))
			return FILETYPE_RASTER;
		if ((filename.Right(4) == ".cur") || (filename.Right(4) == ".ani") || (filename.Right(4) == ".jtf"))
			return FILETYPE_RASTER;
	}
	if (radioFileTypeDicom->GetValue())
		return FILETYPE_DICOM;
	if (radioFileTypeImage->GetValue()) {
		if ((filename.Right(4) == ".jpg") || (filename.Right(5) == ".jpeg") || (filename.Right(4) == ".jpe"))
			return FILETYPE_RASTER;
		if ((filename.Right(4) == ".jff") || (filename.Right(5) == ".jfif") || (filename.Right(4) == ".jif"))
			return FILETYPE_RASTER;
		if ((filename.Right(4) == ".gif") || (filename.Right(4) == ".bmp") || (filename.Right(4) == ".xpm"))
			return FILETYPE_RASTER;
		if ((filename.Right(4) == ".tif") || (filename.Right(5) == ".tiff") || (filename.Right(4) == ".png"))
			return FILETYPE_RASTER;
		if ((filename.Right(4) == ".pcx") || (filename.Right(4) == ".pnm") || (filename.Right(4) == ".ico"))
			return FILETYPE_RASTER;
		if ((filename.Right(4) == ".cur") || (filename.Right(4) == ".ani") || (filename.Right(4) == ".jtf"))
			return FILETYPE_RASTER;
	}

	return FILETYPE_UNKNOWN;
}
