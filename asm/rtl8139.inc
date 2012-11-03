section .bss
[bits 32]

buffertx: resb 0x2000
network: resb 2048
mac: resb 6
dest: resd 100
curr: resb 1024
str2: resb 100
section .data


section .text

mmioaddres	equ 145000
ethertypevalue equ 16
extern network_buffer
extern network_bufferPointer
extern bufferpointer
extern buffer
align 4
install_rtl8139:
cli
	mov dword [network_bufferPointer], 0x0

	mov edx, mmioaddres
	add edx, 0x52	
	mov byte [edx], 0x0			;turning on 8139

	mov edx, mmioaddres
	add edx, 0x37
	mov byte [edx], 0x10			;reset

.cont:	
	mov dx, word [pciio]
	add dx, 0x37
	movzx  eax,ax
	in eax, dx
	movzx  eax,al
	and    eax,0x10
	test   eax,eax			
    jne .cont
	

	mov edx, mmioaddres
	add edx, 0x30
	mov eax, network_buffer		
	mov dword [edx], eax
	

	mov edx, mmioaddres
	add edx, 0x38
	mov word [edx], 0x0	
	
		
	mov edx, mmioaddres
	add edx, 0x37
	mov byte [edx], (1<<3)
		

	
	mov edx, mmioaddres
	add edx, 0x40
	mov dword [edx], 0x03000700

	mov edx, mmioaddres
	add edx, 0x3c
	mov word [edx], 0x0005		;IMR + ISR
	
	mov edx, mmioaddres
	add edx, 0x44
	
	mov eax, 0xf
	;or eax, (1 << 7)
	mov dword [edx],eax			 ;				0x0000071A	promisc. mode
	
	mov edx, mmioaddres
	add edx, 0x37
	mov byte [edx], 0x0C
	
	mov esi, macmsg
	call putstring
	mov ecx, 0
.mac:
	mov edx, mmioaddres
	add edx, 0x0
	add edx, ecx
	mov eax, dword [edx]
	
	push ecx
	mov ecx, 2
	mov ebx, str
	call i2hex
	mov esi, dword [dest]
	call putstring_nonewline
	pop ecx
	inc ecx
	cmp ecx, 6
	jl .mac


	sti	
ret


rtl_net_handle:
	
	
	pusha

    push ds
    push es
    push fs
    push gs


    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
   
    mov dx, word [pciio]
	add dx, 0x37
	in al, dx
	test al, (1 << 0)
	jnz .finish		; No packet received, then jump over everything
	
	call bufferpointer

    mov bx, word [buffer+ethertypevalue]
	cmp bx, 0x608		;ipv4 ARP!
	je .equalarp
	jmp .fail
	.equalarp:
	call arpsearcher
	
	
    mov dx, word [pciio]
	add dx, 0x3e
	in ax, dx  
	push ax  
	call putbin
	pop ax
	
	cmp ax, 0b0000000000000001
	je .success
	jmp .fail
	.success:
	 
	mov esi, rok
	call putstring
	
	
	.fail:
	mov esi, rokfail
	call putstring
	
		
.overfail:	

	mov bx, word [buffer+ethertypevalue]
	cmp bx, 0x0008		;ipv4!
	je .equalipv4
	jmp .finish
	.equalipv4:
	
	call icmpsearcher
	

	
	;clear active interrupt sources

    mov edx, mmioaddres
	add edx, 0x3e
	mov word [edx], 0xffff	
       
	
	.finish:
	
	
	mov al, 0x20
    mov dx, 0xA0
	out dx, al    ;clear the irq so we can get more interupts 

	mov al, 0x20	
	mov dx, 0x20
	out dx, al    ;clear the irq so we can get more interupts           
           
	pop gs
    pop fs
    pop es
    pop ds

	popa
	                                           
iret                                              
align 4
;IN - ecx bytes
send_packet:
	
	
	mov edx, mmioaddres
	add edx, 0x3e
	mov word [edx], 0xffff
	
	;mov ecx,98 	;60 is miniumum packet size
	
	xor edx, edx
	
	mov eax, 4
	mov edx, dword [tx_desc]
	mul edx
	
	
	mov ebx, mmioaddres
	add ebx, 0x20
	add ebx, eax
	mov dword [ebx], buffertx
	
	
	mov ebx, mmioaddres
	add ebx, 0x10
	add ebx, eax
	mov dword [ebx], ecx
	
	

	inc dword [tx_desc]
	
	and dword [tx_desc], 0x3
	
	

ret
align 4






icmpmsg: db "hello from icmp!", 0		
rok: db "Recieve ok!", 0
rokfail: db "Transmit ok!", 0
irqtest: db "Interupt test!", 0
macmsg: db "MAC adress for RTL 8139", 0
macmsgtr: db "MAC adress for transmitter", 0
tx_desc: dd 0
curr2: dd 0
curr4: dd 0
buffertx1: dd 0
test: dw 0
ether: dd 0
macaddress: dd 0
networkpointer: dd 0
length: dd 0
