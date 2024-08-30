
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
 * File:	mtxcatf.c
 * Creator:	hsa@sgi.com
 * Create Date:	Thu Nov  2 13:03:02 PST 1995
 *
 */

#include <libultraship.h>

void guMtxCatF(float mf[4][4], float nf[4][4], float res[4][4]) {
    int i, j, k;
    float temp[4][4];

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            temp[i][j] = 0.0;
            for (k = 0; k < 4; k++) {
                temp[i][j] += mf[i][k] * nf[k][j];
            }
        }
    }

    /* make sure we handle case where result is an input */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            res[i][j] = temp[i][j];
        }
    }
}

void guMtxXFMF(float mf[4][4], float x, float y, float z, float* ox, float* oy, float* oz) {
    *ox = mf[0][0] * x + mf[1][0] * y + mf[2][0] * z + mf[3][0];
    *oy = mf[0][1] * x + mf[1][1] * y + mf[2][1] * z + mf[3][1];
    *oz = mf[0][2] * x + mf[1][2] * y + mf[2][2] * z + mf[3][2];
}
