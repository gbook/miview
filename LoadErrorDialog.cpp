/* ----------------------------------------------------------------------------
	LoadErrorDialog.cpp


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


#include "MainFrame.h"
#include "LoadErrorDialog.h"
#include "xpm/exclamation.xpm"


IMPLEMENT_DYNAMIC_CLASS( LoadErrorDialog, wxDialog )

BEGIN_EVENT_TABLE( LoadErrorDialog, wxDialog )
    EVT_BUTTON( ID_OK, LoadErrorDialog::OnOkClick )
END_EVENT_TABLE()



// --------------------------------------------------------
// ---------- LoadErrorDialog -----------------------------
// --------------------------------------------------------
LoadErrorDialog::LoadErrorDialog( )
{
}

// --------------------------------------------------------
// ---------- LoadErrorDialog -----------------------------
// --------------------------------------------------------
LoadErrorDialog::LoadErrorDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style, wxString errMsg)
{
    Create(parent, id, caption, pos, size, style);
	errorMessage = errMsg;
}

// --------------------------------------------------------
// ---------- ~LoadErrorDialog ----------------------------
// --------------------------------------------------------
LoadErrorDialog::~LoadErrorDialog( )
{
}

bool LoadErrorDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    mainSizer = NULL;
    messageSizer = NULL;
    txtErrorMessage = NULL;
    reloadRadio = NULL;
    noReloadRadio = NULL;
    okButton = NULL;

    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    if (!wxDialog::Create( parent, id, caption, pos, size, style ))
		return false;

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();

    return true;
}


// --------------------------------------------------------
// ---------- CreateControls ------------------------------
// --------------------------------------------------------
void LoadErrorDialog::CreateControls()
{    
    mainSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(mainSizer);

    messageSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(messageSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBitmap staticBitmapBitmap(this->GetBitmapResource(wxT("xpm/exclamation.xpm")));
    staticBitmap = new wxStaticBitmap( this, wxID_STATIC, staticBitmapBitmap, wxDefaultPosition, wxSize(32, 32), 0 );
    messageSizer->Add(staticBitmap, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	/* the error message */
    txtErrorMessage = new wxStaticText( this, wxID_STATIC, errorMessage, wxDefaultPosition, wxDefaultSize, 0 );
    messageSizer->Add(txtErrorMessage, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

	/* radio buttons */
    reloadRadio = new wxRadioButton( this, ID_RADIO_RELOAD, _("Load this data anyway. (It may not look correct)"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
    reloadRadio->SetValue(false);
    mainSizer->Add(reloadRadio, 0, wxALIGN_LEFT|wxALL, 3);
    noReloadRadio = new wxRadioButton( this, ID_RADIO_NOLOAD, _("Don't load this data"), wxDefaultPosition, wxDefaultSize);
    noReloadRadio->SetValue(true);
    mainSizer->Add(noReloadRadio, 0, wxALIGN_LEFT|wxALL, 3);

	/* spacer */
    mainSizer->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    okButton = new wxButton( this, ID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
    mainSizer->Add(okButton, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
}


// --------------------------------------------------------
// ---------- ShowToolTips --------------------------------
// --------------------------------------------------------
bool LoadErrorDialog::ShowToolTips()
{
    return true;
}

// --------------------------------------------------------
// ---------- GetBitmapResource ---------------------------
// --------------------------------------------------------
wxBitmap LoadErrorDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
    wxUnusedVar(name);
    if (name == _T("xpm/exclamation.xpm"))
    {
        wxBitmap bitmap( exclamation_xpm);
        return bitmap;
    }
    return wxNullBitmap;
}

// --------------------------------------------------------
// ---------- GetIconResource -----------------------------
// --------------------------------------------------------
wxIcon LoadErrorDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
}


// --------------------------------------------------------
// ---------- OnOkClick -----------------------------------
// --------------------------------------------------------
void LoadErrorDialog::OnOkClick( wxCommandEvent& event )
{
    // Before editing this code, remove the block markers.
    EndModal(wxID_OK);
}


// --------------------------------------------------------
// ---------- SetErrorMessage -----------------------------
// --------------------------------------------------------
void LoadErrorDialog::SetErrorMessage(wxString msg)
{
//	errorMessage = msg;
}


// --------------------------------------------------------
// ---------- GetContinueAnswer ---------------------------
// --------------------------------------------------------
bool LoadErrorDialog::GetContinueAnswer()
{
    return reloadRadio->GetValue();
}
