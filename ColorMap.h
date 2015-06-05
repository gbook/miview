/* ----------------------------------------------------------------------------
	ColorMap.h


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

#pragma once

class ColorMap
{
public:
	ColorMap(int texSize, wxSize bmSize);
	~ColorMap(void);

	void GenerateGLTexture();
	void GenerateGradientBitmap();

	wxArrayInt r,g,b,a,p;
	int numColors;
	wxString name;
	float *pMap;
	wxBitmap bitmap;
	int glTextureSize;
	wxSize bitmapSize;
};
