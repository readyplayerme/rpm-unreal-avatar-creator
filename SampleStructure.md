# Sample Structure

## Avatar Creator State Machine
The avatar creator is the topmost widget that contains all the component subwidgets: navigation panel, avatar editor, editor setup widgets as well as the loading screens and popups.
It changes the visibility of the sub-widgets at runtime by using a simple state machine logic.
It also creates an instance of the **AvatarCreatorApi** class, and provides it to the child widgets. **AvatarCreatorApi** class handles all the server requests.

## Selection Screens(States)
1. **Login** - Login by sending a one time code to user's email address.
2. **User Avatars** - Select/edit/delete the previously created avatar from the list of avatars.
3. **Gender Selection** - Select gender of the avatar.
4. **BodyType Selection** - Select body type of the avatar - full-body or half-body.
5. **Avatar Templates** - Select the avatar template from the list.
6. **Selfie Suggestion** - Landing page for taking a selfie and use it to generate the avatar.
7. **Take Selfie** - Opens webcam and allows to take a photo.
8. **Avatar Editor** - Avatar editor, allows to customization the avatar.

## Navigation Panel
Navigation panel contains the following buttons needed for the navigation between the scenes:
**Back**, **Save**, **Profile** and optional **Close**. Their visibility is automatically toggled depending on the state of the avatar creator.

## Navigation Switcher
In order for the child widgets to be able to communicate with the avatar creator, they use an interface of the AvatarSwitcher.
Avatar creator implements the NavigationSwitcher interface allowing the child widgets to send events.

## Popups
1. **Loading Screen** - Blocker screen that opens while the resources for the avatar editor are being downloaded.
2. **Warning View** - Opens when a server request fails.
3. **Preview Loading View** - Toggles a popup while the avatar is being updated, after the outfit change.
4. **Save View** - Opens while the avatar is being saved in the server.
5. **Signup View** - **Ready Player Me** signup suggestion popup that opens after the avatar gets saved.

## Avatar Editor
The editor widget contains several subwidgets and handles the logic and UI of the avatar editing process.

### Avatar Renderer
AvatarRenderer Widget is a helper widget that creates and manages the render actor.

### Panels
The editor contains scroll-boxes that contain the buttons for avatar customization.

1. **Outfit Category ScrollBox** - Contains buttons for different asset-type - face, hairstyle, outfit, glasses, face mask, face wear and head wear.
   On click the buttons will open the respective asset-type panel.<br>
   <img src="https://github.com/readyplayerme/rpm-unity-sdk-avatar-creator/assets/1121080/f9a77994-5069-427f-b2d9-64ae4d6edaf9"  width="100" height="300">

2. **Outfit Assets ScrollBox** - Contains asset buttons for the selected outfit asset type.<br>
   <img src="https://github.com/readyplayerme/rpm-unity-sdk-avatar-creator/assets/1121080/ae7713b3-2958-4d54-a433-570b5505372c"  width="100" height="300">

3. **Face Category ScrollBox** - Contains asset type panels for face - face shape, eyes, eyebrows, nose, mouth, beard, etc.<br>
   <img src="https://github.com/readyplayerme/rpm-unity-sdk-avatar-creator/assets/1121080/ef2fe745-02aa-4647-8274-dd84c83b076b" width="100" height="300">

4. **Face Assets Scrollbox** - Contains asset buttons for the selected face asset type.<br>

5. **Color ScrollBox** - Eye color, Skin color, Hair color, etc.<br>
   <img src="https://github.com/readyplayerme/rpm-unity-sdk-avatar-creator/assets/1121080/56baf347-8053-4ce0-852a-cd2aeaf1d5b6"  width="100" height="250">
   <img src="https://github.com/readyplayerme/rpm-unity-sdk-avatar-creator/assets/1121080/55a360d2-15e2-43c1-b92a-c8dfb2becf0e"  width="100" height="250">

### Powered By Badge
**Powered By Badge** shows the brands of the asset providers for the outfits. It gets changed when the assets are changed.

### Editor Buttons
1. **Category Button** - For selecting an asset type. The thumbnail image are present on path `Content/Icons`.<br>
   <img src="https://github.com/readyplayerme/rpm-unity-sdk-avatar-creator/assets/1121080/87d1cc45-c5c5-4044-a26f-0b9ee73c0596"  width="400" height="250">

2. **Asset Button** - For selecting an asset with thumbnail and a selection circle. The thumbnail image is fetched from the server.

3. **Color Button** - For selecting a color with thumbnail and a selection circle.
   <img src="https://github.com/readyplayerme/rpm-unity-sdk-avatar-creator/assets/1121080/24abe1b2-60f5-482c-9d1a-60ca9c8e622c"  width="50" height="50">

