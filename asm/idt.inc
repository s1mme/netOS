;=======================================================  ;
;  IDT                                                    ;
;=======================================================  ; 
section .text
align 4
start_of_idt:
;---Exceptions! ---
;interrupt 0
base_lo: dw 0
dw 0x8
dw 0x8E00 ;flags
base_hi: dw 0

;interrupt 1
dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20

;interrupt 2, intel reserved, we set the 'present' bit to 0 on this one
dw 0x0000
dw 0x10
dw 0xE00
dw 0x20

;interrupts 3-14 now, since we are making the descriptors
;identical, we are going to loop to get them all(12 total)
%rep 0xa
  dw 0x0000
  dw 0x10
  dw 0x8E00
  dw 0x20
%endrep
;interrupt 13
base_lo_gpf: dw 0
dw 0x08
dw 0x8E00
base_hi_gpf: dw 0
;interrupt 14 - page fault!
base_lo_page: dw 0
dw 0x08
dw 0x8E00
base_hi_page: dw 0
;interrupt 15, intel reserved, we set the 'present' bit to 0 on this one
dw 0x0000
dw 0x10
dw 0xE00
dw 0x20

;interrupt 16
dw 0x0000
dw 0x08
dw 0x8E00
dw 0x20

;interrupt 17
dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20
;interrupt 18
dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20
;interrupt 19
dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20
;interrupt 20
dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20
;interrupt 21
dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20
;interrupt 22
dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20

;interrupt 23
dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20
;interrupt 24
dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20
;interrupt 25
dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20
;interrupt 26
dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20
;interrupt 27
dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20
;interrupt 28
dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20
;interrupt 29
dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20
;interrupt 30
dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20
;interrupt 31
dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20

;----IRQ :D----
;interrupt 32 -timer 0
base_lo_timer: dw 0
dw 0x08
dw 0x8E00
base_hi_timer: dw 0



;----IRQ :D----
;interrupt 33 -keyboard 0
base_lo_kb: dw 0
dw 0x8
dw 0x8E00
base_hi_kb: dw 0

dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20

dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20

dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20

dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20

dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20

dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20

dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20

dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20


dw 0x0000
dw 0x10
dw 0x8E00
dw 0x20




;interrupt 43 -0x1692
base_lo_broadcom: dw 0
dw 0x8
dw 0x8E00
base_hi_broadcom: dw 0



;interrupt 43 -0x8139 
base_lo_rtl: dw 0
dw 0x08
dw 0x8E00
base_hi_rtl: dw 0
;rtl8139 irq!
;----IRQ :D----




end_of_idt:

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; now for the IDT pointer
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
idt_pointer:
  dw end_of_idt - start_of_idt - 1
  dd start_of_idt

idt_flush:
cli
	  
        lidt    [idt_pointer]
sti
        
        ret
