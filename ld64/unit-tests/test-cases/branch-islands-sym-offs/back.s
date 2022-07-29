

#if __ppc__ || __ppc64__

        .cstring
        .align 2
LC0:
        .ascii "back\12\0"

    .text
        .align 2
        .globl _back
_back:
        mflr r0
        stw r0,8(r1)
        bl saveFP+52 ; save f27-f31
#if __ppc__
	stwu r1,-96(r1)
#else
        stdu r1,-128(r1)
#endif

        bcl 20,31,L0$pb
L0$pb:
        mflr r31
        addis r2,r31,ha16(L___stdoutp$non_lazy_ptr-L0$pb)
#if __ppc__
        lwz r2,lo16(L___stdoutp$non_lazy_ptr-L0$pb)(r2)
        lwz r0,0(r2)
#else
        ld r2,lo16(L___stdoutp$non_lazy_ptr-L0$pb)(r2)
        ld r0,0(r2)
#endif
        addis r2,r31,ha16(LC0-L0$pb)
        la r3,lo16(LC0-L0$pb)(r2)
        li r4,1
        li r5,6
        mr r6,r0
#if __ppc__
        bl _fwrite$UNIX2003
        lwz r1,0(r1)
        lwz r0,8(r1)
#else
        bl _fwrite
	ld r1,0(r1)
	ld r0,16(r1)
#endif
	mtlr r0
	b restFP+52 ; restore f27-f31
	blr


        .non_lazy_symbol_pointer
L___stdoutp$non_lazy_ptr:
        .indirect_symbol ___stdoutp
#if __ppc__
        .long   0
#else
	.quad	0
#endif
        .subsections_via_symbols

#endif
