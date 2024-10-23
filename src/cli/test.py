import click

supported_test_cases = [
  'XPeanut'
]

def test(case, params):
  print(case)
  print(params)

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
