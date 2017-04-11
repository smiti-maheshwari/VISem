I = imread('eight.tif');

subplot(131);
imshow(I)
title('original image');

subplot(132);
J = imnoise(I,'gaussian');
imshow(J);
title('gaussian');

subplot(133);
K = wiener2(J,[5 5]);
imshow(K);
title('weiner filter');
