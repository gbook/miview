/* ----------------------------------------------------------------------------
	ImageVolumeRender.cpp


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
//#include "LogWindow.h"
#include "XYZTriplet.h"
#include "ImageVolumeRender.h"
#include "wx/progdlg.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

//#include "gl/glut.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImageVolumeRender::ImageVolumeRender(ImageData *imgdata, ImageVolumeHelper *imghelper, MainFrameVariables *mVars, int viewtype, wxColour col, int al)
{
	initFinished = false;
	//logWindow = logwind;
	imgData = imgdata;
	imgHelper = imghelper;
	mainVars = mVars;
	viewType = viewtype;
	color = col;
	alpha = al;

	scale = 0.75;

	Init();
}

ImageVolumeRender::~ImageVolumeRender()
{

}


void ImageVolumeRender::SetParamsReload(wxColour col, int viewtype)
{
	WriteLog("ImageVolumeRender->SetParamsReload() called...\n");
	color = col;
	viewType = viewtype;

	initFinished = false;
}


void ImageVolumeRender::SetGLShaderProgram(GLuint prog)
{
	WriteLog("ImageVolumeRender->SetGLShaderProgram() called...\n");
	pRayTrace = prog;
}


void ImageVolumeRender::SetParams(XYZTriplet rotate, float ang, double intens, double dens, int numSlices)
{
	rotations = rotate;
	intensity = intens;
	density = dens;
	slices = numSlices;
	angle = ang;

	WriteLog(msg.Format("ImageVolumeRender->SetParams(%.4f,%.4f,%.4f,%.4f) called...\n",angle, rotate.x,rotate.y,rotate.z));
}

// --------------------------------------------------------
// ---------- Init ----------------------------------------
// --------------------------------------------------------
void ImageVolumeRender::Init(void)
{    
	WriteLog("ImageVolumeRender->Init() called...\n");

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

	texSizeX = 0.0;
	texSizeY = 0.0;
	texSizeZ = 0.0;

	/* get the width of the texture */
	n = 0; found = false;
	while (!found) {
		if (iWidth <= pow(2.0,n)) {
			textureWidth = pow(2.0,n);
			found = true;
		}
		n++;
	}

	/* get the height of the texture */
	n = 0; found = false;
	while (!found) {
		if (iHeight <= pow(2.0,n)) {
			textureHeight = pow(2.0,n);
			found = true;
		}
		n++;
	}

	/* get the depth of the texture */
	n = 0; found = false;
	while (!found) {
		if (iDepth <= pow(2.0,n)) {
			textureDepth = pow(2.0,n);
			found = true;
		}
		n++;
	}

	texSizeX = (double)iWidth/(double)textureWidth;
	texSizeY = (double)iHeight/(double)textureHeight;
	texSizeZ = (double)iDepth/(double)textureDepth;

	WriteLog( msg.Format("Alpha %d; Color (%d,%d,%d)\n",alpha, color.Red(),color.Green(),color.Blue()) );

	WriteLog( msg.Format("Created texture of size (%d,%d,%d)...\n",textureWidth, textureHeight, textureDepth) );

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
	glGenTextures(1, &mainTexture); /* GL_TEXTURE0 */
	glBindTexture(GL_TEXTURE_3D_EXT, mainTexture);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (mainVars->VolRend_method() == VOLMETHOD_3DTEXTURE) {
		switch (imgData->imgInfo->GetPixelType()) {
			case IMG_UINT8:
				glTexImage3DEXT(GL_TEXTURE_3D, 0, GL_ALPHA16, textureWidth, textureHeight, textureDepth, 0, GL_ALPHA, GL_UNSIGNED_BYTE, drawBuffer8U); break;
			case IMG_INT8:
				glTexImage3DEXT(GL_TEXTURE_3D, 0, GL_ALPHA16, textureWidth, textureHeight, textureDepth, 0, GL_ALPHA, GL_BYTE, drawBuffer8S); break;
			case IMG_UINT16:
				glTexImage3DEXT(GL_TEXTURE_3D, 0, GL_ALPHA16, textureWidth, textureHeight, textureDepth, 0, GL_ALPHA, GL_UNSIGNED_SHORT, drawBuffer16U); break;
			case IMG_INT16:
				glTexImage3DEXT(GL_TEXTURE_3D, 0, GL_ALPHA16, textureWidth, textureHeight, textureDepth, 0, GL_ALPHA, GL_SHORT, drawBuffer16S); break;
			case IMG_UINT32:
				glTexImage3DEXT(GL_TEXTURE_3D, 0, GL_ALPHA16, textureWidth, textureHeight, textureDepth, 0, GL_ALPHA, GL_UNSIGNED_INT, drawBuffer32U); break;
			case IMG_INT32:
				glTexImage3DEXT(GL_TEXTURE_3D, 0, GL_ALPHA16, textureWidth, textureHeight, textureDepth, 0, GL_ALPHA, GL_INT, drawBuffer32S); break;
		}
		// For automatic texture coordinate generation
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	}
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	WriteLog( msg.Format("Drawing a Volume Rendering\n"));

	initFinished = true;
		switch (imgData->imgInfo->GetPixelType()) {
			case IMG_UINT8: delete[] drawBuffer8U; break;
			case IMG_INT8: delete[] drawBuffer8S; break;
			case IMG_UINT16: delete[] drawBuffer16U; break;
			case IMG_INT16: delete[] drawBuffer16S; break;
			case IMG_UINT32: delete[] drawBuffer32U; break;
			case IMG_INT32: delete[] drawBuffer32S; break;
		}

	int ii,gg;
	float temp;
	//create the points used to draw the slices
	for (ii=0;ii<8;ii++){
		for (gg=0;gg<8;gg++){
			points[ii*8+gg][0]=(float)gg/7.0f-0.5f;
			points[ii*8+gg][1]=(float)ii/7.0f-0.5f;
			points[ii*8+gg][2]=-0.5f;
			temp=points[ii*8+gg][0]*points[ii*8+gg][0]+
				 points[ii*8+gg][1]*points[ii*8+gg][1]+
				 points[ii*8+gg][2]*points[ii*8+gg][2];
			temp=1.0f/(float)sqrt(temp);
			points[ii*8+gg][0]*=temp;
			points[ii*8+gg][1]*=temp;
			points[ii*8+gg][2]*=temp;
			points[ii*8+gg][0]*=2.0f;
			points[ii*8+gg][1]*=2.0f;
			points[ii*8+gg][2]+=1.0f;
		}
	}

	/* ------------------------------------------------ */
	/* need to reenable this after testing is complete: */
	/* ------------------------------------------------ */

	//glGenTextures(1, &lutTexture);  /* GL_TEXTURE1 */
	//glBindTexture(GL_TEXTURE_1D,lutTexture);
	//glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 //   glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//float colors[8];
	//colors[0] = 0.3;
	//colors[1] = 0.0;
	//colors[2] = 0.0;
	//colors[3] = 1.0;
	//colors[4] = 0.0;
	//colors[5] = 0.0;
	//colors[6] = 1.0;
	//colors[7] = 1.0;
	//glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 2, 0, GL_RGBA, GL_FLOAT, colors);

	mainVars->VolRend_reInit(false);
}


