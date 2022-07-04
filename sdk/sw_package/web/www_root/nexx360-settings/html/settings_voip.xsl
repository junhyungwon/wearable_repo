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
<title>VOIP Settings</title>
<script type="text/javascript" src="/script/utils.js"></script>
<script type="text/javascript">
<![CDATA[
var xmlStr;
var isSubmitting=false;
var ModelType=0; // 0:FITT360, 1:NEXX360, 2:NEXXONE

function OnLoad() {
	if(document.getElementById('model_name').value == 'FITT360'){
		ModelType = 0;
	}
	else if(document.getElementById('model_name').value == 'NEXX360'){
		ModelType = 1;
	}
	else if(document.getElementById('model_name').value == 'NEXXONE'){
		ModelType = 2;
	}

	document.title += ' - ' + document.getElementById('model_name').value;
    var lang = document.getElementById('lang').value;
    xmlStr = LoadXmlDoc('/language/'+lang+'.xml');
}

function OnApply()
{
    if(isSubmitting == true){
	  str = GetXmlString(xmlStr, 'running_submit');
	  alert(str);
	  return;
    }

    var frm = document.frm_config;

	if(frm.txt_voip_ip.value == "") {
		alert("Please, check IP Address");
		frm.txt_voip_ip.focus();
		return;
	}
	if(frm.txt_voip_port.value == "") {
		alert("Please, check Port");
		frm.txt_voip_port.focus();
		return;
	}
	if(frm.txt_voip_id.value == "") {
		alert("Please, check ID");
		frm.txt_voip_id.focus();
		return;
	}
	if(frm.txt_voip_pw.value == "") {
		alert("Please, check Password");
		frm.txt_voip_pw.focus();
		return;
	}
	if(frm.txt_voip_peerid.value == "") {
		alert("Please, check Peer ID");
		frm.txt_voip_peerid.focus();
		return;
	}

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
	<xsl:variable name="MODEL_NAME" select="model_name" />

<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0"> <!-- 최상위 contents -->
    <tr><td>
    <form method="post" name="frm_config" action="/cgi/settings_voip_submit.cgi">
    <table align="center" >
        <tr align="center"><td><font size="5">VOIP</font></td></tr>
        <tr align="center" bgcolor="#E6E6E6"><td height="1" colspan="1" bgcolor="#EDEDED"></td></tr>
        <tr align="center">
            <td>
                <table class="tbl_1">
                    <tr><td colspan="3"><font size="3" color="white">&#160;VOIP Configuration</font></td></tr>
                    <tr>
                        <td><font size="3" color="gray">&#160;&#160;IP&#160;Address</font></td>
                        <td colspan="2">
                            <input type="text" id="txt_voip_ip" name="txt_voip_ip">
                                <xsl:attribute name="value">
									<xsl:value-of select="voip/ip" />
                                </xsl:attribute>
                            </input>
						</td>
                    </tr>
                    <tr>
                        <td><font size="3" color="gray">&#160;&#160;Port</font></td>
                        <td colspan="2">
                            <input type="text" id="txt_voip_port" name="txt_voip_port">
                                <xsl:attribute name="value">
									<xsl:value-of select="voip/port" />
                                </xsl:attribute>
                            </input>
						</td>
                    </tr>
                    <tr>
                        <td><font size="3" color="gray">&#160;&#160;ID</font></td>
                        <td colspan="2">
                            <input type="text" id="txt_voip_id" name="txt_voip_id">
                                <xsl:attribute name="value">
									<xsl:value-of select="voip/id" />
                                </xsl:attribute>
                            </input>
						</td>
                    </tr>
                    <tr>
                        <td><font size="3" color="gray">&#160;&#160;Password</font></td>
                        <td colspan="2">
                            <input type="text" id="txt_voip_pw" name="txt_voip_pw">
                                <xsl:attribute name="value">
									<xsl:value-of select="voip/pw" />
                                </xsl:attribute>
                            </input>
						</td>
                    </tr>
                    <tr>
                        <td><font size="3" color="gray">&#160;&#160;Peer&#160;ID</font></td>
                        <td colspan="2">
                            <input type="text" id="txt_voip_peerid" name="txt_voip_peerid">
                                <xsl:attribute name="value">
									<xsl:value-of select="voip/peerid" />
                                </xsl:attribute>
                            </input>
						</td>
                    </tr>
                    <tr><td colspan="3">&#160;</td></tr>
					<tr>
						<td colspan="3" align="center">
							<font size="3" color="gray">
								<a href="#">
								</a>
							</font>
						</td>
					</tr>
                    <tr><td colspan="3">&#160;</td></tr>
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
<input type="hidden" id="lang" name="lang"><xsl:attribute name="value"><xsl:value-of select="$lang"></xsl:value-of></xsl:attribute></input>  
<input type="hidden" id="model_name" name="model_name"><xsl:attribute name="value"><xsl:value-of select="model_name" /></xsl:attribute></input>  
    </td></tr>
</table>
</xsl:template> <!-- settings_contents -->
</xsl:stylesheet>

