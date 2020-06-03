.  ./env.sh
cd ./scripts/

./load_m3.sh
./osa_kermod_load.sh

./wait_cmd.sh s m3vpss
./wait_cmd.sh s m3video

cd -

# load module fb
insmod ./kermod/vpss.ko sbufaddr=${HDVPSS_SHARED_MEM}
insmod ./kermod/ti81xxfb.ko
insmod ./kermod/ti81xxhdmi.ko
#insmod ./kermod/rndis_host.ko
insmod ./kermod/ax88179_178a.ko

#
# enable range compression in HDMI 0..255 to 16..235.
# This is needed for consumer HDTVs
#
#./bin/mem_rdwr.out --wr 0x46c00524 2

echo -e "----- load done...! -----\n"
