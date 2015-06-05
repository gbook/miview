/* ----------------------------------------------------------------------------
	ImageData.cpp


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
#include "MainFrameVariables.h"
//#include "LogWindow.h"
#include "ImageData.h"
#include "LoadErrorDialog.h"
#include "wx/dir.h"
#include "wx/File.h"
#include "wx/busyinfo.h"
#include "wx/progdlg.h"
#include <iostream>
#include "math.h"
#include "stdio.h"
#include "ImageInformationDialog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// --------------------------------------------------------
// ---------- ImageData -----------------------------------
// --------------------------------------------------------
//ImageData::ImageData(wxString imgFiles, bool multiFile, LogWindow &logwind)
ImageData::ImageData(MainFrameVariables *MainVars)
{
	//int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	//flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
	//_CrtSetDbgFlag(flag); // Set flag to the new value

	dataIsValid = false;

	//logWindow = logwind;
	mainVars = MainVars;
	InitializeVariables();
}


// --------------------------------------------------------
// ---------- ~ImageData ----------------------------------
// --------------------------------------------------------
ImageData::~ImageData()
{
	dataIsValid = false;

	/* gotta check if data was actually loaded before trying to delete it */
	if (dataHasBeenLoaded) {
		
		/* setup the progress dialog */
		wxProgressDialog progDlg(wxT("Unloading Data"), wxT(" "), imgInfo->GetVolumeYSize(), NULL, wxPD_APP_MODAL | wxPD_SMOOTH);

//		if (logWindow != NULL)
//			WriteLog(msg.Format("~ImageData: %ds\n", imgInfo->GetPixelType()));

		switch (imgInfo->GetPixelType()) {
			case IMG_UINT8: delete imgDataUINT8; break;
			case IMG_INT8: delete imgDataINT8; break;
			case IMG_UINT16: delete imgDataUINT16; break;
			case IMG_INT16: delete imgDataINT16; break;
			case IMG_UINT32: delete imgDataUINT32; break;
			case IMG_INT32: delete imgDataINT32; break;
			case IMG_UINT64: delete imgDataUINT64; break;
			case IMG_INT64: delete imgDataINT64; break;
			case IMG_FLOAT32: delete imgDataFLOAT32; break;
			case IMG_FLOAT64: delete imgDataFLOAT64; break;
			case IMG_FLOAT128: delete imgDataFLOAT128; break;
			case IMG_COMPLEX64: delete imgDataCOMPLEX64; break;
			case IMG_COMPLEX128: delete imgDataCOMPLEX128; break;
			case IMG_COMPLEX256: delete imgDataCOMPLEX256; break;
			//case IMG_FD: break;
			case IMG_RGB24: delete imgDataRGB24; break;
		}
	}
	delete imgInfo;
	delete dcmInfo;
	delete niftiInfo;
}


// --------------------------------------------------------
// ---------- InitializeVariables -------------------------
// --------------------------------------------------------
void ImageData::InitializeVariables()
{
	WriteLog("ImageData->InitializeVariables() called...");

	/* setup the info objects */
	imgInfo = new ImageInfo();
	dcmInfo = new DicomInfo();
	niftiInfo = new NiftiInfo();
	
	dataHasBeenLoaded = false;
	totalBytesAllocated = 0;

	/* load-error variables */
	ignoreLoadError = false;
	loadError = false;
	errorMessage = "";
	allowReload = false;
}


