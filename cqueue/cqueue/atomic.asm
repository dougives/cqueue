public atomic_lock
public atomic_unlock
public atomic_set64
public atomic_set32
public atomic_increment

; assumes threads aren't using the same rsp...

.code

;------------------------------------------------------------------------------

atomic_lock proc

	xor eax, eax
	lock cmpxchg qword ptr [rcx], rsp
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

atomic_set64 proc

	mov rax, qword ptr [rcx]
	lock cmpxchg qword ptr [rcx], rdx
	jne atomic_set64_blocked
	xor eax, eax
	dec eax
	ret

atomic_set64_blocked:
	xor eax, eax
	ret

atomic_set64 endp

;------------------------------------------------------------------------------

atomic_set32 proc

	mov eax, dword ptr [rcx]
	lock cmpxchg dword ptr [rcx], edx
	jne atomic_set32_blocked
	xor eax, eax
	dec eax
	ret

atomic_set32_blocked:
	xor eax, eax
	ret

atomic_set32 endp

;------------------------------------------------------------------------------

atomic_increment proc

	lock inc qword ptr [rcx]
	ret

atomic_increment endp

;------------------------------------------------------------------------------

end