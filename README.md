Ubuntu on the HP TouchPad - Supporting Software
===============================================

rotationd automatically rotates the screen.

bcattach initializes the Bluetooth hardware, and hciattach_awesome (NOT 
included) deals with it afterwards.

audiod.conf runs webOS' audiod binary from the webOS root partition, taking
care of different ABIs (armel vs armhf). No chroot is used. Make sure to copy
the /usr/share/alsa/ucm/ directory from webOS (and delete the old contents!)
You should then be able to configure sound with the "audiocfg" script. Note
that PulseAudio crashes oftentimes during long playbacks, so ALSA dmix is
recommended instead for long running streams.

ts_srv is split into four files because you currently need different binaries
for landscape/portrait orientation, and also depending on kernel version (2.6 
or 3.0.) Two scripts are provided in the "sbin" directory that launch the 
correct executable based on the kernel release, with minimal fuss.

The config-2.6.35-palm-tenderloin file should be copied (as ".config") to your 
kernel directory. It works best on my kernel. Another thing about my kernel:
if you're not testing Freedreno, revert the commit to the one called "ION."

For a 3.0 kernel, I recommend Freedreno/kernel-msm, branch "hp-tenderloin-3.0".
Use the configuration called "tenderloin_rob_defconfig."

The "initscripts" require Upstart. They won't work with systemd or any other 
system AFAIK.

The "bin" directory contains (mostly graphical) utilities to configure the
various features I added/ported/put on here. They all require Zenity, and are
written in bash shell. You might want to create desktop files for them and make
them launchable from the menu of your chosen X11 window manager/DE.

"make install" will warn you about if you are missing hciattach_awesome, or if
you need to put the webOS root in /etc/fstab.

Credits
-------

* jcsullins (for ts_srv)
* CyanogenMod project (for bcattach)
* BlueZ project (for hciattach)
* Ubuntu/Canonical (of course!)

If you contributed something and want on here, just send a private message to
castrwilliam at XDA.

