im = imread('/home/placements2017/VISem/1.jpeg')

i1 = im2uint8(im)
i2 = im2uint16(im)
i3 = char(im)
i4 = im2single(im)
i5 = im2double(im)
i6 = num2str(im)

subplot(3,2,1);
subimage(rgb2gray(im))
title('original')

subplot(3,2,2);
subimage(rgb2gray(i1))
title('int8')

subplot(3,2,3);
subimage(rgb2gray(i2))
title('int16')

subplot(3,2,4);
subimage(rgb2gray(i4))
title('single')

subplot(3,2,5);
subimage(rgb2gray(i5))
title('double')

subplot(3,2,6);
subimage(rgb2gray(i6))
title('str')



