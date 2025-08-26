#!/usr/bin/env python3

# Christoph Lechner, Aug 2025
# Demo for automatic testing: reports FEL power at the end of the simulated beamline

import h5py as h5

f = h5.File('Example1.out.h5','r')
P = f['/Field/power'][()]
f.close()

Pfinal = P[-1]
Pfinal = Pfinal[0]
print(f'FEL power at the end of Example1 run: {Pfinal/1e9:.6f}GW')
