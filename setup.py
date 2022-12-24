import setuptools

with open("README.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()

setuptools.setup(
    name="dim2xdf",
    version="0.0.1",
    author="tantanGH",
    author_email="tantanGH@github",
    license='MIT',
    description="DIM disk image to XDF image file converter",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/tantanGH/dim2xdf",
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    entry_points={
        'console_scripts': [
            'dim2xdf=dim2xdf.__main__:main'
        ]
    },
    packages=setuptools.find_packages(),
    python_requires=">=3.5",
)
