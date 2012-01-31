/* ratchet.c - Unknown serial fix for Asus Transformer Prime.
 *
 * Copyright (C) 2012 androidroot.mobi
 *
 * This file is part of Ratchet.
 *
 * Razorclaw is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License v2 as published by the
 * Free Software Foundation.
 *
 * Razorclaw is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.

 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv, char **env)
{
    char ssn[13] = {0};
    FILE *outputFile;

    printf("\n[**] Ratchet - 'Unknown serial' fix for Asus Transformer Prime TF201.\n");
    printf("[**] (C) 2012 Androidroot.mobi. All rights reserved.\n\n");

    if(geteuid() != 0) {
        printf("[-] This utility needs to be run as root. Ensure you have a root shell (run mempodroid?) and try again.\n");
        return -1;
    }

    //Get the serial number as user input.
    printf("[?] Enter your serial number. This is located on the packaging, and stickers on your tablet: ");
    fgets(ssn,13,stdin);

    //Get the ChipID from sysfs.
    FILE *cardhu = fopen("/sys/devices/platform/cardhu_misc/cardhu_backup_chipid", "rb");
    char chipid[17] = {0};
    fgets(chipid, 17, cardhu);
    fclose(cardhu);

    printf("\n[+] Mounting configuration partition (P5).\n");
    mkdir("/Removable/p5",0777);
    mount("/dev/block/mmcblk0p5","/Removable/p5","vfat", 0, 0);

    printf("[+] Writing serial number.\n");
    // Write SSN from user input
    outputFile = fopen("/Removable/p5/SSN","w");
    fwrite(ssn, 1, 12, outputFile);
    fclose(outputFile);

    // Write UUID, converted from sysfs.
    outputFile = fopen("/Removable/p5/UUID","w");
    fwrite(chipid, 1, 16, outputFile);
    fclose(outputFile);

    umount2("/Removable/p5",0);

    printf("[=] Configuration data written, all done! Reboot for the changes to take effect.\n\n");

	return 0;
}

