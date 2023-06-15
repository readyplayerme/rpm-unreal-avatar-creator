# Sample Structure

## Avatar Creator State Machine
The avatar creator is the topmost widget that contains all the component sub-widgets: navigation panel, avatar editor, editor setup widgets as well as the loading screens and popups.
It changes the visibility of the sub-widgets at runtime by using a simple state machine logic.
It also creates an instance of the **AvatarCreatorApi** class and provides it to the child widgets. **AvatarCreatorApi** class handles all the server requests.

## Screens
1. **Login** - Login by sending a one-time code to the user's email address.<br>
   ![Screenshot 2023-06-15 131415](https://github.com/readyplayerme/rpm-unreal-avatar-creator/assets/3124894/6b19934c-d7fd-4f78-b050-cab75c410269)
2. **User Avatars** - Select/edit/delete the previously created avatar from the list of avatars.<br>
   ![Screenshot 2023-06-15 131508](https://github.com/readyplayerme/rpm-unreal-avatar-creator/assets/3124894/b2d256e8-bce8-46ea-8406-3be7c99588c8)
3. **Gender Selection** - Select gender of the avatar.<br>
   ![Screenshot 2023-06-15 131027](https://github.com/readyplayerme/rpm-unreal-avatar-creator/assets/3124894/960d8ca3-5ebe-4349-a833-8193cebf1db8)
4. **BodyType Selection** - Select the body type of the avatar - full-body or half-body.<br>
   ![Screenshot 2023-06-15 130738](https://github.com/readyplayerme/rpm-unreal-avatar-creator/assets/3124894/0d3a6bc0-3157-4331-a18d-a23cb1f421c3)
5. **Avatar Templates** - Select the avatar template from the list.<br>
   ![Screenshot 2023-06-15 131216](https://github.com/readyplayerme/rpm-unreal-avatar-creator/assets/3124894/30633ac4-a582-48d0-b196-b8d3ec7f056f)
6. **Selfie Suggestion** - Landing page for taking a selfie and using it to generate the avatar.
   ![Screenshot 2023-06-15 131051](https://github.com/readyplayerme/rpm-unreal-avatar-creator/assets/3124894/466bc705-1ebe-4529-99d3-c1b24a194dfb)
7. **Take Selfie** - Opens webcam and allows one to take a photo.
   ![Screenshot 2023-06-15 131145](https://github.com/readyplayerme/rpm-unreal-avatar-creator/assets/3124894/8344793e-212c-4d59-85a0-7ef71fe1bfaf)
8. **Avatar Editor** - Avatar editor, allows customization of the avatar.
   ![Screenshot 2023-06-15 131259](https://github.com/readyplayerme/rpm-unreal-avatar-creator/assets/3124894/466ac898-52e1-483d-8ddd-ac938674b8a1)

## Navigation Panel
The navigation panel contains the following buttons needed for the navigation between the scenes:
**Back**, **Save**, **Profile** and optional **Close**. Their visibility is automatically toggled depending on the state of the avatar creator.
![Screenshot 2023-06-15 140631](https://github.com/readyplayerme/rpm-unreal-avatar-creator/assets/3124894/ac7a57f9-bc05-4a64-9213-4639f52e1df9)

## Navigation Switcher
In order for the child widgets to be able to communicate with the avatar creator, they use an interface of the AvatarSwitcher.
Avatar creator implements the NavigationSwitcher interface allowing the child widgets to send events.

## Preview Actor
The preview actor is spawned in the world when the avatar creator widget is opened. It contains one half and one full-body avatar.
It also contains lights and a camera for rendering the avatar at runtime and displaying it in the avatar editor afterward.

## Popups
1. **Loading Screen** - Blocker screen that opens while the resources for the avatar editor are being downloaded.
2. **Warning View** - Opens when a server request fails.
3. **Preview Loading View** - Toggles a popup while the avatar is being updated, after the outfit change.
4. **Save View** - Opens while the avatar is being saved on the server.
5. **Signup View** - **Ready Player Me** signup suggestion popup that opens after the avatar gets saved.

## Avatar Editor
The editor widget contains several sub-widgets and handles the logic and UI of the avatar editing process.

### Avatar Renderer
AvatarRenderer Widget is a helper widget that creates and manages the render actor.

### Panels
The editor contains scroll boxes that contain buttons for avatar customization.

1. **Outfit Category ScrollBox** - Contains buttons for different asset-type - face, hairstyle, outfit, glasses, face mask, face wear, and headwear.
   Clicking the buttons will open the respective asset-type panel.
2. **Outfit Assets ScrollBox** - Contains asset buttons for the selected outfit asset type.
3. **Face Category ScrollBox** - Contains asset type panels for face - face shape, eyes, eyebrows, nose, mouth, beard, etc.
4. **Face Assets Scrollbox** - Contains asset buttons for the selected face asset type.<br>
5. **Color ScrollBox** - Eye color, Skin color, Hair color, etc.

### Powered By Badge
**Powered By Badge** shows the brands of the asset providers for the outfits. It gets changed when the assets are changed.

### Editor Buttons
Editor buttons are included in custom widgets. While the category buttons are hardcoded inside of the editor, the asset and color buttons are added at runtime.

1. **Category Button** - For selecting an asset type. The thumbnail image is present on the path `Content/Icons`.
2. **Asset Button** - For selecting an asset with a thumbnail and a selection circle. The thumbnail image is fetched from the server.
3. **Color Button** - For selecting a color with a thumbnail and a selection circle.

