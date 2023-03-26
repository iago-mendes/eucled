

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

import matplotlib
print(matplotlib.__version__)

stridepow = 1
stride = 2**stridepow
nth = stride*10#10
nph = stride*20#20

theta1 = .5*np.pi/nth + np.linspace(0, np.pi, nth, endpoint=False)
phi1 = np.linspace(0, 2.*np.pi, nph, endpoint=False)

dtheta = np.pi/nth
dphi = 2.*np.pi/nph

#theta, phi = np.meshgrid(theta1, phi1)
phi, theta = np.meshgrid(phi1, theta1, indexing='xy')

x = np.sin(theta)*np.cos(phi)
y = np.sin(theta)*np.sin(phi)
z = np.cos(theta)

ethx = np.cos(theta)*np.cos(phi)
ethy = np.cos(theta)*np.sin(phi)
ethz = -np.sin(theta)

ephx = -y
ephy = x
ephz = np.zeros_like(x)

#fig = plt.figure()
#ax = fig.add_subplot(111, projection='3d')

fig = plt.figure()
ax = fig.gca(projection='3d')

#ax.quiver(x, y, z, ethx, ethy, ethz, length=.3, normalize=False, pivot='middle')
ax.quiver(x[::stride,::stride], y[::stride,::stride], z[::stride,::stride], ethx[::stride,::stride], ethy[::stride,::stride], ethz[::stride,::stride], length=.3, normalize=False, pivot='middle')
#ax.quiver(x, y, z, ephx, ephy, ephz, length=.3, normalize=False, color="red", pivot='middle')
ax.quiver(x[::stride,::stride], y[::stride,::stride], z[::stride,::stride], ephx[::stride,::stride], ephy[::stride,::stride], ephz[::stride,::stride], length=.3, normalize=False, color="red", pivot='middle')
plt.show()



## ====== Below code copied from EmbeddingByFactorization.py =======

Theta = theta
Phi = phi
Nth = nth
Nphi = nph

# Metric of an ellipsoid:
aa = 1.5
bb = 1.
cc = 1.
g_thth = (aa*np.cos(Theta)*np.cos(Phi))**2 + (bb*np.cos(Theta)*np.sin(Phi))**2 + (cc*np.sin(Theta))**2
g_phph = (aa*np.sin(Theta)*np.sin(Phi))**2 + (bb*np.sin(Theta)*np.cos(Phi))**2
g_thph = (bb**2 - aa**2)*np.cos(Theta)*np.sin(Theta)*np.cos(Phi)*np.sin(Phi)

#print(np.mean((g_thth0-g_thth)**2))
#print(np.mean((g_thph0-g_thph)**2))
#print(np.mean((g_phph0-g_phph)**2))

sqrtdetgOrig = np.sqrt((g_thth*g_phph-g_thph**2))
sqrtdetg = np.zeros((Nth, Nphi, 3))
for i in range(3):
    sqrtdetg[:,:,i] = sqrtdetgOrig

    
R = np.mean([aa, bb, cc])

# Initial guess vectors:
e_th = np.zeros((Nth, Nphi, 3))
e_th[:,:,0] = R*np.cos(Theta)*np.cos(Phi)
e_th[:,:,1] = R*np.cos(Theta)*np.sin(Phi)
e_th[:,:,2] = -R*np.sin(Theta)

e_ph = np.zeros((Nth, Nphi, 3))
e_ph[:,:,0] = -R*np.sin(Theta)*np.sin(Phi)
e_ph[:,:,1] = R*np.sin(Theta)*np.cos(Phi)

#fig = plt.figure()
#ax = fig.gca(projection='3d')
    
#ax.quiver(x, y, z, e_th[::1,:,0], e_th[::1,:,1], e_th[::1,:,2], length=.3, normalize=False, pivot='middle')
#ax.quiver(x, y, z, e_ph[::-1,:,0], e_ph[::-1,:,1], e_ph[::-1,:,2], length=.3, normalize=False, color="red", pivot='middle')
#plt.show()


# Scaling and shearing to satisfy metric conditions:
#   First, construct inner products of the purported basis vectors:
G0_thth = e_th[:,:,0]**2 + e_th[:,:,1]**2 + e_th[:,:,2]**2
G0_phph = e_ph[:,:,0]**2 + e_ph[:,:,1]**2 + e_ph[:,:,2]**2
G0_thph = e_th[:,:,0]*e_ph[:,:,0] + e_th[:,:,1]*e_ph[:,:,1] + e_th[:,:,2]*e_ph[:,:,2]

#   Finding alpha (the scaling of e_theta) is easy:
alpha = np.sqrt(g_thth/G0_thth)

#   Now, initial guesses for beta and gamma
beta = np.ones_like(alpha)
gamma = np.zeros_like(alpha)

