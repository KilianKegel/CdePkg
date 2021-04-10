# 
#   Copyright (c) 2020, Kilian Kegel. All rights reserved.<BR>
#   SPDX-License-Identifier: BSD-2-Clause-Patent
# 
TARGET_FILES = $(OUTPUT_DIR)\CdeLib.lib
TARGET_FILES = $(OUTPUT_DIR)\CdeLib.lib

RELEASE_STATIC_LIBRARY_FILE =   $(BIN_DIR)\MdePkg\Library\BaseDebugLibNull\BaseDebugLibNull\OUTPUT\BaseDebugLibNull.lib \
                                $(BIN_DIR)\MdePkg\Library\UefiRuntimeServicesTableLib\UefiRuntimeServicesTableLib\OUTPUT\UefiRuntimeServicesTableLib.lib \
                                $(BIN_DIR)\MdePkg\Library\UefiBootServicesTableLib\UefiBootServicesTableLib\OUTPUT\UefiBootServicesTableLib.lib 

DEBUG_STATIC_LIBRARY_FILE =     $(BIN_DIR)\MdePkg\Library\BaseDebugLibSerialPort\BaseDebugLibSerialPort\OUTPUT\BaseDebugLibSerialPort.lib \
                                $(BIN_DIR)\MdePkg\Library\UefiRuntimeServicesTableLib\UefiRuntimeServicesTableLib\OUTPUT\UefiRuntimeServicesTableLib.lib \
                                $(BIN_DIR)\MdePkg\Library\UefiBootServicesTableLib\UefiBootServicesTableLib\OUTPUT\UefiBootServicesTableLib.lib 

STATIC_LIBRARY_FILES =  \
    $(BIN_DIR)\MdePkg\Library\BaseDebugLibNull\BaseDebugLibNull\OUTPUT\BaseDebugLibNull.lib \
    $(BIN_DIR)\MdePkg\Library\BaseDebugLibSerialPort\BaseDebugLibSerialPort\OUTPUT\BaseDebugLibSerialPort.lib \
    $(BIN_DIR)\MdePkg\Library\UefiRuntimeServicesTableLib\UefiRuntimeServicesTableLib\OUTPUT\UefiRuntimeServicesTableLib.lib \
    $(BIN_DIR)\MdePkg\Library\UefiBootServicesTableLib\UefiBootServicesTableLib\OUTPUT\UefiBootServicesTableLib.lib

#############################################################################
# UefiRuntimeServicesTableLib.lib
#############################################################################
$(BIN_DIR)\MdePkg\Library\UefiRuntimeServicesTableLib\UefiRuntimeServicesTableLib\OUTPUT\UefiRuntimeServicesTableLib.lib:
    IF     EXIST $(BIN_DIR)\MdePkg\Library\UefiRuntimeServicesTableLib\UefiRuntimeServicesTableLib\Makefile \
        $(MAKE) $(MAKE_FLAGS) -f $(BIN_DIR)\MdePkg\Library\UefiRuntimeServicesTableLib\UefiRuntimeServicesTableLib\Makefile
    IF NOT EXIST $(BIN_DIR)\MdePkg\Library\UefiRuntimeServicesTableLib\UefiRuntimeServicesTableLib\Makefile \
        @echo +++ $(ARCH) --- $(TARGET) FAKED: UefiRuntimeServicesTableLib.lib & \
        mkdir $(BIN_DIR)\MdePkg\Library\UefiRuntimeServicesTableLib\UefiRuntimeServicesTableLib\OUTPUT & 
        copy $(WORKSPACE)\CdePkg\CdeLib\void32.lib $(BIN_DIR)\MdePkg\Library\UefiRuntimeServicesTableLib\UefiRuntimeServicesTableLib\OUTPUT\UefiRuntimeServicesTableLib.lib

#############################################################################
# UefiBootServicesTableLib.lib
#############################################################################
$(BIN_DIR)\MdePkg\Library\UefiBootServicesTableLib\UefiBootServicesTableLib\OUTPUT\UefiBootServicesTableLib.lib:
    IF     EXIST $(BIN_DIR)\MdePkg\Library\UefiBootServicesTableLib\UefiBootServicesTableLib\Makefile \
        $(MAKE) $(MAKE_FLAGS) -f $(BIN_DIR)\MdePkg\Library\UefiBootServicesTableLib\UefiBootServicesTableLib\Makefile
    IF NOT EXIST $(BIN_DIR)\MdePkg\Library\UefiBootServicesTableLib\UefiBootServicesTableLib\Makefile \
    @echo +++ $(ARCH) --- $(TARGET) FAKED: UefiBootServicesTableLib.lib & \
    mkdir $(BIN_DIR)\MdePkg\Library\UefiBootServicesTableLib\UefiBootServicesTableLib\OUTPUT & \
    copy $(WORKSPACE)\CdePkg\CdeLib\void32.lib $(BIN_DIR)\MdePkg\Library\UefiBootServicesTableLib\UefiBootServicesTableLib\OUTPUT\UefiBootServicesTableLib.lib

#############################################################################
# BaseDebugLibNull.lib
#############################################################################
$(BIN_DIR)\MdePkg\Library\BaseDebugLibNull\BaseDebugLibNull\OUTPUT\BaseDebugLibNull.lib:
    IF     EXIST $(BIN_DIR)\MdePkg\Library\BaseDebugLibNull\BaseDebugLibNull\Makefile $(MAKE) $(MAKE_FLAGS) -f $(BIN_DIR)\MdePkg\Library\BaseDebugLibNull\BaseDebugLibNull\Makefile
    IF NOT EXIST $(BIN_DIR)\MdePkg\Library\BaseDebugLibNull\BaseDebugLibNull\Makefile @echo +++ $(ARCH) --- $(TARGET) FAKED: BaseDebugLibNull.lib & mkdir $(BIN_DIR)\MdePkg\Library\BaseDebugLibNull\BaseDebugLibNull\OUTPUT & copy $(WORKSPACE)\CdePkg\CdeLib\void64.lib $(BIN_DIR)\MdePkg\Library\BaseDebugLibNull\BaseDebugLibNull\OUTPUT\BaseDebugLibNull.lib

