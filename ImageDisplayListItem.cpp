/* ----------------------------------------------------------------------------
	ImageDisplayListItem.cpp


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
#include "ImageDisplayListItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImageDisplayListItem::ImageDisplayListItem(int m_plane, int m_slice, int m_zoom, wxSize m_screenSize, wxRealPoint m_centerOffset, wxRealPoint m_bottomLeft, int m_zOrder, bool m_hiRes, int m_alpha, bool m_isStack, bool m_is3ViewSlice, XYZTriplet m_minminmin, XYZTriplet m_maxmaxmax )
{
	plane = m_plane;
	slice = m_slice;
	zoom = m_zoom;
	screenSize = m_screenSize;
	centerOffset = m_centerOffset;
	bottomLeft = m_bottomLeft;
	hiRes = m_hiRes;
	zOrder = m_zOrder;
	alpha = m_alpha;
	isStack = m_isStack;
	is3ViewSlice = m_is3ViewSlice;
	minminmin = m_minminmin;
	maxmaxmax = m_maxmaxmax;
}

ImageDisplayListItem::~ImageDisplayListItem()
{

}