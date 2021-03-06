#ifndef ELF32_H
#define ELF32_H

typedef unsigned int Elf32_Addr;
typedef unsigned short Elf32_Half;
typedef unsigned int Elf32_Off;
typedef signed int Elf32_Sword;
typedef unsigned int Elf32_Word;

#define EI_NIDENT	16

enum {
	ET_NONE		= 0,
	ET_REL		= 1,
	ET_EXEC		= 2,
	ET_DYN		= 3,
	ET_CORE		= 4,
	ET_LOPROC	= 0xff00,
	ET_HIPROC	= 0xffff
};

typedef struct {
	unsigned char	e_nident[EI_NIDENT];
	Elf32_Half		e_type;
	Elf32_Half		e_machine;
	Elf32_Word		e_version;
	Elf32_Addr		e_entry;
	Elf32_Off		e_phoff;
	Elf32_Off		e_shoff;
	Elf32_Word		e_flags;
	Elf32_Half		e_ehsize;
	Elf32_Half		e_phentsize;
	Elf32_Half		e_phnum;
	Elf32_Half		e_shentsize;
	Elf32_Half		e_shnum;
	Elf32_Half		e_shstrndx;
} Elf32_Ehdr;

enum {
	SHN_UNDEF		= 0,
	SHN_LORESERVE	= 0xff00,
	SHN_LOPROC		= 0xff00,
	SHN_HIPROC		= 0xff1f,
	SHN_ABS			= 0xfff1,
	SHN_COMMON		= 0xfff2,
	SHN_HIRESERVE	= 0xffff
};

enum {
	SHT_NULL		= 0,
	SHT_PROGBITS	= 1,
	SHT_SYMTAB		= 2,
	SHT_STRTAB		= 3,
	SHT_RELA		= 4,
	SHT_HASH		= 5,
	SHT_DYNAMIC		= 6,
	SHT_NOTE		= 7,
	SHT_NOBITS		= 8,
	SHT_REL			= 9,
	SHT_SHLIB		= 10,
	SHT_DYNSYM		= 11,
	SHT_LOPROC		= 0x70000000,
	SHT_HIPROC		= 0x7fffffff,
	SHT_LOUSER		= 0x80000000,
	SHT_HIUSER		= 0xffffffff
};

typedef struct {
	Elf32_Word		sh_name;
	Elf32_Word		sh_type;
	Elf32_Word		sh_flags;
	Elf32_Addr		sh_addr;
	Elf32_Off		sh_offset;
	Elf32_Word		sh_size;
	Elf32_Word		sh_link;
	Elf32_Word		sh_info;
	Elf32_Word		sh_addralign;
	Elf32_Word		sh_entsize;
} Elf32_Shdr;

#define ELF32_ST_BIND(i) ((i)>>4)
#define ELF32_ST_TYPE(i) ((i)&0xf)
#define ELF32_ST_INFO(b,t) (((b)<<4) + ((t)&0xf))

enum {
	STB_LOCAL	= 0,
	STB_GLOBAL	= 1,
	STB_WEAK	= 2,
	STB_LOPROC	= 13,
	STB_HIPROC	= 15
};

enum {
	STT_NOTYPE	= 0,
	STT_OBJECT	= 1,
	STT_FUNC	= 2,
	STT_SECTION	= 3,
	STT_FILE	= 4,
	STT_LOPROC	= 13,
	STT_HIPROC	= 15
};

typedef struct {
	Elf32_Word		st_name;
	Elf32_Addr		st_value;
	Elf32_Word		st_size;
	unsigned char	st_info;
	unsigned char	st_other;
	Elf32_Half		st_shndx;
} Elf32_Sym;

#define ELF32_R_SYM(i) ((i)>>8)
#define ELF32_R_TYPE(i) ((unsigned char)(i))
#define ELF32_R_INFO(s,t) (((s)<<8) + (unsigned char)(t))

typedef struct {
	Elf32_Addr		r_offset;
	Elf32_Word		r_info;
} Elf32_Rel;

typedef struct {
	Elf32_Addr		r_offset;
	Elf32_Word		r_info;
	Elf32_Word		r_addend;
} Elf32_Rela;

enum {
	PT_NULL		= 0,
	PT_LOAD		= 1,
	PT_DYNAMIC	= 2,
	PT_INTERP	= 3,
	PT_NOTE		= 4,
	PT_SHLIB	= 5,
	PT_PHDR		= 6,
	PT_LOPROC	= 0x70000000,
	PT_HIPROC	= 0x7fffffff
};

typedef struct {
	Elf32_Word		p_type;
	Elf32_Off		p_offset;
	Elf32_Addr		p_vaddr;
	Elf32_Addr		p_paddr;
	Elf32_Word		p_filesz;
	Elf32_Word		p_memsz;
	Elf32_Word		p_flags;
	Elf32_Word		p_align;
} Elf32_Phdr;

enum {
	DT_NULL		= 0,
	DT_NEEDED	= 1,
	DT_PLTRELSZ	= 2,
	DT_PLTGOT	= 3,
	DT_HASH		= 4,
	DT_STRTAB	= 5,
	DT_SYMTAB	= 6,
	DT_RELA		= 7,
	DT_RELASZ	= 8,
	DT_RELAENT	= 9,
	DT_STRSZ	= 10,
	DT_SYMENT	= 11,
	DT_INIT		= 12,
	DT_FINI		= 13,
	DT_SONAME	= 14,
	DT_RPATH	= 15,
	DT_SYMBOLIC	= 16,
	DT_REL		= 17,
	DT_RELSZ	= 18,
	DT_RELENT	= 19,
	DT_PLTREL	= 20,
	DT_DEBUG	= 21,
	DT_TEXTREL	= 22,
	DT_JMPREL	= 23,
	DT_LOPROC	= 0x70000000,
	DT_HIPROC	= 0x7fffffff
};

typedef struct {
	Elf32_Sword	d_tag;
	union {
		Elf32_Word	d_val;
		Elf32_Addr	d_ptr;
	} d_un;
} Elf32_Dyn;

#endif