// --------------------------------------------------------
// ---------- PrintFileInfo -------------------------------
// --------------------------------------------------------
void ImageData::PrintFileInfo()
{
	wxString fileInfo;
	wxString tmpStr;

	fileInfo = "------------------------- FILE INFO -------------------------\n";
	fileInfo += tmpStr.Format("Directory Path: %s\n",imgInfo->GetDirPath().c_str());
	fileInfo += tmpStr.Format("File Prefix: %s\n",imgInfo->GetFilePrefix().c_str());
	fileInfo += tmpStr.Format("Image Size: %dx%dx%dx%d\n",imgInfo->GetImageXSize(), imgInfo->GetImageYSize(), imgInfo->GetImageZSize(), imgInfo->GetImageTSize());
	if (imgInfo->GetImageType() == IMG_2D)
		fileInfo += tmpStr.Format("Bytes per image (2D): %d\n",imgInfo->GetImageXSize() * imgInfo->GetImageYSize() * imgInfo->GetPixelByteSize());
	if (imgInfo->GetImageType() == IMG_3D)
		fileInfo += tmpStr.Format("Bytes per image (3D): %d\n",imgInfo->GetImageXSize() * imgInfo->GetImageYSize() * imgInfo->GetImageZSize() * imgInfo->GetPixelByteSize());
	if (imgInfo->GetImageType() == IMG_4D)
		fileInfo += tmpStr.Format("Bytes per image (4D): %d\n",imgInfo->GetImageXSize() * imgInfo->GetImageYSize() * imgInfo->GetImageZSize() * imgInfo->GetImageTSize() * imgInfo->GetPixelByteSize());
	fileInfo += tmpStr.Format("Bytes total: %d\n",imgInfo->GetImageXSize() * imgInfo->GetImageYSize() * imgInfo->GetImageZSize() * imgInfo->GetImageTSize() * imgInfo->GetPixelByteSize());
	fileInfo += tmpStr.Format("Image origin: %d, %d, %d\n",imgInfo->GetOrigin().x, imgInfo->GetOrigin().y, imgInfo->GetOrigin().z);
	fileInfo += tmpStr.Format("Max pixel values: (R%.2f, G%.2f, B%.2f) Gray: %.2f\n",imgInfo->GetMaxPixelValues().Red(), imgInfo->GetMaxPixelValues().Green(), imgInfo->GetMaxPixelValues().Blue(), imgInfo->GetMaxPixelValues().Gray());
	fileInfo += tmpStr.Format("Min pixel values: (R%.2f, G%.2f, B%.2f) Gray: %.2f\n",imgInfo->GetMinPixelValues().Red(), imgInfo->GetMinPixelValues().Green(), imgInfo->GetMinPixelValues().Blue(), imgInfo->GetMinPixelValues().Gray());
	fileInfo += tmpStr.Format("Bytes per pixel: %d\n",imgInfo->GetPixelByteSize());
	fileInfo += tmpStr.Format("Bits Stored: %d\n",imgInfo->GetBitsStored());
	fileInfo += tmpStr.Format("Bits Allocated: %d\n",imgInfo->GetBitsAllocated());
	fileInfo += tmpStr.Format("High Bit Position: %d\n",imgInfo->GetHighBitPosition());
	fileInfo += tmpStr.Format("Samples per pixel: %d\n",imgInfo->GetSamplesPerPixel());
	fileInfo += tmpStr.Format("Pixel Type: %s\n",imgInfo->GetPixelTypeStr().c_str());
	if (fileType == FILETYPE_DICOM) {
		fileInfo += tmpStr.Format("Swap code: %d\n",dcmInfo->GetSwapCode());
	//	fileInfo += tmpStr.Format("Planar Configuration: %d\n",PlanarConfiguration);
		fileInfo += tmpStr.Format("Patient Name: %s\n",dcmInfo->GetPatientName().c_str());
		fileInfo += tmpStr.Format("Patient ID: %s\n",dcmInfo->GetPatientID().c_str());
		fileInfo += tmpStr.Format("Patient Birthdate: %s\n",dcmInfo->GetPatientBirthdate().c_str());
		fileInfo += tmpStr.Format("Patient Position: %s\n",dcmInfo->GetPatientPositionStr().c_str());
	}
	fileInfo += "------------------------- end file info -------------------------\n";

	WriteLog(fileInfo);
}


// --------------------------------------------------------
// ---------- GetFileType ---------------------------------
// --------------------------------------------------------
int ImageData::GetFileType() {
	return fileType;
}


// --------------------------------------------------------
// ---------- GetFilenames --------------------------------
// --------------------------------------------------------
wxArrayString ImageData::GetFilenames() {
	return filenames;
}


// --------------------------------------------------------
// ---------- ShowFileInfo --------------------------------
// --------------------------------------------------------
void ImageData::ShowFileInfo()
{
	WriteLog("Creating the file info window...");
    ImageInformationDialog *imgInfoDialog = new ImageInformationDialog(NULL);
	imgInfoDialog->SetInfoPointers(this,imgInfo,dcmInfo,niftiInfo);
}


// --------------------------------------------------------
// ---------- DataIsValid ---------------------------------
// --------------------------------------------------------
bool ImageData::DataIsValid()
{
	return dataIsValid;
}


