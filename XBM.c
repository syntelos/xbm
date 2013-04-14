/*
 * XBM 
 * Copyright (C) 2011 Tom de Grunt <tom@degrunt.nl>
 * Copyright (C) 2013 John Pritchard (jdp@syntelos.org)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _XBM_C

#include "XBM.h"

/*
 */
XBM *XBMCreate( unsigned int width, unsigned int height ) {
   
    XBM *result = malloc( sizeof( XBM ) );

    XBMInit(result,width,height);
   
    return result;
}
/*
 */
XBM *XBMCreateFrom( unsigned int width, unsigned int height, unsigned char* bits) {
   
    XBM *result = malloc( sizeof( XBM ) );

    XBMInit(result,width,height);

    memcpy(result->bits,bits,XBMSize(result));
   
    return result;
}
/*
 */
void XBMFree( XBM *xbm ) {
 
    free( xbm->bits );
    free( xbm );
}
/*
 */
void XBMClear( XBM *xbm ){

    memset(xbm->bits,0,XBMSize(xbm));
}
/*
 */
void XBMFill( XBM *xbm, unsigned char value){

    memset(xbm->bits,value,XBMSize(xbm));
}
/*
 */
unsigned char XBMGetBit( XBM *xbm, unsigned int x, unsigned int y ) {
	
	unsigned int s = x >> 3;
	unsigned int b = x & 0x7;
	unsigned char byteValue = XBMGetBlock( xbm, s, y );
	
	return (byteValue & ( 1 << b )) >> b;		
}
/*
 */
void XBMSetBit( XBM *xbm, unsigned int x, unsigned int y, unsigned char value ) {
	
	unsigned int s = x >> 3;
	unsigned int b = x & 0x7;

	if ( (x >= xbm->width) ||
         (y >= xbm->height) )

		return;

	else if (value){

		xbm->bits[xbm->stride * y + s] |= 1 << b;
    }
    else {
		xbm->bits[xbm->stride * y + s] &= ~(1 << b);
    }
}
/*
 */
unsigned char XBMGetBlock( XBM *xbm, unsigned int s, unsigned int y ) {

    if ( (s >= xbm->stride) || 
        (y >= xbm->height) )

        return 0;
    else
        return xbm->bits[(xbm->stride * y) + s]; 
}
/*
 */
void XBMSetBlock( XBM *xbm, unsigned int s, unsigned int y, unsigned char b) {

    if ( (s >= xbm->stride) || 
         (y >= xbm->height) )

        return ;
    else
        xbm->bits[(xbm->stride * y) + s] = b; 
}
/*
 */
unsigned char XBMGetBlockRaster(XBM* xbm, unsigned int x, unsigned int y){

    unsigned int s = (x>>3);
	unsigned int b = x & 0x7;

    if (0 == b)
        return XBMFlipBits[XBMGetBlock(xbm,s,y)];

    else if ( (x >= xbm->width) || 
              (y >= xbm->height) )

        return 0;
    else {
        unsigned int ofs = (xbm->stride * y) + s;

        unsigned char sa = b;
        unsigned char sb = (8-b);
        unsigned char ma = (0xff>>b);
        unsigned char mb = (0xff<<sb);

        unsigned char ra = (XBMFlipBits[xbm->bits[ofs]] & ma) << sa;

        ofs += 1;

        unsigned char rb = (XBMFlipBits[xbm->bits[ofs]] & mb) >> sb;

        return (ra | rb);
    }
}
/*
 */
void XBMSetRasterBlock(XBM* xbm, unsigned int x, unsigned int y, unsigned int w, unsigned char rblock){
	unsigned int s = x >> 3;
	unsigned int b = x & 0x7;

    if (0 == b){
        if (8 == w){

            XBMSetBlock(xbm,s,y,XBMFlipBits[rblock]);
        }
        else if (0 < w){

            const unsigned int ofs = (xbm->stride * y) + s;
            const unsigned int iw = (8-w);
            /*
             * Mask width
             */
            const unsigned char mr = (0xff<<iw);
            const unsigned char mx = (0xff>>w);

            unsigned char ra0 = (XBMFlipBits[xbm->bits[ofs]] & mx);

            unsigned char ra1 = (rblock & mr);

            unsigned char ra = (ra0|ra1);

            xbm->bits[ofs] = XBMFlipBits[ra];
        }
    }
    else if ( ((x+w) > xbm->width) || 
              (y >= xbm->height) ||
              (0 == w) || (8 < w) )

        return ;
    else {
        const unsigned int iw = (8-w);
        const unsigned char sa = b;
        const unsigned char sb = (8-b);

        unsigned int ofs = (xbm->stride * y) + s;

        unsigned char mr, mx;
        /*
         * Mask shared and width
         */
        mx = (0xff<<sb)|(0xff>>(b+w));
        mr = (0xff<<b)&(0xff<<iw);

        unsigned char ra0 = (XBMFlipBits[xbm->bits[ofs]] & mx);

        unsigned char ra1 = (rblock & mr) >> sa;

        unsigned char ra = (ra0|ra1);

        xbm->bits[ofs] = XBMFlipBits[ra]; 

        ofs += 1;

        if (b > iw){

            mx = (0xff>>(b-iw));

            mr = (0xff>>sb)&(0xff<<iw);

            unsigned char rb0 = (rblock & mr) << sb;

            unsigned char rb1 = (XBMFlipBits[xbm->bits[ofs]] & mx);

            unsigned char rb = (rb0|rb1);

            xbm->bits[ofs] = XBMFlipBits[rb];
        }
    }
}
/*
 */
void XBMSetRasterStream(XBM* xbm, unsigned int x, unsigned int y,
                        unsigned int w, unsigned int h, 
                        const unsigned char* bits)
{
    const unsigned int riw = max(1,((w+7))/8);

    const unsigned int srz = ((8 < w)?(8):(w));

    const unsigned int riz = (riw * h);

    unsigned int stride = 0;

    unsigned int cc, xx = x, yy = y;

    for (cc = 0; cc < riz; cc++){

        XBMSetRasterBlock(xbm,xx,yy,srz,bits[cc]);

        stride += srz;

        if ( stride >= w){
            stride = 0;
            xx = x;
            yy += 1;
        }
        else {
            xx += srz;
        }
    }
}
/*
 */
void XBMCopy( XBM *dst, unsigned int dx, unsigned int dy, XBM *src, unsigned int sx, unsigned int sy, unsigned int width, unsigned int height ) {

	unsigned int x, y;

	for ( y = 0; y < height; y++ ) {
		for ( x = 0; x < width; x++ ) {

			XBMSetBit( dst, x + dx, y + dy, XBMGetBit( src, x + sx, y + sy ) );
		}
	}
}
/*
 */
void XBMWriteFile( XBM *xbm, const char *name, FILE* out){
 
    unsigned int s, y;
 
    fprintf(out, "#define %s_width %d\n", name, xbm->width );  
    fprintf(out, "#define %s_height %d\n", name, xbm->height );  
    fprintf(out, "static char %s_bits[] = {\n   ", name );
   
    for ( y = 0; y < xbm->height; y++ ) {

        if (0 < y){
            fprintf(out, "\n   " );
        }

        for ( s = 0; s < xbm->stride; s++ ) {
       
            fprintf(out, "0x%02x", XBMGetBlock( xbm, s, y ) );

            if ( ( y < xbm->height-1 ) || ( s < xbm->stride-1 ) ) {
                fprintf(out, ", " );	
            }
        }
    }
   
    fprintf(out, "\n};\n" );
}
