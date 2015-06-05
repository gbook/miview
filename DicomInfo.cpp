/* ----------------------------------------------------------------------------
	DicomInfo.cpp


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

#include "DicomInfo.h"

#include "wx/dir.h"
#include "wx/File.h"


// --------------------------------------------------------
// ---------- DicomInfo -----------------------------------
// --------------------------------------------------------
DicomInfo::DicomInfo()
{
	//logWindow = &logwind;
	InitializeVariables();
}


// --------------------------------------------------------
// ---------- ~DicomInfo ----------------------------------
// --------------------------------------------------------
DicomInfo::~DicomInfo(void)
{
}


// --------------------------------------------------------
// ---------- InitializeVariables -------------------------
// --------------------------------------------------------
void DicomInfo::InitializeVariables()
{
	WriteLog("DicomInfo->InitializeVariables() called...");

	rowCosines.SetXYZ(0.0,0.0,0.0);
	colCosines.SetXYZ(0.0,0.0,0.0);
	normCosines.SetXYZ(0.0,0.0,0.0);

	/* initialize orientation */
//	orientation.highLabelX = "?";
//	orientation.highLabelY = "?";
//	orientation.highLabelZ = "?";
//	orientation.lowLabelX = "?";
//	orientation.lowLabelY = "?";
//	orientation.lowLabelZ = "?";
//	orientation.isOblique = false;
//	orientation.reverseX = false;
//	orientation.reverseY = false;
//	orientation.reverseZ = false;

}


/* the Get functions */
wxString DicomInfo::GetModality() { return modality; }
wxString DicomInfo::GetModalityDescription() { return modalityDescription; }
XYZTriplet DicomInfo::GetRowCosines() { return rowCosines; }
XYZTriplet DicomInfo::GetColCosines() { return colCosines; }
XYZTriplet DicomInfo::GetNormCosines() { return normCosines; }
wxString DicomInfo::GetPatientName() { return patientName; }
wxString DicomInfo::GetPatientID() { return patientID; }
wxString DicomInfo::GetPatientBirthdate() { return patientBirthdate; }
wxString DicomInfo::GetPatientSex() { return patientSex; }
wxString DicomInfo::GetInstitutionName() { return institutionName; }
wxString DicomInfo::GetStudyDate() { return studyDate; }
wxString DicomInfo::GetSeriesDate() { return seriesDate; }
wxString DicomInfo::GetStudyTime() { return studyTime; }
wxString DicomInfo::GetSeriesTime() { return seriesTime; }
wxString DicomInfo::GetStudyDescription() { return studyDescription; }
wxString DicomInfo::GetSeriesDescription() { return seriesDescription; }
wxString DicomInfo::GetPerformingPhysician() { return performingPhysician; }
wxString DicomInfo::GetProtocolName() { return protocolName; }
wxString DicomInfo::GetPatientPositionStr() { return patientPositionStr; }
int DicomInfo::GetPatientPosition() { return patientPosition; }
wxDateTime DicomInfo::GetStudyDateTime() { return studyDateTime; }
wxDateTime DicomInfo::GetSeriesDateTime() { return seriesDateTime; }
int DicomInfo::GetSwapCode() { return swapCode; }

/* the Set functions */
void DicomInfo::SetColCosines(XYZTriplet value) { colCosines = value; }
void DicomInfo::SetNormCosines(XYZTriplet value) { normCosines = value; }
void DicomInfo::SetInstitutionName(wxString value) { institutionName = value; }
void DicomInfo::SetModality(wxString value) { modality = value; }
void DicomInfo::SetModalityDescription(wxString value) { modalityDescription = value; }
void DicomInfo::SetPatientBirthdate(wxString value) { patientBirthdate = value; }
void DicomInfo::SetPatientSex(wxString value) { patientSex = value; }
void DicomInfo::SetPatientID(wxString value) { patientID = value; }
void DicomInfo::SetPatientName(wxString value) { patientName = value; }
void DicomInfo::SetPatientPosition(int value) { patientPosition = value; }
void DicomInfo::SetPatientPositionStr(wxString value) { patientPositionStr = value; }
void DicomInfo::SetPerformingPhysician(wxString value) { performingPhysician = value; }
void DicomInfo::SetProtocolName(wxString value) { protocolName = value; }
void DicomInfo::SetRowCosines(XYZTriplet value) { rowCosines = value; }
void DicomInfo::SetSeriesDate(wxString value) { seriesDate = value; }
void DicomInfo::SetSeriesDateTime(wxDateTime value) { seriesDateTime = value; }
void DicomInfo::SetSeriesDescription(wxString value) { seriesDescription = value; }
void DicomInfo::SetSeriesTime(wxString value) { seriesTime = value; }
void DicomInfo::SetStudyDate(wxString value) { studyDate = value; }
void DicomInfo::SetStudyDateTime(wxDateTime value) { studyDateTime = value; }
void DicomInfo::SetStudyDescription(wxString value) { studyDescription = value; }
void DicomInfo::SetStudyTime(wxString value) { studyTime = value; }
void DicomInfo::SetSwapCode(int value) { swapCode = value; }
