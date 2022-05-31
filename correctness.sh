mpirun -np 2 --allow-run-as-root ./src/count.out ./test 

cp output.txt dueproc.txt

for i in {5..100..5}
do
  echo "Checking with $i processors"

   mpirun -np $i --allow-run-as-root ./src/count.out ./test 

  if [[ $(diff <(sort dueproc.txt) <(sort output.txt)) ]]; then
        echo "Differences found."
    else
        echo "No differences found."
    fi

done




