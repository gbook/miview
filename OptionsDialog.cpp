/* ----------------------------------------------------------------------------
	OptionsDialog.cpp


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
#pragma implementation "OptionsDialog.h"
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

#include "OptionsDialog.h"

////@begin XPM images
////@end XPM images


/*!
 * OptionsDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( OptionsDialog, wxDialog )


/*!
 * OptionsDialog event table definition
 */

BEGIN_EVENT_TABLE( OptionsDialog, wxDialog )

////@begin OptionsDialog event table entries
////@end OptionsDialog event table entries

END_EVENT_TABLE()


/*!
 * OptionsDialog constructors
 */

OptionsDialog::OptionsDialog()
{
    Init();
}

OptionsDialog::OptionsDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * OptionsDialog creator
 */

bool OptionsDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin OptionsDialog creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end OptionsDialog creation
    return true;
}


/*!
 * OptionsDialog destructor
 */

OptionsDialog::~OptionsDialog()
{
////@begin OptionsDialog destruction
////@end OptionsDialog destruction
}


/*!
 * Member initialisation
 */

void OptionsDialog::Init()
{
////@begin OptionsDialog member initialisation
    nbkOptions = NULL;
    pnlGeneral = NULL;
    chkUseSI = NULL;
    radOpenSingle = NULL;
    radOpenAll = NULL;
    radOpenPrefix = NULL;
    pnlFileAssoc = NULL;
    chkDicom = NULL;
    chkAnalyze = NULL;
    chkNifti = NULL;
////@end OptionsDialog member initialisation
}


/*!
 * Control creation for OptionsDialog
 */

void OptionsDialog::CreateControls()
{    
////@begin OptionsDialog content construction
    OptionsDialog* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    nbkOptions = new wxNotebook( itemDialog1, ID_NOTEBOOK_OPTIONS, wxDefaultPosition, wxDefaultSize, wxNB_DEFAULT|wxNB_TOP );

    pnlGeneral = new wxPanel( nbkOptions, ID_PANEL_GENERAL, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    pnlGeneral->SetSizer(itemBoxSizer5);

    chkUseSI = new wxCheckBox( pnlGeneral, ID_CHECK_USE_SI, _("Use S,I (Superior/Inferior) orientation labels instead of H,F (Head/Feet)"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE );
    chkUseSI->SetValue(false);
    itemBoxSizer5->Add(chkUseSI, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    itemBoxSizer5->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer8Static = new wxStaticBox(pnlGeneral, wxID_ANY, _("Default Action for Opening Files"));
    wxStaticBoxSizer* itemStaticBoxSizer8 = new wxStaticBoxSizer(itemStaticBoxSizer8Static, wxVERTICAL);
    itemBoxSizer5->Add(itemStaticBoxSizer8, 0, wxALIGN_LEFT|wxALL, 5);
    radOpenSingle = new wxRadioButton( pnlGeneral, ID_RADIO_OPENSINGLE, _("Open single image"), wxDefaultPosition, wxDefaultSize, 0 );
    radOpenSingle->SetValue(false);
    itemStaticBoxSizer8->Add(radOpenSingle, 0, wxALIGN_LEFT|wxALL, 5);

    radOpenAll = new wxRadioButton( pnlGeneral, ID_RADIO_OPENALLIMAGES, _("Open all images"), wxDefaultPosition, wxDefaultSize, 0 );
    radOpenAll->SetValue(false);
    itemStaticBoxSizer8->Add(radOpenAll, 0, wxALIGN_LEFT|wxALL, 5);

    radOpenPrefix = new wxRadioButton( pnlGeneral, ID_RADIO_OPENPREFIX, _("Open all images with closest prefix"), wxDefaultPosition, wxDefaultSize, 0 );
    radOpenPrefix->SetValue(false);
    itemStaticBoxSizer8->Add(radOpenPrefix, 0, wxALIGN_LEFT|wxALL, 5);

    nbkOptions->AddPage(pnlGeneral, _("General"));

    pnlFileAssoc = new wxPanel( nbkOptions, ID_PANEL_FILE_ASSOC, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxVERTICAL);
    pnlFileAssoc->SetSizer(itemBoxSizer13);

    wxStaticText* itemStaticText14 = new wxStaticText( pnlFileAssoc, wxID_STATIC, _("Associate MIView with the following filetypes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText14->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer13->Add(itemStaticText14, 0, wxALIGN_LEFT|wxALL|wxADJUST_MINSIZE, 5);

    chkDicom = new wxCheckBox( pnlFileAssoc, ID_CHECK_DICOM, _(".dcm .acr (Dicom)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkDicom->SetValue(false);
    itemBoxSizer13->Add(chkDicom, 0, wxALIGN_LEFT|wxALL, 5);

    chkAnalyze = new wxCheckBox( pnlFileAssoc, ID_CHECK_ANALYZE, _(".img .hdr (Analyze)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkAnalyze->SetValue(false);
    itemBoxSizer13->Add(chkAnalyze, 0, wxALIGN_LEFT|wxALL, 5);

    chkNifti = new wxCheckBox( pnlFileAssoc, ID_CHECK_NIFTI, _(".nii (Nifti)"), wxDefaultPosition, wxDefaultSize, 0 );
    chkNifti->SetValue(false);
    itemBoxSizer13->Add(chkNifti, 0, wxALIGN_LEFT|wxALL, 5);

    nbkOptions->AddPage(pnlFileAssoc, _("File Associations"));

    itemBoxSizer2->Add(nbkOptions, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer18 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer18, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton19 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer18->AddButton(itemButton19);

    wxButton* itemButton20 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer18->AddButton(itemButton20);

    itemStdDialogButtonSizer18->Realize();

////@end OptionsDialog content construction
}


/*!
 * Should we show tooltips?
 */

bool OptionsDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap OptionsDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin OptionsDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end OptionsDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon OptionsDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin OptionsDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end OptionsDialog icon retrieval
}
