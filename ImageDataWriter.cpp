/* ----------------------------------------------------------------------------
	ImageDataWriter.cpp


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
#include "gdcmAttribute.h"
#include "gdcmFile.h"
#include "gdcmImage.h"
#include "gdcmImageReader.h"
#include "gdcmImageWriter.h"
#include "gdcmByteValue.h"
#include "gdcmFileDerivation.h"
#include "gdcmUIDGenerator.h"
#include "wx/dir.h"
#include "wx/File.h"
#include "wx/progdlg.h"
#include "wx/filename.h"
#include "wx/image.h"
#include <iostream>
#include "math.h"
#include "stdio.h"
#include "float.h"
#include <stdio.h>
#include <stdlib.h>
#include "ImageDataWriter.h"
#include "fslio/fslio.h"


ImageDataWriter::ImageDataWriter(ImageData *imgdata, ImageInfo *imageInfo, DicomInfo *dicomInfo, NiftiInfo *niftiInfo)
{
	imgData = imgdata;

	imgInfo = imageInfo;
	dcmInfo = dicomInfo;
	nftInfo = niftiInfo;
}

ImageDataWriter::~ImageDataWriter()
{
}


// --------------------------------------------------------
// ---------- SetFileParameters ---------------------------
// --------------------------------------------------------
void ImageDataWriter::SetFileParameters(int outType, int convertType, wxFileName dir, wxString fname, int serNum)
{

	filename = fname;
	outputType = outType;
	outConvertType = convertType;
	outDir = dir;
	outFilename = fname;
	seriesNum = serNum;
}


// --------------------------------------------------------
// ---------- SetFileParameters ---------------------------
// --------------------------------------------------------
bool ImageDataWriter::Write()
{
	bool ret = 1;

	switch (outputType) {
		case OUTPUT_FT_DICOM:
			if (outConvertType == CONVERT_1_TO_N)
				ret = WriteMultiFileDicomImage();
			else
				ret = WriteSingleDicomImage();
			break;
		case OUTPUT_FT_ANALYZE: /* these four options fall through to the same function */
		case OUTPUT_FT_NIFTI1FILE:
		case OUTPUT_FT_NIFTI2FILE:
		case OUTPUT_FT_NIFTIASCII:
			ret = SaveNiftiImage();
			break;
		case OUTPUT_FT_JPEG: /* these four options also fall through to the same function */
		case OUTPUT_FT_PNG:
		case OUTPUT_FT_TIFF:
		case OUTPUT_FT_BMP:
			ret = SaveRasterImage();
			break;
		default:
			break;
	}

	return ret;
}


// --------------------------------------------------------
// ---------- WriteSingleDicomImage -----------------------
// --------------------------------------------------------
bool ImageDataWriter::WriteSingleDicomImage()
{

	wxString fullFilePath;

	fullFilePath = outDir.GetPathWithSep() + filename;

	gdcm::Trace::DebugOn();
	gdcm::Trace::WarningOn();
	gdcm::Trace::ErrorOn();

	WriteLog("Inside SaveDicomImage()...\n");

	/* create the image */
	gdcm::SmartPointer<gdcm::Image> im = new gdcm::Image;

	/* setup the pixeltype and monoschrome/RGB type */
	gdcm::PixelFormat pixType;
	gdcm::PhotometricInterpretation photoInterp;
	void *imgBufferPtr;
	switch (imgData->imgInfo->GetPixelType()) {
		case IMG_UINT8:
			pixType = gdcm::PixelFormat::UINT8;
			photoInterp = gdcm::PhotometricInterpretation::MONOCHROME2;
			imgBufferPtr = imgData->imgDataUINT8->GetDataPointer();
			break;
		case IMG_INT8:
			pixType = gdcm::PixelFormat::INT8;
			photoInterp = gdcm::PhotometricInterpretation::MONOCHROME2;
			imgBufferPtr = imgData->imgDataINT8->GetDataPointer();
			break;
		case IMG_UINT16:
			pixType = gdcm::PixelFormat::UINT16;
			photoInterp = gdcm::PhotometricInterpretation::MONOCHROME2;
			imgBufferPtr = imgData->imgDataUINT16->GetDataPointer();
			break;
		case IMG_INT16:
			pixType = gdcm::PixelFormat::INT16;
			photoInterp = gdcm::PhotometricInterpretation::MONOCHROME2;
			imgBufferPtr = imgData->imgDataINT16->GetDataPointer();
			break;
		case IMG_UINT32:
			pixType = gdcm::PixelFormat::UINT32;
			photoInterp = gdcm::PhotometricInterpretation::MONOCHROME2;
			imgBufferPtr = imgData->imgDataUINT32->GetDataPointer();
			break;
		case IMG_INT32:
			pixType = gdcm::PixelFormat::INT32;
			photoInterp = gdcm::PhotometricInterpretation::MONOCHROME2;
			imgBufferPtr = imgData->imgDataINT32->GetDataPointer();
			break;
		case IMG_RGB24:
			pixType = gdcm::PixelFormat::UINT8;
			photoInterp = gdcm::PhotometricInterpretation::RGB;
			imgBufferPtr = imgData->imgDataRGB24->GetDataPointer();
			break;
	}

	/* setup image information */
	if (imgData->imgInfo->GetVolumeZSize() > 1)
		im->SetNumberOfDimensions( 3 );
	else
		im->SetNumberOfDimensions( 2 );

	im->SetDimension(0, imgData->imgInfo->GetVolumeXSize());
	im->SetDimension(1, imgData->imgInfo->GetVolumeYSize());
	if (imgData->imgInfo->GetVolumeZSize() > 1)
		im->SetDimension(2, imgData->imgInfo->GetVolumeZSize());

	im->GetPixelFormat().SetScalarType(pixType);
	im->GetPixelFormat().SetSamplesPerPixel(imgData->imgInfo->GetSamplesPerPixel()); /* NOTE: this call must come AFTER SetScalarType() */
	im->SetPhotometricInterpretation( photoInterp );

	WriteLog("Created an image...");

	/* set the image data */
	unsigned long l = im->GetBufferLength();
	gdcm::DataElement pixeldata( gdcm::Tag(0x7fe0,0x0010) );
	pixeldata.SetByteValue( (const char *)imgBufferPtr, l );
	im->SetDataElement( pixeldata );

	/* create the DICOM file. It's blank at first, so we'll need to update the important dicom tags */
	gdcm::SmartPointer<gdcm::File> file = new gdcm::File; // empty file

	/* again... the Attribute is set via template, so theres no way to set the tags programmatically at run-time */
	gdcm::Attribute<0x0010,0x0010> patientname;
	patientname.SetValue("Patient Name");
	file->GetDataSet().Insert(patientname.GetAsDataElement());

	/* write the image */
	gdcm::ImageWriter w;
	w.SetImage( *im );
	w.SetFile( *file );
	w.SetFileName(fullFilePath);
	if( !w.Write() ) {
		WriteLog(fullFilePath + " not written\n");
		return false;
	}
	else {
		WriteLog(fullFilePath + " written!\n");
		return true;
	}

}


