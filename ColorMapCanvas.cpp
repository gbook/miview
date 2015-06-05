/* ----------------------------------------------------------------------------
	ColorMapCanvas.cpp


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


#include "MainFrame.h"
#include "ColorMapCanvas.h"

#include "wx/dir.h"
#include "wx/timer.h"
#include <gl/glu.h>
#include <gl/glut.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <wx/msgdlg.h>
#include <wx/colordlg.h>

#include "ImageVolume.h"

/* to aid in discovery of memory leaks */
//#include <crtdbg.h>
//#define new new(_NORMAL_BLOCK,__FILE__, __LINE__)

BEGIN_EVENT_TABLE(ColorMapCanvas, wxGLCanvas)
    EVT_SIZE(ColorMapCanvas::OnSize)
    EVT_PAINT(ColorMapCanvas::OnPaint)
    EVT_ERASE_BACKGROUND(ColorMapCanvas::OnEraseBackground)
	EVT_LEFT_DOWN(ColorMapCanvas::OnLeftMouseDown)
	EVT_LEFT_DCLICK(ColorMapCanvas::OnLeftMouseDoubleClick)
	EVT_RIGHT_DOWN(ColorMapCanvas::OnRightMouseDown)
	EVT_MOTION(ColorMapCanvas::OnMouseMove)
END_EVENT_TABLE()


// --------------------------------------------------------
// ---------- ColorMapCanvas ---------------------------------
// --------------------------------------------------------
ColorMapCanvas::ColorMapCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name, int *glAttributes)
    : wxGLCanvas(parent, (wxGLCanvas*) NULL, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name, glAttributes)
{
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
	_CrtSetDbgFlag(flag); // Set flag to the new value

	//logWindow = NULL;
	InitializeVariables();
	WriteLog("ColorMapCanvas->ColorMapCanvas() 1 called...");
}


// --------------------------------------------------------
// ---------- ColorMapCanvas ---------------------------------
// --------------------------------------------------------
ColorMapCanvas::ColorMapCanvas(wxWindow *parent, const ColorMapCanvas *other, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name, int *glAttributes)
    : wxGLCanvas(parent, other->GetContext(), id, pos, size, style|wxFULL_REPAINT_ON_RESIZE , name, glAttributes)
{
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
	_CrtSetDbgFlag(flag); // Set flag to the new value

	//logWindow = NULL;
	InitializeVariables();
	WriteLog("ColorMapCanvas->ColorMapCanvas() 2 called...");
}

// --------------------------------------------------------
// ---------- ~ColorMapCanvas --------------------------------
// --------------------------------------------------------
ColorMapCanvas::~ColorMapCanvas()
{
}


// --------------------------------------------------------
// ---------- InitializeVariables -------------------------
// --------------------------------------------------------
void ColorMapCanvas::InitializeVariables()
{
	WriteLog("ColorMapCanvas->InitializeVariables() called...");
	m_init = false;
	//InitGL(0);
}


/* ============================================================================
	these functions may be called before the logwindow is setup, so they must contain WriteLog() calls
		Render()
		OnPaint()
		OnSize()
		OnEraseBackground()
   ============================================================================ */

// --------------------------------------------------------
// ---------- Render --------------------------------------
// --------------------------------------------------------
void ColorMapCanvas::Render()
{
//	WriteLog(_("ColorMapCanvas->Render() called..."));

    wxPaintDC dc(this);

    SetCurrent();
    /* Initialize OpenGL once, but only after SetCurrent */
    if (!m_init)
    {
        InitGL(GLMODE_FLAT);
        m_init = true;
    }

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Draw();

    glFlush();
    SwapBuffers();

//	WriteLog(_("ColorMapCanvas->Render() finished...\n"));
}


// --------------------------------------------------------
// ---------- OnPaint -------------------------------------
// --------------------------------------------------------
void ColorMapCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
//	WriteLog("ColorMapCanvas->OnPaint() called...");

