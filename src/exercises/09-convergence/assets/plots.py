import numpy as np
import matplotlib.pyplot as plt

plt.rcParams.update({
	'text.usetex' : True,
	'font.family' : 'serif',
	'font.serif' : ['Computer Modern Serif'],
	'font.size': 20
})

# fig, (ax1, ax2) = plt.subplots(1,2)
fig = plt.figure()
ax = fig.gca()

entries = np.genfromtxt('durations.csv', delimiter=',')
# entries = np.genfromtxt('solution_residuals.csv', delimiter=',')

N_values = np.array(entries[:,0])
data = np.array(entries[:,1])
log_N_values = np.log(N_values) / np.log(10)
log_data = np.log(data) / np.log(10)

a, b = np.polyfit(log_N_values, log_data, 1)
ax.plot(log_N_values, log_data, marker='o', markersize=10, color='black', linewidth=3)
ax.plot(log_N_values, a*log_N_values+b, label=f'linear fit: y = {a:.2f}x + ({b:.2f})', color='#e89e64', linewidth=3)
ax.set(xlabel=r'$\log N_\theta$', ylabel=r'$\log\ ($wall time in $s)$')
# ax.set(xlabel=r'$\log N_\theta$', ylabel=r'$\log\ ($solution residual$)$')

for i, log_N in enumerate(log_N_values):
	print(f'({log_N}, {log_data[i]})')
print(f'y = {a:.2f} * x + {b:.2f}')

fig.set_size_inches(8, 4, forward=True)
fig.set_dpi(100)

ax.legend()
plt.tight_layout()

fig.savefig(fname='plots.pdf', format='pdf', transparent=True)
# plt.show()
