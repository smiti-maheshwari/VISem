img = imread('faceimages.jpg');
[height, width] = size(img);

bak = img;
subplot(1, 3, 1), imshow(img);
i = 1;
while i + 2 <= height
    j = 1;
    while j + 2 <= width      
        total = 0;
        for it = 0:1
            for jt = 0:1
                total = total + img(i + it, j + jt);
            end
        end
        avg = total / 4.0;
        for it = 0:1
            for jt = 0:1
                img(i+it, j+jt) = avg;
            end
        end
        j = j + 2;
    end
    i = i + 2;
end

subplot(1, 3, 2), imshow(mat2gray(img));

i = 1;
img = bak;
while i + 2 < height
    j = 1;
    while j + 2 < width 
        %display(1);
        arr = zeros(4);
        count = 1;
        for it = 0:1
            for jt = 0:1
                arr(count) = img(i + it, j + jt);
                count = count + 1;
            end
        end
        arr = sort(arr);
        median = (arr(2) + arr(3)) / 2.0;
        for it = 0:1
            for jt = 0:1
                img(i+it, j+jt) = median;
            end
        end
        j = j + 2;
    end
    i = i + 2;
end

subplot(1, 3, 3), imshow(mat2gray(img));
        
        
        

        
            
 

