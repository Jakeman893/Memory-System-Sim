
######################################################################################
# This scripts runs all three traces
# You will need to uncomment the configurations that you want to run
# the results are stored in the ../results/ folder 
######################################################################################

make
rm ../src.tar.gz
rm ../results.tar.gz
rm ../results/*

########## ---------------  A.1 ---------------- ################
echo "Part A..."
./sim -mode 1 ../traces/bzip2.mtr.gz > ../results/A1.bzip2.res
./sim -mode 1 ../traces/lbm.mtr.gz  > ../results/A1.lbm.res 
./sim -mode 1 ../traces/libq.mtr.gz   > ../results/A1.libq.res 
echo "done"
########## ---------------  B ---------------- ################
echo "Part B..."
./sim -mode 2 -L2sizeKB 1024 ../traces/bzip2.mtr.gz  > ../results/B.S1MB.bzip2.res 
./sim -mode 2 -L2sizeKB 1024 ../traces/lbm.mtr.gz    > ../results/B.S1MB.lbm.res &
./sim -mode 2 -L2sizeKB 1024 ../traces/libq.mtr.gz    > ../results/B.S1MB.libq.res
echo "done"
########## ---------------  C ---------------- ################
echo "Part C..."
./sim -mode 3 -L2sizeKB 1024 ../traces/bzip2.mtr.gz  > ../results/C.S1MB.bzip2.res 
./sim -mode 3 -L2sizeKB 1024 ../traces/lbm.mtr.gz    > ../results/C.S1MB.lbm.res &
./sim -mode 3 -L2sizeKB 1024 ../traces/libq.mtr.gz    > ../results/C.S1MB.libq.res 
echo "done"
########## ---------------  D ---------------- ################

echo "Part D..."
./sim -mode 4 ../traces/bzip2.mtr.gz ../traces/libq.mtr.gz > ../results/D.mix1.res
./sim -mode 4 ../traces/bzip2.mtr.gz ../traces/lbm.mtr.gz  > ../results/D.mix2.res
./sim -mode 4 ../traces/lbm.mtr.gz ../traces/libq.mtr.gz   > ../results/D.mix3.res
echo "done"
########## ---------------  E (Same as D, except L2rep=2l) -------------- ################
echo "Part E..."
./sim -mode 5 -L2repl 2 -SWP_core0ways 4 ../traces/bzip2.mtr.gz ../traces/libq.mtr.gz  > ../results/E.Q1.mix1.res
./sim -mode 5 -L2repl 2 -SWP_core0ways 8 ../traces/bzip2.mtr.gz ../traces/libq.mtr.gz  > ../results/E.Q2.mix1.res
./sim -mode 5 -L2repl 2 -SWP_core0ways 12 ../traces/bzip2.mtr.gz ../traces/libq.mtr.gz > ../results/E.Q3.mix1.res

./sim -mode 5 -L2repl 2 -SWP_core0ways 4 ../traces/bzip2.mtr.gz ../traces/lbm.mtr.gz  > ../results/E.Q1.mix2.res
./sim -mode 5 -L2repl 2 -SWP_core0ways 8 ../traces/bzip2.mtr.gz ../traces/lbm.mtr.gz  > ../results/E.Q2.mix2.res
./sim -mode 5 -L2repl 2 -SWP_core0ways 12 ../traces/bzip2.mtr.gz ../traces/lbm.mtr.gz > ../results/E.Q3.mix2.res

./sim -mode 5 -L2repl 2 -SWP_core0ways 4 ../traces/lbm.mtr.gz ../traces/libq.mtr.gz  > ../results/E.Q1.mix3.res
./sim -mode 5 -L2repl 2 -SWP_core0ways 8 ../traces/lbm.mtr.gz ../traces/libq.mtr.gz  > ../results/E.Q2.mix3.res
./sim -mode 5 -L2repl 2 -SWP_core0ways 12 ../traces/lbm.mtr.gz ../traces/libq.mtr.gz > ../results/E.Q3.mix3.res
echo "done"

########## ---------------  F (Same as D, except L2repl=3) -------------- ################


# ./sim -mode 6 -L2repl 3  ../traces/bzip2.mtr.gz ../traces/libq.mtr.gz  > ../results/F.mix1.res
# ./sim -mode 6 -L2repl 3  ../traces/bzip2.mtr.gz ../traces/lbm.mtr.gz  > ../results/F.mix2.res
# ./sim -mode 6 -L2repl 3  ../traces/lbm.mtr.gz ../traces/libq.mtr.gz  > ../results/F.mix3.res

echo "Make tar archive..."

tar -zcvpf ../results.tar.gz ../results

tar -zcvpf ../src.tar.gz ../src --exclude=*.out --exclude=sim

echo "done"

echo "All Done. Check the .res file in ../results directory";

