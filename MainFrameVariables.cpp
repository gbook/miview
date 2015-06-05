/* ----------------------------------------------------------------------------
	MainFrameVariables.cpp


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



#include "MainFrameVariables.h"

MainFrameVariables::MainFrameVariables(void)
{
	viewID = 0; /* default viewID */

	int i;
	for (i=0; i<4; i++) {
		views[i].toolbar_displayType = IMGVIEW_SINGLEPLANE;
		views[i].toolbar_showTextOverlay = true;
		views[i].toolbar_showLineOverlay = true;
		views[i].toolbar_showHighRes = true;
		views[i].slice_value = 0;
		views[i].image_zoomUseBestFit = true;
		views[i].image_zoomPercent = 100;
		views[i].image_plane = 0;
		views[i].image_volume = 0;
		views[i].image_trackPoints = false;
		views[i].image_displayTrackedPoints = true;
		views[i].display_windowCenter = 0;
		views[i].display_windowWidth = 0;
		views[i].multislice_sortBy = ROWS;
		views[i].multislice_numRows = 1;
		views[i].multislice_numCols = 1;
		views[i].multislice_useBestFit = true;
		views[i].colorMap_useColor = false;
		views[i].colorMap_color1 = wxColour::wxColour(64, 0, 0);
		views[i].colorMap_color2 = wxColour::wxColour(255, 255, 128);
		views[i].colorMap_index = 0;
		views[i].colorMap_useWindowLevel = true;
		views[i].volRend_intensity = 0.5;
		views[i].volRend_density = 0.5;
		views[i].volRend_numSlices = 50;
		views[i].volRend_useColor = false;
		views[i].volRend_color = wxColour::wxColour(255,255,255);
//		views[i].volRend_method = VOLMETHOD_3DTEXTURE;
		views[i].volRend_method = VOLMETHOD_RAYTRACE;
		views[i].volRend_stepSize = 0.05;
		views[i].volRend_isMoving = false;
		views[i].volRend_numIterations = 80;
		views[i].volRend_reInit = false;
		views[i].volRend_useSSD = false;
		views[i].volRend_SSDCutoff = 0.001;
		views[i].rotations_angle = 0.0;
		views[i].rotations_X = 0.0;
		views[i].rotations_Y = 0.0;
		views[i].rotations_Z = 0.0;
		views[i].rotations_isMoving = false;
		views[i].load_filenames = wxArrayString(NULL);
		views[i].load_filetype = FILETYPE_DICOM;
		views[i].load_isMosaic = false;
		views[i].load_loadType = OPEN_SELECTED;
		views[i].load_mosaicNumSlices = 36;
		views[i].load_mosaicXSize = 64;
		views[i].load_mosaicYSize = 64;
	}
}

MainFrameVariables::~MainFrameVariables(void)
{
}

void MainFrameVariables::SetCurrentView(int view)
{
	/* make sure the viewID is within the correct range */
	if ((view > 0) && (view < 4))
		viewID = view;
}

/* functions for access to the variables... overloaded, so only one function name is needed */
int MainFrameVariables::Toolbar_displayType() { return views[viewID].toolbar_displayType; }
void MainFrameVariables::Toolbar_displayType(int value) { views[viewID].toolbar_displayType = value; }

bool MainFrameVariables::Toolbar_showTextOverlay() { return views[viewID].toolbar_showTextOverlay; }
void MainFrameVariables::Toolbar_showTextOverlay(bool value) { views[viewID].toolbar_showTextOverlay = value; }

bool MainFrameVariables::Toolbar_showLineOverlay() { return views[viewID].toolbar_showLineOverlay; }
void MainFrameVariables::Toolbar_showLineOverlay(bool value) { views[viewID].toolbar_showLineOverlay = value; }

bool MainFrameVariables::Toolbar_showHighRes() { return views[viewID].toolbar_showHighRes; }
void MainFrameVariables::Toolbar_showHighRes(bool value) { views[viewID].toolbar_showHighRes = value; }

int MainFrameVariables::Slice_value() { return views[viewID].slice_value; }
void MainFrameVariables::Slice_value(int value) { views[viewID].slice_value = value; }

bool MainFrameVariables::Image_zoomUseBestFit() { return views[viewID].image_zoomUseBestFit; }
void MainFrameVariables::Image_zoomUseBestFit(bool value) { views[viewID].image_zoomUseBestFit = value; }

int MainFrameVariables::Image_zoomPercent() { return views[viewID].image_zoomPercent; }
void MainFrameVariables::Image_zoomPercent(int value) { views[viewID].image_zoomPercent = value; }

int MainFrameVariables::Image_plane() { return views[viewID].image_plane; }
void MainFrameVariables::Image_plane(int value) { views[viewID].image_plane = value; }

int MainFrameVariables::Image_volume() { return views[viewID].image_volume; }
void MainFrameVariables::Image_volume(int value) { views[viewID].image_volume = value; }

bool MainFrameVariables::Image_trackPoints() { return views[viewID].image_trackPoints; }
void MainFrameVariables::Image_trackPoints(bool value) { views[viewID].image_trackPoints = value; }

bool MainFrameVariables::Image_displayTrackedPoints() { return views[viewID].image_displayTrackedPoints; }
void MainFrameVariables::Image_displayTrackedPoints(bool value) { views[viewID].image_displayTrackedPoints = value; }

int MainFrameVariables::Display_windowCenter() { return views[viewID].display_windowCenter; }
void MainFrameVariables::Display_windowCenter(int value) { views[viewID].display_windowCenter = value; }

int MainFrameVariables::Display_windowWidth() { return views[viewID].display_windowWidth; }
void MainFrameVariables::Display_windowWidth(int value) { views[viewID].display_windowWidth = value; }

int MainFrameVariables::Multislice_sortBy() { return views[viewID].multislice_sortBy; }
void MainFrameVariables::Multislice_sortBy(int value) { views[viewID].multislice_sortBy = value; }

int MainFrameVariables::Multislice_numRows() { return views[viewID].multislice_numRows; }
void MainFrameVariables::Multislice_numRows(int value) { views[viewID].multislice_numRows = value; }

int MainFrameVariables::Multislice_numCols() { return views[viewID].multislice_numCols; }
void MainFrameVariables::Multislice_numCols(int value) { views[viewID].multislice_numCols = value; }

bool MainFrameVariables::Multislice_useBestFit() { return views[viewID].multislice_useBestFit; }
void MainFrameVariables::Multislice_useBestFit(bool value) { views[viewID].multislice_useBestFit = value; }

bool MainFrameVariables::ColorMap_useColor() { return views[viewID].colorMap_useColor; }
void MainFrameVariables::ColorMap_useColor(bool value) { views[viewID].colorMap_useColor = value; }

wxColour MainFrameVariables::ColorMap_color1() { return views[viewID].colorMap_color1; }
void MainFrameVariables::ColorMap_color1(wxColour value) { views[viewID].colorMap_color1 = value; }

wxColour MainFrameVariables::ColorMap_color2() { return views[viewID].colorMap_color2; }
void MainFrameVariables::ColorMap_color2(wxColour value) { views[viewID].colorMap_color2 = value; }

int MainFrameVariables::ColorMap_index() { return views[viewID].colorMap_index; }
void MainFrameVariables::ColorMap_index(int value) { views[viewID].colorMap_index = value; }

bool MainFrameVariables::ColorMap_useWindowLevel() { return views[viewID].colorMap_useWindowLevel; }
void MainFrameVariables::ColorMap_useWindowLevel(bool value) { views[viewID].colorMap_useWindowLevel = value; }

double MainFrameVariables::VolRend_intensity() { return views[viewID].volRend_intensity; }
void MainFrameVariables::VolRend_intensity(double value) { views[viewID].volRend_intensity = value; }

double MainFrameVariables::VolRend_density() { return views[viewID].volRend_density; }
void MainFrameVariables::VolRend_density(double value) { views[viewID].volRend_density = value; }

int  MainFrameVariables::VolRend_numSlices() { return views[viewID].volRend_numSlices; }
void  MainFrameVariables::VolRend_numSlices(int value) { views[viewID].volRend_numSlices = value; }

