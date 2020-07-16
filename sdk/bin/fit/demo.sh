#!/bin/sh

test -f ./load.sh || exit 0
test -f ./init.sh || exit 0

./init.sh
./load.sh

if [ -e /mmc/hw_test.out ]; then
	echo "Running Hardware Test!!"
	chmod +x /mmc/hw_test.out
	/mmc/hw_test.out
fi

if [ -x ./bin/app_fitt.out ]; then
	./bin/app_fitt.out &
	./bin/netdev.out &
fi

# For get disk free space
/bin/df /mmc > /dev/null
