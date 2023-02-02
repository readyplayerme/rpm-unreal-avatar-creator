# Ready Player Me Avatar Creator

![Screenshot 2023-02-02 141147](https://user-images.githubusercontent.com/3124894/216334496-96b035ce-d6dc-4609-a43a-f53cf8722a92.png)

Ready Player Me Avatar Creator is an extension to www.readyplayer.me avatar platform, which helps you create avatars .

# Integration

1. Copy the **RpmAvatarCreator** plugin in your project under the **Plugins** folder.
2. Enable the **RpmAvatarCreator** plugin from the PluginManager.
3. Open the **DemoMap** that is located in the `RpmAvatarCreator\Content\Maps` folder.
4. Run the game.

## Show Avatar Creator

AvatarCreator is a widget that can be added to the map.
Inside of the DemoMap we create and add the widget to the viewport.
**AvatarCreator** can be customized by providing default values when the widget is created.
We need to subsctibe to the **Avatar Saved** event to get the url when the avatar is saved.
Additionally we can load an avatar with this url.

![Screenshot 2023-02-02 141254](https://user-images.githubusercontent.com/3124894/216334568-a64b674c-f9b8-4da7-a74e-4ba3249f8798.png)

## Dependencies
- Ready Player Me Unreal SDK 
- glTFRuntime (Third party plugin)

## Links
**Ready Player Me Unreal SDK**
- [Documentation](https://docs.readyplayer.me/ready-player-me/integration-guides/unreal-engine-4)
- [Download](https://docs.readyplayer.me/ready-player-me/integration-guides/unreal-engine-4/unreal-plugin-download)
- [Support](https://docs.readyplayer.me/ready-player-me/integration-guides/unreal-engine-4/troubleshooting)

**glTFRuntime**
- [Documentation](https://github.com/rdeioris/glTFRuntime-docs/blob/master/README.md)
