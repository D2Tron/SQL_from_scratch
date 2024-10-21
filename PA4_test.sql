--Execute this first in Terminal 1
CREATE DATABASE CS457_PA4;
USE CS457_PA4;
create table Flights (seat int, status int);
insert into Flights values (22,0);
insert into Flights values (23,1);
begin transaction;
update Flights set status = 1 where seat = 22;

--Execute this second in Terminal 2
USE CS457_PA4;
select * from Flights;
begin transaction;
update Flights set status = 1 where seat = 22;
commit;
select * from Flights;

--Exectute this third in Terminal 1
commit;
select * from Flights;
.EXIT;

--Execute this fourth in Terminal 2
select * from Flights;
.EXIT;

