clear all;
im1 = imread('faceimages.jpg');
im1 = im2bw(im1);
[m,n] = size(im1);
im2 = imread('faceimages.jpg');
im2 = im2bw(im2);
im3 = zeros(m,n);
for i = 1:m
    for j = 1:n
        im3(i,j) = im1(i,j)+im2(i,j);
        im3(i,j) = im3(i,m)/2;
    end
end
imshow(im3);