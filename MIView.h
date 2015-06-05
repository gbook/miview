/* ----------------------------------------------------------------------------
	MIView.h


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



#ifndef _MIVIEW_H_
#define _MIVIEW_H_

//#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
//#pragma interface "MIView.h"
//#endif

/*!
 * Includes
 */
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

////@begin includes
#include "wx/image.h"
#include "MainFrame.h"
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
////@end control identifiers

/*!
 * MIViewApp class declaration
 */

class MIViewApp: public wxApp
{    
    DECLARE_CLASS( MIViewApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    MIViewApp();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin MIViewApp event handler declarations
////@end MIViewApp event handler declarations

////@begin MIViewApp member function declarations
////@end MIViewApp member function declarations

////@begin MIViewApp member variables
////@end MIViewApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(MIViewApp)
////@end declare app

#endif
    // _MIVIEW_H_