// --------------------------------------------------------
// ---------- GetTotalBytesAllocated ----------------------
// --------------------------------------------------------
int ImageData::GetTotalBytesAllocated()
{
	return totalBytesAllocated;
}


// --------------------------------------------------------
// ---------- DisplayErrorDialog --------------------------
// --------------------------------------------------------
bool ImageData::DisplayErrorDialog()
{
	bool reload = false;

//	LoadErrorDialog dialog(wxWindow::FindWindowById(MAIN_FRAME_ID), LOADERROR_DIALOG_ID, "Medical Image Viewer", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, errorMessage);

//	if (dialog.ShowModal() == wxID_OK) {
//		reload = dialog.GetContinueAnswer();
//		WriteLog(msg.Format("GetContinueAnswer() returned (%d)...",reload));
//	}
//	else {
		//else: dialog was cancelled or some another button pressed
//	}

//	dialog.Destroy();

	return reload;
}


// --------------------------------------------------------
// ---------- SetLoadParameters ---------------------------
// --------------------------------------------------------
void ImageData::SetLoadParameters(int ftype, wxArrayString fnames,int ltype, bool isMos, int mosXSize, int mosYSize, int mosNumSlices)
{
	fileType = ftype;
	filenames = fnames;
	loadType = ltype;
	isMosaic = isMos;
	mosaicXSize = mosXSize;
	mosaicYSize = mosYSize;
	mosaicNumSlices = mosNumSlices;

	WriteLog("ImageData->SetLoadParameters has been called...");
}


// --------------------------------------------------------
// ---------- Load ----------------------------------------
// --------------------------------------------------------
int ImageData::Load()
{
	int ret = 1;
	ImageDataReader *r = new ImageDataReader(this, imgInfo, dcmInfo, niftiInfo);

	r->SetFileParameters(fileType,filenames,loadType,isMosaic,mosaicXSize,mosaicYSize,mosaicNumSlices);
	if (!r->Load()) {
		wxMessageBox(r->errorMsg,"MIView encountered the following error(s)...");
		ret = 0;
	}

	delete r;
	return ret;
}


// --------------------------------------------------------
// ---------- SetSaveParameters ---------------------------
// --------------------------------------------------------
void ImageData::SetSaveParameters(int outputType, int convertType, wxFileName dir, wxString filename, int i)
{
	outType = outputType;
	outConvertType = convertType;
	outDir = dir;
	outFilename = filename;
	seriesNum = i;
}

// --------------------------------------------------------
// ---------- Save ----------------------------------------
// --------------------------------------------------------
int ImageData::Save()
{
	int ret = 1;
	ImageDataWriter *w = new ImageDataWriter(this, imgInfo, dcmInfo, niftiInfo);

	w->SetFileParameters(outType, outConvertType, outDir, outFilename, seriesNum);
	if (!w->Write()) {
		wxMessageBox("Couldn't save file","MIView encountered the following error(s)...");
		ret = 0;
	}

	delete w;
	return ret;
}


