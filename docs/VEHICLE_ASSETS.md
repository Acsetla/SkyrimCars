# Vehicle asset candidates

The project needs two redistributable vehicle meshes:

1. Mercedes G-Class
2. Jeep Wrangler

## Candidate sources reviewed

### Mercedes
Sketchfab model: "Mercedes-Benz G-Class Free Download" by slowpoly.
License shown on the listing: CC Attribution (CC BY).
The model is about 1.5k triangles and is therefore a practical low-poly candidate for a Skyrim conversion.

### Jeep
Sketchfab model: "VEHICLE - JEEP WRANGLER" by Thcyrax.
License shown on the listing: CC Attribution (CC BY).
The listing reports separate body/wheel meshes and game-oriented files, which is useful for a vehicle conversion.

## Conversion policy

Do not commit downloaded source meshes until the exact license metadata has been captured and attribution is included in the distribution.

The final Skyrim asset must be converted to NIF with:
- optimized body mesh
- separate wheel nodes
- collision geometry
- seat/driver marker
- correct scale/orientation
- material and texture paths
- Havok-compatible collision setup

## Important

The repository will not use models from other Skyrim mods when those mods prohibit redistribution or require additional permissions. For example, current Nexus listings for existing drivable-car mods explicitly restrict asset use/upload in ways that prevent simply copying their meshes into this project.
