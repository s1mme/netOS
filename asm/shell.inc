
help:
add byte [ypos], 1
mov byte [xpos], 0
push eax
	push ecx
	call scroll
	pop ecx
	pop eax
	mov byte [attrib], 0x03
	mov esi,packetmsg
	call putstring_nonewline
	mov byte [attrib], 0
	
	
	
ret

packetmsg: db "List of commands:", 0
shelluser: db "netOS[root] $ ", 0




