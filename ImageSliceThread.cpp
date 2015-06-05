/* ----------------------------------------------------------------------------
	ImageSliceThread.cpp


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


#include "MIView.h"
#include "ImageViews.h"
//#include "LogWindow.h"
#include "ImageData.h"

#include "wx/thread.h"

#include "ImageVolumeHelper.h"
#include "ImageSliceThread.h"

#include "gl/glut.h"

#include <crtdbg.h>
#define new new(_NORMAL_BLOCK,__FILE__, __LINE__)


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImageSliceThread::ImageSliceThread(ImageData *imageData, int tnum, wxMutex *mutex, wxCondition *condition)
 : wxThread(wxTHREAD_JOINABLE)
{
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
	_CrtSetDbgFlag(flag); // Set flag to the new value

	//logWindow = &logwind;
	imgData = imageData;
	threadnum = tnum;
	m_mutex = mutex;
	m_condition = condition;
	Create();
}

ImageSliceThread::~ImageSliceThread()
{
	wxString msg;
	WriteLog( msg.Format("Thread #%d - FINISHED\n",threadnum) );
}


// --------------------------------------------------------
// ---------- SetupThread ---------------------------------
// --------------------------------------------------------
void ImageSliceThread::SetupThread(int datatype, void *tempImage, int bufferSize, wxSize originalSize, wxSize textureSize, int plane, int slice, bool returnRGB, int volumeNum, double minVal)
{
	m_bufferSize = bufferSize;
	m_originalSize = originalSize;
	m_textureSize = textureSize;
	m_plane = plane;
	m_slice = slice;
	imgtype = datatype;
	m_retRGB = returnRGB;
//	m_alpha = alpha;
	m_minVal = minVal;
	m_volume = volumeNum;
	
	if (m_retRGB)
		drawBuffer8U = (unsigned char*)tempImage;
	else {
		switch (imgtype) {
			case IMG_UINT8: drawBuffer8U = (unsigned char*)tempImage; break;
			case IMG_INT8: drawBuffer8U = (unsigned char*)tempImage; break;
			case IMG_UINT16: drawBuffer16U = (unsigned short int*)tempImage; break;
			case IMG_INT16: drawBuffer16U = (unsigned short int*)tempImage; break;
			case IMG_UINT32: drawBuffer32U = (unsigned int*)tempImage; break;
			case IMG_INT32: drawBuffer32U = (unsigned int*)tempImage; break;
			case IMG_FLOAT32: drawBufferFLOAT32 = (float*)tempImage; break;
			case IMG_RGB24: drawBufferRGB = (unsigned char*)tempImage; break;
		}
	}
}


// --------------------------------------------------------
// ---------- Entry ---------------------------------------
// --------------------------------------------------------
/* this part is run when Run() is called for this thread */
void *ImageSliceThread::Entry()
{
	wxString msg;

	/* do the work */
	WriteLog( msg.Format("Thread #%d - SetupThread() called\n",threadnum) );

	switch (imgtype) {
		case IMG_BINARY: break;
		case IMG_UINT8: PopulateBufferUINT8(); break;
		case IMG_INT8: PopulateBufferINT8(); break;
		case IMG_UINT16: PopulateBufferUINT16(); break;
		case IMG_INT16: PopulateBufferINT16(); break;
		case IMG_UINT32: PopulateBufferUINT32(); break;
		case IMG_INT32: PopulateBufferINT32(); break;
		case IMG_UINT64: break;
		case IMG_INT64: break;
		case IMG_FLOAT32: PopulateBufferFLOAT32(); break;
		case IMG_FLOAT64: break;
		case IMG_FLOAT128: break;
		case IMG_COMPLEX64: break;
		case IMG_COMPLEX128: break;
		case IMG_COMPLEX256: break;
//		case IMG_FD: PopulateBufferFLOAT32(); break;
		case IMG_RGB24: PopulateBufferRGB(); break;
		case IMG_UNKNOWN: break;
	}

	return NULL;
}


