(window.webpackJsonp=window.webpackJsonp||[]).push([[3],{289:function(t,e,n){"use strict";n.d(e,"a",(function(){return o}));n(123);var r=n(24);function o(t){if("object"===Object(r.a)(t.data)){if(void 0!==t.data["return value"]){if(0==t.data["return value"]||1==t.data["return value"])return t.data;throw t.data["return value"]}return t.data}var e=t.data.match(/\-\d+/m);if(!e)throw"unknown";if(e.length<1)throw"unknown";throw e[0]}},290:function(t,e,n){"use strict";n(1);e.a={methods:{alert:function(t){var e=this;return new Promise((function(n,r){e.$modal.show("dialog",{text:t,buttons:[{title:e.$polyglot.t("dialog.ok"),handler:function(){n(!0)}}]})}))},confirm:function(t){var e=this;return new Promise((function(n,r){e.$modal.show("dialog",{text:t,buttons:[{title:e.$polyglot.t("dialog.cancel"),handler:function(){n(!1)}},{title:e.$polyglot.t("dialog.ok"),default:!0,handler:function(){n(!0)}}]})}))}}}},299:function(t,e,n){var content=n(309);"string"==typeof content&&(content=[[t.i,content,""]]),content.locals&&(t.exports=content.locals);(0,n(72).default)("2e107364",content,!0,{sourceMap:!1})},308:function(t,e,n){"use strict";var r=n(299);n.n(r).a},309:function(t,e,n){(e=n(71)(!1)).push([t.i,".home-page header.page-header{width:304px;margin:0 auto;padding-top:39px}.home-page header.page-header figure{position:relative;line-height:0;margin-bottom:0}.home-page header.page-header figure.nexx360 figcaption{position:absolute;left:50%;top:69px;-webkit-transform:translateX(-50%);transform:translateX(-50%)}.home-page header.page-header figure.nexx360 figcaption img{width:85px;height:14px}.home-page header.page-header figure.nexxone figcaption{position:absolute;left:70%;top:69px;-webkit-transform:translateX(-50%);transform:translateX(-50%)}.home-page header.page-header figure.nexxone figcaption img{width:85px;height:14px}.home-page header.page-header figure.fitt360-security figcaption{position:absolute;left:50%;top:55px;-webkit-transform:translateX(-50%);transform:translateX(-50%)}.home-page header.page-header figure.fitt360-security figcaption img{width:100px;height:42px}.home-page header.page-header img{width:296px;height:200px}.home-page main{width:340px;margin:-11px auto 0}.home-page main nav ol{list-style-type:none;margin:0;padding:0;display:-webkit-box;display:flex;flex-wrap:wrap;-webkit-box-pack:justify;justify-content:space-between}.home-page main nav ol li{flex-basis:166px;margin-bottom:8px}.home-page main nav ol li a.block{display:block;padding-top:29px;padding-bottom:28px;color:inherit;border-radius:4px;box-shadow:0 2px 14px 0 rgba(0,0,0,.08);background-color:#1f1f1f;font-size:16px;font-weight:300;line-height:1.19;letter-spacing:.57px;text-align:center;color:hsla(0,0%,100%,.87)}.home-page main nav ol li a.block strong{display:block;font-weight:700}.home-page main nav ol li a.restart{display:block;padding-top:5px;padding-bottom:5px;font-size:14px;font-weight:500;line-height:1.36;letter-spacing:.5px;text-align:center;color:hsla(0,0%,100%,.6)}.home-page main nav ol li a{text-decoration:none;text-transform:uppercase;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.home-page main nav ol li a:active,.home-page main nav ol li a:hover{color:#ffc600}.home-page main nav ol li:last-child{flex-basis:100%;margin-top:14px}.home-page main nav ol li:last-child button.text-btn{margin:0 auto;text-transform:uppercase}",""]),t.exports=e},317:function(t,e,n){"use strict";n.r(e);n(5),n(3),n(2),n(1),n(4),n(57);var r=n(15),o=n(0),l=n(289),c=n(53);function h(object,t){var e=Object.keys(object);if(Object.getOwnPropertySymbols){var n=Object.getOwnPropertySymbols(object);t&&(n=n.filter((function(t){return Object.getOwnPropertyDescriptor(object,t).enumerable}))),e.push.apply(e,n)}return e}var m,d,f={layout:"main",mixins:[n(290).a],data:function(){return{model:null}},watch:{model:function(t){window.document.title="NEXX360"==t?"NEXX360":"NEXXONE"==t?"NEXXONE":"FITT360 Security"}},methods:function(t){for(var i=1;i<arguments.length;i++){var source=null!=arguments[i]?arguments[i]:{};i%2?h(Object(source),!0).forEach((function(e){Object(o.a)(t,e,source[e])})):Object.getOwnPropertyDescriptors?Object.defineProperties(t,Object.getOwnPropertyDescriptors(source)):h(Object(source)).forEach((function(e){Object.defineProperty(t,e,Object.getOwnPropertyDescriptor(source,e))}))}return t}({fetchData:(d=Object(r.a)(regeneratorRuntime.mark((function t(){var e,n,data;return regeneratorRuntime.wrap((function(t){for(;;)switch(t.prev=t.next){case 0:return e=this.$apiRouter.resolve({name:"cgi/config",query:{action:"search",param:"all_config"}}).href,t.prev=1,t.next=4,this.$axios.get(e);case 4:n=t.sent,data=Object(l.a)(n),this.model=data.model,t.next=21;break;case 9:if(t.prev=9,t.t0=t.catch(1),!this.$polyglot.has("error_messages."+t.t0)){t.next=17;break}return t.next=14,this.alert(this.$polyglot.t("error_messages."+t.t0));case 14:this.$modal.hide("dialog"),t.next=20;break;case 17:return t.next=19,this.alert(t.t0);case 19:this.$modal.hide("dialog");case 20:console.error(t.t0);case 21:case"end":return t.stop()}}),t,this,[[1,9]])}))),function(){return d.apply(this,arguments)}),restartDevice:(m=Object(r.a)(regeneratorRuntime.mark((function t(){var e,n;return regeneratorRuntime.wrap((function(t){for(;;)switch(t.prev=t.next){case 0:return t.next=2,this.confirm(this.$polyglot.t("messages.confirm_restart"));case 2:if(e=t.sent,this.$modal.hide("dialog"),e){t.next=6;break}return t.abrupt("return");case 6:return n=this.$apiRouter.resolve({name:"cgi/cmd",query:{action:"sysmng",param:"restart"}}).href,t.prev=7,t.next=10,this.$axios.get(n);case 10:return 200===t.sent.status&&window.$nuxt.$nextTick((function(){window.$nuxt.$emit("x-deviceRestarted")})),t.next=14,this.alert(this.$polyglot.t("messages.alert_restart"));case 14:this.$modal.hide("dialog"),this.setStatus(!0),t.next=24;break;case 18:return t.prev=18,t.t0=t.catch(7),t.next=22,this.alert(t.t0);case 22:this.$modal.hide("dialog"),console.error(t.t0);case 24:case"end":return t.stop()}}),t,this,[[7,18]])}))),function(){return m.apply(this,arguments)})},Object(c.b)("device",["setStatus"])),created:function(){this.fetchData()}},x=(n(308),n(30)),component=Object(x.a)(f,(function(){var t=this,e=t.$createElement,n=t._self._c||e;return n("div",{staticClass:"home-page"},[n("header",{staticClass:"page-header"},["NEXXONE"==t.model?[t._m(0)]:"NEXX360"==t.model?[t._m(1)]:[t._m(2)]],2),t._v(" "),n("main",[n("nav",[n("ol",{staticClass:"menu"},[n("li",[n("nuxt-link",{staticClass:"block",attrs:{to:"/camera",unselectable:"on"}},[n("strong",[t._v(t._s(t.$polyglot.t("index.camera")))]),t._v("\n\t\t\t\t\t\t"+t._s(t.$polyglot.t("index.settings"))+"\n\t\t\t\t\t")])],1),t._v(" "),n("li",[n("nuxt-link",{staticClass:"block",attrs:{to:"/operation",unselectable:"on"}},[n("strong",[t._v(t._s(t.$polyglot.t("index.operation")))]),t._v("\n\t\t\t\t\t\t"+t._s(t.$polyglot.t("index.settings"))+"\n\t\t\t\t\t")])],1),t._v(" "),n("li",[n("nuxt-link",{staticClass:"block",attrs:{to:"/network",unselectable:"on"}},[n("strong",[t._v(t._s(t.$polyglot.t("index.network")))]),t._v("\n\t\t\t\t\t\t"+t._s(t.$polyglot.t("index.settings"))+"\n\t\t\t\t\t")])],1),t._v(" "),n("li",[n("nuxt-link",{staticClass:"block",attrs:{to:"/server",unselectable:"on"}},[n("strong",[t._v(t._s(t.$polyglot.t("index.server")))]),t._v("\n\t\t\t\t\t\t"+t._s(t.$polyglot.t("index.settings"))+"\n\t\t\t\t\t")])],1),t._v(" "),n("li",[n("nuxt-link",{staticClass:"block",attrs:{to:"/system",unselectable:"on"}},[n("strong",[t._v(t._s(t.$polyglot.t("index.system")))]),t._v("\n\t\t\t\t\t\t"+t._s(t.$polyglot.t("index.settings"))+"\n\t\t\t\t\t")])],1),t._v(" "),n("li",[n("nuxt-link",{staticClass:"block",attrs:{to:"/user",unselectable:"on"}},[n("strong",[t._v(t._s(t.$polyglot.t("index.user")))]),t._v("\n\t\t\t\t\t\t"+t._s(t.$polyglot.t("index.settings"))+"\n\t\t\t\t\t")])],1),t._v(" "),n("li",[n("button",{staticClass:"text-btn",attrs:{type:"button"},on:{click:t.restartDevice}},[t._v("\n\t\t\t\t\t\t"+t._s(t.$polyglot.t("index.restart"))+"\n\t\t\t\t\t\t"),n("img",{staticClass:"hover",attrs:{src:"/images/components/arrows/right-hover.svg",alt:"arrow right"}}),t._v(" "),n("img",{staticClass:"default",attrs:{src:"/images/components/arrows/right-default.svg",alt:"arrow right"}})])])])])])])}),[function(){var t=this.$createElement,e=this._self._c||t;return e("figure",{staticClass:"nexx360"},[e("img",{attrs:{src:"/images/pages/home/nexxone-small@2x.png",alt:"NEXXONE"}}),this._v(" "),e("figcaption",[e("img",{attrs:{src:"/images/pages/home/nexxone_85x14.png",alt:"NEXXONE"}})])])},function(){var t=this.$createElement,e=this._self._c||t;return e("figure",{staticClass:"nexx360"},[e("img",{attrs:{src:"/images/pages/home/product-b-2-b-small@2x.png",alt:"NEXX360"}}),this._v(" "),e("figcaption",[e("img",{attrs:{src:"/images/pages/home/group-25.svg",alt:"NEXX360"}})])])},function(){var t=this.$createElement,e=this._self._c||t;return e("figure",{staticClass:"fitt360-security"},[e("img",{attrs:{src:"/images/pages/home/product-b-2-b-small@2x.png",alt:"FITT360 Security"}}),this._v(" "),e("figcaption",[e("img",{attrs:{src:"/images/pages/home/group-25.svg",alt:"FITT360 Security"}})])])}],!1,null,null,null);e.default=component.exports}}]);