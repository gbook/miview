/* ----------------------------------------------------------------------------
	ImageDataReader.cpp


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
#include "wx/dir.h"
#include "gdcmAttribute.h"
#include "gdcmFile.h"
#include "gdcmImage.h"
#include "gdcmImageReader.h"
#include "gdcmByteValue.h"
#include "wx/File.h"
#include "wx/progdlg.h"
#include "wx/filename.h"
#include "wx/image.h"
#include <iostream>
#include "math.h"
#include "stdio.h"
#include "ImageDataReader.h"
#include "fslio/fslio.h"


/* --------------------------------------------------------------- */
/* -------------------- ImageDataReader --------------------------------- */
/// Constructor
/* --------------------------------------------------------------- */
ImageDataReader::ImageDataReader(ImageData *imgdata, ImageInfo *imageInfo, DicomInfo *dicomInfo, NiftiInfo *niftiInfo)
{
	imgData = imgdata;
	
	imgInfo = imageInfo;
	dcmInfo = dicomInfo;
	nftInfo = niftiInfo;

	errorMsg = "";
	errorOccured = false;
}

/* --------------------------------------------------------------- */
/* -------------------- ~ImageDataReader -------------------------------- */
/// Destructor
/* --------------------------------------------------------------- */
ImageDataReader::~ImageDataReader(void)
{
}


/* --------------------------------------------------------------- */
/* -------------------- SetFileParameters ------------------------ */
/* --------------------------------------------------------------- */
void ImageDataReader::SetFileParameters(int ftype, wxArrayString fnames, int lType, bool IsMosaic, int mosaicXSize, int mosaicYSize, int mosaicNumSlices)
{
	filetype = ftype;
	filenames = fnames;
	loadType = lType;
	isMosaic = IsMosaic;
	mosaicX = mosaicXSize;
	mosaicY = mosaicYSize;
	mosaicZ = mosaicNumSlices;

	/* the file loading parameters have been changed... so the data contained should be considered invalid until it is actually loaded */
	imgData->dataIsValid = false;
}



/* --------------------------------------------------------------- */
/* -------------------- Load ------------------------------------- */
/* --------------------------------------------------------------- */
int ImageDataReader::Load()
{

	int ret = 1;
	wxArrayString newFilenames;


	/* based on filetype, call the appropriate loading function/object */
	switch (filetype) {
		case FILETYPE_DICOM:
			WriteLog("DICOM");
			/* load the file info, also checks if the file is valid */
			if (!GetFileInfo(filenames[0])) { SetError("Failed to load DICOM file info"); ret = 0; }
			//imgData->PrintFileInfo();
			if ((loadType == OPEN_ALL) || (loadType == OPEN_SELECTED) || (loadType == OPEN_SINGLE)) {

				if (loadType == OPEN_SINGLE)
					imgData->imgInfo->fileList.Add(filenames[0]);
				else
					/* when loading multiple files, get the sorted file list first... */
					imgData->imgInfo->fileList = GetOrderedDicomFileList(filenames, loadType);

				/* check if anything was returned (error occured, or user cancelled, etc) */
				if (imgData->imgInfo->fileList.Count() < 1) {
					SetError("Failed to get a sorted DICOM file list");
					ret = 0;
				}
				/* ... then load the file list */
				if (!LoadDicomFilelist(imgData->imgInfo->fileList)) {
					SetError("Failed to load DICOM file list");
					ret = 0;
				}
			}
			break;
		case FILETYPE_ANALYZE:
			WriteLog("3DANALYZE");
			//if (loadType == OPEN_SINGLE) {
			//	/* use same function as loading multiples, but specify the file before loading */
			//	imgData->imgInfo->fileList = GetOrderedNiftiFileList(filenames, loadType);
			//	if (imgData->imgInfo->fileList.Count() < 1) { SetError("Failed to load single Analyze/Nifti file"); ret = 0; }
			//}
			if ((loadType == OPEN_ALL) || (loadType == OPEN_SELECTED) || (loadType == OPEN_SINGLE)) {
				imgData->imgInfo->fileList = GetOrderedNiftiFileList(filenames, loadType);
				if (imgData->imgInfo->fileList.Count() < 1) {
					SetError("Failed to get sorted Analyze/Nifti file list");
					ret = 0;
				}
			}

			/* ... then load the file list */
			if (!LoadNiftiFilelist()) {
				SetError("Failed to load Analyze/Nifti file list");
				ret = 0;
			}
			break;
		case FILETYPE_RASTER:
			WriteLog("RASTER");
			/* just load a single file */
			if (loadType == OPEN_SINGLE) {
				imgData->imgInfo->fileList = filenames; /* use same function as loading multiples, but specify the file before loading */
				if (!LoadImageFilelist()) {
					SetError("Failed to load single raster image");
					ret = 0;
				}
			}
			else {
				/* when loading multiple files, get the file list first... */
				if (!GetOrderedImageFileList(filenames[0], "", true)) {
					SetError("Failed to get sorted raster image file list");
					ret = 0;
				}
				/* ... then load the file list */
				if (!LoadImageFilelist()) {
					SetError("Failed to load raster image file list");
					ret = 0;
				}
			}
			SetRasterInfoParameters();
			break;
		case FILETYPE_UNKNOWN:
			WriteLog("UNKNOWN");
			SetError("File type is not recognized");
			break;
		default:
			WriteLog("default");
			break;
	}

	if (ret)
		imgData->dataIsValid = true;
	else
		imgData->dataIsValid = false;

	return ret;
}


/* -------------------- GetOrderedDicomFileList ------------------ */
/// Gets correctly ordered Dicom file list.
/** This function retrieves an ordered dicom file list. First alphabetizing
 * then sorting by slice number.
 */
/* --------------------------------------------------------------- */
wxArrayString ImageDataReader::GetOrderedDicomFileList(wxArrayString files, int loadType)
{
	int slice;
	int i;
	int j,tmp;
	int numFiles;
	wxArrayString tmpFileList, finalFileList;
	wxArrayInt sliceNums;
	wxString dirpath; //, prefix;
	wxString dirname;
	wxString filename;
	wxString filepath;
	wxString tmpStr;
	wxFileName firstfile;
	gdcm::ImageReader reader;
	gdcm::DataSet dataSet;
	gdcm::DataElement dataElement;
	wxDir directory;
	int imageNumber;

	i = 0;
	slice = 0;

	/* get the information about the first file */
	firstfile.Assign(files[0],wxPATH_UNIX);
	dirname = firstfile.GetPath();
	filename = firstfile.GetFullName();
	WriteLog(msg.Format("Directory: %s\nFilename: %s",dirname.c_str(), filename.c_str()));
	imgData->imgInfo->SetDirPath(dirname);

	if (loadType == OPEN_ALL) {
		wxString filePattern;
		filePattern = "*.dcm";

		/* read list of files from directory and make an array */
		numFiles = (int)directory.GetAllFiles(dirname,&tmpFileList,filePattern,wxDIR_DEFAULT);

		WriteLog(msg.Format("Found %d files", tmpFileList.Count()));
	}
	else {
		numFiles = (int)files.Count();
		tmpFileList = files;
	}

	/* number of dicom's to read is known, so setup the progress dialog */
	wxProgressDialog progDlg(wxT("Loading Dicom Volume"), wxT(" "), numFiles, NULL, wxPD_APP_MODAL | wxPD_SMOOTH | wxPD_ELAPSED_TIME | wxPD_CAN_ABORT);

	tmpFileList.Sort();

	gdcm::SmartPointer<gdcm::Image> imagePtr;
	gdcm::SmartPointer<gdcm::File> filePtr;

	/* read the files, get the slice numbers */
	for (i=0;i<numFiles;i++) {
		if (!progDlg.Update(i,msg.Format("Reading slice %d of %d",i,numFiles))) {
			SetError("User cancelled load operation");
			tmpFileList.Clear();
			return tmpFileList;
		}
		filepath = tmpFileList[i];

		/* open it, get the image number */
		gdcm::ImageReader r;
		r.SetFileName(filepath.c_str());

		if (!r.Read()) {
			SetError(msg.Format("Dicomfile '%s' not readable - ImageDataReader->LoadDicomFilelist()",filepath.c_str()));
			return 0;
		}
		imagePtr = const_cast<gdcm::Image*>(&r.GetImage()); // very ugly
		filePtr = const_cast<gdcm::File*>(&r.GetFile()); // very ugly, but it works :)

		dataSet = filePtr->GetDataSet();
		XSizeOrig = imagePtr->GetDimension(0);
		YSizeOrig = imagePtr->GetDimension(1);
		imgData->imgInfo->SetImageZSize(imagePtr->GetDimension(2));

		gdcm::Attribute<0x0020,0x0013> at;
		if( dataSet.FindDataElement( at.GetTag() )) {
			const gdcm::DataElement &de = dataSet.GetDataElement( at.GetTag() );
			at.SetFromDataElement( de );
			imageNumber = at.GetValue();
		}
		sliceNums.Add(imageNumber);

		//WriteLog(msg.Format("Processing Slice: %d\n",imgnum));
		//XSizeOrig = dcfile->GetXSize();
		//YSizeOrig = dcfile->GetYSize();
		if (i > 0) {
			if ((imgData->imgInfo->GetImageXSize() != imagePtr->GetDimension(0)) || (imgData->imgInfo->GetImageYSize() != imagePtr->GetDimension(1)) )
				SetError("Consecutive image sizes do not match.");
		}
	}
	
	/* sort the final list based on the slice numbers */
	for (i=numFiles-1;i>=0;i--) {
		for (j=1;j<=i;j++) {
			if (sliceNums[j-1] > sliceNums[j]) {
				tmp = sliceNums[j-1];
				tmpStr = tmpFileList[j-1];

				sliceNums[j-1] = sliceNums[j];
				tmpFileList[j-1] = tmpFileList[j];

				sliceNums[j] = tmp;
				tmpFileList[j] = tmpStr;
			}
		}
	}

	WriteLog(msg.Format("readdir() found a total of %d files", numFiles));

	if (numFiles > imgData->imgInfo->GetImageZSize())
		imgData->imgInfo->SetVolumeZSize(numFiles);

	if (imgData->imgInfo->GetVolumeZSize() > 1)
		imgData->imgInfo->SetIsMultiSlice(true);

	if (isMosaic) {
		imgData->imgInfo->SetImageXSize(mosaicX);
		imgData->imgInfo->SetImageYSize(mosaicY);
		imgData->imgInfo->SetImageZSize(mosaicZ);
		imgData->imgInfo->SetImageTSize(1);
		imgData->imgInfo->SetVolumeXSize(mosaicX);
		imgData->imgInfo->SetVolumeYSize(mosaicY);
		imgData->imgInfo->SetVolumeZSize(mosaicZ);
		imgData->imgInfo->SetVolumeTSize(numFiles);
	}

	return tmpFileList;
}


/* -------------------- LoadDicomFileList ------------------ */
/// Loads a Dicom file list into the image data buffer.
/** This function loads all dicom files from the filelist into the imageData buffer.
 */
int ImageDataReader::LoadDicomFilelist(wxArrayString files)
{
	bool sizeFound = false;
	int i;
	int imageDataSize;
	int volumeDataSize = 0;
	int VolXSize, VolYSize, VolZSize, VolTSize;
	int ImgXSize, ImgYSize, ImgZSize, ImgTSize;
	unsigned char *tmpImageData;
	double percent;
	double maxValue, minValue;
	double maxValueR, minValueR;
	double maxValueG, minValueG;
	double maxValueB, minValueB;
	wxString filename;
	wxString filepath;
	gdcm::ImageReader reader;

	maxValue = minValue = 0.0;
	maxValueR = minValueR = 0.0;
	maxValueG = minValueG = 0.0;
	maxValueB = minValueB = 0.0;

	/* set the filelist in dicomInfo */
	imgData->imgInfo->SetFileList(imgData->imgInfo->fileList);
	WriteLog("fileList set in ImageDataReader->LoadDicomFilelist()");

	/* setup the progress dialog */
	wxProgressDialog progDlg2(wxT("Loading Dicom Volume"), wxT(" "), (int)imgData->imgInfo->fileList.Count(), NULL, wxPD_APP_MODAL | wxPD_SMOOTH | wxPD_ELAPSED_TIME | wxPD_CAN_ABORT);

	if (isMosaic) {
		VolXSize = imgData->imgInfo->GetVolumeXSize();
		VolYSize = imgData->imgInfo->GetVolumeYSize();
		VolZSize = imgData->imgInfo->GetVolumeZSize();
		VolTSize = imgData->imgInfo->GetVolumeTSize();
		ImgXSize = imgData->imgInfo->GetImageXSize();
		ImgYSize = imgData->imgInfo->GetImageYSize();
		ImgZSize = imgData->imgInfo->GetImageZSize();
		ImgTSize = 1;
		imgInfo->SetImageType(IMG_3D);
		imgInfo->SetVolumeType(VOL_4D);
	}
	else {
		VolXSize = imgData->imgInfo->GetVolumeXSize();
		VolYSize = imgData->imgInfo->GetVolumeYSize();
		VolZSize = imgData->imgInfo->GetVolumeZSize();
		VolTSize = 1;
		ImgXSize = imgData->imgInfo->GetImageXSize();
		ImgYSize = imgData->imgInfo->GetImageYSize();
		ImgZSize = imgData->imgInfo->GetImageZSize();
		ImgTSize = 1;
		if (ImgZSize > 1)
			imgInfo->SetImageType(IMG_3D);
		else
			imgInfo->SetImageType(IMG_2D);
		imgInfo->SetVolumeType(VOL_3D);
	}

	sizeFound = true;
	/* allocate space for the images based on "numImageXXXs" */
	if ((imgData->imgInfo->GetSamplesPerPixel() == 1) && (imgData->imgInfo->GetIsMonochrome())) {
		imgData->imgInfo->SetIsRGB(false);
		switch (imgData->imgInfo->GetPixelType()) {
		case IMG_UINT8:
			imgData->imgDataUINT8 = new DataBuffer<UINT_8>(VolYSize, VolXSize, VolZSize, VolTSize,1);
			volumeDataSize = imgData->imgDataUINT8->NumBytes();
			break;
		case IMG_INT8:
			imgData->imgDataINT8 = new DataBuffer<INT_8>(VolYSize, VolXSize, VolZSize, VolTSize,1);
			volumeDataSize = imgData->imgDataINT8->NumBytes();
			break;
		case IMG_UINT16:
			imgData->imgDataUINT16 = new DataBuffer<UINT_16>(VolYSize, VolXSize, VolZSize, VolTSize,1);
			volumeDataSize = imgData->imgDataUINT16->NumBytes();
			break;
		case IMG_INT16:
			imgData->imgDataINT16 = new DataBuffer<INT_16>(VolYSize, VolXSize, VolZSize, VolTSize,1);
			volumeDataSize = imgData->imgDataINT16->NumBytes();
			break;
		case IMG_UINT32:
			imgData->imgDataUINT32 = new DataBuffer<UINT_32>(VolYSize, VolXSize, VolZSize, VolTSize,1);
			volumeDataSize = imgData->imgDataUINT32->NumBytes();
			break;
		case IMG_INT32:
			imgData->imgDataINT32 = new DataBuffer<INT_32>(VolYSize, VolXSize, VolZSize, VolTSize,1);
			volumeDataSize = imgData->imgDataINT32->NumBytes();
			break;
		case IMG_FLOAT32:
		case IMG_FLOAT64:
			imgData->imgDataFLOAT64 = new DataBuffer<FLOAT_64>(VolYSize, VolXSize, VolZSize, VolTSize,1);
			volumeDataSize = imgData->imgDataFLOAT64->NumBytes();
			break;
		case IMG_FLOAT128:
			SetError("Sorry, not processing floating point DICOM datatypes yet!");
			sizeFound = false;
			break;
		default:
			SetError("Unknown Pixel type!");
			sizeFound = false;
		}
	}
	if (sizeFound)
		WriteLog(msg.Format("Buffer size %d bytes",volumeDataSize));

	/* check if it's RGB data */
	if ((imgData->imgInfo->GetSamplesPerPixel() == 3) || (!imgData->imgInfo->GetIsMonochrome())) {
		imgData->imgInfo->SetIsRGB(true);
		imgData->imgDataRGB24 = new DataBuffer<UINT_8>(VolYSize, VolXSize, VolZSize, 1,3);
		volumeDataSize = imgData->imgDataRGB24->NumBytes();
		sizeFound = true;
	}

	/* the pixel type wasn't valid, so return a 0 */
	if (!sizeFound) {
		SetError("Pixel type not found.");

		return 0;
	}

	WriteLog(msg.Format("Allocated array of size (%dx%dx%dx%d) = %d bytes",VolXSize, VolYSize, VolZSize, VolTSize, volumeDataSize));
//	imgData->dataHasBeenLoaded = true; /* a data buffer has been created by this point */

	gdcm::SmartPointer<gdcm::Image> imgPtr;

	int imgIndex = -1;
	/* loop through all files in fileList and load the data */
	for (i=0;i<imgData->imgInfo->fileList.Count();i++) {
		/* update the progress dialog */
		if (!progDlg2.Update(i,msg.Format("Loading file %d of %d",i,imgData->imgInfo->fileList.Count()))) {
			SetError("User cancelled load operation");
			return 0;
		}

		imgIndex++;

		/* open the dicom file */
		filepath = imgData->imgInfo->fileList[i];

		gdcm::ImageReader r;
		r.SetFileName(filepath.c_str());
		if (!r.Read()) {
			SetError(msg.Format("Dicomfile '%s' not readable - ImageDataReader->LoadDicomFilelist()",filepath.c_str()));
			return 0;
		}
		imgPtr = const_cast<gdcm::Image*>(&r.GetImage()); // very ugly

		/* PCs use little-endian format, check to see what the swap code says... */
		bool swap = false;
		if (imgData->dcmInfo->GetSwapCode() == 4321)
			swap = true;

		/* get the dicom data (in a char array) */
		imageDataSize = imgPtr->GetBufferLength();
		tmpImageData = new unsigned char[imageDataSize];
		imgPtr->GetBuffer((char*)tmpImageData);

		percent = ((double)i+1.0)/(double)imgData->imgInfo->fileList.Count();
		WriteLog(msg.Format("Loading %s [%f%%]",filename.c_str(), percent*100));
		//WriteLog(msg.Format("Pixel Size is %d bytes\n",imgData->PixelByteSize));

		/* ----- read the data from the file-----  */
		if ((imgData->imgInfo->GetSamplesPerPixel() == 1) && (imgData->imgInfo->GetIsMonochrome())) {
			switch (imgData->imgInfo->GetPixelType()) {
			case IMG_UINT8: LoadUINT8ToVolumeBuffer(tmpImageData, imageDataSize, imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap); break;
			case IMG_INT8: LoadINT8ToVolumeBuffer(tmpImageData, imageDataSize, imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap); break;
			case IMG_UINT16: LoadUINT16ToVolumeBuffer(tmpImageData, imageDataSize, imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap); break;
			case IMG_INT16: LoadINT16ToVolumeBuffer(tmpImageData, imageDataSize, imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap); break;
			case IMG_UINT32: LoadUINT32ToVolumeBuffer(tmpImageData, imageDataSize, imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap); break;
			case IMG_INT32: LoadINT32ToVolumeBuffer(tmpImageData, imageDataSize, imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap); break;
			case IMG_FLOAT32:
			case IMG_FLOAT64:
			case IMG_FLOAT128:
				WriteLog("Sorry, not processing this datatype yet!");
				sizeFound = false;
				break;
			default:
				WriteLog("Unknown Pixel type!");
				sizeFound = false;
			}
		}
		/* if it's RGB data */
		if ((imgData->imgInfo->GetSamplesPerPixel() == 3) || (!imgData->imgInfo->GetIsMonochrome())) {
			LoadRGB24ToVolumeBuffer(tmpImageData, imageDataSize, imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap);
		}

		WriteLog(msg.Format("done.."));
	}

	/* set the max and min pixel values */
	imgData->imgInfo->minPixelValues.SetNormalized(false);
	imgData->imgInfo->maxPixelValues.SetNormalized(false);

	WriteLog(msg.Format("Max Pixel Values: (%.2f, %.2f, %.2f) or %.2f",imgData->imgInfo->maxPixelValues.Red(), imgData->imgInfo->maxPixelValues.Green(), imgData->imgInfo->maxPixelValues.Blue(), imgData->imgInfo->maxPixelValues.Gray()));
	WriteLog(msg.Format("Min Pixel Values: (%.2f, %.2f, %.2f) or %.2f",imgData->imgInfo->minPixelValues.Red(), imgData->imgInfo->minPixelValues.Green(), imgData->imgInfo->minPixelValues.Blue(), imgData->imgInfo->minPixelValues.Gray()));

	WriteLog(msg.Format("End of LoadDicomFilelist() imgData->imgInfo->fileList.Count(): %d", imgData->imgInfo->fileList.Count()));

	return 1;
}


