#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########
#SBATCH --time=00:40:00                  # limit of wall clock time - how long the job will run (same as -t)
#SBATCH --nodes=2                        # Number of nodes
#SBATCH --ntasks-per-node=64             # Number of MPI tasks per node
#SBATCH --cpus-per-task=1                # number of CPUs (or cores) per task (same as -c)
#SBATCH --mem=8G                         # memory required per node - amount of memory (in bytes)
#SBATCH --job-name ring_shift_blocking   # you can give your job a name for easier identification (same as -J)
 
 
########## Command Lines to Run ##########

module purge
module load intel/2020a
module load CMake/3.16.4

cd $SLURM_SUBMIT_DIR                    ### change to the directory where your code is located

chmod +x ./scripts/run_ringshift.sh
./scripts/run_ringshift.sh "results/part3_ringshift.csv"
 
scontrol show job $SLURM_JOB_ID         ### write job information to output file