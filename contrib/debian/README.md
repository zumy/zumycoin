
Debian
====================
This directory contains files used to package zumyd/zumy-qt
for Debian-based Linux systems. If you compile zumyd/zumy-qt yourself, there are some useful files here.

## zumy: URI support ##


zumy-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install zumy-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your zumy-qt binary to `/usr/bin`
and the `../../share/pixmaps/zumy128.png` to `/usr/share/pixmaps`

zumy-qt.protocol (KDE)

