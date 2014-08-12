TARGET 	:= exp
CC	:=	gcc
CFLAGS 	:=	-O2 -g -Wall
CXXFLAGS	:=	$(CFLAGS)
OBJS 	:=		$(TARGET).o
LIBS 	:=	-lpthread

$(TARGET):	$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)
all:	$(TARGET)
clean:
	rm -f $(OBJS) $(TARGET)
