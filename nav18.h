/*  GIMP header image file format (INDEXED): /home/guu/Pictures/nav18.h  */

static unsigned int nav_width = 18;
static unsigned int nav_height = 18;

/*  Call this macro repeatedly.  After each use, the pixel data can be extracted  */

#define NAV_PIXEL(data,pixel) {\
pixel[0] = nav_header_data_cmap[(unsigned char)data[0]][0]; \
pixel[1] = nav_header_data_cmap[(unsigned char)data[0]][1]; \
pixel[2] = nav_header_data_cmap[(unsigned char)data[0]][2]; \
data ++; }

static unsigned char nav_header_data_cmap[3][3] = {
	{129,129,129},
	{247,247,247},
	{  0,  0,  0}
	};
static unsigned char nav_header_data[] = {
	2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,
	2,2,
	2,2,2,2,2,0,0,0,1,0,0,0,2,2,2,2,
	2,2,
	2,2,2,2,2,0,0,1,1,1,0,0,2,2,2,2,
	2,2,
	2,2,2,2,2,0,0,1,1,1,0,0,2,2,2,2,
	2,2,
	2,2,2,2,2,0,0,0,0,0,0,0,2,2,2,2,
	2,2,
	2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	2,2,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,2,
	0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,
	0,2,
	0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,
	0,2,
	0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,
	0,2,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,2,
	2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	2,2,
	2,2,2,2,2,0,0,0,0,0,0,0,2,2,2,2,
	2,2,
	2,2,2,2,2,0,0,1,1,1,0,0,2,2,2,2,
	2,2,
	2,2,2,2,2,0,0,1,1,1,0,0,2,2,2,2,
	2,2,
	2,2,2,2,2,0,0,0,1,0,0,0,2,2,2,2,
	2,2,
	2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,
	2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2
	};
