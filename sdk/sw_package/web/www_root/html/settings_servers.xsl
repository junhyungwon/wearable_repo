<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="html" encoding="utf-8"></xsl:output>
    <xsl:variable name="lang"><xsl:value-of select="/*/@lang"></xsl:value-of></xsl:variable>
    <xsl:variable name="strings" select="document(concat('/language/',$lang,'.xml'))/strings"></xsl:variable>
    <xsl:template match="/">
<!-- start  -->
<html>
    <xsl:attribute name="lang"><xsl:value-of select="$lang"></xsl:value-of></xsl:attribute>
    <xsl:attribute name="xml:lang"><xsl:value-of select="$lang"></xsl:value-of></xsl:attribute>
<head>
    <META HTTP-EQUIV="EXPIRES" CONTENT="0"></META>
<META HTTP-EQUIV="CACHE-CONTROL" CONTENT="NO-CACHE"></META>
<META HTTP-EQUIV="PRAGMA" CONTENT="NO-CACHE"></META>
<META NAME="viewport" CONTENT="user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0, width=device-width"></META>
<LINK REL="stylesheet" TYPE="text/css" HREF="/css/css.css"></LINK>
<title>Servers Settings</title>
<script type="text/javascript" src="/script/utils.js"></script>
<script type="text/javascript">
<![CDATA[
var xmlStr;
var isSubmitting=false;

function OnLoad() {
	document.title += ' - ' + document.getElementById('model_name').value;
    var lang = document.getElementById('lang').value;
    xmlStr = LoadXmlDoc('/language/'+lang+'.xml');

    onClickBackupServerEnable();
    onClickManagementServerEnable();
    onClickDdnsEnable();
    onClickNtpEnable();
}

function onClickBackupServerEnable()
{
    var frm = document.frm_config;

	if(frm.chk_bs_enable.checked){
		EnableObj(frm.txt_bs_ip);
		EnableObj(frm.txt_bs_id);
		EnableObj(frm.txt_bs_pw);
		EnableObj(frm.txt_bs_port);
	}
	else {
		DisableObj(frm.txt_bs_ip);
		DisableObj(frm.txt_bs_id);
		DisableObj(frm.txt_bs_pw);
		DisableObj(frm.txt_bs_port);
	}
}
function onClickManagementServerEnable()
{
    var frm = document.frm_config;

	if(frm.chk_ms_enable.checked){
		EnableObj(frm.txt_ms_ip);
		EnableObj(frm.txt_ms_port);
	}
	else {
		DisableObj(frm.txt_ms_ip);
		DisableObj(frm.txt_ms_port);
	}
}
function onClickDdnsEnable()
{
    var frm = document.frm_config;

	if(frm.chk_ddns_enable.checked){
		EnableObj(frm.txt_ddns_server);
		EnableObj(frm.txt_ddns_hostname);
		EnableObj(frm.txt_ddns_id);
		EnableObj(frm.txt_ddns_pw);
	}
	else {
		DisableObj(frm.txt_ddns_server);
		DisableObj(frm.txt_ddns_hostname);
		DisableObj(frm.txt_ddns_id);
		DisableObj(frm.txt_ddns_pw);
	}
}
function onClickNtpEnable()
{
    var frm = document.frm_config;

	if(frm.chk_ntp_enable.checked){
		EnableObj(frm.txt_ntp_ip);
	}
	else {
		DisableObj(frm.txt_ntp_ip);
	}
}

function OnApply()
{
    if(isSubmitting == true){
	  str = GetXmlString(xmlStr, 'running_submit');
	  alert(str);
	  return;
    }

    var frm = document.frm_config;

    // TODO : make sure values are vaild
    frm.bs_enable.value   = GetChkValue(frm.chk_bs_enable);
	if(frm.bs_enable.value == "1"){
		if(frm.txt_bs_ip.value == ""){
			alert("Please, check Backup server address");
			frm.txt_bs_ip.focus();
			return;
		}
		if(frm.txt_bs_id.value == ""){
			alert("Please, check Backup server id");
			frm.txt_bs_id.focus();
			return;
		}
		if(frm.txt_bs_pw.value == ""){
			alert("Please, check Backup server password");
			frm.txt_bs_pw.focus();
			return;
		}
		if(frm.txt_bs_port.value == ""){
			alert("Please, check Backup server port");
			frm.txt_bs_port.focus();
			return;
		}
	}

    frm.ms_enable.value   = GetChkValue(frm.chk_ms_enable);
	if(frm.ms_enable.value == "1"){
		if(frm.txt_ms_ip.value == ""){
			alert("Please, check Manage server address");
			frm.txt_ms_ip.focus();
			return;
		}
		if(frm.txt_ms_port.value == ""){
			alert("Please, check Manage server port");
			frm.txt_ms_port.focus();
			return;
		}
	}

    frm.ddns_enable.value = GetChkValue(frm.chk_ddns_enable);
	if(frm.ddns_enable.value == "1"){
		if(frm.txt_ddns_server.value == ""){
			alert("Please, check DDNS address");
			frm.txt_ddns_server.focus();
			return;
		}
		if(frm.txt_ddns_hostname.value == ""){
			alert("Please, check DDNS Hostname");
			frm.txt_ddns_hostname.focus();
			return;
		}
		if(frm.txt_ddns_id.value == ""){
			alert("Please, check DDNS user id");
			frm.txt_ddns_id.focus();
			return;
		}
		if(frm.txt_ddns_pw.value == ""){
			alert("Please, check DDNS password");
			frm.txt_ddns_pw.focus();
			return;
		}
	}

	if(frm.txt_dns_1.value == ""){
		alert("Please, check DNS address1");
		frm.txt_dns_1.focus();
		return;
	}
	if(frm.txt_dns_2.value == ""){
		alert("Please, check DNS address2");
		frm.txt_dns_2.focus();
		return;
	}

    frm.ntp_enable.value  = GetChkValue(frm.chk_ntp_enable);
	if(frm.ntp_enable.value == "1"){
		if(frm.txt_ntp_ip.value == ""){
			alert("Please, check NTP server address");
			frm.txt_ntp_ip.focus();
			return;
		}
	}

    frm.daylight_saving.value  = GetChkValue(frm.chk_daylight_enable);

	var strArray = (frm.cbo_timezone.options[frm.cbo_timezone.selectedIndex].text).split(',');
	frm.timezone_abbr.value  = strArray[1];

    isSubmitting = true;

    frm.submit();

}
function goHome() {
    window.location.href='../index.html';
}

]]>
</script>
</head>
<xsl:apply-templates />
</html>
</xsl:template>

