oi = imread('Image_2.bmp');
se = strel('line',11,90);
im = oi;
subplot(321);
imshow(oi);
title('original image');

for i=2:1:5
    im = imdilate(im,se);
    subplot(3,2,i);
    imshow(im);
    title(i-1);
end

subplot(326);
imshow(oi-im);
title('subtracted image');
