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

<script type="text/javascript" src="/html/script/utils.js" />
<script type="text/javascript" src="/html/script/setup.js" />
<script type="text/javascript">
<![CDATA[
var xmlStr;
var isSubmitting=false;
var isNEXX360=false;

function init()
{
    var form = document.setupHttps;
    SetRadioValue(form.service_type, form.http_mode.value);
    SetMode(form.http_mode.value);

    document.install_cert.txt_cert_name.value = form.cert_name.value;

}

function OnLoad() {
	if(document.getElementById('model_name').value == 'NEXX360'){
		isNEXX360 = true;
	}
	document.title += ' - ' + document.getElementById('model_name').value;
    //var lang = document.getElementById('lang').value;
    //xmlStr = LoadXmlDoc('/language/'+lang+'.xml');

    init();
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
	  //str = GetXmlString(xmlStr, 'running_submit');
      str = "Please, wait for a minute";
	  alert(str);
	  return;
    }

    var form = document.setupHttps;

    if ( form.http_mode.value == 1 
    && ( form.txt_C.value == 0
		|| form.txt_ST.value == 0
		|| form.txt_L.value == 0
		|| form.txt_O.value == 0
		|| form.txt_OU.value == 0
		|| form.txt_CN.value == 0 
		|| form.txt_Email.value == 0 )
    ) {
        alert("Please, check Self Signed Certificate values");
        return;
    }
    else if ( form.http_mode.value == 2) {

        if( form.is_cert.value == 0) {
            str = "Please, register Certificate!";
            alert(str);
            return;
        }
    }

    isSubmitting = true;
    form.submit();
}

function goHome() {
    window.location.href='/';
}

function OnInstall() {
    var form = document.setupHttps;

    form.is_cert = 1;

}

function EnableControlsHttpsSelfSigned(enable){
    if(enable){
     	var el = document.getElementById("div_https_self_signed");
		//el.setAttribute('style', 'display:inline;');
		el.style.display="inline";
	}
	else {
		var el = document.getElementById("div_https_self_signed");
		//el.setAttribute('style', 'display:none;');
		el.style.display="none";
    }
}

function EnableControlsHttpsSigned(enable){
    if(enable){
     	var el = document.getElementById("div_https_signed");
		//el.setAttribute('style', 'display:inline;');
		el.style.display="inline";
	}
	else {
		var el = document.getElementById("div_https_signed");
		//el.setAttribute('style', 'display:none;');
		el.style.display="none";
    }
}

function SetMode(http_mode){

    var form = document.setupHttps;

    // http only
    if (http_mode == 0) {
        EnableControlsHttpsSelfSigned(false);
        EnableControlsHttpsSigned(false);
    }
    else if (http_mode == 1) {
        EnableControlsHttpsSelfSigned(true);
        EnableControlsHttpsSigned(false);
    }
    else if (http_mode == 2) {
        EnableControlsHttpsSelfSigned(false);
        EnableControlsHttpsSigned(true);
    }

    form.http_mode.value = http_mode;

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
    <form id="setupHttps" method="post" name="setupHttps" action="/cgi/settings_setupHttps_submit.cgi">
    <input type="hidden" name="http_mode">
        <xsl:attribute name="value">
			<xsl:value-of select="http_mode" />
		</xsl:attribute>
    </input> 
    <input type="hidden" name="cert_name">
        <xsl:attribute name="value">
			<xsl:value-of select="cert_name" />
		</xsl:attribute>
    </input> 
    <input type="hidden" name="is_cert">
        <xsl:attribute name="value">
			<xsl:value-of select="is_cert" />
		</xsl:attribute>
    </input> 
    <table align="center" width="90%" border="0" cellpadding="3" cellspacing="1">
        <tr><td colspan="3"><font size="3" color="white">&#160;</font></td></tr>
        <tr align="center">
            <td align="right">Service Type</td>
            <td colspan="2" align="left">
                <input type="radio" name="service_type" value="0" onclick="SetMode(0)"/>
                    HTTP Only<br/>
			    <input type="radio" name="service_type" value="1" onclick="SetMode(1)"/>
			        HTTPS Self Signed<br/>
			    <input type="radio" name="service_type" value="2" onclick="SetMode(2)"/>
                    HTTPS Signed
		    </td>
        </tr>            
    </table>
    <table align="center" >
        <tr align="center" bgcolor="#E6E6E6"><td height="1" colspan="1" bgcolor="#EDEDED"></td></tr><!-- horizontal line -->
    </table>

    <div id="div_https_self_signed">
    <table align="center" >
        <tr align="center"><td><font size="3">Self Signed Certificate</font></td></tr>
        <tr align="center" bgcolor="#E6E6E6"><td height="1" colspan="1" bgcolor="#EDEDED"></td></tr><!-- horizontal line -->
        <tr align="center">
            <td>
                <table>
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
    </table>
    </div>
    </form>
<div id="div_https_signed">
    <form method="post" action="/cgi/install_cert.cgi" name="install_cert" id="install_cert" enctype="multipart/form-data">
    <table align="center" >
        <tr align="center"><td><font size="3">Install Certificate</font></td></tr>
        <tr align="center" bgcolor="#E6E6E6"><td height="1" colspan="1" bgcolor="#EDEDED"></td></tr><!-- horizontal line -->
        <tr align="center">
            <td>
        <table>
            <tr>
                <td><font size="3" color="gray">Certificate Name</font></td>
                <td colspan="2">
                    <input type="text" id="txt_cert_name" name="txt_cert_name" style="width:240px;">
                        <xsl:attribute name="value">
                            <xsl:value-of select="setupHttps/cert_name" />
                        </xsl:attribute>
                    </input>
                </td>
            </tr>
            <tr>
                <td><font size="3" color="gray">Certificate File</font></td>
                <td colspan="2">
                    <input type="file" name="cert_file" />
                </td>
            </tr>
            <tr>
                <td><font size="3" color="gray">Key File</font></td>
                <td colspan="2">
                    <input type="file" name="key_file" />
                </td>
            </tr>
            <tr>
                <td><font size="3" color="gray">CA File</font></td>
                <td colspan="2">
                    <input type="file" name="ca_file" />
                </td>
            </tr>
        </table>
        </td>
        </tr>
        </table>
    </form>

</div>
    <table align="center" >
        <tr align="center" bgcolor="#E6E6E6"><td height="1" colspan="1" bgcolor="#EDEDED"></td></tr><!-- horizontal line -->
        <tr align="center">
            <td>
                <input type="button" onclick="OnApply()" value="OK" style="width:100px;height:24px"></input>
                <input type="button" onclick="goHome()" value="Cancel" style="width:100px;height:24px"></input>
            </td>
        </tr>
    </table>
<input type="hidden" id="mac" name="mac">
	<xsl:attribute name="value"><xsl:value-of select="system/mac"></xsl:value-of></xsl:attribute>
</input>  
<input type="hidden" id="lang" name="lang"><xsl:attribute name="value"><xsl:value-of select="$lang"></xsl:value-of></xsl:attribute></input>  
<input type="hidden" id="model_name" name="model_name"><xsl:attribute name="value"><xsl:value-of select="model_name" /></xsl:attribute></input>  
    </td></tr>
</table>
</xsl:template> <!-- settings_contents -->
</xsl:stylesheet>

