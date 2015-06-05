/* ----------------------------------------------------------------------------
	ColorMapDialog.h


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


#ifndef _COLORMAPDIALOG_H_
#define _COLORMAPDIALOG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "ColorMapDialog.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/statline.h"
#include "wx/glcanvas.h"
#include "wx/clrpicker.h"
#include "wx/spinctrl.h"
////@end includes
#include "ColorMap.h"
#include "wx/textfile.h"
#include "wx/dynarray.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class ColorMapCanvas;
class wxColourPickerCtrl;
class wxSpinCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_COLORMAPDIALOG 24000
#define ID_PANEL 24007
#define ID_COLORMAP_LIST 24003
#define ID_DELETE_FUNCTION 24002
#define ID_NEW_FUNCTION 24004
#define ID_PANEL1 24008
#define ID_PREVIEW_CANVAS 24005
#define ID_TEXT_NAME 24014
#define ID_COLORPICKER 24011
#define ID_SPIN_ALPHA 24012
#define ID_SPIN_POSITION 24013
#define ID_BUTTON_DELETE 24017
#define ID_SAVE_COLOR_MAPS 24006
#define SYMBOL_COLORMAPDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_COLORMAPDIALOG_TITLE _("Color Maps")
#define SYMBOL_COLORMAPDIALOG_IDNAME ID_COLORMAPDIALOG
#define SYMBOL_COLORMAPDIALOG_SIZE wxDefaultSize
#define SYMBOL_COLORMAPDIALOG_POSITION wxDefaultPosition
////@end control identifiers

//WX_DECLARE_OBJARRAY(ColorMap,c_colorMaps);

/*!
 * ColorMapDialog class declaration
 */

class ColorMapDialog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( ColorMapDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ColorMapDialog();
    ColorMapDialog(MainFrame *Frame, MainFrameVariables *MainVars, ColorMapList *mapList, int texSize, wxWindow* parent, wxWindowID id = SYMBOL_COLORMAPDIALOG_IDNAME, const wxString& caption = SYMBOL_COLORMAPDIALOG_TITLE, const wxPoint& pos = SYMBOL_COLORMAPDIALOG_POSITION, const wxSize& size = SYMBOL_COLORMAPDIALOG_SIZE, long style = SYMBOL_COLORMAPDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_COLORMAPDIALOG_IDNAME, const wxString& caption = SYMBOL_COLORMAPDIALOG_TITLE, const wxPoint& pos = SYMBOL_COLORMAPDIALOG_POSITION, const wxSize& size = SYMBOL_COLORMAPDIALOG_SIZE, long style = SYMBOL_COLORMAPDIALOG_STYLE );

    /// Destructor
    ~ColorMapDialog();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

	void SetObjectPointers(MainFrame *Frame, MainFrameVariables *MainVars, ColorMapList *mapList, int texSize);
	wxArrayString Explode(wxString str, wxChar chr);
	void PopulateColorMapTree();
	void UpdateSelectedColor();
	void UpdateControlsForSelectedItem();
	void SortColorMapByPosition(int map);
	void AddColorToMap(int r, int g, int b, int a, int p);
	void UpdateImageCanvas();

////@begin ColorMapDialog event handler declarations

    /// wxEVT_CLOSE_WINDOW event handler for ID_COLORMAPDIALOG
    void OnCloseWindow( wxCloseEvent& event );

    /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_COLORMAP_LIST
    void OnColormapListSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DELETE_FUNCTION
    void OnDeleteFunctionClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_NEW_FUNCTION
    void OnNewFunctionClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXT_NAME
    void OnTextNameTextUpdated( wxCommandEvent& event );

    /// wxEVT_COLOURPICKER_CHANGED event handler for ID_COLORPICKER
    void OnColorpickerColourPickerChanged( wxColourPickerEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPIN_ALPHA
    void OnSpinAlphaUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPIN_POSITION
    void OnSpinPositionUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DELETE
    void OnButtonDeleteClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SAVE_COLOR_MAPS
    void OnSaveColorMapsClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnDoneClick( wxCommandEvent& event );

////@end ColorMapDialog event handler declarations

////@begin ColorMapDialog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end ColorMapDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin ColorMapDialog member variables
    wxListBox* lstColorMaps;
    wxButton* btnDeleteFunction;
    wxButton* btnNewFunction;
    ColorMapCanvas* previewCanvas;
    wxTextCtrl* txtColorMapName;
    wxColourPickerCtrl* btnColorPicker;
    wxSpinCtrl* spinAlpha;
    wxSpinCtrl* spinPosition;
    wxButton* btnDeleteSelected;
    wxButton* btnSaveMaps;
////@end ColorMapDialog member variables

	//LogWindow *logWindow;
	MainFrame *frame;
	MainFrameVariables *mainVars;
	ColorMapList *colorMapList;

	int glTextureSize;
	wxSize bitmapSize;
	
	bool comboRepopulated;

	int currentMap;
	int currentColor;
};

#endif
