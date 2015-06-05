/* ----------------------------------------------------------------------------
	ImageCanvas.cpp


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
#include "ImageCanvas.h"

#include "wx/dir.h"
#include "wx/timer.h"
#include <gl/glu.h>
#include <gl/glut.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <wx/msgdlg.h>

#include "ImageVolume.h"

/* to aid in discovery of memory leaks */
//#include <crtdbg.h>
//#define new new(_NORMAL_BLOCK,__FILE__, __LINE__)

ImageVolume *imgVol;

BEGIN_EVENT_TABLE(ImageCanvas, wxGLCanvas)
    EVT_SIZE(ImageCanvas::OnSize)
    EVT_PAINT(ImageCanvas::OnPaint)
    EVT_ERASE_BACKGROUND(ImageCanvas::OnEraseBackground)
    EVT_KEY_DOWN(ImageCanvas::OnKeyDown)
    EVT_KEY_UP(ImageCanvas::OnKeyUp)
	EVT_LEFT_DOWN(ImageCanvas::OnLeftMouseDown)
	EVT_LEFT_UP(ImageCanvas::OnLeftMouseUp)
	EVT_LEFT_DCLICK(ImageCanvas::OnLeftMouseDoubleClick)
	EVT_RIGHT_DOWN(ImageCanvas::OnRightMouseDown)
	EVT_RIGHT_UP(ImageCanvas::OnRightMouseUp)
	EVT_MOTION(ImageCanvas::OnMouseMove)
	EVT_MOUSEWHEEL(ImageCanvas::OnMouseWheel)
END_EVENT_TABLE()


// --------------------------------------------------------
// ---------- ImageCanvas ---------------------------------
// --------------------------------------------------------
ImageCanvas::ImageCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name, int *glAttributes)
    : wxGLCanvas(parent, (wxGLCanvas*) NULL, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name, glAttributes)
{
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
	_CrtSetDbgFlag(flag); // Set flag to the new value

	//logWindow = NULL;
	InitializeVariables();
	WriteLog("ImageCanvas->ImageCanvas() 1 called...");
	
	//GLenum err = glewInit();
	//if (GLEW_OK != err) {
	//	char buffer[50];
	//	sprintf(buffer, "GlewInit() failed: %s", glewGetErrorString(err));
	//	WriteLog(buffer);
	//}
}


// --------------------------------------------------------
// ---------- ImageCanvas ---------------------------------
// --------------------------------------------------------
ImageCanvas::ImageCanvas(wxWindow *parent, const ImageCanvas *other, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name, int *glAttributes)
    : wxGLCanvas(parent, other->GetContext(), id, pos, size, style|wxFULL_REPAINT_ON_RESIZE , name, glAttributes)
{
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
	_CrtSetDbgFlag(flag); // Set flag to the new value

	//logWindow = NULL;
	InitializeVariables();
	WriteLog("ImageCanvas->ImageCanvas() 2 called...");

	//GLenum err = glewInit();
	//if (GLEW_OK != err) {
	//	char buffer[50];
	//	sprintf(buffer, "GlewInit() failed: %s", glewGetErrorString(err));
	//	WriteLog(buffer);
	//}
}

// --------------------------------------------------------
// ---------- ~ImageCanvas --------------------------------
// --------------------------------------------------------
ImageCanvas::~ImageCanvas()
{
	imgVolumeReady = false;

	if (imgVol != NULL)
		delete imgVol;
}


// --------------------------------------------------------
// ---------- InitializeVariables -------------------------
// --------------------------------------------------------
void ImageCanvas::InitializeVariables()
{
	WriteLog("ImageCanvas->InitializeVariables() called...");
	imgVol = NULL;
	m_init = false;
	rightMouseDown = false;
	imgVolumeReady = false;

	/* simple trackball-like motion control */
	trackingMotion = GL_FALSE;
	trackingTranslation = GL_FALSE;
	angle = 0.0f;
	axis[0] = 1.0f; axis[1] = 0.0f; axis[2] = 0.0f;
	pAngle = 0.0f;
	pAxis[0] = 1.0f; pAxis[1] = 0.0f; pAxis[2] = 0.0f;
	mode = MoveNone;
	viewDistance = -5.0f;
	user[0] = 1.0; user[1] = 0.0; user[2] = 0.0; user[3] = 1.0;
	gbPlane = false;
	gbPlaneMove = false;

}


