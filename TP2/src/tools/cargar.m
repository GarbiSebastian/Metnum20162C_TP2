function [train,labels,fold] = cargar(tr,f)
fold = load(f);
X = load(tr);
labels = X(:,1);
train = X(:,2:785);
