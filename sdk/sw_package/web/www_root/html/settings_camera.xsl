<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html" encoding="utf-8"></xsl:output>
<xsl:variable name="language" select="/*/@lang"></xsl:variable>
<xsl:variable name="lang"><xsl:value-of select="$language" /></xsl:variable>
<xsl:variable name="strings" select="document(concat('/language/',$lang,'.xml'))/strings"></xsl:variable>
<xsl:template match="/">
<!-- start  -->
<html>
    <xsl:attribute name="lang"><xsl:value-of select="$lang" /></xsl:attribute>
    <xsl:attribute name="xml:lang"><xsl:value-of select="$lang" /></xsl:attribute>
<head>
<META HTTP-EQUIV="EXPIRES" CONTENT="0" />
<META HTTP-EQUIV="CACHE-CONTROL" CONTENT="NO-CACHE" />
<META HTTP-EQUIV="PRAGMA" CONTENT="NO-CACHE" />
<META NAME="viewport" CONTENT="user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0, width=device-width" />
<title>Camera Settings</title>
<script type="text/javascript" src="/script/utils.js" />
<script type="text/javascript">
<![CDATA[
var xmlStr;
var isSubmitting=false;
var MAX_IFRAME = 15;

function OnLoad() {
	document.title += ' - ' + document.getElementById('model_name').value;
    var lang = document.getElementById('lang').value;
    xmlStr = LoadXmlDoc('/language/'+lang+'.xml');
}

function CheckIFrameNumber(obj)
{
    var value = obj.value;

    if(value < 1){
        obj.value = 1;
    }
    if(value > MAX_IFRAME) {
        obj.value = MAX_IFRAME;
    }
}

function OnApply()
{
    if(isSubmitting == true){
	  str = GetXmlString(xmlStr, 'running_submit');
	  alert(str);
	  return;
    }

    var frm = document.frm_camera_settings;

    // TODO : make sure values are vaild

    frm.rec_fps.value = frm.rec_cbo_fps.value;
    frm.rec_bps.value = frm.rec_cbo_bps.value;
    frm.rec_gop.value = frm.rec_num_gop.value;
    frm.rec_rc.value  = frm.rec_cbo_rc.value;

    frm.stm_fps.value = frm.stm_cbo_fps.value;
    frm.stm_bps.value = frm.stm_cbo_bps.value;
    frm.stm_gop.value = frm.stm_num_gop.value;
    frm.stm_rc.value  = frm.stm_cbo_rc.value;

    frm.submit();

    isSubmitting = true;
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
    <form method="post" name="frm_camera_settings" action="/cgi/settings_camera_submit.cgi">
    <xsl:variable name="rec_fps" select="rec_fps" />
    <xsl:variable name="rec_bps" select="rec_bps" />
    <xsl:variable name="rec_rc" select="rec_rc" />
    <xsl:variable name="rec_gop" select="rec_gop" />
    <xsl:variable name="stm_fps" select="stm_fps" />
    <xsl:variable name="stm_bps" select="stm_bps" />
    <xsl:variable name="stm_rc" select="stm_rc" />
    <xsl:variable name="stm_gop" select="stm_gop" />
    <table align="center">
        <tr align="center"><td><img src="/image/camera.png" height="32"/>&#160;<font size="5">Camera&#160;Settings</font></td></tr>
        <tr align="center" bgcolor="#E6E6E6"><td height="1" colspan="1" bgcolor="#EDEDED"></td></tr>
        <tr align="center">
            <td>
                <table style="width:100%;table_layout:fixed" bgcolor="#213238">
                    <tr><td colspan="4"><font size="3" color="white">Video Quality for Recording</font></td></tr>
                    <tr>
                        <td ><font size="3" color="gray">Framereate</font></td>
                        <td >
                            <select id="rec_cbo_fps" style="width:80px">
                                <xsl:for-each select="fps_list">
                                    <option>
                                        <xsl:attribute name="value">
                                            <xsl:value-of select="value" />
                                        </xsl:attribute>
                                        <xsl:if test="value=$rec_fps">
                                            <xsl:attribute name="selected">selected</xsl:attribute>
                                        </xsl:if>
                                        <xsl:value-of select="name" />
                                    </option>
                                </xsl:for-each>
                            </select>  
                        </td>
                        <td ><font size="3" color="gray">Bitrate</font></td>
                        <td >
                            <select id="rec_cbo_bps" style="width:80px">
                                <xsl:for-each select="bps_list">
                                    <option>
                                        <xsl:attribute name="value">
                                            <xsl:value-of select="value" />
                                        </xsl:attribute>
                                        <xsl:if test="value=$rec_bps">
                                            <xsl:attribute name="selected">selected</xsl:attribute>
                                        </xsl:if>
                                        <xsl:value-of select="name" />
                                    </option>
                                </xsl:for-each>
                            </select>
                        </td>
                    </tr>
                    <tr>
                        <td><font size="3" color="gray">I-Frame Interval</font></td>
                        <td>
                            <input type="number"  id="rec_num_gop" onfocusout="CheckIFrameNumber(this);" style="width:80px" value="30" min="1" max="30" maxlength="2"><xsl:attribute name="value"><xsl:value-of select="$rec_gop" /></xsl:attribute></input>
                        </td>
                        <td ><font size="3" color="gray">Rate Control</font></td>
                        <td >
                            <select id="rec_cbo_rc"  style="width:80px">
                                <xsl:for-each select="rc_list">
                                    <option>
                                        <xsl:attribute name="value">
                                            <xsl:value-of select="value" />
                                        </xsl:attribute>
                                        <xsl:if test="value=$rec_rc">
                                            <xsl:attribute name="selected">selected</xsl:attribute>
                                        </xsl:if>
                                        <xsl:value-of select="name" />
                                    </option>
                                </xsl:for-each>
                            </select>  
                        </td>
                    </tr>
                    <tr><td>&#160;</td></tr>
                </table>
            </td>
        </tr>
        <tr align="center">
            <td>
                <table style="width:100%;table_layout:fixed" bgcolor="#213238">
                    <tr><td colspan="4"><font size="3" color="white">Video Quality for Streaming</font></td></tr>
                    <tr>
                        <td ><font size="3" color="gray">Framereate</font></td>
                        <td >
                            <select id="stm_cbo_fps" style="width:80px">
                                <xsl:for-each select="fps_list">
                                    <option>
                                        <xsl:attribute name="value">
                                            <xsl:value-of select="value" />
                                        </xsl:attribute>
                                        <xsl:if test="value=$stm_fps">
                                            <xsl:attribute name="selected">selected</xsl:attribute>
                                        </xsl:if>
                                        <xsl:value-of select="name" />
                                    </option>
                                </xsl:for-each>
                            </select>  
                        </td>
                        <td ><font size="3" color="gray">Bitrate</font></td>
                        <td >
                            <select id="stm_cbo_bps" style="width:80px">
                                <xsl:for-each select="bps_list">
                                    <option>
                                        <xsl:attribute name="value">
                                            <xsl:value-of select="value" />
                                        </xsl:attribute>
                                        <xsl:if test="value=$stm_bps">
                                            <xsl:attribute name="selected">selected</xsl:attribute>
                                        </xsl:if>
                                        <xsl:value-of select="name" />
                                    </option>
                                </xsl:for-each>
                            </select>  
                        </td>
                    </tr>
                    <tr>
                        <td><font size="3" color="gray">I-Frame Interval</font></td>
                        <td>
                            <input type="number" id="stm_num_gop" onfocusout="CheckIFrameNumber(this);" style="width:80px" value="30" min="1" max="30" maxlength="2"><xsl:attribute name="value"><xsl:value-of select="$stm_gop" /></xsl:attribute></input>
                        </td>
                        <td ><font size="3" color="gray">Rate Control</font></td>
                        <td >
                            <select id="stm_cbo_rc" style="width:80px">
                                <xsl:for-each select="rc_list">
                                    <option>
                                        <xsl:attribute name="value">
                                            <xsl:value-of select="value" />
                                        </xsl:attribute>
                                        <xsl:if test="value=$stm_rc">
                                            <xsl:attribute name="selected">selected</xsl:attribute>
                                        </xsl:if>
                                        <xsl:value-of select="name" />
                                    </option>
                                </xsl:for-each>
                            </select>
                        </td>
                    </tr>
                    <tr><td>&#160;</td></tr>
                </table>
            </td>
        </tr>
        <tr align="center" bgcolor="#E6E6E6"><td height="1" colspan="1" bgcolor="#EDEDED"></td></tr><!-- horizontal line -->
        <tr align="center">
            <td>
                <input type="button" onclick="OnApply()" value="OK" style="width:100px;height:24px" />
                <input type="button" onclick="goHome()" value="Cancel" style="width:100px;height:24px" />
            </td>
        </tr>
    </table>
    <input type="hidden" id="rec_fps" name="rec_fps"><xsl:attribute name="value"><xsl:value-of select="$rec_fps" /></xsl:attribute></input>
    <input type="hidden" id="rec_bps" name="rec_bps"><xsl:attribute name="value"><xsl:value-of select="$rec_bps" /></xsl:attribute></input>
    <input type="hidden" id="rec_gop" name="rec_gop"><xsl:attribute name="value"><xsl:value-of select="$rec_gop" /></xsl:attribute></input>
    <input type="hidden" id="rec_rc"  name="rec_rc" ><xsl:attribute name="value"><xsl:value-of select="$rec_rc" /></xsl:attribute></input>
    <input type="hidden" id="stm_fps" name="stm_fps"><xsl:attribute name="value"><xsl:value-of select="$stm_fps" /></xsl:attribute></input>
    <input type="hidden" id="stm_bps" name="stm_bps"><xsl:attribute name="value"><xsl:value-of select="$stm_bps" /></xsl:attribute></input>
    <input type="hidden" id="stm_gop" name="stm_gop"><xsl:attribute name="value"><xsl:value-of select="$stm_gop" /></xsl:attribute></input>
    <input type="hidden" id="stm_rc"  name="stm_rc" ><xsl:attribute name="value"><xsl:value-of select="$stm_rc" /></xsl:attribute></input>
</form>
<input type="hidden" id="lang" name="lang"><xsl:attribute name="value"><xsl:value-of select="$lang" /></xsl:attribute></input>  
<input type="hidden" id="model_name" name="model_name"><xsl:attribute name="value"><xsl:value-of select="model_name" /></xsl:attribute></input>  
    </td></tr>
</table>
</xsl:template> <!-- settings_contents -->
</xsl:stylesheet>