// --------------------------------------------------------
// ---------- SetError ------------------------------------
// --------------------------------------------------------
void ImageDataReader::SetError(wxString errmsg)
{
	errorOccured = true;
	errorMsg = errorMsg + errmsg + "\n";
	WriteLog("ERROR: " + errmsg + "");
}


// --------------------------------------------------------
// ---------- GetFileInfo ---------------------------------
// --------------------------------------------------------
int ImageDataReader::GetFileInfo(wxString filename)
{
	wxArrayInt offsets, lengths;
	wxFile dicomfile;
	wxFileName fname(filename);
	wxString str;
	
	gdcm::ImageReader reader;
	gdcm::PixelFormat pixelformat;
	gdcm::PhotometricInterpretation photo;
	gdcm::SwapCode swapcode;
	gdcm::DataSet dataSet;
	gdcm::DataElement dataElement;

	wxDateTime theDateTime;

	/* check for a .dcm extension */
	if (filename.Right(4) != ".dcm") {
		SetError("File should really have a .dcm extension. I'll let it slide though...");
		/* maybe its a directory being passed in... */
		if (fname.IsDir()) {
			/* get potential files from this directory */
			wxString filePattern;
			filePattern = "*.dcm";
			wxDir thedir(fname.GetFullPath());
			wxArrayString tmpFileList;

			/* read list of files from directory and make an array */
			int numFiles = (int)thedir.GetAllFiles(fname.GetFullPath(),&tmpFileList,filePattern,wxDIR_DEFAULT);

			WriteLog(msg.Format("Found %d files", tmpFileList.Count()));
			filename = tmpFileList[0];
		}
	}

	/* check if the file exists */
	if (!wxFile::Exists(filename.c_str())) {
		SetError("Selected DICOM file does not exist");
		return 0;
	}

	/* see if the file will open */
	reader.SetFileName(filename.c_str());
	if (!reader.Read()) {
		SetError("DICOM file is not readable");
		return 0;
	}
	const gdcm::File &file = reader.GetFile();
	const gdcm::Image &image = reader.GetImage();
	dataSet = file.GetDataSet();

	/* set the sizes of the file */
	imgData->imgInfo->SetImageXSize(image.GetDimension(0));
	imgData->imgInfo->SetImageYSize(image.GetDimension(1));
	imgData->imgInfo->SetImageZSize(image.GetDimension(2));
	imgData->imgInfo->SetImageTSize(1); /* dicom files generally are not 4D */
	imgData->imgInfo->SetVolumeXSize(image.GetDimension(0));
	imgData->imgInfo->SetVolumeYSize(image.GetDimension(1));
	imgData->imgInfo->SetVolumeZSize(image.GetDimension(2));
	imgData->imgInfo->SetVolumeTSize(1); /* need to fix this if its a DICOM mosaic image */

	/* get the origin */
	imgData->imgInfo->SetOrigin(XYZTriplet(image.GetOrigin(0), image.GetOrigin(1), image.GetOrigin(2)));

	/* get the pixel spacing */
	imgData->imgInfo->SetSpacing(XYZTriplet(image.GetSpacing(0), image.GetSpacing(1), image.GetSpacing(2)));

	/* get some image information */
	pixelformat = image.GetPixelFormat();
	photo = image.GetPhotometricInterpretation();
	swapcode = image.GetSwapCode();

	imgData->imgInfo->SetPixelByteSize(pixelformat.GetPixelSize());
	imgData->imgInfo->SetHighBitPosition(pixelformat.GetHighBit());
	imgData->imgInfo->SetBitsStored(pixelformat.GetBitsStored());
	imgData->imgInfo->SetBitsAllocated(pixelformat.GetBitsAllocated());
	imgData->imgInfo->SetPlanarConfiguration(image.GetPlanarConfiguration());
	imgData->imgInfo->SetSamplesPerPixel(pixelformat.GetSamplesPerPixel());

	/* get the photometric information */
	if ((photo == gdcm::PhotometricInterpretation::MONOCHROME1) || (photo == gdcm::PhotometricInterpretation::MONOCHROME2))
		imgData->imgInfo->SetIsMonochrome(true);
	else
		imgData->imgInfo->SetIsMonochrome(false);

	/* set the same information in the dicomInfo object */
	imgData->dcmInfo->SetSwapCode(swapcode.operator gdcm::SwapCode::SwapCodeType());

	/* get the pixel type */
	switch (pixelformat.GetScalarType()) {
		case gdcm::PixelFormat::UINT8: imgData->imgInfo->SetPixelType(IMG_UINT8); imgData->imgInfo->SetPixelTypeStr("8U"); imgData->imgInfo->SetIsSignedPixelData(false); break;
		case gdcm::PixelFormat::INT8: imgData->imgInfo->SetPixelType(IMG_INT8); imgData->imgInfo->SetPixelTypeStr("8S"); imgData->imgInfo->SetIsSignedPixelData(true); break;
		case gdcm::PixelFormat::UINT12: imgData->imgInfo->SetPixelType(IMG_UINT12); imgData->imgInfo->SetPixelTypeStr("12U"); imgData->imgInfo->SetIsSignedPixelData(false); break;
		case gdcm::PixelFormat::INT12: imgData->imgInfo->SetPixelType(IMG_INT12); imgData->imgInfo->SetPixelTypeStr("12S"); imgData->imgInfo->SetIsSignedPixelData(true); break;
		case gdcm::PixelFormat::UINT16: imgData->imgInfo->SetPixelType(IMG_UINT16); imgData->imgInfo->SetPixelTypeStr("16U"); imgData->imgInfo->SetIsSignedPixelData(false); break;
		case gdcm::PixelFormat::INT16: imgData->imgInfo->SetPixelType(IMG_INT16); imgData->imgInfo->SetPixelTypeStr("16S"); imgData->imgInfo->SetIsSignedPixelData(true); break;
		case gdcm::PixelFormat::UINT32: imgData->imgInfo->SetPixelType(IMG_UINT32); imgData->imgInfo->SetPixelTypeStr("32U"); imgData->imgInfo->SetIsSignedPixelData(false); break;
		case gdcm::PixelFormat::INT32: imgData->imgInfo->SetPixelType(IMG_INT32); imgData->imgInfo->SetPixelTypeStr("32S"); imgData->imgInfo->SetIsSignedPixelData(true); break;
		case gdcm::PixelFormat::FLOAT16: imgData->imgInfo->SetPixelType(IMG_FLOAT32); imgData->imgInfo->SetPixelTypeStr("FLOAT16"); imgData->imgInfo->SetIsSignedPixelData(true); break;
		case gdcm::PixelFormat::FLOAT32: imgData->imgInfo->SetPixelType(IMG_FLOAT64); imgData->imgInfo->SetPixelTypeStr("FLOAT32"); imgData->imgInfo->SetIsSignedPixelData(true); break;
		case gdcm::PixelFormat::FLOAT64: imgData->imgInfo->SetPixelType(IMG_FLOAT128); imgData->imgInfo->SetPixelTypeStr("FLOAT64"); imgData->imgInfo->SetIsSignedPixelData(true); break;
		case gdcm::PixelFormat::UNKNOWN: imgData->imgInfo->SetPixelType(IMG_UNKNOWN); imgData->imgInfo->SetPixelTypeStr("Unknown"); imgData->imgInfo->SetIsSignedPixelData(false); break;
	}

	/* is it RGB or not */
	if ((imgData->imgInfo->GetSamplesPerPixel() == 3) || (!imgData->imgInfo->GetIsMonochrome())) {
		imgData->imgInfo->SetPixelType(IMG_RGB24);
		imgData->imgInfo->SetPixelTypeStr("RGB");
		imgData->imgInfo->SetIsRGB(true);
	}

	/* get the image orientation patient direction cosines */
	double x1,y1,z1, x2,y2,z2, x3,y3,z3;
	x1 = image.GetDirectionCosines(0);
	y1 = image.GetDirectionCosines(1);
	z1 = image.GetDirectionCosines(2);
	x2 = image.GetDirectionCosines(3);
	y2 = image.GetDirectionCosines(4);
	z2 = image.GetDirectionCosines(5);
	imgData->dcmInfo->SetRowCosines(XYZTriplet(x1,y1,z1));
	imgData->dcmInfo->SetColCosines(XYZTriplet(x2,y2,z2));

	/* get the normal vector for the Z direction cosines */
	x3 = y1*z2 - z1*y2;
	y3 = x1*z2 - z1*x2;
	z3 = x1*y2 - y1*x2;
	imgData->dcmInfo->SetNormCosines(XYZTriplet(x3,y3,z3));
	

	/* get the patient information, etc */
	//	patientName;         /* (0010,0010) */
	//	patientID;           /* (0010,0020) */
	//	patientBirthdate;    /* (0010,0030) */
	//	institutionName;     /* (0008,0080) */
	//	studyDate;           /* (0008,0020) */
	//	seriesDate;          /* (0008,0021) */
	//	studyTime;           /* (0008,0030) */
	//	seriesTime;          /* (0008,0031) */
	//	studyDescription;    /* (0008,1030) */
	//	seriesDescription;   /* (0008,103E) */
	//	performingPhysician; /* (0008,1050) */
	//	protocolName;        /* (0018,0030) */
	//  modality;            /* (0008,0060) */

	/* doesn't seem to be any easy way to do this because
	   gdcm::Attribute uses templates to access the info */

	gdcm::Attribute<0x0010,0x0010> at; /* patientName */
	if (dataSet.FindDataElement( at.GetTag() )) {
		const gdcm::DataElement &de = dataSet.GetDataElement( at.GetTag() );
		at.SetFromDataElement( de );
		str = at.GetValue().c_str();
		str.Replace("^",",");
	}
	else { str = "";}
	imgData->dcmInfo->SetPatientName(str);

	gdcm::Attribute<0x0010,0x0020> at2; /* patientID */
	if (dataSet.FindDataElement( at2.GetTag() )) {
		const gdcm::DataElement &de2 = dataSet.GetDataElement( at2.GetTag() );
		at2.SetFromDataElement( de2 );
		str = at2.GetValue().c_str();
	}
	else { str = "";}
	imgData->dcmInfo->SetPatientID(wxString(str));

	gdcm::Attribute<0x0010,0x0030> at3; /* patientBirthdate */
	if (dataSet.FindDataElement( at3.GetTag() )) {
		const gdcm::DataElement &de3 = dataSet.GetDataElement( at3.GetTag() );
		at3.SetFromDataElement( de3 );
		str = at3.GetValue().c_str();
	}
	else { str = "";}
	imgData->dcmInfo->SetPatientBirthdate(wxString(str));

	gdcm::Attribute<0x0008,0x0080> at4; /* institutionName */
	if (dataSet.FindDataElement( at4.GetTag() )) {
		const gdcm::DataElement &de4 = dataSet.GetDataElement( at4.GetTag() );
		at4.SetFromDataElement( de4 );
		str = at4.GetValue().c_str();
		str.Replace("^"," ");
	}
	else { str = "";}
	imgData->dcmInfo->SetInstitutionName(str);

	gdcm::Attribute<0x0008,0x0020> at5; /* studyDate */
	if (dataSet.FindDataElement( at5.GetTag() )) {
		const gdcm::DataElement &de5 = dataSet.GetDataElement( at5.GetTag() );
		at5.SetFromDataElement( de5 );
		str = at5.GetValue().c_str();
	}
	else { str = "";}
	imgData->dcmInfo->SetStudyDate(wxString(str));

	gdcm::Attribute<0x0008,0x0021> at6; /* seriesDate */
	if (dataSet.FindDataElement( at6.GetTag() )) {
		const gdcm::DataElement &de6 = dataSet.GetDataElement( at6.GetTag() );
		at6.SetFromDataElement( de6 );
		str = at6.GetValue().c_str();
	}
	else { str = "";}
	imgData->dcmInfo->SetSeriesDate(wxString(str));

	gdcm::Attribute<0x0008,0x0030> at7; /* studyTime */
	if (dataSet.FindDataElement( at7.GetTag() )) {
		const gdcm::DataElement &de7 = dataSet.GetDataElement( at7.GetTag() );
		at7.SetFromDataElement( de7 );
		str = at7.GetValue().c_str();
	}
	else { str = "";}
	imgData->dcmInfo->SetStudyTime(wxString(str));

	gdcm::Attribute<0x0008,0x0031> at8; /* seriesTime */
	if (dataSet.FindDataElement( at8.GetTag() )) {
		const gdcm::DataElement &de8 = dataSet.GetDataElement( at8.GetTag() );
		at8.SetFromDataElement( de8 );
		str = at8.GetValue().c_str();
	}
	else { str = "";}
	imgData->dcmInfo->SetSeriesTime(wxString(str));

	gdcm::Attribute<0x0008,0x1030> at9; /* studyDescription */
	if (dataSet.FindDataElement( at9.GetTag() )) {
		const gdcm::DataElement &de9 = dataSet.GetDataElement( at9.GetTag() );
		at9.SetFromDataElement( de9 );
		str = at9.GetValue().c_str();
		str.Replace("^",",");
	}
	else { str = "";}
	imgData->dcmInfo->SetStudyDescription(str);

	gdcm::Attribute<0x0008,0x103E> at10; /* seriesDescription */
	if (dataSet.FindDataElement( at10.GetTag() )) {
		const gdcm::DataElement &de10 = dataSet.GetDataElement( at10.GetTag() );
		at10.SetFromDataElement( de10 );
		str = at10.GetValue().c_str();
		str.Replace("^",",");
	}
	else { str = "";}
	imgData->dcmInfo->SetSeriesDescription(str);

	gdcm::Attribute<0x0008,0x1050> at11; /* performingPhysician */
	if (dataSet.FindDataElement( at11.GetTag() )) {
		const gdcm::DataElement &de11 = dataSet.GetDataElement( at11.GetTag() );
		at11.SetFromDataElement( de11 );
		str = at11.GetValue().c_str();
		str.Replace("^",",");
	}
	else { str = "";}
	imgData->dcmInfo->SetPerformingPhysician(str);

	gdcm::Attribute<0x0018,0x1030> at12; /* protocolName */
	if (dataSet.FindDataElement( at12.GetTag() )) {
		const gdcm::DataElement &de12 = dataSet.GetDataElement( at12.GetTag() );
		at12.SetFromDataElement( de12 );
		str = at12.GetValue().c_str();
		str.Replace("^",",");
	}
	else { str = "";}
	imgData->dcmInfo->SetProtocolName(str);

	gdcm::Attribute<0x0018,0x5100> at13; /* patientPosition */
	if (dataSet.FindDataElement( at13.GetTag() )) {
		const gdcm::DataElement &de13 = dataSet.GetDataElement( at13.GetTag() );
		at13.SetFromDataElement( de13 );
		str = at13.GetValue().c_str();
	}
	else { str = "";}
	imgData->dcmInfo->SetPatientPositionStr(str);

	gdcm::Attribute<0x0008,0x0060> at14; /* modality */
	if (dataSet.FindDataElement( at14.GetTag() )) {
		const gdcm::DataElement &de14 = dataSet.GetDataElement( at14.GetTag() );
		at14.SetFromDataElement( de14 );
		str = at14.GetValue().c_str();
	}
	else { str = "";}
	imgData->dcmInfo->SetModality(wxString(str));

	WriteLog(msg.Format("seriesDate: '%s'...", imgData->dcmInfo->GetSeriesDate().c_str()));
	WriteLog(msg.Format("seriesTime: '%s'...", imgData->dcmInfo->GetSeriesTime().c_str()));
	WriteLog(msg.Format("studyDate: '%s'...", imgData->dcmInfo->GetStudyDate().c_str()));
	WriteLog(msg.Format("studyTime: '%s'...", imgData->dcmInfo->GetStudyTime().c_str()));

	wxString theTime = imgData->dcmInfo->GetStudyTime().c_str();
	wxString theDate = imgData->dcmInfo->GetStudyDate().c_str();

	/* study */
	if (imgData->dcmInfo->GetStudyTime().Trim() == "")
		theTime = "000000";
	if (imgData->dcmInfo->GetStudyDate().Trim() == "")
		theDate = "100000";
	theTime.Replace(":","");
	theTime.Replace(".","");
	str = theDate + theTime.Mid(0,6);
	theDateTime.ParseFormat(str, "%Y%m%d%H%M%S");
	if (theDateTime.IsValid()) WriteLog("Study: " + theDateTime.FormatISODate());
	else theDateTime.ParseDateTime("19000101120000");
	imgData->dcmInfo->SetStudyDateTime(theDateTime);


	/* series */
	theTime = imgData->dcmInfo->GetSeriesTime().c_str();
	theDate = imgData->dcmInfo->GetSeriesDate().c_str();

	if (imgData->dcmInfo->GetSeriesTime().Trim() == "")
		theTime = "000000";
	if (imgData->dcmInfo->GetSeriesDate().Trim() == "")
		theDate = "100000";
	theTime.Replace(":","");
	theTime.Replace(".","");
	str = theDate + theTime.Mid(0,6);
	theDateTime.ParseFormat(str, "%Y%m%d%H%M%S");
	if (theDateTime.IsValid()) WriteLog("Series: " + theDateTime.FormatISODate());
	else theDateTime.ParseDateTime("19000101120000");
	imgData->dcmInfo->SetSeriesDateTime(theDateTime);

	imgData->dcmInfo->SetModalityDescription(GetModalityDescription(imgData->dcmInfo->GetModality()));

	/* setup the patient position */
	if (imgData->dcmInfo->GetPatientPositionStr() == "HFS") imgData->dcmInfo->SetPatientPosition(PPOS_HFS);
	else if (imgData->dcmInfo->GetPatientPositionStr() == "FFS") imgData->dcmInfo->SetPatientPosition(PPOS_FFS);
	else if (imgData->dcmInfo->GetPatientPositionStr() == "HFP") imgData->dcmInfo->SetPatientPosition(PPOS_HFP);
	else if (imgData->dcmInfo->GetPatientPositionStr() == "FFP") imgData->dcmInfo->SetPatientPosition(PPOS_FFP);
	else if (imgData->dcmInfo->GetPatientPositionStr() == "HFDR") imgData->dcmInfo->SetPatientPosition(PPOS_HFDR);
	else if (imgData->dcmInfo->GetPatientPositionStr() == "HFDL") imgData->dcmInfo->SetPatientPosition(PPOS_HFDL);
	else if (imgData->dcmInfo->GetPatientPositionStr() == "FFDR") imgData->dcmInfo->SetPatientPosition(PPOS_FFDR);
	else if (imgData->dcmInfo->GetPatientPositionStr() == "FFDL") imgData->dcmInfo->SetPatientPosition(PPOS_FFDL);
	else imgData->dcmInfo->SetPatientPosition(PPOS_UNKNOWN);

	return 1;
}


