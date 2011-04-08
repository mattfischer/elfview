#ifndef ELF32_H
#define ELF32_H

typedef unsigned int Elf32_Addr;
typedef unsigned short Elf32_Half;
typedef unsigned int Elf32_Off;
typedef signed int Elf32_Sword;
typedef unsigned int Elf32_Word;

#define EI_NIDENT	16

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

typedef struct {
	Elf32_Addr		r_offset;
	Elf32_Word		r_info;
} Elf32_Rel;

typedef struct {
	Elf32_Addr		r_offset;
	Elf32_Word		r_info;
	Elf32_Word		r_addend;
} Elf32_Rela;

#endif