// --------------------------------------------------------
// ---------- Render --------------------------------------
// --------------------------------------------------------
void ImageCanvas::Render()
{
	WriteLog("ImageCanvas->Render() called...");

    wxPaintDC dc(this);

    SetCurrent();
    /* Initialize OpenGL once, but only after SetCurrent */
    if (!m_init)
    {
        InitGL(GLMODE_FLAT);
        m_init = true;
    }

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	WriteLog(msg.Format("imgVol->Draw is about to be called (W%f,H%f)", canvasSize.GetWidth(), canvasSize.GetHeight()));

	if (imgVol != NULL)
		imgVol->Draw();

    glFlush();
    SwapBuffers();

	WriteLog("ImageCanvas->Render() finished...");
}


// --------------------------------------------------------
// ---------- OnPaint -------------------------------------
// --------------------------------------------------------
void ImageCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
	WriteLog("ImageCanvas->OnPaint() called...");

	int w,h;

	/* determine the size the OpenGL Canvas should be */
	GetClientSize(&w, &h);
	canvasSize.SetWidth(w);
	canvasSize.SetHeight(h);

	if ((imgVol != NULL) && (imgVolumeReady))
		imgVol->SetCanvasSize(canvasSize);

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
void ImageCanvas::OnSize(wxSizeEvent& event)
{
	WriteLog("ImageCanvas->OnSize() called...");

	int w, h;

	GetClientSize(&w, &h);
	canvasSize.SetWidth(w);
	canvasSize.SetHeight(h);

	if ((imgVol != NULL) && (imgVolumeReady))
		imgVol->SetCanvasSize(canvasSize);

	// this is also necessary to update the context on some platforms
    //wxGLCanvas::OnSize(event);
}


// --------------------------------------------------------
// ---------- OnEraseBackground ---------------------------
// --------------------------------------------------------
void ImageCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
  // Do nothing, to avoid flashing.
}


/* ========== IsReady ===================================== */
bool ImageCanvas::IsReady() {
	if ((imgVol != NULL) && (imgVolumeReady)) return 1;
	else return 0;
}

/* ========== IsCurrentVolumeMultiFrame =================== */
bool ImageCanvas::IsCurrentVolumeMultiFrame() {
	if (imgVol != NULL) return imgVol->IsMultiFrame();
	else return false;
}

/* ========== SetObjectPointers =========================== */
void ImageCanvas::SetObjectPointers(MainFrame *Frame, MainFrameVariables *MainVars) { frame = Frame; mainVars = MainVars; }

/* ========== GetStartSlice ============================= */
int ImageCanvas::GetStartSlice() {
	if (imgVol != NULL) {
		if (imgVol->IsReady()) return imgVol->GetStartSlice();
		else return 0;
	}
	else return 0;
}

/* ========== GetEndSlice ============================= */
int ImageCanvas::GetEndSlice() {
	if (imgVol != NULL) {
		if (imgVol->IsReady()) return imgVol->GetEndSlice();
		else return 0;
	}
	else return 0;
}

// --------------------------------------------------------
// ---------- GetMaxTextureSize ---------------------------
// --------------------------------------------------------
int ImageCanvas::GetMaxTextureSize()
{
	SetCurrent();
	int texSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
	return texSize;
}


// --------------------------------------------------------
// ---------- InitGL --------------------------------------
// --------------------------------------------------------
void ImageCanvas::InitGL(int mode)
{
	WriteLog("ImageCanvas->InitGL() called...");

	wxString msg;
	int width, height;
	wxMessageDialog *dlg;

    SetCurrent();
	
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		char buffer[50];
		sprintf(buffer, "GlewInit() failed: %s", glewGetErrorString(err));
		WriteLog(buffer);
	}

	if (!GL_ARB_fragment_shader) {
		dlg = new wxMessageDialog(this, "GL_ARB_fragment_shader unavailable. Your video drivers do not support OpenGL 2.0, please update your drivers.","MIView",wxOK);
	}

	/* setup the fragment and/or vertex shaders */
	pWindow = SetupShader("Default.vert","WindowLevel.frag");
	if (!pWindow) WriteLog("Window level shader program failed to link...");
	else WriteLog("Window level shader program linked successfully...");

	pRayTrace = SetupShader("RayTrace.vert","RayTrace.frag");
	if (!pWindow) WriteLog("Ray trace shader program failed to link...");
	else WriteLog("Ray trace shader program linked successfully...");

	pRayTraceSSD = SetupShader("RayTrace.vert","RayTraceSSD.frag");
	if (!pWindow) WriteLog("Ray trace SSD shader program failed to link...");
	else WriteLog("Ray trace SSD shader program linked successfully...");

    /* set available canvas size */
	GetClientSize(&width, &height);
	canvasSize.SetWidth(width);
	canvasSize.SetHeight(height);

	if (imgVol != NULL)
		imgVol->SetCanvasSize(canvasSize);

	WriteLog("ImageCanvas->InitGL() finished...");
}


