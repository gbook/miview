/* ----------------------------------------------------------------------------
	MainFrame.cpp


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
#pragma implementation "MainFrame.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
//#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes
#include "MainFrame.h"
#include "BugReport.h"
//#include "LogWindow.h"
#include "MainFrameVariables.h"
#include "ImageCanvas.h"
#include "AboutDialog.h"
#include "AnonymizeDialog.h"
#include "OptionsDialog.h"
#include "FileOpenDialog.h"
#include "ConvertDialog.h"
#include "SaveScreenshot.h"
#include "ColorMapDialog.h"
#include "MIViewBrowser.h"
#include "wx/colordlg.h"
#include "wx/file.h"

////@begin XPM images
#include "xpm/openfile.xpm"
#include "xpm/close32.xpm"
#include "xpm/normalview.xpm"
#include "xpm/brain3view.xpm"
#include "xpm/multisliceview2.xpm"
#include "xpm/mip.xpm"
#include "xpm/ssd.xpm"
#include "xpm/volume-rend.xpm"
#include "xpm/show-info-overlay.xpm"
#include "xpm/showlines.xpm"
#include "xpm/hi-res.xpm"
#include "xpm/image-info.xpm"
////@end XPM images

#include "xpm/openfile-16.xpm"
#include "xpm/close16.xpm"
#include "xpm/help.xpm"
#include "xpm/brain.xpm"

void *logWindow;
//void *openFileDialog;
MainFrame *frame;
wxString msg;

/* for the debugging of memory leaks */
//#include <crtdbg.h>
//#define new new(_NORMAL_BLOCK,__FILE__, __LINE__)


/*!
 * MainFrame type definition
 */

IMPLEMENT_CLASS( MainFrame, wxFrame )


BEGIN_EVENT_TABLE( MainFrame, wxFrame )

////@begin MainFrame event table entries
    EVT_CLOSE( MainFrame::OnCloseWindow )
    EVT_SIZE( MainFrame::OnSize )
    EVT_PAINT( MainFrame::OnPaint )
    EVT_MOUSEWHEEL( MainFrame::OnMouseWheel )
    EVT_KEY_DOWN( MainFrame::OnKeyDown )
    EVT_KEY_UP( MainFrame::OnKeyUp )

    EVT_MENU( ID_FILE_OPEN, MainFrame::OnFileOpen )

    EVT_MENU( ID_FILE_CLOSE, MainFrame::OnFileClose )

    EVT_MENU( ID_FILE_BROWSER, MainFrame::OnFileBrowserClick )

    EVT_MENU( ID_FILE_SAVE_SCREENGRAB, MainFrame::OnFileSaveScreengrabClick )

    EVT_MENU( ID_FILE_EXIT, MainFrame::OnFileExit )

    EVT_MENU( ID_EDIT_OPTIONS, MainFrame::OnEditOptions )

    EVT_MENU( ID_OPTIONS_FUNCTION_EDITOR, MainFrame::OnOptionsFunctionEditorClick )

    EVT_MENU( ID_VIEW_NORMAL, MainFrame::OnViewNormal )

    EVT_MENU( ID_VIEW_ORTHOGONAL, MainFrame::OnViewOrthogonal )

    EVT_MENU( ID_VIEW_MIP, MainFrame::OnViewMIP )

    EVT_MENU( ID_VIEW_SSD, MainFrame::OnViewSSD )

    EVT_MENU( ID_VIEW_VOL_REND, MainFrame::OnViewVolumeRendering )

    EVT_MENU( ID_VIEW_IMAGE_OVERLAY, MainFrame::OnShowOverlay )

    EVT_MENU( ID_IMAGE_INFORMATION, MainFrame::OnShowImageInformation )

    EVT_MENU( ID_FILE_ANONYMIZE, MainFrame::OnAnonymize )

    EVT_MENU( ID_FILE_CONVERT, MainFrame::OnFileConvertClick )

    EVT_MENU( ID_HELP_HELP, MainFrame::OnHelpHelp )

    EVT_MENU( ID_HELP_REPORTBUG, MainFrame::OnHelpReportbugClick )

    EVT_MENU( ID_HELP_ABOUT, MainFrame::OnHelpAbout )

    EVT_MENU( ID_MAIN_TOOLBAR_OPEN_FILES, MainFrame::OnFileOpen )

    EVT_MENU( ID_MAIN_TOOLBAR_CLOSE_FILE, MainFrame::OnFileClose )

    EVT_MENU( ID_MAIN_TOOLBAR_VIEW_NORMAL, MainFrame::OnViewNormal )

    EVT_MENU( ID_MAIN_TOOLBAR_VIEW_ORTHOGONAL, MainFrame::OnViewOrthogonal )

    EVT_MENU( ID_MAIN_TOOLBAR_VIEW_MULTISLICE, MainFrame::OnViewMultislice )

    EVT_MENU( ID_MAIN_TOOLBAR_VIEW_MIP, MainFrame::OnViewMIP )

    EVT_MENU( ID_MAIN_TOOLBAR_VIEW_SSD, MainFrame::OnViewSSD )

    EVT_MENU( ID_MAIN_TOOLBAR_VIEW_VOL_REND, MainFrame::OnViewVolumeRendering )

    EVT_MENU( ID_MAIN_TOOLBAR_SHOW_OVERLAY, MainFrame::OnShowOverlay )

    EVT_MENU( ID_MAIN_TOOLBAR_SHOW_LINES, MainFrame::OnMainToolbarShowLinesClick )

    EVT_MENU( ID_IMAGE_TOOLBAR_HIRES, MainFrame::OnViewHiRes )

    EVT_MENU( ID_MAIN_TOOLBAR_INFO, MainFrame::OnShowImageInformation )

    EVT_COMMAND_SCROLL( ID_SCROLLBAR, MainFrame::OnScrollSliceScroll )

    EVT_CHECKBOX( ID_BESTFIT_ZOOM, MainFrame::OnBestfitZoomClick )

    EVT_SLIDER( ID_ZOOM_SLIDER, MainFrame::OnZoomSliderUpdated )

    EVT_SPINCTRL( ID_SPIN_ZOOM, MainFrame::OnSpinZoomUpdated )
    EVT_TEXT( ID_SPIN_ZOOM, MainFrame::OnSpinZoomTextUpdated )

    EVT_RADIOBUTTON( ID_RADIO_USE_PLANE0, MainFrame::OnRadioUsePlaneSelected )

    EVT_RADIOBUTTON( ID_RADIO_USE_PLANE1, MainFrame::OnRadioUsePlaneSelected )

    EVT_RADIOBUTTON( ID_RADIO_USE_PLANE2, MainFrame::OnRadioUsePlaneSelected )

    EVT_SPINCTRL( ID_VOLUME_NUM, MainFrame::OnVolumeNumUpdated )

    EVT_RADIOBUTTON( ID_RENDERMETHOD_RAY_TRACE, MainFrame::OnRenderMethodSelected )

    EVT_RADIOBUTTON( ID_RENDERMETHOD_3D_TEXTURE, MainFrame::OnRenderMethodSelected )

    EVT_SPINCTRL( ID_NUM_STEPS, MainFrame::OnNumStepsUpdated )

    EVT_CHECKBOX( ID_USE_SSD, MainFrame::OnUseSSDClick )

    EVT_TEXT( ID_TEXT_SSD_CUTOFF, MainFrame::OnTextSSDCutoffTextUpdated )

    EVT_SPIN( ID_SPIN_SSD_CUTOFF, MainFrame::OnSpinSSDCutoffUpdated )

    EVT_TEXT( ID_TEXT_INTENSITY, MainFrame::OnTextIntensityUpdated )

    EVT_SPIN( ID_SPIN_INTENSITY, MainFrame::OnSpinIntensityUpdated )

    EVT_SPINCTRL( ID_SPIN_NUM_VOLUME_SLICES, MainFrame::OnSpinNumVolumeSlicesUpdated )
    EVT_TEXT( ID_SPIN_NUM_VOLUME_SLICES, MainFrame::OnSpinNumVolumeSlicesTextUpdated )

    EVT_TEXT( ID_TEXT_DENSITY, MainFrame::OnTextDensityUpdated )

    EVT_SPIN( ID_SPIN_DENSITY, MainFrame::OnSpinDensityUpdated )

    EVT_SLIDER( ID_SLIDER_X_ROTATE, MainFrame::OnSliderXRotateUpdated )

    EVT_SPINCTRL( ID_SPIN_X, MainFrame::OnSpinXUpdated )
    EVT_TEXT( ID_SPIN_X, MainFrame::OnSpinXTextUpdated )

    EVT_SLIDER( ID_SLIDER_Y_ROTATE, MainFrame::OnSliderYRotateUpdated )

    EVT_SPINCTRL( ID_SPIN_Y, MainFrame::OnSpinYUpdated )
    EVT_TEXT( ID_SPIN_Y, MainFrame::OnSpinYTextUpdated )

    EVT_SLIDER( ID_SLIDER_Z_ROTATE, MainFrame::OnSliderZRotateUpdated )

    EVT_SPINCTRL( ID_SPIN_Z, MainFrame::OnSpinZUpdated )
    EVT_TEXT( ID_SPIN_Z, MainFrame::OnSpinZTextUpdated )

    EVT_SLIDER( ID_SLIDER_ANGLE, MainFrame::OnSliderAngleUpdated )

    EVT_SPINCTRL( ID_SPIN_ANGLE, MainFrame::OnSpinAngleUpdated )
    EVT_TEXT( ID_SPIN_ANGLE, MainFrame::OnSpinAngleTextUpdated )

    EVT_BUTTON( ID_RESET_ROTATION, MainFrame::OnResetRotation )

    EVT_CHECKBOX( ID_COLORMAP_USE_WINDOW, MainFrame::OnColormapUseWindowClick )

    EVT_CHECKBOX( ID_COLORIZE, MainFrame::OnUseColorMapClick )

    EVT_COMBOBOX( ID_COLORMAP_COMBO, MainFrame::OnColormapComboSelected )

////@end MainFrame event table entries

END_EVENT_TABLE()


MainFrame::MainFrame( )
{
}

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	/* memory leak debugging... */
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
	_CrtSetDbgFlag(flag); // Set flag to the new value

	controlsAreReady = false;
    Create( parent, id, caption, pos, size, style );
	
	if (wxGetApp().argc > 1) {
		wxArrayString filepaths;
		filepaths.Add(wxGetApp().argv[1]);
		OpenFiles(filepaths);
	}
}


