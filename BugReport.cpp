/* ----------------------------------------------------------------------------
	BugReport.cpp


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
#pragma implementation "BugReport.h"
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

#include "BugReport.h"

////@begin XPM images
////@end XPM images


/*!
 * BugReport type definition
 */

IMPLEMENT_DYNAMIC_CLASS( BugReport, wxDialog )


/*!
 * BugReport event table definition
 */

BEGIN_EVENT_TABLE( BugReport, wxDialog )

////@begin BugReport event table entries
////@end BugReport event table entries

END_EVENT_TABLE()


/*!
 * BugReport constructors
 */

BugReport::BugReport()
{
    Init();
}

BugReport::BugReport( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * BugReport creator
 */

bool BugReport::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin BugReport creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end BugReport creation
    return true;
}


/*!
 * BugReport destructor
 */

BugReport::~BugReport()
{
////@begin BugReport destruction
////@end BugReport destruction
}


/*!
 * Member initialisation
 */

void BugReport::Init()
{
////@begin BugReport member initialisation
////@end BugReport member initialisation
}


/*!
 * Control creation for BugReport
 */

void BugReport::CreateControls()
{    
////@begin BugReport content construction
    BugReport* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Found a bug?"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText3->SetFont(wxFont(10, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 10);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Report it to the developer:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText4, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);

    wxHyperlinkCtrl* itemHyperlinkCtrl5 = new wxHyperlinkCtrl( itemDialog1, ID_HYPERLINKCTRL3, _("gbook@gbook.org"), _T("mailto:gbook@gbook.org"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
    itemBoxSizer2->Add(itemHyperlinkCtrl5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);

    itemBoxSizer2->Add(5, 10, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("Include as much of the following information as possible:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText7->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer2->Add(itemStaticText7, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("* The steps taken to reproduce the problem\n* The version and build date of MIView\n* Your PC hardware specifications including\n     * CPU type and speed (eg Intel Core 2 Duo E6400)\n     * Video card model (eg NVidia 8600GTS)\n     * Video driver revision number (eg Forceware 169.28)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText8, 0, wxALIGN_LEFT|wxALL, 3);

    itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton10 = new wxButton( itemDialog1, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemButton10, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

////@end BugReport content construction
}


/*!
 * Should we show tooltips?
 */

bool BugReport::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap BugReport::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin BugReport bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end BugReport bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon BugReport::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin BugReport icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end BugReport icon retrieval
}
