function [val,vec]= pca(Mx, alfa)
rows = size(Mx,2);
val = zeros(alfa,1);
vec = zeros(rows,alfa);
for i=1:alfa
  [lambda,wi] = metodoPotencia(Mx,1000);
  val(i,1)= lambda;
  wi = wi/norm(wi);
  vec(:,i)= wi;
  Mx = Mx - lambda*wi*wi';
end