// --------------------------------------------------------
// ---------- Create --------------------------------------
// --------------------------------------------------------
bool MainFrame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin MainFrame member initialisation
    mainToolbar = NULL;
    panelCanvas = NULL;
    sizerCanvas = NULL;
    scrollSlice = NULL;
    imgCanvas = NULL;
    panelImage = NULL;
    chkBestFitZoom = NULL;
    sldZoom = NULL;
    spinZoom = NULL;
    radUsePlane0 = NULL;
    radUsePlane1 = NULL;
    radUsePlane2 = NULL;
    spinVolumeNum = NULL;
    panelMultislice = NULL;
    radByRows = NULL;
    radByCols = NULL;
    chkBestFitMultislice = NULL;
    spinNumRows = NULL;
    spinNumCols = NULL;
    panelVolumeRendering = NULL;
    radRenderRayTrace = NULL;
    radRender3DTexture = NULL;
    spinNumSteps = NULL;
    chkUseShading = NULL;
    txtSSDCutoff = NULL;
    spinSSDCutoff = NULL;
    txtIntensity = NULL;
    spinIntensity = NULL;
    spinNumVolSlices = NULL;
    txtDensity = NULL;
    spinDensity = NULL;
    panelRotations = NULL;
    sldXRotate = NULL;
    spinX = NULL;
    sldYRotate = NULL;
    spinY = NULL;
    sldZRotate = NULL;
    spinZ = NULL;
    sldAngleRotate = NULL;
    spinAngle = NULL;
    btnResetRotation = NULL;
    panelColorMap = NULL;
    chkUseWindow = NULL;
    chkColorize = NULL;
    cmbColorMap = NULL;
    statusbar = NULL;
////@end MainFrame member initialisation


////@begin MainFrame creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    this->SetBackgroundColour(wxColour(236, 233, 216));
    CreateControls();
    SetIcon(GetIconResource(wxT("../xpm/brain.xpm")));
    Centre();
////@end MainFrame creation
	this->Maximize(true);
    return true;
}


// --------------------------------------------------------
// ---------- CreateControls ------------------------------
// --------------------------------------------------------
void MainFrame::CreateControls()
{    
	WriteLog("CreateControls() called...");

	/* setup the OpenGL canvas */
	int glAttributes[6] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 32, 0, 0};
	
	/* setup the log window */
	frame = this;
	//logWindow = new LogWindow();
	mainVars = new MainFrameVariables();

