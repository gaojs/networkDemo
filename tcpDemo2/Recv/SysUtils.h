//
// 系统扩展
// 最后更新: 2003.08.18
// 历史:
// 1 create by alone on 2003.08.18
//
#ifndef _SYSUTILS_INCLUDE__
#define _SYSUTILS_INCLUDE__

#include "Shlwapi.h"

// 判断文件是否存在
BOOL IsFileExists(char *pszPathName);
// 创建多层目录,成功返回TRUE，识别返回FALSE
BOOL ForceDirectories(char *pszDir);
// 扩展文件操作
BOOL DeleteFileEx(char *szPathName, BOOL bAllowUndo = FALSE);
BOOL RenameFileEx(char *szOldPathName, char *szNewPathName);
BOOL MoveFileEx(char *szSrcPathName, char *szDstPathName);
BOOL CopyFileEx(char *szSrcPathName, char *szDstPathName);
// 重新启动操作系统
BOOL RebootWindows();
// 设置程序是否在操作系统启动后自动运行
void SetAutoRun(BOOL bEnable);
BOOL ShutDownWin98();
BOOL ShutDownWinNT();

BOOL IsLegalFileName(char *szFileName);

#endif // #ifndef _SYSUTILS_INCLUDE__
