# $Id: GNUmakefile,v 1.2 2000-10-19 12:22:10 stanaka Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := exampleB3
G4TARGET := $(name)
G4EXLIB := true

# [mtsuk] Note that this G4INSTALL is important if you want to use ROOT.
# This is related to 
# include $(G4INSTALL)/config/binmake.gmk
# at the last of this makefile.
G4INSTALL = /home/mtsuk/Geant4/geant4.9.6.p04
ifndef G4INSTALL
  G4INSTALL = ../../..
endif

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*

CPPFLAGS += -D__OUTPUT_ROOTFILE__

# [mtsuk] note that these lines should be described before 
# include $(G4INSTALL)/config/binmake.gmk
# at the last of this makefile.
CPPFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS = $(shell $(ROOTSYS)/bin/root-config --glibs)
EXTRALIBS += $(ROOTLIBS)


.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk

