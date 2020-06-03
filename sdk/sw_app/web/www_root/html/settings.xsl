<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE xsl:stylesheet PUBLIC "-//WAPFORUM//DTD XHTML Mobile 1.2//EN" "http://www.wapforum.org/DTD/xhtml-mobile12.dtd">

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html" encoding="utf-8"></xsl:output>
<xsl:variable name="language" select="/*/@lang"></xsl:variable>
<xsl:variable name="lang"><xsl:value-of select="$language" /></xsl:variable>
<xsl:variable name="strings" select="document(concat('/language/',$lang,'.xml'))/strings"></xsl:variable>
<xsl:template match="/">
<!-- start  -->
<html xmlns="http://www.w3.org/1999/xhtml">
    <xsl:attribute name="lang"><xsl:value-of select="$lang" /></xsl:attribute>
    <xsl:attribute name="xml:lang"><xsl:value-of select="$lang" /></xsl:attribute>
<head>
<META HTTP-EQUIV="EXPIRES" CONTENT="0" />
<META HTTP-EQUIV="CACHE-CONTROL" CONTENT="NO-CACHE" />
<META HTTP-EQUIV="PRAGMA" CONTENT="NO-CACHE" />
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=utf-8" />
<META NAME="viewport" CONTENT="user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0, width=device-width" />
<title>Settings</title>
<script type="text/javascript" src="/script/utils.js" />
<script>
<![CDATA[
var xmlStr;
var isSubmitting;
var g_timer_id = 0;

function dummy() {
}

function OnLoad() {
	document.title += ' - ' + document.getElementById('model_name').value;
    var lang = document.getElementById('lang').value;

    // set default
    isSubmitting = false;

    xmlStr = LoadXmlDoc('/language/'+lang+'.xml');
}


function OnApply()
{
    if(isSubmitting == true){
	  str = GetXmlString(xmlStr, 'running_submit');
	  alert(str);
	  return;
    }


    isSubmitting = true;
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
    <table align="center">
        <tr align="center">
            <td height="10" colspan="2"></td>
        </tr>
        <tr align="center">
            <td width="100">Settings</td>
        </tr>
        <tr>
            <td>&nbsp;</td>
        </tr>
        <tr align="center" bgcolor="#E6E6E6">
            <td height="1" colspan="2" bgcolor="#EDEDED"></td>
        </tr>
        <tr align="center">
            <td width="100"><a href="../index.html">Home</a></td>
        </tr>
    <table>
<input type="hidden" id="lang" name="lang"><xsl:attribute name="value"><xsl:value-of select="$lang"></xsl:value-of></xsl:attribute></input>
<input type="hidden" id="model_name" name="model_name"><xsl:attribute name="value"><xsl:value-of select="model_name" /></xsl:attribute></input>  
    </td></tr>
</table>
</xsl:template> <!-- settings_contents -->
</xsl:stylesheet>
