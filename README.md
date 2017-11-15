# ASM Assembler

This can handle the following operations on byte-sized and word-sized registers: `MOV`, `ADD`, `ADC`, `SUB`, `SBB`, `INT`, `JMP`. 

# Compiling and Running

Compile using:
```bash
$ git clone https://github.com/harsh96/Assembler.git
$ cd Assembler/
$ gcc assembler.c
```

Run:
```bash
$ ./a.out
```

Sample input.txt:
```bash
MOV AH 2
MOV DX 17218
INT 33
MOV DL DH
INT 33
JMP M
L: ADD DL 5
INT 33
JMP N
M: MOV DL 65
INT 33
JMP L
N: MOV AH 76
INT 33
END
```

Sample machineCode.txt Generated:
```bash
10110100 2
10111010 17218
10001010 11010110
11101011 9
10110010 65
11101011 -15
10000000 11000010 5
11101011 9
10110100 76
```

Sample Output in STDOUT:
````bash
BCAF
```

# License

MIT
