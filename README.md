## This branch is a modification of the original PYNQ-HelloWorld

I created a simple image processing pipeline, consisting of the following xfOpenCV functions (nearly) in order:
* bgr2gray
* GaussianBlur
* Sobel
* zero
* bitwise_or
* gray2bgr
The original _resize_accel_ function is still used as the top function for ease of usage, tho it is no longer doing resizing.
The parameter variables are left as they are, the might be useful for transmitting simple information to the IP.



## Original PYNQ-Helloworld Description as it is

This repository contains a "Hello World" introduction application to the Xilinx PYNQ framework. 

The design illustrates how to run a resizer IP to resize an image on the FPGA. There are two notebooks that illustrate the resize operation. One notebook shows the image resizing done purely in software using Python Image Library. The second notebook shows the resize operation being performed in the programmable logic using a resizer IP from the  Xilinx xfopencv library. 
https://github.com/Xilinx/xfopencv/tree/master/examples/resize   



## Quick Start

To install v2.5 specifically, open a terminal on your PYNQ board and run:

```
sudo pip3 install --upgrade git+https://github.com/xilinx/pynq-helloworld.git@v2.5
```

Currently this repository is compatible with [PYNQ image v2.4](http://www.pynq.io/board).



## License

**PYNQ** License : [BSD 3-Clause License](https://github.com/Xilinx/PYNQ/blob/master/LICENSE)
