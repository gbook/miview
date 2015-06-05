/* ----------------------------------------------------------------------------
	ImageDataReader.h


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

#include "Color.h"
#include "XYZTriplet.h"
#include "ImageInfo.h"
#include "DicomInfo.h"
#include "NiftiInfo.h"
#include <wx/datetime.h>


/* class to load all different types of files */
class ImageDataReader
{
	friend class ImageData;
	public:
		ImageDataReader(ImageData *imgdata, ImageInfo *imgInfo, DicomInfo *dicomInfo, NiftiInfo *niftiInfo);
		~ImageDataReader(void);

		void SetFileParameters(int ftype, wxArrayString fnames, int lType, bool IsMosaic, int mosaicXSize, int mosaicYSize, int mosaicNumSlices);
		int Load();

		/* error variables */
		bool errorOccured;		//!< indicates if an error has occured
		wxString errorMsg;		//!< the error message is one occured
	private:
		/* local image objects */
		ImageData *imgData;		//!< pointer to the ImageData object
		ImageInfo *imgInfo;		//!< pointer to the ImageInfo
		DicomInfo *dcmInfo;		//!< pointer to the DicomInfo
		NiftiInfo *nftInfo;		//!< pointer to the NiftiInfo

		/* local variables */
		wxString msg;			//!< string object in which to write to logWindow
		int filetype;
		wxArrayString filenames;
		int loadType;
		bool isMosaic;			//!< if this is a DICOM mosaic image
		int mosaicX;			//!< X size of each slice of the reconstructed volume
		int mosaicY;			//!< Y size of each slice of the reconstructed volume
		int mosaicZ;			//!< number of slices in the reconstructed volume
		int XSizeOrig, YSizeOrig;	//!< size of the original mosaic image

		/* loading functions */
		wxString GetModalityDescription(wxString mode);
		int GetFileInfo(wxString filename);
		void SetRasterInfoParameters();
		wxArrayString GetOrderedDicomFileList(wxArrayString files, int loadType);
		int GetOrderedImageFileList(wxString firstfile, wxString fileprefix, bool loadAll);
		wxArrayString GetOrderedNiftiFileList(wxArrayString files, int loadType);
		int LoadDicomFilelist(wxArrayString files);
		int LoadImageFilelist();
		int LoadNiftiFilelist();

		/* utility functions */
//		bool Between(double x, double low, double high);
		void SetError(wxString errmsg);

		/* load data to buffers */
		bool LoadBINARYToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap);
		bool LoadUINT8ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap);
		bool LoadINT8ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap);
		bool LoadUINT16ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap);
		bool LoadINT16ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap);
		bool LoadUINT32ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap);
		bool LoadINT32ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap);
		bool LoadUINT64ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap);
		bool LoadINT64ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap);
		bool LoadFLOAT32ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap);
		bool LoadFLOAT64ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap);
		bool LoadFLOAT128ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap);
		bool LoadCOMPLEX64ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap);
		bool LoadCOMPLEX128ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap);
		bool LoadCOMPLEX256ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap);
		bool LoadRGB24ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap);
};
