(window.webpackJsonp=window.webpackJsonp||[]).push([[4],{288:function(e,t,r){var content=r(293);"string"==typeof content&&(content=[[e.i,content,""]]),content.locals&&(e.exports=content.locals);(0,r(72).default)("e8eb5854",content,!0,{sourceMap:!1})},289:function(e,t,r){"use strict";r.d(t,"a",(function(){return o}));r(123);var l=r(24);function o(e){if("object"===Object(l.a)(e.data)){if(void 0!==e.data["return value"]){if(0==e.data["return value"]||1==e.data["return value"])return e.data;throw e.data["return value"]}return e.data}var t=e.data.match(/\-\d+/m);if(!t)throw"unknown";if(t.length<1)throw"unknown";throw t[0]}},290:function(e,t,r){"use strict";r(1);t.a={methods:{alert:function(e){var t=this;return new Promise((function(r,l){t.$modal.show("dialog",{text:e,buttons:[{title:t.$polyglot.t("dialog.ok"),handler:function(){r(!0)}}]})}))},confirm:function(e){var t=this;return new Promise((function(r,l){t.$modal.show("dialog",{text:e,buttons:[{title:t.$polyglot.t("dialog.cancel"),handler:function(){r(!1)}},{title:t.$polyglot.t("dialog.ok"),default:!0,handler:function(){r(!0)}}]})}))}}}},291:function(e,t,r){"use strict";r.d(t,"a",(function(){return o}));var l=r(24);function o(e,t){for(var r in e)void 0!==t[r]&&(Array.isArray(t[r])?e[r]=t[r].map((function(e){return e})):("object"===Object(l.a)(t[r])&&t[r]&&o(e[r],t[r]),e[r]=isNaN(t[r])?t[r]:parseInt(t[r],10)))}},292:function(e,t,r){"use strict";var l=r(288);r.n(l).a},293:function(e,t,r){(t=r(71)(!1)).push([e.i,"label[data-v-9e138a9e]{display:-webkit-box;display:flex;width:100%;-webkit-box-align:center;align-items:center}label input[type=checkbox][data-v-9e138a9e]{display:none}label img[data-v-9e138a9e]{display:block;flex-basis:24px;flex-shrink:0;-webkit-box-flex:0;flex-grow:0;width:24px;height:24px}label:hover img.unchecked[data-v-9e138a9e],label img.hover[data-v-9e138a9e]{display:none}label:hover img.hover[data-v-9e138a9e]{display:block}label span[data-v-9e138a9e]{-webkit-box-flex:1;flex-grow:1;padding-left:14px;font-size:14px;font-weight:400;letter-spacing:.26px;color:#fff;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}",""]),e.exports=t},294:function(e,t,r){"use strict";var l={props:["checked"],computed:{state:{get:function(){return!0===this.checked||!1!==this.checked&&(isNaN(this.checked)?!!this.checked:!!parseInt(this.checked,10))},set:function(e){this.$emit("updated",e)}}}},o=(r(292),r(30)),component=Object(o.a)(l,(function(){var e=this,t=e.$createElement,r=e._self._c||t;return r("label",[r("input",{directives:[{name:"model",rawName:"v-model",value:e.state,expression:"state"}],attrs:{type:"checkbox"},domProps:{checked:Array.isArray(e.state)?e._i(e.state,null)>-1:e.state},on:{change:function(t){var r=e.state,l=t.target,o=!!l.checked;if(Array.isArray(r)){var n=e._i(r,null);l.checked?n<0&&(e.state=r.concat([null])):n>-1&&(e.state=r.slice(0,n).concat(r.slice(n+1)))}else e.state=o}}}),e._v(" "),e.state?r("img",{staticClass:"checked",attrs:{src:"/images/components/input-checkbox/filled.svg",alt:"Checked"}}):[r("img",{staticClass:"unchecked",attrs:{src:"/images/components/input-checkbox/outline.svg",alt:"Unchecked"}}),e._v(" "),r("img",{staticClass:"hover",attrs:{src:"/images/components/input-checkbox/hover.svg",alt:"Active"}})],e._v(" "),e._t("default")],2)}),[],!1,null,"9e138a9e",null);t.a=component.exports},295:function(e,t,r){"use strict";r.d(t,"a",(function(){return l}));r(25);function l(e){var t=e.split(".");return 4===t.length&&t.reduce((function(e,t){if(0===t.length||t.length>3)return!1;var r=parseInt(t,10);return!isNaN(r)&&(!(r<0||r>255)&&e)}),!0)}},309:function(e,t,r){"use strict";r.r(t);r(57);var l,o,n,c=r(15),d=(r(1),r(289)),f=r(291),m=r(295),_=r(294),dialog=r(290),h=/^([0-9]|[a-z]|[A-Z]|_|\-|!|@|#|\$|%|\^){1,31}$/,v={components:{"input-checkbox":_.a},mixins:[dialog.a],data:function(){return{form:{model:null,wireless:{addr_type:null,ipv4:null,gateway:null,netmask:null},cradle:{addr_type:null,ipv4:null,gateway:null,netmask:null},wifi_ap:{ssid:null,pw:null},live_stream_account:{enable:null,enctype:null,id:null,pw:null}},disableSubmit:!0}},watch:{"form.model":function(e){window.document.title="NEXX360"==e?"NEXX360":"FITT360 Security"}},methods:{wait:function(){return new Promise((function(e){return setTimeout(e,1e3)}))},fetchData:(n=Object(c.a)(regeneratorRuntime.mark((function e(){var t,r,data;return regeneratorRuntime.wrap((function(e){for(;;)switch(e.prev=e.next){case 0:return t=this.$apiRouter.resolve({name:"cgi/config",query:{action:"search",param:"network_config"}}).href,e.prev=1,e.next=4,this.$axios.get(t);case 4:return r=e.sent,data=Object(d.a)(r),Object(f.a)(this.form,data),e.next=9,this.wait();case 9:this.disableSubmit=!1,e.next=24;break;case 12:if(e.prev=12,e.t0=e.catch(1),!this.$polyglot.has("error_messages."+e.t0)){e.next=20;break}return e.next=17,this.alert(this.$polyglot.t("error_messages."+e.t0));case 17:this.$modal.hide("dialog"),e.next=23;break;case 20:return e.next=22,this.alert(e.t0);case 22:this.$modal.hide("dialog");case 23:console.error(e.t0);case 24:case"end":return e.stop()}}),e,this,[[1,12]])}))),function(){return n.apply(this,arguments)}),storeData:(o=Object(c.a)(regeneratorRuntime.mark((function e(){var t,form,r;return regeneratorRuntime.wrap((function(e){for(;;)switch(e.prev=e.next){case 0:return e.next=2,this.validation();case 2:if(e.sent){e.next=4;break}return e.abrupt("return");case 4:return this.disableSubmit=!0,t=this.$apiRouter.resolve({name:"cgi/network"}).href,(form=new URLSearchParams).append("cbo_wireless_ip_type",this.form.wireless.addr_type),0===this.form.wireless.addr_type&&(form.append("txt_wireless_ipv4",this.form.wireless.ipv4),form.append("txt_wireless_gw",this.form.wireless.gateway),form.append("txt_wireless_mask",this.form.wireless.netmask)),form.append("cbo_cradle_ip_type",this.form.cradle.addr_type),0===this.form.cradle.addr_type&&(form.append("txt_cradle_ipv4",this.form.cradle.ipv4),form.append("txt_cradle_gw",this.form.cradle.gateway),form.append("txt_cradle_mask",this.form.cradle.netmask)),form.append("txt_wifi_ap_ssid",this.form.wifi_ap.ssid),form.append("txt_wifi_ap_pw",this.form.wifi_ap.pw),form.append("live_stream_account_enable",this.form.live_stream_account.enable?"1":"0"),form.append("cbo_live_stream_account_enc_type",this.form.live_stream_account.enctype),form.append("txt_live_stream_account_id",this.form.live_stream_account.id),form.append("txt_live_stream_account_pw",this.form.live_stream_account.pw),e.prev=17,e.next=20,this.$axios.post(t,form);case 20:return r=e.sent,Object(d.a)(r),e.next=24,this.alert(this.$polyglot.t("messages.saved"));case 24:this.$modal.hide("dialog"),this.$router.push("/"),e.next=40;break;case 28:if(e.prev=28,e.t0=e.catch(17),!this.$polyglot.has("error_messages."+e.t0)){e.next=36;break}return e.next=33,this.alert(this.$polyglot.t("error_messages."+e.t0));case 33:this.$modal.hide("dialog"),e.next=39;break;case 36:return e.next=38,this.alert(e.t0);case 38:this.$modal.hide("dialog");case 39:console.error(e.t0);case 40:return e.prev=40,this.disableSubmit=!1,e.finish(40);case 43:case"end":return e.stop()}}),e,this,[[17,28,40,43]])}))),function(){return o.apply(this,arguments)}),validation:(l=Object(c.a)(regeneratorRuntime.mark((function e(){return regeneratorRuntime.wrap((function(e){for(;;)switch(e.prev=e.next){case 0:if(0!==this.form.wireless.addr_type){e.next=37;break}if(this.form.wireless.ipv4){e.next=7;break}return e.next=4,this.alert(this.$polyglot.t("network.wireless.messages.ipv4.null"));case 4:return this.$modal.hide("dialog"),this.$refs.wireless_ipv4.focus(),e.abrupt("return",!1);case 7:if(Object(m.a)(this.form.wireless.ipv4)){e.next=13;break}return e.next=10,this.alert(this.$polyglot.t("network.wireless.messages.ipv4.format"));case 10:return this.$modal.hide("dialog"),this.$refs.wireless_ipv4.focus(),e.abrupt("return",!1);case 13:if(this.form.wireless.gateway){e.next=19;break}return e.next=16,this.alert(this.$polyglot.t("network.wireless.messages.gateway.null"));case 16:return this.$modal.hide("dialog"),this.$refs.wireless_gateway.focus(),e.abrupt("return",!1);case 19:if(Object(m.a)(this.form.wireless.gateway)){e.next=25;break}return e.next=22,this.alert(this.$polyglot.t("network.wireless.messages.gateway.format"));case 22:return this.$modal.hide("dialog"),this.$refs.wireless_gateway.focus(),e.abrupt("return",!1);case 25:if(this.form.wireless.netmask){e.next=31;break}return e.next=28,this.alert(this.$polyglot.t("network.wireless.messages.netmask.null"));case 28:return this.$modal.hide("dialog"),this.$refs.wireless_netmask.focus(),e.abrupt("return",!1);case 31:if(Object(m.a)(this.form.wireless.netmask)){e.next=37;break}return e.next=34,this.alert(this.$polyglot.t("network.wireless.messages.netmask.format"));case 34:return this.$modal.hide("dialog"),this.$refs.wireless_netmask.focus(),e.abrupt("return",!1);case 37:if(0!==this.form.cradle.addr_type){e.next=74;break}if(this.form.cradle.ipv4){e.next=44;break}return e.next=41,this.alert(this.$polyglot.t("network.cradle.messages.ipv4.null"));case 41:return this.$modal.hide("dialog"),this.$refs.cradle_ipv4.focus(),e.abrupt("return",!1);case 44:if(Object(m.a)(this.form.cradle.ipv4)){e.next=50;break}return e.next=47,this.alert(this.$polyglot.t("network.cradle.messages.ipv4.format"));case 47:return this.$modal.hide("dialog"),this.$refs.cradle_ipv4.focus(),e.abrupt("return",!1);case 50:if(this.form.cradle.gateway){e.next=56;break}return e.next=53,this.alert(this.$polyglot.t("network.cradle.messages.gateway.null"));case 53:return this.$modal.hide("dialog"),this.$refs.cradle_gateway.focus(),e.abrupt("return",!1);case 56:if(Object(m.a)(this.form.cradle.gateway)){e.next=62;break}return e.next=59,this.alert(this.$polyglot.t("network.cradle.messages.gateway.format"));case 59:return this.$modal.hide("dialog"),this.$refs.cradle_gateway.focus(),e.abrupt("return",!1);case 62:if(this.form.cradle.netmask){e.next=68;break}return e.next=65,this.alert(this.$polyglot.t("network.cradle.messages.netmask.null"));case 65:return this.$modal.hide("dialog"),this.$refs.cradle_netmask.focus(),e.abrupt("return",!1);case 68:if(Object(m.a)(this.form.cradle.netmask)){e.next=74;break}return e.next=71,this.alert(this.$polyglot.t("network.cradle.messages.netmask.format"));case 71:return this.$modal.hide("dialog"),this.$refs.cradle_netmask.focus(),e.abrupt("return",!1);case 74:if(this.form.wifi_ap.ssid){e.next=80;break}return e.next=77,this.alert(this.$polyglot.t("network.wifi_ap.messages.ssid.null"));case 77:return this.$modal.hide("dialog"),this.$refs.wifi_ap_ssid.focus(),e.abrupt("return",!1);case 80:if(h.test(this.form.wifi_ap.ssid)){e.next=86;break}return e.next=83,this.alert(this.$polyglot.t("network.wifi_ap.messages.ssid.format"));case 83:return this.$modal.hide("dialog"),this.$refs.wifi_ap_ssid.focus(),e.abrupt("return",!1);case 86:if(!this.form.wifi_ap.pw||h.test(this.form.wifi_ap.pw)){e.next=92;break}return e.next=89,this.alert(this.$polyglot.t("network.wifi_ap.messages.pw.format"));case 89:return this.$modal.hide("dialog"),this.$refs.wifi_ap_pw.focus(),e.abrupt("return",!1);case 92:if(!this.form.live_stream_account.enable){e.next=117;break}if(this.form.live_stream_account.id){e.next=99;break}return e.next=96,this.alert(this.$polyglot.t("network.live_stream_account.messages.id.null"));case 96:return this.$modal.hide("dialog"),this.$refs.live_stream_account_id.focus(),e.abrupt("return",!1);case 99:if(h.test(this.form.live_stream_account.id)){e.next=105;break}return e.next=102,this.alert(this.$polyglot.t("network.live_stream_account.messages.id.format"));case 102:return this.$modal.hide("dialog"),this.$refs.live_stream_account_id.focus(),e.abrupt("return",!1);case 105:if(this.form.live_stream_account.pw){e.next=111;break}return e.next=108,this.alert(this.$polyglot.t("network.live_stream_account.messages.pw.null"));case 108:return this.$modal.hide("dialog"),this.$refs.live_stream_account_pw.focus(),e.abrupt("return",!1);case 111:if(h.test(this.form.live_stream_account.pw)){e.next=117;break}return e.next=114,this.alert(this.$polyglot.t("network.live_stream_account.messages.pw.format"));case 114:return this.$modal.hide("dialog"),this.$refs.live_stream_account_pw.focus(),e.abrupt("return",!1);case 117:return e.abrupt("return",!0);case 118:case"end":return e.stop()}}),e,this)}))),function(){return l.apply(this,arguments)})},created:function(){this.fetchData()}},w=r(30),component=Object(w.a)(v,(function(){var e=this,t=e.$createElement,r=e._self._c||t;return r("div",{staticClass:"network-page"},[r("header",{staticClass:"page-header"},[r("h1",[e._v(e._s(e.$polyglot.t("network.page_title")))])]),e._v(" "),r("main",{staticClass:"page-body"},[r("section",[r("h2",[e._v(e._s(e.$polyglot.t("network.wireless.name")))]),e._v(" "),r("ol",{staticClass:"controls"},[r("li",[r("label",{attrs:{for:"wirelessAddrType"}},[e._v(e._s(e.$polyglot.t("network.wireless.fields.addr_type")))]),e._v(" "),r("select",{directives:[{name:"model",rawName:"v-model",value:e.form.wireless.addr_type,expression:"form.wireless.addr_type"}],staticClass:"custom-select small",attrs:{id:"wirelessAddrType"},on:{change:function(t){var r=Array.prototype.filter.call(t.target.options,(function(e){return e.selected})).map((function(e){return"_value"in e?e._value:e.value}));e.$set(e.form.wireless,"addr_type",t.target.multiple?r:r[0])}}},[r("option",{domProps:{value:0}},[e._v(e._s(e.$polyglot.t("values.static")))]),e._v(" "),r("option",{domProps:{value:1}},[e._v(e._s(e.$polyglot.t("values.dhcp")))])])]),e._v(" "),r("li",[r("label",{attrs:{for:"wirelessIpv4"}},[e._v(e._s(e.$polyglot.t("network.wireless.fields.ipv4")))]),e._v(" "),r("input",{ref:"wireless_ipv4",staticClass:"form-control",attrs:{type:"text",id:"wirelessIpv4",disabled:1==e.form.wireless.addr_type,placeholder:"0.0.0.0"},domProps:{value:e.form.wireless.ipv4},on:{input:function(t){e.form.wireless.ipv4=t.target.value}}})]),e._v(" "),r("li",[r("label",{attrs:{for:"wirelessGateway"}},[e._v(e._s(e.$polyglot.t("network.wireless.fields.gateway")))]),e._v(" "),r("input",{ref:"wireless_gateway",staticClass:"form-control",attrs:{type:"text",id:"wirelessGateway",disabled:1==e.form.wireless.addr_type,placeholder:"0.0.0.0"},domProps:{value:e.form.wireless.gateway},on:{input:function(t){e.form.wireless.gateway=t.target.value}}})]),e._v(" "),r("li",[r("label",{attrs:{for:"wirelessNetmask"}},[e._v(e._s(e.$polyglot.t("network.wireless.fields.netmask")))]),e._v(" "),r("input",{ref:"wireless_netmask",staticClass:"form-control",attrs:{type:"text",id:"wirelessNetmask",disabled:1==e.form.wireless.addr_type,placeholder:"0.0.0.0"},domProps:{value:e.form.wireless.netmask},on:{input:function(t){e.form.wireless.netmask=t.target.value}}})])])]),e._v(" "),r("section",[r("h2",[e._v(e._s(e.$polyglot.t("network.cradle.name")))]),e._v(" "),r("ol",{staticClass:"controls"},[r("li",[r("label",{attrs:{for:"cradleAddrType"}},[e._v(e._s(e.$polyglot.t("network.cradle.fields.addr_type")))]),e._v(" "),r("select",{directives:[{name:"model",rawName:"v-model",value:e.form.cradle.addr_type,expression:"form.cradle.addr_type"}],staticClass:"custom-select small",attrs:{id:"cradleAddrType"},on:{change:function(t){var r=Array.prototype.filter.call(t.target.options,(function(e){return e.selected})).map((function(e){return"_value"in e?e._value:e.value}));e.$set(e.form.cradle,"addr_type",t.target.multiple?r:r[0])}}},[r("option",{domProps:{value:0}},[e._v(e._s(e.$polyglot.t("values.static")))]),e._v(" "),r("option",{domProps:{value:1}},[e._v(e._s(e.$polyglot.t("values.dhcp")))])])]),e._v(" "),r("li",[r("label",{attrs:{for:"cradleIpv4"}},[e._v(e._s(e.$polyglot.t("network.cradle.fields.ipv4")))]),e._v(" "),r("input",{ref:"cradle_ipv4",staticClass:"form-control",attrs:{type:"text",id:"cradleIpv4",disabled:1==e.form.cradle.addr_type,placeholder:"0.0.0.0"},domProps:{value:e.form.cradle.ipv4},on:{input:function(t){e.form.cradle.ipv4=t.target.value}}})]),e._v(" "),r("li",[r("label",{attrs:{for:"cradleGateway"}},[e._v(e._s(e.$polyglot.t("network.cradle.fields.gateway")))]),e._v(" "),r("input",{ref:"cradle_gateway",staticClass:"form-control",attrs:{type:"text",id:"cradleGateway",disabled:1==e.form.cradle.addr_type,placeholder:"0.0.0.0"},domProps:{value:e.form.cradle.gateway},on:{input:function(t){e.form.cradle.gateway=t.target.value}}})]),e._v(" "),r("li",[r("label",{attrs:{for:"cradleNetmask"}},[e._v(e._s(e.$polyglot.t("network.cradle.fields.netmask")))]),e._v(" "),r("input",{ref:"cradle_netmask",staticClass:"form-control",attrs:{type:"text",id:"cradleNetmask",disabled:1==e.form.cradle.addr_type,placeholder:"0.0.0.0"},domProps:{value:e.form.cradle.netmask},on:{input:function(t){e.form.cradle.netmask=t.target.value}}})])])]),e._v(" "),r("section",[r("h2",[e._v(e._s(e.$polyglot.t("network.wifi_ap.name")))]),e._v(" "),r("ol",{staticClass:"controls"},[r("li",[r("label",{attrs:{for:"wifiApSsid"}},[e._v(e._s(e.$polyglot.t("network.wifi_ap.fields.ssid")))]),e._v(" "),r("input",{ref:"wifi_ap_ssid",staticClass:"form-control",attrs:{type:"text",id:"wifiApSsid",maxlength:"31",placeholder:""},domProps:{value:e.form.wifi_ap.ssid},on:{input:function(t){e.form.wifi_ap.ssid=t.target.value}}})]),e._v(" "),r("li",[r("label",{attrs:{for:"wifiApPw"}},[e._v(e._s(e.$polyglot.t("network.wifi_ap.fields.pw")))]),e._v(" "),r("input",{ref:"wifi_ap_pw",staticClass:"form-control",attrs:{type:"password",id:"wifiApPw",maxlength:"31",placeholder:""},domProps:{value:e.form.wifi_ap.pw},on:{input:function(t){e.form.wifi_ap.pw=t.target.value}}})])])]),e._v(" "),r("section",[r("h2",[e._v(e._s(e.$polyglot.t("network.live_stream_account.name")))]),e._v(" "),r("ol",{staticClass:"controls"},[r("li",[r("input-checkbox",{attrs:{checked:e.form.live_stream_account.enable},on:{updated:function(t){e.form.live_stream_account.enable=t}}},[r("span",[e._v(e._s(e.$polyglot.t("network.live_stream_account.fields.enable")))])])],1),e._v(" "),r("li",[r("label",{attrs:{for:"liveStreamAccountEnctype"}},[e._v(e._s(e.$polyglot.t("network.live_stream_account.fields.enctype")))]),e._v(" "),r("select",{directives:[{name:"model",rawName:"v-model",value:e.form.live_stream_account.enctype,expression:"form.live_stream_account.enctype"}],staticClass:"custom-select small",attrs:{id:"liveStreamAccountEnctype",disabled:!e.form.live_stream_account.enable},on:{change:function(t){var r=Array.prototype.filter.call(t.target.options,(function(e){return e.selected})).map((function(e){return"_value"in e?e._value:e.value}));e.$set(e.form.live_stream_account,"enctype",t.target.multiple?r:r[0])}}},[r("option",{domProps:{value:0}},[e._v(e._s(e.$polyglot.t("values.none")))]),e._v(" "),r("option",{domProps:{value:1}},[e._v(e._s(e.$polyglot.t("values.aes")))])])]),e._v(" "),r("li",[r("label",{attrs:{for:"liveStreamAccountId"}},[e._v(e._s(e.$polyglot.t("network.live_stream_account.fields.id")))]),e._v(" "),r("input",{ref:"live_stream_account_id",staticClass:"form-control",attrs:{type:"text",id:"liveStreamAccountId",disabled:!e.form.live_stream_account.enable,maxlength:"16",placeholder:""},domProps:{value:e.form.live_stream_account.id},on:{input:function(t){e.form.live_stream_account.id=t.target.value}}})]),e._v(" "),r("li",[r("label",{attrs:{for:"liveStreamAccountPw"}},[e._v(e._s(e.$polyglot.t("network.live_stream_account.fields.pw")))]),e._v(" "),r("input",{ref:"live_stream_account_pw",staticClass:"form-control",attrs:{type:"password",id:"liveStreamAccountPw",disabled:!e.form.live_stream_account.enable,maxlength:"16",placeholder:""},domProps:{value:e.form.live_stream_account.pw},on:{input:function(t){e.form.live_stream_account.pw=t.target.value}}})])])])]),e._v(" "),r("footer",{staticClass:"page-footer"},[r("div",{staticClass:"cancel-submit-buttons"},[r("nuxt-link",{staticClass:"cancel-btn",attrs:{to:"/"}},[e._v(e._s(e.$polyglot.t("buttons.cancel")))]),e._v(" "),r("button",{staticClass:"submit-btn",attrs:{type:"submit",disabled:e.disableSubmit},on:{click:function(t){return t.preventDefault(),e.storeData(t)}}},[e._v("\n\t\t\t\t"+e._s(e.$polyglot.t("buttons.submit"))+"\n\t\t\t")])],1)])])}),[],!1,null,null,null);t.default=component.exports}}]);