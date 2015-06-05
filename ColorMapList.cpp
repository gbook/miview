/* ----------------------------------------------------------------------------
	ColorMapList.cpp


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
#include "wx/wx.h"
#include "ColorMapList.h"

#include "wx/arrimpl.cpp"
WX_DEFINE_OBJARRAY(c_colorMaps);

ColorMapList::ColorMapList(int texSize)
{
	glTextureSize = texSize;

	bitmapSize = wxSize(120,15);

	/* first thing... read in the colormap file */
	ReadColorMapFile();
}

ColorMapList::~ColorMapList(void)
{
	maps.Clear();
}


// --------------------------------------------------------
// ---------- ReadColorMapFile  ---------------------------
// --------------------------------------------------------
bool ColorMapList::ReadColorMapFile()
{
	int numLines;
	int i,j;
	wxArrayString line;
	wxTextFile mapFile;
	ColorMap newMap(glTextureSize,bitmapSize);

	/* always add Grayscale as the first color */
	newMap.name = "Gray";
	newMap.numColors = 2;
	newMap.r.Add(0);   newMap.g.Add(0);   newMap.b.Add(0);   newMap.a.Add(255); newMap.p.Add(0);
	newMap.r.Add(255); newMap.g.Add(255); newMap.b.Add(255); newMap.a.Add(255); newMap.p.Add(1000);
	maps.Add(newMap);
	maps[0].GenerateGLTexture();
	maps[0].GenerateGradientBitmap();

	if (mapFile.Open("colormaps.txt")) {
		/* format:
			colormapname #colors col1.r col1.g col1.b col1.a ... */
		/* read the file ... */
		numLines = (int)mapFile.GetLineCount();
		for (i=0;i<numLines;i++) {
			/* clear out the temp map values */
			newMap.r.Clear();
			newMap.g.Clear();
			newMap.b.Clear();
			newMap.a.Clear();
			newMap.p.Clear();
			newMap.numColors = 0;
			newMap.name = "";
			newMap.pMap = NULL;

			/* load the values from the file */
			line = Explode(mapFile[i],' ');
			newMap.name = line[0];
			newMap.numColors = atoi(line[1].c_str());
			for (j=2;j<newMap.numColors*5;j+=5) {
				newMap.r.Add(atoi(line[j].c_str()));
				newMap.g.Add(atoi(line[j+1].c_str()));
				newMap.b.Add(atoi(line[j+2].c_str()));
				newMap.a.Add(atoi(line[j+3].c_str()));
				newMap.p.Add(atoi(line[j+4].c_str()));
			}
			maps.Add(newMap);
			maps[i+1].GenerateGLTexture();
			maps[i+1].GenerateGradientBitmap();
		}

		mapFile.Close();
		return true;
	}
	else {
		wxMessageDialog(NULL,"Failure to open colormaps.txt","MIView error");
		return false;
	}
}


// --------------------------------------------------------
// ---------- WriteColorMapFile  --------------------------
// --------------------------------------------------------
void ColorMapList::WriteColorMapFile()
{
	int numMaps;
	int i,j;
	wxString line;
	wxTextFile mapFile;
	ColorMap newMap(glTextureSize,bitmapSize);

	mapFile.Open("colormaps.txt");
	mapFile.Clear();

	/* format:
		colormapname #colors col1.r col1.g col1.b col1.a col1.p ... */
	numMaps = (int)maps.Count();

	/* first map is always gray and is generated at runtime, so don't write it to the file */
	for (i=1;i<numMaps;i++) {
		line = maps[i].name;
		line += wxString::Format(" %d", maps[i].numColors);

		for (j=0;j<maps[i].numColors;j++) {
			line += wxString::Format(" %d %d %d %d %d",maps[i].r[j], maps[i].g[j], maps[i].b[j], maps[i].a[j], maps[i].p[j]);
		}
		mapFile.AddLine(line);
	}

	mapFile.Write();
}


// --------------------------------------------------------
// ---------- DeleteMap -----------------------------------
// --------------------------------------------------------
void ColorMapList::DeleteMap(int mapIndex)
{
	maps.RemoveAt(mapIndex);
}


// --------------------------------------------------------
// ---------- AddMap --------------------------------------
// --------------------------------------------------------
int ColorMapList::AddMap(wxString name)
{
	ColorMap newMap(glTextureSize,bitmapSize);
	newMap.name = name;
	newMap.GenerateGLTexture();
	newMap.GenerateGradientBitmap();

	maps.Add(newMap);
	return (int)maps.Count();
}


// --------------------------------------------------------
// ---------- AddMap --------------------------------------
// --------------------------------------------------------
int ColorMapList::AddMap(wxString name, wxArrayInt r, wxArrayInt g, wxArrayInt b, wxArrayInt a, wxArrayInt p)
{
	ColorMap newMap(glTextureSize,bitmapSize);
	newMap.name = name;
	newMap.r = r;
	newMap.g = g;
	newMap.b = b;
	newMap.a = a;
	newMap.p = p;
	newMap.numColors = (int)r.Count();
	newMap.GenerateGLTexture();
	newMap.GenerateGradientBitmap();
	maps.Add(newMap);

	return (int)maps.Count();
}


