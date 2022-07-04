#!/bin/bash

VERSION_TXT_FILE="fw_version.txt"

function fmenu()
{
 echo "=========================================="
 echo "           NEXX Device MODEL List         "
 echo "=========================================="
 echo "1. NEXX360 Basic                          "
 echo "2. NEXX360 Wireless                       "
 echo "3. NEXX ONE                               "
 echo "4. NEXXB                                  "
 echo "5. NEXXB_ONE                              "
 echo "6. NEXX360 Wireless Mux                   "
 echo "7. NEXX360 Basic for CCTV                 "
 echo "8. NEXX360 Wireless for CCTV              "
 echo "9. NEXX360 Basic for Military             "
 echo "=========================================="
}

function _parsing_fw_ver ()
{
 ver1=$(echo $fw_ver | cut -d '.' -f1)
 ver2=$(echo $fw_ver | cut -d '.' -f2)
 ver3=$(echo $fw_ver | cut -d '.' -f3)
 ver4=$(echo $fw_ver | cut -d '.' -f4)
 ver4=$(echo "$ver4" | tr '[:lower:]' '[:upper:]')
}

function _compile_ubifs()
{
 echo
 echo "------------------------------------------"
 echo      Compile "$fw_name" ubifs...
 echo "------------------------------------------"

 echo
 echo "Making UBIFS ..."
 make -s ubifs
 cd bin
 md5sum rfs_fit.ubifs > rfs_fit.ubifs.md5
}

# write to fw_version.txt
function _fw_version_write()
{
/bin/cat <<EOM >$VERSION_TXT_FILE
TYPE	$1
MODEL	$FW_PREFIX
MAJOR	$2
MINOR	$3
PATCH	$4
CTAG	$5
EOM

echo
echo Write to $VERSION_TXT_FILE: "$FW_PREFIX"_$2.$3.$4$5-$1
echo
}

if [ "$1" == "" ]
then
	echo
	echo "You have to input the <FIRMWARE VERSION> for build!!! (ex 1.90.00.T)"
	echo
	exit 0
else
	fw_ver=$1
	echo
	echo -n "Make the firmware "$fw_ver" Right? (y/n): "
	read yn

	if [ "$yn" != "y" ] && [ "$yn" != "Y" ]
	then
		echo
		echo "Exit make...."
		echo
		exit 0
	fi
fi

while true
do
	echo " "
	fmenu
	echo " "
	echo -n "Please select Model Name..(default 2): "
	read fnum
	case "$fnum" in
		"2" )
			echo " "
			echo "select NEXX360 Wireless "
			FW_PREFIX="NEXX360W"
			break;;
		"9" )
			echo " "
			echo "select NEXX360 Basic for Military "
			FW_PREFIX="NEXX360M"
			break;;
		"8" )
			echo " "
			echo "select NEXX360 Wireless for CCTV "
			FW_PREFIX="NEXX360W_CCTV"
			break;;
		"7" )
			echo " "
			echo "select NEXX360 Basic for CCTV "
			FW_PREFIX="NEXX360C"
			break;;
		"6" )
			echo " "
			echo "select NEXX360 Wireless MUX"
			FW_PREFIX="NEXX360W_MUX"
			break;;
		"5" )
			echo " "
			echo "select NEXXB_ONE"
			FW_PREFIX="NEXXB_ONE"
			break;;
		"4" )
			echo " "
			echo "select NEXXB"
			FW_PREFIX="NEXXB"
			break;;
		"3" )
			echo " "
			echo "select NEXX ONE "
			FW_PREFIX="NEXXONE"
			break;;
		"1" )
			echo " "
			echo "select NEXX360 Basic "
			FW_PREFIX="NEXX360B"
			break;;
		* )
			echo " "
			echo "select NEXX360 Wireless "
			FW_PREFIX="NEXX360W"
			break;;
	esac
done

# version parsing and naming...
_parsing_fw_ver
#ver_name="$ver1.$ver2.$ver3$ver4"
ver_name="$ver1.$ver2.$ver3"
fw_name=""$FW_PREFIX"_"$ver_name"_full.dat"

if [ "$FW_PREFIX" == "NEXX360B" ]
then
ver4="B"
elif [ "$FW_PREFIX" == "NEXX360M" ]
then
ver4="B"
else
ver4="N"
fi

# factory distribute package
factory_fw_name=""$FW_PREFIX"_"$ver_name"_full_F.dat"

echo
echo Make $"FW_PREFIX" firmware "$ver_name" ....
echo

# compile ubifs
_compile_ubifs

# normal distribute package
# update fw_version.txt
#cd bin
_fw_version_write "release" "$ver1" "$ver2" "$ver3" "$ver4"

# packaging binary
echo
echo "Packaging files..."
echo

tar cvf "$fw_name" boot.scr u-boot_fit.min.nand u-boot_fit.bin MLO fw_version.txt uImage_fit mcu_fitt.txt mcu_extb.txt mcu_cctv.txt rfs_fit.ubifs rfs_fit.ubifs.md5
mv "$fw_name" ../.

# factory distribute package
# update fw_version.txt
#cd bin
_fw_version_write "debug" "$ver1" "$ver2" "$ver3" "$ver4"

# packaging binary
echo
echo "Packaging files..."
echo

tar cvf "$factory_fw_name" boot.scr u-boot_fit.min.nand u-boot_fit.bin MLO fw_version.txt uImage_fit mcu_fitt.txt mcu_extb.txt mcu_cctv.txt rfs_fit.ubifs rfs_fit.ubifs.md5
mv "$factory_fw_name" ../.

# mass production package
echo
echo "Mass production package file"
echo

masspack_name=hw_test_binary_"$FW_PREFIX"

if [ ! -f "$masspack_name" ]; then
mkdir "$masspack_name"
fi

cp ./fit/bin/hw_diag.out ./$masspack_name

cp boot.scr u-boot_fit.min.nand u-boot_fit.bin MLO fw_version.txt uImage_fit mcu_fitt.txt mcu_extb.txt mcu_cctv.txt rfs_fit.ubifs ./$masspack_name
zip $masspack_name.zip -r $masspack_name
mv "$masspack_name.zip" ../.

echo
echo "Make "$fw_name" done ...."
echo
