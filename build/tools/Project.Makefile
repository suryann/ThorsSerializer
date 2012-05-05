
.PHONY:	all test clean install %.dir

SUB_PROJECTS	= $(foreach target,$(TARGET),$(target).dir)
PREFIX			?= $(THORSANVIL_ROOT)/build

all:		ACTION=install
test:		ACTION=install
install:	ACTION=install
clean:		ACTION=clean
veryclean:	ACTION=veryclean

ACTION		?=all


all:		$(SUB_PROJECTS)
test:		$(SUB_PROJECTS)
clean:		$(SUB_PROJECTS)
veryclean:	$(SUB_PROJECTS)
install:	$(SUB_PROJECTS)


%.dir:
	make -C $* $(ACTION) PREFIX=$(PREFIX)

	
