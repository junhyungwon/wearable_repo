// BaseWriter.cpp: implementation of the CBaseWriter class.
//
//////////////////////////////////////////////////////////////////////

#include "basewriter.h"


BOOL CBaseWriter::OpenExtraFile(const char* strPath)
{
/*
	CString strTempPath = strPath.Left(strPath.GetLength()-4);
	strPath.Format("%s.mvd", (LPCTSTR)strTempPath);

	m_fileExtra = fopen((LPCTSTR)strPath, "wb");
	if(NULL == m_fileExtra)
		return FALSE;
*/		
	return TRUE;
}

BOOL CBaseWriter::WriteExtraData(BYTE* pbyData, int nDataSize)
{
	if(m_fileExtra)
	{
		fwrite(pbyData, nDataSize, 1, m_fileExtra);
		return TRUE;
	}

	return FALSE;
}

void CBaseWriter::CloseExtraFile()
{
	if(m_fileExtra)
		fclose(m_fileExtra);
}


