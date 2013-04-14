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
#ifndef _XBM_H
#define _XBM_H

/**
 * XBM structure plus frequently used "stride".
 */
typedef struct {
    /*
     * Image width in pixels
     */
    unsigned int width;
    /*
     * Image height in pixels
     */
    unsigned int height;
    /*
     * Image width in bytes
     */
    unsigned int stride;
    /*
     * Image bits
     */
    unsigned char *bits;
} XBM;

/**
 * Define stride from width
 */
#define XBMStride(w) ((w+7)/8)
/**
 * Number of bytes in the image bit buffer
 */
#define XBMSize(xp) ((xp)->stride*(xp)->height)
/*
 * Initialize allocated XBM structure.  This is used by XBMCreate
 * before returning a new structure.
 */
#define XBMInit(xp,w,h)                             \
    (xp)->width = w;                                \
    (xp)->height = h;                               \
    (xp)->stride = XBMStride(w);                    \
    (xp)->bits = malloc(XBMSize(xp));               \
    XBMClear(xp);

/**
 * XBM 'constructor'
 *
 * @param width  Width of the resulting XBM
 * @param height Height of the resulting XBM
 */
XBM *XBMCreate( unsigned int width, unsigned int height );

/**
 * XBM 'constructor' from XBM source 
 *
 * @param width  XBM source pixel width
 * @param height XBM source pixel height
 * @param bits   XBM source bit stream 
 */
XBM *XBMCreateFrom( unsigned int width, unsigned int height, unsigned char* bits);

/**
 * XBM 'destructor'
 *
 * @param xbm  Reference to the XBM to be freed
 */
void XBMFree( XBM *xbm );

/**
 * Fill the image bit buffer with value zero.
 *
 * @param xbm  Reference to the XBM to be cleared
 */
void XBMClear( XBM *xbm );

/**
 * Fill the image bit buffer with the argument value
 *
 * @param xbm  Reference to the XBM to be filled
 */
void XBMFill( XBM *xbm, unsigned char value);

/**
 * Returns the bit-value at a certain x-pixel, y-pixel location in XBM
 * pixel order
 *
 * @param xbm  Reference to a XBM
 * @param x    X-pixel location in the XBM
 * @param y    Y-pixel location in the XBM
 * 
 * @returns the bit-value at the requested location
 */
unsigned char XBMGetBit( XBM *xbm, unsigned int x, unsigned int y );

/**
 * Sets the bit-value at a certain x-pixel, y-pixel location in XBM
 * pixel order
 *
 * @param xbm    Reference to a XBM
 * @param x      X-pixel location in the XBM
 * @param y      Y-pixel location in the XBM
 * @param value  Value of the bit you want to set
 */
void XBMSetBit( XBM *xbm, unsigned int x, unsigned int y, unsigned char value );

/**
 * Returns the pixel block (byte) at a certain x-byte, y-pixel
 * location in XBM pixel order
 *
 * @param xbm   Reference to a XBM
 * @param xb    X-byte location in the XBM
 * @param yp    Y-pixel location in the XBM
 * 
 * @returns the pixel block (byte) at the requested location, or zero
 * if out-of-range
 */
unsigned char XBMGetBlock( XBM *xbm, unsigned int xb, unsigned int yp);

/**
 * Set the pixel block (byte) at a certain x-byte, y-pixel location in
 * XBM pixel order
 *
 * @param xbm    Reference to a XBM
 * @param xb     X-byte location in the XBM
 * @param yp     Y-pixel location in the XBM
 * @param xblock XBM pixel block
 */
void XBMSetBlock( XBM *xbm, unsigned int xb, unsigned int yp, unsigned char xblock);

/**
 * Returns the pixel block (byte) at a certain x-pixel, y-pixel
 * location in raster pixel order
 *
 * @param xbm   Reference to a XBM
 * @param x     X-pixel location in the XBM
 * @param y     Y-pixel location in the XBM
 * 
 * @returns The pixel block (byte) at the requested location in raster
 * bit order, or zero if out-of-range
 */
unsigned char XBMGetBlockRaster(XBM* xbm, unsigned int x, unsigned int y);

/**
 * Copy a raster bit block into an XBM image buffer
 *
 * @param xbm     Reference to a XBM
 * @param x       X-pixel location in the XBM
 * @param y       Y-pixel location in the XBM
 * @param w       Raster block pixel width (w <= 8)
 * @param rblock  Raster bit order pixel block
 */
void XBMSetRasterBlock(XBM* xbm, unsigned int x, unsigned int y, unsigned int w, unsigned char rblock);

/**
 * Copy a raster bit block stream into an XBM image buffer
 * 
 * @param x     Destination horizontal pixel coordinate from top-left origin
 * 
 * @param y     Destination vertical pixel coordinate from top-left origin
 * 
 * @param w     Source image pixel width dimension
 * 
 * @param h     Source image pixel height dimension
 * 
 * @param bits  Raster bit block stream in 8 bit blocks
 * 
 * Raster Bit Block Stream
 * 
 *   Coordinate space {0,0} at top-left, the MSB in the first byte
 * 
 *   One bit pixel
 * 
 *   Horizontal raster image in 8 bit bytes
 * 
 *     Raster block width   (rbw, bytes)  =  max(1,(w / 8))
 *     Raster block height  (rbh, bytes)  =  (h)
 *     Raster block length  (rbl, bytes)  =  (rbw * rbh)
 * 
 *   Each horizontal scan line ends with unused bits when 0 != (w % 8)
 */