// --------------------------------------------------------
// ---------- Display -------------------------------------
// --------------------------------------------------------
void ImageVolumeRender::Display(void)
{
	WriteLog("ImageVolumeRender->Display() called...\n");

	static float objectXform[16] =
	{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
	};

	int ii,gg,hh;
	int w = canvasSize.GetWidth();
	int h = canvasSize.GetHeight();
	GLfloat size_ = 1.0;
	GLfloat aspect = (GLfloat) h/(GLfloat) w;

	/* setup viewport, etc */
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-size_, size_, -size_*aspect, size_*aspect, -size_, size_);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

//	WriteLog("ImageVolumeRender->Display() checkpoint 1 reached...\n");

	/* setup the texture environment */
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_3D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_3D, mainTexture);

	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT);

//	WriteLog("ImageVolumeRender->Display() checkpoint 2 reached...\n");

	/* Clip plane equations... still not sure how/why they are used */
	static GLdouble eqx0[4] = { 1.0, 0.0, 0.0, 0.0};
	static GLdouble eqx1[4] = {-1.0, 0.0, 0.0, 1.0};
	static GLdouble eqy0[4] = {0.0,  1.0, 0.0, 0.0};
	static GLdouble eqy1[4] = {0.0, -1.0, 0.0, 1.0};
	static GLdouble eqz0[4] = {0.0, 0.0,  1.0, 0.0};
	static GLdouble eqz1[4] = {0.0, 0.0, -1.0, 1.0};

	// Define equations for automatic texture coordinate generation
	static GLfloat x[] = {1.0, 0.0, 0.0, 0.0};
	static GLfloat y[] = {0.0, 1.0, 0.0, 0.0};
	static GLfloat z[] = {0.0, 0.0, 1.0, 0.0};

