/* ----------------------------------------------------------------------------
	ImageVolumeRenderRay.cpp


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
//#include "LogWindow.h"
#include "XYZTriplet.h"
#include "ImageVolumeRenderRay.h"
#include "wx/progdlg.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "gl/glut.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImageVolumeRenderRay::ImageVolumeRenderRay(ImageData *imgdata, ImageVolumeHelper *imghelper, MainFrameVariables *mVars, ImageCanvas *imgCan, MainFrame *Frame)
{
	initFinished = false;
	//logWindow = logwind;
	imgData = imgdata;
	imgHelper = imghelper;
	mainVars = mVars;
	imgCanvas = imgCan;
	frame = Frame;

	Init();
}

ImageVolumeRenderRay::~ImageVolumeRenderRay()
{

}


void ImageVolumeRenderRay::SetGLShaderProgram(GLuint progReg, GLuint progSSD)
{
	WriteLog("ImageVolumeRenderRay->SetGLShaderProgram() called...\n");
	pRayTrace = progReg;
	pRayTraceSSD = progSSD;
}


// --------------------------------------------------------
// ---------- Init ----------------------------------------
// --------------------------------------------------------
void ImageVolumeRenderRay::Init(void)
{
	w = canvasSize.GetWidth();
	h = canvasSize.GetHeight();

	CreateVolumeTexture();

	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Setup our FBO
	glGenFramebuffersEXT(1, &fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

	// Create the render buffer for depth	
	glGenRenderbuffersEXT(1, &depthBuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, 512, 512);

	// Now setup a texture to render to
	glGenTextures(1, &backfaceBuffer);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, backfaceBuffer); /* GL_TEXTURE1 */
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB,  512, 512, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, backfaceBuffer, 0); // And attach it to the FBO so we can render to it

	// Now setup a texture to render to
	glGenTextures(1, &frontfaceBuffer);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, frontfaceBuffer); /* GL_TEXTURE1 */
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB,  512, 512, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, frontfaceBuffer, 0); // And attach it to the FBO so we can render to it

	/* the final texture which will be operated on by the fragment shader */
	glGenTextures(1, &finalImage);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, finalImage); /* GL_TEXTURE2 */
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA16F_ARB, 512, 512, 0, GL_RGBA, GL_FLOAT, NULL);

	// Attach the depth render buffer to the FBO as it's depth attachment
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer);

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
		exit(1);
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);	// Unbind the FBO for now
	initFinished = true;
}


// --------------------------------------------------------
// ---------- CreateVolumeTexture -------------------------
// --------------------------------------------------------
void ImageVolumeRenderRay::CreateVolumeTexture(void)
{    
	WriteLog("ImageVolumeRenderRay->CreateVolumeTexture() called...\n");

	int iWidth = imgData->imgInfo->GetVolumeXSize();
	int iHeight = imgData->imgInfo->GetVolumeYSize();
	int iDepth = imgData->imgInfo->GetVolumeZSize();
	int bufferSize;
	int index;
	int maxVal, minVal;
	int i,j,k;
	int n;
	bool found;
	double maxSize;
	int larger, largest;
	int textureSize;

	/* determine the largest side of the cube, make all sides that size */
	if (iWidth > iHeight) larger = iWidth;
	else larger = iHeight;
	
	if (larger > iDepth) largest = larger;
	else largest = iDepth;

	/* get the width of the texture */
	n = 0; found = false;
	while (!found) {
		if (largest <= pow(2.0,n)) {
			textureSize = pow(2.0,n);
			found = true;
		}
		n++;
	}

	textureWidth = textureHeight = textureDepth = textureSize;

	/* number of dicom's to read is known, so setup the progress dialog */
	wxProgressDialog progDlg(wxT("Recalculating Volume"), wxT(" "), textureWidth, NULL, wxPD_APP_MODAL | wxPD_SMOOTH);

	/* create the buffer */
	bufferSize = textureWidth*textureHeight*textureDepth;

	maxVal = (int)imgData->imgInfo->GetMaxPixelValues().Gray();
	minVal = (int)imgData->imgInfo->GetMinPixelValues().Gray();

	index = 0;
	/* transform the imageData to contain an alpha component */
	switch (imgData->imgInfo->GetPixelType()) {
		case IMG_UINT8:   /* --- 1 byte   8 bits  --- */
			maxSize = pow(2.0, 8.0) - 1.0;
			drawBuffer8U = new unsigned char[bufferSize];
			WriteLog("Pixel type 8U\n");
			for (i=0; i<textureWidth; i++) {
				progDlg.Update(i,msg.Format("Slice %d of %d",i,textureWidth));
				for (j=0; j<textureHeight; j++) {
					for (k=0; k<textureDepth; k++) {
						if ( (i < iWidth) && (j < iHeight) && (k < iDepth))
							drawBuffer8U[index] = ((double)imgData->imgDataUINT8->Get(j,i,k,0,0)/(double)maxVal)*maxSize;
						else
							drawBuffer8U[index] = 0;
						index++;
					}
				}
			}
			break;
		case IMG_INT8:   /* --- 1 byte   8 bits  --- */
			maxSize = pow(2.0, 7.0) - 1.0;
			drawBuffer8S = new char[bufferSize];
			WriteLog("Pixel type 8S\n");
			for (i=0; i<textureWidth; i++) {
				progDlg.Update(i,msg.Format("Slice %d of %d",i,textureWidth));
				for (j=0; j<textureHeight; j++) {
					for (k=0; k<textureDepth; k++) {
						if ( (i < iWidth) && (j < iHeight) && (k < iDepth))
							drawBuffer8S[index] = ((double)imgData->imgDataINT8->Get(j,i,k,0,0)/(double)maxVal)*maxSize;
						else
							drawBuffer8S[index] = 0;
						index++;
					}
				}
			}
			break;
		case IMG_UINT16:  /* --- 2 bytes  16 bits --- */
			maxSize = pow(2.0, 16.0) - 1.0;
			drawBuffer16U = new unsigned short int[bufferSize];
			WriteLog("Pixel type 16U\n");
			for (i=0; i<textureWidth; i++) {
				progDlg.Update(i,msg.Format("Slice %d of %d",i,textureWidth));
				for (j=0; j<textureHeight; j++) {
					for (k=0; k<textureDepth; k++) {
						if ( (i < iWidth) && (j < iHeight) && (k < iDepth))
							drawBuffer16U[index] = ((double)imgData->imgDataUINT16->Get(j,i,k,0,0)/(double)maxVal)*maxSize;
						else
							drawBuffer16U[index] = 0;
						index++;
					}
				}
			}
			break;
		case IMG_INT16:  /* --- 2 bytes  16 bits --- */
			maxSize = pow(2.0, 15.0) - 1.0;
			drawBuffer16S = new short int[bufferSize];
			WriteLog("Pixel type 16S\n");
			for (i=0; i<textureWidth; i++) {
				progDlg.Update(i,msg.Format("Slice %d of %d",i,textureWidth));
				for (j=0; j<textureHeight; j++) {
					for (k=0; k<textureDepth; k++) {
						if ( (i < iWidth) && (j < iHeight) && (k < iDepth))
							drawBuffer16S[index] = ((double)imgData->imgDataINT16->Get(j,i,k,0,0)/(double)maxVal)*maxSize;
						else
							drawBuffer16S[index] = 0;
						index++;
					}
				}
			}
			break;
		case IMG_UINT32:  /* --- 4 bytes  32 bits --- */
			maxSize = pow(2.0, 32.0) - 1.0;
			drawBuffer32U = new unsigned int[bufferSize];
			WriteLog("Pixel type 32U\n");
			for (i=0; i<textureWidth; i++) {
				progDlg.Update(i,msg.Format("Slice %d of %d",i,textureWidth));
				for (j=0; j<textureHeight; j++) {
					for (k=0; k<textureDepth; k++) {
						if ( (i < iWidth) && (j < iHeight) && (k < iDepth))
							drawBuffer32U[index] = ((double)imgData->imgDataUINT32->Get(j,i,k,0,0)/(double)maxVal)*maxSize;
						else
							drawBuffer32U[index] = 0;
						index++;
					}
				}
			}
			break;
		case IMG_INT32:  /* --- 4 bytes  32 bits --- */
			maxSize = pow(2.0, 31.0) - 1.0;
			drawBuffer32S = new int[bufferSize];
			WriteLog("Pixel type 32S\n");
			for (i=0; i<textureWidth; i++) {
				progDlg.Update(i,msg.Format("Slice %d of %d",i,textureWidth));
				for (j=0; j<textureHeight; j++) {
					for (k=0; k<textureDepth; k++) {
						if ( (i < iWidth) && (j < iHeight) && (k < iDepth))
							drawBuffer32S[index] = ((double)imgData->imgDataINT32->Get(j,i,k,0,0)/(double)maxVal)*maxSize;
						else
							drawBuffer32S[index] = 0;
						index++;
					}
				}
			}
			break;
		case IMG_RGB24: /* --- 1 byte   8 bits  --- */
			break;
		default:
			WriteLog("Pixel type unknown\n");

	}

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &mainTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D_EXT, mainTexture);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	switch (imgData->imgInfo->GetPixelType()) {
		case IMG_UINT8:
			glTexImage3DEXT(GL_TEXTURE_3D, 0, GL_LUMINANCE16, textureWidth, textureHeight, textureDepth, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, drawBuffer8U); break;
		case IMG_INT8:
			glTexImage3DEXT(GL_TEXTURE_3D, 0, GL_LUMINANCE16, textureWidth, textureHeight, textureDepth, 0, GL_LUMINANCE, GL_BYTE, drawBuffer8S); break;
		case IMG_UINT16:
			glTexImage3DEXT(GL_TEXTURE_3D, 0, GL_LUMINANCE16, textureWidth, textureHeight, textureDepth, 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, drawBuffer16U); break;
		case IMG_INT16:
			glTexImage3DEXT(GL_TEXTURE_3D, 0, GL_LUMINANCE16, textureWidth, textureHeight, textureDepth, 0, GL_LUMINANCE, GL_SHORT, drawBuffer16S); break;
		case IMG_UINT32:
			glTexImage3DEXT(GL_TEXTURE_3D, 0, GL_LUMINANCE16, textureWidth, textureHeight, textureDepth, 0, GL_LUMINANCE, GL_UNSIGNED_INT, drawBuffer32U); break;
		case IMG_INT32:
			glTexImage3DEXT(GL_TEXTURE_3D, 0, GL_LUMINANCE16, textureWidth, textureHeight, textureDepth, 0, GL_LUMINANCE, GL_INT, drawBuffer32S); break;
	}

	switch (imgData->imgInfo->GetPixelType()) {
		case IMG_UINT8: delete[] drawBuffer8U; break;
		case IMG_INT8: delete[] drawBuffer8S; break;
		case IMG_UINT16: delete[] drawBuffer16U; break;
		case IMG_INT16: delete[] drawBuffer16S; break;
		case IMG_UINT32: delete[] drawBuffer32U; break;
		case IMG_INT32: delete[] drawBuffer32S; break;
	}

	mainVars->VolRend_reInit(false);
}


