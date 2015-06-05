/* ----------------------------------------------------------------------------
	ColorMapCanvas.h


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


//////////////////////////////////////////////////////////////////////

#include "MainFrame.h"
#include "ColorMapDialog.h"
//#include "LogWindow.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ColorMapCanvas: public wxGLCanvas
{
    friend class MainFrame;
public:
	/* constructors, destructors */
    ColorMapCanvas(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = _T("ColorMapCanvas"), int *glAttributes = NULL);
    ColorMapCanvas(wxWindow *parent, const ColorMapCanvas *other, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = _T("ColorMapCanvas"), int *glAttributes = NULL);
    ~ColorMapCanvas();

	/* public functions */
	void Draw();

	/* GETs and returning information */
	bool IsReady();

	/* SETs and setting information */
	void SetObjectPointers(MainFrame *Frame, MainFrameVariables *MainVars, ColorMapDialog *mapdialog, ColorMapList *mapList);
    void InitGL(int mode);

	/* event handlers */
    void OnEraseBackground(wxEraseEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
	void OnLeftMouseDown(wxMouseEvent &event);
	void OnLeftMouseDoubleClick(wxMouseEvent &event);
	void OnRightMouseDown(wxMouseEvent &event);
	void OnMouseMove(wxMouseEvent &event);

private:
	/* private functions */
    void Render();
	void InitializeVariables();

	/* private variables */
	bool m_init;
	wxSize canvasSize;
	wxString msg;

	int currentColor;

	MainFrame *frame;
//	LogWindow *logWindow;
	MainFrameVariables *mainVars;
	ColorMapDialog *mapDialog;
	ColorMapList *colorMapList;

	DECLARE_EVENT_TABLE()
};