//	WriteLog("ImageVolumeRender->Display() checkpoint 3 reached...\n");

	/* push what? */
	glPushMatrix ();
	glScalef(scale, scale, scale);

	/* push the scaling? */
	glPushMatrix ();

//	WriteLog("ImageVolumeRender->Display() checkpoint 4 reached...\n");

	if (mainVars->Rotations_isMoving()) {
		/* Transform the viewing direction */
		//WriteLog(msg.Format("Rotation in ImageVolumeRender() (%.4f,%.4f,%.4f,%.4f)\n",mainVars->Rotations_angle(), mainVars->Rotations_X(), mainVars->Rotations_Y(), mainVars->Rotations_Z()));
		glPushMatrix();
		glLoadIdentity();
		glRotatef(mainVars->Rotations_angle(), mainVars->Rotations_X(), mainVars->Rotations_Y(), mainVars->Rotations_Z());
		glMultMatrixf((GLfloat *) objectXform);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) objectXform);
		glPopMatrix();
	}
	glMultMatrixf((GLfloat *) objectXform);

	glTranslatef(-0.5, -0.5, -0.5); /* since the texture coords are 1 unit cubed, lower left should be drawn here */

	// Take a copy of the model view matrix now shove it in to the GPU 
	// buffer for later use in automatic texture coord generation.
	glTexGenfv(GL_S, GL_EYE_PLANE, x);
	glTexGenfv(GL_T, GL_EYE_PLANE, y);
	glTexGenfv(GL_R, GL_EYE_PLANE, z);

//	WriteLog("ImageVolumeRender->Display() checkpoint 5 reached...\n");

	// Take a copy of the model view matrix and shove it in to the GPU
	// buffer for later use in clipping planes.
	glClipPlane(GL_CLIP_PLANE0, eqx0);
	glClipPlane(GL_CLIP_PLANE1, eqx1);
	glClipPlane(GL_CLIP_PLANE2, eqy0);
	glClipPlane(GL_CLIP_PLANE3, eqy1);
	glClipPlane(GL_CLIP_PLANE4, eqz0);
	glClipPlane(GL_CLIP_PLANE5, eqz1);

//	WriteLog("ImageVolumeRender->Display() checkpoint 6 reached...\n");

	glPopMatrix ();

	// Enable clip planes
	glEnable(GL_CLIP_PLANE0);
	glEnable(GL_CLIP_PLANE1);
	glEnable(GL_CLIP_PLANE2);
	glEnable(GL_CLIP_PLANE3);
	glEnable(GL_CLIP_PLANE4);
	glEnable(GL_CLIP_PLANE5);

	/* set the color for the slices - white, with intensity being the alpha value */
	glColor4f(1.0f,1.0f,1.0f,intensity);

	/* enable the alpha blending/testing */
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,density*intensity);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

