# 
#   Copyright (c) 2020-2024, Kilian Kegel. All rights reserved.<BR>
#   SPDX-License-Identifier: BSD-2-Clause-Patent
# 
TARGET_FILES = $(OUTPUT_DIR)\CdeLib.lib

!IF "$(ARCH)" == "X64"
TOROCLIBRARY=$(WORKSPACE)\CdePkg\toroC64.lib
!ELSE
TOROCLIBRARY=$(WORKSPACE)\CdePkg\toroC32.lib
!ENDIF

$(OUTPUT_DIR)\CdeLib.lib: $(TOROCLIBRARY)
	@ECHO ###############################################################################################################
	@ECHO ### CdeLib -- .OBJ removal of all non-CDEABI functions ########################################################
	@ECHO ###############################################################################################################
	copy /y $(TOROCLIBRARY) $(OUTPUT_DIR)\CdeLib.lib 
!IF "$(ARCH)" == "X64"
	"$(SLINK)" $(SLINK_FLAGS) @$(WORKSPACE)\CdePkg\CdeLib\removeNONCDEABI64MSFT.lst	$(OUTPUT_DIR)\CdeLib.lib
!ELSE
	"$(SLINK)" $(SLINK_FLAGS) @$(WORKSPACE)\CdePkg\CdeLib\removeNONCDEABI32MSFT.lst	$(OUTPUT_DIR)\CdeLib.lib
!ENDIF

all: $(TARGET_FILES)

