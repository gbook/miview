/* ----------------------------------------------------------------------------
	Color.cpp


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


#include "Color.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Color::Color()
{
	r = g = b = gray = 0.0;
	isNormalized = false;
}

Color::Color(double red, double green, double blue, double g) {
	r = red;
	g = green;
	b = blue;
	gray = g;
}

Color::~Color()
{

}


// --------------------------------------------------------
// ---------- SetRGB --------------------------------------
// --------------------------------------------------------
void Color::SetRGB(double red, double green, double blue)
{
	r = red;
	g = green;
	b = blue;
}


// --------------------------------------------------------
// ---------- SetGray -------------------------------------
// --------------------------------------------------------
void Color::SetGray(double g)
{
	gray = g;
}


// --------------------------------------------------------
// ---------- Red -----------------------------------------
// --------------------------------------------------------
double Color::Red()
{
	return r;
}


// --------------------------------------------------------
// ---------- Green ---------------------------------------
// --------------------------------------------------------
double Color::Green()
{
	return g;
}


// --------------------------------------------------------
// ---------- Blue ----------------------------------------
// --------------------------------------------------------
double Color::Blue()
{
	return b;
}


// --------------------------------------------------------
// ---------- Gray ----------------------------------------
// --------------------------------------------------------
double Color::Gray()
{
	return gray;
}


// --------------------------------------------------------
// ---------- SetNormalized -------------------------------
// --------------------------------------------------------
void Color::SetNormalized(bool isNorm)
{
	isNormalized = isNorm;
}


// --------------------------------------------------------
// ---------- IsNormalized --------------------------------
// --------------------------------------------------------
bool Color::IsNormalized()
{
	return isNormalized;
}
