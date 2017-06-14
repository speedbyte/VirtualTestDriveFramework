/*
 * Resolution.h
 *
 *  Created on: Sep 29, 2016
 *      Author: geislerd
 */

#ifndef RESOLUTION_H_
#define RESOLUTION_H_

// high definition (hd) resolution
#define RES_NHD             640 , 360
#define RES_QHD             960 , 540
#define RES_HD              1280, 720
#define RES_FHD             1920,1080
#define RES_DCI_2K          2048,1080
#define RES_WQHD            2560,1440
#define RES_UWQHD           3440,1440
#define RES_QHD_PLUS        3200,1800
#define RES_4K_UHD          3840,2160
#define RES_DCI_4K          4096,2160
#define RES_5K_UHD_PLUS     5120,2880
#define RES_8K_UHD          7680,4320

// video graphics array (vga) resolution
#define RES_QQVGA            160, 120
#define RES_HQVGA            240, 160
#define RES_QVGA             320, 240
#define RES_WQVGA            400, 240
#define RES_HVGA             480, 320
#define RES_VGA              640, 480
#define RES_WVGA             768, 480
#define RES_FWVGA            854, 480
#define RES_SVGA             800, 600
#define RES_DVGA             960, 640
#define RES_WSVGA           1024, 576

// extended graphics array (xga) resolution
#define RES_XGA             1024,768
#define RES_WXGA            1366,768
#define RES_XGA_PLUS        1152,864
#define RES_WXGA_PLUS       1440,900
#define RES_SXGA            1280,1024
#define RES_SXGA_PLUS       1400,1050
#define RES_WSXGA_PLUS      1680,1050
#define RES_UXGA            1600,1200
#define RES_WUXGA           1920,1200

// quad extended graphics array (qxga) resolution
#define RES_QWXGA           2048,1152
#define RES_QXGA            2048,1536
#define RES_WQXGA           2560,1600
#define RES_QSXGA           2560,2048
#define RES_WQSXGA          3200,2048
#define RES_QUXGA           3200,2400
#define RES_WQUXGA          3840,2400

// hyper extended graphics array (hxga) resolution
#define RES_HXGA            4096,3072
#define RES_WHXGA           5120,3200
#define RES_HSXGA           5120,4096
#define RES_WHSXGA          6400,4096
#define RES_HUXGA           6400,4800
#define RES_WHUXGA          7680,4800

#define RES_WIDTH_I( width , height ) width
#define RES_HEIGHT_I( width , height ) height
#define RES_WIDTH( RES ) RES_WIDTH_I( RES )
#define RES_HEIGHT( RES ) RES_HEIGHT_I( RES )
#define RES_WIDTH_I_S( width , height ) #width
#define RES_HEIGHT_I_S( width , height ) #height
#define RES_WIDTH_S( RES ) RES_WIDTH_I_S( RES )
#define RES_HEIGHT_S( RES ) RES_HEIGHT_I_S( RES )

#endif /* RESOLUTION_H_ */
