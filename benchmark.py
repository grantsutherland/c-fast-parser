import pandas as pd
import time
import sys

file_path = 'data/test.csv'

try:
    start_time = time.time()
    

    df = pd.read_csv(file_path, header=None, engine='c')
    
  
    means = df.mean()
    vars = df.var(ddof=0) 
    
    end_time = time.time()
    
    print(f"\n--- Python (Pandas) Results ---")
    print(f"Processed in: {end_time - start_time:.4f} seconds")
    print(f"Mean Col 1: {means[1]:.4f}")
    print(f"Var  Col 1: {vars[1]:.4f}")

except FileNotFoundError:
    print(f"Could not find {file_path}")