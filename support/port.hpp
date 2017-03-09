#ifndef PORT_HPP
#define PORT_HPP
/**
 * Partial C++ conversion of the netlib port library. 
 * 
 * http://www.netlib.org/port/
 */



#include <limits>
#include <cmath>
#include <iostream>
#include <cfloat>
#include <limits.h>
#include <vector>
#include <string>

namespace port {

    typedef long int integer;
    typedef long int logical;


#define TRUE_ (1)
#define FALSE_ (0)

    typedef long int flag;
    typedef long int ftnlen;
    typedef long int ftnint;

    typedef struct {
        flag cierr;
        ftnint ciunit;
        flag ciend;
        char *cifmt;
        ftnint cirec;
    } cilist;



    static integer ndfalt = 0;
    static integer c__2 = 2;
    static double c_b13 = 0.;
    static integer c_n1 = -1;
    static integer c__1 = 1;
    static double c_b33 = 1.;
    static double c_b44 = -1.;
    static double c_b32 = 1.;
    static double c_b38 = 1.;
    static double c_b43 = -1.;
    static double c_b5 = 0.;
    static double c_b23 = 1.;

    int stopx_(integer* i) {

        return 0;
    }

    static integer c__3 = 3;
    static integer c__4 = 4;
    static double c_b4 = .33333333333333331;
    static integer c__5 = 5;
    static integer c__6 = 6;

    static integer c__32767 = 32767;
    static integer c_b8 = 16777215;
    static integer c__16405 = 16405;
    static integer c_b12 = 9876536;
    static integer c__0 = 0;
    static integer c_b18 = 4194303;
    static integer c__9 = 9;
    static double c_b16 = 0.;
    static logical c_false = false;
    static double c_b40 = 1.;
    static double c_b45 = -1.;

    //
    //integer i1mach_(integer *i__)
    //{
    //    /* Initialized data */
    //
    //    static integer t3e[3] = { 9777664,5323660,46980 };
    //    static integer sc = 0;
    //
    //    /* Format strings */
    //    static char fmt_9010[] = "(/\002 Adjust autodoubled I1MACH by uncommenti"
    //	    "ng data\002/\002 statements appropriate for your machine and set"
    //	    "ting\002/\002 IMACH(I) = IMACH(I+3) for I = 11, 12, and 13.\002)";
    //    static char fmt_9020[] = "(/\002 Adjust I1MACH by uncommenting data stat"
    //	    "ements\002/\002 appropriate for your machine.\002)";
    //
    //    /* System generated locals */
    //    integer ret_val;
    //    static integer equiv_0[16];
    //    static double equiv_1[2];
    //
    //    /* Builtin functions */
    //    integer s_wsfe(cilist *), e_wsfe(void);
    //    /* Subroutine */ int s_stop(char *, ftnlen);
    //    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
    //	    e_wsle(void);
    //
    //    /* Local variables */
    //    static integer j, k, i3;
    //#define imach (equiv_0)
    //#define rmach (equiv_1)
    //    //extern /* Subroutine */ int i1mcr1_(integer *, integer *, integer *, 
    ////	    integer *, integer *);
    //#define small ((integer *)equiv_1)
    //#define output (equiv_0 + 3)
    //
    //    /* Fortran I/O blocks */
    //    static cilist io___7 = { 0, 6, 0, fmt_9010, 0 };
    //    static cilist io___11 = { 0, 6, 0, fmt_9020, 0 };
    //    static cilist io___12 = { 0, 6, 0, 0, 0 };
    //
    //
    //
    ///*    I1MACH( 1) = THE STANDARD INPUT UNIT. */
    ///*    I1MACH( 2) = THE STANDARD OUTPUT UNIT. */
    ///*    I1MACH( 3) = THE STANDARD PUNCH UNIT. */
    ///*    I1MACH( 4) = THE STANDARD ERROR MESSAGE UNIT. */
    ///*    I1MACH( 5) = THE NUMBER OF BITS PER INTEGER STORAGE UNIT. */
    ///*    I1MACH( 6) = THE NUMBER OF CHARACTERS PER CHARACTER STORAGE UNIT. */
    ///*    INTEGERS HAVE FORM SIGN ( X(S-1)*A**(S-1) + ... + X(1)*A + X(0) ) */
    ///*    I1MACH( 7) = A, THE BASE. */
    ///*    I1MACH( 8) = S, THE NUMBER OF BASE-A DIGITS. */
    ///*    I1MACH( 9) = A**S - 1, THE LARGEST MAGNITUDE. */
    ///*    FLOATS HAVE FORM  SIGN (B**E)*( (X(1)/B) + ... + (X(T)/B**T) ) */
    ///*               WHERE  EMIN .LE. E .LE. EMAX. */
    ///*    I1MACH(10) = B, THE BASE. */
    ///*  SINGLE-PRECISION */
    ///*    I1MACH(11) = T, THE NUMBER OF BASE-B DIGITS. */
    ///*    I1MACH(12) = EMIN, THE SMALLEST EXPONENT E. */
    ///*    I1MACH(13) = EMAX, THE LARGEST EXPONENT E. */
    ///*  DOUBLE-PRECISION */
    ///*    I1MACH(14) = T, THE NUMBER OF BASE-B DIGITS. */
    ///*    I1MACH(15) = EMIN, THE SMALLEST EXPONENT E. */
    ///*    I1MACH(16) = EMAX, THE LARGEST EXPONENT E. */
    //
    ///*  THIS VERSION ADAPTS AUTOMATICALLY TO MOST CURRENT MACHINES, */
    ///*  INCLUDING AUTO-DOUBLE COMPILERS. */
    ///*  TO COMPILE ON OLDER MACHINES, ADD A C IN COLUMN 1 */
    ///*  ON THE NEXT LINE */
    ///*  AND REMOVE THE C FROM COLUMN 1 IN ONE OF THE SECTIONS BELOW. */
    ///*  CONSTANTS FOR EVEN OLDER MACHINES CAN BE OBTAINED BY */
    ///*          mail netlib@research.bell-labs.com */
    ///*          send old1mach from blas */
    ///*  PLEASE SEND CORRECTIONS TO dmg OR ehg@bell-labs.com. */
    //
    ///*     MACHINE CONSTANTS FOR THE HONEYWELL DPS 8/70 SERIES. */
    //
    ///*      DATA IMACH( 1) /    5 / */
    ///*      DATA IMACH( 2) /    6 / */
    ///*      DATA IMACH( 3) /   43 / */
    ///*      DATA IMACH( 4) /    6 / */
    ///*      DATA IMACH( 5) /   36 / */
    ///*      DATA IMACH( 6) /    4 / */
    ///*      DATA IMACH( 7) /    2 / */
    ///*      DATA IMACH( 8) /   35 / */
    ///*      DATA IMACH( 9) / O377777777777 / */
    ///*      DATA IMACH(10) /    2 / */
    ///*      DATA IMACH(11) /   27 / */
    ///*      DATA IMACH(12) / -127 / */
    ///*      DATA IMACH(13) /  127 / */
    ///*      DATA IMACH(14) /   63 / */
    ///*      DATA IMACH(15) / -127 / */
    ///*      DATA IMACH(16) /  127 /, SC/987/ */
    //
    ///*     MACHINE CONSTANTS FOR PDP-11 FORTRANS SUPPORTING */
    ///*     32-BIT INTEGER ARITHMETIC. */
    //
    ///*      DATA IMACH( 1) /    5 / */
    ///*      DATA IMACH( 2) /    6 / */
    ///*      DATA IMACH( 3) /    7 / */
    ///*      DATA IMACH( 4) /    6 / */
    ///*      DATA IMACH( 5) /   32 / */
    ///*      DATA IMACH( 6) /    4 / */
    ///*      DATA IMACH( 7) /    2 / */
    ///*      DATA IMACH( 8) /   31 / */
    ///*      DATA IMACH( 9) / 2147483647 / */
    ///*      DATA IMACH(10) /    2 / */
    ///*      DATA IMACH(11) /   24 / */
    ///*      DATA IMACH(12) / -127 / */
    ///*      DATA IMACH(13) /  127 / */
    ///*      DATA IMACH(14) /   56 / */
    ///*      DATA IMACH(15) / -127 / */
    ///*      DATA IMACH(16) /  127 /, SC/987/ */
    //
    ///*     MACHINE CONSTANTS FOR THE UNIVAC 1100 SERIES. */
    //
    ///*     NOTE THAT THE PUNCH UNIT, I1MACH(3), HAS BEEN SET TO 7 */
    ///*     WHICH IS APPROPRIATE FOR THE UNIVAC-FOR SYSTEM. */
    ///*     IF YOU HAVE THE UNIVAC-FTN SYSTEM, SET IT TO 1. */
    //
    ///*      DATA IMACH( 1) /    5 / */
    ///*      DATA IMACH( 2) /    6 / */
    ///*      DATA IMACH( 3) /    7 / */
    ///*      DATA IMACH( 4) /    6 / */
    ///*      DATA IMACH( 5) /   36 / */
    ///*      DATA IMACH( 6) /    6 / */
    ///*      DATA IMACH( 7) /    2 / */
    ///*      DATA IMACH( 8) /   35 / */
    ///*      DATA IMACH( 9) / O377777777777 / */
    ///*      DATA IMACH(10) /    2 / */
    ///*      DATA IMACH(11) /   27 / */
    ///*      DATA IMACH(12) / -128 / */
    ///*      DATA IMACH(13) /  127 / */
    ///*      DATA IMACH(14) /   60 / */
    ///*      DATA IMACH(15) /-1024 / */
    ///*      DATA IMACH(16) / 1023 /, SC/987/ */
    //
    //    if (sc != 987) {
    ///*        *** CHECK FOR AUTODOUBLE *** */
    //	small[1] = 0;
    //	*rmach = 1e13f;
    //	if (small[1] != 0) {
    ///*           *** AUTODOUBLED *** */
    //	    if (small[0] == 1117925532 && small[1] == -448790528 || small[1] 
    //		    == 1117925532 && small[0] == -448790528) {
    ///*               *** IEEE *** */
    //		imach[9] = 2;
    //		imach[13] = 53;
    //		imach[14] = -1021;
    //		imach[15] = 1024;
    //	    } else if (small[0] == -2065213935 && small[1] == 10752) {
    ///*               *** VAX WITH D_FLOATING *** */
    //		imach[9] = 2;
    //		imach[13] = 56;
    //		imach[14] = -127;
    //		imach[15] = 127;
    //	    } else if (small[0] == 1267827943 && small[1] == 704643072) {
    ///*               *** IBM MAINFRAME *** */
    //		imach[9] = 16;
    //		imach[13] = 14;
    //		imach[14] = -64;
    //		imach[15] = 63;
    //	    } else {
    //		s_wsfe(&io___7);
    //		e_wsfe();
    //		s_stop("777", (ftnlen)3);
    //	    }
    //	    imach[10] = imach[13];
    //	    imach[11] = imach[14];
    //	    imach[12] = imach[15];
    //	} else {
    //	    *rmach = 1234567.f;
    //	    if (small[0] == 1234613304) {
    ///*               *** IEEE *** */
    //		imach[9] = 2;
    //		imach[10] = 24;
    //		imach[11] = -125;
    //		imach[12] = 128;
    //		imach[13] = 53;
    //		imach[14] = -1021;
    //		imach[15] = 1024;
    //		sc = 987;
    //	    } else if (small[0] == -1271379306) {
    ///*               *** VAX *** */
    //		imach[9] = 2;
    //		imach[10] = 24;
    //		imach[11] = -127;
    //		imach[12] = 127;
    //		imach[13] = 56;
    //		imach[14] = -127;
    //		imach[15] = 127;
    //		sc = 987;
    //	    } else if (small[0] == 1175639687) {
    ///*               *** IBM MAINFRAME *** */
    //		imach[9] = 16;
    //		imach[10] = 6;
    //		imach[11] = -64;
    //		imach[12] = 63;
    //		imach[13] = 14;
    //		imach[14] = -64;
    //		imach[15] = 63;
    //		sc = 987;
    //	    } else if (small[0] == 1251390520) {
    ///*              *** CONVEX C-1 *** */
    //		imach[9] = 2;
    //		imach[10] = 24;
    //		imach[11] = -128;
    //		imach[12] = 127;
    //		imach[13] = 53;
    //		imach[14] = -1024;
    //		imach[15] = 1023;
    //	    } else {
    //		for (i3 = 1; i3 <= 3; ++i3) {
    //		    j = small[0] / 10000000;
    //		    k = small[0] - j * 10000000;
    //		    if (k != t3e[i3 - 1]) {
    //			goto L20;
    //		    }
    //		    small[0] = j;
    ///* L10: */
    //		}
    ///*              *** CRAY T3E *** */
    //		imach[0] = 5;
    //		imach[1] = 6;
    //		imach[2] = 0;
    //		imach[3] = 0;
    //		imach[4] = 64;
    //		imach[5] = 8;
    //		imach[6] = 2;
    //		imach[7] = 63;
    //		i1mcr1_(&imach[8], &k, &c__32767, &c_b8, &c_b8);
    //		imach[9] = 2;
    //		imach[10] = 53;
    //		imach[11] = -1021;
    //		imach[12] = 1024;
    //		imach[13] = 53;
    //		imach[14] = -1021;
    //		imach[15] = 1024;
    //		goto L35;
    //L20:
    //		i1mcr1_(&j, &k, &c__16405, &c_b12, &c__0);
    //		if (small[0] != j) {
    //		    s_wsfe(&io___11);
    //		    e_wsfe();
    //		    s_stop("777", (ftnlen)3);
    //		}
    ///*              *** CRAY 1, XMP, 2, AND 3 *** */
    //		imach[0] = 5;
    //		imach[1] = 6;
    //		imach[2] = 102;
    //		imach[3] = 6;
    //		imach[4] = 46;
    //		imach[5] = 8;
    //		imach[6] = 2;
    //		imach[7] = 45;
    //		i1mcr1_(&imach[8], &k, &c__0, &c_b18, &c_b8);
    //		imach[9] = 2;
    //		imach[10] = 47;
    //		imach[11] = -8188;
    //		imach[12] = 8189;
    //		imach[13] = 94;
    //		imach[14] = -8141;
    //		imach[15] = 8189;
    //		goto L35;
    //	    }
    //	}
    //	imach[0] = 5;
    //	imach[1] = 6;
    //	imach[2] = 7;
    //	imach[3] = 6;
    //	imach[4] = 32;
    //	imach[5] = 4;
    //	imach[6] = 2;
    //	imach[7] = 31;
    //	imach[8] = 2147483647;
    //L35:
    //	sc = 987;
    //    }
    //    if (*i__ < 1 || *i__ > 16) {
    //	goto L40;
    //    }
    //    ret_val = imach[*i__ - 1];
    //    return ret_val;
    //L40:
    //    s_wsle(&io___12);
    //    do_lio(&c__9, &c__1, "I1MACH(I): I =", (ftnlen)14);
    //    do_lio(&c__3, &c__1, (char *)&(*i__), (ftnlen)sizeof(integer));
    //    do_lio(&c__9, &c__1, " is out of bounds.", (ftnlen)18);
    //    e_wsle();
    //    s_stop("", (ftnlen)0);
    ///* /+ C source for I1MACH -- remove the * in column 1 +/ */
    ///* /+ Note that some values may need changing. +/ */
    ///* #include <stdio.h> */
    ///* #include <float.h> */
    ///* #include <limits.h> */
    ///* #include <math.h> */
    //

    inline long i1mach_(long *i) {
        switch (*i) {
            case 1: return 5;
            case 2: return 6;
            case 3: return 7;
            case 4: return 0;
            case 5: return 32;
            case 6: return sizeof (int);
            case 7: return 2;
            case 8: return 31;
            case 9: return LONG_MAX;
            case 10: return FLT_RADIX;
            case 11: return FLT_MANT_DIG;
            case 12: return FLT_MIN_EXP;
            case 13: return FLT_MAX_EXP;
            case 14: return DBL_MANT_DIG;
            case 15: return DBL_MIN_EXP;
            case 16: return DBL_MAX_EXP;
        }
        fprintf(stderr, "invalid argument: i1mach(%ld)\n", *i);
        exit(1);
        return 0;
    }
    //    return ret_val;
    //} /* i1mach_ */

    template<typename T>
    inline T d1mach_(long *i) {
        switch (*i) {
            case 1: return std::numeric_limits<T>::min();
            case 2: return std::numeric_limits<T>::max();
            case 3: return std::numeric_limits<T>::epsilon() / std::numeric_limits<T>::radix;
            case 4: return std::numeric_limits<T>::epsilon();
            case 5: return std::log10(std::numeric_limits<T>::radix);
        }
        std::cerr << "invalid argument: d1mach(%ld)\n" << *i;
        exit(1);
        return 0;
    }

    //

    integer i7mdcn_(integer *k) {
        /* Initialized data */

        static integer mdperm[3] = {2, 4, 1};

        /* System generated locals */
        integer ret_val;

        /* Local variables */
        //extern integer i1mach_(integer *);



        /*  ***  RETURN INTEGER MACHINE-DEPENDENT CONSTANTS  *** */

        /*     ***  K = 1 MEANS RETURN STANDARD OUTPUT UNIT NUMBER.   *** */
        /*     ***  K = 2 MEANS RETURN ALTERNATE OUTPUT UNIT NUMBER.  *** */
        /*     ***  K = 3 MEANS RETURN  INPUT UNIT NUMBER.            *** */
        /*          (NOTE -- K = 2, 3 ARE USED ONLY BY TEST PROGRAMS.) */

        /*  +++  PORT VERSION FOLLOWS... */
        ret_val = i1mach_(&mdperm[(0 + (0 + ((*k - 1) << 2))) / 4]);
        /*  +++  END OF PORT VERSION  +++ */

        /*  +++  NON-PORT VERSION FOLLOWS... */
        /*     INTEGER MDCON(3) */
        /*     DATA MDCON(1)/6/, MDCON(2)/8/, MDCON(3)/5/ */
        /*     I7MDCN = MDCON(K) */
        /*  +++  END OF NON-PORT VERSION  +++ */

        /* L999: */
        return ret_val;
        /*  ***  LAST CARD OF I7MDCN FOLLOWS  *** */
    } /* i7mdcn_ */

    template<typename doublereal>
    doublereal dr7mdc_(integer *k) {
        /* Initialized data */

        static doublereal big = 0.;
        static doublereal eta = 0.;
        static doublereal machep = 0.;
        static doublereal zero = 0.;

        /* System generated locals */
        doublereal ret_val;

        /* Builtin functions */
        //    double sqrt(doublereal);

        /* Local variables */
        //    extern doublereal d1mach_(integer *);


        /*  ***  RETURN MACHINE DEPENDENT CONSTANTS USED BY NL2SOL  *** */


        /*  ***  THE CONSTANT RETURNED DEPENDS ON K... */

        /*  ***        K = 1... SMALLEST POS. ETA SUCH THAT -ETA EXISTS. */
        /*  ***        K = 2... SQUARE ROOT OF ETA. */
        /*  ***        K = 3... UNIT ROUNDOFF = SMALLEST POS. NO. MACHEP SUCH */
        /*  ***                 THAT 1 + MACHEP .GT. 1 .AND. 1 - MACHEP .LT. 1. */
        /*  ***        K = 4... SQUARE ROOT OF MACHEP. */
        /*  ***        K = 5... SQUARE ROOT OF BIG (SEE K = 6). */
        /*  ***        K = 6... LARGEST MACHINE NO. BIG SUCH THAT -BIG EXISTS. */

        /* /+ */
        /* / */

        if (big > zero) {
            goto L1;
        }
        big = d1mach_<doublereal>(&c__2);
        eta = d1mach_<doublereal>(&c__1);
        machep = d1mach_<doublereal>(&c__4);
L1:

        /* -------------------------------  BODY  -------------------------------- */

        switch (*k) {
            case 1: goto L10;
            case 2: goto L20;
            case 3: goto L30;
            case 4: goto L40;
            case 5: goto L50;
            case 6: goto L60;
        }

L10:
        ret_val = eta;
        goto L999;

L20:
        ret_val = std::sqrt(eta * 256.) / 16.;
        goto L999;

L30:
        ret_val = machep;
        goto L999;

L40:
        ret_val = std::sqrt(machep);
        goto L999;

L50:
        ret_val = std::sqrt(big / 256.) * 16.;
        goto L999;

L60:
        ret_val = big;

L999:
        return ret_val;
        /*  ***  LAST CARD OF DR7MDC FOLLOWS  *** */
    } /* dr7mdc_ */

    //

    template<typename doublereal>
    int dv7dfl_(integer *alg, integer *lv, doublereal *v) {
        /* System generated locals */
        doublereal d__1, d__2, d__3;

        /* Builtin functions */
        //    double pow_dd(doublereal *, doublereal *);

        /* Local variables */
        //extern doublereal dr7mdc_(integer *);
        static doublereal machep, mepcrt, sqteps;


        /*  ***  SUPPLY ***SOL (VERSION 2.3) DEFAULT VALUES TO V  *** */

        /*  ***  ALG = 1 MEANS REGRESSION CONSTANTS. */
        /*  ***  ALG = 2 MEANS GENERAL UNCONSTRAINED OPTIMIZATION CONSTANTS. */


        /* DR7MDC... RETURNS MACHINE-DEPENDENT CONSTANTS */


        /*  ***  SUBSCRIPTS FOR V  *** */


        /* /6 */
        /*     DATA ONE/1.D+0/, THREE/3.D+0/ */
        /* /7 */
        /* / */

        /*  ***  V SUBSCRIPT VALUES  *** */

        /* /6 */
        /*     DATA AFCTOL/31/, BIAS/43/, COSMIN/47/, DECFAC/22/, DELTA0/44/, */
        /*    1     DFAC/41/, DINIT/38/, DLTFDC/42/, DLTFDJ/43/, DTINIT/39/, */
        /*    2     D0INIT/40/, EPSLON/19/, ETA0/42/, FUZZ/45/, HUBERC/48/, */
        /*    3     INCFAC/23/, LMAX0/35/, LMAXS/36/, PHMNFC/20/, PHMXFC/21/, */
        /*    4     RDFCMN/24/, RDFCMX/25/, RFCTOL/32/, RLIMIT/46/, RSPTOL/49/, */
        /*    5     SCTOL/37/, SIGMIN/50/, TUNER1/26/, TUNER2/27/, TUNER3/28/, */
        /*    6     TUNER4/29/, TUNER5/30/, XCTOL/33/, XFTOL/34/ */
        /* /7 */
        /* / */

        /* -------------------------------  BODY  -------------------------------- */

        /* Parameter adjustments */
        --v;

        /* Function Body */
        machep = dr7mdc_<doublereal>(&c__3);
        v[31] = 1e-20;
        if (machep > 1e-10) {
            /* Computing 2nd power */
            d__1 = machep;
            v[31] = d__1 * d__1;
        }
        v[22] = .5;
        sqteps = dr7mdc_<doublereal>(&c__4);
        v[41] = .6;
        v[39] = 1e-6;
        mepcrt = std::pow(machep, c_b4);
        v[40] = 1.;
        v[19] = .1;
        v[23] = 2.;
        v[35] = 1.;
        v[36] = 1.;
        v[20] = -.1;
        v[21] = .1;
        v[24] = .1;
        v[25] = 4.;
        /* Computing MAX */
        /* Computing 2nd power */
        d__3 = mepcrt;
        d__1 = 1e-10, d__2 = d__3 * d__3;
        v[32] = std::max(d__1, d__2);
        v[37] = v[32];
        v[26] = .1;
        v[27] = 1e-4;
        v[28] = .75;
        v[29] = .5;
        v[30] = .75;
        v[33] = sqteps;
        v[34] = machep * 100.;

        if (*alg >= 2) {
            goto L10;
        }

        /*  ***  REGRESSION  VALUES */

        /* Computing MAX */
        d__1 = 1e-6, d__2 = machep * 100.;
        v[47] = std::max(d__1, d__2);
        v[38] = 0.;
        v[44] = sqteps;
        v[42] = mepcrt;
        v[43] = sqteps;
        v[45] = 1.5;
        v[48] = .7;
        v[46] = dr7mdc_<doublereal>(&c__5);
        v[49] = .001;
        v[50] = 1e-4;
        goto L999;

        /*  ***  GENERAL OPTIMIZATION VALUES */

L10:
        v[43] = .8;
        v[38] = -1.;
        v[42] = machep * 1e3;

L999:
        return 0;
        /*  ***  LAST CARD OF DV7DFL FOLLOWS  *** */
    } /* dv7dfl_ */
    //

    template<typename doublereal>
    inline int divset_(integer *alg, integer *iv, integer *liv, integer
            *lv, doublereal *v) {
        /* Initialized data */

        static integer miniv[4] = {82, 59, 103, 103};
        static integer minv[4] = {98, 71, 101, 85};

        static integer mv, miv, alg1;
        //extern integer i7mdcn_(integer *);
        //extern /* Subroutine */ int dv7dfl_(integer *, integer *, doublereal *);


        /*  ***  SUPPLY ***SOL (VERSION 2.3) DEFAULT VALUES TO IV AND V  *** */

        /*  ***  ALG = 1 MEANS REGRESSION CONSTANTS. */
        /*  ***  ALG = 2 MEANS GENERAL UNCONSTRAINED OPTIMIZATION CONSTANTS. */


        /* I7MDCN... RETURNS MACHINE-DEPENDENT INTEGER CONSTANTS. */
        /* DV7DFL.... PROVIDES DEFAULT VALUES TO V. */


        /*  ***  SUBSCRIPTS FOR IV  *** */


        /*  ***  IV SUBSCRIPT VALUES  *** */

        /* /6 */
        /*     DATA ALGSAV/51/, COVPRT/14/, COVREQ/15/, DRADPR/101/, DTYPE/16/, */
        /*    1     HC/71/, IERR/75/, INITH/25/, INITS/25/, IPIVOT/76/, */
        /*    2     IVNEED/3/, LASTIV/44/, LASTV/45/, LMAT/42/, MXFCAL/17/, */
        /*    3     MXITER/18/, NFCOV/52/, NGCOV/53/, NVDFLT/50/, NVSAVE/9/, */
        /*    4     OUTLEV/19/, PARPRT/20/, PARSAV/49/, PERM/58/, PRUNIT/21/, */
        /*    5     QRTYP/80/, RDREQ/57/, RMAT/78/, SOLPRT/22/, STATPR/23/, */
        /*    6     VNEED/4/, VSAVE/60/, X0PRT/24/ */
        /* /7 */
        /* / */
        /* Parameter adjustments */
        --iv;
        --v;

        /* Function Body */

        /* -------------------------------  BODY  -------------------------------- */

        if (21 <= *liv) {
            iv[21] = i7mdcn_(&c__1);
        }
        if (51 <= *liv) {
            iv[51] = *alg;
        }
        if (*alg < 1 || *alg > 4) {
            goto L40;
        }
        miv = miniv[*alg - 1];
        if (*liv < miv) {
            goto L20;
        }
        mv = minv[*alg - 1];
        if (*lv < mv) {
            goto L30;
        }
        alg1 = (*alg - 1) % 2 + 1;
        dv7dfl_(&alg1, lv, &v[1]);
        iv[1] = 12;
        if (*alg > 2) {
            iv[101] = 1;
        }
        iv[3] = 0;
        iv[44] = miv;
        iv[45] = mv;
        iv[42] = mv + 1;
        iv[17] = 200;
        iv[18] = 150;
        iv[19] = 1;
        iv[20] = 1;
        iv[58] = miv + 1;
        iv[22] = 1;
        iv[23] = 1;
        iv[4] = 0;
        iv[24] = 1;

        if (alg1 >= 2) {
            goto L10;
        }

        /*  ***  REGRESSION  VALUES */

        iv[14] = 3;
        iv[15] = 1;
        iv[16] = 1;
        iv[71] = 0;
        iv[75] = 0;
        iv[25] = 0;
        iv[76] = 0;
        iv[50] = 32;
        iv[60] = 58;
        if (*alg > 2) {
            iv[60] += 3;
        }
        iv[49] = iv[60] + 9;
        iv[80] = 1;
        iv[57] = 3;
        iv[78] = 0;
        goto L999;

        /*  ***  GENERAL OPTIMIZATION VALUES */

L10:
        iv[16] = 0;
        iv[25] = 1;
        iv[52] = 0;
        iv[53] = 0;
        iv[50] = 25;
        iv[49] = 47;
        if (*alg > 2) {
            iv[49] = 61;
        }
        goto L999;

L20:
        iv[1] = 15;
        goto L999;

L30:
        iv[1] = 16;
        goto L999;

L40:
        iv[1] = 67;

L999:
        return 0;
        /*  ***  LAST CARD OF DIVSET FOLLOWS  *** */
    } /* divset_ */

    template<typename doublereal>
    inline int dv7scp_(integer *p, doublereal *y, doublereal *s) {
        /* System generated locals */
        integer i__1;

        /* Local variables */
        static integer i__;


        /*  ***  SET P-VECTOR Y TO SCALAR S  *** */



        /* Parameter adjustments */
        --y;

        /* Function Body */
        i__1 = *p;
        for (i__ = 1; i__ <= i__1; ++i__) {
            /* L10: */
            y[i__] = *s;
        }
        return 0;
    } /* dv7scp_ */

    template<typename doublereal>
    inline int dv7vmp_(integer *n, doublereal *x, doublereal *y,
            doublereal *z__, integer *k) {
        /* System generated locals */
        integer i__1;

        /* Local variables */
        static integer i__;


        /* ***  SET X(I) = Y(I) * Z(I)**K, 1 .LE. I .LE. N (FOR K = 1 OR -1)  *** */


        /* Parameter adjustments */
        --z__;
        --y;
        --x;

        /* Function Body */
        if (*k >= 0) {
            goto L20;
        }
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            /* L10: */
            x[i__] = y[i__] / z__[i__];
        }
        goto L999;

L20:
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            /* L30: */
            x[i__] = y[i__] * z__[i__];
        }
