	## stack-param-error.s #########################################

	## Robin Miyagi ################################################
	## http://www.geocities.com/SiliconValley/Ridge/2544/ ##########

	## This file  shows how one  can access command  line parameters
	## via the stack at process  start up.  This behavior is defined
	## in the ELF specification.

	## Compile Instructions:
	## -------------------------------------------------------------
	## as --gstabs -o stack-param-error.o stack-param-error.s
	## ld -O0 -o stack-param-error stack-param-error.o
########################################################################
	.section .data

new_line_char:
	.byte 0x0a
########################################################################
	.section .text

	.globl _start

	.align 4
_start:
	movl %esp, %ebp		# store %esp in %ebp
again:
	addl $4, %esp		# %esp ---> next parameter on stack
	leal (%esp), %eax	# move next parameter into %eax
	testl %eax, %eax	# %eax (parameter) == NULL pointer?
	jz end_again		# get out of loop if yes
	call putstring		# output parameter to stdout.
	jmp again		# repeat loop
end_again:
	xorl %eax, %eax		# %eax = 0
	incl %eax		# %eax = 1, system call _exit ()
	xorl %ebx, %ebx		# %ebx = 0, normal program exit.
	int $0x80		# execute _exit () system call

	## prints string to stdout
putstring:	.type @function
	pushl %ebp
	movl %esp, %ebp
	movl 8(%ebp), %ecx
	xorl %edx, %edx
count_chars:
	movb (%ecx,%edx,1), %al
	testb %al, %al
	jz done_count_chars
	incl %edx
	jmp count_chars
done_count_chars:
	movl $4, %eax
	xorl %ebx, %ebx
	incl %ebx
	int $0x80
	movl $4, %eax
	leal new_line_char, %ecx
	xorl %edx, %edx
	incl %edx
	int $0x80 
	movl %ebp, %esp
	popl %ebp
	ret

