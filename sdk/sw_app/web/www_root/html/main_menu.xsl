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
<title>LINKFLOW</title>
<script type="text/javascript" src="/script/utils.js"></script>
<script type="text/javascript">
<![CDATA[
var xmlStr;
var isSubmitting=false;

function OnLoad() {
	document.title += ' - ' + document.getElementById('model_name').value;
    var lang = document.getElementById('lang').value;
    xmlStr = LoadXmlDoc('/language/'+lang+'.xml');
}

function onApply() {
	if(confirm('The system will restart. Do you want to process it?')){
		location.href="/cgi/cmd.cgi?action=sysmng&param=restart";
		return true;
	}else {
		return false;
	}
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
    <table align="center" >
        <tr align="center">
            <td>
					<table align="center" width="360" border="0" cellpadding="0" cellspacing="0">
						<tr align="center"><td height="10" colspan="2"></td></tr>
						<tr align="center"><td valign="top"><font size="6"><xsl:value-of select="model_name" /></font></td></tr>
						<tr><td>&#160;</td></tr>
						<tr bgcolor="#E6E6E6"><td align="center" height="1" colspan="2" bgcolor="#EDEDED"></td></tr>
						<tr><td>&#160;</td></tr>
						<tr align="center"><td><a href="/cgi/settings_camera.cgi"><font size="5">Camera Settings</font></a></td></tr>
						<tr align="center"><td><a href="/cgi/settings_operation.cgi"><font size="5">Operation Settings</font></a></td></tr>
						<tr align="center"><td><a href="/cgi/settings_network.cgi"><font size="5">Network Settings</font></a></td></tr>
						<tr align="center"><td><a href="/cgi/settings_servers.cgi"><font size="5">Servers Settings</font></a></td></tr>
						<tr align="center"><td><a href="/cgi/settings_system.cgi"><font size="5">System Settings</font></a></td></tr>
						<tr align="center"><td><a href="/cgi/settings_user.cgi"><font size="5">User Settings</font></a></td></tr>
						<tr align="center"><td>&#160;</td></tr>
						<tr align="center"><td><a href="#" onclick="onApply();"><font size="5">Apply(Restart)</font></a></td></tr>
						<!--
						<tr align="center"><td><a href="/html/preview.html">JPEG Preview</a></td></tr>
						<tr align="center"><td>About...</td></tr>
						-->
						<tr align="center"><td>&#160;</td></tr>
						<tr><td></td></tr>
						<tr><td>&#160;</td></tr>
					</table>
				<table align="center" cellspacing="3">
					<tr align="center">
						<td valign="top"><a href="https://www.linkflow.co.kr/" target="_top"><img width="80%" src="/image/linkflow-252x30.png"/></a></td>
					</tr>
					<tr align="center"><td>&#160;</td></tr>
					<tr>
						<td align="center"><a href="https://www.onvif.org/profiles/profile-s/" target="_top"><img width="50%" src="/image/onvif-logo-profile-s.jpg"/></a></td>
					</tr>
				</table>
				<table align="center">
					<tr align="center"><td>&#160;</td></tr>
					<tr align="center">
						<td><img src="/image/fitt360-security.png" width="260px" /> </td>
					</tr>
				</table>
<input type="hidden" id="lang" name="lang"><xsl:attribute name="value"><xsl:value-of select="$lang"></xsl:value-of></xsl:attribute></input>
<input type="hidden" id="model_name" name="model_name"><xsl:attribute name="value"><xsl:value-of select="model_name" /></xsl:attribute></input>  
			</td>
        </tr>
    </table>
    </td></tr>
</table>
</xsl:template> <!-- settings_contents -->
</xsl:stylesheet>

