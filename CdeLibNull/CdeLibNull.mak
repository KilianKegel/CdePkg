# 
#   Copyright (c) 2020, Kilian Kegel. All rights reserved.<BR>
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
	copy /y $(TOROCLIBRARY) $(STATIC_LIBRARY_FILES) 
#
# remove all entry points /CRT0 that provides a real "application interface"
#
!IF "$(ARCH)" == "X64"
	"$(SLINK)" $(SLINK_FLAGS) /REMOVE:x64\Release\osifUefiDxeEntryPoint.obj    $(OUTPUT_DIR)\CdeLibNull.lib
	"$(SLINK)" $(SLINK_FLAGS) /REMOVE:x64\Release\osifUefiPeiEntryPoint.obj    $(OUTPUT_DIR)\CdeLibNull.lib
	"$(SLINK)" $(SLINK_FLAGS) /REMOVE:x64\Release\osifUefiShellEntryPoint.obj  $(OUTPUT_DIR)\CdeLibNull.lib
	"$(SLINK)" $(SLINK_FLAGS) /REMOVE:x64\Release\osifWinNTEntryPoint.obj      $(OUTPUT_DIR)\CdeLibNull.lib
!ELSE
	"$(SLINK)" $(SLINK_FLAGS) /REMOVE:x86\Release\osifUefiDxeEntryPoint.obj    $(OUTPUT_DIR)\CdeLibNull.lib
	"$(SLINK)" $(SLINK_FLAGS) /REMOVE:x86\Release\osifUefiPeiEntryPoint.obj    $(OUTPUT_DIR)\CdeLibNull.lib
	"$(SLINK)" $(SLINK_FLAGS) /REMOVE:x86\Release\osifWinNTEntryPoint.obj      $(OUTPUT_DIR)\CdeLibNull.lib
	"$(SLINK)" $(SLINK_FLAGS) /REMOVE:x86\Release\osifUefiShellEntryPoint.obj  $(OUTPUT_DIR)\CdeLibNull.lib
#
# remove additional object modules that conflict with alternate pseudo-implementations of C Library
#
	"$(SLINK)" $(SLINK_FLAGS) /REMOVE:x86\Release\getenv.obj  $(OUTPUT_DIR)\CdeLibNull.lib
	"$(SLINK)" $(SLINK_FLAGS) /REMOVE:x86\Release\qsort.obj  $(OUTPUT_DIR)\CdeLibNull.lib
	"$(SLINK)" $(SLINK_FLAGS) /REMOVE:x86\Release\strtoul.obj  $(OUTPUT_DIR)\CdeLibNull.lib
	"$(SLINK)" $(SLINK_FLAGS) /REMOVE:x86\Release\strtol.obj  $(OUTPUT_DIR)\CdeLibNull.lib
	"$(SLINK)" $(SLINK_FLAGS) /REMOVE:x86\Release\isspace.obj  $(OUTPUT_DIR)\CdeLibNull.lib
	"$(SLINK)" $(SLINK_FLAGS) /REMOVE:x86\Release\sscanf.obj  $(OUTPUT_DIR)\CdeLibNull.lib
	"$(SLINK)" $(SLINK_FLAGS) /REMOVE:x86\Release\strrchr.obj  $(OUTPUT_DIR)\CdeLibNull.lib
	"$(SLINK)" $(SLINK_FLAGS) /REMOVE:x86\Release\strchr.obj  $(OUTPUT_DIR)\CdeLibNull.lib
!ENDIF
all: $(TARGET_FILES) $(STATIC_LIBRARY_FILES)

