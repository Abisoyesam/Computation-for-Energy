%% Program to read, manipulate, and plot weather data
clc; clear; close all
%% Load the Data
data = readtable("weather.csv", "VariableNamingRule","preserve", "ReadVariableNames",true);
data

% select needed columns data (temperature, humidity, speed)
date = data(2:end,"Var1"); % date -> the first column
temp = data(2:end,2:4);    % temperature 
humdity = data(2:end,8:10);% humidity
speed = data(2:end,11:13); % speed (mph)

%% data manipulation
% remove degree F from the temperature 
avg_temp_ = temp(:,2).Variables;
A = string(avg_temp_); % chnage cell to string array
% data casting string -> number
avg_temp = str2double(strrep(A, ' °F', ''));

% remove percentage from humidity 
avg_humdity_ =  humdity(:,2).Variables;
B = string(avg_humdity_); % chnage cell to string array
avg_humidity = str2double(strrep(B, '%', ''));

% remove mph from speed
avg_speed_ =  speed(:,2).Variables;
C = string(avg_speed_); % chnage cell to string array
avg_speed = str2double(strrep(C, ' mph', ''));

% date already in datetime datatype
date = date.Variables;

%% plot the graph
figure(1)
plot(date,avg_speed,date,avg_humidity,date,avg_temp,'LineWidth', 1.5)
xlabel("Date")
legend(["Average Speed","Average Humidity", "Average Temp"])
title("Weather Data")
grid on

%% Using Subplot
figure(2)
% Subplot 1: Average Temperature
subplot(3,1,1);       % 3 rows, 1 column, 1st subplot
plot(date, avg_temp, '-r', 'LineWidth', 1.5);
title('Average Temperature');
xlabel('Date');
ylabel('Temperature (°F)');
grid on;

% Subplot 2: Average Humidity
subplot(3,1,2);       % 3 rows, 1 column, 2nd subplot
plot(date, avg_humidity, '-b', 'LineWidth', 1.5);
title('Average Humidity');
xlabel('Date');
ylabel('Humidity (%)');
grid on;

% Subplot 3: Average Speed
subplot(3,1,3);       % 3 rows, 1 column, 3rd subplot
plot(date, avg_speed, '-g', 'LineWidth', 1.5);
title('Average Speed');
xlabel('Date');
ylabel('Speed (km/h)');
grid on;

