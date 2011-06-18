#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <wx/string.h>

#include <dis-asm.h>

class Disassembler
{
public:
	Disassembler();
	
	wxString Disassemble(void *data, int &bytes);
	
private:
	disassemble_info mDisassembleInfo;
	wxString mString;
	
	static void DisassembleFprintf(FILE *f, char *fmt, ...);
	static int DisassembleReadFunc(bfd_vma pc, bfd_byte *b, size_t size, disassemble_info *info);
};
#endif