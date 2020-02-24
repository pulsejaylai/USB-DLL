// usbdll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "tchar.h"
#include "setupapi.h"
extern "C" {
#include "hidsdi.h"
}
#include <cfgmgr32.h>

#pragma comment( lib, "setupapi.lib" )
#pragma comment( lib, "hid.lib" )
#pragma comment(lib, "rpcrt4.lib")

typedef LPSTR LPTSTR2;


BOOL GetRegistryProperty(HDEVINFO DeviceInfoSet,
	PSP_DEVINFO_DATA DeviceInfoData,
	ULONG Property,
	PVOID Buffer,
	PULONG Length);
char* TCHAR2char(const TCHAR* STR);

extern "C" __declspec(dllexport) char * Gpiblist()
{
	char *result;
	char str[1000],str3[1000],str2[1000];
	strcpy_s(str,256,"");
	result = str;
	DWORD Status, Problem;
	LPTSTR Buffer = NULL;
	DWORD BufSize = 0;
	BOOL bResult = FALSE;
	int x = 0;
	// 返回所有设备信息
	HDEVINFO hDevInfo = SetupDiGetClassDevs(NULL, NULL, 0, DIGCF_PRESENT | DIGCF_ALLCLASSES);

	if (INVALID_HANDLE_VALUE == hDevInfo)
		//return bResult;
	{
		result = str;
	}
	SP_DEVINFO_DATA DeviceInfoData = { sizeof(SP_DEVINFO_DATA) };
	
	for (DWORD DeviceId = 0;
		SetupDiEnumDeviceInfo(hDevInfo, DeviceId, &DeviceInfoData);
		DeviceId++)
	{
		TCHAR szDevName[MAX_PATH] =_T( "");
		//strcpy_s(szDevName, MAX_PATH, "");
		if (GetRegistryProperty(hDevInfo, &DeviceInfoData, SPDRP_CLASS, &Buffer, (PULONG)&BufSize))
		{
			lstrcpyn(szDevName, Buffer, MAX_PATH);
		}
		if (lstrcmp(szDevName, _T("USB")) == 0)
		{
			TCHAR szName[MAX_PATH] = _T("");
			if (GetRegistryProperty(hDevInfo, &DeviceInfoData, SPDRP_DEVICEDESC, &Buffer, (PULONG)&BufSize))
			{
				lstrcpyn(szName, Buffer, MAX_PATH);
				if(x==0){ strcpy_s(str3, 1000, TCHAR2char(szName)); }
				if (x != 0)
				{
					strcpy_s(str2, 1000, TCHAR2char(szName));
					strcat_s(str3, ",");
					strcat_s(str3, str2);
				}
				//::MessageBox(NULL,szName,_T("DEVICE"),MB_OK);
				  //IDC_LIST1.Items.Add
					// m_list1.AddString(szName);
				//m_list1.InsertString(x, CString(szName));
				x++;
				//m_list1.setitemtext		
			}



		}







	}


	






	result = str3;
	
	
	return result;
}

BOOL GetRegistryProperty(HDEVINFO DeviceInfoSet,
	PSP_DEVINFO_DATA DeviceInfoData,
	ULONG Property,
	PVOID Buffer,
	PULONG Length)
{
	while (!SetupDiGetDeviceRegistryProperty(DeviceInfoSet,
		DeviceInfoData,
		Property,
		NULL,
		(BYTE *)*(TCHAR **)Buffer,
		*Length,
		Length))
	{
		// 长度不够则重新分配缓冲区
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (*(LPTSTR2 *)Buffer)
				LocalFree(*(LPTSTR2 *)Buffer);

			*(LPTSTR2 *)Buffer = (PCHAR)LocalAlloc(LPTR, *Length);
		}
		else
		{
			return false;
		}
	}

	return (BOOL)(*(LPTSTR *)Buffer)[0];
}

char* TCHAR2char(const TCHAR* STR)

{

	//返回字符串的长度

	int size = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, FALSE);

	//申请一个多字节的字符串变量

	char* str2 = new char[sizeof(char) * size];

	//将STR转成str

	WideCharToMultiByte(CP_ACP, 0, STR, -1, str2, size, NULL, FALSE);

	return str2;

}