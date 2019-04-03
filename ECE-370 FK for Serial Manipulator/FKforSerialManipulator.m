%{
ECE-370: FK for Serial Manipulator
Name: Berket Yemaneberhane
Date: 4/3/19
%}

%clear previous data
clear;
clc;
format short;

% Assign variables

%units in Au
L0 = 0.3;
L1 = 0.2;
L2 = 0.1;

%units in radians
t0 = 0.4;
t1 = 0.6;
t2 = 1.2;

%long hand method
disp('long hand method: x and y coordinates')
x = L0*cos(t0) + L1*cos(t0+t1) + L2*cos(t0+t1+t2)
y = L0*sin(t0) + L1*sin(t0+t1) + L2*sin(t0+t1+t2)


%matrix method
disp('Matrix method: transformation matrix')

%origin
T = [1 0 0 0; 0 1 0 0 ; 0 0 1 0 ; 0 0 0 1];

%joint 1
T0 =[cos(t0) -sin(t0) 0 0; sin(t0) cos(t0) 0 0 ; 0 0 1 0 ; 0 0 0 1] * [1 0 0 L0; 0 1 0 0 ; 0 0 1 0 ; 0 0 0 1];

%joint 2
T1 = [cos(t1) -sin(t1) 0 0; sin(t1) cos(t1) 0 0 ; 0 0 1 0 ; 0 0 0 1] * [1 0 0 L1; 0 1 0 0 ; 0 0 1 0 ; 0 0 0 1];

%joint 3
T2 = [cos(t2) -sin(t2) 0 0; sin(t2) cos(t2) 0 0 ; 0 0 1 0 ; 0 0 0 1] * [1 0 0 L2; 0 1 0 0 ; 0 0 1 0 ; 0 0 0 1];

transformMatrix = T * T0 * T1 * T2


