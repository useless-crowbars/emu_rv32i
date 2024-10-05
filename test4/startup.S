.text
.global _start
_start:
	li sp, 0x8000 // 32kb
	call main
	fence