// --------------------------------------------------------
// ---------- GetModalityDescription ----------------------
// --------------------------------------------------------
wxString ImageDataReader::GetModalityDescription(wxString mode)
{
	wxString ret;

	if (mode == "Unknow") ret = "Unknown";
	if (mode == "AU") ret = "Voice Audio";
	if (mode == "AS") ret = "Angioscopy";
	if (mode == "BI") ret = "Biomagnetic Imaging";
	if (mode == "CF") ret = "Cinefluorography";
	if (mode == "CP") ret = "Culposcopy";
	if (mode == "CR") ret = "Computed Radiography";
	if (mode == "CS") ret = "Cystoscopy";
	if (mode == "CT") ret = "Computed Tomography";
	if (mode == "DD") ret = "Duplex Dopler";
	if (mode == "DF") ret = "Digital Fluoroscopy";
	if (mode == "DG") ret = "Diaphanography";
	if (mode == "DM") ret = "Digital Microscopy";
	if (mode == "DS") ret = "Digital Substraction Angiography";
	if (mode == "DX") ret = "Digital Radiography";
	if (mode == "ECG") ret = "Echocardiography";
	if (mode == "EPS") ret = "Basic Cardiac EP";
	if (mode == "ES") ret = "Endoscopy";
	if (mode == "FA") ret = "Fluorescein Angiography";
	if (mode == "FS") ret = "Fundoscopy";
	if (mode == "HC") ret = "Hard Copy";
	if (mode == "HD") ret = "Hemodynamic";
	if (mode == "LP") ret = "Laparoscopy";
	if (mode == "LS") ret = "Laser Surface Scan";
	if (mode == "MA") ret = "Magnetic Resonance Angiography";
	if (mode == "MR") ret = "Magnetic Resonance";
	if (mode == "NM") ret = "Nuclear Medicine";
	if (mode == "OT") ret = "Other";
	if (mode == "PT") ret = "Positron Emission Tomography";
	if (mode == "RF") ret = "Radio Fluoroscopy";
	if (mode == "RG") ret = "Radiographic Imaging";
	if (mode == "RTDOSE") ret = "Radiotherapy Dose";
	if (mode == "RTIMAGE") ret = "Radiotherapy Image";
	if (mode == "RTPLAN") ret = "Radiotherapy Plan";
	if (mode == "RTSTRUCT") ret = "Radiotherapy Structure Set";
	if (mode == "SM") ret = "Microscopic Imaging";
	if (mode == "ST") ret = "Single-photon Emission Computed Tomography";
	if (mode == "TG") ret = "Thermography";
	if (mode == "US") ret = "Ultrasound";
	if (mode == "VF") ret = "Videofluorography";
	if (mode == "XA") ret = "X-Ray Angiography";
	if (mode == "XC") ret = "Photographic Imaging";

	return ret;
}


// --------------------------------------------------------
// ---------- GetOrderedImageFileList ---------------------
// --------------------------------------------------------
int ImageDataReader::GetOrderedImageFileList(wxString firstfile, wxString fileprefix, bool loadAll)
{
	bool error = false;
	int numFiles;
	wxArrayString tmpFileList;
	wxFileName *file;
	wxString dirpath, prefix, dirname;
	wxString extension, filename, filepath;
	wxDir directory;

	/* get the file name, extension and directory */
	file = new wxFileName(firstfile);
	dirname = file->GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
	extension = file->GetExt();
	filename = file->GetName();

	WriteLog(msg.Format("Directory: %s\nFilename: %s\nPrefix: %s",dirname.c_str(), filename.c_str(), prefix.c_str()));

	/* set image information */
	imgData->imgInfo->SetDirPath(dirname);
	imgData->imgInfo->SetFilePrefix(prefix);

	/* open the directory */
	if (!directory.Open(dirname)) {
		SetError("Couldn't find the directory! thats odd...\n");
		error = true;
	}

	wxString filePattern;
	filePattern = prefix + "*." + extension;

	/* read list of files from directory and make an array */
	numFiles = (int)directory.GetAllFiles(dirname,&tmpFileList,filePattern,wxDIR_DEFAULT);

	WriteLog(msg.Format("Found %d files", numFiles));

	/* sort the list and set up other image information */
	tmpFileList.Sort();
	imgInfo->fileList = tmpFileList;
	imgData->imgInfo->SetVolumeZSize(numFiles);

	if (numFiles > 1)
		imgData->imgInfo->SetIsMultiSlice(true);

	if ((numFiles == 0) || error) return 0; /* no files found, or directory not found */
	else return 1;
}


// --------------------------------------------------------
// ---------- SetRasterInfoParameters ---------------------
// --------------------------------------------------------
void ImageDataReader::SetRasterInfoParameters()
{
	imgData->imgInfo->SetPixelByteSize(1);
	imgData->imgInfo->SetIsSignedPixelData(false);
	imgData->imgInfo->SetIsMonochrome(false);
	imgData->imgInfo->SetHighBitPosition(8);
	imgData->imgInfo->SetBitsStored(8);
	imgData->imgInfo->SetBitsAllocated(8);
	imgData->imgInfo->SetPlanarConfiguration(3);
	imgData->imgInfo->SetSamplesPerPixel(3);
	imgData->imgInfo->SetPixelType(IMG_RGB24);
	imgData->imgInfo->SetPixelTypeStr("RGB");
	imgData->imgInfo->SetIsRGB(true);
	imgData->imgInfo->SetImageType(IMG_2D);
	imgData->imgInfo->SetVolumeType(VOL_3D);
}


// --------------------------------------------------------
// ---------- LoadImageFilelist ---------------------------
// --------------------------------------------------------
int ImageDataReader::LoadImageFilelist()
{
	bool error = false;
	int x, y, z;
	int i;
	unsigned char *tmpImageData;
	int imageDataSize;
	int volumeDataSize;
	wxString firstfile;
	wxImage *img;

	/* setup the progress dialog */
	wxProgressDialog progDlg2(wxT("Loading Images"), wxT(" "), (int)imgData->imgInfo->fileList.Count(), NULL, wxPD_APP_MODAL | wxPD_SMOOTH | wxPD_ELAPSED_TIME | wxPD_CAN_ABORT);

	/* setup image handlers */
	::wxInitAllImageHandlers();

	/* read first image, determine size */
	firstfile = imgInfo->fileList[0];
	img = new wxImage;
	img->LoadFile(firstfile, wxBITMAP_TYPE_ANY);
	x = img->GetWidth();
	y = img->GetHeight();
	z = (int)imgInfo->fileList.Count();
	delete img;

	/* set the image and volume sizes */
	imgInfo->SetImageXSize(x);
	imgInfo->SetImageYSize(y);
	imgInfo->SetImageZSize(1);
	imgInfo->SetImageTSize(1);
	imgInfo->SetVolumeXSize(x);
	imgInfo->SetVolumeYSize(y);
	imgInfo->SetVolumeZSize(z);
	imgInfo->SetVolumeTSize(1);

	WriteLog(msg.Format("RASTER IMAGE! %dx%dx%d", x, y, z));

	imgData->imgDataRGB24 = new DataBuffer<UINT_8>(y, x, z, 1,3);
	volumeDataSize = imgData->imgDataRGB24->NumBytes();
//	volumeDataSize = AllocateRGB24Buffer(x,y,z,1);

	/* setup the matrix for image */
	imgData->imgInfo->SetIsRGB(true);

	/* set image/volume types */
	imgData->imgInfo->SetImageType(IMG_2D);
	if (z>1)
		imgData->imgInfo->SetVolumeType(VOL_3D);
	else
		imgData->imgInfo->SetVolumeType(VOL_2D);

	int imgIndex = -1;
	/* now load all the files */
	for (i=0; i<z; i++) {
		img = new wxImage;
		img->LoadFile(imgInfo->fileList[i], wxBITMAP_TYPE_ANY);
		x = img->GetWidth();
		y = img->GetHeight();
		if ((x != imgInfo->GetImageXSize()) || (y != imgInfo->GetImageYSize())) {
			error = true;
			SetError("Consecutive raster image sizes do not match");
			return 0;
		}
		else {
			/* update the progress dialog */
			if (!progDlg2.Update(i,msg.Format("Loading slice %d of %d",i,imgData->imgInfo->GetVolumeZSize()))) {
				SetError("User cancelled load operation");
				return 0;
			}

			imgIndex++;
			imageDataSize = x * y * 3;

			/* load the data from the image */
			tmpImageData = img->GetData();
			/* load the image into the volume */
			LoadRGB24ToVolumeBuffer(tmpImageData, imageDataSize, imgIndex, x, y, 1, 1, false);
		}
		delete img;
	}

	if (error) return 0;
	else return 1;
}