#############################################################################
# BaseDebugLibSerialPort.lib
#############################################################################
$(BIN_DIR)\MdePkg\Library\BaseDebugLibSerialPort\BaseDebugLibSerialPort\OUTPUT\BaseDebugLibSerialPort.lib:
    IF     EXIST $(BIN_DIR)\MdePkg\Library\BaseDebugLibSerialPort\BaseDebugLibSerialPort\Makefile   @$(MAKE) $(MAKE_FLAGS) -f $(BIN_DIR)\MdePkg\Library\BaseDebugLibSerialPort\BaseDebugLibSerialPort\Makefile
    IF NOT EXIST $(BIN_DIR)\MdePkg\Library\BaseDebugLibSerialPort\BaseDebugLibSerialPort\Makefile   @echo +++ $(ARCH) --- $(TARGET) FAKED: BaseDebugLibSerialPort.lib & mkdir $(BIN_DIR)\MdePkg\Library\BaseDebugLibSerialPort\BaseDebugLibSerialPort\OUTPUT & copy $(WORKSPACE)\CdePkg\CdeLib\void64.lib $(BIN_DIR)\MdePkg\Library\BaseDebugLibSerialPort\BaseDebugLibSerialPort\OUTPUT\BaseDebugLibSerialPort.lib

#gen_libs:$(STATIC_LIBRARY_FILES)
#    @echo ------------------------------ gen_libs ----------------------------------------
#    @IF EXIST $(BIN_DIR)\MdePkg\Library\UefiRuntimeServicesTableLib\UefiRuntimeServicesTableLib\Makefile    "$(MAKE)" $(MAKE_FLAGS) -f $(BIN_DIR)\MdePkg\Library\UefiRuntimeServicesTableLib\UefiRuntimeServicesTableLib\Makefile
#    @IF EXIST $(BIN_DIR)\MdePkg\Library\UefiBootServicesTableLib\UefiBootServicesTableLib\Makefile          "$(MAKE)" $(MAKE_FLAGS) -f $(BIN_DIR)\MdePkg\Library\UefiBootServicesTableLib\UefiBootServicesTableLib\Makefile
#    @$(MAKE) $(MAKE_FLAGS) -f $(BIN_DIR)\MdePkg\Library\BaseDebugLibNull\BaseDebugLibNull\Makefile
#    @IF $(ARCH) == IA32  IF $(TARGET) == RELEASE echo -- 1- ARCH TARGET X86REL gen_libs
#    @IF $(ARCH) == IA32  IF $(TARGET) == DEBUG   echo -- 2- ARCH TARGET X86DEB gen_libs
#    @IF $(ARCH) == X64   IF $(TARGET) == RELEASE echo -- 3- ARCH TARGET X64REL gen_libs
#    @IF $(ARCH) == IA64  IF $(TARGET) == DEBUG   echo -- 4- ARCH TARGET X64DEB gen_libs
#    @$(MAKE) $(MAKE_FLAGS) -f $(BIN_DIR)\MdePkg\Library\BaseDebugLibNull\BaseDebugLibNull\Makefile
#
#    @$(MAKE) $(MAKE_FLAGS) -f $(BIN_DIR)\MdePkg\Library\BaseDebugLibSerialPort\BaseDebugLibSerialPort\Makefile

all: $(TARGET_FILES) $(STATIC_LIBRARY_FILES)

$(OUTPUT_DIR)\CdeLib.lib:$(STATIC_LIBRARY_FILES)
    
    @IF $(ARCH) == IA32  IF $(TARGET) == RELEASE    echo ###-- 1- ARCH TARGET X86REL & "$(SLINK)" $(SLINK_FLAGS) /nologo /out:$(OUTPUT_DIR)\CdeLib.lib $(RELEASE_STATIC_LIBRARY_FILE) $(WORKSPACE)\CdePkg\CdeLib\toritoC32R.lib
    @IF $(ARCH) == IA32  IF $(TARGET) == DEBUG      echo ###-- 2- ARCH TARGET X86DEB & "$(SLINK)" $(SLINK_FLAGS) /nologo /out:$(OUTPUT_DIR)\CdeLib.lib $(DEBUG_STATIC_LIBRARY_FILE)   $(WORKSPACE)\CdePkg\CdeLib\toritoC32R.lib
    @IF $(ARCH) == X64   IF $(TARGET) == RELEASE    echo ###-- 3- ARCH TARGET X64REL & "$(SLINK)" $(SLINK_FLAGS) /nologo /out:$(OUTPUT_DIR)\CdeLib.lib $(RELEASE_STATIC_LIBRARY_FILE) $(WORKSPACE)\CdePkg\CdeLib\toritoC64R.lib
    @IF $(ARCH) == X64   IF $(TARGET) == DEBUG      echo ###-- 4- ARCH TARGET X64DEB & "$(SLINK)" $(SLINK_FLAGS) /nologo /out:$(OUTPUT_DIR)\CdeLib.lib $(DEBUG_STATIC_LIBRARY_FILE)   $(WORKSPACE)\CdePkg\CdeLib\toritoC64R.lib 
