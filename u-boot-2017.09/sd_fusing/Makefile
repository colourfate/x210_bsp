.SUFFIXES : .c .o

CC		= gcc
ASM		=
LINK		=
LIBCC		= ar
RM		= rm

#----------------------------------------------------------------------------------------------
INCPATH		=
LIBPATH		= 
OBJPATH		= .
OUTPATH		= .
SOURCEPATH	= .

LDFLAGS		= -L$(LIBPATH) 
#LFLAGS		= -lm
ASFLAGS		=
ARFLAGS		= -ruv
CFLAGS		= -o

LIBS =

#---------------------------------------------------------------------------------------
SOURCES = $(OBJECTS:.o=.c)

all:
	$(CC) $(CFLAGS)	mkbl1 C110-EVT1-mkbl1.c 
	$(CC) $(CFLAGS)	sd_fdisk sd_fdisk.c
	$(CC) $(CFLAGS) mkx210 mkv210_image.c

#---------------------------------------------------------------------------------------
.c.o:
		$(CC) $(CFLAGS) -c $< -o $@

dep:
		gccmakedep	$(SOURCES)

#---------------------------------------------------------------------------------------
clean:
		$(RM)		-rf sd_fdisk
		$(RM)		-rf mkbl1
		$(RM)		mkx210

new:
		$(MAKE)		clean
		$(MAKE)

