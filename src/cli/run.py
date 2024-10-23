import click

@click.command()
@click.argument('input_file')
def run(input_file):
  """Run embedding algorithm."""
  click.echo(f'Input file: {input_file}')
