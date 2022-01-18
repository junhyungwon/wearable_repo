#!/bin/sh

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
