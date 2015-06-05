/* ----------------------------------------------------------------------------
	ImageVolume.h


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
		ImageCanvas
	
	Owns:
		ImageVolumeHelper
		ImageVolumeRender
		ImageData
		ImageViews
		ImageSliceThread
		ImageDisplayListItem

	Basic functionality:
		*draws everything based on the variables it reads from MainFrameVariables
		*pass some variables from ImageCanvas to ImageData and beyond
*/
//////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageViews.h"
#include "ImageDisplayListItem.h"
#include "ImageData.h"
#include "ImageVolumeHelper.h"
#include "ImageVolumeRender.h"
#include "ImageVolumeRenderRay.h"
//#include "SegmentationFrame.h"
#include "MainFrameVariables.h"
#include "ImageSliceThread.h"
#include "wx/dynarray.h"

/* forward declare the image slice thread */
class ImageSliceThread;

WX_DECLARE_OBJARRAY(ImageViews,c_imgViews);
WX_DECLARE_OBJARRAY(ImageDisplayListItem,c_imageList);
WX_DEFINE_ARRAY_PTR(ImageSliceThread *, ThreadArray);

class ImageVolume  
{
public:
	ImageVolume(MainFrame *Frame, MainFrameVariables *MainVars);
	virtual ~ImageVolume();

	int  Draw();				/* function called by imgCanvas to do the drawing */
	void DrawVolumeRendering();	/* setup the volume rendering object and draw it */
	int  GetCurrentView();
	int  GetEndSlice();
	int  GetStartSlice();
	bool IsMultiFrame();
	bool IsReady();
	int LoadFile();
	void SetActive(bool active);	/* set the current imageVolume object as the active one... if multiple are present */
	void SetCanvasSize(wxSize size);
	void SetCenterOffset(wxPoint point);
	void SetColorize(bool useC, wxColour col1, wxColour col2);
	void SetHiRes(bool hiRes);
	void SetMultisliceParams(int cols, int rows, bool bestfit, int sortby);
	void SetWindowShader(GLuint prog);
	void SetRayTraceShaders(GLuint progReg, GLuint progSSD);
	void SetShowInfoOverlay(bool show);
	void SetShowLineOverlay(bool show);
	void SetVRParams(wxColour color, double intensity, double density, int numSlices);
	void SetWindowLevel(int center, int width);
	void ShowFileInfo();
	int  SwitchView(int view);

	/* the one and only image data object, all image/volume related data is in here */
	ImageData *imgData;

	/* array of image views */
	c_imgViews imgViews;

	/* information about where on the screen things are drawn */
	/* this is only used in the orthogonal view */
	struct structDrawnInfo {
		wxRealPoint imageOrigin; /* center of the image */
		wxPoint minmin; /* origin of both axes */
		wxPoint maxmax; /* max point of both axes */
		wxPoint lineA1, lineA2; /* points where line A intersects edges of plane */
		wxPoint lineB1, lineB2; /* points where line B intersects edges of plane */
		wxPoint lineC1, lineC2; /* points where line C intersects edges of plane */
		int lineA, lineB, lineC; /* slice numbers from other planes... this is the simple version of the
								 orthogonal view */
		wxPoint focus; /* point where lines are centered, place where mouse was clicked */
		int sliceNum; /* the slice displayed in the plane */
		bool flipX, flipY, rotateR, rotateL; /* flags if the image needed to be rotated to look correct */
		int zoom; /* current zoom for the plane */
		XYZTriplet scale;
	} drawnPlaneInfo[3];

private:
	/* private drawing functions */
	void Draw3View();
	void Draw3ViewInfoOverlay();
	void DrawColorBar();
	void DrawImage();
	void DrawLargeInfoOverlay(int plane);
	void DrawOrientation();
	void DrawSliceStack();
	void DrawMultislice();
	void DrawTimecourse(int x, int y, int z);
	void Draw3Plane(int x, int y, int z);
	void DrawTimecourseLine(int x, int y, int z, wxRect drawArea, double max);
	void DrawScaleBar(int plane);
	int GetBestFitZoom(wxSize windowSize, wxSize imageSize);
	wxString GetOrientation(double x, double y, double z);
	wxSize GetImageSizes(int plane);
	wxRealPoint GetImageSpacings(int plane);
	wxSize GetTextureSize(int Size1, int Size2);
	void InitializeVariables();
	void DrawTriPlaneLines();
	wxString ReadWholeFile(wxString filepath);

	/* pointers to other classes */
	MainFrame			*frame;			/* the main frame */
	ImageVolumeHelper	*imgHelper;		/* drawing helper */
	//LogWindow			*logWindow;		/* pointer to logwindow */
	ImageCanvas			*imgCanvas;		/* pointer to the parent canvas */
	ImageVolumeRender	*imgVolRender;	/* volume rendering using 3D texture slicing */
	ImageVolumeRenderRay *imgVolRenderRay; /* volume rendering using ray casting */
	MainFrameVariables	*mainVars;		/* variables contained in the UI elements of the main frame */

	wxSize canvasSize;
	wxString msg;
	wxString dirPath;
	c_imageList imgList; /* parameters for a list of image to be displayed */
	ThreadArray threads; /* threads for the DrawImage() function */

	bool isActive;
	int viewID;
	bool readyToDraw;
	unsigned long long int freq, startTime, currentTime;
	double fps;
	int frameCount;
	bool volRendInitialized;
	bool fragmentShaderAvailable;
	GLuint pWindow; /* shader program for the window level and color map */
	GLuint pRayTrace; /* shader program for the volume rendering ray tracer */
	GLuint pRayTraceSSD; /* shader program for the volume rendering SSD */

	/* single drawing buffers */
	UINT_8		*drawBuffer8U;
	INT_8		*drawBuffer8S;
	UINT_16		*drawBuffer16U;
	INT_16		*drawBuffer16S;
	UINT_32		*drawBuffer32U;
	INT_32		*drawBuffer32S;
	FLOAT_32	*drawBufferFLOAT32;
	UINT_8		*drawBufferRGB;

	/* define array of pointers for the buffers, so each thread can have its own buffer */
	WX_DEFINE_ARRAY_PTR(UINT_8 *, arrayBuffer8U);
	WX_DEFINE_ARRAY_PTR(INT_8 *, arrayBuffer8S);
	WX_DEFINE_ARRAY_PTR(UINT_16 *, arrayBuffer16U);
	WX_DEFINE_ARRAY_PTR(INT_16 *, arrayBuffer16S);
	WX_DEFINE_ARRAY_PTR(UINT_32 *, arrayBuffer32U);
	WX_DEFINE_ARRAY_PTR(INT_32 *, arrayBuffer32S);
	WX_DEFINE_ARRAY_PTR(FLOAT_32 *, arrayBufferFLOAT32);
	WX_DEFINE_ARRAY_PTR(UINT_8 *, arrayBufferRGB); /* just to make the naming scheme easier */

	/* array of OpenGL drawing buffers... needed for the multi-threading */
	arrayBuffer8U		buffer8U;
	arrayBuffer8S		buffer8S;
	arrayBuffer16U		buffer16U;
	arrayBuffer16S		buffer16S;
	arrayBuffer32U		buffer32U;
	arrayBuffer32S		buffer32S;
	arrayBufferFLOAT32	bufferFLOAT32;
	arrayBufferRGB		bufferRGB;
};