<xsl:template match="settings_body">
	<body onload="OnLoad()" onselectstart="return false" ondragstart="return false" oncontextmenu="return false">
    <xsl:apply-templates />
</body>
</xsl:template>

<xsl:template match="settings_contents">
<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0"> <!-- 최상위 contents -->
    <tr><td>
    <form method="post" name="frm_config" action="/cgi/settings_servers_submit.cgi">
    <table align="center" >
        <tr align="center" valign="middle"><td><font size="5"><img src="/image/servers.png" height="32"/>&#160;Servers&#160;Configuration</font></td></tr>
        <tr align="center" bgcolor="#E6E6E6"><td height="1" colspan="1" bgcolor="#EDEDED"></td></tr>
        <tr align="center">
            <td>
                <table width="360px" bgcolor="#213238">
                    <tr><td colspan="2"><font size="3" color="white">Server&#160;Configurations</font></td></tr>
                    <tr>
                        <td colspan="2">
                            <input type="checkbox" id="chk_bs_enable" name="chk_bs_enable" onclick="onClickBackupServerEnable()">
                                <xsl:if test="bs/enable=1">
                                    <xsl:attribute name="checked">
                                        <xsl:value-of select="bs/enable" />
                                    </xsl:attribute>
                                </xsl:if>
                            </input>
                            <font size="3" color="white">Backup&#160;Server(FTP)</font>
                        </td>
                    </tr>
                    <tr>
                        <td width="30%"><font size="3" color="gray">IP&#160;Address</font></td>
                        <td>
                            <input type="text" id="txt_bs_ip" name="txt_bs_ip" maxlength="32" style="width:160px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="bs/serveraddr" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                    <tr>
                        <td width="30%"><font size="3" color="gray">ID</font></td>
                        <td>
                            <input type="text" id="txt_bs_id" name="txt_bs_id" maxlength="32" style="width:160px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="bs/id" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                    <tr>
                        <td width="30%"><font size="3" color="gray">Password</font></td>
                        <td>
                            <input type="password" id="txt_bs_pw" name="txt_bs_pw" maxlength="32" style="width:160px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="bs/pw" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                    <tr>
                        <td width="30%"><font size="3" color="gray">Port&#160;Number</font></td>
                        <td>
                            <input type="number" id="txt_bs_port" name="txt_bs_port" maxlength="32" style="width:160px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="bs/port" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr align="center">
            <td>
                <table width="360px" bgcolor="#213238">
                    <tr>
                        <td colspan="2">
                            <input type="checkbox" id="chk_ms_enable" name="chk_ms_enable" onclick="onClickManagementServerEnable()">
                                <xsl:if test="ms/enable=1">
                                    <xsl:attribute name="checked">
                                        <xsl:value-of select="ms/enable" />
                                    </xsl:attribute>
                                </xsl:if>
                            </input>
                            <font size="3" color="white">Management&#160;Server(HTTP)</font>
                        </td>
                    </tr>
                    <tr>
                        <td width="30%"><font size="3" color="gray">URL(IP)</font></td>
                        <td>
                            <input type="text" id="txt_ms_ip" name="txt_ms_ip" maxlength="32" style="width:160px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="ms/serveraddr" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                    <tr>
                        <td width="30%"><font size="3" color="gray">Port&#160;Number</font></td>
                        <td>
                            <input type="number" id="txt_ms_port" name="txt_ms_port" maxlength="32" style="width:160px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="ms/port" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr align="center">
            <td>
                <table width="360px" bgcolor="#213238">
                    <tr>
                        <td colspan="2">
                            <input type="checkbox" id="chk_ddns_enable" name="chk_ddns_enable" onclick="onClickDdnsEnable()">
                                <xsl:if test="ddns/enable=1">
                                    <xsl:attribute name="checked">
                                        <xsl:value-of select="ddns/enable" />
                                    </xsl:attribute>
                                </xsl:if>
                            </input>
                            <font size="3" color="white">Dynamic&#160;DNS(DDNS)</font>
                        </td>
                    </tr>
                    <tr>
                        <td width="30%"><font size="3" color="gray">Server</font></td>
                        <td>
                            <input type="text" id="txt_ddns_server" name="txt_ddns_server" maxlength="32" style="width:160px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="ddns/serveraddr" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                    <tr>
                        <td width="30%"><font size="3" color="gray">Hostname</font></td>
                        <td>
                            <input type="text" id="txt_ddns_hostname" name="txt_ddns_hostname" maxlength="32" style="width:160px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="ddns/hostname" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                    <tr>
                        <td width="30%"><font size="3" color="gray">ID</font></td>
                        <td>
                            <input type="text" id="txt_ddns_id" name="txt_ddns_id" maxlength="32" style="width:160px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="ddns/id" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                    <tr>
                        <td width="30%"><font size="3" color="gray">Password</font></td>
                        <td>
                            <input type="password" id="txt_ddns_pw" name="txt_ddns_pw" maxlength="32" style="width:160px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="ddns/pw" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr align="center">
			<td>
                <table width="360px" bgcolor="#213238">
                    <tr>
                        <td colspan="2">
                            <font size="3" color="white">DNS&#160;Server</font>
                        </td>
                    </tr>
                    <tr>
                        <td width="30%"><font size="3" color="gray">Server1</font></td>
                        <td>
                            <input type="text" id="txt_dns_1" name="txt_dns_1" maxlength="32" style="width:160px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="dns/server1" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                    <tr>
                        <td width="30%"><font size="3" color="gray">Server2</font></td>
                        <td>
                            <input type="text" id="txt_dns_2" name="txt_dns_2" maxlength="32" style="width:160px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="dns/server2" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                </table>
			</td>
		</tr>
        <tr align="center">
            <td>
                <table width="360px" bgcolor="#213238">
                    <tr>
                        <td colspan="2">
                            <input type="checkbox" id="chk_ntp_enable" name="chk_ntp_enable" onclick="onClickNtpEnable()">
                                <xsl:if test="ntp/enable=1">
                                    <xsl:attribute name="checked">
                                        <xsl:value-of select="ntp/enable" />
                                    </xsl:attribute>
                                </xsl:if>
                            </input>
                            <font size="3" color="white">Time(NTP)</font>
                        </td>
                    </tr>
                    <tr>
                        <td width="30%"><font size="3" color="gray">Time Server</font></td>
                        <td>
                            <input type="text" id="txt_ntp_ip" name="txt_ntp_ip" maxlength="32" style="width:160px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="ntp/serveraddr" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr align="center">
            <td>
                <table width="360px" bgcolor="#213238">
                    <tr>
                        <td colspan="2">
                            <input type="checkbox" id="chk_daylight_enable" name="chk_daylight_enable">
                                <xsl:if test="daylightsaving=1">
                                    <xsl:attribute name="checked">
                                        <xsl:value-of select="daylight_saving" />
                                    </xsl:attribute>
                                </xsl:if>
                            </input>
                            <font size="3" color="white">Daylight&#160;Saving</font>
                        </td>
                    </tr>
                    <tr>
                        <td width="30%"><font size="3" color="gray">Time&#160;Zone</font></td>
                        <td>
							<xsl:variable name="timezone" select='format-number(timezone, "00")' />
							<xsl:variable name="timezone_abbr" select="timezone_abbr" />
 
							<select id="cbo_timezone" name="cbo_timezone" style="width:200px">
                                <xsl:for-each select="timezone_list">
                                    <option>
                                        <xsl:attribute name="value">
											<xsl:value-of select="value" />
                                        </xsl:attribute>
										<xsl:if test="value=$timezone">
											<xsl:if test="contains(name, $timezone_abbr)">
												<xsl:attribute name="selected">selected</xsl:attribute>
											</xsl:if>
										</xsl:if>
										<xsl:value-of select="name" />
                                    </option>
                                </xsl:for-each>
                            </select>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>

        <tr align="center" bgcolor="#E6E6E6"><td height="1" colspan="1" bgcolor="#EDEDED"></td></tr><!-- horizontal line -->
        <tr align="center">
            <td>
                <input type="button" onclick="OnApply()" value="OK" style="width:100px;height:24px"></input>
                <input type="button" onclick="goHome()" value="Cancel" style="width:100px;height:24px"></input>
            </td>
        </tr>
    </table>
    <input type="hidden" id="bs_enable" name="bs_enable"></input>
    <input type="hidden" id="ms_enable" name="ms_enable"></input>
    <input type="hidden" id="ddns_enable" name="ddns_enable"></input>
    <input type="hidden" id="ntp_enable" name="ntp_enable"></input>
    <input type="hidden" id="daylight_saving" name="daylight_saving"></input>
    <input type="hidden" id="timezone_abbr" name="timezone_abbr"></input>
</form>
<input type="hidden" id="lang" name="lang"><xsl:attribute name="value"><xsl:value-of select="$lang"></xsl:value-of></xsl:attribute></input>  
<input type="hidden" id="model_name" name="model_name"><xsl:attribute name="value"><xsl:value-of select="model_name" /></xsl:attribute></input>  
    </td></tr>
</table>
</xsl:template> <!-- settings_contents -->
</xsl:stylesheet>