// --------------------------------------------------------
// ---------- GetOrderedNiftiFileList ---------------------
// --------------------------------------------------------
wxArrayString ImageDataReader::GetOrderedNiftiFileList(wxArrayString files, int loadType)
{
//	bool error = false;
	int numFiles;
	wxArrayString tmpFileList;
//	wxFileName *file;
	wxFileName firstfile;
	wxString dirpath, prefix, dirname;
	wxString extension, filename, filepath;
	wxString filePattern;
	wxDir directory;
	nifti_image *img;

	/* get the information about the first file */
	firstfile.Assign(files[0],wxPATH_UNIX);

	/* read in the Nifti/Analyze image */
	img = nifti_image_read(firstfile.GetFullPath().c_str(),1);
	if (!nifti_image_load(img)) { /* nifti_image_load returns 0 on success... -1 on failure */
		WriteLog(msg.Format("ANALYZE IMAGE! %dx%dx%d", img->dim[1], img->dim[2], img->dim[3]));
		nftInfo->SetNiftiHeader(img);

		/* setup the image information */
		imgInfo->SetImageXSize(nftInfo->Get_nx());
		imgInfo->SetImageYSize(nftInfo->Get_ny());
		imgInfo->SetImageZSize(nftInfo->Get_nz());
		imgInfo->SetImageTSize(nftInfo->Get_nt());
		imgInfo->SetSpacing(XYZTriplet(nftInfo->Get_dx(), nftInfo->Get_dy(), nftInfo->Get_dz()));
		//imgInfo->SetOrigin(XYZTriplet(nftInfo->
		imgInfo->SetPixelByteSize(nftInfo->Get_nbyper());

		imgInfo->SetPixelType(nftInfo->GetMIViewDatatype());
		imgInfo->SetBitsAllocated(nftInfo->GetBitsAllocated());
		imgInfo->SetBitsStored(nftInfo->GetBitsStored());
		imgInfo->SetHighBitPosition(nftInfo->GetHighBitPosition());
		imgInfo->SetIsMonochrome(nftInfo->GetIsMonochrome());
		imgInfo->SetIsRGB(nftInfo->GetIsRGB());
		imgInfo->SetIsSignedPixelData(nftInfo->GetIsSigned());
		imgInfo->SetPixelTypeStr(nftInfo->GetPixelTypeStr());
		imgInfo->SetSamplesPerPixel(1); /* nifti doesn't read RGB, so samples per pixel should always be 1 */

		/* determine if each imagefile is 1D, 2D, 3D or 4D */
		if ((nftInfo->Get_ny() == 1) && (nftInfo->Get_nz() == 1) && (nftInfo->Get_nt() == 1))
			imgInfo->SetImageType(IMG_1D);
		if ((nftInfo->Get_nz() == 1) && (nftInfo->Get_nt() == 1))
			imgInfo->SetImageType(IMG_2D);
		if ((nftInfo->Get_nz() > 1) && (nftInfo->Get_nt() == 1))
			imgInfo->SetImageType(IMG_3D);
		if ((nftInfo->Get_nz() > 1) && (nftInfo->Get_nt() > 1))
			imgInfo->SetImageType(IMG_4D);

		/* get the information about the first file */
		//firstfile.Assign(files[0],wxPATH_UNIX);

		dirname = firstfile.GetPath();
		filename = firstfile.GetFullName();

		WriteLog(msg.Format("Directory: %s\nFilename: %s",dirname.c_str(), filename.c_str()));

		imgData->imgInfo->SetDirPath(dirname);
	//	imgData->imgInfo->SetFilePrefix(prefix);

		if (loadType == OPEN_ALL) {
			filePattern = "*." + firstfile.GetExt();

			/* read list of files from directory and make an array */
			numFiles = (int)directory.GetAllFiles(dirname,&tmpFileList,filePattern,wxDIR_DEFAULT);

			WriteLog(msg.Format("Found %d files", tmpFileList.Count()));
		}
		else {
			numFiles = (int)files.Count();
			tmpFileList = files;
		}

		WriteLog(msg.Format("Found %d files", numFiles));

		/* sort the list and set up other image information */
		tmpFileList.Sort();

		/* determine the volume type... either 1D, 2D, 3D, or 4D (no 5D yet) */
		switch (imgInfo->GetImageType()) {
			case IMG_1D:
				if (numFiles > 1) imgInfo->SetVolumeType(VOL_2D);
				else imgInfo->SetVolumeType(VOL_1D);
				break;
			case IMG_2D:
				if (numFiles > 1) imgInfo->SetVolumeType(VOL_3D);
				else imgInfo->SetVolumeType(VOL_2D);
				break;
			case IMG_3D:
				if (numFiles > 1) imgInfo->SetVolumeType(VOL_4D);
				else imgInfo->SetVolumeType(VOL_3D);
				break;
			case IMG_4D:
				if (numFiles > 1) SetError("Cannot load multiple 4D files yet...");
				else imgInfo->SetVolumeType(VOL_4D);
				break;
		}

		/* determine volume sizes, based on image and volume types */
		if ((imgInfo->GetImageType() == IMG_2D) && (imgInfo->GetVolumeType() == VOL_2D)) {
			imgInfo->SetVolumeXSize(imgInfo->GetImageXSize());
			imgInfo->SetVolumeYSize(imgInfo->GetImageYSize());
			imgInfo->SetVolumeZSize(1);
			imgInfo->SetVolumeTSize(1);
		}
		if ((imgInfo->GetImageType() == IMG_2D) && (imgInfo->GetVolumeType() == VOL_3D)) {
			imgInfo->SetVolumeXSize(imgInfo->GetImageXSize());
			imgInfo->SetVolumeYSize(imgInfo->GetImageYSize());
			imgInfo->SetVolumeZSize(numFiles);
			imgInfo->SetVolumeTSize(1);
		}
		if ((imgInfo->GetImageType() == IMG_3D) && (imgInfo->GetVolumeType() == VOL_3D)) {
			imgInfo->SetVolumeXSize(imgInfo->GetImageXSize());
			imgInfo->SetVolumeYSize(imgInfo->GetImageYSize());
			imgInfo->SetVolumeZSize(imgInfo->GetImageZSize());
			imgInfo->SetVolumeTSize(1);
		}
		if ((imgInfo->GetImageType() == IMG_3D) && (imgInfo->GetVolumeType() == VOL_4D)) {
			imgInfo->SetVolumeXSize(imgInfo->GetImageXSize());
			imgInfo->SetVolumeYSize(imgInfo->GetImageYSize());
			imgInfo->SetVolumeZSize(imgInfo->GetImageZSize());
			imgInfo->SetVolumeTSize(numFiles);
		}
		if ((imgInfo->GetImageType() == IMG_4D) && (imgInfo->GetVolumeType() == VOL_4D)) {
			imgInfo->SetVolumeXSize(imgInfo->GetImageXSize());
			imgInfo->SetVolumeYSize(imgInfo->GetImageYSize());
			imgInfo->SetVolumeZSize(imgInfo->GetImageZSize());
			imgInfo->SetVolumeTSize(imgInfo->GetImageTSize());
		}

		if (numFiles > 1)
			imgData->imgInfo->SetIsMultiSlice(true);
	}
	else {
		SetError("nifti_image_load() failed. Image file is probably missing its .hdr file");
	}

	return tmpFileList;
}


// --------------------------------------------------------
// ---------- LoadNiftiFilelist ---------------------------
// --------------------------------------------------------
int ImageDataReader::LoadNiftiFilelist()
{
	bool sizeFound = true;
	bool swap = false;
	unsigned char *tmpImageData;
	int i;
	int VolXSize, VolYSize, VolZSize, VolTSize;
	int ImgXSize, ImgYSize, ImgZSize, ImgTSize;
	int imageDataSize;
	int volumeDataSize;
	double percent;
	nifti_image *img;
	wxString filename;
	wxString filepath;

	/* setup max/min values */
	imgData->imgInfo->maxPixelValues.SetGray(0.0);
	imgData->imgInfo->minPixelValues.SetGray(0.0);

	imageDataSize = 0;
	VolXSize = imgData->imgInfo->GetVolumeXSize();
	VolYSize = imgData->imgInfo->GetVolumeYSize();
	VolZSize = imgData->imgInfo->GetVolumeZSize();
	VolTSize = imgData->imgInfo->GetVolumeTSize();
	ImgXSize = imgData->imgInfo->GetImageXSize();
	ImgYSize = imgData->imgInfo->GetImageYSize();
	ImgZSize = imgData->imgInfo->GetImageZSize();
	ImgTSize = imgData->imgInfo->GetImageTSize();

	volumeDataSize = VolXSize*VolYSize*VolZSize*VolTSize;

	/* setup the progress dialog */
	wxProgressDialog progDlg2(wxT("Loading Nifti Volume"), wxT(" "), (int)imgData->imgInfo->fileList.Count(), NULL, wxPD_APP_MODAL | wxPD_SMOOTH | wxPD_ELAPSED_TIME | wxPD_CAN_ABORT);

	/* setup the buffer for the image data */
	switch (imgInfo->GetPixelType()) {
		case IMG_BINARY:
			imgData->imgDataBINARY = new DataBuffer<UINT_1>(VolYSize, VolXSize, VolZSize, VolTSize,1);
			volumeDataSize = imgData->imgDataBINARY->NumBytes();
			break;
		case IMG_UINT8:
			imgData->imgDataUINT8 = new DataBuffer<UINT_8>(VolYSize, VolXSize, VolZSize, VolTSize,1);
			volumeDataSize = imgData->imgDataUINT8->NumBytes();
			break;
		case IMG_INT8:
			imgData->imgDataINT8 = new DataBuffer<INT_8>(VolYSize, VolXSize, VolZSize, VolTSize,1);
			volumeDataSize = imgData->imgDataINT8->NumBytes();
			break;
		case IMG_UINT16:
			imgData->imgDataUINT16 = new DataBuffer<UINT_16>(VolYSize, VolXSize, VolZSize, VolTSize,1);
			volumeDataSize = imgData->imgDataUINT16->NumBytes();
			break;
		case IMG_INT16:
			imgData->imgDataINT16 = new DataBuffer<INT_16>(VolYSize, VolXSize, VolZSize, VolTSize,1);
			volumeDataSize = imgData->imgDataINT16->NumBytes();
			break;
		case IMG_UINT32:
			imgData->imgDataUINT32 = new DataBuffer<UINT_32>(VolYSize, VolXSize, VolZSize, VolTSize,1);
			volumeDataSize = imgData->imgDataUINT32->NumBytes();
			break;
		case IMG_INT32:
			imgData->imgDataINT32 = new DataBuffer<INT_32>(VolYSize, VolXSize, VolZSize, VolTSize,1);
			volumeDataSize = imgData->imgDataINT32->NumBytes();
			break;
		case IMG_UINT64:
			imgData->imgDataUINT64 = new DataBuffer<UINT_64>(VolYSize, VolXSize, VolZSize, VolTSize,1);
			volumeDataSize = imgData->imgDataUINT64->NumBytes();
			break;
		case IMG_INT64:
			imgData->imgDataINT64 = new DataBuffer<INT_64>(VolYSize, VolXSize, VolZSize, VolTSize,1);
			volumeDataSize = imgData->imgDataINT64->NumBytes();
			break;
		case IMG_FLOAT32:
			imgData->imgDataFLOAT32 = new DataBuffer<FLOAT_32>(VolYSize, VolXSize, VolZSize, VolTSize,1);
			volumeDataSize = imgData->imgDataFLOAT32->NumBytes();
			break;
		case IMG_FLOAT64:
			imgData->imgDataFLOAT64 = new DataBuffer<FLOAT_64>(VolYSize, VolXSize, VolZSize, VolTSize,1);
			volumeDataSize = imgData->imgDataFLOAT64->NumBytes();
			break;
		case IMG_FLOAT128:
			imgData->imgDataFLOAT128 = new DataBuffer<FLOAT_128>(VolYSize, VolXSize, VolZSize, VolTSize,1);
			volumeDataSize = imgData->imgDataFLOAT128->NumBytes();
			break;
		case IMG_COMPLEX64:
			imgData->imgDataCOMPLEX64 = new DataBuffer<FLOAT_32>(VolYSize, VolXSize, VolZSize, VolTSize,2);
			volumeDataSize = imgData->imgDataCOMPLEX64->NumBytes();
			break;
		case IMG_COMPLEX128:
			imgData->imgDataCOMPLEX128 = new DataBuffer<FLOAT_64>(VolYSize, VolXSize, VolZSize, VolTSize,2);
			volumeDataSize = imgData->imgDataCOMPLEX128->NumBytes();
			break;
		case IMG_COMPLEX256:
			imgData->imgDataCOMPLEX256 = new DataBuffer<FLOAT_128>(VolYSize, VolXSize, VolZSize, VolTSize,2);
			volumeDataSize = imgData->imgDataCOMPLEX256->NumBytes();
			break;
		case IMG_RGB24:
			imgData->imgDataRGB24 = new DataBuffer<UINT_8>(VolYSize, VolXSize, VolZSize, VolTSize,3);
			volumeDataSize = imgData->imgDataRGB24->NumBytes();
			break;
		default:
			SetError("Analyze/Nifti file contains an unknown Pixel type!");
			sizeFound = false;
			break;
	}

	WriteLog(msg.Format("Allocated array of size (%dx%dx%dx%d) = %d bytes",VolXSize,VolYSize,VolZSize, VolTSize, volumeDataSize));
//	imgData->dataHasBeenLoaded = true; /* a data buffer has been created by this point */

	int imgIndex = -1;
	/* loop through all files in fileList and load the data */
	for (i=0;i<imgData->imgInfo->fileList.Count();i++) {

		imgIndex++;
		imageDataSize = ImgXSize * ImgYSize * ImgZSize * ImgTSize;

		/* update the progress dialog */
		if (!progDlg2.Update(i,msg.Format("Loading slice %d of %d - %d bytes",i,imgData->imgInfo->fileList.Count(), imageDataSize))) {
			SetError("User cancelled load operation");
			return 0;
		}

		/* now load the images */
		img = nifti_image_read(imgData->imgInfo->fileList[i].c_str(),1);
		if (!nifti_image_load(img)) { /* nifti_image_load returns 0 on success... -1 on failure */
			tmpImageData = (unsigned char*)img->data;

			percent = ((double)i+1.0)/(double)imgData->imgInfo->fileList.Count();
			WriteLog(msg.Format("Loading %s [%f%%]",filename.c_str(), percent*100));

			/* ----- read the data from the file-----  */
			switch (imgData->imgInfo->GetPixelType()) {
			case IMG_UINT8: LoadUINT8ToVolumeBuffer(tmpImageData, imageDataSize , imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap); break;
			case IMG_INT8: LoadINT8ToVolumeBuffer(tmpImageData, imageDataSize, imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap); break;
			case IMG_UINT16: LoadUINT16ToVolumeBuffer(tmpImageData, imageDataSize*2, imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap); break;
			case IMG_INT16: LoadINT16ToVolumeBuffer(tmpImageData, imageDataSize*2, imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap); break;
			case IMG_UINT32: LoadUINT32ToVolumeBuffer(tmpImageData, imageDataSize*4, imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap); break;
			case IMG_INT32: LoadINT32ToVolumeBuffer(tmpImageData, imageDataSize*4, imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap); break;
			case IMG_UINT64: LoadUINT64ToVolumeBuffer(tmpImageData, imageDataSize*8, imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap); break;
			case IMG_INT64: LoadINT64ToVolumeBuffer(tmpImageData, imageDataSize*8, imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap); break;
			case IMG_FLOAT32: LoadFLOAT32ToVolumeBuffer(tmpImageData, imageDataSize*4, imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap); break;
			case IMG_FLOAT64: LoadFLOAT64ToVolumeBuffer(tmpImageData, imageDataSize*8, imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap); break;
			case IMG_FLOAT128: LoadFLOAT128ToVolumeBuffer(tmpImageData, imageDataSize*16, imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap); break;
			case IMG_COMPLEX64: LoadCOMPLEX64ToVolumeBuffer(tmpImageData, imageDataSize*8, imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap); break;
			case IMG_COMPLEX128: LoadCOMPLEX128ToVolumeBuffer(tmpImageData, imageDataSize*16, imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap); break;
			case IMG_COMPLEX256: LoadCOMPLEX256ToVolumeBuffer(tmpImageData, imageDataSize*32, imgIndex, ImgXSize, ImgYSize, ImgZSize, ImgTSize, swap); break;
			}
		}
	}
	WriteLog(msg.Format("Min/Max %f/%f",imgInfo->minPixelValues.Gray() ,imgInfo->maxPixelValues.Gray()));

	if (sizeFound) return 1;
	else return 0;
}


// --------------------------------------------------------
// --------------------------------------------------------
// ------- LOAD Image to Volume Buffer functions ----------
// --------------------------------------------------------
// --------------------------------------------------------
/*  why are there a dozen different loading functions? why
    not templates? because image data from the file is read
	in as single bytes and must transformed into the
	correct multi-byte datatypes the pixels are originally
	stored in */

