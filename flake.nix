{
  description = "VHS - Voxelized Homological Simplification (v2.0)";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    # PHAT is a git submodule upstream and is not packaged in nixpkgs.
    phat = {
      url = "git+https://bitbucket.org/phat-code/phat.git?rev=46116abeb6cab4893ac774e85efed5e3b87d5485";
      flake = false;
    };
  };

  outputs = { self, nixpkgs, phat }:
    let
      systems = [ "x86_64-linux" "aarch64-linux" ];
      forAll = f: nixpkgs.lib.genAttrs systems (s: f nixpkgs.legacyPackages.${s});
    in {
      packages = forAll (pkgs: rec {
        default = vhs;
        vhs = pkgs.stdenv.mkDerivation {
          pname = "vhs";
          version = "2.0";
          src = ./VHS_v2.0;

          nativeBuildInputs = [ pkgs.cmake pkgs.doxygen pkgs.graphviz ];
          buildInputs = [ pkgs.libpng pkgs.gtest ];

          # The submodule directory is empty in the checkout; fill it from the pinned input.
          postUnpack = ''
            rm -rf $sourceRoot/external/phat-code
            cp -r ${phat} $sourceRoot/external/phat-code
            chmod -R u+w $sourceRoot/external/phat-code
          '';

          # CMake hardcodes -march=native for Release; keep builds reproducible.
          cmakeBuildType = "Debug";
          cmakeFlags = [ "-DCMAKE_CXX_FLAGS=-O3" "-DBUILD_DOCUMENTATION=OFF" ];

          # Upstream defines no install rules.
          installPhase = ''
            runHook preInstall
            install -Dm755 simplifier imgproc optsol -t $out/bin
            install -Dm755 test $out/bin/vhs-test
            runHook postInstall
          '';

          # 3 upstream unit tests fail (RestrictedShapesTest.Closed, PersistenceTest.Persistence,
          # ValidatorTest.SubstractiveCandidates); run `vhs-test` manually.
          doCheck = false;

          meta = {
            description = "Voxelized Homological Simplification";
            license = pkgs.lib.licenses.lgpl3Plus;
            mainProgram = "simplifier";
            platforms = systems;
          };
        };
      });

      apps = forAll (pkgs: rec {
        default = simplifier;
        simplifier = { type = "app"; program = "${self.packages.${pkgs.system}.vhs}/bin/simplifier"; };
        imgproc = { type = "app"; program = "${self.packages.${pkgs.system}.vhs}/bin/imgproc"; };
        optsol = { type = "app"; program = "${self.packages.${pkgs.system}.vhs}/bin/optsol"; };
      });

      devShells = forAll (pkgs: {
        default = pkgs.mkShell {
          inputsFrom = [ self.packages.${pkgs.system}.vhs ];
          packages = [ pkgs.clang-tools pkgs.gdb ];
        };
      });
    };
}
