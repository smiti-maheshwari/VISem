A=imread('/home/placements2017/VISem/IVP/LAB/Image.jpg');


x1=zeros([size(A,1)*size(A,2) 1]);
x2=zeros([size(A,2)*size(A,1) 1]);

deg=45;

C=uint8(zeros([size(A,1) size(A,2) 3 ]));

m=1;

midx=ceil((size(C,1)+1)/2);
midy=ceil((size(C,2)+1)/2);

for i=1:size(A,1)
    i1=i-midx;
    for j=1:size(A,2)
      
        [t,r]=cart2pol(i1,j-midy);
       
        t1=radtodeg(t)+deg;
       
        t=degtorad(t1);
       
        [x,y]=pol2cart(t,r);
        x1(m)=round(x+midx);
        x2(m)=round(y+midy);
       
         
        m=m+1;
       
       
       
    end
   
end

x1(find(x1 < 1))=1;
x2(find(x2 < 1))=1;

n=1;
for i=1:size(A,1)
    for j=1:size(A,2)
        C(x1(n),x2(n),:)=A(i,j,:);
       
        n=n+1;
    end
   
end
imshow(C);