/* ----------------------------------------------------------------------------
	ImageVolume.cpp


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
#include "wx/File.h"
#include <iostream>
#include "math.h"
#include "stdio.h"
#include "ImageVolume.h"

#include "wx/arrimpl.cpp"
WX_DEFINE_OBJARRAY(c_imgViews);
WX_DEFINE_OBJARRAY(c_imageList);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include <crtdbg.h>
#define new new(_NORMAL_BLOCK,__FILE__, __LINE__)

// --------------------------------------------------------
// ---------- ImageVolume ---------------------------------
// --------------------------------------------------------
ImageVolume::ImageVolume(MainFrame *Frame, MainFrameVariables *MainVars)
{
	//int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	//flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
	//_CrtSetDbgFlag(flag); // Set flag to the new value

	readyToDraw = false;
	//logWindow = logwind;
	frame = Frame;
	mainVars = MainVars;
	imgCanvas = frame->imgCanvas;

	//WriteLog("ImageVolume->InitializeVariables() about to be called...\n");
	InitializeVariables();
	//WriteLog("ImageVolume->InitializeVariables() finished being called...\n");

	imgData = new ImageData(mainVars);
	imgHelper = new ImageVolumeHelper(imgData);
}


// --------------------------------------------------------
// ---------- ~ImageVolume --------------------------------
// --------------------------------------------------------
ImageVolume::~ImageVolume()
{
	readyToDraw = false;
//	WriteLog("ImageVolume->~ImageVolume() called...\n");

	delete imgData;
	delete imgHelper;
	imgViews.Empty();
	if (imgVolRender != NULL)
		delete imgVolRender;
	if (imgVolRenderRay != NULL)
		delete imgVolRenderRay;
}


// --------------------------------------------------------
// ---------- InitializeVariables -------------------------
// --------------------------------------------------------
void ImageVolume::InitializeVariables()
{
	int i;

	WriteLog("ImageVolume->InitializeVariables() called... \n");

	viewID = 0;
	imgVolRender = NULL;
	imgVolRenderRay = NULL;
	imgData = NULL;
	imgHelper = NULL;

	volRendInitialized = false;

	fps = 0.0;
	frameCount = 0;

	/* create a default view, and populate it */
	ImageViews newView;
	newView.xScreenOffset = 0;
	newView.yScreenOffset = 0;
	newView.multislicePageNum = 0;

	imgViews.Add(newView);

	/* initialize the structure for the orthogonal view */
	for (i=0;i<3;i++) {
		drawnPlaneInfo[i].focus = wxPoint::wxPoint(-1,-1);
		drawnPlaneInfo[i].imageOrigin = wxRealPoint::wxRealPoint(-1.0, -1.0);
		drawnPlaneInfo[i].maxmax = wxPoint::wxPoint(-1,-1);
		drawnPlaneInfo[i].minmin = wxPoint::wxPoint(-1,-1);
		drawnPlaneInfo[i].sliceNum = -1;
	}

	WriteLog("ImageVolume->InitializeVariables() finished... \n");
}


// ========================================================
// ---------- Gets & Sets ---------------------------------
// ========================================================


void ImageVolume::SetWindowShader(GLuint prog) { pWindow = prog; }
void ImageVolume::SetRayTraceShaders(GLuint progReg, GLuint progSSD) { pRayTrace = progReg; pRayTraceSSD = progSSD; }

/* ========== GetStartSlice =============================== */
int ImageVolume::GetStartSlice() { return 0; }

/* ========== GetEndSlice ================================= */
int ImageVolume::GetEndSlice() {
	int slice;
	switch (mainVars->Image_plane()) {
		case 0: slice = imgData->imgInfo->GetVolumeZSize() - 1; break;
		case 1: slice = imgData->imgInfo->GetVolumeXSize() - 1; break;
		case 2: slice = imgData->imgInfo->GetVolumeYSize() - 1; break;
		default: slice = imgData->imgInfo->GetVolumeZSize() - 1;
	}
	return slice;
}

/* ========== GetCurrentView ============================== */
int ImageVolume::GetCurrentView() { return viewID; }

/* ========== IsMultiFrame ================================ */
bool ImageVolume::IsMultiFrame()
{
	if (readyToDraw != true)
		return false;
	if ((imgHelper == NULL) || (imgData == NULL))
		return false;
	if (!imgData->DataIsValid())
		return false;

	if (imgData->imgInfo->GetVolumeZSize() > 1)
		return true;
	else
		return false;
}

/* ========== ShowFileInfo ================================ */
void ImageVolume::ShowFileInfo() { imgData->ShowFileInfo(); }

/* ========== SwitchView ================================== */
int ImageVolume::SwitchView(int view)
{
	int numViews = (int)imgViews.GetCount();
	if (view > numViews) view = numViews - 1;
	if (view < 0) view = 0;
	viewID = view;
	return viewID;
}

/* ========== SetCanvasSize =============================== */
void ImageVolume::SetCanvasSize(wxSize size) { canvasSize = size; }

/* ========== SetActive =================================== */
void ImageVolume::SetActive(bool active) { isActive = active; }

/* ========== SetCenterOffset ============================= */
void ImageVolume::SetCenterOffset(wxPoint point)
{
	imgViews[viewID].xScreenOffset = point.x;
	imgViews[viewID].yScreenOffset = point.y;
}

/* ========== SetWindowLevel ============================== */
void ImageVolume::SetWindowLevel(int center, int width)
{
	bool isRGB;
	if (imgData->imgInfo->GetPixelType() == IMG_RGB24) isRGB = true;
	else isRGB = false;
	if (readyToDraw)
		imgHelper->CalculateValueLookupTable(mainVars->Display_windowWidth(), mainVars->Display_windowCenter(), imgData->imgInfo->GetMinPixelValues().Gray(), imgData->imgInfo->GetMaxPixelValues().Gray(), isRGB);
}

/* ========== IsReady ===================================== */
bool ImageVolume::IsReady()
{
	if ((readyToDraw) && (imgData != NULL)) {
		if (imgData->DataIsValid()) return true;
		else return false;
	}
	else return false;
}

// --------------------------------------------------------
// ---------- Draw -------------------------------public---
// --------------------------------------------------------
int ImageVolume::Draw()
{
	/* before ANYTHING is done... check to see if we're actually ready to draw */
	if (readyToDraw != true)
		return 0;
	if ((imgHelper == NULL) || (imgData == NULL))
		return 0;
	if (!imgData->DataIsValid())
		return 0;

	/* setup some variables */
	double avgTime;
	wxSize size;
	wxRealPoint point;
	int plane, slice;

	/* clear the background to black */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* reset the FPS variables */
	if (frameCount == 0) {
	    QueryPerformanceFrequency( ( LARGE_INTEGER * ) &freq );
		QueryPerformanceCounter( ( LARGE_INTEGER * ) &startTime );
	}

	WriteLog("---------------------- ImageVolume->Draw() called... ----------------------\n");

	/* before we do any DRAWING... don't let this function be called again until it is complete */
	readyToDraw = false;

	/* clear the list of images */
	imgList.Clear();

	frameCount++;

	/* determine the type of view to be displayed */
	switch (mainVars->Toolbar_displayType()) {
		case IMGVIEW_DVR:
			//glPushMatrix();
			DrawVolumeRendering();
			//glPopMatrix();
			
			/* setup the viewport for the text overlays */
			glViewport(0, 0, canvasSize.GetWidth(), canvasSize.GetHeight());
			glMatrixMode (GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0.0, (double)canvasSize.GetWidth(), 0.0, (double)canvasSize.GetHeight(), 1000.0, -1000.0);
			glMatrixMode (GL_MODELVIEW);

			/* get the FPS count */
			if (frameCount >= 19) {
				QueryPerformanceCounter( ( LARGE_INTEGER * ) &currentTime );
				avgTime = ((double)currentTime - (double)startTime)/20.0;
				fps = 1000.0/(1000.0 * (avgTime / (double)freq));
				frameCount = 0;
			}

			//if (mainVars->Toolbar_showTextOverlay())
			//	DrawLargeInfoOverlay(0);
			break;
		case IMGVIEW_MIP:
			//DrawSliceStack();
			//QueryPerformanceCounter( ( LARGE_INTEGER * ) &currentTime );
			//fps = 1000.0/(1000.0 * ((double)currentTime - (double)startTime) / (double)freq);
			break;
		case IMGVIEW_3VIEW:
			/* make sure the orthogonal view is appropriate for the loaded dataset */
			if ((imgData->imgInfo->GetVolumeType() == VOL_3D) || (imgData->imgInfo->GetVolumeType() == VOL_4D)) {
				Draw3View();
				if (frameCount >= 19) {
					QueryPerformanceCounter( ( LARGE_INTEGER * ) &currentTime );
					avgTime = ((double)currentTime - (double)startTime)/20.0;
					fps = 1000.0/(1000.0 * (avgTime / (double)freq));
					frameCount = 0;
				}
				if (mainVars->Toolbar_showLineOverlay())
					Draw3ViewInfoOverlay();
			}
			break;
		case IMGVIEW_MULTISLICE:
			DrawMultislice();
			if (frameCount >= 19) {
				QueryPerformanceCounter( ( LARGE_INTEGER * ) &currentTime );
				avgTime = ((double)currentTime - (double)startTime)/20.0;
				fps = 1000.0/(1000.0 * (avgTime / (double)freq));
				frameCount = 0;
			}
			break;
		default:
			size.Set(canvasSize.GetWidth(), canvasSize.GetHeight());
			point = wxRealPoint::wxRealPoint((double)imgViews[viewID].xScreenOffset,(double)imgViews[viewID].yScreenOffset);
			plane = mainVars->Image_plane();
			slice = mainVars->Slice_value();

			/* setup a single image and add it to the display list */
			ImageDisplayListItem img = ImageDisplayListItem(plane, slice, mainVars->Image_zoomPercent(), size, point, wxRealPoint(0.0, 0.0), 0, mainVars->Toolbar_showHighRes(), 0, false, false);
			imgList.Clear();
			imgList.Add(img);
			DrawImage();
			DrawColorBar();

			if (frameCount >= 19) {
				QueryPerformanceCounter( ( LARGE_INTEGER * ) &currentTime );
				avgTime = ((double)currentTime - (double)startTime)/20.0;
				fps = 1000.0/(1000.0 * (avgTime / (double)freq));
				frameCount = 0;
			}

			if (mainVars->Toolbar_showTextOverlay())
				DrawLargeInfoOverlay(plane);
	}

	/* everything is done, so now its ok to draw again */
	readyToDraw = true;

	return 1;
}


