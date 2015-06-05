/* ----------------------------------------------------------------------------
	DataContainers.h


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


/* ----------------------- generic X*Y*Z*T*W buffer ------------------------ */
template <class T>
class DataBuffer {
public:
	DataBuffer(int ySize, int xSize, int zSize, int tSize, int wSize) : p_( new T[ySize*xSize*zSize*tSize*wSize] )
		, ySize_(ySize), xSize_(xSize), zSize_(zSize), tSize_(tSize), wSize_(wSize) { }

	~DataBuffer() { delete[] p_; }
	long int NumBytes() { return xSize_*ySize_*zSize_*tSize_*wSize_*sizeof(T); }

	T& Get(int y,int x,int z,int t, int w)       { return p_[(((y*xSize_*wSize_)+(x*wSize_)+w)*zSize_+z)*tSize_+t]; }
	T  Get(int y,int x,int z,int t, int w) const { return p_[(((y*xSize_*wSize_)+(x*wSize_)+w)*zSize_+z)*tSize_+t]; }
	void Set(int y, int x, int z, int t, int w, T value) { p_[(((y*xSize_*wSize_)+(x*wSize_)+w)*zSize_+z)*tSize_+t] = value; }
	void *GetDataPointer() { return p_; }
private:
	T* p_;
	int xSize_,ySize_,zSize_,tSize_,wSize_;
	DataBuffer(DataBuffer const&); //disabled copy-ctr
	void operator=(DataBuffer const&); //disabled copy-assign
};
