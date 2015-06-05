/* ----------------------------------------------------------------------------
	MainFrame.h


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

#ifndef _MAINFRAME_H_
#define _MAINFRAME_H_

//#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
//#pragma interface "MainFrame.h"
//#endif

#if (_MSC_VER >= 1400)       // VC8+
#pragma warning(disable : 4996)    // Either disable all deprecation warnings,
#pragma warning(disable : 4244)    // Either disable all deprecation warnings,
// Or just turn off warnings about the newly deprecated CRT functions.
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE 1
#endif
#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE 1
#endif
#endif   // VC8+

#include <crtdbg.h>
#define new new(_NORMAL_BLOCK,__FILE__, __LINE__)

#include "glew.h"

////@begin includes
#include "wx/aui/framemanager.h"
#include "wx/frame.h"
#include "wx/toolbar.h"
#include "wx/scrolbar.h"
#include "wx/glcanvas.h"
#include "wx/spinctrl.h"
#include "wx/spinbutt.h"
#include "wx/statline.h"
#include "wx/bmpcbox.h"
#include "wx/statusbr.h"
////@end includes
#include "MIView.h"
#include "wx/frame.h"
#include "wx/toolbar.h"
#include "wx/scrolbar.h"
#include "wx/glcanvas.h"
#include "wx/spinctrl.h"
#include "wx/statusbr.h"
#include "wx/sizer.h"

#include <sys/timeb.h>
#include "wx/glcanvas.h"
#include "MIViewConstants.h"
#include "wx/dynarray.h"
#include "Color.h"
#include "XYZTriplet.h"
#include "ColorMapList.h"
//#include <gl/glu.h>
#include <gl/glut.h>
#include "GlobalRoutines.h"
#include "DisplayPoints.h"

////@begin control identifiers
#define ID_MAINFRAME 16000
#define ID_FILE_OPEN 16002
#define ID_FILE_CLOSE 16003
#define ID_FILE_BROWSER 16065
#define ID_FILE_SAVE_SCREENGRAB 16077
#define ID_FILE_EXIT 16004
#define ID_EDIT_OPTIONS 16005
#define ID_OPTIONS_FUNCTION_EDITOR 16064
#define ID_VIEW_NORMAL 16006
#define ID_VIEW_ORTHOGONAL 16007
#define ID_VIEW_MULTISLICE 16008
#define ID_VIEW_MIP 16009
#define ID_VIEW_SSD 16010
#define ID_VIEW_VOL_REND 16011
#define ID_VIEW_IMAGE_OVERLAY 16012
#define ID_IMAGE_INFORMATION 16013
#define ID_FILE_ANONYMIZE 16014
#define ID_FILE_CONVERT 16015
#define ID_HELP_HELP 16016
#define ID_HELP_REPORTBUG 16075
#define ID_HELP_ABOUT 16017
#define ID_MAIN_TOOLBAR 16018
#define ID_MAIN_TOOLBAR_OPEN_FILES 16019
#define ID_MAIN_TOOLBAR_CLOSE_FILE 16020
#define ID_MAIN_TOOLBAR_VIEW_NORMAL 16021
#define ID_MAIN_TOOLBAR_VIEW_ORTHOGONAL 16022
#define ID_MAIN_TOOLBAR_VIEW_MULTISLICE 16023
#define ID_MAIN_TOOLBAR_VIEW_MIP 16024
#define ID_MAIN_TOOLBAR_VIEW_SSD 16025
#define ID_MAIN_TOOLBAR_VIEW_VOL_REND 16026
#define ID_MAIN_TOOLBAR_SHOW_OVERLAY 16027
#define ID_MAIN_TOOLBAR_SHOW_LINES 16028
#define ID_IMAGE_TOOLBAR_HIRES 16029
#define ID_MAIN_TOOLBAR_INFO 16030
#define ID_GLCANVAS_PANEL 16031
#define ID_SCROLLBAR 16032
#define ID_GLCANVAS 16033
#define ID_IMAGE_PANEL 16034
#define ID_BESTFIT_ZOOM 16035
#define ID_ZOOM_SLIDER 16036
#define ID_SPIN_ZOOM 16037
#define ID_RADIO_USE_PLANE0 16001
#define ID_RADIO_USE_PLANE1 16044
#define ID_RADIO_USE_PLANE2 16048
#define ID_VOLUME_NUM 16074
#define ID_MULTISLICE_PANEL 16038
#define ID_ORDER_BY_ROWS 16039
#define ID_ORDER_BY_COLS 16040
#define ID_CHECK_BESTFIT_MULTISLICE 16043
#define ID_NUM_ROWS 16041
#define ID_NUM_COLS 16042
#define ID_VOLUME_PANEL 16045
#define ID_RENDERMETHOD_RAY_TRACE 16067
#define ID_RENDERMETHOD_3D_TEXTURE 16066
#define ID_NUM_STEPS 16073
#define ID_USE_SSD 16046
#define ID_TEXT_SSD_CUTOFF 16069
#define ID_SPIN_SSD_CUTOFF 16047
#define ID_TEXT_INTENSITY 16049
#define ID_SPIN_INTENSITY 16050
#define ID_SPIN_NUM_VOLUME_SLICES 16053
#define ID_TEXT_DENSITY 16051
#define ID_SPIN_DENSITY 16052
#define ID_ROTATIONS_PANEL 16054
#define ID_SLIDER_X_ROTATE 16056
#define ID_SPIN_X 16055
#define ID_SLIDER_Y_ROTATE 16078
#define ID_SPIN_Y 16080
#define ID_SLIDER_Z_ROTATE 16057
#define ID_SPIN_Z 16058
#define ID_SLIDER_ANGLE 16059
#define ID_SPIN_ANGLE 16060
#define ID_RESET_ROTATION 16061
#define ID_COLOR_MAP_PANEL 16062
#define ID_COLORMAP_USE_WINDOW 16076
#define ID_COLORIZE 16063
#define ID_COLORMAP_COMBO 16079
#define ID_STATUSBAR 16068
#define SYMBOL_MAINFRAME_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxCLOSE_BOX
#define SYMBOL_MAINFRAME_TITLE _("Medical Image Viewer")
#define SYMBOL_MAINFRAME_IDNAME ID_MAINFRAME
#define SYMBOL_MAINFRAME_SIZE wxSize(650, 750)
#define SYMBOL_MAINFRAME_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/* forward declare some classes */
class ImageCanvas;
//class LogWindow;
class OptionsDialog;
class ConvertDialog;
class AboutDialog;
class AnonymizeDialog;
class MainFrameVariables;
class ColorMapList;
class DisplayPoints;

