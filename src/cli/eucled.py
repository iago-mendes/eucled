import click
import os

from .run import run
from .test import test_cli
 
@click.group()
def eucled():
  """Euclidean Embedding Diagram (EuclED) code"""
  working_dir = os.getenv('WORKING_DIR')
  if working_dir:
    os.chdir(working_dir)

eucled.add_command(run)
eucled.add_command(test_cli)
 
if __name__ == '__main__':
  eucled()
