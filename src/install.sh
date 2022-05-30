export OMPI_MCA_btl_vader_single_copy_mechanism=none
apt-get update
apt-get install libglib2.0-dev

#mpirun --mca btl_vader_single_copy_mechanism none -np 4 --allow-run-as-root driver.out /home/gerardo/Scaricati/test_files