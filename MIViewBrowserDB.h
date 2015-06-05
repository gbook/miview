/* ----------------------------------------------------------------------------
	MIViewBrowserDB.h


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

#include "wx/dynarray.h"
//#include "wx/arrimpl.cpp"

/* forward declare the classes */
class Patient;
class Study;
class Series;
class File;

/* ------------ File definition ---------------- */
class File {
public:
	File() {};
	~File() {};

	wxString fileName;
	int slicenum;
};

WX_DECLARE_OBJARRAY(File,arr_files);

/* ------------ Series definition -------------- */
class Series {
public:
	Series() {};
	~Series() {};

	wxString seriesName; /* key */
	int GetNumFiles() { return (int)files.Count(); };

	File &operator [](wxString key);
	File &operator [] (long index);
	//File &operator [](wxString key) {
	//	for(long i=0; i<(int)files.Count(); i++) /* search for they key */
	//		if(files[i].fileName == key) /* if its there, return the object */
	//			return files[i];
	//	long idx = files.Count();
	//	File newFile;
	//	newFile.fileName = key;
	//	files.Add(newFile);
	//	return files[idx]; /* otherwise add it and return the object */
	//} 

	/* array of files for this series */
	arr_files files;
};

//WX_DEFINE_OBJARRAY(arr_files);
WX_DECLARE_OBJARRAY(Series,arr_series);

/* ------------ Study definition --------------- */
class Study {
public:
	Study() {};
	~Study() {};

	wxString studyName; /* key */

	int GetNumSeries() { return (int)series.Count(); };

	Series &operator [](wxString key);
	Series &operator [] (long index);
	//Series &operator [](wxString key) {
	//	for(long i=0; i<(int)series.Count(); i++) /* search for they key */
	//		if(series[i].seriesName == key) /* if its there, return the object */
	//			return series[i];
	//	long idx = series.Count();
	//	Series newSeries;
	//	newSeries.seriesName = key;
	//	series.Add(newSeries);
	//	return series[idx]; /* otherwise add it and return the object */
	//} 

	/* array of series for this study */
	arr_series series;
};

//WX_DEFINE_OBJARRAY(arr_series);
WX_DECLARE_OBJARRAY(Study,arr_studies);

/* ------------ Patient definition ------------- */
class Patient {
public:
	Patient() {};
	~Patient() {};

	wxString patientName; /* key */

	int GetNumStudies() { return (int)studies.Count(); };

	Study &operator [](wxString key);
	Study &operator [] (long index);
	//Study &operator [](wxString key) {
	//	for(long i=0; i<(int)studies.Count(); i++) /* search for they key */
	//		if(studies[i].studyName == key) /* if its there, return the object */
	//			return studies[i];
	//	long idx = studies.Count();
	//	Study newStudy;
	//	newStudy.studyName = key;
	//	studies.Add(newStudy);
	//	return studies[idx]; /* otherwise add it and return the object */
	//} 

	/* array of studies for this patient */
	arr_studies studies;
};

//WX_DEFINE_OBJARRAY(arr_studies);
WX_DECLARE_OBJARRAY(Patient,arr_patients);

/* ------------ MIViewDB definition ------------- */
class BrowserDB {
public:
	BrowserDB() {};
	~BrowserDB() {};

	wxString patientName; /* key */

	int GetNumPatients() { return (int)patients.Count(); };

	Patient &operator [](wxString key);
	Patient &operator [] (long index);

	//Patient &operator [](wxString key) {
	//	for(long i=0; i<(int)patients.Count(); i++) /* search for they key */
	//		if(patients[i].patientName == key) /* if its there, return the object */
	//			return patients[i];
	//	long idx = patients.Count();
	//	Patient newPatient;
	//	newPatient.patientName = key;
	//	patients.Add(newPatient);
	//	return patients[idx]; /* otherwise add it and return the object */
	//} 

	/* array of patients */
	arr_patients patients;
};
//WX_DEFINE_OBJARRAY(arr_patients);

