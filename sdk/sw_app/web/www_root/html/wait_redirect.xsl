<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html" encoding="utf-8"></xsl:output>
<xsl:variable name="lang"><xsl:value-of select="/*/@lang"/></xsl:variable>
<xsl:variable name="str" select="document(concat('/language/',$lang,'.xml'))/str"></xsl:variable>

<!-- start  -->
<xsl:template match="/">
<html>
    <xsl:attribute name="lang"><xsl:value-of select="$lang" /></xsl:attribute>
<head>
<META HTTP-EQUIV="EXPIRES" CONTENT="0" />
<META HTTP-EQUIV="CACHE-CONTROL" CONTENT="NO-CACHE" />
<META HTTP-EQUIV="PRAGMA" CONTENT="NO-CACHE" />
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=utf-8" />
<META NAME="viewport" CONTENT="user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0, width=device-width" />
<title>LINKFLOW</title>
<script type="text/javascript" src="/script/utils.js" />
<script>
<![CDATA[
var g_timeout;

function redirect_page() {
	var form = document.frm_wait;
	var page = form.redirect_page.value;
	var isTopRefresh = form.top_refresh.value;

	document.body.style.cursor = 'default';
   
    if(isTopRefresh == 1)
	    top.location.href = page;
    else
    	location.href = page;
}

function time_counting() {
    var form = document.frm_wait;
    g_timeout--;
    form.time_remain.value = g_timeout;

    if(g_timeout == 0){
        redirect_page();
    }
    else {
        setTimeout("time_counting()", 1000);
    }
}

function OnLoad() {
    var form = document.frm_wait;
    document.body.style.cursor = 'wait';
    g_timeout = form.timeout.value;
    form.time_remain.value = form.timeout.value;
    setTimeout("time_counting()", 1000);
}
]]>
</script>
</head>
<xsl:apply-templates />
</html>
</xsl:template>

<xsl:template match="waiting_info">
    <body onload="OnLoad()" onselectstart="return false" ondragstart="return false" oncontextmenu="return false">
        <xsl:apply-templates />
    </body>
</xsl:template>

<xsl:template match="waiting">

<table align="center">
    <tr><td>
    <form method="post" name="frm_wait" id="frm_wait">
      <input type="hidden" name="lang">
        <xsl:attribute name="value">
          <xsl:value-of select="$lang" />
        </xsl:attribute>
      </input>  	
	  <input type="hidden" name="redirect_page" >
        <xsl:attribute name="value">
          <xsl:value-of select="redirect_page" />
        </xsl:attribute>
      </input>
	  <input type="hidden" name="timeout" id="timeout">
        <xsl:attribute name="value">
          <xsl:value-of select="timeout" />
        </xsl:attribute>
      </input>

	  <input type="hidden" name="top_refresh" id="top_refresh" >
        <xsl:attribute name="value">
          <xsl:value-of select="top_refresh" />
        </xsl:attribute>
      </input>

    Running setup. Please, wait.
    <input type="text" name="time_remain" style="width:23px" onfocus="this.blur();"/> second(s)
    </form>
    </td></tr>
</table>

</xsl:template> <!-- match=waiting -->
</xsl:stylesheet>


