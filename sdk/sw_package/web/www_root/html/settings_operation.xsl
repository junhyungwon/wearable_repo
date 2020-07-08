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
<title>Operation Settings</title>
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

	if(isNEXX360)
		onClickP2PEnable();

    onClickAutoRec();
}

function OnApply()
{
    if(isSubmitting == true){
	  str = GetXmlString(xmlStr, 'running_submit');
	  alert(str);
	  return;
    }

    var frm = document.frm_operation_config;

    // TODO : make sure values are vaild


    frm.pre_rec.value    = GetChkValue(frm.chk_pre_rec);
    frm.auto_rec.value    = GetChkValue(frm.chk_auto_rec);
    frm.audio_rec.value    = GetChkValue(frm.chk_audio_rec);
    frm.rec_interval.value  = frm.cbo_rec_interval.value;
    frm.rec_overwrite.value = frm.cbo_rec_overwrite.value;
    frm.display_datetime.value = GetChkValue(frm.chk_display_datetime);

	if(isNEXX360){ 
		frm.p2p_enable.value = GetChkValue(frm.chk_p2p_enable);

		if(frm.p2p_enable.value==1){
			if(frm.txt_username.value == ""){
				alert("Please, check NSS ID");
				frm.txt_username.focus();
				return;
			}
			if(frm.txt_password.value == ""){
				alert("Please, check NSS Password");
				frm.txt_password.focus();
				return;
			}

			frm.p2p_username.value = frm.txt_username.value;
			frm.p2p_password.value = frm.txt_password.value;
		}

	}

    isSubmitting = false;

    frm.submit();

}
function goHome() {
    window.location.href='../index.html';
}

function onClickP2PEnable(){
    var frm = document.frm_operation_config;

	if(frm.chk_p2p_enable.checked){
		EnableObj(frm.txt_username);
		EnableObj(frm.txt_password);
	}
	else {
		DisableObj(frm.txt_username);
		DisableObj(frm.txt_password);
	}
}

