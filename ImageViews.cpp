/* ----------------------------------------------------------------------------
	ImageViews.cpp


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
#include "ImageViews.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImageViews::ImageViews()
{
//	for (int i=0;i<3;i++)
//		currentSlice[i] = 0;
}

ImageViews::~ImageViews()
{

}


// --------------------------------------------------------
// ---------- GetCurrentSlice -----------------------------
// --------------------------------------------------------
//int ImageViews::GetCurrentSlice()
//{
//	return currentSlice[currentPlane];
//}


// --------------------------------------------------------
// ---------- IncreaseCurrentSlice ------------------------
// --------------------------------------------------------
// this increments the current slice for the current plane
// its assumed that #slices in the plane was checked before
// calling this function, so the new slice isn't out of range
//int ImageViews::IncreaseCurrentSlice()
//{
//	currentSlice[currentPlane]++;
//	return currentSlice[currentPlane];
//}


// --------------------------------------------------------
// ---------- DecreaseCurrentSlice ------------------------
// --------------------------------------------------------
// this decrements the current slice for the current plane
// its assumed that #slices in the plane was checked before
// calling this function, so the new slice isn't out of range
//int ImageViews::DecreaseCurrentSlice()
//{
//	currentSlice[currentPlane]--;
//	return currentSlice[currentPlane];
//}

//void ImageViews::SetCurrentSlice(int slice)
//{
//	currentSlice[currentPlane] = slice;
//}
