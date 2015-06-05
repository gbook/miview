/* ----------------------------------------------------------------------------
	MainFrameVariables.h


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

#include "MainFrame.h"

class MainFrameVariables
{
public:
	MainFrameVariables(void);
	~MainFrameVariables(void);

	/* variable access/assignment functions */

	/* ----- toolbar variables ----- */
	int  Toolbar_displayType();
	void Toolbar_displayType(int value);
	bool Toolbar_showTextOverlay();
	void Toolbar_showTextOverlay(bool value);
	bool Toolbar_showLineOverlay();
	void Toolbar_showLineOverlay(bool value);
	bool Toolbar_showHighRes();
	void Toolbar_showHighRes(bool value);
	
	int  Slice_value();
	void Slice_value(int value);
	
	/* ----- image variables ----- */
	bool Image_zoomUseBestFit();
	void Image_zoomUseBestFit(bool value);
	int  Image_zoomPercent();
	void Image_zoomPercent(int value);
	int  Image_plane();
	void Image_plane(int value);
	int  Image_volume();
	void Image_volume(int value);
	bool Image_trackPoints();
	void Image_trackPoints(bool value);
	bool Image_displayTrackedPoints();
	void Image_displayTrackedPoints(bool value);
	
	/* ----- window center/width variables ----- */
	int  Display_windowCenter();
	void Display_windowCenter(int value);
	int  Display_windowWidth();
	void Display_windowWidth(int value);
	
	/* ----- multislice variables */
	int  Multislice_sortBy();
	void Multislice_sortBy(int value);
	int  Multislice_numRows();
	void Multislice_numRows(int value);
	int  Multislice_numCols();
	void Multislice_numCols(int value);
	bool Multislice_useBestFit();
	void Multislice_useBestFit(bool value);
	
	/* ----- colormap variables ----- */
	bool ColorMap_useColor();
	void ColorMap_useColor(bool value);
	wxColour ColorMap_color1();
	void     ColorMap_color1(wxColour value);
	wxColour ColorMap_color2();
	void     ColorMap_color2(wxColour value);
	int  ColorMap_index();
	void ColorMap_index(int value);
	bool ColorMap_useWindowLevel();
	void ColorMap_useWindowLevel(bool value);

	/* ----- volume rendering variables ----- */
	double VolRend_intensity();
	void   VolRend_intensity(double value);
	double VolRend_density();
	void   VolRend_density(double value);
	int   VolRend_numSlices();
	void  VolRend_numSlices(int value);
	bool VolRend_useColor();
	void VolRend_useColor(bool value);
	wxColour VolRend_color();
	void     VolRend_color(wxColour value);
	int  VolRend_method();
	void VolRend_method(int value);
	double VolRend_stepSize();
	void  VolRend_stepSize(double value);
	bool VolRend_isMoving();
	void VolRend_isMoving(bool value);
	int  VolRend_numIterations();
	void VolRend_numIterations(int value);
	bool VolRend_reInit();
	void VolRend_reInit(bool value);
	bool VolRend_useSSD();
	void VolRend_useSSD(bool value);
	double VolRend_SSDCutoff();
	void VolRend_SSDCutoff(double value);

	/* ----- rotation variables ----- */
	double Rotations_angle();
	void   Rotations_angle(double value);
	double Rotations_X();
	void   Rotations_X(double value);
	double Rotations_Y();
	void   Rotations_Y(double value);
	double Rotations_Z();
	void   Rotations_Z(double value);
	bool Rotations_isMoving();
	void Rotations_isMoving(bool value);
	
	/* ----- load variables ----- */
	int  Load_loadType();
	void Load_loadType(int value);
	int  Load_filetype();
	void Load_filetype(int value);
	bool Load_isMosaic();
	void Load_isMosaic(bool value);
	wxArrayString Load_filenames();
	void          Load_filenames(wxArrayString value);
	int  Load_mosaicXSize();
	void Load_mosaicXSize(int value);
	int  Load_mosaicYSize();
	void Load_mosaicYSize(int value);
	int  Load_mosaicNumSlices();
	void Load_mosaicNumSlices(int value);

	void SetCurrentView(int view);
	int  GetCurrentView();
private:
	/* for multiple view types */
	int viewID;

	/* the variables to be held */
	struct viewInfo {
		int		toolbar_displayType;
		bool	toolbar_showTextOverlay;
		bool	toolbar_showLineOverlay;
		bool	toolbar_showHighRes;
		int		slice_value;
		bool	image_zoomUseBestFit;
		int		image_zoomPercent;
		int		image_plane;
		int		image_volume;
		bool	image_trackPoints;
		bool	image_displayTrackedPoints;
		int		display_windowCenter;
		int		display_windowWidth;
		int		multislice_sortBy;
		int		multislice_numRows;
		int		multislice_numCols;
		bool	multislice_useBestFit;
		bool	colorMap_useColor;
		wxColour colorMap_color1;
		wxColour colorMap_color2;
		int colorMap_index;
		bool	colorMap_useWindowLevel;
		double	volRend_intensity;
		double	volRend_density;
		int		volRend_numSlices;
		bool	volRend_useColor;
		wxColour volRend_color;
		int		volRend_method;
		double	volRend_stepSize;
		bool	volRend_isMoving;
		int		volRend_numIterations;
		bool	volRend_reInit;
		bool	volRend_useSSD;
		double	volRend_SSDCutoff;
		double	rotations_angle;
		double	rotations_X;
		double	rotations_Y;
		double	rotations_Z;
		bool	rotations_isMoving;
		int		load_loadType;
		int		load_filetype;
		bool	load_isMosaic;
		wxArrayString load_filenames;
		int		load_mosaicXSize;
		int		load_mosaicYSize;
		int		load_mosaicNumSlices;
	} views[4];

};
