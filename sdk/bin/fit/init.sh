# Set the environment variables used by other scripts.
# This should be first script executed

.  ./env.sh

./bin/remote_debug_client.out ${REMOTE_DEBUG_ADDR} &

insmod ./kermod/syslink.ko TRACE=1 TRACEFAILURE=1 2> /dev/null
insmod ./kermod/cmemk.ko phys_start=${CMEM_ADDR_START} phys_end=${CMEM_ADDR_END} allowOverlap=1

./bin/linux_prcm_ipcam.out p

#pdflush is 1%
#echo 1 > /proc/sys/vm/dirty_background_ratio
#pdflush is 10s
#echo 1000 > /proc/sys/vm/dirty_expire_centisecs

# Disable ARP for Virtual IP address
echo 1 > /proc/sys/net/ipv4/conf/all/arp_ignore
echo 2 > /proc/sys/net/ipv4/conf/all/arp_announce

# free vfs cache
sysctl -w vm.vfs_cache_pressure=10000
sysctl -w vm.min_free_kbytes=8192

#
# Format of the system priority setting utility for setting bandwidth regulator
#
# ./bin/sys_pri.out --L3-bw-reg-set <L3-bw-reg-initiator-name> <L3-pressure-High> <L3-pressure-Low> <L3-Bandwidth> <L3-Watermark-cycles>
#
# "L3-bw-reg-initiator-name" can be HDVICP0 or HDVICP1 or HDVICP2 or other initiators
# "L3-pressure-High" can be 0 (low), 1 (medium), 3 (high)
# "L3-pressure-Low"  can be 0 (low), 1 (medium), 3 (high)
# "L3-Bandwidth" is in MB/s
# "L3-Watermark-cycles" is in bytes
#
# See also ./bin/sys_pri.out --help for more details
#
# IVA-HD BW requlator programing.
# TODO. THIS IS CURRENTLY HARDCODED.
# THIS SHOULD CALCULATED FROM EXPECTED IVA BANDWIDTH USAGE
#
# IVA-HD0, IVA-HD1, IVA-HD2
#
./bin/sys_pri.out --L3-bw-reg-set HDVICP0 0 0 900 2500 &
./bin/sys_pri.out --dmm-pri-set ISS 0 &
./bin/sys_pri.out --dmm-pri-set HDVICP0 1 &

echo -e "\n----- init done...! -----\n"
