#
# project config
# GNU Make include file
#

MODULES=	\
types		\
math		\
fps		\
pixmap		\
gui		\
api		\
device		\
devicemanager	\
rglview		\
scene		\
glgui

OBJS=$(foreach x, $(MODULES), $x.o )

