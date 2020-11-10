#!/bin/bash

#FW_PREFIX="NEXX360B"
FW_PREFIX="NEXX360W"
#FW_PREFIX="NEXXONE"
UPDATE_MODE="release"
#UPDATE_MODE="debug"

VERSION_TXT_FILE="fw_version.txt"

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

# version parsing and naming...
_parsing_fw_ver
ver_name="$ver1.$ver2.$ver3$ver4" 
fw_name=""$FW_PREFIX"_"$ver_name"_full.dat"

echo
echo Make $"FW_PREFIX" firmware "$ver_name" ....
echo

# compile ubifs
_compile_ubifs

# update fw_version.txt
cd bin
_fw_version_write "$UPDATE_MODE" "$ver1" "$ver2" "$ver3" "$ver4"


# packaging binary
echo
echo "Packaging files..."
echo
tar cvf "$fw_name" boot.scr u-boot_fit.min.nand u-boot_fit.bin MLO fw_version.txt uImage_fit mcu_fitt.txt rfs_fit.ubifs rfs_fit.ubifs.md5
mv "$fw_name" ../.

echo
echo "Make "$fw_name" done ...."
echo



