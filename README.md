# Ready Player Me Avatar Creator

![Screenshot_20230221_044146](https://user-images.githubusercontent.com/3124894/229149244-df433bd5-b8f3-40ba-b87f-ce071ecdc773.png)

Ready Player Me Avatar Creator is an extension to www.readyplayer.me avatar platform, which helps you create avatars.

RpmAvatarCreator plugin is an in-engine implementation of the web avatar creator.

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

There are three ways you can add the **RpmAvatarCreator** plugin to your project. Add as submodules, paste the plugin inside of the **Plugins** folder, or add the blueprint version of the plugin to the Engine.

- To add the plugin to your project as a submodules, run the following command in the terminal from your project folder. This is preferred way of setting up the plugin.

  ```
   git submodule add --name Plugins/RpmAvatarCreator -- https://github.com/readyplayerme/rpm-unreal-avatar-creator.git Plugins/RpmAvatarCreator
  ```

- To clone the plugin into your project, run the following command in the terminal from your project folder.

- To paste the plugin inside of the Plugins folder, create a **Plugins** folder from the root of your project, download the latest tags of the [RpmAvatarCreator](https://github.com/readyplayerme/rpm-unreal-avatar-creator.git) plugins into it. Rename the **rpm-unreal-avatar-creator** plugin folder to the **RpmAvatarCreator**.

- To add the blueprint version of the plugin, find the plugin attached in the [Latest Release](https://github.com/readyplayerme/rpm-unreal-avatar-creator/releases/latest), add it to the Plugins/Runtime folder of the installed Unreal Engine.

### Update RpmAvatarCreator plugin submodule

To update the plugin submodule paste the following command in the terminal from your project folder. This will fetch and switch the plugin repositories to the latest stable release.

  ```
  cd Plugins/RpmAvatarCreator ; git fetch --tags ; git checkout $(git describe --tags $(git rev-list --tags --max-count=1)) ; cd ../..
  ```

## Quick Start

A demo map is included in the plugin for demonstrating how the **Sample Avatar Creator** opens at runtime. It is located in the `RpmAvatarCreator\Content\Maps` folder.
To add the AvatarCreator widget to your project simply copy the existing blueprint logic from the demo map into your project.

**AvatarCreator** is a widget that can be added to a map or another widget.
Inside the AvatarCreatorDemo map we create and add the widget to the viewport.
**AvatarCreator** can be customized by providing default values when the widget is created.
We need to subscribe to the **Avatar Saved** event to get the URL when the avatar is saved.
We need to subscribe to the **Avatar Selected** event as well to get the URL when the avatar is selected from the list of user avatars.
Additionally, we can load an avatar with this URL afterward.

![Screenshot_20230221_060932](https://user-images.githubusercontent.com/3124894/229149539-1ae0e7a5-c9ae-4671-b6e1-e267e14fd204.png)

## Customization Options

### Use the Sample project
When spawning the **Avatar Creator** widget, it's possible to configure it with the specified parameters.

Customization options:
 - **Partner Domain** Specify your partner domain
 - **Select Body Type** Allows skipping the body type selection screen
 - **Select Gender** Allows skipping the gender selection screen
 - **Allow Close Button** Hides the close button
 - **Allow Webcam** Enables the selfie selection screen if the webcam is available
 - **Default Avatar Ids** Specify the avatar ids that will be shown on the **Pick An Avatar** page. **Important** this field needs to be filled with the avatar ids from the demo map.

### Duplicate The Sample
The plugin **Content** represents a sample project, if you want to have a completely different UI, you can duplicate the sample project and change the UI.
In this case, you will **not** be able to get the latest changes of this sample project UI.

### UI From Scratch
**Avatar Creator** sample uses **AvatarCreatorApi** C++ class for communicating with the **Ready Player Me** Backend. The plugin **Content** can be completely rewritten to suit your project needs while using the **AvatarCreatorApi** for communication with the Backend.

## TODO

- Fix the webcam support for mobile
- Improve the UI
- Add shadow for the avatar in the avatar editor
- Add a sample for the VR

## Links
- [Documentation](https://docs.readyplayer.me/ready-player-me/integration-guides/unreal-engine)
- [Support](https://docs.readyplayer.me/ready-player-me/integration-guides/unreal-engine/troubleshooting)