////@begin forward declarations
class wxBoxSizer;
class ImageCanvas;
class wxSpinCtrl;
class wxSpinButton;
class wxBitmapComboBox;
class wxStatusBar;
////@end forward declarations

/* define the main frame */
class MainFrame: public wxFrame
{
    DECLARE_CLASS( MainFrame )
    DECLARE_EVENT_TABLE()

	public:
		/// Constructors
		MainFrame( );
		MainFrame( wxWindow* parent, wxWindowID id = SYMBOL_MAINFRAME_IDNAME, const wxString& caption = SYMBOL_MAINFRAME_TITLE, const wxPoint& pos = SYMBOL_MAINFRAME_POSITION, const wxSize& size = SYMBOL_MAINFRAME_SIZE, long style = SYMBOL_MAINFRAME_STYLE );

		bool Create( wxWindow* parent, wxWindowID id = SYMBOL_MAINFRAME_IDNAME, const wxString& caption = SYMBOL_MAINFRAME_TITLE, const wxPoint& pos = SYMBOL_MAINFRAME_POSITION, const wxSize& size = SYMBOL_MAINFRAME_SIZE, long style = SYMBOL_MAINFRAME_STYLE );

		/// Creates the controls and sizers
		void CreateControls();

		void SetCurrentSlice(int startSlice, int endSlice, int slice);
		void SetCurrentRotation(float angle, float x, float y, float z);
		void SetZoom(int zoom);
		bool IsImgCanvasReady();
		void OpenFiles(wxArrayString filepaths);