function onClickAutoRec(){

    /*
    var frm = document.frm_operation_config;

	if(frm.chk_auto_rec.checked){
		EnableObj(frm.cbo_rec_interval);
		EnableObj(frm.cbo_rec_overwrite);
	}
	else {
		DisableObj(frm.cbo_rec_interval);
		DisableObj(frm.cbo_rec_overwrite);
	}
    */
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
    <form method="post" name="frm_operation_config" action="/cgi/settings_operation_submit.cgi">
    <xsl:variable name="pre_rec" select="pre_rec" />
    <xsl:variable name="auto_rec" select="auto_rec" />
    <xsl:variable name="audio_rec" select="audio_rec" />
    <xsl:variable name="rec_interval" select="rec_interval" />
    <xsl:variable name="rec_overwrite" select="rec_overwrite" />
    <xsl:variable name="display_datetime" select="display_datetime" />

	<!-- variable 선언시 xsl:if문으로 둘러 싸려고 했다가 망 -->
	<xsl:variable name="p2p_enable" select="p2p_enable" />
	<xsl:variable name="p2p_username" select="p2p_username" />
	<xsl:variable name="p2p_password" select="p2p_password" />

    <table align="center" >
        <tr align="center"><td><font size="5">Operation</font></td></tr>
        <tr align="center" bgcolor="#E6E6E6"><td height="1" colspan="1" bgcolor="#EDEDED"></td></tr>
        <tr align="center">
            <td>
                <table class="tbl_1">
                    <tr><td colspan="3"><font size="3" color="white">Recording Options</font></td></tr>
                    <tr>
                        <td colspan="3">
                            <input type="checkbox" id="chk_pre_rec" name="chk_pre_rec">
                                <xsl:if test="$pre_rec=1">
                                    <xsl:attribute name="checked">
                                        <xsl:value-of select="$pre_rec" />
                                    </xsl:attribute>
                                </xsl:if>
                            </input>
                            <font size="3" color="gray">Pre-record</font>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="3">
                            <input type="checkbox" id="chk_audio_rec" name="chk_audio_rec">
                                <xsl:if test="$audio_rec=1">
                                    <xsl:attribute name="checked">
                                        <xsl:value-of select="$audio_rec" />
                                    </xsl:attribute>
                                </xsl:if>
                            </input>
                            <font size="3" color="gray">Audio record</font>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="3">
                            <input type="checkbox" id="chk_auto_rec" name="chk_auto_rec" onclick="onClickAutoRec()">
                                <xsl:if test="$auto_rec=1">
                                    <xsl:attribute name="checked">
                                        <xsl:value-of select="$auto_rec" />
                                    </xsl:attribute>
                                </xsl:if>
                            </input>
                            <font size="3" color="gray">Auto record</font>
                        </td>
                    </tr>
                    <tr>
                        <td><font size="3" color="gray">Rec.Interval</font></td>
                        <td>
                            <select id="cbo_rec_interval" name="cbo_rec_interval" style="width:100px">
                                <xsl:for-each select="rec_interval_list">
                                    <option>
                                        <xsl:attribute name="value">
                                            <xsl:value-of select="value"></xsl:value-of>
                                        </xsl:attribute>
                                        <xsl:if test="value=$rec_interval">
                                            <xsl:attribute name="selected">selected</xsl:attribute>
                                        </xsl:if>
                                        <xsl:value-of select="name"></xsl:value-of>
                                    </option>
                                </xsl:for-each>
                            </select>  
                        </td>
                        <td><font size="3" color="gray">minute(s).</font></td>
                    </tr>
                    <tr>
                        <td ><font size="3" color="gray">overwrite</font></td>
                        <td >
                            <select id="cbo_rec_overwrite" name="cbo_rec_overwrite" style="width:100px">
                                <xsl:for-each select="rec_overwrite_list">
                                    <option>
                                        <xsl:attribute name="value">
                                            <xsl:value-of select="value"></xsl:value-of>
                                        </xsl:attribute>
                                        <xsl:if test="value=$rec_overwrite">
                                            <xsl:attribute name="selected">selected</xsl:attribute>
                                        </xsl:if>
                                        <xsl:value-of select="name"></xsl:value-of>
                                    </option>
                                </xsl:for-each>
                            </select>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr align="center">
            <td>
                <table class="tbl_1">
                    <tr><td><font size="3" color="white">MISC</font></td></tr>
                    <tr>
                        <td><input type="checkbox" id="chk_display_datetime" name="chk_display_datetime">
                            <xsl:if test="$display_datetime=1">
                                <xsl:attribute name="checked">
                                    <xsl:value-of select="$display_datetime" />
                                </xsl:attribute>
                            </xsl:if>
                            </input>
                            <font size="3" color="gray">Show date/time on display</font>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>

			<xsl:if test="$MODEL_NAME='NEXX360'">
				<tr align="center">
					<td>
						<table class="tbl_1">
							<tr>
								<td colspan="2"><font size="3" color="white">NEXX Streaming Server(NSS)</font></td>
							</tr>
							<tr>
								<td colspan="2">
									<input type="checkbox" id="chk_p2p_enable" name="chk_p2p_enable" onclick="onClickP2PEnable()">
										<xsl:if test="$p2p_enable=1">
											<xsl:attribute name="checked">
												<xsl:value-of select="$p2p_enable" />
											</xsl:attribute>
										</xsl:if>
									</input>
									<font size="3" color="gray">Enable/Disable</font></td>
							</tr>
							<tr>
								<td ><font size="3" color="gray">Username</font></td>
								<td >
									<input type="text" id="txt_username" name="txt_username" maxlength="32" style="width:160px">
										<xsl:attribute name="value">
											<xsl:value-of select="$p2p_username" />
										</xsl:attribute>
									</input>
								</td>
							</tr>
							<tr>
								<td ><font size="3" color="gray">Password</font></td>
								<td >
									<input type="password" id="txt_password" name="txt_password" maxlength="32" style="width:160px">
										<xsl:attribute name="value">
											<xsl:value-of select="$p2p_password" />
										</xsl:attribute>
									</input>
								</td>
							</tr>
							<tr><td>&#160;</td></tr>
						</table>
					</td>
				</tr>
			</xsl:if>

        <tr align="center" bgcolor="#E6E6E6"><td height="1" colspan="1" bgcolor="#EDEDED"></td></tr><!-- horizontal line -->
        <tr align="center">
            <td>
                <input type="button" onclick="OnApply()" value="OK" style="width:100px;height:24px"></input>
                <input type="button" onclick="goHome()" value="Cancel" style="width:100px;height:24px"></input>
            </td>
        </tr>
    </table>
    <input type="hidden" id="pre_rec" name="pre_rec"><xsl:attribute name="value"><xsl:value-of select="$pre_rec"></xsl:value-of></xsl:attribute></input>
    <input type="hidden" id="auto_rec" name="auto_rec"><xsl:attribute name="value"><xsl:value-of select="$auto_rec"></xsl:value-of></xsl:attribute></input>
    <input type="hidden" id="audio_rec" name="audio_rec"><xsl:attribute name="value"><xsl:value-of select="$audio_rec"></xsl:value-of></xsl:attribute></input>
    <input type="hidden" id="rec_interval" name="rec_interval"><xsl:attribute name="value"><xsl:value-of select="$rec_interval"></xsl:value-of></xsl:attribute></input>
    <input type="hidden" id="rec_overwrite" name="rec_overwrite"><xsl:attribute name="value"><xsl:value-of select="$rec_overwrite"></xsl:value-of></xsl:attribute></input>
    <input type="hidden" id="display_datetime" name="display_datetime"><xsl:attribute name="value"><xsl:value-of select="$display_datetime"></xsl:value-of></xsl:attribute></input>

		<xsl:if test="$MODEL_NAME='NEXX360'">
			<input type="hidden" id="p2p_enable" name="p2p_enable"><xsl:attribute name="value"><xsl:value-of select="$p2p_enable"></xsl:value-of></xsl:attribute></input>
			<input type="hidden" id="p2p_username" name="p2p_username"><xsl:attribute name="value"><xsl:value-of select="$p2p_username"></xsl:value-of></xsl:attribute></input>
			<input type="hidden" id="p2p_password" name="p2p_password"><xsl:attribute name="value"><xsl:value-of select="$p2p_password"></xsl:value-of></xsl:attribute></input>
		</xsl:if>

</form>
<input type="hidden" id="lang" name="lang"><xsl:attribute name="value"><xsl:value-of select="$lang"></xsl:value-of></xsl:attribute></input>  
<input type="hidden" id="model_name" name="model_name"><xsl:attribute name="value"><xsl:value-of select="model_name" /></xsl:attribute></input>  
    </td></tr>
</table>
</xsl:template> <!-- settings_contents -->
</xsl:stylesheet>

