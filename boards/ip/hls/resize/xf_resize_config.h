/***************************************************************************
Copyright (c) 2016, Xilinx, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors 
may be used to endorse or promote products derived from this software 
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***************************************************************************/
#ifndef _XF_RESIZE_CONFIG_
#define _XF_RESIZE_CONFIG_

#include <iostream>
#include <math.h> 
#include "hls_stream.h"
#include "ap_int.h"
#include "common/xf_common.h"
#include "xf_config_params.h"
#include "imgproc/xf_resize.hpp"
#include "imgproc/xf_cvt_color.hpp"
#include "imgproc/xf_cvt_color_1.hpp"
#include "imgproc/xf_rgb2hsv.hpp"
#include "imgproc/xf_bgr2hsv.hpp"
#include "features/xf_fast.hpp"
#include "common/xf_utility.h"
#include "imgproc/xf_sobel.hpp"
#include "core/xf_arithm.hpp"
#include "imgproc/xf_gaussian_filter.hpp"

/* Interface types*/
#if RO
#define NPC1 XF_NPPC8
#else
#define NPC1 XF_NPPC1
#endif

/*
#if GRAY
#define TYPE XF_8UC1
#endif
#if RGBA
#define TYPE XF_8UC3
#endif
*/

#define FILTER_SIZE_3 1
#define FILTER_SIZE_5 0
#define FILTER_SIZE_7 0

#define MAXCORNERS  1024
#define NMS 1

struct axis_t {
    ap_uint<24> data;
    ap_int<1> last;
};

typedef xf::Mat<XF_8UC3, HEIGHT, WIDTH, NPC1> RGB_IMAGE;
typedef xf::Mat<XF_8UC1, HEIGHT, WIDTH, NPC1> GRAY_IMAGE;

void resize_accel (axis_t *src, axis_t *dst, int src_rows, int src_cols, int dst_rows, int dst_cols);


#endif
