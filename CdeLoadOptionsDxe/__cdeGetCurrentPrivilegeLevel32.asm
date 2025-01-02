;/*!
;\copyright     
;     
;    Torito C Library, Copyright (c) 2020-2025, Kilian Kegel. All rights reserved.
;
; \mainpage _CdeGetCurrentPrivilegeLevel
; 
;\brief Implementation of the <em>_CdeGetCurrentPrivilegeLevel</em> function
; 
;\file _CdeGetCurrentPrivilegeLevel32.asm
; 
;*/
	.586p

	.model flat,C
	.code	

__cdeGetCurrentPrivilegeLevel proc near public 

	mov ax,cs
	and ax,3
	ret

__cdeGetCurrentPrivilegeLevel endp
end