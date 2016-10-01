function [lambda,v] = metodoPotencia(B,niter)
v = rand(size(B,2),1);
for i=1:niter
  Bv = B*v;
  v = Bv/norm(Bv);
end
lambda = (v'*B*v)/(norm(v)^2);