////@begin MainFrame content construction
    MainFrame* itemFrame1 = this;

    GetAuiManager().SetManagedWindow(this);

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    {
        wxMenuItem* menuItem = new wxMenuItem(itemMenu3, ID_FILE_OPEN, _("&Open File(s)..."), wxEmptyString, wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("../xpm/openfile-16.xpm")));
        menuItem->SetBitmap(bitmap);
        itemMenu3->Append(menuItem);
    }
    {
        wxMenuItem* menuItem = new wxMenuItem(itemMenu3, ID_FILE_CLOSE, _("Close Current File(s)"), wxEmptyString, wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("../xpm/close16.xpm")));
        menuItem->SetBitmap(bitmap);
        itemMenu3->Append(menuItem);
    }
    itemMenu3->AppendSeparator();
    itemMenu3->Append(ID_FILE_BROWSER, _("DICOM Browser..."), wxEmptyString, wxITEM_NORMAL);
    itemMenu3->AppendSeparator();
    itemMenu3->Append(ID_FILE_SAVE_SCREENGRAB, _("Save screenshot..."), wxEmptyString, wxITEM_NORMAL);
    itemMenu3->AppendSeparator();
    itemMenu3->Append(ID_FILE_EXIT, _("E&xit"), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu3, _("&File"));
    wxMenu* itemMenu12 = new wxMenu;
    itemMenu12->Append(ID_EDIT_OPTIONS, _("&Options..."), wxEmptyString, wxITEM_NORMAL);
    itemMenu12->AppendSeparator();
    itemMenu12->Append(ID_OPTIONS_FUNCTION_EDITOR, _("Transfer Function Editor (Color-mapping)..."), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu12, _("&Edit"));
    wxMenu* itemMenu16 = new wxMenu;
    itemMenu16->Append(ID_VIEW_NORMAL, _("Normal"), wxEmptyString, wxITEM_NORMAL);
    itemMenu16->Append(ID_VIEW_ORTHOGONAL, _("Orthogonal"), wxEmptyString, wxITEM_NORMAL);
    itemMenu16->Append(ID_VIEW_MULTISLICE, _("Multislice"), wxEmptyString, wxITEM_NORMAL);
    itemMenu16->Append(ID_VIEW_MIP, _("Maximum Intensity Projection"), wxEmptyString, wxITEM_NORMAL);
    itemMenu16->Append(ID_VIEW_SSD, _("Surface Shaded Display"), wxEmptyString, wxITEM_NORMAL);
    itemMenu16->Append(ID_VIEW_VOL_REND, _("Volume Rendering"), wxEmptyString, wxITEM_NORMAL);
    itemMenu16->AppendSeparator();
    itemMenu16->Append(ID_VIEW_IMAGE_OVERLAY, _("Image Overlay"), wxEmptyString, wxITEM_CHECK);
    itemMenu16->Check(ID_VIEW_IMAGE_OVERLAY, true);
    menuBar->Append(itemMenu16, _("View"));
    wxMenu* itemMenu25 = new wxMenu;
    itemMenu25->Append(ID_IMAGE_INFORMATION, _("Information..."), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu25, _("&Image"));
    wxMenu* itemMenu27 = new wxMenu;
    itemMenu27->Append(ID_FILE_ANONYMIZE, _("Anonymize..."), wxEmptyString, wxITEM_NORMAL);
    itemMenu27->Append(ID_FILE_CONVERT, _("Convert Files..."), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu27, _("Tools"));
    wxMenu* itemMenu30 = new wxMenu;
    {
        wxMenuItem* menuItem = new wxMenuItem(itemMenu30, ID_HELP_HELP, _("&Help"), wxEmptyString, wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("../xpm/help.xpm")));
        menuItem->SetBitmap(bitmap);
        itemMenu30->Append(menuItem);
    }
    itemMenu30->AppendSeparator();
    itemMenu30->Append(ID_HELP_REPORTBUG, _("Report a Bug..."), wxEmptyString, wxITEM_NORMAL);
    itemMenu30->Append(ID_HELP_ABOUT, _("&About"), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu30, _("&Help"));
    itemFrame1->SetMenuBar(menuBar);

    mainToolbar = new wxToolBar( itemFrame1, ID_MAIN_TOOLBAR, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL|wxTB_NODIVIDER );
    mainToolbar->SetToolBitmapSize(wxSize(32, 32));
    wxBitmap itemtool36Bitmap(itemFrame1->GetBitmapResource(wxT("xpm/openfile.xpm")));
    wxBitmap itemtool36BitmapDisabled;
    mainToolbar->AddTool(ID_MAIN_TOOLBAR_OPEN_FILES, _("Open File(s)..."), itemtool36Bitmap, itemtool36BitmapDisabled, wxITEM_NORMAL, _("Open File(s)..."), wxEmptyString);
    wxBitmap itemtool37Bitmap(itemFrame1->GetBitmapResource(wxT("xpm/close32.xpm")));
    wxBitmap itemtool37BitmapDisabled;
    mainToolbar->AddTool(ID_MAIN_TOOLBAR_CLOSE_FILE, _("Close Current File(s)..."), itemtool37Bitmap, itemtool37BitmapDisabled, wxITEM_NORMAL, _("Close Current File(s)..."), wxEmptyString);
    mainToolbar->AddSeparator();
    wxBitmap itemtool39Bitmap(itemFrame1->GetBitmapResource(wxT("xpm/normalview.xpm")));
    wxBitmap itemtool39BitmapDisabled;
    mainToolbar->AddTool(ID_MAIN_TOOLBAR_VIEW_NORMAL, _("Normal View"), itemtool39Bitmap, itemtool39BitmapDisabled, wxITEM_RADIO, _("Normal View"), wxEmptyString);
    wxBitmap itemtool40Bitmap(itemFrame1->GetBitmapResource(wxT("xpm/brain3view.xpm")));
    wxBitmap itemtool40BitmapDisabled;
    mainToolbar->AddTool(ID_MAIN_TOOLBAR_VIEW_ORTHOGONAL, _("Orthogonal View"), itemtool40Bitmap, itemtool40BitmapDisabled, wxITEM_RADIO, _("Orthogonal View"), wxEmptyString);
    wxBitmap itemtool41Bitmap(itemFrame1->GetBitmapResource(wxT("xpm/multisliceview2.xpm")));
    wxBitmap itemtool41BitmapDisabled;
    mainToolbar->AddTool(ID_MAIN_TOOLBAR_VIEW_MULTISLICE, _("Multislice View"), itemtool41Bitmap, itemtool41BitmapDisabled, wxITEM_RADIO, _("Multislice View"), wxEmptyString);
    wxBitmap itemtool42Bitmap(itemFrame1->GetBitmapResource(wxT("xpm/mip.xpm")));
    wxBitmap itemtool42BitmapDisabled;
    mainToolbar->AddTool(ID_MAIN_TOOLBAR_VIEW_MIP, _("Maximum Intensity Projection"), itemtool42Bitmap, itemtool42BitmapDisabled, wxITEM_RADIO, _("Maximum Intensity Projection"), wxEmptyString);
    wxBitmap itemtool43Bitmap(itemFrame1->GetBitmapResource(wxT("xpm/ssd.xpm")));
    wxBitmap itemtool43BitmapDisabled;
    mainToolbar->AddTool(ID_MAIN_TOOLBAR_VIEW_SSD, _("Surface Shaded Display"), itemtool43Bitmap, itemtool43BitmapDisabled, wxITEM_RADIO, _("Surface Shaded Display"), wxEmptyString);
    mainToolbar->EnableTool(ID_MAIN_TOOLBAR_VIEW_SSD, false);
    wxBitmap itemtool44Bitmap(itemFrame1->GetBitmapResource(wxT("xpm/volume-rend.xpm")));
    wxBitmap itemtool44BitmapDisabled;
    mainToolbar->AddTool(ID_MAIN_TOOLBAR_VIEW_VOL_REND, _("Volume Rendering View"), itemtool44Bitmap, itemtool44BitmapDisabled, wxITEM_RADIO, _("Volume Rendering View"), wxEmptyString);
    mainToolbar->AddSeparator();
    wxBitmap itemtool46Bitmap(itemFrame1->GetBitmapResource(wxT("xpm/show-info-overlay.xpm")));
    wxBitmap itemtool46BitmapDisabled;
    mainToolbar->AddTool(ID_MAIN_TOOLBAR_SHOW_OVERLAY, _("Show/Hide Image Overlay"), itemtool46Bitmap, itemtool46BitmapDisabled, wxITEM_CHECK, _("Show/Hide Image Overlay"), wxEmptyString);
    wxBitmap itemtool47Bitmap(itemFrame1->GetBitmapResource(wxT("xpm/showlines.xpm")));
    wxBitmap itemtool47BitmapDisabled;
    mainToolbar->AddTool(ID_MAIN_TOOLBAR_SHOW_LINES, _("Show/Hide slice lines"), itemtool47Bitmap, itemtool47BitmapDisabled, wxITEM_CHECK, _("Show/Hide Slice Lines"), wxEmptyString);
    wxBitmap itemtool48Bitmap(itemFrame1->GetBitmapResource(wxT("xpm/hi-res.xpm")));
    wxBitmap itemtool48BitmapDisabled;
    mainToolbar->AddTool(ID_IMAGE_TOOLBAR_HIRES, _("View Hi/Lo Resolution"), itemtool48Bitmap, itemtool48BitmapDisabled, wxITEM_CHECK, _("View Hi/Lo Resolution"), wxEmptyString);
    mainToolbar->AddSeparator();
    wxBitmap itemtool50Bitmap(itemFrame1->GetBitmapResource(wxT("xpm/image-info.xpm")));
    wxBitmap itemtool50BitmapDisabled;
    mainToolbar->AddTool(ID_MAIN_TOOLBAR_INFO, _("Show Image Information"), itemtool50Bitmap, itemtool50BitmapDisabled, wxITEM_NORMAL, _("Show Image Information"), wxEmptyString);
    mainToolbar->Realize();
    itemFrame1->GetAuiManager().AddPane(mainToolbar, wxAuiPaneInfo()
        .ToolbarPane().Name(_T("Pane1")).Top().Layer(10).CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(false).Floatable(false).Movable(false));

    panelCanvas = new wxPanel( itemFrame1, ID_GLCANVAS_PANEL, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
    itemFrame1->GetAuiManager().AddPane(panelCanvas, wxAuiPaneInfo()
        .Name(_T("panelGLCanvas")).Caption(_("No File Loaded")).Centre().CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false));

    sizerCanvas = new wxBoxSizer(wxHORIZONTAL);
    panelCanvas->SetSizer(sizerCanvas);

    scrollSlice = new wxScrollBar( panelCanvas, ID_SCROLLBAR, wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL );
    scrollSlice->SetScrollbar(0, 1, 100, 1);
    sizerCanvas->Add(scrollSlice, 0, wxGROW|wxALL, 0);

    imgCanvas = new ImageCanvas( panelCanvas, ID_GLCANVAS, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER, wxT("GLCanvas"), glAttributes );
    sizerCanvas->Add(imgCanvas, 1, wxGROW|wxALL, 0);

    // Fit to content
    itemFrame1->GetAuiManager().GetPane(_T("panelGLCanvas")).BestSize(panelCanvas->GetSizer()->Fit(panelCanvas)).MinSize(panelCanvas->GetSizer()->GetMinSize());

    panelImage = new wxPanel( itemFrame1, ID_IMAGE_PANEL, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
    itemFrame1->GetAuiManager().AddPane(panelImage, wxAuiPaneInfo()
        .Name(_T("panelImage")).Caption(_("Image")).MinSize(wxSize(175, 50)).MaxSize(wxSize(-1, 50)).BestSize(wxSize(-1, 50)).CloseButton(false).DestroyOnClose(false).Resizable(true).FloatingSize(wxSize(175, 75)));

    wxBoxSizer* itemBoxSizer56 = new wxBoxSizer(wxVERTICAL);
    panelImage->SetSizer(itemBoxSizer56);

    wxBoxSizer* itemBoxSizer57 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer57, 0, wxGROW|wxALL, 0);

    wxStaticText* itemStaticText58 = new wxStaticText( panelImage, wxID_STATIC, _("Zoom %"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText58->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer57->Add(itemStaticText58, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    chkBestFitZoom = new wxCheckBox( panelImage, ID_BESTFIT_ZOOM, _("Best Fit"), wxDefaultPosition, wxDefaultSize, 0 );
    chkBestFitZoom->SetValue(true);
    itemBoxSizer57->Add(chkBestFitZoom, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer60 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer60, 0, wxGROW|wxALL, 0);

    sldZoom = new wxSlider( panelImage, ID_ZOOM_SLIDER, 100, 5, 1000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
    if (MainFrame::ShowToolTips())
        sldZoom->SetToolTip(_("Zoom (5-1000%)"));
    itemBoxSizer60->Add(sldZoom, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    spinZoom = new wxSpinCtrl( panelImage, ID_SPIN_ZOOM, _T("100"), wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS, 5, 1000, 100 );
    if (MainFrame::ShowToolTips())
        spinZoom->SetToolTip(_("Zoom (5-1000%)"));
    itemBoxSizer60->Add(spinZoom, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxBoxSizer* itemBoxSizer63 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer63, 0, wxALIGN_LEFT|wxALL, 2);

    wxBoxSizer* itemBoxSizer64 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer63->Add(itemBoxSizer64, 0, wxGROW|wxALL, 2);

    wxStaticText* itemStaticText65 = new wxStaticText( panelImage, wxID_STATIC, _("Viewing Plane"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText65->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer64->Add(itemStaticText65, 0, wxALIGN_LEFT|wxALL, 1);

    radUsePlane0 = new wxRadioButton( panelImage, ID_RADIO_USE_PLANE0, _("Plane 0"), wxDefaultPosition, wxDefaultSize, 0 );
    radUsePlane0->SetValue(true);
    itemBoxSizer64->Add(radUsePlane0, 0, wxALIGN_LEFT|wxALL, 1);

    radUsePlane1 = new wxRadioButton( panelImage, ID_RADIO_USE_PLANE1, _("Plane 1"), wxDefaultPosition, wxDefaultSize, 0 );
    radUsePlane1->SetValue(false);
    itemBoxSizer64->Add(radUsePlane1, 0, wxALIGN_LEFT|wxALL, 1);

    radUsePlane2 = new wxRadioButton( panelImage, ID_RADIO_USE_PLANE2, _("Plane 2"), wxDefaultPosition, wxDefaultSize, 0 );
    radUsePlane2->SetValue(false);
    itemBoxSizer64->Add(radUsePlane2, 0, wxALIGN_LEFT|wxALL, 1);

    wxBoxSizer* itemBoxSizer69 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer63->Add(itemBoxSizer69, 0, wxGROW|wxALL, 2);

    wxStaticText* itemStaticText70 = new wxStaticText( panelImage, wxID_STATIC, _("Volume"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText70->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer69->Add(itemStaticText70, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);

    spinVolumeNum = new wxSpinCtrl( panelImage, ID_VOLUME_NUM, _T("0"), wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 0, 1000, 0 );
    itemBoxSizer69->Add(spinVolumeNum, 0, wxALIGN_LEFT|wxALL, 1);

    panelMultislice = new wxPanel( itemFrame1, ID_MULTISLICE_PANEL, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
    itemFrame1->GetAuiManager().AddPane(panelMultislice, wxAuiPaneInfo()
        .Name(_T("panelMultislice")).Caption(_("Multislice")).MinSize(wxSize(-1, 100)).MaxSize(wxSize(-1, 100)).BestSize(wxSize(-1, 100)).Position(1).CloseButton(false).DestroyOnClose(false).Resizable(true));

    wxBoxSizer* itemBoxSizer73 = new wxBoxSizer(wxVERTICAL);
    panelMultislice->SetSizer(itemBoxSizer73);

    wxBoxSizer* itemBoxSizer74 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer73->Add(itemBoxSizer74, 0, wxGROW|wxALL, 0);

    wxStaticText* itemStaticText75 = new wxStaticText( panelMultislice, wxID_STATIC, _("Order images by"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer74->Add(itemStaticText75, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxBoxSizer* itemBoxSizer76 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer74->Add(itemBoxSizer76, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    radByRows = new wxRadioButton( panelMultislice, ID_ORDER_BY_ROWS, _("Rows"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
    radByRows->SetValue(true);
    itemBoxSizer76->Add(radByRows, 0, wxALIGN_LEFT|wxALL, 0);

    radByCols = new wxRadioButton( panelMultislice, ID_ORDER_BY_COLS, _("Columns"), wxDefaultPosition, wxDefaultSize, 0 );
    radByCols->SetValue(false);
    itemBoxSizer76->Add(radByCols, 0, wxALIGN_LEFT|wxALL, 0);

    itemBoxSizer74->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    chkBestFitMultislice = new wxCheckBox( panelMultislice, ID_CHECK_BESTFIT_MULTISLICE, _("Best Fit"), wxDefaultPosition, wxDefaultSize, 0 );
    chkBestFitMultislice->SetValue(false);
    itemBoxSizer74->Add(chkBestFitMultislice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    itemBoxSizer73->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

    wxBoxSizer* itemBoxSizer82 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer73->Add(itemBoxSizer82, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

    wxFlexGridSizer* itemFlexGridSizer83 = new wxFlexGridSizer(2, 3, 0, 0);
    itemBoxSizer82->Add(itemFlexGridSizer83, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    wxStaticText* itemStaticText84 = new wxStaticText( panelMultislice, wxID_STATIC, _("Rows"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer83->Add(itemStaticText84, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 0);

    wxStaticText* itemStaticText85 = new wxStaticText( panelMultislice, wxID_STATIC, _("x"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer83->Add(itemStaticText85, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 0);

    wxStaticText* itemStaticText86 = new wxStaticText( panelMultislice, wxID_STATIC, _("Columns"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer83->Add(itemStaticText86, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 0);

    spinNumRows = new wxSpinCtrl( panelMultislice, ID_NUM_ROWS, _T("1"), wxDefaultPosition, wxSize(45, -1), wxSP_ARROW_KEYS, 1, 100, 1 );
    itemFlexGridSizer83->Add(spinNumRows, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxStaticText* itemStaticText88 = new wxStaticText( panelMultislice, wxID_STATIC, _("x"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer83->Add(itemStaticText88, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 2);

    spinNumCols = new wxSpinCtrl( panelMultislice, ID_NUM_COLS, _T("1"), wxDefaultPosition, wxSize(45, -1), wxSP_ARROW_KEYS, 1, 100, 1 );
    itemFlexGridSizer83->Add(spinNumCols, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 0);

    panelVolumeRendering = new wxPanel( itemFrame1, ID_VOLUME_PANEL, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
    itemFrame1->GetAuiManager().AddPane(panelVolumeRendering, wxAuiPaneInfo()
        .Name(_T("panelVolumeRendering")).Caption(_("Volume Rendering")).MinSize(wxSize(-1, 200)).MaxSize(wxSize(-1, 200)).BestSize(wxSize(-1, 200)).CloseButton(false).DestroyOnClose(false).Resizable(true));

    wxBoxSizer* itemBoxSizer91 = new wxBoxSizer(wxVERTICAL);
    panelVolumeRendering->SetSizer(itemBoxSizer91);

    wxBoxSizer* itemBoxSizer92 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer91->Add(itemBoxSizer92, 0, wxGROW|wxALL, 1);

    wxStaticText* itemStaticText93 = new wxStaticText( panelVolumeRendering, wxID_STATIC, _("Rendering Method"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer92->Add(itemStaticText93, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer94 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer92->Add(itemBoxSizer94, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    radRenderRayTrace = new wxRadioButton( panelVolumeRendering, ID_RENDERMETHOD_RAY_TRACE, _("Ray Tracing"), wxDefaultPosition, wxDefaultSize, 0 );
    radRenderRayTrace->SetValue(true);
    itemBoxSizer94->Add(radRenderRayTrace, 0, wxALIGN_LEFT|wxALL, 1);

    radRender3DTexture = new wxRadioButton( panelVolumeRendering, ID_RENDERMETHOD_3D_TEXTURE, _("3D Texture Slicing"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
    radRender3DTexture->SetValue(false);
    itemBoxSizer94->Add(radRender3DTexture, 0, wxALIGN_LEFT|wxALL, 1);

    wxBoxSizer* itemBoxSizer97 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer91->Add(itemBoxSizer97, 0, wxGROW|wxALL, 1);

    wxStaticBox* itemStaticBoxSizer98Static = new wxStaticBox(panelVolumeRendering, wxID_ANY, _("Ray Casting"));
    wxStaticBoxSizer* itemStaticBoxSizer98 = new wxStaticBoxSizer(itemStaticBoxSizer98Static, wxVERTICAL);
    itemBoxSizer97->Add(itemStaticBoxSizer98, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer99 = new wxBoxSizer(wxVERTICAL);
    itemStaticBoxSizer98->Add(itemBoxSizer99, 0, wxGROW|wxALL, 1);

    wxBoxSizer* itemBoxSizer100 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer99->Add(itemBoxSizer100, 0, wxGROW|wxALL, 1);

    wxStaticText* itemStaticText101 = new wxStaticText( panelVolumeRendering, wxID_STATIC, _("# Steps"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer100->Add(itemStaticText101, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    spinNumSteps = new wxSpinCtrl( panelVolumeRendering, ID_NUM_STEPS, _T("80"), wxDefaultPosition, wxSize(50, -1), wxSP_ARROW_KEYS, 10, 500, 80 );
    itemBoxSizer100->Add(spinNumSteps, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    chkUseShading = new wxCheckBox( panelVolumeRendering, ID_USE_SSD, _("Surface shading"), wxDefaultPosition, wxDefaultSize, 0 );
    chkUseShading->SetValue(false);
    itemBoxSizer99->Add(chkUseShading, 0, wxALIGN_LEFT|wxALL, 2);

    wxBoxSizer* itemBoxSizer104 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer99->Add(itemBoxSizer104, 0, wxGROW|wxALL, 1);

    wxStaticText* itemStaticText105 = new wxStaticText( panelVolumeRendering, wxID_STATIC, _("SSD cutoff"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer104->Add(itemStaticText105, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtSSDCutoff = new wxTextCtrl( panelVolumeRendering, ID_TEXT_SSD_CUTOFF, _("0.000"), wxDefaultPosition, wxSize(40, -1), 0 );
    itemBoxSizer104->Add(txtSSDCutoff, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    spinSSDCutoff = new wxSpinButton( panelVolumeRendering, ID_SPIN_SSD_CUTOFF, wxDefaultPosition, wxSize(-1, 22), wxSP_VERTICAL );
    spinSSDCutoff->SetRange(0, 100);
    spinSSDCutoff->SetValue(0);
    itemBoxSizer104->Add(spinSSDCutoff, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxStaticBox* itemStaticBoxSizer108Static = new wxStaticBox(panelVolumeRendering, wxID_ANY, _("Texture Slicing"));
    wxStaticBoxSizer* itemStaticBoxSizer108 = new wxStaticBoxSizer(itemStaticBoxSizer108Static, wxVERTICAL);
    itemBoxSizer97->Add(itemStaticBoxSizer108, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer109 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer108->Add(itemBoxSizer109, 0, wxGROW|wxALL, 1);

    wxStaticText* itemStaticText110 = new wxStaticText( panelVolumeRendering, wxID_STATIC, _("Intensity"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer109->Add(itemStaticText110, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtIntensity = new wxTextCtrl( panelVolumeRendering, ID_TEXT_INTENSITY, _("0.5"), wxDefaultPosition, wxSize(40, -1), 0 );
    txtIntensity->SetMaxLength(4);
    itemBoxSizer109->Add(txtIntensity, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    spinIntensity = new wxSpinButton( panelVolumeRendering, ID_SPIN_INTENSITY, wxDefaultPosition, wxSize(-1, 22), wxSP_VERTICAL );
    spinIntensity->SetRange(0, 100);
    spinIntensity->SetValue(50);
    itemBoxSizer109->Add(spinIntensity, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer113 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer108->Add(itemBoxSizer113, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText114 = new wxStaticText( panelVolumeRendering, wxID_STATIC, _("# slices"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer113->Add(itemStaticText114, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    spinNumVolSlices = new wxSpinCtrl( panelVolumeRendering, ID_SPIN_NUM_VOLUME_SLICES, _T("50"), wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS, 0, 1000, 50 );
    itemBoxSizer113->Add(spinNumVolSlices, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    wxBoxSizer* itemBoxSizer116 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer108->Add(itemBoxSizer116, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText117 = new wxStaticText( panelVolumeRendering, wxID_STATIC, _("Density"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer116->Add(itemStaticText117, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    txtDensity = new wxTextCtrl( panelVolumeRendering, ID_TEXT_DENSITY, _("0.5"), wxDefaultPosition, wxSize(40, -1), 0 );
    txtDensity->SetMaxLength(4);
    itemBoxSizer116->Add(txtDensity, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    spinDensity = new wxSpinButton( panelVolumeRendering, ID_SPIN_DENSITY, wxDefaultPosition, wxSize(-1, 22), wxSP_VERTICAL );
    spinDensity->SetRange(0, 100);
    spinDensity->SetValue(50);
    itemBoxSizer116->Add(spinDensity, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    // Fit to content
    itemFrame1->GetAuiManager().GetPane(_T("panelVolumeRendering")).BestSize(panelVolumeRendering->GetSizer()->Fit(panelVolumeRendering)).MinSize(panelVolumeRendering->GetSizer()->GetMinSize());

    panelRotations = new wxPanel( itemFrame1, ID_ROTATIONS_PANEL, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
    itemFrame1->GetAuiManager().AddPane(panelRotations, wxAuiPaneInfo()
        .Name(_T("panelRotations")).Caption(_("Rotations")).MinSize(wxSize(-1, 250)).MaxSize(wxSize(-1, 250)).BestSize(wxSize(-1, 250)).Position(3).CloseButton(false).DestroyOnClose(false).Resizable(true));

    wxBoxSizer* itemBoxSizer121 = new wxBoxSizer(wxHORIZONTAL);
    panelRotations->SetSizer(itemBoxSizer121);

    wxBoxSizer* itemBoxSizer122 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer121->Add(itemBoxSizer122, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer123 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer122->Add(itemBoxSizer123, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer124 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer123->Add(itemBoxSizer124, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

    wxStaticText* itemStaticText125 = new wxStaticText( panelRotations, wxID_STATIC, _("X"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer123->Add(itemStaticText125, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 2);

    sldXRotate = new wxSlider( panelRotations, ID_SLIDER_X_ROTATE, 0, 0, 360, wxDefaultPosition, wxSize(20, 50), wxSL_VERTICAL );
    itemBoxSizer123->Add(sldXRotate, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

    spinX = new wxSpinCtrl( panelRotations, ID_SPIN_X, _T("0"), wxDefaultPosition, wxSize(40, 15), wxSP_ARROW_KEYS, -1, 360, 0 );
    spinX->SetFont(wxFont(7, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Tahoma")));
    itemBoxSizer123->Add(spinX, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer128 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer122->Add(itemBoxSizer128, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer129 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer128->Add(itemBoxSizer129, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

    wxStaticText* itemStaticText130 = new wxStaticText( panelRotations, wxID_STATIC, _("Y"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer128->Add(itemStaticText130, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 2);

    sldYRotate = new wxSlider( panelRotations, ID_SLIDER_Y_ROTATE, 0, 0, 360, wxDefaultPosition, wxSize(20, 50), wxSL_VERTICAL );
    itemBoxSizer128->Add(sldYRotate, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

    spinY = new wxSpinCtrl( panelRotations, ID_SPIN_Y, _T("0"), wxDefaultPosition, wxSize(40, 15), wxSP_ARROW_KEYS, -1, 360, 0 );
    spinY->SetFont(wxFont(7, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Tahoma")));
    itemBoxSizer128->Add(spinY, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer133 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer122->Add(itemBoxSizer133, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer134 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer133->Add(itemBoxSizer134, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

    wxStaticText* itemStaticText135 = new wxStaticText( panelRotations, wxID_STATIC, _("Z"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer133->Add(itemStaticText135, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 2);

    sldZRotate = new wxSlider( panelRotations, ID_SLIDER_Z_ROTATE, 0, 0, 360, wxDefaultPosition, wxSize(20, 50), wxSL_VERTICAL );
    itemBoxSizer133->Add(sldZRotate, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

    spinZ = new wxSpinCtrl( panelRotations, ID_SPIN_Z, _T("0"), wxDefaultPosition, wxSize(40, 15), wxSP_ARROW_KEYS, -1, 360, 0 );
    spinZ->SetFont(wxFont(7, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Tahoma")));
    itemBoxSizer133->Add(spinZ, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer138 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer122->Add(itemBoxSizer138, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer139 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer138->Add(itemBoxSizer139, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

    wxStaticText* itemStaticText140 = new wxStaticText( panelRotations, wxID_STATIC, _("angle"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer138->Add(itemStaticText140, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 2);

    sldAngleRotate = new wxSlider( panelRotations, ID_SLIDER_ANGLE, 0, 0, 360, wxDefaultPosition, wxSize(20, 50), wxSL_VERTICAL );
    itemBoxSizer138->Add(sldAngleRotate, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

    spinAngle = new wxSpinCtrl( panelRotations, ID_SPIN_ANGLE, _T("0"), wxDefaultPosition, wxSize(40, 15), wxSP_ARROW_KEYS, -1, 360, 0 );
    spinAngle->SetFont(wxFont(7, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Tahoma")));
    itemBoxSizer138->Add(spinAngle, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

    btnResetRotation = new wxButton( panelRotations, ID_RESET_ROTATION, _("Reset"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    if (MainFrame::ShowToolTips())
        btnResetRotation->SetToolTip(_("Reset all rotations to zero"));
    itemBoxSizer121->Add(btnResetRotation, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    // Fit to content
    itemFrame1->GetAuiManager().GetPane(_T("panelRotations")).BestSize(panelRotations->GetSizer()->Fit(panelRotations)).MinSize(panelRotations->GetSizer()->GetMinSize());

    panelColorMap = new wxPanel( itemFrame1, ID_COLOR_MAP_PANEL, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
    itemFrame1->GetAuiManager().AddPane(panelColorMap, wxAuiPaneInfo()
        .Name(_T("panelColorMap")).Caption(_("Color Map")).MinSize(wxSize(-1, 75)).MaxSize(wxSize(-1, 75)).BestSize(wxSize(-1, 75)).CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false));

    wxBoxSizer* itemBoxSizer145 = new wxBoxSizer(wxVERTICAL);
    panelColorMap->SetSizer(itemBoxSizer145);

    chkUseWindow = new wxCheckBox( panelColorMap, ID_COLORMAP_USE_WINDOW, _("Use window levels"), wxDefaultPosition, wxDefaultSize, 0 );
    chkUseWindow->SetValue(true);
    itemBoxSizer145->Add(chkUseWindow, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticLine* itemStaticLine147 = new wxStaticLine( panelColorMap, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer145->Add(itemStaticLine147, 0, wxGROW|wxALL, 0);

    chkColorize = new wxCheckBox( panelColorMap, ID_COLORIZE, _("Use Colormap"), wxDefaultPosition, wxDefaultSize, 0 );
    chkColorize->SetValue(false);
    itemBoxSizer145->Add(chkColorize, 0, wxALIGN_LEFT|wxALL, 5);

    wxBoxSizer* itemBoxSizer149 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer145->Add(itemBoxSizer149, 0, wxGROW|wxALL, 0);

    itemBoxSizer149->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString cmbColorMapStrings;
    cmbColorMap = new wxBitmapComboBox( panelColorMap, ID_COLORMAP_COMBO, wxEmptyString, wxDefaultPosition, wxDefaultSize, cmbColorMapStrings, wxCB_READONLY );
    itemBoxSizer149->Add(cmbColorMap, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    // Fit to content
    itemFrame1->GetAuiManager().GetPane(_T("panelColorMap")).BestSize(panelColorMap->GetSizer()->Fit(panelColorMap)).MinSize(panelColorMap->GetSizer()->GetMinSize());

    statusbar = new wxStatusBar( itemFrame1, ID_STATUSBAR, wxST_SIZEGRIP );
    statusbar->SetFieldsCount(1);
    statusbar->SetStatusText(_("Ready"), 0);
    itemFrame1->SetStatusBar(statusbar);

    GetAuiManager().Update();

////@end MainFrame content construction

	itemFrame1->imgCanvasIsReady = true;
	imgCanvas->SetObjectPointers(frame,mainVars);

	/* set the default toolbar button selection */
	frame->mainToolbar->ToggleTool(ID_MAIN_TOOLBAR_SHOW_OVERLAY, true);
	frame->mainToolbar->ToggleTool(ID_MAIN_TOOLBAR_SHOW_LINES, true);
	frame->mainToolbar->ToggleTool(ID_IMAGE_TOOLBAR_HIRES, true);

	frame->GetAuiManager().GetPane("panelVolumeRendering").Hide();
	frame->GetAuiManager().GetPane("panelMultislice").Hide();
	frame->GetAuiManager().Update();

	/* hide the slice scroll bar by default */
	frame->scrollSlice->Hide();
	frame->sizerCanvas->Layout();

	/* get the maximum texture size for the colormap LUT */
	int texSize = imgCanvas->GetMaxTextureSize();
	colorMaps = new ColorMapList(texSize);
	displayPoints = new DisplayPoints();

	/* setup the color mapping combo box */
	PopulateColorMapBox();

	/* now the controls have been created... */
	controlsAreReady = true;
}


// --------------------------------------------------------
// ---------- ShowToolTips --------------------------------
// --------------------------------------------------------
bool MainFrame::ShowToolTips()
{
    return true;
}


// --------------------------------------------------------
// ---------- GetBitmapResource ---------------------------
// --------------------------------------------------------
wxBitmap MainFrame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin MainFrame bitmap retrieval
    wxUnusedVar(name);
    if (name == _T("xpm/openfile-16.xpm"))
    {
        wxBitmap bitmap(openfile_16_xpm);
        return bitmap;
    }
    else if (name == _T("xpm/close16.xpm"))
    {
        wxBitmap bitmap(close16_xpm);
        return bitmap;
    }
    else if (name == _T("xpm/help.xpm"))
    {
        wxBitmap bitmap(help_xpm);
        return bitmap;
    }
    else if (name == _T("xpm/openfile.xpm"))
    {
        wxBitmap bitmap(openfile_xpm);
        return bitmap;
    }
    else if (name == _T("xpm/close32.xpm"))
    {
        wxBitmap bitmap(close32_xpm);
        return bitmap;
    }
    else if (name == _T("xpm/normalview.xpm"))
    {
        wxBitmap bitmap(normalview_xpm);
        return bitmap;
    }
    else if (name == _T("xpm/brain3view.xpm"))
    {
        wxBitmap bitmap(brain3view_xpm);
        return bitmap;
    }
    else if (name == _T("xpm/multisliceview2.xpm"))
    {
        wxBitmap bitmap(multisliceview2_xpm);
        return bitmap;
    }
    else if (name == _T("xpm/mip.xpm"))
    {
        wxBitmap bitmap(mip_xpm);
        return bitmap;
    }
    else if (name == _T("xpm/ssd.xpm"))
    {
        wxBitmap bitmap(ssd_xpm);
        return bitmap;
    }
    else if (name == _T("xpm/volume-rend.xpm"))
    {
        wxBitmap bitmap(volume_rend_xpm);
        return bitmap;
    }
    else if (name == _T("xpm/show-info-overlay.xpm"))
    {
        wxBitmap bitmap(show_info_overlay_xpm);
        return bitmap;
    }
    else if (name == _T("xpm/showlines.xpm"))
    {
        wxBitmap bitmap(showlines_xpm);
        return bitmap;
    }
    else if (name == _T("xpm/hi-res.xpm"))
    {
        wxBitmap bitmap(hi_res_xpm);
        return bitmap;
    }
    else if (name == _T("xpm/image-info.xpm"))
    {
        wxBitmap bitmap(image_info_xpm);
        return bitmap;
    }
    return wxNullBitmap;
////@end MainFrame bitmap retrieval
}


// --------------------------------------------------------
// ---------- GetIconResources ----------------------------
// --------------------------------------------------------
wxIcon MainFrame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin MainFrame icon retrieval
    wxUnusedVar(name);
    if (name == _T("xpm/brain.xpm"))
    {
        wxIcon icon(brain_xpm);
        return icon;
    }
    return wxNullIcon;
////@end MainFrame icon retrieval
}


// --------------------------------------------------------
// ---------- SetZoom -------------------------------------
// --------------------------------------------------------
/* sets the zoom on the GUI */
void MainFrame::SetZoom(int zoom)
{
	frame->sldZoom->SetValue(zoom);
	frame->spinZoom->SetValue(zoom);
	mainVars->Image_zoomPercent(spinZoom->GetValue());
}


// --------------------------------------------------------
// ---------- SetCurrentRotation --------------------------
// --------------------------------------------------------
void MainFrame::SetCurrentRotation(float angle, float x, float y, float z)
{
	frame->spinAngle->SetValue(int(360.0*angle));
	frame->spinX->SetValue(int(360.0*x));
	frame->spinY->SetValue(int(360.0*y));
	frame->spinZ->SetValue(int(360.0*z));
	frame->sldAngleRotate->SetValue(int(360.0*angle));
	frame->sldXRotate->SetValue(int(360.0*x));
	frame->sldYRotate->SetValue(int(360.0*y));
	frame->sldZRotate->SetValue(int(360.0*z));
}


// --------------------------------------------------------
// ---------- SetCurrentSlice -----------------------------
// --------------------------------------------------------
/* sets the slice scroll bar to the correct position */
void MainFrame::SetCurrentSlice(int startSlice, int endSlice, int slice)
{
	int w,h;
	frame->GetClientSize(&w, &h);

	if ((frame->imgCanvas->IsCurrentVolumeMultiFrame()) && (frame->scrollSlice != NULL)) {
		frame->scrollSlice->SetScrollbar(slice, 1, endSlice+1, 1, true);
		frame->scrollSlice->SetThumbPosition(slice);
	}
}


// --------------------------------------------------------
// ---------- IsImgCanvasReady ----------------------------
// --------------------------------------------------------
bool MainFrame::IsImgCanvasReady()
{
	if (imgCanvas != NULL)
		if (imgCanvas->IsReady())
			return true;
		else
			return false;
	else
		return imgCanvasIsReady;
}


// --------------------------------------------------------
// ---------- HideSliceScroll -----------------------------
// --------------------------------------------------------
void MainFrame::HideSliceScroll(bool hide)
{
	if (hide)
		frame->scrollSlice->Hide(); /* hide it */
	else
		frame->scrollSlice->Show(); /* show it */

	frame->sizerCanvas->Layout();
}


// --------------------------------------------------------
// ---------- SetRotationTo -------------------------------
// --------------------------------------------------------
void MainFrame::SetRotationTo(int value)
{
	mainVars->Rotations_X(value); mainVars->Rotations_Y(value); mainVars->Rotations_Z(value);
	spinX->SetValue(value); spinY->SetValue(value); spinZ->SetValue(value);
	sldXRotate->SetValue(value); sldYRotate->SetValue(value); sldZRotate->SetValue(value);
}



/* ================================================================== */
/* ---------- EVENT HANDLERS ---------------------------------------- */
/* ================================================================== */

// --------------------------------------------------------
// ---------- OnExit --------------------------------------

void MainFrame::OnFileExit( wxCommandEvent& WXUNUSED(event) )
{
	/* call the imageCanvas CloseFile function */
	if (IsImgCanvasReady())
		imgCanvas->CloseCurrentFile();

	delete colorMaps;
	delete displayPoints;
	
	delete statusbar;
	delete chkColorize;
	delete cmbColorMap;
	delete panelRotations;
	delete spinDensity;

	delete mainVars;
	//delete logWindow;
	delete imgCanvas;

    Close(true); /* true is to force the frame to close */

	exit(0);
}


// --------------------------------------------------------
// ---------- OnFileOpen ----------------------------------
void MainFrame::OnFileOpen(wxCommandEvent &event)
{
	int i;
	wxArrayString filenames;

	/* setup and display the file open dialog */
	FileOpenDialog *openFileDialog = new FileOpenDialog(this, wxID_ANY, "Open files...", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
	if (openFileDialog->ShowModal() == wxID_OK) {
		/* check to see which, and how many files should be loaded */
		filenames = openFileDialog->GetFilePaths();
		for (i=0;i<(int)filenames.Count();i++)
			filenames[i].Replace("\\","/");

		/* setup the variables to used by the loading routines */
		mainVars->Load_filenames(filenames);
		mainVars->Load_filetype(openFileDialog->GetFileType());
		mainVars->Load_isMosaic(openFileDialog->GetIsMosaic());
		mainVars->Load_loadType(openFileDialog->GetLoadType());
		mainVars->Load_mosaicXSize(openFileDialog->GetMosaicXSize());
		mainVars->Load_mosaicYSize(openFileDialog->GetMosaicYSize());
		mainVars->Load_mosaicNumSlices(openFileDialog->GetMosaicNumSlices());

		/* try to load the file(s) */
		if (!imgCanvas->LoadFile()) {
			WriteLog("File not loaded...");
			this->GetAuiManager().GetPane("panelGLCanvas").Caption("File not loaded");
			this->SendSizeEvent();
		}
		else {
			WriteLog("File loaded...");
			this->GetAuiManager().GetPane("panelGLCanvas").Caption(filenames[0]);
			this->SendSizeEvent();
		}
	}

	delete openFileDialog;
}

// --------------------------------------------------------
// ---------- OnFileClose ---------------------------------
/* closes the current image/volume */
void MainFrame::OnFileClose(wxCommandEvent &event)
{
	/* call the imageCanvas CloseFile function */
	if (IsImgCanvasReady())
		imgCanvas->CloseCurrentFile();

	/* reset the AUI window caption */
	frame->GetAuiManager().GetPane("PanelGLCanvas").Caption("No File Loaded");
	frame->scrollSlice->Hide(); /* hide it */
	frame->SendSizeEvent();
}


// --------------------------------------------------------
// ---------- OnViewOrthogonal ----------------------------
void MainFrame::OnViewOrthogonal(wxCommandEvent &event)
{
//	imgCanvas->SetDrawingType(IMGVIEW_3VIEW);
	mainVars->Toolbar_displayType(IMGVIEW_3VIEW);

	SetRotationTo(45);

	frame->GetAuiManager().GetPane("panelVolumeRendering").Hide();
	frame->GetAuiManager().GetPane("panelMultislice").Hide();
	frame->GetAuiManager().Update();

	/* set the menu item, and the main toolbar item */
	frame->mainToolbar->ToggleTool(ID_MAIN_TOOLBAR_VIEW_ORTHOGONAL, true);

	frame->scrollSlice->Hide(); /* hide it */

	imgCanvas->Draw();
}

// --------------------------------------------------------
// ---------- OnViewNormal --------------------------------
void MainFrame::OnViewNormal(wxCommandEvent &event)
{
//	imgCanvas->SetDrawingType(IMGVIEW_SINGLEPLANE);
	mainVars->Toolbar_displayType(IMGVIEW_SINGLEPLANE);

	SetRotationTo(0);

	frame->GetAuiManager().GetPane("panelVolumeRendering").Hide();
	frame->GetAuiManager().GetPane("panelMultislice").Hide();
	frame->GetAuiManager().Update();

	/* set the menu item, and the main toolbar item */
	frame->mainToolbar->ToggleTool(ID_MAIN_TOOLBAR_VIEW_NORMAL, true);

	imgCanvas->Draw();
}

// --------------------------------------------------------
// ---------- OnViewVolumeRendering -----------------------
void MainFrame::OnViewVolumeRendering(wxCommandEvent &event)
{
//	imgCanvas->SetDrawingType(IMGVIEW_DVR);
	mainVars->Toolbar_displayType(IMGVIEW_DVR);

	SetRotationTo(45);

	frame->GetAuiManager().GetPane("panelVolumeRendering").Show();
	frame->GetAuiManager().GetPane("panelMultislice").Hide();
	frame->GetAuiManager().Update();

	/* set the menu item, and the main toolbar item */
	frame->mainToolbar->ToggleTool(ID_MAIN_TOOLBAR_VIEW_VOL_REND, true);

	frame->scrollSlice->Hide(); /* hide it */

	imgCanvas->Draw();
}

// --------------------------------------------------------
// ---------- OnViewMultislice ----------------------------
void MainFrame::OnViewMultislice(wxCommandEvent &event)
{
//	imgCanvas->SetDrawingType(IMGVIEW_MULTISLICE);
	mainVars->Toolbar_displayType(IMGVIEW_MULTISLICE);

	SetRotationTo(0);

	frame->GetAuiManager().GetPane("panelVolumeRendering").Hide();
	frame->GetAuiManager().GetPane("panelMultislice").Show();
	frame->GetAuiManager().Update();

	/* set the menu item, and the main toolbar item */
	frame->mainToolbar->ToggleTool(ID_MAIN_TOOLBAR_VIEW_MULTISLICE, true);

	frame->scrollSlice->Hide(); /* hide it */

	imgCanvas->Draw();
}

// --------------------------------------------------------
// ---------- OnViewMIP -----------------------------------
void MainFrame::OnViewMIP(wxCommandEvent &event)
{
//	imgCanvas->SetDrawingType(IMGVIEW_MIP);
	mainVars->Toolbar_displayType(IMGVIEW_MIP);

	SetRotationTo(45);

	/* set the menu item, and the main toolbar item */
	frame->mainToolbar->ToggleTool(ID_MAIN_TOOLBAR_VIEW_MIP, true);

	frame->scrollSlice->Hide(); /* hide it */
	frame->sizerCanvas->Layout();

	imgCanvas->Draw();
}

// --------------------------------------------------------
// ---------- OnViewSSD -----------------------------------
void MainFrame::OnViewSSD(wxCommandEvent &event)
{
//	imgCanvas->SetDrawingType(IMGVIEW_SSD);
	mainVars->Toolbar_displayType(IMGVIEW_SSD);

	SetRotationTo(45);

	/* set the menu item, and the main toolbar item */
	frame->mainToolbar->ToggleTool(ID_MAIN_TOOLBAR_VIEW_SSD, true);

	frame->scrollSlice->Hide(); /* hide it */
	frame->sizerCanvas->Layout();

	imgCanvas->Draw();
}


// --------------------------------------------------------
// ---------- OnEditOptions -------------------------------
void MainFrame::OnEditOptions(wxCommandEvent &event)
{
//	frame->optDialog = new OptionsDialog(frame, wxID_ANY, "Medical Image Viewer Options", wxDefaultPosition, wxDefaultSize, 0, "optionsdialog");
//	frame->optDialog->ShowModal();
//	frame->optDialog->Close();
}


// --------------------------------------------------------
// ---------- OnHelpHelp ----------------------------------
void MainFrame::OnHelpHelp(wxCommandEvent &event)
{
}

// --------------------------------------------------------
// ---------- OnKeyDown -----------------------------------
// --------------------------------------------------------
void MainFrame::OnKeyDown( wxKeyEvent& event )
{
    long evkey = event.GetKeyCode();
    if (evkey == 0) return;

	switch (evkey) {
	case WXK_LEFT:
		if (mainVars->Toolbar_displayType() == IMGVIEW_DVR) {
		}
	case WXK_RIGHT:
		if (mainVars->Toolbar_displayType() == IMGVIEW_DVR) {
		}
	case WXK_UP:
		if (mainVars->Toolbar_displayType() == IMGVIEW_DVR) {
		}
		else {
			imgCanvas->Draw();
		}
		break;
	case WXK_DOWN:
		if (mainVars->Toolbar_displayType() == IMGVIEW_DVR) {
		}
		else {
			imgCanvas->Draw();
		}
		break;
	case WXK_NUMPAD_ADD:
		imgCanvas->Draw();
		break;
	case WXK_NUMPAD_SUBTRACT:
		imgCanvas->Draw();
		break;
	}

    event.Skip();
}


// --------------------------------------------------------
// ---------- OnKeyUp -------------------------------------
// --------------------------------------------------------
void MainFrame::OnKeyUp( wxKeyEvent& event )
{
    event.Skip();
}


// --------------------------------------------------------
// ---------- OnSpinZoomUpdated ---------------------------
// --------------------------------------------------------
void MainFrame::OnSpinZoomUpdated(wxSpinEvent &event)
{
	frame->sldZoom->SetValue(spinZoom->GetValue());
	mainVars->Image_zoomPercent(spinZoom->GetValue());
	imgCanvas->Draw();
}


// --------------------------------------------------------
// ---------- OnSpinZoomTextUpdated -----------------------
// --------------------------------------------------------
void MainFrame::OnSpinZoomTextUpdated(wxCommandEvent &event)
{
	frame->sldZoom->SetValue(spinZoom->GetValue());
	mainVars->Image_zoomPercent(spinZoom->GetValue());
	imgCanvas->Draw();
}


// --------------------------------------------------------
// ---------- OnScrollSliceScroll -------------------------
// --------------------------------------------------------
void MainFrame::OnScrollSliceScroll(wxScrollEvent &event)
{
	mainVars->Slice_value(scrollSlice->GetThumbPosition());
	imgCanvas->Draw();
}

// --------------------------------------------------------
// ---------- OnSize --------------------------------------
void MainFrame::OnSize(wxSizeEvent& event)
{
    event.Skip();
}

// --------------------------------------------------------
// ---------- OnViewHiRes ---------------------------------
void MainFrame::OnViewHiRes(wxCommandEvent &event)
{
	if (frame->mainToolbar->GetToolState(ID_IMAGE_TOOLBAR_HIRES)) {
		mainVars->Toolbar_showHighRes(true);
	}
	else {
		mainVars->Toolbar_showHighRes(false);
	}

	imgCanvas->Draw();
}

// --------------------------------------------------------
// ---------- OnShowOverlay -------------------------------
void MainFrame::OnShowOverlay(wxCommandEvent &event)
{
	if (frame->mainToolbar->GetToolState(ID_MAIN_TOOLBAR_SHOW_OVERLAY)) {
		mainVars->Toolbar_showTextOverlay(true);
	}
	else {
		mainVars->Toolbar_showTextOverlay(false);
	}

	imgCanvas->Draw();
}

// --------------------------------------------------------
// ---------- OnShowImageInformation ----------------------
void MainFrame::OnShowImageInformation(wxCommandEvent &event)
{
	imgCanvas->ShowFileInfo();
}

// --------------------------------------------------------
// ---------- OnHelpAbout ---------------------------------
// --------------------------------------------------------
void MainFrame::OnHelpAbout(wxCommandEvent &event)
{
	AboutDialog *aboutDialog = new AboutDialog(this, wxID_ANY, "About MIView...", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
	aboutDialog->ShowModal();
}

// --------------------------------------------------------
// ---------- OnAnonymize ---------------------------------
void MainFrame::OnAnonymize( wxCommandEvent& event )
{
	AnonymizeDialog *anonDialog = new AnonymizeDialog(this, wxID_ANY, "Anonymize Files...", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
	anonDialog->ShowModal();
}


// --------------------------------------------------------
// ---------- OnSpinXTextUpdated --------------------------
// --------------------------------------------------------
void MainFrame::OnSpinXTextUpdated( wxCommandEvent& event )
{
	if (frame->spinX->GetValue() >= 360)
		spinX->SetValue(0);
	mainVars->Rotations_X(frame->spinX->GetValue());
	imgCanvas->Draw();
}


// --------------------------------------------------------
// ---------- OnSpinYTextUpdated --------------------------
// --------------------------------------------------------
void MainFrame::OnSpinYTextUpdated( wxCommandEvent& event )
{
	if (frame->spinY->GetValue() >= 360)
		spinY->SetValue(0);
	mainVars->Rotations_Y(frame->spinY->GetValue());
	imgCanvas->Draw();
}


// --------------------------------------------------------
// ---------- OnSpinZTextUpdated --------------------------
// --------------------------------------------------------
void MainFrame::OnSpinZTextUpdated( wxCommandEvent& event )
{
	if (frame->spinZ->GetValue() >= 360)
		spinZ->SetValue(0);
	mainVars->Rotations_Z(frame->spinZ->GetValue());
	imgCanvas->Draw();
}


// --------------------------------------------------------
// ---------- OnZoomSliderUpdated -------------------------
// --------------------------------------------------------
void MainFrame::OnZoomSliderUpdated( wxCommandEvent& event )
{
	mainVars->Image_zoomPercent(frame->sldZoom->GetValue());
	imgCanvas->Draw();
	frame->spinZoom->SetValue(frame->sldZoom->GetValue());
}


// --------------------------------------------------------
// ---------- OnMouseWheel --------------------------------
// --------------------------------------------------------
void MainFrame::OnMouseWheel( wxMouseEvent& event )
{
////@begin wxEVT_MOUSEWHEEL event handler for ID_MAINFRAME in MainFrame.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_MOUSEWHEEL event handler for ID_MAINFRAME in MainFrame. 
}


// --------------------------------------------------------
// ---------- OnResetRotation -----------------------------
// --------------------------------------------------------
void MainFrame::OnResetRotation( wxCommandEvent& event )
{
	SetRotationTo(0);
}


// --------------------------------------------------------
// ---------- OnSliderXRotateUpdated ----------------------
// --------------------------------------------------------
void MainFrame::OnSliderXRotateUpdated( wxCommandEvent& event )
{
	frame->spinX->SetValue(frame->sldXRotate->GetValue());
}


// --------------------------------------------------------
// ---------- OnSliderYRotateUpdated ----------------------
// --------------------------------------------------------
void MainFrame::OnSliderYRotateUpdated( wxCommandEvent& event )
{
	frame->spinY->SetValue(frame->sldYRotate->GetValue());
}


// --------------------------------------------------------
// ---------- OnSliderZRotateUpdated ----------------------
// --------------------------------------------------------
void MainFrame::OnSliderZRotateUpdated( wxCommandEvent& event )
{
	frame->spinZ->SetValue(frame->sldZRotate->GetValue());
}


// --------------------------------------------------------
// ---------- OnSpinXUpdated ------------------------------
// --------------------------------------------------------
void MainFrame::OnSpinXUpdated( wxSpinEvent& event )
{
	if (frame->spinX->GetValue() >= 360)
		spinX->SetValue(0);
	if (frame->spinX->GetValue() <= -1)
		spinX->SetValue(359);
	mainVars->Rotations_X(frame->spinX->GetValue());
	imgCanvas->Draw();
}


// --------------------------------------------------------
// ---------- OnSpinYUpdated ------------------------------
// --------------------------------------------------------
void MainFrame::OnSpinYUpdated( wxSpinEvent& event )
{
	if (frame->spinY->GetValue() >= 360)
		spinY->SetValue(0);
	if (frame->spinY->GetValue() <= -1)
		spinY->SetValue(359);
	mainVars->Rotations_Y(frame->spinY->GetValue());
	imgCanvas->Draw();
}


// --------------------------------------------------------
// ---------- OnSpinZUpdated ------------------------------
// --------------------------------------------------------
void MainFrame::OnSpinZUpdated( wxSpinEvent& event )
{
	if (frame->spinZ->GetValue() >= 360)
		spinZ->SetValue(0);
	if (frame->spinZ->GetValue() <= -1)
		spinZ->SetValue(359);
	mainVars->Rotations_Z(frame->spinZ->GetValue());
	imgCanvas->Draw();
}


// --------------------------------------------------------
// ---------- OnPaint -------------------------------------
// --------------------------------------------------------
void MainFrame::OnPaint( wxPaintEvent& event )
{
////@begin wxEVT_PAINT event handler for ID_MAINFRAME in MainFrame.
    // Before editing this code, remove the block markers.
    wxPaintDC dc(this);
////@end wxEVT_PAINT event handler for ID_MAINFRAME in MainFrame. 
}


// --------------------------------------------------------
// ---------- OnMainToolbarShowLinesClick -----------------
// --------------------------------------------------------
void MainFrame::OnMainToolbarShowLinesClick( wxCommandEvent& event )
{
	if (frame->mainToolbar->GetToolState(ID_MAIN_TOOLBAR_SHOW_LINES)) {
		mainVars->Toolbar_showLineOverlay(true);
	}
	else {
		mainVars->Toolbar_showLineOverlay(false);
	}

	imgCanvas->Draw();
}


// --------------------------------------------------------
// ---------- OnBestFitZoomClick --------------------------
// --------------------------------------------------------
void MainFrame::OnBestfitZoomClick( wxCommandEvent& event )
{
	if (IsImgCanvasReady()) {
		mainVars->Image_zoomUseBestFit(chkBestFitZoom->GetValue());
		imgCanvas->Draw();
	}
}


// --------------------------------------------------------
// ---------- OnUseColorMapClick --------------------------
// --------------------------------------------------------
void MainFrame::OnUseColorMapClick( wxCommandEvent& event )
{
	mainVars->ColorMap_useColor(chkColorize->GetValue());

	if (!chkColorize->GetValue()) {
		cmbColorMap->SetValue("Gray");
		mainVars->ColorMap_index(0);
	}
	imgCanvas->Draw();
}


// --------------------------------------------------------
// ---------- OnFileConvertClick --------------------------
// --------------------------------------------------------
void MainFrame::OnFileConvertClick( wxCommandEvent& event )
{
	ConvertDialog *convertDialog = new ConvertDialog(NULL);
	convertDialog->SetLogWindow(mainVars);
	convertDialog->ShowModal();
	convertDialog->Close();
	delete convertDialog;
}


// --------------------------------------------------------
// ---------- OnCloseWindow -------------------------------
// --------------------------------------------------------
void MainFrame::OnCloseWindow( wxCloseEvent& event )
{
	/* call the imageCanvas CloseFile function */
	if (IsImgCanvasReady())
		imgCanvas->CloseCurrentFile();

//	delete colorMaps;
	exit(0);
}


// --------------------------------------------------------
// ---------- OnTextIntensityUpdated ----------------------
// --------------------------------------------------------
void MainFrame::OnTextIntensityUpdated( wxCommandEvent& event )
{
	if (controlsAreReady) {
		mainVars->VolRend_intensity((double)spinIntensity->GetValue()/100.0);
		imgCanvas->Draw();
	}
}


// --------------------------------------------------------
// ---------- OnSpinIntensityUpdated ----------------------
// --------------------------------------------------------
void MainFrame::OnSpinIntensityUpdated( wxSpinEvent& event )
{
	double intensity = (double)spinIntensity->GetValue()/100.0;
	txtIntensity->SetValue(wxString::Format("%.2f",intensity));
}


// --------------------------------------------------------
// ---------- OnTextDensityUpdated ------------------------
// --------------------------------------------------------
void MainFrame::OnTextDensityUpdated( wxCommandEvent& event )
{
	if (controlsAreReady) {
		mainVars->VolRend_density((double)spinDensity->GetValue()/100.0);
		imgCanvas->Draw();
	}
}


// --------------------------------------------------------
// ---------- OnSpinDensityUpdated ------------------------
// --------------------------------------------------------
void MainFrame::OnSpinDensityUpdated( wxSpinEvent& event )
{
	double density = (double)spinDensity->GetValue()/100.0;
	txtDensity->SetValue(wxString::Format("%.2f",density));
}


// --------------------------------------------------------
// ---------- OnSpinNumVolumeSlicesUpdated ----------------
// --------------------------------------------------------
void MainFrame::OnSpinNumVolumeSlicesUpdated( wxSpinEvent& event )
{
	mainVars->VolRend_numSlices(spinNumVolSlices->GetValue());
}


// --------------------------------------------------------
// ---------- OnSpinNumVolumeSlicesTextUpdated ------------
// --------------------------------------------------------
void MainFrame::OnSpinNumVolumeSlicesTextUpdated( wxCommandEvent& event )
{
	mainVars->VolRend_numSlices(spinNumVolSlices->GetValue());
	imgCanvas->Draw();
}


// --------------------------------------------------------
// ---------- OnRadioUsePlaneSelected ---------------------
// --------------------------------------------------------
void MainFrame::OnRadioUsePlaneSelected( wxCommandEvent& event )
{
	if (radUsePlane0->GetValue())
		mainVars->Image_plane(0);
	if (radUsePlane1->GetValue())
		mainVars->Image_plane(1);
	if (radUsePlane2->GetValue())
		mainVars->Image_plane(2);

	frame->SetCurrentSlice(imgCanvas->GetStartSlice(), imgCanvas->GetEndSlice(), mainVars->Slice_value());
	
	WriteLog(msg.Format("SetCurrentSlice(%d,%d,%d)",imgCanvas->GetStartSlice(), imgCanvas->GetEndSlice(), mainVars->Slice_value() ));

	imgCanvas->Draw();
}


// --------------------------------------------------------
// ---------- OnRenderMethodSelected ----------------------
// --------------------------------------------------------
void MainFrame::OnRenderMethodSelected( wxCommandEvent& event )
{
	if (radRender3DTexture->GetValue())
		mainVars->VolRend_method(VOLMETHOD_3DTEXTURE);
	if (radRenderRayTrace->GetValue())
		mainVars->VolRend_method(VOLMETHOD_RAYTRACE);

	mainVars->VolRend_reInit(true);

	imgCanvas->Draw();
}


// --------------------------------------------------------
// ---------- OnNumStepsUpdated ---------------------------
// --------------------------------------------------------
void MainFrame::OnNumStepsUpdated( wxSpinEvent& event )
{
	if (controlsAreReady) {
		mainVars->VolRend_numIterations(spinNumSteps->GetValue());
		imgCanvas->Draw();
	}
}


// --------------------------------------------------------
// ---------- OnVolumeNumUpdated --------------------------
// --------------------------------------------------------
void MainFrame::OnVolumeNumUpdated( wxSpinEvent& event )
{
	if (controlsAreReady) {
		mainVars->Image_volume(spinVolumeNum->GetValue());
		imgCanvas->Draw();
	}
}


// --------------------------------------------------------
// ---------- OnHelpReportbugClick ------------------------
// --------------------------------------------------------
void MainFrame::OnHelpReportbugClick( wxCommandEvent& event )
{
	BugReport *bugDialog = new BugReport(this, wxID_ANY, "Report a Bug...", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
	bugDialog->ShowModal();
}


// --------------------------------------------------------
// ---------- OnColormapUseWindowClick --------------------
// --------------------------------------------------------
void MainFrame::OnColormapUseWindowClick( wxCommandEvent& event )
{
	if (controlsAreReady) {
		mainVars->ColorMap_useWindowLevel(chkUseWindow->GetValue());
		imgCanvas->Draw();
	}
}


// --------------------------------------------------------
// ---------- OnFileSaveScreengrabClick -------------------
// --------------------------------------------------------
void MainFrame::OnFileSaveScreengrabClick( wxCommandEvent& event )
{
	int filetype;
	wxString filepath;

	/* show the save screenshot dialog */
	SaveScreenshot *screenshotDialog = new SaveScreenshot(this, wxID_ANY, "Save Screenshot", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
	if (screenshotDialog->ShowModal() == wxID_OK) {
		/* get the filepath and filetype */
		filepath = screenshotDialog->GetFilePath();
		filetype = screenshotDialog->GetFiletype();

		imgCanvas->SaveScreenGrab(filetype, filepath);
	}

	delete screenshotDialog;
}


// --------------------------------------------------------
// ---------- PopulateColorMapBox -------------------------
// --------------------------------------------------------
void MainFrame::PopulateColorMapBox()
{
	int i;
	wxBitmap bitmap;
	wxString name;
	cmbColorMap->Clear();

	int count = (int)colorMaps->maps.Count();

	for (i=0;i<count;i++) {
		bitmap = colorMaps->maps[i].bitmap;
		name = colorMaps->maps[i].name;
		cmbColorMap->Append(name, bitmap);
	}
	cmbColorMap->SetValue("Gray");
}


// --------------------------------------------------------
// ---------- OnColormapComboSelected ---------------------
// --------------------------------------------------------
void MainFrame::OnColormapComboSelected( wxCommandEvent& event )
{
	mainVars->ColorMap_index(cmbColorMap->GetSelection());
	imgCanvas->Draw();
}


// --------------------------------------------------------
// ---------- OnOptionsFunctionEditorClick ----------------
// --------------------------------------------------------
void MainFrame::OnOptionsFunctionEditorClick( wxCommandEvent& event )
{
	int texSize = imgCanvas->GetMaxTextureSize();
	ColorMapDialog *functionDialog = new ColorMapDialog(this, mainVars, colorMaps, texSize, this);
	functionDialog->Show();
}


// --------------------------------------------------------
// ---------- OnFileBrowserClick --------------------------
// --------------------------------------------------------
void MainFrame::OnFileBrowserClick( wxCommandEvent& event )
{
	MIViewBrowser *browserFrame = new MIViewBrowser(this);
	browserFrame->Show();
	browserFrame->SetPointers(this, mainVars);
}


// --------------------------------------------------------
// ---------- OpenFiles -----------------------------------
// --------------------------------------------------------
void MainFrame::OpenFiles(wxArrayString filepaths)
{
	wxFileName file(filepaths[0]);
	wxString ext = file.GetExt();
	int type;

	type = FILETYPE_DICOM;

	if ((ext == "dcm") || (ext == "acr") || (ext == ""))
		type = FILETYPE_DICOM;
	if ((ext == "img") || (ext == "hdr") || (ext == "nii"))
		type = FILETYPE_ANALYZE;
	if ((ext == "jpg") || (ext == "jpeg") || (ext == "jpe") || (ext == "jff") || (ext == "jfif") || (ext == "jif"))
		type = FILETYPE_RASTER;
	if ((ext == "gif") || (ext == "bmp") || (ext == "xpm") || (ext == "tif") || (ext == "tiff") || (ext == "png"))
		type = FILETYPE_RASTER;
	if ((ext == "pcx") || (ext == "pnm") || (ext == "ico") || (ext == "cur") || (ext == "ani") || (ext == "jtf"))
		type = FILETYPE_RASTER;

	/* probably called from command line, so load the file and display it */
	//wxArrayString filenames;

	/* check to see which, and how many files should be loaded */
	//filenames.Add(filepath);

	/* setup the variables to used by the loading routines */
	mainVars->Load_filenames(filepaths);
	mainVars->Load_filetype(type);
	mainVars->Load_loadType(OPEN_SELECTED);

	/* try to load the file(s) */
	if (!imgCanvas->LoadFile()) {
		WriteLog("File not loaded...");
		this->GetAuiManager().GetPane("panelGLCanvas").Caption("File not loaded");
		this->SendSizeEvent();
	}
	else {
		WriteLog("File loaded...");
		this->GetAuiManager().GetPane("panelGLCanvas").Caption(filepaths[0]);
		this->SendSizeEvent();
	}

}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXT_SSD_CUTOFF
 */

void MainFrame::OnTextSSDCutoffTextUpdated( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXT_SSD_CUTOFF in MainFrame.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXT_SSD_CUTOFF in MainFrame. 
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPIN_SSD_CUTOFF
 */

void MainFrame::OnSpinSSDCutoffUpdated( wxSpinEvent& event )
{
////@begin wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPIN_SSD_CUTOFF in MainFrame.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPIN_SSD_CUTOFF in MainFrame. 
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_USE_SSD
 */

void MainFrame::OnUseSSDClick( wxCommandEvent& event )
{
	if (chkUseShading->IsChecked())
		mainVars->VolRend_useSSD(true);
	else
		mainVars->VolRend_useSSD(false);

	imgCanvas->Draw();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_TRACK_POINTS
 */

//void MainFrame::OnTrackPointsClick( wxCommandEvent& event )
//{
//	if (chkTrackPoints->IsChecked())
//		mainVars->Image_trackPoints(true);
//	else
//		mainVars->Image_trackPoints(false);
//
//	imgCanvas->Draw();
//}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_VIEW_POINT_LIST
 */

//void MainFrame::OnViewPointListClick( wxCommandEvent& event )
//{
//	wxString msg;
//	int x, y;
//	wxFile *pointsFile;
//
//	pointsFile = new wxFile();
//	pointsFile->Open(wxT("points.log"), wxFile::write);
//	for (int i=0;i<(int)displayPoints->thePoints.Count();i++) {
//		x = displayPoints->thePoints[i].x;
//		y = displayPoints->thePoints[i].y;
//		pointsFile->Write(wxString::Format("%d,%d\n",x,y));
//	}
//	pointsFile->Close();
//}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_DISPLAY_TRACKED_POINTS
 */

//void MainFrame::OnDisplayTrackedPointsClick( wxCommandEvent& event )
//{
//	if (chkDisplayTrackedPoints->IsChecked())
//		mainVars->Image_displayTrackedPoints(true);
//	else
//		mainVars->Image_displayTrackedPoints(false);
//
//	imgCanvas->Draw();
//}


///*!
// * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL1
// */
//
//void MainFrame::OnSpinZUpdated( wxSpinEvent& event )
//{
//////@begin wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL1 in MainFrame.
//    // Before editing this code, remove the block markers.
//    event.Skip();
//////@end wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL1 in MainFrame. 
//}
//
//
///*!
// * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SPINCTRL1
// */
//
//void MainFrame::OnSpinZTextUpdated( wxCommandEvent& event )
//{
//////@begin wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SPINCTRL1 in MainFrame.
//    // Before editing this code, remove the block markers.
//    event.Skip();
//////@end wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SPINCTRL1 in MainFrame. 
//}


/*!
 * wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER_ANGLE
 */

void MainFrame::OnSliderAngleUpdated( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER_ANGLE in MainFrame.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER_ANGLE in MainFrame. 
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPIN_ANGLE
 */

void MainFrame::OnSpinAngleUpdated( wxSpinEvent& event )
{
////@begin wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPIN_ANGLE in MainFrame.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPIN_ANGLE in MainFrame. 
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SPIN_ANGLE
 */

void MainFrame::OnSpinAngleTextUpdated( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SPIN_ANGLE in MainFrame.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SPIN_ANGLE in MainFrame. 
}

