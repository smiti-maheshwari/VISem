img1 = imread('img1.JPG');
img2 = imread('img2.JPG');
img3 = imread('img3.JPG');
img4 = imread('img4.JPG');

[x1 freq1] = drawhist(rgb2gray(img1));
figure, plot(x1, freq1);
%[x2 freq2] = drawhist(img2);
figure, imhist(rgb2gray(img2));
[x3 freq3] = drawhist(rgb2gray(img3));
figure, plot(x3, freq3);
%[x4 freq4] = drawhist(rgb2gray(img4));
figure,imhist(rgb2gray(img4));

eqim1 = equaliz(img1);
eqim2 = equaliz(img2);
eqim3 = equaliz(img3);
eqim4 = equaliz(img4);

figure, subplot(2, 2, 1), imshow(eqim1);
subplot(2, 2, 2), imshow(eqim2);
subplot(2, 2, 3), imshow(eqim3);
subplot(2, 2, 4), imshow(eqim4);

figure, subplot(2, 2, 1), imhist(eqim1);
subplot(2, 2, 2), imhist(eqim2);
 subplot(2, 2, 3), imhist(eqim3);
subplot(2, 2, 4), imhist(eqim4);
%[x5 freq5] = drawhist(rgb2gray(equim1));
%figure, plot(x5, freq5);
%figure,imhist(rgb2gray(euqim1));