		/// the one and only (for now) image canvas object
		ImageCanvas *imgCanvas;

		/// this will allow variables to be accessed more easily outside of MainFrame
		MainFrameVariables *mainVars;

		ColorMapList *colorMaps;
		DisplayPoints *displayPoints;

		wxString openArgs;
		void PopulateColorMapBox();

	private:
		/* functions used only in this class */
		void SetRotationTo(int angle);
		//wxBitmap GenerateGradientBitmap(wxSize size, int numColors, wxArrayInt r, wxArrayInt g, wxArrayInt b, wxArrayInt a, wxArrayInt p);
		//wxArrayString Explode(wxString str, wxChar chr);

		/* variables used only in this class */
		bool controlsAreReady;
		bool imgCanvasIsReady;
		int numOpenImages;
		OptionsDialog *optDialog;
		//LogWindow *logWindow; /* create a log window */

////@begin MainFrame event handler declarations

    /// wxEVT_CLOSE_WINDOW event handler for ID_MAINFRAME
    void OnCloseWindow( wxCloseEvent& event );

    /// wxEVT_SIZE event handler for ID_MAINFRAME
    void OnSize( wxSizeEvent& event );

    /// wxEVT_PAINT event handler for ID_MAINFRAME
    void OnPaint( wxPaintEvent& event );

    /// wxEVT_MOUSEWHEEL event handler for ID_MAINFRAME
    void OnMouseWheel( wxMouseEvent& event );

    /// wxEVT_KEY_DOWN event handler for ID_MAINFRAME
    void OnKeyDown( wxKeyEvent& event );

