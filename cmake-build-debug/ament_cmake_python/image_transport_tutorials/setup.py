from setuptools import find_packages
from setuptools import setup

setup(
    name='galaxy_camera',
    version='0.0.0',
    packages=find_packages(
        include=('galaxy_camera', 'galaxy_camera.*')),
)
