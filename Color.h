/* ----------------------------------------------------------------------------
	Color.h


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

class Color  
{
public:
	bool IsNormalized();
	void SetNormalized(bool isNorm);
	double Blue();
	double Green();
	double Red();
	double Gray();
	void SetRGB(double r, double g, double b);
	void SetGray(double g);

	/* constructors */
	Color();
	Color(double red, double green, double blue, double g);
	virtual ~Color();

private:
	double r, g, b, gray;
	bool isNormalized; /* true for being normalized to between 0 and 1 */
};

