# 
#   Copyright (c) 2020-2023, Kilian Kegel. All rights reserved.<BR>
#   SPDX-License-Identifier: BSD-2-Clause-Patent
# 
TARGET_FILES = $(OUTPUT_DIR)\CdeLibNull.lib

!IF "$(ARCH)" == "X64"
TOROCLIBRARY=$(WORKSPACE)\CdePkg\toroC64.lib
!ELSE
TOROCLIBRARY=$(WORKSPACE)\CdePkg\toroC32.lib
!ENDIF
                                
$(OUTPUT_DIR)\CdeLibNull.lib:$(STATIC_LIBRARY_FILES)
	@ECHO ### CdeLibNull ####################################################################################################
	copy /y $(TOROCLIBRARY) $(OUTPUT_DIR)\CdeLibNull.lib 
!IF "$(ARCH)" == "X64"
	"$(SLINK)" $(SLINK_FLAGS) @$(WORKSPACE)\CdePkg\CdeLib\removeNONCDEABI64MSFT.lst $(OUTPUT_DIR)\CdeLibNull.lib | find "LNK4006" > NUL
	"$(SLINK)" $(SLINK_FLAGS) @$(WORKSPACE)\CdePkg\CdeLib\removeOSIF64MSFT.lst		$(OUTPUT_DIR)\CdeLibNull.lib | find "LNK4006" > NUL
!ELSE
	"$(SLINK)" $(SLINK_FLAGS) @$(WORKSPACE)\CdePkg\CdeLib\removeNONCDEABI32MSFT.lst	$(OUTPUT_DIR)\CdeLibNull.lib | find "LNK4006" > NUL
	"$(SLINK)" $(SLINK_FLAGS) @$(WORKSPACE)\CdePkg\CdeLib\removeOSIF32MSFT.lst		$(OUTPUT_DIR)\CdeLibNull.lib | find "LNK4006" > NUL
!ENDIF
all: $(TARGET_FILES) $(STATIC_LIBRARY_FILES)
