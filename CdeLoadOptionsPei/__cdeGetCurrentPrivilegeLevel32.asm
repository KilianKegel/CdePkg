;/*!
;\copyright     
;     
;    Torito C Library, Copyright (c) 2020-2025, Kilian Kegel. All rights reserved.
;
; \mainpage __cdeGetCurrentPrivilegeLevel
; 
;\brief Implementation of the <em>__cdeGetCurrentPrivilegeLevel</em> function
; 
;\file __cdeGetCurrentPrivilegeLevel32.asm
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