// --------------------------------------------------------
// ---------- GetPointerToMap -----------------------------
// --------------------------------------------------------
float *ColorMapList::GetPointerToMap(int mapIndex)
{
	return maps[mapIndex].pMap;
}


// --------------------------------------------------------
// ---------- SortColorMapByPosition ----------------------
// --------------------------------------------------------
void ColorMapList::SortColorMapByPosition(int mapIndex)
{
	int n = maps[mapIndex].numColors;
	int i,j;
	int tmp;

	/* bubble sort by position */
	for (i=0; i<n-1; i++) {
		for (j=0; j<n-1-i; j++)
			if (maps[mapIndex].p[j+1] < maps[mapIndex].p[j]) {  /* compare the two neighbors */
				/* swap a[j] and a[j+1] */
				tmp = maps[mapIndex].p[j]; maps[mapIndex].p[j] = maps[mapIndex].p[j+1]; maps[mapIndex].p[j+1] = tmp; /* position */
				tmp = maps[mapIndex].r[j]; maps[mapIndex].r[j] = maps[mapIndex].r[j+1]; maps[mapIndex].r[j+1] = tmp; /* red */
				tmp = maps[mapIndex].g[j]; maps[mapIndex].g[j] = maps[mapIndex].g[j+1]; maps[mapIndex].g[j+1] = tmp; /* green */
				tmp = maps[mapIndex].b[j]; maps[mapIndex].b[j] = maps[mapIndex].b[j+1]; maps[mapIndex].b[j+1] = tmp; /* blue */
				tmp = maps[mapIndex].a[j]; maps[mapIndex].a[j] = maps[mapIndex].a[j+1]; maps[mapIndex].a[j+1] = tmp; /* alpha */
			}
	}
	maps[mapIndex].GenerateGLTexture();
	maps[mapIndex].GenerateGradientBitmap();
}


// --------------------------------------------------------
// ---------- AddColorToMap -------------------------------
// --------------------------------------------------------
int ColorMapList::AddColorToMap(int mapIndex, int r, int g, int b, int a, int p)
{
	maps[mapIndex].r.Add(r);
	maps[mapIndex].g.Add(g);
	maps[mapIndex].b.Add(b);
	maps[mapIndex].a.Add(a);
	maps[mapIndex].p.Add(p);
	maps[mapIndex].numColors++;

	SortColorMapByPosition(mapIndex);

	maps[mapIndex].GenerateGLTexture();
	maps[mapIndex].GenerateGradientBitmap();

	return maps[mapIndex].numColors - 1;
}


// --------------------------------------------------------
// ---------- DeleteColorFromMap --------------------------
// --------------------------------------------------------
void ColorMapList::DeleteColorFromMap(int mapIndex, int colorIndex)
{
	maps[mapIndex].r.RemoveAt(colorIndex);
	maps[mapIndex].g.RemoveAt(colorIndex);
	maps[mapIndex].b.RemoveAt(colorIndex);
	maps[mapIndex].a.RemoveAt(colorIndex);
	maps[mapIndex].p.RemoveAt(colorIndex);
	maps[mapIndex].numColors--;

	SortColorMapByPosition(mapIndex);

	maps[mapIndex].GenerateGLTexture();
	maps[mapIndex].GenerateGradientBitmap();
}


void ColorMapList::UpdateRed(int mapIndex, int colorIndex,int red)
{
	maps[mapIndex].r[colorIndex] = red;
	maps[mapIndex].GenerateGLTexture();
	maps[mapIndex].GenerateGradientBitmap();
}

void ColorMapList::UpdateGreen(int mapIndex, int colorIndex,int green)
{
	maps[mapIndex].g[colorIndex] = green;
	maps[mapIndex].GenerateGLTexture();
	maps[mapIndex].GenerateGradientBitmap();
}

void ColorMapList::UpdateBlue(int mapIndex, int colorIndex,int blue)
{
	maps[mapIndex].b[colorIndex] = blue;
	maps[mapIndex].GenerateGLTexture();
	maps[mapIndex].GenerateGradientBitmap();
}

void ColorMapList::UpdateAlpha(int mapIndex, int colorIndex,int alpha)
{
	maps[mapIndex].a[colorIndex] = alpha;
	maps[mapIndex].GenerateGLTexture();
	maps[mapIndex].GenerateGradientBitmap();
}

void ColorMapList::UpdatePosition(int mapIndex, int colorIndex,int position)
{
	maps[mapIndex].p[colorIndex] = position;
	SortColorMapByPosition(mapIndex);
//	maps[mapIndex].GenerateGLTexture();
//	maps[mapIndex].GenerateGradientBitmap();
}