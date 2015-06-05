/* ----------------------------------------------------------------------------
	ImageInformationDialog.cpp


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
#pragma implementation "ImageInformationDialog.h"
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
#include "AboutDialog.h"
////@end includes

#include "MainFrame.h"
#include "AboutDialog.h"
//#include "gdcmAttribute.h"
//#include "gdcmFile.h"
//#include "gdcmImage.h"
#include "gdcmReader.h"
#include "gdcmStringFilter.h"
#include "gdcmCSAHeader.h"
#include "gdcmPrinter.h"
#include "wx/dir.h"
#include "wx/File.h"
#include "ImageInformationDialog.h"

////@begin XPM images
#include "xpm/brain.xpm"
#include "xpm/help.xpm"
#include "xpm/smiley-bw.xpm"
#include "xpm/mri.xpm"
#include "xpm/histogram.xpm"
#include "xpm/dicomtags.xpm"
////@end XPM images

/*!
 * ImageInformationDialog type definition
 */

IMPLEMENT_CLASS( ImageInformationDialog, wxFrame )

/*!
 * ImageInformationDialog event table definition
 */

BEGIN_EVENT_TABLE( ImageInformationDialog, wxFrame )

////@begin ImageInformationDialog event table entries
    EVT_MENU( ID_IMGINFO_FILE_CLOSE, ImageInformationDialog::OnImginfoFileCloseClick )

    EVT_MENU( ID_IMGINFO_HELP_HELP, ImageInformationDialog::OnImginfoHelpHelpClick )

    EVT_MENU( ID_IMGINFO_HELP_ABOUT, ImageInformationDialog::OnImginfoHelpAboutClick )

    EVT_COMBOBOX( ID_FILE_COMBO, ImageInformationDialog::OnFileComboSelected )

    EVT_BUTTON( ID_DRAW_HISTOGRAM, ImageInformationDialog::OnDrawHistogramClick )

    EVT_BUTTON( wxID_OK, ImageInformationDialog::OnClickClose )

////@end ImageInformationDialog event table entries

END_EVENT_TABLE()

/*!
 * ImageInformationDialog constructors
 */

ImageInformationDialog::ImageInformationDialog( )
{
    Init();
}

ImageInformationDialog::ImageInformationDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
	controlsCreated = false;
    Create( parent, id, caption, pos, size, style );
	Show(true);
}

/*!
 * ImageInformationDialog creator
 */

bool ImageInformationDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin ImageInformationDialog creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    SetIcon(GetIconResource(wxT("../xpm/brain.xpm")));
    Centre();
////@end ImageInformationDialog creation
	dc = new wxClientDC(this->pnlHistogram);
	controlsCreated = true;
    return true;
}

// --------------------------------------------------------
// ---------- SetInfoPointers -----------------------------
// --------------------------------------------------------
void ImageInformationDialog::SetInfoPointers(ImageData *imageData, ImageInfo *imageInfo, DicomInfo *dicomInfo, NiftiInfo *niftiInfo) {
	imgData = imageData;
	imgInfo = imageInfo;
	dcmInfo = dicomInfo;
	nftInfo = niftiInfo;

	PopulateFilelist();

	wxArrayString filelist;
	filelist = imgInfo->GetFileList();

	switch (imgData->GetFileType()) {
		case FILETYPE_DICOM:
			PopulatePatientTab();
			PopulateImageTab();
			PopulateHistogramTab();
			PopulateDicomTags(filelist[0]);
			mainNotebook->DeletePage(4);
			break;
		case FILETYPE_ANALYZE:
			PopulatePatientTab();
			PopulateImageTab();
			PopulateHistogramTab();
			PopulateNiftiTags();
			mainNotebook->DeletePage(3);
			break;
		case FILETYPE_RASTER:
			PopulateImageTab();
			PopulateHistogramTab();
			mainNotebook->DeletePage(0);
			mainNotebook->DeletePage(2);
			mainNotebook->DeletePage(2);
			break;
		default:
			PopulatePatientTab();
			PopulateImageTab();
			PopulateHistogramTab();
			PopulateDicomTags(filelist[0]);
			PopulateNiftiTags();
	}
}


/*!
 * Member initialisation 
 */

void ImageInformationDialog::Init()
{
////@begin ImageInformationDialog member initialisation
    mainPanel = NULL;
    mainSizer = NULL;
    fileSizer = NULL;
    cmbFile = NULL;
    mainNotebook = NULL;
    pnlPatient = NULL;
    patientSizer = NULL;
    txtPatientName = NULL;
    txtPatientID = NULL;
    txtBirthdate = NULL;
    txtStudy = NULL;
    txtStudyDateTime = NULL;
    txtSeries = NULL;
    txtSeriesDateTime = NULL;
    txtPhysician = NULL;
    txtProtocol = NULL;
    pnlImage = NULL;
    imageSizer = NULL;
    dimOutsideSizer = NULL;
    dimSizer = NULL;
    txtPixelSizeX = NULL;
    txtPixelSizeY = NULL;
    txtPixelSizeZ = NULL;
    txtPixelSizeT = NULL;
    txtVolumeSizeX = NULL;
    txtVolumeSizeY = NULL;
    txtVolumeSizeZ = NULL;
    txtVolumeSizeT = NULL;
    txtPixelSpacingX = NULL;
    txtPixelSpacingY = NULL;
    txtPixelSpacingZ = NULL;
    txtPixelSpacingT = NULL;
    txtOriginX = NULL;
    txtOriginY = NULL;
    txtOriginZ = NULL;
    imageInfoSizer = NULL;
    txtImageType = NULL;
    txtVolumeType = NULL;
    txtIsRGB = NULL;
    txtPixelType = NULL;
    txtModality = NULL;
    txtIsMultiFile = NULL;
    txtIsSigned = NULL;
    txtPatientPosition = NULL;
    txtDirectionCosines = NULL;
    txtMaxImageRGB = NULL;
    txtMinImageRGB = NULL;
    txtMaxImageGray = NULL;
    txtMinImageGray = NULL;
    txtMaxVolumeRGB = NULL;
    txtMinVolumeRGB = NULL;
    txtMaxVolumeGray = NULL;
    txtMinVolumeGray = NULL;
    btnDrawHistogram = NULL;
    pnlHistogram = NULL;
    pnlDicomTags = NULL;
    lstDicomTags = NULL;
    txtDicomTags = NULL;
    pnlNiftiTags = NULL;
    lstNiftiTags = NULL;
    btnClose = NULL;
////@end ImageInformationDialog member initialisation
}
/*!
 * Control creation for ImageInformationDialog
 */

