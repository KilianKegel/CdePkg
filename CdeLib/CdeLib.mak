# 
#   Copyright (c) 2020, Kilian Kegel. All rights reserved.<BR>
#   SPDX-License-Identifier: BSD-2-Clause-Patent
# 
TARGET_FILES = $(OUTPUT_DIR)\CdeLib.lib

!IF "$(ARCH)" == "X64"
TORITOCLIBRARY=$(WORKSPACE)\CdePkg\toritoC64R.lib
RELEASE_STATIC_LIBRARY_FILE=$(BIN_DIR)\MdePkg\Library\BaseDebugLibNull\BaseDebugLibNull\OUTPUT\BaseDebugLibNull.lib \
	$(BIN_DIR)\MdePkg\Library\UefiRuntimeServicesTableLib\UefiRuntimeServicesTableLib\OUTPUT\UefiRuntimeServicesTableLib.lib \
	$(BIN_DIR)\MdePkg\Library\UefiBootServicesTableLib\UefiBootServicesTableLib\OUTPUT\UefiBootServicesTableLib.lib 

DEBUG_STATIC_LIBRARY_FILE=$(BIN_DIR)\MdePkg\Library\BaseDebugLibSerialPort\BaseDebugLibSerialPort\OUTPUT\BaseDebugLibSerialPort.lib \
	$(BIN_DIR)\MdePkg\Library\UefiRuntimeServicesTableLib\UefiRuntimeServicesTableLib\OUTPUT\UefiRuntimeServicesTableLib.lib \
	$(BIN_DIR)\MdePkg\Library\UefiBootServicesTableLib\UefiBootServicesTableLib\OUTPUT\UefiBootServicesTableLib.lib 
!ELSE
TORITOCLIBRARY=$(WORKSPACE)\CdePkg\toritoC32R.lib
RELEASE_STATIC_LIBRARY_FILE=
DEBUG_STATIC_LIBRARY_FILE=
!ENDIF
                                
!IF "$(TARGET)" == "DEBUG"
STATIC_LIBRARY_FILES=$(DEBUG_STATIC_LIBRARY_FILE)
!ELSE
STATIC_LIBRARY_FILES=$(RELEASE_STATIC_LIBRARY_FILE)
!ENDIF

#############################################################################
# UefiRuntimeServicesTableLib.lib
#############################################################################
$(BIN_DIR)\MdePkg\Library\UefiRuntimeServicesTableLib\UefiRuntimeServicesTableLib\OUTPUT\UefiRuntimeServicesTableLib.lib:
	@ECHO ### UefiRuntimeServicesTableLib.lib ###########################################################################
	$(MAKE) $(MAKE_FLAGS) -f $(BIN_DIR)\MdePkg\Library\UefiRuntimeServicesTableLib\UefiRuntimeServicesTableLib\Makefile

#############################################################################
# UefiBootServicesTableLib.lib
#############################################################################
$(BIN_DIR)\MdePkg\Library\UefiBootServicesTableLib\UefiBootServicesTableLib\OUTPUT\UefiBootServicesTableLib.lib:
	@ECHO ### UefiBootServicesTableLib.lib ##############################################################################
	$(MAKE) $(MAKE_FLAGS) -f $(BIN_DIR)\MdePkg\Library\UefiBootServicesTableLib\UefiBootServicesTableLib\Makefile

#############################################################################
# BaseDebugLibNull.lib
#############################################################################
$(BIN_DIR)\MdePkg\Library\BaseDebugLibNull\BaseDebugLibNull\OUTPUT\BaseDebugLibNull.lib:
	@ECHO ### BaseDebugLibNull.lib ######################################################################################
	$(MAKE) $(MAKE_FLAGS) -f $(BIN_DIR)\MdePkg\Library\BaseDebugLibNull\BaseDebugLibNull\Makefile

#############################################################################
# BaseDebugLibSerialPort.lib
#############################################################################
$(BIN_DIR)\MdePkg\Library\BaseDebugLibSerialPort\BaseDebugLibSerialPort\OUTPUT\BaseDebugLibSerialPort.lib:
	@ECHO ### BaseDebugLibSerialPort.lib ################################################################################
	$(MAKE) $(MAKE_FLAGS) -f $(BIN_DIR)\MdePkg\Library\BaseDebugLibSerialPort\BaseDebugLibSerialPort\Makefile

$(OUTPUT_DIR)\CdeLib.lib:$(STATIC_LIBRARY_FILES)
	@ECHO ### CdeLib ####################################################################################################
	"$(SLINK)" $(SLINK_FLAGS) /nologo /out:$(OUTPUT_DIR)\CdeLib.lib $(STATIC_LIBRARY_FILES) $(TORITOCLIBRARY)

all: $(TARGET_FILES) $(STATIC_LIBRARY_FILES)

