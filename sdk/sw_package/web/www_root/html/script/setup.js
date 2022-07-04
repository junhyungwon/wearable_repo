
function getCookie( name ) {
  var start = document.cookie.indexOf( name + "=" );
  var len = start + name.length + 1;
  if ( ( !start ) && ( name != document.cookie.substring( 0, name.length ) ) ) {
    return "";
  }
  if ( start == -1 ) return "";
  var end = document.cookie.indexOf( ';', len );
  if ( end == -1 ) end = document.cookie.length;
  return unescape( document.cookie.substring( len, end ) );
}

function setCookie( name, value, expires, path, domain, secure ) {
  var today = new Date();
  today.setTime( today.getTime() );
  if ( expires ) {
      expires = expires * 1000 * 60 * 60 * 24;
  }
  var expires_date = new Date( today.getTime() + (expires) );
  document.cookie = name+'='+escape( value ) +
    ( ( expires ) ? ';expires='+expires_date.toGMTString() : '' ) +
    ( ( path ) ? ';path=' + path : '' ) +
    ( ( domain ) ? ';domain=' + domain : '' ) +
    ( ( secure ) ? ';secure' : '' );
}

function deleteCookie( name, path, domain ) {
  if ( getCookie( name ) ) document.cookie = name + '=' +
   ( ( path ) ? ';path=' + path : '') +
   ( ( domain ) ? ';domain=' + domain : '' ) +
   ';expires=Thu, 01-Jan-1970 00:00:01 GMT';
}

function checkCookie(name)
{
	username=getCookie(name);
	if (username!=null && username!="")
	{
		alert('Welcome '+username);
	}
	else 
	{
		alert('no cookie!');
		//username=prompt('Please enter your name:',"");
		//if (username!=null && username!="")
		//{
		//	setCookie('USER',username,365);
		//}
	}
}


function check_web_security()
{
	return;

	var isPossible;

	isPossible = getCookie( 'DVR_LOGIN' );

	if(isPossible != 'ON'){
		window.location = "index.html";
	}
}

function check_mobile_security()
{
	return;

	var isPossible;

	isPossible = getCookie( 'MLOGIN' );

	if(isPossible != 'ON'){
		window.location = "index.html";
	}
}


function load_mobile()
{
  setCookie( 'MLOGIN', 'ON', '','','','','');
}

function unload_mobile()
{
  deleteCookie( 'MLOGIN', '', '' );
  deleteCookie( 'USER', '', '' );
  deleteCookie( 'PASSWORD', '', '' );
  window.location = "index.html";
}


// common xml
function EnableObjNames(name)
{
	var i;
	var obj=document.getElementsByName(name);

	for(i=0; obj[i]; i++)
		EnableObj(obj[i]);
}


function EnableObj(obj)
{
	if(obj) {
		obj.disabled = false;
		obj.style.backgroundColor = "";
	}
}

function DisableObj(obj)
{
	if(obj)
	{
		obj.disabled = true;
		if(obj.type == 'text' || obj.type == 'password') 
			obj.style.backgroundColor = "#EEEEEE";
	}
}

function DisableObjNames(name)
{
	var i;
	var obj=document.getElementsByName(name);

	for(i=0; obj[i]; i++)
        DisableObj(obj[i]);
}


function DisableRadio( radioobj )
{
	for( var i = 0 ; i <  radioobj.length; i++ )
		DisableObj(radioobj[i]);
	return;
}


function EnableRadio( radioobj )
{
	for( var i = 0 ; i <  radioobj.length; i++ )
		EnableObj(radioobj[i]);
	return;
}

function DisableCheckBox( checkobj )
{
	if(checkobj.length == null)
	{
		DisableObj(checkobj); 
		return;
	}
	for( var i = 0 ; i <  checkobj.length; i++ )
		DisableObj(checkobj[i]);
	return;
}

function EnableCheckBox(checkobj)
{
	if(checkobj.length == null)
	{
		EnableObj(checkobj); 
		return;
	}
	for( var i = 0 ; i <  checkobj.length; i++ )
		EnableObj(checkobj[i]);
	return;
}

function ClearValue(obj)
{
	if(obj && obj.type == 'text' || obj.type == 'password') {
		obj.value = "";
	}else if(obj.type == 'radio' || obj.type == 'checkbox') {
		obj.checked = false;
	}else if(obj.type == 'selected') {
		obj[0].selected = true;
	}
}

function GetCheckValue(checkobj)
{
	if(checkobj.checked) {
		checkobj.value = 1;
		return true;
	}
	else {
		checkobj.value = 0;
		return false;
	}
}

function GetValue(obj)
{
	if(obj[0] && obj[0].type == 'radio')
		return GetRadioValue(obj);
	else if(obj[0].type == 'select')
		return GetSelectValue(obj);
	return obj.value;
}

function GetRadioValue(radioobj)
{
	for(var i=0; i<radioobj.length; i++)
	{
		if(radioobj[i].checked){
		//	return radioobj[i].value;
			return i;
		}
	}
	return "";
}

function GetSelectValue(selectobj)
{
	for(var i=0; i<selectobj.length; i++)
	{
		if(selectobj[i].selected)
			return selectobj[i].value;
	}
//	alert("select obj not found");
	return "";
}

function getCheckedValue(radioObj) {
	if(!radioObj)
		return "";
	var radioLength = radioObj.length;
	if(radioLength == undefined)
		if(radioObj.checked)
			return radioObj.value;
		else
			return "";
	for(var i = 0; i < radioLength; i++) {
		if(radioObj[i].checked) {
			return radioObj[i].value;
		}
	}
	return "";
}

