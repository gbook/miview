/* ----------------------------------------------------------------------------
	NiftiInfo.h


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

#include "wx/dynarray.h"
#include "fslio/fslio.h"

//WX_DEFINE_ARRAY_DOUBLE(double,wxArrayDouble);

class NiftiInfo
{
	friend class ImageData;
public:
	NiftiInfo(void);
	~NiftiInfo(void);

	int Get_ndim();                    /*!< last dimension greater than 1 (1..7) */
	int Get_nx();                      /*!< dimensions of grid array             */
	int Get_ny();                      /*!< dimensions of grid array             */
	int Get_nz();                      /*!< dimensions of grid array             */
	int Get_nt();                      /*!< dimensions of grid array             */
	int Get_nu();                      /*!< dimensions of grid array             */
	int Get_nv();                      /*!< dimensions of grid array             */
	int Get_nw();                      /*!< dimensions of grid array             */
	wxArrayInt Get_dim();              /*!< dim[0]=ndim, dim[1]=nx, etc.         */
	int Get_nvox();                    /*!< number of voxels = nx*ny*nz*...*nw   */
	int Get_nbyper();                  /*!< bytes per voxel, matches datatype    */
	int Get_datatype();                /*!< type of data in voxels: DT_* code    */
	float Get_dx();                    /*!< grid spacings      */
	float Get_dy();                    /*!< grid spacings      */
	float Get_dz();                    /*!< grid spacings      */
	float Get_dt();                    /*!< grid spacings      */
	float Get_du();                    /*!< grid spacings      */
	float Get_dv();                    /*!< grid spacings      */
	float Get_dw();                    /*!< grid spacings      */
	wxArrayDouble Get_pixdim();        /*!< pixdim[1]=dx, etc. */
	float Get_scl_slope();             /*!< scaling parameter - slope        */
	float Get_scl_inter();             /*!< scaling parameter - intercept    */
	float Get_cal_min();               /*!< calibration parameter, minimum   */
	float Get_cal_max();               /*!< calibration parameter, maximum   */
	int Get_qform_code();              /*!< codes for (x,y,z) space meaning  */
	int Get_sform_code();              /*!< codes for (x,y,z) space meaning  */
	int Get_freq_dim();                /*!< indexes (1,2,3, or 0) for MRI    */
	int Get_phase_dim();               /*!< directions in dim[]/pixdim[]     */
	int Get_slice_dim();               /*!< directions in dim[]/pixdim[]     */
	int Get_slice_code();              /*!< code for slice timing pattern    */
	int Get_slice_start();             /*!< index for start of slices        */
	int Get_slice_end();               /*!< index for end of slices          */
	float Get_slice_duration();        /*!< time between individual slices   */
	float Get_quatern_b();
	float Get_quatern_c();
	float Get_quatern_d();
	float Get_qoffset_x();
	float Get_qoffset_y();
	float Get_qoffset_z();
	float Get_qfac();
	float Get_toffset();               /*!< time coordinate offset */
	int Get_xyz_units();               /*!< dx,dy,dz units: NIFTI_UNITS_* code  */
	int Get_time_units();              /*!< dt       units: NIFTI_UNITS_* code  */
	int Get_nifti_type();              /*!< 0==ANALYZE, 1==NIFTI-1 (1 file), 2==NIFTI-1 (2 files), 3==NIFTI-ASCII (1 file) */
	int Get_intent_code();             /*!< statistic type (or something)       */
	float Get_intent_p1();             /*!< intent parameters                   */
	float Get_intent_p2();             /*!< intent parameters                   */
	float Get_intent_p3();             /*!< intent parameters                   */
	wxString Get_intent_name();        /*!< optional description of intent data */
	wxString Get_descrip();            /*!< optional text to describe dataset   */
	wxString Get_aux_file();           /*!< auxiliary filename                  */
	wxString Get_fname();              /*!< header filename (.hdr or .nii)         */
	wxString Get_iname();              /*!< image filename  (.img or .nii)         */
	int Get_iname_offset();            /*!< offset into iname where data starts    */
	int Get_swapsize();                /*!< swap unit in image data (might be 0)   */
	int Get_byteorder();               /*!< byte order on disk (MSB_ or LSB_FIRST) */
	int Get_num_ext();                 /*!< number of extensions in ext_list       */
	int GetMIViewDatatype();
	wxString GetPixelTypeStr();
	bool GetIsSigned();
	bool GetIsRGB();
	bool GetIsMonochrome();
	int GetBitsStored();
	int GetBitsAllocated();
	int GetHighBitPosition();

	void SetNiftiHeader(nifti_image *img);

