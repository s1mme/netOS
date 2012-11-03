section .bss
stackarray: resb 14
espointer: resb 4096

section .text
stackstr: db 0



struc stackstruct

eflags: resb 4 

codesegment: resb 4 
eip1: resb 4
edi1: resb 4 
esi1: resb 4
ebp1: resb 4
null1: resb 4
;null2: resb 4
ebx1: resb 4
edx1: resb 4 
ecx1: resb 4 
eax1: resb 4
DSseg: resb 4
ESseg: resb 4
FSseg: resb 4
GSseg: resb 4
endstruc


STACKSTRU:
istruc stackstruct

at eflags, db 0

at codesegment, db 0
at eip1, db 0
at edi1, db 0 
at esi1, db 0 
at ebp1, db 0 
at null1, db 0
;at null2, db 0
at ebx1, db 0
at edx1, db 0 
at ecx1, db 0 
at eax1, db 0
at DSseg, db 0
at ESseg, db 0
at FSseg, db 0 
at GSseg, db 0 
iend


;IN: eax - holds the eip of thread
createthread:			;anropas i kernel32.s
cli	


	
	mov dword [STACKSTRU + eflags], 0x202
	mov dword [STACKSTRU + eip1], ebx
	mov dword [STACKSTRU + codesegment], 0x08
	
	
	;mov esi, [STACKSTRU +8]
	;mov edi, stackarray
	;mov ecx, 4 ;
	;rep movsb
sti
ret


;scheduler - called from timer 
;IN: eax - holds the old esp value
;OUT: eax - new stack frame - the new esp
scheduler:

	
	
	cmp dword [pid], 0		;simple round robin
	je .pid1
	jmp .next
	.pid1: 
	mov dword [pid], 1
	
	.next:	
	cmp dword [pid], 1
	
	je .pid0
	jmp .overthis
	.pid0:
	mov dword [pid], 0
		

	.overthis:
	
	cmp dword [pid], 2
	je .zeroit
	jmp .nottwo
	.zeroit:
	mov dword [pid], 0
	.nottwo:	
	

ret


thread1:

	mov esi, threadmsg1
	call putstring
	add byte [ypos], 5
	
sti
jmp $



ret
thread2:

	mov esi, threadmsg2
	call putstring  

	hlt


ret


pid: dd 0
threadmsg1: db "This is a thread", 0
threadmsg2: db "Thread #2", 0

