/* ----------------------------------------------------------------------------
	BugReport.h


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


#ifndef _BUGREPORT_H_
#define _BUGREPORT_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "BugReport.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/hyperlink.h"
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
#define ID_BUGREPORT 20000
#define ID_HYPERLINKCTRL3 20001
#define SYMBOL_BUGREPORT_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_MODAL|wxTAB_TRAVERSAL
#define SYMBOL_BUGREPORT_TITLE _("Bug Report")
#define SYMBOL_BUGREPORT_IDNAME ID_BUGREPORT
#define SYMBOL_BUGREPORT_SIZE wxSize(400, 300)
#define SYMBOL_BUGREPORT_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * BugReport class declaration
 */

class BugReport: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( BugReport )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    BugReport();
    BugReport( wxWindow* parent, wxWindowID id = SYMBOL_BUGREPORT_IDNAME, const wxString& caption = SYMBOL_BUGREPORT_TITLE, const wxPoint& pos = SYMBOL_BUGREPORT_POSITION, const wxSize& size = SYMBOL_BUGREPORT_SIZE, long style = SYMBOL_BUGREPORT_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_BUGREPORT_IDNAME, const wxString& caption = SYMBOL_BUGREPORT_TITLE, const wxPoint& pos = SYMBOL_BUGREPORT_POSITION, const wxSize& size = SYMBOL_BUGREPORT_SIZE, long style = SYMBOL_BUGREPORT_STYLE );

    /// Destructor
    ~BugReport();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin BugReport event handler declarations

////@end BugReport event handler declarations

////@begin BugReport member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end BugReport member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin BugReport member variables
////@end BugReport member variables
};

#endif
    // _BUGREPORT_H_
