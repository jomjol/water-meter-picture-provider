
# ESP32-MOD Lens Focal-Length Modification

In order to use it for reading a meter, the focal-length of the OV2640 camera has to be manipulated, as by default it only results in sharp image for distance bigger than ~40cm, causing the image of the water meter to be too small for automated readout processing.

**ATTENTION:** this modification will void any warranty, as the sealing of the lens objective is broken!

**ATTENTION:** This modification will render the camera unsuitable for general, web-cam type applications unless the focal length is changed back to the original setting.

I removed the fixing glue of the OV2640 lens with a sharp knife [see Picture](./images/Lens_Fixing_Glue.jpg). After this I could screw the lens in and out. Rotating it by about a quarter of a turn counterclockwise results in a focus plane of about 10cm. You need to figure out your best setting with a little bit of trial and error for your specific environment.

