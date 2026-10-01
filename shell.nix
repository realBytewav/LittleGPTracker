# Dev shell for cross-building Little Piggy Tracker for the TrimUI Brick
# (NextUI, platform "tg5040").
#
#   nix-shell --run 'cd projects && make PLATFORM=TG5040'
#
# The cross toolchain itself is NOT provided by Nix: it is the prebuilt
# NextUI/crosstool-NG toolchain plus TrimUI's SDK sysroot, fetched by
# tools/setup-toolchain.sh. Those are ordinary glibc binaries, which run here
# because the host has programs.nix-ld enabled.
{ pkgs ? import <nixpkgs> { } }:

pkgs.mkShell {
  name = "lgpt-tg5040";

  nativeBuildInputs = with pkgs; [
    gnumake
    (python3.withPackages (ps: [ ps.pillow ])) # sources/Resources/mkfont.py
    zip     # packaging the .pak
    rsync   # deploying to the device
    openssh # scp/ssh to the Brick
  ];

  shellHook = ''
    export LGPT_DEVKIT="''${LGPT_DEVKIT:-$PWD/../toolchain/aarch64-nextui-linux-gnu}"
    if [ -x "$LGPT_DEVKIT/bin/aarch64-nextui-linux-gnu-gcc" ]; then
      echo "tg5040 toolchain: $LGPT_DEVKIT"
    else
      echo "tg5040 toolchain NOT found at $LGPT_DEVKIT"
      echo "  run tools/setup-toolchain.sh, or set LGPT_DEVKIT"
    fi
    echo "build: cd projects && make PLATFORM=TG5040 DEVKIT=\$LGPT_DEVKIT"
  '';
}