//	if (logWindow != NULL)
//		WriteLog("ColorMapCanvas->OnPaint() called...\n");

	int w,h;

	/* determine the size the OpenGL Canvas should be */
	GetClientSize(&w, &h);
	canvasSize.SetWidth(w);
	canvasSize.SetHeight(h);

	SetCurrent(); /* set this as the current GLCanvas */

	/* change the OpenGL viewing area... dont change any of the OpenGL enable/disable */
	glViewport(0, 0, canvasSize.GetWidth(), canvasSize.GetHeight());
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (double)canvasSize.GetWidth(), 0.0, (double)canvasSize.GetHeight(), 1000.0, -1000.0);
	glMatrixMode (GL_MODELVIEW);

    Render();
}


// --------------------------------------------------------
// ---------- OnSize --------------------------------------
// --------------------------------------------------------
void ColorMapCanvas::OnSize(wxSizeEvent& event)
{
//	WriteLog("ColorMapCanvas->OnSize() called...");

	int w, h;

	GetClientSize(&w, &h);
	canvasSize.SetWidth(w);
	canvasSize.SetHeight(h);

	// this is also necessary to update the context on some platforms
    //wxGLCanvas::OnSize(event);
}


// --------------------------------------------------------
// ---------- OnEraseBackground ---------------------------
// --------------------------------------------------------
void ColorMapCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
  // Do nothing, to avoid flashing.
}

/* ============================================================================ */
/*                end functions requiring WriteLog()                          */
/* ============================================================================ */

// --------------------------------------------------------
// ---------- InitGL --------------------------------------
// --------------------------------------------------------
void ColorMapCanvas::InitGL(int mode)
{
//	WriteLog("ColorMapCanvas->InitGL() called...\n");
	WriteLog("ColorMapCanvas->InitGL() called...\n");

	wxString msg;
	int width, height;

    SetCurrent();

    /* set available canvas size */
	GetClientSize(&width, &height);
	canvasSize.SetWidth(width);
	canvasSize.SetHeight(height);

	glClearColor(0.0, 0.0, 0.0, 0.0);

	glViewport(0, 0, canvasSize.GetWidth(), canvasSize.GetHeight());
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (double)canvasSize.GetWidth(), 0.0, (double)canvasSize.GetHeight(), 1000.0, -1000.0);
	glMatrixMode (GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_init = true;

	Render();

	WriteLog("ColorMapCanvas->InitGL() finished...\n");
}


