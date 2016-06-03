public atomic_lock
public atomic_unlock

; assumes threads aren't using the same rsp...

.code

;------------------------------------------------------------------------------

atomic_lock proc

	xor eax, eax
	lock cmpxchg dword ptr [rcx], rsp
	jne atomic_lock_blocked
	dec eax
	ret

atomic_lock_blocked:
	ret

atomic_lock endp

;------------------------------------------------------------------------------

atomic_unlock proc

	mov rax, qword ptr [rcx]
	lock xor qword ptr [rcx], rax
	ret

atomic_unlock endp

;------------------------------------------------------------------------------

atomic_set proc

	mov rax, qword ptr [rcx]
	lock cmpxchg qword ptr [rcx], rdx
	jne atomic_set_blocked
	xor eax, eax
	dec eax
	ret

atomic_set_blocked:
	xor eax, eax
	ret

atomic_set endp

;------------------------------------------------------------------------------

end