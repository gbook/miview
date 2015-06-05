/* ----------------------------------------------------------------------------
	NiftiInfo.cpp


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
#include "NiftiInfo.h"
//#include "wx/arrimpl.cpp"

//WX_DEFINE_ARRAY_DOUBLE(double,wxArrayDouble);

NiftiInfo::NiftiInfo(void)
{
}

NiftiInfo::~NiftiInfo(void)
{
}


void NiftiInfo::SetNiftiHeader(nifti_image *img)
{
	ndim = img->ndim;
	nx = img->nx;
	ny = img->ny;
	nz = img->nz;
	nt = img->nt;
	nu = img->nu;
	nv = img->nv;
	nw = img->nw;
	dim.Add(img->dim[0]);
	dim.Add(img->dim[1]);
	dim.Add(img->dim[2]);
	dim.Add(img->dim[3]);
	dim.Add(img->dim[4]);
	dim.Add(img->dim[5]);
	dim.Add(img->dim[6]);
	dim.Add(img->dim[7]);
	nvox = (int)img->nvox;
	nbyper = img->nbyper;
	datatype = img->datatype;
	dx = img->dx;
	dy = img->dy;
	dz = img->dz;
	dt = img->dt;
	du = img->du;
	dv = img->dv;
	dw = img->dw;
	pixdim.Add(img->pixdim[0]);
	pixdim.Add(img->pixdim[1]);
	pixdim.Add(img->pixdim[2]);
	pixdim.Add(img->pixdim[3]);
	pixdim.Add(img->pixdim[4]);
	pixdim.Add(img->pixdim[5]);
	pixdim.Add(img->pixdim[6]);
	pixdim.Add(img->pixdim[7]);
	scl_slope = img->scl_slope;
	scl_inter = img->scl_inter;
	cal_min = img->cal_min;
	cal_max = img->cal_max;
	qform_code = img->qform_code;
	sform_code = img->sform_code;
	freq_dim = img->freq_dim;
	phase_dim = img->phase_dim;
	slice_dim = img->slice_dim;
	slice_code = img->slice_code;
	slice_start = img->slice_start;
	slice_end = img->slice_end;
	slice_duration = img->slice_duration;
	/*! quaternion transform parameters
	[when writing a dataset, these are used for qform, NOT qto_xyz]   */
	quatern_b = img->quatern_b;
	quatern_c = img->quatern_c;
	quatern_d = img->quatern_d;
	qoffset_x = img->qoffset_x;
	qoffset_y = img->qoffset_y;
	qoffset_z = img->qoffset_z;
	qfac = img->qfac;
