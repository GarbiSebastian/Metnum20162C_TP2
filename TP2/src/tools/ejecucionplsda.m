particion = fold(1,:)';
[foldTrain,foldTrainLabels,foldTest,foldTestLabels] = particionar(train,labels,particion);
X = foldTrain.-mean(foldTrain);
Y = armarY(foldTrainLabels);
g=10;
[val,vec] = plsda(X,Y,g);
val
