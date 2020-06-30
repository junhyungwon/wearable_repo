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
<title>System Settings</title>
<script type="text/javascript" src="/script/utils.js"></script>
<script type="text/javascript">
<![CDATA[
var xmlStr;
var isSubmitting=false;
var isNEXX360=false;

function OnLoad() {
	if(document.getElementById('model_name').value == 'NEXX360'){
		isNEXX360 = true;
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

	if(frm.txt_devid.value == "") {
		alert("Please, check Device ID");
		frm.txt_devid.focus();
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
function onClickSetToDefault() {
	if(confirm('The system will restart, Do you want to process it?')){
		location.href="/cgi/cmd.cgi?action=sysmng&param=factorydefault_hard";
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
	<xsl:variable name="MODEL_NAME" select="model_name" />

<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0"> <!-- 최상위 contents -->
    <tr><td>
    <form method="post" name="frm_config" action="/cgi/settings_system_submit.cgi">
    <table align="center" >
        <tr align="center"><td><font size="5">System</font></td></tr>
        <tr align="center" bgcolor="#E6E6E6"><td height="1" colspan="1" bgcolor="#EDEDED"></td></tr>
        <tr align="center">
            <td>
                <table class="tbl_1">
                    <tr><td colspan="3"><font size="3" color="white">&#160;Information</font></td></tr>
                    <tr>
                        <td><font size="3" color="gray">&#160;&#160;Model</font></td>
                        <td colspan="2">
                            <input type="text" id="txt_model" name="txt_model" readonly="true" style="background-color:#c0c0c0;">
                                <xsl:attribute name="value">
									<xsl:value-of select="system/model" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                    <tr>
                        <td><font size="3" color="gray">&#160;&#160;Firmware&#160;Ver.</font></td>
                        <td colspan="2">
                            <input type="text" id="txt_fwver" name="txt_fwver" readonly="true" style="background-color:#c0c0c0;">
                                <xsl:attribute name="value">
									<xsl:value-of select="system/fwver" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr>
                    <tr>
                        <td><font size="3" color="gray">&#160;&#160;Device&#160;ID</font></td>
                        <td colspan="2">
                            <input type="text" id="txt_devid" name="txt_devid">
                                <xsl:attribute name="value">
									<xsl:value-of select="system/devid" />
                                </xsl:attribute>
                            </input>
                            &#160;
							<input type="button" onclick="onMactoID()" value="MAC to ID" style="height:20px"/>
						</td>
                    </tr>
			<xsl:if test="$MODEL_NAME='NEXX360'">
                    <tr>
                        <td><font size="3" color="gray">&#160;&#160;UID</font></td>
                        <td colspan="2">
                            <input type="text" id="txt_uid" name="txt_uid">
                                <xsl:attribute name="value">
									<xsl:value-of select="system/uid" />
                                </xsl:attribute>
                            </input>
						</td>
                    </tr>
			</xsl:if>
                    <tr><td colspan="3">&#160;</td></tr>
					<tr>
						<td colspan="3" align="center">
							<font size="3" color="gray">
								<a href="#" onclick="onClickSetToDefault();">
									Set to Default(reset all configurations)
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
<input type="hidden" id="mac" name="mac">
	<xsl:attribute name="value"><xsl:value-of select="system/mac"></xsl:value-of></xsl:attribute>
</input>  
<input type="hidden" id="lang" name="lang"><xsl:attribute name="value"><xsl:value-of select="$lang"></xsl:value-of></xsl:attribute></input>  
<input type="hidden" id="model_name" name="model_name"><xsl:attribute name="value"><xsl:value-of select="model_name" /></xsl:attribute></input>  
    </td></tr>
</table>
</xsl:template> <!-- settings_contents -->
</xsl:stylesheet>

