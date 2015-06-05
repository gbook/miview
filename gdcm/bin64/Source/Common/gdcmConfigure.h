/*=========================================================================

  Program: GDCM (Grassroots DICOM). A DICOM library
  Module:  $URL$

  Copyright (c) 2006-2010 Mathieu Malaterre
  All rights reserved.
  See Copyright.txt or http://gdcm.sourceforge.net/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef GDCMCONFIGURE_H
#define GDCMCONFIGURE_H

/* This header is configured by GDCM's build process.  */

/*--------------------------------------------------------------------------*/
/* Platform Features                                                        */

/* Byte order.  */
/* All compilers that support Mac OS X define either __BIG_ENDIAN__ or
   __LITTLE_ENDIAN__ to match the endianness of the architecture being
   compiled for. This is not necessarily the same as the architecture of the
   machine doing the building. In order to support Universal Binaries on
   Mac OS X, we prefer those defines to decide the endianness.
   On other platforms we use the result of the TRY_RUN. */
#if !defined(__APPLE__)
/* #undef GDCM_WORDS_BIGENDIAN */
#elif defined(__BIG_ENDIAN__)
# define GDCM_WORDS_BIGENDIAN
#endif

/* Allow access to UINT32_MAX , cf gdcmCommon.h */
#define __STDC_LIMIT_MACROS

/* Hard code the path to the public dictionary */
#define PUB_DICT_PATH ""

/* Usefull in particular for loadshared where the full path
 * to the lib is needed */
#define GDCM_SOURCE_DIR "C:/gdcm/gdcm-2.0.16"
#define GDCM_EXECUTABLE_OUTPUT_PATH "C:/gdcm/binaries-64/bin"
#define GDCM_LIBRARY_OUTPUT_PATH    "C:/gdcm/binaries-64/bin"

/* #undef GDCM_BUILD_TESTING */

/* #undef GDCM_USE_SYSTEM_ZLIB */
/* #undef GDCM_USE_SYSTEM_UUID */
/* #undef GDCM_USE_SYSTEM_POPPLER */
/* #undef GDCM_USE_SYSTEM_PODOFO */
/* #undef GDCM_USE_SYSTEM_OPENSSL */
/* #undef GDCM_USE_SYSTEM_MD5 */
/* #undef GDCM_USE_SYSTEM_EXPAT */
/* #undef GDCM_USE_SYSTEM_LJPEG */
/* #undef GDCM_USE_SYSTEM_OPENJPEG */
/* #undef GDCM_USE_SYSTEM_KAKADU */
/* #undef GDCM_USE_SYSTEM_PVRG */

#ifndef GDCM_USE_SYSTEM_OPENJPEG
#define OPENJPEG_MAJOR_VERSION 1
#else
#define OPENJPEG_MAJOR_VERSION 
#endif //GDCM_USE_SYSTEM_OPENJPEG

#ifndef OPENJPEG_MAJOR_VERSION
#error problem with openjpeg major version
#endif // OPENJPEG_MAJOR_VERSION

/* #undef GDCM_USE_PVRG */
/* #undef GDCM_USE_KAKADU */
#define GDCM_USE_JPEGLS

/* For older gcc / broken plateform */
/* #undef GDCM_NO_ANSI_STRING_STREAM */

/* I guess something important */
#define GDCM_HAVE_STDINT_H
/* #undef GDCM_HAVE_INTTYPES_H */

/* This variable allows you to have helpful debug statement */
/* That are in between #ifdef / endif in the gdcm code */
/* That means if GDCM_DEBUG is OFF there shouldn't be any 'cout' at all ! */
/* only cerr, for instance 'invalid file' will be allowed */
/* #undef GDCM_DEBUG */

#define GDCM_CMAKE_INSTALL_PREFIX "C:/Program Files/GDCM 2.0"
#define GDCM_INSTALL_INCLUDE_DIR "include/gdcm-2.0"
#define GDCM_INSTALL_DATA_DIR "share/gdcm-2.0"

/* Whether we are building shared libraries.  */
#define GDCM_BUILD_SHARED_LIBS

/* GDCM uses __FUNCTION__ which is not ANSI C, but C99 */
/* #undef GDCM_CXX_HAS_FUNCTION */

#define GDCM_SIZEOF_LONG 

/* Special time structure support */
/* #undef GDCM_HAVE_SYS_TIME_H */
#define GDCM_HAVE_WINSOCK_H
/* #undef GDCM_HAVE_BYTESWAP_H */
#define GDCM_HAVE_RPC_H
// CMS with PBE (added in OpenSSL 1.0.0 ~ Fri Nov 27 15:33:25 CET 2009)
/* #undef GDCM_HAVE_CMS_RECIPIENT_PASSWORD */

/* #undef GDCM_HAVE_STRCASECMP */
/* #undef GDCM_HAVE_STRNCASECMP */
/* #undef GDCM_HAVE_SNPRINTF */
#define GDCM_HAVE__STRICMP
#define GDCM_HAVE__STRNICMP
#define GDCM_HAVE__SNPRINTF
/* #undef GDCM_HAVE_GETTIMEOFDAY */

// MM: I have a feeling that if GDCM_HAVE_WCHAR_IFSTREAM, then UNICODE filename 
// are expected to be specified as UTF-16, but if no API exist for UTF-16
// at std::ifstream level, then const char* of std::ifstream might accept
// UTF-8
#define GDCM_HAVE_WCHAR_IFSTREAM

/* #undef GDCM_FORCE_BIGENDIAN_EMULATION */

/* To Remove code that support broken DICOM implementation and therefore
 * add some over head. Turn Off at your own risk 
 */
#define GDCM_SUPPORT_BROKEN_IMPLEMENTATION

#define GDCM_PVRG_JPEG_EXECUTABLE ""
#define GDCM_KAKADU_EXPAND_EXECUTABLE ""

/* Undocumented on purpose :-P */
/* #undef GDCM_WRITE_ODD_LENGTH */
#ifdef GDCM_WRITE_ODD_LENGTH
#error do not use !
#endif

/*--------------------------------------------------------------------------*/
/* GDCM Versioning                                                          */

/* Version number.  */
#define GDCM_MAJOR_VERSION 2
#define GDCM_MINOR_VERSION 0
#define GDCM_BUILD_VERSION 16
#define GDCM_VERSION "2.0.16"

/*
#define GDCM_FILE_META_INFORMATION_VERSION "\0\1"
// echo "gdcm" | od -b
#define GDCM_IMPLEMENTATION_CLASS_UID "107.104.103.115";
#define GDCM_IMPLEMENTATION_VERSION_NAME "GDCM " GDCM_VERSION
#define GDCM_SOURCE_APPLICATION_ENTITY_TITLE "GDCM"
*/


/*--------------------------------------------------------------------------*/
/* GDCM deprecation mechanism                                               */
/* #undef GDCM_LEGACY_REMOVE */
/* #undef GDCM_LEGACY_SILENT */


#endif