// --------------------------------------------------------
// ---------- WriteMultiFileDicomImage---------------------
// --------------------------------------------------------
bool ImageDataWriter::WriteMultiFileDicomImage()
{
	WriteLog("Inside SaveDicomImage()...\n");
	gdcm::Trace::DebugOn();
	gdcm::Trace::WarningOn();
	gdcm::Trace::ErrorOn();

	bool ret = false;
	int xSize = imgData->imgInfo->GetVolumeXSize();
	int ySize = imgData->imgInfo->GetVolumeYSize();
	int zSize = imgData->imgInfo->GetVolumeZSize();
	int index = 0;
	int row, col;
	unsigned long l;
	wxString fullFilePath;
	UINT_8 *bufferU8;
	INT_8 *buffer8;
	UINT_16 *bufferU16;
	INT_16 *buffer16;
	UINT_32 *bufferU32;
	INT_32 *buffer32;
	UINT_8 *bufferRGB;
	wxString msg;
	void *imgBufferPtr;
	gdcm::PixelFormat pixType;
	gdcm::PhotometricInterpretation photoInterp;

	wxDateTime now = wxDateTime::Now();

	/* setup the progress dialog */
	wxProgressDialog progDlg(wxT("Writing DICOM Files"), wxT(" "), (int)zSize, NULL, wxPD_APP_MODAL | wxPD_SMOOTH | wxPD_ELAPSED_TIME | wxPD_CAN_ABORT);

	/* unfortunately... gdcm::Attribute is only set via template, so theres no way to set the tags programmatically at run-time */
	gdcm::Attribute<0x0008,0x0005> specCharSet;
	gdcm::Attribute<0x0008,0x0008> imageType;
	gdcm::Attribute<0x0008,0x0020> studyDate;
	gdcm::Attribute<0x0008,0x0021> seriesDate;
	gdcm::Attribute<0x0008,0x0030> studyTime;
	gdcm::Attribute<0x0008,0x0031> seriesTime;
	gdcm::Attribute<0x0008,0x0060> modality;
	gdcm::Attribute<0x0008,0x0080> institutionName;
	gdcm::Attribute<0x0008,0x1030> studyDesc;
	gdcm::Attribute<0x0008,0x103E> seriesDesc;
	gdcm::Attribute<0x0010,0x0010> patientName;
	gdcm::Attribute<0x0010,0x0020> patientID;
	gdcm::Attribute<0x0010,0x0030> patientDOB;
	gdcm::Attribute<0x0010,0x0040> patientSex;
	gdcm::Attribute<0x0018,0x0050> sliceThickness;
	gdcm::Attribute<0x0018,0x5100> patientPosition;
	gdcm::Attribute<0x0018,0x0023> MR_AcqType;
	gdcm::Attribute<0x0018,0x0080> MR_TR;
	gdcm::Attribute<0x0018,0x0081> MR_TE;
	gdcm::Attribute<0x0018,0x1314> MR_Flip;
	gdcm::Attribute<0x0020,0x0010> studyID;
	gdcm::Attribute<0x0020,0x0011> seriesNumber;
	gdcm::Attribute<0x0020,0x0011> acquisitionNumber;
	gdcm::Attribute<0x0020,0x0037> imageOrientPat;
	gdcm::Attribute<0x0020,0x0052> frameOfReferenceUID;
	gdcm::Attribute<0x0020,0x1002> imagesInAcq;
	gdcm::Attribute<0x0028,0x0030> pixelSpacing;

	/* fill in all of the above attributes */
	//specCharSet.SetValue("ISO_IR 100");
	static const gdcm::CSComp imageTypeValues[] = {"DERIVED","SECONDARY"};
	imageType.SetValues(imageTypeValues,2);
	studyDesc.SetValue("Converted by MIView");
	acquisitionNumber.SetValue(1);
	imagesInAcq.SetValue(zSize);
	frameOfReferenceUID.SetValue("1.3.12.2.1107.5.2.7.20107.20110503102228796.0.0.0");
	MR_AcqType.SetValue("3D");
	MR_TR.SetValue(0);
	MR_TE.SetValue(0);
	MR_Flip.SetValue(0);

	if (dcmInfo->GetModality() == "")
		modality.SetValue("OT");
	else
		modality.SetValue(dcmInfo->GetModality().c_str());

	/* patient name from conversion dialog */
	if (dcmInfo->GetPatientName() == "")
		patientName.SetValue("Patient^Name");
	else
		patientName.SetValue(dcmInfo->GetPatientName().c_str());

	/* patient ID from conversion dialog */
	if (dcmInfo->GetPatientID() == "")
		patientID.SetValue("Patient^ID");
	else
		patientID.SetValue(dcmInfo->GetPatientID().c_str());

	institutionName.SetValue("Converted by MIView");
	studyID.SetValue("1");

	/* patient dob from conversion dialog */
	if (dcmInfo->GetPatientBirthdate() == "")
		patientDOB.SetValue(now.Format("%Y%m%d").c_str());
	else
		patientDOB.SetValue(dcmInfo->GetPatientBirthdate().c_str());

	/* set patient gender unknown */
	if (dcmInfo->GetPatientSex() == "")
		patientSex.SetValue("U");
	else
		patientSex.SetValue(dcmInfo->GetPatientSex().c_str());

	/* set patient gender unknown */
	if (dcmInfo->GetPatientPositionStr() == "")
		patientPosition.SetValue("HFS");
	else
		patientPosition.SetValue(dcmInfo->GetPatientPositionStr().c_str());

	/* series number obtained from file index */
	seriesNumber.SetValue(seriesNum);

	/* series description from the original filename */
	wxFileName fn = wxFileName(imgData->GetFilenames().Item(0));
	seriesDesc.SetValue(fn.GetName().c_str());

	/* study/series date set to current date/time, if not already specified */
	if (dcmInfo->GetStudyDate() == "")
		studyDate.SetValue(now.Format("%Y%m%d").c_str());
	else
		studyDate.SetValue(dcmInfo->GetStudyDate().c_str());
	
	if (dcmInfo->GetSeriesDate() == "")
		seriesDate.SetValue(now.Format("%Y%m%d").c_str());
	else
		seriesDate.SetValue(dcmInfo->GetSeriesDate().c_str());

	if (dcmInfo->GetStudyTime() == "")
		studyTime.SetValue(now.Format("%H%M%S").c_str());
	else
		studyTime.SetValue(dcmInfo->GetStudyTime().c_str());

	if (dcmInfo->GetSeriesTime() == "")
		seriesTime.SetValue(now.Format("%H%M%S").c_str());
	else
		seriesTime.SetValue(dcmInfo->GetSeriesTime().c_str());

	/* setup pixel spacing and slice thickness */
	sliceThickness.SetValue(imgInfo->GetSpacing().z);
	double *arr = new double[2];
	arr[0] = imgInfo->GetSpacing().x;
	arr[1] = imgInfo->GetSpacing().y;

	pixelSpacing.SetValues(arr);
	delete[] arr;

	/* default image orientation patient (direction cosines) */
	double *arr2 = new double[6];
	arr2[0] = 1;
	arr2[1] = 0;
	arr2[2] = 0;
	arr2[3] = 0;
	arr2[4] = 1;
	arr2[5] = 0;
	imageOrientPat.SetValues(arr2);

	/* make a directory for this set of files */
	outDir.AppendDir(wxString::Format("%d",seriesNum));
	if (!outDir.DirExists())
		outDir.Mkdir();
	/* write each Z slice as a file */
	for (int z=0;z<zSize;z++) {
		fullFilePath = outDir.GetPathWithSep() + wxString::Format("%05d.dcm",z);

		/* for z indexing */
		gdcm::Attribute<0x0020,0x0013> instanceNum; instanceNum.SetValue(z+1);
		gdcm::Attribute<0x0020,0x1041> sliceLoc; sliceLoc.SetValue(z+1);
		/* for UIDs */
		gdcm::Attribute<0x0020,0x000D> studyUID;
		gdcm::Attribute<0x0020,0x000E> seriesUID;
		
		/* make remaining series have the same series UID */
		if (z>0) {
			studyUID.SetValue(studyUIDValue.c_str());
			seriesUID.SetValue(seriesUIDValue.c_str());
		}

		WriteLog(msg.Format("Writing dicom image %d of %d: %s",z,zSize, fullFilePath.c_str()));

		/* update the progress dialog */
		if (!progDlg.Update(z,msg.Format("Writing file %d of %d",z,zSize))) {
			WriteLog("User cancelled SaveDicom operation");
			return false;
		}

		/* create the image */
		gdcm::SmartPointer<gdcm::Image> im = new gdcm::Image;

		/* setup the pixeltype and monoschrome/RGB type */
		index = 0;
		switch (imgData->imgInfo->GetPixelType()) {
			case IMG_UINT8:
				pixType = gdcm::PixelFormat::UINT8;
				photoInterp = gdcm::PhotometricInterpretation::MONOCHROME2;

				/* fill the buffer to be passed to the gdcm::Writer */
				bufferU8 = new UINT_8[xSize*ySize];
				for (col=0; col < ySize; col++)
					for (row=0; row < xSize; row++)
						bufferU8[index++] = imgData->imgDataUINT8->Get(col,row,z,0,0);
				imgBufferPtr = bufferU8;
				break;
			case IMG_INT8:
				pixType = gdcm::PixelFormat::INT8;
				photoInterp = gdcm::PhotometricInterpretation::MONOCHROME2;

				/* fill the buffer to be passed to the gdcm::Writer */
				buffer8 = new INT_8[xSize*ySize];
				for (col=0; col < ySize; col++)
					for (row=0; row < xSize; row++)
						buffer8[index++] = imgData->imgDataINT8->Get(col,row,z,0,0);
				imgBufferPtr = buffer8;
				break;
			case IMG_UINT16:
				pixType = gdcm::PixelFormat::UINT16;
				photoInterp = gdcm::PhotometricInterpretation::MONOCHROME2;

				/* fill the buffer to be passed to the gdcm::Writer */
				bufferU16 = new UINT_16[xSize*ySize];
				for (col=0; col < ySize; col++)
					for (row=0; row < xSize; row++)
						bufferU16[index++] = imgData->imgDataUINT16->Get(col,row,z,0,0);
				imgBufferPtr = bufferU16;
				break;
			case IMG_INT16:
				pixType = gdcm::PixelFormat::INT16;
				photoInterp = gdcm::PhotometricInterpretation::MONOCHROME2;

				/* fill the buffer to be passed to the gdcm::Writer */
				buffer16 = new INT_16[xSize*ySize];
				for (col=0; col < ySize; col++)
					for (row=0; row < xSize; row++)
						buffer16[index++] = imgData->imgDataINT16->Get(col,row,z,0,0);
				imgBufferPtr = buffer16;
				break;
			case IMG_UINT32:
				pixType = gdcm::PixelFormat::UINT32;
				photoInterp = gdcm::PhotometricInterpretation::MONOCHROME2;

				/* fill the buffer to be passed to the gdcm::Writer */
				bufferU32 = new UINT_32[xSize*ySize];
				for (col=0; col < ySize; col++)
					for (row=0; row < xSize; row++)
						bufferU32[index++] = imgData->imgDataUINT32->Get(col,row,z,0,0);
				imgBufferPtr = bufferU32;
				break;
			case IMG_INT32:
				pixType = gdcm::PixelFormat::INT32;
				photoInterp = gdcm::PhotometricInterpretation::MONOCHROME2;

				/* fill the buffer to be passed to the gdcm::Writer */
				buffer32 = new INT_32[xSize*ySize];
				for (col=0; col < ySize; col++)
					for (row=0; row < xSize; row++)
						buffer32[index++] = imgData->imgDataINT32->Get(col,row,z,0,0);
				imgBufferPtr = buffer32;
				break;
			case IMG_RGB24:
				pixType = gdcm::PixelFormat::UINT8;
				photoInterp = gdcm::PhotometricInterpretation::RGB;

				/* fill the buffer to be passed to the gdcm::Writer */
				bufferRGB = new UINT_8[xSize*ySize*3];
				for (col=0; col < xSize; col++)
					for (row=0; row < ySize; row++) {
						bufferRGB[index++] = imgData->imgDataRGB24->Get(col,row,z,0,0);
						bufferRGB[index++] = imgData->imgDataRGB24->Get(col,row,z,0,1);
						bufferRGB[index++] = imgData->imgDataRGB24->Get(col,row,z,0,2);
					}
				imgBufferPtr = bufferRGB;
				break;
		}

		/* setup image information */
		im->SetNumberOfDimensions( 2 );
		im->SetDimension(0, imgData->imgInfo->GetVolumeXSize());
		im->SetDimension(1, imgData->imgInfo->GetVolumeYSize());
		im->GetPixelFormat().SetScalarType(pixType);
		im->GetPixelFormat().SetSamplesPerPixel(imgData->imgInfo->GetSamplesPerPixel()); /* NOTE: this call must come AFTER SetScalarType() */
		im->SetPhotometricInterpretation( photoInterp );
		im->SetDirectionCosines(arr2);
		im->SetSpacing(0,imgInfo->GetSpacing().x);
		im->SetSpacing(1,imgInfo->GetSpacing().y);
		im->SetSpacing(2,imgInfo->GetSpacing().z);
		im->SetOrigin(0,0);
		im->SetOrigin(1,0);
		im->SetOrigin(2,z+1);

		/* set the image data */
		l = im->GetBufferLength();
		WriteLog(msg.Format("Image size...(%dx%d) [%d,%d] bytes",xSize,ySize,xSize*ySize*2,l));
		gdcm::DataElement pixeldata( gdcm::Tag(0x7fe0,0x0010) );
		pixeldata.SetByteValue( (const char *)imgBufferPtr, l );
		im->SetDataElement( pixeldata );

		/* delete the tmp buffers after the data's been copied to the gdcm Image */
		switch (imgData->imgInfo->GetPixelType()) {
			case IMG_UINT8: delete[] bufferU8; break;
			case IMG_INT8: delete[] buffer8; break;
			case IMG_UINT16: delete[] bufferU16; break;
			case IMG_INT16: delete[] buffer16; break;
			case IMG_UINT32: delete[] bufferU32; break;
			case IMG_INT32: delete[] bufferU32; break;
			case IMG_RGB24: delete[] bufferRGB; break;
		}

		WriteLog(msg.Format("Created an image...(%dx%d) %d bytes",xSize,ySize,l));

		/* create the DICOM file. It's blank at first, so we'll need to update the important dicom tags */
		gdcm::SmartPointer<gdcm::File> file = new gdcm::File; // empty file

		/* set the important DICOM tags */
		file->GetDataSet().Insert(patientName.GetAsDataElement());
		file->GetDataSet().Insert(patientID.GetAsDataElement());
		file->GetDataSet().Insert(patientDOB.GetAsDataElement());
		file->GetDataSet().Insert(patientSex.GetAsDataElement());
		file->GetDataSet().Insert(studyDate.GetAsDataElement());
		file->GetDataSet().Insert(seriesDate.GetAsDataElement());
		file->GetDataSet().Insert(studyTime.GetAsDataElement());
		file->GetDataSet().Insert(seriesTime.GetAsDataElement());
		file->GetDataSet().Insert(seriesDesc.GetAsDataElement());
		file->GetDataSet().Insert(seriesNumber.GetAsDataElement());
		file->GetDataSet().Insert(sliceThickness.GetAsDataElement());
		file->GetDataSet().Insert(pixelSpacing.GetAsDataElement());
		file->GetDataSet().Insert(instanceNum.GetAsDataElement());
		file->GetDataSet().Insert(sliceLoc.GetAsDataElement());
		file->GetDataSet().Insert(imageOrientPat.GetAsDataElement());
		file->GetDataSet().Insert(modality.GetAsDataElement());
		file->GetDataSet().Insert(patientPosition.GetAsDataElement());
		file->GetDataSet().Insert(institutionName.GetAsDataElement());
		file->GetDataSet().Insert(studyID.GetAsDataElement());
		file->GetDataSet().Insert(imageType.GetAsDataElement());
		file->GetDataSet().Insert(studyDesc.GetAsDataElement());
		file->GetDataSet().Insert(acquisitionNumber.GetAsDataElement());
		file->GetDataSet().Insert(imagesInAcq.GetAsDataElement());
		file->GetDataSet().Insert(frameOfReferenceUID.GetAsDataElement());
		file->GetDataSet().Insert(MR_AcqType.GetAsDataElement());
		file->GetDataSet().Insert(MR_TR.GetAsDataElement());
		file->GetDataSet().Insert(MR_TE.GetAsDataElement());
		file->GetDataSet().Insert(MR_Flip.GetAsDataElement());
		if (z>0) {
			file->GetDataSet().Insert(studyUID.GetAsDataElement());
			file->GetDataSet().Insert(seriesUID.GetAsDataElement());
		}

		/* write the image */
		gdcm::ImageWriter w;
		w.SetImage( *im );
		w.SetFile( *file );
		w.SetFileName(fullFilePath);
		if( !w.Write() ) {
			WriteLog(fullFilePath + " not written\n");
			ret = false;
		}
		else {
			WriteLog(fullFilePath + " written!\n");
			ret = true;
		}

		/* read the series UID, and set it for use in the remaining files */
		if (z==0) {
			gdcm::ImageReader reader;
			gdcm::DataSet dataSet;
			gdcm::DataElement dataElement;
			gdcm::SmartPointer<gdcm::Image> imagePtr;
			gdcm::SmartPointer<gdcm::File> filePtr;
			/* open it, get the image number */
			gdcm::ImageReader r;
			r.SetFileName(fullFilePath);

			if (!r.Read()) {
				//SetError(msg.Format("Dicomfile '%s' not readable - ImageDataReader->LoadDicomFilelist()",filepath.c_str()));
				return 0;
			}
			imagePtr = const_cast<gdcm::Image*>(&r.GetImage()); // very ugly
			filePtr = const_cast<gdcm::File*>(&r.GetFile()); // very ugly, but it works :)

			dataSet = filePtr->GetDataSet();

			gdcm::Attribute<0x0020,0x000D> at1;
			if( dataSet.FindDataElement( at1.GetTag() )) {
				const gdcm::DataElement &de = dataSet.GetDataElement( at1.GetTag() );
				at1.SetFromDataElement( de );
				studyUIDValue = at1.GetValue().c_str();
			}
			gdcm::Attribute<0x0020,0x000E> at2;
			if( dataSet.FindDataElement( at2.GetTag() )) {
				const gdcm::DataElement &de = dataSet.GetDataElement( at2.GetTag() );
				at2.SetFromDataElement( de );
				seriesUIDValue = at2.GetValue().c_str();
			}
		}
	}
	delete[] arr2;

	return ret;
}


