;/*!
;\copyright     
;     
;    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.
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

_CdeGetCurrentPrivilegeLevel proc near public 

	mov ax,cs
	and ax,3
	ret

_CdeGetCurrentPrivilegeLevel endp
end