L999:
        return 0;
        /*  ***  LAST CARD OF DV7VMP FOLLOWS  *** */
    } /* dv7vmp_ */

    template<typename doublereal>
    doublereal dv2nrm_(integer *p, doublereal *x) {
        /* Initialized data */

        static doublereal sqteta = 0.;

        /* System generated locals */
        integer i__1;
        doublereal ret_val, d__1;

        /* Builtin functions */
        //        double sqrt(doublereal);

        /* Local variables */
        static integer i__, j;
        static doublereal r__, t, xi, scale;
        //        extern doublereal dr7mdc_(integer *);


        /*  ***  RETURN THE 2-NORM OF THE P-VECTOR X, TAKING  *** */
        /*  ***  CARE TO AVOID THE MOST LIKELY UNDERFLOWS.    *** */


        /* /+ */
        /* / */

        /* /6 */
        /*     DATA ONE/1.D+0/, ZERO/0.D+0/ */
        /* /7 */
        /* / */
        /* Parameter adjustments */
        --x;

        /* Function Body */

        if (*p > 0) {
            goto L10;
        }
        ret_val = 0.;
        goto L999;
L10:
        i__1 = *p;
        for (i__ = 1; i__ <= i__1; ++i__) {
            if (x[i__] != 0.) {
                goto L30;
            }
            /* L20: */
        }
        ret_val = 0.;
        goto L999;

L30:
        scale = (d__1 = x[i__], std::fabs(d__1));
        if (i__ < *p) {
            goto L40;
        }
        ret_val = scale;
        goto L999;
L40:
        t = 1.;
        if (sqteta == 0.) {
            sqteta = dr7mdc_<doublereal>(&c__2);
        }

        /*     ***  SQTETA IS (SLIGHTLY LARGER THAN) THE SQUARE ROOT OF THE */
        /*     ***  SMALLEST POSITIVE FLOATING POINT NUMBER ON THE MACHINE. */
        /*     ***  THE TESTS INVOLVING SQTETA ARE DONE TO PREVENT UNDERFLOWS. */

        j = i__ + 1;
        i__1 = *p;
        for (i__ = j; i__ <= i__1; ++i__) {
            xi = (d__1 = x[i__], std::fabs(d__1));
            if (xi > scale) {
                goto L50;
            }
            r__ = xi / scale;
            if (r__ > sqteta) {
                t += r__ * r__;
            }
            goto L60;
L50:
            r__ = scale / xi;
            if (r__ <= sqteta) {
                r__ = 0.;
            }
            t = t * r__ * r__ + 1.;
            scale = xi;
L60:
            ;
        }

        ret_val = scale * std::sqrt(t);
L999:
        return ret_val;
        /*  ***  LAST LINE OF DV2NRM FOLLOWS  *** */
    } /* dv2nrm_ */

    template<typename doublereal>
    inline int dv7cpy_(integer *p, doublereal *y, doublereal *x) {
        /* System generated locals */
        integer i__1;

        /* Local variables */
        static integer i__;


        /*  ***  SET Y = X, WHERE X AND Y ARE P-VECTORS  *** */



        /* Parameter adjustments */
        --x;
        --y;

        /* Function Body */
        i__1 = *p;
        for (i__ = 1; i__ <= i__1; ++i__) {
            /* L10: */
            y[i__] = x[i__];
        }
        return 0;
    } /* dv7cpy_ */

    template<typename doublereal>
    doublereal dd7tpr_(integer *p, doublereal *x, doublereal *y) {
        /* Initialized data */

        static doublereal sqteta = 0.;

        /* System generated locals */
        integer i__1;
        doublereal ret_val, d__1, d__2, d__3, d__4;

        /* Local variables */
        static integer i__;
        static doublereal t;
        //        extern doublereal dr7mdc_(integer *);


        /*  ***  RETURN THE INNER PRODUCT OF THE P-VECTORS X AND Y.  *** */



        /*  ***  DR7MDC(2) RETURNS A MACHINE-DEPENDENT CONSTANT, SQTETA, WHICH */
        /*  ***  IS SLIGHTLY LARGER THAN THE SMALLEST POSITIVE NUMBER THAT */
        /*  ***  CAN BE SQUARED WITHOUT UNDERFLOWING. */

        /* /6 */
        /*     DATA ONE/1.D+0/, SQTETA/0.D+0/, ZERO/0.D+0/ */
        /* /7 */
        /* Parameter adjustments */
        --y;
        --x;

        /* Function Body */
        /* / */

        ret_val = 0.;
        if (*p <= 0) {
            goto L999;
        }
        if (sqteta == 0.) {
            sqteta = dr7mdc_<doublereal>(&c__2);
        }
        i__1 = *p;
        for (i__ = 1; i__ <= i__1; ++i__) {
            /* Computing MAX */
            d__3 = (d__1 = x[i__], std::fabs(d__1)), d__4 = (d__2 = y[i__], std::fabs(d__2));
            t = std::max(d__3, d__4);
            if (t > 1.) {
                goto L10;
            }
            if (t < sqteta) {
                goto L20;
            }
            t = x[i__] / sqteta * y[i__];
            if (std::fabs(t) < sqteta) {
                goto L20;
            }
L10:
            ret_val += x[i__] * y[i__];
L20:
            ;
        }

L999:
        return ret_val;
        /*  ***  LAST LINE OF DD7TPR FOLLOWS  *** */
    } /* dd7tpr_ */

    template<typename doublereal>
    inline int dl7ivm_(integer *n, doublereal *x, doublereal *l,
            doublereal *y) {
        /* System generated locals */
        integer i__1, i__2;

        /* Local variables */
        static integer i__, j, k;
        static doublereal t;
        //        extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);


        /*  ***  SOLVE  L*X = Y, WHERE  L  IS AN  N X N  LOWER TRIANGULAR */
        /*  ***  MATRIX STORED COMPACTLY BY ROWS.  X AND Y MAY OCCUPY THE SAME */
        /*  ***  STORAGE.  *** */

        /* /6 */
        /*     DATA ZERO/0.D+0/ */
        /* /7 */
        /* / */

        /* Parameter adjustments */
        --y;
        --x;
        --l;

        /* Function Body */
        i__1 = *n;
        for (k = 1; k <= i__1; ++k) {
            if (y[k] != 0.) {
                goto L20;
            }
            x[k] = 0.;
            /* L10: */
        }
        goto L999;
L20:
        j = k * (k + 1) / 2;
        x[k] = y[k] / l[j];
        if (k >= *n) {
            goto L999;
        }
        ++k;
        i__1 = *n;
        for (i__ = k; i__ <= i__1; ++i__) {
            i__2 = i__ - 1;
            t = dd7tpr_(&i__2, &l[j + 1], &x[1]);
            j += i__;
            x[i__] = (y[i__] - t) / l[j];
            /* L30: */
        }
L999:
        return 0;
        /*  ***  LAST CARD OF DL7IVM FOLLOWS  *** */
    } /* dl7ivm_ */

    template<typename doublereal>
    int dl7itv_(integer *n, doublereal *x, doublereal *l,
            doublereal *y) {
        /* System generated locals */
        integer i__1, i__2;

        /* Local variables */
        static integer i__, j, i0, ii, ij;
        static doublereal xi;
        static integer im1, np1;


        /*  ***  SOLVE  (L**T)*X = Y,  WHERE  L  IS AN  N X N  LOWER TRIANGULAR */
        /*  ***  MATRIX STORED COMPACTLY BY ROWS.  X AND Y MAY OCCUPY THE SAME */
        /*  ***  STORAGE.  *** */

        /* /6 */
        /*     DATA ZERO/0.D+0/ */
        /* /7 */
        /* / */

        /* Parameter adjustments */
        --y;
        --x;
        --l;

        /* Function Body */
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            /* L10: */
            x[i__] = y[i__];
        }
        np1 = *n + 1;
        i0 = *n * (*n + 1) / 2;
        i__1 = *n;
        for (ii = 1; ii <= i__1; ++ii) {
            i__ = np1 - ii;
            xi = x[i__] / l[i0];
            x[i__] = xi;
            if (i__ <= 1) {
                goto L999;
            }
            i0 -= i__;
            if (xi == 0.) {
                goto L30;
            }
            im1 = i__ - 1;
            i__2 = im1;
            for (j = 1; j <= i__2; ++j) {
                ij = i0 + j;
                x[j] -= xi * l[ij];
                /* L20: */
            }
L30:
            ;
        }
L999:
        return 0;
        /*  ***  LAST CARD OF DL7ITV FOLLOWS  *** */
    } /* dl7itv_ */

    template<typename doublereal>
    int dl7tvm_(integer *n, doublereal *x, doublereal *l,
            doublereal *y) {
        /* System generated locals */
        integer i__1, i__2;

        /* Local variables */
        static integer i__, j, i0, ij;
        static doublereal yi;


        /*  ***  COMPUTE  X = (L**T)*Y, WHERE  L  IS AN  N X N  LOWER */
        /*  ***  TRIANGULAR MATRIX STORED COMPACTLY BY ROWS.  X AND Y MAY */
        /*  ***  OCCUPY THE SAME STORAGE.  *** */

        /*     DIMENSION L(N*(N+1)/2) */
        /* /6 */
        /*     DATA ZERO/0.D+0/ */
        /* /7 */
        /* / */

        /* Parameter adjustments */
        --y;
        --x;
        --l;

        /* Function Body */
        i0 = 0;
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            yi = y[i__];
            x[i__] = 0.;
            i__2 = i__;
            for (j = 1; j <= i__2; ++j) {
                ij = i0 + j;
                x[j] += yi * l[ij];
                /* L10: */
            }
            i0 += i__;
            /* L20: */
        }
        /* L999: */
        return 0;
        /*  ***  LAST CARD OF DL7TVM FOLLOWS  *** */
    } /* dl7tvm_ */

    template<typename doublereal>
    int dd7dog_(doublereal *dig, integer *lv, integer *n,
            doublereal *nwtstp, doublereal *step, doublereal *v) {
        /* System generated locals */
        integer i__1;
        doublereal d__1;

        /* Builtin functions */
        //        double sqrt(doublereal);

        /* Local variables */
        static integer i__;
        static doublereal t, t1, t2, cfact, relax, cnorm, gnorm, rlambd, ghinvg,
                femnsq, ctrnwt, nwtnrm;


        /*  ***  COMPUTE DOUBLE DOGLEG STEP  *** */

        /*  ***  PARAMETER DECLARATIONS  *** */


        /*  ***  PURPOSE  *** */

        /*        THIS SUBROUTINE COMPUTES A CANDIDATE STEP (FOR USE IN AN UNCON- */
        /*     STRAINED MINIMIZATION CODE) BY THE DOUBLE DOGLEG ALGORITHM OF */
        /*     DENNIS AND MEI (REF. 1), WHICH IS A VARIATION ON POWELL*S DOGLEG */
        /*     SCHEME (REF. 2, P. 95). */

        /* --------------------------  PARAMETER USAGE  -------------------------- */

        /*    DIG (INPUT) DIAG(D)**-2 * G -- SEE ALGORITHM NOTES. */
        /*      G (INPUT) THE CURRENT GRADIENT VECTOR. */
        /*     LV (INPUT) LENGTH OF V. */
        /*      N (INPUT) NUMBER OF COMPONENTS IN  DIG, G, NWTSTP,  AND  STEP. */
        /* NWTSTP (INPUT) NEGATIVE NEWTON STEP -- SEE ALGORITHM NOTES. */
        /*   STEP (OUTPUT) THE COMPUTED STEP. */
        /*      V (I/O) VALUES ARRAY, THE FOLLOWING COMPONENTS OF WHICH ARE */
        /*             USED HERE... */
        /* V(BIAS)   (INPUT) BIAS FOR RELAXED NEWTON STEP, WHICH IS V(BIAS) OF */
        /*             THE WAY FROM THE FULL NEWTON TO THE FULLY RELAXED NEWTON */
        /*             STEP.  RECOMMENDED VALUE = 0.8 . */
        /* V(DGNORM) (INPUT) 2-NORM OF DIAG(D)**-1 * G -- SEE ALGORITHM NOTES. */
        /* V(DSTNRM) (OUTPUT) 2-NORM OF DIAG(D) * STEP, WHICH IS V(RADIUS) */
        /*             UNLESS V(STPPAR) = 0 -- SEE ALGORITHM NOTES. */
        /* V(DST0) (INPUT) 2-NORM OF DIAG(D) * NWTSTP -- SEE ALGORITHM NOTES. */
        /* V(GRDFAC) (OUTPUT) THE COEFFICIENT OF  DIG  IN THE STEP RETURNED -- */
        /*             STEP(I) = V(GRDFAC)*DIG(I) + V(NWTFAC)*NWTSTP(I). */
        /* V(GTHG)   (INPUT) SQUARE-ROOT OF (DIG**T) * (HESSIAN) * DIG -- SEE */
        /*             ALGORITHM NOTES. */
        /* V(GTSTEP) (OUTPUT) INNER PRODUCT BETWEEN G AND STEP. */
        /* V(NREDUC) (OUTPUT) FUNCTION REDUCTION PREDICTED FOR THE FULL NEWTON */
        /*             STEP. */
        /* V(NWTFAC) (OUTPUT) THE COEFFICIENT OF  NWTSTP  IN THE STEP RETURNED -- */
        /*             SEE V(GRDFAC) ABOVE. */
        /* V(PREDUC) (OUTPUT) FUNCTION REDUCTION PREDICTED FOR THE STEP RETURNED. */
        /* V(RADIUS) (INPUT) THE TRUST REGION RADIUS.  D TIMES THE STEP RETURNED */
        /*             HAS 2-NORM V(RADIUS) UNLESS V(STPPAR) = 0. */
        /* V(STPPAR) (OUTPUT) CODE TELLING HOW STEP WAS COMPUTED... 0 MEANS A */
        /*             FULL NEWTON STEP.  BETWEEN 0 AND 1 MEANS V(STPPAR) OF THE */
        /*             WAY FROM THE NEWTON TO THE RELAXED NEWTON STEP.  BETWEEN */
        /*             1 AND 2 MEANS A TRUE DOUBLE DOGLEG STEP, V(STPPAR) - 1 OF */
        /*             THE WAY FROM THE RELAXED NEWTON TO THE CAUCHY STEP. */
        /*             GREATER THAN 2 MEANS 1 / (V(STPPAR) - 1) TIMES THE CAUCHY */
        /*             STEP. */

        /* -------------------------------  NOTES  ------------------------------- */

        /*  ***  ALGORITHM NOTES  *** */

        /*        LET  G  AND  H  BE THE CURRENT GRADIENT AND HESSIAN APPROXIMA- */
        /*     TION RESPECTIVELY AND LET D BE THE CURRENT SCALE VECTOR.  THIS */
        /*     ROUTINE ASSUMES DIG = DIAG(D)**-2 * G  AND  NWTSTP = H**-1 * G. */
        /*     THE STEP COMPUTED IS THE SAME ONE WOULD GET BY REPLACING G AND H */
        /*     BY  DIAG(D)**-1 * G  AND  DIAG(D)**-1 * H * DIAG(D)**-1, */
        /*     COMPUTING STEP, AND TRANSLATING STEP BACK TO THE ORIGINAL */
        /*     VARIABLES, I.E., PREMULTIPLYING IT BY DIAG(D)**-1. */

        /*  ***  REFERENCES  *** */

        /* 1.  DENNIS, J.E., AND MEI, H.H.W. (1979), TWO NEW UNCONSTRAINED OPTI- */
        /*             MIZATION ALGORITHMS WHICH USE FUNCTION AND GRADIENT */
        /*             VALUES, J. OPTIM. THEORY APPLIC. 28, PP. 453-482. */
        /* 2. POWELL, M.J.D. (1970), A HYBRID METHOD FOR NON-LINEAR EQUATIONS, */
        /*             IN NUMERICAL METHODS FOR NON-LINEAR EQUATIONS, EDITED BY */
        /*             P. RABINOWITZ, GORDON AND BREACH, LONDON. */

        /*  ***  GENERAL  *** */

        /*     CODED BY DAVID M. GAY. */
        /*     THIS SUBROUTINE WAS WRITTEN IN CONNECTION WITH RESEARCH SUPPORTED */
        /*     BY THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS MCS-7600324 AND */
        /*     MCS-7906671. */

        /* ------------------------  EXTERNAL QUANTITIES  ------------------------ */

        /*  ***  INTRINSIC FUNCTIONS  *** */
        /* /+ */
        /* / */
        /* --------------------------  LOCAL VARIABLES  -------------------------- */


        /*  ***  V SUBSCRIPTS  *** */


        /*  ***  DATA INITIALIZATIONS  *** */

        /* /6 */
        /*     DATA HALF/0.5D+0/, ONE/1.D+0/, TWO/2.D+0/, ZERO/0.D+0/ */
        /* /7 */
        /* / */

        /* /6 */
        /*     DATA BIAS/43/, DGNORM/1/, DSTNRM/2/, DST0/3/, GRDFAC/45/, */
        /*    1     GTHG/44/, GTSTEP/4/, NREDUC/6/, NWTFAC/46/, PREDUC/7/, */
        /*    2     RADIUS/8/, STPPAR/5/ */
        /* /7 */
        /* / */

        /* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

        /* Parameter adjustments */
        --v;
        --step;
        --nwtstp;
        --dig;

        /* Function Body */
        nwtnrm = v[3];
        rlambd = 1.;
        if (nwtnrm > 0.) {
            rlambd = v[8] / nwtnrm;
        }
        gnorm = v[1];
        ghinvg = v[6] * 2.;
        v[45] = 0.;
        v[46] = 0.;
        if (rlambd < 1.) {
            goto L30;
        }

        /*        ***  THE NEWTON STEP IS INSIDE THE TRUST REGION  *** */

        v[5] = 0.;
        v[2] = nwtnrm;
        v[4] = -ghinvg;
        v[7] = v[6];
        v[46] = -1.;
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            /* L20: */
            step[i__] = -nwtstp[i__];
        }
        goto L999;

L30:
        v[2] = v[8];
        /* Computing 2nd power */
        d__1 = gnorm / v[44];
        cfact = d__1 * d__1;
        /*     ***  CAUCHY STEP = -CFACT * G. */
        cnorm = gnorm * cfact;
        relax = 1. - v[43] * (1. - gnorm * cnorm / ghinvg);
        if (rlambd < relax) {
            goto L50;
        }

        /*        ***  STEP IS BETWEEN RELAXED NEWTON AND FULL NEWTON STEPS  *** */

        v[5] = 1. - (rlambd - relax) / (1. - relax);
        t = -rlambd;
        v[4] = t * ghinvg;
        v[7] = rlambd * (1. - rlambd * .5) * ghinvg;
        v[46] = t;
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            /* L40: */
            step[i__] = t * nwtstp[i__];
        }
        goto L999;

L50:
        if (cnorm < v[8]) {
            goto L70;
        }

        /*        ***  THE CAUCHY STEP LIES OUTSIDE THE TRUST REGION -- */
        /*        ***  STEP = SCALED CAUCHY STEP  *** */

        t = -v[8] / gnorm;
        v[45] = t;
        v[5] = cnorm / v[8] + 1.;
        v[4] = -v[8] * gnorm;
        /* Computing 2nd power */
        d__1 = v[44] / gnorm;
        v[7] = v[8] * (gnorm - v[8] * .5 * (d__1 * d__1));
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            /* L60: */
            step[i__] = t * dig[i__];
        }
        goto L999;

        /*     ***  COMPUTE DOGLEG STEP BETWEEN CAUCHY AND RELAXED NEWTON  *** */
        /*     ***  FEMUR = RELAXED NEWTON STEP MINUS CAUCHY STEP  *** */

L70:
        ctrnwt = cfact * relax * ghinvg / gnorm;
        /*     *** CTRNWT = INNER PROD. OF CAUCHY AND RELAXED NEWTON STEPS, */
        /*     *** SCALED BY GNORM**-1. */
        /* Computing 2nd power */
        d__1 = cfact;
        t1 = ctrnwt - gnorm * (d__1 * d__1);
        /*     ***  T1 = INNER PROD. OF FEMUR AND CAUCHY STEP, SCALED BY */
        /*     ***  GNORM**-1. */
        /* Computing 2nd power */
        d__1 = cfact;
        t2 = v[8] * (v[8] / gnorm) - gnorm * (d__1 * d__1);
        t = relax * nwtnrm;
        femnsq = t / gnorm * t - ctrnwt - t1;
        /*     ***  FEMNSQ = SQUARE OF 2-NORM OF FEMUR, SCALED BY GNORM**-1. */
        /* Computing 2nd power */
        d__1 = t1;
        t = t2 / (t1 + std::sqrt(d__1 * d__1 + femnsq * t2));
        /*     ***  DOGLEG STEP  =  CAUCHY STEP  +  T * FEMUR. */
        t1 = (t - 1.) * cfact;
        v[45] = t1;
        t2 = -t * relax;
        v[46] = t2;
        v[5] = 2. - t;
        /* Computing 2nd power */
        d__1 = gnorm;
        v[4] = t1 * (d__1 * d__1) + t2 * ghinvg;
        /* Computing 2nd power */
        d__1 = v[44] * t1;
        v[7] = -t1 * gnorm * ((t2 + 1.) * gnorm) - t2 * (t2 * .5 + 1.) * ghinvg -
                d__1 * d__1 * .5;
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            /* L80: */
            step[i__] = t1 * dig[i__] + t2 * nwtstp[i__];
        }

