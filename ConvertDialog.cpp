/* ----------------------------------------------------------------------------
	ConvertDialog.cpp


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
#pragma implementation "ConvertDialog.h"
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
#include "wx/imaglist.h"
////@end includes

#include "MIView.h"
#include "MainFrameVariables.h"
#include "ImageData.h"
//#include "LogWindow.h"
#include "ConvertDialog.h"
#include <wx/filename.h>

////@begin XPM images
////@end XPM images


/*!
 * ConvertDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ConvertDialog, wxDialog )


/*!
 * ConvertDialog event table definition
 */

BEGIN_EVENT_TABLE( ConvertDialog, wxDialog )

////@begin ConvertDialog event table entries
    EVT_RADIOBUTTON( ID_INTYPE_RASTER, ConvertDialog::OnIntypeRasterSelected )

    EVT_RADIOBUTTON( ID_INTYPE_2DDICOM, ConvertDialog::OnIntype2ddicomSelected )

    EVT_RADIOBUTTON( ID_INTYPE_2DDICOMMOSAIC, ConvertDialog::OnIntype2ddicommosaicSelected )

    EVT_RADIOBUTTON( ID_INTYPE_3DDICOM, ConvertDialog::OnIntype3ddicomSelected )

    EVT_RADIOBUTTON( ID_INTYPE_3DNIFTI, ConvertDialog::OnIntype3dniftiSelected )

    EVT_RADIOBUTTON( ID_INTYPE_4DNIFTI, ConvertDialog::OnIntype4dniftiSelected )

    EVT_BUTTON( ID_CLEAR_FILES, ConvertDialog::OnClearFilesClick )

    EVT_BUTTON( ID_REMOVE_FILES, ConvertDialog::OnRemoveFilesClick )

    EVT_BUTTON( ID_CONVERT_ADD_FILES, ConvertDialog::OnConvertAddFilesClick )

    EVT_RADIOBUTTON( ID_OUTTYPE_RASTER, ConvertDialog::OnOuttypeRasterSelected )

    EVT_RADIOBUTTON( ID_OUTTYPE_2DDICOM, ConvertDialog::OnOuttype2ddicomSelected )

    EVT_RADIOBUTTON( ID_OUTTYPE_2DDICOMMOS, ConvertDialog::OnOuttype2ddicommosSelected )

    EVT_RADIOBUTTON( ID_OUTTYPE_3DDICOM, ConvertDialog::OnOuttype3ddicomSelected )

    EVT_RADIOBUTTON( ID_OUTTYPE_3DNIFTI, ConvertDialog::OnOuttype3dniftiSelected )

    EVT_RADIOBUTTON( ID_OUTTYPE_4DNIFTI, ConvertDialog::OnOuttype4dniftiSelected )

    EVT_CHECKBOX( ID_ORIGINAL_IS_INTERLEAVED, ConvertDialog::OnOriginalIsInterleavedClick )

    EVT_BUTTON( ID_SELECT_DIR, ConvertDialog::OnSelectDirClick )

    EVT_BUTTON( ID_CONVERT, ConvertDialog::OnConvertClick )

////@end ConvertDialog event table entries

END_EVENT_TABLE()


/*!
 * ConvertDialog constructors
 */

ConvertDialog::ConvertDialog()
{
    Init();
}

ConvertDialog::ConvertDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


// --------------------------------------------------------
// ---------- Create --------------------------------------
// --------------------------------------------------------
bool ConvertDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin ConvertDialog creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end ConvertDialog creation
    return true;
}


void ConvertDialog::SetLogWindow(MainFrameVariables *MainVars)
{
	//logWindow = logwind;
	mainVars = MainVars;
}


// --------------------------------------------------------
// ---------- ~ConvertDialog ------------------------------
// --------------------------------------------------------
ConvertDialog::~ConvertDialog()
{
////@begin ConvertDialog destruction
////@end ConvertDialog destruction
}


// --------------------------------------------------------
// ---------- Init ----------------------------------------
// --------------------------------------------------------
void ConvertDialog::Init()
{
////@begin ConvertDialog member initialisation
    radInRaster = NULL;
    radIn2DDicom = NULL;
    radIn2DDicomMosaic = NULL;
    radIn3DDicom = NULL;
    radIn3DNifti = NULL;
    radIn4DNifti = NULL;
    btnClearFiles = NULL;
    btnRemoveFiles = NULL;
    btnAddFiles = NULL;
    lstFiles = NULL;
    radOutRaster = NULL;
    radOut2DDicom = NULL;
    radOut2DDicomMosaic = NULL;
    radOut3DDicom = NULL;
    radOut3DNifti = NULL;
    radOut4DNifti = NULL;
    radSubOutDicom = NULL;
    radSubOutAnalyze = NULL;
    radSubOutNifti = NULL;
    radSubOutNifti2File = NULL;
    radSubOutNiftiAscii = NULL;
    radSubOutJPEG = NULL;
    radSubOutTIFF = NULL;
    radSubOutPNG = NULL;
    radSubOutBMP = NULL;
    sizerXYSize = NULL;
    staticXSizeLabel = NULL;
    spinXSize = NULL;
    staticYSizeLabel = NULL;
    spinYSize = NULL;
    sizerNumberSlices = NULL;
    staticNumberSlicesLabel = NULL;
    spinNumberOfSlices = NULL;
    chkOriginalIsInterleaved = NULL;
    radVolStartOdd = NULL;
    radVolStartEven = NULL;
    chkFlipX = NULL;
    chkFlipY = NULL;
    chkFlipZ = NULL;
    radSortByFilename = NULL;
    radSortBySlice = NULL;
    lstOutDatatype = NULL;
    txtPatientName = NULL;
    txtPatientID = NULL;
    txtPatientDOB = NULL;
    txtPatientSex = NULL;
    txtStudyDate = NULL;
    txtStudyTime = NULL;
    txtSeriesDate = NULL;
    txtSeriesTime = NULL;
    lstModality = NULL;
    lstPatientPosition = NULL;
    chkOutdirEqualIndir = NULL;
    txtOutputDir = NULL;
    btnSelectDir = NULL;
    radKeepFilename = NULL;
    radUseFilename = NULL;
    txtNewFilename = NULL;
    spinPadSize = NULL;
    btnConvert = NULL;
////@end ConvertDialog member initialisation
}


