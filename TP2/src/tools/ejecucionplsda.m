Y = armarY(foldTrainLabels);
g=10;
[valPLSDA,vecPLSDA] = plsda(X,Y,g);
valPLSDA;
vecPLSDA
