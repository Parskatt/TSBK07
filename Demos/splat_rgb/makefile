# set this variable to the director in which you saved the common files
commondir = ../../common/

all : splat

splat : splat.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o splat -I$(commondir) -I../../common/Linux -DGL_GLEXT_PROTOTYPES splat.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

clean :
	rm splat
