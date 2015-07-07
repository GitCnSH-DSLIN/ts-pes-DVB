KBUILD_SRC=$(CURDIR)
INCLUDE_PATHS := -I$(CURDIR)/include -I$(CURDIR)/src/descriptors/include
KBUILD_OUT=$(CURDIR)/bin

export KBUILD_SRC INCLUDE_PATHS KBUILD_OUT

CFLAGS  += -g  -Wall $(INCLUDE_PATHS) 

export CFLAGS


srctree     := $(if $(KBUILD_SRC),$(KBUILD_SRC),$(CURDIR))
objtree     := $(CURDIR)
src     := $(srctree) 
obj     := $(objtree) 

export srctree objtree


NSTALL_MOD_PATH ?= $(KBUILD_OUT) 
export INSTALL_MOD_PATH

# Make variables (CC, etc...)

AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CFLAGS	+= $(INCLUDE_PATHS)
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
LDR		= $(CROSS_COMPILE)ldr
STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump
AWK		= awk
CONFIG_SHELL	= /bin/sh
RANLIB		= $(CROSS_COMPILE)RANLIB
DTC		= dtc
CHECK		= sparse

CHECKFLAGS     := -D__linux__ -Dlinux -D__STDC__ -Dunix -D__unix__ \
		  -Wbitwise -Wno-return-void -D__CHECK_ENDIAN__ $(CF)

KBUILD_CPPFLAGS := -D__KERNEL__

KBUILD_CFLAGS   := -Wall -Wstrict-prototypes \
		   -Wno-format-security \
		   -fno-builtin -ffreestanding
KBUILD_AFLAGS   := -D__ASSEMBLY__

export CONFIG_SHELL CROSS_COMPILE AS LD CC CFLAGS
export CPP AR NM LDR STRIP OBJCOPY OBJDUMP
export MAKE AWK
export DTC CHECK CHECKFLAGS
export KBUILD_CFLAGS KBUILD_AFLAGS


all: _all install 


_all:
	@$(MAKE) -C src all
	@echo "***********************************************"
	@echo "**        Build Finished                     **"
	@echo "***********************************************"


install:
	#$(shell mkdir -p $(KBUILD_OUT))
	@$(MAKE) -C src install
	@echo "***********************************************"
	@echo "**        Install Finished                   **"
	@echo "***********************************************"

.PHONY: clean
clean:
	@$(MAKE) -C src clean
	-rm -rf  $(KBUILD_OUT)/test*
	@echo "***********************************************"
	@echo "**        Clean Finished                     **"
	@echo "***********************************************"

