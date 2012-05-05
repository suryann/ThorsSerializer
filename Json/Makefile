

THORSANVIL_ROOT			= $(realpath ../)

TARGET					= json.slib
EXTRA_HEADERS			= ParserShiftReduce.tab.hpp location.hh position.hh stack.hh
CLEAN_EXTRA				= location.hh position.hh stack.hh
NOCOVERAGE				= Lexer.lex ParserShiftReduce.tab

include ${THORSANVIL_ROOT}/build/tools/Makefile

%/ParserShiftReduce.tab.o:  FILE_WARNING_FLAGS  += -Wno-parentheses



