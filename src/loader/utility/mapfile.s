; Runs and compiles only on macOS under AArch64!
; I think that low-level syscalls better use in assembly language,
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
	stp		X1, X2, [SP, #-16]!
	stp		X3, X4, [SP, #-16]!
	stp		X5, X16, [SP, #-16]!
	; X0 already has path to the executable
	mov		X1, #O_RDONLY
	mov		X2, XZR
	mov		X16, #5					; open(3) bsd syscall
	svc		#0x80
	cmp		X0, #-1
	b.eq	_mopenFailure
	; Save the file descriptor
	adrp	X1, _fd@PAGE
	str		X0, [X1, _fd@PAGEOFF]
	mov		X1, XZR
	mov		X2, #SEEK_END
	mov		X16, #199				; lseek(3) bsd syscall
	svc		#0x80
	cmp		X0, #-1
	b.eq	_mopenFailure
	; Save the file size
	adrp	X1, _mapfile_t@PAGE
	add		X1, X1, _mapfile_t@PAGEOFF
	str		X0, [X1, #8]
	mov     X1, X0
	mov		X0, XZR
	mov		X2, #PROT_READ
	mov		X3, #(MAP_FILE | MAP_PRIVATE)
	adrp	X4, _fd@PAGE
	ldr		X4, [X4, _fd@PAGEOFF]
	mov		X5, XZR
	mov		X16, #197				; mmap(6) bsd syscall
	svc		#0x80
	cmp		X0, #-1
	b.eq	_mopenFailure
	adrp	X1, _mapfile_t@PAGE
	str		X0, [X1, _mapfile_t@PAGEOFF]
	; Get the file descriptor
	adrp	X1, _fd@PAGE
	ldr		X0, [X1, _fd@PAGEOFF]
	mov		X16, #6					; close(1) bsd syscall
	svc		#0x80
	adrp	X0, _mapfile_t@PAGE
	add		X0, X0, _mapfile_t@PAGEOFF
	b		_mopenEpilogue
_mopenFailure:
	mov		X0, XZR
_mopenEpilogue:
	ldp		X5, X16, [SP], #16
	ldp		X3, X4, [SP], #16
	ldp		X1, X2, [SP], #16
	ret

.private_extern _mclose
.p2align 2

_mclose:
	stp		LR, X1, [SP, #-16]!
	stp		X16, XZR, [SP, #-16]!
	adrp	X0, _mapfile_t@PAGE
	add		X0, X0, _mapfile_t@PAGEOFF
	ldp		X0, X1, [X0]
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
	ldp		X16, XZR, [SP], #16
	ldp		LR, X1, [SP], #16
	ret

.section __DATA,__bss

_fd: .long	0						; File descriptor

.p2align 3
_mapfile_t:	
	.quad 0							; mptr
	.quad 0							; msize