//	mat44 qto_xyz;
//	mat44 qto_ijk;
//	mat44 sto_xyz;
//	mat44 sto_ijk;
	toffset = img->toffset;
	xyz_units = img->xyz_units;
	time_units = img->time_units;
	nifti_type = img->nifti_type;
	intent_code = img->intent_code;
	intent_p1 = img->intent_p1;
	intent_p2 = img->intent_p2;
	intent_p3 = img->intent_p3;
	intent_name = img->intent_name;
	descrip = img->descrip;
	aux_file = img->aux_file;
	fname = img->fname;
	iname = img->iname;
	iname_offset = img->iname_offset;
	swapsize = img->swapsize;
	byteorder = img->byteorder;
	num_ext = img->num_ext;
	ext_list = img->ext_list;

	/* possible datatype codes */
	switch (datatype) {
		case DT_UNKNOWN:
			MIViewDatatype = IMG_UNKNOWN;
			pixelTypeStr = "IMG_UNKNOWN"; isSigned = false; isRGB = false; isMonochrome = false;
			bitsStored = bitsAllocated = highBitPosition = 0;
			break;
		case DT_BINARY:
			MIViewDatatype = IMG_BINARY;
			pixelTypeStr = "IMG_BINARY"; isSigned = false; isRGB = false; isMonochrome = true;
			bitsStored = bitsAllocated = highBitPosition = 1;
			break;
		case DT_UINT8:
			MIViewDatatype = IMG_UINT8;
			pixelTypeStr = "IMG_UINT8"; isSigned = false; isRGB = false; isMonochrome = true;
			bitsStored = bitsAllocated = highBitPosition = 8;
			break;
		case DT_INT8:
			MIViewDatatype = IMG_INT8;
			pixelTypeStr = "IMG_INT8"; isSigned = true; isRGB = false; isMonochrome = true;
			bitsStored = bitsAllocated = highBitPosition = 8;
			break;
		case DT_UINT16:	MIViewDatatype = IMG_UINT16;
			pixelTypeStr = "IMG_UINT16"; isSigned = false; isRGB = false; isMonochrome = true;
			bitsStored = bitsAllocated = highBitPosition = 16;
			break;
		case DT_INT16:
			MIViewDatatype = IMG_INT16;
			pixelTypeStr = "IMG_INT16"; isSigned = true; isRGB = false; isMonochrome = true;
			bitsStored = bitsAllocated = highBitPosition = 16;
			break;
		case DT_UINT32:
			MIViewDatatype = IMG_UINT32;
			pixelTypeStr = "IMG_UINT32"; isSigned = false; isRGB = false; isMonochrome = true;
			bitsStored = bitsAllocated = highBitPosition = 32;
			break;
		case DT_INT32:
			MIViewDatatype = IMG_INT32;
			pixelTypeStr = "IMG_UINT32"; isSigned = true; isRGB = false; isMonochrome = true;
			bitsStored = bitsAllocated = highBitPosition = 32;
			break;
		case DT_UINT64:
			MIViewDatatype = IMG_UINT64;
			pixelTypeStr = "IMG_UINT64"; isSigned = false; isRGB = false; isMonochrome = true;
			bitsStored = bitsAllocated = highBitPosition = 64;
			break;
		case DT_INT64:
			MIViewDatatype = IMG_INT64;
			pixelTypeStr = "IMG_INT64"; isSigned = true; isRGB = false; isMonochrome = true;
			bitsStored = bitsAllocated = highBitPosition = 64;
			break;
		case DT_FLOAT32:
			MIViewDatatype = IMG_FLOAT32;
			pixelTypeStr = "IMG_FLOAT32"; isSigned = true; isRGB = false; isMonochrome = true;
			bitsStored = bitsAllocated = highBitPosition = 32;
			break;
		case DT_FLOAT64:
			MIViewDatatype = IMG_FLOAT64;
			pixelTypeStr = "IMG_FLOAT64"; isSigned = true; isRGB = false; isMonochrome = true;
			bitsStored = bitsAllocated = highBitPosition = 64;
			break;
		case DT_FLOAT128:
			MIViewDatatype = IMG_FLOAT128;
			pixelTypeStr = "IMG_FLOAT128"; isSigned = true; isRGB = false; isMonochrome = true;
			bitsStored = bitsAllocated = highBitPosition = 128;
			break;
		case DT_COMPLEX64:
			MIViewDatatype = IMG_COMPLEX64;
			pixelTypeStr = "IMG_COMPLEX64"; isSigned = true; isRGB = false; isMonochrome = true;
			bitsStored = bitsAllocated = highBitPosition = 32;
			break;
		case DT_COMPLEX128:
			MIViewDatatype = IMG_COMPLEX128;
			pixelTypeStr = "IMG_COMPLEX128"; isSigned = true; isRGB = false; isMonochrome = true;
			bitsStored = bitsAllocated = highBitPosition = 64;
			break;
		case DT_COMPLEX256:
			MIViewDatatype = IMG_COMPLEX256;
			pixelTypeStr = "IMG_COMPLEX256"; isSigned = true; isRGB = false; isMonochrome = true;
			bitsStored = bitsAllocated = highBitPosition = 128;
			break;
		case DT_RGB24:
			MIViewDatatype = IMG_RGB24;
			pixelTypeStr = "IMG_RGB24"; isSigned = false; isRGB = true; isMonochrome = false;
			bitsStored = bitsAllocated = highBitPosition = 8;
			break;
	}
}

int NiftiInfo::GetMIViewDatatype() { return MIViewDatatype; }
wxString NiftiInfo::GetPixelTypeStr() { return pixelTypeStr; }
bool NiftiInfo::GetIsSigned() { return isSigned; }
bool NiftiInfo::GetIsRGB() { return isRGB; }
bool NiftiInfo::GetIsMonochrome() { return isMonochrome; }
int NiftiInfo::GetBitsStored() { return bitsStored; }
int NiftiInfo::GetBitsAllocated() { return bitsAllocated; }
int NiftiInfo::GetHighBitPosition() { return highBitPosition; }

