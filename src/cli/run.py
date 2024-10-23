import click
import yaml
from pathlib import Path

from .test import test

@click.command()
@click.argument('input_file')
def run(input_file):
  """Run embedding algorithm."""
  # Resolve input file path
  input_file_path = Path(input_file).resolve()
  assert input_file_path.exists(), "Input file not found"

  # Load input file
  with open(input_file_path, 'r') as file:
    input_file_content = yaml.safe_load(file)
  
  if 'TestCase' in input_file_content:
    test_case = input_file_content['TestCase']
    test_params = None
    if 'TestParams' in input_file_content:
      test_params = input_file_content['TestParams']
    
    test(test_case, test_params)
