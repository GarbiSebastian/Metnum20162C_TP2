conffile = '../salida.out.cosas';
conf = dlmread(conffile,' ',0,0);
reales = conf(:,1);
predicto = conf(:,2);
[C, order] = confusionmat(reales,predicto,'order',[0 1 2 3 4 5 6 7 8 9]);
%[C,rate] = confmat(reales,predicto);
imagesc(C), axis equal tight, colorbar
title('plsda test1.in')
colormap jet
