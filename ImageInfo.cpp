/* ----------------------------------------------------------------------------
	ImageInfo.cpp


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
//#include "LogWindow.h"
#include "ImageData.h"

#include "ImageInfo.h"

ImageInfo::ImageInfo(void)
{
}

ImageInfo::~ImageInfo(void)
{
}

void ImageInfo::InitializeVariables() {
	dirPath = "";
	minPixelValues = Color(DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX);
	maxPixelValues = Color(DBL_MIN, DBL_MIN, DBL_MIN, DBL_MIN);
	ImageXSize = ImageYSize = ImageZSize = ImageTSize = 1;
	VolumeXSize = VolumeYSize = VolumeZSize = VolumeTSize = 1;
	origin = XYZTriplet(0.0, 0.0, 0.0);

	PixelByteSize = 0;
	BitsStored = 0;
	BitsAllocated = 0;
	HighBitPosition = 0;
	SamplesPerPixel = 0;
	IsRGB = false;
	IsSignedPixelData = false;
	IsMultiSlice = false;
	IsMultiFile = false;
	IsMonochrome = false;
//	PlanarConfiguration = 0;
}


// --------------------------------------------------------
// ---------- GetMaxPixelValue ----------------------------
// --------------------------------------------------------
double ImageInfo::GetMaxPixelValue()
{
	double maxPixVal = 0.0;

	/* compare red and green */
	if (maxPixelValues.Red() > maxPixelValues.Green())
		maxPixVal = maxPixelValues.Red();
	else
		maxPixVal = maxPixelValues.Green();

	/* compare the previous max to the blue */
	if (maxPixVal > maxPixelValues.Blue())
		maxPixVal = maxPixVal;
	else
		maxPixVal = maxPixelValues.Blue();

	return maxPixVal;
}


Color ImageInfo::GetMinPixelValues() { return minPixelValues; }
Color ImageInfo::GetMaxPixelValues() { return maxPixelValues; }
bool ImageInfo::GetIsMultiSlice() { return IsMultiSlice; }
bool ImageInfo::GetIsMultiFile() { return IsMultiFile; }
bool ImageInfo::GetIsMonochrome() { return IsMonochrome; }
bool ImageInfo::GetIsRGB() { return IsRGB; }
bool ImageInfo::GetIsSignedPixelData() { return IsSignedPixelData; }
int ImageInfo::GetPixelType() { return PixelType; }
int ImageInfo::GetSamplesPerPixel() { return SamplesPerPixel; }
int ImageInfo::GetPixelByteSize() { return PixelByteSize; }
int ImageInfo::GetBitsStored() { return BitsStored; }
int ImageInfo::GetBitsAllocated() { return BitsAllocated; }
int ImageInfo::GetHighBitPosition() { return HighBitPosition; }
int ImageInfo::GetPlanarConfiguration() { return PlanarConfiguration; }
wxString ImageInfo::GetPixelTypeStr() { return PixelTypeStr; }  /* can be: 8U, 8S, 16U, 16S, 32U, 32S, FD */
wxString ImageInfo::GetDirPath() { return dirPath; }
wxString ImageInfo::GetFilePrefix() { return filePrefix; }
wxArrayString ImageInfo::GetFileList() { return fileList; }
int ImageInfo::GetImageXSize() { return ImageXSize; }
int ImageInfo::GetImageYSize() { return ImageYSize; }
int ImageInfo::GetImageZSize() { return ImageZSize; }
int ImageInfo::GetImageTSize() { return ImageTSize; }
int ImageInfo::GetVolumeXSize() { return VolumeXSize; }
int ImageInfo::GetVolumeYSize() { return VolumeYSize; }
int ImageInfo::GetVolumeZSize() { return VolumeZSize; }
int ImageInfo::GetVolumeTSize() { return VolumeTSize; }
XYZTriplet ImageInfo::GetOrigin() { return origin; }
XYZTriplet ImageInfo::GetSpacing() { return spacing; }
int ImageInfo::GetFileType() { return fileType; }
int ImageInfo::GetImageType() { return imageType; }
int ImageInfo::GetVolumeType() { return volumeType; }


void ImageInfo::SetMinPixelValues(Color value) { minPixelValues = value; }
void ImageInfo::SetMaxPixelValues(Color value) { maxPixelValues = value; }
void ImageInfo::SetIsMultiSlice(bool value) { IsMultiSlice = value; }
void ImageInfo::SetIsMultiFile(bool value) { IsMultiFile = value; }
void ImageInfo::SetIsMonochrome(bool value) { IsMonochrome = value; }
void ImageInfo::SetIsRGB(bool value) { IsRGB = value; }
void ImageInfo::SetIsSignedPixelData(bool value) { IsSignedPixelData = value; }
void ImageInfo::SetPixelType(int value) { PixelType = value; }
void ImageInfo::SetSamplesPerPixel(int value) { SamplesPerPixel = value; }
void ImageInfo::SetPixelByteSize(int value) { PixelByteSize = value; }
void ImageInfo::SetBitsStored(int value) { BitsStored = value; }
void ImageInfo::SetBitsAllocated(int value) { BitsAllocated = value; }
void ImageInfo::SetHighBitPosition(int value) { HighBitPosition = value; }
void ImageInfo::SetPlanarConfiguration(int value) { PlanarConfiguration = value; }
void ImageInfo::SetPixelTypeStr(wxString value) { PixelTypeStr = value; }
void ImageInfo::SetDirPath(wxString value) { dirPath = value; }
void ImageInfo::SetFilePrefix(wxString value) { filePrefix = value; }
void ImageInfo::SetFileList(wxArrayString value) { fileList = value; }
void ImageInfo::SetImageXSize(int value) { ImageXSize = value; }
void ImageInfo::SetImageYSize(int value) { ImageYSize = value; }
void ImageInfo::SetImageZSize(int value) { ImageZSize = value; }
void ImageInfo::SetImageTSize(int value) { ImageTSize = value; }
void ImageInfo::SetVolumeXSize(int value) { VolumeXSize = value; }
void ImageInfo::SetVolumeYSize(int value) { VolumeYSize = value; }
void ImageInfo::SetVolumeZSize(int value) { VolumeZSize = value; }
void ImageInfo::SetVolumeTSize(int value) { VolumeTSize = value; }
void ImageInfo::SetOrigin(XYZTriplet value) { origin = value; }
void ImageInfo::SetSpacing(XYZTriplet value) { spacing = value; }
void ImageInfo::SetFileType(int value) { fileType = value; }
void ImageInfo::SetImageType(int value) { imageType = value; }
void ImageInfo::SetVolumeType(int value) { volumeType = value; }
