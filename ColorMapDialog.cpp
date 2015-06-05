/* ----------------------------------------------------------------------------
	ColorMapDialog.cpp


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
#pragma implementation "ColorMapDialog.h"
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
#include "ColorMapCanvas.h"
////@end includes

#include "MainFrame.h"
#include "ImageCanvas.h"

#include "ColorMapDialog.h"

////@begin XPM images
////@end XPM images

/*!
 * ColorMapDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ColorMapDialog, wxDialog )


/*!
 * ColorMapDialog event table definition
 */

BEGIN_EVENT_TABLE( ColorMapDialog, wxDialog )

////@begin ColorMapDialog event table entries
    EVT_CLOSE( ColorMapDialog::OnCloseWindow )

    EVT_LISTBOX( ID_COLORMAP_LIST, ColorMapDialog::OnColormapListSelected )

    EVT_BUTTON( ID_DELETE_FUNCTION, ColorMapDialog::OnDeleteFunctionClick )

    EVT_BUTTON( ID_NEW_FUNCTION, ColorMapDialog::OnNewFunctionClick )

    EVT_TEXT( ID_TEXT_NAME, ColorMapDialog::OnTextNameTextUpdated )

    EVT_COLOURPICKER_CHANGED( ID_COLORPICKER, ColorMapDialog::OnColorpickerColourPickerChanged )

    EVT_SPINCTRL( ID_SPIN_ALPHA, ColorMapDialog::OnSpinAlphaUpdated )

    EVT_SPINCTRL( ID_SPIN_POSITION, ColorMapDialog::OnSpinPositionUpdated )

    EVT_BUTTON( ID_BUTTON_DELETE, ColorMapDialog::OnButtonDeleteClick )

    EVT_BUTTON( ID_SAVE_COLOR_MAPS, ColorMapDialog::OnSaveColorMapsClick )

    EVT_BUTTON( wxID_OK, ColorMapDialog::OnDoneClick )

////@end ColorMapDialog event table entries

END_EVENT_TABLE()


// --------------------------------------------------------
// ---------- ColorMapDialog ------------------------------
// --------------------------------------------------------
ColorMapDialog::ColorMapDialog()
{
    Init();
}

ColorMapDialog::ColorMapDialog(MainFrame *Frame, MainFrameVariables *MainVars, ColorMapList *mapList, int texSize, wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	//logWindow = logwind;
	frame = Frame;
	mainVars = MainVars;
	colorMapList = mapList;
    Init();
    Create(parent, id, caption, pos, size, style);
}


// --------------------------------------------------------
// ---------- Create --------------------------------------
// --------------------------------------------------------
bool ColorMapDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin ColorMapDialog creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end ColorMapDialog creation

//	ReadColorMapFile();
	PopulateColorMapTree();

    return true;
}


// --------------------------------------------------------
// ---------- ~ColorMapDialog -----------------------------
// --------------------------------------------------------
ColorMapDialog::~ColorMapDialog()
{
////@begin ColorMapDialog destruction
////@end ColorMapDialog destruction
}


// --------------------------------------------------------
// ---------- Init ----------------------------------------
// --------------------------------------------------------
void ColorMapDialog::Init()
{
////@begin ColorMapDialog member initialisation
    lstColorMaps = NULL;
    btnDeleteFunction = NULL;
    btnNewFunction = NULL;
    previewCanvas = NULL;
    txtColorMapName = NULL;
    btnColorPicker = NULL;
    spinAlpha = NULL;
    spinPosition = NULL;
    btnDeleteSelected = NULL;
    btnSaveMaps = NULL;
////@end ColorMapDialog member initialisation

	currentMap = 0;
	currentColor = -1;
	bitmapSize = wxSize(120,15);
	comboRepopulated = false;
}

void ColorMapDialog::SetObjectPointers(MainFrame *Frame, MainFrameVariables *MainVars, ColorMapList *mapList, int texSize)
{
	//logWindow = logwind;
	frame = Frame;
	mainVars = MainVars;
	colorMapList = mapList;
	glTextureSize = texSize;
}


// --------------------------------------------------------
// ---------- CreateControls ------------------------------
// --------------------------------------------------------
void ColorMapDialog::CreateControls()
{    
	/* setup the OpenGL canvas */
	int glAttributes[5] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 32, 0 };

