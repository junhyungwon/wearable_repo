// BlockBuffer.cpp: implementation of the CBlockBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include "syscommon.h"
#include "blockbuffer.hh"

/*
 * Alloc and Realloc
 * Functions
 */

CBlockBuffer::CBlockBuffer()	{ m_pData = NULL; m_nSize = m_nWritePtr = m_nReadPtr = 0; }
CBlockBuffer::CBlockBuffer(int nSize) { m_pData = NULL; Alloc(nSize); }
CBlockBuffer::~CBlockBuffer()	{ Free(); }

void		*CBlockBuffer::GetData()	{ return m_pData; }
unsigned int CBlockBuffer::GetSize()	{ return m_nSize; }
void		*CBlockBuffer::GetCurPtr()	{ return ((void *)((char *)m_pData+m_nWritePtr)); }
void		 CBlockBuffer::SetCurPtr(int offset)	{ m_nWritePtr+= offset; }
unsigned int CBlockBuffer::GetCurSize()	{ return (m_nWritePtr - m_nReadPtr); }
unsigned int CBlockBuffer::GetRestSize()	{ return (m_nSize - m_nWritePtr); }


int CBlockBuffer::Alloc(unsigned int nSize)
{
//	if(m_pData)
//		return FALSE;

	m_nSize = nSize;

	if(m_pData)
		free(m_pData);

	m_pData = (void *)malloc(nSize);

	if(m_pData)
		memset(this->GetData(), 0, this->GetSize());
	else{
		DBG2_LOG("Can't Create Buffer\n");
		return PROC_ERROR;
	}

	m_nWritePtr = 0;
	m_nReadPtr = 0;

	return PROC_SUCCESS;
}

int CBlockBuffer::ReAlloc(unsigned int nSize)
{
	if(!m_pData)
		return Alloc(nSize);

	m_pData = realloc(m_pData, nSize);
	m_nSize = nSize;

	m_nWritePtr = 0;
	m_nReadPtr = 0;

	return PROC_SUCCESS;
}


/*
 * I/O functions
 */
int CBlockBuffer::Write(unsigned char* pData, unsigned int nSize)
{
	if(m_nSize-m_nWritePtr < nSize){
		DBG2_LOG("Invalid Write size: %d\n", nSize);
		return PROC_ERROR;
	}
	memcpy(((char*)m_pData+m_nWritePtr), (char*)pData, nSize);
	m_nWritePtr += nSize;
//	TRACE("CBlockBuffer::Write(%5d)\n", nSize);

	return nSize;
}

int CBlockBuffer::Read(unsigned char* pData, unsigned int nSize)
{
	if(m_nWritePtr < (m_nReadPtr + nSize)){
		DBG2_LOG("Invalid Read Size: %d\n", nSize);
		return PROC_ERROR;
	}

	memcpy((char*)pData, ((char*)m_pData+m_nReadPtr), nSize);
	m_nReadPtr += nSize;

	return nSize;
}

void CBlockBuffer::ClearData()	{ m_nWritePtr = 0, m_nReadPtr = 0; }
void CBlockBuffer::Free() { if(m_pData) free(m_pData); m_pData = NULL; m_nSize = m_nWritePtr = m_nReadPtr = 0; }



