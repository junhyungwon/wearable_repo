(window.webpackJsonp=window.webpackJsonp||[]).push([[2],{291:function(t,e,o){"use strict";o.d(e,"a",(function(){return l}));o(123);var r=o(24);function l(t){if("object"===Object(r.a)(t.data)){if(void 0!==t.data["return value"]){if(0==t.data["return value"]||1==t.data["return value"])return t.data;throw t.data["return value"]}return t.data}var e=t.data.match(/\-\d+/m);if(!e)throw"unknown";if(e.length<1)throw"unknown";throw e[0]}},292:function(t,e,o){"use strict";o(1);e.a={methods:{alert:function(t){var e=this;return new Promise((function(o,r){e.$modal.show("dialog",{text:t,buttons:[{title:e.$polyglot.t("dialog.ok"),handler:function(){o(!0)}}]})}))},confirm:function(t){var e=this;return new Promise((function(o,r){e.$modal.show("dialog",{text:t,buttons:[{title:e.$polyglot.t("dialog.cancel"),handler:function(){o(!1)}},{title:e.$polyglot.t("dialog.ok"),default:!0,handler:function(){o(!0)}}]})}))}}}},293:function(t,e,o){"use strict";o.d(e,"a",(function(){return l}));var r=o(24);function l(t,e){for(var o in t)void 0!==e[o]&&(Array.isArray(e[o])?t[o]=e[o].map((function(t){return t})):("object"===Object(r.a)(e[o])&&e[o]&&l(t[o],e[o]),t[o]=isNaN(e[o])?e[o]:parseInt(e[o],10)))}},317:function(t,e,o){"use strict";o.r(e);o(51);var r,l,n,m=o(14),c=(o(1),o(291)),v=o(293),d={mixins:[o(292).a],data:function(){return{form:{model:null,record:{fps:null,bps:null,gop:null,rc:null},stream:{resolution:null,fps:null,bps:null,gop:null,rc:null}},fps_list:["1","2","3","4","5","6","7","8","9","10","11","12","13","14","15"],fps_list_one:["1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30"],disableSubmit:!0}},watch:{"form.model":function(t){"NEXXONE"===this.form.model?window.document.title="NEXX ONE":window.document.title=t},"form.record.fps":function(t){if("NEXXONE"===this.form.model||"NEXX360H"===this.form.model)this.form.record.gop=t+1;else if("NEXX360"==this.form.model||"NEXX360W"==this.form.model||"NEXX360B"==this.form.model)this.form.record.gop=t+1;else switch(t){case 0:this.form.record.gop=15;break;case 1:this.form.record.gop=10;break;case 2:this.form.record.gop=5}},"form.stream.fps":function(t){if("NEXXONE"===this.form.model||"NEXX360H"===this.form.model)this.form.stream.gop=t+1;else if("NEXX360"==this.form.model||"NEXX360W"==this.form.model||"NEXX360B"==this.form.model)this.form.stream.gop=t+1;else switch(t){case 0:this.form.stream.gop=15;break;case 1:this.form.stream.gop=10;break;case 2:this.form.stream.gop=5}}},methods:{wait:function(){return new Promise((function(t){return setTimeout(t,1e3)}))},fetchData:(n=Object(m.a)(regeneratorRuntime.mark((function t(){var e,o,data;return regeneratorRuntime.wrap((function(t){for(;;)switch(t.prev=t.next){case 0:return e=this.$apiRouter.resolve({name:"cgi/config",query:{action:"search",param:"camera_config"}}).href,t.prev=1,t.next=4,this.$axios.get(e);case 4:return o=t.sent,data=Object(c.a)(o),Object(v.a)(this.form,data),t.next=9,this.wait();case 9:this.disableSubmit=!1,t.next=24;break;case 12:if(t.prev=12,t.t0=t.catch(1),!this.$polyglot.has("error_messages."+t.t0)){t.next=20;break}return t.next=17,this.alert(this.$polyglot.t("error_messages."+t.t0));case 17:this.$modal.hide("dialog"),t.next=23;break;case 20:return t.next=22,this.alert(t.t0);case 22:this.$modal.hide("dialog");case 23:console.error(t.t0);case 24:case"end":return t.stop()}}),t,this,[[1,12]])}))),function(){return n.apply(this,arguments)}),storeData:(l=Object(m.a)(regeneratorRuntime.mark((function t(){var e,form,o;return regeneratorRuntime.wrap((function(t){for(;;)switch(t.prev=t.next){case 0:return this.disableSubmit=!0,e=this.$apiRouter.resolve({name:"cgi/camera"}).href,(form=new URLSearchParams).append("rec_fps",this.form.record.fps),form.append("rec_bps",this.form.record.bps),form.append("rec_gop",this.form.record.gop),form.append("rec_rc",this.form.record.rc),form.append("stm_resolution",this.form.stream.resolution),form.append("stm_fps",this.form.stream.fps),form.append("stm_bps",this.form.stream.bps),form.append("stm_gop",this.form.stream.gop),form.append("stm_rc",this.form.stream.rc),t.prev=12,t.next=15,this.$axios.post(e,form);case 15:return o=t.sent,Object(c.a)(o),t.next=19,this.alert(this.$polyglot.t("messages.saved"));case 19:this.$modal.hide("dialog"),this.$router.push("/"),t.next=35;break;case 23:if(t.prev=23,t.t0=t.catch(12),!this.$polyglot.has("error_messages."+t.t0)){t.next=31;break}return t.next=28,this.alert(this.$polyglot.t("error_messages."+t.t0));case 28:this.$modal.hide("dialog"),t.next=34;break;case 31:return t.next=33,this.alert(t.t0);case 33:this.$modal.hide("dialog");case 34:console.error(t.t0);case 35:return t.prev=35,this.disableSubmit=!1,t.finish(35);case 38:case"end":return t.stop()}}),t,this,[[12,23,35,38]])}))),function(){return l.apply(this,arguments)})},created:(r=Object(m.a)(regeneratorRuntime.mark((function t(){return regeneratorRuntime.wrap((function(t){for(;;)switch(t.prev=t.next){case 0:this.fetchData();case 1:case"end":return t.stop()}}),t,this)}))),function(){return r.apply(this,arguments)})},f=o(30),component=Object(f.a)(d,(function(){var t=this,e=t.$createElement,o=t._self._c||e;return o("div",{staticClass:"camera-page"},[o("header",{staticClass:"page-header"},[o("h1",[t._v(t._s(t.$polyglot.t("camera.page_title")))])]),t._v(" "),o("main",{staticClass:"page-body"},[o("section",[o("h2",[t._v(t._s(t.$polyglot.t("camera.record.name")))]),t._v(" "),o("ol",{staticClass:"controls"},[o("li",[o("label",{attrs:{for:"recordingFramerate"}},[t._v(t._s(t.$polyglot.t("camera.record.fields.fps")))]),t._v(" "),o("select",{directives:[{name:"model",rawName:"v-model",value:t.form.record.fps,expression:"form.record.fps"}],staticClass:"custom-select medium",attrs:{id:"recordingFramerate"},on:{change:function(e){var o=Array.prototype.filter.call(e.target.options,(function(t){return t.selected})).map((function(t){return"_value"in t?t._value:t.value}));t.$set(t.form.record,"fps",e.target.multiple?o:o[0])}}},["NEXX360"==t.form.model||"NEXX360W"==t.form.model||"NEXX360B"==t.form.model?t._l(t.fps_list,(function(e,r){return o("option",{key:r,domProps:{value:r}},[t._v(t._s(e))])})):"NEXXONE"==t.form.model||"NEXX360H"==t.form.model?t._l(t.fps_list_one,(function(e,r){return o("option",{key:r,domProps:{value:r}},[t._v(t._s(e))])})):[o("option",{domProps:{value:0}},[t._v(t._s(t.$polyglot.t("values.high")))]),t._v(" "),o("option",{domProps:{value:1}},[t._v(t._s(t.$polyglot.t("values.middle")))]),t._v(" "),o("option",{domProps:{value:2}},[t._v(t._s(t.$polyglot.t("values.low")))])]],2)]),t._v(" "),o("li",[o("label",{attrs:{for:"recordingBitrate"}},[t._v(t._s(t.$polyglot.t("camera.record.fields.bps")))]),t._v(" "),o("select",{directives:[{name:"model",rawName:"v-model",value:t.form.record.bps,expression:"form.record.bps"}],staticClass:"custom-select medium",attrs:{id:"recordingBitrate"},on:{change:function(e){var o=Array.prototype.filter.call(e.target.options,(function(t){return t.selected})).map((function(t){return"_value"in t?t._value:t.value}));t.$set(t.form.record,"bps",e.target.multiple?o:o[0])}}},["NEXX360"==t.form.model||"NEXX360W"==t.form.model||"NEXX360H"==t.form.model||"NEXX360B"==t.form.model||"NEXXONE"==t.form.model?[o("option",{domProps:{value:0}},[t._v(t._s(t.$polyglot.t("values.bitrate0")))]),t._v(" "),o("option",{domProps:{value:1}},[t._v(t._s(t.$polyglot.t("values.bitrate1")))]),t._v(" "),o("option",{domProps:{value:2}},[t._v(t._s(t.$polyglot.t("values.bitrate2")))]),t._v(" "),o("option",{domProps:{value:3}},[t._v(t._s(t.$polyglot.t("values.bitrate3")))]),t._v(" "),o("option",{domProps:{value:4}},[t._v(t._s(t.$polyglot.t("values.bitrate4")))])]:[o("option",{domProps:{value:0}},[t._v(t._s(t.$polyglot.t("values.high")))]),t._v(" "),o("option",{domProps:{value:1}},[t._v(t._s(t.$polyglot.t("values.middle")))]),t._v(" "),o("option",{domProps:{value:2}},[t._v(t._s(t.$polyglot.t("values.low")))])]],2)]),t._v(" "),o("li",[o("label",{attrs:{for:"recordingRateControl"}},[t._v(t._s(t.$polyglot.t("camera.record.fields.rc")))]),t._v(" "),o("select",{directives:[{name:"model",rawName:"v-model",value:t.form.record.rc,expression:"form.record.rc"}],staticClass:"custom-select large",attrs:{id:"recordingRateControl"},on:{change:function(e){var o=Array.prototype.filter.call(e.target.options,(function(t){return t.selected})).map((function(t){return"_value"in t?t._value:t.value}));t.$set(t.form.record,"rc",e.target.multiple?o:o[0])}}},[o("option",{domProps:{value:0}},[t._v(t._s(t.$polyglot.t("values.vbr")))]),t._v(" "),o("option",{domProps:{value:1}},[t._v(t._s(t.$polyglot.t("values.cbr")))])])])])]),t._v(" "),o("section",[o("h2",[t._v(t._s(t.$polyglot.t("camera.stream.name")))]),t._v(" "),o("ol",{staticClass:"controls"},[o("li",[o("label",{attrs:{for:"streamingResolution"}},[t._v(t._s(t.$polyglot.t("camera.stream.fields.resolution")))]),t._v(" "),o("select",{directives:[{name:"model",rawName:"v-model",value:t.form.stream.resolution,expression:"form.stream.resolution"}],staticClass:"custom-select medium",attrs:{id:"streamingResolution"},on:{change:function(e){var o=Array.prototype.filter.call(e.target.options,(function(t){return t.selected})).map((function(t){return"_value"in t?t._value:t.value}));t.$set(t.form.stream,"resolution",e.target.multiple?o:o[0])}}},["NEXX360"==t.form.model||"NEXX360W"==t.form.model||"NEXX360H"==t.form.model||"NEXX360B"==t.form.model?[o("option",{domProps:{value:0}},[t._v("1080p")])]:t._e(),t._v(" "),o("option",{domProps:{value:1}},[t._v("720p")]),t._v(" "),o("option",{domProps:{value:2}},[t._v("480p")])],2)]),t._v(" "),o("li",[o("label",{attrs:{for:"streamingFramerate"}},[t._v(t._s(t.$polyglot.t("camera.stream.fields.fps")))]),t._v(" "),o("select",{directives:[{name:"model",rawName:"v-model",value:t.form.stream.fps,expression:"form.stream.fps"}],staticClass:"custom-select medium",attrs:{id:"streamingFramerate"},on:{change:function(e){var o=Array.prototype.filter.call(e.target.options,(function(t){return t.selected})).map((function(t){return"_value"in t?t._value:t.value}));t.$set(t.form.stream,"fps",e.target.multiple?o:o[0])}}},["NEXX360"==t.form.model||"NEXX360W"==t.form.model||"NEXX360B"==t.form.model?t._l(t.fps_list,(function(e,r){return o("option",{key:r,domProps:{value:r}},[t._v(t._s(e))])})):"NEXXONE"==t.form.model||"NEXX360H"==t.form.model?t._l(t.fps_list_one,(function(e,r){return o("option",{key:r,domProps:{value:r}},[t._v(t._s(e))])})):[o("option",{domProps:{value:0}},[t._v(t._s(t.$polyglot.t("values.high")))]),t._v(" "),o("option",{domProps:{value:1}},[t._v(t._s(t.$polyglot.t("values.middle")))]),t._v(" "),o("option",{domProps:{value:2}},[t._v(t._s(t.$polyglot.t("values.low")))])]],2)]),t._v(" "),o("li",[o("label",{attrs:{for:"streamingBitrate"}},[t._v(t._s(t.$polyglot.t("camera.stream.fields.bps")))]),t._v(" "),o("select",{directives:[{name:"model",rawName:"v-model",value:t.form.stream.bps,expression:"form.stream.bps"}],staticClass:"custom-select medium",attrs:{id:"streamingBitrate"},on:{change:function(e){var o=Array.prototype.filter.call(e.target.options,(function(t){return t.selected})).map((function(t){return"_value"in t?t._value:t.value}));t.$set(t.form.stream,"bps",e.target.multiple?o:o[0])}}},["NEXX360"==t.form.model||"NEXX360W"==t.form.model||"NEXX360H"==t.form.model||"NEXX360B"==t.form.model||"NEXXONE"==t.form.model?[o("option",{domProps:{value:0}},[t._v(t._s(t.$polyglot.t("values.bitrate0")))]),t._v(" "),o("option",{domProps:{value:1}},[t._v(t._s(t.$polyglot.t("values.bitrate1")))]),t._v(" "),o("option",{domProps:{value:2}},[t._v(t._s(t.$polyglot.t("values.bitrate2")))]),t._v(" "),o("option",{domProps:{value:3}},[t._v(t._s(t.$polyglot.t("values.bitrate3")))]),t._v(" "),o("option",{domProps:{value:4}},[t._v(t._s(t.$polyglot.t("values.bitrate4")))]),t._v(" "),o("option",{domProps:{value:5}},[t._v(t._s(t.$polyglot.t("values.bitrate5")))]),t._v(" "),o("option",{domProps:{value:6}},[t._v(t._s(t.$polyglot.t("values.bitrate6")))]),t._v(" "),o("option",{domProps:{value:7}},[t._v(t._s(t.$polyglot.t("values.bitrate7")))]),t._v(" "),o("option",{domProps:{value:8}},[t._v(t._s(t.$polyglot.t("values.bitrate8")))])]:[o("option",{domProps:{value:0}},[t._v(t._s(t.$polyglot.t("values.high")))]),t._v(" "),o("option",{domProps:{value:1}},[t._v(t._s(t.$polyglot.t("values.middle")))]),t._v(" "),o("option",{domProps:{value:2}},[t._v(t._s(t.$polyglot.t("values.low")))])]],2)]),t._v(" "),o("li",[o("label",{attrs:{for:"streamingRateControl"}},[t._v(t._s(t.$polyglot.t("camera.stream.fields.rc")))]),t._v(" "),o("select",{directives:[{name:"model",rawName:"v-model",value:t.form.stream.rc,expression:"form.stream.rc"}],staticClass:"custom-select large",attrs:{id:"streamingRateControl"},on:{change:function(e){var o=Array.prototype.filter.call(e.target.options,(function(t){return t.selected})).map((function(t){return"_value"in t?t._value:t.value}));t.$set(t.form.stream,"rc",e.target.multiple?o:o[0])}}},[o("option",{domProps:{value:0}},[t._v(t._s(t.$polyglot.t("values.vbr")))]),t._v(" "),o("option",{domProps:{value:1}},[t._v(t._s(t.$polyglot.t("values.cbr")))])])])])])]),t._v(" "),o("footer",{staticClass:"page-footer"},[o("div",{staticClass:"cancel-submit-buttons"},[o("nuxt-link",{staticClass:"cancel-btn",attrs:{to:"/"}},[t._v(t._s(t.$polyglot.t("buttons.cancel")))]),t._v(" "),o("button",{staticClass:"submit-btn",attrs:{type:"submit",disabled:t.disableSubmit},on:{click:function(e){return e.preventDefault(),t.storeData(e)}}},[t._v("\n\t\t\t\t"+t._s(t.$polyglot.t("buttons.submit"))+"\n\t\t\t")])],1)])])}),[],!1,null,null,null);e.default=component.exports}}]);