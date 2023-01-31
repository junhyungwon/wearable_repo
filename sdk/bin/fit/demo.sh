#!/bin/sh
VAL=0
test -f ./load.sh || exit 0
test -f ./init.sh || exit 0

./init.sh
./load.sh

if [ -e /mmc/hw_diag.out ]; then
	echo "Running Hardware Test!!"
	chmod +x /mmc/hw_diag.out
	/mmc/hw_diag.out
fi

if [ -x ./bin/app_fitt.out ]; then
	./bin/app_fitt.out &
fi

if [ -x ./bin/process_check ]; then
	./bin/process_check &
fi

if [ -x ./bin/proxy ]; then
	until [ $VAL -ge 100 ]
	do
	    VAL=$(expr $VAL + 1)
#		echo $VAL
		sleep 1

		if [ -f /media/nand/cfg/_ssc_certificate.crt ]; then
			./bin/proxy -lhost :4444 --rhost :8300 -ltls -lcert /media/nand/cfg/_ssc_certificate.crt -lkey /media/nand/cfg/_ssc_private.key &
			break ;
		fi
	done
fi
