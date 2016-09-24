function [v,lambda] = metodoPotencia(B,niter)
v = rand(size(B,1),1);
for i=1:niter
  Bv = B*v;
  v = Bv/norm(Bv);
end
lambda = (v'*B*v)/(norm(v)^2);