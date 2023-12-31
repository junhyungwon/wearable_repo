
SRCDIR += links_m3video/system \
	  links_m3video/iva_enc links_m3video/iva_dec links_m3video/codec_utils/src \
	  links_m3video/alg/common links_m3video/alg/motion_detection \
	  links_m3video/vstab

SRCS_CODECUTILS = alg_control.c alg_create.c alg_malloc.c iresman_hdvicp2.c \
                  utils_encdec.c iresman_tiledmemory.c hdvicp2_config.c

#SRCS_ENCLINK    = encLink_common.c encLink_h264.c encLink_tsk.c encLink_jpeg.c encLink_mpeg4.c
SRCS_ENCLINK    = encLink_common.c encLink_tsk.c encLink_h264.c encLink_jpeg.c

#SRCS_DECLINK    = decLink_common.c decLink_h264.c decLink_mpeg4.c decLink_jpeg.c decLink_tsk.c
SRCS_DECLINK    = decLink_common.c decLink_tsk.c decLink_h264.c decLink_jpeg.c

SRCS_SYSTEMVID  = system_m3video.c systemLink_tsk_m3video.c system_m3video_edma.c

SRCS_MD         = ti_motiondetect.c

SRCS_VSTAB		= vstabLink_tsk.c vstabLink_alg.c

SRCS_m3video +=  $(SRCS_SYSTEMVID) $(SRCS_CODECUTILS) $(SRCS_ENCLINK) $(SRCS_DECLINK) $(SRCS_MD) $(SRCS_VSTAB)

SRCS_m3video += utils_tiler.c