// --------------------------------------------------------
// ---------- LoadBINARYToVolumeBuffer --------------------
// --------------------------------------------------------
bool ImageDataReader::LoadBINARYToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap)
{
	unsigned char bit;
	int Xm, Ym, Zm, Tm; /* coordinates for the original mosaic image */
	int X, Y, Z, T; /* coordinates for the final 3D/4D image */
	int imageType = imgInfo->GetImageType();
	int volumeType = imgInfo->GetVolumeType();
//	double maxValue = imgData->imgInfo->maxPixelValues.Gray();
//	double minValue = imgData->imgInfo->minPixelValues.Gray();
//	int numCols = XSizeOrig/mosaicX;
	int numRows = YSizeOrig/mosaicY;
	int col, row;

	col = row = 0;
	X = Y = Z = T = 0;
	Xm = Ym = Zm = Tm = 0;
	swap = false;

	/* access data through [Y,X,Z,T] */
	for (int j=0;j<imageDataSize;j++) {
		for (int i=0;i<8;i++) {
			bit = tmpBuffer[j] & (1 << i);
			bit == 1 ? bit = 255 : bit = 0; /* make the byte either 0 or 255 */

			if (isMosaic) {
				imgData->imgDataBINARY->Set(Y,X,Z,index,bit,0); /* mosaic will always load 3D image into 4D volume */

				Xm++; /* do all the nice transformations to get the mosaic coordinates into the final volume coordinates */
				if (Xm >= XSizeOrig) { Xm = 0; Ym++; } /* check if the original X is past the last column */
				if (Xm%mosaicX == 0) col = (Xm/mosaicX); /* check if were at a column boundary; if so, set the column */
				X = Xm%mosaicX; /* get the new volume X position */
				if (Ym >= YSizeOrig) { Ym = 0; } /* check if the original Y is past the last row (shouldn't happen) */
				if (Ym%mosaicY == 0) row = (Ym/mosaicY); /* check if were at a row boundary; if so, set the row */
				Y = Ym%mosaicY; /* get the new volume Y position */
				Z = (row*numRows) + col; /* find the new volume slice */
				if (Z >= mosaicZ) break; /* unlikely, but better have it in case something is wrong from before */
			}
			else {
				if ((imageType == IMG_2D) && (volumeType == VOL_2D)) imgData->imgDataBINARY->Set(Y,X,0,0,bit,0); /* load 2D image into 2D volume (single file) */
				if ((imageType == IMG_2D) && (volumeType == VOL_3D)) imgData->imgDataBINARY->Set(Y,X,index,0,bit,0); /* load 2D image into 3D volume */
				if ((imageType == IMG_3D) && (volumeType == VOL_3D)) imgData->imgDataBINARY->Set(Y,X,Z,0,bit,0); /* load 3D image into 3D volume (single file) */
				if ((imageType == IMG_3D) && (volumeType == VOL_4D)) imgData->imgDataBINARY->Set(Y,X,Z,index,bit,0); /* load 3D image into 4D volume */
				if ((imageType == IMG_4D) && (volumeType == VOL_4D)) imgData->imgDataBINARY->Set(Y,X,Z,T,bit,0); /* load 4D image into 4D volume (single file) */
				
				X++;
				if (X >= XSize) { X = 0; Y++; }
				if (Y >= YSize) { Y = 0; Z++; }
				if (Z >= ZSize) { Z = 0; T++; }
				if (T >= TSize) break;
			}
		}
	}
	imgData->imgInfo->maxPixelValues.SetGray(1.0);
	imgData->imgInfo->minPixelValues.SetGray(0.0);
	return true;
}


// --------------------------------------------------------
// ---------- LoadUINT8ToVolumeBuffer ---------------------
// --------------------------------------------------------
bool ImageDataReader::LoadUINT8ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap)
{
	int Xm, Ym, Zm, Tm; /* coordinates for the original mosaic image */
	int X, Y, Z, T; /* coordinates for the final 3D/4D image */
	int imageType = imgInfo->GetImageType();
	int volumeType = imgInfo->GetVolumeType();
	double maxValue = imgData->imgInfo->maxPixelValues.Gray();
	double minValue = imgData->imgInfo->minPixelValues.Gray();
//	int numCols = XSizeOrig/mosaicX;
	int numRows = YSizeOrig/mosaicY;
	int col, row;

	col = row = 0;
	X = Y = Z = T = 0;
	Xm = Ym = Zm = Tm = 0;
	swap = false;

	/* access data through [Y,X,Z,T] */
	for (int j=0;j<imageDataSize;j++) {
		if (tmpBuffer[j] < 0) tmpBuffer[j] = 0; /* check for negative numbers */

		if (isMosaic) {
			imgData->imgDataUINT8->Set(Y,X,Z,index,0,tmpBuffer[j]); /* mosaic will always load 3D image into 4D volume */

			Xm++; /* do all the nice transformations to get the mosaic coordinates into the final volume coordinates */
			if (Xm >= XSizeOrig) { Xm = 0; Ym++; } /* check if the original X is past the last column */
			if (Xm%mosaicX == 0) col = (Xm/mosaicX); /* check if were at a column boundary; if so, set the column */
			X = Xm%mosaicX; /* get the new volume X position */
			if (Ym >= YSizeOrig) { Ym = 0; } /* check if the original Y is past the last row (shouldn't happen) */
			if (Ym%mosaicY == 0) row = (Ym/mosaicY); /* check if were at a row boundary; if so, set the row */
			Y = Ym%mosaicY; /* get the new volume Y position */
			Z = (row*numRows) + col; /* find the new volume slice */
			if (Z >= mosaicZ) break; /* unlikely, but better have it in case something is wrong from before */
		}
		else {
			if ((imageType == IMG_2D) && (volumeType == VOL_2D)) imgData->imgDataUINT8->Set(Y,X,0,0,0,tmpBuffer[j]); /* load 2D image into 2D volume (single file) */
			if ((imageType == IMG_2D) && (volumeType == VOL_3D)) imgData->imgDataUINT8->Set(Y,X,index,0,0,tmpBuffer[j]); /* load 2D image into 3D volume */
			if ((imageType == IMG_3D) && (volumeType == VOL_3D)) imgData->imgDataUINT8->Set(Y,X,Z,0,0,tmpBuffer[j]); /* load 3D image into 3D volume (single file) */
			if ((imageType == IMG_3D) && (volumeType == VOL_4D)) imgData->imgDataUINT8->Set(Y,X,Z,index,0,tmpBuffer[j]); /* load 3D image into 4D volume */
			if ((imageType == IMG_4D) && (volumeType == VOL_4D)) imgData->imgDataUINT8->Set(Y,X,Z,T,0,tmpBuffer[j]); /* load 4D image into 4D volume (single file) */

			X++;
			if (X >= XSize) { X = 0; Y++; }
			if (Y >= YSize) { Y = 0; Z++; }
			if (Z >= ZSize) { Z = 0; T++; }
			if (T >= TSize) break;
		}
		if ((double)tmpBuffer[j] > maxValue) maxValue = (double)tmpBuffer[j]; /* get the max and min values */
		if ((double)tmpBuffer[j] < minValue) minValue = (double)tmpBuffer[j];
	}
	imgData->imgInfo->maxPixelValues.SetGray(maxValue);
	imgData->imgInfo->minPixelValues.SetGray(minValue);
	return true;
}


// --------------------------------------------------------
// ---------- LoadINT8ToVolumeBuffer ----------------------
// --------------------------------------------------------
bool ImageDataReader::LoadINT8ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap)
{
	int Xm, Ym, Zm, Tm; /* coordinates for the original mosaic image */
	int X, Y, Z, T; /* coordinates for the final 3D/4D image */
	int imageType = imgInfo->GetImageType();
	int volumeType = imgInfo->GetVolumeType();
	double maxValue = imgData->imgInfo->maxPixelValues.Gray();
	double minValue = imgData->imgInfo->minPixelValues.Gray();
//	int numCols = XSizeOrig/mosaicX;
	int numRows = YSizeOrig/mosaicY;
	int col, row;

	col = row = 0;
	X = Y = Z = T = 0;
	Xm = Ym = Zm = Tm = 0;
	swap = false;

	/* access data through [Y,X,Z,T] */
	for (int j=0;j<imageDataSize;j++) {
		//if (tmpBuffer[j] < 0) tmpBuffer[j] = 0; /* check for negative numbers */
		if (isMosaic) {
			imgData->imgDataINT8->Set(Y,X,Z,index,0,tmpBuffer[j]); /* mosaic will always load 3D image into 4D volume */

			Xm++; /* do all the nice transformations to get the mosaic coordinates into the final volume coordinates */
			if (Xm >= XSizeOrig) { Xm = 0; Ym++; } /* check if the original X is past the last column */
			if (Xm%mosaicX == 0) col = (Xm/mosaicX); /* check if were at a column boundary; if so, set the column */
			X = Xm%mosaicX; /* get the new volume X position */
			if (Ym >= YSizeOrig) { Ym = 0; } /* check if the original Y is past the last row (shouldn't happen) */
			if (Ym%mosaicY == 0) row = (Ym/mosaicY); /* check if were at a row boundary; if so, set the row */
			Y = Ym%mosaicY; /* get the new volume Y position */
			Z = (row*numRows) + col; /* find the new volume slice */
			if (Z >= mosaicZ) break; /* unlikely, but better have it in case something is wrong from before */
		}
		else {
			if ((imageType == IMG_2D) && (volumeType == VOL_2D)) imgData->imgDataINT8->Set(Y,X,0,0,0,tmpBuffer[j]); /* load 2D image into 2D volume (single file) */
			if ((imageType == IMG_2D) && (volumeType == VOL_3D)) imgData->imgDataINT8->Set(Y,X,index,0,0,tmpBuffer[j]); /* load 2D image into 3D volume */
			if ((imageType == IMG_3D) && (volumeType == VOL_3D)) imgData->imgDataINT8->Set(Y,X,Z,0,0,tmpBuffer[j]); /* load 3D image into 3D volume (single file) */
			if ((imageType == IMG_3D) && (volumeType == VOL_4D)) imgData->imgDataINT8->Set(Y,X,Z,index,0,tmpBuffer[j]); /* load 3D image into 4D volume */
			if ((imageType == IMG_4D) && (volumeType == VOL_4D)) imgData->imgDataINT8->Set(Y,X,Z,T,0,tmpBuffer[j]); /* load 4D image into 4D volume (single file) */
		
			X++;
			if (X >= XSize) { X = 0; Y++; }
			if (Y >= YSize) { Y = 0; Z++; }
			if (Z >= ZSize) { Z = 0; T++; }
			if (T >= TSize) break;
		}
		if ((double)tmpBuffer[j] > maxValue) maxValue = (double)tmpBuffer[j]; /* get the max and min values */
		if ((double)tmpBuffer[j] < minValue) minValue = (double)tmpBuffer[j];
	}
	imgData->imgInfo->maxPixelValues.SetGray(maxValue);
	imgData->imgInfo->minPixelValues.SetGray(minValue);
	return true;
}


// --------------------------------------------------------
// ---------- LoadUINT16ToVolumeBuffer --------------------
// --------------------------------------------------------
bool ImageDataReader::LoadUINT16ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap)
{
	unsigned short	byte1, byte2, byte3;
	int Xm, Ym, Zm, Tm; /* coordinates for the original mosaic image */
	int X, Y, Z, T; /* coordinates for the final 3D/4D image */
	int imageType = imgInfo->GetImageType();
	int volumeType = imgInfo->GetVolumeType();
	double maxValue = imgData->imgInfo->maxPixelValues.Gray();
	double minValue = imgData->imgInfo->minPixelValues.Gray();
	int numRows = YSizeOrig/mosaicY;
	int col, row;

	col = row = 0;
	X = Y = Z = T = 0;
	Xm = Ym = Zm = Tm = 0;
	swap = false;

	/* access data through [Y,X,Z,T] */
	for (int j=0;j<imageDataSize;j+=2) {
		/* each pixel is described by two bytes, so concatenate them in the correct order */
		byte1 = tmpBuffer[j];
		byte2 = tmpBuffer[j+1];
		if (swap) byte3 = (byte1 << 8) | byte2;
		else byte3 = (byte2 << 8) | byte1;

		if (byte3 < 0) byte3 = 0; /* clamp negative values to zero */

		if (isMosaic) {
			imgData->imgDataUINT16->Set(Y,X,Z,index,0,byte3); /* mosaic will always load 3D image into 4D volume */

			Xm++; /* do all the nice transformations to get the mosaic coordinates into the final volume coordinates */
			if (Xm >= XSizeOrig) { Xm = 0; Ym++; } /* check if the original X is past the last column */
			if (Xm%mosaicX == 0) col = (Xm/mosaicX); /* check if were at a column boundary; if so, set the column */
			X = Xm%mosaicX; /* get the new volume X position */
			if (Ym >= YSizeOrig) { Ym = 0; } /* check if the original Y is past the last row (shouldn't happen) */
			if (Ym%mosaicY == 0) row = (Ym/mosaicY); /* check if were at a row boundary; if so, set the row */
			Y = Ym%mosaicY; /* get the new volume Y position */
			Z = (row*numRows) + col; /* find the new volume slice */
			if (Z >= mosaicZ) break; /* unlikely, but better have it in case something is wrong from before */
		}
		else {
			if ((imageType == IMG_2D) && (volumeType == VOL_2D)) imgData->imgDataUINT16->Set(Y,X,0,0,0,byte3);		/* load 2D image into 2D volume (single file) */
			if ((imageType == IMG_2D) && (volumeType == VOL_3D)) imgData->imgDataUINT16->Set(Y,X,index,0,0,byte3);	/* load 2D image into 3D volume               */
			if ((imageType == IMG_3D) && (volumeType == VOL_3D)) imgData->imgDataUINT16->Set(Y,X,Z,0,0,byte3);		/* load 3D image into 3D volume (single file) */
			if ((imageType == IMG_3D) && (volumeType == VOL_4D)) imgData->imgDataUINT16->Set(Y,X,Z,index,0,byte3);	/* load 3D image into 4D volume               */
			if ((imageType == IMG_4D) && (volumeType == VOL_4D)) imgData->imgDataUINT16->Set(Y,X,Z,T,0,byte3);		/* load 4D image into 4D volume (single file) */

			X++;
			if (X >= XSize) { X = 0; Y++; }
			if (Y >= YSize) { Y = 0; Z++; }
			if (Z >= ZSize) { Z = 0; T++; }
			if (T >= TSize) break;
		}
		/* get the max and min values */
		if ((double)byte3 > maxValue) maxValue = (double)byte3;
		if ((double)byte3 < minValue) minValue = (double)byte3;
	}
	imgData->imgInfo->maxPixelValues.SetGray(maxValue);
	imgData->imgInfo->minPixelValues.SetGray(minValue);
	return true;
}


// --------------------------------------------------------
// ---------- LoadINT16ToVolumeBuffer ---------------------
// --------------------------------------------------------
bool ImageDataReader::LoadINT16ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap)
{
	short byte1, byte2, byte3;
	int Xm, Ym, Zm, Tm; /* coordinates for the original mosaic image */
	int X, Y, Z, T; /* coordinates for the final 3D/4D image */
	int imageType = imgInfo->GetImageType();
	int volumeType = imgInfo->GetVolumeType();
	double maxValue = imgData->imgInfo->maxPixelValues.Gray();
	double minValue = imgData->imgInfo->minPixelValues.Gray();
//	int numCols = XSizeOrig/mosaicX;
	int numRows = YSizeOrig/mosaicY;
	int col, row;

	col = row = 0;
	X = Y = Z = T = 0;
	Xm = Ym = Zm = Tm = 0;
	swap = false;

	WriteLog(msg.Format("INT16-ImageDataSize: %d", imageDataSize));
	/* access data through [Y,X,Z,T] */
	for (int j=0;j<imageDataSize;j+=2) {
		/* each pixel is described by two bytes, so concatenate them in the correct order */
		byte1 = tmpBuffer[j];
		byte2 = tmpBuffer[j+1];
		if (swap) byte3 = (byte1 << 8) | byte2;
		else byte3 = (byte2 << 8) | byte1;
	
		if (isMosaic) {
			imgData->imgDataINT16->Set(Y,X,Z,index,0,byte3); /* mosaic will always load 3D image into 4D volume */

			Xm++; /* do all the nice transformations to get the mosaic coordinates into the final volume coordinates */
			if (Xm >= XSizeOrig) { Xm = 0; Ym++; } /* check if the original X is past the last column */
			if (Xm%mosaicX == 0) col = (Xm/mosaicX); /* check if were at a column boundary; if so, set the column */
			X = Xm%mosaicX; /* get the new volume X position */
			if (Ym >= YSizeOrig) { Ym = 0; } /* check if the original Y is past the last row (shouldn't happen) */
			if (Ym%mosaicY == 0) row = (Ym/mosaicY); /* check if were at a row boundary; if so, set the row */
			Y = Ym%mosaicY; /* get the new volume Y position */
			Z = (row*numRows) + col; /* find the new volume slice */
			if (Z >= mosaicZ) break; /* unlikely, but better have it in case something is wrong from before */
		}
		else {
			if ((imageType == IMG_2D) && (volumeType == VOL_2D)) imgData->imgDataINT16->Set(Y,X,0,0,0,byte3); /* load 2D image into 2D volume (single file) */
			if ((imageType == IMG_2D) && (volumeType == VOL_3D)) imgData->imgDataINT16->Set(Y,X,index,0,0,byte3); /* load 2D image into 3D volume */
			if ((imageType == IMG_3D) && (volumeType == VOL_3D)) imgData->imgDataINT16->Set(Y,X,Z,0,0,byte3); /* load 3D image into 3D volume (single file) */
			if ((imageType == IMG_3D) && (volumeType == VOL_4D)) imgData->imgDataINT16->Set(Y,X,Z,index,0,byte3); /* load 3D image into 4D volume */
			if ((imageType == IMG_4D) && (volumeType == VOL_4D)) imgData->imgDataINT16->Set(Y,X,Z,T,0,byte3); /* load 4D image into 4D volume (single file) */

			X++;
			if (X >= XSize) { X = 0; Y++; }
			if (Y >= YSize) { Y = 0; Z++; }
			if (Z >= ZSize) { Z = 0; T++; }
			if (T >= TSize) break;
		}
		/* get the max and min values */
		if ((double)byte3 > maxValue) maxValue = (double)byte3;
		if ((double)byte3 < minValue) minValue = (double)byte3;
	}
	imgInfo->maxPixelValues.SetGray(maxValue);
	imgInfo->minPixelValues.SetGray(minValue);
	return true;
}


