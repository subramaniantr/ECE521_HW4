clc

subplot(3,2,1)
a = csvread('out19.csv');
plot(a(:,1),a(:,3))
title('Plot19');

subplot(3,2,2)
a = csvread('out20.csv');
plot(a(:,1),a(:,3))
title('Plot20');

subplot(3,2,3)
a = csvread('out21.csv');
plot(a(:,1),a(:,3))
title('Plot21');

subplot(3,2,4)
a = csvread('out22.csv');
plot(a(:,1),a(:,3))
title('Plot22');

subplot(3,2,5)
a = csvread('out23.csv');
plot(a(:,1),a(:,3))
title('Plot23');

subplot(3,2,6)
a = csvread('out24.csv');
plot(a(:,1),a(:,3))
title('Plot24');
