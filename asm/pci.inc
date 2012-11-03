;---------------------------------------------
;pci_read -- get data with Configuration Mechanism #1
;IN: BL = Bus number
;    CL = Slot number
;OUT: EAX = data
;---------------------------------------------
section .text
CONFIG_ADDRESS	EQU	0x0CF8
CONFIG_DATA	EQU	0x0CFC
pci_read:

	mov    eax, ebx
	mov    edx,eax
	shl    edx,0x10
	mov    eax, ecx 
	shl    eax,0xb
	or     edx,eax
	mov    eax,0		;dword [func]
	shl    eax,0x8
	or     edx,eax
	mov    ax, WORD  [offset]
	and    eax,0xfc
	or     eax,edx
	or     eax,0x80000000
	mov    DWORD  [adress],eax
	mov dx, CONFIG_ADDRESS
  	out dx, eax
  	
	
	mov dx, CONFIG_DATA
	
	in eax, dx


    movzx  edx,WORD  [offset]
    and    edx,0x2
    shl    edx,0x3

    mov    cl,dl
    shr    eax,cl
	
ret

;---------------------------------------------
;pci_check -- list all devices on pci bus
;IN: none 
; 
;OUT: none
;---------------------------------------------
pci_check:

	mov dword [busloop], 0
	mov ecx, 0
.loop1:
.loop2:	
	push ecx
	mov ebx, dword [busloop]
	
	mov WORD  [offset], 0
	
	call pci_read
	mov dword [vendor], eax
	
	cmp dword [vendor], 0xffffffff	
	jz .end
	
	pop ecx
	
	mov WORD  [offset], 0x2  ;BAR #1, dvs 0x14 ger 0xf2001000 som 32-Bit Memory Space BARs, på 5:e raden dvs för 0x8139,vilket är korrekt minnes adress, dvs den loopar igenom alla enheter efter denna register adress/offset
	push ecx
	
	call pci_read
	mov dword [device], eax	
	;and  eax, 0xFFFFFFFC			;BAR #0 dvs 0x10 ger korrekt I/O Space BARs 
	
	cmp eax, 0x8139
	jz .bar
	mov esi, eax
	
	call puthex	;ta bort denna för att se bara I/O space bar
	jmp .end
.bar:
 
	pop ecx		;på 4:e loopen hittar den I/O space bar för realtek 8139 
	mov ebx, 0
	push ecx
	mov WORD  [offset], 0x10
	call pci_read
	and  eax, 0xFFFFFFFC
	
	push eax
	mov esi, pcibario
	call putstring
	pop eax
	mov dword [pciio], eax

	call puthex
	
	pop ecx		;på 4:e loopen hittar den I/O space bar för realtek 8139 
	mov ebx, 0
	push ecx
	mov WORD  [offset], 0x14
	
	
	call pci_read
	push eax
	
	mov esi, pcibarmem
	call putstring
	pop eax
	mov dword [pcimem], eax
	call puthex
	
	;pop ecx		;på 4:e loopen hittar den I/O space bar för realtek 8139 
	;mov ebx, 0
	;push ecx
	;mov WORD  [offset], 0x3c
	
	;call pci_read
	;shl eax, 16 		; high lagrat i ax
	;mov  word [irq], ax
	
	;call putbin
.end:
	
	pop ecx
	
	inc ecx

	cmp ecx, 32
	jl .loop2
	
ret


section .data
pcibarmem: db "Available memory space bar for RTL 8139:", 0	
pcibario: db "Available I/O space bar for RTL 8139:", 0
	irq: dw 0
	pcimem: dd 0
	pciio: dw 0
	adress: dd 0
	bus:    dd 0
	slot:   dd 0
	func:   dd 0
	offset: dw 0
	vendor: dd 0
	device: dd 0
	busloop: dd 0
	slotnumber: dd 0
