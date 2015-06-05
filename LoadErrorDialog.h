/* ----------------------------------------------------------------------------
	LoadErrorDialog.h


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



// begin forward declarations
class wxBoxSizer;

// begin control identifiers
//#define LOADERROR_DIALOG_ID 10000
#define SYMBOL_LOADERRORDIALOG_STYLE wxDEFAULT_DIALOG_STYLE|wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_MODAL|wxSIMPLE_BORDER
//#define SYMBOL_LOADERRORDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_LOADERRORDIALOG_TITLE _("MIView Options")
#define SYMBOL_LOADERRORDIALOG_IDNAME LOADERROR_DIALOG_ID
#define SYMBOL_LOADERRORDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_LOADERRORDIALOG_POSITION wxDefaultPosition
#define ID_RADIO_RELOAD 10006
#define ID_RADIO_NOLOAD 10002
#define ID_OK 10004
// end control identifiers


class LoadErrorDialog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( LoadErrorDialog )
    DECLARE_EVENT_TABLE()

public:
	bool GetContinueAnswer();
	void SetErrorMessage(wxString msg);
	// Constructors
	LoadErrorDialog();
	LoadErrorDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& caption = "File Load Error", const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE, wxString errMsg = "An error occured");
	~LoadErrorDialog();

	// Creation
	bool Create( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& caption = "File Load Error", const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);

	void CreateControls();
	void OnOkClick( wxCommandEvent& event );

	wxBitmap GetBitmapResource( const wxString& name );
	wxIcon GetIconResource( const wxString& name );

	static bool ShowToolTips();

private:
	wxString errorMessage;

    wxBoxSizer* mainSizer;
    wxBoxSizer* messageSizer;
    wxStaticText* txtErrorMessage;
	wxStaticBitmap* staticBitmap;
    wxRadioButton* reloadRadio;
    wxRadioButton* noReloadRadio;
    wxButton* okButton;
};
