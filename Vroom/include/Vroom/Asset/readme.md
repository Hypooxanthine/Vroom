# Roadmap of asset system

"Assets": a class that manages assets, keeping track of instances for memory management. It loads/unloads data at runtime using a scheduling mechanism. If running out of memory, it unloads old and unused assets. When using an asset, it is instantiated again if already loaded; if not loaded, it is loaded at runtime and then instantiated. Unused assets are not immediately unloaded, as they may be used again soon. They will only be unloaded if the manager runs out of memory, and the unused asset is the oldest and least recently used.

"AssetData": I'm still not sure if inheritance would be useful here. Classes of this type store the actual data of assets (data structures).
Examples:

    - TextureData: stores a texture.
    - SpriteData: stores a sprite, including information such as the associated texture, color mask, subrect of texture, scaling, and rotation.

"StaticAsset": an abstract class that stores data of its corresponding asset. It instantiates "DynamicAssets" based on its type and keeps track of the number of instantiated DynamicAssets.

    - TextureSAsset: stores TextureData and instantiates Textures (DynamicAsset).
    - SpriteSAsset: stores SpriteData and instantiates Sprites (DynamicAsset).

"DynamicAsset": an abstract class that only stores a reference to the actual data (AssetData). It notifies its parent StaticAsset when destructed.

    - Texture: stores a const reference to TextureData.
    - Sprite: stores a const reference to SpriteData.

In the future, we will introduce rendering components, such as SpriteComponent. It will store a Sprite instance (DynamicAsset) and have const access to SpriteData. SpriteData stores a Texture instance (DynamicAsset), which has const access to TextureData. Therefore, a SpriteComponent will have const access to everything it needs to render a sprite. Simultaneously, the Assets (asset manager) can keep track of instances of these Assets (Texture & Sprite).
