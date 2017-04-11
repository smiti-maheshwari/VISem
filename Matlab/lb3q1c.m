I = imread('eight.tif');

subplot(231);
imshow(I)
title('original image');

subplot(232);
J = imnoise(I,'poisson');
imshow(J);
title('poisson');

subplot(233);
Kaverage = filter2(fspecial('average',3),J)/255;
imshow(Kaverage);
title('average filter');

subplot(234);
K = wiener2(J,[5 5]);
imshow(K);
title('weiner filter');

subplot(235);
Kmedian = medfilt2(J);
imshowpair(Kaverage,Kmedian,'montage');
title('median filter');




