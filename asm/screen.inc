;=======================================================  ;
;  Screen routines                                                    ;
;=======================================================  ; 
[bits 32]
section .bss

format: resb 20
bufferprintk: resb 20

;***************************************
;--pustring--
;IN: esi contains the message 
;
;***************************************
bits 32
section .text
dochar: ;so we can print out all characters
    call putchar              ; print one character
    
putstring:
	mov eax, [esi] ; put whats in the esi location in eax
	lea esi, [esi+1] ;point to next address in the string with inc 1
	cmp al, 0 ; is the low byte(the character area) of the word a null?
	jne dochar ; if not, lets print the character

	
	add byte [ypos], 1
	mov byte [xpos], 0
	push eax
	push ecx
	call scroll
	pop ecx
	pop eax
	ret
dochar2: ;so we can print out all characters
    call putchar              ; print one character
    
putstring_nonewline:
	mov eax, [esi] ; put whats in the esi location in eax
	lea esi, [esi+1] ;point to next address in the string with inc 1
	cmp al, 0 ; is the low byte(the character area) of the word a null?
	jne dochar2; if not, lets print the character

	
		push eax
	push ecx
	call scroll
	pop ecx
	pop eax
	ret

settextcolor:

	
	ret
;***************************************
;--putchar--
;IN: al contains the character 
;
;***************************************	
putchar:
	
	cmp byte [attrib], 0
	je .eq
	jmp .over
	.eq:
	mov ah, 0x0f ;what attribut do we want?
	jmp .overattrib
	.over
	mov ah, byte [attrib]
	.overattrib:
	mov ecx, eax ; lets save it
	movzx eax, byte [ypos]
	mov ebx, 160
	mul ebx
	movzx edx, byte [xpos]
	shl edx, 1
	mov edi, 0xb8000
	add edi, eax
	add edi, edx
	mov eax, ecx
	mov word [ds:edi], ax
	add byte [xpos], 1 ;5
	
	
	call update_cursor 
	
	ret
;***************************************
;--clear_screen--
;clears the screen with black color
;
;***************************************		
clear_screen:
	mov ax, 0x0720
	mov ecx, 2000 ;how many time to repeat, 2000 because 80*25 is 2000
	mov edi, 0xb8000
	rep stosw ;put whats in ax to edi, ecx times!
							
	ret
	
;***************************************
;--update_cursor--
;IN: xpos and ypos
;
;***************************************

update_cursor:

	mov bh, byte [xpos]
	mov bl, byte [ypos]
	xor ax, ax
	mov al, 80
	mul bl				; bl * al = ax
	movzx bx, bh
	add bx, ax
	mov al, 0xE
	mov ah, bh
	mov dx, 0x3D4
	out dx, ax
	inc ax
	mov ah, bl
	out dx, ax	
	
	ret
	
;***************************************
;--scroll-- it works!!
; in: [ypos]
;***************************************
scroll:

	mov eax, 0x0f
	shl eax, 0x8
	or eax, 0x20
	mov word [blank], ax

	cmp byte [ypos], 0x18
	jle .done
	mov dword [iterator], 0x0
.loop:

	mov eax, videoram
	mov edx, dword [iterator]
	shl edx, 1
	lea edx, [eax+edx*1] ;vektor videoram med edx dvs iterator som antal medlemmar.
	mov eax, videoram
	mov ecx, dword [iterator]
	add ecx, 0x50
	shl ecx, 1
	add eax,ecx
	mov ax, word [eax]
	mov [edx], ax
	inc dword [iterator]
	cmp dword [iterator], 0x77f
	jle .loop
	mov dword [iterator], 0x780
.loop2:
	mov eax, videoram
	mov edx, dword [iterator]
	shl edx, 1
	lea edx, [eax+edx*1]
	mov ax, word [blank]
	mov [edx], ax  ; blank är lika med vektorn
	inc dword [iterator]
	cmp dword [iterator], 0x7cf
	jle .loop2
	mov byte [ypos], 0x18
.done:

ret


;***************************************
;--putint-- 
; In: eax
; Return: none
;***************************************
section .bss
numstr: resb 16
section .text
putint:


