particion = logical(fold(1,:)');
foldTrain = train(particion,:);
foldTrainLabels = labels(particion,:);
foldTest = train(not(particion),:);
foldTestLabels = labels(not(particion),:);
n = size(foldTrain,1);
X = foldTrain.-mean(foldTrain);
X = X/sqrt(n-1);