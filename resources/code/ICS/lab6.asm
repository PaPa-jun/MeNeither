; Unfortunately we have not YET installed Windows or Linux on the LC-3,
; so we are going to have to write some operating system code to enable
; keyboard interrupts. The OS code does three things:
;
;    (1) Initializes the interrupt vector table with the starting
;        address of the interrupt service routine. The keyboard
;        interrupt vector is x80 and the interrupt vector table begins
;        at memory location x0100. The keyboard interrupt service routine
;        begins at x1000. Therefore, we must initialize memory location
;        x0180 with the value x1000.
;    (2) Sets bit 14 of the KBSR to enable interrupts.
;    (3) Pushes a PSR and PC to the system stack so that it can jump
;        to the user program at x3000 using an RTI instruction.

            .ORIG   x800
            ; (1) Initialize interrupt vector table.
            LD      R0, VEC
            LD      R1, ISR
            STR     R1, R0, #0
    
            ; (2) Set bit 14 of KBSR.
            LDI     R0, KBSR
            LD      R1, MASK
            NOT     R1, R1
            AND     R0, R0, R1
            NOT     R1, R1
            ADD     R0, R0, R1
            STI     R0, KBSR
    
            ; (3) Set up system stack to enter user space.
            LD      R0, PSR
            ADD     R6, R6, #-1
            STR     R0, R6, #0
            LD      R0, PC
            ADD     R6, R6, #-1
            STR     R0, R6, #0
            ; Enter user space.
            RTI
        
VEC         .FILL   x0180
ISR         .FILL   x1000
KBSR        .FILL   xFE00
MASK        .FILL   x4000   ;0100 0000 0000 0000
PSR         .FILL   x8002   ;1000 0000 0000 0010
PC          .FILL   x3000
            .END


            .ORIG   x3000
            ; *** Begin user program code here ***
            
            LD      R6, USP             ;initialize USP
            
LOOP        LDI     R0, ADDR            ;Detect if value valid
            BRp     CALCULATE
            LEA     R0, ID              ;Print ID
            TRAP    x22
            JSR     DELAY               ;Delay
            BR      LOOP
            
CALCULATE   ADD     R4, R0, #0          ;If the value that detected is valid
            LD      R1, ASCII_0
            ADD     R0, R0, R1          ;Translate the ASCII to decimal number
            ADD     R1, R0, #-8         ;If the number is too large
            BRzp    BIG
            AND     R1, R1, #0          ;Init R1 for factorial function
            JSR     FACT
            BR      OK
            
BIG         ADD     R0, R4, #0          ;Print the wornning message
            TRAP    x21
            LEA     R0, FAIL
            TRAP    x22
            BR      OVER

OK          ADD     R0, R4, #0          ;Print the result
            TRAP    x21
            LEA     R0, ANS
            TRAP    x22
            JSR     PRINT
            
OVER        HALT
            
FACT        ADD     R6, R6, #-1         ;Push R7
            STR     R7, R6, #0

            ADD     R0, R0, #0          ;If N == 0
            BRz     ZERO
            
            ADD     R6, R6, #-1         ;else Push N
            STR     R0, R6, #0
            ADD     R0, R0, #-1
            JSR     FACT                ;Calculate (n-1)!
            LDR     R0, R6, #0          ;Pop N
            ADD     R6, R6, #1
            
            AND     R3, R3, #0          ;Calculate n(n-1)!
AGAIN       ADD     R3, R1, R3
            ADD     R0, R0, #-1
            BRp     AGAIN
            ADD     R1, R3, #0
            BR      FINISH
            
ZERO        ADD     R1, R1, #1          ;0! = 1

FINISH      LDR     R7, R6, #0          ;Pop R7
            ADD     R6, R6, #1
            RET
            
DELAY       ST      R1, Save_R1         ;Delay #2500
            LD      R1, COUNT
REP         ADD     R1, R1, #-1
            BRp     REP
            LD      R1, Save_R1
            RET
            
PRINT       ADD     R6, R6, #-1         ;Print the result of N!
            STR     R7, R6, #0          ;Push R7
            
            AND     R4, R4, #0          ;Split the result bit by bit
            AND     R0, R0, #0          ;R0 for (N!/10); R4 for the bits of N!; R1 for (N!%10)
BACK        ADD     R3, R1, #-10
            BRn     PUSH
            ADD     R1, R1, #-10
            ADD     R0, R0, #1
            BR      BACK
            
PUSH        ADD     R6, R6, #-1         ;Push R1
            STR     R1, R6, #0
            ADD     R4, R4, #1          ;R4++
            ADD     R1, R0, #0
            BRz     FINE
            AND     R0, R0, #0          ;Clear R0
            BR      BACK
            
FINE        LD      R1, ASCIII_0        ;Prepare to print the result
FINEB       LDR     R0, R6, #0          ;Pop one bit of result
            ADD     R0, R0, R1          ;Translate it to ASCII
            TRAP    x21                 ;Print
            ADD     R6, R6, #1
            ADD     R4, R4, #-1
            BRp     FINEB
            
            LDR     R7, R6, #0          ;Pop R7
            ADD     R6, R6, #1
            RET
            
            

USP         .FILL   x5000
COUNT       .FILL   #2500
ADDR        .FILL   x3FFF
ASCII_0     .FILL   xFFD0
ASCIII_0    .FILL   x0030
Save_R1     .BLKW   1
ID          .STRINGZ "PB22051087 "
ANS         .STRINGZ "! = "
FAIL        .STRINGZ "! is too large for LC-3!"
            ; *** End user program code here ***
            .END


            .ORIG   x3FFF
            ; *** Begin factorial data here ***
FACT_N      .FILL   xFFFF
            ; *** End factorial data here ***
            .END


            .ORIG   x1000
            ; *** Begin interrupt service routine code here ***
            
            ST      R0, SaveR0          ;Save used register
            ST      R1, SaveR1
            
            LDI     R0, KBDR            ;Read KBDR
            LD      R1, ASC_0
            ADD     R1, R1, R0          ;Check if input is valid
            BRn     NODECIMAL
            BRz     ISDECIMAL
            
BIGGER      LD      R1, ASC_9
            ADD     R1, R1, R0
            BRnz    ISDECIMAL
            
NODECIMAL   ADD     R1, R0, #0          ;Input is not valid
            LD      R0, NEWLINE         ;Print the message
            TRAP    x21
            ADD     R0, R1, #0
            TRAP    x21
            LEA     R0, WRONG
            TRAP    x22
            BR      DONE

ISDECIMAL   LD      R1, N               ;Input is valid
            STR     R0, R1, #0          ;Store the valid value into x3FFF
            ADD     R1, R0, #0
            LD      R0, NEWLINE         ;Print the message
            TRAP    x21
            ADD     R0, R1, #0
            TRAP     x21
            LEA     R0, RIGHT
            TRAP    x22
            
DONE        LD      R0, SaveR0          ;Pop R7
            LD      R1, SaveR1
            RTI
            ;DONE
            
KBDR        .FILL   xFE02
ASC_0       .FILL   xFFD0
ASC_9       .FILL   xFFC7
NEWLINE     .FILL   x000A
N           .FILL   x3FFF
SaveR1      .BLKW   1
SaveR0      .BLKW   1
WRONG       .STRINGZ " is not a decimal digit.\n"
RIGHT       .STRINGZ " is a decimal digit.\n"
            ; *** End interrupt service routine code here ***
            .END