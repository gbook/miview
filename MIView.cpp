/* ----------------------------------------------------------------------------
	MIView.cpp


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


//#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
//#pragma implementation "miview.h"
//#endif

// For compilers that support precompilation, includes "wx/wx.h".
//#include "wx/wxprec.h"

//#ifdef __BORLANDC__
//#pragma hdrstop
//#endif

//#ifndef WX_PRECOMP
//#include <wx/wx.h>
//#endif

////@begin includes
////@end includes

#include "miview.h"

////@begin XPM images

////@end XPM images

/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( MIViewApp )
////@end implement app

/*!
 * MIViewApp type definition
 */

IMPLEMENT_CLASS( MIViewApp, wxApp )

/*!
 * MIViewApp event table definition
 */

BEGIN_EVENT_TABLE( MIViewApp, wxApp )

////@begin MIViewApp event table entries
////@end MIViewApp event table entries

END_EVENT_TABLE()

/*!
 * Constructor for MIViewApp
 */

MIViewApp::MIViewApp()
{
////@begin MIViewApp member initialisation
////@end MIViewApp member initialisation
}

/*!
 * Initialisation for MIViewApp
 */

bool MIViewApp::OnInit()
{
	freopen ("logfile-stderr.txt","w",stderr);
	freopen ("logfile-stdout.txt","w",stdout);

//@begin MIViewApp initialisation
    // Remove the comment markers above and below this block
    // to make permanent changes to the code.

#if wxUSE_XPM
    wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
    wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
    wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
    wxImage::AddHandler(new wxGIFHandler);
#endif
#if wxUSE_LIBTIFF
    wxImage::AddHandler(new wxTIFFHandler);
#endif
    MainFrame* mainWindow = new MainFrame( NULL, ID_MAINFRAME );
    mainWindow->Show(true);
//@end MIViewApp initialisation

	/* parse the command line */
//	if (argc > 1) {
//		wxString filepath = argv[1];
		//mainWindow->OpenFile(filepath);
//		WriteLog(filepath);
//		mainWindow->openArgs = filepath;
//	}

    return true;
}

/*!
 * Cleanup for MIViewApp
 */
int MIViewApp::OnExit()
{
	fclose(stdout);
	fclose(stderr);
 //   delete wxXPMHandler;
	//delete wxPNGHandler;
	//delete wxJPEGHandler;
	//delete wxGIFHandler;
	//delete wxTIFFHandler;

////@begin MIViewApp cleanup
    return wxApp::OnExit();
////@end MIViewApp cleanup
}

