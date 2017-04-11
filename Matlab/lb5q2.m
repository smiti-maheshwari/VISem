
clear all;
origimg = imread('Earth.jpg');
subplot(151);
imshow(origimg);
title('Original');


H = fspecial('laplacian');
blurred = imfilter(origimg,H,'replicate');
subplot(152);
imshow(blurred);
title('Laplacian Filter without Scaling');


subplot(153);
imshow(origimg+blurred);
title('Sharpened Image  without Scaling');


lap=fspecial('laplacian',0.6);
g1=imfilter(origimg,lap,'replicate');
subplot(154);
imshow(g1);
title('Laplacian Filter with Scaling');



subplot(155);
imshow(origimg+g1);
title('Sharpened with Scaling');