L999:
        return 0;
        /*  ***  LAST LINE OF DD7DOG FOLLOWS  *** */
    } /* dd7dog_ */

    template<typename doublereal>
    int dv2axy_(integer *p, doublereal *w, doublereal *a,
            doublereal *x, doublereal *y) {
        /* System generated locals */
        integer i__1;

        /* Local variables */
        static integer i__;


        /*  ***  SET W = A*X + Y  --  W, X, Y = P-VECTORS, A = SCALAR  *** */



        /* Parameter adjustments */
        --y;
        --x;
        --w;

        /* Function Body */
        i__1 = *p;
        for (i__ = 1; i__ <= i__1; ++i__) {
            /* L10: */
            w[i__] = *a * x[i__] + y[i__];
        }
        return 0;
    } /* dv2axy_ */

    template<typename doublereal>
    doublereal drldst_(integer *p, doublereal *d__, doublereal *x, doublereal *x0) {
        /* System generated locals */
        integer i__1;
        doublereal ret_val, d__1, d__2;

        /* Local variables */
        static integer i__;
        static doublereal t, emax, xmax;


        /*  ***  COMPUTE AND RETURN RELATIVE DIFFERENCE BETWEEN X AND X0  *** */
        /*  ***  NL2SOL VERSION 2.2  *** */


        /* /6 */
        /*     DATA ZERO/0.D+0/ */
        /* /7 */
        /* / */

        /*  ***  BODY  *** */

        /* Parameter adjustments */
        --x0;
        --x;
        --d__;

        /* Function Body */
        emax = 0.;
        xmax = 0.;
        i__1 = *p;
        for (i__ = 1; i__ <= i__1; ++i__) {
            t = (d__1 = d__[i__] * (x[i__] - x0[i__]), std::fabs(d__1));
            if (emax < t) {
                emax = t;
            }
            t = d__[i__] * ((d__1 = x[i__], std::fabs(d__1)) + (d__2 = x0[i__], std::fabs(
                    d__2)));
            if (xmax < t) {
                xmax = t;
            }
            /* L10: */
        }
        ret_val = 0.;
        if (xmax > 0.) {
            ret_val = emax / xmax;
        }
        /* L999: */
        return ret_val;
        /*  ***  LAST CARD OF DRLDST FOLLOWS  *** */
    } /* drldst_ */

    template<typename doublereal>
    int da7sst_(integer *iv, integer *liv, integer *lv,
            doublereal *v) {
        /* System generated locals */
        doublereal d__1, d__2;

        /* Local variables */
        static integer i__, nfc;
        static doublereal gts, emax, xmax, rfac1, emaxs;
        static logical goodx;


        /*  ***  ASSESS CANDIDATE STEP (***SOL VERSION 2.3)  *** */


        /*  ***  PURPOSE  *** */

        /*        THIS SUBROUTINE IS CALLED BY AN UNCONSTRAINED MINIMIZATION */
        /*     ROUTINE TO ASSESS THE NEXT CANDIDATE STEP.  IT MAY RECOMMEND ONE */
        /*     OF SEVERAL COURSES OF ACTION, SUCH AS ACCEPTING THE STEP, RECOM- */
        /*     PUTING IT USING THE SAME OR A NEW QUADRATIC MODEL, OR HALTING DUE */
        /*     TO CONVERGENCE OR FALSE CONVERGENCE.  SEE THE RETURN CODE LISTING */
        /*     BELOW. */

        /* --------------------------  PARAMETER USAGE  -------------------------- */

        /*  IV (I/O) INTEGER PARAMETER AND SCRATCH VECTOR -- SEE DESCRIPTION */
        /*             BELOW OF IV VALUES REFERENCED. */
        /* LIV (IN)  LENGTH OF IV ARRAY. */
        /*  LV (IN)  LENGTH OF V ARRAY. */
        /*   V (I/O) REAL PARAMETER AND SCRATCH VECTOR -- SEE DESCRIPTION */
        /*             BELOW OF V VALUES REFERENCED. */

        /*  ***  IV VALUES REFERENCED  *** */

        /*    IV(IRC) (I/O) ON INPUT FOR THE FIRST STEP TRIED IN A NEW ITERATION, */
        /*             IV(IRC) SHOULD BE SET TO 3 OR 4 (THE VALUE TO WHICH IT IS */
        /*             SET WHEN STEP IS DEFINITELY TO BE ACCEPTED).  ON INPUT */
        /*             AFTER STEP HAS BEEN RECOMPUTED, IV(IRC) SHOULD BE */
        /*             UNCHANGED SINCE THE PREVIOUS RETURN OF DA7SST. */
        /*                ON OUTPUT, IV(IRC) IS A RETURN CODE HAVING ONE OF THE */
        /*             FOLLOWING VALUES... */
        /*                  1 = SWITCH MODELS OR TRY SMALLER STEP. */
        /*                  2 = SWITCH MODELS OR ACCEPT STEP. */
        /*                  3 = ACCEPT STEP AND DETERMINE V(RADFAC) BY GRADIENT */
        /*                       TESTS. */
        /*                  4 = ACCEPT STEP, V(RADFAC) HAS BEEN DETERMINED. */
        /*                  5 = RECOMPUTE STEP (USING THE SAME MODEL). */
        /*                  6 = RECOMPUTE STEP WITH RADIUS = V(LMAXS) BUT DO NOT */
        /*                       EVALUATE THE OBJECTIVE FUNCTION. */
        /*                  7 = X-CONVERGENCE (SEE V(XCTOL)). */
        /*                  8 = RELATIVE FUNCTION CONVERGENCE (SEE V(RFCTOL)). */
        /*                  9 = BOTH X- AND RELATIVE FUNCTION CONVERGENCE. */
        /*                 10 = ABSOLUTE FUNCTION CONVERGENCE (SEE V(AFCTOL)). */
        /*                 11 = SINGULAR CONVERGENCE (SEE V(LMAXS)). */
        /*                 12 = FALSE CONVERGENCE (SEE V(XFTOL)). */
        /*                 13 = IV(IRC) WAS OUT OF RANGE ON INPUT. */
        /*             RETURN CODE I HAS PRECEDENCE OVER I+1 FOR I = 9, 10, 11. */
        /* IV(MLSTGD) (I/O) SAVED VALUE OF IV(MODEL). */
        /*  IV(MODEL) (I/O) ON INPUT, IV(MODEL) SHOULD BE AN INTEGER IDENTIFYING */
        /*             THE CURRENT QUADRATIC MODEL OF THE OBJECTIVE FUNCTION. */
        /*             IF A PREVIOUS STEP YIELDED A BETTER FUNCTION REDUCTION, */
        /*             THEN IV(MODEL) WILL BE SET TO IV(MLSTGD) ON OUTPUT. */
        /* IV(NFCALL) (IN)  INVOCATION COUNT FOR THE OBJECTIVE FUNCTION. */
        /* IV(NFGCAL) (I/O) VALUE OF IV(NFCALL) AT STEP THAT GAVE THE BIGGEST */
        /*             FUNCTION REDUCTION THIS ITERATION.  IV(NFGCAL) REMAINS */
        /*             UNCHANGED UNTIL A FUNCTION REDUCTION IS OBTAINED. */
        /* IV(RADINC) (I/O) THE NUMBER OF RADIUS INCREASES (OR MINUS THE NUMBER */
        /*             OF DECREASES) SO FAR THIS ITERATION. */
        /* IV(RESTOR) (OUT) SET TO 1 IF V(F) HAS BEEN RESTORED AND X SHOULD BE */
        /*             RESTORED TO ITS INITIAL VALUE, TO 2 IF X SHOULD BE SAVED, */
        /*             TO 3 IF X SHOULD BE RESTORED FROM THE SAVED VALUE, AND TO */
        /*             0 OTHERWISE. */
        /*  IV(STAGE) (I/O) COUNT OF THE NUMBER OF MODELS TRIED SO FAR IN THE */
        /*             CURRENT ITERATION. */
        /* IV(STGLIM) (IN)  MAXIMUM NUMBER OF MODELS TO CONSIDER. */
        /* IV(SWITCH) (OUT) SET TO 0 UNLESS A NEW MODEL IS BEING TRIED AND IT */
        /*             GIVES A SMALLER FUNCTION VALUE THAN THE PREVIOUS MODEL, */
        /*             IN WHICH CASE DA7SST SETS IV(SWITCH) = 1. */
        /* IV(TOOBIG) (I/O)  IS NONZERO ON INPUT IF STEP WAS TOO BIG (E.G., IF */
        /*             IT WOULD CAUSE OVERFLOW).  IT IS SET TO 0 ON RETURN. */
        /*   IV(XIRC) (I/O) VALUE THAT IV(IRC) WOULD HAVE IN THE ABSENCE OF */
        /*             CONVERGENCE, FALSE CONVERGENCE, AND OVERSIZED STEPS. */

        /*  ***  V VALUES REFERENCED  *** */

        /* V(AFCTOL) (IN)  ABSOLUTE FUNCTION CONVERGENCE TOLERANCE.  IF THE */
        /*             ABSOLUTE VALUE OF THE CURRENT FUNCTION VALUE V(F) IS LESS */
        /*             THAN V(AFCTOL) AND DA7SST DOES NOT RETURN WITH */
        /*             IV(IRC) = 11, THEN DA7SST RETURNS WITH IV(IRC) = 10. */
        /* V(DECFAC) (IN)  FACTOR BY WHICH TO DECREASE RADIUS WHEN IV(TOOBIG) IS */
        /*             NONZERO. */
        /* V(DSTNRM) (IN)  THE 2-NORM OF D*STEP. */
        /* V(DSTSAV) (I/O) VALUE OF V(DSTNRM) ON SAVED STEP. */
        /*   V(DST0) (IN)  THE 2-NORM OF D TIMES THE NEWTON STEP (WHEN DEFINED, */
        /*             I.E., FOR V(NREDUC) .GE. 0). */
        /*      V(F) (I/O) ON BOTH INPUT AND OUTPUT, V(F) IS THE OBJECTIVE FUNC- */
        /*             TION VALUE AT X.  IF X IS RESTORED TO A PREVIOUS VALUE, */
        /*             THEN V(F) IS RESTORED TO THE CORRESPONDING VALUE. */
        /*   V(FDIF) (OUT) THE FUNCTION REDUCTION V(F0) - V(F) (FOR THE OUTPUT */
        /*             VALUE OF V(F) IF AN EARLIER STEP GAVE A BIGGER FUNCTION */
        /*             DECREASE, AND FOR THE INPUT VALUE OF V(F) OTHERWISE). */
        /* V(FLSTGD) (I/O) SAVED VALUE OF V(F). */
        /*     V(F0) (IN)  OBJECTIVE FUNCTION VALUE AT START OF ITERATION. */
        /* V(GTSLST) (I/O) VALUE OF V(GTSTEP) ON SAVED STEP. */
        /* V(GTSTEP) (IN)  INNER PRODUCT BETWEEN STEP AND GRADIENT. */
        /* V(INCFAC) (IN)  MINIMUM FACTOR BY WHICH TO INCREASE RADIUS. */
        /*  V(LMAXS) (IN)  MAXIMUM REASONABLE STEP SIZE (AND INITIAL STEP BOUND). */
        /*             IF THE ACTUAL FUNCTION DECREASE IS NO MORE THAN TWICE */
        /*             WHAT WAS PREDICTED, IF A RETURN WITH IV(IRC) = 7, 8, OR 9 */
        /*             DOES NOT OCCUR, IF V(DSTNRM) .GT. V(LMAXS) OR THE CURRENT */
        /*             STEP IS A NEWTON STEP, AND IF */
        /*             V(PREDUC) .LE. V(SCTOL) * ABS(V(F0)), THEN DA7SST RETURNS */
        /*             WITH IV(IRC) = 11.  IF SO DOING APPEARS WORTHWHILE, THEN */
        /*            DA7SST REPEATS THIS TEST (DISALLOWING A FULL NEWTON STEP) */
        /*             WITH V(PREDUC) COMPUTED FOR A STEP OF LENGTH V(LMAXS) */
        /*             (BY A RETURN WITH IV(IRC) = 6). */
        /* V(NREDUC) (I/O)  FUNCTION REDUCTION PREDICTED BY QUADRATIC MODEL FOR */
        /*             NEWTON STEP.  IF DA7SST IS CALLED WITH IV(IRC) = 6, I.E., */
        /*             IF V(PREDUC) HAS BEEN COMPUTED WITH RADIUS = V(LMAXS) FOR */
        /*             USE IN THE SINGULAR CONVERGENCE TEST, THEN V(NREDUC) IS */
        /*             SET TO -V(PREDUC) BEFORE THE LATTER IS RESTORED. */
        /* V(PLSTGD) (I/O) VALUE OF V(PREDUC) ON SAVED STEP. */
        /* V(PREDUC) (I/O) FUNCTION REDUCTION PREDICTED BY QUADRATIC MODEL FOR */
        /*             CURRENT STEP. */
        /* V(RADFAC) (OUT) FACTOR TO BE USED IN DETERMINING THE NEW RADIUS, */
        /*             WHICH SHOULD BE V(RADFAC)*DST, WHERE  DST  IS EITHER THE */
        /*             OUTPUT VALUE OF V(DSTNRM) OR THE 2-NORM OF */
        /*             DIAG(NEWD)*STEP  FOR THE OUTPUT VALUE OF STEP AND THE */
        /*             UPDATED VERSION, NEWD, OF THE SCALE VECTOR D.  FOR */
        /*             IV(IRC) = 3, V(RADFAC) = 1.0 IS RETURNED. */
        /* V(RDFCMN) (IN)  MINIMUM VALUE FOR V(RADFAC) IN TERMS OF THE INPUT */
        /*             VALUE OF V(DSTNRM) -- SUGGESTED VALUE = 0.1. */
        /* V(RDFCMX) (IN)  MAXIMUM VALUE FOR V(RADFAC) -- SUGGESTED VALUE = 4.0. */
        /*  V(RELDX) (IN) SCALED RELATIVE CHANGE IN X CAUSED BY STEP, COMPUTED */
        /*             (E.G.) BY FUNCTION  DRLDST  AS */
        /*                 MAX (D(I)*ABS(X(I)-X0(I)), 1 .LE. I .LE. P) / */
        /*                    MAX (D(I)*(ABS(X(I))+ABS(X0(I))), 1 .LE. I .LE. P). */
        /* V(RFCTOL) (IN)  RELATIVE FUNCTION CONVERGENCE TOLERANCE.  IF THE */
        /*             ACTUAL FUNCTION REDUCTION IS AT MOST TWICE WHAT WAS PRE- */
        /*             DICTED AND  V(NREDUC) .LE. V(RFCTOL)*ABS(V(F0)),  THEN */
        /*            DA7SST RETURNS WITH IV(IRC) = 8 OR 9. */
        /*  V(SCTOL) (IN)  SINGULAR CONVERGENCE TOLERANCE -- SEE V(LMAXS). */
        /* V(STPPAR) (IN)  MARQUARDT PARAMETER -- 0 MEANS FULL NEWTON STEP. */
        /* V(TUNER1) (IN)  TUNING CONSTANT USED TO DECIDE IF THE FUNCTION */
        /*             REDUCTION WAS MUCH LESS THAN EXPECTED.  SUGGESTED */
        /*             VALUE = 0.1. */
        /* V(TUNER2) (IN)  TUNING CONSTANT USED TO DECIDE IF THE FUNCTION */
        /*             REDUCTION WAS LARGE ENOUGH TO ACCEPT STEP.  SUGGESTED */
        /*             VALUE = 10**-4. */
        /* V(TUNER3) (IN)  TUNING CONSTANT USED TO DECIDE IF THE RADIUS */
        /*             SHOULD BE INCREASED.  SUGGESTED VALUE = 0.75. */
        /*  V(XCTOL) (IN)  X-CONVERGENCE CRITERION.  IF STEP IS A NEWTON STEP */
        /*             (V(STPPAR) = 0) HAVING V(RELDX) .LE. V(XCTOL) AND GIVING */
        /*             AT MOST TWICE THE PREDICTED FUNCTION DECREASE, THEN */
        /*            DA7SST RETURNS IV(IRC) = 7 OR 9. */
        /*  V(XFTOL) (IN)  FALSE CONVERGENCE TOLERANCE.  IF STEP GAVE NO OR ONLY */
        /*             A SMALL FUNCTION DECREASE AND V(RELDX) .LE. V(XFTOL), */
        /*             THEN DA7SST RETURNS WITH IV(IRC) = 12. */

        /* -------------------------------  NOTES  ------------------------------- */

        /*  ***  APPLICATION AND USAGE RESTRICTIONS  *** */

        /*        THIS ROUTINE IS CALLED AS PART OF THE NL2SOL (NONLINEAR */
        /*     LEAST-SQUARES) PACKAGE.  IT MAY BE USED IN ANY UNCONSTRAINED */
        /*     MINIMIZATION SOLVER THAT USES DOGLEG, GOLDFELD-QUANDT-TROTTER, */
        /*     OR LEVENBERG-MARQUARDT STEPS. */

        /*  ***  ALGORITHM NOTES  *** */

        /*        SEE (1) FOR FURTHER DISCUSSION OF THE ASSESSING AND MODEL */
        /*     SWITCHING STRATEGIES.  WHILE NL2SOL CONSIDERS ONLY TWO MODELS, */
        /*    DA7SST IS DESIGNED TO HANDLE ANY NUMBER OF MODELS. */

        /*  ***  USAGE NOTES  *** */

        /*        ON THE FIRST CALL OF AN ITERATION, ONLY THE I/O VARIABLES */
        /*     STEP, X, IV(IRC), IV(MODEL), V(F), V(DSTNRM), V(GTSTEP), AND */
        /*     V(PREDUC) NEED HAVE BEEN INITIALIZED.  BETWEEN CALLS, NO I/O */
        /*     VALUES EXCEPT STEP, X, IV(MODEL), V(F) AND THE STOPPING TOLER- */
        /*     ANCES SHOULD BE CHANGED. */
        /*        AFTER A RETURN FOR CONVERGENCE OR FALSE CONVERGENCE, ONE CAN */
        /*     CHANGE THE STOPPING TOLERANCES AND CALL DA7SST AGAIN, IN WHICH */
        /*     CASE THE STOPPING TESTS WILL BE REPEATED. */

        /*  ***  REFERENCES  *** */

        /*     (1) DENNIS, J.E., JR., GAY, D.M., AND WELSCH, R.E. (1981), */
        /*        AN ADAPTIVE NONLINEAR LEAST-SQUARES ALGORITHM, */
        /*        ACM TRANS. MATH. SOFTWARE, VOL. 7, NO. 3. */

        /*     (2) POWELL, M.J.D. (1970)  A FORTRAN SUBROUTINE FOR SOLVING */
        /*        SYSTEMS OF NONLINEAR ALGEBRAIC EQUATIONS, IN NUMERICAL */
        /*        METHODS FOR NONLINEAR ALGEBRAIC EQUATIONS, EDITED BY */
        /*        P. RABINOWITZ, GORDON AND BREACH, LONDON. */

        /*  ***  HISTORY  *** */

        /*        JOHN DENNIS DESIGNED MUCH OF THIS ROUTINE, STARTING WITH */
        /*     IDEAS IN (2). ROY WELSCH SUGGESTED THE MODEL SWITCHING STRATEGY. */
        /*        DAVID GAY AND STEPHEN PETERS CAST THIS SUBROUTINE INTO A MORE */
        /*     PORTABLE FORM (WINTER 1977), AND DAVID GAY CAST IT INTO ITS */
        /*     PRESENT FORM (FALL 1978), WITH MINOR CHANGES TO THE SINGULAR */
        /*     CONVERGENCE TEST IN MAY, 1984 (TO DEAL WITH FULL NEWTON STEPS). */

        /*  ***  GENERAL  *** */

        /*     THIS SUBROUTINE WAS WRITTEN IN CONNECTION WITH RESEARCH */
        /*     SUPPORTED BY THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS */
        /*     MCS-7600324, DCR75-10143, 76-14311DSS, MCS76-11989, AND */
        /*     MCS-7906671. */

        /* ------------------------  EXTERNAL QUANTITIES  ------------------------ */

        /*  ***  NO EXTERNAL FUNCTIONS AND SUBROUTINES  *** */

        /* --------------------------  LOCAL VARIABLES  -------------------------- */


        /*  ***  SUBSCRIPTS FOR IV AND V  *** */


        /*  ***  DATA INITIALIZATIONS  *** */

        /* /6 */
        /*     DATA HALF/0.5D+0/, ONE/1.D+0/, ONEP2/1.2D+0/, TWO/2.D+0/, */
        /*    1     ZERO/0.D+0/ */
        /* /7 */
        /* / */

        /* /6 */
        /*     DATA IRC/29/, MLSTGD/32/, MODEL/5/, NFCALL/6/, NFGCAL/7/, */
        /*    1     RADINC/8/, RESTOR/9/, STAGE/10/, STGLIM/11/, SWITCH/12/, */
        /*    2     TOOBIG/2/, XIRC/13/ */
        /* /7 */
        /* / */
        /* /6 */
        /*     DATA AFCTOL/31/, DECFAC/22/, DSTNRM/2/, DST0/3/, DSTSAV/18/, */
        /*    1     F/10/, FDIF/11/, FLSTGD/12/, F0/13/, GTSLST/14/, GTSTEP/4/, */
        /*    2     INCFAC/23/, LMAXS/36/, NREDUC/6/, PLSTGD/15/, PREDUC/7/, */
        /*    3     RADFAC/16/, RDFCMN/24/, RDFCMX/25/, RELDX/17/, RFCTOL/32/, */
        /*    4     SCTOL/37/, STPPAR/5/, TUNER1/26/, TUNER2/27/, TUNER3/28/, */
        /*    5     XCTOL/33/, XFTOL/34/ */
        /* /7 */
        /* / */

        /* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

        /* Parameter adjustments */
        --iv;
        --v;

        /* Function Body */
        nfc = iv[6];
        iv[12] = 0;
        iv[9] = 0;
        rfac1 = 1.;
        goodx = TRUE_;
        i__ = iv[29];
        if (i__ >= 1 && i__ <= 12) {
            switch (i__) {
                case 1: goto L20;
                case 2: goto L30;
                case 3: goto L10;
                case 4: goto L10;
                case 5: goto L40;
                case 6: goto L280;
                case 7: goto L220;
                case 8: goto L220;
                case 9: goto L220;
                case 10: goto L220;
                case 11: goto L220;
                case 12: goto L170;
            }
        }
        iv[29] = 13;
        goto L999;

        /*  ***  INITIALIZE FOR NEW ITERATION  *** */

L10:
        iv[10] = 1;
        iv[8] = 0;
        v[12] = v[13];
        if (iv[2] == 0) {
            goto L110;
        }
        iv[10] = -1;
        iv[13] = i__;
        goto L60;

        /*  ***  STEP WAS RECOMPUTED WITH NEW MODEL OR SMALLER RADIUS  *** */
        /*  ***  FIRST DECIDE WHICH  *** */

L20:
        if (iv[5] != iv[32]) {
            goto L30;
        }
        /*        ***  OLD MODEL RETAINED, SMALLER RADIUS TRIED  *** */
        /*        ***  DO NOT CONSIDER ANY MORE NEW MODELS THIS ITERATION  *** */
        iv[10] = iv[11];
        iv[8] = -1;
        goto L110;

        /*  ***  A NEW MODEL IS BEING TRIED.  DECIDE WHETHER TO KEEP IT.  *** */

L30:
        ++iv[10];

        /*     ***  NOW WE ADD THE POSSIBILITY THAT STEP WAS RECOMPUTED WITH  *** */
        /*     ***  THE SAME MODEL, PERHAPS BECAUSE OF AN OVERSIZED STEP.     *** */

L40:
        if (iv[10] > 0) {
            goto L50;
        }

        /*        ***  STEP WAS RECOMPUTED BECAUSE IT WAS TOO BIG.  *** */

        if (iv[2] != 0) {
            goto L60;
        }

        /*        ***  RESTORE IV(STAGE) AND PICK UP WHERE WE LEFT OFF.  *** */

        iv[10] = -iv[10];
        i__ = iv[13];
        switch (i__) {
            case 1: goto L20;
            case 2: goto L30;
            case 3: goto L110;
            case 4: goto L110;
            case 5: goto L70;
        }

L50:
        if (iv[2] == 0) {
            goto L70;
        }

        /*  ***  HANDLE OVERSIZE STEP  *** */

        iv[2] = 0;
        if (iv[8] > 0) {
            goto L80;
        }
        iv[10] = -iv[10];
        iv[13] = iv[29];

L60:
        iv[2] = 0;
        v[16] = v[22];
        --iv[8];
        iv[29] = 5;
        iv[9] = 1;
        v[10] = v[12];
        goto L999;

L70:
        if (v[10] < v[12]) {
            goto L110;
        }

        /*     *** THE NEW STEP IS A LOSER.  RESTORE OLD MODEL.  *** */

        if (iv[5] == iv[32]) {
            goto L80;
        }
        iv[5] = iv[32];
        iv[12] = 1;

        /*     ***  RESTORE STEP, ETC. ONLY IF A PREVIOUS STEP DECREASED V(F). */

L80:
        if (v[12] >= v[13]) {
            goto L110;
        }
        if (iv[10] < iv[11]) {
            goodx = FALSE_;
        } else if (nfc < iv[7] + iv[11] + 2) {
            goodx = FALSE_;
        } else if (iv[12] != 0) {
            goodx = FALSE_;
        }
        iv[9] = 3;
        v[10] = v[12];
        v[7] = v[15];
        v[4] = v[14];
        if (iv[12] == 0) {
            rfac1 = v[2] / v[18];
        }
        v[2] = v[18];
        if (goodx) {

            /*     ***  ACCEPT PREVIOUS SLIGHTLY REDUCING STEP *** */

            v[11] = v[13] - v[10];
            iv[29] = 4;
            v[16] = rfac1;
            goto L999;
        }
        nfc = iv[7];

L110:
        v[11] = v[13] - v[10];
        if (v[11] > v[27] * v[7]) {
            goto L140;
        }
        if (iv[8] > 0) {
            goto L140;
        }

        /*        ***  NO (OR ONLY A TRIVIAL) FUNCTION DECREASE */
        /*        ***  -- SO TRY NEW MODEL OR SMALLER RADIUS */

        if (v[10] < v[13]) {
            goto L120;
        }
        iv[32] = iv[5];
        v[12] = v[10];
        v[10] = v[13];
        iv[9] = 1;
        goto L130;
L120:
        iv[7] = nfc;
L130:
        iv[29] = 1;
        if (iv[10] < iv[11]) {
            goto L160;
        }
        iv[29] = 5;
        --iv[8];
        goto L160;

        /*  ***  NONTRIVIAL FUNCTION DECREASE ACHIEVED  *** */

L140:
        iv[7] = nfc;
        rfac1 = 1.;
        v[18] = v[2];
        if (v[11] > v[7] * v[26]) {
            goto L190;
        }

        /*  ***  DECREASE WAS MUCH LESS THAN PREDICTED -- EITHER CHANGE MODELS */
        /*  ***  OR ACCEPT STEP WITH DECREASED RADIUS. */

        if (iv[10] >= iv[11]) {
            goto L150;
        }
        /*        ***  CONSIDER SWITCHING MODELS  *** */
        iv[29] = 2;
        goto L160;

        /*     ***  ACCEPT STEP WITH DECREASED RADIUS  *** */

L150:
        iv[29] = 4;

        /*  ***  SET V(RADFAC) TO FLETCHER*S DECREASE FACTOR  *** */

L160:
        iv[13] = iv[29];
        emax = v[4] + v[11];
        v[16] = rfac1 * .5;
        if (emax < v[4]) {
            /* Computing MAX */
            d__1 = v[24], d__2 = v[4] * .5 / emax;
            v[16] = rfac1 * std::max(d__1, d__2);
        }

        /*  ***  DO FALSE CONVERGENCE TEST  *** */

L170:
        if (v[17] <= v[34]) {
            goto L180;
        }
        iv[29] = iv[13];
        if (v[10] < v[13]) {
            goto L200;
        }
        goto L230;

L180:
        iv[29] = 12;
        goto L240;

        /*  ***  HANDLE GOOD FUNCTION DECREASE  *** */

L190:
        if (v[11] < -v[28] * v[4]) {
            goto L210;
        }

        /*     ***  INCREASING RADIUS LOOKS WORTHWHILE.  SEE IF WE JUST */
        /*     ***  RECOMPUTED STEP WITH A DECREASED RADIUS OR RESTORED STEP */
        /*     ***  AFTER RECOMPUTING IT WITH A LARGER RADIUS. */

        if (iv[8] < 0) {
            goto L210;
        }
        if (iv[9] == 1) {
            goto L210;
        }
        if (iv[9] == 3) {
            goto L210;
        }

        /*        ***  WE DID NOT.  TRY A LONGER STEP UNLESS THIS WAS A NEWTON */
        /*        ***  STEP. */

        v[16] = v[25];
        gts = v[4];
        if (v[11] < (.5 / v[16] - 1.) * gts) {
            /* Computing MAX */
            d__1 = v[23], d__2 = gts * .5 / (gts + v[11]);
            v[16] = std::max(d__1, d__2);
        }
        iv[29] = 4;
        if (v[5] == 0.) {
            goto L230;
        }
        if (v[3] >= 0. && (v[3] < v[2] * 2. || v[6] < v[11] * 1.2)) {
            goto L230;
        }
        /*             ***  STEP WAS NOT A NEWTON STEP.  RECOMPUTE IT WITH */
        /*             ***  A LARGER RADIUS. */
        iv[29] = 5;
        ++iv[8];

        /*  ***  SAVE VALUES CORRESPONDING TO GOOD STEP  *** */

L200:
        v[12] = v[10];
        iv[32] = iv[5];
        if (iv[9] == 0) {
            iv[9] = 2;
        }
        v[18] = v[2];
        iv[7] = nfc;
        v[15] = v[7];
        v[14] = v[4];
        goto L230;

        /*  ***  ACCEPT STEP WITH RADIUS UNCHANGED  *** */

L210:
        v[16] = 1.;
        iv[29] = 3;
        goto L230;

        /*  ***  COME HERE FOR A RESTART AFTER CONVERGENCE  *** */

L220:
        iv[29] = iv[13];
        if (v[18] >= 0.) {
            goto L240;
        }
        iv[29] = 12;
        goto L240;

        /*  ***  PERFORM CONVERGENCE TESTS  *** */

L230:
        iv[13] = iv[29];
L240:
        if (iv[9] == 1 && v[12] < v[13]) {
            iv[9] = 3;
        }
        if (std::fabs(v[10]) < v[31]) {
            iv[29] = 10;
        }
        if (v[11] * .5 > v[7]) {
            goto L999;
        }
        emax = v[32] * std::fabs(v[13]);
        emaxs = v[37] * std::fabs(v[13]);
        if (v[7] <= emaxs && (v[2] > v[36] || v[5] == 0.)) {
            iv[29] = 11;
        }
        if (v[3] < 0.) {
            goto L250;
        }
        i__ = 0;
        if ((v[6] > 0. && v[6] <= emax) || (v[6] == 0. && v[7] == 0.)) {
            i__ = 2;
        }
        if (v[5] == 0. && v[17] <= v[33] && goodx) {
            ++i__;
        }
        if (i__ > 0) {
            iv[29] = i__ + 6;
        }

        /*  ***  CONSIDER RECOMPUTING STEP OF LENGTH V(LMAXS) FOR SINGULAR */
        /*  ***  CONVERGENCE TEST. */

L250:
        if (iv[29] > 5 && iv[29] != 12) {
            goto L999;
        }
        if (v[5] == 0.) {
            goto L999;
        }
        if (v[2] > v[36]) {
            goto L260;
        }
        if (v[7] >= emaxs) {
            goto L999;
        }
        if (v[3] <= 0.) {
            goto L270;
        }
        if (v[3] * .5 <= v[36]) {
            goto L999;
        }
        goto L270;
L260:
        if (v[2] * .5 <= v[36]) {
            goto L999;
        }
        xmax = v[36] / v[2];
        if (xmax * (2. - xmax) * v[7] >= emaxs) {
            goto L999;
        }
L270:
        if (v[6] < 0.) {
            goto L290;
        }

        /*  ***  RECOMPUTE V(PREDUC) FOR USE IN SINGULAR CONVERGENCE TEST  *** */

        v[14] = v[4];
        v[18] = v[2];
        if (iv[29] == 12) {
            v[18] = -v[18];
        }
        v[15] = v[7];
        i__ = iv[9];
        iv[9] = 2;
        if (i__ == 3) {
            iv[9] = 0;
        }
        iv[29] = 6;
        goto L999;

        /*  ***  PERFORM SINGULAR CONVERGENCE TEST WITH RECOMPUTED V(PREDUC)  *** */

L280:
        v[4] = v[14];
        v[2] = std::fabs(v[18]);
        iv[29] = iv[13];
        if (v[18] <= 0.) {
            iv[29] = 12;
        }
        v[6] = -v[7];
        v[7] = v[15];
        iv[9] = 3;
L290:
        if (-v[6] <= v[37] * std::fabs(v[13])) {
            iv[29] = 11;
        }

L999:
        return 0;

        /*  ***  LAST LINE OF DA7SST FOLLOWS  *** */
    } /* da7sst_ */

    template<typename doublereal>
    int dl7vml_(integer *n, doublereal *x, doublereal *l,
            doublereal *y) {
        /* System generated locals */
        integer i__1, i__2;

        /* Local variables */
        static integer i__, j;
        static doublereal t;
        static integer i0, ii, ij, np1;


        /*  ***  COMPUTE  X = L*Y, WHERE  L  IS AN  N X N  LOWER TRIANGULAR */
        /*  ***  MATRIX STORED COMPACTLY BY ROWS.  X AND Y MAY OCCUPY THE SAME */
        /*  ***  STORAGE.  *** */

        /*     DIMENSION L(N*(N+1)/2) */
        /* /6 */
        /*     DATA ZERO/0.D+0/ */
        /* /7 */
        /* / */

        /* Parameter adjustments */
        --y;
        --x;
        --l;

        /* Function Body */
        np1 = *n + 1;
        i0 = *n * (*n + 1) / 2;
        i__1 = *n;
        for (ii = 1; ii <= i__1; ++ii) {
            i__ = np1 - ii;
            i0 -= i__;
            t = 0.;
            i__2 = i__;
            for (j = 1; j <= i__2; ++j) {
                ij = i0 + j;
                t += l[ij] * y[j];
                /* L10: */
            }
            x[i__] = t;
            /* L20: */
        }
        /* L999: */
        return 0;
        /*  ***  LAST CARD OF DL7VML FOLLOWS  *** */
    } /* dl7vml_ */

    template<typename doublereal>
    int dw7zbf_(doublereal *l, integer *n, doublereal *s,
            doublereal *w, doublereal *y, doublereal *z__) {
        /* System generated locals */
        integer i__1;

        /* Builtin functions */
        //        double sqrt(doublereal);

        /* Local variables */
        static integer i__;
        static doublereal cs, cy, ys, shs, theta, epsrt;
        //        extern /* Subroutine */ int dl7ivm_(integer *, doublereal *, doublereal *,
        //                doublereal *);
        //        extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
        //        extern /* Subroutine */ int dl7tvm_(integer *, doublereal *, doublereal *,
        //                doublereal *);


        /*  ***  COMPUTE  Y  AND  Z  FOR  DL7UPD  CORRESPONDING TO BFGS UPDATE. */

        /*     DIMENSION L(N*(N+1)/2) */

        /* --------------------------  PARAMETER USAGE  -------------------------- */

        /* L (I/O) CHOLESKY FACTOR OF HESSIAN, A LOWER TRIANG. MATRIX STORED */
        /*             COMPACTLY BY ROWS. */
        /* N (INPUT) ORDER OF  L  AND LENGTH OF  S,  W,  Y,  Z. */
        /* S (INPUT) THE STEP JUST TAKEN. */
        /* W (OUTPUT) RIGHT SINGULAR VECTOR OF RANK 1 CORRECTION TO L. */
        /* Y (INPUT) CHANGE IN GRADIENTS CORRESPONDING TO S. */
        /* Z (OUTPUT) LEFT SINGULAR VECTOR OF RANK 1 CORRECTION TO L. */

        /* -------------------------------  NOTES  ------------------------------- */

        /*  ***  ALGORITHM NOTES  *** */

        /*        WHEN  S  IS COMPUTED IN CERTAIN WAYS, E.G. BY  GQTSTP  OR */
        /*     DBLDOG,  IT IS POSSIBLE TO SAVE N**2/2 OPERATIONS SINCE  (L**T)*S */
        /*     OR  L*(L**T)*S IS THEN KNOWN. */
        /*        IF THE BFGS UPDATE TO L*(L**T) WOULD REDUCE ITS DETERMINANT TO */
        /*     LESS THAN EPS TIMES ITS OLD VALUE, THEN THIS ROUTINE IN EFFECT */
        /*     REPLACES  Y  BY  THETA*Y + (1 - THETA)*L*(L**T)*S,  WHERE  THETA */
        /*     (BETWEEN 0 AND 1) IS CHOSEN TO MAKE THE REDUCTION FACTOR = EPS. */

        /*  ***  GENERAL  *** */

        /*     CODED BY DAVID M. GAY (FALL 1979). */
        /*     THIS SUBROUTINE WAS WRITTEN IN CONNECTION WITH RESEARCH SUPPORTED */
        /*     BY THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS MCS-7600324 AND */
        /*     MCS-7906671. */

        /* ------------------------  EXTERNAL QUANTITIES  ------------------------ */

        /*  ***  FUNCTIONS AND SUBROUTINES CALLED  *** */

        /* DD7TPR RETURNS INNER PRODUCT OF TWO VECTORS. */
        /* DL7IVM MULTIPLIES L**-1 TIMES A VECTOR. */
        /* DL7TVM MULTIPLIES L**T TIMES A VECTOR. */

        /*  ***  INTRINSIC FUNCTIONS  *** */
        /* /+ */
        /* / */
        /* --------------------------  LOCAL VARIABLES  -------------------------- */


        /*  ***  DATA INITIALIZATIONS  *** */

        /* /6 */
        /*     DATA EPS/0.1D+0/, ONE/1.D+0/ */
        /* /7 */
        /* / */

        /* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

        /* Parameter adjustments */
        --l;
        --z__;
        --y;
        --w;
        --s;

        /* Function Body */
        dl7tvm_(n, &w[1], &l[1], &s[1]);
        shs = dd7tpr_(n, &w[1], &w[1]);
        ys = dd7tpr_(n, &y[1], &s[1]);
        if (ys >= shs * .1) {
            goto L10;
        }
        theta = shs * .90000000000000002 / (shs - ys);
        epsrt = std::sqrt(.1);
        cy = theta / (shs * epsrt);
        cs = ((theta - 1.) / epsrt + 1.) / shs;
        goto L20;
L10:
        cy = 1. / (std::sqrt(ys) * std::sqrt(shs));
        cs = 1. / shs;
L20:
        dl7ivm_(n, &z__[1], &l[1], &y[1]);
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            /* L30: */
            z__[i__] = cy * z__[i__] - cs * w[i__];
        }

        /* L999: */
        return 0;
        /*  ***  LAST CARD OF DW7ZBF FOLLOWS  *** */
    } /* dw7zbf_ */

    template<typename doublereal>
    int dl7upd_(doublereal *beta, doublereal *gamma, doublereal *
            l, doublereal *lambda, doublereal *lplus, integer *n, doublereal *w,
            doublereal *z__) {
        /* System generated locals */
        integer i__1, i__2;
        doublereal d__1;

        /* Builtin functions */
        //        double sqrt(doublereal);

        /* Local variables */
        static doublereal a, b;
        static integer i__, j, k;
        static doublereal s, bj, gj;
        static integer ij, jj;
        static doublereal lj, wj, nu, zj;
        static integer jp1, nm1, np1;
        static doublereal eta, lij, ljj, theta;


        /*  ***  COMPUTE LPLUS = SECANT UPDATE OF L  *** */

        /*  ***  PARAMETER DECLARATIONS  *** */

        /*     DIMENSION L(N*(N+1)/2), LPLUS(N*(N+1)/2) */

        /* --------------------------  PARAMETER USAGE  -------------------------- */

        /*   BETA = SCRATCH VECTOR. */
        /*  GAMMA = SCRATCH VECTOR. */
        /*      L (INPUT) LOWER TRIANGULAR MATRIX, STORED ROWWISE. */
        /* LAMBDA = SCRATCH VECTOR. */
        /*  LPLUS (OUTPUT) LOWER TRIANGULAR MATRIX, STORED ROWWISE, WHICH MAY */
        /*             OCCUPY THE SAME STORAGE AS  L. */
        /*      N (INPUT) LENGTH OF VECTOR PARAMETERS AND ORDER OF MATRICES. */
        /*      W (INPUT, DESTROYED ON OUTPUT) RIGHT SINGULAR VECTOR OF RANK 1 */
        /*             CORRECTION TO  L. */
        /*      Z (INPUT, DESTROYED ON OUTPUT) LEFT SINGULAR VECTOR OF RANK 1 */
        /*             CORRECTION TO  L. */

        /* -------------------------------  NOTES  ------------------------------- */

        /*  ***  APPLICATION AND USAGE RESTRICTIONS  *** */

        /*        THIS ROUTINE UPDATES THE CHOLESKY FACTOR  L  OF A SYMMETRIC */
        /*     POSITIVE DEFINITE MATRIX TO WHICH A SECANT UPDATE IS BEING */
        /*     APPLIED -- IT COMPUTES A CHOLESKY FACTOR  LPLUS  OF */
        /*     L * (I + Z*W**T) * (I + W*Z**T) * L**T.  IT IS ASSUMED THAT  W */
        /*     AND  Z  HAVE BEEN CHOSEN SO THAT THE UPDATED MATRIX IS STRICTLY */
        /*     POSITIVE DEFINITE. */

        /*  ***  ALGORITHM NOTES  *** */

        /*        THIS CODE USES RECURRENCE 3 OF REF. 1 (WITH D(J) = 1 FOR ALL J) */
        /*     TO COMPUTE  LPLUS  OF THE FORM  L * (I + Z*W**T) * Q,  WHERE  Q */
        /*     IS AN ORTHOGONAL MATRIX THAT MAKES THE RESULT LOWER TRIANGULAR. */
        /*        LPLUS MAY HAVE SOME NEGATIVE DIAGONAL ELEMENTS. */

        /*  ***  REFERENCES  *** */

        /* 1.  GOLDFARB, D. (1976), FACTORIZED VARIABLE METRIC METHODS FOR UNCON- */
        /*             STRAINED OPTIMIZATION, MATH. COMPUT. 30, PP. 796-811. */

        /*  ***  GENERAL  *** */

        /*     CODED BY DAVID M. GAY (FALL 1979). */
        /*     THIS SUBROUTINE WAS WRITTEN IN CONNECTION WITH RESEARCH SUPPORTED */
        /*     BY THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS MCS-7600324 AND */
        /*     MCS-7906671. */

        /* ------------------------  EXTERNAL QUANTITIES  ------------------------ */

        /*  ***  INTRINSIC FUNCTIONS  *** */
        /* /+ */
        /* / */
        /* --------------------------  LOCAL VARIABLES  -------------------------- */


        /*  ***  DATA INITIALIZATIONS  *** */

        /* /6 */
        /*     DATA ONE/1.D+0/, ZERO/0.D+0/ */
        /* /7 */
        /* / */

        /* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

        /* Parameter adjustments */
        --l;
        --lplus;
        --z__;
        --w;
        --lambda;
        --gamma;
        --beta;

        /* Function Body */
        nu = 1.;
        eta = 0.;
        if (*n <= 1) {
            goto L30;
        }
        nm1 = *n - 1;

        /*  ***  TEMPORARILY STORE S(J) = SUM OVER K = J+1 TO N OF W(K)**2 IN */
        /*  ***  LAMBDA(J). */

        s = 0.;
        i__1 = nm1;
        for (i__ = 1; i__ <= i__1; ++i__) {
            j = *n - i__;
            /* Computing 2nd power */
            d__1 = w[j + 1];
            s += d__1 * d__1;
            lambda[j] = s;
            /* L10: */
        }

        /*  ***  COMPUTE LAMBDA, GAMMA, AND BETA BY GOLDFARB*S RECURRENCE 3. */

        i__1 = nm1;
        for (j = 1; j <= i__1; ++j) {
            wj = w[j];
            a = nu * z__[j] - eta * wj;
            theta = a * wj + 1.;
            s = a * lambda[j];
            /* Computing 2nd power */
            d__1 = theta;
            lj = std::sqrt(d__1 * d__1 + a * s);
            if (theta > 0.) {
                lj = -lj;
            }
            lambda[j] = lj;
            b = theta * wj + s;
            gamma[j] = b * nu / lj;
            beta[j] = (a - b * eta) / lj;
            nu = -nu / lj;
            /* Computing 2nd power */
            d__1 = a;
            eta = -(eta + d__1 * d__1 / (theta - lj)) / lj;
            /* L20: */
        }
L30:
        lambda[*n] = (nu * z__[*n] - eta * w[*n]) * w[*n] + 1.;

        /*  ***  UPDATE L, GRADUALLY OVERWRITING  W  AND  Z  WITH  L*W  AND  L*Z. */

        np1 = *n + 1;
        jj = *n * (*n + 1) / 2;
        i__1 = *n;
        for (k = 1; k <= i__1; ++k) {
            j = np1 - k;
            lj = lambda[j];
            ljj = l[jj];
            lplus[jj] = lj * ljj;
            wj = w[j];
            w[j] = ljj * wj;
            zj = z__[j];
            z__[j] = ljj * zj;
            if (k == 1) {
                goto L50;
            }
            bj = beta[j];
            gj = gamma[j];
            ij = jj + j;
            jp1 = j + 1;
            i__2 = *n;
            for (i__ = jp1; i__ <= i__2; ++i__) {
                lij = l[ij];
                lplus[ij] = lj * lij + bj * w[i__] + gj * z__[i__];
                w[i__] += lij * wj;
                z__[i__] += lij * zj;
                ij += i__;
                /* L40: */
            }
L50:
            jj -= j;
            /* L60: */
        }

        /* L999: */
        return 0;
        /*  ***  LAST CARD OF DL7UPD FOLLOWS  *** */
    } /* dl7upd_ */

    template<typename doublereal>
    int dparck_(integer *alg, doublereal *d__, integer *iv,
            integer *liv, integer *lv, integer *n, doublereal *v) {
        /* Initialized data */

        static doublereal big = 0.;
        static std::vector<std::string> cngd = {"---C", "HANG", "ED V"};
        static std::vector<std::string> dflt = {"NOND", "EFAU", "LT V"};
        static integer ijmp = 33;
        static integer jlim[4] = {0, 24, 0, 24};
        static integer ndflt[4] = {32, 25, 32, 25};
        static integer miniv[4] = {82, 59, 103, 103};
        static doublereal machep = -1.;
        static doublereal tiny = 1.;
        static doublereal zero = 0.;
        static std::vector<std::string> vn = {"EPSL", "ON..", "PHMN", "FC..", "PHMX", "FC..", "DECF",
            "AC..", "INCF", "AC..", "RDFC", "MN..", "RDFC", "MX..", "TUNE", "R1..",
            "TUNE", "R2..", "TUNE", "R3..", "TUNE", "R4..", "TUNE", "R5..", "AFCT",
            "OL..", "RFCT", "OL..", "XCTO", "L...", "XFTO", "L...", "LMAX", "0...",
            "LMAX", "S...", "SCTO", "L...", "DINI", "T...", "DTIN", "IT..", "D0IN",
            "IT..", "DFAC", "....", "DLTF", "DC..", "DLTF", "DJ..", "DELT", "A0..",
            "FUZZ", "....", "RLIM", "IT..", "COSM", "IN..", "HUBE", "RC..", "RSPT",
            "OL..", "SIGM", "IN..", "ETA0", "....", "BIAS", "...."};
        static doublereal vm[34] = {.001, -.99, .001, .01, 1.2, .01, 1.2, 0., 0., .001,
            -1., 0.0, 0., 0.0, 0., 0., 0.0, 0.0, 0., -10., 0., 0., 0., 0.0, 0.0, 0.0, 1.01,
            1e10, 0.0, 0., 0., 0., 0.0, 0.};
        static doublereal vx[34] = {.9, -.001, 10., .8, 100., .8, 100., .5, .5, 1., 1., 0.0,
            0.0, .1, 1., 1., 0.0, 0.0, 1., 0.0, 0.0, 0.0, 1., 1., 1., 1., 1e10, 0.0, 1., 0.0,
            1., 1., 1., 1.};
        static std::vector<std::string> varnm = {"P", "P"};
        static std::vector<std::string> sh = {"S", "H"};

        /* Format strings */
        static char fmt_10[] = "(/\002 THE FIRST PARAMETER TO DIVSET SHOULD B"
                "E\002,i3,\002 RATHER THAN\002,i3)";
        static char fmt_40[] = "(/\002 /// BAD\002,a1,\002 =\002,i5)";
        static char fmt_70[] = "(/\002 /// \002,1a1,\002 CHANGED FROM \002,i5"
                ",\002 TO \002,i5)";
        static char fmt_90[] = "(/\002 ///  IV(1) =\002,i5,\002 SHOULD BE BETWEE"
                "N 0 AND 14.\002)";
        static char fmt_130[] = "(/\002 ///  \002,2a4,\002.. V(\002,i2,\002) "
                "=\002,d11.3,\002 SHOULD\002,\002 BE BETWEEN\002,d11.3,\002 AN"
                "D\002,d11.3)";
        static char fmt_160[] = "(/\002 IV(NVDFLT) =\002,i5,\002 RATHER THAN "
                "\002,i5)";
        static char fmt_180[] = "(/\002 ///  D(\002,i3,\002) =\002,d11.3,\002 SH"
                "OULD BE POSITIVE\002)";
        static char fmt_220[] = "(/\002 NONDEFAULT VALUES....\002/\002 INIT\002,"
                "a1,\002..... IV(25) =\002,i3)";
        static char fmt_260[] = "(/\002 \002,3a4,\002ALUES....\002/)";
        static char fmt_240[] = "(\002 DTYPE..... IV(16) =\002,i3)";
        static char fmt_270[] = "(1x,2a4,\002.. V(\002,i2,\002) =\002,d15.7)";
        static char fmt_310[] = "(/\002 /// LIV =\002,i5,\002 MUST BE AT LEAS"
                "T\002,i5)";
        static char fmt_330[] = "(/\002 /// LV =\002,i5,\002 MUST BE AT LEAST"
                "\002,i5)";
        static char fmt_350[] = "(/\002 /// ALG =\002,i5,\002 MUST BE 1 2, 3, OR"
                " 4\002)";
        static char fmt_370[] = "(/\002 /// LIV =\002,i5,\002 MUST BE AT LEAS"
                "T\002,i5,\002 TO COMPUTE TRUE MIN. LIV AND MIN. LV\002)";

        /* System generated locals */
        integer i__1, i__2;

        /* Builtin functions */
        //        integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void);
        //        /* Subroutine */ int s_copy(char *, char *, ftnlen, ftnlen);

        /* Local variables */
        static integer i__, j, k, l, m, ii;
        static doublereal vk;
        static integer pu, iv1, alg1, miv1, miv2;
        static char which[4 * 3];
        //    extern doublereal dr7mdc_(integer *);
        //    extern /* Subroutine */ int dv7dfl_(integer *, integer *, doublereal *), 
        //	    dv7cpy_(integer *, doublereal *, doublereal *);
        static integer parsv1, ndfalt;
        //    extern /* Subroutine */ int divset_(integer *, integer *, integer *, 
        //	    integer *, doublereal *);

        /* Fortran I/O blocks */
        static cilist io___17 = {0, 0, 0, fmt_10, 0};
        static cilist io___22 = {0, 0, 0, fmt_40, 0};
        static cilist io___25 = {0, 0, 0, fmt_70, 0};
        static cilist io___26 = {0, 0, 0, fmt_90, 0};
        static cilist io___33 = {0, 0, 0, fmt_130, 0};
        static cilist io___34 = {0, 0, 0, fmt_160, 0};
        static cilist io___35 = {0, 0, 0, fmt_180, 0};
        static cilist io___36 = {0, 0, 0, fmt_220, 0};
        static cilist io___37 = {0, 0, 0, fmt_260, 0};
        static cilist io___38 = {0, 0, 0, fmt_240, 0};
        static cilist io___40 = {0, 0, 0, fmt_260, 0};
        static cilist io___41 = {0, 0, 0, fmt_270, 0};
        static cilist io___43 = {0, 0, 0, fmt_310, 0};
        static cilist io___44 = {0, 0, 0, fmt_330, 0};
        static cilist io___45 = {0, 0, 0, fmt_350, 0};
        static cilist io___46 = {0, 0, 0, fmt_370, 0};



        /*  ***  CHECK ***SOL (VERSION 2.3) PARAMETERS, PRINT CHANGED VALUES  *** */

        /*  ***  ALG = 1 FOR REGRESSION, ALG = 2 FOR GENERAL UNCONSTRAINED OPT. */


        /* DIVSET  -- SUPPLIES DEFAULT VALUES TO BOTH IV AND V. */
        /* DR7MDC -- RETURNS MACHINE-DEPENDENT CONSTANTS. */
        /* DV7CPY  -- COPIES ONE VECTOR TO ANOTHER. */
        /* DV7DFL  -- SUPPLIES DEFAULT PARAMETER VALUES TO V ALONE. */

        /*  ***  LOCAL VARIABLES  *** */

        /* /6S */
        /*     INTEGER VARNM(2), SH(2) */
        /*     REAL CNGD(3), DFLT(3), VN(2,34), WHICH(3) */
        /* /7S */
        /* / */

        /*  ***  IV AND V SUBSCRIPTS  *** */



        /* /6 */
        /*     DATA ALGSAV/51/, DINIT/38/, DTYPE/16/, DTYPE0/54/, EPSLON/19/, */
        /*    1     INITS/25/, IVNEED/3/, LASTIV/44/, LASTV/45/, LMAT/42/, */
        /*    2     NEXTIV/46/, NEXTV/47/, NVDFLT/50/, OLDN/38/, PARPRT/20/, */
        /*    3     PARSAV/49/, PERM/58/, PRUNIT/21/, VNEED/4/ */
        /* /7 */
        /* / */

        /* Parameter adjustments */
        --iv;
        --v;
        --d__;

        /* Function Body */
        /* /6S */
        /*     DATA VN(1,1),VN(2,1)/4HEPSL,4HON../ */
        /*     DATA VN(1,2),VN(2,2)/4HPHMN,4HFC../ */
        /*     DATA VN(1,3),VN(2,3)/4HPHMX,4HFC../ */
        /*     DATA VN(1,4),VN(2,4)/4HDECF,4HAC../ */
        /*     DATA VN(1,5),VN(2,5)/4HINCF,4HAC../ */
        /*     DATA VN(1,6),VN(2,6)/4HRDFC,4HMN../ */
        /*     DATA VN(1,7),VN(2,7)/4HRDFC,4HMX../ */
        /*     DATA VN(1,8),VN(2,8)/4HTUNE,4HR1../ */
        /*     DATA VN(1,9),VN(2,9)/4HTUNE,4HR2../ */
        /*     DATA VN(1,10),VN(2,10)/4HTUNE,4HR3../ */
        /*     DATA VN(1,11),VN(2,11)/4HTUNE,4HR4../ */
        /*     DATA VN(1,12),VN(2,12)/4HTUNE,4HR5../ */
        /*     DATA VN(1,13),VN(2,13)/4HAFCT,4HOL../ */
        /*     DATA VN(1,14),VN(2,14)/4HRFCT,4HOL../ */
        /*     DATA VN(1,15),VN(2,15)/4HXCTO,4HL.../ */
        /*     DATA VN(1,16),VN(2,16)/4HXFTO,4HL.../ */
        /*     DATA VN(1,17),VN(2,17)/4HLMAX,4H0.../ */
        /*     DATA VN(1,18),VN(2,18)/4HLMAX,4HS.../ */
        /*     DATA VN(1,19),VN(2,19)/4HSCTO,4HL.../ */
        /*     DATA VN(1,20),VN(2,20)/4HDINI,4HT.../ */
        /*     DATA VN(1,21),VN(2,21)/4HDTIN,4HIT../ */
        /*     DATA VN(1,22),VN(2,22)/4HD0IN,4HIT../ */
        /*     DATA VN(1,23),VN(2,23)/4HDFAC,4H..../ */
        /*     DATA VN(1,24),VN(2,24)/4HDLTF,4HDC../ */
        /*     DATA VN(1,25),VN(2,25)/4HDLTF,4HDJ../ */
        /*     DATA VN(1,26),VN(2,26)/4HDELT,4HA0../ */
        /*     DATA VN(1,27),VN(2,27)/4HFUZZ,4H..../ */
        /*     DATA VN(1,28),VN(2,28)/4HRLIM,4HIT../ */
        /*     DATA VN(1,29),VN(2,29)/4HCOSM,4HIN../ */
        /*     DATA VN(1,30),VN(2,30)/4HHUBE,4HRC../ */
        /*     DATA VN(1,31),VN(2,31)/4HRSPT,4HOL../ */
        /*     DATA VN(1,32),VN(2,32)/4HSIGM,4HIN../ */
        /*     DATA VN(1,33),VN(2,33)/4HETA0,4H..../ */
        /*     DATA VN(1,34),VN(2,34)/4HBIAS,4H..../ */
        /* /7S */
        /* / */


        /* /6S */
        /*     DATA VARNM(1)/1HP/, VARNM(2)/1HP/, SH(1)/1HS/, SH(2)/1HH/ */
        /*     DATA CNGD(1),CNGD(2),CNGD(3)/4H---C,4HHANG,4HED V/, */
        /*    1     DFLT(1),DFLT(2),DFLT(3)/4HNOND,4HEFAU,4HLT V/ */
        /* /7S */
        /* / */

        /* ...............................  BODY  ................................ */

        pu = 0;
        if (21 <= *liv) {
            pu = iv[21];
        }
        if (51 > *liv) {
            goto L20;
        }
        if (*alg == iv[51]) {
            goto L20;
        }
        if (pu != 0) {
            io___17.ciunit = pu;
            //	s_wsfe(&io___17);
            //	do_fio(&c__1, (char *)&(*alg), (ftnlen)sizeof(integer));
            //	do_fio(&c__1, (char *)&iv[51], (ftnlen)sizeof(integer));
            //	e_wsfe();
        }
        iv[1] = 67;
        goto L999;
L20:
        if (*alg < 1 || *alg > 4) {
            goto L340;
        }
        miv1 = miniv[*alg - 1];
        if (iv[1] == 15) {
            goto L360;
        }
        alg1 = (*alg - 1) % 2 + 1;
        if (iv[1] == 0) {
            divset_(alg, &iv[1], liv, lv, &v[1]);
            //std::cout<<__LINE__<<" "<<iv[1]<<"\n";
        }
        iv1 = iv[1];
        if (iv1 != 13 && iv1 != 12) {
            goto L30;
        }
        if (58 <= *liv) {
            /* Computing MAX */
            i__1 = miv1, i__2 = iv[58] - 1;
            miv1 = std::max(i__1, i__2);
        }
        if (3 <= *liv) {
            miv2 = miv1 + std::max(iv[3], 0l);
        }
        if (44 <= *liv) {
            iv[44] = miv2;
        }
        if (*liv < miv1) {
            goto L300;
        }
        iv[3] = 0;
        iv[45] = std::max(iv[4], 0l) + iv[42] - 1;
        iv[4] = 0;
        if (*liv < miv2) {
            goto L300;
        }
        if (*lv < iv[45]) {
            goto L320;
        }
L30:
        if (iv1 < 12 || iv1 > 14) {
            goto L60;
        }
        if (*n >= 1) {
            goto L50;
        }
        iv[1] = 81;
        if (pu == 0) {
            goto L999;
        }
        io___22.ciunit = pu;
        //    s_wsfe(&io___22);
        //    do_fio(&c__1, varnm + (alg1 - 1), (ftnlen)1);
        //    do_fio(&c__1, (char *)&(*n), (ftnlen)sizeof(integer));
        //    e_wsfe();
        goto L999;
L50:
        if (iv1 != 14) {
            iv[46] = iv[58];
        }
        if (iv1 != 14) {
            iv[47] = iv[42];
        }
        if (iv1 == 13) {
            goto L999;
        }
        k = iv[49] - 19;
        i__1 = *lv - k;
        dv7dfl_(&alg1, &i__1, &v[k + 1]);
        iv[54] = 2 - alg1;
        iv[38] = *n;
        //            s_copy(which, dflt, (ftnlen) 4, (ftnlen) 4);
        //            s_copy(which + 4, dflt + 4, (ftnlen) 4, (ftnlen) 4);
        //            s_copy(which + 8, dflt + 8, (ftnlen) 4, (ftnlen) 4);
        goto L110;
L60:
        if (*n == iv[38]) {
            goto L80;
        }
        iv[1] = 17;
        if (pu == 0) {
            goto L999;
        }
        io___25.ciunit = pu;
        //    s_wsfe(&io___25);
        //    do_fio(&c__1, varnm + (alg1 - 1), (ftnlen)1);
        //    do_fio(&c__1, (char *)&iv[38], (ftnlen)sizeof(integer));
        //    do_fio(&c__1, (char *)&(*n), (ftnlen)sizeof(integer));
        //    e_wsfe();
        goto L999;

L80:
        if (iv1 <= 11 && iv1 >= 1) {
            goto L100;
        }
        iv[1] = 80;
        if (pu != 0) {
            //	io___26.ciunit = pu;
            //	s_wsfe(&io___26);
            //	do_fio(&c__1, (char *)&iv1, (ftnlen)sizeof(integer));
            //	e_wsfe();
        }
        goto L999;

L100:
        //        s_copy(which, cngd, (ftnlen) 4, (ftnlen) 4);
        //        s_copy(which + 4, cngd + 4, (ftnlen) 4, (ftnlen) 4);
        //        s_copy(which + 8, cngd + 8, (ftnlen) 4, (ftnlen) 4);

        L110 :
        if (iv1 == 14) {
            iv1 = 12;
        }
        if (big > tiny) {
            goto L120;
        }
        tiny = dr7mdc_<doublereal>(&c__1);
        machep = dr7mdc_<doublereal>(&c__3);
        big = dr7mdc_<doublereal>(&c__6);
        vm[11] = machep;
        vx[11] = big;
        vx[12] = big;
        vm[13] = machep;
        vm[16] = tiny;
        vx[16] = big;
        vm[17] = tiny;
        vx[17] = big;
        vx[19] = big;
        vx[20] = big;
        vx[21] = big;
        vm[23] = machep;
        vm[24] = machep;
        vm[25] = machep;
        vx[27] = dr7mdc_<doublereal>(&c__5);
        vm[28] = machep;
        vx[29] = big;
        vm[32] = machep;
L120:
        m = 0;
        i__ = 1;
        j = jlim[alg1 - 1];
        k = 19;
        ndfalt = ndflt[alg1 - 1];
        i__1 = ndfalt;
        for (l = 1; l <= i__1; ++l) {
            vk = v[k];
            if (vk >= vm[i__ - 1] && vk <= vx[i__ - 1]) {
                goto L140;
            }
            m = k;
            if (pu != 0) {
                io___33.ciunit = pu;
                //	    s_wsfe(&io___33);
                //	    do_fio(&c__1, vn + ((i__ << 1) - 2 << 2), (ftnlen)4);
                //	    do_fio(&c__1, vn + ((i__ << 1) - 1 << 2), (ftnlen)4);
                //	    do_fio(&c__1, (char *)&k, (ftnlen)sizeof(integer));
                //	    do_fio(&c__1, (char *)&vk, (ftnlen)sizeof(doublereal));
                //	    do_fio(&c__1, (char *)&vm[i__ - 1], (ftnlen)sizeof(doublereal));
                //	    do_fio(&c__1, (char *)&vx[i__ - 1], (ftnlen)sizeof(doublereal));
                //	    e_wsfe();
            }
L140:
            ++k;
            ++i__;
            if (i__ == j) {
                i__ = ijmp;
            }
            /* L150: */
        }

        if (iv[50] == ndfalt) {
            goto L170;
        }
        iv[1] = 51;
        if (pu == 0) {
            goto L999;
        }
        io___34.ciunit = pu;
        //    s_wsfe(&io___34);
        //    do_fio(&c__1, (char *)&iv[50], (ftnlen)sizeof(integer));
        //    do_fio(&c__1, (char *)&ndfalt, (ftnlen)sizeof(integer));
        //    e_wsfe();
        goto L999;
L170:
        if ((iv[16] > 0 || v[38] > zero) && iv1 == 12) {
            goto L200;
        }
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            if (d__[i__] > zero) {
                goto L190;
            }
            m = 18;
            if (pu != 0) {
                io___35.ciunit = pu;
                //	    s_wsfe(&io___35);
                //	    do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
                //	    do_fio(&c__1, (char *)&d__[i__], (ftnlen)sizeof(doublereal));
                //	    e_wsfe();
            }
L190:
            ;
        }
