/////////////////////////////////////////////////////////////////////////////
// Name:        ConvertDialog.h
// Purpose:     
// Author:      Gregory Book
// Created:     14/03/2011 10:38:18
// Copyright:   The Book Company
/////////////////////////////////////////////////////////////////////////////

#ifndef _CONVERTDIALOG_H_
#define _CONVERTDIALOG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "ConvertDialog.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/listctrl.h"
#include "wx/spinctrl.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxListCtrl;
class wxBoxSizer;
class wxSpinCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_CONVERTDIALOG 19000
#define ID_INTYPE_RASTER 19028
#define ID_INTYPE_2DDICOM 19029
#define ID_INTYPE_2DDICOMMOSAIC 19041
#define ID_INTYPE_3DDICOM 19042
#define ID_INTYPE_3DNIFTI 19043
#define ID_INTYPE_4DNIFTI 19044
#define ID_CLEAR_FILES 19034
#define ID_REMOVE_FILES 19035
#define ID_CONVERT_ADD_FILES 19036
#define ID_CONVERT_LIST_FILES 19037
#define ID_OUTTYPE_RASTER 19045
#define ID_OUTTYPE_2DDICOM 19046
#define ID_OUTTYPE_2DDICOMMOS 19047
#define ID_OUTTYPE_3DDICOM 19048
#define ID_OUTTYPE_3DNIFTI 19049
#define ID_OUTTYPE_4DNIFTI 19050
#define ID_SUBOUT_DICOM 19012
#define ID_SUBOUT_ANALYZE 19001
#define ID_SUBOUT_NIFTI 19002
#define ID_SUBOUT_NIFTI2FILE 19003
#define ID_SUBOUT_NIFTIASCII 19004
#define ID_SUBOUT_JPEG 19005
#define ID_SUBOUT_TIFF 19052
#define ID_SUBOUT_PNG 19053
#define ID_SUBOUT_BMP 19054
#define ID_CONV_MOSAIC_XSIZE 19006
#define ID_CONV_MOSAIC_YSIZE 19007
#define ID_SPINCTRL2 19008
#define ID_ORIGINAL_IS_INTERLEAVED 19017
#define ID_VOL_START_ODD 19018
#define ID_VOL_START_EVEN 19019
#define ID_FLIPX 19020
#define ID_FLIPY 19021
#define ID_FLIPZ 19022
#define ID_SORTBY_FILENAME 19023
#define ID_SORTBY_SLICE 19024
#define ID_OUT_DATATYPE 19009
#define ID_TEXTCTRL25 19014
#define ID_TEXTCTRL240 19013
#define ID_TEXTCTRL26 19015
#define ID_TEXTCTRL100 19016
#define ID_TEXTCTRL101 19025
#define ID_TEXTCTRL102 19030
#define ID_TEXTCTRL103 19026
#define ID_TEXTCTRL104 19031
#define ID_CHOICE 19055
#define ID_CHOICE1 19051
#define ID_OUTDIR_EQUAL_INDIR 19039
#define ID_OUTPUT_DIR 19032
#define ID_SELECT_DIR 19033
#define ID_KEEP_FILENAME 19010
#define ID_USE_FILENAME 19011
#define ID_NEW_FILENAME 19027
#define ID_PAD_SIZE 19040
#define ID_CONVERT 19038
#define SYMBOL_CONVERTDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX
#define SYMBOL_CONVERTDIALOG_TITLE _("Convert Files...")
#define SYMBOL_CONVERTDIALOG_IDNAME ID_CONVERTDIALOG
#define SYMBOL_CONVERTDIALOG_SIZE wxDefaultSize
#define SYMBOL_CONVERTDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * ConvertDialog class declaration
 */

