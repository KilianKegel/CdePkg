# 
#   Copyright (c) 2020-2024, Kilian Kegel. All rights reserved.<BR>
#   SPDX-License-Identifier: BSD-2-Clause-Patent
# 
TARGET_FILES = $(OUTPUT_DIR)\CdeLibNull.lib

!IF "$(ARCH)" == "X64"
TOROCLIBRARY=$(WORKSPACE)\CdePkg\toroC64.lib
!ELSE
TOROCLIBRARY=$(WORKSPACE)\CdePkg\toroC32.lib
!ENDIF
                                
$(OUTPUT_DIR)\CdeLibNull.lib:$(STATIC_LIBRARY_FILES)
	@ECHO ###############################################################################################################
	@ECHO ### CdeLibNull -- .OBJ removal of all non-CDEABI and operating system interface OSIF functions ################
	@ECHO ###############################################################################################################
	copy /y $(TOROCLIBRARY) $(OUTPUT_DIR)\CdeLibNull.lib 
!IF "$(ARCH)" == "X64"
	"$(SLINK)" $(SLINK_FLAGS) @$(WORKSPACE)\CdePkg\CdeLib\removeNONCDEABI64MSFT.lst $(OUTPUT_DIR)\CdeLibNull.lib
	"$(SLINK)" $(SLINK_FLAGS) @$(WORKSPACE)\CdePkg\CdeLib\removeOSIF64MSFT.lst		$(OUTPUT_DIR)\CdeLibNull.lib
!ELSE
	"$(SLINK)" $(SLINK_FLAGS) @$(WORKSPACE)\CdePkg\CdeLib\removeNONCDEABI32MSFT.lst	$(OUTPUT_DIR)\CdeLibNull.lib
	"$(SLINK)" $(SLINK_FLAGS) @$(WORKSPACE)\CdePkg\CdeLib\removeOSIF32MSFT.lst		$(OUTPUT_DIR)\CdeLibNull.lib
!ENDIF
all: $(TARGET_FILES) $(STATIC_LIBRARY_FILES)
