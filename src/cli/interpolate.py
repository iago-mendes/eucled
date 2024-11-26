import click
import h5py
import numpy as np
import re
from scipy.interpolate import griddata

supported_codes = [
  'SpECTRE',
]

@click.command()
@click.option(
  '--code',
  '-c',
  'code',
  type=str,
  required=True,
  help=f'Code that originated the data to be interpolated. Supported codes: {", ".join(supported_codes)}.',
)
@click.option(
  '--input',
  '-i',
  'input_path',
  type=click.Path(exists=True, file_okay=True, readable=True),
  required=True,
  help=f'H5 file with data to be interpolated.',
)
@click.option(
  '--output',
  '-o',
  'output_path',
  type=click.Path(),
  required=True,
  help=f'H5 file to output the interpolated data.',
)
def interpolate(code, input_path, output_path):
  code = code.lower()
  assert code.lower() in [code.lower() for code in supported_codes], f'{code} is not a supported interpolation origin'
  if (code == 'spectre'):
    interpolate_spectre(input_path, output_path)


def interpolate_spectre(input_path, output_path):
  with h5py.File(input_path, 'r') as input_file:
    with h5py.File(output_path, 'w') as output_file:

      horizon_keys = [key for key in input_file.keys() if re.match(r'ObservationAh[A-C]\.vol', key)]
      for horizon_key in horizon_keys:
        horizon_group = output_file.create_group(horizon_key)
        print(horizon_key)

        for observation_id in input_file[horizon_key].keys():
          observation_group = horizon_group.create_group(observation_id)
          print(observation_id)

          # Load surface inertial coordinates and 2-metric
          x = input_file[horizon_key][observation_id]['InertialCoordinates_x']
          y = input_file[horizon_key][observation_id]['InertialCoordinates_y']
          z = input_file[horizon_key][observation_id]['InertialCoordinates_z']
          g_theta_theta = input_file[horizon_key][observation_id]['SurfaceMetric_theta_theta']
          g_phi_phi = input_file[horizon_key][observation_id]['SurfaceMetric_phi_phi']
          g_theta_phi = input_file[horizon_key][observation_id]['SurfaceMetric_theta_phi']

          # Load number of grid points
          extents = input_file[horizon_key][observation_id]['total_extents']
          N_theta = int(extents[0])
          N_phi = int(extents[1])

          # Reshape and transpose data
          x = np.array(x).reshape((N_phi, N_theta)).T
          y = np.array(y).reshape((N_phi, N_theta)).T
          z = np.array(z).reshape((N_phi, N_theta)).T
          g_theta_theta = np.array(g_theta_theta).reshape((N_phi, N_theta)).T
          g_phi_phi = np.array(g_phi_phi).reshape((N_phi, N_theta)).T
          g_theta_phi = np.array(g_theta_phi).reshape((N_phi, N_theta)).T
          
          # Get relative coordinates by subtracting the center from the global
          # coordinates
          x = x - np.mean(x)
          y = y - np.mean(y)
          z = z - np.mean(z)
          
          # Get spherical coordinates
          r = np.sqrt(x**2 + y**2 + z**2)
          theta = np.arccos(z / r)
          phi = np.arctan2(y, x)
          phi = np.where(phi < 0, phi + 2*np.pi, phi)

          # Enforce periodic boundaries of phi coordinates by copying the grid in
          # 2pi intervals before and after the original grid
          phi = np.concatenate([phi - 2. * np.pi, phi, phi + 2. * np.pi], axis=1)
          theta = np.concatenate([theta, theta, theta], axis=1)
          g_theta_theta = np.concatenate([g_theta_theta, g_theta_theta, g_theta_theta], axis=1)
          g_phi_phi = np.concatenate([g_phi_phi, g_phi_phi, g_phi_phi], axis=1)
          g_theta_phi = np.concatenate([g_theta_phi, g_theta_phi, g_theta_phi], axis=1)

          # Create target grid by reducing its size until all theta values are
          # within the limits of the original grid
          target_theta = None
          min_theta = np.min(theta)
          max_theta = np.max(theta)
          while True:
            i = np.array(range(int(N_theta)))
            delta_theta = np.pi / N_theta
            target_theta = delta_theta * (i + 0.5)
            if target_theta[0] > min_theta and target_theta[-1] < max_theta:
                break
            else:
                N_theta = int(N_theta - 1)
          N_phi = 2*N_theta
          j = np.array(range(N_phi))
          delta_phi = 2. * np.pi / N_phi
          target_phi = delta_phi * j
          target_theta, target_phi = np.meshgrid(target_theta, target_phi, indexing='ij')
          target_grid = (target_theta, target_phi)

          # Interpolate data into the target grid
          original_points = (theta.flatten(), phi.flatten())
          target_g_theta_theta = griddata(original_points, g_theta_theta.flatten(), target_grid)
          target_g_phi_phi = griddata(original_points, g_phi_phi.flatten(), target_grid)
          target_g_theta_phi = griddata(original_points, g_theta_phi.flatten(), target_grid)

          # Output interpolated data
          observation_group.create_dataset('g_theta_theta', data=target_g_theta_theta)
          observation_group.create_dataset('g_phi_phi', data=target_g_phi_phi)
          observation_group.create_dataset('g_theta_phi', data=target_g_theta_phi)
        