L200:
        if (m == 0) {
            goto L210;
        }
        iv[1] = m;
        //std::cout<<"m = "<<m<<"\n";
        goto L999;

L210:
        if (pu == 0 || iv[20] == 0) {
            goto L999;
        }
        if (iv1 != 12 || iv[25] == alg1 - 1) {
            goto L230;
        }
        m = 1;
        io___36.ciunit = pu;
        //    s_wsfe(&io___36);
        //    do_fio(&c__1, sh + (alg1 - 1), (ftnlen)1);
        //    do_fio(&c__1, (char *)&iv[25], (ftnlen)sizeof(integer));
        //    e_wsfe();
L230:
        if (iv[16] == iv[54]) {
            goto L250;
        }
        if (m == 0) {
            io___37.ciunit = pu;
            //	s_wsfe(&io___37);
            //	do_fio(&c__3, which, (ftnlen)4);
            //	e_wsfe();
        }
        m = 1;
        io___38.ciunit = pu;
        //    s_wsfe(&io___38);
        //    do_fio(&c__1, (char *)&iv[16], (ftnlen)sizeof(integer));
        //    e_wsfe();
L250:
        i__ = 1;
        j = jlim[alg1 - 1];
        k = 19;
        l = iv[49];
        ndfalt = ndflt[alg1 - 1];
        i__1 = ndfalt;
        for (ii = 1; ii <= i__1; ++ii) {
            if (v[k] == v[l]) {
                goto L280;
            }
            if (m == 0) {
                io___40.ciunit = pu;
                //	    s_wsfe(&io___40);
                //	    do_fio(&c__3, which, (ftnlen)4);
                //	    e_wsfe();
            }
            m = 1;
            io___41.ciunit = pu;
            //	s_wsfe(&io___41);
            //	do_fio(&c__1, vn + ((i__ << 1) - 2 << 2), (ftnlen)4);
            //	do_fio(&c__1, vn + ((i__ << 1) - 1 << 2), (ftnlen)4);
            //	do_fio(&c__1, (char *)&k, (ftnlen)sizeof(integer));
            //	do_fio(&c__1, (char *)&v[k], (ftnlen)sizeof(doublereal));
            //	e_wsfe();
L280:
            ++k;
            ++l;
            ++i__;
            if (i__ == j) {
                i__ = ijmp;
            }
            /* L290: */
        }

        iv[54] = iv[16];
        parsv1 = iv[49];
        dv7cpy_(&iv[50], &v[parsv1], &v[19]);
        goto L999;

