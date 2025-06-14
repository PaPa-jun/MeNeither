.ORIG x3000
Init
    LDI R0, N
    LD R5, N
    LD R1, BACK    ;store return address
    ADD R5, R5, #1    ;State pointer
    LD R6, STACK    ;STACK for storing Ns
    AND R2, R2, #0    ;0000_0000_0000_0000
    
FunctionRemove
    STR R7, R1, #0    ;Record the address
    ADD R1, R1, #1    ;Pop R3
    ADD R3, R0, #0    ;Check if R0 == 0
    BRz FunctionRemove_END
    ADD R3, R0, #-1    ;Check if R0 == 1
    BRz FunctionRemove_BASE
    
    STR R0, R6, #0
    ADD R6, R6, #1    ;Push R0
    ADD R0, R0, #-2
    JSR FunctionRemove    ;R(i-2)
    ADD R6, R6, #-1
    LDR R0, R6, #0    ;Pop R0
    
    AND R4, R4, #0
    ADD R4, R4, #1    ;MASK
    ADD R3, R0, #-1
    BRz #3
Remove_ith
    ADD R4, R4, R4
    ADD R3, R3, #-1
    BRp Remove_ith
    ADD R2, R2, R4
    JSR STORE_STATE
    
    STR R0, R6, #0
    ADD R6, R6, #1    ;Push R0
    ADD R0, R0, #-2
    JSR FunctionPut    ;P(i-2)
    ADD R6, R6, #-1
    LDR R0, R6, #0    ;Pop R0
    
    STR R0, R6, #0
    ADD R6, R6, #1    ;Push R0
    ADD R0, R0, #-1
    JSR FunctionRemove    ;R(i-1)
    ADD R6, R6, #-1
    LDR R0, R6, #0    ;Pop R0
    BR FunctionRemove_END

FunctionRemove_BASE
    ADD R2, R2, #1    ;N == 1
    STR R2, R5, #0
    ADD R5, R5, #1
    
FunctionRemove_END
    LD R3, BACK
    ADD R3, R3, #1
    NOT R3, R3
    ADD R3, R3, #1
    ADD R3, R3, R1
    BRz STOP
    ADD R1, R1, #-1
    LDR R7, R1, #0    ;Load return address
    RET
    

FunctionPut
    STR R7, R1, #0
    ADD R1, R1, #1
    ADD R3, R0, #0
    BRz FunctionPut_END
    ADD R3, R0, #-1
    BRz FunctionPut_BASE
    
    STR R0, R6, #0
    ADD R6, R6, #1    ;Push R0
    ADD R0, R0, #-1
    JSR FunctionPut    ;P(i-1)
    ADD R6, R6, #-1
    LDR R0, R6, #0    ;Pop R0
    
    STR R0, R6, #0
    ADD R6, R6, #1    ;Push R0
    ADD R0, R0, #-2
    JSR FunctionRemove    ;R(i-2)
    ADD R6, R6, #-1
    LDR R0, R6, #0    ;Pop R0
    
    AND R4, R4, #0
    ADD R4, R4, #1    ;MASK
    ADD R3, R0, #-1
    BRz #3
Put_ith
    ADD R4, R4, R4
    ADD R3, R3, #-1
    BRp Put_ith
    NOT R4, R4
    ADD R4, R4, #1
    ADD R2, R2, R4
    JSR STORE_STATE
    
    STR R0, R6, #0
    ADD R6, R6, #1    ;Push R0
    ADD R0, R0, #-2
    JSR FunctionPut    ;P(i-2)
    ADD R6, R6, #-1
    LDR R0, R6, #0    ;Pop R0
    BR FunctionPut_END
    
FunctionPut_BASE
    ADD R2, R2, #-1    ;N == 1
    STR R2, R5, #0
    ADD R5, R5, #1

FunctionPut_END
    ADD R1, R1, #-1
    LDR R7, R1, #0
    RET

STORE_STATE
    STR R2, R5, #0
    ADD R5, R5, #1
    RET
    
    
STOP
    TRAP x25
    
N       .FILL x3100
STACK   .FILL x4100
BACK    .FILL x5100

.END