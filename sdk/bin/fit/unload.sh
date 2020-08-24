
rmmod ./kermod/ti81xxhdmi.ko
rmmod ./kermod/vpss.ko
rmmod ./kermod/osa_kermod.ko

cd ./scripts/

#./send_cmd.sh x c6xdsp
#./wait_cmd.sh e c6xdsp

./send_cmd.sh x m3video
./wait_cmd.sh e m3video

./send_cmd.sh x m3vpss
./wait_cmd.sh e m3vpss

cd -

#./bin/fw_load.out shutdown DSP
./bin/fw_load.out shutdown VIDEO-M3
./bin/fw_load.out shutdown VPSS-M3

killall ./bin/remote_debug_client.out 2> /dev/null

echo -e "----- unload done...! -----\n"