// --------------------------------------------------------
// ---------- DrawMultislice ------------------------------
// --------------------------------------------------------
void ImageVolume::DrawMultislice()
{
	int numPerPage, numPages, numSlices;
	int startSlice, endSlice;
	int col, row, i;
	wxRealPoint centerOffset;
	wxRealPoint bottomLeft;
	wxSize screenSize = canvasSize;
	wxSize sliceBoxSize;
	double sb_h, sb_w;
	wxSize originalImageSize;
	wxString str;
	int newZoom;
	int plane = mainVars->Image_plane();
	double x1,x2,y1,y2;
	int sortby = mainVars->Multislice_sortBy(); //imgViews[viewID].multisliceSortby;
	bool bestfit = mainVars->Multislice_useBestFit();
	int pageNum = imgViews[viewID].multislicePageNum;
	int numCols = mainVars->Multislice_numCols();
	int numRows = mainVars->Multislice_numRows();

	/* determine number of slices for the current plane */
	switch (plane) {
		case 0: numSlices = imgData->imgInfo->GetVolumeZSize();
			originalImageSize.Set(imgData->imgInfo->GetVolumeXSize(),imgData->imgInfo->GetVolumeYSize());
			break;
		case 1: numSlices = imgData->imgInfo->GetVolumeXSize();
			originalImageSize.Set(imgData->imgInfo->GetVolumeZSize(),imgData->imgInfo->GetVolumeYSize());
			break;
		case 2: numSlices = imgData->imgInfo->GetVolumeYSize();
			originalImageSize.Set(imgData->imgInfo->GetVolumeXSize(),imgData->imgInfo->GetVolumeZSize());
			break;
		default: numSlices = imgData->imgInfo->GetVolumeZSize();
			originalImageSize.Set(imgData->imgInfo->GetVolumeXSize(),imgData->imgInfo->GetVolumeYSize());
			break;
	}

	/* determine the number of pages needed */
	if (bestfit) {
		numPerPage = numCols * numRows;
		numPages = 1;
		numCols = ceil((double)sqrt((double)numSlices));
		numRows = ceil((double)((double)numSlices/(double)numCols));
	}
	else {
		numPerPage = 20;
		numPages = ceil((double)((double)numSlices/(double)numPerPage));
	}

	/* determine available slice area for each slice... */
	sliceBoxSize.Set(screenSize.GetWidth()/numCols,screenSize.GetHeight()/numRows);
	sb_w = screenSize.GetWidth()/numCols;
	sb_h = screenSize.GetHeight()/numRows;

	/* ...and the new zoom percentage */
	newZoom = GetBestFitZoom(sliceBoxSize,originalImageSize);

	/* find start and end slices for the current page */
	startSlice = pageNum * numPerPage;
	endSlice = startSlice + numPerPage - 1;

	/* this will be the same for all the slices */
	centerOffset.x = 0;
	centerOffset.y = 0;

	glLineWidth(1.0);

	/* loop through all the slices and calculate their position */
	imgList.Clear();
	col = row = 0;

	WriteLog(msg.Format("Multislice, endSlice: %d\n",endSlice));

	for (i=startSlice;i<=endSlice;i++) {
		//bottomLeft.x = col * sliceBoxSize.GetWidth();
		bottomLeft.x = col * sb_w;
		//bottomLeft.y = row * sliceBoxSize.GetHeight();
		bottomLeft.y = row * sb_h;

		WriteLog(msg.Format("Calling DrawImage(%d,%d,%d,[%d,%d],[%d,%d],[%d,%d],true)\n",plane, i, newZoom, sliceBoxSize.GetWidth(), sliceBoxSize.GetHeight(), centerOffset.x, centerOffset.y, bottomLeft.x, bottomLeft.y ));

		ImageDisplayListItem img = ImageDisplayListItem(plane, i, newZoom, sliceBoxSize, centerOffset, bottomLeft, 0, mainVars->Toolbar_showHighRes(), 0, false, false);
		imgList.Add(img);

		glColor3d(212.0/255.0, 191.0/255.0, 49.0/255.0);
		x1 = bottomLeft.x;
		x2 = x1 + sliceBoxSize.GetWidth();
		y1 = bottomLeft.y;
		y2 = y1 + sliceBoxSize.GetHeight();
		glBegin(GL_LINES);
			glVertex2f(x1, y1); glVertex2f(x1, y2);
			glVertex2f(x1, y2); glVertex2f(x2, y2);
			glVertex2f(x2, y2); glVertex2f(x2, y1);
			glVertex2f(x2, y1); glVertex2f(x1, y1);
		glEnd();

		str = wxString::Format("%d",i);
		DrawShadowText(str,bottomLeft.x + 2,bottomLeft.y + 2,1.0,1.0,1.0,10);

		if (sortby == ROWS) {
			col++;
			if (col >= numCols) {
				col = 0;
				row++;
			}
		}
		else {
			row++;
			if (row >= numRows) {
				row = 0;
				col++;
			}
		}
	}
	DrawImage();
}




// --------------------------------------------------------
// ---------- DrawLargeInfoOverlay ------------------------
// --------------------------------------------------------
void ImageVolume::DrawLargeInfoOverlay(int plane)
{
//	WriteLog("ImageVolume->DrawLargeInfoOverlay() called...\n");

	wxString string;
	int width, height;
	int borderWidth;
	int stringWidth;

	width = canvasSize.GetWidth();
	height = canvasSize.GetHeight();
	borderWidth = 4;

	int slice;
	switch (plane) {
		case 0: slice = imgData->imgInfo->GetVolumeZSize(); break;
		case 1: slice = imgData->imgInfo->GetVolumeXSize(); break;
		case 2: slice = imgData->imgInfo->GetVolumeYSize(); break;
		default: slice = imgData->imgInfo->GetVolumeZSize();
	}

	/* --- lower right display --- */
	/* display current window center */
	string = wxString::Format("C: %d", mainVars->Display_windowCenter());
	stringWidth = GetDrawnStringWidth(string, 12);
	DrawShadowText(string,width-20-stringWidth,20,1.0,1.0,1.0,12);
	/* display current window width */
	string = wxString::Format("W: %d", mainVars->Display_windowWidth());
	stringWidth = GetDrawnStringWidth(string, 12);
	DrawShadowText(string,width-20-stringWidth,35,1.0,1.0,1.0,12);
	/* display current frames per second */
	string = wxString::Format("fps: %.1f", fps);
	stringWidth = GetDrawnStringWidth(string, 12);
	DrawShadowText(string,width-20-stringWidth,50,1.0,1.0,1.0,12);

	/* --- upper left display --- patient, series info */
	if (imgData->GetFileType() == FILETYPE_DICOM) {
		/* display patient name */
		string = imgData->dcmInfo->GetPatientName();
		DrawShadowText(string,15,height-25,1.0,1.0,1.0,10);
		/* display protocol name */
		string = imgData->dcmInfo->GetProtocolName();
		DrawShadowText(string,15,height-37,1.0,1.0,1.0,10);
		/* display series date */
		if (imgData->dcmInfo->GetSeriesDateTime().IsValid()) {
//			WriteLog(msg.Format("seriesDate: %s...\n", imgData->dcmInfo->GetSeriesDate().c_str()));
			string = imgData->dcmInfo->GetSeriesDateTime().Format("%b-%d-%Y %I:%M:%S %p");
			string = "Series: " + string;
			DrawShadowText(string,15,height-49,1.0,1.0,1.0,10);
		}
		/* display date of birth */
		string = wxString::Format("DOB: %s",imgData->dcmInfo->GetPatientBirthdate());
		DrawShadowText(string,15,height-61,1.0,1.0,1.0,10);

		/* --- upper right display --- study, intitution info */
		/* display performing physician */
		string = imgData->dcmInfo->GetPerformingPhysician();
		stringWidth = GetDrawnStringWidth(string, 10);
		DrawShadowText(string,width-20-stringWidth,height-25,1.0,1.0,1.0,10);
		/* display study description */
		string = imgData->dcmInfo->GetStudyDescription();
		stringWidth = GetDrawnStringWidth(string, 10);
		DrawShadowText(string,width-20-stringWidth,height-37,1.0,1.0,1.0,10);
		/* display institution name */
		string = imgData->dcmInfo->GetInstitutionName();
		stringWidth = GetDrawnStringWidth(string, 10);
		DrawShadowText(string,width-20-stringWidth,height-49,1.0,1.0,1.0,10);
		/* display study date */
		if (imgData->dcmInfo->GetStudyDateTime().IsValid()) {
//			WriteLog(msg.Format("studyDate: %s...\n", imgData->dcmInfo->GetStudyDate().c_str()));
			string = imgData->dcmInfo->GetStudyDateTime().Format("%b-%d-%Y %I:%M:%S %p");
			string = "Study: " + string;
			stringWidth = GetDrawnStringWidth(string, 10);
			DrawShadowText(string,width-20-stringWidth,height-61,1.0,1.0,1.0,10);
		}

		/* --- lower left display --- image info */
		/* display modality */
		string = imgData->dcmInfo->GetModalityDescription();
		DrawShadowText(string,15,15,1.0,1.0,1.0,10);
		/* display position string (HFS, FFDR, etc) */
		string = imgData->dcmInfo->GetPatientPositionStr();
		DrawShadowText(string,15,27,1.0,1.0,1.0,10);
		/* display cosines */
		string = wxString::Format("%.6f,%.6f,%.6f %.6f,%.6f,%.6f",imgData->dcmInfo->GetRowCosines().x, imgData->dcmInfo->GetRowCosines().y, imgData->dcmInfo->GetRowCosines().z, imgData->dcmInfo->GetColCosines().x, imgData->dcmInfo->GetColCosines().y, imgData->dcmInfo->GetColCosines().z);
		DrawShadowText(string,15,39,1.0,1.0,1.0,10);
	}
	/* display image size */
	string = wxString::Format("X:%d Y:%d Z:%d T:%d",imgData->imgInfo->GetVolumeXSize(),imgData->imgInfo->GetVolumeYSize(),imgData->imgInfo->GetVolumeZSize(),imgData->imgInfo->GetVolumeTSize());
	DrawShadowText(string,15,51,1.0,1.0,1.0,10);

	/* display current slice */
	string = wxString::Format("Slice %d of %d",mainVars->Slice_value() + 1, slice);
	DrawShadowText(string,15,63,1.0,1.0,1.0,10);

	/* if its active, draw a box around the image */
	if (isActive) {
		glColor3d(23.0/255.0, 90.0/255.0, 202.0/255.0);
		glBegin(GL_QUADS);
			glVertex2d(0, 0);
			glVertex2d(borderWidth, 0);
			glVertex2d(borderWidth, height);
			glVertex2d(0, height);

			glVertex2d(0, 0);
			glVertex2d(0, borderWidth);
			glVertex2d(width, borderWidth);
			glVertex2d(width, 0);

			glVertex2d(0, height);
			glVertex2d(0, height-borderWidth);
			glVertex2d(width, height-borderWidth);
			glVertex2d(width, height);

			glVertex2d(width, 0);
			glVertex2d(width-borderWidth, 0);
			glVertex2d(width-borderWidth, height);
			glVertex2d(width, height);
		glEnd();
	}

	DrawScaleBar(plane);
	DrawOrientation();

//	WriteLog("ImageVolume->DrawLargeInfoOverlay() finished...\n");
}


