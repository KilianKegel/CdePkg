;/*!
;\copyright     
;     
;    Torito C Library, Copyright (c) 2020, Kilian Kegel. All rights reserved.
;
; \mainpage _CdeGetCurrentPrivilegeLevel
; 
;\brief Implementation of the <em>_CdeGetCurrentPrivilegeLevel</em>
; 
;\file _CdeGetCurrentPrivilegeLevel64.asm
; 
;*/
	.code	
_CdeGetCurrentPrivilegeLevel proc public

	mov ax,cs
	and ax,3
	ret

_CdeGetCurrentPrivilegeLevel endp
end