// --------------------------------------------------------
// ---------- SetupShader ---------------------------------
// --------------------------------------------------------
int ImageCanvas::SetupShader(wxString vertexFile, wxString fragmentFile)
{
	int v; /* vertex shader */
	int f; /* fragment shader index */
	GLuint p;
	GLint vertCompiled, fragCompiled, progLinked;
	const char *fstr; /* fragment shader string */
	const char *vstr; /* vertex shader string */
	wxString fragStr, vertStr;

	v = glCreateShader(GL_VERTEX_SHADER);	
	f = glCreateShader(GL_FRAGMENT_SHADER);	
	
	/* load the code into the fragment shader */
	fragStr = ReadWholeFile(fragmentFile); /* fragment shader */
	fstr = (const char *)fragStr.c_str();
	glShaderSource(f, 1, &fstr,NULL); /* specify the fragment shader code */

	/* load the code into the vertex shader */
	vertStr = ReadWholeFile(vertexFile); /* vertex shader */
	vstr = (const char *)vertStr.c_str();
	glShaderSource(v, 1, &vstr,NULL); /* specify the vertex shader code */

	/* compile and text the vertex shader */
	glCompileShader(v);
	glGetShaderiv(v, GL_COMPILE_STATUS, &vertCompiled);
	if (!vertCompiled) {
		WriteLog("vertex shader failed to compile...");
		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;
		glGetShaderiv(f, GL_INFO_LOG_LENGTH,&infologLength);
		if (infologLength > 0) {
			infoLog = new char [infologLength];
			glGetShaderInfoLog(f, infologLength, &charsWritten, infoLog);
			WriteLog(msg.Format("%s\n",infoLog));
			delete[] infoLog;
		}
	}
	else
		WriteLog("vertex shader compiled successfully...");

	/* compile and text the fragment shader */
	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &fragCompiled);
	if (!fragCompiled) {
		WriteLog("fragment shader failed to compile...");
		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;
		glGetShaderiv(f, GL_INFO_LOG_LENGTH,&infologLength);
		if (infologLength > 0) {
			infoLog = new char [infologLength];
			glGetShaderInfoLog(f, infologLength, &charsWritten, infoLog);
			WriteLog(msg.Format("%s",infoLog));
			delete[] infoLog;
		}
	}
	else
		WriteLog("fragment shader compiled successfully...");

	/* create the program and attach the shader to it */
	p = glCreateProgram();
	glAttachShader(p,v);
	glAttachShader(p,f);

	/* link and test the program */
	glLinkProgram(p);
	glGetProgramiv(p, GL_LINK_STATUS, &progLinked);
	if (!progLinked)
		return 0;

	return p;
}


// --------------------------------------------------------
// ---------- OnKeyDown -----------------------------------
// --------------------------------------------------------
void ImageCanvas::OnKeyDown( wxKeyEvent& event )
{
    long evkey = event.GetKeyCode();
    if (evkey == 0) return;

    event.Skip();
}


// --------------------------------------------------------
// ---------- OnKeyUp -------------------------------------
// --------------------------------------------------------
void ImageCanvas::OnKeyUp( wxKeyEvent& event )
{
    event.Skip();
}


// --------------------------------------------------------
// ---------- OnRightMouseDown ----------------------------
// --------------------------------------------------------
void ImageCanvas::OnRightMouseDown(wxMouseEvent &event)
{
	rightMouseDown = true;
	if (mainVars->Image_trackPoints()) {
		int numPoints = (int)frame->displayPoints->thePoints.GetCount();
		if (numPoints > 0)
			frame->displayPoints->thePoints.RemoveAt(numPoints-1);
	}
}


