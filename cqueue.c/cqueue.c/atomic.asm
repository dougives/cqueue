public atomic_lock

; assumes threads aren't using the same stack frame...

.code
atomic_lock proc

	xor eax, eax
	lock cmpxchg qword ptr [rcx], rsp
	jne blocked
	dec eax
	ret

blocked:
	ret

atomic_lock endp
end