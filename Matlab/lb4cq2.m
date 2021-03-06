clear all;

im1 = imread('img1.jpg');
im2 = imread('img2.jpg');
im3 = imread('img3.jpg');
im4 = imread('img4.jpg');
im1 = rgb2gray(im1);
imhist(im1);
GIm=im1;


%function [x, freq] = drawhist(img)

freq = zeros(255);
x = zeros(255);

[w, h] = size(img);

for i = 1:w
    for j = 1:h
        freq(img(i, j)) = freq(img(i, j)) + 1;    
    end
end

for i = 1: 255
    x(i) = i;
end



%yooooo
numofpixels=size(GIm,1)*size(GIm,2);


figure,imshow(GIm);

title('Original Image');

HIm=uint8(zeros(size(GIm,1),size(GIm,2)));

freq=zeros(256,1);

probf=zeros(256,1);

probc=zeros(256,1);

cum=zeros(256,1);

output=zeros(256,1);

for i=1:size(GIm,1)

    for j=1:size(GIm,2)

        value=GIm(i,j);

        freq(value+1)=freq(value+1)+1;

        probf(value+1)=freq(value+1)/numofpixels;

    end

end


sum=0;

no_bins=255;

for i=1:size(probf)

   sum=sum+freq(i);

   cum(i)=sum;

   probc(i)=cum(i)/numofpixels;

   output(i)=round(probc(i)*no_bins);

end

for i=1:size(GIm,1)

    for j=1:size(GIm,2)

            HIm(i,j)=output(GIm(i,j)+1);

    end

end

figure,imshow(HIm);

title('Histogram equalization');

IIm = GIm * 1.2;

figure,imshow(IIm);

title('3rd image');