// --------------------------------------------------------
// ---------- Draw3View -----------------------------------
// --------------------------------------------------------
void ImageVolume::Draw3View()
{
	WriteLog("ImageVolume->Draw3View() called...\n");

	wxRealPoint offset, offset0, offset1, offset2;
	wxRealPoint bottomLeft0, bottomLeft1, bottomLeft2;
	wxSize size;
	int zoom, zoom0, zoom1, zoom2;
	int slice0, slice1, slice2;
	int width, height;
	int XSize, YSize, ZSize;

	width = canvasSize.GetWidth();
	height = canvasSize.GetHeight();
	XSize = imgData->imgInfo->GetVolumeXSize();
	YSize = imgData->imgInfo->GetVolumeYSize();
	ZSize = imgData->imgInfo->GetVolumeZSize();

	/* plane	size1	size2
	   0		X		Y
	   1		Z		Y
	   2		X		Z */
	offset.x = 0;
	offset.y = 0;
	size.Set((int)(width/2), (int)(height/2));

	/* upper left */
	bottomLeft2 = wxRealPoint(0.0, (double)canvasSize.GetHeight()/2);
	zoom2 = GetBestFitZoom(size,wxSize(XSize, ZSize));

	/* upper right */
	bottomLeft1 = wxRealPoint((double)canvasSize.GetWidth()/2, (double)canvasSize.GetHeight()/2);
	zoom1 = GetBestFitZoom(size,wxSize(ZSize, YSize));

	/* lower left */
	bottomLeft0 = wxRealPoint(0.0, 0.0);
	zoom0 = GetBestFitZoom(size,wxSize(XSize, YSize));

	/* find the smallest zoom ratio */
	if (zoom0 > zoom1) zoom = zoom1;
	else zoom = zoom0;
	if (zoom > zoom2) zoom = zoom2;

	/* shrink the zoom a little so its not touching the edges */
	zoom = (int)((double)zoom * 0.95);

	if (drawnPlaneInfo[0].sliceNum < 0) slice0 = (int)((double)ZSize/2.0);
	else slice0 = drawnPlaneInfo[0].sliceNum;
	if (drawnPlaneInfo[1].sliceNum < 0) slice1 = (int)((double)YSize/2.0);
	else slice1 = drawnPlaneInfo[1].sliceNum;
	if (drawnPlaneInfo[2].sliceNum < 0) slice2 = (int)((double)XSize/2.0);
	else slice2 = drawnPlaneInfo[2].sliceNum;

	WriteLog(msg.Format("Draw3View() - slice0: %d \n",slice0 ));
	WriteLog(msg.Format("Draw3View() - slice1: %d \n",slice1 ));
	WriteLog(msg.Format("Draw3View() - slice2: %d \n",slice2 ));

	/* add the 3 views to the display list */
	ImageDisplayListItem img = ImageDisplayListItem(2, slice2, zoom, size, offset, bottomLeft2, 0, mainVars->Toolbar_showHighRes(), 0, false, false);
	imgList.Add(img);
	img = ImageDisplayListItem(1, slice1, zoom, size, offset, bottomLeft1, 0, mainVars->Toolbar_showHighRes(), 0, false, false);
	imgList.Add(img);
	img = ImageDisplayListItem(0, slice0, zoom, size, offset, bottomLeft0, 0, mainVars->Toolbar_showHighRes(), 0, false, false);
	imgList.Add(img);
	DrawImage();

	if (imgData->imgInfo->GetVolumeType() == VOL_4D)
		DrawTimecourse(slice1, slice2, slice0);
	if (imgData->imgInfo->GetVolumeType() == VOL_3D)
		Draw3Plane(slice1, slice2, slice0);

	WriteLog("ImageVolume->Draw3View() finished...\n");
}


// --------------------------------------------------------
// ---------- DrawTimecourse ------------------------------
// --------------------------------------------------------
void ImageVolume::DrawTimecourse(int x, int y, int z)
{
	int i;
	int xSize = imgData->imgInfo->GetVolumeXSize();
	int ySize = imgData->imgInfo->GetVolumeYSize();
	int zSize = imgData->imgInfo->GetVolumeZSize();
	int t = imgData->imgInfo->GetVolumeTSize();
	int avgline; //, px, py;
	int width = canvasSize.GetWidth();
	int height = canvasSize.GetHeight();
	double total, value, average, min, max;
	wxRect drawArea;
	wxString string;

	value = 0.0;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);

	/* setup the area in which the timecourse will be drawn */
	drawArea = wxRect((int)(width/2),0,(int)(width/2),(int)(height/2));

	/* determine the mean value for the timecourse */
	total = 0.0;
	min = DBL_MAX;
	max = DBL_MIN;
	for (i=0;i<t;i++) {
		switch (imgData->imgInfo->GetPixelType()) {
			case IMG_UINT8: value = imgData->imgDataUINT8->Get(x,y,z,i,0); break;
			case IMG_INT8: value = imgData->imgDataINT8->Get(x,y,z,i,0); break;
			case IMG_UINT16: value = imgData->imgDataUINT16->Get(x,y,z,i,0); break;
			case IMG_INT16: value = imgData->imgDataINT16->Get(x,y,z,i,0); break;
			case IMG_UINT32: value = imgData->imgDataUINT32->Get(x,y,z,i,0); break;
			case IMG_INT32: value = imgData->imgDataINT32->Get(x,y,z,i,0); break;
		}
		total += value;
		if (value > max)
			max = value;
		if (value < min)
			min = value;
	}
	max = imgData->imgInfo->maxPixelValues.Gray();
	average = total/t;

	/* draw the mean line */
	avgline = (drawArea.GetHeight() * average)/max;
	glColor3d(1.0,1.0,1.0);
	glLineWidth(0.5);
	glBegin(GL_LINES);
		glVertex2d(width/2,avgline);
		glVertex2d(width,avgline);
	glEnd();

	/* draw the surrounding pixels timecourse */
	glColor4d(1.0,1.0,0.5,0.3);
	glLineWidth(0.1);
	if ((x+1) < xSize) DrawTimecourseLine(x+1,y,z,drawArea,max);
	if ((x-1) >= 0) DrawTimecourseLine(x-1,y,z,drawArea,max);
	if ((y+1) < ySize) DrawTimecourseLine(x,y+1,z,drawArea,max);
	if ((y-1) >= 0) DrawTimecourseLine(x,y-1,z,drawArea,max);
	if ((z+1) < zSize) DrawTimecourseLine(x,y,z+1,drawArea,max);
	if ((z-1) >= 0) DrawTimecourseLine(x,y,z-1,drawArea,max);

	/* overlay the current time course */
	glColor3d(1.0,1.0,0.0);
	glLineWidth(2.0);
	DrawTimecourseLine(x,y,z,drawArea,max);

	///* display current point */
	//string = wxString::Format("Timecourse at (%d,%d,%d)",x,y,z);
	//DrawShadowText(string,(width/2) + 16,15,1.0,1.0,1.0,10);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_BLEND);
}


// --------------------------------------------------------
// ---------- DrawTimecourseLine --------------------------
// --------------------------------------------------------
void ImageVolume::DrawTimecourseLine(int x, int y, int z, wxRect drawArea, double max)
{
	int i;
	int t = imgData->imgInfo->GetVolumeTSize();
	int px, py;
	double value;

	value = 0.0;
	glBegin(GL_LINE_STRIP);
//		glVertex2d(width/2,avgline); /* draw first point */
		for (i=0;i<t;i++) {
			switch (imgData->imgInfo->GetPixelType()) {
				case IMG_UINT8: value = imgData->imgDataUINT8->Get(x,y,z,i,0); break;
				case IMG_INT8: value = imgData->imgDataINT8->Get(x,y,z,i,0); break;
				case IMG_UINT16: value = imgData->imgDataUINT16->Get(x,y,z,i,0); break;
				case IMG_INT16: value = imgData->imgDataINT16->Get(x,y,z,i,0); break;
				case IMG_UINT32: value = imgData->imgDataUINT32->Get(x,y,z,i,0); break;
				case IMG_INT32: value = imgData->imgDataINT32->Get(x,y,z,i,0); break;
			}
			py = (drawArea.GetHeight() * (value/max));
			px = (drawArea.GetWidth() * ((double)i/(double)t)) + drawArea.GetLeft();
			//WriteLog(msg.Format("Point at (%d,%d)\n",px,py));
			glVertex2d(px,py);
		}
	glEnd();
}


