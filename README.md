# Zildj1an-OS

A very light Operating System from scratch.

<img src = "https://github.com/Zildj1an/Zildj1an_OS/blob/master/im_screen.png"/>

Booting from a USB:

<img src = "https://github.com/Zildj1an/Zildj1an_OS/blob/master/im2.jpg"/>

PXE (Network) Boot Server:
Tested on Arch Linux, with client and server directly connected via Ethernet.

1. Install ISC DHCP and TFTP servers (dhcp, tftp on Arch)

2. Write this in /etc/dhcpd.conf. Change 10.0.0.1 to whatever your ip is.

        allow booting;
        allow bootp;

        authoritative;

        option domain-name-servers 10.0.0.1;

        option architecture code 93 = unsigned integer 16;

        group {
            next-server 10.0.0.1;

            if option architecture = 00:07 {
                filename "/grub/x86_64-efi/core.efi";
            } else {
                filename "/grub/i386-pc/core.0";
            }

            subnet 10.0.0.0 netmask 255.255.255.0 {
                option routers 10.0.0.1;
                range 10.0.0.128 10.0.0.254;
            }
        }

3. Start DHCP server.

4. Start tftp server with root on /srv/tftp/

5. Run

        grub-mknetdir --net-directory=/srv/tftp

6. Run make

7. Copy the grub.cfg file from boot to /srv/tftp/boot/grub/grub.cfg

Any PC connected to the server should now be able to boot Zildj1an_OS


## Note

This project has to be stalled for a while do to my lack of free-time. But please, feel free to collaborate, it will be reviewed at some point! <3

## Contributors
* Carlos Bilbao.
* Pablo Villalobos.