L300:
        iv[1] = 15;
        if (pu == 0) {
            goto L999;
        }
        io___43.ciunit = pu;
        //    s_wsfe(&io___43);
        //    do_fio(&c__1, (char *)&(*liv), (ftnlen)sizeof(integer));
        //    do_fio(&c__1, (char *)&miv2, (ftnlen)sizeof(integer));
        //    e_wsfe();
        if (*liv < miv1) {
            goto L999;
        }
        if (*lv < iv[45]) {
            goto L320;
        }
        goto L999;

L320:
        iv[1] = 16;
        if (pu != 0) {
            io___44.ciunit = pu;
            //	s_wsfe(&io___44);
            //	do_fio(&c__1, (char *)&(*lv), (ftnlen)sizeof(integer));
            //	do_fio(&c__1, (char *)&iv[45], (ftnlen)sizeof(integer));
            //	e_wsfe();
        }
        goto L999;

L340:
        iv[1] = 67;
        if (pu != 0) {
            io___45.ciunit = pu;
            //	s_wsfe(&io___45);
            //	do_fio(&c__1, (char *)&(*alg), (ftnlen)sizeof(integer));
            //	e_wsfe();
        }
        goto L999;
L360:
        if (pu != 0) {
            io___46.ciunit = pu;
            //	s_wsfe(&io___46);
            //	do_fio(&c__1, (char *)&(*liv), (ftnlen)sizeof(integer));
            //	do_fio(&c__1, (char *)&miv1, (ftnlen)sizeof(integer));
            //	e_wsfe();
        }
        if (44 <= *liv) {
            iv[44] = miv1;
        }
        if (45 <= *liv) {
            iv[45] = 0;
        }

L999:
        return 0;
        /*  ***  LAST LINE OF DPARCK FOLLOWS  *** */
    } /* dparck_ */

    template<typename doublereal>
    doublereal dh2rfg_(doublereal *a, doublereal *b, doublereal *x, doublereal *y,
            doublereal *z__) {
        /* Initialized data */

        static doublereal zero = 0.;

        /* System generated locals */
        doublereal ret_val, d__1, d__2;

        /* Builtin functions */
//        double sqrt(doublereal);

        /* Local variables */
        static doublereal c__, t, a1, b1;


        /*  ***  DETERMINE X, Y, Z SO  I + (1,Z)**T * (X,Y)  IS A 2X2 */
        /*  ***  HOUSEHOLDER REFLECTION SENDING (A,B)**T INTO (C,0)**T, */
        /*  ***  WHERE  C = -SIGN(A)*SQRT(A**2 + B**2)  IS THE VALUE DH2RFG */
        /*  ***  RETURNS. */


        /* /+ */
        /* / */

        /*  ***  BODY  *** */

        if (*b != zero) {
            goto L10;
        }
        *x = zero;
        *y = zero;
        *z__ = zero;
        ret_val = *a;
        goto L999;
L10:
        t = std::fabs(*a) + std::fabs(*b);
        a1 = *a / t;
        b1 = *b / t;
        /* Computing 2nd power */
        d__1 = a1;
        /* Computing 2nd power */
        d__2 = b1;
        c__ = sqrt(d__1 * d__1 + d__2 * d__2);
        if (a1 > zero) {
            c__ = -c__;
        }
        a1 -= c__;
        *z__ = b1 / a1;
        *x = a1 / c__;
        *y = b1 / c__;
        ret_val = t * c__;
L999:
        return ret_val;
        /*  ***  LAST LINE OF DH2RFG FOLLOWS  *** */
    } /* dh2rfg_ */

    template<typename doublereal>
    /* Subroutine */ int dh2rfa_(integer *n, doublereal *a, doublereal *b,
            doublereal *x, doublereal *y, doublereal *z__) {
        /* System generated locals */
        integer i__1;

        /* Local variables */
        static integer i__;
        static doublereal t;


        /*  ***  APPLY 2X2 HOUSEHOLDER REFLECTION DETERMINED BY X, Y, Z TO */
        /*  ***  N-VECTORS A, B  *** */

        /* Parameter adjustments */
        --b;
        --a;

        /* Function Body */
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            t = a[i__] * *x + b[i__] * *y;
            a[i__] += t;
            b[i__] += t * *z__;
            /* L10: */
        }
        /* L999: */
        return 0;
        /*  ***  LAST LINE OF DH2RFA FOLLOWS  *** */
    } /* dh2rfa_ */

    template<typename doublereal>
    /* Subroutine */ int dq7rsh_(integer *k, integer *p, logical *havqtr,
            doublereal *qtr, doublereal *r__, doublereal *w) {
        /* Initialized data */

        static doublereal zero = 0.;

        /* System generated locals */
        integer i__1, i__2;

        /* Local variables */
        static doublereal a, b;
        static integer i__, j;
        static doublereal t, x, y, z__;
        static integer i1, j1, k1;
        static doublereal wj;
        static integer jm1, km1, jp1, pm1;
        //        extern /* Subroutine */ int dh2rfa_(integer *, doublereal *, doublereal *,
        //                doublereal *, doublereal *, doublereal *);
        //        extern doublereal dh2rfg_(doublereal *, doublereal *, doublereal *,
        //                doublereal *, doublereal *);
        //        extern /* Subroutine */ int dv7cpy_(integer *, doublereal *, doublereal *)
        //                ;


        /*  ***  PERMUTE COLUMN K OF R TO COLUMN P, MODIFY QTR ACCORDINGLY  *** */

        /*     DIMSNSION R(P*(P+1)/2) */


        /*  ***  LOCAL VARIABLES  *** */


        /* Parameter adjustments */
        --w;
        --qtr;
        --r__;

        /* Function Body */

        /* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

        if (*k >= *p) {
            goto L999;
        }
        km1 = *k - 1;
        k1 = *k * km1 / 2;
        dv7cpy_(k, &w[1], &r__[k1 + 1]);
        wj = w[*k];
        pm1 = *p - 1;
        j1 = k1 + km1;
        i__1 = pm1;
        for (j = *k; j <= i__1; ++j) {
            jm1 = j - 1;
            jp1 = j + 1;
            if (jm1 > 0) {
                dv7cpy_(&jm1, &r__[k1 + 1], &r__[j1 + 2]);
            }
            j1 += jp1;
            k1 += j;
            a = r__[j1];
            b = r__[j1 + 1];
            if (b != zero) {
                goto L10;
            }
            r__[k1] = a;
            x = zero;
            z__ = zero;
            goto L40;
L10:
            r__[k1] = dh2rfg_(&a, &b, &x, &y, &z__);
            if (j == pm1) {
                goto L30;
            }
            i1 = j1;
            i__2 = pm1;
            for (i__ = jp1; i__ <= i__2; ++i__) {
                i1 += i__;
                dh2rfa_(&c__1, &r__[i1], &r__[i1 + 1], &x, &y, &z__);
                /* L20: */
            }
L30:
            if (*havqtr) {
                dh2rfa_(&c__1, &qtr[j], &qtr[jp1], &x, &y, &z__);
            }
L40:
            t = x * wj;
            w[j] = wj + t;
            wj = t * z__;
            /* L50: */
        }
        w[*p] = wj;
        dv7cpy_(p, &r__[k1 + 1], &w[1]);
L999:
        return 0;
    } /* dq7rsh_ */

    template<typename doublereal>
    /* Subroutine */ int dv7ipr_(integer *n, integer *ip, doublereal *x) {
        /* System generated locals */
        integer i__1;

        /* Local variables */
        static integer i__, j, k;
        static doublereal t;


        /*     PERMUTE X SO THAT X.OUTPUT(I) = X.INPUT(IP(I)). */
        /*     IP IS UNCHANGED ON OUTPUT. */


        /* Parameter adjustments */
        --x;
        --ip;

        /* Function Body */
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            j = ip[i__];
            if (j == i__) {
                goto L30;
            }
            if (j > 0) {
                goto L10;
            }
            ip[i__] = -j;
            goto L30;
L10:
            t = x[i__];
            k = i__;
L20:
            x[k] = x[j];
            k = j;
            j = ip[k];
            ip[k] = -j;
            if (j > i__) {
                goto L20;
            }
            x[k] = t;
L30:
            ;
        }
        /* L999: */
        return 0;
        /*  ***  LAST LINE OF DV7IPR FOLLOWS  *** */
    } /* dv7ipr_ */

    template<typename doublereal>
    /* Subroutine */ int dv7shf_(integer *n, integer *k, doublereal *x) {
        /* System generated locals */
        integer i__1;

        /* Local variables */
        static integer i__;
        static doublereal t;
        static integer nm1;


        /*  ***  SHIFT X(K),...,X(N) LEFT CIRCULARLY ONE POSITION  *** */



        /* Parameter adjustments */
        --x;

        /* Function Body */
        if (*k >= *n) {
            goto L999;
        }
        nm1 = *n - 1;
        t = x[*k];
        i__1 = nm1;
        for (i__ = *k; i__ <= i__1; ++i__) {
            /* L10: */
            x[i__] = x[i__ + 1];
        }
        x[*n] = t;
L999:
        return 0;
    } /* dv7shf_ */

    int i7shft_(integer *n, integer *k, integer *x) {
        /* System generated locals */
        integer i__1;

        /* Local variables */
        static integer i__, t, k1, ii, nm1;


        /*  ***  SHIFT X(K),...,X(N) LEFT CIRCULARLY ONE POSITION IF K .GT. 0. */
        /*  ***  SHIFT X(-K),...,X(N) RIGHT CIRCULARLY ONE POSITION IF K .LT. 0. */



        /* Parameter adjustments */
        --x;

        /* Function Body */
        if (*k < 0) {
            goto L20;
        }
        if (*k >= *n) {
            goto L999;
        }
        nm1 = *n - 1;
        t = x[*k];
        i__1 = nm1;
        for (i__ = *k; i__ <= i__1; ++i__) {
            /* L10: */
            x[i__] = x[i__ + 1];
        }
        x[*n] = t;
        goto L999;

L20:
        k1 = -(*k);
        if (k1 >= *n) {
            goto L999;
        }
        t = x[*n];
        nm1 = *n - k1;
        i__1 = nm1;
        for (ii = 1; ii <= i__1; ++ii) {
            i__ = *n - ii;
            x[i__ + 1] = x[i__];
            /* L30: */
        }
        x[k1] = t;
L999:
        return 0;
        /*  ***  LAST LINE OF I7SHFT FOLLOWS  *** */
    } /* i7shft_ */

    template<typename doublereal>
    /* Subroutine */ int dd7dgb_(doublereal *b, doublereal *d__, doublereal *dig,
            doublereal *dst, doublereal *g, integer *ipiv, integer *ka,
            doublereal *l, integer *lv, integer *p, integer *pc, doublereal *
            nwtst, doublereal *step, doublereal *td, doublereal *tg, doublereal *
            v, doublereal *w, doublereal *x0) {
        /* Initialized data */

        static doublereal meps2 = 0.;

        /* System generated locals */
        integer i__1, i__2;
        doublereal d__1, d__2, d__3, d__4;

        /* Local variables */
        static integer i__, j, k;
        static doublereal t;
        static integer p1;
        static doublereal t1, t2, ti, xi, x0i, rad;
        static integer p1m1;
        static doublereal nred, pred, gnorm;
        //    extern /* Subroutine */ int dd7dog_(doublereal *, integer *, integer *, 
        //	    doublereal *, doublereal *, doublereal *);
        //    extern doublereal dr7mdc_(integer *);
        //    extern /* Subroutine */ int dv7shf_(integer *, integer *, doublereal *), 
        //	    dl7ivm_(integer *, doublereal *, doublereal *, doublereal *);
        static doublereal gnorm0;
        //    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
        //    extern /* Subroutine */ int i7shft_(integer *, integer *, integer *), 
        //	    dl7vml_(integer *, doublereal *, doublereal *, doublereal *), 
        //	    dv7scp_(integer *, doublereal *, doublereal *);
        //    extern doublereal dv2nrm_(integer *, doublereal *);
        //    extern /* Subroutine */ int dl7itv_(integer *, doublereal *, doublereal *,
        //	     doublereal *), dq7rsh_(integer *, integer *, logical *, 
        //	    doublereal *, doublereal *, doublereal *), dv7ipr_(integer *, 
        //	    integer *, doublereal *), dv7cpy_(integer *, doublereal *, 
        //	    doublereal *), dl7tvm_(integer *, doublereal *, doublereal *, 
        //	    doublereal *), dv2axy_(integer *, doublereal *, doublereal *, 
        //	    doublereal *, doublereal *), dv7vmp_(integer *, doublereal *, 
        //	    doublereal *, doublereal *, integer *);
        static doublereal ghinvg, dnwtst;


        /*  ***  COMPUTE DOUBLE-DOGLEG STEP, SUBJECT TO SIMPLE BOUNDS ON X  *** */


        /*     DIMENSION L(P*(P+1)/2) */


        /*  ***  LOCAL VARIABLES  *** */


        /*  ***  V SUBSCRIPTS  *** */


        /* /6 */
        /*     DATA DGNORM/1/, DST0/3/, DSTNRM/2/, GRDFAC/45/, GTHG/44/, */
        /*    1     GTSTEP/4/, NREDUC/6/, NWTFAC/46/, PREDUC/7/, RADIUS/8/, */
        /*    2     STPPAR/5/ */
        /* /7 */
        /* / */
        /* /6 */
        /*     DATA HALF/0.5D+0/, ONE/1.D+0/, TWO/2.D+0/, ZERO/0.D+0/ */
        /* /7 */
        /* / */
        /* Parameter adjustments */
        --l;
        --v;
        --x0;
        --w;
        --tg;
        --td;
        --step;
        --nwtst;
        --ipiv;
        --g;
        --dst;
        --dig;
        --d__;
        b -= 3;

        /* Function Body */

        /* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

        if (meps2 <= 0.) {
            meps2 = 2. * dr7mdc_<doublereal>(&c__3);
        }
        gnorm0 = v[1];
        v[2] = 0.;
        if (*ka < 0) {
            goto L10;
        }
        dnwtst = v[3];
        nred = v[6];
L10:
        pred = 0.;
        v[5] = 0.;
        rad = v[8];
        if (*pc > 0) {
            goto L20;
        }
        dnwtst = 0.;
        dv7scp_(p, &step[1], &c_b5);
        goto L140;

L20:
        p1 = *pc;
        dv7cpy_(p, &td[1], &d__[1]);
        dv7ipr_(p, &ipiv[1], &td[1]);
        dv7scp_(pc, &dst[1], &c_b5);
        dv7cpy_(p, &tg[1], &g[1]);
        dv7ipr_(p, &ipiv[1], &tg[1]);

L30:
        dl7ivm_(&p1, &nwtst[1], &l[1], &tg[1]);
        ghinvg = dd7tpr_(&p1, &nwtst[1], &nwtst[1]);
        v[6] = ghinvg * .5;
        dl7itv_(&p1, &nwtst[1], &l[1], &nwtst[1]);
        dv7vmp_(&p1, &step[1], &nwtst[1], &td[1], &c__1);
        v[3] = dv2nrm_(pc, &step[1]);
        if (*ka >= 0) {
            goto L40;
        }
        *ka = 0;
        dnwtst = v[3];
        nred = v[6];
L40:
        v[8] = rad - v[2];
        if (v[8] <= 0.) {
            goto L100;
        }
        dv7vmp_(&p1, &dig[1], &tg[1], &td[1], &c_n1);
        gnorm = dv2nrm_(&p1, &dig[1]);
        if (gnorm <= 0.) {
            goto L100;
        }
        v[1] = gnorm;
        dv7vmp_(&p1, &dig[1], &dig[1], &td[1], &c_n1);
        dl7tvm_(&p1, &w[1], &l[1], &dig[1]);
        v[44] = dv2nrm_(&p1, &w[1]);
        ++(*ka);
        dd7dog_(&dig[1], lv, &p1, &nwtst[1], &step[1], &v[1]);

        /*     ***  FIND T SUCH THAT X - T*STEP IS STILL FEASIBLE. */

        t = 1.;
        k = 0;
        i__1 = p1;
        for (i__ = 1; i__ <= i__1; ++i__) {
            j = ipiv[i__];
            x0i = x0[j] + dst[i__] / td[i__];
            xi = x0i + step[i__];
            if (xi < b[(j << 1) + 1]) {
                goto L50;
            }
            if (xi <= b[(j << 1) + 2]) {
                goto L70;
            }
            ti = (b[(j << 1) + 2] - x0i) / step[i__];
            j = i__;
            goto L60;
L50:
            ti = (b[(j << 1) + 1] - x0i) / step[i__];
            j = -i__;
L60:
            if (t <= ti) {
                goto L70;
            }
            k = j;
            t = ti;
L70:
            ;
        }

        /*  ***  UPDATE DST, TG, AND PRED  *** */

        dv7vmp_(&p1, &step[1], &step[1], &td[1], &c__1);
        dv2axy_(&p1, &dst[1], &t, &step[1], &dst[1]);
        v[2] = dv2nrm_(pc, &dst[1]);
        t1 = t * v[45];
        t2 = t * v[46];
        /* Computing 2nd power */
        d__1 = v[44] * t1;
        pred = pred - t1 * gnorm * ((t2 + 1.) * gnorm) - t2 * (t2 * .5 + 1.) *
                ghinvg - d__1 * d__1 * .5;
        if (k == 0) {
            goto L100;
        }
        dl7vml_(&p1, &w[1], &l[1], &w[1]);
        t2 = 1. - t2;
        i__1 = p1;
        for (i__ = 1; i__ <= i__1; ++i__) {
            /* L80: */
            tg[i__] = t2 * tg[i__] - t1 * w[i__];
        }

        /*     ***  PERMUTE L, ETC. IF NECESSARY  *** */

        p1m1 = p1 - 1;
        j = std::fabs(k);
        if (j == p1) {
            goto L90;
        }
        dq7rsh_<doublereal>(&j, &p1, &c_false, &tg[1], &l[1], &w[1]);
        i7shft_(&p1, &j, &ipiv[1]);
        dv7shf_(&p1, &j, &tg[1]);
        dv7shf_(&p1, &j, &td[1]);
        dv7shf_(&p1, &j, &dst[1]);
L90:
        if (k < 0) {
            ipiv[p1] = -ipiv[p1];
        }
        p1 = p1m1;
        if (p1 > 0) {
            goto L30;
        }

        /*     ***  UNSCALE STEP, UPDATE X AND DIHDI  *** */

L100:
        dv7scp_(p, &step[1], &c_b5);
        i__1 = *pc;
        for (i__ = 1; i__ <= i__1; ++i__) {
            j = (i__2 = ipiv[i__], std::fabs(i__2));
            step[j] = dst[i__] / td[i__];
            /* L110: */
        }

        /*  ***  FUDGE STEP TO ENSURE THAT IT FORCES APPROPRIATE COMPONENTS */
        /*  ***  TO THEIR BOUNDS  *** */

        if (p1 >= *pc) {
            goto L140;
        }
        dv2axy_(p, &td[1], &c_b23, &step[1], &x0[1]);
        k = p1 + 1;
        i__1 = *pc;
        for (i__ = k; i__ <= i__1; ++i__) {
            j = ipiv[i__];
            t = meps2;
            if (j > 0) {
                goto L120;
            }
            t = -t;
            j = -j;
            ipiv[i__] = j;
L120:
            /* Computing MAX */
            d__3 = (d__1 = td[j], std::fabs(d__1)), d__4 = (d__2 = x0[j], std::fabs(d__2));
            t *= std::max(d__3, d__4);
            step[j] += t;
            /* L130: */
        }

L140:
        v[1] = gnorm0;
        v[6] = nred;
        v[7] = pred;
        v[8] = rad;
        v[3] = dnwtst;
        v[4] = dd7tpr_(p, &step[1], &g[1]);

        /* L999: */
        return 0;
        /*  ***  LAST LINE OF DD7DGB FOLLOWS  *** */
    } /* dd7dgb_ */

    template<typename doublereal>
    int drmng_(doublereal *d__, doublereal *fx, doublereal *g,
            integer *iv, integer *liv, integer *lv, integer *n, doublereal *v,
            doublereal *x) {
        /* System generated locals */
        integer i__1;
        doublereal c_b33_l = static_cast<doublereal> (c_b33);
        doublereal c_b44_l = static_cast<doublereal> (c_b44);
        doublereal c_b13_l = static_cast<doublereal> (c_b13);
        /* Local variables */
        static integer i__, k, l;
        static doublereal t;
        static integer w, z__, g01, x01, dg1, temp1, step1, dummy;
        //    //extern /* Subroutine */ int dd7dog_(doublereal *, integer *, integer *, 
        //	    doublereal *, doublereal *, doublereal *);
        //    //extern logical stopx_(integer *);
        //    //extern /* Subroutine */ int dl7upd_(doublereal *, doublereal *, 
        //	    doublereal *, doublereal *, doublereal *, integer *, doublereal *,
        //	     doublereal *), dl7ivm_(integer *, doublereal *, doublereal *, 
        //	    doublereal *), dw7zbf_(doublereal *, integer *, doublereal *, 
        //	    doublereal *, doublereal *, doublereal *);
        //    //extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
        //    //extern /* Subroutine */ int da7sst_(integer *, integer *, integer *, 
        //	    doublereal *), dl7vml_(integer *, doublereal *, doublereal *, 
        //	    doublereal *), dv7scp_(integer *, doublereal *, doublereal *);
        //    //extern doublereal dv2nrm_(integer *, doublereal *);
        //    //extern /* Subroutine */ int dl7itv_(integer *, doublereal *, doublereal *,
        //	     doublereal *), dv7cpy_(integer *, doublereal *, doublereal *), 
        //	    dl7tvm_(integer *, doublereal *, doublereal *, doublereal *), 
        //	    dv2axy_(integer *, doublereal *, doublereal *, doublereal *, 
        //	    doublereal *), dv7vmp_(integer *, doublereal *, doublereal *, 
        //	    doublereal *, integer *);
        static integer nwtst1;
        //    //extern /* Subroutine */ int dparck_(integer *, doublereal *, integer *, 
        //	    integer *, integer *, integer *, doublereal *);
        //    //extern doublereal drldst_(integer *, doublereal *, doublereal *, 
        //	    doublereal *);
        //    //extern /* Subroutine */ int divset_(integer *, integer *, integer *, 
        //	    integer *, doublereal *), ditsum_(doublereal *, doublereal *, 
        //	    integer *, integer *, integer *, integer *, doublereal *, 
        //	    doublereal *);
        static integer lstgst, rstrst;


        /*  ***  CARRY OUT  DMNG (UNCONSTRAINED MINIMIZATION) ITERATIONS, USING */
        /*  ***  DOUBLE-DOGLEG/BFGS STEPS. */

        /*  ***  PARAMETER DECLARATIONS  *** */


        /* --------------------------  PARAMETER USAGE  -------------------------- */

        /* D.... SCALE VECTOR. */
        /* FX... FUNCTION VALUE. */
        /* G.... GRADIENT VECTOR. */
        /* IV... INTEGER VALUE ARRAY. */
        /* LIV.. LENGTH OF IV (AT LEAST 60). */
        /* LV... LENGTH OF V (AT LEAST 71 + N*(N+13)/2). */
        /* N.... NUMBER OF VARIABLES (COMPONENTS IN X AND G). */
        /* V.... FLOATING-POINT VALUE ARRAY. */
        /* X.... VECTOR OF PARAMETERS TO BE OPTIMIZED. */

        /*  ***  DISCUSSION  *** */

        /*        PARAMETERS IV, N, V, AND X ARE THE SAME AS THE CORRESPONDING */
        /*     ONES TO  DMNG (WHICH SEE), EXCEPT THAT V CAN BE SHORTER (SINCE */
        /*     THE PART OF V THAT  DMNG USES FOR STORING G IS NOT NEEDED). */
        /*     MOREOVER, COMPARED WITH  DMNG, IV(1) MAY HAVE THE TWO ADDITIONAL */
        /*     OUTPUT VALUES 1 AND 2, WHICH ARE EXPLAINED BELOW, AS IS THE USE */
        /*     OF IV(TOOBIG) AND IV(NFGCAL).  THE VALUE IV(G), WHICH IS AN */
        /*     OUTPUT VALUE FROM  DMNG (AND  DMNF), IS NOT REFERENCED BY */
        /*     DRMNG OR THE SUBROUTINES IT CALLS. */
        /*        FX AND G NEED NOT HAVE BEEN INITIALIZED WHEN DRMNG IS CALLED */
        /*     WITH IV(1) = 12, 13, OR 14. */

        /* IV(1) = 1 MEANS THE CALLER SHOULD SET FX TO F(X), THE FUNCTION VALUE */
        /*             AT X, AND CALL DRMNG AGAIN, HAVING CHANGED NONE OF THE */
        /*             OTHER PARAMETERS.  AN EXCEPTION OCCURS IF F(X) CANNOT BE */
        /*             (E.G. IF OVERFLOW WOULD OCCUR), WHICH MAY HAPPEN BECAUSE */
        /*             OF AN OVERSIZED STEP.  IN THIS CASE THE CALLER SHOULD SET */
        /*             IV(TOOBIG) = IV(2) TO 1, WHICH WILL CAUSE DRMNG TO IG- */
        /*             NORE FX AND TRY A SMALLER STEP.  THE PARAMETER NF THAT */
        /*              DMNG PASSES TO CALCF (FOR POSSIBLE USE BY CALCG) IS A */
        /*             COPY OF IV(NFCALL) = IV(6). */
        /* IV(1) = 2 MEANS THE CALLER SHOULD SET G TO G(X), THE GRADIENT VECTOR */
        /*             OF F AT X, AND CALL DRMNG AGAIN, HAVING CHANGED NONE OF */
        /*             THE OTHER PARAMETERS EXCEPT POSSIBLY THE SCALE VECTOR D */
        /*             WHEN IV(DTYPE) = 0.  THE PARAMETER NF THAT  DMNG PASSES */
        /*             TO CALCG IS IV(NFGCAL) = IV(7).  IF G(X) CANNOT BE */
        /*             EVALUATED, THEN THE CALLER MAY SET IV(TOOBIG) TO 0, IN */
        /*             WHICH CASE DRMNG WILL RETURN WITH IV(1) = 65. */
        /* . */
        /*  ***  GENERAL  *** */

        /*     CODED BY DAVID M. GAY (DECEMBER 1979).  REVISED SEPT. 1982. */
        /*     THIS SUBROUTINE WAS WRITTEN IN CONNECTION WITH RESEARCH SUPPORTED */
        /*     IN PART BY THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS */
        /*     MCS-7600324 AND MCS-7906671. */

        /*        (SEE  DMNG FOR REFERENCES.) */

        /* +++++++++++++++++++++++++++  DECLARATIONS  ++++++++++++++++++++++++++++ */

        /*  ***  LOCAL VARIABLES  *** */


        /*     ***  CONSTANTS  *** */


        /*  ***  NO INTRINSIC FUNCTIONS  *** */

        /*  ***  EXTERNAL FUNCTIONS AND SUBROUTINES  *** */


        /* DA7SST.... ASSESSES CANDIDATE STEP. */
        /* DD7DOG.... COMPUTES DOUBLE-DOGLEG (CANDIDATE) STEP. */
        /* DIVSET.... SUPPLIES DEFAULT IV AND V INPUT COMPONENTS. */
        /* DD7TPR... RETURNS INNER PRODUCT OF TWO VECTORS. */
        /* DITSUM.... PRINTS ITERATION SUMMARY AND INFO ON INITIAL AND FINAL X. */
        /* DL7ITV... MULTIPLIES INVERSE TRANSPOSE OF LOWER TRIANGLE TIMES VECTOR. */
        /* DL7IVM... MULTIPLIES INVERSE OF LOWER TRIANGLE TIMES VECTOR. */
        /* DL7TVM... MULTIPLIES TRANSPOSE OF LOWER TRIANGLE TIMES VECTOR. */
        /* LUPDT.... UPDATES CHOLESKY FACTOR OF HESSIAN APPROXIMATION. */
        /* DL7VML.... MULTIPLIES LOWER TRIANGLE TIMES VECTOR. */
        /* DPARCK.... CHECKS VALIDITY OF INPUT IV AND V VALUES. */
        /* DRLDST... COMPUTES V(RELDX) = RELATIVE STEP SIZE. */
        /* STOPX.... RETURNS .TRUE. IF THE BREAK KEY HAS BEEN PRESSED. */
        /* DV2AXY.... COMPUTES SCALAR TIMES ONE VECTOR PLUS ANOTHER. */
        /* DV7CPY.... COPIES ONE VECTOR TO ANOTHER. */
        /* DV7SCP... SETS ALL ELEMENTS OF A VECTOR TO A SCALAR. */
        /* DV7VMP... MULTIPLIES VECTOR BY VECTOR RAISED TO POWER (COMPONENTWISE). */
        /* DV2NRM... RETURNS THE 2-NORM OF A VECTOR. */
        /* DW7ZBF... COMPUTES W AND Z FOR DL7UPD CORRESPONDING TO BFGS UPDATE. */

        /*  ***  SUBSCRIPTS FOR IV AND V  *** */


        /*  ***  IV SUBSCRIPT VALUES  *** */

        /* /6 */
        /*     DATA CNVCOD/55/, DG/37/, G0/48/, INITH/25/, IRC/29/, KAGQT/33/, */
        /*    1     MODE/35/, MODEL/5/, MXFCAL/17/, MXITER/18/, NFCALL/6/, */
        /*    2     NFGCAL/7/, NGCALL/30/, NITER/31/, NWTSTP/34/, RADINC/8/, */
        /*    3     RESTOR/9/, STEP/40/, STGLIM/11/, STLSTG/41/, TOOBIG/2/, */
        /*    4     VNEED/4/, XIRC/13/, X0/43/ */
        /* /7 */
        /* / */

        /*  ***  V SUBSCRIPT VALUES  *** */

        /* /6 */
        /*     DATA DGNORM/1/, DINIT/38/, DSTNRM/2/, DST0/3/, F/10/, F0/13/, */
        /*    1     FDIF/11/, GTHG/44/, GTSTEP/4/, INCFAC/23/, LMAT/42/, */
        /*    2     LMAX0/35/, LMAXS/36/, NEXTV/47/, NREDUC/6/, PREDUC/7/, */
        /*    3     RADFAC/16/, RADIUS/8/, RAD0/9/, RELDX/17/, TUNER4/29/, */
        /*    4     TUNER5/30/ */
        /* /7 */
        /* / */

        /* /6 */
        /*     DATA HALF/0.5D+0/, NEGONE/-1.D+0/, ONE/1.D+0/, ONEP2/1.2D+0/, */
        /*    1     ZERO/0.D+0/ */
        /* /7 */
        /* / */

        /* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

        /* Parameter adjustments */
        --iv;
        --v;
        --x;
        --g;
        --d__;
        //std::cout<<__LINE__<<" "<<iv[1]<<"\n";
        /* Function Body */
        i__ = iv[1];
        //std::cout<<__LINE__<<" "<<iv[1]<<"\n";
        if (i__ == 1) {
            goto L50;
        }
        if (i__ == 2) {
            goto L60;
        }

        /*  ***  CHECK VALIDITY OF IV AND V INPUT VALUES  *** */

        if (iv[1] == 0) {
            divset_(&c__2, &iv[1], liv, lv, &v[1]);
            //std::cout<<__LINE__<<" "<<iv[1]<<"\n";
        }
        if (iv[1] == 12 || iv[1] == 13) {
            iv[4] += *n * (*n + 13) / 2;
        }
        //std::cout<<__LINE__<<" "<<iv[1]<<"\n";
        dparck_<doublereal>(&c__2, &d__[1], &iv[1], liv, lv, n, &v[1]);
        //std::cout<<__LINE__<<" "<<iv[1]<<"\n";
        i__ = iv[1] - 2;
        if (i__ > 12) {
            goto L999;
        }
        switch (i__) {
            case 1: goto L190;
            case 2: goto L190;
            case 3: goto L190;
            case 4: goto L190;
            case 5: goto L190;
            case 6: goto L190;
            case 7: goto L120;
            case 8: goto L90;
            case 9: goto L120;
            case 10: goto L10;
            case 11: goto L10;
            case 12: goto L20;
        }

        /*  ***  STORAGE ALLOCATION  *** */

L10:
        //std::cout<<__LINE__<<" "<<iv[1]<<"\n";
        l = iv[42];
        iv[43] = l + *n * (*n + 1) / 2;
        iv[40] = iv[43] + *n;
        iv[41] = iv[40] + *n;
        iv[48] = iv[41] + *n;
        iv[34] = iv[48] + *n;
        iv[37] = iv[34] + *n;
        iv[47] = iv[37] + *n;
        //std::cout<<__LINE__<<" "<<iv[1]<<"\n";
        if (iv[1] != 13) {
            goto L20;
        }
        iv[1] = 14;
        goto L999;

        /*  ***  INITIALIZATION  *** */

L20:
        iv[31] = 0;
        iv[6] = 1;
        iv[30] = 1;
        iv[7] = 1;
        iv[35] = -1;
        iv[5] = 1;
        iv[11] = 1;
        iv[2] = 0;
        iv[55] = 0;
        iv[8] = 0;
        v[9] = 0.;
        if (v[38] >= 0.) {
            dv7scp_(n, &d__[1], &v[38]);
        }
        if (iv[25] != 1) {
            goto L40;
        }

        /*     ***  SET THE INITIAL HESSIAN APPROXIMATION TO DIAG(D)**-2  *** */

        l = iv[42];
        i__1 = *n * (*n + 1) / 2;

        dv7scp_<doublereal>(&i__1, &v[l], &c_b13_l);
        k = l - 1;
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            k += i__;
            t = d__[i__];
            if (t <= 0.) {
                t = 1.;
            }
            v[k] = t;
            /* L30: */
        }

        /*  ***  COMPUTE INITIAL FUNCTION VALUE  *** */

