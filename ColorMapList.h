/* ----------------------------------------------------------------------------
	ColorMapList.h


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

#include "ColorMap.h"
#include "wx/textfile.h"
#include "wx/dynarray.h"

//WX_DEFINE_ARRAY_PTR(float *, wxArrayFloatPointer);
WX_DECLARE_OBJARRAY(ColorMap,c_colorMaps);

class ColorMapList
{
public:
	ColorMapList(int texSize);
	~ColorMapList(void);

	/* resize the map when its added */
	int AddMap(wxString name);
	int AddMap(wxString name, wxArrayInt r, wxArrayInt g, wxArrayInt b, wxArrayInt a, wxArrayInt p);
	/* delete extra maps if needed */
	void DeleteMap(int mapIndex);

	bool ReadColorMapFile();
	void WriteColorMapFile();
	
	void SortColorMapByPosition(int mapIndex);
	int AddColorToMap(int mapIndex, int r, int g, int b, int a, int p);
	void DeleteColorFromMap(int mapIndex, int colorIndex);

	void UpdateRed(int mapIndex, int colorIndex,int red);
	void UpdateGreen(int mapIndex, int colorIndex,int green);
	void UpdateBlue(int mapIndex, int colorIndex,int blue);
	void UpdateAlpha(int mapIndex, int colorIndex,int alpha);
	void UpdatePosition(int mapIndex, int colorIndex,int position);

	/* get the map at the specified index */
	float *GetPointerToMap(int mapIndex);

	/* array of colormaps */
	c_colorMaps maps;

//	wxArrayFloatPointer maps; /* array of pointers to the maps */
private:
	//int newMapSize; /* OpenGL texture size limit for the current video card */
	int glTextureSize;
	wxSize bitmapSize;

	wxString msg;
};
