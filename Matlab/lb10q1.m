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