// --------------------------------------------------------
// ---------- DisplayData ---------------------------------
// --------------------------------------------------------
void ImageData::DisplayData()
{
	int i;
	int cols = imgInfo->GetImageXSize();
	int rows = imgInfo->GetImageYSize();
	int slices = imgInfo->GetImageZSize();

	int col, row, slice;

	for (row=0;row<rows;row++) {
		for (col=0;col<cols;col++) {
			for (slice=0;slice<slices;slice++) {
				switch (imgInfo->GetPixelType()) {
					case IMG_BINARY: WriteLog( msg.Format("DataValue: (%d,%d,%d,0) %d",row,col,slice,imgDataBINARY->Get(row,col,slice,0,0)) ); break;
					case IMG_UINT8: WriteLog( msg.Format("DataValue: (%d,%d,%d,0) %d",row,col,slice,imgDataUINT8->Get(row,col,slice,0,0)) ); break;
					case IMG_INT8: WriteLog( msg.Format("DataValue: (%d,%d,%d,0) %d",row,col,slice,imgDataINT8->Get(row,col,slice,0,0)) ); break;
					case IMG_UINT16: WriteLog( msg.Format("DataValue: (%d,%d,%d,0) %d",row,col,slice,imgDataUINT16->Get(row,col,slice,0,0)) ); break;
					case IMG_INT16: WriteLog( msg.Format("DataValue: (%d,%d,%d,0) %d",row,col,slice,imgDataINT16->Get(row,col,slice,0,0)) ); break;
					case IMG_UINT32: WriteLog( msg.Format("DataValue: (%d,%d,%d,0) %d",row,col,slice,imgDataUINT32->Get(row,col,slice,0,0)) ); break;
					case IMG_INT32: WriteLog( msg.Format("DataValue: (%d,%d,%d,0) %d",row,col,slice,imgDataINT32->Get(row,col,slice,0,0)) ); break;
					case IMG_UINT64: WriteLog( msg.Format("DataValue: (%d,%d,%d,0) %d",row,col,slice,imgDataUINT64->Get(row,col,slice,0,0)) ); break;
					case IMG_INT64: WriteLog( msg.Format("DataValue: (%d,%d,%d,0) %d",row,col,slice,imgDataINT64->Get(row,col,slice,0,0)) ); break;
					case IMG_FLOAT32: WriteLog( msg.Format("DataValue: (%.4f,%.4f,%.4f,0) %d",row,col,slice,imgDataFLOAT32->Get(row,col,slice,0,0)) ); break;
					case IMG_FLOAT64: WriteLog( msg.Format("DataValue: (%.4f,%.4f,%.4f,0) %d",row,col,slice,imgDataFLOAT64->Get(row,col,slice,0,0)) ); break;
					case IMG_FLOAT128: WriteLog( msg.Format("DataValue: (%.4f,%.4f,%.4f,0) %d",row,col,slice,imgDataFLOAT128->Get(row,col,slice,0,0)) ); break;
					case IMG_COMPLEX64:
						for (i=0;i<2;i++)
							WriteLog( msg.Format("DataValue: (%d,%d,%d,0,[%d]) %d",row,col,slice,i,imgDataCOMPLEX64->Get(col,row,slice,0,i)) );
						break;
					case IMG_COMPLEX128:
						for (i=0;i<2;i++)
							WriteLog( msg.Format("DataValue: (%d,%d,%d,0,[%d]) %d",row,col,slice,i,imgDataCOMPLEX128->Get(col,row,slice,0,i)) );
						break;
					case IMG_COMPLEX256:
						for (i=0;i<2;i++)
							WriteLog( msg.Format("DataValue: (%d,%d,%d,0,[%d]) %d",row,col,slice,i,imgDataCOMPLEX256->Get(col,row,slice,0,i)) );
						break;
					case IMG_RGB24:
						for (i=0;i<3;i++)
							WriteLog( msg.Format("DataValue: (%d,%d,%d,0,[%d]) %d",row,col,slice,i,imgDataRGB24->Get(col,row,slice,0,i)) );
						break;
					default:
						break;
				}
			}
		}
	}
}


// --------------------------------------------------------
// ---------- FlipDataX -----------------------------------
// --------------------------------------------------------
void ImageData::FlipDataX()
{
	int x, y, z;
	int xSize = imgInfo->GetVolumeXSize();
	int ySize = imgInfo->GetVolumeYSize();
	int zSize = imgInfo->GetVolumeZSize();
	char tmpVal8S;
	unsigned char tmpVal8U;
	short int tmpVal16S;
	unsigned short tmpVal16U;
	int tmpVal32S;
	unsigned int tmpVal32U;

	WriteLog("FlipDataX()...");
	//wxProgressDialog progDlg(wxT("Reorienting Volume (X)"), wxT(" "), zSize, NULL, wxPD_APP_MODAL | wxPD_SMOOTH | wxPD_ELAPSED_TIME);

	/* loop through each plane (Z) */
	for (z=0; z < zSize; z++) {
		//progDlg.Update(z,msg.Format("Flipping slice %d of %d",z,zSize));
		/* loop through each row */
		for (y=0; y < ySize; y++) {
			/* flip the row in the X direction by loop to width/2, and swapping numbers */
			for (x=0; x < (floor((double)xSize/2.0)); x++) {
				/* row, column, plane */
				switch (imgInfo->GetPixelType()) {
					case IMG_INT8:
						tmpVal8S = imgDataINT8->Get(y,x,z,0,0);
						imgDataINT8->Set(y,x,z,0,0, imgDataINT8->Get(y,xSize - 1 - x,z,0,0));
						imgDataINT8->Set(y,xSize - 1 - x,z,0,0, tmpVal8S);
						break;
					case IMG_UINT8:
						tmpVal8U = imgDataUINT8->Get(y,x,z,0,0);
						imgDataUINT8->Set(y,x,z,0,0, imgDataUINT8->Get(y,xSize - 1 - x,z,0,0));
						imgDataUINT8->Set(y,xSize - 1 - x,z,0,0, tmpVal8U);
						break;
					case IMG_INT16:
						tmpVal16S = imgDataINT16->Get(y,x,z,0,0);
						imgDataINT16->Set(y,x,z,0,0, imgDataINT16->Get(y,xSize - 1 - x,z,0,0));
						imgDataINT16->Set(y,xSize - 1 - x,z,0,0, tmpVal16S);
						break;
					case IMG_UINT16:
						tmpVal16U = imgDataUINT16->Get(y,x,z,0,0);
						imgDataUINT16->Set(y,x,z,0,0, imgDataUINT16->Get(y,xSize - 1 - x,z,0,0));
						imgDataUINT16->Set(y,xSize - 1 - x,z,0,0, tmpVal16U);
						break;
					case IMG_INT32:
						tmpVal32S = imgDataINT32->Get(y,x,z,0,0);
						imgDataINT32->Set(y,x,z,0,0, imgDataINT32->Get(y,xSize - 1 - x,z,0,0));
						imgDataINT32->Set(y,xSize - 1 - x,z,0,0, tmpVal32S);
						break;
					case IMG_UINT32:
						tmpVal32U = imgDataUINT32->Get(y,x,z,0,0);
						imgDataUINT32->Set(y,x,z,0,0, imgDataUINT32->Get(y,xSize - 1 - x,z,0,0));
						imgDataUINT32->Set(y,xSize - 1 - x,z,0,0, tmpVal32U);
						break;
				}
			}
		}
	}
}


