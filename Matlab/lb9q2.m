im = imread('Image_2.bmp');
subplot(131);
imshow(im);
title('original image');

bw = bwmorph(im,'open');
subplot(132);
imshow(bw);
title('erosion followed by dilation');

bw = bwmorph(bw,'close');
subplot(133);
imshow(bw);
title('dilation followed by erosion');

