/*
 * Copyright (c) 2000-2002 Miracom Inc.
 * All rights reserved.
 * IOI is protected under US Patent No. xxxxxxx
 * For more information, please contact:
 * Miracom Inc., Seoul, Korea
 *
 * Release : xgen.h	3.0
 *
 */

#ifndef _XGEN_H
#define _XGEN_H

#if defined(_XOPEN_SOURCE_EXTENDED)
#include <arpa/inet.h>
#endif

#if defined(_HPUX_SOURCE)
#	include <stdio.h>
#	include <stdlib.h>
#	include	<netinet/in.h>
#	include <netdb.h>
#elif defined(__sun)
#	include <stdio.h>
#	include <stdlib.h>
#	include <netinet/in.h>
#elif defined(WIN32) || defined(_WIN32_WCE)
#	include <stdio.h>
#	include <stdlib.h>
#	include <windows.h>
#   include <winsock.h>
#	include <tchar.h>
#endif

#include "transceiverx.h"

#if defined(_HPUX_SOURCE)
#	define TSTRLEN(s)	strlen(s)
#	define TSTRCPY(s1, s2)	strcpy(s1, s2)
#	define TSTRNCPY(s1,s2,s3)	strncpy(s1,s2,s3)
#	define TSTRSTR(s1, s2)	strstr(s1, s2)
#	define TSTRCHR(s1, c)	strchr(s1, c)
#	define TSTRCMP(s1, s2)	strcmp(s1, s2)
#	define TSTRNCMP(s1,s2,s3)	strncmp(s1, s2, s3)
#	define TOLOWER(c)		tolower(c)
#	define TSTRTOK(s1, s2)	strtok(s1, s2)
#	define TATOI(s)			atoi(s)
#	define TPRINTF			printf
#	define TFPRINTF			fprintf
#	define TSPRINTF			sprintf
#	define TSTRCAT(a, b)	strcat(a, b)
#elif defined(__digital__)
#	define TSTRLEN(s)	strlen(s)
#	define TSTRCPY(s1, s2)	strcpy(s1, s2)
#	define TSTRNCPY(s1,s2,s3)	strncpy(s1,s2,s3)
#	define TSTRSTR(s1, s2)	strstr(s1, s2)
#	define TSTRCHR(s1, c)	strchr(s1, c)
#	define TSTRCMP(s1, s2)	strcmp(s1, s2)
#	define TSTRNCMP(s1,s2,s3)	strncmp(s1, s2, s3)
#	define TOLOWER(c)		tolower(c)
#	define TSTRTOK(s1, s2)	strtok(s1, s2)
#	define TATOI(s)			atoi(s)
#	define TPRINTF			printf
#	define TFPRINTF			fprintf
#	define TSPRINTF			sprintf
#	define TSTRCAT(a, b)	strcat(a, b)
#elif defined(vms)
#	define TSTRLEN(s)	strlen(s)
#	define TSTRCPY(s1, s2)	strcpy(s1, s2)
#	define TSTRNCPY(s1,s2,s3)	strncpy(s1,s2,s3)
#	define TSTRSTR(s1, s2)	strstr(s1, s2)
#	define TSTRCHR(s1, c)	strchr(s1, c)
#	define TSTRCMP(s1, s2)	strcmp(s1, s2)
#	define TSTRNCMP(s1,s2,s3)	strncmp(s1, s2, s3)
#	define TOLOWER(c)		tolower(c)
#	define TSTRTOK(s1, s2)	strtok(s1, s2)
#	define TATOI(s)			atoi(s)
#	define TPRINTF			printf
#	define TFPRINTF			fprintf
#	define TSPRINTF			sprintf
#	define TSTRCAT(a, b)	strcat(a, b)
#elif defined(__sun)
#	define TSTRLEN(s)	strlen(s)
#	define TSTRCPY(s1, s2)	strcpy(s1, s2)
#	define TSTRNCPY(s1,s2,s3)	strncpy(s1,s2,s3)
#	define TSTRSTR(s1, s2)	strstr(s1, s2)
#	define TSTRCHR(s1, c)	strchr(s1, c)
#	define TSTRCMP(s1, s2)	strcmp(s1, s2)
#	define TSTRNCMP(s1,s2,s3)	strncmp(s1, s2, s3)
#	define TOLOWER(c)		tolower(c)
#	define TSTRTOK(s1, s2)	strtok(s1, s2)
#	define TATOI(s)			atoi(s)
#	define TPRINTF			printf
#	define TFPRINTF			fprintf
#	define TSPRINTF			sprintf
#	define TSTRCAT(a, b)	strcat(a, b)
#elif !defined(WIN32) && !defined(_WIN32_WCE)
#	define TSTRLEN(s)	strlen(s)
#	define TSTRCPY(s1, s2)	strcpy(s1, s2)
#	define TSTRNCPY(s1,s2,s3)	strncpy(s1,s2,s3)
#	define TSTRSTR(s1, s2)	strstr(s1, s2)
#	define TSTRCHR(s1, c)	strchr(s1, c)
#	define TSTRCMP(s1, s2)	strcmp(s1, s2)
#	define TSTRNCMP(s1,s2,s3)	strncmp(s1, s2, s3)
#	define TOLOWER(c)		tolower(c)
#	define TSTRTOK(s1, s2)	strtok(s1, s2)
#	define TATOI(s)			atoi(s)
#	define TPRINTF			printf
#	define TFPRINTF			fprintf
#	define TSPRINTF			sprintf
#	define TSTRCAT(a, b)	strcat(a, b)
#else
#	define TSTRLEN(s)		_tcslen(s)
#	define TSTRCPY(s1, s2)	_tcscpy(s1, s2)
#	define TSTRNCPY(s1,s2,s3)	_tcsncpy(s1,s2,s3)
#	define TSTRSTR(s1, s2)	_tcsstr(s1, s2)
#	define TSTRCHR(s1, c)	_tcschr(s1, c)
#	define TSTRCMP(s1, s2)	_tcscmp(s1, s2)
#	define TSTRNCMP(s1,s2,s3)	_tcsncmp(s1, s2, s3)
#	if defined(UNICODE) || defined(_UNICODE)
#		define TOLOWER(c)		towlower(c)
#	else
#		define TOLOWER(c)		tolower(c)	
#	endif
#	define TSTRTOK(s1, s2)	_tcstok(s1, s2)
#	define TATOI(s)			_ttoi(s)
#	define TPRINTF			_tprintf
#	define TFPRINTF			_tfprintf
#	define TSPRINTF			_stprintf
#	define TSTRCAT(a, b)	_tcscat(a, b)
#	define strcasecmp(a, b) (strcmp_icase(a, b))		
#	define WCTMB(x,y,z)		WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK,(x),-1,(y),(z),Null_Ptr,Null_Ptr)					 
#	define MBTWC(x,y,z) 	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED,	(x), -1, (y), (z) )
#endif /* !defined(WIN32) && !defined(_WIN32_WCE) */

