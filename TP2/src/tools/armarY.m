function Y = armarY(labels)
l2 = labels+1;
n = size(labels,1);
Y = -1*ones(n,10);
for i=1:n
  Y(i, l2(i))=1;
end
Y = Y.-mean(Y);
Y = Y/sqrt(n-1);