void XBMSetRasterStream(XBM* xbm, unsigned int x, unsigned int y,
                        unsigned int w, unsigned int h, 
                        const unsigned char* bits);

/**
 * Copies one XBM into another XBM
 *
 * @param dst     Reference to the destination XBM
 * @param dx      Destination x-pixel location in the destination XBM
 * @param dy      Destination y-pixel location in the destination XBM
 * @param src     Reference to the source XBM
 * @param sx      Source x-pixel location in the source XBM
 * @param sy      Source y-pixel location in the source XBM
 * @param width   Width of the rect to be copied
 * @param height  Height of the rect to be copied
 */
void XBMCopy( XBM *dst, unsigned int dx, unsigned int dy, XBM *src, unsigned int sx, unsigned int sy, unsigned int width, unsigned int height );

/**
 * Write an XBM file
 * 
 * @param xbm  Image content
 * @param name Symbolic prefix
 * @param out  File output
 */
void XBMWriteFile( XBM *xbm, const char *name, FILE* out);


#ifdef _XBM_C
/*
 * Convert raster bits to xbm bits by flipping bit order.
 * 
 * The XBM format employs a flipped bit representation wherein the
 * left pixel is the right bit.
 * 
 * This table performs pixel block (byte) flipping in both input and
 * output directions.
 * 
 *   output = XBMFlipBits[input]
 *   input = XBMFlipBits[output]
 */
unsigned char XBMFlipBits[256] = {
    0x00,0x80,0x40,0xc0,0x20,0xa0,0x60,0xe0,0x10,0x90,0x50,0xd0,0x30,0xb0,0x70,0xf0,
    0x08,0x88,0x48,0xc8,0x28,0xa8,0x68,0xe8,0x18,0x98,0x58,0xd8,0x38,0xb8,0x78,0xf8,
    0x04,0x84,0x44,0xc4,0x24,0xa4,0x64,0xe4,0x14,0x94,0x54,0xd4,0x34,0xb4,0x74,0xf4,
    0x0c,0x8c,0x4c,0xcc,0x2c,0xac,0x6c,0xec,0x1c,0x9c,0x5c,0xdc,0x3c,0xbc,0x7c,0xfc,
    0x02,0x82,0x42,0xc2,0x22,0xa2,0x62,0xe2,0x12,0x92,0x52,0xd2,0x32,0xb2,0x72,0xf2,
    0x0a,0x8a,0x4a,0xca,0x2a,0xaa,0x6a,0xea,0x1a,0x9a,0x5a,0xda,0x3a,0xba,0x7a,0xfa,
    0x06,0x86,0x46,0xc6,0x26,0xa6,0x66,0xe6,0x16,0x96,0x56,0xd6,0x36,0xb6,0x76,0xf6,
    0x0e,0x8e,0x4e,0xce,0x2e,0xae,0x6e,0xee,0x1e,0x9e,0x5e,0xde,0x3e,0xbe,0x7e,0xfe,
    0x01,0x81,0x41,0xc1,0x21,0xa1,0x61,0xe1,0x11,0x91,0x51,0xd1,0x31,0xb1,0x71,0xf1,
    0x09,0x89,0x49,0xc9,0x29,0xa9,0x69,0xe9,0x19,0x99,0x59,0xd9,0x39,0xb9,0x79,0xf9,
    0x05,0x85,0x45,0xc5,0x25,0xa5,0x65,0xe5,0x15,0x95,0x55,0xd5,0x35,0xb5,0x75,0xf5,
    0x0d,0x8d,0x4d,0xcd,0x2d,0xad,0x6d,0xed,0x1d,0x9d,0x5d,0xdd,0x3d,0xbd,0x7d,0xfd,
    0x03,0x83,0x43,0xc3,0x23,0xa3,0x63,0xe3,0x13,0x93,0x53,0xd3,0x33,0xb3,0x73,0xf3,
    0x0b,0x8b,0x4b,0xcb,0x2b,0xab,0x6b,0xeb,0x1b,0x9b,0x5b,0xdb,0x3b,0xbb,0x7b,0xfb,
    0x07,0x87,0x47,0xc7,0x27,0xa7,0x67,0xe7,0x17,0x97,0x57,0xd7,0x37,0xb7,0x77,0xf7,
    0x0f,0x8f,0x4f,0xcf,0x2f,0xaf,0x6f,0xef,0x1f,0x9f,0x5f,0xdf,0x3f,0xbf,0x7f,0xff
};
#else
extern unsigned char XBMFlipBits[256];
#endif

#ifndef max
#define max(a,b) ((a > b)?(a):(b))
#endif

#ifndef min
#define min(a,b) ((a < b)?(a):(b))
#endif

#endif