private:

	int MIViewDatatype;			/* MIView's interpretation of the datatype */
	wxString pixelTypeStr;
	bool isSigned;
	bool isRGB;
	bool isMonochrome;
	int bitsStored;
	int bitsAllocated;
	int highBitPosition;

	int ndim;                    /*!< last dimension greater than 1 (1..7) */
	int nx;                      /*!< dimensions of grid array             */
	int ny;                      /*!< dimensions of grid array             */
	int nz;                      /*!< dimensions of grid array             */
	int nt;                      /*!< dimensions of grid array             */
	int nu;                      /*!< dimensions of grid array             */
	int nv;                      /*!< dimensions of grid array             */
	int nw;                      /*!< dimensions of grid array             */
	wxArrayInt dim;              /*!< dim[0]=ndim, dim[1]=nx, etc.         */
	int nvox;                    /*!< number of voxels = nx*ny*nz*...*nw   */
	int nbyper;                  /*!< bytes per voxel, matches datatype    */
	int datatype;                /*!< type of data in voxels: DT_* code    */
	float dx;                    /*!< grid spacings      */
	float dy;                    /*!< grid spacings      */
	float dz;                    /*!< grid spacings      */
	float dt;                    /*!< grid spacings      */
	float du;                    /*!< grid spacings      */
	float dv;                    /*!< grid spacings      */
	float dw;                    /*!< grid spacings      */
	wxArrayDouble pixdim;        /*!< pixdim[1]=dx, etc. */
	float scl_slope;             /*!< scaling parameter - slope        */
	float scl_inter;             /*!< scaling parameter - intercept    */
	float cal_min;               /*!< calibration parameter, minimum   */
	float cal_max;               /*!< calibration parameter, maximum   */
	int qform_code;              /*!< codes for (x,y,z) space meaning  */
	int sform_code;              /*!< codes for (x,y,z) space meaning  */
	int freq_dim;                /*!< indexes (1,2,3, or 0) for MRI    */
	int phase_dim;               /*!< directions in dim[]/pixdim[]     */
	int slice_dim;               /*!< directions in dim[]/pixdim[]     */
	int slice_code;              /*!< code for slice timing pattern    */
	int slice_start;             /*!< index for start of slices        */
	int slice_end;               /*!< index for end of slices          */
	float slice_duration;        /*!< time between individual slices   */
	/*! quaternion transform parameters
	[when writing a dataset, these are used for qform, NOT qto_xyz]   */
	float quatern_b , quatern_c , quatern_d , qoffset_x , qoffset_y , qoffset_z , qfac;
//	mat44 qto_xyz;               /*!< qform: transform (i,j,k) to (x,y,z) */
//	mat44 qto_ijk;               /*!< qform: transform (x,y,z) to (i,j,k) */
//	mat44 sto_xyz;               /*!< sform: transform (i,j,k) to (x,y,z) */
//	mat44 sto_ijk;               /*!< sform: transform (x,y,z) to (i,j,k) */
	float toffset;               /*!< time coordinate offset */
	int xyz_units;               /*!< dx,dy,dz units: NIFTI_UNITS_* code  */
	int time_units;              /*!< dt       units: NIFTI_UNITS_* code  */
	int nifti_type;              /*!< 0==ANALYZE, 1==NIFTI-1 (1 file), 2==NIFTI-1 (2 files), 3==NIFTI-ASCII (1 file) */
	int intent_code;             /*!< statistic type (or something)       */
	float intent_p1;             /*!< intent parameters                   */
	float intent_p2;             /*!< intent parameters                   */
	float intent_p3;             /*!< intent parameters                   */
	wxString intent_name;        /*!< optional description of intent data */
	wxString descrip;            /*!< optional text to describe dataset   */
	wxString aux_file;           /*!< auxiliary filename                  */
	wxString fname;              /*!< header filename (.hdr or .nii)         */
	wxString iname;              /*!< image filename  (.img or .nii)         */
	int iname_offset;            /*!< offset into iname where data starts    */
	int swapsize;                /*!< swap unit in image data (might be 0)   */
	int byteorder;               /*!< byte order on disk (MSB_ or LSB_FIRST) */
	int num_ext;                 /*!< number of extensions in ext_list       */
	nifti1_extension * ext_list; /*!< array of extension structs (with data) */
};