// --------------------------------------------------------
// ---------- Draw3Plane ----------------------------------
// --------------------------------------------------------
void ImageVolume::Draw3Plane(int x, int y, int z)
{
	/* this draws a single view where all three planes intersect */

	int width = canvasSize.GetWidth();
	int height = canvasSize.GetHeight();
	double zoom;
	wxRect drawArea;
	wxRealPoint bottomLeft, offset;
	wxSize size;

	/* setup the area in which the planes will be drawn */
	drawArea = wxRect((int)(width/2),0,(int)(width/2),(int)(height/2));

	offset.x = 0.0;
	offset.y = 0.0;
	size.Set((int)(width/2), (int)(height/2));

	bottomLeft = wxRealPoint((double)(width/2),0.0);
	zoom = 100.0;

	ImageDisplayListItem img = ImageDisplayListItem(2, y, zoom, size, offset, bottomLeft, 0, mainVars->Toolbar_showHighRes(), 0, false, true);
	imgList.Add(img);
	img = ImageDisplayListItem(1, x, zoom, size, offset, bottomLeft, 0, mainVars->Toolbar_showHighRes(), 0, false, true);
	imgList.Add(img);
	img = ImageDisplayListItem(0, z, zoom, size, offset, bottomLeft, 0, mainVars->Toolbar_showHighRes(), 0, false, true);
	imgList.Add(img);
	DrawImage();
}


// --------------------------------------------------------
// ---------- Draw3ViewInfoOverlay ------------------------
// --------------------------------------------------------
void ImageVolume::Draw3ViewInfoOverlay()
{
	WriteLog("ImageVolume->Draw3ViewInfoOverlay() called...\n");

	wxString string;
	int width, height;
	int stringWidth;
	int borderWidth;
	int i;
	int x,y,z;
	wxPoint min, max;
	wxRealPoint imgCenter;
	wxString str;

	width = canvasSize.GetWidth();
	height = canvasSize.GetHeight();
	borderWidth = 4;

	/* display current level */
	string = wxString::Format("L: %d", mainVars->Display_windowCenter());
	stringWidth = GetDrawnStringWidth(string, 12);
	DrawShadowText(string,width-20-stringWidth,20,1.0,1.0,1.0,12);
	/* display current width */
	string = wxString::Format("W: %d", mainVars->Display_windowWidth());
	stringWidth = GetDrawnStringWidth(string, 12);
	DrawShadowText(string,width-20-stringWidth,35,1.0,1.0,1.0,12);
	/* display current frames per second */
	string = wxString::Format("fps: %.1f", fps);
	stringWidth = GetDrawnStringWidth(string, 12);
	DrawShadowText(string,width-20-stringWidth,50,1.0,1.0,1.0,12);

	/* if its active, draw a box around the image */
	if (isActive) {
		glColor3d(2.0/255.0, 42.0/255.0, 114.0/255.0);
		glBegin(GL_QUADS);
			glVertex2d(0, 0);
			glVertex2d(borderWidth, 0);
			glVertex2d(borderWidth, height);
			glVertex2d(0, height);

			glVertex2d(0, 0);
			glVertex2d(0, borderWidth);
			glVertex2d(width, borderWidth);
			glVertex2d(width, 0);

			glVertex2d(0, height);
			glVertex2d(0, height-borderWidth);
			glVertex2d(width, height-borderWidth);
			glVertex2d(width, height);

			glVertex2d(width, 0);
			glVertex2d(width-borderWidth, 0);
			glVertex2d(width-borderWidth, height);
			glVertex2d(width, height);

			/* middle vertical line */
			glVertex2d(width/2 - borderWidth/2, 0);
			glVertex2d(width/2 + borderWidth/2, 0);
			glVertex2d(width/2 + borderWidth/2, height);
			glVertex2d(width/2 - borderWidth/2, height);

			/* middle horizontal line */
			glVertex2d(0, height/2 - borderWidth/2);
			glVertex2d(0, height/2 + borderWidth/2);
			glVertex2d(width, height/2 + borderWidth/2);
			glVertex2d(width, height/2 - borderWidth/2);
		glEnd();
	}

	/* yellowish highlight around the current quadrant */
	glColor3d(212.0/255.0, 191.0/255.0, 49.0/255.0);
	glLineWidth(borderWidth);
	glBegin(GL_LINES);
	switch (mainVars->Image_plane()) {
		/* lower left */
		case 0:
			glVertex2d(borderWidth/2, 0);							glVertex2d(borderWidth/2, height/2 + borderWidth/2);
			glVertex2d(0, height/2);								glVertex2d(width/2 + borderWidth/2, height/2);
			glVertex2d(borderWidth/2, borderWidth/2);				glVertex2d(width/2 + borderWidth/2, borderWidth/2);
			glVertex2d(width/2, height/2);							glVertex2d(width/2, borderWidth/2);
			break;
		/* upper right */
		case 1:
			glVertex2d(width/2, height/2 - borderWidth/2);			glVertex2d(width/2, height);
			glVertex2d(width/2, height - borderWidth/2);			glVertex2d(width, height - borderWidth/2);
			glVertex2d(width/2, height/2);							glVertex2d(width, height/2);
			glVertex2d(width - borderWidth/2, height);				glVertex2d(width - borderWidth/2, height/2);
			break;
		/* upper left */
		case 2:
			glVertex2d(borderWidth/2, height/2 - borderWidth/2);	glVertex2d(borderWidth/2, height);
			glVertex2d(borderWidth/2, height - borderWidth/2);		glVertex2d(width/2, height - borderWidth/2);
			glVertex2d(borderWidth/2, height/2);					glVertex2d(width/2, height/2);
			glVertex2d(width/2, height);							glVertex2d(width/2, height/2 - borderWidth/2);
			break;
	}
	glEnd();

	glLineWidth(1.0);

	/* points 1 & 2 for the x and y lines */
	wxPoint sliceHighlight, p1x, p1y, p2x, p2y;

	/* draw some bounding boxes for the 3 planes */
	for (i=0;i<3;i++) {
		glColor3d(1.0,1.0,1.0);
		min = drawnPlaneInfo[i].minmin;
		max = drawnPlaneInfo[i].maxmax;
		imgCenter = drawnPlaneInfo[i].imageOrigin;
		glBegin(GL_LINES);
			glVertex2d(min.x,min.y); glVertex2d(max.x,min.y);
			glVertex2d(max.x,min.y); glVertex2d(max.x,max.y);
			glVertex2d(max.x,max.y); glVertex2d(min.x,max.y);
			glVertex2d(min.x,max.y); glVertex2d(min.x,min.y);
		glEnd();
//		str = str.Format("(%d,%d)",min.x, min.y);
//		DrawShadowText(str,min.x,min.y,1.0,0.0,0.0,10);
//		str = str.Format("(%d,%d)",min.x, max.y);
//		DrawShadowText(str,min.x,max.y,1.0,0.0,0.0,10);
//		str = str.Format("(%d,%d)",max.x, max.y);
//		DrawShadowText(str,max.x,max.y,1.0,0.0,0.0,10);
//		str = str.Format("(%d,%d)",max.x, min.y);
//		DrawShadowText(str,max.x,min.y,1.0,0.0,0.0,10);

		if (i == 0) {
			sliceHighlight.x = min.x + drawnPlaneInfo[1].sliceNum*((double)drawnPlaneInfo[0].zoom/100.0);
			sliceHighlight.y = min.y + drawnPlaneInfo[2].sliceNum*((double)drawnPlaneInfo[0].zoom/100.0);
			/* calculate x slice line */
			p1x.x = sliceHighlight.x;
			p2x.x = p1x.x;
			p1x.y = 0;
			p2x.y = height/2;
			/* calculate y slice line */
			p1y.y = sliceHighlight.y;
			p2y.y = p1y.y;
			p1y.x = 0;
			p2y.x = width/2;
		}
		if (i == 1) {
			sliceHighlight.x = min.x + drawnPlaneInfo[0].sliceNum*((double)drawnPlaneInfo[1].zoom/100.0);
			sliceHighlight.y = min.y + drawnPlaneInfo[2].sliceNum*((double)drawnPlaneInfo[1].zoom/100.0);
			/* calculate x slice line */
			p1x.x = sliceHighlight.x;
			p2x.x = p1x.x;
			p1x.y = height/2;
			p2x.y = height;
			/* calculate y slice line */
			p1y.y = sliceHighlight.y;
			p2y.y = p1y.y;
			p1y.x = width/2;
			p2y.x = width;
		}
		if (i == 2) {
			sliceHighlight.x = min.x + drawnPlaneInfo[0].sliceNum*((double)drawnPlaneInfo[2].zoom/100.0);
			sliceHighlight.y = min.y + drawnPlaneInfo[1].sliceNum*((double)drawnPlaneInfo[2].zoom/100.0);
			/* calculate x slice line */
			p1x.x = p2x.x = sliceHighlight.x; /* x coordinate is constant */
			p1x.y = height/2;
			p2x.y = height;
			/* calculate y slice line */
			p1y.y = p2y.y = sliceHighlight.y; /* y coordinate is constant */
			p1y.x = 0;
			p2y.x = width/2;

//			str = str.Format("Z slice: %d",drawnPlaneInfo[0].sliceNum);
//			DrawShadowText(str,p1x.x,p1x.y+height/4,1.0,0.0,0.0,10);
//			str = str.Format("X slice: %d",drawnPlaneInfo[1].sliceNum);
//			DrawShadowText(str,p1y.y,p1y.x+height/4,1.0,0.0,0.0,10);
		}

		/* draw the X slice line */
		glColor3d(0.0,1.0,0.0);
		glBegin(GL_LINES);
			glVertex2d(p1x.x,p1x.y); glVertex2d(p2x.x,p2x.y);
		glEnd();

		/* draw the Y slice line */
		glColor3d(0.0,0.0,1.0);
		glBegin(GL_LINES);
			glVertex2d(p1y.x,p1y.y); glVertex2d(p2y.x,p2y.y);
		glEnd();

		//string = wxString::Format("Center (%d,%d)",imgCenter.x, imgCenter.y);
		//DrawShadowText(string,imgCenter.x,imgCenter.y,1.0,1.0,1.0,10);
	}

	/* display current point */
	x = drawnPlaneInfo[1].sliceNum;
	y = drawnPlaneInfo[2].sliceNum;
	z = drawnPlaneInfo[0].sliceNum;
	string = wxString::Format("Timecourse at (%d,%d,%d)",x,y,z);
	DrawShadowText(string,(width/2) + 16,15,1.0,1.0,1.0,10);

	WriteLog("ImageVolume->Draw3ViewInfoOverlay() finished...\n");
}


