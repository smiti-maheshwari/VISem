oi = imread('Image_1.bmp');
se = strel('line',11,90);
im = oi;
subplot(421);
imshow(oi);
title('original image');

for i=2:1:7
    im = imerode(im,se);
    subplot(4,2,i);
    imshow(im);
    title(i-1);
end
subplot(428);
imshow(oi-im);
title('subtracted image');