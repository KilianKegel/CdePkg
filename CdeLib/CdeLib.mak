# 
#   Copyright (c) 2020, Kilian Kegel. All rights reserved.<BR>
#   SPDX-License-Identifier: BSD-2-Clause-Patent
# 
TARGET_FILES = $(OUTPUT_DIR)\CdeLib.lib

!IF "$(ARCH)" == "X64"
TOROCLIBRARY=$(WORKSPACE)\CdePkg\toroC64.lib
!ELSE
TOROCLIBRARY=$(WORKSPACE)\CdePkg\toroC32.lib
!ENDIF

$(OUTPUT_DIR)\CdeLib.lib: $(TOROCLIBRARY)
	@ECHO ### CdeLib ####################################################################################################
	copy /y $(TOROCLIBRARY) $(OUTPUT_DIR)\CdeLib.lib 

all: $(TARGET_FILES)