// --------------------------------------------------------
// ---------- Draw ----------------------------------------
// --------------------------------------------------------
void ImageVolumeRenderRay::Draw(bool reInit)
{
	w = canvasSize.GetWidth();
	h = canvasSize.GetHeight();

	WriteLog("ImageVolumeRenderRay->Draw() called...\n");
	WriteLog("Error (-1): " + GetGLErrorString() + "\n");
	if ((!initFinished) || (reInit)) {
		Init();
	}
	glPushAttrib(GL_ALL_ATTRIB_BITS);
		if (mainVars->VolRend_useSSD())
			DrawRayTraceSSD();
		else
			DrawRayTrace();
	glPopAttrib();
	WriteLog("Error (1): " + GetGLErrorString() + "\n");
}


// --------------------------------------------------------
// ---------- DrawRayTrace --------------------------------
// --------------------------------------------------------
void ImageVolumeRenderRay::DrawRayTrace()
{
	WriteLog("ImageVolumeRenderRay->DrawRayTrace() called...\n");
	float *colormap;
	GLint texSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);

	/* setup the color map... whether or not its used is determined in the fragment shader */
	glGenTextures(1, &colorMapTexture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_1D,colorMapTexture);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	colormap = frame->colorMaps->GetPointerToMap(mainVars->ColorMap_index());
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, texSize, 0, GL_RGBA, GL_FLOAT, colormap);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	/* first we bind the FBO so we can render to it */
	EnableRenderbuffers();
	SetFBOViewport();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	SetupRotationMatrix();
	RenderBackface();
	RenderFrontface();
	/* restore the rendering destination to the screen buffer */
	DisableRenderbuffers();

	/* setup the screen for the regular drawing */
	SetFullscreenViewport();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	RenderBufferToScreen();

	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();

	/* display rotation information */
	/* setup the viewport for the text overlays */
	glViewport(0, 0, canvasSize.GetWidth(), canvasSize.GetHeight());
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (double)canvasSize.GetWidth(), 0.0, (double)canvasSize.GetHeight(), 1000.0, -1000.0);
	glMatrixMode (GL_MODELVIEW);
	//wxString string = wxString::Format("Rotation: %d", mainVars->Display_windowCenter());
