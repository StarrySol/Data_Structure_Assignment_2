@echo off
set BASE=C:\Users\tikus\OneDrive\Documents\GitHub\Light_out\csd2401f25_team_clockworkz\Data_Structure_Assignment_2
set EXE=%BASE%\simplify.exe
set DS=%BASE%\own dataset

echo [1/16] combined_stress.csv
"%EXE%" "%DS%\combined_stress.csv" 190 > "%DS%\output1.csv"

echo [2/16] flat_ellipse.csv
"%EXE%" "%DS%\flat_ellipse.csv" 150 > "%DS%\output2.csv"

echo [3/16] high_vertex_1000.csv
"%EXE%" "%DS%\high_vertex_1000.csv" 500 > "%DS%\output3.csv"

echo [4/16] high_vertex_10000.csv
"%EXE%" "%DS%\high_vertex_10000.csv" 5000 > "%DS%\output4.csv"

echo [5/16] high_vertex_500.csv
"%EXE%" "%DS%\high_vertex_500.csv" 250 > "%DS%\output5.csv"

echo [6/16] high_vertex_5000.csv
"%EXE%" "%DS%\high_vertex_5000.csv" 2500 > "%DS%\output6.csv"

echo [7/16] many_holes_10.csv
"%EXE%" "%DS%\many_holes_10.csv" 42 > "%DS%\output7.csv"

echo [8/16] many_holes_20.csv
"%EXE%" "%DS%\many_holes_20.csv" 82 > "%DS%\output8.csv"

echo [9/16] many_holes_5.csv
"%EXE%" "%DS%\many_holes_5.csv" 22 > "%DS%\output9.csv"

echo [10/16] many_holes_50.csv
"%EXE%" "%DS%\many_holes_50.csv" 202 > "%DS%\output10.csv"

echo [11/16] narrow_gap_2.csv
"%EXE%" "%DS%\narrow_gap_2.csv" 4 > "%DS%\output11.csv"

echo [12/16] narrow_gap_50(1).csv
"%EXE%" "%DS%\narrow_gap_50(1).csv" 4 > "%DS%\output12.csv"

echo [13/16] narrow_gap_50.csv
"%EXE%" "%DS%\narrow_gap_50.csv" 4 > "%DS%\output13.csv"

echo [14/16] near_degenerate_1000.csv
"%EXE%" "%DS%\near_degenerate_1000.csv" 500 > "%DS%\output14.csv"

echo [15/16] near_degenerate_200.csv
"%EXE%" "%DS%\near_degenerate_200.csv" 100 > "%DS%\output15.csv"

echo [16/16] near_degenerate_500.csv
"%EXE%" "%DS%\near_degenerate_500.csv" 250 > "%DS%\output16.csv"

echo All done!
