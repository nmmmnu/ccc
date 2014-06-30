/*
   This file is part of mad.h, so you do
not need to include it separately.
It is used for compatibility and porting
one and same C/C++ code between:

	1. UNIX G++ for Linux
	2. Borland C/C++ for DOS and Windows 3.11
	3. Borland C++Builder
	4. UNIX G++ for FreeBSD
	5. Microsoft Visual C++
	6. Microsoft Visual C++ and MFC support
	7. G++ for CYGWIN (Posix layer for Win95)
	8. G++ for BeOS
*/

#ifndef _MAD_MAD_PORT_H
#define _MAD_MAD_PORT_H

/************** Borland C *****************/
/*  __BORLANDC__ is always defined in BC  */

/************** Mcrosift C ****************//*
   Trying to catch Microsoft C++, there 
isn't way to do this, so I cannot do it
Uncomment this bellow or put /D "__MICROSOFTC__" /D "__MFC__"
*//*
#define __MICROSOFTC__
#define __MFC__
*/

/* Prevantive catch of __MICROSOFTC__*/
#if defined(__MFC__) && !defined(__MICROSOFTC__)
	#define __MICROSOFTC__
#endif

/***** Mcrosift Visual C++ things *********/

#ifdef __MICROSOFTC__
	#if _MSC_VER > 1000
		#pragma once
	#endif

	#ifdef _DEBUG
		#include <afxwin.h>
		#define new DEBUG_NEW
		#undef THIS_FILE
		static char THIS_FILE[] = __FILE__;
	#endif
#endif

/***** Mcrosift Visual C++ MFC Init *******/

#ifdef __MFC__
	#include <afxwin.h>
	#define initMFC() (AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))

//CWinApp theApp;

#else
	/*Fake*/
	#define initMFC() 1
#endif

/************** _DOS_ *********************/

#if defined(__BORLANDC__) || defined(__MICROSOFTC__)
	#define __DOS__
#endif

/************** NULLs *********************/
#ifndef NULLs
    #define NULLs ((char *) (NULL))
#endif

/************** NOOP (for macroses) *******/

#ifndef NOOP
	#define NOOP
#endif

/************** Boolean type **************/

#ifndef boolean
	#define boolean int
#endif

#ifndef true
	#define true    1
#endif

#ifndef false
	#define false   0
#endif

/************** elif keyword **************/

#ifndef elif
	#define elif else if
#endif

/************** OS path delimiter *********/

/* I change the char[x] to 'x' because I play with preprocessor */ 

#define UNIX_PATH_DELIMITER_STR "/"
#define UNIX_PATH_DELIMITER     '/'

#ifdef __DOS__
	#define PATH_DELIMITER_STR "\\"
	#define PATH_DELIMITER     '\\'
#else
	#define PATH_DELIMITER_STR UNIX_PATH_DELIMITER_STR
	#define PATH_DELIMITER     UNIX_PATH_DELIMITER
#endif

/************** OS default shell **********/

#ifdef __DOS__
	#if defined(__WINNT__) || defined(__OS2__)
		#define BIN_SHELL          "cmd.exe"
	#else
		#define BIN_SHELL          "c:\\command.com"
	#endif
#else
	#define BIN_SHELL          "/bin/sh -s"
#endif

/************** case strcmp() *************//*
   stricmp()    - Borland C++, used as standard
   strcasecmp() - is BSD function supported in G++
   _strcmpi()   - Microsoft C/C++
*/

#if defined(__BORLANDC__)
	/* empty */
#elif defined(__MICROSOFTC__) 
	#define stricmp _strcmpi
#else
	#define stricmp strcasecmp
#endif

/************** popen()/pclose() **********//*
    popen(); pclose() - POSIX/UNIX
   _popen();_pclose() - Borland C++Builder
   _popen();_pclose() - Microsoft Visual C++
*/

#ifdef __DOS__
	#if defined(__BORLANDC__) || defined(__MICROSOFTC__)
		#define PIPEREADMODE    "rt"
		#define PIPEWRITEMODE   "wt"
		#define PIPEREADBMODE   "rb"
		#define PIPEWRITEBMODE  "wb"
		#define popen  _popen
		#define pclose _pclose
	#else
		#define _NOPOPEN
	#endif
#else
	#define PIPEREADMODE    "r"
	#define PIPEWRITEMODE   "w"
	#define PIPEREADBMODE   PIPEREADMODE
	#define PIPEWRITEBMODE  PIPEWRITEMODE
#endif

/************** pipe() ********************/

#ifdef __DOS__
	#define _NOPIPE
#endif

/************** chdir() *******************//*
POSIX/UNIX	chdir	unistd.h
Borland C++	chdir	dir.h
Microsoft C++	_chdir	direct.h
*/

#if defined(__MICROSOFTC__)
	#define chdir(a) _chdir(a)
	#define CHDIR_LIB <direct.h>
#elif defined(__BORLANDC__)
	#define CHDIR_LIB <dir.h>
#else
	#define CHDIR_LIB <unistd.h>
#endif

/******************************************/

//_MAD_MAD_PORT_H
#endif
