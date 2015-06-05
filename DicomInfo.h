/* ----------------------------------------------------------------------------
	DicomInfo.h


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

#include <wx/datetime.h>

class XYZTriplet;

class DicomInfo
{
public:
	DicomInfo();
	~DicomInfo(void);

	wxString GetModality();
	wxString GetModalityDescription();
	XYZTriplet GetRowCosines();
	XYZTriplet GetColCosines();
	XYZTriplet GetNormCosines();
	wxString GetPatientName();
	wxString GetPatientID();
	wxString GetPatientBirthdate();
	wxString GetPatientSex();
	wxString GetInstitutionName();
	wxString GetStudyDate();
	wxString GetSeriesDate();
	wxString GetStudyTime();
	wxString GetSeriesTime();
	wxString GetStudyDescription();
	wxString GetSeriesDescription();
	wxString GetPerformingPhysician();
	wxString GetProtocolName();
	wxString GetPatientPositionStr();
	int GetPatientPosition();
	wxDateTime GetStudyDateTime();
	wxDateTime GetSeriesDateTime();
	XYZTriplet GetOrigin();
	XYZTriplet GetSpacing();
	int GetSwapCode();

	void SetColCosines(XYZTriplet value);
	void SetNormCosines(XYZTriplet value);
	void SetInstitutionName(wxString value);
	void SetModality(wxString value);
	void SetModalityDescription(wxString value);
	void SetPatientBirthdate(wxString value);
	void SetPatientSex(wxString value);
	void SetPatientID(wxString value);
	void SetPatientName(wxString value);
	void SetPatientPosition(int value);
	void SetPatientPositionStr(wxString value);
	void SetPerformingPhysician(wxString value);
	void SetProtocolName(wxString value);
	void SetRowCosines(XYZTriplet value);
	void SetSeriesDate(wxString value);
	void SetSeriesDateTime(wxDateTime value);
	void SetSeriesDescription(wxString value);
	void SetSeriesTime(wxString value);
	void SetStudyDate(wxString value);
	void SetStudyDateTime(wxDateTime value);
	void SetStudyDescription(wxString value);
	void SetStudyTime(wxString value);
	void SetSwapCode(int value);

	///* this structure contains the interpreted orientation information from the
	//   patient position, and image orientation patient dicom fields */
	//struct structOrientation {
	//	bool isOblique; /* if the cosines are anything but -1,0,1 then the orientation can't
	//							   be reliably determined (yet)... without some work */
	//	/* possible side labels are:
	//	   H - head  *or*  S - superior  (preference is in Options screen, H is default)
	//	   F - foot  *or*  I - inferior  (preference is in Options screen, I is default)
	//	   R - right
	//	   L - left
	//	   A - anterior (frontside)
	//	   P - posterior (backside) */
	//	/* data is always stored as it is read in, labels are assigned accordingly
	//	   some data may need to be flipped when viewing, but labels will still be
	//	   correct */
	//	wxString lowLabelX; /* label on the low side of the X side */
	//	wxString lowLabelY;
	//	wxString lowLabelZ;
	//	wxString highLabelX;
	//	wxString highLabelY;
	//	wxString highLabelZ;
	//	bool reverseX;
	//	bool reverseY;
	//	bool reverseZ;
	//	wxString imagePlane;
	//	/* row and column directions can help to load the image data into the correct area
	//	   of the 3D image array. it makes determining the side labels easier */
	//	wxString rowDirection; /* can be something like:  +Y  -Z  +X  */
	//	wxString colDirection; /* can be something like:  +Y  -Z  +X  */
	//} orientation;

private:
	//LogWindow *logWindow;
	void InitializeVariables();

	wxString modality;
	wxString modalityDescription;
	wxString PixelTypeStr;  /* can be: 8U, 8S, 16U, 16S, 32U, 32S, FD */

	XYZTriplet rowCosines;
	XYZTriplet colCosines;
	XYZTriplet normCosines;

	wxString patientName;         /* (0010,0010) */
	wxString patientID;           /* (0010,0020) */
	wxString patientBirthdate;    /* (0010,0030) */
	wxString patientSex;		  /* (0010,0040) */
	wxString institutionName;     /* (0008,0080) */
	wxString studyDate;           /* (0008,0020) */
	wxString seriesDate;          /* (0008,0021) */
	wxString studyTime;           /* (0008,0030) */
	wxString seriesTime;          /* (0008,0031) */
	wxString studyDescription;    /* (0008,1030) */
	wxString seriesDescription;   /* (0008,103E) */
	wxString performingPhysician; /* (0008,1050) */
	wxString protocolName;        /* (0018,0030) */
	wxString patientPositionStr;  /* (0018,5100) */
	int patientPosition; /* one of the enumerated position types HFS, FFS, etc */

	wxDateTime studyDateTime;
	wxDateTime seriesDateTime;

	int swapCode;
};
