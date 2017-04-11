clc;
clear all;

I = imread('faceimages.jpg');

imcrop(I,[ 117   155    59    35])
title('Lips')

imcrop(I,[65.563829787234 54.5997628614716 73.8816427511133 49.178183255385])
title('eye left')

imcrop(I,[149.467303240741 54.7196207430341 70.4311342592593 47.0994582043344])
title('eye right')


imcrop(I,[103.4765625 83.7196207430341 89.4218750000001 59.7991486068112]);
title('nose')

imcrop(I,[114.287326388889 145.237229102167 66.1892361111111 44.2627708978328]);
title('mouth')