// --------------------------------------------------------
// ---------- LoadUINT32ToVolumeBuffer --------------------
// --------------------------------------------------------
bool ImageDataReader::LoadUINT32ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap)
{
	unsigned int byte1, byte2, byte3, byte4, byte5;
	int Xm, Ym, Zm, Tm; /* coordinates for the original mosaic image */
	int X, Y, Z, T; /* coordinates for the final 3D/4D image */
	int imageType = imgInfo->GetImageType();
	int volumeType = imgInfo->GetVolumeType();
	double maxValue = imgData->imgInfo->maxPixelValues.Gray();
	double minValue = imgData->imgInfo->minPixelValues.Gray();
//	int numCols = XSizeOrig/mosaicX;
	int numRows = YSizeOrig/mosaicY;
	int col, row;

	col = row = 0;
	X = Y = Z = T = 0;
	Xm = Ym = Zm = Tm = 0;
	swap = false;

	/* access data through [Y,X,Z,T] */
	for (int j=0;j<imageDataSize;j+=4) {
		/* each pixel is described by two bytes, so concatenate them in the correct order */
		byte1 = tmpBuffer[j];
		byte2 = tmpBuffer[j+1];
		byte3 = tmpBuffer[j+2];
		byte4 = tmpBuffer[j+3];
		if (swap) byte5 = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
		else byte5 = (byte4 << 24) | (byte3 << 16) | (byte2 << 8) | byte1;
	
		if (byte5 < 0) byte5 = 0; /* check for negative values */

		if (isMosaic) {
			imgData->imgDataUINT32->Set(Y,X,Z,index,byte5,0); /* mosaic will always load 3D image into 4D volume */

			Xm++; /* do all the nice transformations to get the mosaic coordinates into the final volume coordinates */
			if (Xm >= XSizeOrig) { Xm = 0; Ym++; } /* check if the original X is past the last column */
			if (Xm%mosaicX == 0) col = (Xm/mosaicX); /* check if were at a column boundary; if so, set the column */
			X = Xm%mosaicX; /* get the new volume X position */
			if (Ym >= YSizeOrig) { Ym = 0; } /* check if the original Y is past the last row (shouldn't happen) */
			if (Ym%mosaicY == 0) row = (Ym/mosaicY); /* check if were at a row boundary; if so, set the row */
			Y = Ym%mosaicY; /* get the new volume Y position */
			Z = (row*numRows) + col; /* find the new volume slice */
			if (Z >= mosaicZ) break; /* unlikely, but better have it in case something is wrong from before */
		}
		else {
			if ((imageType == IMG_2D) && (volumeType == VOL_2D)) imgData->imgDataUINT32->Set(Y,X,0,0,0,byte5); /* load 2D image into 2D volume (single file) */
			if ((imageType == IMG_2D) && (volumeType == VOL_3D)) imgData->imgDataUINT32->Set(Y,X,index,0,0,byte5); /* load 2D image into 3D volume */
			if ((imageType == IMG_3D) && (volumeType == VOL_3D)) imgData->imgDataUINT32->Set(Y,X,Z,0,0,byte5); /* load 3D image into 3D volume (single file) */
			if ((imageType == IMG_3D) && (volumeType == VOL_4D)) imgData->imgDataUINT32->Set(Y,X,Z,index,0,byte5); /* load 3D image into 4D volume */
			if ((imageType == IMG_4D) && (volumeType == VOL_4D)) imgData->imgDataUINT32->Set(Y,X,Z,T,0,byte5); /* load 4D image into 4D volume (single file) */
			
			X++;
			if (X >= XSize) { X = 0; Y++; }
			if (Y >= YSize) { Y = 0; Z++; }
			if (Z >= ZSize) { Z = 0; T++; }
			if (T >= TSize) break;
		}
		/* get the max and min values */
		if ((double)byte5 > maxValue) maxValue = (double)byte5;
		if ((double)byte5 < minValue) minValue = (double)byte5;
	}
	imgData->imgInfo->maxPixelValues.SetGray(maxValue);
	imgData->imgInfo->minPixelValues.SetGray(minValue);
	return true;
}


// --------------------------------------------------------
// ---------- LoadINT32ToVolumeBuffer --------------------
// --------------------------------------------------------
bool ImageDataReader::LoadINT32ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap)
{
	int byte1, byte2, byte3, byte4, byte5;
	int Xm, Ym, Zm, Tm; /* coordinates for the original mosaic image */
	int X, Y, Z, T; /* coordinates for the final 3D/4D image */
	int imageType = imgInfo->GetImageType();
	int volumeType = imgInfo->GetVolumeType();
	double maxValue = imgData->imgInfo->maxPixelValues.Gray();
	double minValue = imgData->imgInfo->minPixelValues.Gray();
//	int numCols = XSizeOrig/mosaicX;
	int numRows = YSizeOrig/mosaicY;
	int col, row;

	col = row = 0;
	X = Y = Z = T = 0;
	Xm = Ym = Zm = Tm = 0;
	swap = false;

	/* access data through [Y,X,Z,T] */
	for (int j=0;j<imageDataSize;j+=4) {
		/* each pixel is described by two bytes, so concatenate them in the correct order */
		byte1 = tmpBuffer[j];
		byte2 = tmpBuffer[j+1];
		byte3 = tmpBuffer[j+2];
		byte4 = tmpBuffer[j+3];
		if (swap) byte5 = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
		else byte5 = (byte4 << 24) | (byte3 << 16) | (byte2 << 8) | byte1;
	
		if (isMosaic) {
			imgData->imgDataINT32->Set(Y,X,Z,index,0,byte5); /* mosaic will always load 3D image into 4D volume */

			Xm++; /* do all the nice transformations to get the mosaic coordinates into the final volume coordinates */
			if (Xm >= XSizeOrig) { Xm = 0; Ym++; } /* check if the original X is past the last column */
			if (Xm%mosaicX == 0) col = (Xm/mosaicX); /* check if were at a column boundary; if so, set the column */
			X = Xm%mosaicX; /* get the new volume X position */
			if (Ym >= YSizeOrig) { Ym = 0; } /* check if the original Y is past the last row (shouldn't happen) */
			if (Ym%mosaicY == 0) row = (Ym/mosaicY); /* check if were at a row boundary; if so, set the row */
			Y = Ym%mosaicY; /* get the new volume Y position */
			Z = (row*numRows) + col; /* find the new volume slice */
			if (Z >= mosaicZ) break; /* unlikely, but better have it in case something is wrong from before */
		}
		else {
			if ((imageType == IMG_2D) && (volumeType == VOL_2D)) imgData->imgDataINT32->Set(Y,X,0,0,0,byte5); /* load 2D image into 2D volume (single file) */
			if ((imageType == IMG_2D) && (volumeType == VOL_3D)) imgData->imgDataINT32->Set(Y,X,index,0,0,byte5); /* load 2D image into 3D volume */
			if ((imageType == IMG_3D) && (volumeType == VOL_3D)) imgData->imgDataINT32->Set(Y,X,Z,0,0,byte5); /* load 3D image into 3D volume (single file) */
			if ((imageType == IMG_3D) && (volumeType == VOL_4D)) imgData->imgDataINT32->Set(Y,X,Z,index,0,byte5); /* load 3D image into 4D volume */
			if ((imageType == IMG_4D) && (volumeType == VOL_4D)) imgData->imgDataINT32->Set(Y,X,Z,T,0,byte5); /* load 4D image into 4D volume (single file) */

			X++;
			if (X >= XSize) { X = 0; Y++; }
			if (Y >= YSize) { Y = 0; Z++; }
			if (Z >= ZSize) { Z = 0; T++; }
			if (T >= TSize) break;
		}
		/* get the max and min values */
		if ((double)byte5 > maxValue) maxValue = (double)byte5;
		if ((double)byte5 < minValue) minValue = (double)byte5;
	}
	imgData->imgInfo->maxPixelValues.SetGray(maxValue);
	imgData->imgInfo->minPixelValues.SetGray(minValue);
	return true;
}


// --------------------------------------------------------
// ---------- LoadUINT64ToVolumeBuffer --------------------
// --------------------------------------------------------
bool ImageDataReader::LoadUINT64ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap)
{
	unsigned long long int byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8, byte9;
	int Xm, Ym, Zm, Tm; /* coordinates for the original mosaic image */
	int X, Y, Z, T; /* coordinates for the final 3D/4D image */
	int imageType = imgInfo->GetImageType();
	int volumeType = imgInfo->GetVolumeType();
	double maxValue = imgData->imgInfo->maxPixelValues.Gray();
	double minValue = imgData->imgInfo->minPixelValues.Gray();
//	int numCols = XSizeOrig/mosaicX;
	int numRows = YSizeOrig/mosaicY;
	int col, row;

	col = row = 0;
	X = Y = Z = T = 0;
	Xm = Ym = Zm = Tm = 0;
	swap = false;

	/* access data through [Y,X,Z,T] */
	for (int j=0;j<imageDataSize;j+=8) {
		/* each pixel is described by two bytes, so concatenate them in the correct order */
		byte1 = tmpBuffer[j];
		byte2 = tmpBuffer[j+1];
		byte3 = tmpBuffer[j+2];
		byte4 = tmpBuffer[j+3];
		byte5 = tmpBuffer[j+4];
		byte6 = tmpBuffer[j+5];
		byte7 = tmpBuffer[j+6];
		byte8 = tmpBuffer[j+7];
		if (swap) byte9 = (byte1 << 56) | (byte2 << 48) | (byte3 << 40) | (byte4 << 32) | (byte5 << 24) | (byte6 << 16) | (byte7 << 8) | byte8;
		else byte9 = (byte8 << 56) | (byte4 << 48) | (byte4 << 40) | (byte4 << 32) | (byte4 << 24) | (byte3 << 16) | (byte2 << 8) | byte1;
	
		if (byte9 < 0) byte9 = 0; /* check for negative values */

		if (isMosaic) {
			imgData->imgDataUINT64->Set(Y,X,Z,index,0,byte9); /* mosaic will always load 3D image into 4D volume */

			Xm++; /* do all the nice transformations to get the mosaic coordinates into the final volume coordinates */
			if (Xm >= XSizeOrig) { Xm = 0; Ym++; } /* check if the original X is past the last column */
			if (Xm%mosaicX == 0) col = (Xm/mosaicX); /* check if were at a column boundary; if so, set the column */
			X = Xm%mosaicX; /* get the new volume X position */
			if (Ym >= YSizeOrig) { Ym = 0; } /* check if the original Y is past the last row (shouldn't happen) */
			if (Ym%mosaicY == 0) row = (Ym/mosaicY); /* check if were at a row boundary; if so, set the row */
			Y = Ym%mosaicY; /* get the new volume Y position */
			Z = (row*numRows) + col; /* find the new volume slice */
			if (Z >= mosaicZ) break; /* unlikely, but better have it in case something is wrong from before */
		}
		else {
			if ((imageType == IMG_2D) && (volumeType == VOL_2D)) imgData->imgDataUINT64->Set(Y,X,0,0,0,byte9); /* load 2D image into 2D volume (single file) */
			if ((imageType == IMG_2D) && (volumeType == VOL_3D)) imgData->imgDataUINT64->Set(Y,X,index,0,0,byte9); /* load 2D image into 3D volume */
			if ((imageType == IMG_3D) && (volumeType == VOL_3D)) imgData->imgDataUINT64->Set(Y,X,Z,0,0,byte9); /* load 3D image into 3D volume (single file) */
			if ((imageType == IMG_3D) && (volumeType == VOL_4D)) imgData->imgDataUINT64->Set(Y,X,Z,index,0,byte9); /* load 3D image into 4D volume */
			if ((imageType == IMG_4D) && (volumeType == VOL_4D)) imgData->imgDataUINT64->Set(Y,X,Z,T,0,byte9); /* load 4D image into 4D volume (single file) */
			X++;
			if (X >= XSize) { X = 0; Y++; }
			if (Y >= YSize) { Y = 0; Z++; }
			if (Z >= ZSize) { Z = 0; T++; }
			if (T >= TSize) break;
		}
		/* get the max and min values */
		if ((double)byte9 > maxValue) maxValue = (double)byte9;
		if ((double)byte9 < minValue) minValue = (double)byte9;
	}
	imgData->imgInfo->maxPixelValues.SetGray(maxValue);
	imgData->imgInfo->minPixelValues.SetGray(minValue);
	return true;
}


// --------------------------------------------------------
// ---------- LoadINT64ToVolumeBuffer ---------------------
// --------------------------------------------------------
bool ImageDataReader::LoadINT64ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap)
{
	long long int byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8, byte9;
	int Xm, Ym, Zm, Tm; /* coordinates for the original mosaic image */
	int X, Y, Z, T; /* coordinates for the final 3D/4D image */
	int imageType = imgInfo->GetImageType();
	int volumeType = imgInfo->GetVolumeType();
	double maxValue = imgData->imgInfo->maxPixelValues.Gray();
	double minValue = imgData->imgInfo->minPixelValues.Gray();
//	int numCols = XSizeOrig/mosaicX;
	int numRows = YSizeOrig/mosaicY;
	int col, row;

	col = row = 0;
	X = Y = Z = T = 0;
	Xm = Ym = Zm = Tm = 0;
	swap = false;

	/* access data through [Y,X,Z,T] */
	for (int j=0;j<imageDataSize;j+=8) {
		/* each pixel is described by two bytes, so concatenate them in the correct order */
		byte1 = tmpBuffer[j];
		byte2 = tmpBuffer[j+1];
		byte3 = tmpBuffer[j+2];
		byte4 = tmpBuffer[j+3];
		byte5 = tmpBuffer[j+4];
		byte6 = tmpBuffer[j+5];
		byte7 = tmpBuffer[j+6];
		byte8 = tmpBuffer[j+7];
		if (swap) byte9 = (byte1 << 56) | (byte2 << 48) | (byte3 << 40) | (byte4 << 32) | (byte5 << 24) | (byte6 << 16) | (byte7 << 8) | byte8;
		else byte9 = (byte8 << 56) | (byte4 << 48) | (byte4 << 40) | (byte4 << 32) | (byte4 << 24) | (byte3 << 16) | (byte2 << 8) | byte1;
	
		if (isMosaic) {
			imgData->imgDataINT64->Set(Y,X,Z,index,0,byte9); /* mosaic will always load 3D image into 4D volume */

			Xm++; /* do all the nice transformations to get the mosaic coordinates into the final volume coordinates */
			if (Xm >= XSizeOrig) { Xm = 0; Ym++; } /* check if the original X is past the last column */
			if (Xm%mosaicX == 0) col = (Xm/mosaicX); /* check if were at a column boundary; if so, set the column */
			X = Xm%mosaicX; /* get the new volume X position */
			if (Ym >= YSizeOrig) { Ym = 0; } /* check if the original Y is past the last row (shouldn't happen) */
			if (Ym%mosaicY == 0) row = (Ym/mosaicY); /* check if were at a row boundary; if so, set the row */
			Y = Ym%mosaicY; /* get the new volume Y position */
			Z = (row*numRows) + col; /* find the new volume slice */
			if (Z >= mosaicZ) break; /* unlikely, but better have it in case something is wrong from before */
		}
		else {
			if ((imageType == IMG_2D) && (volumeType == VOL_2D)) imgData->imgDataINT64->Set(Y,X,0,0,0,byte9); /* load 2D image into 2D volume (single file) */
			if ((imageType == IMG_2D) && (volumeType == VOL_3D)) imgData->imgDataINT64->Set(Y,X,index,0,0,byte9); /* load 2D image into 3D volume */
			if ((imageType == IMG_3D) && (volumeType == VOL_3D)) imgData->imgDataINT64->Set(Y,X,Z,0,0,byte9); /* load 3D image into 3D volume (single file) */
			if ((imageType == IMG_3D) && (volumeType == VOL_4D)) imgData->imgDataINT64->Set(Y,X,Z,index,0,byte9); /* load 3D image into 4D volume */
			if ((imageType == IMG_4D) && (volumeType == VOL_4D)) imgData->imgDataINT64->Set(Y,X,Z,T,0,byte9); /* load 4D image into 4D volume (single file) */

			X++;
			if (X >= XSize) { X = 0; Y++; }
			if (Y >= YSize) { Y = 0; Z++; }
			if (Z >= ZSize) { Z = 0; T++; }
			if (T >= TSize) break;
		}
		/* get the max and min values */
		if ((double)byte9 > maxValue) maxValue = (double)byte9;
		if ((double)byte9 < minValue) minValue = (double)byte9;
	}
	imgData->imgInfo->maxPixelValues.SetGray(maxValue);
	imgData->imgInfo->minPixelValues.SetGray(minValue);
	return true;
}