// --------------------------------------------------------
// ---------- CreateControls ------------------------------
// --------------------------------------------------------
void ConvertDialog::CreateControls()
{    
////@begin ConvertDialog content construction
    ConvertDialog* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Input"));
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxHORIZONTAL);
    itemStaticBoxSizer3Static->SetFont(wxFont(10, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer2->Add(itemStaticBoxSizer3, 1, wxGROW|wxALL, 2);

    wxStaticBox* itemStaticBoxSizer4Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Format"));
    wxStaticBoxSizer* itemStaticBoxSizer4 = new wxStaticBoxSizer(itemStaticBoxSizer4Static, wxVERTICAL);
    itemStaticBoxSizer3->Add(itemStaticBoxSizer4, 0, wxGROW|wxALL, 2);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer4->Add(itemBoxSizer5, 0, wxGROW|wxALL, 0);

    radInRaster = new wxRadioButton( itemDialog1, ID_INTYPE_RASTER, _("Raster Image"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
    radInRaster->SetValue(false);
    itemBoxSizer5->Add(radInRaster, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    itemBoxSizer5->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("2D"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText8->SetForegroundColour(wxColour(255, 128, 128));
    itemBoxSizer5->Add(itemStaticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer4->Add(itemBoxSizer9, 0, wxGROW|wxALL, 0);

    radIn2DDicom = new wxRadioButton( itemDialog1, ID_INTYPE_2DDICOM, _("2D DICOM"), wxDefaultPosition, wxDefaultSize, 0 );
    radIn2DDicom->SetValue(true);
    itemBoxSizer9->Add(radIn2DDicom, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    itemBoxSizer9->Add(5, 5, 1, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText12 = new wxStaticText( itemDialog1, wxID_STATIC, _("2D"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText12->SetForegroundColour(wxColour(255, 128, 128));
    itemBoxSizer9->Add(itemStaticText12, 0, wxGROW|wxALL, 2);

    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer4->Add(itemBoxSizer13, 0, wxGROW|wxALL, 0);

    radIn2DDicomMosaic = new wxRadioButton( itemDialog1, ID_INTYPE_2DDICOMMOSAIC, _("2D DICOM Mosaic"), wxDefaultPosition, wxDefaultSize, 0 );
    radIn2DDicomMosaic->SetValue(false);
    itemBoxSizer13->Add(radIn2DDicomMosaic, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    itemBoxSizer13->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText16 = new wxStaticText( itemDialog1, wxID_STATIC, _("3D"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText16->SetForegroundColour(wxColour(0, 128, 255));
    itemBoxSizer13->Add(itemStaticText16, 0, wxGROW|wxALL, 2);

    wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer4->Add(itemBoxSizer17, 0, wxGROW|wxALL, 0);

    radIn3DDicom = new wxRadioButton( itemDialog1, ID_INTYPE_3DDICOM, _("3D DICOM"), wxDefaultPosition, wxDefaultSize, 0 );
    radIn3DDicom->SetValue(false);
    itemBoxSizer17->Add(radIn3DDicom, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    itemBoxSizer17->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText20 = new wxStaticText( itemDialog1, wxID_STATIC, _("3D"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText20->SetForegroundColour(wxColour(0, 128, 255));
    itemBoxSizer17->Add(itemStaticText20, 0, wxGROW|wxALL, 2);

    wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer4->Add(itemBoxSizer21, 0, wxGROW|wxALL, 0);

    radIn3DNifti = new wxRadioButton( itemDialog1, ID_INTYPE_3DNIFTI, _("3D Nifti/Analyze"), wxDefaultPosition, wxDefaultSize, 0 );
    radIn3DNifti->SetValue(false);
    itemBoxSizer21->Add(radIn3DNifti, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    itemBoxSizer21->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText24 = new wxStaticText( itemDialog1, wxID_STATIC, _("3D"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText24->SetForegroundColour(wxColour(0, 128, 255));
    itemBoxSizer21->Add(itemStaticText24, 0, wxGROW|wxALL, 2);

    wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer4->Add(itemBoxSizer25, 0, wxGROW|wxALL, 0);

    radIn4DNifti = new wxRadioButton( itemDialog1, ID_INTYPE_4DNIFTI, _("4D Nifti/Analyze"), wxDefaultPosition, wxDefaultSize, 0 );
    radIn4DNifti->SetValue(false);
    itemBoxSizer25->Add(radIn4DNifti, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    itemBoxSizer25->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText28 = new wxStaticText( itemDialog1, wxID_STATIC, _("4D"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText28->SetForegroundColour(wxColour(128, 0, 255));
    itemBoxSizer25->Add(itemStaticText28, 0, wxGROW|wxALL, 2);

    wxStaticBox* itemStaticBoxSizer29Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Files"));
    wxStaticBoxSizer* itemStaticBoxSizer29 = new wxStaticBoxSizer(itemStaticBoxSizer29Static, wxVERTICAL);
    itemStaticBoxSizer3->Add(itemStaticBoxSizer29, 1, wxGROW|wxALL, 2);

    wxBoxSizer* itemBoxSizer30 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer29->Add(itemBoxSizer30, 0, wxGROW|wxALL, 1);

    btnClearFiles = new wxButton( itemDialog1, ID_CLEAR_FILES, _("Clear List"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer30->Add(btnClearFiles, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    btnRemoveFiles = new wxButton( itemDialog1, ID_REMOVE_FILES, _("Remove Selected"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer30->Add(btnRemoveFiles, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    itemBoxSizer30->Add(5, 5, 1, wxGROW|wxALL, 5);

    btnAddFiles = new wxButton( itemDialog1, ID_CONVERT_ADD_FILES, _("Add Files..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer30->Add(btnAddFiles, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    lstFiles = new wxListCtrl( itemDialog1, ID_CONVERT_LIST_FILES, wxDefaultPosition, wxSize(100, 100), wxLC_REPORT|wxLC_HRULES );
    itemStaticBoxSizer29->Add(lstFiles, 1, wxGROW|wxALL, 0);

    wxStaticBox* itemStaticBoxSizer36Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Output"));
    wxStaticBoxSizer* itemStaticBoxSizer36 = new wxStaticBoxSizer(itemStaticBoxSizer36Static, wxHORIZONTAL);
    itemStaticBoxSizer36Static->SetFont(wxFont(10, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer2->Add(itemStaticBoxSizer36, 0, wxGROW|wxALL, 2);

    wxBoxSizer* itemBoxSizer37 = new wxBoxSizer(wxVERTICAL);
    itemStaticBoxSizer36->Add(itemBoxSizer37, 0, wxGROW|wxALL, 0);

    wxBoxSizer* itemBoxSizer38 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer37->Add(itemBoxSizer38, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

    wxStaticBox* itemStaticBoxSizer39Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Format"));
    wxStaticBoxSizer* itemStaticBoxSizer39 = new wxStaticBoxSizer(itemStaticBoxSizer39Static, wxVERTICAL);
    itemBoxSizer38->Add(itemStaticBoxSizer39, 0, wxGROW|wxALL, 2);

    wxBoxSizer* itemBoxSizer40 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer39->Add(itemBoxSizer40, 0, wxGROW|wxALL, 0);

    radOutRaster = new wxRadioButton( itemDialog1, ID_OUTTYPE_RASTER, _("Raster Image"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
    radOutRaster->SetValue(false);
    itemBoxSizer40->Add(radOutRaster, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    itemBoxSizer40->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText43 = new wxStaticText( itemDialog1, wxID_STATIC, _("2D"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText43->SetForegroundColour(wxColour(255, 128, 128));
    itemBoxSizer40->Add(itemStaticText43, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxBoxSizer* itemBoxSizer44 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer39->Add(itemBoxSizer44, 0, wxGROW|wxALL, 0);

    radOut2DDicom = new wxRadioButton( itemDialog1, ID_OUTTYPE_2DDICOM, _("2D DICOM"), wxDefaultPosition, wxDefaultSize, 0 );
    radOut2DDicom->SetValue(false);
    itemBoxSizer44->Add(radOut2DDicom, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    itemBoxSizer44->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText47 = new wxStaticText( itemDialog1, wxID_STATIC, _("2D"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText47->SetForegroundColour(wxColour(255, 128, 128));
    itemBoxSizer44->Add(itemStaticText47, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxBoxSizer* itemBoxSizer48 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer39->Add(itemBoxSizer48, 0, wxGROW|wxALL, 0);

    radOut2DDicomMosaic = new wxRadioButton( itemDialog1, ID_OUTTYPE_2DDICOMMOS, _("2D DICOM Mosaic"), wxDefaultPosition, wxDefaultSize, 0 );
    radOut2DDicomMosaic->SetValue(false);
    itemBoxSizer48->Add(radOut2DDicomMosaic, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    itemBoxSizer48->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText51 = new wxStaticText( itemDialog1, wxID_STATIC, _("2D"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText51->SetForegroundColour(wxColour(255, 128, 128));
    itemBoxSizer48->Add(itemStaticText51, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxBoxSizer* itemBoxSizer52 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer39->Add(itemBoxSizer52, 0, wxGROW|wxALL, 0);

    radOut3DDicom = new wxRadioButton( itemDialog1, ID_OUTTYPE_3DDICOM, _("3D DICOM"), wxDefaultPosition, wxDefaultSize, 0 );
    radOut3DDicom->SetValue(false);
    itemBoxSizer52->Add(radOut3DDicom, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    itemBoxSizer52->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText55 = new wxStaticText( itemDialog1, wxID_STATIC, _("3D"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText55->SetForegroundColour(wxColour(0, 128, 255));
    itemBoxSizer52->Add(itemStaticText55, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxBoxSizer* itemBoxSizer56 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer39->Add(itemBoxSizer56, 0, wxGROW|wxALL, 0);

    radOut3DNifti = new wxRadioButton( itemDialog1, ID_OUTTYPE_3DNIFTI, _("3D Nifti/Analyze"), wxDefaultPosition, wxDefaultSize, 0 );
    radOut3DNifti->SetValue(true);
    itemBoxSizer56->Add(radOut3DNifti, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    itemBoxSizer56->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText59 = new wxStaticText( itemDialog1, wxID_STATIC, _("3D"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText59->SetForegroundColour(wxColour(0, 128, 255));
    itemBoxSizer56->Add(itemStaticText59, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxBoxSizer* itemBoxSizer60 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer39->Add(itemBoxSizer60, 0, wxGROW|wxALL, 0);

    radOut4DNifti = new wxRadioButton( itemDialog1, ID_OUTTYPE_4DNIFTI, _("4D Nifti/Analyze"), wxDefaultPosition, wxDefaultSize, 0 );
    radOut4DNifti->SetValue(false);
    itemBoxSizer60->Add(radOut4DNifti, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    itemBoxSizer60->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText63 = new wxStaticText( itemDialog1, wxID_STATIC, _("4D"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText63->SetForegroundColour(wxColour(128, 0, 255));
    itemBoxSizer60->Add(itemStaticText63, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticBox* itemStaticBoxSizer64Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Sub Format"));
    wxStaticBoxSizer* itemStaticBoxSizer64 = new wxStaticBoxSizer(itemStaticBoxSizer64Static, wxVERTICAL);
    itemBoxSizer38->Add(itemStaticBoxSizer64, 0, wxGROW|wxALL, 2);

    radSubOutDicom = new wxRadioButton( itemDialog1, ID_SUBOUT_DICOM, _("DICOM (.dcm)"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
    radSubOutDicom->SetValue(false);
    itemStaticBoxSizer64->Add(radSubOutDicom, 0, wxGROW|wxALL, 2);

    radSubOutAnalyze = new wxRadioButton( itemDialog1, ID_SUBOUT_ANALYZE, _("Analyze (.img .hdr)"), wxDefaultPosition, wxDefaultSize, 0 );
    radSubOutAnalyze->SetValue(false);
    itemStaticBoxSizer64->Add(radSubOutAnalyze, 0, wxGROW|wxALL, 2);

    radSubOutNifti = new wxRadioButton( itemDialog1, ID_SUBOUT_NIFTI, _("Nifti (.nii)"), wxDefaultPosition, wxDefaultSize, 0 );
    radSubOutNifti->SetValue(true);
    itemStaticBoxSizer64->Add(radSubOutNifti, 0, wxGROW|wxALL, 2);

    radSubOutNifti2File = new wxRadioButton( itemDialog1, ID_SUBOUT_NIFTI2FILE, _("Nifti (.hdr .img)"), wxDefaultPosition, wxDefaultSize, 0 );
    radSubOutNifti2File->SetValue(false);
    itemStaticBoxSizer64->Add(radSubOutNifti2File, 0, wxGROW|wxALL, 2);

    radSubOutNiftiAscii = new wxRadioButton( itemDialog1, ID_SUBOUT_NIFTIASCII, _("Nifti ASCII (.asc)"), wxDefaultPosition, wxDefaultSize, 0 );
    radSubOutNiftiAscii->SetValue(false);
    itemStaticBoxSizer64->Add(radSubOutNiftiAscii, 0, wxGROW|wxALL, 2);

    radSubOutJPEG = new wxRadioButton( itemDialog1, ID_SUBOUT_JPEG, _("JPEG (.jpg)"), wxDefaultPosition, wxDefaultSize, 0 );
    radSubOutJPEG->SetValue(false);
    itemStaticBoxSizer64->Add(radSubOutJPEG, 0, wxGROW|wxALL, 2);

    radSubOutTIFF = new wxRadioButton( itemDialog1, ID_SUBOUT_TIFF, _("TIFF (.tiff)"), wxDefaultPosition, wxDefaultSize, 0 );
    radSubOutTIFF->SetValue(false);
    itemStaticBoxSizer64->Add(radSubOutTIFF, 0, wxGROW|wxALL, 2);

    radSubOutPNG = new wxRadioButton( itemDialog1, ID_SUBOUT_PNG, _("PNG (.png)"), wxDefaultPosition, wxDefaultSize, 0 );
    radSubOutPNG->SetValue(false);
    itemStaticBoxSizer64->Add(radSubOutPNG, 0, wxGROW|wxALL, 2);

    radSubOutBMP = new wxRadioButton( itemDialog1, ID_SUBOUT_BMP, _("Bitmap (.bmp)"), wxDefaultPosition, wxDefaultSize, 0 );
    radSubOutBMP->SetValue(false);
    itemStaticBoxSizer64->Add(radSubOutBMP, 0, wxGROW|wxALL, 2);

    wxBoxSizer* itemBoxSizer74 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer38->Add(itemBoxSizer74, 0, wxGROW|wxALL, 0);

    wxStaticBox* itemStaticBoxSizer75Static = new wxStaticBox(itemDialog1, wxID_ANY, _("DICOM Mosaic Options"));
    wxStaticBoxSizer* itemStaticBoxSizer75 = new wxStaticBoxSizer(itemStaticBoxSizer75Static, wxVERTICAL);
    itemBoxSizer74->Add(itemStaticBoxSizer75, 0, wxGROW|wxALL, 5);

    sizerXYSize = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer75->Add(sizerXYSize, 0, wxALIGN_LEFT|wxALL, 0);

    staticXSizeLabel = new wxStaticText( itemDialog1, wxID_STATIC, _("X size"), wxDefaultPosition, wxDefaultSize, 0 );
    staticXSizeLabel->Enable(false);
    sizerXYSize->Add(staticXSizeLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 3);

    spinXSize = new wxSpinCtrl( itemDialog1, ID_CONV_MOSAIC_XSIZE, _T("64"), wxDefaultPosition, wxSize(49, -1), wxSP_ARROW_KEYS, 0, 32768, 64 );
    spinXSize->Enable(false);
    sizerXYSize->Add(spinXSize, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    sizerXYSize->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    staticYSizeLabel = new wxStaticText( itemDialog1, wxID_STATIC, _("Y size"), wxDefaultPosition, wxDefaultSize, 0 );
    staticYSizeLabel->Enable(false);
    sizerXYSize->Add(staticYSizeLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 3);

    spinYSize = new wxSpinCtrl( itemDialog1, ID_CONV_MOSAIC_YSIZE, _T("64"), wxDefaultPosition, wxSize(49, -1), wxSP_ARROW_KEYS, 0, 32768, 64 );
    spinYSize->Enable(false);
    sizerXYSize->Add(spinYSize, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    sizerNumberSlices = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer75->Add(sizerNumberSlices, 0, wxALIGN_LEFT|wxALL, 0);

    staticNumberSlicesLabel = new wxStaticText( itemDialog1, wxID_STATIC, _("Number of Slices"), wxDefaultPosition, wxDefaultSize, 0 );
    staticNumberSlicesLabel->Enable(false);
    sizerNumberSlices->Add(staticNumberSlicesLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 3);

    spinNumberOfSlices = new wxSpinCtrl( itemDialog1, ID_SPINCTRL2, _T("36"), wxDefaultPosition, wxSize(75, -1), wxSP_ARROW_KEYS, 0, 65565, 36 );
    spinNumberOfSlices->Enable(false);
    sizerNumberSlices->Add(spinNumberOfSlices, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    chkOriginalIsInterleaved = new wxCheckBox( itemDialog1, ID_ORIGINAL_IS_INTERLEAVED, _("Original is interleaved"), wxDefaultPosition, wxDefaultSize, 0 );
    chkOriginalIsInterleaved->SetValue(false);
    if (ConvertDialog::ShowToolTips())
        chkOriginalIsInterleaved->SetToolTip(_("3D or 2D Mosaic Only"));
    chkOriginalIsInterleaved->Enable(false);
    itemBoxSizer74->Add(chkOriginalIsInterleaved, 0, wxALIGN_LEFT|wxALL, 5);

    wxBoxSizer* itemBoxSizer86 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer74->Add(itemBoxSizer86, 0, wxALIGN_LEFT|wxALL, 0);

    itemBoxSizer86->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer88 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer86->Add(itemBoxSizer88, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    radVolStartOdd = new wxRadioButton( itemDialog1, ID_VOL_START_ODD, _("Volume starts on odd slice"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
    radVolStartOdd->SetValue(true);
    radVolStartOdd->Enable(false);
    itemBoxSizer88->Add(radVolStartOdd, 0, wxALIGN_LEFT|wxALL, 1);

    radVolStartEven = new wxRadioButton( itemDialog1, ID_VOL_START_EVEN, _("Volume starts on even slice"), wxDefaultPosition, wxDefaultSize, 0 );
    radVolStartEven->SetValue(false);
    radVolStartEven->Enable(false);
    itemBoxSizer88->Add(radVolStartEven, 0, wxALIGN_LEFT|wxALL, 1);

    wxBoxSizer* itemBoxSizer91 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer38->Add(itemBoxSizer91, 0, wxGROW|wxALL, 0);

    wxFlexGridSizer* itemFlexGridSizer92 = new wxFlexGridSizer(1, 4, 0, 0);
    itemBoxSizer91->Add(itemFlexGridSizer92, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticText* itemStaticText93 = new wxStaticText( itemDialog1, wxID_STATIC, _("Flip"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer92->Add(itemStaticText93, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    chkFlipX = new wxCheckBox( itemDialog1, ID_FLIPX, _("X"), wxDefaultPosition, wxDefaultSize, 0 );
    chkFlipX->SetValue(false);
    itemFlexGridSizer92->Add(chkFlipX, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    chkFlipY = new wxCheckBox( itemDialog1, ID_FLIPY, _("Y"), wxDefaultPosition, wxDefaultSize, 0 );
    chkFlipY->SetValue(true);
    itemFlexGridSizer92->Add(chkFlipY, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    chkFlipZ = new wxCheckBox( itemDialog1, ID_FLIPZ, _("Z"), wxDefaultPosition, wxDefaultSize, 0 );
    chkFlipZ->SetValue(false);
    itemFlexGridSizer92->Add(chkFlipZ, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    radSortByFilename = new wxRadioButton( itemDialog1, ID_SORTBY_FILENAME, _("Sort slices by filename"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
    radSortByFilename->SetValue(true);
    radSortByFilename->Enable(false);
    itemBoxSizer91->Add(radSortByFilename, 0, wxALIGN_LEFT|wxALL, 1);

    radSortBySlice = new wxRadioButton( itemDialog1, ID_SORTBY_SLICE, _("Sort slices by internal slice number"), wxDefaultPosition, wxDefaultSize, 0 );
    radSortBySlice->SetValue(false);
    radSortBySlice->Enable(false);
    itemBoxSizer91->Add(radSortBySlice, 0, wxALIGN_LEFT|wxALL, 1);

    itemBoxSizer91->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer100 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer91->Add(itemBoxSizer100, 0, wxGROW|wxALL, 1);

    wxStaticText* itemStaticText101 = new wxStaticText( itemDialog1, wxID_STATIC, _("Datatype"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer100->Add(itemStaticText101, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString lstOutDatatypeStrings;
    lstOutDatatypeStrings.Add(_("No Change"));
    lstOutDatatypeStrings.Add(_("UINT8"));
    lstOutDatatypeStrings.Add(_("INT8"));
    lstOutDatatypeStrings.Add(_("UINT16"));
    lstOutDatatypeStrings.Add(_("INT16"));
    lstOutDatatypeStrings.Add(_("UINT32"));
    lstOutDatatypeStrings.Add(_("INT32"));
    lstOutDatatypeStrings.Add(_("UINT64"));
    lstOutDatatypeStrings.Add(_("INT64"));
    lstOutDatatypeStrings.Add(_("FLOAT32"));
    lstOutDatatypeStrings.Add(_("FLOAT64"));
    lstOutDatatype = new wxChoice( itemDialog1, ID_OUT_DATATYPE, wxDefaultPosition, wxDefaultSize, lstOutDatatypeStrings, 0 );
    lstOutDatatype->SetStringSelection(_("No Change"));
    itemBoxSizer100->Add(lstOutDatatype, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer103Static = new wxStaticBox(itemDialog1, wxID_ANY, _("DICOM Tags"));
    wxStaticBoxSizer* itemStaticBoxSizer103 = new wxStaticBoxSizer(itemStaticBoxSizer103Static, wxVERTICAL);
    itemBoxSizer91->Add(itemStaticBoxSizer103, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer104 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer103->Add(itemBoxSizer104, 0, wxGROW|wxALL, 0);

    wxStaticText* itemStaticText105 = new wxStaticText( itemDialog1, wxID_STATIC, _("Patient Name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer104->Add(itemStaticText105, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtPatientName = new wxTextCtrl( itemDialog1, ID_TEXTCTRL25, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer104->Add(txtPatientName, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticText* itemStaticText107 = new wxStaticText( itemDialog1, wxID_STATIC, _("Last^First"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText107->SetForegroundColour(wxColour(128, 128, 128));
    itemBoxSizer104->Add(itemStaticText107, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer108 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer103->Add(itemBoxSizer108, 0, wxGROW|wxALL, 0);

    wxStaticText* itemStaticText109 = new wxStaticText( itemDialog1, wxID_STATIC, _("Patient ID"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer108->Add(itemStaticText109, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtPatientID = new wxTextCtrl( itemDialog1, ID_TEXTCTRL240, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer108->Add(txtPatientID, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxBoxSizer* itemBoxSizer111 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer103->Add(itemBoxSizer111, 0, wxGROW|wxALL, 0);

    wxStaticText* itemStaticText112 = new wxStaticText( itemDialog1, wxID_STATIC, _("Patient DOB"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer111->Add(itemStaticText112, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtPatientDOB = new wxTextCtrl( itemDialog1, ID_TEXTCTRL26, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer111->Add(txtPatientDOB, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticText* itemStaticText114 = new wxStaticText( itemDialog1, wxID_STATIC, _("YYYYMMDD"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText114->SetForegroundColour(wxColour(128, 128, 128));
    itemBoxSizer111->Add(itemStaticText114, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer115 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer103->Add(itemBoxSizer115, 0, wxGROW|wxALL, 0);

    wxStaticText* itemStaticText116 = new wxStaticText( itemDialog1, wxID_STATIC, _("Patient Sex"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer115->Add(itemStaticText116, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtPatientSex = new wxTextCtrl( itemDialog1, ID_TEXTCTRL100, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer115->Add(txtPatientSex, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticText* itemStaticText118 = new wxStaticText( itemDialog1, wxID_STATIC, _("M, F, U, O"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText118->SetForegroundColour(wxColour(128, 128, 128));
    itemBoxSizer115->Add(itemStaticText118, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer119 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer103->Add(itemBoxSizer119, 0, wxGROW|wxALL, 0);

    wxStaticText* itemStaticText120 = new wxStaticText( itemDialog1, wxID_STATIC, _("Study date/time"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer119->Add(itemStaticText120, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtStudyDate = new wxTextCtrl( itemDialog1, ID_TEXTCTRL101, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer119->Add(txtStudyDate, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticText* itemStaticText122 = new wxStaticText( itemDialog1, wxID_STATIC, _("YYYYMMDD"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText122->SetForegroundColour(wxColour(128, 128, 128));
    itemBoxSizer119->Add(itemStaticText122, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtStudyTime = new wxTextCtrl( itemDialog1, ID_TEXTCTRL102, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer119->Add(txtStudyTime, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticText* itemStaticText124 = new wxStaticText( itemDialog1, wxID_STATIC, _("HHMMSS"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText124->SetForegroundColour(wxColour(128, 128, 128));
    itemBoxSizer119->Add(itemStaticText124, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer125 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer103->Add(itemBoxSizer125, 0, wxGROW|wxALL, 0);

    wxStaticText* itemStaticText126 = new wxStaticText( itemDialog1, wxID_STATIC, _("Series date/time"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer125->Add(itemStaticText126, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtSeriesDate = new wxTextCtrl( itemDialog1, ID_TEXTCTRL103, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer125->Add(txtSeriesDate, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticText* itemStaticText128 = new wxStaticText( itemDialog1, wxID_STATIC, _("YYYYMMDD"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText128->SetForegroundColour(wxColour(128, 128, 128));
    itemBoxSizer125->Add(itemStaticText128, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtSeriesTime = new wxTextCtrl( itemDialog1, ID_TEXTCTRL104, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer125->Add(txtSeriesTime, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticText* itemStaticText130 = new wxStaticText( itemDialog1, wxID_STATIC, _("HHMMSS"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText130->SetForegroundColour(wxColour(128, 128, 128));
    itemBoxSizer125->Add(itemStaticText130, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer131 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer103->Add(itemBoxSizer131, 0, wxGROW|wxALL, 0);

    wxStaticText* itemStaticText132 = new wxStaticText( itemDialog1, wxID_STATIC, _("Modality"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer131->Add(itemStaticText132, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxArrayString lstModalityStrings;
    lstModalityStrings.Add(_("AS"));
    lstModalityStrings.Add(_("BI"));
    lstModalityStrings.Add(_("CD"));
    lstModalityStrings.Add(_("CF"));
    lstModalityStrings.Add(_("CP"));
    lstModalityStrings.Add(_("CR"));
    lstModalityStrings.Add(_("CS"));
    lstModalityStrings.Add(_("CT"));
    lstModalityStrings.Add(_("DD"));
    lstModalityStrings.Add(_("DG"));
    lstModalityStrings.Add(_("DM"));
    lstModalityStrings.Add(_("DS"));
    lstModalityStrings.Add(_("DX"));
    lstModalityStrings.Add(_("EC"));
    lstModalityStrings.Add(_("ECG"));
    lstModalityStrings.Add(_("EM"));
    lstModalityStrings.Add(_("ES"));
    lstModalityStrings.Add(_("FA"));
    lstModalityStrings.Add(_("FS"));
    lstModalityStrings.Add(_("GM"));
    lstModalityStrings.Add(_("HC"));
    lstModalityStrings.Add(_("LP"));
    lstModalityStrings.Add(_("LS"));
    lstModalityStrings.Add(_("MA"));
    lstModalityStrings.Add(_("MG"));
    lstModalityStrings.Add(_("MR"));
    lstModalityStrings.Add(_("MS"));
    lstModalityStrings.Add(_("NM"));
    lstModalityStrings.Add(_("OP"));
    lstModalityStrings.Add(_("OT"));
    lstModalityStrings.Add(_("PT"));
    lstModalityStrings.Add(_("RD"));
    lstModalityStrings.Add(_("RF"));
    lstModalityStrings.Add(_("RG"));
    lstModalityStrings.Add(_("RTIMAG"));
    lstModalityStrings.Add(_("RP"));
    lstModalityStrings.Add(_("RS"));
    lstModalityStrings.Add(_("RT"));
    lstModalityStrings.Add(_("SC"));
    lstModalityStrings.Add(_("SM"));
    lstModalityStrings.Add(_("SR"));
    lstModalityStrings.Add(_("ST"));
    lstModalityStrings.Add(_("TG"));
    lstModalityStrings.Add(_("US"));
    lstModalityStrings.Add(_("VF"));
    lstModalityStrings.Add(_("VL"));
    lstModalityStrings.Add(_("XA"));
    lstModalityStrings.Add(_("XC"));
    lstModality = new wxChoice( itemDialog1, ID_CHOICE, wxDefaultPosition, wxDefaultSize, lstModalityStrings, 0 );
    lstModality->SetStringSelection(_("OT"));
    itemBoxSizer131->Add(lstModality, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer131->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText135 = new wxStaticText( itemDialog1, wxID_STATIC, _("Patient position"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer131->Add(itemStaticText135, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxArrayString lstPatientPositionStrings;
    lstPatientPositionStrings.Add(_("HFS"));
    lstPatientPositionStrings.Add(_("HFP"));
    lstPatientPositionStrings.Add(_("FFS"));
    lstPatientPositionStrings.Add(_("FFP"));
    lstPatientPositionStrings.Add(_("HFDL"));
    lstPatientPositionStrings.Add(_("HFDR"));
    lstPatientPositionStrings.Add(_("FFDL"));
    lstPatientPositionStrings.Add(_("FFDR"));
    lstPatientPosition = new wxChoice( itemDialog1, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, lstPatientPositionStrings, 0 );
    lstPatientPosition->SetStringSelection(_("HFS"));
    itemBoxSizer131->Add(lstPatientPosition, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer137 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer37->Add(itemBoxSizer137, 0, wxGROW|wxALL, 2);

    wxStaticText* itemStaticText138 = new wxStaticText( itemDialog1, wxID_STATIC, _("Directory"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer137->Add(itemStaticText138, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 1);

    itemBoxSizer137->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    chkOutdirEqualIndir = new wxCheckBox( itemDialog1, ID_OUTDIR_EQUAL_INDIR, _("Same as input directory"), wxDefaultPosition, wxDefaultSize, 0 );
    chkOutdirEqualIndir->SetValue(true);
    itemBoxSizer137->Add(chkOutdirEqualIndir, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    wxBoxSizer* itemBoxSizer141 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer37->Add(itemBoxSizer141, 0, wxGROW|wxALL, 0);

    txtOutputDir = new wxTextCtrl( itemDialog1, ID_OUTPUT_DIR, wxEmptyString, wxDefaultPosition, wxSize(130, -1), 0 );
    txtOutputDir->SetMaxLength(2);
    itemBoxSizer141->Add(txtOutputDir, 1, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    btnSelectDir = new wxButton( itemDialog1, ID_SELECT_DIR, _("..."), wxDefaultPosition, wxSize(25, -1), 0 );
    itemBoxSizer141->Add(btnSelectDir, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxStaticBox* itemStaticBoxSizer144Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Filename(s)"));
    wxStaticBoxSizer* itemStaticBoxSizer144 = new wxStaticBoxSizer(itemStaticBoxSizer144Static, wxVERTICAL);
    itemBoxSizer37->Add(itemStaticBoxSizer144, 0, wxGROW|wxALL, 5);

    radKeepFilename = new wxRadioButton( itemDialog1, ID_KEEP_FILENAME, _("Use original filename"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
    radKeepFilename->SetValue(false);
    itemStaticBoxSizer144->Add(radKeepFilename, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer146 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer144->Add(itemBoxSizer146, 0, wxGROW|wxALL, 0);

    radUseFilename = new wxRadioButton( itemDialog1, ID_USE_FILENAME, _("Use this filename / prefix"), wxDefaultPosition, wxDefaultSize, 0 );
    radUseFilename->SetValue(true);
    itemBoxSizer146->Add(radUseFilename, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtNewFilename = new wxTextCtrl( itemDialog1, ID_NEW_FILENAME, _("file"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer146->Add(txtNewFilename, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer149 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer144->Add(itemBoxSizer149, 0, wxGROW|wxALL, 0);

    itemBoxSizer149->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText151 = new wxStaticText( itemDialog1, wxID_STATIC, _("Pad new file prefix number to"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer149->Add(itemStaticText151, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    spinPadSize = new wxSpinCtrl( itemDialog1, ID_PAD_SIZE, _T("4"), wxDefaultPosition, wxSize(45, -1), wxSP_ARROW_KEYS, 0, 9, 4 );
    itemBoxSizer149->Add(spinPadSize, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxStaticText* itemStaticText153 = new wxStaticText( itemDialog1, wxID_STATIC, _("digits"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer149->Add(itemStaticText153, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer154 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer154, 0, wxALIGN_RIGHT|wxALL, 0);

    wxButton* itemButton155 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer154->Add(itemButton155, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer154->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    btnConvert = new wxButton( itemDialog1, ID_CONVERT, _("Convert"), wxDefaultPosition, wxDefaultSize, 0 );
    btnConvert->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer154->Add(btnConvert, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton158 = new wxButton( itemDialog1, wxID_OK, _("Done"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer154->Add(itemButton158, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end ConvertDialog content construction

	wxListItem colItem;
	colItem.SetText("Filepath");
	lstFiles->InsertColumn(0,colItem);
	lstFiles->SetColumnWidth(0,400);
	colItem.SetText("New Filename");
	lstFiles->InsertColumn(1,colItem);
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_ORIGINAL_IS_INTERLEAVED
 */

void ConvertDialog::OnOriginalIsInterleavedClick( wxCommandEvent& event )
{
	if (chkOriginalIsInterleaved->GetValue()) {
		radVolStartOdd->Enable();
		radVolStartEven->Enable();
	}
	else {
		radVolStartOdd->Disable();
		radVolStartEven->Disable();
	}
}


// --------------------------------------------------------
// ---------- OnSelectDirClick ----------------------------
// --------------------------------------------------------
void ConvertDialog::OnSelectDirClick( wxCommandEvent& event )
{
	wxDirDialog *openDirDialog = new wxDirDialog(this);

	if (openDirDialog->ShowModal() == wxID_OK) {
		wxString dirname = openDirDialog->GetPath();
		dirname.Replace("\\","/");
		txtOutputDir->SetValue(dirname);
	}
}


// --------------------------------------------------------
// ---------- OnClearFilesClick ---------------------------
// --------------------------------------------------------
void ConvertDialog::OnClearFilesClick( wxCommandEvent& event )
{
	lstFiles->DeleteAllItems();
}


// --------------------------------------------------------
// ---------- OnRemoveFilesClick --------------------------
// --------------------------------------------------------
void ConvertDialog::OnRemoveFilesClick( wxCommandEvent& event )
{
    long item;

	item = lstFiles->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	while ( item != -1 ) {
		lstFiles->DeleteItem(item);
		// -1 because the indices were shifted by DeleteItem()
		item = lstFiles->GetNextItem(item - 1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	}
}


// --------------------------------------------------------
// ---------- OnCovertAddFilesClick -----------------------
// --------------------------------------------------------
void ConvertDialog::OnConvertAddFilesClick( wxCommandEvent& event )
{
	int i;
	wxFileName *extname;
	static const wxChar	*FILETYPES = _T( "All supported formats (*.img, *.hdr, *.nii, *.dcm, *.acr, *.pap, *.gif, *.tif, *.tiff, *.bmp, *.png, *.jpg, *.jpeg, *.jpe)|*.img;*.hdr;*.nii;*.dcm;*.acr;*.pap;*.gif;*.tif;*.tiff;*.bmp;*.png;*.jpg;*.jpeg;*.jpe|Nifti-1 (*.nii)|*.nii|Dicom/ACR-NEMA/Papyrus (*.dcm, *.acr, *.pap)|*.dcm;*.acr;*.pap|Analyze 7.5 (*.img,*.hdr)|*.img;*.hdr|GIF (*.gif)|*.gif|TIFF (*.tif, *.tiff)|*.tif;*.tiff|PNG (*.png)|*.png|BMP (*.bmp)|*.bmp|JPEG (*.jpg,*.jpeg,*.jpe)|*.jpg;*.jpeg;*.jpe;*.jff;*.jfif;*.jif;*.jtf|All files(*.*)|*.*");

	wxFileDialog *openFileDialog = new wxFileDialog (this, "Select files...", "", "", FILETYPES, wxOPEN|wxFD_MULTIPLE, wxDefaultPosition);

	if (openFileDialog->ShowModal() == wxID_OK) {
		wxArrayString filenames;
		openFileDialog->GetPaths(filenames);
		int numItems = (int)filenames.Count();
		int index = lstFiles->GetItemCount();
		for (i=0;i<numItems;i++) {
			filenames[i].Replace("\\","/");
			lstFiles->InsertItem(index,filenames[i]);
			WriteLog("Should've added a file...");
			if (i <= 1) {
				/* determine extension of first file */
				extname = new wxFileName(filenames[i]);
			}
		}
	}

	wxString ext = extname->GetExt();
	if ( (ext == "dcm") || (ext == "") || (ext == "acr") || (ext == "pap")) {
		radIn2DDicom->SetValue(true);
		radOutRaster->Enable();
		radOut2DDicom->Disable();
		radOut2DDicomMosaic->Enable();
		radOut3DDicom->Enable();
		radOut3DNifti->Enable();
		radOut4DNifti->Disable();
		radSubOutAnalyze->Enable();
		radSubOutNifti->Enable();
		radSubOutNifti2File->Enable();
		radSubOutNiftiAscii->Enable();
		radSubOutJPEG->Enable();
		radSubOutTIFF->Enable();
		radSubOutPNG->Enable();
		radSubOutBMP->Enable();
	}
	else if ( (ext == "img") || (ext == "hdr") || (ext == "nii")) {
		radIn3DNifti->SetValue(true);
		radOutRaster->Disable();
		radOut2DDicom->Enable();
		radOut2DDicom->SetValue(true);
		radOut2DDicomMosaic->Enable();
		radOut3DDicom->Enable();
		radOut3DNifti->Disable();
		radOut4DNifti->Enable();
		radSubOutDicom->Enable();
		radSubOutDicom->SetValue(true);
		radSubOutAnalyze->Disable();
		radSubOutNifti->Disable();
		radSubOutNifti2File->Disable();
		radSubOutNiftiAscii->Disable();
		radSubOutJPEG->Disable();
		radSubOutTIFF->Disable();
		radSubOutPNG->Disable();
		radSubOutBMP->Disable();
	}
	else {
		radInRaster->SetValue(true);
		radOutRaster->Disable();
		radOut2DDicom->Enable();
		radOut2DDicomMosaic->Enable();
		radOut3DDicom->Enable();
		radOut3DNifti->Enable();
		radOut4DNifti->Disable();
		radSubOutAnalyze->Enable();
		radSubOutNifti->Enable();
		radSubOutNifti2File->Enable();
		radSubOutNiftiAscii->Enable();
		radSubOutJPEG->Disable();
		radSubOutTIFF->Disable();
		radSubOutPNG->Disable();
		radSubOutBMP->Disable();
	}
}


// --------------------------------------------------------
// ---------- OnConvertClick ------------------------------
// --------------------------------------------------------
void ConvertDialog::OnConvertClick( wxCommandEvent& event )
{
	int i = 0;
	int volInType = VOL_2D;
	int volOutType = VOL_3D;
	int formatInType;
	int formatOutType;
	int numFiles;
	int convertType;
	int loadType;
	long item = -1;
	wxString ext;
	wxString newname;
	wxArrayString filepaths;
	wxArrayString singleFile;
	wxFileName *oldFilename;
	wxFileName *newFilename;
	wxFileName outDir;

	bool keepFilename = radKeepFilename->GetValue();
	bool keepInputDir = chkOutdirEqualIndir->GetValue();

	/* determine the input dimensionality and filetype */
	if (radInRaster->GetValue() == true) { volInType = VOL_2D; formatInType = FILETYPE_RASTER; }
	if (radIn2DDicom->GetValue() == true) { volInType = VOL_2D; formatInType = FILETYPE_DICOM; }
	if (radIn2DDicomMosaic->GetValue() == true) { volInType = VOL_3D; formatInType = FILETYPE_DICOM; }
	if (radIn3DDicom->GetValue() == true) { volInType = VOL_3D; formatInType = FILETYPE_DICOM; }
	if (radIn3DNifti->GetValue() == true) { volInType = VOL_3D; formatInType = FILETYPE_ANALYZE; }
	if (radIn4DNifti->GetValue() == true) { volInType = VOL_4D; formatInType = FILETYPE_ANALYZE; }

	/* determine the output dimensionality */
	if (radOutRaster->GetValue() == true) volOutType = VOL_2D;
	if (radOut2DDicom->GetValue() == true) volOutType = VOL_2D;
	if (radOut2DDicomMosaic->GetValue() == true) volOutType = VOL_2D;
	if (radOut3DDicom->GetValue() == true) volOutType = VOL_3D;
	if (radOut3DNifti->GetValue() == true) volOutType = VOL_3D;
	if (radOut4DNifti->GetValue() == true) volOutType = VOL_4D;

	/* set the output type and new extension */
	if (radSubOutJPEG->GetValue()) { formatOutType = OUTPUT_FT_JPEG; ext = "jpg"; }
	if (radSubOutPNG->GetValue()) { formatOutType = OUTPUT_FT_PNG; ext = "png"; }
	if (radSubOutTIFF->GetValue()) { formatOutType = OUTPUT_FT_TIFF; ext = "tiff"; }
	if (radSubOutBMP->GetValue()) { formatOutType = OUTPUT_FT_BMP; ext = "bmp"; }
	if (radSubOutDicom->GetValue()) { formatOutType = OUTPUT_FT_DICOM; ext = "dcm"; }
	if (radSubOutAnalyze->GetValue()) { formatOutType = OUTPUT_FT_ANALYZE; ext = "img"; }
	if (radSubOutNifti->GetValue()) { formatOutType = OUTPUT_FT_NIFTI1FILE; ext = "nii"; }
	if (radSubOutNifti2File->GetValue()) { formatOutType = OUTPUT_FT_NIFTI2FILE; ext = "img"; }
	if (radSubOutNiftiAscii->GetValue()) { formatOutType = OUTPUT_FT_NIFTIASCII; ext = "asc"; }

	/* get list of files */
    for (;;) {
        item = lstFiles->GetNextItem(item, wxLIST_NEXT_ALL);
        if ( item == -1 ) break;

		/* get the full filepath */
		filepaths.Add(lstFiles->GetItemText(item));
    }
	numFiles = (int)filepaths.Count();

	/* setup the output directory */
	oldFilename = new wxFileName(filepaths[0]);
	if (!keepInputDir) {
		/* and if the output directory is blank, set it to the input dir */
		if (txtOutputDir->GetValue() != "")
			outDir.AssignDir(txtOutputDir->GetValue());
		else
			outDir.SetPath(oldFilename->GetPath());
	}
	else
		outDir.SetPath(oldFilename->GetPath());
	
	/* ------------------------------------------------------------------------

		THREE basic types of conversion (1 to 1 , N to 1 , 1 to N)
		EIGHT possible in/out configurations
			2D->2D (1 in  1 out)
			2D->3D (N in  1 out)
			3D->2D (1 in  N out)
			3D->2D (1 in  1 out mosaic)
			3D->3D (1 in  1 out)
			3D->4D (N in  1 out)
			4D->3D (1 in  N out)
			4D->4D (1 in  1 out)                                               */

	/* ----- N to 1 conversion ----- */
	if ( ((volInType == VOL_2D) && (volOutType == VOL_3D)) || ((volInType == VOL_3D) && (volOutType == VOL_4D)) ) {
		WriteLog("Performing an N --> 1 conversion");

		convertType = CONVERT_N_TO_1;
		loadType = OPEN_SELECTED;

		/* keep the original filename, just change the extension (or path) */
		if (keepFilename)
			newFilename = new wxFileName(*oldFilename);
		else
			newFilename = new wxFileName(txtNewFilename->GetValue());
		newFilename->SetExt(ext);

		WriteLog(wxString::Format("ConvertFile(%d,%s,%d,%d,%d,%s,%s,%d)", formatInType, filepaths.Item(0).c_str(), loadType, formatOutType, convertType, outDir.GetFullPath().c_str(), newFilename->GetFullPath().c_str(), i));
		ConvertFile(formatInType, filepaths, loadType, formatOutType, convertType, outDir, newFilename->GetFullName(), i);

		delete newFilename;
	}
	/* ----- 1 to N conversion ----- */
	else if ( ((volInType == VOL_3D) && (volOutType == VOL_2D)) || ((volInType == VOL_4D) && (volOutType == VOL_3D)) ) {
		convertType = CONVERT_1_TO_N;
		loadType = OPEN_SINGLE;
		WriteLog("Performing a 1 --> N conversion");
		/* each file will be converted to its own directory and the directory name will basically be the generated filename */

		wxArrayString fp;
		/* load each file and save to a new directory */
		for (i=0;i<numFiles;i++) {
			oldFilename = new wxFileName(filepaths[i]);
			/* keep the original filename, just change the extension (or path) */
			if (keepFilename) {
				newFilename = new wxFileName(*oldFilename);
				newFilename->SetExt(ext);
			}
			else {
				newname = txtNewFilename->GetValue();
				/* if there is more than one file, they must be renumbered */
				if ((int)filepaths.Count() > 1)
					newname += GetPaddedNumber(spinPadSize->GetValue(),i);
				newFilename = new wxFileName(newname);
				newFilename->SetExt(ext);
			}
			/* need to pass a wxArrayString, so create a temp one, since we're only passing 1 file at a time */
			fp.Add(filepaths.Item(i));
			ConvertFile(formatInType, fp, loadType, formatOutType, convertType, outDir, newFilename->GetFullName(), i);
			fp.Clear();
		}

	}
	/* ----- 1 to 1 conversion ----- */
	else {
		convertType = CONVERT_1_TO_1;
		loadType = OPEN_SINGLE;
		WriteLog("Performing a 1 --> 1 conversion");

		/* load each file and save each as a new image */
		for (i=0;i<numFiles;i++) {
			oldFilename = new wxFileName(filepaths[i]);
			/* keep the original filename, just change the extension (or path) */
			if (keepFilename) {
				newFilename = new wxFileName(*oldFilename);
				newFilename->SetExt(ext);
			}
			else {
				newname = txtNewFilename->GetValue();
				/* if there is more than one file, they must be renumbered */
				if ((int)filepaths.Count() > 1)
					newname += GetPaddedNumber(spinPadSize->GetValue(),i);
				newFilename = new wxFileName(newname);
				newFilename->SetExt(ext);
			}

			ConvertFile(formatInType, filepaths, loadType, formatOutType, convertType, outDir, newFilename->GetFullName(), i);
		}
	}

}


// --------------------------------------------------------
// ---------- ConvertFile ---------------------------------
// --------------------------------------------------------
void ConvertDialog::ConvertFile(int formatInType, wxArrayString filepaths, int loadType, int formatOutType, int convertType, wxFileName outDir, wxString outFilename, int i)
{
	ImageData *imgData;

	/* ---------- do the saving ----------- */
	/* load up the file into an ImageData object */
	imgData = new ImageData(mainVars);
	imgData->SetLoadParameters(formatInType, filepaths, loadType, radIn2DDicomMosaic->GetValue(), spinXSize->GetValue(), spinYSize->GetValue(), spinNumberOfSlices->GetValue());
	if (imgData->Load()) {
		/* flip if necessary */
		if (chkFlipX->GetValue()) imgData->FlipDataX();
		if (chkFlipY->GetValue()) imgData->FlipDataY();
		if (chkFlipZ->GetValue()) imgData->FlipDataZ();

		/* add dicom info if necessary */
		imgData->dcmInfo->SetPatientName(txtPatientName->GetValue());
		imgData->dcmInfo->SetPatientID(txtPatientID->GetValue());
		imgData->dcmInfo->SetPatientBirthdate(txtPatientDOB->GetValue());
		imgData->dcmInfo->SetPatientSex(txtPatientSex->GetValue());
		imgData->dcmInfo->SetStudyDate(txtStudyDate->GetValue());
		imgData->dcmInfo->SetStudyTime(txtStudyTime->GetValue());
		imgData->dcmInfo->SetSeriesDate(txtSeriesDate->GetValue());
		imgData->dcmInfo->SetSeriesTime(txtSeriesTime->GetValue());
		imgData->dcmInfo->SetModality(lstModality->GetStringSelection());
		imgData->dcmInfo->SetPatientPositionStr(lstPatientPosition->GetStringSelection());
		
		/* write the file(s) */
		imgData->SetSaveParameters(formatOutType, convertType, outDir, outFilename, i);
		if (!imgData->Save()) {
			/* report any saving errors */
			lstFiles->SetItem(i,1,"Error saving file");
			lstFiles->SetItemBackgroundColour(i,wxColour(255,210,210));
		}
		else {
			/* add the file to the list of completed ones */
			lstFiles->SetItem(i,1,outFilename);
			lstFiles->SetItemBackgroundColour(i,wxColour(210,255,210));
		}
	}
	else {
		/* report any loading errors */
		lstFiles->SetItem(i,1,"Error opening file");
		lstFiles->SetItemBackgroundColour(i,wxColour(255,210,210));
	}
	delete imgData;

}


/*!
 * Should we show tooltips?
 */

bool ConvertDialog::ShowToolTips()
{
    return true;
}


// --------------------------------------------------------
// ---------- GetBitmapResources --------------------------
// --------------------------------------------------------
wxBitmap ConvertDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ConvertDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end ConvertDialog bitmap retrieval
}


// --------------------------------------------------------
// ---------- GetIconResources ----------------------------
// --------------------------------------------------------
wxIcon ConvertDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ConvertDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end ConvertDialog icon retrieval
}


// --------------------------------------------------------
// ---------- OnFormatSelected ----------------------------
// --------------------------------------------------------
//void ConvertDialog::OnFormatSelected( wxCommandEvent& event )
//{
//	int volInType = VOL_2D;
//	int volOutType = VOL_3D;
//
//	/* determine the input dimensionality and filetype */
//	if (radInRaster->GetValue() == true) volInType = VOL_2D;
//	if (radIn2DDicom->GetValue() == true) volInType = VOL_2D;
//	if (radIn2DDicomMosaic->GetValue() == true) volInType = VOL_3D;
//	if (radIn3DDicom->GetValue() == true) volInType = VOL_3D;
//	if (radIn3DNifti->GetValue() == true) volInType = VOL_3D;
//	if (radIn4DNifti->GetValue() == true) volInType = VOL_4D;
//
//	/* determine the output dimensionality */
//	if (radOutRaster->GetValue() == true) volOutType = VOL_2D;
//	if (radOut2DDicom->GetValue() == true) volOutType = VOL_2D;
//	if (radOut2DDicomMosaic->GetValue() == true) volOutType = VOL_2D;
//	if (radOut3DDicom->GetValue() == true) volOutType = VOL_3D;
//	if (radOut3DNifti->GetValue() == true) volOutType = VOL_3D;
//	if (radOut4DNifti->GetValue() == true) volOutType = VOL_4D;
//
//	if ( ((volInType == VOL_2D) && (volOutType == VOL_3D)) || ((volInType == VOL_3D) && (volOutType == VOL_4D)) ) {
//		conversionType = CONVERT_N_TO_1;
//		chkKeepFilename->Disable();
//	}
//	else if ( ((volInType == VOL_3D) && (volOutType == VOL_2D)) || ((volInType == VOL_4D) && (volOutType == VOL_3D)) ) {
//		conversionType = CONVERT_1_TO_N;
//		chkKeepFilename->Enable();
//	}
//	else {
//		conversionType = CONVERT_1_TO_1;
//		chkKeepFilename->Enable();
//	}
//}


void ConvertDialog::EnableAllFormatOptions()
{
	radOutRaster->Enable();
	radOut2DDicom->Enable();
	radOut2DDicomMosaic->Enable();
	radOut3DDicom->Enable();
	radOut3DNifti->Enable();
	radOut4DNifti->Enable();
	radSubOutAnalyze->Enable();
	radSubOutNifti->Enable();
	radSubOutNifti2File->Enable();
	radSubOutNiftiAscii->Enable();
	radSubOutJPEG->Enable();
	radSubOutTIFF->Enable();
	radSubOutPNG->Enable();
	radSubOutBMP->Enable();
}


// --------------------------------------------------------
// ---------- GetPaddedNumber -----------------------------
// --------------------------------------------------------
wxString ConvertDialog::GetPaddedNumber(int numPlaces, int number)
{
	wxString str;
	int numZeros;
	int i;

	str.Printf("%d",number);

	/* if it has room for more zeros, add em in */
	if ((int)str.Length() < numPlaces) {
		numZeros = numPlaces - (int)str.Length();
		for (i=0;i<numZeros;i++)
			str = "0" + str;
	}

	/* otherwise just return the number */
	return str;
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_INTYPE_RASTER
 */

void ConvertDialog::OnIntypeRasterSelected( wxCommandEvent& event )
{
	radOutRaster->Disable();
	radOut2DDicom->Enable();
	radOut2DDicom->SetValue(true);
	radOut2DDicomMosaic->Enable();
	radOut3DDicom->Enable();
	radOut3DNifti->Enable();
	radOut4DNifti->Disable();
	radSubOutDicom->Enable();
	radSubOutDicom->SetValue(true);
	radSubOutAnalyze->Enable();
	radSubOutNifti->Enable();
	radSubOutNifti2File->Enable();
	radSubOutNiftiAscii->Enable();
	radSubOutJPEG->Disable();
	radSubOutTIFF->Disable();
	radSubOutPNG->Disable();
	radSubOutBMP->Disable();
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_INTYPE_2DDICOM
 */

void ConvertDialog::OnIntype2ddicomSelected( wxCommandEvent& event )
{
	radOutRaster->Enable();
	radOut2DDicom->Disable();
	radOut2DDicomMosaic->Enable();
	radOut3DDicom->Enable();
	radOut3DNifti->Enable();
	radOut3DNifti->SetValue(true);
	radOut4DNifti->Disable();
	radSubOutDicom->Disable();
	radSubOutAnalyze->Enable();
	radSubOutNifti->Enable();
	radSubOutNifti->SetValue(true);
	radSubOutNifti2File->Enable();
	radSubOutNiftiAscii->Enable();
	radSubOutJPEG->Enable();
	radSubOutTIFF->Enable();
	radSubOutPNG->Enable();
	radSubOutBMP->Enable();
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_INTYPE_2DDICOMMOSAIC
 */

void ConvertDialog::OnIntype2ddicommosaicSelected( wxCommandEvent& event )
{
	radOutRaster->Enable();
	radOut2DDicom->Disable();
	radOut2DDicomMosaic->Enable();
	radOut3DDicom->Enable();
	radOut3DNifti->Enable();
	radOut3DNifti->SetValue(true);
	radOut4DNifti->Disable();
	radSubOutDicom->Disable();
	radSubOutAnalyze->Enable();
	radSubOutNifti->Enable();
	radSubOutNifti->SetValue(true);
	radSubOutNifti2File->Enable();
	radSubOutNiftiAscii->Enable();
	radSubOutJPEG->Enable();
	radSubOutTIFF->Enable();
	radSubOutPNG->Enable();
	radSubOutBMP->Enable();
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_INTYPE_3DDICOM
 */

void ConvertDialog::OnIntype3ddicomSelected( wxCommandEvent& event )
{
	radOutRaster->Disable();
	radOut2DDicom->Enable();
	radOut2DDicomMosaic->Enable();
	radOut3DDicom->Enable();
	radOut3DNifti->Disable();
	radOut4DNifti->Enable();
	radOut4DNifti->SetValue(true);
	radSubOutAnalyze->Enable();
	radSubOutNifti->Enable();
	radSubOutNifti->SetValue(true);
	radSubOutNifti2File->Enable();
	radSubOutNiftiAscii->Enable();
	radSubOutJPEG->Disable();
	radSubOutTIFF->Disable();
	radSubOutPNG->Disable();
	radSubOutBMP->Disable();
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_INTYPE_3DNIFTI
 */

void ConvertDialog::OnIntype3dniftiSelected( wxCommandEvent& event )
{
	radOutRaster->Disable();
	radOut2DDicom->Enable();
	radOut2DDicomMosaic->Enable();
	radOut3DDicom->Enable();
	radOut3DNifti->Disable();
	radOut4DNifti->Enable();
	radOut4DNifti->SetValue(true);
	radSubOutAnalyze->Enable();
	radSubOutNifti->Enable();
	radSubOutNifti->SetValue(true);
	radSubOutNifti2File->Enable();
	radSubOutNiftiAscii->Enable();
	radSubOutJPEG->Disable();
	radSubOutTIFF->Disable();
	radSubOutPNG->Disable();
	radSubOutBMP->Disable();
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_INTYPE_4DNIFTI
 */

void ConvertDialog::OnIntype4dniftiSelected( wxCommandEvent& event )
{
	radOutRaster->Disable();
	radOut2DDicom->Disable();
	radOut2DDicomMosaic->Disable();
	radOut3DDicom->Enable();
	radOut3DNifti->Enable();
	radOut3DNifti->SetValue(true);
	radOut4DNifti->Disable();
	radSubOutAnalyze->Enable();
	radSubOutNifti->Enable();
	radSubOutNifti->SetValue(true);
	radSubOutNifti2File->Enable();
	radSubOutNiftiAscii->Enable();
	radSubOutJPEG->Disable();
	radSubOutTIFF->Disable();
	radSubOutPNG->Disable();
	radSubOutBMP->Disable();
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OUTTYPE_RASTER
 */

void ConvertDialog::OnOuttypeRasterSelected( wxCommandEvent& event )
{
	radSubOutDicom->Disable();
	radSubOutAnalyze->Disable();
	radSubOutNifti->Disable();
	radSubOutNifti2File->Disable();
	radSubOutNiftiAscii->Disable();
	radSubOutJPEG->Enable();
	radSubOutJPEG->SetValue(true);
	radSubOutTIFF->Enable();
	radSubOutPNG->Enable();
	radSubOutBMP->Enable();
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OUTTYPE_2DDICOM
 */

void ConvertDialog::OnOuttype2ddicomSelected( wxCommandEvent& event )
{
	radSubOutDicom->Enable();
	radSubOutDicom->SetValue(true);
	radSubOutAnalyze->Disable();
	radSubOutNifti->Disable();
	radSubOutNifti2File->Disable();
	radSubOutNiftiAscii->Disable();
	radSubOutJPEG->Disable();
	radSubOutTIFF->Disable();
	radSubOutPNG->Disable();
	radSubOutBMP->Disable();
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OUTTYPE_2DDICOMMOS
 */

void ConvertDialog::OnOuttype2ddicommosSelected( wxCommandEvent& event )
{
	radSubOutDicom->Enable();
	radSubOutDicom->SetValue(true);
	radSubOutAnalyze->Disable();
	radSubOutNifti->Disable();
	radSubOutNifti2File->Disable();
	radSubOutNiftiAscii->Disable();
	radSubOutJPEG->Disable();
	radSubOutTIFF->Disable();
	radSubOutPNG->Disable();
	radSubOutBMP->Disable();
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OUTTYPE_3DDICOM
 */

void ConvertDialog::OnOuttype3ddicomSelected( wxCommandEvent& event )
{
	radSubOutDicom->Enable();
	radSubOutDicom->SetValue(true);
	radSubOutAnalyze->Disable();
	radSubOutNifti->Disable();
	radSubOutNifti2File->Disable();
	radSubOutNiftiAscii->Disable();
	radSubOutJPEG->Disable();
	radSubOutTIFF->Disable();
	radSubOutPNG->Disable();
	radSubOutBMP->Disable();
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OUTTYPE_3DNIFTI
 */

void ConvertDialog::OnOuttype3dniftiSelected( wxCommandEvent& event )
{
	radSubOutDicom->Disable();
	radSubOutAnalyze->Enable();
	radSubOutNifti->Enable();
	radSubOutNifti->SetValue(true);
	radSubOutNifti2File->Enable();
	radSubOutNiftiAscii->Enable();
	radSubOutJPEG->Disable();
	radSubOutTIFF->Disable();
	radSubOutPNG->Disable();
	radSubOutBMP->Disable();
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OUTTYPE_4DNIFTI
 */

void ConvertDialog::OnOuttype4dniftiSelected( wxCommandEvent& event )
{
	radSubOutDicom->Disable();
	radSubOutAnalyze->Enable();
	radSubOutNifti->Enable();
	radSubOutNifti->SetValue(true);
	radSubOutNifti2File->Enable();
	radSubOutNiftiAscii->Enable();
	radSubOutJPEG->Disable();
	radSubOutTIFF->Disable();
	radSubOutPNG->Disable();
	radSubOutBMP->Disable();
}