//	WriteLog("ImageVolumeRender->Display() checkpoint 7 reached...\n");

	// Render enclosing rectangles
	/* draw the slices */
	for (ii=0;ii<slices;ii++){
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -1.0f +(float)ii*2.0f/(float)(slices-1));
		glBegin(GL_QUADS);
		for(hh=0;hh<7;hh++){
			for(gg=0;gg<7;gg++){
				glVertex3fv(points[hh*8+gg]);
				glVertex3fv(points[hh*8+gg+1]);
				glVertex3fv(points[(hh+1)*8+gg+1]);
				glVertex3fv(points[(hh+1)*8+gg]);
			}
		}
		glEnd();
		glPopMatrix();
	}

	WriteLog("ImageVolumeRender->Display() checkpoint 8 reached...\n");

	glPopMatrix ();

	WriteLog("ImageVolumeRender->Display() checkpoint 9 reached...\n");

	glDisable(GL_TEXTURE_3D);

	// Disable clip planes
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE1);
	glDisable(GL_CLIP_PLANE2);
	glDisable(GL_CLIP_PLANE3);
	glDisable(GL_CLIP_PLANE4);
	glDisable(GL_CLIP_PLANE5);

	/* display a white cube around the data */
	glEnable(GL_LINE_SMOOTH);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glScalef(scale, scale, scale);

	if (mainVars->Rotations_isMoving()) {
		/* rotate the thing */
		glPushMatrix();
		glLoadIdentity();
		glRotatef(mainVars->Rotations_angle(), mainVars->Rotations_X(), mainVars->Rotations_Y(), mainVars->Rotations_Z());
		glMultMatrixf((GLfloat *) objectXform);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) objectXform);
		glPopMatrix();
	}
	glMultMatrixf((GLfloat *) objectXform);
	glColor3f(1.0,1.0,1.0);
	glutWireCube(1.0);

//	glColor4f(0.0,1.0,0.0,0.1); /* try to draw a side to the box */
//	glBegin(GL_QUADS);
//		glVertex3f(-0.5,-0.5,-0.5);
//		glVertex3f(0.5,-0.5,-0.5);
//		glVertex3f(0.5,0.5,-0.5);
//		glVertex3f(-0.5,0.5,-0.5);
//	glEnd();

	glDisable(GL_LINE_SMOOTH);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_3D);
}


// --------------------------------------------------------
// ---------- SetCanvasSize ----------------------public---
// --------------------------------------------------------
void ImageVolumeRender::SetCanvasSize(wxSize size)
{
	canvasSize = size;
}


// --------------------------------------------------------
// ---------- Draw ----------------------------------------
// --------------------------------------------------------
void ImageVolumeRender::Draw(bool reInit)
{
	WriteLog("ImageVolumeRender->Draw() called...\n");
	WriteLog("Error (-1): " + GetGLErrorString() + "\n");
	if ((!initFinished) || (reInit)) {
		Init();
	}
	if (mainVars->VolRend_method() == VOLMETHOD_3DTEXTURE) {
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		Display();
		glPopAttrib();
	}
}


wxString ImageVolumeRender::GetGLErrorString()
{
	wxString str;
	int error = glGetError();
	switch (error) {
		case GL_NO_ERROR: str = "GL_NO_ERROR"; break;
		case GL_INVALID_ENUM: str = "GL_INVALID_ENUM"; break;
		case GL_INVALID_VALUE: str = "GL_INVALID_VALUE"; break;
		case GL_INVALID_OPERATION: str = "GL_INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW: str = "GL_STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW: str = "GL_STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY: str = "GL_OUT_OF_MEMORY"; break;
		case 0x506: str = "GL_INVALID_FRAMEBUFFER_OPERATION_EXT"; break;
		default: str = "Unknown error"; break;
	}

	GLenum status = (GLenum)glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch(status) {
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			str += " <<<< OK >>>>";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			str += " GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			str += " GL_FRAMEBUFFER_UNSUPPORTED_EXT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			str += " GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			str += " GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			str += " GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			str += " GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			str += " GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT";
			break;
		default:
			str += " UNKNWON\n";
	}
	return str;
}