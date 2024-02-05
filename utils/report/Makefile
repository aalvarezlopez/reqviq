include .env
export
TARGET = report
SOURCES = ./report.c
SOURCES += ./databaseconnect.c
SOURCES += ./requirements.c

OBJECTS = ./report.o
OBJECTS += ./databaseconnect.o
OBJECTS += ./requirements.o

ifndef ${G_MYSQL_HOST}
ifndef G_MYSQL_HOST
$(error MYSQL HOST not set. Please define -DG_MYSQL_HOST=)
endif
else
G_MYSQL_HOST=${G_MYSQL_HOST}
endif

ifndef ${G_MYSQL_USER}
ifndef G_MYSQL_USER
$(error MYSQL USER not set. Please define -DG_MYSQL_USER=)
endif
else
G_MYSQL_USER=${G_MYSQL_USER}
endif

ifndef ${G_MYSQL_PASS}
ifndef G_MYSQL_PASS
$(error MYSQL PASS not set. Plase define -DG_MYSQL_PASS)
endif
else
G_MYSQL_PASS=${G_MYSQL_PASS}
endif

CFLAGS = -DG_MYSQL_PASS=$(G_MYSQL_PASS)
CFLAGS += -DG_MYSQL_USER=$(G_MYSQL_USER)
CFLAGS += -DG_MYSQL_HOST=$(G_MYSQL_HOST)

all: $(TARGET)

.phony: clean

clean:
	@find ./ -name "*.o" -delete
	@rm -f $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ -lncurses $(shell pkg-config --libs mysqlclient)

%.o: %.c
	$(CC) $(CFLAGS) $(shell pkg-config --cflags mysqlclient)  -g -c -o $@ $<