// --------------------------------------------------------
// ---------- LoadFLOAT32ToVolumeBuffer --------------------
// --------------------------------------------------------
bool ImageDataReader::LoadFLOAT32ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap)
{
//	unsigned char byte1, byte2, byte3, byte4, byte5;
	unsigned char bytes[4];
	float value;
	int Xm, Ym, Zm, Tm; /* coordinates for the original mosaic image */
	int X, Y, Z, T; /* coordinates for the final 3D/4D image */
	int imageType = imgInfo->GetImageType();
	int volumeType = imgInfo->GetVolumeType();
	double maxValue = imgData->imgInfo->maxPixelValues.Gray();
	double minValue = imgData->imgInfo->minPixelValues.Gray();
//	int numCols = XSizeOrig/mosaicX;
	int numRows = YSizeOrig/mosaicY;
	int col, row;

	col = row = 0;
	X = Y = Z = T = 0;
	Xm = Ym = Zm = Tm = 0;
	swap = false;

	/* access data through [Y,X,Z,T] */
	for (int j=0;j<imageDataSize;j+=4) {
		if (swap) {
			for (int i=0;i<4;i++)
				bytes[i] = tmpBuffer[j+i];
		}
		else {
			for (int i=3;i>=0;i--)
				bytes[i] = tmpBuffer[j+abs(3-i)];
		}
		memcpy(&value, (void *)bytes, 4);
	
		if (isMosaic) {
			imgData->imgDataFLOAT32->Set(Y,X,Z,index,0,value); /* mosaic will always load 3D image into 4D volume */

			Xm++; /* do all the nice transformations to get the mosaic coordinates into the final volume coordinates */
			if (Xm >= XSizeOrig) { Xm = 0; Ym++; } /* check if the original X is past the last column */
			if (Xm%mosaicX == 0) col = (Xm/mosaicX); /* check if were at a column boundary; if so, set the column */
			X = Xm%mosaicX; /* get the new volume X position */
			if (Ym >= YSizeOrig) { Ym = 0; } /* check if the original Y is past the last row (shouldn't happen) */
			if (Ym%mosaicY == 0) row = (Ym/mosaicY); /* check if were at a row boundary; if so, set the row */
			Y = Ym%mosaicY; /* get the new volume Y position */
			Z = (row*numRows) + col; /* find the new volume slice */
			if (Z >= mosaicZ) break; /* unlikely, but better have it in case something is wrong from before */
		}
		else {
			if ((imageType == IMG_2D) && (volumeType == VOL_2D)) imgData->imgDataFLOAT32->Set(Y,X,0,0,0,value); /* load 2D image into 2D volume (single file) */
			if ((imageType == IMG_2D) && (volumeType == VOL_3D)) imgData->imgDataFLOAT32->Set(Y,X,index,0,0,value); /* load 2D image into 3D volume */
			if ((imageType == IMG_3D) && (volumeType == VOL_3D)) imgData->imgDataFLOAT32->Set(Y,X,Z,0,0,value); /* load 3D image into 3D volume (single file) */
			if ((imageType == IMG_3D) && (volumeType == VOL_4D)) imgData->imgDataFLOAT32->Set(Y,X,Z,index,0,value); /* load 3D image into 4D volume */
			if ((imageType == IMG_4D) && (volumeType == VOL_4D)) imgData->imgDataFLOAT32->Set(Y,X,Z,T,0,value); /* load 4D image into 4D volume (single file) */

			X++;
			if (X >= XSize) { X = 0; Y++; }
			if (Y >= YSize) { Y = 0; Z++; }
			if (Z >= ZSize) { Z = 0; T++; }
			if (T >= TSize) break;
		}
		/* get the max and min values */
		if ((double)value > maxValue) maxValue = (double)value;
		if ((double)value < minValue) minValue = (double)value;
	}
	imgData->imgInfo->maxPixelValues.SetGray(maxValue);
	imgData->imgInfo->minPixelValues.SetGray(minValue);
	return true;
}


// --------------------------------------------------------
// ---------- LoadFLOAT64ToVolumeBuffer --------------------
// --------------------------------------------------------
bool ImageDataReader::LoadFLOAT64ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap)
{
	unsigned char bytes[8];
	double value;
	int Xm, Ym, Zm, Tm; /* coordinates for the original mosaic image */
	int X, Y, Z, T; /* coordinates for the final 3D/4D image */
	int imageType = imgInfo->GetImageType();
	int volumeType = imgInfo->GetVolumeType();
	double maxValue = imgData->imgInfo->maxPixelValues.Gray();
	double minValue = imgData->imgInfo->minPixelValues.Gray();
	int numRows = YSizeOrig/mosaicY;
	int col, row;

	col = row = 0;
	X = Y = Z = T = 0;
	Xm = Ym = Zm = Tm = 0;
	swap = false;

	/* access data through [Y,X,Z,T] */
	for (int j=0;j<imageDataSize;j+=8) {
		if (swap) {
			for (int i=0;i<8;i++)
				bytes[i] = tmpBuffer[j+i];
		}
		else {
			for (int i=7;i>=0;i--)
				bytes[i] = tmpBuffer[j+abs(7-i)];
		}
		memcpy(&value, (void *)bytes, 8);
	
		if (isMosaic) {
			imgData->imgDataFLOAT64->Set(Y,X,Z,index,0,value); /* mosaic will always load 3D image into 4D volume */

			Xm++; /* do all the nice transformations to get the mosaic coordinates into the final volume coordinates */
			if (Xm >= XSizeOrig) { Xm = 0; Ym++; } /* check if the original X is past the last column */
			if (Xm%mosaicX == 0) col = (Xm/mosaicX); /* check if were at a column boundary; if so, set the column */
			X = Xm%mosaicX; /* get the new volume X position */
			if (Ym >= YSizeOrig) { Ym = 0; } /* check if the original Y is past the last row (shouldn't happen) */
			if (Ym%mosaicY == 0) row = (Ym/mosaicY); /* check if were at a row boundary; if so, set the row */
			Y = Ym%mosaicY; /* get the new volume Y position */
			Z = (row*numRows) + col; /* find the new volume slice */
			if (Z >= mosaicZ) break; /* unlikely, but better have it in case something is wrong from before */
		}
		else {
			if ((imageType == IMG_2D) && (volumeType == VOL_2D)) imgData->imgDataFLOAT64->Set(Y,X,0,0,0,value); /* load 2D image into 2D volume (single file) */
			if ((imageType == IMG_2D) && (volumeType == VOL_3D)) imgData->imgDataFLOAT64->Set(Y,X,index,0,0,value); /* load 2D image into 3D volume */
			if ((imageType == IMG_3D) && (volumeType == VOL_3D)) imgData->imgDataFLOAT64->Set(Y,X,Z,0,0,value); /* load 3D image into 3D volume (single file) */
			if ((imageType == IMG_3D) && (volumeType == VOL_4D)) imgData->imgDataFLOAT64->Set(Y,X,Z,index,0,value); /* load 3D image into 4D volume */
			if ((imageType == IMG_4D) && (volumeType == VOL_4D)) imgData->imgDataFLOAT64->Set(Y,X,Z,T,0,value); /* load 4D image into 4D volume (single file) */

			X++;
			if (X >= XSize) { X = 0; Y++; }
			if (Y >= YSize) { Y = 0; Z++; }
			if (Z >= ZSize) { Z = 0; T++; }
			if (T >= TSize) break;
		}
		/* get the max and min values */
		if (value > maxValue) maxValue = value;
		if (value < minValue) minValue = value;
	}
	imgData->imgInfo->maxPixelValues.SetGray(maxValue);
	imgData->imgInfo->minPixelValues.SetGray(minValue);
	return true;
}


// --------------------------------------------------------
// ---------- LoadFLOAT128ToVolumeBuffer --------------------
// --------------------------------------------------------
bool ImageDataReader::LoadFLOAT128ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap)
{
	unsigned char bytes[16];
	FLOAT_128 value;
	int Xm, Ym, Zm, Tm; /* coordinates for the original mosaic image */
	int X, Y, Z, T; /* coordinates for the final 3D/4D image */
	int imageType = imgInfo->GetImageType();
	int volumeType = imgInfo->GetVolumeType();
	double maxValue = imgData->imgInfo->maxPixelValues.Gray();
	double minValue = imgData->imgInfo->minPixelValues.Gray();
//	int numCols = XSizeOrig/mosaicX;
	int numRows = YSizeOrig/mosaicY;
	int col, row;

	col = row = 0;
	X = Y = Z = T = 0;
	Xm = Ym = Zm = Tm = 0;
	swap = false;

	/* access data through [Y,X,Z,T] */
	for (int j=0;j<imageDataSize;j+=16) {
		if (swap) {
			for (int i=0;i<16;i++)
				bytes[i] = tmpBuffer[j+i];
		}
		else {
			for (int i=15;i>=0;i--)
				bytes[i] = tmpBuffer[j+abs(15-i)];
		}
		memcpy(&value, (void *)bytes, 16);
	
		if (isMosaic) {
			imgData->imgDataFLOAT128->Set(Y,X,Z,index,0,value); /* mosaic will always load 3D image into 4D volume */

			Xm++; /* do all the nice transformations to get the mosaic coordinates into the final volume coordinates */
			if (Xm >= XSizeOrig) { Xm = 0; Ym++; } /* check if the original X is past the last column */
			if (Xm%mosaicX == 0) col = (Xm/mosaicX); /* check if were at a column boundary; if so, set the column */
			X = Xm%mosaicX; /* get the new volume X position */
			if (Ym >= YSizeOrig) { Ym = 0; } /* check if the original Y is past the last row (shouldn't happen) */
			if (Ym%mosaicY == 0) row = (Ym/mosaicY); /* check if were at a row boundary; if so, set the row */
			Y = Ym%mosaicY; /* get the new volume Y position */
			Z = (row*numRows) + col; /* find the new volume slice */
			if (Z >= mosaicZ) break; /* unlikely, but better have it in case something is wrong from before */
		}
		else {
			if ((imageType == IMG_2D) && (volumeType == VOL_2D)) imgData->imgDataFLOAT128->Set(Y,X,0,0,0,value); /* load 2D image into 2D volume (single file) */
			if ((imageType == IMG_2D) && (volumeType == VOL_3D)) imgData->imgDataFLOAT128->Set(Y,X,index,0,0,value); /* load 2D image into 3D volume */
			if ((imageType == IMG_3D) && (volumeType == VOL_3D)) imgData->imgDataFLOAT128->Set(Y,X,Z,0,0,value); /* load 3D image into 3D volume (single file) */
			if ((imageType == IMG_3D) && (volumeType == VOL_4D)) imgData->imgDataFLOAT128->Set(Y,X,Z,index,0,value); /* load 3D image into 4D volume */
			if ((imageType == IMG_4D) && (volumeType == VOL_4D)) imgData->imgDataFLOAT128->Set(Y,X,Z,T,0,value); /* load 4D image into 4D volume (single file) */

			X++;
			if (X >= XSize) { X = 0; Y++; }
			if (Y >= YSize) { Y = 0; Z++; }
			if (Z >= ZSize) { Z = 0; T++; }
			if (T >= TSize) break;
		}
		/* get the max and min values */
		if ((double)value > maxValue) maxValue = (double)value;
		if ((double)value < minValue) minValue = (double)value;
	}
	imgData->imgInfo->maxPixelValues.SetGray(maxValue);
	imgData->imgInfo->minPixelValues.SetGray(minValue);
	return true;
}


// --------------------------------------------------------
// ---------- LoadCOMPLEX64ToVolumeBuffer -----------------
// --------------------------------------------------------
bool ImageDataReader::LoadCOMPLEX64ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap)
{
	unsigned char bytes1[4];
	unsigned char bytes2[4];
	FLOAT_32 value1, value2;
	int Xm, Ym, Zm, Tm; /* coordinates for the original mosaic image */
	int X, Y, Z, T; /* coordinates for the final 3D/4D image */
	int imageType = imgInfo->GetImageType();
	int volumeType = imgInfo->GetVolumeType();
	double maxValue = imgData->imgInfo->maxPixelValues.Gray();
	double minValue = imgData->imgInfo->minPixelValues.Gray();
//	int numCols = XSizeOrig/mosaicX;
	int numRows = YSizeOrig/mosaicY;
	int col, row;

	col = row = 0;
	X = Y = Z = T = 0;
	Xm = Ym = Zm = Tm = 0;
	swap = false;

	/* access data through [Y,X,Z,T] */
	for (int j=0;j<imageDataSize;j+=8) {
		if (swap) {
			for (int i=0;i<4;i++)
				bytes1[i] = tmpBuffer[j+i];
			for (int i=0;i<4;i++)
				bytes2[i] = tmpBuffer[j+i+4];
		}
		else {
			for (int i=3;i>=0;i--)
				bytes1[i] = tmpBuffer[j+abs(3-i)];
			for (int i=3;i>=0;i--)
				bytes1[i] = tmpBuffer[j+abs(3-i)+4];
		}
		memcpy(&value1, (void *)bytes1, 4);
		memcpy(&value2, (void *)bytes2, 4);
	
		if (isMosaic) {
			imgData->imgDataCOMPLEX64->Set(Y,X,Z,index,0,value1); /* mosaic will always load 3D image into 4D volume */
			imgData->imgDataCOMPLEX64->Set(Y,X,Z,index,1,value2); /* mosaic will always load 3D image into 4D volume */

			Xm++; /* do all the nice transformations to get the mosaic coordinates into the final volume coordinates */
			if (Xm >= XSizeOrig) { Xm = 0; Ym++; } /* check if the original X is past the last column */
			if (Xm%mosaicX == 0) col = (Xm/mosaicX); /* check if were at a column boundary; if so, set the column */
			X = Xm%mosaicX; /* get the new volume X position */
			if (Ym >= YSizeOrig) { Ym = 0; } /* check if the original Y is past the last row (shouldn't happen) */
			if (Ym%mosaicY == 0) row = (Ym/mosaicY); /* check if were at a row boundary; if so, set the row */
			Y = Ym%mosaicY; /* get the new volume Y position */
			Z = (row*numRows) + col; /* find the new volume slice */
			if (Z >= mosaicZ) break; /* unlikely, but better have it in case something is wrong from before */
		}
		else {
			if ((imageType == IMG_2D) && (volumeType == VOL_2D)) { /* load 2D image into 2D volume (single file) */
				imgData->imgDataCOMPLEX64->Set(Y,X,0,0,0,value1);
				imgData->imgDataCOMPLEX64->Set(Y,X,0,0,1,value2);
			}
			if ((imageType == IMG_2D) && (volumeType == VOL_3D)) { /* load 2D image into 3D volume */
				imgData->imgDataCOMPLEX64->Set(Y,X,index,0,0,value1);
				imgData->imgDataCOMPLEX64->Set(Y,X,index,0,1,value2);
			}
			if ((imageType == IMG_3D) && (volumeType == VOL_3D)) { /* load 3D image into 3D volume (single file) */
				imgData->imgDataCOMPLEX64->Set(Y,X,Z,0,0,value1);
				imgData->imgDataCOMPLEX64->Set(Y,X,Z,0,1,value2);
			}
			if ((imageType == IMG_3D) && (volumeType == VOL_4D)) { /* load 3D image into 4D volume */
				imgData->imgDataCOMPLEX64->Set(Y,X,Z,index,0,value1);
				imgData->imgDataCOMPLEX64->Set(Y,X,Z,index,1,value2);
			}
			if ((imageType == IMG_4D) && (volumeType == VOL_4D)) { /* load 4D image into 4D volume (single file) */
				imgData->imgDataCOMPLEX64->Set(Y,X,Z,T,0,value1);
				imgData->imgDataCOMPLEX64->Set(Y,X,Z,T,1,value2);
			}

			X++;
			if (X >= XSize) { X = 0; Y++; }
			if (Y >= YSize) { Y = 0; Z++; }
			if (Z >= ZSize) { Z = 0; T++; }
			if (T >= TSize) break;
		}
		/* get the max and min values */
		if ((double)value1 > maxValue) maxValue = (double)value1;
		if ((double)value1 < minValue) minValue = (double)value1;
	}
	imgData->imgInfo->maxPixelValues.SetGray(maxValue);
	imgData->imgInfo->minPixelValues.SetGray(minValue);
	return true;
}


