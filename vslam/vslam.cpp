#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>

bool initCom(HANDLE& hCom){
	hCom = CreateFile(L"COM4",//指定COM口  
		GENERIC_READ | GENERIC_WRITE, //允许读和写  
		0, //独占方式  
		NULL,
		OPEN_EXISTING, //打开而不是创建  
		0, //同步方式  
		NULL);
	if (hCom == (HANDLE)-1)
	{
		printf("打开COM失败!");
		return false;
	}
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR); // 清除缓存
	return true;
}

bool writeCom(HANDLE& hCom){
	char lpOutBuffer[100];
	DWORD dwBytesWrite = 100;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	ClearCommError(hCom, &dwErrorFlags, &ComStat);
	bWriteStat = WriteFile(hCom, lpOutBuffer, dwBytesWrite, &dwBytesWrite, NULL);
	if (!bWriteStat)
	{
		printf("写串口失败!");
		return false;
	}
	return true;
}

DWORD readCom(HANDLE& hCom){

	char lpInBuffer[1024];
	DWORD dwBytesRead = 1024;
	BOOL bReadStatus;
	DWORD dwErrorFlags;
	COMSTAT ComStat;
	OVERLAPPED m_osRead;

	ClearCommError(hCom, &dwErrorFlags, &ComStat);
	if (!ComStat.cbInQue)
		return 0;
	dwBytesRead = min(dwBytesRead, (DWORD)ComStat.cbInQue);
	bReadStatus = ReadFile(hCom, lpInBuffer, dwBytesRead,
		&dwBytesRead, &m_osRead);
	if (!bReadStatus) //如果ReadFile函数返回FALSE  
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			GetOverlappedResult(hCom,
				&m_osRead, &dwBytesRead, TRUE);
			// GetOverlappedResult函数的最后一个参数设为TRUE，  
			//函数会一直等待，直到读操作完成或由于错误而返回。  

			return dwBytesRead;
		}
		return 0;
	}
	return dwBytesRead;
}


int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hCom;
	initCom(hCom);
	writeCom(hCom);
	readCom(hCom);
	system("PAUSE");
	return 0;
}