// --------------------------------------------------------
// ---------- SaveNiftiImage ------------------------------
// --------------------------------------------------------
bool ImageDataWriter::SaveNiftiImage()
{
//	nifti_image *img;
	//wxString filenameHDR;
	//wxString filenameIMG;
	wxFileName theFilename;
	wxString fileString;
	wxString description = "Converted by MIView";
	int bufferSize;
	int i, row, col, slice, vol;
	int xSize, ySize, zSize, tSize;
	double maxVal;
	double maxByteBoundary;
	double voxelValue;
	int nbyper;
	int swapsize;
	unsigned int NumVoxels;

	xSize = imgInfo->GetVolumeXSize();
	ySize = imgInfo->GetVolumeYSize();
	zSize = imgInfo->GetVolumeZSize();
	tSize = imgInfo->GetVolumeTSize();
	bufferSize = xSize*ySize*zSize*tSize;

	/* setup the displaying of the colors, normalized to the gl byte boundaries */
	maxByteBoundary = pow(2.0, (double)imgData->imgInfo->GetBitsAllocated()); /* get the maximum byte boundary value */

	/* setup the filename: nifti_image_write() is expecting a blank extension */
	theFilename.Assign(filename);
	fileString = outDir.GetPathWithSep() + theFilename.GetName();

	WriteLog(wxString("Writing Nifti file: " + fileString));

	/* setup the Nifti1 header */
	nifti_1_header reference_header;
	memset(&reference_header,0,sizeof(reference_header));
	reference_header.sizeof_hdr=sizeof(reference_header);
	reference_header.regular='r';
	reference_header.extents=16384;
	if (imgInfo->GetVolumeType() == VOL_2D) reference_header.dim[0]=2; /* number of dimensions */
	if (imgInfo->GetVolumeType() == VOL_3D) reference_header.dim[0]=3; /* number of dimensions */
	if (imgInfo->GetVolumeType() == VOL_4D) reference_header.dim[0]=4; /* number of dimensions */
	reference_header.dim[1]=xSize;
	reference_header.dim[2]=ySize;
	reference_header.dim[3]=zSize;
	reference_header.dim[4]=tSize;
	reference_header.dim[5]=1;
	reference_header.dim[6]=1; //This MUST be 1 anything else is invalid due to code that usees huristics to fix other possible problems;
	reference_header.dim[7]=1; //This MUST be 1 anything else is invalid due to code that usees huristics to fix other possible problems;
	reference_header.intent_p1=10101010.101F;
	reference_header.intent_p2=987654321.0F;
	reference_header.intent_p3=-1234.0F;
	reference_header.intent_code=NIFTI_INTENT_ESTIMATE;
	/* set the data type */
	switch (imgInfo->GetPixelType()) {
		case IMG_UNKNOWN: reference_header.datatype=DT_UNKNOWN; maxVal = 1.0; break;
		case IMG_BINARY: reference_header.datatype=DT_BINARY; maxVal = 1.0; break;
		case IMG_UINT8: reference_header.datatype=DT_UINT8; maxVal = pow(2.0, 8.0) - 1.0; break;
		case IMG_INT8: reference_header.datatype=DT_INT8; maxVal = pow(2.0, 7.0) - 1.0; break;
		case IMG_UINT16: reference_header.datatype=DT_UINT16; maxVal = pow(2.0, 16.0) - 1.0; break;
		case IMG_INT16: reference_header.datatype=DT_INT16; maxVal = pow(2.0, 15.0) - 1.0; break;
		case IMG_UINT32: reference_header.datatype=DT_UINT32; maxVal = pow(2.0, 32.0) - 1.0; break;
		case IMG_INT32: reference_header.datatype=DT_INT32; maxVal = pow(2.0, 31.0) - 1.0; break;
		case IMG_UINT64: reference_header.datatype=DT_UINT64; maxVal = pow(2.0, 64.0) - 1.0; break;
		case IMG_INT64: reference_header.datatype=DT_INT64; maxVal = pow(2.0, 63.0) - 1.0; break;
		case IMG_FLOAT32: reference_header.datatype=DT_FLOAT32; maxVal = FLT_MAX; break;
		case IMG_FLOAT64: reference_header.datatype=DT_FLOAT64; maxVal = DBL_MAX; break;
		case IMG_FLOAT128: reference_header.datatype=DT_FLOAT128; maxVal = LDBL_MAX; break;
		case IMG_COMPLEX64: reference_header.datatype=DT_COMPLEX64; maxVal = FLT_MAX; break;
		case IMG_COMPLEX128: reference_header.datatype=DT_COMPLEX128; maxVal = DBL_MAX; break;
		case IMG_COMPLEX256: reference_header.datatype=DT_COMPLEX256; maxVal = LDBL_MAX; break;
		case IMG_RGB24: reference_header.datatype=DT_RGB24; maxVal = 255; break;
		default: reference_header.datatype=DT_UNKNOWN; maxVal = 1.0; break;
	}
	/* continue with the header */
	reference_header.pixdim[0]=-1.0F; /* this is really qfac */
	reference_header.pixdim[1]=imgInfo->GetSpacing().x;		/* the spacings in mm between pixels */
	reference_header.pixdim[2]=imgInfo->GetSpacing().y;
	reference_header.pixdim[3]=imgInfo->GetSpacing().z;
	reference_header.pixdim[4]=2.0F;
	reference_header.pixdim[5]=4.0F;
	reference_header.pixdim[6]=-2.0e10F;
	reference_header.pixdim[7]=-2.0e10F;
	reference_header.vox_offset=0;
	reference_header.scl_slope=1.0;
	reference_header.scl_inter=0;
	reference_header.qform_code=NIFTI_XFORM_SCANNER_ANAT;
	reference_header.quatern_b=-0.5F;
	reference_header.quatern_c= 0.5F;
	reference_header.quatern_d=-0.5F;
	reference_header.qoffset_x=reference_header.dim[1]/2.0F;
	reference_header.qoffset_y=reference_header.dim[2]/2.0F;
	reference_header.qoffset_z=reference_header.dim[3]/2.0F;
	reference_header.sform_code=NIFTI_XFORM_SCANNER_ANAT;
	reference_header.srow_x[0]=0.5;
	reference_header.srow_x[1]=0.0;
	reference_header.srow_x[2]=0.0;
	reference_header.srow_x[3]=0.0;
	reference_header.srow_y[0]=0.0;
	reference_header.srow_y[1]=1.0;
	reference_header.srow_y[2]=0.0;
	reference_header.srow_y[3]=0.0;
	reference_header.srow_z[0]=0.0;
	reference_header.srow_z[1]=0.0;
	reference_header.srow_z[2]=2.0;
	reference_header.srow_z[3]=0.0;
	reference_header.magic[0]='n';		/* magic = 'ni1' for Analyze, 'n+1' for .nii files */
	if ((outputType == OUTPUT_FT_ANALYZE) || (outputType == OUTPUT_FT_NIFTI2FILE)) reference_header.magic[1]='i';
	if ((outputType == OUTPUT_FT_NIFTI1FILE) || (outputType == OUTPUT_FT_NIFTIASCII)) reference_header.magic[1]='+';
	reference_header.magic[2]='1';
	reference_header.magic[3]='\0';
	reference_header.glmax = (int)maxVal;
//	reference_header.glmin = (int)minVal;
	strncpy(reference_header.intent_name,"Coverted by MIView",16);
	strncpy(reference_header.descrip,"Coverted by MIView",80);

	/* create the nifti_image */
	nifti_datatype_sizes(reference_header.datatype ,&nbyper,&swapsize);
	reference_header.bitpix=nbyper*8;
	nifti_image * reference_image=nifti_convert_nhdr2nim(reference_header,fileString.c_str());
	switch (outputType) {
		case OUTPUT_FT_ANALYZE: reference_image->nifti_type = NIFTI_FTYPE_ANALYZE; break;
		case OUTPUT_FT_NIFTI1FILE: reference_image->nifti_type = NIFTI_FTYPE_NIFTI1_1; break;
		case OUTPUT_FT_NIFTI2FILE: reference_image->nifti_type = NIFTI_FTYPE_NIFTI1_2; break;
		case OUTPUT_FT_NIFTIASCII: reference_image->nifti_type = NIFTI_FTYPE_ASCII; break;
	}

	/* allocate space for the image->data */
	NumVoxels = reference_image->nx * reference_image->ny * reference_image->nz * reference_image->nt * reference_image->nu;
	reference_image->data=(signed int *)calloc(NumVoxels,sizeof(signed int)) ; /*!< pointer to data: nbyper*nvox bytes     */
	
	i=0;
	/* create a buffer and load the data into it */
	switch (imgInfo->GetPixelType()) {
		case IMG_BINARY: break; /* not implemented yet */
		case IMG_UINT8:
			reference_image->data = (unsigned char*)calloc(bufferSize,sizeof(unsigned char));
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = ((double)imgData->imgDataUINT8->Get(col,row,slice,vol,0)/maxByteBoundary)*maxVal;
				((unsigned char*)(reference_image->data))[i] = (unsigned char)voxelValue + 1; /* not sure why, but pixels converted by other programs are 1 more than by this one. so add 1 to compensate */
				i++;
			}
			break;
		case IMG_INT8:
			reference_image->data = (char*)calloc(bufferSize,sizeof(char));
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = ((double)imgData->imgDataINT8->Get(col,row,slice,vol,0)/(maxByteBoundary/2.0))*maxVal;
				((char*)(reference_image->data))[i] = (char)voxelValue + 1; /* not sure why, but pixels converted by other programs are 1 more than by this one. so add 1 to compensate */
				i++;
			}
			break;
		case IMG_UINT16:
			reference_image->data = (unsigned short*)calloc(bufferSize,sizeof(unsigned short));
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = ((double)imgData->imgDataUINT16->Get(col,row,slice,vol,0)/maxByteBoundary)*maxVal;
				((unsigned short*)(reference_image->data))[i] = (unsigned short)voxelValue + 1; /* not sure why, but pixels converted by other programs are 1 more than by this one. so add 1 to compensate */
				i++;
			}
			break;
		case IMG_INT16:
			reference_image->data = (short*)calloc(bufferSize,sizeof(short));
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = ((double)imgData->imgDataINT16->Get(col,row,slice,vol,0)/(maxByteBoundary/2.0))*maxVal;
				((short*)(reference_image->data))[i] = (short)voxelValue + 1; /* not sure why, but pixels converted by other programs are 1 more than by this one. so add 1 to compensate */
				i++;
			}
			break;
		case IMG_UINT32:
			reference_image->data = (unsigned int*)calloc(bufferSize,sizeof(unsigned int));
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = ((double)imgData->imgDataUINT32->Get(col,row,slice,vol,0)/maxByteBoundary)*maxVal;
				((unsigned int*)(reference_image->data))[i] = (unsigned int)voxelValue + 1; /* not sure why, but pixels converted by other programs are 1 more than by this one. so add 1 to compensate */
				i++;
			}
			break;
		case IMG_INT32:
			reference_image->data = (int*)calloc(bufferSize,sizeof(int));
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = ((double)imgData->imgDataINT32->Get(col,row,slice,vol,0)/(maxByteBoundary/2.0))*maxVal;
				((int*)(reference_image->data))[i] = (int)voxelValue + 1; /* not sure why, but pixels converted by other programs are 1 more than by this one. so add 1 to compensate */
				i++;
			}
			break;
		case IMG_UINT64:
			reference_image->data = (unsigned long int*)calloc(bufferSize,sizeof(unsigned long int));
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = ((double)imgData->imgDataUINT64->Get(col,row,slice,vol,0)/maxByteBoundary)*maxVal;
				((unsigned long int*)(reference_image->data))[i] = (unsigned long int)voxelValue + 1; /* not sure why, but pixels converted by other programs are 1 more than by this one. so add 1 to compensate */
				i++;
			}
			break;
		case IMG_INT64:
			reference_image->data = (long int*)calloc(bufferSize,sizeof(long int));
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = ((double)imgData->imgDataINT64->Get(col,row,slice,vol,0)/maxByteBoundary)*maxVal;
				((long int*)(reference_image->data))[i] = (long int)voxelValue + 1; /* not sure why, but pixels converted by other programs are 1 more than by this one. so add 1 to compensate */
				i++;
			}
			break;
		case IMG_FLOAT32:
			reference_image->data = (float*)calloc(bufferSize,sizeof(float));
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = ((double)imgData->imgDataFLOAT64->Get(col,row,slice,vol,0)/maxByteBoundary)*maxVal;
				((float*)(reference_image->data))[i] = (float)voxelValue + 1; /* not sure why, but pixels converted by other programs are 1 more than by this one. so add 1 to compensate */
				i++;
			}
			break;
		case IMG_FLOAT64:
			reference_image->data = (double*)calloc(bufferSize,sizeof(double));
			for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
				voxelValue = ((double)imgData->imgDataFLOAT128->Get(col,row,slice,vol,0)/maxByteBoundary)*maxVal;
				((double*)(reference_image->data))[i] = (double)voxelValue + 1; /* not sure why, but pixels converted by other programs are 1 more than by this one. so add 1 to compensate */
				i++;
			}
			break;
		case IMG_FLOAT128:
			//reference_image->data = (long double*)calloc(bufferSize,sizeof(long double));
			//for (vol=0;vol<tSize;vol++) for (slice=0; slice<zSize; slice++) for (col=0; col<ySize; col++) for (row=0; row<xSize; row++) { /* loop T,Z,Y,X */
			//	voxelValue = (imgData->imgDataFLOAT128->Get(col,row,slice,vol)/maxByteBoundary)*maxVal;
			//	((long double*)(reference_image->data))[i] = (long double)voxelValue + 1; /* not sure why, but pixels converted by other programs are 1 more than by this one. so add 1 to compensate */
			//	i++;
			//}
			//break;
		case IMG_COMPLEX64: /* also not implemented yet */
		case IMG_COMPLEX128:
		case IMG_COMPLEX256:
		case IMG_RGB24:
		default: break;
	}

	/* write the image(s) to disk and clean up after */
	nifti_image_write(reference_image);
	nifti_image_free(reference_image);

	return true;
}


