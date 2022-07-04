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
<title>User Management</title>
<script type="text/javascript" src="/script/utils.js"></script>
<script type="text/javascript">
<![CDATA[
var xmlStr;
var isSubmitting=false;

function OnLoad() {
	document.title += ' - ' + document.getElementById('model_name').value;
    var lang = document.getElementById('lang').value;
    xmlStr = LoadXmlDoc('/language/'+lang+'.xml');

  	var form = document.frm_config;
    form.txt_cur_pw.focus();
}

function OnApply()
{
    if(isSubmitting == true){
	  str = GetXmlString(xmlStr, 'running_submit');
	  alert(str);
	  return;
    }

    var frm = document.frm_config;

	if(frm.txt_cur_pw.value == "") {
		alert("Please, check current password");
		frm.txt_cur_pw.focus();
		return;
	}

	if(frm.txt_new_pw1.value == "") {
		alert("Please, check new password");
		frm.txt_new_pw1.focus();
		return;
	}
	if(frm.txt_new_pw2.value == "") {
		alert("Please, check confirm password");
		frm.txt_new_pw2.focus();
		return;
	}

	if(frm.txt_new_pw1.value != frm.txt_new_pw2.value) {
		alert("Please, check new password and confirm password");
		frm.txt_new_pw1.focus();
		return;
	}

	if(frm.txt_cur_pw.value == frm.txt_new_pw1.value) {
		alert("The new password is the same as current one.");
		frm.txt_cur_pw.focus();
		return;
	}

    isSubmitting = true;

    frm.submit();

}
function goHome() {
    window.location.href='../index.html';
}
function onMactoID() {
    var frm = document.frm_config;

	//frm.txt_devid.value = document.getElementById('mac').value;
	frm.txt_devid.value = "_MACADDRESS_";
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
    <form method="post" name="frm_config" action="/cgi/settings_user_submit.cgi">
    <table align="center" >
		<tr align="center"><td><font size="5">User&#160;Management</font></td></tr>
        <tr align="center" bgcolor="#E6E6E6"><td height="1" colspan="1" bgcolor="#EDEDED"></td></tr>
        <tr align="center">
            <td>
                <table width="360" bgcolor="#213238">
                    <tr><td colspan="2"><font size="4" color="white">&#160;Change&#160;Administrator&#160;Password</font></td></tr>
                    <tr><td colspan="2">&#160;</td></tr>
                    <tr>
                        <td align="right"><font size="3" color="gray">Current&#160;&#160;</font></td>
						<td><input type="Password" id="txt_cur_pw" name="txt_cur_pw"></input></td>
                    </tr>
                    <tr>
                        <td align="right"><font size="3" color="gray">New&#160;&#160;</font></td>
						<td><input type="Password" id="txt_new_pw1" name="txt_new_pw1"></input></td>
                    </tr>
                    <tr>
                        <td align="right"><font size="3" color="gray">Confirm&#160;&#160;</font></td>
						<td><input type="Password" id="txt_new_pw2" name="txt_new_pw2"></input></td>
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
</form>
<input type="hidden" id="mac" name="mac">
	<xsl:attribute name="value"><xsl:value-of select="system/mac"></xsl:value-of></xsl:attribute>
</input>  
<input type="hidden" id="lang" name="lang"><xsl:attribute name="value"><xsl:value-of select="$lang"></xsl:value-of></xsl:attribute></input>  
<input type="hidden" id="model_name" name="model_name"><xsl:attribute name="value"><xsl:value-of select="model_name" /></xsl:attribute></input>  
    </td></tr>
</table>
</xsl:template> <!-- settings_contents -->
</xsl:stylesheet>

