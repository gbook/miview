/* ----------------------------------------------------------------------------
	ImageSliceThread.h


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


//class LogWindow;
class ImageData;

class ImageSliceThread: public wxThread
{
public:
	ImageSliceThread(ImageData *imageData, int tnum, wxMutex *mutex, wxCondition *condition);
	virtual void *Entry();
	virtual ~ImageSliceThread();

	void SetupThread(int datatype, void *tempImage, int bufferSize, wxSize originalSize, wxSize textureSize, int plane, int slice, bool returnRGB, int volumeNum, double minVal);
	
private:
	void PopulateBufferUINT8();
	void PopulateBufferINT8();
	void PopulateBufferUINT16();
	void PopulateBufferINT16();
	void PopulateBufferUINT32();
	void PopulateBufferINT32();
	void PopulateBufferFLOAT32();
	void PopulateBufferRGB();

	wxCondition *m_condition;
    wxMutex *m_mutex;

	//LogWindow *logWindow;
	ImageData *imgData;
	void *tmpImage;
	int threadnum;
	int imgtype;

	int m_bufferSize;
	wxSize m_originalSize;
	wxSize m_textureSize;
	int m_plane;
	int m_slice;
	bool m_retRGB;
//	int m_alpha;
	double m_minVal;
	int m_volume;

	/* drawing buffers */
	unsigned char		*drawBuffer8U;
	char				*drawBuffer8S;
	unsigned short int	*drawBuffer16U;
	short int			*drawBuffer16S;
	unsigned int		*drawBuffer32U;
	int					*drawBuffer32S;
	float				*drawBufferFLOAT32;
	unsigned char		*drawBufferRGB;
};