bool MainFrameVariables::VolRend_useColor() { return views[viewID].volRend_useColor; }
void MainFrameVariables::VolRend_useColor(bool value) { views[viewID].volRend_useColor = value; }

wxColour MainFrameVariables::VolRend_color() { return views[viewID].volRend_color; }
void MainFrameVariables::VolRend_color(wxColour value) { views[viewID].volRend_color = value; }

int MainFrameVariables::VolRend_method() { return views[viewID].volRend_method; }
void MainFrameVariables::VolRend_method(int value) { views[viewID].volRend_method = value; }

double MainFrameVariables::VolRend_stepSize() { return views[viewID].volRend_stepSize; }
void MainFrameVariables::VolRend_stepSize(double value) { views[viewID].volRend_stepSize = value; }

bool MainFrameVariables::VolRend_isMoving() { return views[viewID].volRend_isMoving; }
void MainFrameVariables::VolRend_isMoving(bool value) { views[viewID].volRend_isMoving = value; }

int MainFrameVariables::VolRend_numIterations() { return views[viewID].volRend_numIterations; }
void MainFrameVariables::VolRend_numIterations(int value) { views[viewID].volRend_numIterations = value; }

bool MainFrameVariables::VolRend_reInit() { return views[viewID].volRend_reInit; }
void MainFrameVariables::VolRend_reInit(bool value) { views[viewID].volRend_reInit = value; }

bool MainFrameVariables::VolRend_useSSD() { return views[viewID].volRend_useSSD; }
void MainFrameVariables::VolRend_useSSD(bool value) { views[viewID].volRend_useSSD = value; }

double MainFrameVariables::VolRend_SSDCutoff() { return views[viewID].volRend_SSDCutoff; }
void MainFrameVariables::VolRend_SSDCutoff(double value) { views[viewID].volRend_SSDCutoff = value; }

double MainFrameVariables::Rotations_angle() { return views[viewID].rotations_angle; }
void MainFrameVariables::Rotations_angle(double value) { views[viewID].rotations_angle = value; }

double MainFrameVariables::Rotations_X() { return views[viewID].rotations_X; }
void MainFrameVariables::Rotations_X(double value) { views[viewID].rotations_X = value; }

double MainFrameVariables::Rotations_Y() { return views[viewID].rotations_Y; }
void MainFrameVariables::Rotations_Y(double value) { views[viewID].rotations_Y = value; }

double MainFrameVariables::Rotations_Z() { return views[viewID].rotations_Z; }
void MainFrameVariables::Rotations_Z(double value) { views[viewID].rotations_Z = value; }

bool MainFrameVariables::Rotations_isMoving() { return views[viewID].rotations_isMoving; }
void MainFrameVariables::Rotations_isMoving(bool value) { views[viewID].rotations_isMoving = value; }

int MainFrameVariables::Load_loadType() { return views[viewID].load_loadType; }
void MainFrameVariables::Load_loadType(int value) { views[viewID].load_loadType = value; }

int MainFrameVariables::Load_filetype() { return views[viewID].load_filetype; }
void MainFrameVariables::Load_filetype(int value) { views[viewID].load_filetype = value; }

bool MainFrameVariables::Load_isMosaic() { return views[viewID].load_isMosaic; }
void MainFrameVariables::Load_isMosaic(bool value) { views[viewID].load_isMosaic = value; }

wxArrayString MainFrameVariables::Load_filenames() { return views[viewID].load_filenames; }
void MainFrameVariables::Load_filenames(wxArrayString value) { views[viewID].load_filenames = value; }

int MainFrameVariables::Load_mosaicXSize() { return views[viewID].load_mosaicXSize; }
void MainFrameVariables::Load_mosaicXSize(int value) { views[viewID].load_mosaicXSize = value; }

int MainFrameVariables::Load_mosaicYSize() { return views[viewID].load_mosaicYSize; }
void MainFrameVariables::Load_mosaicYSize(int value) { views[viewID].load_mosaicYSize = value; }

int MainFrameVariables::Load_mosaicNumSlices() { return views[viewID].load_mosaicNumSlices; }
void MainFrameVariables::Load_mosaicNumSlices(int value) { views[viewID].load_mosaicNumSlices = value; }
