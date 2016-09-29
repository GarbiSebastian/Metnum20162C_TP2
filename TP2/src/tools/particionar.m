function [Z,l] = particionar(X,lab,particion)
cols = size(X,2);
j=0;
for i=1:size(particion,1)
  if particion(i)==1
    j=j+1
    Z(j,1:cols) = X(i,:);
    l(j,1:cols) = lab(i,:);
  end
end