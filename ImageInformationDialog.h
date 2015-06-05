/* ----------------------------------------------------------------------------
	ImageInformationDialog.h


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



#ifndef _IMAGEINFORMATIONDIALOG_H_
#define _IMAGEINFORMATIONDIALOG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "ImageInformationDialog.h"
#endif

#include "XYZTriplet.h"
#include "ImageData.h"
#include "ImageInfo.h"
#include "DicomInfo.h"
#include "NiftiInfo.h"

/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/notebook.h"
#include "wx/statline.h"
#include "wx/listctrl.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxBoxSizer;
class wxNotebook;
class wxFlexGridSizer;
class wxListCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_FRAME 14000
#define ID_IMGINFO_FILE_CLOSE 14002
#define ID_IMGINFO_HELP_HELP 14003
#define ID_IMGINFO_HELP_ABOUT 14004
#define ID_MAINPANEL 14005
#define ID_STATIC_FILES 14006
#define ID_FILE_COMBO 14077
#define ID_NOTEBOOK 14008
#define ID_PANEL_PATIENT 14009
#define ID_STATIC_PATIENTNAME 14010
#define ID_PATIENTNAME 14011
#define ID_STATIC_PATIENTID 14012
#define ID_PATIENTID 14013
#define ID_STATIC_BIRTHDATE 14014
#define ID_BIRTHDATE 14015
#define ID_STATIC_STUDY 14016
#define ID_STUDY 14017
#define ID_STATIC_STUDYDATETIME 14018
#define ID_STUDYDATETIME 14019
#define ID_STATIC_SERIES 14020
#define ID_SERIES 14021
#define ID_STATIC_SERIESDATETIME 14022
#define ID_SERIESDATETIME 14023
#define ID_STATIC_PHYSICIAN 14024
#define ID_PHYSICIAN 14025
#define ID_STATIC_PROTOCOL 14026
#define ID_PROTOCOL 14027
#define ID_PANEL_IMAGE 14028
#define ID_STATIC_X 14029
#define ID_STATIC_Y 14030
#define ID_STATIC_Z 14031
#define ID_STATIC_T 14068
#define ID_STATIC_PIXELSIZE 14032
#define ID_PIXELSIZE_X 14033
#define ID_PIXELSIZE_Y 14034
#define ID_PIXELSIZE_Z 14035
#define ID_PIXELSIZE_T 14070
#define ID_STATIC_VOLUMESIZE 14071
#define ID_VOLUMESIZE_X 14072
#define ID_VOLUMESIZE_Y 14073
#define ID_VOLUMESIZE_Z 14074
#define ID_VOLUMESIZE_T 14075
#define ID_STATIC_PIXELSPACE 14036
#define ID_PIXELSPACE_X 14037
#define ID_PIXELSPACE_Y 14038
#define ID_PIXELSPACE_Z 14039
#define ID_PIXELSPACE_T 14069
#define ID_STATIC_ORIGIN 14040
#define ID_ORIGIN_X 14041
#define ID_ORIGIN_Y 14042
#define ID_ORIGIN_Z 14043
#define ID_STATIC_IMAGETYPE 14044
#define ID_IMAGETYPE 14045
#define ID_STATIC_NUMVOLS 14046
#define ID_VOLUMETYPE 14047
#define ID_STATIC_ISRGB 14048
#define ID_ISRGB 14049
#define ID_STATIC_PIXELTYPE 14050
#define ID_PIXELTYPE 14051
#define ID_STATIC_MODALITY 14052
#define ID_MODALITY 14053
#define ID_STATIC_ISMULTIFILE 14054
#define ID_ISMULTIFILE 14055
#define ID_STATIC_ISSIGNED 14056
#define ID_ISSIGNED 14057
#define ID_STATIC_PAT_POS 14058
#define ID_PAT_POS 14059
#define ID_STATIC_DIR_COS 14060
#define ID_DIR_COS 14061
#define ID_PANEL_HISTOGRAM 14062
#define ID_DRAW_HISTOGRAM 14001
#define ID_HISTOGRAM 14063
#define ID_PANEL_DICOMTAGS 14064
#define ID_DICOMTAG_LIST 14065
#define ID_DICOM_TAGS 14076
#define ID_PANELNIFTITAGS 14066
#define ID_NIFTITAG_LIST 14067
#define SYMBOL_IMAGEINFORMATIONDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxCLOSE_BOX
#define SYMBOL_IMAGEINFORMATIONDIALOG_TITLE _("Image Information")
#define SYMBOL_IMAGEINFORMATIONDIALOG_IDNAME ID_FRAME
#define SYMBOL_IMAGEINFORMATIONDIALOG_SIZE wxSize(600, 500)
#define SYMBOL_IMAGEINFORMATIONDIALOG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * ImageInformationDialog class declaration
 */

