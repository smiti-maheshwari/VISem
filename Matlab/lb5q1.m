%Blur Demo

%Import image
origimg = imread('Earth','jpg');

%show original image
subplot(221);
imshow(origimg);
title('original');

%blurred image
H = fspecial('disk',10);
blurred = imfilter(origimg,H,'replicate');

%plot blurred image
subplot(222);
imshow(blurred);
title('blurred');

mask = blurred - origimg;
subplot(223);
imshow(mask);
title('mask');

newimg = origimg + mask;
subplot(224);
imshow(newimg);
title('added mask');




