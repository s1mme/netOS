struc   myEth
 recv_mac: resb 6
 send_mac: resb 6
 type_len: resb 2
endstruc

structEth:
	istruc myEth
	  at recv_mac, db 0
	  at send_mac, db 0
      at type_len, db 0
    iend

;definiera strukturen för ARP
struc   myARP
 ;payload
 hardware_addresstype: resb 1
 protocol_addresstype: resb 2

 mac_size: resb 1
 ip_size: resb 1 
 operation: resb 2
 source_mac: resb 6
 source_ip: resb 4
 dest_mac: resb 6
 dest_ip: resb 4 
endstruc



structARP:
    istruc myARP
 ;payload
	  at hardware_addresstype, db 0
      at protocol_addresstype, db 0

      at mac_size, db 0
      at ip_size, db 0
      at operation, db 0
      at source_mac, db 0
      at source_ip, db 0
      at dest_mac, db 0 
      at dest_ip, db 0 
    iend


arpsearcher:		;Called from irq handler!

	
	;beginning of ethernet frame with ethernet structure!
	mov eax, dword [buffer+10]	;get mac source adress which is destination adress for ARP reply
	mov dword [structEth + recv_mac-1], eax
	mov ax, word [buffer+ 14]
	mov word [structEth+recv_mac+ 3], ax

	mov byte [structEth + send_mac-1], 0x00
	mov byte [structEth + send_mac+0], 0xE0
	mov byte [structEth + send_mac+1], 0x4C
	mov byte [structEth + send_mac+2], 0x4D
	mov byte [structEth + send_mac+3], 0xB8
	mov byte [structEth + send_mac+4], 0x3E

	mov byte [structEth + type_len-1], 0x08	;ethertype: ARP
	mov byte [structEth + type_len+0], 0x06


	
	
	mov byte [structARP +   hardware_addresstype+0], 0x1		;here begins ARP struct
	mov byte [structARP +   hardware_addresstype+1], 0x0		
	mov byte [structARP +   protocol_addresstype+0], 0x8		
	mov byte [structARP +   protocol_addresstype+1], 0x0	


	mov byte [structARP +   mac_size], 6		;mac size
	mov byte [structARP +   ip_size], 4	;	ip size

	
	mov byte [structARP +   operation+0], 0x0		;opcode
	mov byte [structARP +   operation+1], 0x2		;opcode

	mov byte [structARP + source_mac+0], 0x00
	mov byte [structARP + source_mac+1], 0xE0
	mov byte [structARP + source_mac+2], 0x4C
	mov byte [structARP + source_mac+3], 0x4d
	mov byte [structARP + source_mac+4], 0xB8
	mov byte [structARP + source_mac+5], 0x3E

	mov byte [structARP + source_ip+0], 192		;IP adress receiver
	mov byte [structARP + source_ip+1], 168
	mov byte [structARP + source_ip+2], 1
	mov byte [structARP + source_ip+3], 70			;3

	mov eax, dword [buffer+10]	;get mac source adress which is destination adress for ARP reply
	mov dword [structARP + dest_mac+0], eax
	mov ax, word [buffer+ 14]
	mov word [structARP + dest_mac+4], ax


	mov byte [structARP + dest_ip+0], 10	;192		;IP adrerss sender
	mov byte [structARP + dest_ip+1], 10	;168
	mov byte [structARP + dest_ip+2], 19	;1	
	mov byte [structARP + dest_ip+3], 100		;1
	

	
	mov esi, structEth
	mov edi, buffertx
	mov ecx, 14 ;
	rep movsb
	
	mov esi, structARP
	lea edi, [buffertx + 14] 
	mov ecx, 28
	rep movsb
	mov ecx, 98
	call send_packet
	
	
	
	.end:
ret	

	

operation2: db "this is a request operation!", 0
ethertype: db "this is a ARP message", 0
