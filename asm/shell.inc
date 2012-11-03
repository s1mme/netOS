
help:

	add byte [ypos], 1
	mov byte [xpos], 0
	mov byte [attrib], 0x03
	call scroll
	mov esi,packetmsg
	call putstring
	mov byte [attrib], 0
	
	mov byte [xpos], 0
	mov byte [attrib], 0x05
	
	mov esi, resetcpu
	call putstring
	mov byte [attrib], 0
	
	mov byte [xpos], 0
	mov byte [attrib], 0x05
	mov esi, uptimemsg
	call putstring 
	mov byte [attrib], 0
	
		
	
ret

reboot:
	mov dx, 0x64
	mov ax, 0xFE
	out dx, ax
ret

uptime:
	mov ecx, 100
	xor edx, edx
	mov eax,  dword [tick_count]
	div ecx
	add byte [ypos], 1
	mov byte [xpos], 0
	push eax
	call scroll
	pop eax
	mov byte [attrib], 0x04
	call putint
	mov byte [attrib], 0x0
ret


packetmsg: db "List of commands:", 0
shelluser: db "netOS[root] $ ", 0
resetcpu: db "reboot - resets the computer", 0
uptimemsg: db "uptime - displays current uptime in seconds", 0

