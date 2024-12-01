import pandas as pd
import matplotlib.pyplot as plt
import os

def load_norms(file_path):
    """Load the Norms.csv file into a pandas DataFrame."""
    if not os.path.exists(file_path):
        print(f"Error: File {file_path} does not exist.")
        return None
    return pd.read_csv(file_path)

def plot_2x2_for_scheme(df, scheme):
    """Plot norms (L1, L2, LInf, Lp) for a specific scheme in a 2x2 grid."""
    # Filter data for the selected scheme
    scheme_data = df[df['Scheme'] == scheme]
    if scheme_data.empty:
        print(f"No data found for Scheme: {scheme}")
        return

    # Set up the 2x2 grid of subplots
    fig, axes = plt.subplots(2, 2, figsize=(12, 10))
    norms = ['L1', 'L2', 'LInf', 'Lp(p=2.5)']
    axes = axes.flatten()

    for ax, norm in zip(axes, norms):
        for tmax in scheme_data['Tmax'].unique():
            subset = scheme_data[scheme_data['Tmax'] == tmax]
            ax.plot(
                subset['Samples'], 
                subset[norm], 
                marker='o', 
                linestyle='-', 
                label=f'Tmax={tmax}'
            )

        ax.set_title(f'{norm} Norm', fontsize=14)
        ax.set_xlabel('Samples', fontsize=12)
        ax.set_ylabel(norm, fontsize=12)
        ax.grid(True)
        ax.legend(title="Tmax")

    plt.suptitle(f'Norms for Scheme: {scheme}', fontsize=16)
    plt.tight_layout(rect=[0, 0, 1, 0.96])
    plt.show()

# Main function
def main():
    # Path to the Norms.csv file
    norms_file = os.path.join('Results', 'NormsResult', 'Norms.csv')
    
    # Load the norms data
    df = load_norms(norms_file)
    if df is None:
        return

    # Uncomment the following function calls to enable other visualizations
    # visualize_all_norms_grouped(df)  # Uncomment this for grouped visualizations of all norms
    # plot_norms_by_method_and_tmax(df, "L1")  # Uncomment this to visualize a specific norm grouped by method and Tmax

    # Directly visualize the selected scheme with 2x2 layout
    selected_scheme = "E_FTBS"  # Replace with the desired scheme name
    plot_2x2_for_scheme(df, selected_scheme)

if __name__ == "__main__":
    main()
