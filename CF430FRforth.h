; Register, Model, and Macro declarations - MSP430FR5739  ; M.Kalus Juli 2011
; Template was: CF430forth.h
; for TI MSP430F1611 by B. Rodriguez  3 Jan 09

; This file is used to build CF430FR using the IAR workbench.

; ----------------------------------------------------------------------
; CamelForth for the Texas Instruments MSP430 
; (c) 2009 Bradford J. Rodriguez.
; 
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.
;
; Commercial inquiries should be directed to the author at 
; 115 First St., #105, Collingwood, Ontario L9Y 4W3 Canada
; or via email to bj@camelforth.com

; ----------------------------------------------------------------------
; REVISION  HISTORY

;    dec 11 mk  - added memory map of MSP430FR5739.
;  (1 mar 09 bjr - added INFOSTART, changed FLASHSTART to be Main flash address
;  no flash in MSP430FR5739, we have FRAM now.) 
; 17 jan 09 bjr - changed IMMEDIATE flag from $00 to $FE to allow
;   use as a token field.

; ----------------------------------------------------------------------
; MEMORY MAP of the MSP430FR5739 
#define peri0  0000  ; 0000-0FFF = peripherals (4 KB)
#define BSL0   1000  ; 1000-17FF = bootstrap loader BSL0..3 (ROM 4x512 B) 
#define infoB0 1800  ; 1800-187F = info B (FRAM 128 B) 
#define infoA0 1880  ; 1880-18FF = info A (FRAM 128 B) 
                     ; 1900-19FF = N/A (mirrored into info space) 
#define ddi0   1A00  ; 1A00-1A7F = device descriptor info (FRAM 128 B) 
                     ; 1A80-1BFF = unused (385 B)
#define ram0   1C00  ; 1C00-1FFF = RAM (1 KB)
                     ; 2000-C1FF = unused (41472 B) 
#define fram0  C200  ; C200-FF7F = code memory (FRAM 15743 B) 
#define vecs0  FF80  ; FF80-FFFF = interrupt vectors (FRAM 127 B) 


; ----------------------------------------------------------------------
; FORTH MEMORY USAGE

; user area, stacks, tib, pad, holdarea have to fit in RAMSEG 
#define RAMSEG     1024

; INFO B,A (FRAM) unused so far.
#define INFOASEG    128
#define INFOBSEG    128
                       
; Reset service, MCU initialisation, Forth Dictionary
#define FRAMSEG   15743                        

; Vector area. Only RESET is used.                       
#define VECAREA     127

; ----------------------------------------------------------------------
; FORTH REGISTER USAGE

; Forth virtual machine
#define RSP SP
#define PSP R4
#define IP  R5
#define W   R6
#define TOS R7

; Loop parameters in registers
#define INDEX R8
#define LIMIT R9

; Scratch registers
#define X  R10
#define Y  R11
#define Q  R12
#define T  R13

; T.I. Integer Subroutines Definitions
#define IROP1   TOS
#define IROP2L  R10
#define IROP2M  R11
#define IRACL   R12
#define IRACM   R13
#define IRBT    W  


; ----------------------------------------------------------------------
; Routines of the Forth virtual machine

; INDIRECT-THREADED NEXT

NEXT    MACRO
        MOV @IP+,W      ; fetch word address into W
        MOV @W+,PC      ; fetch code address into PC, W=PFA
        ENDM

; BRANCH DESTINATION (RELATIVE BRANCH)
; For relative branch addresses, i.e., a branch is ADD @IP,IP

DEST    MACRO   label
        DW      label-$
        ENDM

; HEADER CONSTRUCTION MACROS

HEADER  MACRO   asmname,length,litname,action
        PUBLIC  asmname
        DW      link
        DB      0FFh       ; not immediate
link    SET     $
        DB      length
        DB      litname
        EVEN
        IF      'action'='DOCODE'
asmname: DW     $+2
        ELSE
asmname: DW      action
        ENDIF
        ENDM

HEADLESS  MACRO   asmname,action
        PUBLIC  asmname
        IF      'action'='DOCODE'
asmname: DW     $+2
        ELSE
asmname: DW      action
        ENDIF
        ENDM

IMMED   MACRO   asmname,length,litname,action
        PUBLIC  asmname
        DW      link
        DB      0FEh      ; immediate (LSB=0)
link    SET     $
        DB      length
        DB      litname
        EVEN
        IF      'action'='DOCODE'
asmname: DW     $+2
        ELSE
asmname: DW      action
        ENDIF
        ENDM

