bin_PROGRAMS = elfview

elfview_CXXFLAGS = \
  -I/c/wxWidgets-2.9.1/include \
  -I/c/wxWidgets-2.9.1/build-mingw/lib/wx/include/msw-unicode-static-2.9

elfview_LDFLAGS = \
  -L/c/wxWidgets-2.9.1/build-mingw/lib \
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
  -luuid


elfview_SOURCES = \
  App.cpp \
  Location.cpp \
  ViewRelocations.cpp \
  ElfFile.cpp \
  View.cpp \
  ViewSectionHeaders.cpp \
  FileManager.cpp \
  ViewDynamic.cpp \
  ViewStringTable.cpp \
  FlagManager.cpp \
  ViewElfHeader.cpp \
  ViewSymbolTable.cpp \
  FrameFlags.cpp \
  ViewHexDump.cpp \
  WindowMain.cpp \
  FrameMain.cpp \
  ViewManager.cpp \
  WindowNavigator.cpp \
  LinkTable.cpp \
  ViewProgramHeaders.cpp