// --------------------------------------------------------
// ---------- PopulateBufferUINT8 -------------------------
// --------------------------------------------------------
void ImageSliceThread::PopulateBufferUINT8()
{
	int index, row, col;
	unsigned char voxelValue;
	wxMutexLocker lock(*m_mutex);

	index=0;
	/* loop through the size of the texture, pad the image with dark pixels */
	for (col=0; col < m_textureSize.GetHeight(); col++) {
		for (row=0; row < m_textureSize.GetWidth(); row++) {
			/* check if the original image falls within the current col/row pair */
			if ((row < m_originalSize.GetWidth()) && (col < m_originalSize.GetHeight())) {
				switch (m_plane) {
					case 0: voxelValue = imgData->imgDataUINT8->Get(col,row,m_slice,m_volume,0); break;
					case 1: voxelValue = imgData->imgDataUINT8->Get(col,m_slice,row,m_volume,0); break;
					case 2: voxelValue = imgData->imgDataUINT8->Get(m_slice,col,row,m_volume,0); break;
					default: voxelValue = imgData->imgDataUINT8->Get(col,row,m_slice,m_volume,0); break;
				}
			}
			else
				voxelValue = 0;
			drawBuffer8U[index] = voxelValue;
			index++;
		}
	}
	tmpImage = &drawBuffer8U;
}


// --------------------------------------------------------
// ---------- PopulateBufferINT8 --------------------------
// --------------------------------------------------------
void ImageSliceThread::PopulateBufferINT8()
{
	int index, row, col;
	char voxelValue;
	wxMutexLocker lock(*m_mutex);

	index=0;
	/* loop through the size of the texture, pad the image with dark pixels */
	for (col=0; col < m_textureSize.GetHeight(); col++) {
		for (row=0; row < m_textureSize.GetWidth(); row++) {
			/* check if the original image falls within the current col/row pair */
			if ((row < m_originalSize.GetWidth()) && (col < m_originalSize.GetHeight())) {
				switch (m_plane) {
					case 0: voxelValue = imgData->imgDataINT8->Get(col,row,m_slice,m_volume,0); break;
					case 1: voxelValue = imgData->imgDataINT8->Get(col,m_slice,row,m_volume,0); break;
					case 2: voxelValue = imgData->imgDataINT8->Get(m_slice,col,row,m_volume,0); break;
					default: voxelValue = imgData->imgDataINT8->Get(col,row,m_slice,m_volume,0); break;
				}
			}
			else
				voxelValue = 0;
			drawBuffer8U[index] = (unsigned char)((double)voxelValue - m_minVal);
			index++;
		}
	}
	tmpImage = &drawBuffer8U;
}


// --------------------------------------------------------
// ---------- PopulateBufferUINT16 ------------------------
// --------------------------------------------------------
void ImageSliceThread::PopulateBufferUINT16()
{
	int index, row, col;
	unsigned short int voxelValue;
	index=0;

	wxMutexLocker lock(*m_mutex);

	/* loop through the size of the texture, pad the image with dark pixels */
	for (col=0; col < m_textureSize.GetHeight(); col++) {
		for (row=0; row < m_textureSize.GetWidth(); row++) {
			/* check if the original image falls within the current col/row pair */
			if ((row < m_originalSize.GetWidth()) && (col < m_originalSize.GetHeight())) {
				switch (m_plane) {
					case 0: voxelValue = imgData->imgDataUINT16->Get(col,row,m_slice,m_volume,0); break;
					case 1: voxelValue = imgData->imgDataUINT16->Get(col,m_slice,row,m_volume,0); break;
					case 2: voxelValue = imgData->imgDataUINT16->Get(m_slice,col,row,m_volume,0); break;
					default: voxelValue = imgData->imgDataUINT16->Get(col,row,m_slice,m_volume,0); break;
				}
			}
			else
				voxelValue = 0;
			drawBuffer16U[index] = voxelValue;
			index++;
		}
	}
	tmpImage = &drawBuffer16U;
}


