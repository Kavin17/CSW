#include "StdAfx.h"
#include "CgMulti.h"

CgMulti::CgMulti(void)
{
}

CgMulti::~CgMulti(void)
{
}

void CgMulti::WriteLog(CString str)
{
	CTime time = CTime::GetCurrentTime();

	CString strfolder;
	strfolder.Format("c:\\glim\\LOG");
	CreateDirectory(strfolder,NULL);

	CString strFile;
	strFile.Format("%s%s_LOG.txt","C:\\GLIM\\LOG\\",time.Format("%Y%m%d"));


	//strFile.Format("%s%s_LOG.txt",LOG_PATH,time.Format("%Y%m%d"));

	FILE *fp;
	fp = fopen((LPCSTR)strFile, "a");

	if(!fp)
		return;
	char* buf = new char[str.GetLength()+1];
	memcpy(buf, str.LockBuffer(), str.GetLength());
	buf[str.GetLength()] = '\0';

	fputs(buf, fp);
	fputs("\n",fp);
	delete []buf;
	fclose(fp);
}

void CgMulti::WriteCpuLog(CString str)
{
	CTime time = CTime::GetCurrentTime();

	CString strFile;
	strFile.Format("%s%s_CPULOG.txt","C:\\GLIM\\LOG\\",time.Format("%Y%m%d"));
	//strFile.Format("%s%s_LOG.txt",LOG_PATH,time.Format("%Y%m%d"));

	FILE *fp;
	fp = fopen((LPCSTR)strFile, "a");

	if(!fp)
		return;
	char* buf = new char[str.GetLength()+1];
	memcpy(buf, str.LockBuffer(), str.GetLength());
	buf[str.GetLength()] = '\0';

	fputs(buf, fp);
	fputs("\n",fp);
	delete []buf;
	fclose(fp);
}

void CgMulti::WriteData(CString name,CString str,int nn)
{
	CStdioFile   file;
	CString filename;
	CString strpath("C:\\GLIM\\LOG\\");
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%y%m%d");
	CString kk1 = ttime.Format("%y%m");

	CString strheader;
	CreateDirectory(strpath,NULL);

	if(nn==1)
		filename.Format(_T("%s%s%s_1Data.txt"),strpath,kk,name);
	else if(nn==2)
		filename.Format(_T("%s%s%s_2Data.txt"),strpath,kk,name);
	else if(nn==3)
		filename.Format(_T("%s%s%s_3Data.txt"),strpath,kk,name);
	else if(nn==4)
		filename.Format(_T("%s%s_Full_Data.txt"),strpath,kk);
	else if(nn==5)
		filename.Format(_T("%s%s%s.txt"),strpath,kk,name);
	else
		filename.Format(_T("%s%s%s.txt"),strpath,kk,name);

	if(!file.Open(filename, CFile::modeReadWrite|CFile::typeText)) 
	{		
		if(file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
		{
			file.WriteString(strheader);	
			file.WriteString(_T("\n"));
			file.WriteString(str);	
			file.Close();
		}
	}
	else
	{
		file.SeekToEnd();
		if(nn!=5)
			file.WriteString(_T("\n"));
		file.WriteString(str);
		file.Close(); 
	}	
}

