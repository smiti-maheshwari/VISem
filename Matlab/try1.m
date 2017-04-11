%img = imread('faceimages.jpg');
img = [2 3 4 5 ; 2 3 4 5 ; 1 2 3 4 ;1 2 3 4]
%img = rgb2gray(img);
[m,n] = size(img);
%disp(m);
%disp(n);
%imshow(img);
disp(img);
for i=1:2:m
    for j = 1:2:n
        k = i;
        l = j;
        sum = 0;
        %disp(i);
        while (k<2+i &k<m)
            while (l<2+j &l<n)
               disp (k);
                sum = sum + img(k,l);
                k = k+1;
                l = l+1;
            end
        end
        %disp(sum);
        sum = sum/4;
        while (k<2+i && k<m)
            while (l<2+j && l<n)
                 img(k,l) = sum;
                 k = k+1;
                 l = l+1;
            end
        end
    end
end
%disp(img);
        