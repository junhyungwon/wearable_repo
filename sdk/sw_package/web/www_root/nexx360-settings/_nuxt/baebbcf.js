(window.webpackJsonp=window.webpackJsonp||[]).push([[2],{302:function(e,t,o){"use strict";o.d(t,"a",(function(){return l}));o(101);var r=o(24);function l(e){if("object"===Object(r.a)(e.data)){if(void 0!==e.data["return value"]){if(0==e.data["return value"]||1==e.data["return value"])return e.data;throw e.data["return value"]}return e.data}var t=e.data.match(/\-\d+/m);if(!t)throw"unknown";if(t.length<1)throw"unknown";throw t[0]}},303:function(e,t,o){"use strict";o(1);t.a={methods:{alert:function(e){var t=this;return new Promise((function(o,r){t.$modal.show("dialog",{text:e,buttons:[{title:t.$polyglot.t("dialog.ok"),handler:function(){o(!0)}}]})}))},confirm:function(e){var t=this;return new Promise((function(o,r){t.$modal.show("dialog",{text:e,buttons:[{title:t.$polyglot.t("dialog.cancel"),handler:function(){o(!1)}},{title:t.$polyglot.t("dialog.ok"),default:!0,handler:function(){o(!0)}}]})}))}}}},304:function(e,t,o){"use strict";o.d(t,"a",(function(){return l}));var r=o(24);function l(e,t){for(var o in e)void 0!==t[o]&&(Array.isArray(t[o])?e[o]=t[o].map((function(e){return e})):("object"===Object(r.a)(t[o])&&t[o]&&l(e[o],t[o]),e[o]=isNaN(t[o])?t[o]:parseInt(t[o],10)))}},319:function(e,t){Object.defineProperty(this,"mylog",{get:function(){return console.log.bind(window.console,"["+Date.now()+"]","[DEBUG]")}})},338:function(e,t,o){"use strict";o.r(t);o(50);var r=o(10),l=(o(1),o(302)),n=o(304),dialog=o(303),m=(o(319),{mixins:[dialog.a],data:function(){return{form:{model:null,record:{fps:null,bps:null,gop:null,rc:null},stream:{resolution:null,fps:null,bps:null,gop:null,rc:null}},list_15fps:["1","2","3","4","5","6","7","8","9","10","11","12","13","14","15"],list_30fps:["1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30"],models_voip_supported:["NEXXONE","NEXXB","NEXXB_ONE"],models_30fps_supported:["NEXXONE","NEXXB_ONE"],models_15fps_supported:["NEXX360","NEXX360W","NEXXB","NEXX360B","NEXX360C","NEXX360M","NEXX360W_MUX","NEXX360W_CCTV"],models_fullHD_supported:["NEXX360","NEXX360W","NEXXB","NEXX360B","NEXX360C","NEXX360W_MUX","NEXX360W_CCTV"],models_128k_stm_supported:["NEXXONE","NEXXB","NEXXB_ONE","NEXX360M","NEXX360","NEXX360W","NEXX360B","NEXX360C","NEXX360W_MUX","NEXX360W_CCTV"],models_nexxs:["NEXXONE","NEXXB","NEXXB_ONE","NEXX360M","NEXX360","NEXX360W","NEXX360B","NEXX360C","NEXX360W_MUX","NEXX360W_CCTV"],models_fitt360_security:["FITT360 SECURITY","FITT360 Security"],disableSubmit:!0}},watch:{"form.model":function(e){"NEXXONE"===this.form.model?window.document.title="NEXX ONE":window.document.title=e},"form.record.fps":function(e){if(this.models_30fps_supported.indexOf(this.form.model)>-1)this.form.record.gop=e+1;else if(this.models_15fps_supported.indexOf(this.form.model)>-1)this.form.record.gop=e+1;else switch(e){case 0:this.form.record.gop=15;break;case 1:this.form.record.gop=10;break;case 2:this.form.record.gop=5}},"form.stream.fps":function(e){if(this.models_15fps_supported.indexOf(this.form.model)>-1)this.form.stream.gop=e+1;else if(this.models_30fps_supported.indexOf(this.form.model)>-1)this.form.stream.gop=e+1;else switch(e){case 0:this.form.stream.gop=15;break;case 1:this.form.stream.gop=10;break;case 2:this.form.stream.gop=5}}},methods:{wait:function(){return new Promise((function(e){return setTimeout(e,1e3)}))},fetchData:function(){var e=this;return Object(r.a)(regeneratorRuntime.mark((function t(){var o,r,data;return regeneratorRuntime.wrap((function(t){for(;;)switch(t.prev=t.next){case 0:return o=e.$apiRouter.resolve({name:"cgi/config",query:{action:"search",param:"camera_config"}}).href,t.prev=1,t.next=4,e.$axios.get(o);case 4:return r=t.sent,data=Object(l.a)(r),Object(n.a)(e.form,data),console.log("fetchData:model:"+e.model),console.log("fetchData:form.model:"+e.form.model),t.next=11,e.wait();case 11:e.disableSubmit=!1,t.next=26;break;case 14:if(t.prev=14,t.t0=t.catch(1),!e.$polyglot.has("error_messages."+t.t0)){t.next=22;break}return t.next=19,e.alert(e.$polyglot.t("error_messages."+t.t0));case 19:e.$modal.hide("dialog"),t.next=25;break;case 22:return t.next=24,e.alert(t.t0);case 24:e.$modal.hide("dialog");case 25:console.error(t.t0);case 26:case"end":return t.stop()}}),t,null,[[1,14]])})))()},storeData:function(){var e=this;return Object(r.a)(regeneratorRuntime.mark((function t(){var o,form,r;return regeneratorRuntime.wrap((function(t){for(;;)switch(t.prev=t.next){case 0:return e.disableSubmit=!0,o=e.$apiRouter.resolve({name:"cgi/camera"}).href,(form=new URLSearchParams).append("rec_fps",e.form.record.fps),form.append("rec_bps",e.form.record.bps),form.append("rec_gop",e.form.record.gop),form.append("rec_rc",e.form.record.rc),form.append("stm_resolution",e.form.stream.resolution),form.append("stm_fps",e.form.stream.fps),form.append("stm_bps",e.form.stream.bps),form.append("stm_gop",e.form.stream.gop),form.append("stm_rc",e.form.stream.rc),t.prev=12,t.next=15,e.$axios.post(o,form);case 15:return r=t.sent,Object(l.a)(r),t.next=19,e.alert(e.$polyglot.t("messages.saved"));case 19:e.$modal.hide("dialog"),e.$router.push("/"),t.next=35;break;case 23:if(t.prev=23,t.t0=t.catch(12),!e.$polyglot.has("error_messages."+t.t0)){t.next=31;break}return t.next=28,e.alert(e.$polyglot.t("error_messages."+t.t0));case 28:e.$modal.hide("dialog"),t.next=34;break;case 31:return t.next=33,e.alert(t.t0);case 33:e.$modal.hide("dialog");case 34:console.error(t.t0);case 35:return t.prev=35,e.disableSubmit=!1,t.finish(35);case 38:case"end":return t.stop()}}),t,null,[[12,23,35,38]])})))()}},created:function(){var e=this;return Object(r.a)(regeneratorRuntime.mark((function t(){return regeneratorRuntime.wrap((function(t){for(;;)switch(t.prev=t.next){case 0:e.fetchData();case 1:case"end":return t.stop()}}),t)})))()}}),c=o(33),component=Object(c.a)(m,(function(){var e=this,t=e.$createElement,o=e._self._c||t;return o("div",{staticClass:"camera-page"},[o("header",{staticClass:"page-header"},[o("h1",[e._v(e._s(e.$polyglot.t("camera.page_title")))])]),e._v(" "),o("main",{staticClass:"page-body"},[o("section",[o("h2",[e._v(e._s(e.$polyglot.t("camera.record.name")))]),e._v(" "),o("ol",{staticClass:"controls"},[o("li",[o("label",{attrs:{for:"recordingFramerate"}},[e._v(e._s(e.$polyglot.t("camera.record.fields.fps")))]),e._v(" "),o("select",{directives:[{name:"model",rawName:"v-model",value:e.form.record.fps,expression:"form.record.fps"}],staticClass:"custom-select medium",attrs:{id:"recordingFramerate"},on:{change:function(t){var o=Array.prototype.filter.call(t.target.options,(function(e){return e.selected})).map((function(e){return"_value"in e?e._value:e.value}));e.$set(e.form.record,"fps",t.target.multiple?o:o[0])}}},[e.models_15fps_supported.indexOf(this.form.model)>-1?e._l(e.list_15fps,(function(t,r){return o("option",{key:r,domProps:{value:r}},[e._v(e._s(t))])})):e.models_30fps_supported.indexOf(this.form.model)>-1?e._l(e.list_30fps,(function(t,r){return o("option",{key:r,domProps:{value:r}},[e._v(e._s(t))])})):[o("option",{domProps:{value:0}},[e._v(e._s(e.$polyglot.t("values.high")))]),e._v(" "),o("option",{domProps:{value:1}},[e._v(e._s(e.$polyglot.t("values.middle")))]),e._v(" "),o("option",{domProps:{value:2}},[e._v(e._s(e.$polyglot.t("values.low")))])]],2)]),e._v(" "),o("li",[o("label",{attrs:{for:"recordingBitrate"}},[e._v(e._s(e.$polyglot.t("camera.record.fields.bps")))]),e._v(" "),o("select",{directives:[{name:"model",rawName:"v-model",value:e.form.record.bps,expression:"form.record.bps"}],staticClass:"custom-select medium",attrs:{id:"recordingBitrate"},on:{change:function(t){var o=Array.prototype.filter.call(t.target.options,(function(e){return e.selected})).map((function(e){return"_value"in e?e._value:e.value}));e.$set(e.form.record,"bps",t.target.multiple?o:o[0])}}},[e.models_nexxs.indexOf(this.form.model)>-1?[o("option",{domProps:{value:0}},[e._v(e._s(e.$polyglot.t("values.bitrate02")))]),e._v(" "),o("option",{domProps:{value:1}},[e._v(e._s(e.$polyglot.t("values.bitrate03")))]),e._v(" "),o("option",{domProps:{value:2}},[e._v(e._s(e.$polyglot.t("values.bitrate04")))]),e._v(" "),o("option",{domProps:{value:3}},[e._v(e._s(e.$polyglot.t("values.bitrate05")))]),e._v(" "),o("option",{domProps:{value:4}},[e._v(e._s(e.$polyglot.t("values.bitrate06")))])]:[o("option",{domProps:{value:0}},[e._v(e._s(e.$polyglot.t("values.high")))]),e._v(" "),o("option",{domProps:{value:1}},[e._v(e._s(e.$polyglot.t("values.middle")))]),e._v(" "),o("option",{domProps:{value:2}},[e._v(e._s(e.$polyglot.t("values.low")))])]],2)]),e._v(" "),o("li",[o("label",{attrs:{for:"recordingRateControl"}},[e._v(e._s(e.$polyglot.t("camera.record.fields.rc")))]),e._v(" "),o("select",{directives:[{name:"model",rawName:"v-model",value:e.form.record.rc,expression:"form.record.rc"}],staticClass:"custom-select large",attrs:{id:"recordingRateControl"},on:{change:function(t){var o=Array.prototype.filter.call(t.target.options,(function(e){return e.selected})).map((function(e){return"_value"in e?e._value:e.value}));e.$set(e.form.record,"rc",t.target.multiple?o:o[0])}}},[o("option",{domProps:{value:0}},[e._v(e._s(e.$polyglot.t("values.vbr")))]),e._v(" "),o("option",{domProps:{value:1}},[e._v(e._s(e.$polyglot.t("values.cbr")))])])])])]),e._v(" "),o("section",[o("h2",[e._v(e._s(e.$polyglot.t("camera.stream.name")))]),e._v(" "),o("ol",{staticClass:"controls"},[o("li",[o("label",{attrs:{for:"streamingResolution"}},[e._v(e._s(e.$polyglot.t("camera.stream.fields.resolution")))]),e._v(" "),o("select",{directives:[{name:"model",rawName:"v-model",value:e.form.stream.resolution,expression:"form.stream.resolution"}],staticClass:"custom-select medium",attrs:{id:"streamingResolution"},on:{change:function(t){var o=Array.prototype.filter.call(t.target.options,(function(e){return e.selected})).map((function(e){return"_value"in e?e._value:e.value}));e.$set(e.form.stream,"resolution",t.target.multiple?o:o[0])}}},[e.models_fullHD_supported.indexOf(this.form.model)>-1?[o("option",{domProps:{value:0}},[e._v("1080p")])]:e._e(),e._v(" "),o("option",{domProps:{value:1}},[e._v("720p")]),e._v(" "),o("option",{domProps:{value:2}},[e._v("480p")])],2)]),e._v(" "),o("li",[o("label",{attrs:{for:"streamingFramerate"}},[e._v(e._s(e.$polyglot.t("camera.stream.fields.fps")))]),e._v(" "),o("select",{directives:[{name:"model",rawName:"v-model",value:e.form.stream.fps,expression:"form.stream.fps"}],staticClass:"custom-select medium",attrs:{id:"streamingFramerate"},on:{change:function(t){var o=Array.prototype.filter.call(t.target.options,(function(e){return e.selected})).map((function(e){return"_value"in e?e._value:e.value}));e.$set(e.form.stream,"fps",t.target.multiple?o:o[0])}}},[e.models_15fps_supported.indexOf(this.form.model)>-1?e._l(e.list_15fps,(function(t,r){return o("option",{key:r,domProps:{value:r}},[e._v(e._s(t))])})):e.models_30fps_supported.indexOf(this.form.model)>-1?e._l(e.list_30fps,(function(t,r){return o("option",{key:r,domProps:{value:r}},[e._v(e._s(t))])})):[o("option",{domProps:{value:0}},[e._v(e._s(e.$polyglot.t("values.high")))]),e._v(" "),o("option",{domProps:{value:1}},[e._v(e._s(e.$polyglot.t("values.middle")))]),e._v(" "),o("option",{domProps:{value:2}},[e._v(e._s(e.$polyglot.t("values.low")))])]],2)]),e._v(" "),o("li",[o("label",{attrs:{for:"streamingBitrate"}},[e._v(e._s(e.$polyglot.t("camera.stream.fields.bps")))]),e._v(" "),o("select",{directives:[{name:"model",rawName:"v-model",value:e.form.stream.bps,expression:"form.stream.bps"}],staticClass:"custom-select medium",attrs:{id:"streamingBitrate"},on:{change:function(t){var o=Array.prototype.filter.call(t.target.options,(function(e){return e.selected})).map((function(e){return"_value"in e?e._value:e.value}));e.$set(e.form.stream,"bps",t.target.multiple?o:o[0])}}},[e.models_nexxs.indexOf(this.form.model)>-1?[o("option",{domProps:{value:0}},[e._v(e._s(e.$polyglot.t("values.bitrate00")))]),e._v(" "),o("option",{domProps:{value:1}},[e._v(e._s(e.$polyglot.t("values.bitrate01")))]),e._v(" "),o("option",{domProps:{value:2}},[e._v(e._s(e.$polyglot.t("values.bitrate02")))]),e._v(" "),o("option",{domProps:{value:3}},[e._v(e._s(e.$polyglot.t("values.bitrate03")))]),e._v(" "),o("option",{domProps:{value:4}},[e._v(e._s(e.$polyglot.t("values.bitrate04")))]),e._v(" "),o("option",{domProps:{value:5}},[e._v(e._s(e.$polyglot.t("values.bitrate05")))]),e._v(" "),o("option",{domProps:{value:6}},[e._v(e._s(e.$polyglot.t("values.bitrate06")))]),e._v(" "),o("option",{domProps:{value:7}},[e._v(e._s(e.$polyglot.t("values.bitrate07")))]),e._v(" "),o("option",{domProps:{value:8}},[e._v(e._s(e.$polyglot.t("values.bitrate08")))]),e._v(" "),o("option",{domProps:{value:9}},[e._v(e._s(e.$polyglot.t("values.bitrate09")))]),e._v(" "),o("option",{domProps:{value:10}},[e._v(e._s(e.$polyglot.t("values.bitrate10")))])]:[o("option",{domProps:{value:0}},[e._v(e._s(e.$polyglot.t("values.high")))]),e._v(" "),o("option",{domProps:{value:1}},[e._v(e._s(e.$polyglot.t("values.middle")))]),e._v(" "),o("option",{domProps:{value:2}},[e._v(e._s(e.$polyglot.t("values.low")))])]],2)]),e._v(" "),o("li",[o("label",{attrs:{for:"streamingRateControl"}},[e._v(e._s(e.$polyglot.t("camera.stream.fields.rc")))]),e._v(" "),o("select",{directives:[{name:"model",rawName:"v-model",value:e.form.stream.rc,expression:"form.stream.rc"}],staticClass:"custom-select large",attrs:{id:"streamingRateControl"},on:{change:function(t){var o=Array.prototype.filter.call(t.target.options,(function(e){return e.selected})).map((function(e){return"_value"in e?e._value:e.value}));e.$set(e.form.stream,"rc",t.target.multiple?o:o[0])}}},[o("option",{domProps:{value:0}},[e._v(e._s(e.$polyglot.t("values.vbr")))]),e._v(" "),o("option",{domProps:{value:1}},[e._v(e._s(e.$polyglot.t("values.cbr")))])])])])])]),e._v(" "),o("footer",{staticClass:"page-footer"},[o("div",{staticClass:"cancel-submit-buttons"},[o("nuxt-link",{staticClass:"cancel-btn",attrs:{to:"/"}},[e._v(e._s(e.$polyglot.t("buttons.cancel")))]),e._v(" "),o("button",{staticClass:"submit-btn",attrs:{type:"submit",disabled:e.disableSubmit},on:{click:function(t){return t.preventDefault(),e.storeData(t)}}},[e._v("\n\t\t\t\t"+e._s(e.$polyglot.t("buttons.submit"))+"\n\t\t\t")])],1)])])}),[],!1,null,null,null);t.default=component.exports}}]);