//	string += wxString::Format("%.4f %.4f %.4f %.4f\n", mainVars->Rotations_angle(), mainVars->Rotations_X(), mainVars->Rotations_Y(), mainVars->Rotations_Z());
//	string += wxString::Format("%.4f %.4f %.4f %.4f\n", rotMat[0], rotMat[4], rotMat[8], rotMat[12]);
//	string += wxString::Format("%.4f %.4f %.4f %.4f\n", rotMat[1], rotMat[5], rotMat[9], rotMat[13]);
//	string += wxString::Format("%.4f %.4f %.4f %.4f\n", rotMat[2], rotMat[6], rotMat[10], rotMat[14]);
//	string += wxString::Format("%.4f %.4f %.4f %.4f\n", rotMat[3], rotMat[7], rotMat[11], rotMat[15]);

	DrawShadowText(wxString::Format("%.4f %.4f %.4f %.4f\n", mainVars->Rotations_angle(), mainVars->Rotations_X(), mainVars->Rotations_Y(), mainVars->Rotations_Z()),10,320,1.0,1.0,1.0,8);
	DrawShadowText(wxString::Format("%.4f %.4f %.4f %.4f\n", rotMat[0], rotMat[4], rotMat[8], rotMat[12]),10,300,1.0,1.0,1.0,8);
	DrawShadowText(wxString::Format("%.4f %.4f %.4f %.4f\n", rotMat[1], rotMat[5], rotMat[9], rotMat[13]),10,280,1.0,1.0,1.0,8);
	DrawShadowText(wxString::Format("%.4f %.4f %.4f %.4f\n", rotMat[2], rotMat[6], rotMat[10], rotMat[14]),10,260,1.0,1.0,1.0,8);
	DrawShadowText(wxString::Format("%.4f %.4f %.4f %.4f\n", rotMat[3], rotMat[7], rotMat[11], rotMat[15]),10,240,1.0,1.0,1.0,8);
	WriteLog("ImageVolumeRenderRay->DrawRayTrace() done...\n");
}


// --------------------------------------------------------
// ---------- DrawRayTraceSSD -----------------------------
// --------------------------------------------------------
void ImageVolumeRenderRay::DrawRayTraceSSD()
{
//	glDisable(GL_BLEND); /* disable alpha blending */

//	glEnable(GL_ALPHA_TEST);
//	glAlphaFunc(GL_LESS,fIsoValue);
}