class ImageInformationDialog: public wxFrame
{    
    DECLARE_CLASS( ImageInformationDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ImageInformationDialog( );
    ImageInformationDialog( wxWindow* parent, wxWindowID id = SYMBOL_IMAGEINFORMATIONDIALOG_IDNAME, const wxString& caption = SYMBOL_IMAGEINFORMATIONDIALOG_TITLE, const wxPoint& pos = SYMBOL_IMAGEINFORMATIONDIALOG_POSITION, const wxSize& size = SYMBOL_IMAGEINFORMATIONDIALOG_SIZE, long style = SYMBOL_IMAGEINFORMATIONDIALOG_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_IMAGEINFORMATIONDIALOG_IDNAME, const wxString& caption = SYMBOL_IMAGEINFORMATIONDIALOG_TITLE, const wxPoint& pos = SYMBOL_IMAGEINFORMATIONDIALOG_POSITION, const wxSize& size = SYMBOL_IMAGEINFORMATIONDIALOG_SIZE, long style = SYMBOL_IMAGEINFORMATIONDIALOG_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

	void SetInfoPointers(ImageData *imageData, ImageInfo *imageInfo, DicomInfo *dicomInfo, NiftiInfo *niftiInfo);
	void Init();

////@begin ImageInformationDialog event handler declarations

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_IMGINFO_FILE_CLOSE
    void OnImginfoFileCloseClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_IMGINFO_HELP_HELP
    void OnImginfoHelpHelpClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_IMGINFO_HELP_ABOUT
    void OnImginfoHelpAboutClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_FILE_COMBO
    void OnFileComboSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DRAW_HISTOGRAM
    void OnDrawHistogramClick( wxCommandEvent& event );

    /// wxEVT_PAINT event handler for ID_HISTOGRAM
    void OnPaint( wxPaintEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnClickClose( wxCommandEvent& event );

////@end ImageInformationDialog event handler declarations

////@begin ImageInformationDialog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end ImageInformationDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

private:
	ImageData *imgData;
	ImageInfo *imgInfo;
	DicomInfo *dcmInfo;
	NiftiInfo *nftInfo;

	bool controlsCreated;
	wxArrayInt histogram;
	wxClientDC *dc;

	void OnSize(wxSizeEvent& event);

	void PopulateFilelist();
	void PopulatePatientTab();
	void PopulateImageTab();
	void PopulateHistogramTab();
	void PopulateDicomTags(wxString filename);
	void PopulateNiftiTags();
	void CalculateHistogram();
	void DrawHistogram();

////@begin ImageInformationDialog member variables
    wxPanel* mainPanel;
    wxBoxSizer* mainSizer;
    wxBoxSizer* fileSizer;
    wxComboBox* cmbFile;
    wxNotebook* mainNotebook;
    wxPanel* pnlPatient;
    wxFlexGridSizer* patientSizer;
    wxStaticText* txtPatientName;
    wxStaticText* txtPatientID;
    wxStaticText* txtBirthdate;
    wxStaticText* txtStudy;
    wxStaticText* txtStudyDateTime;
    wxStaticText* txtSeries;
    wxStaticText* txtSeriesDateTime;
    wxStaticText* txtPhysician;
    wxStaticText* txtProtocol;
    wxPanel* pnlImage;
    wxBoxSizer* imageSizer;
    wxStaticBox* dimOutsideSizer;
    wxFlexGridSizer* dimSizer;
    wxStaticText* txtPixelSizeX;
    wxStaticText* txtPixelSizeY;
    wxStaticText* txtPixelSizeZ;
    wxStaticText* txtPixelSizeT;
    wxStaticText* txtVolumeSizeX;
    wxStaticText* txtVolumeSizeY;
    wxStaticText* txtVolumeSizeZ;
    wxStaticText* txtVolumeSizeT;
    wxStaticText* txtPixelSpacingX;
    wxStaticText* txtPixelSpacingY;
    wxStaticText* txtPixelSpacingZ;
    wxStaticText* txtPixelSpacingT;
    wxStaticText* txtOriginX;
    wxStaticText* txtOriginY;
    wxStaticText* txtOriginZ;
    wxFlexGridSizer* imageInfoSizer;
    wxStaticText* txtImageType;
    wxStaticText* txtVolumeType;
    wxStaticText* txtIsRGB;
    wxStaticText* txtPixelType;
    wxStaticText* txtModality;
    wxStaticText* txtIsMultiFile;
    wxStaticText* txtIsSigned;
    wxStaticText* txtPatientPosition;
    wxStaticText* txtDirectionCosines;
    wxStaticText* txtMaxImageRGB;
    wxStaticText* txtMinImageRGB;
    wxStaticText* txtMaxImageGray;
    wxStaticText* txtMinImageGray;
    wxStaticText* txtMaxVolumeRGB;
    wxStaticText* txtMinVolumeRGB;
    wxStaticText* txtMaxVolumeGray;
    wxStaticText* txtMinVolumeGray;
    wxButton* btnDrawHistogram;
    wxPanel* pnlHistogram;
    wxPanel* pnlDicomTags;
    wxListCtrl* lstDicomTags;
    wxTextCtrl* txtDicomTags;
    wxPanel* pnlNiftiTags;
    wxListCtrl* lstNiftiTags;
    wxButton* btnClose;
////@end ImageInformationDialog member variables
};


#endif
    // _IMAGEINFORMATIONDIALOG_H_