// --------------------------------------------------------
// ---------- DrawSliceStack ------------------------------
// --------------------------------------------------------
void ImageVolume::DrawSliceStack()
{
	int i;
	wxSize size;
	wxRealPoint point;

	imgList.Clear();
	for (i=0;i<100;i++) {
		size.Set(canvasSize.GetWidth(), canvasSize.GetHeight());
		point = wxRealPoint::wxRealPoint((double)imgViews[viewID].xScreenOffset, (double)imgViews[viewID].yScreenOffset);
		
		ImageDisplayListItem img = ImageDisplayListItem(0, i, mainVars->Image_zoomPercent(), size, point, wxRealPoint(0.0, 0.0), i, mainVars->Toolbar_showHighRes(), 100, true, false);
		imgList.Add(img);
	}
	DrawImage();
}


// --------------------------------------------------------
// ---------- DrawImage -----------------------------------
// --------------------------------------------------------
void ImageVolume::DrawImage()
{
	bool hiRes;
	bool isStack;
	bool isTriPlane;
	float *colormap;
	GLuint mainTexture, lutTexture, colorMapTexture;
	GLint texSize;
	int bufferSize;
	int i;
	int alpha;
	int datatype;
	int numImages;
	int Size1, Size2;
	int XScale, YScale;
	int XX1, YY1, XX2, YY2;
	int plane, slice, zoom;
	int zOrder;
	int XSize, YSize, ZSize;
	int volNum;
	double X1, X2, X3, X4, Y1, Y2, Y3, Y4, Z1, Z2, Z3, Z4;
	double texX1, texX2, texY1, texY2;
	double multiplier;
	double maxImageVal, minImageVal;
	double minVal;
	double maxDisplayByteVal;
	double XSpacing, YSpacing, ZSpacing, Spacing1, Spacing2;
	double XScaling, YScaling, ZScaling;
	wxRealPoint screenCenter, imageCenter, centerOffset, bottomLeft;
	wxRealPoint Spacings;
	wxSize Sizes, screenSize;
	wxSize imageZoomedSize, imageOriginalSize, imageDisplaySize;
	wxSize textureSize;

	/* concurrency variables for the multithreading */
	wxMutex mutex;
	wxCondition condition(mutex);

	/* temporary buffers for use during deletion of buffer arrays */
	unsigned char *tmpBuffer8U;
	unsigned short int *tmpBuffer16U;
	unsigned int *tmpBuffer32U;
	float *tmpBufferFLOAT32;
	unsigned char *tmpBufferRGB;

	/* temp image thread for deletion section */
	ImageSliceThread *imgThread;

	/* initialize the variables */
	volNum = mainVars->Image_volume();
	Size1 = Size2 = 0;
	X1 = X2 = X3 = X4 = Y1 = Y2 = Y3 = Y4 = Z1 = Z2 = Z3 = Z4 = 0.0;

	/* used for the 3-plane view */
	XSize = imgData->imgInfo->GetVolumeXSize();
	YSize = imgData->imgInfo->GetVolumeYSize();
	ZSize = imgData->imgInfo->GetVolumeZSize();

	/* get the spacings... in millimeters for DICOM. usually mm for Analyze/Nifti */
	XSpacing = imgData->imgInfo->GetSpacing().x;
	YSpacing = imgData->imgInfo->GetSpacing().y;
	ZSpacing = imgData->imgInfo->GetSpacing().z;

	/* check if there is data to draw */
	if (imgData == NULL) WriteLog("DrawImage() - No data to draw!\n");
	else WriteLog("DrawImage() - Looks like there is data to draw!\n");

	/* get the maximum texture size supported on this machine */
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
	WriteLog( msg.Format("Max texture size: %d",texSize));

	/* setup the color map... whether or not its used is determined in the fragment shader */
	glGenTextures(1, &colorMapTexture);  /* GL_TEXTURE0 */
	glBindTexture(GL_TEXTURE_1D,colorMapTexture);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	colormap = frame->colorMaps->GetPointerToMap(mainVars->ColorMap_index());
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, texSize, 0, GL_RGBA, GL_FLOAT, colormap);

	/* setup the window level lookup table */
	glGenTextures(1, &lutTexture);  /* GL_TEXTURE1 */
	glBindTexture(GL_TEXTURE_1D,lutTexture);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	/* use the same grayscale gradient for all datatypes... since it's normalized to [0,1] in the shader */
	glTexImage1D(GL_TEXTURE_1D, 0, GL_LUMINANCE16, texSize, 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, imgHelper->lut16U);

	/* get maximum pixel value and calculate the multiplier to make the maximum value
	   match the maximum possible value for n bytes */
	if (imgData->imgInfo->GetPixelType() == IMG_RGB24) { maxImageVal = 255.0; minImageVal = 0.0; }
	else { maxImageVal = imgData->imgInfo->GetMaxPixelValues().Gray(); minImageVal = imgData->imgInfo->GetMinPixelValues().Gray(); }

	/* clear the thread list in case theres leftovers */
	/* ----- now loop through all images in the image list and setup the threads for them ----- */
	threads.Clear();
	numImages = (int)imgList.GetCount();
	WriteLog( msg.Format("DrawImage() - About to create %d threads...\n",numImages) );
	isStack = false;
	for (i=0; i<numImages; i++) {
		plane = imgList[i].plane;
		slice = imgList[i].slice;
		alpha = imgList[i].alpha;

		WriteLog( msg.Format("Working on plane %d, slice %d...\n",plane, slice) );

		/* determine the appropriate image sizes based on the plane */
		Sizes = GetImageSizes(plane);
		Size1 = Sizes.GetWidth();
		Size2 = Sizes.GetHeight();
		imageOriginalSize.SetWidth(Size1);
		imageOriginalSize.SetHeight(Size2);

		WriteLog( msg.Format("Plane %d original size (%dx%d)\n",plane, imageOriginalSize.GetWidth(), imageOriginalSize.GetHeight()) );

		/* get the appropriate 2^n size of the texture */
		textureSize = GetTextureSize(Size1,Size2);

		/* get the size of the buffer (number of pixels) */
		bufferSize = textureSize.GetHeight()*textureSize.GetWidth();

		WriteLog( msg.Format("Creating buffer of size %d...\n",bufferSize) );

		threads.Add(new ImageSliceThread(imgData, i, &mutex, &condition));
		/* signed and unsigned data use the same display datatype */
		switch (imgData->imgInfo->GetPixelType()) {
			case IMG_UINT8:
			case IMG_INT8:
				buffer8U.Add(new UINT_8[bufferSize]);
				threads[i]->SetupThread(imgData->imgInfo->GetPixelType(),buffer8U[i],bufferSize,imageOriginalSize, textureSize, plane, slice, isStack, volNum, minImageVal);
				break;
			case IMG_UINT16:
			case IMG_INT16:
				buffer16U.Add(new UINT_16[bufferSize]);
				threads[i]->SetupThread(imgData->imgInfo->GetPixelType(),buffer16U[i],bufferSize,imageOriginalSize, textureSize, plane, slice, isStack, volNum, minImageVal);
				break;
			case IMG_UINT32:
			case IMG_INT32:
				buffer32U.Add(new UINT_32[bufferSize]);
				threads[i]->SetupThread(imgData->imgInfo->GetPixelType(),buffer32U[i],bufferSize,imageOriginalSize, textureSize, plane, slice, isStack, volNum, minImageVal);
				break;
			case IMG_FLOAT32:
				bufferFLOAT32.Add(new FLOAT_32[bufferSize]);
				threads[i]->SetupThread(imgData->imgInfo->GetPixelType(),bufferFLOAT32[i],bufferSize,imageOriginalSize, textureSize, plane, slice, isStack, volNum, minImageVal);
				break;
			case IMG_RGB24:
				bufferRGB.Add(new UINT_8[bufferSize*3]);
				threads[i]->SetupThread(imgData->imgInfo->GetPixelType(),bufferRGB[i],bufferSize*3,imageOriginalSize, textureSize, plane, slice, isStack, volNum, minImageVal);
				break;
			default:
				WriteLog("DrawImage() - Not sure how we got this far, but the pixel type is not supported\n");
		}
		/* start all the threads ... */
		threads[i]->Run();
	}

	/* loop through all threads, run them, wait, and do the OpenGL stuff for them */
	for (i=0; i<numImages; i++) {
		zoom = imgList[i].zoom;
		plane = imgList[i].plane;
		slice = imgList[i].slice;
		screenSize = imgList[i].screenSize;
		centerOffset = imgList[i].centerOffset;
		bottomLeft = imgList[i].bottomLeft;
		hiRes = imgList[i].hiRes;
		zOrder = imgList[i].zOrder;
		isTriPlane = imgList[i].is3ViewSlice;

		if (isStack)
			datatype = IMG_RGBA24;
		else
			datatype = imgData->imgInfo->GetPixelType();

		/* check if it should be drawn with 'best fit' zoom */
		if ((mainVars->Image_zoomUseBestFit()) && (mainVars->Toolbar_displayType() != IMGVIEW_3VIEW))
			zoom = GetBestFitZoom(screenSize,wxSize(Size1,Size2));

		/* get original and new (zoomed) size */
		Sizes = GetImageSizes(plane);
		Spacings = GetImageSpacings(plane);
		Spacing1 = Spacings.x;
		Spacing2 = Spacings.y;
		Size1 = Sizes.GetWidth();
		Size2 = Sizes.GetHeight();
		imageOriginalSize.SetWidth(Size1);
		imageOriginalSize.SetHeight(Size2);
		imageZoomedSize.SetWidth((double)Size1 * (double)zoom/100.0);
		imageZoomedSize.SetHeight((double)Size2 * (double)zoom/100.0);
//		imageZoomedSize.SetWidth((double)Size1 * (double)zoom/100.0 * Spacing1);
//		imageZoomedSize.SetHeight((double)Size2 * (double)zoom/100.0 * Spacing2);

		/* set the screen center (center of drawing area) and image center (relative to screen) */
		screenCenter.x = screenSize.GetWidth()/2;
		screenCenter.y = screenSize.GetHeight()/2;
		imageCenter.x = screenCenter.x + centerOffset.x + bottomLeft.x;
		imageCenter.y = screenCenter.y + centerOffset.y + bottomLeft.y;

		/* get the appropriate 2^n size of the texture */
		textureSize = GetTextureSize(Size1,Size2);

		WriteLog( msg.Format("Texture size %dx%d for plane %d...\n",textureSize.GetWidth(), textureSize.GetHeight(),plane) );

		/* setup the mainTexture OpenGL stuff */
		glGenTextures(1, &mainTexture); /* GL_TEXTURE2 */
		glBindTexture(GL_TEXTURE_2D,mainTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if (hiRes) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}

		/* ...now wait for each thread to finish */
		threads[i]->Wait();
		WriteLog( msg.Format("Thread %d should be finished (within ImageVolume)...\n",i) );

		/* buffers should be filled after the thread has finished, so setup the display texture */
		switch (datatype) {
			case IMG_UINT8:
			case IMG_INT8:
				maxDisplayByteVal = pow(2.0, 8.0) - 1.0;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, textureSize.GetWidth(), textureSize.GetHeight(), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, buffer8U.Item(i));
				break;
			case IMG_UINT16:
			case IMG_INT16:
				maxDisplayByteVal = pow(2.0, 16.0) - 1.0;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE16, textureSize.GetWidth(), textureSize.GetHeight(), 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, buffer16U.Item(i));
				break;
			case IMG_UINT32:
			case IMG_INT32:
				maxDisplayByteVal = pow(2.0, 32.0) - 1.0;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE32F_ARB, textureSize.GetWidth(), textureSize.GetHeight(), 0, GL_LUMINANCE, GL_UNSIGNED_INT, buffer32U.Item(i));
				break;
			case IMG_FLOAT32:
				maxDisplayByteVal = pow(2.0, 32.0) - 1.0;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE32F_ARB, textureSize.GetWidth(), textureSize.GetHeight(), 0, GL_LUMINANCE, GL_FLOAT, bufferFLOAT32.Item(i));
				break;
			case IMG_RGB24:
				maxDisplayByteVal = pow(2.0, 8.0) - 1.0;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureSize.GetWidth(), textureSize.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, bufferRGB.Item(i));
				break;
			case IMG_RGBA24:
				maxDisplayByteVal = pow(2.0, 8.0) - 1.0;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureSize.GetWidth(), textureSize.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer8U.Item(i));
				break;
			default:
				WriteLog("DrawImage() - Not sure how we got this far, but the pixel type is not supported\n");
		}

		/* in case the number of bits stored is less than the byte boundary...
		   such as 12-bits stored in a 16-bit value */
		multiplier = (maxDisplayByteVal/(maxImageVal-minImageVal));
		minVal = (minImageVal/maxDisplayByteVal); /* normalize the min image value to [0 1] over the range of the display type (eg 16 bit [65536]) */
		
		/* now use the fragment shader program before doing opengl stuff to all the slices */
		glUseProgram(pWindow);

		/* Set up the shader parameters */
		glUniform1f(glGetUniformLocation(pWindow,"minVal"), (float)minVal);
		glUniform1f(glGetUniformLocation(pWindow,"multiplier"), (float)multiplier );
		if (datatype == IMG_RGB24) glUniform1i(glGetUniformLocation(pWindow,"useColorMap"), false);
		else glUniform1i(glGetUniformLocation(pWindow,"useColorMap"), mainVars->ColorMap_useColor());
		glUniform1i(glGetUniformLocation(pWindow,"useLUT"), mainVars->ColorMap_useWindowLevel());
		glUniform1i(glGetUniformLocation(pWindow,"mainTexture"), 2);
		glUniform1i(glGetUniformLocation(pWindow,"lutTexture"), 1);
		glUniform1i(glGetUniformLocation(pWindow,"colorMapTexture"), 0);

		/* bind the textures (need glActiveTexture() for bindings) */
		glEnable(GL_TEXTURE_1D);
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_1D,colorMapTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_1D,lutTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D,mainTexture);

		/* translate to the center of the screen, rotate and scale to zoom in/flip */
		glTranslatef((double)imageCenter.x, (double)imageCenter.y, 0.0);
		
		/* don't let the user rotate the orthogonal images */
		if ((mainVars->Toolbar_displayType() == IMGVIEW_3VIEW) && (!isTriPlane)) { }
		else {
			glRotatef((double)mainVars->Rotations_Z(),0.0,0.0,1.0);
			glRotatef((double)mainVars->Rotations_X(),1.0,0.0,0.0);
			glRotatef((double)mainVars->Rotations_Y(),0.0,1.0,0.0);
		}

		/* setup the scaling (zooming) and make Y negative if flipping */
		XScale = 1.0;
		if (mainVars->Toolbar_displayType() == IMGVIEW_3VIEW) YScale = 1.0;
		else YScale = -1.0;
		XScaling = (XScale * (double)zoom / 100.0);
		YScaling = (YScale * (double)zoom / 100.0);