L40:
        iv[1] = 1;
        goto L999;

L50:
        v[10] = *fx;
        if (iv[35] >= 0) {
            goto L190;
        }
        v[13] = *fx;
        iv[1] = 2;
        if (iv[2] == 0) {
            goto L999;
        }
        iv[1] = 63;
        goto L350;

        /*  ***  MAKE SURE GRADIENT COULD BE COMPUTED  *** */

L60:
        if (iv[2] == 0) {
            goto L70;
        }
        iv[1] = 65;
        goto L350;

L70:
        dg1 = iv[37];
        dv7vmp_(n, &v[dg1], &g[1], &d__[1], &c_n1);
        v[1] = dv2nrm_(n, &v[dg1]);

        if (iv[55] != 0) {
            goto L340;
        }
        if (iv[35] == 0) {
            goto L300;
        }

        /*  ***  ALLOW FIRST STEP TO HAVE SCALED 2-NORM AT MOST V(LMAX0)  *** */

        v[8] = v[35];

        iv[35] = 0;


        /* -----------------------------  MAIN LOOP  ----------------------------- */


        /*  ***  PRINT ITERATION SUMMARY, CHECK ITERATION LIMIT  *** */

L80:
        //    ditsum_(&d__[1], &g[1], &iv[1], liv, lv, n, &v[1], &x[1]);
        L90 :
                k = iv[31];
        if (k < iv[18]) {
            goto L100;
        }
        iv[1] = 10;
        goto L350;

        /*  ***  UPDATE RADIUS  *** */

L100:
        iv[31] = k + 1;
        if (k > 0) {
            v[8] = v[16] * v[2];
        }

        /*  ***  INITIALIZE FOR START OF NEXT ITERATION  *** */

        g01 = iv[48];
        x01 = iv[43];
        v[13] = v[10];
        iv[29] = 4;
        iv[33] = -1;

        /*     ***  COPY X TO X0, G TO G0  *** */

        dv7cpy_(n, &v[x01], &x[1]);
        dv7cpy_(n, &v[g01], &g[1]);

        /*  ***  CHECK STOPX AND FUNCTION EVALUATION LIMIT  *** */

L110:
        if (!stopx_(&dummy)) {
            goto L130;
        }
        iv[1] = 11;
        goto L140;

        /*     ***  COME HERE WHEN RESTARTING AFTER FUNC. EVAL. LIMIT OR STOPX. */

L120:
        if (v[10] >= v[13]) {
            goto L130;
        }
        v[16] = 1.;
        k = iv[31];
        goto L100;

L130:
        if (iv[6] < iv[17]) {
            goto L150;
        }
        iv[1] = 9;
L140:
        if (v[10] >= v[13]) {
            goto L350;
        }

        /*        ***  IN CASE OF STOPX OR FUNCTION EVALUATION LIMIT WITH */
        /*        ***  IMPROVED V(F), EVALUATE THE GRADIENT AT X. */

        iv[55] = iv[1];
        goto L290;

        /* . . . . . . . . . . . . .  COMPUTE CANDIDATE STEP  . . . . . . . . . . */

L150:
        step1 = iv[40];
        dg1 = iv[37];
        nwtst1 = iv[34];
        if (iv[33] >= 0) {
            goto L160;
        }
        l = iv[42];
        dl7ivm_(n, &v[nwtst1], &v[l], &g[1]);
        v[6] = dd7tpr_(n, &v[nwtst1], &v[nwtst1]) * .5;
        dl7itv_(n, &v[nwtst1], &v[l], &v[nwtst1]);
        dv7vmp_(n, &v[step1], &v[nwtst1], &d__[1], &c__1);
        v[3] = dv2nrm_(n, &v[step1]);
        dv7vmp_(n, &v[dg1], &v[dg1], &d__[1], &c_n1);
        dl7tvm_(n, &v[step1], &v[l], &v[dg1]);
        v[44] = dv2nrm_(n, &v[step1]);
        iv[33] = 0;
L160:
        dd7dog_(&v[dg1], lv, n, &v[nwtst1], &v[step1], &v[1]);
        if (iv[29] != 6) {
            goto L170;
        }
        if (iv[9] != 2) {
            goto L190;
        }
        rstrst = 2;
        goto L200;

        /*  ***  CHECK WHETHER EVALUATING F(X0 + STEP) LOOKS WORTHWHILE  *** */

L170:
        iv[2] = 0;
        if (v[2] <= 0.) {
            goto L190;
        }
        if (iv[29] != 5) {
            goto L180;
        }
        if (v[16] <= 1.) {
            goto L180;
        }
        if (v[7] > v[11] * 1.2) {
            goto L180;
        }
        if (iv[9] != 2) {
            goto L190;
        }
        rstrst = 0;
        goto L200;

        /*  ***  COMPUTE F(X0 + STEP)  *** */

L180:
        x01 = iv[43];
        step1 = iv[40];
        dv2axy_(n, &x[1], &c_b33_l, &v[step1], &v[x01]);
        ++iv[6];
        iv[1] = 1;
        goto L999;

        /* . . . . . . . . . . . . .  ASSESS CANDIDATE STEP  . . . . . . . . . . . */

L190:
        rstrst = 3;
L200:
        x01 = iv[43];
        v[17] = drldst_(n, &d__[1], &x[1], &v[x01]);
        da7sst_(&iv[1], liv, lv, &v[1]);
        //std::cout<<__LINE__<<" "<<iv[1]<<"\n";
        step1 = iv[40];
        lstgst = iv[41];
        i__ = iv[9] + 1;
        switch (i__) {
            case 1: goto L240;
            case 2: goto L210;
            case 3: goto L220;
            case 4: goto L230;
        }
L210:
        dv7cpy_(n, &x[1], &v[x01]);
        goto L240;
L220:
        dv7cpy_(n, &v[lstgst], &v[step1]);
        goto L240;
L230:
        dv7cpy_(n, &v[step1], &v[lstgst]);

        dv2axy_(n, &x[1], &c_b33_l, &v[step1], &v[x01]);
        v[17] = drldst_(n, &d__[1], &x[1], &v[x01]);
        iv[9] = rstrst;

L240:
        k = iv[29];
        switch (k) {
            case 1: goto L250;
            case 2: goto L280;
            case 3: goto L280;
            case 4: goto L280;
            case 5: goto L250;
            case 6: goto L260;
            case 7: goto L270;
            case 8: goto L270;
            case 9: goto L270;
            case 10: goto L270;
            case 11: goto L270;
            case 12: goto L270;
            case 13: goto L330;
            case 14: goto L300;
        }

        /*     ***  RECOMPUTE STEP WITH CHANGED RADIUS  *** */

L250:
        v[8] = v[16] * v[2];
        goto L110;

        /*  ***  COMPUTE STEP OF LENGTH V(LMAXS) FOR SINGULAR CONVERGENCE TEST. */

L260:
        v[8] = v[36];
        goto L150;

        /*  ***  CONVERGENCE OR FALSE CONVERGENCE  *** */

L270:
        iv[55] = k - 4;
        //std::cout<<__LINE__<<"iv[55] = "<<iv[55]<<"\n";
        if (v[10] >= v[13]) {
            goto L340;
        }
        if (iv[13] == 14) {
            goto L340;
        }
        iv[13] = 14;

        /* . . . . . . . . . . . .  PROCESS ACCEPTABLE STEP  . . . . . . . . . . . */

L280:
        if (iv[29] != 3) {
            goto L290;
        }
        step1 = iv[40];
        temp1 = iv[41];

        /*     ***  SET  TEMP1 = HESSIAN * STEP  FOR USE IN GRADIENT TESTS  *** */

        l = iv[42];
        dl7tvm_(n, &v[temp1], &v[l], &v[step1]);
        dl7vml_(n, &v[temp1], &v[l], &v[temp1]);

        /*  ***  COMPUTE GRADIENT  *** */

L290:
        ++iv[30];
        iv[1] = 2;
        goto L999;

        /*  ***  INITIALIZATIONS -- G0 = G - G0, ETC.  *** */

L300:
        g01 = iv[48];

        dv2axy_(n, &v[g01], &c_b44_l, &v[g01], &g[1]);
        step1 = iv[40];
        temp1 = iv[41];
        if (iv[29] != 3) {
            goto L320;
        }

        /*  ***  SET V(RADFAC) BY GRADIENT TESTS  *** */

        /*     ***  SET  TEMP1 = DIAG(D)**-1 * (HESSIAN*STEP + (G(X0)-G(X)))  *** */

        dv2axy_(n, &v[temp1], &c_b44_l, &v[g01], &v[temp1]);
        dv7vmp_(n, &v[temp1], &v[temp1], &d__[1], &c_n1);

        /*        ***  DO GRADIENT TESTS  *** */

        if (dv2nrm_(n, &v[temp1]) <= v[1] * v[29]) {
            goto L310;
        }
        if (dd7tpr_(n, &g[1], &v[step1]) >= v[4] * v[30]) {
            goto L320;
        }
L310:
        v[16] = v[23];

        /*  ***  UPDATE H, LOOP  *** */

L320:
        w = iv[34];
        z__ = iv[43];
        l = iv[42];
        dw7zbf_(&v[l], n, &v[step1], &v[w], &v[g01], &v[z__]);

        /*     ** USE THE N-VECTORS STARTING AT V(STEP1) AND V(G01) FOR SCRATCH.. */
        dl7upd_(&v[temp1], &v[step1], &v[l], &v[g01], &v[l], n, &v[w], &v[z__]);
        iv[1] = 2;
        goto L80;

        /* . . . . . . . . . . . . . .  MISC. DETAILS  . . . . . . . . . . . . . . */

        /*  ***  BAD PARAMETERS TO ASSESS  *** */

L330:
        iv[1] = 64;
        goto L350;

        /*  ***  PRINT SUMMARY OF FINAL ITERATION AND OTHER REQUESTED ITEMS  *** */

L340:
        //std::cout<<__LINE__<<" "<<iv[1]<<"\n";
        iv[1] = iv[55];
        //std::cout<<__LINE__<<" "<<iv[1]<<"\n";
        iv[55] = 0;
L350:
        //    ditsum_(&d__[1], &g[1], &iv[1], liv, lv, n, &v[1], &x[1]);

        L999 :
                //std::cout<<__LINE__<<" "<<iv[1]<<"\n";
        return 0;

        /*  ***  LAST LINE OF DRMNG FOLLOWS  *** */
    } /* drmng_ */

    template<typename doublereal>
    int drmnh_(doublereal *d__, doublereal *fx, doublereal *g,
            doublereal *h__, integer *iv, integer *lh, integer *liv, integer *lv,
            integer *n, doublereal *v, doublereal *x) {
        /* System generated locals */
        integer i__1, i__2;

        /* Local variables */
        static integer i__, j, k, l;
        static doublereal t;
        static integer w1, x01, dg1, nn1o2, temp1, step1, dummy;
        //        extern logical stopx_(integer *);
        extern /* Subroutine */ int dd7dup_(doublereal *, doublereal *, integer *,
                integer *, integer *, integer *, doublereal *);
        extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
        extern /* Subroutine */ int da7sst_(integer *, integer *, integer *,
                doublereal *), dv7scp_(integer *, doublereal *, doublereal *);
        extern doublereal dv2nrm_(integer *, doublereal *);
        extern /* Subroutine */ int dg7qts_(doublereal *, doublereal *,
                doublereal *, integer *, doublereal *, integer *, doublereal *,
                doublereal *, doublereal *), ds7lvm_(integer *, doublereal *,
                doublereal *, doublereal *), dv2axy_(integer *, doublereal *,
                doublereal *, doublereal *, doublereal *), dv7cpy_(integer *,
                doublereal *, doublereal *), dparck_(integer *, doublereal *,
                integer *, integer *, integer *, integer *, doublereal *);
        extern doublereal drldst_(integer *, doublereal *, doublereal *,
                doublereal *);
        extern /* Subroutine */ int divset_(integer *, integer *, integer *,
                integer *, doublereal *), ditsum_(doublereal *, doublereal *,
                integer *, integer *, integer *, integer *, doublereal *,
                doublereal *);
        static integer lstgst, rstrst;


        /*  ***  CARRY OUT  DMNH (UNCONSTRAINED MINIMIZATION) ITERATIONS, USING */
        /*  ***  HESSIAN MATRIX PROVIDED BY THE CALLER. */

        /*  ***  PARAMETER DECLARATIONS  *** */


        /* --------------------------  PARAMETER USAGE  -------------------------- */

        /* D.... SCALE VECTOR. */
        /* FX... FUNCTION VALUE. */
        /* G.... GRADIENT VECTOR. */
        /* H.... LOWER TRIANGLE OF THE HESSIAN, STORED ROWWISE. */
        /* IV... INTEGER VALUE ARRAY. */
        /* LH... LENGTH OF H = P*(P+1)/2. */
        /* LIV.. LENGTH OF IV (AT LEAST 60). */
        /* LV... LENGTH OF V (AT LEAST 78 + N*(N+21)/2). */
        /* N.... NUMBER OF VARIABLES (COMPONENTS IN X AND G). */
        /* V.... FLOATING-POINT VALUE ARRAY. */
        /* X.... PARAMETER VECTOR. */

        /*  ***  DISCUSSION  *** */

        /*        PARAMETERS IV, N, V, AND X ARE THE SAME AS THE CORRESPONDING */
        /*     ONES TO  DMNH (WHICH SEE), EXCEPT THAT V CAN BE SHORTER (SINCE */
        /*     THE PART OF V THAT  DMNH USES FOR STORING G AND H IS NOT NEEDED). */
        /*     MOREOVER, COMPARED WITH  DMNH, IV(1) MAY HAVE THE TWO ADDITIONAL */
        /*     OUTPUT VALUES 1 AND 2, WHICH ARE EXPLAINED BELOW, AS IS THE USE */
        /*     OF IV(TOOBIG) AND IV(NFGCAL).  THE VALUE IV(G), WHICH IS AN */
        /*     OUTPUT VALUE FROM  DMNH, IS NOT REFERENCED BY DRMNH OR THE */
        /*     SUBROUTINES IT CALLS. */

        /* IV(1) = 1 MEANS THE CALLER SHOULD SET FX TO F(X), THE FUNCTION VALUE */
        /*             AT X, AND CALL DRMNH AGAIN, HAVING CHANGED NONE OF THE */
        /*             OTHER PARAMETERS.  AN EXCEPTION OCCURS IF F(X) CANNOT BE */
        /*             COMPUTED (E.G. IF OVERFLOW WOULD OCCUR), WHICH MAY HAPPEN */
        /*             BECAUSE OF AN OVERSIZED STEP.  IN THIS CASE THE CALLER */
        /*             SHOULD SET IV(TOOBIG) = IV(2) TO 1, WHICH WILL CAUSE */
        /*             DRMNH TO IGNORE FX AND TRY A SMALLER STEP.  THE PARA- */
        /*             METER NF THAT  DMNH PASSES TO CALCF (FOR POSSIBLE USE BY */
        /*             CALCGH) IS A COPY OF IV(NFCALL) = IV(6). */
        /* IV(1) = 2 MEANS THE CALLER SHOULD SET G TO G(X), THE GRADIENT OF F AT */
        /*             X, AND H TO THE LOWER TRIANGLE OF H(X), THE HESSIAN OF F */
        /*             AT X, AND CALL DRMNH AGAIN, HAVING CHANGED NONE OF THE */
        /*             OTHER PARAMETERS EXCEPT PERHAPS THE SCALE VECTOR D. */
        /*                  THE PARAMETER NF THAT  DMNH PASSES TO CALCG IS */
        /*             IV(NFGCAL) = IV(7).  IF G(X) AND H(X) CANNOT BE EVALUATED, */
        /*             THEN THE CALLER MAY SET IV(TOOBIG) TO 0, IN WHICH CASE */
        /*             DRMNH WILL RETURN WITH IV(1) = 65. */
        /*                  NOTE -- DRMNH OVERWRITES H WITH THE LOWER TRIANGLE */
        /*             OF  DIAG(D)**-1 * H(X) * DIAG(D)**-1. */
        /* . */
        /*  ***  GENERAL  *** */

        /*     CODED BY DAVID M. GAY (WINTER 1980).  REVISED SEPT. 1982. */
        /*     THIS SUBROUTINE WAS WRITTEN IN CONNECTION WITH RESEARCH SUPPORTED */
        /*     IN PART BY THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS */
        /*     MCS-7600324 AND MCS-7906671. */

        /*        (SEE  DMNG AND  DMNH FOR REFERENCES.) */

        /* +++++++++++++++++++++++++++  DECLARATIONS  ++++++++++++++++++++++++++++ */

        /*  ***  LOCAL VARIABLES  *** */


        /*     ***  CONSTANTS  *** */


        /*  ***  NO INTRINSIC FUNCTIONS  *** */

        /*  ***  EXTERNAL FUNCTIONS AND SUBROUTINES  *** */


        /* DA7SST.... ASSESSES CANDIDATE STEP. */
        /* DIVSET.... PROVIDES DEFAULT IV AND V INPUT VALUES. */
        /* DD7TPR... RETURNS INNER PRODUCT OF TWO VECTORS. */
        /* DD7DUP.... UPDATES SCALE VECTOR D. */
        /* DG7QTS.... COMPUTES OPTIMALLY LOCALLY CONSTRAINED STEP. */
        /* DITSUM.... PRINTS ITERATION SUMMARY AND INFO ON INITIAL AND FINAL X. */
        /* DPARCK.... CHECKS VALIDITY OF INPUT IV AND V VALUES. */
        /* DRLDST... COMPUTES V(RELDX) = RELATIVE STEP SIZE. */
        /* DS7LVM... MULTIPLIES SYMMETRIC MATRIX TIMES VECTOR, GIVEN THE LOWER */
        /*             TRIANGLE OF THE MATRIX. */
        /* STOPX.... RETURNS .TRUE. IF THE BREAK KEY HAS BEEN PRESSED. */
        /* DV2AXY.... COMPUTES SCALAR TIMES ONE VECTOR PLUS ANOTHER. */
        /* DV7CPY.... COPIES ONE VECTOR TO ANOTHER. */
        /* DV7SCP... SETS ALL ELEMENTS OF A VECTOR TO A SCALAR. */
        /* DV2NRM... RETURNS THE 2-NORM OF A VECTOR. */

        /*  ***  SUBSCRIPTS FOR IV AND V  *** */


        /*  ***  IV SUBSCRIPT VALUES  *** */

        /* /6 */
        /*     DATA CNVCOD/55/, DG/37/, DTOL/59/, DTYPE/16/, IRC/29/, KAGQT/33/, */
        /*    1     LMAT/42/, MODE/35/, MODEL/5/, MXFCAL/17/, MXITER/18/, */
        /*    2     NEXTV/47/, NFCALL/6/, NFGCAL/7/, NGCALL/30/, NITER/31/, */
        /*    3     RADINC/8/, RESTOR/9/, STEP/40/, STGLIM/11/, STLSTG/41/, */
        /*    4     TOOBIG/2/, VNEED/4/, W/34/, XIRC/13/, X0/43/ */
        /* /7 */
        /* / */

        /*  ***  V SUBSCRIPT VALUES  *** */

        /* /6 */
        /*     DATA DGNORM/1/, DINIT/38/, DSTNRM/2/, DTINIT/39/, D0INIT/40/, */
        /*    1     F/10/, F0/13/, FDIF/11/, GTSTEP/4/, INCFAC/23/, LMAX0/35/, */
        /*    2     LMAXS/36/, PHMXFC/21/, PREDUC/7/, RADFAC/16/, RADIUS/8/, */
        /*    3     RAD0/9/, RELDX/17/, STPPAR/5/, TUNER4/29/, TUNER5/30/ */
        /* /7 */
        /* / */

        /* /6 */
        /*     DATA ONE/1.D+0/, ONEP2/1.2D+0/, ZERO/0.D+0/ */
        /* /7 */
        /* / */

        /* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

        /* Parameter adjustments */
        --h__;
        --iv;
        --v;
        --x;
        --g;
        --d__;

        /* Function Body */
        i__ = iv[1];
        if (i__ == 1) {
            goto L30;
        }
        if (i__ == 2) {
            goto L40;
        }

        /*  ***  CHECK VALIDITY OF IV AND V INPUT VALUES  *** */

        if (iv[1] == 0) {
            divset_(&c__2, &iv[1], liv, lv, &v[1]);
        }
        if (iv[1] == 12 || iv[1] == 13) {
            iv[4] = iv[4] + *n * (*n + 21) / 2 + 7;
        }
        dparck_(&c__2, &d__[1], &iv[1], liv, lv, n, &v[1]);
        i__ = iv[1] - 2;
        if (i__ > 12) {
            goto L999;
        }
        nn1o2 = *n * (*n + 1) / 2;
        if (*lh >= nn1o2) {
            switch (i__) {
                case 1: goto L220;
                case 2: goto L220;
                case 3: goto L220;
                case 4: goto L220;
                case 5: goto L220;
                case 6: goto L220;
                case 7: goto L160;
                case 8: goto L120;
                case 9: goto L160;
                case 10: goto L10;
                case 11: goto L10;
                case 12: goto L20;
            }
        }
        iv[1] = 66;
        goto L400;

        /*  ***  STORAGE ALLOCATION  *** */

L10:
        iv[59] = iv[42] + nn1o2;
        iv[43] = iv[59] + (*n << 1);
        iv[40] = iv[43] + *n;
        iv[41] = iv[40] + *n;
        iv[37] = iv[41] + *n;
        iv[34] = iv[37] + *n;
        iv[47] = iv[34] + (*n << 2) + 7;
        if (iv[1] != 13) {
            goto L20;
        }
        iv[1] = 14;
        goto L999;

        /*  ***  INITIALIZATION  *** */

L20:
        iv[31] = 0;
        iv[6] = 1;
        iv[30] = 1;
        iv[7] = 1;
        iv[35] = -1;
        iv[5] = 1;
        iv[11] = 1;
        iv[2] = 0;
        iv[55] = 0;
        iv[8] = 0;
        v[9] = 0.;
        v[5] = 0.;
        if (v[38] >= 0.) {
            dv7scp_(n, &d__[1], &v[38]);
        }
        k = iv[59];
        if (v[39] > 0.) {
            dv7scp_(n, &v[k], &v[39]);
        }
        k += *n;
        if (v[40] > 0.) {
            dv7scp_(n, &v[k], &v[40]);
        }
        iv[1] = 1;
        goto L999;

L30:
        v[10] = *fx;
        if (iv[35] >= 0) {
            goto L220;
        }
        v[13] = *fx;
        iv[1] = 2;
        if (iv[2] == 0) {
            goto L999;
        }
        iv[1] = 63;
        goto L400;

        /*  ***  MAKE SURE GRADIENT COULD BE COMPUTED  *** */

L40:
        if (iv[2] == 0) {
            goto L50;
        }
        iv[1] = 65;
        goto L400;

        /*  ***  UPDATE THE SCALE VECTOR D  *** */

L50:
        dg1 = iv[37];
        if (iv[16] <= 0) {
            goto L70;
        }
        k = dg1;
        j = 0;
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            j += i__;
            v[k] = h__[j];
            ++k;
            /* L60: */
        }
        dd7dup_(&d__[1], &v[dg1], &iv[1], liv, lv, n, &v[1]);

        /*  ***  COMPUTE SCALED GRADIENT AND ITS NORM  *** */

L70:
        dg1 = iv[37];
        k = dg1;
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            v[k] = g[i__] / d__[i__];
            ++k;
            /* L80: */
        }
        v[1] = dv2nrm_(n, &v[dg1]);

        /*  ***  COMPUTE SCALED HESSIAN  *** */

        k = 1;
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            t = 1. / d__[i__];
            i__2 = i__;
            for (j = 1; j <= i__2; ++j) {
                h__[k] = t * h__[k] / d__[j];
                ++k;
                /* L90: */
            }
            /* L100: */
        }

        if (iv[55] != 0) {
            goto L390;
        }
        if (iv[35] == 0) {
            goto L350;
        }

        /*  ***  ALLOW FIRST STEP TO HAVE SCALED 2-NORM AT MOST V(LMAX0)  *** */

        v[8] = v[35] / (v[21] + 1.);

        iv[35] = 0;


        /* -----------------------------  MAIN LOOP  ----------------------------- */


        /*  ***  PRINT ITERATION SUMMARY, CHECK ITERATION LIMIT  *** */

L110:
        ditsum_(&d__[1], &g[1], &iv[1], liv, lv, n, &v[1], &x[1]);
L120:
        k = iv[31];
        if (k < iv[18]) {
            goto L130;
        }
        iv[1] = 10;
        goto L400;

L130:
        iv[31] = k + 1;

        /*  ***  INITIALIZE FOR START OF NEXT ITERATION  *** */

        dg1 = iv[37];
        x01 = iv[43];
        v[13] = v[10];
        iv[29] = 4;
        iv[33] = -1;

        /*     ***  COPY X TO X0  *** */

        dv7cpy_(n, &v[x01], &x[1]);

        /*  ***  UPDATE RADIUS  *** */

        if (k == 0) {
            goto L150;
        }
        step1 = iv[40];
        k = step1;
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            v[k] = d__[i__] * v[k];
            ++k;
            /* L140: */
        }
        v[8] = v[16] * dv2nrm_(n, &v[step1]);

        /*  ***  CHECK STOPX AND FUNCTION EVALUATION LIMIT  *** */

