function [train,labels,fold,X] = cargar(train_csv,foldFile)
fold = dlmread(foldFile,' ',1,0);
fold(:,end) = [];
X = dlmread(train_csv,',',1,0);
labels = X(:,1);
train = X(:,2:end);
