(window.webpackJsonp=window.webpackJsonp||[]).push([[2],{282:function(t,e,r){var content=r(284);"string"==typeof content&&(content=[[t.i,content,""]]),content.locals&&(t.exports=content.locals);(0,r(93).default)("2e107364",content,!0,{sourceMap:!1})},283:function(t,e,r){"use strict";var o=r(282);r.n(o).a},284:function(t,e,r){(e=r(92)(!1)).push([t.i,".init-page footer.page-footer .cancel-submit-buttons .submit-btn{flex-basis:100%}",""]),t.exports=e},285:function(t,e,r){"use strict";r.r(e);r(69);var o=r(19),n=(r(120),r(26));function l(t){if("object"===Object(n.a)(t.data)){if(void 0!==t.data["return value"]){if(0==t.data["return value"])return t.data;throw t.data["return value"]}return t.data}var e=t.data.match(/\-\d+/m);if(!e)throw"unknown";if(e.length<1)throw"unknown";throw e[0]}r(1);var d,c,h,f=/^([0-9]|[a-z]|[A-Z]|_|\-|!|@|#|\$|%|\^){1,31}$/,m={mixins:[{methods:{alert:function(t){var e=this;return new Promise((function(r,o){e.$modal.show("dialog",{text:t,buttons:[{title:e.$polyglot.t("dialog.ok"),handler:function(){r(!0)}}]})}))},confirm:function(t){var e=this;return new Promise((function(r,o){e.$modal.show("dialog",{text:t,buttons:[{title:e.$polyglot.t("dialog.cancel"),handler:function(){r(!1)}},{title:e.$polyglot.t("dialog.ok"),default:!0,handler:function(){r(!0)}}]})}))}}}],data:function(){return{model:null,form:{password1:null,password2:null,authtype:0},disabled:!1}},watch:{model:function(t){window.document.title="NEXX360"==t?"NEXX360":"FITT360 Security"}},methods:{fetchData:(h=Object(o.a)(regeneratorRuntime.mark((function t(){var e,r,data;return regeneratorRuntime.wrap((function(t){for(;;)switch(t.prev=t.next){case 0:return e=this.$apiRouter.resolve({name:"cgi/config",query:{action:"search",param:"all_config"}}).href,t.prev=1,t.next=4,this.$axios.get(e);case 4:r=t.sent,data=l(r),this.model=data.model,t.next=21;break;case 9:if(t.prev=9,t.t0=t.catch(1),!this.$polyglot.has("error_messages."+t.t0)){t.next=17;break}return t.next=14,this.alert(this.$polyglot.t("error_messages."+t.t0));case 14:this.$modal.hide("dialog"),t.next=20;break;case 17:return t.next=19,this.alert(t.t0);case 19:this.$modal.hide("dialog");case 20:console.error(t.t0);case 21:case"end":return t.stop()}}),t,this,[[1,9]])}))),function(){return h.apply(this,arguments)}),storeData:(c=Object(o.a)(regeneratorRuntime.mark((function t(){var e,form;return regeneratorRuntime.wrap((function(t){for(;;)switch(t.prev=t.next){case 0:return t.next=2,this.validation();case 2:if(t.sent){t.next=4;break}return t.abrupt("return");case 4:if(!this.disabled){t.next=6;break}return t.abrupt("return");case 6:return this.disabled=!0,e=this.$apiRouter.resolve({name:"cgi/password"}).href,(form=new URLSearchParams).append("password1",this.form.password1),form.append("password2",this.form.password2),form.append("authtype",this.form.authtype),t.prev=12,t.next=15,this.$axios.post(e,form);case 15:return l(t.sent),t.next=19,this.alert(this.$polyglot.t("messages.saved"));case 19:this.$modal.hide("dialog"),window.location.reload(!0),t.next=35;break;case 23:if(t.prev=23,t.t0=t.catch(12),!this.$polyglot.has("error_messages."+t.t0)){t.next=31;break}return t.next=28,this.alert(this.$polyglot.t("error_messages."+t.t0));case 28:this.$modal.hide("dialog"),t.next=34;break;case 31:return t.next=33,this.alert(t.t0);case 33:this.$modal.hide("dialog");case 34:console.error(t.t0);case 35:return t.prev=35,this.disabled=!1,t.finish(35);case 38:case"end":return t.stop()}}),t,this,[[12,23,35,38]])}))),function(){return c.apply(this,arguments)}),validation:(d=Object(o.a)(regeneratorRuntime.mark((function t(){return regeneratorRuntime.wrap((function(t){for(;;)switch(t.prev=t.next){case 0:if(this.form.password1){t.next=6;break}return t.next=3,this.alert(this.$polyglot.t("init.messages.password1.null"));case 3:return this.$modal.hide("dialog"),this.$refs.password1.focus(),t.abrupt("return",!1);case 6:if("admin"!==this.form.password1){t.next=12;break}return t.next=9,this.alert(this.$polyglot.t("init.messages.password1.admin"));case 9:return this.$modal.hide("dialog"),this.$refs.password1.focus(),t.abrupt("return",!1);case 12:if(f.test(this.form.password1)){t.next=18;break}return t.next=15,this.alert(this.$polyglot.t("init.messages.password1.format"));case 15:return this.$modal.hide("dialog"),this.$refs.password1.focus(),t.abrupt("return",!1);case 18:if(this.form.password2===this.form.password1){t.next=24;break}return t.next=21,this.alert(this.$polyglot.t("init.messages.password2.confirmation"));case 21:return this.$modal.hide("dialog"),this.$refs.password2.focus(),t.abrupt("return",!1);case 24:if(f.test(this.form.password2)){t.next=30;break}return t.next=27,this.alert(this.$polyglot.t("init.messages.password2.format"));case 27:return this.$modal.hide("dialog"),this.$refs.password2.focus(),t.abrupt("return",!1);case 30:return t.abrupt("return",!0);case 31:case"end":return t.stop()}}),t,this)}))),function(){return d.apply(this,arguments)})},created:function(){},mounted:function(){this.$refs.password1.focus()}},w=(r(283),r(43)),component=Object(w.a)(m,(function(){var t=this,e=t.$createElement,r=t._self._c||e;return r("div",{staticClass:"init-page"},[r("header",{staticClass:"page-header"},[r("h1",[t._v(t._s(t.$polyglot.t("init.page_title")))])]),t._v(" "),r("main",{staticClass:"page-body"},[r("section",[r("h2",[t._v(t._s(t.$polyglot.t("init.name")))]),t._v(" "),r("ol",{staticClass:"controls"},[r("li",[r("label",{attrs:{for:"pswdField"}},[t._v(t._s(t.$polyglot.t("init.fields.password1")))]),t._v(" "),r("input",{ref:"password1",staticClass:"form-control",attrs:{type:"password",id:"pswdField",maxlength:"31",placeholder:"",disabled:t.disabled},domProps:{value:t.form.password1},on:{input:function(e){t.form.password1=e.target.value}}})]),t._v(" "),r("li",[r("label",{attrs:{for:"pswdConfirmationField"}},[t._v(t._s(t.$polyglot.t("init.fields.password2")))]),t._v(" "),r("input",{ref:"password2",staticClass:"form-control",attrs:{type:"password",id:"pswdConfirmationField",maxlength:"31",placeholder:"",disabled:t.disabled},domProps:{value:t.form.password2},on:{input:function(e){t.form.password2=e.target.value}}})])])])]),t._v(" "),r("footer",{staticClass:"page-footer"},[r("div",{staticClass:"cancel-submit-buttons"},[r("button",{staticClass:"submit-btn",attrs:{type:"submit",disabled:t.disabled},on:{click:function(e){return e.preventDefault(),t.storeData(e)}}},[t._v("\n\t\t\t\t"+t._s(t.$polyglot.t("buttons.submit"))+"\n\t\t\t")])])])])}),[],!1,null,null,null);e.default=component.exports}}]);