// --------------------------------------------------------
// ---------- PopulateBufferINT16 -------------------------
// --------------------------------------------------------
void ImageSliceThread::PopulateBufferINT16()
{
	int index, row, col;
	short int voxelValue;
	wxMutexLocker lock(*m_mutex);

	index=0;
	/* loop through the size of the texture, pad the image with dark pixels */
	for (col=0; col < m_textureSize.GetHeight(); col++) {
		for (row=0; row < m_textureSize.GetWidth(); row++) {
			/* check if the original image falls within the current col/row pair */
			if ((row < m_originalSize.GetWidth()) && (col < m_originalSize.GetHeight())) {
				switch (m_plane) {
					case 0: voxelValue = imgData->imgDataINT16->Get(col,row,m_slice,m_volume,0); break;
					case 1: voxelValue = imgData->imgDataINT16->Get(col,m_slice,row,m_volume,0); break;
					case 2: voxelValue = imgData->imgDataINT16->Get(m_slice,col,row,m_volume,0); break;
					default: voxelValue = imgData->imgDataINT16->Get(col,row,m_slice,m_volume,0); break;
				}
			}
			else
				voxelValue = 0;
			drawBuffer16U[index] = (unsigned short int)((double)voxelValue - m_minVal);
			index++;
		}
	}
	tmpImage = &drawBuffer16U;
}


// --------------------------------------------------------
// ---------- PopulateBufferUINT32 ------------------------
// --------------------------------------------------------
void ImageSliceThread::PopulateBufferUINT32()
{
	int index, row, col;
	unsigned int voxelValue;
	wxMutexLocker lock(*m_mutex);

	index=0;
	/* loop through the size of the texture, pad the image with dark pixels */
	for (col=0; col < m_textureSize.GetHeight(); col++) {
		for (row=0; row < m_textureSize.GetWidth(); row++) {
			/* check if the original image falls within the current col/row pair */
			if ((row < m_originalSize.GetWidth()) && (col < m_originalSize.GetHeight())) {
				switch (m_plane) {
					case 0: voxelValue = imgData->imgDataUINT32->Get(col,row,m_slice,m_volume,0); break;
					case 1: voxelValue = imgData->imgDataUINT32->Get(col,m_slice,row,m_volume,0); break;
					case 2: voxelValue = imgData->imgDataUINT32->Get(m_slice,col,row,m_volume,0); break;
					default: voxelValue = imgData->imgDataUINT32->Get(col,row,m_slice,m_volume,0); break;
				}
			}
			else
				voxelValue = 0;
			drawBuffer32U[index] = voxelValue;
			index++;
		}
	}
	tmpImage = &drawBuffer32U;
}


// --------------------------------------------------------
// ---------- PopulateBufferINT32 -------------------------
// --------------------------------------------------------
void ImageSliceThread::PopulateBufferINT32()
{
	int index, row, col;
	int voxelValue;
	wxMutexLocker lock(*m_mutex);

	index=0;
	/* loop through the size of the texture, pad the image with dark pixels */
	for (col=0; col < m_textureSize.GetHeight(); col++) {
		for (row=0; row < m_textureSize.GetWidth(); row++) {
			/* check if the original image falls within the current col/row pair */
			if ((row < m_originalSize.GetWidth()) && (col < m_originalSize.GetHeight())) {
				switch (m_plane) {
					case 0: voxelValue = imgData->imgDataINT32->Get(col,row,m_slice,m_volume,0); break;
					case 1: voxelValue = imgData->imgDataINT32->Get(col,m_slice,row,m_volume,0); break;
					case 2: voxelValue = imgData->imgDataINT32->Get(m_slice,col,row,m_volume,0); break;
					default: voxelValue = imgData->imgDataINT32->Get(col,row,m_slice,m_volume,0); break;
				}
			}
			else
				voxelValue = 0;
			drawBuffer32U[index] = (unsigned int)((double)voxelValue - m_minVal);;
			index++;
		}
	}
	tmpImage = &drawBuffer32U;
}


