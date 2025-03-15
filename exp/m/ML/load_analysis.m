clc; close all, clear;
crv_table = readtable("flat.csv");
crv = table2array(crv_table(:,2:24));
%plot (crv(2:24,1), crv(2:,2))
h=[0:23];
y=crv(:,2);
figure (1)
plot (h,y);
figure (2)
stackedplot(crv);
crvn = crv./mean(crv);
figure(3)
plot(h, crvn);
crv_mean = mean(crvn')';
hold on
plot(h, crv_mean, 'r', 'LineWidth',2);
legend();

data_for_clustering = crvn';
Y = pdist(data_for_clustering);
Z = linkage(Y,"average");
dendrogram(Z)
% get clusters
n_clusters = 6;
T = cluster(Z,"maxclust",n_clusters);
clusters = zeros(size(crvn, 1), n_clusters);
for c = 1:n_clusters
 clusters(:, c) = mean(crvn(:, T==c), 2);
end
figure
plot(clusters)
legend()
