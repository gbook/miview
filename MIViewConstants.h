/* ----------------------------------------------------------------------------
	MIViewConstants.h


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


/* version information */
#define VERSION_MAJOR		0
#define VERSION_MINOR		6
#define VERSION_REVISION	0
#define VERSION_ISBETA		0

/* type defintions */
typedef bool				UINT_1;
typedef unsigned char		UINT_8;
typedef unsigned short		UINT_16;
typedef unsigned int		UINT_32;
typedef unsigned long long int	UINT_64;
typedef char				INT_8;
typedef short				INT_16;
typedef int					INT_32;
typedef long long int		INT_64;
typedef float				FLOAT_32;
typedef double				FLOAT_64;
typedef long double			FLOAT_128;

/* used for the multislice 'sort by' */
#define ROWS     97
#define COLUMNS  98

/* OpenGL display modes */
#define GLMODE_FLAT			2900
#define GLMODE_VOLREND		2901
#define GLMODE_MIP			2902
#define GLMODE_3D_DEPTH		2903
#define GLMODE_3D_NODEPTH	2904

/* file open method */
#define OPEN_ALL		2950
#define OPEN_SINGLE		2951
#define OPEN_SELECTED	2952

/* image data types */
#define IMG_BINARY		3100	/* 1-bit (unsigned char)		Stored as unsigned bytes	*/
#define IMG_UINT8		3101	/* 8-bit (unsigned char)		1 byte						*/
#define IMG_INT8		3102	/* 8-bit (char)					1 byte						*/
#define IMG_UINT12		3103	/* 16-bit (unsigned short)		2 bytes						*/
#define IMG_INT12		3104	/* 16-bit (short)				2 bytes						*/
#define IMG_UINT16		3105	/* 16-bit (unsigned short)		2 bytes						*/
#define IMG_INT16		3106	/* 16-bit (short)				2 bytes						*/
#define IMG_UINT32		3107	/* 32-bit (unsigned int)		4 bytes						*/
#define IMG_INT32		3108	/* 32-bit (int)					4 bytes						*/
#define IMG_UINT64		3109	/* 64-bit (unsigned long int)	8 bytes						*/
#define IMG_INT64		3110	/* 64-bit (long int)			8 bytes						*/
#define IMG_FLOAT16		3111	/* 16-bit (stored as float)		4 bytes						*/
#define IMG_FLOAT32		3112	/* 32-bit (float)				4 bytes						*/
#define IMG_FLOAT64		3113	/* 64-bit (double)				8 bytes						*/
#define IMG_FLOAT128	3114	/* 128-bit (long double)		16 bytes					*/
#define IMG_COMPLEX64	3115	/* 2x32-bit (float, float)		8 bytes						*/
#define IMG_COMPLEX128	3116	/* 2x64-bit (double, double)	16 bytes					*/
#define IMG_COMPLEX256	3117	/* 2x128-bit (long double, long double)	32 bytes			*/
//#define IMG_FD			3116	/* same as IMG_FLOAT64, but used in Dicom	8 bytes			*/
#define IMG_RGB24		3118	/* 3x8-bit (unsigned int, unsigned int, unsigned int) 3 bytes */
#define IMG_RGBA24		3119	/* 3x8-bit (unsigned int, unsigned int, unsigned int) 3 bytes */
#define IMG_UNKNOWN		3120	/* 0 bytes */

/* volume types */
#define VOL_1D			3200
#define VOL_2D			3201
#define VOL_3D			3202
#define VOL_4D			3203

/* image types */
#define IMG_1D			3204
#define IMG_2D			3205
#define IMG_3D			3206
#define IMG_4D			3207

/* define the view types */
#define IMGVIEW_3VIEW          2000 /* axial, coronal, sagittal */
#define IMGVIEW_SINGLEPLANE    2001 /* single plane */
#define IMGVIEW_SSD            2002 /* surface shaded display */
#define IMGVIEW_MIP            2003 /* maximum intensity projection */
#define IMGVIEW_DVR            2004 /* volume rendering */
#define IMGVIEW_MULTISLICE     2005 /* multislice */
#define IMGVIEW_FREEROTATE     2006 /* 2D free rotation */

/* define file data type */
#define FILETYPE_DICOM			2500
#define FILETYPE_ANALYZE		2501
#define FILETYPE_RASTER			2502
//#define FILETYPE_RASTER_BMP		2503
//#define FILETYPE_RASTER_GIF		2504
//#define FILETYPE_RASTER_TIF		2505
//#define FILETYPE_RASTER_PNG		2506
//#define FILETYPE_RASTER_XPM		2507
#define FILETYPE_AUTO			2509
#define FILETYPE_UNKNOWN		2510

/* output filetypes */
#define OUTPUT_FT_DICOM			2600
#define OUTPUT_FT_DICOM_MOS		2601
#define OUTPUT_FT_ANALYZE		2602
#define OUTPUT_FT_NIFTI1FILE	2603
#define OUTPUT_FT_NIFTI2FILE	2604
#define OUTPUT_FT_NIFTIASCII	2605
#define OUTPUT_FT_JPEG			2606
#define OUTPUT_FT_PNG			2607
#define OUTPUT_FT_TIFF			2608
#define OUTPUT_FT_BMP			2609

/* conversion types */
#define CONVERT_1_TO_1			2700
#define CONVERT_1_TO_N			2701
#define CONVERT_N_TO_1			2702

/* window IDs */
#define MAIN_FRAME_ID			3000
#define LOADERROR_DIALOG_ID		3001

/* patient position */
#define PPOS_HFS			4000
#define PPOS_FFS			4001
#define PPOS_HFP			4002
#define PPOS_FFP			4003
#define PPOS_HFDR			4004
#define PPOS_HFDL			4005
#define PPOS_FFDR			4006
#define PPOS_FFDL			4007
#define PPOS_UNKNOWN		4008

/* define Volume rendering methods */
#define VOLMETHOD_3DTEXTURE	4100
#define VOLMETHOD_RAYTRACE	4101