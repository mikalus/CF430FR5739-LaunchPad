
        CAMELFORTH FOR THE MSP430 - VERSION 0.2 - 9 JANUARY 2009
        adopted for MSP430FR5739 by M.Kalus july & december 2011
                  (You will find me at www.forth-ev.de)
        ========================================================

This is an ALPHA TEST version of CF430FRexp, an ANSI Standard* Forth for the Texas Instruments MSP430 FRAM family of microprocessors. This means that I have tested the bulk of this code for correct functioning, but you may still discover bugs.  

                                 * * *

                         Download image with IAR 

Connect the experimenter board USB to your PC, then turn on PC and bring up Windows. In IAR klick "Project/Download/Download file" and navigate to CF430FRexp.a43 which is the intel hex image of our CF430FRexp project. Download it to the Experimenter Board. Press reset button on the Experimenter board. Connect Terminal to appropriate COM port (9600,8,N,1). Press <ENTER> to get "ok" prompt from CamelForth.

                                 * * *



                        Build project with IAR

As distributed, CF430FRexp will assemble to run on the TI MSP-EXP430FR5739 board, which uses the MSP430FR5739 processor. 

CF430FR is written to be assembled with the IAR Systems MSP430 Workbench "Kickstart", which can be downloaded from the TI web page:
<http://focus.ti.com/docs/toolsw/folders/print/iar-kickstart.html>
Note that the files expapp.s43, deps430.s43 and hilvl430.s43 are INCLUDEd from the core430.s43 source file; they are not linked separately.

The Workbench project should contain only the files:
core430fr5739.s43
infoA.s43
vecs430fr5739.s43
init430fr5739.s43

You will also need to use the provided linker control file, and _not_ the default provided by IAR. So doublecheck you use the correct linker file.

; MEMORY MAP of the MSP430FR5739 
; 0000-0FFF = peripherals (4 KB)
; 1000-17FF = bootstrap loader BSL0..3 (ROM 4x512 B) 
; 1800-187F = info B (FRAM 128 B) 
; 1880-18FF = info A (FRAM 128 B) 
; 1900-19FF = N/A (mirrored into info space) 
; 1A00-1A7F = device descriptor info (FRAM 128 B) 
; 1A80-1BFF = unused (385 B)
; 1C00-1FFF = RAM (1 KB)
; 2000-C1FF = unused (41472 B) 
; C200-FF7F = code memory (FRAM 15743 B) 
; FF80-FFFF = interrupt vectors (FRAM 127 B) 

The memory map is controlled by equates in the init430fr5739.s43 assembler source file, and statements in the lnk430fr5739.xcl linker control file.  There are also #defines in the CF430FRforth.h file that pertain specifically to FRAM memory usage. 

CF430FR uses a continuous Program/Data model.  New definitions are compiled into the Program (Instruction) space in FRAM, as indicated by the dictionary pointer HERE. New data structures (e.g., VARIABLEs) are allocated in FRAM too.

CF430FR features direct-to-FRAM compilation, it behaves just like compiling into RAM. You may place buffers or stacks as well in FRAM instead of RAM, though they will execut somewhat slower. 

                                 * * *
                                 
BUILDING CAMELFORTH USING THE IAR WORKBENCH

I assume that you have already installed the IAR MSP430 Workbench.

1. Create a new working directory.  For this example, "cf430fr".

2. Extract the files from cf430fr-02.zip into the new directory.

3. Launch the MSP430 Workbench.  In the Embedded Workbench Startup window, click "Create new project in current workspace." 
(If you allready have IAR running do this: In Menue klick "File/New/Workspace" and start a new project there.)

4. In the Create New Project window, select the "asm" template (double-click "asm" and then click the "asm" that appears, then click OK).

5. In the Save As window, you will need to specify a directory and a Project file name.  Navigate to the directory you created in step 1. For "File name:" you can type whatever you want -- for this example, "cf430fr-forth". Click Save.

6. In the Workspace window, click on the Project name (e.g. "cf430fr-forth") to select it.  Then on the menu bar click Project, Add Files. In the Add Files window select core430FR.s43, infoA.s43, init430FR5739.s43 and vecs430FR5739.s43, then click Open.  
Do NOT add deps430FR.s43, hilvl430FR.s43,expapps.s43, these are included from core430FR.

7. On the menu bar, click Project, Options.  
Select "General Options":
Select the "Target" tab there. 
Under "Device" select the MSP430FR5739. 
(Click the selection button to the right of the current device, then select MSP430FRxxx Family, then MSP430FR5739.) 
Still in "General Options":
Select the "Stack/Heap" tab there. 
Select the "Override default" box and " Read from linker command file". 
(If you don't, you get Warning [2] while making CF430FR.)
Click OK.

8. In the Options window, select "Linker", then the "Config" tab.  Under "Linker configuration file," select the "Override default" box.  Click the selection button to the right of the current file, browse to the project directory you created in step 1, select lnk430fr5739.xcl, and click Open. 
Still in the Options Window, select "Debugger", then the "Setup" tab. 
Under "Driver" seclect "FET Debugger". 
(If you select Simulator you can't download the project later on.)
Click OK.

9. In the Workspace window, right-click on "asm.s43", then click Remove. 
Confirm the removal.

10. On the menu bar, click Project, Make.  You will need to specify a Workspace file name.  This can be whatever you want ... you can use "cf430fr". 
Click Save. 
Then the project will be built. You should see number of errors and number of warnings both "0". 

11. If you have a MSP-EXP430FR5739 board connected with an USB cable, you should be able to now click Project, Debug to download CamelForth to the target board.


                                 * * *

There are TWO WAYS to write programs in CamelForth: 

1. If you have CamelForth running on your MSP430 board, you can download Forth code directly to CamelForth.  This lets you type new words from the keyboard, test them as they are defined, and re-define them to make changes.  Or you can edit an ASCII text file, and use a program such as TeraTerm to send this file over the serial port to your MSP430. It can take a few seconds to compile each line, so be sure to leave plenty of delay after the line.  Also be sure that no line exceeds 80 characters.

2. You can add your code to the assembler source files.  This requires you to convert your Forth code to assembler code.  To show how this is done, every high-level Forth word in the file is shown with its equivalent Forth code in a comment.  Be especially careful with control structures (IF..ELSE..THEN, BEGIN..UNTIL, DO..LOOP, and the like), and with the Forth word headers.  For this option it is recommended that you create a new .s43 assembler file, and INCLUDE it at the end of core430FR.s43. This is necessary to preserve the dictionary linking between your new definitions and the kernel definitions.  Reassemble core430FR.s43, and download to the MSP430 board, then test.  This is a much slower process, and is best saved for the final stage when you have a tested & debugged program that you want to put in FRAM.


                                 * * *


--------------------------- LICENSE TERMS ------------------------------
CamelForth for the Texas Instruments MSP430 
(c) 2009 Bradford J. Rodriguez. 

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version. 

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

Commercial inquiries should be directed to the author at 115 First St., #105, Collingwood, Ontario L9Y 4W3 Canada or via email to bj@camelforth.com 
------------------------------------------------------------------------