//		XScaling = (XScale * (double)zoom / 100.0) * Spacing1;
//		YScaling = (YScale * (double)zoom / 100.0) * Spacing2;
		ZScaling = ((double)zoom/100.0);
		glScalef( XScaling, YScaling, ZScaling);

		/* useful later for drawing the scale marker */
		drawnPlaneInfo[plane].scale = XYZTriplet(XScaling, YScaling, ZScaling);

		/* setup the portion of the texture to be displayed */
		texY1 = 0.0;
		texY2 = (double)imageOriginalSize.GetHeight()/(double)textureSize.GetHeight();
		texX1 = 0.0;
		texX2 = (double)imageOriginalSize.GetWidth()/(double)textureSize.GetWidth();

		/* setup texture display-on-screen coordinates */
		X1 = 0.0 - (imageOriginalSize.GetWidth()/2);
		X2 = X1 + imageOriginalSize.GetWidth();
		Y1 = 0.0 - (imageOriginalSize.GetHeight()/2);
		Y2 = Y1 + imageOriginalSize.GetHeight();

		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		/* draw the polygon onto which the texture is mapped */
		if (isTriPlane) {
			//glShadeModel(GL_SMOOTH);
			glEnable(GL_DEPTH_TEST);
			switch (plane) {
				case 0: /* change the z-plane */
					X1 = X2 = 0.0; X3 = X4 = XSize;
					Y1 = Y4 = 0.0; Y2 = Y3 = YSize;
					Z1 = Z2 = Z3 = Z4 = slice;
					break;
				case 1: /* change the y-plane */
					Z1 = Z2 = 0.0; Z3 = Z4 = ZSize;
					Y1 = Y4 = 0.0; Y2 = Y3 = YSize;
					X1 = X2 = X3 = X4 = slice;
					break;
				case 2: /* change the x-plane */
					Z1 = Z2 = 0.0; Z3 = Z4 = ZSize;
					X1 = X4 = 0.0; X2 = X3 = XSize;
					Y1 = Y2 = Y3 = Y4 = slice;
					break;
			}
			glBegin(GL_QUADS);
				glTexCoord2f(texX1, texY1); glVertex3f(X1-(float)XSize/2.0, Y1-(float)YSize/2.0, Z1-(float)ZSize/2.0);
				glTexCoord2f(texX1, texY2); glVertex3f(X2-(float)XSize/2.0, Y2-(float)YSize/2.0, Z2-(float)ZSize/2.0);
				glTexCoord2f(texX2, texY2); glVertex3f(X3-(float)XSize/2.0, Y3-(float)YSize/2.0, Z3-(float)ZSize/2.0);
				glTexCoord2f(texX2, texY1); glVertex3f(X4-(float)XSize/2.0, Y4-(float)YSize/2.0, Z4-(float)ZSize/2.0);
			glEnd();
			glDisable(GL_DEPTH_TEST);
		}
		else {
			/* regular old 2D texture drawing */
			glDisable(GL_DEPTH_TEST);
			glBegin(GL_QUADS);
				glTexCoord2f(texX1, texY1); glVertex3f(X1, Y1, (float)zOrder);
				glTexCoord2f(texX1, texY2); glVertex3f(X1, Y2, (float)zOrder);
				glTexCoord2f(texX2, texY2); glVertex3f(X2, Y2, (float)zOrder);
				glTexCoord2f(texX2, texY1); glVertex3f(X2, Y1, (float)zOrder);
			glEnd();
		}
		glDisable(GL_BLEND);
		/* restore the default fixed pipeline */
		glUseProgram(0);

		/* delete the main texture */
		glDeleteTextures(1, &mainTexture);

		/* clean up the OpenGL stuff */
		glDisable(GL_TEXTURE_1D);
		glDisable(GL_TEXTURE_2D);

		/* one last drawing step... after the default pipeline is restored */
		if (isTriPlane) {
			//glShadeModel(GL_SMOOTH);
			glEnable(GL_DEPTH_TEST);
			DrawTriPlaneLines();
		}
		glDisable(GL_DEPTH_TEST);

		/* last step, restore the identity matrix, to get rid of all the transformations */
		glLoadIdentity();

		//if ((mainVars->Toolbar_displayType() == IMGVIEW_3VIEW) && (!isTriPlane)) {
			/* determine exactly where things were drawn, in terms of the monitor space (eg. 1024x768) */
			XX1 = 0.0 - (imageZoomedSize.GetWidth()/2) + imageCenter.x;
			XX2 = XX1 + imageZoomedSize.GetWidth();
			YY1 = 0.0 - (imageZoomedSize.GetHeight()/2) + imageCenter.y;
			YY2 = YY1 + imageZoomedSize.GetHeight();
			drawnPlaneInfo[plane].imageOrigin = imageCenter;
			drawnPlaneInfo[plane].sliceNum = slice;
			drawnPlaneInfo[plane].minmin = wxPoint(XX1, YY1);
			drawnPlaneInfo[plane].maxmax = wxPoint(XX2, YY2);
			drawnPlaneInfo[plane].zoom = zoom;
			if (YScale < 0.0) /* Y axis is inverted, so invert the Y points */
				drawnPlaneInfo[plane].flipY = true;
			WriteLog( msg.Format("DrawImage() drew plane %d at (%d,%d),(%d,%d) centered at (%d,%d)\n",plane, XX1,YY1,XX2,YY2,imageCenter.x,imageCenter.y) );
		//}
	}
	/* delete any threads that were created */
	for (i=0;i<(int)threads.Count();i++) { imgThread = threads.Item(i); delete imgThread; } threads.Clear();

	/* delete the lookup table textures (dont need glActiveTexture() for deletes) */
	glDeleteTextures(1, &colorMapTexture);
	glDeleteTextures(1, &lutTexture);

	/* restore the default fixed pipeline */
	glUseProgram(0);

	/* clear all the buffers. each object must be detached and deleted individually... Clear() won't do that */
	for (i=0;i<(int)buffer8U.Count();i++) { tmpBuffer8U = buffer8U.Item(i); delete[] tmpBuffer8U; } buffer8U.Clear();
	for (i=0;i<(int)buffer16U.Count();i++) { tmpBuffer16U = buffer16U.Item(i); delete[] tmpBuffer16U; } buffer16U.Clear();
	for (i=0;i<(int)buffer32U.Count();i++) { tmpBuffer32U = buffer32U.Item(i); delete[] tmpBuffer32U; } buffer32U.Clear();
	for (i=0;i<(int)bufferFLOAT32.Count();i++) { tmpBufferFLOAT32 = bufferFLOAT32.Item(i); delete[] tmpBufferFLOAT32; } bufferFLOAT32.Clear();
	for (i=0;i<(int)bufferRGB.Count();i++) { tmpBufferRGB = bufferRGB.Item(i); delete[] tmpBufferRGB; } bufferRGB.Clear();