lea esi, [numstr + 14] ; ladda in adressen till slutet av numstr, lämna 1 byte för NULL
.loop:
	xor edx, edx ; töm EDX, används av div annars
	mov ebx, 0xa ; div kan inte ta numret direkt
	div ebx
	; nu är eax = talet / 10, medans edx är sista siffran som var kvar, dvs den som vi vill ha
	
	mov byte [esi], dl ; skriv siffran (lagrad i dl, eftersom resten av edx är nollor) till arrayen i RAM
	add byte [esi], 0x30 ;Börja från offset 0x30, konvertera från t ex numret 5 till siffran '5' i ASCII, 0x30 = '0', 0x31 = '1' osv

	dec esi ; flytta pekaren ett steg närmare början, dvs jobba till vänster eftersom vi börjar på adressen längst till höger i arrayen
	cmp eax, 0 ; är numret 0 efter divisionen? dvs om numret är 0 så finns inga remainders.
	jne .loop ; om inte, loopa

	;.end:
	; numret är 0 nu, så skriv ut siffrorna som blev

	; skriv null till slutet av strängen
	;lea ebx, [numstr + 15]
	;mov byte [ebx], 0

	inc esi ;esi innehåller strängen men [] behövs itne då den användes med lea, vi gick bak 1 byte för långt förut, och pekar FÖRE strängen!
	call putstring
	ret
;***************************************
;--puthex-- 
; In: eax
; Return: none
;***************************************	
section .bss
hexkod: resb 16
section .text
puthex:
	lea esi, [hexkod + 14] ;lagra arrayens adress i esi, vi räknar sedan från höger till vänster
.loop:
	xor edx, edx
	mov ebx, 16
	div ebx		;tecknet finns i edx, är det över eller under 10?
	mov byte [esi], dl	;lagra dl i arrayen, dvs esi pekar på arrayen
	cmp dl, 0xa
	jge .bokstaver
	
	add byte [esi], 0x30	;börja från offset 0x30
	jmp .end		;det är ett tecken i taget
.bokstaver:
	mov byte [esi], dl
	add byte [esi], 'A' - 10
.end:
	dec esi			;vi arbetar till vänster
	cmp eax, 0		;är numret 0, isf är dl ochså 0
	jne .loop

		
    dec esi			
	mov byte [esi], '0'
	mov byte [esi +1], 'x'
	
	call putstring ;läser in adressen esi och printar ut dess innehåll
	ret
	
	

;***************************************
;--putbin-- 
; In: eax
; Return: none
;***************************************
     
    section .bss
 
    binkod: resb 33
     
    section .text
     
    putbin:
    ;Input: eax
    ;return: none
       lea esi, [binkod]
       mov dword [iterator], (1 << 31)
       mov word [leading_zero], 1
    .loop:
     
       test eax, dword [iterator]
     
       jnz .notzero
       jmp .iszero
    .notzero:
       mov word [esi], '1'
       inc esi
     
       mov byte [leading_zero], 0x0
       jmp .end
    .iszero:
       cmp word [leading_zero], 0x0
       jnz .end
       mov word [esi], '0'
       inc esi
    .end:
     
       shr dword [iterator], 1
       cmp dword [iterator], 0x0
     
       jnz .loop
     
       mov esi, binkod
       call putstring
    ret
    
    
    
    
    
    
printk:			;dont works

mov ecx, 0

;mov   [format], eax
 .looparg:
  
  cmp byte[format+ecx+0], '%'
  je .correctformat
  jmp .end
 .correctformat:
  
  cmp byte[format+ecx+1], 's'
  je .string
  jmp .nexttype
  .string:
  push ecx
  push esi
   call putstring
   add byte [ypos], 1
    pop esi
    pop ecx
   jmp .next
  .nexttype: 
  cmp byte[format+ecx+1], 'd'
  je .integer
  jmp .next
  .integer:
  
 mov eax, 4
 push ecx
 push eax
  call putint
  pop eax
  pop ecx
 .next: 

 add ecx, 2
 cmp ecx, 10
 jnz .looparg   ;loopa så länge antal argument är skilt från noll
 jmp .toret
 .end:
 mov esi, errorprintk
 call putstring
 .toret:
 
ret

	

;-------DATA---------------	
attrib: db 0
leading_zero: db 0
xpos: db 0
ypos: db 0
blank: dw 0
iterator: dd 0
hex: db "0x", 0 
videoram equ 0xb8000
hexit: dd 0
tmp: dd 0
noZeroes: db 0
