
;definiera strukturen för ipv4
struc   myIPV4

 version: resb 1			; :4
 typeOfService: resb 1		; :4

 Total_Length: resb 2
 identification: resb 2 
 fragmentation: resb 2
 Time_To_Live: resb 1
 protocol: resb 1
 Header_Checksum: resb 2
 sourceIP: resb 4 
 destIP: resb 4 
endstruc

structIPV4:
    istruc myIPV4

	  at version, db 0
      at typeOfService, db 0

      at Total_Length, db 0
      at identification, db 0
      at fragmentation, db 0
      at Time_To_Live, db 0
      at protocol, db 0
      at Header_Checksum, db 0 
      at sourceIP, db 0 
      at destIP, db 0 
    iend

struc   myICMP

 typeOfMessage: resb 1
 code: resb 1

 checksum: resb 2
 id: resb 2 
 seqnumber: resb 2 
endstruc

structICMP:
    istruc myICMP

	  at typeOfMessage, db 0
      at code, db 0
      at checksum, db 0
      at id, db 0
      at seqnumber, db 0
 
    iend

  


icmpsearcher:
	
	
	;beginning of ethernet frame with ethernet structure!
	mov eax, dword [buffer+10]	
	mov dword [structEth + recv_mac-1], eax
	mov ax, word [buffer+ 14]
	mov word [structEth+recv_mac+ 3], ax

	mov byte [structEth + send_mac-1], 0x00
	mov byte [structEth + send_mac+0], 0xE0
	mov byte [structEth + send_mac+1], 0x4C
	mov byte [structEth + send_mac+2], 0x4D
	mov byte [structEth + send_mac+3], 0xB8
	mov byte [structEth + send_mac+4], 0x3E

	mov byte [structEth + type_len-1], 0x08	;ethertype: ipv4, no ARP
	mov byte [structEth + type_len+0], 0x00
	
	
	
	;beginning of ipv4 structure
	mov byte [structIPV4 + version], 0x45
	mov byte [structIPV4 + typeOfService], 0
	mov byte [structIPV4 + Total_Length+0], 0 ; ?
	mov byte [structIPV4 + Total_Length+1], 0x54 ; ?
	mov word [structIPV4 + identification], 0
	mov byte [structIPV4 + fragmentation], 0 ; ?
	mov byte [structIPV4 + fragmentation], 0 ; ?
	mov byte [structIPV4 + Time_To_Live], 64
	mov byte [structIPV4 + protocol], 1
	
	mov byte [structIPV4 + sourceIP+0], 192			
	mov byte [structIPV4 + sourceIP+1], 168
	mov byte [structIPV4 + sourceIP+2], 1			;1
	mov byte [structIPV4 + sourceIP+3], 70			;3
	
	mov byte [structIPV4 + destIP+0], 10		;192
	mov byte [structIPV4 + destIP+1], 10		;168		
	mov byte [structIPV4 + destIP+2], 19		;1
	mov byte [structIPV4 + destIP+3], 100		;1
	
	
	;call internetchecksum
	;mov word [structIPV4 + Header_Checksum], ax
	
	;beginning of icmp structure
	mov byte [structICMP + typeOfMessage], 0		;0 – Echo Reply,(used to ping)
	mov byte [structICMP + code], 0


	mov ax, word [buffer+42]		
	mov word [structICMP + id],ax
	mov bx, word [buffer+44]
	mov word [structICMP + seqnumber],bx		
	
	
	mov word [structICMP + checksum],ax
	
	
	
	;data
	
	mov ecx, 46
	.loop:
	mov ebx, dword [buffer+ecx]
	sub ecx, 5
	mov [buffertx + ecx], ebx
	add ecx, 5
	inc ecx
	cmp ecx, 98
	jle .loop
	

	mov esi, structEth
	mov edi, buffertx
	mov ecx, 14 ;
	rep movsb
	
	mov esi, structIPV4
	lea edi, [buffertx + 13] 
	mov ecx, 20
	rep movsb
	
	
	
	mov esi, structICMP
	lea edi, [buffertx + 33] 
	mov ecx, 8
	rep movsb
	mov esi, icmpmsg2
	
	mov eax, [buffer + 18]
	call puthex
	
	mov ecx, 98
	
	call send_packet

	.end:
ret
ethertypeIPV4: db "this is a ipv4 message", 0
icmpmsg2: db "hello from icmp again", 0
intchecksum: dw 0