class ConvertDialog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( ConvertDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ConvertDialog();
    ConvertDialog( wxWindow* parent, wxWindowID id = SYMBOL_CONVERTDIALOG_IDNAME, const wxString& caption = SYMBOL_CONVERTDIALOG_TITLE, const wxPoint& pos = SYMBOL_CONVERTDIALOG_POSITION, const wxSize& size = SYMBOL_CONVERTDIALOG_SIZE, long style = SYMBOL_CONVERTDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_CONVERTDIALOG_IDNAME, const wxString& caption = SYMBOL_CONVERTDIALOG_TITLE, const wxPoint& pos = SYMBOL_CONVERTDIALOG_POSITION, const wxSize& size = SYMBOL_CONVERTDIALOG_SIZE, long style = SYMBOL_CONVERTDIALOG_STYLE );

    /// Destructor
    ~ConvertDialog();

	void SetLogWindow(MainFrameVariables *MainVars);
	wxString GetPaddedNumber(int numPlaces, int number);
	void EnableAllFormatOptions();
	void ConvertFile(int formatInType, wxArrayString filepaths, int loadType, int formatOutType, int convertType, wxFileName outDir, wxString outFilename, int i);

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin ConvertDialog event handler declarations

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_INTYPE_RASTER
    void OnIntypeRasterSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_INTYPE_2DDICOM
    void OnIntype2ddicomSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_INTYPE_2DDICOMMOSAIC
    void OnIntype2ddicommosaicSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_INTYPE_3DDICOM
    void OnIntype3ddicomSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_INTYPE_3DNIFTI
    void OnIntype3dniftiSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_INTYPE_4DNIFTI
    void OnIntype4dniftiSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_CLEAR_FILES
    void OnClearFilesClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_REMOVE_FILES
    void OnRemoveFilesClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_CONVERT_ADD_FILES
    void OnConvertAddFilesClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OUTTYPE_RASTER
    void OnOuttypeRasterSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OUTTYPE_2DDICOM
    void OnOuttype2ddicomSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OUTTYPE_2DDICOMMOS
    void OnOuttype2ddicommosSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OUTTYPE_3DDICOM
    void OnOuttype3ddicomSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OUTTYPE_3DNIFTI
    void OnOuttype3dniftiSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OUTTYPE_4DNIFTI
    void OnOuttype4dniftiSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_ORIGINAL_IS_INTERLEAVED
    void OnOriginalIsInterleavedClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SELECT_DIR
    void OnSelectDirClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_CONVERT
    void OnConvertClick( wxCommandEvent& event );

////@end ConvertDialog event handler declarations

////@begin ConvertDialog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end ConvertDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin ConvertDialog member variables
    wxRadioButton* radInRaster;
    wxRadioButton* radIn2DDicom;
    wxRadioButton* radIn2DDicomMosaic;
    wxRadioButton* radIn3DDicom;
    wxRadioButton* radIn3DNifti;
    wxRadioButton* radIn4DNifti;
    wxButton* btnClearFiles;
    wxButton* btnRemoveFiles;
    wxButton* btnAddFiles;
    wxListCtrl* lstFiles;
    wxRadioButton* radOutRaster;
    wxRadioButton* radOut2DDicom;
    wxRadioButton* radOut2DDicomMosaic;
    wxRadioButton* radOut3DDicom;
    wxRadioButton* radOut3DNifti;
    wxRadioButton* radOut4DNifti;
    wxRadioButton* radSubOutDicom;
    wxRadioButton* radSubOutAnalyze;
    wxRadioButton* radSubOutNifti;
    wxRadioButton* radSubOutNifti2File;
    wxRadioButton* radSubOutNiftiAscii;
    wxRadioButton* radSubOutJPEG;
    wxRadioButton* radSubOutTIFF;
    wxRadioButton* radSubOutPNG;
    wxRadioButton* radSubOutBMP;
    wxBoxSizer* sizerXYSize;
    wxStaticText* staticXSizeLabel;
    wxSpinCtrl* spinXSize;
    wxStaticText* staticYSizeLabel;
    wxSpinCtrl* spinYSize;
    wxBoxSizer* sizerNumberSlices;
    wxStaticText* staticNumberSlicesLabel;
    wxSpinCtrl* spinNumberOfSlices;
    wxCheckBox* chkOriginalIsInterleaved;
    wxRadioButton* radVolStartOdd;
    wxRadioButton* radVolStartEven;
    wxCheckBox* chkFlipX;
    wxCheckBox* chkFlipY;
    wxCheckBox* chkFlipZ;
    wxRadioButton* radSortByFilename;
    wxRadioButton* radSortBySlice;
    wxChoice* lstOutDatatype;
    wxTextCtrl* txtPatientName;
    wxTextCtrl* txtPatientID;
    wxTextCtrl* txtPatientDOB;
    wxTextCtrl* txtPatientSex;
    wxTextCtrl* txtStudyDate;
    wxTextCtrl* txtStudyTime;
    wxTextCtrl* txtSeriesDate;
    wxTextCtrl* txtSeriesTime;
    wxChoice* lstModality;
    wxChoice* lstPatientPosition;
    wxCheckBox* chkOutdirEqualIndir;
    wxTextCtrl* txtOutputDir;
    wxButton* btnSelectDir;
    wxRadioButton* radKeepFilename;
    wxRadioButton* radUseFilename;
    wxTextCtrl* txtNewFilename;
    wxSpinCtrl* spinPadSize;
    wxButton* btnConvert;
////@end ConvertDialog member variables

	//LogWindow *logWindow;
	MainFrameVariables *mainVars;
	int conversionType;
};

#endif
    // _CONVERTDIALOG_H_
