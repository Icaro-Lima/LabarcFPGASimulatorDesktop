/* Comentário igual a C
       isso: // nao funciona como comentario !
*/

/* Alguns exemplos de instruções para ilustrar a sintaxe */

.section .text
.globl main
main:
        addi    a2,zero,55
	add	a3,a2,a1
	addi	sp,zero,0x40
pula:
	lw	a1,8(sp)
        sw      a3,16(sp)
        beq     a2,a3,pula

