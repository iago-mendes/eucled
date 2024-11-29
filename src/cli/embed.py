import click
import subprocess
import sys

@click.command()
@click.option(
  '--file',
  '-f',
  'file_path',
  type=click.Path(exists=True, file_okay=True, readable=True),
  required=True,
  help='H5 file with data for the 2-metric to be embedded.',
)
@click.option(
  '--horizon',
  '-h',
  'horizon_key',
  type=str,
  required=True,
  help='Key of the horizon in the H5 file.',
)
@click.option(
  '--observation',
  '-o',
  'observation_id',
  type=str,
  required=True,
  help='Observation ID in the H5 file.',
)
def embed(file_path, horizon_key, observation_id):
  """Run embedding algorithm on a numeric metric."""

  process = subprocess.Popen(
    ['eucled_numeric_embedding'],
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    stderr=subprocess.STDOUT,
    text=True
  )

  # TODO: check numeric data

  input_data = [
    file_path,
    horizon_key,
    observation_id,
  ]

  process.stdin.write('\n'.join(input_data) + '\n')
  process.stdin.flush()
  process.stdin.close()
  
  for line in process.stdout:
    sys.stdout.write(line)
    sys.stdout.flush()

  process.wait()

# @click.command('embed')
# @click.option(
#   '--resolution',
#   '-N',
#   'Ntheta',
#   type=int,
#   required=True,
#   help="Number of theta grid points.",
# )
# @click.option(
#   '--case',
#   '-C',
#   'case',
#   type=str,
#   required=True,
#   help=f'Which test case to run. Options: {", ".join(supported_cases)}.',
# )
# @click.option(
#   '--param',
#   '-P',
#   'param_list',
#   multiple=True,
#   default=None,
#   help=f'Parameters specific to the selected test case.',
# )
# @click.option(
#   '--file',
#   '-f',
#   'file_path',
#   type=click.Path(exists=True, file_okay=True, readable=True),
#   required=True,
#   help='H5 file with data for the 2-metric to be embedded.',
# )
# @click.option(
#   '--horizon',
#   '-h',
#   'horizon_key',
#   type=str,
#   required=True,
#   help='Key of the horizon in the H5 file.',
# )
# @click.option(
#   '--observation',
#   '-o',
#   'observation_id',
#   type=str,
#   required=True,
#   help='Observation ID in the H5 file.',
# )
# def embed_cli(file_path, horizon_key, observation_id):
#   resolution = {
#     'Ntheta': Ntheta,
#     'Nphi': 2*Ntheta
#   }

#   params = {}
#   for param in param_list:
#     key, value = param.split('=')
#     params[key] = value

#   test(case, params, resolution)
