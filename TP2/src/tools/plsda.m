function [val,vec]= plsda(X ,Y ,g)
rows = size(X,2);
val = zeros(g,1);
vec = zeros(rows,g);
for i=1:g
  #Mi= X'*Y*Y'*X;
  A = X'*Y;
  Mi = A*A';
  [lambda,wi] = metodoPotencia(Mi,1000);
  val(i,1)= lambda;
  wi = wi/norm(wi);
  vec(:,i)= wi;
  ti = X*wi;
  ti = ti/norm(ti);
  X = X- ti*(ti'*X);
  Y = Y- ti*(ti'*Y);
end
