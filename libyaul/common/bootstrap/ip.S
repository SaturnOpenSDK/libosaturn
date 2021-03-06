/*
 * Copyright (c) 2012-2014
 * See LICENSE for details.
 *
 * Anders Montonen
 */

.section ".system_id"
.align 1

.ascii "SEGA SEGASATURN "       /* Hardware identifier */
.ascii "SEGA TP KAISHA-A"       /* Manufacturer ID */
.ascii "T-99901G  "             /* Product ID */
.ascii "$VERSION"               /* Version */
.ascii "$RELEASE_DATE"          /* Release date */
.ascii "CD-1/1  "               /* Device information */
.ascii "$AREAS"                 /* Target area symbol(s) */
.ascii "      "
.ascii "$PERIPHERALS"           /* Compatible peripheral(s) */
.ascii "$TITLE"                 /* Game name */
.ascii "                "
.ascii "                "
.ascii "                "
.ascii "                "
.ascii "                "
.ascii "                "
.long 0x00000000                /* Reserved */
.long 0x00000000                /* Reserved */
.long 0x00000000                /* Reserved */
.long 0x00000000                /* Reserved */
.long __ip_len                  /* IP size */
.long $MASTER_STACK_ADDR        /* Stack-M */
.long 0x00000000                /* Reserved */
.long $SLAVE_STACK_ADDR         /* Stack-S */
.LC0:
.long $1ST_READ_ADDR            /* 1st read address */
.long 0x00000000                /* 1st read size */
.long 0x00000000                /* Reserved */
.long 0x00000000                /* Reserved */
.incbin "sys_sec.bin"
.incbin "sys_arej.bin"
.incbin "sys_aret.bin"
.incbin "sys_areu.bin"
.incbin "sys_aree.bin"

.text
.align 1

        .incbin "sys_init.bin"

        mov.l .LC1, r1
        mov.l @r1, r1
        jmp @r1
        nop

.align 2

.LC1:
.long .LC0
