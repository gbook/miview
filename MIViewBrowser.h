/* ----------------------------------------------------------------------------
	MIViewBrowser.h


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



#ifndef _MIVIEWBROWSER_H_
#define _MIVIEWBROWSER_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "MIViewBrowser.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/aui/framemanager.h"
#include "wx/frame.h"
#include "wx/html/htmlwin.h"
#include "wx/treectrl.h"
#include "wx/statline.h"
////@end includes

//#include "LogWindow.h"
#include "MainFrameVariables.h"
#include "MIViewBrowserDB.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxHtmlWindow;
class wxTreeCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_MIVIEWBROWSER 25000
#define ID_PREVIEW_PANEL 25004
#define ID_INFORMATION_PANEL 25003
#define ID_LISTING_PANEL 25007
#define ID_SCAN_RECURSIVELY 25008
#define ID_SCAN 25011
#define ID_SCAN_DIR 25009
#define ID_SELECT_SCAN_DIR 25010
#define ID_FILELIST_TREE 25005
#define ID_EXPAND_ALL 25012
#define ID_EXPAND_ALL_PATIENTS 25013
#define ID_COLLAPSE_ALL 25014
#define ID_OPEN_SERIES 10000
#define ID_OPEN_FILE 10001
#define SYMBOL_MIVIEWBROWSER_STYLE wxDEFAULT_FRAME_STYLE|wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMAXIMIZE_BOX|wxCLOSE_BOX
#define SYMBOL_MIVIEWBROWSER_TITLE _("MIViewBrowser")
#define SYMBOL_MIVIEWBROWSER_IDNAME ID_MIVIEWBROWSER
#define SYMBOL_MIVIEWBROWSER_SIZE wxSize(650, 500)
#define SYMBOL_MIVIEWBROWSER_POSITION wxDefaultPosition
////@end control identifiers
#define ID_MENU_BROWSER_TREE 27000
#define ID_MENU_OPEN_SERIES 27001
#define ID_MENU_OPEN_FILE 27002

#define TREE_TYPE_PATIENT 27500
#define TREE_TYPE_STUDY 27501
#define TREE_TYPE_SERIES 27502
#define TREE_TYPE_FILE 27503

/*!
 * MIViewBrowser class declaration
 */

class MIViewBrowser: public wxFrame
{    
    DECLARE_CLASS( MIViewBrowser )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    MIViewBrowser();
    MIViewBrowser( wxWindow* parent, wxWindowID id = SYMBOL_MIVIEWBROWSER_IDNAME, const wxString& caption = SYMBOL_MIVIEWBROWSER_TITLE, const wxPoint& pos = SYMBOL_MIVIEWBROWSER_POSITION, const wxSize& size = SYMBOL_MIVIEWBROWSER_SIZE, long style = SYMBOL_MIVIEWBROWSER_STYLE);

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_MIVIEWBROWSER_IDNAME, const wxString& caption = SYMBOL_MIVIEWBROWSER_TITLE, const wxPoint& pos = SYMBOL_MIVIEWBROWSER_POSITION, const wxSize& size = SYMBOL_MIVIEWBROWSER_SIZE, long style = SYMBOL_MIVIEWBROWSER_STYLE );

    /// Destructor
    ~MIViewBrowser();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

	void SetPointers(MainFrame *Frame, MainFrameVariables *MainVars);

	void RecreateDB();
	void ReloadDB();
	void DisplayPatientHTML(wxTreeItemId id);
	void DisplayStudyHTML(wxTreeItemId id);
	void DisplaySeriesHTML(wxTreeItemId id);
	void DisplayFileHTML(wxTreeItemId id);
	wxArrayString GetChildList(wxTreeItemId id);
	wxString ParseDateAndTime(wxString date, wxString time);
	wxString ParseDate(wxString date);
	void ShowContextMenu(const wxPoint& pos);
	void ExpandAllPatients();
	void ExpandAllPatientsRecursive();
	void CollapseAllPatients();

	//bool IsPatientItem(wxTreeItemId id);
	//bool IsStudyItem(wxTreeItemId id);
	//bool IsSeriesItem(wxTreeItemId id);
	//bool IsFileItem(wxTreeItemId id);
	int GetItemType(wxTreeItemId id);

////@begin MIViewBrowser event handler declarations

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_CLOSE
    void OnCloseClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SCAN
    void OnScanClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SELECT_SCAN_DIR
    void OnSelectScanDirClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_TREE_SEL_CHANGED event handler for ID_FILELIST_TREE
    void OnFilelistTreeSelChanged( wxTreeEvent& event );

    /// wxEVT_COMMAND_TREE_ITEM_ACTIVATED event handler for ID_FILELIST_TREE
    void OnFilelistTreeItemActivated( wxTreeEvent& event );

    /// wxEVT_RIGHT_DOWN event handler for ID_FILELIST_TREE
    void OnRightDown( wxMouseEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_EXPAND_ALL
    void OnExpandAllClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_EXPAND_ALL_PATIENTS
    void OnExpandAllPatientsClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_COLLAPSE_ALL
    void OnCollapseAllClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_OPEN_SERIES
    void OnOpenSeriesClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_OPEN_FILE
    void OnOpenFileClick( wxCommandEvent& event );

////@end MIViewBrowser event handler declarations

////@begin MIViewBrowser member function declarations

    /// Returns the AUI manager object
    wxAuiManager& GetAuiManager() { return m_auiManager; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end MIViewBrowser member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin MIViewBrowser member variables
    wxAuiManager m_auiManager;
    wxHtmlWindow* htmlInfo;
    wxPanel* pnlListing;
    wxCheckBox* chkScanRecursively;
    wxBitmapButton* btnScan;
    wxTextCtrl* txtScanDir;
    wxButton* btnSelectScanDir;
    wxTreeCtrl* treeFileList;
    wxButton* btnExpandAll;
    wxButton* btnExpandAllPatients;
    wxButton* btnCollapseAll;
    wxButton* btnOpenSeries;
    wxButton* btnOpenFile;
////@end MIViewBrowser member variables

	wxImageList *treeImageList;
	BrowserDB browsedb;
	wxTreeItemId rootID;

	//LogWindow *logWindow;
	MainFrameVariables *mainVars;
	MainFrame *frame;
};

#endif
    // _MIVIEWBROWSER_H_
