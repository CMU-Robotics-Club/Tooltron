#!/bin/bash

stty 9600 raw < /dev/ttyUSB0
#-echo -echok -icrnl -ixon -icanon -opost -onlcr 

xxd -r readhex > /dev/ttyUSB0

sleep 0.1

response=`head -c 5 /dev/ttyUSB0`
ok=`printf "\001"`

if [ ${response:0:1} = $ok ]; then
  echo -n ${response:1} | xxd
else
  # if it failed, it hangs waiting for more characters anyway, so this never
  # happens unless the serial port is missing :(
  echo Failed.
fi
