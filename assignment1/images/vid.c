int volatile *fb;
int width = 640; // default to VGA mode for 640x480

#define WIDTH 640

int fbuf_init ()
{
    fb = (int *)(0x200000); // at 2 MB to 4 MB
    //***************** for 640x480 VGA ****************/
    *(volatile unsigned int *)(0x1000001c) = 0x2C77;
    *(volatile unsigned int *)(0x10120000) = 0x3F1F3F9C;
    *(volatile unsigned int *)(0x10120004) = 0x090B61DF;
    *(volatile unsigned int *)(0x10120008) = 0x067F1800;
    /***************** for 800X600 SVGA ****************
    *(volatile unsigned int *)(0x1000001c) = 0x2CAC;
    *(volatile unsigned int *)(0x10120000) = 0x1313A4C4;
    *(volatile unsigned int *)(0x10120004) = 0x0505F6F7;
    *(volatile unsigned int *)(0x10120008) = 0x071F1800;
    }
    ****************************************************/
    *(volatile unsigned int *)(0x10120010) = 0x200000; // fbuf
    *(volatile unsigned int *)(0x10120018) = 0x82B;
}


// KC's copied code, this is disgusting
int show_bmp(char *p, int start_row, int start_col)
{
    int h, w, pixel, rsize, i, j;
    unsigned char r, g, b;
    char *pp;
    int *q = (int *)(p+14); // skip over 14-byte file header
    w = *(q+1);
    h = *(q+2); // image width in pixels
    // image height in pixels
    p += 54; // p-> pixels in image
    //BMP images are upside down, each row is a multiple of 4 bytes
    rsize = 4*((3*w + 3)/4); // multiple of 4
    p += (h-1)*rsize; // last row of pixels
    for (i=start_row; i<start_row + h; i++)
    {
        pp = p;
        for (j=start_col; j<start_col + w; j++)
        {
            b = *pp; g = *(pp+1); r = *(pp+2); // BRG values
            pixel = (b<<16) | (g<<8) | r;
            // pixel value
            fb[i*WIDTH + j] = pixel; // write to frame buffer
            pp += 3;
        }
        p -= rsize;
        // advance pp to next pixel
        // to preceding row
    }
    uprintf("\nBMP image height=%d width=%d\n", h, w);
}
