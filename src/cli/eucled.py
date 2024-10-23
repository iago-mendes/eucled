import click
from run import run
 
@click.group()
def eucled():
  """Euclidean Embedding Diagram (EuclED) code"""
  pass

eucled.add_command(run)
 
if __name__ == '__main__':
  eucled()