#define IOI_DEFAULT_BUFFER_SIZE 2000000

#define IOI_DEFAULT_TIMEOUT 20000

#define XGEN_ERROR_MESSAGE_START        (IOI_ERROR_MESSAGE_END)
#define XGEN_ERROR_UNEXPECTED_OPERATION (XGEN_ERROR_MESSAGE_START - 1)
#define XGEN_ERROR_UNEXPECTED_VERSION   (XGEN_ERROR_MESSAGE_START - 2)
#define XGEN_ERROR_DIFF_MSG_SIZE        (XGEN_ERROR_MESSAGE_START - 3)
#define XGEN_ERROR_VARIABLE_ARRAY_SIZE  (XGEN_ERROR_MESSAGE_START - 4)
#define XGEN_ERROR_MESSAGE_END          XGEN_ERROR_UNEXPECTED_VERSION


#define DECLARE_INTERFACE_TUNE( interface_ ) LPCTSTR interface_##_channel();\
 void interface_##_Tune( IOISession session , Bool32 tune );\
 extern TCHAR interface_##_channel_string[255];

#define INTERFACE_INIT( _size_ ) static int __index_interface = 0; \
	INTERFACE_MAP	global_interface_map[ _size_ ];

#define INTERFACE_TUNE( interface_ , session_ , tune_ ) interface_##_Tune( session_ , tune_ )

#define _map_size( _map_ ) ( sizeof( _map_ ) / sizeof( INTERFACE_CTX ) )

#define UNICAST 	1
#define GUNICAST 	2
#define MULTICAST 	3
#define GMULTICAST 	4