// --------------------------------------------------------
// ---------- OnRightMouseUp ------------------------------
// --------------------------------------------------------
void ImageCanvas::OnRightMouseUp(wxMouseEvent &event)
{
	rightMouseDown = false;
}


// --------------------------------------------------------
// ---------- OnMouseMove ---------------------------------
// --------------------------------------------------------
void ImageCanvas::OnMouseMove(wxMouseEvent &event)
{
	/* right button adjusts the window center and width */
	if ((event.RightIsDown()) && (imgVol != NULL)) {
		wxPoint coords = event.GetPosition();
		double x = (double)coords.x;
		double y = (double)coords.y;
		double height = canvasSize.GetHeight();
		double width = canvasSize.GetWidth();
		int newWidth, newCenter;
		double maxVal, minVal;

		if (imgVol->imgData->imgInfo->GetPixelType() == IMG_RGB24) {
			maxVal = 255;
			minVal = 0;
		}
		else {
			maxVal = (double)imgVol->imgData->imgInfo->GetMaxPixelValues().Gray();
			minVal = (double)imgVol->imgData->imgInfo->GetMinPixelValues().Gray();
		}

		newCenter = (int)(((y*(maxVal-minVal))/height) + minVal);
		newWidth = (int)(((x*(maxVal-minVal))/width) + minVal);
		mainVars->Display_windowCenter(newCenter);
		mainVars->Display_windowWidth(newWidth);
		imgVol->SetWindowLevel(newCenter, newWidth);

		Draw();
	}

	/* left button moves the image around the screen */
	if ((event.LeftIsDown()) && (imgVol != NULL)) {
		wxPoint coords = event.GetPosition();
		int x;
		int y;

		WriteLog(msg.Format("Left Mouse is moving at (%d,%d)...", coords.x, coords.y));

		switch (mainVars->Toolbar_displayType()) {
			case IMGVIEW_3VIEW:
				break;
			case IMGVIEW_DVR:
				//WriteLog("trackMotion() should be called next\n");
				coords.y = canvasSize.GetHeight() - coords.y;
				trackMotion(coords.x, coords.y);
				/* set rotation params... */
				if (imgVol != NULL)
					if (imgVol->IsReady()) {
						WriteLog(msg.Format("Rotation (%.4f,%.4f,%.4f,%.4f)",angle, axis[0],axis[1],axis[2]));
						mainVars->Rotations_angle((double)angle);
						mainVars->Rotations_X((double)axis[0]);
						mainVars->Rotations_Y((double)axis[1]);
						mainVars->Rotations_Z((double)axis[2]);
						WriteLog(msg.Format("Setting 3D Rotations: (%.4f;%.4f,%.4d,%.4d)...", angle, axis[0], axis[1], axis[2]));
						Draw();
					}
				break;
			case IMGVIEW_MULTISLICE:
				break;
			case IMGVIEW_SINGLEPLANE:
				y = (int)(canvasSize.GetHeight()/2) - coords.y;
				x = coords.x - (int)(canvasSize.GetWidth()/2);
				
				imgVol->SetCenterOffset(wxPoint::wxPoint(x,y));
				
				/* determine which slice was clicked, and where */
				/* always flip the mouse's Y coords */
				coords.y = canvasSize.GetHeight() - coords.y;
				Draw();
				break;
		}
	}
}


// --------------------------------------------------------
// ---------- OnMouseWheel --------------------------------
// --------------------------------------------------------
void ImageCanvas::OnMouseWheel(wxMouseEvent &event)
{

}