// --------------------------------------------------------
// ---------- PopulateBufferFLOAT32 -----------------------
// --------------------------------------------------------
void ImageSliceThread::PopulateBufferFLOAT32()
{
	int index, row, col;
	float voxelValue;
	wxMutexLocker lock(*m_mutex);

	index=0;
	/* loop through the size of the texture, pad the image with dark pixels */
	for (col=0; col < m_textureSize.GetHeight(); col++) {
		for (row=0; row < m_textureSize.GetWidth(); row++) {
			/* check if the original image falls within the current col/row pair */
			if ((row < m_originalSize.GetWidth()) && (col < m_originalSize.GetHeight())) {
				switch (m_plane) {
					case 0: voxelValue = imgData->imgDataFLOAT32->Get(col,row,m_slice,m_volume,0); break;
					case 1: voxelValue = imgData->imgDataFLOAT32->Get(col,m_slice,row,m_volume,0); break;
					case 2: voxelValue = imgData->imgDataFLOAT32->Get(m_slice,col,row,m_volume,0); break;
					default: voxelValue = imgData->imgDataFLOAT32->Get(col,row,m_slice,m_volume,0); break;
				}
			}
			else
				voxelValue = 0;
			drawBufferFLOAT32[index] = voxelValue;
			index++;
		}
	}
	tmpImage = &drawBufferFLOAT32;
}


// --------------------------------------------------------
// ---------- PopulateBufferRGB ---------------------------
// --------------------------------------------------------
void ImageSliceThread::PopulateBufferRGB()
{
	unsigned char voxelValueR, voxelValueG, voxelValueB;
	int index, row, col;
	wxMutexLocker lock(*m_mutex);

	index=0;
	/* loop through the size of the texture, pad the image with dark pixels */
	for (col=0; col < m_textureSize.GetHeight(); col++) {
		for (row=0; row < m_textureSize.GetWidth(); row++) {
			/* check if the original image falls within the current col/row pair */
			if ((row < m_originalSize.GetWidth()) && (col < m_originalSize.GetHeight())) {
				switch (m_plane) {
					case 0:
						voxelValueR = imgData->imgDataRGB24->Get(col,row,m_slice,m_volume,0);
						voxelValueG = imgData->imgDataRGB24->Get(col,row,m_slice,m_volume,1);
						voxelValueB = imgData->imgDataRGB24->Get(col,row,m_slice,m_volume,2);
						break;
					case 1:
						voxelValueR = imgData->imgDataRGB24->Get(col,m_slice,row,m_volume,0);
						voxelValueG = imgData->imgDataRGB24->Get(col,m_slice,row,m_volume,1);
						voxelValueB = imgData->imgDataRGB24->Get(col,m_slice,row,m_volume,2);
						break;
					case 2:
						voxelValueR = imgData->imgDataRGB24->Get(m_slice,col,row,m_volume,0);
						voxelValueG = imgData->imgDataRGB24->Get(m_slice,col,row,m_volume,1);
						voxelValueB = imgData->imgDataRGB24->Get(m_slice,col,row,m_volume,2);
						break;
					default:
						voxelValueR = imgData->imgDataRGB24->Get(col,row,m_slice,m_volume,0);
						voxelValueG = imgData->imgDataRGB24->Get(col,row,m_slice,m_volume,1);
						voxelValueB = imgData->imgDataRGB24->Get(col,row,m_slice,m_volume,2);
						break;
				}
			}
			else
				voxelValueR = voxelValueG = voxelValueB = 0;

			drawBufferRGB[index] = voxelValueR;
			drawBufferRGB[index+1] = voxelValueG;
			drawBufferRGB[index+2] = voxelValueB;
			index+=3;
		}
	}
	tmpImage = &drawBufferRGB;
}