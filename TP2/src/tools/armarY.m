function Y = armarY(labels)
l2 = labels+1;
rows = size(labels,1);
Y = zeros(rows,10);
for i=1:rows
  Y(i, l2(i))=1;
end