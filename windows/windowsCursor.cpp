/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#if 0

// to store colors
struct COLOR {
   uint8_t r,g,b,a;
   COLOR() {
      this->r = this->g = this->b = this->a = 0;
   }
   COLOR(uint8_t r,uint8_t g,uint8_t b) {
      this->r = r;
      this->g = g;
      this->b = b;
      this->a = 255;
   }
   bool operator == (COLOR c) const {
      return (r == c.r && g == c.g && b == c.b);
   }
};

size_t get_bit(int32_t number,int8_t position) {
   size_t bitmask = 1 << position;
   return (number & bitmask) ? 1 : 0;
}

....
// load cursor
#if defined(_WIN32)
   // etc. use standard WinApi code, (HCURSOR)LoadImage(..) and such
#endif
#if defined(__linux__)

  Display* display = XOpenDisplay(NULL);

  string filename = "mycursor.cur";    // <-- add your .cur cursor here
  char buf[4];
  FILE* fp = fopen(filename.c_str(),"rb");
  fread(buf,1,2,fp);    // reserved. always 0
  fread(buf,1,2,fp);    // image type; we're only interested for .cur (=2)
  int16_t imagetype = buf[0] | (buf[1]<<8);
  if (imagetype!=2) {
     // error:  file is not a valid .cur file
     return;
  }
  fread(buf,1,2,fp);    // number of images
  // we're only interested in the first image
  fread(buf,1,1,fp);    // width  (0 to 255; 0=means 256 width)
  int8_t width = (buf[0]==0 ? 256 : buf[0]);
  fread(buf,1,1,fp);    // height (0 to 255; 0=means 256 height)
  int8_t height = (buf[0]==0 ? 256 : buf[0]);
  fread(buf,1,1,fp);    // number of colors in palette (0 for no palette)
  fread(buf,1,1,fp);    // reserved. should be 0
  fread(buf,1,2,fp);    // hotspot x
  int16_t hotspot_x = buf[0] | (buf[1]<<8);
  fread(buf,1,2,fp);    // hotspot y
  int16_t hotspot_y = buf[0] | (buf[1]<<8);
  fread(buf,1,4,fp);    // image data in bytes
  fread(buf,1,4,fp);    // offset to image data

  // Now we need to verify if image in .cur is BMP or PNG (Vista+)
  // We can't just check 'magic' since if it's BMP, the header will be missing (PNG is whole)
  // So we search for PNG magic; if doesnt exist, we have a BMP!
  // NOTE:  for simplicity we go only for BMP for the moment.
  //        So just check if 'biSize' is 40 (Windows NT & 3.1x or later)

  // BITMAPINFOHEADER
  fread(buf,1,4,fp);    // biSize
  int32_t biSize = (buf[0]&0xff) | (buf[1]<<8) | (buf[2]<<16) | (buf[3]<<24);
  if (biSize!=40) {
     // error:  file does not contain valid BMP data;
     return;
  }
  fread(buf,1,4,fp);    // biWidth
  int32_t biWidth = (buf[0]&0xff) | (buf[1]<<8) | (buf[2]<<16) | (buf[3]<<24);
  fread(buf,1,4,fp);    // biHeight (if positive => bottom-up, if negative => up-bottom)
  int32_t biHeight = (buf[0]&0xff) | (buf[1]<<8) | (buf[2]<<16) | (buf[3]<<24);
  fread(buf,1,2,fp);    // biPlanes
  fread(buf,1,2,fp);    // biBitCount
  int16_t biBitCount = (buf[0]&0xff) | (buf[1]<<8) | (buf[2]<<16) | (buf[3]<<24);
  if (biBitCount!=24 && biBitCount!=32) {
     // error:  only 24/32 bits supported;
     return;
  }
  fread(buf,1,4,fp);    // biCompression
  int32_t biCompression = (buf[0]&0xff) | (buf[1]<<8) | (buf[2]<<16) | (buf[3]<<24);
  // we want only uncompressed BMP data
  if (biCompression!=0 /*BI_RGB*/ ) {
     // error:  file is compressed; only uncompressed BMP is supported;
     return;
  }
  fread(buf,1,4,fp);    // biSizeImage
  fread(buf,1,4,fp);    // biXPelsPerMeter
  fread(buf,1,4,fp);    // biYPelsPerMeter
  fread(buf,1,4,fp);    // biClrUsed
  fread(buf,1,4,fp);    // biClrImportant

  // DECODE IMAGE
  uint8_t origin = (biHeight>0 ? 0 : 1);    // 0=bottom-up, 1=up-bottom
  // there are cases where BMP sizes are NOT the same with cursor; we use the cursor ones
  biWidth = width;
  biHeight = height;

  COLOR* pixels = new COLOR[biWidth * biHeight];
  for(int32_t y=0;y<biHeight;y++) {
     for(int32_t x=0;x<biWidth;x++) {
        uint32_t offset = ((origin==1?y:biHeight-1-y)*biWidth)+x;
        // read pixels by number of bits
        switch(biBitCount) {
           // 24-bit
           case 24:
              fread(buf,1,3,fp);
              pixels[offset] = COLOR(buf[0],buf[1],buf[2]);
           break;
           // 32-bit
           case 32:
              fread(buf,1,4,fp);
              pixels[offset] = COLOR(buf[0],buf[1],buf[2]);
              pixels[offset].a = buf[3];
           break;
        }
     }
  }

  // read mask
  // mask is 1-bit-per-pixel for describing the cursor bitmap's opaque and transparent pixels.
  // so for 1 pixel we need 1 bit, for etc. 32 pixels width, we need 32*1 bits (or 4 bytes per line)
  // so for etc. 32 pixels height we need 4*32 = 128 bytes or [mask bytes per line * height]
  uint16_t mask_bytes_per_line = biWidth / 8;
  uint16_t mask_bytes = mask_bytes_per_line * biHeight;
  char* mask = new char[mask_bytes];

  fread(mask,1,mask_bytes,fp);
  fclose(fp);

  // reverse every [mask_bytes_per_line] bytes; (etc. for 4 bytes per line do:  0,1,2,3 => 3,2,1,0) -> you really need to ask Microsoft for this 8)
  char rmask[4];
  for(uint16_t x=0;x<mask_bytes;x+=mask_bytes_per_line) {
     for(uint16_t r=0;r<mask_bytes_per_line;r++) {
        rmask[r] = mask[x+mask_bytes_per_line-1-r];
     }
     // copy the reversed line
     for(uint16_t r=0;r<mask_bytes_per_line;r++) {
        mask[x+r] = rmask[r];
     }
  }

  // now extract all bits from mask bytes into new array (equal to width*height)
  vector<uint8_t> bits;
  for(uint16_t x=0;x<mask_bytes;x++) {
     for(uint8_t b=0;b<8;b++) {
        bits.push_back( get_bit(mask[x],b) );
     }
  }
  delete[] mask;
  // reverse vector (?)
  reverse(bits.begin(),bits.end());

  // now the bits contains =1 (transparent) and =0 (opaque) which we use on cursor directly
  XcursorImage* cimg = XcursorImageCreate(biWidth,biHeight);
  cimg->xhot = hotspot_x;
  cimg->yhot = hotspot_y;
  // create the image
  for(int32_t y=0;y<biHeight;y++) {
     for(int32_t x=0;x<biWidth;x++) {
        uint32_t offset = (y*biWidth)+x;
        COLOR pix = pixels[offset];
        cimg->pixels[offset] = ((bits[offset]==1?0:pix.a)<<24) + (pix.r<<16) + (pix.g<<8) + (pix.b);
     }
  }
  // create cursor from image and release the image
  Cursor cursor = XcursorImageLoadCursor(display,cimg);
  XcursorImageDestroy(cimg);

  ...
  // set the cursor
  XDefineCursor(display,yourwindow,cursor);
  XFlush(display);

  ...
  // free cursor
  if (cursor!=None) {
     XFreeCursor(display,cursor);
  }
#endif

#endif