// --------------------------------------------------------
// ---------- Draw ----------------------------------------
// --------------------------------------------------------
void ColorMapCanvas::Draw()
{
	SetCurrent(); /* set this as the current GLCanvas */

	int col, row;
	int numCols, numRows;
	int i;
	int count;
	int selectedColor = -1;
	int currMap = mapDialog->currentMap;
	double w,h;
	double rowSize,colSize;
	double x1,x2,y1,y2;
	float r,g,b,a,p;

	glEnable(GL_BLEND);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* assign variables */
	w = canvasSize.GetWidth();
	h = canvasSize.GetHeight();
	numRows = 20;
	rowSize = h/numRows;
	numCols = w/rowSize;
	colSize = w/numCols;
	selectedColor = mapDialog->currentColor;

	/* draw checkerboard covering entire canvas */
	glBegin(GL_QUADS);
	for (row=0;row<numRows;row++) {
		for (col=0;col<numCols;col++) {
			if (row%2 == 0) { /* even row */
				if (col%2 == 0) glColor3f(0.5,0.5,0.5); /* even column */
				else glColor3f(1.0,1.0,1.0); /* odd column */
			}
			else { /* odd row */
				if (col%2 == 0) glColor3f(1.0,1.0,1.0); /* even column */
				else glColor3f(0.5,0.5,0.5); /* odd column */
			}
			x1 = ((double)col/(double)numCols)*w;
			x2 = (((double)col+1.0)/(double)numCols)*w;
			y1 = ((double)row/(double)numRows)*h;
			y2 = (((double)row+1.0)/(double)numRows)*h;
			/* draw the square */
			glVertex2f(x1, y1);
			glVertex2f(x1, y2);
			glVertex2f(x2, y2);
			glVertex2f(x2, y1);
		}
	}
	glEnd();

	/* draw the grayscale colorbar in the bottom half */
	glBegin(GL_QUADS);
		glColor3f(0.0,0.0,0.0); /* black on the left */
		glVertex2f(0.0, 0.0);
		glVertex2f(0.0, h/2.0);
		glColor3f(1.0,1.0,1.0); /* white on the right */
		glVertex2f(w, h/2.0);
		glVertex2f(w, 0.0);
	glEnd();

	/* draw the color map */
	count = colorMapList->maps[mapDialog->currentMap].numColors;
	glBegin(GL_QUADS);
	for (i=0;i<count-1;i++) {
		/* current color */
		r = (float)colorMapList->maps[currMap].r[i];
		g = (float)colorMapList->maps[currMap].g[i];
		b = (float)colorMapList->maps[currMap].b[i];
		a = (float)colorMapList->maps[currMap].a[i];
		p = (float)colorMapList->maps[currMap].p[i]; /* range [0 1000] */
		glColor4f(r/255.0, g/255.0, b/255.0, a/255.0);
		glVertex2f((p/1000.0)*w, h/2.0);
		glVertex2f((p/1000.0)*w, h);
		
		/* next color */
		r = (float)colorMapList->maps[currMap].r[i+1];
		g = (float)colorMapList->maps[currMap].g[i+1];
		b = (float)colorMapList->maps[currMap].b[i+1];
		a = (float)colorMapList->maps[currMap].a[i+1];
		p = (float)colorMapList->maps[currMap].p[i+1]; /* range [0 1000] */
		glColor4f(r/255.0, g/255.0, b/255.0, a/255.0);
		glVertex2f((p/1000.0)*w, h);
		glVertex2f((p/1000.0)*w, h/2.0);
	}
	glEnd();

	/* draw a handle to move the position of the colors */
	for (i=0;i<count;i++) {
		p = (float)colorMapList->maps[currMap].p[i]; /* range [0 1000] */
		/* black underlay */
		glColor4f(0.0, 0.0, 0.0, 1.0);
		glBegin(GL_QUADS);
			/* vertical line */
			glVertex2f((p/1000.0)*w+1.5, h);
			glVertex2f((p/1000.0)*w+1.5, 0.0);
			glVertex2f((p/1000.0)*w-1.5, 0.0);
			glVertex2f((p/1000.0)*w-1.5, h);
			/* bottom box */
			glVertex2f((p/1000.0)*w+6, (h/4.0)+1);
			glVertex2f((p/1000.0)*w+6, 0.0);
			glVertex2f((p/1000.0)*w-6, 0.0);
			glVertex2f((p/1000.0)*w-6, (h/4.0)+1);
		glEnd();

		/* yellow overlay */
		if (currentColor == i)
			glColor4f(1.0, 1.0, 1.0, 1.0);
		else
			glColor4f(1.0, 1.0, 0.3, 1.0);
		glBegin(GL_QUADS);
			/* vertical line */
			glVertex2f((p/1000.0)*w+0.5, h);
			glVertex2f((p/1000.0)*w+0.5, 0.0);
			glVertex2f((p/1000.0)*w-0.5, 0.0);
			glVertex2f((p/1000.0)*w-0.5, h);
			/* bottom box */
			glVertex2f((p/1000.0)*w+5, h/4.0);
			glVertex2f((p/1000.0)*w+5, 0.0);
			glVertex2f((p/1000.0)*w-5, 0.0);
			glVertex2f((p/1000.0)*w-5, h/4.0);
		glEnd();
	}

	glFlush();
	SwapBuffers();
}


/* ========== SetObjectPointers =========================== */
void ColorMapCanvas::SetObjectPointers(MainFrame *Frame, MainFrameVariables *MainVars, ColorMapDialog *mapdialog, ColorMapList *mapList)
{
	frame = Frame;
	mainVars = MainVars;
	mapDialog = mapdialog;
	colorMapList = mapList;
}


