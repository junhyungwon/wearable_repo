(window.webpackJsonp=window.webpackJsonp||[]).push([[8],{302:function(e,t,r){"use strict";r.d(t,"a",(function(){return n}));r(101);var o=r(24);function n(e){if("object"===Object(o.a)(e.data)){if(void 0!==e.data["return value"]){if(0==e.data["return value"]||1==e.data["return value"])return e.data;throw e.data["return value"]}return e.data}var t=e.data.match(/\-\d+/m);if(!t)throw"unknown";if(t.length<1)throw"unknown";throw t[0]}},303:function(e,t,r){"use strict";r(1);t.a={methods:{alert:function(e){var t=this;return new Promise((function(r,o){t.$modal.show("dialog",{text:e,buttons:[{title:t.$polyglot.t("dialog.ok"),handler:function(){r(!0)}}]})}))},confirm:function(e){var t=this;return new Promise((function(r,o){t.$modal.show("dialog",{text:e,buttons:[{title:t.$polyglot.t("dialog.cancel"),handler:function(){r(!1)}},{title:t.$polyglot.t("dialog.ok"),default:!0,handler:function(){r(!0)}}]})}))}}}},304:function(e,t,r){"use strict";r.d(t,"a",(function(){return n}));var o=r(24);function n(e,t){for(var r in e)void 0!==t[r]&&(Array.isArray(t[r])?e[r]=t[r].map((function(e){return e})):("object"===Object(o.a)(t[r])&&t[r]&&n(e[r],t[r]),e[r]=isNaN(t[r])?t[r]:parseInt(t[r],10)))}},308:function(e,t,r){"use strict";r.d(t,"a",(function(){return o}));r(27);function o(e){var t=e.split(".");return 4===t.length&&t.reduce((function(e,t){if(0===t.length||t.length>3)return!1;var r=parseInt(t,10);return!isNaN(r)&&(!(r<0||r>255)&&e)}),!0)}},315:function(e,t,r){var content=r(327);"string"==typeof content&&(content=[[e.i,content,""]]),content.locals&&(e.exports=content.locals);(0,r(78).default)("7e988942",content,!0,{sourceMap:!1})},316:function(e,t,r){var content=r(329);"string"==typeof content&&(content=[[e.i,content,""]]),content.locals&&(e.exports=content.locals);(0,r(78).default)("1faaaf72",content,!0,{sourceMap:!1})},326:function(e,t,r){"use strict";r(315)},327:function(e,t,r){(t=r(77)(!1)).push([e.i,"*{margin:0;padding:0;box-sizing:border-box}body,html{width:100%;height:100%}.mymodal{background-color:rgba(0,0,0,.8);top:0;right:0;bottom:0;left:0;position:fixed;overflow:auto;margin:0}.mymodal__dialog{position:absolute;background:#fff}.mymodal__header{font-size:28px;font-weight:700;line-height:1.29;padding:16px 16px 0 25px;position:relative}.mymodal__body{padding-top:20px;min-height:150px;max-height:412px}.mymodal .upload_percentage{margin-top:20px}.mymodal .loader{margin-top:20px;border-bottom:16px solid #f3f3f3;border-top:16px solid #f3f3f3;border-radius:50%;border-color:#deb887 #f3f3f3;border-style:solid;border-width:16px;width:120px;height:120px;-webkit-animation:spin 2s linear infinite;animation:spin 2s linear infinite}@-webkit-keyframes spin{0%{-webkit-transform:rotate(0deg)}to{-webkit-transform:rotate(1turn)}}@keyframes spin{0%{transform:rotate(0deg)}to{transform:rotate(1turn)}}",""]),e.exports=t},328:function(e,t,r){"use strict";r(316)},329:function(e,t,r){(t=r(77)(!1)).push([e.i,".system-page ol.controls li:nth-child(2){flex-wrap:wrap}.system-page ol.controls li:nth-child(2) button.mac-to-id-btn{margin-top:8px;margin-left:auto}.system-page button.text-btn{margin-left:auto;text-transform:uppercase}.system-page .li-fwupdate{padding-top:7px;padding-bottom:10px}.system-page .li-fwupdate button.flat-btn{margin-left:auto}.system-page .div-update-btn{flex-wrap:wrap;margin-top:8px;margin-left:auto}.system-page .filebox input[type=file]{position:absolute;width:0;height:0;padding:0;overflow:hidden;border:0}.system-page .filebox label{margin:0;display:inline-block;font-size:14px;font-weight:500;padding-top:7px;padding-bottom:6px;text-align:center;color:#1c1c1e;text-transform:uppercase;vertical-align:top;background-color:#7f7f81;cursor:pointer;width:38px}.system-page .filebox label:hover{background-color:#6b6b6d}.system-page .filebox .upload-name{font-size:14px;font-weight:400;padding:7px 0 6px 2px;color:#ababb4;background-color:transparent;background-clip:padding-box;border:1px solid #37373b;width:270px;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none;overflow:visible}",""]),e.exports=t},335:function(e,t,r){"use strict";r.r(t);r(5),r(2),r(3),r(4),r(63),r(79),r(27),r(16),r(50);var o=r(10),n=(r(1),r(0)),l=r(302),d=r(304),m=(r(308),r(49)),dialog=r(303),c=(r(326),r(33)),f=Object(c.a)({},(function(){var e=this,t=e.$createElement,r=e._self._c||t;return r("div",{staticClass:"mymodal mymodal__dim"},[r("center",[r("div",{attrs:{calss:"mymodal__dialog"}},[r("header",{staticClass:"mymodal__header"},[r("span",[e._v(e._s(e.title))])]),e._v(" "),r("div",{staticClass:"mymodal__body"},[e._t("mymodal-text",[e._v("(모달 내용)")]),e._v(" "),r("div",{staticClass:"upload_percentage"},[e._t("mymodal-percent",[e._v("(percent)")])],2),e._v(" "),r("div",[e._t("mymodal-progress",[e._v("(progressText)")])],2),e._v(" "),r("div",{staticClass:"loader"})],2)])])],1)}),[],!1,null,null,null).exports;function y(object,e){var t=Object.keys(object);if(Object.getOwnPropertySymbols){var r=Object.getOwnPropertySymbols(object);e&&(r=r.filter((function(e){return Object.getOwnPropertyDescriptor(object,e).enumerable}))),t.push.apply(t,r)}return t}var v=/^([0-9]|[a-z]|[A-Z]|_|\-|!|@|#|\$|%|\^){1,31}$/,h={components:{MyModal:f},mixins:[dialog.a],data:function(){return{showModal:!1,modalText:"",modalText2:"",progressText:"",fw:{fwfile:null},form:{model:null,system:{model:null,fwver:null,devid:null,uid:null,mac:null}},disableSubmit:!0,FWUpdateSubmit:!1,uploadPercentage:0,uploadedCurrSize:0}},watch:{"form.model":function(e){"NEXXONE"===this.form.model?(window.document.title="NEXX ONE",this.$refs.model_name.value="NEXX ONE"):(window.document.title=e,this.$refs.model_name.value=e)},uploadPercentage:function(e){}},methods:function(e){for(var i=1;i<arguments.length;i++){var source=null!=arguments[i]?arguments[i]:{};i%2?y(Object(source),!0).forEach((function(t){Object(n.a)(e,t,source[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(source)):y(Object(source)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(source,t))}))}return e}({wait:function(){return new Promise((function(e){return setTimeout(e,1e3)}))},fetchData:function(){var e=this;return Object(o.a)(regeneratorRuntime.mark((function t(){var r,o,data,n;return regeneratorRuntime.wrap((function(t){for(;;)switch(t.prev=t.next){case 0:return r=e.$apiRouter.resolve({name:"cgi/config",query:{action:"search",param:"system_config"}}).href,t.prev=1,t.next=4,e.$axios.get(r);case 4:return o=t.sent,data=Object(l.a)(o),Object(d.a)(e.form,data),e.form.model_name=e.form.model,n=(n=e.form.system.uid).substr(n.indexOf("LFS-LSCS-A1-")+9,n.length-n.indexOf("LFS-LSCS-A1-")+9),e.form.system.uid=n,t.next=13,e.wait();case 13:e.disableSubmit=!1,t.next=28;break;case 16:if(t.prev=16,t.t0=t.catch(1),!e.$polyglot.has("error_messages."+t.t0)){t.next=24;break}return t.next=21,e.alert(e.$polyglot.t("error_messages."+t.t0));case 21:e.$modal.hide("dialog"),t.next=27;break;case 24:return t.next=26,e.alert(t.t0);case 26:e.$modal.hide("dialog");case 27:console.error(t.t0);case 28:case"end":return t.stop()}}),t,null,[[1,16]])})))()},storeData:function(){var e=this;return Object(o.a)(regeneratorRuntime.mark((function t(){var r,form,o;return regeneratorRuntime.wrap((function(t){for(;;)switch(t.prev=t.next){case 0:return t.next=2,e.validation();case 2:if(t.sent){t.next=4;break}return t.abrupt("return");case 4:return e.disableSubmit=!0,r=e.$apiRouter.resolve({name:"cgi/system"}).href,(form=new URLSearchParams).append("txt_devid",e.form.system.devid),"NEXX360"!==e.form.model&&"NEXX360B"!==e.form.model&&"NEXX360C"!==e.form.model&&"NEXX360W_CCTV"!==e.form.model&&"NEXX360W"!==e.form.model&&"NEXXB"!==e.form.model&&"NEXX360M"!==e.form.model&&"NEXXONE"!==e.form.model&&"NEXX360W_MUX"!==e.form.model&&"NEXXB_ONE"!==e.form.model||form.append("txt_uid",e.form.system.uid),t.prev=9,t.next=12,e.$axios.post(r,form);case 12:return o=t.sent,Object(l.a)(o),t.next=16,e.alert(e.$polyglot.t("messages.saved"));case 16:e.$modal.hide("dialog"),e.$router.push("/"),t.next=32;break;case 20:if(t.prev=20,t.t0=t.catch(9),!e.$polyglot.has("error_messages."+t.t0)){t.next=28;break}return t.next=25,e.alert(e.$polyglot.t("error_messages."+t.t0));case 25:e.$modal.hide("dialog"),t.next=31;break;case 28:return t.next=30,e.alert(t.t0);case 30:e.$modal.hide("dialog");case 31:console.error(t.t0);case 32:return t.prev=32,e.disableSubmit=!1,t.finish(32);case 35:case"end":return t.stop()}}),t,null,[[9,20,32,35]])})))()},factoryReset:function(){var e=this;return Object(o.a)(regeneratorRuntime.mark((function t(){var r,o;return regeneratorRuntime.wrap((function(t){for(;;)switch(t.prev=t.next){case 0:return t.next=2,e.confirm(e.$polyglot.t("messages.confirm_reset"));case 2:if(r=t.sent,e.$modal.hide("dialog"),r){t.next=6;break}return t.abrupt("return");case 6:return o=e.$apiRouter.resolve({name:"cgi/cmd",query:{action:"sysmng",param:"factorydefault_hard"}}).href,t.prev=7,e.$router.push("/"),t.next=11,e.$axios.get(o);case 11:return t.sent,t.next=14,e.alert(e.$polyglot.t("messages.alert_reset"));case 14:e.$modal.hide("dialog"),e.setStatus(!0),t.next=30;break;case 18:if(t.prev=18,t.t0=t.catch(7),!e.$polyglot.has("error_messages."+t.t0)){t.next=26;break}return t.next=23,e.alert(e.$polyglot.t("error_messages."+t.t0));case 23:e.$modal.hide("dialog"),t.next=29;break;case 26:return t.next=28,e.alert(t.t0);case 28:e.$modal.hide("dialog");case 29:console.error(t.t0);case 30:case"end":return t.stop()}}),t,null,[[7,18]])})))()},fwUpdate_XMLRequest:function(){var e=this;return Object(o.a)(regeneratorRuntime.mark((function t(){var r,form,o;return regeneratorRuntime.wrap((function(t){for(;;)switch(t.prev=t.next){case 0:if(null!==e.fw.fwfile){t.next=5;break}return t.next=3,e.alert("fwupdate_XMLRequest() "+e.$polyglot.t("system.fwupdate.messages.fwfile.null"));case 3:return e.$modal.hide("dialog"),t.abrupt("return",!1);case 5:return t.next=7,e.confirm(e.$polyglot.t("messages.confirm_reset"));case 7:if(r=t.sent,e.$modal.hide("dialog"),r){t.next=11;break}return t.abrupt("return");case 11:(form=new FormData).append("fw",e.fw.fwfile),(o=new XMLHttpRequest).open("POST","/cgi/upload.cgi",!0),o.onprogress=function(e){document.getElementById("progressbar").innerHTML=e.loaded+"/"+e.total},o.onerror=function(){console.error(o.responseText)},o.send(form);case 18:case"end":return t.stop()}}),t)})))()},fwUpdate:function(){var e=this;return Object(o.a)(regeneratorRuntime.mark((function t(){var r,o,n,d,m,c,data,f,y,form;return regeneratorRuntime.wrap((function(t){for(;;)switch(t.prev=t.next){case 0:if(!e.FWUpdateSubmit){t.next=5;break}return t.next=3,e.alert(e.$polyglot.t("system.maintenance.messages.fwupdate.in_progress"));case 3:return e.$modal.hide("dialog"),t.abrupt("return",!1);case 5:if(e.FWUpdateSubmit=!0,r=!1,o="Failed Update",null!==e.fw.fwfile){t.next=14;break}return t.next=11,e.alert(e.$polyglot.t("system.fwupdate.messages.fwfile.null"));case 11:return e.$modal.hide("dialog"),e.FWUpdateSubmit=!1,t.abrupt("return",!1);case 14:if(n=e.fw.fwfile.name,"dat"==n.split(".").pop()){t.next=22;break}return t.next=19,e.alert(e.$polyglot.t("system.fwupdate.messages.fwfile.name"));case 19:return e.$modal.hide("dialog"),e.FWUpdateSubmit=!1,t.abrupt("return",!1);case 22:if(!((d=e.fw.fwfile.size)<31457280||d>=62914560)){t.next=32;break}return d>=1073741824?d=(d/=1073741824).toFixed(2)+"GB":d>=1048576?d=(d/=1048576).toFixed(2)+"MB":d>=1024&&(d=(d/=1024).toFixed(2)+"KB"),t.next=27,e.alert(e.$polyglot.t("system.fwupdate.messages.fwfile.size")+"("+d+")");case 27:return e.$modal.hide("dialog"),e.FWUpdateSubmit=!1,e.fw.fwfile=null,e.$refs.selectedfile.value="",t.abrupt("return",!1);case 32:return t.prev=32,m=e.$apiRouter.resolve({name:"cgi/config",query:{action:"search",param:"system_info"}}).href,t.next=36,e.$axios.get(m);case 36:if(c=t.sent,data=Object(l.a)(c),1!=data.ftp){t.next=45;break}return t.next=42,e.alert(e.$polyglot.t("messages.alert_fw_ftp_running"));case 42:return e.$modal.hide("dialog"),e.FWUpdateSubmit=!1,t.abrupt("return",!1);case 45:t.next=61;break;case 47:if(t.prev=47,t.t0=t.catch(32),!e.$polyglot.has("error_messages."+t.t0)){t.next=55;break}return t.next=52,e.alert(e.$polyglot.t("error_messages."+t.t0));case 52:e.$modal.hide("dialog"),t.next=58;break;case 55:return t.next=57,e.alert(t.t0);case 57:e.$modal.hide("dialog");case 58:return console.error(t.t0),e.FWUpdateSubmit=!1,t.abrupt("return",!1);case 61:return t.next=63,e.confirm(e.$polyglot.t("messages.confirm_update"));case 63:if(f=t.sent,e.$modal.hide("dialog"),f){t.next=68;break}return e.FWUpdateSubmit=!1,t.abrupt("return",!1);case 68:return e.modalText=e.$polyglot.t("messages.firmware_updating"),e.modalText2=e.$polyglot.t("messages.stay_page"),e.showModal=!0,e.uploadedCurrSize=0,e.uploadedPrevSize=0,0,y=e.$apiRouter.resolve({name:"cgi/upload"}).href,t.prev=75,(form=new FormData).append("fw",e.fw.fwfile),t.next=80,e.$axios.post(y,form,{headers:{"Content-Type":"multipart/form-data"},onUploadProgress:function(e){this.uploadPercentage=parseInt(Math.round(100*e.loaded/e.total))}.bind(e)}).then((function(t){var n=t.data;n.includes("OK_FW_UPDATE")?r=!0:o=n.includes("ERR_FWUPDATE_FTP_RUNNING")?e.$polyglot.t("messages.alert_fw_ftp_running"):e.$polyglot.t("messages.failedAndCheckFile")})).catch((function(e){console.log(e)}));case 80:if(t.sent,!r){t.next=90;break}return t.next=84,e.alert(e.$polyglot.t("messages.alert_restart_for_update"));case 84:return e.$modal.hide("dialog"),t.next=87,new Promise((function(e){return setTimeout(e,2e3)}));case 87:e.setStatus(!0),t.next=93;break;case 90:return t.next=92,e.alert(o);case 92:e.$modal.hide("dialog");case 93:t.next=107;break;case 95:if(t.prev=95,t.t1=t.catch(75),!e.$polyglot.has("error_messages."+t.t1)){t.next=103;break}return t.next=100,e.alert(e.$polyglot.t("error_messages."+t.t1));case 100:e.$modal.hide("dialog"),t.next=106;break;case 103:return t.next=105,e.alert(t.t1);case 105:e.$modal.hide("dialog");case 106:console.error(t.t1);case 107:e.showModal=!1,e.FWUpdateSubmit=!1;case 109:case"end":return t.stop()}}),t,null,[[32,47],[75,95]])})))()},fileSelect:function(e){var text=this.$refs.fwfile.files[0];this.fw.fwfile=text,this.$refs.selectedfile.value=text.name},macToId:function(){this.form.system.devid="_MACADDRESS_"},validation:function(){var e=this;return Object(o.a)(regeneratorRuntime.mark((function t(){return regeneratorRuntime.wrap((function(t){for(;;)switch(t.prev=t.next){case 0:if(e.form.system.devid){t.next=6;break}return t.next=3,e.alert(e.$polyglot.t("system.system.messages.devid.null"));case 3:return e.$modal.hide("dialog"),e.$refs.system_devid.focus(),t.abrupt("return",!1);case 6:if(v.test(e.form.system.devid)){t.next=12;break}return t.next=9,e.alert(e.$polyglot.t("system.system.messages.devid.format"));case 9:return e.$modal.hide("dialog"),e.$refs.system_devid.focus(),t.abrupt("return",!1);case 12:if("NEXX360"!=e.form.model&&"NEXX360B"!=e.form.model&&"NEXX360C"!=e.form.model&&"NEXX360W_CCTV"!=e.form.model&&"NEXX360W"!=e.form.model&&"NEXXB"!=e.form.model&&"NEXX360M"!=e.form.model&&"NEXXONE"!=e.form.model&&"NEXX360W_MUX"!=e.form.model&&"NEXXB_ONE"!=e.form.model||e.form.system.uid){t.next=18;break}return t.next=15,e.alert(e.$polyglot.t("system.system.messages.uid.null"));case 15:return e.$modal.hide("dialog"),e.$refs.system_uid.focus(),t.abrupt("return",!1);case 18:if(v.test(e.form.system.uid)){t.next=24;break}return t.next=21,e.alert(e.$polyglot.t("system.system.messages.uid.format"));case 21:return e.$modal.hide("dialog"),e.$refs.system_uid.focus(),t.abrupt("return",!1);case 24:return t.abrupt("return",!0);case 25:case"end":return t.stop()}}),t)})))()}},Object(m.b)("device",["setStatus"])),created:function(){this.fetchData()}},x=(r(328),Object(c.a)(h,(function(){var e=this,t=e.$createElement,r=e._self._c||t;return r("div",{staticClass:"system-page"},[r("header",{staticClass:"page-header"},[r("h1",[e._v(e._s(e.$polyglot.t("system.page_title")))])]),e._v(" "),r("main",{staticClass:"page-body"},[r("div",{attrs:{id:"app"}},[r("MyModal",{directives:[{name:"show",rawName:"v-show",value:e.showModal,expression:"showModal"}],on:{close:function(t){e.showModal=!1}},scopedSlots:e._u([{key:"mymodal-text",fn:function(){return[e._v(e._s(e.modalText)+" "+e._s(e.modalText2))]},proxy:!0},{key:"mymodal-percent",fn:function(){return[e._v("Uploaded "+e._s(e.uploadPercentage)+" %")]},proxy:!0},{key:"mymodal-progress",fn:function(){return[e._v(e._s(e.progressText))]},proxy:!0}])},[e._v(" "),e._v(" "),r("br")])],1),e._v(" "),r("section",[r("h2",[e._v(e._s(e.$polyglot.t("system.system.title")))]),e._v(" "),r("ol",{staticClass:"controls"},[r("li",[r("label",{attrs:{for:"modelNameField"}},[e._v(e._s(e.$polyglot.t("system.system.fields.model")))]),e._v(" "),r("input",{staticClass:"form-control",attrs:{type:"text",id:"modelField",disabled:"",hidden:""},domProps:{value:e.form.system.model},on:{input:function(t){e.form.system.model=t.target.value}}}),e._v(" "),r("input",{ref:"model_name",staticClass:"form-control",attrs:{type:"text",id:"modelNameField",disabled:"",placeholder:""}})]),e._v(" "),r("li",[r("label",{attrs:{for:"deviceField"}},[e._v(e._s(e.$polyglot.t("system.system.fields.devid")))]),e._v(" "),r("input",{ref:"system_devid",staticClass:"form-control",attrs:{type:"text",id:"deviceField",maxlength:"31",placeholder:""},domProps:{value:e.form.system.devid},on:{input:function(t){e.form.system.devid=t.target.value}}}),e._v(" "),r("button",{staticClass:"text-btn",attrs:{type:"button"},on:{click:e.macToId}},[e._v("\n\t\t\t\t\t\t"+e._s(e.$polyglot.t("system.system.fields.mac_to_id"))+"\n\t\t\t\t\t\t"),r("img",{staticClass:"hover",attrs:{src:"/images/components/arrows/right-hover.svg",alt:"arrow right"}}),e._v(" "),r("img",{staticClass:"default",attrs:{src:"/images/components/arrows/right-default.svg",alt:"arrow right"}})])]),e._v(" "),"NEXX360"===e.form.model||"NEXX360B"===e.form.model||"NEXX360C"===e.form.model||"NEXX360W_CCTV"===e.form.model||"NEXX360W"===e.form.model||"NEXXB"===e.form.model||"NEXX360M"===e.form.model||"NEXXONE"===e.form.model||"NEXX360W_MUX"===e.form.model||"NEXXB_ONE"===e.form.model?r("li",[r("label",{attrs:{for:"uidField"}},[e._v(e._s(e.$polyglot.t("system.system.fields.uid")))]),e._v(" "),r("input",{ref:"system_uid",staticClass:"form-control",attrs:{type:"text",id:"uidField",maxlength:"31",placeholder:"",disabled:""},domProps:{value:e.form.system.uid},on:{input:function(t){e.form.system.uid=t.target.value}}})]):e._e()])]),e._v(" "),r("section",[r("h2",[e._v(e._s(e.$polyglot.t("system.maintenance.name")))]),e._v(" "),r("ol",{staticClass:"controls"},[r("li",[r("label",{attrs:{for:"firmwareVersion"}},[e._v(e._s(e.$polyglot.t("system.maintenance.fields.version")))]),e._v(" "),r("input",{staticClass:"form-control",attrs:{type:"text",id:"firmwareVersion",disabled:"",placeholder:""},domProps:{value:e.form.system.fwver},on:{input:function(t){e.form.system.fwver=t.target.value}}})]),e._v(" "),r("li",[r("label",{attrs:{for:"factoryreset"}},[e._v(e._s(e.$polyglot.t("system.maintenance.fields.configuration")))]),e._v(" "),r("button",{staticClass:"text-btn",attrs:{id:"factoryreset",type:"button"},on:{click:e.factoryReset}},[e._v("\n\t\t\t\t\t\t"+e._s(e.$polyglot.t("system.maintenance.fields.factoryreset"))+"\n\t\t\t\t\t\t"),r("img",{staticClass:"hover",attrs:{src:"/images/components/arrows/right-hover.svg",alt:"arrow right"}}),e._v(" "),r("img",{staticClass:"default",attrs:{src:"/images/components/arrows/right-default.svg",alt:"arrow right"}})])]),e._v(" "),r("label",{staticStyle:{"font-size":"14px","margin-top":"10px"}},[e._v(e._s(e.$polyglot.t("system.maintenance.fields.fwupdate")))]),e._v(" "),r("div",{staticClass:"li-fwupdate"},[r("div",{ref:"progressbar",attrs:{id:"progressbar"}}),e._v(" "),r("div",{staticClass:"filebox"},[r("label",{attrs:{for:"ctrlfwfile"}},[e._v("...")]),e._v(" "),r("input",{ref:"fwfile",attrs:{type:"file",id:"ctrlfwfile"},on:{change:e.fileSelect}}),e._v(" "),r("input",{ref:"selectedfile",staticClass:"upload-name",attrs:{type:"text",placeholder:"Choose F/W file to upload"}})]),e._v(" "),r("div",{staticClass:"div-update-btn"},[r("button",{staticClass:"text-btn",attrs:{type:"button"},on:{click:e.fwUpdate}},[e._v("\n\t\t\t\t\t\t"+e._s(e.$polyglot.t("system.maintenance.fields.update"))+"\n\t\t\t\t\t\t"),r("img",{staticClass:"hover",attrs:{src:"/images/components/arrows/right-hover.svg",alt:"arrow right"}}),e._v(" "),r("img",{staticClass:"default",attrs:{src:"/images/components/arrows/right-default.svg",alt:"arrow right"}})])])])])])]),e._v(" "),r("footer",{staticClass:"page-footer"},[r("div",{staticClass:"cancel-submit-buttons"},[r("nuxt-link",{staticClass:"cancel-btn",attrs:{to:"/"}},[e._v(e._s(e.$polyglot.t("buttons.cancel")))]),e._v(" "),r("button",{staticClass:"submit-btn",attrs:{type:"submit",disabled:e.disableSubmit},on:{click:function(t){return t.preventDefault(),e.storeData(t)}}},[e._v("\n\t\t\t\t"+e._s(e.$polyglot.t("buttons.submit"))+"\n\t\t\t")])],1)])])}),[],!1,null,null,null));t.default=x.exports}}]);