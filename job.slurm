#!/bin/bash
#SBATCH --job-name=simple_job        # Job name
#SBATCH --output=output_%j.txt       # Output file (%j = Job ID)
#SBATCH --time=00:02:00              # Max time (hh:mm:ss)
#SBATCH --partition=short            # Partition/queue name
#SBATCH --ntasks=1                   # Number of tasks
#SBATCH --cpus-per-task=1            # CPUs per task
#SBATCH --mem=512M                   # Memory

echo "Running on host: $(hostname)"
echo "Start time: $(date)"
sleep 30                              # Simulate a short task
echo "End time: $(date)"

