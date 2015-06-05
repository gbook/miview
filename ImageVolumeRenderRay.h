/* ----------------------------------------------------------------------------
	ImageVolumeRenderRay.h


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



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageData.h"
#include "ImageVolumeHelper.h"
#include "MainFrameVariables.h"

class ImageVolumeRenderRay
{
public:
	void SetCanvasSize(wxSize size);
	void Draw(bool reInit);
	ImageVolumeRenderRay(ImageData *imgdata, ImageVolumeHelper *imghelper, MainFrameVariables *mVars, ImageCanvas *imgCan, MainFrame *Frame);
	virtual ~ImageVolumeRenderRay();
	ImageData *imgData;

	void SetGLShaderProgram(GLuint progReg, GLuint progSSD);

private:
	void Init(void);
	void CreateVolumeTexture(void);
	void Display(void);
	void DrawRayTrace();
	void DrawRayTraceSSD();
	void drawQuads(float x, float y, float z);
	void vertex(float x, float y, float z);
	void EnableRenderbuffers();
	void DisableRenderbuffers();
	void RenderBufferToScreen();
	void RenderBackface();
	void RenderFrontface();
	void SetFullscreenViewport();
	void SetFBOViewport();
	void SetupRotationMatrix();

	float rotMat[16];

	int viewType;
	int w,h;		/* width and height of the canvas */
	GLuint fbo;					// Our handle to the FBO
	GLuint depthBuffer;			// Our handle to the depth render buffer

	GLubyte *image;
	GLuint texName, mainTexture, lutTexture, windowLevelTexture, renderTexture, backfaceBuffer, frontfaceBuffer;
	GLuint colorMapTexture;
	GLuint finalImage, renderBuffer, frameBuffer;

	GLuint pRayTrace;		/* regular volume rendering shader program */
	GLuint pRayTraceSSD;	/* SSD shader program */

	//LogWindow *logWindow;			/* pointer to logwindow */
	ImageVolumeHelper *imgHelper;	/* drawing helper */
	MainFrameVariables *mainVars;	/* variables from UI */
	ImageCanvas *imgCanvas;
	MainFrame *frame;

	XYZTriplet rotations;

	unsigned char		*drawBuffer8U;
	char				*drawBuffer8S;
	unsigned short int	*drawBuffer16U;
	short int			*drawBuffer16S;
	unsigned int		*drawBuffer32U;
	int					*drawBuffer32S;
	unsigned char		*drawBufferRGB24;

	wxString msg;
	bool initFinished;
	wxSize canvasSize;

	int textureWidth, textureHeight, textureDepth;
};
