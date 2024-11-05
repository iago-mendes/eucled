import click
import subprocess
import sys

supported_test_cases = [
  'XPeanut'
]

def test(case, params, resolution):
  assert case in supported_test_cases, f'{case} is not a supported test case'

  process = subprocess.Popen(
    ['eucled_test_case'],
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    stderr=subprocess.STDOUT,
    text=True
  )

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

@click.command('test')
@click.option(
  '--case',
  '-c',
  'case',
  type=str,
  required=True
)
@click.option(
  '--param',
  '-p',
  'params',
  multiple=True,
  default=None
)
def test_cli(case, params):
  test(case, params)
