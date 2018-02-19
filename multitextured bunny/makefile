commondir = ../common/

all : multitexbunny

lab3-1 : multitexbunny.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o multitexbunny -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES multitexbunny.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

clean :
	rm multitexbunny
