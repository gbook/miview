/* ----------------------------------------------------------------------------
	XYZTriplet.cpp


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

#include "XYZTriplet.h"

XYZTriplet::XYZTriplet(void)
{
	x = y = z = -1.0;
}

XYZTriplet::XYZTriplet(double X, double Y, double Z)
{
	x = X;
	y = Y;
	z = Z;
}

XYZTriplet::~XYZTriplet(void)
{
}

void XYZTriplet::SetXYZ(double X, double Y, double Z)
{
	x = X;
	y = Y;
	z = Z;
}
