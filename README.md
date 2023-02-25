# Ready Player Me Avatar Creator

![Screenshot 2023-02-02 141147](https://user-images.githubusercontent.com/3124894/216334496-96b035ce-d6dc-4609-a43a-f53cf8722a92.png)

Ready Player Me Avatar Creator is an extension to www.readyplayer.me avatar platform, which helps you create avatars.

Please visit the online documentation and join our public `discord` community.

![](https://i.imgur.com/zGamwPM.png) **[Online Documentation]( https://docs.readyplayer.me/ready-player-me/integration-guides/unreal-sdk )**

![](https://i.imgur.com/FgbNsPN.png) **[Discord Channel]( https://discord.gg/9veRUu2 )**

## Dependencies
- **ReadyPlayerMe** Unreal SDK, an open source plugin that contains all the core functionality required for loading and displaying avatars.
  The plugin can be found on GitHub [here](https://github.com/readyplayerme/rpm-unreal-sdk).
- **glTFRuntime** An open source third party plugin with functionality for downloading and importing assets from GLTF and GLB files.
  The plugin can be found on GitHub [here](https://github.com/rdeioris/glTFRuntime), but can also be purchased from the Unreal Marketplace.

### Requirements

- Unreal Engine Version 4.27 or higher

## Integration

**Avatar Creator** plugin depends on the **Ready Player Me Unreal SDK**, follow the setup steps in the [Documentation](https://docs.readyplayer.me/ready-player-me/integration-guides/unreal-engine/quickstart) or the [Readme](https://github.com/readyplayerme/rpm-unreal-sdk/blob/master/README.md) for setting it up before adding the **RpmAvatarCreator** plugin.

### Add RpmAvatarCreator plugin

There are three ways you can add the **RpmAvatarCreator** plugin to your project. Add as submodules, clone the repo, or paste the plugin inside of the **Plugins** folder.

- To add the plugin to your project as a submodules, run the following command in the terminal from your project folder. This is preferred way of setting up the plugin.

  ```
   git submodule add --name RpmAvatarCreator -- https://github.com/readyplayerme/rpm-unreal-avatar-creator.git Plugins/RpmAvatarCreator
  ```

- To clone the plugin into your project, run the following command in the terminal from your project folder.

  ```
  git clone https://github.com/readyplayerme/rpm-unreal-avatar-creator.git Plugins/RpmAvatarCreator ; cd Plugins/RpmAvatarCreator ; git checkout $(git describe --tags $(git rev-list --tags --max-count=1)) ; cd ../..
  ```

- To paste the plugin inside of the Plugins folder, create a **Plugins** folder from the root of your project, download the latest tags of the [RpmAvatarCreator](https://github.com/readyplayerme/rpm-unreal-avatar-creator.git) plugins into it. Rename the **rpm-unreal-avatar-creator** plugin folder to the **RpmAvatarCreator**.

### Update RpmAvatarCreator plugin

To update the plugin paste the following command in the terminal from your project folder. This will fetch and switch the plugin repositories to the latest stable release.

  ```
  cd Plugins/RpmAvatarCreator ; git fetch --tags ; git checkout $(git describe --tags $(git rev-list --tags --max-count=1)) ; cd ../..
  ```

## Quick Start

A demo map is included in the plugin for demonstrating how the **Sample Avatar Creator** opens at runtime. It is located in the `RpmAvatarCreator\Content\Maps` folder.

## Show Avatar Creator

AvatarCreator is a widget that can be added to the map.
Inside of the DemoMap we create and add the widget to the viewport.
**AvatarCreator** can be customized by providing default values when the widget is created.
We need to subscribe to the **Avatar Saved** event to get the url when the avatar is saved.
Additionally we can load an avatar with this url.

![Screenshot 2023-02-02 141254](https://user-images.githubusercontent.com/3124894/216334568-a64b674c-f9b8-4da7-a74e-4ba3249f8798.png)

## Customization Options

### Use the Sample project
When spawning the **Avatar Creator** widget, it's possible to configure it with the specified parameters.

### Duplicate The Sample
The plugin **Content** represents a sample project, if you want to have a completely different UI, you can duplicate the sample project and change the UI.
In this case you **will not** be able to get the latest changes of this sample project UI.

### UI From Scratch
**Avatar Creator** sample uses **AvatarCreatorApi** C++ class for communicating with the **Ready Player Me** Backend. The plugin **Content** can be completely rewritten to suit your project needs, while using the **AvatarCreatorApi** for the communication with the Backend.

## TODO

- Add color selection in the avatar editor
- Add **Chose your avatar** page
- Add ability to log-in and create a user
- Add shadow for the avatar in the avatar editor
- Add a sample for the VR
- Add support for editing existing avatars

## Links
- [Documentation](https://docs.readyplayer.me/ready-player-me/integration-guides/unreal-engine)
- [Support](https://docs.readyplayer.me/ready-player-me/integration-guides/unreal-engine/troubleshooting)