// --------------------------------------------------------
// ---------- OnLeftMouseDown -----------------------------
// --------------------------------------------------------
void ImageCanvas::OnLeftMouseDown(wxMouseEvent &event)
{
	if (imgVolumeReady) {
		int plane;
		double zoom;
		//wxRect plane0, plane1, plane2;
		wxRect drawnImage;
		wxSize size;
		wxPoint bottomLeft;
		wxPoint relativeImageClick;
		wxPoint origImageClick;
		leftMouseDown = true;
		wxPoint coords = event.GetPosition();
		
		/* always flip the mouse's Y coords */
		coords.y = canvasSize.GetHeight() - coords.y;

		WriteLog(msg.Format("Left Mouse button pressed at (%d,%d)...", coords.x, coords.y));
		switch (mainVars->Toolbar_displayType()) {
			case IMGVIEW_SINGLEPLANE:
				if (mainVars->Image_trackPoints()) {
					/* determine where on the image it was clicked... in image coordinates */
					plane = mainVars->Image_plane();
					bottomLeft = imgVol->drawnPlaneInfo[plane].minmin;
					size = wxSize::wxSize(imgVol->drawnPlaneInfo[plane].maxmax.x - imgVol->drawnPlaneInfo[plane].minmin.x, imgVol->drawnPlaneInfo[plane].maxmax.y - imgVol->drawnPlaneInfo[plane].minmin.y);
					drawnImage = wxRect::wxRect(bottomLeft,size);
					if (drawnImage.Contains(coords)) {
						zoom = (double)imgVol->drawnPlaneInfo[0].zoom/100.0;
						relativeImageClick.x = coords.x - bottomLeft.x;
						relativeImageClick.y = coords.y - bottomLeft.y;
						/* determine coordinates in image where clicked */
						origImageClick.x = (int)((double)relativeImageClick.x/zoom);
						origImageClick.y = (int)((double)relativeImageClick.y/zoom);
						WriteLog(msg.Format("Left Mouse button pressed in plane %d (%d,%d)...", plane, origImageClick.x, origImageClick.y));
						frame->displayPoints->thePoints.Add(origImageClick);
					}
				}
				break;
			case IMGVIEW_3VIEW:
				/* determine which slice was clicked, and where */
				Get3ViewSlices(coords);
				Draw();
				break;
			case IMGVIEW_DVR:
				/* flip the Y coord again... this is done by default up above */
				//coords.y = canvasSize.GetHeight() - coords.y;
				startMotion(coords.x, coords.y, glutGet(GLUT_ELAPSED_TIME));
				break;
			case IMGVIEW_MULTISLICE:
				break;
		}
	}
}


// --------------------------------------------------------
// ---------- GetQuadrantClicked --------------------------
// --------------------------------------------------------
int ImageCanvas::GetQuadrantClicked(wxPoint p)
{
	int quad = 0;
	wxPoint bottomLeft0, bottomLeft1, bottomLeft2, bottomLeft3;
	wxSize size;
	wxRect *quad0, *quad1, *quad2, *quad3;
	int width = canvasSize.GetWidth();
	int height = canvasSize.GetHeight();
	size.Set((int)(width/2), (int)(height/2));

	/* upper left */
	quad2 = new wxRect(0, canvasSize.GetHeight()/2, canvasSize.GetWidth()/2, canvasSize.GetHeight()/2);
	/* upper right */
	quad1 = new wxRect(canvasSize.GetWidth()/2, canvasSize.GetHeight()/2, canvasSize.GetWidth()/2, canvasSize.GetHeight()/2);
	/* lower left */
	quad0 = new wxRect(0, 0, canvasSize.GetWidth()/2, canvasSize.GetHeight()/2);
	/* lower right */
	quad3 = new wxRect(canvasSize.GetWidth()/2, 0, canvasSize.GetWidth()/2, canvasSize.GetHeight()/2);

	if (quad0->Contains(p))
		quad = 0;
	if (quad1->Contains(p))
		quad = 1;
	if (quad2->Contains(p))
		quad = 2;
	if (quad3->Contains(p))
		quad = 3;

	delete quad2;
	delete quad1;
	delete quad0;
	delete quad3;

	return quad;
}


