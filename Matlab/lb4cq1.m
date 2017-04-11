clear all;

im1 = rand(512,512);
im2 = rand(512,512);
im1 = im2bw(im1);
im2 = im2bw(im2);

im3 = zeros(512,512);
for i=1:512
    for j=1:512
        im3(i,j) = (im1(i,j)+im2(i,j))/2;
    end
end

subplot(131);
imshow(im1);
title('1st image');

subplot(132);
imshow(im2);
title('2nd image');

subplot(133);
imshow(im3);
title('resultant image');

x = -1;

for i=1:512
    for j=1:512
        if (im3(i,j)>x)
            x = im3(i,j);
        end
    end
end

disp(x);