//	WriteLog( msg.Format("DrawImage() - Threads should've been cleared, current count: %d\n",threads.GetCount()));
}


// --------------------------------------------------------
// ---------- DrawVolumeRendering -------------------------
// --------------------------------------------------------
void ImageVolume::DrawVolumeRendering()
{
	/* make sure no other drawing takes place until this function is complete */
	readyToDraw = false;

	if (mainVars->VolRend_method() == VOLMETHOD_3DTEXTURE) {
		XYZTriplet rotation3D;
		if (imgVolRender == NULL)
			imgVolRender = new ImageVolumeRender(imgData, imgHelper, mainVars, mainVars->Toolbar_displayType(), wxColor(255,255,255), 0);
		volRendInitialized = true;
		rotation3D.SetXYZ(mainVars->Rotations_X(), mainVars->Rotations_Y(),mainVars->Rotations_Z());
		imgVolRender->SetGLShaderProgram(pRayTrace);
		imgVolRender->SetCanvasSize(canvasSize);
		imgVolRender->SetParams(rotation3D, mainVars->Rotations_angle(), mainVars->VolRend_intensity(), mainVars->VolRend_density(), mainVars->VolRend_numSlices());
		if (volRendInitialized)
			imgVolRender->Draw(mainVars->VolRend_reInit());
	}
	else { /* the ray trace method */
		XYZTriplet rotation3D;
		if (imgVolRenderRay == NULL)
			imgVolRenderRay = new ImageVolumeRenderRay(imgData, imgHelper, mainVars, imgCanvas, frame);
		volRendInitialized = true;
		rotation3D.SetXYZ(mainVars->Rotations_X(), mainVars->Rotations_Y(),mainVars->Rotations_Z());
		imgVolRenderRay->SetGLShaderProgram(pRayTrace, pRayTraceSSD);
		imgVolRenderRay->SetCanvasSize(canvasSize);
		if (volRendInitialized)
			imgVolRenderRay->Draw(mainVars->VolRend_reInit());
	}

	/* everything is done, so now its ok to draw again */
	readyToDraw = true;
}


// --------------------------------------------------------
// ---------- LoadFile ------------------------------------
// --------------------------------------------------------
int ImageVolume::LoadFile()
{
	imgData->SetLoadParameters(mainVars->Load_filetype(), mainVars->Load_filenames(), mainVars->Load_loadType(), mainVars->Load_isMosaic(), mainVars->Load_mosaicXSize(), mainVars->Load_mosaicYSize(), mainVars->Load_mosaicNumSlices());
	//WriteLog("ImageVolume->imgData and imgHelper created...\n");
	if (!imgData->Load()) {
		WriteLog("Image data not successfuly loaded in ImageVolume->Load()...\n");
		return 0;
	}
	WriteLog("Image data successfuly loaded in ImageVolume->Load()...\n");

	/* check for valid Window and Center values */

	if (imgData->imgInfo->GetPixelType() == IMG_RGB24) {
		if (imgData->imgInfo->GetMaxPixelValues().Red() < 0) {
			readyToDraw = false;
			return 0;
		}
		mainVars->Display_windowCenter((int)(imgData->imgInfo->GetMaxPixelValues().Red()/2));
		mainVars->Display_windowWidth((int)(imgData->imgInfo->GetMaxPixelValues().Red()/2));
	}
	else {
		if (imgData->imgInfo->GetMaxPixelValues().Gray() < 0) {
			readyToDraw = false;
			return 0;
		}
		mainVars->Display_windowCenter((int)(imgData->imgInfo->GetMaxPixelValues().Gray()/2));
		mainVars->Display_windowWidth((int)(imgData->imgInfo->GetMaxPixelValues().Gray()/2));
	}
	//WriteLog("Done setting default window center/width, now calling calc lookup table...\n");

	bool isRGB;
	if (imgData->imgInfo->GetPixelType() == IMG_RGB24)
		isRGB = true;
	else
		isRGB = false;

	imgHelper->CalculateValueLookupTable(mainVars->Display_windowWidth(), mainVars->Display_windowCenter(), imgData->imgInfo->GetMinPixelValues().Gray(), imgData->imgInfo->GetMaxPixelValues().Gray(), isRGB);
	readyToDraw = true;

	return 1;
}


// --------------------------------------------------------
// ---------- GetImageSizes -------------------------------
// --------------------------------------------------------
wxSize ImageVolume::GetImageSizes(int plane)
{
	int XSize = imgData->imgInfo->GetVolumeXSize();
	int YSize = imgData->imgInfo->GetVolumeYSize();
	int ZSize = imgData->imgInfo->GetVolumeZSize();
	int Size1,Size2;

	/* set the image dimensions for the selected plane */
	switch (plane) {
		case 0: /* change the z-plane */
			Size1 = XSize;
			Size2 = YSize;
			break;
		case 1: /* change the y-plane */
			Size1 = ZSize;
			Size2 = YSize;
			break;
		case 2: /* change the x-plane */
			Size1 = ZSize;
			Size2 = XSize;
			break;
		default: /* change the z-plane */
			Size1 = XSize;
			Size2 = YSize;
			break;
	}

	return wxSize(Size1,Size2);
}


// --------------------------------------------------------
// ---------- GetImageSpacings ----------------------------
// --------------------------------------------------------
wxRealPoint ImageVolume::GetImageSpacings(int plane)
{
	double XSpace = imgData->imgInfo->GetSpacing().x;
	double YSpace = imgData->imgInfo->GetSpacing().y;
	double ZSpace = imgData->imgInfo->GetSpacing().z;
	double Spacing1,Spacing2;

	/* set the image dimensions for the selected plane
	   also take into account the voxel size in mm */
	switch (plane) {
		case 0: /* change the z-plane */
			Spacing1 = XSpace;
			Spacing2 = YSpace;
			break;
		case 1: /* change the y-plane */
			Spacing1 = ZSpace;
			Spacing2 = YSpace;
			break;
		case 2: /* change the x-plane */
			Spacing1 = ZSpace;
			Spacing2 = XSpace;
			break;
		default: /* change the z-plane */
			Spacing1 = XSpace;
			Spacing2 = YSpace;
			break;
	}

	return wxRealPoint(Spacing1,Spacing2);
}


// --------------------------------------------------------
// ---------- GetTextureSize ------------------------------
// --------------------------------------------------------
wxSize ImageVolume::GetTextureSize(int Size1, int Size2)
{
	bool found;
	int n;
	wxSize theSize;

	/* get the width of the texture */
	n = 0; found = false;
	while (!found) {
		if (Size1 <= pow(2.0,n)) {
			theSize.SetWidth(pow(2.0,n));
			found = true;
		}
		n++;
	}

	/* get the height of the texture */
	n = 0; found = false;
	while (!found) {
		if (Size2 <= pow(2.0,n)) {
			theSize.SetHeight(pow(2.0,n));
			found = true;
		}
		n++;
	}

	return theSize;
}


// --------------------------------------------------------
// ---------- DrawTriPlaneLines ---------------------------
// --------------------------------------------------------
void ImageVolume::DrawTriPlaneLines()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glColor3f(1.0, 1.0, 0.0);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(0.5);

	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, -200.0); glVertex3f(0.0, 0.0, 200.0);
		glVertex3f(-200.0, 0.0, 0.0); glVertex3f(200.0, 0.0, 0.0);
		glVertex3f(0.0, -200.0, 0.0); glVertex3f(0.0, 200.0, 0.0);
	glEnd();
	glDisable(GL_BLEND);
}


// --------------------------------------------------------
// ---------- DrawScaleBar --------------------------------
// --------------------------------------------------------
void ImageVolume::DrawScaleBar(int plane)
{
	int width = canvasSize.GetWidth();
	int height = canvasSize.GetHeight();
	wxString string;
//	double zoom = (double)imgViews[viewID].zoomFactor;
	double zoom = (double)mainVars->Image_zoomPercent();
	zoom = zoom/100.0;

//	double Spacing1, Spacing2;
	wxRealPoint Spacings;

	Spacings = GetImageSpacings(plane);

	glColor3d(1.0, 1.0, 1.0);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(0.1);

	glTranslatef((double)width*0.90, (double)height/2.0-(10*zoom), 0.0);

	glBegin(GL_LINES);
		glVertex3f(0.0,0.0,0.0); glVertex3f(10.0, 0.0, 0.0);
		if (zoom > 1.0) {
			glVertex3f(0.0,2.0*zoom,0.0); glVertex3f(10.0, 2.0*zoom, 0.0);
			glVertex3f(0.0,4.0*zoom,0.0); glVertex3f(10.0, 4.0*zoom, 0.0);
			glVertex3f(0.0,6.0*zoom,0.0); glVertex3f(10.0, 6.0*zoom, 0.0);
			glVertex3f(0.0,8.0*zoom,0.0); glVertex3f(10.0, 8.0*zoom, 0.0);
			glVertex3f(0.0,10.0*zoom,0.0); glVertex3f(10.0, 10.0*zoom, 0.0);
			glVertex3f(0.0,12.0*zoom,0.0); glVertex3f(10.0, 12.0*zoom, 0.0);
			glVertex3f(0.0,14.0*zoom,0.0); glVertex3f(10.0, 14.0*zoom, 0.0);
			glVertex3f(0.0,16.0*zoom,0.0); glVertex3f(10.0, 16.0*zoom, 0.0);
			glVertex3f(0.0,18.0*zoom,0.0); glVertex3f(10.0, 18.0*zoom, 0.0);
		}
		glVertex3f(0.0,20.0*zoom,0.0); glVertex3f(10.0, 20.0*zoom, 0.0);

		glVertex3f(10.0,0.0*zoom,0.0); glVertex3f(10.0, 20.0*zoom, 0.0);
	glEnd();

	DrawShadowText("20 mm",0, (int)(20.0*zoom) + 5,1.0,1.0,1.0,10);
	DrawShadowText("0 mm",0, -12,1.0,1.0,1.0,10);

	glDisable(GL_LINE_SMOOTH);
	glLineWidth(1.0);
	glLoadIdentity();
}


