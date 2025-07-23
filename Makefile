#
# Raytris -> Happy Pong
# Copyright (C) 2024 Cole Hall
# Juice Lizard modifies it on 22/04/2025 with Tchan advices from Simulant Discord

#TARGET = raytris.elf
TARGET = Happy_Pong.elf
#OBJS = src/raytris.o romdisk.o src/grid/grid.o src/colors/colors.o src/position/position.o src/blocks/block.o src/constants/vmuIcons.o src/game/game.o src/sound/soundManager.o src/vmu/vmuManager.o
#OBJS = happypong.o romdisk.o
OBJS = happypong.o
#KOS_ROMDISK_DIR = romdisk

KOS_CFLAGS += -I${KOS_PORTS}/include/raylib

all: rm-elf $(TARGET)

include $(KOS_BASE)/Makefile.rules

clean: rm-elf
	-rm -f $(OBJS)

rm-elf:
#	-rm -f $(TARGET) romdisk.*
	-rm -f $(TARGET)

$(TARGET): $(OBJS)
#	kos-c++ -o $(TARGET) $(OBJS) -lraylib -lGL -lwav
	kos-c++ -o $(TARGET) $(OBJS) -lraylib -lGL

run: $(TARGET)
	$(KOS_LOADER) $(TARGET)

dist: $(TARGET)
#	-rm -f $(OBJS) romdisk.img
	-rm -f $(OBJS)
	$(KOS_STRIP) $(TARGET)
