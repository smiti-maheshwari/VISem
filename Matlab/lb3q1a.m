I = imread('eight.tif');

subplot(221);
imshow(I)
title('original image');

subplot(222);
J = imnoise(I,'salt & pepper',0.02);
imshow(J);
title('salt & pepper');

subplot(223);
Kaverage = filter2(fspecial('average',3),J)/255;
imshow(Kaverage);
title('average filter');

subplot(224);
Kmedian = medfilt2(J);
imshowpair(Kaverage,Kmedian,'montage');
title('median filter');
