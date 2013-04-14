all: tssrv rotationd bcattach 

bcattach: bcattach.c hsuart.h
	gcc -o bcattach bcattach.c

tssrv: tssrv2l tssrv2p tssrv3l tssrv3p

tssrv2l: tssrv2l.c hsuart.h
	gcc -o tssrv2l tssrv2l.c -lm

tssrv2p: tssrv2p.c hsuart.h
	gcc -o tssrv2p tssrv2p.c -lm

tssrv3l: tssrv3l.c hsuart.h
	gcc -o tssrv3l tssrv3l.c -lm

tssrv3p: tssrv3p.c hsuart.h
	gcc -o tssrv3p tssrv3p.c -lm

rotationd: rotationd.c
	gcc -o rotationd rotationd.c

clean:
	rm -f tssrv[23][lp] rotationd bcattach

install: tssrv rotationd bcattach
	cp tssrv[23][lp] /usr/sbin/
	cp rotationd /usr/bin/
	cp bcattach /usr/sbin/
	if [ ! -e /usr/sbin/hciattach_awesome ]; then
		echo "(!) You will need to install hciattach_awesome"
		echo "(!) for bluetooth to work!"
		echo "(!)"
		echo "(!) See the posts in the original 11.10 Ubuntu post"
		echo "(!) at XDA-Developers."
	fi
	if grep -c '\/mnt\/webos' /etc/fstab; then
		echo "(!) You will need to add the following line to"
		echo "(!) /etc/fstab: "
		echo "/dev/store/root /mnt/webos ext3 ro 0 0"
	fi
	cp initscripts/* /etc/init/
	cp bin/* /usr/bin/
	cp sbin/* /usr/sbin/

