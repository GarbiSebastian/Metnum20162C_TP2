function [Z,l,W,r] = particionar(X,lab,particion)
[rows , cols] = size(X);
rowsTest = sum(sum(particion == 0));
rowsTrain = rows-rowsTest;
Z = zeros(rowsTrain,cols);
W = zeros(rowsTest,cols);
j=0;
k=0;
for i=1:size(particion,1)
  if particion(i)==1
    j=j+1;
    Z(j,1:cols) = X(i,:);
    l(j,1) = lab(i,1);
  else
    k=k+1;
    W(k,1:cols) = X(i,:);
    r(k,1) = lab(i,1);    
  end
end