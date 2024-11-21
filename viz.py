import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV file
file_path = 'test2.csv'  # Replace with your actual file path
data = pd.read_csv(file_path)

# Strip any leading or trailing spaces from column names
data.columns = data.columns.str.strip()

# Normalize time values for transparency scaling
t_values = data['t'].unique()
min_t, max_t = t_values.min(), t_values.max()

# Plot the first curve (t = min_t) in dashed red
subset_first = data[data['t'] == min_t]
plt.plot(subset_first['x'], subset_first['f'], color='red', linestyle='--', label=f't={min_t:.2f} (initial)')

# Plot the rest with same color but variable opacity
for t in t_values[1:]:
    subset = data[data['t'] == t]
    alpha = (t - min_t) / (max_t - min_t)  # Normalize to range [0, 1]
    plt.plot(subset['x'], subset['f'], color='blue', alpha=alpha, label=f't={t:.2f}')

# Customize the plot
plt.xlabel('x')
plt.ylabel('f(x,t)')
plt.title('2D Plot of x vs f(x,t) for different values of t')
plt.legend(title="Time t", loc='upper left', bbox_to_anchor=(1, 1))
plt.grid(True)
plt.tight_layout()
plt.show()
