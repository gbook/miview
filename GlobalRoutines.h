/* ----------------------------------------------------------------------------
	GlobalRoutines.h


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

#include <iostream>
#include <stdio.h>


// --------------------------------------------------------
// ---------- GetGLErrorString ----------------------------
// --------------------------------------------------------
static wxString GetGLErrorString()
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
			str += " - GL_FRAMEBUFFER_COMPLETE_EXT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			str += " - GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			str += " - GL_FRAMEBUFFER_UNSUPPORTED_EXT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			str += " - GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			str += " - GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			str += " - GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			str += " - GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			str += " - GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT";
			break;
		default:
			str += " UNKNWON\n";
	}
	return str;
}


// --------------------------------------------------------
// ---------- DrawShadowText ------------------------------
// --------------------------------------------------------
static int DrawShadowText(wxString string, int x, int y, double r, double g, double b, int size)
{
	void *font;
	const char *c;

	switch (size) {
		case 10:
			font = GLUT_BITMAP_HELVETICA_10;
			break;
		case 12:
			font = GLUT_BITMAP_HELVETICA_12;
			break;
		case 18:
			font = GLUT_BITMAP_HELVETICA_18;
			break;
		default:
			font = GLUT_BITMAP_HELVETICA_12;
	}

	glColor3d(0.1, 0.1, 0.1); /* set shadow color */
	glRasterPos2f(x+1, y-1);
	for (c=string.c_str(); *c != '\0'; c++) { glutBitmapCharacter(font, *c); }

	glColor3d(r, g, b); /* set text color */
	glRasterPos2f(x, y);
	for (c=string.c_str(); *c != '\0'; c++) { glutBitmapCharacter(font, *c); }

	return (int)string.Len();
}


// --------------------------------------------------------
// ---------- GetDrawnStringWidth -------------------------
// --------------------------------------------------------
static int GetDrawnStringWidth(wxString string, int size)
{
	void *font;
	const char *c;
	int stringWidth = 0;

	switch (size) {
		case 10:
			font = GLUT_BITMAP_HELVETICA_10;
			break;
		case 12:
			font = GLUT_BITMAP_HELVETICA_12;
			break;
		case 18:
			font = GLUT_BITMAP_HELVETICA_18;
			break;
		default:
			font = GLUT_BITMAP_HELVETICA_12;
	}
	for (c=string.c_str(); *c != '\0'; c++) {
		stringWidth += glutBitmapWidth(font, *c);
	}

	return stringWidth;
}


// --------------------------------------------------------
// ---------- Round ---------------------------------------
// --------------------------------------------------------
//static int Round(double num)
//{
//	int retNum;
//
//	if ((num - floor(num)) >= 0.5)
//		retNum = (int)ceil(num);
//	else
//		retNum = (int)floor(num);
//
//	return retNum;
//}


// --------------------------------------------------------
// ---------- Explode -------------------------------------
// --------------------------------------------------------
static wxArrayString Explode(wxString str, wxChar chr)
{
	wxString tmpStr;
	wxArrayString theArray;
	bool done=false;

	while (!done) {
		if (str.Len() < 1)
			done = true;

		tmpStr = str.BeforeFirst(chr);
		str = str.AfterFirst(chr);
		theArray.Add(tmpStr);
	}

	return theArray;
}


static void WriteLog(wxString msg)
{
	#ifdef _DEBUG
		msg = wxString::Format("[%dms] ",clock()) + msg;
		std::cout<<msg.c_str()<<std::endl;
	#endif
}

//// --------------------------------------------------------
//// ---------- GetMaxTextureSize ---------------------------
//// --------------------------------------------------------
//static int GetMaxTextureSize()
//{
//	int texSize;
//	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
//	return texSize;
//}