'''
#   Newton-Raphson solves, point-by-point:
for ii in range(Nth):
    for jj in range(Nphi):
        for iteration in range(0):# was 10
            Jac = np.zeros((2,2))
            Jac[0,0] = G0_thph[ii,jj]
            Jac[0,1] = G0_thth[ii,jj]
            Jac[1,0] = 2.*beta[ii,jj]*G0_phph[ii,jj] + 2.*gamma[ii,jj]*G0_thph[ii,jj]
            Jac[1,1] = 2.*gamma[ii,jj]*G0_thth[ii,jj] + 2.*beta[ii,jj]*G0_thph[ii,jj]
            MRes = np.zeros(2)
            MRes[0] = (g_thph[ii,jj]/alpha[ii,jj]) - beta[ii,jj]*G0_thph[ii,jj] - gamma[ii,jj]*G0_thth[ii,jj]
            MRes[1] = g_phph[ii,jj] - beta[ii,jj]**2*G0_phph[ii,jj] - gamma[ii,jj]**2*G0_thth[ii,jj] - 2.*beta[ii,jj]*gamma[ii,jj]*G0_thph[ii,jj]
            deltabeta, deltagamma = np.linalg.solve(Jac, MRes)

            beta[ii,jj] += deltabeta
            gamma[ii,jj] += deltagamma

            if(ii==Nth/2 and jj==Nphi/2):
                print("Iteration", iteration, ", residual:", MRes)
            
# Now, construct the rescaled basis:
enew_th = np.zeros_like(e_th)
enew_ph = np.zeros_like(e_ph)
for k in range(3):
    enew_th[:,:,k] = alpha[:,:]*e_th[:,:,k]
    enew_ph[:,:,k] = beta[:,:]*e_ph[:,:,k] + gamma[:,:]*e_th[:,:,k]

'''

enew_th = np.copy(e_th)
enew_ph = np.copy(e_ph)

fig = plt.figure()
ax = fig.gca(projection='3d')
    
#ax.quiver(x, y, z, enew_th[:,:,0], enew_th[:,:,1], enew_th[:,:,2], length=.3, normalize=False, pivot='middle')
ax.quiver(x[::stride,::stride], y[::stride,::stride], z[::stride,::stride], enew_th[::stride,::stride,0], enew_th[::stride,::stride,1], enew_th[::stride,::stride,2], length=.3, normalize=False, pivot='middle')
#ax.quiver(x, y, z, enew_ph[:,:,0], enew_ph[:,:,1], enew_ph[:,:,2], length=.3, normalize=False, color="red", pivot='middle')
ax.quiver(x[::stride,::stride], y[::stride,::stride], z[::stride,::stride], enew_ph[::stride,::stride,0], enew_ph[::stride,::stride,1], enew_ph[::stride,::stride,2], length=.3, normalize=False, color="red", pivot='middle')
plt.show()



def thetaderiv(f):
    fprime = (np.roll(f, -1, axis = 0) - np.roll(f, 1, axis=0))/(2.*dtheta)
    fprime[0,:] = (f[1,:]-f[0,:])/(dtheta)
    #fprime[0,:] = -(f[2,:]-4.*f[1,:]+3.*f[0,:])/(2.*dtheta)
    fprime[-1,:] = (f[-1,:]-f[-2,:])/(dtheta)
    return fprime

def phideriv(f):
    return (np.roll(f, -1, axis=1) - np.roll(f, 1, axis=1))/(2.*dphi)
    #return (-1.*np.roll(f, -2, axis=1) + 8.*np.roll(f, -1, axis=1) - 8.*np.roll(f, 1, axis=1) + 1.*np.roll(f, 2, axis=1))/(12.*dphi)


# Now, load the factored basis vectors into our starting data
# for commutator relaxation:
e_th = np.copy(enew_th)
e_ph = np.copy(enew_ph)

# And just because I named some buffer variables differently below:
ethnew = np.copy(e_th)
ephnew = np.copy(e_ph)

# The commutator relaxation code:

Comm = thetaderiv(e_ph) - phideriv(e_th)

#print(Commutator)

CommutatorNormSquared = Comm[:,:,0]**2 + Comm[:,:,1]**2 + Comm[:,:,2]**2

#print(CommutatorNormSquared)

CommL2 = np.sqrt(np.mean(CommutatorNormSquared))
print(CommL2)



# Relaxation of embedding functions:
#X = np.sin(theta)*np.cos(phi)
#Y = np.sin(theta)*np.sin(phi)
#Z = np.cos(theta)
X = np.zeros((Nth, Nphi, 3))
X[:,:,0] = np.sin(theta)*np.cos(phi)
X[:,:,1] = np.sin(theta)*np.sin(phi)
X[:,:,2] = np.cos(theta)

LapX = np.zeros_like(X)

#print("shape of x:", np.shape(X))




