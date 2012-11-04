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
	
	mov WORD  [offset], 0x2  ;
	push ecx
	
	call pci_read
	mov dword [device], eax	
	;and  eax, 0xFFFFFFFC			 
	
	cmp eax, 0x8139
	jz .bar
	
	cmp eax, 0x1692
	jz .bar
	;mov esi, eax
	
	call puthex
	jmp .end
.bar:
	mov dword [typeofdevice], eax
	pop ecx		;på 4:e loopen hittar den I/O space bar för realtek 8139 
	mov ebx, 0
	push ecx
	mov WORD  [offset], 0x10			;BAR #1 has the mmio for broadcom! 
	call pci_read
	and  eax, 0xFFFFFFFC ;0xFFFFFFFC for 8139
	
	push eax
	cmp dword [typeofdevice], 0x1692
	je .broadcom
	jmp .nobroadcom
	.broadcom:	
	mov esi, pcibarmembroadcom
	call putstring
	jmp .over8139
	.nobroadcom:
	mov esi, pcibario
	call putstring
	.over8139:
	pop eax
	mov dword [pciio], eax

	call puthex
	
	pop ecx		;på 4:e loopen hittar den I/O space bar för realtek 8139 
	mov ebx, 0
	push ecx
	mov WORD  [offset], 0x14   ;bar #2 has the mmio for realtek 8139!
	
	
	call pci_read
	push eax
	
	cmp dword [typeofdevice], 0x1692
	je .broad
	jmp .nobroad
	.broad:	
	mov esi, pcibariobroadcom		
	call putstring
	jmp .overrtl
	.nobroad:
	mov esi, pcibarmem		;BASE0     0xf0200000 SIZE 65536  MEM;
	call putstring
	.overrtl:
	
	pop eax
	mov dword [pcimem], eax
	call puthex
	

	pop ecx
	mov ebx, 0
	push ecx
	mov WORD  [offset], 0x3c
	
	
	call pci_read
	
		
	mov esi, irqnumber
	push eax
	call putstring
	pop eax
	call puthex
.end:
	
	pop ecx
	
	inc ecx

	cmp ecx, 256
	jl .loop2
	
ret


section .data
pcibarmem: db "Available memory space bar for RTL 8139:", 0	
pcibario: db "Available I/O space bar for RTL 8139:", 0
irqnumber: db "irq:", 0
pcibarmembroadcom: db "Available memory space bar for NetLink BCM57780:", 0	
pcibariobroadcom: db "Available I/O space bar for NetLink BCM57780:", 0
	irq: dw 0
	pcimem: dd 0
	pciio: dd 0
	adress: dd 0
	bus:    dd 0
	slot:   dd 0
	func:   dd 0
	offset: dw 0
	vendor: dd 0
	device: dd 0
	busloop: dd 0
	slotnumber: dd 0
	typeofdevice: dd 0
