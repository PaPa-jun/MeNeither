.ORIG x3000

Begin
    AND R6, R6, #0
    ADD R6, R6, #3
    LEA R0, START
    PUTS
    LD R1, JUDGE_W

First_Input
    TRAP x23    ;Input W
    ADD R2, R0, R1
    BRnp First_Input
    LEA R0, ENTER
    PUTS

AGAIN
    LD R4, LARG
    LD R1, JUDGE_Y
    LD R3, INP_PSW
Input_PSW
    TRAP x23
    ADD R4, R4, #-1
    BRz Compare_A
    ADD R2, R0, R1
    BRz Compare_A
    STR R0, R3, #0
    ADD R3, R3, #1
    BR Input_PSW
    
Compare_A
    LD R3, INP_PSW    ;Input password
    LEA R4, PSW       ;Correct password
    AND R5, R5, #0
    ADD R5, R5, #10
Compare_B
    LDR R1, R3, #0    ;Fetch one letter
    LDR R2, R4, #0
    NOT R1, R1        ;Compare
    ADD R1, R1, #1
    ADD R1, R2, R1
    BRnp Incorrect
    ADD R3, R3, #1
    ADD R4, R4, #1
    ADD R5, R5, #-1
    BRp Compare_B
    
If_Success
    LEA R0, CORRECT
    PUTS
    BRnzp STOP
    
Incorrect
    LEA R0, INC_A
    PUTS
    ADD R6, R6, #-1
    ADD R7, R6, #12
    ADD R7, R7, #12
    ADD R7, R7, #12
    ADD R7, R7, #12
    ADD R0, R7, #0
    OUT
    LEA R0, INC_B
    PUTS
    ADD R6, R6, #0
    BRp AGAIN
    
If_Failure
    LD R0, NEWLINE
    OUT
    LEA R0, FAIL
    PUTS
    LD R0, NEWLINE
    OUT
    BR Begin


STOP    HALT

START   .STRINGZ    "Welcome to the bank system! Type 'W' to withdraw some fund."
ENTER   .STRINGZ    "Please input your password:"
CORRECT .STRINGZ    "Success!"
INC_A   .STRINGZ    "Incorrect password!"
INC_B   .STRINGZ    "attempt(s) remain."
FAIL    .STRINGZ    "Fails."
PSW     .STRINGZ    "PB22051087"
JUDGE_W .FILL   xFFA9
JUDGE_Y .FILL   xFFA7
INP_PSW .FILL   x4000
NEWLINE .FILL   x000A
LARG    .FILL   #20


.END