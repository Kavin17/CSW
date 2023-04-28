#pragma once

class CgMulti
{
public:
	CgMulti(void);
public:
	~CgMulti(void);
	void WriteData(CString name,CString str,int nn);
	void WriteCpuLog(CString str);
	void WriteLog(CString str);

};
