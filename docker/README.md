# Docker

## Using docker-compose

You may build and test your changes by using the compose file:

    docker-compose -f ./docker/docker-compose.yml up

## Entering the build environment

While in the repository root, to enter the environment and be presented with a
shell:

    docker run -it --rm -u builder \
      -v $PWD:/workspace ghcr.io/KaruroChori/vs-fltk:build-env

This will mount your current directory as */workspace* inside the container.
Your username will be *builder*. By default, you will not have root privileges
(which are not necessary to build and test). However, you can use `sudo` if
you need to run `apt` or any other commands that require root access.

## Getting the image

You can pull the image manually:

    docker pull ghcr.io/KaruroChori/vs-fltk:build-env

If you use `docker-compose` or `docker run ...` the image will be pulled
automatically the first time.


