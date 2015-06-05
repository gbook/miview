/* ----------------------------------------------------------------------------
	SaveScreenshot.cpp


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
#pragma implementation "SaveScreenshot.h"
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

#include "SaveScreenshot.h"
#include <wx/filename.h>

////@begin XPM images
////@end XPM images


/*!
 * SaveScreenshot type definition
 */

IMPLEMENT_DYNAMIC_CLASS( SaveScreenshot, wxDialog )


/*!
 * SaveScreenshot event table definition
 */

BEGIN_EVENT_TABLE( SaveScreenshot, wxDialog )

////@begin SaveScreenshot event table entries
    EVT_BUTTON( ID_SELECT_FILENAME, SaveScreenshot::OnSelectFilenameClick )

////@end SaveScreenshot event table entries

END_EVENT_TABLE()


/*!
 * SaveScreenshot constructors
 */

SaveScreenshot::SaveScreenshot()
{
    Init();
}

SaveScreenshot::SaveScreenshot( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * SaveScreenshot creator
 */

bool SaveScreenshot::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin SaveScreenshot creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end SaveScreenshot creation
    return true;
}


/*!
 * SaveScreenshot destructor
 */

SaveScreenshot::~SaveScreenshot()
{
////@begin SaveScreenshot destruction
////@end SaveScreenshot destruction
}


/*!
 * Member initialisation
 */

void SaveScreenshot::Init()
{
////@begin SaveScreenshot member initialisation
    txtFilename = NULL;
    btnSelectFilename = NULL;
    radBMP = NULL;
    radJPEG = NULL;
    radPNG = NULL;
    radTIFF = NULL;
////@end SaveScreenshot member initialisation
}


/*!
 * Control creation for SaveScreenshot
 */

void SaveScreenshot::CreateControls()
{    
////@begin SaveScreenshot content construction
    SaveScreenshot* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Save screenshot as"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtFilename = new wxTextCtrl( itemDialog1, ID_FILENAME_SAVE, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0 );
    itemBoxSizer3->Add(txtFilename, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    btnSelectFilename = new wxButton( itemDialog1, ID_SELECT_FILENAME, _("..."), wxDefaultPosition, wxSize(25, -1), 0 );
    itemBoxSizer3->Add(btnSelectFilename, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxStaticLine* itemStaticLine7 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer2->Add(itemStaticLine7, 0, wxGROW|wxALL, 0);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer8, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText9 = new wxStaticText( itemDialog1, wxID_STATIC, _("Filetype"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer8->Add(itemStaticText9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer8->Add(itemBoxSizer10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    radBMP = new wxRadioButton( itemDialog1, ID_FILETYPE_BMP, _("Bitmap"), wxDefaultPosition, wxDefaultSize, 0 );
    radBMP->SetValue(false);
    itemBoxSizer10->Add(radBMP, 0, wxALIGN_LEFT|wxALL, 2);

    radJPEG = new wxRadioButton( itemDialog1, ID_FILETYPE_JPEG, _("Jpeg"), wxDefaultPosition, wxDefaultSize, 0 );
    radJPEG->SetValue(false);
    itemBoxSizer10->Add(radJPEG, 0, wxALIGN_LEFT|wxALL, 2);

    radPNG = new wxRadioButton( itemDialog1, ID_FILETYPE_PNG, _("PNG"), wxDefaultPosition, wxDefaultSize, 0 );
    radPNG->SetValue(true);
    itemBoxSizer10->Add(radPNG, 0, wxALIGN_LEFT|wxALL, 2);

    radTIFF = new wxRadioButton( itemDialog1, ID_FILETYPE_TIFF, _("TIFF"), wxDefaultPosition, wxDefaultSize, 0 );
    radTIFF->SetValue(false);
    itemBoxSizer10->Add(radTIFF, 0, wxALIGN_LEFT|wxALL, 2);

    itemBoxSizer8->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton16 = new wxButton( itemDialog1, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer8->Add(itemButton16, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end SaveScreenshot content construction
}


/*!
 * Should we show tooltips?
 */

bool SaveScreenshot::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap SaveScreenshot::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin SaveScreenshot bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end SaveScreenshot bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon SaveScreenshot::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin SaveScreenshot icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end SaveScreenshot icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SELECT_FILENAME
 */

void SaveScreenshot::OnSelectFilenameClick( wxCommandEvent& event )
{
	static const wxChar	*FILETYPES = _T( "All files(*.*)|*.*");
	wxString filepath;

	wxFileDialog *saveFileDialog = new wxFileDialog(this, "Save File As...", "", "", FILETYPES, wxFD_SAVE|wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

	if (saveFileDialog->ShowModal() == wxID_OK) {
		filepath = saveFileDialog->GetPath();
		txtFilename->SetValue(filepath);
	}
}


wxString SaveScreenshot::GetFilePath()
{
	wxFileName filepath = wxFileName(txtFilename->GetValue());
	if (radBMP->GetValue())
		filepath.SetExt("bmp");
	if (radJPEG->GetValue())
		filepath.SetExt("jpg");
	if (radPNG->GetValue())
		filepath.SetExt("png");
	if (radTIFF->GetValue())
		filepath.SetExt("tiff");

	return filepath.GetFullPath();
}


int SaveScreenshot::GetFiletype()
{
	if (radBMP->GetValue())
		return wxBITMAP_TYPE_BMP;
	if (radJPEG->GetValue())
		return wxBITMAP_TYPE_JPEG;
	if (radPNG->GetValue())
		return wxBITMAP_TYPE_PNG;
	if (radTIFF->GetValue())
		return wxBITMAP_TYPE_TIF;
	
	return 0;
}
