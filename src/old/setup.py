#!/usr/bin/env python

# TODO: Clean up this file. Most of the stuff is not used.
from distutils.core import setup
from distutils.dist import Distribution
from distutils.cmd import Command
from distutils.command.install_data import install_data
from distutils.command.build import build
from distutils.dep_util import newer
from distutils.log import warn, info, error, fatal
import glob, os, sys, subprocess, platform
from fleetingpmlib.Version import Version

class BuildData(build):
  def run(self):
    build.run(self)
    TOP_BUILDDIR='.'
    desktop_data='data/fleetingpm.desktop'

class Uninstall(Command):
  description  = "Attempt an uninstall from an install --record file"
  user_options = [('manifest=', None, 'Installation record filename')]

  def initialize_options(self):
    self.manifest = None

  def finalize_options(self):
    pass

  def get_command_name(self):
    return 'uninstall'

  def run(self):
    f = None
    self.ensure_filename('manifest')
    try:
      try:
        f = open(self.manifest)
        files = [file.strip() for file in f]
      except IOError, e:
        raise DistutilsFileError("Unable to open install manifest: %s", str(e))
    finally:
      if f:
        f.close()

    for file in files:
      if os.path.isfile(file) or os.path.islink(file):
        info("Removing %s" % repr(file))
        if not self.dry_run:
          try:
            os.unlink(file)
          except OSError, e:
            warn("Could not delete: %s" % repr(file))
      elif not os.path.isdir(file):
        info("Skipping %s" % repr(file))

    dirs = set()
    for file in reversed(sorted(files)):
      dir = os.path.dirname(file)
      if dir not in dirs and os.path.isdir(dir) and len(os.listdir(dir)) == 0:
        dirs.add(dir)
        # Only nuke empty Python library directories
        if dir.find("site-packages/") > 0:
          info("removing %s" % repr(dir))
          if not self.dry_run:
            try:
              os.rmdir(dir)
            except OSError, e:
              warn("Could not remove directory: %s" % str(e))
        else:
          info("Skipping empty directory %s" % repr(dir))

class InstallData(install_data):
  def run(self):
    install_data.run(self)

setup(name='Fleeting Password Manager',
      version=Version.verString,
      description='Fleeting Password Manager',
      author='Jussi Lind',
      author_email='jussi.lind@iki.fi',
      url='http://fleetingpm.sourceforge.net',
      license='GNU GPL v3',
      scripts=['fleetingpm'],
      data_files=[
                  ('share/applications', ['data/fleetingpm.desktop']),
                  ('share/pixmaps',      ['data/icons/fleetingpm.png']),
                  ('share/pixmaps',      ['data/icons/fleetingpm.svg'])
                 ],
      packages=['fleetingpmlib'],
      cmdclass={'build': BuildData, 'install_data': InstallData, 'uninstall': Uninstall}
     )
