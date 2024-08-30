
/*
 * Copyright 1995, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 *
 * UNPUBLISHED -- Rights reserved under the copyright laws of the United
 * States.   Use of a copyright notice is precautionary only and does not
 * imply publication or disclosure.
 *
 * U.S. GOVERNMENT RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to restrictions
 * as set forth in FAR 52.227.19(c)(2) or subparagraph (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS 252.227-7013 and/or
 * in similar or successor clauses in the FAR, or the DOD or NASA FAR
 * Supplement.  Contractor/manufacturer is Silicon Graphics, Inc.,
 * 2011 N. Shoreline Blvd. Mountain View, CA 94039-7311.
 *
 * THE CONTENT OF THIS WORK CONTAINS CONFIDENTIAL AND PROPRIETARY
 * INFORMATION OF SILICON GRAPHICS, INC. ANY DUPLICATION, MODIFICATION,
 * DISTRIBUTION, OR DISCLOSURE IN ANY FORM, IN WHOLE, OR IN PART, IS STRICTLY
 * PROHIBITED WITHOUT THE PRIOR EXPRESS WRITTEN PERMISSION OF SILICON
 * GRAPHICS, INC.
 *
 */

/*
 * File:	mtxcatl.c
 * Creator:	hsa@sgi.com
 * Create Date:	Thu Nov  2 13:03:02 PST 1995
 *
 */

#include <libultraship.h>

// void guMtxXFMF(Mtx*, float, float, float, float*, float*, float*);
void guMtxCatF(float mf[4][4], float nf[4][4], float res[4][4]);

void guMtxCatL(Mtx* m, Mtx* n, Mtx* res) {
    float mf[4][4], nf[4][4], resf[4][4];

    guMtxL2F(mf, m);
    guMtxL2F(nf, n);

    guMtxCatF(mf, nf, resf);

    guMtxF2L(resf, res);
}

void guMtxXFML(Mtx* m, float x, float y, float z, float* ox, float* oy, float* oz) {
    float mf[4][4];

    guMtxL2F(mf, m);

    guMtxXFMF((Mtx*) mf, x, y, z, ox, oy, oz);
}
