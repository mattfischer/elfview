#include "FlagManager.h"

#include "Elf32.h"

std::vector<FlagManager::Value> FlagManager::GetValues(Set set)
{
	std::vector<Value> values;

	switch(set) {
		case SetElfType:
			values.push_back(Value("NONE", ET_NONE));
			values.push_back(Value("REL",  ET_REL));
			values.push_back(Value("EXEC", ET_EXEC));
			values.push_back(Value("DYN",  ET_DYN));
			values.push_back(Value("CORE", ET_CORE));
			break;

		case SetSectionType:
			values.push_back(Value("NULL",     SHT_NULL));
			values.push_back(Value("PROGBITS", SHT_PROGBITS));
			values.push_back(Value("SYMTAB",   SHT_SYMTAB));
			values.push_back(Value("STRTAB",   SHT_STRTAB));
			values.push_back(Value("RELA",     SHT_RELA));
			values.push_back(Value("HASH",     SHT_HASH));
			values.push_back(Value("DYNAMIC",  SHT_DYNAMIC));
			values.push_back(Value("NOTE",     SHT_NOTE));
			values.push_back(Value("NOBITS",   SHT_NOBITS));
			values.push_back(Value("REL",      SHT_REL));
			values.push_back(Value("SHLIB",    SHT_SHLIB));
			values.push_back(Value("DYNSYM",   SHT_DYNSYM));
			break;

		case SetSymbolBind:
			values.push_back(Value("LOCAL",  STB_LOCAL));
			values.push_back(Value("GLOBAL", STB_GLOBAL));
			values.push_back(Value("WEAK",   STB_WEAK));
			break;

		case SetSymbolType:
			values.push_back(Value("NOTYPE",  STT_NOTYPE));
			values.push_back(Value("OBJECT",  STT_OBJECT));
			values.push_back(Value("FUNC",    STT_FUNC));
			values.push_back(Value("SECTION", STT_SECTION));
			values.push_back(Value("FILE",    STT_FILE));
			break;

		case SetSegmentType:
			values.push_back(Value("NULL",    PT_NULL));
			values.push_back(Value("LOAD",    PT_LOAD));
			values.push_back(Value("DYNAMIC", PT_DYNAMIC));
			values.push_back(Value("INTERP",  PT_INTERP));
			values.push_back(Value("NOTE",    PT_NOTE));
			values.push_back(Value("SHLIB",   PT_SHLIB));
			values.push_back(Value("PHDR",    PT_PHDR));
			break;

		case SetDynamicType:
			values.push_back(Value("NULL",     DT_NULL));
			values.push_back(Value("NEEDED",   DT_NEEDED));
			values.push_back(Value("PLTRELSZ", DT_PLTRELSZ));
			values.push_back(Value("PLTGOT",   DT_PLTGOT));
			values.push_back(Value("HASH",     DT_HASH));
			values.push_back(Value("STRTAB",   DT_STRTAB));
			values.push_back(Value("SYMTAB",   DT_SYMTAB));
			values.push_back(Value("RELA",     DT_RELA));
			values.push_back(Value("RELASZ",   DT_RELASZ));
			values.push_back(Value("RELAENT",  DT_RELAENT));
			values.push_back(Value("STRSZ",    DT_STRSZ));
			values.push_back(Value("SYMENT",   DT_SYMENT));
			values.push_back(Value("INIT",     DT_INIT));
			values.push_back(Value("FINI",     DT_FINI));
			values.push_back(Value("SONAME",   DT_SONAME));
			values.push_back(Value("RPATH",    DT_RPATH));
			values.push_back(Value("SYMBOLIC", DT_SYMBOLIC));
			values.push_back(Value("REL",      DT_REL));
			values.push_back(Value("RELSZ",    DT_RELSZ));
			values.push_back(Value("RELENT",   DT_RELENT));
			values.push_back(Value("PLTREL",   DT_PLTREL));
			values.push_back(Value("DEBUG",    DT_DEBUG));
			values.push_back(Value("TEXTREL",  DT_TEXTREL));
			values.push_back(Value("JMPREL",   DT_JMPREL));
			break;
	}

	return values;
}

wxString FlagManager::GetDescription(Set set, int value)
{
	std::vector<Value> values = GetValues(set);

	for(int i=0; i<values.size(); i++) {
		if(values[i].value == value) {
			return values[i].name;
		}
	}

	return "(Unknown)";
}