/* in stub code ( unicast / multicast )*/
#define IMPLEMENT_INTERFACE_TUNE( prefix_ , module_ , interface_ , size_  , tuneh_ ) TCHAR interface_##_channel_string[255];\
void interface_##_Tune( IOISession ss , Bool32 tune )\
{\
	if( ( prefix_ != Null_Ptr ) && ( TSTRLEN( prefix_ ) > 0 ) )\
		TSPRINTF( interface_##_channel_string , "/%s/%s/%s", prefix_ , #module_ , #interface_ );\
	else\
		TSPRINTF( interface_##_channel_string , "/%s/%s", #module_ , #interface_ );\
		\
	if( tune )\
	{\
		global_interface_map[ __index_interface ].interface_name = #interface_;\
		global_interface_map[ __index_interface ].pmap = interface_##contexts;\
		global_interface_map[ __index_interface ].size = size_;\
		\
		if( tuneh_ == UNICAST )\
			IOITuneUnicast( ss , interface_##_channel_string );\
		else if( tuneh_ == MULTICAST )\
			IOITuneMulticast( ss , interface_##_channel_string );\
		else if( tuneh_ == GMULTICAST )\
			IOITuneGuaranteedMulticast( ss , interface_##_channel_string );\
		else if( tuneh_ == GUNICAST )\
			IOITuneGuaranteedUnicast( ss , interface_##_channel_string );\
	}\
	else\
	{\
		if( tuneh_ == UNICAST )\
			IOIUntuneUnicast( ss , interface_##_channel_string );\
		else if( tuneh_ == MULTICAST )\
			IOIUntuneMulticast( ss , interface_##_channel_string );\
		else if( tuneh_ == GMULTICAST )\
			IOIUntuneGuaranteedMulticast( ss , interface_##_channel_string );\
		else if( tuneh_ == GUNICAST )\
			IOIUntuneGuaranteedUnicast( ss , interface_##_channel_string );\
	}\
	__index_interface++;\
}\
\
LPCTSTR interface_##_channel()\
{\
	return interface_##_channel_string;\
}

/* in proxy code */
#define IMPLEMENT_PROXY_TUNE( prefix_ , module_ , interface_ ) TCHAR interface_##_channel_string[255];\
LPCTSTR interface_##_channel()\
{\
	if( ( prefix_ != Null_Ptr ) && ( TSTRLEN( prefix_ ) > 0 ) )\
		TSPRINTF( interface_##_channel_string , "/%s/%s/%s", prefix_ , #module_ , #interface_ );\
	else\
		TSPRINTF( interface_##_channel_string , "/%s/%s", #module_ , #interface_ );\
	return (LPCTSTR)interface_##_channel_string;\
}


#define BEGIN_INTERFACE_CONTEXT( interface_ ) interface_ctx interface_##contexts[] = {
#define INTERFACE_CONTEXT( method_ ) \
{ #method_ ,  method_##_dispatcher },
#define END_INTERFACE_CONTEXT( interface_ ) { NULL, NULL } };

#define CHECK_SIZE(size_) \
{ \
if(pos_+(size_) > len_){ \
replyError_(issuer, req , XGEN_ERROR_DIFF_MSG_SIZE, xgen_error_diff_msg_size); \
return XGEN_ERROR_DIFF_MSG_SIZE; \
} \
}

/* Operation Dispatcher Type */
typedef int (*operation_initialize)();
typedef int (*operation_terminate)();
typedef int (*operation_dispatcher)(IOIHandle issuer, IOIMessage req );
typedef struct interface_ctx
{
	LPCTSTR op_name;
	operation_dispatcher op_dispatcher;
} ;

typedef interface_ctx INTERFACE_CTX;
typedef struct interface_map
{
	LPCTSTR interface_name;
	int		size;
	INTERFACE_CTX * pmap;
} ;

typedef interface_map INTERFACE_MAP;

typedef int (*operation_handler)(IOIHandle issuer, IOIMessage req );
typedef struct IOIResult
{
	long code;
	TCHAR msg[1024];
} ;


/* Epilogue, Prologue Type */
typedef int (*xgen_prologue_)(IOIHandle issuer, IOIMessage req );
typedef int (*xgen_epilogue_)(IOIHandle issuer, IOIMessage req, IOIStatus status);

/* Function prototype */
extern long _find_last_space(TCHAR *s, long size);

extern void setXgenPrologue_(xgen_prologue_ users);
extern void setXgenEpilogue_(xgen_epilogue_ users);
extern void setChannelPrefix_(TCHAR* prefix);
extern void setSessionMode_(int sess_mode);
extern int replyError_(IOIHandle issuer, IOIMessage req, long err_code, TCHAR* err_msg);

extern int xgenGetLastError();

/* Global Variable */
extern TCHAR req_data_[IOI_DEFAULT_BUFFER_SIZE];
extern TCHAR channel_prefix_[30];
extern IOIResult g_result_;
extern TCHAR xgen_error_diff_msg_size[64];

/*add bkkwon - start*/
extern INTERFACE_MAP	global_interface_map[];

extern IOIHandle operator_map;
/*end*/

#endif /* _XGEN_H */
