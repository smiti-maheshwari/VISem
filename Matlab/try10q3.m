I = imread('Image_4.bmp');
%I = rgb2gray(I);
%I = double(mat2gray(I));
I = imresize(I, [256 256]);

[rows,cols] = size(I);

f = fftshift(fft2(I));

fLog = log(1 + abs(f));

filter = (fLog > 1*max(fLog(:)) ) | (fLog < 0.08*max(fLog(:)) );

B = abs(ifft2(f.*filter));

colormap(gray)
subplot(2,2,1),imagesc(I); title('Original Image')
subplot(2,2,2),imagesc(fLog); title('Fourier Image')
subplot(2,2,3),imagesc(filter); title('Zeroed Fourier Image')
subplot(2,2,4),imshow(B); title('Cleaned Image')
annotation('textbox', [0 0.9 1 0.1], ...
    'String', 'Fourier Analysis on Clown Image', ...
    'EdgeColor', 'none', ...
    'HorizontalAlignment', 'center', ...
    'FontSize', 15, ...
    'FontWeight', 'bold')