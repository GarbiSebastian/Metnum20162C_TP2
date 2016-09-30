function h = randHH(n,k)
I = h = eye(n);
for i = 1:k
  v=vectorRandom(n);
  v=v/norm(v);
  h2= I - 2*(v*v');
  h=h*h2;
end