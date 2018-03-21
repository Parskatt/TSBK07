# set this variable to the director in which you saved the common files
commondir = ../../common/

all : skybox_and_cubemap_demo

skybox_and_cubemap_demo : skybox_and_cubemap_demo.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o skybox_and_cubemap_demo -I$(commondir) -I$(commondir)Linux -DGL_GLEXT_PROTOTYPES skybox_and_cubemap_demo.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm
clean :
	rm skybox_and_cubemap_demo
