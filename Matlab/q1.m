img1 = rgb2gray(imread('img1.jpg'));
img2 = rgb2gray(imread('img2.jpg'));


[w1, h1] = size(img1);
[w2, h2] = size(img2);

width = min(w1, w2);
height = min(h1, h2);

result = zeros(width, height);
maxi = 0;
mini = 1000;
for i = 1:width
    for j = 1:height
        result(i, j) = (img1(i, j) + img2(i, j)) / 2.0;
        maxi = max(maxi, result(i, j));
        mini = min(mini, result(i, j));
    end
end

result = mat2gray(result);
display(maxi);
display(mini);
subplot(3, 1, 1), imshow(img1);
subplot(3, 1, 2), imshow(img2);
subplot(3, 1, 3), imshow(result);
