/*******************************************************************************
' CUSDGWCaster.h
'
' Copyright (c) 2004 by Miracom,Inc.
' All rights reserved.
'
' Generated by XGen 4.0
'
' Created at 
'
' Author : Miracom
' Description : Generated by XGen4.3.0.1
'******************************************************************************/

#ifndef _XGEN_CUSCOREDGW_SENDER_H
#define _XGEN_CUSCOREDGW_SENDER_H

#include "CUSDGWType.h"


extern IOIStatus send_CUSDGW_Send(CUSDGW_Send_In_Tag *source_type, LPTSTR channel, long ttl, int mode);
extern IOIStatus send_CUSInsp_Lbl(CUSInsp_Lbl_In_Tag *source_type, LPTSTR channel, long ttl, int mode);
extern IOIStatus send_CUSInsp_Lbl_AIS(CUSInsp_Lbl_In_Tag *source_type, CUSDGW_Cmn_Out_Tag *target_type, LPTSTR channel, long ttl, int mode);
extern IOIStatus send_CUSDGW_SendAuto(CUSDGW_SendAuto_In_Tag *source_type, CUSDGW_Cmn_Out_Tag *target_type, LPTSTR channel, long ttl, int mode);
extern IOIStatus send_CUSDGW_ViewSanpoData(CUSDGW_ViewSanpoData_In_Tag *source_type, CUSDGW_ViewSanpoData_Out_Tag *target_type, LPTSTR channel, long ttl, int mode);


#endif /* _XGEN_CUSCOREDGW_SENDER_H */



