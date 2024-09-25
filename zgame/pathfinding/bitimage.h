#ifndef _BITIMAGE_H_
#define _BITIMAGE_H_

#include <stdio.h>
#include <memory.h>
#include <ABaseDef.h>
#include "StdFile.h"

#define UCHAR_BITWISE_NOT(x)  ( 0xFF ^ (x) )

// Version Info (DWORD)

// Current Version
#define BITIMAGE_VER 0x00000003	

// old   version
#define BITIMAGE_OLD_VER 0x00000001	

// magic 
#define  BITIMAGE_MAGIC   (DWORD)(('b'<<24)| ('m'<<16)|('p'<<8)|('f'))





inline void SetUCharBit(unsigned char& uch, int shift, bool bSet=true)
{
	if(bSet)
	{
		uch |= 1<<shift;
	}
	else
	{
		uch &= UCHAR_BITWISE_NOT(1<<shift);
	}
}



class CBitImage
{

public:	
	CBitImage() { m_BitImage = NULL; }
	virtual ~CBitImage() { Release(); }
	
	void Release() 
	{ 
		if(m_BitImage) 
		{
			delete [] m_BitImage; 
			m_BitImage=NULL;
		}
	}

	void Init(unsigned char* pImage, int iWidth, int iLength, unsigned char tSetBitValue, float fPixelSize = 1.0f)
	{
		// Firstly, we release myself
		Release();

		m_iWidth=iWidth >> 3;
		//int iLastBitWidth=iWidth - (m_iWidth << 3); 
		int iLastBitWidth=iWidth & 0x7; 
		if(iLastBitWidth!=0)
			m_iWidth++;

		m_iLength=iLength;

		m_fPixelSize = fPixelSize;
		m_iImageWidth = iWidth;
		m_iImageLength= iLength;

		int iImageSize = m_iWidth * m_iLength;
		m_BitImage = new unsigned char[iImageSize];
		memset(m_BitImage, 0, iImageSize);
		
		for(int i=0; i<iLength; i++)
			for(int j=0; j< iWidth; j++)
			{
				int jInChar= j >> 3;
				//int Shift = j - ( jInChar << 3 );
				int Shift = j & 0x7 ;
				bool bSet=(pImage[i*iWidth+j]==tSetBitValue)?true:false;
				SetUCharBit(m_BitImage[i*m_iWidth+ jInChar], Shift, bSet);
			}
		
	}

	inline void InitZero(int iWidth, int iLength, float fPixelSize = 1.0f);
	inline void InitNoneZero(int iWidth, int iLength, float fPixelSize = 1.0f);
	
	bool GetPixel(int u, int v) const
	{
		if (u < 0 || u >= m_iImageWidth || v < 0 || v >= m_iImageLength)
		{
			return false;
		}

		int uInChar = u >> 3;
		//int shift = u - (uInChar << 3);
		int shift = u & 0x7;
		if( m_BitImage[v*m_iWidth + uInChar] & (unsigned char)(1 << shift) )
			return true;
		else
			return false;
	}

	inline void SetPixel(int u, int v, bool bSet);


	void GetImageSize(int& width, int& length) const
	{
		width = m_iImageWidth;
		length = m_iImageLength;
	}

	float GetPixelSize() const
	{
		return m_fPixelSize;
	}
	
	inline	bool  Load(const char * szName);
	inline  bool Save(const char * szName);

private:
	inline bool   _Load(IFile& iFile);
	inline bool   _Save(IFile& iFile);
protected:
	
	int m_iWidth;			// width of the bit image (in chars)
	int m_iLength;			// length of the bit image (in chars)
	
	unsigned char *m_BitImage;
	
	float m_fPixelSize;					// Each pixel of the image is a square area
	int m_iImageWidth;			// width of the bit image ( in pixels)
	int m_iImageLength;			// length of the bit image ( in pixels)
	
};



inline void CBitImage::SetPixel(int u, int v, bool bSet)
{
	int uInChar = u >> 3;
	int shift = u & 0x7;

	SetUCharBit(m_BitImage[v*m_iWidth + uInChar], shift, bSet);
}


inline void CBitImage::InitZero(int iWidth, int iLength, float fPixelSize)
{
	// Firstly, we release myself
	Release();

	m_iWidth=iWidth >> 3;
//	int iLastBitWidth=iWidth & 0x7; 
	if(iWidth & 0x7) m_iWidth++;
	m_iLength = iLength;
	
	int iImageSize = m_iWidth * m_iLength;
	m_BitImage = new unsigned char[iImageSize];
	memset(m_BitImage, 0, iImageSize);

	m_iImageWidth = iWidth;
	m_iImageLength = iLength;
	m_fPixelSize = fPixelSize;
}

