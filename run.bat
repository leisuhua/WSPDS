@echo off
set curdir=%~dp0
cd /d %curdir%

echo ^<table width='100%' border='1' cellspacing='1' cellpadding='1' borderColor=#0000FF^>^<tr^>^<th^>PostRes^</th^>^<th^>PostTime(s)^</th^>^<th^>PostSpace(byte)^</th^>^<th^>EdgeNum^</th^>^<th^>ReachRes^</th^>^<th^>ReachTime(s)^</th^>^<th^>ReachSpace(byte)^</th^>^<th^>NodeNum^</th^>^</tr^>^</table^> > res.xls

echo ^<table width='100%' border='1' cellspacing='1' cellpadding='1' borderColor=#0000FF^>^<tr^>^<th^>Tag^</th^>^<th^>Coefficientq^</th^>^<th^>Constantq^</th^>^<th^>Tab^</th^>^<th^>Tab^</th^>^<th^>Tab^</th^>^<th^>Tab^</th^>^<th^>Tab^</th^>^</tr^>^</table^>  > record.xls

for /l %%i in (1,1,1000) do ( 

echo ^<table width='100%' border='1' cellspacing='1' cellpadding='1' borderColor=#0000FF^>^<tr^> >> res.xls
WSPDS_Generator.exe %%i

for /f "tokens=1,2,4,6" %%i in ('Post*-automata_algorithm.exe') do (
echo ^<th^>%%i^</th^> >> res.xls
echo ^<th^>%%j^</th^> >> res.xls
echo ^<th^>%%k^</th^> >> res.xls
echo ^<th^>%%l^</th^> >> res.xls
)

for /f "tokens=1,2,4,6" %%i in ('Reachability_tree_algorithm.exe') do (
echo ^<th^>%%i^</th^> >> res.xls
echo ^<th^>%%j^</th^> >> res.xls
echo ^<th^>%%k^</th^> >> res.xls
echo ^<th^>%%l^</th^> >> res.xls
)

echo ^</tr^>^</table^> >> res.xls

timeout 1
)

pause
