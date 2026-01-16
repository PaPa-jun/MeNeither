.ORIG x3000
;R0 stores v, R1 stores d
AND R0, R0, #0
ADD R0, R0, #3
AND R1, R1, #0    ;0 for +, 1 for -

;Load N into R4
LEA R3, #252    ;x3100
LDR R4, R3, #2
ADD R4, R4, #-1
BRz STORE
LD R5, B    ;4096
NOT R5, R5
ADD R5, R5, #1    ;-4096

CALCULATE
    ADD R0, R0, R0
    ADD R1, R1, #0
    BRp STRACTION
PLUS
    ADD R0, R0, #2
    BRnzp MODB
STRACTION
    ADD R0, R0, #-2
        
MODB
    ADD R6, R0, R5
    BRn CALCULATEDONE
    ADD R0, R0, R5
    BRnzp MODB

CALCULATEDONE
    ADD R4, R4, #-1
    BRz STORE
    
MODE
    AND R7, R0, #7
    BRz CHANGE
    
MODT
    AND R7, R7, #0
    ADD R7, R7, R0
MODTBACK
    ADD R6, R7, #-10
    BRnz MODTDONE
    ADD R7, R7, #-10
    BRnzp MODTBACK
    
MODTDONE
    ADD R7, R7, #-8
    BRnp CALCULATE

CHANGE
    NOT R1, R1
    ADD R1, R1, #2    ;R1 = 1 - R1
    BRnzp CALCULATE

STORE
    STR R0, R3, #3
    
STOP
    TRAP x25
B
    ADD R0, R0, R0
.END