inline void CBitImage::InitNoneZero(int iWidth, int iLength, float fPixelSize)
{
	// Firstly, we release myself
	Release();
	
	//*
	m_iWidth=iWidth >> 3;
	if(iWidth & 0x7) m_iWidth++;
	m_iLength = iLength;
	
	int iImageSize = m_iWidth * m_iLength;
	m_BitImage = new unsigned char[iImageSize];
	memset(m_BitImage, 0xff, iImageSize);
	m_iImageWidth = iWidth;
	m_iImageLength = iLength;
	m_fPixelSize = fPixelSize;	
}

inline bool CBitImage::_Load(IFile& iFile)
{
	unsigned long readlen;
	DWORD flag;

	
	//version
	iFile.Read(&flag, sizeof(DWORD));
	
	if (flag == 0x42584f4d)
	{
		//Angelica  bin file header
		iFile.Read(&flag, sizeof(DWORD));
	}



	if(flag ==BITIMAGE_OLD_VER)
	{
		
	}
	else if (flag == BITIMAGE_MAGIC)
	{
		iFile.Read(&flag, sizeof(DWORD));
		if (flag != BITIMAGE_VER)
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	
	DWORD BufSize;
	iFile.Read(&BufSize, sizeof(DWORD));
	unsigned char * buf = new unsigned char[BufSize];

	// Read the data
	readlen = iFile.Read(buf, BufSize);
	if (readlen != BufSize)
	{
		delete[] buf;
		return false;
	}
	Release();			// Release the old data
	int cur=0;
	m_iWidth=* (int *) (buf+cur);
	cur+=sizeof(int);
	m_iLength=* (int *) (buf+cur);
	cur+=sizeof(int);
		
	m_iImageWidth=* (int *) (buf+cur);
	cur+=sizeof(int);
	m_iImageLength=* (int *) (buf+cur);
	cur+=sizeof(int);
	m_fPixelSize=* (float *) (buf+cur);
	cur+=sizeof(float);
	m_BitImage = new unsigned char[ m_iWidth * m_iLength ];
	memcpy(m_BitImage, buf+cur, m_iWidth * m_iLength);
	delete [] buf;
	return true;
}


bool CBitImage::Load( const char * szName )
{
	StdFile   iFile;

	if (!iFile.Open(szName, IFILE_BIN|IFILE_READ))
	{
		return false;
	}
	
	
	return _Load(iFile);

}

inline bool CBitImage::_Save(IFile& iFile)
{
	DWORD dwWrite;
	// write magic
	dwWrite = BITIMAGE_MAGIC;
	iFile.Write(&dwWrite, sizeof(DWORD));	
	// write the Version info
	dwWrite=BITIMAGE_VER;
	iFile.Write(&dwWrite, sizeof(DWORD));

	// write the buf size
	DWORD BufSize=4* sizeof(int) + sizeof(float)+m_iWidth * m_iLength;
	dwWrite= BufSize;

	iFile.Write(&dwWrite, sizeof(DWORD));

	
	// write the following info in order
	// 1. m_iWidth
	// 2. m_iLength
	// 3. m_iImageWidth
	// 4. m_iImageLength
	// 5. m_fPixelSize
	// 6. data in m_BitImage
	unsigned char *buf=new unsigned char[BufSize];
	int cur=0;
	* (int *) (buf+cur) = m_iWidth;
	cur+=sizeof(int);

	* (int *) (buf+cur) = m_iLength;
	cur+=sizeof(int);

	* (int *) (buf+cur) = m_iImageWidth;
	cur+=sizeof(int);

	* (int *) (buf+cur) = m_iImageLength;
	cur+=sizeof(int);

	* (float *) (buf+cur) = m_fPixelSize;
	cur+=sizeof(float);	

	memcpy(buf+cur, m_BitImage, m_iWidth*m_iLength);
	
	iFile.Write(buf, BufSize);

	delete [] buf;
	return true;
}


bool CBitImage::Save( const char * szName)
{
	StdFile   iFile;

	if (!iFile.Open(szName, IFILE_BIN|IFILE_WRITE))
	{
		return false;
	}
	
	
	return _Save(iFile);
	
}




#endif
