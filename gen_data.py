import csv
import sys

def generate_csv(filename, rows, cols):
    print(f"Generating {filename} ({rows:,} rows, {cols} cols)...")
    with open(filename, 'w', newline='') as f:
        writer = csv.writer(f)
        # Write in chunks of 1 million to save RAM
        for chunk in range(rows // 1000000):
            batch = []
            for i in range(1000000):
                # Generates simple floats: row_num, 50.0, 750.0, etc.
                batch.append([float(chunk * 1000000 + i)] + [50.0] * (cols - 1))
            writer.writerows(batch)
            print(f"  Wrote {min((chunk + 1) * 1000000, rows):,} rows...")

if __name__ == "__main__":
    import os
    os.makedirs("data", exist_ok=True)
    
    # 1. Generate Deep Data (100M rows, 3 cols) - For Thread Scaling
    #generate_csv("data/gigantic_test.csv", 100000000, 3)
    
    # 2. Generate Wide Data (1M rows, 250 cols) - For Stack Memory Testing
    generate_csv("data/wide_test.csv", 1000000, 250)
    
    print("Data generation complete!")