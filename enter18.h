/*  GIMP header image file format (INDEXED): /home/guu/Pictures/enter.h  */

static unsigned int enter_width = 18;
static unsigned int enter_height = 18;

/*  Call this macro repeatedly.  After each use, the pixel data can be extracted  */

#define ENTER_PIXEL(data,pixel) {\
pixel[0] = enter_header_data_cmap[(unsigned char)data[0]][0]; \
pixel[1] = enter_header_data_cmap[(unsigned char)data[0]][1]; \
pixel[2] = enter_header_data_cmap[(unsigned char)data[0]][2]; \
data ++; }

static unsigned char enter_header_data_cmap[3][3] = {
	{127,127,127},
	{247,247,247},
	{  0,  0,  0}
	};
static unsigned char enter_header_data[] = {
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,
	2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	2,2,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,2,
	0,1,1,1,1,0,1,1,0,1,0,0,1,1,1,0,
	0,2,
	0,1,0,0,0,0,1,1,0,1,0,0,0,1,0,0,
	0,2,
	0,1,1,1,1,0,1,1,1,1,0,0,0,1,0,0,
	0,2,
	0,1,0,0,0,0,1,0,1,1,0,0,0,1,0,0,
	0,2,
	0,1,1,1,1,0,1,0,1,1,0,0,0,1,0,0,
	0,2,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,2,
	2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2
	};