// --------------------------------------------------------
// ---------- SaveRasterImage -----------------------------
// --------------------------------------------------------
bool ImageDataWriter::SaveRasterImage()
{
	unsigned char *buffer;
	int bufferSize;
	int i, row, col;
	int slice;
	int xSize, ySize;
	double voxelValue, voxelValueR, voxelValueG, voxelValueB;
	double maxVal, minVal, multiplier;
	double maxByteBoundary;
	wxImage *img;

	xSize = imgInfo->GetImageXSize();
	ySize = imgInfo->GetImageYSize();
	slice = 0;

	wxFileName theFilename;
	theFilename.Assign(filename);
	wxString fileString = outDir.GetPathWithSep() + theFilename.GetFullName();

	/* setup the displaying of the colors, normalized to the gl byte boundaries */
	maxByteBoundary = pow(2.0, (double)imgData->imgInfo->GetBitsAllocated()); /* get the maximum byte boundary value */

	/* get maximum pixel value and calculate the multiplier to make the maximum value
	   match the maximum possible value for n bytes */
	if (imgData->imgInfo->GetPixelType() == IMG_RGB24) { maxVal = 255.0; minVal = 0.0; }
	else { maxVal = imgData->imgInfo->GetMaxPixelValues().Gray(); minVal = imgData->imgInfo->GetMinPixelValues().Gray(); }

	multiplier = (255.0/(maxVal-minVal));
//	minVal = (minVal/255.0); /* normalize the min image value to [0 1] over the range of the display type (eg 16 bit [65536]) */

	/* create new buffer using malloc() <-- needed for the wxImage object */
	bufferSize = xSize*ySize*3;
	buffer = (unsigned char *)malloc(bufferSize);

	/* loop through the data */
	i=0;
	switch (imgInfo->GetPixelType()) {
		case IMG_UNKNOWN:
		case IMG_BINARY:
			for (col=0; col<ySize; col++)
				for (row=0; row<xSize; row++) {
					voxelValue = ((double)imgData->imgDataBINARY->Get(col,row,slice,0,0)-minVal)*multiplier;
					buffer[i] = (unsigned char)voxelValue; buffer[i+1] = (unsigned char)voxelValue; buffer[i+2] = (unsigned char)voxelValue;
					i+=3;
				}
			break;
		case IMG_UINT8:
			for (col=0; col<ySize; col++)
				for (row=0; row<xSize; row++) {
					voxelValue = ((double)imgData->imgDataUINT8->Get(col,row,slice,0,0)-minVal)*multiplier;
					buffer[i] = (unsigned char)voxelValue; buffer[i+1] = (unsigned char)voxelValue; buffer[i+2] = (unsigned char)voxelValue;
					i+=3;
				}
			break;
		case IMG_INT8:
			for (col=0; col<ySize; col++)
				for (row=0; row<xSize; row++) {
					voxelValue = ((double)imgData->imgDataINT8->Get(col,row,slice,0,0)-minVal)*multiplier;
					buffer[i] = (unsigned char)voxelValue; buffer[i+1] = (unsigned char)voxelValue; buffer[i+2] = (unsigned char)voxelValue;
					i+=3;
				}
			break;
		case IMG_UINT16:
			for (col=0; col<ySize; col++)
				for (row=0; row<xSize; row++) {
					voxelValue = ((double)imgData->imgDataUINT16->Get(col,row,slice,0,0)-minVal)*multiplier;
					buffer[i] = (unsigned char)voxelValue; buffer[i+1] = (unsigned char)voxelValue; buffer[i+2] = (unsigned char)voxelValue;
					i+=3;
				}
			break;
		case IMG_INT16:
			for (col=0; col<ySize; col++)
				for (row=0; row<xSize; row++) {
					voxelValue = ((double)imgData->imgDataINT16->Get(col,row,slice,0,0)-minVal)*multiplier;
					buffer[i] = (unsigned char)voxelValue; buffer[i+1] = (unsigned char)voxelValue; buffer[i+2] = (unsigned char)voxelValue;
					i+=3;
				}
			break;
		case IMG_UINT32:
			for (col=0; col<ySize; col++)
				for (row=0; row<xSize; row++) {
					voxelValue = ((double)imgData->imgDataUINT32->Get(col,row,slice,0,0)-minVal)*multiplier;
					buffer[i] = (unsigned char)voxelValue; buffer[i+1] = (unsigned char)voxelValue; buffer[i+2] = (unsigned char)voxelValue;
					i+=3;
				}
			break;
		case IMG_INT32:
			for (col=0; col<ySize; col++)
				for (row=0; row<xSize; row++) {
					voxelValue = ((double)imgData->imgDataINT32->Get(col,row,slice,0,0)-minVal)*multiplier;
					buffer[i] = (unsigned char)voxelValue; buffer[i+1] = (unsigned char)voxelValue; buffer[i+2] = (unsigned char)voxelValue;
					i+=3;
				}
			break;
		case IMG_UINT64:
			for (col=0; col<ySize; col++)
				for (row=0; row<xSize; row++) {
					voxelValue = ((double)imgData->imgDataUINT64->Get(col,row,slice,0,0)-minVal)*multiplier;
					buffer[i] = (unsigned char)voxelValue; buffer[i+1] = (unsigned char)voxelValue; buffer[i+2] = (unsigned char)voxelValue;
					i+=3;
				}
			break;
		case IMG_INT64:
			for (col=0; col<ySize; col++)
				for (row=0; row<xSize; row++) {
					voxelValue = ((double)imgData->imgDataINT64->Get(col,row,slice,0,0)-minVal)*multiplier;
					buffer[i] = (unsigned char)voxelValue; buffer[i+1] = (unsigned char)voxelValue; buffer[i+2] = (unsigned char)voxelValue;
					i+=3;
				}
			break;
		case IMG_FLOAT32:
			for (col=0; col<ySize; col++)
				for (row=0; row<xSize; row++) {
					voxelValue = ((double)imgData->imgDataFLOAT32->Get(col,row,slice,0,0)-minVal)*multiplier;
					buffer[i] = (unsigned char)voxelValue; buffer[i+1] = (unsigned char)voxelValue; buffer[i+2] = (unsigned char)voxelValue;
					i+=3;
				}
			break;
		case IMG_FLOAT64:
			for (col=0; col<ySize; col++)
				for (row=0; row<xSize; row++) {
					voxelValue = ((double)imgData->imgDataFLOAT64->Get(col,row,slice,0,0)-minVal)*multiplier;
					buffer[i] = (unsigned char)voxelValue; buffer[i+1] = (unsigned char)voxelValue; buffer[i+2] = (unsigned char)voxelValue;
					i+=3;
				}
			break;
		case IMG_FLOAT128:
			for (col=0; col<ySize; col++)
				for (row=0; row<xSize; row++) {
					voxelValue = ((double)imgData->imgDataFLOAT128->Get(col,row,slice,0,0)-minVal)*multiplier;
					buffer[i] = (unsigned char)voxelValue; buffer[i+1] = (unsigned char)voxelValue; buffer[i+2] = (unsigned char)voxelValue;
					i+=3;
				}
			break;
		case IMG_COMPLEX64:
			for (col=0; col<ySize; col++)
				for (row=0; row<xSize; row++) {
					voxelValue = ((double)imgData->imgDataUINT16->Get(col,row,slice,0,0)-minVal)*multiplier;
					buffer[i] = (unsigned char)voxelValue; buffer[i+1] = (unsigned char)voxelValue; buffer[i+2] = (unsigned char)voxelValue;
					i+=3;
				}
			break;
		case IMG_COMPLEX128:
			for (col=0; col<ySize; col++)
				for (row=0; row<xSize; row++) {
					voxelValue = ((double)imgData->imgDataUINT16->Get(col,row,slice,0,0)-minVal)*multiplier;
					buffer[i] = (unsigned char)voxelValue; buffer[i+1] = (unsigned char)voxelValue; buffer[i+2] = (unsigned char)voxelValue;
					i+=3;
				}
			break;
		case IMG_COMPLEX256:
			for (col=0; col<ySize; col++)
				for (row=0; row<xSize; row++) {
					voxelValue = ((double)imgData->imgDataUINT16->Get(col,row,slice,0,0)-minVal)*multiplier;
					buffer[i] = (unsigned char)voxelValue; buffer[i+1] = (unsigned char)voxelValue; buffer[i+2] = (unsigned char)voxelValue;
					i+=3;
				}
			break;
		case IMG_RGB24:
			for (col=0; col<ySize; col++)
				for (row=0; row<xSize; row++) {
					voxelValueR = ((double)imgData->imgDataRGB24->Get(col,row,slice,0,0)-minVal)*multiplier;
					voxelValueG = ((double)imgData->imgDataRGB24->Get(col,row,slice,0,1)-minVal)*multiplier;
					voxelValueB = ((double)imgData->imgDataRGB24->Get(col,row,slice,0,2)-minVal)*multiplier;
					buffer[i] = (unsigned char)voxelValueR;
					buffer[i+1] = (unsigned char)voxelValueG;
					buffer[i+2] = (unsigned char)voxelValueB;
					i+=3;
				}
			break;
		default:
			break;
	}

	img = new wxImage(xSize,ySize,buffer,false);
	switch (outputType) {
		case OUTPUT_FT_JPEG: img->SaveFile(fileString, wxBITMAP_TYPE_JPEG); break;
		case OUTPUT_FT_PNG: img->SaveFile(fileString, wxBITMAP_TYPE_PNG); break;
//		case OUTPUT_FT_TIFF: img->SaveFile(fileString, wxBITMAP_TYPE_TIFF); break;
		case OUTPUT_FT_BMP: img->SaveFile(fileString, wxBITMAP_TYPE_BMP); break;
		default: img->SaveFile(fileString, wxBITMAP_TYPE_JPEG); break;
	}

	return true;
}