W = np.zeros_like(e_th)
# dt=2.e-4
dt=0.01
#Now, the relaxation:
plt.ion()
#image = plt.imshow(np.log(CommutatorNormSquared)/np.log(10.), vmin = -12)
image = plt.imshow(X[:,:,1])
plt.colorbar()
#plt.title("log_10(Squared Norm of Commutator)")
plt.show()

#fig = plt.figure()
#ax = fig.gca(projection='3d')
    
#ax.quiver(x, y, z, enew_th[:,:,0], enew_th[:,:,1], enew_th[:,:,2], length=.3, normalize=False, pivot='middle')
#ax.quiver(x, y, z, enew_ph[:,:,0], enew_ph[:,:,1], enew_ph[:,:,2], length=.3, normalize=False, color="red", pivot='middle')
#plt.show()



CommL2 = 100.
ethbkup = np.copy(e_th)
ephbkup = np.copy(e_ph)

Constraint = np.zeros((nth, nph, 2, 2))

angdiv = np.zeros_like(e_th)
residuals = []

for step in range(200000):
    Comm = thetaderiv(e_ph) - phideriv(e_th)

    if(step%100==0):
        ConstraintNormSquared = (Constraint[:,:,0,0])**2 + 2.*(Constraint[:,:,0,1])**2/np.sin(Theta)**2 + (Constraint[:,:,1,1]/np.sin(Theta)**2)**2
        ConstraintL2 = np.sqrt(np.mean(ConstraintNormSquared*np.sin(Theta)))
        print("Constraint norm:", ConstraintL2)
        CommutatorNormSquared = Comm[:,:,0]**2 + Comm[:,:,1]**2 + Comm[:,:,2]**2
        CommL2 = np.sqrt(np.mean(CommutatorNormSquared*np.sin(Theta)))
        residuals.append(CommL2)
        print(f"({step}) commutator:", CommL2, ", timestep:", dt)
        #image.set_data(np.log(CommutatorNormSquared)/np.log(10.))
        image.set_data(X[:,:,1])
        plt.draw()
        plt.pause(.001)
        if(dt < 1e-7): break
        
    if(step%20==0):
        ethbkup2 = np.copy(ethbkup)
        ephbkup2 = np.copy(ephbkup)
        ethbkup = np.copy(e_th)
        ephbkup = np.copy(e_ph)
        CL2Prev = CommL2
        CommutatorNormSquared = Comm[:,:,0]**2 + Comm[:,:,1]**2 + Comm[:,:,2]**2
        CommL2 = np.sqrt(np.mean(CommutatorNormSquared*np.sin(Theta)))
        if(CommL2 > CL2Prev and dt>1.e-6 and step>1000 and False):
            e_th = np.copy(ethbkup2)
            e_ph = np.copy(ephbkup2)
            dt *= .5
            print("\n#################### Retracting timestep. ####################\n")
        else:
            dt *= 1.0#1.001


    Constraint[:,:,0,0] = np.sum(e_th*e_th, axis=2) - g_thth
    Constraint[:,:,0,1] = np.sum(e_th*e_ph, axis=2) - g_thph
    Constraint[:,:,1,0] = np.sum(e_th*e_ph, axis=2) - g_thph
    Constraint[:,:,1,1] = np.sum(e_ph*e_ph, axis=2) - g_phph

    Comm_theta = (1./sqrtdetg)*thetaderiv(sqrtdetg*Comm)
    Comm_phi = (1./sqrtdetg)*phideriv(sqrtdetg*Comm)
    W[:,:,0] = Comm_theta[:,:,1]*e_ph[:,:,2] - Comm_theta[:,:,2]*e_ph[:,:,1] - (Comm_phi[:,:,1]*e_th[:,:,2] - Comm_phi[:,:,2]*e_th[:,:,1]) 
    W[:,:,1] = Comm_theta[:,:,2]*e_ph[:,:,0] - Comm_theta[:,:,0]*e_ph[:,:,2] - (Comm_phi[:,:,2]*e_th[:,:,0] - Comm_phi[:,:,0]*e_th[:,:,2]) 
    W[:,:,2] = Comm_theta[:,:,0]*e_ph[:,:,1] - Comm_theta[:,:,1]*e_ph[:,:,0] - (Comm_phi[:,:,0]*e_th[:,:,1] - Comm_phi[:,:,1]*e_th[:,:,0]) 

    ethnew[:,:,0] = e_th[:,:,0] - dt*(W[:,:,1]*e_th[:,:,2] - W[:,:,2]*e_th[:,:,1])
    ethnew[:,:,1] = e_th[:,:,1] - dt*(W[:,:,2]*e_th[:,:,0] - W[:,:,0]*e_th[:,:,2])
    ethnew[:,:,2] = e_th[:,:,2] - dt*(W[:,:,0]*e_th[:,:,1] - W[:,:,1]*e_th[:,:,0])
    ephnew[:,:,0] = e_ph[:,:,0] - dt*(W[:,:,1]*e_ph[:,:,2] - W[:,:,2]*e_ph[:,:,1])
    ephnew[:,:,1] = e_ph[:,:,1] - dt*(W[:,:,2]*e_ph[:,:,0] - W[:,:,0]*e_ph[:,:,2])
    ephnew[:,:,2] = e_ph[:,:,2] - dt*(W[:,:,0]*e_ph[:,:,1] - W[:,:,1]*e_ph[:,:,0])

    alpha = 1.#*np.sin(Theta)**2
    
    for i in range(3):
        ethnew[:,:,i] -= alpha*dt*Constraint[:,:,0,0]*e_th[:,:,i] + alpha*dt*Constraint[:,:,0,1]*e_ph[:,:,i]/np.sin(Theta)**2
        ephnew[:,:,i] -= alpha*dt*Constraint[:,:,1,0]*e_th[:,:,i] + alpha*dt*Constraint[:,:,1,1]*e_ph[:,:,i]/np.sin(Theta)**2
        

    e_th = np.copy(ethnew)
    e_ph = np.copy(ephnew)

    # Now, the update of the embedding functions:
    for i in range(3):
        angdiv[:,:,i] = thetaderiv(e_th[:,:,i]) + (np.cos(theta)/np.sin(theta))*e_th[:,:,i] + (1./np.sin(theta)**2)*phideriv(e_ph[:,:,i])
        LapX[:,:,i] = thetaderiv(thetaderiv(X[:,:,i])) + (np.cos(theta)/np.sin(theta))*thetaderiv(X[:,:,i]) + (1./np.sin(theta)**2)*phideriv(phideriv(X[:,:,i]))
    X += dt*(LapX - angdiv)
        # This is hacky. I should implement second-derivative operators.
    
    