// --------------------------------------------------------
// ---------- OnLeftMouseDown -----------------------------
// --------------------------------------------------------
void ColorMapCanvas::OnLeftMouseDown(wxMouseEvent &event)
{
	wxPoint coords = event.GetPosition();
	double x = (double)coords.x;
	//double y = (double)coords.y;
	double w = canvasSize.GetWidth();
	int count;
	int i;

	int p = (x/w)*1000.0; /* mouse position */
	int pp; /* original color position */

	int currMap = mapDialog->currentMap;

	currentColor = -1;

	/* loop through and find a color thats at this position */
	count = colorMapList->maps[mapDialog->currentMap].numColors;
	for (i=0;i<count;i++) {
		pp = colorMapList->maps[currMap].p[i];

		if ((p < pp+8) && (p > pp-8)) {
			currentColor = i;
			mapDialog->currentColor = currentColor;
			mapDialog->UpdateSelectedColor();
//			mapDialog->UpdateImageCanvas();
			break;
		}
	}
	Draw();
}


// --------------------------------------------------------
// ---------- OnLeftMouseDoubleClick ----------------------
// --------------------------------------------------------
void ColorMapCanvas::OnLeftMouseDoubleClick(wxMouseEvent &event)
{
	wxPoint coords = event.GetPosition();
	double x = (double)coords.x;
	double y = (double)coords.y;
	double w = canvasSize.GetWidth();
	wxColour newColor;
	int count;
	int i;

	int p = (x/w)*1000.0; /* mouse position */
	int pp; /* original color position */

	int currMap = mapDialog->currentMap;

	currentColor = -1;

	/* loop through and find a color thats at this position */
	count = colorMapList->maps[mapDialog->currentMap].numColors;
	for (i=0;i<count;i++) {
		pp = colorMapList->maps[currMap].p[i];

		if ((p < pp+8) && (p > pp-8)) {
			currentColor = i;
			mapDialog->currentColor = currentColor;
			break;
		}
	}

	if (currentColor >= 0) {
		wxColourDialog *colorDialog = new wxColourDialog(this);

		if (colorDialog->ShowModal() == wxID_OK) {
			newColor = colorDialog->GetColourData().GetColour();
//			colorMapList->maps[currMap].r[currentColor] = newColor.Red();
//			colorMapList->maps[currMap].g[currentColor] = newColor.Green();
//			colorMapList->maps[currMap].b[currentColor] = newColor.Blue();
			
			colorMapList->UpdateRed(currMap,currentColor,newColor.Red());
			colorMapList->UpdateGreen(currMap,currentColor,newColor.Green());
			colorMapList->UpdateBlue(currMap,currentColor,newColor.Blue());

			//colorMapList->maps[currMap].GenerateGLTexture();
			//colorMapList->maps[currMap].GenerateGradientBitmap();
			mapDialog->UpdateSelectedColor();
			Draw();
		}
	}
}


// --------------------------------------------------------
// ---------- OnMouseMove ---------------------------------
// --------------------------------------------------------
void ColorMapCanvas::OnMouseMove(wxMouseEvent &event)
{
	if ((event.LeftIsDown()) && (currentColor >= 0)) {
		wxPoint coords = event.GetPosition();
		double x = (double)coords.x;
		//double y = (double)coords.y;
		double w = canvasSize.GetWidth();
		int currMap = mapDialog->currentMap;

		int p = (x/w)*1000.0;

		//colorMapList->maps[currMap].p[currentColor] = p;
		colorMapList->UpdatePosition(currMap,currentColor,p);
		mapDialog->UpdateSelectedColor();
		mapDialog->UpdateImageCanvas();
		Draw();
	}
}


// --------------------------------------------------------
// ---------- OnRightMouseDown ----------------------------
// --------------------------------------------------------
void ColorMapCanvas::OnRightMouseDown(wxMouseEvent &event)
{
	wxPoint coords = event.GetPosition();
	double x = (double)coords.x;
	double y = (double)coords.y;
	double w = canvasSize.GetWidth();

	int p = (x/w)*1000.0; /* mouse position */

	/* create a new color at this position */
	currentColor = colorMapList->AddColorToMap(mapDialog->currentMap,255,255,255,255,p);
	mapDialog->UpdateImageCanvas();

	Draw();
}