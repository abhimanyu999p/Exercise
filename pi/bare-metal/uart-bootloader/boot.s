/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

;@ start-up code

.globl _start
_start:
    b reset

.space 0x200000-4,0

reset:
    mov sp,#0x10000000
    bl myMain
hang: b hang

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.globl GET8
GET8:
    ldrb r0,[r0]
    bx lr

.globl PUT8
PUT8:
    strb r1,[r0]
    bx lr

.globl JUMPTO
JUMPTO:
    bx r0

.globl DUMMY
DUMMY:
    bx lr
