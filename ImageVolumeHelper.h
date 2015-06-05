/* ----------------------------------------------------------------------------
	ImageVolumeHelper.h


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

class ImageVolumeHelper  
{
public:
	int GetLUTSize();
	double CalculateWindowLevel(double width, double center, double value, double minVal, double maxVal);
	void DrawContrastBrightnessBar(bool rgb, int maxVal, wxSize screenSize);
	void CalculateValueLookupTable(int windowWidth, int windowCenter, double minPixel, double maxPixel, bool isRGB);

	ImageVolumeHelper(ImageData *ImgData);
	virtual ~ImageVolumeHelper();

	unsigned short int *lut16U;
	GLint texSize;

	void InitNanoTime(void);
	float GetNanoTime(void);

private:
	//LogWindow *logWindow;
	ImageData *imgData;

	int lutSize;

	bool LUTCreated;

	/* Some global variables used to measure the time */
	float timeAtGameStart;
	unsigned long long int ticksPerSecond;

	wxString msg;
};
