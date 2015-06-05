/* ----------------------------------------------------------------------------
	AboutDialog.h


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



#ifndef _ABOUTDIALOG_H_
#define _ABOUTDIALOG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "AboutDialog.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/statline.h"
#include "wx/hyperlink.h"
#include "wx/richtext/richtextctrl.h"
#include "wx/html/htmlwin.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxHtmlWindow;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_ABOUT_DIALOG 15000
#define ID_PANEL2 15004
#define ID_HYPERLINKCTRL2 15001
#define ID_HYPERLINKCTRL1 15002
#define ID_RICHTEXTCTRL1 15005
#define ID_HTMLWINDOW 15003
#define ID_HYPERLINK_WX 15006
#define ID_HYPERLINK_GDCM 15007
#define ID_HYPERLINK_AFNI 15008
#define ID_HYPERLINK_GLSL 15009
#define ID_HYPERLINKCTRL 15016
#define SYMBOL_ABOUTDIALOG_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_ABOUTDIALOG_TITLE _("About MIView")
#define SYMBOL_ABOUTDIALOG_IDNAME ID_ABOUT_DIALOG
#define SYMBOL_ABOUTDIALOG_SIZE wxSize(500, -1)
#define SYMBOL_ABOUTDIALOG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * AboutDialog class declaration
 */

class AboutDialog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( AboutDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    AboutDialog( );
    AboutDialog( wxWindow* parent, wxWindowID id = SYMBOL_ABOUTDIALOG_IDNAME, const wxString& caption = SYMBOL_ABOUTDIALOG_TITLE, const wxPoint& pos = SYMBOL_ABOUTDIALOG_POSITION, const wxSize& size = SYMBOL_ABOUTDIALOG_SIZE, long style = SYMBOL_ABOUTDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ABOUTDIALOG_IDNAME, const wxString& caption = SYMBOL_ABOUTDIALOG_TITLE, const wxPoint& pos = SYMBOL_ABOUTDIALOG_POSITION, const wxSize& size = SYMBOL_ABOUTDIALOG_SIZE, long style = SYMBOL_ABOUTDIALOG_STYLE );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin AboutDialog event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

////@end AboutDialog event handler declarations

////@begin AboutDialog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end AboutDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin AboutDialog member variables
    wxStaticText* txtBuildDate;
    wxStaticText* lblVideoCard;
    wxStaticText* lblGLVersion;
    wxStaticText* lblDisplay;
    wxStaticText* lblOSVersion;
    wxStaticText* lblPlatform;
    wxHtmlWindow* htmlWindow;
////@end AboutDialog member variables
	void CheckOpenGL(void);
};

#endif
    // _ABOUTDIALOG_H_