// --------------------------------------------------------
// ---------- Get3ViewSlices ------------------------------
// --------------------------------------------------------
void ImageCanvas::Get3ViewSlices(wxPoint coords) {
	int quad;
	double zoom;
	wxRect plane0, plane1, plane2;
	wxSize size0, size1, size2;
	wxPoint bottomLeft0, bottomLeft1, bottomLeft2;
	wxPoint relativeImageClick;

	/* determine which quadrant is selected, and then where in the view it was clicked */
	quad = GetQuadrantClicked(coords);
	if (quad == 0)
		mainVars->Slice_value(0);
	if (quad == 1)
		mainVars->Slice_value(1);
	if (quad == 2)
		mainVars->Slice_value(2);
	if (quad == 3)
		mainVars->Slice_value(2);

	bottomLeft0 = imgVol->drawnPlaneInfo[0].minmin;
	size0 = wxSize::wxSize(imgVol->drawnPlaneInfo[0].maxmax.x - imgVol->drawnPlaneInfo[0].minmin.x, imgVol->drawnPlaneInfo[0].maxmax.y - imgVol->drawnPlaneInfo[0].minmin.y);
	plane0 = wxRect::wxRect(bottomLeft0,size0);

	bottomLeft1 = imgVol->drawnPlaneInfo[1].minmin;
	size1 = wxSize::wxSize(imgVol->drawnPlaneInfo[1].maxmax.x - imgVol->drawnPlaneInfo[1].minmin.x, imgVol->drawnPlaneInfo[1].maxmax.y - imgVol->drawnPlaneInfo[1].minmin.y);
	plane1 = wxRect::wxRect(bottomLeft1,size1);

	bottomLeft2 = imgVol->drawnPlaneInfo[2].minmin;
	size2 = wxSize::wxSize(imgVol->drawnPlaneInfo[2].maxmax.x - imgVol->drawnPlaneInfo[2].minmin.x, imgVol->drawnPlaneInfo[2].maxmax.y - imgVol->drawnPlaneInfo[2].minmin.y);
	plane2 = wxRect::wxRect(bottomLeft2,size2);

	zoom = (double)imgVol->drawnPlaneInfo[0].zoom/100.0;
	if (plane0.Contains(coords)) {
		/* determine coordinates in image where clicked */
		relativeImageClick.x = coords.x - bottomLeft0.x;
		relativeImageClick.y = coords.y - bottomLeft0.y;
		imgVol->drawnPlaneInfo[1].sliceNum = (int)((double)relativeImageClick.x/zoom);
		imgVol->drawnPlaneInfo[2].sliceNum = (int)((double)relativeImageClick.y/zoom);
		WriteLog(msg.Format("Left Mouse button pressed in plane 0 (%d,%d)...", relativeImageClick.x, relativeImageClick.y));
	}
	if (plane1.Contains(coords)) {
		/* determine coordinates in image where clicked */
		relativeImageClick.x = coords.x - bottomLeft1.x;
		relativeImageClick.y = coords.y - bottomLeft1.y;
		imgVol->drawnPlaneInfo[0].sliceNum = (int)((double)relativeImageClick.x/zoom);
		imgVol->drawnPlaneInfo[2].sliceNum = (int)((double)relativeImageClick.y/zoom);
		WriteLog(msg.Format("Left Mouse button pressed in plane 1 (%d,%d)...", relativeImageClick.x, relativeImageClick.y));
	}
	if (plane2.Contains(coords)) {
		/* determine coordinates in image where clicked */
		relativeImageClick.x = coords.x - bottomLeft2.x;
		relativeImageClick.y = coords.y - bottomLeft2.y;
		imgVol->drawnPlaneInfo[0].sliceNum = (int)((double)relativeImageClick.x/zoom);
		imgVol->drawnPlaneInfo[1].sliceNum = (int)((double)relativeImageClick.y/zoom);
		WriteLog(msg.Format("Left Mouse button pressed in plane 2 (%d,%d)...", relativeImageClick.x, relativeImageClick.y));
	}
}

// --------------------------------------------------------
// ---------- OnLeftMouseUp -------------------------------
// --------------------------------------------------------
void ImageCanvas::OnLeftMouseUp(wxMouseEvent &event)
{
	wxPoint coords = event.GetPosition();
		
	/* always flip the mouse's Y coords */
	coords.y = canvasSize.GetHeight() - coords.y;
	leftMouseDown = false;
	WriteLog(msg.Format("Left Mouse button up at (%d,%d)...", coords.x, coords.y));

	stopMotion(coords.x, coords.y, glutGet(GLUT_ELAPSED_TIME));

	Draw();
}


