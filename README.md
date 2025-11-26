# Rain Anim
## Compiling From Source
If somehow by some divine intervention you find this repo and are bat-shit insane enough actually want to compile it yourself, here are some things you should know:

  1. It is only easy to do on NixOS
  2. You will need all the dependancies in [shell.nix](https://github.com/WhoJeb/Rain-Anim/blob/main/shell.nix) to be installed globally or you will have to mess with the makefile#Installing-dependancies-globally and/or environment variables to point to the right spot

### NixOS

### General Linux
You've kinda got 2 options: 
  1. [Installing Dependancies Globally](#Installing-dependancies-globally)
  2. Messing with the [CMakeLists.txt](https://github.com/WhoJeb/Rain-Anim/blob/main/CMakeLists.txt) or environment variables

#### Installing dependancies globally
The dependancies you will likey have to install are contained in:
```nix
packages = with pkgs; [
  ...
];
```


### Windows
Don't...

---

# To Do
- [x] Instancing
- [x] Random x axis drift
- [ ] Y axis acceleration
- [ ] Abstract the code a little
