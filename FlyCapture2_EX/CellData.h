#pragma once
class CCellData
{
public:
	CCellData();
	~CCellData();

	void				Init(LPVOID lParam);
	bool				GetSpecParam(int nVisionType);
	bool				GetSpecParamCSW();
	bool				GetSystemParam();


	typedef struct
	{
		int				nInspType;
		char			cDetectionName[30];
		CRect		rtROI;
		double		dSpec[3]; // LSL, USL, Offset
	}SPECPARAM;

	SPECPARAM		m_structSpecParamData[150];

	//DEFECTINFO	m_stDefectCandidate[MAX_FRAME][MAX_CANDIDATE_COUNT];
	//DEFECTINFO	m_stDefectInfo[MAX_DEFECT_COUNT];
};