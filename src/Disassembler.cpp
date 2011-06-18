#include "Disassembler.h"

#include <stdarg.h>

Disassembler::Disassembler()
{
	init_disassemble_info(&mDisassembleInfo, NULL, (fprintf_ftype)DisassembleFprintf);
	mDisassembleInfo.arch = bfd_arch_arm;
	mDisassembleInfo.mach = bfd_mach_arm_unknown;
	mDisassembleInfo.endian = BFD_ENDIAN_LITTLE;
	mDisassembleInfo.read_memory_func = DisassembleReadFunc;
	mDisassembleInfo.stream = (FILE*)this;
}

wxString Disassembler::Disassemble(void *data, int &bytes)
{
	mString = "";
	bytes = print_insn_little_arm((bfd_vma)data, &mDisassembleInfo);
	
	return mString;
}

void Disassembler::DisassembleFprintf(FILE *f, char *fmt, ...)
{
	va_list list;
	char buffer[1000];
	va_start(list, fmt);
	vsprintf(buffer, fmt, list);
	
	Disassembler *dis = (Disassembler*)f;
	dis->mString += buffer;
}

int Disassembler::DisassembleReadFunc(bfd_vma pc, bfd_byte *b, size_t size, disassemble_info *info)
{
	memcpy(b, (void*)pc, size);
	return 0;
}
