LIB_FILES = yaecppl.cpp

# All source files have associated object files
LIBOFILES		= $(LIB_FILES:%.cpp=%.o)       

# all is the default rule
all	: libyaecppl.a

# remove the old tapestry library and remake the new one
libyaecppl.a:	$(LIBOFILES)
	rm -f $@
	ar cq $@ $(LIBOFILES)