function setCheckedValue(radioObj, newValue) {
	if(!radioObj)
		return;
	var radioLength = radioObj.length;
	if(radioLength == undefined) {
		radioObj.checked = (radioObj.value == newValue.toString());
		return;
	}
	for(var i = 0; i < radioLength; i++) {
		radioObj[i].checked = false;
		if(radioObj[i].value == newValue.toString()) {
			radioObj[i].checked = true;
		}
	}
}


function SetCheckValue(checkobj, value)
{
	if(value > 0 || value == 'yes' || value == 'checked')
		checkobj.checked = true;
	else
        checkobj.checked = false;
}

function SetRadioValue( radioobj, value )
{
	for(var i=0 ; i<radioobj.length; i++)
	{
//		if(radioobj[i].value == value)
//			radioobj[i].checked = true;
//		else
//			radioobj[i].checked = false;
		if(i == value)
			radioobj[i].checked = true;
		else
			radioobj[i].checked = false;

	}
	return;
}

function SetSelectValue( selectobj, value )
{
	for(var i=0 ; i<selectobj.length; i++)
	{
		if(selectobj[i].value == value)
			selectobj[i].selected = true;
		else
			selectobj[i].selected = false;
	}
	return;
}


function HideIt(id)
{
   	document.getElementById(id).style.display = "none";
}

function ShowIt(id)
{
	if (navigator.appName.indexOf("Microsoft") != -1)
		document.getElementById(id).style.display = "block";
	else
		document.getElementById(id).style.display = "table-row";
				
}


function DisableIP(name)
{
	for( i = 1; i <= 4; i++)
	{
		obj=document.getElementsByName(name+i);
		if(obj[0])
		{
			obj[0].style.backgroundColor = "#EEEEEE";
			obj[0].disabled = true;
		}
	}
}

function EnableIP(name)
{
	for( i = 1; i <= 4; i++)
	{
		obj=document.getElementsByName(name+i);
		if(obj[0])
		{
			obj[0].style.backgroundColor = "";
			obj[0].disabled = false;
		}
	}
}

function LoadXMLDoc(dname) 
{
	var xmlDoc;
	try {//Internet Explorer
		xmlDoc=new ActiveXObject("Microsoft.XMLDOM");
    }
    catch(e) {
        try {//Firefox, Mozilla, Opera, etc.
//			xmlDoc=document.implementation.createDocument("","",null);
			var xmlhttp = new window.XMLHttpRequest();
			xmlhttp.open("GET",dname,false);
			xmlhttp.send(null);
			xmlDoc = xmlhttp.responseXML.documentElement;
			return(xmlDoc);
        }
        catch(e) {
			alert(e.message);
		}
    }
    try {
		xmlDoc.async=false;
		xmlDoc.load(dname);
		return(xmlDoc);
    }
    catch(e) {
		alert(e.message);
	}
    return(null);
}

function GetXmlString(xml, tag)
{
    var str = xml.getElementsByTagName(tag);
    
    try{
		return (str.item(0).childNodes[0].nodeValue);
	}catch (e){
		alert(e.message);
	}
    return(null);
}


// Why xsl embedded script error.(for. while,...)
function displayXML()
{
    var form = document.user;
    var sel_index;
    var string = "";

    sel_index = GetSelectValue(form.user_list);
    var nlist = xmlDoc.getElementsByTagName('user_list');
    for(var i=0; i < nlist.length; i++)
    {
		string = nlist.item(i);
		//alert(string.xml);
    }
}

function Window_modal(url, name, width, height) 
{
    var result = null;
    /* 
    if (window.showModalDialog) { 
    window.showModalDialog(url, window, 'dialogWidth:'+width+'px;dialogHeight:'+height+'px'); 
    } */
    if (window.showModelessDialog) {
        var property ='dialogWidth:'+(parseInt(width)+10)+'px;dialogHeight:'+(parseInt(height)+30)+'px;'+
			'scroll:no;resizable:no;help:no;center:yes;status:no;edge:sunken;unadorned:yes;';
        result = window.showModelessDialog(url, null, property);
	} else {
        var left = (screen.width-width)/2;
        var top = (screen.height-height)/3;
        var property ='left='+left+',top='+top+',height='+height+',width='+width+',toolbar=no,directories=no,status=no,linemenubar=no,scrollbars=no,resizable=no,modal=yes,dependent=yes';
        result = window.open(url, name, property);
	}
    return result;
}

//function setCookie( cookieName, cookieValue, expireDate )
//{
//    var today = new Date();
//    today.setDate( today.getDate() + parseInt( expireDate ) );
//    document.cookie = cookieName + "=" + escape( cookieValue ) + "; path=/; expires=" + today.toGMTString() + ";"
//}

function numCheck(obj, maxValue){
	var ptn = new RegExp('[0-9]');
	var inValue = obj.value;
	if(!ptn.test(inValue) && inValue != ""){
		alert(GetXmlString(xmlStr, 'check_number'));
		obj.value="";
		obj.focus();
		return;
	}
	else if(inValue > maxValue){
		alert(maxValue + ' ' + GetXmlString(xmlStr, 'less_error'));
		obj.value="";
		obj.focus();
		return;
	}
}

function NumRangeCheck(obj, minValue, maxValue){
	var ptn = new RegExp('[0-9]');
	var inValue = obj.value;
	if(!ptn.test(inValue) && inValue != ""){
		alert(GetXmlString(xmlStr, 'check_number'));
		obj.value="";
		obj.focus();
		return;
	}
	else if(inValue > maxValue){
		alert(maxValue + ' ' + GetXmlString(xmlStr, 'less_error'));
		obj.value=maxValue;
		obj.focus();
		return;
	}
	else if(inValue < minValue){
		alert(minValue + ' ' + GetXmlString(xmlStr, 'over_error'));
		obj.value=minValue;
		obj.focus();
		return;
	}
}


