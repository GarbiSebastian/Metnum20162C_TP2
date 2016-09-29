function [val,vec]= plsda(X ,Y ,g)
for i=1:g
  Mi= X'*Y*Y'*X;
  [wi,lambda] = metodoPotencia(Mi,1000);
  val(i,1)= lambda;
  wi = wi/norm(wi);
  vec(i)= wi;
  ti = X*wi;
  ti = ti/norm(ti);
  X = X- ti*(ti'*X);
  Y = Y- ti*(ti'*Y);
end
