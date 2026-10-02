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
    # The makefiles default DEVKIT to /opt (upstream's convention). Locally we
    # keep the toolchains beside the checkout, so point DEVKIT at whichever one
    # matches the platform being built. Exported, because DEVKIT uses "?=" and
    # therefore honours the environment.
    export LGPT_TG5040_DEVKIT="''${LGPT_TG5040_DEVKIT:-$PWD/../toolchain/aarch64-nextui-linux-gnu}"
    export LGPT_TG5050_DEVKIT="''${LGPT_TG5050_DEVKIT:-$PWD/../toolchain/tg5050/aarch64-nextui-linux-gnu}"

    lgpt-build() {
      local plat="''${1:-TG5040}" devkit
      case "$plat" in
        TG5040) devkit="$LGPT_TG5040_DEVKIT" ;;
        TG5050) devkit="$LGPT_TG5050_DEVKIT" ;;
        *)      devkit="" ;;
      esac
      ( cd projects && make PLATFORM="$plat" ''${devkit:+DEVKIT="$devkit"} )
    }

    for p in TG5040 TG5050; do
      eval "d=\$LGPT_''${p}_DEVKIT"
      if [ -x "$d/bin/aarch64-nextui-linux-gnu-gcc" ]; then
        echo "$p toolchain: $d"
      else
        echo "$p toolchain NOT found at $d"
      fi
    done
    echo "build: lgpt-build TG5040   (or TG5050)"
  '';
}
