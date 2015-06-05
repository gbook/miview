/* ----------------------------------------------------------------------------
	ImageViews.h


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

#include "XYZTriplet.h"

class ImageViews  
{
public:
	bool IsReady();
//	void SetCurrentSlice(int slice);
//	int DecreaseCurrentSlice();
//	int IncreaseCurrentSlice();
//	int GetCurrentSlice();
	ImageViews();
	virtual ~ImageViews();

//	int windowCenter, windowWidth;
	int yScreenOffset, xScreenOffset; /* offset from center at which image should be drawn */
//	int flipUD;			/* true for flipping the image up/down from original, false for original orientation */
//	int flipLR;			/* true for flipping the image left/right from original, false for original orientation */
//	int flipFB;			/* true for flipping the image front/back from original, false for original orientation */
//	int flipX, flipY, flipZ; /* not sure if these are necessary... */
	int viewType;		/* defined above */
//	bool threeDMode;	/* true for drawing in 3D, false otherwise... false for all single slice images */
//	double rotation2D;		/* rotation in degrees of 2D image, can be positive or negative */
//	XYZTriplet rotation3D;
//	unsigned short int zoomFactor; /* value from 1 - N (5% to 1000% of original size) */
//	bool zoomBestFit;	/* true if it should be drawn as best fit */
//	bool drawHighRes;	/* true if the image should be drawn in high resolution (linear interpolation) */
//	int currentSlice[3]; /* current slice in the 3 planes */
//	int currentPlane;
//	bool showInfoOverlay;
//	int multisliceRows;
//	int multisliceCols;
//	bool multisliceBestFit;
//	int multisliceSortby;
	int multislicePageNum;
//	bool showLineOverlay;
	wxColour VRColor;
//	double VRIntensity;
//	double VRDensity;
//	int VRnumSlices;
	float angle;
//	bool useColor;
//	wxColour color1;
//	wxColour color2;

	struct singlePlane {
		int zoom;
		int currentSlice;
		wxPoint centerOffset;
	} threeViewPlanes[3];

};

