close all; clear all;

img   = imread('Image_4.bmp');
imagesc(img)
F     = fft2(img);
img   = fftshift(F);

figure;

imagesc(100*log(1+abs(fftshift(F)))); colormap(gray); 
title('magnitude spectrum');

figure;
imagesc(angle(F));  colormap(gray);
title('phase spectrum');