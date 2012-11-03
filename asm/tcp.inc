

;definiera strukturen för TCP
struc   myTCP

 sourcePort: resb 2
 destPort: resb 2

 sequenceNumber: resb 4
 acknowledgmentNumber: resb 4
 reservedanddataoffset: resb 1
 flags: resb 1
 window: resb 2
 checksumtcp: resb 2 
 urgentPointer: resb 2 
endstruc

structTCP:
    istruc myTCP

	  at sourcePort, db 0
      at destPort, db 0

      at sequenceNumber, db 0
      at acknowledgmentNumber, db 0
      at reservedanddataoffset, db 0
      at flags, db 0
      at window, db 0
      at urgentPointer, db 0 
     
    iend

process_tcp:

	
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
	mov byte [structIPV4 + protocol], 6
	

	;Header checksum: 0x68a8 [incorrect, should be 0x69a8 (maybe caused by "IP checksum offload"?)]
	mov byte [structIPV4 + sourceIP+0], 192
	mov byte [structIPV4 + sourceIP+1], 168
	mov byte [structIPV4 + sourceIP+2], 1
	mov byte [structIPV4 + sourceIP+3], 70 	;3
	
	mov byte [structIPV4 + destIP+0], 10
	mov byte [structIPV4 + destIP+1], 10
	mov byte [structIPV4 + destIP+2], 19
	mov byte [structIPV4 + destIP+3], 100
	
	
	call internetchecksum
	push ax
    call puthex
    pop ax
	mov byte [structIPV4 + Header_Checksum+0], 0xf7
	mov byte [structIPV4 + Header_Checksum+1], 0x4f
	
	;beginning of tcp structure!
	
	mov byte [structTCP + sourcePort+0], 0xcf
	mov byte [structTCP + sourcePort+1], 0x79
	mov byte [structTCP + destPort+0], 0x11
	mov byte [structTCP + destPort+1], 0x43
	
	mov eax, dword [seqnmbr]
	mov dword [structTCP + sequenceNumber], eax
	inc dword [seqnmbr]
	mov dword [structTCP + acknowledgmentNumber], 0x0

	mov byte [structTCP + reservedanddataoffset], 0xa0
	mov byte [structTCP + flags], 0b00000010
	mov byte [structTCP + window+0], 0x39
	mov byte [structTCP + window+1], 0x08
	
	mov word [structTCP + checksumtcp], 0
	mov word [structTCP + urgentPointer], 0
	
	
	
	mov esi, structEth
	mov edi, buffertx
	mov ecx, 14 ;
	rep movsb
	
	mov esi, structIPV4
	lea edi, [buffertx + 13] 
	mov ecx, 20
	rep movsb
	
	mov esi, structTCP
	lea edi, [buffertx + 33] 
	mov ecx, 20
	rep movsb
	

	mov ecx, 74
	call send_packet
ret
seqnmbr: dd 0
