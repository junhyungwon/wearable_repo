(window.webpackJsonp=window.webpackJsonp||[]).push([[5],{288:function(e,t,r){var content=r(293);"string"==typeof content&&(content=[[e.i,content,""]]),content.locals&&(e.exports=content.locals);(0,r(72).default)("e8eb5854",content,!0,{sourceMap:!1})},289:function(e,t,r){"use strict";r.d(t,"a",(function(){return n}));r(123);var o=r(24);function n(e){if("object"===Object(o.a)(e.data)){if(void 0!==e.data["return value"]){if(0==e.data["return value"]||1==e.data["return value"])return e.data;throw e.data["return value"]}return e.data}var t=e.data.match(/\-\d+/m);if(!t)throw"unknown";if(t.length<1)throw"unknown";throw t[0]}},290:function(e,t,r){"use strict";r(1);t.a={methods:{alert:function(e){var t=this;return new Promise((function(r,o){t.$modal.show("dialog",{text:e,buttons:[{title:t.$polyglot.t("dialog.ok"),handler:function(){r(!0)}}]})}))},confirm:function(e){var t=this;return new Promise((function(r,o){t.$modal.show("dialog",{text:e,buttons:[{title:t.$polyglot.t("dialog.cancel"),handler:function(){r(!1)}},{title:t.$polyglot.t("dialog.ok"),default:!0,handler:function(){r(!0)}}]})}))}}}},291:function(e,t,r){"use strict";r.d(t,"a",(function(){return n}));var o=r(24);function n(e,t){for(var r in e)void 0!==t[r]&&(Array.isArray(t[r])?e[r]=t[r].map((function(e){return e})):("object"===Object(o.a)(t[r])&&t[r]&&n(e[r],t[r]),e[r]=isNaN(t[r])?t[r]:parseInt(t[r],10)))}},292:function(e,t,r){"use strict";var o=r(288);r.n(o).a},293:function(e,t,r){(t=r(71)(!1)).push([e.i,"label[data-v-9e138a9e]{display:-webkit-box;display:flex;width:100%;-webkit-box-align:center;align-items:center}label input[type=checkbox][data-v-9e138a9e]{display:none}label img[data-v-9e138a9e]{display:block;flex-basis:24px;flex-shrink:0;-webkit-box-flex:0;flex-grow:0;width:24px;height:24px}label:hover img.unchecked[data-v-9e138a9e],label img.hover[data-v-9e138a9e]{display:none}label:hover img.hover[data-v-9e138a9e]{display:block}label span[data-v-9e138a9e]{-webkit-box-flex:1;flex-grow:1;padding-left:14px;font-size:14px;font-weight:400;letter-spacing:.26px;color:#fff;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}",""]),e.exports=t},294:function(e,t,r){"use strict";var o={props:["checked"],computed:{state:{get:function(){return!0===this.checked||!1!==this.checked&&(isNaN(this.checked)?!!this.checked:!!parseInt(this.checked,10))},set:function(e){this.$emit("updated",e)}}}},n=(r(292),r(30)),component=Object(n.a)(o,(function(){var e=this,t=e.$createElement,r=e._self._c||t;return r("label",[r("input",{directives:[{name:"model",rawName:"v-model",value:e.state,expression:"state"}],attrs:{type:"checkbox"},domProps:{checked:Array.isArray(e.state)?e._i(e.state,null)>-1:e.state},on:{change:function(t){var r=e.state,o=t.target,n=!!o.checked;if(Array.isArray(r)){var c=e._i(r,null);o.checked?c<0&&(e.state=r.concat([null])):c>-1&&(e.state=r.slice(0,c).concat(r.slice(c+1)))}else e.state=n}}}),e._v(" "),e.state?r("img",{staticClass:"checked",attrs:{src:"/images/components/input-checkbox/filled.svg",alt:"Checked"}}):[r("img",{staticClass:"unchecked",attrs:{src:"/images/components/input-checkbox/outline.svg",alt:"Unchecked"}}),e._v(" "),r("img",{staticClass:"hover",attrs:{src:"/images/components/input-checkbox/hover.svg",alt:"Active"}})],e._v(" "),e._t("default")],2)}),[],!1,null,"9e138a9e",null);t.a=component.exports},313:function(e,t,r){"use strict";r.r(t);r(57);var o,n,c,l=r(15),d=(r(1),r(289)),h=r(291),m=r(294),dialog=r(290),f={components:{"input-checkbox":m.a},mixins:[dialog.a],data:function(){return{form:{model:null,record:{pre_rec:null,auto_rec:null,audio_rec:null,rec_interval:null,rec_overwrite:null},misc:{display_datetime:null}},disableSubmit:!0}},watch:{"form.model":function(e){window.document.title="NEXX360"==e?"NEXX360":"NEXXONE"==e?"NEXXONE":"FITT360 Security"}},methods:{wait:function(){return new Promise((function(e){return setTimeout(e,1e3)}))},fetchData:(c=Object(l.a)(regeneratorRuntime.mark((function e(){var t,r,data;return regeneratorRuntime.wrap((function(e){for(;;)switch(e.prev=e.next){case 0:return t=this.$apiRouter.resolve({name:"cgi/config",query:{action:"search",param:"operation_config"}}).href,e.prev=1,e.next=4,this.$axios.get(t);case 4:return r=e.sent,data=Object(d.a)(r),Object(h.a)(this.form,data),e.next=9,this.wait();case 9:this.disableSubmit=!1,e.next=24;break;case 12:if(e.prev=12,e.t0=e.catch(1),!this.$polyglot.has("error_messages."+e.t0)){e.next=20;break}return e.next=17,this.alert(this.$polyglot.t("error_messages."+e.t0));case 17:this.$modal.hide("dialog"),e.next=23;break;case 20:return e.next=22,this.alert(e.t0);case 22:this.$modal.hide("dialog");case 23:console.error(e.t0);case 24:case"end":return e.stop()}}),e,this,[[1,12]])}))),function(){return c.apply(this,arguments)}),storeData:(n=Object(l.a)(regeneratorRuntime.mark((function e(){var t,form,r;return regeneratorRuntime.wrap((function(e){for(;;)switch(e.prev=e.next){case 0:return e.next=2,this.validation();case 2:if(e.sent){e.next=4;break}return e.abrupt("return");case 4:return this.disableSubmit=!0,t=this.$apiRouter.resolve({name:"cgi/operation"}).href,(form=new URLSearchParams).append("pre_rec",this.form.record.pre_rec?"1":"0"),form.append("auto_rec",this.form.record.auto_rec?"1":"0"),form.append("audio_rec",this.form.record.audio_rec?"1":"0"),form.append("rec_interval",this.form.record.rec_interval),form.append("rec_overwrite",this.form.record.rec_overwrite?"1":"0"),form.append("display_datetime",this.form.misc.display_datetime?"1":"0"),e.prev=13,e.next=16,this.$axios.post(t,form);case 16:return r=e.sent,Object(d.a)(r),e.next=20,this.alert(this.$polyglot.t("messages.saved"));case 20:this.$modal.hide("dialog"),this.$router.push("/"),e.next=36;break;case 24:if(e.prev=24,e.t0=e.catch(13),!this.$polyglot.has("error_messages."+e.t0)){e.next=32;break}return e.next=29,this.alert(this.$polyglot.t("error_messages."+e.t0));case 29:this.$modal.hide("dialog"),e.next=35;break;case 32:return e.next=34,this.alert(e.t0);case 34:this.$modal.hide("dialog");case 35:console.error(e.t0);case 36:return e.prev=36,this.disableSubmit=!1,e.finish(36);case 39:case"end":return e.stop()}}),e,this,[[13,24,36,39]])}))),function(){return n.apply(this,arguments)}),validation:(o=Object(l.a)(regeneratorRuntime.mark((function e(){return regeneratorRuntime.wrap((function(e){for(;;)switch(e.prev=e.next){case 0:return e.abrupt("return",!0);case 1:case"end":return e.stop()}}),e)}))),function(){return o.apply(this,arguments)})},created:function(){this.fetchData()}},v=r(30),component=Object(v.a)(f,(function(){var e=this,t=e.$createElement,r=e._self._c||t;return r("div",{staticClass:"operation-page"},[r("header",{staticClass:"page-header"},[r("h1",[e._v(e._s(e.$polyglot.t("operation.page_title")))])]),e._v(" "),r("main",{staticClass:"page-body"},[r("section",[r("h2",[e._v(e._s(e.$polyglot.t("operation.record.name")))]),e._v(" "),r("ol",{staticClass:"controls"},[r("li",[r("input-checkbox",{attrs:{checked:e.form.record.pre_rec},on:{updated:function(t){e.form.record.pre_rec=t}}},[r("span",[e._v(e._s(e.$polyglot.t("operation.record.fields.pre_rec")))])])],1),e._v(" "),r("li",[r("input-checkbox",{attrs:{checked:e.form.record.auto_rec},on:{updated:function(t){e.form.record.auto_rec=t}}},[r("span",[e._v(e._s(e.$polyglot.t("operation.record.fields.auto_rec")))])])],1),e._v(" "),r("li",[r("input-checkbox",{attrs:{checked:e.form.record.audio_rec},on:{updated:function(t){e.form.record.audio_rec=t}}},[r("span",[e._v(e._s(e.$polyglot.t("operation.record.fields.audio_rec")))])])],1),e._v(" "),r("li",[r("label",{attrs:{for:"recInterval"}},[e._v(e._s(e.$polyglot.t("operation.record.fields.rec_interval")))]),e._v(" "),r("select",{directives:[{name:"model",rawName:"v-model",value:e.form.record.rec_interval,expression:"form.record.rec_interval"}],staticClass:"custom-select small",attrs:{id:"recInterval"},on:{change:function(t){var r=Array.prototype.filter.call(t.target.options,(function(e){return e.selected})).map((function(e){return"_value"in e?e._value:e.value}));e.$set(e.form.record,"rec_interval",t.target.multiple?r:r[0])}}},[r("option",{domProps:{value:0}},[e._v(e._s(e.$polyglot.t("values.minute",{m:1})))]),e._v(" "),r("option",{domProps:{value:1}},[e._v(e._s(e.$polyglot.t("values.minute",{m:5})))])])]),e._v(" "),r("li",[r("input-checkbox",{attrs:{checked:e.form.record.rec_overwrite},on:{updated:function(t){e.form.record.rec_overwrite=t}}},[r("span",[e._v(e._s(e.$polyglot.t("operation.record.fields.rec_overwrite")))])])],1)])]),e._v(" "),r("section",[r("h2",[e._v(e._s(e.$polyglot.t("operation.misc.name")))]),e._v(" "),r("ol",{staticClass:"controls"},[r("li",[r("input-checkbox",{attrs:{checked:e.form.misc.display_datetime},on:{updated:function(t){e.form.misc.display_datetime=t}}},[r("span",[e._v(e._s(e.$polyglot.t("operation.misc.fields.display_datetime")))])])],1)])])]),e._v(" "),r("footer",{staticClass:"page-footer"},[r("div",{staticClass:"cancel-submit-buttons"},[r("nuxt-link",{staticClass:"cancel-btn",attrs:{to:"/"}},[e._v(e._s(e.$polyglot.t("buttons.cancel")))]),e._v(" "),r("button",{staticClass:"submit-btn",attrs:{type:"submit",disabled:e.disableSubmit},on:{click:function(t){return t.preventDefault(),e.storeData(t)}}},[e._v("\n\t\t\t\t"+e._s(e.$polyglot.t("buttons.submit"))+"\n\t\t\t")])],1)])])}),[],!1,null,null,null);t.default=component.exports}}]);