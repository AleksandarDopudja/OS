.global interrupt_routine
.global mem_alloc_kernel
.global mem_free_kernel
.global thread_create_kernel
.global thread_exit_kernel
.global thread_dispatch_kernel
.global running
.global console_handler1

interrupt_routine:

    addi sp, sp, -16
    sd t0, 0(sp)

     #cuvanje korsnickog sp
     ld t0, running
     sd sp, 8(t0)

     #novi ssp tj prelazak na sis stek
     ld sp, 16(t0)

    #cuvanje reg
        addi sp, sp, -256

        sd sp, 0(sp)
        sd ra, 8(sp)
        sd s0, 16(sp)
        sd s1, 24(sp)
        sd s2, 32(sp)
        sd s3, 40(sp)
        sd s4, 48(sp)
        sd s5, 56(sp)
        sd s6, 64(sp)
        sd s7, 72(sp)
        sd s8, 80(sp)
        sd s9, 88(sp)
        sd s10, 96(sp)
        sd s11, 104(sp)
        sd a0, 112(sp)
        sd a1, 120(sp)
        sd a2, 128(sp)
        sd a3, 136(sp)
        sd a4, 144(sp)
        sd a5, 152(sp)
        sd a6, 160(sp)
        sd a7, 168(sp)
        sd t0, 176(sp)
        sd t1, 184(sp)
        sd t2, 192(sp)
        sd t3, 200(sp)
        sd t4, 208(sp)
        sd t5, 216(sp)
        sd t6, 224(sp)
        sd zero, 232(sp)
        sd gp, 240(sp)
        sd tp, 248(sp)

        csrr t0, scause
        li t1, 8
        li t2, 9
        beq t0,t1, soft
        beq t0,t2, soft
hard:
       li t0,0x8000000000000009
       csrr t1, scause
       beq t0, t1, console
       call timer
       ld a0, 112(sp)
       #j thread_dispatch
        j end

console:

       call console_handler1

       ld a0, 112(sp)
       j end

soft:

    csrr t0, sepc
    addi t0, t0, 4 #mozda 8?
    csrw sepc, t0

    li t0, 1
    beq t0, a0, allocation
    li t0, 2
    beq t0, a0, free
    li t0, 0x11
    beq t0, a0, thread_create
    li t0, 0x12
    beq t0, a0, thread_exit
    li t0, 0x13
    beq t0, a0, thread_dispatch
    li t0, 0x21
    beq t0, a0, sem_open
    li t0, 0x22
    beq t0, a0, sem_close
    li t0, 0x23
    beq t0, a0, sem_wait
    li t0, 0x24
    beq t0, a0, sem_signal

allocation:

    addi sp, sp, -16
    sd ra, 0(sp)

    mv a0, a1

    call mem_alloc_kernel

    ld ra, 0(sp)
    addi sp, sp, 16

    j end

free:

    addi sp, sp, -16
    sd ra, 0(sp)

    mv a0, a1

    call mem_free_kernel

    ld ra, 0(sp)
    addi sp, sp, 16

    j end

thread_create:

    mv a0, a1
    mv a1, a2
    mv a2, a3
    mv a3, a4

    addi sp, sp, -16
    sd ra, 0(sp)

    call thread_create_kernel

    ld ra, 0(sp)
    addi sp, sp, 16

    j end

thread_exit:

    addi sp, sp, -16
    sd ra, 0(sp)

    call thread_exit_kernel

    ld ra, 0(sp)
    addi sp, sp, 16

    j end

thread_dispatch:

    ld t0, running
    sd sp, 16(t0)

    #jel moze kao za a0 dole posto je ra vec na sis steku
    addi sp, sp, -16
    sd ra, 0(sp)

    call thread_dispatch_kernel

    ld ra, 0(sp)
    addi sp, sp, 16

    ld t0, running
    ld sp, 16(t0)

    ld a0, 112(sp)

    j end

sem_signal:

    addi sp, sp, -16
    sd ra, 0(sp)

    mv a0, a1

    call sem_signal_kernel

    ld ra, 0(sp)
    addi sp, sp, 16

    j end

sem_wait:

    addi sp, sp, -16
    sd ra, 0(sp)

    mv a0, a1

    call sem_wait_kernel

    ld ra, 0(sp)
    addi sp, sp, 16

    j end

sem_close:

    addi sp, sp, -16
    sd ra, 0(sp)

    mv a0, a1

    call sem_close_kernel

    ld ra, 0(sp)
    addi sp, sp, 16

    j end

sem_open:

    addi sp, sp, -16
    sd ra, 0(sp)

    mv a0, a1
    mv a1, a2

    call sem_open_kernel

    ld ra, 0(sp)
    addi sp, sp, 16

    j end

end:
   #restauriranje
   #ld sp, 0(sp)
    ld ra, 8(sp)
    ld s0, 16(sp)
    ld s1, 24(sp)
    ld s2, 32(sp)
    ld s3, 40(sp)
    ld s4, 48(sp)
    ld s5, 56(sp)
    ld s6, 64(sp)
    ld s7, 72(sp)
    ld s8, 80(sp)
    ld s9, 88(sp)
    ld s10, 96(sp)
    ld s11, 104(sp)
   #ld a0, 112(sp)
    ld a1, 120(sp)
    ld a2, 128(sp)
    ld a3, 136(sp)
    ld a4, 144(sp)
    ld a5, 152(sp)
    ld a6, 160(sp)
    ld a7, 168(sp)
    ld t0, 176(sp)
    ld t1, 184(sp)
    ld t2, 192(sp)
    ld t3, 200(sp)
    ld t4, 208(sp)
    ld t5, 216(sp)
    ld t6, 224(sp)
    ld zero, 232(sp)
    ld gp, 240(sp)
    ld tp, 248(sp)

    addi sp, sp, 256

    #cuvanje ssp
    ld t0, running
    sd sp, 16(t0)

    #vracanje na kor stek
    ld sp, 8(t0)

    ld t0, 0(sp)

    addi sp,sp,16

    sret