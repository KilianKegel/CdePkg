;/*!
;\copyright     
;     
;    Torito C Library, Copyright (c) 2020-2025, Kilian Kegel. All rights reserved.
;
; \mainpage _CdeGetCurrentPrivilegeLevel
; 
;\brief Implementation of the <em>_CdeGetCurrentPrivilegeLevel</em>
; 
;\file _CdeGetCurrentPrivilegeLevel64.asm
; 
;*/
	.code	
__cdeGetCurrentPrivilegeLevel proc public

	mov ax,cs
	and ax,3
	ret

__cdeGetCurrentPrivilegeLevel endp
end