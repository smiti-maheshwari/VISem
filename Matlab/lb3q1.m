I = imread('eight.tif');

subplot(231);
imshow(I)
title('original image');

subplot(232);
J = imnoise(I,'salt & pepper',0.02);
imshow(J);
title('salt & pepper');

subplot(233);
J = imnoise(I,'gaussian');
imshow(J);
title('gaussian');

subplot(234);
J = imnoise(I,'poisson');
imshow(J);
title('poisson');

subplot(235);
J = imnoise(I,'speckle');
imshow(J);
title('speckle');