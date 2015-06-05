/* ----------------------------------------------------------------------------
	ColorMap.cpp


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
#include "ColorMap.h"

ColorMap::ColorMap(int texSize, wxSize bmSize)
{
	pMap = NULL;
	glTextureSize = texSize;
	bitmapSize = bmSize;
}

ColorMap::~ColorMap(void)
{
	delete[] pMap;
}


// --------------------------------------------------------
// ---------- GenerateGLTexture ---------------------------
// --------------------------------------------------------
void ColorMap::GenerateGLTexture()
{
	/* resize the gradient from origSize to newSize */
	//float *newMap;
	int i,j;
	int newPstart, newPend;
	double pos;

	if (pMap != NULL)
		delete[] pMap;

	pMap = new float[glTextureSize*4]; /* RGBA components */

	/* loop through all positions */
	for (i=0;i<(int)p.Count()-1;i++) {
		/* calculate position in new map */
		newPstart = ((double)p[i]/1000.0)*(double)glTextureSize;
		newPend = ((double)p[i+1]/1000.0)*(double)glTextureSize;
		/* loop through newP-1 to newP */
		for (j=newPstart*4;j<newPend*4;j+=4) {
			pos = ((double)(j/4)-(double)newPstart)/((double)newPend-(double)newPstart);
			pMap[j] = ((1.0-pos)*(double)r[i] + pos*(double)r[i+1])/255.0;
			pMap[j+1] = ((1.0-pos)*(double)g[i] + pos*(double)g[i+1])/255.0;
			pMap[j+2] = ((1.0-pos)*(double)b[i] + pos*(double)b[i+1])/255.0;
			pMap[j+3] = ((1.0-pos)*(double)a[i] + pos*(double)a[i+1])/255.0;
		}
	}
}


// --------------------------------------------------------
// ---------- GenerateGradientBitmap ----------------------
// --------------------------------------------------------
void ColorMap::GenerateGradientBitmap()
{
	//double sectionSize = (double)bitmapSize.GetWidth()/(double)(numColors-1);
	int sectionStart, sectionEnd;
	int i;
	wxRect sectionRect;
	wxBitmap tmpBitmap(bitmapSize.GetWidth(),bitmapSize.GetHeight());

	wxMemoryDC dc;
	dc.SelectObject(tmpBitmap);

	for (i=0;i<numColors-1;i++) {
		sectionStart = floor((double)bitmapSize.GetWidth()*((double)p[i]/1000.0));
		sectionEnd = ceil((double)bitmapSize.GetWidth()*((double)p[i+1]/1000.0));
		sectionRect = wxRect(sectionStart,0,(sectionEnd-sectionStart),bitmapSize.GetHeight());
		dc.GradientFillLinear(sectionRect,wxColour(r[i],g[i],b[i]),wxColour(r[i+1],g[i+1],b[i+1]));
	}

	/* done, now get the bitmap out of the DC */
	dc.SelectObject(wxNullBitmap);

//	tmpBitmap.SaveFile("Testing"+name+".bmp",wxBITMAP_TYPE_BMP);
	bitmap = tmpBitmap;
}
