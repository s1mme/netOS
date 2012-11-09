section .bss
[bits 32]

status_block: resb 80

recieve_buffer_stdring: resb 6000
recieve_buffer_returnring: resb 6000
section .text

broadcom_init:
;INITIALIZATION PROCEDURE
	cli	
	;Set the Enable bit in the Memory Arbiter Mode register
	mov edx, 0x145000
	add edx, 0x4000
	mov dword [edx], 0b00000000000000000000000000000010
	;Write the T3_MAGIC_NUMBER (i.e., 0x4B657654) to the MAC memory at 0xb50 
	mov edx, 0x145000
	add edx, 0xb50
	mov dword [edx], 0x4B657654
	;Enable MAC memory space decode and bus mastering
	mov edx, 0x145000
	add edx, 0x04
	mov word [edx], 0b0000000000000110
	;Set the Enable bit in the Memory Arbiter Mode register
	mov edx, 0x145000
	add edx, 0x4000
	mov dword [edx], 0b00000000000000000000000000000010
	
	;Set the Enable_Indirect_Access bit and the Enable the PCI State register and Enable the PCI Clock Control register 
	mov edx, 0x145000
	add edx, 0x68
	mov dword [edx], 0b00000000000000000000000000110000
	
	mov edx, 0x145000
	add edx, 0x400
	mov dword [edx], 0x00000000
	
	mov edx, 0x145000
	add edx, 0x6c
	mov dword [edx], 0b11111111001111110000000000000000		
	
	mov edx, 0x145000
	add edx, 0x6800
	mov dword [edx], 0b00000000000000100000000000000000
	;Indicate Driver is ready to RX traffic. Set the Host_Stack_Up bit in the General Mode Control register
	mov edx, 0x145000
	add edx, 0x6800
	mov dword [edx], 0b00000000000000010000000000000000
	;Enable the buffer manager
	mov edx, 0x145000
	add edx, 0x4400
	mov dword [edx], 0b00000000000000000000000000000110
	;Enable internal hardware queues.
	mov edx, 0x145000
	add edx, 0x5c00
	mov dword [edx], 0xffffffff
	
	mov edx, 0x145000
	add edx, 0x5c00
	mov dword [edx], 0x00000000
	;Initialize the Standard Receive Buffer Ring	
	mov edx, 0x145000
	add edx, 0x2450
	mov dword [edx], recieve_buffer_stdring

	mov ebx, 0x145000
	add ebx, 0x2458 ; Max_Length 
	mov eax, 0x600
	shl eax, 16 ; shift 16 bits to left
	mov dword [ebx], eax

	
	mov edx, 0x145000
	add edx, 0x2C18
	mov dword [edx], 0
	;Initialize send producer index registers in mailbox
	mov edx, 0x145000
	add edx, 0x380
	mov dword [edx], 0x00000000
	;Initialize send rings.
	mov edx, 0x145000	;NIC Ring Address = 0x4000 + (Ring_Number * sizeof(Send_Buffer_Descriptor) *NO_BDS_IN_RING) / 4
	add edx, 0x100
	mov eax, 0x4000
	add eax, 0x400	
	mov dword [edx],eax     ;0x4000 + ((1*32*128)/4	
	
	mov edx, 0x145000
	add edx, 0x280
	mov eax, recieve_buffer_returnring		;Initialize Receive Return Rings. The Receive Return RCBs are located in the Miscellaneous Memory region from 0x200 to 0x2FF
	mov eax, dword [edx]
	
	mov edx, 0x145000
	add edx, 0x245C
	mov dword [edx], 0x00000000
	;Initialize the Receive Producer Ring mailbox registers
	mov ebx, 0x145000
	add ebx, 0x268 ;  
	mov dword [ebx], 0b00000000000000000000000000000000	
	;Configure the Message Transfer Unit MTU size.
	mov ebx, 0x145000
	add ebx, 0x43C ;  
	mov dword [ebx], 0x05F2	
	;Configure IPG for transmit
	mov ebx, 0x145000
	add ebx, 0x464 ;  
	mov dword [ebx], 0x2620	
	;Configure default RX return ring for non-matched packets
	mov ebx, 0x145000
	add ebx, 0x500 ;  
	mov dword [ebx], 0b00000000000000000000000001000000
	;Configure the number of Receive Lists.
	mov ebx, 0x145000
	add ebx, 0x2010 ;  
	mov dword [ebx], 0x181
	;Enable RX statistics
	mov ebx, 0x145000
	add ebx, 0x2014 ;  
	mov dword [ebx], 0b00000000000000000000000000000001
	;Disable the host coalescing engine.
	mov ebx, 0x145000
	add ebx, 0x3c00 ;  
	mov dword [ebx], 0x0000
	;Configure the host coalescing tick count.
	;receive
	mov ebx, 0x145000
	add ebx, 0x3C08 ;  
	mov dword [ebx], 150
	;send
	mov ebx, 0x145000
	add ebx, 0x3c0c ;  
	mov dword [ebx], 150

	;Configure the host coalescing BD count.
	;receive
	mov ebx, 0x145000
	add ebx, 0x3C10 ;  
	mov dword [ebx], 150
	;send
	mov ebx, 0x145000
	add ebx, 0x3c14 ;  
	mov dword [ebx], 150	
	;Configure the max-coalesced frames during interrupt counter
	;receive
	mov ebx, 0x145000
	add ebx, 0x3C20 ;  
	mov dword [ebx],0
	;send
	mov ebx, 0x145000
	add ebx, 0x3c24 ;  
	mov dword [ebx], 0
	;Initialize host status block address
	mov ebx, 0x145000
	add ebx, 0x3c38 ;  
	mov dword [ebx], status_block
	;Enable the host coalescing engine
	mov ebx, 0x145000
	add ebx, 0x3c00 ;  
	mov dword [ebx], 0b00000000000000000000000000000010
	;Enable the receive BD completion functional block
	mov ebx, 0x145000
	add ebx, 0x3000 ;  
	mov dword [ebx], 0b00000000000000000000000000000110
	;Enable the receive list placement functional block. Set the Enable bit in the Receive List Placement Mode register 
	mov ebx, 0x145000
	add ebx, 0x2000 ;  
	mov dword [ebx], 0b00000000000000000000000000000010
	;Enable and clear statistics and 77. Enable DMA engines.
	mov ebx, 0x145000
	add ebx, 0x400 ;  
	mov dword [ebx], 0b00000000011100001101100000000000
	;Configure the General Miscellaneous Local Control register		
	mov ebx, 0x145000
	add ebx, 0x6808 ;  
	mov dword [ebx], 0b00000000000000000000000000001000
	;Write a value of zero to the Interrupt Mailbox 0 low word for the host standard and flat modes
	mov ebx, 0x145000
	add bx, 0x200 ;  
	mov word [bx], 0x0000
	;Configure the Write DMA Mode register
	mov ebx, 0x145000
	add ebx, 0x4c00 ;  
	mov dword [ebx], 0b00000000000000000000000000000010
	;Configure the Read DMA Mode register
	mov ebx, 0x145000
	add ebx, 0x4800 ;  
	mov dword [ebx], 0b00000000000000000000000000000010
	;Enable the receive data completion functional block
	mov ebx, 0x145000
	add ebx, 0x2800 ;  
	mov dword [ebx], 0b00000000000000000000000000000110
	;Enable the send data completion functional block.
	mov ebx, 0x145000
	add ebx, 0x1000 ;  
	mov dword [ebx], 0b00000000000000000000000000000010
	;Enable the send BD completion functional block
	mov ebx, 0x145000
	add ebx, 0x1c00 ;  
	mov dword [ebx], 0b00000000000000000000000000000110

	mov ebx, 0x145000
	add ebx, 0x2c00 ;  
	mov dword [ebx], 0b00000000000000000000000000000110	
	;Set the Enable and Illegal_Return_Ring_Size bits
	mov ebx, 0x145000
	add ebx, 0x2400 ;  
	mov dword [ebx], 0b00000000000000000000000000010010		
	;Enable the send data initiator functional block. 
	mov ebx, 0x145000
	add ebx, 0x0c00 ;  
	mov dword [ebx], 0b00000000000000000000000000000010
	;Enable the send BD initiator functional block.
	mov ebx, 0x145000
	add ebx, 0x1800 ;  
	mov dword [ebx], 0b00000000000000000000000000000110
	;Enable the send BD selector functional block.
	mov ebx, 0x145000
	add ebx, 0x1400 ;  
	mov dword [ebx], 0b00000000000000000000000000000110
	;Enable the transmit MAC.
	mov ebx, 0x145000
	add ebx, 0x45c ;  
	mov dword [ebx], 0b00000000000000000000000000000010
	;Enable the receive MAC	
	mov ebx, 0x145000
	add ebx, 0x468 ;  
	mov dword [ebx], 0b00000000000000000000000000000010

	mov ebx, 0x145000
	add ebx, 0x450 ;  
	mov dword [ebx], 0b00000000000000000000000000000010
	;Setup multicast filters.
	mov edx, 0x145000
	add edx, 0x468
	mov dword [edx], 0b00000000000000000000000100000000	;promisc. mode
	;Enable interrupts. Clear the Mask_PCI_Interrupt_Output bit in 
	mov edx, 0x145000
	add edx, 0x68
	mov dword [edx], 0b00000000000000000000000000000001
	
;	mov ebx, 0x145000
;	add ebx, 0x1b
;	mov dword [ebx], 0xffff	
	
		
	mov edx, 0x145000
	add edx, 0x40c
	mov dword [edx],0xffffffff
	sti

ret

broadcom_net_handle:
cli
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

	;Acknowledge interrupt. Write a nonzero value (i.e., value = 1) to the interrupt mailbox 0
	mov ebx, 0x145000
	add ebx, 0x200 ;  
	mov dword [ebx], 0xffffffff
	
	;Read and save the value of the Status Tag field of the Status Block
	mov eax, [status_block + 4]		;save the status tag	
	push eax	

	
	mov ebx, 0x145000
	add ebx, 0x280 			;Receive BD Return Ring 1 Consumer Index in mailbox
	mov eax, dword [ebx]
	
	.update:
	mov ebx, [status_block+0]
	cmp ebx, 0b00000000000000000000000000000001
	je .action
	jmp .end
	.action:
	;Clear the Updated bit of the status word
	mov dword [status_block+0], 0x0
	
	;Check for RX traffic.
	;Loop through enabled RX Return Rings (1 to 16).
	;Check for difference between RX Return Ring Producer index (Status block) and RX Return Ring Consumer index
	;(value written to mailbox on previous call) are the number of frames to process for RX Return Ring.
	mov ecx, [status_block+18]		;RX Return Ring Producer index (Status block)
	
	sub ecx, eax ; ??? difference?
.loopindexrec:	
	
	;process packet!
	mov eax, dword [recieve_buffer_returnring + ecx]

	dec ecx
	cmp ecx, 0
	jnz .loopindexrec
	
	;Compare the current value of the Status Tag to the saved value of the Status Tag
	mov edx, [status_block + 4]
	
	pop eax
	push eax
	cmp eax, edx
	jne .equal
	jmp .end
	.equal:
	;Update the RX Return Ring consumer pointer in each mailbox for new RX frames.
	mov ebx, 0x145000
	add ebx, 0x280 			;Receive BD Return Ring 1 Consumer Index in mailbox
	mov eax, dword [ebx]
	jmp .update		;Check the Updated bit in the status word


	
	.end:
	pop eax
	mov ebx, 0x145000
	add ebx, 0x200 ;  
	shl eax, 23
	or eax, 0xff000000
	mov dword [ebx], eax

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
	sti
iret


;When a packet is received from the LAN, the BCM57XX DMAs the packet to a location in the host, and then DMAs the
;related BD to a return ring. As the producer of this packet to the host, the BCM57XX family updates the Status Block producer
;indices for the related return ring (i.e., return ring 1 to return ring 16 that was DMAed the BD received packet). These return
;ring indices, RX Return Producer 1 to RX Return Producer 16 shown in Table 39 on page 105, can then be read by the host
;software to determine the last BD index value of a particular ring that has information of the last received packet.
;As the consumer of the received packet, the host software must update the return ring consumer indices in Mailbox registers
;Receive BD Return Ring 1 Consumer Index (memory offset 0x280–0x287 for host standard and flat modes and 0x5880–
;0x5887 for indirect mode) through Receive BD Return Ring 16 Consumer Index (memory offset 0x300–0x307 for host
;standard and flat modes and 0x5900–0x5907 for indirect mode).
