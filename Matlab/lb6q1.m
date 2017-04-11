img = imread('faceimages.jpg')
img = rgb2gray(img);
[m,n] = size(img);
imglsb = zeros(m,n);
imgmsb = zeros(m,n);

for i = 1:1:m
    for j = 1:1:n
        if mod(img(i,j), 2) ==0
            imglsb(i,j) = 0;
        else
            imglsb(i,j) = 1;
        end
        
        if img(i,j)/128 ==0
            imgmsb(i,j) = 0;
        else
            imgmsb(i,j) = 128;
        end
    end
end

subplot(321);
imshow(img);
title('original image');

subplot(323);
imshow(imglsb);
title('lsb image');

subplot(325);
imshow(imgmsb);
title('msb image');

arr2=zeros(256);
for i=1:m
    for j=1:n
        arr2(imglsb(i,j)+1)=arr2(imglsb(i,j)+1)+1;
    end
end
subplot(3,2,4);
bar(arr2);

arr=zeros(256);
for i=1:m
    for j=1:n
        arr(imgmsb(i,j)+1)=arr(imgmsb(i,j)+1)+1;
    end
end
subplot(3,2,6)
bar(arr);