// --------------------------------------------------------
// ---------- GetBestFitZoom ------------------------------
// --------------------------------------------------------
int ImageVolume::GetBestFitZoom(wxSize windowSize, wxSize imageSize)
{
	int newzoom;

	/* ...and the new zoom percentage */
	double ratio1 = (double)windowSize.GetWidth()/(double)imageSize.GetWidth();
	double ratio2 = (double)windowSize.GetHeight()/(double)imageSize.GetHeight();
	if (ratio1 < ratio2)
		newzoom = (int)(100.0*ratio1);
	else
		newzoom = (int)(100.0*ratio2);

	frame->SetZoom(newzoom);

	WriteLog(msg.Format("windowSize (%d,%d), imageSize (%d,%d), newZoom %d", windowSize.GetWidth(), windowSize.GetHeight(), imageSize.GetWidth(), imageSize.GetHeight(), newzoom));

	return newzoom;
}


// --------------------------------------------------------
// ---------- DrawColorBar --------------------------------
// --------------------------------------------------------
void ImageVolume::DrawColorBar()
{
	WriteLog("ImageVolume->DrawContrastBrightnessBar() called...\n");

	int texSize;
	int height = canvasSize.GetHeight();
	GLuint colorMapTexture, lutTexture, mainTexture;
	float X1,X2,Y1,Y2;
	float *colormap;
	double maxPixel16U = pow(2.0, 16.0) - 1.0;
	double barheight, barstart;

	barheight = height * 0.75;
	barstart = (height - barheight)/2;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);

	/* setup the color map... whether or not its used is determined in the fragment shader */
	glGenTextures(1, &colorMapTexture);  /* GL_TEXTURE0 */
	glBindTexture(GL_TEXTURE_1D,colorMapTexture);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	colormap = frame->colorMaps->GetPointerToMap(mainVars->ColorMap_index());
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, texSize, 0, GL_RGBA, GL_FLOAT, colormap);

	/* setup the window level lookup table */
	glGenTextures(1, &lutTexture);  /* GL_TEXTURE1 */
	glBindTexture(GL_TEXTURE_1D,lutTexture);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	/* use the same grayscale gradient for all datatypes... since it's normalized to [0,1] in the shader */
	glTexImage1D(GL_TEXTURE_1D, 0, GL_LUMINANCE16, texSize, 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, imgHelper->lut16U);

	glGenTextures(1, &mainTexture);  /* GL_TEXTURE2 */
	glBindTexture(GL_TEXTURE_2D,mainTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	colormap = frame->colorMaps->GetPointerToMap(mainVars->ColorMap_index());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texSize, 1, 0, GL_RGBA, GL_FLOAT, colormap);

	/* now use the fragment shader program before doing opengl stuff to all the slices */
	glUseProgram(pWindow);

	/* Set up the shader parameters */
	glUniform1f(glGetUniformLocation(pWindow,"minVal"), 0.0);
	glUniform1f(glGetUniformLocation(pWindow,"multiplier"), 1.0);
	glUniform1i(glGetUniformLocation(pWindow,"useColorMap"), false);
	glUniform1i(glGetUniformLocation(pWindow,"useLUT"), true);
	glUniform1i(glGetUniformLocation(pWindow,"mainTexture"), 2); /* colorMapTexture */
	glUniform1i(glGetUniformLocation(pWindow,"lutTexture"), 1); /* same old window level lookup texture */
	glUniform1i(glGetUniformLocation(pWindow,"colorMapTexture"), 0); /* none */

	/* bind the textures (need to use glActiveTexture() for bindings) */
	glEnable(GL_TEXTURE_1D);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D,colorMapTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D,lutTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,mainTexture);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glColor4f(0.0,0.0,0.0,1.0);

	X1 = 15.0;
	Y1 = barstart;
	Y2 = barstart + barheight;
	X2 = 30.0;
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(X1, Y1, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(X1, Y2, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(X2, Y2, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(X2, Y1, 0.0);
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	/* delete the textures (don't need to use glActiveTexture() for deletes) */
	glDeleteTextures(1, &mainTexture);
	glDeleteTextures(1, &colorMapTexture);
	glDeleteTextures(1, &lutTexture);

	/* clean up the OpenGL stuff */
	glDisable(GL_TEXTURE_1D);
	glDisable(GL_TEXTURE_2D);

	/* restore the default fixed pipeline */
	glUseProgram(0);

	/* last step, restore the identity matrix, to get rid of all the transformations */
	glLoadIdentity();
}


// --------------------------------------------------------
// ---------- GetOrientation ------------------------------
// --------------------------------------------------------
wxString ImageVolume::GetOrientation(double x, double y, double z) //Vector3D vector)
{
	/* this code was adapted from Dave Clunie's webpage:
	http://www.dclunie.com/medical-image-faq/html/part2.html
	which is an excellent source for DICOM related information */

	/* this function returns the landmark that the vector is pointing to,
	   for example; if you pass the row cosines and it returns "F"
	   that means "the row in the positive direction is pointing toward the foot"
	*/

	char orientationX, orientationY, orientationZ;
//	char *orientation[4];
	char *orientation=new char[4];
	char *optr = orientation;
	int i;
	double absX, absY, absZ;
	wxString orient;

	*optr='\0';

	orientationX = x < 0 ? 'R' : 'L';
	orientationY = y < 0 ? 'A' : 'P';
	orientationZ = z < 0 ? 'F' : 'H';

	absX = fabs(x);
	absY = fabs(y);
	absZ = fabs(z);

	for (i=0; i<3; ++i) {
		if (absX>.0001 && absX>absY && absX>absZ) {
			*optr++=orientationX;
			absX=0;
		}
		else if (absY>.0001 && absY>absX && absY>absZ) {
			*optr++=orientationY;
			absY=0;
		}
		else if (absZ>.0001 && absZ>absX && absZ>absY) {
			*optr++=orientationZ;
			absZ=0;
		}
		else break;
		*optr='\0';
	}
	orient = orientation;
	delete[] orientation;
	return orient;
}


// --------------------------------------------------------
// ---------- DrawOrientation -----------------------------
// --------------------------------------------------------
void ImageVolume::DrawOrientation()
{
	wxString stringXPos,stringYPos,stringZPos;
	wxString string1, string2, string3; /* string1 is at the top of the box, string2 is on the left */
	int height = canvasSize.GetHeight();
	int width = canvasSize.GetWidth();
	int x1,y1,x2,y2;
	int plane = mainVars->Image_plane();

	/* display orientation */
	stringXPos = GetOrientation(imgData->dcmInfo->GetRowCosines().x, imgData->dcmInfo->GetRowCosines().y, imgData->dcmInfo->GetRowCosines().z);
	stringYPos = GetOrientation(imgData->dcmInfo->GetColCosines().x, imgData->dcmInfo->GetColCosines().y, imgData->dcmInfo->GetColCosines().z);
	stringZPos = GetOrientation(imgData->dcmInfo->GetNormCosines().x, imgData->dcmInfo->GetNormCosines().y, imgData->dcmInfo->GetNormCosines().z);

	x1 = width - 80;
	x2 = x1 + 40;
	y1 = 100;
	y2 = 140;

	switch (plane) {
		case 0:
			string1 = stringXPos;
			string2 = stringYPos;
			string3 = stringZPos;
			break;
		case 1:
			string1 = stringZPos;
			string2 = stringYPos;
			string3 = stringXPos;
			break;
		case 2:
			string1 = stringZPos;
			string2 = stringXPos;
			string3 = stringYPos;
			break;
		default:
			string1 = stringXPos;
			string2 = stringYPos;
			string3 = stringZPos;
			break;
	}

	/* translate is to the side, and rotate the same way as the image */
	glTranslatef((double)(x1+20), (double)(y1+20), 0.0);
	glRotatef((double)mainVars->Rotations_Z(),0.0,0.0,1.0);
	glRotatef((double)mainVars->Rotations_X(),1.0,0.0,0.0);
	glRotatef((double)mainVars->Rotations_Y(),0.0,1.0,0.0);

	x1 = -20;
	x2 = x1 + 40;
	y1 = -20;
	y2 = y1 + 40;

	glColor3f(1.0,1.0,1.0);
	glLineWidth(0.5);
	glBegin(GL_LINES);
		glVertex3f(x1, y1, 0.0); glVertex3f(x1, y2, 0.0);
		glVertex3f(x1, y2, 0.0); glVertex3f(x2, y2, 0.0);
		glVertex3f(x2, y2, 0.0); glVertex3f(x2, y1, 0.0);
		glVertex3f(x2, y1, 0.0); glVertex3f(x1, y1, 0.0);
	glEnd();

	DrawShadowText(string1,x2+3,y1+17,1.0,1.0,1.0,10); /* right side */
	DrawShadowText(string2,x1+17,y1-10,1.0,1.0,1.0,10); /* bottom */
	DrawShadowText(string3,x1+17,y1+17,1.0,1.0,1.0,10); /* middle */

	glLineWidth(1.0);

	glLoadIdentity();
}