void ImageInformationDialog::CreateControls()
{    
////@begin ImageInformationDialog content construction
    ImageInformationDialog* itemFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    itemMenu3->Append(ID_IMGINFO_FILE_CLOSE, _("&Close"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu3, _("File"));
    wxMenu* itemMenu5 = new wxMenu;
    {
        wxMenuItem* menuItem = new wxMenuItem(itemMenu5, ID_IMGINFO_HELP_HELP, _("Help"), _T(""), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("../xpm/help.xpm")));
        menuItem->SetBitmap(bitmap);
        itemMenu5->Append(menuItem);
    }
    itemMenu5->AppendSeparator();
    itemMenu5->Append(ID_IMGINFO_HELP_ABOUT, _("About MIView..."), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu5, _("Help"));
    itemFrame1->SetMenuBar(menuBar);

    mainPanel = new wxPanel( itemFrame1, ID_MAINPANEL, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );

    mainSizer = new wxBoxSizer(wxVERTICAL);
    mainPanel->SetSizer(mainSizer);

    fileSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(fileSizer, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText12 = new wxStaticText( mainPanel, ID_STATIC_FILES, _("File(s)..."), wxDefaultPosition, wxDefaultSize, 0 );
    fileSizer->Add(itemStaticText12, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxArrayString cmbFileStrings;
    cmbFile = new wxComboBox( mainPanel, ID_FILE_COMBO, _T(""), wxDefaultPosition, wxDefaultSize, cmbFileStrings, wxCB_DROPDOWN );
    fileSizer->Add(cmbFile, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mainNotebook = new wxNotebook( mainPanel, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxNB_DEFAULT|wxNB_TOP );
    wxImageList* mainNotebookImageList = new wxImageList(16, 16, true, 5);
    {
        wxIcon mainNotebookIcon0(itemFrame1->GetIconResource(wxT("../xpm/smiley-bw.xpm")));
        mainNotebookImageList->Add(mainNotebookIcon0);
        wxIcon mainNotebookIcon1(itemFrame1->GetIconResource(wxT("../xpm/mri.xpm")));
        mainNotebookImageList->Add(mainNotebookIcon1);
        wxIcon mainNotebookIcon2(itemFrame1->GetIconResource(wxT("../xpm/histogram.xpm")));
        mainNotebookImageList->Add(mainNotebookIcon2);
        wxIcon mainNotebookIcon3(itemFrame1->GetIconResource(wxT("../xpm/dicomtags.xpm")));
        mainNotebookImageList->Add(mainNotebookIcon3);
        wxIcon mainNotebookIcon4(itemFrame1->GetIconResource(wxT("../xpm/dicomtags.xpm")));
        mainNotebookImageList->Add(mainNotebookIcon4);
    }
    mainNotebook->AssignImageList(mainNotebookImageList);

    pnlPatient = new wxPanel( mainNotebook, ID_PANEL_PATIENT, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
    patientSizer = new wxFlexGridSizer(2, 2, 0, 0);
    pnlPatient->SetSizer(patientSizer);

    wxStaticText* itemStaticText17 = new wxStaticText( pnlPatient, ID_STATIC_PATIENTNAME, _("Patient name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText17->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    patientSizer->Add(itemStaticText17, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtPatientName = new wxStaticText( pnlPatient, ID_PATIENTNAME, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    txtPatientName->SetForegroundColour(wxColour(0, 0, 255));
    patientSizer->Add(txtPatientName, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText19 = new wxStaticText( pnlPatient, ID_STATIC_PATIENTID, _("Patient ID"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText19->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    patientSizer->Add(itemStaticText19, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtPatientID = new wxStaticText( pnlPatient, ID_PATIENTID, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    txtPatientID->SetForegroundColour(wxColour(0, 0, 255));
    patientSizer->Add(txtPatientID, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText21 = new wxStaticText( pnlPatient, ID_STATIC_BIRTHDATE, _("Birthdate"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText21->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    patientSizer->Add(itemStaticText21, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtBirthdate = new wxStaticText( pnlPatient, ID_BIRTHDATE, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    txtBirthdate->SetForegroundColour(wxColour(0, 0, 255));
    patientSizer->Add(txtBirthdate, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText23 = new wxStaticText( pnlPatient, ID_STATIC_STUDY, _("Study"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText23->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    patientSizer->Add(itemStaticText23, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtStudy = new wxStaticText( pnlPatient, ID_STUDY, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    txtStudy->SetForegroundColour(wxColour(0, 0, 255));
    patientSizer->Add(txtStudy, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText25 = new wxStaticText( pnlPatient, ID_STATIC_STUDYDATETIME, _("Study Date/Time"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText25->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    patientSizer->Add(itemStaticText25, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtStudyDateTime = new wxStaticText( pnlPatient, ID_STUDYDATETIME, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    txtStudyDateTime->SetForegroundColour(wxColour(0, 0, 255));
    patientSizer->Add(txtStudyDateTime, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText27 = new wxStaticText( pnlPatient, ID_STATIC_SERIES, _("Series"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText27->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    patientSizer->Add(itemStaticText27, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtSeries = new wxStaticText( pnlPatient, ID_SERIES, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    txtSeries->SetForegroundColour(wxColour(0, 0, 255));
    patientSizer->Add(txtSeries, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText29 = new wxStaticText( pnlPatient, ID_STATIC_SERIESDATETIME, _("Series Date/Time"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText29->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    patientSizer->Add(itemStaticText29, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtSeriesDateTime = new wxStaticText( pnlPatient, ID_SERIESDATETIME, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    txtSeriesDateTime->SetForegroundColour(wxColour(0, 0, 255));
    patientSizer->Add(txtSeriesDateTime, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText31 = new wxStaticText( pnlPatient, ID_STATIC_PHYSICIAN, _("Performing physician"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText31->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    patientSizer->Add(itemStaticText31, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtPhysician = new wxStaticText( pnlPatient, ID_PHYSICIAN, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    txtPhysician->SetForegroundColour(wxColour(0, 0, 255));
    patientSizer->Add(txtPhysician, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText33 = new wxStaticText( pnlPatient, ID_STATIC_PROTOCOL, _("Protocol name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText33->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    patientSizer->Add(itemStaticText33, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtProtocol = new wxStaticText( pnlPatient, ID_PROTOCOL, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    txtProtocol->SetForegroundColour(wxColour(0, 0, 255));
    patientSizer->Add(txtProtocol, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    mainNotebook->AddPage(pnlPatient, _("Patient"), false, 0);

    pnlImage = new wxPanel( mainNotebook, ID_PANEL_IMAGE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    pnlImage->SetForegroundColour(wxColour(0, 0, 255));
    imageSizer = new wxBoxSizer(wxVERTICAL);
    pnlImage->SetSizer(imageSizer);

    dimOutsideSizer = new wxStaticBox(pnlImage, wxID_ANY, _("Dimensions"));
    wxStaticBoxSizer* itemStaticBoxSizer37 = new wxStaticBoxSizer(dimOutsideSizer, wxVERTICAL);
    imageSizer->Add(itemStaticBoxSizer37, 0, wxGROW|wxALL, 5);
    dimSizer = new wxFlexGridSizer(4, 5, 0, 0);
    itemStaticBoxSizer37->Add(dimSizer, 1, wxALIGN_LEFT|wxALL, 5);
    dimSizer->Add(3, 3, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText40 = new wxStaticText( pnlImage, ID_STATIC_X, _("X"), wxDefaultPosition, wxSize(50, -1), wxALIGN_CENTRE );
    itemStaticText40->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    dimSizer->Add(itemStaticText40, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticText* itemStaticText41 = new wxStaticText( pnlImage, ID_STATIC_Y, _("Y"), wxDefaultPosition, wxSize(50, -1), wxALIGN_CENTRE );
    itemStaticText41->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    dimSizer->Add(itemStaticText41, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticText* itemStaticText42 = new wxStaticText( pnlImage, ID_STATIC_Z, _("Z"), wxDefaultPosition, wxSize(50, -1), wxALIGN_CENTRE );
    itemStaticText42->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    dimSizer->Add(itemStaticText42, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticText* itemStaticText43 = new wxStaticText( pnlImage, ID_STATIC_T, _("T"), wxDefaultPosition, wxSize(50, -1), wxALIGN_CENTRE );
    itemStaticText43->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    dimSizer->Add(itemStaticText43, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText44 = new wxStaticText( pnlImage, ID_STATIC_PIXELSIZE, _("Image size"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText44->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    dimSizer->Add(itemStaticText44, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtPixelSizeX = new wxStaticText( pnlImage, ID_PIXELSIZE_X, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    dimSizer->Add(txtPixelSizeX, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtPixelSizeY = new wxStaticText( pnlImage, ID_PIXELSIZE_Y, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    dimSizer->Add(txtPixelSizeY, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtPixelSizeZ = new wxStaticText( pnlImage, ID_PIXELSIZE_Z, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    dimSizer->Add(txtPixelSizeZ, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtPixelSizeT = new wxStaticText( pnlImage, ID_PIXELSIZE_T, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    dimSizer->Add(txtPixelSizeT, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticText* itemStaticText49 = new wxStaticText( pnlImage, ID_STATIC_VOLUMESIZE, _("Volume Size"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText49->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    dimSizer->Add(itemStaticText49, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtVolumeSizeX = new wxStaticText( pnlImage, ID_VOLUMESIZE_X, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    dimSizer->Add(txtVolumeSizeX, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtVolumeSizeY = new wxStaticText( pnlImage, ID_VOLUMESIZE_Y, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    dimSizer->Add(txtVolumeSizeY, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtVolumeSizeZ = new wxStaticText( pnlImage, ID_VOLUMESIZE_Z, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    dimSizer->Add(txtVolumeSizeZ, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtVolumeSizeT = new wxStaticText( pnlImage, ID_VOLUMESIZE_T, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    dimSizer->Add(txtVolumeSizeT, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticText* itemStaticText54 = new wxStaticText( pnlImage, ID_STATIC_PIXELSPACE, _("Pixel spacing (mm)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText54->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    dimSizer->Add(itemStaticText54, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtPixelSpacingX = new wxStaticText( pnlImage, ID_PIXELSPACE_X, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    dimSizer->Add(txtPixelSpacingX, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtPixelSpacingY = new wxStaticText( pnlImage, ID_PIXELSPACE_Y, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    dimSizer->Add(txtPixelSpacingY, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtPixelSpacingZ = new wxStaticText( pnlImage, ID_PIXELSPACE_Z, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    dimSizer->Add(txtPixelSpacingZ, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtPixelSpacingT = new wxStaticText( pnlImage, ID_PIXELSPACE_T, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    dimSizer->Add(txtPixelSpacingT, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticText* itemStaticText59 = new wxStaticText( pnlImage, ID_STATIC_ORIGIN, _("Origin"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText59->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    dimSizer->Add(itemStaticText59, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtOriginX = new wxStaticText( pnlImage, ID_ORIGIN_X, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    dimSizer->Add(txtOriginX, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtOriginY = new wxStaticText( pnlImage, ID_ORIGIN_Y, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    dimSizer->Add(txtOriginY, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtOriginZ = new wxStaticText( pnlImage, ID_ORIGIN_Z, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    dimSizer->Add(txtOriginZ, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    dimSizer->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    imageInfoSizer = new wxFlexGridSizer(2, 2, 0, 0);
    imageSizer->Add(imageInfoSizer, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText65 = new wxStaticText( pnlImage, ID_STATIC_IMAGETYPE, _("Image type"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText65->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    imageInfoSizer->Add(itemStaticText65, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtImageType = new wxStaticText( pnlImage, ID_IMAGETYPE, _("2D"), wxDefaultPosition, wxDefaultSize, 0 );
    txtImageType->SetForegroundColour(wxColour(0, 0, 255));
    imageInfoSizer->Add(txtImageType, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText67 = new wxStaticText( pnlImage, ID_STATIC_NUMVOLS, _("Volume type"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText67->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    imageInfoSizer->Add(itemStaticText67, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtVolumeType = new wxStaticText( pnlImage, ID_VOLUMETYPE, _("2D"), wxDefaultPosition, wxDefaultSize, 0 );
    txtVolumeType->SetForegroundColour(wxColour(0, 0, 255));
    imageInfoSizer->Add(txtVolumeType, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText69 = new wxStaticText( pnlImage, ID_STATIC_ISRGB, _("Is RGB"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText69->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    imageInfoSizer->Add(itemStaticText69, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtIsRGB = new wxStaticText( pnlImage, ID_ISRGB, _("No"), wxDefaultPosition, wxDefaultSize, 0 );
    txtIsRGB->SetForegroundColour(wxColour(0, 0, 255));
    imageInfoSizer->Add(txtIsRGB, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText71 = new wxStaticText( pnlImage, ID_STATIC_PIXELTYPE, _("Pixel type"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText71->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    imageInfoSizer->Add(itemStaticText71, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtPixelType = new wxStaticText( pnlImage, ID_PIXELTYPE, _("8U"), wxDefaultPosition, wxDefaultSize, 0 );
    txtPixelType->SetForegroundColour(wxColour(0, 0, 255));
    imageInfoSizer->Add(txtPixelType, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText73 = new wxStaticText( pnlImage, ID_STATIC_MODALITY, _("Modality"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText73->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    imageInfoSizer->Add(itemStaticText73, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtModality = new wxStaticText( pnlImage, ID_MODALITY, _("Unknown"), wxDefaultPosition, wxDefaultSize, 0 );
    txtModality->SetForegroundColour(wxColour(0, 0, 255));
    imageInfoSizer->Add(txtModality, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText75 = new wxStaticText( pnlImage, ID_STATIC_ISMULTIFILE, _("Is multi-file"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText75->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    imageInfoSizer->Add(itemStaticText75, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtIsMultiFile = new wxStaticText( pnlImage, ID_ISMULTIFILE, _("No"), wxDefaultPosition, wxDefaultSize, 0 );
    txtIsMultiFile->SetForegroundColour(wxColour(0, 0, 255));
    imageInfoSizer->Add(txtIsMultiFile, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText77 = new wxStaticText( pnlImage, ID_STATIC_ISSIGNED, _("Is signed"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText77->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    imageInfoSizer->Add(itemStaticText77, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtIsSigned = new wxStaticText( pnlImage, ID_ISSIGNED, _("No"), wxDefaultPosition, wxDefaultSize, 0 );
    txtIsSigned->SetForegroundColour(wxColour(0, 0, 255));
    imageInfoSizer->Add(txtIsSigned, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText79 = new wxStaticText( pnlImage, ID_STATIC_PAT_POS, _("Patient position"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText79->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    imageInfoSizer->Add(itemStaticText79, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtPatientPosition = new wxStaticText( pnlImage, ID_PAT_POS, _("Unknown"), wxDefaultPosition, wxDefaultSize, 0 );
    txtPatientPosition->SetForegroundColour(wxColour(0, 0, 255));
    imageInfoSizer->Add(txtPatientPosition, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText81 = new wxStaticText( pnlImage, ID_STATIC_DIR_COS, _("Direction cosines"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText81->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    imageInfoSizer->Add(itemStaticText81, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtDirectionCosines = new wxStaticText( pnlImage, ID_DIR_COS, _("0/0/0/0/0/0"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ImageInformationDialog::ShowToolTips())
        txtDirectionCosines->SetToolTip(_("Row(X,Y,Z) Col(X,Y,Z)"));
    txtDirectionCosines->SetForegroundColour(wxColour(0, 0, 255));
    imageInfoSizer->Add(txtDirectionCosines, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    mainNotebook->AddPage(pnlImage, _("Image"), false, 1);

    wxPanel* itemPanel83 = new wxPanel( mainNotebook, ID_PANEL_HISTOGRAM, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer84 = new wxBoxSizer(wxVERTICAL);
    itemPanel83->SetSizer(itemBoxSizer84);

    wxBoxSizer* itemBoxSizer85 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer84->Add(itemBoxSizer85, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer86 = new wxFlexGridSizer(2, 2, 0, 0);
    itemBoxSizer85->Add(itemFlexGridSizer86, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText87 = new wxStaticText( itemPanel83, wxID_STATIC, _("Image Values"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText87->SetFont(wxFont(9, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemFlexGridSizer86->Add(itemStaticText87, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 3);

    wxStaticText* itemStaticText88 = new wxStaticText( itemPanel83, wxID_STATIC, _("Volume Values"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText88->SetFont(wxFont(9, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemFlexGridSizer86->Add(itemStaticText88, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 3);

    wxFlexGridSizer* itemFlexGridSizer89 = new wxFlexGridSizer(4, 2, 0, 0);
    itemFlexGridSizer86->Add(itemFlexGridSizer89, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText90 = new wxStaticText( itemPanel83, wxID_STATIC, _("Max (R,G,B)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText90->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemFlexGridSizer89->Add(itemStaticText90, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtMaxImageRGB = new wxStaticText( itemPanel83, wxID_STATIC, _("(0.0, 0.0, 0.0)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer89->Add(txtMaxImageRGB, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText92 = new wxStaticText( itemPanel83, wxID_STATIC, _("Min (R,G,B)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText92->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemFlexGridSizer89->Add(itemStaticText92, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtMinImageRGB = new wxStaticText( itemPanel83, wxID_STATIC, _("(0.0, 0.0, 0.0)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer89->Add(txtMinImageRGB, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText94 = new wxStaticText( itemPanel83, wxID_STATIC, _("Max Gray"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText94->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemFlexGridSizer89->Add(itemStaticText94, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtMaxImageGray = new wxStaticText( itemPanel83, wxID_STATIC, _("0.0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer89->Add(txtMaxImageGray, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText96 = new wxStaticText( itemPanel83, wxID_STATIC, _("Min Gray"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText96->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemFlexGridSizer89->Add(itemStaticText96, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtMinImageGray = new wxStaticText( itemPanel83, wxID_STATIC, _("0.0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer89->Add(txtMinImageGray, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxFlexGridSizer* itemFlexGridSizer98 = new wxFlexGridSizer(4, 2, 0, 0);
    itemFlexGridSizer86->Add(itemFlexGridSizer98, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText99 = new wxStaticText( itemPanel83, wxID_STATIC, _("Max (R,G,B)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText99->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemFlexGridSizer98->Add(itemStaticText99, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtMaxVolumeRGB = new wxStaticText( itemPanel83, wxID_STATIC, _("(0.0, 0.0, 0.0)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer98->Add(txtMaxVolumeRGB, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText101 = new wxStaticText( itemPanel83, wxID_STATIC, _("Min (R,G,B)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText101->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemFlexGridSizer98->Add(itemStaticText101, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtMinVolumeRGB = new wxStaticText( itemPanel83, wxID_STATIC, _("(0.0, 0.0, 0.0)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer98->Add(txtMinVolumeRGB, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText103 = new wxStaticText( itemPanel83, wxID_STATIC, _("Max Gray"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText103->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemFlexGridSizer98->Add(itemStaticText103, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtMaxVolumeGray = new wxStaticText( itemPanel83, wxID_STATIC, _("0.0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer98->Add(txtMaxVolumeGray, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    wxStaticText* itemStaticText105 = new wxStaticText( itemPanel83, wxID_STATIC, _("Min Gray"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText105->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemFlexGridSizer98->Add(itemStaticText105, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    txtMinVolumeGray = new wxStaticText( itemPanel83, wxID_STATIC, _("0.0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer98->Add(txtMinVolumeGray, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    itemBoxSizer85->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    btnDrawHistogram = new wxButton( itemPanel83, ID_DRAW_HISTOGRAM, _("Draw Histogram"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer85->Add(btnDrawHistogram, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticLine* itemStaticLine109 = new wxStaticLine( itemPanel83, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer84->Add(itemStaticLine109, 0, wxGROW|wxALL, 5);

    pnlHistogram = new wxPanel( itemPanel83, ID_HISTOGRAM, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
    itemBoxSizer84->Add(pnlHistogram, 1, wxGROW|wxALL, 5);

    mainNotebook->AddPage(itemPanel83, _("Histogram"), false, 2);

    pnlDicomTags = new wxPanel( mainNotebook, ID_PANEL_DICOMTAGS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer112 = new wxBoxSizer(wxHORIZONTAL);
    pnlDicomTags->SetSizer(itemBoxSizer112);

    lstDicomTags = new wxListCtrl( pnlDicomTags, ID_DICOMTAG_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxSIMPLE_BORDER );
    lstDicomTags->SetFont(wxFont(10, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Tahoma")));
    itemBoxSizer112->Add(lstDicomTags, 1, wxGROW|wxALL, 5);

    txtDicomTags = new wxTextCtrl( pnlDicomTags, ID_DICOM_TAGS, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    itemBoxSizer112->Add(txtDicomTags, 1, wxGROW|wxALL, 5);

    mainNotebook->AddPage(pnlDicomTags, _("Dicom Tags"), false, 3);

    pnlNiftiTags = new wxPanel( mainNotebook, ID_PANELNIFTITAGS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer116 = new wxBoxSizer(wxHORIZONTAL);
    pnlNiftiTags->SetSizer(itemBoxSizer116);

    lstNiftiTags = new wxListCtrl( pnlNiftiTags, ID_NIFTITAG_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxSIMPLE_BORDER );
    itemBoxSizer116->Add(lstNiftiTags, 1, wxGROW|wxALL, 5);

    mainNotebook->AddPage(pnlNiftiTags, _("Nifti Tags"), false, 4);

    mainSizer->Add(mainNotebook, 1, wxGROW|wxALL, 5);

    btnClose = new wxButton( mainPanel, wxID_OK, _("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
    btnClose->SetDefault();
    mainSizer->Add(btnClose, 0, wxALIGN_RIGHT|wxALL, 5);

    // Connect events and objects
    pnlHistogram->Connect(ID_HISTOGRAM, wxEVT_PAINT, wxPaintEventHandler(ImageInformationDialog::OnPaint), NULL, this);
////@end ImageInformationDialog content construction
}

// --------------------------------------------------------
// ---------- PopulateFilelist ----------------------------
// --------------------------------------------------------
void ImageInformationDialog::PopulateFilelist()
{
	cmbFile->Clear();

	/* print file list */
	wxArrayString theList;
	theList = imgInfo->GetFileList();

	for (int i=0; i<theList.Count(); i++) {
//		WriteLog(msg.Format("[%d] - %s\n",i,theList[i].c_str() ));
		cmbFile->Append(theList[i]);
	}

	cmbFile->SetSelection(0);
}


// --------------------------------------------------------
// ---------- PopulatePatientTab --------------------------
// --------------------------------------------------------
void ImageInformationDialog::PopulatePatientTab()
{
	txtPatientName->SetLabel(dcmInfo->GetPatientName());
    txtPatientID->SetLabel(dcmInfo->GetPatientID());
    txtBirthdate->SetLabel(dcmInfo->GetPatientBirthdate());
    txtStudy->SetLabel(dcmInfo->GetStudyDescription());
	if (dcmInfo->GetStudyDateTime().IsValid())
		txtStudyDateTime->SetLabel(dcmInfo->GetStudyDateTime().Format("%b-%d-%Y %I:%M:%S %p"));
    txtSeries->SetLabel(dcmInfo->GetSeriesDescription());
	if (dcmInfo->GetSeriesDateTime().IsValid())
	    txtSeriesDateTime->SetLabel(dcmInfo->GetSeriesDateTime().Format("%b-%d-%Y %I:%M:%S %p"));
    txtPhysician->SetLabel(dcmInfo->GetPerformingPhysician());
    txtProtocol->SetLabel(dcmInfo->GetProtocolName());

}


// --------------------------------------------------------
// ---------- PopulateImageTab ----------------------------
// --------------------------------------------------------
void ImageInformationDialog::PopulateImageTab()
{
	wxString str;

	txtPixelSizeX->SetLabel(str.Format("%d",imgInfo->GetImageXSize()));
    txtPixelSizeY->SetLabel(str.Format("%d",imgInfo->GetImageYSize()));
    txtPixelSizeZ->SetLabel(str.Format("%d",imgInfo->GetImageZSize()));
    txtPixelSizeT->SetLabel(str.Format("%d",imgInfo->GetImageTSize()));
    txtVolumeSizeX->SetLabel(str.Format("%d",imgInfo->GetVolumeXSize()));
    txtVolumeSizeY->SetLabel(str.Format("%d",imgInfo->GetVolumeYSize()));
    txtVolumeSizeZ->SetLabel(str.Format("%d",imgInfo->GetVolumeZSize()));
    txtVolumeSizeT->SetLabel(str.Format("%d",imgInfo->GetVolumeTSize()));
    txtPixelSpacingX->SetLabel(str.Format("%.4f",imgInfo->GetSpacing().x));
    txtPixelSpacingY->SetLabel(str.Format("%.4f",imgInfo->GetSpacing().y));
    txtPixelSpacingZ->SetLabel(str.Format("%.4f",imgInfo->GetSpacing().z));
    txtOriginX->SetLabel(str.Format("%.4f",imgInfo->GetOrigin().x));
    txtOriginY->SetLabel(str.Format("%.4f",imgInfo->GetOrigin().y));
    txtOriginZ->SetLabel(str.Format("%.4f",imgInfo->GetOrigin().z));

	switch (imgInfo->GetImageType()) {
		case IMG_1D: txtImageType->SetLabel("1D"); break;
		case IMG_2D: txtImageType->SetLabel("2D"); break;
		case IMG_3D: txtImageType->SetLabel("3D"); break;
		case IMG_4D: txtImageType->SetLabel("4D"); break;
	}
	switch (imgInfo->GetVolumeType()) {
		case VOL_1D: txtVolumeType->SetLabel("1D"); break;
		case VOL_2D: txtVolumeType->SetLabel("2D"); break;
		case VOL_3D: txtVolumeType->SetLabel("3D"); break;
		case VOL_4D: txtVolumeType->SetLabel("4D"); break;
	}
	txtIsRGB->SetLabel(imgInfo->GetIsRGB()?"Yes":"No");
    txtPixelType->SetLabel(str.Format("%s",imgInfo->GetPixelTypeStr().c_str()));
    txtModality->SetLabel(str.Format("%s",dcmInfo->GetModalityDescription().c_str()));
    txtIsMultiFile->SetLabel(imgInfo->GetIsMultiFile()?"Yes":"No");
    txtIsSigned->SetLabel(imgInfo->GetIsSignedPixelData()?"Yes":"No");
    txtPatientPosition->SetLabel(dcmInfo->GetPatientPositionStr());
	
	wxString cosines;
	cosines = str.Format("%.6f/%.6f/%.6f/%.6f/%.6f/%.6f",dcmInfo->GetRowCosines().x, dcmInfo->GetRowCosines().y, dcmInfo->GetRowCosines().z, dcmInfo->GetColCosines().x, dcmInfo->GetColCosines().y, dcmInfo->GetColCosines().z);
    txtDirectionCosines->SetLabel(cosines);
}


// --------------------------------------------------------
// ---------- PopulateHistogramTab ------------------------
// --------------------------------------------------------
void ImageInformationDialog::PopulateHistogramTab()
{
	wxString str;

    txtMaxVolumeRGB->SetLabel(str.Format("(%.4f, %.4f, %.4f)", imgInfo->maxPixelValues.Red(), imgInfo->maxPixelValues.Green(), imgInfo->maxPixelValues.Blue()));
    txtMinVolumeRGB->SetLabel(str.Format("(%.4f, %.4f, %.4f)", imgInfo->minPixelValues.Red(), imgInfo->minPixelValues.Green(), imgInfo->minPixelValues.Blue()));
    txtMaxVolumeGray->SetLabel(str.Format("%.4f", imgInfo->maxPixelValues.Gray()));
    txtMinVolumeGray->SetLabel(str.Format("%.4f", imgInfo->minPixelValues.Gray()));

	CalculateHistogram();
}


// --------------------------------------------------------
// ---------- PopulateDicomTags ---------------------------
// --------------------------------------------------------
void ImageInformationDialog::PopulateDicomTags(wxString filename)
{
	/* get rid of everything, in case there is something left over */
	lstDicomTags->ClearAll();

	/* setup the columns */
	wxListItem colItem;
	colItem.SetText("(group,element)");
	lstDicomTags->InsertColumn(0,colItem);
	colItem.SetText("Key");
	lstDicomTags->InsertColumn(1,colItem);
	colItem.SetText("Name");
	lstDicomTags->InsertColumn(2,colItem);
	colItem.SetText("VR");
	lstDicomTags->InsertColumn(3,colItem);
	colItem.SetText("VM");
	lstDicomTags->InsertColumn(4,colItem);
	colItem.SetText("Offset");
	lstDicomTags->InsertColumn(5,colItem);
	colItem.SetText("Length");
	lstDicomTags->InsertColumn(6,colItem);
	colItem.SetText("Value");
	lstDicomTags->InsertColumn(7,colItem);

	int index; //, idx;
	wxString currentFilename;
	unsigned short int group, element;
	wxString entryKey;
	wxString entryName;
	wxString entryVR;
	wxString entryVM;
	wxString entryValue;
	wxString cellValue;
	wxString str;
	wxArrayInt offsets, lengths;
//	char *valueBuffer;
	int valueOffset;
	unsigned int valueLength;
	wxFile dicomfile;
//	int bytesRead;

//	wxString filename = fileChoice->GetStringSelection();

	index = 0;

	/* setup all the gdcm stuff */
//	gdcm::ImageReader reader;
//	gdcm::DataSet dataSet;
//	gdcm::StringFilter sf;
////	gdcm::Tag tag;
//	reader.SetFileName(filename.c_str());
//	reader.Read();
//	const gdcm::Image &image = reader.GetImage();
//	const gdcm::File &file = reader.GetFile();
//	dataSet = file.GetDataSet();
//	sf.SetFile( file );

	//const gdcm::File &file = reader.GetFile();
	//const gdcm::DataSet &ds = r.GetFile().GetDataSet();
//	gdcm::DataSet ds = r.GetFile().GetDataSet();
//	sf.SetFile( r.GetFile() );

	/* loop through all the doc elements */
//	gdcm::DataSet::Iterator it = ds.Begin();
	//gdcm::DataSet::ConstIterator it = ds.Begin();

	gdcm::StringFilter sf;
	gdcm::Reader r;
	r.SetFileName( filename.c_str() );
	if( !r.Read() )
	{
		wxMessageDialog(NULL,"Failure to read file","MIView error");
	}
	gdcm::DataSet const& ds = r.GetFile().GetDataSet();
	sf.SetFile( r.GetFile() );

	std::ostringstream oss (std::ostringstream::out);
	gdcm::Printer p;
	gdcm::File f;
	f.SetDataSet( ds );
	p.SetFile( f );
	p.Print( oss );

	wxString output = oss.str();
	//WriteLog(output);
	wxArrayString arr = Explode(output,'\n');
	txtDicomTags->SetValue(output);

	gdcm::DataSet::ConstIterator it = ds.Begin();
	for( ; it != ds.End(); ++it)
	{
		const gdcm::DataElement &ref = *it;
		//const gdcm::VL &vl = ref.GetVL();
		//const gdcm::VR &vr = ref.GetVR();
		std::pair<std::string, std::string> s = sf.ToStringPair( ref.GetTag() );

		element = ref.GetTag().GetElement();
		group = ref.GetTag().GetGroup();
		//valueLength = vr.GetSize(); //.GetLength();
		//entryVR = ref.GetVR().GetVRString(.GetVRString(vr); //GetSize();
		entryName = s.first.c_str();
		cellValue = s.second.c_str();
		if( !s.second.empty() || ref.GetVL() == 0 ) {
		//	std::cout << s.first << " -> " << s.second << std::endl;
		}
		else if( !ref.GetByteValue() ) {  // It means it's a SQ
			entryValue = "SQ";
		}
		else if( ref.GetTag().IsPrivate() ) {
			entryName = "Private";
		}
		else {
			entryName = "Unsupported";
		}
		///* populate the grid */
		lstDicomTags->InsertItem(index,cellValue.Format("(%04X,%04X)",group,element));
		lstDicomTags->SetItem(index,2,entryName);
		lstDicomTags->SetItem(index,3,entryVR);
		//lstDicomTags->SetItem(index,4,entryVM);
		//lstDicomTags->SetItem(index,5,cellValue.Format("%d",valueOffset));
		//lstDicomTags->SetItem(index,6,cellValue.Format("%d",valueLength));
		lstDicomTags->SetItem(index,7,cellValue);
		index++;
	}

	/* any siemens specific CSA tags */
	gdcm::CSAHeader csa;
	const gdcm::DataSet& ds1 = r.GetFile().GetDataSet();

	//const gdcm::PrivateTag &t1 = csa.GetCSAImageHeaderInfoTag();
	const gdcm::PrivateTag &t1 = csa.GetCSASeriesHeaderInfoTag();

	bool found = false;
	int ret = 0;
	if( ds1.FindDataElement( t1 ) )
	{
		csa.LoadFromDataElement( ds1.GetDataElement( t1 ) );
		csa.Print( std::cout );
		found = true;
		if( csa.GetFormat() == gdcm::CSAHeader::ZEROED_OUT )
		{
			std::cout << "CSA Header has been zero-out (contains only 0)" << std::endl;
			ret = 1;
		}
		else if( csa.GetFormat() == gdcm::CSAHeader::DATASET_FORMAT )
		{
			const gdcm::DataSet& ds2 = csa.GetDataSet();
			/* loop through all the doc elements */
			gdcm::DataSet::ConstIterator it1 = ds2.Begin();
			for( ; it1 != ds2.End(); ++it1)
			{
				const gdcm::DataElement &ref2 = *it1;
				//tag = ref2.GetTag();
				std::pair<std::string, std::string> s = sf.ToStringPair( ref2.GetTag() );

				element = ref2.GetTag().GetElement();
				group = ref2.GetTag().GetGroup();
				entryName = s.first;
				entryVR = "0";
				entryVM = s.second;
				valueLength = 0;
				valueOffset = 0;
				cellValue = s.second;
				//if( !s.second.empty() || ref2.GetVL() == 0 ) {
				//	std::cout << s.first << " -> " << s.second << std::endl;
				//}
				//else if( !ref2.GetByteValue() ) {  // It means it's a SQ
				//	std::cout << "SQ:" << ref2.GetTag() << std::endl;
				//	entryValue = "SQ";
				//}
				//else if( ref2.GetTag().IsPrivate() ) {
				//	std::cout << "Private:" << ref2.GetTag() << std::endl;
				//	entryName = "Private";
				//}
				//else {
				//	std::cerr << "Not supported: " << ref2 << std::endl;
				//	entryName = "Unsupported";
				//}
				/* populate the grid */
				lstDicomTags->InsertItem(index,cellValue.Format("(%04X,%04X)",group,element));
				lstDicomTags->SetItem(index,1,entryKey);
				lstDicomTags->SetItem(index,2,entryName);
				lstDicomTags->SetItem(index,3,entryVR);
				lstDicomTags->SetItem(index,4,entryVM);
				lstDicomTags->SetItem(index,5,cellValue.Format("%d",valueOffset));
				lstDicomTags->SetItem(index,6,cellValue.Format("%d",valueLength));
				lstDicomTags->SetItem(index,7,cellValue);
				index++;
			}
			//f.SetDataSet( csa.GetDataSet() );
			//p.SetFile( f );
			//p.Print( std::cout );
		}
	}
	//if( ds.FindDataElement( t2 ) )
	//{
	//	csa.LoadFromDataElement( ds.GetDataElement( t2 ) );
	//	csa.Print( std::cout );
	//	found = true;
	//	if( csa.GetFormat() == gdcm::CSAHeader::ZEROED_OUT )
	//	{
	//		std::cout << "CSA Header has been zero-out (contains only 0)" << std::endl;
	//		ret = 1;
	//	}
	//	else if( csa.GetFormat() == gdcm::CSAHeader::DATASET_FORMAT )
	//	{
	//		gdcm::Printer p;
	//		gdcm::File f;
	//		f.SetDataSet( csa.GetDataSet() );
	//		p.SetFile( f );
	//		p.Print( std::cout );
	//	}
	//}
	//if( !found )
	//{
	//	std::cout << "no csa tag found" << std::endl;
	//	ret = 1;
	//}


	//while (docEntry) {
	//	group = docEntry->GetGroup();
	//	element = docEntry->GetElement();
	//	valueOffset = docEntry->GetOffset();
	//	offsets.Add(valueOffset);
	//	valueLength = docEntry->GetLength();
	//	lengths.Add(valueLength);
	//	dKey = docEntry->GetKey();
	//	std::string dName = docEntry->GetName();
	//	dVR = docEntry->GetVR();
	//	std::string dVM = docEntry->GetVM();
	//	entryKey = dKey.str();
	//	entryName = dName.c_str();
	//	entryVR = dVR.str();
	//	entryVM = dVM.c_str();

	//	/* populate the grid */
	//	lstDicomTags->InsertItem(index,cellValue.Format("(%d,%d)",group,element));
	//	lstDicomTags->SetItem(index,1,entryKey);
	//	lstDicomTags->SetItem(index,2,entryName);
	//	lstDicomTags->SetItem(index,3,entryVR);
	//	lstDicomTags->SetItem(index,4,entryVM);
	//	lstDicomTags->SetItem(index,5,cellValue.Format("%d",valueOffset));
	//	lstDicomTags->SetItem(index,6,cellValue.Format("%d",valueLength));

	//	index++;
	//	docEntry = dcfile->GetNextEntry();
	//}
	//dcfile->CloseFile();
	//dcfile->Delete();

	/* open the dicom file for reading, the file can only be opened after the gdcm::File has been closed */
//	dicomfile.Open(filename.c_str(),wxFile::read);
//
//	/* go through the previous doc entries, and get the actual value */
//	for (idx=0; idx<index;idx++) {
//		// get the entry value
//		if (lengths[idx] <= 5500) {
//			/* create a buffer */
//			valueBuffer = new char[lengths[idx]+1];
//			dicomfile.Seek(offsets[idx],wxFromStart);
//
//			bytesRead = dicomfile.Read((void *)valueBuffer, lengths[idx]);
//			valueBuffer[lengths[idx]] = '\0'; /* pad the string with a null terminator */
//			cellValue = valueBuffer;
//
//			lstDicomTags->SetItem(idx,7,cellValue);
////			logWnd->AppendText(msg.Format("Seek %d, Read %d bytes... actual read: %d\n",offsets[idx], lengths[idx], bytesRead));
//
//			delete[] valueBuffer;
//		}
//	}
//	dicomfile.Close();
}


// --------------------------------------------------------
// ---------- PopulateNiftiTags ---------------------------
// --------------------------------------------------------
void ImageInformationDialog::PopulateNiftiTags()
{
	wxString str;
	int i;

	/* get rid of everything, in case there is something left over */
	lstNiftiTags->ClearAll();

	/* setup the columns */
	wxListItem colItem;
	colItem.SetText("Name");
	lstNiftiTags->InsertColumn(0,colItem);
	colItem.SetText("Value");
	lstNiftiTags->InsertColumn(1,colItem);

	i = 0;
	lstNiftiTags->InsertItem(0,"Num Dimensions"); lstNiftiTags->SetItem(i,1,str.Format("%d",nftInfo->Get_ndim()));
	lstNiftiTags->InsertItem(++i,"Dimension X"); lstNiftiTags->SetItem(i,1,str.Format("%d",nftInfo->Get_nx()));
	lstNiftiTags->InsertItem(++i,"Dimension Y"); lstNiftiTags->SetItem(i,1,str.Format("%d",nftInfo->Get_ny()));
	lstNiftiTags->InsertItem(++i,"Dimension Z"); lstNiftiTags->SetItem(i,1,str.Format("%d",nftInfo->Get_nz()));
	lstNiftiTags->InsertItem(++i,"Dimension T"); lstNiftiTags->SetItem(i,1,str.Format("%d",nftInfo->Get_nt()));
	lstNiftiTags->InsertItem(++i,"Dimension U"); lstNiftiTags->SetItem(i,1,str.Format("%d",nftInfo->Get_nu()));
	lstNiftiTags->InsertItem(++i,"Dimension V"); lstNiftiTags->SetItem(i,1,str.Format("%d",nftInfo->Get_nv()));
	lstNiftiTags->InsertItem(++i,"Dimension W"); lstNiftiTags->SetItem(i,1,str.Format("%d",nftInfo->Get_nw()));
	lstNiftiTags->InsertItem(++i,"Num Voxels"); lstNiftiTags->SetItem(i,1,str.Format("%d",nftInfo->Get_nvox()));
	lstNiftiTags->InsertItem(++i,"Bytes Per Voxel"); lstNiftiTags->SetItem(i,1,str.Format("%d",nftInfo->Get_nbyper()));
	lstNiftiTags->InsertItem(++i,"Grid Spacing X"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_dx()));
	lstNiftiTags->InsertItem(++i,"Grid Spacing Y"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_dy()));
	lstNiftiTags->InsertItem(++i,"Grid Spacing Z"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_dz()));
	lstNiftiTags->InsertItem(++i,"Grid Spacing T"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_dt()));
	lstNiftiTags->InsertItem(++i,"Grid Spacing U"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_du()));
	lstNiftiTags->InsertItem(++i,"Grid Spacing V"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_dv()));
	lstNiftiTags->InsertItem(++i,"Grid Spacing W"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_dw()));
	lstNiftiTags->InsertItem(++i,"Scale Slope"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_scl_slope()));
	lstNiftiTags->InsertItem(++i,"Scale Intercept"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_scl_inter()));
	lstNiftiTags->InsertItem(++i,"Minimum Calibration"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_cal_min()));
	lstNiftiTags->InsertItem(++i,"Maximum Calibration"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_cal_max()));

	lstNiftiTags->InsertItem(++i,"QForm code");
	switch (nftInfo->Get_qform_code()) {
		case NIFTI_XFORM_UNKNOWN: lstNiftiTags->SetItem(i,1,str.Format("Unknown")); break;
		case NIFTI_XFORM_SCANNER_ANAT: lstNiftiTags->SetItem(i,1,str.Format("Scanner-based anatomical coordinates")); break;
		case NIFTI_XFORM_ALIGNED_ANAT: lstNiftiTags->SetItem(i,1,str.Format("Coordinates aligned to another file's, or to anatomical 'truth'")); break;
		case NIFTI_XFORM_TALAIRACH: lstNiftiTags->SetItem(i,1,str.Format("Coordinates aligned to Talairach-Tournoux Atlas; (0,0,0)=AC, etc")); break;
		case NIFTI_XFORM_MNI_152: lstNiftiTags->SetItem(i,1,str.Format("MNI 152 normalized coordinates")); break;
	}

	lstNiftiTags->InsertItem(++i,"SForm code");
	switch (nftInfo->Get_sform_code()) {
		case NIFTI_XFORM_UNKNOWN: lstNiftiTags->SetItem(i,1,str.Format("Unknown")); break;
		case NIFTI_XFORM_SCANNER_ANAT: lstNiftiTags->SetItem(i,1,str.Format("Scanner-based anatomical coordinates")); break;
		case NIFTI_XFORM_ALIGNED_ANAT: lstNiftiTags->SetItem(i,1,str.Format("Coordinates aligned to another file's, or to anatomical 'truth'")); break;
		case NIFTI_XFORM_TALAIRACH: lstNiftiTags->SetItem(i,1,str.Format("Coordinates aligned to Talairach-Tournoux Atlas; (0,0,0)=AC, etc")); break;
		case NIFTI_XFORM_MNI_152: lstNiftiTags->SetItem(i,1,str.Format("MNI 152 normalized coordinates")); break;
	}
	lstNiftiTags->InsertItem(++i,"Frequency Dimension"); lstNiftiTags->SetItem(i,1,str.Format("%d",nftInfo->Get_freq_dim()));
	lstNiftiTags->InsertItem(++i,"Phase Dimension"); lstNiftiTags->SetItem(i,1,str.Format("%d",nftInfo->Get_phase_dim()));
	lstNiftiTags->InsertItem(++i,"Slice Dimension"); lstNiftiTags->SetItem(i,1,str.Format("%d",nftInfo->Get_slice_dim()));
	lstNiftiTags->InsertItem(++i,"Slice Code"); lstNiftiTags->SetItem(i,1,str.Format("%d",nftInfo->Get_slice_code()));
	lstNiftiTags->InsertItem(++i,"Slice Start"); lstNiftiTags->SetItem(i,1,str.Format("%d",nftInfo->Get_slice_start()));
	lstNiftiTags->InsertItem(++i,"Slice End"); lstNiftiTags->SetItem(i,1,str.Format("%d",nftInfo->Get_slice_end()));
	lstNiftiTags->InsertItem(++i,"Slice Duration"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_slice_duration()));
	lstNiftiTags->InsertItem(++i,"Quaternion B"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_quatern_b()));
	lstNiftiTags->InsertItem(++i,"Quaternion C"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_quatern_c()));
	lstNiftiTags->InsertItem(++i,"Quaternion D"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_quatern_d()));
	lstNiftiTags->InsertItem(++i,"Quaternion Offset X"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_qoffset_x()));
	lstNiftiTags->InsertItem(++i,"Quaternion Offset Y"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_qoffset_y()));
	lstNiftiTags->InsertItem(++i,"Quaternion Offset Z"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_qoffset_z()));
	lstNiftiTags->InsertItem(++i,"Qfac"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_qfac()));
	lstNiftiTags->InsertItem(++i,"Time Coord Offset"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_toffset()));

	lstNiftiTags->InsertItem(++i,"XYZ Units");
	switch (nftInfo->Get_xyz_units()) {
		case NIFTI_UNITS_UNKNOWN: lstNiftiTags->SetItem(i,1,"Unknown"); break;
		case NIFTI_UNITS_METER: lstNiftiTags->SetItem(i,1,"Meters"); break;
		case NIFTI_UNITS_MM: lstNiftiTags->SetItem(i,1,"Millimeters"); break;
		case NIFTI_UNITS_MICRON: lstNiftiTags->SetItem(i,1,"Micrometers"); break;
		case NIFTI_UNITS_SEC: lstNiftiTags->SetItem(i,1,"Second"); break;
		case NIFTI_UNITS_MSEC: lstNiftiTags->SetItem(i,1,"Millisecond"); break;
		case NIFTI_UNITS_USEC: lstNiftiTags->SetItem(i,1,"Microseconds"); break;
		case NIFTI_UNITS_HZ: lstNiftiTags->SetItem(i,1,"Hertz"); break;
		case NIFTI_UNITS_PPM: lstNiftiTags->SetItem(i,1,"Parts/Million"); break;
		case NIFTI_UNITS_RADS: lstNiftiTags->SetItem(i,1,"Radians/sec"); break;
	}

	lstNiftiTags->InsertItem(++i,"Time Units");
	switch (nftInfo->Get_time_units()) {
		case NIFTI_UNITS_UNKNOWN: lstNiftiTags->SetItem(i,1,"Unknown"); break;
		case NIFTI_UNITS_METER: lstNiftiTags->SetItem(i,1,"Meters"); break;
		case NIFTI_UNITS_MM: lstNiftiTags->SetItem(i,1,"Millimeters"); break;
		case NIFTI_UNITS_MICRON: lstNiftiTags->SetItem(i,1,"Micrometers"); break;
		case NIFTI_UNITS_SEC: lstNiftiTags->SetItem(i,1,"Second"); break;
		case NIFTI_UNITS_MSEC: lstNiftiTags->SetItem(i,1,"Millisecond"); break;
		case NIFTI_UNITS_USEC: lstNiftiTags->SetItem(i,1,"Microseconds"); break;
		case NIFTI_UNITS_HZ: lstNiftiTags->SetItem(i,1,"Hertz"); break;
		case NIFTI_UNITS_PPM: lstNiftiTags->SetItem(i,1,"Parts/Million"); break;
		case NIFTI_UNITS_RADS: lstNiftiTags->SetItem(i,1,"Radians/sec"); break;
	}

	lstNiftiTags->InsertItem(++i,"Nifti Type");
	switch (nftInfo->Get_nifti_type()) {
		case 0: lstNiftiTags->SetItem(i,1,"Analyze"); break;
		case 1: lstNiftiTags->SetItem(i,1,"Nifti-1 (1 file)"); break;
		case 2: lstNiftiTags->SetItem(i,1,"Nifti-1 (2 files)"); break;
		case 3: lstNiftiTags->SetItem(i,1,"Nifti-ASCII"); break;
	}

	lstNiftiTags->InsertItem(++i,"Intent Type");
	switch (nftInfo->Get_intent_code()) {
		case NIFTI_INTENT_NONE: lstNiftiTags->SetItem(i,1,"None"); break;
		case NIFTI_INTENT_CORREL: lstNiftiTags->SetItem(i,1,"Correlation"); break;
		case NIFTI_INTENT_TTEST: lstNiftiTags->SetItem(i,1,"Students T-test"); break;
		case NIFTI_INTENT_FTEST: lstNiftiTags->SetItem(i,1,"F Test"); break;
		case NIFTI_INTENT_ZSCORE: lstNiftiTags->SetItem(i,1,"Z Score"); break;
		case NIFTI_INTENT_CHISQ: lstNiftiTags->SetItem(i,1,"Chi Squared"); break;
		case NIFTI_INTENT_BETA: lstNiftiTags->SetItem(i,1,"Beta Distribution"); break;
		case NIFTI_INTENT_BINOM: lstNiftiTags->SetItem(i,1,"Binomial Distribution"); break;
		case NIFTI_INTENT_GAMMA: lstNiftiTags->SetItem(i,1,"Gamma Distribution"); break;
		case NIFTI_INTENT_POISSON: lstNiftiTags->SetItem(i,1,"Poisson Distribution"); break;
		case NIFTI_INTENT_NORMAL: lstNiftiTags->SetItem(i,1,"Normal Distribution"); break;
		case NIFTI_INTENT_FTEST_NONC: lstNiftiTags->SetItem(i,1,"Non-central F statistic"); break;
		case NIFTI_INTENT_CHISQ_NONC: lstNiftiTags->SetItem(i,1,"Non-central Chi-squared statistic"); break;
		case NIFTI_INTENT_LOGISTIC: lstNiftiTags->SetItem(i,1,"Logistic Distribution"); break;
		case NIFTI_INTENT_LAPLACE: lstNiftiTags->SetItem(i,1,"Laplace Distribution"); break;
		case NIFTI_INTENT_UNIFORM: lstNiftiTags->SetItem(i,1,"Uniform Distribution"); break;
		case NIFTI_INTENT_TTEST_NONC: lstNiftiTags->SetItem(i,1,"Non-central T statistic"); break;
		case NIFTI_INTENT_WEIBULL: lstNiftiTags->SetItem(i,1,"Weibull Distribution"); break;
		case NIFTI_INTENT_CHI: lstNiftiTags->SetItem(i,1,"Chi Distribution"); break;
		case NIFTI_INTENT_INVGAUSS: lstNiftiTags->SetItem(i,1,"Inverse Gaussian"); break;
		case NIFTI_INTENT_EXTVAL: lstNiftiTags->SetItem(i,1,"Extreme Value Type I"); break;
		case NIFTI_INTENT_PVAL: lstNiftiTags->SetItem(i,1,"P-value"); break;
		case NIFTI_INTENT_LOGPVAL: lstNiftiTags->SetItem(i,1,"ln(p-value)"); break;
		case NIFTI_INTENT_LOG10PVAL: lstNiftiTags->SetItem(i,1,"log10(p-value)"); break;
		case NIFTI_INTENT_ESTIMATE: lstNiftiTags->SetItem(i,1,"Estimate"); break;
		case NIFTI_INTENT_LABEL: lstNiftiTags->SetItem(i,1,"Label"); break;
		case NIFTI_INTENT_NEURONAME: lstNiftiTags->SetItem(i,1,"NeuroName"); break;
		case NIFTI_INTENT_GENMATRIX: lstNiftiTags->SetItem(i,1,"M x N matrix at each voxel"); break;
		case NIFTI_INTENT_SYMMATRIX: lstNiftiTags->SetItem(i,1,"N x N matrix at each voxel"); break;
		case NIFTI_INTENT_DISPVECT: lstNiftiTags->SetItem(i,1,"Displacement vector"); break;
		case NIFTI_INTENT_VECTOR: lstNiftiTags->SetItem(i,1,"Vector"); break;
		case NIFTI_INTENT_POINTSET: lstNiftiTags->SetItem(i,1,"Spatial Coordinates"); break;
		case NIFTI_INTENT_TRIANGLE: lstNiftiTags->SetItem(i,1,"Triples"); break;
		case NIFTI_INTENT_QUATERNION: lstNiftiTags->SetItem(i,1,"Quaternions"); break;
		case NIFTI_INTENT_DIMLESS: lstNiftiTags->SetItem(i,1,"Dimensionless"); break;
	}
	lstNiftiTags->InsertItem(++i,"Intent Parameter 1"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_intent_p1()));
	lstNiftiTags->InsertItem(++i,"Intent Parameter 2"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_intent_p2()));
	lstNiftiTags->InsertItem(++i,"Intent Parameter 3"); lstNiftiTags->SetItem(i,1,str.Format("%.6f",nftInfo->Get_intent_p3()));
	lstNiftiTags->InsertItem(++i,"Intent Name"); lstNiftiTags->SetItem(i,1,nftInfo->Get_intent_name());
	lstNiftiTags->InsertItem(++i,"Description"); lstNiftiTags->SetItem(i,1,nftInfo->Get_descrip());
	lstNiftiTags->InsertItem(++i,"Auxiliary filename"); lstNiftiTags->SetItem(i,1,nftInfo->Get_aux_file());
	lstNiftiTags->InsertItem(++i,"Header filename"); lstNiftiTags->SetItem(i,1,nftInfo->Get_fname());
	lstNiftiTags->InsertItem(++i,"Image filename"); lstNiftiTags->SetItem(i,1,nftInfo->Get_iname());
	lstNiftiTags->InsertItem(++i,"Data start offset"); lstNiftiTags->SetItem(i,1,str.Format("%d",nftInfo->Get_iname_offset()));
	lstNiftiTags->InsertItem(++i,"Swap size"); lstNiftiTags->SetItem(i,1,str.Format("%d",nftInfo->Get_swapsize()));
	lstNiftiTags->InsertItem(++i,"Byte order"); lstNiftiTags->SetItem(i,1,str.Format("%d",nftInfo->Get_byteorder()));
	lstNiftiTags->InsertItem(++i,"Number of Extensions"); lstNiftiTags->SetItem(i,1,str.Format("%d",nftInfo->Get_num_ext()));

}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_IMGINFO_FILE_CLOSE
 */

void ImageInformationDialog::OnImginfoFileCloseClick( wxCommandEvent& event )
{
    Close(true);
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_IMGINFO_HELP_HELP
 */

void ImageInformationDialog::OnImginfoHelpHelpClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_MENU_SELECTED event handler for ID_IMGINFO_HELP_HELP in ImageInformationDialog.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_MENU_SELECTED event handler for ID_IMGINFO_HELP_HELP in ImageInformationDialog. 
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_IMGINFO_HELP_ABOUT
 */

void ImageInformationDialog::OnImginfoHelpAboutClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_MENU_SELECTED event handler for ID_IMGINFO_HELP_ABOUT in ImageInformationDialog.
    // Before editing this code, remove the block markers.
    AboutDialog* window = new AboutDialog(this);
    window->Show(true);
////@end wxEVT_COMMAND_MENU_SELECTED event handler for ID_IMGINFO_HELP_ABOUT in ImageInformationDialog. 
}

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_FILE_CHOICE
 */

//void ImageInformationDialog::OnFileChoiceSelected( wxCommandEvent& event )
//{
//	wxString filename = fileChoice->GetStringSelection();
//
//	switch (imgData->GetFileType()) {
//		case FILETYPE_DICOM:
//			PopulatePatientTab();
//			PopulateImageTab();
//			PopulateHistogramTab();
//			PopulateDicomTags(filename);
////			mainNotebook->DeletePage(4);
//			break;
//		case FILETYPE_ANALYZE:
//			PopulatePatientTab();
//			PopulateImageTab();
//			PopulateHistogramTab();
//			PopulateNiftiTags();
////			mainNotebook->DeletePage(3);
//			break;
//		case FILETYPE_RASTER:
//			PopulateImageTab();
//			PopulateHistogramTab();
////			mainNotebook->DeletePage(0);
////			mainNotebook->DeletePage(2);
////			mainNotebook->DeletePage(2);
//			break;
//		default:
//			PopulatePatientTab();
//			PopulateImageTab();
//			PopulateHistogramTab();
//			PopulateDicomTags(filename);
//			PopulateNiftiTags();
//	}
//}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void ImageInformationDialog::OnClickClose( wxCommandEvent& event )
{
    Close(true);
}

/*!
 * Should we show tooltips?
 */

bool ImageInformationDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ImageInformationDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ImageInformationDialog bitmap retrieval
    wxUnusedVar(name);
    if (name == _T("../xpm/help.xpm"))
    {
        wxBitmap bitmap( help_xpm);
        return bitmap;
    }
    return wxNullBitmap;
////@end ImageInformationDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon ImageInformationDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ImageInformationDialog icon retrieval
    wxUnusedVar(name);
    if (name == _T("../xpm/brain.xpm"))
    {
        wxIcon icon( brain_xpm);
        return icon;
    }
    else if (name == _T("../xpm/smiley-bw.xpm"))
    {
        wxIcon icon( smiley_bw_xpm);
        return icon;
    }
    else if (name == _T("../xpm/mri.xpm"))
    {
        wxIcon icon( mri_xpm);
        return icon;
    }
    else if (name == _T("../xpm/histogram.xpm"))
    {
        wxIcon icon( histogram_xpm);
        return icon;
    }
    else if (name == _T("../xpm/dicomtags.xpm"))
    {
        wxIcon icon( dicomtags_xpm);
        return icon;
    }
    return wxNullIcon;
////@end ImageInformationDialog icon retrieval
}
/*!
 * wxEVT_PAINT event handler for ID_PANEL1
 */

void ImageInformationDialog::OnPaint( wxPaintEvent& event )
{
    wxPaintDC pdc(this);
	dc->SetMapMode(wxMM_TEXT);
}


void ImageInformationDialog::DrawHistogram()
{
	wxSize size;
	wxPen pen;
	wxBrush brush;
	int histoCount;
	int maxBucket = 0;
	unsigned char gray;
	int i;
	int x1, x2, y1, y2;
	int width;
	x1 = x2 = y1 = y2 = 0;

	histoCount = (int)histogram.Count();
	/* start drawing the histogram */
	wxBrush whiteBrush(wxColour(255, 255, 255), wxSOLID);
	dc->SetBackground(whiteBrush);
	dc->Clear();

	size = dc->GetSize();
	width = size.GetWidth();
	pen = wxPen(wxColour(0,0,0), 0, wxTRANSPARENT);
	dc->SetPen(pen);

	/* find the largest bucket */
	for (i=0;i<histoCount;i++)
		if (histogram[i] > maxBucket)
			maxBucket = histogram[i];

	/* draw the shaded bars in the background */
	for (i=0;i<width;i++) {
		gray = (unsigned char)(255.0*((double)i/(double)size.GetWidth()));
		brush = wxBrush(wxColour(gray, gray, gray, wxSOLID));
		dc->SetBrush(brush);

		x1 = i;
		y1 = 0;
		y2 = size.GetHeight();
		x2 = x1 + 1;
		dc->DrawRectangle(x1,y1,x2,y2);
	}
	
	/* draw the histogram */
	brush = wxBrush(wxColour(255, 0, 0, wxSOLID));
	dc->SetBrush(brush);
	for (i=0;i<histoCount;i++) {
		x1 = size.GetWidth()*((double)i/(double)histoCount);
		y1 = 0;
		y2 = size.GetHeight()*((double)histogram[i]/(double)maxBucket);
		x2 = x1 + 1;
		dc->DrawRectangle(x1,y1,x2,y2);
	}
}


void ImageInformationDialog::CalculateHistogram()
{
	int col, row, slice, vol;
	int maxVal = (int)imgInfo->GetMaxPixelValues().Gray();
	int minVal = (int)imgInfo->GetMinPixelValues().Gray();
	int totalRange = maxVal - minVal;
	int voxelValue;

	int xSize = imgInfo->GetImageXSize();
	int ySize = imgInfo->GetImageYSize();
	int zSize = imgInfo->GetImageZSize();
	int tSize = imgInfo->GetImageTSize();

	histogram.Alloc(totalRange+1);
	histogram.SetCount(totalRange+1,0);

	/* loop through all elements */
	switch (imgInfo->GetPixelType()) {
		case IMG_BINARY: break; /* not implemented yet */
		case IMG_UINT8:
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = (int)imgData->imgDataUINT8->Get(col,row,slice,vol,0);
				histogram[voxelValue]++;
			}
			break;
		case IMG_INT8:
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = (int)imgData->imgDataINT8->Get(col,row,slice,vol,0);
				histogram[voxelValue]++;
			}
			break;
		case IMG_UINT16:
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = (int)imgData->imgDataUINT16->Get(col,row,slice,vol,0);
				histogram[voxelValue-minVal]++;
			}
			break;
		case IMG_INT16:
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = (int)imgData->imgDataINT16->Get(col,row,slice,vol,0);
				histogram[voxelValue-minVal]++;
			}
			break;
		case IMG_UINT32:
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = imgData->imgDataUINT32->Get(col,row,slice,vol,0);
				histogram[voxelValue]++;
			}
			break;
		case IMG_INT32:
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = (int)imgData->imgDataINT32->Get(col,row,slice,vol,0);
				histogram[voxelValue]++;
			}
			break;
		case IMG_UINT64:
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = (int)imgData->imgDataUINT64->Get(col,row,slice,vol,0);
				histogram[voxelValue]++;
			}
			break;
		case IMG_INT64:
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = (int)imgData->imgDataINT64->Get(col,row,slice,vol,0);
				histogram[voxelValue]++;
			}
			break;
		case IMG_FLOAT32:
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = (int)imgData->imgDataFLOAT32->Get(col,row,slice,vol,0);
				histogram[voxelValue]++;
			}
			break;
		case IMG_FLOAT64:
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = (int)imgData->imgDataFLOAT64->Get(col,row,slice,vol,0);
				histogram[voxelValue]++;
			}
			break;
		case IMG_FLOAT128:
			//for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
			//	voxelValue = (int)imgData->imgDataFLOAT128->Get(col,row,slice,vol,0);
			//	histogram[voxelValue]++;
			//}
			//break;
		case IMG_COMPLEX64: /* also not implemented yet */
		case IMG_COMPLEX128:
		case IMG_COMPLEX256:
		case IMG_RGB24:
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = (int)imgData->imgDataRGB24->Get(col,row,slice,vol,0);
				histogram[voxelValue]++;
			}
			break;
		default: break;
	}

}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DRAW_HISTOGRAM
 */

void ImageInformationDialog::OnDrawHistogramClick( wxCommandEvent& event )
{
	DrawHistogram();
}


/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_FILE_COMBO
 */

void ImageInformationDialog::OnFileComboSelected( wxCommandEvent& event )
{
	wxString filename = cmbFile->GetStringSelection();

	switch (imgData->GetFileType()) {
		case FILETYPE_DICOM:
			PopulatePatientTab();
			PopulateImageTab();
			PopulateHistogramTab();
			PopulateDicomTags(filename);
//			mainNotebook->DeletePage(4);
			break;
		case FILETYPE_ANALYZE:
			PopulatePatientTab();
			PopulateImageTab();
			PopulateHistogramTab();
			PopulateNiftiTags();
//			mainNotebook->DeletePage(3);
			break;
		case FILETYPE_RASTER:
			PopulateImageTab();
			PopulateHistogramTab();
//			mainNotebook->DeletePage(0);
//			mainNotebook->DeletePage(2);
//			mainNotebook->DeletePage(2);
			break;
		default:
			PopulatePatientTab();
			PopulateImageTab();
			PopulateHistogramTab();
			PopulateDicomTags(filename);
			PopulateNiftiTags();
	}
}

