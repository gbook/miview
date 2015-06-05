/* ----------------------------------------------------------------------------
	ImageCanvas.h


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

/*	Owned by:
		MainFrame
	
	Owns:
		ImageVolume

	Basic functionality:
		*setup the OpenGL environment
		*handle mouse movements
		*pass some variables from MainFrame to ImageVolume and beyond
*/
//////////////////////////////////////////////////////////////////////


//#include "LogWindow.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class ImageCanvas: public wxGLCanvas
{
    friend class MainFrame;
public:
	/* constructors, destructors */
    ImageCanvas(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = _T("ImageCanvas"), int *glAttributes = NULL);
    ImageCanvas(wxWindow *parent, const ImageCanvas *other, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = _T("ImageCanvas"), int *glAttributes = NULL);
    ~ImageCanvas();

	/* public functions */
	void CloseCurrentFile();
	void Draw();
	int LoadFile();
	void ShowFileInfo();
	int GetMaxTextureSize();

	/* GETs and returning information */
	int GetStartSlice();
	int GetEndSlice();
	bool IsCurrentVolumeMultiFrame();
	bool IsReady();

	/* SETs and setting information */
	void SetCurrentSlice(int slice);
	void SetFrame(MainFrame &Frame);
	void SetObjectPointers(MainFrame *Frame, MainFrameVariables *MainVars);
    void InitGL(int mode);
	bool SaveScreenGrab(int filetype, wxString filepath);

	/* event handlers */
    void OnEraseBackground(wxEraseEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnKeyUp(wxKeyEvent& event);
	void OnLeftMouseDoubleClick(wxMouseEvent &event);
	void OnLeftMouseUp(wxMouseEvent &event);
	void OnLeftMouseDown(wxMouseEvent &event);
	void OnMouseWheel(wxMouseEvent &event);
	void OnMouseMove(wxMouseEvent &event);
    void OnPaint(wxPaintEvent& event);
	void OnRightMouseUp(wxMouseEvent &event);
	void OnRightMouseDown(wxMouseEvent &event);
    void OnSize(wxSizeEvent& event);

private:
	/* private functions */
    void Render();
	void InitializeVariables();
	void Get3ViewSlices(wxPoint coords);
	int GetQuadrantClicked(wxPoint p);
	wxString ReadWholeFile(wxString filepath);
	int SetupShader(wxString vertexFile, wxString fragmentFile);
	void trackMotion(int x, int y);
	void startMotion(int x, int y, int time);
	void stopMotion(int x, int y, int time);

	/* private variables */
	bool m_init;
	bool rightMouseDown;
	bool leftMouseDown;
	bool imgVolumeReady;
	int sliceStart, sliceEnd, slice;
	wxSize canvasSize;
	wxString msg;

	MainFrame *frame;
	//LogWindow *logWindow;
	MainFrameVariables *mainVars;

	GLuint pWindow;			/* shader program for window levels, and color mapping */
	GLuint pRayTrace;		/* shader program for ray tracing in the volume rendering */
	GLuint pRayTraceSSD;	/* shader program for ray tracing SSD in the volume rendering */

	/* simple trackball-like motion control */
	GLboolean trackingMotion;
	GLboolean trackingTranslation;
	float lastPos[3], transPosY;
	int lastTime;
	float angle, axis[3];
	float pAngle, pAxis[3];
	enum MoveModes { MoveNone, MoveView, MoveObject, MoveTexture };
	enum MoveModes mode;
	GLboolean redrawContinuously;
	float viewDistance;
	double user[4];
	bool gbPlane;
	bool gbPlaneMove;

	DECLARE_EVENT_TABLE()
};

