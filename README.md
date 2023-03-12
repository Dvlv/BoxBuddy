# BoxBuddy
A Graphical manager for [Distrobox](https://github.com/89luca89/distrobox), enabling the creation and use of multiple linux distributions via containers.

## Screenshots
![screenshots/newbox.png](/screenshots/newbox.png)
![screenshots/manage.png](/screenshots/manage.png)
![screenshots/installedapps.png](/screenshots/installedapps.png)


## Installing

### Dependencies
- [Distrobox](https://github.com/89luca89/distrobox)
    - `Podman` or `Docker`

- One of the following Terminals:
    - Konsole
    - Gnome Terminal
    - XTerm

### Instructions
Simply clone or download a zip of this repo, and launch the `install.sh` script. 

The application will be installed to `~/.local/bin` by default, or you can export `INSTALL_DIR=/my/other/path` to change the destination.
Icons will be installed in `$XDG_DATA_HOME`, which is `~/.local/share` by default.

Ensure that the install destination is in your `$PATH`.


### Uninstalling
Simply delete the `boxbuddy` folder in your install destination and your `$XDG_DATA_HOME`, then remove the `boxbuddy.desktop` file from `~/.local/share/applications`.

## Developing

### Dependencies
All of the above mentioned dependencies, as well as:

- cmake
- qt development libraries

The easiest way to get these on most distros is to just install Qt Creator from your package manager.

### Building
Typical `cmake` workflow, e.g.:

    - `mkdir build`
    - `cd build`
    - `cmake ..`
    - `make`

A `go.sh` script is provided for convenience, but be aware this will run the `install.sh` script on every build

