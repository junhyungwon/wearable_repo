(window.webpackJsonp=window.webpackJsonp||[]).push([[3],{302:function(t,e,n){"use strict";n.d(e,"a",(function(){return r}));n(101);var o=n(24);function r(t){if("object"===Object(o.a)(t.data)){if(void 0!==t.data["return value"]){if(0==t.data["return value"]||1==t.data["return value"])return t.data;throw t.data["return value"]}return t.data}var e=t.data.match(/\-\d+/m);if(!e)throw"unknown";if(e.length<1)throw"unknown";throw e[0]}},303:function(t,e,n){"use strict";n(1);e.a={methods:{alert:function(t){var e=this;return new Promise((function(n,o){e.$modal.show("dialog",{text:t,buttons:[{title:e.$polyglot.t("dialog.ok"),handler:function(){n(!0)}}]})}))},confirm:function(t){var e=this;return new Promise((function(n,o){e.$modal.show("dialog",{text:t,buttons:[{title:e.$polyglot.t("dialog.cancel"),handler:function(){n(!1)}},{title:e.$polyglot.t("dialog.ok"),default:!0,handler:function(){n(!0)}}]})}))}}}},318:function(t,e,n){var content=n(334);"string"==typeof content&&(content=[[t.i,content,""]]),content.locals&&(t.exports=content.locals);(0,n(78).default)("2e107364",content,!0,{sourceMap:!1})},332:function(t,e){var n=null,o={key:"vue-session-key",flash_key:"vue-session-flash-key",setAll:function(t){n.setItem(o.key,JSON.stringify(t))},install:function(t,e){n=e&&"persist"in e&&e.persist?window.localStorage:window.sessionStorage,t.prototype.$session={flash:{parent:function(){return t.prototype.$session},get:function(t){var e=(this.parent().getAll()[o.flash_key]||{})[t];return this.remove(t),e},set:function(t,e){var n=this.parent().getAll(),r=n[o.flash_key]||{};r[t]=e,n[o.flash_key]=r,o.setAll(n)},remove:function(t){var e=this.parent().getAll(),n=e[o.flash_key]||{};delete n[t],e[o.flash_key]=n,o.setAll(e)}},getAll:function(){return JSON.parse(n.getItem(o.key))||{}},set:function(t,e){if("session-id"==t)return!1;var n=this.getAll();"session-id"in n||(this.start(),n=this.getAll()),n[t]=e,o.setAll(n)},get:function(t){return this.getAll()[t]},start:function(){var t=this.getAll();t["session-id"]="sess:"+Date.now(),o.setAll(t)},renew:function(t){var e=this.getAll();e["session-id"]="sess:"+t,o.setAll(e)},exists:function(){return"session-id"in this.getAll()},has:function(t){return t in this.getAll()},remove:function(t){var e=this.getAll();delete e[t],o.setAll(e)},clear:function(){var t=this.getAll();o.setAll({"session-id":t["session-id"]})},destroy:function(){o.setAll({})},id:function(){return this.get("session-id")}}}};t.exports=o},333:function(t,e,n){"use strict";n(318)},334:function(t,e,n){(e=n(77)(!1)).push([t.i,".home-page header.page-header{width:304px;margin:0 auto;padding-top:39px}.home-page header.page-header figure{position:relative;line-height:0;margin-bottom:0}.home-page header.page-header figure.nexx360 figcaption{position:absolute;left:50%;top:69px;transform:translateX(-50%)}.home-page header.page-header figure.nexx360 figcaption img{width:119px;height:15px}.home-page header.page-header figure.nexxb figcaption{position:absolute;left:50%;top:69px;transform:translateX(-50%)}.home-page header.page-header figure.nexxb figcaption img{width:62px;height:15px}.home-page header.page-header figure.nexxone figcaption{position:absolute;left:50%;top:64px;transform:translateX(-50%)}.home-page header.page-header figure.nexxone figcaption img{width:90px;height:14px}.home-page header.page-header figure.fitt360-security figcaption{position:absolute;left:50%;top:55px;transform:translateX(-50%)}.home-page header.page-header figure.fitt360-security figcaption img{width:100px;height:42px}.home-page header.page-header img{width:296px;height:200px}.home-page main{width:340px;margin:0 auto}.home-page main nav ol{list-style-type:none;margin:0;padding:0;display:flex;flex-wrap:wrap;justify-content:space-between}.home-page main nav ol li{flex-basis:166px;margin-bottom:8px}.home-page main nav ol li a.block{display:block;padding-top:29px;padding-bottom:28px;color:inherit;border-radius:4px;box-shadow:0 2px 14px 0 rgba(0,0,0,.08);background-color:#1f1f1f;font-size:16px;font-weight:300;line-height:1.19;letter-spacing:.57px;text-align:center;color:hsla(0,0%,100%,.87)}.home-page main nav ol li a.block strong{display:block;font-weight:700}.home-page main nav ol li a.restart{display:block;padding-top:5px;padding-bottom:5px;font-size:14px;font-weight:500;line-height:1.36;letter-spacing:.5px;text-align:center;color:hsla(0,0%,100%,.6)}.home-page main nav ol li a{text-decoration:none;text-transform:uppercase;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.home-page main nav ol li a:active,.home-page main nav ol li a:hover{color:#ffc600}.home-page main nav ol li:last-child{flex-basis:100%;margin-top:14px}.home-page main nav ol li:last-child button.text-btn{margin:0 auto;text-transform:uppercase}",""]),t.exports=e},342:function(t,e,n){"use strict";n.r(e);n(5),n(2),n(3),n(4),n(50);var o=n(10),r=(n(1),n(0)),l=n(302),c=n(49),dialog=n(303),m=n(8),h=n(332),d=n.n(h);function f(object,t){var e=Object.keys(object);if(Object.getOwnPropertySymbols){var n=Object.getOwnPropertySymbols(object);t&&(n=n.filter((function(t){return Object.getOwnPropertyDescriptor(object,t).enumerable}))),e.push.apply(e,n)}return e}m.default.use(d.a,{persist:!0});var x={layout:"main",mixins:[dialog.a],beforeCreate:function(){console.log("beforeCreate"),console.log(this.$session.get("user_timeNow")),this.$session.exists()||this.$router.push("/")},data:function(){return{model:null}},watch:{model:function(t){window.document.title="NEXXONE"===t||"NEXXONE_CCTV_SA"===t?"NEXX ONE":t}},methods:function(t){for(var i=1;i<arguments.length;i++){var source=null!=arguments[i]?arguments[i]:{};i%2?f(Object(source),!0).forEach((function(e){Object(r.a)(t,e,source[e])})):Object.getOwnPropertyDescriptors?Object.defineProperties(t,Object.getOwnPropertyDescriptors(source)):f(Object(source)).forEach((function(e){Object.defineProperty(t,e,Object.getOwnPropertyDescriptor(source,e))}))}return t}({wait:function(){return new Promise((function(t){return setTimeout(t,300)}))},login:function(){},logout:function(){this.$session.destroy(),this.$router.push("/")},fetchData:function(){var t=this;return Object(o.a)(regeneratorRuntime.mark((function e(){var n,o,r,data;return regeneratorRuntime.wrap((function(e){for(;;)switch(e.prev=e.next){case 0:return n=t.$apiRouter.resolve({name:"cgi/config",query:{action:"search",param:"camera_config"}}).href,e.prev=1,e.next=4,t.wait();case 4:return e.next=6,t.$axios.get(n);case 6:200===(o=e.sent).status&&"model"in o.data&&(t.$session.start(),r=new Date,t.$session.set("user_timeNow",r),console.log(r)),data=Object(l.a)(o),t.model=data.model,e.next=24;break;case 12:if(e.prev=12,e.t0=e.catch(1),!t.$polyglot.has("error_messages."+e.t0)){e.next=20;break}return e.next=17,t.alert(t.$polyglot.t("error_messages."+e.t0));case 17:t.$modal.hide("dialog"),e.next=23;break;case 20:return e.next=22,t.alert(e.t0);case 22:t.$modal.hide("dialog");case 23:console.error(e.t0);case 24:case"end":return e.stop()}}),e,null,[[1,12]])})))()},restartDevice:function(){var t=this;return Object(o.a)(regeneratorRuntime.mark((function e(){var n,o;return regeneratorRuntime.wrap((function(e){for(;;)switch(e.prev=e.next){case 0:return e.next=2,t.confirm(t.$polyglot.t("messages.confirm_restart"));case 2:if(n=e.sent,t.$modal.hide("dialog"),n){e.next=6;break}return e.abrupt("return");case 6:return o=t.$apiRouter.resolve({name:"cgi/cmd",query:{action:"sysmng",param:"restart"}}).href,e.prev=7,e.next=10,t.$axios.get(o);case 10:return 200===e.sent.status&&window.$nuxt.$nextTick((function(){window.$nuxt.$emit("x-deviceRestarted")})),e.next=14,t.alert(t.$polyglot.t("messages.alert_restart"));case 14:t.$modal.hide("dialog"),t.setStatus(!0),e.next=24;break;case 18:return e.prev=18,e.t0=e.catch(7),e.next=22,t.alert(e.t0);case 22:t.$modal.hide("dialog"),console.error(e.t0);case 24:case"end":return e.stop()}}),e,null,[[7,18]])})))()}},Object(c.b)("device",["setStatus"])),created:function(){console.log("created")},mounted:function(){this.fetchData(),console.log("mounted")},beforeUpdate:function(){console.log("beforeUpdate")},updated:function(){console.log("update")}},v=(n(333),n(33)),component=Object(v.a)(x,(function(){var t=this,e=t.$createElement,n=t._self._c||e;return n("div",{staticClass:"home-page"},[n("header",{staticClass:"page-header"},["NEXXONE"==t.model||"NEXXONE_CCTV_SA"==t.model?[t._m(0)]:"NEXXB"==t.model||"NEXXB_ONE"==t.model?[t._m(1)]:"NEXX360"==t.model||"NEXX360M"==t.model||"NEXX360W"==t.model||"NEXX360B"==t.model||"NEXX360C"==t.model||"NEXX360W_CCTV"==t.model||"NEXX360W_CCTV_SA"==t.model||"NEXX360W_MUX"==t.model?[t._m(2)]:"FITT360 Security"==t.model?[t._m(3)]:[t._m(4)]],2),t._v(" "),n("main",[n("nav",[n("ol",{staticClass:"menu"},[n("li",[n("nuxt-link",{staticClass:"block",attrs:{to:"/camera",unselectable:"on"}},[n("strong",[t._v(t._s(t.$polyglot.t("index.camera")))]),t._v("\n\t\t\t\t\t\t"+t._s(t.$polyglot.t("index.settings"))+"\n\t\t\t\t\t")])],1),t._v(" "),n("li",[n("nuxt-link",{staticClass:"block",attrs:{to:"/operation",unselectable:"on"}},[n("strong",[t._v(t._s(t.$polyglot.t("index.operation")))]),t._v("\n\t\t\t\t\t\t"+t._s(t.$polyglot.t("index.settings"))+"\n\t\t\t\t\t")])],1),t._v(" "),n("li",[n("nuxt-link",{staticClass:"block",attrs:{to:"/network",unselectable:"on"}},[n("strong",[t._v(t._s(t.$polyglot.t("index.network")))]),t._v("\n\t\t\t\t\t\t"+t._s(t.$polyglot.t("index.settings"))+"\n\t\t\t\t\t")])],1),t._v(" "),n("li",[n("nuxt-link",{staticClass:"block",attrs:{to:"/server",unselectable:"on"}},[n("strong",[t._v(t._s(t.$polyglot.t("index.server")))]),t._v("\n\t\t\t\t\t\t"+t._s(t.$polyglot.t("index.settings"))+"\n\t\t\t\t\t")])],1),t._v(" "),n("li",[n("nuxt-link",{staticClass:"block",attrs:{to:"/system",unselectable:"on"}},[n("strong",[t._v(t._s(t.$polyglot.t("index.system")))]),t._v("\n\t\t\t\t\t\t"+t._s(t.$polyglot.t("index.settings"))+"\n\t\t\t\t\t")])],1),t._v(" "),n("li",[n("nuxt-link",{staticClass:"block",attrs:{to:"/user",unselectable:"on"}},[n("strong",[t._v(t._s(t.$polyglot.t("index.user")))]),t._v("\n\t\t\t\t\t\t"+t._s(t.$polyglot.t("index.settings"))+"\n\t\t\t\t\t")])],1),t._v(" "),n("li",[n("button",{staticClass:"text-btn",attrs:{type:"button"},on:{click:t.restartDevice}},[t._v("\n\t\t\t\t\t\t"+t._s(t.$polyglot.t("index.restart"))+"\n\t\t\t\t\t\t"),n("img",{staticClass:"hover",attrs:{src:"/images/components/arrows/right-hover.svg",alt:"arrow right"}}),t._v(" "),n("img",{staticClass:"default",attrs:{src:"/images/components/arrows/right-default.svg",alt:"arrow right"}})])])])])])])}),[function(){var t=this.$createElement,e=this._self._c||t;return e("figure",{staticClass:"nexxone"},[e("img",{attrs:{src:"/images/pages/home/nexxone-small@2x.png",alt:"NEXX ONE"}}),this._v(" "),e("figcaption",[e("img",{attrs:{src:"/images/pages/home/nexxone-25.svg",alt:"NEXX ONE"}})])])},function(){var t=this.$createElement,e=this._self._c||t;return e("figure",{staticClass:"nexxb"},[e("img",{attrs:{src:"/images/pages/home/product-b-2-b-small@2x.png",alt:"NEXXB"}}),this._v(" "),e("figcaption",[e("img",{attrs:{src:"/images/pages/home/nexxb-62x15.svg",alt:"NEXXB"}})])])},function(){var t=this.$createElement,e=this._self._c||t;return e("figure",{staticClass:"nexx360"},[e("img",{attrs:{src:"/images/pages/home/product-b-2-b-small@2x.png",alt:"NEXX360"}}),this._v(" "),e("figcaption",[e("img",{attrs:{src:"/images/pages/home/nexx360-119x15.svg",alt:"NEXX360"}})])])},function(){var t=this.$createElement,e=this._self._c||t;return e("figure",{staticClass:"fitt360-security"},[e("img",{attrs:{src:"/images/pages/home/product-b-2-b-small@2x.png",alt:"FITT360 Security"}}),this._v(" "),e("figcaption",[e("img",{attrs:{src:"/images/pages/home/fitt-360-security.svg",alt:"FITT360 Security"}})])])},function(){var t=this.$createElement,e=this._self._c||t;return e("figure",{staticClass:"nexx360"},[e("img",{attrs:{src:"/images/pages/home/product-small@2x.png",alt:"NEXX"}})])}],!1,null,null,null);e.default=component.exports}}]);