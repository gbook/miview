/* ----------------------------------------------------------------------------
	MIViewBrowserDB.cpp


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



#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "MIViewBrowserDB.h"

#include "wx/arrimpl.cpp"
WX_DEFINE_OBJARRAY(arr_files);
WX_DEFINE_OBJARRAY(arr_series);
WX_DEFINE_OBJARRAY(arr_studies);
WX_DEFINE_OBJARRAY(arr_patients);

File &Series::operator [](wxString key) {
	for(long i=0; i<(int)files.Count(); i++) /* search for they key */
		if(files[i].fileName == key) /* if its there, return the object */
			return files[i];
	long idx = (int)files.Count();
	File newFile;
	newFile.fileName = key;
	files.Add(newFile);
	return files[idx]; /* otherwise add it and return the object */
} 

File &Series::operator [] (long index) {
	if(index < 0) index=0;
	return files[index];
}


Series &Study::operator [](wxString key) {
	for(long i=0; i<(int)series.Count(); i++) /* search for they key */
		if(series[i].seriesName == key) /* if its there, return the object */
			return series[i];
	long idx = (int)series.Count();
	Series newSeries;
	newSeries.seriesName = key;
	series.Add(newSeries);
	return series[idx]; /* otherwise add it and return the object */
} 
Series &Study::operator [] (long index) {
	if(index < 0) index=0;
	return series[index];
}


Study &Patient::operator [](wxString key) {
	for(long i=0; i<(int)studies.Count(); i++) /* search for they key */
		if(studies[i].studyName == key) /* if its there, return the object */
			return studies[i];
	long idx = (int)studies.Count();
	Study newStudy;
	newStudy.studyName = key;
	studies.Add(newStudy);
	return studies[idx]; /* otherwise add it and return the object */
} 
Study &Patient::operator [] (long index) {
	if(index < 0) index=0;
	return studies[index];
}


Patient &BrowserDB::operator [](wxString key) {
	for(long i=0; i<(int)patients.Count(); i++) /* search for they key */
		if(patients[i].patientName == key) /* if its there, return the object */
			return patients[i];
	long idx = (int)patients.Count();
	Patient newPatient;
	newPatient.patientName = key;
	patients.Add(newPatient);
	return patients[idx]; /* otherwise add it and return the object */
} 
Patient &BrowserDB::operator [] (long index) {
	if(index < 0) index=0;
	return patients[index];
}
