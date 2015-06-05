/* ----------------------------------------------------------------------------
	ImageData.h


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

#include "DataContainers.h"
#include "ImageDataReader.h"
#include "ImageDataWriter.h"
#include "Color.h"
#include "XYZTriplet.h"
#include "ImageInfo.h"
#include "DicomInfo.h"
#include "NiftiInfo.h"
#include <wx/datetime.h>
#include <wx/filename.h>

/* forward declarations */
class Color;
class DicomInfo;
class LoadErrorDialog;
class ImageDataReader;

class ImageData
{
public:
	/* constructor and destructor */
	ImageData(MainFrameVariables *MainVars);
	virtual ~ImageData();

	/* public functions */
	void SetLoadParameters(int ftype, wxArrayString fnames,int ltype, bool isMos, int mosXSize, int mosYSize, int mosNumSlices);
	int Load();

	void SetSaveParameters(int outputType, int convertType, wxFileName dir, wxString filename, int i);
	int Save();
	void ShowFileInfo();

	/* GETs and information functions */
	bool DataIsValid();
	int GetFileType();
	int GetTotalBytesAllocated();
	wxArrayString GetFilenames();

	/* object to hold the important image information */
	ImageInfo *imgInfo;
	
	/* classes to hold all the specific special tags */
	DicomInfo *dcmInfo;
	NiftiInfo *niftiInfo;

	MainFrameVariables *mainVars;

	/* data storage containers (gray scale) */
	DataBuffer<UINT_1> *imgDataBINARY;
	DataBuffer<UINT_8> *imgDataUINT8;
	DataBuffer<INT_8> *imgDataINT8;
	DataBuffer<UINT_16> *imgDataUINT16;
	DataBuffer<INT_16> *imgDataINT16;
	DataBuffer<UINT_32> *imgDataUINT32;
	DataBuffer<INT_32> *imgDataINT32;
	DataBuffer<UINT_64> *imgDataUINT64;
	DataBuffer<INT_64> *imgDataINT64;
	DataBuffer<FLOAT_32> *imgDataFLOAT32;
	DataBuffer<FLOAT_64> *imgDataFLOAT64;
	DataBuffer<FLOAT_128> *imgDataFLOAT128;

	/* data storage containers (complex: real & imaginary) */
	DataBuffer<FLOAT_32> *imgDataCOMPLEX64;
	DataBuffer<FLOAT_64> *imgDataCOMPLEX128;
	DataBuffer<FLOAT_128> *imgDataCOMPLEX256;

	/* data storage containers (RGB) */
	DataBuffer<UINT_8> *imgDataRGB24;

	/* data manipulation functions */
	void FlipDataX();
	void FlipDataY();
	void FlipDataZ();

	int fileType;

private:
	/* private functions */
	void DisplayData();
	bool DisplayErrorDialog();
	void GetOrientation();
	void InitializeVariables();
	void PrintFileInfo();

	/* variables pertaining to loading */
	wxArrayString filenames;
	int loadType;
	bool isMosaic;
	int mosaicXSize;
	int mosaicYSize;
	int mosaicNumSlices;

	/* variables for saving */
	int outType;
	int outConvertType;
	wxFileName outDir;
	wxString outFilename;
	int seriesNum;

	/* private variables */
	bool dataIsValid;
	bool dataHasBeenLoaded; /* flag to tell if anything has been loaded... so we dont bonk on the delete[] section */
	wxString msg;
	int totalBytesAllocated; /* returned from LoadFile() */

	/* local pointers to remote objects */
	//LogWindow *logWindow;
	LoadErrorDialog *dialog;

	/* variables used when a loading error occurs, or before an error */
	bool ignoreLoadError;	/* ignore a load error... ie, reloading a dataset */
	bool loadError;			/* an error occured while loading the dataset */
	wxString errorMessage;	/* the error message is an error occured */
	bool allowReload;		/* the error is not critical, so allow the user to choose to load the data anyway */

	/* forward declare the ImageDataReader class... and let it see ImageData's private members */
	friend class ImageDataReader;
	ImageDataReader* loadFile;
};
