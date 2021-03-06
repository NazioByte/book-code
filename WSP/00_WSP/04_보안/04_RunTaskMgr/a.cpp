	#include <windows.h>
	#include <iostream.h>

	// Process에 특권을 부여한다.
	BOOL EnablePri( char* pri, BOOL bEnable )
	{
		// 현재 프로세스의 ProcessToken을 얻는다.
		HANDLE hToken;

		OpenProcessToken( GetCurrentProcess(), // 프로세스 핸들.
						  TOKEN_ADJUST_PRIVILEGES, 
						  &hToken);

		// 특권 이름을 특권 번호( 64비트, luid라고 부름)으로 바꾼다.
		LUID luid;
		LookupPrivilegeValue( ".", // PC이름
							  pri, // 특권이름
							  &luid ); // 특권 번호를 담을 변수.
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1; // 특권 갯수.
		tp.Privileges[0].Luid = luid; // 번호
		tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;

		return AdjustTokenPrivileges( hToken, 0, &tp, sizeof( tp), 0, 0);
	}


	void main()
	{
		EnablePri( SE_DEBUG_NAME,TRUE );

		STARTUPINFO si = { sizeof( si ) };

		PROCESS_INFORMATION pi;

		CreateProcess( 0, "TaskMgr.exe", 0,0,0,0,0,0,&si, &pi );
		CloseHandle( pi.hProcess);
		CloseHandle( pi.hThread);

	}