////@begin ColorMapDialog content construction
    ColorMapDialog* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer4, 0, wxGROW|wxALL, 5);

    wxPanel* itemPanel5 = new wxPanel( itemDialog1, ID_PANEL, wxDefaultPosition, wxSize(-1, 20), wxTAB_TRAVERSAL );
    itemPanel5->SetBackgroundColour(wxColour(0, 0, 132));
    itemBoxSizer4->Add(itemPanel5, 0, wxGROW|wxALL, 0);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemPanel5->SetSizer(itemBoxSizer6);

    itemBoxSizer6->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticText* itemStaticText8 = new wxStaticText( itemPanel5, wxID_STATIC, _("Color Maps"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
    itemStaticText8->SetForegroundColour(wxColour(255, 255, 255));
    itemBoxSizer6->Add(itemStaticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString lstColorMapsStrings;
    lstColorMaps = new wxListBox( itemDialog1, ID_COLORMAP_LIST, wxDefaultPosition, wxDefaultSize, lstColorMapsStrings, wxLB_SINGLE|wxLB_NEEDED_SB|wxLB_HSCROLL );
    itemBoxSizer4->Add(lstColorMaps, 1, wxGROW|wxALL, 0);

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer4->Add(itemBoxSizer10, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    btnDeleteFunction = new wxButton( itemDialog1, ID_DELETE_FUNCTION, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(btnDeleteFunction, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    btnNewFunction = new wxButton( itemDialog1, ID_NEW_FUNCTION, _("New..."), wxDefaultPosition, wxDefaultSize, 0 );
    btnNewFunction->SetDefault();
    itemBoxSizer10->Add(btnNewFunction, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticLine* itemStaticLine13 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
    itemBoxSizer3->Add(itemStaticLine13, 0, wxGROW|wxALL, 0);

    wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer14, 1, wxGROW|wxALL, 5);

    wxPanel* itemPanel15 = new wxPanel( itemDialog1, ID_PANEL1, wxDefaultPosition, wxSize(-1, 20), wxTAB_TRAVERSAL );
    itemPanel15->SetBackgroundColour(wxColour(0, 0, 132));
    itemBoxSizer14->Add(itemPanel15, 0, wxGROW|wxALL, 0);

    wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
    itemPanel15->SetSizer(itemBoxSizer16);

    itemBoxSizer16->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticText* itemStaticText18 = new wxStaticText( itemPanel15, wxID_STATIC, _("Preview"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
    itemStaticText18->SetForegroundColour(wxColour(255, 255, 255));
    itemBoxSizer16->Add(itemStaticText18, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    previewCanvas = new ColorMapCanvas( itemDialog1, ID_PREVIEW_CANVAS, wxDefaultPosition, wxSize(-1, 150), wxSIMPLE_BORDER, wxT("GLCanvas"), glAttributes );
    itemBoxSizer14->Add(previewCanvas, 1, wxGROW|wxALL, 0);

    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer14->Add(itemBoxSizer20, 0, wxGROW|wxALL, 2);

    wxStaticText* itemStaticText21 = new wxStaticText( itemDialog1, wxID_STATIC, _("Left-click to select color. Click and move to change position"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText21->SetFont(wxFont(7, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Tahoma")));
    itemBoxSizer20->Add(itemStaticText21, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemBoxSizer20->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText23 = new wxStaticText( itemDialog1, wxID_STATIC, _("Right click to add a new color"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText23->SetFont(wxFont(7, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Tahoma")));
    itemBoxSizer20->Add(itemStaticText23, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxBoxSizer* itemBoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer14->Add(itemBoxSizer24, 0, wxGROW|wxALL, 2);

    wxStaticText* itemStaticText25 = new wxStaticText( itemDialog1, wxID_STATIC, _("Colormap Name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer24->Add(itemStaticText25, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtColorMapName = new wxTextCtrl( itemDialog1, ID_TEXT_NAME, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer24->Add(txtColorMapName, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticBox* itemStaticBoxSizer27Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Colors"));
    wxStaticBoxSizer* itemStaticBoxSizer27 = new wxStaticBoxSizer(itemStaticBoxSizer27Static, wxHORIZONTAL);
    itemBoxSizer14->Add(itemStaticBoxSizer27, 0, wxGROW|wxALL, 1);

    btnColorPicker = new wxColourPickerCtrl( itemDialog1, ID_COLORPICKER, wxColour(255, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    itemStaticBoxSizer27->Add(btnColorPicker, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    itemStaticBoxSizer27->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText30 = new wxStaticText( itemDialog1, wxID_STATIC, _("Alpha"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ColorMapDialog::ShowToolTips())
        itemStaticText30->SetToolTip(_("Range: 0-255"));
    itemStaticBoxSizer27->Add(itemStaticText30, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    spinAlpha = new wxSpinCtrl( itemDialog1, ID_SPIN_ALPHA, _T("255"), wxDefaultPosition, wxSize(50, -1), wxSP_ARROW_KEYS, 0, 255, 255 );
    if (ColorMapDialog::ShowToolTips())
        spinAlpha->SetToolTip(_("Range: 0-255"));
    itemStaticBoxSizer27->Add(spinAlpha, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    itemStaticBoxSizer27->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText33 = new wxStaticText( itemDialog1, wxID_STATIC, _("Position"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ColorMapDialog::ShowToolTips())
        itemStaticText33->SetToolTip(_("Range: 0-1000"));
    itemStaticBoxSizer27->Add(itemStaticText33, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    spinPosition = new wxSpinCtrl( itemDialog1, ID_SPIN_POSITION, _T("0"), wxDefaultPosition, wxSize(75, -1), wxSP_ARROW_KEYS, 0, 1000, 0 );
    if (ColorMapDialog::ShowToolTips())
        spinPosition->SetToolTip(_("Range: 0-1000"));
    itemStaticBoxSizer27->Add(spinPosition, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    itemStaticBoxSizer27->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    btnDeleteSelected = new wxButton( itemDialog1, ID_BUTTON_DELETE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer27->Add(btnDeleteSelected, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticLine* itemStaticLine37 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer2->Add(itemStaticLine37, 0, wxGROW|wxALL, 0);

    wxBoxSizer* itemBoxSizer38 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer38, 0, wxGROW|wxALL, 0);

    itemBoxSizer38->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText40 = new wxStaticText( itemDialog1, wxID_STATIC, _("Note: You must Save Color Maps to make changes permanent"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText40->SetForegroundColour(wxColour(128, 0, 0));
    itemStaticText40->SetFont(wxFont(7, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Tahoma")));
    itemBoxSizer38->Add(itemStaticText40, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    btnSaveMaps = new wxButton( itemDialog1, ID_SAVE_COLOR_MAPS, _("Save Color Maps"), wxDefaultPosition, wxDefaultSize, 0 );
    btnSaveMaps->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
    itemBoxSizer38->Add(btnSaveMaps, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer38->Add(25, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton43 = new wxButton( itemDialog1, wxID_OK, _("Done"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer38->Add(itemButton43, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end ColorMapDialog content construction

	previewCanvas->SetObjectPointers(frame,mainVars,this,colorMapList);
}


/*!
 * Should we show tooltips?
 */

bool ColorMapDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ColorMapDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ColorMapDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end ColorMapDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon ColorMapDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ColorMapDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end ColorMapDialog icon retrieval
}


// --------------------------------------------------------
// ---------- OnDeleteFunctionClick -----------------------
// --------------------------------------------------------
void ColorMapDialog::OnDeleteFunctionClick( wxCommandEvent& event )
{
	int i = lstColorMaps->GetSelection();

	colorMapList->DeleteMap(i);
}


// --------------------------------------------------------
// ---------- OnNewFunctionClick --------------------------
// --------------------------------------------------------
void ColorMapDialog::OnNewFunctionClick( wxCommandEvent& event )
{
	ColorMap newMap(glTextureSize,bitmapSize);
	wxString name = "NewMap";

	colorMapList->AddMap(name);

	lstColorMaps->Append(name);
}


// --------------------------------------------------------
// ---------- OnButtonDeleteClick -------------------------
// --------------------------------------------------------
void ColorMapDialog::OnButtonDeleteClick( wxCommandEvent& event )
{
	colorMapList->DeleteColorFromMap(currentMap,currentColor);
//	colorMapList->DeleteMap(currentMap);

	previewCanvas->Draw();
	UpdateImageCanvas();
}


// --------------------------------------------------------
// ---------- PopulateColorMapTree ------------------------
// --------------------------------------------------------
void ColorMapDialog::PopulateColorMapTree()
{
	wxString name;
	int count;
	int i;

	count = (int)colorMapList->maps.Count();

	for (i=0;i<count;i++) {
		name = colorMapList->maps[i].name;
		lstColorMaps->Append(name);
	}
}


// --------------------------------------------------------
// ---------- OnSaveColorMapsClick ------------------------
// --------------------------------------------------------
void ColorMapDialog::OnSaveColorMapsClick( wxCommandEvent& event )
{
	colorMapList->WriteColorMapFile();
}


// --------------------------------------------------------
// ---------- OnTextNameTextUpdated -----------------------
// --------------------------------------------------------
void ColorMapDialog::OnTextNameTextUpdated( wxCommandEvent& event )
{
	wxString name = txtColorMapName->GetValue();

	/* update the name of this map */
	colorMapList->maps[currentMap].name = name;
	lstColorMaps->SetString(lstColorMaps->GetSelection(), name);
}


// --------------------------------------------------------
// ---------- OnColorpickerColourPickerChanged ------------
// --------------------------------------------------------
void ColorMapDialog::OnColorpickerColourPickerChanged( wxColourPickerEvent& event )
{
//	colorMapList->maps[currentMap].r[currentColor] = btnColorPicker->GetColour().Red();
//	colorMapList->maps[currentMap].g[currentColor] = btnColorPicker->GetColour().Green();
//	colorMapList->maps[currentMap].b[currentColor] = btnColorPicker->GetColour().Blue();

	colorMapList->UpdateRed(currentMap,currentColor,btnColorPicker->GetColour().Red());
	colorMapList->UpdateGreen(currentMap,currentColor,btnColorPicker->GetColour().Green());
	colorMapList->UpdateBlue(currentMap,currentColor,btnColorPicker->GetColour().Blue());

	/* update the canvas */
	previewCanvas->Draw();
	UpdateImageCanvas();
}


// --------------------------------------------------------
// ---------- OnSpinAlphaUpdated --------------------------
// --------------------------------------------------------
void ColorMapDialog::OnSpinAlphaUpdated( wxSpinEvent& event )
{
//	colorMapList->maps[currentMap].a[currentColor] = spinAlpha->GetValue();
	colorMapList->UpdateAlpha(currentMap,currentColor,spinAlpha->GetValue());

	/* update the canvas */
	previewCanvas->Draw();
	UpdateImageCanvas();
}


// --------------------------------------------------------
// ---------- UpdateSelectedColor -------------------------
// --------------------------------------------------------
void ColorMapDialog::UpdateSelectedColor()
{
	btnColorPicker->SetColour(wxColour(colorMapList->maps[currentMap].r[currentColor], colorMapList->maps[currentMap].g[currentColor], colorMapList->maps[currentMap].b[currentColor]));
	spinAlpha->SetValue(colorMapList->maps[currentMap].a[currentColor]);
	spinPosition->SetValue(colorMapList->maps[currentMap].p[currentColor]);
}


// --------------------------------------------------------
// ---------- OnSpinPositionUpdated -----------------------
// --------------------------------------------------------
void ColorMapDialog::OnSpinPositionUpdated( wxSpinEvent& event )
{
//	colorMapList->maps[currentMap].p[currentColor] = spinPosition->GetValue();
	colorMapList->UpdatePosition(currentMap,currentColor,spinPosition->GetValue());

	/* update the canvas */
	previewCanvas->Draw();
	UpdateImageCanvas();
}


// --------------------------------------------------------
// ---------- AddColorToMap -------------------------------
// --------------------------------------------------------
void ColorMapDialog::AddColorToMap(int r, int g, int b, int a, int p)
{
	colorMapList->AddColorToMap(currentMap,r,g,b,a,p);

	previewCanvas->Draw();
	UpdateImageCanvas();
}


/*!
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_COLORMAP_LIST
 */

void ColorMapDialog::OnColormapListSelected( wxCommandEvent& event )
{
	wxString name;

	int i = lstColorMaps->GetSelection();
	currentMap = i;

	txtColorMapName->SetValue(colorMapList->maps[i].name);
	/* update the canvas */
	previewCanvas->Draw();
}


/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_COLORMAPDIALOG
 */

void ColorMapDialog::OnCloseWindow( wxCloseEvent& event )
{
	if (!comboRepopulated) {
		frame->PopulateColorMapBox();
		comboRepopulated = true;
	}
	this->EndDialog(0);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void ColorMapDialog::OnDoneClick( wxCommandEvent& event )
{
	if (!comboRepopulated) {
		frame->PopulateColorMapBox();
		comboRepopulated = true;
	}
	this->EndDialog(0);
}


void ColorMapDialog::UpdateImageCanvas()
{
	frame->imgCanvas->Draw();
}