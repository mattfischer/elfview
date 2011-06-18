bin_PROGRAMS = elfview

include_flags = \
  -I/c/wxWidgets-2.9.1/include \
  -I/c/wxWidgets-2.9.1/build-mingw/lib/wx/include/msw-unicode-static-2.9 \
  -I/c/projects/binutils-2.21/include/

elfview_CXXFLAGS = $(include_flags)

elfview_LDFLAGS = \
  -L/c/wxWidgets-2.9.1/build-mingw/lib \
  -L/c/projects/binutils-2.21/build/opcodes \
  -mwindows \
  -static

elfview_LDADD = \
  -lwx_mswu_html-2.9 \
  -lwx_mswu_adv-2.9 \
  -lwx_mswu_core-2.9 \
  -lwx_baseu-2.9 \
  -lole32 \
  -loleaut32 \
  -lcomctl32 \
  -lwinspool \
  -luuid \
  -lopcodes \
  -lintl \
  -liberty \
  -liconv

elfview_SOURCES = \
  App.cpp \
  Disassembler.cpp \
  ElfFile.cpp \
  FileManager.cpp \
  FlagManager.cpp \
  FrameFlags.cpp \
  FrameMain.cpp \
  LinkTable.cpp \
  Location.cpp \
  View.cpp \
  ViewDynamic.cpp \  
  ViewElfHeader.cpp \
  ViewHexDump.cpp \
  ViewManager.cpp \
  ViewProgramHeaders.cpp \
  ViewRelocations.cpp \  
  ViewSectionHeaders.cpp \  
  ViewStringTable.cpp \
  ViewSymbolTable.cpp \
  WindowMain.cpp \
  WindowNavigator.cpp \
  Resource.rc
  
.rc.o:
	windres $(include_flags) -i $< -o $@