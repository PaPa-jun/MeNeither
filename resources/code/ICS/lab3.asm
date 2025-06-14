        .ORIG x3000
    
Init    LD R1, S1_ADDR
        LD R2, S2_ADDR
    
COMPARE LDR R3, R1, #0
        LDR R4, R2, #0
        ADD R3, R3, #0
        BRz OVER
        
        NOT R5, R4
        ADD R5, R5, #1
        ADD R5, R3, R5
        BRnp OVER
        ADD R1, R1, #1
        ADD R2, R2, #1
        BRnzp COMPARE
        
OVER    NOT R5, R4
        ADD R5, R5, #1
        ADD R5, R3, R5
        STI R5, RESULT
        HALT

S1_ADDR .FILL x3100
S2_ADDR .FILL x3200
RESULT  .FILL x3300

        .END
    
        .ORIG x3100
S1      .STRINGZ "DsTAs"
        .END
    
        .ORIG x3200
S2      .STRINGZ "DstA"
        .END