// --------------------------------------------------------
// ---------- Draw ----------------------------------------
// --------------------------------------------------------
void ImageCanvas::Draw()
{
	SetCurrent(); /* set this as the current GLCanvas */

	int w, h;
	WriteLog("ImageCanvas->Draw() called...");

	if (imgVol != NULL)
		if (imgVol->IsReady()) {
			if ((imgVol->IsMultiFrame()) && (mainVars->Toolbar_displayType() == IMGVIEW_SINGLEPLANE))
				frame->HideSliceScroll(false); /* show it */
			else
				frame->HideSliceScroll(true); /* hide it */

			GetClientSize(&w, &h);
			canvasSize.SetWidth(w);
			canvasSize.SetHeight(h);
			imgVol->SetCanvasSize(canvasSize);

			imgVol->Draw();
			glFlush();
			SwapBuffers();
		}

	WriteLog("ImageCanvas->Draw() finished...");
}


// --------------------------------------------------------
// ---------- ShowFileInfo --------------------------------
// --------------------------------------------------------
void ImageCanvas::ShowFileInfo()
{
	if (imgVol != NULL)
		if (imgVol->IsReady())
			imgVol->ShowFileInfo();
}



// --------------------------------------------------------
// ---------- OnLeftMouseDoubleClick ----------------------
// --------------------------------------------------------
void ImageCanvas::OnLeftMouseDoubleClick(wxMouseEvent &event)
{

}


// --------------------------------------------------------
// ---------- CloseCurrentFile ----------------------------
// --------------------------------------------------------
void ImageCanvas::CloseCurrentFile()
{
	imgVolumeReady = false;
	if (imgVol != NULL)
		delete imgVol;

	imgVol = NULL;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();
	SwapBuffers();
}


// --------------------------------------------------------
// ---------- LoadFile ------------------------------------
// --------------------------------------------------------
int ImageCanvas::LoadFile()
{
	WriteLog("ImageCanvas->LoadDicomVolume() called...");

	SetCurrent();
    /* Initialize OpenGL once, but only after SetCurrent */
    if (!m_init)
    {
        InitGL(GLMODE_FLAT);
        m_init = true;
    }

	/* delete the old object, set it to NULL, create a blank screen in preparation for loading the files */
	if (imgVol != NULL)
		delete imgVol;
	imgVol = NULL;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();
	SwapBuffers();

	/* create the ImageVolume object, then load the file(s)... */
	imgVol = new ImageVolume(frame, mainVars);
	if (!imgVol->LoadFile()) {
		WriteLog("Load failed in ImageCanvas->LoadFile()...");
		return 0;
	}
	WriteLog("Load successful in ImageCanvas->LoadFile()...");

	/* the object was created, but not everything in it may have been loaded... so
	   check to make sure the object is ready to be drawn */
	if (!imgVol->IsReady()) {
		WriteLog("Image volume NOT ready in ImageCanvas->LoadFile()...");
		return 0;
	}
	WriteLog("Image volume IS ready in ImageCanvas->LoadFile()...");

	imgVol->SetWindowShader(pWindow);
	imgVol->SetRayTraceShaders(pRayTrace, pRayTraceSSD);

	/* draw the volume */
	Draw();

	WriteLog("ImageCanvas->LoadDicomVolume() finished...");

	if (imgVol == NULL)
		return 0;
	else {
		frame->SetCurrentSlice(imgVol->GetStartSlice(), imgVol->GetEndSlice(), mainVars->Slice_value());
		frame->SetZoom(100);
		imgVolumeReady = true;
		return 1;
	}
}


// --------------------------------------------------------
// ---------- SaveScreenGrab ------------------------------
// --------------------------------------------------------
bool ImageCanvas::SaveScreenGrab(int filetype, wxString filepath)
{
	int w, h;
	GetClientSize(&w, &h);
	GLenum format = GL_RGB;
	wxImage *img;
	wxImage img2;

	/* Allocate with malloc, because the data will be managed by wxImage */
	unsigned char* pixels = static_cast<unsigned char*>(malloc(w*h*3));

	/* read from front buffer */
	glReadBuffer(GL_FRONT);

	/* read pixel data */
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0,0,w,h,format,GL_UNSIGNED_BYTE,pixels);

	/* write the file... */
	img = new wxImage(w,h,pixels);
	img2 = img->Mirror(false); /* flip it vertically */

	/* clean up */
	delete img;

	/* try to save the file, return false if it fails */
	if (img2.SaveFile(filepath,filetype))
		return true;
	else
		return false;
}


