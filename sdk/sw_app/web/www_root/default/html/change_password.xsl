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
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=utf-8" />
<META NAME="viewport" CONTENT="user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0, width=device-width" />
<title>LINKFLOW</title>
<script type="text/javascript" src="/script/utils.js" />
<script type="text/javascript">
<![CDATA[
var xmlStr;
var isSubmitting = false;

function OnLoad() {
	document.title += ' - ' + document.getElementById('model_name').value;

    // set default
    var idx = 0;   // 0:basic(default), 1:digest, TODO: Set this value stored in Device
  	var rdo = document.frm_change_password.authtype;
    rdo[idx].checked = true; 


    var lang = document.getElementById('lang').value;
    xmlStr = LoadXmlDoc('/language/'+lang+'.xml');

  	var form = document.frm_change_password;
    form.password1.focus();
}

function CheckPass(obj){
  	var form = document.frm_change_password;
	var len = 0;
	var str = obj.value;

	for(var idx=0 ; idx < str.length ; idx++){
		var ch = escape(str.charAt(idx));
		if(ch.length == 1){
			len++;
		}
		else if(ch.indexOf("%u") != -1){
			len += 2;
		}
		else if(ch.indexOf("%") != -1){
			len += ch.length/3;
		}
	}
    //DebugAlert(str+":"+len);

    // Below is not happend, bcz obj.maxlength is 32.
	if(len > 32){
		var err;
	    err = GetXmlString(xmlStr, 'max_text');
		alert(err); 
	  	obj.value = "";
	}
}

function OnApply()
{
    if(isSubmitting == true){
	  str = GetXmlString(xmlStr, 'running_submit');
	  alert(str);
	  return;
    }

  	var form = document.frm_change_password;

    if(form.password1.value == ""){
        alert(GetXmlString(xmlStr, 'check_password'));
        form.password1.focus();
        return false;
    }

    if(form.password1.value == "admin"){
        alert(GetXmlString(xmlStr, 'use_different_password'));
        form.password1.focus();
        return false;
    }

    if(form.password2.value == ""){
        alert(GetXmlString(xmlStr, 'check_password'));
        form.password2.focus();
        return false;
    }

    if(form.password1.value != form.password2.value){
        alert(GetXmlString(xmlStr, 'not_match_password'));
        return false;
    }

	form.submit();

    isSubmitting = true;
}
]]>
</script>
</head>
<xsl:apply-templates />
</html>
</xsl:template>

<xsl:template match="new_password_body">
<body onload="OnLoad();" onselectstart="return false" ondragstart="return false" oncontextmenu="return false">
    <xsl:apply-templates />
</body>
</xsl:template>

<xsl:template match="new_password_table">
    <table width="100%" height="100%" align="center" border="0" cellpadding="0" cellspacing="0"><!-- 1 -->
        <tr>
            <td>
                <table width="90%" align="center" border="0" cellpadding="0" cellspacing="0">
                    <tr align="center">
                        <td>
                            <img src="../image/eye-240.png" alt="information" height="42" width="42" />
                        </td>
                    </tr>
                    <tr align="center">
                        <td>Set the initial administrator password</td>
                    </tr>
                </table><br/>
                <form method="post" name="frm_change_password" id="frm_change_password" action="/cgi/change_password_submit.cgi">
                    <table width="90%" align="center" border="0" cellpadding="3" cellspacing="1">
                        <tr>
                            <td align="right" width="50%">ID</td>
                            <td width="50%">admin</td>
                        </tr>
                        <!-- tr>
                            <td align="right" width="30%">Default <xsl:value-of select="$strings/password" /></td>
                            <td width="50%">
                                <input name="password0" id="password0" type="password" maxlength="32" style="width:200px" onKeyUp="CheckPass(this);" />
                            </td>
                        </tr -->
                        <tr>
                            <td align="right" width="30%">New <xsl:value-of select="$strings/password" /></td>
                            <td width="50%">
                                <input name="password1" id="password1" type="password" maxlength="32" style="width:200px" onKeyUp="CheckPass(this);" />
                            </td>
                        </tr>
                        <tr>
                            <td align="right" width="50%">Confirm <xsl:value-of select="$strings/password" /></td>
                            <td width="50%">
                                <input name="password2" id="password2" type="password" maxlength="32" style="width:200px" onKeyUp="CheckPass(this);" />
                            </td>
                        </tr>
                        <tr>
                            <td align="right" width="50%"><xsl:value-of select="$strings/authentication" /></td>
                            <td width="50%">
								<input type="radio" name="authtype" value="0">basic&#160;&#160;</input>
								<input type="radio" name="authtype" value="1" disabled="true">digest(<xsl:value-of select="$strings/messages/itwillbesupportedsoon" />)</input>
                            </td>
                        </tr>
                    </table> 

                    <br/>
    <!-- Buttons -->
                    <table align="center" border="0" cellpadding="0" cellspacing="10">
                      <tr>                       
                        <td>
                          <input type="button" onclick="OnApply()" value="Apply" style="width:200px;height:40px" />
                        </td> 
                      </tr>
                    </table>
                </form>
				<input type="hidden" id="lang" name="lang"><xsl:attribute name="value"><xsl:value-of select="$lang" /></xsl:attribute></input>  
				<input type="hidden" id="model_name" name="model_name"><xsl:attribute name="value"><xsl:value-of select="model_name" /></xsl:attribute></input>  
            </td>
        </tr>
    </table><!-- 1 -->
</xsl:template> <!-- new_password_table -->
</xsl:stylesheet>


