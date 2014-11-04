#!/bin/sh -x

#chown root:root $1
#chmod ug+s $1
#chmod o+rw $1
setcap cap_sys_rawio,cap_setuid+eip indicator-power-gadget
#chmod a+rw /dev/cpu/0/cpuid
#chmod a+rw /dev/cpu/0/msr

#sudo chown root ./power-gadget
#sudo chmod ug+s ./power-gadget