L150:
        if (!stopx_(&dummy)) {
            goto L170;
        }
        iv[1] = 11;
        goto L180;

        /*     ***  COME HERE WHEN RESTARTING AFTER FUNC. EVAL. LIMIT OR STOPX. */

L160:
        if (v[10] >= v[13]) {
            goto L170;
        }
        v[16] = 1.;
        k = iv[31];
        goto L130;

L170:
        if (iv[6] < iv[17]) {
            goto L190;
        }
        iv[1] = 9;
L180:
        if (v[10] >= v[13]) {
            goto L400;
        }

        /*        ***  IN CASE OF STOPX OR FUNCTION EVALUATION LIMIT WITH */
        /*        ***  IMPROVED V(F), EVALUATE THE GRADIENT AT X. */

        iv[55] = iv[1];
        goto L340;

        /* . . . . . . . . . . . . .  COMPUTE CANDIDATE STEP  . . . . . . . . . . */

L190:
        step1 = iv[40];
        dg1 = iv[37];
        l = iv[42];
        w1 = iv[34];
        dg7qts_(&d__[1], &v[dg1], &h__[1], &iv[33], &v[l], n, &v[step1], &v[1], &
                v[w1]);
        if (iv[29] != 6) {
            goto L200;
        }
        if (iv[9] != 2) {
            goto L220;
        }
        rstrst = 2;
        goto L230;

        /*  ***  CHECK WHETHER EVALUATING F(X0 + STEP) LOOKS WORTHWHILE  *** */

L200:
        iv[2] = 0;
        if (v[2] <= 0.) {
            goto L220;
        }
        if (iv[29] != 5) {
            goto L210;
        }
        if (v[16] <= 1.) {
            goto L210;
        }
        if (v[7] > v[11] * 1.2) {
            goto L210;
        }
        if (iv[9] != 2) {
            goto L220;
        }
        rstrst = 0;
        goto L230;

        /*  ***  COMPUTE F(X0 + STEP)  *** */

L210:
        x01 = iv[43];
        step1 = iv[40];
        dv2axy_(n, &x[1], &c_b32, &v[step1], &v[x01]);
        ++iv[6];
        iv[1] = 1;
        goto L999;

        /* . . . . . . . . . . . . .  ASSESS CANDIDATE STEP  . . . . . . . . . . . */

L220:
        rstrst = 3;
L230:
        x01 = iv[43];
        v[17] = drldst_(n, &d__[1], &x[1], &v[x01]);
        da7sst_(&iv[1], liv, lv, &v[1]);
        step1 = iv[40];
        lstgst = iv[41];
        i__ = iv[9] + 1;
        switch (i__) {
            case 1: goto L270;
            case 2: goto L240;
            case 3: goto L250;
            case 4: goto L260;
        }
L240:
        dv7cpy_(n, &x[1], &v[x01]);
        goto L270;
L250:
        dv7cpy_(n, &v[lstgst], &v[step1]);
        goto L270;
L260:
        dv7cpy_(n, &v[step1], &v[lstgst]);
        dv2axy_(n, &x[1], &c_b32, &v[step1], &v[x01]);
        v[17] = drldst_(n, &d__[1], &x[1], &v[x01]);
        iv[9] = rstrst;

L270:
        k = iv[29];
        switch (k) {
            case 1: goto L280;
            case 2: goto L310;
            case 3: goto L310;
            case 4: goto L310;
            case 5: goto L280;
            case 6: goto L290;
            case 7: goto L300;
            case 8: goto L300;
            case 9: goto L300;
            case 10: goto L300;
            case 11: goto L300;
            case 12: goto L300;
            case 13: goto L380;
            case 14: goto L350;
        }

        /*     ***  RECOMPUTE STEP WITH NEW RADIUS  *** */

L280:
        v[8] = v[16] * v[2];
        goto L150;

        /*  ***  COMPUTE STEP OF LENGTH V(LMAXS) FOR SINGULAR CONVERGENCE TEST. */

L290:
        v[8] = v[36];
        goto L190;

        /*  ***  CONVERGENCE OR FALSE CONVERGENCE  *** */

L300:
        iv[55] = k - 4;
        if (v[10] >= v[13]) {
            goto L390;
        }
        if (iv[13] == 14) {
            goto L390;
        }
        iv[13] = 14;

        /* . . . . . . . . . . . .  PROCESS ACCEPTABLE STEP  . . . . . . . . . . . */

L310:
        if (iv[29] != 3) {
            goto L340;
        }
        temp1 = lstgst;

        /*     ***  PREPARE FOR GRADIENT TESTS  *** */
        /*     ***  SET  TEMP1 = HESSIAN * STEP + G(X0) */
        /*     ***             = DIAG(D) * (H * STEP + G(X0)) */

        /*        USE X0 VECTOR AS TEMPORARY. */
        k = x01;
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            v[k] = d__[i__] * v[step1];
            ++k;
            ++step1;
            /* L320: */
        }
        ds7lvm_(n, &v[temp1], &h__[1], &v[x01]);
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            v[temp1] = d__[i__] * v[temp1] + g[i__];
            ++temp1;
            /* L330: */
        }

        /*  ***  COMPUTE GRADIENT AND HESSIAN  *** */

L340:
        ++iv[30];
        iv[2] = 0;
        iv[1] = 2;
        goto L999;

L350:
        iv[1] = 2;
        if (iv[29] != 3) {
            goto L110;
        }

        /*  ***  SET V(RADFAC) BY GRADIENT TESTS  *** */

        temp1 = iv[41];
        step1 = iv[40];

        /*     ***  SET  TEMP1 = DIAG(D)**-1 * (HESSIAN*STEP + (G(X0)-G(X)))  *** */

        k = temp1;
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            v[k] = (v[k] - g[i__]) / d__[i__];
            ++k;
            /* L360: */
        }

        /*     ***  DO GRADIENT TESTS  *** */

        if (dv2nrm_(n, &v[temp1]) <= v[1] * v[29]) {
            goto L370;
        }
        if (dd7tpr_(n, &g[1], &v[step1]) >= v[4] * v[30]) {
            goto L110;
        }
L370:
        v[16] = v[23];
        goto L110;

        /* . . . . . . . . . . . . . .  MISC. DETAILS  . . . . . . . . . . . . . . */

        /*  ***  BAD PARAMETERS TO ASSESS  *** */

L380:
        iv[1] = 64;
        goto L400;

        /*  ***  PRINT SUMMARY OF FINAL ITERATION AND OTHER REQUESTED ITEMS  *** */

L390:
        iv[1] = iv[55];
        iv[55] = 0;
L400:
        ditsum_(&d__[1], &g[1], &iv[1], liv, lv, n, &v[1], &x[1]);

L999:
        return 0;

        /*  ***  LAST CARD OF DRMNH FOLLOWS  *** */
    }

    template<typename doublereal>
    /* Subroutine */ int drmngb_(doublereal *b, doublereal *d__, doublereal *fx,
            doublereal *g, integer *iv, integer *liv, integer *lv, integer *n,
            doublereal *v, doublereal *x) {
        /* System generated locals */
        integer i__1;

        /* Local variables */
        static integer i__, j, k, l;
        static doublereal t;
        static integer z__, i1, n1, w1, g01;
        static doublereal gi;
        static integer x01;
        static doublereal xi;
        static integer dg1, td1, tg1, np1, ipi, ipn, temp0, temp1, step1, dummy;
        //        extern /* Subroutine */ int dd7dgb_(doublereal *, doublereal *,
        //                doublereal *, doublereal *, doublereal *, integer *, integer *,
        //                doublereal *, integer *, integer *, integer *, doublereal *,
        //                doublereal *, doublereal *, doublereal *, doublereal *,
        //                doublereal *, doublereal *);
        //        //        extern logical stopx_(integer *);
        //        extern /* Subroutine */ int dl7upd_(doublereal *, doublereal *,
        //                doublereal *, doublereal *, doublereal *, integer *, doublereal *,
        //                doublereal *);
        static integer dstep1;
        //        extern /* Subroutine */ int dw7zbf_(doublereal *, integer *, doublereal *,
        //                doublereal *, doublereal *, doublereal *);
        //        extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
        //        extern /* Subroutine */ int da7sst_(integer *, integer *, integer *,
        //                doublereal *), i7shft_(integer *, integer *, integer *), dl7vml_(
        //                integer *, doublereal *, doublereal *, doublereal *);
        //        extern doublereal dv2nrm_(integer *, doublereal *);
        //        extern /* Subroutine */ int dq7rsh_(integer *, integer *, logical *,
        //                doublereal *, doublereal *, doublereal *), dv7scp_(integer *,
        //                doublereal *, doublereal *), dv7ipr_(integer *, integer *,
        //                doublereal *), dv7cpy_(integer *, doublereal *, doublereal *),
        //                dl7tvm_(integer *, doublereal *, doublereal *, doublereal *),
        //                dv2axy_(integer *, doublereal *, doublereal *, doublereal *,
        //                doublereal *), dv7vmp_(integer *, doublereal *, doublereal *,
        //                doublereal *, integer *);
        static integer nwtst1;
        //        extern /* Subroutine */ int dparck_(integer *, doublereal *, integer *,
        //                integer *, integer *, integer *, doublereal *);
        //        extern doublereal drldst_(integer *, doublereal *, doublereal *,
        //                doublereal *);
        //        extern /* Subroutine */ int divset_(integer *, integer *, integer *,
        //                integer *, doublereal *), ditsum_(doublereal *, doublereal *,
        //                integer *, integer *, integer *, integer *, doublereal *,
        //                doublereal *);
        static integer lstgst, rstrst;


        /*  ***  CARRY OUT  DMNGB (SIMPLY BOUNDED MINIMIZATION) ITERATIONS, */
        /*  ***  USING DOUBLE-DOGLEG/BFGS STEPS. */

        /*  ***  PARAMETER DECLARATIONS  *** */


        /* --------------------------  PARAMETER USAGE  -------------------------- */

        /* B.... VECTOR OF LOWER AND UPPER BOUNDS ON X. */
        /* D.... SCALE VECTOR. */
        /* FX... FUNCTION VALUE. */
        /* G.... GRADIENT VECTOR. */
        /* IV... INTEGER VALUE ARRAY. */
        /* LIV.. LENGTH OF IV (AT LEAST 59) + N. */
        /* LV... LENGTH OF V (AT LEAST 71 + N*(N+19)/2). */
        /* N.... NUMBER OF VARIABLES (COMPONENTS IN X AND G). */
        /* V.... FLOATING-POINT VALUE ARRAY. */
        /* X.... VECTOR OF PARAMETERS TO BE OPTIMIZED. */

        /*  ***  DISCUSSION  *** */

        /*        PARAMETERS IV, N, V, AND X ARE THE SAME AS THE CORRESPONDING */
        /*     ONES TO  DMNGB (WHICH SEE), EXCEPT THAT V CAN BE SHORTER (SINCE */
        /*     THE PART OF V THAT  DMNGB USES FOR STORING G IS NOT NEEDED). */
        /*     MOREOVER, COMPARED WITH  DMNGB, IV(1) MAY HAVE THE TWO ADDITIONAL */
        /*     OUTPUT VALUES 1 AND 2, WHICH ARE EXPLAINED BELOW, AS IS THE USE */
        /*     OF IV(TOOBIG) AND IV(NFGCAL).  THE VALUE IV(G), WHICH IS AN */
        /*     OUTPUT VALUE FROM  DMNGB (AND SMSNOB), IS NOT REFERENCED BY */
        /*     DRMNGB OR THE SUBROUTINES IT CALLS. */
        /*        FX AND G NEED NOT HAVE BEEN INITIALIZED WHEN DRMNGB IS CALLED */
        /*     WITH IV(1) = 12, 13, OR 14. */

        /* IV(1) = 1 MEANS THE CALLER SHOULD SET FX TO F(X), THE FUNCTION VALUE */
        /*             AT X, AND CALL DRMNGB AGAIN, HAVING CHANGED NONE OF THE */
        /*             OTHER PARAMETERS.  AN EXCEPTION OCCURS IF F(X) CANNOT BE */
        /*             (E.G. IF OVERFLOW WOULD OCCUR), WHICH MAY HAPPEN BECAUSE */
        /*             OF AN OVERSIZED STEP.  IN THIS CASE THE CALLER SHOULD SET */
        /*             IV(TOOBIG) = IV(2) TO 1, WHICH WILL CAUSE DRMNGB TO IG- */
        /*             NORE FX AND TRY A SMALLER STEP.  THE PARAMETER NF THAT */
        /*              DMNGB PASSES TO CALCF (FOR POSSIBLE USE BY CALCG) IS A */
        /*             COPY OF IV(NFCALL) = IV(6). */
        /* IV(1) = 2 MEANS THE CALLER SHOULD SET G TO G(X), THE GRADIENT VECTOR */
        /*             OF F AT X, AND CALL DRMNGB AGAIN, HAVING CHANGED NONE OF */
        /*             THE OTHER PARAMETERS EXCEPT POSSIBLY THE SCALE VECTOR D */
        /*             WHEN IV(DTYPE) = 0.  THE PARAMETER NF THAT  DMNGB PASSES */
        /*             TO CALCG IS IV(NFGCAL) = IV(7).  IF G(X) CANNOT BE */
        /*             EVALUATED, THEN THE CALLER MAY SET IV(NFGCAL) TO 0, IN */
        /*             WHICH CASE DRMNGB WILL RETURN WITH IV(1) = 65. */
        /* . */
        /*  ***  GENERAL  *** */

        /*     CODED BY DAVID M. GAY (DECEMBER 1979).  REVISED SEPT. 1982. */
        /*     THIS SUBROUTINE WAS WRITTEN IN CONNECTION WITH RESEARCH SUPPORTED */
        /*     IN PART BY THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS */
        /*     MCS-7600324 AND MCS-7906671. */

        /*        (SEE  DMNG FOR REFERENCES.) */

        /* +++++++++++++++++++++++++++  DECLARATIONS  ++++++++++++++++++++++++++++ */

        /*  ***  LOCAL VARIABLES  *** */


        /*     ***  CONSTANTS  *** */


        /*  ***  NO INTRINSIC FUNCTIONS  *** */

        /*  ***  EXTERNAL FUNCTIONS AND SUBROUTINES  *** */


        /* DA7SST.... ASSESSES CANDIDATE STEP. */
        /* DD7DGB... COMPUTES SIMPLY BOUNDED DOUBLE-DOGLEG (CANDIDATE) STEP. */
        /* DIVSET.... SUPPLIES DEFAULT IV AND V INPUT COMPONENTS. */
        /* DD7TPR... RETURNS INNER PRODUCT OF TWO VECTORS. */
        /* I7SHFT... CYCLICALLLY SHIFTS AN ARRAY OF INTEGERS. */
        /* DITSUM.... PRINTS ITERATION SUMMARY AND INFO ON INITIAL AND FINAL X. */
        /* DL7TVM... MULTIPLIES TRANSPOSE OF LOWER TRIANGLE TIMES VECTOR. */
        /* LUPDT.... UPDATES CHOLESKY FACTOR OF HESSIAN APPROXIMATION. */
        /* DL7VML.... MULTIPLIES LOWER TRIANGLE TIMES VECTOR. */
        /* DPARCK.... CHECKS VALIDITY OF INPUT IV AND V VALUES. */
        /* DQ7RSH... CYCLICALLY SHIFTS CHOLESKY FACTOR. */
        /* DRLDST... COMPUTES V(RELDX) = RELATIVE STEP SIZE. */
        /* STOPX.... RETURNS .TRUE. IF THE BREAK KEY HAS BEEN PRESSED. */
        /* DV2NRM... RETURNS THE 2-NORM OF A VECTOR. */
        /* DV2AXY.... COMPUTES SCALAR TIMES ONE VECTOR PLUS ANOTHER. */
        /* DV7CPY.... COPIES ONE VECTOR TO ANOTHER. */
        /* DV7IPR... CYCLICALLY SHIFTS A FLOATING-POINT ARRAY. */
        /* DV7SCP... SETS ALL ELEMENTS OF A VECTOR TO A SCALAR. */
        /* DV7VMP... MULTIPLIES VECTOR BY VECTOR RAISED TO POWER (COMPONENTWISE). */
        /* DW7ZBF... COMPUTES W AND Z FOR DL7UPD CORRESPONDING TO BFGS UPDATE. */

        /*  ***  SUBSCRIPTS FOR IV AND V  *** */


        /*  ***  IV SUBSCRIPT VALUES  *** */

        /*  ***  (NOTE THAT NC IS STORED IN IV(G0)) *** */

        /* /6 */
        /*     DATA CNVCOD/55/, DG/37/, INITH/25/, IRC/29/, IVNEED/3/, KAGQT/33/, */
        /*    1     MODE/35/, MODEL/5/, MXFCAL/17/, MXITER/18/, NC/48/, */
        /*    2     NEXTIV/46/, NEXTV/47/, NFCALL/6/, NFGCAL/7/, NGCALL/30/, */
        /*    3     NITER/31/, NWTSTP/34/, PERM/58/, RADINC/8/, RESTOR/9/, */
        /*    4     STEP/40/, STGLIM/11/, STLSTG/41/, TOOBIG/2/, XIRC/13/, X0/43/ */
        /* /7 */
        /* / */

        /*  ***  V SUBSCRIPT VALUES  *** */

        /* /6 */
        /*     DATA DGNORM/1/, DINIT/38/, DSTNRM/2/, F/10/, F0/13/, FDIF/11/, */
        /*    1     GTSTEP/4/, INCFAC/23/, LMAT/42/, LMAX0/35/, LMAXS/36/, */
        /*    2     PREDUC/7/, RADFAC/16/, RADIUS/8/, RAD0/9/, RELDX/17/, */
        /*    3     TUNER4/29/, TUNER5/30/, VNEED/4/ */
        /* /7 */
        /* / */

        /* /6 */
        /*     DATA NEGONE/-1.D+0/, ONE/1.D+0/, ONEP2/1.2D+0/, ZERO/0.D+0/ */
        /* /7 */
        /* / */

        /* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

        /* Parameter adjustments */
        --iv;
        --v;
        --x;
        --g;
        --d__;
        b -= 3;

        /* Function Body */
        i__ = iv[1];
        if (i__ == 1) {
            goto L70;
        }
        if (i__ == 2) {
            goto L80;
        }

        /*  ***  CHECK VALIDITY OF IV AND V INPUT VALUES  *** */

        if (iv[1] == 0) {
            divset_(&c__2, &iv[1], liv, lv, &v[1]);
        }
        if (iv[1] < 12) {
            goto L10;
        }
        if (iv[1] > 13) {
            goto L10;
        }
        iv[4] += *n * (*n + 19) / 2;
        iv[3] += *n;
L10:
        dparck_(&c__2, &d__[1], &iv[1], liv, lv, n, &v[1]);
        i__ = iv[1] - 2;
        if (i__ > 12) {
            goto L999;
        }
        switch (i__) {
            case 1: goto L250;
            case 2: goto L250;
            case 3: goto L250;
            case 4: goto L250;
            case 5: goto L250;
            case 6: goto L250;
            case 7: goto L190;
            case 8: goto L150;
            case 9: goto L190;
            case 10: goto L20;
            case 11: goto L20;
            case 12: goto L30;
        }

        /*  ***  STORAGE ALLOCATION  *** */

L20:
        l = iv[42];
        iv[43] = l + *n * (*n + 1) / 2;
        iv[40] = iv[43] + (*n << 1);
        iv[41] = iv[40] + (*n << 1);
        iv[34] = iv[41] + *n;
        iv[37] = iv[34] + (*n << 1);
        iv[47] = iv[37] + (*n << 1);
        iv[46] = iv[58] + *n;
        if (iv[1] != 13) {
            goto L30;
        }
        iv[1] = 14;
        goto L999;

        /*  ***  INITIALIZATION  *** */

L30:
        iv[31] = 0;
        iv[6] = 1;
        iv[30] = 1;
        iv[7] = 1;
        iv[35] = -1;
        iv[5] = 1;
        iv[11] = 1;
        iv[2] = 0;
        iv[55] = 0;
        iv[8] = 0;
        iv[48] = *n;
        v[9] = 0.;

        /*  ***  CHECK CONSISTENCY OF B AND INITIALIZE IP ARRAY  *** */

        ipi = iv[58];
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            iv[ipi] = i__;
            ++ipi;
            if (b[(i__ << 1) + 1] > b[(i__ << 1) + 2]) {
                goto L410;
            }
            /* L40: */
        }

        if (v[38] >= 0.) {
            dv7scp_(n, &d__[1], &v[38]);
        }
        if (iv[25] != 1) {
            goto L60;
        }

        /*     ***  SET THE INITIAL HESSIAN APPROXIMATION TO DIAG(D)**-2  *** */

        l = iv[42];
        i__1 = *n * (*n + 1) / 2;
        dv7scp_(&i__1, &v[l], &c_b16);
        k = l - 1;
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            k += i__;
            t = d__[i__];
            if (t <= 0.) {
                t = 1.;
            }
            v[k] = t;
            /* L50: */
        }

        /*  ***  GET INITIAL FUNCTION VALUE  *** */

L60:
        iv[1] = 1;
        goto L440;

L70:
        v[10] = *fx;
        if (iv[35] >= 0) {
            goto L250;
        }
        v[13] = *fx;
        iv[1] = 2;
        if (iv[2] == 0) {
            goto L999;
        }
        iv[1] = 63;
        goto L430;

        /*  ***  MAKE SURE GRADIENT COULD BE COMPUTED  *** */

L80:
        if (iv[2] == 0) {
            goto L90;
        }
        iv[1] = 65;
        goto L430;

        /*  ***  CHOOSE INITIAL PERMUTATION  *** */

L90:
        ipi = iv[58];
        ipn = ipi + *n;
        n1 = *n;
        np1 = *n + 1;
        l = iv[42];
        w1 = iv[34] + *n;
        k = *n - iv[48];
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            --ipn;
            j = iv[ipn];
            if (b[(j << 1) + 1] >= b[(j << 1) + 2]) {
                goto L100;
            }
            xi = x[j];
            gi = g[j];
            if (xi <= b[(j << 1) + 1] && gi > 0.) {
                goto L100;
            }
            if (xi >= b[(j << 1) + 2] && gi < 0.) {
                goto L100;
            }
            /*           *** DISALLOW CONVERGENCE IF X(J) HAS JUST BEEN FREED *** */
            if (i__ <= k) {
                iv[55] = 0;
            }
            goto L120;
L100:
            i1 = np1 - i__;
            if (i1 >= n1) {
                goto L110;
            }
            i7shft_(&n1, &i1, &iv[ipi]);
            dq7rsh_(&i1, &n1, &c_false, &g[1], &v[l], &v[w1]);
L110:
            --n1;
L120:
            ;
        }

        iv[48] = n1;
        v[1] = 0.;
        if (n1 <= 0) {
            goto L130;
        }
        dg1 = iv[37];
        dv7vmp_(n, &v[dg1], &g[1], &d__[1], &c_n1);
        dv7ipr_(n, &iv[ipi], &v[dg1]);
        v[1] = dv2nrm_(&n1, &v[dg1]);
L130:
        if (iv[55] != 0) {
            goto L420;
        }
        if (iv[35] == 0) {
            goto L370;
        }

        /*  ***  ALLOW FIRST STEP TO HAVE SCALED 2-NORM AT MOST V(LMAX0)  *** */

        v[8] = v[35];

        iv[35] = 0;


        /* -----------------------------  MAIN LOOP  ----------------------------- */


        /*  ***  PRINT ITERATION SUMMARY, CHECK ITERATION LIMIT  *** */

L140:
        //        ditsum_(&d__[1], &g[1], &iv[1], liv, lv, n, &v[1], &x[1]);
        L150 :
                k = iv[31];
        if (k < iv[18]) {
            goto L160;
        }
        iv[1] = 10;
        goto L430;

        /*  ***  UPDATE RADIUS  *** */

L160:
        iv[31] = k + 1;
        if (k == 0) {
            goto L170;
        }
        t = v[16] * v[2];
        if (v[16] < 1. || t > v[8]) {
            v[8] = t;
        }

        /*  ***  INITIALIZE FOR START OF NEXT ITERATION  *** */

L170:
        x01 = iv[43];
        v[13] = v[10];
        iv[29] = 4;
        iv[33] = -1;

        /*     ***  COPY X TO X0  *** */

        dv7cpy_(n, &v[x01], &x[1]);

        /*  ***  CHECK STOPX AND FUNCTION EVALUATION LIMIT  *** */

L180:
        if (!stopx_(&dummy)) {
            goto L200;
        }
        iv[1] = 11;
        goto L210;

        /*     ***  COME HERE WHEN RESTARTING AFTER FUNC. EVAL. LIMIT OR STOPX. */

L190:
        if (v[10] >= v[13]) {
            goto L200;
        }
        v[16] = 1.;
        k = iv[31];
        goto L160;

L200:
        if (iv[6] < iv[17]) {
            goto L220;
        }
        iv[1] = 9;
L210:
        if (v[10] >= v[13]) {
            goto L430;
        }

        /*        ***  IN CASE OF STOPX OR FUNCTION EVALUATION LIMIT WITH */
        /*        ***  IMPROVED V(F), EVALUATE THE GRADIENT AT X. */

        iv[55] = iv[1];
        goto L360;

        /* . . . . . . . . . . . . .  COMPUTE CANDIDATE STEP  . . . . . . . . . . */

L220:
        step1 = iv[40];
        dg1 = iv[37];
        nwtst1 = iv[34];
        w1 = nwtst1 + *n;
        dstep1 = step1 + *n;
        ipi = iv[58];
        l = iv[42];
        tg1 = dg1 + *n;
        x01 = iv[43];
        td1 = x01 + *n;
        dd7dgb_(&b[3], &d__[1], &v[dg1], &v[dstep1], &g[1], &iv[ipi], &iv[33], &v[
                l], lv, n, &iv[48], &v[nwtst1], &v[step1], &v[td1], &v[tg1], &v[1]
                , &v[w1], &v[x01]);
        if (iv[29] != 6) {
            goto L230;
        }
        if (iv[9] != 2) {
            goto L250;
        }
        rstrst = 2;
        goto L260;

        /*  ***  CHECK WHETHER EVALUATING F(X0 + STEP) LOOKS WORTHWHILE  *** */

L230:
        iv[2] = 0;
        if (v[2] <= 0.) {
            goto L250;
        }
        if (iv[29] != 5) {
            goto L240;
        }
        if (v[16] <= 1.) {
            goto L240;
        }
        if (v[7] > v[11] * 1.2) {
            goto L240;
        }
        if (iv[9] != 2) {
            goto L250;
        }
        rstrst = 0;
        goto L260;

        /*  ***  COMPUTE F(X0 + STEP)  *** */

L240:
        dv2axy_(n, &x[1], &c_b40, &v[step1], &v[x01]);
        ++iv[6];
        iv[1] = 1;
        goto L440;

        /* . . . . . . . . . . . . .  ASSESS CANDIDATE STEP  . . . . . . . . . . . */

L250:
        rstrst = 3;
L260:
        x01 = iv[43];
        v[17] = drldst_(n, &d__[1], &x[1], &v[x01]);
        da7sst_(&iv[1], liv, lv, &v[1]);
        step1 = iv[40];
        lstgst = iv[41];
        i__ = iv[9] + 1;
        switch (i__) {
            case 1: goto L300;
            case 2: goto L270;
            case 3: goto L280;
            case 4: goto L290;
        }
L270:
        dv7cpy_(n, &x[1], &v[x01]);
        goto L300;
L280:
        dv7cpy_(n, &v[lstgst], &x[1]);
        goto L300;
L290:
        dv7cpy_(n, &x[1], &v[lstgst]);
        dv2axy_(n, &v[step1], &c_b45, &v[x01], &x[1]);
        v[17] = drldst_(n, &d__[1], &x[1], &v[x01]);
        iv[9] = rstrst;

L300:
        k = iv[29];
        switch (k) {
            case 1: goto L310;
            case 2: goto L340;
            case 3: goto L340;
            case 4: goto L340;
            case 5: goto L310;
            case 6: goto L320;
            case 7: goto L330;
            case 8: goto L330;
            case 9: goto L330;
            case 10: goto L330;
            case 11: goto L330;
            case 12: goto L330;
            case 13: goto L400;
            case 14: goto L370;
        }

        /*     ***  RECOMPUTE STEP WITH CHANGED RADIUS  *** */

L310:
        v[8] = v[16] * v[2];
        goto L180;

        /*  ***  COMPUTE STEP OF LENGTH V(LMAXS) FOR SINGULAR CONVERGENCE TEST. */

L320:
        v[8] = v[36];
        goto L220;

        /*  ***  CONVERGENCE OR FALSE CONVERGENCE  *** */

L330:
        iv[55] = k - 4;
        if (v[10] >= v[13]) {
            goto L420;
        }
        if (iv[13] == 14) {
            goto L420;
        }
        iv[13] = 14;

        /* . . . . . . . . . . . .  PROCESS ACCEPTABLE STEP  . . . . . . . . . . . */

