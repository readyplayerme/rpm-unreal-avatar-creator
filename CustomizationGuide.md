# Customization Guide

The same UI of the custom avatar creator is entirely made with blueprints.
This is done on purpose so that it would be easier to duplicate and customize it.

## Customization Steps
To make a custom UI for the avatar creator, you need to do the following steps:

1. Make sure that you have the `ReadyPlayerMe` and `RpmAvatarCreator` plugins set up for your project.
2. Make a folder in your project's content folder and Duplicate the files from the `RpmAvatarCreator` content folder into it.
3. Make sure that you can add the duplicated avatar creator widget to the viewport, and that there are no errors.
4. Read the SampleStructure.md readme file to better understand the components of the sample UI.
5. When making cosmetic changes to the duplicated UI, make sure that the public variables of the UI blueprints are not removed. All the public variables have blueprint logic connected with them.
6. If you still need to remove the public variables, make sure that you understand the blueprint logic connected with them.

## Up-To-Date Sample
While duplicating the sample you will not be able to have up-to-date changes from the sample UI.
To have an up-to-date version of the custom avatar creator, the following steps are required:
1. First follow the `RpmAvatarCreator` repository to be notified about the new releases.
2. Update the ReadyPlayerMe and RpmAvatarCreator plugins in your project to the latest released version.
3. Open the CHANGELOG.md readme file to check the changes.
4. Open the changed UI blueprint files that are mentioned in the readme.
5. Try to apply the UI changes to your duplicated custom version of the avatar creator.

## Customization Example
Currently, the sample is designed for the landscape mode.
If your application is made for portrait mode, you need to make your own custom version of the avatar creator UI.
First, duplicate the sample in your project. One by one open all the widgets that don't look good on the portrait mode.
Change the layout of the widgets, without removing the public variables.
Keep an eye on the RpmAvatarCreator repo, to update your local version of avatar creator if a new version is released.

## Constraints
You are allowed to change the entire UI, the only thing that you are required to have in your custom implementation is the **Ready Player Me sign-in** button and **Ready Player Me account-creation** ui.
This is a legal requirement from **ReadyPlayerMe**.