// --------------------------------------------------------
// ---------- FlipDataY -----------------------------------
// --------------------------------------------------------
void ImageData::FlipDataY()
{
	int x, y, z;
	int xSize = imgInfo->GetVolumeXSize();
	int ySize = imgInfo->GetVolumeYSize();
	int zSize = imgInfo->GetVolumeZSize();
	int pixType = imgInfo->GetPixelType();
	INT_8 tmpVal8S;
	UINT_8 tmpVal8U;
	INT_16 tmpVal16S;
	UINT_16 tmpVal16U;
	INT_32 tmpVal32S;
	UINT_32 tmpVal32U;

	WriteLog("FlipDataY()...");
	wxProgressDialog progDlg(wxT("Reorienting Volume (Y)"), wxT(" "), zSize, NULL, wxPD_APP_MODAL | wxPD_SMOOTH | wxPD_ELAPSED_TIME);

	int i = 0;
	/* loop through each plane (Z) */
	for (z=0; z < zSize; z++) {
		progDlg.Update(z,msg.Format("Flipping slice (Y direction) %d of %d",z,zSize));
		WriteLog(msg.Format("Flipping slice (Y direction) %d of %d",z,zSize));
		/* loop through each row */
		for (x=0; x < xSize; x++) {
			/* flip the row in the X direction by loop to width/2, and swapping numbers */
			for (y=0; y < (int)(floor((double)ySize/2.0)); y++) {
				/* row, column, plane */
				switch (pixType) {
					case IMG_INT8:
						tmpVal8S = imgDataINT8->Get(y,x,z,0,0);
						imgDataINT8->Set(y,x,z,0,0, imgDataINT8->Get(ySize - 1 - y,x,z,0,0));
						imgDataINT8->Set(ySize - 1 - y,x,z,0,0, tmpVal8S);
						break;
					case IMG_UINT8:
						tmpVal8U = imgDataUINT8->Get(y,x,z,0,0);
						imgDataUINT8->Set(y,x,z,0,0, imgDataUINT8->Get(ySize - 1 - y,x,z,0,0));
						imgDataUINT8->Set(ySize - 1 - y,x,z,0,0, tmpVal8U);
						break;
					case IMG_INT16:
						tmpVal16S = imgDataINT16->Get(y,x,z,0,0);
						imgDataINT16->Set(y,x,z,0,0, imgDataINT16->Get(ySize - 1 - y,x,z,0,0));
						imgDataINT16->Set(ySize - 1 - y,x,z,0,0, tmpVal16S);
						break;
					case IMG_UINT16:
						tmpVal16U = imgDataUINT16->Get(y,x,z,0,0);
						imgDataUINT16->Set(y,x,z,0,0, imgDataUINT16->Get(ySize - 1 - y,x,z,0,0));
						imgDataUINT16->Set(ySize - 1 - y,x,z,0,0, tmpVal16U);
						break;
					case IMG_INT32:
						tmpVal32S = imgDataINT32->Get(y,x,z,0,0);
						imgDataINT32->Set(y,x,z,0,0, imgDataINT32->Get(ySize - 1 - y,x,z,0,0));
						imgDataINT32->Set(ySize - 1 - y,x,z,0,0, tmpVal32S);
						break;
					case IMG_UINT32:
						tmpVal32U = imgDataUINT32->Get(y,x,z,0,0);
						imgDataUINT32->Set(y,x,z,0,0, imgDataUINT32->Get(ySize - 1 - y,x,z,0,0));
						imgDataUINT32->Set(ySize - 1 - y,x,z,0,0, tmpVal32U);
						break;
				}
				i++;
			}
		}
	}
}