int NiftiInfo::Get_ndim() { return ndim; }                    /*!< last dimension greater than 1 (1..7) */
int NiftiInfo::Get_nx() { return nx; }                      /*!< dimensions of grid array             */
int NiftiInfo::Get_ny() { return ny; }                     /*!< dimensions of grid array             */
int NiftiInfo::Get_nz() { return nz; }                      /*!< dimensions of grid array             */
int NiftiInfo::Get_nt() { return nt; }                      /*!< dimensions of grid array             */
int NiftiInfo::Get_nu() { return nu; }                      /*!< dimensions of grid array             */
int NiftiInfo::Get_nv() { return nv; }                      /*!< dimensions of grid array             */
int NiftiInfo::Get_nw() { return nw; }                      /*!< dimensions of grid array             */
wxArrayInt NiftiInfo::Get_dim() { return dim; }              /*!< dim[0]=ndim, dim[1]=nx, etc.         */
int NiftiInfo::Get_nvox() { return nvox; }                    /*!< number of voxels = nx*ny*nz*...*nw   */
int NiftiInfo::Get_nbyper() { return nbyper; }                  /*!< bytes per voxel, matches datatype    */
int NiftiInfo::Get_datatype() { return datatype; }                /*!< type of data in voxels: DT_* code    */
float NiftiInfo::Get_dx() { return dx; }                    /*!< grid spacings      */
float NiftiInfo::Get_dy() { return dy; }                    /*!< grid spacings      */
float NiftiInfo::Get_dz() { return dz; }                    /*!< grid spacings      */
float NiftiInfo::Get_dt() { return dt; }                    /*!< grid spacings      */
float NiftiInfo::Get_du() { return du; }                    /*!< grid spacings      */
float NiftiInfo::Get_dv() { return dv; }                    /*!< grid spacings      */
float NiftiInfo::Get_dw() { return dw; }                    /*!< grid spacings      */
wxArrayDouble NiftiInfo::Get_pixdim() { return pixdim; }        /*!< pixdim[1]=dx, etc. */
float NiftiInfo::Get_scl_slope() { return scl_slope; }             /*!< scaling parameter - slope        */
float NiftiInfo::Get_scl_inter() { return scl_inter; }             /*!< scaling parameter - intercept    */
float NiftiInfo::Get_cal_min() { return cal_min; }               /*!< calibration parameter, minimum   */
float NiftiInfo::Get_cal_max() { return cal_max; }               /*!< calibration parameter, maximum   */
int NiftiInfo::Get_qform_code() { return qform_code; }              /*!< codes for (x,y,z) space meaning  */
int NiftiInfo::Get_sform_code() { return sform_code; }              /*!< codes for (x,y,z) space meaning  */
int NiftiInfo::Get_freq_dim() { return freq_dim; }                /*!< indexes (1,2,3, or 0) for MRI    */
int NiftiInfo::Get_phase_dim() { return phase_dim; }               /*!< directions in dim[]/pixdim[]     */
int NiftiInfo::Get_slice_dim() { return slice_dim; }               /*!< directions in dim[]/pixdim[]     */
int NiftiInfo::Get_slice_code() { return slice_code; }              /*!< code for slice timing pattern    */
int NiftiInfo::Get_slice_start() { return slice_start; }             /*!< index for start of slices        */
int NiftiInfo::Get_slice_end() { return slice_end; }               /*!< index for end of slices          */
float NiftiInfo::Get_slice_duration() { return slice_duration; }        /*!< time between individual slices   */
float NiftiInfo::Get_quatern_b() { return quatern_b; }
float NiftiInfo::Get_quatern_c() { return quatern_c; }
float NiftiInfo::Get_quatern_d() { return quatern_d; }
float NiftiInfo::Get_qoffset_x() { return qoffset_x; }
float NiftiInfo::Get_qoffset_y() { return qoffset_y; }
float NiftiInfo::Get_qoffset_z() { return qoffset_z; }
float NiftiInfo::Get_qfac() { return qfac; }
float NiftiInfo::Get_toffset() { return toffset; }               /*!< time coordinate offset */
int NiftiInfo::Get_xyz_units() { return xyz_units; }               /*!< dx,dy,dz units: NIFTI_UNITS_* code  */
int NiftiInfo::Get_time_units() { return time_units; }              /*!< dt       units: NIFTI_UNITS_* code  */
int NiftiInfo::Get_nifti_type() { return nifti_type; }              /*!< 0==ANALYZE, 1==NIFTI-1 (1 file), 2==NIFTI-1 (2 files), 3==NIFTI-ASCII (1 file) */
int NiftiInfo::Get_intent_code() { return intent_code; }             /*!< statistic type (or something)       */
float NiftiInfo::Get_intent_p1() { return intent_p1; }             /*!< intent parameters                   */
float NiftiInfo::Get_intent_p2() { return intent_p2; }             /*!< intent parameters                   */
float NiftiInfo::Get_intent_p3() { return intent_p3; }             /*!< intent parameters                   */
wxString NiftiInfo::Get_intent_name() { return intent_name; }        /*!< optional description of intent data */
wxString NiftiInfo::Get_descrip() { return descrip; }            /*!< optional text to describe dataset   */
wxString NiftiInfo::Get_aux_file() { return aux_file; }           /*!< auxiliary filename                  */
wxString NiftiInfo::Get_fname() { return fname; }              /*!< header filename (.hdr or .nii)         */
wxString NiftiInfo::Get_iname() { return iname; }              /*!< image filename  (.img or .nii)         */
int NiftiInfo::Get_iname_offset() { return iname_offset; }            /*!< offset into iname where data starts    */
int NiftiInfo::Get_swapsize() { return swapsize; }                /*!< swap unit in image data (might be 0)   */
int NiftiInfo::Get_byteorder() { return byteorder; }               /*!< byte order on disk (MSB_ or LSB_FIRST) */
int NiftiInfo::Get_num_ext() { return num_ext; }                 /*!< number of extensions in ext_list       */
