;=======================================================  ;
;  Misc                                                    ;
;=======================================================  ; 

str: db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 
	section .text
cpu1:

	mov eax, 0x0
	cpuid
	mov [str], ebx
	mov [str + 4], edx
	mov [str + 8], ecx
	mov [str + 12], byte 0
	mov esi, str
	call putstring
	add byte [ypos], 1
	ret
	
;IN:edx - src, ebx - dest, ecx - count	
;	
memcpy:
	mov    eax,edx		;src,ebp+0xc
	mov    DWORD [srcp],eax		;sp,ebp-0x4
	mov    eax,ebx		;dest?,ebp+0x8
	mov    DWORD  [destp],eax
.mem:
	mov    eax,DWORD  [srcp]
	mov    edx,eax
	mov    eax,DWORD  [destp]		;dp, ebp-0x8
	mov    dword  [eax],edx
	inc    DWORD  [destp]			;dp
	inc    DWORD  [srcp]			;sp
	dec    DWORD  [ecx]			;ecx, count,ebp+0x10
	cmp    DWORD  [ecx],0x0		;ebp+0x10
	jne    .mem
ret




;IN: ECX - len, EBX - dest, EAX - val

i2hex:

   mov dword [val], eax
   mov dword [len], ecx
   mov dword [dest], ebx
   xor eax, eax
   xor edx, edx
   
   mov    eax,dword [len]		;[ebp+0x10] = len ?
   mov    edx,dword [dest]		;ebp+0xc = dest
   lea    eax,[edx+eax*1]
   mov    DWORD [cp],eax
.hexlo:
   
   
   mov    eax,dword [val]		;ebp+0x8 = val ?
   
   and    eax,0xf
   mov    BYTE [x],al
   
   shr    dword [val],0x4
   dec    DWORD [cp]
   cmp    BYTE [x],0x9
   jg .over
   mov al, 0x30
   jmp .done
.over:
   mov al, 0x37
.done:
   mov    dl,BYTE [x]		;ebp-0x5 = x?
   add    eax,edx
   mov    dl,al
   mov    eax,DWORD [cp]

   mov    BYTE [eax],dl
   mov    eax,DWORD [cp]		;ebp-0x4 = cp ?
   cmp    eax,dword [dest]
   ja     .hexlo
   mov    eax,dword [len]
   mov    edx,dword [dest]
   lea    eax,[edx+eax*1]
   mov    byte [eax],0x0

ret   

;IN: ecx - lenght of string
;    eax - string value
;    ebx - routine adress to return to


strcmp:
	mov edx, dword [eax]
	cmp dword [stringbuffer+0], edx
	je .true
	jmp .end
	.true:
	
	cmp byte [stringbuffer+ecx], '\n'
	je .calc
	jmp .end
	
	.calc:
	
	push eax
	push ecx
	
	call ebx
	
	pop ecx
	pop eax
	mov dword [buffercounter], 0x0
	mov dword [stringbuffer+0], 0x0
	mov byte [stringbuffer+ecx], 0x0
	.end:
ret


len: dd 0
val: dd 0
x: dd 0
cp: dd 0
srcp: dd 0
destp: dd 0