L340:
        x01 = iv[43];
        step1 = iv[40];
        dv2axy_(n, &v[step1], &c_b45, &v[x01], &x[1]);
        if (iv[29] != 3) {
            goto L360;
        }

        /*     ***  SET  TEMP1 = HESSIAN * STEP  FOR USE IN GRADIENT TESTS  *** */

        /*     ***  USE X0 AS TEMPORARY... */

        ipi = iv[58];
        dv7cpy_(n, &v[x01], &v[step1]);
        dv7ipr_(n, &iv[ipi], &v[x01]);
        l = iv[42];
        dl7tvm_(n, &v[x01], &v[l], &v[x01]);
        dl7vml_(n, &v[x01], &v[l], &v[x01]);

        /*        *** UNPERMUTE X0 INTO TEMP1 *** */

        temp1 = iv[41];
        temp0 = temp1 - 1;
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            j = iv[ipi];
            ++ipi;
            k = temp0 + j;
            v[k] = v[x01];
            ++x01;
            /* L350: */
        }

        /*  ***  SAVE OLD GRADIENT, COMPUTE NEW ONE  *** */

L360:
        g01 = iv[34] + *n;
        dv7cpy_(n, &v[g01], &g[1]);
        ++iv[30];
        iv[2] = 0;
        iv[1] = 2;
        goto L999;

        /*  ***  INITIALIZATIONS -- G0 = G - G0, ETC.  *** */

L370:
        g01 = iv[34] + *n;
        dv2axy_(n, &v[g01], &c_b45, &v[g01], &g[1]);
        step1 = iv[40];
        temp1 = iv[41];
        if (iv[29] != 3) {
            goto L390;
        }

        /*  ***  SET V(RADFAC) BY GRADIENT TESTS  *** */

        /*     ***  SET  TEMP1 = DIAG(D)**-1 * (HESSIAN*STEP + (G(X0)-G(X)))  *** */

        dv2axy_(n, &v[temp1], &c_b45, &v[g01], &v[temp1]);
        dv7vmp_(n, &v[temp1], &v[temp1], &d__[1], &c_n1);

        /*        ***  DO GRADIENT TESTS  *** */

        if (dv2nrm_(n, &v[temp1]) <= v[1] * v[29]) {
            goto L380;
        }
        if (dd7tpr_(n, &g[1], &v[step1]) >= v[4] * v[30]) {
            goto L390;
        }
L380:
        v[16] = v[23];

        /*  ***  UPDATE H, LOOP  *** */

L390:
        w1 = iv[34];
        z__ = iv[43];
        l = iv[42];
        ipi = iv[58];
        dv7ipr_(n, &iv[ipi], &v[step1]);
        dv7ipr_(n, &iv[ipi], &v[g01]);
        dw7zbf_(&v[l], n, &v[step1], &v[w1], &v[g01], &v[z__]);

        /*     ** USE THE N-VECTORS STARTING AT V(STEP1) AND V(G01) FOR SCRATCH.. */
        dl7upd_(&v[temp1], &v[step1], &v[l], &v[g01], &v[l], n, &v[w1], &v[z__]);
        iv[1] = 2;
        goto L140;

        /* . . . . . . . . . . . . . .  MISC. DETAILS  . . . . . . . . . . . . . . */

        /*  ***  BAD PARAMETERS TO ASSESS  *** */

L400:
        iv[1] = 64;
        goto L430;

        /*  ***  INCONSISTENT B  *** */

L410:
        iv[1] = 82;
        goto L430;

        /*  ***  PRINT SUMMARY OF FINAL ITERATION AND OTHER REQUESTED ITEMS  *** */

L420:
        iv[1] = iv[55];
        iv[55] = 0;
L430:
        //        ditsum_(&d__[1], &g[1], &iv[1], liv, lv, n, &v[1], &x[1]);
        goto L999;

        /*  ***  PROJECT X INTO FEASIBLE REGION (PRIOR TO COMPUTING F OR G)  *** */

L440:
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            if (x[i__] < b[(i__ << 1) + 1]) {
                x[i__] = b[(i__ << 1) + 1];
            }
            if (x[i__] > b[(i__ << 1) + 2]) {
                x[i__] = b[(i__ << 1) + 2];
            }
            /* L450: */
        }

L999:
        return 0;

        /*  ***  LAST CARD OF DRMNGB FOLLOWS  *** */
    } /* drmngb_ */

    template<typename doublereal>
    /* Subroutine */ int drmnhb_(doublereal *b, doublereal *d__, doublereal *fx,
            doublereal *g, doublereal *h__, integer *iv, integer *lh, integer *
            liv, integer *lv, integer *n, doublereal *v, doublereal *x) {
        /* System generated locals */
        integer i__1, i__2;

        /* Local variables */
        static integer i__, j, k, l;
        static doublereal t;
        static integer w1;
        static doublereal gi;
        static integer x01, x11;
        static doublereal xi;
        static integer dg1, td1, tg1, ipi, ipn, nn1o2, temp0, temp1, ipiv2, step0,
                step1, dummy;
        //    extern logical stopx_(integer *);
        extern /* Subroutine */ int dd7dup_(doublereal *, doublereal *, integer *,
                integer *, integer *, integer *, doublereal *), dg7qsb_(
                doublereal *, doublereal *, doublereal *, doublereal *, integer *,
                integer *, integer *, integer *, doublereal *, integer *,
                integer *, integer *, integer *, doublereal *, doublereal *,
                doublereal *, doublereal *, doublereal *, doublereal *,
                doublereal *);
        extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
        extern /* Subroutine */ int da7sst_(integer *, integer *, integer *,
                doublereal *), dv7scp_(integer *, doublereal *, doublereal *);
        extern doublereal dv2nrm_(integer *, doublereal *);
        extern /* Subroutine */ int ds7ipr_(integer *, integer *, doublereal *),
                dv7ipr_(integer *, integer *, doublereal *), ds7lvm_(integer *,
                doublereal *, doublereal *, doublereal *), dv2axy_(integer *,
                doublereal *, doublereal *, doublereal *, doublereal *), dv7cpy_(
                integer *, doublereal *, doublereal *), dv7vmp_(integer *,
                doublereal *, doublereal *, doublereal *, integer *), i7pnvr_(
                integer *, integer *, integer *), dparck_(integer *, doublereal *,
                integer *, integer *, integer *, integer *, doublereal *);
        extern doublereal drldst_(integer *, doublereal *, doublereal *,
                doublereal *);
        extern /* Subroutine */ int divset_(integer *, integer *, integer *,
                integer *, doublereal *), ditsum_(doublereal *, doublereal *,
                integer *, integer *, integer *, integer *, doublereal *,
                doublereal *);
        static integer lstgst, rstrst;


        /*  ***  CARRY OUT  DMNHB (SIMPLY BOUNDED MINIMIZATION) ITERATIONS, */
        /*  ***  USING HESSIAN MATRIX PROVIDED BY THE CALLER. */

        /*  ***  PARAMETER DECLARATIONS  *** */


        /* --------------------------  PARAMETER USAGE  -------------------------- */

        /* D.... SCALE VECTOR. */
        /* FX... FUNCTION VALUE. */
        /* G.... GRADIENT VECTOR. */
        /* H.... LOWER TRIANGLE OF THE HESSIAN, STORED ROWWISE. */
        /* IV... INTEGER VALUE ARRAY. */
        /* LH... LENGTH OF H = P*(P+1)/2. */
        /* LIV.. LENGTH OF IV (AT LEAST 59 + 3*N). */
        /* LV... LENGTH OF V (AT LEAST 78 + N*(N+27)/2). */
        /* N.... NUMBER OF VARIABLES (COMPONENTS IN X AND G). */
        /* V.... FLOATING-POINT VALUE ARRAY. */
        /* X.... PARAMETER VECTOR. */

        /*  ***  DISCUSSION  *** */

        /*        PARAMETERS IV, N, V, AND X ARE THE SAME AS THE CORRESPONDING */
        /*     ONES TO  DMNHB (WHICH SEE), EXCEPT THAT V CAN BE SHORTER (SINCE */
        /*     THE PART OF V THAT  DMNHB USES FOR STORING G AND H IS NOT NEEDED). */
        /*     MOREOVER, COMPARED WITH  DMNHB, IV(1) MAY HAVE THE TWO ADDITIONAL */
        /*     OUTPUT VALUES 1 AND 2, WHICH ARE EXPLAINED BELOW, AS IS THE USE */
        /*     OF IV(TOOBIG) AND IV(NFGCAL).  THE VALUE IV(G), WHICH IS AN */
        /*     OUTPUT VALUE FROM  DMNHB, IS NOT REFERENCED BY DRMNHB OR THE */
        /*     SUBROUTINES IT CALLS. */

        /* IV(1) = 1 MEANS THE CALLER SHOULD SET FX TO F(X), THE FUNCTION VALUE */
        /*             AT X, AND CALL DRMNHB AGAIN, HAVING CHANGED NONE OF THE */
        /*             OTHER PARAMETERS.  AN EXCEPTION OCCURS IF F(X) CANNOT BE */
        /*             COMPUTED (E.G. IF OVERFLOW WOULD OCCUR), WHICH MAY HAPPEN */
        /*             BECAUSE OF AN OVERSIZED STEP.  IN THIS CASE THE CALLER */
        /*             SHOULD SET IV(TOOBIG) = IV(2) TO 1, WHICH WILL CAUSE */
        /*             DRMNHB TO IGNORE FX AND TRY A SMALLER STEP.  THE PARA- */
        /*             METER NF THAT  DMNH PASSES TO CALCF (FOR POSSIBLE USE BY */
        /*             CALCGH) IS A COPY OF IV(NFCALL) = IV(6). */
        /* IV(1) = 2 MEANS THE CALLER SHOULD SET G TO G(X), THE GRADIENT OF F AT */
        /*             X, AND H TO THE LOWER TRIANGLE OF H(X), THE HESSIAN OF F */
        /*             AT X, AND CALL DRMNHB AGAIN, HAVING CHANGED NONE OF THE */
        /*             OTHER PARAMETERS EXCEPT PERHAPS THE SCALE VECTOR D. */
        /*                  THE PARAMETER NF THAT  DMNHB PASSES TO CALCG IS */
        /*             IV(NFGCAL) = IV(7).  IF G(X) AND H(X) CANNOT BE EVALUATED, */
        /*             THEN THE CALLER MAY SET IV(NFGCAL) TO 0, IN WHICH CASE */
        /*             DRMNHB WILL RETURN WITH IV(1) = 65. */
        /*                  NOTE -- DRMNHB OVERWRITES H WITH THE LOWER TRIANGLE */
        /*             OF  DIAG(D)**-1 * H(X) * DIAG(D)**-1. */
        /* . */
        /*  ***  GENERAL  *** */

        /*     CODED BY DAVID M. GAY (WINTER, SPRING 1983). */

        /*        (SEE  DMNG AND  DMNH FOR REFERENCES.) */

        /* +++++++++++++++++++++++++++  DECLARATIONS  ++++++++++++++++++++++++++++ */

        /*  ***  LOCAL VARIABLES  *** */


        /*     ***  CONSTANTS  *** */


        /*  ***  NO INTRINSIC FUNCTIONS  *** */

        /*  ***  EXTERNAL FUNCTIONS AND SUBROUTINES  *** */


        /* DA7SST.... ASSESSES CANDIDATE STEP. */
        /* DIVSET.... PROVIDES DEFAULT IV AND V INPUT VALUES. */
        /* DD7TPR... RETURNS INNER PRODUCT OF TWO VECTORS. */
        /* DD7DUP.... UPDATES SCALE VECTOR D. */
        /* DG7QSB... COMPUTES APPROXIMATE OPTIMAL BOUNDED STEP. */
        /* I7PNVR... INVERTS PERMUTATION ARRAY. */
        /* DITSUM.... PRINTS ITERATION SUMMARY AND INFO ON INITIAL AND FINAL X. */
        /* DPARCK.... CHECKS VALIDITY OF INPUT IV AND V VALUES. */
        /* DRLDST... COMPUTES V(RELDX) = RELATIVE STEP SIZE. */
        /* DS7IPR... APPLIES PERMUTATION TO LOWER TRIANG. OF SYM. MATRIX. */
        /* DS7LVM... MULTIPLIES SYMMETRIC MATRIX TIMES VECTOR, GIVEN THE LOWER */
        /*             TRIANGLE OF THE MATRIX. */
        /* STOPX.... RETURNS .TRUE. IF THE BREAK KEY HAS BEEN PRESSED. */
        /* DV2NRM... RETURNS THE 2-NORM OF A VECTOR. */
        /* DV2AXY.... COMPUTES SCALAR TIMES ONE VECTOR PLUS ANOTHER. */
        /* DV7CPY.... COPIES ONE VECTOR TO ANOTHER. */
        /* DV7IPR... APPLIES PERMUTATION TO VECTOR. */
        /* DV7SCP... SETS ALL ELEMENTS OF A VECTOR TO A SCALAR. */
        /* DV7VMP... MULTIPLIES (OR DIVIDES) TWO VECTORS COMPONENTWISE. */

        /*  ***  SUBSCRIPTS FOR IV AND V  *** */


        /*  ***  IV SUBSCRIPT VALUES  *** */

        /*  ***  (NOTE THAT NC AND N0 ARE STORED IN IV(G0) AND IV(STLSTG) RESP.) */

        /* /6 */
        /*     DATA CNVCOD/55/, DG/37/, DTOL/59/, DTYPE/16/, IRC/29/, IVNEED/3/, */
        /*    1     KAGQT/33/, LMAT/42/, MODE/35/, MODEL/5/, MXFCAL/17/, */
        /*    2     MXITER/18/, N0/41/, NC/48/, NEXTIV/46/, NEXTV/47/, NFCALL/6/, */
        /*    3     NFGCAL/7/, NGCALL/30/, NITER/31/, PERM/58/, RADINC/8/, */
        /*    4     RESTOR/9/, STEP/40/, STGLIM/11/, TOOBIG/2/, VNEED/4/, W/34/, */
        /*    5     XIRC/13/, X0/43/ */
        /* /7 */
        /* / */

        /*  ***  V SUBSCRIPT VALUES  *** */

        /* /6 */
        /*     DATA DGNORM/1/, DINIT/38/, DSTNRM/2/, DTINIT/39/, D0INIT/40/, */
        /*    1     F/10/, F0/13/, FDIF/11/, GTSTEP/4/, INCFAC/23/, LMAX0/35/, */
        /*    2     LMAXS/36/, PHMXFC/21/, PREDUC/7/, RADFAC/16/, RADIUS/8/, */
        /*    3     RAD0/9/, RELDX/17/, STPPAR/5/, TUNER4/29/, TUNER5/30/ */
        /* /7 */
        /* / */

        /* /6 */
        /*     DATA NEGONE/-1.D+0/, ONE/1.D+0/, ONEP2/1.2D+0/, ZERO/0.D+0/ */
        /* /7 */
        /* / */

        /* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

        /* Parameter adjustments */
        --h__;
        --iv;
        --v;
        --x;
        --g;
        --d__;
        b -= 3;

        /* Function Body */
        i__ = iv[1];
        if (i__ == 1) {
            goto L50;
        }
        if (i__ == 2) {
            goto L60;
        }

        /*  ***  CHECK VALIDITY OF IV AND V INPUT VALUES  *** */

        if (iv[1] == 0) {
            divset_(&c__2, &iv[1], liv, lv, &v[1]);
        }
        if (iv[1] < 12) {
            goto L10;
        }
        if (iv[1] > 13) {
            goto L10;
        }
        iv[4] = iv[4] + *n * (*n + 27) / 2 + 7;
        iv[3] += *n * 3;
L10:
        dparck_(&c__2, &d__[1], &iv[1], liv, lv, n, &v[1]);
        i__ = iv[1] - 2;
        if (i__ > 12) {
            goto L999;
        }
        nn1o2 = *n * (*n + 1) / 2;
        if (*lh >= nn1o2) {
            switch (i__) {
                case 1: goto L250;
                case 2: goto L250;
                case 3: goto L250;
                case 4: goto L250;
                case 5: goto L250;
                case 6: goto L250;
                case 7: goto L190;
                case 8: goto L150;
                case 9: goto L190;
                case 10: goto L20;
                case 11: goto L20;
                case 12: goto L30;
            }
        }
        iv[1] = 81;
        goto L440;

        /*  ***  STORAGE ALLOCATION  *** */

L20:
        iv[59] = iv[42] + nn1o2;
        iv[43] = iv[59] + (*n << 1);
        iv[40] = iv[43] + (*n << 1);
        iv[37] = iv[40] + *n * 3;
        iv[34] = iv[37] + (*n << 1);
        iv[47] = iv[34] + (*n << 2) + 7;
        iv[46] = iv[58] + *n * 3;
        if (iv[1] != 13) {
            goto L30;
        }
        iv[1] = 14;
        goto L999;

        /*  ***  INITIALIZATION  *** */

L30:
        iv[31] = 0;
        iv[6] = 1;
        iv[30] = 1;
        iv[7] = 1;
        iv[35] = -1;
        iv[5] = 1;
        iv[11] = 1;
        iv[2] = 0;
        iv[55] = 0;
        iv[8] = 0;
        iv[48] = *n;
        v[9] = 0.;
        v[5] = 0.;
        if (v[38] >= 0.) {
            dv7scp_(n, &d__[1], &v[38]);
        }
        k = iv[59];
        if (v[39] > 0.) {
            dv7scp_(n, &v[k], &v[39]);
        }
        k += *n;
        if (v[40] > 0.) {
            dv7scp_(n, &v[k], &v[40]);
        }

        /*  ***  CHECK CONSISTENCY OF B AND INITIALIZE IP ARRAY  *** */

        ipi = iv[58];
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            iv[ipi] = i__;
            ++ipi;
            if (b[(i__ << 1) + 1] > b[(i__ << 1) + 2]) {
                goto L420;
            }
            /* L40: */
        }

        /*  ***  GET INITIAL FUNCTION VALUE  *** */

        iv[1] = 1;
        goto L450;

L50:
        v[10] = *fx;
        if (iv[35] >= 0) {
            goto L250;
        }
        v[13] = *fx;
        iv[1] = 2;
        if (iv[2] == 0) {
            goto L999;
        }
        iv[1] = 63;
        goto L440;

        /*  ***  MAKE SURE GRADIENT COULD BE COMPUTED  *** */

L60:
        if (iv[2] == 0) {
            goto L70;
        }
        iv[1] = 65;
        goto L440;

        /*  ***  UPDATE THE SCALE VECTOR D  *** */

L70:
        dg1 = iv[37];
        if (iv[16] <= 0) {
            goto L90;
        }
        k = dg1;
        j = 0;
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            j += i__;
            v[k] = h__[j];
            ++k;
            /* L80: */
        }
        dd7dup_(&d__[1], &v[dg1], &iv[1], liv, lv, n, &v[1]);

        /*  ***  COMPUTE SCALED GRADIENT AND ITS NORM  *** */

L90:
        dg1 = iv[37];
        dv7vmp_(n, &v[dg1], &g[1], &d__[1], &c_n1);

        /*  ***  COMPUTE SCALED HESSIAN  *** */

        k = 1;
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            t = 1. / d__[i__];
            i__2 = i__;
            for (j = 1; j <= i__2; ++j) {
                h__[k] = t * h__[k] / d__[j];
                ++k;
                /* L100: */
            }
            /* L110: */
        }

        /*  ***  CHOOSE INITIAL PERMUTATION  *** */

        ipi = iv[58];
        ipn = ipi + *n;
        ipiv2 = ipn - 1;
        /*     *** INVERT OLD PERMUTATION ARRAY *** */
        i7pnvr_(n, &iv[ipn], &iv[ipi]);
        k = iv[48];
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            if (b[(i__ << 1) + 1] >= b[(i__ << 1) + 2]) {
                goto L120;
            }
            xi = x[i__];
            gi = g[i__];
            if (xi <= b[(i__ << 1) + 1] && gi > 0.) {
                goto L120;
            }
            if (xi >= b[(i__ << 1) + 2] && gi < 0.) {
                goto L120;
            }
            iv[ipi] = i__;
            ++ipi;
            j = ipiv2 + i__;
            /*           *** DISALLOW CONVERGENCE IF X(I) HAS JUST BEEN FREED *** */
            if (iv[j] > k) {
                iv[55] = 0;
            }
            goto L130;
L120:
            --ipn;
            iv[ipn] = i__;
L130:
            ;
        }
        iv[48] = ipn - iv[58];

        /*  ***  PERMUTE SCALED GRADIENT AND HESSIAN ACCORDINGLY  *** */

        ipi = iv[58];
        ds7ipr_(n, &iv[ipi], &h__[1]);
        dv7ipr_(n, &iv[ipi], &v[dg1]);
        v[1] = 0.;
        if (iv[48] > 0) {
            v[1] = dv2nrm_(&iv[48], &v[dg1]);
        }

        if (iv[55] != 0) {
            goto L430;
        }
        if (iv[35] == 0) {
            goto L380;
        }

        /*  ***  ALLOW FIRST STEP TO HAVE SCALED 2-NORM AT MOST V(LMAX0)  *** */

        v[8] = v[35] / (v[21] + 1.);

        iv[35] = 0;


        /* -----------------------------  MAIN LOOP  ----------------------------- */


        /*  ***  PRINT ITERATION SUMMARY, CHECK ITERATION LIMIT  *** */

L140:
        ditsum_(&d__[1], &g[1], &iv[1], liv, lv, n, &v[1], &x[1]);
L150:
        k = iv[31];
        if (k < iv[18]) {
            goto L160;
        }
        iv[1] = 10;
        goto L440;

L160:
        iv[31] = k + 1;

        /*  ***  INITIALIZE FOR START OF NEXT ITERATION  *** */

        x01 = iv[43];
        v[13] = v[10];
        iv[29] = 4;
        iv[33] = -1;

        /*     ***  COPY X TO X0  *** */

        dv7cpy_(n, &v[x01], &x[1]);

        /*  ***  UPDATE RADIUS  *** */

        if (k == 0) {
            goto L180;
        }
        step1 = iv[40];
        k = step1;
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            v[k] = d__[i__] * v[k];
            ++k;
            /* L170: */
        }
        t = v[16] * dv2nrm_(n, &v[step1]);
        if (v[16] < 1. || t > v[8]) {
            v[8] = t;
        }

        /*  ***  CHECK STOPX AND FUNCTION EVALUATION LIMIT  *** */

L180:
        if (!stopx_(&dummy)) {
            goto L200;
        }
        iv[1] = 11;
        goto L210;

        /*     ***  COME HERE WHEN RESTARTING AFTER FUNC. EVAL. LIMIT OR STOPX. */

L190:
        if (v[10] >= v[13]) {
            goto L200;
        }
        v[16] = 1.;
        k = iv[31];
        goto L160;

L200:
        if (iv[6] < iv[17]) {
            goto L220;
        }
        iv[1] = 9;
L210:
        if (v[10] >= v[13]) {
            goto L440;
        }

        /*        ***  IN CASE OF STOPX OR FUNCTION EVALUATION LIMIT WITH */
        /*        ***  IMPROVED V(F), EVALUATE THE GRADIENT AT X. */

        iv[55] = iv[1];
        goto L370;

        /* . . . . . . . . . . . . .  COMPUTE CANDIDATE STEP  . . . . . . . . . . */

L220:
        step1 = iv[40];
        l = iv[42];
        w1 = iv[34];
        ipi = iv[58];
        ipn = ipi + *n;
        ipiv2 = ipn + *n;
        tg1 = iv[37];
        td1 = tg1 + *n;
        x01 = iv[43];
        x11 = x01 + *n;
        dg7qsb_(&b[3], &d__[1], &h__[1], &g[1], &iv[ipi], &iv[ipn], &iv[ipiv2], &
                iv[33], &v[l], lv, n, &iv[41], &iv[48], &v[step1], &v[td1], &v[
                tg1], &v[1], &v[w1], &v[x11], &v[x01]);
        if (iv[29] != 6) {
            goto L230;
        }
        if (iv[9] != 2) {
            goto L250;
        }
        rstrst = 2;
        goto L260;

        /*  ***  CHECK WHETHER EVALUATING F(X0 + STEP) LOOKS WORTHWHILE  *** */

L230:
        iv[2] = 0;
        if (v[2] <= 0.) {
            goto L250;
        }
        if (iv[29] != 5) {
            goto L240;
        }
        if (v[16] <= 1.) {
            goto L240;
        }
        if (v[7] > v[11] * 1.2) {
            goto L240;
        }
        if (iv[9] != 2) {
            goto L250;
        }
        rstrst = 0;
        goto L260;

        /*  ***  COMPUTE F(X0 + STEP)  *** */

L240:
        dv2axy_(n, &x[1], &c_b38, &v[step1], &v[x01]);
        ++iv[6];
        iv[1] = 1;
        goto L450;

        /* . . . . . . . . . . . . .  ASSESS CANDIDATE STEP  . . . . . . . . . . . */

L250:
        rstrst = 3;
L260:
        x01 = iv[43];
        v[17] = drldst_(n, &d__[1], &x[1], &v[x01]);
        da7sst_(&iv[1], liv, lv, &v[1]);
        step1 = iv[40];
        lstgst = step1 + (*n << 1);
        i__ = iv[9] + 1;
        switch (i__) {
            case 1: goto L300;
            case 2: goto L270;
            case 3: goto L280;
            case 4: goto L290;
        }
L270:
        dv7cpy_(n, &x[1], &v[x01]);
        goto L300;
L280:
        dv7cpy_(n, &v[lstgst], &x[1]);
        goto L300;
L290:
        dv7cpy_(n, &x[1], &v[lstgst]);
        dv2axy_(n, &v[step1], &c_b43, &v[x01], &x[1]);
        v[17] = drldst_(n, &d__[1], &x[1], &v[x01]);
        iv[9] = rstrst;

L300:
        k = iv[29];
        switch (k) {
            case 1: goto L310;
            case 2: goto L340;
            case 3: goto L340;
            case 4: goto L340;
            case 5: goto L310;
            case 6: goto L320;
            case 7: goto L330;
            case 8: goto L330;
            case 9: goto L330;
            case 10: goto L330;
            case 11: goto L330;
            case 12: goto L330;
            case 13: goto L410;
            case 14: goto L380;
        }

        /*     ***  RECOMPUTE STEP WITH NEW RADIUS  *** */

L310:
        v[8] = v[16] * v[2];
        goto L180;

        /*  ***  COMPUTE STEP OF LENGTH V(LMAXS) FOR SINGULAR CONVERGENCE TEST. */

L320:
        v[8] = v[36];
        goto L220;

        /*  ***  CONVERGENCE OR FALSE CONVERGENCE  *** */

L330:
        iv[55] = k - 4;
        if (v[10] >= v[13]) {
            goto L430;
        }
        if (iv[13] == 14) {
            goto L430;
        }
        iv[13] = 14;

        /* . . . . . . . . . . . .  PROCESS ACCEPTABLE STEP  . . . . . . . . . . . */

L340:
        if (iv[29] != 3) {
            goto L370;
        }
        temp1 = lstgst;

        /*     ***  PREPARE FOR GRADIENT TESTS  *** */
        /*     ***  SET  TEMP1 = HESSIAN * STEP + G(X0) */
        /*     ***             = DIAG(D) * (H * STEP + G(X0)) */

        k = temp1;
        step0 = step1 - 1;
        ipi = iv[58];
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            j = iv[ipi];
            ++ipi;
            step1 = step0 + j;
            v[k] = d__[j] * v[step1];
            ++k;
            /* L350: */
        }
        /*        USE X0 VECTOR AS TEMPORARY. */
        ds7lvm_(n, &v[x01], &h__[1], &v[temp1]);
        temp0 = temp1 - 1;
        ipi = iv[58];
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            j = iv[ipi];
            ++ipi;
            temp1 = temp0 + j;
            v[temp1] = d__[j] * v[x01] + g[j];
            ++x01;
            /* L360: */
        }

        /*  ***  COMPUTE GRADIENT AND HESSIAN  *** */

L370:
        ++iv[30];
        iv[2] = 0;
        iv[1] = 2;
        goto L450;

L380:
        iv[1] = 2;
        if (iv[29] != 3) {
            goto L140;
        }

        /*  ***  SET V(RADFAC) BY GRADIENT TESTS  *** */

        step1 = iv[40];
        /*     *** TEMP1 = STLSTG *** */
        temp1 = step1 + (*n << 1);

        /*     ***  SET  TEMP1 = DIAG(D)**-1 * (HESSIAN*STEP + (G(X0)-G(X)))  *** */

        k = temp1;
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            v[k] = (v[k] - g[i__]) / d__[i__];
            ++k;
            /* L390: */
        }

        /*     ***  DO GRADIENT TESTS  *** */

        if (dv2nrm_(n, &v[temp1]) <= v[1] * v[29]) {
            goto L400;
        }
        if (dd7tpr_(n, &g[1], &v[step1]) >= v[4] * v[30]) {
            goto L140;
        }
L400:
        v[16] = v[23];
        goto L140;

        /* . . . . . . . . . . . . . .  MISC. DETAILS  . . . . . . . . . . . . . . */

        /*  ***  BAD PARAMETERS TO ASSESS  *** */

L410:
        iv[1] = 64;
        goto L440;

        /*  ***  INCONSISTENT B  *** */

L420:
        iv[1] = 82;
        goto L440;

        /*  ***  PRINT SUMMARY OF FINAL ITERATION AND OTHER REQUESTED ITEMS  *** */

L430:
        iv[1] = iv[55];
        iv[55] = 0;
L440:
        ditsum_(&d__[1], &g[1], &iv[1], liv, lv, n, &v[1], &x[1]);
        goto L999;

        /*  ***  PROJECT X INTO FEASIBLE REGION (PRIOR TO COMPUTING F OR G)  *** */

L450:
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            if (x[i__] < b[(i__ << 1) + 1]) {
                x[i__] = b[(i__ << 1) + 1];
            }
            if (x[i__] > b[(i__ << 1) + 2]) {
                x[i__] = b[(i__ << 1) + 2];
            }
            /* L460: */
        }

L999:
        return 0;

        /*  ***  LAST CARD OF DRMNHB FOLLOWS  *** */
    } /* drmnhb_ */




}//end namespace port
#endif
