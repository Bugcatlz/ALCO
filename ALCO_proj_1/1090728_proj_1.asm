.globl main
.data 
	Input:	.string"Input a number:\n"
	Output:	.string"The damage:\n"
.text
main:
	la  a0, Input		#將Input放數ao
	li  a7, 4		#PrintString Input
	ecall			#system call
	li  a7 5		#ReadInt存於a0
	ecall			#system call
	jal  ra, F		#F(a0)
End:
	mv  t2, a0		#t2=a0
	la  a0, Output		#將Output放於a0
	li  a7,4		#PrintString Output
	ecall			#system call
	mv  a0, t2		#a0=t2
	li  a7,1		#PrintInt a0
	ecall			#system call
	li  a7, 10		#End
	ecall
F:
	addi  sp, sp, -4	#r建立stack
	sw  ra, 0(sp)		#先將回傳的位置存起來
	addi t0, zero, 20	#t0 = 20
	ble  a0, t0, L2		#判斷是否小於等於20則去L2
	addi  sp, sp, -4	#建立stack
	sw  a0, 0(sp)		#將a0存在stack內
	addi  t2, zero, 5 	#t2=5
	div  a0, a0, t2		#a0=a0/5
	jal  ra, F		#F(a0/5)
	mv  t2, a0		#將F(a0/5)存在t2
	lw  a0, 0(sp)		#讀取原本的a0
	addi  t3, zero, 2	#t3=2
	mul  a0, a0, t3		#x=x*2
	add  a0, a0, t2		#a0=x*3+F(x/5)
	addi  sp, sp , 4	#回復stack
	j L7
L2:
	addi  t0 ,zero, 10	#t0=10
	ble  a0, t0, L3		#判斷a0是否小於等於10則去L3
	addi  t0, zero, 20	#t0=20
	bgt  a0, t0, L3		#判斷a0是否大於20則去L3
	addi  sp, sp, -8	#建立stack
	sw  a0, 4(sp)		#將a0存在stack內
	addi  a0, a0, -2	#a0=a0-2
	jal  ra, F		#F(x-2)
	sw  a0, 0(sp)		#將F(x-2)存到stack內
	lw  a0, 4(sp)		#讀取原本的a0
	addi  a0, a0, -3	#a0=a0-3
	jal  ra, F		#F(x-3)
	lw  t0, 0(sp)		#將F(x-2)存到t0
	add  a0, a0, t0		#a0=F(x-3)+F(x-2)
	addi  sp, sp, 8		#回復stack
	j L7
L3:
	addi  t0, zero, 1	#t0=1
	ble  a0, t0, L4		#判斷是否小於等於1則去L4
	addi  t0, zero, 10	#t0=10
	bgt  a0, t0, L4		#判斷是否大於10則去L4
	addi  sp, sp, -8	#建立stack
	sw  a0, 4(sp)		#將a0存在stack內
	addi  a0, a0, -1	#a0=a0-1
	jal  ra, F		#F(x-1)
	sw  a0, 0(sp)		#將F(x-1)存到stack內
	lw  a0, 4(sp)		#讀取原本的a0
	addi  a0, a0, -2	#a0=a0-2
	jal  ra, F		#F(x-2)
	lw  t0, 0(sp)		#將F(x-1)存到t0
	add  a0, a0, t0		#a0=F(x-3)+F(x-2)
	addi  sp, sp, 8		#回復stack
	j L7
L4:
	bne   a0, zero, L5	#判斷是否不等於0則去L5
	addi  a0, zero, 1	#a0=1
	j L7
L5:
	addi  t0, zero, 1	#t0=1
	bne   a0, t0, L6	#判斷是否不等於1則去L6
	addi  a0, zero, 5	#a0=5
	j L7
L6:
	addi  a0, zero, -1	#a0=-1
	j L7
L7:
	lw  ra, 0(sp)		#讀取原本的回傳位置
	addi  sp, sp, 4		#回復stack
	jalr  x0, ra, 0