// --------------------------------------------------------
// ---------- RenderBufferToScreen ------------------------
// --------------------------------------------------------
void ImageVolumeRenderRay::RenderBufferToScreen()
{
	int numIts;
	int useSSD;

	if (mainVars->VolRend_isMoving()) numIts = mainVars->VolRend_numIterations()/2;
	else numIts = mainVars->VolRend_numIterations();
	
	if (mainVars->VolRend_useSSD()) useSSD = 1;
	else useSSD = 0;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D,mainTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,backfaceBuffer);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,frontfaceBuffer);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,finalImage);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_1D,colorMapTexture);
	glUseProgram(pRayTrace);
	/* Set up the shader parameters */
	glUniform1i(glGetUniformLocation(pRayTrace,"mainTexture"), 0);
	glUniform1i(glGetUniformLocation(pRayTrace,"backfaceBuffer"), 1);
	glUniform1i(glGetUniformLocation(pRayTrace,"frontfaceBuffer"), 2);
	glUniform1i(glGetUniformLocation(pRayTrace,"colorMapTexture"), 4);
	glUniform1i(glGetUniformLocation(pRayTrace,"numIts"), numIts);
	glUniform1i(glGetUniformLocation(pRayTrace,"useSSD"), useSSD);

	glColor4f(1.0f,1.0f,1.0f,1.0f); /* necessary to correctly display the texture, though the texture is correctly generated in the FBO */
	glDisable(GL_DEPTH_TEST);
	/* render the texture */
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0, -1.0,  0.0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0, -1.0,  0.0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0,  1.0,  0.0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0,  1.0,  0.0);
	glEnd();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	/* restore the default fixed pipeline */
	glUseProgram(0);
}


// --------------------------------------------------------
// ---------- RenderBackface ------------------------------
// --------------------------------------------------------
void ImageVolumeRenderRay::RenderBackface()
{
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, backfaceBuffer, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	drawQuads(1.0,1.0,1.0);
	glDisable(GL_CULL_FACE);
}


// --------------------------------------------------------
// ---------- RenderFrontface -----------------------------
// --------------------------------------------------------
void ImageVolumeRenderRay::RenderFrontface()
{
	glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, frontfaceBuffer, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	drawQuads(1.0,1.0,1.0);
	glDisable(GL_CULL_FACE);
}


// --------------------------------------------------------
// ---------- SetFullscreenViewport -----------------------
// --------------------------------------------------------
void ImageVolumeRenderRay::SetFullscreenViewport()
{
	float aspect;
	float size = 1.0;

	/* setup the projection matrix so that the shortest dimension is 1 */
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (h>w) {
		aspect = (float)h/(float)w;
		glOrtho(-size, size, -size*aspect, size*aspect, -10.0, 10.0);
	}
	else {
		aspect = (float)w/(float)h;
		glOrtho(-size*aspect, size*aspect,-size, size, -10.0, 10.0);
	}
	glMatrixMode(GL_MODELVIEW);
}


// --------------------------------------------------------
// ---------- SetFBOViewport ------------------------------
// --------------------------------------------------------
void ImageVolumeRenderRay::SetFBOViewport()
{
	/* setup the viewport for the cube rendering */
	glViewport(0, 0, 512, 512);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.95,0.95,-0.95,0.95,-100.0,100.0);
	glMatrixMode(GL_MODELVIEW);
}


// --------------------------------------------------------
// ---------- SetCanvasSize ----------------------public---
// --------------------------------------------------------
void ImageVolumeRenderRay::SetCanvasSize(wxSize size)
{
	canvasSize = size;
}


// --------------------------------------------------------
// ---------- vertex --------------------------------------
// --------------------------------------------------------
void ImageVolumeRenderRay::vertex(float x, float y, float z)
{
	glColor4f(x,y,z,1.0);
//	glMultiTexCoord3fARB(GL_TEXTURE1_ARB, x, y, z);
	glVertex3f(x,y,z);
}


