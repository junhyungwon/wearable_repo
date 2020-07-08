// BlockBuffer.h: interface for the CBlockBuffer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _BLOCKBUFFER_HH_
#define _BLOCKBUFFER_HH_


#include <stdlib.h>

class CBlockBuffer  
{
public:
	CBlockBuffer();//	{ m_pData = NULL; m_nSize = m_nWritePtr = m_nReadPtr = 0; }
	CBlockBuffer(int nSize);//	{ m_pData = NULL; Alloc(nSize); }
	~CBlockBuffer();//	{ Free(); }

	/*
	 * Info functions
	 */

	void        *GetData();//	{ return m_pData; }
	unsigned int GetSize();//	{ return m_nSize; }
	void		*GetCurPtr();//	{ return ((void *)((char *)m_pData+m_nWritePtr)); }
	void		 SetCurPtr(int offset);//	{ m_nWritePtr+= offset; }
	unsigned int GetCurSize();//	{ return (m_nWritePtr - m_nReadPtr); }
	unsigned int GetRestSize();//	{ return (m_nSize - m_nWritePtr); }

	/*
	 * I/O functions
	 */
	int Write(unsigned char* pData, unsigned int nSize);
	int Read(unsigned char* pData, unsigned int nSize);

	/*
	 * Alloc and Realloc
	 * Functions
	 */

	int Alloc(unsigned int nSize);
	int ReAlloc(unsigned int nSize);

	/*
	 * Free functions
	 */

	void ClearData();//	{ m_nWritePtr = 0, m_nReadPtr = 0; }
	void Free();//	{ if(m_pData) free(m_pData); m_pData = NULL; m_nSize = m_nWritePtr = m_nReadPtr = 0; }

private:
	void* m_pData;
	unsigned int m_nSize;

	unsigned int m_nWritePtr;
	unsigned int m_nReadPtr;
};

#endif // _BLOCKBUFFER_H_

