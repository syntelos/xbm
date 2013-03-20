
  XBM package

  Authors

    Tom de Grunt

      https://github.com/tdegrunt/barcode

    John Pritchard

      https://github.com/syntelos/xbm

  Status

    Complete & tested to high coverage

  API

    XBM.h

  Background

    X BitMap Format

      XBM is a file format of The X Window System used for cursor and
      icon bitmaps, as well as stipple masks.  

      XBM files have C programming language syntax, and may be included
      into C language header files as static programming resources.

      XBM files may be named with the dot suffix "xbm" (portable) or
      "bmp" (native), and may be communicated with the MIME type
      "image/x-xbitmap".

      FILE SYNTAX

        An XBM file contains two C preprocessor defines for width and
        height, and an array of pixel bits in C's array declaration
        syntax.

        The three primary symbols, width, height and bits have the
        conventional string format: <prefix> + '_' +
        ("width"|"height"|"bits").

        The <prefix> substring is a legal C identifier.  It is unique to
        the image and shared among all symbols within an image.

        An optional hotspot may be defined following width and height
        with a pair of symbols named: <prefix> + "_" + ('x'|'y') +
        "_hot".

        The pixel bits array is always declared static with type char,
        and should be declared unsigned.

        A C style (multiline) comment may be included before or after
        the image dimension and bits declarations.

      IMAGE DATA

        XBM image data is a horizontal raster array containing one bit
        pixel data packed into 8 bit pixel blocks (bytes).

        XBM pixel blocks are expressed in the usual horizontal raster
        order left to right, top to bottom.  The image width and height
        define the total image pixel count, and thereby the number of
        pixel blocks.  When the image width is not a multiple of the
        number of pixels in a block (bits in a byte), unused pixels are
        appended to each horizontal raster scan line for a unit number
        of blocks per horizontal line.  The number of blocks per line is
        width plus seven divided by eight ((width + 7)/8).  The number
        of blocks in the image is height multiplied by the number of
        blocks per line (height * ((width + 7)/8)).

        Within the XBM pixel block (byte) the order of pixels (bits) is
        inverted from raster order.  The most significant or left bit is
        the right pixel.  

        A lookup table is typically employed to map XBM bit order to
        raster pixel order, and vice versa.  This table is included at
        the end of this file.  

      EXAMPLE TEST

        For example, the following is an X BitMap file.

          #define test_width 16
          #define test_height 16
          static unsigned char test_bits[] = {
             0xff, 0xff, 0x01, 0x80, 0xfd, 0xbf, 0x05, 0xa0, 0xf5, 0xaf, 0x15, 0xa8,
             0xd5, 0xab, 0x55, 0xaa, 0x55, 0xaa, 0xd5, 0xab, 0x15, 0xa8, 0xf5, 0xaf,
             0x05, 0xa0, 0xfd, 0xbf, 0x01, 0x80, 0xff, 0xff};

        This example defines a bitmap that looks like the following.

            ################
            #              #
            # ############ #
            # #          # #
            # # ######## # #
            # # #      # # #
            # # # #### # # #
            # # # #  # # # # 
            # # # #  # # # #
            # # # #### # # #
            # # #      # # #
            # # ######## # #
            # #          # #
            # ############ #
            #              #
            ################

      EXAMPLE SMILEY

        Unix/Linux command line example.

          $ cat smiley.xbm 
          #define smiley_width 16
          #define smiley_height 16
          #define smiley_x_hot 8
          #define smiley_y_hot 7
          static unsigned char smiley_bits[] = {
             0xc0, 0x07, 0x30, 0x18, 0x08, 0x20, 0x04, 0x40, 0x44, 0x44, 0x02, 0x80,
             0x02, 0x80, 0x02, 0x80, 0x22, 0x88, 0x62, 0x8c, 0xc4, 0x47, 0x04, 0x40,
             0x08, 0x20, 0x30, 0x18, 0xc0, 0x07, 0x00, 0x00};
          $ bmtoa smiley.xbm 
          ------#####-----
          ----##-----##---
          ---#---------#--
          --#-----------#-
          --#---#---#---#-
          -#-------------#
          -#-------------#
          -#-------------#
          -#---#-----#---#
          -#---##---##---#
          --#---#####---#-
          --#-----------#-
          ---#---------#--
          ----##-----##---
          ------#####-----
          ----------------

      FOR MORE INFO

        Xlib-C language X Interface, James Gettys and Robert Scheiffler

        Xlib Programming Manual, Adrian Nye

