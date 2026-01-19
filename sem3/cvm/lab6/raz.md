1. Байты: 55                      push    ebp               ; сохранение
Opcode: 50+rw / 50 + rb
Инструкция: PUSH r32 
push = 50 = 01010
ebp = 101

55 = 01010 + 101 = 01010101



2. Байты: 89 e5                   mov     ebp, esp          ; установка нового базового указателя
Opcode: 89 /r 

Инструкция: MOV r/m32,r32

MOD-REG-R/M Byte = e5
e5 = 11100101
MOD = 11 - register mod
REG = 100 - esp
R/M = 101 - ebp

3. Байты: 83 3d 00 00 00 00 09    cmp     dword ptr [0x0], 0x9 ; сравнить i с 9
Opcode: 83 /7 ib 
/7 означает что REG должно быть = 101

Инструкция: CMP r/m32,imm8 

MOD-REG-R/M Byte = 3d
3d = 00111101
MOD = 00
REG = 111
RM = 101 - displacement mod постоянное смещение, добавляемое к базовому адресу при вычислении эффективного адреса операнда в x86-архитектуре. Displacement (смещение) — это константа в инструкции, используемая для вычисления адреса памяти.
09 - с чем сравнимаем


4. Байты: 6b 0d 00 00 00 00 11    imul    ecx, dword ptr [0x0], 0x11 ; ecx = i * 17
Opcode: 6B /r ib
Инструкция: IMUL r32,r/m32,imm8

MOD-REG-R/M Byte = 0d
0d = 00001101
MOD = 00 - 
REG = 001 - ecx
R/M = 101 displacement mod постоянное смещение, добавляемое к базовому адресу при вычислении эффективного адреса операнда в x86-архитектуре. Displacement (смещение) — это константа в инструкции, используемая для вычисления адреса памяти.
11 - на что умножаем 17




5. Байты: 83 e1 0e                and     ecx, 0xe           ; ecx = ecx & 14

Opcode: 83 /4 ib 
Инструкция: AND r/m32,imm8

MOD-REG-R/M Byte = e1
e1 = 11100001
MOD = 11
REG = 100 = 4 => and
R/M = 001 - ecx

0e = 14 на что делаем побитовое И



6. Байты: c1 e9 02                shr     ecx, 0x2           ; ecx = ecx / 4 (сдвиг вправо на два бита)
Opcode: C1 /5 ib
Инструкция: SHR r/m32,imm8 

MOD-REG-R/M Byte = e9
e9 = 11101001
MOD = 11
REG = 101
R/M = 001

02 = на 2 бита сдвиг


7. Байты: 83 c0 01                add     eax, 0x1           ; eax = i + 1
Opcode: 83 /0 ib
Инструкция: ADD r/m32,imm8

MOD-REG-R/M Byte = c0
c0 = 11000000
MOD = 11 - register mode
REG = 000 = 0 - add
R/M = 000 - eax

0x1 = 1 побавляем


8. Байты: 31 c0                   xor     eax, eax           ; eax = 0 (возвращаемое значение)
Opcode: 31 /r 
Инструкция: XOR r/m32,r32

MOD-REG-R/M Byte = c0
c0 = 11000000
MOD = 11 - register mode
REG = 000 - eax
R/M = 000

9. Байты: 5d                      pop     ebp                ; восстановление ebp
Opcode: 58 + rd 
Инструкция: POP r32
58 = 01011_000
ebp = 101

01011101 = 5d

10. Байты: c3                      ret                        ; возврат из функции
Opcode: C3 
Инструкция: RET