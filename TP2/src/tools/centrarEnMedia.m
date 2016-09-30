function [X,media] = centrarEnMedia(Y)
media = mean(Y);
X=Y-media;