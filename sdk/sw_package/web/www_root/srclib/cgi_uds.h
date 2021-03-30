#ifndef __CGI_UDS_H__
#define __CGI_UDS_H__
////////////////////////////////////////////////////////////////////////////////

#define FILE_UDS_SYSTEM "/tmp/system.socket"
#define FILE_UDS_ONVIF  "/tmp/onvif.socket"

#define STR_MSG_CMD_FWUPDATE         "FWUPDATE"
#define STR_MSG_CMD_CHANGE_PASSWORD  "ChangePassword"
#define STR_MSG_CMD_CHECK_ACCOUNT    "CheckAccount"
#define STR_MSG_CMD_FACTORYDEFAULT   "SystemFactoryDefault"
#define STR_MSG_CMD_REBOOT           "SystemReboot"
#define STR_MSG_CMD_RESTART          "SystemReboot"
#define STR_MSG_CMD_UPDATE_USER      "UpdateUser"
#define STR_MSG_CMD_UPDATE_ONVIF_USER "UpdateOnvifUser"
#define STR_MSG_CMD_CONTRL_TELNETD    "ControlTelnetd"
#define STR_MSG_CMD_CONTRL_RTMP       "ControlRtmp"

#define STR_MSG_GET_ENCODER_INFO     "GetVideoEncoderInformation"
#define STR_MSG_GET_VIDEO_QUALITY    "GetVideoQuality"
#define STR_MSG_SET_VIDEO_QUALITY    "SetVideoQuality"
#define STR_MSG_SET_DYN_VIDEO_QUALITY    "SetDynVideoQuality"
#define STR_MSG_GET_OPERATION_CONFIG "GetOperationConfiguration"
#define STR_MSG_SET_OPERATION_CONFIG "SetOperationConfiguration"
#define STR_MSG_GET_NETWORK_CONFIG   "GetNetworkConfiguration"
#define STR_MSG_SET_NETWORK_CONFIG   "SetNetworkConfiguration"
#define STR_MSG_GET_SERVERS_CONFIG   "GetServersConfiguration"
#define STR_MSG_SET_SERVERS_CONFIG   "SetServersConfiguration"
#define STR_MSG_GET_SYSTEM_CONFIG    "GetSystemConfiguration"
#define STR_MSG_SET_SYSTEM_CONFIG    "SetSystemConfiguration"
#define STR_MSG_GET_USER_CONFIG      "GetUserConfiguration"
#define STR_MSG_SET_USER_CONFIG      "SetUserConfiguration"
#define STR_MSG_GET_VOIP_CONFIG      "GetVoipConfiguration"
#define STR_MSG_SET_VOIP_CONFIG      "SetVoipConfiguration"
#define STR_MSG_GET_SYSTEM_INFO      "GetSystemInfo"
#define STR_MSG_SET_SYSTEM_INFO      "SetSystemInfo"

enum _enumUdsCmd {
    UDS_CMD_FWUPDATE,
    UDS_CMD_RESTART,
    UDS_CMD_REBOOT,
    UDS_CMD_FACTORYSET,
    UDS_CMD_FACTORYSET_SOFT,        // except network information
    UDS_CMD_FACTORYSET_HARD,        // all
    UDS_CMD_FACTORYDEFAULT,
    UDS_CMD_FACTORYDEFAULT_SOFT,    // except network information
	UDS_CMD_FACTORYDEFAULT_HARD,	// all

	UDS_CMD_CONTROL_TELNETD,	// 
    UDS_CMD_CONTROL_RTMP,

    UDS_CMD_ADD_USER,
    UDS_CMD_MOD_USER,
    UDS_CMD_DEL_USER,
    UDS_CMD_CHANGE_PASSWORD,        
    UDS_CMD_CHECK_ACCOUNT,         // check current password
    UDS_CMD_UPDATE_USER,
    UDS_CMD_UPDATE_ONVIF_USER,

    UDS_GET_VIDEO_ENCODER_INFORMATION,
    UDS_SET_VIDEO_ENCODER_INFORMATION,

    UDS_GET_VIDEO_QUALITY,
    UDS_SET_VIDEO_QUALITY,
    UDS_SET_DYN_VIDEO_QUALITY,
    UDS_GET_OPERATION_CONFIG,
    UDS_SET_OPERATION_CONFIG,
    UDS_GET_NETWORK_CONFIG,
    UDS_SET_NETWORK_CONFIG,
    UDS_GET_SERVERS_CONFIG,
    UDS_SET_SERVERS_CONFIG,
    UDS_GET_SYSTEM_CONFIG,
    UDS_SET_SYSTEM_CONFIG,
    UDS_GET_USER_CONFIG,
    UDS_SET_USER_CONFIG,
    UDS_GET_VOIP_CONFIG,
    UDS_SET_VOIP_CONFIG,
    UDS_GET_SYSTEM_INFO,
    UDS_SET_SYSTEM_INFO,
};

enum _tagUdsErrs{
    ERR_UDS =-1000,
    ERR_FAILED_CONNECT,
    ERR_FAILED_CREATE_DESCRIPTOR,
    ERR_INVALID_ID,
    ERR_INVALID_PW,
    ERR_UDS_ERROR=-1,
	ERR_NO_ERROR = 0,
	ERR_NO_CHANGE = 1,
};


#ifdef __cplusplus
extern "C" {
#endif

int sysctl_message(int cmd, void *data, int data_len);


#ifdef __cplusplus
}
#endif
////////////////////////////////////////////////////////////////////////////////
#endif//__CGI_UDS_H__
