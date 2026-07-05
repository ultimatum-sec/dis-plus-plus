; Runs and compiles only on macOS under AArch64!
; I think that low-level syscalls better use in assembly language:
; it must increase some perfomance

.section __TEXT,__text

.equ O_RDONLY, 		0x0000
;.equ S_RDWR, 		0x29A
.equ SEEK_END, 		2
.equ PROT_READ,		0x01
.equ MAP_FILE, 		0x0000
.equ MAP_PRIVATE, 	0x0002

.private_extern _mopen
.p2align 2

_mopen:
	stp		X2, X3, [SP, #-16]!
	stp		X4, X5, [SP, #-16]!
	stp		X16, FP, [SP, #-16]!
	sub		SP, SP, #16
	mov		FP, SP
	; X0 already has path to the executable
	mov		X1, #O_RDONLY
	mov		X2, XZR
	mov		X16, #5					; open(3) bsd syscall
	svc		#0x80
	cmp		X0, #-1
	b.eq	_mopenFailure
	; Save the file descriptor
	str		X0, [FP]
	mov		X1, XZR
	mov		X2, #SEEK_END
	mov		X16, #199				; lseek(3) bsd syscall
	svc		#0x80
	cmp		X0, #-1
	b.eq	_mopenFailure
	; Save the file size
	str		X0, [FP, #8]
	mov     X1, X0
	mov		X0, XZR
	mov		X2, #PROT_READ
	mov		X3, #(MAP_FILE | MAP_PRIVATE)
	ldr		X4, [FP]
	mov		X5, XZR
	mov		X16, #197				; mmap(6) bsd syscall
	svc		#0x80
	cmp		X0, #-1
	b.eq	_mopenFailure
	; Get the file descriptor
	ldr		X1, [FP]
	str		X0, [FP]
	mov		X0, X1
	mov		X16, #6					; close(1) bsd syscall
	svc		#0x80
	; Restore the values from FP
	ldp		X0, X1, [FP]
	b		_mopenEpilogue
_mopenFailure:
	mov		X0, XZR
	mov		X1, XZR
_mopenEpilogue:
	add		SP, SP, #16
	ldp		X16, FP, [SP], #16
	ldp		X4, X5, [SP], #16
	ldp		X2, X3, [SP], #16
	ret

.private_extern _mclose
.p2align 2

_mclose:
	stp		X16, LR, [SP, #-16]!
	cbz		X0, _mcloseFailure
	cmp		X1, #-1
	b.eq	_mcloseFailure			; It ain't good...
	mov		X16, #73				; munmap(2) bsd syscall
	svc		#0x80
	cmp		X0, #-1
	b.eq	_mcloseFailure			; It ain't good too...
	b		_mcloseEpilogue
_mcloseFailure:
	mov		X0, #-1					; Something went wrong...
_mcloseEpilogue:
	ldp		X16, LR, [SP], #16
	ret
