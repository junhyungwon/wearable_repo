(window.webpackJsonp=window.webpackJsonp||[]).push([[9],{301:function(e,t,r){var content=r(306);"string"==typeof content&&(content=[[e.i,content,""]]),content.locals&&(e.exports=content.locals);(0,r(78).default)("e8eb5854",content,!0,{sourceMap:!1})},302:function(e,t,r){"use strict";r.d(t,"a",(function(){return n}));r(101);var o=r(24);function n(e){if("object"===Object(o.a)(e.data)){if(void 0!==e.data["return value"]){if(0==e.data["return value"]||1==e.data["return value"])return e.data;throw e.data["return value"]}return e.data}var t=e.data.match(/\-\d+/m);if(!t)throw"unknown";if(t.length<1)throw"unknown";throw t[0]}},303:function(e,t,r){"use strict";r(1);t.a={methods:{alert:function(e){var t=this;return new Promise((function(r,o){t.$modal.show("dialog",{text:e,buttons:[{title:t.$polyglot.t("dialog.ok"),handler:function(){r(!0)}}]})}))},confirm:function(e){var t=this;return new Promise((function(r,o){t.$modal.show("dialog",{text:e,buttons:[{title:t.$polyglot.t("dialog.cancel"),handler:function(){r(!1)}},{title:t.$polyglot.t("dialog.ok"),default:!0,handler:function(){r(!0)}}]})}))}}}},304:function(e,t,r){"use strict";r.d(t,"a",(function(){return n}));var o=r(24);function n(e,t){for(var r in e)void 0!==t[r]&&(Array.isArray(t[r])?e[r]=t[r].map((function(e){return e})):("object"===Object(o.a)(t[r])&&t[r]&&n(e[r],t[r]),e[r]=isNaN(t[r])?t[r]:parseInt(t[r],10)))}},305:function(e,t,r){"use strict";r(301)},306:function(e,t,r){(t=r(77)(!1)).push([e.i,"label[data-v-9e138a9e]{display:flex;width:100%;align-items:center}label input[type=checkbox][data-v-9e138a9e]{display:none}label img[data-v-9e138a9e]{display:block;flex-basis:24px;flex-shrink:0;flex-grow:0;width:24px;height:24px}label:hover img.unchecked[data-v-9e138a9e],label img.hover[data-v-9e138a9e]{display:none}label:hover img.hover[data-v-9e138a9e]{display:block}label span[data-v-9e138a9e]{flex-grow:1;padding-left:14px;font-size:14px;font-weight:400;letter-spacing:.26px;color:#fff;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}",""]),e.exports=t},307:function(e,t,r){"use strict";var o={props:["checked"],computed:{state:{get:function(){return!0===this.checked||!1!==this.checked&&(isNaN(this.checked)?!!this.checked:!!parseInt(this.checked,10))},set:function(e){this.$emit("updated",e)}}}},n=(r(305),r(33)),component=Object(n.a)(o,(function(){var e=this,t=e.$createElement,r=e._self._c||t;return r("label",[r("input",{directives:[{name:"model",rawName:"v-model",value:e.state,expression:"state"}],attrs:{type:"checkbox"},domProps:{checked:Array.isArray(e.state)?e._i(e.state,null)>-1:e.state},on:{change:function(t){var r=e.state,o=t.target,n=!!o.checked;if(Array.isArray(r)){var l=e._i(r,null);o.checked?l<0&&(e.state=r.concat([null])):l>-1&&(e.state=r.slice(0,l).concat(r.slice(l+1)))}else e.state=n}}}),e._v(" "),e.state?r("img",{staticClass:"checked",attrs:{src:"/images/components/input-checkbox/filled.svg",alt:"Checked"}}):[r("img",{staticClass:"unchecked",attrs:{src:"/images/components/input-checkbox/outline.svg",alt:"Unchecked"}}),e._v(" "),r("img",{staticClass:"hover",attrs:{src:"/images/components/input-checkbox/hover.svg",alt:"Active"}})],e._v(" "),e._t("default")],2)}),[],!1,null,"9e138a9e",null);t.a=component.exports},312:function(e,t,r){var content=r(323);"string"==typeof content&&(content=[[e.i,content,""]]),content.locals&&(e.exports=content.locals);(0,r(78).default)("2d7fc220",content,!0,{sourceMap:!1})},322:function(e,t,r){"use strict";r(312)},323:function(e,t,r){(t=r(77)(!1)).push([e.i,"h2.checkbox-flex[data-v-a3515fec]{display:flex;align-items:center}h2.checkbox-flex>label[data-v-a3515fec]{margin:0 10px 0 0;flex-basis:24px}",""]),e.exports=t},332:function(e,t,r){"use strict";r.r(t);r(50);var o=r(10),n=(r(1),r(307)),l=r(302),c=r(304),dialog=r(303),d=/^([0-9]|[a-z]|[A-Z]|_|\-|!|@|#|\$|%|\^){1,31}$/,f={components:{"input-checkbox":n.a},mixins:[dialog.a],data:function(){return{form:{model:null,current_pw:null,new_pw:null,confirm_pw:null,live_stream_account:{enable:null,enctype:null,id:null,pw:null},onvif:{id:null,pw:null}},disableSubmit:!1,updatePassword:!1}},watch:{"form.model":function(e){"NEXXONE"===this.form.model?window.document.title="NEXX ONE":window.document.title=e}},methods:{wait:function(){return new Promise((function(e){return setTimeout(e,1e3)}))},fetchData:function(){var e=this;return Object(o.a)(regeneratorRuntime.mark((function t(){var r,o,data;return regeneratorRuntime.wrap((function(t){for(;;)switch(t.prev=t.next){case 0:return r=e.$apiRouter.resolve({name:"cgi/config",query:{action:"search",param:"user_config"}}).href,t.prev=1,t.next=4,e.$axios.get(r);case 4:return o=t.sent,data=Object(l.a)(o),Object(c.a)(e.form,data),t.next=9,e.wait();case 9:e.disableSubmit=!1,t.next=24;break;case 12:if(t.prev=12,t.t0=t.catch(1),!e.$polyglot.has("error_messages."+t.t0)){t.next=20;break}return t.next=17,e.alert(e.$polyglot.t("error_messages."+t.t0));case 17:e.$modal.hide("dialog"),t.next=23;break;case 20:return t.next=22,e.alert(t.t0);case 22:e.$modal.hide("dialog");case 23:console.error(t.t0);case 24:case"end":return t.stop()}}),t,null,[[1,12]])})))()},storeData:function(){var e=this;return Object(o.a)(regeneratorRuntime.mark((function t(){var r,form,o;return regeneratorRuntime.wrap((function(t){for(;;)switch(t.prev=t.next){case 0:return t.next=2,e.validation();case 2:if(t.sent){t.next=4;break}return t.abrupt("return");case 4:return e.disableSubmit=!0,r=e.$apiRouter.resolve({name:"cgi/user"}).href,(form=new URLSearchParams).append("chk_change_web_passwd",e.updatePassword?"1":"0"),form.append("txt_cur_pw",e.form.current_pw),form.append("txt_new_pw1",e.form.new_pw),form.append("txt_new_pw2",e.form.confirm_pw),form.append("txt_onvif_id",e.form.onvif.id),form.append("txt_onvif_pw",e.form.onvif.pw),"FITT360 Security"==e.form.model&&form.append("live_stream_account_enable",e.form.live_stream_account.enable?"1":"0"),form.append("cbo_live_stream_account_enc_type",e.form.live_stream_account.enctype),form.append("txt_live_stream_account_id",e.form.live_stream_account.id),form.append("txt_live_stream_account_pw",e.form.live_stream_account.pw),t.prev=17,t.next=20,e.$axios.post(r,form);case 20:return o=t.sent,Object(l.a)(o),t.next=24,e.alert(e.$polyglot.t("messages.saved"));case 24:e.$modal.hide("dialog"),e.$router.push("/"),t.next=41;break;case 28:if(t.prev=28,t.t0=t.catch(17),console.log(t.t0),!e.$polyglot.has("error_messages."+t.t0)){t.next=37;break}return t.next=34,e.alert(e.$polyglot.t("error_messages."+t.t0));case 34:e.$modal.hide("dialog"),t.next=40;break;case 37:return t.next=39,e.alert(t.t0);case 39:e.$modal.hide("dialog");case 40:console.error(t.t0);case 41:return t.prev=41,e.disableSubmit=!1,t.finish(41);case 44:case"end":return t.stop()}}),t,null,[[17,28,41,44]])})))()},validation:function(){var e=this;return Object(o.a)(regeneratorRuntime.mark((function t(){return regeneratorRuntime.wrap((function(t){for(;;)switch(t.prev=t.next){case 0:if(!e.updatePassword){t.next=13;break}if(e.form.current_pw){t.next=7;break}return t.next=4,e.alert(e.$polyglot.t("user.password.messages.current_pw.null"));case 4:return e.$modal.hide("dialog"),e.$refs.current_pw.focus(),t.abrupt("return",!1);case 7:if(d.test(e.form.current_pw)){t.next=13;break}return t.next=10,e.alert(e.$polyglot.t("user.password.messages.current_pw.format"));case 10:return e.$modal.hide("dialog"),e.$refs.current_pw.focus(),t.abrupt("return",!1);case 13:if(!e.updatePassword){t.next=32;break}if(e.form.new_pw){t.next=20;break}return t.next=17,e.alert(e.$polyglot.t("user.password.messages.new_pw.null"));case 17:return e.$modal.hide("dialog"),e.$refs.new_pw.focus(),t.abrupt("return",!1);case 20:if("admin"!==e.form.new_pw){t.next=26;break}return t.next=23,e.alert(e.$polyglot.t("user.password.messages.new_pw.admin"));case 23:return e.$modal.hide("dialog"),e.$refs.new_pw.focus(),t.abrupt("return",!1);case 26:if(d.test(e.form.new_pw)){t.next=32;break}return t.next=29,e.alert(e.$polyglot.t("user.password.messages.new_pw.format"));case 29:return e.$modal.hide("dialog"),e.$refs.new_pw.focus(),t.abrupt("return",!1);case 32:if(!e.updatePassword){t.next=45;break}if(e.form.confirm_pw===e.form.new_pw){t.next=39;break}return t.next=36,e.alert(e.$polyglot.t("user.password.messages.confirm_pw.confirmation"));case 36:return e.$modal.hide("dialog"),e.$refs.confirm_pw.focus(),t.abrupt("return",!1);case 39:if(d.test(e.form.confirm_pw)){t.next=45;break}return t.next=42,e.alert(e.$polyglot.t("user.password.messages.confirm_pw.format"));case 42:return e.$modal.hide("dialog"),e.$refs.confirm_pw.focus(),t.abrupt("return",!1);case 45:if(e.form.onvif.pw){t.next=51;break}return t.next=48,e.alert(e.$polyglot.t("user.onvif.messages.pw.null"));case 48:return e.$modal.hide("dialog"),e.$refs.onvif_pw.focus(),t.abrupt("return",!1);case 51:if(d.test(e.form.onvif.pw)){t.next=57;break}return t.next=54,e.alert(e.$polyglot.t("user.onvif.messages.pw.format"));case 54:return e.$modal.hide("dialog"),e.$refs.onvif_pw.focus(),t.abrupt("return",!1);case 57:if(!("NEXX360"==e.form.model||"NEXX360B"==e.form.model||"NEXX360W"==e.form.model||"NEXX360H"==e.form.model||"NEXXONE"==e.form.model||"FITT360 Security"==e.form.model&&e.form.live_stream_account.enable)){t.next=82;break}if(e.form.live_stream_account.id){t.next=64;break}return t.next=61,e.alert(e.$polyglot.t("user.live_stream_account.messages.id.null"));case 61:return e.$modal.hide("dialog"),e.$refs.live_stream_account_id.focus(),t.abrupt("return",!1);case 64:if(d.test(e.form.live_stream_account.id)){t.next=70;break}return t.next=67,e.alert(e.$polyglot.t("user.live_stream_account.messages.id.format"));case 67:return e.$modal.hide("dialog"),e.$refs.live_stream_account_id.focus(),t.abrupt("return",!1);case 70:if(e.form.live_stream_account.pw){t.next=76;break}return t.next=73,e.alert(e.$polyglot.t("user.live_stream_account.messages.pw.null"));case 73:return e.$modal.hide("dialog"),e.$refs.live_stream_account_pw.focus(),t.abrupt("return",!1);case 76:if(d.test(e.form.live_stream_account.pw)){t.next=82;break}return t.next=79,e.alert(e.$polyglot.t("user.live_stream_account.messages.pw.format"));case 79:return e.$modal.hide("dialog"),e.$refs.live_stream_account_pw.focus(),t.abrupt("return",!1);case 82:return t.abrupt("return",!0);case 83:case"end":return t.stop()}}),t)})))()}},created:function(){},mounted:function(){this.fetchData(),this.$refs.current_pw.focus()}},m=(r(322),r(33)),component=Object(m.a)(f,(function(){var e=this,t=e.$createElement,r=e._self._c||t;return r("div",{staticClass:"user-page"},[r("header",{staticClass:"page-header"},[r("h1",[e._v(e._s(e.$polyglot.t("user.page_title")))])]),e._v(" "),r("main",{staticClass:"page-body"},[r("section",[r("h2",{staticClass:"checkbox-flex"},[r("input-checkbox",{attrs:{checked:e.updatePassword},on:{updated:function(t){e.updatePassword=t}}}),e._v(" "),r("span",[e._v("\n\t\t\t\t\t"+e._s(e.$polyglot.t("user.password.name"))+"\n\t\t\t\t")])],1),e._v(" "),r("ol",{staticClass:"controls"},[r("li",[r("label",{attrs:{for:"currentPswd"}},[e._v(e._s(e.$polyglot.t("user.password.fields.current_pw")))]),e._v(" "),r("input",{ref:"current_pw",staticClass:"form-control",attrs:{type:"password",id:"currentPswd",maxlength:"31",placeholder:"",disabled:!e.updatePassword},domProps:{value:e.form.current_pw},on:{input:function(t){e.form.current_pw=t.target.value}}})]),e._v(" "),r("li",[r("label",{attrs:{for:"newPswd"}},[e._v(e._s(e.$polyglot.t("user.password.fields.new_pw")))]),e._v(" "),r("input",{ref:"new_pw",staticClass:"form-control",attrs:{type:"password",id:"newPswd",maxlength:"31",placeholder:"",disabled:!e.updatePassword},domProps:{value:e.form.new_pw},on:{input:function(t){e.form.new_pw=t.target.value}}})]),e._v(" "),r("li",[r("label",{attrs:{for:"confirmPswd"}},[e._v(e._s(e.$polyglot.t("user.password.fields.confirm_pw")))]),e._v(" "),r("input",{ref:"confirm_pw",staticClass:"form-control",attrs:{type:"password",id:"confirmPswd",maxlength:"31",placeholder:"",disabled:!e.updatePassword},domProps:{value:e.form.confirm_pw},on:{input:function(t){e.form.confirm_pw=t.target.value}}})])])]),e._v(" "),r("section",[r("h2",[e._v(e._s(e.$polyglot.t("user.onvif.name")))]),e._v(" "),r("ol",{staticClass:"controls"},[r("li",[r("label",{attrs:{for:"onvifId"}},[e._v(e._s(e.$polyglot.t("user.onvif.fields.id")))]),e._v(" "),r("input",{ref:"onvif_id",staticClass:"form-control",attrs:{type:"text",id:"onvifId",disabled:"",maxlength:"31",placeholder:""},domProps:{value:e.form.onvif.id},on:{input:function(t){e.form.onvif.id=t.target.value}}})]),e._v(" "),r("li",[r("label",{attrs:{for:"onvifPw"}},[e._v(e._s(e.$polyglot.t("user.onvif.fields.pw")))]),e._v(" "),r("input",{ref:"onvif_pw",staticClass:"form-control",attrs:{type:"password",id:"onvifPw",maxlength:"16",placeholder:""},domProps:{value:e.form.onvif.pw},on:{input:function(t){e.form.onvif.pw=t.target.value}}})])])]),e._v(" "),r("section",[r("h2",[e._v(e._s(e.$polyglot.t("user.live_stream_account.name")))]),e._v(" "),r("ol",{staticClass:"controls"},["FITT360 Security"==e.form.model?r("li",[r("input-checkbox",{attrs:{checked:e.form.live_stream_account.enable},on:{updated:function(t){e.form.live_stream_account.enable=t}}},[r("span",[e._v(e._s(e.$polyglot.t("user.live_stream_account.fields.enable")))])])],1):e._e(),e._v(" "),r("li",[r("label",{attrs:{for:"liveStreamAccountEnctype"}},[e._v(e._s(e.$polyglot.t("user.live_stream_account.fields.enctype")))]),e._v(" "),r("select",{directives:[{name:"model",rawName:"v-model",value:e.form.live_stream_account.enctype,expression:"form.live_stream_account.enctype"}],staticClass:"custom-select small",attrs:{id:"liveStreamAccountEnctype",disabled:!e.form.live_stream_account.enable},on:{change:function(t){var r=Array.prototype.filter.call(t.target.options,(function(e){return e.selected})).map((function(e){return"_value"in e?e._value:e.value}));e.$set(e.form.live_stream_account,"enctype",t.target.multiple?r:r[0])}}},[r("option",{domProps:{value:0}},[e._v(e._s(e.$polyglot.t("values.none")))]),e._v(" "),r("option",{domProps:{value:1}},[e._v(e._s(e.$polyglot.t("values.aes")))])])]),e._v(" "),r("li",[r("label",{attrs:{for:"liveStreamAccountId"}},[e._v(e._s(e.$polyglot.t("user.live_stream_account.fields.id")))]),e._v(" "),r("input",{ref:"live_stream_account_id",staticClass:"form-control",attrs:{type:"text",id:"liveStreamAccountId",disabled:!e.form.live_stream_account.enable,maxlength:"16",placeholder:""},domProps:{value:e.form.live_stream_account.id},on:{input:function(t){e.form.live_stream_account.id=t.target.value}}})]),e._v(" "),r("li",[r("label",{attrs:{for:"liveStreamAccountPw"}},[e._v(e._s(e.$polyglot.t("user.live_stream_account.fields.pw")))]),e._v(" "),r("input",{ref:"live_stream_account_pw",staticClass:"form-control",attrs:{type:"password",id:"liveStreamAccountPw",disabled:!e.form.live_stream_account.enable,maxlength:"16",placeholder:""},domProps:{value:e.form.live_stream_account.pw},on:{input:function(t){e.form.live_stream_account.pw=t.target.value}}})])])])]),e._v(" "),r("footer",{staticClass:"page-footer"},[r("div",{staticClass:"cancel-submit-buttons"},[r("nuxt-link",{staticClass:"cancel-btn",attrs:{to:"/"}},[e._v(e._s(e.$polyglot.t("buttons.cancel")))]),e._v(" "),r("button",{staticClass:"submit-btn",attrs:{type:"submit",disabled:e.disableSubmit},on:{click:function(t){return t.preventDefault(),e.storeData(t)}}},[e._v("\n\t\t\t\t"+e._s(e.$polyglot.t("buttons.submit"))+"\n\t\t\t")])],1)])])}),[],!1,null,"a3515fec",null);t.default=component.exports}}]);