/*
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "XBM.h"

#include "hexagons.bmp"

#define test_width 100
#define test_height 100

int test_xcopy(){
    /*
     * input, init
     */
    XBM* src = XBMCreateFrom(hexagons_width,hexagons_height,hexagons_bits);

    XBM* dst = XBMCreate(test_width,test_height);
    /*
     * tile-copy
     */
    unsigned int x, y;

    for (x = 0; x < test_width; x += hexagons_width){

        for (y = 0; y < test_height; y += hexagons_height){

            XBMCopy(dst,x,y,src,0,0,hexagons_width,hexagons_height);
        }
    }
    /*
     * output
     */
    FILE* file = fopen("test.xbm","w");
    if (file){

        XBMWriteFile(dst,"test",file);

        fclose(file);

        XBMFree(src);

        XBMFree(dst);

        fprintf(stderr,"Wrote file 'test.xbm'\n");
        return 0;
    }
    else {
        fprintf(stderr,"Failed to open file 'test.xbm'\n");
        return 1;
    }
}

int main(int argc, char** argv){


    return test_xcopy();
}
