#as:
#readelf: -x.rodata -wL
#name: DWARF2 19
# The am33 avr cr16 crx ft32 mn10 msp430 nds32 pru rl78 and xtensa targets do not evaluate the subtraction of symbols at assembly time.
# The mep targets turns some view computations into complex relocations.
# The riscv targets do not support the subtraction of symbols.
# The tile targets require 8-byte instructions, but the test only simulates 4-byte instructions.
#notarget: am3*-* avr-* cr16-* crx-* ft32*-* mep-* mn10*-* msp430-* nds32*-* pru-* riscv*-* rl78-* tile*-* xtensa-*

Hex dump of section '\.rodata':
  0x00000000 01000102 *.*

Contents of the \.debug_line section:

CU: dwarf2-19\.c:
File name  *Line number  *Starting address  *View +Stmt
dwarf2-19\.c  *1  *0 +x
dwarf2-19\.c  *2  *0  *1 +x
dwarf2-19\.c  *4  *0x4 +x
dwarf2-19\.c  *5  *0x4  *1 +x
dwarf2-19\.c  *3  *0x4  *2 +x
dwarf2-19\.c  *3  *0x4  *3 +x