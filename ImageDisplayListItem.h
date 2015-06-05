/* ----------------------------------------------------------------------------
	ImageDisplayListItem.h


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

class ImageDisplayListItem  
{
public:
	ImageDisplayListItem(int m_plane, int m_slice, int m_zoom, wxSize m_screenSize, wxRealPoint m_centerOffset, wxRealPoint m_bottomLeft, int m_zOrder, bool m_hiRes, int m_alpha, bool m_isStack,	bool is3ViewSlice, XYZTriplet minminmin = XYZTriplet(0,0,0), XYZTriplet maxmaxmax = XYZTriplet(0,0,0));
	virtual ~ImageDisplayListItem();

	int plane;
	int slice;
	int zoom;
	wxSize screenSize;
	wxRealPoint centerOffset;
	wxRealPoint bottomLeft;
	bool hiRes;
	int zOrder;
	int alpha;
	bool isStack;
	bool is3ViewSlice;
	XYZTriplet minminmin;
	XYZTriplet maxmaxmax;
};
