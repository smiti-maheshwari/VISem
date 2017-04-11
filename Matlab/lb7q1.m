img = imread('peppers256.png');
subplot(251);
imshow(img);
title('256*256 img');

BW = edge(img, 'sobel');
subplot(252);
imshow(BW);
title('sobel');

BW = edge(img, 'Prewitt');
subplot(253);
imshow(BW);
title('Prewitt');

BW = edge(img, 'log');
subplot(254);
imshow(BW);
title('log');

BW = edge(img, 'Canny');
subplot(255);
imshow(BW);
title('Canny');

img = imread('girl512.jpg');
subplot(256);
imshow(img);
title('512*512 img');
img = rgb2gray(img);
BW = edge(img, 'sobel');
subplot(257);
imshow(BW);
title('sobel');

BW = edge(img, 'Prewitt');
subplot(258);
imshow(BW);
title('Prewitt');

BW = edge(img, 'log');
subplot(259);
imshow(BW);
title('log');

BW = edge(img, 'Canny');
subplot(2,5,10);
imshow(BW);
title('Canny');