// --------------------------------------------------------
// ---------- ReadWholeFile -------------------------------
// --------------------------------------------------------
wxString ImageCanvas::ReadWholeFile(wxString filepath)
{
	wxString str;
	wxChar c;
	wxFile *f;

	str = "";

	f = new wxFile(filepath);
	while (!f->Eof()) {
		f->Read(&c,1);
		str += c;
	}

	delete f;
	return str;
}


// --------------------------------------------------------
// ---------- ptov ----------------------------------------
// --------------------------------------------------------
void ptov(int x, int y, int width, int height, float v[3]){
	float d, a;

	/* project x,y onto a hemi-sphere centered within width, height */
	v[0] = (2.0*x - width) / width; /* normalize to [0,1] */
	v[1] = (height - 2.0*y) / height; /* normalize to [0,1] */
	d = sqrt(v[0]*v[0] + v[1]*v[1]); /* find the distance of this to (0,0) */
	v[2] = cos((M_PI/2.0) * ((d < 1.0) ? d : 1.0));
	a = 1.0 / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	v[0] *= a;
	v[1] *= a;
	v[2] *= a;
}


// --------------------------------------------------------
// ---------- startMotion ---------------------------------
// --------------------------------------------------------
void ImageCanvas::startMotion(int x, int y, int time) {
	
	/* Start tracking mouse motion (user pressed mouse) */
	mainVars->VolRend_isMoving(true);

	int width = canvasSize.GetWidth();
	int height = canvasSize.GetHeight();

	if (gbPlane)
		gbPlaneMove=true;
	else
		mode = MoveView;
	mainVars->Rotations_isMoving(true);

	trackingMotion = GL_TRUE;
	redrawContinuously = GL_FALSE;
	lastTime = time;
	ptov(x, y, width, height, lastPos);
}

// --------------------------------------------------------
// ---------- stopMotion ----------------------------------
// --------------------------------------------------------
void ImageCanvas::stopMotion(int x, int y, int time) {

	/* Stop tracking motion (user released mouse) */
	mainVars->VolRend_isMoving(false);

	trackingMotion = GL_FALSE;

	if (time == lastTime) {
		redrawContinuously = GL_TRUE;
	}
	else {
		if ((!gbPlaneMove)&&(mode!=MoveNone)){
			angle = 0.0;
			redrawContinuously = GL_FALSE;
		}
		angle = 0.0;
	}

	mainVars->Rotations_isMoving(false);

	if (!redrawContinuously){
		if (gbPlane)
			gbPlaneMove=false;
		else
			mode = MoveNone;
	}
}

// --------------------------------------------------------
// ---------- trackMotion ---------------------------------
// --------------------------------------------------------
void ImageCanvas::trackMotion(int x, int y) {
	
	/* Call-back for mouse movements */

	int width = canvasSize.GetWidth();
	int height = canvasSize.GetHeight();

	if (trackingMotion) {
		float curPos[3], dx, dy, dz;

		ptov(x, y, width, height, curPos);
		WriteLog(msg.Format("ptov(%d,%d,%d,%d,[%.4f,%.4f,%.4f])",x,y,width,height,curPos[0],curPos[1],curPos[2]));

		/* distance between current and last position */
		dx = curPos[0] - lastPos[0];
		dy = curPos[1] - lastPos[1];
		dz = curPos[2] - lastPos[2];

		/* 90 * distance fro 0,0,0 to current position */
		angle = 90.0 * sqrt(dx*dx + dy*dy + dz*dz);

		/* cross product of current position and last position */
		axis[0] = lastPos[1]*curPos[2] - lastPos[2]*curPos[1];
		axis[1] = lastPos[2]*curPos[0] - lastPos[0]*curPos[2];
		axis[2] = lastPos[0]*curPos[1] - lastPos[1]*curPos[0];

		WriteLog(msg.Format("axis[0] %.4f  axis[1]%.4f  axis[2]%.4f",axis[0],axis[1],axis[2]));

		lastTime = glutGet(GLUT_ELAPSED_TIME);
		lastPos[0] = curPos[0];
		lastPos[1] = curPos[1];
		lastPos[2] = curPos[2];
	}
}