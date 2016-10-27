clear all;
close all;
fclose all;
clc;

LOG_ID = 9;

mainpath='C:\WORK\uaworld\Crazyflie\code\cflieOffboardAAU\trunk\x64\Release\ControlLogs';


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ACCELEROMETER
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fileID = fopen([mainpath,num2str(LOG_ID),'\output_CF_acc.txt'],'r');
% InputText = textscan(fileID,'%s',1,'delimiter','\n');  % Read 1 header lines
FormatString = repmat('%f64',1,7);           
InputText = textscan(fileID,FormatString,'delimiter',' ');
data = cell2mat(InputText);
fclose(fileID);
 
 figure; hold on;
 timeline = (data(:,2)); 
 plot( timeline, data(:,3),'.-b');
 plot( timeline, data(:,4),'.-g');
 plot( timeline, data(:,5),'.-k');
 title('Acc data');
 legend('X','Y','Z');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% GYROSCOPE
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fileID = fopen([mainpath,num2str(LOG_ID),'\output_CF_gyro.txt'],'r');
% InputText = textscan(fileID,'%s',1,'delimiter','\n');  % Read 1 header lines
FormatString = repmat('%f64',1,5);           
InputText = textscan(fileID,FormatString,'delimiter',' ');
data = cell2mat(InputText);
fclose(fileID);

figure; hold on;
timeline = (data(:,2));
plot( timeline, data(:,3),'.-b');
plot( timeline, data(:,4),'.-g');
plot( timeline, data(:,5),'.-k');
title('Gyro data');
legend('X','Y','Z');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% MAGNETOMETER
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fileID = fopen([mainpath,num2str(LOG_ID),'\output_CF_mag.txt'],'r');
% InputText = textscan(fileID,'%s',1,'delimiter','\n');  % Read 1 header lines
FormatString = repmat('%f64',1,5);           
InputText = textscan(fileID,FormatString,'delimiter',' ');
data = cell2mat(InputText);
fclose(fileID);

figure; hold on;
timeline = (data(:,2));
plot( timeline, data(:,3),'.-b');
plot( timeline, data(:,4),'.-g');
plot( timeline, data(:,5),'.-k');
title('Mag data');
legend('X','Y','Z');

% figure; hold on;
% plot( data(:,3), data(:,4),'.-b');
% axis([-1.5,1.5,-1.5,1.5]);
% 
% 
% figure; hold on;
% timeline = (data(:,2));
% plot( timeline, (data(:,3)+1/2)/0.3,'.-b');
% plot( timeline, (data(:,4)-1.22)/0.3,'.-g');
% title('Mag data');
% legend('X','Y');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% MOTOR RAW DATA
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fileID = fopen([mainpath,num2str(LOG_ID),'\output_CF_motor.txt'],'r');
% InputText = textscan(fileID,'%s',1,'delimiter','\n');  % Read 1 header lines
FormatString = repmat('%f64',1,6);           
InputText = textscan(fileID,FormatString,'delimiter',' ');
data = cell2mat(InputText);
fclose(fileID);

figure; hold on;
timeline = (data(:,2));
plot( timeline, data(:,3),'.-b');
plot( timeline, data(:,4),'.-g');
plot( timeline, data(:,5),'.-k');
plot( timeline, data(:,5),'.-r');
title('Motor data');
legend('M1','M2','M3','M4');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ROLL,PITCH,YAW as estimated by the FIRMWARE
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fileID = fopen([mainpath,num2str(LOG_ID),'\output_CF_stab.txt'],'r');
% InputText = textscan(fileID,'%s',1,'delimiter','\n');  % Read 1 header lines
FormatString = repmat('%f64',1,6);  
InputText = textscan(fileID,FormatString,'delimiter',' ');
data = cell2mat(InputText);
fclose(fileID);

figure; hold on;
timeline = (data(:,2));
plot( timeline, data(:,3),'.-b');
plot( timeline, -data(:,4),'.-g');
plot( timeline, data(:,5),'.-k');
title('STAB data');
legend('Roll','Pitch','Yaw');

CFest_timeline = data(:,2);
CFest_pitch = -data(:,4);
CFest_roll = data(:,3);
CFest_yaw = data(:,5);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% GoT RAW measurements 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fileID = fopen([mainpath,num2str(LOG_ID),'\output_GoT_raw.txt'],'r');
% InputText = textscan(fileID,'%s',1,'delimiter','\n');  % Read 1 header lines
FormatString = repmat('%f64',1,5);  
InputText = textscan(fileID,FormatString,'delimiter',' ');
data = cell2mat(InputText);
fclose(fileID);

figure; 
subplot(2,1,1); hold on;
    timeline = (data(:,1));
    plot( timeline, data(:,3),'.-b');
    plot( timeline, data(:,4),'.-g');
    plot( timeline, data(:,5),'.-k');
    title('GoT RAW data');
    legend('x','y','z');
subplot(2,1,2); hold on;
    plot( diff(data(:,1)),'.-b');
    plot( diff(data(:,2))/1000,'.-r');
    legend('PC Sample','GoT Sample');
    
