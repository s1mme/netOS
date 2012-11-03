


;OUT: ax - checksum

internetchecksum:
mov ax, word [buffertx + 23]	
call puthex

mov word [buffertx + 23], 0x0		
mov ax, word [buffertx + 13]		
push eax
call puthex
pop eax
mov ecx, 13										
 xor eax, eax
.compute: 	
        add ax, word [buffertx + ecx]	
     jc .addone
.back:     
        add ecx, 2			;loop 10 times!
  
        cmp ecx, 33	;here ends ipv4			
     jl .compute    
jmp .end
    .addone:
        inc ax
        jmp .back
   .end:
    not ax ; logical NOT
    mov word [intchecksum], ax 
     xchg al, ah
     push ax
    call puthex
    pop ax 
ret    

