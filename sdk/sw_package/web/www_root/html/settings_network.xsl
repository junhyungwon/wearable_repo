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
<title>Network Settings</title>
<script type="text/javascript" src="/script/utils.js"></script>
<script type="text/javascript">
<![CDATA[
var xmlStr;
var isSubmitting=false;

function OnLoad() {
	document.title += ' - ' + document.getElementById('model_name').value;
    var lang = document.getElementById('lang').value;
    xmlStr = LoadXmlDoc('/language/'+lang+'.xml');

	OnChangeWirelessIpType();
	OnChangeCradleIpType();
	onClickLiveStreamAccount();
}

function OnApply()
{
    if(isSubmitting == true){
	  str = GetXmlString(xmlStr, 'running_submit');
	  alert(str);
	  return;
    }

    var frm = document.frm_network_config;

    // TODO : make sure values are vaild
	frm.live_stream_account_enable.value=GetChkValue(frm.chk_live_stream_account);

	if(frm.live_stream_account_enable.value == 1){
		if(frm.txt_live_stream_account_id.value == ""){
			alert("Please, check Live Stream ID");
			frm.txt_live_stream_account_id.focus();
			return;
		}
		if(frm.txt_live_stream_account_pw.value == ""){
			alert("Please, check Live Stream Password");
			frm.txt_live_stream_account_pw.focus();
			return;
		}
	}

    isSubmitting = true;

    frm.submit();

}
function goHome() {
    window.location.href='../index.html';
}

function OnChangeWirelessIpType() {
    var frm = document.frm_network_config;
	var mode = frm.cbo_wireless_ip_type.value; //0:static, 1:dhcp

	if(mode == 1){
		DisableObj(frm.txt_wireless_ipv4);
		DisableObj(frm.txt_wireless_gw);
		DisableObj(frm.txt_wireless_mask);
	} else {
		EnableObj(frm.txt_wireless_ipv4);
		EnableObj(frm.txt_wireless_gw);
		EnableObj(frm.txt_wireless_mask);
	}
}
function OnChangeCradleIpType() {
    var frm = document.frm_network_config;
	var mode = frm.cbo_cradle_ip_type.value; //0:static, 1:dhcp

	if(mode == 1){
		DisableObj(frm.txt_cradle_ipv4);
		DisableObj(frm.txt_cradle_gw);
		DisableObj(frm.txt_cradle_mask);
	} else {
		EnableObj(frm.txt_cradle_ipv4);
		EnableObj(frm.txt_cradle_gw);
		EnableObj(frm.txt_cradle_mask);
	}
}
function onClickLiveStreamAccount() {
    var frm = document.frm_network_config;

	if(frm.chk_live_stream_account.checked){
		EnableObj(frm.cbo_live_stream_account_enc_type);
		EnableObj(frm.txt_live_stream_account_id);
		EnableObj(frm.txt_live_stream_account_pw);
	}
	else {
		DisableObj(frm.cbo_live_stream_account_enc_type);
		DisableObj(frm.txt_live_stream_account_id);
		DisableObj(frm.txt_live_stream_account_pw);
	}
}
function OnChangeLiveStreamAccountEncType() {
    var frm = document.frm_network_config;
	var mode = frm.cbo_live_stream_account_enc_type.value; //0:NONE, 1:AES
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
    <form method="post" name="frm_network_config" action="/cgi/settings_network_submit.cgi">
    <table align="center" >
        <tr align="center"><td><font size="5">Network&#160;Configurations</font></td></tr>
        <tr align="center" bgcolor="#E6E6E6"><td height="1" colspan="1" bgcolor="#EDEDED"></td></tr>
        <tr align="center">
            <td>
                <table width="360px" bgcolor="#213238">
                    <tr><td colspan="2"><font size="3" color="white">Network&#160;Settings</font></td></tr>
                    <tr>
                        <td colspan="2">
                            <font size="3" color="white">&#160;<xsl:value-of select="model_name" />&#160;Device&#160;(Wireless)</font>
                        </td>
                    </tr>
                    <tr>
                        <td><font size="3" color="gray">&#160;IP Type</font></td>
                        <td>
							<xsl:variable name="wireless_addr_type" select="wireless/addr_type" />
                            <select id="cbo_wireless_ip_type" name="cbo_wireless_ip_type" style="width:100px" onChange="OnChangeWirelessIpType()">
                                <xsl:for-each select="ip_type_list">
                                    <option>
                                        <xsl:attribute name="value">
                                            <xsl:value-of select="value"></xsl:value-of>
                                        </xsl:attribute>
                                        <xsl:if test="value=$wireless_addr_type">
                                            <xsl:attribute name="selected">selected</xsl:attribute>
                                        </xsl:if>
                                        <xsl:value-of select="name"></xsl:value-of>
                                    </option>
                                </xsl:for-each>
                            </select>  
                        </td>
                    </tr>
                    <tr>
                        <td width="30%"><font size="3" color="gray">&#160;IP Address</font></td>
                        <td >
                            <input type="text" id="txt_wireless_ipv4" name="txt_wireless_ipv4" maxlength="16" style="width:200px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="wireless/ipv4" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                    <tr>
                        <td ><font size="3" color="gray">&#160;Gateway</font></td>
                        <td >
                            <input type="text" id="txt_wireless_gw" name="txt_wireless_gw" maxlength="16" style="width:200px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="wireless/gateway" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                    <tr>
                        <td ><font size="3" color="gray">&#160;Netmask</font></td>
                        <td >
                            <input type="text" id="txt_wireless_mask" name="txt_wireless_mask" maxlength="16" style="width:200px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="wireless/netmask" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                    <tr><td colspan="2">&#160;</td></tr>
                    <tr>
                        <td colspan="2">
                            <font size="3" color="white">&#160;Cradle (Wired)</font>
                        </td>
                    </tr>
                    <tr>
                        <td><font size="3" color="gray">&#160;IP Type</font></td>
                        <td>
							<xsl:variable name="cradle_addr_type" select="cradle/addr_type" />
                            <select id="cbo_cradle_ip_type" name="cbo_cradle_ip_type" style="width:100px" onChange="OnChangeCradleIpType()">
                                <xsl:for-each select="ip_type_list">
                                    <option>
                                        <xsl:attribute name="value">
                                            <xsl:value-of select="value"></xsl:value-of>
                                        </xsl:attribute>
                                        <xsl:if test="value=$cradle_addr_type">
                                            <xsl:attribute name="selected">selected</xsl:attribute>
                                        </xsl:if>
                                        <xsl:value-of select="name"></xsl:value-of>
                                    </option>
                                </xsl:for-each>
                            </select>  
                        </td>
                    </tr>
                    <tr>
                        <td ><font size="3" color="gray">&#160;IP Address</font></td>
                        <td >
                            <input type="text" id="txt_cradle_ipv4" name="txt_cradle_ipv4" maxlength="16" style="width:200px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="cradle/ipv4" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                    <tr>
                        <td ><font size="3" color="gray">&#160;Gateway</font></td>
                        <td >
                            <input type="text" id="txt_cradle_gw" name="txt_cradle_gw" maxlength="16" style="width:200px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="cradle/gateway" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                    <tr>
                        <td ><font size="3" color="gray">&#160;Netmask</font></td>
                        <td >
                            <input type="text" id="txt_cradle_mask" name="txt_cradle_mask" maxlength="16" style="width:200px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="cradle/netmask" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                    <tr><td colspan="2">&#160;</td></tr>
                    <tr>
                        <td colspan="2">
                            <font size="3" color="white">&#160;WIFI Access Point(Wired)</font>
                        </td>
                    </tr>
                    <tr>
                        <td ><font size="3" color="gray">&#160;SSID</font></td>
                        <td >
                            <input type="text" id="txt_wifi_ap_ssid" name="txt_wifi_ap_ssid" maxlength="16" style="width:200px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="wifi_ap/ssid" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                    <tr>
                        <td ><font size="3" color="gray">&#160;Password</font></td>
                        <td >
                            <input type="password" id="txt_wifi_ap_pw" name="txt_wifi_ap_pw" maxlength="16" style="width:200px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="wifi_ap/pw" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                    <tr><td colspan="2">&#160;</td></tr>
                    <tr>
                        <td colspan="2">&#160;
							<input type="checkbox" id="chk_live_stream_account" name="chk_live_stream_account" onclick="onClickLiveStreamAccount()">
                                <xsl:if test="live_stream_account/enable=1">
                                <xsl:attribute name="checked">
                                    <xsl:value-of select="live_stream_account/enable" />
                                </xsl:attribute>
                            </xsl:if>
                            </input>
                            <font size="3" color="white">Live Stream Account</font>
                        </td>
                    </tr>
                    <tr>
                        <td ><font size="3" color="gray">&#160;Encryption</font></td>
                        <td >
							<xsl:variable name="live_stream_account_enc_type" select="live_stream_account/enc_type" />
                            <select id="cbo_live_stream_account_enc_type" name="cbo_live_stream_account_enc_type" style="width:100px" onChange="OnChangeLiveStreamAccountEncType()">
                                <xsl:for-each select="enc_type_list">
                                    <option>
                                        <xsl:attribute name="value">
                                            <xsl:value-of select="value"></xsl:value-of>
                                        </xsl:attribute>
                                        <xsl:if test="value=$live_stream_account_enc_type">
                                            <xsl:attribute name="selected">selected</xsl:attribute>
                                        </xsl:if>
                                        <xsl:value-of select="name"></xsl:value-of>
                                    </option>
                                </xsl:for-each>
                            </select>  
                        </td>
                    </tr>
                    <tr>
                        <td ><font size="3" color="gray">&#160;ID</font></td>
                        <td >
                            <input type="text" id="txt_live_stream_account_id" name="txt_live_stream_account_id" maxlength="16" style="width:200px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="live_stream_account/id" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                    <tr>
                        <td ><font size="3" color="gray">&#160;Password</font></td>
                        <td >
                            <input type="password" id="txt_live_stream_account_pw" name="txt_live_stream_account_pw" maxlength="16" style="width:200px">
                                <xsl:attribute name="value">
                                    <xsl:value-of select="live_stream_account/pw" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                    <tr><td colspan="2">&#160;</td></tr>
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
	<input type="hidden" id="live_stream_account_enable" name="live_stream_account_enable"></input>  
	</form>
	<input type="hidden" id="lang" name="lang"><xsl:attribute name="value"><xsl:value-of select="$lang"></xsl:value-of></xsl:attribute></input>
	<input type="hidden" id="model_name" name="model_name"><xsl:attribute name="value"><xsl:value-of select="model_name" /></xsl:attribute></input>  
    </td></tr>
</table>
</xsl:template> <!-- settings_contents -->
</xsl:stylesheet>

