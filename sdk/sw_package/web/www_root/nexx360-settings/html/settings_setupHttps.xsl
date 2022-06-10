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

function openCountryList()
{
    var w = 570;
    var h = 350;
    var url = '/html/CountryList.html';
    var title = 'Country List';
    var left = (screen.width/2)-(w/2);
    var top = (screen.height/2)-(h/2);

    return window.open(url, title, 'toolbar=no, location=no, directories=no, status=no, menubar=no, scrollbars=no, resizable=no, copyhistory=no, width='+w+', height='+h+', top='+top+', left='+left);

    //return window.open(url, title, "resizable = yes, scrollbars = yes, width='+w+', height='+h+', top='+top+', left='+left");
}

function OnApply()
{
    if(isSubmitting == true){
	  str = GetXmlString(xmlStr, 'running_submit');
	  alert(str);
	  return;
    }

    var frm = document.frm_config;

    isSubmitting = true;

    frm.submit();

}
function goHome() {
    window.location.href='/';
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
    <form method="post" name="frm_config" action="/cgi/settings_setupHttps_submit.cgi">
    <table align="center" >
        <tr align="center"><td><font size="5">Self Signed Certificate</font></td></tr>
        <tr align="center" bgcolor="#E6E6E6"><td height="1" colspan="1" bgcolor="#EDEDED"></td></tr>
        <tr align="center">
            <td>
                <table class="tbl_1">
                    <tr><td colspan="3"><font size="3" color="white">&#160;Information</font></td></tr>
                    <!-- tr>
                        <td><font size="3" color="gray">&#160;&#160;Model</font></td>
                        <td colspan="2">
                            <input type="text" id="txt_model" name="txt_model" readonly="true" style="background-color:#c0c0c0;">
                                <xsl:attribute name="value">
									<xsl:value-of select="model_name" />
                                </xsl:attribute>
                            </input>
                        </td>
                    </tr -->
                    <tr>
                        <td><font size="3" color="gray">&#160;&#160;C</font></td>
                        <td colspan="2">
                            <input type="text" id="txt_C" name="txt_C" style="width:240px;" placeholder="Country Name(2 letter code)">
                                <xsl:attribute name="value">
									<xsl:value-of select="setupHttps/C" />
                                </xsl:attribute>
                            </input>
                            &#160;
                            <a href="javascript:openCountryList()">❔</a>
						</td>
                    </tr>
                    <tr>
                        <td><font size="3" color="gray">&#160;&#160;ST</font></td>
                        <td colspan="2">
                            <input type="text" id="txt_ST" name="txt_ST" style="width:240px;" placeholder="State or Province Name(full name)">
                                <xsl:attribute name="value">
									<xsl:value-of select="setupHttps/ST" />
                                </xsl:attribute>
                            </input>
						</td>
                    </tr>
                    <tr>
                        <td><font size="3" color="gray">&#160;&#160;L</font></td>
                        <td colspan="2">
                            <input type="text" id="txt_L" name="txt_L" style="width:240px;" placeholder="Location (e.g. city)">
                                <xsl:attribute name="value">
									<xsl:value-of select="setupHttps/L" />
                                </xsl:attribute>
                            </input>
						</td>
                    </tr>
                    <tr>
                        <td><font size="3" color="gray">&#160;&#160;O</font></td>
                        <td colspan="2">
                            <input type="text" id="txt_O" name="txt_O" style="width:240px;" placeholder="Organization Name(e.g. company)">
                                <xsl:attribute name="value">
									<xsl:value-of select="setupHttps/O" />
                                </xsl:attribute>
                            </input>
						</td>
                    </tr>
                    <tr>
                        <td><font size="3" color="gray">&#160;&#160;OU</font></td>
                        <td colspan="2">
                            <input type="text" id="txt_OU" name="txt_OU" style="width:240px;" placeholder="Organization Unit Name (e.g. section)">
                                <xsl:attribute name="value">
									<xsl:value-of select="setupHttps/OU" />
                                </xsl:attribute>
                            </input>
						</td>
                    </tr>
                    <tr>
                        <td><font size="3" color="gray">&#160;&#160;CN</font></td>
                        <td colspan="2">
                            <input type="text" id="txt_CN" name="txt_CN" style="width:240px;" placeholder="Common Name (e.g. server FQDN or YOUR name)">
                                <xsl:attribute name="value">
									<xsl:value-of select="setupHttps/CN" />
                                </xsl:attribute>
                            </input>
						</td>
                    </tr>
                    <tr>
                        <td><font size="3" color="gray">&#160;&#160;✉</font></td>
                        <td colspan="2">
                            <input type="text" id="txt_Email" name="txt_Email" style="width:240px;" placeholder="Email Address">
                                <xsl:attribute name="value">
									<xsl:value-of select="setupHttps/Email" />
                                </xsl:attribute>
                            </input>
						</td>
                    </tr>
                    <tr><td colspan="3">&#160;</td></tr>
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

