	.file	"fcopy.c"
	.text
	.section	.rodata
.LC0:
	.string	"Usage: fcopy file1 file2\n"
.LC1:
	.string	"r"
.LC2:
	.string	"w"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$4096, %rsp
	orq	$0, (%rsp)
	subq	$4096, %rsp
	orq	$0, (%rsp)
	subq	$64, %rsp
	movl	%edi, -8244(%rbp)
	movq	%rsi, -8256(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	cmpl	$3, -8244(%rbp)
	je	.L2
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$25, %edx
	movl	$1, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	movl	$-1, %eax
	jmp	.L8
.L2:
	movq	-8256(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	leaq	.LC1(%rip), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	fopen@PLT
	movq	%rax, -8224(%rbp)
	cmpq	$0, -8224(%rbp)
	jne	.L4
	movq	-8256(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	perror@PLT
	movl	$-1, %eax
	jmp	.L8
.L4:
	movq	-8256(%rbp), %rax
	addq	$16, %rax
	movq	(%rax), %rax
	leaq	.LC2(%rip), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	fopen@PLT
	movq	%rax, -8216(%rbp)
	cmpq	$0, -8216(%rbp)
	jne	.L6
	movq	-8256(%rbp), %rax
	addq	$16, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	perror@PLT
	movl	$-1, %eax
	jmp	.L8
.L7:
	movl	-8228(%rbp), %eax
	movslq	%eax, %rdx
	movq	-8216(%rbp), %rcx
	leaq	-8208(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	fwrite@PLT
.L6:
	movq	-8224(%rbp), %rdx
	leaq	-8208(%rbp), %rax
	movq	%rdx, %rcx
	movl	$8192, %edx
	movl	$1, %esi
	movq	%rax, %rdi
	call	fread@PLT
	movl	%eax, -8228(%rbp)
	cmpl	$0, -8228(%rbp)
	jg	.L7
	movq	-8216(%rbp), %rax
	movq	%rax, %rdi
	call	fclose@PLT
	movq	-8224(%rbp), %rax
	movq	%rax, %rdi
	call	fclose@PLT
	movl	$0, %eax
.L8:
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L9
	call	__stack_chk_fail@PLT
.L9:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.2.0-19ubuntu1) 11.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