// --------------------------------------------------------
// ---------- drawQuads -----------------------------------
// --------------------------------------------------------
void ImageVolumeRenderRay::drawQuads(float x, float y, float z)
{
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, -1.0); vertex(0.0, 0.0, 0.0); vertex(0.0, y, 0.0); vertex(x, y, 0.0); vertex(x, 0.0, 0.0); /* Back side */
		glNormal3f(0.0, 0.0, 1.0); vertex(0.0, 0.0, z); vertex(x, 0.0, z); vertex(x, y, z); vertex(0.0, y, z); /* Front side */
		glNormal3f(0.0, 1.0, 0.0); vertex(0.0, y, 0.0); vertex(0.0, y, z); vertex(x, y, z); vertex(x, y, 0.0); /* Top side */
		glNormal3f(0.0, -1.0, 0.0); vertex(0.0, 0.0, 0.0); vertex(x, 0.0, 0.0); vertex(x, 0.0, z); vertex(0.0, 0.0, z); /* Bottom side */
		glNormal3f(-1.0, 0.0, 0.0); vertex(0.0, 0.0, 0.0); vertex(0.0, 0.0, z); vertex(0.0, y, z); vertex(0.0, y, 0.0); /* Left side */
		glNormal3f(1.0, 0.0, 0.0); vertex(x, 0.0, 0.0); vertex(x, y, 0.0); vertex(x, y, z); vertex(x, 0.0, z); /* Right side */
	glEnd();
}


// --------------------------------------------------------
// ---------- EnableRenderbuffers -------------------------
// --------------------------------------------------------
void ImageVolumeRenderRay::EnableRenderbuffers()
{
	glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, fbo);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);
}


// --------------------------------------------------------
// ---------- DisableRenderbuffers ------------------------
// --------------------------------------------------------
void ImageVolumeRenderRay::DisableRenderbuffers()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}


// --------------------------------------------------------
// ---------- SetupRotationMatrix -------------------------
// --------------------------------------------------------
void ImageVolumeRenderRay::SetupRotationMatrix()
{
	static float objectXform[16] =
	{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
	};

	/* move it back on the Z axis first ... */
//	glTranslatef(0,0,-2.25);

	glScalef(1.25,1.25,1.25);

	/* ... then rotate it ... */
	if (mainVars->Rotations_isMoving()) { /* Transform the viewing direction */
		glPushMatrix();
		glLoadIdentity();
		glRotatef(mainVars->Rotations_angle(), mainVars->Rotations_X(), mainVars->Rotations_Y(), mainVars->Rotations_Z());
		glMultMatrixf((GLfloat *) objectXform);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) objectXform);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) rotMat);
		glPopMatrix();
	}
	WriteLog("Rotation Matrix:\n");
	WriteLog(wxString::Format("%.4f %.4f %.4f %.4f\n", mainVars->Rotations_angle(), mainVars->Rotations_X(), mainVars->Rotations_Y(), mainVars->Rotations_Z()));
	WriteLog(wxString::Format("%.4f %.4f %.4f %.4f\n", objectXform[0], objectXform[4], objectXform[8], objectXform[12]));
	WriteLog(wxString::Format("%.4f %.4f %.4f %.4f\n", objectXform[1], objectXform[5], objectXform[9], objectXform[13]));
	WriteLog(wxString::Format("%.4f %.4f %.4f %.4f\n", objectXform[2], objectXform[6], objectXform[10], objectXform[14]));
	WriteLog(wxString::Format("%.4f %.4f %.4f %.4f\n", objectXform[3], objectXform[7], objectXform[11], objectXform[15]));
	frame->SetCurrentRotation(mainVars->Rotations_angle(), mainVars->Rotations_X(), mainVars->Rotations_Y(), mainVars->Rotations_Z());
	glMultMatrixf((GLfloat *) objectXform);
	/* ... then center it at the origin */
	glTranslatef(-0.5,-0.5,-0.5);
}
