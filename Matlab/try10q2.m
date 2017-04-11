I = imread('Image_4.bmp'); %read image
I = imresize(I, [256 256]); %resize image
[m,n] = size(I);%get size of image as m and n
[X,Y]=meshgrid(1:256,1:256); % it is a meshgrid for circle mask
filter=ones(m,n); % filter initially only ones in it
%according to notch filter equation it will find point on image is on    imaginary circle.i found circle coordinates.
for i=1:m-1
  for j=1:n-1
  d0 = (i-130)^2 + (j-130)^2 <= 32^2 && (i-130)^2 + (j-130)^2 >=20^2; 
      if d0
         filter(i,j)=0;
     else
         filter(i,j)=1;
     end
   end
 end
f = fftshift(fft2(I));
G = abs(ifft2(f.*filter));
figure(1),imshow(I);
figure(2),imshow(G,[]);