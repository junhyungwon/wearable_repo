function DebugAlert(str) 
{
    alert("*** DEBUG ***\r\n"+str);
}

function LoadXmlDoc(dname) 
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


function GetXmlString(xml, tag) { return GetStringFromXml(xml, tag); }
function GetStringFromXml(xml, tag)
{
    var str = xml.getElementsByTagName(tag);
    
    try{
		return (str.item(0).childNodes[0].nodeValue);
	}catch (e){
		alert(e.message);
	}
    return(null);
}


function CheckNumber(obj, maxValue){
	var ptn = new RegExp('[0-9]');
	var inValue = obj.value;
	if(!ptn.test(inValue) && inValue != ""){
		alert(GetStringFromXml(xmlStr, 'check_number'));
		obj.value="";
		obj.focus();
		return;
	}
	else if(inValue > maxValue){
		alert(maxValue + ' ' + GetStringFromXml(xmlStr, 'less_error'));
		obj.value="";
		obj.focus();
		return;
	}
}

function CheckNumberRange(obj, minValue, maxValue){
	var ptn = new RegExp('[0-9]');
	var inValue = obj.value;
	if(!ptn.test(inValue) && inValue != ""){
		alert(GetStringFromXml(xmlStr, 'check_number'));
		obj.value="";
		obj.focus();
		return;
	}
	else if(inValue > maxValue){
		alert(maxValue + ' ' + GetStringFromXml(xmlStr, 'less_error'));
		obj.value="";
		obj.focus();
		return;
	}
    else if(inValue < minValue){
		alert(minValue + ' ' + GetStringFromXml(xmlStr, 'over_error'));
		obj.value=minValue;
		obj.focus();
		return;
	}
}

function isEng(str) {
  for(var i=0;i<str.length;i++){          
    achar = str.charCodeAt(i);              
    if( achar > 255 ){ 
       return false;
    } 
  }
  return true;
}

function Frameset(page) {
    framecode = "<frameset rows='1*'>"
        + "<frame name=main src='" + page + "'>"
        + "</frameset>";

    document.write(framecode);
    document.close();
}

function GetRdoValue(obj)
{
	for(var i=0; i<obj.length; i++)
	{
		if(obj[i].checked){
			return i;
		}
	}
	return "";
}

function GetChkValue(obj)
{
	if(obj.checked) {
		obj.value = 1;
		return 1;
	}
	else {
		obj.value = 0;
		return 0;
	}
}

function EnableWnd(obj, en)
{
	if(obj){
		if(en==1){
			obj.disabled = false;
			obj.style.backgroundColor = "";
		}
		else {
			obj.disabled = true;
			obj.style.textDecorationColor = "#DDDDDD";
			if(obj.type == 'text' || obj.type == 'password') 
				obj.style.backgroundColor = "#DDDDDD";
		}
	}
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
	if(obj) {
		obj.disabled = true;
        obj.style.textDecorationColor = "#DDDDDD";
		if(obj.type == 'text' || obj.type == 'password') 
			obj.style.backgroundColor = "#DDDDDD";
	}
}

function inet_aton(ip){
    // split into octets
    var a = ip.split('.');
    var buffer = new ArrayBuffer(4);
    var dv = new DataView(buffer);
    for(var i = 0; i < 4; i++){
        dv.setUint8(i, a[i]);
    }
    return(dv.getUint32(0));
}

function inet_ntoa(num){
    var nbuffer = new ArrayBuffer(4);
    var ndv = new DataView(nbuffer);
    ndv.setUint32(0, num);

    var a = new Array();
    for(var i = 0; i < 4; i++){
        a[i] = ndv.getUint8(i);
    }
    return a.join('.');
}
