# Ready Player Me Avatar Creator

![Screenshot 2023-02-02 141147](https://user-images.githubusercontent.com/3124894/216334496-96b035ce-d6dc-4609-a43a-f53cf8722a92.png)

Ready Player Me Avatar Creator is an extension to www.readyplayer.me avatar platform, which helps you create avatars.

# Integration

- Add **RpmAvatarCreator** plugin as a submodule to your existing project using the command below. Alternatively copy the plugin in your project under the **Plugins** folder and name it **RpmAvatarCreator**.
  ```
   git submodule add --name RpmAvatarCreator -- https://github.com/readyplayerme/rpm-unreal-avatar-creator.git Plugins/RpmAvatarCreator
  ```
- Enable the **RpmAvatarCreator** plugin from the PluginManager.
- Open the **DemoMap** that is located in the `RpmAvatarCreator\Content\Maps` folder.
- Run the game.

## Show Avatar Creator

AvatarCreator is a widget that can be added to the map.
Inside of the DemoMap we create and add the widget to the viewport.
**AvatarCreator** can be customized by providing default values when the widget is created.
We need to subscribe to the **Avatar Saved** event to get the url when the avatar is saved.
Additionally we can load an avatar with this url.

![Screenshot 2023-02-02 141254](https://user-images.githubusercontent.com/3124894/216334568-a64b674c-f9b8-4da7-a74e-4ba3249f8798.png)

## TODO

- Add color selection in the avatar editor
- Add **Chose your avatar** page
- Add ability to log-in and create a user
- Add photo and image upload functionality
- Add shadow for the avatar in the avatar editor
- Add a sample for the VR

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
