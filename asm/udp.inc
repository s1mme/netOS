
[bits 32]

section .bss
buffermsg: resb 2048




section .text

struc   myUDP

 Source_port_udp: resb 2
 Destination_port_udp: resb 2

 Length_udp: resb 2
 Checksum_udp: resb 2 

endstruc

structUDP:
    istruc myUDP
	  at Source_port_udp, db 0
      at Destination_port_udp, db 0
      at Length_udp, db 0
      at Checksum_udp, db 0
 
    iend

udpsearcher:

	
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
	mov byte [structIPV4 + protocol], 17
	

	mov byte [structIPV4 + sourceIP+0], 192
	mov byte [structIPV4 + sourceIP+1], 168
	mov byte [structIPV4 + sourceIP+2], 1
	mov byte [structIPV4 + sourceIP+3], 3
	
	mov byte [structIPV4 + destIP+0], 192
	mov byte [structIPV4 + destIP+1], 168
	mov byte [structIPV4 + destIP+2], 1
	mov byte [structIPV4 + destIP+3], 1
	
	;beginning of udp structure!
	
	mov byte [structUDP + Source_port_udp+0], 0x13
	mov byte [structUDP + Source_port_udp+1], 0x88
	mov byte [structUDP + Destination_port_udp+0], 0x13
	mov byte [structUDP + Destination_port_udp+1], 0x88

	mov byte [structUDP + Length_udp+0], 0x00
	mov byte [structUDP + Length_udp+1], 0x40
	mov word [structUDP + Checksum_udp], 0
	
	
	mov esi, structEth
	mov edi, buffertx
	mov ecx, 14 ;
	rep movsb
	
	mov esi, structIPV4
	lea edi, [buffertx + 13] 
	mov ecx, 20
	rep movsb
	
	mov esi, structUDP
	lea edi, [buffertx + 33] 
	mov ecx, 8
	rep movsb
	
	
	;datagram!
	mov ecx, 41
	.loop:
	
	;hello world datagram!
	mov byte [buffertx + ecx +0], 0x20
	mov byte [buffertx + ecx +1], 0x48 
	mov byte [buffertx + ecx +2], 0x45
	mov byte [buffertx + ecx +3], 0x4C
	mov byte [buffertx + ecx +4], 0x4C
	mov byte [buffertx + ecx +5], 0x4F
	mov byte [buffertx + ecx +6], 0x20
	mov byte [buffertx + ecx +7], 0x57
	mov byte [buffertx + ecx +8], 0x4F
	mov byte [buffertx + ecx +9], 0x52
	mov byte [buffertx + ecx +10], 0x4C
	mov byte [buffertx + ecx +11], 0x44
	mov byte [buffertx + ecx +12], 0x21
	mov byte [buffertx + ecx +13], 0x21
	mov byte [buffertx + ecx +14], 0x20
	add ecx, 14
	cmp ecx, 97
	jle .loop
	
	mov ecx, 98

	
	call send_packet
ret
	
