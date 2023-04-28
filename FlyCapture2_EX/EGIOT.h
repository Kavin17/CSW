#ifndef __EGIOT_H__
#define __EGIOT_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* DLL support */
#ifndef ___EXPORT
	#define ___EXPORT
#endif

___EXPORT	int		__stdcall EGIOT_Find();
___EXPORT	BOOL	__stdcall EGIOT_FindUnit(int id);								
___EXPORT	BOOL	__stdcall EGIOT_Open(int id);
___EXPORT	void	__stdcall EGIOT_Close(int id);
___EXPORT	void	__stdcall EGIOT_CloseAll();

___EXPORT	void	__stdcall EGIOT_SetTTLSource(int id, BOOL input);		

___EXPORT	BOOL	__stdcall EGIOT_ReadInputPort(int id, int port);
___EXPORT	USHORT	__stdcall EGIOT_ReadInput(int id);
___EXPORT	USHORT	__stdcall EGIOT_ReadOutput(int id);

___EXPORT	void	__stdcall EGIOT_WriteOutputPort(int id, int port, BOOL state);
___EXPORT	void	__stdcall EGIOT_WriteOutput(int id, USHORT mask);

___EXPORT	void	__stdcall EGIOT_WriteOutputPortTrigger(int id, int port, ULONG msec);
___EXPORT	void	__stdcall EGIOT_WriteOutputTrigger(int id, USHORT mask, ULONG msec);

___EXPORT	void	__stdcall EGIOT_WriteIntrPolarity(int id, int polarity);
___EXPORT	void	__stdcall EGIOT_WriteIntrEnable(int id, BOOL enable);

___EXPORT	BOOL	__stdcall EGIOT_WaitForIntrPort(int id, int port, ULONG msec);
___EXPORT	USHORT	__stdcall EGIOT_WaitForIntr(int id, USHORT mask, ULONG msec);

#ifdef __cplusplus
}
#endif

#endif	/* __EGIOT_H__ 20090601 Update */