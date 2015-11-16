#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>

bool initCom(HANDLE& hCom){
	hCom = CreateFile(L"COM4",//ָ��COM��  
		GENERIC_READ | GENERIC_WRITE, //�������д  
		0, //��ռ��ʽ  
		NULL,
		OPEN_EXISTING, //�򿪶����Ǵ���  
		0, //ͬ����ʽ  
		NULL);
	if (hCom == (HANDLE)-1)
	{
		printf("��COMʧ��!");
		return false;
	}
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR); // �������
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
		printf("д����ʧ��!");
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
	if (!bReadStatus) //���ReadFile��������FALSE  
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			GetOverlappedResult(hCom,
				&m_osRead, &dwBytesRead, TRUE);
			// GetOverlappedResult���������һ��������ΪTRUE��  
			//������һֱ�ȴ���ֱ����������ɻ����ڴ�������ء�  

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