GoT_raw_timeline = timeline;
GoT_raw_GoT_timeline = data(:,2);
GoT_raw(:,1) = data(:,3);
GoT_raw(:,2) = data(:,4);
GoT_raw(:,3) = data(:,5);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% GoT measurements 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fileID = fopen([mainpath,num2str(LOG_ID),'\output_GoT.txt'],'r');
% InputText = textscan(fileID,'%s',1,'delimiter','\n');  % Read 1 header lines
FormatString = repmat('%f64',1,5);  
InputText = textscan(fileID,FormatString,'delimiter',' ');
data = cell2mat(InputText);
fclose(fileID);

figure; 
subplot(2,1,1); hold on;
    timeline = (data(:,1));
    plot( timeline, data(:,3),'.-b');
    plot( timeline, data(:,4),'.-g');
    plot( timeline, data(:,5),'.-k');
    title('GoT data');
    legend('x','y','z');
subplot(2,1,2); hold on;
    plot( diff(data(:,1)),'.-b');
    plot( diff(data(:,2))/1000,'.-r');
    legend('PC Sample','GoT Sample');

GoT_timeline = timeline;
GoT_GoT_timeline = data(:,2);
GoT(:,1) = data(:,3);
GoT(:,2) = data(:,4);
GoT(:,3) = data(:,5);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CMD as sent by computer/remote algorithm 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fileID = fopen([mainpath,num2str(LOG_ID),'\output_CTRL.txt'],'r');
% InputText = textscan(fileID,'%s',1,'delimiter','\n');  % Read 1 header lines
FormatString = repmat('%f64',1,5);  
InputText = textscan(fileID,FormatString,'delimiter',' ');
data = cell2mat(InputText);
fclose(fileID);

figure; hold on; 
timeline = (data(:,1));
subplot(2,1,1); hold on;
plot( timeline, data(:,2),'.-b');
plot( timeline, data(:,3),'.-g');
plot( timeline, data(:,4),'.-k');
legend('R','P','Y'); hold on;
title('CMD data');
subplot(2,1,2);
plot( timeline, data(:,5),'.-r');
legend('Thrust');
title('CMD data');

CMD_timeline = timeline;
CMD(:,1) = data(:,2);
CMD(:,2) = data(:,3);
CMD(:,3) = data(:,4);
CMD(:,4) = data(:,5);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% POSITION GIVEN REFERENCE 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fileID = fopen([mainpath,num2str(LOG_ID),'\output_REF.txt'],'r');
% InputText = textscan(fileID,'%s',1,'delimiter','\n');  % Read 1 header lines
FormatString = repmat('%f64',1,5);  
InputText = textscan(fileID,FormatString,'delimiter',' ');
data = cell2mat(InputText);
fclose(fileID);

REF_timeline = (data(:,1));
REF(:,1) = data(:,2);
REF(:,2) = data(:,3);
REF(:,3) = data(:,4);

%# Try to add the first reference, which is the first meansurement (and z is 1 meter higher)
REF_timeline = [GoT_timeline(1) ; REF_timeline];
REF = [ [GoT(1,1); REF(:,1)],  [GoT(1,2); REF(:,2)], [GoT(1,3)-1; REF(:,3)] ];

figure; hold on; title('REF data');
stairs( REF_timeline, REF(:,1),'.-b');
stairs( REF_timeline, REF(:,2),'.-g');
stairs( REF_timeline, REF(:,3),'.-k');
legend('X','Y','Z');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% POSITION TRACKING 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure; title('Pos: REAL vs Meas');
subplot(3,1,1); title('X'); hold on;
    plot(GoT_timeline, GoT(:,1),'.-k');
    stairs(REF_timeline, REF(:,1),'.-b');
    legend('Meas','Ref');
subplot(3,1,2); title('Y'); hold on;
    plot(GoT_timeline, GoT(:,2),'.-k');
    stairs(REF_timeline, REF(:,2),'.-b');
    legend('Meas','Ref');
subplot(3,1,3); title('Z'); hold on;
    plot(GoT_timeline, GoT(:,3),'.-k');
    stairs(REF_timeline, REF(:,3),'.-b');
    legend('Meas','Ref');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ALL IN ONE MESSY PLOT 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure; title('XYZ vs RPT');
subplot(3,1,1); title('X'); hold on;
    plot(GoT_timeline, GoT(:,1),'.-k');
    stairs(REF_timeline, REF(:,1),'.-b');
    plot(CMD_timeline, CMD(:,2),'.-m');
    plot(CFest_timeline, CFest_pitch,'.-r');
    legend('Meas','Ref','Cmd/Pitch','CF Est. Pitch');
subplot(3,1,2); title('Y'); hold on;
    plot(GoT_timeline, GoT(:,2),'.-k');
    stairs(REF_timeline, REF(:,2),'.-b');
    plot(CMD_timeline, CMD(:,1),'.-m');
    plot(CFest_timeline, CFest_roll,'.-r');
    legend('Meas','Ref','Cmd/Roll','CF Est. Roll');
subplot(3,1,3); title('Z'); hold on;
    plot(GoT_timeline, GoT(:,3),'.-k');
    stairs(REF_timeline, REF(:,3),'.-b');
    plot(CMD_timeline, CMD(:,4),'.-m');
    legend('Meas','Ref/Thrust','Cmd');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    