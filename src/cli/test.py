import click
import subprocess
import sys

supported_cases = [
  'Ellipsoid',
  'KerrHorizon',
  'Sphere',
  'XPeanut',
  'ZPeanut',
]

def test(case, params, resolution):
  """Run embedding algorithm on an analytic test case."""

  assert case in supported_cases, f'{case} is not a supported test case'

  process = subprocess.Popen(
    ['eucled_test_case'],
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    stderr=subprocess.STDOUT,
    text=True
  )

  # TODO: check params

  input_data = [
    resolution['Ntheta'],
    resolution['Nphi'],
    case,
    *params.values()
  ]
  input_data = [str(data) for data in input_data]

  process.stdin.write('\n'.join(input_data) + '\n')
  process.stdin.flush()
  process.stdin.close() 
  
  for line in process.stdout:
    sys.stdout.write(line)
    sys.stdout.flush()

  process.wait()

@click.command('test', help=test.__doc__)
@click.option(
  '--resolution',
  '-N',
  'Ntheta',
  type=int,
  required=True,
  help="Number of theta grid points.",
)
@click.option(
  '--case',
  '-C',
  'case',
  type=str,
  required=True,
  help=f'Which test case to run. Options: {", ".join(supported_cases)}.',
)
@click.option(
  '--param',
  '-P',
  'param_list',
  multiple=True,
  default=None,
  help=f'Parameters specific to the selected test case.',
)
def test_cli(Ntheta, case, param_list):
  resolution = {
    'Ntheta': Ntheta,
    'Nphi': 2*Ntheta
  }

  params = {}
  for param in param_list:
    key, value = param.split('=')
    params[key] = value

  test(case, params, resolution)
