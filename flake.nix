{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";

    kcli = {
      url = "github:krscott/kcli";
      inputs.nixpkgs.follows = "nixpkgs";
    };

    ktl = {
      url = "github:krscott/ktl";
      inputs.nixpkgs.follows = "nixpkgs";
    };

    ktest = {
      url = "github:krscott/ktest";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      kcli,
      ktl,
      ktest,
    }:
    let
      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];
    in
    flake-utils.lib.eachSystem supportedSystems (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};

        # Final derivation including any overrides made to output package
        inherit (self.packages.${system}) aoc aoc-gcc;

        devPkgs =
          with pkgs;
          [
            shfmt
            cmake-format
            clang-tools # NOTE: clang-tools must come before clang
            clang
          ]
          ++ aoc.buildInputs;

        mkApp = text: {
          type = "app";
          program = pkgs.lib.getExe (
            pkgs.writeShellApplication {
              name = "app";
              runtimeInputs = devPkgs;
              inherit text;
            }
          );
        };
      in
      {
        packages = {
          aoc = pkgs.callPackage ./. {
            inherit (kcli.packages.${system}) kcli;
            inherit (ktest.packages.${system}) ktest;
            inherit (ktl.packages.${system}) ktl;
            stdenv = pkgs.clangStdenv;
          };

          aoc-gcc = aoc.override {
            inherit (pkgs) stdenv;
          };

          aoc-win = aoc.override {
            inherit (pkgs.pkgsCross.mingwW64) stdenv;
          };

          default = aoc;

          aoc-test = aoc.override {
            doCheck = true;
          };
          aoc-gcc-test = aoc-gcc.override {
            doCheck = true;
          };
        };

        devShells = {
          default = pkgs.mkShell {
            inputsFrom = [ aoc ];
            nativeBuildInputs = devPkgs;
            shellHook = ''
              source dev_shell.sh
            '';
          };
        };

        apps = {
          format = mkApp ''
            ./format.sh
          '';
        };

        formatter = pkgs.nixfmt-rfc-style;
      }
    );
}
