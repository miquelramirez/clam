/* A kludge until mingw fixes the assembler. See CLAMVstPlugin.cxx */

.section .rodata
.def _embededUI
_embededUI:
	.incbin "interface.ui"
	.byte 0
	.scl 2
	.size . - _embededUI
.endef 
.def _embededNetwork
_embededNetwork:
	.incbin "wire.xml"
	.byte 0
	.scl 2
	.size . - _embededNetwork
.endef 



