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
#include "xf_resize_config.h"

void axis2xfMat (xf::Mat<XF_8UC3, HEIGHT, WIDTH, NPC1> &_src, axis_t *src, int src_rows, int src_cols) {
#pragma HLS inline off

	for (int i=0; i<src_rows; i++) {
		for (int j=0; j<src_cols; j++) {
#pragma HLS pipeline
#pragma HLS loop_flatten off
			_src.data[i*src_cols+j] = src[i*src_cols+j].data;
		}	
	}

}

void xfMat2axis (xf::Mat<XF_8UC3, HEIGHT, WIDTH, NPC1> &_dst, axis_t *dst, int dst_rows, int dst_cols) {
#pragma HLS inline off

	for (int i=0; i<dst_rows; i++) {
		for (int j=0; j<dst_cols; j++) {
#pragma HLS pipeline
#pragma HLS loop_flatten off
			ap_uint<1> tmp = 0;
			if ((i==dst_rows-1) && (j== dst_cols-1)) {
				tmp = 1;
			}
			dst[i*dst_cols+j].last = tmp;
			dst[i*dst_cols+j].data = _dst.data[i*dst_cols+j];
		}
	}
}

void resize_accel (axis_t *src, axis_t *dst, int src_rows, int src_cols, int dst_rows, int dst_cols) {
	
#pragma HLS INTERFACE axis port=src depth=384*288 // Added depth for C/RTL cosimulation
#pragma HLS INTERFACE axis port=dst depth=192*144 // Added depth for C/RTL cosimulation
#pragma HLS INTERFACE s_axilite port=src_rows
#pragma HLS INTERFACE s_axilite port=src_cols
#pragma HLS INTERFACE s_axilite port=dst_rows
#pragma HLS INTERFACE s_axilite port=dst_cols
#pragma HLS INTERFACE s_axilite port=return
	
	// there are unused ones here
	xf::Mat<TYPE, HEIGHT, WIDTH, NPC1> _src(src_rows, src_cols);
	xf::Mat<TYPE, HEIGHT, WIDTH, NPC1> _dst(dst_rows, dst_cols);
	RGB_IMAGE  img_0(dst_rows, dst_cols);
	GRAY_IMAGE img_1(dst_rows, dst_cols);
	RGB_IMAGE  img_2(dst_rows, dst_cols);
	GRAY_IMAGE sobel_0(dst_rows, dst_cols);
	GRAY_IMAGE sobel_1(dst_rows, dst_cols);
	GRAY_IMAGE sobel_out(dst_rows, dst_cols);
	GRAY_IMAGE sobel_zero(dst_rows, dst_cols);
	GRAY_IMAGE gaussian_out(dst_rows, dst_cols);
	uchar_t threshold = 20;

#pragma HLS stream variable=_src.data depth=200
#pragma HLS stream variable=_dst.data depth=200
#pragma HLS stream variable=img_0.data depth=200
#pragma HLS stream variable=img_1.data depth=200
#pragma HLS stream variable=img_2.data depth=200
#pragma HLS stream variable=sobel_0.data depth=200
#pragma HLS stream variable=sobel_1.data depth=200
#pragma HLS stream variable=sobel_out.data depth=200
#pragma HLS stream variable=sobel_zero.data depth=200
#pragma HLS stream variable=gaussian_out.data depth=200

#pragma HLS dataflow
	axis2xfMat(_src, src, src_rows, src_cols);	
	// xf::resize <INTERPOLATION, XF_8UC3, HEIGHT, WIDTH, HEIGHT, WIDTH, NPC1, 2> (_src, _dst);    // not needed yet
	xf::bgr2gray <XF_8UC3,XF_8UC1, HEIGHT, WIDTH, NPC1> (_src, img_1);

	xf::GaussianBlur<5, XF_BORDER_CONSTANT, XF_8UC1, HEIGHT, WIDTH, NPC1>(img_1, gaussian_out, 2.5);

	xf::Sobel<XF_BORDER_CONSTANT, 5 , XF_8UC1, XF_8UC1, HEIGHT, WIDTH, NPC1, false>(gaussian_out, sobel_1, sobel_0);
	// xf::fast<NMS,XF_8UC1,HEIGHT,WIDTH,NPC1>(img_1, fasted, threshold);    // not okay yet

	xf::zero<XF_8UC1, HEIGHT, WIDTH, NPC1>(sobel_0, sobel_zero);
	xf::bitwise_or< XF_8UC1, HEIGHT, WIDTH, NPC1 >(sobel_zero, sobel_1, sobel_out);

	xf::gray2bgr <XF_8UC1,XF_8UC3, HEIGHT, WIDTH, NPC1> (sobel_out, img_2);
	xfMat2axis(img_2, dst, dst_rows, dst_cols);	

}

