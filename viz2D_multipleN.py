import pandas as pd
import matplotlib.pyplot as plt
import os

def plot_t0_tn(file_paths, dataframes, output_path):
    """
    Plot T0 (initial curve) from the first CSV and TN (final curves) from all CSVs.
    Saves the plot to the specified output path.
    """
    plt.figure(figsize=(10, 6))

    # Plot T0 from the first CSV
    first_df = dataframes[0]
    subset_t0_first = first_df[first_df['t'] == first_df['t'].min()]
    plt.plot(subset_t0_first['x'], subset_t0_first['f'], color='red', linewidth=2)

    # Define colors for TN curves
    tn_colors = ['blue', 'green', 'orange']

    # Plot TN from all CSVs with distinct colors
    for i, (df, color) in enumerate(zip(dataframes, tn_colors)):
        subset_t_last = df[df['t'] == df['t'].max()]
        plt.plot(subset_t_last['x'], subset_t_last['f'], label=f'CSV {file_paths[i]} a T = 10', linestyle='--', color=color)

    # Customize the plot
    plt.xlabel('x (Spatial Coordinate)')
    plt.ylabel('f (Function Value)')
    plt.legend(loc='upper left')
    plt.grid(True)

    # Save the plot
    plt.tight_layout()
    plt.savefig(output_path)
    print(f"Plot saved: {output_path}")

# Main script
def main():
    """
    Process multiple CSV files and generate a single plot showing T0 and TN curves.
    """
    # Define file paths
    file_paths = [
        r"Results\I_FTBS_SET2_exp_100_10.csv",
        r"Results\I_FTBS_SET2_exp_200_10.csv",
        r"Results\I_FTBS_SET2_exp_400_10.csv",
    ]

    # Output file path
    output_path = r"Results\I_FTBS_SET2_exp_T0_First_T10_All.png"

    # Load and preprocess data
    dataframes = []
    for file_path in file_paths:
        df = pd.read_csv(file_path, skiprows=1, names=['x', 't', 'f'])  # Skip header row and assign correct column names
        df = df.astype({'x': 'float64', 't': 'float64', 'f': 'float64'})  # Ensure numeric data
        dataframes.append(df)

    # Generate the plot
    plot_t0_tn(file_paths, dataframes, output_path)

if __name__ == "__main__":
    main()
