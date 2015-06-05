/* ----------------------------------------------------------------------------
	ImageInfo.h


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

/* includes */
#include "Color.h"
#include "XYZTriplet.h"

class ImageInfo
{
public:
	ImageInfo(void);
	~ImageInfo(void);

	/* GETs */
	Color GetMinPixelValues();
	Color GetMaxPixelValues();
	double GetMaxPixelValue();
	bool GetIsMultiSlice();
	bool GetIsMultiFile();
	bool GetIsMonochrome();
	bool GetIsRGB();
	bool GetIsSignedPixelData();
	int GetPixelType();
	int GetSamplesPerPixel();
	int GetPixelByteSize();
	int GetBitsStored();
	int GetBitsAllocated();
	int GetHighBitPosition();
	int GetPlanarConfiguration();
	wxString GetPixelTypeStr();  /* can be: 8U, 8S, 16U, 16S, 32U, 32S, FD */
	wxString GetDirPath();
	wxString GetFilePrefix();
	wxArrayString GetFileList();
	int GetImageXSize();
	int GetImageYSize();
	int GetImageZSize();
	int GetImageTSize();
	int GetVolumeXSize();
	int GetVolumeYSize();
	int GetVolumeZSize();
	int GetVolumeTSize();
	XYZTriplet GetOrigin();
	XYZTriplet GetSpacing();
	int GetFileType();
	int GetImageType();
	int GetVolumeType();

	/* SETs */
	void SetMinPixelValues(Color value);
	void SetMaxPixelValues(Color value);
	void SetIsMultiSlice(bool value);
	void SetIsMultiFile(bool value);
	void SetIsMonochrome(bool value);
	void SetIsRGB(bool value);
	void SetIsSignedPixelData(bool value);
	void SetPixelType(int value);
	void SetSamplesPerPixel(int value);
	void SetPixelByteSize(int value);
	void SetBitsStored(int value);
	void SetBitsAllocated(int value);
	void SetHighBitPosition(int value);
	void SetPlanarConfiguration(int value);
	void SetPixelTypeStr(wxString value);  /* can be: 8U, 8S, 16U, 16S, 32U, 32S, FD */
	void SetDirPath(wxString value);
	void SetFilePrefix(wxString value);
	void SetFileList(wxArrayString value);
	void SetImageXSize(int value);
	void SetImageYSize(int value);
	void SetImageZSize(int value);
	void SetImageTSize(int value);
	void SetVolumeXSize(int value);
	void SetVolumeYSize(int value);
	void SetVolumeZSize(int value);
	void SetVolumeTSize(int value);
	void SetOrigin(XYZTriplet value);
	void SetSpacing(XYZTriplet value);
	void SetFileType(int value);
	void SetImageType(int value);
	void SetVolumeType(int value);

	/* public variables */
	wxArrayString fileList;		/* list of file(s) in the volume set */
	Color minPixelValues;	/* minimum pixel values */
	Color maxPixelValues;	/* maximum pixel values */

private:
	void InitializeVariables();

	/* image specific information */
	bool IsMultiSlice; /* true for volumes with 2 or more slices, false for single slice images */
	bool IsMultiFile;  /* true if more than one file makes up the image/volume */
	bool IsMonochrome;
	bool IsRGB;
	bool IsSignedPixelData;
	int PixelType;  /* can be: 8U, 8S, 16U, 16S, 32U, 32S, FD */
	int SamplesPerPixel; /* samples per pixel; 1-gray, 3-RGB */
	int PixelByteSize; /* size of a pixel in bytes */
	int BitsStored;    /* number of bits stored: usually 12 bits in dicom */
	int BitsAllocated; /* number of bits allocated */
	int HighBitPosition; /* the high bit position */
	int PlanarConfiguration;
	wxString PixelTypeStr;	/* can be: 8U, 8S, 16U, 16S, 32U, 32S, FD */
	wxString dirPath;		/* path where the first file was read from */
	wxString filePrefix;	/* list of file(s) in the volume set */
	/* sizes for each individual file */
	int ImageXSize;
	int ImageYSize;
	int ImageZSize;
	int ImageTSize;
	/* sizes for the complete volume made from all the image files */
	int VolumeXSize;
	int VolumeYSize;
	int VolumeZSize;
	int VolumeTSize;
	/* dicom, analyze, etc */
	int fileType;
	/* either 2D, 3D, or 4D */
	int imageType;
	int volumeType;
	XYZTriplet origin;
	XYZTriplet spacing;
};
