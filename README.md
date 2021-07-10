# [@FileMapBot](https://t.me/filemapbot)

[`@filemapbot`](https://t.me/filemapbot) `= f(`[`message`](https://core.telegram.org/bots/api#message)`)` `⟹` [`your last document id`](https://core.telegram.org/bots/api#document)

## Build

We need [`CMake`](https://cmake.org/download) build system and [`Conan`](https://conan.io) manager libraries.

Prepare the project for building in release mode:

```shell
cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" -B bin
```

Building together the application and tests:

```shell
cmake --build bin --target all
```  

After that, the main self-executable utility will appear in the `bin` directory under the name `app`. The tests can be
run by launching the `test` file located nearby.

## Containerization

We need [`Docker`](https://www.docker.com/) installed:

```shell
docker build . -t utility
```

After that, we can run the utility in the container (if needed):

```shell
docker run -it utility
```

## Deploy

The sample configuration already exists in the `.do` directory. To deploy the application, just click the button.

[![Deploy to DigitalOcean](https://www.deploytodo.com/do-btn-blue-ghost.svg)](https://cloud.digitalocean.com/apps/new?repo=https://github.com/demidko/filemapbot/tree/main)

