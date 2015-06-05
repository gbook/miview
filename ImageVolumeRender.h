/* ----------------------------------------------------------------------------
	ImageVolumeRender.h


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

class ImageVolumeRender
{
public:
	void SetCanvasSize(wxSize size);
	void Draw(bool reInit);
	ImageVolumeRender(ImageData *imgdata, ImageVolumeHelper *imghelper, MainFrameVariables *mVars, int viewtype, wxColour col, int al);
	virtual ~ImageVolumeRender();
	ImageData *imgData;

	void SetParams(XYZTriplet rotate, float ang, double intens, double dens, int numSlices);
	void SetParamsReload(wxColour col, int viewtype);
	void SetGLShaderProgram(GLuint prog);

private:
	void Init(void);
	//void SetDisplayList(void);
	void Display(void);
//	void DrawRayTrace();
//	void drawQuads(float x, float y, float z);
//	void vertex(float x, float y, float z);
//	void enable_renderbuffers();
//	void disable_renderbuffers();
	wxString GetGLErrorString();

	int viewType;

	GLubyte *image;
	GLuint texName, mainTexture, lutTexture, windowLevelTexture, renderTexture, backface_buffer;
//	GLuint final_image, renderbuffer, framebuffer;
	GLuint listName;
	float points[64][3];
	float intensity;
	float density;
	int slices;
	float angle;
//	GLuint fbo;					// Our handle to the FBO
//	GLuint depthBuffer;			// Our handle to the depth render buffer
//	GLuint img;					// Our handle to a texture

	GLuint pRayTrace; /* shader program */

	//LogWindow *logWindow;			/* pointer to logwindow */
	ImageVolumeHelper *imgHelper;	/* drawing helper */
	MainFrameVariables *mainVars;	/* variables from UI */

	XYZTriplet rotations;
	wxColour color;
	int alpha;
	double scale;
	char step_p;

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
	double texSizeX, texSizeY, texSizeZ;
};
