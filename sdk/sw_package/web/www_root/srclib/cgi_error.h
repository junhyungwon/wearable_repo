#ifndef __CGI_ERROR_H__
#define __CGI_ERROR_H__
///////////////////////////////////////////////////////////////////////////////

enum _enumCgiError{
    ERR_RSAKEYUPDATE               = -1007,                 //  -1006,
    ERR_RSAKEYUPDATE_NOFILE        = -1006,          // 파일 없음
    ERR_RSAKEYUPDATE_SIZE          = -1005,            // 사이즈에 문제가 있음.
    ERR_RSAKEYUPDATE_NOTATION      = -1004,        // 모델에 맞는 RSAKEY 검사..
    ERR_RSAKEYUPDATE_CONTENTS      = -1003,        // RSAKEY 파일 내용 이상.
    ERR_RSAKEYUPDATE_INVALID_FILE  = -1002,    // -985, RSAKEY 파일 내용 이상.
    ERR_RSAKEYUPDATE_FTP_RUNNING   = -1001,
    ERR_RECORD                     = -1000,
    ERR_EXIST_ID                   = -999,
    ERR_PASSWORD                   = -998,
    ERR_UNKNOWN                    = -997,
    ERR_SET_FRAME                  = -996,
    ERR_SETUP                      = -995,                    // -995
    ERR_INPUT                      = -994,
    ERR_HOLIDAY                    = -993,
    ERR_EMAIL_TEST                 = -992,
    ERR_NO_SDCARD                  = -991,                // SDCARD 없음(마운트 안됨)
    ERR_FWUPDATE                   = -990,                 //  -990,
    ERR_FWUPDATE_NOFILE            = -989,          // 파일 없음
    ERR_FWUPDATE_SIZE              = -988,            // 사이즈에 문제가 있음.
    ERR_FWUPDATE_NOTATION          = -987,        // 모델에 맞는 펌웨어 검사..
    ERR_FWUPDATE_CONTENTS          = -986,        // 펌웨어 파일 내용 이상.
    ERR_FWUPDATE_INVALID_FILE      = -985,    // -985, 펌웨어 파일 내용 이상.
    ERR_FWUPDATE_FTP_RUNNING       = -984,
    ERR_INVALID                    = -983,
    ERR_INVALID_PARAM              = -982,
    ERR_INVALID_ACCOUNT            = -981,          // 
    ERR_INVALID_ACTION             = -980,			  // -980, invalid action name..search, update, delete, ...
    ERR_INVALID_METHOD             = -979,
    ERR_NOT_SUPPORT                = -978,
    ERR_NOT_AVAILABLE              = -977,
    SUBMIT_ERR                     = -976,

    SOK           = 0,
    OK            = 0,
    CGI_NO_ERR    = 0,
    CGI_OK        = 0,
    CGI_OK_SUBMIT = 0,
    SUBMIT_OK     = 0,
    SUCCEED       = 0,
	OK_FW_UPDATE  = 0,
	OK_RSAKEY_UPDATE  = 0,
	CGI_NO_CHANGE = 1,
	SUBMIT_NO_CHANGE = 1,
	SUBMIT_GET_VID = 100,
};

///////////////////////////////////////////////////////////////////////////////
#endif//__CGI_ERROR_H__
