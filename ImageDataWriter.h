/* ----------------------------------------------------------------------------
	ImageDataWriter.h


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

#include "Color.h"
#include "XYZTriplet.h"
#include "ImageInfo.h"
#include "DicomInfo.h"
#include "NiftiInfo.h"
#include "wx/filename.h"

/* class to save all types of files */
class ImageDataWriter
{
	public:
		ImageDataWriter(ImageData *imgdata, ImageInfo *imgInfo, DicomInfo *dicomInfo, NiftiInfo *niftiInfo);
		~ImageDataWriter(void);

		void SetFileParameters(int outType, int convertType, wxFileName dir, wxString fname, int serNum);
		bool Write();

	private:
		/* local image objects */
		ImageData *imgData;
		ImageInfo *imgInfo;
		DicomInfo *dcmInfo;
		NiftiInfo *nftInfo;
		wxString msg;

		int outputType;
		int outConvertType;
		wxFileName outDir;
		wxString outFilename;
		int seriesNum;
		wxString seriesUIDValue;
		wxString studyUIDValue;

		/* routines for saving images */
		bool WriteSingleDicomImage();
		bool WriteMultiFileDicomImage();
		bool SaveNiftiImage();
		bool SaveRasterImage();

		/* variables */
		wxString filename;
		wxString dirPath;
		int fileType;
};
