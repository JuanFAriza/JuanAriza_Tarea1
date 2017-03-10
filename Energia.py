import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

datos = np.loadtxt('datos.dat')

t = datos[:,0]
E1 = datos[:,1]
E2 = datos[:,2]
E3 = datos[:,3]

plt.plot(t,E1)
plt.plot(t,E2)
plt.plot(t,E3)

archivo = 'Energia.pdf'
plt.savefig(archivo,format='pdf',transparent=True)
