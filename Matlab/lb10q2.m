grayImage = imread('Image_4.bmp');
[rows columns numberOfColorChannels] = size(grayImage)
if numberOfColorChannels > 1
	grayImage = rgb2gray(grayImage);
end

% Display original grayscale image.
subplot(2, 2, 1);
imshow(grayImage)
title('Original Gray Scale Image')

% Perform 2D FFTs
fftOriginal = fft2(double(grayImage));
shiftedFFT = fftshift(fftOriginal);
subplot(2, 2, 2);
scaledFFTr = 255 * mat2gray(real(shiftedFFT));
imshow(log(scaledFFTr), []);
title('Log of Real Part of Spectrum')
subplot(2, 2, 3);
scaledFFTi = mat2gray(imag(shiftedFFT));
imshow(log(scaledFFTi), []);
title('Log of Imaginary Part of Spectrum')

% Display magnitude and phase of 2D FFTs
subplot(2, 2, 4);
imshow(log(abs(shiftedFFT)),[]);
colormap gray
title('Log Magnitude of Spectrum')
% Enlarge figure to full screen.
set(gcf, 'units','normalized','outerposition',[0 0 1 1]);

figure;
rectWidth = 10;
rectHeight = 5;
kernel = ones(rectHeight, rectWidth) / (rectHeight * rectWidth);
% Display it
subplot(2, 2, 1);
k = padarray(kernel, [3, 3]); % Just for display.
imshow(k, []);
axis on;
title('Kernel')
% Enlarge figure to full screen.
set(gcf, 'units','normalized','outerposition',[0 0 1 1]);

% Convolve kernel (box filter) with the image
filteredImage = conv2(double(grayImage), kernel, 'same');
% Display filtered image.
subplot(2, 2, 2);
imshow(filteredImage,[]);
title('Filtered Image')

% Perform 2D FFT on the filtered image to see its spectrum.
% We expect to see a sinc multiplication effect.
% It should look like the original but with a sinc pattern overlaid on it.
fftFiltered = fft2(double(filteredImage));
shiftedFFT = fftshift(fftFiltered);

% Display magnitude of the 2D FFT of the filtered image.
subplot(2, 2, 3);
imshow(log(abs(shiftedFFT)),[]);
colormap gray
title('Log Magnitude of Spectrum - Note sinc multiplication')