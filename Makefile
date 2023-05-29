TARGET=suntimes.com

# Get Hi-Tech C binaries from here:
HITECHURL=https://raw.githubusercontent.com/agn453/HI-TECH-Z80-C/master/htc-bin.lbr

# To make the executable handle file redirection on the command line,
#   use crtcpm.obj as the entry module. This adds about 8kb to the
#   executable size.
CRT=nrtcpm.obj

HITECHFILES=assert.h debug.com libovr.lib overlay.h stdio.h \
            c309-17.com drtcpm.obj libr.com p1.com stdlib.h \
            c309.com limits.h string.h cgen.com linq.com \
            symtoas.com exec.h rrtcpm.obj sys.h conio.h float.h \
            setjmp.h time.h cpm.h math.h signal.h unixio.h cpp.com \
            nrtcpm.obj stat.h zas.com cref.com hitech.h objtohex.com  \
            stdarg.h crtcpm.obj libc.lib optim.com stddef.h ctype.h \
            libf.lib options stdint.h

SRCS=$(shell find . -type f -name '*.c' -execdir basename {} \;)
OBJS=$(patsubst %,%,$(SRCS:.c=.obj))

# Warn the user if lbrate is not installed. 
LBRATE=$(shell which lbrate)
ifeq (, ${LBRATE})
  $(warning "lbrate" is not installed. You will not be able to do)	
  $(warning "'make prepare'. Get lbrate from here:)
  $(warning "https://www.ibiblio.org/pub/linux/utils/compress/lbrate-1.1.tar.gz)
endif

# Invoke the checkprepared script when building. This checks for the presence
#   of certain prerequisies, that are hard to check in a Makefile
all: checkprepared $(TARGET) 
	@echo Building $(TARGET) 

# My dependencies. HI-TECH C has, understandably enough, no build in
#   dependency generation. We could use Linux tools like 'makedepend' to add
#   these to the Makefile but, frankly, no CP/M C program is ever likely to 
#   be complex enough to need this.
main.obj: defs.h getopt.h compat.h strings.h suntimes.h
getopt.obj: defs.h getopt.h compat.h
compat.obj: defs.h compat.h
strings.obj: defs.h strings.h
suntimes.obj: defs.h suntimes.h

checkprepared: 
	@./checkprepared.sh

# Rules for the various stages of invoking HI-TECH C

%.pre: %.c
	echo Preprocessing $<
	@./preprocess.sh $< 
	
%.p1: %.pre
	echo Pass one $<
	@./pass1.sh $< 

%.asm: %.p1
	echo Code generation $<
	@./codegen.sh $< 

%.as2: %.asm
	echo Optimization $<
	@./optim.sh $< 

%.obj: %.as2
	cpm zas $< 

# romwbw.zas is the only human-generated assembly-language file in the
#   program.
romwbw.obj: romwbw.zas
	cpm zas romwbw.zas

$(TARGET): $(OBJS) romwbw.obj 
	@./rename_objs.sh $(OBJS)
	@# libf must come before libc to get floating-point versions
	@#   of printf(), etc
	cpm linq -C100H -Oo.com $(CRT) "_*.obj" romwbw.obj libf.lib libc.lib
	mv o.com $(TARGET)

clean:
	rm -f *.pre *.p1 *.asm *.as2 o.com _*.obj romwbw.obj $(OBJS) '$$'*

unprepare: 
	rm -f hitech.lbr
	rm -f $(HITECHFILES)

prepare:
	curl -o hitech.lbr $(HITECHURL)
	lbrate hitech.lbr

