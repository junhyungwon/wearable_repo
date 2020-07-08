#ifndef __CGI_ERROR_H__
#define __CGI_ERROR_H__
///////////////////////////////////////////////////////////////////////////////

enum _enumCgiError{
    ERR_RECORD = -1000,
    ERR_EXIST_ID,
    ERR_PASSWORD,
    ERR_UNKNOWN,
    ERR_SET_FRAME,
    ERR_SETUP,
    ERR_INPUT,
    ERR_HOLIDAY,
    ERR_EMAIL_TEST,
    ERR_FW_UPDATE,
    ERR_NO_SDCARD = -990,              // SDCARD 없음(마운트 안됨)
    ERR_NO_FWFILE,              // 파일 없음
    ERR_FWFILE_SIZE,            // 사이즈에 문제가 있음.
    ERR_FWFILE_NOTATION,        // 모델에 맞는 펌웨어 검사..
    ERR_FWFILE_CONTENTS,        // 펌웨어 파일 내용 이상.
    ERR_NOT_SUPPORT,
    ERR_NOT_AVAILABLE,
    ERR_INVALID_PARAM,
    ERR_INVALID_ACCOUNT,
    ERR_INVALID_ACTION,			// invalid action name..search, update, delete, ...
    ERR_INVALID_METHOD = -980,
    SUBMIT_ERR,

    SOK           = 0,
    OK            = 0,
    CGI_NO_ERR    = 0,
    CGI_OK        = 0,
    CGI_OK_SUBMIT = 0,
    SUBMIT_OK     = 0,
    SUCCEED       = 0,
	CGI_NO_CHANGE = 1,
	SUBMIT_NO_CHANGE = 1,
};

///////////////////////////////////////////////////////////////////////////////
#endif//__CGI_ERROR_H__