// --------------------------------------------------------
// ---------- LoadCOMPLEX128ToVolumeBuffer -----------------
// --------------------------------------------------------
bool ImageDataReader::LoadCOMPLEX128ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap)
{
	unsigned char bytes1[8];
	unsigned char bytes2[8];
	double value1, value2;
	int Xm, Ym, Zm, Tm; /* coordinates for the original mosaic image */
	int X, Y, Z, T; /* coordinates for the final 3D/4D image */
	int imageType = imgInfo->GetImageType();
	int volumeType = imgInfo->GetVolumeType();
	double maxValue = imgData->imgInfo->maxPixelValues.Gray();
	double minValue = imgData->imgInfo->minPixelValues.Gray();
//	int numCols = XSizeOrig/mosaicX;
	int numRows = YSizeOrig/mosaicY;
	int col, row;

	col = row = 0;
	X = Y = Z = T = 0;
	Xm = Ym = Zm = Tm = 0;
	swap = false;

	/* access data through [Y,X,Z,T] */
	for (int j=0;j<imageDataSize;j+=16) {
		if (swap) {
			for (int i=0;i<8;i++)
				bytes1[i] = tmpBuffer[j+i];
			for (int i=0;i<8;i++)
				bytes2[i] = tmpBuffer[j+i+8];
		}
		else {
			for (int i=7;i>=0;i--)
				bytes1[i] = tmpBuffer[j+abs(7-i)];
			for (int i=7;i>=0;i--)
				bytes1[i] = tmpBuffer[j+abs(7-i)+8];
		}
		memcpy(&value1, (void *)bytes1, 8);
		memcpy(&value2, (void *)bytes2, 8);
	
		if (isMosaic) {
			imgData->imgDataCOMPLEX128->Set(Y,X,Z,index,0,value1); /* mosaic will always load 3D image into 4D volume */
			imgData->imgDataCOMPLEX128->Set(Y,X,Z,index,1,value2);

			Xm++; /* do all the nice transformations to get the mosaic coordinates into the final volume coordinates */
			if (Xm >= XSizeOrig) { Xm = 0; Ym++; } /* check if the original X is past the last column */
			if (Xm%mosaicX == 0) col = (Xm/mosaicX); /* check if were at a column boundary; if so, set the column */
			X = Xm%mosaicX; /* get the new volume X position */
			if (Ym >= YSizeOrig) { Ym = 0; } /* check if the original Y is past the last row (shouldn't happen) */
			if (Ym%mosaicY == 0) row = (Ym/mosaicY); /* check if were at a row boundary; if so, set the row */
			Y = Ym%mosaicY; /* get the new volume Y position */
			Z = (row*numRows) + col; /* find the new volume slice */
			if (Z >= mosaicZ) break; /* unlikely, but better have it in case something is wrong from before */
		}
		else {
			if ((imageType == IMG_2D) && (volumeType == VOL_2D)) { /* load 2D image into 2D volume (single file) */
				imgData->imgDataCOMPLEX128->Set(Y,X,0,0,0,value1);
				imgData->imgDataCOMPLEX128->Set(Y,X,0,0,1,value2);
			}
			if ((imageType == IMG_2D) && (volumeType == VOL_3D)) { /* load 2D image into 3D volume */
				imgData->imgDataCOMPLEX128->Set(Y,X,index,0,0,value1);
				imgData->imgDataCOMPLEX128->Set(Y,X,index,0,1,value2);
			}
			if ((imageType == IMG_3D) && (volumeType == VOL_3D)) { /* load 3D image into 3D volume (single file) */
				imgData->imgDataCOMPLEX128->Set(Y,X,Z,0,0,value1);
				imgData->imgDataCOMPLEX128->Set(Y,X,Z,0,1,value2);
			}
			if ((imageType == IMG_3D) && (volumeType == VOL_4D)) { /* load 3D image into 4D volume */
				imgData->imgDataCOMPLEX128->Set(Y,X,Z,index,0,value1);
				imgData->imgDataCOMPLEX128->Set(Y,X,Z,index,1,value2);
			}
			if ((imageType == IMG_4D) && (volumeType == VOL_4D)) { /* load 4D image into 4D volume (single file) */
				imgData->imgDataCOMPLEX128->Set(Y,X,Z,T,0,value1);
				imgData->imgDataCOMPLEX128->Set(Y,X,Z,T,1,value2);
			}

			X++;
			if (X >= XSize) { X = 0; Y++; }
			if (Y >= YSize) { Y = 0; Z++; }
			if (Z >= ZSize) { Z = 0; T++; }
			if (T >= TSize) break;
		}
		/* get the max and min values */
		if ((double)value1 > maxValue) maxValue = (double)value1;
		if ((double)value1 < minValue) minValue = (double)value1;
	}
	imgData->imgInfo->maxPixelValues.SetGray(maxValue);
	imgData->imgInfo->minPixelValues.SetGray(minValue);
	return true;
}


// --------------------------------------------------------
// ---------- LoadCOMPLEX256ToVolumeBuffer -----------------
// --------------------------------------------------------
bool ImageDataReader::LoadCOMPLEX256ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap)
{
	unsigned char bytes1[16];
	unsigned char bytes2[16];
	long double value1, value2;
	int Xm, Ym, Zm, Tm; /* coordinates for the original mosaic image */
	int X, Y, Z, T; /* coordinates for the final 3D/4D image */
	int imageType = imgInfo->GetImageType();
	int volumeType = imgInfo->GetVolumeType();
	double maxValue = imgData->imgInfo->maxPixelValues.Gray();
	double minValue = imgData->imgInfo->minPixelValues.Gray();
//	int numCols = XSizeOrig/mosaicX;
	int numRows = YSizeOrig/mosaicY;
	int col, row;

	col = row = 0;
	X = Y = Z = T = 0;
	Xm = Ym = Zm = Tm = 0;
	swap = false;

	/* access data through [Y,X,Z,T] */
	for (int j=0;j<imageDataSize;j+=32) {
		if (swap) {
			for (int i=0;i<16;i++)
				bytes1[i] = tmpBuffer[j+i];
			for (int i=0;i<16;i++)
				bytes2[i] = tmpBuffer[j+i+16];
		}
		else {
			for (int i=15;i>=0;i--)
				bytes1[i] = tmpBuffer[j+abs(15-i)];
			for (int i=15;i>=0;i--)
				bytes1[i] = tmpBuffer[j+abs(15-i)+16];
		}
		memcpy(&value1, (void *)bytes1, 16);
		memcpy(&value2, (void *)bytes2, 16);
	
		if (isMosaic) {
			imgData->imgDataCOMPLEX256->Set(Y,X,Z,index,0,value1); /* mosaic will always load 3D image into 4D volume */
			imgData->imgDataCOMPLEX256->Set(Y,X,Z,index,1,value2);

			Xm++; /* do all the nice transformations to get the mosaic coordinates into the final volume coordinates */
			if (Xm >= XSizeOrig) { Xm = 0; Ym++; } /* check if the original X is past the last column */
			if (Xm%mosaicX == 0) col = (Xm/mosaicX); /* check if were at a column boundary; if so, set the column */
			X = Xm%mosaicX; /* get the new volume X position */
			if (Ym >= YSizeOrig) { Ym = 0; } /* check if the original Y is past the last row (shouldn't happen) */
			if (Ym%mosaicY == 0) row = (Ym/mosaicY); /* check if were at a row boundary; if so, set the row */
			Y = Ym%mosaicY; /* get the new volume Y position */
			Z = (row*numRows) + col; /* find the new volume slice */
			if (Z >= mosaicZ) break; /* unlikely, but better have it in case something is wrong from before */
		}
		else {
			if ((imageType == IMG_2D) && (volumeType == VOL_2D)) { /* load 2D image into 2D volume (single file) */
				imgData->imgDataCOMPLEX256->Set(Y,X,0,0,0,value1);
				imgData->imgDataCOMPLEX256->Set(Y,X,0,0,1,value2);
			}
			if ((imageType == IMG_2D) && (volumeType == VOL_3D)) { /* load 2D image into 3D volume */
				imgData->imgDataCOMPLEX256->Set(Y,X,index,0,0,value1);
				imgData->imgDataCOMPLEX256->Set(Y,X,index,0,1,value2);
			}
			if ((imageType == IMG_3D) && (volumeType == VOL_3D)) { /* load 3D image into 3D volume (single file) */
				imgData->imgDataCOMPLEX256->Set(Y,X,Z,0,0,value1);
				imgData->imgDataCOMPLEX256->Set(Y,X,Z,0,1,value2);
			}
			if ((imageType == IMG_3D) && (volumeType == VOL_4D)) { /* load 3D image into 4D volume */
				imgData->imgDataCOMPLEX256->Set(Y,X,Z,index,0,value1);
				imgData->imgDataCOMPLEX256->Set(Y,X,Z,index,1,value2);
			}
			if ((imageType == IMG_4D) && (volumeType == VOL_4D)) { /* load 4D image into 4D volume (single file) */
				imgData->imgDataCOMPLEX256->Set(Y,X,Z,T,0,value1);
				imgData->imgDataCOMPLEX256->Set(Y,X,Z,T,1,value2);
			}

			X++;
			if (X >= XSize) { X = 0; Y++; }
			if (Y >= YSize) { Y = 0; Z++; }
			if (Z >= ZSize) { Z = 0; T++; }
			if (T >= TSize) break;
		}
		/* get the max and min values */
		if ((double)value1 > maxValue) maxValue = (double)value1;
		if ((double)value1 < minValue) minValue = (double)value1;
	}
	imgData->imgInfo->maxPixelValues.SetGray(maxValue);
	imgData->imgInfo->minPixelValues.SetGray(minValue);
	return true;
}


// --------------------------------------------------------
// ---------- LoadRGB24ToVolumeBuffer ---------------------
// --------------------------------------------------------
bool ImageDataReader::LoadRGB24ToVolumeBuffer(unsigned char *tmpBuffer, int imageDataSize, int index, int XSize, int YSize, int ZSize, int TSize, int swap)
{
	short byte1, byte2, byte3;
//	int i;
	int Xm, Ym, Zm, Tm; /* coordinates for the original mosaic image */
	int X, Y, Z, T; /* coordinates for the final 3D/4D image */
	int imageType = imgInfo->GetImageType();
	int volumeType = imgInfo->GetVolumeType();
//	int numCols = XSizeOrig/mosaicX;
	int numRows = YSizeOrig/mosaicY;
	int col, row;

	col = row = 0;
	X = Y = Z = T = 0;
	Xm = Ym = Zm = Tm = 0;
	swap = false;

	/* access data through [Y,X,Z,T,r/g/b] */
	for (int j=0;j<imageDataSize;j+=3) {
		/* each pixel is described by three bytes: R G B */
		byte1 = tmpBuffer[j];
		byte2 = tmpBuffer[j+1];
		byte3 = tmpBuffer[j+2];

		if (isMosaic) {
			imgData->imgDataRGB24->Set(Y,X,Z,index,0,byte1); /* mosaic will always load 3D image into 4D volume */
			imgData->imgDataRGB24->Set(Y,X,Z,index,1,byte2);
			imgData->imgDataRGB24->Set(Y,X,Z,index,2,byte3);

			Xm++; /* do all the nice transformations to get the mosaic coordinates into the final volume coordinates */
			if (Xm >= XSizeOrig) { Xm = 0; Ym++; } /* check if the original X is past the last column */
			if (Xm%mosaicX == 0) col = (Xm/mosaicX); /* check if were at a column boundary; if so, set the column */
			X = Xm%mosaicX; /* get the new volume X position */
			if (Ym >= YSizeOrig) { Ym = 0; } /* check if the original Y is past the last row (shouldn't happen) */
			if (Ym%mosaicY == 0) row = (Ym/mosaicY); /* check if were at a row boundary; if so, set the row */
			Y = Ym%mosaicY; /* get the new volume Y position */
			Z = (row*numRows) + col; /* find the new volume slice */
			if (Z >= mosaicZ) break; /* unlikely, but better have it in case something is wrong from before */
		}
		else {
			if ((imageType == IMG_2D) && (volumeType == VOL_2D)){ /* load 2D image into 2D volume (single file) */
				imgData->imgDataRGB24->Set(Y,X,0,0,0,byte1); /* R */
				imgData->imgDataRGB24->Set(Y,X,0,0,1,byte2); /* G */
				imgData->imgDataRGB24->Set(Y,X,0,0,2,byte3); /* B */
			}
			if ((imageType == IMG_2D) && (volumeType == VOL_3D)) { /* load 2D image into 3D volume */
				imgData->imgDataRGB24->Set(Y,X,index,0,0,byte1);
				imgData->imgDataRGB24->Set(Y,X,index,0,1,byte2);
				imgData->imgDataRGB24->Set(Y,X,index,0,2,byte3);
			}
			if ((imageType == IMG_3D) && (volumeType == VOL_3D)) { /* load 3D image into 3D volume (single file) */
				imgData->imgDataRGB24->Set(Y,X,Z,0,0,byte1);
				imgData->imgDataRGB24->Set(Y,X,Z,0,1,byte2);
				imgData->imgDataRGB24->Set(Y,X,Z,0,2,byte3);
			}
			if ((imageType == IMG_3D) && (volumeType == VOL_4D)) { /* load 3D image into 4D volume */
				imgData->imgDataRGB24->Set(Y,X,Z,index,0,byte1);
				imgData->imgDataRGB24->Set(Y,X,Z,index,1,byte2);
				imgData->imgDataRGB24->Set(Y,X,Z,index,2,byte3);
			}
			if ((imageType == IMG_4D) && (volumeType == VOL_4D)) { /* load 4D image into 4D volume (single file) */
				imgData->imgDataRGB24->Set(Y,X,Z,T,0,byte1);
				imgData->imgDataRGB24->Set(Y,X,Z,T,1,byte2);
				imgData->imgDataRGB24->Set(Y,X,Z,T,2,byte3);
			}

			X++;
			if (X >= XSize) { X = 0; Y++; }
			if (Y >= YSize) { Y = 0; Z++; }
			if (Z >= ZSize) { Z = 0; T++; }
			if (T >= TSize) break;
		}
	}
	imgData->imgInfo->maxPixelValues.SetRGB(255.0,255.0,255.0);
	imgData->imgInfo->minPixelValues.SetRGB(0.0,0.0,0.0);
	imgData->imgInfo->maxPixelValues.SetGray(255.0);
	imgData->imgInfo->minPixelValues.SetGray(0.0);
	return true;
}