    /// wxEVT_KEY_UP event handler for ID_MAINFRAME
    void OnKeyUp( wxKeyEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_FILE_OPEN
    void OnFileOpen( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_FILE_CLOSE
    void OnFileClose( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_FILE_BROWSER
    void OnFileBrowserClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_FILE_SAVE_SCREENGRAB
    void OnFileSaveScreengrabClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_FILE_EXIT
    void OnFileExit( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_EDIT_OPTIONS
    void OnEditOptions( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_OPTIONS_FUNCTION_EDITOR
    void OnOptionsFunctionEditorClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_VIEW_NORMAL
    void OnViewNormal( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_VIEW_ORTHOGONAL
    void OnViewOrthogonal( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_VIEW_MIP
    void OnViewMIP( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_VIEW_SSD
    void OnViewSSD( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_VIEW_VOL_REND
    void OnViewVolumeRendering( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_VIEW_IMAGE_OVERLAY
    void OnShowOverlay( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_IMAGE_INFORMATION
    void OnShowImageInformation( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_FILE_ANONYMIZE
    void OnAnonymize( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_FILE_CONVERT
    void OnFileConvertClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_HELP_HELP
    void OnHelpHelp( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_HELP_REPORTBUG
    void OnHelpReportbugClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_HELP_ABOUT
    void OnHelpAbout( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MAIN_TOOLBAR_VIEW_MULTISLICE
    void OnViewMultislice( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MAIN_TOOLBAR_SHOW_LINES
    void OnMainToolbarShowLinesClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_IMAGE_TOOLBAR_HIRES
    void OnViewHiRes( wxCommandEvent& event );

    /// wxEVT_SCROLL_* event handler for ID_SCROLLBAR
    void OnScrollSliceScroll( wxScrollEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_BESTFIT_ZOOM
    void OnBestfitZoomClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_ZOOM_SLIDER
    void OnZoomSliderUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPIN_ZOOM
    void OnSpinZoomUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SPIN_ZOOM
    void OnSpinZoomTextUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIO_USE_PLANE0
    void OnRadioUsePlaneSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_VOLUME_NUM
    void OnVolumeNumUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RENDERMETHOD_RAY_TRACE
    void OnRenderMethodSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_NUM_STEPS
    void OnNumStepsUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_USE_SSD
    void OnUseSSDClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXT_SSD_CUTOFF
    void OnTextSSDCutoffTextUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPIN_SSD_CUTOFF
    void OnSpinSSDCutoffUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXT_INTENSITY
    void OnTextIntensityUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPIN_INTENSITY
    void OnSpinIntensityUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPIN_NUM_VOLUME_SLICES
    void OnSpinNumVolumeSlicesUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SPIN_NUM_VOLUME_SLICES
    void OnSpinNumVolumeSlicesTextUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXT_DENSITY
    void OnTextDensityUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPIN_DENSITY
    void OnSpinDensityUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER_X_ROTATE
    void OnSliderXRotateUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPIN_X
    void OnSpinXUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SPIN_X
    void OnSpinXTextUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER_Y_ROTATE
    void OnSliderYRotateUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPIN_Y
    void OnSpinYUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SPIN_Y
    void OnSpinYTextUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER_Z_ROTATE
    void OnSliderZRotateUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPIN_Z
    void OnSpinZUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SPIN_Z
    void OnSpinZTextUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER_ANGLE
    void OnSliderAngleUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPIN_ANGLE
    void OnSpinAngleUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SPIN_ANGLE
    void OnSpinAngleTextUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RESET_ROTATION
    void OnResetRotation( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_COLORMAP_USE_WINDOW
    void OnColormapUseWindowClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_COLORIZE
    void OnUseColorMapClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COLORMAP_COMBO
    void OnColormapComboSelected( wxCommandEvent& event );

////@end MainFrame event handler declarations

////@begin MainFrame member function declarations

    /// Returns the AUI manager object
    wxAuiManager& GetAuiManager() { return m_auiManager; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end MainFrame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin MainFrame member variables
    wxAuiManager m_auiManager;
    wxToolBar* mainToolbar;
    wxPanel* panelCanvas;
    wxBoxSizer* sizerCanvas;
    wxScrollBar* scrollSlice;
    //ImageCanvas* imgCanvas;
    wxPanel* panelImage;
    wxCheckBox* chkBestFitZoom;
    wxSlider* sldZoom;
    wxSpinCtrl* spinZoom;
    wxRadioButton* radUsePlane0;
    wxRadioButton* radUsePlane1;
    wxRadioButton* radUsePlane2;
    wxSpinCtrl* spinVolumeNum;
    wxPanel* panelMultislice;
    wxRadioButton* radByRows;
    wxRadioButton* radByCols;
    wxCheckBox* chkBestFitMultislice;
    wxSpinCtrl* spinNumRows;
    wxSpinCtrl* spinNumCols;
    wxPanel* panelVolumeRendering;
    wxRadioButton* radRenderRayTrace;
    wxRadioButton* radRender3DTexture;
    wxSpinCtrl* spinNumSteps;
    wxCheckBox* chkUseShading;
    wxTextCtrl* txtSSDCutoff;
    wxSpinButton* spinSSDCutoff;
    wxTextCtrl* txtIntensity;
    wxSpinButton* spinIntensity;
    wxSpinCtrl* spinNumVolSlices;
    wxTextCtrl* txtDensity;
    wxSpinButton* spinDensity;
    wxPanel* panelRotations;
    wxSlider* sldXRotate;
    wxSpinCtrl* spinX;
    wxSlider* sldYRotate;
    wxSpinCtrl* spinY;
    wxSlider* sldZRotate;
    wxSpinCtrl* spinZ;
    wxSlider* sldAngleRotate;
    wxSpinCtrl* spinAngle;
    wxButton* btnResetRotation;
    wxPanel* panelColorMap;
    wxCheckBox* chkUseWindow;
    wxCheckBox* chkColorize;
    wxBitmapComboBox* cmbColorMap;
    wxStatusBar* statusbar;
////@end MainFrame member variables
public:
	void HideSliceScroll(bool hide);
	//void ResetRotation(void);
};

#endif
    // _MAINFRAME_H_
