/* ----------------------------------------------------------------------------
	ImageVolumeHelper.cpp


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
#include "Color.h"
#include "ImageData.h"
//#include "LogWindow.h"
#include "ImageVolumeHelper.h"
#include <iostream>
#include "math.h"
#include "stdio.h"
#include <mmsystem.h>

extern wxTextCtrl *logWnd;

#include <crtdbg.h>
#define new new(_NORMAL_BLOCK,__FILE__, __LINE__)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImageVolumeHelper::ImageVolumeHelper(ImageData *ImgData)
{
	//int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	//flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
	//_CrtSetDbgFlag(flag); // Set flag to the new value

	//logWindow = &logwind;
	imgData = ImgData;
	LUTCreated = false;
	InitNanoTime();

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
}

ImageVolumeHelper::~ImageVolumeHelper()
{
	if (LUTCreated)
		delete[] lut16U;
}


// --------------------------------------------------------
// ---------- CalculateValueLookupTable -------------------
// --------------------------------------------------------
//void ImageVolumeHelper::CalculateValueLookupTable(int contrast, int brightness, int maxPixel)
void ImageVolumeHelper::CalculateValueLookupTable(int windowWidth, int windowCenter, double minPixel, double maxPixel, bool isRGB)
{
	WriteLog("ImageVolumeHelper->CalculateValueLookupTable() called...\n");

	int i;
	double maxPixel16U;
	double multA, multB;
	double valueRangeSize;
	double valA;

	maxPixel16U = pow(2.0, 16.0) - 1.0;

	/* get the size of the entire range of the values in the image */
	valueRangeSize = maxPixel - minPixel;

	WriteLog(msg.Format("Range size %.4f [%.4f - %.4f]\n", valueRangeSize, minPixel, maxPixel));

	/* if the lookup table has not been allocated yet, create it */
	if (!LUTCreated)
		lut16U = new unsigned short int[texSize];

	WriteLog(msg.Format("Created LUT array of size: %d\n", texSize));
	lutSize = texSize;

	/*	TWO levels of mapping values below:
		example: image values are [-2000 1800], display range is 16-bit -or- [0 65535],
		which is the display range of the lookup table
		But the eventual lookup table will have a size of 4095 (the texture size limit)
		so... there will only be 4096 values in the lookup table. These are the 2 steps to make it happen:
		1) map [0 4095] to [0 1800-(-2000)]
		4) map [0 1800-(-2000)] to [0 65535]<- with window level applied
	*/
	/* create the multipliers */
	multA = (valueRangeSize/(double)texSize);
	multB = maxPixel16U/valueRangeSize;
	/* loop through the texture size and calculate the loopup table values */
	for (i=0;i<texSize;i++) {
		valA = (double)i * multA;
		lut16U[i] = (unsigned short int)CalculateWindowLevel(windowWidth-minPixel, windowCenter-minPixel, valA, 0.0, maxPixel-minPixel)*multB;
	}
	LUTCreated = true;
}


// --------------------------------------------------------
// ---------- CalculateWindowLevel ------------------------
// --------------------------------------------------------
double ImageVolumeHelper::CalculateWindowLevel(double width, double center, double value, double minVal, double maxVal)
{
	double retVal;

	if (width < 0.5) retVal = minVal;
	else retVal = ( (value - center + 0.5)/(width-1.0) + 0.5)*(maxVal - minVal) + minVal;

	if (retVal > maxVal) retVal = maxVal;
	if (retVal < minVal) retVal = minVal;

	return retVal;
}


// --------------------------------------------------------
// ---------- DrawContrastBrightnessBar -------------------
// --------------------------------------------------------
void ImageVolumeHelper::DrawContrastBrightnessBar(bool rgb, int maxVal, wxSize screenSize)
{
	WriteLog("ImageVolumeHelper->DrawContrastBrightnessBar() called...\n");

	glShadeModel(GL_SMOOTH);

//	int width = screenSize.GetWidth();
	int height = screenSize.GetHeight();
	int i;
	float x1,x2,y1,y2;
	double maxPixel16U = pow(2.0, 16.0) - 1.0;

	glBegin(GL_QUADS);
	if (rgb) {

	}
	else {
//		WriteLog( msg.Format("DrawContrastBrightnessBar checkpoint 1: %dms (maxval: %d)\n",clock(), maxVal) );
		double barheight = height * 0.75;
		double barstart = (height - barheight)/2;

		/* draw grayscale color bar */
		for (i=0;i<texSize;i++) {
			glColor3d((double)lut16U[i]/maxPixel16U, (double)lut16U[i]/maxPixel16U, (double)lut16U[i]/maxPixel16U);
			x1 = (double)10;
			y1 = (double)(i*barheight)/texSize + barstart;
			y2 = (double)((i+1)*barheight)/texSize + barstart;
			x2 = (double)20;
			glVertex2f(x1,y1); glVertex2f(x1,y2); glVertex2f(x2,y2); glVertex2f(x2,y1);
		}
//		WriteLog( msg.Format("DrawContrastBrightnessBar checkpoint 2: %dms\n",clock()) );
	}
	glEnd();
}


// --------------------------------------------------------
// ---------- GetLUTSize ----------------------------------
// --------------------------------------------------------
int ImageVolumeHelper::GetLUTSize()
{
	return lutSize;
}


// --------------------------------------------------------
// ---------- InitNanoTime --------------------------------
// --------------------------------------------------------
/* Called once at the start of the game */
void ImageVolumeHelper::InitNanoTime(void)
{
	/* We need to know how often the clock is updated */
	if( !QueryPerformanceFrequency((LARGE_INTEGER *)&ticksPerSecond) )
		ticksPerSecond = 1000;

	/* If timeAtGameStart is 0 then we get the time since
	   the start of the computer when we call GetGameTime() */
	timeAtGameStart = 0;
	timeAtGameStart = GetNanoTime();
}


// --------------------------------------------------------
// ---------- GetNanoTime ---------------------------------
// --------------------------------------------------------
/* Called every time you need the current game time */
float ImageVolumeHelper::GetNanoTime(void)
{
	unsigned long long int ticks;
	float time;

	// This is the number of clock ticks since start
	if( !QueryPerformanceCounter((LARGE_INTEGER *)&ticks) )
		ticks = (unsigned long long int)timeGetTime();

	// Divide by frequency to get the time in seconds
	time = (float)(__int64)ticks/(float)(__int64)ticksPerSecond;

	/* Subtract the time at game start to get the time since the game started */
	time -= timeAtGameStart;

	return time;
}