plt.ioff()

fig = plt.figure()
ax = fig.gca(projection='3d')
    
#ax.quiver(x, y, z, e_th[:,:,0], e_th[:,:,1], e_th[:,:,2], length=.3, normalize=False, pivot='middle')
ax.quiver(x[::stride,::stride], y[::stride,::stride], z[::stride,::stride], e_th[::stride,::stride,0], e_th[::stride,::stride,1], e_th[::stride,::stride,2], length=.3, normalize=False, pivot='middle')
#ax.quiver(x, y, z, e_ph[:,:,0], e_ph[:,:,1], e_ph[:,:,2], length=.3, normalize=False, color="red", pivot='middle')
ax.quiver(x[::stride,::stride], y[::stride,::stride], z[::stride,::stride], e_ph[::stride,::stride,0], e_ph[::stride,::stride,1], e_ph[::stride,::stride,2], length=.3, normalize=False, color="red", pivot='middle')
plt.show()


fig = plt.figure()
ax = fig.gca(projection='3d')
    
#ax.quiver(aa*x, bb*y, cc*z, e_th[:,:,0], e_th[:,:,1], e_th[:,:,2], length=.3, normalize=False, pivot='middle')
ax.quiver(aa*x[::stride,::stride], bb*y[::stride,::stride], cc*z[::stride,::stride], e_th[::stride,::stride,0], e_th[::stride,::stride,1], e_th[::stride,::stride,2], length=.3, normalize=False, pivot='middle')
#ax.quiver(aa*x, bb*y, cc*z, e_ph[:,:,0], e_ph[:,:,1], e_ph[:,:,2], length=.3, normalize=False, color="red", pivot='middle')
ax.quiver(aa*x[::stride,::stride], bb*y[::stride,::stride], cc*z[::stride,::stride], e_ph[::stride,::stride,0], e_ph[::stride,::stride,1], e_ph[::stride,::stride,2], length=.3, normalize=False, color="red", pivot='middle')
plt.xlim(-2,2)
plt.ylim(-2,2)
ax.set_zlim(-2,2)
plt.show()


fig = plt.figure()
ax = fig.gca(projection='3d')
ax.quiver(X[::stride,::stride,0], X[::stride,::stride,1], X[::stride,::stride,2], e_th[::stride,::stride,0], e_th[::stride,::stride,1], e_th[::stride,::stride,2], length=.3, normalize=False, pivot='middle')
ax.quiver(X[::stride,::stride,0], X[::stride,::stride,1], X[::stride,::stride,2], e_ph[::stride,::stride,0], e_ph[::stride,::stride,1], e_ph[::stride,::stride,2], length=.3, normalize=False, color="red", pivot='middle')
plt.xlim(-2,2)
plt.ylim(-2,2)
ax.set_zlim(-2,2)
plt.show()

residuals = np.array(residuals)
plt.plot(np.log(residuals))
plt.show()
