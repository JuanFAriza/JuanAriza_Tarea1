import numpy as np
import matplotlib.pyplot as plt

datos = np.loadtxt('datosTiempo.dat')

proc = datos[:,0]
t = datos[:,1]


plt.plot(proc,t)

archivo = 'Tiempo.pdf'
plt.savefig(archivo,format='pdf',transparent=True)
