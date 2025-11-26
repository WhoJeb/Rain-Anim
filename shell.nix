{ pkgs ? import <nixpkgs> {} }:

# To enter shell run: nix-shell

pkgs.mkShell {
  # use “packages" instead of buildInputs
  packages = with pkgs; [
    libGL # OpenGL library
    libGLU # OpenGL Utility library
    glfw # GLFW windowing library
    glew # GLEW OpenGL Extension Wrangler
    # Or glad, if preferred:
    # glad
    glm # OpenGL Math Module
    # sdl2 # SDL2 windowing library (alternative to GLFW)
  ];

  # pkg-config must be in nativeBuildInputs
  nativeBuildInputs = with pkgs; [
    gcc # C/C++ Compiler
    libclang # C/C++ Compiler
    cmake # Build system
    pkg-config
  ];

  # Set environment variables for OpenGL driver paths if needed,
  # especially if encountering issues with finding drivers.
  # This might be necessary if your system-wide OpenGL setup isn't fully propagated.
  shellHook = ''
    export SHELL=${pkgs.zsh}/bin/zsh
    exec $SHELL

    export LD_LIBRARY_PATH="/run/opengl-driver/lib:$LD_LIBRARY_PATH"
    # For 32-bit support, if needed:
    # export LD_LIBRARY_PATH="/run/opengl-driver-32/lib:$LD_LIBRARY_PATH"
  '';
}