// --------------------------------------------------------
// ---------- FlipDataZ -----------------------------------
// --------------------------------------------------------
void ImageData::FlipDataZ()
{
	int x, y, z;
	int xSize = imgInfo->GetVolumeXSize();
	int ySize = imgInfo->GetVolumeYSize();
	int zSize = imgInfo->GetVolumeZSize();
	char tmpVal8S;
	unsigned char tmpVal8U;
	short int tmpVal16S;
	unsigned short tmpVal16U;
	int tmpVal32S;
	unsigned int tmpVal32U;

	WriteLog("FlipDataZ()...");
	//wxProgressDialog progDlg(wxT("Reorienting Volume (Z)"), wxT(" "), zSize, NULL, wxPD_APP_MODAL | wxPD_SMOOTH | wxPD_ELAPSED_TIME);

	/* loop through each plane (Z) */
	for (z=0; z < (floor((double)zSize/2.0)); z++) {
		//progDlg.Update(z,msg.Format("Flipping slice (Z direction) %d of %d",z,zSize));
		/* loop through each row */
		for (x=0; x < xSize; x++) {
			/* flip the row in the X direction by loop to width/2, and swapping numbers */
			for (y=0; y < ySize; y++) {
				/* row, column, plane */
				switch (imgInfo->GetPixelType()) {
					case IMG_INT8:
						tmpVal8S = imgDataINT8->Get(y,x,z,0,0);
						imgDataINT8->Set(y,x,z,0,0, imgDataINT8->Get(y,x,zSize - 1 - z,0,0));
						imgDataINT8->Set(y,x,zSize - 1 - z,0,0, tmpVal8S);
						break;
					case IMG_UINT8:
						tmpVal8U = imgDataUINT8->Get(y,x,z,0,0);
						imgDataUINT8->Set(y,x,z,0,0, imgDataUINT8->Get(y,x,zSize - 1 - z,0,0));
						imgDataUINT8->Set(y,x,zSize - 1 - z,0,0, tmpVal8U);
						break;
					case IMG_INT16:
						tmpVal16S = imgDataINT16->Get(y,x,z,0,0);
						imgDataINT16->Set(y,x,z,0,0, imgDataINT16->Get(y,x,zSize - 1 - z,0,0));
						imgDataINT16->Set(y,x,zSize - 1 - z,0,0, tmpVal16S);
						break;
					case IMG_UINT16:
						tmpVal16U = imgDataUINT16->Get(y,x,z,0,0);
						imgDataUINT16->Set(y,x,z,0,0, imgDataUINT16->Get(y,x,zSize - 1 - z,0,0));
						imgDataUINT16->Set(y,x,zSize - 1 - z,0,0, tmpVal16U);
						break;
					case IMG_INT32:
						tmpVal32S = imgDataINT32->Get(y,x,z,0,0);
						imgDataINT32->Set(y,x,z,0,0, imgDataINT32->Get(y,x,zSize - 1 - z,0,0));
						imgDataINT32->Set(y,x,zSize - 1 - z,0,0, tmpVal32S);
						break;
					case IMG_UINT32:
						tmpVal32U = imgDataUINT32->Get(y,x,z,0,0);
						imgDataUINT32->Set(y,x,z,0,0, imgDataUINT32->Get(y,x,zSize - 1 - z,0,0));
						imgDataUINT32->Set(y,x,zSize - 1 - z,0,0, tmpVal32U);
						break;
				}
			}
		}
	}
}
